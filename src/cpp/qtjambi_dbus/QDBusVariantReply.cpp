/****************************************************************************
**
** Copyright (C) 2016 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
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
#include "QDBusVariantReply.h"

#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_registry.h>
#include <qtjambi/qtjambi_cast.h>

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_dbus_QDBusReply__1_1qt_1QDBusReply_1clone)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    try{
        const QDBusReply<QVariant> *__qt_this = qtjambi_object_from_nativeId<QDBusReply<QVariant>>(__this_nativeId);
        Q_ASSERT(__qt_this);
        return qtjambi_cast<jobject>(__jni_env, *__qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

// emitting (writeSignalFunction)
// emitting (writeConstructors)
// new QDBusReply(const QDBusError & dbusError)
void __qt_construct_QDBusReply_0(void* __qtjambi_ptr, JNIEnv*, jobject, jvalue*)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QDBusReply(const QDBusError & dbusError)")
    QDBusReply<QVariant> *__qt_this = new(__qtjambi_ptr) QDBusReply<QVariant>();
    Q_UNUSED(__qt_this)
}

// new QDBusReply(const QDBusError & dbusError)
void __qt_construct_QDBusReply_1(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject, jvalue* __java_arguments)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QDBusReply(const QDBusError & dbusError)")
    jobject dbusError0 = __java_arguments[0].l;
    const QDBusError& __qt_dbusError0 = qtjambi_cast<const QDBusError& >(__jni_env, dbusError0);
    QDBusReply<QVariant> *__qt_this = new(__qtjambi_ptr) QDBusReply<QVariant>(__qt_dbusError0);
    Q_UNUSED(__qt_this)
}

// new QDBusReply(const QDBusMessage & reply)
void __qt_construct_QDBusReply_2(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QDBusReply(const QDBusMessage & reply)")
    jobject reply0 = __java_arguments[0].l;
    const QDBusMessage& __qt_reply0 = qtjambi_cast<const QDBusMessage& >(__jni_env, reply0);
    QDBusReply<QVariant> *__qt_this = new(__qtjambi_ptr) QDBusReply<QVariant>(__qt_reply0);
    Q_UNUSED(__jni_object)
    Q_UNUSED(__qt_this)
}

// new QDBusReply(const QDBusPendingCall & pcall)
void __qt_construct_QDBusReply_3(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QDBusReply(const QDBusPendingCall & pcall)")
    jobject pcall0 = __java_arguments[0].l;
    const QDBusPendingCall& __qt_pcall0 = qtjambi_cast<const QDBusPendingCall& >(__jni_env, pcall0);
    QDBusReply<QVariant> *__qt_this = new(__qtjambi_ptr) QDBusReply<QVariant>(__qt_pcall0);
    Q_UNUSED(__jni_object)
    Q_UNUSED(__qt_this)
}

// destruct QDBusReply
void __qt_destruct_QDBusReply(void* ptr)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "destruct QDBusReply<QVariant>")
    reinterpret_cast<QDBusReply<QVariant>*>(ptr)->~QDBusReply<QVariant>();
}

// emitting  (functionsInTargetLang writeFinalFunction)
// QDBusReply::QDBusReply(const QDBusError & dbusError)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_dbus_QDBusReply__1_1qt_1QDBusReply_1new_1cref_1QDBusError__Ljava_lang_Object_2Lio_qt_dbus_QDBusError_2)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject dbusError0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QDBusReply::QDBusReply(const QDBusError & dbusError)")
    try{
        jvalue arguments;
        arguments.l = dbusError0;
        qtjambi_initialize_native_value(__jni_env, __jni_class, __jni_object, &__qt_construct_QDBusReply_1, sizeof(QDBusReply<QVariant>), typeid(QDBusReply<QVariant>), false, &arguments);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }

}

// QDBusReply::QDBusReply(const QDBusMessage & reply)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_dbus_QDBusReply__1_1qt_1QDBusReply_1new_1cref_1QDBusMessage__Ljava_lang_Object_2Lio_qt_dbus_QDBusMessage_2)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject reply0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QDBusReply::QDBusReply(const QDBusMessage & reply)")
    try{
        jvalue arguments;
        arguments.l = reply0;
        qtjambi_initialize_native_value(__jni_env, __jni_class, __jni_object, &__qt_construct_QDBusReply_2, sizeof(QDBusReply<QVariant>), typeid(QDBusReply<QVariant>), false, &arguments);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }

}

// QDBusReply::QDBusReply(const QDBusPendingCall & pcall)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_dbus_QDBusReply__1_1qt_1QDBusReply_1new_1cref_1QDBusPendingCall__Ljava_lang_Object_2Lio_qt_dbus_QDBusPendingCall_00024Impl_2)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject pcall0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QDBusReply::QDBusReply(const QDBusPendingCall & pcall)")
    try{
        jvalue arguments;
        arguments.l = pcall0;
        qtjambi_initialize_native_value(__jni_env, __jni_class, __jni_object, &__qt_construct_QDBusReply_3, sizeof(QDBusReply<QVariant>), typeid(QDBusReply<QVariant>), false, &arguments);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }

}

// QDBusReply::error() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_dbus_QDBusReply__1_1qt_1QDBusReply_1error_1constfct__J)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QDBusReply::error() const")
    try{
        const QDBusReply<QVariant> *__qt_this = qtjambi_object_from_nativeId<QDBusReply<QVariant>>(__this_nativeId);
        Q_ASSERT(__qt_this);
        const QDBusError& __qt_return_value = __qt_this->error();
        return qtjambi_cast<jobject>(__jni_env, __qt_return_value);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;

}

// QDBusReply::isValid() const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_dbus_QDBusReply__1_1qt_1QDBusReply_1isValid_1constfct__J)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QDBusReply::isValid() const")
    try{
        const QDBusReply<QVariant> *__qt_this = qtjambi_object_from_nativeId<QDBusReply<QVariant>>(__this_nativeId);
        Q_ASSERT(__qt_this);
        bool __qt_return_value = __qt_this->isValid();
        return jboolean(__qt_return_value);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;

}

// QDBusReply::operator=(const QDBusReply<QVariant > & other)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_dbus_QDBusReply__1_1qt_1QDBusReply_1set_1cref_1QDBusReply__JLio_qt_dbus_QDBusReply_2)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jobject other0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QDBusReply::operator=(const QDBusReply<QVariant > & other)")
    QtJambiScope __qtjambi_scope(__this_nativeId);
    try{
        QDBusReply<QVariant> *__qt_this = qtjambi_object_from_nativeId<QDBusReply<QVariant>>(__this_nativeId);
        Q_ASSERT(__qt_this);
        const QDBusReply<QVariant>&  __qt_other0 = qtjambi_cast<QDBusReply<QVariant> >(__jni_env, __qtjambi_scope, other0);
        QDBusReply<QVariant>& __qt_return_value = ((*static_cast<QDBusReply<QVariant>*>(__qt_this)) = __qt_other0);
        return qtjambi_cast<jobject>(__jni_env, __qt_return_value);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;

}

// QDBusReply::value() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_dbus_QDBusReply__1_1qt_1QDBusReply_1_1value_1constfct__J)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QDBusReply::value() const")
    try{
        const QDBusReply<QVariant> *__qt_this = qtjambi_object_from_nativeId<QDBusReply<QVariant>>(__this_nativeId);
        Q_ASSERT(__qt_this);
        QVariant __qt_return_value = __qt_this->value();
        return qtjambi_cast<jobject>(__jni_env, __qt_return_value);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;

}

void initialize_meta_info_QtDBus()
{
    const std::type_info& typeId = registerTypeInfo<QDBusReply<QVariant>>("QDBusReply", "io/qt/dbus/QDBusReply");
    registerOperators<QDBusReply<QVariant>>();
    int metaTypeID = registerMetaType<QDBusReply<QVariant>>("QDBusReply<QVariant>");
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QMetaType::registerNormalizedTypedef("QDBusReply", metaTypeID);
#else
    QMetaType::registerNormalizedTypedef("QDBusReply", QMetaType(metaTypeID));
#endif
    registerConstructorInfos(typeId, &__qt_destruct_QDBusReply, {
        ConstructorInfo(&__qt_construct_QDBusReply_0, nullptr)
       ,ConstructorInfo(&__qt_construct_QDBusReply_1, "Lio/qt/dbus/QDBusError;")
       ,ConstructorInfo(&__qt_construct_QDBusReply_2, "Lio/qt/dbus/QDBusMessage;")
       ,ConstructorInfo(&__qt_construct_QDBusReply_3, "Lio/qt/dbus/QDBusPendingCall;")
    });
}
