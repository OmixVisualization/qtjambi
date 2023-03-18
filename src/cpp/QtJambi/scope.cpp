/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

DoFinally::DoFinally(std::function<void()> _action) : action(_action)
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
        }else qWarning("%s", exn.what());
    } catch (const std::exception& e) {
        qWarning("%s", e.what());
    } catch (...) {
    }
}

class QtJambiScopePrivate : public QVector<std::function<void()>>
{
public:
    QtJambiScopePrivate();
    virtual ~QtJambiScopePrivate();
    virtual QtJambiLink * link() const;
};

QtJambiScopePrivate::QtJambiScopePrivate() : QVector<std::function<void()>>() {}
QtJambiScopePrivate::~QtJambiScopePrivate(){}
QtJambiLink *QtJambiScopePrivate::link() const { return nullptr; }

class QtJambiScopePrivateLink : public QtJambiScopePrivate
{
public:
    QtJambiScopePrivateLink(const QSharedPointer<QtJambiLink>& _link);
    QtJambiLink * link() const override;
private:
    QSharedPointer<QtJambiLink> m_link;
};

QtJambiScopePrivateLink::QtJambiScopePrivateLink(const QSharedPointer<QtJambiLink>& _link) : QtJambiScopePrivate(), m_link(_link) {}
QtJambiLink *QtJambiScopePrivateLink::link() const { return m_link.data(); }

class QtJambiScopePrivateObject : public QtJambiScopePrivate
{
public:
    QtJambiScopePrivateObject(JNIEnv* env, jobject object);
    QtJambiLink * link() const override;
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
                else qWarning("%s", exn.what());
            } catch (const std::exception& e) {
                qWarning("%s", e.what());
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

void QtJambiScope::addObjectInvalidation(JNIEnv *env, jobject object, bool checkJavaOwnership, bool persistent){
    if(persistent){
        JObjectWrapper obj(env, object);
        addFinalAction([obj, checkJavaOwnership](){
            if(JniEnvironment env{200}){
                InvalidateAfterUse::invalidate(env, obj.object(), checkJavaOwnership);
            }
        });
    }else{
        addFinalAction([env, object, checkJavaOwnership](){
            InvalidateAfterUse::invalidate(env, object, checkJavaOwnership);
        });
    }
}

void QtJambiScope::addObjectInvalidation(JNIEnv *env, QtJambiNativeID nativeId, bool persistent){
    if(persistent){
        addFinalAction([nativeId](){
            if(JniEnvironment env{200}){
                InvalidateAfterUse::invalidate(env, nativeId);
            }
        });
    }else{
        addFinalAction([env, nativeId](){
            InvalidateAfterUse::invalidate(env, nativeId);
        });
    }
}

void QtJambiScope::addFinalAction(std::function<void()>&& action){
    if(!d)
        d = new QtJambiScopePrivate();
    d->append(std::move(action));
}

QtJambiNativeID QtJambiScope::relatedNativeID() const{
    return d ? QtJambiNativeID(jlong(d->link())) : InvalidNativeID;
}

