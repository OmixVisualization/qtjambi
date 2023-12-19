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

#ifndef QTJAMBI_CAST_IMPL_CONTAINER_ASSOCIATIVE_H
#define QTJAMBI_CAST_IMPL_CONTAINER_ASSOCIATIVE_H

#include "qtjambi_cast_impl_util.h"
#include "qtjambi_cast_impl_container_iterator.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QT_WARNING_DISABLE_GCC("-Wdeprecated-declarations")
QT_WARNING_DISABLE_DEPRECATED
#endif

namespace RegistryAPI{
template<typename T>
int registerMetaType(const QByteArray& typeName);
}

enum class QtJambiNativeID : jlong;

namespace QtJambiPrivate {

template<template<typename K, typename T> class Container, typename K, typename T>
struct IntermediateAssociativeContainer : Container<K,T>{
    IntermediateAssociativeContainer(JNIEnv *env, jobject object, QtJambiScope& scope) : Container<K,T>(), m_scope(scope), m_object(env->NewWeakGlobalRef(object)){}
    ~IntermediateAssociativeContainer(){
        QTJAMBI_TRY_ANY{
            if(JniEnvironment env{200}){
                QTJAMBI_TRY{
                    jobject object = env->NewLocalRef(m_object);
                    if(!env->IsSameObject(object, nullptr)){
                        QtJambiAPI::clearJavaMap(env, object);
                        for(typename Container<K,T>::const_iterator i = Container<K,T>::constBegin(); i!=Container<K,T>::constEnd(); ++i){
                            jobject key = qtjambi_scoped_cast<true,jobject,decltype(i.key())>::cast(env, i.key(), nullptr, &m_scope);
                            jobject val = qtjambi_scoped_cast<true,jobject,decltype(i.value())>::cast(env, i.value(), nullptr, &m_scope);
                            QtJambiAPI::putJavaMap(env, object, key, val);
                        }
                    }
                }QTJAMBI_CATCH(const JavaException& exn){
                    exn.raiseInJava(env);
                }QTJAMBI_TRY_END
            }
        }QTJAMBI_CATCH_ANY{
            printf("An unknown exception occurred.\n");
        }QTJAMBI_TRY_END
    }
    QtJambiScope& m_scope;
    jobject m_object;
};

typedef bool (*IsAssociativeContainerFunction)(JNIEnv *, jobject, const std::type_info&, const QMetaType&, const std::type_info&, const QMetaType&, void*& pointer);

template<typename Iterator>
class QAssociativeConstIteratorAccess : public AbstractConstIteratorAccess<Iterator,AbstractAssociativeConstIteratorAccess>{
protected:
    QAssociativeConstIteratorAccess(){}
public:
    static QAssociativeConstIteratorAccess<Iterator>* newInstance(){
        static QAssociativeConstIteratorAccess<Iterator> instance;
        return &instance;
    }

    AbstractSequentialConstIteratorAccess* clone() override{
        return this;
    }

    void dispose() override {}

    jobject value(JNIEnv * env, const void* ptr) override {
        const Iterator* iterator = static_cast<const Iterator*>(ptr);
        const auto& value = iterator->value();
        return qtjambi_scoped_cast<false,jobject,decltype(value)>::cast(env, value, nullptr, nullptr);
    }
    jobject key(JNIEnv * env, const void* ptr) override {
        const Iterator* iterator = static_cast<const Iterator*>(ptr);
        const auto& key = iterator->key();
        return qtjambi_scoped_cast<false,jobject,decltype(key)>::cast(env, key, nullptr, nullptr);
    }
};

template<typename Iterator>
class QAssociativeIteratorAccess : public virtual QAssociativeConstIteratorAccess<Iterator>, public virtual AbstractAssociativeIteratorAccess{
private:
    QAssociativeIteratorAccess(){}
public:
    static QAssociativeConstIteratorAccess<Iterator>* newInstance(){
        static QAssociativeIteratorAccess<Iterator> instance;
        return &instance;
    }

    void dispose() override {}

    QAssociativeConstIteratorAccess<Iterator>* clone() override{
        return this;
    }

    jobject value(JNIEnv * env, const void* ptr) override {
        return QAssociativeConstIteratorAccess<Iterator>::value(env, ptr);
    }

    jobject key(JNIEnv * env, const void* ptr) override {
        return QAssociativeConstIteratorAccess<Iterator>::key(env, ptr);
    }

    void increment(JNIEnv *env, void* iterator) override {
        QAssociativeConstIteratorAccess<Iterator>::increment(env, iterator);
    }
    void decrement(JNIEnv *env, void* iterator) override {
        QAssociativeConstIteratorAccess<Iterator>::decrement(env, iterator);
    }
    jboolean lessThan(JNIEnv *env, const void* iterator, const void* other) override {
        return QAssociativeConstIteratorAccess<Iterator>::lessThan(env, iterator, other);
    }
    bool canLess() override {
        return QAssociativeConstIteratorAccess<Iterator>::canLess();
    }
    jboolean equals(JNIEnv *env, const void* ptr, const void* ptr2) override {
        return QAssociativeConstIteratorAccess<Iterator>::equals(env, ptr, ptr2);
    }

    void setValue(JNIEnv * env, void* ptr, jobject newValue) override {
        Iterator* iterator = static_cast<Iterator*>(ptr);
        iterator->value() = qtjambi_scoped_cast<false,typename std::remove_reference<decltype(iterator->value())>::type,jobject>::cast(env, newValue, nullptr, nullptr);
    }
};

template<typename Iterator> struct supports_mutableAssIterator : std::conditional<std::is_reference<decltype(std::declval<Iterator>().value())>::value
                                                                && !std::is_const<typename std::remove_reference<decltype(std::declval<Iterator>().value())>::type>::value, std::true_type, std::false_type>::type{};

template<typename Iterator, bool isMutable = supports_mutableAssIterator<Iterator>::value>
struct qtjambi_associative_iterator_caster{
    static jobject cast(JNIEnv * env, QtJambiNativeID nativeId, typename std::conditional<std::is_pointer<Iterator>::value, Iterator, const Iterator&>::type iter){
        return QtJambiAPI::convertQAssociativeIteratorToJavaObject(env, nativeId,
            new Iterator(iter),
            [](void* ptr,bool) {
               delete reinterpret_cast<Iterator*>(ptr);
            },
            QAssociativeConstIteratorAccess<Iterator>::newInstance()
        );
    }
};

template<typename Iterator>
struct qtjambi_associative_iterator_caster<Iterator,true>{
    struct IteratorContainer{
        Iterator i;
    };
    static jobject cast(JNIEnv * env, QtJambiNativeID nativeId, typename std::conditional<std::is_pointer<Iterator>::value, Iterator, const Iterator&>::type iter){
        return QtJambiAPI::convertQAssociativeIteratorToJavaObject(env, nativeId,
            new IteratorContainer{iter},
            [](void* ptr,bool) {
               delete reinterpret_cast<IteratorContainer*>(ptr);
            },
            QAssociativeIteratorAccess<Iterator>::newInstance()
        );
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerBegin{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "begin()" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerBegin<Container, K, T, true>{
    static jobject function(JNIEnv *env, QtJambiNativeID nativeId, void* ptr) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
        return qtjambi_associative_iterator_caster<decltype(container->begin())>::cast(env, nativeId, container->begin());
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerConstBegin{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "constBegin()" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerConstBegin<Container, K, T, true>{
    static jobject function(JNIEnv *env, QtJambiNativeID nativeId, const void* ptr) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return qtjambi_associative_iterator_caster<decltype(container->constBegin())>::cast(env, nativeId, container->constBegin());
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerCapacity{
    static jint function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "capacity()" QTJAMBI_STACKTRACEINFO );
        return 0;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerCapacity<Container, K, T, true>{
    static jint function(JNIEnv *, const void* ptr) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return jint(container->capacity());
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerReserve{
    static void function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "reserve(size)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerReserve<Container, K, T, true>{
    static void function(JNIEnv *, void* ptr, jint size) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
        container->reserve(size);
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerContains{
    static jboolean function(JNIEnv * env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "contains(key)" QTJAMBI_STACKTRACEINFO );
        return false;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerContains<Container, K, T, true>{
    static jboolean function(JNIEnv * env, const void* ptr, jobject object) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return container->contains(qtjambi_scoped_cast<false,K,jobject>::cast(env, object, nullptr, nullptr));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerCountObject{
    static jint function(JNIEnv * env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "count(key)" QTJAMBI_STACKTRACEINFO );
        return 0;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerCountObject<Container, K, T, true>{
    static jint function(JNIEnv * env, const void* ptr, jobject object) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return container->count(qtjambi_scoped_cast<false,K,jobject>::cast(env, object, nullptr, nullptr));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerEnd{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "end()" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerEnd<Container, K, T, true>{
    static jobject function(JNIEnv *env, QtJambiNativeID nativeId, void* ptr) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
        return qtjambi_associative_iterator_caster<decltype(container->end())>::cast(env, nativeId, container->end());
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerConstEnd{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "constEnd()" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerConstEnd<Container, K, T, true>{
    static jobject function(JNIEnv *env, QtJambiNativeID nativeId, const void* ptr) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return qtjambi_associative_iterator_caster<decltype(container->constEnd())>::cast(env, nativeId, container->constEnd());
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerFindPair{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "find(key,value)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerFindPair<Container, K, T, true>{
    static jobject function(JNIEnv *env, QtJambiNativeID nativeId, void* ptr, jobject key) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
        return qtjambi_associative_iterator_caster<typename Container<K,T>::iterator>::cast(env, nativeId, container->find(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr)));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerConstFindPair{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "constFind(key,value)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerConstFindPair<Container, K, T, true>{
    static jobject function(JNIEnv *env, QtJambiNativeID nativeId, const void* ptr, jobject key) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return qtjambi_associative_iterator_caster<typename Container<K,T>::const_iterator>::cast(env, nativeId, container->constFind(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr)));
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
        return qtjambi_scoped_cast<false,jobject,const T>::cast(env, container->first(), nullptr, nullptr);
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
        return qtjambi_scoped_cast<false,jobject,const T>::cast(env, container->last(), nullptr, nullptr);
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
        return qtjambi_scoped_cast<false,jobject,const K>::cast(env, container->firstKey(), nullptr, nullptr);
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
        return qtjambi_scoped_cast<false,jobject,const K>::cast(env, container->lastKey(), nullptr, nullptr);
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
        return qtjambi_scoped_cast<false,jobject,const K>::cast(env, container->key(qtjambi_scoped_cast<false,T,jobject>::cast(env, value, nullptr, nullptr), qtjambi_scoped_cast<false,K,jobject>::cast(env, defaultKey, nullptr, nullptr)), nullptr, nullptr);
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerKeys{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "keys()" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerKeys<Container, K, T, true>{
    static jobject function(JNIEnv *env, const void* ptr) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return qtjambi_scoped_cast<false,jobject,const QList<K>>::cast(env, container->keys(), nullptr, nullptr);
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_keys_by_value<Container,K,T>::value && supports_map_sort<Container,K,T>::value && supports_equal<K>::value && supports_equal<T>::value>
struct AssociativeContainerKeysForValue{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "keys(value)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerKeysForValue<Container, K, T, true>{
    static jobject function(JNIEnv *env, const void* ptr, jobject value) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return qtjambi_scoped_cast<false,jobject,const QList<K>>::cast(env, container->keys(qtjambi_scoped_cast<false,T,jobject>::cast(env, value, nullptr, nullptr)), nullptr, nullptr);
    }
};

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
template<typename K, typename T>
struct AssociativeContainerKeysForValue<QMultiHash, K, T, true>{
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
struct AssociativeContainerLowerBound{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "lowerBounds(value)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerLowerBound<Container, K, T, true>{
    static jobject function(JNIEnv *env, QtJambiNativeID nativeId, const void* ptr, jobject key) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return qtjambi_associative_iterator_caster<typename Container<K,T>::const_iterator>::cast(env, nativeId, container->lowerBound(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr)));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerUpperBound{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "upperBounds(value)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerUpperBound<Container, K, T, true>{
    static jobject function(JNIEnv *env, QtJambiNativeID nativeId, const void* ptr, jobject key) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return qtjambi_associative_iterator_caster<typename Container<K,T>::const_iterator>::cast(env, nativeId, container->upperBound(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr)));
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
            if (!isContainer(env, other, qtjambi_type<K>::id(), QTJAMBI_METATYPE_FROM_TYPE2(K), qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(__qt_other_pointer))) {
                __qt_scoped_pointer.reset(new Container<K,T>());
                __qt_other_pointer = __qt_scoped_pointer.get();
                jobject iterator = QtJambiAPI::entrySetIteratorOfJavaMap(env, other);
                while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                    jobject entry = QtJambiAPI::nextOfJavaIterator(env, iterator);
                    jobject key = QtJambiAPI::keyOfJavaMapEntry(env, entry);
                    jobject val = QtJambiAPI::valueOfJavaMapEntry(env, entry);
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
struct AssociativeContainerSize{
    static jint function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "size()" QTJAMBI_STACKTRACEINFO );
        return 0;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerSize<Container, K, T, true>{
    static jint function(JNIEnv *, const void* ptr) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return container->size();
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerUniqueKeys{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "uniqueKeys()" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerUniqueKeys<Container, K, T, true>{
    static jobject function(JNIEnv *env, const void* ptr) {
            const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
            return qtjambi_scoped_cast<false,jobject,const QList<K>>::cast(env, container->uniqueKeys(), nullptr, nullptr);
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerValue{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "value(key)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerValue<Container, K, T, true>{
    static jobject function(JNIEnv * env, const void* ptr, jobject key, jobject defaultValue) {
            const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
            return qtjambi_scoped_cast<false,jobject,const T>::cast(env, container->value(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr), qtjambi_scoped_cast<false,T,jobject>::cast(env, defaultValue, nullptr, nullptr)), nullptr, nullptr);
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerValues{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "values()" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerValues<Container, K, T, true>{
    static jobject function(JNIEnv * env, const void* ptr) {
            const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
            return qtjambi_scoped_cast<false,jobject,const QList<T>>::cast(env, container->values(), nullptr, nullptr);
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerValuesKey{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "values(key)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerValuesKey<Container, K, T, true>{
    static jobject function(JNIEnv * env, const void* ptr, jobject key) {
            const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
            return qtjambi_scoped_cast<false,jobject,const QList<T>>::cast(env, container->values(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr)), nullptr, nullptr);
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
    static void function(JNIEnv * __jni_env, void* ptr) {
        Q_UNUSED(__jni_env)
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
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
        return qtjambi_scoped_cast<false,T,jobject>::cast(__jni_env, value1, nullptr, nullptr) < qtjambi_scoped_cast<false,T,jobject>::cast(__jni_env, value2, nullptr, nullptr);
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerInsert{
    static void function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "insert(key,value)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerInsert<Container, K, T, true>{
    static void function(JNIEnv * __jni_env, void* ptr, jobject key, jobject value) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
        container->insert(qtjambi_scoped_cast<false,K,jobject>::cast(__jni_env, key, nullptr, nullptr), qtjambi_scoped_cast<false,T,jobject>::cast(__jni_env, value, nullptr, nullptr));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerRemove{
    static jint function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "remove(key)" QTJAMBI_STACKTRACEINFO );
        return 0;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerRemove<Container, K, T, true>{
    static jint function(JNIEnv * __jni_env, void* ptr, jobject key) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
        return container->remove(qtjambi_scoped_cast<false,K,jobject>::cast(__jni_env, key, nullptr, nullptr));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value && is_default_constructible<T>::value>
struct AssociativeContainerTake{
    static jobject function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "take(key)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerTake<Container, K, T, true>{
    static jobject function(JNIEnv * env, void* ptr, jobject key) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
        return qtjambi_scoped_cast<false,jobject,const T>::cast(env, container->take(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr)), nullptr, nullptr);
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, IsAssociativeContainerFunction isContainer, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value>
struct AssociativeContainerUnite{
    static void function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "unite(map)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, IsAssociativeContainerFunction isContainer>
struct AssociativeContainerUnite<Container, K, T, isContainer, true>{
    static void function(JNIEnv * env, void* ptr, jobject other) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
        std::unique_ptr<Container<K,T> > __qt_scoped_pointer;
        Container<K,T> *__qt_other_pointer = nullptr;
        if (other!= nullptr) {
            if (!isContainer(env, other, qtjambi_type<K>::id(), QTJAMBI_METATYPE_FROM_TYPE2(K), qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(__qt_other_pointer))) {
                __qt_scoped_pointer.reset(new Container<K,T> ());
                __qt_other_pointer = __qt_scoped_pointer.get();
                jobject iterator = QtJambiAPI::entrySetIteratorOfJavaMap(env, other);
                while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                    jobject entry = QtJambiAPI::nextOfJavaIterator(env, iterator);
                    jobject key = QtJambiAPI::keyOfJavaMapEntry(env, entry);
                    jobject valCollection = QtJambiAPI::valueOfJavaMapEntry(env, entry);
                    jobject iterator2 = QtJambiAPI::iteratorOfJavaCollection(env, valCollection);
                    while(QtJambiAPI::hasJavaIteratorNext(env, iterator2)) {
                        jobject val = QtJambiAPI::nextOfJavaIterator(env, iterator2);
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
        return container->contains(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr), qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value && supports_equal<T>::value>
struct AssociativeContainerCountPair{
    static jint function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "count(key,value)" QTJAMBI_STACKTRACEINFO );
        return 0;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerCountPair<Container, K, T, true>{
    static jint function(JNIEnv * env, const void* ptr, jobject key, jobject object) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return container->count(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr), qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr));
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
    static jobject function(JNIEnv *env, QtJambiNativeID nativeId, void* ptr, jobject key, jobject object) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
        return qtjambi_associative_iterator_caster<typename Container<K,T>::iterator>::cast(env, nativeId, container->find(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr), qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr)));
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
    static jobject function(JNIEnv *env, QtJambiNativeID nativeId, const void* ptr, jobject key, jobject object) {
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return qtjambi_associative_iterator_caster<typename Container<K,T>::const_iterator>::cast(env, nativeId, container->constFind(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr), qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr)));
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool = supports_map_sort<Container,K,T>::value && supports_equal<K>::value && supports_equal<T>::value>
struct AssociativeContainerRemovePair{
    static jint function(JNIEnv *env, ...) {
        JavaException::raiseUnsupportedOperationException(env, "remove(key,value)" QTJAMBI_STACKTRACEINFO );
        return 0;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerRemovePair<Container, K, T, true>{
    static jint function(JNIEnv * env, void* ptr, jobject key, jobject object) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
        return container->remove(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr), qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr));
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
    static void function(JNIEnv * env, void* ptr, jobject key, jobject object) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
        container->replace(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr), qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr));
    }
};

 template<template<typename K, typename T> class Container, typename K, typename T, bool isPointer, bool = is_copy_constructible<K>::value && is_copy_constructible<T>::value && is_default_constructible<K>::value && is_default_constructible<T>::value && is_copy_constructible<Container<K,T>>::value>
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
    void assign(void* container, const void* other) override { (*reinterpret_cast<QMap<K,T>*>(container)) = (*reinterpret_cast<const QMap<K,T>*>(other)); }
    size_t sizeOf() override {
        return sizeof(QMap<K,T>);
    }
    void* constructContainer(void* placement, const void* copyOf = nullptr) override {
        if(copyOf){
            return new(placement) QMap<K,T>(*reinterpret_cast<const QMap<K,T>*>(copyOf));
        }else{
            return new(placement) QMap<K,T>();
        }
    }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* move) override {
        return new(placement) QMap<K,T>(std::move(*reinterpret_cast<const QMap<K,T>*>(move)));
    }
#endif
    bool destructContainer(void* container) override {
        reinterpret_cast<QMap<K,T>*>(container)->~QMap<K,T>();
        return true;
    }
     int registerContainer(const QByteArray& containerTypeName) override {
         return RegistryAPI::registerMetaType<QMap<K,T>>(containerTypeName);
     }

    void clear(JNIEnv * env,void*) override {
        JavaException::raiseUnsupportedOperationException(env, "QMap::clear()" QTJAMBI_STACKTRACEINFO );
    }

    jboolean contains(JNIEnv * env, const void* container, jobject key) override {
        return AssociativeContainerContains<QMap, K, T>::function(env, container, key);
    }

    jint count(JNIEnv * env, const void* container, jobject key) override {
         return AssociativeContainerCountObject<QMap, K, T>::function(env, container, key);
    }

    jobject begin(JNIEnv * env, QtJambiNativeID, void*) override {
        JavaException::raiseUnsupportedOperationException(env, "QMap::begin" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }

    jobject end(JNIEnv * env, QtJambiNativeID, void*) override {
        JavaException::raiseUnsupportedOperationException(env, "QMap::end" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }

    jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
         return AssociativeContainerConstEnd<QMap, K, T>::function(env, ownerId, container);
    }

    jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        return AssociativeContainerConstBegin<QMap, K, T>::function(env, ownerId, container);
    }

    jobject find(JNIEnv * env, QtJambiNativeID, void*, jobject) override {
         JavaException::raiseUnsupportedOperationException(env, "QMap::find" QTJAMBI_STACKTRACEINFO );
         return nullptr;
    }

    jobject constFind(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override {
         return AssociativeContainerConstFindPair<QMap, K, T>::function(env, ownerId, container, key);
    }

    jobject first(JNIEnv * env, const void* container) override {
         return AssociativeContainerFirst<QMap, K, T>::function(env, container);
    }

    jobject firstKey(JNIEnv * env, const void* container) override {
         return AssociativeContainerFirstKey<QMap, K, T>::function(env, container);
    }

    void insert(JNIEnv *env,void*,jobject,jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QMap::insert(key,value)" QTJAMBI_STACKTRACEINFO );
    }

    jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override {
         return AssociativeContainerKey<QMap, K, T>::function(env, container, value, defaultKey);
    }

    jobject keys(JNIEnv * env, const void* container) override {
         return AssociativeContainerKeys<QMap, K, T>::function(env, container);
    }

    jobject keys(JNIEnv * env, const void* container, jobject value) override {
         return AssociativeContainerKeysForValue<QMap, K, T>::function(env, container, value);
    }

    jobject last(JNIEnv * env, const void* container) override {
         return AssociativeContainerLast<QMap, K, T>::function(env, container);
    }

    jobject lastKey(JNIEnv * env, const void* container) override {
         return AssociativeContainerLastKey<QMap, K, T>::function(env, container);
    }

    jobject constLowerBound(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override {
         return AssociativeContainerLowerBound<QMap, K, T>::function(env, ownerId, container, key);
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
         return AssociativeContainerEquals<QMap, K, T, ContainerAPI::getAsQMap>::function(env, container, other);
    }

    jint remove(JNIEnv * env,void*,jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QMap::removeAll(key)" QTJAMBI_STACKTRACEINFO );
        return 0;
    }

    jint size(JNIEnv * env, const void* container) override {
         return AssociativeContainerSize<QMap, K, T>::function(env, container);
    }

    jobject take(JNIEnv *env,void*,jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QMap::take(key)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }

    jobject constUpperBound(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override {
         return AssociativeContainerUpperBound<QMap, K, T>::function(env, ownerId, container, key);
    }

    jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override {
         return AssociativeContainerValue<QMap, K, T>::function(env, container, key, defaultValue);
    }

    jobject values(JNIEnv * env, const void* container) override {
         return AssociativeContainerValues<QMap, K, T>::function(env, container);
    }

    bool keyLessThan(JNIEnv *env, jobject key1, jobject key2) override {
        return AssociativeContainerElementLessThan<K>::function(env, key1, key2);
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

    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) override {
        return AssociativeContainerBegin<QMap, K, T>::function(env, ownerId, container);
    }

    jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) override {
         return AssociativeContainerEnd<QMap, K, T>::function(env, ownerId, container);
    }

    jobject find(JNIEnv * env, QtJambiNativeID ownerId, void* container, jobject key) override {
         return AssociativeContainerFindPair<QMap, K, T>::function(env, ownerId, container, key);
    }

    void clear(JNIEnv * env, void* container) override {
        AssociativeContainerClear<QMap, K, T>::function(env, container);
    }

    void insert(JNIEnv *env, void* container,jobject key,jobject value) override {
        AssociativeContainerInsert<QMap, K, T>::function(env, container, key, value);
    }

    jint remove(JNIEnv * env, void* container,jobject key) override {
        return AssociativeContainerRemove<QMap, K, T>::function(env, container, key);
    }

    jobject take(JNIEnv *env, void* container,jobject key) override {
        return AssociativeContainerTake<QMap, K, T>::function(env, container, key);
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
    size_t sizeOf() override {
        return sizeof(QMultiMap<K,T>);
    }
    void* constructContainer(void* placement, const void* copyOf = nullptr) override {
        if(copyOf){
            return new(placement) QMultiMap<K,T>(*reinterpret_cast<const QMultiMap<K,T>*>(copyOf));
        }else{
            return new(placement) QMultiMap<K,T>();
        }
    }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* move) override {
        return new(placement) QMultiMap<K,T>(std::move(*reinterpret_cast<const QMultiMap<K,T>*>(move)));
    }
#endif
    bool destructContainer(void* container) override {
        reinterpret_cast<QMultiMap<K,T>*>(container)->~QMultiMap<K,T>();
        return true;
    }
     void assign(void* container, const void* other) override { (*reinterpret_cast<QMultiMap<K,T>*>(container)) = (*reinterpret_cast<const QMultiMap<K,T>*>(other)); }
     int registerContainer(const QByteArray& containerTypeName) override {
         return RegistryAPI::registerMetaType<QMultiMap<K,T>>(containerTypeName);
     }

    void clear(JNIEnv * env,void*) override {
        JavaException::raiseUnsupportedOperationException(env, "QMultiMap::clear()" QTJAMBI_STACKTRACEINFO );
    }

    jboolean contains(JNIEnv * env, const void* container, jobject key) override {
        return AssociativeContainerContains<QMultiMap, K, T>::function(env, container, key);
    }

    jint count(JNIEnv * env, const void* container, jobject key) override {
        return AssociativeContainerCountObject<QMultiMap, K, T>::function(env, container, key);
    }

    jobject begin(JNIEnv * env, QtJambiNativeID, void*) override {
        JavaException::raiseUnsupportedOperationException(env, "QMultiMap::begin" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }

    jobject end(JNIEnv * env, QtJambiNativeID, void*) override {
        JavaException::raiseUnsupportedOperationException(env, "QMultiMap::end" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }

    jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
         return AssociativeContainerConstEnd<QMultiMap, K, T>::function(env, ownerId, container);
    }

    jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        return AssociativeContainerConstBegin<QMultiMap, K, T>::function(env, ownerId, container);
    }

    jobject find(JNIEnv * env, QtJambiNativeID, void*, jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QMultiMap::find" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }

    jobject find(JNIEnv * env, QtJambiNativeID, void*, jobject, jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QMultiMap::find" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }

    jobject constFind(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override {
        return AssociativeContainerConstFindPair<QMultiMap, K, T>::function(env, ownerId, container, key);
    }

    jobject constFind(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key, jobject value) override {
        return AssociativeContainerConstFindPairs<QMultiMap, K, T>::function(env, ownerId, container, key, value);
    }
    jobject first(JNIEnv * env, const void* container) override {
        return AssociativeContainerFirst<QMultiMap, K, T>::function(env, container);
    }

    jobject firstKey(JNIEnv * env, const void* container) override {
        return AssociativeContainerFirstKey<QMultiMap, K, T>::function(env, container);
    }

    void insert(JNIEnv *env,void*,jobject,jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QMultiMap::insert(key,value)" QTJAMBI_STACKTRACEINFO );
    }

    jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override {
        return AssociativeContainerKey<QMultiMap, K, T>::function(env, container, value, defaultKey);
    }

    jobject keys(JNIEnv * env, const void* container) override {
        return AssociativeContainerKeys<QMultiMap, K, T>::function(env, container);
    }

    jobject keys(JNIEnv * env, const void* container, jobject value) override {
        return AssociativeContainerKeysForValue<QMultiMap, K, T>::function(env, container, value);
    }

    jobject last(JNIEnv * env, const void* container) override {
        return AssociativeContainerLast<QMultiMap, K, T>::function(env, container);
    }

    jobject lastKey(JNIEnv * env, const void* container) override {
        return AssociativeContainerLastKey<QMultiMap, K, T>::function(env, container);
    }

    jobject constLowerBound(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override {
        return AssociativeContainerLowerBound<QMultiMap, K, T>::function(env, ownerId, container, key);
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        return AssociativeContainerEquals<QMultiMap, K, T, ContainerAPI::getAsQMultiMap>::function(env, container, other);
    }

    jint remove(JNIEnv * env,void*,jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QMultiMap::removeAll(key)" QTJAMBI_STACKTRACEINFO );
        return 0;
    }

    jint size(JNIEnv * env, const void* container) override {
        return AssociativeContainerSize<QMultiMap, K, T>::function(env, container);
    }

    jobject take(JNIEnv *env,void*,jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QMultiMap::take(key)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }

    jobject uniqueKeys(JNIEnv * env, const void* container) override {
        return AssociativeContainerUniqueKeys<QMultiMap, K, T>::function(env, container);
    }

    void unite(JNIEnv *env,void*,jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QMultiMap::unite(other)" QTJAMBI_STACKTRACEINFO );
    }

    jobject values(JNIEnv * env, const void* container, jobject key) override {
        return AssociativeContainerValuesKey<QMultiMap, K, T>::function(env, container, key);
    }

    jobject constUpperBound(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override {
        return AssociativeContainerUpperBound<QMultiMap, K, T>::function(env, ownerId, container, key);
    }

    jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override {
        return AssociativeContainerValue<QMultiMap, K, T>::function(env, container, key, defaultValue);
    }

    jobject values(JNIEnv * env, const void* container) override {
        return AssociativeContainerValues<QMultiMap, K, T>::function(env, container);
    }

    jboolean contains(JNIEnv *env, const void* container, jobject key, jobject value) override {
         return AssociativeContainerContainsPair<QMultiMap, K, T>::function(env, container, key, value);
    }

    jint count(JNIEnv *env, const void* container, jobject key, jobject value) override {
         return AssociativeContainerCountPair<QMultiMap, K, T>::function(env, container, key, value);
    }

    jint remove(JNIEnv * env, void*,jobject, jobject) override {
         JavaException::raiseUnsupportedOperationException(env, "QMultiMap::remove(key, value)" QTJAMBI_STACKTRACEINFO );
         return 0;
    }

    void replace(JNIEnv *env, void*,jobject, jobject) override {
         JavaException::raiseUnsupportedOperationException(env, "QMultiMap::replace(key, value)" QTJAMBI_STACKTRACEINFO );
    }

    bool keyLessThan(JNIEnv *env, jobject key1, jobject key2) override {
        return AssociativeContainerElementLessThan<K>::function(env, key1, key2);
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

    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) override {
        return AssociativeContainerBegin<QMultiMap, K, T>::function(env, ownerId, container);
    }

    jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) override {
        return AssociativeContainerEnd<QMultiMap, K, T>::function(env, ownerId, container);
    }

    jobject find(JNIEnv * env, QtJambiNativeID ownerId, void* container, jobject key) override {
        return AssociativeContainerFindPair<QMultiMap, K, T>::function(env, ownerId, container, key);
    }

    jobject find(JNIEnv * env, QtJambiNativeID ownerId, void* container, jobject key, jobject value) override {
        return AssociativeContainerFindPairs<QMultiMap, K, T>::function(env, ownerId, container, key, value);
    }

    jint remove(JNIEnv * env, void* container,jobject key, jobject value) override {
         return AssociativeContainerRemovePair<QMultiMap, K, T>::function(env, container, key, value);
    }

    void replace(JNIEnv * env, void* container,jobject key, jobject value) override {
         AssociativeContainerReplacePair<QMultiMap, K, T>::function(env, container, key, value);
    }

    void clear(JNIEnv * env, void* container) override {
         AssociativeContainerClear<QMultiMap, K, T>::function(env, container);
    }

    void insert(JNIEnv *env, void* container,jobject key,jobject value) override {
         AssociativeContainerInsert<QMultiMap, K, T>::function(env, container, key, value);
    }

    jint remove(JNIEnv * env, void* container,jobject key) override {
         return AssociativeContainerRemove<QMultiMap, K, T>::function(env, container, key);
    }

    jobject take(JNIEnv *env, void* container,jobject key) override {
         return AssociativeContainerTake<QMultiMap, K, T>::function(env, container, key);
    }

    void unite(JNIEnv *env, void* container, jobject other) override {
         AssociativeContainerUnite<QMultiMap, K, T, ContainerAPI::getAsQMultiMap>::function(env, container, other);
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
     void assign(void* container, const void* other) override { (*reinterpret_cast<QHash<K,T>*>(container)) = (*reinterpret_cast<const QHash<K,T>*>(other)); }
     int registerContainer(const QByteArray& containerTypeName) override {
         return RegistryAPI::registerMetaType<QHash<K,T>>(containerTypeName);
     }

     size_t sizeOf() override {
         return sizeof(QHash<K,T>);
     }
     void* constructContainer(void* placement, const void* copyOf = nullptr) override {
         if(copyOf){
             return new(placement) QHash<K,T>(*reinterpret_cast<const QHash<K,T>*>(copyOf));
         }else{
             return new(placement) QHash<K,T>();
         }
     }
 #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
     void* constructContainer(void* placement, void* move) override {
         return new(placement) QHash<K,T>(std::move(*reinterpret_cast<const QHash<K,T>*>(move)));
     }
 #endif
     bool destructContainer(void* container) override {
         reinterpret_cast<QHash<K,T>*>(container)->~QHash<K,T>();
         return true;
     }

     jint capacity(JNIEnv * env, const void* container) override {
         return AssociativeContainerCapacity<QHash, K, T>::function(env, container);
     }

     void clear(JNIEnv * env,void*) override {
         JavaException::raiseUnsupportedOperationException(env, "QHash::clear()" QTJAMBI_STACKTRACEINFO );
     }

     jboolean contains(JNIEnv * env, const void* container, jobject key) override {
         return AssociativeContainerContains<QHash, K, T>::function(env, container, key);
     }

     jint count(JNIEnv * env, const void* container, jobject key) override {
          return AssociativeContainerCountObject<QHash, K, T>::function(env, container, key);
     }

     jobject begin(JNIEnv * env, QtJambiNativeID, void*) override {
         JavaException::raiseUnsupportedOperationException(env, "QHash::begin" QTJAMBI_STACKTRACEINFO );
         return nullptr;
     }

     jobject end(JNIEnv * env, QtJambiNativeID, void*) override {
         JavaException::raiseUnsupportedOperationException(env, "QHash::end" QTJAMBI_STACKTRACEINFO );
         return nullptr;
     }

     jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
          return AssociativeContainerConstEnd<QHash, K, T>::function(env, ownerId, container);
     }

     jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
         return AssociativeContainerConstBegin<QHash, K, T>::function(env, ownerId, container);
     }

     jobject find(JNIEnv * env, QtJambiNativeID, void*, jobject) override {
          JavaException::raiseUnsupportedOperationException(env, "QHash::find" QTJAMBI_STACKTRACEINFO );
          return nullptr;
     }

     jobject constFind(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override {
          return AssociativeContainerConstFindPair<QHash, K, T>::function(env, ownerId, container, key);
     }

     void insert(JNIEnv *env,void*,jobject,jobject) override {
         JavaException::raiseUnsupportedOperationException(env, "QHash::insert(key,value)" QTJAMBI_STACKTRACEINFO );
     }

     jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override {
          return AssociativeContainerKey<QHash, K, T>::function(env, container, value, defaultKey);
     }

     jobject keys(JNIEnv * env, const void* container) override {
          return AssociativeContainerKeys<QHash, K, T>::function(env, container);
     }

     jobject keys(JNIEnv * env, const void* container, jobject value) override {
          return AssociativeContainerKeysForValue<QHash, K, T>::function(env, container, value);
     }

     jboolean equal(JNIEnv * env, const void* container, jobject other) override {
          return AssociativeContainerEquals<QHash, K, T, ContainerAPI::getAsQHash>::function(env, container, other);
     }

     jint remove(JNIEnv * env,void*,jobject) override {
         JavaException::raiseUnsupportedOperationException(env, "QHash::remove(key)" QTJAMBI_STACKTRACEINFO );
         return 0;
     }

     void reserve(JNIEnv * env,void*,jint) override {
         JavaException::raiseUnsupportedOperationException(env, "QHash::reserve(size)" QTJAMBI_STACKTRACEINFO );
     }

     jint size(JNIEnv * env, const void* container) override {
          return AssociativeContainerSize<QHash, K, T>::function(env, container);
     }

     jobject take(JNIEnv *env,void*,jobject) override {
         JavaException::raiseUnsupportedOperationException(env, "QHash::take(key)" QTJAMBI_STACKTRACEINFO );
         return nullptr;
     }

     jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override {
          return AssociativeContainerValue<QHash, K, T>::function(env, container, key, defaultValue);
     }

     jobject values(JNIEnv * env, const void* container) override {
          return AssociativeContainerValues<QHash, K, T>::function(env, container);
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

     jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) override {
         return AssociativeContainerBegin<QHash, K, T>::function(env, ownerId, container);
     }

     jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) override {
          return AssociativeContainerEnd<QHash, K, T>::function(env, ownerId, container);
     }

     jobject find(JNIEnv * env, QtJambiNativeID ownerId, void* container, jobject key) override {
          return AssociativeContainerFindPair<QHash, K, T>::function(env, ownerId, container, key);
     }

     void clear(JNIEnv * env, void* container) override {
         AssociativeContainerClear<QHash, K, T>::function(env, container);
     }
     void insert(JNIEnv *env, void* container,jobject key,jobject value) override {
         AssociativeContainerInsert<QHash, K, T>::function(env, container, key, value);
     }
     jint remove(JNIEnv * env, void* container,jobject key) override {
         return AssociativeContainerRemove<QHash, K, T>::function(env, container, key);
     }
     jobject take(JNIEnv *env, void* container,jobject key) override {
         return AssociativeContainerTake<QHash, K, T>::function(env, container, key);
     }
     void reserve(JNIEnv * env,void* container, jint newSize) override {
         AssociativeContainerReserve<QHash, K, T>::function(env, container, newSize);
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
     void assign(void* container, const void* other) override { (*reinterpret_cast<QMultiHash<K,T>*>(container)) = (*reinterpret_cast<const QMultiHash<K,T>*>(other)); }
     int registerContainer(const QByteArray& containerTypeName) override {
         return RegistryAPI::registerMetaType<QMultiHash<K,T>>(containerTypeName);
     }

     size_t sizeOf() override {
         return sizeof(QMultiHash<K,T>);
     }
     void* constructContainer(void* placement, const void* copyOf = nullptr) override {
         if(copyOf){
             return new(placement) QMultiHash<K,T>(*reinterpret_cast<const QMultiHash<K,T>*>(copyOf));
         }else{
             return new(placement) QMultiHash<K,T>();
         }
     }
 #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
     void* constructContainer(void* placement, void* move) override {
         return new(placement) QMultiHash<K,T>(std::move(*reinterpret_cast<const QMultiHash<K,T>*>(move)));
     }
 #endif
     bool destructContainer(void* container) override {
         reinterpret_cast<QMultiHash<K,T>*>(container)->~QMultiHash<K,T>();
         return true;
     }

     jint capacity(JNIEnv * env, const void* container) override {
         return AssociativeContainerCapacity<QMultiHash, K, T>::function(env, container);
     }

     void clear(JNIEnv * env,void*) override {
         JavaException::raiseUnsupportedOperationException(env, "QMultiHash::clear()" QTJAMBI_STACKTRACEINFO );
     }

     jboolean contains(JNIEnv * env, const void* container, jobject key) override {
         return AssociativeContainerContains<QMultiHash, K, T>::function(env, container, key);
     }

     jint count(JNIEnv * env, const void* container, jobject key) override {
         return AssociativeContainerCountObject<QMultiHash, K, T>::function(env, container, key);
     }

     jobject begin(JNIEnv * env, QtJambiNativeID, void*) override {
         JavaException::raiseUnsupportedOperationException(env, "QMultiHash::begin" QTJAMBI_STACKTRACEINFO );
         return nullptr;
     }

     jobject end(JNIEnv * env, QtJambiNativeID, void*) override {
         JavaException::raiseUnsupportedOperationException(env, "QMultiHash::end" QTJAMBI_STACKTRACEINFO );
         return nullptr;
     }

     jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
          return AssociativeContainerConstEnd<QMultiHash, K, T>::function(env, ownerId, container);
     }

     jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
         return AssociativeContainerConstBegin<QMultiHash, K, T>::function(env, ownerId, container);
     }

     jobject find(JNIEnv * env, QtJambiNativeID, void*, jobject) override {
         JavaException::raiseUnsupportedOperationException(env, "QMultiHash::find" QTJAMBI_STACKTRACEINFO );
         return nullptr;
     }

     jobject find(JNIEnv * env, QtJambiNativeID, void*, jobject, jobject) override {
         JavaException::raiseUnsupportedOperationException(env, "QMultiHash::find" QTJAMBI_STACKTRACEINFO );
         return nullptr;
     }

     jobject constFind(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override {
         return AssociativeContainerConstFindPair<QMultiHash, K, T>::function(env, ownerId, container, key);
     }

     jobject constFind(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key, jobject value) override {
         return AssociativeContainerConstFindPairs<QMultiHash, K, T>::function(env, ownerId, container, key, value);
     }

     void insert(JNIEnv *env,void*,jobject,jobject) override {
         JavaException::raiseUnsupportedOperationException(env, "QMultiHash::insert(key,value)" QTJAMBI_STACKTRACEINFO );
     }

     jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override {
         return AssociativeContainerKey<QMultiHash, K, T>::function(env, container, value, defaultKey);
     }

     jobject keys(JNIEnv * env, const void* container) override {
         return AssociativeContainerKeys<QMultiHash, K, T>::function(env, container);
     }

     jobject keys(JNIEnv * env, const void* container, jobject value) override {
         return AssociativeContainerKeysForValue<QMultiHash, K, T>::function(env, container, value);
     }

     jboolean equal(JNIEnv * env, const void* container, jobject other) override {
         return AssociativeContainerEquals<QMultiHash, K, T, ContainerAPI::getAsQMultiHash>::function(env, container, other);
     }

     jint remove(JNIEnv * env,void*,jobject) override {
         JavaException::raiseUnsupportedOperationException(env, "QMultiHash::remove(key)" QTJAMBI_STACKTRACEINFO );
        return 0;
     }

     void reserve(JNIEnv * env,void*,jint) override {
         JavaException::raiseUnsupportedOperationException(env, "QMultiHash::reserve(size)" QTJAMBI_STACKTRACEINFO );
     }

     jint size(JNIEnv * env, const void* container) override {
         return AssociativeContainerSize<QMultiHash, K, T>::function(env, container);
     }

     jobject take(JNIEnv *env,void*,jobject) override {
         JavaException::raiseUnsupportedOperationException(env, "QMultiHash::take(key)" QTJAMBI_STACKTRACEINFO );
         return nullptr;
     }

     jobject uniqueKeys(JNIEnv * env, const void* container) override {
         return AssociativeContainerUniqueKeys<QMultiHash, K, T>::function(env, container);
     }

     void unite(JNIEnv *env,void*,jobject) override {
         JavaException::raiseUnsupportedOperationException(env, "QMultiHash::unite(other)" QTJAMBI_STACKTRACEINFO );
     }

     jobject values(JNIEnv * env, const void* container, jobject key) override {
         return AssociativeContainerValuesKey<QMultiHash, K, T>::function(env, container, key);
     }

     jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override {
         return AssociativeContainerValue<QMultiHash, K, T>::function(env, container, key, defaultValue);
     }

     jobject values(JNIEnv * env, const void* container) override {
         return AssociativeContainerValues<QMultiHash, K, T>::function(env, container);
     }

     jboolean contains(JNIEnv *env, const void* container, jobject key, jobject value) override {
          return AssociativeContainerContainsPair<QMultiHash, K, T>::function(env, container, key, value);
     }

     jint count(JNIEnv *env, const void* container, jobject key, jobject value) override {
          return AssociativeContainerCountPair<QMultiHash, K, T>::function(env, container, key, value);
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

     jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) override {
         return AssociativeContainerBegin<QMultiHash, K, T>::function(env, ownerId, container);
     }

     jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) override {
         return AssociativeContainerEnd<QMultiHash, K, T>::function(env, ownerId, container);
     }

     jobject find(JNIEnv * env, QtJambiNativeID ownerId, void* container, jobject key) override {
         return AssociativeContainerFindPair<QMultiHash, K, T>::function(env, ownerId, container, key);
     }

     jobject find(JNIEnv * env, QtJambiNativeID ownerId, void* container, jobject key, jobject value) override {
         return AssociativeContainerFindPairs<QMultiHash, K, T>::function(env, ownerId, container, key, value);
     }

     void clear(JNIEnv * env, void* container) override {
         AssociativeContainerClear<QMultiHash, K, T>::function(env, container);
     }
     void insert(JNIEnv *env, void* container,jobject key,jobject value) override {
         AssociativeContainerInsert<QMultiHash, K, T>::function(env, container, key, value);
     }
     jint remove(JNIEnv * env, void* container,jobject key) override {
         return AssociativeContainerRemove<QMultiHash, K, T>::function(env, container, key);
     }
     jobject take(JNIEnv *env, void* container,jobject key) override {
         return AssociativeContainerTake<QMultiHash, K, T>::function(env, container, key);
     }

     jint remove(JNIEnv * env, void* container,jobject key, jobject value) override {
          return AssociativeContainerRemovePair<QMultiHash, K, T>::function(env, container, key, value);
     }

     void replace(JNIEnv * env, void* container,jobject key, jobject value) override {
          AssociativeContainerReplacePair<QMultiHash, K, T>::function(env, container, key, value);
     }

     void unite(JNIEnv *env, void* container, jobject other) override {
          AssociativeContainerUnite<QMultiHash, K, T, ContainerAPI::getAsQMultiHash>::function(env, container, other);
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
struct AssociativeContainerAccess{
    static constexpr AbstractContainerAccess* newInstance(){return nullptr;}
};

template<typename K, typename T, bool isConst>
struct AssociativeContainerAccess<QMap,K,T,isConst> : QMapAccess<K,T,isConst>{
};

template<typename K, typename T, bool isConst>
struct AssociativeContainerAccess<QMultiMap,K,T,isConst> : QMultiMapAccess<K,T,isConst>{
};

template<typename K, typename T, bool isConst>
struct AssociativeContainerAccess<QHash,K,T,isConst> : QHashAccess<K,T,isConst>{
};

template<typename K, typename T, bool isConst>
struct AssociativeContainerAccess<QMultiHash,K,T,isConst> : QMultiHashAccess<K,T,isConst>{
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_IMPL_CONTAINER_ASSOCIATIVE_H
