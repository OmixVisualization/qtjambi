/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBI_CONVERT_H
#define QTJAMBI_CONVERT_H
#define QTJAMBI_CONTAINERACCESS_H

#include "qtjambi_cast_impl.h"
#include "qtjambi_cast_array.h"

class QtJambiScope;

template<class O, class T>
constexpr O qtjambi_cast(JNIEnv *env, QtJambiScope& scope, const T& in, const char* nativeTypeName){
    typedef typename std::remove_reference<O>::type O_noref;
    typedef typename std::remove_reference<const T>::type T_noref;
    typedef typename std::remove_cv<O_noref>::type O_noconst;
    typedef typename std::remove_cv<T_noref>::type T_noconst;

    return QtJambiPrivate::qtjambi_cast_decider<true, O,
                          std::is_arithmetic<O_noref>::value,
                          std::is_enum<O_noref>::value,
                          QtJambiPrivate::is_jni_type<O_noconst>::value,
                          const T&,
                          std::is_arithmetic<T_noref>::value,
                          std::is_enum<T_noref>::value,
                          QtJambiPrivate::is_jni_type<T_noconst>::value,
                          std::is_same<O_noref, T_noref>::value
    >::cast(env, in, nativeTypeName, &scope);
}

template<class O, class T>
constexpr O qtjambi_cast(JNIEnv *env, QtJambiScope& scope, T& in, const char* nativeTypeName){
    typedef typename std::remove_reference<O>::type O_noref;
    typedef typename std::remove_reference<T>::type T_noref;
    typedef typename std::remove_cv<O_noref>::type O_noconst;
    typedef typename std::remove_cv<T_noref>::type T_noconst;

    return QtJambiPrivate::qtjambi_cast_decider<true, O,
                          std::is_arithmetic<O_noref>::value,
                          std::is_enum<O_noref>::value,
                          QtJambiPrivate::is_jni_type<O_noconst>::value,
                          T&,
                          std::is_arithmetic<T_noref>::value,
                          std::is_enum<T_noref>::value,
                          QtJambiPrivate::is_jni_type<T_noconst>::value,
                          std::is_same<O_noref, T_noref>::value
    >::cast(env, in, nativeTypeName, &scope);
}

template<class O, class T>
constexpr O qtjambi_cast(JNIEnv *env, QtJambiScope& scope, T* in, const char* nativeTypeName){
    typedef typename std::remove_reference<O>::type O_noref;
    typedef typename std::remove_reference<T*>::type T_noref;
    typedef typename std::remove_cv<O_noref>::type O_noconst;
    typedef typename std::remove_cv<T_noref>::type T_noconst;
    return QtJambiPrivate::qtjambi_cast_decider<true, O,
                          std::is_arithmetic<O_noref>::value,
                          std::is_enum<O_noref>::value,
                          QtJambiPrivate::is_jni_type<O_noconst>::value,
                          T*,
                          std::is_arithmetic<T_noref>::value,
                          std::is_enum<T_noref>::value,
                          QtJambiPrivate::is_jni_type<T_noconst>::value,
                          std::is_same<O_noref, T_noref>::value
    >::cast(env, in, nativeTypeName, &scope);
}

template<class O, class T>
constexpr O qtjambi_cast(JNIEnv *env, const T& in, const char* nativeTypeName){
    typedef typename std::remove_reference<O>::type O_noref;
    typedef typename std::remove_reference<const T>::type T_noref;
    typedef typename std::remove_cv<O_noref>::type O_noconst;
    typedef typename std::remove_cv<T_noref>::type T_noconst;

    return QtJambiPrivate::qtjambi_cast_decider<false, O,
                          std::is_arithmetic<O_noref>::value,
                          std::is_enum<O_noref>::value,
                          QtJambiPrivate::is_jni_type<O_noconst>::value,
                          const T&,
                          std::is_arithmetic<T_noref>::value,
                          std::is_enum<T_noref>::value,
                          QtJambiPrivate::is_jni_type<T_noconst>::value,
                          std::is_same<O_noref, T_noref>::value
     >::cast(env, in, nativeTypeName, nullptr);
}

template<class O, class T>
constexpr O qtjambi_cast(JNIEnv *env, T& in, const char* nativeTypeName){
    typedef typename std::remove_reference<O>::type O_noref;
    typedef typename std::remove_reference<T>::type T_noref;
    typedef typename std::remove_cv<O_noref>::type O_noconst;
    typedef typename std::remove_cv<T_noref>::type T_noconst;

    return QtJambiPrivate::qtjambi_cast_decider<false, O,
                          std::is_arithmetic<O_noref>::value,
                          std::is_enum<O_noref>::value,
                          QtJambiPrivate::is_jni_type<O_noconst>::value,
                          T&,
                          std::is_arithmetic<T_noref>::value,
                          std::is_enum<T_noref>::value,
                          QtJambiPrivate::is_jni_type<T_noconst>::value,
                          std::is_same<O_noref, T_noref>::value
     >::cast(env, in, nativeTypeName, nullptr);
}

template<class O, class T>
constexpr O qtjambi_cast(JNIEnv *env, T* in, const char* nativeTypeName){
     typedef typename std::remove_reference<O>::type O_noref;
     typedef typename std::remove_reference<T*>::type T_noref;
     typedef typename std::remove_cv<O_noref>::type O_noconst;
     typedef typename std::remove_cv<T_noref>::type T_noconst;
    return QtJambiPrivate::qtjambi_cast_decider<false, O,
                          std::is_arithmetic<O_noref>::value,
                          std::is_enum<O_noref>::value,
                          QtJambiPrivate::is_jni_type<O_noconst>::value,
                          T*,
                          std::is_arithmetic<T_noref>::value,
                          std::is_enum<T_noref>::value,
                          QtJambiPrivate::is_jni_type<T_noconst>::value,
                          std::is_same<O_noref, T_noref>::value
     >::cast(env, in, nativeTypeName, nullptr);
}

template<class O, class T>
constexpr O qtjambi_cast(JNIEnv *env, const T& in){
    typedef typename std::remove_reference<O>::type O_noref;
    typedef typename std::remove_reference<const T>::type T_noref;
    typedef typename std::remove_cv<O_noref>::type O_noconst;
    typedef typename std::remove_cv<T_noref>::type T_noconst;

    return QtJambiPrivate::qtjambi_cast_decider<false, O,
                          std::is_arithmetic<O_noref>::value,
                          std::is_enum<O_noref>::value,
                          QtJambiPrivate::is_jni_type<O_noconst>::value,
                          const T&,
                          std::is_arithmetic<T_noref>::value,
                          std::is_enum<T_noref>::value,
                          QtJambiPrivate::is_jni_type<T_noconst>::value,
                          std::is_same<O_noref, T_noref>::value
    >::cast(env, in, nullptr, nullptr);
}

template<class O, class T>
constexpr O qtjambi_cast(JNIEnv *env, T& in){
    typedef typename std::remove_reference<O>::type O_noref;
    typedef typename std::remove_reference<T>::type T_noref;
    typedef typename std::remove_cv<O_noref>::type O_noconst;
    typedef typename std::remove_cv<T_noref>::type T_noconst;

    return QtJambiPrivate::qtjambi_cast_decider<false, O,
                          std::is_arithmetic<O_noref>::value,
                          std::is_enum<O_noref>::value,
                          QtJambiPrivate::is_jni_type<O_noconst>::value,
                          T&,
                          std::is_arithmetic<T_noref>::value,
                          std::is_enum<T_noref>::value,
                          QtJambiPrivate::is_jni_type<T_noconst>::value,
                          std::is_same<O_noref, T_noref>::value
    >::cast(env, in, nullptr, nullptr);
}

template<class O, class T>
constexpr O qtjambi_cast(JNIEnv *env, T* in){
    typedef typename std::remove_reference<O>::type O_noref;
    typedef typename std::remove_reference<T*>::type T_noref;
    typedef typename std::remove_cv<O_noref>::type O_noconst;
    typedef typename std::remove_cv<T_noref>::type T_noconst;
    return QtJambiPrivate::qtjambi_cast_decider<false, O,
                          std::is_arithmetic<O_noref>::value,
                          std::is_enum<O_noref>::value,
                          QtJambiPrivate::is_jni_type<O_noconst>::value,
                          T*,
                          std::is_arithmetic<T_noref>::value,
                          std::is_enum<T_noref>::value,
                          QtJambiPrivate::is_jni_type<T_noconst>::value,
                          std::is_same<O_noref, T_noref>::value
    >::cast(env, in, nullptr, nullptr);
}

template<class O, class T>
constexpr O qtjambi_cast(JNIEnv *env, QtJambiScope& scope, const T& in){
    typedef typename std::remove_reference<O>::type O_noref;
    typedef typename std::remove_reference<const T>::type T_noref;
    typedef typename std::remove_cv<O_noref>::type O_noconst;
    typedef typename std::remove_cv<T_noref>::type T_noconst;

    return QtJambiPrivate::qtjambi_cast_decider<true, O,
                          std::is_arithmetic<O_noref>::value,
                          std::is_enum<O_noref>::value,
                          QtJambiPrivate::is_jni_type<O_noconst>::value,
                          const T&,
                          std::is_arithmetic<T_noref>::value,
                          std::is_enum<T_noref>::value,
                          QtJambiPrivate::is_jni_type<T_noconst>::value,
                          std::is_same<O_noref, T_noref>::value
    >::cast(env, in, nullptr, &scope);
}

template<class O, class T>
constexpr O qtjambi_cast(JNIEnv *env, QtJambiScope& scope, T& in){
    typedef typename std::remove_reference<O>::type O_noref;
    typedef typename std::remove_reference<T>::type T_noref;
    typedef typename std::remove_cv<O_noref>::type O_noconst;
    typedef typename std::remove_cv<T_noref>::type T_noconst;

    return QtJambiPrivate::qtjambi_cast_decider<true, O,
                          std::is_arithmetic<O_noref>::value,
                          std::is_enum<O_noref>::value,
                          QtJambiPrivate::is_jni_type<O_noconst>::value,
                          T&,
                          std::is_arithmetic<T_noref>::value,
                          std::is_enum<T_noref>::value,
                          QtJambiPrivate::is_jni_type<T_noconst>::value,
                          std::is_same<O_noref, T_noref>::value
    >::cast(env, in, nullptr, &scope);
}

template<class O, class T>
constexpr O qtjambi_cast(JNIEnv *env, QtJambiScope& scope, T* in){
    typedef typename std::remove_reference<O>::type O_noref;
    typedef typename std::remove_reference<T*>::type T_noref;
    typedef typename std::remove_cv<O_noref>::type O_noconst;
    typedef typename std::remove_cv<T_noref>::type T_noconst;
    return QtJambiPrivate::qtjambi_cast_decider<true, O,
                          std::is_arithmetic<O_noref>::value,
                          std::is_enum<O_noref>::value,
                          QtJambiPrivate::is_jni_type<O_noconst>::value,
                          T*,
                          std::is_arithmetic<T_noref>::value,
                          std::is_enum<T_noref>::value,
                          QtJambiPrivate::is_jni_type<T_noconst>::value,
                          std::is_same<O_noref, T_noref>::value
    >::cast(env, in, nullptr, &scope);
}

template<class T>
constexpr jobject qtjambi_cast_sequential_iterator(JNIEnv *env, QtJambiScope& scope, const T& in){
    typedef typename std::remove_reference<const T>::type T_noref;
    typedef typename std::remove_cv<T_noref>::type T_noconst;
    return QtJambiPrivate::qtjambi_sequential_iterator_caster<T_noconst>::cast(env, scope.relatedNativeID(), in);
}

template<class T>
constexpr jobject qtjambi_cast_sequential_iterator(JNIEnv *env, QtJambiScope& scope, T& in){
    typedef typename std::remove_reference<T>::type T_noref;
    typedef typename std::remove_cv<T_noref>::type T_noconst;
    return QtJambiPrivate::qtjambi_sequential_iterator_caster<T_noconst>::cast(env, scope.relatedNativeID(), in);
}

template<class T>
constexpr jobject qtjambi_cast_sequential_iterator(JNIEnv *env, QtJambiScope& scope, T* in){
    typedef typename std::remove_reference<T*>::type T_noref;
    typedef typename std::remove_cv<T_noref>::type T_noconst;
    return QtJambiPrivate::qtjambi_sequential_iterator_caster<T_noconst>::cast(env, scope.relatedNativeID(), in);
}

template<class T>
constexpr jobject qtjambi_cast_associative_iterator(JNIEnv *env, QtJambiScope& scope, const T& in){
    typedef typename std::remove_reference<const T>::type T_noref;
    typedef typename std::remove_cv<T_noref>::type T_noconst;
    return QtJambiPrivate::qtjambi_associative_iterator_caster<T_noconst>::cast(env, scope.relatedNativeID(), in);
}

template<class T>
constexpr jobject qtjambi_cast_associative_iterator(JNIEnv *env, QtJambiScope& scope, T& in){
    typedef typename std::remove_reference<T>::type T_noref;
    typedef typename std::remove_cv<T_noref>::type T_noconst;
    return QtJambiPrivate::qtjambi_associative_iterator_caster<T_noconst>::cast(env, scope.relatedNativeID(), in);
}

template<class T>
constexpr jobject qtjambi_cast_associative_iterator(JNIEnv *env, QtJambiScope& scope, T* in){
    typedef typename std::remove_reference<T*>::type T_noref;
    typedef typename std::remove_cv<T_noref>::type T_noconst;
    return QtJambiPrivate::qtjambi_associative_iterator_caster<T_noconst>::cast(env, scope.relatedNativeID(), in);
}

template<class O, class T>
O qtjambi_cast(QtJambiScope& scope, const T& in){
    JniEnvironment env{500};
    Q_ASSERT(env);
    return QtJambiPrivate::qtjambi_scoped_cast<true,O,const T&>::cast(env, in, nullptr, &scope);
}

template<class O, class T>
O qtjambi_cast(QtJambiScope& scope, T& in){
    JniEnvironment env{500};
    Q_ASSERT(env);
    return QtJambiPrivate::qtjambi_scoped_cast<true,O,T&>::cast(env, in, nullptr, &scope);
}

template<class O, class T>
O qtjambi_cast(QtJambiScope& scope, T* in){
    JniEnvironment env{500};
    Q_ASSERT(env);
    return QtJambiPrivate::qtjambi_scoped_cast<true,O,T*>::cast(env, in, nullptr, &scope);
}

template<class O, class T>
O qtjambi_cast(QtJambiScope& scope, const T& in, const char* nativeTypeName){
    Q_ASSERT(nativeTypeName);
    JniEnvironment env{500};
    Q_ASSERT(env);
    return QtJambiPrivate::qtjambi_scoped_cast<true,O,const T&>::cast(env, in, nativeTypeName, &scope);
}

template<class O, class T>
O qtjambi_cast(QtJambiScope& scope, T& in, const char* nativeTypeName){
    Q_ASSERT(nativeTypeName);
    JniEnvironment env{500};
    Q_ASSERT(env);
    return QtJambiPrivate::qtjambi_scoped_cast<true,O,T&>::cast(env, in, nativeTypeName, &scope);
}

template<class O, class T>
O qtjambi_cast(QtJambiScope& scope, T* in, const char* nativeTypeName){
    Q_ASSERT(nativeTypeName);
    JniEnvironment env{500};
    Q_ASSERT(env);
    return QtJambiPrivate::qtjambi_scoped_cast<true,O,T*>::cast(env, in, nativeTypeName, &scope);
}

template<class O, class T>
O qtjambi_cast(const T& in){
    JniEnvironment env{500};
    Q_ASSERT(env);
    return QtJambiPrivate::qtjambi_scoped_cast<false,O,const T&>::cast(env, in, nullptr, nullptr);
}

template<class O, class T>
O qtjambi_cast(T& in){
    JniEnvironment env{500};
    Q_ASSERT(env);
    return QtJambiPrivate::qtjambi_scoped_cast<false,O,T&>::cast(env, in, nullptr, nullptr);
}

template<class O, class T>
O qtjambi_cast(T* in){
    JniEnvironment env{500};
    Q_ASSERT(env);
    return QtJambiPrivate::qtjambi_scoped_cast<false,O,T*>::cast(env, in, nullptr, nullptr);
}

template<class O, class T>
O qtjambi_cast_by_name(const T& in, const char* nativeTypeName){
    Q_ASSERT(nativeTypeName);
    JniEnvironment env{500};
    Q_ASSERT(env);
    return QtJambiPrivate::qtjambi_scoped_cast<false,O,const T&>::cast(env, in, nativeTypeName, nullptr);
}

template<class O, class T>
O qtjambi_cast_by_name(T& in, const char* nativeTypeName){
    Q_ASSERT(nativeTypeName);
    JniEnvironment env{500};
    Q_ASSERT(env);
    return QtJambiPrivate::qtjambi_scoped_cast<false,O,T&>::cast(env, in, nativeTypeName, nullptr);
}

template<class O, class T>
O qtjambi_cast_by_name(T* in, const char* nativeTypeName){
    Q_ASSERT(nativeTypeName);
    JniEnvironment env{500};
    Q_ASSERT(env);
    return QtJambiPrivate::qtjambi_scoped_cast<false,O,T*>::cast(env, in, nativeTypeName, nullptr);
}

#endif // QTJAMBI_CONVERT_H
