/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
**
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
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef QTJAMBI_TYPETESTS_H
#define QTJAMBI_TYPETESTS_H

#include <QtCore/QVariant>
#include <QtCore/QDebug>
#include <QtCore/QDataStream>
#include "global.h"

#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
#define QT610_EXTRA_ARG(ARG) , ARG
#else
#define QT610_EXTRA_ARG(ARG)
#endif

QT_WARNING_DISABLE_DEPRECATED

template<typename T>
class QQmlListProperty;
template <typename T>
class QFutureInterface;
template <typename T>
class QFuture;
template <typename T>
class QFutureWatcher;
template <typename T>
class QPromise;
template <typename PropertyType>
class QPropertyBinding;
template <typename PropertyType>
class QBinding;
template <typename PropertyType>
class QPropertyChangeHandler;
template <class T>
class QQueue;
template <class T>
class QStack;
template <class T>
class QList;
template <class T>
class QSet;

namespace QtJambiPrivate {

template<class T>
struct is_default_constructible : std::is_default_constructible<T>{};
template<typename T>
constexpr bool is_default_constructible_v = is_default_constructible<T>::value;

template<class T>
struct is_copy_constructible : std::is_copy_constructible<T>{};
template<typename T>
constexpr bool is_copy_constructible_v = is_copy_constructible<T>::value;

template<class T>
struct is_move_constructible : std::is_move_constructible<T>{};
template<typename T>
constexpr bool is_move_constructible_v = is_move_constructible<T>::value;

template<class T>
struct is_copy_assignable : std::is_copy_assignable<T>{};
template<typename T>
constexpr bool is_copy_assignable_v = is_copy_assignable<T>::value;

template<class T>
struct is_move_assignable : std::is_move_assignable<T>{};
template<typename T>
constexpr bool is_move_assignable_v = is_move_assignable<T>::value;

template<class T>
struct is_destructible : std::is_destructible<T>{};
template<typename T>
constexpr bool is_destructible_v = is_destructible<T>::value;

template<class T1, class T2>
struct is_default_constructible<std::pair<T1,T2>> : std::bool_constant<is_default_constructible_v<T1> && is_default_constructible_v<T2>>{};
template<class T1, class T2>
struct is_copy_constructible<std::pair<T1,T2>> : std::bool_constant<is_copy_constructible_v<T1> && is_copy_constructible_v<T2>>{};
template<class T1, class T2>
struct is_move_constructible<std::pair<T1,T2>> : std::bool_constant<is_move_constructible_v<T1> && is_move_constructible_v<T2>>{};
template<class T1, class T2>
struct is_copy_assignable<std::pair<T1,T2>> : std::bool_constant<is_copy_assignable_v<T1> && is_copy_assignable_v<T2>>{};
template<class T1, class T2>
struct is_move_assignable<std::pair<T1,T2>> : std::bool_constant<is_move_assignable_v<T1> && is_move_assignable_v<T2>>{};
template<class T1, class T2>
struct is_destructible<std::pair<T1,T2>> : std::bool_constant<is_destructible_v<T1> && is_destructible_v<T2>>{};

#ifdef Q_COMPILER_CONCEPTS
#define BI_OPERATOR_TEST_CONCEPT(operator_name, operator_sign)\
    template<typename, class T1, class T2 = T1> concept supports_##operator_name##_impl = requires(T1 t1, T2 t2){t1 operator_sign t2;};
#define PREFIX_OPERATOR_TEST_CONCEPT(operator_name, operator_sign)\
    template<typename, class T> concept supports_##operator_name##_impl = requires(T t){operator_sign t;};
#define SUFFIX_OPERATOR_TEST_CONCEPT(operator_name, operator_sign)\
template<typename, class T> concept supports_##operator_name##_impl = requires(T t){t operator_sign;};
#define CONSTRUCTOR_TEST_CONCEPT(function_name)\
template<typename, class T, typename...Args> concept supports_##function_name##_impl = requires(Args... args){T(args...);};
#define MEMBER_METHOD_TEST_CONCEPT(function_name)\
    template<typename, class T, typename...Args> concept supports_##function_name##_impl = requires(T t, Args... args){t.function_name(args...);};
#define STATIC_FIELD_TEST_CONCEPT(name)\
template<typename, class T> concept supports_##name##_impl = requires(T t){T::name;};
#define TYPENAME_TEST_CONCEPT(name)\
template<typename, class T> concept supports_##name##_impl = requires(T t){T::name;};
#define GLOBAL_METHOD_TEST_CONCEPT(function_name)\
    template<typename, typename...Args> concept supports_##function_name##_impl = requires(Args... args){function_name(args...);};
#define TEMPLATE_METHOD_TEST_CONCEPT(function_name)\
    template<typename, typename...Args> concept supports_##function_name##_impl = requires(){function_name<Args...>();};
#else
#define BI_OPERATOR_TEST_CONCEPT(operator_name, operator_sign)\
    template<typename, class T1, class T2 = T1> struct supports_##operator_name##_impl : std::false_type {};\
    template<class T1, class T2> struct supports_##operator_name##_impl<std::void_t<decltype(std::declval<T1>() operator_sign std::declval<T2>())>, T1, T2> : std::true_type {};
#define PREFIX_OPERATOR_TEST_CONCEPT(operator_name, operator_sign)\
    template<typename, class T> struct supports_##operator_name##_impl : std::false_type {};\
    template<class T> struct supports_##operator_name##_impl<std::void_t<decltype(operator_sign std::declval<T>())>,T> : std::true_type {};
#define SUFFIX_OPERATOR_TEST_CONCEPT(operator_name, operator_sign)\
template<typename, class T> struct supports_##operator_name##_impl : std::false_type {};\
    template<class T> struct supports_##operator_name##_impl<std::void_t<decltype(std::declval<T>()operator_sign)>,T> : std::true_type {};
#define MEMBER_METHOD_TEST_CONCEPT(function_name)\
    template<typename, class T, typename...Args> struct supports_##function_name##_impl : std::false_type {};\
    template<class T, typename...Args> struct supports_##function_name##_impl<std::void_t<decltype(std::declval<T&>().function_name(std::declval<Args>()...))>, T, Args...> : std::true_type {};
#define CONSTRUCTOR_TEST_CONCEPT(function_name)\
template<typename, class T, typename...Args> struct supports_##function_name##_impl : std::false_type {};\
    template<class T, typename...Args> struct supports_##function_name##_impl<std::void_t<decltype(T(std::declval<Args>()...))>, T, Args...> : std::true_type {};
#define STATIC_FIELD_TEST_CONCEPT(name)\
template<typename, class T> struct supports_##name##_impl : std::false_type {};\
    template<class T> struct supports_##name##_impl<std::void_t<decltype(T::name)>, T> : std::true_type {};
#define TYPENAME_TEST_CONCEPT(name)\
template<typename, class T> struct supports_##name##_impl : std::false_type {};\
    template<class T> struct supports_##name##_impl<std::void_t<typename T::name>, T> : std::true_type {};
#define GLOBAL_METHOD_TEST_CONCEPT(function_name)\
    template<typename, typename...Args> struct supports_##function_name##_impl : std::false_type {};\
    template<typename...Args> struct supports_##function_name##_impl<std::void_t<decltype(function_name(std::declval<Args>()...))>, Args...> : std::true_type {};
#define TEMPLATE_METHOD_TEST_CONCEPT(function_name)\
    template<typename, typename...Args> struct supports_##function_name##_impl : std::false_type {};\
    template<typename...Args> struct supports_##function_name##_impl<std::void_t<decltype(function_name<Args...>())>, Args...> : std::true_type {};
#endif

#define BI_OPERATOR_TEST(operator_name, operator_sign)\
    BI_OPERATOR_TEST_CONCEPT(operator_name, operator_sign)\
    template<class T1, class T2 = T1> struct supports_##operator_name : supports_##operator_name##_impl<void,T1,T2>{};\
    template<> struct supports_##operator_name<void> : std::false_type{};\
    template<class T> struct supports_##operator_name<T,void> : std::false_type{};\
    template<class T> struct supports_##operator_name<void,T> : std::false_type{};\
    template<class T1, class T2 = T1> static constexpr bool supports_##operator_name##_v = supports_##operator_name<T1,T2>::value;

#define PREFIX_OPERATOR_TEST(operator_name, operator_sign)\
    PREFIX_OPERATOR_TEST_CONCEPT(operator_name, operator_sign)\
    template<class T> struct supports_##operator_name : supports_##operator_name##_impl<void,T>{};\
    template<class T> static constexpr bool supports_##operator_name##_v = supports_##operator_name<T>::value;

#define SUFFIX_OPERATOR_TEST(operator_name, operator_sign)\
SUFFIX_OPERATOR_TEST_CONCEPT(operator_name, operator_sign)\
    template<class T> struct supports_##operator_name : supports_##operator_name##_impl<void,T>{};\
    template<class T> static constexpr bool supports_##operator_name##_v = supports_##operator_name<T>::value;

#define MEMBER_METHOD_TEST(function_name)\
    MEMBER_METHOD_TEST_CONCEPT(function_name)\
    template<typename T, typename...Args> struct supports_##function_name : supports_##function_name##_impl<void,T,Args...>{};\
    template<typename T, typename...Args> static constexpr bool supports_##function_name##_v = supports_##function_name<T,Args...>::value;

#define CONSTRUCTOR_TEST(function_name)\
    CONSTRUCTOR_TEST_CONCEPT(function_name)\
    template<typename T, typename...Args> struct supports_##function_name : supports_##function_name##_impl<void,T,Args...>{};\
    template<typename T, typename...Args> static constexpr bool supports_##function_name##_v = supports_##function_name<T,Args...>::value;

#define STATIC_FIELD_TEST(name)\
STATIC_FIELD_TEST_CONCEPT(name)\
    template<typename T> struct supports_##name : supports_##name##_impl<void,T>{};\
    template<typename T> static constexpr bool supports_##name##_v = supports_##name<T>::value;

#define TYPENAME_TEST(name)\
TYPENAME_TEST_CONCEPT(name)\
    template<typename T> struct supports_##name : supports_##name##_impl<void,T>{};\
    template<typename T> static constexpr bool supports_##name##_v = supports_##name<T>::value;

#define GLOBAL_METHOD_TEST(function_name)\
    GLOBAL_METHOD_TEST_CONCEPT(function_name)\
    template<typename...Args> struct supports_##function_name : supports_##function_name##_impl<void,Args...>{};\
    template<> struct supports_##function_name<void> : std::false_type{};\
    template<typename...Args> static constexpr bool supports_##function_name##_v = supports_##function_name<Args...>::value;

#define TEMPLATE_METHOD_TEST(function_name)\
    TEMPLATE_METHOD_TEST_CONCEPT(function_name)\
    template<typename...Args> struct supports_##function_name : supports_##function_name##_impl<void,Args...>{};\
    template<> struct supports_##function_name<void> : std::false_type{};\
    template<typename...Args> static constexpr bool supports_##function_name##_v = supports_##function_name<Args...>::value;

#define CONDITIONAL_CONTAINER_TEST_BASE(function_name)\
    template<class T> struct supports_##function_name<QList<T>> : supports_##function_name##_conditional<QList<int>,supports_##function_name<T>::value>{};\
    template<class T> struct supports_##function_name<QQueue<T>> : supports_##function_name##_conditional<QQueue<int>,supports_##function_name<T>::value>{};\
    template<class T> struct supports_##function_name<QStack<T>> : supports_##function_name##_conditional<QStack<int>,supports_##function_name<T>::value>{};\
    template<class T> struct supports_##function_name<QSet<T>> : supports_##function_name##_conditional<QSet<int>,supports_##function_name<T>::value>{};\
    template<class T1, class T2> struct supports_##function_name<std::pair<T1,T2>> : supports_##function_name##_conditional<std::pair<int,int>, supports_##function_name<T1>::value && supports_##function_name<T2>::value>{};\
    template<class T1, class T2> struct supports_##function_name<QMap<T1,T2>> : supports_##function_name##_conditional<QMap<int,int>, supports_##function_name<T1>::value && supports_##function_name<T2>::value>{};\
    template<class T1, class T2> struct supports_##function_name<QHash<T1,T2>> : supports_##function_name##_conditional<QHash<int,int>, supports_##function_name<T1>::value && supports_##function_name<T2>::value>{};\
    template<class T1, class T2> struct supports_##function_name<QMultiMap<T1,T2>> : supports_##function_name##_conditional<QMultiMap<int,int>, supports_##function_name<T1>::value && supports_##function_name<T2>::value>{};\
    template<class T1, class T2> struct supports_##function_name<QMultiHash<T1,T2>> : supports_##function_name##_conditional<QMultiHash<int,int>, supports_##function_name<T1>::value && supports_##function_name<T2>::value>{};\
    template<typename T, typename Alloc> struct supports_##function_name<std::vector<T, Alloc>> : supports_##function_name##_conditional<std::vector<int,Alloc>,supports_##function_name<T>::value>{};\
    template<typename T, typename Alloc> struct supports_##function_name<std::list<T, Alloc>> : supports_##function_name##_conditional<std::list<int,Alloc>,supports_##function_name<T>::value>{};\
    template<class T1, class T2, typename Compare, typename Alloc> struct supports_##function_name<std::map<T1,T2,Compare,Alloc>> : supports_##function_name##_conditional<std::map<int,int,Compare,Alloc>,supports_##function_name<T1>::value && supports_##function_name<T2>::value>{};\
    template<class T1, class T2, typename Compare, typename Alloc> struct supports_##function_name<std::multimap<T1,T2,Compare,Alloc>> : supports_##function_name##_conditional<std::multimap<int,int,Compare,Alloc>,supports_##function_name<T1>::value && supports_##function_name<T2>::value>{};\
    template<class T> struct supports_##function_name<const QList<T>&> : supports_##function_name##_conditional<const QList<int>&,supports_##function_name<T>::value>{};\
    template<class T> struct supports_##function_name<const QQueue<T>&> : supports_##function_name##_conditional<const QQueue<int>&,supports_##function_name<T>::value>{};\
    template<class T> struct supports_##function_name<const QStack<T>&> : supports_##function_name##_conditional<const QStack<int>&,supports_##function_name<T>::value>{};\
    template<class T> struct supports_##function_name<const QSet<T>&> : supports_##function_name##_conditional<const QSet<int>&,supports_##function_name<T>::value>{};\
    template<class T1, class T2> struct supports_##function_name<const std::pair<T1,T2>&> : supports_##function_name##_conditional<const std::pair<int,int>&, supports_##function_name<T1>::value && supports_##function_name<T2>::value>{};\
    template<class T1, class T2> struct supports_##function_name<const QMap<T1,T2>&> : supports_##function_name##_conditional<const QMap<int,int>&, supports_##function_name<T1>::value && supports_##function_name<T2>::value>{};\
    template<class T1, class T2> struct supports_##function_name<const QHash<T1,T2>&> : supports_##function_name##_conditional<const QHash<int,int>&, supports_##function_name<T1>::value && supports_##function_name<T2>::value>{};\
    template<class T1, class T2> struct supports_##function_name<const QMultiMap<T1,T2>&> : supports_##function_name##_conditional<const QMultiMap<int,int>&, supports_##function_name<T1>::value && supports_##function_name<T2>::value>{};\
    template<class T1, class T2> struct supports_##function_name<const QMultiHash<T1,T2>&> : supports_##function_name##_conditional<const QMultiHash<int,int>&, supports_##function_name<T1>::value && supports_##function_name<T2>::value>{};\
    template<typename T, typename Alloc> struct supports_##function_name<const std::vector<T, Alloc>&> : supports_##function_name##_conditional<const std::vector<int,Alloc>&, supports_##function_name<T>::value>{};\
    template<typename T, typename Alloc> struct supports_##function_name<const std::list<T, Alloc>&> : supports_##function_name##_conditional<const std::list<int,Alloc>&, supports_##function_name<T>::value>{};\
    template<class T1, class T2, typename Compare, typename Alloc> struct supports_##function_name<const std::map<T1,T2,Compare,Alloc>&> : supports_##function_name##_conditional<const std::map<int,int,Compare,Alloc>&, supports_##function_name<T1>::value && supports_##function_name<T2>::value>{};\
    template<class T1, class T2, typename Compare, typename Alloc> struct supports_##function_name<const std::multimap<T1,T2,Compare,Alloc>&> : supports_##function_name##_conditional<const std::multimap<int,int,Compare,Alloc>&, supports_##function_name<T1>::value && supports_##function_name<T2>::value>{};\
    template<class T> struct supports_##function_name<QList<T>&> : supports_##function_name##_conditional<QList<int>&,supports_##function_name<T>::value>{};\
    template<class T> struct supports_##function_name<QQueue<T>&> : supports_##function_name##_conditional<QQueue<int>&,supports_##function_name<T>::value>{};\
    template<class T> struct supports_##function_name<QStack<T>&> : supports_##function_name##_conditional<QStack<int>&,supports_##function_name<T>::value>{};\
    template<class T> struct supports_##function_name<QSet<T>&> : supports_##function_name##_conditional<QSet<int>&,supports_##function_name<T>::value>{};\
    template<class T1, class T2> struct supports_##function_name<std::pair<T1,T2>&> : supports_##function_name##_conditional<std::pair<int,int>&, supports_##function_name<T1>::value && supports_##function_name<T2>::value>{};\
    template<class T1, class T2> struct supports_##function_name<QMap<T1,T2>&> : supports_##function_name##_conditional<QMap<int,int>&, supports_##function_name<T1>::value && supports_##function_name<T2>::value>{};\
    template<class T1, class T2> struct supports_##function_name<QHash<T1,T2>&> : supports_##function_name##_conditional<QHash<int,int>&, supports_##function_name<T1>::value && supports_##function_name<T2>::value>{};\
    template<class T1, class T2> struct supports_##function_name<QMultiMap<T1,T2>&> : supports_##function_name##_conditional<QMultiMap<int,int>&, supports_##function_name<T1>::value && supports_##function_name<T2>::value>{};\
    template<class T1, class T2> struct supports_##function_name<QMultiHash<T1,T2>&> : supports_##function_name##_conditional<QMultiHash<int,int>&, supports_##function_name<T1>::value && supports_##function_name<T2>::value>{};\
    template<typename T, typename Alloc> struct supports_##function_name<std::vector<T, Alloc>&> : supports_##function_name##_conditional<std::vector<int,Alloc>&, supports_##function_name<T>::value>{};\
    template<typename T, typename Alloc> struct supports_##function_name<std::list<T, Alloc>&> : supports_##function_name##_conditional<std::list<int,Alloc>&, supports_##function_name<T>::value>{};\
    template<class T1, class T2, typename Compare, typename Alloc> struct supports_##function_name<std::map<T1,T2,Compare,Alloc>&> : supports_##function_name##_conditional<std::map<int,int,Compare,Alloc>&, supports_##function_name<T1>::value && supports_##function_name<T2>::value>{};\
    template<class T1, class T2, typename Compare, typename Alloc> struct supports_##function_name<std::multimap<T1,T2,Compare,Alloc>&> : supports_##function_name##_conditional<std::multimap<int,int,Compare,Alloc>&, supports_##function_name<T1>::value && supports_##function_name<T2>::value>{};\
    template<class T> struct supports_##function_name<QList<T>&&> : supports_##function_name##_conditional<QList<int>&&,supports_##function_name<T>::value>{};\
    template<class T> struct supports_##function_name<QQueue<T>&&> : supports_##function_name##_conditional<QQueue<int>&&,supports_##function_name<T>::value>{};\
    template<class T> struct supports_##function_name<QStack<T>&&> : supports_##function_name##_conditional<QStack<int>&&,supports_##function_name<T>::value>{};\
    template<class T> struct supports_##function_name<QSet<T>&&> : supports_##function_name##_conditional<QSet<int>&&,supports_##function_name<T>::value>{};\
    template<class T1, class T2> struct supports_##function_name<std::pair<T1,T2>&&> : supports_##function_name##_conditional<std::pair<int,int>&&, supports_##function_name<T1>::value && supports_##function_name<T2>::value>{};\
    template<class T1, class T2> struct supports_##function_name<QMap<T1,T2>&&> : supports_##function_name##_conditional<QMap<int,int>&&, supports_##function_name<T1>::value && supports_##function_name<T2>::value>{};\
    template<class T1, class T2> struct supports_##function_name<QHash<T1,T2>&&> : supports_##function_name##_conditional<QHash<int,int>&&, supports_##function_name<T1>::value && supports_##function_name<T2>::value>{};\
    template<class T1, class T2> struct supports_##function_name<QMultiMap<T1,T2>&&> : supports_##function_name##_conditional<QMultiMap<int,int>&&, supports_##function_name<T1>::value && supports_##function_name<T2>::value>{};\
    template<class T1, class T2> struct supports_##function_name<QMultiHash<T1,T2>&&> : supports_##function_name##_conditional<QMultiHash<int,int>&&, supports_##function_name<T1>::value && supports_##function_name<T2>::value>{};\
    template<typename T, typename Alloc> struct supports_##function_name<std::vector<T, Alloc>&&> : supports_##function_name##_conditional<std::vector<int,Alloc>&&, supports_##function_name<T>::value>{};\
    template<typename T, typename Alloc> struct supports_##function_name<std::list<T, Alloc>&&> : supports_##function_name##_conditional<std::list<int,Alloc>&&, supports_##function_name<T>::value>{};\
    template<class T1, class T2, typename Compare, typename Alloc> struct supports_##function_name<std::map<T1,T2,Compare,Alloc>&&> : supports_##function_name##_conditional<std::map<int,int,Compare,Alloc>&&, supports_##function_name<T1>::value && supports_##function_name<T2>::value>{};\
    template<class T1, class T2, typename Compare, typename Alloc> struct supports_##function_name<std::multimap<T1,T2,Compare,Alloc>&&> : supports_##function_name##_conditional<std::multimap<int,int,Compare,Alloc>&&, supports_##function_name<T1>::value && supports_##function_name<T2>::value>{};\

#define CONDITIONAL_CONTAINER_TEST(function_name)\
    template<class T, bool> struct supports_##function_name##_conditional : supports_##function_name##_impl<void,T>{};\
    template<class T> struct supports_##function_name##_conditional<T,false> : std::false_type{};\
    CONDITIONAL_CONTAINER_TEST_BASE(function_name)

#define CONDITIONAL_BI_OPERATOR_TEST(operator_name)\
    template<class T, bool> struct supports_##operator_name##_conditional : supports_##operator_name##_impl<void,T>{};\
    template<class T> struct supports_##operator_name##_conditional<T,false> : std::false_type{};\
    CONDITIONAL_CONTAINER_TEST_BASE(operator_name)\
    template<class T1, class T2> struct supports_##operator_name<T1*, T2*> : std::true_type{};

BI_OPERATOR_TEST(assign,=)
BI_OPERATOR_TEST(equal,==)
CONDITIONAL_BI_OPERATOR_TEST(equal)
BI_OPERATOR_TEST(less_than,<)
CONDITIONAL_BI_OPERATOR_TEST(less_than)
BI_OPERATOR_TEST(greater_than,<)
CONDITIONAL_BI_OPERATOR_TEST(greater_than)
BI_OPERATOR_TEST(less_or_eual,<=)
CONDITIONAL_BI_OPERATOR_TEST(less_or_eual)
BI_OPERATOR_TEST(greater_or_eual,<=)
CONDITIONAL_BI_OPERATOR_TEST(greater_or_eual)

BI_OPERATOR_TEST(not_equal,!=)
BI_OPERATOR_TEST(add,+)
BI_OPERATOR_TEST(subtract,-)
BI_OPERATOR_TEST(multiply,*)
BI_OPERATOR_TEST(divide,/)
BI_OPERATOR_TEST(add_assign,+=)
BI_OPERATOR_TEST(subtract_assign,-=)
BI_OPERATOR_TEST(multiply_assign,*=)
BI_OPERATOR_TEST(divide_assign,/=)
BI_OPERATOR_TEST(or_assign,|=)
BI_OPERATOR_TEST(and_assign,&=)
BI_OPERATOR_TEST(xor_assign,^=)
BI_OPERATOR_TEST(rem_assign,%=)
BI_OPERATOR_TEST(or,|)
BI_OPERATOR_TEST(and,&)
BI_OPERATOR_TEST(xor,^)
BI_OPERATOR_TEST(rem,%)
BI_OPERATOR_TEST(exclusive_or,||)
BI_OPERATOR_TEST(exclusive_and,&&)
PREFIX_OPERATOR_TEST(not,!)
PREFIX_OPERATOR_TEST(invert,~)

#ifdef Q_COMPILER_CONCEPTS
template<typename, class T1, class T2 = T1> concept supports_subscribe_impl = requires(T1 t1, T2 t2){t1[t2];};
#else
template<typename, class T1, class T2> struct supports_subscribe_impl : std::false_type {};
template<class T1, class T2> struct supports_subscribe_impl<std::void_t<decltype(std::declval<T1>()[std::declval<T2>()])>, T1, T2> : std::true_type {};
#endif
template<class T1, class T2> struct supports_subscribe : supports_subscribe_impl<void,T1,T2>{};
template<class T1, class T2>
static constexpr bool supports_subscribe_v = supports_subscribe<T1,T2>::value;

BI_OPERATOR_TEST(streamin,<<)
BI_OPERATOR_TEST(streamout,>>)

GLOBAL_METHOD_TEST(qHash)
CONDITIONAL_CONTAINER_TEST(qHash)
template<class T> struct supports_qHash<T*> : std::true_type{};

template<typename, class T> struct supports_debugstream_impl : supports_streamin<QDebug&,T>{};
template<class T> struct supports_debugstream : supports_debugstream_impl<void,T>{};
template<> struct supports_debugstream<void> : std::false_type{};
template<typename T>
constexpr bool supports_debugstream_v = supports_debugstream<T>::value;
CONDITIONAL_CONTAINER_TEST(debugstream)

template<typename, class T> struct supports_stream_operators_impl : std::conjunction<supports_streamin<QDataStream&,const T&>, supports_streamout<QDataStream&,T&>>{};
template<class T> struct supports_stream_operators : supports_stream_operators_impl<void,T>{};
template<> struct supports_stream_operators<void> : std::false_type{};
template<typename T>
constexpr bool supports_stream_operators_v = supports_stream_operators<T>::value;
CONDITIONAL_CONTAINER_TEST(stream_operators)

PREFIX_OPERATOR_TEST(increment,++)
PREFIX_OPERATOR_TEST(decrement,--)
PREFIX_OPERATOR_TEST(deref,*)
PREFIX_OPERATOR_TEST(ref,&)
SUFFIX_OPERATOR_TEST(suffix_increment,++)
SUFFIX_OPERATOR_TEST(suffix_decrement,--)

MEMBER_METHOD_TEST(firstKey)
MEMBER_METHOD_TEST(lastKey)
MEMBER_METHOD_TEST(isSharedWith)
MEMBER_METHOD_TEST(key)
MEMBER_METHOD_TEST(keys)
MEMBER_METHOD_TEST(value)
MEMBER_METHOD_TEST(values)
MEMBER_METHOD_TEST(isBegin)
MEMBER_METHOD_TEST(isEnd)
MEMBER_METHOD_TEST(isValid)
MEMBER_METHOD_TEST(initialEnd)
MEMBER_METHOD_TEST(initialBegin)
CONSTRUCTOR_TEST(new)

TEMPLATE_METHOD_TEST(qobject_interface_iid)

MEMBER_METHOD_TEST(lowerBound)
MEMBER_METHOD_TEST(upperBound)
MEMBER_METHOD_TEST(size)
MEMBER_METHOD_TEST(at)
MEMBER_METHOD_TEST(remove)
MEMBER_METHOD_TEST(replace)
MEMBER_METHOD_TEST(find)
MEMBER_METHOD_TEST(constFind)
MEMBER_METHOD_TEST(take)
MEMBER_METHOD_TEST(insert)
MEMBER_METHOD_TEST(clear)
MEMBER_METHOD_TEST(begin)
MEMBER_METHOD_TEST(constBegin)
MEMBER_METHOD_TEST(keyBegin)
MEMBER_METHOD_TEST(keyValueBegin)
MEMBER_METHOD_TEST(constKeyValueBegin)
MEMBER_METHOD_TEST(rbegin)
MEMBER_METHOD_TEST(crbegin)
MEMBER_METHOD_TEST(end)
MEMBER_METHOD_TEST(constEnd)
MEMBER_METHOD_TEST(rend)
MEMBER_METHOD_TEST(crend)
MEMBER_METHOD_TEST(keyEnd)
MEMBER_METHOD_TEST(keyValueEnd)
MEMBER_METHOD_TEST(constKeyValueEnd)
MEMBER_METHOD_TEST(constReverseBegin)
MEMBER_METHOD_TEST(constReverseEnd)
MEMBER_METHOD_TEST(reverseBegin)
MEMBER_METHOD_TEST(reverseEnd)
MEMBER_METHOD_TEST(first)
MEMBER_METHOD_TEST(last)
MEMBER_METHOD_TEST(constFirst)
MEMBER_METHOD_TEST(constLast)
MEMBER_METHOD_TEST(count)
MEMBER_METHOD_TEST(unite)
MEMBER_METHOD_TEST(capacity)
MEMBER_METHOD_TEST(contains)
MEMBER_METHOD_TEST(reserve)
MEMBER_METHOD_TEST(uniqueKeys)
MEMBER_METHOD_TEST(isDetached)

TYPENAME_TEST(iterator)
TYPENAME_TEST(const_iterator)
TYPENAME_TEST(reverse_iterator)
TYPENAME_TEST(const_reverse_iterator)
TYPENAME_TEST(key_value_iterator)
TYPENAME_TEST(const_key_value_iterator)
TYPENAME_TEST(key_iterator)
TYPENAME_TEST(sentinel)
TYPENAME_TEST(iterator_category)
TYPENAME_TEST(value_type)
TYPENAME_TEST(difference_type)

template<template<typename K, typename T> class Container, typename K, typename T>
struct supports_map_sort : supports_less_than<K>{};

template<typename K, typename T>
struct supports_map_sort<QHash,K,T> : supports_qHash<K>{};

template<typename K, typename T>
struct supports_map_sort<QMultiHash,K,T> : supports_qHash<K>{};


template<template<typename K, typename T> class Container, typename K, typename T>
constexpr bool supports_map_sort_v = supports_map_sort<Container,K,T>::value;

template<typename Iterator, bool support = supports_iterator_category_v<std::iterator_traits<Iterator>>>
struct is_random_access_iterator : std::is_convertible<typename std::iterator_traits<Iterator>::iterator_category, std::random_access_iterator_tag>{
};

template<typename Iterator>
struct is_random_access_iterator<Iterator,false> : std::false_type{
};

template<typename Iterator>
constexpr bool is_random_access_iterator_v = is_random_access_iterator<Iterator>::value;

template<typename Iterator, bool support = supports_iterator_category_v<std::iterator_traits<Iterator>>>
struct is_bidirectional_iterator : std::is_convertible<typename std::iterator_traits<Iterator>::iterator_category, std::bidirectional_iterator_tag>{
};

template<typename Iterator>
struct is_bidirectional_iterator<Iterator,false> : std::false_type{
};

template<typename Iterator>
constexpr bool is_bidirectional_iterator_v = is_bidirectional_iterator<Iterator>::value;

template<typename Iterator, bool support = supports_iterator_category_v<std::iterator_traits<Iterator>>>
struct is_forward_iterator : std::is_convertible<typename std::iterator_traits<Iterator>::iterator_category, std::forward_iterator_tag>{
};

template<typename Iterator>
struct is_forward_iterator<Iterator,false> : std::false_type{
};

template<typename Iterator>
constexpr bool is_forward_iterator_v = is_forward_iterator<Iterator>::value;

template<typename Iterator, bool support = supports_iterator_category_v<std::iterator_traits<Iterator>>>
struct is_output_iterator : std::is_convertible<typename std::iterator_traits<Iterator>::iterator_category, std::output_iterator_tag>{
};

template<typename Iterator>
struct is_output_iterator<Iterator,false> : std::false_type{
};

template<typename Iterator>
constexpr bool is_output_iterator_v = is_output_iterator<Iterator>::value;

template<typename Iterator, bool support = supports_iterator_category_v<std::iterator_traits<Iterator>>>
struct is_input_iterator : std::is_convertible<typename std::iterator_traits<Iterator>::iterator_category, std::input_iterator_tag>{
};

template<typename Iterator>
struct is_input_iterator<Iterator,false> : std::false_type{
};

template<typename Iterator>
constexpr bool is_input_iterator_v = is_input_iterator<Iterator>::value;

template<typename T>
struct qtjambi_type;

template<template<typename T> class Container, typename T>
struct qtjambi_type_container1{
    using type = Container<T>;
};

template<template<typename T> class Property, typename T, int size = sizeof(T), bool isInteger = std::is_integral_v<T>, bool isFloatingPoint = std::is_floating_point_v<T>>
struct qtjambi_type_property_decider{
    using type = Property<QVariant>;
};
template<template<typename> class Property>
struct qtjambi_type_property_decider<Property,QChar,sizeof(QChar),false,false>{
    using type = Property<QChar>;
};
template<template<typename> class Property>
struct qtjambi_type_property_decider<Property,bool,sizeof(bool),true,false>{
    using type = Property<bool>;
};
template<template<typename T> class Property, typename T, int size>
struct qtjambi_type_property_decider<Property,T,size,true,false>{
    using type = Property<typename QIntegerForSizeof<T>::Signed>;
};
template<template<typename> class Property>
struct qtjambi_type_property_decider<Property,char16_t,sizeof(char16_t),true,false>{
    using type = Property<char16_t>;
};
template<template<typename> class Property>
struct qtjambi_type_property_decider<Property,float,sizeof(float),false,true>{
    using type = Property<float>;
};
template<template<typename> class Property>
struct qtjambi_type_property_decider<Property,double,sizeof(double),false,true>{
    using type = Property<double>;
};
template<typename T>
struct qtjambi_type_container1<QPropertyBinding,T>
        : qtjambi_type_property_decider<QPropertyBinding,T>{
};
template<typename T>
struct qtjambi_type_container1<QBindable,T>
        : qtjambi_type_property_decider<QBindable,T>{
};
template<typename T>
struct qtjambi_type_container1<QPropertyChangeHandler,T>{
    using type = QPropertyChangeHandler<void(*)()>;
};

template<typename T>
struct qtjambi_type_container1<QFutureInterface,T>{
    using type = QFutureInterface<QVariant>;
};

template<>
struct qtjambi_type_container1<QFutureInterface,void>{
    using type = QFutureInterface<void>;
};

template<typename T>
struct qtjambi_type_container1<QFuture,T>{
    using type = QFuture<QVariant>;
};

template<>
struct qtjambi_type_container1<QFuture,void>{
    using type = QFuture<void>;
};

template<typename T>
struct qtjambi_type_container1<QFutureWatcher,T>{
    using type = QFutureWatcher<QVariant>;
};
template<>
struct qtjambi_type_container1<QFutureWatcher,void>{
    using type = QFutureWatcher<void>;
};

template<typename T>
struct qtjambi_type_container1<QList,T>{
    using type = QList<QVariant>;
};
template<typename T>
struct qtjambi_type_container1<QSet,T>{
    using type = QSet<QVariant>;
};
template<typename T>
struct qtjambi_type_container1<QQueue,T>{
    using type = QQueue<QVariant>;
};
template<typename T>
struct qtjambi_type_container1<QStack,T>{
    using type = QStack<QVariant>;
};

template<typename T>
struct qtjambi_type_container1<QQmlListProperty,T>{
    using type = QQmlListProperty<QObject>;
};

template<typename T>
struct qtjambi_type_container1<QPointer,T> : qtjambi_type<T>{
};

template<typename T>
struct qtjambi_type_container1<QSharedPointer,T> : qtjambi_type<T>{
};

template<typename T>
struct qtjambi_type_container1<QWeakPointer,T> : qtjambi_type<T>{
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct qtjambi_type_container2{
    using type = Container<K,T>;
};

template<typename K, typename T>
struct qtjambi_type_container2<std::pair,K,T>{
    using type = std::pair<QVariant,QVariant>;
};

template<typename K, typename T>
struct qtjambi_type_container2<QMap,K,T>{
    using type = QMap<QVariant,QVariant>;
};

template<typename K, typename T>
struct qtjambi_type_container2<QMultiMap,K,T>{
    using type = QMultiMap<QVariant,QVariant>;
};

template<typename K, typename T>
struct qtjambi_type_container2<QHash,K,T>{
    using type = QHash<QVariant,QVariant>;
};

template<typename K, typename T>
struct qtjambi_type_container2<QMultiHash,K,T>{
    using type = QMultiHash<QVariant,QVariant>;
};

template<typename K, typename T>
struct qtjambi_type_container2<QScopedPointer,K,T> : qtjambi_type<K>{
};

template<typename K, typename T>
struct qtjambi_type_container2<std::unique_ptr,K,T> : qtjambi_type<K>{
};

template<typename T, typename A>
struct qtjambi_type_container2<std::vector,T,A>{
    using type = std::vector<QVariant>;
};

template<typename T, typename A>
struct qtjambi_type_container2<std::list,T,A>{
    using type = std::list<QVariant>;
};

template<template<typename K, typename T, typename A> class Container, typename K, typename T, typename A>
struct qtjambi_type_container3{
    using type = Container<K,T,A>;
};

#if defined(_SET_) || defined(_SET) || defined(_LIBCPP_SET) || defined(_GLIBCXX_SET)
template<typename T, typename A, typename B>
struct qtjambi_type_container3<std::set,T,A,B>{
    using type = std::set<QVariant>;
};

template<typename T, typename A, typename B>
struct qtjambi_type_container3<std::multiset,T,A,B>{
    using type = std::multiset<QVariant>;
};
#endif

template<template<typename K, typename T, typename A, typename B> class Container, typename K, typename T, typename A, typename B>
struct qtjambi_type_container4{
    using type = Container<K,T,A,B>;
};

template<typename K, typename T, typename A, typename B>
struct qtjambi_type_container4<std::map,K,T,A,B>{
    using type = std::map<QVariant,QVariant>;
};

template<typename K, typename T, typename A, typename B>
struct qtjambi_type_container4<std::multimap,K,T,A,B>{
    using type = std::multimap<QVariant,QVariant>;
};

#if defined(_UNORDERED_SET_) || defined(_UNORDERED_SET) || defined(_LIBCPP_UNORDERED_SET) || defined(_GLIBCXX_UNORDERED_SET)
template<typename K, typename T, typename A, typename B>
struct qtjambi_type_container4<std::unordered_set,K,T,A,B>{
    using type = std::unordered_set<QVariant>;
};
#endif

template<template<typename K, typename T, typename A, typename B, typename C> class Container, typename K, typename T, typename A, typename B, typename C>
struct qtjambi_type_container5{
    using type = Container<K,T,A,B,C>;
};

#if defined(_UNORDERED_MAP_) || defined(_UNORDERED_MAP) || defined(_LIBCPP_UNORDERED_MAP) || defined(_GLIBCXX_UNORDERED_MAP)
template<typename K, typename T, typename A, typename B, typename C>
struct qtjambi_type_container5<std::unordered_map,K,T,A,B,C>{
    using type = std::unordered_map<QVariant,QVariant>;
};

template<typename K, typename T, typename A, typename B, typename C>
struct qtjambi_type_container5<std::unordered_multimap,K,T,A,B,C>{
    using type = std::unordered_multimap<QVariant,QVariant>;
};
#endif

template<template<typename...Ts> class Container, int parameterCount, typename...Ts>
struct qtjambi_type_container_selector{
};

template<template<typename T> class _Container, typename T>
constexpr qtjambi_type_container1<_Container, T> qtjambi_type_container1_selector(){ return {}; }

template<template<typename...Ts> class Container, typename...Ts>
struct qtjambi_type_container_selector<Container, 1, Ts...> : decltype(qtjambi_type_container1_selector<Container,Ts...>()){
};

template<template<typename K, typename T> class _Container, typename K, typename T>
constexpr qtjambi_type_container2<_Container, K, T> qtjambi_type_container2_selector(){ return {}; }

template<template<typename...Ts> class Container, typename...Ts>
struct qtjambi_type_container_selector<Container, 2, Ts...> : decltype(qtjambi_type_container2_selector<Container,Ts...>()){
};

template<template<typename K, typename T, typename A> class _Container, typename K, typename T, typename A>
constexpr qtjambi_type_container3<_Container, K, T, A> qtjambi_type_container3_selector(){ return {}; }

template<template<typename...Ts> class Container, typename...Ts>
struct qtjambi_type_container_selector<Container, 3, Ts...> : decltype(qtjambi_type_container3_selector<Container,Ts...>()){
};

template<template<typename K, typename T, typename A, typename B> class _Container, typename K, typename T, typename A, typename B>
constexpr qtjambi_type_container4<_Container, K, T, A, B> qtjambi_type_container4_selector(){ return {}; }

template<template<typename...Ts> class Container, typename...Ts>
struct qtjambi_type_container_selector<Container, 4, Ts...> : decltype(qtjambi_type_container4_selector<Container,Ts...>()){
};

template<template<typename K, typename T, typename A, typename B, typename C> class _Container, typename K, typename T, typename A, typename B, typename C>
constexpr qtjambi_type_container5<_Container, K, T, A, B, C> qtjambi_type_container5_selector(){ return {}; }

template<template<typename...Ts> class Container, typename...Ts>
struct qtjambi_type_container_selector<Container, 5, Ts...> : decltype(qtjambi_type_container5_selector<Container,Ts...>()){
};

template<typename T>
struct qtjambi_type_selector{
    using type = T;
};

template<template<typename...Ts> class Container, typename...Ts>
struct qtjambi_type_selector<Container<Ts...>> : qtjambi_type_container_selector<Container,sizeof...(Ts),Ts...>{
};

template<typename T>
struct qtjambi_type{
    using type = typename qtjambi_type_selector<std::conditional_t<std::is_function_v<std::remove_pointer_t<T>>, T, std::remove_pointer_t<T>>>::type;
    static constexpr const std::type_info& id() {return typeid(type);}
};

template<typename O, bool = std::is_pointer_v<O>>
struct pointer_from{
    typedef std::add_lvalue_reference_t<std::add_const_t<O>> In;
    typedef std::add_pointer_t<O> Out;
    static const void* from(In o){
        return &o;
    }
};

template<typename O>
struct pointer_from<O,true>{
    typedef O In;
    typedef O Out;
    static const void* from(O o){
        return o;
    }
};

#if !defined(Q_CC_MSVC) && !defined(QTJAMBI_GENERATOR_RUNNING)
template<typename RET, typename... ARGS>
struct pointer_from<RET(*)(ARGS...),true>{
    typedef RET(*In)(ARGS...);
    typedef RET(*Out)(ARGS...);
    static const void* from(In o){
        return reinterpret_cast<const void*>(o);
    }
};
#endif

template<typename T>
struct result_of{
};

template<typename T>
using result_of_t = typename result_of<T>::type;

template<typename RET, typename... ARGS>
struct result_of<RET(*)(ARGS...)>{
    typedef RET type;
};

template<typename RET, typename... ARGS>
struct result_of<RET(&)(ARGS...)>{
    typedef RET type;
};

template<typename RET, typename... ARGS>
struct result_of<RET(*const)(ARGS...)>{
    typedef RET type;
};

template<typename JArray>
struct jni_primitive_array_functions;

template<>
struct jni_primitive_array_functions<jbyteArray>{
    static constexpr auto NewArray = &JNIEnv::NewByteArray;
    static constexpr auto GetArrayRegion = &JNIEnv::GetByteArrayRegion;
    static constexpr auto SetArrayRegion = &JNIEnv::SetByteArrayRegion;
    static constexpr auto GetArrayElements = &JNIEnv::GetByteArrayElements;
    static constexpr auto ReleaseArrayElements = &JNIEnv::ReleaseByteArrayElements;
};

template<>
struct jni_primitive_array_functions<jshortArray>{
    static constexpr auto NewArray = &JNIEnv::NewShortArray;
    static constexpr auto GetArrayRegion = &JNIEnv::GetShortArrayRegion;
    static constexpr auto SetArrayRegion = &JNIEnv::SetShortArrayRegion;
    static constexpr auto GetArrayElements = &JNIEnv::GetShortArrayElements;
    static constexpr auto ReleaseArrayElements = &JNIEnv::ReleaseShortArrayElements;
};

template<>
struct jni_primitive_array_functions<jintArray>{
    static constexpr auto NewArray = &JNIEnv::NewIntArray;
    static constexpr auto GetArrayRegion = &JNIEnv::GetIntArrayRegion;
    static constexpr auto SetArrayRegion = &JNIEnv::SetIntArrayRegion;
    static constexpr auto GetArrayElements = &JNIEnv::GetIntArrayElements;
    static constexpr auto ReleaseArrayElements = &JNIEnv::ReleaseIntArrayElements;
};

template<>
struct jni_primitive_array_functions<jlongArray>{
    static constexpr auto NewArray = &JNIEnv::NewLongArray;
    static constexpr auto GetArrayRegion = &JNIEnv::GetLongArrayRegion;
    static constexpr auto SetArrayRegion = &JNIEnv::SetLongArrayRegion;
    static constexpr auto GetArrayElements = &JNIEnv::GetLongArrayElements;
    static constexpr auto ReleaseArrayElements = &JNIEnv::ReleaseLongArrayElements;
};

template<>
struct jni_primitive_array_functions<jfloatArray>{
    static constexpr auto NewArray = &JNIEnv::NewFloatArray;
    static constexpr auto GetArrayRegion = &JNIEnv::GetFloatArrayRegion;
    static constexpr auto SetArrayRegion = &JNIEnv::SetFloatArrayRegion;
    static constexpr auto GetArrayElements = &JNIEnv::GetFloatArrayElements;
    static constexpr auto ReleaseArrayElements = &JNIEnv::ReleaseFloatArrayElements;
};

template<>
struct jni_primitive_array_functions<jdoubleArray>{
    static constexpr auto NewArray = &JNIEnv::NewDoubleArray;
    static constexpr auto GetArrayRegion = &JNIEnv::GetDoubleArrayRegion;
    static constexpr auto SetArrayRegion = &JNIEnv::SetDoubleArrayRegion;
    static constexpr auto GetArrayElements = &JNIEnv::GetDoubleArrayElements;
    static constexpr auto ReleaseArrayElements = &JNIEnv::ReleaseDoubleArrayElements;
};

template<>
struct jni_primitive_array_functions<jcharArray>{
    static constexpr auto NewArray = &JNIEnv::NewCharArray;
    static constexpr auto GetArrayRegion = &JNIEnv::GetCharArrayRegion;
    static constexpr auto SetArrayRegion = &JNIEnv::SetCharArrayRegion;
    static constexpr auto GetArrayElements = &JNIEnv::GetCharArrayElements;
    static constexpr auto ReleaseArrayElements = &JNIEnv::ReleaseCharArrayElements;
};

template<>
struct jni_primitive_array_functions<jbooleanArray>{
    static constexpr auto NewArray = &JNIEnv::NewBooleanArray;
    static constexpr auto GetArrayRegion = &JNIEnv::GetBooleanArrayRegion;
    static constexpr auto SetArrayRegion = &JNIEnv::SetBooleanArrayRegion;
    static constexpr auto GetArrayElements = &JNIEnv::GetBooleanArrayElements;
    static constexpr auto ReleaseArrayElements = &JNIEnv::ReleaseBooleanArrayElements;
};

template<class T>
struct is_template : std::false_type{
};

template<template<typename...Ts> class C, typename...Ts>
struct is_template<C<Ts...>> : std::true_type{
};

template<typename O>
struct jni_type{
    static constexpr bool isObject = false;
    static constexpr bool isArray = false;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = false;
};

template<>
struct jni_type<jint>{
    static constexpr bool isObject = false;
    static constexpr bool isArray = false;
    static constexpr bool isPrimitive = true;
    static constexpr bool isPrimitiveArray = false;
    typedef jintArray ArrayType;
};

template<>
struct jni_type<jbyte>{
    static constexpr bool isObject = false;
    static constexpr bool isArray = false;
    static constexpr bool isPrimitive = true;
    static constexpr bool isPrimitiveArray = false;
    typedef jbyteArray ArrayType;
};

template<>
struct jni_type<jshort>{
    static constexpr bool isObject = false;
    static constexpr bool isArray = false;
    static constexpr bool isPrimitive = true;
    static constexpr bool isPrimitiveArray = false;
    typedef jshortArray ArrayType;
};

template<>
struct jni_type<jlong>{
    static constexpr bool isObject = false;
    static constexpr bool isArray = false;
    static constexpr bool isPrimitive = true;
    static constexpr bool isPrimitiveArray = false;
    typedef jlongArray ArrayType;
};

template<>
struct jni_type<jchar>{
    static constexpr bool isObject = false;
    static constexpr bool isArray = false;
    static constexpr bool isPrimitive = true;
    static constexpr bool isPrimitiveArray = false;
    typedef jcharArray ArrayType;
};

template<>
struct jni_type<jboolean>{
    static constexpr bool isObject = false;
    static constexpr bool isArray = false;
    static constexpr bool isPrimitive = true;
    static constexpr bool isPrimitiveArray = false;
    typedef jbooleanArray ArrayType;
};

template<>
struct jni_type<jfloat>{
    static constexpr bool isObject = false;
    static constexpr bool isArray = false;
    static constexpr bool isPrimitive = true;
    static constexpr bool isPrimitiveArray = false;
    typedef jfloatArray ArrayType;
};

template<>
struct jni_type<jdouble>{
    static constexpr bool isObject = false;
    static constexpr bool isArray = false;
    static constexpr bool isPrimitive = true;
    static constexpr bool isPrimitiveArray = false;
    typedef jdoubleArray ArrayType;
};

template<>
struct jni_type<jobject>{
    static constexpr bool isObject = true;
    static constexpr bool isArray = false;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = false;
};

template<>
struct jni_type<jthrowable>{
    static constexpr bool isObject = true;
    static constexpr bool isArray = false;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = false;
};

template<>
struct jni_type<jstring>{
    static constexpr bool isObject = true;
    static constexpr bool isArray = false;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = false;
};

template<>
struct jni_type<jclass>{
    static constexpr bool isObject = true;
    static constexpr bool isArray = false;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = false;
};

template<>
struct jni_type<jarray>{
    static constexpr bool isObject = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = false;
    static constexpr bool isIntegerArray = false;
    static constexpr bool isFloatingPointArray = false;
};

template<>
struct jni_type<jobjectArray>{
    static constexpr bool isObject = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = false;
    static constexpr bool isIntegerArray = false;
    static constexpr bool isFloatingPointArray = false;
    typedef jobject ElementType;
};

template<>
struct jni_type<jintArray>{
    static constexpr bool isObject = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = true;
    static constexpr bool isIntegerArray = true;
    static constexpr bool isFloatingPointArray = false;
    static constexpr size_t primitiveSize = sizeof(jint);
    typedef jint ElementType;
};

template<>
struct jni_type<jbyteArray>{
    static constexpr bool isObject = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = true;
    static constexpr bool isIntegerArray = true;
    static constexpr bool isFloatingPointArray = false;
    static constexpr size_t primitiveSize = sizeof(jbyte);
    typedef jbyte ElementType;
};

template<>
struct jni_type<jshortArray>{
    static constexpr bool isObject = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = true;
    static constexpr bool isIntegerArray = true;
    static constexpr bool isFloatingPointArray = false;
    static constexpr size_t primitiveSize = sizeof(jshort);
    typedef jshort ElementType;
};

template<>
struct jni_type<jlongArray>{
    static constexpr bool isObject = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = true;
    static constexpr bool isIntegerArray = true;
    static constexpr bool isFloatingPointArray = false;
    static constexpr size_t primitiveSize = sizeof(jlong);
    typedef jlong ElementType;
};

template<>
struct jni_type<jcharArray>{
    static constexpr bool isObject = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = true;
    static constexpr bool isIntegerArray = false;
    static constexpr bool isFloatingPointArray = false;
    static constexpr size_t primitiveSize = sizeof(jchar);
    typedef jchar ElementType;
};

template<>
struct jni_type<jbooleanArray>{
    static constexpr bool isObject = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = true;
    static constexpr bool isIntegerArray = false;
    static constexpr bool isFloatingPointArray = false;
    static constexpr size_t primitiveSize = sizeof(jboolean);
    typedef jboolean ElementType;
};

template<>
struct jni_type<jdoubleArray>{
    static constexpr bool isObject = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = true;
    static constexpr bool isIntegerArray = false;
    static constexpr bool isFloatingPointArray = true;
    static constexpr size_t primitiveSize = sizeof(jdouble);
    typedef jdouble ElementType;
};

template<>
struct jni_type<jfloatArray>{
    static constexpr bool isObject = true;
    static constexpr bool isArray = true;
    static constexpr bool isPrimitive = false;
    static constexpr bool isPrimitiveArray = true;
    static constexpr bool isIntegerArray = false;
    static constexpr bool isFloatingPointArray = true;
    static constexpr size_t primitiveSize = sizeof(jfloat);
    typedef jfloat ElementType;
};

template<typename O>
using jni_array_element_type_t = typename jni_type<O>::ElementType;

template<typename O>
constexpr bool is_jni_array_type_v = jni_type<O>::isArray;

template<typename O>
constexpr bool is_jni_primitive_array_type_v = jni_type<O>::isPrimitiveArray;

template<typename O>
constexpr bool is_jni_integer_array_type_v = jni_type<O>::isIntegerArray;

template<typename O>
constexpr bool is_jni_floatingpoint_array_type_v = jni_type<O>::isFloatingPointArray;

template<typename O>
constexpr bool is_jni_object_type_v = jni_type<O>::isObject;

template<typename O>
constexpr bool is_jni_primitive_type_v = jni_type<O>::isPrimitive;
}

#endif // QTJAMBI_TYPETESTS_H
