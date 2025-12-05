/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

template<bool forward, typename JniType, typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_arithmetic_cast{
    Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
    static void cast(...){}
};

template<bool forward,
         typename JniType, typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_function_cast{
    Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
    static void cast(...){}
};

template<bool forward, class JniType, typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_string_cast;

template<bool forward, typename JniType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename Args, template<typename... Ts> class NativeType, typename... Ts>
static constexpr auto qtjambi_jobject_template_cast_impl(const NativeType<Ts...>&);

template<bool forward, typename JniType, typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
static constexpr auto qtjambi_jobject_cast_impl() {
    if constexpr(is_template<NativeType>::value){
        return decltype(qtjambi_jobject_template_cast_impl<forward, JniType, is_pointer, is_const, is_reference, is_rvalue, std::tuple<Args...>>(std::declval<const NativeType&>())){};
    }else if constexpr(std::is_arithmetic<NativeType>::value
                         || std::is_same_v<NativeType, QChar>
                         || std::is_same_v<NativeType, QLatin1Char>
                         || std::is_same_v<NativeType, std::byte>){
        return qtjambi_jobject_arithmetic_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>{};
    }else if constexpr(std::is_function_v<NativeType>){
        return qtjambi_jobject_function_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>{};
#ifdef QOBJECT_H
    }else if constexpr(std::is_base_of_v<QObject, NativeType>){
        return qtjambi_jnitype_qobject_cast<forward, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>{};
#endif
    }else if constexpr(std::is_same_v<JniType, jstring>
                         || std::is_same_v<NativeType, QString>
                         || std::is_same_v<NativeType, QStringView>
                         || std::is_same_v<NativeType, QAnyStringView>
                         || std::is_same_v<NativeType, QUtf8StringView>
                         || std::is_same_v<NativeType, QLatin1String>){
        return qtjambi_string_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>{};
    }else{
        return qtjambi_jobject_plain_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>{};
    }
}

template<bool forward, class JniType, class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_cast : decltype(qtjambi_jobject_cast_impl<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>()){
};

template<bool forward,
         typename JniType, typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_plain_cast{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;
    typedef typename std::conditional<forward, NativeType_in, JniType>::type In;
    typedef typename std::conditional<forward, JniType, NativeType_out>::type Out;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(!std::is_same_v<JniType, jobject>
            && !(std::is_same_v<JniType, jstring> && (std::is_same_v<NativeType, QByteArray>
                                                      || std::is_same_v<NativeType, QByteArrayView>))){
            if constexpr(std::is_same_v<JniType, jobjectArray> || std::is_same_v<JniType, jbyteArray>){
                if constexpr(forward){
                    return value_range_converter<JniType, char, true, JniType, NativeType_c, Args...>::toJavaArray(ref_ptr<is_pointer, NativeType_c>::ref(in), arg_pointer<Args>::ref(args)...);
                }else{
                    return value_range_converter<JniType, char, true, JniType, NativeType, Args...>::toNativeContainer(in, -1, arg_pointer<Args>::ref(args)...);
                }
            }else{
                Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
            }
        }else if constexpr(forward){
            if constexpr(std::is_enum_v<NativeType>){
                if constexpr(is_pointer){
                    return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, in, typeid(NativeType));
                }else{
                    return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &in, typeid(NativeType));
                }
#if defined(QTJAMBI_JOBJECTWRAPPER_H)
            }else if constexpr(std::is_base_of_v<JObjectWrapper, NativeType>){
                if constexpr(is_pointer){
                    return JniType(in->object(env));
                }else{
                    return JniType(in.object(env));
                }
#endif //defined(QTJAMBI_JOBJECTWRAPPER_H)
            }
            if constexpr(is_pointer){
                if constexpr(std::is_polymorphic<NativeType>::value){
                    jobject o = nullptr;
                    if(in){
                        try{
                            QtJambiAPI::checkDanglingPointer(env, in);
                            // check rtti availability:
                            if(QtJambiPrivate::CheckPointer<NativeType>::trySupplyType(in)!=nullptr)
                                o = QtJambiShellInterface::getJavaObjectLocalRef(env, dynamic_cast<const QtJambiShellInterface*>(in));
                        }catch(...){}
                    }
                    if(!o){
                        if constexpr(cast_var_args<Args...>::hasScope)
                            o = QtJambiAPI::convertNativeToJavaObjectAsWrapperAndInvalidateAfterUse(env, cast_var_args<Args...>::scope(args...), in, typeid(NativeType));
                        else
                           o = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, in, typeid(NativeType));
                    }
                    return o;
                }else{
                    if constexpr(cast_var_args<Args...>::hasScope)
                        return QtJambiAPI::convertNativeToJavaObjectAsWrapperAndInvalidateAfterUse(env, cast_var_args<Args...>::scope(args...), in, typeid(NativeType));
                    else
                        return QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, in, typeid(NativeType));
                }
            }else if constexpr(is_rvalue && !is_const && std::is_move_constructible<NativeType>::value){
                NativeType* ptr = new NativeType(std::move(in));
                try{
                    jobject out = QtJambiAPI::convertNativeToJavaOwnedObjectAsWrapper(env, ptr, typeid(NativeType));
                    if(!out)
                        delete ptr;
                    return out;
                }catch(const JavaException& exn){
                    delete ptr;
                    exn.raise();
                    return nullptr;
                }
            }else if constexpr(!std::is_copy_constructible<NativeType>::value || (is_reference && !is_rvalue && !is_const && cast_var_args<Args...>::hasScope)){
                if constexpr(cast_var_args<Args...>::hasScope)
                    return QtJambiAPI::convertNativeToJavaObjectAsWrapperAndInvalidateAfterUse(env, cast_var_args<Args...>::scope(args...), &in, typeid(NativeType));
                else
                    return QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, &in, typeid(NativeType));
            }else{
                return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &in, typeid(NativeType));
            }
        }else{
            if constexpr(std::is_enum_v<NativeType>){
                Q_STATIC_ASSERT_X(!is_reference || is_const, "Cannot cast to non-const enum reference");
                Q_STATIC_ASSERT_X(!is_pointer || is_const, "Cannot cast to non-const enum pointer");
                if constexpr(is_pointer){
                    if(!in)
                        return nullptr;
                }
                NativeType result(NativeType(0));
                if(!QtJambiAPI::convertJavaToNative(env, in, &result, typeid(NativeType))){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null"), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                }
                return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(std::move(result), args...);
#if defined(QTJAMBI_JOBJECTWRAPPER_H)
            }else if constexpr(std::is_base_of_v<JObjectWrapper, NativeType>){
                Q_STATIC_ASSERT_X(!is_reference || is_const, "Cannot cast to non-const JObjectWrapper&");
                Q_STATIC_ASSERT_X(!is_pointer || is_const, "Cannot cast to non-const JObjectWrapper*");
                if constexpr(is_pointer){
                    if(!in)
                        return nullptr;
                }
                NativeType result;
                result.assign(env, in);
                return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(std::move(result), args...);
#endif //defined(QTJAMBI_JOBJECTWRAPPER_H)
            }else{
                NativeType_ptr result = nullptr;
                if(!QtJambiAPI::convertJavaToNative(env, in, &result, typeid(NativeType))){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null"), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                }
                if constexpr(is_pointer){
                    return result;
                }else{
                    if constexpr(!std::is_default_constructible<NativeType>::value || (is_reference && !is_const)){
                        if(!result)
                            JavaException::raiseNullPointerException(env, QStringLiteral("Cannot cast null to reference type %1").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                    }
                    return qtjambi_deref_value<NativeType, is_default_constructible<NativeType>::value, is_copy_constructible<NativeType>::value, is_const, is_reference>::deref(env, result);
                }
            }
        }
    }
};

template<bool forward,
         typename JniType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename Args, size_t P, typename... Ts>
struct qtjambi_jobject_template_cast{
};

template<bool forward, typename JniType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename Args, template<typename... Ts> class NativeType, typename... Ts>
static constexpr auto qtjambi_jobject_template_cast_impl(const NativeType<Ts...>&){
    return qtjambi_jobject_template_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args, sizeof...(Ts), Ts...>{};
}

template<bool forward,
         typename JniType,
         template<typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename T, typename... Args>
struct qtjambi_jobject_template_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, std::tuple<Args...>, 1, T>
 : qtjambi_jobject_template1_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, T, Args...>{
};

template<bool forward,
         typename JniType,
         template<typename T, typename K> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename T, typename K, typename... Args>
struct qtjambi_jobject_template_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, std::tuple<Args...>, 2, T, K>
 : qtjambi_jobject_template2_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, T, K, Args...>{
};

template<bool forward,
         typename JniType,
         template<typename T, typename K, typename L> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename T, typename K, typename L, typename... Args>
struct qtjambi_jobject_template_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, std::tuple<Args...>, 3, T, K, L>
 : qtjambi_jobject_template3_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, T, K, L, Args...>{
};

template<bool forward,
         typename JniType,
         template<typename T, typename K, typename L, typename M> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename T, typename K, typename L, typename M, typename... Args>
struct qtjambi_jobject_template_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, std::tuple<Args...>, 4, T, K, L, M>
 : qtjambi_jobject_template4_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, T, K, L, M, Args...>{
};

template<bool forward,
         typename JniType,
         template<typename T, typename K, typename L, typename M, typename N> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename T, typename K, typename L, typename M, typename N, typename... Args>
struct qtjambi_jobject_template_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, std::tuple<Args...>, 5, T, K, L, M, N>
 : qtjambi_jobject_template5_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, T, K, L, M, N, Args...>{
};

#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0) //&& defined(QSPAN_H)
template<bool forward,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, std::size_t E, typename... Args>
struct qtjambi_jobject_plain_cast<forward,
                              JniType,
                              QSpan<T,E>, is_pointer, is_const, is_reference, is_rvalue, Args...>
    : qtjambi_jobject_span_cast<forward, JniType, is_pointer, is_const, is_reference, is_rvalue, typename std::remove_cv<T>::type, E, std::is_const<T>::value, Args...>{
};

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, std::size_t E, typename... Args>
struct qtjambi_jobject_plain_cast<forward,
                              jobject,
                              QSpan<T,E>, is_pointer, is_const, is_reference, is_rvalue, Args...>
    : qtjambi_jobject_span_cast<forward, jobject, is_pointer, is_const, is_reference, is_rvalue, typename std::remove_cv<T>::type, E, std::is_const<T>::value, Args...>{
};
#endif // QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)

template<bool forward,
         typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_function_cast<forward,
                                     jobject, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;
    typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
    typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(forward){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
            if constexpr(is_pointer){
                return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, reinterpret_cast<const void*>(in), typeid(NativeType_ptr), cast_var_args<Args...>::nativeTypeName(args...));
            }else{
                return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, reinterpret_cast<const void*>(&in), typeid(NativeType_ptr), cast_var_args<Args...>::nativeTypeName(args...));
            }
        }else{
            Q_STATIC_ASSERT_X(!is_reference || is_const, "Cannot cast to non-const function pointer reference");
            NativeType_ptr result = nullptr;
            if(!QtJambiAPI::convertJavaToNative(env, in, &result, typeid(NativeType_ptr), cast_var_args<Args...>::nativeTypeName(args...))){
                if constexpr(cast_var_args<Args...>::hasNativeTypeName){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null"), cast_var_args<Args...>::nativeTypeName(args...)) QTJAMBI_STACKTRACEINFO );
                }else{
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null"), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                }
            }
            if constexpr(is_pointer){
                return result;
            }else{
                return qtjambi_deref_value<NativeType, is_default_constructible<NativeType>::value, is_copy_constructible<NativeType>::value, is_const, is_reference>::deref(env, result);
            }
        }
    }
};

template<bool forward,
         typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jnitype_qobject_cast{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;
    typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
    typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;
    Q_STATIC_ASSERT_X(is_reference || is_pointer, "Cannot cast to QObject without pointer or reference");

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(forward){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
            if constexpr(is_pointer){
                return QtJambiAPI::convertQObjectToJavaObject(env, in, typeid(NativeType));
            }else{
                return QtJambiAPI::convertQObjectToJavaObject(env, &in, typeid(NativeType));
            }
        }else{
            NativeType_ptr result = nullptr;
            if(!QtJambiAPI::convertJavaToNative(env, in, &result, typeid(NativeType))){
                JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null"), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
            }
            if constexpr(is_pointer){
                return result;
            }else{
                return qtjambi_deref_value<NativeType, is_default_constructible<NativeType>::value, is_copy_constructible<NativeType>::value, is_const, is_reference>::deref(env, result);
            }
        }
    }
};

template<bool forward,
         typename JniType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_plain_cast<forward, JniType, QVariant, is_pointer, is_const, is_reference, is_rvalue, Args...>{
    typedef QVariant NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;
    typedef typename std::conditional<forward, NativeType_in, JniType>::type In;
    typedef typename std::conditional<forward, JniType, NativeType_out>::type Out;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(forward){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
            if constexpr(std::is_same_v<JniType, jcoreobject>){
                if constexpr(is_pointer){
                    if constexpr(is_const){
                        return in ? QtJambiAPI::convertQVariantToJavaVariant(env, *in) : nullptr;
                    }else{
                        if constexpr(cast_var_args<Args...>::hasScope){
                            return QtJambiAPI::convertQVariantToJavaVariantAndInvalidateAfterUse(env, cast_var_args<Args...>::scope(args...), in);
                        }else{
                            return QtJambiAPI::convertQVariantToJavaVariant(env, in);
                        }
                    }
                }else if constexpr(is_rvalue){
                    return QtJambiAPI::convertQVariantToJavaVariant(env, std::move(in));
                }else if constexpr(is_reference && !is_const){
                    if constexpr(cast_var_args<Args...>::hasScope){
                        return QtJambiAPI::convertQVariantToJavaVariantAndInvalidateAfterUse(env, cast_var_args<Args...>::scope(args...), &in);
                    }else{
                        return QtJambiAPI::convertQVariantToJavaVariant(env, &in);
                    }
                }else
                    return QtJambiAPI::convertQVariantToJavaVariant(env, in);
            }else{
                if constexpr(is_pointer){
                    return in ? QtJambiAPI::convertQVariantToJavaObject(env, *in) : nullptr;
                }else{
                    return QtJambiAPI::convertQVariantToJavaObject(env, in);
                }
            }
        }else{
            if(QtJambiAPI::isQVariantObject(env, in)){
                if constexpr(is_pointer){
                    return QtJambiAPI::convertJavaObjectToNative<QVariant>(env, in);
                }else{
                    return QtJambiAPI::convertJavaObjectToNativeReference<QVariant>(env, in);
                }
            }
            if constexpr(cast_var_args<Args...>::hasScope){
                if constexpr(is_pointer || is_reference){
                    NativeType* result = new NativeType(QtJambiAPI::convertJavaObjectToQVariant(env, in));
                    cast_var_args<Args...>::scope(args...).addDeletion(result);
                    if constexpr(is_pointer){
                        return result;
                    }else{
                        return qtjambi_deref_value<NativeType, is_default_constructible<NativeType>::value, is_copy_constructible<NativeType>::value, is_const, is_reference>::deref(env, result);
                    }
                }
            }
            if constexpr(!is_pointer && !is_reference){
                return QtJambiAPI::convertJavaObjectToQVariant(env, in);
            }else{
                if constexpr(is_reference){
                    Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast to reference without scope.");
                    throw;
                }else return nullptr;
            }
        }
    }
};

#ifdef QABSTRACTITEMMODEL_H

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_plain_cast<forward, jobject, QModelIndex, is_pointer, is_const, is_reference, is_rvalue, Args...>{
    typedef QModelIndex NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;
    typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
    typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(forward){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
            if constexpr(is_pointer){
                if constexpr(cast_var_args<Args...>::hasScope)
                    return QtJambiAPI::convertNativeToJavaObjectAsWrapperAndInvalidateAfterUse(env, cast_var_args<Args...>::scope(args...), in, typeid(NativeType));
                else
                    return QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, in, typeid(NativeType));
            }else if constexpr(is_rvalue){
                return QtJambiAPI::convertNativeToJavaOwnedObjectAsWrapper(env, new QModelIndex(std::move(in)));
            }else if constexpr(is_reference && !is_const && cast_var_args<Args...>::hasScope){
                return QtJambiAPI::convertNativeToJavaObjectAsWrapperAndInvalidateAfterUse(env, cast_var_args<Args...>::scope(args...), &in, typeid(NativeType));
            }else{
                return QtJambiAPI::convertNativeToJavaObjectAsCopy(env, &in, typeid(NativeType));
            }
        }else{
            if constexpr(is_pointer || is_reference){
                QModelIndex* result = nullptr;
                if(!QtJambiAPI::convertJavaToModelIndex(env, in, result
#if defined(QTJAMBI_LIGHTWEIGHT_MODELINDEX)
                                                         , cast_var_args<Args...>::scope(args...)
#endif
                                                        )){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null"), QLatin1String(QtJambiAPI::typeName(typeid(QModelIndex)))) QTJAMBI_STACKTRACEINFO );
                }
                if constexpr(is_pointer){
                    return result;
                }else if constexpr(is_reference && !is_const){
                    if(!result)
                        JavaException::raiseNullPointerException(env, QStringLiteral("Cannot cast null to reference type %1").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                    return *result;
                }else{
                    return qtjambi_deref_value<NativeType, is_default_constructible<NativeType>::value, is_copy_constructible<NativeType>::value, is_const, is_reference>::deref(env, result);
                }
            }else{
                QModelIndex result;
                if(!QtJambiAPI::convertJavaToModelIndex(env, in, result)){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null"), QLatin1String(QtJambiAPI::typeName(typeid(QModelIndex)))) QTJAMBI_STACKTRACEINFO );
                }
                return result;
            }
        }
    }
};

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_plain_cast<forward, jobject, QModelRoleDataSpan, is_pointer, is_const, is_reference, is_rvalue, Args...>{
    typedef QModelRoleDataSpan NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;
    typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
    typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;

    static Out cast(In in, Args... args){
        if constexpr(forward){
            Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
            QMap<int,QVariant> map;
            for(const QModelRoleData& data : deref_ptr<is_pointer,const QModelRoleDataSpan>::deref(in)){
                map[data.role()] = data.data();
            }
            return qtjambi_cast<jobject, const QMap<int,QVariant>&, Args...>::cast(map, args...);
        }else{
            auto env = cast_var_args<Args...>::env(args...);
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast to QModelRoleDataSpan without scope.");
            void *data(nullptr);
            qsizetype length(0);
            QtJambiAPI::convertJavaObjectToQModelRoleData(env, cast_var_args<Args...>::scope(args...), in, data, length);
            if constexpr(is_pointer || is_reference){
                QModelRoleDataSpan* result = new QModelRoleDataSpan(reinterpret_cast<QModelRoleData*>(data), length);
                cast_var_args<Args...>::scope(args...).addDeletion(result);
                if constexpr(is_pointer){
                    return result;
                }else if constexpr(is_reference && !is_const){
                    if(!result)
                        JavaException::raiseNullPointerException(env, QStringLiteral("Cannot cast null to reference type %1").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                    return *result;
                }else{
                    return qtjambi_deref_value<NativeType, is_default_constructible<NativeType>::value, is_copy_constructible<NativeType>::value, is_const, is_reference>::deref(env, result);
                }
            }else{
                return QModelRoleDataSpan(reinterpret_cast<QModelRoleData*>(data), length);
            }
        }
    }
};

#endif // def QABSTRACTITEMMODEL_H

// template for jstring

template<class JniType>
struct qtjambi_to_jstring{
    static constexpr jstring cast(JNIEnv* env, JniType in){
        return QtJambiAPI::toJavaString(env, in);
    }
};

template<>
struct qtjambi_to_jstring<jstring>{
    static constexpr jstring cast(JNIEnv*, jstring in){
        return in;
    }
};

template<bool forward, class JniType, typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_string_cast{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<forward, NativeType_in, JniType>::type In;
    typedef typename std::conditional<forward, JniType, NativeType_out>::type Out;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(forward){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
            if constexpr(std::is_same_v<JniType, jcoreobject> && std::is_same_v<NativeType, QString>){
                if constexpr(is_pointer){
                    if constexpr(is_const){
                        return in ? QtJambiAPI::convertQStringToJavaObject(env, *in) : nullptr;
                    }else{
                        if constexpr(cast_var_args<Args...>::hasScope){
                            return QtJambiAPI::convertQStringToJavaObjectAndInvalidateAfterUse(env, cast_var_args<Args...>::scope(args...), in);
                        }else{
                            return QtJambiAPI::convertQStringToJavaObject(env, in);
                        }
                    }
                }else if constexpr(is_rvalue){
                    return QtJambiAPI::convertQStringToJavaObject(env, std::move(in));
                }else if constexpr(is_reference && !is_const){
                    if constexpr(cast_var_args<Args...>::hasScope){
                        return QtJambiAPI::convertQStringToJavaObjectAndInvalidateAfterUse(env, cast_var_args<Args...>::scope(args...), &in);
                    }else{
                        return QtJambiAPI::convertQStringToJavaObject(env, &in);
                    }
                }else
                    return QtJambiAPI::convertQStringToJavaObject(env, in);
            }else{
                if constexpr(std::is_same_v<NativeType, QString>
                              || std::is_same_v<NativeType, QStringView>){
                    if constexpr(std::is_same_v<JniType, jcharArray>){
                        if constexpr(is_pointer){
                            if(in){
                                ConstQCharPointerArray pointerArray(env, in->data(), jsize(in->length()));
                                JavaException::check(env QTJAMBI_STACKTRACEINFO );
                                return pointerArray.array();
                            }else
                                return nullptr;
                        }else{
                            ConstQCharPointerArray pointerArray(env, in.data(), jsize(in.length()));
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            return pointerArray.array();
                        }
                    }else{
                        if constexpr(is_pointer){
                            if(in){
                                jstring str = env->NewString(reinterpret_cast<const jchar *>(in->data()), jsize(in->length()));
                                JavaException::check(env QTJAMBI_STACKTRACEINFO );
                                return str;
                            }else
                                return nullptr;
                        }else{
                            jstring str = env->NewString(reinterpret_cast<const jchar *>(in.data()), jsize(in.length()));
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            return str;
                        }
                    }
                }else if constexpr(std::is_same_v<NativeType, QAnyStringView>){
                    if constexpr(is_pointer){
                        if(in){
                            return in->visit([env](auto text){
                                if(std::is_same_v<decltype(text), QStringView>){
                                    if constexpr(std::is_same_v<JniType, jcharArray>){
                                        ConstQCharPointerArray pointerArray(env, text.data(), jsize(text.length()));
                                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                                        return pointerArray.array();
                                    }else if constexpr(std::is_same_v<JniType, jbyteArray>){
                                        QByteArray b(text.toUtf8());
                                        ConstCharPointerArray pointerArray(env, b.data(), jsize(b.length()));
                                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                                        return pointerArray.array();
                                    }else{
                                        jstring str = env->NewString(reinterpret_cast<const jchar *>(text.data()), jsize(text.length()));
                                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                                        return str;
                                    }
                                }else{
                                    if constexpr(std::is_same_v<JniType, jcharArray>){
                                        QString s(text);
                                        ConstQCharPointerArray pointerArray(env, s.data(), jsize(s.length()));
                                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                                        return pointerArray.array();
                                    }else if constexpr(std::is_same_v<JniType, jbyteArray>){
                                        ConstCharPointerArray pointerArray(env, text.data(), jsize(text.length()));
                                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                                        return pointerArray.array();
                                    }else{
                                        const char * data = reinterpret_cast<const char *>(text.data());
                                        jstring str;
                                        if(decltype(in->length())(qstrlen(data))==text.length())
                                            str = env->NewStringUTF(data);
                                        else{
                                            QString strg = text.toString();
                                            str = env->NewString(reinterpret_cast<const jchar *>(strg.data()), jsize(strg.length()));
                                        }
                                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                                        return str;
                                    }
                                }
                            });
                        }else
                            return nullptr;
                    }else{
                        return in.visit([env](auto text){
                            if(std::is_same_v<decltype(text), QStringView>){
                                if constexpr(std::is_same_v<JniType, jcharArray>){
                                    ConstQCharPointerArray pointerArray(env, text.data(), jsize(text.length()));
                                    JavaException::check(env QTJAMBI_STACKTRACEINFO );
                                    return pointerArray.array();
                                }else if constexpr(std::is_same_v<JniType, jbyteArray>){
                                    QByteArray b(text.toUtf8());
                                    ConstCharPointerArray pointerArray(env, b.data(), jsize(b.length()));
                                    JavaException::check(env QTJAMBI_STACKTRACEINFO );
                                    return pointerArray.array();
                                }else{
                                    jstring str = env->NewString(reinterpret_cast<const jchar *>(text.data()), jsize(text.length()));
                                    JavaException::check(env QTJAMBI_STACKTRACEINFO );
                                    return str;
                                }
                            }else{
                                if constexpr(std::is_same_v<JniType, jcharArray>){
                                    QString s(text);
                                    ConstQCharPointerArray pointerArray(env, s.data(), jsize(s.length()));
                                    JavaException::check(env QTJAMBI_STACKTRACEINFO );
                                    return pointerArray.array();
                                }else if constexpr(std::is_same_v<JniType, jbyteArray>){
                                    ConstCharPointerArray pointerArray(env, text.data(), jsize(text.length()));
                                    JavaException::check(env QTJAMBI_STACKTRACEINFO );
                                    return pointerArray.array();
                                }else{
                                    const char * data = reinterpret_cast<const char *>(text.data());
                                    jstring str;
                                    if(decltype(text.length())(qstrlen(data))==text.length())
                                        str = env->NewStringUTF(data);
                                    else{
                                        QString strg = text.toString();
                                        str = env->NewString(reinterpret_cast<const jchar *>(strg.data()), jsize(strg.length()));
                                    }
                                    JavaException::check(env QTJAMBI_STACKTRACEINFO );
                                    return str;
                                }
                            }
                        });
                    }
                }else if constexpr(std::is_same_v<NativeType, QLatin1String>
                                    || std::is_same_v<NativeType, QUtf8StringView>
                                     || std::is_same_v<NativeType, QByteArrayView>
                                     || std::is_same_v<NativeType, QByteArray>){
                    if constexpr(std::is_same_v<JniType, jbyteArray>){
                        if constexpr(is_pointer){
                            if(in){
                                ConstCharPointerArray pointerArray(env, reinterpret_cast<const char *>(in->data()), jsize(in->length()));
                                JavaException::check(env QTJAMBI_STACKTRACEINFO );
                                return pointerArray.array();
                            }else
                                return nullptr;
                        }else{
                            ConstCharPointerArray pointerArray(env, reinterpret_cast<const char *>(in.data()), jsize(in.length()));
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            return pointerArray.array();
                        }
                    }else if constexpr(is_pointer){
                        if(in){
                            const char * data = reinterpret_cast<const char *>(in->data());
                            jstring str;
                            if(decltype(in->length())(qstrlen(data))==in->length())
                                str = env->NewStringUTF(data);
                            else{
                                QString strg;
                                if constexpr(std::is_same_v<NativeType, QByteArrayView>
                                              || std::is_same_v<NativeType, QByteArray>){
                                    strg = QString::fromLatin1(*in);
                                }else{
                                    strg = in->toString();
                                }
                                str = env->NewString(reinterpret_cast<const jchar *>(strg.data()), jsize(strg.length()));
                            }
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            return str;
                        }else
                            return nullptr;
                    }else{
                        const char * data = reinterpret_cast<const char *>(in.data());
                        jstring str;
                        if(decltype(in.length())(qstrlen(data))==in.length())
                            str = env->NewStringUTF(data);
                        else{
                            QString strg;
                            if constexpr(std::is_same_v<NativeType, QByteArrayView>
                                          || std::is_same_v<NativeType, QByteArray>){
                                strg = QString::fromLatin1(in);
                            }else{
                                strg = in.toString();
                            }
                            str = env->NewString(reinterpret_cast<const jchar *>(strg.data()), jsize(strg.length()));
                        }
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        return str;
                    }
                }else if constexpr(std::is_same_v<NativeType, QAnyStringView>){
                    if constexpr(is_pointer){
                        if(in){
                            return QtJambiAPI::convertNativeToJavaObject(env, *in);
                        }
                        return nullptr;
                    }else{
                        return QtJambiAPI::convertNativeToJavaObject(env, in);
                    }
                }else{
                    return QtJambiAPI::toJavaString(env, qtjambi_cast<jobject,NativeType_in, Args...>::cast(in, args...));
                }
            }
        }else{ // !forward
            if constexpr(std::is_same_v<NativeType, QString>){
                if constexpr(is_pointer || is_reference){
                    if constexpr(is_pointer){
                        if(!in)
                            return nullptr;
                    }
                    if constexpr(!cast_var_args<Args...>::hasScope && !std::is_same_v<JniType,jstring>){
                        if(!QtJambiAPI::isQStringObject(env, in)){
                            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null"), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                        }
                        return ptr2ref<is_reference || !is_pointer,NativeType>::value(env, QtJambiAPI::convertJavaObjectToNative<NativeType>(env, in));
                    }else{
                        NativeType* result{nullptr};
                        if constexpr(!std::is_same_v<JniType,jstring>){
                            if(QtJambiAPI::isQStringObject(env, in)){
                                result = QtJambiAPI::convertJavaObjectToNative<QString>(env, in);
                            }else if(JBufferConstData::isBuffer(env, in)){
                                PersistentJBufferConstData* buffer = new PersistentJBufferConstData(env, in);
                                cast_var_args<Args...>::scope(args...).addDeletion(buffer);
                                result = new NativeType(buffer->constData<QChar>(), buffer->size<QChar>());
                                cast_var_args<Args...>::scope(args...).addDeletion(result);
                            }
                        }
                        if(!result){
                            JString2QChars buffer(env, qtjambi_to_jstring<JniType>::cast(env, in));
                            result = new QString(buffer.toString());
                            cast_var_args<Args...>::scope(args...).addDeletion(result);
                        }
                        return ptr2ref<is_reference || !is_pointer,QString>::value(env, result);
                    }
                }else{
                    if(!env->IsSameObject(in, nullptr)){
                        if constexpr(std::is_same_v<JniType,jstring>){
                            JString2QChars buffer(env, in);
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            return buffer.toString();
                        }else{
                            if constexpr(std::is_same_v<JniType,jcharArray>){
                                JConstCharArrayPointer arrayPointer(env, in);
                                JavaException::check(env QTJAMBI_STACKTRACEINFO );
                                return NativeType(arrayPointer.pointer(), arrayPointer.length());
                            }else if constexpr(!std::is_same_v<JniType,jstring>){
                                if(QtJambiAPI::isQStringObject(env, in)){
                                    return QtJambiAPI::convertJavaObjectToNativeReference<QString>(env, in);
                                }
                                if constexpr(cast_var_args<Args...>::hasScope){
                                    if(JBufferConstData::isBuffer(env, in)){
                                        PersistentJBufferConstData* buffer = new PersistentJBufferConstData(env, in);
                                        cast_var_args<Args...>::scope(args...).addDeletion(buffer);
                                        return NativeType(buffer->constData<QChar>(), buffer->size<QChar>());
                                    }
                                }
                            }
                            JString2QChars buffer(env, qtjambi_to_jstring<JniType>::cast(env, in));
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            return buffer.toString();
                        }
                    }
                    return QString{};
                }
            }else if constexpr(std::is_same_v<NativeType, QVariant>){
                if constexpr(is_pointer || is_reference){
                    Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope || !is_reference, "Cannot cast to QVariant& without scope.");
                    Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope || !is_pointer, "Cannot cast to QVariant* without scope.");
                    if constexpr(is_pointer){
                        if(!in)
                            return nullptr;
                    }
                    J2CStringBuffer buffer(env, qtjambi_to_jstring<JniType>::cast(env, in));
                    QVariant* result = new QVariant(buffer.toByteArray());
                    cast_var_args<Args...>::scope(args...).addDeletion(result);
                    return ptr2ref<is_reference || !is_pointer,QVariant>::value(env, result);
                }else{
                    if(!env->IsSameObject(in, nullptr)){
                        J2CStringBuffer buffer(env, qtjambi_to_jstring<JniType>::cast(env, in));
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        return buffer.toByteArray();
                    }
                    return QVariant{};
                }
            }else if constexpr(std::is_same_v<NativeType, QStringView>
                                 || std::is_same_v<NativeType, QAnyStringView>){
                if constexpr(is_pointer){
                    if(!in)
                        return nullptr;
                }
                if constexpr(is_pointer || is_reference){
                    Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast to StringView without scope.");
                    NativeType* result{nullptr};
                    if constexpr(!std::is_same_v<JniType,jstring>){
                        if(QtJambiAPI::isQStringObject(env, in)){
                            result = new NativeType(QtJambiAPI::convertJavaObjectToNativeReference<QString>(env, in));
                        }else if(JBufferConstData::isBuffer(env, in)){
                            PersistentJBufferConstData* buffer = new PersistentJBufferConstData(env, in);
                            cast_var_args<Args...>::scope(args...).addDeletion(buffer);
                            result = new NativeType(buffer->constData<QChar>(), buffer->size<QChar>());
                        }
                        if constexpr(std::is_same_v<NativeType, QAnyStringView>){
                            if(!result){
                                if(QtJambiAPI::isQByteArrayObject(env, in)){
                                    result = new NativeType(QtJambiAPI::convertJavaObjectToNativeReference<QByteArray>(env, in));
                                }else if(QtJambiAPI::isQByteArrayViewObject(env, in)){
                                    result = new NativeType(QtJambiAPI::convertJavaObjectToNativeReference<QByteArrayView>(env, in));
                                }
                            }
                        }
                    }
                    if(!result){
                        PersistentJString2QChars* buffer = new PersistentJString2QChars(env, qtjambi_to_jstring<JniType>::cast(env, in));
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        cast_var_args<Args...>::scope(args...).addDeletion(buffer);
                        result = new NativeType(buffer->operator NativeType());
                    }
                    cast_var_args<Args...>::scope(args...).addDeletion(result);
                    return ptr2ref<is_reference || !is_pointer,NativeType>::value(env, result);
                }else{
                    if(!env->IsSameObject(in, nullptr)){
                        if constexpr(!cast_var_args<Args...>::hasScope && !std::is_same_v<JniType,jstring>){
                            if(QtJambiAPI::isQStringObject(env, in)){
                                return NativeType(QtJambiAPI::convertJavaObjectToNativeReference<QString>(env, in));
                            }
                            if constexpr(std::is_same_v<NativeType, QAnyStringView>){
                                if(QtJambiAPI::isQByteArrayObject(env, in)){
                                    return NativeType(QtJambiAPI::convertJavaObjectToNativeReference<QByteArray>(env, in));
                                }else if(QtJambiAPI::isQByteArrayViewObject(env, in)){
                                    return NativeType(QtJambiAPI::convertJavaObjectToNativeReference<QByteArrayView>(env, in));
                                }
                            }
                            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null"), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                            return NativeType{};
                        }else{
                            if constexpr(!std::is_same_v<JniType,jstring>){
                                if(QtJambiAPI::isQStringObject(env, in)){
                                    return NativeType(QtJambiAPI::convertJavaObjectToNativeReference<QString>(env, in));
                                }else if(JBufferConstData::isBuffer(env, in)){
                                    PersistentJBufferConstData* buffer = new PersistentJBufferConstData(env, in);
                                    cast_var_args<Args...>::scope(args...).addDeletion(buffer);
                                    return NativeType(buffer->constData<QChar>(), buffer->size<QChar>());
                                }
                                if constexpr(std::is_same_v<NativeType, QAnyStringView>){
                                    if(QtJambiAPI::isQByteArrayObject(env, in)){
                                        return NativeType(QtJambiAPI::convertJavaObjectToNativeReference<QByteArray>(env, in));
                                    }else if(QtJambiAPI::isQByteArrayViewObject(env, in)){
                                        return NativeType(QtJambiAPI::convertJavaObjectToNativeReference<QByteArrayView>(env, in));
                                    }
                                }
                            }
                            PersistentJString2QChars* buffer = new PersistentJString2QChars(env, qtjambi_to_jstring<JniType>::cast(env, in));
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            cast_var_args<Args...>::scope(args...).addDeletion(buffer);
                            return buffer->operator NativeType();
                        }
                    }else{
                        return NativeType{};
                    }
                }
            }else if constexpr(std::is_same_v<NativeType, QByteArray>){
                if constexpr(is_pointer || is_reference){
                    if constexpr(is_pointer){
                        if(!in)
                            return nullptr;
                    }
                    if constexpr(!cast_var_args<Args...>::hasScope && !std::is_same_v<JniType,jstring>){
                        if(!QtJambiAPI::isQByteArrayObject(env, in)){
                            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null"), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                        }
                        return ptr2ref<is_reference || !is_pointer,NativeType>::value(env, QtJambiAPI::convertJavaObjectToNative<NativeType>(env, in));
                    }else{
                        QByteArray* result{nullptr};
                        if constexpr(!std::is_same_v<JniType,jstring>){
                            if(QtJambiAPI::isQByteArrayObject(env, in)){
                                result = QtJambiAPI::convertJavaObjectToNative<QByteArray>(env, in);
                            }else if(QtJambiAPI::isQByteArrayViewObject(env, in)){
                                result = new NativeType(QtJambiAPI::convertJavaObjectToNativeReference<QByteArrayView>(env, in));
                                cast_var_args<Args...>::scope(args...).addDeletion(result);
                            }else if(QtJambiAPI::isQStringObject(env, in)){
                                result = new NativeType(QtJambiAPI::convertJavaObjectToNative<QString>(env, in)->toUtf8());
                                cast_var_args<Args...>::scope(args...).addDeletion(result);
                            }else if(QtJambiAPI::isQVariantObject(env, in)){
                                result = new NativeType(QtJambiAPI::convertJavaObjectToNative<QVariant>(env, in)->template value<NativeType>());
                                cast_var_args<Args...>::scope(args...).addDeletion(result);
                            }
                        }
                        if(!result){
                            J2CStringBuffer buffer(env, qtjambi_to_jstring<JniType>::cast(env, in));
                            result = new QByteArray(buffer.toByteArray());
                            cast_var_args<Args...>::scope(args...).addDeletion(result);
                        }
                        return ptr2ref<is_reference || !is_pointer,QByteArray>::value(env, result);
                    }
                }else{
                    if(!env->IsSameObject(in, nullptr)){
                        if constexpr(!std::is_same_v<JniType,jstring>){
                            if(QtJambiAPI::isQByteArrayObject(env, in)){
                                return QtJambiAPI::convertJavaObjectToNativeReference<QByteArray>(env, in);
                            }else if(QtJambiAPI::isQByteArrayViewObject(env, in)){
                                return NativeType(QtJambiAPI::convertJavaObjectToNativeReference<QByteArrayView>(env, in));
                            }else if(QtJambiAPI::isQStringObject(env, in)){
                                return QtJambiAPI::convertJavaObjectToNative<QString>(env, in)->toUtf8();
                            }else if(QtJambiAPI::isQVariantObject(env, in)){
                                return QtJambiAPI::convertJavaObjectToNative<QVariant>(env, in)->template value<NativeType>();
                            }
                        }
                        J2CStringBuffer buffer(env, qtjambi_to_jstring<JniType>::cast(env, in));
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        return buffer.toByteArray();
                    }
                    return QByteArray{};
                }
            }else if constexpr(std::is_same_v<NativeType, QLatin1String>
                                 || std::is_same_v<NativeType, QUtf8StringView>
                                 || std::is_same_v<NativeType, QByteArrayView>){
                if constexpr(is_pointer){
                    if(!in)
                        return nullptr;
                }
                if constexpr(is_pointer || is_reference){
                    Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast without scope.");
                    NativeType* result{nullptr};
                    if constexpr(!std::is_same_v<JniType,jstring>){
                        if(QtJambiAPI::isQByteArrayObject(env, in)){
                            result = new NativeType(QtJambiAPI::convertJavaObjectToNativeReference<QByteArray>(env, in));
                        }else if(QtJambiAPI::isQByteArrayViewObject(env, in)){
                            result = new NativeType(QtJambiAPI::convertJavaObjectToNativeReference<QByteArrayView>(env, in));
                        }else if(JBufferConstData::isBuffer(env, in)){
                            PersistentJBufferConstData* buffer = new PersistentJBufferConstData(env, in);
                            cast_var_args<Args...>::scope(args...).addDeletion(buffer);
                            result = new NativeType(buffer->constData<char>(), buffer->size<char>());
                        }
                    }
                    if(!result){
                        PersistentJ2CStringBuffer* buffer = new PersistentJ2CStringBuffer(env, qtjambi_to_jstring<JniType>::cast(env, in));
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        cast_var_args<Args...>::scope(args...).addDeletion(buffer);
                        result = new NativeType(buffer->operator NativeType());
                    }
                    cast_var_args<Args...>::scope(args...).addDeletion(result);
                    return ptr2ref<is_reference || !is_pointer,NativeType>::value(env, result);
                }else{
                    if(!env->IsSameObject(in, nullptr)){
                        if constexpr(!cast_var_args<Args...>::hasScope && !std::is_same_v<JniType,jstring>){
                            if(QtJambiAPI::isQByteArrayObject(env, in)){
                                return NativeType(QtJambiAPI::convertJavaObjectToNativeReference<QByteArray>(env, in));
                            }else if(QtJambiAPI::isQByteArrayViewObject(env, in)){
                                return NativeType(QtJambiAPI::convertJavaObjectToNativeReference<QByteArrayView>(env, in));
                            }
                            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null"), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                            return NativeType{};
                        }else{
                            if constexpr(!std::is_same_v<JniType,jstring>){
                                if(QtJambiAPI::isQByteArrayObject(env, in)){
                                    return NativeType(QtJambiAPI::convertJavaObjectToNativeReference<QByteArray>(env, in));
                                }else if(QtJambiAPI::isQByteArrayViewObject(env, in)){
                                    return NativeType(QtJambiAPI::convertJavaObjectToNativeReference<QByteArrayView>(env, in));
                                }else if(JBufferConstData::isBuffer(env, in)){
                                    PersistentJBufferConstData* buffer = new PersistentJBufferConstData(env, in);
                                    cast_var_args<Args...>::scope(args...).addDeletion(buffer);
                                    return NativeType(buffer->constData<char>(), buffer->size<char>());
                                }
                            }
                            PersistentJ2CStringBuffer* buffer = new PersistentJ2CStringBuffer(env, qtjambi_to_jstring<JniType>::cast(env, in));
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            cast_var_args<Args...>::scope(args...).addDeletion(buffer);
                            return buffer->operator NativeType();
                        }
                    }else{
                        return NativeType{};
                    }
                }
            }else if constexpr(std::is_same_v<JniType, jstring> && (std::is_same_v<NativeType, char>
                                                                      || std::is_same_v<NativeType, QLatin1Char>
                                                                      || std::is_same_v<NativeType, std::byte>) && is_pointer){
                if constexpr(is_const){
                    Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast to const char* without scope.");
                    PersistentJ2CStringBuffer* buffer = new PersistentJ2CStringBuffer(env, in);
                    cast_var_args<Args...>::scope(args...).addDeletion(buffer);
                    JavaException::check(env QTJAMBI_STACKTRACEINFO );
                    if constexpr(std::is_same_v<NativeType, QLatin1Char>){
                        return reinterpret_cast<const QLatin1Char*>(buffer->constData());
                    }else if constexpr(std::is_same_v<NativeType, std::byte>){
                        return reinterpret_cast<const std::byte*>(buffer->constData());
                    }else{
                        return buffer->constData();
                    }
                }else{
                    Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast to char* without scope.");
                    J2CStringBuffer buffer(env, in);
                    QByteArray* result = new QByteArray(buffer.toByteArray());
                    cast_var_args<Args...>::scope(args...).addDeletion(result);
                    return result->data();
                    if constexpr(std::is_same_v<NativeType, QLatin1Char>){
                        return reinterpret_cast<QLatin1Char*>(result->data());
                    }else if constexpr(std::is_same_v<NativeType, std::byte>){
                        return reinterpret_cast<std::byte*>(result->data());
                    }else{
                        return result->data();
                    }
                }
            }else if constexpr(std::is_same_v<JniType, jstring> && (std::is_same_v<NativeType, QChar>
                                                                      || std::is_same_v<NativeType, char16_t>) && is_pointer){
                if constexpr(is_const){
                    Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast to const QChar* without scope.");
                    PersistentJString2QChars* buffer = new PersistentJString2QChars(env, in);
                    cast_var_args<Args...>::scope(args...).addDeletion(buffer);
                    JavaException::check(env QTJAMBI_STACKTRACEINFO );
                    if constexpr(std::is_same_v<NativeType, char16_t>){
                        return reinterpret_cast<const char16_t*>(buffer->data());
                    }else{
                        return buffer->data();
                    }
                }else{
                    Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast to QChar* without scope.");
                    JString2QChars buffer(env, in);
                    QString* chars = new QString(buffer.toString());
                    cast_var_args<Args...>::scope(args...).addDeletion(chars);
                    JavaException::check(env QTJAMBI_STACKTRACEINFO );
                    if constexpr(std::is_same_v<NativeType, char16_t>){
                        return reinterpret_cast<char16_t*>(chars->data());
                    }else{
                        return chars->data();
                    }
                }
            }
        }
    }
};

template<typename... Args>
struct qtjambi_jobject_plain_cast<true, jobject, _jstring, true, false, false, false, Args...>
{
    static jobject cast(jstring in, Args...){
        return in;
    }
};

template<typename... Args>
struct qtjambi_jobject_plain_cast<true, jstring, _jobject, true, false, false, false, Args...>
{
    static jstring cast(jobject in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        return QtJambiAPI::toJavaString(env, in);
    }
};

// enums

// object <=> arithmetic

template<bool forward,
         typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_arithmetic_cast<forward,
                                       jstring, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;
    typedef typename std::conditional<forward, NativeType_in, jstring>::type In;
    typedef typename std::conditional<forward, jstring, NativeType_out>::type Out;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(forward){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jstring without JNIEnv.");
            if constexpr(is_pointer){
                if constexpr(std::is_integral<NativeType>::value){
                    if constexpr(sizeof(NativeType)==sizeof(char)){
                        return in ? env->NewStringUTF(reinterpret_cast<const char*>(in)) : nullptr;
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                    }else if constexpr(sizeof(NativeType)==sizeof(jchar)){
                        return in ? env->NewString(reinterpret_cast<const jchar*>(in), std::wcslen(reinterpret_cast<const wchar_t*>(in))) : nullptr;
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                    }else{
                        Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
                    }
                }else{
                    Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
                }
            }else{
                Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
            }
        }else if constexpr(is_const && is_pointer){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast jstring to const char* pointer without scope.");
            if(!in)
                return nullptr;
            if constexpr(std::is_integral<NativeType>::value){
                if constexpr(sizeof(NativeType)==sizeof(jbyte)){
                    PersistentJ2CStringBuffer* buffer = new PersistentJ2CStringBuffer(env, in);
                    cast_var_args<Args...>::scope(args...).addDeletion(buffer);
                    return buffer->data();
                }else if constexpr(sizeof(NativeType)==sizeof(jchar)){
                    PersistentJ2CStringBuffer* buffer = new PersistentJ2CStringBuffer(env, in);
                    cast_var_args<Args...>::scope(args...).addDeletion(buffer);
                    return buffer->data();
                }else{
                    Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
                }
            }else{
                Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
            }
        }else{
            Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
        }
    }
};

template<bool forward,
         typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_arithmetic_cast<forward,
                                      jobject, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;
    typedef typename std::conditional<forward, NativeType_in, jobject>::type In;
    typedef typename std::conditional<forward, jobject, NativeType_out>::type Out;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(forward){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
            if constexpr(is_pointer){
                if constexpr(std::is_same_v<NativeType,bool>){
                    return QtJambiAPI::toJavaBooleanObject(env, bool(*in));
                }else if constexpr(std::is_integral<NativeType>::value){
                    if constexpr(sizeof(NativeType)==sizeof(jbyte)){
                        return QtJambiAPI::toJavaByteObject(env, jbyte(*in));
                    }else if constexpr(sizeof(NativeType)==sizeof(jshort)){
                        return QtJambiAPI::toJavaShortObject(env, jshort(*in));
                    }else if constexpr(sizeof(NativeType)==sizeof(jint)){
                        return QtJambiAPI::toJavaIntegerObject(env, jint(*in));
                    }else if constexpr(sizeof(NativeType)==sizeof(jlong)){
                        return QtJambiAPI::toJavaLongObject(env, jlong(*in));
                    }else{
                        Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
                    }
                }else if constexpr(std::is_floating_point<NativeType>::value){
                    if constexpr(sizeof(NativeType)==sizeof(jfloat)){
                        return QtJambiAPI::toJavaFloatObject(env, jfloat(*in));
                    }else if constexpr(sizeof(NativeType)==sizeof(jdouble)){
                        return QtJambiAPI::toJavaDoubleObject(env, jdouble(*in));
                    }else{
                        Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
                    }
                }else if constexpr(std::is_same_v<NativeType,QChar>){
                    return QtJambiAPI::toJavaCharacterObject(env, jchar(in->unicode()));
                }else if constexpr(std::is_same_v<NativeType,QLatin1Char>){
                    return QtJambiAPI::toJavaByteObject(env, jbyte(in->toLatin1()));
                }else{
                    Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
                }
            }else{
                if constexpr(std::is_same_v<NativeType,bool>){
                    return QtJambiAPI::toJavaBooleanObject(env, bool(in));
                }else if constexpr(std::is_integral<NativeType>::value){
                    if constexpr(sizeof(NativeType)==sizeof(jbyte)){
                        return QtJambiAPI::toJavaByteObject(env, jbyte(in));
                    }else if constexpr(sizeof(NativeType)==sizeof(jshort)){
                        return QtJambiAPI::toJavaShortObject(env, jshort(in));
                    }else if constexpr(sizeof(NativeType)==sizeof(jint)){
                        return QtJambiAPI::toJavaIntegerObject(env, jint(in));
                    }else if constexpr(sizeof(NativeType)==sizeof(jlong)){
                        return QtJambiAPI::toJavaLongObject(env, jlong(in));
                    }else{
                        Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
                    }
                }else if constexpr(std::is_floating_point<NativeType>::value){
                    if constexpr(sizeof(NativeType)==sizeof(jfloat)){
                        return QtJambiAPI::toJavaFloatObject(env, jfloat(in));
                    }else if constexpr(sizeof(NativeType)==sizeof(jdouble)){
                        return QtJambiAPI::toJavaDoubleObject(env, jdouble(in));
                    }else{
                        Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
                    }
                }else if constexpr(std::is_same_v<NativeType,QChar>){
                    return QtJambiAPI::toJavaCharacterObject(env, jchar(in.unicode()));
                }else if constexpr(std::is_same_v<NativeType,QLatin1Char>){
                    return QtJambiAPI::toJavaByteObject(env, jbyte(in.toLatin1()));
                }else if constexpr(std::is_same_v<NativeType,std::byte>){
                    return QtJambiAPI::toJavaByteObject(env, jbyte(in));
                }else{
                    Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
                }
            }
        }else{
            if constexpr(is_pointer || is_reference){
                Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast to pointer or reference without scope.");
                NativeType* result = new NativeType(NativeType(0));
                cast_var_args<Args...>::scope(args...).addDeletion(result);
                if constexpr(std::is_same_v<NativeType,bool>){
                    *result = NativeType(QtJambiAPI::fromJavaBooleanObject(env, in));
                }else if constexpr(std::is_integral<NativeType>::value){
                    if constexpr(sizeof(NativeType)==sizeof(jbyte)){
                        *result = NativeType(QtJambiAPI::fromJavaByteObject(env, in));
                    }else if constexpr(sizeof(NativeType)==sizeof(jshort)){
                        *result = NativeType(QtJambiAPI::fromJavaShortObject(env, in));
                    }else if constexpr(sizeof(NativeType)==sizeof(jint)){
                        *result = NativeType(QtJambiAPI::fromJavaIntegerObject(env, in));
                    }else if constexpr(sizeof(NativeType)==sizeof(jlong)){
                        *result = NativeType(QtJambiAPI::fromJavaLongObject(env, in));
                    }else{
                        Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
                    }
                }else if constexpr(std::is_floating_point<NativeType>::value){
                    if constexpr(sizeof(NativeType)==sizeof(jfloat)){
                        *result = NativeType(QtJambiAPI::fromJavaFloatObject(env, in));
                    }else if constexpr(sizeof(NativeType)==sizeof(jdouble)){
                        *result = NativeType(QtJambiAPI::fromJavaDoubleObject(env, in));
                    }else{
                        Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
                    }
                }else if constexpr(std::is_same_v<NativeType,QChar>){
                    *result = NativeType(QtJambiAPI::fromJavaCharacterObject(env, in));
                }else if constexpr(std::is_same_v<NativeType,QLatin1Char>){
                    *result = NativeType(QtJambiAPI::fromJavaByteObject(env, in));
                }else if constexpr(std::is_same_v<NativeType,std::byte>){
                    *result = NativeType(QtJambiAPI::fromJavaByteObject(env, in));
                }else{
                    Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
                }
                if constexpr(is_pointer)
                    return result;
                else
                    return *result;
            }else{
                if constexpr(std::is_same_v<NativeType,bool>){
                    return NativeType(QtJambiAPI::fromJavaBooleanObject(env, in));
                }else if constexpr(std::is_integral<NativeType>::value){
                    if constexpr(sizeof(NativeType)==sizeof(jbyte)){
                        return NativeType(QtJambiAPI::fromJavaByteObject(env, in));
                    }else if constexpr(sizeof(NativeType)==sizeof(jshort)){
                        return NativeType(QtJambiAPI::fromJavaShortObject(env, in));
                    }else if constexpr(sizeof(NativeType)==sizeof(jint)){
                        return NativeType(QtJambiAPI::fromJavaIntegerObject(env, in));
                    }else if constexpr(sizeof(NativeType)==sizeof(jlong)){
                        return NativeType(QtJambiAPI::fromJavaLongObject(env, in));
                    }else{
                        Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
                    }
                }else if constexpr(std::is_floating_point<NativeType>::value){
                    if constexpr(sizeof(NativeType)==sizeof(jfloat)){
                        return NativeType(QtJambiAPI::fromJavaFloatObject(env, in));
                    }else if constexpr(sizeof(NativeType)==sizeof(jdouble)){
                        return NativeType(QtJambiAPI::fromJavaDoubleObject(env, in));
                    }else{
                        Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
                    }
                }else if constexpr(std::is_same_v<NativeType,QChar>){
                    return NativeType(QtJambiAPI::fromJavaCharacterObject(env, in));
                }else if constexpr(std::is_same_v<NativeType,QLatin1Char>){
                    return NativeType(QtJambiAPI::fromJavaByteObject(env, in));
                }else if constexpr(std::is_same_v<NativeType,std::byte>){
                    return NativeType(QtJambiAPI::fromJavaByteObject(env, in));
                }else{
                    Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
                }
            }
        }
    }
};

// QChar

template<bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_plain_cast<true,
        jobject,
        QChar, /*is_pointer*/false, is_const, is_reference, is_rvalue, Args...>{
    typedef typename std::conditional<is_const, typename std::add_const<QChar>::type, QChar>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;

    static jobject cast(NativeType_in in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        return QtJambiAPI::toJavaCharacterObject(env, in.unicode());
    }
};

template<bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_plain_cast<false,
        jobject,
        QChar, is_pointer, is_const, is_reference, is_rvalue, Args...>{
    typedef typename std::conditional<is_const, typename std::add_const<QChar>::type, QChar>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    static NativeType_out cast(jobject in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(is_pointer || is_reference){
            Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast jobject to QChar pointer or reference without scope");
            QChar* result = new QChar(QtJambiAPI::fromJavaCharacterObject(env, in));
            cast_var_args<Args...>::scope(args...).addDeletion(result);
            if constexpr(is_pointer){
                return result;
            }else{
                return *result;
            }
        }else{
            return QChar(QtJambiAPI::fromJavaCharacterObject(env, in));
        }
    }
};

// QStringList

template<bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_plain_cast<true,
        jobject,
        QStringList, is_pointer, is_const, is_reference, is_rvalue, Args...>{
    typedef typename std::conditional<is_const, const QStringList,QStringList>::type Container;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;

    static jobject cast(Container_in in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        return QtJambiAPI::convertQStringListToJavaObject(env,
                                                          cast_var_args<Args...>::relatedNativeID(args...),
                                                          ref_ptr<is_pointer, Container>::ref(in),
                                                          is_pointer && !is_const ? nullptr : CopyFunction([](const void* ptr) -> void* { return new QStringList(*reinterpret_cast<const QStringList*>(ptr)); }),
                                                          [](void* ptr,bool) { delete reinterpret_cast<QStringList*>(ptr); }
                                                          );
    }
};

template<bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_plain_cast<false,
        jobject,
        QStringList, is_pointer, is_const, is_reference, is_rvalue, Args...>{
    typedef typename std::conditional<is_const, typename std::add_const<QStringList>::type, QStringList>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_const, typename std::add_const<QList<QString>>::type, QList<QString>>::type NativeType2_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType2_c>::type, NativeType2_c>::type NativeType2_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType2_c>::type, NativeType2_c>::type NativeType2_ptr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType2_c>::type, NativeType2_cr>::type NativeType2_ptr_out;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType2_c>::type, NativeType2_cr>::type NativeType2_cast;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_c>::type NativeType_ptr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    static NativeType_out cast(jobject in, Args... args){
        NativeType2_ptr_out result = qtjambi_jobject_template1_cast<false,jobject,QList,is_pointer,is_const,is_reference,is_rvalue,QString, Args...>::cast(in, args...);
        return *reinterpret_cast<NativeType_ptr*>(&result);
    }
};

}// namespace QtJambiPrivate

#endif // QTJAMBI_CAST_IMPL_JNITYPE_H
