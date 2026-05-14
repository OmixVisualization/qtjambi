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

#ifndef QTJAMBI_CAST_ARITHMETIC_H
#define QTJAMBI_CAST_ARITHMETIC_H

#include "qtjambi_cast_util.h"
#include "qtjambiapi_boxed.h"

namespace QtJambiPrivate {

template<bool forward,
         typename ArithmeticType,
         typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_arithmetic_plain_cast{
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::add_pointer_t<NativeType> NativeType_ptr;
    typedef std::conditional_t<forward, NativeType_in, ArithmeticType> In;
    typedef std::conditional_t<forward, ArithmeticType, NativeType_out> Out;

    static Out cast(In in, Args... args){
        Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        if constexpr(forward){
            if constexpr(is_pointer){
                return ArithmeticType(*in);
            }else{
                return ArithmeticType(in);
            }
        }else{
            Q_STATIC_ASSERT_X(!is_reference || cast_var_args<Args...>::hasScope, "Cannot cast to arithmetic reference without scope");
            Q_STATIC_ASSERT_X(!is_pointer || cast_var_args<Args...>::hasScope, "Cannot cast to arithmetic pointer without scope");
            if constexpr(is_pointer || is_reference){
                NativeType* result = create<NativeType>(NativeType(in));
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

template<bool forward, typename ArithmeticType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename Args, template<typename... Ts> class NativeType, typename... Ts>
static constexpr auto qtjambi_arithmetic_template_cast_impl(const NativeType<Ts...>&);

template<bool forward, typename ArithmeticType, typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
static constexpr auto qtjambi_arithmetic_cast_impl() {
    if constexpr(is_template<NativeType>::value){
        return decltype(qtjambi_arithmetic_template_cast_impl<forward, ArithmeticType, is_pointer, is_const, is_reference, is_rvalue, std::tuple<Args...>>(std::declval<const NativeType&>())){};
    }else{
        return qtjambi_arithmetic_plain_cast<forward, ArithmeticType, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>{};
    }
}

template<bool forward,
         typename ArithmeticType,
         typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_arithmetic_cast : decltype(qtjambi_arithmetic_cast_impl<forward, ArithmeticType, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args...>()){
};

template<typename ArithmeticType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_arithmetic_plain_cast<true,
                                 ArithmeticType,
                                 QLatin1Char, is_pointer, is_const, is_reference, is_rvalue, Args...>{
    typedef std::conditional_t<is_const, std::add_const_t<QLatin1Char>, QLatin1Char> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;

    static ArithmeticType cast(NativeType_in in, Args...){
        return ArithmeticType(deref_ptr<is_pointer,const QLatin1Char>::deref(in).toLatin1());
    }
};

template<typename ArithmeticType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_arithmetic_plain_cast<false,
                                 ArithmeticType,
                                 QLatin1Char, is_pointer, is_const, is_reference, is_rvalue, Args...>{
    typedef std::conditional_t<is_const, std::add_const_t<QLatin1Char>, QLatin1Char> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;

    static NativeType_out cast(ArithmeticType in, Args... args){
        Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        Q_STATIC_ASSERT_X(!is_reference || cast_var_args<Args...>::hasScope, "Cannot cast to QLatin1Char reference without scope");
        Q_STATIC_ASSERT_X(!is_pointer || cast_var_args<Args...>::hasScope, "Cannot cast to QLatin1Char pointer without scope");
        if constexpr(is_pointer || is_reference){
            QLatin1Char* result = create<QLatin1Char>(char(in));
            cast_var_args<Args...>::scope(args...).addDeletion(result);
            if constexpr(is_pointer){
                return result;
            }else{
                return *result;
            }
        }else{
            return QLatin1Char(char(in));
        }
    }
};

template<typename ArithmeticType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_arithmetic_plain_cast<true,
                                 ArithmeticType,
                                 QChar, is_pointer, is_const, is_reference, is_rvalue, Args...>{
    typedef std::conditional_t<is_const, std::add_const_t<QChar>, QChar> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;

    static ArithmeticType cast(NativeType_in in, Args...){
        return ArithmeticType(deref_ptr<is_pointer,const QChar>::deref(in).unicode());
    }
};

template<typename ArithmeticType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_arithmetic_plain_cast<false,
                                 ArithmeticType,
                                 QChar, is_pointer, is_const, is_reference, is_rvalue, Args...>{
    typedef std::conditional_t<is_const, std::add_const_t<QChar>, QChar> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;

    static NativeType_out cast(ArithmeticType in, Args... args){
        Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        Q_STATIC_ASSERT_X(!is_reference || cast_var_args<Args...>::hasScope, "Cannot cast to QChar reference without scope");
        Q_STATIC_ASSERT_X(!is_pointer || cast_var_args<Args...>::hasScope, "Cannot cast to QChar pointer without scope");
        if constexpr(is_pointer || is_reference){
            QChar* result = create<QChar>(ushort(in));
            cast_var_args<Args...>::scope(args...).addDeletion(result);
            if constexpr(is_pointer){
                return result;
            }else{
                return *result;
            }
        }else{
            return QChar(ushort(in));
        }
    }
};

template<typename ArithmeticType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_arithmetic_plain_cast<false,
                                 ArithmeticType,
                                 QVariant, is_pointer, is_const, is_reference, is_rvalue, Args...>{
    typedef std::conditional_t<is_const, std::add_const_t<QVariant>, QVariant> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;

    static NativeType_out cast(ArithmeticType in, Args... args){
        Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        Q_STATIC_ASSERT_X(!is_reference || cast_var_args<Args...>::hasScope, "Cannot cast to QVariant reference without scope");
        Q_STATIC_ASSERT_X(!is_pointer || cast_var_args<Args...>::hasScope, "Cannot cast to QVariant pointer without scope");
        if constexpr(is_pointer || is_reference){
            QVariant* result = create<QVariant>(QVariant::fromValue<ArithmeticType>(in));
            cast_var_args<Args...>::scope(args...).addDeletion(result);
            if constexpr(is_pointer){
                return result;
            }else{
                return *result;
            }
        }else{
            return QVariant::fromValue<ArithmeticType>(in);
        }
    }
};

template<bool forward,
         typename ArithmeticType,
         template<typename...> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         int parameterCount, typename... Args>
struct qtjambi_arithmetic_container_cast_decider{
    typedef std::conditional_t<is_const, std::add_const_t<NativeType<Args...>>, NativeType<Args...>> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
};

template<bool forward,
         typename ArithmeticType,
         template<typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename T, typename... Args>
struct qtjambi_arithmetic_container1_cast{
    Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
};

template<bool forward,
         typename ArithmeticType,
         template<typename K, typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename K, typename T, typename... Args>
struct qtjambi_arithmetic_container2_cast{
    typedef std::conditional_t<is_const, std::add_const_t<NativeType<K,T>>, NativeType<K,T>> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
};

template<bool forward,
         typename ArithmeticType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename Args, size_t P, typename... Ts>
struct qtjambi_arithmetic_template_cast{
};

template<bool forward, typename ArithmeticType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename Args, template<typename... Ts> class NativeType, typename... Ts>
static constexpr auto qtjambi_arithmetic_template_cast_impl(const NativeType<Ts...>&){
    return qtjambi_arithmetic_template_cast<forward, ArithmeticType, NativeType, is_pointer, is_const, is_reference, is_rvalue, Args, sizeof...(Ts), Ts...>{};
}

template<bool forward,
         typename ArithmeticType,
         template<typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename T, typename... Args>
struct qtjambi_arithmetic_template_cast<forward, ArithmeticType, NativeType, is_pointer, is_const, is_reference, is_rvalue, std::tuple<Args...>, 1, T>
 : qtjambi_arithmetic_container1_cast<forward, ArithmeticType, NativeType, is_pointer, is_const, is_reference, is_rvalue, T, Args...>{
};

template<bool forward,
         typename ArithmeticType,
         template<typename T, typename K> class NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename T, typename K, typename... Args>
struct qtjambi_arithmetic_template_cast<forward, ArithmeticType, NativeType, is_pointer, is_const, is_reference, is_rvalue, std::tuple<Args...>, 2, T, K>
 : qtjambi_arithmetic_container2_cast<forward, ArithmeticType, NativeType, is_pointer, is_const, is_reference, is_rvalue, T, K, Args...>{
};

template<typename ArithmeticType, typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct arithmetic_from_string_decider{
};

template<typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct arithmetic_from_string_decider<double,String,is_pointer,is_const,is_reference, is_rvalue, Args...>{
    typedef std::conditional_t<is_const, std::add_const_t<String>, String> String_c;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<String_c>, std::add_lvalue_reference_t<String_c>> In;

    static double cast(In in, Args...){
        return deref_ptr<is_pointer,const String>::deref(in).toDouble();
    }
};

template<typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct arithmetic_from_string_decider<float,String,is_pointer,is_const,is_reference, is_rvalue, Args...>{
    typedef std::conditional_t<is_const, std::add_const_t<String>, String> String_c;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<String_c>, std::add_lvalue_reference_t<String_c>> In;

    static float cast(In in, Args...){
        return deref_ptr<is_pointer,const String>::deref(in).toFloat();
    }
};

template<typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct arithmetic_from_string_decider<bool,String,is_pointer,is_const,is_reference, is_rvalue, Args...>{
    typedef std::conditional_t<is_const, std::add_const_t<String>, String> String_c;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<String_c>, std::add_lvalue_reference_t<String_c>> In;

    static bool cast(In in, Args...){
        return !deref_ptr<is_pointer,const String>::deref(in).isEmpty();
    }
};

template<typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct arithmetic_from_string_decider<qlonglong,String,is_pointer,is_const,is_reference, is_rvalue, Args...>{
    typedef std::conditional_t<is_const, std::add_const_t<String>, String> String_c;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<String_c>, std::add_lvalue_reference_t<String_c>> In;

    static qlonglong cast(In in, Args...){
        return deref_ptr<is_pointer,const String>::deref(in).toLongLong();
    }
};

template<typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct arithmetic_from_string_decider<qulonglong,String,is_pointer,is_const,is_reference, is_rvalue, Args...>{
    typedef std::conditional_t<is_const, std::add_const_t<String>, String> String_c;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<String_c>, std::add_lvalue_reference_t<String_c>> In;

    static qulonglong cast(In in, Args...){
        return deref_ptr<is_pointer,const String>::deref(in).toULongLong();
    }
};

template<typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct arithmetic_from_string_decider<long,String,is_pointer,is_const,is_reference, is_rvalue, Args...>{
    typedef std::conditional_t<is_const, std::add_const_t<String>, String> String_c;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<String_c>, std::add_lvalue_reference_t<String_c>> In;

    static long cast(In in, Args...){
        return deref_ptr<is_pointer,const String>::deref(in).toLong();
    }
};

template<typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct arithmetic_from_string_decider<ulong,String,is_pointer,is_const,is_reference,is_rvalue,Args...>{
    typedef std::conditional_t<is_const, std::add_const_t<String>, String> String_c;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<String_c>, std::add_lvalue_reference_t<String_c>> In;

    static ulong cast(In in, Args...){
        return deref_ptr<is_pointer,const String>::deref(in).toULong();
    }
};

template<typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct arithmetic_from_string_decider<int,String,is_pointer,is_const,is_reference,is_rvalue,Args...>{
    typedef std::conditional_t<is_const, std::add_const_t<String>, String> String_c;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<String_c>, std::add_lvalue_reference_t<String_c>> In;

    static int cast(In in, Args...){
        return deref_ptr<is_pointer,const String>::deref(in).toInt();
    }
};

template<typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct arithmetic_from_string_decider<uint,String,is_pointer,is_const,is_reference,is_rvalue,Args...>{
    typedef std::conditional_t<is_const, std::add_const_t<String>, String> String_c;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<String_c>, std::add_lvalue_reference_t<String_c>> In;

    static uint cast(In in, Args...){
        return deref_ptr<is_pointer,const String>::deref(in).toUInt();
    }
};

template<typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct arithmetic_from_string_decider<short,String,is_pointer,is_const,is_reference,is_rvalue,Args...>{
    typedef std::conditional_t<is_const, std::add_const_t<String>, String> String_c;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<String_c>, std::add_lvalue_reference_t<String_c>> In;

    static short cast(In in, Args...){
        return deref_ptr<is_pointer,const String>::deref(in).toShort();
    }
};

template<typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct arithmetic_from_string_decider<ushort,String,is_pointer,is_const,is_reference,is_rvalue,Args...>{
    typedef std::conditional_t<is_const, std::add_const_t<String>, String> String_c;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<String_c>, std::add_lvalue_reference_t<String_c>> In;

    static ushort cast(In in, Args...){
        return deref_ptr<is_pointer,const String>::deref(in).toUShort();
    }
};

template<typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct arithmetic_from_string_decider<char,String,is_pointer,is_const,is_reference,is_rvalue,Args...>{
    typedef std::conditional_t<is_const, std::add_const_t<String>, String> String_c;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<String_c>, std::add_lvalue_reference_t<String_c>> In;

    static char cast(In in, Args...){
        return char(deref_ptr<is_pointer,const String>::deref(in).toShort());
    }
};

template<typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct arithmetic_from_string_decider<uchar,String,is_pointer,is_const,is_reference,is_rvalue,Args...>{
    typedef std::conditional_t<is_const, std::add_const_t<String>, String> String_c;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<String_c>, std::add_lvalue_reference_t<String_c>> In;

    static uchar cast(In in, Args...){
        return uchar(deref_ptr<is_pointer,const String>::deref(in).toUShort());
    }
};

template<typename ArithmeticType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_arithmetic_plain_cast<true,
                                 ArithmeticType,
                                 QString, is_pointer, is_const, is_reference, is_rvalue, Args...>
    : arithmetic_from_string_decider<ArithmeticType,QString,is_pointer,is_const,is_reference,is_rvalue,Args...>{};

template<typename ArithmeticType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_arithmetic_plain_cast<true,
                                     ArithmeticType,
                                     QStringView, is_pointer, is_const, is_reference, is_rvalue, Args...>
    : arithmetic_from_string_decider<ArithmeticType,QStringView,is_pointer,is_const,is_reference,is_rvalue,Args...>{};

template<typename ArithmeticType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_arithmetic_plain_cast<true,
                                     ArithmeticType,
                                     QAnyStringView, is_pointer, is_const, is_reference, is_rvalue, Args...>
    : arithmetic_from_string_decider<ArithmeticType,QAnyStringView,is_pointer,is_const,is_reference,is_rvalue,Args...>{};

template<typename ArithmeticType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_arithmetic_plain_cast<true,
                                     ArithmeticType,
                                     QUtf8StringView, is_pointer, is_const, is_reference, is_rvalue, Args...>
    : arithmetic_from_string_decider<ArithmeticType,QUtf8StringView,is_pointer,is_const,is_reference,is_rvalue,Args...>{};

template<typename ArithmeticType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_arithmetic_plain_cast<true,
                                     ArithmeticType,
                                     QLatin1String, is_pointer, is_const, is_reference, is_rvalue, Args...>
    : arithmetic_from_string_decider<ArithmeticType,QLatin1String,is_pointer,is_const,is_reference,is_rvalue,Args...>{};

template<typename ArithmeticType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_arithmetic_plain_cast<false,
                                 ArithmeticType,
                                 QString, is_pointer, is_const, is_reference, is_rvalue, Args...>{
    typedef QString NativeType;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::add_pointer_t<NativeType> NativeType_ptr;

    static NativeType_out cast(ArithmeticType in, Args... args){
        Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        if constexpr(is_pointer || is_reference){
            Q_STATIC_ASSERT_X(!is_reference || cast_var_args<Args...>::hasScope, "Cannot cast to QString reference without scope");
            Q_STATIC_ASSERT_X(!is_pointer || cast_var_args<Args...>::hasScope, "Cannot cast to QString pointer without scope");
            QString* result = create<NativeType>(QString::number(in));
            cast_var_args<Args...>::scope(args...).addDeletion(result);
            if constexpr(is_pointer){
                return result;
            }else{
                return *result;
            }
        }else{
            return QString::number(in);
        }
    }
};
template<bool forward, typename ArithmeticType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename T, typename... Args>
struct qtjambi_arithmetic_container1_cast<forward,
                                          ArithmeticType,
                                          QFlags, is_pointer, is_const, is_reference, is_rvalue, T, Args...>{
    typedef QFlags<T> NativeType;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::conditional_t<forward, NativeType_in, ArithmeticType> In;
    typedef std::conditional_t<forward, ArithmeticType, NativeType_out> Out;

    static Out cast(In in, Args... args){
        Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        if constexpr(forward){
            if constexpr(is_pointer){
                return ArithmeticType(*in);
            }else{
                return ArithmeticType(in);
            }
        }else{
            Q_STATIC_ASSERT_X(!is_reference || cast_var_args<Args...>::hasScope, "Cannot cast to QFlags<T> reference without scope");
            Q_STATIC_ASSERT_X(!is_pointer || cast_var_args<Args...>::hasScope, "Cannot cast to QFlags<T> pointer without scope");
            if constexpr(is_pointer || is_reference){
                NativeType* result = create<NativeType>(T(in));
                cast_var_args<Args...>::scope(args...).addDeletion(result);
                if constexpr(is_pointer){
                    return result;
                }else{
                    return *result;
                }
            }else{
                return NativeType(T(in));
            }
        }
    }
};

template<class O, class T, class I, typename... Args>
static constexpr auto qtjambi_cast_array();

template<class O, class T, class I, typename... Args>
struct qtjambi_jobject_arithmetic_array_cast : decltype(qtjambi_cast_array<O,T,I,Args...>()){
};

template<bool forward, typename JniType, typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_jobject_arithmetic_cast{
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::add_pointer_t<NativeType_c> NativeType_ptr;
    typedef std::conditional_t<forward, NativeType_in, JniType> In;
    typedef std::conditional_t<forward, JniType, NativeType_out> Out;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(jni_type<JniType>::isArray){
            if constexpr(forward){
                return qtjambi_jobject_arithmetic_array_cast<JniType, NativeType_ptr, int, Args...>::cast(&in, 1, std::forward<Args>(args)...);
            }else{
                return *qtjambi_jobject_arithmetic_array_cast<NativeType_ptr, JniType, int, Args...>::cast(in, 1, std::forward<Args>(args)...);
            }
        }else if constexpr(std::is_same_v<JniType,jstring>){
            auto env = cast_var_args<Args...>::env(args...);
            if constexpr(forward){
                Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jstring without JNIEnv.");
                if constexpr(is_pointer){
                    if constexpr(std::is_integral_v<NativeType>){
                        if constexpr(sizeof(NativeType)==sizeof(char)){
                            return in ? env->NewStringUTF(reinterpret_cast<const char*>(in)) : nullptr;
                            JavaException::check(env QTJAMBI_STACKTRACEINFO );
                        }else if constexpr(sizeof(NativeType)==sizeof(jchar)){
                            return in ? env->NewString(reinterpret_cast<const jchar*>(in), jsize(std::wcslen(reinterpret_cast<const wchar_t*>(in)))) : nullptr;
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
            }else{
                if constexpr(is_pointer)
                    if(!in)
                        return nullptr;
                Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasScope, "Cannot cast jstring to const char* without scope.");
                if constexpr(std::is_integral_v<NativeType> || std::is_same_v<NativeType, QLatin1Char> || std::is_same_v<NativeType, QChar>){
                    if constexpr(sizeof(NativeType)==sizeof(jbyte)){
                        if constexpr(is_const){
                            if constexpr(is_pointer || is_reference){
                                constexpr bool hasStringAPI = is_complete_v<convert_jstring_to_chars<NativeType,const char*>>;
                                Q_STATIC_ASSERT_X(hasStringAPI, "Cannot cast without including <QtJambi/StringAPI>");
                                const char* array = convert_jstring_to_chars<NativeType,const char*>::convert(env, cast_var_args<Args...>::scope(args...), in);
                                if constexpr(is_pointer){
                                    return reinterpret_cast<const NativeType*>(array);
                                }else{
                                    return *reinterpret_cast<const NativeType*>(array);
                                }
                            }else{
                                QByteArray buffer;
                                jsize sz = qMin<jsize>(1, env->GetStringUTFLength(in));
                                JavaException::check(env QTJAMBI_STACKTRACEINFO );
                                buffer.resize(sz);
                                env->GetStringUTFRegion(in, 0, sz, buffer.data());
                                JavaException::check(env QTJAMBI_STACKTRACEINFO );
                                return *buffer.data();
                            }
                        }else{
                            if constexpr(std::is_same_v<NativeType, QLatin1Char>){
                                Q_STATIC_ASSERT_X(!is_reference || is_const, "Cannot cast jstring to non-const QLatin1Char&");
                                Q_STATIC_ASSERT_X(!is_pointer || is_const, "Cannot cast jstring to non-const QLatin1Char*");
                            }else{
                                Q_STATIC_ASSERT_X(!is_reference || is_const, "Cannot cast jstring to non-const char&");
                                Q_STATIC_ASSERT_X(!is_pointer || is_const, "Cannot cast jstring to non-const char*");
                            }
                        }
                    }else if constexpr(sizeof(NativeType)==sizeof(jchar)){
                        if constexpr(is_const){
                            if constexpr(is_pointer || is_reference){
                                constexpr bool hasStringAPI = is_complete_v<convert_jstring_to_qchars<NativeType,const char16_t*>>;
                                Q_STATIC_ASSERT_X(hasStringAPI, "Cannot cast without including <QtJambi/StringAPI>");
                                const char16_t* array = convert_jstring_to_qchars<NativeType,const char16_t*>::convert(env, cast_var_args<Args...>::scope(args...), in);
                                if constexpr(is_pointer){
                                    return reinterpret_cast<const NativeType*>(array);
                                }else{
                                    return *reinterpret_cast<const NativeType*>(array);
                                }
                            }else{
                                QString buffer;
                                jsize sz = qMin<jsize>(1, env->GetStringLength(in));
                                JavaException::check(env QTJAMBI_STACKTRACEINFO );
                                buffer.resize(sz);
                                env->GetStringRegion(in, 0, sz, reinterpret_cast<jchar *>(buffer.data()));
                                JavaException::check(env QTJAMBI_STACKTRACEINFO );
                                return *reinterpret_cast<const NativeType*>(buffer.data());
                            }
                        }else{
                            if constexpr(std::is_same_v<NativeType, QChar>){
                                Q_STATIC_ASSERT_X(!is_reference || is_const, "Cannot cast jstring to non-const QChar&");
                                Q_STATIC_ASSERT_X(!is_pointer || is_const, "Cannot cast jstring to non-const QChar*");
                            }else{
                                Q_STATIC_ASSERT_X(!is_reference || is_const, "Cannot cast jstring to non-const char16_t&");
                                Q_STATIC_ASSERT_X(!is_pointer || is_const, "Cannot cast jstring to non-const char16_t*");
                            }
                        }
                    }else{
                        Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
                    }
                }else{
                    Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
                }
            }
        }else{
            if constexpr(forward){
                Q_STATIC_ASSERT_X(cast_var_args<Args...>::hasJNIEnv, "Cannot cast to jobject without JNIEnv.");
                if constexpr(is_pointer){
                    if constexpr(std::is_same_v<NativeType,bool>){
                        return QtJambiAPI::toJavaBooleanObject(env, bool(*in));
                    }else if constexpr(std::is_integral_v<NativeType>){
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
                    }else if constexpr(std::is_floating_point_v<NativeType>){
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
                    }else if constexpr(std::is_integral_v<NativeType>){
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
                    }else if constexpr(std::is_floating_point_v<NativeType>){
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
                    NativeType* result = create<NativeType>(NativeType(0));
                    cast_var_args<Args...>::scope(args...).addDeletion(result);
                    if constexpr(std::is_same_v<NativeType,bool>){
                        *result = NativeType(QtJambiAPI::fromJavaBooleanObject(env, in));
                    }else if constexpr(std::is_integral_v<NativeType>){
                        Q_STATIC_ASSERT_X(!is_reference || is_const, "Cannot cast jobject to non-const integer reference");
                        Q_STATIC_ASSERT_X(!is_pointer || is_const, "Cannot cast jobject to non-const integer pointer");
                        if constexpr(sizeof(NativeType)==sizeof(jbyte)){
                            Q_STATIC_ASSERT_X(!is_reference || is_const, "Cannot cast jobject to non-const char&");
                            Q_STATIC_ASSERT_X(!is_pointer || is_const, "Cannot cast jobject to non-const char*");
                            *result = NativeType(QtJambiAPI::fromJavaByteObject(env, in));
                        }else if constexpr(sizeof(NativeType)==sizeof(jshort)){
                            Q_STATIC_ASSERT_X(!is_reference || is_const, "Cannot cast jobject to non-const short&");
                            Q_STATIC_ASSERT_X(!is_pointer || is_const, "Cannot cast jobject to non-const short*");
                            *result = NativeType(QtJambiAPI::fromJavaShortObject(env, in));
                        }else if constexpr(sizeof(NativeType)==sizeof(jint)){
                            Q_STATIC_ASSERT_X(!is_reference || is_const, "Cannot cast jobject to non-const int&");
                            Q_STATIC_ASSERT_X(!is_pointer || is_const, "Cannot cast jobject to non-const int*");
                            *result = NativeType(QtJambiAPI::fromJavaIntegerObject(env, in));
                        }else if constexpr(sizeof(NativeType)==sizeof(jlong)){
                            Q_STATIC_ASSERT_X(!is_reference || is_const, "Cannot cast jobject to non-const long long&");
                            Q_STATIC_ASSERT_X(!is_pointer || is_const, "Cannot cast jobject to non-const long long*");
                            *result = NativeType(QtJambiAPI::fromJavaLongObject(env, in));
                        }else{
                            Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
                        }
                    }else if constexpr(std::is_floating_point_v<NativeType>){
                        if constexpr(sizeof(NativeType)==sizeof(jfloat)){
                            Q_STATIC_ASSERT_X(!is_reference || is_const, "Cannot cast jobject to non-const float&");
                            Q_STATIC_ASSERT_X(!is_pointer || is_const, "Cannot cast jobject to non-const float*");
                            *result = NativeType(QtJambiAPI::fromJavaFloatObject(env, in));
                        }else if constexpr(sizeof(NativeType)==sizeof(jdouble)){
                            Q_STATIC_ASSERT_X(!is_reference || is_const, "Cannot cast jobject to non-const double&");
                            Q_STATIC_ASSERT_X(!is_pointer || is_const, "Cannot cast jobject to non-const double*");
                            *result = NativeType(QtJambiAPI::fromJavaDoubleObject(env, in));
                        }else{
                            Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
                        }
                    }else if constexpr(std::is_same_v<NativeType,QChar>){
                        Q_STATIC_ASSERT_X(!is_reference || is_const, "Cannot cast jobject to non-const QChar&");
                        Q_STATIC_ASSERT_X(!is_pointer || is_const, "Cannot cast jobject to non-const QChar*");
                        *result = NativeType(QtJambiAPI::fromJavaCharacterObject(env, in));
                    }else if constexpr(std::is_same_v<NativeType,QLatin1Char>){
                        Q_STATIC_ASSERT_X(!is_reference || is_const, "Cannot cast jobject to non-const QLatin1Char&");
                        Q_STATIC_ASSERT_X(!is_pointer || is_const, "Cannot cast jobject to non-const QLatin1Char*");
                        *result = NativeType(QtJambiAPI::fromJavaByteObject(env, in));
                    }else if constexpr(std::is_same_v<NativeType,std::byte>){
                        Q_STATIC_ASSERT_X(!is_reference || is_const, "Cannot cast jobject to non-const std::byte&");
                        Q_STATIC_ASSERT_X(!is_pointer || is_const, "Cannot cast jobject to non-const std::byte*");
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
                    }else if constexpr(std::is_integral_v<NativeType>){
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
                    }else if constexpr(std::is_floating_point_v<NativeType>){
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
    }
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_ARITHMETIC_H
