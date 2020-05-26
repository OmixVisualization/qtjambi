/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBI_CAST_JNITYPE_P_H
#define QTJAMBI_CAST_JNITYPE_P_H

#include "qtjambi_core.h"
#include "qtjambi_cast_util_p.h"
#include "qtjambi_cast_container1_p.h"
#include "qtjambi_cast_container2_p.h"

namespace QtJambiPrivate {

template<bool forward, bool has_scope,
         typename JniType,
         typename NativeType, bool is_arithmetic, bool is_enum, bool is_pointer, bool is_const, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jnitype_caster{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static JniType castfwd(JNIEnv *, NativeType_in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
        return JniType();
    }
    static NativeType_out castbck(JNIEnv *, JniType, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
        return NativeType();
    }
};

template<bool forward, bool has_scope,
         typename JniType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         int parameterCount, typename... Ts>
struct qtjambi_jnitype_container_cast_decider{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType<Ts...>>::type, NativeType<Ts...>>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static JniType castfwd(JNIEnv *, NativeType_in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
        return JniType();
    }
    static NativeType_out castbck(JNIEnv *, JniType, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
        return NativeType<Ts...>();
    }
};

template<bool has_scope,
         typename JniType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename... Ts>
struct qtjambi_jnitype_container_cast_decider<true, has_scope, JniType, NativeType, is_pointer, is_const, is_reference, 1, Ts...>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType<Ts...>>::type, NativeType<Ts...>>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    typedef JniType (*CastFwdReturn)(JNIEnv *, NativeType_in, const char*, QtJambiScope*);
    template<template<typename T> class Container, typename T>
    static constexpr CastFwdReturn _castfwd(){
        return qtjambi_jnitype_container1_cast<true, has_scope, JniType, Container, is_pointer, is_const, is_reference, T>::castfwd;
    }

    static constexpr JniType (*castfwd)(JNIEnv *, NativeType_in, const char*, QtJambiScope*) = _castfwd<NativeType, Ts...>();
};

template<bool has_scope,
         typename JniType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename... Ts>
struct qtjambi_jnitype_container_cast_decider<false, has_scope, JniType, NativeType, is_pointer, is_const, is_reference, 1, Ts...>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType<Ts...>>::type, NativeType<Ts...>>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    typedef NativeType_out (*CastBckReturn)(JNIEnv *, JniType, const char*, QtJambiScope*);
    template<template<typename T> class Container, typename T>
    static constexpr CastBckReturn _castbck(){
        return qtjambi_jnitype_container1_cast<false, has_scope, JniType, Container, is_pointer, is_const, is_reference, T>::castbck;
    }

    static constexpr NativeType_out (*castbck)(JNIEnv *, JniType, const char*, QtJambiScope*) = _castbck<NativeType, Ts...>();
};

template<bool has_scope,
         typename JniType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename... Ts>
struct qtjambi_jnitype_container_cast_decider<true, has_scope, JniType, NativeType, is_pointer, is_const, is_reference, 2, Ts...>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType<Ts...>>::type, NativeType<Ts...>>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    typedef JniType (*CastFwdReturn)(JNIEnv *, NativeType_in, const char*, QtJambiScope*);
    template<template<typename K, typename T> class Container, typename K, typename T>
    static constexpr CastFwdReturn _castfwd(){
        return qtjambi_jnitype_container2_cast<true, has_scope, JniType, Container, is_pointer, is_const, is_reference, K, T>::castfwd;
    }

    static constexpr JniType (*castfwd)(JNIEnv *, NativeType_in, const char*, QtJambiScope*) = _castfwd<NativeType, Ts...>();
};

template<bool has_scope,
         typename JniType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename... Ts>
struct qtjambi_jnitype_container_cast_decider<false, has_scope, JniType, NativeType, is_pointer, is_const, is_reference, 2, Ts...>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType<Ts...>>::type, NativeType<Ts...>>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    typedef NativeType_out (*CastBckReturn)(JNIEnv *, JniType, const char*, QtJambiScope*);
    template<template<typename K, typename T> class Container, typename K, typename T>
    static constexpr CastBckReturn _castbck(){
        return qtjambi_jnitype_container2_cast<false, has_scope, JniType, Container, is_pointer, is_const, is_reference, K, T>::castbck;
    }

    static constexpr NativeType_out (*castbck)(JNIEnv *, JniType, const char*, QtJambiScope*) = _castbck<NativeType, Ts...>();
};

template<bool forward, bool has_scope,
         typename JniType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename... Ts>
struct qtjambi_jnitype_caster<forward, has_scope,
                                 JniType,
                                 NativeType<Ts...>, false, false, is_pointer, is_const, is_reference, false, true>
        : qtjambi_jnitype_container_cast_decider<forward, has_scope, JniType, NativeType, is_pointer, is_const, is_reference, sizeof...(Ts), Ts...>{
};

//template from any function pointer to jobject

template<bool forward, bool has_scope,
         typename NativeType, bool is_const, bool is_reference, bool is_template>
struct qtjambi_jnitype_caster<forward, has_scope,
        jobject,
        NativeType, false, false, true, is_const, is_reference, true, is_template>{
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_in;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char* nativeTypeName, QtJambiScope*){
        jobject result = nullptr;
        if(!qtjambi_convert_from_native(env, typeid(NativeType_ptr), nativeTypeName, &in, true, false, result)){
           JavaException::raiseRuntimeException(env, qPrintable(QString("Cannot cast from type %1").arg(nativeTypeName ? nativeTypeName : typeid(NativeType_ptr).name())) QTJAMBI_STACKTRACEINFO );
        }
        return result;
    }

    static NativeType_out castbck(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope*){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to function pointer reference");
        NativeType_ptr fct = nullptr;
        NativeType_ptr* result = &fct;
        if(!qtjambi_convert_to_native(env, typeid(NativeType_ptr), nativeTypeName, in, &result)){
            JavaException::raiseRuntimeException(env, qPrintable(QString("Cannot cast to type %1").arg(typeid(typename std::remove_pointer<NativeType>::type).name())) QTJAMBI_STACKTRACEINFO );
        }
        return result ? *result : nullptr;
    }
};

//template for any QObject(&/*) and non-QObject

template<bool has_scope,
         typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_q_object>
struct qtjambi_jnitype_qobject_decider_cast{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *, NativeType_in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
        return jobject();
    }
    static NativeType_out castbck(JNIEnv *, jobject, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
        return NativeType();
    }
};

template<bool forward, bool has_scope,
         typename NativeType, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<forward, has_scope,
        jobject,
        NativeType, false, false, is_pointer, is_const, is_reference, false, false>
        : qtjambi_jnitype_qobject_decider_cast<has_scope, NativeType, is_pointer, is_const, is_reference, std::is_base_of<QObject, NativeType>::value>
{
};

//template for QObject*

template<bool has_scope,
         typename NativeType, bool is_const, bool is_reference>
struct qtjambi_jnitype_qobject_decider_cast<has_scope, NativeType, true, is_const, is_reference, true>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_in;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_out;
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;
    static jobject castfwd(JNIEnv * env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_qobject(env, in, typeid(NativeType));
    }

    static NativeType_out castbck(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope*){
        NativeType_ptr result = nullptr;
        if(!qtjambi_convert_to_native(env, typeid(NativeType), nativeTypeName, in, &result)){
            JavaException::raiseRuntimeException(env, qPrintable(QString("Cannot cast to type %1").arg(typeid(NativeType).name())) QTJAMBI_STACKTRACEINFO );
        }
        return result;
    }
};

//template for QObject&

template<bool has_scope,
         typename NativeType, bool is_const>
struct qtjambi_jnitype_qobject_decider_cast<has_scope, NativeType, false, is_const, true, true>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_out;
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;
    static jobject castfwd(JNIEnv * env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_qobject(env, &in, typeid(NativeType));
    }

    static NativeType_out castbck(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope*){
        NativeType_ptr result = nullptr;
        if(!qtjambi_convert_to_native(env, typeid(NativeType), nativeTypeName, in, &result)){
            JavaException::raiseRuntimeException(env, qPrintable(QString("Cannot cast to type %1").arg(typeid(NativeType).name())) QTJAMBI_STACKTRACEINFO );
        }
        return *result;
    }
};

//template for any non-QObject pointer type (e.g. QGraphicsItem*)

template<bool has_scope,
         typename NativeType, bool is_const, bool is_reference>
struct qtjambi_jnitype_qobject_decider_cast<has_scope, NativeType, true, is_const, is_reference, false>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_in;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_out;
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;
    static jobject castfwd(JNIEnv * env, NativeType_in in, const char*, QtJambiScope* scope){
        jobject o = qtjambi_from_object(env, in, typeid(NativeType), false, false);
        if(scope){
            scope->addFinalAction([env, o](){qtjambi_invalidate_object(env, o); env->DeleteLocalRef(o); });
        }
        return o;
    }

    static NativeType_out castbck(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope*){
        NativeType_ptr result = nullptr;
        if(!qtjambi_convert_to_native(env, typeid(NativeType), nativeTypeName, in, &result)){
            JavaException::raiseRuntimeException(env, qPrintable(QString("Cannot cast to type %1").arg(typeid(NativeType).name())) QTJAMBI_STACKTRACEINFO );
        }
        return result;
    }
};

//template for any non-QObject reference type

template<bool has_scope,
         typename NativeType, bool is_const, bool is_reference, bool is_copy_assignable>
struct qtjambi_jnitype_copy_assignable_decider_cast{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    static NativeType_cr castbck(JNIEnv *, jobject, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
        return NativeType();
    }
};

template<bool has_scope,
         typename NativeType, bool is_const, bool is_reference>
struct qtjambi_jnitype_qobject_decider_cast<has_scope, NativeType, false, is_const, is_reference, false>
        : qtjambi_jnitype_copy_assignable_decider_cast<has_scope, NativeType, is_const, is_reference, std::is_copy_assignable<NativeType>::value>
{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_out;
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;
    static jobject castfwd(JNIEnv * env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_object(env, &in, typeid(NativeType), true, false);
    }
};

template<bool has_scope,
         typename NativeType, bool is_const, bool is_reference, bool dont_use_pointer>
struct qtjambi_jnitype_use_pointer_decider_cast{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    static NativeType_cr castbck(JNIEnv *, jobject, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
        return NativeType();
    }
};

template<bool has_scope,
         typename NativeType, bool is_const, bool is_reference>
struct qtjambi_jnitype_copy_assignable_decider_cast<has_scope, NativeType, is_const, is_reference, true>
        : qtjambi_jnitype_use_pointer_decider_cast<has_scope, NativeType, is_const, is_reference, is_lightweight_java_type<NativeType>::value>
{
};

template<bool has_scope,
         typename NativeType, bool is_const, bool is_reference>
struct qtjambi_jnitype_copy_assignable_decider_cast<has_scope, NativeType, is_const, is_reference, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    static NativeType_cr castbck(JNIEnv * env, jobject in, const char* nativeTypeName, QtJambiScope*){
        NativeType* result = nullptr;
        if(!qtjambi_convert_to_native(env, typeid(NativeType), nativeTypeName, in, &result)){
            JavaException::raiseRuntimeException(env, qPrintable(QString("Cannot cast to type %1").arg(typeid(NativeType).name())) QTJAMBI_STACKTRACEINFO );
        }
        return checked_deref<NativeType>(env, result);
    }
};

template<bool has_scope,
         typename NativeType, bool is_const, bool is_reference>
struct qtjambi_jnitype_use_pointer_decider_cast<has_scope, NativeType, is_const, is_reference, true /*dont_use_pointer*/>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    static NativeType_cr castbck(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope*){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to value reference");
        NativeType result;
        if(!qtjambi_convert_to_native(env, typeid(NativeType), nativeTypeName, in, &result)){
            JavaException::raiseRuntimeException(env, qPrintable(QString("Cannot cast to type %1").arg(typeid(NativeType).name())) QTJAMBI_STACKTRACEINFO );
        }
        return result;
    }
};

template<bool has_scope,
         typename NativeType, bool is_const, bool is_reference>
struct qtjambi_jnitype_use_pointer_decider_cast<has_scope, NativeType, is_const, is_reference, false /*dont_use_pointer*/>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;

    static NativeType_cr castbck(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope*){
        NativeType_ptr result = nullptr;
        if(!qtjambi_convert_to_native(env, typeid(NativeType), nativeTypeName, in, &result)){
            JavaException::raiseRuntimeException(env, qPrintable(QString("Cannot cast to type %1").arg(typeid(NativeType).name())) QTJAMBI_STACKTRACEINFO );
        }
        return qtjambi_deref_value<NativeType, supports_StandardConstructor<NativeType>::value, is_const, is_reference>::deref(env, result);
    }
};

// template for jstring

template<bool has_scope,
         typename NativeType, bool is_arithmetic, bool is_enum, bool is_pointer, bool is_const, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jstring_caster{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out castbck(JNIEnv *, jstring, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
        return NativeType();
    }
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_const, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QString, is_arithmetic, is_enum, false, is_const, false, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QString>::type, QString>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out castbck(JNIEnv *env, jstring in, const char* nativeTypeName, QtJambiScope*){
        QString result;
        if(!qtjambi_convert_to_native(env, typeid(QString), nativeTypeName, in, &result)){
            JavaException::raiseRuntimeException(env, "Cannot cast to type QString" QTJAMBI_STACKTRACEINFO );
        }
        return result;
    }
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_pointer, bool is_const, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QString, is_arithmetic, is_enum, is_pointer, is_const, is_reference, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QString>::type, QString>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out castbck(JNIEnv *env, jstring in, const char* nativeTypeName, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QString & without scope.");
        Q_STATIC_ASSERT_X(!is_pointer || has_scope, "Cannot cast to QString* without scope.");
        if((is_reference || is_pointer) && !scope)
            JavaException::raiseError(env, "Cannot cast to QString pointer or reference without scope." QTJAMBI_STACKTRACEINFO );
        QString* result = new QString();
        if(!qtjambi_convert_to_native(env, typeid(QString), nativeTypeName, in, result)){
            JavaException::raiseRuntimeException(env, "Cannot cast to type QString" QTJAMBI_STACKTRACEINFO );
        }
        scope->addFinalAction([result](){delete result;});
        return deref_ptr<is_pointer,typename std::conditional<is_pointer, QString*, QString>::type>::deref(result);
    }
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_const, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QStringRef, is_arithmetic, is_enum, false, is_const, is_reference, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QStringRef>::type, QStringRef>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out castbck(JNIEnv *env, jstring in, const char* nativeTypeName, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QStringRef &.");
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QStringRef without scope.");
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QStringRef without scope." QTJAMBI_STACKTRACEINFO );
        QString *result = new QString();
        if(!qtjambi_convert_to_native(env, typeid(QString), nativeTypeName, in, result)){
            delete result;
            JavaException::raiseRuntimeException(env, "Cannot cast to type QStringRef" QTJAMBI_STACKTRACEINFO );
        }
        scope->addFinalAction([result](){delete result;});
        return QStringRef(result);
    }
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_const, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QStringRef, is_arithmetic, is_enum, true, is_const, false, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QStringRef>::type, QStringRef>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out castbck(JNIEnv *env, jstring in, const char* nativeTypeName, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QStringRef without scope.");
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QStringRef without scope." QTJAMBI_STACKTRACEINFO );
        QString *result = new QString();
        if(!qtjambi_convert_to_native(env, typeid(QString), nativeTypeName, in, result)){
            delete result;
            JavaException::raiseRuntimeException(env, "Cannot cast to type QStringRef" QTJAMBI_STACKTRACEINFO );
        }
        QStringRef* ref = new QStringRef(result);
        scope->addFinalAction([result,ref](){delete result; delete ref;});
        return ref;
    }
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_const, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QStringView, is_arithmetic, is_enum, false, is_const, is_reference, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QStringView>::type, QStringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out castbck(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QStringView &.");
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QStringView without scope.");
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QStringView without scope." QTJAMBI_STACKTRACEINFO );
        const jchar* chars = env->GetStringChars(in, nullptr);
        jsize length = env->GetStringLength(in);
        scope->addFinalAction([chars, env, in](){env->ReleaseStringChars(in, chars);});
        return QStringView(chars, length);
    }
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_const, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QStringView, is_arithmetic, is_enum, true, is_const, false, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QStringView>::type, QStringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out castbck(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QStringView without scope.");
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QStringView without scope." QTJAMBI_STACKTRACEINFO );
        const jchar* chars = env->GetStringChars(in, nullptr);
        jsize length = env->GetStringLength(in);
        QStringView* ref = new QStringView(chars, length);
        scope->addFinalAction([chars, env, in, ref](){delete ref; env->ReleaseStringChars(in, chars); });
        return ref;
    }
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_const, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QLatin1String, is_arithmetic, is_enum, false, is_const, is_reference, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1String>::type, QLatin1String>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out castbck(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QLatin1String &.");
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QLatin1String without scope.");
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QLatin1String without scope." QTJAMBI_STACKTRACEINFO );
        const char* chars = env->GetStringUTFChars(in, nullptr);
        jsize length = env->GetStringUTFLength(in);
        scope->addFinalAction([chars, env, in](){env->ReleaseStringUTFChars(in, chars);});
        return QLatin1String(chars, length);
    }
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_const, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QLatin1String, is_arithmetic, is_enum, true, is_const, false, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1String>::type, QLatin1String>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out castbck(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QLatin1String without scope.");
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QLatin1String without scope." QTJAMBI_STACKTRACEINFO );
        const char* chars = env->GetStringUTFChars(in, nullptr);
        jsize length = env->GetStringUTFLength(in);
        QLatin1String* ref = new QLatin1String(chars, length);
        scope->addFinalAction([chars, env, in, ref](){delete ref; env->ReleaseStringUTFChars(in, chars); });
        return ref;
    }
};

template<bool forward, bool has_scope,
         bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<forward, has_scope,
        jobject,
        QStringRef, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringRef>::type, QStringRef>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_qstringref(env, in);
    }
    static NativeType_out castbck(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QStringRef &.");
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QStringRef without scope.");
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QStringRef without scope." QTJAMBI_STACKTRACEINFO );
        QString *result = new QString();
        if(!qtjambi_convert_to_native(env, typeid(QString), nativeTypeName, in, result)){
            delete result;
            JavaException::raiseRuntimeException(env, "Cannot cast to type QStringRef" QTJAMBI_STACKTRACEINFO );
        }
        scope->addFinalAction([result](){delete result;});
        return QStringRef(result);
    }
};

template<bool forward, bool has_scope,
         bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<forward, has_scope,
        jstring,
        QStringRef, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringRef>::type, QStringRef>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jstring castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_qstringref(env, in);
    }
    static NativeType_out castbck(JNIEnv *env, jstring in, const char* nativeTypeName, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QStringRef &.");
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QStringRef without scope.");
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QStringRef without scope." QTJAMBI_STACKTRACEINFO );
        QString *result = new QString();
        if(!qtjambi_convert_to_native(env, typeid(QString), nativeTypeName, in, result)){
            delete result;
            JavaException::raiseRuntimeException(env, "Cannot cast to type QStringRef" QTJAMBI_STACKTRACEINFO );
        }
        scope->addFinalAction([result](){delete result;});
        return QStringRef(result);
    }
};

template<bool forward, bool has_scope,
         bool is_const>
struct qtjambi_jnitype_caster<forward, has_scope,
        jobject,
        QStringRef, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringRef>::type, QStringRef>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return in ? qtjambi_from_qstringref(env, *in) : nullptr;
    }
    static NativeType_out castbck(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QStringRef without scope.");
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QStringRef without scope." QTJAMBI_STACKTRACEINFO );
        QString *result = new QString();
        if(!qtjambi_convert_to_native(env, typeid(QString), nativeTypeName, in, result)){
            delete result;
            JavaException::raiseRuntimeException(env, "Cannot cast to type QStringRef" QTJAMBI_STACKTRACEINFO );
        }
        QStringRef* ref = new QStringRef(result);
        scope->addFinalAction([result,ref](){delete result; delete ref;});
        return ref;
    }
};

template<bool forward, bool has_scope,
         bool is_const>
struct qtjambi_jnitype_caster<forward, has_scope,
        jstring,
        QStringRef, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringRef>::type, QStringRef>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jstring castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return in ? qtjambi_from_qstringref(env, *in) : nullptr;
    }
    static NativeType_out castbck(JNIEnv *env, jstring in, const char* nativeTypeName, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QStringRef without scope.");
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QStringRef without scope." QTJAMBI_STACKTRACEINFO );
        QString *result = new QString();
        if(!qtjambi_convert_to_native(env, typeid(QString), nativeTypeName, in, result)){
            delete result;
            JavaException::raiseRuntimeException(env, "Cannot cast to type QStringRef" QTJAMBI_STACKTRACEINFO );
        }
        QStringRef* ref = new QStringRef(result);
        scope->addFinalAction([result,ref](){delete result; delete ref;});
        return ref;
    }
};

template<bool forward, bool has_scope,
         bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<forward, has_scope,
        jstring,
        QStringView, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringView>::type, QStringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jstring castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_qstringview(env, in);
    }
    static NativeType_out castbck(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QStringView &.");
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QStringView without scope.");
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QStringView without scope." QTJAMBI_STACKTRACEINFO );
        const jchar* chars = env->GetStringChars(in, nullptr);
        jsize length = env->GetStringLength(in);
        scope->addFinalAction([chars, env, in](){env->ReleaseStringChars(in, chars);});
        return QStringView(chars, length);
    }
};

template<bool forward, bool has_scope,
         bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<forward, has_scope,
        jobject,
        QStringView, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringView>::type, QStringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_qstringview(env, in);
    }
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QStringView &.");
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QStringView without scope.");
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QStringView without scope." QTJAMBI_STACKTRACEINFO );
        jstring _in = qtjambi_to_jstring(env, in);
        const jchar* chars = env->GetStringChars(_in, nullptr);
        jsize length = env->GetStringLength(_in);
        scope->addFinalAction([chars, env, _in](){env->ReleaseStringChars(_in, chars);});
        return QStringView(chars, length);
    }
};

template<bool forward, bool has_scope,
         bool is_const>
struct qtjambi_jnitype_caster<forward, has_scope,
        jstring,
        QStringView, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringView>::type, QStringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jstring castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return in ? qtjambi_from_qstringview(env, *in) : nullptr;
    }
    static NativeType_out castbck(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QStringView without scope.");
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QStringView without scope." QTJAMBI_STACKTRACEINFO );
        const jchar* chars = env->GetStringChars(in, nullptr);
        jsize length = env->GetStringLength(in);
        QStringView* ref = new QStringView(chars, length);
        scope->addFinalAction([chars, env, in, ref](){delete ref; env->ReleaseStringChars(in, chars);});
        return ref;
    }
};

template<bool forward, bool has_scope,
         bool is_const>
struct qtjambi_jnitype_caster<forward, has_scope,
        jobject,
        QStringView, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringView>::type, QStringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return in ? qtjambi_from_qstringview(env, *in) : nullptr;
    }
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QStringView without scope.");
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QStringView without scope." QTJAMBI_STACKTRACEINFO );
        jstring _in = qtjambi_to_jstring(env, in);
        const jchar* chars = env->GetStringChars(_in, nullptr);
        jsize length = env->GetStringLength(_in);
        QStringView* ref = new QStringView(chars, length);
        scope->addFinalAction([chars, env, _in, ref](){delete ref; env->ReleaseStringChars(_in, chars);});
        return ref;
    }
};

template<bool forward, bool has_scope,
         bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<forward, has_scope,
        jstring,
        QLatin1String, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1String>::type, QLatin1String>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jstring castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_qstring(env, in);
    }
    static NativeType_out castbck(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QLatin1String &.");
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QLatin1String without scope.");
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QLatin1String without scope." QTJAMBI_STACKTRACEINFO );
        const char* chars = env->GetStringUTFChars(in, nullptr);
        jsize length = env->GetStringUTFLength(in);
        scope->addFinalAction([chars, env, in](){env->ReleaseStringUTFChars(in, chars);});
        return QLatin1String(chars, length);
    }
};

template<bool forward, bool has_scope,
         bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<forward, has_scope,
        jobject,
        QLatin1String, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1String>::type, QLatin1String>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_qstring(env, in);
    }
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QLatin1String &.");
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QLatin1String without scope.");
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QLatin1String without scope." QTJAMBI_STACKTRACEINFO );
        jstring _in = qtjambi_to_jstring(env, in);
        const char* chars = env->GetStringUTFChars(_in, nullptr);
        jsize length = env->GetStringUTFLength(_in);
        scope->addFinalAction([chars, env, _in](){env->ReleaseStringUTFChars(_in, chars);});
        return QLatin1String(chars, length);
    }
};

template<bool forward, bool has_scope,
         bool is_const>
struct qtjambi_jnitype_caster<forward, has_scope,
        jstring,
        QLatin1String, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1String>::type, QLatin1String>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jstring castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return in ? qtjambi_from_qstring(env, *in) : nullptr;
    }
    static NativeType_out castbck(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QLatin1String without scope.");
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QLatin1String without scope." QTJAMBI_STACKTRACEINFO );
        const char* chars = env->GetStringUTFChars(in, nullptr);
        jsize length = env->GetStringUTFLength(in);
        QLatin1String* ref = new QLatin1String(chars, length);
        scope->addFinalAction([chars, env, in, ref](){delete ref; env->ReleaseStringUTFChars(in, chars);});
        return ref;
    }
};

template<bool forward, bool has_scope,
         bool is_const>
struct qtjambi_jnitype_caster<forward, has_scope,
        jobject,
        QLatin1String, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1String>::type, QLatin1String>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return in ? qtjambi_from_qstring(env, *in) : nullptr;
    }
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QLatin1String without scope.");
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QLatin1String without scope." QTJAMBI_STACKTRACEINFO );
        jstring _in = qtjambi_to_jstring(env, in);
        const char* chars = env->GetStringUTFChars(_in, nullptr);
        jsize length = env->GetStringUTFLength(_in);
        QLatin1String* ref = new QLatin1String(chars, length);
        scope->addFinalAction([chars, env, _in, ref](){delete ref; env->ReleaseStringUTFChars(_in, chars);});
        return ref;
    }
};

template<bool forward, bool has_scope,
         typename NativeType, bool is_arithmetic, bool is_enum, bool is_pointer, bool is_const, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jnitype_caster<forward, has_scope, jstring, NativeType, is_arithmetic, is_enum, is_pointer, is_const, is_reference, is_function, is_template>
        : qtjambi_jstring_caster<has_scope, NativeType, is_arithmetic, is_enum, is_pointer, is_const, is_reference, is_function, is_template>
{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jstring castfwd(JNIEnv *env, NativeType_in in, const char* nativeTypeName, QtJambiScope* scope){
        jobject result = qtjambi_scoped_cast<has_scope,jobject,NativeType_in>::cast(env, in, nativeTypeName, scope);
        return qtjambi_to_jstring(env, result);
    }
};

// enums

template<bool forward, bool has_scope,
         typename NativeType, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<forward, has_scope,
        jobject,
        NativeType, false, true, false, is_const, is_reference, false, false>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_object(env, &in, typeid(NativeType), false, false);
    }
    static NativeType_out castbck(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope*){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast jobject to enumerator reference");
        NativeType result;
        if(!qtjambi_convert_to_native(env, typeid(NativeType), nativeTypeName, in, &result)){
            JavaException::raiseRuntimeException(env, qPrintable(QString("Cannot cast to type %1").arg(typeid(NativeType).name())) QTJAMBI_STACKTRACEINFO );
        }
        return result;
    }
};

// object <=> arithmetic

template<bool has_scope, typename NativeType, bool a_is_const, bool a_is_reference, bool a_is_integral, bool a_is_floating_point, bool a_is_signed, size_t a_size>
struct qtjambi_jnitype_arithmetic_caster{
    typedef typename std::conditional<a_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<a_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    static jobject castfwd(JNIEnv *, NativeType_in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
        return nullptr;
    }
    static NativeType_out castbck(JNIEnv *, jobject, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
        return NativeType(0);
    }
};

template<bool forward, bool has_scope, typename NativeType, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<forward, has_scope,
        jobject,
        NativeType, true, false, false, is_const, is_reference, false, false>
        : qtjambi_jnitype_arithmetic_caster<
            has_scope,
            NativeType,
            is_const,
            is_reference,
            std::is_integral<NativeType>::value,
            std::is_floating_point<NativeType>::value,
            std::is_signed<NativeType>::value,
            sizeof(NativeType)>
{
};

template<bool has_scope, typename NativeType, bool a_is_const, bool a_is_reference, bool a_is_signed>
struct qtjambi_jnitype_arithmetic_caster<has_scope,
        NativeType, a_is_const, a_is_reference, true, false, a_is_signed, sizeof(jbyte)>{
    typedef typename std::conditional<a_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<a_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_byte(env, jbyte(in));
    }
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(!a_is_reference, "Cannot cast jobject to arithmetic reference");
        return NativeType(qtjambi_to_byte(env, in));
    }
};

template<bool has_scope, typename NativeType, bool a_is_const, bool a_is_reference, bool a_is_signed>
struct qtjambi_jnitype_arithmetic_caster<has_scope,
        NativeType, a_is_const, a_is_reference, true, false, a_is_signed, sizeof(jshort)>{
    typedef typename std::conditional<a_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<a_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_short(env, jshort(in));
    }
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(!a_is_reference, "Cannot cast jobject to arithmetic reference");
        return NativeType(qtjambi_to_short(env, in));
    }
};

template<bool has_scope, typename NativeType, bool a_is_const, bool a_is_reference, bool a_is_signed>
struct qtjambi_jnitype_arithmetic_caster<has_scope,
        NativeType, a_is_const, a_is_reference, true, false, a_is_signed, sizeof(jint)>{
    typedef typename std::conditional<a_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<a_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_int(env, jint(in));
    }
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(!a_is_reference, "Cannot cast jobject to arithmetic reference");
        return NativeType(qtjambi_to_int(env, in));
    }
};

template<bool has_scope, typename NativeType, bool a_is_const, bool a_is_reference, bool a_is_signed>
struct qtjambi_jnitype_arithmetic_caster<has_scope,
        NativeType, a_is_const, a_is_reference, true, false, a_is_signed, sizeof(jlong)>{
    typedef typename std::conditional<a_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<a_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_long(env, jlong(in));
    }
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(!a_is_reference, "Cannot cast jobject to arithmetic reference");
        return NativeType(qtjambi_to_long(env, in));
    }
};

template<bool has_scope, typename NativeType, bool a_is_const, bool a_is_reference, bool a_is_signed>
struct qtjambi_jnitype_arithmetic_caster<has_scope,
        NativeType, a_is_const, a_is_reference, false, true, a_is_signed, sizeof(jdouble)>{
    typedef typename std::conditional<a_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<a_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_double(env, jdouble(in));
    }
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(!a_is_reference, "Cannot cast jobject to arithmetic reference");
        return NativeType(qtjambi_to_double(env, in));
    }
};

template<bool has_scope, typename NativeType, bool a_is_const, bool a_is_reference, bool a_is_signed>
struct qtjambi_jnitype_arithmetic_caster<has_scope,
        NativeType, a_is_const, a_is_reference, false, true, a_is_signed, sizeof(jfloat)>{
    typedef typename std::conditional<a_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<a_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_float(env, jfloat(in));
    }
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(!a_is_reference, "Cannot cast jobject to arithmetic reference");
        return NativeType(qtjambi_to_float(env, in));
    }
};

template<bool forward, bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<forward, has_scope,
        jobject,
        wchar_t, true, false, false, is_const, is_reference, false, false>{
    typedef typename std::conditional<is_const, typename std::add_const<wchar_t>::type, wchar_t>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_char(env, in);
    }
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast jobject to arithmetic reference");
        return qtjambi_to_char(env, in);
    }
};

template<bool forward, bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<forward, has_scope,
        jobject,
        bool, true, false, false, is_const, is_reference, false, false>{
    typedef typename std::conditional<is_const, typename std::add_const<bool>::type, bool>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_boolean(env, jboolean(in));
    }
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast jobject to arithmetic reference");
        return qtjambi_to_boolean(env, in);
    }
};

// QChar

template<bool forward, bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<forward, has_scope,
        jobject,
        QChar, false, false, false, is_const, is_reference, false, false>{
    typedef typename std::conditional<is_const, typename std::add_const<QChar>::type, QChar>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_char(env, in.unicode());
    }
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast jobject to QChar reference");
        return QChar(qtjambi_to_char(env, in));
    }
};

// QStringList

template<bool forward, bool has_scope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<forward, has_scope,
        jobject,
        QStringList, false, false, is_pointer, is_const, is_reference, false, false>{
    typedef typename std::conditional<is_const, typename std::add_const<QStringList>::type, QStringList>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_const, typename std::add_const<QList<QString>>::type, QList<QString>>::type NativeType2_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType2_c>::type, NativeType2_c>::type NativeType2_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType2_c>::type, NativeType2_c>::type NativeType2_ptr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType2_c>::type, NativeType2_cr>::type NativeType2_ptr_out;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType2_c>::type, NativeType2_cr>::type NativeType2_cast;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_c>::type NativeType_ptr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject castfwd(JNIEnv *env, NativeType_in in, const char*, QtJambiScope* scope){
        return qtjambi_jnitype_container1_cast<forward,has_scope,jobject,QList,is_pointer,is_const,is_reference,QString>::castfwd(env, in, nullptr, scope);
    }
    static NativeType_out castbck(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        NativeType2_ptr_out result = qtjambi_jnitype_container1_cast<forward,has_scope,jobject,QList,is_pointer,is_const,is_reference,QString>::castbck(env, in, nullptr, scope);
        return *reinterpret_cast<NativeType_ptr*>(&result);
    }
};

}// namespace QtJambiPrivate

#endif // QTJAMBI_CAST_JNITYPE_P_H
