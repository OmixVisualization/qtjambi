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

#ifndef QTJAMBI_COREAPI_H
#define QTJAMBI_COREAPI_H

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QtCore/qnativeinterface.h>
#else
QT_WARNING_DISABLE_GCC("-Wdeprecated-declarations")
QT_WARNING_DISABLE_DEPRECATED
#endif

#include "qtjambiapi.h"
#include <QtCore/QVector>
#include <QtCore/QReadWriteLock>

class J2CStringBuffer;
class QIODevice;

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
namespace CoreAPI{
    QTJAMBI_EXPORT QDebug& appendToDebugStream(QDebug& debug, uint typeId, const void* ptr);
}
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
    Q_DISABLE_COPY_MOVE(QtJambiObjectData)
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
    void update(JNIEnv *env);
    QTJAMBI_OBJECTUSERDATA_ID_DECL
private:
    int m_size;
    char** m_chars;
    Q_DISABLE_COPY_MOVE(ApplicationData)
};

namespace CoreAPI{

QTJAMBI_EXPORT jobject convertQObjectToJavaObjectCppOwnership(JNIEnv *env, const QObject *qt_object, const std::type_info& typeId);

template<typename O>
jobject convertQObjectToJavaObjectCppOwnership(JNIEnv *env, const O *qt_object)
{
    return convertQObjectToJavaObjectCppOwnership(env, qt_object, typeid(O));
}

QTJAMBI_EXPORT void ckeckLinkExtension(JNIEnv *env, QtJambiNativeID nativeId);

QTJAMBI_EXPORT QReadWriteLock* objectDataLock();

QTJAMBI_EXPORT jobject findObject(JNIEnv *env, const void * pointer);

QTJAMBI_EXPORT QIODevice* newDirectAddressDevice(JNIEnv* env, jobject buffer, QObject *parent = nullptr);

QTJAMBI_EXPORT jobject invokeMetaMethodOnGadget(JNIEnv * env, jobject _metaMethod, jobject object, jobjectArray argClassTypeArray, jobjectArray args);

QTJAMBI_EXPORT jobject invokeMetaMethod(JNIEnv * env, jobject _metaMethod, jobject _qobject, jobjectArray argClassTypeArray, jint connection, jobjectArray args);

QTJAMBI_EXPORT jobject convertMetaMethodToReflected(JNIEnv * env, jobject _this);

QTJAMBI_EXPORT jobject convertReflectedMethodToMeta(JNIEnv * env, jlong metaObjectPointer, jobjectArray ok);

QTJAMBI_EXPORT jobject getMetaMethodParameterClassTypes(JNIEnv * env, jobject _this);

QTJAMBI_EXPORT jobject getMetaMethodReturnClassType(JNIEnv * env, jobject _this);

QTJAMBI_EXPORT jboolean resetMetaPropertyOnGadget(JNIEnv *env, jobject _this, jobject gadget);

QTJAMBI_EXPORT jclass getMetaPropertyClassType(JNIEnv *env, jobject _this);

QTJAMBI_EXPORT jobject readMetaPropertyOnGadget(JNIEnv *env, jobject _this, jobject gadget);

QTJAMBI_EXPORT jboolean writeMetaPropertyOnGadget(JNIEnv *env, jobject _this, jobject gadget, jobject value);

QTJAMBI_EXPORT jobject getMetaObjectJavaType(JNIEnv *env, const QMetaObject *metaObject, bool exactOrNull = false);

QTJAMBI_EXPORT jobject metaObjectCast(JNIEnv *env, jobject object, jclass type);

QTJAMBI_EXPORT QMetaMethod findMetaMethod(const QMetaObject *metaObject, const QString& sig);

QTJAMBI_EXPORT jobject newInstanceForMetaObject(JNIEnv *env, QtJambiNativeID constructor, jobjectArray args);

QTJAMBI_EXPORT const QMetaObject *metaObjectForClass(JNIEnv *env, jclass java_class, const QMetaObject *original_meta_object = nullptr, bool hasCustomMetaObject = false);

QTJAMBI_EXPORT QString externalTypeTame(JNIEnv *env, const QString& internalName);

QTJAMBI_EXPORT jobject installMessageHandler(JNIEnv *env, jobject supportedMessageTypes, jobject handler);

QTJAMBI_EXPORT jclass getClassForMetaType(JNIEnv *env, const QMetaType& metaType);

QTJAMBI_EXPORT jobject invokeFunctionPointer(JNIEnv * __jni_env, QFunctionPointer __qt_this, jobject returnType, jobjectArray arguments);

QTJAMBI_EXPORT jobject castFunctionPointer(JNIEnv * __jni_env, jobject function, jclass functionalInterface);

QTJAMBI_EXPORT jclass getFunctionPointerReturnType(JNIEnv * __jni_env, jobject returnType);

QTJAMBI_EXPORT void getFunctionPointerParameterTypes(JNIEnv * __jni_env, jobjectArray argumentTypes, jobjectArray parameterTypes, jboolean isVarArgs);

QTJAMBI_EXPORT jobject convertFunctionPointerReturn(JNIEnv * __jni_env, jobject returnType, jobject result);

QTJAMBI_EXPORT void convertFunctionPointerParameters(JNIEnv * __jni_env, jobjectArray argumentTypes, jobjectArray args, jobjectArray convertedArgs, jboolean isVarArgs, jboolean forward);

QTJAMBI_EXPORT void disposeFunctionPointer(JNIEnv * __jni_env, jlong peer);

QTJAMBI_EXPORT void preExit();

QTJAMBI_EXPORT void unexit();

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QTJAMBI_EXPORT const QtPrivate::AbstractDebugStreamFunction * registeredDebugStreamOperator(int typeId);
#endif

QTJAMBI_EXPORT hash_type computeHash(const QMetaType& metaType, const void* ptr, hash_type seed = 0, bool* success = nullptr);

QTJAMBI_EXPORT void invokeAndCatch(JNIEnv *__jni_env, void* ptr, void(*expression)(void*));

QTJAMBI_EXPORT QVariant convertCheckedObjectToQVariant(JNIEnv *env, jobject object, const QMetaType& metaType);

QTJAMBI_EXPORT int metaTypeId(JNIEnv *env, jclass clazz, jobjectArray instantiations = nullptr);

QTJAMBI_EXPORT int registerMetaType(JNIEnv *env, jclass containerType, jobjectArray instantiations);

#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0) && QT_VERSION < QT_VERSION_CHECK(6, 5, 0)
QTJAMBI_EXPORT void cleanContinuation(JNIEnv*, QFutureInterfaceBase* b);
#endif

QTJAMBI_EXPORT const QHash<int,const char*>* renamedMethods(const QMetaObject* metaObject);

QTJAMBI_EXPORT jclass getInterfaceByIID(JNIEnv *env, const char* iid);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
struct NITypeInfo{
    const char* name;
    int revision;
};
QTJAMBI_EXPORT NITypeInfo getNativeInterfaceInfo(JNIEnv * __jni_env, jclass cls);
#endif

QTJAMBI_EXPORT jobject javaObject(QtJambiNativeID nativeId, JNIEnv* env);

QTJAMBI_EXPORT void registerDependentInterface(JNIEnv *env, jobject dependentObject, jobject owner);

QTJAMBI_EXPORT void registerDependentObject(JNIEnv *env, jobject dependentObject, jobject owner);

QTJAMBI_EXPORT void unregisterDependentInterface(JNIEnv *env, jobject dependentObject, jobject owner);

QTJAMBI_EXPORT void unregisterDependentObject(JNIEnv *env, jobject dependentObject, jobject owner);

QTJAMBI_EXPORT void initializeQList(JNIEnv *env, jobject object, jclass elementType, QtJambiNativeID elementMetaType, jobject other);

QTJAMBI_EXPORT void initializeQSet(JNIEnv *env, jobject object, jclass elementType, QtJambiNativeID elementMetaType, jobject other);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QTJAMBI_EXPORT void initializeQLinkedList(JNIEnv *env, jobject object, jclass elementType, QtJambiNativeID elementMetaType, jobject other);

QTJAMBI_EXPORT void initializeQVector(JNIEnv *env, jobject object, jclass elementType, QtJambiNativeID elementMetaType, jobject other);
#endif

QTJAMBI_EXPORT void initializeQHash(JNIEnv *env, jobject object, jclass keyType, QtJambiNativeID keyMetaType, jclass valueType, QtJambiNativeID valueMetaType, jobject other);

QTJAMBI_EXPORT void initializeQMultiHash(JNIEnv *env, jobject object, jclass keyType, QtJambiNativeID keyMetaType, jclass valueType, QtJambiNativeID valueMetaType, jobject other);

QTJAMBI_EXPORT void initializeQMap(JNIEnv *env, jobject object, jclass keyType, QtJambiNativeID keyMetaType, jclass valueType, QtJambiNativeID valueMetaType, jobject other);

QTJAMBI_EXPORT void initializeQMultiMap(JNIEnv *env, jobject object, jclass keyType, QtJambiNativeID keyMetaType, jclass valueType, QtJambiNativeID valueMetaType, jobject other);

QTJAMBI_EXPORT jobject fromDestroyedQObject(JNIEnv *env, QObject* object);

QTJAMBI_EXPORT const char* getInterfaceIID(JNIEnv *env, jclass javaType);

QTJAMBI_EXPORT QList<const char*> getInterfaceIIDs(JNIEnv *env, jclass javaType);

QTJAMBI_EXPORT jobject getExtraSignal(JNIEnv *env, QtJambiNativeID sender__id, QtJambiNativeID method__id);

}

#endif // QTJAMBI_COREAPI_H
