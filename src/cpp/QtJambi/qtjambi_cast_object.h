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

#ifndef QTJAMBI_CAST_OBJECT_H
#define QTJAMBI_CAST_OBJECT_H

#include "qtjambi_cast_util.h"
#include "qtjambiapi_string.h"
#include "qtjambiapi_variant.h"
#include "qtjambiapi_convert.h"
#include "qtjambishell.h"

class QModelIndex;
class QModelRoleData;
class QModelRoleDataSpan;

namespace QtJambiPrivate {

template<bool forward, typename JniType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename Args, template<typename... Ts> class NativeType, typename... Ts>
static constexpr auto qtjambi_jobject_template_cast_impl(const NativeType<Ts...>&);

template<bool forward, typename JniType, typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
static constexpr auto qtjambi_jobject_cast_impl() {
    if constexpr(std::is_arithmetic<NativeType>::value
                  || std::is_same_v<NativeType, QChar>
                  || std::is_same_v<NativeType, QLatin1Char>
                  || std::is_same_v<NativeType, std::byte>){
        return find_qtjambi_jobject_arithmetic_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>();
    }else if constexpr(std::is_same_v<JniType, jstring>
                  || std::is_same_v<NativeType, QString>
                  || std::is_same_v<NativeType, QStringView>
                  || std::is_same_v<NativeType, QAnyStringView>
                  || std::is_same_v<NativeType, QUtf8StringView>
                  || std::is_same_v<NativeType, QLatin1String>
#if defined(__cpp_char8_t)
                  || std::is_same_v<NativeType, std::u8string>
                  || std::is_same_v<NativeType, std::u8string_view>
#endif
                  || std::is_same_v<NativeType, std::string>
                  || std::is_same_v<NativeType, std::string_view>){
        return qtjambi_string_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>{};
    }else if constexpr(is_template<NativeType>::value){
        return decltype(qtjambi_jobject_template_cast_impl<forward, JniType, is_pointer, is_const, is_reference, is_rvalue, std::tuple<Args...>>(std::declval<const NativeType&>())){};
    }else if constexpr(std::is_function_v<NativeType>){
        return qtjambi_jobject_function_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>{};
#ifdef QOBJECT_H
    }else if constexpr(std::is_base_of_v<QObject, NativeType>){
        return qtjambi_jnitype_qobject_cast<forward, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>{};
#endif
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
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::add_pointer_t<NativeType> NativeType_ptr;
    typedef std::conditional_t<forward, NativeType_in, JniType> In;
    typedef std::conditional_t<forward, JniType, NativeType_out> Out;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(!std::is_same_v<JniType, jobject>
            && !(std::is_same_v<JniType, jstring> && (std::is_same_v<NativeType, QByteArray>
                                                      || std::is_same_v<NativeType, QByteArrayView>))){
            if constexpr(std::is_same_v<JniType, jobjectArray> || std::is_same_v<JniType, jbyteArray>){
                QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/ArrayCast, is_complete_v<value_range_converter<JniType, char, true, JniType, NativeType_c, Args...>>);
                if constexpr(forward){
                    return value_range_converter<JniType, char, true, JniType, NativeType_c, Args...>::toJavaArray(ref_ptr<is_pointer, NativeType_c>::ref(in), arg_pointer<Args>::ref(args)...);
                }else{
                    return value_range_converter<JniType, char, true, JniType, NativeType, Args...>::toNativeContainer(in, -1, arg_pointer<Args>::ref(args)...);
                }
            }else{
                Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
                return {};
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
                    if(in){
                        const std::type_info* typeId{nullptr};
                        try{
                            typeId = &typeid(in);
                        }catch(const std::bad_typeid&){
                        }catch(...){}
                        // check rtti availability:
                        if(typeId){
                            try{
                                if(const QtJambiShellInterface* shell = dynamic_cast<const QtJambiShellInterface*>(in))
                                    return QtJambiShellInterface::getJavaObjectLocalRef(env, shell);
                            }catch(...){}
                        }
                    }
                    if constexpr(cast_var_args<Args...>::hasScope)
                        return QtJambiAPI::convertNativeToJavaObjectAsWrapperAndInvalidateAfterUse(env, cast_var_args<Args...>::scope(args...), in, typeid(NativeType));
                    else
                        return QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, in, typeid(NativeType));
                }else{
                    if constexpr(cast_var_args<Args...>::hasScope)
                        return QtJambiAPI::convertNativeToJavaObjectAsWrapperAndInvalidateAfterUse(env, cast_var_args<Args...>::scope(args...), in, typeid(NativeType));
                    else
                        return QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, in, typeid(NativeType));
                }
            }else if constexpr(is_rvalue && !is_const && is_move_constructible_v<NativeType>){
                std::unique_ptr<NativeType> ptr = std::make_unique<NativeType>(std::move(in));
                jobject out = QtJambiAPI::convertNativeToJavaOwnedObjectAsWrapper(env, ptr.get(), typeid(NativeType));
                if(out)
                    (void)ptr.release();
                return out;
            }else if constexpr((!is_copy_constructible_v<NativeType> || (is_reference && !is_rvalue && !is_const)) && cast_var_args<Args...>::hasScope){
                return QtJambiAPI::convertNativeToJavaObjectAsWrapperAndInvalidateAfterUse(env, cast_var_args<Args...>::scope(args...), &in, typeid(NativeType));
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
                    if constexpr(!is_default_constructible_v<NativeType> || (is_reference && !is_const)){
                        if(!result)
                            JavaException::raiseNullPointerException(env, QStringLiteral("Cannot cast null to reference type %1").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                    }
                    return qtjambi_deref_value<NativeType, is_default_constructible_v<NativeType>, is_copy_constructible_v<NativeType>, is_const, is_reference>::deref(env, result);
                }
            }
        }
    }
};

template<bool forward,
         typename JniType,
         class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename Tuple>
struct qtjambi_jobject_templateX_cast;

template<bool forward,
         typename JniType,
         class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename... Args>
struct qtjambi_jobject_templateX_cast<forward,JniType,NativeType,is_pointer,is_const,is_reference,is_rvalue,std::tuple<Args...>> : decltype(qtjambi_jobject_template_plain_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>()){
};

template<bool forward,
         typename JniType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename Args, size_t P, typename... Ts>
struct qtjambi_jobject_template_cast : qtjambi_jobject_templateX_cast<forward,JniType,NativeType<Ts...>,is_pointer,is_const,is_reference,is_rvalue,Args>{
};

template<bool forward, typename JniType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename Args, template<typename... Ts> class NativeType, typename... Ts>
static constexpr auto qtjambi_jobject_template_cast_impl(const NativeType<Ts...>&){
    return qtjambi_jobject_template_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args, sizeof...(Ts), Ts...>{};
}

template<bool forward,
         typename JniType,
         template<typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_jobject_template1_cast;

template<bool forward,
         typename JniType,
         template<typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
static constexpr auto find_qtjambi_jobject_template1_cast() {
    QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/Template1Cast, is_complete_v< qtjambi_jobject_template1_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, T, Args...> >);
    return qtjambi_jobject_template1_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, T, Args...>{};
}

template<bool forward,
         typename JniType,
         template<typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename T, typename... Args>
struct qtjambi_jobject_template_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, std::tuple<Args...>, 1, T>
 : decltype(find_qtjambi_jobject_template1_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, T, Args...>()){
};

template<bool forward,
         typename JniType,
         template<typename K, typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename... Args>
struct qtjambi_jobject_template2_cast;

template<bool forward,
         typename JniType,
         template<typename K, typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename... Args>
static constexpr auto find_qtjambi_jobject_template2_cast() {
    QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/Template2Cast, is_complete_v< qtjambi_jobject_template2_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, K, T, Args...> >);
    return qtjambi_jobject_template2_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, K, T, Args...>{};
}

template<bool forward,
         typename JniType,
         template<typename K, typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename K, typename T, typename... Args>
struct qtjambi_jobject_template_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, std::tuple<Args...>, 2, K, T>
 : decltype(find_qtjambi_jobject_template2_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, K, T, Args...>()){
};

template<bool forward,
         typename JniType,
         template<typename K, typename T, typename A> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename A, typename... Args>
struct qtjambi_jobject_template3_cast;

template<bool forward,
         typename JniType,
         template<typename K, typename T, typename A> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename A, typename... Args>
static constexpr auto find_qtjambi_jobject_template3_cast() {
    QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/Template3Cast, is_complete_v< qtjambi_jobject_template3_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, K, T, A, Args...> >);
    return qtjambi_jobject_template3_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, K, T, A, Args...>{};
}

template<bool forward,
         typename JniType,
         template<typename K, typename T, typename A> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename K, typename T, typename A, typename... Args>
struct qtjambi_jobject_template_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, std::tuple<Args...>, 3, K, T, A>
 : decltype(find_qtjambi_jobject_template3_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, K, T, A, Args...>()){
};

template<bool forward,
         typename JniType,
         template<typename K, typename T, typename A, typename B> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename A, typename B, typename... Args>
struct qtjambi_jobject_template4_cast;

template<bool forward,
         typename JniType,
         template<typename K, typename T, typename A, typename B> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename A, typename B, typename... Args>
static constexpr auto find_qtjambi_jobject_template4_cast() {
    QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/Template4Cast, is_complete_v< qtjambi_jobject_template4_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, K, T, A, B, Args...> >);
    return qtjambi_jobject_template4_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, K, T, A, B, Args...>{};
}

template<bool forward,
         typename JniType,
         template<typename K, typename T, typename A, typename B> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename K, typename T, typename A, typename B, typename... Args>
struct qtjambi_jobject_template_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, std::tuple<Args...>, 4, K, T, A, B>
 : decltype(find_qtjambi_jobject_template4_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, K, T, A, B, Args...>()){
};

template<bool forward,
         typename JniType,
         template<typename K, typename T, typename A, typename B, typename C> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename A, typename B, typename C, typename... Args>
struct qtjambi_jobject_template5_cast;

template<bool forward,
         typename JniType,
         template<typename K, typename T, typename A, typename B, typename C> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename A, typename B, typename C, typename... Args>
static constexpr auto find_qtjambi_jobject_template5_cast() {
    QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/Template5Cast, is_complete_v< qtjambi_jobject_template5_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, K, T, A, B, C, Args...> >);
    return qtjambi_jobject_template5_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, K, T, A, B, C, Args...>{};
}

template<bool forward,
         typename JniType,
         template<typename K, typename T, typename A, typename B, typename C> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename K, typename T, typename A, typename B, typename C, typename... Args>
struct qtjambi_jobject_template_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, std::tuple<Args...>, 5, K, T, A, B, C>
 : decltype(find_qtjambi_jobject_template5_cast<forward, JniType, NativeType, is_pointer, is_const, is_reference, is_rvalue, K, T, A, B, C, Args...>()){
};

template<bool forward, typename JniType, typename T, size_t N, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_std_array_cast;

template<bool forward, typename JniType, typename T, size_t N, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
constexpr auto qtjambi_jobject_std_array_cast_decider(){
    QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/ArrayCast, is_complete_v<qtjambi_jobject_std_array_cast<forward, JniType, T, N, is_pointer, is_const, is_reference, is_rvalue, Args...>>);
    return qtjambi_jobject_std_array_cast<forward, JniType, T, N, is_pointer, is_const, is_reference, is_rvalue, Args...>{};
};

template<bool forward, typename JniType, typename T, size_t N,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_plain_cast<forward, JniType, std::array<T,N>, is_pointer, is_const, is_reference, is_rvalue, Args...>
    : decltype(qtjambi_jobject_std_array_cast_decider<forward, JniType, T, N, is_pointer, is_const, is_reference, is_rvalue, Args...>()){};

#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0) //&& defined(QSPAN_H)

template<bool forward, typename JniType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename T, std::size_t E, template<typename,std::size_t> typename Span, bool t_is_const, typename... Args>
struct qtjambi_jobject_span_cast;

template<bool forward, typename JniType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename T, std::size_t E, template<typename,std::size_t> typename Span, bool t_is_const, typename... Args>
constexpr auto qtjambi_jobject_span_cast_decider(){
    QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/ArrayCast, is_complete_v<qtjambi_jobject_span_cast<forward, JniType, is_pointer, is_const, is_reference, is_rvalue, T, E, Span, t_is_const, Args...>>);
    return qtjambi_jobject_span_cast<forward, JniType, is_pointer, is_const, is_reference, is_rvalue, T, E, Span, t_is_const, Args...>{};
};

template<bool forward,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, std::size_t E, typename... Args>
struct qtjambi_jobject_plain_cast<forward,
                              JniType,
                              QSpan<T,E>, is_pointer, is_const, is_reference, is_rvalue, Args...>
    : decltype(qtjambi_jobject_span_cast_decider<forward, JniType, is_pointer, is_const, is_reference, is_rvalue, std::remove_cv_t<T>, E, QSpan, std::is_const_v<T>, Args...>()){
};

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, std::size_t E, typename... Args>
struct qtjambi_jobject_plain_cast<forward,
                              jobject,
                              QSpan<T,E>, is_pointer, is_const, is_reference, is_rvalue, Args...>
    : decltype(qtjambi_jobject_span_cast_decider<forward, jobject, is_pointer, is_const, is_reference, is_rvalue, std::remove_cv_t<T>, E, QSpan, std::is_const<T>::value, Args...>()){
};

#ifdef __cpp_lib_span
template<bool forward,
         typename JniType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, std::size_t E, typename... Args>
struct qtjambi_jobject_plain_cast<forward,
                                  JniType,
                                  std::span<T,E>, is_pointer, is_const, is_reference, is_rvalue, Args...>
    : qtjambi_jobject_span_cast<forward, JniType, is_pointer, is_const, is_reference, is_rvalue, std::remove_cv_t<T>, E, std::span, std::is_const<T>::value, Args...>{
};

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, std::size_t E, typename... Args>
struct qtjambi_jobject_plain_cast<forward,
                                  jobject,
                                  std::span<T,E>, is_pointer, is_const, is_reference, is_rvalue, Args...>
    : qtjambi_jobject_span_cast<forward, jobject, is_pointer, is_const, is_reference, is_rvalue, std::remove_cv_t<T>, E, std::span, std::is_const<T>::value, Args...>{
};
#endif // __cpp_lib_span
#endif // QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)

template<bool forward,
         typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_function_cast<forward,
                                     jobject, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>{
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::add_pointer_t<NativeType> NativeType_ptr;
    typedef std::conditional_t<forward, NativeType_in, jobject> In;
    typedef std::conditional_t<forward, jobject, NativeType_out> Out;

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
                return qtjambi_deref_value<NativeType, is_default_constructible_v<NativeType>, is_copy_constructible_v<NativeType>, is_const, is_reference>::deref(env, result);
            }
        }
    }
};

template<bool forward,
         typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jnitype_qobject_cast{
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::add_pointer_t<NativeType> NativeType_ptr;
    typedef std::conditional_t<forward, NativeType_in, jobject> In;
    typedef std::conditional_t<forward, jobject, NativeType_out> Out;
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
                return qtjambi_deref_value<NativeType, is_default_constructible_v<NativeType>, is_copy_constructible_v<NativeType>, is_const, is_reference>::deref(env, result);
            }
        }
    }
};

template<bool forward,
         typename JniType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_plain_cast<forward, JniType, QVariant, is_pointer, is_const, is_reference, is_rvalue, Args...>{
    typedef QVariant NativeType;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::add_pointer_t<NativeType> NativeType_ptr;
    typedef std::conditional_t<forward, NativeType_in, JniType> In;
    typedef std::conditional_t<forward, std::conditional_t<std::is_same_v<JniType, jcoreobject>,jobject,JniType>, NativeType_out> Out;

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
                    auto result = create<NativeType>(QtJambiAPI::convertJavaObjectToQVariant(env, in));
                    cast_var_args<Args...>::scope(args...).addDeletion(result);
                    if constexpr(is_pointer){
                        return result;
                    }else{
                        return qtjambi_deref_value<NativeType, is_default_constructible_v<NativeType>, is_copy_constructible_v<NativeType>, is_const, is_reference>::deref(env, result);
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

template<bool forward,
         typename NativeType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_model_cast;

template<bool forward,
         typename NativeType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
constexpr auto qtjambi_jobject_model_cast_decider(){
    QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/ModelCast, is_complete_v<qtjambi_jobject_model_cast<forward, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>>);
    return qtjambi_jobject_model_cast<forward, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>{};
};

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_plain_cast<forward, jobject, QModelIndex, is_pointer, is_const, is_reference, is_rvalue, Args...> : decltype(qtjambi_jobject_model_cast_decider<forward, QModelIndex, is_pointer, is_const, is_reference, is_rvalue, Args...>()){};

template<bool forward,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_plain_cast<forward, jobject, QModelRoleDataSpan, is_pointer, is_const, is_reference, is_rvalue, Args...> : decltype(qtjambi_jobject_model_cast_decider<forward, QModelRoleDataSpan, is_pointer, is_const, is_reference, is_rvalue, Args...>()){};

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

template<bool is_const, typename JniType, typename NativeType, typename... Args>
struct value_range_converter_from_java_buffer;

template<bool forward, class JniType, typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_string_cast{
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::conditional_t<forward, NativeType_in, JniType> In;
    typedef std::conditional_t<forward, std::conditional_t<std::is_same_v<JniType, jcoreobject>,jobject,JniType>, NativeType_out> Out;

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
                              || std::is_same_v<NativeType, QStringView>
                              || std::is_same_v<NativeType, std::u16string>
                              || std::is_same_v<NativeType, std::u16string_view>){
                    if constexpr(std::is_same_v<JniType, jcharArray>){
                        if constexpr(is_pointer){
                            if(in){
                                return QtJambiAPI::toJCharArray(env, reinterpret_cast<const jchar *>(in->data()), jsize(in->length()));
                            }else
                                return nullptr;
                        }else{
                            return QtJambiAPI::toJCharArray(env, reinterpret_cast<const jchar *>(in.data()), jsize(in.length()));
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
                                        return QtJambiAPI::toJCharArray(env, reinterpret_cast<const jchar *>(text.data()), jsize(text.length()));
                                    }else if constexpr(std::is_same_v<JniType, jbyteArray>){
                                        QByteArray b(text.toUtf8());
                                        return QtJambiAPI::toJByteArray(env, reinterpret_cast<const jbyte *>(b.data()), jsize(b.length()));
                                    }else{
                                        jstring str = env->NewString(reinterpret_cast<const jchar *>(text.data()), jsize(text.length()));
                                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                                        return str;
                                    }
                                }else{
                                    if constexpr(std::is_same_v<JniType, jcharArray>){
                                        QString s(text);
                                        return QtJambiAPI::toJCharArray(env, reinterpret_cast<const jchar *>(s.data()), jsize(s.length()));
                                    }else if constexpr(std::is_same_v<JniType, jbyteArray>){
                                        return QtJambiAPI::toJByteArray(env, reinterpret_cast<const jbyte *>(text.data()), jsize(text.length()));
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
                                    return QtJambiAPI::toJCharArray(env, reinterpret_cast<const jchar *>(text.data()), jsize(text.length()));
                                }else if constexpr(std::is_same_v<JniType, jbyteArray>){
                                    QByteArray b(text.toUtf8());
                                    return QtJambiAPI::toJByteArray(env, reinterpret_cast<const jbyte *>(b.data()), jsize(b.length()));
                                }else{
                                    jstring str = env->NewString(reinterpret_cast<const jchar *>(text.data()), jsize(text.length()));
                                    JavaException::check(env QTJAMBI_STACKTRACEINFO );
                                    return str;
                                }
                            }else{
                                if constexpr(std::is_same_v<JniType, jcharArray>){
                                    QString s(text);
                                    return QtJambiAPI::toJCharArray(env, reinterpret_cast<const jchar *>(s.data()), jsize(s.length()));
                                }else if constexpr(std::is_same_v<JniType, jbyteArray>){
                                    return QtJambiAPI::toJByteArray(env, reinterpret_cast<const jbyte *>(text.data()), jsize(text.length()));
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
                                     || std::is_same_v<NativeType, QByteArray>
#if defined(__cpp_char8_t)
                                     || std::is_same_v<NativeType, std::u8string>
                                     || std::is_same_v<NativeType, std::u8string_view>
#endif
                                     || std::is_same_v<NativeType, std::string>
                                     || std::is_same_v<NativeType, std::string_view>){
                    if constexpr(std::is_same_v<JniType, jbyteArray>){
                        if constexpr(is_pointer){
                            if(in){
                                return QtJambiAPI::toJByteArray(env, reinterpret_cast<const jbyte *>(in->data()), jsize(in->length()));
                            }else
                                return nullptr;
                        }else{
                            return QtJambiAPI::toJByteArray(env, reinterpret_cast<const jbyte *>(in.data()), jsize(in.length()));
                        }
                    }else if constexpr(is_pointer){
                        if(in){
                            const char * data = reinterpret_cast<const char *>(in->data());
                            jstring str;
                            if(decltype(in->length())(qstrlen(data))==in->length())
                                str = env->NewStringUTF(data);
                            else{
                                QString strg;
                                if constexpr(std::is_same_v<NativeType, std::string>){
                                    strg = QString::fromUtf8(QByteArrayView(in->data(), in->length()));
                                }else if constexpr(std::is_same_v<NativeType, std::string_view>){
                                    strg = QString::fromUtf8(QByteArrayView(in->data(), in->length()));
#if defined(__cpp_char8_t)
                                }else if constexpr(std::is_same_v<NativeType, std::u8string_view>
                                                     || std::is_same_v<NativeType, std::u8string>){
                                    strg = QString::fromUtf8(QByteArrayView(in->data(), in->length()));
#endif
                                }else if constexpr(std::is_same_v<NativeType, QByteArrayView>
                                                     || std::is_same_v<NativeType, QByteArray>){
                                    strg = QString::fromUtf8(*in);
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
                            if constexpr(std::is_same_v<NativeType, std::string>){
                                strg = QString::fromUtf8(QByteArrayView(in.data(), in.length()));
                            }else if constexpr(std::is_same_v<NativeType, std::string_view>){
                                strg = QString::fromUtf8(QByteArrayView(in.data(), in.length()));
#if defined(__cpp_char8_t)
                            }else if constexpr(std::is_same_v<NativeType, std::u8string_view>
                                                 || std::is_same_v<NativeType, std::u8string>){
                                strg = QString::fromUtf8(QByteArrayView(in.data(), in.length()));
#endif
                            }else if constexpr(std::is_same_v<NativeType, QByteArrayView>
                                          || std::is_same_v<NativeType, QByteArray>){
                                strg = QString::fromUtf8(in);
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
                    return QtJambiAPI::toJavaString(env, qtjambi_cast_with_args<jobject>(in, std::forward<Args>(args)...));
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
                                result = QtJambiAPI::convertJavaObjectToNative<NativeType>(env, in);
                            }
                            if constexpr(is_complete_v<value_range_converter_from_java_buffer<true, JniType, NativeType, Args...>>){
                                if(auto data = value_range_converter_from_java_buffer<true, JniType, NativeType, Args...>::JBufferPointer(in, args...)){
                                    cast_var_args<Args...>::scope(args...).addDeletion(data);
                                    result = create<NativeType>(data->template constData<QChar>(), data->template size<QChar>());
                                    cast_var_args<Args...>::scope(args...).addDeletion(result);
                                }
                            }
                        }else{
                            Q_STATIC_ASSERT_X(!is_reference || is_const, "Cannot cast jstring to non-const QString&");
                            Q_STATIC_ASSERT_X(!is_pointer || is_const, "Cannot cast jstring to non-const QString*");
                        }
                        if(!result){
                            NativeType buffer;
                            jstring strg = qtjambi_to_jstring<JniType>::cast(env, in);
                            jsize sz = env->GetStringLength(strg);
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            buffer.resize(sz);
                            env->GetStringRegion(strg, 0, sz, reinterpret_cast<jchar *>(buffer.data()));
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            result = create<NativeType>(std::move(buffer));
                            cast_var_args<Args...>::scope(args...).addDeletion(result);
                        }
                        return ptr2ref<is_reference || !is_pointer,NativeType>::value(env, result);
                    }
                }else{
                    if(!env->IsSameObject(in, nullptr)){
                        if constexpr(std::is_same_v<JniType,jstring>){
                            NativeType result;
                            jsize sz = env->GetStringLength(in);
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            result.resize(sz);
                            env->GetStringRegion(in, 0, sz, reinterpret_cast<jchar *>(result.data()));
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            return result;
                        }else{
                            if constexpr(std::is_same_v<JniType,jcharArray>){
                                NativeType result;
                                jsize sz = env->GetArrayLength(in);
                                JavaException::check(env QTJAMBI_STACKTRACEINFO );
                                result.resize(sz);
                                env->GetCharArrayRegion(in, 0, sz, reinterpret_cast<jchar *>(result.data()));
                                return result;
                            }else if constexpr(!std::is_same_v<JniType,jstring>){
                                if(QtJambiAPI::isQStringObject(env, in)){
                                    return QtJambiAPI::convertJavaObjectToNativeReference<NativeType>(env, in);
                                }
                                if constexpr(cast_var_args<Args...>::hasScope
                                            && is_complete_v<value_range_converter_from_java_buffer<true, JniType, NativeType, Args...>>){
                                    if(auto data = value_range_converter_from_java_buffer<true, JniType, NativeType, Args...>::JBufferPointer(in, args...)){
                                        cast_var_args<Args...>::scope(args...).addDeletion(data);
                                        return NativeType(data->template constData<QChar>(), data->template size<QChar>());
                                    }
                                }
                            }
                            NativeType result;
                            jstring strg = qtjambi_to_jstring<JniType>::cast(env, in);
                            jsize sz = env->GetStringLength(strg);
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            result.resize(sz);
                            env->GetStringRegion(strg, 0, sz, reinterpret_cast<jchar *>(result.data()));
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            return result;
                        }
                    }
                    return NativeType{};
                }
            }else if constexpr(std::is_same_v<NativeType, QVariant>){
                if constexpr(is_pointer || is_reference){
                    Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope || !is_reference, "Cannot cast to QVariant& without scope.");
                    Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope || !is_pointer, "Cannot cast to QVariant* without scope.");
                    if constexpr(std::is_same_v<JniType,jstring>){
                        Q_STATIC_ASSERT_X(!is_reference || is_const, "Cannot cast jstring to non-const QVariant&");
                        Q_STATIC_ASSERT_X(!is_pointer || is_const, "Cannot cast jstring to non-const QVariant*");
                    }
                    if constexpr(is_pointer){
                        if(!in)
                            return nullptr;
                    }
                    QByteArray buffer;
                    jstring strg = qtjambi_to_jstring<JniType>::cast(env, in);
                    jsize sz = env->GetStringUTFLength(strg);
                    JavaException::check(env QTJAMBI_STACKTRACEINFO );
                    buffer.resize(sz);
                    env->GetStringUTFRegion(strg, 0, sz, buffer.data());
                    JavaException::check(env QTJAMBI_STACKTRACEINFO );
                    auto result = create<NativeType>(buffer);
                    cast_var_args<Args...>::scope(args...).addDeletion(result);
                    return ptr2ref<is_reference || !is_pointer,NativeType>::value(env, result);
                }else{
                    if(!env->IsSameObject(in, nullptr)){
                        QByteArray result;
                        jstring strg = qtjambi_to_jstring<JniType>::cast(env, in);
                        jsize sz = env->GetStringUTFLength(strg);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        result.resize(sz);
                        env->GetStringUTFRegion(strg, 0, sz, result.data());
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        return result;
                    }
                    return QVariant{};
                }
            }else if constexpr(std::is_same_v<NativeType, QStringView>
                                 || std::is_same_v<NativeType, QAnyStringView>
                                 || std::is_same_v<NativeType, std::u16string_view>){
                if constexpr(is_pointer){
                    if(!in)
                        return nullptr;
                }
                if constexpr(is_pointer || is_reference){
                    Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast to StringView without scope.");
                    NativeType* result{nullptr};
                    if constexpr(!std::is_same_v<JniType,jstring>){
                        if(QtJambiAPI::isQStringObject(env, in)){
                            result = create<NativeType>(QtJambiAPI::convertJavaObjectToNativeReference<QString>(env, in));
                        }else if constexpr(is_complete_v<value_range_converter_from_java_buffer<true, JniType, NativeType, Args...>>){
                            if(auto data = value_range_converter_from_java_buffer<true, JniType, NativeType, Args...>::JBufferPointer(in, args...)){
                                cast_var_args<Args...>::scope(args...).addDeletion(data);
                                result = create<NativeType>(data->template constData<QChar>(), data->template size<QChar>());
                            }
                        }
                        if constexpr(std::is_same_v<NativeType, QAnyStringView>){
                            if(!result){
                                if(QtJambiAPI::isQByteArrayObject(env, in)){
                                    result = create<NativeType>(QtJambiAPI::convertJavaObjectToNativeReference<QByteArray>(env, in));
                                }else if(QtJambiAPI::isQByteArrayViewObject(env, in)){
                                    result = create<NativeType>(QtJambiAPI::convertJavaObjectToNativeReference<QByteArrayView>(env, in));
                                }
                            }
                        }
                    }
                    if(!result){
                        QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/StringAPI, is_complete_v<convert_jstring_to_qchars<NativeType,NativeType>>);
                        result = create<NativeType>(convert_jstring_to_qchars<NativeType,NativeType>::convert(env, cast_var_args<Args...>::scope(args...), qtjambi_to_jstring<JniType>::cast(env, in)));
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
                                }else if constexpr(is_complete_v<value_range_converter_from_java_buffer<true, JniType, NativeType, Args...>>){
                                    if(auto data = value_range_converter_from_java_buffer<true, JniType, NativeType, Args...>::JBufferPointer(in, args...)){
                                        cast_var_args<Args...>::scope(args...).addDeletion(data);
                                        return NativeType(data->template constData<QChar>(), data->template size<QChar>());
                                    }
                                }
                                if constexpr(std::is_same_v<NativeType, QAnyStringView>){
                                    if(QtJambiAPI::isQByteArrayObject(env, in)){
                                        return NativeType(QtJambiAPI::convertJavaObjectToNativeReference<QByteArray>(env, in));
                                    }else if(QtJambiAPI::isQByteArrayViewObject(env, in)){
                                        return NativeType(QtJambiAPI::convertJavaObjectToNativeReference<QByteArrayView>(env, in));
                                    }
                                }
                            }
                            QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/StringAPI, is_complete_v<convert_jstring_to_qchars<NativeType,NativeType>>);
                            return convert_jstring_to_qchars<NativeType,NativeType>::convert(env, cast_var_args<Args...>::scope(args...), qtjambi_to_jstring<JniType>::cast(env, in));
                        }
                    }else{
                        return NativeType{};
                    }
                }
            }else if constexpr(std::is_same_v<NativeType, std::string>
#if defined(__cpp_char8_t)
                                 || std::is_same_v<NativeType, std::u8string>
#endif
                                 || std::is_same_v<NativeType, std::u16string>){
                if constexpr(is_pointer || is_reference){
                    if constexpr(is_pointer){
                        if(!in)
                            return nullptr;
                    }
                    NativeType* result{nullptr};
                    if constexpr(!std::is_same_v<JniType,jstring>){
                        if constexpr(std::is_same_v<NativeType, std::string>){
                            if(QtJambiAPI::isQByteArrayObject(env, in)){
                                result = create<NativeType>(QtJambiAPI::convertJavaObjectToNativeReference<QByteArray>(env, in).toStdString());
                            }else if(QtJambiAPI::isQByteArrayViewObject(env, in)){
                                result = create<NativeType>(QtJambiAPI::convertJavaObjectToNativeReference<QByteArrayView>(env, in).toStdString());
                                cast_var_args<Args...>::scope(args...).addDeletion(result);
                            }else if(QtJambiAPI::isQVariantObject(env, in)){
                                result = create<NativeType>(QtJambiAPI::convertJavaObjectToNativeReference<QVariant>(env, in).template value<NativeType>());
                                cast_var_args<Args...>::scope(args...).addDeletion(result);
                            }
#if defined(__cpp_char8_t)
                        }else if constexpr(std::is_same_v<NativeType, std::u8string>){
                            if(QtJambiAPI::isQByteArrayObject(env, in)){
                                const QByteArray& ba = QtJambiAPI::convertJavaObjectToNativeReference<QByteArray>(env, in);
                                result = create<NativeType>(reinterpret_cast<const char8_t*>(ba.constData()), ba.size());
                                cast_var_args<Args...>::scope(args...).addDeletion(result);
                            }else if(QtJambiAPI::isQByteArrayViewObject(env, in)){
                                const QByteArrayView& ba = QtJambiAPI::convertJavaObjectToNativeReference<QByteArrayView>(env, in);
                                result = create<NativeType>(reinterpret_cast<const char8_t*>(ba.constData()), ba.size());
                                cast_var_args<Args...>::scope(args...).addDeletion(result);
                            }else if(QtJambiAPI::isQVariantObject(env, in)){
                                result = create<NativeType>(QtJambiAPI::convertJavaObjectToNativeReference<QVariant>(env, in).template value<NativeType>());
                                cast_var_args<Args...>::scope(args...).addDeletion(result);
                            }
#endif
                        }else{
                            if(QtJambiAPI::isQStringObject(env, in)){
                                result = create<NativeType>(QtJambiAPI::convertJavaObjectToNativeReference<QString>(env, in).toStdU16String());
                                cast_var_args<Args...>::scope(args...).addDeletion(result);
                            }else if(QtJambiAPI::isQVariantObject(env, in)){
                                result = create<NativeType>(QtJambiAPI::convertJavaObjectToNativeReference<QVariant>(env, in).template value<NativeType>());
                                cast_var_args<Args...>::scope(args...).addDeletion(result);
                            }
                        }
                    }else{
                        Q_STATIC_ASSERT_X(!is_reference || is_const, "Cannot cast jstring to non-const std::string&");
                        Q_STATIC_ASSERT_X(!is_pointer || is_const, "Cannot cast jstring to non-const std::string*");
                    }
                    if(!result){
                        NativeType buffer;
                        jstring strg = qtjambi_to_jstring<JniType>::cast(env, in);
                        if constexpr(std::is_same_v<NativeType, std::string>
#if defined(__cpp_char8_t)
                                      || std::is_same_v<NativeType, std::u8string>
#endif
                                      ){
                            jsize sz = env->GetStringUTFLength(strg);
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            buffer.resize(sz);
                            env->GetStringUTFRegion(strg, 0, sz, reinterpret_cast<char*>(buffer.data()));
                        }else{
                            jsize sz = env->GetStringLength(strg);
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            buffer.resize(sz);
                            env->GetStringRegion(strg, 0, sz, reinterpret_cast<jchar*>(buffer.data()));
                        }
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        result = create<NativeType>(std::move(buffer));
                        cast_var_args<Args...>::scope(args...).addDeletion(result);
                    }
                    return ptr2ref<is_reference || !is_pointer,NativeType>::value(env, result);
                }else{
                    if(!env->IsSameObject(in, nullptr)){
                        if constexpr(!std::is_same_v<JniType,jstring>){
                            if constexpr(std::is_same_v<NativeType, std::string>){
                                if(QtJambiAPI::isQByteArrayObject(env, in)){
                                    return QtJambiAPI::convertJavaObjectToNativeReference<QByteArray>(env, in).toStdString();
                                }else if(QtJambiAPI::isQByteArrayViewObject(env, in)){
                                    return QtJambiAPI::convertJavaObjectToNativeReference<QByteArrayView>(env, in).toStdString();
                                }else if(QtJambiAPI::isQVariantObject(env, in)){
                                    return QtJambiAPI::convertJavaObjectToNativeReference<QVariant>(env, in).template value<NativeType>();
                                }
#if defined(__cpp_char8_t)
                            }else if constexpr(std::is_same_v<NativeType, std::u8string>){
                                if(QtJambiAPI::isQByteArrayObject(env, in)){
                                    const QByteArray& ba = QtJambiAPI::convertJavaObjectToNativeReference<QByteArray>(env, in);
                                    return NativeType(reinterpret_cast<const char8_t*>(ba.constData()), ba.size());
                                }else if(QtJambiAPI::isQByteArrayViewObject(env, in)){
                                    const QByteArrayView& ba = QtJambiAPI::convertJavaObjectToNativeReference<QByteArrayView>(env, in);
                                    return NativeType(reinterpret_cast<const char8_t*>(ba.constData()), ba.size());
                                }else if(QtJambiAPI::isQVariantObject(env, in)){
                                    return QtJambiAPI::convertJavaObjectToNativeReference<QVariant>(env, in).template value<NativeType>();
                                }
#endif
                            }else{
                                if(QtJambiAPI::isQStringObject(env, in)){
                                    return QtJambiAPI::convertJavaObjectToNativeReference<QString>(env, in).toStdU16String();
                                }else if(QtJambiAPI::isQVariantObject(env, in)){
                                    return QtJambiAPI::convertJavaObjectToNativeReference<QVariant>(env, in).template value<NativeType>();
                                }
                            }
                        }
                        NativeType result;
                        jstring strg = qtjambi_to_jstring<JniType>::cast(env, in);
                        if constexpr(std::is_same_v<NativeType, std::string>
#if defined(__cpp_char8_t)
                                      || std::is_same_v<NativeType, std::u8string>
#endif
                                      ){
                            jsize sz = env->GetStringUTFLength(strg);
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            result.resize(sz);
                            env->GetStringUTFRegion(strg, 0, sz, reinterpret_cast<char*>(result.data()));
                        }else{
                            jsize sz = env->GetStringLength(strg);
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            result.resize(sz);
                            env->GetStringRegion(strg, 0, sz, reinterpret_cast<jchar*>(result.data()));
                        }
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        return result;
                    }
                    return NativeType{};
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
                        NativeType* result{nullptr};
                        if constexpr(!std::is_same_v<JniType,jstring>){
                            if(QtJambiAPI::isQByteArrayObject(env, in)){
                                result = QtJambiAPI::convertJavaObjectToNative<NativeType>(env, in);
                            }else if(QtJambiAPI::isQByteArrayViewObject(env, in)){
                                result = create<NativeType>(QtJambiAPI::convertJavaObjectToNativeReference<QByteArrayView>(env, in));
                                cast_var_args<Args...>::scope(args...).addDeletion(result);
                            }else if(QtJambiAPI::isQStringObject(env, in)){
                                result = create<NativeType>(QtJambiAPI::convertJavaObjectToNative<QString>(env, in)->toUtf8());
                                cast_var_args<Args...>::scope(args...).addDeletion(result);
                            }else if(QtJambiAPI::isQVariantObject(env, in)){
                                result = create<NativeType>(QtJambiAPI::convertJavaObjectToNative<QVariant>(env, in)->template value<NativeType>());
                                cast_var_args<Args...>::scope(args...).addDeletion(result);
                            }
                        }else{
                            Q_STATIC_ASSERT_X(!is_reference || is_const, "Cannot cast jstring to non-const QByteArray&");
                            Q_STATIC_ASSERT_X(!is_pointer || is_const, "Cannot cast jstring to non-const QByteArray*");
                        }
                        if(!result){
                            QByteArray buffer;
                            jstring strg = qtjambi_to_jstring<JniType>::cast(env, in);
                            jsize sz = env->GetStringUTFLength(strg);
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            buffer.resize(sz);
                            env->GetStringUTFRegion(strg, 0, sz, buffer.data());
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                            result = create<NativeType>(std::move(buffer));
                            cast_var_args<Args...>::scope(args...).addDeletion(result);
                        }
                        return ptr2ref<is_reference || !is_pointer,QByteArray>::value(env, result);
                    }
                }else{
                    if(!env->IsSameObject(in, nullptr)){
                        if constexpr(!std::is_same_v<JniType,jstring>){
                            if(QtJambiAPI::isQByteArrayObject(env, in)){
                                return QtJambiAPI::convertJavaObjectToNativeReference<NativeType>(env, in);
                            }else if(QtJambiAPI::isQByteArrayViewObject(env, in)){
                                return NativeType(QtJambiAPI::convertJavaObjectToNativeReference<QByteArrayView>(env, in));
                            }else if(QtJambiAPI::isQStringObject(env, in)){
                                return QtJambiAPI::convertJavaObjectToNative<QString>(env, in)->toUtf8();
                            }else if(QtJambiAPI::isQVariantObject(env, in)){
                                return QtJambiAPI::convertJavaObjectToNative<QVariant>(env, in)->template value<NativeType>();
                            }
                        }
                        QByteArray result;
                        jstring strg = qtjambi_to_jstring<JniType>::cast(env, in);
                        jsize sz = env->GetStringUTFLength(strg);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        result.resize(sz);
                        env->GetStringUTFRegion(strg, 0, sz, result.data());
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        return result;
                    }
                    return QByteArray{};
                }
            }else if constexpr(std::is_same_v<NativeType, QLatin1String>
                                 || std::is_same_v<NativeType, QUtf8StringView>
                                 || std::is_same_v<NativeType, QByteArrayView>
#if defined(__cpp_char8_t)
                                 || std::is_same_v<NativeType, std::u8string_view>
#endif
                                 || std::is_same_v<NativeType, std::string_view>){
                if constexpr(is_pointer){
                    if(!in)
                        return nullptr;
                }
                if constexpr(is_pointer || is_reference){
                    Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast without scope.");
                    NativeType* result{nullptr};
                    if constexpr(!std::is_same_v<JniType,jstring>){
                        if(QtJambiAPI::isQByteArrayObject(env, in)){
                            const QByteArray& ba = QtJambiAPI::convertJavaObjectToNativeReference<QByteArray>(env, in);
#if defined(__cpp_char8_t)
                            if constexpr(std::is_same_v<NativeType, std::u8string_view>)
                                result = create<NativeType>(reinterpret_cast<const char8_t*>(ba.constData()), ba.size());
                            else
#endif
                            result = create<NativeType>(ba.constData(), ba.size());
                        }else if(QtJambiAPI::isQByteArrayViewObject(env, in)){
                            const QByteArrayView& ba = QtJambiAPI::convertJavaObjectToNativeReference<QByteArrayView>(env, in);
#if defined(__cpp_char8_t)
                            if constexpr(std::is_same_v<NativeType, std::u8string_view>)
                                result = create<NativeType>(reinterpret_cast<const char8_t*>(ba.constData()), ba.size());
                            else
#endif
                            result = create<NativeType>(ba.constData(), ba.size());
                        }else if constexpr(is_complete_v<value_range_converter_from_java_buffer<true, JniType, NativeType, Args...>>){
                            if(auto data = value_range_converter_from_java_buffer<true, JniType, NativeType, Args...>::JBufferPointer(in, args...)){
                                cast_var_args<Args...>::scope(args...).addDeletion(data);
#if defined(__cpp_char8_t)
                                if constexpr(std::is_same_v<NativeType, std::u8string_view>)
                                    result = create<NativeType>(data->template constData<char8_t>(), data->template size<char8_t>());
                                else
#endif
                                result = create<NativeType>(data->template constData<char>(), data->template size<char>());
                            }
                        }
                    }
                    if(!result){
                        QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/StringAPI, is_complete_v<convert_jstring_to_chars<NativeType,NativeType>>);
                        result = create<NativeType>(convert_jstring_to_chars<NativeType,NativeType>::convert(env, cast_var_args<Args...>::scope(args...), qtjambi_to_jstring<JniType>::cast(env, in)));
                    }
                    cast_var_args<Args...>::scope(args...).addDeletion(result);
                    return ptr2ref<is_reference || !is_pointer,NativeType>::value(env, result);
                }else{
                    if(!env->IsSameObject(in, nullptr)){
                        if constexpr(!cast_var_args<Args...>::hasScope && !std::is_same_v<JniType,jstring>){
                            if(QtJambiAPI::isQByteArrayObject(env, in)){
                                const QByteArray& ba = QtJambiAPI::convertJavaObjectToNativeReference<QByteArray>(env, in);
#if defined(__cpp_char8_t)
                                if constexpr(std::is_same_v<NativeType, std::u8string_view>)
                                    return NativeType(reinterpret_cast<const char8_t*>(ba.constData()), ba.size());
                                else
#endif
                                return NativeType(ba.constData(), ba.size());
                            }else if(QtJambiAPI::isQByteArrayViewObject(env, in)){
                                const QByteArrayView& ba = QtJambiAPI::convertJavaObjectToNativeReference<QByteArrayView>(env, in);
#if defined(__cpp_char8_t)
                                if constexpr(std::is_same_v<NativeType, std::u8string_view>)
                                    return NativeType(reinterpret_cast<const char8_t*>(ba.constData()), ba.size());
                                else
#endif
                                return NativeType(ba.constData(), ba.size());
                            }
                            JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null"), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                            return NativeType{};
                        }else{
                            if constexpr(!std::is_same_v<JniType,jstring>){
                                if(QtJambiAPI::isQByteArrayObject(env, in)){
                                    const QByteArray& ba = QtJambiAPI::convertJavaObjectToNativeReference<QByteArray>(env, in);
#if defined(__cpp_char8_t)
                                    if constexpr(std::is_same_v<NativeType, std::u8string_view>)
                                        return NativeType(reinterpret_cast<const char8_t*>(ba.constData()), ba.size());
                                    else
#endif
                                    return NativeType(ba.constData(), ba.size());
                                }else if(QtJambiAPI::isQByteArrayViewObject(env, in)){
                                    const QByteArrayView& ba = QtJambiAPI::convertJavaObjectToNativeReference<QByteArrayView>(env, in);
#if defined(__cpp_char8_t)
                                    if constexpr(std::is_same_v<NativeType, std::u8string_view>)
                                        return NativeType(reinterpret_cast<const char8_t*>(ba.constData()), ba.size());
                                    else
#endif
                                    return NativeType(ba.constData(), ba.size());
                                }else if constexpr(is_complete_v<value_range_converter_from_java_buffer<true, JniType, NativeType, Args...>>){
                                    if(auto data = value_range_converter_from_java_buffer<true, JniType, NativeType, Args...>::JBufferPointer(in, args...)){
                                        cast_var_args<Args...>::scope(args...).addDeletion(data);
#if defined(__cpp_char8_t)
                                        if constexpr(std::is_same_v<NativeType, std::u8string_view>)
                                            return NativeType(data->template constData<char8_t>(), data->template size<char8_t>());
                                        else
#endif
                                        return NativeType(data->template constData<char>(), data->template size<char>());
                                    }
                                }
                            }
                            QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/StringAPI, is_complete_v<convert_jstring_to_chars<NativeType,NativeType>>);
                            return convert_jstring_to_chars<NativeType,NativeType>::convert(env, cast_var_args<Args...>::scope(args...), qtjambi_to_jstring<JniType>::cast(env, in));
                        }
                    }else{
                        return NativeType{};
                    }
                }
            }else if constexpr(std::is_same_v<JniType, jstring> && (std::is_same_v<NativeType, char>
                                                                      || std::is_same_v<NativeType, QLatin1Char>
                                                                      || std::is_same_v<NativeType, std::byte>) && is_pointer){
                Q_STATIC_ASSERT_X(is_const, "Cannot cast jstring to non-const char*");
                Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast to const char* without scope.");
                QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/StringAPI, is_complete_v<convert_jstring_to_chars<NativeType,const char*>>);
                const char* array = convert_jstring_to_chars<NativeType,const char*>::convert(env, cast_var_args<Args...>::scope(args...), in);
                if constexpr(std::is_same_v<NativeType, QLatin1Char>){
                    return reinterpret_cast<const QLatin1Char*>(array);
                }else if constexpr(std::is_same_v<NativeType, std::byte>){
                    return reinterpret_cast<const std::byte*>(array);
                }else{
                    return array;
                }
            }else if constexpr(std::is_same_v<JniType, jstring> && (std::is_same_v<NativeType, QChar>
                                                                      || std::is_same_v<NativeType, char16_t>) && is_pointer){
                Q_STATIC_ASSERT_X(is_const, "Cannot cast jstring to non-const QChar*");
                Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast to const QChar* without scope.");
                QTJAMBI_CAST_INCLUDE_CHECK(QtJambi/StringAPI, is_complete_v<convert_jstring_to_qchars<NativeType,const NativeType*>>);
                return convert_jstring_to_qchars<NativeType,const NativeType*>::convert(env, cast_var_args<Args...>::scope(args...), in);
            }else{
                Q_STATIC_ASSERT_X(is_const, "Cannot cast jstring to unknown type");
                return {};
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

}// namespace QtJambiPrivate

#endif // QTJAMBI_CAST_OBJECT_H
