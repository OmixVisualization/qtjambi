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

#if !defined(QTJAMBIAPI_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBIAPI_H

#include <QtCore/QString>
#include <QtCore/QSharedDataPointer>
#include <QtCore/QException>
#include <QtCore/QMetaEnum>
#include <QtCore/QMetaMethod>
#include <QtCore/QMetaProperty>
#include <functional>
#include <typeinfo>

#include "jnienvironment.h"
#include "debugapi.h"
#include "qtjambishell.h"
#include "exception.h"
#include "javaarrays.h"
#include "javabuffers.h"
#include "javastrings.h"
#include "javainvalidate.h"
#include "scope.h"

QT_WARNING_DISABLE_CLANG("-Wshift-count-overflow")

class QFutureInterfaceBase;
class QFutureWatcherBase;
class QPaintDevice;
class QtJambiScope;
class AbstractSequentialConstIteratorAccess;
class AbstractAssociativeConstIteratorAccess;
class AbstractListAccess;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
class AbstractSpanAccess;
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)

class AbstractSetAccess;
class AbstractLinkedListAccess;
class AbstractVectorAccess;
class AbstractHashAccess;
class AbstractMultiHashAccess;
class AbstractMapAccess;
class AbstractMultiMapAccess;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
template <typename T> class QFuture;
#endif
enum class QtJambiNativeID : jlong { Invalid = 0 };

#define InvalidNativeID QtJambiNativeID::Invalid

QTJAMBI_EXPORT bool operator !(QtJambiNativeID nativeId);
QTJAMBI_EXPORT bool operator &&(QtJambiNativeID nativeId, QtJambiNativeID nativeId2);
QTJAMBI_EXPORT bool operator &&(QtJambiNativeID nativeId, bool b2);
QTJAMBI_EXPORT bool operator &&(bool b1, QtJambiNativeID nativeId);
QTJAMBI_EXPORT bool operator ||(QtJambiNativeID nativeId, QtJambiNativeID nativeId2);
QTJAMBI_EXPORT bool operator ||(QtJambiNativeID nativeId, bool b2);
QTJAMBI_EXPORT bool operator ||(bool b1, QtJambiNativeID nativeId);

template<typename BoolSupplier>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
typename std::enable_if<std::is_invocable_r<bool, BoolSupplier>::value, bool>::type
#else
bool
#endif
operator &&(QtJambiNativeID nativeId, BoolSupplier&& b2){
    return nativeId!=InvalidNativeID && b2();
}

template<typename BoolSupplier>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
typename std::enable_if<std::is_invocable_r<bool, BoolSupplier>::value, bool>::type
#else
bool
#endif
operator ||(QtJambiNativeID nativeId, BoolSupplier&& b2){
    return nativeId!=InvalidNativeID || b2();
}

typedef const std::type_info* (*TypeInfoSupplier)(const void *object);

namespace QtJambiAPI{

enum ConstructorOptions : int{
    NoConstructorOption = 0,
    HasDerivedMetaObject = 0x01,
    HasOverrides = 0x02,
    IsNativeConstruction = 0x04,
    IsQmlConstruction = 0x08 | IsNativeConstruction,
};
typedef void (*ConstructorFn)(void*, JNIEnv*, jobject, jvalue*, QtJambiAPI::ConstructorOptions);

void QTJAMBI_EXPORT checkNullPointer(JNIEnv *env, const void* ptr, const std::type_info& typeId);
void QTJAMBI_EXPORT checkNullPointer(JNIEnv *env, const void* ptr, const std::type_info& typeId, TypeInfoSupplier typeInfoSupplier);
void QTJAMBI_EXPORT checkDanglingPointer(JNIEnv *env, const void* ptr, const std::type_info& typeId, TypeInfoSupplier typeInfoSupplier);
}//QtJambiAPI

namespace QtJambiPrivate{
template<typename T, bool = std::is_polymorphic<T>::value>
struct CheckPointer{
    static const std::type_info* supplyType(const void *ptr) {
        const T* object = reinterpret_cast<const T*>(ptr);
        try{
            const std::type_info* typeId = &typeid(*object);
            if(!typeId)
                typeId = &typeid(T);
            return typeId;
        }catch(const std::bad_typeid&){
            return nullptr;
        }catch(...){
            return nullptr;
        }
    };
    static const std::type_info* trySupplyType(const T *object) {
        try{
            const std::type_info* typeId = &typeid(*object);
            return typeId;
        }catch(const std::bad_typeid&){
            return nullptr;
        }catch(...){
            return nullptr;
        }
    };
    static void checkNullPointer(JNIEnv *env, const T* ptr){
        QtJambiAPI::checkNullPointer(env, ptr, typeid(T), supplyType);
    }
    static void checkDanglingPointer(JNIEnv *env, const T* ptr){
        QtJambiAPI::checkDanglingPointer(env, ptr, typeid(T), supplyType);
    }
};
template<typename T>
struct CheckPointer<T,false>{
    static constexpr TypeInfoSupplier supplyType = nullptr;
    static void checkNullPointer(JNIEnv *env, const T* ptr){
        QtJambiAPI::checkNullPointer(env, ptr, typeid(T));
    }
    static void checkDanglingPointer(JNIEnv *, const T*){
    }
};
}//QtJambiPrivate

namespace QtJambiAPI{

template<typename T>
void checkNullPointer(JNIEnv *env, const T* ptr)
{
    QtJambiPrivate::CheckPointer<T>::checkNullPointer(env, ptr);
}

template<typename T>
void checkDanglingPointer(JNIEnv *env, const T* ptr)
{
    QtJambiPrivate::CheckPointer<T>::checkDanglingPointer(env, ptr);
}

template<class T>
T& checkedAddressOf(JNIEnv *env, T * ptr)
{
    QtJambiPrivate::CheckPointer<T>::checkNullPointer(env, ptr);
    return *ptr;
}

QTJAMBI_EXPORT QByteArray typeName(const std::type_info* typeId);

QTJAMBI_EXPORT QByteArray typeName(const std::type_info& typeId);

QTJAMBI_EXPORT jintArray toJIntArray(JNIEnv *__jni_env, const jint* in, jsize length);
QTJAMBI_EXPORT jshortArray toJShortArray(JNIEnv *__jni_env, const jshort* in, jsize length);
QTJAMBI_EXPORT jbyteArray toJByteArray(JNIEnv *__jni_env, const jbyte* in, jsize length);
QTJAMBI_EXPORT jlongArray toJLongArray(JNIEnv *__jni_env, const jlong* in, jsize length);
QTJAMBI_EXPORT jfloatArray toJFloatArray(JNIEnv *__jni_env, const jfloat* in, jsize length);
QTJAMBI_EXPORT jdoubleArray toJDoubleArray(JNIEnv *__jni_env, const jdouble* in, jsize length);
QTJAMBI_EXPORT jcharArray toJCharArray(JNIEnv *__jni_env, const jchar* in, jsize length);
QTJAMBI_EXPORT jbooleanArray toJBooleanArray(JNIEnv *__jni_env, const jboolean* in, jsize length);

QTJAMBI_EXPORT bool convertJavaToNative(JNIEnv *env, const std::type_info& typeId, jobject java_object, void * output, QtJambiScope* scope = nullptr);

QTJAMBI_EXPORT bool convertJavaToNative(JNIEnv *env, const std::type_info& typeId, const char* qtName, const char* javaName, jobject java_object, void * output, QtJambiScope* scope = nullptr);

QTJAMBI_EXPORT bool convertJavaToNative(JNIEnv *env, const std::type_info& typeId, const char* typeName, jobject java_object, void * output, QtJambiScope* scope = nullptr);

QTJAMBI_EXPORT bool isShell(QtJambiNativeID nativeId);

QTJAMBI_EXPORT bool javaObjectHasShell(JNIEnv *env, jobject object);

QTJAMBI_EXPORT bool javaInterfaceHasShell(JNIEnv *env, jobject object);

QTJAMBI_EXPORT QtJambiNativeID javaObjectToNativeId(JNIEnv *env, jobject object);

QTJAMBI_EXPORT QtJambiNativeID javaInterfaceToNativeId(JNIEnv *env, jobject object);

QTJAMBI_EXPORT void *fromNativeId(QtJambiNativeID nativeId);

QTJAMBI_EXPORT void *fromNativeId(QtJambiNativeID nativeId, const std::type_info& typeId);

template<typename T>
T *objectFromNativeId(QtJambiNativeID nativeId)
{
    return reinterpret_cast<T*>(fromNativeId(nativeId));
}

template<typename T>
T *interfaceFromNativeId(QtJambiNativeID nativeId)
{
    return reinterpret_cast<T*>(fromNativeId(nativeId, typeid(T)));
}

template<typename T>
T& objectReferenceFromNativeId(JNIEnv *env, QtJambiNativeID nativeId)
{
    return checkedAddressOf<T>(env, objectFromNativeId<T>(nativeId));
}

template<typename T>
T& interfaceReferenceFromNativeId(JNIEnv *env, QtJambiNativeID nativeId)
{
    return checkedAddressOf<T>(env, interfaceFromNativeId<T>(nativeId));
}

QTJAMBI_EXPORT QVariant convertJavaObjectToQVariant(JNIEnv *env, jobject java_object);

QTJAMBI_EXPORT jobject convertQVariantToJavaObject(JNIEnv *env, const QVariant &qt_variant);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
QTJAMBI_EXPORT void convertJavaObjectToQModelRoleData(JNIEnv *env, QtJambiScope& scope, jobject java_object, void * &data, qsizetype &length);
#endif

QTJAMBI_EXPORT void *convertJavaObjectToNative(JNIEnv *env, jobject java_object);

template<typename T>
T *convertJavaObjectToNative(JNIEnv *env, jobject java_object)
{
    return reinterpret_cast<T*>(convertJavaObjectToNative(env, java_object));
}

template<typename T>
T& convertJavaObjectToNativeReference(JNIEnv *env, jobject java_object)
{
    return checkedAddressOf<T>(env, reinterpret_cast<T*>(convertJavaObjectToNative(env, java_object)));
}

QTJAMBI_EXPORT void *convertJavaInterfaceToNative(JNIEnv *env, jobject java_object, const char *interface_name, const std::type_info& typeId);

QTJAMBI_EXPORT void *convertJavaInterfaceToNative(JNIEnv *env, jobject java_object, const std::type_info& typeId);

template<typename T>
T *convertJavaInterfaceToNative(JNIEnv *env, jobject java_object)
{
    return reinterpret_cast<T*>(convertJavaInterfaceToNative(env, java_object, typeid(T)));
}

template<typename T>
T& convertJavaInterfaceToNativeReference(JNIEnv *env, jobject java_object)
{
    return checkedAddressOf<T>(env, convertJavaInterfaceToNative<T>(env, java_object));
}

template<typename T>
T *convertJavaInterfaceToNative(JNIEnv *env, jobject java_object, const char *interface_name)
{
    return reinterpret_cast<T*>(convertJavaInterfaceToNative(env, java_object, interface_name, typeid(T)));
}

template<typename T>
T& convertJavaInterfaceToNativeReference(JNIEnv *env, jobject java_object, const char *interface_name)
{
    return checkedAddressOf<T>(env, convertJavaInterfaceToNative<T>(env, java_object, interface_name));
}

QTJAMBI_EXPORT QObject *convertJavaObjectToQObject(JNIEnv *env, jobject java_object);

template<typename T>
T *convertJavaObjectToQObject(JNIEnv *env, jobject java_object)
{
    return dynamic_cast<T*>(convertJavaObjectToQObject(env, java_object));
}

QTJAMBI_EXPORT bool enumValue(JNIEnv *env, jobject java_object, void* ptr, size_t size);

template<typename I = int>
I enumValue(JNIEnv *env, jobject object){
    I i{};
    enumValue(env, object, &i, sizeof(I));
    return i;
}

QTJAMBI_EXPORT jstring toJavaString(JNIEnv *env, jobject object);

QTJAMBI_EXPORT QSharedPointer<QObject> convertJavaObjectToQSharedPointer(JNIEnv *env, jobject java_object);

QTJAMBI_EXPORT QSharedPointer<char> convertJavaObjectToQSharedPointer(JNIEnv *env,
                                                                             const std::type_info* typeId,
                                                                             jobject java_object);

QTJAMBI_EXPORT QWeakPointer<QObject> convertJavaObjectToQWeakPointer(JNIEnv *env, jobject java_object);

QTJAMBI_EXPORT QWeakPointer<char> convertJavaObjectToQWeakPointer(JNIEnv *env,
                                                                      const std::type_info* typeId,
                                                                      jobject java_object);

QTJAMBI_EXPORT std::shared_ptr<QObject> convertJavaObjectToSharedPtr(JNIEnv *env, jobject java_object);

QTJAMBI_EXPORT std::shared_ptr<char> convertJavaObjectToSharedPtr(JNIEnv *env,
                                                                          const std::type_info* typeId,
                                                                          jobject java_object);

QTJAMBI_EXPORT std::weak_ptr<QObject> convertJavaObjectToWeakPtr(JNIEnv *env, jobject java_object);

QTJAMBI_EXPORT std::weak_ptr<char> convertJavaObjectToWeakPtr(JNIEnv *env,
                                                                  const std::type_info* typeId,
                                                                  jobject java_object);

QTJAMBI_EXPORT jobject convertSmartPointerToJavaObject(JNIEnv *env,
                                                       const std::type_info& typeId,
                                                       const QSharedPointer<QObject>& smartPointer);
QTJAMBI_EXPORT jobject convertSmartPointerToJavaObject(JNIEnv *env,
                                                       const std::type_info& typeId,
                                                       const QSharedPointer<char>& smartPointer);
QTJAMBI_EXPORT jobject convertSmartPointerToJavaObject(JNIEnv *env,
                                                       const std::type_info& typeId,
                                                       const std::shared_ptr<QObject>& smartPointer);
QTJAMBI_EXPORT jobject convertSmartPointerToJavaObject(JNIEnv *env,
                                                       const std::type_info& typeId,
                                                       const std::shared_ptr<char>& smartPointer);

}//namespace QtJambiAPI

namespace QtJambiPrivate{

template<template<typename> class SmartPointer, typename T, bool = std::is_base_of<QObject,T>::value>
struct JavaObjectToNativeAsSmartPointerConverter{
};

template<typename T>
struct JavaObjectToNativeAsSmartPointerConverter<QSharedPointer,T,false>{
    static QSharedPointer<T> convert(JNIEnv *env,
                                     jobject java_object){
        QSharedPointer<char> sp = QtJambiAPI::convertJavaObjectToQSharedPointer(env, &typeid(T), java_object);
        char* _ptr = sp.get();
        return QtSharedPointer::copyAndSetPointer(reinterpret_cast<T*>(_ptr), sp);
    }
    static QSharedPointer<char> convertSmartPointer(const QSharedPointer<T>& sp){
        T* _ptr = sp.get();
        return QtSharedPointer::copyAndSetPointer(reinterpret_cast<char*>(_ptr), sp);
    }
};

template<typename T>
struct JavaObjectToNativeAsSmartPointerConverter<std::shared_ptr,T,false>{
    static std::shared_ptr<T> convert(JNIEnv *env,
                                      jobject java_object){
        std::shared_ptr<char> sp = QtJambiAPI::convertJavaObjectToSharedPtr(env, &typeid(T), java_object);
        char* _ptr = sp.get();
        return std::shared_ptr<T>(sp, reinterpret_cast<T*>(_ptr));
    }
    static std::shared_ptr<char> convertSmartPointer(const std::shared_ptr<T>& sp){
        T* _ptr = sp.get();
        return std::shared_ptr<char>(sp, reinterpret_cast<char*>(_ptr));
    }
};

template<typename T>
struct JavaObjectToNativeAsSmartPointerConverter<QSharedPointer,T,true>{
    static QSharedPointer<T> convert(JNIEnv *env,
                                     jobject java_object){
        QSharedPointer<QObject> sp = QtJambiAPI::convertJavaObjectToQSharedPointer(env, java_object);
        return sp.dynamicCast<T>();
    }
    static QSharedPointer<QObject> convertSmartPointer(const QSharedPointer<T>& sp){
        return sp.template dynamicCast<QObject>();
    }
};

template<typename T>
struct JavaObjectToNativeAsSmartPointerConverter<std::shared_ptr,T,true>{
    static std::shared_ptr<T> convert(JNIEnv *env,
                                      jobject java_object){
        std::shared_ptr<QObject> sp = QtJambiAPI::convertJavaObjectToSharedPtr(env, java_object);
        return std::dynamic_pointer_cast<T>(sp);
    }
    static std::shared_ptr<QObject> convertSmartPointer(const std::shared_ptr<T>& sp){
        return std::dynamic_pointer_cast<T>(sp);
    }
};

template<>
struct JavaObjectToNativeAsSmartPointerConverter<std::shared_ptr,QObject,true>{
    static std::shared_ptr<QObject> convert(JNIEnv *env,
                                            jobject java_object){
        return QtJambiAPI::convertJavaObjectToSharedPtr(env, java_object);
    }
    static const std::shared_ptr<QObject>& convertSmartPointer(const std::shared_ptr<QObject>& sp){
        return sp;
    }
};

template<>
struct JavaObjectToNativeAsSmartPointerConverter<QSharedPointer,QObject,true>{
    static QSharedPointer<QObject> convert(JNIEnv *env,
                                           jobject java_object){
        return QtJambiAPI::convertJavaObjectToQSharedPointer(env, java_object);
    }
    static const QSharedPointer<QObject>& convertSmartPointer(const QSharedPointer<QObject>& sp){
        return sp;
    }
};

typedef const void* (*DefaultValueCreator)();
QTJAMBI_EXPORT const void* getDefaultValue(const std::type_info& type_info, DefaultValueCreator creator);

template<typename T>
const T& getDefaultValue(){
    return *reinterpret_cast<const T*>(getDefaultValue(typeid(T), []()->const void*{return new T();}));
}

}//namespace QtJambiPrivate

namespace QtJambiAPI{

template<template<typename> class SmartPointer, typename T>
SmartPointer<T> convertJavaObjectToSmartPointer(JNIEnv *env, jobject java_object){
    auto out = QtJambiPrivate::JavaObjectToNativeAsSmartPointerConverter<SmartPointer,T>::convert(env, java_object);
    return SmartPointer<T>(*reinterpret_cast<const SmartPointer<T>*>(&out));
}

template<template<typename> class SmartPointer, typename O>
inline jobject convertSmartPointerToJavaObject(JNIEnv *env, const SmartPointer<O> & smartPointer){
    return convertSmartPointerToJavaObject(env,
                typeid(O),
                QtJambiPrivate::JavaObjectToNativeAsSmartPointerConverter<SmartPointer,O>::convertSmartPointer(smartPointer));
}

QTJAMBI_EXPORT void setQQmlListPropertyElementType(JNIEnv *env, jobject list, jobject elementType);

QTJAMBI_EXPORT jobject convertQStringToJavaObject(JNIEnv *env, const QString &strg);

QTJAMBI_EXPORT jobject convertQStringToJavaObject(JNIEnv *env, QString *strg);

QTJAMBI_EXPORT jobject convertQVariantToJavaVariant(JNIEnv *env, const QVariant &variant);

QTJAMBI_EXPORT jobject convertQVariantToJavaVariant(JNIEnv *env, QVariant *variant);

QTJAMBI_EXPORT jobject convertQCharToJavaObject(JNIEnv *env, const QChar &strg);

QTJAMBI_EXPORT jobject convertQCharToJavaObject(JNIEnv *env, QChar *strg);

QTJAMBI_EXPORT bool isQStringObject(JNIEnv *env, jobject obj);

QTJAMBI_EXPORT bool isQVariantObject(JNIEnv *env, jobject obj);

QTJAMBI_EXPORT bool isQCharObject(JNIEnv *env, jobject obj);

QTJAMBI_EXPORT bool isQByteArrayObject(JNIEnv *env, jobject obj);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
QTJAMBI_EXPORT bool isQByteArrayViewObject(JNIEnv *env, jobject obj);
#endif

QTJAMBI_EXPORT bool isSequentialConstIterator(JNIEnv *env, jobject obj);

QTJAMBI_EXPORT bool isSequentialIterator(JNIEnv *env, jobject obj);

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
QTJAMBI_EXPORT bool isQSpanObject(JNIEnv *env, jobject obj);

QTJAMBI_EXPORT void commitQSpanObject(JNIEnv *env, jobject obj);

QTJAMBI_EXPORT QPair<void*,jlong> fromQSpanObject(JNIEnv *env, jobject obj, bool isConst, const QMetaType& metaType);
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)

QTJAMBI_EXPORT bool isJavaString(JNIEnv *env, jobject obj);

QTJAMBI_EXPORT bool isJavaCharSequence(JNIEnv *env, jobject obj);

QTJAMBI_EXPORT bool isJavaList(JNIEnv *env, jobject obj);

QTJAMBI_EXPORT bool isJavaCollection(JNIEnv *env, jobject obj);

QTJAMBI_EXPORT bool isJavaIterable(JNIEnv *env, jobject obj);

QTJAMBI_EXPORT jobject convertNativeToJavaOwnedObjectAsWrapper(JNIEnv *env, const void *qt_object, const std::type_info& typeId, const char *nativeTypeName = nullptr);

QTJAMBI_EXPORT jobject convertNativeToJavaOwnedObjectAsWrapper(JNIEnv *env, const void *qt_object, jclass clazz);

QTJAMBI_EXPORT jobject convertNativeToJavaObjectAsWrapper(JNIEnv *env, const void *qt_object, const std::type_info& typeId, const char *nativeTypeName = nullptr);

QTJAMBI_EXPORT jobject convertNativeToJavaObjectAsWrapper(JNIEnv *env, const void *qt_object, jclass clazz);

QTJAMBI_EXPORT jobject convertNativeToJavaObjectAsCopy(JNIEnv *env, const void *qt_object, const std::type_info& typeId, const char *nativeTypeName = nullptr);

QTJAMBI_EXPORT jobject convertNativeToJavaObjectAsCopy(JNIEnv *env, const void *qt_object, jclass clazz);

QTJAMBI_EXPORT jobject convertModelIndexToJavaObject(JNIEnv *env, const class QModelIndex& index);

QTJAMBI_EXPORT jobject convertModelIndexToEphemeralJavaObject(JNIEnv *env, QtJambiScope& scope, const class QModelIndex* index);

QTJAMBI_EXPORT bool convertJavaToModelIndex(JNIEnv *env, jobject java_object, class QModelIndex& output);

QTJAMBI_EXPORT bool convertJavaToModelIndex(JNIEnv *env, jobject java_object, class QModelIndex*& output, QtJambiScope* scope);

template<typename T>
jobject convertNativeToJavaOwnedObjectAsWrapper(JNIEnv *env, const T *qt_object, const char *nativeTypeName = nullptr)
{
    return convertNativeToJavaOwnedObjectAsWrapper(env, qt_object, typeid(T), nativeTypeName);
}

template<typename T>
jobject convertNativeToJavaObjectAsWrapper(JNIEnv *env, const T *qt_object, const char *nativeTypeName = nullptr)
{
    return convertNativeToJavaObjectAsWrapper(env, qt_object, typeid(T), nativeTypeName);
}

template<typename T>
jobject convertNativeToJavaObjectAsCopy(JNIEnv *env, const T *qt_object, const char *nativeTypeName = nullptr)
{
    return convertNativeToJavaObjectAsCopy(env, qt_object, typeid(T), nativeTypeName);
}

template<typename Ret, typename... Args>
struct FunctionType{
    typedef Ret(*type)(Args...);
    typedef Ret(signature)(Args...);
};

QTJAMBI_EXPORT jobject convertQObjectToJavaObject(JNIEnv *env, const QObject *qt_object, const char *className);

QTJAMBI_EXPORT jobject convertQObjectToJavaObject(JNIEnv *env, const QObject *qt_object, jclass clazz);

QTJAMBI_EXPORT jobject convertQObjectToJavaObject(JNIEnv *env, const QObject *qt_object, const std::type_info& typeId);

template<typename O>
jobject convertQObjectToJavaObject(JNIEnv *env, const O *qt_object)
{
    return convertQObjectToJavaObject(env, qt_object, typeid(O));
}

template<typename E>
jobject convertQFlagsToJavaObject(JNIEnv *env, QFlags<E> qt_flags)
{
    return convertNativeToJavaObjectAsCopy(env, &qt_flags, typeid(QFlags<E>));
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
QTJAMBI_EXPORT jstring convertNativeToJavaObject(JNIEnv *env, QAnyStringView s);
#endif

}//namespace QtJambiAPI

namespace QNativePointer{
    enum class Type{
        /** Java Boolean*/ Boolean = 0,
        /** Java Byte*/ Byte,
        /** Java Char*/ Char,
        /** Java Short*/ Short,
        /** Java Int*/ Int,
        /** Java Long*/ Long,
        /** Java Float*/ Float,
        /** Java Double*/ Double,
        /** Another QNativePointer of any type*/ Pointer,
        /** Java String*/ String
    };
}//namespace QNativePointer

namespace QtJambiAPI{

QTJAMBI_EXPORT void *convertQNativePointerToNative(JNIEnv *env, jobject java_object, int* size = nullptr, int* indirections = nullptr);

QTJAMBI_EXPORT jobject convertNativeToQNativePointer(JNIEnv *env, const void *qt_pointer, QNativePointer::Type type_id, quint64 size, uint indirections);

QTJAMBI_EXPORT QString getClassName(JNIEnv *env, jclass java_class);
QTJAMBI_EXPORT QString getObjectClassName(JNIEnv *env, jobject java_object);

QTJAMBI_EXPORT jobject toJavaIntegerObject(JNIEnv *env, jint int_value);
QTJAMBI_EXPORT jobject toJavaDoubleObject(JNIEnv *env, jdouble double_value);
QTJAMBI_EXPORT jobject toJavaBooleanObject(JNIEnv *env, jboolean bool_value);
QTJAMBI_EXPORT jobject toJavaLongObject(JNIEnv *env, jlong long_value);
QTJAMBI_EXPORT jobject toJavaShortObject(JNIEnv *env, jshort short_value);
QTJAMBI_EXPORT jobject toJavaFloatObject(JNIEnv *env, jfloat float_value);
QTJAMBI_EXPORT jobject toJavaByteObject(JNIEnv *env, jbyte byte_value);
QTJAMBI_EXPORT jobject toJavaCharacterObject(JNIEnv *env, jchar char_value);
QTJAMBI_EXPORT jobject convertDuration(JNIEnv *env, std::chrono::nanoseconds t);
QTJAMBI_EXPORT jobject convertDuration(JNIEnv *env, std::chrono::seconds t);
QTJAMBI_EXPORT jobject convertDuration(JNIEnv *env, std::chrono::milliseconds t);
QTJAMBI_EXPORT std::chrono::nanoseconds convertDuration(JNIEnv *env, jobject t, std::chrono::nanoseconds defaultValue = std::chrono::nanoseconds::zero());
QTJAMBI_EXPORT std::chrono::seconds convertDuration(JNIEnv *env, jobject t, std::chrono::seconds defaultValue = std::chrono::seconds::zero());
QTJAMBI_EXPORT std::chrono::milliseconds convertDuration(JNIEnv *env, jobject t, std::chrono::milliseconds defaultValue = std::chrono::milliseconds::zero());
QTJAMBI_EXPORT jobject convertTimePointFromEpoch(JNIEnv *env, std::chrono::seconds t);
QTJAMBI_EXPORT jobject convertTimePointFromEpoch(JNIEnv *env, std::chrono::nanoseconds t);
QTJAMBI_EXPORT jobject convertTimePointFromEpoch(JNIEnv *env, std::chrono::milliseconds t);
QTJAMBI_EXPORT std::chrono::nanoseconds convertTimePointFromEpoch(JNIEnv *env, jobject t, std::chrono::nanoseconds defaultValue = std::chrono::nanoseconds::zero());
QTJAMBI_EXPORT std::chrono::seconds convertTimePointFromEpoch(JNIEnv *env, jobject t, std::chrono::seconds defaultValue = std::chrono::seconds::zero());
QTJAMBI_EXPORT std::chrono::milliseconds convertTimePointFromEpoch(JNIEnv *env, jobject t, std::chrono::milliseconds defaultValue = std::chrono::milliseconds::zero());
QTJAMBI_EXPORT QPair<std::chrono::seconds, std::chrono::nanoseconds> readDuration(JNIEnv *env, jobject t);
QTJAMBI_EXPORT QPair<std::chrono::seconds, std::chrono::nanoseconds> readTimePoint(JNIEnv *env, jobject t);

QTJAMBI_EXPORT jdouble fromJavaDoubleObject(JNIEnv *env, jobject double_object);
QTJAMBI_EXPORT jint fromJavaIntegerObject(JNIEnv *env, jobject int_object);
QTJAMBI_EXPORT bool fromJavaBooleanObject(JNIEnv *env, jobject bool_object);
QTJAMBI_EXPORT jlong fromJavaLongObject(JNIEnv *env, jobject long_object);
QTJAMBI_EXPORT jchar fromJavaCharacterObject(JNIEnv *env, jobject char_object);
QTJAMBI_EXPORT jfloat fromJavaFloatObject(JNIEnv *env, jobject float_object);
QTJAMBI_EXPORT jshort fromJavaShortObject(JNIEnv *env, jobject short_object);
QTJAMBI_EXPORT jbyte fromJavaByteObject(JNIEnv *env, jobject byte_object);

QTJAMBI_EXPORT jobject newQPair(JNIEnv *env, jobject first, jobject second);
QTJAMBI_EXPORT jobject getQPairFirst(JNIEnv *env, jobject pair);
QTJAMBI_EXPORT jobject getQPairSecond(JNIEnv *env, jobject pair);

QTJAMBI_EXPORT jobject newJavaHashSet(JNIEnv *env);
QTJAMBI_EXPORT jobject newJavaHashMap(JNIEnv *env, int size = 0);
QTJAMBI_EXPORT jobject newJavaTreeMap(JNIEnv *env);

QTJAMBI_EXPORT void putJavaMap(JNIEnv *env, jobject map, jobject key, jobject val);
QTJAMBI_EXPORT void putJavaMultiMap(JNIEnv *env, jobject map, jobject key, jobject val);
QTJAMBI_EXPORT void clearJavaMap(JNIEnv *env, jobject map);
QTJAMBI_EXPORT jobject entrySetIteratorOfJavaMap(JNIEnv *env, jobject map);
QTJAMBI_EXPORT jobject keyOfJavaMapEntry(JNIEnv *env, jobject entry);
QTJAMBI_EXPORT jobject valueOfJavaMapEntry(JNIEnv *env, jobject entry);
QTJAMBI_EXPORT jobject newJavaArrayList(JNIEnv *env, jint size = 0);

QTJAMBI_EXPORT void addToJavaCollection(JNIEnv *env, jobject list, jobject obj);
QTJAMBI_EXPORT void addAllToJavaCollection(JNIEnv *env, jobject list, jobject obj);
QTJAMBI_EXPORT void clearJavaCollection(JNIEnv *env, jobject collection);
QTJAMBI_EXPORT int sizeOfJavaCollection(JNIEnv *env, jobject col);
QTJAMBI_EXPORT jobject iteratorOfJavaIterable(JNIEnv *env, jobject col);
QTJAMBI_EXPORT jobject nextOfJavaIterator(JNIEnv *env, jobject col);
QTJAMBI_EXPORT bool hasJavaIteratorNext(JNIEnv *env, jobject col);
QTJAMBI_EXPORT void setAtJavaList(JNIEnv *env, jobject list, jint index, jobject obj);

QTJAMBI_EXPORT jobject findObject(JNIEnv *env, const void * pointer);
QTJAMBI_EXPORT jobject findObject(JNIEnv *env, const QObject* pointer);
QTJAMBI_EXPORT jobject findFunctionPointerObject(JNIEnv *env, const void * pointer, const std::type_info& typeId);

template<typename T>
const T& valueReferenceFromNativeId(QtJambiNativeID nativeId){
    if(!!nativeId){
        if(const T* value = objectFromNativeId<T>(nativeId)){
            return *value;
        }
    }
    return QtJambiPrivate::getDefaultValue<T>();
}

template<typename T>
T valueFromNativeId(QtJambiNativeID nativeId){
    if(!!nativeId){
        if(const T* value = objectFromNativeId<T>(nativeId)){
            return *value;
        }
    }
    return T{};
}

bool QTJAMBI_EXPORT isValidArray(JNIEnv *env, jobject object, const std::type_info& typeId);

class DeclarativeUtil{
static QTJAMBI_EXPORT void reportDestruction(QObject * obj);
template<typename T>
friend class DeclarativeShellElement;
template<typename T>
friend class DeclarativeElement;
};

template<typename T>
class DeclarativeShellElement final : public T
{
public:
    template<typename... Args>
    DeclarativeShellElement(Args... args) : T(std::forward(args)...) {}
    ~DeclarativeShellElement() override {
        DeclarativeUtil::reportDestruction(this);
    }
    static void operator delete(void * ptr) Q_DECL_NOTHROW{
        T::operator delete(ptr);
    }
};

template<typename T>
class DeclarativeElement final : public T
{
public:
    DeclarativeElement() : T() {}
    ~DeclarativeElement() override {
        DeclarativeUtil::reportDestruction(this);
    }
};

QTJAMBI_EXPORT void registerNonShellDeletion(void* ptr);

QTJAMBI_EXPORT void setJavaOwnership(JNIEnv *env, jobject object);

QTJAMBI_EXPORT void setCppOwnership(JNIEnv *env, jobject object);

QTJAMBI_EXPORT bool isSplitOwnership(JNIEnv *env, jobject object);

QTJAMBI_EXPORT bool isCppOwnership(JNIEnv *env, jobject object);

QTJAMBI_EXPORT bool isJavaOwnership(JNIEnv *env, jobject object);

QTJAMBI_EXPORT void setCppOwnershipAndInvalidate(JNIEnv *env, jobject object);

QTJAMBI_EXPORT void setDefaultOwnership(JNIEnv *env, jobject object);

QTJAMBI_EXPORT void setJavaOwnership(JNIEnv *env, QtJambiNativeID objectId);

QTJAMBI_EXPORT void setCppOwnership(JNIEnv *env, QtJambiNativeID objectId);

QTJAMBI_EXPORT void changeSplitToCppOwnership(JNIEnv *env, QtJambiNativeID objectId);

QTJAMBI_EXPORT void changeSplitToCppOwnership(JNIEnv *env, jobject object);

QTJAMBI_EXPORT void setCppOwnershipAndInvalidate(JNIEnv *env, QtJambiNativeID objectId);

QTJAMBI_EXPORT void setDefaultOwnership(JNIEnv *env, QtJambiNativeID objectId);

QTJAMBI_EXPORT bool isSplitOwnership(QtJambiNativeID objectId);

QTJAMBI_EXPORT bool isCppOwnership(QtJambiNativeID objectId);

QTJAMBI_EXPORT bool isJavaOwnership(QtJambiNativeID objectId);

QTJAMBI_EXPORT bool isSplitOwnership(const QObject* object);

QTJAMBI_EXPORT bool isCppOwnership(const QObject* object);

QTJAMBI_EXPORT bool isJavaOwnership(const QObject* object);

QTJAMBI_EXPORT bool isSplitOwnership(const void* object);

QTJAMBI_EXPORT bool isCppOwnership(const void* object);

QTJAMBI_EXPORT bool isJavaOwnership(const void* object);

QTJAMBI_EXPORT void setJavaOwnershipForTopLevelObject(JNIEnv *env, QObject* qobject);

QTJAMBI_EXPORT void setCppOwnershipForTopLevelObject(JNIEnv *env, QObject* qobject);

QTJAMBI_EXPORT void setDefaultOwnershipForTopLevelObject(JNIEnv *env, QObject* qobject);

QTJAMBI_EXPORT void registerDependency(JNIEnv *env, jobject dependentObject, QtJambiNativeID _this_nativeId);

QTJAMBI_EXPORT uint getJavaObjectIdentity(JNIEnv *env, jobject object);

QTJAMBI_EXPORT uint getJavaObjectHashCode(JNIEnv *env, jobject object);

QTJAMBI_EXPORT jobject newJavaOptional(JNIEnv *env, bool hasValue, jobject object);

QTJAMBI_EXPORT jobject newJavaOptionalInt(JNIEnv *env, bool hasValue, jint value);

QTJAMBI_EXPORT jobject newJavaOptionalLong(JNIEnv *env, bool hasValue, jlong value);

QTJAMBI_EXPORT jobject newJavaOptionalDouble(JNIEnv *env, bool hasValue, jdouble value);

QTJAMBI_EXPORT jobject readJavaOptional(JNIEnv *env, jobject object, bool& isPresent);

QTJAMBI_EXPORT jint readJavaOptionalInt(JNIEnv *env, jobject object, bool& isPresent);

QTJAMBI_EXPORT jlong readJavaOptionalLong(JNIEnv *env, jobject object, bool& isPresent);

QTJAMBI_EXPORT jdouble readJavaOptionalDouble(JNIEnv *env, jobject object, bool& isPresent);

QTJAMBI_EXPORT void checkThread(JNIEnv *env, const QObject* object);

QTJAMBI_EXPORT void checkThread(JNIEnv *env, const std::type_info& argumentType, const void* object);

QTJAMBI_EXPORT void checkMainThread(JNIEnv *env, const std::type_info& typeId);

QTJAMBI_EXPORT void checkThreadOnArgument(JNIEnv *env, const char* argumentName, const QObject* argument);

QTJAMBI_EXPORT void checkThreadOnArgument(JNIEnv *env, const char* argumentName, const std::type_info& argumentType, const QObject* argumentOwner);

QTJAMBI_EXPORT void checkThreadOnArgument(JNIEnv *env, const char* argumentName, const std::type_info& argumentType, const void* argument);

QTJAMBI_EXPORT void checkMainThreadOnArgument(JNIEnv *env, const char* argumentName, const std::type_info& argumentType);

QTJAMBI_EXPORT void checkMainThreadConstructing(JNIEnv *env, const std::type_info& constructedType);

QTJAMBI_EXPORT void checkThreadOnParent(JNIEnv *env, const QObject* parent);

QTJAMBI_EXPORT void checkThreadOnParent(JNIEnv *env, const std::type_info& parentType, const void* parent);

QTJAMBI_EXPORT void checkThreadOnParent(JNIEnv *env, const std::type_info& parentType, const QObject* parentOwner);

QTJAMBI_EXPORT const QObject* mainThreadOwner(const void *);

QTJAMBI_EXPORT const QObject* getPixmapOwner(const void *);

QTJAMBI_EXPORT void checkThreadConstructingQWindow(JNIEnv *env, const std::type_info& constructedType, const QObject* parent);

QTJAMBI_EXPORT void checkThreadConstructingQPixmap(JNIEnv *env, const std::type_info& constructedType);

QTJAMBI_EXPORT void checkThreadConstructingQWidget(JNIEnv *env, const std::type_info& constructedType, const QObject* parent);

QTJAMBI_EXPORT void checkThreadOnArgumentQPixmap(JNIEnv *env, const char* argumentName, const std::type_info& argumentType);

QTJAMBI_EXPORT void checkThreadQPixmap(JNIEnv *env, const std::type_info& typeId);

typedef void(*ResultTranslator)(JNIEnv *, const QSharedPointer<QFutureInterfaceBase>&, const QSharedPointer<QFutureInterfaceBase>&, int, int);

QTJAMBI_EXPORT QFutureInterfaceBase* translateQFutureInterface(QSharedPointer<QFutureInterfaceBase>&& sourceFuture, QSharedPointer<QFutureInterfaceBase>&& targetFuture, const char* translatedType, ResultTranslator resultTranslator, ResultTranslator resultRetranslator);

typedef void(*FutureSetter)(JNIEnv *, QFutureWatcherBase*, jobject);
typedef jobject(*FutureResult)(JNIEnv *, QFutureWatcherBase*, int);
typedef jobject(*FutureGetter)(JNIEnv *, QFutureWatcherBase*);
QTJAMBI_EXPORT jobject convertQFutureWatcherToJavaObject(JNIEnv* env, const QFutureWatcherBase* futureWatcher,
                                                   FutureSetter futureSetter, FutureResult futureResult, FutureGetter futureGetter);

QTJAMBI_EXPORT jobject createQFutureFromQFutureInterface(JNIEnv* env, jobject futureInterface);

QTJAMBI_EXPORT jobject getQFutureInterfaceFromQFuture(JNIEnv* env, jobject future);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QTJAMBI_EXPORT QFutureInterfaceBase& getQFutureInterfaceFromQFuture(const QFuture<void>& future);
#else
QTJAMBI_EXPORT jobject createQPromise(JNIEnv* env, jobject futureInterface, const void* promise, QtJambiScope* scope);

QTJAMBI_EXPORT void* getNativeQPromise(JNIEnv* env, jobject promise);
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
QTJAMBI_EXPORT QMetaObject::Connection connect(const QObject *sender, const char *signal,
                                const QObject *receiver, const char *member, Qt::ConnectionType = Qt::AutoConnection);
QTJAMBI_EXPORT QMetaObject::Connection connect(const QObject *sender, const QMetaMethod &signal,
                        const QObject *receiver, const QMetaMethod &method,
                        Qt::ConnectionType type = Qt::AutoConnection);
#endif

enum class ListType{
    QList, QQueue
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    , QStack
#endif
};

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
enum class VectorType{
    QVector, QStack
};
#endif

QTJAMBI_EXPORT jobject convertQSequentialIteratorToJavaObject(JNIEnv *env,
                           QtJambiNativeID owner,
                           void* iteratorPtr,
                           PtrDeleterFunction destructor_function,
                           AbstractSequentialConstIteratorAccess* access);

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
QTJAMBI_EXPORT jobject convertQSpanToJavaObject(JNIEnv *env,
                                                QtJambiNativeID owner,
                                                AbstractSpanAccess* access,
                                                const void* begin,
                                                jlong size
                                                );

template<typename T, bool t_is_const, std::size_t E>
jobject convertQSpanToJavaObject(JNIEnv *env,
                                 const QSpan<typename std::conditional<t_is_const, typename std::add_const<T>::type, T>::type,E>& span,
                                 QtJambiNativeID owner = InvalidNativeID);

QTJAMBI_EXPORT jobject convertQSpanFromQListToJavaObject(JNIEnv *env,
                                                const void* span,
                                                CopyFunction copyFunction,
                                                PtrDeleterFunction destructor_function,
                                                AbstractListAccess* containerAccess, bool isConst);

template<typename T, std::size_t E>
jobject convertQSpanToDetachedJavaObject(JNIEnv *env, const QSpan<T,E>& span);
#endif

QTJAMBI_EXPORT jobject convertQAssociativeIteratorToJavaObject(JNIEnv *env,
                           QtJambiNativeID owner,
                           void* iteratorPtr,
                           PtrDeleterFunction destructor_function,
                           AbstractAssociativeConstIteratorAccess* access);

QTJAMBI_EXPORT jobject convertQListToJavaObject(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     CopyFunction copyFunction,
                                     PtrDeleterFunction deleter,
                                     ListType listType,
                                     AbstractListAccess* listAccess
                                );

QTJAMBI_EXPORT jobject convertQListToJavaObject(JNIEnv *__jni_env,
                                     const QSharedPointer<char>& listPtr,
                                     ListType listType,
                                     AbstractListAccess* listAccess
                                );
QTJAMBI_EXPORT jobject convertQListToJavaObject(JNIEnv *__jni_env,
                                                const std::shared_ptr<char>& listPtr,
                                                ListType listType,
                                                AbstractListAccess* listAccess
                                                );

QTJAMBI_EXPORT jobject convertQStringListToJavaObject(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     CopyFunction copyFunction,
                                     PtrDeleterFunction deleter
                                );

QTJAMBI_EXPORT jobject convertQStringListToJavaObject(JNIEnv *__jni_env,
                                     const QSharedPointer<char>& listPtr
                                );
QTJAMBI_EXPORT jobject convertQStringListToJavaObject(JNIEnv *__jni_env,
                                                      const std::shared_ptr<char>& listPtr
                                                      );

QTJAMBI_EXPORT jobject convertQSetToJavaObject(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     CopyFunction copyFunction,
                                     PtrDeleterFunction deleter,
                                     AbstractSetAccess* setAccess
                                );

QTJAMBI_EXPORT jobject convertQSetToJavaObject(JNIEnv *__jni_env,
                                     const QSharedPointer<char>& listPtr,
                                     AbstractSetAccess* setAccess
                                );
QTJAMBI_EXPORT jobject convertQSetToJavaObject(JNIEnv *__jni_env,
                                               const std::shared_ptr<char>& listPtr,
                                               AbstractSetAccess* setAccess
                                               );

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QTJAMBI_EXPORT jobject convertQLinkedListToJavaObject(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     CopyFunction copyFunction,
                                     PtrDeleterFunction deleter,
                                     AbstractLinkedListAccess* linkedListAccess
                                );

QTJAMBI_EXPORT jobject convertQLinkedListToJavaObject(JNIEnv *__jni_env,
                                     const QSharedPointer<char>& listPtr,
                                     AbstractLinkedListAccess* linkedListAccess
                                );
QTJAMBI_EXPORT jobject convertQLinkedListToJavaObject(JNIEnv *__jni_env,
                                                      const std::shared_ptr<char>& listPtr,
                                                      AbstractLinkedListAccess* linkedListAccess
                                                      );

QTJAMBI_EXPORT jobject convertQVectorToJavaObject(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     CopyFunction copyFunction,
                                     PtrDeleterFunction deleter,
                                     VectorType vectorType,
                                     AbstractVectorAccess* vectorAccess
                                );

QTJAMBI_EXPORT jobject convertQVectorToJavaObject(JNIEnv *__jni_env,
                                     const QSharedPointer<char>& listPtr,
                                     VectorType vectorType,
                                     AbstractVectorAccess* vectorAccess
                                );
QTJAMBI_EXPORT jobject convertQVectorToJavaObject(JNIEnv *__jni_env,
                                                  const std::shared_ptr<char>& listPtr,
                                                  VectorType vectorType,
                                                  AbstractVectorAccess* vectorAccess
                                                  );
#endif


QTJAMBI_EXPORT jobject convertQHashToJavaObject(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     CopyFunction copyFunction,
                                     PtrDeleterFunction deleter,
                                     AbstractHashAccess* hashAccess
                                );

QTJAMBI_EXPORT jobject convertQHashToJavaObject(JNIEnv *__jni_env,
                                     const QSharedPointer<char>& listPtr,
                                     AbstractHashAccess* hashAccess
                                );
QTJAMBI_EXPORT jobject convertQHashToJavaObject(JNIEnv *__jni_env,
                                                const std::shared_ptr<char>& listPtr,
                                                AbstractHashAccess* hashAccess
                                                );

QTJAMBI_EXPORT jobject convertQMultiHashToJavaObject(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     CopyFunction copyFunction,
                                     PtrDeleterFunction deleter,
                                     AbstractMultiHashAccess* multiHashAccess
                                );

QTJAMBI_EXPORT jobject convertQMultiHashToJavaObject(JNIEnv *__jni_env,
                                     const QSharedPointer<char>& listPtr,
                                     AbstractMultiHashAccess* multiHashAccess
                                );
QTJAMBI_EXPORT jobject convertQMultiHashToJavaObject(JNIEnv *__jni_env,
                                                     const std::shared_ptr<char>& listPtr,
                                                     AbstractMultiHashAccess* multiHashAccess
                                                     );

QTJAMBI_EXPORT jobject convertQMapToJavaObject(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     CopyFunction copyFunction,
                                     PtrDeleterFunction deleter,
                                     AbstractMapAccess* mapAccess
                                );

QTJAMBI_EXPORT jobject convertQMapToJavaObject(JNIEnv *__jni_env,
                                     const QSharedPointer<char>& listPtr,
                                     AbstractMapAccess* mapAccess
                                );

QTJAMBI_EXPORT jobject convertQMapToJavaObject(JNIEnv *__jni_env,
                                               const std::shared_ptr<char>& listPtr,
                                               AbstractMapAccess* mapAccess
                                               );

QTJAMBI_EXPORT jobject convertQMultiMapToJavaObject(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     CopyFunction copyFunction,
                                     PtrDeleterFunction deleter,
                                     AbstractMultiMapAccess* mapAccess
                                );


QTJAMBI_EXPORT jobject convertQMultiMapToJavaObject(JNIEnv *__jni_env,
                                     const QSharedPointer<char>& listPtr,
                                     AbstractMultiMapAccess* mapAccess
                                );
QTJAMBI_EXPORT jobject convertQMultiMapToJavaObject(JNIEnv *__jni_env,
                                                    const std::shared_ptr<char>& listPtr,
                                                    AbstractMultiMapAccess* mapAccess
                                                    );

template<class Container>
Container createIterable(typename Container::const_iterator begin, typename Container::size_type size){
#ifdef Q_CC_MSVC
    return Container(begin, begin+size);
#else
    struct ContainerAccess{
        typename Container::iterator begin;
        typename Container::size_type size;
    } access;
    access.begin = begin;
    access.size = size;
    union{
        Container* container;
        ContainerAccess* access;
    } u;
    u.access = &access;
    return Container(*u.container);
#endif
}

template<class Container>
Container createIterable(typename Container::const_iterator begin, typename Container::const_iterator end){
#ifdef Q_CC_MSVC
    return Container(begin, end);
#else
    return createIterable<Container>(begin, size_t(end)-size_t(begin));
#endif
}

} // namespace QtJambiAPI

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
inline bool operator < (const QVariant& v1, const QVariant& v2){
    if(v1.userType()==v2.userType()){
        QPartialOrdering result = QMetaType(v1.userType()).compare(v1.data(), v2.data());
        return result==QPartialOrdering::Less;
    }
    return false;
}
#endif

template<class T>
const T& reinterpret_value_cast(const void * ptr)
{
    const T* _ptr = reinterpret_cast<const T*>(ptr);
    if(_ptr)
    return *_ptr;
    return QtJambiPrivate::getDefaultValue<T>();
}

template<class T>
const T& reinterpret_deref_cast(JNIEnv *env, const void * ptr)
{
    return QtJambiAPI::checkedAddressOf<const T>(env, reinterpret_cast<const T*>(ptr));
}

template<class T>
T& reinterpret_deref_cast(JNIEnv *env, void * ptr)
{
    return QtJambiAPI::checkedAddressOf<T>(env, reinterpret_cast<T*>(ptr));
}

#endif // QTJAMBIAPI_H
