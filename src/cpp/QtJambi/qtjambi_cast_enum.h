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

#ifndef QTJAMBI_CAST_ENUM_H
#define QTJAMBI_CAST_ENUM_H

#include "qtjambi_cast_util.h"

namespace QtJambiPrivate {

template<bool forward,
         typename EnumType,
         typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_enum_plain_cast{
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::add_pointer_t<NativeType> NativeType_ptr;
    typedef std::conditional_t<forward, NativeType_in, EnumType> In;
    typedef std::conditional_t<forward, EnumType, NativeType_out> Out;

    static Out cast(In in, Args... args){
        Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        if constexpr(forward){
            return EnumType(deref_ptr<is_pointer,const NativeType>::deref(in));
        }else{
            Q_STATIC_ASSERT_X(!is_reference || cast_var_args<Args...>::hasScope, "Cannot cast to enumerator reference without scope");
            Q_STATIC_ASSERT_X(!is_pointer || cast_var_args<Args...>::hasScope, "Cannot cast to enumerator pointer without scope");
            if constexpr(is_pointer || is_reference){
                NativeType* result = new NativeType(NativeType(in));
                cast_var_args<Args...>::scope(args...).addDeletion(result);
                if constexpr(is_pointer){
                    return result;
                }else{
                    return *result;
                }
            }else{
                return NativeType(in);
            }
        }
    }
};

template<bool forward, typename EnumType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename Args, template<typename... Ts> class NativeType, typename... Ts>
static constexpr auto qtjambi_enum_container_cast_impl(const NativeType<Ts...>&);

template<bool forward, typename EnumType, typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
static constexpr auto qtjambi_enum_cast_impl() {
    if constexpr(is_template<NativeType>::value){
        return decltype(qtjambi_enum_container_cast_impl<forward, EnumType, is_pointer, is_const, is_reference, is_rvalue, std::tuple<Args...>>(std::declval<const NativeType&>())){};
    }else{
        return qtjambi_enum_plain_cast<forward, EnumType, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>{};
    }
}

template<bool forward,
         typename EnumType,
         typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_enum_cast : decltype(qtjambi_enum_cast_impl<forward, EnumType, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>()){
};

template<bool forward,
         typename EnumType,
         template<typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_enum_container1_cast{
    Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
};

template<bool forward,
         typename EnumType,
         template<typename K, typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename... Args>
struct qtjambi_enum_container2_cast{
    Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
};

template<bool forward,
         typename JniType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename Args, size_t P, typename... Ts>
struct qtjambi_enum_container_cast{
};

template<bool forward, typename EnumType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename Args, template<typename... Ts> class NativeType, typename... Ts>
static constexpr auto qtjambi_enum_container_cast_impl(const NativeType<Ts...>&){
    return qtjambi_enum_container_cast<forward, EnumType, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args, sizeof...(Ts), Ts...>{};
}

template<bool forward,
         typename EnumType,
         template<typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename T, typename... Args>
struct qtjambi_enum_container_cast<forward, EnumType, NativeType, is_pointer, is_const, is_reference, is_rvalue, std::tuple<Args...>, 1, T>
    : qtjambi_enum_container1_cast<forward, EnumType, NativeType, is_pointer, is_const, is_reference, is_rvalue, T, Args...>{
};

template<bool forward,
         typename EnumType,
         template<typename T, typename K> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename T, typename K, typename... Args>
struct qtjambi_enum_container_cast<forward, EnumType, NativeType, is_pointer, is_const, is_reference, is_rvalue, std::tuple<Args...>, 2, T, K>
    : qtjambi_enum_container2_cast<forward, EnumType, NativeType, is_pointer, is_const, is_reference, is_rvalue, T, K, Args...>{
};

template<typename EnumType, size_t size, typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_enum_from_string_cast{
    typedef std::conditional_t<is_const, std::add_const_t<String>, String> String_c;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<String_c>, std::add_lvalue_reference_t<String_c>> In;

    static EnumType cast(In in, Args...){
        return EnumType(deref_ptr<is_pointer,const String>::deref(in).toLong());
    }
};

template<typename EnumType, typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_enum_from_string_cast<EnumType,sizeof(qlonglong),String,is_pointer,is_const,is_reference,is_rvalue,Args...>{
    typedef std::conditional_t<is_const, std::add_const_t<String>, String> String_c;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<String_c>, std::add_lvalue_reference_t<String_c>> In;

    static EnumType cast(In in, Args...){
        return EnumType(deref_ptr<is_pointer,const String>::deref(in).toLong());
    }
};

template<typename EnumType, typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_enum_from_string_cast<EnumType,sizeof(qint32),String,is_pointer,is_const,is_reference,is_rvalue,Args...>{
    typedef std::conditional_t<is_const, std::add_const_t<String>, String> String_c;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<String_c>, std::add_lvalue_reference_t<String_c>> In;

    static EnumType cast(In in, Args...){
        return EnumType(deref_ptr<is_pointer,const String>::deref(in).toInt());
    }
};

template<typename EnumType, typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_enum_from_string_cast<EnumType,sizeof(qint16),String,is_pointer,is_const,is_reference,is_rvalue,Args...>{
    typedef std::conditional_t<is_const, std::add_const_t<String>, String> String_c;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<String_c>, std::add_lvalue_reference_t<String_c>> In;

    static EnumType cast(In in, Args...){
        return EnumType(deref_ptr<is_pointer,const String>::deref(in).toShort());
    }
};

template<typename EnumType, typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_enum_from_string_cast<EnumType,sizeof(qint8),String,is_pointer,is_const,is_reference,is_rvalue,Args...>{
    typedef std::conditional_t<is_const, std::add_const_t<String>, String> String_c;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<String_c>, std::add_lvalue_reference_t<String_c>> In;

    static EnumType cast(In in, Args...){
        return EnumType(qint8(deref_ptr<is_pointer,const String>::deref(in).toInt()));
    }
};

template<typename EnumType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_enum_plain_cast<true,
                                 EnumType,
                                 QString, is_pointer, is_const, is_reference, is_rvalue, Args...>
        : qtjambi_enum_from_string_cast<EnumType, sizeof(EnumType),QString,is_pointer,is_const,is_reference,is_rvalue,Args...>{};

template<typename EnumType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_enum_plain_cast<true,
                         EnumType,
                         QStringView, is_pointer, is_const, is_reference, is_rvalue, Args...>
    : qtjambi_enum_from_string_cast<EnumType, sizeof(EnumType),QStringView,is_pointer,is_const,is_reference,is_rvalue, Args...>{};

template<typename EnumType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_enum_plain_cast<true,
                         EnumType,
                         QAnyStringView, is_pointer, is_const, is_reference, is_rvalue, Args...>
    : qtjambi_enum_from_string_cast<EnumType, sizeof(EnumType),QAnyStringView,is_pointer,is_const,is_reference,is_rvalue,Args...>{};

template<typename EnumType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_enum_plain_cast<true,
                         EnumType,
                         QUtf8StringView, is_pointer, is_const, is_reference, is_rvalue, Args...>
    : qtjambi_enum_from_string_cast<EnumType, sizeof(EnumType),QUtf8StringView,is_pointer,is_const,is_reference,is_rvalue,Args...>{};

template<typename EnumType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_enum_plain_cast<true,
                         EnumType,
                         QLatin1String, is_pointer, is_const, is_reference, is_rvalue, Args...>
    : qtjambi_enum_from_string_cast<EnumType, sizeof(EnumType),QLatin1String,is_pointer,is_const,is_reference,is_rvalue,Args...>{};

template<typename EnumType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_enum_plain_cast<false,
                                 EnumType,
                                 QString, is_pointer, is_const, is_reference, is_rvalue, Args...>{
    typedef QString NativeType;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::add_pointer_t<NativeType> NativeType_ptr;

    static NativeType_out cast(EnumType in, Args... args){
        Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        Q_STATIC_ASSERT_X(!is_reference || cast_var_args<Args...>::hasScope, "Cannot cast to QString reference");
        Q_STATIC_ASSERT_X(!is_pointer || cast_var_args<Args...>::hasScope, "Cannot cast to QString pointer");
        if constexpr(is_pointer || is_reference){
            QString* result = new QString(QString::number(std::underlying_type_t<EnumType>(in)));
            if constexpr(is_pointer){
                return result;
            }else{
                return *result;
            }
        }else{
            return QString::number(std::underlying_type_t<EnumType>(in));
        }
    }
};

template<bool forward, typename EnumType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_enum_container1_cast<forward,
                                          EnumType,
                                          QFlags, is_pointer, is_const, is_reference, is_rvalue, T, Args...>{
    typedef QFlags<T> NativeType;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef NativeType_cr NativeType_out;
    typedef std::conditional_t<forward, NativeType_in, EnumType> In;
    typedef std::conditional_t<forward, EnumType, NativeType_out> Out;

    static Out cast(In in, Args... args){
        Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        if constexpr(forward){
            return EnumType(typename NativeType::Int(deref_ptr<is_pointer,const NativeType>::deref(in)));
        }else{
            if constexpr(is_pointer || is_reference){
                Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast to QFlags<T>*");
                NativeType* result = new NativeType(in);
                cast_var_args<Args...>::scope(args...).addDeletion(result);
                if constexpr(is_pointer){
                    return result;
                }else{
                    auto env = cast_var_args<Args...>::env(args...);
                    return qtjambi_deref_value<NativeType, is_default_constructible_v<NativeType>, is_copy_constructible_v<NativeType>, is_const, is_reference>::deref(env, result);
                }
            }else{
                return NativeType(typename NativeType::Int(in));
            }
        }
    }
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_ENUM_H
