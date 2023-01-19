/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "qtjambi_cast_impl_util.h"
#include "qtjambi_cast_impl_container_sequential.h"
#include "qtjambi_cast_impl_container_associative.h"
#include "qtjambi_cast_impl_arithmetic.h"
#include "qtjambi_cast_impl_enum.h"
#include "qtjambi_cast_impl_jnitype.h"

template<class O, class T>
constexpr O qtjambi_cast(JNIEnv *env, T& in);
template<class O, class T>
constexpr O qtjambi_cast(JNIEnv *env, T& in, const char* nativeTypeName);
template<class O, class T>
constexpr O qtjambi_cast(JNIEnv *env, const T& in);
template<class O, class T>
constexpr O qtjambi_cast(JNIEnv *env, const T& in, const char* nativeTypeName);
template<class O, class T>
constexpr O qtjambi_cast(JNIEnv *env, T* in);
template<class O, class T>
constexpr O qtjambi_cast(JNIEnv *env, T* in, const char* nativeTypeName);

namespace QtJambiPrivate {

template<bool has_scope,
         typename O, bool o_is_arithmetic, bool o_is_enum, bool o_is_jni_type,
         typename T, bool t_is_arithmetic, bool t_is_enum, bool t_is_jni_type,
         bool is_same>
struct qtjambi_cast_decider{
    Q_STATIC_ASSERT_X(false && !is_same, "Cannot cast types");
    constexpr static int cast(JNIEnv *, ...){return 0;}
};

// the same type
template<bool has_scope,
         typename O, bool o_is_arithmetic, bool o_is_enum, bool o_is_jni_type,
         typename T, bool t_is_arithmetic, bool t_is_enum, bool t_is_jni_type>
struct qtjambi_cast_decider<has_scope, O, o_is_arithmetic, o_is_enum, o_is_jni_type, T, t_is_arithmetic, t_is_enum, t_is_jni_type, true>{
    typedef typename std::remove_reference<T>::type T_noref;
    typedef typename std::remove_cv<T_noref>::type T_noconst;
    typedef typename std::conditional<std::is_pointer<T_noref>::value,
                typename std::conditional<std::is_arithmetic<T_noref>::value || std::is_enum<T_noref>::value, T_noconst, T_noref>::type,
                typename std::add_lvalue_reference<T_noref>::type>::type T_in;
    constexpr static O cast(JNIEnv *, T_in in, const char*, QtJambiScope*){
        return in;
    }
};

// O and T are arithmetic
template<bool has_scope,
         typename O,
         typename T>
struct qtjambi_cast_decider<has_scope, O, true, false, false, T, true, false, false, false>{
    typedef typename std::remove_reference<T>::type T_noref;
    typedef typename std::remove_cv<T_noref>::type T_noconst;
    typedef typename std::conditional<std::is_pointer<T_noref>::value,
                typename std::conditional<std::is_arithmetic<T_noref>::value || std::is_enum<T_noref>::value, T_noconst, T_noref>::type,
                typename std::add_lvalue_reference<T_noref>::type>::type T_in;
    constexpr static O cast(JNIEnv *, T_in in, const char*, QtJambiScope*){
        return static_cast<O>(in);
    }
};

// O and T are enums
template<bool has_scope,
         typename O,
         typename T>
struct qtjambi_cast_decider<has_scope, O, false, true, false, T, false, true, false, false>{
    typedef typename std::remove_reference<T>::type T_noref;
    typedef typename std::remove_cv<T_noref>::type T_noconst;
    typedef typename std::conditional<std::is_pointer<T_noref>::value,
                typename std::conditional<std::is_arithmetic<T_noref>::value || std::is_enum<T_noref>::value, T_noconst, T_noref>::type,
                typename std::add_lvalue_reference<T_noref>::type>::type T_in;
    constexpr static O cast(JNIEnv *, T_in in, const char*, QtJambiScope*){
        return static_cast<O>(in);
    }
};

// O is arithmetic, T is non-jni
template<bool has_scope,
         typename O_arith,
         typename T, bool t_is_enum>
struct qtjambi_cast_decider<has_scope, O_arith, true, false, false, T, false, t_is_enum, false, false>
        :   qtjambi_arithmetic_caster<true, has_scope,
                O_arith,
                typename std::remove_cv<typename std::remove_pointer<typename qtjambi_cast_types<T>::T_noconst>::type>::type,
                t_is_enum,
                std::is_pointer<typename qtjambi_cast_types<T>::T_noref>::value,
                std::is_const<typename std::conditional<std::is_pointer<typename qtjambi_cast_types<T>::T_noref>::value, typename std::remove_pointer<typename qtjambi_cast_types<T>::T_noref>::type, typename qtjambi_cast_types<T>::T_noref>::type>::value,
                std::is_reference<T>::value && !std::is_pointer<typename qtjambi_cast_types<T>::T_noref>::value,
                is_template<typename std::remove_cv<typename std::remove_pointer<typename qtjambi_cast_types<T>::T_noconst>::type>::type>::value
              >
{};

// T is arithmetic, O is non-jni
template<bool has_scope,
         typename O, bool o_is_enum,
         typename T_arith>
struct qtjambi_cast_decider<has_scope, O, false, o_is_enum, false, T_arith, true, false, false, false>
        :   qtjambi_arithmetic_caster<false, has_scope,
                typename qtjambi_cast_types<T_arith>::T_noconst,
                typename std::remove_cv<typename std::remove_pointer<typename qtjambi_cast_types<O>::T_noconst>::type>::type,
                o_is_enum,
                std::is_pointer<typename qtjambi_cast_types<O>::T_noref>::value,
                std::is_const<typename std::conditional<std::is_pointer<typename qtjambi_cast_types<O>::T_noref>::value, typename std::remove_pointer<typename qtjambi_cast_types<O>::T_noref>::type, typename qtjambi_cast_types<O>::T_noref>::type>::value,
                std::is_reference<O>::value && !std::is_pointer<typename qtjambi_cast_types<O>::T_noref>::value,
                is_template<typename std::remove_cv<typename std::remove_pointer<typename qtjambi_cast_types<O>::T_noconst>::type>::type>::value
            >
{};

// O is enum, T is non-jni
template<bool has_scope,
         typename O_enum,
         typename T>
struct qtjambi_cast_decider<has_scope, O_enum, false, true, false, T, false, false, false, false>
        : qtjambi_enum_caster<true, has_scope,
            O_enum,
            typename std::remove_cv<typename std::remove_pointer<typename qtjambi_cast_types<T>::T_noconst>::type>::type,
            std::is_pointer<typename qtjambi_cast_types<T>::T_noref>::value,
            std::is_const<typename std::conditional<std::is_pointer<typename qtjambi_cast_types<T>::T_noref>::value, typename std::remove_pointer<typename qtjambi_cast_types<T>::T_noref>::type, typename qtjambi_cast_types<T>::T_noref>::type>::value,
            std::is_reference<T>::value && !std::is_pointer<typename qtjambi_cast_types<T>::T_noref>::value,
            is_template<typename std::remove_cv<typename std::remove_pointer<typename qtjambi_cast_types<T>::T_noconst>::type>::type>::value
          >
{};

// T is enum, O is non-jni
template<bool has_scope,
         typename O,
         typename T_enum>
struct qtjambi_cast_decider<has_scope, O, false, false, false, T_enum, false, true, false, false>
        : qtjambi_enum_caster<false, has_scope,
                typename qtjambi_cast_types<T_enum>::T_noconst,
                typename std::remove_cv<typename std::remove_pointer<typename qtjambi_cast_types<O>::T_noconst>::type>::type,
                std::is_pointer<typename qtjambi_cast_types<O>::T_noref>::value,
                std::is_const<typename std::conditional<std::is_pointer<typename qtjambi_cast_types<O>::T_noref>::value, typename std::remove_pointer<typename qtjambi_cast_types<O>::T_noref>::type, typename qtjambi_cast_types<O>::T_noref>::type>::value,
                std::is_reference<O>::value && !std::is_pointer<typename qtjambi_cast_types<O>::T_noref>::value,
                is_template<typename std::remove_cv<typename std::remove_pointer<typename qtjambi_cast_types<O>::T_noconst>::type>::type>::value
            > {};

// O is jni type
template<bool has_scope,
         typename O_jni,
         typename T, bool t_is_arithmetic, bool t_is_enum>
struct qtjambi_cast_decider<has_scope, O_jni, false, false, true, T, t_is_arithmetic, t_is_enum, false, false>
        :   qtjambi_jnitype_caster< true, has_scope,
                                    typename qtjambi_cast_types<O_jni>::T_noconst,
                                    typename std::remove_cv<typename std::remove_pointer<typename qtjambi_cast_types<T>::T_noconst>::type>::type,
                                    t_is_arithmetic,
                                    t_is_enum,
                                    std::is_pointer<typename qtjambi_cast_types<T>::T_noref>::value,
                                    std::is_const<typename std::conditional<std::is_pointer<typename qtjambi_cast_types<T>::T_noref>::value, typename std::remove_pointer<typename qtjambi_cast_types<T>::T_noref>::type, typename qtjambi_cast_types<T>::T_noref>::type>::value,
                                    std::is_reference<T>::value && !std::is_pointer<typename qtjambi_cast_types<T>::T_noref>::value,
                                    std::is_function<typename std::remove_pointer<typename qtjambi_cast_types<T>::T_noconst>::type>::value,
                                    is_template<typename std::remove_cv<typename std::remove_pointer<typename qtjambi_cast_types<T>::T_noconst>::type>::type>::value
                                  >
{
};

// T is jni type
template<bool has_scope,
         typename O, bool o_is_arithmetic, bool o_is_enum,
         typename T_jni>
struct qtjambi_cast_decider<has_scope, O, o_is_arithmetic, o_is_enum, false, T_jni, false, false, true, false>
        :   qtjambi_jnitype_caster< false, has_scope,
                                        typename qtjambi_cast_types<T_jni>::T_noconst,
                                        typename std::remove_cv<typename std::remove_pointer<typename qtjambi_cast_types<O>::T_noconst>::type>::type,
                                        o_is_arithmetic,
                                        o_is_enum,
                                        std::is_pointer<typename qtjambi_cast_types<O>::T_noref>::value,
                                        std::is_const<typename std::conditional<std::is_pointer<typename qtjambi_cast_types<O>::T_noref>::value, typename std::remove_pointer<typename qtjambi_cast_types<O>::T_noref>::type, typename qtjambi_cast_types<O>::T_noref>::type>::value,
                                        std::is_reference<O>::value && !std::is_pointer<typename qtjambi_cast_types<O>::T_noref>::value,
                                        std::is_function<typename std::remove_pointer<typename qtjambi_cast_types<O>::T_noconst>::type>::value,
                                        is_template<typename std::remove_cv<typename std::remove_pointer<typename qtjambi_cast_types<O>::T_noconst>::type>::type>::value
                                    >
{
};

// T and O are non-jni non-arithmetic type
template<bool has_scope,
         typename O, bool o_is_pointer, bool o_is_const, bool o_is_reference, bool o_is_function, bool o_is_q_object, bool o_is_copy_assignable, bool o_is_template,
         typename T, bool t_is_pointer, bool t_is_const, bool t_is_reference, bool t_is_function, bool t_is_q_object, bool t_is_copy_assignable, bool t_is_template>
struct qtjambi_misc_caster{
    typedef typename std::conditional<o_is_const, typename std::add_const<O>::type, O>::type O_const;
    typedef typename std::conditional<o_is_reference, typename std::add_lvalue_reference<O_const>::type, O_const>::type O_constref;
    typedef typename std::conditional<o_is_pointer, typename std::add_pointer<O_const>::type, typename std::add_lvalue_reference<O_const>::type>::type O_in;
    typedef typename std::conditional<o_is_pointer, typename std::add_pointer<O_const>::type, O_constref>::type O_out;
    typedef typename std::conditional<t_is_const, typename std::add_const<T>::type, T>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_constref;
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T_const>::type, typename std::add_lvalue_reference<T_const>::type>::type T_in;
    typedef typename std::conditional<t_is_pointer, typename std::add_pointer<T_const>::type, T_constref>::type T_out;
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
};

#ifdef QSTRING_H
// string to enum

template<bool has_scope,
         bool o_is_const, typename T, bool t_is_const, bool t_is_reference>
struct qtjambi_misc_caster<has_scope,
        QString, false, o_is_const, false, false, false, true, false,
        T, true, t_is_const, t_is_reference, false, false, true, false>{
    typedef typename std::conditional<t_is_const, typename std::add_const<T>::type, T>::type T_const;
    typedef typename std::conditional<t_is_reference, typename std::add_lvalue_reference<T_const>::type, T_const>::type T_constref;
    typedef typename std::add_lvalue_reference<T_const>::type T_in;
    typedef T_constref T_out;
    static QString cast(JNIEnv *, T_in in, const char*, QtJambiScope*){
        return QString::number(qint64(in));
    }
};
#endif

template<bool has_scope,
         typename O,
         typename T>
struct qtjambi_cast_decider<has_scope, O, false, false, false, T, false, false, false, false>
        :   qtjambi_misc_caster< has_scope,
                    typename std::remove_cv<typename std::remove_pointer<typename qtjambi_cast_types<O>::T_noconst>::type>::type,
                    std::is_pointer<typename qtjambi_cast_types<O>::T_noref>::value,
                    std::is_const<typename std::conditional<std::is_pointer<typename qtjambi_cast_types<O>::T_noref>::value, typename std::remove_pointer<typename qtjambi_cast_types<O>::T_noref>::type, typename qtjambi_cast_types<O>::T_noref>::type>::value,
                    std::is_reference<O>::value,
                    std::is_function<typename std::remove_pointer<typename qtjambi_cast_types<O>::T_noconst>::type>::value,
                    std::is_base_of<QObject, typename std::remove_pointer<typename qtjambi_cast_types<O>::T_noconst>::type>::value,
                    std::is_copy_assignable<typename qtjambi_cast_types<O>::T_noconst>::value,
                    is_template<typename std::remove_cv<typename std::remove_pointer<typename qtjambi_cast_types<O>::T_noconst>::type>::type>::value,
                    typename std::remove_cv<typename std::remove_pointer<typename qtjambi_cast_types<T>::T_noconst>::type>::type,
                    std::is_pointer<typename qtjambi_cast_types<T>::T_noref>::value,
                    std::is_const<typename std::conditional<std::is_pointer<typename qtjambi_cast_types<T>::T_noref>::value, typename std::remove_pointer<typename qtjambi_cast_types<T>::T_noref>::type, typename qtjambi_cast_types<T>::T_noref>::type>::value,
                    std::is_reference<T>::value && !std::is_pointer<typename qtjambi_cast_types<T>::T_noref>::value,
                    std::is_function<typename std::remove_pointer<typename qtjambi_cast_types<T>::T_noconst>::type>::value,
                    std::is_base_of<QObject, typename std::remove_pointer<typename qtjambi_cast_types<T>::T_noconst>::type>::value,
                    std::is_copy_assignable<typename qtjambi_cast_types<T>::T_noconst>::value,
                    is_template<typename std::remove_cv<typename std::remove_pointer<typename qtjambi_cast_types<T>::T_noconst>::type>::type>::value > {};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_IMPL_H
