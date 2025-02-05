/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
#include "java_p.h"
#include "containerapi.h"
#include "qtjambi_cast.h"

extern "C" JNIEXPORT void JNICALL Java_io_qt_internal_NativeUtility_initializeNativeObject(JNIEnv *env, jclass, jclass callingClass, jobject object, jobject link, jobject arguments){
    try{
        QtJambiShellImpl::initializeNativeInterface(env, callingClass, object, link, arguments);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_internal_NativeUtility_00024NativeLink_setHasDisposedSignal(JNIEnv *env, jclass, QtJambiNativeID native_id){
    try{
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)){
            link->setHasDisposedSignal();
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_internal_NativeUtility_00024NativeLink_clean(JNIEnv *env, jclass, QtJambiNativeID native_id){
    try{
        jthrowable throwable(nullptr);
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)){
            try{
                link->onClean(env);
            }catch(const JavaException& exn){
                throwable = exn.throwable(env);
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

extern "C" JNIEXPORT void JNICALL Java_io_qt_internal_NativeUtility_00024NativeLink_assignNativeId(JNIEnv *, jclass, QtJambiNativeID native_id, jlong ptr){
    if(!!native_id)
        *reinterpret_cast<QSharedPointer<QtJambiLink>*>(ptr) = reinterpret_cast<QtJambiLink*>(native_id)->getStrongPointer();
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_internal_NativeUtility_00024NativeLink_dispose(JNIEnv *env, jclass, QtJambiNativeID native_id){
    try{
        jthrowable throwable(nullptr);
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)) {
            try{
                QtJambiExceptionRaiser __exnRaiser;
                link->onDispose(env);
                __exnRaiser.raise(env);
            }catch(const JavaException& exn){
                throwable = exn.throwable(env);
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

extern "C" JNIEXPORT jstring JNICALL Java_io_qt_internal_NativeUtility_00024NativeLink_qtTypeName(JNIEnv *env, jclass, QtJambiNativeID native_id){
    try{
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)) {
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
            return env->NewStringUTF(link->qtTypeName());
#else
            if(link->metaType())
                return env->NewStringUTF(link->metaType()->name());
#endif
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_internal_NativeUtility_setCppOwnership(JNIEnv *env, jclass, QtJambiNativeID native_id){
    try{
        QtJambiAPI::setCppOwnership(env, native_id);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_internal_NativeUtility_setDefaultOwnership(JNIEnv *env, jclass, QtJambiNativeID native_id){
    try{
        QtJambiAPI::setDefaultOwnership(env, native_id);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_internal_NativeUtility_setJavaOwnership(JNIEnv *env, jclass, QtJambiNativeID native_id){
    try{
        QtJambiAPI::setJavaOwnership(env, native_id);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_internal_NativeUtility_isCppOwnership(JNIEnv *env, jclass, QtJambiNativeID native_id){
    try{
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)){
            return link->ownership()==QtJambiLink::Ownership::Cpp;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_internal_NativeUtility_isSplitOwnership(JNIEnv *env, jclass, QtJambiNativeID native_id){
    try{
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)){
            return link->ownership()==QtJambiLink::Ownership::Split;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_internal_NativeUtility_isJavaOwnership(JNIEnv *env, jclass, QtJambiNativeID native_id){
    try{
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)){
            return link->ownership()==QtJambiLink::Ownership::Java;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_internal_NativeUtility_invalidateObject(JNIEnv *env, jclass, QtJambiNativeID native_id){
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

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_internal_NativeUtility_hasOwnerFunction(JNIEnv *env, jclass, QtJambiNativeID native_id){
    try{
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)) {
            if(!link->isQObject()){
                return link->ownerFunction()!=nullptr;
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_internal_NativeUtility_owner(JNIEnv *env, jclass, QtJambiNativeID native_id){
    try{
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)) {
            if(!link->isQObject()){
                PtrOwnerFunction ownerFunction = link->ownerFunction();
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

extern "C" JNIEXPORT void JNICALL Java_io_qt_internal_NativeUtility_unregisterConDestroyedObject(JNIEnv *env, jclass, QtJambiNativeID dependentObject, QtJambiNativeID owner){
    QSharedPointer<QtJambiLink> _dependentLink = QtJambiLink::fromNativeId(dependentObject);
    QSharedPointer<QtJambiLink> _ownerLink = QtJambiLink::fromNativeId(owner);
    if(_dependentLink && _ownerLink){
        _ownerLink->unregisterConDestroyedObject(env, _dependentLink);
    }
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_internal_NativeUtility_registerConDestroyedObject(JNIEnv *env, jclass, QtJambiNativeID dependentObject, QtJambiNativeID owner){
    QSharedPointer<QtJambiLink> _dependentLink = QtJambiLink::fromNativeId(dependentObject);
    QSharedPointer<QtJambiLink> _ownerLink = QtJambiLink::fromNativeId(owner);
    if(_dependentLink && _ownerLink){
        _ownerLink->registerConDestroyedObject(env, _dependentLink);
    }
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_internal_NativeUtility_unregisterDependentObject(JNIEnv *, jclass, QtJambiNativeID dependentObject, QtJambiNativeID owner){
    QSharedPointer<QtJambiLink> _dependentLink = QtJambiLink::fromNativeId(dependentObject);
    QSharedPointer<QtJambiLink> _ownerLink = QtJambiLink::fromNativeId(owner);
    if(_dependentLink && _ownerLink){
        _ownerLink->unregisterDependentObject(_dependentLink);
    }
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_internal_NativeUtility_registerDependentObject(JNIEnv *, jclass, QtJambiNativeID dependentObject, QtJambiNativeID owner){
    QSharedPointer<QtJambiLink> _dependentLink = QtJambiLink::fromNativeId(dependentObject);
    QSharedPointer<QtJambiLink> _ownerLink = QtJambiLink::fromNativeId(owner);
    if(_dependentLink && _ownerLink){
        _ownerLink->registerDependentObject(_dependentLink);
    }
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_internal_NativeUtility_getInterfaceInfos(JNIEnv *env, jclass, jobject object){
    try{
        return SuperTypeInfos::fromClass(env, env->IsSameObject(object,nullptr) ? nullptr : env->GetObjectClass(object)).interfaceInfos();
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
        return nullptr;
    }
}

void truncateBuffer(JNIEnv *env, jobject buffer);

extern "C" JNIEXPORT void JNICALL Java_io_qt_internal_NativeUtility_truncateBuffer(JNIEnv *env, jclass, QtJambiNativeID nid, jobject buffer){
    QTJAMBI_TRY {
        if(QSharedPointer<QtJambiLink> _ownerLink = QtJambiLink::fromNativeId(nid)){
            _ownerLink->removeFinalization(env, buffer, true);
        }else{
            truncateBuffer(env, buffer);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_internal_NativeUtility_mutableByteArrayData(JNIEnv *env, jclass, QtJambiNativeID nid){
    jobject __java_return_value{0};
    QTJAMBI_TRY {
        QByteArray *__qt_this = QtJambiAPI::objectFromNativeId<QByteArray>(nid);
        QtJambiAPI::checkNullPointer(env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QByteArray::data()", __qt_this)
        char* __qt_return_value = __qt_this->data();
        DataJBuffer d(env, __qt_return_value, jlong(__qt_this->capacity()));
        if(__qt_this->size()<__qt_this->capacity())
            d.setLimit(jsize(__qt_this->size()));
        __java_return_value = d.take();
        QtJambiAPI::registerDependency(env, __java_return_value, nid);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return __java_return_value;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_internal_NativeUtility_mutableStringData(JNIEnv *env, jclass, QtJambiNativeID nid){
    jobject __java_return_value{0};
    QTJAMBI_TRY {
        QString *__qt_this = QtJambiAPI::objectFromNativeId<QString>(nid);
        QtJambiAPI::checkNullPointer(env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QString::data()", __qt_this)
        QChar* __qt_return_value = __qt_this->data();
        DataJBuffer d(env, __qt_return_value, jlong(__qt_this->capacity()));
        if(__qt_this->size()<__qt_this->capacity())
            d.setLimit(jsize(__qt_this->size()));
        __java_return_value = d.take();
        QtJambiAPI::registerDependency(env, __java_return_value, nid);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return __java_return_value;
}

#define MUTABLE_LIST_IMPL(TYPE,type,CONTAINER)\
extern "C" JNIEXPORT jobject JNICALL\
    Java_io_qt_internal_NativeUtility_mutableData##TYPE\
    (JNIEnv *env, jclass, jobject iter, QtJambiNativeID nid)\
{\
        Q_UNUSED(iter)\
        jobject __java_return_value{0};\
        QTJAMBI_TRY {\
            QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(nid);\
            if(dynamic_cast<Abstract##CONTAINER##Access*>(container.second)){\
                Q##CONTAINER<type> *__qt_this = reinterpret_cast<Q##CONTAINER<type>*>(container.first);\
                QtJambiAPI::checkNullPointer(env, __qt_this);\
                type* __qt_return_value = __qt_this->data();\
                DataJBuffer d(env, __qt_return_value, jlong(__qt_this->capacity()));\
                if(__qt_this->size()<__qt_this->capacity())\
                d.setLimit(jsize(__qt_this->size()));\
                __java_return_value = d.take();\
                QtJambiAPI::registerDependency(env, __java_return_value, nid);\
        }\
    }QTJAMBI_CATCH(const JavaException& exn){\
            exn.raiseInJava(env);\
    }QTJAMBI_TRY_END\
        return __java_return_value;\
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#define MUTABLE_LIST(TYPE,type) MUTABLE_LIST_IMPL(TYPE,type,List)
#else
#define MUTABLE_LIST(TYPE,type) MUTABLE_LIST_IMPL(TYPE,type,Vector)
#endif

MUTABLE_LIST(B,char)
MUTABLE_LIST(S,qint16)
MUTABLE_LIST(I,qint32)
MUTABLE_LIST(J,qint64)
MUTABLE_LIST(C,QChar)
MUTABLE_LIST(F,float)
MUTABLE_LIST(D,double)
