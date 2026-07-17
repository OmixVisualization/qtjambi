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

#ifndef QTJAMBI_CAST_ITERATOR_H
#define QTJAMBI_CAST_ITERATOR_H

#include "qtjambi_cast.h"
#include "qtjambiapi_iterator.h"
#include "containeraccess_iterator.h"
#include "containerapi.h"

namespace QtJambiPrivate {

template<typename Iterator>
struct has_container_ref : std::false_type {
};

template<typename Container>
struct has_container_ref<ContainerIterator<ContainerRef<Container>>> : std::true_type {
};

template<typename Iterator>
constexpr bool has_container_ref_v = has_container_ref<Iterator>::value;

template<typename Container>
struct is_hash : std::false_type{
};

template<typename K,typename T>
struct is_hash<QHash<K,T>> : std::true_type{
};

template<typename Container>
struct is_multihash : std::false_type{
};

template<typename K,typename T>
struct is_multihash<QMultiHash<K,T>> : std::true_type{
};

template<typename Container>
struct is_map : std::false_type{
};

template<typename K,typename T>
struct is_map<QMap<K,T>> : std::true_type{
};

template<typename Container>
struct is_multimap : std::false_type{
};

template<typename K,typename T>
struct is_multimap<QMultiMap<K,T>> : std::true_type{
};

template<typename Container>
struct is_span : std::false_type{
};

template<typename T, size_t E>
struct is_span<QSpan<T,E>> : std::true_type{
};

template<typename Container>
struct is_set : std::false_type{
};

template<typename T>
struct is_set<QSet<T>> : std::true_type{
};

template<typename Container>
struct is_list : std::false_type{
};

template<typename T>
struct is_list<QList<T>> : std::true_type{
};

template<typename T>
struct is_list<QQueue<T>> : std::true_type{
};

template<typename T>
struct is_list<QStack<T>> : std::true_type{
};

template<typename Iterator>
struct is_reverse : std::false_type{
};

template<typename Iterator>
struct is_reverse<std::reverse_iterator<Iterator>> : std::true_type{
};

template<typename Container, typename Iterator, bool sequential, bool isMutable>
struct IteratorAccessType{
    using type = std::conditional_t<sequential,
                                   std::conditional_t<isMutable,
                                                      QSequentialIteratorAccess<Iterator,Container>,
                                                      QSequentialConstIteratorAccess<Iterator,Container>>,
                                   std::conditional_t<isMutable,
                                                      QAssociativeIteratorAccess<Iterator,Container>,
                                                      QAssociativeConstIteratorAccess<Iterator,Container>>>;
};

template<typename Container, typename Iterator, typename Storage, bool rvalue, bool sequential, bool isMutable, typename... Args>
struct qtjambi_ContainerIterator_cast{
    using iterator_type = ContainerIterator<Container,Iterator,Storage>;
    using In = std::conditional_t<rvalue,iterator_type&&,const iterator_type&>;
    using IteratorAccess = typename IteratorAccessType<Container, iterator_type, sequential, isMutable>::type;
    static jobject cast(In iter, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(is_list<Container>::value){
            if constexpr(is_reverse<Iterator>::value){
                return QtJambiAPI::convertListReverseIteratorToJavaObject(env,
                                                                       new iterator_type(std::move(iter)),
                                                                       QtJambiAPI::deletePointer<Iterator>,
                                                                       IteratorAccess::newInstance()
                                                                    );
            }else{
                return QtJambiAPI::convertListIteratorToJavaObject(env,
                                                                   new iterator_type(std::move(iter)),
                                                                   QtJambiAPI::deletePointer<Iterator>,
                                                                   IteratorAccess::newInstance()
                                                                   );
            }
        }else if constexpr(is_set<Container>::value){
            return QtJambiAPI::convertSetIteratorToJavaObject(env,
                                                                   new iterator_type(std::move(iter)),
                                                                   QtJambiAPI::deletePointer<Iterator>,
                                                                   IteratorAccess::newInstance()
                                                                   );
        }else if constexpr(is_span<Container>::value){
            if constexpr(is_reverse<Iterator>::value){
                return QtJambiAPI::convertSpanReverseIteratorToJavaObject(env,
                                                                      new iterator_type(std::move(iter)),
                                                                      QtJambiAPI::deletePointer<Iterator>,
                                                                      IteratorAccess::newInstance()
                                                                      );
            }else{
                return QtJambiAPI::convertSpanIteratorToJavaObject(env,
                                                                   new iterator_type(std::move(iter)),
                                                                   QtJambiAPI::deletePointer<Iterator>,
                                                                   IteratorAccess::newInstance()
                                                                   );
            }
        }else if constexpr(is_map<Container>::value){
            if constexpr(is_key_iterator_v<Iterator,Container>){
                return QtJambiAPI::convertMapKeyIteratorToJavaObject(env,
                                                                          new iterator_type(std::move(iter)),
                                                                          QtJambiAPI::deletePointer<Iterator>,
                                                                          IteratorAccess::newInstance()
                                                                          );
            }else if constexpr(is_key_value_iterator_v<Iterator,Container> || is_const_key_value_iterator_v<Iterator,Container>){
                return QtJambiAPI::convertMapKeyValueIteratorToJavaObject(env,
                                                                  new iterator_type(std::move(iter)),
                                                                  QtJambiAPI::deletePointer<Iterator>,
                                                                  IteratorAccess::newInstance()
                                                                  );
            }else{
                return QtJambiAPI::convertMapIteratorToJavaObject(env,
                                                                  new iterator_type(std::move(iter)),
                                                                  QtJambiAPI::deletePointer<Iterator>,
                                                                  IteratorAccess::newInstance()
                                                                  );
            }
        }else if constexpr(is_hash<Container>::value){
            if constexpr(is_key_iterator_v<Iterator,Container>){
                return QtJambiAPI::convertHashKeyIteratorToJavaObject(env,
                                                                   new iterator_type(std::move(iter)),
                                                                   QtJambiAPI::deletePointer<Iterator>,
                                                                   IteratorAccess::newInstance()
                                                                   );
            }else if constexpr(is_key_value_iterator_v<Iterator,Container> || is_const_key_value_iterator_v<Iterator,Container>){
                return QtJambiAPI::convertHashKeyValueIteratorToJavaObject(env,
                                                                   new iterator_type(std::move(iter)),
                                                                   QtJambiAPI::deletePointer<Iterator>,
                                                                   IteratorAccess::newInstance()
                                                                   );
            }else{
                return QtJambiAPI::convertHashIteratorToJavaObject(env,
                                                                   new iterator_type(std::move(iter)),
                                                                   QtJambiAPI::deletePointer<Iterator>,
                                                                   IteratorAccess::newInstance()
                                                                   );
            }
        }else if constexpr(is_multimap<Container>::value){
            if constexpr(is_key_iterator_v<Iterator,Container>){
                return QtJambiAPI::convertMultiMapKeyIteratorToJavaObject(env,
                                                                       new iterator_type(std::move(iter)),
                                                                       QtJambiAPI::deletePointer<Iterator>,
                                                                       IteratorAccess::newInstance()
                                                                       );
            }else if constexpr(is_key_value_iterator_v<Iterator,Container> || is_const_key_value_iterator_v<Iterator,Container>){
                return QtJambiAPI::convertMultiMapKeyValueIteratorToJavaObject(env,
                                                                       new iterator_type(std::move(iter)),
                                                                       QtJambiAPI::deletePointer<Iterator>,
                                                                       IteratorAccess::newInstance()
                                                                       );
            }else{
                return QtJambiAPI::convertMultiMapIteratorToJavaObject(env,
                                                                       new iterator_type(std::move(iter)),
                                                                       QtJambiAPI::deletePointer<Iterator>,
                                                                       IteratorAccess::newInstance()
                                                                       );
            }
        }else if constexpr(is_multihash<Container>::value){
            if constexpr(is_key_iterator_v<Iterator,Container>){
                return QtJambiAPI::convertMultiHashKeyIteratorToJavaObject(env,
                                                                        new iterator_type(std::move(iter)),
                                                                        QtJambiAPI::deletePointer<Iterator>,
                                                                        IteratorAccess::newInstance()
                                                                        );
            }else if constexpr(is_key_value_iterator_v<Iterator,Container> || is_const_key_value_iterator_v<Iterator,Container>){
                return QtJambiAPI::convertMultiHashKeyValueIteratorToJavaObject(env,
                                                                        new iterator_type(std::move(iter)),
                                                                        QtJambiAPI::deletePointer<Iterator>,
                                                                        IteratorAccess::newInstance()
                                                                        );
            }else{
                return QtJambiAPI::convertMultiHashIteratorToJavaObject(env,
                                                                        new iterator_type(std::move(iter)),
                                                                        QtJambiAPI::deletePointer<Iterator>,
                                                                        IteratorAccess::newInstance()
                                                                        );
            }
        }else{
            return QtJambiAPI::convertIteratorToJavaObject(env,
                                                           typeid(Container),
                                                           typeid(Iterator),
                                                           new iterator_type(std::move(iter)),
                                                           QtJambiAPI::deletePointer<Iterator>,
                                                           IteratorAccess::newInstance()
                                                          );
        }
    }
};

template<typename Container, typename Iterator, bool rvalue, bool sequential, bool isMutable, typename... Args>
struct qtjambi_ContainerIterator_cast<Container, Iterator, QtJambiNativeID, rvalue, sequential, isMutable, Args...>{
    using iterator_type = ContainerIterator<Container,Iterator,QtJambiNativeID>;
    using In = std::conditional_t<rvalue,iterator_type&&,const iterator_type&>;
    using IteratorAccess = typename IteratorAccessType<Container, Iterator, sequential, isMutable>::type;
    static jobject cast(In iter, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        return QtJambiAPI::convertIteratorToJavaObject(env,
                                                       typeid(Container),
                                                       typeid(Iterator),
                                                       iter.m_storage,
                                                       new Iterator(std::move(iter.m_iterator)),
                                                       QtJambiAPI::deletePointer<Iterator>,
                                                       IteratorAccess::newInstance()
                                                    );
    }
};

template<typename Container, typename Iterator, bool rvalue, bool sequential, bool isMutable, typename... Args>
struct qtjambi_ContainerIterator_cast<Container, Iterator, jobject, rvalue, sequential, isMutable, Args...>{
    using iterator_type = ContainerIterator<Container,Iterator,jobject>;
    using In = std::conditional_t<rvalue,iterator_type&&,const iterator_type&>;
    using IteratorAccess = std::conditional_t<sequential,
                                              std::conditional_t<isMutable,
                                                                 QSequentialIteratorAccess<Iterator,Container>,
                                                                 QSequentialConstIteratorAccess<Iterator,Container>>,
                                              std::conditional_t<isMutable,
                                                                 QAssociativeIteratorAccess<Iterator,Container>,
                                                                 QAssociativeConstIteratorAccess<Iterator,Container>>>;
    static jobject cast(In iter, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        return QtJambiAPI::convertIteratorToJavaObject(env,
                                                       typeid(Container),
                                                       typeid(Iterator),
                                                       iter.m_storage,
                                                       new Iterator(std::move(iter.m_iterator)),
                                                       QtJambiAPI::deletePointer<Iterator>,
                                                       IteratorAccess::newInstance()
                                                       );
    }
};

template<typename Container, typename Iterator, bool rvalue, bool sequential, bool isMutable, typename... Args>
struct qtjambi_ContainerIterator_cast<Container, Iterator, ContainerRef<Container>, rvalue, sequential, isMutable, Args...>{
    using iterator_type = ContainerIterator<Container,Iterator,ContainerRef<Container>>;
    using In = std::conditional_t<rvalue,iterator_type&&,const iterator_type&>;
    using IteratorAccess = typename IteratorAccessType<Container, iterator_type, sequential, isMutable>::type;
    static jobject cast(In iter, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        QSharedPointer<ContainerRefPrivate> owner = iter.storage().reference();
        if constexpr(is_list<Container>::value){
            if constexpr(is_reverse<Iterator>::value){
                return QtJambiPrivate::convertListReverseIteratorToJavaObject(env,
                                                                       owner,
                                                                       new iterator_type(std::move(iter)),
                                                                       QtJambiAPI::deletePointer<Iterator>,
                                                                       IteratorAccess::newInstance()
                                                                   );
            }else{
                return QtJambiPrivate::convertListIteratorToJavaObject(env,
                                                                       owner,
                                                                       new iterator_type(std::move(iter)),
                                                                       QtJambiAPI::deletePointer<Iterator>,
                                                                       IteratorAccess::newInstance()
                                                                       );
            }
        }else if constexpr(is_set<Container>::value){
            return QtJambiPrivate::convertSetIteratorToJavaObject(env,
                                                                  owner,
                                                                  new iterator_type(std::move(iter)),
                                                                  QtJambiAPI::deletePointer<Iterator>,
                                                                  IteratorAccess::newInstance()
                                                              );
        }else if constexpr(is_span<Container>::value){
            if constexpr(is_reverse<Iterator>::value){
                return QtJambiPrivate::convertSpanReverseIteratorToJavaObject(env,
                                                                       owner,
                                                                       new iterator_type(std::move(iter)),
                                                                       QtJambiAPI::deletePointer<Iterator>,
                                                                       IteratorAccess::newInstance()
                                                                   );
            }else{
                return QtJambiPrivate::convertSpanIteratorToJavaObject(env,
                                                                       owner,
                                                                       new iterator_type(std::move(iter)),
                                                                       QtJambiAPI::deletePointer<Iterator>,
                                                                       IteratorAccess::newInstance()
                                                                       );
            }
        }else if constexpr(is_map<Container>::value){
            if constexpr(is_key_iterator_v<Iterator,Container>){
                return QtJambiPrivate::convertMapKeyIteratorToJavaObject(env,
                                                                              owner,
                                                                              new iterator_type(std::move(iter)),
                                                                              QtJambiAPI::deletePointer<Iterator>,
                                                                              IteratorAccess::newInstance()
                                                                              );
            }else if constexpr(is_key_value_iterator_v<Iterator,Container> || is_const_key_value_iterator_v<Iterator,Container>){
                return QtJambiPrivate::convertMapKeyValueIteratorToJavaObject(env,
                                                                      owner,
                                                                      new iterator_type(std::move(iter)),
                                                                      QtJambiAPI::deletePointer<Iterator>,
                                                                      IteratorAccess::newInstance()
                                                                      );
            }else {
                return QtJambiPrivate::convertMapIteratorToJavaObject(env,
                                                                      owner,
                                                                      new iterator_type(std::move(iter)),
                                                                      QtJambiAPI::deletePointer<Iterator>,
                                                                      IteratorAccess::newInstance()
                                                                      );
            }
        }else if constexpr(is_hash<Container>::value){
            if constexpr(is_key_iterator_v<Iterator,Container>){
                return QtJambiPrivate::convertHashKeyIteratorToJavaObject(env,
                                                                       owner,
                                                                       new iterator_type(std::move(iter)),
                                                                       QtJambiAPI::deletePointer<Iterator>,
                                                                       IteratorAccess::newInstance()
                                                                       );
            }else if constexpr(is_key_value_iterator_v<Iterator,Container> || is_const_key_value_iterator_v<Iterator,Container>){
                return QtJambiPrivate::convertHashKeyValueIteratorToJavaObject(env,
                                                                               owner,
                                                                               new iterator_type(std::move(iter)),
                                                                               QtJambiAPI::deletePointer<Iterator>,
                                                                               IteratorAccess::newInstance()
                                                                               );
            }else {
                return QtJambiPrivate::convertHashIteratorToJavaObject(env,
                                                                       owner,
                                                                       new iterator_type(std::move(iter)),
                                                                       QtJambiAPI::deletePointer<Iterator>,
                                                                       IteratorAccess::newInstance()
                                                                       );
            }
        }else if constexpr(is_multimap<Container>::value){
            if constexpr(is_key_iterator_v<Iterator,Container>){
                return QtJambiPrivate::convertMultiMapKeyIteratorToJavaObject(env,
                                                                           owner,
                                                                           new iterator_type(std::move(iter)),
                                                                           QtJambiAPI::deletePointer<Iterator>,
                                                                           IteratorAccess::newInstance()
                                                                           );
            }else if constexpr(is_key_value_iterator_v<Iterator,Container> || is_const_key_value_iterator_v<Iterator,Container>){
                return QtJambiPrivate::convertMultiMapKeyValueIteratorToJavaObject(env,
                                                                                   owner,
                                                                                   new iterator_type(std::move(iter)),
                                                                                   QtJambiAPI::deletePointer<Iterator>,
                                                                                   IteratorAccess::newInstance()
                                                                                   );
            }else {
                return QtJambiPrivate::convertMultiMapIteratorToJavaObject(env,
                                                                           owner,
                                                                           new iterator_type(std::move(iter)),
                                                                           QtJambiAPI::deletePointer<Iterator>,
                                                                           IteratorAccess::newInstance()
                                                                           );
            }
        }else if constexpr(is_multihash<Container>::value){
            if constexpr(is_key_iterator_v<Iterator,Container>){
                return QtJambiPrivate::convertMultiHashKeyIteratorToJavaObject(env,
                                                                            owner,
                                                                            new iterator_type(std::move(iter)),
                                                                            QtJambiAPI::deletePointer<Iterator>,
                                                                            IteratorAccess::newInstance()
                                                                            );
            }else if constexpr(is_key_value_iterator_v<Iterator,Container> || is_const_key_value_iterator_v<Iterator,Container>){
                return QtJambiPrivate::convertMultiHashKeyValueIteratorToJavaObject(env,
                                                                                    owner,
                                                                                    new iterator_type(std::move(iter)),
                                                                                    QtJambiAPI::deletePointer<Iterator>,
                                                                                    IteratorAccess::newInstance()
                                                                                    );
            }else {
                return QtJambiPrivate::convertMultiHashIteratorToJavaObject(env,
                                                                            owner,
                                                                            new iterator_type(std::move(iter)),
                                                                            QtJambiAPI::deletePointer<Iterator>,
                                                                            IteratorAccess::newInstance()
                                                                            );
            }
        }else{
            return QtJambiPrivate::convertIteratorToJavaObject(env,
                                                               typeid(Container),
                                                               typeid(Iterator),
                                                               owner,
                                                               new iterator_type(std::move(iter)),
                                                               QtJambiAPI::deletePointer<Iterator>,
                                                               IteratorAccess::newInstance()
                                                           );
        }
    }
};

template<typename Iter, bool isMutable, typename... Args>
struct qtjambi_mutable_sequential_iterator_cast{
    using Iterator = std::remove_reference_t<Iter>;
    using In = std::conditional_t<std::is_reference_v<Iterator> || std::is_pointer_v<Iterator>, Iterator, Iterator&&>;
    using IteratorAccess = typename IteratorAccessType<void, Iterator, true, isMutable>::type;
    static jobject cast(QtJambiNativeID __list_nativeId, In iter, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        return QtJambiAPI::convertIteratorToJavaObject(env, __list_nativeId,
                                                      new Iterator(std::move(iter)),
                                                      QtJambiAPI::deletePointer<Iterator>,
                                                      IteratorAccess::newInstance()
                                                      );
    }
};

template<typename Container, typename Iter, typename Storage, bool isMutable, typename... Args>
struct qtjambi_mutable_sequential_iterator_cast<ContainerIterator<Container,Iter,Storage>&&,isMutable,Args...>
    : qtjambi_ContainerIterator_cast<Container,Iter,Storage,true,true,isMutable,Args...>{};

template<typename Container, typename Iter, typename Storage, bool isMutable, typename... Args>
struct qtjambi_mutable_sequential_iterator_cast<const ContainerIterator<Container,Iter,Storage>&,isMutable,Args...>
    : qtjambi_ContainerIterator_cast<Container,Iter,Storage,false,true,isMutable,Args...>{};

template<typename Value>
struct qtjambi_iterator_mutable_test : std::bool_constant<std::is_reference_v<Value> && !std::is_const_v<std::remove_reference_t<Value>>> {
};
template<typename Iterator, bool = supports_deref_v<Iterator&>>
struct qtjambi_sequential_iterator_mutable_test : std::false_type {
};
template<typename Iterator>
struct qtjambi_sequential_iterator_mutable_test<Iterator,true> : qtjambi_iterator_mutable_test<decltype(*std::declval<Iterator>())> {
};
template<typename Iterator>
constexpr bool qtjambi_sequential_iterator_mutable_test_v = qtjambi_sequential_iterator_mutable_test<Iterator>::value;

template<typename Iterator, typename... Args>
struct qtjambi_sequential_iterator_cast : qtjambi_mutable_sequential_iterator_cast<Iterator, qtjambi_sequential_iterator_mutable_test_v<Iterator>, Args...>{
};

template<typename Iter, bool isMutable, typename... Args>
struct qtjambi_mutable_associative_iterator_cast{
    using Iterator = std::remove_reference_t<Iter>;
    using In = std::conditional_t<std::is_reference_v<Iterator> || std::is_pointer_v<Iterator>, Iterator, Iterator&&>;
    using IteratorAccess = std::conditional_t<isMutable,
                                              QAssociativeIteratorAccess<Iterator,void>,
                                              QAssociativeConstIteratorAccess<Iterator,void>>;
    static jobject cast(QtJambiNativeID nativeId, In iter, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        return QtJambiAPI::convertIteratorToJavaObject(env, nativeId,
                                                       new Iterator(std::move(iter)),
                                                       QtJambiAPI::deletePointer<Iterator>,
                                                       IteratorAccess::newInstance()
                                                       );
    }
};

template<typename Container, typename Iter, typename Storage, bool isMutable, typename... Args>
struct qtjambi_mutable_associative_iterator_cast<ContainerIterator<Container,Iter,Storage>&&,isMutable,Args...>
    : qtjambi_ContainerIterator_cast<Container,Iter,Storage,true,false,isMutable,Args...>{};

template<typename Container, typename Iter, typename Storage, bool isMutable, typename... Args>
struct qtjambi_mutable_associative_iterator_cast<const ContainerIterator<Container,Iter,Storage>&,isMutable,Args...>
    : qtjambi_ContainerIterator_cast<Container,Iter,Storage,false,false,isMutable,Args...>{};

template<typename Iterator, bool = supports_value_v<Iterator>>
struct qtjambi_associative_iterator_mutable_test : std::false_type {
};
template<typename Iterator>
struct qtjambi_associative_iterator_mutable_test<Iterator,true> : qtjambi_iterator_mutable_test<decltype(std::declval<Iterator>().value())> {
};
template<typename Key, typename T, typename Iterator QT610_EXTRA_ARG(class Traits)>
struct qtjambi_sequential_iterator_mutable_test<QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>,true> : qtjambi_associative_iterator_mutable_test<Iterator> {
};
template<typename Container, typename Key, typename T, typename Iterator QT610_EXTRA_ARG(class Traits), typename Storage>
struct qtjambi_sequential_iterator_mutable_test<ContainerIterator<Container,QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>,Storage>,true> : qtjambi_associative_iterator_mutable_test<Iterator> {
};
template<typename Key, typename T, typename Iterator QT610_EXTRA_ARG(class Traits)>
struct qtjambi_sequential_iterator_mutable_test<QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>&&,true> : qtjambi_associative_iterator_mutable_test<Iterator> {
};
template<typename Container, typename Key, typename T, typename Iterator QT610_EXTRA_ARG(class Traits), typename Storage>
struct qtjambi_sequential_iterator_mutable_test<ContainerIterator<Container,QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>,Storage>&&,true> : qtjambi_associative_iterator_mutable_test<Iterator> {
};
template<typename Key, typename T, typename Iterator QT610_EXTRA_ARG(class Traits)>
struct qtjambi_sequential_iterator_mutable_test<QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>&,true> : qtjambi_associative_iterator_mutable_test<Iterator> {
};
template<typename Container, typename Key, typename T, typename Iterator QT610_EXTRA_ARG(class Traits), typename Storage>
struct qtjambi_sequential_iterator_mutable_test<ContainerIterator<Container,QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>,Storage>&,true> : qtjambi_associative_iterator_mutable_test<Iterator> {
};
template<typename Key, typename T, typename Iterator QT610_EXTRA_ARG(class Traits)>
struct qtjambi_sequential_iterator_mutable_test<const QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>&,true> : qtjambi_associative_iterator_mutable_test<Iterator> {
};
template<typename Container, typename Key, typename T, typename Iterator QT610_EXTRA_ARG(class Traits), typename Storage>
struct qtjambi_sequential_iterator_mutable_test<const ContainerIterator<Container,QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>,Storage>&,true> : qtjambi_associative_iterator_mutable_test<Iterator> {
};
template<typename Iterator>
constexpr bool qtjambi_associative_iterator_mutable_test_v = qtjambi_associative_iterator_mutable_test<Iterator>::value;

template<typename Iterator, typename... Args>
struct qtjambi_associative_iterator_cast : qtjambi_mutable_associative_iterator_cast<Iterator, qtjambi_associative_iterator_mutable_test_v<Iterator>, Args...>{
};

template<bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename Storage, class Iter, typename Container, typename... Args>
struct qtjambi_jobject_template3_cast<true,
                                      jobject,
                                      ContainerIterator, is_pointer, is_const, is_reference, is_rvalue,
                                      Storage, Iter, Container, Args...> : decltype(qtjambi_cast_iterator<ContainerIterator<Storage,Iter,Container>&&, Args...>()){
};

template<typename Storage, class Iter, typename Container>
struct qtjambi_cast_result<ContainerIterator<Container, Iter, Storage>>{
    using type = Iter;
};

template<bool is_pointer, bool is_const, bool is_reference, bool is_rvalue,
         typename Container, class Iter, typename Storage, typename... Args>
struct qtjambi_jobject_template3_cast<false,
                                      jobject,
                                      ContainerIterator, is_pointer, is_const, is_reference, is_rvalue,
                                      Container, Iter, Storage, Args...>{
    typedef Iter NativeType;
    typedef std::conditional_t<is_const, std::add_const_t<NativeType>, NativeType> NativeType_c;
    typedef std::conditional_t<is_reference, std::conditional_t<is_rvalue, std::add_rvalue_reference_t<NativeType_c>, std::add_lvalue_reference_t<NativeType_c>>, NativeType_c> NativeType_cr;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_in;
    typedef std::conditional_t<is_pointer, std::add_pointer_t<NativeType_c>, NativeType_cr> NativeType_out;
    typedef std::add_pointer_t<NativeType> NativeType_ptr;
    typedef jobject In;
    typedef NativeType_out Out;

    static Out cast(In in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(is_default_constructible_v<NativeType>){
            QPair<void*,AbstractContainerAccess*> pair = ContainerAPI::fromJavaOwner(env, in);
            if(pair.first && pair.second){
                if(pair.second->isSequentialConstIterator()){
                    AbstractSequentialConstIteratorAccess* access = static_cast<AbstractSequentialConstIteratorAccess*>(pair.second);
                    NativeType result;
                    if(access->findIterator(pair.first, typeid(Iter), &result)){
                        return pointer_ref_or_clone_decider<is_pointer, is_const, is_reference, NativeType, Args...>::convert(std::move(result), args...);
                    }
                }
                JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null"), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
            }else{
                JavaException::raiseQNoImplementationException(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassNamePrintable(env, in)) QTJAMBI_STACKTRACEINFO );
            }
        }else{
            NativeType_ptr result = nullptr;
            if(!QtJambiAPI::convertJavaToNative(env, in, &result, typeid(NativeType))){
                JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast object of type %1 to %2").arg(in ? QtJambiAPI::getObjectClassName(env, in) : QStringLiteral("null"), QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
            }
            if constexpr(is_pointer){
                return result;
            }else{
                if constexpr(!is_default_constructible_v<NativeType> || (is_reference && !is_const)){
                    if(!result)
                        JavaException::raiseNullPointerException(env, QStringLiteral("Cannot cast null to reference type %1").arg(QLatin1String(QtJambiAPI::typeName(typeid(NativeType)))) QTJAMBI_STACKTRACEINFO );
                }
                return qtjambi_deref_value<NativeType, is_default_constructible_v<NativeType>, is_copy_constructible_v<NativeType>, is_const, is_reference>::deref(env, result);
            }
        }
    }
};

template<typename Container, class Iter, typename Storage, typename... Args>
struct qtjambi_nojni_plain_cast<ContainerIterator<Container, Iter, Storage>, QtJambiNativeID, Args...>{
    static Iter cast(QtJambiNativeID in, Args... args){
        auto env = cast_var_args<Args...>::env(args...);
        if constexpr(is_default_constructible_v<Iter>){
            QPair<void*,AbstractContainerAccess*> pair = ContainerAPI::fromNativeId(in);
            if(pair.first && pair.second){
                if(pair.second->isSequentialConstIterator()){
                    AbstractSequentialConstIteratorAccess* access = static_cast<AbstractSequentialConstIteratorAccess*>(pair.second);
                    Iter result;
                    if(access->findIterator(pair.first, typeid(Iter), &result)){
                        return result;
                    }
                }
            }
            return Iter();
        }else{
            Iter* result = nullptr;
            QPair<void*,AbstractContainerAccess*> pair = ContainerAPI::fromNativeId(in);
            if(pair.first && pair.second){
                if(pair.second->isSequentialConstIterator()){
                    AbstractSequentialConstIteratorAccess* access = static_cast<AbstractSequentialConstIteratorAccess*>(pair.second);
                    result = reinterpret_cast<Iter*>(access->asIterator(pair.first));
                }
            }
            if(!result)
                JavaException::raiseNullPointerException(env, QStringLiteral("Cannot cast null to reference type %1").arg(QLatin1String(QtJambiAPI::typeName(typeid(Iter)))) QTJAMBI_STACKTRACEINFO );
            return *result;
        }
    }
};

template<typename NativeType>
struct qtjambi_find_iterator{
    static NativeType* function(JNIEnv* env, jobject in){
        QPair<void*,AbstractContainerAccess*> pair = ContainerAPI::fromJavaOwner(env, in);
        if(pair.second && pair.second->isSequentialConstIterator()){
            AbstractSequentialConstIteratorAccess* access = static_cast<AbstractSequentialConstIteratorAccess*>(pair.second);
            return reinterpret_cast<NativeType*>(access->asIterator(pair.first));
        }
        else return nullptr;
    }
};

}//namespace QtJambiPrivate

#endif // QTJAMBI_CAST_ITERATOR_H
