/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
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

#ifndef ITERATORS_P_H
#define ITERATORS_P_H

#include <QtJambi/ContainerAPI>
#include <QtCore/QCborMap>
#include <QtCore/QCborArray>

template<typename,typename,typename>
struct ContainerIterator;

template<typename,typename>
class QSequentialIteratorAccess;

namespace QtJambiPrivate{
void adapt(QCborMap::Iterator& iterator, const QCborMap& map);

template<typename Container, bool>
struct ContainerSharedInfo;

template<>
struct ContainerSharedInfo<QCborMap,false>{
    static constexpr bool is_shared = true;
    using Fn = bool(&)(const QCborMap&,const QCborMap&);
    static bool isSharedWith(const QCborMap& container, const QCborMap& other);
};

template<>
struct ContainerSharedInfo<QCborArray,false>{
    static constexpr bool is_shared = true;
    using Fn = bool(&)(const QCborArray&,const QCborArray&);
    static bool isSharedWith(const QCborArray& container, const QCborArray& other);
};

template<typename, typename>
struct is_writable_iterator;

template<>
struct is_writable_iterator<QCborMap::Iterator,std::pair<QCborValueConstRef, QCborValueRef>> : std::true_type{};

template<>
struct is_writable_iterator<QCborArray::Iterator,QCborValueRef> : std::true_type{};

template<>
struct is_writable_iterator<QJsonObject::Iterator,QJsonValue> : std::true_type{};

template<>
struct is_writable_iterator<QJsonObject::Iterator,QJsonValueRef> : std::true_type{};

template<>
struct is_writable_iterator<QJsonArray::Iterator,QJsonValue> : std::true_type{};

template<>
struct is_writable_iterator<QJsonArray::Iterator,QJsonValueRef> : std::true_type{};

template<typename>
struct qtjambi_iterator_mutable_test;

template<>
struct qtjambi_iterator_mutable_test<QCborValueRef> : std::true_type {
};

template<>
struct qtjambi_iterator_mutable_test<QJsonValueRef> : std::true_type {
};

template<typename, typename, typename, typename>
struct iterator_comparable;

template<>
struct iterator_comparable<QCborArray,QCborArray::Iterator,QCborArray::ConstIterator,QCborArray::ConstIterator>{
    static bool test(const QCborArray::Iterator& iter, const QCborArray::ConstIterator& begin, const QCborArray::ConstIterator& end);
};

template<>
struct iterator_comparable<QCborArray,QCborArray::ConstIterator,QCborArray::ConstIterator,QCborArray::ConstIterator>{
    static bool test(const QCborArray::ConstIterator& iter, const QCborArray::ConstIterator& begin, const QCborArray::ConstIterator& end);
};

template<>
struct iterator_comparable<QCborMap,QCborMap::Iterator,QCborMap::ConstIterator,QCborMap::ConstIterator>{
    static bool test(const QCborMap::Iterator& iter, const QCborMap::ConstIterator& begin, const QCborMap::ConstIterator& end);
};

template<>
struct iterator_comparable<QCborMap,QCborMap::ConstIterator,QCborMap::ConstIterator,QCborMap::ConstIterator>{
    static bool test(const QCborMap::ConstIterator& iter, const QCborMap::ConstIterator& begin, const QCborMap::ConstIterator& end);
};

template<>
struct iterator_comparable<QJsonObject,QJsonObject::Iterator,QJsonObject::ConstIterator,QJsonObject::ConstIterator>{
    static bool test(const QJsonObject::Iterator& iter, const QJsonObject::ConstIterator& begin, const QJsonObject::ConstIterator& end);
};

template<>
struct iterator_comparable<QJsonObject,QJsonObject::ConstIterator,QJsonObject::ConstIterator,QJsonObject::ConstIterator>{
    static bool test(const QJsonObject::ConstIterator& iter, const QJsonObject::ConstIterator& begin, const QJsonObject::ConstIterator& end);
};

template<>
struct iterator_comparable<QJsonArray,QJsonArray::Iterator,QJsonArray::ConstIterator,QJsonArray::ConstIterator>{
    static bool test(const QJsonArray::Iterator& iter, const QJsonArray::ConstIterator& begin, const QJsonArray::ConstIterator& end);
};

template<>
struct iterator_comparable<QJsonArray,QJsonArray::ConstIterator,QJsonArray::ConstIterator,QJsonArray::ConstIterator>{
    static bool test(const QJsonArray::ConstIterator& iter, const QJsonArray::ConstIterator& begin, const QJsonArray::ConstIterator& end);
};

template<>
struct iterator_comparable<QVersionNumber,QVersionNumber::const_iterator,QVersionNumber::const_iterator,QVersionNumber::const_iterator>{
    static bool test(const QVersionNumber::const_iterator& iter, const QVersionNumber::const_iterator& begin, const QVersionNumber::const_iterator& end);
};

template<typename, bool>
struct IteratorType;

template<typename Storage, typename Iter>
struct IteratorType<ContainerIterator<QCborMap,Iter,Storage>,true>{
    static constexpr bool value_is_reference = false;
    static constexpr bool key_is_reference = false;
    using ValueType = QCborValue;
};

template<typename Storage, typename Iter>
struct IteratorType<ContainerIterator<QCborArray,Iter,Storage>,false>{
    static constexpr bool value_is_reference = false;
    using ValueType = QCborValue;
};

template<typename Storage, typename Iter>
struct IteratorType<ContainerIterator<QJsonObject,Iter,Storage>,true>{
    static constexpr bool value_is_reference = false;
    static constexpr bool key_is_reference = false;
    using ValueType = QJsonValue;
};

template<typename Storage, typename Iter>
struct IteratorType<ContainerIterator<QJsonArray,Iter,Storage>,false>{
    static constexpr bool value_is_reference = false;
    using ValueType = QJsonValue;
};

template<typename,typename,bool,bool,bool,bool,bool>
struct iter_value_type;

template<typename Iter, bool cv, bool iv, bool v, bool r>
struct iter_value_type<QJsonArray,Iter,false,cv,iv,v,r>{
    using value_type = QJsonValue;
};

template<typename Iter, bool cv, bool iv, bool v, bool r>
struct iter_value_type<QJsonObject,Iter,false,cv,iv,v,r>{
    using value_type = QJsonValue;
};

template<typename Iter, bool cv, bool iv, bool v, bool r>
struct iter_value_type<QCborArray,Iter,false,cv,iv,v,r>{
    using value_type = QCborValue;
};

template<typename Iter, bool cv, bool iv, bool v, bool r>
struct iter_value_type<QCborMap,Iter,false,cv,iv,v,r>{
    using value_type = QCborValue;
};

const QMetaType& getCborValueMetaType();
const QMetaType& getCborValuePairMetaType();
const QMetaType& getJsonValueMetaType();
const QMetaType& getJsonValuePairMetaType();

template<typename, bool>
struct IteratorSequentialValueType;

template<typename Iterator, typename Storage>
struct IteratorSequentialValueType<ContainerIterator<QCborArray,Iterator,Storage>,true>{
    using type = QCborValue;
    static constexpr const QMetaType& (&function)() = getCborValueMetaType;
};

template<typename Iterator, typename Storage>
struct IteratorSequentialValueType<ContainerIterator<QJsonArray,Iterator,Storage>,true>{
    using type = QJsonValue;
    static constexpr const QMetaType& (&function)() = getJsonValueMetaType;
};

template<typename, bool>
struct IteratorAssociativeValueType;

template<typename Iterator, typename Storage>
struct IteratorAssociativeValueType<ContainerIterator<QCborMap,Iterator,Storage>,true>{
    using type = QCborValue;
    static constexpr const QMetaType& (&function)() = getCborValueMetaType;
};

template<typename Iterator, typename Storage>
struct IteratorAssociativeValueType<ContainerIterator<QJsonObject,Iterator,Storage>,true>{
    using type = QJsonValue;
    static constexpr const QMetaType& (&function)() = getJsonValueMetaType;
};

template<typename Container, typename Iter, typename _Iter>
struct iterator_equals;

template<>
struct iterator_equals<QVersionNumber,QVersionNumber::const_iterator,QVersionNumber::const_iterator> : std::true_type{
    static bool function(const QVersionNumber::const_iterator& a, const QVersionNumber::const_iterator& b);
};

template<typename type>
struct ValueConverter;

template<>
struct ValueConverter<std::pair<QString,QJsonValue>>{
    template<typename K, typename T>
    static auto convert(const std::pair<K,T>& value){
        if constexpr(std::is_same_v<K,QAnyStringView>)
            return std::make_pair(value.first.toString(),QJsonValue(value.second));
        else
            return std::make_pair(QString(value.first),QJsonValue(value.second));
    }
};

template<>
struct ValueConverter<std::pair<QCborValue,QCborValue>>{
    template<typename K, typename T>
    static auto convert(const std::pair<K,T>& value){
        return std::make_pair(QCborValue(value.first),QCborValue(value.second));
    }
};

template<typename Iterator, typename T>
struct IteratorSequentialValueOptional;

template<typename Iter, typename Storage, typename T>
struct IteratorSequentialValueOptional<ContainerIterator<QString,Iter,Storage>,T>{
    using Iterator = ContainerIterator<QString,Iter,Storage>;
    using Iterator_c = std::conditional_t<std::is_same_v<T,void*>, Iterator, const Iterator>;
    static auto function(std::conditional_t<std::is_same_v<T,void*>, void*, const void*> ptr) {
        if constexpr(std::is_same_v<T,const void*> || std::is_same_v<T,void*>){
            Q_UNUSED(ptr)
            return std::nullopt;
        }else{
            Iterator_c& iterator = *static_cast<Iterator_c*>(ptr);
            if constexpr(std::is_same_v<T,QVariant>){
                return QVariant::fromValue(*iterator);
            }else if constexpr(std::is_same_v<T,jchar>){
                return std::make_optional(*reinterpret_cast<const jchar*>(&*iterator));
            }else{
                if constexpr(supports_assign_v<T&, decltype(*std::declval<Iterator_c&>())>){
                    Iterator_c& iterator = *static_cast<Iterator_c*>(ptr);
                    return std::make_optional<T>(*iterator);
                }else{
                    Q_UNUSED(iterator)
                    return std::nullopt;
                }
            }
        }
    }
};

template<typename T>
struct IteratorSequentialValueOptional<ContainerIterator<QVersionNumber,QVersionNumber::const_iterator,ContainerRef<QVersionNumber>>,T>{
    using Iterator = ContainerIterator<QVersionNumber,QVersionNumber::const_iterator,ContainerRef<QVersionNumber>>;
    using Iterator_c = std::conditional_t<std::is_same_v<T,void*>, Iterator, const Iterator>;
    static auto function(std::conditional_t<std::is_same_v<T,void*>, void*, const void*> ptr) {
        if constexpr(std::is_same_v<T,const void*> || std::is_same_v<T,void*>){
            Q_UNUSED(ptr)
            return std::nullopt;
        }else{
            Iterator_c& iterator = *static_cast<Iterator_c*>(ptr);
            if constexpr(std::is_same_v<T,QVariant>){
                return QVariant::fromValue(*iterator);
            }else if constexpr(std::is_same_v<T,jint>){
                return std::make_optional<T>(*iterator);
            }else{
                Q_UNUSED(iterator)
                return std::nullopt;
            }
        }
    }
};

template<typename T>
struct IteratorSequentialValueOptional<ContainerIterator<QVersionNumber,QVersionNumber::const_reverse_iterator,ContainerRef<QVersionNumber>>,T>{
    using Iterator = ContainerIterator<QVersionNumber,QVersionNumber::const_reverse_iterator,ContainerRef<QVersionNumber>>;
    using Iterator_c = std::conditional_t<std::is_same_v<T,void*>, Iterator, const Iterator>;
    static auto function(std::conditional_t<std::is_same_v<T,void*>, void*, const void*> ptr) {
        if constexpr(std::is_same_v<T,const void*> || std::is_same_v<T,void*>){
            Q_UNUSED(ptr)
            return std::nullopt;
        }else{
            Iterator_c& iterator = *static_cast<Iterator_c*>(ptr);
            if constexpr(std::is_same_v<T,QVariant>){
                return QVariant::fromValue(*iterator);
            }else if constexpr(std::is_same_v<T,jint>){
                return std::make_optional<T>(*iterator);
            }else{
                Q_UNUSED(iterator)
                return std::nullopt;
            }
        }
    }
};

template<typename, typename, bool>
struct CreateConstIterator;

template<typename Storage>
struct CreateConstIterator<ContainerIterator<QCborMap,QCborMap::iterator,Storage>, QCborMap, true>{
    using Iter = ContainerIterator<QCborMap,QCborMap::iterator,Storage>;
    using type = ContainerIterator<QCborMap,QCborMap::const_iterator>;
    static void* function(const void* ptr){
        const Iter& iterator = *static_cast<const Iter*>(ptr);
        if constexpr(std::is_same_v<Storage,ContainerRef<QCborMap>>){
            return new type(QCborMap::const_iterator(reinterpret_cast<const QCborMap::const_iterator&>(iterator.iterator())), iterator.storage().container());
        }else if constexpr(std::is_same_v<Storage,QCborMap>){
            return new type(QCborMap::const_iterator(reinterpret_cast<const QCborMap::const_iterator&>(iterator.iterator())), iterator.storage());
        }else{
            Q_UNUSED(ptr)
            return nullptr;
        }
    }
};

template<typename Storage>
struct CreateConstIterator<ContainerIterator<QCborArray,QCborArray::iterator,Storage>, QCborArray, true>{
    using Iter = ContainerIterator<QCborArray,QCborArray::iterator,Storage>;
    using type = ContainerIterator<QCborArray,QCborArray::const_iterator>;
    static void* function(const void* ptr){
        const Iter& iterator = *static_cast<const Iter*>(ptr);
        if constexpr(std::is_same_v<Storage,ContainerRef<QCborArray>>){
            return new type(QCborArray::const_iterator(reinterpret_cast<const QCborArray::const_iterator&>(iterator.iterator())), iterator.storage().container());
        }else if constexpr(std::is_same_v<Storage,QCborArray>){
            return new type(QCborArray::const_iterator(reinterpret_cast<const QCborArray::const_iterator&>(iterator.iterator())), iterator.storage());
        }else{
            Q_UNUSED(ptr)
            return nullptr;
        }
    }
};

template<>
struct CreateConstIterator<ContainerIterator<QJsonArray,QJsonArray::iterator,ContainerRef<QJsonArray>>, QJsonArray, true>{
    using Iter = ContainerIterator<QJsonArray,QJsonArray::iterator,ContainerRef<QJsonArray>>;
    using type = ContainerIterator<QJsonArray,QJsonArray::const_iterator,ContainerRef<QJsonArray>>;
    static void* function(const void* ptr){
        const Iter& iterator = *static_cast<const Iter*>(ptr);
        return new type(QJsonArray::const_iterator(reinterpret_cast<const QJsonArray::const_iterator&>(iterator.iterator())), iterator.storage());
        return nullptr;
    }
};

#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
template<>
struct is_writable_iterator<QCborMap::key_value_iterator,std::pair<QCborValueConstRef, QCborValueRef>> : std::true_type{};

template<>
struct is_writable_iterator<QJsonObject::key_value_iterator,std::pair<QString,QJsonValue>> : std::true_type{};

template<>
struct is_writable_iterator<QJsonObject::key_value_iterator,std::pair<QString,QJsonValueRef>> : std::true_type{};

template<typename Storage, typename Key, typename T, typename Iter, class Traits>
struct IteratorType<ContainerIterator<QCborMap,QKeyValueIterator<Key,T,Iter,Traits>,Storage>,false>{
    static constexpr bool value_is_reference = false;
    using ValueType = std::pair<QCborValue,QCborValue>;
};

template<typename Storage, typename Key, typename T, typename Iter, class Traits>
struct IteratorType<ContainerIterator<QJsonObject,QKeyValueIterator<Key,T,Iter,Traits>,Storage>,false>{
    static constexpr bool value_is_reference = false;
    using ValueType = std::pair<QString,QJsonValue>;
};

template<typename Key, typename T, typename Iter, class Traits, bool cv, bool iv, bool v, bool r>
struct iter_value_type<QCborMap,QKeyValueIterator<Key,T,Iter,Traits>,false,cv,iv,v,r>{
    using value_type = std::pair<QCborValue,QCborValue>;
};

template<typename Key, typename T, typename Iter, class Traits, bool cv, bool iv, bool v, bool r>
struct iter_value_type<QJsonObject,QKeyValueIterator<Key,T,Iter,Traits>,false,cv,iv,v,r>{
    using value_type = std::pair<QString,QJsonValue>;
};

template<typename Storage, typename Key, typename T, typename Iter, class Traits, bool cv, bool iv, bool v, bool r>
struct iter_value_type<QCborMap,ContainerIterator<QCborMap,QKeyValueIterator<Key,T,Iter,Traits>,Storage>,false,cv,iv,v,r>{
    using value_type = std::pair<QCborValue,QCborValue>;
};

template<typename Storage, typename Key, typename T, typename Iter, class Traits, bool cv, bool iv, bool v, bool r>
struct iter_value_type<QJsonObject,ContainerIterator<QCborMap,QKeyValueIterator<Key,T,Iter,Traits>,Storage>,false,cv,iv,v,r>{
    using value_type = std::pair<QString,QJsonValue>;
};

template<>
struct IteratorSequentialValueType<QCborMap::key_value_iterator,true>{
    using type = std::pair<QCborValue,QCborValue>;
    static constexpr const QMetaType& (&function)() = getCborValuePairMetaType;
};

template<>
struct IteratorSequentialValueType<QCborMap::const_key_value_iterator,true>{
    using type = std::pair<QCborValue,QCborValue>;
    static constexpr const QMetaType& (&function)() = getCborValuePairMetaType;
};

template<>
struct IteratorSequentialValueType<QJsonObject::const_key_value_iterator,true>{
    using type = std::pair<QString,QJsonValue>;
    static constexpr const QMetaType& (&function)() = getJsonValuePairMetaType;
};

template<>
struct IteratorSequentialValueType<QJsonObject::key_value_iterator,true>{
    using type = std::pair<QString,QJsonValue>;
    static constexpr const QMetaType& (&function)() = getJsonValuePairMetaType;
};

template<typename Storage>
struct CreateConstIterator<ContainerIterator<QCborMap,QCborMap::key_value_iterator,Storage>, QCborMap, true>{
    using Iter = ContainerIterator<QCborMap,QCborMap::key_value_iterator,Storage>;
    using type = ContainerIterator<QCborMap,QCborMap::const_key_value_iterator>;
    static void* function(const void* ptr){
        const Iter& iterator = *static_cast<const Iter*>(ptr);
        auto base = iterator.iterator().base();
        if constexpr(std::is_same_v<Storage,ContainerRef<QCborMap>>){
            return new type(QCborMap::const_key_value_iterator(*reinterpret_cast<const QCborMap::const_key_value_iterator*>(&base)), iterator.storage().container());
        }else if constexpr(std::is_same_v<Storage,QCborMap>){
            return new type(QCborMap::const_key_value_iterator(*reinterpret_cast<const QCborMap::const_key_value_iterator*>(&base)), iterator.storage());
        }else{
            Q_UNUSED(ptr)
            Q_UNUSED(base)
            return nullptr;
        }
    }
};

template<>
struct CreateConstIterator<ContainerIterator<QJsonObject,QJsonObject::key_value_iterator,ContainerRef<QJsonObject>>, QJsonObject, true>{
    using Iter = ContainerIterator<QJsonObject,QJsonObject::key_value_iterator,ContainerRef<QJsonObject>>;
    using type = ContainerIterator<QJsonObject,QJsonObject::const_key_value_iterator,ContainerRef<QJsonObject>>;
    static void* function(const void* ptr){
        const Iter& iterator = *static_cast<const Iter*>(ptr);
        return new type(QJsonObject::const_key_value_iterator(iterator.iterator().base()), iterator.storage());
        return nullptr;
    }
};

template<typename, bool>
struct qtjambi_sequential_iterator_mutable_test;

template<typename Container, typename Iterator, bool sequential, bool isMutable>
struct IteratorAccessType;

template<typename,bool,bool>
struct IteratorSequentialValue;

template<typename Iterator>
struct IteratorSequentialSetValue;

template<bool b>
struct qtjambi_sequential_iterator_mutable_test<QCborMap::key_value_iterator,b> : std::true_type {
};

template<bool b>
struct qtjambi_sequential_iterator_mutable_test<QJsonObject::key_value_iterator,b> : std::true_type {
};

template<bool b>
struct qtjambi_sequential_iterator_mutable_test<QCborMap::key_value_iterator&&,b> : std::true_type {
};

template<bool b>
struct qtjambi_sequential_iterator_mutable_test<QJsonObject::key_value_iterator&&,b> : std::true_type {
};

template<bool b>
struct qtjambi_sequential_iterator_mutable_test<const QCborMap::key_value_iterator&,b> : std::true_type {
};

template<bool b>
struct qtjambi_sequential_iterator_mutable_test<const QJsonObject::key_value_iterator&,b> : std::true_type {
};

template<>
struct IteratorAccessType<QCborMap,ContainerIterator<QCborMap,QCborMap::key_value_iterator,ContainerRef<QCborMap>>,true,false>{
    using type = QSequentialIteratorAccess<ContainerIterator<QCborMap,QCborMap::key_value_iterator,ContainerRef<QCborMap>>,QCborMap>;
};

template<>
struct IteratorAccessType<QJsonObject,ContainerIterator<QJsonObject,QJsonObject::key_value_iterator,ContainerRef<QJsonObject>>,true,false>{
    using type = QSequentialIteratorAccess<ContainerIterator<QJsonObject,QJsonObject::key_value_iterator,ContainerRef<QJsonObject>>,QJsonObject>;
};

template<>
struct IteratorSequentialValue<ContainerIterator<QCborMap,QCborMap::key_value_iterator,ContainerRef<QCborMap>>,false,true>{
    static void function(JNIEnv * env, void* ptr, jobject newValue);
};

template<>
struct IteratorSequentialValue<ContainerIterator<QJsonObject,QJsonObject::key_value_iterator,ContainerRef<QJsonObject>>,false,true>{
    static void function(JNIEnv * env, void* ptr, jobject newValue);
};

template<typename Storage>
struct IteratorSequentialSetValue<ContainerIterator<QJsonObject,QJsonObject::key_value_iterator,Storage>>{
    static bool function(void* ptr, const QVariant& value) {
        ContainerIterator<QJsonObject,QJsonObject::key_value_iterator,Storage>& iterator = *static_cast<ContainerIterator<QJsonObject,QJsonObject::key_value_iterator,Storage>*>(ptr);
        (*iterator).second = value.value<QJsonValue>();
        return true;
    }
    template<typename T>
    static bool function(void*, const T&) {
        return false;
    }
};

template<typename Storage>
struct IteratorSequentialSetValue<ContainerIterator<QCborMap,QCborMap::key_value_iterator,Storage>>{
    static bool function(void* ptr, const QVariant& value) {
        ContainerIterator<QCborMap,QCborMap::key_value_iterator,Storage>& iterator = *static_cast<ContainerIterator<QCborMap,QCborMap::key_value_iterator,Storage>*>(ptr);
        (*iterator).second = value.value<QCborValue>();
        return true;
    }
    template<typename T>
    static bool function(void*, const T&) {
        return false;
    }
};

template<>
struct is_writable_iterator<QCborMap::key_value_iterator,QCborValue> : std::true_type {};
template<>
struct is_writable_iterator<QJsonObject::key_value_iterator,QJsonValue> : std::true_type {};
#endif // QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
bool operator==(const QJsonObject::const_key_value_iterator& iter1, const QJsonObject::key_value_iterator& iter2);
bool operator==(const QJsonObject::key_value_iterator& iter1, const QJsonObject::const_key_value_iterator& iter2);
bool operator==(const QCborMap::const_key_value_iterator& iter1, const QCborMap::key_value_iterator& iter2);
bool operator==(const QCborMap::key_value_iterator& iter1, const QCborMap::const_key_value_iterator& iter2);
#endif // QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)

#endif // ITERATORS_P_H
