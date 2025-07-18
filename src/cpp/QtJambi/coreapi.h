/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#if !defined(QTJAMBI_COREAPI_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBI_COREAPI_H

#include <QtCore/QVector>
#include <QtCore/QReadWriteLock>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QT_WARNING_DISABLE_GCC("-Wdeprecated-declarations")
QT_WARNING_DISABLE_DEPRECATED
#endif

#include "objectdata.h"

class J2CStringBuffer;
class QIODevice;
class QCoreApplication;
enum class QtJambiNativeID : jlong;

class QTJAMBI_EXPORT ApplicationData : public QtJambiObjectData
{
public:
    template<typename T>
    static std::unique_ptr<ApplicationData> initialize(JNIEnv *env, jobjectArray array){
        typedef typename std::remove_cv<typename std::remove_pointer<T>::type>::type T_;
        return initialize(env, array, typeid(T_), !std::is_same<T_, QCoreApplication>::value);
    }
    ~ApplicationData() override;

    char** chars();
    int& size();
    void update(JNIEnv *env);
    QTJAMBI_OBJECTUSERDATA_ID_DECL
private:
    static std::unique_ptr<ApplicationData> initialize(JNIEnv *env, jobjectArray array, const std::type_info& constructedType, bool checkMainThread = true);
    ApplicationData(int m_size, char** m_chars);
    int m_size;
    char** m_chars;
    Q_DISABLE_COPY_MOVE(ApplicationData)
};

namespace CoreAPI{

QTJAMBI_EXPORT jobject convertQObjectToJavaObjectCppOwnership(JNIEnv *env, const QObject *qt_object, const std::type_info& typeId);

QTJAMBI_EXPORT bool isJObjectWrappedMetaType(const QMetaType& metaType);

QTJAMBI_EXPORT bool isNativeWrapperMetaType(const QMetaType& metaType);

template<typename O>
jobject convertQObjectToJavaObjectCppOwnership(JNIEnv *env, const O *qt_object)
{
    return convertQObjectToJavaObjectCppOwnership(env, qt_object, typeid(O));
}

QTJAMBI_EXPORT void ckeckLinkExtension(JNIEnv *env, QtJambiNativeID nativeId);

QTJAMBI_EXPORT QReadWriteLock* objectDataLock();

QTJAMBI_EXPORT QIODevice* newDirectAddressDevice(JNIEnv* env, jobject buffer, QObject *parent = nullptr);

QTJAMBI_EXPORT jobject invokeMetaMethodOnGadget(JNIEnv * env, QtJambiNativeID _metaMethod, jobject object, jobjectArray args);

QTJAMBI_EXPORT jobject invokeMetaMethod(JNIEnv * env, QtJambiNativeID _metaMethod, QtJambiNativeID _qobject, jint connection, jobjectArray args);

QTJAMBI_EXPORT jobject invokeMetaMethod(JNIEnv * env, jlong metaObjectPointer, jint index, QtJambiNativeID qobject_id, jint connection, jobjectArray args);

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

QTJAMBI_EXPORT void * futurePrivate(const QFutureInterfaceBase * base);

#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
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
#else
QTJAMBI_EXPORT void registerQProperty(JNIEnv *env, QtJambiNativeID __object_nativeId, QtJambiNativeID property);
#endif

QTJAMBI_EXPORT void initializeQHash(JNIEnv *env, jobject object, jclass keyType, QtJambiNativeID keyMetaType, jclass valueType, QtJambiNativeID valueMetaType, jobject other);

QTJAMBI_EXPORT void initializeQMultiHash(JNIEnv *env, jobject object, jclass keyType, QtJambiNativeID keyMetaType, jclass valueType, QtJambiNativeID valueMetaType, jobject other);

QTJAMBI_EXPORT void initializeQMap(JNIEnv *env, jobject object, jclass keyType, QtJambiNativeID keyMetaType, jclass valueType, QtJambiNativeID valueMetaType, jobject other);

QTJAMBI_EXPORT void initializeQMultiMap(JNIEnv *env, jobject object, jclass keyType, QtJambiNativeID keyMetaType, jclass valueType, QtJambiNativeID valueMetaType, jobject other);

QTJAMBI_EXPORT jobject fromDestroyedQObject(JNIEnv *env, QObject* object);

QTJAMBI_EXPORT const char* getInterfaceIID(JNIEnv *env, jclass javaType);

QTJAMBI_EXPORT QList<const char*> getInterfaceIIDs(JNIEnv *env, jclass javaType);

QTJAMBI_EXPORT jobject getExtraSignal(JNIEnv *env, QtJambiNativeID sender__id, QtJambiNativeID method__id);

QTJAMBI_EXPORT jobject convertQFlagsToJavaObject(JNIEnv *env, jint qt_flags, jclass cls);

QTJAMBI_EXPORT jobject convertQFlagsToJavaObject(JNIEnv *env, jlong qt_flags, jclass cls);

QTJAMBI_EXPORT jobject convertEnumToJavaObject(JNIEnv *env, qint32 qt_enum, jclass cls);

QTJAMBI_EXPORT jobject convertEnumToJavaObject(JNIEnv *env, qint8 qt_enum, jclass cls);

QTJAMBI_EXPORT jobject convertEnumToJavaObject(JNIEnv *env, qint16 qt_enum, jclass cls);

QTJAMBI_EXPORT jobject convertEnumToJavaObject(JNIEnv *env, qint64 qt_enum, jclass cls);

QTJAMBI_EXPORT jobject convertEnumToJavaObject(JNIEnv *env, quint32 qt_enum, jclass cls);

QTJAMBI_EXPORT jobject convertEnumToJavaObject(JNIEnv *env, quint8 qt_enum, jclass cls);

QTJAMBI_EXPORT jobject convertEnumToJavaObject(JNIEnv *env, quint16 qt_enum, jclass cls);

QTJAMBI_EXPORT jobject convertEnumToJavaObject(JNIEnv *env, quint64 qt_enum, jclass cls);

template<typename E>
jobject convertEnumToJavaObject(JNIEnv *env, E qt_enum)
{
    return convertNativeToJavaObjectAsCopy(env, &qt_enum, typeid(E));
}

}

struct QtJambiSpan{
    const void* begin = nullptr;
    qsizetype size = 0;
    QTJAMBI_EXPORT static void deleter(void* ptr,bool);
};

struct ManagedSpanData{
    typedef void(*CommitFunction)(ManagedSpanData*,JNIEnv*);
    CommitFunction commitFunction = nullptr;
};

class QTJAMBI_EXPORT ManagedSpan : public QtJambiSpan{
private:
    QSharedPointer<ManagedSpanData> d;
public:
    ManagedSpan();
    ManagedSpan(QSharedPointer<ManagedSpanData>&& _data);
    ManagedSpan(const ManagedSpan&);
    void commit(JNIEnv* env);
    static void deleter(void* ptr,bool);
protected:
    template<typename Data>
    Data* data(){
        return static_cast<Data*>(d.data());
    }
};

#endif // QTJAMBI_COREAPI_H
