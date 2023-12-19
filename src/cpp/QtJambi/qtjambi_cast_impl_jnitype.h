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

#ifndef QTJAMBI_CAST_IMPL_JNITYPE_H
#define QTJAMBI_CAST_IMPL_JNITYPE_H

#include "qtjambi_cast_impl_util.h"
#include "qtjambi_cast_impl_template1.h"
#include "qtjambi_cast_impl_template2.h"
#include "qtjambi_cast_impl_template3.h"
#include "qtjambi_cast_impl_template4.h"
#include "qtjambi_cast_impl_template5.h"

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
struct qtjambi_jnitype_template_cast_decider{
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
};

template<bool forward, bool has_scope, typename JniType, template<typename T> class Template, bool is_pointer, bool is_const, bool is_reference, typename T>
constexpr qtjambi_jnitype_template1_cast<forward, has_scope, JniType, Template, is_pointer, is_const, is_reference, T> qtjambi_jnitype_template1_supertype(){ return {}; }

template<bool forward, bool has_scope,
         typename JniType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename... Ts>
struct qtjambi_jnitype_template_cast_decider<forward, has_scope, JniType, NativeType, is_pointer, is_const, is_reference, 1, Ts...>
    : decltype(qtjambi_jnitype_template1_supertype<forward, has_scope, JniType, NativeType, is_pointer, is_const, is_reference, Ts...>()){};

template<bool forward, bool has_scope, typename JniType, template<typename K, typename T> class Template, bool is_pointer, bool is_const, bool is_reference, typename K, typename T>
constexpr qtjambi_jnitype_template2_cast<forward, has_scope, JniType, Template, is_pointer, is_const, is_reference, K, T> qtjambi_jnitype_template2_supertype(){ return {};}

template<bool forward, bool has_scope,
         typename JniType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename... Ts>
struct qtjambi_jnitype_template_cast_decider<forward, has_scope, JniType, NativeType, is_pointer, is_const, is_reference, 2, Ts...>
        : decltype(qtjambi_jnitype_template2_supertype<forward, has_scope, JniType, NativeType, is_pointer, is_const, is_reference, Ts...>()){};

template<bool forward, bool has_scope, typename JniType, template<typename K, typename T, typename A> class Template, bool is_pointer, bool is_const, bool is_reference, typename K, typename T, typename A>
constexpr qtjambi_jnitype_template3_cast<forward, has_scope, JniType, Template, is_pointer, is_const, is_reference, K, T, A> qtjambi_jnitype_template3_supertype(){ return {};}

template<bool forward, bool has_scope,
         typename JniType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename... Ts>
struct qtjambi_jnitype_template_cast_decider<forward, has_scope, JniType, NativeType, is_pointer, is_const, is_reference, 3, Ts...>
        : decltype(qtjambi_jnitype_template3_supertype<forward, has_scope, JniType, NativeType, is_pointer, is_const, is_reference, Ts...>()){};

template<bool forward, bool has_scope, typename JniType, template<typename K, typename T, typename A, typename B> class Template, bool is_pointer, bool is_const, bool is_reference, typename K, typename T, typename A, typename B>
constexpr qtjambi_jnitype_template4_cast<forward, has_scope, JniType, Template, is_pointer, is_const, is_reference, K, T, A, B> qtjambi_jnitype_template4_supertype(){ return {};}

template<bool forward, bool has_scope,
         typename JniType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename... Ts>
struct qtjambi_jnitype_template_cast_decider<forward, has_scope, JniType, NativeType, is_pointer, is_const, is_reference, 4, Ts...>
        : decltype(qtjambi_jnitype_template4_supertype<forward, has_scope, JniType, NativeType, is_pointer, is_const, is_reference, Ts...>()){};

template<bool forward, bool has_scope, typename JniType, template<typename K, typename T, typename A, typename B, typename C> class Template, bool is_pointer, bool is_const, bool is_reference, typename K, typename T, typename A, typename B, typename C>
constexpr qtjambi_jnitype_template5_cast<forward, has_scope, JniType, Template, is_pointer, is_const, is_reference, K, T, A, B, C> qtjambi_jnitype_template5_supertype(){ return {};}

template<bool forward, bool has_scope,
         typename JniType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename... Ts>
struct qtjambi_jnitype_template_cast_decider<forward, has_scope, JniType, NativeType, is_pointer, is_const, is_reference, 5, Ts...>
        : decltype(qtjambi_jnitype_template5_supertype<forward, has_scope, JniType, NativeType, is_pointer, is_const, is_reference, Ts...>()){};

template<bool forward, bool has_scope,
         typename JniType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename... Ts>
struct qtjambi_jnitype_caster<forward, has_scope,
                                 JniType,
                                 NativeType<Ts...>, false, false, is_pointer, is_const, is_reference, false, true>
        : qtjambi_jnitype_template_cast_decider<forward, has_scope, JniType, NativeType, is_pointer, is_const, is_reference, sizeof...(Ts), Ts...>{
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
        return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, reinterpret_cast<const void*>(in), typeid(NativeType_ptr), nativeTypeName);
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

    static NativeType_out cast(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope* scope){
        NativeType_ptr fct = nullptr;
        if(!QtJambiAPI::convertJavaToNative(env, typeid(NativeType_ptr), nativeTypeName, in, &fct, scope)){
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null")).arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
        }
        return fct;
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
        return QtJambiAPI::convertQObjectToJavaObject(env, in, typeid(NativeType));
    }
};

template<bool has_scope,
         typename NativeType, bool is_const, bool is_reference>
struct qtjambi_jnitype_qobject_decider_cast<false, has_scope, NativeType, true, is_const, is_reference, true, true>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_in;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_out;
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;
    static NativeType_out cast(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope* scope){
        NativeType_ptr result = nullptr;
        if(!QtJambiAPI::convertJavaToNative(env, typeid(NativeType), nativeTypeName, in, &result, scope)){
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null")).arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
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
        return QtJambiAPI::convertQObjectToJavaObject(env, &in, typeid(NativeType));
    }
};

template<bool has_scope,
         typename NativeType, bool is_const>
struct qtjambi_jnitype_qobject_decider_cast<false, has_scope, NativeType, false, is_const, true, true, true>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_out;
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;
    static NativeType_out cast(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope* scope){
        NativeType_ptr result = nullptr;
        if(!QtJambiAPI::convertJavaToNative(env, typeid(NativeType), nativeTypeName, in, &result, scope)){
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null")).arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
        }
        if(!result)
            JavaException::raiseNullPointerException(env, QStringLiteral("Cannot cast null to reference type %1").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
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
        jobject o = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, in, typeid(NativeType));
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
        jobject o = nullptr;
        if(in){
            try{
                QtJambiAPI::checkDanglingPointer(env, in);
                o = QtJambiShellInterface::getJavaObjectLocalRef(env, dynamic_cast<const QtJambiShellInterface*>(in));
            }catch(...){}
        }
        if(!o){
            o = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, in, typeid(NativeType));
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
    static NativeType_out cast(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope* scope){
        NativeType_ptr result = nullptr;
        if(!QtJambiAPI::convertJavaToNative(env, typeid(NativeType), nativeTypeName, in, &result, scope)){
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null")).arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
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
    : qtjambi_move_or_copy_decider<NativeType, is_const>{};

#ifdef QABSTRACTITEMMODEL_H
template<bool has_scope, bool is_const, bool is_reference, bool is_polymorphic>
struct qtjambi_jnitype_qobject_decider_cast<true, has_scope, QModelIndex, false, is_const, is_reference, false, is_polymorphic>{
    constexpr static jobject cast(JNIEnv * env, const QModelIndex& in, const char*, QtJambiScope*){
        return QtJambiAPI::convertModelIndexToJavaObject(env, in);
    }
};

template<bool has_scope, bool is_const, bool is_polymorphic>
struct qtjambi_jnitype_qobject_decider_cast<false, has_scope, QModelIndex, false, is_const, false, false, is_polymorphic>{
    typedef typename std::conditional<is_const, typename std::add_const<QModelIndex>::type, QModelIndex>::type QModelIndex_c;
    static QModelIndex_c cast(JNIEnv * env, jobject in, const char*, QtJambiScope*){
        QModelIndex* result = nullptr;
        if(!QtJambiAPI::convertJavaToModelIndex(env, in, &result)){
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null")).arg(QLatin1String(QtJambiAPI::typeName(typeid(QModelIndex)))) QTJAMBI_STACKTRACEINFO );
        }
        return result ? *result : QModelIndex();
    }
};


template<bool has_scope, bool is_polymorphic>
struct qtjambi_jnitype_qobject_decider_cast<false, has_scope, QModelIndex, false, true, true, false, is_polymorphic>{
    typedef typename std::conditional<true, typename std::add_const<QModelIndex>::type, QModelIndex>::type QModelIndex_c;
    typedef typename std::conditional<true, typename std::add_lvalue_reference<QModelIndex_c>::type, QModelIndex_c>::type QModelIndex_cr;
    static QModelIndex_cr cast(JNIEnv * env, jobject in, const char*, QtJambiScope*){
        QModelIndex* result = nullptr;
        if(!QtJambiAPI::convertJavaToModelIndex(env, in, &result)){
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null")).arg(QLatin1String(QtJambiAPI::typeName(typeid(QModelIndex)))) QTJAMBI_STACKTRACEINFO );
        }
        return deref_ptr_or_default<true,QModelIndex>::deref(result);
    }
};
#endif

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
        return QtJambiAPI::convertQVariantToJavaObject(env, in);
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
    static NativeType_cr cast(JNIEnv * env, jobject in, const char* nativeTypeName, QtJambiScope* scope){
        NativeType* result = nullptr;
        if(!QtJambiAPI::convertJavaToNative(env, typeid(NativeType), nativeTypeName, in, &result, scope)){
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null")).arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
        }
        return QtJambiAPI::checkedAddressOf<NativeType>(env, result);
    }
};

// QVariant& and QString& are not light-weight
template<bool has_scope, bool is_const>
struct qtjambi_jnitype_use_pointer_decider_cast<has_scope, QVariant, is_const, true, true /*dont_use_pointer*/>
        : qtjambi_jnitype_use_pointer_decider_cast<has_scope, QVariant, is_const, true, false>{};

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_use_pointer_decider_cast<has_scope, QString, is_const, true, true /*dont_use_pointer*/>
        : qtjambi_jnitype_use_pointer_decider_cast<has_scope, QString, is_const, true, false>{};

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_use_pointer_decider_cast<has_scope, QVariant, is_const, false, true /*dont_use_pointer*/>{
    typedef typename std::conditional<is_const, typename std::add_const<QVariant>::type, QVariant>::type NativeType_c;

    static NativeType_c cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        return QtJambiAPI::convertJavaObjectToQVariant(env, in);
    }
};

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#ifdef QABSTRACTITEMMODEL_H
template<bool has_scope, bool is_const>
struct qtjambi_jnitype_use_pointer_decider_cast<has_scope, QModelRoleDataSpan, is_const, false, true /*dont_use_pointer*/>{
    typedef typename std::conditional<is_const, typename std::add_const<QModelRoleDataSpan>::type, QModelRoleDataSpan>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;

    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QModelRoleDataSpan reference without scope");
    static NativeType_cr cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QModelRoleDataSpan reference without scope." QTJAMBI_STACKTRACEINFO );
        void *data(nullptr);
        qsizetype length(0);
        QtJambiAPI::convertJavaObjectToQModelRoleData(env, *scope, in, data, length);
        return QModelRoleDataSpan(reinterpret_cast<QModelRoleData*>(data), length);
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_use_pointer_decider_cast<has_scope, QModelRoleDataSpan, is_const, true, true /*dont_use_pointer*/>{
    typedef typename std::conditional<is_const, typename std::add_const<QModelRoleDataSpan>::type, QModelRoleDataSpan>::type NativeType_c;
    typedef typename std::conditional<true, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;

    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QModelRoleDataSpan reference without scope");

    static NativeType_cr cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QModelRoleDataSpan reference without scope." QTJAMBI_STACKTRACEINFO );
        void *data(nullptr);
        qsizetype length(0);
        QtJambiAPI::convertJavaObjectToQModelRoleData(env, *scope, in, data, length);
        QModelRoleDataSpan* buffer = new QModelRoleDataSpan(reinterpret_cast<QModelRoleData*>(data), length);
        scope->addDeletion(buffer);
        return *buffer;
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

    static NativeType_cr cast(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope* scope){
        NativeType result;
        if(!QtJambiAPI::convertJavaToNative(env, typeid(NativeType), nativeTypeName, in, &result, scope)){
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null")).arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
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

    static NativeType_cr cast(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope* scope){
        NativeType_ptr result = nullptr;
        if(!QtJambiAPI::convertJavaToNative(env, typeid(NativeType), nativeTypeName, in, &result, scope)){
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null")).arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
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

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_const, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QByteArrayView, is_arithmetic, is_enum, false, is_const, false, is_function, is_template>{
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QByteArrayView without scope.");
    typedef typename std::conditional<is_const, typename std::add_const<QByteArrayView>::type, QByteArrayView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QByteArrayView without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QByteArrayView without scope." QTJAMBI_STACKTRACEINFO );
        J2CStringBuffer* buffer = new J2CStringBuffer(env, in);
        scope->addDeletion(buffer);
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        return buffer->toByteArrayView();
    }
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_pointer, bool is_const, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QByteArrayView, is_arithmetic, is_enum, is_pointer, is_const, is_reference, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QByteArrayView>::type, QByteArrayView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope || !is_reference, "Cannot cast to QByteArrayView& without scope.");
    Q_STATIC_ASSERT_X(has_scope || !is_pointer, "Cannot cast to QByteArrayView* without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char* name, QtJambiScope* scope){
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        if constexpr(is_pointer){
            if(!in)
                return nullptr;
        }
#else
        if(is_pointer && !is_reference && !in){
            QByteArrayView* result = nullptr;
            return ptr2ref<is_reference || !is_pointer,QByteArrayView>::value(env, result);
        }
#endif
        if(!scope)
            JavaException::raiseError(env, is_pointer ? "Cannot cast to QByteArrayView* or reference without scope." : "Cannot cast to QByteArrayView& or reference without scope." QTJAMBI_STACKTRACEINFO );
        QByteArrayView* result = new QByteArrayView(qtjambi_jstring_caster<true, QByteArrayView, is_arithmetic, is_enum, false, false, false, is_function, is_template>::cast(env, in, name, scope));
        scope->addDeletion(result);
        return ptr2ref<is_reference || !is_pointer,QByteArrayView>::value(env, result);
    }
};
#endif

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_const, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QByteArray, is_arithmetic, is_enum, false, is_const, false, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QByteArray>::type, QByteArray>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jstring in, const char*, QtJambiScope*){
        QByteArray result;
        if(!env->IsSameObject(in, nullptr)){
            int length = env->GetStringUTFLength(in);
            result.resize(length);
            env->GetStringUTFRegion(in, 0, length, result.data());
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
        }
        return result;
    }
};

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
template<bool is_arithmetic, bool is_enum, bool is_const, bool is_function, bool is_template>
struct qtjambi_jstring_caster<true, QByteArray, is_arithmetic, is_enum, false, is_const, false, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QByteArray>::type, QByteArray>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jstring in, const char*name, QtJambiScope* scope){
        if(!scope)
            return qtjambi_jstring_caster<false, QByteArray, is_arithmetic, is_enum, false, is_const, false, is_function, is_template>::cast(env, in, name, scope);
        QByteArray result;
        if(!env->IsSameObject(in, nullptr)){
            J2CStringBuffer* buffer = new J2CStringBuffer(env, in);
            scope->addDeletion(buffer);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            result = buffer->toByteArray();
        }
        return result;
    }
};
#endif

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_pointer, bool is_const, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QByteArray, is_arithmetic, is_enum, is_pointer, is_const, is_reference, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QByteArray>::type, QByteArray>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope || !is_reference, "Cannot cast to QByteArray& without scope.");
    Q_STATIC_ASSERT_X(has_scope || !is_pointer, "Cannot cast to QByteArray* without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char* name, QtJambiScope* scope){
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        if constexpr(is_pointer){
            if(!in)
                return nullptr;
        }
#else
        if(is_pointer && !is_reference && !in){
            QByteArray* result = nullptr;
            return ptr2ref<is_reference || !is_pointer,QByteArray>::value(env, result);
        }
#endif
        if(!scope)
            JavaException::raiseError(env, is_pointer ? "Cannot cast to QByteArray* or reference without scope." : "Cannot cast to QByteArray& or reference without scope." QTJAMBI_STACKTRACEINFO );
        QByteArray* result = new QByteArray(qtjambi_jstring_caster<true, QByteArray, is_arithmetic, is_enum, false, false, false, is_function, is_template>::cast(env, in, name, scope));
        scope->addDeletion(result);
        return ptr2ref<is_reference || !is_pointer,QByteArray>::value(env, result);
    }
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, char, is_arithmetic, is_enum, true, true, is_reference, is_function, is_template>{
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to const char* without scope.");

    static const char* cast(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to const char* without scope." QTJAMBI_STACKTRACEINFO );
        J2CStringBuffer* buffer = new J2CStringBuffer(env, in);
        scope->addDeletion(buffer);
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        return buffer->data();
    }
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QChar, is_arithmetic, is_enum, true, true, is_reference, is_function, is_template>{
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to const QChar* without scope.");

    static const QChar* cast(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to const QChar* without scope." QTJAMBI_STACKTRACEINFO );
        JString2QChars* buffer = new JString2QChars(env, in);
        scope->addDeletion(buffer);
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        return buffer->constData();
    }
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, char, is_arithmetic, is_enum, true, false, is_reference, is_function, is_template>{
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to char* without scope.");

    static char* cast(JNIEnv *env, jstring in, const char* name, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to const char pointer or reference without scope." QTJAMBI_STACKTRACEINFO );
        QByteArray* result = qtjambi_jstring_caster<true, QByteArray, is_arithmetic, is_enum, false, false, false, is_function, is_template>::cast(env, in, name, scope);
        scope->addDeletion(result);
        char* data = result->data();
        return data;
    }
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_const, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QString, is_arithmetic, is_enum, false, is_const, false, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QString>::type, QString>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jstring in, const char*, QtJambiScope*){
        QString result;
        if(!env->IsSameObject(in, nullptr)){
            int length = env->GetStringLength(in);
            result.resize(length);
            env->GetStringRegion(in, 0, length, reinterpret_cast<ushort*>(result.data()));
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
        }
        return result;
    }
};

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
template<bool is_arithmetic, bool is_enum, bool is_const, bool is_function, bool is_template>
struct qtjambi_jstring_caster<true, QString, is_arithmetic, is_enum, false, is_const, false, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QString>::type, QString>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jstring in, const char* name, QtJambiScope* scope){
        if(!scope)
            return qtjambi_jstring_caster<false, QString, is_arithmetic, is_enum, false, is_const, false, is_function, is_template>::cast(env, in, name, scope);
        QString result;
        if(!env->IsSameObject(in, nullptr)){
            JString2QChars* buffer = new JString2QChars(env, in);
            scope->addDeletion(buffer);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            result = buffer->toString();
        }
        return result;
    }
};
#endif

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_pointer, bool is_const, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QString, is_arithmetic, is_enum, is_pointer, is_const, is_reference, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QString>::type, QString>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope || !is_reference, "Cannot cast to QString& without scope.");
    Q_STATIC_ASSERT_X(has_scope || !is_pointer, "Cannot cast to QString* without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char* name, QtJambiScope* scope){
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        if constexpr(is_pointer){
            if(!in)
                return nullptr;
        }
#else
        if(is_pointer && !is_reference && !in){
            QString* result = nullptr;
            return ptr2ref<is_reference || !is_pointer,QString>::value(env, result);
        }
#endif
        if(!scope)
            JavaException::raiseError(env, is_pointer ? "Cannot cast to QString* or reference without scope." : "Cannot cast to QString& or reference without scope." QTJAMBI_STACKTRACEINFO );
        QString* result = new QString(qtjambi_jstring_caster<true, QString, is_arithmetic, is_enum, false, false, false, is_function, is_template>::cast(env, in, name, scope));
        scope->addDeletion(result);
        return ptr2ref<is_reference || !is_pointer,QString>::value(env, result);
    }
};

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_pointer, bool is_const, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QStringRef, is_arithmetic, is_enum, is_pointer, is_const, is_reference, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QStringRef>::type, QStringRef>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope || !is_reference, "Cannot cast to QStringRef &.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char* nativeTypeName, QtJambiScope* scope){
        if(is_pointer && !is_reference && !in){
            QStringRef* result = nullptr;
            return ptr2ref<is_reference || !is_pointer,QStringRef>::value(env, result);
        }
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QStringRef without scope." QTJAMBI_STACKTRACEINFO );
        QStringRef* ref = new QStringRef(qtjambi_jstring_caster<true, QString, is_arithmetic, is_enum, true, false, false, is_function, is_template>::cast(env, in, nativeTypeName, scope));
        scope->addDeletion(ref);
        return ref;
    }
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_const, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QStringRef, is_arithmetic, is_enum, false, is_const, false, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QStringRef>::type, QStringRef>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QStringRef without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char* nativeTypeName, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QStringRef without scope." QTJAMBI_STACKTRACEINFO );
        QString* result = qtjambi_jstring_caster<has_scope, QString, is_arithmetic, is_enum, true, is_const, false, is_function, is_template>::cast(env, in, nativeTypeName, scope);
        scope->addDeletion(result);
        return QStringRef(result);
    }
};

#else

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_pointer, bool is_const, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QAnyStringView, is_arithmetic, is_enum, is_pointer, is_const, is_reference, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QAnyStringView>::type, QAnyStringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope || !is_reference, "Cannot cast to QAnyStringView& without scope.");
    Q_STATIC_ASSERT_X(has_scope || !is_pointer, "Cannot cast to QAnyStringView* without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char* name, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, is_pointer ? "Cannot cast to QAnyStringView* without scope." : "Cannot cast to QAnyStringView& without scope." QTJAMBI_STACKTRACEINFO );
        QAnyStringView* ptr = new QAnyStringView(qtjambi_jstring_caster<true, QAnyStringView, is_arithmetic, is_enum, false, is_const, false, is_function, is_template>::cast(env, in, name, scope));
        scope->addDeletion(ptr);
        return ptr2ref<is_reference || !is_pointer,QAnyStringView>::value(env, ptr);
    }
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_const, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QAnyStringView, is_arithmetic, is_enum, false, is_const, false, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QAnyStringView>::type, QAnyStringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QAnyStringView without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QAnyStringView without scope." QTJAMBI_STACKTRACEINFO );
        JString2QChars* buffer = new JString2QChars(env, in);
        scope->addDeletion(buffer);
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        return buffer->toAnyStringView();
    }
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_pointer, bool is_const, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QUtf8StringView, is_arithmetic, is_enum, is_pointer, is_const, is_reference, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QUtf8StringView>::type, QUtf8StringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope || !is_reference, "Cannot cast to QUtf8StringView& without scope.");
    Q_STATIC_ASSERT_X(has_scope || !is_pointer, "Cannot cast to QUtf8StringView* without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char* name, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, is_pointer ? "Cannot cast to QUtf8StringView* without scope." : "Cannot cast to QUtf8StringView& without scope." QTJAMBI_STACKTRACEINFO );
        QUtf8StringView* ptr = new QUtf8StringView(qtjambi_jstring_caster<true, QUtf8StringView, is_arithmetic, is_enum, false, is_const, false, is_function, is_template>::cast(env, in, name, scope));
        scope->addDeletion(ptr);
        return ptr2ref<is_reference || !is_pointer,QUtf8StringView>::value(env, ptr);
    }
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_const, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QUtf8StringView, is_arithmetic, is_enum, false, is_const, false, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QUtf8StringView>::type, QUtf8StringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QUtf8StringView without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QUtf8StringView without scope." QTJAMBI_STACKTRACEINFO );
        J2CStringBuffer* buffer = new J2CStringBuffer(env, in);
        scope->addDeletion(buffer);
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        return buffer->toUtf8StringView();
    }
};

#endif

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_pointer, bool is_const, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QStringView, is_arithmetic, is_enum, is_pointer, is_const, is_reference, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QStringView>::type, QStringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope || !is_reference, "Cannot cast to QStringView& without scope.");
    Q_STATIC_ASSERT_X(has_scope || !is_pointer, "Cannot cast to QStringView* without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char* name, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, is_pointer ? "Cannot cast to QStringView* without scope." : "Cannot cast to QStringView& without scope." QTJAMBI_STACKTRACEINFO );
        QStringView* ptr = new QStringView(qtjambi_jstring_caster<true, QStringView, is_arithmetic, is_enum, false, is_const, false, is_function, is_template>::cast(env, in, name, scope));
        scope->addDeletion(ptr);
        return ptr2ref<is_reference || !is_pointer,QStringView>::value(env, ptr);
    }
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_const, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QStringView, is_arithmetic, is_enum, false, is_const, false, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QStringView>::type, QStringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QStringView without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QStringView without scope." QTJAMBI_STACKTRACEINFO );
        JString2QChars* buffer = new JString2QChars(env, in);
        scope->addDeletion(buffer);
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        return buffer->toStringView();
    }
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_pointer, bool is_const, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QLatin1String, is_arithmetic, is_enum, is_pointer, is_const, is_reference, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1String>::type, QLatin1String>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope || !is_reference, "Cannot cast to QLatin1String& without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char* name, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, is_pointer ? "Cannot cast to QLatin1String* without scope." : "Cannot cast to QLatin1String& without scope." QTJAMBI_STACKTRACEINFO );
        QLatin1String* ptr = new QLatin1String(qtjambi_jstring_caster<true, QLatin1String, is_arithmetic, is_enum, false, is_const, false, is_function, is_template>::cast(env, in, name, scope));
        scope->addDeletion(ptr);
        return ptr2ref<is_reference || !is_pointer,QLatin1String>::value(env, ptr);
    }
};

template<bool has_scope, bool is_arithmetic, bool is_enum, bool is_const, bool is_function, bool is_template>
struct qtjambi_jstring_caster<has_scope, QLatin1String, is_arithmetic, is_enum, false, is_const, false, is_function, is_template>{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1String>::type, QLatin1String>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QLatin1String without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QLatin1String without scope." QTJAMBI_STACKTRACEINFO );
        J2CStringBuffer* buffer = new J2CStringBuffer(env, in);
        scope->addDeletion(buffer);
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        return buffer->toLatin1String();
    }
};

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_caster<false, has_scope,
        jobject,
        QStringRef, false, false, false, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringRef>::type, QStringRef>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    static NativeType_out cast(JNIEnv *env, jobject in, const char* name, QtJambiScope* scope){
        if(QtJambiAPI::isQStringObject(env, in)){
            return QStringRef(QtJambiAPI::convertJavaObjectToNative<QString>(env, in));
        }else{
            if(!scope)
                JavaException::raiseError(env, "Cannot cast to QStringRef without scope." QTJAMBI_STACKTRACEINFO );
            return qtjambi_scoped_cast<true,NativeType_out,jstring>::cast(env, QtJambiAPI::toJavaString(env, in), name, scope);
        }
    }
};

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jstring,
        QStringRef, false, false, is_pointer, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringRef>::type, QStringRef>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jstring cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        if(NativeType_c* ptr = ref_ptr<is_pointer, NativeType_c>::ref(in)){
            jstring str = env->NewString(reinterpret_cast<const jchar *>(ptr->data()), jsize(ptr->size()));
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            return str;
        }
        return nullptr;
    }
};

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<false, has_scope,
        jobject,
        QStringRef, false, false, is_pointer, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringRef>::type, QStringRef>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope || !is_reference, "Cannot cast to QStringRef& without scope.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char* name, QtJambiScope* scope){
        if(is_pointer && !is_reference && !in){
            QStringRef* result = nullptr;
            return ptr2ref<is_reference || !is_pointer,QStringRef>::value(env, result);
        }
        if(!scope)
            JavaException::raiseError(env, is_pointer ? "Cannot cast to QStringRef* without scope." : "Cannot cast to QStringRef& without scope." QTJAMBI_STACKTRACEINFO );
        QStringRef* ptr = new QStringRef(qtjambi_jnitype_caster<false, has_scope,
                                         jobject, QStringRef, false, false, false, is_const, false, false, false>::cast(env, in, name, scope));
        scope->addDeletion(ptr);
        return ptr2ref<is_reference || !is_pointer,QStringRef>::value(env, ptr);
    }
};

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jobject,
        QStringRef, false, false, is_pointer, is_const, is_reference, false, false>
        : qtjambi_jnitype_caster<true, has_scope,
                                jstring,
                                QStringRef, false, false, is_pointer, is_const, is_reference, false, false>{};

#else

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jstring,
        QAnyStringView, false, false, is_pointer, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QAnyStringView>::type, QAnyStringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jstring cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        if(NativeType_c* ptr = ref_ptr<is_pointer, NativeType_c>::ref(in)){
            return QtJambiAPI::convertNativeToJavaObject(env, *ptr);
        }else return nullptr;
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_caster<false, has_scope,
        jobject,
        QAnyStringView, false, false, false, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QAnyStringView>::type, QAnyStringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    static NativeType_out cast(JNIEnv *env, jobject in, const char* name, QtJambiScope* scope){
        if(QtJambiAPI::isQStringObject(env, in)){
            return QAnyStringView(QtJambiAPI::convertJavaObjectToNativeReference<QString>(env, in));
        }else if(JBufferConstData::isBuffer(env, in)){
            if(!scope)
                JavaException::raiseError(env, "Cannot cast to QStringView without scope." QTJAMBI_STACKTRACEINFO );
            JBufferConstData* buffer = new JBufferConstData(env, in);
            scope->addDeletion(buffer);
            return QAnyStringView(buffer->constData<QChar>(), buffer->size<QChar>());
        }else{
            if(!scope)
                JavaException::raiseError(env, "Cannot cast to QAnyStringView without scope." QTJAMBI_STACKTRACEINFO );
            return qtjambi_scoped_cast<true,NativeType_out,jstring>::cast(env, QtJambiAPI::toJavaString(env, in), name, scope);
        }
    }
};

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<false, has_scope,
        jobject,
        QAnyStringView, false, false, is_pointer, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QAnyStringView>::type, QAnyStringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope || !is_reference, "Cannot cast to QAnyStringView& without scope.");
    Q_STATIC_ASSERT_X(has_scope || !is_pointer, "Cannot cast to QAnyStringView* without scope.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char* name, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, is_pointer ? "Cannot cast to QAnyStringView* without scope." : "Cannot cast to QAnyStringView& without scope." QTJAMBI_STACKTRACEINFO );
        QAnyStringView* ptr = new QAnyStringView(qtjambi_jnitype_caster<false, true,
                                                 jobject, QAnyStringView, false, false, false, is_const, false, false, false>::cast(env, in, name, scope));
        scope->addDeletion(ptr);
        return ptr2ref<is_reference || !is_pointer,QAnyStringView>::value(env, ptr);
    }
};

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jobject,
        QAnyStringView, false, false, is_pointer, is_const, is_reference, false, false>
        : qtjambi_jnitype_caster<true, has_scope,
                                jstring,
                                QAnyStringView, false, false, is_pointer, is_const, is_reference, false, false>{};

// end QAnyStringView

// begin QUtf8StringView

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jstring,
        QUtf8StringView, false, false, is_pointer, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QUtf8StringView>::type, QUtf8StringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jstring cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        if(NativeType_c* ptr = ref_ptr<is_pointer, NativeType_c>::ref(in)){
            Q_ASSERT(ptr->length()>=0);
            jstring str = env->NewStringUTF(ptr->data());
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            return str;
        }else return nullptr;
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_caster<false, has_scope,
        jstring,
        QUtf8StringView, false, false, false, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QUtf8StringView>::type, QUtf8StringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope, "Cannot cast to QUtf8StringView without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QUtf8StringView without scope." QTJAMBI_STACKTRACEINFO );
        J2CStringBuffer* buffer = new J2CStringBuffer(env, in);
        scope->addDeletion(buffer);
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        return buffer->toUtf8StringView();
    }
};

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<false, has_scope,
        jstring,
        QUtf8StringView, false, false, is_pointer, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QUtf8StringView>::type, QUtf8StringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope || !is_reference, "Cannot cast to QUtf8StringView& without scope.");
    Q_STATIC_ASSERT_X(has_scope || !is_pointer, "Cannot cast to QUtf8StringView* without scope.");

    static NativeType_out cast(JNIEnv *env, jstring in, const char* name, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, is_pointer ? "Cannot cast to QUtf8StringView* without scope." : "Cannot cast to QUtf8StringView& without scope." QTJAMBI_STACKTRACEINFO );
        QUtf8StringView* ptr = new QUtf8StringView(qtjambi_jnitype_caster<false, true,
                                                   jstring, QUtf8StringView, false, false, false, is_const, false, false, false>::cast(env, in, name, scope));
        scope->addDeletion(ptr);
        return ptr2ref<is_reference && !is_pointer,QUtf8StringView>::value(env, ptr);
    }
};

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jobject,
        QUtf8StringView, false, false, is_pointer, is_const, is_reference, false, false>
        : qtjambi_jnitype_caster<true, has_scope,
                                jstring,
                                QUtf8StringView, false, false, is_pointer, is_const, is_reference, false, false>{};


#endif

// end QUtf8StringView

// begin QStringView

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_caster<false, has_scope,
        jobject,
        QStringView, false, false, false, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringView>::type, QStringView>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    static NativeType_out cast(JNIEnv *env, jobject in, const char* name, QtJambiScope* scope){
        if(QtJambiAPI::isQStringObject(env, in)){
            return QStringView(QtJambiAPI::convertJavaObjectToNativeReference<QString>(env, in));
        }else if(JBufferConstData::isBuffer(env, in)){
            if(!scope)
                JavaException::raiseError(env, "Cannot cast to QStringView without scope." QTJAMBI_STACKTRACEINFO );
            JBufferConstData* buffer = new JBufferConstData(env, in);
            scope->addDeletion(buffer);
            return QStringView(buffer->constData<QChar>(), buffer->size<QChar>());
        }else{
            if(!scope)
                JavaException::raiseError(env, "Cannot cast to QStringView without scope." QTJAMBI_STACKTRACEINFO );
            return qtjambi_scoped_cast<true,NativeType_out,jstring>::cast(env, QtJambiAPI::toJavaString(env, in), name, scope);
        }
    }
};

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jstring,
        QStringView, false, false, is_pointer, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringView>::type, QStringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jstring cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        if(NativeType_c* ptr = ref_ptr<is_pointer, NativeType_c>::ref(in)){
            jstring str = env->NewString(reinterpret_cast<const jchar *>(ptr->data()), jsize(ptr->length()));
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            return str;
        }
        return nullptr;
    }
};

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<false, has_scope,
        jobject,
        QStringView, false, false, is_pointer, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QStringView>::type, QStringView>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope || !is_reference, "Cannot cast to QStringView& without scope.");
    Q_STATIC_ASSERT_X(has_scope || !is_pointer, "Cannot cast to QStringView* without scope.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char* name, QtJambiScope* scope){
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        if constexpr(is_pointer){
            if(!in)
                return nullptr;
        }
#else
        if(is_pointer && !is_reference && !in){
            QStringView* result = nullptr;
            return ptr2ref<is_reference || !is_pointer,QStringView>::value(env, result);
        }
#endif
        if(!scope)
            JavaException::raiseError(env, is_pointer ? "Cannot cast to QStringView* without scope." : "Cannot cast to QStringView& without scope." QTJAMBI_STACKTRACEINFO );
        QStringView* ptr = new QStringView(qtjambi_jnitype_caster<false, true,
                                           jobject, QStringView, false, false, false, is_const, false, false, false>::cast(env, in, name, scope));
        scope->addDeletion(ptr);
        return ptr2ref<is_reference || !is_pointer,QStringView>::value(env, ptr);
    }
};

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jobject,
        QStringView, false, false, is_pointer, is_const, is_reference, false, false>
        : qtjambi_jnitype_caster<true, has_scope,
                                jstring,
                                QStringView, false, false, is_pointer, is_const, is_reference, false, false>{};

// end QStringView

// begin QString

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jstring,
        QString, false, false, is_pointer, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QString>::type, QString>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jstring cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        if(NativeType_c* ptr = ref_ptr<is_pointer, NativeType_c>::ref(in)){
            jstring str = env->NewString(reinterpret_cast<const jchar *>(ptr->data()), jsize(ptr->length()));
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            return str;
        }
        return nullptr;
    }
};

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<false, has_scope,
        jobject,
        QString, false, false, is_pointer, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QString>::type, QString>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static NativeType_out cast(JNIEnv *env, jobject in, const char* name, QtJambiScope* scope){
        if(QtJambiAPI::isQStringObject(env, in)){
            return ptr2ref<is_reference || !is_pointer, QString>::value(env, QtJambiAPI::convertJavaObjectToNative<QString>(env, in));
        }else if(JBufferConstData::isBuffer(env, in)){
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            if constexpr(is_pointer || is_reference){
#else
            if(is_pointer || is_reference){
#endif
                if(!scope)
                    JavaException::raiseError(env, is_pointer ? "Cannot cast to QString* without scope." : "Cannot cast to QString& without scope." QTJAMBI_STACKTRACEINFO );
                JBufferConstData* buffer = new JBufferConstData(env, in);
                scope->addDeletion(buffer);
                QString* result = new QString(buffer->constData<QChar>(), buffer->size<QChar>());
                return ptr2ref<is_reference || !is_pointer, QString>::value(env, result);
            }else{
                JBufferConstData buffer(env, in);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                return QString(buffer.constData<QChar>(), buffer.size<QChar>());
#else
                QString result(buffer.constData<QChar>(), buffer.size<QChar>());
                return ptr2ref<is_reference || !is_pointer, QString>::value(env, &result);
#endif
            }
        }else{
            if((is_pointer || is_reference) && !scope)
                JavaException::raiseError(env, is_pointer ? "Cannot cast to QString* without scope." : "Cannot cast to QString& without scope." QTJAMBI_STACKTRACEINFO );
            return qtjambi_scoped_cast<is_pointer||is_reference,NativeType_out,jstring>::cast(env, QtJambiAPI::toJavaString(env, in), name, scope);
        }
    }
};

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jobject,
        QString, false, false, is_pointer, is_const, is_reference, false, false>
        : qtjambi_jnitype_caster<true, has_scope,
                                jstring,
                                QString, false, false, is_pointer, is_const, is_reference, false, false>{};

// end QString

// begin QLatin1String

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jstring,
        QLatin1String, false, false, is_pointer, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1String>::type, QLatin1String>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jstring cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        if(NativeType_c* ptr = ref_ptr<is_pointer, NativeType_c>::ref(in)){
            jstring str = env->NewStringUTF(ptr->data());
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            return str;
        }else return nullptr;
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_caster<false, has_scope,
        jobject,
        QLatin1String, false, false, false, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1String>::type, QLatin1String>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<false, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    static NativeType_out cast(JNIEnv *env, jobject in, const char*, QtJambiScope* scope){
        if(!in)
            return QLatin1String();
        if(QtJambiAPI::isQByteArrayObject(env, in)){
            const QByteArray& data = QtJambiAPI::convertJavaObjectToNativeReference<QByteArray>(env, in);
            return QLatin1String(data.data());
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        }else if(QtJambiAPI::isQByteArrayViewObject(env, in)){
            const QByteArrayView& data = QtJambiAPI::convertJavaObjectToNativeReference<QByteArrayView>(env, in);
            return QLatin1String(data.data());
#endif
        }else{
            if(!scope)
                JavaException::raiseError(env, "Cannot cast to QLatin1String without scope." QTJAMBI_STACKTRACEINFO );
            jstring _in = QtJambiAPI::toJavaString(env, in);
            J2CStringBuffer* buffer = new J2CStringBuffer(env, _in);
            scope->addDeletion(buffer);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            return buffer->toLatin1String();
        }
    }
};

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_caster<true, has_scope,
        jobject,
        QLatin1String, false, false, true, is_const, false, false, false>
        : qtjambi_jnitype_caster<true, has_scope,
                                jstring,
                                QLatin1String, false, false, true, is_const, false, false, false>{};

template<bool has_scope, bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<false, has_scope,
        jobject,
        QLatin1String, false, false, is_pointer, is_const, is_reference, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1String>::type, QLatin1String>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(has_scope || !is_reference, "Cannot cast to QLatin1String& without scope.");

    static NativeType_out cast(JNIEnv *env, jobject in, const char*name, QtJambiScope* scope){
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        if constexpr(is_pointer){
            if(!in)
                return nullptr;
        }
#else
        if(is_pointer && !is_reference && !in){
            QLatin1String* result = nullptr;
            return ptr2ref<is_reference || !is_pointer,QLatin1String>::value(env, result);
        }
#endif
        if(!scope)
            JavaException::raiseError(env, is_pointer ? "Cannot cast to QLatin1String* without scope." : "Cannot cast to QLatin1String& without scope." QTJAMBI_STACKTRACEINFO );
        QLatin1String* ref = new QLatin1String(qtjambi_jnitype_caster<false, true,
                                               jobject, QLatin1String, false, false, false, is_const, false, false, false>::cast(env, in, name, scope));
        scope->addDeletion(ref);
        return ptr2ref<is_reference, QLatin1String>::value(env, ref);
    }
};

template<bool has_scope, bool is_const, bool is_reference>
struct qtjambi_jnitype_caster<true, has_scope,
        jobject,
        QLatin1String, false, false, false, is_const, is_reference, false, false>
        : qtjambi_jnitype_caster<true, has_scope,
                                jstring,
                                QLatin1String, false, false, false, is_const, is_reference, false, false>{};

// end QLatin1String

template<bool has_scope, bool is_const>
struct qtjambi_jnitype_caster<true, has_scope,
        jstring,
        char, false, false, true, is_const, false, false, false>
{
    typedef typename std::conditional<is_const, typename std::add_const<char>::type, QLatin1String>::type NativeType_c;
    typedef typename std::conditional<false, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<true, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static jstring cast(JNIEnv *env, NativeType_in in, const char*, QtJambiScope*){
        jstring str = in ? env->NewStringUTF(in) : nullptr;
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        return str;
    }
};

template<bool has_scope,
         bool is_const, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jnitype_caster<true, has_scope, jobject, _jstring, false, false, true, is_const, is_reference, is_function, is_template>
{
    constexpr static jobject cast(JNIEnv *, jstring in, const char*, QtJambiScope*){
        return in;
    }
};

template<bool has_scope,
         bool is_const, bool is_reference, bool is_function, bool is_template>
struct qtjambi_jnitype_caster<true, has_scope, jstring, _jobject, false, false, true, is_const, is_reference, is_function, is_template>
{
    constexpr static jstring cast(JNIEnv *env, jobject in, const char*, QtJambiScope*){
        return QtJambiAPI::toJavaString(env, in);
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
        return QtJambiAPI::toJavaString(env, qtjambi_scoped_cast<has_scope,jobject,NativeType_in>::cast(env, in, nativeTypeName, scope));
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
        return QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, &in, typeid(NativeType));
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

    static NativeType_out cast(JNIEnv *env, jobject in, const char* nativeTypeName, QtJambiScope* scope){
        NativeType result;
        if(!QtJambiAPI::convertJavaToNative(env, typeid(NativeType), nativeTypeName, in, &result, scope)){
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null")).arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
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
        return QtJambiAPI::toJavaByteObject(env, jbyte(in));
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
        return NativeType(QtJambiAPI::fromJavaByteObject(env, in));
    }
};

template<bool has_scope, typename NativeType, bool a_is_const, bool a_is_reference, bool a_is_signed>
struct qtjambi_jnitype_arithmetic_caster<true, has_scope,
        NativeType, a_is_const, a_is_reference, true, false, a_is_signed, sizeof(jshort)>{
    constexpr static jobject cast(JNIEnv *env, NativeType in, const char*, QtJambiScope*){
        return QtJambiAPI::toJavaShortObject(env, jshort(in));
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
        return NativeType(QtJambiAPI::fromJavaShortObject(env, in));
    }
};

template<bool has_scope, typename NativeType, bool a_is_const, bool a_is_reference, bool a_is_signed>
struct qtjambi_jnitype_arithmetic_caster<true, has_scope,
        NativeType, a_is_const, a_is_reference, true, false, a_is_signed, sizeof(jint)>{
    constexpr static jobject cast(JNIEnv *env, NativeType in, const char*, QtJambiScope*){
        return QtJambiAPI::toJavaIntegerObject(env, jint(in));
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
        return NativeType(QtJambiAPI::fromJavaIntegerObject(env, in));
    }
};

template<bool has_scope, typename NativeType, bool a_is_const, bool a_is_reference, bool a_is_signed>
struct qtjambi_jnitype_arithmetic_caster<true, has_scope,
        NativeType, a_is_const, a_is_reference, true, false, a_is_signed, sizeof(jlong)>{
    constexpr static jobject cast(JNIEnv *env, NativeType in, const char*, QtJambiScope*){
        return QtJambiAPI::toJavaLongObject(env, jlong(in));
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
        return NativeType(QtJambiAPI::fromJavaLongObject(env, in));
    }
};

template<bool has_scope, typename NativeType, bool a_is_const, bool a_is_reference, bool a_is_signed>
struct qtjambi_jnitype_arithmetic_caster<true, has_scope,
        NativeType, a_is_const, a_is_reference, false, true, a_is_signed, sizeof(jdouble)>{
    constexpr static jobject cast(JNIEnv *env, NativeType in, const char*, QtJambiScope*){
        return QtJambiAPI::toJavaDoubleObject(env, jdouble(in));
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
        return NativeType(QtJambiAPI::fromJavaDoubleObject(env, in));
    }
};

template<bool has_scope, typename NativeType, bool a_is_const, bool a_is_reference, bool a_is_signed>
struct qtjambi_jnitype_arithmetic_caster<true, has_scope,
        NativeType, a_is_const, a_is_reference, false, true, a_is_signed, sizeof(jfloat)>{
    constexpr static jobject cast(JNIEnv *env, NativeType in, const char*, QtJambiScope*){
        return QtJambiAPI::toJavaFloatObject(env, jfloat(in));
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
        return NativeType(QtJambiAPI::fromJavaFloatObject(env, in));
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
        return QtJambiAPI::toJavaCharacterObject(env, in);
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
        return QtJambiAPI::fromJavaCharacterObject(env, in);
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
        return QtJambiAPI::toJavaBooleanObject(env, jboolean(in));
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
        return QtJambiAPI::fromJavaBooleanObject(env, in);
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
        return QtJambiAPI::toJavaCharacterObject(env, in.unicode());
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
        return QChar(QtJambiAPI::fromJavaCharacterObject(env, in));
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
        return QtJambiAPI::convertQStringListToJavaObject(env,
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
        NativeType2_ptr_out result = qtjambi_jnitype_template1_cast<false,has_scope,jobject,QList,is_pointer,is_const,is_reference,QString>::cast(env, in, nullptr, scope);
        return *reinterpret_cast<NativeType_ptr*>(&result);
    }
};

}// namespace QtJambiPrivate

#endif // QTJAMBI_CAST_IMPL_JNITYPE_H
