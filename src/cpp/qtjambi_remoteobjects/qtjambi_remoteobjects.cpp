/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_functionpointer.h>
#include <qtjambi/qtjambi_registry.h>
#include <QtRemoteObjects/private/qconnectionfactories_p.h>
#include <QtRemoteObjects/QRemoteObjectPendingCall>
#include <qtjambi/qtjambi_cast.h>

#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
using ClientIoDevice = QtROClientIoDevice;
#endif

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_remoteobjects_QtRemoteObjects_qRegisterRemoteObjectsServer)
    (JNIEnv *__jni_env,
    jclass,
    jobject id0,
    jclass type1,
    jobject constructor2)
{
    struct ROServerFactory{
        using CreatorFunc = QConnectionAbstractServer * (*)(QObject *);
        QHash<QString, CreatorFunc> m_creatorFuncs;
    };
    QTJAMBI_DEBUG_METHOD_PRINT("native", "qRegisterRemoteObjectsServer<T>(const QString & id)")
    Q_UNUSED(__jni_env)
    ROServerFactory *serverFactory = reinterpret_cast<ROServerFactory *>(QtROServerFactory::instance());
    Q_ASSERT(serverFactory);
    const QString&  __qt_id0 = qtjambi_to_qstring(__jni_env, jstring(id0));
    jmethodID constructor = __jni_env->FromReflectedMethod(constructor2);
    type1 = jclass(__jni_env->NewGlobalRef(jobject(type1)));
    uint hash = uint(qtjambi_java_object_hashcode(__jni_env,type1));
    serverFactory->m_creatorFuncs[__qt_id0] = qtjambi_function_pointer<16,QConnectionAbstractServer*(QObject *)>(
        [type1,constructor](QObject * o) -> QConnectionAbstractServer*{
            if(JNIEnv* env = qtjambi_current_environment()){
                QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                jobject parent = qtjambi_from_QObject(env, o);
                jobject newObject = env->NewObject(type1, constructor, parent);
                qtjambi_throw_java_exception(env);
                qtjambi_rethrowing(env,
                    return reinterpret_cast<QConnectionAbstractServer*>(qtjambi_to_qobject(env, newObject));
                )
            }
            return nullptr;
        },
        hash);
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_remoteobjects_QtRemoteObjects_qRegisterRemoteObjectsClient)
    (JNIEnv *__jni_env,
    jclass,
    jobject id0,
    jclass type1,
    jobject constructor2)
{
    struct ROClientFactory{
        using CreatorFunc = ClientIoDevice * (*)(QObject *);
        QHash<QString, CreatorFunc> m_creatorFuncs;
    };
    QTJAMBI_DEBUG_METHOD_PRINT("native", "qRegisterRemoteObjectsClient<T>(const QString & id)")
    Q_UNUSED(__jni_env)
    ROClientFactory *factory = reinterpret_cast<ROClientFactory *>(QtROClientFactory::instance());
    Q_ASSERT(factory);
    const QString&  __qt_id0 = qtjambi_to_qstring(__jni_env, jstring(id0));
    jmethodID constructor = __jni_env->FromReflectedMethod(constructor2);
    type1 = jclass(__jni_env->NewGlobalRef(jobject(type1)));
    uint hash = uint(qtjambi_java_object_hashcode(__jni_env, type1));
    factory->m_creatorFuncs[__qt_id0] = qtjambi_function_pointer<16,ClientIoDevice*(QObject *)>(
        [type1,constructor](QObject * o) -> ClientIoDevice*{
            if(JNIEnv* env = qtjambi_current_environment()){
                QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                jobject parent = qtjambi_from_QObject(env, o);
                jobject newObject = env->NewObject(type1, constructor, parent);
                qtjambi_throw_java_exception(env);
                qtjambi_rethrowing(env,
                    return reinterpret_cast<ClientIoDevice*>(qtjambi_to_qobject(env, newObject));
                )
            }
            return nullptr;
        },
        hash);
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_remoteobjects_QRemoteObjectNode_acquire)
(JNIEnv *__jni_env, jobject __this, jclass type, jobject constructor, jobject name)
{
    jmethodID constructorID = __jni_env->FromReflectedMethod(constructor);
    Q_ASSERT(constructorID);
    return __jni_env->NewObject(type, constructorID, __this, name);
}

void __qt_construct_QRemoteObjectPendingCall_1(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments);
void __qt_construct_QRemoteObjectPendingCall_0(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments);
void __qt_destruct_QRemoteObjectPendingCall(void* ptr);

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_remoteobjects_QRemoteObjectPendingReply_initialize_1native__Lio_qt_remoteobjects_QRemoteObjectPendingReply_2)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QRemoteObjectPendingReply<QVariant>::QRemoteObjectPendingReply<QVariant>()")
    try{
        qtjambi_initialize_native_value(__jni_env, __jni_class, __jni_object, &__qt_construct_QRemoteObjectPendingCall_0, sizeof(QRemoteObjectPendingReply<QVariant>), typeid(QRemoteObjectPendingReply<QVariant>), false, nullptr);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }

}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_remoteobjects_QRemoteObjectPendingReply_initialize_1native__Lio_qt_remoteobjects_QRemoteObjectPendingReply_2Lio_qt_remoteobjects_QRemoteObjectPendingCall_2)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject other0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QRemoteObjectPendingReply<QVariant>::QRemoteObjectPendingReply<QVariant>(const QRemoteObjectPendingCall & other)")
    try{
        jvalue arguments;
        arguments.l = other0;
        qtjambi_initialize_native_value(__jni_env, __jni_class, __jni_object, &__qt_construct_QRemoteObjectPendingCall_1, sizeof(QRemoteObjectPendingReply<QVariant>), typeid(QRemoteObjectPendingReply<QVariant>), false, &arguments);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_remoteobjects_QtRemoteObjects_copyStoredProperties__Lio_qt_core_QMetaObject_2Ljava_lang_Object_2Ljava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 jobject type,
 jobject src,
 jobject dst)
{
    const QMetaObject* mo = qtjambi_cast<const QMetaObject*>(__jni_env, type);
    QtRemoteObjects::copyStoredProperties(mo, src, dst);
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_remoteobjects_QtRemoteObjects_copyStoredProperties__Lio_qt_core_QMetaObject_2JJ)
(JNIEnv *__jni_env,
 jclass,
 jobject type,
 QtJambiNativeID src,
 QtJambiNativeID dst)
{
    const QMetaObject* mo = qtjambi_cast<const QMetaObject*>(__jni_env, type);
    QtRemoteObjects::copyStoredProperties(mo, qtjambi_from_nativeId(src), qtjambi_from_nativeId(dst));
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_remoteobjects_QtRemoteObjects_copyStoredPropertiesStreamOut__Lio_qt_core_QMetaObject_2Ljava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 jobject type,
 jobject src,
 QtJambiNativeID dst)
{
    const QMetaObject* mo = qtjambi_cast<const QMetaObject*>(__jni_env, type);
    QDataStream* stream = qtjambi_object_from_nativeId<QDataStream>(dst);
    Q_ASSERT(stream);
    QtRemoteObjects::copyStoredProperties(mo, src, *stream);
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_remoteobjects_QtRemoteObjects_copyStoredPropertiesStreamOut__Lio_qt_core_QMetaObject_2JJ)
(JNIEnv *__jni_env,
 jclass,
 jobject type,
 QtJambiNativeID src,
 QtJambiNativeID dst)
{
    const QMetaObject* mo = qtjambi_cast<const QMetaObject*>(__jni_env, type);
    QDataStream* stream = qtjambi_object_from_nativeId<QDataStream>(dst);
    Q_ASSERT(stream);
    QtRemoteObjects::copyStoredProperties(mo, qtjambi_from_nativeId(src), *stream);
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_remoteobjects_QtRemoteObjects_copyStoredPropertiesStreamIn__Lio_qt_core_QMetaObject_2JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 jobject type,
 QtJambiNativeID src,
 jobject dst)
{
    const QMetaObject* mo = qtjambi_cast<const QMetaObject*>(__jni_env, type);
    QDataStream* stream = qtjambi_object_from_nativeId<QDataStream>(src);
    Q_ASSERT(stream);
    QtRemoteObjects::copyStoredProperties(mo, *stream, dst);
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_remoteobjects_QtRemoteObjects_copyStoredPropertiesStreamIn__Lio_qt_core_QMetaObject_2JJ)
(JNIEnv *__jni_env,
 jclass,
 jobject type,
 QtJambiNativeID src,
 QtJambiNativeID dst)
{
    const QMetaObject* mo = qtjambi_cast<const QMetaObject*>(__jni_env, type);
    QDataStream* stream = qtjambi_object_from_nativeId<QDataStream>(src);
    Q_ASSERT(stream);
    QtRemoteObjects::copyStoredProperties(mo, *stream, qtjambi_from_nativeId(dst));
}

void initialize_meta_info_RemoteObject(){
    const std::type_info& typeId = registerTypeInfo<QRemoteObjectPendingReply<QVariant>>("QRemoteObjectPendingReply", "io/qt/remoteobjects/QRemoteObjectPendingReply");
    registerOperators<QRemoteObjectPendingReply<QVariant>>();
    int metaTypeID = registerMetaType<QRemoteObjectPendingReply<QVariant>>("QRemoteObjectPendingReply<QVariant>");
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QMetaType::registerNormalizedTypedef("QRemoteObjectPendingReply", metaTypeID);
#else
    QMetaType::registerNormalizedTypedef("QRemoteObjectPendingReply", QMetaType(metaTypeID));
#endif
    registerConstructorInfos(typeId, &__qt_destruct_QRemoteObjectPendingCall, {
    });
}
