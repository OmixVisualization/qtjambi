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

#ifndef QTJAMBI_CAST_MAP_P_H
#define QTJAMBI_CAST_MAP_P_H

#include "qtjambi_cast_util_p.h"
#include "qtjambi_cast_type_p.h"

namespace QtJambiPrivate {

typedef bool (*IsBiContainerFunction)(JNIEnv *, jobject, const std::type_info&, const std::type_info&);

template<bool is_pointer, bool is_const, bool is_reference, bool buffer, template<typename K, typename T> class Container, typename K, typename T>
struct create_bicontainer_pointer{
    static Container<K,T> create(JNIEnv *, QtJambiScope*, Container<K,T>*, const char*){
        Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
    }
    static Container<K,T> create(JNIEnv *, QtJambiScope*, const Container<K,T>*, const char*){
        Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
    }
    static Container<K,T> create(JNIEnv *, QtJambiScope*, QScopedPointer<Container<K,T>>&, const char*){
        Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
    }
};

template<bool is_const, template<typename K, typename T> class Container, typename K, typename T>
struct create_bicontainer_pointer<true, is_const, false, false, Container, K, T>{
    typedef typename std::conditional<is_const, typename std::add_const<Container<K,T>>::type, Container<K,T>>::type Container_const;
    static Container<K,T>* create(JNIEnv *, QtJambiScope*, Container<K,T>* t, const char*){
        return t;
    }
    static Container_const* create(JNIEnv *, QtJambiScope*, const Container<K,T>* t, const char*){
        Q_STATIC_ASSERT_X(is_const, "Cannot cast const pointer to non-const pointer without scope.");
        return t;
    }
    static Container<K,T>* create(JNIEnv *, QtJambiScope*, QScopedPointer<Container<K,T>>& t, const char*){
        return t.take();
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct create_bicontainer_pointer<true, false, false, true, Container, K, T>{
    static Container<K,T>* create(JNIEnv *, QtJambiScope*, Container<K,T>* t, const char*){
        return t;
    }
    static Container<K,T>* create(JNIEnv *env, QtJambiScope* scope, const Container<K,T>* t, const char* type){
        if(!scope)
            JavaException::raiseError(env, qPrintable( QString("Cannot cast to non-const %1* without scope.").arg(QLatin1String(type)) ) QTJAMBI_STACKTRACEINFO );
        if(t){
            Container<K,T>* result = new Container<K,T>(*t);
            scope->addFinalAction([result](){ delete result; });
            return result;
        }else{
            return nullptr;
        }
    }
    static Container<K,T>* create(JNIEnv *, QtJambiScope*, QScopedPointer<Container<K,T>>& t, const char*){
        return t.take();
    }
};

template<bool is_const, bool buffer, template<typename K, typename T> class Container, typename K, typename T>
struct create_bicontainer_pointer<false, is_const, false, buffer, Container, K, T>{
    static Container<K,T> create(JNIEnv *, QtJambiScope*, const Container<K,T>* t, const char*){
        return t ? *t : Container<K,T>();
    }
    static Container<K,T> create(JNIEnv *, QtJambiScope*, QScopedPointer<Container<K,T>>& t, const char*){
        return t ? *t : Container<K,T>();
    }
};

template<bool buffer, template<typename K, typename T> class Container, typename K, typename T>
struct create_bicontainer_pointer<false, true, true, buffer, Container, K, T>{
    static const Container<K,T>& create(JNIEnv *, QtJambiScope*, const Container<K,T>* t, const char*){
        return t ? *t : qtjambi_get_default_value<Container<K,T>>();
    }
    static const Container<K,T>& create(JNIEnv *env, QtJambiScope* scope, QScopedPointer<Container<K,T>>& t, const char* type){
        Q_STATIC_ASSERT_X(buffer, "Cannot cast to Container reference without scope.");
        if(t && !scope)
            JavaException::raiseError(env, qPrintable( QString("Cannot cast to %1& without scope.").arg(QLatin1String(type)) ) QTJAMBI_STACKTRACEINFO );
        Container<K,T>* ptr = t.take();
        scope->addFinalAction([ptr](){ delete ptr; });
        return t ? *t : qtjambi_get_default_value<Container<K,T>>();
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct create_bicontainer_pointer<false, false, true, true, Container, K, T>{
    static Container<K,T>& create(JNIEnv *env, QtJambiScope* scope, Container<K,T>* t, const char* type){
        if(!scope)
            JavaException::raiseError(env, qPrintable( QString("Cannot cast to %1& without scope.").arg(QLatin1String(type)) ) QTJAMBI_STACKTRACEINFO );
        if(t){
            scope->addFinalAction([t](){ delete t; });
            return *t;
        }else{
            Container<K,T>* result = new Container<K,T>();
            scope->addFinalAction([result](){ delete result; });
            return *result;
        }
    }

    static const Container<K,T>& create(JNIEnv *env, QtJambiScope* scope, const Container<K,T>* t, const char* type){
        if(!scope)
            JavaException::raiseError(env, qPrintable( QString("Cannot cast to %1& without scope.").arg(QLatin1String(type)) ) QTJAMBI_STACKTRACEINFO );
        if(t){
            scope->addFinalAction([t](){ delete t; });
            return *t;
        }else{
            Container<K,T>* result = new Container<K,T>();
            scope->addFinalAction([result](){ delete result; });
            return *result;
        }
    }

    static Container<K,T>& create(JNIEnv *env, QtJambiScope* scope, QScopedPointer<Container<K,T>>& ptr, const char* type){
        if(!scope)
            JavaException::raiseError(env, qPrintable( QString("Cannot cast to %1& without scope.").arg(QLatin1String(type)) ) QTJAMBI_STACKTRACEINFO );
        Container<K,T>* t = ptr.take();
        if(t){
            scope->addFinalAction([t](){ delete t; });
            return *t;
        }else{
            Container<K,T>* result = new Container<K,T>();
            scope->addFinalAction([result](){ delete result; });
            return *result;
        }
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct create_bicontainer_pointer<false, false, true, false, Container, K, T>{
    typedef Container<K,T> Container_KT;
    static Container<K,T>& create(JNIEnv *env, QtJambiScope*, Container<K,T>* t, const char* type){
        if(!t){
            JavaException::raiseNullPointerException(env, qPrintable( QString("%1 is null.").arg(QLatin1String(type)) ) QTJAMBI_STACKTRACEINFO );
        }
        return *t;
    }
    static Container<K,T>& create(JNIEnv *, QtJambiScope*, QScopedPointer<Container<K,T>>& t, const char*){
        Q_STATIC_ASSERT_X(false && std::is_pointer<Container_KT>::value, "Cannot cast to Container reference without scope.");
        return *t;
    }
};

template<bool c_is_pointer, bool c_is_const, typename K, typename T>
struct qtjambi_QMap_caster{
    typedef QMap<K,T> Container;
    typedef typename std::conditional<c_is_const, const Container, Container>::type Container_const;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container_const>::type, typename std::add_lvalue_reference<Container_const>::type>::type Container_in;
    static jobject castfwd(JNIEnv *, Container_in, const char*, QtJambiScope* ){
        Q_STATIC_ASSERT_X(false && !c_is_const, "Cannot cast types");
        return nullptr;
    }
};

template<bool c_is_pointer, bool c_is_const, typename K, typename T>
struct qtjambi_QMultiMap_caster{
    typedef QMultiMap<K,T> Container;
    typedef typename std::conditional<c_is_const, const Container, Container>::type Container_const;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container_const>::type, typename std::add_lvalue_reference<Container_const>::type>::type Container_in;
    static jobject castfwd(JNIEnv *, Container_in, const char*, QtJambiScope* ){
        Q_STATIC_ASSERT_X(false && !c_is_const, "Cannot cast types");
        return nullptr;
    }
};

template<bool c_is_pointer, bool c_is_const, typename K, typename T>
struct qtjambi_QHash_caster{
    typedef QHash<K,T> Container;
    typedef typename std::conditional<c_is_const, const Container, Container>::type Container_const;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container_const>::type, typename std::add_lvalue_reference<Container_const>::type>::type Container_in;
    static jobject castfwd(JNIEnv *, Container_in, const char*, QtJambiScope* ){
        Q_STATIC_ASSERT_X(false && !c_is_const, "Cannot cast types");
        return nullptr;
    }
};

template<bool c_is_pointer, bool c_is_const, typename K, typename T>
struct qtjambi_QMultiHash_caster{
    typedef QMultiHash<K,T> Container;
    typedef typename std::conditional<c_is_const, const Container, Container>::type Container_const;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container_const>::type, typename std::add_lvalue_reference<Container_const>::type>::type Container_in;
    static jobject castfwd(JNIEnv *, Container_in, const char*, QtJambiScope* ){
        Q_STATIC_ASSERT_X(false && !c_is_const, "Cannot cast types");
        return nullptr;
    }
};

template<typename Iterator>
struct BiIteratorValue{
    static QIteratorValueFunction function() { return  [](JNIEnv *env, void* ptr) -> jobject {
            Q_UNUSED(env)
            Iterator* iterator = static_cast<Iterator*>(ptr);
            return qtjambi_cast<jobject>(env, iterator->value() );
        }; }
};

template<typename Iterator>
struct BiIteratorKey{
    static QIteratorValueFunction function() { return  [](JNIEnv *env, void* ptr) -> jobject {
            Q_UNUSED(env)
            Iterator* iterator = static_cast<Iterator*>(ptr);
            return qtjambi_cast<jobject>(env, iterator->key() );
        }; }
};

template<typename Iterator>
struct qtjambi_BiIterator_caster{
    static jobject castfwd(JNIEnv * env, QtJambiNativeID nativeId, Iterator&& iter){
        return qtjambi_to_QMapIterator(env, nativeId,
            new Iterator(iter),
            BiIteratorKey<Iterator>::function(),
            BiIteratorValue<Iterator>::function(),
            IteratorIncrement<Iterator>::function(),
            IteratorDecrement<Iterator>::function(),
            IteratorLessThan<Iterator, supports_less_than<Iterator>::value>::function(),
            IteratorEquals<Iterator>::function(),
            [](void* ptr) {
               Iterator* iterator = static_cast<Iterator*>(ptr);
               delete iterator;
            }
        );
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool supported>
struct BiContainerBegin{
    static QMapBeginFunction function() { return nullptr;}
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerBegin<Container, K, T, true>{
    static QMapBeginFunction function() { return  [](JNIEnv *env, QtJambiNativeID nativeId, const void* ptr) -> jobject {
            Q_UNUSED(env)
            const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
            return qtjambi_BiIterator_caster<typename Container<K,T>::const_iterator>::castfwd(env, nativeId, container->begin());
        }; }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool supported>
struct BiContainerCapacity{
    static QHashCapacityFunction function() { return nullptr;}
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerCapacity<Container, K, T, true>{
    static QHashCapacityFunction function() { return  [](JNIEnv *env, const void* ptr) -> jint {
            Q_UNUSED(env)
            const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
            return jint(container->capacity());
        }; }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool supported>
struct BiContainerReserve{
    static QHashReserveFunction function() { return nullptr;}
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerReserve<Container, K, T, true>{
    static QHashReserveFunction function() {
        return [](JNIEnv * env, void* ptr, jint size) {
                Q_UNUSED(env)
                Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
                container->reserve(size);
            };
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool supported>
struct BiContainerContains{
    static QMapContainsFunction function() { return nullptr;}
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerContains<Container, K, T, true>{
    static QMapContainsFunction function() { return  [](JNIEnv * env, const void* ptr, jobject object) -> jboolean {
            Q_UNUSED(env)
            const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
            return container->contains(qtjambi_cast<K>(env, object));
        }; }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool supported>
struct BiContainerCountObject{
    static QMapCountObjectFunction function() { return nullptr; }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerCountObject<Container, K, T, true>{
    static QMapCountObjectFunction function() { return  [](JNIEnv * env, const void* ptr, jobject object) -> jint {
            Q_UNUSED(env)
            const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
            return container->count(qtjambi_cast<K>(env, object));
        }; }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool supported>
struct BiContainerEnd{
    static QMapBeginFunction function() { return nullptr; }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerEnd<Container, K, T, true>{
    static QMapBeginFunction function() { return  [](JNIEnv *env, QtJambiNativeID nativeId, const void* ptr) -> jobject {
            Q_UNUSED(env)
            const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
            return qtjambi_BiIterator_caster<typename Container<K,T>::const_iterator>::castfwd(env, nativeId, container->end());
        }; }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool supported>
struct BiContainerFindPair{
    static QMapFindFunction function() { return nullptr; }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerFindPair<Container, K, T, true>{
    static QMapFindFunction function() { return  [](JNIEnv *env, QtJambiNativeID nativeId, const void* ptr, jobject key) -> jobject {
            Q_UNUSED(env)
            const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
            return qtjambi_BiIterator_caster<typename Container<K,T>::const_iterator>::castfwd(env, nativeId, container->find(qtjambi_cast<K>(env, key)));
        }; }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool supported>
struct BiContainerFirst{
    static QMapFirstFunction function() { return nullptr; }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerFirst<Container, K, T, true>{
    static QMapFirstFunction function() { return  [](JNIEnv *env, const void* ptr) -> jobject {
            Q_UNUSED(env)
            const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
            return qtjambi_cast<jobject>(env, container->first());
        }; }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool supported>
struct BiContainerLast{
    static QMapLastFunction function() { return nullptr; }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerLast<Container, K, T, true>{
    static QMapLastFunction function() { return  [](JNIEnv *env, const void* ptr) -> jobject {
            Q_UNUSED(env)
            const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
            return qtjambi_cast<jobject>(env, container->last());
        }; }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool supported>
struct BiContainerFirstKey{
    static QMapFirstKeyFunction function() { return nullptr; }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerFirstKey<Container, K, T, true>{
    static QMapFirstKeyFunction function() { return  [](JNIEnv *env, const void* ptr) -> jobject {
            Q_UNUSED(env)
            const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
            return qtjambi_cast<jobject>(env, container->firstKey());
        }; }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool supported>
struct BiContainerLastKey{
    static QMapLastKeyFunction function() { return nullptr; }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerLastKey<Container, K, T, true>{
    static QMapLastKeyFunction function() { return  [](JNIEnv *env, const void* ptr) -> jobject {
            Q_UNUSED(env)
            const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
            return qtjambi_cast<jobject>(env, container->lastKey());
        }; }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool supported>
struct BiContainerKey{
    static QMapKeyFunction function() { return nullptr; }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerKey<Container, K, T, true>{
    static QMapKeyFunction function() { return  [](JNIEnv *env, const void* ptr, jobject value, jobject defaultKey) -> jobject {
            const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
            return qtjambi_cast<jobject>(env, container->key(qtjambi_cast<T>(env, value), qtjambi_cast<K>(env, defaultKey)));
        }; }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool supported>
struct BiContainerKeys{
    static QMapKeysFunction function() { return nullptr; }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerKeys<Container, K, T, true>{
    static QMapKeysFunction function() { return  [](JNIEnv *env, const void* ptr) -> jobject {
            Q_UNUSED(env)
            const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
            return qtjambi_cast<jobject>(env, container->keys());
        }; }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool supported>
struct BiContainerKeysForValue{
    static QMapKeysForValueFunction function() { return nullptr; }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerKeysForValue<Container, K, T, true>{
    static QMapKeysForValueFunction function() { return  [](JNIEnv *env, const void* ptr, jobject value) -> jobject {
            Q_UNUSED(env)
            const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
            return qtjambi_cast<jobject>(env, container->keys(qtjambi_cast<T>(env, value)));
        }; }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool supported>
struct BiContainerLowerBound{
    static QMapLowerBoundFunction function() { return nullptr; }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerLowerBound<Container, K, T, true>{
    static QMapLowerBoundFunction function() { return  [](JNIEnv *env, QtJambiNativeID nativeId, const void* ptr, jobject key) -> jobject {
            Q_UNUSED(env)
            const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
            return qtjambi_BiIterator_caster<typename Container<K,T>::const_iterator>::castfwd(env, nativeId, container->lowerBound(qtjambi_cast<K>(env, key)));
        }; }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool supported>
struct BiContainerUpperBound{
    static QMapUpperBoundFunction function() { return nullptr; }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerUpperBound<Container, K, T, true>{
    static QMapUpperBoundFunction function() { return  [](JNIEnv *env, QtJambiNativeID nativeId, const void* ptr, jobject key) -> jobject {
            Q_UNUSED(env)
            const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
            return qtjambi_BiIterator_caster<typename Container<K,T>::const_iterator>::castfwd(env, nativeId, container->upperBound(qtjambi_cast<K>(env, key)));
        }; }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool canEquals, IsBiContainerFunction isContainer>
struct BiContainerEquals{
    static QMapEqualFunction function() { return nullptr; }
};

template<template<typename K, typename T> class Container, typename K, typename T, IsBiContainerFunction isContainer>
struct BiContainerEquals<Container, K, T, true, isContainer>{
    static QMapEqualFunction function() { return  [](JNIEnv * env, const void* ptr, jobject other) -> jboolean {
        Q_UNUSED(env)
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        QScopedPointer<Container<K,T> > __qt_scoped_pointer;
        Container<K,T> *__qt_other_pointer = nullptr;
        if (other!= nullptr) {
            if (isContainer(env, other, qtjambi_type<K>::id(), qtjambi_type<T>::id())) {
                __qt_other_pointer = reinterpret_cast<Container<K,T> *>(qtjambi_to_object(env, other));
            } else {
                __qt_scoped_pointer.reset(new Container<K,T>());
                __qt_other_pointer = __qt_scoped_pointer.data();
                jobject iterator = qtjambi_map_entryset_iterator(env, other);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jobject entry = qtjambi_iterator_next(env, iterator);
                    jobject key = qtjambi_map$entry_key(env, entry);
                    jobject val = qtjambi_map$entry_value(env, entry);
                    __qt_other_pointer->insert( qtjambi_cast<K>(env, key), qtjambi_cast<T>(env, val));
                }
            }
        }else{
            __qt_scoped_pointer.reset(new Container<K,T> ());
            __qt_other_pointer = __qt_scoped_pointer.data();
        }
        const Container<K,T>& __qt_other = *__qt_other_pointer;
        return (*container)==__qt_other;
    }; }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool canLess>
struct BiContainerSize{
    static QMapSizeFunction function() { return nullptr; }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerSize<Container, K, T, true>{
    static QMapSizeFunction function() { return  [](JNIEnv * env, const void* ptr) -> jint {
        Q_UNUSED(env)
        const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
        return container->size();
    }; }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool canLess>
struct BiContainerUniqueKeys{
    static QMapUniqueKeysFunction function() { return nullptr; }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerUniqueKeys<Container, K, T, true>{
    static QMapUniqueKeysFunction function() { return  [](JNIEnv *env, const void* ptr) -> jobject {
            Q_UNUSED(env)
            const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
            return qtjambi_cast<jobject>(env, container->uniqueKeys());
        }; }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool supported>
struct BiContainerValue{
    static QMapValueFunction function() { return nullptr; }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerValue<Container, K, T, true>{
    static QMapValueFunction function() { return  [](JNIEnv * env, const void* ptr, jobject key, jobject defaultValue) -> jobject {
            Q_UNUSED(env)
            const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
            return qtjambi_cast<jobject>(env, container->value(qtjambi_cast<K>(env, key), qtjambi_cast<T>(env, defaultValue)));
        }; }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool supported>
struct BiContainerValues{
    static QMapValuesFunction function() { return nullptr; }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerValues<Container, K, T, true>{
    static QMapValuesFunction function() { return  [](JNIEnv * env, const void* ptr) -> jobject {
            Q_UNUSED(env)
            const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
            return qtjambi_cast<jobject>(env, container->values());
        }; }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool hasHash>
struct BiContainerValuesKey{
    static QMapValuesKeyFunction function() { return nullptr; }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerValuesKey<Container, K, T, true>{
    static QMapValuesKeyFunction function() { return  [](JNIEnv * env, const void* ptr, jobject key) -> jobject {
            Q_UNUSED(env)
            const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
            return qtjambi_cast<jobject>(env, container->values(qtjambi_cast<K>(env, key)));
        }; }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool supported>
struct BiContainerClear{
    static QMapClearFunction function() {return nullptr;}
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerClear<Container, K, T, true>{
    static QMapClearFunction function() {
        return [](JNIEnv * __jni_env, void* ptr) {
                Q_UNUSED(__jni_env)
                Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
                container->clear();
            };
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool supported>
struct BiContainerInsert{
    static QMapInsertFunction function() { return nullptr; }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerInsert<Container, K, T, true>{
    static QMapInsertFunction function() {
        return [](JNIEnv * __jni_env, void* ptr, jobject key, jobject value) {
                Q_UNUSED(__jni_env)
                Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
                container->insert(qtjambi_cast<K>(__jni_env, key), qtjambi_cast<T>(__jni_env, value));
            };
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool supported>
struct BiContainerRemove{
    static QMapRemoveAllFunction function() { return nullptr; }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerRemove<Container, K, T, true>{
    static QMapRemoveAllFunction function() {
        return [](JNIEnv * __jni_env, void* ptr, jobject key) -> jint {
                Q_UNUSED(__jni_env)
                Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
                return container->remove(qtjambi_cast<K>(__jni_env, key));
            };
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool supported>
struct BiContainerTake{
    static QMapTakeFunction function() {
        return nullptr;
    }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerTake<Container, K, T, true>{
    static QMapTakeFunction function() {
        return [](JNIEnv * env, void* ptr, jobject key) -> jobject {
                Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
                return qtjambi_cast<jobject>(env, container->take(qtjambi_cast<K>(env, key)));
            };
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool supported, IsBiContainerFunction isContainer>
struct BiContainerUnite{
    static QMapUniteFunction function() { return nullptr; }
};

template<template<typename K, typename T> class Container, typename K, typename T, IsBiContainerFunction isContainer>
struct BiContainerUnite<Container, K, T, true, isContainer>{
    static QMapUniteFunction function() { return  [](JNIEnv * env, void* ptr, jobject other) {
                Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
                QScopedPointer<Container<K,T> > __qt_scoped_pointer;
                Container<K,T> *__qt_other_pointer = nullptr;
                if (other!= nullptr) {
                    if (isContainer(env, other, qtjambi_type<K>::id(), qtjambi_type<T>::id())) {
                        __qt_other_pointer = reinterpret_cast<Container<K,T> *>(qtjambi_to_object(env, other));
                    } else {
                        __qt_scoped_pointer.reset(new Container<K,T> ());
                        __qt_other_pointer = __qt_scoped_pointer.data();
                        jobject iterator = qtjambi_map_entryset_iterator(env, other);
                        while(qtjambi_iterator_has_next(env, iterator)) {
                            jobject entry = qtjambi_iterator_next(env, iterator);
                            jobject key = qtjambi_map$entry_key(env, entry);
                            jobject val = qtjambi_map$entry_value(env, entry);
                            __qt_other_pointer->insert(qtjambi_cast<K>(env, key), qtjambi_cast<T>(env, val));
                        }
                    }
                }else{
                    __qt_scoped_pointer.reset(new Container<K,T> ());
                    __qt_other_pointer = __qt_scoped_pointer.data();
                }
                container->unite(*__qt_other_pointer);
            }; }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool canEquals>
struct BiContainerContainsPair{
    static QMultiMapContainsPairFunction function() { return nullptr; }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerContainsPair<Container, K, T, true>{
    static QMultiMapContainsPairFunction function() {
        return [](JNIEnv * env, const void* ptr, jobject key, jobject object) -> jboolean {
                const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
                return container->contains(qtjambi_cast<K>(env, key), qtjambi_cast<T>(env, object));
            };
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool canEquals>
struct BiContainerCountPair{
    static QMultiMapCountPairFunction function() { return nullptr; }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerCountPair<Container, K, T, true>{
    static QMultiMapCountPairFunction function() {
        return [](JNIEnv * env, const void* ptr, jobject key, jobject object) -> jint {
                const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
                return container->count(qtjambi_cast<K>(env, key), qtjambi_cast<T>(env, object));
            };
    }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool canLess>
struct BiContainerFindPairs{
    static QMultiMapFindPairFunction function() { return nullptr; }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerFindPairs<Container, K, T, true>{
    static QMultiMapFindPairFunction function() {
            return [](JNIEnv *env, QtJambiNativeID nativeId, const void* ptr, jobject key, jobject object) -> jobject {
                const Container<K,T> *container = static_cast<const Container<K,T> *>(ptr);
                return qtjambi_BiIterator_caster<typename Container<K,T>::const_iterator>::castfwd(env, nativeId, container->find(qtjambi_cast<K>(env, key), qtjambi_cast<T>(env, object)));
            };
        }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool canEquals>
struct BiContainerRemovePair{
    static QMultiMapRemovePairFunction function() { return nullptr; }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerRemovePair<Container, K, T, true>{
    static QMultiMapRemovePairFunction function() {
        return [](JNIEnv * env, void* ptr, jobject key, jobject object) -> jint {
                Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
                return container->remove(qtjambi_cast<K>(env, key), qtjambi_cast<T>(env, object));
            };
        }
};

template<template<typename K, typename T> class Container, typename K, typename T, bool canEquals>
struct BiContainerReplacePair{
    static QMultiMapReplaceFunction function() { return nullptr; }
};

template<template<typename K, typename T> class Container, typename K, typename T>
struct BiContainerReplacePair<Container, K, T, true>{
    static QMultiMapReplaceFunction function() {
        return [](JNIEnv * env, void* ptr, jobject key, jobject object) {
                Container<K,T> *container = static_cast<Container<K,T> *>(ptr);
                container->replace(qtjambi_cast<K>(env, key), qtjambi_cast<T>(env, object));
            };
        }
};

template<bool c_is_pointer, typename K, typename T>
struct qtjambi_QMap_caster<c_is_pointer, true, K, T>{
    typedef const QMap<K,T> Container;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;
    static constexpr bool supported = supports_less_than<K>::value && supports_equal<K>::value;
    static constexpr bool supports_value_equals = supports_equal<T>::value;

    static jobject castfwd(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
        jobject result = qtjambi_to_constQMap(env,
                                              scope ? scope->relatedNativeID() : InvalidNativeID,
                                              ref_ptr<c_is_pointer, Container>::ref(in),
                                              qtjambi_type<K>::id(),
                                              qtjambi_type<T>::id(),
                                              QMapBeginFunction(BiContainerBegin<QMap, K, T, supported>::function()),
                                              QMapContainsFunction(BiContainerContains<QMap, K, T, supported>::function()),
                                              QMapCountObjectFunction(BiContainerCountObject<QMap, K, T, supported>::function()),
                                              QMapEndFunction(BiContainerEnd<QMap, K, T, supported>::function()),
                                              QMapFindFunction(BiContainerFindPair<QMap, K, T, supported>::function()),
                                              QMapFirstFunction(BiContainerFirst<QMap, K, T, supported>::function()),
                                              QMapFirstKeyFunction(BiContainerFirstKey<QMap, K, T, supported>::function()),
                                              QMapKeyFunction(BiContainerKey<QMap, K, T, supported && supports_value_equals>::function()),
                                              QMapKeysFunction(BiContainerKeys<QMap, K, T, supported>::function()),
                                              QMapKeysForValueFunction(BiContainerKeysForValue<QMap, K, T, supported && supports_value_equals>::function()),
                                              QMapLastFunction(BiContainerLast<QMap, K, T, supported>::function()),
                                              QMapLastKeyFunction(BiContainerLastKey<QMap, K, T, supported>::function()),
                                              QMapLowerBoundFunction(BiContainerLowerBound<QMap, K, T, supported>::function()),
                                              QMapEqualFunction(BiContainerEquals<QMap, K, T, supported && supports_value_equals, qtjambi_is_QMap>::function()),
                                              QMapSizeFunction(BiContainerSize<QMap, K, T, supported>::function()),
                                              QMapUniqueKeysFunction(BiContainerUniqueKeys<QMap, K, T, supported>::function()),
                                              QMapUpperBoundFunction(BiContainerUpperBound<QMap, K, T, supported>::function()),
                                              QMapValueFunction(BiContainerValue<QMap, K, T, supported>::function()),
                                              QMapValuesFunction(BiContainerValues<QMap, K, T, supported>::function()),
                                              QMapValuesKeyFunction(BiContainerValuesKey<QMap, K, T, supported>::function())
                                 );
        if(scope && !scope->relatedNativeID()){
            scope->addFinalAction([env, result](){ qtjambi_invalidate_object(env, result); });
        }
        return result;
    }
};

template<bool c_is_pointer, typename K, typename T>
struct qtjambi_QMap_caster<c_is_pointer, false, K, T>{
    typedef QMap<K,T> Container;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;
    typedef typename std::remove_reference<T>::type T_noref;
    typedef typename std::remove_cv<T_noref>::type T_noconst;
    static constexpr bool supported = supports_less_than<K>::value && supports_equal<K>::value;
    static constexpr bool supports_value_equals = supports_equal<T>::value;

    static jobject castfwd(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
        jobject result = qtjambi_to_QMap(env,
                                         scope ? scope->relatedNativeID() : InvalidNativeID,
                                         ref_ptr<c_is_pointer, Container>::ref(in),
                                         qtjambi_type<K>::id(),
                                         qtjambi_type<T>::id(),
                                         QMapBeginFunction(BiContainerBegin<QMap, K, T, supported>::function()),
                                         QMapClearFunction(BiContainerClear<QMap, K, T, supported>::function()),
                                         QMapContainsFunction(BiContainerContains<QMap, K, T, supported>::function()),
                                         QMapCountObjectFunction(BiContainerCountObject<QMap, K, T, supported>::function()),
                                         QMapEndFunction(BiContainerEnd<QMap, K, T, supported>::function()),
                                         QMapFindFunction(BiContainerFindPair<QMap, K, T, supported>::function()),
                                         QMapFirstFunction(BiContainerFirst<QMap, K, T, supported>::function()),
                                         QMapFirstKeyFunction(BiContainerFirstKey<QMap, K, T, supported>::function()),
                                         QMapInsertFunction(BiContainerInsert<QMap, K, T, supported>::function()),
                                         QMapKeyFunction(BiContainerKey<QMap, K, T, supported && supports_value_equals>::function()),
                                         QMapKeysFunction(BiContainerKeys<QMap, K, T, supported>::function()),
                                         QMapKeysForValueFunction(BiContainerKeysForValue<QMap, K, T, supports_equal<T>::value>::function()),
                                         QMapLastFunction(BiContainerLast<QMap, K, T, supported>::function()),
                                         QMapLastKeyFunction(BiContainerLastKey<QMap, K, T, supported>::function()),
                                         QMapLowerBoundFunction(BiContainerLowerBound<QMap, K, T, supported>::function()),
                                         QMapEqualFunction(BiContainerEquals<QMap, K, T, supported && supports_value_equals, qtjambi_is_QMap>::function()),
                                         QMapRemoveAllFunction(BiContainerRemove<QMap, K, T, supported>::function()),
                                         QMapSizeFunction(BiContainerSize<QMap, K, T, supported>::function()),
                                         QMapTakeFunction(BiContainerTake<QMap, K, T, supported && supports_StandardConstructor<T>::value>::function()),
                                         QMapUniqueKeysFunction(BiContainerUniqueKeys<QMap, K, T, supported>::function()),
                                         QMapUniteFunction(BiContainerUnite<QMap, K, T, supported, qtjambi_is_QMap>::function()),
                                         QMapUpperBoundFunction(BiContainerUpperBound<QMap, K, T, supported>::function()),
                                         QMapValueFunction(BiContainerValue<QMap, K, T, supported>::function()),
                                         QMapValuesFunction(BiContainerValues<QMap, K, T, supported>::function()),
                                         QMapValuesKeyFunction(BiContainerValuesKey<QMap, K, T, supported>::function())
                            );
        if(scope && !scope->relatedNativeID()){
            scope->addFinalAction([env, result](){ qtjambi_invalidate_object(env, result); });
        }
        return result;
    }
};

template<bool c_is_pointer, typename K, typename T>
struct qtjambi_QMultiMap_caster<c_is_pointer, true, K, T>{
    typedef const QMultiMap<K,T> Container;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;
    static constexpr bool supported = supports_less_than<K>::value && supports_equal<K>::value;
    static constexpr bool supports_value_equals = supports_equal<T>::value;

    static jobject castfwd(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
        jobject result = qtjambi_to_constQMultiMap(env,
                                                  scope ? scope->relatedNativeID() : InvalidNativeID,
                                                  ref_ptr<c_is_pointer, Container>::ref(in),
                                                  qtjambi_type<K>::id(),
                                                  qtjambi_type<T>::id(),
                                                  BiContainerBegin<QMultiMap, K, T, supported>::function(),
                                                  BiContainerContains<QMultiMap, K, T, supported>::function(),
                                                  BiContainerCountObject<QMultiMap, K, T, supported>::function(),
                                                  BiContainerEnd<QMultiMap, K, T, supported>::function(),
                                                  BiContainerFindPair<QMultiMap, K, T, supported>::function(),
                                                  BiContainerFirst<QMultiMap, K, T, supported>::function(),
                                                  BiContainerFirstKey<QMultiMap, K, T, supported>::function(),
                                                  BiContainerKey<QMultiMap, K, T, supported && supports_value_equals>::function(),
                                                  BiContainerKeys<QMultiMap, K, T, supported>::function(),
                                                  BiContainerKeysForValue<QMultiMap, K, T, supported && supports_value_equals>::function(),
                                                  BiContainerLast<QMultiMap, K, T, supported>::function(),
                                                  BiContainerLastKey<QMultiMap, K, T, supported>::function(),
                                                  BiContainerLowerBound<QMultiMap, K, T, supported>::function(),
                                                  BiContainerEquals<QMultiMap, K, T, supported && supports_value_equals, qtjambi_is_QMultiMap>::function(),
                                                  BiContainerSize<QMultiMap, K, T, supported>::function(),
                                                  BiContainerUniqueKeys<QMultiMap, K, T, supported>::function(),
                                                  BiContainerUpperBound<QMultiMap, K, T, supported>::function(),
                                                  BiContainerValue<QMultiMap, K, T, supported>::function(),
                                                  BiContainerValues<QMultiMap, K, T, supported>::function(),
                                                  BiContainerValuesKey<QMultiMap, K, T, supported && supports_value_equals>::function(),
                                                  BiContainerContainsPair<QMultiMap, K, T, supported && supports_value_equals>::function(),
                                                  BiContainerCountPair<QMultiMap, K, T, supported && supports_value_equals>::function(),
                                                  BiContainerFindPairs<QMultiMap, K, T, supported && supports_value_equals>::function()
                                 );
        if(scope && !scope->relatedNativeID()){
            scope->addFinalAction([env, result](){ qtjambi_invalidate_object(env, result); });
        }
        return result;
    }
};

template<bool c_is_pointer, typename K, typename T>
struct qtjambi_QMultiMap_caster<c_is_pointer, false, K, T>{
    typedef QMultiMap<K,T> Container;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;
    typedef typename std::remove_reference<T>::type T_noref;
    typedef typename std::remove_cv<T_noref>::type T_noconst;
    static constexpr bool supported = supports_less_than<K>::value && supports_equal<K>::value;
    static constexpr bool supports_value_equals = supports_equal<T>::value;

    static jobject castfwd(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
        jobject result = qtjambi_to_QMultiMap(env,
                                         scope ? scope->relatedNativeID() : InvalidNativeID,
                                         ref_ptr<c_is_pointer, Container>::ref(in),
                                         qtjambi_type<K>::id(),
                                         qtjambi_type<T>::id(),
                                         BiContainerBegin<QMultiMap, K, T, supported>::function(),
                                         BiContainerClear<QMultiMap, K, T, supported>::function(),
                                         BiContainerContains<QMultiMap, K, T, supported>::function(),
                                         BiContainerCountObject<QMultiMap, K, T, supported>::function(),
                                         BiContainerEnd<QMultiMap, K, T, supported>::function(),
                                         BiContainerFindPair<QMultiMap, K, T, supported>::function(),
                                         BiContainerFirst<QMultiMap, K, T, supported>::function(),
                                         BiContainerFirstKey<QMultiMap, K, T, supported>::function(),
                                         BiContainerInsert<QMultiMap, K, T, supported>::function(),
                                         BiContainerKey<QMultiMap, K, T, supported && supports_value_equals>::function(),
                                         BiContainerKeys<QMultiMap, K, T, supported>::function(),
                                         BiContainerKeysForValue<QMultiMap, K, T, supported && supports_value_equals>::function(),
                                         BiContainerLast<QMultiMap, K, T, supported>::function(),
                                         BiContainerLastKey<QMultiMap, K, T, supported>::function(),
                                         BiContainerLowerBound<QMultiMap, K, T, supported>::function(),
                                         BiContainerEquals<QMultiMap, K, T, supported && supports_value_equals, qtjambi_is_QMultiMap>::function(),
                                         BiContainerRemove<QMultiMap, K, T, supported>::function(),
                                         BiContainerSize<QMultiMap, K, T, supported>::function(),
                                         BiContainerTake<QMultiMap, K, T, supported && supports_StandardConstructor<T>::value>::function(),
                                         BiContainerUniqueKeys<QMultiMap, K, T, supported>::function(),
                                         BiContainerUnite<QMultiMap, K, T, supported, qtjambi_is_QMultiMap>::function(),
                                         BiContainerUpperBound<QMultiMap, K, T, supported>::function(),
                                         BiContainerValue<QMultiMap, K, T, supported>::function(),
                                         BiContainerValues<QMultiMap, K, T, supported>::function(),
                                         BiContainerValuesKey<QMultiMap, K, T, supported>::function(),
                                         BiContainerContainsPair<QMultiMap, K, T, supported && supports_value_equals>::function(),
                                         BiContainerCountPair<QMultiMap, K, T, supported && supports_value_equals>::function(),
                                         BiContainerFindPairs<QMultiMap, K, T, supported && supports_value_equals>::function(),
                                         BiContainerRemovePair<QMultiMap, K, T, supported && supports_value_equals>::function(),
                                         BiContainerReplacePair<QMultiMap, K, T, supported && supports_value_equals>::function()
                );
        if(scope && !scope->relatedNativeID()){
            scope->addFinalAction([env, result](){ qtjambi_invalidate_object(env, result); });
        }
        return result;
    }
};

template<bool c_is_pointer, typename K, typename T>
struct qtjambi_QHash_caster<c_is_pointer, true, K, T>{
    typedef const QHash<K,T> Container;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;
    static constexpr bool supported = supports_qHash<K>::value && supports_equal<K>::value;
    static constexpr bool supports_value_equals = supports_equal<T>::value;

    static jobject castfwd(JNIEnv *env, const Container_in in, const char*, QtJambiScope* scope){
        jobject result = qtjambi_to_constQHash(env,
                                              scope ? scope->relatedNativeID() : InvalidNativeID,
                                              ref_ptr<c_is_pointer, Container>::ref(in),
                                              qtjambi_type<K>::id(),
                                              qtjambi_type<T>::id(),
                                              QHashBeginFunction(BiContainerBegin<QHash, K, T, supported>::function()),
                                              QHashCapacityFunction(BiContainerCapacity<QHash, K, T, supported>::function()),
                                              QHashContainsFunction(BiContainerContains<QHash, K, T, supported>::function()),
                                              QHashCountObjectFunction(BiContainerCountObject<QHash, K, T, supported>::function()),
                                              QHashEndFunction(BiContainerEnd<QHash, K, T, supported>::function()),
                                              QHashFindFunction(BiContainerFindPair<QHash, K, T, supported>::function()),
                                              QHashKeyFunction(BiContainerKey<QHash, K, T, supported && supports_value_equals>::function()),
                                              QHashKeysFunction(BiContainerKeys<QHash, K, T, supported>::function()),
                                              QHashKeysForValueFunction(BiContainerKeysForValue<QHash, K, T, supported && supports_value_equals>::function()),
                                              QHashEqualFunction(BiContainerEquals<QHash, K, T, supported && supports_value_equals, qtjambi_is_QHash>::function()),
                                              QHashSizeFunction(BiContainerSize<QHash, K, T, supported>::function()),
                                              QHashUniqueKeysFunction(BiContainerUniqueKeys<QHash, K, T, supported>::function()),
                                              QHashValueFunction(BiContainerValue<QHash, K, T, supported>::function()),
                                              QHashValuesFunction(BiContainerValues<QHash, K, T, supported>::function()),
                                              QHashValuesKeyFunction(BiContainerValuesKey<QHash, K, T, supported>::function())
                                 );
        if(scope && !scope->relatedNativeID()){
            scope->addFinalAction([env, result](){ qtjambi_invalidate_object(env, result); });
        }
        return result;
    }
};

template<bool c_is_pointer, typename K, typename T>
struct qtjambi_QHash_caster<c_is_pointer, false, K, T>{
    typedef QHash<K,T> Container;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;
    typedef typename std::remove_reference<T>::type T_noref;
    typedef typename std::remove_cv<T_noref>::type T_noconst;
    static constexpr bool supported = supports_qHash<K>::value && supports_equal<K>::value;
    static constexpr bool supports_value_equals = supports_equal<T>::value;

    static jobject castfwd(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
        jobject result = qtjambi_to_QHash(env,
                                         scope ? scope->relatedNativeID() : InvalidNativeID,
                                         ref_ptr<c_is_pointer, Container>::ref(in),
                                         qtjambi_type<K>::id(),
                                         qtjambi_type<T>::id(),
                                         QHashBeginFunction(BiContainerBegin<QHash, K, T, supported>::function()),
                                         QHashCapacityFunction(BiContainerCapacity<QHash, K, T, supported>::function()),
                                         QHashClearFunction(BiContainerClear<QHash, K, T, supported>::function()),
                                         QHashContainsFunction(BiContainerContains<QHash, K, T, supported>::function()),
                                         QHashCountObjectFunction(BiContainerCountObject<QHash, K, T, supported>::function()),
                                         QHashEndFunction(BiContainerEnd<QHash, K, T, supported>::function()),
                                         QHashFindFunction(BiContainerFindPair<QHash, K, T, supported>::function()),
                                         QHashInsertFunction(BiContainerInsert<QHash, K, T, supported>::function()),
                                         QHashKeyFunction(BiContainerKey<QHash, K, T, supported && supports_value_equals>::function()),
                                         QHashKeysFunction(BiContainerKeys<QHash, K, T, supported>::function()),
                                         QHashKeysForValueFunction(BiContainerKeysForValue<QHash, K, T, supported && supports_value_equals>::function()),
                                         QHashEqualFunction(BiContainerEquals<QHash, K, T, supported && supports_value_equals, qtjambi_is_QHash>::function()),
                                         QHashRemoveAllFunction(BiContainerRemove<QHash, K, T, supported>::function()),
                                         QHashReserveFunction(BiContainerReserve<QHash, K, T, supported>::function()),
                                         QHashSizeFunction(BiContainerSize<QHash, K, T, supported>::function()),
                                         QHashTakeFunction(BiContainerTake<QHash, K, T, supported && supports_StandardConstructor<T>::value>::function()),
                                         QHashUniqueKeysFunction(BiContainerUniqueKeys<QHash, K, T, supported>::function()),
                                         QHashUniteFunction(BiContainerUnite<QHash, K, T, supported, qtjambi_is_QHash>::function()),
                                         QHashValueFunction(BiContainerValue<QHash, K, T, supported>::function()),
                                         QHashValuesFunction(BiContainerValues<QHash, K, T, supported>::function()),
                                         QHashValuesKeyFunction(BiContainerValuesKey<QHash, K, T, supported>::function())
                            );
        if(scope && !scope->relatedNativeID()){
            scope->addFinalAction([env, result](){ qtjambi_invalidate_object(env, result); });
        }
        return result;
    }
};

template<bool c_is_pointer, typename K, typename T>
struct qtjambi_QMultiHash_caster<c_is_pointer, true, K, T>{
    typedef const QMultiHash<K,T> Container;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;
    static constexpr bool supported = supports_qHash<K>::value && supports_equal<K>::value;

    static constexpr bool supports_value_equals = supports_equal<T>::value;

    static jobject castfwd(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
        jobject result = qtjambi_to_constQMultiHash(env,
                                              scope ? scope->relatedNativeID() : InvalidNativeID,
                                              ref_ptr<c_is_pointer, Container>::ref(in),
                                              qtjambi_type<K>::id(),
                                              qtjambi_type<T>::id(),
                                              QHashBeginFunction(BiContainerBegin<QMultiHash, K, T, supported>::function()),
                                              QHashCapacityFunction(BiContainerCapacity<QMultiHash, K, T, supported>::function()),
                                              QHashContainsFunction(BiContainerContains<QMultiHash, K, T, supported>::function()),
                                              QHashCountObjectFunction(BiContainerCountObject<QMultiHash, K, T, supported>::function()),
                                              QHashEndFunction(BiContainerEnd<QMultiHash, K, T, supported>::function()),
                                              QHashFindFunction(BiContainerFindPair<QMultiHash, K, T, supported>::function()),
                                              QHashKeyFunction(BiContainerKey<QMultiHash, K, T, supported && supports_value_equals>::function()),
                                              QHashKeysFunction(BiContainerKeys<QMultiHash, K, T, supported>::function()),
                                              QHashKeysForValueFunction(BiContainerKeysForValue<QMultiHash, K, T, supported && supports_value_equals>::function()),
                                              QHashEqualFunction(BiContainerEquals<QMultiHash, K, T, supported && supports_value_equals, qtjambi_is_QMultiHash>::function()),
                                              QHashSizeFunction(BiContainerSize<QMultiHash, K, T, supported>::function()),
                                              QHashUniqueKeysFunction(BiContainerUniqueKeys<QMultiHash, K, T, supported>::function()),
                                              QHashValueFunction(BiContainerValue<QMultiHash, K, T, supported>::function()),
                                              QHashValuesFunction(BiContainerValues<QMultiHash, K, T, supported>::function()),
                                              QHashValuesKeyFunction(BiContainerValuesKey<QMultiHash, K, T, supported>::function()),
                                              BiContainerContainsPair<QMultiMap, K, T, supported && supports_value_equals>::function(),
                                              BiContainerCountPair<QMultiMap, K, T, supported && supports_value_equals>::function(),
                                              BiContainerFindPairs<QMultiMap, K, T, supported && supports_value_equals>::function()
                                 );
        if(scope && !scope->relatedNativeID()){
            scope->addFinalAction([env, result](){ qtjambi_invalidate_object(env, result); });
        }
        return result;
    }
};

template<bool c_is_pointer, typename K, typename T>
struct qtjambi_QMultiHash_caster<c_is_pointer, false, K, T>{
    typedef QMultiHash<K,T> Container;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;
    typedef typename std::remove_reference<T>::type T_noref;
    typedef typename std::remove_cv<T_noref>::type T_noconst;
    static constexpr bool supported = supports_qHash<K>::value && supports_equal<K>::value;
    static constexpr bool supports_value_equals = supports_equal<T>::value;

    static jobject castfwd(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
        jobject result = qtjambi_to_QMultiHash(env,
                                         scope ? scope->relatedNativeID() : InvalidNativeID,
                                         ref_ptr<c_is_pointer, Container>::ref(in),
                                         qtjambi_type<K>::id(),
                                         qtjambi_type<T>::id(),
                                         QHashBeginFunction(BiContainerBegin<QMultiHash, K, T, supported>::function()),
                                         QHashCapacityFunction(BiContainerCapacity<QMultiHash, K, T, supported>::function()),
                                         QHashClearFunction(BiContainerClear<QMultiHash, K, T, supported>::function()),
                                         QHashContainsFunction(BiContainerContains<QMultiHash, K, T, supported>::function()),
                                         QHashCountObjectFunction(BiContainerCountObject<QMultiHash, K, T, supported>::function()),
                                         QHashEndFunction(BiContainerEnd<QMultiHash, K, T, supported>::function()),
                                         QHashFindFunction(BiContainerFindPair<QMultiHash, K, T, supported>::function()),
                                         QHashInsertFunction(BiContainerInsert<QMultiHash, K, T, supported>::function()),
                                         QHashKeyFunction(BiContainerKey<QMultiHash, K, T, supported && supports_value_equals>::function()),
                                         QHashKeysFunction(BiContainerKeys<QMultiHash, K, T, supported>::function()),
                                         QHashKeysForValueFunction(BiContainerKeysForValue<QMultiHash, K, T, supported && supports_value_equals>::function()),
                                         QHashEqualFunction(BiContainerEquals<QMultiHash, K, T, supported && supports_value_equals, qtjambi_is_QMultiHash>::function()),
                                         QHashRemoveAllFunction(BiContainerRemove<QMultiHash, K, T, supported>::function()),
                                         QHashReserveFunction(BiContainerReserve<QMultiHash, K, T, supported>::function()),
                                         QHashSizeFunction(BiContainerSize<QMultiHash, K, T, supported>::function()),
                                         QHashTakeFunction(BiContainerTake<QMultiHash, K, T, supported && supports_StandardConstructor<T>::value>::function()),
                                         QHashUniqueKeysFunction(BiContainerUniqueKeys<QMultiHash, K, T, supported>::function()),
                                         QHashUniteFunction(BiContainerUnite<QMultiHash, K, T, supported, qtjambi_is_QMultiHash>::function()),
                                         QHashValueFunction(BiContainerValue<QMultiHash, K, T, supported>::function()),
                                         QHashValuesFunction(BiContainerValues<QMultiHash, K, T, supported>::function()),
                                         QHashValuesKeyFunction(BiContainerValuesKey<QMultiHash, K, T, supported>::function()),
                                         BiContainerContainsPair<QMultiMap, K, T, supported && supports_value_equals>::function(),
                                         BiContainerCountPair<QMultiMap, K, T, supported && supports_value_equals>::function(),
                                         BiContainerFindPairs<QMultiMap, K, T, supported && supports_value_equals>::function(),
                                         BiContainerRemovePair<QMultiMap, K, T, supported && supports_value_equals>::function(),
                                         BiContainerReplacePair<QMultiMap, K, T, supported && supports_value_equals>::function()
                            );
        if(scope && !scope->relatedNativeID()){
            scope->addFinalAction([env, result](){ qtjambi_invalidate_object(env, result); });
        }
        return result;
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
                qtjambi_scoped_cast<has_scope,jobject,decltype(i.key())>::cast(env, i.key(), nullptr, scope),
                qtjambi_scoped_cast<has_scope,jobject,decltype(i.value())>::cast(env, i.value(), nullptr, scope));
        }
    }
};

template<bool has_scope, typename Container, typename K, typename T, void (*put)(JNIEnv *, jobject, jobject, jobject)>
struct qtjambi_bicollection_fill<has_scope, Container, false, K, T, put>{
    static void fill(JNIEnv *env, Container& container, jobject collection, QtJambiScope* scope){
        for(typename Container::iterator i = container.begin(); i!=container.end(); ++i){
            put(env, collection,
                qtjambi_scoped_cast<has_scope,jobject,decltype(i.key())>::cast(env, i.key(), nullptr, scope),
                qtjambi_scoped_cast<has_scope,jobject,decltype(i.value())>::cast(env, i.value(), nullptr, scope));
        }
    }
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_MAP_P_H
