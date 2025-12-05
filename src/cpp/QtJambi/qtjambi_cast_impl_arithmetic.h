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

#ifndef QTJAMBI_CAST_IMPL_ARITHMETIC_H
#define QTJAMBI_CAST_IMPL_ARITHMETIC_H

#include "qtjambi_cast_impl_util.h"

namespace QtJambiPrivate {

template<bool forward,
         typename ArithmeticType,
         typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_arithmetic_plain_cast{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;
    typedef typename std::conditional<forward, NativeType_in, ArithmeticType>::type In;
    typedef typename std::conditional<forward, ArithmeticType, NativeType_out>::type Out;

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
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1Char>::type, QLatin1Char>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    static ArithmeticType cast(NativeType_in in, Args...){
        return ArithmeticType(deref_ptr<is_pointer,const QLatin1Char>::deref(in).toLatin1());
    }
};

template<typename ArithmeticType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_arithmetic_plain_cast<false,
                                 ArithmeticType,
                                 QLatin1Char, is_pointer, is_const, is_reference, is_rvalue, Args...>{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1Char>::type, QLatin1Char>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    static NativeType_out cast(ArithmeticType in, Args... args){
        Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        Q_STATIC_ASSERT_X(!is_reference || cast_var_args<Args...>::hasScope, "Cannot cast to QLatin1Char reference without scope");
        Q_STATIC_ASSERT_X(!is_pointer || cast_var_args<Args...>::hasScope, "Cannot cast to QLatin1Char pointer without scope");
        if constexpr(is_pointer || is_reference){
            QLatin1Char* result = new QLatin1Char(char(in));
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
    typedef typename std::conditional<is_const, typename std::add_const<QChar>::type, QChar>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    static ArithmeticType cast(NativeType_in in, Args...){
        return ArithmeticType(deref_ptr<is_pointer,const QChar>::deref(in).unicode());
    }
};

template<typename ArithmeticType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_arithmetic_plain_cast<false,
                                 ArithmeticType,
                                 QChar, is_pointer, is_const, is_reference, is_rvalue, Args...>{
    typedef typename std::conditional<is_const, typename std::add_const<QChar>::type, QChar>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    static NativeType_out cast(ArithmeticType in, Args... args){
        Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        Q_STATIC_ASSERT_X(!is_reference || cast_var_args<Args...>::hasScope, "Cannot cast to QChar reference without scope");
        Q_STATIC_ASSERT_X(!is_pointer || cast_var_args<Args...>::hasScope, "Cannot cast to QChar pointer without scope");
        if constexpr(is_pointer || is_reference){
            QChar* result = new QChar(ushort(in));
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
    typedef typename std::conditional<is_const, typename std::add_const<QVariant>::type, QVariant>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    static NativeType_out cast(ArithmeticType in, Args... args){
        Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        Q_STATIC_ASSERT_X(!is_reference || cast_var_args<Args...>::hasScope, "Cannot cast to QVariant reference without scope");
        Q_STATIC_ASSERT_X(!is_pointer || cast_var_args<Args...>::hasScope, "Cannot cast to QVariant pointer without scope");
        if constexpr(is_pointer || is_reference){
            QVariant* result = new QVariant(QVariant::fromValue<ArithmeticType>(in));
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
    typedef typename std::conditional<is_const, typename std::add_const<NativeType<Args...>>::type, NativeType<Args...>>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
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
    typedef typename std::conditional<is_const, typename std::add_const<NativeType<K,T>>::type, NativeType<K,T>>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
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
    typedef typename std::conditional<is_const, typename std::add_const<String>::type, String>::type String_c;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<String_c>::type, typename std::add_lvalue_reference<String_c>::type>::type In;

    static double cast(In in, Args...){
        return deref_ptr<is_pointer,const String>::deref(in).toDouble();
    }
};

template<typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct arithmetic_from_string_decider<float,String,is_pointer,is_const,is_reference, is_rvalue, Args...>{
    typedef typename std::conditional<is_const, typename std::add_const<String>::type, String>::type String_c;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<String_c>::type, typename std::add_lvalue_reference<String_c>::type>::type In;

    static float cast(In in, Args...){
        return deref_ptr<is_pointer,const String>::deref(in).toFloat();
    }
};

template<typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct arithmetic_from_string_decider<bool,String,is_pointer,is_const,is_reference, is_rvalue, Args...>{
    typedef typename std::conditional<is_const, typename std::add_const<String>::type, String>::type String_c;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<String_c>::type, typename std::add_lvalue_reference<String_c>::type>::type In;

    static bool cast(In in, Args...){
        return !deref_ptr<is_pointer,const String>::deref(in).isEmpty();
    }
};

template<typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct arithmetic_from_string_decider<qlonglong,String,is_pointer,is_const,is_reference, is_rvalue, Args...>{
    typedef typename std::conditional<is_const, typename std::add_const<String>::type, String>::type String_c;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<String_c>::type, typename std::add_lvalue_reference<String_c>::type>::type In;

    static qlonglong cast(In in, Args...){
        return deref_ptr<is_pointer,const String>::deref(in).toLongLong();
    }
};

template<typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct arithmetic_from_string_decider<qulonglong,String,is_pointer,is_const,is_reference, is_rvalue, Args...>{
    typedef typename std::conditional<is_const, typename std::add_const<String>::type, String>::type String_c;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<String_c>::type, typename std::add_lvalue_reference<String_c>::type>::type In;

    static qulonglong cast(In in, Args...){
        return deref_ptr<is_pointer,const String>::deref(in).toULongLong();
    }
};

template<typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct arithmetic_from_string_decider<long,String,is_pointer,is_const,is_reference, is_rvalue, Args...>{
    typedef typename std::conditional<is_const, typename std::add_const<String>::type, String>::type String_c;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<String_c>::type, typename std::add_lvalue_reference<String_c>::type>::type In;

    static long cast(In in, Args...){
        return deref_ptr<is_pointer,const String>::deref(in).toLong();
    }
};

template<typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct arithmetic_from_string_decider<ulong,String,is_pointer,is_const,is_reference,is_rvalue,Args...>{
    typedef typename std::conditional<is_const, typename std::add_const<String>::type, String>::type String_c;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<String_c>::type, typename std::add_lvalue_reference<String_c>::type>::type In;

    static ulong cast(In in, Args...){
        return deref_ptr<is_pointer,const String>::deref(in).toULong();
    }
};

template<typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct arithmetic_from_string_decider<int,String,is_pointer,is_const,is_reference,is_rvalue,Args...>{
    typedef typename std::conditional<is_const, typename std::add_const<String>::type, String>::type String_c;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<String_c>::type, typename std::add_lvalue_reference<String_c>::type>::type In;

    static int cast(In in, Args...){
        return deref_ptr<is_pointer,const String>::deref(in).toInt();
    }
};

template<typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct arithmetic_from_string_decider<uint,String,is_pointer,is_const,is_reference,is_rvalue,Args...>{
    typedef typename std::conditional<is_const, typename std::add_const<String>::type, String>::type String_c;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<String_c>::type, typename std::add_lvalue_reference<String_c>::type>::type In;

    static uint cast(In in, Args...){
        return deref_ptr<is_pointer,const String>::deref(in).toUInt();
    }
};

template<typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct arithmetic_from_string_decider<short,String,is_pointer,is_const,is_reference,is_rvalue,Args...>{
    typedef typename std::conditional<is_const, typename std::add_const<String>::type, String>::type String_c;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<String_c>::type, typename std::add_lvalue_reference<String_c>::type>::type In;

    static short cast(In in, Args...){
        return deref_ptr<is_pointer,const String>::deref(in).toShort();
    }
};

template<typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct arithmetic_from_string_decider<ushort,String,is_pointer,is_const,is_reference,is_rvalue,Args...>{
    typedef typename std::conditional<is_const, typename std::add_const<String>::type, String>::type String_c;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<String_c>::type, typename std::add_lvalue_reference<String_c>::type>::type In;

    static ushort cast(In in, Args...){
        return deref_ptr<is_pointer,const String>::deref(in).toUShort();
    }
};

template<typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct arithmetic_from_string_decider<char,String,is_pointer,is_const,is_reference,is_rvalue,Args...>{
    typedef typename std::conditional<is_const, typename std::add_const<String>::type, String>::type String_c;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<String_c>::type, typename std::add_lvalue_reference<String_c>::type>::type In;

    static char cast(In in, Args...){
        return char(deref_ptr<is_pointer,const String>::deref(in).toShort());
    }
};

template<typename String, bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct arithmetic_from_string_decider<uchar,String,is_pointer,is_const,is_reference,is_rvalue,Args...>{
    typedef typename std::conditional<is_const, typename std::add_const<String>::type, String>::type String_c;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<String_c>::type, typename std::add_lvalue_reference<String_c>::type>::type In;

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
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::add_pointer<NativeType>::type NativeType_ptr;

    static NativeType_out cast(ArithmeticType in, Args... args){
        Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        if constexpr(is_pointer || is_reference){
            Q_STATIC_ASSERT_X(!is_reference || cast_var_args<Args...>::hasScope, "Cannot cast to QString reference without scope");
            Q_STATIC_ASSERT_X(!is_pointer || cast_var_args<Args...>::hasScope, "Cannot cast to QString pointer without scope");
            QString* result = new QString(QString::number(in));
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
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<forward, NativeType_in, ArithmeticType>::type In;
    typedef typename std::conditional<forward, ArithmeticType, NativeType_out>::type Out;

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
                NativeType* result = new NativeType(T(in));
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

#ifdef QURL_H
template<bool forward, typename ArithmeticType,
         bool is_pointer, bool is_const, bool is_reference, bool is_rvalue, typename... Args>
struct qtjambi_arithmetic_container2_cast<forward,
                                          ArithmeticType,
                                          QUrlTwoFlags, is_pointer, is_const, is_reference, is_rvalue, QUrl::UrlFormattingOption, QUrl::ComponentFormattingOption, Args...>{
    typedef QUrl::FormattingOptions NativeType;
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::conditional<is_rvalue, typename std::add_rvalue_reference<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    typedef typename std::conditional<forward, NativeType_in, ArithmeticType>::type In;
    typedef typename std::conditional<forward, ArithmeticType, NativeType_out>::type Out;

    static Out cast(In in, Args... args){
        Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        if constexpr(forward){
            if constexpr(is_pointer){
                return ArithmeticType(*in);
            }else{
                return ArithmeticType(in);
            }
        }else{
            Q_STATIC_ASSERT_X(!is_reference || cast_var_args<Args...>::hasScope, "Cannot cast to QUrl::FormattingOptions reference without scope");
            Q_STATIC_ASSERT_X(!is_pointer || cast_var_args<Args...>::hasScope, "Cannot cast to QUrl::FormattingOptions pointer without scope");
            if constexpr(is_pointer || is_reference){
                QUrl::FormattingOptions* result = new QUrl::FormattingOptions(int(in));
                cast_var_args<Args...>::scope(args...).addDeletion(result);
                if constexpr(is_pointer){
                    return result;
                }else{
                    return *result;
                }
            }else{
                return QUrl::FormattingOptions(int(in));
            }
        }
    }
};

#endif //QURL_H

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_IMPL_ARITHMETIC_H
