/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

#ifndef QTJAMBI_APPLICATION_H
#define QTJAMBI_APPLICATION_H

#include "qtjambi_core.h"
#include <QtCore/QVector>

class J2CStringBuffer;

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
using QtJambiObjectData = QObjectUserData;
#define QTJAMBI_OBJECTUSERDATA_TYPE_ID(TYPE) TYPE::id()
#define QTJAMBI_OBJECTUSERDATA_ID_TYPE uint
#define QTJAMBI_GET_OBJECTUSERDATA(TYPE, object) static_cast<TYPE*>(object->userData(TYPE::id()))
#define QTJAMBI_SET_OBJECTUSERDATA(TYPE, object, data) object->setUserData(TYPE::id(), data)
#define QTJAMBI_SET_OBJECTUSERDATA_ID(ID, object, data) object->setUserData(ID, data)
#define QTJAMBI_OBJECTUSERDATA_ID_DECL static uint id();
#define QTJAMBI_OBJECTUSERDATA_ID_IMPL(STATIC,SCOPE)STATIC uint SCOPE id(){\
    static uint _id = QObject::registerUserData();\
    return _id;\
}
QTJAMBI_EXPORT QDebug& debug_stream(QDebug& debug, uint typeId, const void* ptr);
#else
struct QTJAMBI_EXPORT QtJambiObjectData{
protected:
    QtJambiObjectData();
public:
    virtual ~QtJambiObjectData();
    static QtJambiObjectData* userData(const QObject* object, const std::type_info& id);
    static void setUserData(QObject* object, const std::type_info& id, QtJambiObjectData* data);

    template<typename T>
    static T* userData(const QObject* object){
        return static_cast<T*>(userData(object, typeid(T)));
    }

    template<typename T>
    static void setUserData(QObject* object, T* data){
        setUserData(object, typeid(T), data);
    }

    static bool isRejectedUserProperty(const QObject* object, const char * propertyName);
};
#define QTJAMBI_OBJECTUSERDATA_TYPE_ID(TYPE) typeid(TYPE)
#define QTJAMBI_OBJECTUSERDATA_ID_TYPE const std::type_info&
#define QTJAMBI_OBJECTUSERDATA_ID_DECL
#define QTJAMBI_OBJECTUSERDATA_ID_IMPL(STATIC,SCOPE)
#define QTJAMBI_GET_OBJECTUSERDATA(TYPE, object) QtJambiObjectData::userData<TYPE>(object)
#define QTJAMBI_SET_OBJECTUSERDATA(TYPE, object, data) QtJambiObjectData::setUserData<TYPE>(object, data)
#define QTJAMBI_SET_OBJECTUSERDATA_ID(ID, object, data) QtJambiObjectData::setUserData(object, ID, data)
#endif

class QTJAMBI_EXPORT ApplicationData : public QtJambiObjectData
{
public:
    ApplicationData(JNIEnv *env, jobjectArray array);
    ~ApplicationData() override;

    char** chars();
    int& size();
    QTJAMBI_OBJECTUSERDATA_ID_DECL
private:
    int m_size;
    char** m_chars;
    Q_DISABLE_COPY_MOVE(ApplicationData)
};

QTJAMBI_EXPORT jobject qtjambi_find_object(JNIEnv *env, const void * pointer);

QTJAMBI_EXPORT jobject qtjambi_metamethod_invoke
    (JNIEnv * env, jobject _metaMethod, jobject _qobject, jobjectArray argClassTypeArray, jint connection, jobjectArray args);

QTJAMBI_EXPORT jobject qtjambi_metamethod_invoke_on_gadget
    (JNIEnv * env, jobject _metaMethod, jobject object, jobjectArray argClassTypeArray, jobjectArray args);

QTJAMBI_EXPORT jobject qtjambi_metamethod_to_reflected(JNIEnv * env, jobject _this);

QTJAMBI_EXPORT jobject qtjambi_metamethod_parameter_class_types(JNIEnv * env, jobject _this);

QTJAMBI_EXPORT jobject qtjambi_metamethod_return_class_type(JNIEnv * env, jobject _this);

QTJAMBI_EXPORT jobject qtjambi_method_from_javamethod(JNIEnv * env, jlong metaObjectPointer, jobjectArray ok);

QTJAMBI_EXPORT jboolean qtjambi_metaproperty_reset_on_gadget
    (JNIEnv *env, jobject _this, jobject gadget);

QTJAMBI_EXPORT jclass qtjambi_metaproperty_class
    (JNIEnv *env, jobject _this);

QTJAMBI_EXPORT jobject qtjambi_metaproperty_read_on_gadget
    (JNIEnv *env, jobject _this, jobject gadget);

QTJAMBI_EXPORT jboolean qtjambi_metaproperty_write_on_gadget
    (JNIEnv *env, jobject _this, jobject gadget, jobject value);

class QIODevice;
QTJAMBI_EXPORT QIODevice* qtjambi_new_direct_address_device
    (qint64 capacity, char* address, bool readOnly, JNIEnv* env, jobject buffer, QObject *parent = nullptr);

QTJAMBI_EXPORT jobject qtjambi_metaobject_javatype
    (JNIEnv *env, const QMetaObject *metaObject, bool exactOrNull = false);

QTJAMBI_EXPORT jobject qtjambi_metaobject_cast(JNIEnv *env, jobject object, jclass type);

QTJAMBI_EXPORT QMetaMethod qtjambi_metaobject_get_method(const QMetaObject *metaObject, const QString& sig);

QTJAMBI_EXPORT jobject qtjambi_metaobject_new_instance(JNIEnv *env, jlong metaObjectPointer, jint methodIndex, jobject argTypes, jobjectArray args);

QTJAMBI_EXPORT const QMetaObject *qtjambi_metaobject_for_class(JNIEnv *env, jclass java_class, const QMetaObject *original_meta_object = nullptr, bool hasCustomMetaObject = false);

QTJAMBI_EXPORT QString qtjambi_external_type_name(JNIEnv *env, const QString& internalName);

QTJAMBI_EXPORT jobject qtjambi_install_message_handler(JNIEnv *env, jobject supportedMessageTypes, jobject handler);

typedef bool(*ThreadedPixmapsChecker)();
QTJAMBI_EXPORT void qtjambi_install_threaded_pixmaps_checker(ThreadedPixmapsChecker threadedPixmapsChecker);

QTJAMBI_EXPORT jclass qtjambi_class_for_metatype(JNIEnv *env, int id);

QTJAMBI_EXPORT jobject qtjambi_invoke_function_pointer(JNIEnv * __jni_env, QFunctionPointer __qt_this, jobject returnType, jobjectArray arguments);

QTJAMBI_EXPORT jobject qtjambi_function_pointer_cast(JNIEnv * __jni_env, jobject function, jclass functionalInterface);

QTJAMBI_EXPORT jclass qtjambi_function_pointer_return_type(JNIEnv * __jni_env, jobject returnType);

QTJAMBI_EXPORT jobject qtjambi_function_pointer_convert_return(JNIEnv * __jni_env, jobject returnType, jobject result);

QTJAMBI_EXPORT void qtjambi_function_pointer_get_parameter_types(JNIEnv * __jni_env, jobjectArray argumentTypes, jobjectArray parameterTypes, jboolean isVarArgs);

QTJAMBI_EXPORT void qtjambi_function_pointer_convert_parameters(JNIEnv * __jni_env, jobjectArray argumentTypes, jobjectArray args, jobjectArray convertedArgs, jboolean isVarArgs, jboolean forward);

QTJAMBI_EXPORT void qtjambi_function_pointer_dispose(JNIEnv * __jni_env, jlong peer);

QTJAMBI_EXPORT void qtjambi_pre_exit();

QTJAMBI_EXPORT void qtjambi_unexit();

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QTJAMBI_EXPORT const QtPrivate::AbstractDebugStreamFunction * qtjambi_registered_debugstream_operator(int typeId);
#endif

QTJAMBI_EXPORT hash_type qHash(const QMetaType& metaType, const void* ptr, bool* success = nullptr);

QTJAMBI_EXPORT void qtjambi_exception_handler(JNIEnv *__jni_env, void* ptr, void(*expression)(void*));

#endif // QTJAMBI_APPLICATION_H
