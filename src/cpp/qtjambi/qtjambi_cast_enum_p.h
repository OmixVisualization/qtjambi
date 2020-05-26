/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBI_CAST_ENUM_P_H
#define QTJAMBI_CAST_ENUM_P_H

#include "qtjambi_core.h"
#include "qtjambi_cast_util_p.h"

namespace QtJambiPrivate {

template<bool forward, bool has_scope,
         typename EnumType,
         typename NativeType, bool is_pointer, bool is_const, bool is_reference, bool is_template>
struct qtjambi_enum_caster{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static EnumType castfwd(JNIEnv *, NativeType_in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
    }
    static NativeType_out castbck(JNIEnv *, EnumType, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
    }
};

template<bool forward, bool has_scope,
         typename EnumType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         int parameterCount, typename... Ts>
struct qtjambi_enum_container_cast_decider{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType<Ts...>>::type, NativeType<Ts...>>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static EnumType castfwd(JNIEnv *, NativeType_in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
    }
    static NativeType_out castbck(JNIEnv *, EnumType, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
    }
};

template<bool has_scope,
         typename EnumType,
         template<typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_enum_container1_cast{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType<T>>::type, NativeType<T>>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static EnumType castfwd(JNIEnv *, NativeType_in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
    }
    static NativeType_out castbck(JNIEnv *, EnumType, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
    }
};

template<bool has_scope,
         typename EnumType,
         template<typename K, typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_enum_container2_cast{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType<K,T>>::type, NativeType<K,T>>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    static EnumType castfwd(JNIEnv *, NativeType_in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
    }
    static NativeType_out castbck(JNIEnv *, EnumType, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
    }
};

template<bool has_scope,
         typename EnumType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename... Ts>
struct qtjambi_enum_container_cast_decider<true, has_scope, EnumType, NativeType, is_pointer, is_const, is_reference, 1, Ts...>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType<Ts...>>::type, NativeType<Ts...>>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    typedef EnumType (*CastFwdReturn)(JNIEnv *, NativeType_in, const char*, QtJambiScope*);
    template<template<typename T> class Container, typename T>
    static constexpr CastFwdReturn _castfwd(){
        return qtjambi_enum_container1_cast<has_scope, EnumType, Container, is_pointer, is_const, is_reference, T>::castfwd;
    }

    static constexpr EnumType (*castfwd)(JNIEnv *, NativeType_in, const char*, QtJambiScope*) = _castfwd<NativeType, Ts...>();
};

template<bool has_scope,
         typename EnumType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename... Ts>
struct qtjambi_enum_container_cast_decider<false, has_scope, EnumType, NativeType, is_pointer, is_const, is_reference, 1, Ts...>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType<Ts...>>::type, NativeType<Ts...>>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    typedef NativeType_out (*CastBckReturn)(JNIEnv *, EnumType, const char*, QtJambiScope*);
    template<template<typename T> class Container, typename T>
    static constexpr CastBckReturn _castbck(){
        return qtjambi_enum_container1_cast<has_scope, EnumType, Container, is_pointer, is_const, is_reference, T>::castbck;
    }

    static constexpr NativeType_out (*castbck)(JNIEnv *, EnumType, const char*, QtJambiScope*) = _castbck<NativeType, Ts...>();
};

template<bool has_scope,
         typename EnumType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename... Ts>
struct qtjambi_enum_container_cast_decider<true, has_scope, EnumType, NativeType, is_pointer, is_const, is_reference, 2, Ts...>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType<Ts...>>::type, NativeType<Ts...>>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    typedef EnumType (*CastFwdReturn)(JNIEnv *, NativeType_in, const char*, QtJambiScope*);
    template<template<typename K, typename T> class Container, typename K, typename T>
    static constexpr CastFwdReturn _castfwd(){
        return qtjambi_enum_container2_cast<has_scope, EnumType, Container, is_pointer, is_const, is_reference, K, T>::castfwd;
    }

    static constexpr EnumType (*castfwd)(JNIEnv *, NativeType_in, const char*, QtJambiScope*) = _castfwd<NativeType, Ts...>();
};

template<bool has_scope,
         typename EnumType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename... Ts>
struct qtjambi_enum_container_cast_decider<false, has_scope, EnumType, NativeType, is_pointer, is_const, is_reference, 2, Ts...>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType<Ts...>>::type, NativeType<Ts...>>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;

    typedef NativeType_out (*CastBckReturn)(JNIEnv *, EnumType, const char*, QtJambiScope*);
    template<template<typename K, typename T> class Container, typename K, typename T>
    static constexpr CastBckReturn _castbck(){
        return qtjambi_enum_container2_cast<has_scope, EnumType, Container, is_pointer, is_const, is_reference, K, T>::castbck;
    }

    static constexpr NativeType_out (*castbck)(JNIEnv *, EnumType, const char*, QtJambiScope*) = _castbck<NativeType, Ts...>();
};

template<bool forward, bool has_scope,
         typename EnumType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename... Ts>
struct qtjambi_enum_caster<forward, has_scope,
                                 EnumType,
                                 NativeType<Ts...>, is_pointer, is_const, is_reference, true>
        : qtjambi_enum_container_cast_decider<forward, has_scope, EnumType, NativeType, is_pointer, is_const, is_reference, sizeof...(Ts), Ts...>{
};

template<typename EnumType, size_t size>
struct enum_from_string_decider{
    static EnumType castfwd(JNIEnv *, typename std::add_lvalue_reference<QString>::type in, const char*, QtJambiScope*){
        return EnumType(in.toLong());
    }
};

template<typename EnumType>
struct enum_from_string_decider<EnumType,sizeof(qlonglong)>{
    static EnumType castfwd(JNIEnv *, typename std::add_lvalue_reference<QString>::type in, const char*, QtJambiScope*){
        return EnumType(in.toLong());
    }
};

template<typename EnumType>
struct enum_from_string_decider<EnumType,sizeof(qint32)>{
    static EnumType castfwd(JNIEnv *, typename std::add_lvalue_reference<QString>::type in, const char*, QtJambiScope*){
        return EnumType(in.toInt());
    }
};

template<typename EnumType>
struct enum_from_string_decider<EnumType,sizeof(qint16)>{
    static EnumType castfwd(JNIEnv *, typename std::add_lvalue_reference<QString>::type in, const char*, QtJambiScope*){
        return EnumType(in.toShort());
    }
};

template<typename EnumType>
struct enum_from_string_decider<EnumType,sizeof(qint8)>{
    static EnumType castfwd(JNIEnv *, typename std::add_lvalue_reference<QString>::type in, const char*, QtJambiScope*){
        return EnumType(qint8(in.toInt()));
    }
};

template<bool has_scope, bool forward,
         typename EnumType,
         bool is_const, bool is_reference>
struct qtjambi_enum_caster<forward, has_scope,
                                 EnumType,
                                 QString, false, is_const, is_reference, false>
        : enum_from_string_decider<EnumType, sizeof(EnumType)>{

    static QString castbck(JNIEnv *, EnumType in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QString reference");
        return QString::number(qint64(in));
    }
};

template<bool has_scope,
         typename EnumType,
         bool is_const, bool is_reference,
         typename T>
struct qtjambi_enum_container1_cast<has_scope,
                                          EnumType,
                                          QFlags, false, is_const, is_reference, T>{
    typedef typename std::conditional<is_const, typename std::add_const<QFlags<T>>::type, QFlags<T>>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    static EnumType castfwd(JNIEnv *, NativeType_in in, const char*, QtJambiScope*){
        return EnumType(int(in));
    }

    static NativeType_out castbck(JNIEnv *, EnumType in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QFlags<T> reference");
        return QFlags<T>(in);
    }
};

template<bool has_scope,
         typename EnumType,
         bool is_const,
         typename T>
struct qtjambi_enum_container1_cast<has_scope,
                                          EnumType,
                                          QFlags, true, is_const, false, T>{
    typedef typename std::conditional<is_const, typename std::add_const<QFlags<T>>::type, QFlags<T>>::type NativeType_c;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_in;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_out;
    static EnumType castfwd(JNIEnv *, NativeType_in in, const char*, QtJambiScope*){
        return EnumType(int(*in));
    }

    static NativeType_out castbck(JNIEnv * env, EnumType in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!has_scope, "Cannot cast to QFlags<T>*");
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QFlags<T>*" QTJAMBI_STACKTRACEINFO );
        QFlags<T>* result = new QFlags<T>(in);
        scope->addFinalAction([result](){delete result;});
        return result;
    }
};

template<bool has_scope,
         typename EnumType,
         bool is_const, bool is_reference>
struct qtjambi_enum_container2_cast<has_scope,
                                          EnumType,
                                          QUrlTwoFlags, false, is_const, is_reference, QUrl::UrlFormattingOption, QUrl::ComponentFormattingOption>{
    typedef typename std::conditional<is_const, typename std::add_const<QUrl::FormattingOptions>::type, QUrl::FormattingOptions>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    static EnumType castfwd(JNIEnv *, NativeType_in in, const char*, QtJambiScope*){
        return EnumType(in);
    }

    static NativeType_out castbck(JNIEnv *, EnumType in, const char*, QtJambiScope*){
        Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QUrl::FormattingOptions reference");
        return QUrl::FormattingOptions(int(in));
    }
};

template<bool has_scope,
         typename EnumType,
         bool is_const>
struct qtjambi_enum_container2_cast<has_scope,
                                          EnumType,
                                          QUrlTwoFlags, true, is_const, false, QUrl::UrlFormattingOption, QUrl::ComponentFormattingOption>{
    typedef typename std::conditional<is_const, typename std::add_const<QUrl::FormattingOptions>::type, QUrl::FormattingOptions>::type NativeType_c;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_in;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_out;
    static EnumType castfwd(JNIEnv *, NativeType_in in, const char*, QtJambiScope*){
        return EnumType(*in);
    }

    static NativeType_out castbck(JNIEnv * env, EnumType in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(!has_scope, "Cannot cast to QUrl::FormattingOptions*");
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QUrl::FormattingOptions*" QTJAMBI_STACKTRACEINFO );
        QUrl::FormattingOptions* result = new QUrl::FormattingOptions(int(in));
        scope->addFinalAction([result](){delete result;});
        return result;
    }
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_ENUM_P_H
