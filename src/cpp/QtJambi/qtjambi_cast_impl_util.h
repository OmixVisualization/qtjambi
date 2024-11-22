/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBI_CAST_IMPL_UTIL_H
#define QTJAMBI_CAST_IMPL_UTIL_H

#include <type_traits>
#include <typeinfo>
#include <utility>
#include <memory>
#include "global.h"
#include "typetests.h"

class QtJambiScope;

namespace QtJambiPrivate {

template<typename T>
struct qtjambi_cast_types{
    typedef typename std::remove_reference<T>::type T_noref;
    typedef typename std::remove_cv<T_noref>::type T_noconst;
};

template<bool has_scope,
         typename O, bool o_is_arithmetic, bool o_is_enum, bool o_is_jni_type,
         typename T, bool t_is_arithmetic, bool t_is_enum, bool t_is_jni_type,
         bool is_same>
struct qtjambi_cast_decider;

template<typename O>
struct is_jni_type;

template<bool has_scope,class O, class T>
struct qtjambi_scoped_cast
        : qtjambi_cast_decider<has_scope,
        O,
        std::is_arithmetic<typename std::remove_reference<O>::type>::value,
        std::is_enum<typename std::remove_reference<O>::type>::value,
        is_jni_type<typename std::remove_cv<typename std::remove_reference<O>::type>::type>::value,
        T,
        std::is_arithmetic<typename std::remove_reference<T>::type>::value,
        std::is_enum<typename std::remove_reference<T>::type>::value,
        is_jni_type<typename std::remove_cv<typename std::remove_reference<T>::type>::type>::value,
        std::is_same<typename std::remove_reference<O>::type, typename std::remove_reference<T>::type>::value
        >
{
};

template<class T>
struct is_template : std::false_type{
};

template<template<typename...Ts> class C, typename...Ts>
struct is_template<C<Ts...>> : std::true_type{
};

template<typename O>
struct is_lightweight_java_type : std::false_type{
};

template<>
struct is_lightweight_java_type<QMetaObject::Connection> : std::true_type{
};

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#ifdef QABSTRACTITEMMODEL_H
template<>
struct is_lightweight_java_type<QModelRoleDataSpan> : std::true_type{
};
#endif
#endif

#ifdef QOBJECTWRAPPER_H
template<>
struct is_lightweight_java_type<JIteratorWrapper> : std::true_type{
};

template<>
struct is_lightweight_java_type<JCollectionWrapper> : std::true_type{
};

template<>
struct is_lightweight_java_type<JMapWrapper> : std::true_type{
};

template<>
struct is_lightweight_java_type<JObjectWrapper> : std::true_type{
};

template<>
struct is_lightweight_java_type<JEnumWrapper> : std::true_type{
};

template<>
struct is_lightweight_java_type<JObjectArrayWrapper> : std::true_type{
};

template<>
struct is_lightweight_java_type<JIntArrayWrapper> : std::true_type{
};

template<>
struct is_lightweight_java_type<JLongArrayWrapper> : std::true_type{
};

template<>
struct is_lightweight_java_type<JShortArrayWrapper> : std::true_type{
};

template<>
struct is_lightweight_java_type<JByteArrayWrapper> : std::true_type{
};

template<>
struct is_lightweight_java_type<JBooleanArrayWrapper> : std::true_type{
};

template<>
struct is_lightweight_java_type<JCharArrayWrapper> : std::true_type{
};

template<>
struct is_lightweight_java_type<JDoubleArrayWrapper> : std::true_type{
};

template<>
struct is_lightweight_java_type<JFloatArrayWrapper> : std::true_type{
};
#endif

template<>
struct is_lightweight_java_type<QString> : std::true_type{
};

template<>
struct is_lightweight_java_type<QVariant> : std::true_type{
};

template<typename O>
struct jni_type{
    static constexpr bool isValid = false;
    static constexpr bool isArray = false;
    static constexpr bool isPrimitiveArray = false;
};

template<>
struct jni_type<jobject>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = false;
    static constexpr bool isPrimitiveArray = false;
};

template<>
struct jni_type<jthrowable>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = false;
    static constexpr bool isPrimitiveArray = false;
};

template<>
struct jni_type<jstring>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = false;
    static constexpr bool isPrimitiveArray = false;
};

template<>
struct jni_type<jclass>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = false;
    static constexpr bool isPrimitiveArray = false;
};

template<>
struct jni_type<jarray>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitiveArray = false;
};

template<>
struct jni_type<jobjectArray>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitiveArray = false;
    static constexpr bool isIntegerArray = false;
    static constexpr bool isFloatingPointArray = false;
    typedef jobject ElementType;
};

template<>
struct jni_type<jintArray>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitiveArray = true;
    static constexpr bool isIntegerArray = true;
    static constexpr bool isFloatingPointArray = false;
    static constexpr size_t primitiveSize = sizeof(jint);
    static constexpr auto converterFunction = QtJambiAPI::toJIntArray;
    typedef jint ElementType;

    template<typename T, bool isConst = std::is_const<T>::value>
    struct NativeFactory{
    };
    template<typename T, bool isConst = std::is_const<T>::value, bool isUnsigned = std::is_unsigned<T>::value>
    struct JavaFactory{
    };
    template<typename T>
    struct NativeFactory<T,true>{
        typedef JConstIntArrayPointer type;
    };
    template<typename T>
    struct NativeFactory<T,false>{
        typedef JIntArrayPointer type;
    };
    template<typename T>
    struct JavaFactory<T,true,false>{
        typedef ConstInt32PointerArray type;
    };
    template<typename T>
    struct JavaFactory<T,true,true>{
        typedef ConstUInt32PointerArray type;
    };
    template<typename T>
    struct JavaFactory<T,false,false>{
        typedef Int32PointerArray type;
    };
    template<typename T>
    struct JavaFactory<T,false,true>{
        typedef UInt32PointerArray type;
    };
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    template<bool isUnsigned>
    struct JavaFactory<char32_t,true,isUnsigned>{
        typedef ConstChar32PointerArray type;
    };
    template<bool isUnsigned>
    struct JavaFactory<char32_t,false,isUnsigned>{
        typedef Char32PointerArray type;
    };
#endif
};

template<>
struct jni_type<jbyteArray>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitiveArray = true;
    static constexpr bool isIntegerArray = true;
    static constexpr bool isFloatingPointArray = false;
    static constexpr size_t primitiveSize = sizeof(jbyte);
    static constexpr auto converterFunction = QtJambiAPI::toJByteArray;
    typedef jbyte ElementType;

    template<typename T, bool isConst = std::is_const<T>::value>
    struct NativeFactory{
    };
    template<typename T, bool isConst = std::is_const<T>::value, bool isUnsigned = std::is_unsigned<T>::value>
    struct JavaFactory{
    };
    template<typename T>
    struct NativeFactory<T,true>{
        typedef JConstByteArrayPointer type;
    };
    template<typename T>
    struct NativeFactory<T,false>{
        typedef JByteArrayPointer type;
    };
    template<typename T>
    struct JavaFactory<T,true,false>{
        typedef typename std::conditional<std::is_same<T,char>::value, ConstCharPointerArray, ConstInt8PointerArray>::type type;
    };
    template<typename T>
    struct JavaFactory<T,true,true>{
        typedef ConstUInt8PointerArray type;
    };
    template<typename T>
    struct JavaFactory<T,false,false>{
        typedef typename std::conditional<std::is_same<T,char>::value, CharPointerArray, Int8PointerArray>::type type;
    };
    template<typename T>
    struct JavaFactory<T,false,true>{
        typedef UInt8PointerArray type;
    };
};

template<>
struct jni_type<jshortArray>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitiveArray = true;
    static constexpr bool isIntegerArray = true;
    static constexpr bool isFloatingPointArray = false;
    static constexpr size_t primitiveSize = sizeof(jshort);
    static constexpr auto converterFunction = QtJambiAPI::toJShortArray;
    typedef jshort ElementType;

    template<typename T, bool isConst = std::is_const<T>::value>
    struct NativeFactory{
    };
    template<typename T, bool isConst = std::is_const<T>::value, bool isUnsigned = std::is_unsigned<T>::value>
    struct JavaFactory{
    };
    template<typename T>
    struct NativeFactory<T,true>{
        typedef JConstShortArrayPointer type;
    };
    template<typename T>
    struct NativeFactory<T,false>{
        typedef JShortArrayPointer type;
    };
    template<typename T>
    struct JavaFactory<T,true,false>{
        typedef ConstInt16PointerArray type;
    };
    template<typename T>
    struct JavaFactory<T,true,true>{
        typedef ConstUInt16PointerArray type;
    };
    template<typename T>
    struct JavaFactory<T,false,false>{
        typedef Int16PointerArray type;
    };
    template<typename T>
    struct JavaFactory<T,false,true>{
        typedef UInt16PointerArray type;
    };
};

template<>
struct jni_type<jlongArray>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitiveArray = true;
    static constexpr bool isIntegerArray = true;
    static constexpr bool isFloatingPointArray = false;
    static constexpr size_t primitiveSize = sizeof(jlong);
    static constexpr auto converterFunction = QtJambiAPI::toJLongArray;
    typedef jlong ElementType;

    template<typename T, bool isConst = std::is_const<T>::value>
    struct NativeFactory{
    };
    template<typename T, bool isConst = std::is_const<T>::value, bool isUnsigned = std::is_unsigned<T>::value>
    struct JavaFactory{
    };
    template<typename T>
    struct NativeFactory<T,true>{
        typedef JConstLongArrayPointer type;
    };
    template<typename T>
    struct NativeFactory<T,false>{
        typedef JLongArrayPointer type;
    };
    template<typename T>
    struct JavaFactory<T,true,false>{
        typedef ConstInt64PointerArray type;
    };
    template<typename T>
    struct JavaFactory<T,true,true>{
        typedef ConstUInt64PointerArray type;
    };
    template<typename T>
    struct JavaFactory<T,false,false>{
        typedef Int64PointerArray type;
    };
    template<typename T>
    struct JavaFactory<T,false,true>{
        typedef UInt64PointerArray type;
    };
};

template<>
struct jni_type<jcharArray>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitiveArray = true;
    static constexpr bool isIntegerArray = false;
    static constexpr bool isFloatingPointArray = false;
    static constexpr size_t primitiveSize = sizeof(jchar);
    static constexpr auto converterFunction = QtJambiAPI::toJCharArray;
    typedef jchar ElementType;

    template<typename T, bool isConst = std::is_const<T>::value>
    struct NativeFactory{
    };
    template<typename T, bool isConst = std::is_const<T>::value>
    struct JavaFactory{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        typedef typename std::conditional<std::is_same<T,QChar>::value,
                                          typename std::conditional<isConst, ConstQCharPointerArray, QCharPointerArray>::type,
                                          typename std::conditional<std::is_same<T,char16_t>::value,
                                                                    typename std::conditional<isConst, ConstChar16PointerArray, Char16PointerArray>::type,
                                                                    typename std::conditional<isConst, ConstWCharPointerArray, WCharPointerArray>::type
                                                                    >::type
                                          >::type type;
#else
        typedef typename std::conditional<std::is_same<T,QChar>::value,
                                          typename std::conditional<isConst, ConstQCharPointerArray, QCharPointerArray>::type,
                                          typename std::conditional<isConst, ConstWCharPointerArray, WCharPointerArray>::type>::type type;
#endif
    };
    template<typename T>
    struct NativeFactory<T,true>{
        typedef JConstCharArrayPointer type;
    };
    template<typename T>
    struct NativeFactory<T,false>{
        typedef JCharArrayPointer type;
    };
};

template<>
struct jni_type<jbooleanArray>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitiveArray = true;
    static constexpr bool isIntegerArray = false;
    static constexpr bool isFloatingPointArray = false;
    static constexpr size_t primitiveSize = sizeof(jboolean);
    static constexpr auto converterFunction = QtJambiAPI::toJBooleanArray;
    typedef jboolean ElementType;

    template<typename T, bool isConst = std::is_const<T>::value>
    struct NativeFactory{
    };
    template<typename T, bool isConst = std::is_const<T>::value>
    struct JavaFactory{
    };
    template<typename T>
    struct NativeFactory<T,true>{
        typedef JConstBooleanArrayPointer type;
    };
    template<typename T>
    struct NativeFactory<T,false>{
        typedef JBooleanArrayPointer type;
    };
    template<typename T>
    struct JavaFactory<T,true>{
        typedef typename std::conditional<std::is_same<T,uchar>::value, ConstBool2PointerArray, ConstBoolPointerArray>::type type;
    };
    template<typename T>
    struct JavaFactory<T,false>{
        typedef typename std::conditional<std::is_same<T,uchar>::value, Bool2PointerArray, BoolPointerArray>::type type;
    };
};

template<>
struct jni_type<jdoubleArray>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitiveArray = true;
    static constexpr bool isIntegerArray = false;
    static constexpr bool isFloatingPointArray = true;
    static constexpr size_t primitiveSize = sizeof(jdouble);
    static constexpr auto converterFunction = QtJambiAPI::toJDoubleArray;
    typedef jdouble ElementType;

    template<typename T, bool isConst = std::is_const<T>::value>
    struct NativeFactory{
    };
    template<typename T, bool isConst = std::is_const<T>::value>
    struct JavaFactory{
    };
    template<typename T>
    struct NativeFactory<T,true>{
        typedef JConstDoubleArrayPointer type;
    };
    template<typename T>
    struct NativeFactory<T,false>{
        typedef JDoubleArrayPointer type;
    };
    template<typename T>
    struct JavaFactory<T,true>{
        typedef ConstDoublePointerArray type;
    };
    template<typename T>
    struct JavaFactory<T,false>{
        typedef DoublePointerArray type;
    };
};

template<>
struct jni_type<jfloatArray>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitiveArray = true;
    static constexpr bool isFloatingPointArray = true;
    static constexpr size_t primitiveSize = sizeof(jfloat);
    static constexpr auto converterFunction = QtJambiAPI::toJFloatArray;
    typedef jfloat ElementType;

    template<typename T, bool isConst = std::is_const<T>::value>
    struct NativeFactory{
    };
    template<typename T, bool isConst = std::is_const<T>::value>
    struct JavaFactory{
    };
    template<typename T>
    struct NativeFactory<T,true>{
        typedef JConstFloatArrayPointer type;
    };
    template<typename T>
    struct NativeFactory<T,false>{
        typedef JFloatArrayPointer type;
    };
    template<typename T>
    struct JavaFactory<T,true>{
        typedef ConstFloatPointerArray type;
    };
    template<typename T>
    struct JavaFactory<T,false>{
        typedef FloatPointerArray type;
    };
};

template<typename O>
struct is_jni_array_type : std::conditional<jni_type<O>::isArray, std::true_type, std::false_type>::type {
};

template<typename O>
struct is_jni_type : std::conditional<jni_type<O>::isValid, std::true_type, std::false_type>::type {
};

template<typename T, bool isIntegral = std::is_integral<T>::value, bool isFP = std::is_floating_point<T>::value, size_t size = sizeof(T)>
struct jni_type_decider_impl{
    static constexpr bool isPrimitive = false;
    typedef jobject JType;
    typedef jobjectArray JArrayType;
    static constexpr auto readJavaOptional = QtJambiAPI::readJavaOptional;
    static constexpr auto newJavaOptional = QtJambiAPI::newJavaOptional;
};

template<typename T>
struct jni_type_decider : jni_type_decider_impl<typename std::remove_cv<T>::type>{
};

template<>
struct jni_type_decider_impl<bool, std::is_integral<bool>::value, std::is_floating_point<bool>::value, sizeof(bool)>{
    static constexpr bool isPrimitive = true;
    typedef jboolean JType;
    typedef jbooleanArray JArrayType;
    static constexpr auto readJavaOptional = QtJambiAPI::readJavaOptional;
    static constexpr auto newJavaOptional = QtJambiAPI::newJavaOptional;
};

template<>
struct jni_type_decider_impl<QChar, std::is_integral<QChar>::value, std::is_floating_point<QChar>::value, sizeof(jchar)>{
    static constexpr bool isPrimitive = true;
    typedef jchar JType;
    typedef jcharArray JArrayType;
    static constexpr auto readJavaOptional = QtJambiAPI::readJavaOptional;
    static constexpr auto newJavaOptional = QtJambiAPI::newJavaOptional;
};

template<>
struct jni_type_decider_impl<wchar_t, std::is_integral<wchar_t>::value, std::is_floating_point<wchar_t>::value, sizeof(jchar)>{
    static constexpr bool isPrimitive = true;
    typedef jchar JType;
    typedef jcharArray JArrayType;
    static constexpr auto readJavaOptional = QtJambiAPI::readJavaOptional;
    static constexpr auto newJavaOptional = QtJambiAPI::newJavaOptional;
};

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
template<>
struct jni_type_decider_impl<char16_t, std::is_integral<char16_t>::value, std::is_floating_point<char16_t>::value, sizeof(jchar)>{
    static constexpr bool isPrimitive = true;
    typedef jchar JType;
    typedef jcharArray JArrayType;
    static constexpr auto readJavaOptional = QtJambiAPI::readJavaOptional;
    static constexpr auto newJavaOptional = QtJambiAPI::newJavaOptional;
};
#endif

template<typename T>
struct jni_type_decider_impl<T, true, false, sizeof(jbyte)>{
    static constexpr bool isPrimitive = true;
    typedef jbyte JType;
    typedef jbyteArray JArrayType;
    static constexpr auto readJavaOptional = QtJambiAPI::readJavaOptional;
    static constexpr auto newJavaOptional = QtJambiAPI::newJavaOptional;
};

template<typename T>
struct jni_type_decider_impl<T, true, false, sizeof(jshort)>{
    static constexpr bool isPrimitive = true;
    typedef jshort JType;
    typedef jshortArray JArrayType;
    static constexpr auto readJavaOptional = QtJambiAPI::readJavaOptional;
    static constexpr auto newJavaOptional = QtJambiAPI::newJavaOptional;
};

template<typename T>
struct jni_type_decider_impl<T, true, false, sizeof(jint)>{
    static constexpr bool isPrimitive = true;
    typedef jint JType;
    typedef jintArray JArrayType;
    static constexpr auto readJavaOptional = QtJambiAPI::readJavaOptionalInt;
    static constexpr auto newJavaOptional = QtJambiAPI::newJavaOptionalInt;
};

template<typename T>
struct jni_type_decider_impl<T, true, false, sizeof(jlong)>{
    typedef jlong JType;
    typedef jlongArray JArrayType;
    static constexpr auto readJavaOptional = QtJambiAPI::readJavaOptionalLong;
    static constexpr auto newJavaOptional = QtJambiAPI::newJavaOptionalLong;
};

template<typename T>
struct jni_type_decider_impl<T, false, true, sizeof(jdouble)>{
    static constexpr bool isPrimitive = true;
    typedef jdouble JType;
    typedef jdoubleArray JArrayType;
    static constexpr auto readJavaOptional = QtJambiAPI::readJavaOptionalDouble;
    static constexpr auto newJavaOptional = QtJambiAPI::newJavaOptionalDouble;
};

template<typename T>
struct jni_type_decider_impl<T, false, true, sizeof(jfloat)>{
    static constexpr bool isPrimitive = true;
    typedef jfloat JType;
    typedef jfloatArray JArrayType;
    static constexpr auto readJavaOptional = QtJambiAPI::readJavaOptional;
    static constexpr auto newJavaOptional = QtJambiAPI::newJavaOptional;
};

template<bool is_reference,typename O>
struct ptr2ref{
};

template<typename O>
struct ptr2ref<false,O>{
    static constexpr O* value(JNIEnv *, O* o){
        return o;
    }
};

template<typename O>
struct ptr2ref<true,O>{
    static constexpr auto value = &QtJambiAPI::checkedAddressOf<O>;
};

template<bool is_pointer,typename O>
struct deref_ptr{
};

template<typename O>
struct deref_ptr<false,O>{
    static constexpr O& deref(O& o){
        return o;
    }
};

template<typename O>
struct deref_ptr<true,O>{
    static O& deref(O* o){
        Q_ASSERT(o);
        return *o;
    }
};

template<bool is_pointer,typename O>
struct ref_ptr{
};

template<typename O>
struct ref_ptr<true,O>{
    static constexpr O* ref(O* o){
        return o;
    }
};

template<typename O>
struct ref_ptr<false,O>{
    static constexpr O* ref(O& o){
        return &o;
    }
};

template<bool is_pointer,typename O>
struct deref_ptr_or_default{
};

template<typename O>
struct deref_ptr_or_default<false,O>{
    static constexpr O& deref(O& o){
        return o;
    }
};

template<typename O>
struct deref_ptr_or_default<true,O>{
    typedef typename std::add_const<O>::type O_const;
    static O_const& deref(O_const* o){
        return o ? *o : getDefaultValue<typename std::remove_const<O>::type>();
    }
};

template<typename O, bool has_std_constructor, bool has_copy_constructor, bool is_const, bool o_is_reference>
struct qtjambi_deref_value{
    Q_STATIC_ASSERT_X(!has_std_constructor && false, "Cannot deref type");
    typedef typename std::conditional<is_const, typename std::add_const<O>::type, O>::type O_const;
    static O_const& deref(JNIEnv *env, O* o){
        return QtJambiAPI::checkedAddressOf(env, o);
    }
};

template<typename O, bool is_const>
struct qtjambi_deref_value<O,false, true, is_const, false>{
    typedef typename std::conditional<is_const, typename std::add_const<O>::type, O>::type O_const;
    static O_const deref(JNIEnv *env, O* o){
        return O(QtJambiAPI::checkedAddressOf(env, o));
    }
};

template<typename O, bool has_copy_constructor>
struct qtjambi_deref_value<O,false/*has_std_constructor*/, has_copy_constructor, false/*is_const*/, true>{
    static constexpr auto deref = &QtJambiAPI::checkedAddressOf<O>;
};

template<typename O, bool has_copy_constructor>
struct qtjambi_deref_value<O,false/*has_std_constructor*/, has_copy_constructor, true/*is_const*/, true>{
    typedef typename std::add_const<O>::type O_const;
    static O_const& deref(JNIEnv *env, O* o){
        return QtJambiAPI::checkedAddressOf(env, o);
    }
};

template<typename O, bool has_copy_constructor>
struct qtjambi_deref_value<O,true/*has_std_constructor*/, has_copy_constructor,true/*is_const*/,true>{
    typedef typename std::add_const<O>::type O_const;
    static O_const& deref(JNIEnv *, O_const* o){
        return o ? *o : getDefaultValue<typename std::remove_const<O>::type>();
    }
};

template<typename O, bool has_copy_constructor>
struct qtjambi_deref_value<O,true/*has_std_constructor*/,has_copy_constructor,false/*is_const*/,true>{
    static constexpr auto deref = &QtJambiAPI::checkedAddressOf<O>;
};

template<typename O, bool is_const>
struct qtjambi_deref_value<O,true,true,is_const,false>{
    typedef typename std::conditional<is_const, typename std::add_const<O>::type, O>::type O_const;
    static O deref(JNIEnv *, O_const* o){
        return o ? O(*o) : O();
    }
};

template<bool is_pointer, bool is_const, bool is_reference, bool has_scope, class Type, bool has_default_ctor = is_default_constructible<Type>::value, bool has_copy_ctor = is_copy_constructible<Type>::value, bool has_move_ctor = is_move_constructible<Type>::value>
struct pointer_ref_or_clone_decider{
    static Type convert(JNIEnv *,...){
        Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
    }
};

template<bool is_const, class Type, bool has_default_ctor, bool has_copy_ctor, bool has_move_ctor>
struct pointer_ref_or_clone_decider</*is_pointer=*/true, is_const, /*is_reference=*/false, /*has_scope=*/false, Type, has_default_ctor, has_copy_ctor, has_move_ctor>{
    typedef typename std::conditional<is_const, typename std::add_const<Type>::type, Type>::type Type_const;
    static Type* convert(JNIEnv *, QtJambiScope*, std::nullptr_t){
        return nullptr;
    }
    static Type* convert(JNIEnv *, QtJambiScope*, Type&& t){
        Q_STATIC_ASSERT_X(false && !is_const, "Cannot cast to pointer without scope.");
        return &t;
    }
    static Type* convert(JNIEnv *, QtJambiScope*, Type* t){
        return t;
    }
    static Type_const* convert(JNIEnv *, QtJambiScope*, const Type* t){
        Q_STATIC_ASSERT_X(is_const, "Cannot cast const pointer to non-const pointer without scope.");
        return t;
    }
};

template<bool is_const, class Type, bool has_default_ctor, bool has_move_ctor>
struct pointer_ref_or_clone_decider</*is_pointer=*/true, is_const, /*is_reference=*/false, /*has_scope=*/true, Type, has_default_ctor, /*has_copy_ctor=*/true, has_move_ctor>{
    static Type* convert(JNIEnv *, QtJambiScope*, std::nullptr_t){
        return nullptr;
    }
    static Type* convert(JNIEnv *env, QtJambiScope* scope, Type&& t){
        if(!scope)
            JavaException::raiseError(env, QStringLiteral("Cannot cast to %1* without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        Type* result = new Type(std::move(t));
        scope->addDeletion(result);
        return result;
    }
    static Type* convert(JNIEnv *, QtJambiScope*, Type* t){
        return t;
    }
    static Type* convert(JNIEnv *env, QtJambiScope* scope, const Type* t){
        if(t){
            if(!scope)
                JavaException::raiseError(env, QStringLiteral("Cannot cast to %1* without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
            Type* result = new Type(*t);
            scope->addDeletion(result);
            return result;
        }else{
            return nullptr;
        }
    }
};

template<bool is_const, class Type, bool has_default_ctor, bool has_move_ctor>
struct pointer_ref_or_clone_decider</*is_pointer=*/true, is_const, /*is_reference=*/false, /*has_scope=*/true, Type, has_default_ctor, /*has_copy_ctor=*/false, has_move_ctor>{
    static Type* convert(JNIEnv *, QtJambiScope*, std::nullptr_t){
        return nullptr;
    }
    static Type* convert(JNIEnv *env, QtJambiScope* scope, Type&& t){
        if(!scope)
            JavaException::raiseError(env, QStringLiteral("Cannot cast to %1* without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        Type* result = new Type(std::move(t));
        scope->addDeletion(result);
        return result;
    }
    static Type* convert(JNIEnv *, QtJambiScope*, Type* t){
        return t;
    }
    static Type* convert(JNIEnv *env, QtJambiScope*, const Type*){
        JavaException::raiseError(env, QStringLiteral("Cannot cast to %1*.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<bool is_const, bool buffer, class Type, bool has_move_ctor>
struct pointer_ref_or_clone_decider</*is_pointer=*/false, is_const, /*is_reference=*/false, buffer, Type, /*has_default_ctor=*/true, /*has_copy_ctor=*/true, has_move_ctor>{
    static Type convert(JNIEnv *, QtJambiScope*, Type&& t){
        return std::move(t);
    }
    static Type convert(JNIEnv *, QtJambiScope*, const Type* t){
        return t ? *t : Type();
    }
};

template<bool is_const, bool has_scope, class Type>
struct pointer_ref_or_clone_decider</*is_pointer=*/false, is_const, false, has_scope, Type, /*has_default_ctor=*/true, false, true>{
    static Type convert(JNIEnv *, QtJambiScope*, Type&& t){
        return Type(std::move(t));
    }
    static Type convert(JNIEnv *, QtJambiScope*, Type* t){
        return t ? std::move(*t) : Type();
    }
};

template<bool has_scope, class Type, bool has_copy_ctor, bool has_move_ctor>
struct pointer_ref_or_clone_decider</*is_pointer=*/false, /*is_const=*/true, /*is_reference=*/true, has_scope, Type, /*has_default_ctor=*/true, has_copy_ctor, has_move_ctor>{
    static const Type& convert(JNIEnv *, QtJambiScope*, std::nullptr_t){
        return getDefaultValue<Type>();
    }
    static const Type& convert(JNIEnv *env, QtJambiScope* scope, Type&& t){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast to const T& without scope.");
        if(!scope)
            JavaException::raiseError(env, QStringLiteral("Cannot cast to %1* without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        Type* result = new Type(std::move(t));
        scope->addDeletion(result);
        return *result;
    }
    static const Type& convert(JNIEnv *, QtJambiScope*, const Type* t){
        return t ? *t : getDefaultValue<Type>();
    }
};

template<bool has_scope, class Type, bool has_copy_ctor, bool has_move_ctor>
struct pointer_ref_or_clone_decider</*is_pointer=*/false, /*is_const=*/true, /*is_reference=*/true, has_scope, Type, /*has_default_ctor=*/false, has_copy_ctor, has_move_ctor>{
    static const Type& convert(JNIEnv *env, QtJambiScope* scope, Type&& t){
        Q_STATIC_ASSERT_X(has_scope, "Cannot cast to const T& without scope.");
        if(!scope)
            JavaException::raiseError(env, QStringLiteral("Cannot cast to %1* without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        Type* result = new Type(std::move(t));
        scope->addDeletion(result);
        return *result;
    }
    static const Type& convert(JNIEnv *env, QtJambiScope*, const Type* t){
        if(!t)
            JavaException::raiseNullPointerException(env, QStringLiteral("Cannot cast nullptr to %1&.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        return *t;
    }
};

template<class Type, bool has_copy_ctor, bool has_move_ctor>
struct pointer_ref_or_clone_decider</*is_pointer=*/false, /*is_const=*/false, /*is_reference=*/true, /*has_scope=*/true, Type, /*has_default_ctor*/true, has_copy_ctor, has_move_ctor>{
    static Type& convert(JNIEnv *env, QtJambiScope* scope, Type&& t){
        if(!scope)
            JavaException::raiseError(env, QStringLiteral("Cannot cast to %1* without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        Type* result = new Type(std::move(t));
        scope->addDeletion(result);
        return *result;
    }
    static Type& convert(JNIEnv * env, QtJambiScope* scope, std::nullptr_t){
        if(!scope)
            JavaException::raiseError(env, QStringLiteral("Cannot cast to %1& without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        Type* result = new Type();
        scope->addDeletion(result);
        return *result;
    }
    static Type& convert(JNIEnv *env, QtJambiScope* scope, Type* t){
        if(t){
            return *t;
        }else{
            if(!scope)
                JavaException::raiseError(env, QStringLiteral("Cannot cast to %1& without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
            Type* result = new Type();
            scope->addDeletion(result);
            return *result;
        }
    }
};


template<class Type, bool has_copy_ctor, bool has_move_ctor>
struct pointer_ref_or_clone_decider</*is_pointer=*/false, /*is_const=*/false, /*is_reference=*/true, /*has_scope=*/true, Type, /*has_default_ctor=*/false, has_copy_ctor, has_move_ctor>{
    static Type& convert(JNIEnv *env, QtJambiScope* scope, Type&& t){
        if(!scope)
            JavaException::raiseError(env, QStringLiteral("Cannot cast to %1* without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        Type* result = new Type(std::move(t));
        scope->addDeletion(result);
        return *result;
    }

    static Type& convert(JNIEnv * env, QtJambiScope*, std::nullptr_t){
        JavaException::raiseError(env, QStringLiteral("Cannot cast nullptr to %1&.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        throw "Cannot cast nullptr";
    }
    static Type& convert(JNIEnv *env, QtJambiScope*, Type* t){
        if(!t)
            JavaException::raiseNullPointerException(env, QStringLiteral("Cannot cast nullptr to %1&.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        return *t;
    }

    static const Type& convert(JNIEnv *env, QtJambiScope*, const Type* t){
        if(!t)
            JavaException::raiseNullPointerException(env, QStringLiteral("Cannot cast nullptr to %1&.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        return *t;
    }
};

template<class Type, bool has_default_ctor, bool has_copy_ctor, bool has_move_ctor>
struct pointer_ref_or_clone_decider</*is_pointer=*/false, /*is_const=*/false, /*is_reference=*/true, /*has_scope=*/false, Type, has_default_ctor, has_copy_ctor, has_move_ctor>{
    typedef Type Type_KT;
    static Type& convert(JNIEnv *, QtJambiScope*, Type&& t){
        Q_STATIC_ASSERT_X(false && !has_default_ctor, "Cannot cast to Type reference without scope.");
        return t;
    }
    static Type& convert(JNIEnv *env, QtJambiScope*, Type* t){
        if(!t)
            JavaException::raiseNullPointerException(env, QStringLiteral("Cannot cast null to %1&.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        return *t;
    }
};

template<typename NativeType, bool is_const, bool is_movable = std::is_move_constructible<NativeType>::value>
struct qtjambi_move_or_copy_decider{
    constexpr static jobject cast(JNIEnv * env, const NativeType& in, const char* nativeTypeName, QtJambiScope*){
        return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &in, typeid(NativeType), nativeTypeName);
    }
};

template<typename NativeType>
struct qtjambi_move_or_copy_decider<NativeType,false,true>{
    static jobject cast(JNIEnv * env, NativeType& in, const char* nativeTypeName, QtJambiScope*){
        NativeType* ptr = new NativeType(std::move(in));
        try{
            jobject out = QtJambiAPI::convertNativeToJavaOwnedObjectAsWrapper(env, ptr, typeid(NativeType), nativeTypeName);
            if(!out)
                delete ptr;
            return out;
        }catch(const JavaException& exn){
            delete ptr;
            exn.raise();
            return nullptr;
        }
    }
};

template<typename K, bool isQObject =
#ifdef QOBJECT_H
                     std::is_base_of<QObject, K>::value
#else
                     false
#endif
         , bool isDisabled = std::is_arithmetic<K>::value || std::is_enum<K>::value
         >
struct qtjambi_ownership_decider{
    static void setJavaOwnership(JNIEnv *env, jobject o, K*)
    {
        QtJambiAPI::setJavaOwnership(env, o);
    }

    static void setCppOwnershipAndInvalidate(JNIEnv *env, jobject o, K*, QtJambiScope* scope)
    {
        if(scope){
            QtJambiAPI::setCppOwnership(env, o);
            scope->addObjectInvalidation(env, o);
        }else{
            QtJambiAPI::setCppOwnershipAndInvalidate(env, o);
        }
    }
};

template<typename K>
struct qtjambi_ownership_decider<K,false,true>{
    static void setJavaOwnership(JNIEnv *, jobject, K*){}

    static void setCppOwnershipAndInvalidate(JNIEnv *, jobject, K*, QtJambiScope*){}
};

template<typename K>
struct qtjambi_ownership_decider<K,true,false>{
    static void setJavaOwnership(JNIEnv *env, jobject, K* qo)
    {
        QtJambiAPI::setJavaOwnershipForTopLevelObject(env, qo);
    }

    static void setCppOwnershipAndInvalidate(JNIEnv *env, jobject, K* qo, QtJambiScope*)
    {
        QtJambiAPI::setCppOwnershipForTopLevelObject(env, qo);
    }
};

template<typename Out>
struct qtjambi_construct_from_argument{
    typedef Out create;
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_IMPL_UTIL_H
