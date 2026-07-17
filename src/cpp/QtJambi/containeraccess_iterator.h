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

#ifndef CONTAINERACCESS_ITERATOR_H
#define CONTAINERACCESS_ITERATOR_H

#include "utils.h"
#include "typetests.h"
#include "qtjambiapi.h"
#include "containerapi.h"

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
#include <QtCore/QSpan>
#endif

enum class QtJambiNativeID : jlong;

template<typename T>
class QListAccess;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
template<typename T, std::size_t E = q20::dynamic_extent>
class QSpanAccess;
#endif
template<typename T>
class QSetAccess;
template<typename K, typename T>
class QMapAccess;
template<typename K, typename T>
class QMultiMapAccess;
template<typename K, typename T>
class QHashAccess;
template<typename K, typename T>
class QMultiHashAccess;

namespace QtJambiPrivate {

QTJAMBI_EXPORT jobject convertIteratorToJavaObject(JNIEnv *env,
                                                   const std::type_info& containerTypeId,
                                                   const std::type_info& iteratorTypeId,
                                                   const QSharedPointer<ContainerRefPrivate>& owner,
                                                   void* iteratorPtr,
                                                   PtrDeleterFunction destructor_function,
                                                   AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertIteratorToJavaObject(JNIEnv *env,
                                                   const std::type_info& containerTypeId,
                                                   const std::type_info& iteratorTypeId,
                                                   const QSharedPointer<ContainerRefPrivate>& owner,
                                                   void* iteratorPtr,
                                                   PtrDeleterFunction destructor_function,
                                                   AbstractAssociativeConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertListIteratorToJavaObject(JNIEnv *env,
                                                       const QSharedPointer<ContainerRefPrivate>& owner,
                                                       void* iteratorPtr,
                                                       PtrDeleterFunction destructor_function,
                                                       AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertSpanIteratorToJavaObject(JNIEnv *env,
                                                       const QSharedPointer<ContainerRefPrivate>& owner,
                                                       void* iteratorPtr,
                                                       PtrDeleterFunction destructor_function,
                                                       AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertListReverseIteratorToJavaObject(JNIEnv *env,
                                                       const QSharedPointer<ContainerRefPrivate>& owner,
                                                       void* iteratorPtr,
                                                       PtrDeleterFunction destructor_function,
                                                       AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertSpanReverseIteratorToJavaObject(JNIEnv *env,
                                                       const QSharedPointer<ContainerRefPrivate>& owner,
                                                       void* iteratorPtr,
                                                       PtrDeleterFunction destructor_function,
                                                       AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertSetIteratorToJavaObject(JNIEnv *env,
                                                      const QSharedPointer<ContainerRefPrivate>& owner,
                                                      void* iteratorPtr,
                                                      PtrDeleterFunction destructor_function,
                                                      AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertMapIteratorToJavaObject(JNIEnv *env,
                                                      const QSharedPointer<ContainerRefPrivate>& owner,
                                                      void* iteratorPtr,
                                                      PtrDeleterFunction destructor_function,
                                                      AbstractAssociativeConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertHashIteratorToJavaObject(JNIEnv *env,
                                                       const QSharedPointer<ContainerRefPrivate>& owner,
                                                       void* iteratorPtr,
                                                       PtrDeleterFunction destructor_function,
                                                       AbstractAssociativeConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertMultiMapIteratorToJavaObject(JNIEnv *env,
                                                      const QSharedPointer<ContainerRefPrivate>& owner,
                                                      void* iteratorPtr,
                                                      PtrDeleterFunction destructor_function,
                                                      AbstractAssociativeConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertMultiHashIteratorToJavaObject(JNIEnv *env,
                                                       const QSharedPointer<ContainerRefPrivate>& owner,
                                                       void* iteratorPtr,
                                                       PtrDeleterFunction destructor_function,
                                                       AbstractAssociativeConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertMapKeyIteratorToJavaObject(JNIEnv *env,
                                                         const QSharedPointer<ContainerRefPrivate>& owner,
                                                         void* iteratorPtr,
                                                         PtrDeleterFunction destructor_function,
                                                         AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertHashKeyIteratorToJavaObject(JNIEnv *env,
                                                       const QSharedPointer<ContainerRefPrivate>& owner,
                                                       void* iteratorPtr,
                                                       PtrDeleterFunction destructor_function,
                                                       AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertMultiMapKeyIteratorToJavaObject(JNIEnv *env,
                                                           const QSharedPointer<ContainerRefPrivate>& owner,
                                                           void* iteratorPtr,
                                                           PtrDeleterFunction destructor_function,
                                                           AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertMultiHashKeyIteratorToJavaObject(JNIEnv *env,
                                                            const QSharedPointer<ContainerRefPrivate>& owner,
                                                            void* iteratorPtr,
                                                            PtrDeleterFunction destructor_function,
                                                            AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertMapKeyValueIteratorToJavaObject(JNIEnv *env,
                                                              const QSharedPointer<ContainerRefPrivate>& owner,
                                                              void* iteratorPtr,
                                                              PtrDeleterFunction destructor_function,
                                                              AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertHashKeyValueIteratorToJavaObject(JNIEnv *env,
                                                       const QSharedPointer<ContainerRefPrivate>& owner,
                                                       void* iteratorPtr,
                                                       PtrDeleterFunction destructor_function,
                                                       AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertMultiMapKeyValueIteratorToJavaObject(JNIEnv *env,
                                                           const QSharedPointer<ContainerRefPrivate>& owner,
                                                           void* iteratorPtr,
                                                           PtrDeleterFunction destructor_function,
                                                           AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertMultiHashKeyValueIteratorToJavaObject(JNIEnv *env,
                                                            const QSharedPointer<ContainerRefPrivate>& owner,
                                                            void* iteratorPtr,
                                                            PtrDeleterFunction destructor_function,
                                                            AbstractSequentialConstIteratorAccess* access);

template<typename Iterator, bool = supports_key_v<Iterator>>
struct IteratorType{
    static constexpr bool value_is_reference = std::is_reference_v<decltype(std::declval<const Iterator&>().value())>;
    static constexpr bool key_is_reference = std::is_reference_v<decltype(std::declval<const Iterator&>().key())>;
    using ValueType = std::remove_cv_t<std::remove_reference_t<decltype(std::declval<const Iterator&>().value())>>;
};

template<typename Iterator>
struct IteratorType<Iterator,false>{
    static constexpr bool value_is_reference = std::is_reference_v<decltype(*std::declval<const Iterator&>())>;
    using ValueType = std::remove_cv_t<std::remove_reference_t<decltype(*std::declval<const Iterator&>())>>;
};

template<typename Iterator, bool is_const, bool supported = supports_deref_v<Iterator&>>
struct IteratorSequentialValue{
    static jobject function(JNIEnv *env, const void* ptr) {
        const Iterator& iterator = *static_cast<const Iterator*>(ptr);
        return ::qtjambi_cast<jobject>(env, *iterator);
    }
};

template<typename Iterator>
struct IteratorSequentialValue<Iterator,false,true>{
    static void function(JNIEnv * env, void* ptr, jobject newValue) {
        Iterator& iterator = *static_cast<Iterator*>(ptr);
        *iterator = ::qtjambi_cast<typename QtJambiPrivate::IteratorType<Iterator>::ValueType>(env, newValue);
    }
};

template<typename Key, typename T, typename Iterator QT610_EXTRA_ARG(class Traits)>
struct IteratorSequentialValue<QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>,false,true>{
    static void function(JNIEnv * env, void* ptr, jobject newValue) {
        QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>& iterator = *static_cast<QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>*>(ptr);
        iterator.base().value() = ::qtjambi_cast<std::remove_const_t<std::remove_reference_t<T>>>(env, newValue);
    }
};

template<typename Container, typename Key, typename T, typename Iterator QT610_EXTRA_ARG(class Traits), typename Storage>
struct IteratorSequentialValue<ContainerIterator<Container,QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>,Storage>,false,true>{
    static void function(JNIEnv * env, void* ptr, jobject newValue) {
        ContainerIterator<Container,QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>,Storage>& iterator = *static_cast<ContainerIterator<Container,QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>,Storage>*>(ptr);
        iterator.iterator().base().value() = ::qtjambi_cast<std::remove_const_t<std::remove_reference_t<T>>>(env, newValue);
    }
};

template<typename Iterator, bool is_const>
struct IteratorSequentialValue<Iterator, is_const, false>{
    static std::conditional_t<is_const,jobject,void> function(JNIEnv * env,...) {
        JavaException::raiseUnsupportedOperationException(env, "QIterator::value" QTJAMBI_STACKTRACEINFO );
    }
};

template<typename Iterator, bool supported = supports_deref_v<Iterator&>>
struct IteratorSequentialValueType{
    using type = std::remove_cv_t<std::remove_reference_t<decltype(*std::declval<Iterator>())>>;
    static const QMetaType& function() {
        static QMetaType mt(QMetaType::fromType<type>());
        return mt;
    }
};

template<typename type>
struct ValueConverter{
    template<typename T>
    static auto convert(const T& value){
        return std::forward<const T>(value);
    }
};

template<typename Key, typename V>
struct ValueConverter<std::pair<Key,V>>{
    template<typename K, typename T>
    static auto convert(const std::pair<K,T>& value){
        if constexpr(std::is_same_v<Key, K> && std::is_same_v<V, T>){
            return std::forward<const std::pair<K,T>>(value);
        }else{
            return std::make_pair(Key(value.first), V(value.second));
        }
    }
};

template<typename Container, typename Iterator, typename Storage>
struct IteratorSequentialValueType<ContainerIterator<Container,Iterator,Storage>, true> : IteratorSequentialValueType<Iterator> {
};

template<typename Iterator, typename T>
struct IteratorSequentialValueOptional{
    using Iterator_c = std::conditional_t<std::is_same_v<T,void*>, Iterator, const Iterator>;
    static auto function(std::conditional_t<std::is_same_v<T,void*>, void*, const void*> ptr) {
        if constexpr(std::is_same_v<T,QVariant>){
            if constexpr(QtJambiPrivate::supports_deref_v<Iterator_c&>){
                Iterator_c& iterator = *static_cast<Iterator_c*>(ptr);
                return QVariant::fromValue(ValueConverter<typename IteratorSequentialValueType<Iterator>::type>::convert(*iterator));
            }else{
                Q_UNUSED(ptr)
                return QVariant();
            }
        }else if constexpr(supports_deref_v<Iterator_c&>){
            if constexpr(std::is_same_v<T,const void*> || std::is_same_v<T,void*>){
                if constexpr(IteratorType<Iterator>::value_is_reference){
                    Iterator_c& iterator = *static_cast<Iterator_c*>(ptr);
                    return std::make_optional<T>(&*iterator);
                }else{
                    Q_UNUSED(ptr)
                    return std::nullopt;
                }
            }else{
                if constexpr(supports_assign_v<T&, decltype(*std::declval<Iterator_c&>())>){
                    Iterator_c& iterator = *static_cast<Iterator_c*>(ptr);
                    return std::make_optional<T>(*iterator);
                }else{
                    Q_UNUSED(ptr)
                    return std::nullopt;
                }
            }
        }else{
            Q_UNUSED(ptr)
            return std::nullopt;
        }
    }
};

template<typename Key, typename T, class Iterator QT610_EXTRA_ARG(class Traits)>
struct IteratorSequentialValueType<QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>,true>{
    using Key_plain = std::remove_reference_t<Key>;
    using T_plain = std::remove_reference_t<T>;
    using type = std::pair<std::conditional_t<std::is_pointer_v<Key_plain>, Key_plain, std::remove_cv_t<Key_plain>>,
                           std::conditional_t<std::is_pointer_v<T_plain>, T_plain, std::remove_cv_t<T_plain>>>;
    static const QMetaType& function() {
        static QMetaType mt(QMetaType::fromType<type>());
        return mt;
    }
};

template<typename Iterator>
struct IteratorSequentialSetValue{
    static bool function(void* ptr, const QVariant& value) {
        using V = typename IteratorSequentialValueType<Iterator>::type;
        if constexpr(supports_deref_v<Iterator&> && supports_assign_v<decltype(*std::declval<Iterator>()), V> && is_default_constructible_v<V>){
            Iterator& iterator = *static_cast<Iterator*>(ptr);
            *iterator = value.value<V>();
            return true;
        }else{
            Q_UNUSED(ptr)
            Q_UNUSED(value)
            return false;
        }
    }

    template<typename T>
    static bool function(void* ptr, const T& value) {
        if constexpr(supports_deref_v<Iterator&> && supports_assign_v<decltype(*std::declval<Iterator>()), T>){
            Iterator& iterator = *static_cast<Iterator*>(ptr);
            *iterator = value;
            return true;
        }else{
            Q_UNUSED(ptr)
            Q_UNUSED(value)
            return false;
        }
    }
};

template<typename Iterator, typename Container, bool = supports_const_iterator_v<Container>>
struct CreateConstIterator{
    using type = typename Container::const_iterator;
    static void* function(const void* ptr){
        if constexpr(supports_new_v<typename Container::const_iterator, const Iterator&>){
            const Iterator& iterator = *static_cast<const Iterator*>(ptr);
            return new typename Container::const_iterator(iterator);
        }
        Q_UNUSED(ptr)
        return nullptr;
    }
};

template<typename Container>
struct CreateConstIterator<typename Container::reverse_iterator, Container, true>{
    using type = typename Container::const_reverse_iterator;
    static void* function(const void* ptr){
        if constexpr(supports_new_v<typename Container::const_reverse_iterator, const typename Container::iterator&>){
            const typename Container::reverse_iterator& iterator = *static_cast<const typename Container::reverse_iterator*>(ptr);
            return new typename Container::const_reverse_iterator(iterator.base());
        }else if constexpr(supports_new_v<typename Container::const_reverse_iterator, const typename Container::reverse_iterator&>){
            const typename Container::reverse_iterator& iterator = *static_cast<const typename Container::reverse_iterator*>(ptr);
            return new typename Container::const_reverse_iterator(iterator);
        }
        Q_UNUSED(ptr)
        return nullptr;
    }
};

template<typename Container>
struct CreateConstIterator<typename Container::key_value_iterator, Container, true>{
    using type = typename Container::const_key_value_iterator;
    static void* function(const void* ptr){
        if constexpr(supports_new_v<typename Container::const_key_value_iterator, const typename Container::iterator&>){
            const typename Container::key_value_iterator& iterator = *static_cast<const typename Container::key_value_iterator*>(ptr);
            return new typename Container::const_key_value_iterator(iterator.base());
        }else if constexpr(supports_new_v<typename Container::const_key_value_iterator, const typename Container::key_value_iterator&>){
            const typename Container::key_value_iterator& iterator = *static_cast<const typename Container::key_value_iterator*>(ptr);
            return new typename Container::const_key_value_iterator(iterator);
        }
        Q_UNUSED(ptr)
        return nullptr;
    }
};

template<typename Container>
struct CreateConstIterator<typename Container::iterator, Container, true>{
    using type = typename Container::const_iterator;
    static void* function(const void* ptr){
        if constexpr(supports_new_v<typename Container::const_iterator, const typename Container::iterator&>){
            const typename Container::iterator& iterator = *static_cast<const typename Container::iterator*>(ptr);
            return new typename Container::const_iterator(iterator);
        }
        Q_UNUSED(ptr)
        return nullptr;
    }
};

template<typename Iterator, typename Container>
struct CreateConstIterator<Iterator,Container,false>{
    using type = void;
};

template<typename Container, typename Storage>
struct CreateConstIterator<ContainerIterator<Container,typename Container::iterator,Storage>, Container, true>{
    using Iter = ContainerIterator<Container,typename Container::iterator,Storage>;
    using type = ContainerIterator<Container,typename Container::const_iterator,Container>;
    static void* function(const void* ptr){
        if constexpr(supports_new_v<typename Container::const_iterator, const typename Container::iterator&>){
            const Iter& iterator = *static_cast<const Iter*>(ptr);
            if constexpr(std::is_same_v<Container,Storage>){
                return new type(typename Container::const_iterator(iterator.iterator()), iterator.storage());
            }else if constexpr(std::is_same_v<Storage,ContainerRef<Container>>){
                return new type(typename Container::const_iterator(iterator.iterator()), iterator.storage().container(), iterator.storage().nativeId());
            }else{
                return new type(typename Container::const_iterator(iterator.iterator()), iterator.storage(), QtJambiNativeID::Invalid);
            }
        }
        Q_UNUSED(ptr)
        return nullptr;
    }
};

template<typename Container, typename Storage>
struct CreateConstIterator<ContainerIterator<Container,typename Container::key_value_iterator,Storage>, Container, true>{
    using Iter = ContainerIterator<Container,typename Container::key_value_iterator,Storage>;
    using type = ContainerIterator<Container,typename Container::const_key_value_iterator, Container>;
    static void* function(const void* ptr){
        if constexpr(supports_new_v<typename Container::const_key_value_iterator, const typename Container::const_iterator&>){
            const Iter& iterator = *static_cast<const Iter*>(ptr);
            if constexpr(std::is_same_v<Container,Storage>){
                return new type(typename Container::const_key_value_iterator(typename Container::const_iterator(iterator.iterator().base())), iterator.storage());
            }else if constexpr(std::is_same_v<Storage,ContainerRef<Container>>){
                return new type(typename Container::const_key_value_iterator(typename Container::const_iterator(iterator.iterator().base())), iterator.storage().container(), iterator.storage().nativeId());
            }else{
                return new type(typename Container::const_key_value_iterator(typename Container::const_iterator(iterator.iterator().base())), iterator.storage().container(), QtJambiNativeID::Invalid);
            }
        }
        Q_UNUSED(ptr)
        return nullptr;
    }
};

template<typename Container, typename Storage>
struct CreateConstIterator<ContainerIterator<Container,typename Container::reverse_iterator,Storage>, Container, true>{
    using Iter = ContainerIterator<Container,typename Container::reverse_iterator,Storage>;
    using type = ContainerIterator<Container,typename Container::const_reverse_iterator, Container>;
    static void* function(const void* ptr){
        if constexpr(supports_new_v<typename Container::const_reverse_iterator, const typename Container::const_iterator&>){
            const Iter& iterator = *static_cast<const Iter*>(ptr);
            if constexpr(std::is_same_v<Container,Storage>){
                return new type(typename Container::const_reverse_iterator(typename Container::const_iterator(iterator.iterator().base())), iterator.storage());
            }else if constexpr(std::is_same_v<Storage,ContainerRef<Container>>){
                return new type(typename Container::const_reverse_iterator(typename Container::const_iterator(iterator.iterator().base())), iterator.storage().container(), iterator.storage().nativeId());
            }else{
                return new type(typename Container::const_reverse_iterator(typename Container::const_iterator(iterator.iterator().base())), iterator.storage().container(), QtJambiNativeID::Invalid);
            }
        }
        Q_UNUSED(ptr)
        return nullptr;
    }
};

template<typename Iterator>
struct IteratorSequentialValueType<Iterator, false>{
    using type = void;
    static const QMetaType& function() {
        static QMetaType mt{};
        return mt;
    }
};

template<typename Iterator, bool is_const, bool supported = supports_value_v<Iterator>>
struct IteratorAssociativeValue{
    static jobject function(JNIEnv *env, const void* ptr) {
        const Iterator& iterator = *static_cast<const Iterator*>(ptr);
        return ::qtjambi_cast<jobject>(env, iterator.value());
    }
};

template<typename Iterator>
struct IteratorAssociativeValue<Iterator,false,true>{
    static void function(JNIEnv * env, void* ptr, jobject newValue) {
        Iterator& iterator = *static_cast<Iterator*>(ptr);
        iterator.value() = ::qtjambi_cast<typename QtJambiPrivate::IteratorType<Iterator>::ValueType>(env, newValue);
    }
};

template<typename Iterator, bool is_const>
struct IteratorAssociativeValue<Iterator, is_const, false>{
    static std::conditional_t<is_const,jobject,void> function(JNIEnv * env,...) {
        JavaException::raiseUnsupportedOperationException(env, "QIterator::value" QTJAMBI_STACKTRACEINFO );
    }
};

template<typename Iterator, bool supported = supports_value_v<Iterator>>
struct IteratorAssociativeValueType{
    using type = std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Iterator>().value())>>;
    static const QMetaType& function() {
        static QMetaType mt(QMetaType::fromType<type>());
        return mt;
    }
};

template<typename Iterator>
struct IteratorAssociativeValueType<Iterator, false>{
    using type = void;
    static const QMetaType& function() {
        static QMetaType mt{};
        return mt;
    }
};

template<typename Iterator>
struct IteratorAssociativeSetValue{
    static bool function(void* ptr, const QVariant& value) {
        using V = typename IteratorSequentialValueType<Iterator>::type;
        if constexpr(supports_value_v<Iterator> && supports_assign_v<decltype(std::declval<Iterator>().value()), V> && is_default_constructible_v<V>){
            Iterator& iterator = *static_cast<Iterator*>(ptr);
            iterator.value() = value.value<V>();
            return true;
        }else{
            Q_UNUSED(ptr)
            Q_UNUSED(value)
            return false;
        }
    }
    template<typename T>
    static bool function(void* ptr, const T& value) {
        if constexpr(supports_value_v<Iterator> && supports_assign_v<decltype(std::declval<Iterator>().value()), T>){
            Iterator& iterator = *static_cast<Iterator*>(ptr);
            iterator.value() = value;
            return true;
        }else{
            Q_UNUSED(ptr)
            Q_UNUSED(value)
            return false;
        }
    }
};

template<typename Iterator, typename T>
struct IteratorAssociativeValueOptional{
    using Iterator_c = std::conditional_t<std::is_same_v<T,void*>, Iterator, const Iterator>;
    static auto function(std::conditional_t<std::is_same_v<T,void*>, void*, const void*> ptr) {
        if constexpr(std::is_same_v<T,QVariant>){
            if constexpr(QtJambiPrivate::supports_deref_v<Iterator_c&>){
                Iterator_c& iterator = *static_cast<Iterator_c*>(ptr);
                return QVariant::fromValue(ValueConverter<typename IteratorAssociativeValueType<Iterator>::type>::convert(iterator.value()));
            }else{
                Q_UNUSED(ptr)
                return QVariant();
            }
        }else if constexpr(supports_value_v<Iterator_c&>){
            if constexpr(std::is_same_v<T,const void*> || std::is_same_v<T,void*>){
                if constexpr(IteratorType<Iterator>::value_is_reference){
                    Iterator_c& iterator = *static_cast<Iterator_c*>(ptr);
                    return std::make_optional<T>(&iterator.value());
                }else{
                    Q_UNUSED(ptr)
                    return std::nullopt;
                }
            }else{
                if constexpr(supports_assign_v<T, std::add_lvalue_reference_t<decltype(*std::declval<Iterator_c&>())>>){
                    Iterator_c& iterator = *static_cast<Iterator_c*>(ptr);
                    return std::make_optional<T>(iterator.value());
                }else{
                    Q_UNUSED(ptr)
                    return std::nullopt;
                }
            }
        }else{
            Q_UNUSED(ptr)
            return std::nullopt;
        }
    }
};

template<typename Iterator, bool supported = supports_key_v<Iterator>>
struct IteratorAssociativeKey{
    static jobject function(JNIEnv *env, const void* ptr) {
        const Iterator& iterator = *static_cast<const Iterator*>(ptr);
        return ::qtjambi_cast<jobject>(env, iterator.key());
    }
};

template<typename Iterator, bool supported = supports_key_v<Iterator>>
struct IteratorAssociativeKeyType{
    using type = std::remove_cv_t<std::remove_reference_t<decltype(std::declval<Iterator>().key())>>;
    static const QMetaType& function() {
        static QMetaType mt(QMetaType::fromType<type>());
        return mt;
    }
};

template<typename Iterator>
struct IteratorAssociativeKeyType<Iterator, false>{
    using type = void;
    static const QMetaType& function() {
        static QMetaType mt{};
        return mt;
    }
};

template<typename Iterator, typename T>
struct IteratorAssociativeKeyOptional{
    using Iterator_c = std::conditional_t<std::is_same_v<T,void*>, Iterator, const Iterator>;
    static auto function(std::conditional_t<std::is_same_v<T,void*>, void*, const void*> ptr) {
        if constexpr(std::is_same_v<T,QVariant>){
            if constexpr(QtJambiPrivate::supports_deref_v<const Iterator&>){
                Iterator_c& iterator = *static_cast<Iterator_c*>(ptr);
                return QVariant::fromValue(ValueConverter<typename IteratorAssociativeKeyType<Iterator>::type>::convert(iterator.key()));
            }else{
                Q_UNUSED(ptr)
                return QVariant();
            }
        }else if constexpr(supports_key_v<Iterator_c&>){
            if constexpr(std::is_same_v<T,const void*> || std::is_same_v<T,void*>){
                if constexpr(IteratorType<Iterator>::key_is_reference){
                    Iterator_c& iterator = *static_cast<Iterator_c*>(ptr);
                    return std::make_optional<T>(&iterator.key());
                }else{
                    Q_UNUSED(ptr)
                    return std::nullopt;
                }
            }else{
                if constexpr(supports_assign_v<T, std::add_lvalue_reference_t<decltype(*std::declval<Iterator_c&>())>>){
                    Iterator_c& iterator = *static_cast<Iterator_c*>(ptr);
                    return std::make_optional<T>(iterator.key());
                }else{
                    Q_UNUSED(ptr)
                    return std::nullopt;
                }
            }
        }else{
            Q_UNUSED(ptr)
            return std::nullopt;
        }
    }
};

template<typename Iterator>
struct IteratorAssociativeKey<Iterator, false>{
    static jobject function(JNIEnv * env,...) {
        JavaException::raiseUnsupportedOperationException(env, "QIterator::key" QTJAMBI_STACKTRACEINFO );
    }
};

template<typename Iterator, typename Container>
struct AsIterator{
    static void* function(void* iterator){
        return iterator;
    }
};

template<typename Iterator, typename Container>
struct IteratorCastOptional{
    static bool function(const void* iterator, const std::type_info& typeId, void* out){
        if constexpr(supports_assign_v<Iterator,const Iterator&>){
            if(typeid_equals(typeid(Iterator), typeId)){
                *static_cast<Iterator*>(out) = *static_cast<const Iterator*>(iterator);
                return true;
            }
        }
        if constexpr(supports_const_iterator_v<Container>){
            if constexpr(supports_assign_v<typename Container::const_iterator, const Iterator&>){
                if(typeid_equals(typeid(typename Container::const_iterator), typeId)){
                    *static_cast<typename Container::const_iterator*>(out) = *static_cast<const Iterator*>(iterator);
                    return true;
                }
            }
        }
        if constexpr(supports_iterator_v<Container>){
            if constexpr(supports_assign_v<typename Container::iterator, const Iterator&>){
                if(typeid_equals(typeid(typename Container::iterator), typeId)){
                    *static_cast<typename Container::iterator*>(out) = *static_cast<const Iterator*>(iterator);
                    return true;
                }
            }
        }
        return false;
    }
};

template<typename Iterator, typename Container, typename Storage>
struct IteratorCastOptional<ContainerIterator<Container,Iterator,Storage>,Container>{
    static bool function(const void* iterator, const std::type_info& typeId, void* out){
        if constexpr(supports_assign_v<Iterator&,const Iterator&>){
            if(typeid_equals(typeid(Iterator), typeId)){
                *static_cast<Iterator*>(out) = static_cast<const ContainerIterator<Container,Iterator,Storage>*>(iterator)->iterator();
                return true;
            }
        }
        if constexpr(supports_const_iterator_v<Container>){
            if constexpr(supports_assign_v<typename Container::const_iterator, const Iterator&>){
                if(typeid_equals(typeid(typename Container::const_iterator), typeId)){
                    *static_cast<typename Container::const_iterator*>(out) = static_cast<const ContainerIterator<Container,Iterator,Storage>*>(iterator)->iterator();
                    return true;
                }
            }
        }
        if constexpr(supports_iterator_v<Container>){
            if constexpr(supports_assign_v<typename Container::iterator, const Iterator&>){
                if(typeid_equals(typeid(typename Container::iterator), typeId)){
                    *static_cast<typename Container::iterator*>(out) = static_cast<const ContainerIterator<Container,Iterator,Storage>*>(iterator)->iterator();
                    return true;
                }
            }
        }
        return false;
    }
};

template<typename Iterator, typename Container, typename Storage>
struct AsIterator<ContainerIterator<Container,Iterator,Storage>,Container>{
    static void* function(void* iterator){
        return &static_cast<ContainerIterator<Container,Iterator,Storage>*>(iterator)->iterator();
    }
};

template<typename Iterator, bool = supports_isValid_v<const Iterator>>
struct IteratorIsValid{
    static jboolean function(JNIEnv *, const void* ptr) {
        const Iterator* iterator = static_cast<const Iterator*>(ptr);
        return iterator->isValid();
    }
    static std::optional<bool> function(const void* ptr) {
        const Iterator* iterator = static_cast<const Iterator*>(ptr);
        return std::make_optional<bool>(iterator->isValid());
    }
};

template<typename Iterator>
struct IteratorIsValid<Iterator, false>{
    static jboolean function(JNIEnv *env, const void*) {
        JavaException::raiseUnsupportedOperationException(env, "QIterator::isValid" QTJAMBI_STACKTRACEINFO );
        return false;
    }
    static std::optional<bool> function(const void*) {
        return std::nullopt;
    }
};

template<typename Iterator, bool supported = std::is_pointer_v<Iterator> || supports_increment_v<Iterator>>
struct IteratorIncrement{
    static void function(JNIEnv *, void* ptr) {
        Iterator& iterator = *static_cast<Iterator*>(ptr);
        ++iterator;
    }
    static void function(void* ptr) {
        Iterator& iterator = *static_cast<Iterator*>(ptr);
        ++iterator;
    }
};

template<typename Iterator>
struct IteratorIncrement<Iterator,false>{
    static void function(JNIEnv * env, void*) {
        JavaException::raiseUnsupportedOperationException(env, "QIterator::increment" QTJAMBI_STACKTRACEINFO );
    }
    static void function(void*) {
    }
};

template<typename Iterator>
struct IteratorAdvance{
    static void function(JNIEnv *env, void* ptr, qsizetype n) {
        Iterator& iterator = *static_cast<Iterator*>(ptr);
        if constexpr(std::is_pointer_v<Iterator> || supports_add_assign_v<Iterator, qsizetype>){
            iterator += n;
        }else{
            if(n>0) {
                if constexpr(supports_increment_v<Iterator>){
                    for(;n>0;--n) {
                        if(IteratorIsValid<Iterator>::function(env, ptr))
                            ++iterator;
                        else JavaException::raiseNoSuchElementException(env, "" QTJAMBI_STACKTRACEINFO );
                    }
                }else{
                    JavaException::raiseUnsupportedOperationException(env, "advance" QTJAMBI_STACKTRACEINFO );
                }
            }else if(n<0) {
                if constexpr(supports_decrement_v<Iterator>){
                    for(;n<0;++n) {
                        if(IteratorIsValid<Iterator>::function(env, ptr))
                            --iterator;
                        else JavaException::raiseNoSuchElementException(env, "" QTJAMBI_STACKTRACEINFO );
                    }
                }else{
                    JavaException::raiseUnsupportedOperationException(env, "retreat" QTJAMBI_STACKTRACEINFO );
                }
            }
        }
        Q_UNUSED(env)
    }
    static bool function(void* ptr, qsizetype n) {
        Iterator& iterator = *static_cast<Iterator*>(ptr);
        if constexpr(std::is_pointer_v<Iterator> || supports_add_assign_v<Iterator, qsizetype>){
            iterator += n;
            return true;
        }else{
            if(n>0) {
                if constexpr(supports_increment_v<Iterator>){
                    for(;n>0;--n) {
                        if(IteratorIsValid<Iterator>::function(ptr).value_or(false))
                            ++iterator;
                        else return false;
                    }
                }else return false;
            }else if(n<0) {
                if constexpr(supports_decrement_v<Iterator>){
                    for(;n<0;++n) {
                        if(IteratorIsValid<Iterator>::function(ptr).value_or(false))
                            --iterator;
                        else return false;
                    }
                }else return false;
            }
            return true;
        }
    }
};

template<typename Iterator, bool supported = std::is_pointer_v<Iterator> || supports_decrement_v<Iterator>>
struct IteratorDecrement : std::bool_constant<supported> {
    static void function(JNIEnv *, void* ptr) {
        Iterator& iterator = *static_cast<Iterator*>(ptr);
        --iterator;
    }
    static void function(void* ptr) {
        Iterator& iterator = *static_cast<Iterator*>(ptr);
        --iterator;
    }
};

template<typename Iterator>
struct IteratorDecrement<Iterator,false> : std::false_type {
    static void function(JNIEnv * env, void*) {
        JavaException::raiseUnsupportedOperationException(env, "QIterator::decrement" QTJAMBI_STACKTRACEINFO );
    }
    static void function(void*) {
    }
};

template<typename Iterator, typename Iterator2 = Iterator, bool support = (std::is_pointer_v<Iterator> && std::is_pointer_v<Iterator2>) || supports_equal_v<Iterator,Iterator2>>
struct IteratorEqual : std::bool_constant<support>{
    static jboolean function(JNIEnv *, const void* ptr, const void* ptr2) {
        const Iterator& iterator = *static_cast<const Iterator*>(ptr);
        const Iterator2& iterator2 = *static_cast<const Iterator2*>(ptr2);
        return iterator==iterator2;
    }
    static std::optional<bool> function(const void* ptr, const void* ptr2) {
        const Iterator& iterator = *static_cast<const Iterator*>(ptr);
        const Iterator2& iterator2 = *static_cast<const Iterator2*>(ptr2);
        return std::make_optional<bool>(iterator==iterator2);
    }
};

template<typename Iterator,typename Iterator2>
struct IteratorEqual<Iterator, Iterator2, false> : std::false_type {
    static jboolean function(JNIEnv *env, const void*, const void*) {
        JavaException::raiseUnsupportedOperationException(env, "QIterator::equals" QTJAMBI_STACKTRACEINFO );
        return false;
    }
    static std::optional<bool> function(const void*, const void*) {
        return std::nullopt;
    }
};

template<typename Container, typename Iterator, typename Storage, typename Iterator2, typename Storage2, bool support>
struct IteratorEqual<ContainerIterator<Container,Iterator,Storage>,ContainerIterator<Container,Iterator2,Storage2>,support> : std::bool_constant<IteratorEqual<Iterator,Iterator2>::value> {
    static jboolean function(JNIEnv *env, const void* ptr, const void* ptr2) {
        const ContainerIterator<Container,Iterator,Storage>& iterator = *static_cast<const ContainerIterator<Container,Iterator,Storage>*>(ptr);
        const ContainerIterator<Container,Iterator2,Storage2>& iterator2 = *static_cast<const ContainerIterator<Container,Iterator2,Storage2>*>(ptr2);
        return IteratorEqual<Iterator,Iterator2>::function(env, &iterator.iterator(), &iterator2.iterator());
    }
    static std::optional<bool> function(const void* ptr, const void* ptr2) {
        const ContainerIterator<Container,Iterator,Storage>& iterator = *static_cast<const ContainerIterator<Container,Iterator,Storage>*>(ptr);
        const ContainerIterator<Container,Iterator2,Storage2>& iterator2 = *static_cast<const ContainerIterator<Container,Iterator2,Storage2>*>(ptr2);
        return IteratorEqual<Iterator,Iterator2>::function(&iterator.iterator(), &iterator2.iterator());
    }
};


template<typename Iterator, typename Iterator2 = Iterator, bool support = (std::is_pointer_v<Iterator> && std::is_pointer_v<Iterator2>) || supports_less_than_v<Iterator,Iterator2>>
struct IteratorLessThan : std::true_type {
    static jboolean function(JNIEnv *, const void* ptr, const void* ptr2) {
        const Iterator& iterator = *static_cast<const Iterator*>(ptr);
        const Iterator2& iterator2 = *static_cast<const Iterator2*>(ptr2);
        return iterator<iterator2;
    }
    static std::optional<bool> function(const void* ptr, const void* ptr2) {
        const Iterator& iterator = *static_cast<const Iterator*>(ptr);
        const Iterator2& iterator2 = *static_cast<const Iterator2*>(ptr2);
        return std::make_optional<bool>(iterator<iterator2);
    }
};

template<typename Iterator, typename Iterator2>
struct IteratorLessThan<Iterator, Iterator2, false> : std::false_type {
    static jboolean function(JNIEnv *env, const void*, const void*) {
        JavaException::raiseUnsupportedOperationException(env, "QIterator::lessThan" QTJAMBI_STACKTRACEINFO );
        return false;
    }
    static std::optional<bool> function(const void*, const void*) {
        return std::nullopt;
    }
};

template<typename Iterator, typename Iterator2 = Iterator>
struct IteratorDistance : supports_subtract<const Iterator2&,const Iterator&>{
    static std::optional<size_t> function(const void* ptr, const void* ptr2) {
        if constexpr(supports_subtract_v<const Iterator2&,const Iterator&>){
            const Iterator& iterator = *static_cast<const Iterator*>(ptr);
            const Iterator2& iterator2 = *static_cast<const Iterator2*>(ptr2);
            return std::make_optional<size_t>(iterator2 - iterator);
        }else{
            return std::nullopt;
        }
    }
};

template<typename Container, typename Iterator, typename Storage, typename Iterator2, typename Storage2>
struct IteratorDistance<ContainerIterator<Container,Iterator,Storage>,ContainerIterator<Container,Iterator2,Storage2>> : supports_subtract<const Iterator2&,const Iterator&>{
    static std::optional<size_t> function(const void* ptr, const void* ptr2) {
        if constexpr(supports_subtract_v<const Iterator2&,const Iterator&>){
            const ContainerIterator<Container,Iterator,Storage>& iterator = *static_cast<const ContainerIterator<Container,Iterator,Storage>*>(ptr);
            const ContainerIterator<Container,Iterator2,Storage2>& iterator2 = *static_cast<const ContainerIterator<Container,Iterator2,Storage2>*>(ptr2);
            return std::make_optional<size_t>(iterator2.iterator() - iterator.iterator());
        }else{
            return std::nullopt;
        }
    }
};

template<typename Iterator, bool = supports_isBegin_v<const Iterator>>
struct IteratorIsBegin{
    static jboolean function(JNIEnv *, const void* ptr) {
        const Iterator* iterator = static_cast<const Iterator*>(ptr);
        return iterator->isBegin();
    }
    static constexpr std::optional<bool> function(const void* ptr) {
        const Iterator* iterator = static_cast<const Iterator*>(ptr);
        return std::make_optional<bool>(iterator->isBegin());
    }
};

template<typename Iterator>
struct IteratorIsBegin<Iterator, false>{
    static jboolean function(JNIEnv *env, const void*) {
        JavaException::raiseUnsupportedOperationException(env, "QIterator::isBegin" QTJAMBI_STACKTRACEINFO );
        return false;
    }
    static std::optional<bool> function(const void*) {
        return std::nullopt;
    }
};

template<typename Iterator, bool = supports_isEnd_v<const Iterator>>
struct IteratorIsEnd{
    static jboolean function(JNIEnv *, const void* ptr) {
        const Iterator* iterator = static_cast<const Iterator*>(ptr);
        return iterator->isEnd();
    }
    static std::optional<bool> function(const void* ptr) {
        const Iterator* iterator = static_cast<const Iterator*>(ptr);
        return std::make_optional<bool>(iterator->isEnd());
    }
};

template<typename Iterator>
struct IteratorIsEnd<Iterator, false>{
    static jboolean function(JNIEnv *env, const void*) {
        JavaException::raiseUnsupportedOperationException(env, "QIterator::isEnd" QTJAMBI_STACKTRACEINFO );
        return false;
    }
    static std::optional<bool> function(const void*) {
        return std::nullopt;
    }
};

template<typename Container>
struct ContainerAccessDecider : std::false_type {
};

template<typename Container>
constexpr bool has_access_v = ContainerAccessDecider<Container>::value;

template<typename Container>
using container_access_t = typename ContainerAccessDecider<Container>::type;

template<typename Container>
struct ContainerAccessDecider<ContainerRef<Container>> : ContainerAccessDecider<Container> {
};

template<typename T>
struct ContainerAccessDecider<QList<T>> : std::true_type {
    using type = QListAccess<T>;
};

template<typename T>
struct ContainerAccessDecider<QSet<T>> : std::true_type {
    using type = QSetAccess<T>;
};

template<typename T, size_t E>
struct ContainerAccessDecider<QSpan<T,E>> : std::true_type {
    using type = QSpanAccess<T,E>;
};

template<typename K, typename T>
struct ContainerAccessDecider<QMap<K,T>> : std::true_type {
    using type = QMapAccess<K,T>;
};

template<typename K, typename T>
struct ContainerAccessDecider<QHash<K,T>> : std::true_type {
    using type = QHashAccess<K,T>;
};

template<typename K, typename T>
struct ContainerAccessDecider<QMultiMap<K,T>> : std::true_type {
    using type = QMultiMapAccess<K,T>;
};

template<typename K, typename T>
struct ContainerAccessDecider<QMultiHash<K,T>> : std::true_type {
    using type = QMultiHashAccess<K,T>;
};

template<typename Iterator, typename Container, bool = QtJambiPrivate::supports_sentinel_v<Container>>
struct is_sentinel : std::false_type{
};

template<typename Iterator, typename Container>
struct is_sentinel<Iterator, Container, true> : std::is_same<Iterator,typename Container::sentinel>{
};

template<typename Container, typename Iterator, typename Storage>
struct is_sentinel<ContainerIterator<Container,Iterator,Storage>, Container, true> : std::is_same<Iterator,typename Container::sentinel>{
};

template<typename Iterator, typename Container>
constexpr bool is_sentinel_v = is_sentinel<Iterator,Container>::value;

template<typename Iterator, typename Container, bool = QtJambiPrivate::supports_reverse_iterator_v<Container>>
struct is_reverse_iterator : std::false_type{
};

template<typename Iterator, typename Container>
struct is_reverse_iterator<Iterator, Container, true> : std::is_same<Iterator,typename Container::reverse_iterator>{
};

template<typename Container, typename Iterator, typename Storage>
struct is_reverse_iterator<ContainerIterator<Container,Iterator,Storage>, Container, true> : std::is_same<Iterator,typename Container::reverse_iterator>{
};

template<typename Iterator, typename Container>
constexpr bool is_reverse_iterator_v = is_reverse_iterator<Iterator,Container>::value;

template<typename Iterator, typename Container, bool = QtJambiPrivate::supports_const_reverse_iterator_v<Container>>
struct is_const_reverse_iterator : std::false_type{
};

template<typename Iterator, typename Container>
struct is_const_reverse_iterator<Iterator, Container, true> : std::is_same<Iterator,typename Container::const_reverse_iterator>{
};

template<typename Container, typename Iterator, typename Storage>
struct is_const_reverse_iterator<ContainerIterator<Container,Iterator,Storage>, Container, true> : std::is_same<Iterator,typename Container::const_reverse_iterator>{
};

template<typename Iterator, typename Container>
constexpr bool is_const_reverse_iterator_v = is_const_reverse_iterator<Iterator,Container>::value;

template<typename Iterator, typename Container, bool = QtJambiPrivate::supports_key_iterator_v<Container>>
struct is_key_iterator : std::false_type{
};

template<typename Iterator, typename Container>
struct is_key_iterator<Iterator, Container, true> : std::is_same<Iterator,typename Container::key_iterator>{
};

template<typename Container, typename Iterator, typename Storage>
struct is_key_iterator<ContainerIterator<Container,Iterator,Storage>, Container, true> : std::is_same<Iterator,typename Container::key_iterator>{
};

template<typename Iterator, typename Container>
constexpr bool is_key_iterator_v = is_key_iterator<Iterator,Container>::value;

template<typename Iterator, typename Container, bool = QtJambiPrivate::supports_key_value_iterator_v<Container>>
struct is_key_value_iterator : std::false_type{
};

template<typename Iterator, typename Container>
struct is_key_value_iterator<Iterator, Container, true> : std::is_same<Iterator,typename Container::key_value_iterator>{
};

template<typename Container, typename Iterator, typename Storage>
struct is_key_value_iterator<ContainerIterator<Container,Iterator,Storage>, Container, true> : std::is_same<Iterator,typename Container::key_value_iterator>{
};

template<typename Iterator, typename Container>
constexpr bool is_key_value_iterator_v = is_key_value_iterator<Iterator,Container>::value;

template<typename Iterator, typename Container, bool = QtJambiPrivate::supports_const_key_value_iterator_v<Container>>
struct is_const_key_value_iterator : std::false_type{
};

template<typename Iterator, typename Container>
struct is_const_key_value_iterator<Iterator, Container, true> : std::is_same<Iterator,typename Container::const_key_value_iterator>{
};

template<typename Container, typename Iterator, typename Storage>
struct is_const_key_value_iterator<ContainerIterator<Container,Iterator,Storage>, Container, true> : std::is_same<Iterator,typename Container::const_key_value_iterator>{
};

template<typename Iterator, typename Container>
constexpr bool is_const_key_value_iterator_v = is_const_key_value_iterator<Iterator,Container>::value;

template<typename Iterator, bool isMutable>
struct IteratorTypeDecider{
private:
    static constexpr AbstractSequentialConstIteratorAccess::IteratorType getType(){
        if constexpr(isMutable){
            return AbstractSequentialConstIteratorAccess::IteratorType::iterator;
        }else{
            return AbstractSequentialConstIteratorAccess::IteratorType::const_iterator;
        }
    }
public:
    static constexpr AbstractSequentialConstIteratorAccess::IteratorType type = getType();
};

template<typename Container, typename Iter, typename Storage, bool isMutable>
struct IteratorTypeDecider<ContainerIterator<Container,Iter,Storage>,isMutable>{
private:
    static constexpr AbstractSequentialConstIteratorAccess::IteratorType getType(){
        if constexpr(is_sentinel_v<Iter,Container>){
            return AbstractSequentialConstIteratorAccess::IteratorType::sentinel;
        }else if constexpr(is_const_key_value_iterator_v<Iter,Container>){
            return AbstractSequentialConstIteratorAccess::IteratorType::const_key_value_iterator;
        }else if constexpr(is_key_value_iterator_v<Iter,Container>){
            return AbstractSequentialConstIteratorAccess::IteratorType::key_value_iterator;
        }else if constexpr(is_key_iterator_v<Iter,Container>){
            return AbstractSequentialConstIteratorAccess::IteratorType::key_iterator;
        }else if constexpr(is_const_reverse_iterator_v<Iter,Container>){
            return AbstractSequentialConstIteratorAccess::IteratorType::const_reverse_iterator;
        }else if constexpr(is_reverse_iterator_v<Iter,Container>){
            return AbstractSequentialConstIteratorAccess::IteratorType::reverse_iterator;
        }else if constexpr(isMutable){
            return AbstractSequentialConstIteratorAccess::IteratorType::iterator;
        }else{
            return AbstractSequentialConstIteratorAccess::IteratorType::const_iterator;
        }
    }
public:
    static constexpr AbstractSequentialConstIteratorAccess::IteratorType type = getType();
};

template<typename Container, typename Iterator>
struct StorageTypeDecider{
    static constexpr AbstractSequentialConstIteratorAccess::IteratorStorage type = AbstractSequentialConstIteratorAccess::IteratorStorage::Clone;
};

template<typename Container, typename Iterator>
struct StorageTypeDecider<Container,ContainerIterator<Container,Iterator,ContainerRef<Container>>>{
    static constexpr AbstractSequentialConstIteratorAccess::IteratorStorage type = AbstractSequentialConstIteratorAccess::IteratorStorage::Ref;
};

template<typename Container, typename Iterator, bool isMutable, bool is_associative, AbstractSequentialConstIteratorAccess::IteratorStorage storageType>
struct IteratorComparator{
    using IteratorType = AbstractSequentialConstIteratorAccess::IteratorType;
    static constexpr jboolean equals(JNIEnv *, const void* ptr, const ConstContainerAndAccessInfo& ptr2){
        const Iterator& iterator = *reinterpret_cast<const Iterator*>(ptr);
        if(ptr2.access->isSequentialConstIterator() && ptr2.access->isTemplateAccess()){
            AbstractSequentialConstIteratorAccess* iterAccess = static_cast<AbstractSequentialConstIteratorAccess*>(ptr2.access);
            if(IteratorTypeDecider<Iterator,isMutable>::type==iterAccess->iteratorType()){
                if constexpr(supports_equal_v<const Iterator&, const Iterator&>)
                    return iterator==*reinterpret_cast<const Iterator*>(ptr2.container);
                else return false;
            }else{
                switch(iterAccess->iteratorType()){
                case IteratorType::iterator:
                    if constexpr(QtJambiPrivate::supports_iterator_v<Container>
                                  && IteratorTypeDecider<Iterator,isMutable>::type!=AbstractSequentialConstIteratorAccess::IteratorType::reverse_iterator
                                  && IteratorTypeDecider<Iterator,isMutable>::type!=AbstractSequentialConstIteratorAccess::IteratorType::const_reverse_iterator){
                        switch(iterAccess->iteratorStorage()){
                        case AbstractSequentialConstIteratorAccess::IteratorStorage::Ref:{
                            using Iter2 = ContainerIterator<Container, typename Container::iterator, ContainerRef<Container>>;
                            return iterator_equals<Container,Iterator,Iter2>::function(iterator, *reinterpret_cast<const Iter2*>(ptr2.container));
                        } break;
                        default:{
                            using Iter2 = ContainerIterator<Container, typename Container::iterator, Container>;
                            return iterator_equals<Container,Iterator,Iter2>::function(iterator, *reinterpret_cast<const Iter2*>(ptr2.container));
                        } break;
                        }
                    }
                    break;
                case IteratorType::const_iterator:
                    if constexpr(QtJambiPrivate::supports_const_iterator_v<Container>
                                  && IteratorTypeDecider<Iterator,isMutable>::type!=AbstractSequentialConstIteratorAccess::IteratorType::reverse_iterator
                                  && IteratorTypeDecider<Iterator,isMutable>::type!=AbstractSequentialConstIteratorAccess::IteratorType::const_reverse_iterator){
                        switch(iterAccess->iteratorStorage()){
                        case AbstractSequentialConstIteratorAccess::IteratorStorage::Ref:{
                            using Iter2 = ContainerIterator<Container, typename Container::const_iterator, ContainerRef<Container>>;
                            return iterator_equals<Container,Iterator,Iter2>::function(iterator, *reinterpret_cast<const Iter2*>(ptr2.container));
                        } break;
                        default:{
                            using Iter2 = ContainerIterator<Container,typename Container::const_iterator>;
                            return iterator_equals<Container,Iterator,Iter2>::function(iterator, *reinterpret_cast<const Iter2*>(ptr2.container));
                        } break;
                        }
                    }break;
                case IteratorType::reverse_iterator:
                    if constexpr(QtJambiPrivate::supports_reverse_iterator_v<Container>
                                  && IteratorTypeDecider<Iterator,isMutable>::type!=AbstractSequentialConstIteratorAccess::IteratorType::iterator
                                  && IteratorTypeDecider<Iterator,isMutable>::type!=AbstractSequentialConstIteratorAccess::IteratorType::const_iterator){
                        switch(iterAccess->iteratorStorage()){
                        case AbstractSequentialConstIteratorAccess::IteratorStorage::Ref:{
                            using Iter2 = ContainerIterator<Container, typename Container::reverse_iterator, ContainerRef<Container>>;
                            return iterator_equals<Container,Iterator,Iter2>::function(iterator, *reinterpret_cast<const Iter2*>(ptr2.container));
                        } break;
                        default:{
                            using Iter2 = ContainerIterator<Container,typename Container::reverse_iterator>;
                            return iterator_equals<Container,Iterator,Iter2>::function(iterator, *reinterpret_cast<const Iter2*>(ptr2.container));
                        } break;
                        }
                    }
                    break;
                case IteratorType::const_reverse_iterator:
                    if constexpr(QtJambiPrivate::supports_const_reverse_iterator_v<Container>
                                  && IteratorTypeDecider<Iterator,isMutable>::type!=AbstractSequentialConstIteratorAccess::IteratorType::iterator
                                  && IteratorTypeDecider<Iterator,isMutable>::type!=AbstractSequentialConstIteratorAccess::IteratorType::const_iterator){
                        switch(iterAccess->iteratorStorage()){
                        case AbstractSequentialConstIteratorAccess::IteratorStorage::Ref:{
                            using Iter2 = ContainerIterator<Container, typename Container::const_reverse_iterator, ContainerRef<Container>>;
                            return iterator_equals<Container,Iterator,Iter2>::function(iterator, *reinterpret_cast<const Iter2*>(ptr2.container));
                        } break;
                        default:{
                            using Iter2 = ContainerIterator<Container,typename Container::const_reverse_iterator>;
                            return iterator_equals<Container,Iterator,Iter2>::function(iterator, *reinterpret_cast<const Iter2*>(ptr2.container));
                        } break;
                        }
                    }
                    break;
                case IteratorType::key_iterator:
                    if constexpr(QtJambiPrivate::supports_key_iterator_v<Container>){
                        switch(iterAccess->iteratorStorage()){
                        case AbstractSequentialConstIteratorAccess::IteratorStorage::Ref:{
                            using Iter2 = ContainerIterator<Container, typename Container::key_iterator, ContainerRef<Container>>;
                            return iterator_equals<Container,Iterator,Iter2>::function(iterator, *reinterpret_cast<const Iter2*>(ptr2.container));
                        } break;
                        default:{
                            using Iter2 = ContainerIterator<Container,typename Container::key_iterator>;
                            return iterator_equals<Container,Iterator,Iter2>::function(iterator, *reinterpret_cast<const Iter2*>(ptr2.container));
                        } break;
                        }
                    }
                    break;
                case IteratorType::key_value_iterator:
                    if constexpr(QtJambiPrivate::supports_key_value_iterator_v<Container>){
                        switch(iterAccess->iteratorStorage()){
                        case AbstractSequentialConstIteratorAccess::IteratorStorage::Ref:{
                            using Iter2 = ContainerIterator<Container, typename Container::key_value_iterator, ContainerRef<Container>>;
                            return iterator_equals<Container,Iterator,Iter2>::function(iterator, *reinterpret_cast<const Iter2*>(ptr2.container));
                        } break;
                        default:{
                            using Iter2 = ContainerIterator<Container,typename Container::key_value_iterator>;
                            return iterator_equals<Container,Iterator,Iter2>::function(iterator, *reinterpret_cast<const Iter2*>(ptr2.container));
                        } break;
                        }
                    }
                    break;
                case IteratorType::const_key_value_iterator:
                    if constexpr(QtJambiPrivate::supports_const_key_value_iterator_v<Container>){
                        switch(iterAccess->iteratorStorage()){
                        case AbstractSequentialConstIteratorAccess::IteratorStorage::Ref:{
                            using Iter2 = ContainerIterator<Container, typename Container::const_key_value_iterator, ContainerRef<Container>>;
                            return iterator_equals<Container,Iterator,Iter2>::function(iterator, *reinterpret_cast<const Iter2*>(ptr2.container));
                        } break;
                        default:{
                            using Iter2 = ContainerIterator<Container,typename Container::const_key_value_iterator>;
                            return iterator_equals<Container,Iterator,Iter2>::function(iterator, *reinterpret_cast<const Iter2*>(ptr2.container));
                        } break;
                        }
                    }
                    break;
                case IteratorType::sentinel:
                    if constexpr(QtJambiPrivate::supports_sentinel_v<Container>){
                        switch(iterAccess->iteratorStorage()){
                        case AbstractSequentialConstIteratorAccess::IteratorStorage::Ref:{
                            using Iter2 = ContainerIterator<Container, typename Container::sentinel, ContainerRef<Container>>;
                            return iterator_equals<Container,Iterator,Iter2>::function(iterator, *reinterpret_cast<const Iter2*>(ptr2.container));
                        } break;
                        default:{
                            using Iter2 = ContainerIterator<Container,typename Container::sentinel>;
                            return iterator_equals<Container,Iterator,Iter2>::function(iterator, *reinterpret_cast<const Iter2*>(ptr2.container));
                        } break;
                        }
                    }
                    break;
                }
            }
        }
        Q_UNUSED(ptr)
        Q_UNUSED(ptr2)
        return false;
    }
};

template<typename Iterator, bool isMutable, bool is_associative, AbstractSequentialConstIteratorAccess::IteratorStorage storageType>
struct IteratorComparator<QtJambiNativeID, Iterator, isMutable, is_associative, storageType>{
    static constexpr jboolean equals(JNIEnv *, const void*, const ConstContainerAndAccessInfo&){ return false; }
};

template<typename Iterator, bool isMutable, bool is_associative, AbstractSequentialConstIteratorAccess::IteratorStorage storageType>
struct IteratorComparator<jobject, Iterator, isMutable, is_associative, storageType>{
    static constexpr jboolean equals(JNIEnv *, const void*, const ConstContainerAndAccessInfo&){ return false; }
};

template<typename Iterator, typename Container, typename SuperType = AbstractSequentialConstIteratorAccess>
struct AbstractConstIteratorAccess : SuperType{
    using container_type = Container;
    AbstractContainerAccess::ContainerType containerType() const override {
        return AbstractContainerAccess::ContainerType(SuperType::containerType() | AbstractContainerAccess::TemplateAccess);
    }
    AbstractSequentialConstIteratorAccess::IteratorType iteratorType() const override {
        return IteratorTypeDecider<Iterator,false>::type;
    }
    AbstractSequentialConstIteratorAccess::IteratorStorage iteratorStorage() const override {
        return StorageTypeDecider<Container,Iterator>::type;
    }
    void dispose() final override {}
    void advance(JNIEnv *env, void* iterator, qsizetype n) override {
        IteratorAdvance<Iterator>::function(env, iterator, n);
    }
    bool advance(void* iterator, qsizetype n) override {
        return IteratorAdvance<Iterator>::function(iterator, n);
    }
    void increment(JNIEnv *env, void* iterator) override {
        IteratorIncrement<Iterator>::function(env, iterator);
    }
    void increment(void* iterator) override {
        IteratorIncrement<Iterator>::function(iterator);
    }
    void decrement(JNIEnv *env, void* iterator) override {
        IteratorDecrement<Iterator>::function(env, iterator);
    }
    void decrement(void* iterator) override {
        IteratorDecrement<Iterator>::function(iterator);
    }
    bool isBidirectionalIterator() override {
        return IteratorDecrement<Iterator>::value;
    }
    jboolean lessThan(JNIEnv *env, const void* iterator, const void* other) override {
        return IteratorLessThan<Iterator>::function(env, iterator, other);
    }
    std::optional<bool> lessThan(const void* iterator, const void* other) override {
        return IteratorLessThan<Iterator>::function(iterator, other);
    }
    std::optional<size_t> distance(const void* iterator, const void* other) override {
        return IteratorDistance<Iterator>::function(iterator, other);
    }
    bool canDistance() override {
        return IteratorDistance<Iterator>::value;
    }
    bool canLess() override {
        return IteratorLessThan<Iterator>::value;
    }
    bool isBegin(JNIEnv *env, const void* iterator) override {
        return IteratorIsBegin<Iterator>::function(env, iterator);
    }
    std::optional<bool> isBegin(const void* iterator) override {
        return IteratorIsBegin<Iterator>::function(iterator);
    }
    bool isEnd(JNIEnv *env, const void* iterator) override {
        return IteratorIsEnd<Iterator>::function(env, iterator);
    }
    std::optional<bool> isEnd(const void* iterator) override {
        return IteratorIsEnd<Iterator>::function(iterator);
    }
    bool isValid(JNIEnv *env, const void* iterator) override {
        return IteratorIsValid<Iterator>::function(env, iterator);
    }
    std::optional<bool> isValid(const void* iterator) override {
        return IteratorIsValid<Iterator>::function(iterator);
    }
    jboolean equals(JNIEnv *env, const void* ptr, const void* ptr2) override {
        return IteratorEqual<Iterator>::function(env, ptr, ptr2);
    }
    bool equals(const void* ptr, const void* ptr2) override {
        return IteratorEqual<Iterator>::function(ptr, ptr2).value_or(false);
    }
};

} // namespace QtJambiPrivate

template<typename Iterator, typename Container, typename SuperType = AbstractSequentialConstIteratorAccess>
class QSequentialConstIteratorAccess : public QtJambiPrivate::AbstractConstIteratorAccess<Iterator,Container,SuperType>{
protected:
    QSequentialConstIteratorAccess(){}
    AbstractSequentialConstIteratorAccess::IteratorType iteratorType() const override {
        return QtJambiPrivate::IteratorTypeDecider<Iterator,false>::type;
    }
    AbstractSequentialConstIteratorAccess::IteratorStorage iteratorStorage() const override {
        return QtJambiPrivate::StorageTypeDecider<Container,Iterator>::type;
    }
public:
    using SuperType::value;
    using QtJambiPrivate::AbstractConstIteratorAccess<Iterator,Container,SuperType>::equals;
    static QSequentialConstIteratorAccess<Iterator,Container,SuperType>* newInstance(){
        static QSequentialConstIteratorAccess<Iterator,Container,SuperType> instance;
        return &instance;
    }

    QSequentialConstIteratorAccess<Iterator,Container,SuperType>* clone() override{
        return this;
    }

    jobject value(JNIEnv * env, const void* ptr) override {
        return QtJambiPrivate::IteratorSequentialValue<Iterator,true>::function(env, ptr);
    }
    std::optional<const void*> value(const void* ptr) override {
        return QtJambiPrivate::IteratorSequentialValueOptional<Iterator,const void*>::function(ptr);
    }
    std::optional<jint> intValue(const void* ptr) override {
        return QtJambiPrivate::IteratorSequentialValueOptional<Iterator,jint>::function(ptr);
    }
    std::optional<jlong> longValue(const void* ptr) override {
        return QtJambiPrivate::IteratorSequentialValueOptional<Iterator,jlong>::function(ptr);
    }
    std::optional<jshort> shortValue(const void* ptr) override {
        return QtJambiPrivate::IteratorSequentialValueOptional<Iterator,jshort>::function(ptr);
    }
    std::optional<jbyte> byteValue(const void* ptr) override {
        return QtJambiPrivate::IteratorSequentialValueOptional<Iterator,jbyte>::function(ptr);
    }
    std::optional<jfloat> floatValue(const void* ptr) override {
        return QtJambiPrivate::IteratorSequentialValueOptional<Iterator,jfloat>::function(ptr);
    }
    std::optional<jdouble> doubleValue(const void* ptr) override {
        return QtJambiPrivate::IteratorSequentialValueOptional<Iterator,jdouble>::function(ptr);
    }
    std::optional<jchar> charValue(const void* ptr) override {
        return QtJambiPrivate::IteratorSequentialValueOptional<Iterator,jchar>::function(ptr);
    }
    std::optional<jboolean> booleanValue(const void* ptr) override {
        return QtJambiPrivate::IteratorSequentialValueOptional<Iterator,jboolean>::function(ptr);
    }
    QVariant variantValue(const void* ptr) override {
        return QtJambiPrivate::IteratorSequentialValueOptional<Iterator,QVariant>::function(ptr);
    }
    const QMetaType& valueMetaType() override{
        return QtJambiPrivate::IteratorSequentialValueType<Iterator>::function();
    }
    void* asIterator(void* iterator) override{
        return QtJambiPrivate::AsIterator<Iterator,Container>::function(iterator);
    }
    bool findIterator(const void* iterator, const std::type_info& typeId, void* output) override{
        return QtJambiPrivate::IteratorCastOptional<Iterator,Container>::function(iterator, typeId, output);
    }
    void assign(void* iterator, const void* other) override{
        if constexpr(QtJambiPrivate::supports_assign_v<Iterator&,const Iterator&>){
            *static_cast<Iterator*>(iterator) = *static_cast<const Iterator*>(other);
        }
        Q_UNUSED(iterator)
        Q_UNUSED(other)
    }
    void* constructContainer(void* placement) override{
        if constexpr(QtJambiPrivate::is_default_constructible_v<Iterator>){
            return new(placement)Iterator();
        }else{
            Q_UNUSED(placement)
            return nullptr;
        }
    }
    void* constructContainer(void* placement, const void* copyOf) override{
        if constexpr(QtJambiPrivate::is_copy_constructible_v<Iterator>){
            return new(placement)Iterator(*static_cast<const Iterator*>(copyOf));
        }else{
            Q_UNUSED(placement)
            Q_UNUSED(copyOf)
            return nullptr;
        }
    }
    void* constructContainer(void* placement,void* moveOf) override{
        if constexpr(QtJambiPrivate::is_move_constructible_v<Iterator>){
            return new(placement)Iterator(std::move(*static_cast<Iterator*>(moveOf)));
        }else{
            Q_UNUSED(placement)
            Q_UNUSED(moveOf)
            return nullptr;
        }
    }
    bool destructContainer(void* ptr) override {
        static_cast<const Iterator*>(ptr)->~Iterator();
        return true;
    }
    size_t sizeOf() const override { return sizeof(Iterator); }
    size_t alignOf() const override { return alignof(Iterator); }
    bool canCopy() const override { return QtJambiPrivate::is_copy_constructible_v<Iterator>; }

    jboolean equals(JNIEnv *, const void* ptr, const ConstContainerAndAccessInfo& ptr2) override;
    bool isContiguousIterator() override{
        return std::is_pointer_v<Iterator>;
    }
    bool isRandomAccessIterator() override{
        return QtJambiPrivate::is_random_access_iterator_v<Iterator>;
    }
    std::pair<void*,AbstractSequentialConstIteratorAccess*> createConstIterator(const void* iterator) override {
        if constexpr(std::is_same_v<typename QtJambiPrivate::CreateConstIterator<Iterator,Container>::type,void>){
            return {nullptr,nullptr};
        }else{
            return {
                QtJambiPrivate::CreateConstIterator<Iterator,Container>::function(iterator),
                QSequentialConstIteratorAccess<typename QtJambiPrivate::CreateConstIterator<Iterator,Container>::type,Container>::newInstance()
            };
        }
    }
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    AbstractSpanAccess* createSpanAccess() override;
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)
};

template<typename Iterator, typename Container>
class QSequentialIteratorAccess : public QSequentialConstIteratorAccess<Iterator,Container,AbstractSequentialIteratorAccess>{
protected:
    AbstractSequentialConstIteratorAccess::IteratorType iteratorType() const override {
        return QtJambiPrivate::IteratorTypeDecider<Iterator,true>::type;
    }
    AbstractSequentialConstIteratorAccess::IteratorStorage iteratorStorage() const override {
        return QtJambiPrivate::StorageTypeDecider<Container,Iterator>::type;
    }
private:
    QSequentialIteratorAccess(){}
public:
    using QSequentialConstIteratorAccess<Iterator,Container,AbstractSequentialIteratorAccess>::value;
    using QSequentialConstIteratorAccess<Iterator,Container,AbstractSequentialIteratorAccess>::equals;
    static QSequentialIteratorAccess<Iterator,Container>* newInstance(){
        static QSequentialIteratorAccess<Iterator,Container> instance;
        return &instance;
    }
    QSequentialIteratorAccess<Iterator,Container>* clone() override{
        return this;
    }
    void setValue(JNIEnv * env, void* ptr, jobject newValue) override {
        QtJambiPrivate::IteratorSequentialValue<Iterator,false>::function(env, ptr, newValue);
    }
    std::optional<void*> value(void* ptr) override {
        return QtJambiPrivate::IteratorSequentialValueOptional<Iterator,void*>::function(ptr);
    }
    bool setIntValue(void* ptr, jint value) override {
        return QtJambiPrivate::IteratorSequentialSetValue<Iterator>::function(ptr, value);
    }
    bool setLongValue(void* ptr, jlong value) override {
        return QtJambiPrivate::IteratorSequentialSetValue<Iterator>::function(ptr, value);
    }
    bool setShortValue(void* ptr, jshort value) override {
        return QtJambiPrivate::IteratorSequentialSetValue<Iterator>::function(ptr, value);
    }
    bool setByteValue(void* ptr, jbyte value) override {
        return QtJambiPrivate::IteratorSequentialSetValue<Iterator>::function(ptr, value);
    }
    bool setFloatValue(void* ptr, jfloat value) override {
        return QtJambiPrivate::IteratorSequentialSetValue<Iterator>::function(ptr, value);
    }
    bool setDoubleValue(void* ptr, jdouble value) override {
        return QtJambiPrivate::IteratorSequentialSetValue<Iterator>::function(ptr, value);
    }
    bool setCharValue(void* ptr, jchar value) override {
        return QtJambiPrivate::IteratorSequentialSetValue<Iterator>::function(ptr, value);
    }
    bool setBooleanValue(void* ptr, jboolean value) override {
        return QtJambiPrivate::IteratorSequentialSetValue<Iterator>::function(ptr, value);
    }
    bool setVariantValue(void* ptr, const QVariant& value) override {
        return QtJambiPrivate::IteratorSequentialSetValue<Iterator>::function(ptr, value);
    }
    jboolean equals(JNIEnv *, const void* ptr, const ConstContainerAndAccessInfo& ptr2) override;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    AbstractSpanAccess* createSpanAccess() override;
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)
};

template<typename Iterator, typename Container, typename SuperType>
jboolean QSequentialConstIteratorAccess<Iterator,Container,SuperType>::equals(JNIEnv *env, const void* ptr, const ConstContainerAndAccessInfo& ptr2) {
    if constexpr(std::is_void_v<Container>){
        return *reinterpret_cast<const Iterator*>(ptr)==*reinterpret_cast<const Iterator*>(ptr2.container);
    }else{
        return QtJambiPrivate::IteratorComparator<Container,Iterator,false,false,QtJambiPrivate::StorageTypeDecider<Container,Iterator>::type>::equals(env, ptr, ptr2);
    }
}

template<typename Iterator, typename Container>
jboolean QSequentialIteratorAccess<Iterator,Container>::equals(JNIEnv * env, const void* ptr, const ConstContainerAndAccessInfo& ptr2) {
    if constexpr(std::is_void_v<Container>){
        return *reinterpret_cast<const Iterator*>(ptr)==*reinterpret_cast<const Iterator*>(ptr2.container);
    }else{
        return QtJambiPrivate::IteratorComparator<Container,Iterator,true,false,QtJambiPrivate::StorageTypeDecider<Container,Iterator>::type>::equals(env, ptr, ptr2);
    }
}


template<typename Iterator, typename Container, typename SuperType = AbstractAssociativeConstIteratorAccess>
class QAssociativeConstIteratorAccess : public QtJambiPrivate::AbstractConstIteratorAccess<Iterator,Container,SuperType>{
protected:
    QAssociativeConstIteratorAccess(){}
    AbstractSequentialConstIteratorAccess::IteratorType iteratorType() const override {
        return QtJambiPrivate::IteratorTypeDecider<Iterator,false>::type;
    }
    AbstractSequentialConstIteratorAccess::IteratorStorage iteratorStorage() const override {
        return QtJambiPrivate::StorageTypeDecider<Container,Iterator>::type;
    }
public:
    using SuperType::value;
    using QtJambiPrivate::AbstractConstIteratorAccess<Iterator,Container,SuperType>::equals;
    static QAssociativeConstIteratorAccess<Iterator,Container,SuperType>* newInstance(){
        static QAssociativeConstIteratorAccess<Iterator,Container,SuperType> instance;
        return &instance;
    }

    QAssociativeConstIteratorAccess<Iterator,Container,SuperType>* clone() override{
        return this;
    }

    jobject value(JNIEnv * env, const void* ptr) override {
        if constexpr(QtJambiPrivate::IteratorTypeDecider<Iterator,false>::type==AbstractSequentialConstIteratorAccess::IteratorType::key_iterator)
            return key(env, ptr);
        else
            return QtJambiPrivate::IteratorAssociativeValue<Iterator,true>::function(env, ptr);
    }
    std::optional<const void*> value(const void* ptr) override {
        if constexpr(QtJambiPrivate::IteratorTypeDecider<Iterator,false>::type==AbstractSequentialConstIteratorAccess::IteratorType::key_iterator)
            return key(ptr);
        else
            return QtJambiPrivate::IteratorAssociativeValueOptional<Iterator,const void*>::function(ptr);
    }
    std::optional<jint> intValue(const void* ptr) override {
        if constexpr(QtJambiPrivate::IteratorTypeDecider<Iterator,false>::type==AbstractSequentialConstIteratorAccess::IteratorType::key_iterator)
            return intKey(ptr);
        else
            return QtJambiPrivate::IteratorAssociativeValueOptional<Iterator,jint>::function(ptr);
    }
    std::optional<jlong> longValue(const void* ptr) override {
        if constexpr(QtJambiPrivate::IteratorTypeDecider<Iterator,false>::type==AbstractSequentialConstIteratorAccess::IteratorType::key_iterator)
            return longKey(ptr);
        else
            return QtJambiPrivate::IteratorAssociativeValueOptional<Iterator,jlong>::function(ptr);
    }
    std::optional<jshort> shortValue(const void* ptr) override {
        if constexpr(QtJambiPrivate::IteratorTypeDecider<Iterator,false>::type==AbstractSequentialConstIteratorAccess::IteratorType::key_iterator)
            return shortKey(ptr);
        else
            return QtJambiPrivate::IteratorAssociativeValueOptional<Iterator,jshort>::function(ptr);
    }
    std::optional<jbyte> byteValue(const void* ptr) override {
        if constexpr(QtJambiPrivate::IteratorTypeDecider<Iterator,false>::type==AbstractSequentialConstIteratorAccess::IteratorType::key_iterator)
            return byteKey(ptr);
        else
            return QtJambiPrivate::IteratorAssociativeValueOptional<Iterator,jbyte>::function(ptr);
    }
    std::optional<jfloat> floatValue(const void* ptr) override {
        if constexpr(QtJambiPrivate::IteratorTypeDecider<Iterator,false>::type==AbstractSequentialConstIteratorAccess::IteratorType::key_iterator)
            return doubleKey(ptr);
        else
            return QtJambiPrivate::IteratorAssociativeValueOptional<Iterator,jfloat>::function(ptr);
    }
    std::optional<jdouble> doubleValue(const void* ptr) override {
        if constexpr(QtJambiPrivate::IteratorTypeDecider<Iterator,false>::type==AbstractSequentialConstIteratorAccess::IteratorType::key_iterator)
            return doubleKey(ptr);
        else
            return QtJambiPrivate::IteratorAssociativeValueOptional<Iterator,jdouble>::function(ptr);
    }
    std::optional<jchar> charValue(const void* ptr) override {
        if constexpr(QtJambiPrivate::IteratorTypeDecider<Iterator,false>::type==AbstractSequentialConstIteratorAccess::IteratorType::key_iterator)
            return QtJambiPrivate::IteratorAssociativeKeyOptional<Iterator,const void*>::function(ptr);
        else
            return QtJambiPrivate::IteratorAssociativeValueOptional<Iterator,jchar>::function(ptr);
    }
    std::optional<jboolean> booleanValue(const void* ptr) override {
        if constexpr(QtJambiPrivate::IteratorTypeDecider<Iterator,false>::type==AbstractSequentialConstIteratorAccess::IteratorType::key_iterator)
            return booleanKey(ptr);
        else
            return QtJambiPrivate::IteratorAssociativeValueOptional<Iterator,jboolean>::function(ptr);
    }
    QVariant variantValue(const void* ptr) override {
        if constexpr(QtJambiPrivate::IteratorTypeDecider<Iterator,false>::type==AbstractSequentialConstIteratorAccess::IteratorType::key_iterator)
            return variantKey(ptr);
        else
            return QtJambiPrivate::IteratorAssociativeValueOptional<Iterator,QVariant>::function(ptr);
    }
    jobject key(JNIEnv * env, const void* ptr) override {
        return QtJambiPrivate::IteratorAssociativeKey<Iterator>::function(env, ptr);
    }
    std::optional<const void*> key(const void* ptr) override {
        return QtJambiPrivate::IteratorAssociativeKeyOptional<Iterator,const void*>::function(ptr);
    }
    std::optional<jint> intKey(const void* ptr) override {
        return QtJambiPrivate::IteratorAssociativeKeyOptional<Iterator,jint>::function(ptr);
    }
    std::optional<jlong> longKey(const void* ptr) override {
        return QtJambiPrivate::IteratorAssociativeKeyOptional<Iterator,jlong>::function(ptr);
    }
    std::optional<jshort> shortKey(const void* ptr) override {
        return QtJambiPrivate::IteratorAssociativeKeyOptional<Iterator,jshort>::function(ptr);
    }
    std::optional<jbyte> byteKey(const void* ptr) override {
        return QtJambiPrivate::IteratorAssociativeKeyOptional<Iterator,jbyte>::function(ptr);
    }
    std::optional<jfloat> floatKey(const void* ptr) override {
        return QtJambiPrivate::IteratorAssociativeKeyOptional<Iterator,jfloat>::function(ptr);
    }
    std::optional<jdouble> doubleKey(const void* ptr) override {
        return QtJambiPrivate::IteratorAssociativeKeyOptional<Iterator,jdouble>::function(ptr);
    }
    std::optional<jchar> charKey(const void* ptr) override {
        return QtJambiPrivate::IteratorAssociativeKeyOptional<Iterator,jchar>::function(ptr);
    }
    std::optional<jboolean> booleanKey(const void* ptr) override {
        return QtJambiPrivate::IteratorAssociativeKeyOptional<Iterator,jboolean>::function(ptr);
    }
    QVariant variantKey(const void* ptr) override {
        return QtJambiPrivate::IteratorAssociativeKeyOptional<Iterator,QVariant>::function(ptr);
    }

    const QMetaType& keyMetaType() override{
        return QtJambiPrivate::IteratorAssociativeKeyType<Iterator>::function();
    }

    const QMetaType& valueMetaType() override{
        if constexpr(QtJambiPrivate::IteratorTypeDecider<Iterator,false>::type==AbstractSequentialConstIteratorAccess::IteratorType::key_iterator)
            return QtJambiPrivate::IteratorAssociativeKeyType<Iterator>::function();
        else
            return QtJambiPrivate::IteratorAssociativeValueType<Iterator>::function();
    }
    void* asIterator(void* iterator) override{
        return QtJambiPrivate::AsIterator<Iterator,Container>::function(iterator);
    }
    bool findIterator(const void* iterator, const std::type_info& typeId, void* output) override{
        return QtJambiPrivate::IteratorCastOptional<Iterator,Container>::function(iterator, typeId, output);
    }
    std::pair<void*,AbstractSequentialConstIteratorAccess*> createConstIterator(const void* iterator) override {
        if constexpr(std::is_same_v<typename QtJambiPrivate::CreateConstIterator<Iterator,Container>::type,void>){
            return {nullptr,nullptr};
        }else{
            return {
                QtJambiPrivate::CreateConstIterator<Iterator,Container>::function(iterator),
                QAssociativeConstIteratorAccess<typename QtJambiPrivate::CreateConstIterator<Iterator,Container>::type,Container>::newInstance()
            };
        }
    }
    void* constructContainer(void* placement, const void* copyOf) override{
        if constexpr(QtJambiPrivate::is_copy_constructible_v<Iterator>){
            return new(placement)Iterator(*static_cast<const Iterator*>(copyOf));
        }else{
            Q_UNUSED(placement)
            Q_UNUSED(copyOf)
            return nullptr;
        }
    }
    size_t sizeOf() const override { return sizeof(Iterator); }
    size_t alignOf() const override { return alignof(Iterator); }
    bool canCopy() const override { return QtJambiPrivate::is_copy_constructible_v<Iterator>; }
    jboolean equals(JNIEnv *, const void* ptr, const ConstContainerAndAccessInfo& ptr2) override;
};

template<typename Iterator, typename Container>
class QAssociativeIteratorAccess : public QAssociativeConstIteratorAccess<Iterator,Container,AbstractAssociativeIteratorAccess> {
protected:
    AbstractSequentialConstIteratorAccess::IteratorType iteratorType() const override {
        return QtJambiPrivate::IteratorTypeDecider<Iterator,true>::type;
    }
    AbstractSequentialConstIteratorAccess::IteratorStorage iteratorStorage() const override {
        return QtJambiPrivate::StorageTypeDecider<Container,Iterator>::type;
    }
private:
    QAssociativeIteratorAccess(){}
public:
    using QAssociativeConstIteratorAccess<Iterator,Container,AbstractAssociativeIteratorAccess>::value;
    using QAssociativeConstIteratorAccess<Iterator,Container,AbstractAssociativeIteratorAccess>::equals;
    static QAssociativeIteratorAccess<Iterator,Container>* newInstance(){
        static QAssociativeIteratorAccess<Iterator,Container> instance;
        return &instance;
    }
    QAssociativeIteratorAccess<Iterator,Container>* clone() override{
        return this;
    }
    void setValue(JNIEnv * env, void* ptr, jobject newValue) override {
        QtJambiPrivate::IteratorAssociativeValue<Iterator,false>::function(env, ptr, newValue);
    }
    std::optional<void*> value(void* ptr) override {
        return QtJambiPrivate::IteratorAssociativeValueOptional<Iterator,void*>::function(ptr);
    }
    bool setIntValue(void* ptr, jint value) override {
            return QtJambiPrivate::IteratorAssociativeSetValue<Iterator>::function(ptr, value);
    }
    bool setLongValue(void* ptr, jlong value) override {
        return QtJambiPrivate::IteratorAssociativeSetValue<Iterator>::function(ptr, value);
    }
    bool setShortValue(void* ptr, jshort value) override {
        return QtJambiPrivate::IteratorAssociativeSetValue<Iterator>::function(ptr, value);
    }
    bool setByteValue(void* ptr, jbyte value) override {
        return QtJambiPrivate::IteratorAssociativeSetValue<Iterator>::function(ptr, value);
    }
    bool setFloatValue(void* ptr, jfloat value) override {
        return QtJambiPrivate::IteratorAssociativeSetValue<Iterator>::function(ptr, value);
    }
    bool setDoubleValue(void* ptr, jdouble value) override {
        return QtJambiPrivate::IteratorAssociativeSetValue<Iterator>::function(ptr, value);
    }
    bool setCharValue(void* ptr, jchar value) override {
        return QtJambiPrivate::IteratorAssociativeSetValue<Iterator>::function(ptr, value);
    }
    bool setBooleanValue(void* ptr, jboolean value) override {
        return QtJambiPrivate::IteratorAssociativeSetValue<Iterator>::function(ptr, value);
    }
    bool setVariantValue(void* ptr, const QVariant& value) override {
        return QtJambiPrivate::IteratorAssociativeSetValue<Iterator>::function(ptr, value);
    }
    jboolean equals(JNIEnv *, const void* ptr, const ConstContainerAndAccessInfo& ptr2) override;
};

template<typename Iterator, typename Container, typename SuperType>
jboolean QAssociativeConstIteratorAccess<Iterator,Container,SuperType>::equals(JNIEnv *env, const void* ptr, const ConstContainerAndAccessInfo& ptr2) {
    if constexpr(std::is_void_v<Container>){
        return *reinterpret_cast<const Iterator*>(ptr)==*reinterpret_cast<const Iterator*>(ptr2.container);
    }else{
        return QtJambiPrivate::IteratorComparator<Container,Iterator,false,true,QtJambiPrivate::StorageTypeDecider<Container,Iterator>::type>::equals(env, ptr, ptr2);
    }
}

template<typename Iterator, typename Container>
jboolean QAssociativeIteratorAccess<Iterator,Container>::equals(JNIEnv * env, const void* ptr, const ConstContainerAndAccessInfo& ptr2) {
    if constexpr(std::is_void_v<Container>){
        return *reinterpret_cast<const Iterator*>(ptr)==*reinterpret_cast<const Iterator*>(ptr2.container);
    }else{
        return QtJambiPrivate::IteratorComparator<Container,Iterator,true,true,QtJambiPrivate::StorageTypeDecider<Container,Iterator>::type>::equals(env, ptr, ptr2);
    }
}

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
template<typename Iterator, typename Container, typename SuperType>
AbstractSpanAccess* QSequentialConstIteratorAccess<Iterator,Container,SuperType>::createSpanAccess(){
    if constexpr(!std::is_same_v<typename QtJambiPrivate::IteratorSequentialValueType<Iterator>::type,void>){
        return QSpanAccess<std::add_const_t<typename QtJambiPrivate::IteratorSequentialValueType<Iterator>::type>>::newInstance();
    }else{
        return nullptr;
    }
}
template<typename Iterator, typename Container>
AbstractSpanAccess* QSequentialIteratorAccess<Iterator,Container>::createSpanAccess(){
    if constexpr(!std::is_same_v<typename QtJambiPrivate::IteratorSequentialValueType<Iterator>::type,void>){
        typedef typename QtJambiPrivate::IteratorSequentialValueType<Iterator>::type T;
        if constexpr(QtJambiPrivate::supports_assign_v<T&,const T&>){
            return QSpanAccess<T>::newInstance();
        }else{
            return QSpanAccess<std::add_const_t<T>>::newInstance();
        }
    }else{
        return nullptr;
    }
}
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)

#endif // CONTAINERACCESS_ITERATOR_H
