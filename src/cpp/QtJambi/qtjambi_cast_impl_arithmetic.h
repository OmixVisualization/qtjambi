/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
         bool has_scope,
         typename ArithmeticType,
         typename NativeType, bool is_enum, bool is_pointer, bool is_const, bool is_reference, bool is_template>
struct qtjambi_arithmetic_caster{
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
};

template<bool has_scope,
         typename ArithmeticType,
         typename NativeType, bool is_const, bool is_reference>
struct qtjambi_arithmetic_caster<true,
                                 has_scope,
                                 ArithmeticType,
                                 NativeType, true, false, is_const, is_reference, false>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    constexpr static ArithmeticType cast(JNIEnv *, NativeType_in in, const char*, QtJambiScope*){
        return ArithmeticType(in);
    }
};

template<bool has_scope,
         typename ArithmeticType,
         typename NativeType, bool is_const, bool is_reference>
struct qtjambi_arithmetic_caster<false,
                                 has_scope,
                                 ArithmeticType,
                                 NativeType, true, false, is_const, is_reference, false>{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType>::type, NativeType>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to enumerator reference");

    constexpr static NativeType_out cast(JNIEnv *, ArithmeticType in, const char*, QtJambiScope*){
        return NativeType(in);
    }
};

template<bool has_scope,
         typename ArithmeticType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_arithmetic_caster<true,
                                 has_scope,
                                 ArithmeticType,
                                 QLatin1Char, false, is_pointer, is_const, is_reference, false>{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1Char>::type, QLatin1Char>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static ArithmeticType cast(JNIEnv *, NativeType_in in, const char*, QtJambiScope*){
        return ArithmeticType(deref_ptr<is_pointer,QLatin1Char>::deref(in).unicode());
    }
};

template<bool has_scope,
         typename ArithmeticType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_arithmetic_caster<false,
                                 has_scope,
                                 ArithmeticType,
                                 QLatin1Char, false, is_pointer, is_const, is_reference, false>{
    typedef typename std::conditional<is_const, typename std::add_const<QLatin1Char>::type, QLatin1Char>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QLatin1Char reference");
    Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to QLatin1Char pointer");

    constexpr static NativeType_out cast(JNIEnv *, ArithmeticType in, const char*, QtJambiScope*){
        return QLatin1Char(char(in));
    }
};

template<bool has_scope,
         typename ArithmeticType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_arithmetic_caster<true,
                                 has_scope,
                                 ArithmeticType,
                                 QChar, false, is_pointer, is_const, is_reference, false>{
    typedef typename std::conditional<is_const, typename std::add_const<QChar>::type, QChar>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    constexpr static ArithmeticType cast(JNIEnv *, NativeType_in in, const char*, QtJambiScope*){
        return ArithmeticType(deref_ptr<is_pointer,QChar>::deref(in).unicode());
    }
};

template<bool has_scope,
         typename ArithmeticType,
         bool is_pointer, bool is_const, bool is_reference>
struct qtjambi_arithmetic_caster<false,
                                 has_scope,
                                 ArithmeticType,
                                 QChar, false, is_pointer, is_const, is_reference, false>{
    typedef typename std::conditional<is_const, typename std::add_const<QChar>::type, QChar>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QChar reference");
    Q_STATIC_ASSERT_X(!is_pointer, "Cannot cast to QChar pointer");

    constexpr static NativeType_out cast(JNIEnv *, ArithmeticType in, const char*, QtJambiScope*){
        return QChar(ushort(in));
    }
};

template<bool forward,
         bool has_scope,
         typename ArithmeticType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         int parameterCount, typename... Ts>
struct qtjambi_arithmetic_container_cast_decider{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType<Ts...>>::type, NativeType<Ts...>>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
};

template<bool forward,
         bool has_scope,
         typename ArithmeticType,
         template<typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename T>
struct qtjambi_arithmetic_container1_cast{
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
};

template<bool forward,
         bool has_scope,
         typename ArithmeticType,
         template<typename K, typename T> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename K, typename T>
struct qtjambi_arithmetic_container2_cast{
    typedef typename std::conditional<is_const, typename std::add_const<NativeType<K,T>>::type, NativeType<K,T>>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, typename std::add_lvalue_reference<NativeType_c>::type>::type NativeType_in;
    typedef typename std::conditional<is_pointer, typename std::add_pointer<NativeType_c>::type, NativeType_cr>::type NativeType_out;
    Q_STATIC_ASSERT_X(false && !has_scope, "Cannot cast types");
};

template<bool forward, bool has_scope, typename ArithmeticType, template<typename T> class Container, bool is_pointer, bool is_const, bool is_reference, typename T>
static constexpr qtjambi_arithmetic_container1_cast<forward, has_scope, ArithmeticType, Container, is_pointer, is_const, is_reference, T> qtjambi_arithmetic_container1_supertype(){
    return {};
}

template<bool forward, bool has_scope,
         typename ArithmeticType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename... Ts>
struct qtjambi_arithmetic_container_cast_decider<forward, has_scope, ArithmeticType, NativeType, is_pointer, is_const, is_reference, 1, Ts...>
        : decltype(qtjambi_arithmetic_container1_supertype<forward, has_scope, ArithmeticType, NativeType, is_pointer, is_const, is_reference, Ts...>()){};

template<bool forward, bool has_scope, typename ArithmeticType, template<typename K, typename T> class Container, bool is_pointer, bool is_const, bool is_reference, typename K, typename T>
static constexpr qtjambi_arithmetic_container2_cast<forward, has_scope, ArithmeticType, Container, is_pointer, is_const, is_reference, K, T> qtjambi_arithmetic_container2_supertype(){
    return {};
}

template<bool forward, bool has_scope,
         typename ArithmeticType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename... Ts>
struct qtjambi_arithmetic_container_cast_decider<forward, has_scope, ArithmeticType, NativeType, is_pointer, is_const, is_reference, 2, Ts...>
        : decltype(qtjambi_arithmetic_container2_supertype<forward, has_scope, ArithmeticType, NativeType, is_pointer, is_const, is_reference, Ts...>()){};

template<bool forward,
         bool has_scope,
         typename ArithmeticType,
         template<typename... Ts> class NativeType, bool is_pointer, bool is_const, bool is_reference,
         typename... Ts>
struct qtjambi_arithmetic_caster<forward,
                                 has_scope,
                                 ArithmeticType,
                                 NativeType<Ts...>, false, is_pointer, is_const, is_reference, true>
        : qtjambi_arithmetic_container_cast_decider<forward, has_scope, ArithmeticType, NativeType, is_pointer, is_const, is_reference, sizeof...(Ts), Ts...>{
};

template<typename ArithmeticType>
struct arithmetic_from_string_decider{
    static ArithmeticType cast(JNIEnv *, const QString& in, const char*, QtJambiScope*){
        return ArithmeticType(in.toLong());
    }
};

template<>
struct arithmetic_from_string_decider<double>{
    static double cast(JNIEnv *, const QString& in, const char*, QtJambiScope*){
        return in.toDouble();
    }
};

template<>
struct arithmetic_from_string_decider<float>{
    static float cast(JNIEnv *, const QString& in, const char*, QtJambiScope*){
        return in.toFloat();
    }
};

template<>
struct arithmetic_from_string_decider<bool>{
    static bool cast(JNIEnv *, const QString& in, const char*, QtJambiScope*){
        return !in.isEmpty();
    }
};

template<>
struct arithmetic_from_string_decider<qlonglong>{
    static qlonglong cast(JNIEnv *, const QString& in, const char*, QtJambiScope*){
        return in.toLongLong();
    }
};

template<>
struct arithmetic_from_string_decider<qulonglong>{
    static qulonglong cast(JNIEnv *, const QString& in, const char*, QtJambiScope*){
        return in.toULongLong();
    }
};

template<>
struct arithmetic_from_string_decider<long>{
    static long cast(JNIEnv *, const QString& in, const char*, QtJambiScope*){
        return in.toLong();
    }
};

template<>
struct arithmetic_from_string_decider<ulong>{
    static ulong cast(JNIEnv *, const QString& in, const char*, QtJambiScope*){
        return in.toULong();
    }
};

template<>
struct arithmetic_from_string_decider<int>{
    static int cast(JNIEnv *, const QString& in, const char*, QtJambiScope*){
        return in.toInt();
    }
};

template<>
struct arithmetic_from_string_decider<uint>{
    static uint cast(JNIEnv *, const QString& in, const char*, QtJambiScope*){
        return in.toUInt();
    }
};

template<>
struct arithmetic_from_string_decider<short>{
    static short cast(JNIEnv *, const QString& in, const char*, QtJambiScope*){
        return in.toShort();
    }
};

template<>
struct arithmetic_from_string_decider<ushort>{
    static ushort cast(JNIEnv *, const QString& in, const char*, QtJambiScope*){
        return in.toUShort();
    }
};

template<>
struct arithmetic_from_string_decider<char>{
    static char cast(JNIEnv *, const QString& in, const char*, QtJambiScope*){
        return char(in.toShort());
    }
};

template<>
struct arithmetic_from_string_decider<uchar>{
    static uchar cast(JNIEnv *, const QString& in, const char*, QtJambiScope*){
        return uchar(in.toUShort());
    }
};

template<bool has_scope,
         typename ArithmeticType,
         bool is_const, bool is_reference>
struct qtjambi_arithmetic_caster<true,
                                 has_scope,
                                 ArithmeticType,
                                 QString, false, false, is_const, is_reference, false>
        : arithmetic_from_string_decider<ArithmeticType>{};

template<bool has_scope,
         typename ArithmeticType,
         bool is_const, bool is_reference>
struct qtjambi_arithmetic_caster<false,
                                 has_scope,
                                 ArithmeticType,
                                 QString, false, false, is_const, is_reference, false>{
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QString reference");

    static QString cast(JNIEnv *, ArithmeticType in, const char*, QtJambiScope*){
        return QString::number(in);
    }
};

template<bool has_scope,
         typename ArithmeticType,
         bool is_const, bool is_reference,
         typename T>
struct qtjambi_arithmetic_container1_cast<true, has_scope,
                                          ArithmeticType,
                                          QFlags, false, is_const, is_reference, T>{
    typedef typename std::conditional<is_const, typename std::add_const<QFlags<T>>::type, QFlags<T>>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    constexpr static ArithmeticType cast(JNIEnv *, NativeType_in in, const char*, QtJambiScope*){
        return ArithmeticType(in);
    }
};

template<bool has_scope,
         typename ArithmeticType,
         bool is_const, bool is_reference,
         typename T>
struct qtjambi_arithmetic_container1_cast<false, has_scope,
                                          ArithmeticType,
                                          QFlags, false, is_const, is_reference, T>{
    typedef typename std::conditional<is_const, typename std::add_const<QFlags<T>>::type, QFlags<T>>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QFlags<T> reference");

    constexpr static NativeType_out cast(JNIEnv *, ArithmeticType in, const char*, QtJambiScope*){
        return QFlags<T>(int(in));
    }
};

template<bool has_scope,
         typename ArithmeticType,
         bool is_const,
         typename T>
struct qtjambi_arithmetic_container1_cast<true, has_scope,
                                          ArithmeticType,
                                          QFlags, true, is_const, false, T>{
    typedef typename std::conditional<is_const, typename std::add_const<QFlags<T>>::type, QFlags<T>>::type NativeType_c;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_in;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_out;
    constexpr static ArithmeticType cast(JNIEnv *, NativeType_in in, const char*, QtJambiScope*){
        return ArithmeticType(*in);
    }
};

template<bool has_scope,
         typename ArithmeticType,
         bool is_const,
         typename T>
struct qtjambi_arithmetic_container1_cast<false, has_scope,
                                          ArithmeticType,
                                          QFlags, true, is_const, false, T>{
    typedef typename std::conditional<is_const, typename std::add_const<QFlags<T>>::type, QFlags<T>>::type NativeType_c;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_in;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_out;
    Q_STATIC_ASSERT_X(!has_scope, "Cannot cast to QFlags<T>*");

    static NativeType_out cast(JNIEnv * env, ArithmeticType in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QFlags<T>*" QTJAMBI_STACKTRACEINFO );
        QFlags<T>* result = new QFlags<T>(int(in));
        scope->addDeletion(result);
        return result;
    }
};

#ifdef QURL_H
template<bool has_scope,
         typename ArithmeticType,
         bool is_const, bool is_reference>
struct qtjambi_arithmetic_container2_cast<true, has_scope,
                                          ArithmeticType,
                                          QUrlTwoFlags, false, is_const, is_reference, QUrl::UrlFormattingOption, QUrl::ComponentFormattingOption>{
    typedef typename std::conditional<is_const, typename std::add_const<QUrl::FormattingOptions>::type, QUrl::FormattingOptions>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    constexpr static ArithmeticType cast(JNIEnv *, NativeType_in in, const char*, QtJambiScope*){
        return ArithmeticType(in);
    }
};

template<bool has_scope,
         typename ArithmeticType,
         bool is_const, bool is_reference>
struct qtjambi_arithmetic_container2_cast<false, has_scope,
                                          ArithmeticType,
                                          QUrlTwoFlags, false, is_const, is_reference, QUrl::UrlFormattingOption, QUrl::ComponentFormattingOption>{
    typedef typename std::conditional<is_const, typename std::add_const<QUrl::FormattingOptions>::type, QUrl::FormattingOptions>::type NativeType_c;
    typedef typename std::conditional<is_reference, typename std::add_lvalue_reference<NativeType_c>::type, NativeType_c>::type NativeType_cr;
    typedef typename std::add_lvalue_reference<NativeType_c>::type NativeType_in;
    typedef NativeType_cr NativeType_out;
    Q_STATIC_ASSERT_X(!is_reference, "Cannot cast to QUrl::FormattingOptions reference");

    constexpr static NativeType_out cast(JNIEnv *, ArithmeticType in, const char*, QtJambiScope*){
        return QUrl::FormattingOptions(int(in));
    }
};

template<bool has_scope,
         typename ArithmeticType,
         bool is_const>
struct qtjambi_arithmetic_container2_cast<true, has_scope,
                                          ArithmeticType,
                                          QUrlTwoFlags, true, is_const, false, QUrl::UrlFormattingOption, QUrl::ComponentFormattingOption>{
    typedef typename std::conditional<is_const, typename std::add_const<QUrl::FormattingOptions>::type, QUrl::FormattingOptions>::type NativeType_c;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_in;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_out;
    constexpr static ArithmeticType cast(JNIEnv *, NativeType_in in, const char*, QtJambiScope*){
        return ArithmeticType(*in);
    }
};

template<bool has_scope,
         typename ArithmeticType,
         bool is_const>
struct qtjambi_arithmetic_container2_cast<false, has_scope,
                                          ArithmeticType,
                                          QUrlTwoFlags, true, is_const, false, QUrl::UrlFormattingOption, QUrl::ComponentFormattingOption>{
    typedef typename std::conditional<is_const, typename std::add_const<QUrl::FormattingOptions>::type, QUrl::FormattingOptions>::type NativeType_c;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_in;
    typedef typename std::add_pointer<NativeType_c>::type NativeType_out;
    Q_STATIC_ASSERT_X(!has_scope, "Cannot cast to QUrl::FormattingOptions*");

    static NativeType_out cast(JNIEnv * env, ArithmeticType in, const char*, QtJambiScope* scope){
        if(!scope)
            JavaException::raiseError(env, "Cannot cast to QUrl::FormattingOptions*" QTJAMBI_STACKTRACEINFO );
        QUrl::FormattingOptions* result = new QUrl::FormattingOptions(int(in));
        scope->addDeletion(result);
        return result;
    }
};
#endif //QURL_H

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_IMPL_ARITHMETIC_H
