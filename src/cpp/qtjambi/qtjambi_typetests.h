/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
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

namespace QtJambiPrivate {

template<class T, class = decltype(std::declval<T>() == std::declval<T>() )>
std::true_type  supports_equal_test(const T&);
std::false_type supports_equal_test(...);

template<class T> struct supports_equal : decltype(supports_equal_test(std::declval<T>())){};
template<> struct supports_equal<void> : std::false_type{};

template<class T, class = decltype(std::declval<T>() < std::declval<T>() )>
std::true_type  supports_less_than_test(const T&);
std::false_type supports_less_than_test(...);

template<class T> struct supports_less_than : decltype(supports_less_than_test(std::declval<T>())){};
template<> struct supports_less_than<void> : std::false_type{};

template<class T, class = decltype(qHash(std::declval<T>()))>
std::true_type  supports_qHash_test(T&&);
std::false_type supports_qHash_test(...);

template<class T1, class T2, class = decltype(qHash(std::declval<T1>())), class = decltype(qHash(std::declval<T2>()))>
std::true_type  supports_qHash_test2(T1&&,T2&&);
std::false_type supports_qHash_test2(...);

template<class T> struct supports_qHash : decltype(supports_qHash_test(std::declval<T>())){};
template<class T> struct supports_qHash<QList<T>> : decltype(supports_qHash_test(std::declval<T>())){};
template<class T> struct supports_qHash<QQueue<T>> : decltype(supports_qHash_test(std::declval<T>())){};
template<class T> struct supports_qHash<QVector<T>> : decltype(supports_qHash_test(std::declval<T>())){};
template<class T> struct supports_qHash<QStack<T>> : decltype(supports_qHash_test(std::declval<T>())){};
template<class T> struct supports_qHash<QSet<T>> : decltype(supports_qHash_test(std::declval<T>())){};
template<class T1, class T2> struct supports_qHash<QPair<T1,T2>> : decltype(supports_qHash_test2(std::declval<T1>(), std::declval<T2>())){};
template<class T1, class T2> struct supports_qHash<std::pair<T1,T2>> : decltype(supports_qHash_test2(std::declval<T1>(), std::declval<T2>())){};
template<class T1, class T2> struct supports_qHash<QMap<T1,T2>> : decltype(supports_qHash_test2(std::declval<T1>(), std::declval<T2>())){};
template<class T1, class T2> struct supports_qHash<QHash<T1,T2>> : decltype(supports_qHash_test2(std::declval<T1>(), std::declval<T2>())){};
template<class T1, class T2> struct supports_qHash<QMultiMap<T1,T2>> : decltype(supports_qHash_test2(std::declval<T1>(), std::declval<T2>())){};
template<class T1, class T2> struct supports_qHash<QMultiHash<T1,T2>> : decltype(supports_qHash_test2(std::declval<T1>(), std::declval<T2>())){};
template<> struct supports_qHash<void> : std::false_type{};

template<class T, class = decltype(T())>
std::true_type  supports_StandardConstructor_test(T&&);
std::false_type supports_StandardConstructor_test(...);

template<class T> struct supports_StandardConstructor : decltype(supports_StandardConstructor_test(std::declval<T>())){};
template<> struct supports_StandardConstructor<void> : std::false_type{};

}

#endif // QTJAMBI_TYPETESTS_H
