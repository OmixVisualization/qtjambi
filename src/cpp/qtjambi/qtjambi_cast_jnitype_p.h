/****************************************************************************
**
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

#ifndef QTJAMBI_CAST_JNITYPE_P_H
#define QTJAMBI_CAST_JNITYPE_P_H

#include "qtjambi_cast_util_p.h"
#include "qtjambi_cast_container1_p.h"
#include "qtjambi_cast_container2_p.h"
#include "qtjambi_cast_container3_p.h"
#include "qtjambi_cast_container4_p.h"
#include "qtjambi_cast_container5_p.h"

namespace QtJambiPrivate {

template<bool forward, bool has_scope,
         typename JniType,
         typename NativeType, bool is_arithmetic, bool is_enum, bool is_pointer, bool is_const, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jnitype_caster{
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
};

template<bool forward, bool has_scope,
         typename JniType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         int parameterCount, typename... Ts>
struct qtjambi_jnitype_container_cast_decider{
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
};

template<bool forward, bool has_scope, typename JniType, template<typename T> class Container, bool is_pointer, bool is_const, bool is_reference, typename T>
constexpr qtjambi_jnitype_container1_cast<forward, has_scope, JniType, Container, is_pointer, is_const, is_reference, T> qtjambi_jnitype_container1_supertype(){ return {}; }

template<bool forward, bool has_scope,
         typename JniType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename... Ts>
struct qtjambi_jnitype_container_cast_decider<forward, has_scope, JniType, NativeType, is_pointer, is_const, is_reference, 1, Ts...>
    : decltype(qtjambi_jnitype_container1_supertype<forward, has_scope, JniType, NativeType, is_pointer, is_const, is_reference, Ts...>()){};

template<bool forward, bool has_scope, typename JniType, template<typename K, typename T> class Container, bool is_pointer, bool is_const, bool is_reference, typename K, typename T>
constexpr qtjambi_jnitype_container2_cast<forward, has_scope, JniType, Container, is_pointer, is_const, is_reference, K, T> qtjambi_jnitype_container2_supertype(){ return {};}

template<bool forward, bool has_scope,
         typename JniType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename... Ts>
struct qtjambi_jnitype_container_cast_decider<forward, has_scope, JniType, NativeType, is_pointer, is_const, is_reference, 2, Ts...>
        : decltype(qtjambi_jnitype_container2_supertype<forward, has_scope, JniType, NativeType, is_pointer, is_const, is_reference, Ts...>()){};

template<bool forward, bool has_scope, typename JniType, template<typename K, typename T, typename A> class Container, bool is_pointer, bool is_const, bool is_reference, typename K, typename T, typename A>
constexpr qtjambi_jnitype_container3_cast<forward, has_scope, JniType, Container, is_pointer, is_const, is_reference, K, T, A> qtjambi_jnitype_container3_supertype(){ return {};}

template<bool forward, bool has_scope,
         typename JniType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename... Ts>
struct qtjambi_jnitype_container_cast_decider<forward, has_scope, JniType, NativeType, is_pointer, is_const, is_reference, 3, Ts...>
        : decltype(qtjambi_jnitype_container3_supertype<forward, has_scope, JniType, NativeType, is_pointer, is_const, is_reference, Ts...>()){};

template<bool forward, bool has_scope, typename JniType, template<typename K, typename T, typename A, typename B> class Container, bool is_pointer, bool is_const, bool is_reference, typename K, typename T, typename A, typename B>
constexpr qtjambi_jnitype_container4_cast<forward, has_scope, JniType, Container, is_pointer, is_const, is_reference, K, T, A, B> qtjambi_jnitype_container4_supertype(){ return {};}

template<bool forward, bool has_scope,
         typename JniType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename... Ts>
struct qtjambi_jnitype_container_cast_decider<forward, has_scope, JniType, NativeType, is_pointer, is_const, is_reference, 4, Ts...>
        : decltype(qtjambi_jnitype_container4_supertype<forward, has_scope, JniType, NativeType, is_pointer, is_const, is_reference, Ts...>()){};

template<bool forward, bool has_scope, typename JniType, template<typename K, typename T, typename A, typename B, typename C> class Container, bool is_pointer, bool is_const, bool is_reference, typename K, typename T, typename A, typename B, typename C>
constexpr qtjambi_jnitype_container5_cast<forward, has_scope, JniType, Container, is_pointer, is_const, is_reference, K, T, A, B, C> qtjambi_jnitype_container5_supertype(){ return {};}

template<bool forward, bool has_scope,
         typename JniType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename... Ts>
struct qtjambi_jnitype_container_cast_decider<forward, has_scope, JniType, NativeType, is_pointer, is_const, is_reference, 5, Ts...>
        : decltype(qtjambi_jnitype_container5_supertype<forward, has_scope, JniType, NativeType, is_pointer, is_const, is_reference, Ts...>()){};

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

template<bool has_scope,
         typename NativeType, bool is_const, bool is_reference, bool is_template>
struct qtjambi_jnitype_caster<true, has_scope,
        jobject,
        NativeType, false, false, true, is_const, is_reference, true, is_template>{
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_in;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char* nativeTypeName, QtJambiScope*){
        jobject result = nullptr;
        if(!qtjambi_convert_from_native(env, typeid(NativeType_ptr), nativeTypeName, &in, true, false, result)){
           JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast to type %1").arg(QLatin1String(nativeTypeName ? nativeTypeName : qtjambi_type_name(typeid(NativeType)).data())) QTJAMBI_STACKTRACEINFO );
        }
        return result;
    }
};

template<bool has_scope,
         typename NativeType, bool is_const, bool is_reference, bool is_template>
struct qtjambi_jnitype_caster<false, has_scope,
        jobject,
        NativeType, false, false, true, is_const, is_reference, true, is_template>{
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_in;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to function pointer reference");

    static NativeType_out cast(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope*){
        NativeType_ptr fct = nullptr;
        NativeType_ptr* result = &fct;
        if(!qtjambi_convert_to_native(env, typeid(NativeType_ptr), nativeTypeName, in, &result)){
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? qtjambi_object_class_name(env, in) : QStringLiteral("null")).arg(QLatin1String(qtjambi_type_name(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
        }
        return result ? *result : nullptr;
    }
};

//template for any QObject(&/*) and non-QObject

template<bool forward, bool has_scope,
         typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_q_object, bool is_polymorphic>
struct qtjambi_jnitype_qobject_decider_cast{
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
};

template<bool forward, bool has_scope,
         typename NativeType, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<forward, has_scope,
        jobject,
        NativeType, false, false, is_pointer, is_const, is_reference, false, false>
        : qtjambi_jnitype_qobject_decider_cast<forward, has_scope, NativeType, is_pointer, is_const, is_reference,
#ifdef QOBJECT_H
                    std::is_base_of<QObject, NativeType>::value,
#else
                    false,
#endif
        std::is_polymorphic<NativeType>::value>
{
};

//template for QObject*

template<bool has_scope,
         typename NativeType, bool is_const, bool is_reference>
struct qtjambi_jnitype_qobject_decider_cast<true, has_scope, NativeType, true, is_const, is_reference, true, true>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_in;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_out;
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;
    constexpr static jobject cast(JNIEnv * env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_qobject(env, in, typeid(NativeType));
    }
};

template<bool has_scope,
         typename NativeType, bool is_const, bool is_reference>
struct qtjambi_jnitype_qobject_decider_cast<false, has_scope, NativeType, true, is_const, is_reference, true, true>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_in;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_out;
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;
    static NativeType_out cast(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope*){
        NativeType_ptr result = nullptr;
        if(!qtjambi_convert_to_native(env, typeid(NativeType), nativeTypeName, in, &result)){
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? qtjambi_object_class_name(env, in) : QStringLiteral("null")).arg(QLatin1String(qtjambi_type_name(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
        }
        return result;
    }
};

//template for QObject&

template<bool has_scope,
         typename NativeType, bool is_const>
struct qtjambi_jnitype_qobject_decider_cast<true, has_scope, NativeType, false, is_const, true, true, true>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_out;
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;
    constexpr static jobject cast(JNIEnv * env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_qobject(env, &in, typeid(NativeType));
    }
};

template<bool has_scope,
         typename NativeType, bool is_const>
struct qtjambi_jnitype_qobject_decider_cast<false, has_scope, NativeType, false, is_const, true, true, true>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_out;
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;
    static NativeType_out cast(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope*){
        NativeType_ptr result = nullptr;
        if(!qtjambi_convert_to_native(env, typeid(NativeType), nativeTypeName, in, &result)){
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? qtjambi_object_class_name(env, in) : QStringLiteral("null")).arg(QLatin1String(qtjambi_type_name(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
        }
        if(!result)
            JavaException::raiseNullPointerException(env, QStringLiteral("Cannot cast null to reference type %1").arg(QLatin1String(qtjambi_type_name(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
        return *result;
    }
};

//template for any non-QObject pointer type (e.g. QGraphicsItem*)

template<bool has_scope,
         typename NativeType, bool is_const, bool is_reference>
struct qtjambi_jnitype_qobject_decider_cast<true, has_scope, NativeType, true, is_const, is_reference, false, false>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_in;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_out;
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;
    static jobject cast(JNIEnv * env, NativeType_in in, const char*, QtJambiScope* scope){
        jobject o = qtjambi_from_object(env, in, typeid(NativeType), false, false);
        if(scope)
            scope->addObjectInvalidation(env, o, true, false);
        return o;
    }
};

template<bool has_scope,
         typename NativeType, bool is_const, bool is_reference>
struct qtjambi_jnitype_qobject_decider_cast<true, has_scope, NativeType, true, is_const, is_reference, false, true>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_in;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_out;
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;
    static jobject cast(JNIEnv * env, NativeType_in in, const char*, QtJambiScope* scope){
        jobject o = QtJambiShellInterface::getJavaObjectLocalRef(env, dynamic_cast<const QtJambiShellInterface*>(in));
        if(!o){
            o = qtjambi_from_object(env, in, typeid(NativeType), false, false);
            if(scope)
                scope->addObjectInvalidation(env, o, true, false);
        }
        return o;
    }
};

template<bool has_scope,
         typename NativeType, bool is_const, bool is_reference, bool is_polymorphic>
struct qtjambi_jnitype_qobject_decider_cast<false, has_scope, NativeType, true, is_const, is_reference, false, is_polymorphic>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_in;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_out;
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;
    static NativeType_out cast(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope*){
        NativeType_ptr result = nullptr;
        if(!qtjambi_convert_to_native(env, typeid(NativeType), nativeTypeName, in, &result)){
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? qtjambi_object_class_name(env, in) : QStringLiteral("null")).arg(QLatin1String(qtjambi_type_name(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
        }
        return result;
    }
};

//template for any non-QObject reference type

template<bool has_scope,
         typename NativeType, bool is_const, bool is_reference, bool is_copy_assignable>
struct qtjambi_jnitype_copy_assignable_decider_cast{
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
};

template<bool has_scope,
         typename NativeType, bool is_const, bool is_reference, bool is_polymorphic>
struct qtjambi_jnitype_qobject_decider_cast<true, has_scope, NativeType, false, is_const, is_reference, false, is_polymorphic>
{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<typename std::add_const<NativeType>::type>::type NativeType_in;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_out;
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;
    constexpr static jobject cast(JNIEnv * env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_object(env, &in, typeid(NativeType), true, false);
    }
};

template<bool has_scope,
         bool is_const, bool is_reference>
struct qtjambi_jnitype_qobject_decider_cast<true, has_scope, QVariant, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QVariant>::type, QVariant>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<typename std::add_const<QVariant>::type>::type NativeType_in;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_out;
    typedef typename std::add_pointer<QVariant>::type NativeType_ptr;
    constexpr static jobject cast(JNIEnv * env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_qvariant(env, in);
    }
};

template<bool has_scope,
         typename NativeType, bool is_const, bool is_reference, bool is_polymorphic>
struct qtjambi_jnitype_qobject_decider_cast<false, has_scope, NativeType, false, is_const, is_reference, false, is_polymorphic>
        : qtjambi_jnitype_copy_assignable_decider_cast<has_scope, NativeType, is_const, is_reference, std::is_copy_assignable<NativeType>::value>
{
};

template<bool has_scope,
         typename NativeType, bool is_const, bool is_reference, bool dont_use_pointer>
struct qtjambi_jnitype_use_pointer_decider_cast{
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
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
    static NativeType_cr cast(JNIEnv * env, jobject in, const char* nativeTypeName, QtJambiScope*){
        NativeType* result = nullptr;
        if(!qtjambi_convert_to_native(env, typeid(NativeType), nativeTypeName, in, &result)){
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? qtjambi_object_class_name(env, in) : QStringLiteral("null")).arg(QLatin1String(qtjambi_type_name(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
        }
        return checked_deref<NativeType>(env, result);
    }
};

template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_use_pointer_decider_cast<has_scope, QVariant, is_const, is_reference, true /*dont_use_pointer*/>{
    typedef typename std::conditional<is_const, typename std::add_const<QVariant>::type, QVariant>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QVariant reference");

    static NativeType_cr cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        return qtjambi_to_qvariant(env, in);
    }
};

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#ifdef QABSTRACTITEMMODEL_H
template<bool is_const, bool is_reference>
struct qtjambi_jnitype_use_pointer_decider_cast<true, QModelRoleDataSpan, is_const, is_reference, true /*dont_use_pointer*/>{
    typedef typename std::conditional<is_const, typename std::add_const<QModelRoleDataSpan>::type, QVariant>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QVariant reference");

    static QModelRoleDataSpan cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        void *data(nullptr);
        qsizetype length(0);
        qtjambi_to_QModelRoleData(env, *scope, in, data, length);
        return QModelRoleDataSpan(reinterpret_cast<QModelRoleData*>(data), length);
    }
};
#endif
#endif

template<bool has_scope,
         typename NativeType, bool is_const, bool is_reference>
struct qtjambi_jnitype_use_pointer_decider_cast<has_scope, NativeType, is_const, is_reference, true /*dont_use_pointer*/>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to value reference");

    static NativeType_cr cast(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope*){
        NativeType result;
        if(!qtjambi_convert_to_native(env, typeid(NativeType), nativeTypeName, in, &result)){
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? qtjambi_object_class_name(env, in) : QStringLiteral("null")).arg(QLatin1String(qtjambi_type_name(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
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

    static NativeType_cr cast(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope*){
        NativeType_ptr result = nullptr;
        if(!qtjambi_convert_to_native(env, typeid(NativeType), nativeTypeName, in, &result)){
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? qtjambi_object_class_name(env, in) : QStringLiteral("null")).arg(QLatin1String(qtjambi_type_name(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
        }
        return qtjambi_deref_value<NativeType, supports_StandardConstructor<NativeType>::value, supports_CopyConstructor<NativeType>::value, is_const, is_reference>::deref(env, result);
    }
};

// template for jstring

template<bool has_scope,
         typename NativeType, bool is_arithmetic, bool is_enum, bool is_pointer, bool is_const, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jstring_caster{
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_const, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QString, is_arithmetic, is_enum, false, is_const, false, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QString>::type, QString>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jstring in, const char* nativeTypeName, QtJambiScope*){
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
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QString & without scope.");
    Q_STATIC_ASSERT_X(!is_pointer || has_scope, "Cannot cast to QString* without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char* nativeTypeName, QtJambiScope* scope){
        if((is_reference || is_pointer) && !scope)
            JavaException::raiseError(env, "Cannot cast to QString pointer or reference without scope." QTJAMBI_STACKTRACEINFO );
        QString* result = new QString();
        if(!qtjambi_convert_to_native(env, typeid(QString), nativeTypeName, in, result)){
            JavaException::raiseRuntimeException(env, "Cannot cast to type QString" QTJAMBI_STACKTRACEINFO );
        }
        scope->addDeletion(result);
        return deref_ptr<is_pointer,typename std::conditional<is_pointer, QString*, QString>::type>::deref(result);
    }
};

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_const, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QStringRef, is_arithmetic, is_enum, false, is_const, is_reference, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QStringRef>::type, QStringRef>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QStringRef &.");
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QStringRef without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char* nativeTypeName, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QStringRef without scope." QTJAMBI_STACKTRACEINFO );
        QString *result = new QString();
        if(!qtjambi_convert_to_native(env, typeid(QString), nativeTypeName, in, result)){
            delete result;
            JavaException::raiseRuntimeException(env, "Cannot cast to type QStringRef" QTJAMBI_STACKTRACEINFO );
        }
        scope->addDeletion(result);
        return QStringRef(result);
    }
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_const, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QStringRef, is_arithmetic, is_enum, true, is_const, false, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QStringRef>::type, QStringRef>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QStringRef without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char* nativeTypeName, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QStringRef without scope." QTJAMBI_STACKTRACEINFO );
        QString *result = new QString();
        if(!qtjambi_convert_to_native(env, typeid(QString), nativeTypeName, in, result)){
            delete result;
            JavaException::raiseRuntimeException(env, "Cannot cast to type QStringRef" QTJAMBI_STACKTRACEINFO );
        }
        QStringRef* ref = new QStringRef(result);
        scope->addDeletion(result);
        scope->addDeletion(ref);
        return ref;
    }
};

#else

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_const, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QAnyStringView, is_arithmetic, is_enum, false, is_const, is_reference, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QAnyStringView>::type, QAnyStringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QAnyStringView &.");
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QAnyStringView without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QAnyStringView without scope." QTJAMBI_STACKTRACEINFO );
        JString2QChars* buffer = new JString2QChars(env, in);
        scope->addDeletion(buffer);
        return buffer->toAnyStringView();
    }
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_const, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QAnyStringView, is_arithmetic, is_enum, true, is_const, false, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QAnyStringView>::type, QAnyStringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QStringView without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QAnyStringView without scope." QTJAMBI_STACKTRACEINFO );
        JString2QChars* buffer = new JString2QChars(env, in);
        QAnyStringView* ref = new QAnyStringView(buffer->data(), buffer->length());
        scope->addDeletion(ref);
        scope->addDeletion(buffer);
        return ref;
    }
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_const, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QUtf8StringView, is_arithmetic, is_enum, false, is_const, is_reference, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QUtf8StringView>::type, QUtf8StringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QUtf8StringView &.");
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QUtf8StringView without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QUtf8StringView without scope." QTJAMBI_STACKTRACEINFO );
        J2CStringBuffer* buffer = new J2CStringBuffer(env, in);
        scope->addDeletion(buffer);
        return buffer->toLatin1String();
    }
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_const, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QUtf8StringView, is_arithmetic, is_enum, true, is_const, false, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QUtf8StringView>::type, QUtf8StringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QUtf8StringView without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QUtf8StringView without scope." QTJAMBI_STACKTRACEINFO );
        J2CStringBuffer* buffer = new J2CStringBuffer(env, in);
        QUtf8StringView* ref = new QUtf8StringView(buffer->data(), buffer->length());
        scope->addDeletion(ref);
        scope->addDeletion(buffer);
        return ref;
    }
};

#endif

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_const, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QStringView, is_arithmetic, is_enum, false, is_const, is_reference, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QStringView>::type, QStringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QStringView &.");
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QStringView without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QStringView without scope." QTJAMBI_STACKTRACEINFO );
        JString2QChars* buffer = new JString2QChars(env, in);
        scope->addDeletion(buffer);
        return buffer->toStringView();
    }
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_const, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QStringView, is_arithmetic, is_enum, true, is_const, false, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QStringView>::type, QStringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QStringView without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QStringView without scope." QTJAMBI_STACKTRACEINFO );
        JString2QChars* buffer = new JString2QChars(env, in);
        QStringView* ref = new QStringView(buffer->data(), buffer->length());
        scope->addDeletion(ref);
        scope->addDeletion(buffer);
        return ref;
    }
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_const, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QLatin1String, is_arithmetic, is_enum, false, is_const, is_reference, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1String>::type, QLatin1String>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QLatin1String &.");
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QLatin1String without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QLatin1String without scope." QTJAMBI_STACKTRACEINFO );
        J2CStringBuffer* buffer = new J2CStringBuffer(env, in);
        scope->addDeletion(buffer);
        return buffer->toLatin1String();
    }
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_const, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QLatin1String, is_arithmetic, is_enum, true, is_const, false, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1String>::type, QLatin1String>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QLatin1String without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QLatin1String without scope." QTJAMBI_STACKTRACEINFO );
        J2CStringBuffer* buffer = new J2CStringBuffer(env, in);
        QLatin1String* ref = new QLatin1String(buffer->data(), buffer->length());
        scope->addDeletion(ref);
        scope->addDeletion(buffer);
        return ref;
    }
};

#if QT_VERSION >= QT_VERSION_CHECK(7, 0, 0)
template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_const, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QLatin1StringView, is_arithmetic, is_enum, false, is_const, is_reference, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1StringView>::type, QLatin1StringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QLatin1StringView &.");
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QLatin1StringView without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QLatin1StringView without scope." QTJAMBI_STACKTRACEINFO );
        J2CStringBuffer* buffer = new J2CStringBuffer(env, in);
        scope->addDeletion(buffer);
        return buffer->toLatin1StringView();
    }
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_const, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QLatin1StringView, is_arithmetic, is_enum, true, is_const, false, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1StringView>::type, QLatin1StringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QLatin1StringView without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QLatin1StringView without scope." QTJAMBI_STACKTRACEINFO );
        J2CStringBuffer* buffer = new J2CStringBuffer(env, in);
        QLatin1StringView* ref = new QLatin1StringView(buffer->data(), buffer->length());
        scope->addDeletion(ref);
        scope->addDeletion(buffer);
        return ref;
    }
};
#endif

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
template<bool has_scope,
         bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jobject,
        QStringRef, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringRef>::type, QStringRef>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_qstringref(env, in);
    }
};

template<bool has_scope,
         bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<false, has_scope,
        jobject,
        QStringRef, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringRef>::type, QStringRef>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QStringRef &.");
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QStringRef without scope.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QStringRef without scope." QTJAMBI_STACKTRACEINFO );
        QString *result = new QString();
        if(!qtjambi_convert_to_native(env, typeid(QString), nativeTypeName, in, result)){
            delete result;
            JavaException::raiseRuntimeException(env, "Cannot cast to type QStringRef" QTJAMBI_STACKTRACEINFO );
        }
        scope->addDeletion(result);
        return QStringRef(result);
    }
};

template<bool has_scope,
         bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jstring,
        QStringRef, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringRef>::type, QStringRef>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static jstring cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_qstringref(env, in);
    }
};

template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<false, has_scope,
        jstring,
        QStringRef, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringRef>::type, QStringRef>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QStringRef &.");
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QStringRef without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char* nativeTypeName, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QStringRef without scope." QTJAMBI_STACKTRACEINFO );
        QString *result = new QString();
        if(!qtjambi_convert_to_native(env, typeid(QString), nativeTypeName, in, result)){
            delete result;
            JavaException::raiseRuntimeException(env, "Cannot cast to type QStringRef" QTJAMBI_STACKTRACEINFO );
        }
        scope->addDeletion(result);
        return QStringRef(result);
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_caster<true, has_scope,
        jobject,
        QStringRef, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringRef>::type, QStringRef>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return in ? qtjambi_from_qstringref(env, *in) : nullptr;
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_caster<false, has_scope,
        jobject,
        QStringRef, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringRef>::type, QStringRef>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QStringRef without scope.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QStringRef without scope." QTJAMBI_STACKTRACEINFO );
        QString *result = new QString();
        if(!qtjambi_convert_to_native(env, typeid(QString), nativeTypeName, in, result)){
            delete result;
            JavaException::raiseRuntimeException(env, "Cannot cast to type QStringRef" QTJAMBI_STACKTRACEINFO );
        }
        QStringRef* ref = new QStringRef(result);
        scope->addDeletion(ref);
        scope->addDeletion(result);
        return ref;
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_caster<true, has_scope,
        jstring,
        QStringRef, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringRef>::type, QStringRef>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jstring cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return in ? qtjambi_from_qstringref(env, *in) : nullptr;
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_caster<false, has_scope,
        jstring,
        QStringRef, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringRef>::type, QStringRef>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QStringRef without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char* nativeTypeName, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QStringRef without scope." QTJAMBI_STACKTRACEINFO );
        QString *result = new QString();
        if(!qtjambi_convert_to_native(env, typeid(QString), nativeTypeName, in, result)){
            delete result;
            JavaException::raiseRuntimeException(env, "Cannot cast to type QStringRef" QTJAMBI_STACKTRACEINFO );
        }
        QStringRef* ref = new QStringRef(result);
        scope->addDeletion(ref);
        scope->addDeletion(result);
        return ref;
    }
};

#else

template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jstring,
        QAnyStringView, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QAnyStringView>::type, QAnyStringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static jstring cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_qanystringview(env, in);
    }
};

template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<false, has_scope,
        jstring,
        QAnyStringView, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QAnyStringView>::type, QAnyStringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QAnyStringView &.");
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QAnyStringView without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QAnyStringView without scope." QTJAMBI_STACKTRACEINFO );
        JString2QChars* buffer = new JString2QChars(env, in);
        scope->addDeletion(buffer);
        return buffer->toAnyStringView();
    }
};

template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jobject,
        QAnyStringView, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QAnyStringView>::type, QAnyStringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_qanystringview(env, in);
    }
};

template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<false, has_scope,
        jobject,
        QAnyStringView, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QAnyStringView>::type, QAnyStringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QAnyStringView &.");
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QAnyStringView without scope.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QAnyStringView without scope." QTJAMBI_STACKTRACEINFO );
        jstring _in = qtjambi_to_jstring(env, in);
        JString2QChars* buffer = new JString2QChars(env, _in);
        scope->addDeletion(buffer);
        return buffer->toAnyStringView();
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_caster<true, has_scope,
        jstring,
        QAnyStringView, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QAnyStringView>::type, QAnyStringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jstring cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return in ? qtjambi_from_qanystringview(env, *in) : nullptr;
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_caster<false, has_scope,
        jstring,
        QAnyStringView, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QAnyStringView>::type, QAnyStringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QAnyStringView without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QAnyStringView without scope." QTJAMBI_STACKTRACEINFO );
        JString2QChars* buffer = new JString2QChars(env, in);
        QAnyStringView* ref = new QAnyStringView(buffer->constData(), buffer->length());
        scope->addDeletion(ref);
        scope->addDeletion(buffer);
        return ref;
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_caster<true, has_scope,
        jobject,
        QAnyStringView, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QAnyStringView>::type, QAnyStringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return in ? qtjambi_from_qanystringview(env, *in) : nullptr;
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_caster<false, has_scope,
        jobject,
        QAnyStringView, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QAnyStringView>::type, QAnyStringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QAnyStringView without scope.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QAnyStringView without scope." QTJAMBI_STACKTRACEINFO );
        jstring _in = qtjambi_to_jstring(env, in);
        JString2QChars* buffer = new JString2QChars(env, _in);
        QAnyStringView* ref = new QAnyStringView(buffer->constData(), buffer->length());
        scope->addDeletion(ref);
        scope->addDeletion(buffer);
        return ref;
    }
};

template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jstring,
        QUtf8StringView, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QUtf8StringView>::type, QUtf8StringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static jstring cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_qutf8stringview(env, in);
    }
};

template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<false, has_scope,
        jstring,
        QUtf8StringView, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QUtf8StringView>::type, QUtf8StringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QUtf8StringView &.");
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QUtf8StringView without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QUtf8StringView without scope." QTJAMBI_STACKTRACEINFO );
        J2CStringBuffer* buffer = new J2CStringBuffer(env, in);
        scope->addDeletion(buffer);
        return buffer->toUtf8StringView();
    }
};

template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jobject,
        QUtf8StringView, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QUtf8StringView>::type, QUtf8StringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_qutf8stringview(env, in);
    }
};

template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<false, has_scope,
        jobject,
        QUtf8StringView, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QUtf8StringView>::type, QUtf8StringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QUtf8StringView &.");
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QUtf8StringView without scope.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QUtf8StringView without scope." QTJAMBI_STACKTRACEINFO );
        jstring _in = qtjambi_to_jstring(env, in);
        J2CStringBuffer* buffer = new J2CStringBuffer(env, _in);
        scope->addDeletion(buffer);
        return buffer->toUtf8StringView();
    }
};

template<bool has_scope,
         bool is_const>
struct qtjambi_jnitype_caster<true, has_scope,
        jstring,
        QUtf8StringView, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QUtf8StringView>::type, QUtf8StringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jstring cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return in ? qtjambi_from_qutf8stringview(env, *in) : nullptr;
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_caster<false, has_scope,
        jstring,
        QUtf8StringView, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QUtf8StringView>::type, QUtf8StringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QUtf8StringView without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QUtf8StringView without scope." QTJAMBI_STACKTRACEINFO );
        J2CStringBuffer* buffer = new J2CStringBuffer(env, in);
        QUtf8StringView* ref = new QUtf8StringView(buffer->data(), buffer->length());
        scope->addDeletion(ref);
        scope->addDeletion(buffer);
        return ref;
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_caster<true, has_scope,
        jobject,
        QUtf8StringView, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QUtf8StringView>::type, QUtf8StringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return in ? qtjambi_from_qutf8stringview(env, *in) : nullptr;
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_caster<false, has_scope,
        jobject,
        QUtf8StringView, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QUtf8StringView>::type, QUtf8StringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QUtf8StringView without scope.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QUtf8StringView without scope." QTJAMBI_STACKTRACEINFO );
        jstring _in = qtjambi_to_jstring(env, in);
        J2CStringBuffer* buffer = new J2CStringBuffer(env, _in);
        QUtf8StringView* ref = new QUtf8StringView(buffer->data(), buffer->length());
        scope->addDeletion(ref);
        scope->addDeletion(buffer);
        return ref;
    }
};

#endif

template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jstring,
        QStringView, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringView>::type, QStringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static jstring cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_qstringview(env, in);
    }
};

template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<false, has_scope,
        jstring,
        QStringView, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringView>::type, QStringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QStringView &.");
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QStringView without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QStringView without scope." QTJAMBI_STACKTRACEINFO );
        JString2QChars* buffer = new JString2QChars(env, in);
        scope->addDeletion(buffer);
        return buffer->toStringView();
    }
};

template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jobject,
        QStringView, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringView>::type, QStringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_qstringview(env, in);
    }
};

template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<false, has_scope,
        jobject,
        QStringView, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringView>::type, QStringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QStringView &.");
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QStringView without scope.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QStringView without scope." QTJAMBI_STACKTRACEINFO );
        jstring _in = qtjambi_to_jstring(env, in);
        JString2QChars* buffer = new JString2QChars(env, _in);
        scope->addDeletion(buffer);
        return buffer->toStringView();
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_caster<true, has_scope,
        jstring,
        QStringView, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringView>::type, QStringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jstring cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return in ? qtjambi_from_qstringview(env, *in) : nullptr;
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_caster<false, has_scope,
        jstring,
        QStringView, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringView>::type, QStringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QStringView without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QStringView without scope." QTJAMBI_STACKTRACEINFO );
        JString2QChars* buffer = new JString2QChars(env, in);
        QStringView* ref = new QStringView(buffer->constData(), buffer->length());
        scope->addDeletion(ref);
        scope->addDeletion(buffer);
        return ref;
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_caster<true, has_scope,
        jobject,
        QStringView, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringView>::type, QStringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return in ? qtjambi_from_qstringview(env, *in) : nullptr;
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_caster<false, has_scope,
        jobject,
        QStringView, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringView>::type, QStringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QStringView without scope.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QStringView without scope." QTJAMBI_STACKTRACEINFO );
        jstring _in = qtjambi_to_jstring(env, in);
        JString2QChars* buffer = new JString2QChars(env, _in);
        QStringView* ref = new QStringView(buffer->constData(), buffer->length());
        scope->addDeletion(ref);
        scope->addDeletion(buffer);
        return ref;
    }
};

template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jstring,
        QLatin1String, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1String>::type, QLatin1String>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static jstring cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_qstring(env, in);
    }
};

template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<false, has_scope,
        jstring,
        QLatin1String, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1String>::type, QLatin1String>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QLatin1String &.");
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QLatin1String without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QLatin1String without scope." QTJAMBI_STACKTRACEINFO );
        J2CStringBuffer* buffer = new J2CStringBuffer(env, in);
        scope->addDeletion(buffer);
        return buffer->toLatin1String();
    }
};

template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jobject,
        QLatin1String, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1String>::type, QLatin1String>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_qstring(env, in);
    }
};

template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<false, has_scope,
        jobject,
        QLatin1String, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1String>::type, QLatin1String>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QLatin1String &.");
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QLatin1String without scope.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QLatin1String without scope." QTJAMBI_STACKTRACEINFO );
        jstring _in = qtjambi_to_jstring(env, in);
        J2CStringBuffer* buffer = new J2CStringBuffer(env, _in);
        scope->addDeletion(buffer);
        return buffer->toLatin1String();
    }
};

template<bool has_scope,
         bool is_const>
struct qtjambi_jnitype_caster<true, has_scope,
        jstring,
        QLatin1String, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1String>::type, QLatin1String>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jstring cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return in ? qtjambi_from_qstring(env, *in) : nullptr;
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_caster<false, has_scope,
        jstring,
        QLatin1String, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1String>::type, QLatin1String>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QLatin1String without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QLatin1String without scope." QTJAMBI_STACKTRACEINFO );
        J2CStringBuffer* buffer = new J2CStringBuffer(env, in);
        QLatin1String* ref = new QLatin1String(buffer->data(), buffer->length());
        scope->addDeletion(ref);
        scope->addDeletion(buffer);
        return ref;
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_caster<true, has_scope,
        jobject,
        QLatin1String, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1String>::type, QLatin1String>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return in ? qtjambi_from_qstring(env, *in) : nullptr;
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_caster<false, has_scope,
        jobject,
        QLatin1String, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1String>::type, QLatin1String>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QLatin1String without scope.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QLatin1String without scope." QTJAMBI_STACKTRACEINFO );
        jstring _in = qtjambi_to_jstring(env, in);
        J2CStringBuffer* buffer = new J2CStringBuffer(env, _in);
        QLatin1String* ref = new QLatin1String(buffer->data(), buffer->length());
        scope->addDeletion(ref);
        scope->addDeletion(buffer);
        return ref;
    }
};

#if QT_VERSION >= QT_VERSION_CHECK(7,0,0)
template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jstring,
        QLatin1StringView, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1StringView>::type, QLatin1StringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static jstring cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_qstring(env, in);
    }
};

template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<false, has_scope,
        jstring,
        QLatin1StringView, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1StringView>::type, QLatin1StringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QLatin1StringView &.");
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QLatin1StringView without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QLatin1StringView without scope." QTJAMBI_STACKTRACEINFO );
        J2CStringBuffer* buffer = new J2CStringBuffer(env, in);
        scope->addDeletion(buffer);
        return buffer->toLatin1String();
    }
};

template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jobject,
        QLatin1StringView, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1StringView>::type, QLatin1StringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_qstring(env, in);
    }
};

template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<false, has_scope,
        jobject,
        QLatin1StringView, false, false, false, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1StringView>::type, QLatin1StringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QLatin1StringView &.");
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QLatin1StringView without scope.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QLatin1StringView without scope." QTJAMBI_STACKTRACEINFO );
        jstring _in = qtjambi_to_jstring(env, in);
        J2CStringBuffer* buffer = new J2CStringBuffer(env, _in);
        scope->addDeletion(buffer);
        return buffer->toLatin1String();
    }
};

template<bool has_scope,
         bool is_const>
struct qtjambi_jnitype_caster<true, has_scope,
        jstring,
        QLatin1StringView, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1StringView>::type, QLatin1StringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jstring cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return in ? qtjambi_from_qstring(env, *in) : nullptr;
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_caster<false, has_scope,
        jstring,
        QLatin1StringView, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1StringView>::type, QLatin1StringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QLatin1StringView without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QLatin1StringView without scope." QTJAMBI_STACKTRACEINFO );
        J2CStringBuffer* buffer = new J2CStringBuffer(env, in);
        QLatin1StringView* ref = new QLatin1StringView(buffer->data(), buffer->length());
        scope->addDeletion(ref);
        scope->addDeletion(buffer);
        return ref;
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_caster<true, has_scope,
        jobject,
        QLatin1StringView, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1StringView>::type, QLatin1StringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return in ? qtjambi_from_qstring(env, *in) : nullptr;
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_caster<false, has_scope,
        jobject,
        QLatin1StringView, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1StringView>::type, QLatin1StringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QLatin1StringView without scope.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QLatin1StringView without scope." QTJAMBI_STACKTRACEINFO );
        jstring _in = qtjambi_to_jstring(env, in);
        J2CStringBuffer* buffer = new J2CStringBuffer(env, _in);
        QLatin1StringView* ref = new QLatin1StringView(buffer->data(), buffer->length());
        scope->addDeletion(ref);
        scope->addDeletion(buffer);
        return ref;
    }
};
#endif

template<bool has_scope,
         bool is_const, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jnitype_caster<true, has_scope, jstring, _jobject, false, false, true, is_const, is_reference, is_function, is_template>
{
    constexpr static jstring cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        return qtjambi_to_jstring(env, in);
    }
};

template<bool has_scope,
         typename NativeType, bool is_arithmetic, bool is_enum, bool is_pointer, bool is_const, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jnitype_caster<true, has_scope, jstring, NativeType, is_arithmetic, is_enum, is_pointer, is_const, is_reference, is_function, is_template>
{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static jstring cast(JNIEnv *env, NativeType_in in, const char* nativeTypeName, QtJambiScope* scope){
        return qtjambi_to_jstring(env, qtjambi_scoped_cast<has_scope,jobject,NativeType_in>::cast(env, in, nativeTypeName, scope));
    }
};

template<bool has_scope,
         typename NativeType, bool is_arithmetic, bool is_enum, bool is_pointer, bool is_const, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jnitype_caster<false, has_scope, jstring, NativeType, is_arithmetic, is_enum, is_pointer, is_const, is_reference, is_function, is_template>
        : qtjambi_jstring_caster<has_scope, NativeType, is_arithmetic, is_enum, is_pointer, is_const, is_reference, is_function, is_template>{};

// enums

template<bool has_scope,
         typename NativeType, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jobject,
        NativeType, false, true, false, is_const, is_reference, false, false>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    constexpr static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_object(env, &in, typeid(NativeType), false, false);
    }
};

template<bool has_scope,
         typename NativeType, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<false, has_scope,
        jobject,
        NativeType, false, true, false, is_const, is_reference, false, false>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast jobject to enumerator reference");

    static NativeType_out cast(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope*){
        NativeType result;
        if(!qtjambi_convert_to_native(env, typeid(NativeType), nativeTypeName, in, &result)){
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? qtjambi_object_class_name(env, in) : QStringLiteral("null")).arg(QLatin1String(qtjambi_type_name(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
        }
        return result;
    }
};

// object <=> arithmetic

template<bool forward, bool has_scope, typename NativeType, bool a_is_const, bool a_is_reference, bool a_is_integral, bool a_is_floating_point, bool a_is_signed, size_t a_size>
struct qtjambi_jnitype_arithmetic_caster{
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
};

template<bool forward, bool has_scope, typename NativeType, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<forward, has_scope,
        jobject,
        NativeType, true, false, false, is_const, is_reference, false, false>
        : qtjambi_jnitype_arithmetic_caster<
            forward,
            has_scope,
            NativeType,
            is_const,
            is_reference,
            std::is_integral<NativeType>::value,
            std::is_floating_point<NativeType>::value,
            std::is_signed<NativeType>::value,
            sizeof(NativeType)>{};

template<bool has_scope, typename NativeType, bool a_is_const, bool a_is_reference, bool a_is_signed>
struct qtjambi_jnitype_arithmetic_caster<true, has_scope,
        NativeType, a_is_const, a_is_reference, true, false, a_is_signed, sizeof(jbyte)>{
    constexpr static jobject cast(JNIEnv *env, NativeType in, const char*, QtJambiScope*){
        return qtjambi_from_byte(env, jbyte(in));
    }
};

template<bool has_scope, typename NativeType, bool a_is_const, bool a_is_reference, bool a_is_signed>
struct qtjambi_jnitype_arithmetic_caster<false, has_scope,
        NativeType, a_is_const, a_is_reference, true, false, a_is_signed, sizeof(jbyte)>{
    typedef typename std::conditional<a_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<a_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    Q_STATIC_ASSERT_X(!a_is_reference, "Cannot cast jobject to arithmetic reference");

    constexpr static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        return NativeType(qtjambi_to_byte(env, in));
    }
};

template<bool has_scope, typename NativeType, bool a_is_const, bool a_is_reference, bool a_is_signed>
struct qtjambi_jnitype_arithmetic_caster<true, has_scope,
        NativeType, a_is_const, a_is_reference, true, false, a_is_signed, sizeof(jshort)>{
    constexpr static jobject cast(JNIEnv *env, NativeType in, const char*, QtJambiScope*){
        return qtjambi_from_short(env, jshort(in));
    }
};

template<bool has_scope, typename NativeType, bool a_is_const, bool a_is_reference, bool a_is_signed>
struct qtjambi_jnitype_arithmetic_caster<false, has_scope,
        NativeType, a_is_const, a_is_reference, true, false, a_is_signed, sizeof(jshort)>{
    typedef typename std::conditional<a_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<a_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    Q_STATIC_ASSERT_X(!a_is_reference, "Cannot cast jobject to arithmetic reference");

    constexpr static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        return NativeType(qtjambi_to_short(env, in));
    }
};

template<bool has_scope, typename NativeType, bool a_is_const, bool a_is_reference, bool a_is_signed>
struct qtjambi_jnitype_arithmetic_caster<true, has_scope,
        NativeType, a_is_const, a_is_reference, true, false, a_is_signed, sizeof(jint)>{
    constexpr static jobject cast(JNIEnv *env, NativeType in, const char*, QtJambiScope*){
        return qtjambi_from_int(env, jint(in));
    }
};

template<bool has_scope, typename NativeType, bool a_is_const, bool a_is_reference, bool a_is_signed>
struct qtjambi_jnitype_arithmetic_caster<false, has_scope,
        NativeType, a_is_const, a_is_reference, true, false, a_is_signed, sizeof(jint)>{
    typedef typename std::conditional<a_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<a_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    Q_STATIC_ASSERT_X(!a_is_reference, "Cannot cast jobject to arithmetic reference");

    constexpr static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        return NativeType(qtjambi_to_int(env, in));
    }
};

template<bool has_scope, typename NativeType, bool a_is_const, bool a_is_reference, bool a_is_signed>
struct qtjambi_jnitype_arithmetic_caster<true, has_scope,
        NativeType, a_is_const, a_is_reference, true, false, a_is_signed, sizeof(jlong)>{
    constexpr static jobject cast(JNIEnv *env, NativeType in, const char*, QtJambiScope*){
        return qtjambi_from_long(env, jlong(in));
    }
};

template<bool has_scope, typename NativeType, bool a_is_const, bool a_is_reference, bool a_is_signed>
struct qtjambi_jnitype_arithmetic_caster<false, has_scope,
        NativeType, a_is_const, a_is_reference, true, false, a_is_signed, sizeof(jlong)>{
    typedef typename std::conditional<a_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<a_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    Q_STATIC_ASSERT_X(!a_is_reference, "Cannot cast jobject to arithmetic reference");

    constexpr static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        return NativeType(qtjambi_to_long(env, in));
    }
};

template<bool has_scope, typename NativeType, bool a_is_const, bool a_is_reference, bool a_is_signed>
struct qtjambi_jnitype_arithmetic_caster<true, has_scope,
        NativeType, a_is_const, a_is_reference, false, true, a_is_signed, sizeof(jdouble)>{
    constexpr static jobject cast(JNIEnv *env, NativeType in, const char*, QtJambiScope*){
        return qtjambi_from_double(env, jdouble(in));
    }
};

template<bool has_scope, typename NativeType, bool a_is_const, bool a_is_reference, bool a_is_signed>
struct qtjambi_jnitype_arithmetic_caster<false, has_scope,
        NativeType, a_is_const, a_is_reference, false, true, a_is_signed, sizeof(jdouble)>{
    typedef typename std::conditional<a_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<a_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    Q_STATIC_ASSERT_X(!a_is_reference, "Cannot cast jobject to arithmetic reference");

    constexpr static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        return NativeType(qtjambi_to_double(env, in));
    }
};

template<bool has_scope, typename NativeType, bool a_is_const, bool a_is_reference, bool a_is_signed>
struct qtjambi_jnitype_arithmetic_caster<true, has_scope,
        NativeType, a_is_const, a_is_reference, false, true, a_is_signed, sizeof(jfloat)>{
    constexpr static jobject cast(JNIEnv *env, NativeType in, const char*, QtJambiScope*){
        return qtjambi_from_float(env, jfloat(in));
    }
};

template<bool has_scope, typename NativeType, bool a_is_const, bool a_is_reference, bool a_is_signed>
struct qtjambi_jnitype_arithmetic_caster<false, has_scope,
        NativeType, a_is_const, a_is_reference, false, true, a_is_signed, sizeof(jfloat)>{
    typedef typename std::conditional<a_is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<a_is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    Q_STATIC_ASSERT_X(!a_is_reference, "Cannot cast jobject to arithmetic reference");

    constexpr static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        return NativeType(qtjambi_to_float(env, in));
    }
};

template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jobject,
        wchar_t, true, false, false, is_const, is_reference, false, false>{
    typedef typename std::conditional<is_const, typename std::add_const<wchar_t>::type, wchar_t>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    constexpr static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_char(env, in);
    }
};

template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<false, has_scope,
        jobject,
        wchar_t, true, false, false, is_const, is_reference, false, false>{
    typedef typename std::conditional<is_const, typename std::add_const<wchar_t>::type, wchar_t>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast jobject to arithmetic reference");

    constexpr static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        return qtjambi_to_char(env, in);
    }
};

template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jobject,
        bool, true, false, false, is_const, is_reference, false, false>{
    typedef typename std::conditional<is_const, typename std::add_const<bool>::type, bool>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    constexpr static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_boolean(env, jboolean(in));
    }
};

template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<false, has_scope,
        jobject,
        bool, true, false, false, is_const, is_reference, false, false>{
    typedef typename std::conditional<is_const, typename std::add_const<bool>::type, bool>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast jobject to arithmetic reference");

    constexpr static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        return qtjambi_to_boolean(env, in);
    }
};

// QChar

template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jobject,
        QChar, false, false, false, is_const, is_reference, false, false>{
    typedef typename std::conditional<is_const, typename std::add_const<QChar>::type, QChar>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    constexpr static jobject cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        return qtjambi_from_char(env, in.unicode());
    }
};

template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<false, has_scope,
        jobject,
        QChar, false, false, false, is_const, is_reference, false, false>{
    typedef typename std::conditional<is_const, typename std::add_const<QChar>::type, QChar>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast jobject to QChar reference");

    constexpr static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        return QChar(qtjambi_to_char(env, in));
    }
};

// QStringList

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jobject,
        QStringList, false, false, is_pointer, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, const QStringList,QStringList>::type Container;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;
    static jobject cast(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
        return qtjambi_from_QStringList(env,
                                  scope ? scope->relatedNativeID() : InvalidNativeID,
                                  ref_ptr<is_pointer, Container>::ref(in),
                                  is_pointer ? nullptr : CopyFunction([](const void* ptr) -> void* { return new QStringList(*reinterpret_cast<const QStringList*>(ptr)); }),
                                  [](void* ptr,bool) { delete reinterpret_cast<QStringList*>(ptr); },
                                  is_pointer && is_const
                                );
    }
};

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<false, has_scope,
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
    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        NativeType2_ptr_out result = qtjambi_jnitype_container1_cast<false,has_scope,jobject,QList,is_pointer,is_const,is_reference,QString>::cast(env, in, nullptr, scope);
        return *reinterpret_cast<NativeType_ptr*>(&result);
    }
};

}// namespace QtJambiPrivate

#endif // QTJAMBI_CAST_JNITYPE_P_H
