/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBI_CAST_MAP_P_H
#define QTJAMBI_CAST_MAP_P_H

#include "qtjambi_cast_util_p.h"
#include "qtjambi_cast_container_util_p.h"

template<typename T>
int registerMetaType(const QByteArray& typeName);

namespace QtJambiPrivate {

typedef bool (*IsBiContainerFunction)(JNIEnv *, jobject, const std::type_info&, const QMetaType&, const std::type_info&, const QMetaType&);

template<typename Iterator>
class QMapIteratorAccess : QAbstractIteratorAccess<Iterator,AbstractBiIteratorAccess>{
private:
    QMapIteratorAccess(){}
public:
    static AbstractBiIteratorAccess* newInstance(){
        static QMapIteratorAccess<Iterator> instance;
        return &instance;
    }

    AbstractBiIteratorAccess* clone() override{
        return this;
    }

    void dispose() override {}

    jobject value(JNIEnv * env, void* ptr) override {
        Iterator* iterator = static_cast<Iterator*>(ptr);
        return qtjambi_scoped_cast<false,jobject,typename std::add_const<decltype(iterator->value())>::type>::cast(env, iterator->value(), nullptr, nullptr);
    }
    jobject key(JNIEnv * env, void* ptr) override {
        Iterator* iterator = static_cast<Iterator*>(ptr);
        return qtjambi_scoped_cast<false,jobject,typename std::add_const<decltype(iterator->key())>::type>::cast(env, iterator->key(), nullptr, nullptr);
    }
};

template<typename Iterator>
struct qtjambi_BiIterator_caster{
    static jobject cast(JNIEnv * env, QtJambiNativeID nativeId, typename std::conditional<std::is_pointer<Iterator>::value, Iterator, const Iterator&>::type iter){
        return qtjambi_from_QMapIterator(env, nativeId,
            new Iterator(iter),
            [](void* ptr,bool) {
               Iterator* iterator = static_cast<Iterator*>(ptr);
               delete iterator;
            },
            QMapIteratorAccess<Iterator>::newInstance()
        );
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct BiContainerBegin{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "begin()" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerBegin<Container, K, T, true>{
    static jobject function(JNIEnv *env, QtJambiNativeID nativeId, const void* ptr) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return qtjambi_BiIterator_caster<typename Container<K,T>::const_iterator>::cast(env, nativeId, container->begin());
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct BiContainerCapacity{
    static jint function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "capacity()" QTJAMBI_STACKTRACEINFO );
        return 0;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerCapacity<Container, K, T, true>{
    static jint function(JNIEnv *, const void* ptr) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return jint(container->capacity());
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct BiContainerReserve{
    static void function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "reserve(size)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerReserve<Container, K, T, true>{
    static void function(JNIEnv *, void* ptr, jint size) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
        container->reserve(size);
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct BiContainerContains{
    static jboolean function(JNIEnv * env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "contains(key)" QTJAMBI_STACKTRACEINFO );
        return false;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerContains<Container, K, T, true>{
    static jboolean function(JNIEnv * env, const void* ptr, jobject object) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return container->contains(qtjambi_scoped_cast<false,K,jobject>::cast(env, object, nullptr, nullptr));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct BiContainerCountObject{
    static jint function(JNIEnv * env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "count(key)" QTJAMBI_STACKTRACEINFO );
        return 0;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerCountObject<Container, K, T, true>{
    static jint function(JNIEnv * env, const void* ptr, jobject object) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return container->count(qtjambi_scoped_cast<false,K,jobject>::cast(env, object, nullptr, nullptr));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct BiContainerEnd{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "end()" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerEnd<Container, K, T, true>{
    static jobject function(JNIEnv *env, QtJambiNativeID nativeId, const void* ptr) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return qtjambi_BiIterator_caster<typename Container<K,T>::const_iterator>::cast(env, nativeId, container->end());
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct BiContainerFindPair{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "find(key,value)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerFindPair<Container, K, T, true>{
    static jobject function(JNIEnv *env, QtJambiNativeID nativeId, const void* ptr, jobject key) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return qtjambi_BiIterator_caster<typename Container<K,T>::const_iterator>::cast(env, nativeId, container->find(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr)));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct BiContainerFirst{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "first()" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerFirst<Container, K, T, true>{
    static jobject function(JNIEnv *env, const void* ptr) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return qtjambi_scoped_cast<false,jobject,const T>::cast(env, container->first(), nullptr, nullptr);
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct BiContainerLast{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "last()" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerLast<Container, K, T, true>{
    static jobject function(JNIEnv *env, const void* ptr) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return qtjambi_scoped_cast<false,jobject,const T>::cast(env, container->last(), nullptr, nullptr);
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct BiContainerFirstKey{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "firstKey()" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerFirstKey<Container, K, T, true>{
    static jobject function(JNIEnv *env, const void* ptr) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return qtjambi_scoped_cast<false,jobject,const K>::cast(env, container->firstKey(), nullptr, nullptr);
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct BiContainerLastKey{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "lastKey()" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerLastKey<Container, K, T, true>{
    static jobject function(JNIEnv *env, const void* ptr) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return qtjambi_scoped_cast<false,jobject,const K>::cast(env, container->lastKey(), nullptr, nullptr);
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value && supports_equal<T>::value>
struct BiContainerKey{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "key(value, defaultKey)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerKey<Container, K, T, true>{
    static jobject function(JNIEnv *env, const void* ptr, jobject value, jobject defaultKey) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return qtjambi_scoped_cast<false,jobject,const K>::cast(env, container->key(qtjambi_scoped_cast<false,T,jobject>::cast(env, value, nullptr, nullptr), qtjambi_scoped_cast<false,K,jobject>::cast(env, defaultKey, nullptr, nullptr)), nullptr, nullptr);
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct BiContainerKeys{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "keys()" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerKeys<Container, K, T, true>{
    static jobject function(JNIEnv *env, const void* ptr) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return qtjambi_scoped_cast<false,jobject,const QList<K>>::cast(env, container->keys(), nullptr, nullptr);
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_keys_by_value<Container,K,T>::value && supports_map_sort<Container,K,T>::value && supports_equal<K>::value && supports_equal<T>::value>
struct BiContainerKeysForValue{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "keys(value)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerKeysForValue<Container, K, T, true>{
    static jobject function(JNIEnv *env, const void* ptr, jobject value) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return qtjambi_scoped_cast<false,jobject,const QList<K>>::cast(env, container->keys(qtjambi_scoped_cast<false,T,jobject>::cast(env, value, nullptr, nullptr)), nullptr, nullptr);
    }
};

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
template<typename K, typename T>
struct BiContainerKeysForValue<QMultiHash, K, T, true>{
    static jobject function(JNIEnv *env, const void* ptr, jobject value) {
        const QMultiHash<K,T> *container = static_cast<const QMultiHash<K,T> *>(ptr);
        QList<K> _keys;
        typename QMultiHash<K,T>::const_iterator i = reinterpret_cast<const QMultiHash<K,T> *>(container)->begin();
        typename QMultiHash<K,T>::const_iterator end = reinterpret_cast<const QMultiHash<K,T> *>(container)->end();
        T _qvalue = qtjambi_scoped_cast<false,T,jobject>::cast(env, value, nullptr, nullptr);
        while (i != end) {
            if(i.value() == _qvalue)
                _keys.append(i.key());
            ++i;
        }
        return qtjambi_scoped_cast<false,jobject,const QList<K>>::cast(env, _keys, nullptr, nullptr);
    }
};
#endif


template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct BiContainerLowerBound{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "lowerBounds(value)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerLowerBound<Container, K, T, true>{
    static jobject function(JNIEnv *env, QtJambiNativeID nativeId, const void* ptr, jobject key) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return qtjambi_BiIterator_caster<typename Container<K,T>::const_iterator>::cast(env, nativeId, container->lowerBound(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr)));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct BiContainerUpperBound{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "upperBounds(value)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerUpperBound<Container, K, T, true>{
    static jobject function(JNIEnv *env, QtJambiNativeID nativeId, const void* ptr, jobject key) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return qtjambi_BiIterator_caster<typename Container<K,T>::const_iterator>::cast(env, nativeId, container->upperBound(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr)));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, IsBiContainerFunction isContainer, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value && supports_equal<T>::value>
struct BiContainerEquals{
    static jboolean function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "operator==(map)" QTJAMBI_STACKTRACEINFO );
        return false;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, IsBiContainerFunction isContainer>
struct BiContainerEquals<Container, K, T, isContainer, true>{
    static jboolean function(JNIEnv * env, const void* ptr, jobject other) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        std::unique_ptr<Container<K,T> > __qt_scoped_pointer;
        Container<K,T> *__qt_other_pointer = nullptr;
        if (other!= nullptr) {
            if (isContainer(env, other, qtjambi_type<K>::id(), QTJAMBI_METATYPE_FROM_TYPE2(K), qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T))) {
                __qt_other_pointer = qtjambi_to_object<Container<K,T>>(env, other);
            } else {
                __qt_scoped_pointer.reset(new Container<K,T>());
                __qt_other_pointer = __qt_scoped_pointer.get();
                jobject iterator = qtjambi_map_entryset_iterator(env, other);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jobject entry = qtjambi_iterator_next(env, iterator);
                    jobject key = qtjambi_map$entry_key(env, entry);
                    jobject val = qtjambi_map$entry_value(env, entry);
                    __qt_other_pointer->insert( qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr), qtjambi_scoped_cast<false,T,jobject>::cast(env, val, nullptr, nullptr));
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
struct BiContainerSize{
    static jint function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "size()" QTJAMBI_STACKTRACEINFO );
        return 0;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerSize<Container, K, T, true>{
    static jint function(JNIEnv *, const void* ptr) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return container->size();
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct BiContainerUniqueKeys{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "uniqueKeys()" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerUniqueKeys<Container, K, T, true>{
    static jobject function(JNIEnv *env, const void* ptr) {
            const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
            return qtjambi_scoped_cast<false,jobject,const QList<K>>::cast(env, container->uniqueKeys(), nullptr, nullptr);
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct BiContainerValue{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "value(key)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerValue<Container, K, T, true>{
    static jobject function(JNIEnv * env, const void* ptr, jobject key, jobject defaultValue) {
            const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
            return qtjambi_scoped_cast<false,jobject,const T>::cast(env, container->value(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr), qtjambi_scoped_cast<false,T,jobject>::cast(env, defaultValue, nullptr, nullptr)), nullptr, nullptr);
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct BiContainerValues{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "values()" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerValues<Container, K, T, true>{
    static jobject function(JNIEnv * env, const void* ptr) {
            const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
            return qtjambi_scoped_cast<false,jobject,const QList<T>>::cast(env, container->values(), nullptr, nullptr);
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct BiContainerValuesKey{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "values(key)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerValuesKey<Container, K, T, true>{
    static jobject function(JNIEnv * env, const void* ptr, jobject key) {
            const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
            return qtjambi_scoped_cast<false,jobject,const QList<T>>::cast(env, container->values(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr)), nullptr, nullptr);
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct BiContainerClear{
    static void function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "clear()" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerClear<Container, K, T, true>{
    static void function(JNIEnv * __jni_env, void* ptr) {
        Q_UNUSED(__jni_env)
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
        container->clear();
    }
};

template<typename T, bool = supports_less_than<T>::value>
struct BiContainerElementLessThan{
    static bool function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "key1 < key2" QTJAMBI_STACKTRACEINFO );
        return false;
    }
};

template<typename T>
struct BiContainerElementLessThan<T, true>{
    static bool function(JNIEnv * __jni_env, jobject value1, jobject value2) {
        return qtjambi_scoped_cast<false,T,jobject>::cast(__jni_env, value1, nullptr, nullptr) < qtjambi_scoped_cast<false,T,jobject>::cast(__jni_env, value2, nullptr, nullptr);
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct BiContainerInsert{
    static void function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "insert(key,value)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerInsert<Container, K, T, true>{
    static void function(JNIEnv * __jni_env, void* ptr, jobject key, jobject value) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
        container->insert(qtjambi_scoped_cast<false,K,jobject>::cast(__jni_env, key, nullptr, nullptr), qtjambi_scoped_cast<false,T,jobject>::cast(__jni_env, value, nullptr, nullptr));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct BiContainerRemove{
    static jint function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "remove(key)" QTJAMBI_STACKTRACEINFO );
        return 0;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerRemove<Container, K, T, true>{
    static jint function(JNIEnv * __jni_env, void* ptr, jobject key) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
        return container->remove(qtjambi_scoped_cast<false,K,jobject>::cast(__jni_env, key, nullptr, nullptr));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value && std::is_default_constructible<T>::value>
struct BiContainerTake{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "take(key)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerTake<Container, K, T, true>{
    static jobject function(JNIEnv * env, void* ptr, jobject key) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
        return qtjambi_scoped_cast<false,jobject,const T>::cast(env, container->take(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr)), nullptr, nullptr);
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, IsBiContainerFunction isContainer, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct BiContainerUnite{
    static void function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "unite(map)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, IsBiContainerFunction isContainer>
struct BiContainerUnite<Container, K, T, isContainer, true>{
    static void function(JNIEnv * env, void* ptr, jobject other) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
        std::unique_ptr<Container<K,T> > __qt_scoped_pointer;
        Container<K,T> *__qt_other_pointer = nullptr;
        if (other!= nullptr) {
            if (isContainer(env, other, qtjambi_type<K>::id(), QTJAMBI_METATYPE_FROM_TYPE2(K), qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T))) {
                __qt_other_pointer = qtjambi_to_object<Container<K,T>>(env, other);
            } else {
                __qt_scoped_pointer.reset(new Container<K,T> ());
                __qt_other_pointer = __qt_scoped_pointer.get();
                jobject iterator = qtjambi_map_entryset_iterator(env, other);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jobject entry = qtjambi_iterator_next(env, iterator);
                    jobject key = qtjambi_map$entry_key(env, entry);
                    jobject valCollection = qtjambi_map$entry_value(env, entry);
                    jobject iterator2 = qtjambi_collection_iterator(env, valCollection);
                    while(qtjambi_iterator_has_next(env, iterator2)) {
                        jobject val = qtjambi_iterator_next(env, iterator2);
                        __qt_other_pointer->insert(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr), qtjambi_scoped_cast<false,T,jobject>::cast(env, val, nullptr, nullptr));
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
struct BiContainerContainsPair{
    static jboolean function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "contains(key,value)" QTJAMBI_STACKTRACEINFO );
        return false;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerContainsPair<Container, K, T, true>{
    static jboolean function(JNIEnv * env, const void* ptr, jobject key, jobject object) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return container->contains(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr), qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value && supports_equal<T>::value>
struct BiContainerCountPair{
    static jint function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "count(key,value)" QTJAMBI_STACKTRACEINFO );
        return 0;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerCountPair<Container, K, T, true>{
    static jint function(JNIEnv * env, const void* ptr, jobject key, jobject object) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return container->count(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr), qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value && supports_equal<T>::value>
struct BiContainerFindPairs{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "find(key,value)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerFindPairs<Container, K, T, true>{
    static jobject function(JNIEnv *env, QtJambiNativeID nativeId, const void* ptr, jobject key, jobject object) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return qtjambi_BiIterator_caster<typename Container<K,T>::const_iterator>::cast(env, nativeId, container->find(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr), qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr)));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value && supports_equal<T>::value>
struct BiContainerRemovePair{
    static jint function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "remove(key,value)" QTJAMBI_STACKTRACEINFO );
        return 0;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerRemovePair<Container, K, T, true>{
    static jint function(JNIEnv * env, void* ptr, jobject key, jobject object) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
        return container->remove(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr), qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value && supports_equal<T>::value>
struct BiContainerReplacePair{
    static void function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "replace(key,value)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerReplacePair<Container, K, T, true>{
    static void function(JNIEnv * env, void* ptr, jobject key, jobject object) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
        container->replace(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr), qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr));
    }
};

 template<template<typename K, typename T> class Container, typename K, typename T, bool isPointer, bool = std::is_copy_constructible<K>::value && std::is_copy_constructible<T>::value && std::is_default_constructible<K>::value && std::is_default_constructible<T>::value && std::is_copy_constructible<Container<K,T>>::value>
 struct CloneBiContainer{
     static constexpr CopyFunction function = nullptr;
 };

 template<template<typename K, typename T> class Container, typename K, typename T>
 struct CloneBiContainer<Container,K,T,false,true>{
     static void* clone(const void* ptr) { return new Container<K,T>(*reinterpret_cast<const Container<K,T>*>(ptr)); }
     static constexpr CopyFunction function = &clone;
 };

 template<template<typename K, typename T> class Container, typename K, typename T>
 struct DeleteBiContainer{
     static void del(void* ptr,bool) { delete reinterpret_cast<Container<K,T>*>(ptr); }
     static constexpr PtrDeleterFunction function = &del;
 };

template<typename K, typename T, bool isConst = false>
class QMapAccess : public AbstractMapAccess{
protected:
    QMapAccess(){}
public:
    static AbstractMapAccess* newInstance(){
        static QMapAccess<K, T, isConst> instance;
        return &instance;
    }

     AbstractMapAccess* clone() override{
         return this;
     }

    bool isConstant() override {return true;}
    const QMetaType& keyMetaType() override {static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(K)); return type;}
    const QMetaType& valueMetaType() override {static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(T)); return type;}
    void* createContainer() override {return new QMap<K,T>();}
     void* copyContainer(const void* container) override {return container ? new QMap<K,T>(*reinterpret_cast<const QMap<K,T>*>(container)) : createContainer();}
         void assign(void* container, const void* other) override { (*reinterpret_cast<QMap<K,T>*>(container)) = (*reinterpret_cast<const QMap<K,T>*>(other)); }
     void deleteContainer(void* container) override {delete reinterpret_cast<QMap<K,T>*>(container);}
     int registerContainer(const QByteArray& containerTypeName) override {
         return registerMetaType<QMap<K,T>>(containerTypeName);
     }
    PtrDeleterFunction containerDeleter() override {return DeleteBiContainer<QMap,K,T>::function;}

    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        return BiContainerBegin<QMap, K, T>::function(env, ownerId, container);
    }

    void clear(JNIEnv * env,void*) override {
        JavaException::raiseUnsupportedOperationException(env, "QMap::clear()" QTJAMBI_STACKTRACEINFO );
    }

    jboolean contains(JNIEnv * env, const void* container, jobject key) override {
        return BiContainerContains<QMap, K, T>::function(env, container, key);
    }

    jint count(JNIEnv * env, const void* container, jobject key) override {
         return BiContainerCountObject<QMap, K, T>::function(env, container, key);
    }

    jobject end(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
         return BiContainerEnd<QMap, K, T>::function(env, ownerId, container);
    }

    jobject find(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override {
         return BiContainerFindPair<QMap, K, T>::function(env, ownerId, container, key);
    }
    jobject first(JNIEnv * env, const void* container) override {
         return BiContainerFirst<QMap, K, T>::function(env, container);
    }

    jobject firstKey(JNIEnv * env, const void* container) override {
         return BiContainerFirstKey<QMap, K, T>::function(env, container);
    }

    void insert(JNIEnv *env,void*,jobject,jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QMap::insert(key,value)" QTJAMBI_STACKTRACEINFO );
    }

    jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override {
         return BiContainerKey<QMap, K, T>::function(env, container, value, defaultKey);
    }

    jobject keys(JNIEnv * env, const void* container) override {
         return BiContainerKeys<QMap, K, T>::function(env, container);
    }

    jobject keys(JNIEnv * env, const void* container, jobject value) override {
         return BiContainerKeysForValue<QMap, K, T>::function(env, container, value);
    }

    jobject last(JNIEnv * env, const void* container) override {
         return BiContainerLast<QMap, K, T>::function(env, container);
    }

    jobject lastKey(JNIEnv * env, const void* container) override {
         return BiContainerLastKey<QMap, K, T>::function(env, container);
    }

    jobject lowerBound(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override {
         return BiContainerLowerBound<QMap, K, T>::function(env, ownerId, container, key);
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
         return BiContainerEquals<QMap, K, T, qtjambi_is_QMap>::function(env, container, other);
    }

    jint remove(JNIEnv * env,void*,jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QMap::removeAll(key)" QTJAMBI_STACKTRACEINFO );
        return 0;
    }

    jint size(JNIEnv * env, const void* container) override {
         return BiContainerSize<QMap, K, T>::function(env, container);
    }

    jobject take(JNIEnv *env,void*,jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QMap::take(key)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }

    jobject upperBound(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override {
         return BiContainerUpperBound<QMap, K, T>::function(env, ownerId, container, key);
    }

    jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override {
         return BiContainerValue<QMap, K, T>::function(env, container, key, defaultValue);
    }

    jobject values(JNIEnv * env, const void* container) override {
         return BiContainerValues<QMap, K, T>::function(env, container);
    }

    bool keyLessThan(JNIEnv *env, jobject key1, jobject key2) override {
        return BiContainerElementLessThan<K>::function(env, key1, key2);
    }

    void analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) override {
        for(auto iter = reinterpret_cast<const QMap<K,T>*>(container)->begin();
            iter != reinterpret_cast<const QMap<K,T>*>(container)->end();
            ++iter
            ){
            if(!analyzer(pointer_from<decltype(iter.key())>::from(iter.key()),
                         pointer_from<decltype(iter.value())>::from(iter.value()), data)){
                break;
            }
        }
    }
};

template<typename K, typename T>
class QMapAccess<K, T, false> : public QMapAccess<K, T, true>{
protected:
    QMapAccess(){}
public:
    static AbstractMapAccess* newInstance(){
        static QMapAccess<K, T, false> instance;
        return &instance;
    }

     AbstractMapAccess* clone() override{
         return this;
     }

    bool isConstant() override {return false;}

    void clear(JNIEnv * env, void* container) override {
        BiContainerClear<QMap, K, T>::function(env, container);
    }

    void insert(JNIEnv *env, void* container,jobject key,jobject value) override {
        BiContainerInsert<QMap, K, T>::function(env, container, key, value);
    }

    jint remove(JNIEnv * env, void* container,jobject key) override {
        return BiContainerRemove<QMap, K, T>::function(env, container, key);
    }

    jobject take(JNIEnv *env, void* container,jobject key) override {
        return BiContainerTake<QMap, K, T>::function(env, container, key);
    }
};

template<typename K, typename T, bool isConst = false>
class QMultiMapAccess : public AbstractMultiMapAccess{
protected:
    QMultiMapAccess(){}
public:
    static AbstractMultiMapAccess* newInstance(){
        static QMultiMapAccess<K, T, isConst> instance;
        return &instance;
    }

     AbstractMultiMapAccess* clone() override{
         return this;
     }

    bool isConstant() override {return true;}
    const QMetaType& keyMetaType() override {static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(K)); return type;}
    const QMetaType& valueMetaType() override {static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(T)); return type;}
    void* createContainer() override {return new QMultiMap<K,T>();}
         void* copyContainer(const void* container) override {return container ? new QMultiMap<K,T>(*reinterpret_cast<const QMultiMap<K,T>*>(container)) : createContainer();}
         void assign(void* container, const void* other) override { (*reinterpret_cast<QMultiMap<K,T>*>(container)) = (*reinterpret_cast<const QMultiMap<K,T>*>(other)); }
         void deleteContainer(void* container) override {delete reinterpret_cast<QMultiMap<K,T>*>(container);}
         int registerContainer(const QByteArray& containerTypeName) override {
             return registerMetaType<QMultiMap<K,T>>(containerTypeName);
         }
    PtrDeleterFunction containerDeleter() override {return DeleteBiContainer<QMultiMap,K,T>::function;}

    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        return BiContainerBegin<QMultiMap, K, T>::function(env, ownerId, container);
    }

    void clear(JNIEnv * env,void*) override {
        JavaException::raiseUnsupportedOperationException(env, "QMultiMap::clear()" QTJAMBI_STACKTRACEINFO );
    }

    jboolean contains(JNIEnv * env, const void* container, jobject key) override {
        return BiContainerContains<QMultiMap, K, T>::function(env, container, key);
    }

    jint count(JNIEnv * env, const void* container, jobject key) override {
        return BiContainerCountObject<QMultiMap, K, T>::function(env, container, key);
    }

    jobject end(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        return BiContainerEnd<QMultiMap, K, T>::function(env, ownerId, container);
    }

    jobject find(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override {
        return BiContainerFindPair<QMultiMap, K, T>::function(env, ownerId, container, key);
    }
    jobject first(JNIEnv * env, const void* container) override {
        return BiContainerFirst<QMultiMap, K, T>::function(env, container);
    }

    jobject firstKey(JNIEnv * env, const void* container) override {
        return BiContainerFirstKey<QMultiMap, K, T>::function(env, container);
    }

    void insert(JNIEnv *env,void*,jobject,jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QMultiMap::insert(key,value)" QTJAMBI_STACKTRACEINFO );
    }

    jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override {
        return BiContainerKey<QMultiMap, K, T>::function(env, container, value, defaultKey);
    }

    jobject keys(JNIEnv * env, const void* container) override {
        return BiContainerKeys<QMultiMap, K, T>::function(env, container);
    }

    jobject keys(JNIEnv * env, const void* container, jobject value) override {
        return BiContainerKeysForValue<QMultiMap, K, T>::function(env, container, value);
    }

    jobject last(JNIEnv * env, const void* container) override {
        return BiContainerLast<QMultiMap, K, T>::function(env, container);
    }

    jobject lastKey(JNIEnv * env, const void* container) override {
        return BiContainerLastKey<QMultiMap, K, T>::function(env, container);
    }

    jobject lowerBound(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override {
        return BiContainerLowerBound<QMultiMap, K, T>::function(env, ownerId, container, key);
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        return BiContainerEquals<QMultiMap, K, T, qtjambi_is_QMultiMap>::function(env, container, other);
    }

    jint remove(JNIEnv * env,void*,jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QMultiMap::removeAll(key)" QTJAMBI_STACKTRACEINFO );
        return 0;
    }

    jint size(JNIEnv * env, const void* container) override {
        return BiContainerSize<QMultiMap, K, T>::function(env, container);
    }

    jobject take(JNIEnv *env,void*,jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QMultiMap::take(key)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }

    jobject uniqueKeys(JNIEnv * env, const void* container) override {
        return BiContainerUniqueKeys<QMultiMap, K, T>::function(env, container);
    }

    void unite(JNIEnv *env,void*,jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QMultiMap::unite(other)" QTJAMBI_STACKTRACEINFO );
    }

    jobject values(JNIEnv * env, const void* container, jobject key) override {
        return BiContainerValuesKey<QMultiMap, K, T>::function(env, container, key);
    }

    jobject upperBound(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override {
        return BiContainerUpperBound<QMultiMap, K, T>::function(env, ownerId, container, key);
    }

    jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override {
        return BiContainerValue<QMultiMap, K, T>::function(env, container, key, defaultValue);
    }

    jobject values(JNIEnv * env, const void* container) override {
        return BiContainerValues<QMultiMap, K, T>::function(env, container);
    }

    jboolean contains(JNIEnv *env, const void* container, jobject key, jobject value) override {
         return BiContainerContainsPair<QMultiMap, K, T>::function(env, container, key, value);
    }

    jint count(JNIEnv *env, const void* container, jobject key, jobject value) override {
         return BiContainerCountPair<QMultiMap, K, T>::function(env, container, key, value);
    }

    jobject find(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key, jobject value) override {
        return BiContainerFindPairs<QMultiMap, K, T>::function(env, ownerId, container, key, value);
    }

    jint remove(JNIEnv * env, void*,jobject, jobject) override {
         JavaException::raiseUnsupportedOperationException(env, "QMultiMap::remove(key, value)" QTJAMBI_STACKTRACEINFO );
         return 0;
    }

    void replace(JNIEnv *env, void*,jobject, jobject) override {
         JavaException::raiseUnsupportedOperationException(env, "QMultiMap::replace(key, value)" QTJAMBI_STACKTRACEINFO );
    }

    bool keyLessThan(JNIEnv *env, jobject key1, jobject key2) override {
        return BiContainerElementLessThan<K>::function(env, key1, key2);
    }

    void analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) override {
        for(auto iter = reinterpret_cast<const QMultiMap<K,T>*>(container)->begin();
            iter != reinterpret_cast<const QMultiMap<K,T>*>(container)->end();
            ++iter
            ){
            if(!analyzer(pointer_from<decltype(iter.key())>::from(iter.key()),
                         pointer_from<decltype(iter.value())>::from(iter.value()), data)){
                break;
            }
        }
    }
};

template<typename K, typename T>
class QMultiMapAccess<K, T, false> : public QMultiMapAccess<K, T, true>{
protected:
    QMultiMapAccess(){}
public:
    static AbstractMultiMapAccess* newInstance(){
        static QMultiMapAccess<K, T, false> instance;
        return &instance;
    }

     AbstractMultiMapAccess* clone() override{
         return this;
     }

    bool isConstant() override {return false;}

    jint remove(JNIEnv * env, void* container,jobject key, jobject value) override {
         return BiContainerRemovePair<QMultiMap, K, T>::function(env, container, key, value);
    }

    void replace(JNIEnv * env, void* container,jobject key, jobject value) override {
         BiContainerReplacePair<QMultiMap, K, T>::function(env, container, key, value);
    }

    void clear(JNIEnv * env, void* container) override {
         BiContainerClear<QMultiMap, K, T>::function(env, container);
    }

    void insert(JNIEnv *env, void* container,jobject key,jobject value) override {
         BiContainerInsert<QMultiMap, K, T>::function(env, container, key, value);
    }

    jint remove(JNIEnv * env, void* container,jobject key) override {
         return BiContainerRemove<QMultiMap, K, T>::function(env, container, key);
    }

    jobject take(JNIEnv *env, void* container,jobject key) override {
         return BiContainerTake<QMultiMap, K, T>::function(env, container, key);
    }

    void unite(JNIEnv *env, void* container, jobject other) override {
         BiContainerUnite<QMultiMap, K, T, qtjambi_is_QMultiMap>::function(env, container, other);
    }
};

 template<typename K, typename T, bool isConst = false>
 class QHashAccess : public AbstractHashAccess{
 protected:
     QHashAccess(){}
 public:
     static AbstractHashAccess* newInstance(){
         static QHashAccess<K, T, isConst> instance;
         return &instance;
     }

     AbstractHashAccess* clone() override{
         return this;
     }

     bool isConstant() override {return true;}
     const QMetaType& keyMetaType() override {static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(K)); return type;}
     const QMetaType& valueMetaType() override {static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(T)); return type;}
     void* createContainer() override {return new QHash<K,T>();}
         void* copyContainer(const void* container) override {return container ? new QHash<K,T>(*reinterpret_cast<const QHash<K,T>*>(container)) : createContainer();}
         void assign(void* container, const void* other) override { (*reinterpret_cast<QHash<K,T>*>(container)) = (*reinterpret_cast<const QHash<K,T>*>(other)); }
         void deleteContainer(void* container) override {delete reinterpret_cast<QHash<K,T>*>(container);}
         int registerContainer(const QByteArray& containerTypeName) override {
             return registerMetaType<QHash<K,T>>(containerTypeName);
         }
     PtrDeleterFunction containerDeleter() override {return DeleteBiContainer<QHash,K,T>::function;}

     jobject begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
         return BiContainerBegin<QHash, K, T>::function(env, ownerId, container);
     }

     jint capacity(JNIEnv * env, const void* container) override {
         return BiContainerCapacity<QHash, K, T>::function(env, container);
     }

     void clear(JNIEnv * env,void*) override {
         JavaException::raiseUnsupportedOperationException(env, "QHash::clear()" QTJAMBI_STACKTRACEINFO );
     }

     jboolean contains(JNIEnv * env, const void* container, jobject key) override {
         return BiContainerContains<QHash, K, T>::function(env, container, key);
     }

     jint count(JNIEnv * env, const void* container, jobject key) override {
          return BiContainerCountObject<QHash, K, T>::function(env, container, key);
     }

     jobject end(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
          return BiContainerEnd<QHash, K, T>::function(env, ownerId, container);
     }

     jobject find(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override {
          return BiContainerFindPair<QHash, K, T>::function(env, ownerId, container, key);
     }

     void insert(JNIEnv *env,void*,jobject,jobject) override {
         JavaException::raiseUnsupportedOperationException(env, "QHash::insert(key,value)" QTJAMBI_STACKTRACEINFO );
     }

     jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override {
          return BiContainerKey<QHash, K, T>::function(env, container, value, defaultKey);
     }

     jobject keys(JNIEnv * env, const void* container) override {
          return BiContainerKeys<QHash, K, T>::function(env, container);
     }

     jobject keys(JNIEnv * env, const void* container, jobject value) override {
          return BiContainerKeysForValue<QHash, K, T>::function(env, container, value);
     }

     jboolean equal(JNIEnv * env, const void* container, jobject other) override {
          return BiContainerEquals<QHash, K, T, qtjambi_is_QHash>::function(env, container, other);
     }

     jint remove(JNIEnv * env,void*,jobject) override {
         JavaException::raiseUnsupportedOperationException(env, "QHash::remove(key)" QTJAMBI_STACKTRACEINFO );
         return 0;
     }

     void reserve(JNIEnv * env,void*,jint) override {
         JavaException::raiseUnsupportedOperationException(env, "QHash::reserve(size)" QTJAMBI_STACKTRACEINFO );
     }

     jint size(JNIEnv * env, const void* container) override {
          return BiContainerSize<QHash, K, T>::function(env, container);
     }

     jobject take(JNIEnv *env,void*,jobject) override {
         JavaException::raiseUnsupportedOperationException(env, "QHash::take(key)" QTJAMBI_STACKTRACEINFO );
         return nullptr;
     }

     jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override {
          return BiContainerValue<QHash, K, T>::function(env, container, key, defaultValue);
     }

     jobject values(JNIEnv * env, const void* container) override {
          return BiContainerValues<QHash, K, T>::function(env, container);
     }

     void analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) override {
         for(auto iter = reinterpret_cast<const QHash<K,T>*>(container)->begin();
             iter != reinterpret_cast<const QHash<K,T>*>(container)->end();
             ++iter
             ){
             if(!analyzer(pointer_from<decltype(iter.key())>::from(iter.key()),
                          pointer_from<decltype(iter.value())>::from(iter.value()), data)){
                 break;
             }
         }
     }
 };

 template<typename K, typename T>
 class QHashAccess<K, T, false> : public QHashAccess<K, T, true>{
 protected:
     QHashAccess(){}
 public:
     static AbstractHashAccess* newInstance(){
         static QHashAccess<K, T, false> instance;
         return &instance;
     }

     AbstractHashAccess* clone() override{
         return this;
     }

     bool isConstant() override {return false;}

     void clear(JNIEnv * env, void* container) override {
         BiContainerClear<QHash, K, T>::function(env, container);
     }
     void insert(JNIEnv *env, void* container,jobject key,jobject value) override {
         BiContainerInsert<QHash, K, T>::function(env, container, key, value);
     }
     jint remove(JNIEnv * env, void* container,jobject key) override {
         return BiContainerRemove<QHash, K, T>::function(env, container, key);
     }
     jobject take(JNIEnv *env, void* container,jobject key) override {
         return BiContainerTake<QHash, K, T>::function(env, container, key);
     }
     void reserve(JNIEnv * env,void* container, jint newSize) override {
         BiContainerReserve<QHash, K, T>::function(env, container, newSize);
     }
 };

 template<typename K, typename T, bool isConst = false>
 class QMultiHashAccess : public AbstractMultiHashAccess{
 protected:
     QMultiHashAccess(){}
 public:
     static AbstractMultiHashAccess* newInstance(){
         static QMultiHashAccess<K, T, isConst> instance;
         return &instance;
     }

     AbstractMultiHashAccess* clone() override{
         return this;
     }

     bool isConstant() override {return true;}
     const QMetaType& keyMetaType() override {static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(K)); return type;}
     const QMetaType& valueMetaType() override {static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(T)); return type;}
     void* createContainer() override {return new QMultiHash<K,T>();}
         void* copyContainer(const void* container) override {return container ? new QMultiHash<K,T>(*reinterpret_cast<const QMultiHash<K,T>*>(container)) : createContainer();}
         void assign(void* container, const void* other) override { (*reinterpret_cast<QMultiHash<K,T>*>(container)) = (*reinterpret_cast<const QMultiHash<K,T>*>(other)); }
         void deleteContainer(void* container) override {delete reinterpret_cast<QMultiHash<K,T>*>(container);}
         int registerContainer(const QByteArray& containerTypeName) override {
             return registerMetaType<QMultiHash<K,T>>(containerTypeName);
         }
     PtrDeleterFunction containerDeleter() override {return DeleteBiContainer<QMultiHash,K,T>::function;}

     jobject begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
         return BiContainerBegin<QMultiHash, K, T>::function(env, ownerId, container);
     }

     jint capacity(JNIEnv * env, const void* container) override {
         return BiContainerCapacity<QMultiHash, K, T>::function(env, container);
     }

     void clear(JNIEnv * env,void*) override {
         JavaException::raiseUnsupportedOperationException(env, "QMultiHash::clear()" QTJAMBI_STACKTRACEINFO );
     }

     jboolean contains(JNIEnv * env, const void* container, jobject key) override {
         return BiContainerContains<QMultiHash, K, T>::function(env, container, key);
     }

     jint count(JNIEnv * env, const void* container, jobject key) override {
         return BiContainerCountObject<QMultiHash, K, T>::function(env, container, key);
     }

     jobject end(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
         return BiContainerEnd<QMultiHash, K, T>::function(env, ownerId, container);
     }

     jobject find(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override {
         return BiContainerFindPair<QMultiHash, K, T>::function(env, ownerId, container, key);
     }

     void insert(JNIEnv *env,void*,jobject,jobject) override {
         JavaException::raiseUnsupportedOperationException(env, "QMultiHash::insert(key,value)" QTJAMBI_STACKTRACEINFO );
     }

     jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override {
         return BiContainerKey<QMultiHash, K, T>::function(env, container, value, defaultKey);
     }

     jobject keys(JNIEnv * env, const void* container) override {
         return BiContainerKeys<QMultiHash, K, T>::function(env, container);
     }

     jobject keys(JNIEnv * env, const void* container, jobject value) override {
         return BiContainerKeysForValue<QMultiHash, K, T>::function(env, container, value);
     }

     jboolean equal(JNIEnv * env, const void* container, jobject other) override {
         return BiContainerEquals<QMultiHash, K, T, qtjambi_is_QMultiHash>::function(env, container, other);
     }

     jint remove(JNIEnv * env,void*,jobject) override {
         JavaException::raiseUnsupportedOperationException(env, "QMultiHash::remove(key)" QTJAMBI_STACKTRACEINFO );
        return 0;
     }

     void reserve(JNIEnv * env,void*,jint) override {
         JavaException::raiseUnsupportedOperationException(env, "QMultiHash::reserve(size)" QTJAMBI_STACKTRACEINFO );
     }

     jint size(JNIEnv * env, const void* container) override {
         return BiContainerSize<QMultiHash, K, T>::function(env, container);
     }

     jobject take(JNIEnv *env,void*,jobject) override {
         JavaException::raiseUnsupportedOperationException(env, "QMultiHash::take(key)" QTJAMBI_STACKTRACEINFO );
         return nullptr;
     }

     jobject uniqueKeys(JNIEnv * env, const void* container) override {
         return BiContainerUniqueKeys<QMultiHash, K, T>::function(env, container);
     }

     void unite(JNIEnv *env,void*,jobject) override {
         JavaException::raiseUnsupportedOperationException(env, "QMultiHash::unite(other)" QTJAMBI_STACKTRACEINFO );
     }

     jobject values(JNIEnv * env, const void* container, jobject key) override {
         return BiContainerValuesKey<QMultiHash, K, T>::function(env, container, key);
     }

     jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override {
         return BiContainerValue<QMultiHash, K, T>::function(env, container, key, defaultValue);
     }

     jobject values(JNIEnv * env, const void* container) override {
         return BiContainerValues<QMultiHash, K, T>::function(env, container);
     }

     jboolean contains(JNIEnv *env, const void* container, jobject key, jobject value) override {
          return BiContainerContainsPair<QMultiHash, K, T>::function(env, container, key, value);
     }

     jint count(JNIEnv *env, const void* container, jobject key, jobject value) override {
          return BiContainerCountPair<QMultiHash, K, T>::function(env, container, key, value);
     }

     jobject find(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key, jobject value) override {
         return BiContainerFindPairs<QMultiHash, K, T>::function(env, ownerId, container, key, value);
     }

     jint remove(JNIEnv * env, void*,jobject, jobject) override {
          JavaException::raiseUnsupportedOperationException(env, "QMultiHash::remove(key, value)" QTJAMBI_STACKTRACEINFO );
          return 0;
     }

     void replace(JNIEnv *env, void*,jobject, jobject) override {
          JavaException::raiseUnsupportedOperationException(env, "QMultiHash::replace(key, value)" QTJAMBI_STACKTRACEINFO );
     }

     void analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) override {
         for(auto iter = reinterpret_cast<const QMultiHash<K,T>*>(container)->begin();
             iter != reinterpret_cast<const QMultiHash<K,T>*>(container)->end();
             ++iter
             ){
             if(!analyzer(pointer_from<decltype(iter.key())>::from(iter.key()),
                          pointer_from<decltype(iter.value())>::from(iter.value()), data)){
                 break;
             }
         }
     }
 };

 template<typename K, typename T>
 class QMultiHashAccess<K, T, false> : public QMultiHashAccess<K, T, true>{
 protected:
     QMultiHashAccess(){}
 public:
     static AbstractMultiHashAccess* newInstance(){
         static QMultiHashAccess<K, T, false> instance;
         return &instance;
     }

     AbstractMultiHashAccess* clone() override{
         return this;
     }

     bool isConstant() override {return false;}

     void clear(JNIEnv * env, void* container) override {
         BiContainerClear<QMultiHash, K, T>::function(env, container);
     }
     void insert(JNIEnv *env, void* container,jobject key,jobject value) override {
         BiContainerInsert<QMultiHash, K, T>::function(env, container, key, value);
     }
     jint remove(JNIEnv * env, void* container,jobject key) override {
         return BiContainerRemove<QMultiHash, K, T>::function(env, container, key);
     }
     jobject take(JNIEnv *env, void* container,jobject key) override {
         return BiContainerTake<QMultiHash, K, T>::function(env, container, key);
     }

     jint remove(JNIEnv * env, void* container,jobject key, jobject value) override {
          return BiContainerRemovePair<QMultiHash, K, T>::function(env, container, key, value);
     }

     void replace(JNIEnv * env, void* container,jobject key, jobject value) override {
          BiContainerReplacePair<QMultiHash, K, T>::function(env, container, key, value);
     }

     void unite(JNIEnv *env, void* container, jobject other) override {
          BiContainerUnite<QMultiHash, K, T, qtjambi_is_QMultiHash>::function(env, container, other);
     }
 };


template<bool has_scope, typename Container, bool is_const, typename K, typename T, void (*put)(JNIEnv *, jobject, jobject, jobject)>
struct qtjambi_bicollection_fill{

};

template<bool has_scope, typename Container, typename K, typename T, void (*put)(JNIEnv *, jobject, jobject, jobject)>
struct qtjambi_bicollection_fill<has_scope, Container, true, K, T, put>{
    static void fill(JNIEnv *env, const Container& container, jobject collection, QtJambiScope* scope){
        for(typename Container::const_iterator i = container.begin(); i!=container.end(); ++i){
            put(env, collection,
                qtjambi_scoped_cast<has_scope,jobject,typename std::add_const<decltype(i.key())>::type>::cast(env, i.key(), nullptr, scope),
                qtjambi_scoped_cast<has_scope,jobject,typename std::add_const<decltype(i.value())>::type>::cast(env, i.value(), nullptr, scope));
        }
    }
};

template<bool has_scope, typename Container, typename K, typename T, void (*put)(JNIEnv *, jobject, jobject, jobject)>
struct qtjambi_bicollection_fill<has_scope, Container, false, K, T, put>{
    static void fill(JNIEnv *env, Container& container, jobject collection, QtJambiScope* scope){
        for(typename Container::iterator i = container.begin(); i!=container.end(); ++i){
            put(env, collection,
                qtjambi_scoped_cast<has_scope,jobject,typename std::add_const<decltype(i.key())>::type>::cast(env, i.key(), nullptr, scope),
                qtjambi_scoped_cast<has_scope,jobject,typename std::add_const<decltype(i.value())>::type>::cast(env, i.value(), nullptr, scope));
        }
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool isConst = false>
struct BiContainerAccess{
    static constexpr AbstractContainerAccess* newInstance(){return nullptr;}
};

template<typename K, typename T, bool isConst>
struct BiContainerAccess<QMap,K,T,isConst> : QMapAccess<K,T,isConst>{
};

template<typename K, typename T, bool isConst>
struct BiContainerAccess<QMultiMap,K,T,isConst> : QMultiMapAccess<K,T,isConst>{
};

template<typename K, typename T, bool isConst>
struct BiContainerAccess<QHash,K,T,isConst> : QHashAccess<K,T,isConst>{
};

template<typename K, typename T, bool isConst>
struct BiContainerAccess<QMultiHash,K,T,isConst> : QMultiHashAccess<K,T,isConst>{
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_MAP_P_H
