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

#ifndef CONTAINERACCESS_ASSOCIATIVE_H
#define CONTAINERACCESS_ASSOCIATIVE_H

#include "containeraccess_sequential.h"

namespace QtJambiPrivate {

typedef bool (*IsAssociativeContainerFunction)(JNIEnv *, jobject, const std::type_info&, const QMetaType&, const std::type_info&, const QMetaType&, void*& pointer);
typedef bool (*IsAssociativeContainerAccessFunction)(JNIEnv *, jobject, const QMetaType&, const QMetaType&, void*& pointer, AbstractContainerAccess*& access);

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerBegin{
    static jobject function(JNIEnv *env, const ExtendedContainerInfo&) {
        JavaException::raiseUnsupportedOperationException(env, "begin()" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerBegin<Container, K, T, true>{
    static jobject function(JNIEnv *env, const ExtendedContainerInfo& ptr) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr.container);
        return ::qtjambi_cast<jobject>(env, ptr.nativeId, container->begin());
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerConstBegin{
    static jobject function(JNIEnv *env, const ConstExtendedContainerInfo&) {
        JavaException::raiseUnsupportedOperationException(env, "constBegin()" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerConstBegin<Container, K, T, true>{
    static jobject function(JNIEnv *env, const ConstExtendedContainerInfo& ptr) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr.container);
        return ::qtjambi_cast<jobject>(env, ptr.nativeId, container->constBegin());
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerCapacity{
    static qsizetype function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "capacity()" QTJAMBI_STACKTRACEINFO );
        return 0;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerCapacity<Container, K, T, true>{
    static qsizetype function(JNIEnv *, const void* ptr) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return container->capacity();
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerReserve{
    static void function(JNIEnv *env, const ContainerInfo&, qsizetype) {
        JavaException::raiseUnsupportedOperationException(env, "reserve(size)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerReserve<Container, K, T, true>{
    static void function(JNIEnv *, const ContainerInfo& ptr, qsizetype size) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr.container);
        container->reserve(size);
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerContains{
    static jboolean function(JNIEnv * env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "contains(key)" QTJAMBI_STACKTRACEINFO );
        return false;
    }
    static bool function(const void*, ...) {
        return false;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerContains<Container, K, T, true>{
    static jboolean function(JNIEnv * env, const void* ptr, jobject object) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return container->contains(::qtjambi_cast<K>(env, object));
    }
    static bool function(const void* ptr, const void* object) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return container->contains(*reinterpret_cast<const K*>(object));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerCountObject{
    static qsizetype function(JNIEnv * env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "count(key)" QTJAMBI_STACKTRACEINFO );
        return 0;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerCountObject<Container, K, T, true>{
    static qsizetype function(JNIEnv * env, const void* ptr, jobject object) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return container->count(::qtjambi_cast<K>(env, object));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerEnd{
    static jobject function(JNIEnv *env, const ExtendedContainerInfo&) {
        JavaException::raiseUnsupportedOperationException(env, "end()" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerEnd<Container, K, T, true>{
    static jobject function(JNIEnv *env, const ExtendedContainerInfo& ptr) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr.container);
        return ::qtjambi_cast<jobject>(env, ptr.nativeId, container->end());
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerConstEnd{
    static jobject function(JNIEnv *env, const ConstExtendedContainerInfo&) {
        JavaException::raiseUnsupportedOperationException(env, "constEnd()" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerConstEnd<Container, K, T, true>{
    static jobject function(JNIEnv *env, const ConstExtendedContainerInfo& ptr) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr.container);
        return ::qtjambi_cast<jobject>(env, ptr.nativeId, container->constEnd());
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerFindPair{
    static jobject function(JNIEnv *env, const ExtendedContainerInfo&, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "find(key,value)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerFindPair<Container, K, T, true>{
    static jobject function(JNIEnv *env, const ExtendedContainerInfo& ptr, jobject key) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr.container);
        return ::qtjambi_cast<jobject>(env, ptr.nativeId, container->find(::qtjambi_cast<K>(env, key)));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerConstFindPair{
    static jobject function(JNIEnv *env, const ConstExtendedContainerInfo&, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "constFind(key,value)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerConstFindPair<Container, K, T, true>{
    static jobject function(JNIEnv *env, const ConstExtendedContainerInfo& ptr, jobject key) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr.container);
        return ::qtjambi_cast<jobject>(env, ptr.nativeId, container->constFind(::qtjambi_cast<K>(env, key)));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerFirst{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "first()" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerFirst<Container, K, T, true>{
    static jobject function(JNIEnv *env, const void* ptr) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return ::qtjambi_cast<jobject>(env, container->first());
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerLast{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "last()" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerLast<Container, K, T, true>{
    static jobject function(JNIEnv *env, const void* ptr) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return ::qtjambi_cast<jobject>(env, container->last());
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerFirstKey{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "firstKey()" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerFirstKey<Container, K, T, true>{
    static jobject function(JNIEnv *env, const void* ptr) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return ::qtjambi_cast<jobject>(env, container->firstKey());
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerLastKey{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "lastKey()" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerLastKey<Container, K, T, true>{
    static jobject function(JNIEnv *env, const void* ptr) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return ::qtjambi_cast<jobject>(env, container->lastKey());
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value && supports_equal<T>::value>
struct AssociativeContainerKey{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "key(value, defaultKey)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerKey<Container, K, T, true>{
    static jobject function(JNIEnv *env, const void* ptr, jobject value, jobject defaultKey) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return ::qtjambi_cast<jobject>(env, container->key(::qtjambi_cast<T>(env, value), ::qtjambi_cast<K>(env, defaultKey)));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerKeys{
    static ContainerAndAccessInfo function(JNIEnv *env, const ConstContainerInfo&) {
        JavaException::raiseUnsupportedOperationException(env, "keys()" QTJAMBI_STACKTRACEINFO );
        return {};
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerKeys<Container, K, T, true>{
    static ContainerAndAccessInfo function(JNIEnv *env, const ConstContainerInfo& ptr) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr.container);
        ContainerAndAccessInfo result;
        result.container = new QList<K>(container->keys());
        result.access = QListAccess<K>::newInstance();
        result.object = ContainerAPI::objectFromQList(env, result.container, result.access);
        return result;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_keys_by_value<Container,K,T>::value && supports_map_sort<Container,K,T>::value && supports_equal<K>::value && supports_equal<T>::value>
struct AssociativeContainerKeysForValue{
    static ContainerAndAccessInfo function(JNIEnv *env, const ConstContainerInfo&, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "keys(value)" QTJAMBI_STACKTRACEINFO );
        return {};
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerKeysForValue<Container, K, T, true>{
    static ContainerAndAccessInfo function(JNIEnv *env, const ConstContainerInfo& ptr, jobject value) {
        ContainerAndAccessInfo result;
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr.container);
        result.container = new QList<K>(container->keys(::qtjambi_cast<T>(env, value)));
        result.access = QListAccess<K>::newInstance();
        result.object = ContainerAPI::objectFromQList(env, result.container, result.access);
        return result;
    }
};

#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
template<typename K, typename T>
struct AssociativeContainerKeysForValue<QMultiHash, K, T, true>{
    static ContainerAndAccessInfo function(JNIEnv *env, const ConstContainerInfo& ptr, jobject value) {
        ContainerAndAccessInfo result;
        const QMultiHash<K,T> *container = static_cast<const QMultiHash<K,T> *>(ptr.container);
        QList<K> _keys;
        typename QMultiHash<K,T>::const_iterator i = reinterpret_cast<const QMultiHash<K,T> *>(container)->begin();
        typename QMultiHash<K,T>::const_iterator end = reinterpret_cast<const QMultiHash<K,T> *>(container)->end();
        T _qvalue = ::qtjambi_cast<T>(env, value);
        while (i != end) {
            if(i.value() == _qvalue)
                _keys.append(i.key());
            ++i;
        }
        result.container = new QList<K>(std::move(_keys));
        result.access = QListAccess<K>::newInstance();
        result.object = ContainerAPI::objectFromQList(env, result.container, result.access);
        return result;
    }
};
#endif


template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerLowerBound{
    static jobject function(JNIEnv *env, const ConstExtendedContainerInfo&, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "lowerBounds(value)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerLowerBound<Container, K, T, true>{
    static jobject function(JNIEnv *env, const ConstExtendedContainerInfo& ptr, jobject key) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr.container);
        return ::qtjambi_cast<jobject>(env, ptr.nativeId, container->lowerBound(::qtjambi_cast<K>(env, key)));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerUpperBound{
    static jobject function(JNIEnv *env, const ConstExtendedContainerInfo&, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "upperBounds(value)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerUpperBound<Container, K, T, true>{
    static jobject function(JNIEnv *env, const ConstExtendedContainerInfo& ptr, jobject key) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr.container);
        return ::qtjambi_cast<jobject>(env, ptr.nativeId, container->upperBound(::qtjambi_cast<K>(env, key)));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, IsAssociativeContainerFunction isContainer, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value && supports_equal<T>::value>
struct AssociativeContainerEquals{
    static jboolean function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "operator==(map)" QTJAMBI_STACKTRACEINFO );
        return false;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, IsAssociativeContainerFunction isContainer>
struct AssociativeContainerEquals<Container, K, T, isContainer, true>{
    static jboolean function(JNIEnv * env, const void* ptr, jobject other) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        std::unique_ptr<Container<K,T> > __qt_scoped_pointer;
        Container<K,T> *__qt_other_pointer = nullptr;
        if (other!= nullptr) {
            if (!isContainer(env, other, qtjambi_type<K>::id(), QMetaType::fromType<std::remove_cv_t<K>>(), qtjambi_type<T>::id(), QMetaType::fromType<std::remove_cv_t<T>>(), reinterpret_cast<void*&>(__qt_other_pointer))) {
                __qt_scoped_pointer.reset(new Container<K,T>());
                __qt_other_pointer = __qt_scoped_pointer.get();
                jobject iterator = QtJambiAPI::entrySetIteratorOfJavaMap(env, other);
                while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                    jobject entry = QtJambiAPI::nextOfJavaIterator(env, iterator);
                    jobject key = QtJambiAPI::keyOfJavaMapEntry(env, entry);
                    jobject val = QtJambiAPI::valueOfJavaMapEntry(env, entry);
                    __qt_other_pointer->insert( ::qtjambi_cast<K>(env, key), ::qtjambi_cast<T>(env, val));
                }
            }
        }else{
            __qt_scoped_pointer.reset(new Container<K,T> ());
            __qt_other_pointer = __qt_scoped_pointer.get();
        }
        const Container<K,T>& __qt_other = *__qt_other_pointer;
        return (*container)==__qt_other;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerSize{
    static qsizetype function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "size()" QTJAMBI_STACKTRACEINFO );
        return 0;
    }
    static qsizetype function(const void*) {
        return 0;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerSize<Container, K, T, true>{
    static qsizetype function(JNIEnv *, const void* ptr) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return container->size();
    }
    static qsizetype function(const void* ptr) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return container->size();
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerUniqueKeys{
    static ContainerAndAccessInfo function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "uniqueKeys()" QTJAMBI_STACKTRACEINFO );
        return {};
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerUniqueKeys<Container, K, T, true>{
    static ContainerAndAccessInfo function(JNIEnv *env, const ConstContainerInfo& ptr) {
        ContainerAndAccessInfo result;
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr.container);
        result.container = new QList<K>(container->uniqueKeys());
        result.access = QListAccess<K>::newInstance();
        result.object = ContainerAPI::objectFromQList(env, result.container, result.access);
        return result;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerValue{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "value(key)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
    static const void* function(const void*, ...) {
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerValue<Container, K, T, true>{
    static jobject function(JNIEnv * env, const void* ptr, jobject key, jobject defaultValue) {
            const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
            return ::qtjambi_cast<jobject>(env, container->value(::qtjambi_cast<K>(env, key), ::qtjambi_cast<T>(env, defaultValue)));
    }
    static const void* function(const void* ptr, const void* key, const void* defaultValue) {
        const Container<K,T> &container = *static_cast<const Container<K,T> *>(ptr);
        const K& _key = *reinterpret_cast<const K*>(key);
        auto iter = container.find(_key);
        if(iter != container.end()){
            return &iter.value();
        }else{
            return defaultValue;
        }
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerValues{
    static ContainerAndAccessInfo function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "values()" QTJAMBI_STACKTRACEINFO );
        return {};
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerValues<Container, K, T, true>{
    static ContainerAndAccessInfo function(JNIEnv * env, const ConstContainerInfo& ptr) {
        ContainerAndAccessInfo result;
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr.container);
        result.container = new QList<T>(container->values());
        result.access = QListAccess<T>::newInstance();
        result.object = ContainerAPI::objectFromQList(env, result.container, result.access);
        return result;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerValuesKey{
    static ContainerAndAccessInfo function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "values(key)" QTJAMBI_STACKTRACEINFO );
        return {};
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerValuesKey<Container, K, T, true>{
    static ContainerAndAccessInfo function(JNIEnv * env, const ConstContainerInfo& ptr, jobject key) {
        ContainerAndAccessInfo result;
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr.container);
        result.container = new QList<T>(container->values(::qtjambi_cast<K>(env, key)));
        result.access = QListAccess<T>::newInstance();
        result.object = ContainerAPI::objectFromQList(env, result.container, result.access);
        return result;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerClear{
    static void function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "clear()" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerClear<Container, K, T, true>{
    static void function(JNIEnv * __jni_env, const ContainerInfo& ptr) {
        Q_UNUSED(__jni_env)
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr.container);
        container->clear();
    }
};

template<typename T, bool = supports_less_than<T>::value>
struct AssociativeContainerElementLessThan{
    static bool function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "key1 < key2" QTJAMBI_STACKTRACEINFO );
        return false;
    }
};

template<typename T>
struct AssociativeContainerElementLessThan<T, true>{
    static bool function(JNIEnv * __jni_env, jobject value1, jobject value2) {
        return ::qtjambi_cast<T>(__jni_env, value1) < ::qtjambi_cast<T>(__jni_env, value2);
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerInsert{
    static void function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "insert(key,value)" QTJAMBI_STACKTRACEINFO );
    }
    static void function(void*, ...) {
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerInsert<Container, K, T, true>{
    static void function(JNIEnv * __jni_env, const ContainerInfo& ptr, jobject key, jobject value) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr.container);
        container->insert(::qtjambi_cast<K>(__jni_env, key), ::qtjambi_cast<T>(__jni_env, value));
    }
    static void function(void* ptr, const void* key, const void* value) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
        container->insert(*reinterpret_cast<const K*>(key), *reinterpret_cast<const T*>(value));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerRemove{
    static qsizetype function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "remove(key)" QTJAMBI_STACKTRACEINFO );
        return 0;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerRemove<Container, K, T, true>{
    static qsizetype function(JNIEnv * __jni_env, const ContainerInfo& ptr, jobject key) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr.container);
        return container->remove(::qtjambi_cast<K>(__jni_env, key));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value && is_default_constructible_v<T>>
struct AssociativeContainerTake{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "take(key)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerTake<Container, K, T, true>{
    static jobject function(JNIEnv * env, const ContainerInfo& ptr, jobject key) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr.container);
        return ::qtjambi_cast<jobject>(env, container->take(::qtjambi_cast<K>(env, key)));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, IsAssociativeContainerAccessFunction isContainer, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerUnite{
    static void function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "unite(map)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, IsAssociativeContainerAccessFunction isContainer>
struct AssociativeContainerUnite<Container, K, T, isContainer, true>{
    static void function(JNIEnv * env, const ContainerInfo& ptr, ContainerAndAccessInfo& other) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr.container);
        std::unique_ptr<Container<K,T> > __qt_scoped_pointer;
        Container<K,T> *__qt_other_pointer = nullptr;
        if (other.object != nullptr) {
            if (isContainer(env, other.object, QMetaType::fromType<std::remove_cv_t<K>>(), QMetaType::fromType<std::remove_cv_t<T>>(), reinterpret_cast<void*&>(__qt_other_pointer), other.access)) {
                other.container = __qt_other_pointer;
            }else{
                __qt_scoped_pointer.reset(new Container<K,T> ());
                __qt_other_pointer = __qt_scoped_pointer.get();
                jobject iterator = QtJambiAPI::entrySetIteratorOfJavaMap(env, other.object);
                while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                    jobject entry = QtJambiAPI::nextOfJavaIterator(env, iterator);
                    jobject key = QtJambiAPI::keyOfJavaMapEntry(env, entry);
                    jobject valCollection = QtJambiAPI::valueOfJavaMapEntry(env, entry);
                    jobject iterator2 = QtJambiAPI::iteratorOfJavaIterable(env, valCollection);
                    while(QtJambiAPI::hasJavaIteratorNext(env, iterator2)) {
                        jobject val = QtJambiAPI::nextOfJavaIterator(env, iterator2);
                        __qt_other_pointer->insert(::qtjambi_cast<K>(env, key), ::qtjambi_cast<T>(env, val));
                    }
                }
            }
        }else{
            __qt_scoped_pointer.reset(new Container<K,T> ());
            __qt_other_pointer = __qt_scoped_pointer.get();
        }
        container->unite(*__qt_other_pointer);
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value && supports_equal<T>::value>
struct AssociativeContainerContainsPair{
    static jboolean function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "contains(key,value)" QTJAMBI_STACKTRACEINFO );
        return false;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerContainsPair<Container, K, T, true>{
    static jboolean function(JNIEnv * env, const void* ptr, jobject key, jobject object) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return container->contains(::qtjambi_cast<K>(env, key), ::qtjambi_cast<T>(env, object));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value && supports_equal<T>::value>
struct AssociativeContainerCountPair{
    static qsizetype function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "count(key,value)" QTJAMBI_STACKTRACEINFO );
        return 0;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerCountPair<Container, K, T, true>{
    static qsizetype function(JNIEnv * env, const void* ptr, jobject key, jobject object) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return container->count(::qtjambi_cast<K>(env, key), ::qtjambi_cast<T>(env, object));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value && supports_equal<T>::value>
struct AssociativeContainerFindPairs{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "find(key,value)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerFindPairs<Container, K, T, true>{
    static jobject function(JNIEnv *env, const ExtendedContainerInfo& ptr, jobject key, jobject object) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr.container);
        return ::qtjambi_cast<jobject>(env, ptr.nativeId, container->find(::qtjambi_cast<K>(env, key), ::qtjambi_cast<T>(env, object)));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value && supports_equal<T>::value>
struct AssociativeContainerConstFindPairs{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "constFind(key,value)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerConstFindPairs<Container, K, T, true>{
    static jobject function(JNIEnv *env, const ConstExtendedContainerInfo& ptr, jobject key, jobject object) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr.container);
        return ::qtjambi_cast<jobject>(env, ptr.nativeId, container->constFind(::qtjambi_cast<K>(env, key), ::qtjambi_cast<T>(env, object)));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value && supports_equal<T>::value>
struct AssociativeContainerRemovePair{
    static qsizetype function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "remove(key,value)" QTJAMBI_STACKTRACEINFO );
        return 0;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerRemovePair<Container, K, T, true>{
    static qsizetype function(JNIEnv * env, const ContainerInfo& ptr, jobject key, jobject object) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr.container);
        return container->remove(::qtjambi_cast<K>(env, key), ::qtjambi_cast<T>(env, object));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value && supports_equal<T>::value>
struct AssociativeContainerReplacePair{
    static void function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "replace(key,value)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerReplacePair<Container, K, T, true>{
    static void function(JNIEnv * env, const ContainerInfo& ptr, jobject key, jobject object) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr.container);
        container->replace(::qtjambi_cast<K>(env, key), ::qtjambi_cast<T>(env, object));
    }
};

 template<template<typename K, typename T> class Container, typename K, typename T, bool isPointer, bool = is_copy_constructible_v<K> && is_copy_constructible_v<T> && is_default_constructible<K>::value && is_default_constructible_v<T> && is_copy_constructible_v<Container<K,T>>>
 struct CloneAssociativeContainer{
     static constexpr CopyFunction function = nullptr;
 };

 template<template<typename K, typename T> class Container, typename K, typename T>
 struct CloneAssociativeContainer<Container,K,T,false,true>{
     static void* clone(const void* ptr) { return new Container<K,T>(*reinterpret_cast<const Container<K,T>*>(ptr)); }
     static constexpr CopyFunction function = &clone;
 };

 template<template<typename K, typename T> class Container, typename K, typename T>
 struct DeleteAssociativeContainer{
     static void del(void* ptr,bool) { delete reinterpret_cast<Container<K,T>*>(ptr); }
     static constexpr PtrDeleterFunction function = &del;
 };

template<typename K, typename T, typename Super>
class OwnerFunctionAssociativeAccess : public Super{
protected:
    OwnerFunctionAssociativeAccess(){}
public:
    const QObject* getOwner(const void* container) override{
        Super* _this = this;
        const QObject* owner = nullptr;
        if constexpr(QtJambiPrivate::ContainerContentType<K>::isContainer){
            if(AbstractContainerAccess* elementNestedContainerAccess = ContainerContentType<K>::accessFactory()){
                auto iter = _this->constKeyValueIterator(container);
                while(iter->hasNext()){
                    auto current = iter->next();
                    owner = elementNestedContainerAccess->getOwner(current.first);
                    if(owner)
                        break;
                }
                elementNestedContainerAccess->dispose();
            }
        }else{
            if(PtrOwnerFunction ownerFunctionK = registeredOwnerFunction<K>()){
                auto iter = _this->constKeyValueIterator(container);
                while(iter->hasNext()){
                    auto current = iter->next();
                    if((owner = ownerFunctionK(current.first)))
                        break;
                }
            }
        }
        if(!owner){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::isContainer){
                if(AbstractContainerAccess* elementNestedContainerAccess = ContainerContentType<T>::accessFactory()){
                    auto iter = _this->constKeyValueIterator(container);
                    while(iter->hasNext()){
                        auto current = iter->next();
                        owner = elementNestedContainerAccess->getOwner(current.second);
                        if(owner)
                            break;
                    }
                    elementNestedContainerAccess->dispose();
                }
            }else{
                if(PtrOwnerFunction ownerFunctionT = registeredOwnerFunction<T>()){
                    auto iter = _this->constKeyValueIterator(container);
                    while(iter->hasNext()){
                        auto current = iter->next();
                        if((owner = ownerFunctionT(current.second)))
                            break;
                    }
                }
            }
        }
        return owner;
    }
    bool hasOwnerFunction() override{
        bool result = false;
        if constexpr(QtJambiPrivate::ContainerContentType<K>::isContainer){
            if(AbstractContainerAccess* elementNestedContainerAccess = ContainerContentType<K>::accessFactory()){
                result = elementNestedContainerAccess->hasOwnerFunction();
                elementNestedContainerAccess->dispose();
            }
        }else{
            result = registeredOwnerFunction<K>()!=nullptr;
        }
        if(!result){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::isContainer){
                if(AbstractContainerAccess* elementNestedContainerAccess = ContainerContentType<T>::accessFactory()){
                    result = elementNestedContainerAccess->hasOwnerFunction();
                    elementNestedContainerAccess->dispose();
                }
            }else{
                result = registeredOwnerFunction<T>()!=nullptr;
            }
        }
        return result;
    }
};

template<typename K, typename T, typename Super>
class ReferenceCountingAssociativeSetAccess : public Super, public ReferenceCountingSetContainer{
protected:
    ReferenceCountingAssociativeSetAccess(){}
public:
    AbstractReferenceCountingContainer* asRC() override {return this;}
    void updateRC(JNIEnv * env, const ContainerInfo& container) override {
        Super* _this = this;
        JniLocalFrame frame(env, 200);
        jobject set{nullptr};
        if constexpr((QtJambiPrivate::ContainerContentType<K>::isContainer && ContainerContentType<K>::needsReferenceCounting)
                      || (QtJambiPrivate::ContainerContentType<K>::isContainer && ContainerContentType<K>::needsReferenceCounting)){
            set = QtJambiAPI::newJavaHashSet(env);
            auto access1 = _this->keyNestedContainerAccess();
            auto access2 = _this->valueNestedContainerAccess();
            auto iterator = _this->constKeyValueIterator(container.container);
            while(iterator->hasNext()){
                auto current = iterator->next();
                unfoldAndAddContainer(env, set, current.first, ContainerContentType<K>::type, _this->keyMetaType(), access1);
                unfoldAndAddContainer(env, set, current.second, ContainerContentType<T>::type, _this->valueMetaType(), access2);
            }
            if(access1)
                access1->dispose();
            if(access2)
                access2->dispose();
        }else if constexpr(QtJambiPrivate::ContainerContentType<K>::type==AbstractContainerAccess::PointerToQObject){
            Q_STATIC_ASSERT_X(QtJambiPrivate::ContainerContentType<T>::type==AbstractContainerAccess::Value, "Associative reference counting required");
            set = QtJambiAPI::newJavaArrayList(env);
            auto iterator = _this->constKeyValueIterator(container.container);
            while(iterator->hasNext()){
                auto content = iterator->next();
                if(jobject obj = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content.first)))
                    QtJambiAPI::addToJavaCollection(env, set, obj);
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<K>::type==AbstractContainerAccess::FunctionPointer){
            Q_STATIC_ASSERT_X(QtJambiPrivate::ContainerContentType<T>::type==AbstractContainerAccess::Value, "Associative reference counting required");
            set = QtJambiAPI::newJavaArrayList(env);
            auto iterator = _this->constKeyValueIterator(container.container);
            while(iterator->hasNext()){
                auto content = iterator->next();
                if(jobject obj = findFunctionPointerObject(env, content.first, typeid(K)))
                    QtJambiAPI::addToJavaCollection(env, set, obj);
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<K>::type==AbstractContainerAccess::Pointer){
            Q_STATIC_ASSERT_X(QtJambiPrivate::ContainerContentType<T>::type==AbstractContainerAccess::Value, "Associative reference counting required");
            set = QtJambiAPI::newJavaArrayList(env);
            auto iterator = _this->constKeyValueIterator(container.container);
            while(iterator->hasNext()){
                auto content = iterator->next();
                if(jobject obj = QtJambiAPI::findObject(env, content.first))
                    QtJambiAPI::addToJavaCollection(env, set, obj);
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<T>::type==AbstractContainerAccess::PointerToQObject){
            Q_STATIC_ASSERT_X(QtJambiPrivate::ContainerContentType<K>::type==AbstractContainerAccess::Value, "Associative reference counting required");
            set = QtJambiAPI::newJavaArrayList(env);
            auto iterator = _this->constKeyValueIterator(container.container);
            while(iterator->hasNext()){
                auto content = iterator->next();
                if(jobject obj = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content.second)))
                    QtJambiAPI::addToJavaCollection(env, set, obj);
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<T>::type==AbstractContainerAccess::FunctionPointer){
            Q_STATIC_ASSERT_X(QtJambiPrivate::ContainerContentType<K>::type==AbstractContainerAccess::Value, "Associative reference counting required");
            set = QtJambiAPI::newJavaArrayList(env);
            auto iterator = _this->constKeyValueIterator(container.container);
            while(iterator->hasNext()){
                auto content = iterator->next();
                if(jobject obj = findFunctionPointerObject(env, content.second, typeid(T)))
                    QtJambiAPI::addToJavaCollection(env, set, obj);
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<T>::type==AbstractContainerAccess::Pointer){
            Q_STATIC_ASSERT_X(QtJambiPrivate::ContainerContentType<K>::type==AbstractContainerAccess::Value, "Associative reference counting required");
            set = QtJambiAPI::newJavaArrayList(env);
            auto iterator = _this->constKeyValueIterator(container.container);
            while(iterator->hasNext()){
                auto content = iterator->next();
                if(jobject obj = QtJambiAPI::findObject(env, content.second))
                    QtJambiAPI::addToJavaCollection(env, set, obj);
            }
        }else{
            //Q_STATIC_ASSERT_X(false, "No reference counting required");
            return;
        }
        clearRC(env, container.object);
        addAllRC(env, container.object, set);
    }
};

template<typename K, typename T, typename Super>
class ReferenceCountingAssociativeMapAccess : public Super, public ReferenceCountingMapContainer{
protected:
    ReferenceCountingAssociativeMapAccess(){}
    Q_STATIC_ASSERT_X((QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0
                          && (QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0, "Sequential reference counting required");
public:
    AbstractReferenceCountingContainer* asRC() override {return this;}
    void updateRC(JNIEnv * env, const ContainerInfo& container) override {
        Super* _this = this;
        JniLocalFrame frame(env, 200);
        jobject map = QtJambiAPI::newJavaHashMap(env);
        jobject key{nullptr};
        jobject value{nullptr};
        auto iterator = _this->constKeyValueIterator(container.container);
        while(iterator->hasNext()){
            auto content = iterator->next();
            if constexpr(QtJambiPrivate::ContainerContentType<K>::type==AbstractContainerAccess::PointerToQObject){
                key = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content.first));
            }else if constexpr(QtJambiPrivate::ContainerContentType<K>::type==AbstractContainerAccess::FunctionPointer){
                key = findFunctionPointerObject(env, content.first, typeid(K));
            }else{
                key = QtJambiAPI::findObject(env, content.first);
            }
            if constexpr(QtJambiPrivate::ContainerContentType<T>::type==AbstractContainerAccess::PointerToQObject){
                value = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content.second));
            }else if constexpr(QtJambiPrivate::ContainerContentType<T>::type==AbstractContainerAccess::FunctionPointer){
                value = findFunctionPointerObject(env, content.second, typeid(T));
            }else{
                value = QtJambiAPI::findObject(env, content.second);
            }
            QtJambiAPI::putJavaMap(env, map, key, value);
        }
        clearRC(env, container.object);
        putAllRC(env, container.object, map);
    }
};

template<typename K, typename T, typename Super>
class ReferenceCountingAssociativeMultiMapAccess : public Super, public ReferenceCountingMultiMapContainer{
protected:
    ReferenceCountingAssociativeMultiMapAccess(){}
public:
    Q_STATIC_ASSERT_X((QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0
                          && (QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0, "Sequential reference counting required");
    AbstractReferenceCountingContainer* asRC() override {return this;}
    void updateRC(JNIEnv * env, const ContainerInfo& container) override {
        Super* _this = this;
        JniLocalFrame frame(env, 200);
        jobject map = ReferenceCountingMultiMapContainer::newRCMultiMap(env);
        jobject key{nullptr};
        jobject value{nullptr};
        auto iterator = _this->constKeyValueIterator(container.container);
        while(iterator->hasNext()){
            auto content = iterator->next();
            if constexpr(QtJambiPrivate::ContainerContentType<K>::type==AbstractContainerAccess::PointerToQObject){
                key = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content.first));
            }else if constexpr(QtJambiPrivate::ContainerContentType<K>::type==AbstractContainerAccess::FunctionPointer){
                key = findFunctionPointerObject(env, content.first, typeid(K));
            }else{
                key = QtJambiAPI::findObject(env, content.first);
            }
            if constexpr(QtJambiPrivate::ContainerContentType<T>::type==AbstractContainerAccess::PointerToQObject){
                value = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content.second));
            }else if constexpr(QtJambiPrivate::ContainerContentType<T>::type==AbstractContainerAccess::FunctionPointer){
                value = findFunctionPointerObject(env, content.second, typeid(T));
            }else{
                value = QtJambiAPI::findObject(env, content.second);
            }
            QtJambiAPI::putJavaMap(env, map, key, value);
        }
        clearRC(env, container.object);
        putAllRC(env, container.object, map);
    }
};

template<typename K, typename T, typename Super,
         bool K_is_pointer = (QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0,
         bool T_is_pointer = (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0,
         bool K_is_container = ContainerContentType<K>::isContainer /*true*/,
         bool T_is_container = ContainerContentType<T>::isContainer /*true*/>
struct AssociativeAccessSuperclassDecider_IsContainer{
    typedef Super type;
};

template<typename K, typename T, typename Super>
struct AssociativeAccessSuperclassDecider_IsContainer<K,T,Super,false,true,true,false>{
    // Container<Container,Ptr*>
    typedef std::conditional_t<ContainerContentType<K>::needsOwnerCheck, OwnerFunctionAssociativeAccess<K,T,Super>, Super> ownerSuper;
    typedef ReferenceCountingAssociativeSetAccess<K,T,ownerSuper> type;
};

template<typename K, typename T, typename Super>
struct AssociativeAccessSuperclassDecider_IsContainer<K,T,Super,true,false,false,true>{
    // Container<Ptr*,Container>
    typedef std::conditional_t<ContainerContentType<T>::needsOwnerCheck, OwnerFunctionAssociativeAccess<K,T,Super>, Super> ownerSuper;
    typedef ReferenceCountingAssociativeSetAccess<K,T,ownerSuper> type;
};

template<typename K, typename T, typename Super>
struct AssociativeAccessSuperclassDecider_IsContainer<K,T,Super,false,true,false,false>{
    // Container<Value,Ptr*>
    typedef ReferenceCountingAssociativeSetAccess<K,T,OwnerFunctionAssociativeAccess<K,T,Super>> type;
};

template<typename K, typename T, typename Super>
struct AssociativeAccessSuperclassDecider_IsContainer<K,T,Super,true,false,false,false>{
    // Container<Ptr*,Value>
    typedef ReferenceCountingAssociativeSetAccess<K,T,OwnerFunctionAssociativeAccess<K,T,Super>> type;
};

template<typename K, typename T, typename Super>
struct AssociativeAccessSuperclassDecider_IsContainer<K,T,Super,false,false,true,false>{
    // Container<Container,Value>
    typedef OwnerFunctionAssociativeAccess<K,T,Super> ownerSuper;
    typedef std::conditional_t<ContainerContentType<K>::needsReferenceCounting, ReferenceCountingAssociativeSetAccess<K,T,ownerSuper>,ownerSuper> type;
};

template<typename K, typename T, typename Super>
struct AssociativeAccessSuperclassDecider_IsContainer<K,T,Super,false,false,false,true>{
    // Container<Value,Container>
    typedef OwnerFunctionAssociativeAccess<K,T,Super> ownerSuper;
    typedef std::conditional_t<ContainerContentType<T>::needsReferenceCounting, ReferenceCountingAssociativeSetAccess<K,T,ownerSuper>,ownerSuper> type;
};

template<typename K, typename T, typename Super>
struct AssociativeAccessSuperclassDecider_IsContainer<K,T,Super,false,false,false,false>{
    // Container<Value,Value>
    typedef OwnerFunctionAssociativeAccess<K,T,Super> type;
};

template<typename K, typename T, typename Super,
         bool isMulti = std::is_same_v<Super,AbstractMultiMapAccess> || std::is_same_v<Super,AbstractMultiHashAccess>,
         bool K_is_pointer = (QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0,
         bool T_is_pointer = (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0>
struct AssociativeAccessSuperclassDecider : AssociativeAccessSuperclassDecider_IsContainer<K,T,Super>{
};

template<typename K, typename T, typename Super>
struct AssociativeAccessSuperclassDecider<K,T,Super,/*isMulti*/false,true,true>{
    typedef ReferenceCountingAssociativeMapAccess<K,T,Super> type;
};

template<typename K, typename T, typename Super>
struct AssociativeAccessSuperclassDecider<K,T,Super,/*isMulti*/true,true,true>{
    typedef ReferenceCountingAssociativeMultiMapAccess<K,T,Super> type;
};

} // namespace QtJambiPrivate



template<typename K, typename T>
class QMapAccess : public QtJambiPrivate::AssociativeAccessSuperclassDecider<K,T,AbstractMapAccess>::type{
    typedef typename QtJambiPrivate::AssociativeAccessSuperclassDecider<K,T,AbstractMapAccess>::type Super;
protected:
    QMapAccess(){}
public:
    static AbstractMapAccess* newInstance(){
        static QMapAccess<K, T> instance;
        return &instance;
    }

    AbstractMapAccess* clone() override{
        return this;
    }

    bool isDetached(const void* container) override{
        return reinterpret_cast<const QMap<K,T> *>(container)->isDetached();
    }

    void detach(const ContainerInfo& container) override{
        reinterpret_cast<QMap<K,T> *>(container.container)->detach();
    }

    bool isSharedWith(const void* container, const void* container2) override{
        return reinterpret_cast<const QMap<K,T> *>(container)->isSharedWith(*reinterpret_cast<const QMap<K,T> *>(container2));
    }

    void swap(JNIEnv *env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override{
        reinterpret_cast<QMap<K,T> *>(container.container)->swap(*reinterpret_cast<QMap<K,T> *>(container2.container));
        if constexpr ((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0 && (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if(container2.access!=this){
                if(AbstractReferenceCountingContainer* access = container2.access->asRC()){
                    if(access->asRCMap())
                        Super::swapRC(env, container, container2);
                    else
                        Super::updateRC(env, container);
                }else{
                    Super::updateRC(env, container);
                }
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<QMap<K,T>>::needsReferenceCounting){
            if(container2.access!=this){
                if(AbstractReferenceCountingContainer* access = container2.access->asRC()){
                    if(access->asRCSet())
                        Super::swapRC(env, container, container2);
                    else
                        Super::updateRC(env, container);
                }else{
                    Super::updateRC(env, container);
                }
            }
        }else{
            Q_UNUSED(env);
        }
    }

    const QMetaType& keyMetaType() override {
        static QMetaType type(QMetaType::fromType<std::remove_cv_t<K>>());
        return type;
    }
    const QMetaType& valueMetaType() override {
        static QMetaType type(QMetaType::fromType<std::remove_cv_t<T>>());
        return type;
    }

    AbstractContainerAccess::DataType keyType() override{
        return QtJambiPrivate::ContainerContentType<K>::type;
    }

    AbstractContainerAccess::DataType valueType() override{
        return QtJambiPrivate::ContainerContentType<T>::type;
    }

    AbstractContainerAccess* keyNestedContainerAccess() override {
        return QtJambiPrivate::ContainerContentType<K>::accessFactory();
    }

    AbstractContainerAccess* valueNestedContainerAccess() override {
        return QtJambiPrivate::ContainerContentType<T>::accessFactory();
    }

    bool hasKeyNestedContainerAccess() override {
        return QtJambiPrivate::ContainerContentType<K>::isContainer;
    }
    bool hasValueNestedContainerAccess() override {
        return QtJambiPrivate::ContainerContentType<T>::isContainer;
    }
    bool hasKeyNestedPointers() override {
        return QtJambiPrivate::ContainerContentType<K>::isContainer && QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting;
    }
    bool hasValueNestedPointers() override {
        return QtJambiPrivate::ContainerContentType<T>::isContainer && QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting;
    }

    void assign(void* container, const void* other) override {
        (*reinterpret_cast<QMap<K,T>*>(container)) = (*reinterpret_cast<const QMap<K,T>*>(other));
    }
    void assign(JNIEnv *env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override {
        (*reinterpret_cast<QMap<K,T>*>(container.container)) = (*reinterpret_cast<const QMap<K,T>*>(other.container));
        if constexpr ((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0 && (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if(other.access!=this){
                if(AbstractReferenceCountingContainer* access = other.access->asRC()){
                    if(access->asRCMap())
                        Super::assignRC(env, container.object, other.object);
                    else
                        Super::updateRC(env, container);
                }else{
                    Super::updateRC(env, container);
                }
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<QMap<K,T>>::needsReferenceCounting){
            if(other.access!=this){
                if(AbstractReferenceCountingContainer* access = other.access->asRC()){
                    if(access->asRCSet())
                        Super::assignRC(env, container.object, other.object);
                    else
                        Super::updateRC(env, container);
                }else{
                    Super::updateRC(env, container);
                }
            }
        }else{
            Q_UNUSED(env);
        }
    }
    size_t sizeOf() const override {
        return sizeof(QMap<K,T>);
    }
    size_t alignOf() const override {
        return alignof(QMap<K,T>);
    }
    void* constructContainer(void* placement) override {
        return new(placement) QMap<K,T>();
    }

    void* constructContainer(void* placement, const void* copyOf) override {
        return new(placement) QMap<K,T>(*reinterpret_cast<const QMap<K,T>*>(copyOf));
    }
    void* constructContainer(JNIEnv *, void* placement, const ConstContainerAndAccessInfo& copyOf) override {
        return constructContainer(placement, copyOf.container);
    }
    void* constructContainer(void* placement, void* move) override {
        return new(placement) QMap<K,T>(std::move(*reinterpret_cast<const QMap<K,T>*>(move)));
    }
    void* constructContainer(JNIEnv *, void* placement, const ContainerAndAccessInfo& move) override {
        return constructContainer(placement, move.container);
    }
    bool destructContainer(void* container) override {
        reinterpret_cast<QMap<K,T>*>(container)->~QMap<K,T>();
        return true;
    }

    QMetaType registerContainer(QByteArrayView containerTypeName) override {
        return QtJambiPrivate::container_registry_impl<QMap<K,T>>::register_container(containerTypeName, this);
    }

    jboolean contains(JNIEnv * env, const void* container, jobject key) override {
        return QtJambiPrivate::AssociativeContainerContains<QMap, K, T>::function(env, container, key);
    }

    bool contains(const void* container, const void* key) override {
        return QtJambiPrivate::AssociativeContainerContains<QMap, K, T>::function(container, key);
    }

    qsizetype count(JNIEnv * env, const void* container, jobject key) override {
        return QtJambiPrivate::AssociativeContainerCountObject<QMap, K, T>::function(env, container, key);
    }

    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        return QtJambiPrivate::AssociativeContainerConstEnd<QMap, K, T>::function(env, container);
    }

    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        return QtJambiPrivate::AssociativeContainerConstBegin<QMap, K, T>::function(env, container);
    }

    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override {
        return QtJambiPrivate::AssociativeContainerConstFindPair<QMap, K, T>::function(env, container, key);
    }

    jobject first(JNIEnv * env, const void* container) override {
        return QtJambiPrivate::AssociativeContainerFirst<QMap, K, T>::function(env, container);
    }

    jobject firstKey(JNIEnv * env, const void* container) override {
        return QtJambiPrivate::AssociativeContainerFirstKey<QMap, K, T>::function(env, container);
    }

    jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override {
        return QtJambiPrivate::AssociativeContainerKey<QMap, K, T>::function(env, container, value, defaultKey);
    }

    ContainerAndAccessInfo keys(JNIEnv * env, const ConstContainerInfo& container) override {
        return QtJambiPrivate::AssociativeContainerKeys<QMap, K, T>::function(env, container);
    }

    ContainerAndAccessInfo keys(JNIEnv * env, const ConstContainerInfo& container, jobject value) override {
        return QtJambiPrivate::AssociativeContainerKeysForValue<QMap, K, T>::function(env, container, value);
    }

    jobject last(JNIEnv * env, const void* container) override {
        return QtJambiPrivate::AssociativeContainerLast<QMap, K, T>::function(env, container);
    }

    jobject lastKey(JNIEnv * env, const void* container) override {
        return QtJambiPrivate::AssociativeContainerLastKey<QMap, K, T>::function(env, container);
    }

    jobject constLowerBound(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override {
        return QtJambiPrivate::AssociativeContainerLowerBound<QMap, K, T>::function(env, container, key);
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        return QtJambiPrivate::AssociativeContainerEquals<QMap, K, T, ContainerAPI::getAsQMap>::function(env, container, other);
    }

    qsizetype size(JNIEnv * env, const void* container) override {
        return QtJambiPrivate::AssociativeContainerSize<QMap, K, T>::function(env, container);
    }

    qsizetype size(const void* container) override {
        return QtJambiPrivate::AssociativeContainerSize<QMap, K, T>::function(container);
    }

    jobject constUpperBound(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override {
        return QtJambiPrivate::AssociativeContainerUpperBound<QMap, K, T>::function(env, container, key);
    }

    jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override {
        return QtJambiPrivate::AssociativeContainerValue<QMap, K, T>::function(env, container, key, defaultValue);
    }

    const void* value(const void* container, const void* key, const void* defaultValue) override {
        return QtJambiPrivate::AssociativeContainerValue<QMap, K, T>::function(container, key, defaultValue);
    }

    ContainerAndAccessInfo values(JNIEnv * env, const ConstContainerInfo& container) override {
        return QtJambiPrivate::AssociativeContainerValues<QMap, K, T>::function(env, container);
    }

    bool keyLessThan(JNIEnv *env, jobject key1, jobject key2) override {
        return QtJambiPrivate::AssociativeContainerElementLessThan<K>::function(env, key1, key2);
    }

    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override {
        return QtJambiPrivate::AssociativeContainerBegin<QMap, K, T>::function(env, container);
    }

    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override {
        return QtJambiPrivate::AssociativeContainerEnd<QMap, K, T>::function(env, container);
    }

    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key) override {
        return QtJambiPrivate::AssociativeContainerFindPair<QMap, K, T>::function(env, container, key);
    }

    void clear(JNIEnv * env, const ContainerInfo& container) override {
        QtJambiPrivate::AssociativeContainerClear<QMap, K, T>::function(env, container);
        if constexpr(QtJambiPrivate::ContainerContentType<QMap<K,T>>::needsReferenceCounting){
            Super::clearRC(env, container.object);
        }
    }

    void insert(JNIEnv *env, const ContainerInfo& container,jobject key,jobject _value) override {
        if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            QtJambiPrivate::AssociativeContainerInsert<QMap, K, T>::function(env, container, key, _value);
            Super::putRC(env, container.object, key, _value);
        }else if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                QtJambiPrivate::AssociativeContainerInsert<QMap, K, T>::function(env, container, key, _value);
                Super::addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
                Super::addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), _value);
            }else{
                QtJambiPrivate::AssociativeContainerInsert<QMap, K, T>::function(env, container, key, _value);
                Super::addUniqueRC(env, container.object, key);
            }
        }else if constexpr((QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                QtJambiPrivate::AssociativeContainerInsert<QMap, K, T>::function(env, container, key, _value);
                Super::addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
                Super::addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), _value);
            }else{
                jobject oldValue = value(env, container.container, key, nullptr);
                QtJambiPrivate::AssociativeContainerInsert<QMap, K, T>::function(env, container, key, _value);
                Super::removeRC(env, container.object, oldValue);
                Super::addRC(env, container.object, _value);
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting || QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            QtJambiPrivate::AssociativeContainerInsert<QMap, K, T>::function(env, container, key, _value);
            Super::addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
            Super::addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), _value);
        }else{
            QtJambiPrivate::AssociativeContainerInsert<QMap, K, T>::function(env, container, key, _value);
        }
    }

    void insert(void* container,const void* key,const void* _value) override {
        if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            QtJambiPrivate::AssociativeContainerInsert<QMap, K, T>::function(container, key, _value);
            if(JniEnvironment env{100}){
                if(jobject object = QtJambiAPI::findObject(env, container)){
                    if(jobject _key = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(key))){
                        if(jobject __value = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(_value))){
                            Super::putRC(env, object, _key, __value);
                        }
                    }
                }
            }
        }else if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                QtJambiPrivate::AssociativeContainerInsert<QMap, K, T>::function(container, key, _value);
                if(JniEnvironment env{100}){
                    if(jobject object = QtJambiAPI::findObject(env, container)){
                        if(jobject _key = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(key))){
                            Super::addNestedValueRC(env, object, keyType(), hasKeyNestedPointers(), _key);
                        }
                        if(jobject __value = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(_value))){
                            Super::addNestedValueRC(env, object, valueType(), hasValueNestedPointers(), __value);
                        }
                    }
                }
            }else{
                QtJambiPrivate::AssociativeContainerInsert<QMap, K, T>::function(container, key, _value);
                if(JniEnvironment env{100}){
                    if(jobject object = QtJambiAPI::findObject(env, container)){
                        if(jobject _key = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(key))){
                            Super::addUniqueRC(env, object, _key);
                        }
                    }
                }
            }
        }else if constexpr((QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                QtJambiPrivate::AssociativeContainerInsert<QMap, K, T>::function(container, key, _value);
                if(JniEnvironment env{100}){
                    if(jobject object = QtJambiAPI::findObject(env, container)){
                        if(jobject _key = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(key))){
                            if(jobject __value = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(_value))){
                                Super::addNestedValueRC(env, object, keyType(), hasKeyNestedPointers(), _key);
                                Super::addNestedValueRC(env, object, valueType(), hasValueNestedPointers(), __value);
                            }
                        }
                    }
                }
            }else{
                const void* oldValue = value(container, key, nullptr);
                QtJambiPrivate::AssociativeContainerInsert<QMap, K, T>::function(container, key, _value);
                if(JniEnvironment env{100}){
                    if(jobject object = QtJambiAPI::findObject(env, container)){
                        if(oldValue){
                            if(jobject __value = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(oldValue))){
                                Super::removeRC(env, object, __value);
                            }
                        }
                        if(jobject __value = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(_value))){
                            Super::addRC(env, object, __value);
                        }
                    }
                }
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting || QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            QtJambiPrivate::AssociativeContainerInsert<QMap, K, T>::function(container, key, _value);
            if(JniEnvironment env{100}){
                if(jobject object = QtJambiAPI::findObject(env, container)){
                    if(jobject _key = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(key))){
                        Super::addNestedValueRC(env, object, keyType(), hasKeyNestedPointers(), _key);
                    }
                    if(jobject __value = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(_value))){
                        Super::addNestedValueRC(env, object, valueType(), hasValueNestedPointers(), __value);
                    }
                }
            }
        }else{
            QtJambiPrivate::AssociativeContainerInsert<QMap, K, T>::function(container, key, _value);
        }
    }

    qsizetype remove(JNIEnv * env, const ContainerInfo& container,jobject key) override {
        qsizetype result;
        if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            result = QtJambiPrivate::AssociativeContainerRemove<QMap, K, T>::function(env, container, key);
            Super::removeRC(env, container.object, key, result);
        }else if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                result = QtJambiPrivate::AssociativeContainerRemove<QMap, K, T>::function(env, container, key);
                if(result>0)
                    Super::updateRC(env, container);
            }else{
                result = QtJambiPrivate::AssociativeContainerRemove<QMap, K, T>::function(env, container, key);
                if(result>0){
                    Super::removeRC(env, container.object, key);
                }
            }
        }else if constexpr((QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                result = QtJambiPrivate::AssociativeContainerRemove<QMap, K, T>::function(env, container, key);

            }else{
                jobject oldValue = value(env, container.container, key, nullptr);
                result = QtJambiPrivate::AssociativeContainerRemove<QMap, K, T>::function(env, container, key);
                if(result>0){
                    Super::removeRC(env, container.object, oldValue);
                }
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting || QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            result = QtJambiPrivate::AssociativeContainerRemove<QMap, K, T>::function(env, container, key);
            if(result>0)
                Super::updateRC(env, container);
        }else{
            result = QtJambiPrivate::AssociativeContainerRemove<QMap, K, T>::function(env, container, key);
        }
        return result;
    }

    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override {
        jobject result = QtJambiPrivate::AssociativeContainerTake<QMap, K, T>::function(env, container, key);
        if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            Super::removeRC(env, container.object, key);
        }else if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                Super::updateRC(env, container);
            }else{
                Super::removeRC(env, container.object, key);
            }
        }else if constexpr((QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                Super::updateRC(env, container);
            }else{
                Super::removeRC(env, container.object, result);
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting || QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            Super::updateRC(env, container);
        }
        return result;
    }
private:
    template<bool _isConst>
    class KeyValueIterator : public AbstractMapAccess::KeyValueIterator{
        using Container = std::conditional_t<_isConst, const QMap<K,T>, QMap<K,T>>;
        using iterator = decltype(std::declval<Container>().begin());
        QMapAccess<K,T>* m_access;
        iterator current;
        iterator end;
        KeyValueIterator(const KeyValueIterator& other)
            :m_access(other.m_access),
            current(other.current),
            end(other.end) {}
    protected:
        AbstractAssociativeAccess* access() override {return m_access;}
    public:
        KeyValueIterator(QMapAccess<K,T>* _access, Container& container)
            : m_access(_access),
            current(container.begin()),
            end(container.end()) {}
        ~KeyValueIterator() override {};
        bool hasNext() override {return current!=end;};
        QPair<jobject,jobject> next(JNIEnv * env) override {
            QPair<jobject,jobject> result{::qtjambi_cast<jobject>(env, current.key()),
                                          ::qtjambi_cast<jobject>(env, current.value())};
            ++current;
            return result;
        }
        QPair<const void*,const void*> next() override {
            QPair<const void*,const void*> result{QtJambiPrivate::ContainerContentDeref<K>::deref(current.key()), QtJambiPrivate::ContainerContentDeref<T>::deref(current.value())};
            ++current;
            return result;
        }
        bool isConst() override{
            return _isConst;
        }
        QPair<const void*,const void*> constNext() override {
            QPair<const void*,const void*> result{&current.key(), &current.value()};
            ++current;
            return result;
        }
        QPair<const void*,void*> mutableNext() override {
            if constexpr(!_isConst){
                QPair<const void*,void*> result{&current.key(), &current.value()};
                ++current;
                return result;
            }else{
                return {nullptr,nullptr};
            }
        }
        bool operator==(const AbstractMapAccess::KeyValueIterator& other) const override {
            return current==reinterpret_cast<const KeyValueIterator&>(other).current;
        }
        std::unique_ptr<AbstractAssociativeAccess::KeyValueIterator> clone() const override {
            return std::unique_ptr<AbstractAssociativeAccess::KeyValueIterator>(new KeyValueIterator(*this));
        }
        std::function<jobject(JNIEnv*,const void*)> keyConverter() const override {
            return [](JNIEnv* env,const void* pointer) -> jobject{
                return ::qtjambi_cast<jobject>(env, reinterpret_cast<const K*>(pointer));
            };
        }
        std::function<jobject(JNIEnv*,const void*)> valueConverter() const override {
            return [](JNIEnv* env,const void* pointer) -> jobject{
                return ::qtjambi_cast<jobject>(env, reinterpret_cast<const T*>(pointer));
            };
        }
    };
public:
    std::unique_ptr<AbstractMapAccess::KeyValueIterator> keyValueIterator(const void* container) override {
        return std::unique_ptr<AbstractMapAccess::KeyValueIterator>(new KeyValueIterator<true>(this, *reinterpret_cast<const QMap<K,T>*>(container)));
    }
    std::unique_ptr<AbstractMapAccess::KeyValueIterator> keyValueIterator(void* container) override {
        return std::unique_ptr<AbstractMapAccess::KeyValueIterator>(new KeyValueIterator<false>(this, *reinterpret_cast<QMap<K,T>*>(container)));
    }
};

template<typename K, typename T>
class QMultiMapAccess : public QtJambiPrivate::AssociativeAccessSuperclassDecider<K,T,AbstractMultiMapAccess>::type{
    typedef typename QtJambiPrivate::AssociativeAccessSuperclassDecider<K,T,AbstractMultiMapAccess>::type Super;
protected:
    QMultiMapAccess(){}
public:
    static AbstractMultiMapAccess* newInstance(){
        static QMultiMapAccess<K, T> instance;
        return &instance;
    }

    AbstractMultiMapAccess* clone() override{
        return this;
    }

    bool isDetached(const void* container) override{
        return reinterpret_cast<const QMultiMap<K,T> *>(container)->isDetached();
    }

    void detach(const ContainerInfo& container) override{
        reinterpret_cast<QMultiMap<K,T> *>(container.container)->detach();
    }

    bool isSharedWith(const void* container, const void* container2) override{
        return reinterpret_cast<const QMultiMap<K,T> *>(container)->isSharedWith(*reinterpret_cast<const QMultiMap<K,T> *>(container2));
    }

    void swap(JNIEnv *env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override{
        reinterpret_cast<QMultiMap<K,T> *>(container.container)->swap(*reinterpret_cast<QMultiMap<K,T> *>(container2.container));
        if constexpr ((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0 && (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if(container2.access!=this){
                if(AbstractReferenceCountingContainer* access = container2.access->asRC()){
                    if(access->asRCMultiMap())
                        Super::swapRC(env, container, container2);
                    else
                        Super::updateRC(env, container);
                }else{
                    Super::updateRC(env, container);
                }
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<QMultiMap<K,T>>::needsReferenceCounting){
            if(container2.access!=this){
                if(AbstractReferenceCountingContainer* access = container2.access->asRC()){
                    if(access->asRCSet())
                        Super::swapRC(env, container, container2);
                    else
                        Super::updateRC(env, container);
                }else{
                    Super::updateRC(env, container);
                }
            }
        }else{
            Q_UNUSED(env);
        }
    }

    const QMetaType& keyMetaType() override {
        static QMetaType type(QMetaType::fromType<std::remove_cv_t<K>>());
        return type;
    }

    const QMetaType& valueMetaType() override {
        static QMetaType type(QMetaType::fromType<std::remove_cv_t<T>>());
        return type;
    }

    AbstractContainerAccess::DataType keyType() override{
        return QtJambiPrivate::ContainerContentType<K>::type;
    }

    AbstractContainerAccess::DataType valueType() override{
        return QtJambiPrivate::ContainerContentType<T>::type;
    }

    AbstractContainerAccess* keyNestedContainerAccess() override {
        return QtJambiPrivate::ContainerContentType<K>::accessFactory();
    }

    AbstractContainerAccess* valueNestedContainerAccess() override {
        return QtJambiPrivate::ContainerContentType<T>::accessFactory();
    }

    bool hasKeyNestedContainerAccess() override {
        return QtJambiPrivate::ContainerContentType<K>::isContainer;
    }
    bool hasValueNestedContainerAccess() override {
        return QtJambiPrivate::ContainerContentType<T>::isContainer;
    }
    bool hasKeyNestedPointers() override {
        return QtJambiPrivate::ContainerContentType<K>::isContainer && QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting;
    }
    bool hasValueNestedPointers() override {
        return QtJambiPrivate::ContainerContentType<T>::isContainer && QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting;
    }

    size_t sizeOf() const override {
        return sizeof(QMultiMap<K,T>);
    }
    size_t alignOf() const override {
        return alignof(QMultiMap<K,T>);
    }

    void* constructContainer(void* placement) override {
        return new(placement) QMultiMap<K,T>();
    }

    void* constructContainer(void* placement, const void* copyOf) override {
        return new(placement) QMultiMap<K,T>(*reinterpret_cast<const QMultiMap<K,T>*>(copyOf));
    }
    void* constructContainer(JNIEnv *, void* placement, const ConstContainerAndAccessInfo& copyOf) override {
        return constructContainer(placement, copyOf.container);
    }
    void* constructContainer(void* placement, void* move) override {
        return new(placement) QMultiMap<K,T>(std::move(*reinterpret_cast<const QMultiMap<K,T>*>(move)));
    }
    void* constructContainer(JNIEnv *, void* placement, const ContainerAndAccessInfo& move) override {
        return constructContainer(placement, move.container);
    }
    bool destructContainer(void* container) override {
        reinterpret_cast<QMultiMap<K,T>*>(container)->~QMultiMap<K,T>();
        return true;
    }

    void assign(void* container, const void* other) override {
        (*reinterpret_cast<QMultiMap<K,T>*>(container)) = (*reinterpret_cast<const QMultiMap<K,T>*>(other));
    }

    void assign(JNIEnv *env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override {
        (*reinterpret_cast<QMultiMap<K,T>*>(container.container)) = (*reinterpret_cast<const QMultiMap<K,T>*>(other.container));
        if constexpr ((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0 && (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if(other.access!=this){
                if(AbstractReferenceCountingContainer* access = other.access->asRC()){
                    if(access->asRCMap())
                        Super::assignRC(env, container.object, other.object);
                    else
                        Super::updateRC(env, container);
                }else{
                    Super::updateRC(env, container);
                }
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<QMultiMap<K,T>>::needsReferenceCounting){
            if(other.access!=this){
                if(AbstractReferenceCountingContainer* access = other.access->asRC()){
                    if(access->asRCSet())
                        Super::assignRC(env, container.object, other.object);
                    else
                        Super::updateRC(env, container);
                }else{
                    Super::updateRC(env, container);
                }
            }
        }else{
            Q_UNUSED(env);
        }
    }

    QMetaType registerContainer(QByteArrayView containerTypeName) override {
        return QtJambiPrivate::container_registry_impl<QMultiMap<K,T>>::register_container(containerTypeName, this);
    }

    jboolean contains(JNIEnv * env, const void* container, jobject key) override {
        return QtJambiPrivate::AssociativeContainerContains<QMultiMap, K, T>::function(env, container, key);
    }

    bool contains(const void* container, const void* key) override {
        return QtJambiPrivate::AssociativeContainerContains<QMultiMap, K, T>::function(container, key);
    }

    qsizetype count(JNIEnv * env, const void* container, jobject key) override {
        return QtJambiPrivate::AssociativeContainerCountObject<QMultiMap, K, T>::function(env, container, key);
    }

    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        return QtJambiPrivate::AssociativeContainerConstEnd<QMultiMap, K, T>::function(env, container);
    }

    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        return QtJambiPrivate::AssociativeContainerConstBegin<QMultiMap, K, T>::function(env, container);
    }

    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override {
        return QtJambiPrivate::AssociativeContainerConstFindPair<QMultiMap, K, T>::function(env, container, key);
    }

    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key, jobject value) override {
        return QtJambiPrivate::AssociativeContainerConstFindPairs<QMultiMap, K, T>::function(env, container, key, value);
    }
    jobject first(JNIEnv * env, const void* container) override {
        return QtJambiPrivate::AssociativeContainerFirst<QMultiMap, K, T>::function(env, container);
    }

    jobject firstKey(JNIEnv * env, const void* container) override {
        return QtJambiPrivate::AssociativeContainerFirstKey<QMultiMap, K, T>::function(env, container);
    }

    jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override {
        return QtJambiPrivate::AssociativeContainerKey<QMultiMap, K, T>::function(env, container, value, defaultKey);
    }

    ContainerAndAccessInfo keys(JNIEnv * env, const ConstContainerInfo& container) override {
        return QtJambiPrivate::AssociativeContainerKeys<QMultiMap, K, T>::function(env, container);
    }

    ContainerAndAccessInfo keys(JNIEnv * env, const ConstContainerInfo& container, jobject value) override {
        return QtJambiPrivate::AssociativeContainerKeysForValue<QMultiMap, K, T>::function(env, container, value);
    }

    jobject last(JNIEnv * env, const void* container) override {
        return QtJambiPrivate::AssociativeContainerLast<QMultiMap, K, T>::function(env, container);
    }

    jobject lastKey(JNIEnv * env, const void* container) override {
        return QtJambiPrivate::AssociativeContainerLastKey<QMultiMap, K, T>::function(env, container);
    }

    jobject constLowerBound(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override {
        return QtJambiPrivate::AssociativeContainerLowerBound<QMultiMap, K, T>::function(env, container, key);
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        return QtJambiPrivate::AssociativeContainerEquals<QMultiMap, K, T, ContainerAPI::getAsQMultiMap>::function(env, container, other);
    }

    qsizetype size(JNIEnv * env, const void* container) override {
        return QtJambiPrivate::AssociativeContainerSize<QMultiMap, K, T>::function(env, container);
    }

    qsizetype size(const void* container) override {
        return QtJambiPrivate::AssociativeContainerSize<QMultiMap, K, T>::function(container);
    }

    ContainerAndAccessInfo uniqueKeys(JNIEnv * env, const ConstContainerInfo& container) override {
        return QtJambiPrivate::AssociativeContainerUniqueKeys<QMultiMap, K, T>::function(env, container);
    }

    ContainerAndAccessInfo values(JNIEnv * env, const ConstContainerInfo& container, jobject key) override {
        return QtJambiPrivate::AssociativeContainerValuesKey<QMultiMap, K, T>::function(env, container, key);
    }

    jobject constUpperBound(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override {
        return QtJambiPrivate::AssociativeContainerUpperBound<QMultiMap, K, T>::function(env, container, key);
    }

    jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override {
        return QtJambiPrivate::AssociativeContainerValue<QMultiMap, K, T>::function(env, container, key, defaultValue);
    }

    const void* value(const void* container, const void* key, const void* defaultValue) override {
        return QtJambiPrivate::AssociativeContainerValue<QMultiMap, K, T>::function(container, key, defaultValue);
    }

    ContainerAndAccessInfo values(JNIEnv * env, const ConstContainerInfo& container) override {
        return QtJambiPrivate::AssociativeContainerValues<QMultiMap, K, T>::function(env, container);
    }

    jboolean contains(JNIEnv *env, const void* container, jobject key, jobject value) override {
        return QtJambiPrivate::AssociativeContainerContainsPair<QMultiMap, K, T>::function(env, container, key, value);
    }

    qsizetype count(JNIEnv *env, const void* container, jobject key, jobject value) override {
        return QtJambiPrivate::AssociativeContainerCountPair<QMultiMap, K, T>::function(env, container, key, value);
    }

    bool keyLessThan(JNIEnv *env, jobject key1, jobject key2) override {
        return QtJambiPrivate::AssociativeContainerElementLessThan<K>::function(env, key1, key2);
    }

    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override {
        return QtJambiPrivate::AssociativeContainerBegin<QMultiMap, K, T>::function(env, container);
    }

    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override {
        return QtJambiPrivate::AssociativeContainerEnd<QMultiMap, K, T>::function(env, container);
    }

    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key) override {
        return QtJambiPrivate::AssociativeContainerFindPair<QMultiMap, K, T>::function(env, container, key);
    }

    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key, jobject value) override {
        return QtJambiPrivate::AssociativeContainerFindPairs<QMultiMap, K, T>::function(env, container, key, value);
    }

    qsizetype remove(JNIEnv * env, const ContainerInfo& container,jobject key, jobject _value) override {
        qsizetype result;
        if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            result = QtJambiPrivate::AssociativeContainerRemovePair<QMultiMap, K, T>::function(env, container, key, _value);
            Super::removeRC(env, container.object, key, _value, result);
        }else if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                result = QtJambiPrivate::AssociativeContainerRemovePair<QMultiMap, K, T>::function(env, container, key, _value);
                if(result>0)
                    Super::updateRC(env, container);
            }else{
                result = QtJambiPrivate::AssociativeContainerRemovePair<QMultiMap, K, T>::function(env, container, key, _value);
                if(result>0){
                    Super::removeRC(env, container.object, key);
                }
            }
        }else if constexpr((QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                result = QtJambiPrivate::AssociativeContainerRemovePair<QMultiMap, K, T>::function(env, container, key, _value);

            }else{
                result = QtJambiPrivate::AssociativeContainerRemovePair<QMultiMap, K, T>::function(env, container, key, _value);
                Super::removeRC(env, container.object, _value, result);
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting || QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            result = QtJambiPrivate::AssociativeContainerRemovePair<QMultiMap, K, T>::function(env, container, key, _value);
            if(result>0)
                Super::updateRC(env, container);
        }else{
            result = QtJambiPrivate::AssociativeContainerRemovePair<QMultiMap, K, T>::function(env, container, key, _value);
        }
        return result;
    }

    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject _value) override {
        if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            QtJambiPrivate::AssociativeContainerReplacePair<QMultiMap, K, T>::function(env, container, key, _value);
            Super::removeRC(env, container.object, key);
            Super::putRC(env, container.object, key, _value);
        }else if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                QtJambiPrivate::AssociativeContainerReplacePair<QMultiMap, K, T>::function(env, container, key, _value);
                Super::updateRC(env, container);
            }else{
                QtJambiPrivate::AssociativeContainerReplacePair<QMultiMap, K, T>::function(env, container, key, _value);
                Super::addRC(env, container.object, key);
            }
        }else if constexpr((QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                QtJambiPrivate::AssociativeContainerReplacePair<QMultiMap, K, T>::function(env, container, key, _value);

            }else{
                jobject oldValue = value(env, container.container, key, nullptr);
                QtJambiPrivate::AssociativeContainerReplacePair<QMultiMap, K, T>::function(env, container, key, _value);
                Super::removeRC(env, container.object, oldValue);
                Super::addRC(env, container.object, _value);
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting || QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            QtJambiPrivate::AssociativeContainerReplacePair<QMultiMap, K, T>::function(env, container, key, _value);
            Super::updateRC(env, container);
        }else{
            QtJambiPrivate::AssociativeContainerReplacePair<QMultiMap, K, T>::function(env, container, key, _value);
        }
    }

    void clear(JNIEnv * env, const ContainerInfo& container) override {
        QtJambiPrivate::AssociativeContainerClear<QMultiMap, K, T>::function(env, container);
        if constexpr(QtJambiPrivate::ContainerContentType<QMultiMap<K,T>>::needsReferenceCounting){
            Super::clearRC(env, container.object);
        }
    }

    void insert(JNIEnv *env, const ContainerInfo& container,jobject key,jobject _value) override {
        if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            QtJambiPrivate::AssociativeContainerInsert<QMultiMap, K, T>::function(env, container, key, _value);
            Super::putRC(env, container.object, key, _value);
        }else if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                QtJambiPrivate::AssociativeContainerInsert<QMultiMap, K, T>::function(env, container, key, _value);
                Super::addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
                Super::addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), _value);
            }else{
                QtJambiPrivate::AssociativeContainerInsert<QMultiMap, K, T>::function(env, container, key, _value);
                Super::addUniqueRC(env, container.object, key);
            }
        }else if constexpr((QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                QtJambiPrivate::AssociativeContainerInsert<QMultiMap, K, T>::function(env, container, key, _value);
                Super::addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
                Super::addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), _value);
            }else{
                jobject oldValue = value(env, container.container, key, nullptr);
                QtJambiPrivate::AssociativeContainerInsert<QMultiMap, K, T>::function(env, container, key, _value);
                Super::removeRC(env, container.object, oldValue);
                Super::addRC(env, container.object, _value);
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting || QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            QtJambiPrivate::AssociativeContainerInsert<QMultiMap, K, T>::function(env, container, key, _value);
            Super::addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
            Super::addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), _value);
        }else{
            QtJambiPrivate::AssociativeContainerInsert<QMultiMap, K, T>::function(env, container, key, _value);
        }
    }

    void insert(void* container,const void* key,const void* _value) override {
        if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            QtJambiPrivate::AssociativeContainerInsert<QMultiMap, K, T>::function(container, key, _value);
            if(JniEnvironment env{100}){
                if(jobject object = QtJambiAPI::findObject(env, container)){
                    if(jobject _key = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(key))){
                        if(jobject __value = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(_value))){
                            Super::putRC(env, object, _key, __value);
                        }
                    }
                }
            }
        }else if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                QtJambiPrivate::AssociativeContainerInsert<QMultiMap, K, T>::function(container, key, _value);
                if(JniEnvironment env{100}){
                    if(jobject object = QtJambiAPI::findObject(env, container)){
                        if(jobject _key = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(key))){
                            Super::addNestedValueRC(env, object, keyType(), hasKeyNestedPointers(), _key);
                        }
                        if(jobject __value = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(_value))){
                            Super::addNestedValueRC(env, object, valueType(), hasValueNestedPointers(), __value);
                        }
                    }
                }
            }else{
                QtJambiPrivate::AssociativeContainerInsert<QMultiMap, K, T>::function(container, key, _value);
                if(JniEnvironment env{100}){
                    if(jobject object = QtJambiAPI::findObject(env, container)){
                        if(jobject _key = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(key))){
                            Super::addUniqueRC(env, object, _key);
                        }
                    }
                }
            }
        }else if constexpr((QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                QtJambiPrivate::AssociativeContainerInsert<QMultiMap, K, T>::function(container, key, _value);
                if(JniEnvironment env{100}){
                    if(jobject object = QtJambiAPI::findObject(env, container)){
                        if(jobject _key = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(key))){
                            if(jobject __value = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(_value))){
                                Super::addNestedValueRC(env, object, keyType(), hasKeyNestedPointers(), _key);
                                Super::addNestedValueRC(env, object, valueType(), hasValueNestedPointers(), __value);
                            }
                        }
                    }
                }
            }else{
                const void* oldValue = value(container, key, nullptr);
                QtJambiPrivate::AssociativeContainerInsert<QMultiMap, K, T>::function(container, key, _value);
                if(JniEnvironment env{100}){
                    if(jobject object = QtJambiAPI::findObject(env, container)){
                        if(oldValue){
                            if(jobject __value = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(oldValue))){
                                Super::removeRC(env, object, __value);
                            }
                        }
                        if(jobject __value = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(_value))){
                            Super::addRC(env, object, __value);
                        }
                    }
                }
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting || QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            QtJambiPrivate::AssociativeContainerInsert<QMultiMap, K, T>::function(container, key, _value);
            if(JniEnvironment env{100}){
                if(jobject object = QtJambiAPI::findObject(env, container)){
                    if(jobject _key = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(key))){
                        Super::addNestedValueRC(env, object, keyType(), hasKeyNestedPointers(), _key);
                    }
                    if(jobject __value = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(_value))){
                        Super::addNestedValueRC(env, object, valueType(), hasValueNestedPointers(), __value);
                    }
                }
            }
        }else{
            QtJambiPrivate::AssociativeContainerInsert<QMultiMap, K, T>::function(container, key, _value);
        }
    }

    qsizetype remove(JNIEnv * env, const ContainerInfo& container,jobject key) override {
        qsizetype result;
        if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            result = QtJambiPrivate::AssociativeContainerRemove<QMultiMap, K, T>::function(env, container, key);
            Super::removeRC(env, container.object, key, result);
        }else if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                result = QtJambiPrivate::AssociativeContainerRemove<QMultiMap, K, T>::function(env, container, key);
                if(result>0)
                    Super::updateRC(env, container);
            }else{
                result = QtJambiPrivate::AssociativeContainerRemove<QMultiMap, K, T>::function(env, container, key);
                if(result>0){
                    Super::removeRC(env, container.object, key);
                }
            }
        }else if constexpr((QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                result = QtJambiPrivate::AssociativeContainerRemove<QMultiMap, K, T>::function(env, container, key);

            }else{
                jobject oldValue = value(env, container.container, key, nullptr);
                result = QtJambiPrivate::AssociativeContainerRemove<QMultiMap, K, T>::function(env, container, key);
                if(result>0){
                    Super::removeRC(env, container.object, oldValue);
                }
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting || QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            result = QtJambiPrivate::AssociativeContainerRemove<QMultiMap, K, T>::function(env, container, key);
            if(result>0)
                Super::updateRC(env, container);
        }else{
            result = QtJambiPrivate::AssociativeContainerRemove<QMultiMap, K, T>::function(env, container, key);
        }
        return result;
    }

    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override {
        jobject result = QtJambiPrivate::AssociativeContainerTake<QMultiMap, K, T>::function(env, container, key);
        if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            Super::removeRC(env, container.object, key);
        }else if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                Super::updateRC(env, container);
            }else{
                Super::removeRC(env, container.object, key);
            }
        }else if constexpr((QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                Super::updateRC(env, container);
            }else{
                Super::removeRC(env, container.object, result);
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting || QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            Super::updateRC(env, container);
        }
        return result;
    }

    void unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other) override {
        QtJambiPrivate::AssociativeContainerUnite<QMultiMap, K, T, ContainerAPI::getAsQMultiMap>::function(env, container, other);
        if constexpr(QtJambiPrivate::ContainerContentType<QMultiMap<K,T>>::needsReferenceCounting){
            Super::updateRC(env, container);
        }
    }
private:
    template<bool _isConst>
    class KeyValueIterator : public AbstractMapAccess::KeyValueIterator{
        using Container = std::conditional_t<_isConst, const QMultiMap<K,T>, QMultiMap<K,T>>;
        using iterator = decltype(std::declval<Container>().begin());
        QMultiMapAccess* m_access;
        iterator current;
        iterator end;
        KeyValueIterator(const KeyValueIterator& other)
            :m_access(other.m_access),
            current(other.current),
            end(other.end) {}
    protected:
        AbstractAssociativeAccess* access() override {return m_access;}
    public:
        KeyValueIterator(QMultiMapAccess* _access, Container& container)
            : m_access(_access){
            current = container.begin();
            end = container.end();
        }
        ~KeyValueIterator() override {};
        bool hasNext() override {return current!=end;};
        QPair<jobject,jobject> next(JNIEnv * env) override {
            QPair<jobject,jobject> result{::qtjambi_cast<jobject>(env, current.key()),
                                          ::qtjambi_cast<jobject>(env, current.value())};
            ++current;
            return result;
        }
        QPair<const void*,const void*> next() override {
            QPair<const void*,const void*> result{QtJambiPrivate::ContainerContentDeref<K>::deref(current.key()), QtJambiPrivate::ContainerContentDeref<T>::deref(current.value())};
            ++current;
            return result;
        }
        bool isConst() override{
            return _isConst;
        }
        QPair<const void*,const void*> constNext() override {
            QPair<const void*,const void*> result{&current.key(), &current.value()};
            ++current;
            return result;
        }
        QPair<const void*,void*> mutableNext() override {
            if constexpr(!_isConst){
                QPair<const void*,void*> result{&current.key(), &current.value()};
                ++current;
                return result;
            }else{
                return {nullptr,nullptr};
            }
        }
        bool operator==(const AbstractMapAccess::KeyValueIterator& other) const override {
            return current==reinterpret_cast<const KeyValueIterator&>(other).current;
        }
        std::unique_ptr<AbstractAssociativeAccess::KeyValueIterator> clone() const override {
            return std::unique_ptr<AbstractAssociativeAccess::KeyValueIterator>(new KeyValueIterator(*this));
        }
        std::function<jobject(JNIEnv*,const void*)> keyConverter() const override {
            return [](JNIEnv* env,const void* pointer) -> jobject{
                return ::qtjambi_cast<jobject>(env, reinterpret_cast<const K*>(pointer));
            };
        }
        std::function<jobject(JNIEnv*,const void*)> valueConverter() const override {
            return [](JNIEnv* env,const void* pointer) -> jobject{
                return ::qtjambi_cast<jobject>(env, reinterpret_cast<const T*>(pointer));
            };
        }
    };
public:
    std::unique_ptr<AbstractMapAccess::KeyValueIterator> keyValueIterator(const void* container) override {
        return std::unique_ptr<AbstractMapAccess::KeyValueIterator>(new KeyValueIterator<true>(this, *reinterpret_cast<const QMultiMap<K,T>*>(container)));
    }
    std::unique_ptr<AbstractMapAccess::KeyValueIterator> keyValueIterator(void* container) override {
        return std::unique_ptr<AbstractMapAccess::KeyValueIterator>(new KeyValueIterator<false>(this, *reinterpret_cast<QMultiMap<K,T>*>(container)));
    }
};

template<typename K, typename T>
class QHashAccess : public QtJambiPrivate::AssociativeAccessSuperclassDecider<K,T,AbstractHashAccess>::type{
    typedef typename QtJambiPrivate::AssociativeAccessSuperclassDecider<K,T,AbstractHashAccess>::type Super;
protected:
    QHashAccess(){}
public:
    static AbstractHashAccess* newInstance(){
        static QHashAccess<K, T> instance;
        return &instance;
    }

    AbstractHashAccess* clone() override{
        return this;
    }

    bool isDetached(const void* container) override{
        return reinterpret_cast<const QHash<K,T> *>(container)->isDetached();
    }

    void detach(const ContainerInfo& container) override{
        reinterpret_cast<QHash<K,T> *>(container.container)->detach();
    }

    bool isSharedWith(const void* container, const void* container2) override{
        return reinterpret_cast<const QHash<K,T> *>(container)->isSharedWith(*reinterpret_cast<const QHash<K,T> *>(container2));
    }

    void swap(JNIEnv *env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override{
        reinterpret_cast<QHash<K,T> *>(container.container)->swap(*reinterpret_cast<QHash<K,T> *>(container2.container));
        if constexpr ((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0 && (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if(container2.access!=this){
                if(AbstractReferenceCountingContainer* access = container2.access->asRC()){
                    if(access->asRCMap())
                        Super::swapRC(env, container, container2);
                    else
                        Super::updateRC(env, container);
                }else{
                    Super::updateRC(env, container);
                }
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<QHash<K,T>>::needsReferenceCounting){
            if(container2.access!=this){
                if(AbstractReferenceCountingContainer* access = container2.access->asRC()){
                    if(access->asRCSet())
                        Super::swapRC(env, container, container2);
                    else
                        Super::updateRC(env, container);
                }else{
                    Super::updateRC(env, container);
                }
            }
        }else{
            Q_UNUSED(env);
        }
    }

    const QMetaType& keyMetaType() override {
        static QMetaType type(QMetaType::fromType<std::remove_cv_t<K>>());
        return type;
    }

    const QMetaType& valueMetaType() override {
        static QMetaType type(QMetaType::fromType<std::remove_cv_t<T>>());
        return type;
    }

    AbstractContainerAccess::DataType keyType() override{
        return QtJambiPrivate::ContainerContentType<K>::type;
    }

    AbstractContainerAccess::DataType valueType() override{
        return QtJambiPrivate::ContainerContentType<T>::type;
    }

    AbstractContainerAccess* keyNestedContainerAccess() override {
        return QtJambiPrivate::ContainerContentType<K>::accessFactory();
    }

    AbstractContainerAccess* valueNestedContainerAccess() override {
        return QtJambiPrivate::ContainerContentType<T>::accessFactory();
    }

    bool hasKeyNestedContainerAccess() override {
        return QtJambiPrivate::ContainerContentType<K>::isContainer;
    }
    bool hasValueNestedContainerAccess() override {
        return QtJambiPrivate::ContainerContentType<T>::isContainer;
    }
    bool hasKeyNestedPointers() override {
        return QtJambiPrivate::ContainerContentType<K>::isContainer && QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting;
    }
    bool hasValueNestedPointers() override {
        return QtJambiPrivate::ContainerContentType<T>::isContainer && QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting;
    }

    void assign(void* container, const void* other) override {
        (*reinterpret_cast<QHash<K,T>*>(container)) = (*reinterpret_cast<const QHash<K,T>*>(other));
    }

    void assign(JNIEnv *env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override {
        (*reinterpret_cast<QHash<K,T>*>(container.container)) = (*reinterpret_cast<const QHash<K,T>*>(other.container));
        if constexpr ((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0 && (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if(other.access!=this){
                if(AbstractReferenceCountingContainer* access = other.access->asRC()){
                    if(access->asRCMap())
                        Super::assignRC(env, container.object, other.object);
                    else
                        Super::updateRC(env, container);
                }else{
                    Super::updateRC(env, container);
                }
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<QHash<K,T>>::needsReferenceCounting){
            if(other.access!=this){
                if(AbstractReferenceCountingContainer* access = other.access->asRC()){
                    if(access->asRCSet())
                        Super::assignRC(env, container.object, other.object);
                    else
                        Super::updateRC(env, container);
                }else{
                    Super::updateRC(env, container);
                }
            }
        }else{
            Q_UNUSED(env);
        }
    }

    QMetaType registerContainer(QByteArrayView containerTypeName) override {
        return QtJambiPrivate::container_registry_impl<QHash<K,T>>::register_container(containerTypeName, this);
    }

    size_t sizeOf() const override {
        return sizeof(QHash<K,T>);
    }
    size_t alignOf() const override {
        return alignof(QHash<K,T>);
    }
    void* constructContainer(void* placement) override {
        return new(placement) QHash<K,T>();
    }
    void* constructContainer(void* placement, const void* copyOf) override {
        return new(placement) QHash<K,T>(*reinterpret_cast<const QHash<K,T>*>(copyOf));
    }
    void* constructContainer(JNIEnv *, void* placement, const ConstContainerAndAccessInfo& copyOf) override {
        return constructContainer(placement, copyOf.container);
    }
    void* constructContainer(void* placement, void* move) override {
        return new(placement) QHash<K,T>(std::move(*reinterpret_cast<const QHash<K,T>*>(move)));
    }
    void* constructContainer(JNIEnv *, void* placement, const ContainerAndAccessInfo& move) override {
        return constructContainer(placement, move.container);
    }
    bool destructContainer(void* container) override {
        reinterpret_cast<QHash<K,T>*>(container)->~QHash<K,T>();
        return true;
    }

    qsizetype capacity(JNIEnv * env, const void* container) override {
        return QtJambiPrivate::AssociativeContainerCapacity<QHash, K, T>::function(env, container);
    }

    jboolean contains(JNIEnv * env, const void* container, jobject key) override {
        return QtJambiPrivate::AssociativeContainerContains<QHash, K, T>::function(env, container, key);
    }

    bool contains(const void* container, const void* key) override {
        return QtJambiPrivate::AssociativeContainerContains<QHash, K, T>::function(container, key);
    }

    qsizetype count(JNIEnv * env, const void* container, jobject key) override {
        return QtJambiPrivate::AssociativeContainerCountObject<QHash, K, T>::function(env, container, key);
    }

    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        return QtJambiPrivate::AssociativeContainerConstEnd<QHash, K, T>::function(env, container);
    }

    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        return QtJambiPrivate::AssociativeContainerConstBegin<QHash, K, T>::function(env, container);
    }

    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override {
        return QtJambiPrivate::AssociativeContainerConstFindPair<QHash, K, T>::function(env, container, key);
    }

    jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override {
        return QtJambiPrivate::AssociativeContainerKey<QHash, K, T>::function(env, container, value, defaultKey);
    }

    ContainerAndAccessInfo keys(JNIEnv * env, const ConstContainerInfo& container) override {
        return QtJambiPrivate::AssociativeContainerKeys<QHash, K, T>::function(env, container);
    }

    ContainerAndAccessInfo keys(JNIEnv * env, const ConstContainerInfo& container, jobject value) override {
        return QtJambiPrivate::AssociativeContainerKeysForValue<QHash, K, T>::function(env, container, value);
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        return QtJambiPrivate::AssociativeContainerEquals<QHash, K, T, ContainerAPI::getAsQHash>::function(env, container, other);
    }

    qsizetype size(JNIEnv * env, const void* container) override {
        return QtJambiPrivate::AssociativeContainerSize<QHash, K, T>::function(env, container);
    }

    qsizetype size(const void* container) override {
        return QtJambiPrivate::AssociativeContainerSize<QHash, K, T>::function(container);
    }

    jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override {
        return QtJambiPrivate::AssociativeContainerValue<QHash, K, T>::function(env, container, key, defaultValue);
    }

    const void* value(const void* container, const void* key, const void* defaultValue) override {
        return QtJambiPrivate::AssociativeContainerValue<QHash, K, T>::function(container, key, defaultValue);
    }

    ContainerAndAccessInfo values(JNIEnv * env, const ConstContainerInfo& container) override {
        return QtJambiPrivate::AssociativeContainerValues<QHash, K, T>::function(env, container);
    }

    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override {
        return QtJambiPrivate::AssociativeContainerBegin<QHash, K, T>::function(env, container);
    }
    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override {
        return QtJambiPrivate::AssociativeContainerEnd<QHash, K, T>::function(env, container);
    }
    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key) override {
        return QtJambiPrivate::AssociativeContainerFindPair<QHash, K, T>::function(env, container, key);
    }
    void clear(JNIEnv * env, const ContainerInfo& container) override {
        QtJambiPrivate::AssociativeContainerClear<QHash, K, T>::function(env, container);
        if constexpr(QtJambiPrivate::ContainerContentType<QHash<K,T>>::needsReferenceCounting){
            Super::clearRC(env, container.object);
        }
    }
    void insert(JNIEnv *env, const ContainerInfo& container,jobject key,jobject _value) override {
        if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            QtJambiPrivate::AssociativeContainerInsert<QHash, K, T>::function(env, container, key, _value);
            Super::putRC(env, container.object, key, _value);
        }else if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                QtJambiPrivate::AssociativeContainerInsert<QHash, K, T>::function(env, container, key, _value);
                Super::addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
                Super::addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), _value);
            }else{
                QtJambiPrivate::AssociativeContainerInsert<QHash, K, T>::function(env, container, key, _value);
                Super::addUniqueRC(env, container.object, key);
            }
        }else if constexpr((QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                QtJambiPrivate::AssociativeContainerInsert<QHash, K, T>::function(env, container, key, _value);
                Super::addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
                Super::addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), _value);
            }else{
                jobject oldValue = value(env, container.container, key, nullptr);
                QtJambiPrivate::AssociativeContainerInsert<QHash, K, T>::function(env, container, key, _value);
                Super::removeRC(env, container.object, oldValue);
                Super::addRC(env, container.object, _value);
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting || QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            QtJambiPrivate::AssociativeContainerInsert<QHash, K, T>::function(env, container, key, _value);
            Super::addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
            Super::addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), _value);
        }else{
            QtJambiPrivate::AssociativeContainerInsert<QHash, K, T>::function(env, container, key, _value);
        }
    }

    void insert(void* container,const void* key,const void* _value) override {
        if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            QtJambiPrivate::AssociativeContainerInsert<QHash, K, T>::function(container, key, _value);
            if(JniEnvironment env{100}){
                if(jobject object = QtJambiAPI::findObject(env, container)){
                    if(jobject _key = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(key))){
                        if(jobject __value = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(_value))){
                            Super::putRC(env, object, _key, __value);
                        }
                    }
                }
            }
        }else if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                QtJambiPrivate::AssociativeContainerInsert<QHash, K, T>::function(container, key, _value);
                if(JniEnvironment env{100}){
                    if(jobject object = QtJambiAPI::findObject(env, container)){
                        if(jobject _key = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(key))){
                            Super::addNestedValueRC(env, object, keyType(), hasKeyNestedPointers(), _key);
                        }
                        if(jobject __value = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(_value))){
                            Super::addNestedValueRC(env, object, valueType(), hasValueNestedPointers(), __value);
                        }
                    }
                }
            }else{
                QtJambiPrivate::AssociativeContainerInsert<QHash, K, T>::function(container, key, _value);
                if(JniEnvironment env{100}){
                    if(jobject object = QtJambiAPI::findObject(env, container)){
                        if(jobject _key = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(key))){
                            Super::addUniqueRC(env, object, _key);
                        }
                    }
                }
            }
        }else if constexpr((QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                QtJambiPrivate::AssociativeContainerInsert<QHash, K, T>::function(container, key, _value);
                if(JniEnvironment env{100}){
                    if(jobject object = QtJambiAPI::findObject(env, container)){
                        if(jobject _key = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(key))){
                            if(jobject __value = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(_value))){
                                Super::addNestedValueRC(env, object, keyType(), hasKeyNestedPointers(), _key);
                                Super::addNestedValueRC(env, object, valueType(), hasValueNestedPointers(), __value);
                            }
                        }
                    }
                }
            }else{
                const void* oldValue = value(container, key, nullptr);
                QtJambiPrivate::AssociativeContainerInsert<QHash, K, T>::function(container, key, _value);
                if(JniEnvironment env{100}){
                    if(jobject object = QtJambiAPI::findObject(env, container)){
                        if(oldValue){
                            if(jobject __value = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(oldValue))){
                                Super::removeRC(env, object, __value);
                            }
                        }
                        if(jobject __value = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(_value))){
                            Super::addRC(env, object, __value);
                        }
                    }
                }
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting || QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            QtJambiPrivate::AssociativeContainerInsert<QHash, K, T>::function(container, key, _value);
            if(JniEnvironment env{100}){
                if(jobject object = QtJambiAPI::findObject(env, container)){
                    if(jobject _key = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(key))){
                        Super::addNestedValueRC(env, object, keyType(), hasKeyNestedPointers(), _key);
                    }
                    if(jobject __value = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(_value))){
                        Super::addNestedValueRC(env, object, valueType(), hasValueNestedPointers(), __value);
                    }
                }
            }
        }else{
            QtJambiPrivate::AssociativeContainerInsert<QHash, K, T>::function(container, key, _value);
        }
    }

    qsizetype remove(JNIEnv * env, const ContainerInfo& container,jobject key) override {
        qsizetype result;
        if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            result = QtJambiPrivate::AssociativeContainerRemove<QHash, K, T>::function(env, container, key);
            Super::removeRC(env, container.object, key, result);
        }else if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                result = QtJambiPrivate::AssociativeContainerRemove<QHash, K, T>::function(env, container, key);
                if(result>0)
                    Super::updateRC(env, container);
            }else{
                result = QtJambiPrivate::AssociativeContainerRemove<QHash, K, T>::function(env, container, key);
                if(result>0){
                    Super::removeRC(env, container.object, key);
                }
            }
        }else if constexpr((QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                result = QtJambiPrivate::AssociativeContainerRemove<QHash, K, T>::function(env, container, key);

            }else{
                jobject oldValue = value(env, container.container, key, nullptr);
                result = QtJambiPrivate::AssociativeContainerRemove<QHash, K, T>::function(env, container, key);
                if(result>0){
                    Super::removeRC(env, container.object, oldValue);
                }
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting || QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            result = QtJambiPrivate::AssociativeContainerRemove<QHash, K, T>::function(env, container, key);
            if(result>0)
                Super::updateRC(env, container);
        }else{
            result = QtJambiPrivate::AssociativeContainerRemove<QHash, K, T>::function(env, container, key);
        }
        return result;
    }
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override {
        jobject result = QtJambiPrivate::AssociativeContainerTake<QHash, K, T>::function(env, container, key);
        if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            Super::removeRC(env, container.object, key);
        }else if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                Super::updateRC(env, container);
            }else{
                Super::removeRC(env, container.object, key);
            }
        }else if constexpr((QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                Super::updateRC(env, container);
            }else{
                Super::removeRC(env, container.object, result);
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting || QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            Super::updateRC(env, container);
        }
        return result;
    }
    void reserve(JNIEnv * env, const ContainerInfo& container, qsizetype newSize) override {
        QtJambiPrivate::AssociativeContainerReserve<QHash, K, T>::function(env, container, newSize);
    }
private:
    template<bool _isConst>
    class KeyValueIterator : public AbstractHashAccess::KeyValueIterator{
        using Container = std::conditional_t<_isConst, const QHash<K,T>, QHash<K,T>>;
        using iterator = decltype(std::declval<Container>().begin());
        QHashAccess<K,T>* m_access;
        iterator current;
        iterator end;
        KeyValueIterator(const KeyValueIterator& other)
            :m_access(other.m_access),
            current(other.current),
            end(other.end) {}
    protected:
        AbstractAssociativeAccess* access() override {return m_access;}
    public:
        KeyValueIterator(QHashAccess<K,T>* _access, Container& container)
            : m_access(_access), current(container.begin()),
              end(container.end()) {}
        ~KeyValueIterator() override {};
        bool hasNext() override {return current!=end;};
        QPair<jobject,jobject> next(JNIEnv * env) override {
            QPair<jobject,jobject> result{::qtjambi_cast<jobject>(env, current.key()),
                                          ::qtjambi_cast<jobject>(env, current.value())};
            ++current;
            return result;
        }
        QPair<const void*,const void*> next() override {
            QPair<const void*,const void*> result{QtJambiPrivate::ContainerContentDeref<K>::deref(current.key()), QtJambiPrivate::ContainerContentDeref<T>::deref(current.value())};
            ++current;
            return result;
        }
        bool isConst() override{
            return _isConst;
        }
        QPair<const void*,const void*> constNext() override {
            QPair<const void*,const void*> result{&current.key(), &current.value()};
            ++current;
            return result;
        }
        QPair<const void*,void*> mutableNext() override {
            if constexpr(!_isConst){
                QPair<const void*,void*> result{&current.key(), &current.value()};
                ++current;
                return result;
            }else{
                return {nullptr,nullptr};
            }
        }
        bool operator==(const AbstractMapAccess::KeyValueIterator& other) const override {
            return current==reinterpret_cast<const KeyValueIterator&>(other).current;
        }
        std::unique_ptr<AbstractAssociativeAccess::KeyValueIterator> clone() const override {
            return std::unique_ptr<AbstractAssociativeAccess::KeyValueIterator>(new KeyValueIterator(*this));
        }
        std::function<jobject(JNIEnv*,const void*)> keyConverter() const override {
            return [](JNIEnv* env,const void* pointer) -> jobject{
                return ::qtjambi_cast<jobject>(env, reinterpret_cast<const K*>(pointer));
            };
        }
        std::function<jobject(JNIEnv*,const void*)> valueConverter() const override {
            return [](JNIEnv* env,const void* pointer) -> jobject{
                return ::qtjambi_cast<jobject>(env, reinterpret_cast<const T*>(pointer));
            };
        }
    };
public:
    std::unique_ptr<AbstractHashAccess::KeyValueIterator> keyValueIterator(const void* container) override {
        return std::unique_ptr<AbstractHashAccess::KeyValueIterator>(new KeyValueIterator<true>(this, *reinterpret_cast<const QHash<K,T>*>(container)));
    }
    std::unique_ptr<AbstractHashAccess::KeyValueIterator> keyValueIterator(void* container) override {
        return std::unique_ptr<AbstractHashAccess::KeyValueIterator>(new KeyValueIterator<false>(this, *reinterpret_cast<QHash<K,T>*>(container)));
    }
 };

template<typename K, typename T>
 class QMultiHashAccess : public QtJambiPrivate::AssociativeAccessSuperclassDecider<K,T,AbstractMultiHashAccess>::type{
    typedef typename QtJambiPrivate::AssociativeAccessSuperclassDecider<K,T,AbstractMultiHashAccess>::type Super;
protected:
    QMultiHashAccess(){}
public:
    static AbstractMultiHashAccess* newInstance(){
        static QMultiHashAccess<K, T> instance;
        return &instance;
    }

    AbstractMultiHashAccess* clone() override{
        return this;
    }

    bool isDetached(const void* container) override{
        return reinterpret_cast<const QMultiHash<K,T> *>(container)->isDetached();
    }

    void detach(const ContainerInfo& container) override{
        reinterpret_cast<QMultiHash<K,T> *>(container.container)->detach();
    }

    bool isSharedWith(const void* container, const void* container2) override{
        return reinterpret_cast<const QMultiHash<K,T> *>(container)->isSharedWith(*reinterpret_cast<const QMultiHash<K,T> *>(container2));
    }

    void swap(JNIEnv *env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override{
        reinterpret_cast<QMultiHash<K,T> *>(container.container)->swap(*reinterpret_cast<QMultiHash<K,T> *>(container2.container));
        if constexpr ((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0 && (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if(container2.access!=this){
                if(AbstractReferenceCountingContainer* access = container2.access->asRC()){
                    if(access->asRCMultiMap())
                        Super::swapRC(env, container, container2);
                    else
                        Super::updateRC(env, container);
                }else{
                    Super::updateRC(env, container);
                }
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<QMultiHash<K,T>>::needsReferenceCounting){
            if(container2.access!=this){
                if(AbstractReferenceCountingContainer* access = container2.access->asRC()){
                    if(access->asRCSet())
                        Super::swapRC(env, container, container2);
                    else
                        Super::updateRC(env, container);
                }else{
                    Super::updateRC(env, container);
                }
            }
        }else{
            Q_UNUSED(env);
        }
    }

    const QMetaType& keyMetaType() override {
        static QMetaType type(QMetaType::fromType<std::remove_cv_t<K>>());
        return type;
    }

    const QMetaType& valueMetaType() override {
        static QMetaType type(QMetaType::fromType<std::remove_cv_t<T>>());
        return type;
    }

    AbstractContainerAccess::DataType keyType() override{
        return QtJambiPrivate::ContainerContentType<K>::type;
    }

    AbstractContainerAccess::DataType valueType() override{
        return QtJambiPrivate::ContainerContentType<T>::type;
    }

    AbstractContainerAccess* keyNestedContainerAccess() override {
        return QtJambiPrivate::ContainerContentType<K>::accessFactory();
    }

    AbstractContainerAccess* valueNestedContainerAccess() override {
        return QtJambiPrivate::ContainerContentType<T>::accessFactory();
    }

    bool hasKeyNestedContainerAccess() override {
        return QtJambiPrivate::ContainerContentType<K>::isContainer;
    }
    bool hasValueNestedContainerAccess() override {
        return QtJambiPrivate::ContainerContentType<T>::isContainer;
    }
    bool hasKeyNestedPointers() override {
        return QtJambiPrivate::ContainerContentType<K>::isContainer && QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting;
    }
    bool hasValueNestedPointers() override {
        return QtJambiPrivate::ContainerContentType<T>::isContainer && QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting;
    }

    void assign(void* container, const void* other) override {
        (*reinterpret_cast<QMultiHash<K,T>*>(container)) = (*reinterpret_cast<const QMultiHash<K,T>*>(other));
    }
    void assign(JNIEnv *env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override {
        (*reinterpret_cast<QMultiHash<K,T>*>(container.container)) = (*reinterpret_cast<const QMultiHash<K,T>*>(other.container));
        if constexpr ((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0 && (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if(other.access!=this){
                if(AbstractReferenceCountingContainer* access = other.access->asRC()){
                    if(access->asRCMap())
                        Super::assignRC(env, container.object, other.object);
                    else
                        Super::updateRC(env, container);
                }else{
                    Super::updateRC(env, container);
                }
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<QMultiHash<K,T>>::needsReferenceCounting){
            if(other.access!=this){
                if(AbstractReferenceCountingContainer* access = other.access->asRC()){
                    if(access->asRCSet())
                        Super::assignRC(env, container.object, other.object);
                    else
                        Super::updateRC(env, container);
                }else{
                    Super::updateRC(env, container);
                }
            }
        }else{
            Q_UNUSED(env);
        }
    }
    QMetaType registerContainer(QByteArrayView containerTypeName) override {
        return QtJambiPrivate::container_registry_impl<QMultiHash<K,T>>::register_container(containerTypeName, this);
    }

    size_t sizeOf() const override {
        return sizeof(QMultiHash<K,T>);
    }
    size_t alignOf() const override {
        return alignof(QMultiHash<K,T>);
    }
    void* constructContainer(void* placement) override {
        return new(placement) QMultiHash<K,T>();
    }
    void* constructContainer(void* placement, const void* copyOf) override {
        return new(placement) QMultiHash<K,T>(*reinterpret_cast<const QMultiHash<K,T>*>(copyOf));
    }
    void* constructContainer(JNIEnv *, void* placement, const ConstContainerAndAccessInfo& copyOf) override {
        return constructContainer(placement, copyOf.container);
    }
    void* constructContainer(void* placement, void* move) override {
        return new(placement) QMultiHash<K,T>(std::move(*reinterpret_cast<const QMultiHash<K,T>*>(move)));
    }
    void* constructContainer(JNIEnv *, void* placement, const ContainerAndAccessInfo& move) override {
        return constructContainer(placement, move.container);
    }
    bool destructContainer(void* container) override {
        reinterpret_cast<QMultiHash<K,T>*>(container)->~QMultiHash<K,T>();
        return true;
    }
    qsizetype capacity(JNIEnv * env, const void* container) override {
        return QtJambiPrivate::AssociativeContainerCapacity<QMultiHash, K, T>::function(env, container);
    }
    jboolean contains(JNIEnv * env, const void* container, jobject key) override {
        return QtJambiPrivate::AssociativeContainerContains<QMultiHash, K, T>::function(env, container, key);
    }
    bool contains(const void* container, const void* key) override {
        return QtJambiPrivate::AssociativeContainerContains<QMultiHash, K, T>::function(container, key);
    }
    qsizetype count(JNIEnv * env, const void* container, jobject key) override {
        return QtJambiPrivate::AssociativeContainerCountObject<QMultiHash, K, T>::function(env, container, key);
    }
    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        return QtJambiPrivate::AssociativeContainerConstEnd<QMultiHash, K, T>::function(env, container);
    }
    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        return QtJambiPrivate::AssociativeContainerConstBegin<QMultiHash, K, T>::function(env, container);
    }
    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override {
        return QtJambiPrivate::AssociativeContainerConstFindPair<QMultiHash, K, T>::function(env, container, key);
    }
    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key, jobject value) override {
        return QtJambiPrivate::AssociativeContainerConstFindPairs<QMultiHash, K, T>::function(env, container, key, value);
    }
    jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override {
        return QtJambiPrivate::AssociativeContainerKey<QMultiHash, K, T>::function(env, container, value, defaultKey);
    }
    ContainerAndAccessInfo keys(JNIEnv * env, const ConstContainerInfo& container) override {
        return QtJambiPrivate::AssociativeContainerKeys<QMultiHash, K, T>::function(env, container);
    }
    ContainerAndAccessInfo keys(JNIEnv * env, const ConstContainerInfo& container, jobject value) override {
        return QtJambiPrivate::AssociativeContainerKeysForValue<QMultiHash, K, T>::function(env, container, value);
    }
    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        return QtJambiPrivate::AssociativeContainerEquals<QMultiHash, K, T, ContainerAPI::getAsQMultiHash>::function(env, container, other);
    }
    void reserve(JNIEnv * env,const ContainerInfo&,qsizetype) override {
        JavaException::raiseUnsupportedOperationException(env, "QMultiHash::reserve(size)" QTJAMBI_STACKTRACEINFO );
    }
    qsizetype size(JNIEnv * env, const void* container) override {
        return QtJambiPrivate::AssociativeContainerSize<QMultiHash, K, T>::function(env, container);
    }
    qsizetype size(const void* container) override {
        return QtJambiPrivate::AssociativeContainerSize<QMultiHash, K, T>::function(container);
    }
    ContainerAndAccessInfo uniqueKeys(JNIEnv * env, const ConstContainerInfo& container) override {
        return QtJambiPrivate::AssociativeContainerUniqueKeys<QMultiHash, K, T>::function(env, container);
    }
    ContainerAndAccessInfo values(JNIEnv * env, const ConstContainerInfo& container, jobject key) override {
        return QtJambiPrivate::AssociativeContainerValuesKey<QMultiHash, K, T>::function(env, container, key);
    }
    jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override {
        return QtJambiPrivate::AssociativeContainerValue<QMultiHash, K, T>::function(env, container, key, defaultValue);
    }

    const void* value(const void* container, const void* key, const void* defaultValue) override {
        return QtJambiPrivate::AssociativeContainerValue<QMultiHash, K, T>::function(container, key, defaultValue);
    }
    ContainerAndAccessInfo values(JNIEnv * env, const ConstContainerInfo& container) override {
        return QtJambiPrivate::AssociativeContainerValues<QMultiHash, K, T>::function(env, container);
    }
    jboolean contains(JNIEnv *env, const void* container, jobject key, jobject value) override {
        return QtJambiPrivate::AssociativeContainerContainsPair<QMultiHash, K, T>::function(env, container, key, value);
    }
    qsizetype count(JNIEnv *env, const void* container, jobject key, jobject value) override {
        return QtJambiPrivate::AssociativeContainerCountPair<QMultiHash, K, T>::function(env, container, key, value);
    }
    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override {
        return QtJambiPrivate::AssociativeContainerBegin<QMultiHash, K, T>::function(env, container);
    }
    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override {
        return QtJambiPrivate::AssociativeContainerEnd<QMultiHash, K, T>::function(env, container);
    }
    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key) override {
        return QtJambiPrivate::AssociativeContainerFindPair<QMultiHash, K, T>::function(env, container, key);
    }
    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key, jobject value) override {
        return QtJambiPrivate::AssociativeContainerFindPairs<QMultiHash, K, T>::function(env, container, key, value);
    }
    void clear(JNIEnv * env, const ContainerInfo& container) override {
        QtJambiPrivate::AssociativeContainerClear<QMultiHash, K, T>::function(env, container);
        if constexpr(QtJambiPrivate::ContainerContentType<QMultiHash<K,T>>::needsReferenceCounting){
            Super::clearRC(env, container.object);
        }
    }
    void insert(JNIEnv *env, const ContainerInfo& container,jobject key,jobject _value) override {
        if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            QtJambiPrivate::AssociativeContainerInsert<QMultiHash, K, T>::function(env, container, key, _value);
            Super::putRC(env, container.object, key, _value);
        }else if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                QtJambiPrivate::AssociativeContainerInsert<QMultiHash, K, T>::function(env, container, key, _value);
                Super::addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
                Super::addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), _value);
            }else{
                QtJambiPrivate::AssociativeContainerInsert<QMultiHash, K, T>::function(env, container, key, _value);
                Super::addUniqueRC(env, container.object, key);
            }
        }else if constexpr((QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                QtJambiPrivate::AssociativeContainerInsert<QMultiHash, K, T>::function(env, container, key, _value);
                Super::addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
                Super::addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), _value);
            }else{
                jobject oldValue = value(env, container.container, key, nullptr);
                QtJambiPrivate::AssociativeContainerInsert<QMultiHash, K, T>::function(env, container, key, _value);
                Super::removeRC(env, container.object, oldValue);
                Super::addRC(env, container.object, _value);
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting || QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            QtJambiPrivate::AssociativeContainerInsert<QMultiHash, K, T>::function(env, container, key, _value);
            Super::addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
            Super::addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), _value);
        }else{
            QtJambiPrivate::AssociativeContainerInsert<QMultiHash, K, T>::function(env, container, key, _value);
        }
    }

    void insert(void* container,const void* key,const void* _value) override {
        if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            QtJambiPrivate::AssociativeContainerInsert<QMultiHash, K, T>::function(container, key, _value);
            if(JniEnvironment env{100}){
                if(jobject object = QtJambiAPI::findObject(env, container)){
                    if(jobject _key = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(key))){
                        if(jobject __value = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(_value))){
                            Super::putRC(env, object, _key, __value);
                        }
                    }
                }
            }
        }else if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                QtJambiPrivate::AssociativeContainerInsert<QMultiHash, K, T>::function(container, key, _value);
                if(JniEnvironment env{100}){
                    if(jobject object = QtJambiAPI::findObject(env, container)){
                        if(jobject _key = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(key))){
                            Super::addNestedValueRC(env, object, keyType(), hasKeyNestedPointers(), _key);
                        }
                        if(jobject __value = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(_value))){
                            Super::addNestedValueRC(env, object, valueType(), hasValueNestedPointers(), __value);
                        }
                    }
                }
            }else{
                QtJambiPrivate::AssociativeContainerInsert<QMultiHash, K, T>::function(container, key, _value);
                if(JniEnvironment env{100}){
                    if(jobject object = QtJambiAPI::findObject(env, container)){
                        if(jobject _key = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(key))){
                            Super::addUniqueRC(env, object, _key);
                        }
                    }
                }
            }
        }else if constexpr((QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                QtJambiPrivate::AssociativeContainerInsert<QMultiHash, K, T>::function(container, key, _value);
                if(JniEnvironment env{100}){
                    if(jobject object = QtJambiAPI::findObject(env, container)){
                        if(jobject _key = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(key))){
                            if(jobject __value = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(_value))){
                                Super::addNestedValueRC(env, object, keyType(), hasKeyNestedPointers(), _key);
                                Super::addNestedValueRC(env, object, valueType(), hasValueNestedPointers(), __value);
                            }
                        }
                    }
                }
            }else{
                const void* oldValue = value(container, key, nullptr);
                QtJambiPrivate::AssociativeContainerInsert<QMultiHash, K, T>::function(container, key, _value);
                if(JniEnvironment env{100}){
                    if(jobject object = QtJambiAPI::findObject(env, container)){
                        if(oldValue){
                            if(jobject __value = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(oldValue))){
                                Super::removeRC(env, object, __value);
                            }
                        }
                        if(jobject __value = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(_value))){
                            Super::addRC(env, object, __value);
                        }
                    }
                }
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting || QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            QtJambiPrivate::AssociativeContainerInsert<QMultiHash, K, T>::function(container, key, _value);
            if(JniEnvironment env{100}){
                if(jobject object = QtJambiAPI::findObject(env, container)){
                    if(jobject _key = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(key))){
                        Super::addNestedValueRC(env, object, keyType(), hasKeyNestedPointers(), _key);
                    }
                    if(jobject __value = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(_value))){
                        Super::addNestedValueRC(env, object, valueType(), hasValueNestedPointers(), __value);
                    }
                }
            }
        }else{
            QtJambiPrivate::AssociativeContainerInsert<QMultiHash, K, T>::function(container, key, _value);
        }
    }

    qsizetype remove(JNIEnv * env, const ContainerInfo& container,jobject key) override {
        qsizetype result;
        if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            result = QtJambiPrivate::AssociativeContainerRemove<QMultiHash, K, T>::function(env, container, key);
            Super::removeRC(env, container.object, key, result);
        }else if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                result = QtJambiPrivate::AssociativeContainerRemove<QMultiHash, K, T>::function(env, container, key);
                if(result>0)
                    Super::updateRC(env, container);
            }else{
                result = QtJambiPrivate::AssociativeContainerRemove<QMultiHash, K, T>::function(env, container, key);
                if(result>0){
                    Super::removeRC(env, container.object, key);
                }
            }
        }else if constexpr((QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                result = QtJambiPrivate::AssociativeContainerRemove<QMultiHash, K, T>::function(env, container, key);

            }else{
                jobject oldValue = value(env, container.container, key, nullptr);
                result = QtJambiPrivate::AssociativeContainerRemove<QMultiHash, K, T>::function(env, container, key);
                if(result>0){
                    Super::removeRC(env, container.object, oldValue);
                }
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting || QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            result = QtJambiPrivate::AssociativeContainerRemove<QMultiHash, K, T>::function(env, container, key);
            if(result>0)
                Super::updateRC(env, container);
        }else{
            result = QtJambiPrivate::AssociativeContainerRemove<QMultiHash, K, T>::function(env, container, key);
        }
        return result;
    }
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override {
        jobject result = QtJambiPrivate::AssociativeContainerTake<QMultiHash, K, T>::function(env, container, key);
        if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            Super::removeRC(env, container.object, key);
        }else if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                Super::updateRC(env, container);
            }else{
                Super::removeRC(env, container.object, key);
            }
        }else if constexpr((QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                Super::updateRC(env, container);
            }else{
                Super::removeRC(env, container.object, result);
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting || QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            Super::updateRC(env, container);
        }
        return result;
    }
    qsizetype remove(JNIEnv * env, const ContainerInfo& container,jobject key, jobject _value) override {
        qsizetype result;
        if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            result = QtJambiPrivate::AssociativeContainerRemovePair<QMultiHash, K, T>::function(env, container, key, _value);
            Super::removeRC(env, container.object, key, _value, result);
        }else if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                result = QtJambiPrivate::AssociativeContainerRemovePair<QMultiHash, K, T>::function(env, container, key, _value);
                if(result>0)
                    Super::updateRC(env, container);
            }else{
                result = QtJambiPrivate::AssociativeContainerRemovePair<QMultiHash, K, T>::function(env, container, key, _value);
                if(result>0){
                    Super::removeRC(env, container.object, key);
                }
            }
        }else if constexpr((QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                result = QtJambiPrivate::AssociativeContainerRemovePair<QMultiHash, K, T>::function(env, container, key, _value);

            }else{
                result = QtJambiPrivate::AssociativeContainerRemovePair<QMultiHash, K, T>::function(env, container, key, _value);
                Super::removeRC(env, container.object, _value, result);
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting || QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            result = QtJambiPrivate::AssociativeContainerRemovePair<QMultiHash, K, T>::function(env, container, key, _value);
            if(result>0)
                Super::updateRC(env, container);
        }else{
            result = QtJambiPrivate::AssociativeContainerRemovePair<QMultiHash, K, T>::function(env, container, key, _value);
        }
        return result;
    }
    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject _value) override {
        if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            QtJambiPrivate::AssociativeContainerReplacePair<QMultiHash, K, T>::function(env, container, key, _value);
            Super::removeRC(env, container.object, key);
            Super::putRC(env, container.object, key, _value);
        }else if constexpr((QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                QtJambiPrivate::AssociativeContainerReplacePair<QMultiHash, K, T>::function(env, container, key, _value);
                Super::updateRC(env, container);
            }else{
                QtJambiPrivate::AssociativeContainerReplacePair<QMultiHash, K, T>::function(env, container, key, _value);
                Super::addRC(env, container.object, key);
            }
        }else if constexpr((QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                QtJambiPrivate::AssociativeContainerReplacePair<QMultiHash, K, T>::function(env, container, key, _value);

            }else{
                jobject oldValue = value(env, container.container, key, nullptr);
                QtJambiPrivate::AssociativeContainerReplacePair<QMultiHash, K, T>::function(env, container, key, _value);
                Super::removeRC(env, container.object, oldValue);
                Super::addRC(env, container.object, _value);
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting || QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            QtJambiPrivate::AssociativeContainerReplacePair<QMultiHash, K, T>::function(env, container, key, _value);
            Super::updateRC(env, container);
        }else{
            QtJambiPrivate::AssociativeContainerReplacePair<QMultiHash, K, T>::function(env, container, key, _value);
        }
    }
    void unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other) override {
        QtJambiPrivate::AssociativeContainerUnite<QMultiHash, K, T, ContainerAPI::getAsQMultiHash>::function(env, container, other);
        if constexpr(QtJambiPrivate::ContainerContentType<QMultiHash<K,T>>::needsReferenceCounting){
            Super::updateRC(env, container);
        }
    }
private:
    template<bool _isConst>
    class KeyValueIterator : public AbstractHashAccess::KeyValueIterator{
        using Container = std::conditional_t<_isConst, const QMultiHash<K,T>, QMultiHash<K,T>>;
        using iterator = decltype(std::declval<Container>().begin());
        QMultiHashAccess* m_access;
        iterator current;
        iterator end;
        KeyValueIterator(const KeyValueIterator& other)
            :m_access(other.m_access),
            current(other.current),
            end(other.end) {}
    protected:
        AbstractAssociativeAccess* access() override {return m_access;}
    public:
        KeyValueIterator(QMultiHashAccess* _access, Container& container)
            : m_access(_access),
              current(container.begin()),
              end(container.end())
        {
        }
        bool hasNext() override {return current!=end;};
        QPair<jobject,jobject> next(JNIEnv * env) override {
            QPair<jobject,jobject> result{::qtjambi_cast<jobject>(env, current.key()),
                                          ::qtjambi_cast<jobject>(env, current.value())};
            ++current;
            return result;
        }
        QPair<const void*,const void*> next() override {
            QPair<const void*,const void*> result{QtJambiPrivate::ContainerContentDeref<K>::deref(current.key()), QtJambiPrivate::ContainerContentDeref<T>::deref(current.value())};
            ++current;
            return result;
        }
        bool isConst() override{
            return _isConst;
        }
        QPair<const void*,const void*> constNext() override {
            QPair<const void*,const void*> result{&current.key(), &current.value()};
            ++current;
            return result;
        }
        QPair<const void*,void*> mutableNext() override {
            if constexpr(!_isConst){
                QPair<const void*,void*> result{&current.key(), &current.value()};
                ++current;
                return result;
            }else{
                return {nullptr,nullptr};
            }
        }
        bool operator==(const AbstractMapAccess::KeyValueIterator& other) const override {
            return current==reinterpret_cast<const KeyValueIterator&>(other).current;
        }
        std::unique_ptr<AbstractAssociativeAccess::KeyValueIterator> clone() const override {
            return std::unique_ptr<AbstractAssociativeAccess::KeyValueIterator>(new KeyValueIterator(*this));
        }
        std::function<jobject(JNIEnv*,const void*)> keyConverter() const override {
            return [](JNIEnv* env,const void* pointer) -> jobject{
                return ::qtjambi_cast<jobject>(env, reinterpret_cast<const K*>(pointer));
            };
        }
        std::function<jobject(JNIEnv*,const void*)> valueConverter() const override {
            return [](JNIEnv* env,const void* pointer) -> jobject{
                return ::qtjambi_cast<jobject>(env, reinterpret_cast<const T*>(pointer));
            };
        }
    };
public:
    std::unique_ptr<AbstractHashAccess::KeyValueIterator> keyValueIterator(const void* container) override {
        return std::unique_ptr<AbstractHashAccess::KeyValueIterator>(new KeyValueIterator<true>(this, *reinterpret_cast<const QMultiHash<K,T>*>(container)));
    }
    std::unique_ptr<AbstractHashAccess::KeyValueIterator> keyValueIterator(void* container) override {
        return std::unique_ptr<AbstractHashAccess::KeyValueIterator>(new KeyValueIterator<false>(this, *reinterpret_cast<QMultiHash<K,T>*>(container)));
    }
};

template<typename K, typename T>
class QPairAccess : public AbstractPairAccess{
    typedef AbstractMapAccess Super;
protected:
    QPairAccess(){}
public:
    static AbstractPairAccess* newInstance(){
        static QPairAccess<K, T> instance;
        return &instance;
    }

    AbstractPairAccess* clone() override{
        return this;
    }

    void swap(JNIEnv *, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override{
        reinterpret_cast<QPair<K,T> *>(container.container)->swap(*reinterpret_cast<QPair<K,T> *>(container2.container));
    }

    const QMetaType& firstMetaType() override {
        static QMetaType type(QMetaType::fromType<std::remove_cv_t<K>>());
        return type;
    }
    const QMetaType& secondMetaType() override {
        static QMetaType type(QMetaType::fromType<std::remove_cv_t<T>>());
        return type;
    }

    AbstractContainerAccess::DataType firstType() override{
        return QtJambiPrivate::ContainerContentType<K>::type;
    }

    AbstractContainerAccess::DataType secondType() override{
        return QtJambiPrivate::ContainerContentType<T>::type;
    }

    AbstractContainerAccess* firstNestedContainerAccess() override {
        return QtJambiPrivate::ContainerContentType<K>::accessFactory();
    }

    AbstractContainerAccess* secondNestedContainerAccess() override {
        return QtJambiPrivate::ContainerContentType<T>::accessFactory();
    }

    bool hasFirstNestedContainerAccess() override {
        return QtJambiPrivate::ContainerContentType<K>::isContainer;
    }
    bool hasSecondNestedContainerAccess() override {
        return QtJambiPrivate::ContainerContentType<T>::isContainer;
    }
    bool hasFirstNestedPointers() override {
        return QtJambiPrivate::ContainerContentType<K>::isContainer && QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting;
    }
    bool hasSecondNestedPointers() override {
        return QtJambiPrivate::ContainerContentType<T>::isContainer && QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting;
    }

    void assign(void* container, const void* other) override {
        (*reinterpret_cast<QPair<K,T>*>(container)) = (*reinterpret_cast<const QPair<K,T>*>(other));
    }
    void assign(JNIEnv *, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override {
        (*reinterpret_cast<QPair<K,T>*>(container.container)) = (*reinterpret_cast<const QPair<K,T>*>(other.container));
    }
    size_t sizeOf() const override {
        return sizeof(QPair<K,T>);
    }
    size_t alignOf() const override {
        return alignof(QPair<K,T>);
    }
    void* constructContainer(void* placement) override {
        return new(placement) QPair<K,T>();
    }

    void* constructContainer(void* placement, const void* copyOf) override {
        return new(placement) QPair<K,T>(*reinterpret_cast<const QPair<K,T>*>(copyOf));
    }
    void* constructContainer(JNIEnv *, void* placement, const ConstContainerAndAccessInfo& copyOf) override {
        return constructContainer(placement, copyOf.container);
    }
    void* constructContainer(void* placement, void* move) override {
        return new(placement) QPair<K,T>(std::move(*reinterpret_cast<const QPair<K,T>*>(move)));
    }
    void* constructContainer(JNIEnv *, void* placement, const ContainerAndAccessInfo& move) override {
        return constructContainer(placement, move.container);
    }
    bool destructContainer(void* container) override {
        reinterpret_cast<QPair<K,T>*>(container)->~QPair<K,T>();
        return true;
    }

    QMetaType registerContainer(QByteArrayView containerTypeName) override {
        return QtJambiPrivate::container_registry_impl<QPair<K,T>>::register_container(containerTypeName, this);
    }

    jobject first(JNIEnv * env, const void* container) override {
        const QPair<K,T> *pair = static_cast<const QPair<K,T> *>(container);
        return ::qtjambi_cast<jobject>(env, pair->first);
    }

    jobject second(JNIEnv * env, const void* container) override {
        const QPair<K,T> *pair = static_cast<const QPair<K,T> *>(container);
        return ::qtjambi_cast<jobject>(env, pair->second);
    }

    void setFirst(JNIEnv * env, void* container, jobject first) override {
        QPair<K,T> *pair = static_cast<QPair<K,T> *>(container);
        pair->first = ::qtjambi_cast<K>(env, first);
    }

    void setSecond(JNIEnv * env, void* container, jobject second) override {
        QPair<K,T> *pair = static_cast<QPair<K,T> *>(container);
        pair->second = ::qtjambi_cast<T>(env, second);
    }
public:
    QPair<const void*,const void*> elements(const void* container) override {
        const QPair<K,T> *pair = static_cast<const QPair<K,T> *>(container);
        return {QtJambiPrivate::ContainerContentDeref<K>::deref(pair->first), QtJambiPrivate::ContainerContentDeref<T>::deref(pair->second)};
    }
private:
    template<bool _isConst>
    class KeyValueIterator : public AbstractHashAccess::KeyValueIterator{
        using Container = std::conditional_t<_isConst, const QPair<K,T>, QPair<K,T>>;
        QPairAccess* m_access;
        Container* container;
        KeyValueIterator(const KeyValueIterator& other)
            :m_access(other.m_access),
            container(other.container) {}
    protected:
        AbstractAssociativeAccess* access() override {return nullptr;}
    public:
        KeyValueIterator(QPairAccess* _access, Container& container)
            : m_access(_access),
            container(&container)
        {
        }
        const QMetaType& keyMetaType() override {return m_access->firstMetaType();}
        const QMetaType& valueMetaType() override {return m_access->secondMetaType();}
        DataType keyType() override {return m_access->firstType();}
        DataType valueType() override {return m_access->secondType();}
        AbstractContainerAccess* keyNestedContainerAccess() override {return m_access->firstNestedContainerAccess();}
        AbstractContainerAccess* valueNestedContainerAccess() override {return m_access->secondNestedContainerAccess();}
        bool hasKeyNestedContainerAccess() override {return m_access->hasFirstNestedContainerAccess();}
        bool hasValueNestedContainerAccess() override {return m_access->hasSecondNestedContainerAccess();}
        bool hasKeyNestedPointers() override {return m_access->hasFirstNestedPointers();}
        bool hasValueNestedPointers() override {return m_access->hasSecondNestedPointers();}
        bool hasNext() override {return container;};
        QPair<jobject,jobject> next(JNIEnv * env) override {
            QPair<jobject,jobject> result;
            if(container){
                result.first = m_access->first(env, container);
                result.second = m_access->second(env, container);
                container = nullptr;
            }
            return result;
        }
        QPair<const void*,const void*> next() override {
            QPair<const void*,const void*> result;
            if(container){
                result.first = QtJambiPrivate::ContainerContentDeref<K>::deref(container->first);
                result.second = QtJambiPrivate::ContainerContentDeref<T>::deref(container->second);
                container = nullptr;
            }
            return result;
        }
        bool isConst() override{
            return _isConst;
        }
        QPair<const void*,const void*> constNext() override {
            QPair<const void*,const void*> result;
            if(container){
                result.first = &container->first;
                result.second = &container->second;
                container = nullptr;
            }
            return result;
        }
        QPair<const void*,void*> mutableNext() override {
            if constexpr(!_isConst){
                QPair<const void*,void*> result;
                if(container){
                    result.first = &container->first;
                    result.second = &container->second;
                    container = nullptr;
                }
                return result;
            }else{
                return {nullptr,nullptr};
            }
        }
        bool operator==(const AbstractMapAccess::KeyValueIterator& other) const override {
            return container==reinterpret_cast<const KeyValueIterator&>(other).container;
        }
        std::unique_ptr<AbstractAssociativeAccess::KeyValueIterator> clone() const override {
            return std::unique_ptr<AbstractAssociativeAccess::KeyValueIterator>(new KeyValueIterator(*this));
        }
        std::function<jobject(JNIEnv*,const void*)> keyConverter() const override {
            return [](JNIEnv* env,const void* pointer) -> jobject{
                return ::qtjambi_cast<jobject>(env, reinterpret_cast<const K*>(pointer));
            };
        }
        std::function<jobject(JNIEnv*,const void*)> valueConverter() const override {
            return [](JNIEnv* env,const void* pointer) -> jobject{
                return ::qtjambi_cast<jobject>(env, reinterpret_cast<const T*>(pointer));
            };
        }
    };
public:
    std::unique_ptr<AbstractHashAccess::KeyValueIterator> keyValueIterator(const void* container) override {
        return std::unique_ptr<AbstractHashAccess::KeyValueIterator>(new KeyValueIterator<true>(this, *reinterpret_cast<const QPair<K,T>*>(container)));
    }
    std::unique_ptr<AbstractHashAccess::KeyValueIterator> keyValueIterator(void* container) override {
        return std::unique_ptr<AbstractHashAccess::KeyValueIterator>(new KeyValueIterator<false>(this, *reinterpret_cast<QPair<K,T>*>(container)));
    }
private:
    template<bool is_const>
    class ElementIterator : public AbstractSequentialAccess::ElementIterator{
        using Container = std::conditional_t<is_const, const QPair<K,T>, QPair<K,T>>;
        QPairAccess* m_access;
        Container* container;
        uint index = 0;
        ElementIterator(const ElementIterator& other)
            :m_access(other.m_access),
            container(other.container) {}
    protected:
        AbstractSequentialAccess* access() override { return nullptr; }
    public:
        ElementIterator(QPairAccess* _access, Container& container)
            :m_access(_access),
            container(&container) {}
        ~ElementIterator() override {};
        const QMetaType& elementMetaType() override {
            switch(index){
            case 0:
                return m_access->firstMetaType();
            default:
                return m_access->secondMetaType();
            }
        }
        DataType elementType() override {
            switch(index){
            case 0:
                return m_access->firstType();
            default:
                return m_access->secondType();
            }
        }
        AbstractContainerAccess* elementNestedContainerAccess() override {
            switch(index){
            case 0:
                return m_access->firstNestedContainerAccess();
            case 1:
                return m_access->secondNestedContainerAccess();
            default:
                return nullptr;
            }
        }
        bool hasNestedContainerAccess() override {
            return elementNestedContainerAccess();
        }
        bool hasNestedPointers() override {
            switch(index){
            case 0:
                return m_access->hasFirstNestedPointers();
            default:
                return m_access->hasSecondNestedPointers();
            }
        }
        bool hasNext() override {return index<2;};
        jobject next(JNIEnv * env) override {
            switch(index){
            case 0:
                ++index;
                return m_access->first(env, container);
            case 1:
                ++index;
                return m_access->second(env, container);
            default:
                return nullptr;
            }
        }
        const void* next() override {
            switch(index){
            case 0:
                ++index;
                return QtJambiPrivate::ContainerContentDeref<K>::deref(container->first);
            case 1:
                ++index;
                return QtJambiPrivate::ContainerContentDeref<T>::deref(container->second);
            default:
                return nullptr;
            }
        };
        const void* constNext() override {
            switch(index){
            case 0:
                ++index;
                return &container->first;
            case 1:
                ++index;
                return &container->second;
            default:
                return nullptr;
            }
        };
        bool isConst() override{
            return is_const;
        }
        void* mutableNext() override {
            if constexpr(!is_const){
                switch(index){
                case 0:
                    ++index;
                    return &container->first;
                case 1:
                    ++index;
                    return &container->second;
                default:
                    return nullptr;
                }
            }else{
                return nullptr;
            }
        }
        bool operator==(const AbstractSequentialAccess::ElementIterator& other) const override {
            return container==reinterpret_cast<const ElementIterator&>(other).container && index==reinterpret_cast<const ElementIterator&>(other).index;
        }
        std::unique_ptr<AbstractSequentialAccess::ElementIterator> clone() const override {
            return std::unique_ptr<AbstractSequentialAccess::ElementIterator>(new ElementIterator(*this));
        }
        std::function<jobject(JNIEnv*,const void*)> elementConverter() const override {
            switch(index){
            case 0:
                return [](JNIEnv* env,const void* pointer) -> jobject{
                    return ::qtjambi_cast<jobject>(env, reinterpret_cast<const K*>(pointer));
                };
            default:
                return [](JNIEnv* env,const void* pointer) -> jobject{
                    return ::qtjambi_cast<jobject>(env, reinterpret_cast<const T*>(pointer));
                };
            }
        }
    };
public:
    std::unique_ptr<AbstractSequentialAccess::ElementIterator> elementIterator(const void* container) override {
        return std::unique_ptr<AbstractSequentialAccess::ElementIterator>(new ElementIterator<true>(this, *reinterpret_cast<const QPair<K,T>*>(container)));
    }
    std::unique_ptr<AbstractSequentialAccess::ElementIterator> elementIterator(void* container) override {
        return std::unique_ptr<AbstractSequentialAccess::ElementIterator>(new ElementIterator<false>(this, *reinterpret_cast<QPair<K,T>*>(container)));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerAccess{
    static constexpr AbstractContainerAccess* newInstance(){return nullptr;}
};

template<typename K, typename T>
struct AssociativeContainerAccess<QMap,K,T> : QMapAccess<K,T>{
};

template<typename K, typename T>
struct AssociativeContainerAccess<QMultiMap,K,T> : QMultiMapAccess<K,T>{
};

template<typename K, typename T>
struct AssociativeContainerAccess<QHash,K,T> : QHashAccess<K,T>{
};

template<typename K, typename T>
struct AssociativeContainerAccess<QMultiHash,K,T> : QMultiHashAccess<K,T>{
};

template<typename K, typename T>
struct AssociativeContainerAccess<QPair,K,T> : QPairAccess<K,T>{
};

namespace QtJambiPrivate{

template<typename T>
struct ContainerContentType;

template<typename K, typename T>
struct ContainerContentType<QMap<K,T>>{
    static constexpr AbstractContainerAccess::DataType type = AbstractContainerAccess::Value;
    static constexpr bool isContainer = true;
    static constexpr bool needsReferenceCounting = (QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)
                                                  || (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)
                                                  || QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting
                                                  || QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting;
    static constexpr bool needsOwnerCheck = ContainerContentType<K>::needsOwnerCheck
                                            || QtJambiPrivate::ContainerContentType<T>::needsOwnerCheck;
    static constexpr AbstractContainerAccess* accessFactory(){return QMapAccess<K,T>::newInstance();};
};

template<typename K, typename T>
struct ContainerContentType<QHash<K,T>>{
    static constexpr AbstractContainerAccess::DataType type = AbstractContainerAccess::Value;
    static constexpr bool isContainer = true;
    static constexpr bool needsReferenceCounting = (QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)
                                                  || (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)
                                                  || QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting
                                                  || QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting;
    static constexpr bool needsOwnerCheck = ContainerContentType<K>::needsOwnerCheck
                                            || QtJambiPrivate::ContainerContentType<T>::needsOwnerCheck;
    static constexpr AbstractContainerAccess* accessFactory(){return QHashAccess<K,T>::newInstance();};
};

template<typename K, typename T>
struct ContainerContentType<QMultiMap<K,T>>{
    static constexpr AbstractContainerAccess::DataType type = AbstractContainerAccess::Value;
    static constexpr bool isContainer = true;
    static constexpr bool needsReferenceCounting = (QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)
                                                  || (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)
                                                  || QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting
                                                  || QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting;
    static constexpr bool needsOwnerCheck = ContainerContentType<K>::needsOwnerCheck
                                            || QtJambiPrivate::ContainerContentType<T>::needsOwnerCheck;
    static constexpr AbstractContainerAccess* accessFactory(){return QMultiMapAccess<K,T>::newInstance();};
};

template<typename K, typename T>
struct ContainerContentType<QMultiHash<K,T>>{
    static constexpr AbstractContainerAccess::DataType type = AbstractContainerAccess::Value;
    static constexpr bool isContainer = true;
    static constexpr bool needsReferenceCounting = (QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)
                                                  || (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)
                                                  || QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting
                                                  || QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting;
    static constexpr bool needsOwnerCheck = ContainerContentType<K>::needsOwnerCheck
                                            || QtJambiPrivate::ContainerContentType<T>::needsOwnerCheck;
    static constexpr AbstractContainerAccess* accessFactory(){return QMultiHashAccess<K,T>::newInstance();};
};

template<typename K, typename T>
struct ContainerContentType<QPair<K,T>>{
    static constexpr AbstractContainerAccess::DataType type = AbstractContainerAccess::Value;
    static constexpr bool isContainer = true;
    static constexpr bool needsReferenceCounting = (QtJambiPrivate::ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)
                                                   || (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)
                                                   || QtJambiPrivate::ContainerContentType<K>::needsReferenceCounting
                                                   || QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting;
    static constexpr bool needsOwnerCheck = ContainerContentType<K>::needsOwnerCheck
                                            || QtJambiPrivate::ContainerContentType<T>::needsOwnerCheck;
    static constexpr AbstractContainerAccess* accessFactory(){return QPairAccess<K,T>::newInstance();};
};

} // namespace QtJambiPrivate

#endif // CONTAINERACCESS_ASSOCIATIVE_H
