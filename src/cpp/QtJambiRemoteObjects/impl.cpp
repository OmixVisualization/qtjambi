/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtJambi/QtJambiAPI>
#include <QtJambi/FunctionPointer>
#include <QtJambi/RegistryAPI>
#include <QtJambi/JObjectWrapper>
#include <QtJambi/JavaAPI>
#include <QtJambi/CoreAPI>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QtRemoteObjects/qconnectionfactories.h>
#endif
#include <QtRemoteObjects/QRemoteObjectPendingCall>
#include <QtJambi/qtjambi_cast.h>
#include "hashes.h"
#include "utils_p.h"

#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
using ClientIoDevice = QtROClientIoDevice;
#endif

void __qt_construct_QRemoteObjectPendingCall_1(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments);
void __qt_construct_QRemoteObjectPendingCall_0(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments);
void __qt_destruct_QRemoteObjectPendingCall(void* ptr);

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_remoteobjects_QRemoteObjectPendingReply_initialize_1native__Lio_qt_remoteobjects_QRemoteObjectPendingReply_2)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QRemoteObjectPendingReply<QVariant>::QRemoteObjectPendingReply<QVariant>()")
    QTJAMBI_TRY{
        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_construct_QRemoteObjectPendingCall_0, sizeof(QRemoteObjectPendingReply<QVariant>), typeid(QRemoteObjectPendingReply<QVariant>), false, nullptr);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END

}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_remoteobjects_QRemoteObjectPendingReply_initialize_1native__Lio_qt_remoteobjects_QRemoteObjectPendingReply_2Lio_qt_remoteobjects_QRemoteObjectPendingCall_2)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject other0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QRemoteObjectPendingReply<QVariant>::QRemoteObjectPendingReply<QVariant>(const QRemoteObjectPendingCall & other)")
    QTJAMBI_TRY{
        jvalue arguments;
        arguments.l = other0;
        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_construct_QRemoteObjectPendingCall_1, sizeof(QRemoteObjectPendingReply<QVariant>), typeid(QRemoteObjectPendingReply<QVariant>), false, &arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0) || !defined(QTJAMBI_NO_RO_PRIVATE)
void qtjambi_qRegisterRemoteObjectsClient(JNIEnv *env, jclass type, const QString& id){
    struct ROClientFactory{
        using CreatorFunc = ClientIoDevice * (*)(QObject *);
        QHash<QString, CreatorFunc> m_creatorFuncs;
    };
    ROClientFactory *factory = reinterpret_cast<ROClientFactory *>(QtROClientFactory::instance());
    Q_ASSERT(factory);
    jmethodID constructor = env->GetMethodID(type, "<init>", "(Lio/qt/core/QObject;)V");
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    Q_ASSERT(constructor);
    JObjectWrapper _type(env, type);
    uint hash = uint(QtJambiAPI::getJavaObjectHashCode(env, type));
    factory->m_creatorFuncs[id] = qtjambi_function_pointer<4,ClientIoDevice*(QObject *)>(
        [_type,constructor](QObject * o) -> ClientIoDevice*{
        if(JniEnvironment env{300}){
                jobject parent = QtJambiAPI::convertQObjectToJavaObject(env, o);
                jobject newObject = env->NewObject(jclass(_type.object()), constructor, parent);
                JavaException::check(env QTJAMBI_STACKTRACEINFO );
                return reinterpret_cast<ClientIoDevice*>(QtJambiAPI::convertJavaObjectToQObject(env, newObject));
            }
            return nullptr;
        },
        hash);
}

void qtjambi_qRegisterRemoteObjectsServer(JNIEnv *env, jclass type, const QString& id){
    struct ROServerFactory{
        using CreatorFunc = QConnectionAbstractServer * (*)(QObject *);
        QHash<QString, CreatorFunc> m_creatorFuncs;
    };
    ROServerFactory *serverFactory = reinterpret_cast<ROServerFactory *>(QtROServerFactory::instance());
    Q_ASSERT(serverFactory);
    jmethodID constructor = env->GetMethodID(type, "<init>", "(Lio/qt/core/QObject;)V");
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    Q_ASSERT(constructor);
    JObjectWrapper _type(env, type);
    uint hash = uint(QtJambiAPI::getJavaObjectHashCode(env,type));
    serverFactory->m_creatorFuncs[id] = qtjambi_function_pointer<4,QConnectionAbstractServer*(QObject *)>(
        [_type,constructor](QObject * o) -> QConnectionAbstractServer*{
        if(JniEnvironment env{300}){
                jobject parent = QtJambiAPI::convertQObjectToJavaObject(env, o);
                jobject newObject = env->NewObject(jclass(_type.object()), constructor, parent);
                JavaException::check(env QTJAMBI_STACKTRACEINFO );
                return reinterpret_cast<QConnectionAbstractServer*>(QtJambiAPI::convertJavaObjectToQObject(env, newObject));
            }
            return nullptr;
        },
        hash);
}
#endif

void qtjambi_copyStoredProperties(JNIEnv *env, jclass type, jobject src, jobject dst){
    if(Java::QtCore::QObject::isAssignableFrom(env, type)){
        JavaException::raiseIllegalArgumentException(env, "Only gadget types allowed." QTJAMBI_STACKTRACEINFO );
    }
    const QMetaObject *meta_object = CoreAPI::metaObjectForClass(env, type);
    if(!meta_object){
        jclass closestClass = JavaAPI::resolveClosestQtSuperclass(env, type);
        if(closestClass){
            const QMetaObject *original_meta_object = CoreAPI::metaObjectForClass(env, closestClass);
            meta_object = CoreAPI::metaObjectForClass(env, type, original_meta_object);
        }
    }
    if(!meta_object)
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("No meta object available for class %1").arg(QtJambiAPI::getClassName(env, type)) QTJAMBI_STACKTRACEINFO );
    const void *_src;
    void *_dst;
    if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, src)){
        _src = QtJambiAPI::convertJavaObjectToNative(env, src);
    }else{
        _src = src;
    }
    JObjectWrapper wrapper;
    if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, dst)){
        _dst = QtJambiAPI::convertJavaObjectToNative(env, dst);
    }else{
        wrapper = JObjectWrapper(env, dst);
        _dst = &wrapper;
    }
    QtRemoteObjects::copyStoredProperties(meta_object, _src, _dst);
}

void qtjambi_copyStoredProperties(JNIEnv *env, jclass type, QDataStream& src, jobject dst){
    if(Java::QtCore::QObject::isAssignableFrom(env, type)){
        JavaException::raiseIllegalArgumentException(env, "Only gadget types allowed." QTJAMBI_STACKTRACEINFO );
    }
    const QMetaObject *meta_object = CoreAPI::metaObjectForClass(env, type);
    if(!meta_object){
        jclass closestClass = JavaAPI::resolveClosestQtSuperclass(env, type);
        if(closestClass){
            const QMetaObject *original_meta_object = CoreAPI::metaObjectForClass(env, closestClass);
            meta_object = CoreAPI::metaObjectForClass(env, type, original_meta_object);
        }
    }
    if(!meta_object)
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("No meta object available for class %1").arg(QtJambiAPI::getClassName(env, type)) QTJAMBI_STACKTRACEINFO );
    void *_dst;
    JObjectWrapper wrapper;
    if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, dst)){
        _dst = QtJambiAPI::convertJavaObjectToNative(env, dst);
    }else{
        wrapper = JObjectWrapper(env, dst);
        _dst = &wrapper;
    }
    QtRemoteObjects::copyStoredProperties(meta_object, src, _dst);
}

void qtjambi_copyStoredProperties(JNIEnv *env, jclass type, jobject src, QDataStream& dst){
    if(Java::QtCore::QObject::isAssignableFrom(env, type)){
        JavaException::raiseIllegalArgumentException(env, "Only gadget types allowed." QTJAMBI_STACKTRACEINFO );
    }
    const QMetaObject *meta_object = CoreAPI::metaObjectForClass(env, type);
    if(!meta_object){
        jclass closestClass = JavaAPI::resolveClosestQtSuperclass(env, type);
        if(closestClass){
            const QMetaObject *original_meta_object = CoreAPI::metaObjectForClass(env, closestClass);
            meta_object = CoreAPI::metaObjectForClass(env, type, original_meta_object);
        }
    }
    if(!meta_object)
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("No meta object available for class %1").arg(QtJambiAPI::getClassName(env, type)) QTJAMBI_STACKTRACEINFO );
    const void *_src;
    JObjectWrapper wrapper;
    if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, src)){
        _src = QtJambiAPI::convertJavaObjectToNative(env, src);
    }else{
        wrapper = JObjectWrapper(env, src);
        _src = &wrapper;
    }
    QtRemoteObjects::copyStoredProperties(meta_object, _src, dst);
}


void initialize_meta_info_RemoteObject(){
    using namespace RegistryAPI;
    const std::type_info& typeId = registerUnspecificTypeInfo<QRemoteObjectPendingReply<QVariant>>("QRemoteObjectPendingReply", "io/qt/remoteobjects/QRemoteObjectPendingReply");
    RegistryAPI::registerOperators<QRemoteObjectPendingReply<QVariant>>();
    int metaTypeID = registerMetaType<QRemoteObjectPendingReply<QVariant>>("QRemoteObjectPendingReply<QVariant>");
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QMetaType::registerNormalizedTypedef("QRemoteObjectPendingReply", metaTypeID);
#else
    QMetaType::registerNormalizedTypedef("QRemoteObjectPendingReply", QMetaType(metaTypeID));
#endif
    registerConstructorInfos(typeId, &__qt_destruct_QRemoteObjectPendingCall, {
    });
}

#if QT_VERSION >= QT_VERSION_CHECK(6,4,0)
int qRegisterNormalizedMetaType_QIntHash(class QByteArray const &name){
    return qRegisterNormalizedMetaTypeImplementation<QHash<int, QByteArray>>(name);
}
#endif


namespace Java{
namespace QtRemoteObjects{
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/remoteobjects,IoDeviceBase$ReadResult,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/remoteobjects/QtRemoteObjects$QRemoteObjectPacketTypeEnum;Ljava/lang/String;)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/remoteobjects,QtROIoDeviceBase$ReadResult,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/remoteobjects/QtRemoteObjects$QRemoteObjectPacketTypeEnum;Ljava/lang/String;)
)
}
}
