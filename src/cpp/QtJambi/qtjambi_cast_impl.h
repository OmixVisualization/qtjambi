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

#ifndef QTJAMBI_CAST_IMPL_H
#define QTJAMBI_CAST_IMPL_H

#include "qtjambi_cast_impl_arithmetic.h"
#include "qtjambi_cast_impl_enum.h"
#include "qtjambi_cast_impl_jnitype.h"

namespace QtJambiPrivate {

template<class O, class T, typename... Args>
struct qtjambi_nojni_plain_cast{
    typedef typename std::remove_reference<O>::type O_noref;
    typedef typename std::remove_reference<T>::type T_noref;
    typedef typename std::remove_cv<O_noref>::type O_noconst;
    typedef typename std::remove_cv<T_noref>::type T_noconst;
    static constexpr bool is_same = std::is_same<O_noref, T_noref>::value;
    static constexpr bool is_O_arithmetic = std::is_arithmetic<O_noconst>::value;
    static constexpr bool is_O_enum = std::is_enum<O_noconst>::value;

    static O cast(T in, Args... args){
        Q_STATIC_ASSERT(unuseArgs(sizeof(args)...));
        if constexpr(is_same){
            return in;
        }else if constexpr(is_O_arithmetic || is_O_enum){
            return static_cast<O>(in);
        }else if constexpr(std::is_same_v<T,QtJambiNativeID>){
            if constexpr(std::is_pointer_v<O>){
                return QtJambiAPI::objectFromNativeId<O_noconst>(in);
            }else if constexpr(std::is_reference_v<O>){
                if constexpr(std::is_const_v<typename qtjambi_cast_types<O>::T_noref>
                              && std::is_default_constructible_v<O_noconst>){
                    return QtJambiAPI::valueReferenceFromNativeId<O_noconst>(in);
                }else{
                    auto env = cast_var_args<Args...>::env(args...);
                    return QtJambiAPI::objectReferenceFromNativeId<O_noconst>(env, in);
                }
                return QtJambiAPI::objectFromNativeId<O_noconst>(in);
            }else{
                return QtJambiAPI::valueFromNativeId<O_noconst>(in);
            }
        }else{
            Q_STATIC_ASSERT_X(is_same, "Cannot cast types");
            throw;
        }
    }
};

template<class O, class T_in, bool is_rvalue, typename... Args>
static constexpr auto qtjambi_cast_impl() {
    typedef std::conditional_t<std::is_array_v<std::remove_reference_t<T_in>>, std::decay_t<T_in>, T_in> T;
    if constexpr(is_jni_type<O>::value){
        return qtjambi_jobject_cast<true,
                                typename qtjambi_cast_types<O>::T_noconst,
                                typename std::remove_cv<typename std::remove_pointer<typename qtjambi_cast_types<T>::T_noconst>::type>::type,
                                std::is_pointer<typename qtjambi_cast_types<T>::T_noref>::value,
                                std::is_const<typename std::conditional<std::is_pointer<typename qtjambi_cast_types<T>::T_noref>::value, typename std::remove_pointer<typename qtjambi_cast_types<T>::T_noref>::type, typename qtjambi_cast_types<T>::T_noref>::type>::value,
                                std::is_reference<T>::value && !std::is_pointer<typename qtjambi_cast_types<T>::T_noref>::value, is_rvalue, Args...>{};
    }else if constexpr(is_jni_type<typename qtjambi_cast_types<T>::T_noconst>::value){
        return qtjambi_jobject_cast<false,
                                typename qtjambi_cast_types<T>::T_noconst,
                                typename std::remove_cv<typename std::remove_pointer<typename qtjambi_cast_types<O>::T_noconst>::type>::type,
                                std::is_pointer<typename qtjambi_cast_types<O>::T_noref>::value,
                                std::is_const<typename std::conditional<std::is_pointer<typename qtjambi_cast_types<O>::T_noref>::value, typename std::remove_pointer<typename qtjambi_cast_types<O>::T_noref>::type, typename qtjambi_cast_types<O>::T_noref>::type>::value,
                                std::is_reference<O>::value && !std::is_pointer<typename qtjambi_cast_types<O>::T_noref>::value, is_rvalue, Args...>{};

    }else if constexpr(std::is_arithmetic<O>::value
                         || std::is_same<O,QChar>::value
                         || std::is_same<O,QLatin1Char>::value){
        return qtjambi_arithmetic_cast<true,
                                       O,
                                       typename std::remove_cv<typename std::remove_pointer<typename qtjambi_cast_types<T>::T_noconst>::type>::type,
                                       std::is_pointer<typename qtjambi_cast_types<T>::T_noref>::value,
                                       std::is_const<typename std::conditional<std::is_pointer<typename qtjambi_cast_types<T>::T_noref>::value, typename std::remove_pointer<typename qtjambi_cast_types<T>::T_noref>::type, typename qtjambi_cast_types<T>::T_noref>::type>::value,
                                       std::is_reference<T>::value && !std::is_pointer<typename qtjambi_cast_types<T>::T_noref>::value, is_rvalue, Args...>{};

    }else if constexpr(std::is_arithmetic<typename qtjambi_cast_types<T>::T_noconst>::value
                         || std::is_same<typename qtjambi_cast_types<T>::T_noconst,QChar>::value
                         || std::is_same<typename qtjambi_cast_types<T>::T_noconst,QLatin1Char>::value){
        return qtjambi_arithmetic_cast<false,
                                       typename qtjambi_cast_types<T>::T_noconst,
                                       typename std::remove_cv<typename std::remove_pointer<typename qtjambi_cast_types<O>::T_noconst>::type>::type,
                                       std::is_pointer<typename qtjambi_cast_types<O>::T_noref>::value,
                                       std::is_const<typename std::conditional<std::is_pointer<typename qtjambi_cast_types<O>::T_noref>::value, typename std::remove_pointer<typename qtjambi_cast_types<O>::T_noref>::type, typename qtjambi_cast_types<O>::T_noref>::type>::value,
                                       std::is_reference<O>::value && !std::is_pointer<typename qtjambi_cast_types<O>::T_noref>::value, is_rvalue, Args...>{};

    }else if constexpr(std::is_enum<O>::value){
        return qtjambi_enum_cast<true,
                                   O,
                                   typename std::remove_cv<typename std::remove_pointer<typename qtjambi_cast_types<T>::T_noconst>::type>::type,
                                   std::is_pointer<typename qtjambi_cast_types<T>::T_noref>::value,
                                   std::is_const<typename std::conditional<std::is_pointer<typename qtjambi_cast_types<T>::T_noref>::value, typename std::remove_pointer<typename qtjambi_cast_types<T>::T_noref>::type, typename qtjambi_cast_types<T>::T_noref>::type>::value,
                                   std::is_reference<T>::value && !std::is_pointer<typename qtjambi_cast_types<T>::T_noref>::value, is_rvalue, Args...>{};

    }else if constexpr(std::is_enum<typename qtjambi_cast_types<T>::T_plain>::value){
        return qtjambi_enum_cast<false,
                                   typename qtjambi_cast_types<T>::T_noconst,
                                   typename std::remove_cv<typename std::remove_pointer<typename qtjambi_cast_types<O>::T_noconst>::type>::type,
                                   std::is_pointer<typename qtjambi_cast_types<O>::T_noref>::value,
                                   std::is_const<typename std::conditional<std::is_pointer<typename qtjambi_cast_types<O>::T_noref>::value, typename std::remove_pointer<typename qtjambi_cast_types<O>::T_noref>::type, typename qtjambi_cast_types<O>::T_noref>::type>::value,
                                   std::is_reference<O>::value && !std::is_pointer<typename qtjambi_cast_types<O>::T_noref>::value, is_rvalue, Args...>{};

    }else{
        return qtjambi_nojni_plain_cast<O,T, Args...>{};
    }
}

template<class O, class T, typename... Args>
struct qtjambi_cast : decltype(qtjambi_cast_impl<O, std::conditional_t<std::is_rvalue_reference_v<T>, std::add_rvalue_reference_t<std::remove_reference_t<T>>, T>, std::is_rvalue_reference_v<T>, Args...>()){
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_IMPL_H
