/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

struct jcoreobject{
    jobject obj;
    inline jcoreobject(jobject _obj) : obj(_obj){}
    inline operator jobject(){return obj;}
    inline operator bool(){return obj;}
};

template<class O, class T, class = void, size_t = 0>
constexpr O qtjambi_cast(JNIEnv *env, T&& in);

namespace QtJambiPrivate {

template<typename... Args>
static constexpr bool unuseArgs(Args...){return true;}

template<typename T>
struct qtjambi_cast_types{
    typedef typename std::remove_reference<T>::type T_noref;
    typedef typename std::remove_cv<T_noref>::type T_noconst;
    typedef typename std::remove_cv<typename std::remove_pointer<T_noconst>::type>::type T_plain;
};

template<class O, class T, typename... Args>
struct qtjambi_cast;

template<typename O>
struct is_jni_type;

template<class T>
struct is_template : std::false_type{
};

template<template<typename...Ts> class C, typename...Ts>
struct is_template<C<Ts...>> : std::true_type{
};

template<typename O>
struct jni_type{
    static constexpr bool isValid = false;
    static constexpr bool isArray = false;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = false;
};

template<>
struct jni_type<jobject>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = false;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = false;
};

template<>
struct jni_type<jcoreobject>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = false;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = false;
};

template<>
struct jni_type<jthrowable>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = false;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = false;
};

template<>
struct jni_type<jstring>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = false;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = false;
};

template<>
struct jni_type<jclass>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = false;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = false;
};

template<>
struct jni_type<jarray>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = false;
};

template<>
struct jni_type<jobjectArray>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = false;
    static constexpr bool isIntegerArray = false;
    static constexpr bool isFloatingPointArray = false;
    typedef jobject ElementType;
};

template<>
struct jni_type<jintArray>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = true;
    static constexpr bool isIntegerArray = true;
    static constexpr bool isFloatingPointArray = false;
    static constexpr size_t primitiveSize = sizeof(jint);
    static constexpr auto converterFunction = QtJambiAPI::toJIntArray;
    static constexpr auto GetArrayRegion = &JNIEnv::GetIntArrayRegion;
    typedef jint ElementType;

    template<typename T, bool isConst = std::is_const<T>::value>
    struct NativeFactory{
        Q_STATIC_ASSERT_X(sizeof(T)==sizeof(jint), "T* cannot be cast to jint*");
        typedef typename std::conditional<isConst, PersistentJConstIntArrayPointer, PersistentJIntArrayPointer>::type PersistentConverter;
        typedef typename std::conditional<isConst, JConstIntArrayPointer, JIntArrayPointer>::type Converter;
    };
    template<bool persistent, typename T, bool isConst = std::is_const<T>::value>
    static constexpr auto getJavaFactory(){
        if constexpr(std::is_same_v<T, char32_t>){
            if constexpr(persistent){
                if constexpr(isConst){
                    PersistentConstChar32PointerArray* d{nullptr};
                    return d;
                }else{
                    PersistentChar32PointerArray* d{nullptr};
                    return d;
                }
            }else{
                if constexpr(isConst){
                    ConstChar32PointerArray* d{nullptr};
                    return d;
                }else{
                    Char32PointerArray* d{nullptr};
                    return d;
                }
            }
        }else{
            if constexpr(persistent){
                if constexpr(isConst){
                    if constexpr(std::is_unsigned<T>::value){
                        PersistentConstUInt32PointerArray* d{nullptr};
                        return d;
                    }else{
                        PersistentConstInt32PointerArray* d{nullptr};
                        return d;
                    }
                }else{
                    if constexpr(std::is_unsigned<T>::value){
                        PersistentUInt32PointerArray* d{nullptr};
                        return d;
                    }else{
                        PersistentInt32PointerArray* d{nullptr};
                        return d;
                    }
                }
            }else{
                if constexpr(isConst){
                    if constexpr(std::is_unsigned<T>::value){
                        ConstUInt32PointerArray* d{nullptr};
                        return d;
                    }else{
                        ConstInt32PointerArray* d{nullptr};
                        return d;
                    }
                }else{
                    if constexpr(std::is_unsigned<T>::value){
                        UInt32PointerArray* d{nullptr};
                        return d;
                    }else{
                        Int32PointerArray* d{nullptr};
                        return d;
                    }
                }
            }
        }
    }
    template<typename T, bool isConst = std::is_const<T>::value>
    struct JavaFactory{
        Q_STATIC_ASSERT_X(sizeof(T)==sizeof(jint), "T* cannot be cast to jint*");
        typedef std::remove_pointer_t<decltype(getJavaFactory<true,T,isConst>())> PersistentConverter;
        typedef std::remove_pointer_t<decltype(getJavaFactory<false,T,isConst>())> Converter;
    };
};

template<>
struct jni_type<jbyteArray>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = true;
    static constexpr bool isIntegerArray = true;
    static constexpr bool isFloatingPointArray = false;
    static constexpr size_t primitiveSize = sizeof(jbyte);
    static constexpr auto converterFunction = QtJambiAPI::toJByteArray;
    static constexpr auto GetArrayRegion = &JNIEnv::GetByteArrayRegion;
    typedef jbyte ElementType;

    template<typename T, bool isConst = std::is_const<T>::value>
    struct NativeFactory{
        Q_STATIC_ASSERT_X(sizeof(T)==sizeof(jbyte), "T* cannot be cast to jbyte*");
        typedef typename std::conditional<isConst, PersistentJConstByteArrayPointer, PersistentJByteArrayPointer>::type PersistentConverter;
        typedef typename std::conditional<isConst, JConstByteArrayPointer, JByteArrayPointer>::type Converter;
    };
    template<bool persistent, typename T, bool isConst = std::is_const<T>::value>
    static constexpr auto getJavaFactory(){
        if constexpr(std::is_same_v<T, char>
                      || std::is_same_v<T, uchar>
                      || std::is_same_v<T, QLatin1Char>
                      || std::is_same_v<T, std::byte>){
            if constexpr(persistent){
                if constexpr(isConst){
                    PersistentConstCharPointerArray* d{nullptr};
                    return d;
                }else{
                    PersistentCharPointerArray* d{nullptr};
                    return d;
                }
            }else{
                if constexpr(isConst){
                    ConstCharPointerArray* d{nullptr};
                    return d;
                }else{
                    CharPointerArray* d{nullptr};
                    return d;
                }
            }
        }else{
            if constexpr(persistent){
                if constexpr(isConst){
                    if constexpr(std::is_unsigned<T>::value){
                        PersistentConstUInt8PointerArray* d{nullptr};
                        return d;
                    }else{
                        PersistentConstInt8PointerArray* d{nullptr};
                        return d;
                    }
                }else{
                    if constexpr(std::is_unsigned<T>::value){
                        PersistentUInt8PointerArray* d{nullptr};
                        return d;
                    }else{
                        PersistentInt8PointerArray* d{nullptr};
                        return d;
                    }
                }
            }else{
                if constexpr(isConst){
                    if constexpr(std::is_unsigned<T>::value){
                        ConstUInt8PointerArray* d{nullptr};
                        return d;
                    }else{
                        ConstInt8PointerArray* d{nullptr};
                        return d;
                    }
                }else{
                    if constexpr(std::is_unsigned<T>::value){
                        UInt8PointerArray* d{nullptr};
                        return d;
                    }else{
                        Int8PointerArray* d{nullptr};
                        return d;
                    }
                }
            }
        }
    }
    template<typename T, bool isConst = std::is_const<T>::value>
    struct JavaFactory{
        Q_STATIC_ASSERT_X(sizeof(T)==sizeof(jbyte), "T* cannot be cast to jbyte*");
        typedef std::remove_pointer_t<decltype(getJavaFactory<true,T,isConst>())> PersistentConverter;
        typedef std::remove_pointer_t<decltype(getJavaFactory<false,T,isConst>())> Converter;
    };
};

template<>
struct jni_type<jshortArray>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = true;
    static constexpr bool isIntegerArray = true;
    static constexpr bool isFloatingPointArray = false;
    static constexpr size_t primitiveSize = sizeof(jshort);
    static constexpr auto converterFunction = QtJambiAPI::toJShortArray;
    static constexpr auto GetArrayRegion = &JNIEnv::GetShortArrayRegion;
    typedef jshort ElementType;

    template<typename T, bool isConst = std::is_const<T>::value>
    struct NativeFactory{
        Q_STATIC_ASSERT_X(sizeof(T)==sizeof(jshort), "T* cannot be cast to jshort*");
        typedef typename std::conditional<isConst, PersistentJConstShortArrayPointer, PersistentJShortArrayPointer>::type PersistentConverter;
        typedef typename std::conditional<isConst, JConstShortArrayPointer, JShortArrayPointer>::type Converter;
    };
    template<bool persistent, typename T, bool isConst = std::is_const<T>::value>
    static constexpr auto getJavaFactory(){
        if constexpr(persistent){
            if constexpr(isConst){
                if constexpr(std::is_unsigned<T>::value){
                    PersistentConstUInt16PointerArray* d{nullptr};
                    return d;
                }else{
                    PersistentConstInt16PointerArray* d{nullptr};
                    return d;
                }
            }else{
                if constexpr(std::is_unsigned<T>::value){
                    PersistentUInt16PointerArray* d{nullptr};
                    return d;
                }else{
                    PersistentInt16PointerArray* d{nullptr};
                    return d;
                }
            }
        }else{
            if constexpr(isConst){
                if constexpr(std::is_unsigned<T>::value){
                    ConstUInt16PointerArray* d{nullptr};
                    return d;
                }else{
                    ConstInt16PointerArray* d{nullptr};
                    return d;
                }
            }else{
                if constexpr(std::is_unsigned<T>::value){
                    UInt16PointerArray* d{nullptr};
                    return d;
                }else{
                    Int16PointerArray* d{nullptr};
                    return d;
                }
            }
        }
    }
    template<typename T, bool isConst = std::is_const<T>::value>
    struct JavaFactory{
        Q_STATIC_ASSERT_X(sizeof(T)==sizeof(jshort), "T* cannot be cast to jshort*");
        typedef std::remove_pointer_t<decltype(getJavaFactory<true,T,isConst>())> PersistentConverter;
        typedef std::remove_pointer_t<decltype(getJavaFactory<false,T,isConst>())> Converter;
    };
};

template<>
struct jni_type<jlongArray>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = true;
    static constexpr bool isIntegerArray = true;
    static constexpr bool isFloatingPointArray = false;
    static constexpr size_t primitiveSize = sizeof(jlong);
    static constexpr auto converterFunction = QtJambiAPI::toJLongArray;
    static constexpr auto GetArrayRegion = &JNIEnv::GetLongArrayRegion;
    typedef jlong ElementType;

    template<typename T, bool isConst = std::is_const<T>::value>
    struct NativeFactory{
        Q_STATIC_ASSERT_X(sizeof(T)==sizeof(jlong), "T* cannot be cast to jlong*");
        typedef typename std::conditional<isConst, PersistentJConstLongArrayPointer, PersistentJLongArrayPointer>::type PersistentConverter;
        typedef typename std::conditional<isConst, JConstLongArrayPointer, JLongArrayPointer>::type Converter;
    };
    template<bool persistent, typename T, bool isConst = std::is_const<T>::value>
    static constexpr auto getJavaFactory(){
        if constexpr(persistent){
            if constexpr(isConst){
                if constexpr(std::is_unsigned<T>::value){
                    PersistentConstUInt64PointerArray* d{nullptr};
                    return d;
                }else{
                    PersistentConstInt64PointerArray* d{nullptr};
                    return d;
                }
            }else{
                if constexpr(std::is_unsigned<T>::value){
                    PersistentUInt64PointerArray* d{nullptr};
                    return d;
                }else{
                    PersistentInt64PointerArray* d{nullptr};
                    return d;
                }
            }
        }else{
            if constexpr(isConst){
                if constexpr(std::is_unsigned<T>::value){
                    ConstUInt64PointerArray* d{nullptr};
                    return d;
                }else{
                    ConstInt64PointerArray* d{nullptr};
                    return d;
                }
            }else{
                if constexpr(std::is_unsigned<T>::value){
                    UInt64PointerArray* d{nullptr};
                    return d;
                }else{
                    Int64PointerArray* d{nullptr};
                    return d;
                }
            }
        }
    }
    template<typename T, bool isConst = std::is_const<T>::value>
    struct JavaFactory{
        Q_STATIC_ASSERT_X(sizeof(T)==sizeof(jlong), "T* cannot be cast to jlong*");
        typedef std::remove_pointer_t<decltype(getJavaFactory<true,T,isConst>())> PersistentConverter;
        typedef std::remove_pointer_t<decltype(getJavaFactory<false,T,isConst>())> Converter;
    };
};

template<>
struct jni_type<jcharArray>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = true;
    static constexpr bool isIntegerArray = false;
    static constexpr bool isFloatingPointArray = false;
    static constexpr size_t primitiveSize = sizeof(jchar);
    static constexpr auto converterFunction = QtJambiAPI::toJCharArray;
    static constexpr auto GetArrayRegion = &JNIEnv::GetCharArrayRegion;
    typedef jchar ElementType;

    template<typename T, bool isConst = std::is_const<T>::value>
    struct NativeFactory{
        Q_STATIC_ASSERT_X(sizeof(T)==sizeof(jchar), "T* cannot be cast to jchar*");
        typedef typename std::conditional<isConst, PersistentJConstCharArrayPointer, PersistentJCharArrayPointer>::type PersistentConverter;
        typedef typename std::conditional<isConst, JConstCharArrayPointer, JCharArrayPointer>::type Converter;
    };
    template<typename T, bool isConst = std::is_const<T>::value>
    struct JavaFactory{
        Q_STATIC_ASSERT_X(sizeof(T)==sizeof(jchar), "T* cannot be cast to jchar*");
        typedef typename std::conditional<std::is_same<T,QChar>::value,
                                          typename std::conditional<isConst, PersistentConstQCharPointerArray, PersistentQCharPointerArray>::type,
                                          typename std::conditional<std::is_same<T,char16_t>::value,
                                                                    typename std::conditional<isConst, PersistentConstChar16PointerArray, PersistentChar16PointerArray>::type,
                                                                    typename std::conditional<isConst, PersistentConstWCharPointerArray, PersistentWCharPointerArray>::type
                                                                    >::type
                                          >::type PersistentConverter;
        typedef typename std::conditional<std::is_same<T,QChar>::value,
                                          typename std::conditional<isConst, ConstQCharPointerArray, QCharPointerArray>::type,
                                          typename std::conditional<std::is_same<T,char16_t>::value,
                                                                    typename std::conditional<isConst, ConstChar16PointerArray, Char16PointerArray>::type,
                                                                    typename std::conditional<isConst, ConstWCharPointerArray, WCharPointerArray>::type
                                                                    >::type
                                          >::type Converter;
    };
};

template<>
struct jni_type<jbooleanArray>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = true;
    static constexpr bool isIntegerArray = false;
    static constexpr bool isFloatingPointArray = false;
    static constexpr size_t primitiveSize = sizeof(jboolean);
    static constexpr auto converterFunction = QtJambiAPI::toJBooleanArray;
    static constexpr auto GetArrayRegion = &JNIEnv::GetBooleanArrayRegion;
    typedef jboolean ElementType;

    template<typename T, bool isConst = std::is_const<T>::value>
    struct NativeFactory{
        Q_STATIC_ASSERT_X(sizeof(T)==sizeof(jboolean) || sizeof(T)==sizeof(bool), "T* cannot be cast to jboolean*");
        typedef typename std::conditional<isConst, PersistentJConstBooleanArrayPointer, PersistentJBooleanArrayPointer>::type PersistentConverter;
        typedef typename std::conditional<isConst, JConstBooleanArrayPointer, JBooleanArrayPointer>::type Converter;
    };
    template<typename T, bool isConst = std::is_const<T>::value>
    struct JavaFactory{
        Q_STATIC_ASSERT_X(sizeof(T)==sizeof(jboolean) || sizeof(T)==sizeof(bool), "T* cannot be cast to jboolean*");
        typedef typename std::conditional<isConst,
                                          typename std::conditional<std::is_same<T,uchar>::value, PersistentConstBool2PointerArray, PersistentConstBoolPointerArray>::type,
                                          typename std::conditional<std::is_same<T,uchar>::value, PersistentBool2PointerArray, PersistentBoolPointerArray>::type>::type PersistentConverter;
        typedef typename std::conditional<isConst,
                                          typename std::conditional<std::is_same<T,uchar>::value, ConstBool2PointerArray, ConstBoolPointerArray>::type,
                                          typename std::conditional<std::is_same<T,uchar>::value, Bool2PointerArray, BoolPointerArray>::type>::type Converter;
    };
};

template<>
struct jni_type<jdoubleArray>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = true;
    static constexpr bool isIntegerArray = false;
    static constexpr bool isFloatingPointArray = true;
    static constexpr size_t primitiveSize = sizeof(jdouble);
    static constexpr auto converterFunction = QtJambiAPI::toJDoubleArray;
    static constexpr auto GetArrayRegion = &JNIEnv::GetDoubleArrayRegion;
    typedef jdouble ElementType;

    template<typename T, bool isConst = std::is_const<T>::value>
    struct NativeFactory{
        Q_STATIC_ASSERT_X(sizeof(T)==sizeof(jdouble), "T* cannot be cast to jdouble*");
        typedef typename std::conditional<isConst, PersistentJConstDoubleArrayPointer, PersistentJDoubleArrayPointer>::type PersistentConverter;
        typedef typename std::conditional<isConst, JConstDoubleArrayPointer, JDoubleArrayPointer>::type Converter;
    };
    template<typename T, bool isConst = std::is_const<T>::value>
    struct JavaFactory{
        Q_STATIC_ASSERT_X(sizeof(T)==sizeof(jdouble), "T* cannot be cast to jdouble*");
        typedef typename std::conditional<isConst, PersistentConstDoublePointerArray, PersistentDoublePointerArray>::type PersistentConverter;
        typedef typename std::conditional<isConst, ConstDoublePointerArray, DoublePointerArray>::type Converter;
    };
};

template<>
struct jni_type<jfloatArray>{
    static constexpr bool isValid = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = true;
    static constexpr bool isFloatingPointArray = true;
    static constexpr size_t primitiveSize = sizeof(jfloat);
    static constexpr auto converterFunction = QtJambiAPI::toJFloatArray;
    static constexpr auto GetArrayRegion = &JNIEnv::GetFloatArrayRegion;
    typedef jfloat ElementType;

    template<typename T, bool isConst = std::is_const<T>::value>
    struct NativeFactory{
        Q_STATIC_ASSERT_X(sizeof(T)==sizeof(jfloat), "T* cannot be cast to jfloat*");
        typedef typename std::conditional<isConst, PersistentJConstFloatArrayPointer, PersistentJFloatArrayPointer>::type PersistentConverter;
        typedef typename std::conditional<isConst, JConstFloatArrayPointer, JFloatArrayPointer>::type Converter;
    };
    template<typename T, bool isConst = std::is_const<T>::value>
    struct JavaFactory{
        Q_STATIC_ASSERT_X(sizeof(T)==sizeof(jfloat), "T* cannot be cast to jfloat*");
        typedef typename std::conditional<isConst, PersistentConstFloatPointerArray, PersistentFloatPointerArray>::type PersistentConverter;
        typedef typename std::conditional<isConst, ConstFloatPointerArray, FloatPointerArray>::type Converter;
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

template<>
struct jni_type_decider_impl<char16_t, std::is_integral<char16_t>::value, std::is_floating_point<char16_t>::value, sizeof(jchar)>{
    static constexpr bool isPrimitive = true;
    typedef jchar JType;
    typedef jcharArray JArrayType;
    static constexpr auto readJavaOptional = QtJambiAPI::readJavaOptional;
    static constexpr auto newJavaOptional = QtJambiAPI::newJavaOptional;
};

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

template<typename... Args>
struct cast_var_args;

template<bool is_pointer, bool is_const, bool is_reference, class Type, bool has_default_ctor = is_default_constructible<Type>::value, bool has_copy_ctor = is_copy_constructible<Type>::value, bool has_move_ctor = is_move_constructible<Type>::value, typename... Args>
struct pointer_ref_or_clone_decider_impl{

    template<typename T>
    static Type convert(T, Args...){
        Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
    }
};

template<bool is_pointer, bool is_const, bool is_reference, class Type, typename... Args>
struct pointer_ref_or_clone_decider : pointer_ref_or_clone_decider_impl<is_pointer, is_const, is_reference, Type,
                                                                        is_default_constructible<Type>::value,
                                                                        is_copy_constructible<Type>::value,
                                                                        is_move_constructible<Type>::value, Args...>{

};

template<bool is_const, class Type, bool has_default_ctor, bool has_copy_ctor, bool has_move_ctor, typename... Args>
struct pointer_ref_or_clone_decider_impl</*is_pointer=*/true, is_const, /*is_reference=*/false, Type, has_default_ctor, has_copy_ctor, has_move_ctor, Args...>{
    typedef typename std::conditional<is_const, typename std::add_const<Type>::type, Type>::type Type_const;

    static Type* convert(std::nullptr_t, Args...){
        return nullptr;
    }
    static Type* convert(Type&& t, Args...){
        return &t;
    }
    static Type* convert(Type* t, Args...){
        return t;
    }
    static Type_const* convert(const Type* t, Args...){
        return t;
    }
};

template<bool is_const, class Type, bool has_default_ctor, bool has_move_ctor, typename... Args>
struct pointer_ref_or_clone_decider_impl</*is_pointer=*/true, is_const, /*is_reference=*/false, Type, has_default_ctor, /*has_copy_ctor=*/true, has_move_ctor, Args...>{

    static Type* convert(std::nullptr_t, Args...){
        return nullptr;
    }

    static Type* convert(Type&& t, Args...args){
        if constexpr(!cast_var_args<Args...>::hasScope){
            auto env = cast_var_args<Args...>::env(args...);
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast to %1* without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        }
        Type* result = new Type(std::move(t));
        cast_var_args<Args...>::scope(args...).addDeletion(result);
        return result;
    }

    static Type* convert(Type* t, Args...){
        return t;
    }

    static Type* convert(const Type* t, Args...args){
        if(t){
            if constexpr(!cast_var_args<Args...>::hasScope){
                auto env = cast_var_args<Args...>::env(args...);
                JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast to %1* without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
            }
            Type* result = new Type(*t);
            cast_var_args<Args...>::scope(args...).addDeletion(result);
            return result;
        }else{
            return nullptr;
        }
    }
};

template<bool is_const, class Type, bool has_default_ctor, bool has_move_ctor, typename... Args>
struct pointer_ref_or_clone_decider_impl</*is_pointer=*/true, is_const, /*is_reference=*/false, Type, has_default_ctor, /*has_copy_ctor=*/false, has_move_ctor, Args...>{

    static Type* convert(std::nullptr_t, Args...){
        return nullptr;
    }

    static Type* convert(Type&& t, Args...args){
        if constexpr(!cast_var_args<Args...>::hasScope){
            auto env = cast_var_args<Args...>::env(args...);
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast to %1* without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        }
        Type* result = new Type(std::move(t));
        cast_var_args<Args...>::scope(args...).addDeletion(result);
        return result;
    }

    static Type* convert(Type* t, Args...){
        return t;
    }

    static Type* convert(const Type*, Args...args){
        auto env = cast_var_args<Args...>::env(args...);
        JavaException::raiseError(env, QStringLiteral("Cannot cast to %1*.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<bool is_const, class Type, bool has_move_ctor, typename... Args>
struct pointer_ref_or_clone_decider_impl</*is_pointer=*/false, is_const, /*is_reference=*/false, Type, /*has_default_ctor=*/true, /*has_copy_ctor=*/true, has_move_ctor, Args...>{

    static Type convert(Type&& t, Args...){
        return std::move(t);
    }

    static Type convert(const Type* t, Args...){
        return t ? *t : Type();
    }
};

template<bool is_const, class Type, typename... Args>
struct pointer_ref_or_clone_decider_impl</*is_pointer=*/false, is_const, false, Type, /*has_default_ctor=*/true, false, true, Args...>{

    static Type convert(Type&& t, Args...){
        return Type(std::move(t));
    }

    static Type convert(Type* t, Args...){
        return t ? std::move(*t) : Type();
    }
};

template<class Type, bool has_copy_ctor, bool has_move_ctor, typename... Args>
struct pointer_ref_or_clone_decider_impl</*is_pointer=*/false, /*is_const=*/true, /*is_reference=*/true, Type, /*has_default_ctor=*/true, has_copy_ctor, has_move_ctor, Args...>{

    static const Type& convert(std::nullptr_t, Args...){
        return getDefaultValue<Type>();
    }

    static const Type& convert(Type&& t, Args...args){
        if constexpr(!cast_var_args<Args...>::hasScope){
            auto env = cast_var_args<Args...>::env(args...);
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast to const %1& without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        }
        Type* result = new Type(std::move(t));
        cast_var_args<Args...>::scope(args...).addDeletion(result);
        return *result;
    }

    static const Type& convert(const Type* t, Args...){
        return t ? *t : getDefaultValue<Type>();
    }
};

template<class Type, bool has_copy_ctor, bool has_move_ctor, typename... Args>
struct pointer_ref_or_clone_decider_impl</*is_pointer=*/false, /*is_const=*/true, /*is_reference=*/true, Type, /*has_default_ctor=*/false, has_copy_ctor, has_move_ctor, Args...>{

    static const Type& convert(Type&& t, Args...args){
        if constexpr(!cast_var_args<Args...>::hasScope){
            auto env = cast_var_args<Args...>::env(args...);
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast to const %1& without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        }
        Type* result = new Type(std::move(t));
        cast_var_args<Args...>::scope(args...).addDeletion(result);
        return *result;
    }

    static const Type& convert(const Type* t, Args...args){
        if(!t){
            auto env = cast_var_args<Args...>::env(args...);
            JavaException::raiseNullPointerException(env, QStringLiteral("Cannot cast nullptr to %1&.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        }
        return *t;
    }
};

template<class Type, bool has_copy_ctor, bool has_move_ctor, typename... Args>
struct pointer_ref_or_clone_decider_impl</*is_pointer=*/false, /*is_const=*/false, /*is_reference=*/true, Type, /*has_default_ctor*/true, has_copy_ctor, has_move_ctor, Args...>{

    static Type& convert(Type&& t, Args...args){
        if constexpr(!cast_var_args<Args...>::hasScope){
            auto env = cast_var_args<Args...>::env(args...);
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast to %1& without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        }
        Type* result = new Type(std::move(t));
        cast_var_args<Args...>::scope(args...).addDeletion(result);
        return *result;
    }

    static Type& convert(std::nullptr_t, Args...args){
        if constexpr(!cast_var_args<Args...>::hasScope){
            auto env = cast_var_args<Args...>::env(args...);
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast to %1& without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        }
        Type* result = new Type();
        cast_var_args<Args...>::scope(args...).addDeletion(result);
        return *result;
    }

    static Type& convert(Type* t, Args...args){
        if(t){
            return *t;
        }else{
            if constexpr(!cast_var_args<Args...>::hasScope){
                auto env = cast_var_args<Args...>::env(args...);
                JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast to %1& without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
            }
            Type* result = new Type();
            cast_var_args<Args...>::scope(args...).addDeletion(result);
            return *result;
        }
    }
};


template<class Type, bool has_copy_ctor, bool has_move_ctor, typename... Args>
struct pointer_ref_or_clone_decider_impl</*is_pointer=*/false, /*is_const=*/false, /*is_reference=*/true, Type, /*has_default_ctor=*/false, has_copy_ctor, has_move_ctor, Args...>{

    static Type& convert(Type&& t, Args...args){
        if constexpr(!cast_var_args<Args...>::hasScope){
            auto env = cast_var_args<Args...>::env(args...);
            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast to %1& without scope.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        }

        Type* result = new Type(std::move(t));
        cast_var_args<Args...>::scope(args...).addDeletion(result);
        return *result;
    }


    static Type& convert(std::nullptr_t, Args...args){
        auto env = cast_var_args<Args...>::env(args...);
        JavaException::raiseError(env, QStringLiteral("Cannot cast nullptr to %1&.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        throw "Cannot cast nullptr";
    }

    static Type& convert(Type* t, Args...args){
        if(!t){
            auto env = cast_var_args<Args...>::env(args...);
            JavaException::raiseNullPointerException(env, QStringLiteral("Cannot cast nullptr to %1&.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        }
        return *t;
    }


    static const Type& convert(const Type* t, Args...args){
        if(!t){
            auto env = cast_var_args<Args...>::env(args...);
            JavaException::raiseNullPointerException(env, QStringLiteral("Cannot cast nullptr to %1&.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        }
        return *t;
    }
};

template<class Type, bool has_default_ctor, bool has_copy_ctor, bool has_move_ctor, typename... Args>
struct pointer_ref_or_clone_decider_impl</*is_pointer=*/false, /*is_const=*/false, /*is_reference=*/true, Type, has_default_ctor, has_copy_ctor, has_move_ctor, Args...>{
    typedef Type Type_KT;

    static Type& convert(Type&& t, Args...){
        return t;
    }

    static Type& convert(Type* t, Args...args){
        if(!t){
            auto env = cast_var_args<Args...>::env(args...);
            JavaException::raiseNullPointerException(env, QStringLiteral("Cannot cast null to %1&.").arg(QLatin1String(QtJambiAPI::typeName(typeid(Type)))) QTJAMBI_STACKTRACEINFO );
        }
        return *t;
    }
};

template<typename K, typename... Args>
struct qtjambi_ownership_decider{
    static void setJavaOwnership(jobject o, K* qo, Args...args)
    {
        if constexpr(std::is_arithmetic<K>::value || std::is_enum<K>::value){
            Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
            Q_UNUSED(o)
            Q_UNUSED(qo)
        }else if constexpr(std::is_base_of<QObject, K>::value){
            auto env = cast_var_args<Args...>::env(args...);
            QtJambiAPI::setJavaOwnershipForTopLevelObject(env, qo);
        }else{
            auto env = cast_var_args<Args...>::env(args...);
            Q_UNUSED(qo)
            QtJambiAPI::setJavaOwnership(env, o);
        }
    }
    static void setCppOwnershipAndInvalidate(jobject o, K* qo, Args...args)
    {
        if constexpr(std::is_arithmetic<K>::value || std::is_enum<K>::value){
            Q_UNUSED(o)
            Q_UNUSED(qo)
            Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        }else if constexpr(std::is_base_of<QObject, K>::value){
            auto env = cast_var_args<Args...>::env(args...);
            QtJambiAPI::setCppOwnershipForTopLevelObject(env, qo);
        }else{
            auto env = cast_var_args<Args...>::env(args...);
            Q_UNUSED(qo)
            if constexpr(cast_var_args<Args...>::hasScope){
                QtJambiAPI::setCppOwnership(env, o);
                cast_var_args<Args...>::scope(args...).addObjectInvalidation(env, o);
            }else{
                Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
                QtJambiAPI::setCppOwnershipAndInvalidate(env, o);
            }
        }
    }
};

template<bool forward,
         typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jnitype_function_cast;

template<bool forward,
         typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jnitype_qobject_cast;

template<bool forward,
         typename JniType,
         typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_plain_cast;

template<typename T, typename... Args>
struct index_of;

template<typename T>
struct index_of<T> {
    static constexpr qint64 value = -1; // not found
};

template<typename T, typename U, typename... Args>
struct index_of<T, U, Args...> {
    static constexpr qint64 sub = index_of<T, Args...>::value;
    static constexpr qint64 value = std::is_same<std::conditional_t<std::is_array_v<std::remove_reference_t<T>>, std::decay<T>, T>, U>::value ? 0 : (sub == -1 ? -1 : 1 + sub);
};

template<size_t n>
struct argument_getter{
    template<typename T, typename... Args>
    static constexpr decltype(auto) get(T first, Args... rest) {
        if constexpr (n == 0)
            return first;
        else
            return argument_getter<n - 1>::get(rest...);
    }
};

template<typename... Args>
struct cast_var_args_name{
private:
    static constexpr qint64 index = index_of<const char*,Args...>::value;
public:
    static constexpr const char* nativeTypeName(Args... args) {
        if constexpr(index>=0){
            return argument_getter<index>::get(args...);
        }else{
            Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
            return nullptr;
        }
    }
    static constexpr bool hasNativeTypeName = index>=0;
};

template<size_t N,typename... Args>
struct cast_var_args_name<const char(&)[N],Args...>{
    static constexpr const char* nativeTypeName(const char(&nativeTypeName)[N], Args...) { return nativeTypeName; }
    static constexpr bool hasNativeTypeName = true;
};

template<typename A, size_t N,typename... Args>
struct cast_var_args_name<A,const char(&)[N],Args...>{
    static constexpr const char* nativeTypeName(A, const char(&nativeTypeName)[N], Args...) { return nativeTypeName; }
    static constexpr bool hasNativeTypeName = true;
};

template<typename A, typename B, size_t N,typename... Args>
struct cast_var_args_name<A,B,const char(&)[N],Args...>{
    static constexpr const char* nativeTypeName(A, B, const char(&nativeTypeName)[N], Args...) { return nativeTypeName; }
    static constexpr bool hasNativeTypeName = true;
};

template<typename... Args>
struct cast_var_args_scope{
private:
    static constexpr qint64 index = index_of<QtJambiScope&,Args...>::value;
    static constexpr qint64 ptr_index = index_of<QtJambiScope*,Args...>::value;
public:
    static constexpr QtJambiScope* scopePointer(Args... args) {
        if constexpr(ptr_index>=0){
            return argument_getter<ptr_index>::get(args...);
        }else if constexpr(index>=0){
            return &argument_getter<index>::get(args...);
        }else{
            Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
            return nullptr;
        }
    }
    static constexpr QtJambiScope& scope(Args... args) {
        if constexpr(ptr_index>=0){
            return *argument_getter<ptr_index>::get(args...);
        }else if constexpr(index>=0){
            return argument_getter<index>::get(args...);
        }else{
            return *scopePointer(args...);
        }
    }
    static constexpr QtJambiNativeID relatedNativeID(Args... args) {
        if constexpr(ptr_index>=0){
            QtJambiScope* scope = argument_getter<ptr_index>::get(args...);
            return scope ? scope->relatedNativeID() : InvalidNativeID;
        }else if constexpr(index>=0){
            return argument_getter<index>::get(args...).relatedNativeID();
        }else{
            Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
            return InvalidNativeID;
        }
    }
    static constexpr bool hasScope = ptr_index>=0 || index>=0;
};

template<typename... Args>
struct cast_var_args_scope<QtJambiScope&,Args...>{
    static constexpr QtJambiScope* scopePointer(QtJambiScope& _scope, Args...) { return &_scope; }
    static constexpr QtJambiScope& scope(QtJambiScope& _scope, Args...) { return _scope; }
    static constexpr QtJambiScope* scopePointer(QtJambiScope* _scope, Args...) { return _scope; }
    static constexpr QtJambiScope& scope(QtJambiScope* _scope, Args...) { return *_scope; }
    static QtJambiNativeID relatedNativeID(QtJambiScope* _scope, Args...) { return _scope ? _scope->relatedNativeID() : InvalidNativeID; }
    static QtJambiNativeID relatedNativeID(QtJambiScope& _scope, Args...) { return _scope.relatedNativeID(); }
    static constexpr bool hasScope = true;
};

template<typename A, typename... Args>
struct cast_var_args_scope<A,QtJambiScope&,Args...>{
    static constexpr QtJambiScope* scopePointer(A, QtJambiScope& _scope, Args...) { return &_scope; }
    static constexpr QtJambiScope& scope(A, QtJambiScope& _scope, Args...) { return _scope; }
    static constexpr QtJambiScope* scopePointer(A, QtJambiScope* _scope, Args...) { return _scope; }
    static constexpr QtJambiScope& scope(A, QtJambiScope* _scope, Args...) { return *_scope; }
    static QtJambiNativeID relatedNativeID(A, QtJambiScope* _scope, Args...) { return _scope ? _scope->relatedNativeID() : InvalidNativeID; }
    static QtJambiNativeID relatedNativeID(A, QtJambiScope& _scope, Args...) { return _scope.relatedNativeID(); }
    static constexpr bool hasScope = true;
};

template<typename A, typename B, typename... Args>
struct cast_var_args_scope<A,B,QtJambiScope&,Args...>{
    static constexpr QtJambiScope* scopePointer(A, B, QtJambiScope& _scope, Args...) { return &_scope; }
    static constexpr QtJambiScope& scope(A, B, QtJambiScope& _scope, Args...) { return _scope; }
    static constexpr QtJambiScope* scopePointer(A, B, QtJambiScope* _scope, Args...) { return _scope; }
    static constexpr QtJambiScope& scope(A, B, QtJambiScope* _scope, Args...) { return *_scope; }
    static QtJambiNativeID relatedNativeID(A, B, QtJambiScope* _scope, Args...) { return _scope ? _scope->relatedNativeID() : InvalidNativeID; }
    static QtJambiNativeID relatedNativeID(A, B, QtJambiScope& _scope, Args...) { return _scope.relatedNativeID(); }
    static constexpr bool hasScope = true;
};

template<typename... Args>
struct cast_var_args_env{
private:
    static constexpr qint64 index = index_of<JNIEnv*,Args...>::value;
public:
    static auto env(Args... args) {
        if constexpr(index>=0){
            return argument_getter<index>::get(args...);
        }else{
            Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
            return JniEnvironment{500};
        }
    }
    static constexpr bool hasJNIEnv = index>=0;
};

template<typename... Args>
struct cast_var_args_env<JNIEnv*,Args...>{
    static constexpr JNIEnv* env(JNIEnv* env,Args...) { return env; }
    static constexpr bool hasJNIEnv = true;
};

template<typename... Args>
struct cast_var_args : cast_var_args_name<Args...>, cast_var_args_scope<Args...>, cast_var_args_env<Args...>{
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_IMPL_UTIL_H
