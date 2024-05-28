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

#include "javainvalidate.h"
#include "qtjambilink_p.h"

bool invalidateJavaObject(JNIEnv *env, jobject java_object)
{
    if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaInterface(env, java_object)){
        if (link->ownership() != QtJambiLink::Ownership::Java
//              && !link->createdByJava()
                && !link->isShell()
                && !link->isQObject()
            ) {
            link->invalidate(env);
        }
        return true;
    }
    return false;
}

bool invalidateArray(JNIEnv *env, jobject java_array)
{
    bool result = false;
    Q_ASSERT(java_array);
    jsize array_size = env->GetArrayLength(jobjectArray(java_array));
    for (int i=0; i<array_size; ++i) {
        jobject java_element = env->GetObjectArrayElement(jobjectArray(java_array), i);
        if (java_element) {
            result |= invalidateJavaObject(env, java_element);
        }
    }
    return result;
}

bool invalidateCollection(JNIEnv *env, jobject java_collection)
{
    bool result = false;
    Q_ASSERT(java_collection);
    if(!invalidateJavaObject(env, java_collection)){
        jobject __qt__iterator = QtJambiAPI::iteratorOfJavaCollection(env, java_collection);
        while(QtJambiAPI::hasJavaIteratorNext(env, __qt__iterator)) {
            jobject java_element = QtJambiAPI::nextOfJavaIterator(env, __qt__iterator);
            if (java_element) {
                result |= invalidateJavaObject(env, java_element);
            }
        }
    }
    return result;
}

bool forcedInvalidateJavaObject(JNIEnv *env, jobject java_object)
{
    if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaInterface(env, java_object)){
        link->invalidate(env);
        return true;
    }
    return false;
}

bool forcedInvalidateArray(JNIEnv *env, jobject java_array)
{
    bool result = false;
    Q_ASSERT(java_array);
    jsize array_size = env->GetArrayLength(jobjectArray(java_array));
    for (int i=0; i<array_size; ++i) {
        jobject java_element = env->GetObjectArrayElement(jobjectArray(java_array), i);
        if (java_element) {
            result |= forcedInvalidateJavaObject(env, java_element);
        }
    }
    return result;
}

bool forcedInvalidateCollection(JNIEnv *env, jobject java_collection)
{
    bool result = false;
    Q_ASSERT(java_collection);
    if(!forcedInvalidateJavaObject(env, java_collection)){
        jobject __qt__iterator = QtJambiAPI::iteratorOfJavaCollection(env, java_collection);
        while(QtJambiAPI::hasJavaIteratorNext(env, __qt__iterator)) {
            jobject java_element = QtJambiAPI::nextOfJavaIterator(env, __qt__iterator);
            if (java_element) {
                result |= forcedInvalidateJavaObject(env, java_element);
            }
        }
    }
    return result;
}

void InvalidateAfterUse::invalidate(JNIEnv *env, jobject java_object){
    invalidateJavaObject(env, java_object);
}

void InvalidateAfterUse::invalidate(JNIEnv *env, QtJambiNativeID nativeId){
    if (QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(nativeId)) {
        if(!link->isShell() && !link->isQObject()){
            link->invalidate(env);
        }
    }
}

void InvalidateAfterUse::forcedInvalidate(JNIEnv *env, jobject java_object){
    forcedInvalidateJavaObject(env, java_object);
}

void InvalidateAfterUse::forcedInvalidate(JNIEnv *env, QtJambiNativeID nativeId){
    if (QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(nativeId)) {
        link->invalidate(env);
    }
}

struct InvalidateAfterUsePrivate{
    virtual ~InvalidateAfterUsePrivate();
};

InvalidateAfterUsePrivate::~InvalidateAfterUsePrivate() = default;

template<bool(INVALIDATE)(JNIEnv*, jobject)>
struct InvalidateObjectAfterUsePrivate : InvalidateAfterUsePrivate{
    InvalidateObjectAfterUsePrivate(JNIEnv *env, jobject object)
        : m_env(env),
          m_object(m_env->NewGlobalRef(object))
    {}
    ~InvalidateObjectAfterUsePrivate() override {
        QtJambiExceptionInhibitor __exnHandler;
        try{
            INVALIDATE(m_env, m_object);
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
};

template<bool(TEST)(const QSharedPointer<QtJambiLink>&)>
struct InvalidateNativeIDAfterUsePrivate : InvalidateAfterUsePrivate{
    InvalidateNativeIDAfterUsePrivate(JNIEnv *env, const QSharedPointer<QtJambiLink>& link)
        : m_env(env),
        m_link(link)
    {}
    ~InvalidateNativeIDAfterUsePrivate() override {
        QtJambiExceptionInhibitor __exnHandler;
        try{
            QSharedPointer<QtJambiLink> link = m_link.toStrongRef();
            if(TEST(link)){
                link->invalidate(m_env);
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

static bool invalidateLinkTest(const QSharedPointer<QtJambiLink>& link){return link && !link->isShell() && !link->isQObject();}
static bool forcedInvalidateLinkTest(const QSharedPointer<QtJambiLink>& link){return !link.isNull();}

AbstractInvalidateAfterUse::AbstractInvalidateAfterUse(InvalidateAfterUsePrivate& _p)
    : p(&_p)
{
}

AbstractInvalidateAfterUse::~AbstractInvalidateAfterUse()
{
    delete p;
}

InvalidateAfterUse::InvalidateAfterUse(JNIEnv *env, jobject object)
    : AbstractInvalidateAfterUse(*new InvalidateObjectAfterUsePrivate<invalidateJavaObject>(env, object))
{}

InvalidateAfterUse::InvalidateAfterUse(JNIEnv *env, QtJambiNativeID nativeId)
    : AbstractInvalidateAfterUse(*new InvalidateNativeIDAfterUsePrivate<invalidateLinkTest>(env, QtJambiLink::fromNativeId(nativeId)))
{}

ForcedInvalidateAfterUse::ForcedInvalidateAfterUse(JNIEnv *env, jobject object)
    : AbstractInvalidateAfterUse(*new InvalidateObjectAfterUsePrivate<forcedInvalidateJavaObject>(env, object))
{}

ForcedInvalidateAfterUse::ForcedInvalidateAfterUse(JNIEnv *env, QtJambiNativeID nativeId)
    : AbstractInvalidateAfterUse(*new InvalidateNativeIDAfterUsePrivate<forcedInvalidateLinkTest>(env, QtJambiLink::fromNativeId(nativeId)))
{}

InvalidateContainerAfterUse::InvalidateContainerAfterUse(JNIEnv *env, jobject object)
    : AbstractInvalidateAfterUse(*new InvalidateObjectAfterUsePrivate<invalidateCollection>(env, object))
{}

InvalidateArrayAfterUse::InvalidateArrayAfterUse(JNIEnv *env, jobjectArray object)
    : AbstractInvalidateAfterUse(*new InvalidateObjectAfterUsePrivate<invalidateArray>(env, object))
{}

ForcedInvalidateContainerAfterUse::ForcedInvalidateContainerAfterUse(JNIEnv *env, jobject object)
    : AbstractInvalidateAfterUse(*new InvalidateObjectAfterUsePrivate<forcedInvalidateCollection>(env, object))
{}

ForcedInvalidateArrayAfterUse::ForcedInvalidateArrayAfterUse(JNIEnv *env, jobjectArray object)
    : AbstractInvalidateAfterUse(*new InvalidateObjectAfterUsePrivate<forcedInvalidateArray>(env, object))
{}
