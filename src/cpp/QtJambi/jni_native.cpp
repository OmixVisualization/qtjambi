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

#include "qtjambilink_p.h"
#include "qtjambishell_p.h"
#include "supertypeinfo_p.h"
#include "qtjambi_cast.h"

extern "C" Q_DECL_EXPORT void
JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_NativeUtility_initializeNativeObject)
(JNIEnv *env, jclass, jclass callingClass, jobject object, jobject link, jobject arguments)
{
    try{
        QtJambiShellImpl::initializeNativeInterface(env, callingClass, object, link, arguments);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void
JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_NativeUtility_00024NativeLink_setHasDisposedSignal)
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)){
            link->setHasDisposedSignal();
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT bool
JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_NativeUtility_00024NativeLink_hasDisposedSignal)
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)){
            return link->hasDisposedSignal();
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT void
JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_NativeUtility_00024NativeLink_clean)
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
        jthrowable throwable(nullptr);
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)){
            try{
                link->onClean(env);
            }catch(const JavaException& exn){
                throwable = jthrowable(env->NewLocalRef(exn.object()));
            }
            if(env->ExceptionCheck()){
                throwable = env->ExceptionOccurred();
                env->ExceptionClear();
            }
            {
                QWriteLocker locker(QtJambiLinkUserData::lock());
                link.clear();
            }
        }
        if(throwable)
            env->Throw(throwable);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void
JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_NativeUtility_00024NativeLink_dispose)
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
        jthrowable throwable(nullptr);
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)) {
            try{
                QtJambiExceptionRaiser __exnRaiser;
                link->onDispose(env);
                __exnRaiser.raise(env);
            }catch(const JavaException& exn){
                throwable = jthrowable(env->NewLocalRef(exn.object()));
            }
            if(env->ExceptionCheck()){
                throwable = env->ExceptionOccurred();
                env->ExceptionClear();
            }
            {
                QWriteLocker locker(QtJambiLinkUserData::lock());
                link.clear();
            }
        }
        if(throwable)
            env->Throw(throwable);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT jstring
JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_NativeUtility_00024NativeLink_qtTypeName)
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)) {
            return env->NewStringUTF(link->qtTypeName());
        }
#else
        Q_UNUSED(native_id)
#endif
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_NativeUtility_setCppOwnership)
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
        QtJambiAPI::setCppOwnership(env, native_id);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_NativeUtility_setDefaultOwnership)
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
        QtJambiAPI::setDefaultOwnership(env, native_id);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_NativeUtility_setJavaOwnership)
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
        QtJambiAPI::setJavaOwnership(env, native_id);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_NativeUtility_isCppOwnership)
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)){
            return link->ownership()==QtJambiLink::Ownership::Cpp;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_NativeUtility_isSplitOwnership)
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)){
            return link->ownership()==QtJambiLink::Ownership::Split;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_NativeUtility_isJavaOwnership)
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)){
            return link->ownership()==QtJambiLink::Ownership::Java;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_NativeUtility_invalidateObject)
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)) {
            if(!link->isShell() && !link->isQObject()){
                link->invalidate(env);
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_NativeUtility_hasOwnerFunction)
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)) {
            if(!link->isQObject()){
                PtrOwnerFunction ownerFunction;
                if(link->isSmartPointer()){
                    ownerFunction = reinterpret_cast<SmartPointerToObjectLink*>(link.data())->ownerFunction();
                }else{
                    ownerFunction = reinterpret_cast<PointerToObjectLink*>(link.data())->ownerFunction();
                }
                return ownerFunction!=nullptr;
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_NativeUtility_owner)
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)) {
            if(!link->isQObject()){
                PtrOwnerFunction ownerFunction;
                if(link->isSmartPointer()){
                    ownerFunction = reinterpret_cast<SmartPointerToObjectLink*>(link.data())->ownerFunction();
                }else{
                    ownerFunction = reinterpret_cast<PointerToObjectLink*>(link.data())->ownerFunction();
                }
                if(ownerFunction){
                    const QObject* owner = ownerFunction(link->pointer());
                    return qtjambi_cast<jobject>(env, owner);
                }else if(auto containerAccess = link->containerAccess()){
                    const QObject* owner = containerAccess->getOwner(link->pointer());
                    return qtjambi_cast<jobject>(env, owner);
                }
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_NativeUtility_unregisterDependentObject)
(JNIEnv *, jclass, QtJambiNativeID dependentObject, QtJambiNativeID owner)
{
    QSharedPointer<QtJambiLink> _dependentLink = QtJambiLink::fromNativeId(dependentObject);
    QSharedPointer<QtJambiLink> _ownerLink = QtJambiLink::fromNativeId(owner);
    if(_dependentLink && _ownerLink){
        _ownerLink->unregisterDependentObject(_dependentLink);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_NativeUtility_registerDependentObject)
(JNIEnv *, jclass, QtJambiNativeID dependentObject, QtJambiNativeID owner)
{
    QSharedPointer<QtJambiLink> _dependentLink = QtJambiLink::fromNativeId(dependentObject);
    QSharedPointer<QtJambiLink> _ownerLink = QtJambiLink::fromNativeId(owner);
    if(_dependentLink && _ownerLink){
        _ownerLink->registerDependentObject(_dependentLink);
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_NativeUtility_getInterfaces)
(JNIEnv *env, jclass, jobject object)
{
    try{
        return SuperTypeInfos::fromClass(env, env->IsSameObject(object,nullptr) ? nullptr : env->GetObjectClass(object)).interfaceList(env);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
        return nullptr;
    }
}
