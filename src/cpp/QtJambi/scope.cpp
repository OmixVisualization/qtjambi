/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtCore/QMetaType>
#include "scope.h"
#include "java_p.h"
#include "qtjambilink_p.h"
#include "qtjambishell_p.h"

DoFinally::DoFinally(QtJambiUtils::Runnable&& _action) : action(std::move(_action))
{

}

DoFinally::~DoFinally(){
    QtJambiExceptionInhibitor __exnHandler;
    try{
        if(action)
            action();
    }catch(const JavaException& exn){
        if(DefaultJniEnvironment env{200}){
            __exnHandler.handle(env, exn, nullptr);
        }else qCWarning(DebugAPI::internalCategory, "%s", exn.what());
    } catch (const std::exception& e) {
        qCWarning(DebugAPI::internalCategory, "%s", e.what());
    } catch (...) {
    }
}

class QtJambiScopePrivate : public QVector<QtJambiUtils::Runnable>
{
public:
    QtJambiScopePrivate();
    virtual ~QtJambiScopePrivate();
    virtual QtJambiLink * link() const;
    virtual jobject getJavaObjectLocalRef(JNIEnv *env) const;
    static jobject getJavaObjectLocalRef(JNIEnv *env, QtJambiScope& scope){
        Q_ASSERT(scope.d);
        return scope.d->getJavaObjectLocalRef(env);
    }
};

namespace QtJambiPrivate{
jobject getJavaObjectLocalRef(JNIEnv *env, QtJambiScope& scope){
    return QtJambiScopePrivate::getJavaObjectLocalRef(env, scope);
}
}

QtJambiScopePrivate::QtJambiScopePrivate() : QVector<QtJambiUtils::Runnable>() {}
QtJambiScopePrivate::~QtJambiScopePrivate(){}
QtJambiLink *QtJambiScopePrivate::link() const { return nullptr; }
jobject QtJambiScopePrivate::getJavaObjectLocalRef(JNIEnv *) const { return nullptr; }

class QtJambiScopePrivateLink : public QtJambiScopePrivate
{
public:
    QtJambiScopePrivateLink(const QSharedPointer<QtJambiLink>& _link);
    QtJambiLink * link() const override;
    jobject getJavaObjectLocalRef(JNIEnv *env) const override;
private:
    QSharedPointer<QtJambiLink> m_link;
};

QtJambiScopePrivateLink::QtJambiScopePrivateLink(const QSharedPointer<QtJambiLink>& _link) : QtJambiScopePrivate(), m_link(_link) {}
QtJambiLink *QtJambiScopePrivateLink::link() const { return m_link.data(); }
jobject QtJambiScopePrivateLink::getJavaObjectLocalRef(JNIEnv *env) const { return m_link->getJavaObjectLocalRef(env); }

class QtJambiScopePrivateObject : public QtJambiScopePrivate
{
public:
    QtJambiScopePrivateObject(JNIEnv* env, jobject object);
    QtJambiLink * link() const override;
    jobject getJavaObjectLocalRef(JNIEnv *env) const override;
private:
    JNIEnv* m_env;
    jobject m_object;
};

QtJambiScopePrivateObject::QtJambiScopePrivateObject(JNIEnv* env, jobject object) : QtJambiScopePrivate(), m_env(env), m_object(object) {}
QtJambiLink *QtJambiScopePrivateObject::link() const {
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(m_env, m_object))
        return link.data();
    else return nullptr;
}

jobject QtJambiScopePrivateObject::getJavaObjectLocalRef(JNIEnv *) const { return m_env->NewLocalRef(m_object); }

QtJambiLinkScope::QtJambiLinkScope(const QSharedPointer<QtJambiLink>& _link)
 : QtJambiScope(*new QtJambiScopePrivateLink(_link)){

}

QtJambiLink * QtJambiLinkScope::link() const{
    return reinterpret_cast<QtJambiLink *>(relatedNativeID());
}

QtJambiScope::QtJambiScope() : d(nullptr) {}

QtJambiScope::QtJambiScope(QtJambiNativeID nativeId)
    : d(!nativeId ? nullptr : new QtJambiScopePrivateLink(QtJambiLink::fromNativeId(nativeId)))
{
}

QtJambiScope::QtJambiScope(JNIEnv *env, jobject object)
    : d(object ? new QtJambiScopePrivateObject(env, object) : nullptr)
{
}

QtJambiScope::QtJambiScope(QtJambiShell* shell)
    : d(nullptr)
{
    if(shell){
        if(QSharedPointer<QtJambiLink> link = static_cast<QtJambiShellImpl*>(shell)->link())
            d = new QtJambiScopePrivateLink(link);
    }
}

QtJambiScope::QtJambiScope(QtJambiScopePrivate& _d)
    :d(&_d)
{
}

QtJambiScope::~QtJambiScope(){
    if(d){
        while(!d->isEmpty()){
            try {
                d->takeLast()();
            }catch(const JavaException& exn){
                if(DefaultJniEnvironment env{200}){
                    exn.report(env);
                }
                else qCWarning(DebugAPI::internalCategory, "%s", exn.what());
            } catch (const std::exception& e) {
                qCWarning(DebugAPI::internalCategory, "%s", e.what());
            } catch (...) {
            }
        }
        delete d;
    }
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
void QtJambiScope::addDeletion(int metaTypeId, void* pointer){
    addFinalAction([metaTypeId,pointer](){QMetaType::destroy(metaTypeId, pointer);});
}
#else
void QtJambiScope::addDeletion(QMetaType metaType, void* pointer){
    addFinalAction([metaType,pointer](){metaType.destroy(pointer);});
}
#endif

void QtJambiScope::addObjectInvalidation(JNIEnv *env, jobject object, bool persistent){
    if(persistent){
        JObjectWrapper obj(env, object);
        addFinalAction([obj]() mutable {
            if(JniEnvironment env{200}){
                QtJambiExceptionInhibitor __exnHandler;
                try{
                    InvalidateAfterUse::invalidate(env, obj.object());
                    obj.clear(env);
                }catch(const JavaException& exn){
                    __exnHandler.handle(env, exn, nullptr);
                } catch (const std::exception& e) {
                    qCWarning(DebugAPI::internalCategory, "%s", e.what());
                } catch (...) {
                }
            }
        });
    }else{
        addFinalAction([env, object](){
            QtJambiExceptionInhibitor __exnHandler;
            try{
                InvalidateAfterUse::invalidate(env, object);
            }catch(const JavaException& exn){
                __exnHandler.handle(env, exn, nullptr);
            } catch (const std::exception& e) {
                qCWarning(DebugAPI::internalCategory, "%s", e.what());
            } catch (...) {
            }
        });
    }
}

void QtJambiScope::addObjectInvalidation(JNIEnv *env, QtJambiNativeID nativeId, bool persistent){
    if(persistent){
        addFinalAction([nativeId](){
            if(JniEnvironment env{200}){
                QtJambiExceptionInhibitor __exnHandler;
                try{
                    InvalidateAfterUse::invalidate(env, nativeId);
                }catch(const JavaException& exn){
                    __exnHandler.handle(env, exn, nullptr);
                } catch (const std::exception& e) {
                    qCWarning(DebugAPI::internalCategory, "%s", e.what());
                } catch (...) {
                }
            }
        });
    }else{
        addFinalAction([env, nativeId](){
            QtJambiExceptionInhibitor __exnHandler;
            try{
                InvalidateAfterUse::invalidate(env, nativeId);
            }catch(const JavaException& exn){
                __exnHandler.handle(env, exn, nullptr);
            } catch (const std::exception& e) {
                qCWarning(DebugAPI::internalCategory, "%s", e.what());
            } catch (...) {
            }
        });
    }
}

void QtJambiScope::addForcedObjectInvalidation(JNIEnv *env, jobject object, bool persistent){
    if(persistent){
        JObjectWrapper obj(env, object);
        addFinalAction([obj]() mutable {
            if(JniEnvironment env{200}){
                QtJambiExceptionInhibitor __exnHandler;
                try{
                    InvalidateAfterUse::forcedInvalidate(env, obj.object());
                    obj.clear(env);
                }catch(const JavaException& exn){
                    __exnHandler.handle(env, exn, nullptr);
                } catch (const std::exception& e) {
                    qCWarning(DebugAPI::internalCategory, "%s", e.what());
                } catch (...) {
                }
            }
        });
    }else{
        addFinalAction([env, object](){
            QtJambiExceptionInhibitor __exnHandler;
            try{
                InvalidateAfterUse::forcedInvalidate(env, object);
            }catch(const JavaException& exn){
                __exnHandler.handle(env, exn, nullptr);
            } catch (const std::exception& e) {
                qCWarning(DebugAPI::internalCategory, "%s", e.what());
            } catch (...) {
            }
        });
    }
}

void QtJambiScope::addForcedObjectInvalidation(JNIEnv *env, QtJambiNativeID nativeId, bool persistent){
    if(persistent){
        addFinalAction([nativeId](){
            if(JniEnvironment env{200}){
                QtJambiExceptionInhibitor __exnHandler;
                try{
                    InvalidateAfterUse::forcedInvalidate(env, nativeId);
                }catch(const JavaException& exn){
                    __exnHandler.handle(env, exn, nullptr);
                } catch (const std::exception& e) {
                    qCWarning(DebugAPI::internalCategory, "%s", e.what());
                } catch (...) {
                }
            }
        });
    }else{
        addFinalAction([env, nativeId](){
            QtJambiExceptionInhibitor __exnHandler;
            try{
                InvalidateAfterUse::forcedInvalidate(env, nativeId);
            }catch(const JavaException& exn){
                __exnHandler.handle(env, exn, nullptr);
            } catch (const std::exception& e) {
                qCWarning(DebugAPI::internalCategory, "%s", e.what());
            } catch (...) {
            }
        });
    }
}

void QtJambiScope::addFinalAction(QtJambiUtils::Runnable&& action){
    if(!d)
        d = new QtJambiScopePrivate();
    d->append(std::move(action));
}

QtJambiNativeID QtJambiScope::relatedNativeID() const{
    return d ? QtJambiNativeID(jlong(d->link())) : InvalidNativeID;
}

