/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
#include "qtjambi_cast_impl_container_sequential.h"

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
typedef bool (*IsAssociativeContainerAccessFunction)(JNIEnv *, jobject, const QMetaType&, const QMetaType&, void*& pointer, AbstractContainerAccess*& access);

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

    const QMetaType& keyMetaType() override{
        typedef typename std::remove_reference<decltype(std::declval<Iterator>().key())>::type K;
        static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(K));
        return type;
    }

    const QMetaType& valueMetaType() override{
        typedef typename std::remove_reference<decltype(std::declval<Iterator>().value())>::type V;
        static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(V));
        return type;
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

    const QMetaType& keyMetaType() override{
        return QAssociativeConstIteratorAccess<Iterator>::keyMetaType();
    }

    const QMetaType& valueMetaType() override{
        return QAssociativeConstIteratorAccess<Iterator>::valueMetaType();
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
    static jobject function(JNIEnv *env, const ExtendedContainerInfo&) {
        JavaException::raiseUnsupportedOperationException(env, "begin()" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerBegin<Container, K, T, true>{
    static jobject function(JNIEnv *env, const ExtendedContainerInfo& ptr) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr.container);
        return qtjambi_associative_iterator_caster<decltype(container->begin())>::cast(env, ptr.nativeId, container->begin());
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
        return qtjambi_associative_iterator_caster<decltype(container->constBegin())>::cast(env, ptr.nativeId, container->constBegin());
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
    static void function(JNIEnv *env, const ContainerInfo&, jint) {
        JavaException::raiseUnsupportedOperationException(env, "reserve(size)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerReserve<Container, K, T, true>{
    static void function(JNIEnv *, const ContainerInfo& ptr, jint size) {
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
    static jobject function(JNIEnv *env, const ExtendedContainerInfo&) {
        JavaException::raiseUnsupportedOperationException(env, "end()" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct AssociativeContainerEnd<Container, K, T, true>{
    static jobject function(JNIEnv *env, const ExtendedContainerInfo& ptr) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr.container);
        return qtjambi_associative_iterator_caster<decltype(container->end())>::cast(env, ptr.nativeId, container->end());
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
        return qtjambi_associative_iterator_caster<decltype(container->constEnd())>::cast(env, ptr.nativeId, container->constEnd());
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
        return qtjambi_associative_iterator_caster<typename Container<K,T>::iterator>::cast(env, ptr.nativeId, container->find(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr)));
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
        return qtjambi_associative_iterator_caster<typename Container<K,T>::const_iterator>::cast(env, ptr.nativeId, container->constFind(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr)));
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
        result.container = new QList<K>(container->keys(qtjambi_scoped_cast<false,T,jobject>::cast(env, value, nullptr, nullptr)));
        result.access = QListAccess<K>::newInstance();
        result.object = ContainerAPI::objectFromQList(env, result.container, result.access);
        return result;
    }
};

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0) && QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
template<typename K, typename T>
struct AssociativeContainerKeysForValue<QMultiHash, K, T, true>{
    static ContainerAndAccessInfo function(JNIEnv *env, const ConstContainerInfo& ptr, jobject value) {
        ContainerAndAccessInfo result;
        const QMultiHash<K,T> *container = static_cast<const QMultiHash<K,T> *>(ptr.container);
        QList<K> _keys;
        typename QMultiHash<K,T>::const_iterator i = reinterpret_cast<const QMultiHash<K,T> *>(container)->begin();
        typename QMultiHash<K,T>::const_iterator end = reinterpret_cast<const QMultiHash<K,T> *>(container)->end();
        T _qvalue = qtjambi_scoped_cast<false,T,jobject>::cast(env, value, nullptr, nullptr);
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
        return qtjambi_associative_iterator_caster<typename Container<K,T>::const_iterator>::cast(env, ptr.nativeId, container->lowerBound(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr)));
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
        return qtjambi_associative_iterator_caster<typename Container<K,T>::const_iterator>::cast(env, ptr.nativeId, container->upperBound(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr)));
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
        result.container = new QList<T>(container->values(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr)));
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
    static void function(JNIEnv * __jni_env, const ContainerInfo& ptr, jobject key, jobject value) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr.container);
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
    static jint function(JNIEnv * __jni_env, const ContainerInfo& ptr, jobject key) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr.container);
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
    static jobject function(JNIEnv * env, const ContainerInfo& ptr, jobject key) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr.container);
        return qtjambi_scoped_cast<false,jobject,const T>::cast(env, container->take(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr)), nullptr, nullptr);
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
            if (isContainer(env, other.object, QTJAMBI_METATYPE_FROM_TYPE2(K), QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(__qt_other_pointer), other.access)) {
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
    static jobject function(JNIEnv *env, const ExtendedContainerInfo& ptr, jobject key, jobject object) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr.container);
        return qtjambi_associative_iterator_caster<typename Container<K,T>::iterator>::cast(env, ptr.nativeId, container->find(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr), qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr)));
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
        return qtjambi_associative_iterator_caster<typename Container<K,T>::const_iterator>::cast(env, ptr.nativeId, container->constFind(qtjambi_scoped_cast<false,K,jobject>::cast(env, key, nullptr, nullptr), qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr)));
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
    static jint function(JNIEnv * env, const ContainerInfo& ptr, jobject key, jobject object) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr.container);
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
    static void function(JNIEnv * env, const ContainerInfo& ptr, jobject key, jobject object) {
        Container<K,T> *container = static_cast<Container<K,T> *>(ptr.container);
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

template<typename K, typename T, typename Super>
class OwnerFunctionAssociativeAccess : public Super{
protected:
    OwnerFunctionAssociativeAccess(){}
public:
    const QObject* getOwner(const void* container) override{
        Super* _this = this;
        const QObject* owner = nullptr;
        if constexpr(ContainerContentType<K>::isContainer){
            if(AbstractContainerAccess* elementNestedContainerAccess = ContainerContentType<K>::accessFactory()){
                auto iter = _this->keyValueIterator(container);
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
                auto iter = _this->keyValueIterator(container);
                while(iter->hasNext()){
                    auto current = iter->next();
                    if((owner = ownerFunctionK(current.first)))
                        break;
                }
            }
        }
        if(!owner){
            if constexpr(ContainerContentType<T>::isContainer){
                if(AbstractContainerAccess* elementNestedContainerAccess = ContainerContentType<T>::accessFactory()){
                    auto iter = _this->keyValueIterator(container);
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
                    auto iter = _this->keyValueIterator(container);
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
        if constexpr(ContainerContentType<K>::isContainer){
            if(AbstractContainerAccess* elementNestedContainerAccess = ContainerContentType<K>::accessFactory()){
                result = elementNestedContainerAccess->hasOwnerFunction();
                elementNestedContainerAccess->dispose();
            }
        }else{
            result = registeredOwnerFunction<K>()!=nullptr;
        }
        if(!result){
            if constexpr(ContainerContentType<T>::isContainer){
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
    void updateRC(JNIEnv * env, const ContainerInfo& container) override {
        Super* _this = this;
        JniLocalFrame frame(env, 200);
        jobject set{nullptr};
        if constexpr((ContainerContentType<K>::isContainer && ContainerContentType<K>::needsReferenceCounting)
                      || (ContainerContentType<K>::isContainer && ContainerContentType<K>::needsReferenceCounting)){
            set = QtJambiAPI::newJavaHashSet(env);
            auto access1 = _this->keyNestedContainerAccess();
            auto access2 = _this->valueNestedContainerAccess();
            auto iterator = _this->keyValueIterator(container.container);
            while(iterator->hasNext()){
                auto current = iterator->next();
                unfoldAndAddContainer(env, set, current.first, ContainerContentType<K>::type, _this->keyMetaType(), access1);
                unfoldAndAddContainer(env, set, current.second, ContainerContentType<T>::type, _this->valueMetaType(), access2);
            }
            if(access1)
                access1->dispose();
            if(access2)
                access2->dispose();
        }else if constexpr(ContainerContentType<K>::type==AbstractContainerAccess::PointerToQObject){
            Q_STATIC_ASSERT_X(ContainerContentType<T>::type==AbstractContainerAccess::Value, "Associative reference counting required");
            set = QtJambiAPI::newJavaArrayList(env);
            auto iterator = _this->keyValueIterator(container.container);
            while(iterator->hasNext()){
                auto content = iterator->next();
                if(jobject obj = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content.first)))
                    Java::Runtime::Collection::add(env, set, obj);
            }
        }else if constexpr(ContainerContentType<K>::type==AbstractContainerAccess::FunctionPointer){
            Q_STATIC_ASSERT_X(ContainerContentType<T>::type==AbstractContainerAccess::Value, "Associative reference counting required");
            set = QtJambiAPI::newJavaArrayList(env);
            auto iterator = _this->keyValueIterator(container.container);
            while(iterator->hasNext()){
                auto content = iterator->next();
                if(jobject obj = QtJambiAPI::findFunctionPointerObject(env, content.first, typeid(K)))
                    Java::Runtime::Collection::add(env, set, obj);
            }
        }else if constexpr(ContainerContentType<K>::type==AbstractContainerAccess::Pointer){
            Q_STATIC_ASSERT_X(ContainerContentType<T>::type==AbstractContainerAccess::Value, "Associative reference counting required");
            set = QtJambiAPI::newJavaArrayList(env);
            auto iterator = _this->keyValueIterator(container.container);
            while(iterator->hasNext()){
                auto content = iterator->next();
                if(jobject obj = QtJambiAPI::findObject(env, content.first))
                    Java::Runtime::Collection::add(env, set, obj);
            }
        }else if constexpr(ContainerContentType<T>::type==AbstractContainerAccess::PointerToQObject){
            Q_STATIC_ASSERT_X(ContainerContentType<K>::type==AbstractContainerAccess::Value, "Associative reference counting required");
            set = QtJambiAPI::newJavaArrayList(env);
            auto iterator = _this->keyValueIterator(container.container);
            while(iterator->hasNext()){
                auto content = iterator->next();
                if(jobject obj = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content.second)))
                    Java::Runtime::Collection::add(env, set, obj);
            }
        }else if constexpr(ContainerContentType<T>::type==AbstractContainerAccess::FunctionPointer){
            Q_STATIC_ASSERT_X(ContainerContentType<K>::type==AbstractContainerAccess::Value, "Associative reference counting required");
            set = QtJambiAPI::newJavaArrayList(env);
            auto iterator = _this->keyValueIterator(container.container);
            while(iterator->hasNext()){
                auto content = iterator->next();
                if(jobject obj = QtJambiAPI::findFunctionPointerObject(env, content.second, typeid(T)))
                    Java::Runtime::Collection::add(env, set, obj);
            }
        }else if constexpr(ContainerContentType<T>::type==AbstractContainerAccess::Pointer){
            Q_STATIC_ASSERT_X(ContainerContentType<K>::type==AbstractContainerAccess::Value, "Associative reference counting required");
            set = QtJambiAPI::newJavaArrayList(env);
            auto iterator = _this->keyValueIterator(container.container);
            while(iterator->hasNext()){
                auto content = iterator->next();
                if(jobject obj = QtJambiAPI::findObject(env, content.second))
                    Java::Runtime::Collection::add(env, set, obj);
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
    Q_STATIC_ASSERT_X((ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0
                          && (ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0, "Sequential reference counting required");
public:
    void updateRC(JNIEnv * env, const ContainerInfo& container) override {
        Super* _this = this;
        JniLocalFrame frame(env, 200);
        jobject map = QtJambiAPI::newJavaHashMap(env);
        jobject key{nullptr};
        jobject value{nullptr};
        auto iterator = _this->keyValueIterator(container.container);
        while(iterator->hasNext()){
            auto content = iterator->next();
            if constexpr(ContainerContentType<K>::type==AbstractContainerAccess::PointerToQObject){
                key = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content.first));
            }else if constexpr(ContainerContentType<K>::type==AbstractContainerAccess::FunctionPointer){
                key = QtJambiAPI::findFunctionPointerObject(env, content.first, typeid(K));
            }else{
                key = QtJambiAPI::findObject(env, content.first);
            }
            if constexpr(ContainerContentType<T>::type==AbstractContainerAccess::PointerToQObject){
                value = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content.second));
            }else if constexpr(ContainerContentType<T>::type==AbstractContainerAccess::FunctionPointer){
                value = QtJambiAPI::findFunctionPointerObject(env, content.second, typeid(T));
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
    Q_STATIC_ASSERT_X((ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0
                          && (ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0, "Sequential reference counting required");
    void updateRC(JNIEnv * env, const ContainerInfo& container) override {
        Super* _this = this;
        JniLocalFrame frame(env, 200);
        jobject map = ReferenceCountingMultiMapContainer::newRCMultiMap(env);
        jobject key{nullptr};
        jobject value{nullptr};
        auto iterator = _this->keyValueIterator(container.container);
        while(iterator->hasNext()){
            auto content = iterator->next();
            if constexpr(ContainerContentType<K>::type==AbstractContainerAccess::PointerToQObject){
                key = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content.first));
            }else if constexpr(ContainerContentType<K>::type==AbstractContainerAccess::FunctionPointer){
                key = QtJambiAPI::findFunctionPointerObject(env, content.first, typeid(K));
            }else{
                key = QtJambiAPI::findObject(env, content.first);
            }
            if constexpr(ContainerContentType<T>::type==AbstractContainerAccess::PointerToQObject){
                value = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content.second));
            }else if constexpr(ContainerContentType<T>::type==AbstractContainerAccess::FunctionPointer){
                value = QtJambiAPI::findFunctionPointerObject(env, content.second, typeid(T));
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
         bool K_is_pointer = (ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0,
         bool T_is_pointer = (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0,
         bool K_is_container = ContainerContentType<K>::isContainer /*true*/,
         bool T_is_container = ContainerContentType<T>::isContainer /*true*/>
struct AssociativeAccessSuperclassDecider_IsContainer{
    typedef Super type;
};

template<typename K, typename T, typename Super>
struct AssociativeAccessSuperclassDecider_IsContainer<K,T,Super,false,true,true,false>{
    // Container<Container,Ptr*>
    typedef typename std::conditional<ContainerContentType<K>::needsOwnerCheck, OwnerFunctionAssociativeAccess<K,T,Super>, Super>::type ownerSuper;
    typedef ReferenceCountingAssociativeSetAccess<K,T,ownerSuper> type;
};

template<typename K, typename T, typename Super>
struct AssociativeAccessSuperclassDecider_IsContainer<K,T,Super,true,false,false,true>{
    // Container<Ptr*,Container>
    typedef typename std::conditional<ContainerContentType<T>::needsOwnerCheck, OwnerFunctionAssociativeAccess<K,T,Super>, Super>::type ownerSuper;
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
    typedef typename std::conditional<ContainerContentType<K>::needsReferenceCounting, ReferenceCountingAssociativeSetAccess<K,T,ownerSuper>,ownerSuper>::type type;
};

template<typename K, typename T, typename Super>
struct AssociativeAccessSuperclassDecider_IsContainer<K,T,Super,false,false,false,true>{
    // Container<Value,Container>
    typedef OwnerFunctionAssociativeAccess<K,T,Super> ownerSuper;
    typedef typename std::conditional<ContainerContentType<T>::needsReferenceCounting, ReferenceCountingAssociativeSetAccess<K,T,ownerSuper>,ownerSuper>::type type;
};

template<typename K, typename T, typename Super>
struct AssociativeAccessSuperclassDecider_IsContainer<K,T,Super,false,false,false,false>{
    // Container<Value,Value>
    typedef OwnerFunctionAssociativeAccess<K,T,Super> type;
};

template<typename K, typename T, typename Super,
         bool isMulti = std::is_same<Super,AbstractMultiMapAccess>::value || std::is_same<Super,AbstractMultiHashAccess>::value,
         bool K_is_pointer = (ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0,
         bool T_is_pointer = (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0>
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

template<typename K, typename T>
class QMapAccess : public AssociativeAccessSuperclassDecider<K,T,AbstractMapAccess>::type{
    typedef typename AssociativeAccessSuperclassDecider<K,T,AbstractMapAccess>::type Super;
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
        if constexpr ((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0 && (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if(ReferenceCountingMapContainer* access = dynamic_cast<ReferenceCountingMapContainer*>(container2.access)){
                if(container2.access!=this)
                    Super::swapRC(env, container, container2);
            }else{
                Super::updateRC(env, container);
            }
        }else if constexpr(ContainerContentType<QMap<K,T>>::needsReferenceCounting){
            if(ReferenceCountingSetContainer* access = dynamic_cast<ReferenceCountingSetContainer*>(container2.access)){
                if(container2.access!=this)
                    Super::swapRC(env, container, container2);
            }else{
                Super::updateRC(env, container);
            }
        }else{
            Q_UNUSED(env);
        }
    }

    const QMetaType& keyMetaType() override {
        static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(K));
        return type;
    }
    const QMetaType& valueMetaType() override {
        static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(T));
        return type;
    }

    AbstractContainerAccess::DataType keyType() override{
        return ContainerContentType<K>::type;
    }

    AbstractContainerAccess::DataType valueType() override{
        return ContainerContentType<T>::type;
    }

    AbstractContainerAccess* keyNestedContainerAccess() override {
        return ContainerContentType<K>::accessFactory();
    }

    AbstractContainerAccess* valueNestedContainerAccess() override {
        return ContainerContentType<T>::accessFactory();
    }

    bool hasKeyNestedContainerAccess() override {
        return ContainerContentType<K>::isContainer;
    }
    bool hasValueNestedContainerAccess() override {
        return ContainerContentType<T>::isContainer;
    }
    bool hasKeyNestedPointers() override {
        return ContainerContentType<K>::isContainer && ContainerContentType<K>::needsReferenceCounting;
    }
    bool hasValueNestedPointers() override {
        return ContainerContentType<T>::isContainer && ContainerContentType<T>::needsReferenceCounting;
    }

    void assign(void* container, const void* other) override {
        (*reinterpret_cast<QMap<K,T>*>(container)) = (*reinterpret_cast<const QMap<K,T>*>(other));
    }
    void assign(JNIEnv *env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override {
        (*reinterpret_cast<QMap<K,T>*>(container.container)) = (*reinterpret_cast<const QMap<K,T>*>(other.container));
        if constexpr ((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0 && (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if(ReferenceCountingMapContainer* access = dynamic_cast<ReferenceCountingMapContainer*>(other.access)){
                if(other.access!=this)
                    Super::assignRC(env, container.object, other.object);
            }else{
                Super::updateRC(env, container);
            }
        }else if constexpr(ContainerContentType<QMap<K,T>>::needsReferenceCounting){
            if(ReferenceCountingSetContainer* access = dynamic_cast<ReferenceCountingSetContainer*>(other.access)){
                if(other.access!=this)
                    Super::assignRC(env, container.object, other.object);
            }else{
                Super::updateRC(env, container);
            }
        }else{
            Q_UNUSED(env);
        }
    }
    size_t sizeOf() override {
        return sizeof(QMap<K,T>);
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* move) override {
        return new(placement) QMap<K,T>(std::move(*reinterpret_cast<const QMap<K,T>*>(move)));
    }
    void* constructContainer(JNIEnv *, void* placement, const ContainerAndAccessInfo& move) override {
        return constructContainer(placement, move.container);
    }
#endif
    bool destructContainer(void* container) override {
        reinterpret_cast<QMap<K,T>*>(container)->~QMap<K,T>();
        return true;
    }

    int registerContainer(const QByteArray& containerTypeName) override {
        return RegistryAPI::registerMetaType<QMap<K,T>>(containerTypeName, this);
    }

    jboolean contains(JNIEnv * env, const void* container, jobject key) override {
        return AssociativeContainerContains<QMap, K, T>::function(env, container, key);
    }

    jint count(JNIEnv * env, const void* container, jobject key) override {
        return AssociativeContainerCountObject<QMap, K, T>::function(env, container, key);
    }

    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        return AssociativeContainerConstEnd<QMap, K, T>::function(env, container);
    }

    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        return AssociativeContainerConstBegin<QMap, K, T>::function(env, container);
    }

    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override {
        return AssociativeContainerConstFindPair<QMap, K, T>::function(env, container, key);
    }

    jobject first(JNIEnv * env, const void* container) override {
        return AssociativeContainerFirst<QMap, K, T>::function(env, container);
    }

    jobject firstKey(JNIEnv * env, const void* container) override {
        return AssociativeContainerFirstKey<QMap, K, T>::function(env, container);
    }

    jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override {
        return AssociativeContainerKey<QMap, K, T>::function(env, container, value, defaultKey);
    }

    ContainerAndAccessInfo keys(JNIEnv * env, const ConstContainerInfo& container) override {
        return AssociativeContainerKeys<QMap, K, T>::function(env, container);
    }

    ContainerAndAccessInfo keys(JNIEnv * env, const ConstContainerInfo& container, jobject value) override {
        return AssociativeContainerKeysForValue<QMap, K, T>::function(env, container, value);
    }

    jobject last(JNIEnv * env, const void* container) override {
        return AssociativeContainerLast<QMap, K, T>::function(env, container);
    }

    jobject lastKey(JNIEnv * env, const void* container) override {
        return AssociativeContainerLastKey<QMap, K, T>::function(env, container);
    }

    jobject constLowerBound(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override {
        return AssociativeContainerLowerBound<QMap, K, T>::function(env, container, key);
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        return AssociativeContainerEquals<QMap, K, T, ContainerAPI::getAsQMap>::function(env, container, other);
    }

    jint size(JNIEnv * env, const void* container) override {
        return AssociativeContainerSize<QMap, K, T>::function(env, container);
    }

    jobject constUpperBound(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override {
        return AssociativeContainerUpperBound<QMap, K, T>::function(env, container, key);
    }

    jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override {
        return AssociativeContainerValue<QMap, K, T>::function(env, container, key, defaultValue);
    }

    ContainerAndAccessInfo values(JNIEnv * env, const ConstContainerInfo& container) override {
        return AssociativeContainerValues<QMap, K, T>::function(env, container);
    }

    bool keyLessThan(JNIEnv *env, jobject key1, jobject key2) override {
        return AssociativeContainerElementLessThan<K>::function(env, key1, key2);
    }

    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override {
        return AssociativeContainerBegin<QMap, K, T>::function(env, container);
    }

    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override {
        return AssociativeContainerEnd<QMap, K, T>::function(env, container);
    }

    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key) override {
        return AssociativeContainerFindPair<QMap, K, T>::function(env, container, key);
    }

    void clear(JNIEnv * env, const ContainerInfo& container) override {
        AssociativeContainerClear<QMap, K, T>::function(env, container);
        if constexpr(ContainerContentType<QMap<K,T>>::needsReferenceCounting){
            Super::clearRC(env, container.object);
        }
    }

    void insert(JNIEnv *env, const ContainerInfo& container,jobject key,jobject _value) override {
        if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            AssociativeContainerInsert<QMap, K, T>::function(env, container, key, _value);
            Super::putRC(env, container.object, key, _value);
        }else if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                AssociativeContainerInsert<QMap, K, T>::function(env, container, key, _value);
                Super::addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
                Super::addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), _value);
            }else{
                AssociativeContainerInsert<QMap, K, T>::function(env, container, key, _value);
                Super::addUniqueRC(env, container.object, key);
            }
        }else if constexpr((ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                AssociativeContainerInsert<QMap, K, T>::function(env, container, key, _value);
                Super::addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
                Super::addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), _value);
            }else{
                jobject oldValue = value(env, container.container, key, nullptr);
                AssociativeContainerInsert<QMap, K, T>::function(env, container, key, _value);
                Super::removeRC(env, container.object, oldValue);
                Super::addRC(env, container.object, _value);
            }
        }else if constexpr(ContainerContentType<K>::needsReferenceCounting || ContainerContentType<T>::needsReferenceCounting){
            AssociativeContainerInsert<QMap, K, T>::function(env, container, key, _value);
            Super::addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
            Super::addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), _value);
        }else{
            AssociativeContainerInsert<QMap, K, T>::function(env, container, key, _value);
        }
    }

    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override {
        jint result;
        if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            result = AssociativeContainerRemove<QMap, K, T>::function(env, container, key);
            Super::removeRC(env, container.object, key, result);
        }else if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                result = AssociativeContainerRemove<QMap, K, T>::function(env, container, key);
                if(result>0)
                    Super::updateRC(env, container);
            }else{
                result = AssociativeContainerRemove<QMap, K, T>::function(env, container, key);
                if(result>0){
                    Super::removeRC(env, container.object, key);
                }
            }
        }else if constexpr((ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                result = AssociativeContainerRemove<QMap, K, T>::function(env, container, key);

            }else{
                jobject oldValue = value(env, container.container, key, nullptr);
                result = AssociativeContainerRemove<QMap, K, T>::function(env, container, key);
                if(result>0){
                    Super::removeRC(env, container.object, oldValue);
                }
            }
        }else if constexpr(ContainerContentType<K>::needsReferenceCounting || ContainerContentType<T>::needsReferenceCounting){
            result = AssociativeContainerRemove<QMap, K, T>::function(env, container, key);
            if(result>0)
                Super::updateRC(env, container);
        }else{
            result = AssociativeContainerRemove<QMap, K, T>::function(env, container, key);
        }
        return result;
    }

    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override {
        jobject result = AssociativeContainerTake<QMap, K, T>::function(env, container, key);
        if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            Super::removeRC(env, container.object, key);
        }else if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                Super::updateRC(env, container);
            }else{
                Super::removeRC(env, container.object, key);
            }
        }else if constexpr((ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                Super::updateRC(env, container);
            }else{
                Super::removeRC(env, container.object, result);
            }
        }else if constexpr(ContainerContentType<K>::needsReferenceCounting || ContainerContentType<T>::needsReferenceCounting){
            Super::updateRC(env, container);
        }
        return result;
    }
private:
    class KeyValueIterator : public AbstractMapAccess::KeyValueIterator{
    public:
        QMapAccess<K,T>* access;
        typename QMap<K,T>::ConstIterator current;
        typename QMap<K,T>::ConstIterator end;
        KeyValueIterator(QMapAccess<K,T>* _access, const QMap<K,T>& container)
            : access(_access),
            current(container.constBegin()),
            end(container.constEnd()) {}
        ~KeyValueIterator() override {};
        bool hasNext() override {return current!=end;};
        QPair<jobject,jobject> next(JNIEnv * env) override {
            return {qtjambi_scoped_cast<false,jobject,decltype(current.key())>::cast(env, current.key(), nullptr, nullptr),
                    qtjambi_scoped_cast<false,jobject,decltype(current.value())>::cast(env, current.value(), nullptr, nullptr)};
            ++current;
        }
        QPair<const void*,const void*> next() override {
            QPair<const void*,const void*> result{ContainerContentDeref<K>::deref(current.key()), ContainerContentDeref<T>::deref(current.value())};
            ++current;
            return result;
        }
    };
public:
    std::unique_ptr<AbstractMapAccess::KeyValueIterator> keyValueIterator(const void* container) override {
        return std::unique_ptr<AbstractMapAccess::KeyValueIterator>(new KeyValueIterator(this, *reinterpret_cast<const QMap<K,T>*>(container)));
    }
};

template<typename K, typename T>
class QMultiMapAccess : public AssociativeAccessSuperclassDecider<K,T,AbstractMultiMapAccess>::type{
    typedef typename AssociativeAccessSuperclassDecider<K,T,AbstractMultiMapAccess>::type Super;
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
        if constexpr ((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0 && (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if(ReferenceCountingMultiMapContainer* access = dynamic_cast<ReferenceCountingMultiMapContainer*>(container2.access)){
                if(container2.access!=this)
                    Super::swapRC(env, container, container2);
            }else{
                Super::updateRC(env, container);
            }
        }else if constexpr(ContainerContentType<QMultiMap<K,T>>::needsReferenceCounting){
            if(ReferenceCountingSetContainer* access = dynamic_cast<ReferenceCountingSetContainer*>(container2.access)){
                if(container2.access!=this)
                    Super::swapRC(env, container, container2);
            }else{
                Super::updateRC(env, container);
            }
        }else{
            Q_UNUSED(env);
        }
    }

    const QMetaType& keyMetaType() override {
        static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(K));
        return type;
    }

    const QMetaType& valueMetaType() override {
        static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(T));
        return type;
    }

    AbstractContainerAccess::DataType keyType() override{
        return ContainerContentType<K>::type;
    }

    AbstractContainerAccess::DataType valueType() override{
        return ContainerContentType<T>::type;
    }

    AbstractContainerAccess* keyNestedContainerAccess() override {
        return ContainerContentType<K>::accessFactory();
    }

    AbstractContainerAccess* valueNestedContainerAccess() override {
        return ContainerContentType<T>::accessFactory();
    }

    bool hasKeyNestedContainerAccess() override {
        return ContainerContentType<K>::isContainer;
    }
    bool hasValueNestedContainerAccess() override {
        return ContainerContentType<T>::isContainer;
    }
    bool hasKeyNestedPointers() override {
        return ContainerContentType<K>::isContainer && ContainerContentType<K>::needsReferenceCounting;
    }
    bool hasValueNestedPointers() override {
        return ContainerContentType<T>::isContainer && ContainerContentType<T>::needsReferenceCounting;
    }

    size_t sizeOf() override {
        return sizeof(QMultiMap<K,T>);
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* move) override {
        return new(placement) QMultiMap<K,T>(std::move(*reinterpret_cast<const QMultiMap<K,T>*>(move)));
    }
    void* constructContainer(JNIEnv *, void* placement, const ContainerAndAccessInfo& move) override {
        return constructContainer(placement, move.container);
    }
#endif
    bool destructContainer(void* container) override {
        reinterpret_cast<QMultiMap<K,T>*>(container)->~QMultiMap<K,T>();
        return true;
    }

    void assign(void* container, const void* other) override {
        (*reinterpret_cast<QMultiMap<K,T>*>(container)) = (*reinterpret_cast<const QMultiMap<K,T>*>(other));
    }

    void assign(JNIEnv *env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override {
        (*reinterpret_cast<QMultiMap<K,T>*>(container.container)) = (*reinterpret_cast<const QMultiMap<K,T>*>(other.container));
        if constexpr ((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0 && (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if(ReferenceCountingMapContainer* access = dynamic_cast<ReferenceCountingMapContainer*>(other.access)){
                if(other.access!=this)
                    Super::assignRC(env, container.object, other.object);
            }else{
                Super::updateRC(env, container);
            }
        }else if constexpr(ContainerContentType<QMultiMap<K,T>>::needsReferenceCounting){
            if(ReferenceCountingSetContainer* access = dynamic_cast<ReferenceCountingSetContainer*>(other.access)){
                if(other.access!=this)
                    Super::assignRC(env, container.object, other.object);
            }else{
                Super::updateRC(env, container);
            }
        }else{
            Q_UNUSED(env);
        }
    }

    int registerContainer(const QByteArray& containerTypeName) override {
        return RegistryAPI::registerMetaType<QMultiMap<K,T>>(containerTypeName, this);
    }

    jboolean contains(JNIEnv * env, const void* container, jobject key) override {
        return AssociativeContainerContains<QMultiMap, K, T>::function(env, container, key);
    }

    jint count(JNIEnv * env, const void* container, jobject key) override {
        return AssociativeContainerCountObject<QMultiMap, K, T>::function(env, container, key);
    }

    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        return AssociativeContainerConstEnd<QMultiMap, K, T>::function(env, container);
    }

    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        return AssociativeContainerConstBegin<QMultiMap, K, T>::function(env, container);
    }

    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override {
        return AssociativeContainerConstFindPair<QMultiMap, K, T>::function(env, container, key);
    }

    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key, jobject value) override {
        return AssociativeContainerConstFindPairs<QMultiMap, K, T>::function(env, container, key, value);
    }
    jobject first(JNIEnv * env, const void* container) override {
        return AssociativeContainerFirst<QMultiMap, K, T>::function(env, container);
    }

    jobject firstKey(JNIEnv * env, const void* container) override {
        return AssociativeContainerFirstKey<QMultiMap, K, T>::function(env, container);
    }

    jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override {
        return AssociativeContainerKey<QMultiMap, K, T>::function(env, container, value, defaultKey);
    }

    ContainerAndAccessInfo keys(JNIEnv * env, const ConstContainerInfo& container) override {
        return AssociativeContainerKeys<QMultiMap, K, T>::function(env, container);
    }

    ContainerAndAccessInfo keys(JNIEnv * env, const ConstContainerInfo& container, jobject value) override {
        return AssociativeContainerKeysForValue<QMultiMap, K, T>::function(env, container, value);
    }

    jobject last(JNIEnv * env, const void* container) override {
        return AssociativeContainerLast<QMultiMap, K, T>::function(env, container);
    }

    jobject lastKey(JNIEnv * env, const void* container) override {
        return AssociativeContainerLastKey<QMultiMap, K, T>::function(env, container);
    }

    jobject constLowerBound(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override {
        return AssociativeContainerLowerBound<QMultiMap, K, T>::function(env, container, key);
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        return AssociativeContainerEquals<QMultiMap, K, T, ContainerAPI::getAsQMultiMap>::function(env, container, other);
    }

    jint size(JNIEnv * env, const void* container) override {
        return AssociativeContainerSize<QMultiMap, K, T>::function(env, container);
    }

    ContainerAndAccessInfo uniqueKeys(JNIEnv * env, const ConstContainerInfo& container) override {
        return AssociativeContainerUniqueKeys<QMultiMap, K, T>::function(env, container);
    }

    ContainerAndAccessInfo values(JNIEnv * env, const ConstContainerInfo& container, jobject key) override {
        return AssociativeContainerValuesKey<QMultiMap, K, T>::function(env, container, key);
    }

    jobject constUpperBound(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override {
        return AssociativeContainerUpperBound<QMultiMap, K, T>::function(env, container, key);
    }

    jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override {
        return AssociativeContainerValue<QMultiMap, K, T>::function(env, container, key, defaultValue);
    }

    ContainerAndAccessInfo values(JNIEnv * env, const ConstContainerInfo& container) override {
        return AssociativeContainerValues<QMultiMap, K, T>::function(env, container);
    }

    jboolean contains(JNIEnv *env, const void* container, jobject key, jobject value) override {
        return AssociativeContainerContainsPair<QMultiMap, K, T>::function(env, container, key, value);
    }

    jint count(JNIEnv *env, const void* container, jobject key, jobject value) override {
        return AssociativeContainerCountPair<QMultiMap, K, T>::function(env, container, key, value);
    }

    bool keyLessThan(JNIEnv *env, jobject key1, jobject key2) override {
        return AssociativeContainerElementLessThan<K>::function(env, key1, key2);
    }

    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override {
        return AssociativeContainerBegin<QMultiMap, K, T>::function(env, container);
    }

    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override {
        return AssociativeContainerEnd<QMultiMap, K, T>::function(env, container);
    }

    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key) override {
        return AssociativeContainerFindPair<QMultiMap, K, T>::function(env, container, key);
    }

    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key, jobject value) override {
        return AssociativeContainerFindPairs<QMultiMap, K, T>::function(env, container, key, value);
    }

    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key, jobject _value) override {
        jint result;
        if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            result = AssociativeContainerRemovePair<QMultiMap, K, T>::function(env, container, key, _value);
            Super::removeRC(env, container.object, key, _value, result);
        }else if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                result = AssociativeContainerRemovePair<QMultiMap, K, T>::function(env, container, key, _value);
                if(result>0)
                    Super::updateRC(env, container);
            }else{
                result = AssociativeContainerRemovePair<QMultiMap, K, T>::function(env, container, key, _value);
                if(result>0){
                    Super::removeRC(env, container.object, key);
                }
            }
        }else if constexpr((ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                result = AssociativeContainerRemovePair<QMultiMap, K, T>::function(env, container, key, _value);

            }else{
                result = AssociativeContainerRemovePair<QMultiMap, K, T>::function(env, container, key, _value);
                Super::removeRC(env, container.object, _value, result);
            }
        }else if constexpr(ContainerContentType<K>::needsReferenceCounting || ContainerContentType<T>::needsReferenceCounting){
            result = AssociativeContainerRemovePair<QMultiMap, K, T>::function(env, container, key, _value);
            if(result>0)
                Super::updateRC(env, container);
        }else{
            result = AssociativeContainerRemovePair<QMultiMap, K, T>::function(env, container, key, _value);
        }
        return result;
    }

    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject _value) override {
        if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            AssociativeContainerReplacePair<QMultiMap, K, T>::function(env, container, key, _value);
            Super::removeRC(env, container.object, key);
            Super::putRC(env, container.object, key, _value);
        }else if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                AssociativeContainerReplacePair<QMultiMap, K, T>::function(env, container, key, _value);
                Super::updateRC(env, container);
            }else{
                AssociativeContainerReplacePair<QMultiMap, K, T>::function(env, container, key, _value);
                Super::addRC(env, container.object, key);
            }
        }else if constexpr((ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                AssociativeContainerReplacePair<QMultiMap, K, T>::function(env, container, key, _value);

            }else{
                jobject oldValue = value(env, container.container, key, nullptr);
                AssociativeContainerReplacePair<QMultiMap, K, T>::function(env, container, key, _value);
                Super::removeRC(env, container.object, oldValue);
                Super::addRC(env, container.object, _value);
            }
        }else if constexpr(ContainerContentType<K>::needsReferenceCounting || ContainerContentType<T>::needsReferenceCounting){
            AssociativeContainerReplacePair<QMultiMap, K, T>::function(env, container, key, _value);
            Super::updateRC(env, container);
        }else{
            AssociativeContainerReplacePair<QMultiMap, K, T>::function(env, container, key, _value);
        }
    }

    void clear(JNIEnv * env, const ContainerInfo& container) override {
        AssociativeContainerClear<QMultiMap, K, T>::function(env, container);
        if constexpr(ContainerContentType<QMultiMap<K,T>>::needsReferenceCounting){
            Super::clearRC(env, container.object);
        }
    }

    void insert(JNIEnv *env, const ContainerInfo& container,jobject key,jobject _value) override {
        if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            AssociativeContainerInsert<QMultiMap, K, T>::function(env, container, key, _value);
            Super::putRC(env, container.object, key, _value);
        }else if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                AssociativeContainerInsert<QMultiMap, K, T>::function(env, container, key, _value);
                Super::addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
                Super::addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), _value);
            }else{
                AssociativeContainerInsert<QMultiMap, K, T>::function(env, container, key, _value);
                Super::addUniqueRC(env, container.object, key);
            }
        }else if constexpr((ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                AssociativeContainerInsert<QMultiMap, K, T>::function(env, container, key, _value);
                Super::addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
                Super::addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), _value);
            }else{
                jobject oldValue = value(env, container.container, key, nullptr);
                AssociativeContainerInsert<QMultiMap, K, T>::function(env, container, key, _value);
                Super::removeRC(env, container.object, oldValue);
                Super::addRC(env, container.object, _value);
            }
        }else if constexpr(ContainerContentType<K>::needsReferenceCounting || ContainerContentType<T>::needsReferenceCounting){
            AssociativeContainerInsert<QMultiMap, K, T>::function(env, container, key, _value);
            Super::addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
            Super::addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), _value);
        }else{
            AssociativeContainerInsert<QMultiMap, K, T>::function(env, container, key, _value);
        }
    }

    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override {
        jint result;
        if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            result = AssociativeContainerRemove<QMultiMap, K, T>::function(env, container, key);
            Super::removeRC(env, container.object, key, result);
        }else if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                result = AssociativeContainerRemove<QMultiMap, K, T>::function(env, container, key);
                if(result>0)
                    Super::updateRC(env, container);
            }else{
                result = AssociativeContainerRemove<QMultiMap, K, T>::function(env, container, key);
                if(result>0){
                    Super::removeRC(env, container.object, key);
                }
            }
        }else if constexpr((ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                result = AssociativeContainerRemove<QMultiMap, K, T>::function(env, container, key);

            }else{
                jobject oldValue = value(env, container.container, key, nullptr);
                result = AssociativeContainerRemove<QMultiMap, K, T>::function(env, container, key);
                if(result>0){
                    Super::removeRC(env, container.object, oldValue);
                }
            }
        }else if constexpr(ContainerContentType<K>::needsReferenceCounting || ContainerContentType<T>::needsReferenceCounting){
            result = AssociativeContainerRemove<QMultiMap, K, T>::function(env, container, key);
            if(result>0)
                Super::updateRC(env, container);
        }else{
            result = AssociativeContainerRemove<QMultiMap, K, T>::function(env, container, key);
        }
        return result;
    }

    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override {
        jobject result = AssociativeContainerTake<QMultiMap, K, T>::function(env, container, key);
        if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            Super::removeRC(env, container.object, key);
        }else if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                Super::updateRC(env, container);
            }else{
                Super::removeRC(env, container.object, key);
            }
        }else if constexpr((ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                Super::updateRC(env, container);
            }else{
                Super::removeRC(env, container.object, result);
            }
        }else if constexpr(ContainerContentType<K>::needsReferenceCounting || ContainerContentType<T>::needsReferenceCounting){
            Super::updateRC(env, container);
        }
        return result;
    }

    void unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other) override {
        AssociativeContainerUnite<QMultiMap, K, T, ContainerAPI::getAsQMultiMap>::function(env, container, other);
        if constexpr(ContainerContentType<QMultiMap<K,T>>::needsReferenceCounting){
            Super::updateRC(env, container);
        }
    }
private:
    class KeyValueIterator : public AbstractMapAccess::KeyValueIterator{
    public:
        QMultiMapAccess* access;
        typename QMultiMap<K,T>::ConstIterator current;
        typename QMultiMap<K,T>::ConstIterator end;
        KeyValueIterator(QMultiMapAccess* _access, const void* container)
            : access(_access){
            current = reinterpret_cast<const QMultiMap<K,T>*>(container)->constBegin();
            end = reinterpret_cast<const QMultiMap<K,T>*>(container)->constEnd();
        }
        ~KeyValueIterator() override {};
        bool hasNext() override {return current!=end;};
        QPair<jobject,jobject> next(JNIEnv * env) override {
            QPair<jobject,jobject> result{qtjambi_scoped_cast<false,jobject,decltype(current.key())>::cast(env, current.key(), nullptr, nullptr),
                                            qtjambi_scoped_cast<false,jobject,decltype(current.value())>::cast(env, current.value(), nullptr, nullptr)};
            ++current;
            return result;
        }
        QPair<const void*,const void*> next() override {
            QPair<const void*,const void*> result{ContainerContentDeref<K>::deref(current.key()), ContainerContentDeref<T>::deref(current.value())};
            ++current;
            return result;
        }
    };
public:
    std::unique_ptr<AbstractMapAccess::KeyValueIterator> keyValueIterator(const void* container) override {
        return std::unique_ptr<AbstractMapAccess::KeyValueIterator>(new KeyValueIterator(this, container));
    }
};

template<typename K, typename T>
class QHashAccess : public AssociativeAccessSuperclassDecider<K,T,AbstractHashAccess>::type{
    typedef typename AssociativeAccessSuperclassDecider<K,T,AbstractHashAccess>::type Super;
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
        if constexpr ((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0 && (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if(ReferenceCountingMapContainer* access = dynamic_cast<ReferenceCountingMapContainer*>(container2.access)){
                if(container2.access!=this)
                    Super::swapRC(env, container, container2);
            }else{
                Super::updateRC(env, container);
            }
        }else if constexpr(ContainerContentType<QHash<K,T>>::needsReferenceCounting){
            if(ReferenceCountingSetContainer* access = dynamic_cast<ReferenceCountingSetContainer*>(container2.access)){
                if(container2.access!=this)
                    Super::swapRC(env, container, container2);
            }else{
                Super::updateRC(env, container);
            }
        }else{
            Q_UNUSED(env);
        }
    }

    const QMetaType& keyMetaType() override {
        static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(K));
        return type;
    }

    const QMetaType& valueMetaType() override {
        static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(T));
        return type;
    }

    AbstractContainerAccess::DataType keyType() override{
        return ContainerContentType<K>::type;
    }

    AbstractContainerAccess::DataType valueType() override{
        return ContainerContentType<T>::type;
    }

    AbstractContainerAccess* keyNestedContainerAccess() override {
        return ContainerContentType<K>::accessFactory();
    }

    AbstractContainerAccess* valueNestedContainerAccess() override {
        return ContainerContentType<T>::accessFactory();
    }

    bool hasKeyNestedContainerAccess() override {
        return ContainerContentType<K>::isContainer;
    }
    bool hasValueNestedContainerAccess() override {
        return ContainerContentType<T>::isContainer;
    }
    bool hasKeyNestedPointers() override {
        return ContainerContentType<K>::isContainer && ContainerContentType<K>::needsReferenceCounting;
    }
    bool hasValueNestedPointers() override {
        return ContainerContentType<T>::isContainer && ContainerContentType<T>::needsReferenceCounting;
    }

    void assign(void* container, const void* other) override {
        (*reinterpret_cast<QHash<K,T>*>(container)) = (*reinterpret_cast<const QHash<K,T>*>(other));
    }

    void assign(JNIEnv *env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override {
        (*reinterpret_cast<QHash<K,T>*>(container.container)) = (*reinterpret_cast<const QHash<K,T>*>(other.container));
        if constexpr ((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0 && (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if(ReferenceCountingMapContainer* access = dynamic_cast<ReferenceCountingMapContainer*>(other.access)){
                if(other.access!=this)
                    Super::assignRC(env, container.object, other.object);
            }else{
                Super::updateRC(env, container);
            }
        }else if constexpr(ContainerContentType<QHash<K,T>>::needsReferenceCounting){
            if(ReferenceCountingSetContainer* access = dynamic_cast<ReferenceCountingSetContainer*>(other.access)){
                if(other.access!=this)
                    Super::assignRC(env, container.object, other.object);
            }else{
                Super::updateRC(env, container);
            }
        }else{
            Q_UNUSED(env);
        }
    }

    int registerContainer(const QByteArray& containerTypeName) override {
        return RegistryAPI::registerMetaType<QHash<K,T>>(containerTypeName, this);
    }

    size_t sizeOf() override {
        return sizeof(QHash<K,T>);
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
 #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* move) override {
        return new(placement) QHash<K,T>(std::move(*reinterpret_cast<const QHash<K,T>*>(move)));
    }
    void* constructContainer(JNIEnv *, void* placement, const ContainerAndAccessInfo& move) override {
        return constructContainer(placement, move.container);
    }
 #endif
    bool destructContainer(void* container) override {
        reinterpret_cast<QHash<K,T>*>(container)->~QHash<K,T>();
        return true;
    }

    jint capacity(JNIEnv * env, const void* container) override {
        return AssociativeContainerCapacity<QHash, K, T>::function(env, container);
    }

    jboolean contains(JNIEnv * env, const void* container, jobject key) override {
        return AssociativeContainerContains<QHash, K, T>::function(env, container, key);
    }

    jint count(JNIEnv * env, const void* container, jobject key) override {
        return AssociativeContainerCountObject<QHash, K, T>::function(env, container, key);
    }

    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        return AssociativeContainerConstEnd<QHash, K, T>::function(env, container);
    }

    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        return AssociativeContainerConstBegin<QHash, K, T>::function(env, container);
    }

    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override {
        return AssociativeContainerConstFindPair<QHash, K, T>::function(env, container, key);
    }

    jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override {
        return AssociativeContainerKey<QHash, K, T>::function(env, container, value, defaultKey);
    }

    ContainerAndAccessInfo keys(JNIEnv * env, const ConstContainerInfo& container) override {
        return AssociativeContainerKeys<QHash, K, T>::function(env, container);
    }

    ContainerAndAccessInfo keys(JNIEnv * env, const ConstContainerInfo& container, jobject value) override {
        return AssociativeContainerKeysForValue<QHash, K, T>::function(env, container, value);
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        return AssociativeContainerEquals<QHash, K, T, ContainerAPI::getAsQHash>::function(env, container, other);
    }

    jint size(JNIEnv * env, const void* container) override {
        return AssociativeContainerSize<QHash, K, T>::function(env, container);
    }

    jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override {
        return AssociativeContainerValue<QHash, K, T>::function(env, container, key, defaultValue);
    }

    ContainerAndAccessInfo values(JNIEnv * env, const ConstContainerInfo& container) override {
        return AssociativeContainerValues<QHash, K, T>::function(env, container);
    }

    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override {
        return AssociativeContainerBegin<QHash, K, T>::function(env, container);
    }
    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override {
        return AssociativeContainerEnd<QHash, K, T>::function(env, container);
    }
    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key) override {
        return AssociativeContainerFindPair<QHash, K, T>::function(env, container, key);
    }
    void clear(JNIEnv * env, const ContainerInfo& container) override {
        AssociativeContainerClear<QHash, K, T>::function(env, container);
        if constexpr(ContainerContentType<QHash<K,T>>::needsReferenceCounting){
            Super::clearRC(env, container.object);
        }
    }
    void insert(JNIEnv *env, const ContainerInfo& container,jobject key,jobject _value) override {
        if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            AssociativeContainerInsert<QHash, K, T>::function(env, container, key, _value);
            Super::putRC(env, container.object, key, _value);
        }else if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                AssociativeContainerInsert<QHash, K, T>::function(env, container, key, _value);
                Super::addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
                Super::addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), _value);
            }else{
                AssociativeContainerInsert<QHash, K, T>::function(env, container, key, _value);
                Super::addUniqueRC(env, container.object, key);
            }
        }else if constexpr((ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                AssociativeContainerInsert<QHash, K, T>::function(env, container, key, _value);
                Super::addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
                Super::addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), _value);
            }else{
                jobject oldValue = value(env, container.container, key, nullptr);
                AssociativeContainerInsert<QHash, K, T>::function(env, container, key, _value);
                Super::removeRC(env, container.object, oldValue);
                Super::addRC(env, container.object, _value);
            }
        }else if constexpr(ContainerContentType<K>::needsReferenceCounting || ContainerContentType<T>::needsReferenceCounting){
            AssociativeContainerInsert<QHash, K, T>::function(env, container, key, _value);
            Super::addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
            Super::addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), _value);
        }else{
            AssociativeContainerInsert<QHash, K, T>::function(env, container, key, _value);
        }
    }
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override {
        jint result;
        if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            result = AssociativeContainerRemove<QHash, K, T>::function(env, container, key);
            Super::removeRC(env, container.object, key, result);
        }else if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                result = AssociativeContainerRemove<QHash, K, T>::function(env, container, key);
                if(result>0)
                    Super::updateRC(env, container);
            }else{
                result = AssociativeContainerRemove<QHash, K, T>::function(env, container, key);
                if(result>0){
                    Super::removeRC(env, container.object, key);
                }
            }
        }else if constexpr((ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                result = AssociativeContainerRemove<QHash, K, T>::function(env, container, key);

            }else{
                jobject oldValue = value(env, container.container, key, nullptr);
                result = AssociativeContainerRemove<QHash, K, T>::function(env, container, key);
                if(result>0){
                    Super::removeRC(env, container.object, oldValue);
                }
            }
        }else if constexpr(ContainerContentType<K>::needsReferenceCounting || ContainerContentType<T>::needsReferenceCounting){
            result = AssociativeContainerRemove<QHash, K, T>::function(env, container, key);
            if(result>0)
                Super::updateRC(env, container);
        }else{
            result = AssociativeContainerRemove<QHash, K, T>::function(env, container, key);
        }
        return result;
    }
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override {
        jobject result = AssociativeContainerTake<QHash, K, T>::function(env, container, key);
        if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            Super::removeRC(env, container.object, key);
        }else if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                Super::updateRC(env, container);
            }else{
                Super::removeRC(env, container.object, key);
            }
        }else if constexpr((ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                Super::updateRC(env, container);
            }else{
                Super::removeRC(env, container.object, result);
            }
        }else if constexpr(ContainerContentType<K>::needsReferenceCounting || ContainerContentType<T>::needsReferenceCounting){
            Super::updateRC(env, container);
        }
        return result;
    }
    void reserve(JNIEnv * env, const ContainerInfo& container, jint newSize) override {
        AssociativeContainerReserve<QHash, K, T>::function(env, container, newSize);
    }
private:
    class KeyValueIterator : public AbstractHashAccess::KeyValueIterator{
    public:
        QHashAccess<K,T>* access;
        typename QHash<K,T>::ConstIterator current;
        typename QHash<K,T>::ConstIterator end;
        KeyValueIterator(QHashAccess<K,T>* _access, const QHash<K,T>& container)
            : access(_access), current(container.constBegin()),
              end(container.constEnd()) {}
        ~KeyValueIterator() override {};
        bool hasNext() override {return current!=end;};
        QPair<jobject,jobject> next(JNIEnv * env) override {
            return {qtjambi_scoped_cast<false,jobject,decltype(current.key())>::cast(env, current.key(), nullptr, nullptr),
                    qtjambi_scoped_cast<false,jobject,decltype(current.value())>::cast(env, current.value(), nullptr, nullptr)};
            ++current;
        }
        QPair<const void*,const void*> next() override {
            QPair<const void*,const void*> result{ContainerContentDeref<K>::deref(current.key()), ContainerContentDeref<T>::deref(current.value())};
            ++current;
            return result;
        }
    };
public:
    std::unique_ptr<AbstractHashAccess::KeyValueIterator> keyValueIterator(const void* container) override {
        return std::unique_ptr<AbstractHashAccess::KeyValueIterator>(new KeyValueIterator(this, *reinterpret_cast<const QHash<K,T>*>(container)));
    }
 };

template<typename K, typename T>
 class QMultiHashAccess : public AssociativeAccessSuperclassDecider<K,T,AbstractMultiHashAccess>::type{
    typedef typename AssociativeAccessSuperclassDecider<K,T,AbstractMultiHashAccess>::type Super;
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
        if constexpr ((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0 && (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if(ReferenceCountingMultiMapContainer* access = dynamic_cast<ReferenceCountingMultiMapContainer*>(container2.access)){
                if(container2.access!=this)
                    Super::swapRC(env, container, container2);
            }else{
                Super::updateRC(env, container);
            }
        }else if constexpr(ContainerContentType<QMultiHash<K,T>>::needsReferenceCounting){
            if(ReferenceCountingSetContainer* access = dynamic_cast<ReferenceCountingSetContainer*>(container2.access)){
                if(container2.access!=this)
                    Super::swapRC(env, container, container2);
            }else{
                Super::updateRC(env, container);
            }
        }else{
            Q_UNUSED(env);
        }
    }

    const QMetaType& keyMetaType() override {
        static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(K));
        return type;
    }

    const QMetaType& valueMetaType() override {
        static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(T));
        return type;
    }

    AbstractContainerAccess::DataType keyType() override{
        return ContainerContentType<K>::type;
    }

    AbstractContainerAccess::DataType valueType() override{
        return ContainerContentType<T>::type;
    }

    AbstractContainerAccess* keyNestedContainerAccess() override {
        return ContainerContentType<K>::accessFactory();
    }

    AbstractContainerAccess* valueNestedContainerAccess() override {
        return ContainerContentType<T>::accessFactory();
    }

    bool hasKeyNestedContainerAccess() override {
        return ContainerContentType<K>::isContainer;
    }
    bool hasValueNestedContainerAccess() override {
        return ContainerContentType<T>::isContainer;
    }
    bool hasKeyNestedPointers() override {
        return ContainerContentType<K>::isContainer && ContainerContentType<K>::needsReferenceCounting;
    }
    bool hasValueNestedPointers() override {
        return ContainerContentType<T>::isContainer && ContainerContentType<T>::needsReferenceCounting;
    }

    void assign(void* container, const void* other) override {
        (*reinterpret_cast<QMultiHash<K,T>*>(container)) = (*reinterpret_cast<const QMultiHash<K,T>*>(other));
    }
    void assign(JNIEnv *env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override {
        (*reinterpret_cast<QMultiHash<K,T>*>(container.container)) = (*reinterpret_cast<const QMultiHash<K,T>*>(other.container));
        if constexpr ((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0 && (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if(ReferenceCountingMapContainer* access = dynamic_cast<ReferenceCountingMapContainer*>(other.access)){
                if(other.access!=this)
                    Super::assignRC(env, container.object, other.object);
            }else{
                Super::updateRC(env, container);
            }
        }else if constexpr(ContainerContentType<QMultiHash<K,T>>::needsReferenceCounting){
            if(ReferenceCountingSetContainer* access = dynamic_cast<ReferenceCountingSetContainer*>(other.access)){
                if(other.access!=this)
                    Super::assignRC(env, container.object, other.object);
            }else{
                Super::updateRC(env, container);
            }
        }else{
            Q_UNUSED(env);
        }
    }
    int registerContainer(const QByteArray& containerTypeName) override {
        return RegistryAPI::registerMetaType<QMultiHash<K,T>>(containerTypeName, this);
    }

    size_t sizeOf() override {
        return sizeof(QMultiHash<K,T>);
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* move) override {
        return new(placement) QMultiHash<K,T>(std::move(*reinterpret_cast<const QMultiHash<K,T>*>(move)));
    }
    void* constructContainer(JNIEnv *, void* placement, const ContainerAndAccessInfo& move) override {
        return constructContainer(placement, move.container);
    }
#endif
    bool destructContainer(void* container) override {
        reinterpret_cast<QMultiHash<K,T>*>(container)->~QMultiHash<K,T>();
        return true;
    }
    jint capacity(JNIEnv * env, const void* container) override {
        return AssociativeContainerCapacity<QMultiHash, K, T>::function(env, container);
    }
    jboolean contains(JNIEnv * env, const void* container, jobject key) override {
        return AssociativeContainerContains<QMultiHash, K, T>::function(env, container, key);
    }
    jint count(JNIEnv * env, const void* container, jobject key) override {
        return AssociativeContainerCountObject<QMultiHash, K, T>::function(env, container, key);
    }
    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        return AssociativeContainerConstEnd<QMultiHash, K, T>::function(env, container);
    }
    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        return AssociativeContainerConstBegin<QMultiHash, K, T>::function(env, container);
    }
    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override {
        return AssociativeContainerConstFindPair<QMultiHash, K, T>::function(env, container, key);
    }
    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key, jobject value) override {
        return AssociativeContainerConstFindPairs<QMultiHash, K, T>::function(env, container, key, value);
    }
    jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override {
        return AssociativeContainerKey<QMultiHash, K, T>::function(env, container, value, defaultKey);
    }
    ContainerAndAccessInfo keys(JNIEnv * env, const ConstContainerInfo& container) override {
        return AssociativeContainerKeys<QMultiHash, K, T>::function(env, container);
    }
    ContainerAndAccessInfo keys(JNIEnv * env, const ConstContainerInfo& container, jobject value) override {
        return AssociativeContainerKeysForValue<QMultiHash, K, T>::function(env, container, value);
    }
    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        return AssociativeContainerEquals<QMultiHash, K, T, ContainerAPI::getAsQMultiHash>::function(env, container, other);
    }
    void reserve(JNIEnv * env,const ContainerInfo&,jint) override {
        JavaException::raiseUnsupportedOperationException(env, "QMultiHash::reserve(size)" QTJAMBI_STACKTRACEINFO );
    }
    jint size(JNIEnv * env, const void* container) override {
        return AssociativeContainerSize<QMultiHash, K, T>::function(env, container);
    }
    ContainerAndAccessInfo uniqueKeys(JNIEnv * env, const ConstContainerInfo& container) override {
        return AssociativeContainerUniqueKeys<QMultiHash, K, T>::function(env, container);
    }
    ContainerAndAccessInfo values(JNIEnv * env, const ConstContainerInfo& container, jobject key) override {
        return AssociativeContainerValuesKey<QMultiHash, K, T>::function(env, container, key);
    }
    jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override {
        return AssociativeContainerValue<QMultiHash, K, T>::function(env, container, key, defaultValue);
    }
    ContainerAndAccessInfo values(JNIEnv * env, const ConstContainerInfo& container) override {
        return AssociativeContainerValues<QMultiHash, K, T>::function(env, container);
    }
    jboolean contains(JNIEnv *env, const void* container, jobject key, jobject value) override {
        return AssociativeContainerContainsPair<QMultiHash, K, T>::function(env, container, key, value);
    }
    jint count(JNIEnv *env, const void* container, jobject key, jobject value) override {
        return AssociativeContainerCountPair<QMultiHash, K, T>::function(env, container, key, value);
    }
    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override {
        return AssociativeContainerBegin<QMultiHash, K, T>::function(env, container);
    }
    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override {
        return AssociativeContainerEnd<QMultiHash, K, T>::function(env, container);
    }
    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key) override {
        return AssociativeContainerFindPair<QMultiHash, K, T>::function(env, container, key);
    }
    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key, jobject value) override {
        return AssociativeContainerFindPairs<QMultiHash, K, T>::function(env, container, key, value);
    }
    void clear(JNIEnv * env, const ContainerInfo& container) override {
        AssociativeContainerClear<QMultiHash, K, T>::function(env, container);
        if constexpr(ContainerContentType<QMultiHash<K,T>>::needsReferenceCounting){
            Super::clearRC(env, container.object);
        }
    }
    void insert(JNIEnv *env, const ContainerInfo& container,jobject key,jobject _value) override {
        if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            AssociativeContainerInsert<QMultiHash, K, T>::function(env, container, key, _value);
            Super::putRC(env, container.object, key, _value);
        }else if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                AssociativeContainerInsert<QMultiHash, K, T>::function(env, container, key, _value);
                Super::addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
                Super::addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), _value);
            }else{
                AssociativeContainerInsert<QMultiHash, K, T>::function(env, container, key, _value);
                Super::addUniqueRC(env, container.object, key);
            }
        }else if constexpr((ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                AssociativeContainerInsert<QMultiHash, K, T>::function(env, container, key, _value);
                Super::addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
                Super::addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), _value);
            }else{
                jobject oldValue = value(env, container.container, key, nullptr);
                AssociativeContainerInsert<QMultiHash, K, T>::function(env, container, key, _value);
                Super::removeRC(env, container.object, oldValue);
                Super::addRC(env, container.object, _value);
            }
        }else if constexpr(ContainerContentType<K>::needsReferenceCounting || ContainerContentType<T>::needsReferenceCounting){
            AssociativeContainerInsert<QMultiHash, K, T>::function(env, container, key, _value);
            Super::addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
            Super::addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), _value);
        }else{
            AssociativeContainerInsert<QMultiHash, K, T>::function(env, container, key, _value);
        }
    }
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override {
        jint result;
        if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            result = AssociativeContainerRemove<QMultiHash, K, T>::function(env, container, key);
            Super::removeRC(env, container.object, key, result);
        }else if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                result = AssociativeContainerRemove<QMultiHash, K, T>::function(env, container, key);
                if(result>0)
                    Super::updateRC(env, container);
            }else{
                result = AssociativeContainerRemove<QMultiHash, K, T>::function(env, container, key);
                if(result>0){
                    Super::removeRC(env, container.object, key);
                }
            }
        }else if constexpr((ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                result = AssociativeContainerRemove<QMultiHash, K, T>::function(env, container, key);

            }else{
                jobject oldValue = value(env, container.container, key, nullptr);
                result = AssociativeContainerRemove<QMultiHash, K, T>::function(env, container, key);
                if(result>0){
                    Super::removeRC(env, container.object, oldValue);
                }
            }
        }else if constexpr(ContainerContentType<K>::needsReferenceCounting || ContainerContentType<T>::needsReferenceCounting){
            result = AssociativeContainerRemove<QMultiHash, K, T>::function(env, container, key);
            if(result>0)
                Super::updateRC(env, container);
        }else{
            result = AssociativeContainerRemove<QMultiHash, K, T>::function(env, container, key);
        }
        return result;
    }
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override {
        jobject result = AssociativeContainerTake<QMultiHash, K, T>::function(env, container, key);
        if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            Super::removeRC(env, container.object, key);
        }else if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                Super::updateRC(env, container);
            }else{
                Super::removeRC(env, container.object, key);
            }
        }else if constexpr((ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                Super::updateRC(env, container);
            }else{
                Super::removeRC(env, container.object, result);
            }
        }else if constexpr(ContainerContentType<K>::needsReferenceCounting || ContainerContentType<T>::needsReferenceCounting){
            Super::updateRC(env, container);
        }
        return result;
    }
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key, jobject _value) override {
        jint result;
        if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            result = AssociativeContainerRemovePair<QMultiHash, K, T>::function(env, container, key, _value);
            Super::removeRC(env, container.object, key, _value, result);
        }else if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                result = AssociativeContainerRemovePair<QMultiHash, K, T>::function(env, container, key, _value);
                if(result>0)
                    Super::updateRC(env, container);
            }else{
                result = AssociativeContainerRemovePair<QMultiHash, K, T>::function(env, container, key, _value);
                if(result>0){
                    Super::removeRC(env, container.object, key);
                }
            }
        }else if constexpr((ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                result = AssociativeContainerRemovePair<QMultiHash, K, T>::function(env, container, key, _value);

            }else{
                result = AssociativeContainerRemovePair<QMultiHash, K, T>::function(env, container, key, _value);
                Super::removeRC(env, container.object, _value, result);
            }
        }else if constexpr(ContainerContentType<K>::needsReferenceCounting || ContainerContentType<T>::needsReferenceCounting){
            result = AssociativeContainerRemovePair<QMultiHash, K, T>::function(env, container, key, _value);
            if(result>0)
                Super::updateRC(env, container);
        }else{
            result = AssociativeContainerRemovePair<QMultiHash, K, T>::function(env, container, key, _value);
        }
        return result;
    }
    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject _value) override {
        if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0
                      && (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            AssociativeContainerReplacePair<QMultiHash, K, T>::function(env, container, key, _value);
            Super::removeRC(env, container.object, key);
            Super::putRC(env, container.object, key, _value);
        }else if constexpr((ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                AssociativeContainerReplacePair<QMultiHash, K, T>::function(env, container, key, _value);
                Super::updateRC(env, container);
            }else{
                AssociativeContainerReplacePair<QMultiHash, K, T>::function(env, container, key, _value);
                Super::addRC(env, container.object, key);
            }
        }else if constexpr((ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0){
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                AssociativeContainerReplacePair<QMultiHash, K, T>::function(env, container, key, _value);

            }else{
                jobject oldValue = value(env, container.container, key, nullptr);
                AssociativeContainerReplacePair<QMultiHash, K, T>::function(env, container, key, _value);
                Super::removeRC(env, container.object, oldValue);
                Super::addRC(env, container.object, _value);
            }
        }else if constexpr(ContainerContentType<K>::needsReferenceCounting || ContainerContentType<T>::needsReferenceCounting){
            AssociativeContainerReplacePair<QMultiHash, K, T>::function(env, container, key, _value);
            Super::updateRC(env, container);
        }else{
            AssociativeContainerReplacePair<QMultiHash, K, T>::function(env, container, key, _value);
        }
    }
    void unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other) override {
        AssociativeContainerUnite<QMultiHash, K, T, ContainerAPI::getAsQMultiHash>::function(env, container, other);
        if constexpr(ContainerContentType<QMultiHash<K,T>>::needsReferenceCounting){
            Super::updateRC(env, container);
        }
    }
private:
    class KeyValueIterator : public AbstractHashAccess::KeyValueIterator{
    public:
        QMultiHashAccess* access;
        typename QMultiHash<K,T>::ConstIterator current;
        typename QMultiHash<K,T>::ConstIterator end;
        KeyValueIterator(QMultiHashAccess* _access, const void* container)
            : access(_access){
            current = reinterpret_cast<const QMultiHash<K,T>*>(container)->constBegin();
            end = reinterpret_cast<const QMultiHash<K,T>*>(container)->constEnd();
        }
        bool hasNext() override {return current!=end;};
            QPair<jobject,jobject> next(JNIEnv * env) override {
            QPair<jobject,jobject> result{qtjambi_scoped_cast<false,jobject,decltype(current.key())>::cast(env, current.key(), nullptr, nullptr),
                                           qtjambi_scoped_cast<false,jobject,decltype(current.value())>::cast(env, current.value(), nullptr, nullptr)};
            ++current;
            return result;
        }
        QPair<const void*,const void*> next() override {
            QPair<const void*,const void*> result{ContainerContentDeref<K>::deref(current.key()), ContainerContentDeref<T>::deref(current.value())};
            ++current;
            return result;
        }
    };
public:
    std::unique_ptr<AbstractHashAccess::KeyValueIterator> keyValueIterator(const void* container) override {
        return std::unique_ptr<AbstractHashAccess::KeyValueIterator>(new KeyValueIterator(this, container));
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
        static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(K));
        return type;
    }
    const QMetaType& secondMetaType() override {
        static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(T));
        return type;
    }

    AbstractContainerAccess::DataType firstType() override{
        return ContainerContentType<K>::type;
    }

    AbstractContainerAccess::DataType secondType() override{
        return ContainerContentType<T>::type;
    }

    AbstractContainerAccess* firstNestedContainerAccess() override {
        return ContainerContentType<K>::accessFactory();
    }

    AbstractContainerAccess* secondNestedContainerAccess() override {
        return ContainerContentType<T>::accessFactory();
    }

    bool hasFirstNestedContainerAccess() override {
        return ContainerContentType<K>::isContainer;
    }
    bool hasSecondNestedContainerAccess() override {
        return ContainerContentType<T>::isContainer;
    }
    bool hasFirstNestedPointers() override {
        return ContainerContentType<K>::isContainer && ContainerContentType<K>::needsReferenceCounting;
    }
    bool hasSecondNestedPointers() override {
        return ContainerContentType<T>::isContainer && ContainerContentType<T>::needsReferenceCounting;
    }

    void assign(void* container, const void* other) override {
        (*reinterpret_cast<QPair<K,T>*>(container)) = (*reinterpret_cast<const QPair<K,T>*>(other));
    }
    void assign(JNIEnv *, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override {
        (*reinterpret_cast<QPair<K,T>*>(container.container)) = (*reinterpret_cast<const QPair<K,T>*>(other.container));
    }
    size_t sizeOf() override {
        return sizeof(QPair<K,T>);
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* move) override {
        return new(placement) QPair<K,T>(std::move(*reinterpret_cast<const QPair<K,T>*>(move)));
    }
    void* constructContainer(JNIEnv *, void* placement, const ContainerAndAccessInfo& move) override {
        return constructContainer(placement, move.container);
    }
#endif
    bool destructContainer(void* container) override {
        reinterpret_cast<QPair<K,T>*>(container)->~QPair<K,T>();
        return true;
    }

    int registerContainer(const QByteArray& containerTypeName) override {
        return RegistryAPI::registerMetaType<QPair<K,T>>(containerTypeName, this);
    }

    jobject first(JNIEnv * env, const void* container) override {
        const QPair<K,T> *pair = static_cast<const QPair<K,T> *>(container);
        return qtjambi_scoped_cast<false,jobject,const K>::cast(env, pair->first, nullptr, nullptr);
    }

    jobject second(JNIEnv * env, const void* container) override {
        const QPair<K,T> *pair = static_cast<const QPair<K,T> *>(container);
        return qtjambi_scoped_cast<false,jobject,const T>::cast(env, pair->second, nullptr, nullptr);
    }

    void setFirst(JNIEnv * env, void* container, jobject first) override {
        QPair<K,T> *pair = static_cast<QPair<K,T> *>(container);
        pair->first = qtjambi_scoped_cast<false,K,jobject>::cast(env, first, nullptr, nullptr);
    }

    void setSecond(JNIEnv * env, void* container, jobject second) override {
        QPair<K,T> *pair = static_cast<QPair<K,T> *>(container);
        pair->second = qtjambi_scoped_cast<false,T,jobject>::cast(env, second, nullptr, nullptr);
    }
public:
    QPair<const void*,const void*> elements(const void* container) override {
        const QPair<K,T> *pair = static_cast<const QPair<K,T> *>(container);
        return {ContainerContentDeref<K>::deref(pair->first), ContainerContentDeref<T>::deref(pair->second)};
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

template<typename K, typename T>
struct ContainerContentType<QMap<K,T>>{
    static constexpr AbstractContainerAccess::DataType type = AbstractContainerAccess::Value;
    static constexpr bool isContainer = true;
    static constexpr bool needsReferenceCounting = (ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)
                                                  || (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)
                                                  || ContainerContentType<K>::needsReferenceCounting
                                                  || ContainerContentType<T>::needsReferenceCounting;
    static constexpr bool needsOwnerCheck = ContainerContentType<K>::needsOwnerCheck
                                            || ContainerContentType<T>::needsOwnerCheck;
    static constexpr AbstractContainerAccess* accessFactory(){return QMapAccess<K,T>::newInstance();};
};

template<typename K, typename T>
struct ContainerContentType<QHash<K,T>>{
    static constexpr AbstractContainerAccess::DataType type = AbstractContainerAccess::Value;
    static constexpr bool isContainer = true;
    static constexpr bool needsReferenceCounting = (ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)
                                                  || (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)
                                                  || ContainerContentType<K>::needsReferenceCounting
                                                  || ContainerContentType<T>::needsReferenceCounting;
    static constexpr bool needsOwnerCheck = ContainerContentType<K>::needsOwnerCheck
                                            || ContainerContentType<T>::needsOwnerCheck;
    static constexpr AbstractContainerAccess* accessFactory(){return QHashAccess<K,T>::newInstance();};
};

template<typename K, typename T>
struct ContainerContentType<QMultiMap<K,T>>{
    static constexpr AbstractContainerAccess::DataType type = AbstractContainerAccess::Value;
    static constexpr bool isContainer = true;
    static constexpr bool needsReferenceCounting = (ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)
                                                  || (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)
                                                  || ContainerContentType<K>::needsReferenceCounting
                                                  || ContainerContentType<T>::needsReferenceCounting;
    static constexpr bool needsOwnerCheck = ContainerContentType<K>::needsOwnerCheck
                                            || ContainerContentType<T>::needsOwnerCheck;
    static constexpr AbstractContainerAccess* accessFactory(){return QMultiMapAccess<K,T>::newInstance();};
};

template<typename K, typename T>
struct ContainerContentType<QMultiHash<K,T>>{
    static constexpr AbstractContainerAccess::DataType type = AbstractContainerAccess::Value;
    static constexpr bool isContainer = true;
    static constexpr bool needsReferenceCounting = (ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)
                                                  || (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)
                                                  || ContainerContentType<K>::needsReferenceCounting
                                                  || ContainerContentType<T>::needsReferenceCounting;
    static constexpr bool needsOwnerCheck = ContainerContentType<K>::needsOwnerCheck
                                            || ContainerContentType<T>::needsOwnerCheck;
    static constexpr AbstractContainerAccess* accessFactory(){return QMultiHashAccess<K,T>::newInstance();};
};

template<typename K, typename T>
struct ContainerContentType<QPair<K,T>>{
    static constexpr AbstractContainerAccess::DataType type = AbstractContainerAccess::Value;
    static constexpr bool isContainer = true;
    static constexpr bool needsReferenceCounting = (ContainerContentType<K>::type & AbstractContainerAccess::PointersMask)
                                                   || (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)
                                                   || ContainerContentType<K>::needsReferenceCounting
                                                   || ContainerContentType<T>::needsReferenceCounting;
    static constexpr bool needsOwnerCheck = ContainerContentType<K>::needsOwnerCheck
                                            || ContainerContentType<T>::needsOwnerCheck;
    static constexpr AbstractContainerAccess* accessFactory(){return QPairAccess<K,T>::newInstance();};
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_IMPL_CONTAINER_ASSOCIATIVE_H
