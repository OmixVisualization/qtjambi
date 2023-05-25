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

#include "javainvalidate.h"
#include "qtjambilink_p.h"

bool invalidateJavaObject(JNIEnv *env, jobject java_object, bool checkJavaOwnership = false)
{
    if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaInterface(env, java_object)){
        if (!checkJavaOwnership || (   link->ownership() != QtJambiLink::Ownership::Java
                                    && !link->createdByJava()
                                    && !link->isShell()
                                    && !link->isQObject()
                                    )
            ) {
            link->invalidate(env);
        }
        return true;
    }
    return false;
}

void invalidateArray(JNIEnv *env, jobjectArray java_array, bool checkJavaOwnership)
{
    Q_ASSERT(java_array);
    jsize array_size = env->GetArrayLength(java_array);
    for (int i=0; i<array_size; ++i) {
        jobject java_element = env->GetObjectArrayElement(java_array, i);
        if (java_element) {
            invalidateJavaObject(env, java_element, checkJavaOwnership);
        }
    }
}

void invalidateCollection(JNIEnv *env, jobject java_collection, bool checkJavaOwnership)
{
    Q_ASSERT(java_collection);
    if(!invalidateJavaObject(env, java_collection, checkJavaOwnership)){
        jobject __qt__iterator = QtJambiAPI::iteratorOfJavaCollection(env, java_collection);
        while(QtJambiAPI::hasJavaIteratorNext(env, __qt__iterator)) {
            jobject java_element = QtJambiAPI::nextOfJavaIterator(env, __qt__iterator);
            if (java_element) {
                invalidateJavaObject(env, java_element, checkJavaOwnership);
            }
        }
    }
}

struct InvalidateAfterUsePrivate{
    virtual ~InvalidateAfterUsePrivate();
};

InvalidateAfterUsePrivate::~InvalidateAfterUsePrivate() = default;

struct InvalidateObjectAfterUsePrivate : InvalidateAfterUsePrivate{
    InvalidateObjectAfterUsePrivate(JNIEnv *env, jobject object, bool checkJavaOwnership)
        : m_env(env),
          m_object(m_env->NewGlobalRef(object)),
          m_checkJavaOwnership(checkJavaOwnership)
    {}
    ~InvalidateObjectAfterUsePrivate(){
        QtJambiExceptionInhibitor __exnHandler;
        try{
            invalidateJavaObject(m_env, m_object, m_checkJavaOwnership);
        }catch(const JavaException& exn){
            m_env->DeleteGlobalRef(m_object);
            m_object = nullptr;
            __exnHandler.handle(m_env, exn, nullptr);
        } catch (const std::exception& e) {
            qCWarning(DebugAPI::internalCategory, "%s", e.what());
        } catch (...) {
        }
        if(m_object)
            m_env->DeleteGlobalRef(m_object);
    }

    JNIEnv *m_env;
    jobject m_object;
    bool m_checkJavaOwnership;
};

struct InvalidateNativeIDAfterUsePrivate : InvalidateAfterUsePrivate{
    InvalidateNativeIDAfterUsePrivate(JNIEnv *env, const QSharedPointer<QtJambiLink>& link)
        : m_env(env),
          m_link(link)
    {}
    ~InvalidateNativeIDAfterUsePrivate(){
        QtJambiExceptionInhibitor __exnHandler;
        try{
            if(QSharedPointer<QtJambiLink> link = m_link.toStrongRef()){
                if(!link->isShell() && !link->isQObject()){
                    link->invalidate(m_env);
                }
            }
        }catch(const JavaException& exn){
            __exnHandler.handle(m_env, exn, nullptr);
        } catch (const std::exception& e) {
            qCWarning(DebugAPI::internalCategory, "%s", e.what());
        } catch (...) {
        }
    }

    JNIEnv *m_env;
    QWeakPointer<QtJambiLink> m_link;
};

InvalidateAfterUse::InvalidateAfterUse(JNIEnv *env, jobject object, bool checkJavaOwnership)
    : p(new InvalidateObjectAfterUsePrivate(env, object, checkJavaOwnership))
{}

InvalidateAfterUse::InvalidateAfterUse(JNIEnv *env, QtJambiNativeID nativeId)
    : p(new InvalidateNativeIDAfterUsePrivate(env, QtJambiLink::fromNativeId(nativeId)))
{}

InvalidateAfterUse::~InvalidateAfterUse()
{
    delete p;
}

void InvalidateAfterUse::invalidate(JNIEnv *env, jobject java_object, bool checkJavaOwnership){
    invalidateJavaObject(env, java_object, checkJavaOwnership);
}

void InvalidateAfterUse::invalidate(JNIEnv *env, QtJambiNativeID nativeId){
    if (QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(nativeId)) {
        if(!link->isShell() && !link->isQObject()){
            link->invalidate(env);
        }
    }
}

InvalidateContainerAfterUse::InvalidateContainerAfterUse(JNIEnv *env, jobject object, bool checkJavaOwnership)
    : m_env(env),
      m_object(m_env->NewGlobalRef(object)),
      m_checkJavaOwnership(checkJavaOwnership)
{}

InvalidateContainerAfterUse::~InvalidateContainerAfterUse()
{
    QtJambiExceptionInhibitor __exnHandler;
    try{
        invalidateCollection(m_env, m_object, m_checkJavaOwnership);
    }catch(const JavaException& exn){
        m_env->DeleteGlobalRef(m_object);
        m_object = nullptr;
        __exnHandler.handle(m_env, exn, nullptr);
    } catch (const std::exception& e) {
        qCWarning(DebugAPI::internalCategory, "%s", e.what());
    } catch (...) {
    }
    if(m_object)
        m_env->DeleteGlobalRef(m_object);
}

InvalidateArrayAfterUse::InvalidateArrayAfterUse(JNIEnv *env, jobjectArray object, bool checkJavaOwnership)
    : m_env(env),
      m_object(object),
      m_checkJavaOwnership(checkJavaOwnership)
{}

InvalidateArrayAfterUse::~InvalidateArrayAfterUse()
{
    QtJambiExceptionInhibitor __exnHandler;
    try{
        invalidateArray(m_env, m_object, m_checkJavaOwnership);
    }catch(const JavaException& exn){
        __exnHandler.handle(m_env, exn, nullptr);
    } catch (const std::exception& e) {
        qCWarning(DebugAPI::internalCategory, "%s", e.what());
    } catch (...) {
    }
}
