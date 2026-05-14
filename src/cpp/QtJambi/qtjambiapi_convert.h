/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#if !defined(QTJAMBIAPI_CONVERT_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBIAPI_CONVERT_H

#include "global.h"

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

namespace QtJambiAPI {

QTJAMBI_EXPORT bool convertJavaToNative(JNIEnv *env, jobject java_object, void * output, const std::type_info& typeId);
QTJAMBI_EXPORT bool convertJavaToNative(JNIEnv *env, jobject java_object, void * output, QtJambiScope& scope, const std::type_info& typeId);

QTJAMBI_EXPORT bool convertJavaToNative(JNIEnv *env, jobject java_object, void * output, const std::type_info& typeId, const char* typeName);
QTJAMBI_EXPORT bool convertJavaToNative(JNIEnv *env, jobject java_object, void * output, QtJambiScope& scope, const std::type_info& typeId, const char* typeName);

QTJAMBI_EXPORT jobject convertNativeToJavaOwnedObjectAsWrapper(JNIEnv *env, const void *qt_object, const std::type_info& typeId, const char *nativeTypeName = nullptr);
QTJAMBI_EXPORT jobject convertNativeToJavaOwnedObjectAsWrapper(JNIEnv *env, const void *qt_object, jclass clazz);

template<typename T, size_t N>
jobject convertNativeToJavaOwnedObjectAsWrapper(JNIEnv *env, const T *qt_object, const char (&nativeTypeName)[N])
{
    return convertNativeToJavaOwnedObjectAsWrapper(env, qt_object, typeid(T), nativeTypeName);
}

template<typename T>
jobject convertNativeToJavaOwnedObjectAsWrapper(JNIEnv *env, const T *qt_object)
{
    return convertNativeToJavaOwnedObjectAsWrapper(env, qt_object, typeid(T));
}

QTJAMBI_EXPORT jobject convertNativeToJavaObjectAsWrapper(JNIEnv *env, const void *qt_object, const std::type_info& typeId, const char *nativeTypeName = nullptr);

QTJAMBI_EXPORT jobject convertNativeToJavaObjectAsWrapperAndInvalidateAfterUse(JNIEnv *env, QtJambiScope& scope, const void *qt_object, const std::type_info& typeId, const char *nativeTypeName = nullptr);

QTJAMBI_EXPORT jobject convertNativeToJavaObjectAsWrapper(JNIEnv *env, const void *qt_object, jclass clazz);

QTJAMBI_EXPORT jobject convertNativeToJavaObjectAsWrapperAndInvalidateAfterUse(JNIEnv *env, QtJambiScope& scope, const void *qt_object, jclass clazz);

QTJAMBI_EXPORT jobject convertNativeToJavaObjectAsCopy(JNIEnv *env, const void *qt_object, const std::type_info& typeId, const char *nativeTypeName = nullptr);

QTJAMBI_EXPORT jobject convertNativeToJavaObjectAsCopy(JNIEnv *env, void *qt_object, const std::type_info& typeId, const char *nativeTypeName = nullptr);

QTJAMBI_EXPORT jobject convertNativeToJavaObjectAsCopy(JNIEnv *env, const void *qt_object, jclass clazz);

template<typename T, size_t N>
jobject convertNativeToJavaObjectAsWrapperAndInvalidateAfterUse(JNIEnv *env, QtJambiScope& scope, const T *qt_object, const char (&nativeTypeName)[N])
{
    return convertNativeToJavaObjectAsWrapperAndInvalidateAfterUse(env, scope, qt_object, typeid(T), nativeTypeName);
}

template<typename T, size_t N>
jobject convertNativeToJavaObjectAsWrapper(JNIEnv *env, const T *qt_object, const char (&nativeTypeName)[N])
{
    return convertNativeToJavaObjectAsWrapper(env, qt_object, typeid(T), nativeTypeName);
}

template<typename T, size_t N>
jobject convertNativeToJavaObjectAsCopy(JNIEnv *env, const T *qt_object, const char (&nativeTypeName)[N])
{
    return convertNativeToJavaObjectAsCopy(env, qt_object, typeid(T), nativeTypeName);
}

template<typename T>
jobject convertNativeToJavaObjectAsWrapperAndInvalidateAfterUse(JNIEnv *env, QtJambiScope& scope, const T *qt_object)
{
    return convertNativeToJavaObjectAsWrapperAndInvalidateAfterUse(env, scope, qt_object, typeid(T));
}

template<typename T>
jobject convertNativeToJavaObjectAsWrapper(JNIEnv *env, const T *qt_object)
{
    return convertNativeToJavaObjectAsWrapper(env, qt_object, typeid(T));
}

template<typename T>
jobject convertNativeToJavaObjectAsCopy(JNIEnv *env, const T *qt_object)
{
    return convertNativeToJavaObjectAsCopy(env, qt_object, typeid(T));
}

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

QTJAMBI_EXPORT jstring convertNativeToJavaObject(JNIEnv *env, QAnyStringView s);

QTJAMBI_EXPORT void *convertQNativePointerToNative(JNIEnv *env, jobject java_object, int* size = nullptr, int* indirections = nullptr);

QTJAMBI_EXPORT jobject convertNativeToQNativePointer(JNIEnv *env, const void *qt_pointer, QNativePointer::Type type_id, quint64 size, uint indirections);

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
}

#endif // QTJAMBIAPI_CONVERT_H
