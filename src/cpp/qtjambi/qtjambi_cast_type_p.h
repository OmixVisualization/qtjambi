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

#ifndef QTJAMBI_CAST_TYPE_P_H
#define QTJAMBI_CAST_TYPE_P_H

#include "qtjambi_core.h"
#include <QtQml/QQmlListProperty>
#ifndef QT_NO_DBUS
#include <QtDBus/QDBusReply>
#endif

namespace QtJambiPrivate {

struct qtjambi_type_proxy{};

template<typename T>
struct qtjambi_type;

template<template<typename T> class Container, typename T>
struct qtjambi_type_container1{
    static const std::type_info& id() {return typeid(Container<T>);}
};

template<typename T>
struct qtjambi_type_container1<QList,T>{
    static const std::type_info& id() {return typeid(QList<QVariant>);}
};

template<typename T>
struct qtjambi_type_container1<QLinkedList,T>{
    static const std::type_info& id() {return typeid(QLinkedList<QVariant>);}
};

template<typename T>
struct qtjambi_type_container1<QSet,T>{
    static const std::type_info& id() {return typeid(QSet<QVariant>);}
};

template<typename T>
struct qtjambi_type_container1<QQueue,T>{
    static const std::type_info& id() {return typeid(QQueue<QVariant>);}
};

template<typename T>
struct qtjambi_type_container1<QStack,T>{
    static const std::type_info& id() {return typeid(QStack<QVariant>);}
};

template<typename T>
struct qtjambi_type_container1<QVector,T>{
    static const std::type_info& id() {return typeid(QVector<QVariant>);}
};

#ifndef QT_NO_DBUS
template<typename T>
struct qtjambi_type_container1<QDBusReply,T>{
    static const std::type_info& id() {return typeid(QDBusReply<QVariant>);}
};
#endif

template<typename T>
struct qtjambi_type_container1<QQmlListProperty,T>{
    static const std::type_info& id() {return typeid(QQmlListProperty<QObject>);}
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
    static const std::type_info& id() {return typeid(Container<K,T>);}
};

template<typename K, typename T>
struct qtjambi_type_container2<QMap,K,T>{
    static const std::type_info& id() {return typeid(QMap<QVariant,QVariant>);}
};

template<typename K, typename T>
struct qtjambi_type_container2<QMultiMap,K,T>{
    static const std::type_info& id() {return typeid(QMultiMap<QVariant,QVariant>);}
};

template<typename K, typename T>
struct qtjambi_type_container2<QHash,K,T>{
    static const std::type_info& id() {return typeid(QHash<QVariant,QVariant>);}
};

template<typename K, typename T>
struct qtjambi_type_container2<QMultiHash,K,T>{
    static const std::type_info& id() {return typeid(QMultiHash<QVariant,QVariant>);}
};

template<typename K, typename T>
struct qtjambi_type_container2<QScopedPointer,K,T> : qtjambi_type<K>{
};

template<template<typename...Ts> class Container, int parameterCount, typename...Ts>
struct qtjambi_type_container_selector{
};

template<template<typename...Ts> class Container, typename...Ts>
struct qtjambi_type_container_selector<Container, 1, Ts...>{
    template<template<typename T> class _Container, typename T>
    static const std::type_info& _id(){
        return qtjambi_type_container1<_Container, T>::id();
    }

    static const std::type_info& id(){
        return _id<Container, Ts...>();
    }
};

template<template<typename...Ts> class Container, typename...Ts>
struct qtjambi_type_container_selector<Container, 2, Ts...>{
    template<template<typename K, typename T> class _Container, typename K, typename T>
    static const std::type_info& _id(){
        return qtjambi_type_container2<_Container, K, T>::id();
    }

    static const std::type_info& id(){
        return _id<Container, Ts...>();
    }
};

template<typename T>
struct qtjambi_type_selector{
    static const std::type_info& id(){ return typeid(T); }
};

template<template<typename...Ts> class Container, typename...Ts>
struct qtjambi_type_selector<Container<Ts...>> : qtjambi_type_container_selector<Container,sizeof...(Ts),Ts...>{
};

template<typename T>
struct qtjambi_type : qtjambi_type_selector<typename std::remove_pointer<T>::type>{
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_TYPE_P_H
