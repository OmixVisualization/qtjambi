/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0) && defined(QLINKEDLIST_H)
QT_WARNING_DISABLE_GCC("-Wdeprecated-declarations")
QT_WARNING_DISABLE_DEPRECATED
#endif

namespace QtJambiPrivate {

template<class T>
struct is_default_constructible : std::is_default_constructible<T>{};

template<class T>
struct is_copy_constructible : std::is_copy_constructible<T>{};

template<class T>
struct is_move_constructible : std::is_move_constructible<T>{};

template<class T>
struct is_copy_assignable : std::is_copy_assignable<T>{};

template<class T>
struct is_move_assignable : std::is_move_assignable<T>{};

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
template<class T1, class T2>
struct is_default_constructible<QPair<T1,T2>> : std::conditional<std::is_default_constructible<T1>::value && std::is_default_constructible<T2>::value, std::true_type, std::false_type>::type{};
template<class T1, class T2>
struct is_copy_constructible<QPair<T1,T2>> : std::conditional<std::is_copy_constructible<T1>::value && std::is_copy_constructible<T2>::value, std::true_type, std::false_type>::type{};
template<class T1, class T2>
struct is_move_constructible<QPair<T1,T2>> : std::conditional<std::is_move_constructible<T1>::value && std::is_move_constructible<T2>::value, std::true_type, std::false_type>::type{};
template<class T1, class T2>
struct is_copy_assignable<QPair<T1,T2>> : std::conditional<std::is_copy_assignable<T1>::value && std::is_copy_assignable<T2>::value, std::true_type, std::false_type>::type{};
template<class T1, class T2>
struct is_move_assignable<QPair<T1,T2>> : std::conditional<std::is_move_assignable<T1>::value && std::is_move_assignable<T2>::value, std::true_type, std::false_type>::type{};
#endif
template<class T1, class T2>
struct is_default_constructible<std::pair<T1,T2>> : std::conditional<std::is_default_constructible<T1>::value && std::is_default_constructible<T2>::value, std::true_type, std::false_type>::type{};
template<class T1, class T2>
struct is_copy_constructible<std::pair<T1,T2>> : std::conditional<std::is_copy_constructible<T1>::value && std::is_copy_constructible<T2>::value, std::true_type, std::false_type>::type{};
template<class T1, class T2>
struct is_move_constructible<std::pair<T1,T2>> : std::conditional<std::is_move_constructible<T1>::value && std::is_move_constructible<T2>::value, std::true_type, std::false_type>::type{};
template<class T1, class T2>
struct is_copy_assignable<std::pair<T1,T2>> : std::conditional<std::is_copy_assignable<T1>::value && std::is_copy_assignable<T2>::value, std::true_type, std::false_type>::type{};
template<class T1, class T2>
struct is_move_assignable<std::pair<T1,T2>> : std::conditional<std::is_move_assignable<T1>::value && std::is_move_assignable<T2>::value, std::true_type, std::false_type>::type{};

template<class T, class = decltype(qobject_interface_iid<T*>())>
std::true_type  supports_IID_test(const T&);
std::false_type supports_IID_test(...);

template<class T> struct supports_IID : decltype(supports_IID_test(std::declval<T>())){};

template<class T, class = decltype(std::declval<T>() == std::declval<T>() )>
std::true_type  supports_equal_test(const T&);
std::false_type supports_equal_test(...);

template<class T> struct supports_equal : decltype(supports_equal_test(std::declval<T>())){};
template<class T, bool> struct supports_equal_conditional : decltype(supports_equal_test(std::declval<T>())){};
template<class T> struct supports_equal_conditional<T,false> : std::false_type{};
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#ifdef QVECTOR_H
template<class T> struct supports_equal<QVector<T>> : supports_equal_conditional<QVector<int>,supports_equal<T>::value>{};
#endif
#ifdef QLINKEDLIST_H
template<class T> struct supports_equal<QLinkedList<T>> : supports_equal_conditional<QLinkedList<int>,supports_equal<T>::value>{};
#endif
template<class T1, class T2> struct supports_equal<QPair<T1,T2>> : supports_equal_conditional<QPair<int,int>, supports_equal<T1>::value && supports_equal<T2>::value>{};
#endif
template<class T> struct supports_equal<QList<T>> : supports_equal_conditional<QList<int>,supports_equal<T>::value>{};
template<class T> struct supports_equal<QQueue<T>> : supports_equal_conditional<QQueue<int>,supports_equal<T>::value>{};
template<class T> struct supports_equal<QStack<T>> : supports_equal_conditional<QStack<int>,supports_equal<T>::value>{};
template<class T> struct supports_equal<QSet<T>> : supports_equal_conditional<QSet<int>,supports_equal<T>::value>{};
template<class T1, class T2> struct supports_equal<std::pair<T1,T2>> : supports_equal_conditional<std::pair<int,int>, supports_equal<T1>::value && supports_equal<T2>::value>{};
template<class T1, class T2> struct supports_equal<QMap<T1,T2>> : supports_equal_conditional<QMap<int,int>, supports_equal<T1>::value && supports_equal<T2>::value>{};
template<class T1, class T2> struct supports_equal<QHash<T1,T2>> : supports_equal_conditional<QHash<int,int>, supports_equal<T1>::value && supports_equal<T2>::value>{};
template<class T1, class T2> struct supports_equal<QMultiMap<T1,T2>> : supports_equal_conditional<QMultiMap<int,int>, supports_equal<T1>::value && supports_equal<T2>::value>{};
template<class T1, class T2> struct supports_equal<QMultiHash<T1,T2>> : supports_equal_conditional<QMultiHash<int,int>, supports_equal<T1>::value && supports_equal<T2>::value>{};
template<typename T, typename Alloc> struct supports_equal<std::vector<T, Alloc>> : supports_equal_conditional<std::vector<int,Alloc>,supports_equal<T>::value>{};
template<typename T, typename Alloc> struct supports_equal<std::list<T, Alloc>> : supports_equal_conditional<std::list<int,Alloc>,supports_equal<T>::value>{};
template<class T1, class T2, typename Compare, typename Alloc> struct supports_equal<std::map<T1,T2,Compare,Alloc>> : supports_equal_conditional<std::map<int,int,Compare,Alloc>,supports_equal<T1>::value && supports_equal<T2>::value>{};
template<class T1, class T2, typename Compare, typename Alloc> struct supports_equal<std::multimap<T1,T2,Compare,Alloc>> : supports_equal_conditional<std::multimap<int,int,Compare,Alloc>,supports_equal<T1>::value && supports_equal<T2>::value>{};
template<> struct supports_equal<void> : std::false_type{};
template<class T> struct supports_equal<T*> : std::true_type{};

template<class T, class = decltype(std::declval<T>() < std::declval<T>() )>
std::true_type  supports_less_than_test(const T&);
std::false_type supports_less_than_test(...);

template<class T> struct supports_less_than : decltype(supports_less_than_test(std::declval<T>())){};
template<class T, bool> struct supports_less_than_conditional : decltype(supports_less_than_test(std::declval<T>())){};
template<class T> struct supports_less_than_conditional<T,false> : std::false_type{};
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#ifdef QVECTOR_H
template<class T> struct supports_less_than<QVector<T>> : supports_less_than_conditional<QVector<int>,supports_less_than<T>::value>{};
#endif
#ifdef QLINKEDLIST_H
template<class T> struct supports_less_than<QLinkedList<T>> : supports_less_than_conditional<QLinkedList<int>,supports_less_than<T>::value>{};
#endif
template<class T1, class T2> struct supports_less_than<QPair<T1,T2>> : supports_less_than_conditional<QPair<int,int>, supports_less_than<T1>::value && supports_less_than<T2>::value>{};
#endif
template<class T> struct supports_less_than<QList<T>> : supports_less_than_conditional<QList<int>,supports_less_than<T>::value>{};
template<class T> struct supports_less_than<QQueue<T>> : supports_less_than_conditional<QQueue<int>,supports_less_than<T>::value>{};
template<class T> struct supports_less_than<QStack<T>> : supports_less_than_conditional<QStack<int>,supports_less_than<T>::value>{};
template<class T> struct supports_less_than<QSet<T>> : supports_less_than_conditional<QSet<int>,supports_less_than<T>::value>{};
template<class T1, class T2> struct supports_less_than<std::pair<T1,T2>> : supports_less_than_conditional<std::pair<int,int>, supports_less_than<T1>::value && supports_less_than<T2>::value>{};
template<class T1, class T2> struct supports_less_than<QMap<T1,T2>> : supports_less_than_conditional<QMap<int,int>, supports_less_than<T1>::value && supports_less_than<T2>::value>{};
template<class T1, class T2> struct supports_less_than<QHash<T1,T2>> : supports_less_than_conditional<QHash<int,int>, supports_less_than<T1>::value && supports_less_than<T2>::value>{};
template<class T1, class T2> struct supports_less_than<QMultiMap<T1,T2>> : supports_less_than_conditional<QMultiMap<int,int>, supports_less_than<T1>::value && supports_less_than<T2>::value>{};
template<class T1, class T2> struct supports_less_than<QMultiHash<T1,T2>> : supports_less_than_conditional<QMultiHash<int,int>, supports_less_than<T1>::value && supports_less_than<T2>::value>{};
template<typename T, typename Alloc> struct supports_less_than<std::vector<T, Alloc>> : supports_less_than_conditional<std::vector<int,Alloc>,supports_less_than<T>::value>{};
template<typename T, typename Alloc> struct supports_less_than<std::list<T, Alloc>> : supports_less_than_conditional<std::list<int,Alloc>,supports_less_than<T>::value>{};
template<class T1, class T2, typename Compare, typename Alloc> struct supports_less_than<std::map<T1,T2,Compare,Alloc>> : supports_less_than_conditional<std::map<int,int,Compare,Alloc>,supports_less_than<T1>::value && supports_less_than<T2>::value>{};
template<class T1, class T2, typename Compare, typename Alloc> struct supports_less_than<std::multimap<T1,T2,Compare,Alloc>> : supports_less_than_conditional<std::multimap<int,int,Compare,Alloc>,supports_less_than<T1>::value && supports_less_than<T2>::value>{};
template<> struct supports_less_than<void> : std::false_type{};
template<class T> struct supports_less_than<T*> : std::true_type{};

template<class T, class = decltype(qHash(std::declval<T>()))>
std::true_type  supports_qHash_test(const T&);
std::false_type supports_qHash_test(...);

template<class T> struct supports_qHash : decltype(supports_qHash_test(std::declval<T>())){};
template<class T, bool> struct supports_qHash_conditional : decltype(supports_qHash_test(std::declval<T>())){};
template<class T> struct supports_qHash_conditional<T,false> : std::false_type{};
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#ifdef QVECTOR_H
template<class T> struct supports_qHash<QVector<T>> : supports_qHash_conditional<QVector<int>,supports_qHash<T>::value>{};
#endif
#ifdef QLINKEDLIST_H
template<class T> struct supports_qHash<QLinkedList<T>> : supports_qHash_conditional<QLinkedList<int>,supports_qHash<T>::value>{};
#endif
template<class T1, class T2> struct supports_qHash<QPair<T1,T2>> : supports_qHash_conditional<QPair<int,int>, supports_qHash<T1>::value && supports_qHash<T2>::value>{};
#endif
template<class T> struct supports_qHash<QList<T>> : supports_qHash_conditional<QList<int>,supports_qHash<T>::value>{};
template<class T> struct supports_qHash<QQueue<T>> : supports_qHash_conditional<QQueue<int>,supports_qHash<T>::value>{};
template<class T> struct supports_qHash<QStack<T>> : supports_qHash_conditional<QStack<int>,supports_qHash<T>::value>{};
template<class T> struct supports_qHash<QSet<T>> : supports_qHash_conditional<QSet<int>,supports_qHash<T>::value>{};
template<class T1, class T2> struct supports_qHash<std::pair<T1,T2>> : supports_qHash_conditional<std::pair<int,int>, supports_qHash<T1>::value && supports_qHash<T2>::value>{};
template<class T1, class T2> struct supports_qHash<QMap<T1,T2>> : supports_qHash_conditional<QMap<int,int>, supports_qHash<T1>::value && supports_qHash<T2>::value>{};
template<class T1, class T2> struct supports_qHash<QHash<T1,T2>> : supports_qHash_conditional<QHash<int,int>, supports_qHash<T1>::value && supports_qHash<T2>::value>{};
template<class T1, class T2> struct supports_qHash<QMultiMap<T1,T2>> : supports_qHash_conditional<QMultiMap<int,int>, supports_qHash<T1>::value && supports_qHash<T2>::value>{};
template<class T1, class T2> struct supports_qHash<QMultiHash<T1,T2>> : supports_qHash_conditional<QMultiHash<int,int>, supports_qHash<T1>::value && supports_qHash<T2>::value>{};
template<typename T, typename Alloc> struct supports_qHash<std::vector<T, Alloc>> : supports_qHash_conditional<std::vector<int,Alloc>,supports_qHash<T>::value>{};
template<typename T, typename Alloc> struct supports_qHash<std::list<T, Alloc>> : supports_qHash_conditional<std::list<int,Alloc>,supports_qHash<T>::value>{};
template<class T1, class T2, typename Compare, typename Alloc> struct supports_qHash<std::map<T1,T2,Compare,Alloc>> : supports_qHash_conditional<std::map<int,int,Compare,Alloc>,supports_qHash<T1>::value && supports_qHash<T2>::value>{};
template<class T1, class T2, typename Compare, typename Alloc> struct supports_qHash<std::multimap<T1,T2,Compare,Alloc>> : supports_qHash_conditional<std::multimap<int,int,Compare,Alloc>,supports_qHash<T1>::value && supports_qHash<T2>::value>{};
template<> struct supports_qHash<void> : std::false_type{};
template<class T> struct supports_qHash<T*> : std::true_type{};

template<typename T, class = decltype( operator << (std::declval<QDebug>(), std::declval<T>()) )>
std::true_type  supports_debugstream_test(const T&);
std::false_type supports_debugstream_test(...);

template<typename T> struct supports_debugstream : decltype(supports_debugstream_test(std::declval<T>())){};
template<class T, bool> struct supports_debugstream_conditional : decltype(supports_debugstream_test(std::declval<T>())){};
template<class T> struct supports_debugstream_conditional<T,false> : std::false_type{};
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#ifdef QVECTOR_H
template<class T> struct supports_debugstream<QVector<T>> : supports_debugstream_conditional<QVector<int>,supports_debugstream<T>::value>{};
#endif
#ifdef QLINKEDLIST_H
template<class T> struct supports_debugstream<QLinkedList<T>> : supports_debugstream_conditional<QLinkedList<int>,supports_debugstream<T>::value>{};
#endif
template<class T1, class T2> struct supports_debugstream<QPair<T1,T2>> : supports_debugstream_conditional<QPair<int,int>, supports_debugstream<T1>::value && supports_debugstream<T2>::value>{};
#endif
template<class T> struct supports_debugstream<QList<T>> : supports_debugstream_conditional<QList<int>,supports_debugstream<T>::value>{};
template<class T> struct supports_debugstream<QQueue<T>> : supports_debugstream_conditional<QQueue<int>,supports_debugstream<T>::value>{};
template<class T> struct supports_debugstream<QStack<T>> : supports_debugstream_conditional<QStack<int>,supports_debugstream<T>::value>{};
template<class T> struct supports_debugstream<QSet<T>> : supports_debugstream_conditional<QSet<int>,supports_debugstream<T>::value>{};
template<class T1, class T2> struct supports_debugstream<std::pair<T1,T2>> : supports_debugstream_conditional<std::pair<int,int>, supports_debugstream<T1>::value && supports_debugstream<T2>::value>{};
template<class T1, class T2> struct supports_debugstream<QMap<T1,T2>> : supports_debugstream_conditional<QMap<int,int>, supports_debugstream<T1>::value && supports_debugstream<T2>::value>{};
template<class T1, class T2> struct supports_debugstream<QHash<T1,T2>> : supports_debugstream_conditional<QHash<int,int>, supports_debugstream<T1>::value && supports_debugstream<T2>::value>{};
template<class T1, class T2> struct supports_debugstream<QMultiMap<T1,T2>> : supports_debugstream_conditional<QMultiMap<int,int>, supports_debugstream<T1>::value && supports_debugstream<T2>::value>{};
template<class T1, class T2> struct supports_debugstream<QMultiHash<T1,T2>> : supports_debugstream_conditional<QMultiHash<int,int>, supports_debugstream<T1>::value && supports_debugstream<T2>::value>{};
template<typename T, typename Alloc> struct supports_debugstream<std::vector<T, Alloc>> : supports_debugstream_conditional<std::vector<int,Alloc>,supports_debugstream<T>::value>{};
template<typename T, typename Alloc> struct supports_debugstream<std::list<T, Alloc>> : supports_debugstream_conditional<std::list<int,Alloc>,supports_debugstream<T>::value>{};
template<class T1, class T2, typename Compare, typename Alloc> struct supports_debugstream<std::map<T1,T2,Compare,Alloc>> : supports_debugstream_conditional<std::map<int,int,Compare,Alloc>,supports_debugstream<T1>::value && supports_debugstream<T2>::value>{};
template<class T1, class T2, typename Compare, typename Alloc> struct supports_debugstream<std::multimap<T1,T2,Compare,Alloc>> : supports_debugstream_conditional<std::multimap<int,int,Compare,Alloc>,supports_debugstream<T1>::value && supports_debugstream<T2>::value>{};
template<> struct supports_debugstream<void> : std::false_type{};

template<typename T, class = decltype( std::declval<QDataStream&>() << std::declval<T>() ), class = decltype( std::declval<QDataStream&>() >> std::declval<T&>() )>
std::true_type  supports_stream_operators_test(T&&);
std::false_type supports_stream_operators_test(...);

template<typename T> struct supports_stream_operators : decltype(supports_stream_operators_test(std::declval<T>())){};
template<class T, bool> struct supports_stream_operators_conditional : decltype(supports_stream_operators_test(std::declval<T>())){};
template<class T> struct supports_stream_operators_conditional<T,false> : std::false_type{};
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#ifdef QVECTOR_H
template<class T> struct supports_stream_operators<QVector<T>> : supports_stream_operators_conditional<QVector<int>,supports_stream_operators<T>::value>{};
#endif
#ifdef QLINKEDLIST_H
template<class T> struct supports_stream_operators<QLinkedList<T>> : supports_stream_operators_conditional<QLinkedList<int>,supports_stream_operators<T>::value>{};
#endif
template<class T1, class T2> struct supports_stream_operators<QPair<T1,T2>> : supports_stream_operators_conditional<QPair<int,int>, supports_stream_operators<T1>::value && supports_stream_operators<T2>::value>{};
#endif
template<class T> struct supports_stream_operators<QList<T>> : supports_stream_operators_conditional<QList<int>,supports_stream_operators<T>::value>{};
template<class T> struct supports_stream_operators<QQueue<T>> : supports_stream_operators_conditional<QQueue<int>,supports_stream_operators<T>::value>{};
template<class T> struct supports_stream_operators<QStack<T>> : supports_stream_operators_conditional<QStack<int>,supports_stream_operators<T>::value>{};
template<class T> struct supports_stream_operators<QSet<T>> : supports_stream_operators_conditional<QSet<int>,supports_stream_operators<T>::value>{};
template<class T1, class T2> struct supports_stream_operators<std::pair<T1,T2>> : supports_stream_operators_conditional<std::pair<int,int>, supports_stream_operators<T1>::value && supports_stream_operators<T2>::value>{};
template<class T1, class T2> struct supports_stream_operators<QMap<T1,T2>> : supports_stream_operators_conditional<QMap<int,int>, supports_stream_operators<T1>::value && supports_stream_operators<T2>::value>{};
template<class T1, class T2> struct supports_stream_operators<QHash<T1,T2>> : supports_stream_operators_conditional<QHash<int,int>, supports_stream_operators<T1>::value && supports_stream_operators<T2>::value>{};
template<class T1, class T2> struct supports_stream_operators<QMultiMap<T1,T2>> : supports_stream_operators_conditional<QMultiMap<int,int>, supports_stream_operators<T1>::value && supports_stream_operators<T2>::value>{};
template<class T1, class T2> struct supports_stream_operators<QMultiHash<T1,T2>> : supports_stream_operators_conditional<QMultiHash<int,int>, supports_stream_operators<T1>::value && supports_stream_operators<T2>::value>{};
template<typename T, typename Alloc> struct supports_stream_operators<std::vector<T, Alloc>> : supports_stream_operators_conditional<std::vector<int,Alloc>,supports_stream_operators<T>::value>{};
template<typename T, typename Alloc> struct supports_stream_operators<std::list<T, Alloc>> : supports_stream_operators_conditional<std::list<int,Alloc>,supports_stream_operators<T>::value>{};
template<class T1, class T2, typename Compare, typename Alloc> struct supports_stream_operators<std::map<T1,T2,Compare,Alloc>> : supports_stream_operators_conditional<std::map<int,int,Compare,Alloc>,supports_stream_operators<T1>::value && supports_stream_operators<T2>::value>{};
template<class T1, class T2, typename Compare, typename Alloc> struct supports_stream_operators<std::multimap<T1,T2,Compare,Alloc>> : supports_stream_operators_conditional<std::multimap<int,int,Compare,Alloc>,supports_stream_operators<T1>::value && supports_stream_operators<T2>::value>{};
template<> struct supports_stream_operators<void> : std::false_type{};

template<class T, class = decltype(T())>
std::true_type  supports_StandardConstructor_test(T&&);
std::false_type supports_StandardConstructor_test(...);

template<class T> struct supports_StandardConstructor : decltype(supports_StandardConstructor_test(std::declval<T>())){};
template<> struct supports_StandardConstructor<void> : std::false_type{};

template<class T, class = decltype(T(std::declval<T>()))>
std::true_type  supports_CopyConstructor_test(T&&);
std::false_type supports_CopyConstructor_test(...);

template<class T> struct supports_CopyConstructor : decltype(supports_CopyConstructor_test(std::declval<T>())){};
template<> struct supports_CopyConstructor<void> : std::false_type{};

template<class T, class = decltype(~T())>
std::true_type  supports_Destructor_test(T&&);
std::false_type supports_Destructor_test(...);

template<class T> struct supports_Destructor : decltype(supports_Destructor_test(std::declval<T>())){};
template<> struct supports_Destructor<void> : std::false_type{};

template<class T, class = decltype( ++std::declval<T>() )>
std::true_type  supports_increment_test(const T&);
std::false_type supports_increment_test(...);

template<class T> struct supports_increment : decltype(supports_increment_test(std::declval<T>())){};
template<> struct supports_increment<void> : std::false_type{};

std::true_type is_bidirectional_iterator_test(const std::bidirectional_iterator_tag&);
std::false_type is_bidirectional_iterator_test(...);
template<class T> struct is_bidirectional_iterator : decltype(is_bidirectional_iterator_test(std::declval<typename std::iterator_traits<T>::iterator_category>())){};
template<> struct is_bidirectional_iterator<void> : std::false_type{};

template<class T, class = decltype( --std::declval<T>() )>
std::true_type supports_decrement_test(const T&);
std::false_type supports_decrement_test(...);

template<class T> struct supports_decrement : decltype(supports_decrement_test(std::declval<T>())){};
template<> struct supports_decrement<void> : std::false_type{};

template<typename T, class = decltype(std::declval<T>().firstKey() )>
std::true_type  supports_firstKey_test(const T&);
std::false_type supports_firstKey_test(...);
template<typename T> struct supports_firstKey : decltype(supports_firstKey_test(std::declval<T>())){};

template<typename T, class = decltype(std::declval<T>().lastKey() )>
std::true_type  supports_lastKey_test(const T&);
std::false_type supports_lastKey_test(...);
template<typename T> struct supports_lastKey : decltype(supports_lastKey_test(std::declval<T>())){};

template<template<typename K, typename T> class Container, typename K, typename T, class = decltype(std::declval<Container<K,T>>().key(std::declval<T>()) )>
std::true_type  supports_map_key_by_value_test(const Container<T,K>&,const K&,const T&);
std::false_type supports_map_key_by_value_test(...);
template<template<typename K, typename T> class Container, typename K, typename T> struct supports_map_key_by_value : decltype(supports_map_key_by_value_test(std::declval<Container<K,T>>(),std::declval<K>(),std::declval<T>())){};

template<template<typename K, typename T> class Container, typename K, typename T, class = decltype(std::declval<Container<K,T>>().keys(std::declval<T>()) )>
std::true_type  supports_map_keys_by_value_test(const Container<T,K>&,const K&,const T&);
std::false_type supports_map_keys_by_value_test(...);
template<template<typename K, typename T> class Container, typename K, typename T> struct supports_map_keys_by_value : decltype(supports_map_keys_by_value_test(std::declval<Container<K,T>>(),std::declval<K>(),std::declval<T>())){};

template<template<typename K, typename T> class Container, typename K, typename T, class = decltype(std::declval<Container<K,T>>().value(std::declval<K>(),std::declval<T>()) )>
std::true_type  supports_map_value_test(const Container<T,K>&,const K&,const T&);
std::false_type supports_map_value_test(...);
template<template<typename K, typename T> class Container, typename K, typename T> struct supports_map_value : decltype(supports_map_value_test(std::declval<Container<K,T>>(),std::declval<K>(),std::declval<T>())){};

template<typename T, class = decltype(std::declval<T>().keys() )>
std::true_type  supports_keys_test(const T&);
std::false_type supports_keys_test(...);
template<typename T> struct supports_keys : decltype(supports_keys_test(std::declval<T>())){};

template<typename T, class = decltype(std::declval<T>().values() )>
std::true_type  supports_values_test(const T&);
std::false_type supports_values_test(...);
template<typename T> struct supports_values : decltype(supports_values_test(std::declval<T>())){};

template<template<typename K, typename T> class Container, typename K, typename T, class = decltype(std::declval<Container<K,T>>().values(std::declval<K>()) )>
std::true_type  supports_map_values_by_key_test(const Container<T,K>&,const K&,const T&);
std::false_type supports_map_values_by_key_test(...);
template<template<typename K, typename T> class Container, typename K, typename T> struct supports_map_values_by_key : decltype(supports_map_values_by_key_test(std::declval<Container<K,T>>(),std::declval<K>(),std::declval<T>())){};

template<template<typename K, typename T> class Container, typename K, typename T, class = decltype(std::declval<Container<K,T>>().insert(std::declval<K>(),std::declval<T>()) )>
std::true_type  supports_map_insert_test(const Container<T,K>&,const K&,const T&);
std::false_type supports_map_insert_test(...);
template<template<typename K, typename T> class Container, typename K, typename T> struct supports_map_insert : decltype(supports_map_insert_test(std::declval<Container<K,T>>(),std::declval<K>(),std::declval<T>())){};

template<template<typename K, typename T> class Container, typename K, typename T, class = decltype(std::declval<Container<K,T>>().remove(std::declval<K>()) )>
std::true_type  supports_map_remove_test(const Container<T,K>&,const K&,const T&);
std::false_type supports_map_remove_test(...);
template<template<typename K, typename T> class Container, typename K, typename T> struct supports_map_remove : decltype(supports_map_remove_test(std::declval<Container<K,T>>(),std::declval<K>(),std::declval<T>())){};

template<template<typename K, typename T> class Container, typename K, typename T, class = decltype(std::declval<Container<K,T>>().take(std::declval<K>()) )>
std::true_type  supports_map_take_test(const Container<T,K>&,const K&,const T&);
std::false_type supports_map_take_test(...);
template<template<typename K, typename T> class Container, typename K, typename T> struct supports_map_take : decltype(supports_map_take_test(std::declval<Container<K,T>>(),std::declval<K>(),std::declval<T>())){};

template<template<typename K, typename T> class Container, typename K, typename T, class = decltype(std::declval<Container<K,T>>().lowerBound(std::declval<T>()) )>
std::true_type  supports_map_lower_bound_test(const Container<T,K>&,const K&,const T&);
std::false_type supports_map_lower_bound_test(...);
template<template<typename K, typename T> class Container, typename K, typename T> struct supports_map_lower_bound : decltype(supports_map_lower_bound_test(std::declval<Container<K,T>>(),std::declval<K>(),std::declval<T>())){};

template<template<typename K, typename T> class Container, typename K, typename T, class = decltype(std::declval<Container<K,T>>().upperBound(std::declval<T>()) )>
std::true_type  supports_map_upper_bound_test(const Container<T,K>&,const K&,const T&);
std::false_type supports_map_upper_bound_test(...);
template<template<typename K, typename T> class Container, typename K, typename T> struct supports_map_upper_bound : decltype(supports_map_upper_bound_test(std::declval<Container<K,T>>(),std::declval<K>(),std::declval<T>())){};

template<typename T, class = decltype(std::declval<T>().size() )>
std::true_type  supports_size_test(const T&);
std::false_type supports_size_test(...);
template<typename T> struct supports_size : decltype(supports_size_test(std::declval<T>())){};

template<typename T, class = decltype(std::declval<T>().at(std::declval<int>()) )>
std::true_type  supports_at_test(const T&);
std::false_type supports_at_test(...);
template<typename T> struct supports_at : decltype(supports_at_test(std::declval<T>())){};

template<typename T, class = decltype(std::declval<T>().takeAt(std::declval<int>()) )>
std::true_type  supports_take_at_test(const T&);
std::false_type supports_take_at_test(...);
template<typename T> struct supports_take_at : decltype(supports_take_at_test(std::declval<T>())){};

template<typename T, class = decltype(std::declval<T>().remove(std::declval<int>()) )>
std::true_type  supports_remove_index_test(const T&);
std::false_type supports_remove_index_test(...);
template<typename T> struct supports_remove_index : decltype(supports_remove_index_test(std::declval<T>())){};

template<typename T, class = decltype(std::declval<T>().remove(std::declval<int>(),std::declval<int>()) )>
std::true_type  supports_remove_index_N_test(const T&);
std::false_type supports_remove_index_N_test(...);
template<typename T> struct supports_remove_index_N : decltype(supports_remove_index_N_test(std::declval<T>())){};

template<typename T, class = decltype(std::declval<T>().value(std::declval<int>()) )>
std::true_type  supports_value_at_test(const T&);
std::false_type supports_value_at_test(...);
template<typename T> struct supports_value_at : decltype(supports_value_at_test(std::declval<T>())){};

template<typename T, typename C, class = decltype(std::declval<T>().value(std::declval<int>(),std::declval<C>()) )>
std::true_type  supports_value_with_default_test(const T&,const C&);
std::false_type supports_value_with_default_test(...);
template<typename T, typename C> struct supports_value_with_default : decltype(supports_value_with_default_test(std::declval<T>(),std::declval<C>())){};

template<typename T, typename C, class = decltype(std::declval<T>().append(std::declval<C>()) )>
std::true_type  supports_append_test(const T&,const C&);
std::false_type supports_append_test(...);
template<typename T, typename C> struct supports_append : decltype(supports_append_test(std::declval<T>(),std::declval<C>())){};

template<typename T, typename C, class = decltype(std::declval<T>().replace(std::declval<int>(),std::declval<C>()) )>
std::true_type  supports_replace_test(const T&,const C&);
std::false_type supports_replace_test(...);
template<typename T, typename C> struct supports_replace : decltype(supports_replace_test(std::declval<T>(),std::declval<C>())){};

template<typename T, typename C, class = decltype(std::declval<T>().prepend(std::declval<C>()) )>
std::true_type  supports_prepend_test(const T&,const C&);
std::false_type supports_prepend_test(...);
template<typename T, typename C> struct supports_prepend : decltype(supports_prepend_test(std::declval<T>(),std::declval<C>())){};

template<typename T, typename C, class = decltype(std::declval<T>().removeOne(std::declval<C>()) )>
std::true_type  supports_removeOne_test(const T&,const C&);
std::false_type supports_removeOne_test(...);
template<typename T, typename C> struct supports_removeOne : decltype(supports_removeOne_test(std::declval<T>(),std::declval<C>())){};

template<typename T, typename C, class = decltype(std::declval<T>().remove(std::declval<C>()) )>
std::true_type  supports_removeElement_test(const T&,const C&);
std::false_type supports_removeElement_test(...);
template<typename T, typename C> struct supports_removeElement : decltype(supports_removeElement_test(std::declval<T>(),std::declval<C>())){};

template<typename T, typename C, class = decltype(std::declval<T>().removeAll(std::declval<C>()) )>
std::true_type  supports_removeAll_test(const T&,const C&);
std::false_type supports_removeAll_test(...);
template<typename T, typename C> struct supports_removeAll : decltype(supports_removeAll_test(std::declval<T>(),std::declval<C>())){};

template<typename T, typename C, class = decltype(std::declval<T>().insert(std::declval<int>(),std::declval<C>()) )>
std::true_type  supports_insert_test(const T&,const C&);
std::false_type supports_insert_test(...);
template<typename T, typename C> struct supports_insert : decltype(supports_insert_test(std::declval<T>(),std::declval<C>())){};

template<typename T, typename C, class = decltype(std::declval<T>().insert(std::declval<int>(),std::declval<int>(),std::declval<C>()) )>
std::true_type  supports_insertN_test(const T&,const C&);
std::false_type supports_insertN_test(...);
template<typename T, typename C> struct supports_insertN : decltype(supports_insertN_test(std::declval<T>(),std::declval<C>())){};

template<typename T, typename C, class = decltype(std::declval<T>().fill(std::declval<C>(),std::declval<int>()) )>
std::true_type  supports_fill_test(const T&,const C&);
std::false_type supports_fill_test(...);
template<typename T, typename C> struct supports_fill : decltype(supports_fill_test(std::declval<T>(),std::declval<C>())){};

template<typename T, class = decltype(std::declval<T>().mid(std::declval<int>(),std::declval<int>()) )>
std::true_type  supports_mid_test(const T&);
std::false_type supports_mid_test(...);
template<typename T> struct supports_mid : decltype(supports_mid_test(std::declval<T>())){};

template<typename T, class = decltype(std::declval<T>().squeeze() )>
std::true_type  supports_squeeze_test(const T&);
std::false_type supports_squeeze_test(...);
template<typename T> struct supports_squeeze : decltype(supports_squeeze_test(std::declval<T>())){};

template<typename T, class = decltype(std::declval<T>().clear() )>
std::true_type  supports_clear_test(const T&);
std::false_type supports_clear_test(...);
template<typename T> struct supports_clear : decltype(supports_clear_test(std::declval<T>())){};

template<typename T, class = decltype(std::declval<T>().begin() )>
std::true_type  supports_begin_test(const T&);
std::false_type supports_begin_test(...);
template<typename T> struct supports_begin : decltype(supports_begin_test(std::declval<T>())){};

template<typename T, class = decltype(std::declval<T>().first() )>
std::true_type  supports_first_test(const T&);
std::false_type supports_first_test(...);
template<typename T> struct supports_first : decltype(supports_first_test(std::declval<T>())){};

template<typename T, class = decltype(std::declval<T>().last() )>
std::true_type  supports_last_test(const T&);
std::false_type supports_last_test(...);
template<typename T> struct supports_last : decltype(supports_last_test(std::declval<T>())){};

template<typename T, class = decltype(std::declval<T>().unite(std::declval<T>()) )>
std::true_type  supports_unite_test(const T&);
std::false_type supports_unite_test(...);
template<typename T> struct supports_unite : decltype(supports_unite_test(std::declval<T>())){};

template<typename T, class = decltype(std::declval<T>().move(std::declval<int>(),std::declval<int>()) )>
std::true_type  supports_move_test(const T&);
std::false_type supports_move_test(...);
template<typename T> struct supports_move : decltype(supports_move_test(std::declval<T>())){};

template<typename T, class = decltype(std::declval<T>().capacity() )>
std::true_type  supports_capacity_test(const T&);
std::false_type supports_capacity_test(...);
template<typename T> struct supports_capacity : decltype(supports_capacity_test(std::declval<T>())){};

template<typename T, typename C, class = decltype(std::declval<T>().contains(std::declval<C>()) )>
std::true_type  supports_contains_test(const T&, const C&);
std::false_type supports_contains_test(...);
template<typename T, typename C> struct supports_contains : decltype(supports_contains_test(std::declval<T>(), std::declval<C>())){};

template<typename T, typename C, typename C2, class = decltype(std::declval<T>().contains(std::declval<C>(), std::declval<C2>()) )>
std::true_type  supports_contains_key_value_test(const T&, const C&, const C2&);
std::false_type supports_contains_key_value_test(...);
template<typename T, typename C, typename C2> struct supports_contains_key_value : decltype(supports_contains_key_value_test(std::declval<T>(), std::declval<C>(), std::declval<C2>())){};

template<typename T, typename C, typename C2, class = decltype(std::declval<T>().find(std::declval<C>(), std::declval<C2>()) )>
std::true_type  supports_find_key_value_test(const T&, const C&, const C2&);
std::false_type supports_find_key_value_test(...);
template<typename T, typename C, typename C2> struct supports_find_key_value : decltype(supports_find_key_value_test(std::declval<T>(), std::declval<C>(), std::declval<C2>())){};

template<typename T, typename C, typename C2, class = decltype(std::declval<T>().remove(std::declval<C>(), std::declval<C2>()) )>
std::true_type  supports_remove_key_value_test(const T&, const C&, const C2&);
std::false_type supports_remove_key_value_test(...);
template<typename T, typename C, typename C2> struct supports_remove_key_value : decltype(supports_remove_key_value_test(std::declval<T>(), std::declval<C>(), std::declval<C2>())){};

template<typename T, typename C, typename C2, class = decltype(std::declval<T>().replace(std::declval<C>(), std::declval<C2>()) )>
std::true_type  supports_replace_key_value_test(const T&, const C&, const C2&);
std::false_type supports_replace_key_value_test(...);
template<typename T, typename C, typename C2> struct supports_replace_key_value : decltype(supports_replace_key_value_test(std::declval<T>(), std::declval<C>(), std::declval<C2>())){};

template<typename T, typename C, typename C2, class = decltype(std::declval<T>().count(std::declval<C>(), std::declval<C2>()) )>
std::true_type  supports_count_key_value_test(const T&, const C&, const C2&);
std::false_type supports_count_key_value_test(...);
template<typename T, typename C, typename C2> struct supports_count_key_value : decltype(supports_count_key_value_test(std::declval<T>(), std::declval<C>(), std::declval<C2>())){};

template<typename T, typename C, class = decltype(std::declval<T>().count(std::declval<C>()) )>
std::true_type  supports_count_elements_test(const T&, const C&);
std::false_type supports_count_elements_test(...);
template<typename T, typename C> struct supports_count_elements : decltype(supports_count_elements_test(std::declval<T>(), std::declval<C>())){};

template<typename T, typename C, class = decltype(std::declval<T>().endsWith(std::declval<C>()) )>
std::true_type  supports_endsWith_test(const T&, const C&);
std::false_type supports_endsWith_test(...);
template<typename T, typename C> struct supports_endsWith : decltype(supports_endsWith_test(std::declval<T>(), std::declval<C>())){};

template<typename T, typename C, class = decltype(std::declval<T>().startsWith(std::declval<C>()) )>
std::true_type  supports_startsWith_test(const T&, const C&);
std::false_type supports_startsWith_test(...);
template<typename T, typename C> struct supports_startsWith : decltype(supports_startsWith_test(std::declval<T>(), std::declval<C>())){};

template<typename T, typename C, class = decltype(std::declval<T>().indexOf(std::declval<C>()) )>
std::true_type  supports_indexOf_test(const T&, const C&);
std::false_type supports_indexOf_test(...);
template<typename T, typename C> struct supports_indexOf : decltype(supports_indexOf_test(std::declval<T>(), std::declval<C>())){};

template<typename T, typename C, class = decltype(std::declval<T>().lastIndexOf(std::declval<C>()) )>
std::true_type  supports_lastIndexOf_test(const T&, const C&);
std::false_type supports_lastIndexOf_test(...);
template<typename T, typename C> struct supports_lastIndexOf : decltype(supports_lastIndexOf_test(std::declval<T>(), std::declval<C>())){};

template<typename T, class = decltype(std::declval<T>().resize(std::declval<int>()) )>
std::true_type  supports_resize_test(const T&);
std::false_type supports_resize_test(...);
template<typename T> struct supports_resize : decltype(supports_resize_test(std::declval<T>())){};

template<typename T, class = decltype(std::declval<T>().reserve(std::declval<int>()) )>
std::true_type  supports_reserve_test(const T&);
std::false_type supports_reserve_test(...);
template<typename T> struct supports_reserve : decltype(supports_reserve_test(std::declval<T>())){};

template<typename T, class = decltype(std::declval<T>().end() )>
std::true_type  supports_end_test(const T&);
std::false_type supports_end_test(...);
template<typename T> struct supports_end : decltype(supports_end_test(std::declval<T>())){};

template<typename T, class = decltype(std::declval<T>().uniqueKeys() )>
std::true_type  supports_uniqueKeys_test(const T&);
std::false_type supports_uniqueKeys_test(...);
template<typename T> struct supports_uniqueKeys : decltype(supports_uniqueKeys_test(std::declval<T>())){};

template<template<typename K, typename T> class Container, typename K, typename T, class = decltype(std::declval<Container<K,T>>().find(std::declval<K>(),std::declval<T>()) )>
std::true_type  supports_map_find_key_value_test(const Container<T,K>&);
std::false_type supports_map_find_key_value_test(...);
template<template<typename K, typename T> class Container, typename K, typename T> struct supports_map_find_key_value : decltype(supports_map_find_key_value_test(std::declval<Container<K,T>>())){};

template<typename T, class = decltype(std::declval<T>().intersects(std::declval<T>()) )>
std::true_type  supports_intersects_test(const T&);
std::false_type supports_intersects_test(...);
template<typename T> struct supports_intersects : decltype(supports_intersects_test(std::declval<T>())){};

template<typename T, class = decltype(std::declval<T>().intersect(std::declval<T>()) )>
std::true_type  supports_intersect_test(const T&);
std::false_type supports_intersect_test(...);
template<typename T> struct supports_intersect : decltype(supports_intersect_test(std::declval<T>())){};

template<template<typename K, typename T> class Container, typename K, typename T>
struct supports_map_sort : supports_less_than<K>{};

template<typename K, typename T>
struct supports_map_sort<QHash,K,T> : supports_qHash<K>{};

template<typename K, typename T>
struct supports_map_sort<QMultiHash,K,T> : supports_qHash<K>{};

template<typename T>
struct qtjambi_type;

template<template<typename T> class Container, typename T>
struct qtjambi_type_container1{
    using type = Container<T>;
};

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#ifdef QLINKEDLIST_H
template<typename T>
struct qtjambi_type_container1<QLinkedList,T>{
    using type = QLinkedList<QVariant>;
};
#endif

#ifdef QVECTOR_H
template<typename T>
struct qtjambi_type_container1<QVector,T>{
    using type = QVector<QVariant>;
};
#endif
#else
#ifdef QPROPERTY_H
template<template<typename T> class Property, typename T, int size = sizeof(T), bool isInteger = std::is_integral<T>::value, bool isFloatingPoint = std::is_floating_point<T>::value>
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
struct qtjambi_type_container1<QPropertyAlias,T>
        : qtjambi_type_property_decider<QPropertyAlias,T>{
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
#endif
#endif

#ifdef QLIST_H
template<typename T>
struct qtjambi_type_container1<QList,T>{
    using type = QList<QVariant>;
};
#endif

#ifdef QSET_H
template<typename T>
struct qtjambi_type_container1<QSet,T>{
    using type = QSet<QVariant>;
};
#endif

#if defined(QFUTUREINTERFACE_H)
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
#endif

#if defined(QFUTUREWATCHER_H)
template<typename T>
struct qtjambi_type_container1<QFutureWatcher,T>{
    using type = QFutureWatcher<QVariant>;
};
template<>
struct qtjambi_type_container1<QFutureWatcher,void>{
    using type = QFutureWatcher<void>;
};
#endif

#ifdef QQUEUE_H
template<typename T>
struct qtjambi_type_container1<QQueue,T>{
    using type = QQueue<QVariant>;
};
#endif

#ifdef QSTACK_H
template<typename T>
struct qtjambi_type_container1<QStack,T>{
    using type = QStack<QVariant>;
};
#endif

#ifdef QDBUSREPLY_H
template<typename T>
struct qtjambi_type_container1<QDBusReply,T>{
    using type = QDBusReply<QVariant>;
};
#endif

#ifdef QQMLLISTPROPERTY
template<typename T>
struct qtjambi_type_container1<QQmlListProperty,T>{
    using type = QQmlListProperty<QObject>;
};
#endif

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

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
template<typename K, typename T>
struct qtjambi_type_container2<QPair,K,T>{
    using type = QPair<QVariant,QVariant>;
};
#endif

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
    using type = typename qtjambi_type_selector<typename std::conditional<std::is_function<typename std::remove_pointer<T>::type>::value, T, typename std::remove_pointer<T>::type>::type>::type;
    static constexpr const std::type_info& id() {return typeid(type);}
};

template<typename O, bool = std::is_pointer<O>::value>
struct pointer_from{
    typedef typename std::add_lvalue_reference<typename std::add_const<O>::type>::type In;
    typedef typename std::add_pointer<O>::type Out;
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

}

#endif // QTJAMBI_TYPETESTS_H
