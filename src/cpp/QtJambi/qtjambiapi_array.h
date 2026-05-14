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

#if !defined(QTJAMBIAPI_ARRAY_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBIAPI_ARRAY_H

#include "global.h"

class AbstractListAccess;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
class AbstractSpanAccess;
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)

namespace QtJambiAPI {
QTJAMBI_EXPORT jintArray toJIntArray(JNIEnv *__jni_env, const jint* in, jsize length);
QTJAMBI_EXPORT jshortArray toJShortArray(JNIEnv *__jni_env, const jshort* in, jsize length);
QTJAMBI_EXPORT jlongArray toJLongArray(JNIEnv *__jni_env, const jlong* in, jsize length);
QTJAMBI_EXPORT jfloatArray toJFloatArray(JNIEnv *__jni_env, const jfloat* in, jsize length);
QTJAMBI_EXPORT jdoubleArray toJDoubleArray(JNIEnv *__jni_env, const jdouble* in, jsize length);
QTJAMBI_EXPORT jbooleanArray toJBooleanArray(JNIEnv *__jni_env, const jboolean* in, jsize length);
QTJAMBI_EXPORT bool isValidArray(JNIEnv *env, jobject object, const std::type_info& typeId);

QTJAMBI_EXPORT jobjectArray createObjectArray(JNIEnv *env, const char* componentClass, jsize size);
QTJAMBI_EXPORT jobjectArray createObjectArray(JNIEnv *env, const std::type_info& componentType, jsize size);

template<typename T, typename E>
inline jobjectArray toJObjectArray(JNIEnv *__jni_env, const char *className, const T& iterable, std::function<jobject(JNIEnv *,const E&)> convertFunction) {
    jsize length = jsize(iterable.size());
    jobjectArray out = createObjectArray(__jni_env, className, length);
    for (jsize i = 0; i < length; ++i) {
        __jni_env->SetObjectArrayElement(out, i, convertFunction(__jni_env, iterable.begin()[i]));
        JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    }
    return out;
}

QTJAMBI_EXPORT jobjectArray toJObjectArray(JNIEnv *__jni_env, const std::type_info& typeInfo, const void* iterable, jsize length, std::function<jobject(JNIEnv *,const void*,jsize)> convertFunction);

template<template<typename E> class T, typename E>
inline jobjectArray toJObjectArray(JNIEnv *__jni_env, const T<E>& iterable, jsize length, std::function<jobject(JNIEnv *,const T<E>&,jsize)> getFunction) {
    jobjectArray out = createObjectArray(__jni_env, typeid(E), length);
    for (jsize i = 0; i < length; ++i) {
        __jni_env->SetObjectArrayElement(out, i, getFunction(__jni_env, iterable, i));
        JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    }
    return out;
}

template<template<typename E> class T, typename E>
inline jobjectArray toJObjectArray(JNIEnv *__jni_env, const T<E>& iterable, jsize (*lengthFunction)(const T<E>&), std::function<jobject(JNIEnv *,const T<E>&,jsize)> getFunction) {
    jsize length = lengthFunction(iterable);
    jobjectArray out = createObjectArray(__jni_env, typeid(E), length);
    for (jsize i = 0; i < length; ++i) {
        __jni_env->SetObjectArrayElement(out, i, getFunction(__jni_env, iterable, i));
        JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    }
    return out;
}

template<typename T, typename E>
inline jobjectArray toJObjectArray(JNIEnv *__jni_env, const char *className, const T* iterable, std::function<jobject(JNIEnv *,const E&)> convertFunction) {
    jsize length = jsize(iterable->size());
    jobjectArray out = createObjectArray(__jni_env, className, length);
    for (jsize i = 0; i < length; ++i) {
        __jni_env->SetObjectArrayElement(out, i, convertFunction(__jni_env, iterable->begin()[i]));
        JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    }
    return out;
}

template<typename T>
inline jobjectArray toJObjectArray(JNIEnv *__jni_env, const char *className, const T* array, jsize length, std::function<jobject(JNIEnv *,const T&)> convertFunction) {
    jobjectArray out = createObjectArray(__jni_env, className, length);
    for (jsize i = 0; i < length; ++i) {
        __jni_env->SetObjectArrayElement(out, i, convertFunction(__jni_env, array[i]));
        JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    }
    return out;
}

template<typename T>
inline jobjectArray toJObjectArray(JNIEnv *__jni_env, const T* array, jsize length, std::function<jobject(JNIEnv *,const T&)> convertFunction) {
    jobjectArray out = createObjectArray(__jni_env, typeid(T), length);
    for (jsize i = 0; i < length; ++i) {
        __jni_env->SetObjectArrayElement(out, i, convertFunction(__jni_env, array[i]));
        JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    }
    return out;
}

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
QTJAMBI_EXPORT bool isQSpanObject(JNIEnv *env, jobject obj);
QTJAMBI_EXPORT void commitQSpanObject(JNIEnv *env, jobject obj);
QTJAMBI_EXPORT QPair<void*,jlong> fromQSpanObject(JNIEnv *env, jobject obj, bool isConst, const QMetaType& metaType);

QTJAMBI_EXPORT jobject convertQSpanToJavaObject(JNIEnv *env,
                                                QtJambiNativeID owner,
                                                AbstractSpanAccess* access,
                                                const void* begin,
                                                jlong size
                                                );

QTJAMBI_EXPORT jobject convertQSpanFromQListToJavaObject(JNIEnv *env,
                                                         const void* span,
                                                         CopyFunction copyFunction,
                                                         PtrDeleterFunction destructor_function,
                                                         AbstractListAccess* containerAccess, bool isConst);
#endif

template<class T>
std::initializer_list<T> initializer_list(typename std::initializer_list<T>::const_iterator begin, typename std::initializer_list<T>::size_type size){
#ifdef Q_CC_MSVC
    return std::initializer_list<T>(begin, begin+size);
#else
    using iterator = typename std::initializer_list<T>::iterator;
    using size_type = typename std::initializer_list<T>::size_type;
    union InitializerList{
        std::initializer_list<T> initializerList;
        struct{
            iterator begin;
            size_type size;
        } creator;
        InitializerList(iterator begin, size_type size) : creator{begin, size} {}
        operator std::initializer_list<T>(){return initializerList;}
    };
    return InitializerList(begin, size);
#endif
}

template<class T>
std::initializer_list<T> initializer_list(typename std::initializer_list<T>::const_iterator begin, typename std::initializer_list<T>::const_iterator end){
#ifdef Q_CC_MSVC
    return std::initializer_list<T>(begin, end);
#else
    return initializer_list<T>(begin, size_t(end)-size_t(begin));
#endif
}
}

#endif // QTJAMBIAPI_ARRAY_H
