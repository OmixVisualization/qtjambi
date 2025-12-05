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


#ifndef CONTAINERACCESS_H
#define CONTAINERACCESS_H

#include <QtCore/QList>
#include <QtCore/QTypeInfo>
#include <QtCore/QDebug>
#include "qtjambiapi.h"
#include "typetests.h"
#include "containerapi.h"
#include "registryapi.h"
#include "qtjambi_cast_impl_container_iterator.h"
#include "qtjambi_cast_impl_container_sequential.h"

#if defined(QTJAMBI_GENERIC_ACCESS)
#define ACCESS_EXPORT QTJAMBI_EXPORT
#else
#define ACCESS_EXPORT
#endif

namespace ContainerAccessAPI{
ACCESS_EXPORT AbstractContainerAccess* createContainerAccess(JNIEnv* env, SequentialContainerType containerType,
                                               const QMetaType& metaType,
                                               size_t align, size_t size,
                                               bool isPointer,
                                               const QtJambiUtils::QHashFunction& hashFunction,
                                               const QtJambiUtils::InternalToExternalConverter& memberConverter,
                                               const QtJambiUtils::ExternalToInternalConverter& memberReConverter,
                                               const QSharedPointer<AbstractContainerAccess>& memberNestedContainerAccess,
                                               PtrOwnerFunction ownerFunction);
ACCESS_EXPORT AbstractContainerAccess* createContainerAccess(JNIEnv* env, AssociativeContainerType mapType,
                                               const QMetaType& memberMetaType1,
                                               size_t align1, size_t size1,
                                               bool isPointer1,
                                               const QtJambiUtils::QHashFunction& hashFunction1,
                                               const QtJambiUtils::InternalToExternalConverter& memberConverter1,
                                               const QtJambiUtils::ExternalToInternalConverter& memberReConverter1,
                                               const QSharedPointer<AbstractContainerAccess>& memberNestedContainerAccess1,
                                               PtrOwnerFunction ownerFunction1,
                                               const QMetaType& memberMetaType2,
                                               size_t align2, size_t size2,
                                               bool isPointer2,
                                               const QtJambiUtils::QHashFunction& hashFunction2,
                                               const QtJambiUtils::InternalToExternalConverter& memberConverter2,
                                               const QtJambiUtils::ExternalToInternalConverter& memberReConverter2,
                                               const QSharedPointer<AbstractContainerAccess>& memberNestedContainerAccess2,
                                               PtrOwnerFunction ownerFunction2);
ACCESS_EXPORT AbstractContainerAccess* createContainerAccess(SequentialContainerType containerType, const QMetaType& memberMetaType);
ACCESS_EXPORT AbstractContainerAccess* createContainerAccess(AssociativeContainerType mapType,
                                               const QMetaType& memberMetaType1,
                                               const QMetaType& memberMetaType2);
ACCESS_EXPORT size_t pointerHashFunction(const void* ptr, size_t seed);
}//namespace ContainerAccessAPI

class AbstractNestedSequentialAccess{
protected:
    AbstractNestedSequentialAccess() = default;
    ACCESS_EXPORT virtual ~AbstractNestedSequentialAccess();
    Q_DISABLE_COPY_MOVE(AbstractNestedSequentialAccess)
public:
    virtual const QSharedPointer<AbstractContainerAccess>& sharedElementNestedContainerAccess() = 0;
};

class AbstractNestedAssociativeAccess{
protected:
    AbstractNestedAssociativeAccess() = default;
    ACCESS_EXPORT virtual ~AbstractNestedAssociativeAccess();
    Q_DISABLE_COPY_MOVE(AbstractNestedAssociativeAccess)
public:
    virtual const QSharedPointer<AbstractContainerAccess>& sharedKeyNestedContainerAccess() = 0;
    virtual const QSharedPointer<AbstractContainerAccess>& sharedValueNestedContainerAccess() = 0;
};

class AbstractNestedPairAccess{
protected:
    AbstractNestedPairAccess() = default;
    ACCESS_EXPORT virtual ~AbstractNestedPairAccess();
    Q_DISABLE_COPY_MOVE(AbstractNestedPairAccess)
public:
    virtual const QSharedPointer<AbstractContainerAccess>& sharedFirstNestedContainerAccess() = 0;
    virtual const QSharedPointer<AbstractContainerAccess>& sharedSecondNestedContainerAccess() = 0;
};

#if defined(QTJAMBI_GENERIC_ACCESS)

namespace ContainerAccessAPI{

template<template<typename> class Container>
struct SequentialContainerAnalyzer{
};

template<>
struct SequentialContainerAnalyzer<QList>{
    static constexpr SequentialContainerType type = SequentialContainerType::QList;
};

template<>
struct SequentialContainerAnalyzer<QStack>{
    static constexpr SequentialContainerType type = SequentialContainerType::QStack;
};

template<>
struct SequentialContainerAnalyzer<QQueue>{
    static constexpr SequentialContainerType type = SequentialContainerType::QQueue;
};

template<>
struct SequentialContainerAnalyzer<QSet>{
    static constexpr SequentialContainerType type = SequentialContainerType::QSet;
};

template<template<typename,typename> class Container>
struct AssociativeContainerAnalyzer{
};

template<>
struct AssociativeContainerAnalyzer<QMap>{
    static constexpr AssociativeContainerType type = AssociativeContainerType::QMap;
};

template<>
struct AssociativeContainerAnalyzer<QHash>{
    static constexpr AssociativeContainerType type = AssociativeContainerType::QHash;
};

template<>
struct AssociativeContainerAnalyzer<QMultiMap>{
    static constexpr AssociativeContainerType type = AssociativeContainerType::QMultiMap;
};

template<>
struct AssociativeContainerAnalyzer<QMultiHash>{
    static constexpr AssociativeContainerType type = AssociativeContainerType::QMultiHash;
};

template<>
struct AssociativeContainerAnalyzer<QPair>{
    static constexpr AssociativeContainerType type = AssociativeContainerType::QPair;
};

ACCESS_EXPORT QDebug containerElementDebugStream(QDebug debug, uint i, const void* ptr);
ACCESS_EXPORT QDataStream & containerElementDataStreamIn(QDataStream & stream, uint i, void* ptr);
ACCESS_EXPORT QDataStream & containerElementDataStreamOut(QDataStream & stream, uint i, const void* ptr);
ACCESS_EXPORT size_t containerElementHash(uint i, const void* ptr, size_t seed);
ACCESS_EXPORT bool containerElementEqual(uint i, const void* ptr, const void* ptr2);
ACCESS_EXPORT bool containerElementLess(uint i, const void* ptr, const void* ptr2);
ACCESS_EXPORT void constructContainerElement(uint i, void* ptr, const void* copy = nullptr);
ACCESS_EXPORT void destructContainerElement(uint i, void* ptr);

template<int index, bool isPointer>
class MetaTypeInfo{
    QMetaType m_metaType;
    QtJambiUtils::QHashFunction m_hashFunction;
public:
    MetaTypeInfo(const QMetaType& metaType,
                 const QtJambiUtils::QHashFunction& hashFunction
            ) : m_metaType(metaType),
                m_hashFunction(hashFunction)
    {}

    MetaTypeInfo(const MetaTypeInfo<index,isPointer>& other)
        : m_metaType(other.m_metaType),
          m_hashFunction(other.m_hashFunction)
    {}

    const QMetaType& metaType() const {
        return m_metaType;
    }

    const QtJambiUtils::QHashFunction& hashFunction() const {
        return m_hashFunction;
    }

    template<int, bool>
    friend struct MetaTypeInfoLocker;
};

template<int index>
class MetaTypeInfo<index,true>{
    QMetaType m_metaType;
public:
    MetaTypeInfo(const QMetaType& metaType,
                 const QtJambiUtils::QHashFunction&
            ) : m_metaType(metaType){}
    MetaTypeInfo(const MetaTypeInfo<index,true>& other) : m_metaType(other.m_metaType){}

    const QMetaType& metaType() const {
        return m_metaType;
    }

    QtJambiUtils::QHashFunction hashFunction() const {
        return &pointerHashFunction;
    }
};

struct ACCESS_EXPORT AbstractMetaTypeInfoLocker{
protected:
    AbstractMetaTypeInfoLocker(int index, const QMetaType& _metaType, const QtJambiUtils::QHashFunction& _hashFunction);
    ~AbstractMetaTypeInfoLocker();
private:
    int m_index;
    QMetaType m_metaType;
    QtJambiUtils::QHashFunction m_hashFunction;
};

template<int index, bool isPointer>
struct MetaTypeInfoLocker : AbstractMetaTypeInfoLocker{
    MetaTypeInfoLocker(const MetaTypeInfo<index,isPointer>& info) :
        AbstractMetaTypeInfoLocker(index, info.m_metaType, info.m_hashFunction){}
};

template<int index>
struct MetaTypeInfoLocker<index,true>{
    MetaTypeInfoLocker(const MetaTypeInfo<index,true>&){}
};

#define QTJAMBI_ELEMENT_LOCKER(_this) ContainerAccessAPI::MetaTypeInfoLocker<0,_size==0> __elementLocker(_this->m_elementMetaTypeInfo)

#define QTJAMBI_KEY_VALUE_LOCKER(_this) ContainerAccessAPI::MetaTypeInfoLocker<0,size1==0> __keyLocker(_this->m_keyMetaTypeInfo);\
    ContainerAccessAPI::MetaTypeInfoLocker<1,size2==0> __valueLocker(_this->m_valueMetaTypeInfo)

template<size_t size, uint i, bool isStatic, size_t align = 0, bool useAlignment = (align>0) >
struct ContainerElement{
    bool operator==(const ContainerElement& value2) const{
        return containerElementEqual(i, this, &value2);
    }
    bool operator<(const ContainerElement& value2) const{
        return containerElementLess(i, this, &value2);
    }
    friend QDebug operator<<(QDebug debug, const ContainerElement& value){
        return ContainerAccessAPI::containerElementDebugStream(debug, i, &value);
    }

    friend QDataStream & operator<<(QDataStream & s, const ContainerElement& value){
        return ContainerAccessAPI::containerElementDataStreamOut(s, i, &value);
    }

    friend QDataStream & operator>>(QDataStream & s, ContainerElement& value){
        return ContainerAccessAPI::containerElementDataStreamIn(s, i, &value);
    }

    friend size_t qHash(const ContainerElement& value, size_t seed = 0){
        return ContainerAccessAPI::containerElementHash(i, &value, seed);
    }
};

template<size_t size, uint i, bool isStatic>
struct ContainerElement<size, i, isStatic, 0, false>{
    ContainerElement(std::nullptr_t = nullptr){
        constructContainerElement(i, this);
    }
    ContainerElement(const ContainerElement& copy){
        constructContainerElement(i, this, &copy);
    }
    ~ContainerElement(){
        destructContainerElement(i, this);
    }
    ContainerElement& operator=(const ContainerElement& other){
        destructContainerElement(i, this);
        constructContainerElement(i, this, &other);
        return *this;
    }
    bool operator==(const ContainerElement& value2) const{
        return containerElementEqual(i, this, &value2);
    }
    bool operator<(const ContainerElement& value2) const{
        return containerElementLess(i, this, &value2);
    }
    friend QDebug operator<<(QDebug debug, const ContainerElement& value){
        return ContainerAccessAPI::containerElementDebugStream(debug, i, &value);
    }

    friend QDataStream & operator<<(QDataStream & s, const ContainerElement& value){
        return ContainerAccessAPI::containerElementDataStreamOut(s, i, &value);
    }

    friend QDataStream & operator>>(QDataStream & s, ContainerElement& value){
        return ContainerAccessAPI::containerElementDataStreamIn(s, i, &value);
    }

    friend size_t qHash(const ContainerElement& value, size_t seed = 0){
        return ContainerAccessAPI::containerElementHash(i, &value, seed);
    }
private:
    quint8 data[size];
};

template<uint i, bool isStatic>
struct ContainerElement<1, i, isStatic, 0, false>{
    ContainerElement(std::nullptr_t = nullptr){
        constructContainerElement(i, this);
    }
    ContainerElement(const ContainerElement& copy){
        constructContainerElement(i, this, &copy);
    }
    ~ContainerElement(){
        destructContainerElement(i, this);
    }
    ContainerElement& operator=(const ContainerElement& other){
        destructContainerElement(i, this);
        constructContainerElement(i, this, &other);
        return *this;
    }
    bool operator==(const ContainerElement& value2) const{
        return containerElementEqual(i, this, &value2);
    }
    bool operator<(const ContainerElement& value2) const{
        return containerElementLess(i, this, &value2);
    }
    friend QDebug operator<<(QDebug debug, const ContainerElement& value){
        return ContainerAccessAPI::containerElementDebugStream(debug, i, &value);
    }

    friend QDataStream & operator<<(QDataStream & s, const ContainerElement& value){
        return ContainerAccessAPI::containerElementDataStreamOut(s, i, &value);
    }

    friend QDataStream & operator>>(QDataStream & s, ContainerElement& value){
        return ContainerAccessAPI::containerElementDataStreamIn(s, i, &value);
    }

    friend size_t qHash(const ContainerElement& value, size_t seed = 0){
        return ContainerAccessAPI::containerElementHash(i, &value, seed);
    }
private:
    quint8 data;
};

template<size_t size, uint i, bool isStatic, size_t align>
struct Q_DECL_ALIGN(align) ContainerElement<size, i, isStatic, align, true>{
    ContainerElement(std::nullptr_t = nullptr){
        constructContainerElement(i, this);
    }
    ContainerElement(const ContainerElement& copy){
        constructContainerElement(i, this, &copy);
    }
    ~ContainerElement(){
        destructContainerElement(i, this);
    }
    ContainerElement& operator=(const ContainerElement& other){
        destructContainerElement(i, this);
        constructContainerElement(i, this, &other);
        return *this;
    }
    bool operator==(const ContainerElement& value2) const{
        return containerElementEqual(i, this, &value2);
    }
    bool operator<(const ContainerElement& value2) const{
        return containerElementLess(i, this, &value2);
    }
    friend QDebug operator<<(QDebug debug, const ContainerElement& value){
        return ContainerAccessAPI::containerElementDebugStream(debug, i, &value);
    }

    friend QDataStream & operator<<(QDataStream & s, const ContainerElement& value){
        return ContainerAccessAPI::containerElementDataStreamOut(s, i, &value);
    }

    friend QDataStream & operator>>(QDataStream & s, ContainerElement& value){
        return ContainerAccessAPI::containerElementDataStreamIn(s, i, &value);
    }

    friend size_t qHash(const ContainerElement& value, size_t seed = 0){
        return ContainerAccessAPI::containerElementHash(i, &value, seed);
    }
private:
    quint8 data[size];
};

template<uint i, bool isStatic, size_t align>
struct Q_DECL_ALIGN(align) ContainerElement<1, i, isStatic, align, true>{
    ContainerElement(std::nullptr_t = nullptr){
        constructContainerElement(i, this);
    }
    ContainerElement(const ContainerElement& copy){
        constructContainerElement(i, this, &copy);
    }
    ~ContainerElement(){
        destructContainerElement(i, this);
    }
    ContainerElement& operator=(const ContainerElement& other){
        destructContainerElement(i, this);
        constructContainerElement(i, this, &other);
        return *this;
    }
    bool operator==(const ContainerElement& value2) const{
        return containerElementEqual(i, this, &value2);
    }
    bool operator<(const ContainerElement& value2) const{
        return containerElementLess(i, this, &value2);
    }
    friend QDebug operator<<(QDebug debug, const ContainerElement& value){
        return ContainerAccessAPI::containerElementDebugStream(debug, i, &value);
    }

    friend QDataStream & operator<<(QDataStream & s, const ContainerElement& value){
        return ContainerAccessAPI::containerElementDataStreamOut(s, i, &value);
    }

    friend QDataStream & operator>>(QDataStream & s, ContainerElement& value){
        return ContainerAccessAPI::containerElementDataStreamIn(s, i, &value);
    }

    friend size_t qHash(const ContainerElement& value, size_t seed = 0){
        return ContainerAccessAPI::containerElementHash(i, &value, seed);
    }
private:
    quint8 data;
};

template<size_t size, uint i, bool isStatic, size_t align, bool useAlignment>
size_t qHash(const ContainerElement<size, i, isStatic, align, useAlignment>& value, size_t seed = 0){
    return containerElementHash(i, &value, seed);
}

template<typename T, bool canLess = QtJambiPrivate::supports_less_than<T>::value>
struct TryLess{
    typedef jboolean (*LessThanFunction)(const T&, const T&);

    static LessThanFunction function(JNIEnv *){
        return [](const T& a, const T& b)->jboolean{return a < b;};
    }
};

template<typename T>
struct TryLess<T,false>{
    typedef jboolean (*LessThanFunction)(const T&, const T&);

    static LessThanFunction function(JNIEnv * env){
        JavaException::raiseQNoImplementationException(env, "operator <" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename> class Container, size_t _align, size_t _size, bool _isStatic, bool isConst = true>
class SequentialConstIteratorAccess : public AbstractSequentialConstIteratorAccess{
    typedef typename std::conditional<_size==0, void*, ContainerElement<_size, 0, _isStatic, _align>>::type T;
    typedef typename std::conditional<isConst, typename Container<T>::const_iterator, typename Container<T>::iterator>::type Iterator;
protected:
    SequentialConstIteratorAccess(const SequentialConstIteratorAccess<Container, _align, _size, _isStatic, isConst>& other)
        : AbstractSequentialConstIteratorAccess(),
        m_elementMetaTypeInfo(other.m_elementMetaTypeInfo),
        m_internalToExternalConverter(other.m_internalToExternalConverter){}
public:
    SequentialConstIteratorAccess(MetaTypeInfo<0,_size==0> elementMetaTypeInfo, const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter)
        : AbstractSequentialConstIteratorAccess(),
        m_elementMetaTypeInfo(elementMetaTypeInfo),
        m_internalToExternalConverter(internalToExternalConverter){}

    AbstractSequentialConstIteratorAccess* clone() override{
        return new SequentialConstIteratorAccess<Container, _align, _size, _isStatic, isConst>(*this);
    }

    void dispose() override { delete this; }

    jobject value(JNIEnv * env, const void* iterator) override {
        const Iterator* iter = reinterpret_cast<const Iterator*>(iterator);
        jvalue _value;
        _value.l = nullptr;
        QTJAMBI_ELEMENT_LOCKER(this);
        bool success = m_internalToExternalConverter(env, nullptr, &(*(*iter)), _value, true);
        if(success){
            return _value.l;
        }
        return nullptr;
    }

    void increment(JNIEnv *env, void* iterator) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        QtJambiPrivate::IteratorIncrement<Iterator>::function(env, iterator);
    }
    void decrement(JNIEnv *env, void* iterator) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        QtJambiPrivate::IteratorDecrement<Iterator>::function(env, iterator);
    }
    jboolean lessThan(JNIEnv *env, const void* iterator, const void* other) override {
        auto lessFunction = TryLess<Iterator>::function(env);
        QTJAMBI_ELEMENT_LOCKER(this);
        const Iterator* iter = reinterpret_cast<const Iterator*>(iterator);
        const Iterator* iter2 = reinterpret_cast<const Iterator*>(other);
        return lessFunction(*iter, *iter2);
    }
    bool canLess() override {
        return std::is_pointer<Iterator>::value || QtJambiPrivate::supports_less_than<Iterator>::value;
    }
    jboolean equals(JNIEnv *, const void* iterator, const void* other) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        const Iterator* iter = reinterpret_cast<const Iterator*>(iterator);
        const Iterator* iter2 = reinterpret_cast<const Iterator*>(other);
        return (*iter) == (*iter2);
    }

    const QMetaType& valueMetaType() override{
        return m_elementMetaTypeInfo.metaType();
    }
protected:
    MetaTypeInfo<0,_size==0> m_elementMetaTypeInfo;
    QtJambiUtils::InternalToExternalConverter m_internalToExternalConverter;
};

template<template<typename> class Container, size_t _align, size_t _size, bool _isStatic>
class SequentialIteratorAccess : public virtual SequentialConstIteratorAccess<Container, _align, _size, _isStatic, false>, public virtual AbstractSequentialIteratorAccess{
    typedef typename std::conditional<_size==0, void*, ContainerElement<_size, 0, _isStatic, _align>>::type T;
    SequentialIteratorAccess(const SequentialIteratorAccess<Container, _align, _size, _isStatic>& other)
        : SequentialConstIteratorAccess<Container, _align, _size, _isStatic, false>(other),
        m_externalToInternalConverter(other.m_externalToInternalConverter){}
public:
    SequentialIteratorAccess(MetaTypeInfo<0,_size==0> elementMetaTypeInfo,
                             const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
                             const QtJambiUtils::ExternalToInternalConverter& externalToInternalConverter)
        : SequentialConstIteratorAccess<Container, _align, _size, _isStatic, false>(elementMetaTypeInfo, internalToExternalConverter),
        m_externalToInternalConverter(externalToInternalConverter){}

    SequentialConstIteratorAccess<Container, _align, _size, _isStatic, false>* clone() override{
        return new SequentialIteratorAccess<Container, _align, _size, _isStatic>(*this);
    }

    void dispose() override {
        delete this;
    }

    const QMetaType& valueMetaType() override{
        return SequentialConstIteratorAccess<Container, _align, _size, _isStatic, false>::valueMetaType();
    }

    jobject value(JNIEnv * env, const void* iterator) override {
        return SequentialConstIteratorAccess<Container, _align, _size, _isStatic, false>::value(env, iterator);
    }

    void increment(JNIEnv *env, void* iterator) override {
        SequentialConstIteratorAccess<Container, _align, _size, _isStatic, false>::increment(env, iterator);
    }

    void decrement(JNIEnv *env, void* iterator) override {
        SequentialConstIteratorAccess<Container, _align, _size, _isStatic, false>::decrement(env, iterator);
    }

    jboolean lessThan(JNIEnv *env, const void* iterator, const void* other) override {
        return SequentialConstIteratorAccess<Container, _align, _size, _isStatic, false>::lessThan(env, iterator, other);
    }

    bool canLess() override {
        return SequentialConstIteratorAccess<Container, _align, _size, _isStatic, false>::canLess();
    }

    jboolean equals(JNIEnv *env, const void* iterator, const void* other) override {
        return SequentialConstIteratorAccess<Container, _align, _size, _isStatic, false>::equals(env, iterator, other);
    }

    void setValue(JNIEnv * env, void* iterator, jobject newValue) override {
        typename Container<T>::iterator& iter = *reinterpret_cast<typename Container<T>::iterator*>(iterator);
        jvalue _value;
        _value.l = newValue;
        ContainerAccessAPI::MetaTypeInfoLocker<0,_size==0> __elementLocker(SequentialConstIteratorAccess<Container, _align, _size, _isStatic, false>::m_elementMetaTypeInfo);
        T& t = *iter;
        void* out = &t;
        m_externalToInternalConverter(env, nullptr, _value, out, jValueType::l);
    }
private:
    QtJambiUtils::ExternalToInternalConverter m_externalToInternalConverter;
};

template<template<typename, typename> class Container, size_t align1, size_t size1, size_t align2, size_t size2, bool isConst = true>
class AssociativeConstIteratorAccess : public AbstractAssociativeConstIteratorAccess{
    typedef typename std::conditional<size1==0, void*, ContainerElement<size1, 0, false, align1>>::type K;
    typedef typename std::conditional<size2==0, void*, ContainerElement<size2, 1, false, align2>>::type T;
    typedef typename std::conditional<isConst, typename Container<K,T>::const_iterator, typename Container<K,T>::iterator>::type Iterator;
protected:
    AssociativeConstIteratorAccess(const AssociativeConstIteratorAccess& other)
        : AbstractAssociativeConstIteratorAccess(),
        m_keyMetaTypeInfo(other.m_keyMetaTypeInfo),
        m_keyInternalToExternalConverter(other.m_keyInternalToExternalConverter),
        m_valueMetaTypeInfo(other.m_valueMetaTypeInfo),
        m_valueInternalToExternalConverter(other.m_valueInternalToExternalConverter){}

public:
    AssociativeConstIteratorAccess(MetaTypeInfo<0,size1==0> keyMetaTypeInfo,
                                   const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
                                   MetaTypeInfo<1,size2==0> valueMetaTypeInfo,
                                   const QtJambiUtils::InternalToExternalConverter& valueInternalToExternalConverter)
        : AbstractAssociativeConstIteratorAccess(),
        m_keyMetaTypeInfo(keyMetaTypeInfo),
        m_keyInternalToExternalConverter(keyInternalToExternalConverter),
        m_valueMetaTypeInfo(valueMetaTypeInfo),
        m_valueInternalToExternalConverter(valueInternalToExternalConverter){}

    AbstractSequentialConstIteratorAccess* clone() override{
        return new AssociativeConstIteratorAccess<Container, align1, size1, align2, size2, isConst>(*this);
    }

    void dispose() override { delete this; }

    jobject value(JNIEnv * env, const void* iterator) override {
        const Iterator* iter = reinterpret_cast<const Iterator*>(iterator);
        jvalue _value;
        _value.l = nullptr;
        QTJAMBI_KEY_VALUE_LOCKER(this);
        bool success = m_valueInternalToExternalConverter(env, nullptr, &(iter->value()), _value, true);
        if(success){
            return _value.l;
        }
        return nullptr;
    }

    jobject key(JNIEnv * env, const void* iterator) override {
        const Iterator* iter = reinterpret_cast<const Iterator*>(iterator);
        jvalue _value;
        _value.l = nullptr;
        QTJAMBI_KEY_VALUE_LOCKER(this);
        bool success = m_keyInternalToExternalConverter(env, nullptr, &(iter->key()), _value, true);
        if(success){
            return _value.l;
        }
        return nullptr;
    }

    void increment(JNIEnv *env, void* iterator) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        QtJambiPrivate::IteratorIncrement<Iterator>::function(env, iterator);
    }
    void decrement(JNIEnv *env, void* iterator) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        QtJambiPrivate::IteratorDecrement<Iterator>::function(env, iterator);
    }
    jboolean lessThan(JNIEnv *env, const void* iterator, const void* other) override {
        auto lessFunction = TryLess<Iterator>::function(env);
        QTJAMBI_KEY_VALUE_LOCKER(this);
        const Iterator* iter = reinterpret_cast<const Iterator*>(iterator);
        const Iterator* iter2 = reinterpret_cast<const Iterator*>(other);
        return lessFunction(*iter, *iter2);
    }
    bool canLess() override {
        return std::is_pointer<Iterator>::value || QtJambiPrivate::supports_less_than<Iterator>::value;
    }
    jboolean equals(JNIEnv *, const void* iterator, const void* other) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        const Iterator* iter = reinterpret_cast<const Iterator*>(iterator);
        const Iterator* iter2 = reinterpret_cast<const Iterator*>(other);
        return (*iter) == (*iter2);
    }

    const QMetaType& keyMetaType() override{
        return m_keyMetaTypeInfo.metaType();
    }

    const QMetaType& valueMetaType() override{
        return m_valueMetaTypeInfo.metaType();
    }
protected:
    MetaTypeInfo<0,size1==0> m_keyMetaTypeInfo;
    QtJambiUtils::InternalToExternalConverter m_keyInternalToExternalConverter;
    MetaTypeInfo<1,size2==0> m_valueMetaTypeInfo;
    QtJambiUtils::InternalToExternalConverter m_valueInternalToExternalConverter;
};

template<template<typename, typename> class Container, size_t align1, size_t size1, size_t align2, size_t size2>
class AssociativeIteratorAccess : public virtual AssociativeConstIteratorAccess<Container, align1, size1, align2, size2, false>, public virtual AbstractAssociativeIteratorAccess{
    typedef typename std::conditional<size1==0, void*, ContainerElement<size1, 0, false, align1>>::type K;
    typedef typename std::conditional<size2==0, void*, ContainerElement<size2, 1, false, align2>>::type T;
    AssociativeIteratorAccess(const AssociativeIteratorAccess<Container, align1, size1, align2, size2>& other)
        : AssociativeConstIteratorAccess<Container, align1, size1, align2, size2, false>(other),
        m_valueExternalToInternalConverter(other.m_valueExternalToInternalConverter){}
public:
    AssociativeIteratorAccess(MetaTypeInfo<0,size1==0> keyMetaTypeInfo,
                              const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
                              MetaTypeInfo<1,size2==0> valueMetaTypeInfo,
                              const QtJambiUtils::InternalToExternalConverter& valueInternalToExternalConverter,
                              const QtJambiUtils::ExternalToInternalConverter& valueExternalToInternalConverter)
        : AssociativeConstIteratorAccess<Container, align1, size1, align2, size2, false>(
            keyMetaTypeInfo, keyInternalToExternalConverter,
            valueMetaTypeInfo, valueInternalToExternalConverter),
        m_valueExternalToInternalConverter(valueExternalToInternalConverter){}

    AssociativeConstIteratorAccess<Container, align1, size1, align2, size2, false>* clone() override{
        return new AssociativeIteratorAccess<Container, align1, size1, align2, size2>(*this);
    }

    void dispose() override {
        delete this;
    }

    const QMetaType& keyMetaType() override{
        return AssociativeConstIteratorAccess<Container, align1, size1, align2, size2, false>::keyMetaType();
    }

    const QMetaType& valueMetaType() override{
        return AssociativeConstIteratorAccess<Container, align1, size1, align2, size2, false>::valueMetaType();
    }

    jobject value(JNIEnv * env, const void* iterator) override {
        return AssociativeConstIteratorAccess<Container, align1, size1, align2, size2, false>::value(env, iterator);
    }

    jobject key(JNIEnv * env, const void* iterator) override {
        return AssociativeConstIteratorAccess<Container, align1, size1, align2, size2, false>::key(env, iterator);
    }

    void increment(JNIEnv *env, void* iterator) override {
        AssociativeConstIteratorAccess<Container, align1, size1, align2, size2, false>::increment(env, iterator);
    }

    void decrement(JNIEnv *env, void* iterator) override {
        AssociativeConstIteratorAccess<Container, align1, size1, align2, size2, false>::decrement(env, iterator);
    }

    jboolean lessThan(JNIEnv *env, const void* iterator, const void* other) override {
        return AssociativeConstIteratorAccess<Container, align1, size1, align2, size2, false>::lessThan(env, iterator, other);
    }

    bool canLess() override {
        return AssociativeConstIteratorAccess<Container, align1, size1, align2, size2, false>::canLess();
    }

    jboolean equals(JNIEnv *env, const void* iterator, const void* other) override {
        return AssociativeConstIteratorAccess<Container, align1, size1, align2, size2, false>::equals(env, iterator, other);
    }

    void setValue(JNIEnv * env, void* iterator, jobject newValue) override {
        typename Container<K,T>::iterator* iter = reinterpret_cast<typename Container<K,T>::iterator*>(iterator);
        jvalue _value;
        _value.l = newValue;
        ContainerAccessAPI::MetaTypeInfoLocker<0,size1==0> __keyLocker(AssociativeConstIteratorAccess<Container, align1, size1, align2, size2, false>::m_keyMetaTypeInfo);
        ContainerAccessAPI::MetaTypeInfoLocker<1,size2==0> __valueLocker(AssociativeConstIteratorAccess<Container, align1, size1, align2, size2, false>::m_valueMetaTypeInfo);
        T& t = iter->value();
        void* out = &t;
        m_valueExternalToInternalConverter(env, nullptr, _value, out, jValueType::l);
    }
private:
    QtJambiUtils::ExternalToInternalConverter m_valueExternalToInternalConverter;
};



template<template <typename> class Container, size_t align, size_t size, bool isStatic>
struct SequentialContainerAccessFac{
};

template<template <typename,typename> class Container, size_t align1, size_t size1, size_t align2, size_t size2>
struct AssociativeContainerAccessFac{
};

typedef AbstractContainerAccess*(*SequentialContainerAccessFactory)(const QMetaType& metaType,
                                                                     const QtJambiUtils::QHashFunction& hashFunction,
                                                                     const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
                                                                     const QtJambiUtils::ExternalToInternalConverter& externalToInternalConverter,
                                                                     const QSharedPointer<AbstractContainerAccess>& elementNestedContainerAccess,
                                                                     PtrOwnerFunction elementOwnerFunction);

typedef AbstractContainerAccess*(*AssociativeContainerAccessFactory)(const QMetaType& keyMetaType,
                                                                      const QtJambiUtils::QHashFunction& keyHashFunction,
                                                                      const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
                                                                      const QtJambiUtils::ExternalToInternalConverter& keyExternalToInternalConverter,
                                                                      const QSharedPointer<AbstractContainerAccess>& keyNestedContainerAccess,
                                                                      PtrOwnerFunction keyOwnerFunction,
                                                                      const QMetaType& valueMetaType,
                                                                      const QtJambiUtils::QHashFunction& valueHashFunction,
                                                                      const QtJambiUtils::InternalToExternalConverter& valueInternalToExternalConverter,
                                                                      const QtJambiUtils::ExternalToInternalConverter& valueExternalToInternalConverter,
                                                                      const QSharedPointer<AbstractContainerAccess>& valueNestedContainerAccess,
                                                                      PtrOwnerFunction valueOwnerFunction);

ACCESS_EXPORT void registerAccessFactory(SequentialContainerType containerType, size_t align, size_t size, bool isStatic, SequentialContainerAccessFactory factory);
ACCESS_EXPORT void registerAccessFactory(AssociativeContainerType containerType, size_t align1, size_t size1, size_t align2, size_t size2, AssociativeContainerAccessFactory factory);

template<template<typename> class Container, size_t align, size_t size, bool isStatic, bool = (align<=size && align <= alignof(std::max_align_t) && (!isStatic || size <= sizeof(void*))) >
struct SequentialContainerAccessFactoryHelper{
    static void registerContainerAccessFactory(){
        registerAccessFactory(SequentialContainerAnalyzer<Container>::type, align, size, isStatic, SequentialContainerAccessFac<Container, align, size, isStatic>::factory());
    }
};

template<template<typename> class Container, size_t align, size_t size, bool isStatic>
struct SequentialContainerAccessFactoryHelper<Container, align, size, isStatic, false>{
    static void registerContainerAccessFactory(){
    }
};

template<template<typename,typename> class Container, size_t align1, size_t size1, size_t align2, size_t size2, bool = (align1<=size1 && align2<=size2 && align1 <= alignof(std::max_align_t) && align2 <= alignof(std::max_align_t)) >
struct AssociativeContainerAccessFactoryHelper{
    static void registerContainerAccessFactory(){
        registerAccessFactory(AssociativeContainerAnalyzer<Container>::type, align1, size1, align2, size2, AssociativeContainerAccessFac<Container, align1, size1, align2, size2>::factory());
    }
};

template<template<typename,typename> class Container, size_t align1, size_t size1, size_t align2, size_t size2>
struct AssociativeContainerAccessFactoryHelper<Container, align1, size1, align2, size2, false>{
    static void registerContainerAccessFactory(){
    }
};

}//namespace ContainerAccessAPI

template<size_t sz, uint i, bool isStatic, size_t algn, bool useAlignment>
struct std::hash<ContainerAccessAPI::ContainerElement<sz, i, isStatic, algn, useAlignment>>{
    size_t operator()(const ContainerAccessAPI::ContainerElement<sz, i, isStatic, algn, useAlignment>& element) const noexcept {
        return ::qHash(element);
    }
};

#endif //defined(QTJAMBI_GENERIC_ACCESS)

#endif // CONTAINERACCESS_H
