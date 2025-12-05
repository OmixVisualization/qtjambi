/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
#include "qtjambi_cast_impl_array.h"

class QtJambiScope;

template<class O, class T, class = void, size_t N>
constexpr O qtjambi_cast(JNIEnv *env, QtJambiScope& scope, T&& in, const char (&nativeTypeName)[N]){
    if constexpr(std::is_array_v<std::remove_reference_t<T>> && (QtJambiPrivate::jni_type<O>::isArray || std::is_same_v<O, jobject> || std::is_same_v<O, jcoreobject>)){
        return QtJambiPrivate::qtjambi_array_cast<O, std::decay_t<T>, size_t, true, JNIEnv *,QtJambiScope&,const char(&)[N]>::cast(in, std::extent_v<std::remove_reference_t<T>>, env, scope, nativeTypeName);
    }else if constexpr((std::is_reference_v<T> || std::is_pointer_v<std::decay_t<T>>) && !std::is_rvalue_reference_v<T>){
        return QtJambiPrivate::qtjambi_cast<O, T, JNIEnv *,QtJambiScope&,const char(&)[N]>::cast(in, env, scope, nativeTypeName);
    }else{
        return QtJambiPrivate::qtjambi_cast<O, T&&, JNIEnv *,QtJambiScope&,const char(&)[N]>::cast(std::move(in), env, scope, nativeTypeName);
    }
}

template<class O, class T, class = void, size_t N>
constexpr O qtjambi_cast(JNIEnv *env, T&& in, const char (&nativeTypeName)[N]){
    if constexpr(std::is_array_v<std::remove_reference_t<T>> && (QtJambiPrivate::jni_type<O>::isArray || std::is_same_v<O, jobject> || std::is_same_v<O, jcoreobject>)){
        return QtJambiPrivate::qtjambi_array_cast<O, std::decay_t<T>, size_t, true,JNIEnv *,const char(&)[N]>::cast(in, std::extent_v<std::remove_reference_t<T>>, env, nativeTypeName);
    }else if constexpr((std::is_reference_v<T> || std::is_pointer_v<std::decay_t<T>>) && !std::is_rvalue_reference_v<T>){
        return QtJambiPrivate::qtjambi_cast<O, T, JNIEnv *,const char(&)[N]>::cast(in, env, nativeTypeName);
    }else{
        return QtJambiPrivate::qtjambi_cast<O, T&&, JNIEnv *,const char(&)[N]>::cast(std::move(in), env, nativeTypeName);
    }
}

template<class O, class T, class = void, size_t = 0>
constexpr O qtjambi_cast(JNIEnv *env, QtJambiScope& scope, T&& in){
    if constexpr(std::is_array_v<std::remove_reference_t<T>> && (QtJambiPrivate::jni_type<O>::isArray || std::is_same_v<O, jobject> || std::is_same_v<O, jcoreobject>)){
        return QtJambiPrivate::qtjambi_array_cast<O, std::decay_t<T>, size_t, true, JNIEnv *,QtJambiScope&>::cast(in, std::extent_v<std::remove_reference_t<T>>, env, scope);
    }else if constexpr((std::is_reference_v<T> || std::is_pointer_v<std::decay_t<T>>) && !std::is_rvalue_reference_v<T>){
        return QtJambiPrivate::qtjambi_cast<O, T, JNIEnv *,QtJambiScope&>::cast(in, env, scope);
    }else{
        return QtJambiPrivate::qtjambi_cast<O, T&&, JNIEnv *,QtJambiScope&>::cast(std::move(in), env, scope);
    }
}

template<class O, class T, class, size_t>
constexpr O qtjambi_cast(JNIEnv *env, T&& in){
    if constexpr(std::is_array_v<std::remove_reference_t<T>> && (QtJambiPrivate::jni_type<O>::isArray || std::is_same_v<O, jobject> || std::is_same_v<O, jcoreobject>)){
        return QtJambiPrivate::qtjambi_array_cast<O, std::decay_t<T>, size_t, true, JNIEnv *>::cast(in, std::extent_v<std::remove_reference_t<T>>, env);
    }else if constexpr((std::is_reference_v<T> || std::is_pointer_v<std::decay_t<T>>) && !std::is_rvalue_reference_v<T>){
        return QtJambiPrivate::qtjambi_cast<O, T, JNIEnv *>::cast(in, env);
    }else{
        return QtJambiPrivate::qtjambi_cast<O, T&&, JNIEnv *>::cast(std::move(in), env);
    }
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

template<class O, class T, class = void, size_t = 0>
constexpr O qtjambi_cast(QtJambiScope& scope, T&& in){
    if constexpr(std::is_array_v<std::remove_reference_t<T>> && (QtJambiPrivate::jni_type<O>::isArray || std::is_same_v<O, jobject> || std::is_same_v<O, jcoreobject>)){
        return QtJambiPrivate::qtjambi_array_cast<O, std::decay_t<T>, size_t, true, QtJambiScope&>::cast(in, std::extent_v<std::remove_reference_t<T>>, scope);
    }else if constexpr((std::is_reference_v<T> || std::is_pointer_v<std::decay_t<T>>) && !std::is_rvalue_reference_v<T>){
        return QtJambiPrivate::qtjambi_cast<O, T, QtJambiScope&>::cast(scope, in);
    }else{
        return QtJambiPrivate::qtjambi_cast<O, T&&, QtJambiScope&>::cast(scope, std::move(in));
    }
}

template<class O, class T, class = void, size_t N>
constexpr O qtjambi_cast(QtJambiScope& scope, T&& in, const char (&nativeTypeName)[N]){
    if constexpr(std::is_array_v<std::remove_reference_t<T>> && (QtJambiPrivate::jni_type<O>::isArray || std::is_same_v<O, jobject> || std::is_same_v<O, jcoreobject>)){
        return QtJambiPrivate::qtjambi_array_cast<O, std::decay_t<T>, size_t, true, QtJambiScope&,const char(&)[N]>::cast(in, std::extent_v<std::remove_reference_t<T>>, scope, nativeTypeName);
    }else if constexpr((std::is_reference_v<T> || std::is_pointer_v<std::decay_t<T>>) && !std::is_rvalue_reference_v<T>){
        return QtJambiPrivate::qtjambi_cast<O, T, QtJambiScope&,const char(&)[N]>::cast(in, scope, nativeTypeName);
    }else{
        return QtJambiPrivate::qtjambi_cast<O, T&&, QtJambiScope&,const char(&)[N]>::cast(std::move(in), scope, nativeTypeName);
    }
}

template<class O, class T, class = void, size_t = 0>
constexpr O qtjambi_cast(T&& in){
    if constexpr(std::is_array_v<std::remove_reference_t<T>> && (QtJambiPrivate::jni_type<O>::isArray || std::is_same_v<O, jobject> || std::is_same_v<O, jcoreobject>)){
        return QtJambiPrivate::qtjambi_array_cast<O, std::decay_t<T>, size_t, true>::cast(in, std::extent_v<std::remove_reference_t<T>>);
    }else if constexpr((std::is_reference_v<T> || std::is_pointer_v<std::decay_t<T>>) && !std::is_rvalue_reference_v<T>){
        return QtJambiPrivate::qtjambi_cast<O, T>::cast(in);
    }else{
        return QtJambiPrivate::qtjambi_cast<O, T&&>::cast(std::move(in));
    }
}

template<class O, class T, std::enable_if_t<!std::is_assignable_v<QtJambiScope&,T> && !std::is_assignable_v<JNIEnv *&,T>, size_t> N>
constexpr O qtjambi_cast(T&& in, const char (&nativeTypeName)[N]){
    if constexpr(std::is_array_v<std::remove_reference_t<T>> && (QtJambiPrivate::jni_type<O>::isArray || std::is_same_v<O, jobject> || std::is_same_v<O, jcoreobject>)){
        return QtJambiPrivate::qtjambi_array_cast<O, std::decay_t<T>, size_t, true, const char(&)[N]>::cast(in, std::extent_v<std::remove_reference_t<T>>, nativeTypeName);
    }else if constexpr((std::is_reference_v<T> || std::is_pointer_v<std::decay_t<T>>) && !std::is_rvalue_reference_v<T>){
        return QtJambiPrivate::qtjambi_cast<O, T, const char(&)[N]>::cast(in, nativeTypeName);
    }else{
        return QtJambiPrivate::qtjambi_cast<O, T&&, const char(&)[N]>::cast(std::move(in), nativeTypeName);
    }
}

template<class O, class T, class I, size_t N>
constexpr O qtjambi_cast(JNIEnv *env, QtJambiScope& scope, T in, I&& size, const char (&nativeTypeName)[N]){
    typedef typename std::remove_reference<I>::type I_noref;
    Q_STATIC_ASSERT_X(!std::is_pointer<I_noref>::value, "Integer type variable required as size.");
    Q_STATIC_ASSERT_X(std::is_integral<I_noref>::value, "Integer type variable required as size.");
//    Q_STATIC_ASSERT_X(!QtJambiPrivate::is_jni_array_type<T>::value || !std::is_const<typename std::remove_reference<I_noref>::type>::value, "Cannot cast native array from java array without non-const size argument.");
    return QtJambiPrivate::qtjambi_array_cast<O, T, I_noref, !(std::is_reference_v<I> || std::is_pointer_v<I>) || std::is_const_v<I>, JNIEnv *, QtJambiScope&,const char(&)[N]>::cast(in, std::move(size), env, scope, nativeTypeName);
}

template<class O, class T, class I, size_t = 0>
constexpr O qtjambi_cast(JNIEnv *env, QtJambiScope& scope, T in, I&& size){
    typedef typename std::remove_reference<I>::type I_noref;
    Q_STATIC_ASSERT_X(!std::is_pointer<I_noref>::value, "Integer type variable required as size.");
    Q_STATIC_ASSERT_X(std::is_integral<I_noref>::value, "Integer type variable required as size.");
//    Q_STATIC_ASSERT_X(!QtJambiPrivate::is_jni_array_type<T>::value || !std::is_const<typename std::remove_reference<I_noref>::type>::value, "Cannot cast native array from java array without non-const size argument.");
    return QtJambiPrivate::qtjambi_array_cast<O, T, I_noref, !(std::is_reference_v<I> || std::is_pointer_v<I>) || std::is_const_v<I>, JNIEnv *, QtJambiScope&>::cast(in, std::move(size), env, scope);
}

template<class O, class T, class I, size_t N>
constexpr O qtjambi_cast(JNIEnv *env, T in, I&& size, const char (&nativeTypeName)[N]){
    typedef typename std::remove_reference<I>::type I_noref;
    Q_STATIC_ASSERT_X(!std::is_pointer<I_noref>::value, "Integer type variable required as size.");
    Q_STATIC_ASSERT_X(std::is_integral<I_noref>::value, "Integer type variable required as size.");
//    Q_STATIC_ASSERT_X(!QtJambiPrivate::is_jni_array_type<T>::value || !std::is_const<typename std::remove_reference<I_noref>::type>::value, "Cannot cast native array from java array without non-const size argument.");
    return QtJambiPrivate::qtjambi_array_cast<O, T, I_noref, !(std::is_reference_v<I> || std::is_pointer_v<I>) || std::is_const_v<I>, JNIEnv *, const char(&)[N]>::cast(in, std::move(size), env, nativeTypeName);
}

template<class O, class T, class I, size_t = 0>
constexpr O qtjambi_cast(JNIEnv *env, T in, I&& size){
    typedef typename std::remove_reference<I>::type I_noref;
    Q_STATIC_ASSERT_X(!std::is_pointer<I_noref>::value, "Integer type variable required as size.");
    Q_STATIC_ASSERT_X(std::is_integral<I_noref>::value, "Integer type variable required as size.");
//    Q_STATIC_ASSERT_X(!QtJambiPrivate::is_jni_array_type<T>::value || !std::is_const<typename std::remove_reference<I_noref>::type>::value, "Cannot cast native array from java array without non-const size argument.");
    return QtJambiPrivate::qtjambi_array_cast<O, T, I_noref, !(std::is_reference_v<I> || std::is_pointer_v<I>) || std::is_const_v<I>, JNIEnv *>::cast(in, std::move(size), env);
}

template<class O, class T, class I, size_t N>
constexpr O qtjambi_cast(QtJambiScope& scope, T in, I&& size, const char (&nativeTypeName)[N]){
    typedef typename std::remove_reference<I>::type I_noref;
    Q_STATIC_ASSERT_X(!std::is_pointer<I_noref>::value, "Integer type variable required as size.");
    Q_STATIC_ASSERT_X(std::is_integral<I_noref>::value, "Integer type variable required as size.");
//    Q_STATIC_ASSERT_X(!QtJambiPrivate::is_jni_array_type<T>::value || !std::is_const<typename std::remove_reference<I_noref>::type>::value, "Cannot cast native array from java array without non-const size argument.");
    return QtJambiPrivate::qtjambi_array_cast<O, T, I_noref, !(std::is_reference_v<I> || std::is_pointer_v<I>) || std::is_const_v<I>, QtJambiScope&,const char(&)[N]>::cast(in, std::move(size), scope, nativeTypeName);
}

template<class O, class T, class I, size_t = 0>
constexpr O qtjambi_cast(QtJambiScope& scope, T in, I&& size){
    typedef typename std::remove_reference<I>::type I_noref;
    Q_STATIC_ASSERT_X(!std::is_pointer<I_noref>::value, "Integer type variable required as size.");
    Q_STATIC_ASSERT_X(std::is_integral<I_noref>::value, "Integer type variable required as size.");
//    Q_STATIC_ASSERT_X(!QtJambiPrivate::is_jni_array_type<T>::value || !std::is_const<typename std::remove_reference<I_noref>::type>::value, "Cannot cast native array from java array without non-const size argument.");
    return QtJambiPrivate::qtjambi_array_cast<O, T, I_noref, !(std::is_reference_v<I> || std::is_pointer_v<I>) || std::is_const_v<I>, QtJambiScope&>::cast(in, std::move(size), scope);
}

template<class O, class T, class I, std::enable_if_t<!std::is_assignable_v<QtJambiScope&,T> && !std::is_assignable_v<JNIEnv *&,T>, size_t> N>
constexpr O qtjambi_cast(T in, I&& size, const char (&nativeTypeName)[N]){
    typedef typename std::remove_reference<I>::type I_noref;
    Q_STATIC_ASSERT_X(!std::is_pointer<I_noref>::value, "Integer type variable required as size.");
//    Q_STATIC_ASSERT_X(!QtJambiPrivate::is_jni_array_type<T>::value || !std::is_const<typename std::remove_reference<I_noref>::type>::value, "Cannot cast native array from java array without non-const size argument.");
    return QtJambiPrivate::qtjambi_array_cast<O, T, I_noref, !(std::is_reference_v<I> || std::is_pointer_v<I>) || std::is_const_v<I>, const char(&)[N]>::cast(in, std::move(size), nativeTypeName);
}

template<class O, class T, class I, std::enable_if_t<!std::is_assignable_v<QtJambiScope&,T> && !std::is_assignable_v<JNIEnv *&,T>, size_t> = 0>
constexpr O qtjambi_cast(T in, I&& size){
    typedef typename std::remove_reference<I>::type I_noref;
    Q_STATIC_ASSERT_X(!std::is_pointer<I_noref>::value, "Integer type variable required as size.");
    Q_STATIC_ASSERT_X(!QtJambiPrivate::is_jni_array_type<T>::value || !std::is_const<typename std::remove_reference<I_noref>::type>::value, "Cannot cast native array from java array without non-const size argument.");
    return QtJambiPrivate::qtjambi_array_cast<O, T, I_noref, !(std::is_reference_v<I> || std::is_pointer_v<I>) || std::is_const_v<I>>::cast(in, std::move(size));
}

template<class O, class T, size_t N>
void qtjambi_copy_into(JNIEnv *env, O javaArray, T(&nativeArray)[N]){
    Q_STATIC_ASSERT_X(QtJambiPrivate::jni_type<O>::isArray, "qtjambi_copy_into can only be used for java array types");
    jsize size{0};
    if((size = javaArray ? env->GetArrayLength(javaArray) : 0) != N)
        JavaException::raiseIllegalArgumentException(env, QString("Wrong number of elements in array. Found: %1, expected: %2").arg(size).arg(N) QTJAMBI_STACKTRACEINFO);
    if constexpr(QtJambiPrivate::jni_type<O>::isPrimitiveArray){
        if constexpr(std::is_same_v<O, jbooleanArray> && sizeof(T)!=sizeof(jboolean)){
            jboolean buffer[N];
            (env->*QtJambiPrivate::jni_type<O>::GetArrayRegion)(javaArray, 0, jsize(N), buffer);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            for(size_t i=0; i<N; ++i){
                nativeArray[i] = buffer[i];
            }
        }else{
            Q_STATIC_ASSERT_X(sizeof(T)==sizeof(typename QtJambiPrivate::jni_type<O>::ElementType), "array element size mismatch");
            (env->*QtJambiPrivate::jni_type<O>::GetArrayRegion)(javaArray, 0, jsize(N), reinterpret_cast<typename QtJambiPrivate::jni_type<O>::ElementType*>(nativeArray));
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
        }
    }else{
        for(size_t i=0; i<N; ++i){
            jobject element = env->GetObjectArrayElement(javaArray, jsize(i));
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            nativeArray[i] = qtjambi_cast<T>(env, element);
        }
    }
}

#endif // QTJAMBI_CONVERT_H
