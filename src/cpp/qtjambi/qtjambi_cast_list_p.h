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

#ifndef QTJAMBI_CAST_LIST_P_H
#define QTJAMBI_CAST_LIST_P_H

#include "qtjambi_core.h"
#include "qtjambi_cast_util_p.h"
#include "qtjambi_cast_type_p.h"

QT_WARNING_DISABLE_DEPRECATED

namespace QtJambiPrivate {

typedef bool (*IsContainerFunction)(JNIEnv *, jobject, const std::type_info&);

template<bool c_is_pointer, bool c_is_const, typename T>
struct qtjambi_QList_caster{
    typedef QList<T> Container;
    typedef typename std::conditional<c_is_const, const Container, Container>::type Container_const;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container_const>::type, typename std::add_lvalue_reference<Container_const>::type>::type Container_in;
    static jobject castfwd(JNIEnv *, Container_in, const char*, QtJambiScope* ){
        Q_STATIC_ASSERT_X(false && !c_is_const, "Cannot cast types");
        return nullptr;
    }
};

template<bool c_is_pointer, bool c_is_const, typename T>
struct qtjambi_QSet_caster{
    typedef QSet<T> Container;
    typedef typename std::conditional<c_is_const, const Container, Container>::type Container_const;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container_const>::type, typename std::add_lvalue_reference<Container_const>::type>::type Container_in;
    static jobject castfwd(JNIEnv *, Container_in, const char*, QtJambiScope* ){
        Q_STATIC_ASSERT_X(false && !c_is_const, "Cannot cast types");
        return nullptr;
    }
};

template<bool c_is_pointer, bool c_is_const, typename T>
struct qtjambi_QLinkedList_caster{
    typedef QLinkedList<T> Container;
    typedef typename std::conditional<c_is_const, const Container, Container>::type Container_const;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container_const>::type, typename std::add_lvalue_reference<Container_const>::type>::type Container_in;
    static jobject castfwd(JNIEnv *, Container_in, const char*, QtJambiScope* ){
        Q_STATIC_ASSERT_X(false && !c_is_const, "Cannot cast types");
        return nullptr;
    }
};

template<bool c_is_pointer, bool c_is_const, typename T>
struct qtjambi_QStack_caster{
    typedef QStack<T> Container;
    typedef typename std::conditional<c_is_const, const Container, Container>::type Container_const;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container_const>::type, typename std::add_lvalue_reference<Container_const>::type>::type Container_in;
    static jobject castfwd(JNIEnv *, Container_in, const char*, QtJambiScope* ){
        Q_STATIC_ASSERT_X(false && !c_is_const, "Cannot cast types");
        return nullptr;
    }
};

template<bool c_is_pointer, bool c_is_const, typename T>
struct qtjambi_QQueue_caster{
    typedef QQueue<T> Container;
    typedef typename std::conditional<c_is_const, const Container, Container>::type Container_const;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container_const>::type, typename std::add_lvalue_reference<Container_const>::type>::type Container_in;
    static jobject castfwd(JNIEnv *, Container_in, const char*, QtJambiScope* ){
        Q_STATIC_ASSERT_X(false && !c_is_const, "Cannot cast types");
        return nullptr;
    }
};

template<bool c_is_pointer, bool c_is_const, typename T>
struct qtjambi_QVector_caster{
    typedef QVector<T> Container;
    typedef typename std::conditional<c_is_const, const Container, Container>::type Container_const;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container_const>::type, typename std::add_lvalue_reference<Container_const>::type>::type Container_in;
    static jobject castfwd(JNIEnv *, Container_in, const char*, QtJambiScope* ){
        Q_STATIC_ASSERT_X(false && !c_is_const, "Cannot cast types");
        return nullptr;
    }
};

template<typename Iterator>
struct IteratorValue{
    static QIteratorValueFunction function() { return  [](JNIEnv *env, void* ptr) -> jobject {
            Q_UNUSED(env)
            Iterator* iterator = static_cast<Iterator*>(ptr);
            return qtjambi_cast<jobject>(env, *(*iterator) );
        }; }
};

template<typename Iterator>
struct qtjambi_QIterator_caster{
    static jobject castfwd(JNIEnv * env, QtJambiNativeID __list_nativeId, Iterator&& iter){
        return qtjambi_to_QIterator(env, __list_nativeId,
            new Iterator(iter),
            IteratorValue<Iterator>::function(),
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

template<template<typename T> class Container, typename T>
struct ContainerToList{
    static QVectorToListFunction function() { return  [](JNIEnv * env, const void* ptr) -> jobject {
            Q_UNUSED(env)
            const Container<T>  *container = static_cast<const Container<T> *>(ptr);
            return qtjambi_cast<jobject>(env, container->toList());
        }; }
};

template<template<typename T> class Container, typename T, bool = supports_qHash<T>::value && supports_equal<T>::value>
struct ContainerToSet{
};

template<template<typename T> class Container, typename T>
struct ContainerToSet<Container, T, false>{
    static QListToSetFunction function() { return  nullptr; }
};

template<template<typename T> class Container, typename T>
struct ContainerToSet<Container, T, true>{
    static QListToSetFunction function() { return  [](JNIEnv * env, const void* ptr) -> jobject {
            const Container<T>  *container = static_cast<const Container<T> *>(ptr);
#if QT_VERSION < QT_VERSION_CHECK(5,14,0)
            return qtjambi_cast<jobject>(env, container->toSet());
#else
            return qtjambi_cast<jobject>(env, QSet<T>(container->begin(), container->end()));
#endif
        }; }
};

template<template<typename T> class Container, typename T, bool = supports_StandardConstructor<T>::value>
struct ContainerAt{
    static QListAtFunction function() { return nullptr; }
};

template<template<typename T> class Container, typename T>
struct ContainerAt<Container,T,true>{
    static QListAtFunction function() { return  [](JNIEnv * env, const void* ptr, jint idx) -> jobject {
            Q_UNUSED(env)
            const Container<T> *container = static_cast<const Container<T> *>(ptr);
            return qtjambi_cast<jobject>(env, container->at(int(idx)));
        }; }
};

template<template<typename T> class Container, typename T>
struct ContainerFirst{
    static QLinkedListFirstFunction function() { return  [](JNIEnv * env, const void* ptr) -> jobject {
            Q_UNUSED(env)
            const Container<T> *container = static_cast<const Container<T> *>(ptr);
            return qtjambi_cast<jobject>(env, container->first());
        }; }
};

template<template<typename T> class Container, typename T>
struct ContainerLast{
    static QLinkedListLastFunction function() { return  [](JNIEnv * env, const void* ptr) -> jobject {
            Q_UNUSED(env)
            const Container<T> *container = static_cast<const Container<T> *>(ptr);
            return qtjambi_cast<jobject>(env, container->last());
        }; }
};

template<template<typename T> class Container, typename T>
struct ContainerCapacity{
    static QSetCapacityFunction function() { return  [](JNIEnv *env, const void* ptr) -> jint {
            Q_UNUSED(env)
            const Container<T> *container = static_cast<const Container<T> *>(ptr);
            return jint(container->capacity());
        }; }
};

template<template<typename T> class Container, typename T>
struct ContainerBegin{
    static QListBeginFunction function() { return  [](JNIEnv *env, QtJambiNativeID __list_nativeId, const void* ptr) -> jobject {
            Q_UNUSED(env)
            const Container<T> *container = static_cast<const Container<T> *>(ptr);
            return qtjambi_QIterator_caster<typename Container<T>::const_iterator>::castfwd(env, __list_nativeId, container->begin());
        }; }
};

template<template<typename T> class Container, typename T, bool = supports_equal<T>::value>
struct ContainerContains{
    static QListContainsFunction function() { return  [](JNIEnv * env, const void* ptr, jobject object) -> jboolean {
            Q_UNUSED(env)
            const Container<T> *container = static_cast<const Container<T> *>(ptr);
            return container->contains(qtjambi_cast<T>(env, object));
        }; }
};

template<template<typename T> class Container, typename T>
struct ContainerContains<Container, T, false>{
    static QListContainsFunction function() { return nullptr; }
};

template<template<typename T> class Container, typename T, IsContainerFunction isContainer>
struct ContainerIntersects{
    static QSetIntersectsFunction function() { return  [](JNIEnv * env, const void* ptr, jobject other) -> jboolean {
                Q_UNUSED(env)
                const Container<T> *container = static_cast<const Container<T> *>(ptr);
                QScopedPointer<Container<T> > __qt_scoped_pointer;
                Container<T> *__qt_other_pointer = nullptr;
                if (other!= nullptr) {
                    if (isContainer(env, other, qtjambi_type<T>::id())) {
                        __qt_other_pointer = reinterpret_cast<Container<T> *>(qtjambi_to_object(env, other));
                    } else {
                        __qt_scoped_pointer.reset(new Container<T> ());
                        __qt_other_pointer = __qt_scoped_pointer.data();
                        jobject iterator = qtjambi_collection_iterator(env, other);
                        while(qtjambi_iterator_has_next(env, iterator)) {
                            jobject element = qtjambi_iterator_next(env, iterator);
                            (*__qt_other_pointer) << qtjambi_cast<T>(env, element);
                        }
                    }
                }else{
                    __qt_scoped_pointer.reset(new Container<T> ());
                    __qt_other_pointer = __qt_scoped_pointer.data();
                }
                return container->intersects(*__qt_other_pointer);
            }; }
};

template<template<typename T> class Container, typename T, IsContainerFunction isContainer>
struct ContainerIntersect{
    static QSetIntersectFunction function() { return  [](JNIEnv * env, void* ptr, jobject other) {
                Q_UNUSED(env)
                Container<T> *container = static_cast<Container<T> *>(ptr);
                QScopedPointer<Container<T> > __qt_scoped_pointer;
                Container<T> *__qt_other_pointer = nullptr;
                if (other!= nullptr) {
                    if (isContainer(env, other, qtjambi_type<T>::id())) {
                        __qt_other_pointer = reinterpret_cast<Container<T> *>(qtjambi_to_object(env, other));
                    } else {
                        __qt_scoped_pointer.reset(new Container<T> ());
                        __qt_other_pointer = __qt_scoped_pointer.data();
                        jobject iterator = qtjambi_collection_iterator(env, other);
                        while(qtjambi_iterator_has_next(env, iterator)) {
                            jobject element = qtjambi_iterator_next(env, iterator);
                            (*__qt_other_pointer) << qtjambi_cast<T>(env, element);
                        }
                    }
                }else{
                    __qt_scoped_pointer.reset(new Container<T> ());
                    __qt_other_pointer = __qt_scoped_pointer.data();
                }
                container->intersect(*__qt_other_pointer);
            }; }
};

template<template<typename T> class Container, typename T, IsContainerFunction isContainer>
struct ContainerUnite{
    static QSetUniteFunction function() { return  [](JNIEnv * env, void* ptr, jobject other) {
                Q_UNUSED(env)
                Container<T> *container = static_cast<Container<T> *>(ptr);
                QScopedPointer<Container<T> > __qt_scoped_pointer;
                Container<T> *__qt_other_pointer = nullptr;
                if (other!= nullptr) {
                    if (isContainer(env, other, qtjambi_type<T>::id())) {
                        __qt_other_pointer = reinterpret_cast<Container<T> *>(qtjambi_to_object(env, other));
                    } else {
                        __qt_scoped_pointer.reset(new Container<T> ());
                        __qt_other_pointer = __qt_scoped_pointer.data();
                        jobject iterator = qtjambi_collection_iterator(env, other);
                        while(qtjambi_iterator_has_next(env, iterator)) {
                            jobject element = qtjambi_iterator_next(env, iterator);
                            (*__qt_other_pointer) << qtjambi_cast<T>(env, element);
                        }
                    }
                }else{
                    __qt_scoped_pointer.reset(new Container<T> ());
                    __qt_other_pointer = __qt_scoped_pointer.data();
                }
                container->unite(*__qt_other_pointer);
            }; }
};

template<template<typename T> class Container, typename T, IsContainerFunction isContainer>
struct ContainerSubtract{
    static QSetSubtractFunction function() { return  [](JNIEnv * env, void* ptr, jobject other) {
                Q_UNUSED(env)
                Container<T> *container = static_cast<Container<T> *>(ptr);
                QScopedPointer<Container<T> > __qt_scoped_pointer;
                Container<T> *__qt_other_pointer = nullptr;
                if (other!= nullptr) {
                    if (isContainer(env, other, qtjambi_type<T>::id())) {
                        __qt_other_pointer = reinterpret_cast<Container<T> *>(qtjambi_to_object(env, other));
                    } else {
                        __qt_scoped_pointer.reset(new Container<T> ());
                        __qt_other_pointer = __qt_scoped_pointer.data();
                        jobject iterator = qtjambi_collection_iterator(env, other);
                        while(qtjambi_iterator_has_next(env, iterator)) {
                            jobject element = qtjambi_iterator_next(env, iterator);
                            (*__qt_other_pointer) << qtjambi_cast<T>(env, element);
                        }
                    }
                }else{
                    __qt_scoped_pointer.reset(new Container<T> ());
                    __qt_other_pointer = __qt_scoped_pointer.data();
                }
                container->subtract(*__qt_other_pointer);
            }; }
};

template<template<typename T> class Container, typename T, bool = supports_equal<T>::value>
struct ContainerCountObject{
    static QListCountObjectFunction function() { return nullptr; }
};

template<template<typename T> class Container, typename T>
struct ContainerCountObject<Container, T, true>{
    static QListCountObjectFunction function() { return  [](JNIEnv * env, const void* ptr, jobject object) -> jint {
            Q_UNUSED(env)
            const Container<T> *container = static_cast<const Container<T> *>(ptr);
            return container->count(qtjambi_cast<T>(env, object));
        }; }
};

template<template<typename T> class Container, typename T>
struct ContainerEnd{
    static QListEndFunction function() { return  [](JNIEnv *env, QtJambiNativeID __list_nativeId, const void* ptr) -> jobject {
            Q_UNUSED(env)
            const Container<T> *container = static_cast<const Container<T> *>(ptr);
            return qtjambi_QIterator_caster<typename Container<T>::const_iterator>::castfwd(env, __list_nativeId, container->end());
        }; }
};

template<template<typename T> class Container, typename T, bool = supports_equal<T>::value>
struct ContainerEndsWith{
    static QListEndsWithFunction function() { return nullptr; }
};

template<template<typename T> class Container, typename T>
struct ContainerEndsWith<Container, T, true>{
    static QListEndsWithFunction function() { return  [](JNIEnv * env, const void* ptr, jobject object) -> jboolean {
        Q_UNUSED(env)
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return container->endsWith(qtjambi_cast<T>(env, object));
    }; }
};

template<template<typename T> class Container, typename T, bool = supports_equal<T>::value>
struct ContainerIndexOf{
    static QListIndexOfFunction function() { return nullptr; }
};

template<template<typename T> class Container, typename T>
struct ContainerIndexOf<Container, T, true>{
    static QListIndexOfFunction function() { return  [](JNIEnv * env, const void* ptr, jobject object, jint idx) -> jint {
        Q_UNUSED(env)
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return container->indexOf(qtjambi_cast<T>(env, object), int(idx));
    }; }
};

template<template<typename T> class Container, typename T, bool = supports_equal<T>::value>
struct ContainerLastIndexOf{
    static QListLastIndexOfFunction function() { return nullptr; }
};

template<template<typename T> class Container, typename T>
struct ContainerLastIndexOf<Container, T, true>{
    static QListLastIndexOfFunction function() { return  [](JNIEnv * env, const void* ptr, jobject object, jint idx) -> jint {
        Q_UNUSED(env)
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return container->lastIndexOf(qtjambi_cast<T>(env, object), int(idx));
    }; }
};

template<template<typename T> class Container, typename T, bool = supports_StandardConstructor<T>::value>
struct ContainerMid{
    static QListMidFunction function() { return nullptr; }
};

template<template<typename T> class Container, typename T>
struct ContainerMid<Container, T, true>{
    static QListMidFunction function() { return  [](JNIEnv * env, const void* ptr, jint idx1, jint idx2) -> jobject {
        Q_UNUSED(env)
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return qtjambi_cast<jobject>(env, container->mid(int(idx1), int(idx2)));
    }; }
};

template<template<typename T> class Container, typename T, IsContainerFunction is_Container1_fct, bool = supports_equal<T>::value>
struct ContainerEquals{
    static QListEqualFunction function() { return  nullptr; }
};

template<template<typename T> class Container, typename T, IsContainerFunction is_Container1_fct>
struct ContainerEquals<Container, T, is_Container1_fct, true>{
    static QListEqualFunction function() { return  [](JNIEnv * env, const void* ptr, jobject other) -> jboolean {
        Q_UNUSED(env)
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        QScopedPointer<Container<T> > __qt_scoped_pointer;
        Container<T> *__qt_other_pointer = nullptr;
        if (other!= nullptr) {
            if (is_Container1_fct(env, other, qtjambi_type<T>::id())) {
                __qt_other_pointer = reinterpret_cast<Container<T> *>(qtjambi_to_object(env, other));
            } else {
                __qt_scoped_pointer.reset(new Container<T> ());
                __qt_other_pointer = __qt_scoped_pointer.data();
                jobject iterator = qtjambi_collection_iterator(env, other);
                while(qtjambi_iterator_has_next(env, iterator)) {
                    jobject element = qtjambi_iterator_next(env, iterator);
                    (*__qt_other_pointer) << qtjambi_cast<T>(env, element);
                }
            }
        }else{
            __qt_scoped_pointer.reset(new Container<T> ());
            __qt_other_pointer = __qt_scoped_pointer.data();
        }
        const Container<T>& __qt_other = *__qt_other_pointer;
        return (*container)==__qt_other;
    }; }
};

template<template<typename T> class Container, typename T>
struct ContainerSize{
    static QListSizeFunction function() { return  [](JNIEnv * env, const void* ptr) -> jint {
        Q_UNUSED(env)
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return container->size();
    }; }
};

template<template<typename T> class Container, typename T, bool = supports_equal<T>::value>
struct ContainerStartsWith{
    static QListStartsWithFunction function() { return nullptr; }
};

template<template<typename T> class Container, typename T>
struct ContainerStartsWith<Container, T, true>{
    static QListStartsWithFunction function() { return  [](JNIEnv * env, const void* ptr, jobject object) -> jboolean {
        Q_UNUSED(env)
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return container->startsWith(qtjambi_cast<T>(env, object));
    }; }
};

template<template<typename T> class Container, typename T, bool = supports_StandardConstructor<T>::value>
struct ContainerValue{
    static QListValueFunction function() { return nullptr; }
};

template<template<typename T> class Container, typename T>
struct ContainerValue<Container, T, true>{
    static QListValueFunction function() { return  [](JNIEnv * env, const void* ptr, jint index) -> jobject {
        Q_UNUSED(env)
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return qtjambi_cast<jobject>(env, container->value(index));
    }; }
};

template<template<typename T> class Container, typename T>
struct ContainerValueDefault{
    static QListValueDefaultFunction function() { return  [](JNIEnv * env, const void* ptr, jint index, jobject object) -> jobject {
        Q_UNUSED(env)
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return qtjambi_cast<jobject>(env, container->value(index, qtjambi_cast<T>(env, object)));
    }; }
};

template<template<typename T> class Container, typename T, bool = true>
struct ContainerAppend{
    static QListAppendFunction function() { return nullptr; }
};

template<template<typename T> class Container, typename T>
struct ContainerAppend<Container, T, true>{
    static QListAppendFunction function() { return [](JNIEnv * env, void* ptr, jobject object) {
            Q_UNUSED(env)
            Container<T> *container = static_cast<Container<T> *>(ptr);
            container->append(qtjambi_cast<T>(env, object));
        };
    }
};

template<template<typename T> class Container, typename T, bool = supports_StandardConstructor<T>::value>
struct ContainerFill{
    static QVectorFillFunction function() { return nullptr; }
};

template<template<typename T> class Container, typename T>
struct ContainerFill<Container, T, true>{
    static QVectorFillFunction function() { return [](JNIEnv * env, void* ptr, jobject object, jint size) {
            Q_UNUSED(env)
            Container<T> *container = static_cast<Container<T> *>(ptr);
            container->fill(qtjambi_cast<T>(env, object), size);
        };
    }
};

template<template<typename T> class Container, typename T, IsContainerFunction is_Container1_fct>
struct ContainerAppendList{
    static QListAppendListFunction function() {
        return [](JNIEnv * env, void* ptr, jobject object) {
                Q_UNUSED(env)
                Container<T> *container = static_cast<Container<T> *>(ptr);
                QScopedPointer<Container<T> > __qt_scoped_pointer;
                Container<T> *__qt_object_pointer = nullptr;
                if (object!= nullptr) {
                    if (is_Container1_fct(env, object, qtjambi_type<T>::id())) {
                        __qt_object_pointer = reinterpret_cast<Container<T> *>(qtjambi_to_object(env, object));
                    } else {
                        __qt_scoped_pointer.reset(new Container<T>());
                        __qt_object_pointer = __qt_scoped_pointer.data();
                        jobject iterator = qtjambi_collection_iterator(env, object);
                        while(qtjambi_iterator_has_next(env, iterator)) {
                            jobject element = qtjambi_iterator_next(env, iterator);
                            __qt_object_pointer->append(qtjambi_cast<T>(env, element));
                        }
                    }
                }else{
                    __qt_scoped_pointer.reset(new Container<T> ());
                    __qt_object_pointer = __qt_scoped_pointer.data();
                }
                container->append(*__qt_object_pointer);
            };
    }
};

template<template<typename T> class Container, typename T, IsContainerFunction is_Container1_fct, bool = supports_StandardConstructor<T>::value>
struct ContainerAppendVector{
    static QVectorAppendVectorFunction function() { return nullptr; }
};

template<template<typename T> class Container, typename T, IsContainerFunction is_Container1_fct>
struct ContainerAppendVector<Container, T, is_Container1_fct, true>{
    static QVectorAppendVectorFunction function() {
        return [](JNIEnv * env, void* ptr, jobject object) {
                Q_UNUSED(env)
                Container<T> *container = static_cast<Container<T> *>(ptr);
                QScopedPointer<Container<T> > __qt_scoped_pointer;
                Container<T> *__qt_object_pointer = nullptr;
                if (object!= nullptr) {
                    if (is_Container1_fct(env, object, qtjambi_type<T>::id())) {
                        __qt_object_pointer = reinterpret_cast<Container<T> *>(qtjambi_to_object(env, object));
                    } else {
                        __qt_scoped_pointer.reset(new Container<T>());
                        __qt_object_pointer = __qt_scoped_pointer.data();
                        jobject iterator = qtjambi_collection_iterator(env, object);
                        while(qtjambi_iterator_has_next(env, iterator)) {
                            jobject element = qtjambi_iterator_next(env, iterator);
                            __qt_object_pointer->append(qtjambi_cast<T>(env, element));
                        }
                    }
                }else{
                    __qt_scoped_pointer.reset(new Container<T> ());
                    __qt_object_pointer = __qt_scoped_pointer.data();
                }
                container->append(*__qt_object_pointer);
            };
    }
};

template<template<typename T> class Container, typename T, bool = true>
struct ContainerClear{
    static QListClearFunction function() { return nullptr; }
};

template<template<typename T> class Container, typename T>
struct ContainerClear<Container, T, true>{
    static QListClearFunction function() {
        return [](JNIEnv * env, void* ptr) {
                Q_UNUSED(env)
                Container<T> *container = static_cast<Container<T> *>(ptr);
                container->clear();
            };
    }
};

template<template<typename T> class Container, typename T, bool = true>
struct ContainerInsertAt{
    static QListInsertFunction function() { return nullptr; }
};

template<template<typename T> class Container, typename T>
struct ContainerInsertAt<Container, T, true>{
    static QListInsertFunction function() {
        return [](JNIEnv * env, void* ptr, jint i, jobject value) {
                Q_UNUSED(env)
                Container<T> *container = static_cast<Container<T> *>(ptr);
                container->insert(i, qtjambi_cast<T>(env, value));
            };
    }
};

template<template<typename T> class Container, typename T, bool = supports_StandardConstructor<T>::value>
struct ContainerInsertN{
    static QVectorInsertNFunction function() { return nullptr; }
};

template<template<typename T> class Container, typename T>
struct ContainerInsertN<Container, T, true>{
    static QVectorInsertNFunction function() {
        return [](JNIEnv * env, void* ptr, jint i, jint n, jobject value) {
                Q_UNUSED(env)
                Container<T> *container = static_cast<Container<T> *>(ptr);
                container->insert(i, n, qtjambi_cast<T>(env, value));
            };
    }
};

template<template<typename T> class Container, typename T>
struct ContainerInsert{
    static QSetInsertFunction function() {
        return [](JNIEnv * env, void* ptr, jobject value) {
                Q_UNUSED(env)
                Container<T> *container = static_cast<Container<T> *>(ptr);
                container->insert(qtjambi_cast<T>(env, value));
            };
    }
};

template<template<typename T> class Container, typename T, bool = true>
struct ContainerMove{
    static QListMoveFunction function() { return nullptr; }
};

template<template<typename T> class Container, typename T>
struct ContainerMove<Container, T, true>{
    static QListMoveFunction function() {
        return [](JNIEnv * env, void* ptr, jint idx1, jint idx2) {
                Q_UNUSED(env)
                Container<T> *container = static_cast<Container<T> *>(ptr);
                container->move(int(idx1), int(idx2));
            };
    }
};

template<template<typename T> class Container, typename T, bool = true>
struct ContainerPrepend{
    static QListPrependFunction function() { return nullptr; }
};

template<template<typename T> class Container, typename T>
struct ContainerPrepend<Container, T, true>{
    static QListPrependFunction function() {
        return [](JNIEnv * env, void* ptr, jobject object) {
                Q_UNUSED(env)
                Container<T> *container = static_cast<Container<T> *>(ptr);
                container->prepend(qtjambi_cast<T>(env, object));
            };
    }
};

template<template<typename T> class Container, typename T, bool = supports_equal<T>::value>
struct ContainerRemoveAll{
    static QListRemoveAllFunction function() {
        return nullptr;
    }
};

template<template<typename T> class Container, typename T>
struct ContainerRemoveAll<Container, T, true>{
    static QListRemoveAllFunction function() {
        return [](JNIEnv * env, void* ptr, jobject object) -> jint {
                Q_UNUSED(env)
                Container<T> *container = static_cast<Container<T> *>(ptr);
                return container->removeAll(qtjambi_cast<T>(env, object));
            };
    }
};

template<template<typename T> class Container, typename T>
struct ContainerTakeFirst{
    static QLinkedListTakeFirstFunction function() {
        return [](JNIEnv * env, void* ptr) -> jobject {
                Q_UNUSED(env)
                Container<T> *container = static_cast<Container<T> *>(ptr);
                return qtjambi_cast<jobject>(container->takeFirst());
            };
    }
};

template<template<typename T> class Container, typename T>
struct ContainerTakeLast{
    static QLinkedListTakeLastFunction function() {
        return [](JNIEnv * env, void* ptr) -> jobject {
                Q_UNUSED(env)
                Container<T> *container = static_cast<Container<T> *>(ptr);
                return qtjambi_cast<jobject>(container->takeLast());
            };
    }
};

template<template<typename T> class Container, typename T>
struct ContainerRemoveFirst{
    static QLinkedListRemoveFirstFunction function() {
        return [](JNIEnv * env, void* ptr) {
                Q_UNUSED(env)
                Container<T> *container = static_cast<Container<T> *>(ptr);
                container->removeFirst();
            };
    }
};

template<template<typename T> class Container, typename T>
struct ContainerRemoveLast{
    static QLinkedListRemoveLastFunction function() {
        return [](JNIEnv * env, void* ptr) {
                Q_UNUSED(env)
                Container<T> *container = static_cast<Container<T> *>(ptr);
                container->removeLast();
            };
    }
};

template<template<typename T> class Container, typename T>
struct ContainerRemoveAt{
    static QListRemoveAtFunction function() {
        return [](JNIEnv * env, void* ptr, jint idx) {
                Q_UNUSED(env)
                Container<T> *container = static_cast<Container<T> *>(ptr);
                container->removeAt(idx);
            };
    }
};

template<template<typename T> class Container, typename T, bool = supports_StandardConstructor<T>::value>
struct ContainerRemoveI{
    static QVectorRemoveAtFunction function() { return nullptr; }
};

template<template<typename T> class Container, typename T>
struct ContainerRemoveI<Container, T, true>{
    static QVectorRemoveAtFunction function() {
        return [](JNIEnv * env, void* ptr, jint idx) {
                Q_UNUSED(env)
                Container<T> *container = static_cast<Container<T> *>(ptr);
                container->remove(idx);
            };
    }
};

template<template<typename T> class Container, typename T, bool = supports_StandardConstructor<T>::value>
struct ContainerRemoveN{
    static QVectorRemoveNFunction function() { return nullptr; }
};

template<template<typename T> class Container, typename T>
struct ContainerRemoveN<Container, T, true>{
    static QVectorRemoveNFunction function() {
        return [](JNIEnv * env, void* ptr, jint idx, jint n) {
                Q_UNUSED(env)
                Container<T> *container = static_cast<Container<T> *>(ptr);
                container->remove(idx, n);
            };
    }
};

template<template<typename T> class Container, typename T, bool = supports_equal<T>::value>
struct ContainerRemoveOne{
    static QListRemoveOneFunction function() { return nullptr; }
};

template<template<typename T> class Container, typename T>
struct ContainerRemoveOne<Container, T, true>{
    static QListRemoveOneFunction function() {
        return [](JNIEnv * env, void* ptr, jobject object) -> jboolean {
                Q_UNUSED(env)
                Container<T> *container = static_cast<Container<T> *>(ptr);
                return container->removeOne(qtjambi_cast<T>(env, object));
            };
    }
};

template<template<typename T> class Container, typename T, bool = supports_equal<T>::value>
struct ContainerRemove{
    static QSetRemoveFunction function() { return nullptr; }
};

template<template<typename T> class Container, typename T>
struct ContainerRemove<Container, T, true>{
    static QSetRemoveFunction function() {
        return [](JNIEnv * env, void* ptr, jobject object) -> jboolean {
                Q_UNUSED(env)
                Container<T> *container = static_cast<Container<T> *>(ptr);
                return container->remove(qtjambi_cast<T>(env, object));
            };
    }
};

template<template<typename T> class Container, typename T, bool = supports_StandardConstructor<T>::value>
struct ContainerReplace{
    static QListReplaceFunction function() { return nullptr; }
};

template<template<typename T> class Container, typename T>
struct ContainerReplace<Container, T, true>{
    static QListReplaceFunction function() {
        return [](JNIEnv * env, void* ptr, jint idx,jobject newObject) {
                Q_UNUSED(env)
                Container<T> *container = static_cast<Container<T> *>(ptr);
                container->replace(idx, qtjambi_cast<T>(env, newObject));
            };
    }
};

template<template<typename T> class Container, typename T, bool = supports_StandardConstructor<T>::value>
struct ContainerReserve{
    static QListReserveFunction function() { return nullptr; }
};

template<template<typename T> class Container, typename T>
struct ContainerReserve<Container, T, true>{
    static QListReserveFunction function() {
        return [](JNIEnv * env, void* ptr, jint size) {
                Q_UNUSED(env)
                Container<T> *container = static_cast<Container<T> *>(ptr);
                container->reserve(size);
            };
    }
};

template<template<typename T> class Container, typename T, bool = supports_StandardConstructor<T>::value>
struct ContainerResize{
    static QVectorResizeFunction function() {return nullptr;}
};

template<template<typename T> class Container, typename T>
struct ContainerResize<Container, T, true>{
    static QVectorResizeFunction function() {
        return [](JNIEnv * env, void* ptr, jint size) {
                Q_UNUSED(env)
                Container<T> *container = static_cast<Container<T> *>(ptr);
                container->resize(size);
            };
    }
};

template<template<typename T> class Container, typename T, bool = supports_StandardConstructor<T>::value>
struct ContainerShrinkToFit{
    static QVectorShrinkToFitFunction function() {return nullptr;}
};

template<template<typename T> class Container, typename T>
struct ContainerShrinkToFit<Container, T, true>{
    static QVectorShrinkToFitFunction function() {
        return [](JNIEnv * env, void* ptr) {
                Q_UNUSED(env)
                Container<T> *container = static_cast<Container<T> *>(ptr);
                container->shrink_to_fit();
            };
    }
};

template<template<typename T> class Container, typename T>
struct ContainerSwap{
    static QListSwapFunction function() {
        return [](JNIEnv * env, void* ptr, jint idx1, jint idx2) {
                Q_UNUSED(env)
                Container<T> *container = static_cast<Container<T> *>(ptr);
#if QT_VERSION < QT_VERSION_CHECK(5,13,0)
                container->swap(int(idx1), int(idx2));
#else
                container->swapItemsAt(int(idx1), int(idx2));
#endif
            };
    }
};

template<template<typename T> class Container, typename T, bool = supports_StandardConstructor<T>::value>
struct ContainerTakeAt{
    static QListTakeAtFunction function() { return nullptr; }
};

template<template<typename T> class Container, typename T>
struct ContainerTakeAt<Container, T, true>{
    static QListTakeAtFunction function() {
        return [](JNIEnv * env, void* ptr, jint index) -> jobject {
                Q_UNUSED(env)
                Container<T> *container = static_cast<Container<T> *>(ptr);
                return qtjambi_cast<jobject>(env, container->takeAt(index));
            };
    }
};

template<template<typename T> class Container, typename T>
struct ContainerValues{
    static QSetValuesFunction function() {
        return [](JNIEnv * env, const void* ptr) -> jobject {
                Q_UNUSED(env)
                const Container<T>  *container = static_cast<const Container<T> *>(ptr);
                return qtjambi_cast<jobject>(env, container->values());
            };
    }
};

template<bool is_pointer, bool is_const, bool is_reference, bool buffer, template<typename T> class Container, typename T>
struct create_container_pointer{
    static Container<T> create(JNIEnv *, QtJambiScope*, Container<T>*, const char*){
        Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
    }

    static Container<T> create(JNIEnv *, QtJambiScope*, const Container<T>*, const char*){
        Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
    }

    static Container<T> create(JNIEnv *, QtJambiScope*, QScopedPointer<Container<T>>&, const char*){
        Q_STATIC_ASSERT_X(false && !is_pointer, "Cannot cast types");
    }
};

template<bool is_const, template<typename T> class Container, typename T>
struct create_container_pointer<true, is_const, false, false, Container, T>{
    typedef typename std::conditional<is_const, typename std::add_const<Container<T>>::type, Container<T>>::type Container_const;
    static Container<T>* create(JNIEnv *, QtJambiScope*, Container<T>* t, const char*){
        return t;
    }
    static Container_const* create(JNIEnv *, QtJambiScope*, const Container<T>* t, const char*){
        Q_STATIC_ASSERT_X(is_const, "Cannot cast const pointer to non-const pointer without scope.");
        return t;
    }
    static Container<T>* create(JNIEnv *, QtJambiScope*, QScopedPointer<Container<T>>& t, const char*){
        return t.take();
    }
};

template<template<typename T> class Container, typename T>
struct create_container_pointer<true, false, false, true, Container, T>{
    static Container<T>* create(JNIEnv *, QtJambiScope*, Container<T>* t, const char*){
        return t;
    }
    static Container<T>* create(JNIEnv *env, QtJambiScope* scope, const Container<T>* t, const char* type){
        if(!scope)
            JavaException::raiseError(env, qPrintable( QString("Cannot cast to non-const %1* without scope.").arg(QLatin1String(type)) ) QTJAMBI_STACKTRACEINFO );
        if(t){
            Container<T>* result = new Container<T>(*t);
            scope->addFinalAction([result](){ delete result; });
            return result;
        }else{
            return nullptr;
        }
    }
    static Container<T>* create(JNIEnv *, QtJambiScope*, QScopedPointer<Container<T>>& t, const char*){
        return t.take();
    }
};

template<bool is_const, bool buffer, template<typename T> class Container, typename T>
struct create_container_pointer<false, is_const, false, buffer, Container, T>{
    static Container<T> create(JNIEnv *, QtJambiScope*, const Container<T>* t, const char*){
        return t ? *t : Container<T>();
    }
    static Container<T> create(JNIEnv *, QtJambiScope*, QScopedPointer<Container<T>>& t, const char*){
        return t ? *t : Container<T>();
    }
};

template<bool buffer, template<typename T> class Container, typename T>
struct create_container_pointer<false, true, true, buffer, Container, T>{
    static const Container<T>& create(JNIEnv *, QtJambiScope*, const Container<T>* t, const char*){
        return t ? *t : qtjambi_get_default_value<Container<T>>();
    }
    static const Container<T>& create(JNIEnv *env, QtJambiScope* scope, QScopedPointer<Container<T>>& t, const char* type){
        Q_STATIC_ASSERT_X(buffer, "Cannot cast to Container reference without scope.");
        if(t && !scope)
            JavaException::raiseError(env, qPrintable( QString("Cannot cast to %1& without scope.").arg(QLatin1String(type)) ) QTJAMBI_STACKTRACEINFO );
        Container<T>* ptr = t.take();
        scope->addFinalAction([ptr](){ delete ptr; });
        return ptr ? *ptr : qtjambi_get_default_value<Container<T>>();
    }
};

template<template<typename T> class Container, typename T>
struct create_container_pointer<false, false, true, true, Container, T>{
    static Container<T>& create(JNIEnv *env, QtJambiScope* scope, Container<T>* t, const char* type){
        if(!scope)
            JavaException::raiseError(env, qPrintable( QString("Cannot cast to %1& without scope.").arg(QLatin1String(type)) ) QTJAMBI_STACKTRACEINFO );
        if(t){
            scope->addFinalAction([t](){ delete t; });
            return *t;
        }else{
            Container<T>* result = new Container<T>();
            scope->addFinalAction([result](){ delete result; });
            return *result;
        }
    }

    static const Container<T>& create(JNIEnv *env, QtJambiScope* scope, const Container<T>* t, const char* type){
        if(!scope)
            JavaException::raiseError(env, qPrintable( QString("Cannot cast to %1& without scope.").arg(QLatin1String(type)) ) QTJAMBI_STACKTRACEINFO );
        if(t){
            scope->addFinalAction([t](){ delete t; });
            return *t;
        }else{
            Container<T>* result = new Container<T>();
            scope->addFinalAction([result](){ delete result; });
            return *result;
        }
    }

    static Container<T>& create(JNIEnv *env, QtJambiScope* scope, QScopedPointer<Container<T>>& ptr, const char* type){
        if(!scope)
            JavaException::raiseError(env, qPrintable( QString("Cannot cast to %1& without scope.").arg(QLatin1String(type)) ) QTJAMBI_STACKTRACEINFO );
        Container<T>* t = ptr.take();
        if(t){
            scope->addFinalAction([t](){ delete t; });
            return *t;
        }else{
            Container<T>* result = new Container<T>();
            scope->addFinalAction([result](){ delete result; });
            return *result;
        }
    }
};

template<template<typename T> class Container, typename T>
struct create_container_pointer<false, false, true, false, Container, T>{
    static Container<T>& create(JNIEnv *env, QtJambiScope*, Container<T>* t, const char* type){
        if(!t){
            JavaException::raiseNullPointerException(env, qPrintable( QString("%1 is null.").arg(QLatin1String(type)) ) QTJAMBI_STACKTRACEINFO );
        }
        return *t;
    }
    static Container<T>& create(JNIEnv *, QtJambiScope*, QScopedPointer<Container<T>>& t, const char*){
        Q_STATIC_ASSERT_X(false && std::is_pointer<Container<T>>::value, "Cannot cast to Container reference without scope.");
        return *t;
    }
};

template<bool c_is_pointer, typename T>
struct qtjambi_QList_caster<c_is_pointer, true, T>{
    typedef const QList<T> Container;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;

    static jobject castfwd(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
        jobject result = qtjambi_to_constQList(env,
                                     scope ? scope->relatedNativeID() : InvalidNativeID,
                                     ref_ptr<c_is_pointer, Container>::ref(in),
                                     qtjambi_type<T>::id(),
                                     ContainerAt<QList, T>::function(),
                                     ContainerBegin<QList, T>::function(),
                                     ContainerContains<QList, T>::function(),
                                     ContainerCountObject<QList, T>::function(),
                                     ContainerEnd<QList, T>::function(),
                                     ContainerEndsWith<QList, T>::function(),
                                     ContainerIndexOf<QList, T>::function(),
                                     ContainerLastIndexOf<QList, T>::function(),
                                     ContainerMid<QList, T>::function(),
                                     ContainerEquals<QList, T, qtjambi_is_QList>::function(),
                                     ContainerSize<QList, T>::function(),
                                     ContainerStartsWith<QList, T>::function(),
                                     ContainerToSet<QList, T>::function(),
                                     ContainerValue<QList, T, supports_StandardConstructor<T>::value>::function(),
                                     ContainerValueDefault<QList, T>::function()
                                 );
        if(scope && !scope->relatedNativeID()){
            scope->addFinalAction([env, result](){ qtjambi_invalidate_object(env, result); });
        }
        return result;
    }
};

template<bool c_is_pointer, typename T>
struct qtjambi_QList_caster<c_is_pointer, false, T>{
    typedef QList<T> Container;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;
    typedef typename std::remove_reference<T>::type T_noref;
    typedef typename std::remove_cv<T_noref>::type T_noconst;

    static jobject castfwd(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
        jobject result = qtjambi_to_QList(env,
                                scope ? scope->relatedNativeID() : InvalidNativeID,
                                ref_ptr<c_is_pointer, Container>::ref(in),
                                qtjambi_type<T>::id(),
                                ContainerAppend<QList, T>::function(),
                                ContainerAppendList<QList, T, qtjambi_is_QList>::function(),
                                ContainerAt<QList, T>::function(),
                                ContainerBegin<QList, T>::function(),
                                ContainerClear<QList, T>::function(),
                                ContainerContains<QList, T>::function(),
                                ContainerCountObject<QList, T>::function(),
                                ContainerEnd<QList, T>::function(),
                                ContainerEndsWith<QList, T>::function(),
                                ContainerIndexOf<QList, T>::function(),
                                ContainerInsertAt<QList, T>::function(),
                                ContainerLastIndexOf<QList, T>::function(),
                                ContainerMid<QList, T>::function(),
                                ContainerMove<QList, T>::function(),
                                ContainerEquals<QList, T, qtjambi_is_QList>::function(),
                                ContainerPrepend<QList, T>::function(),
                                ContainerRemoveAll<QList, T>::function(),
                                ContainerRemoveAt<QList, T>::function(),
                                ContainerRemoveOne<QList, T>::function(),
                                ContainerReplace<QList, T>::function(),
                                ContainerReserve<QList, T>::function(),
                                ContainerSize<QList, T>::function(),
                                ContainerStartsWith<QList, T>::function(),
                                ContainerSwap<QList, T>::function(),
                                ContainerTakeAt<QList, T>::function(),
                                ContainerToSet<QList, T>::function(),
                                ContainerValue<QList, T, supports_StandardConstructor<T>::value>::function(),
                                ContainerValueDefault<QList, T>::function()
                );
        if(scope && !scope->relatedNativeID()){
            scope->addFinalAction([env, result](){ qtjambi_invalidate_object(env, result); });
        }
        return result;
    }
};

template<bool c_is_pointer, typename T>
struct qtjambi_QSet_caster<c_is_pointer, true, T>{
    typedef const QSet<T> Container;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;

    static jobject castfwd(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(supports_qHash<T>::value, "Unsupported type T for QSet: missing qHash().");
        Q_STATIC_ASSERT_X(supports_equal<T>::value, "Unsupported type T for QSet: missing operator ==.");
        jobject result = qtjambi_to_constQSet(env,
                                     scope ? scope->relatedNativeID() : InvalidNativeID,
                                     ref_ptr<c_is_pointer, Container>::ref(in),
                                     qtjambi_type<T>::id(),
                                     ContainerBegin<QSet, T>::function(),
                                     ContainerCapacity<QSet, T>::function(),
                                     ContainerContains<QSet, T, true>::function(),
                                     ContainerEnd<QSet, T>::function(),
                                     ContainerIntersects<QSet, T, qtjambi_is_QSet>::function(),
                                     ContainerEquals<QSet, T, qtjambi_is_QSet>::function(),
                                     ContainerSize<QSet, T>::function(),
                                     ContainerValues<QSet, T>::function()
                                 );
        if(scope && !scope->relatedNativeID()){
            scope->addFinalAction([env, result](){ qtjambi_invalidate_object(env, result); });
        }
        return result;
    }
};

template<bool c_is_pointer, typename T>
struct qtjambi_QSet_caster<c_is_pointer, false, T>{
    typedef QSet<T> Container;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;
    typedef typename std::remove_reference<T>::type T_noref;
    typedef typename std::remove_cv<T_noref>::type T_noconst;

    static jobject castfwd(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
        Q_STATIC_ASSERT_X(supports_qHash<T>::value, "Unsupported type T for QSet: missing qHash().");
        Q_STATIC_ASSERT_X(supports_equal<T>::value, "Unsupported type T for QSet: missing operator ==.");
        jobject result = qtjambi_to_QSet(env,
                                scope ? scope->relatedNativeID() : InvalidNativeID,
                                ref_ptr<c_is_pointer, Container>::ref(in),
                                qtjambi_type<T>::id(),
                                QSetBeginFunction(ContainerBegin<QSet, T>::function()),
                                QSetCapacityFunction(ContainerCapacity<QSet, T>::function()),
                                QSetClearFunction(ContainerClear<QSet, T>::function()),
                                QSetContainsFunction(ContainerContains<QSet, T, true>::function()),
                                QSetEndFunction(ContainerEnd<QSet, T>::function()),
                                QSetInsertFunction(ContainerInsert<QSet, T>::function()),
                                QSetIntersectFunction(ContainerIntersect<QSet, T, qtjambi_is_QSet>::function()),
                                QSetIntersectsFunction(ContainerIntersects<QSet, T, qtjambi_is_QSet>::function()),
                                QSetEqualFunction(ContainerEquals<QSet, T, qtjambi_is_QSet>::function()),
                                QSetRemoveFunction(ContainerRemove<QSet, T>::function()),
                                QSetReserveFunction(ContainerReserve<QSet, T>::function()),
                                QSetSizeFunction(ContainerSize<QSet, T>::function()),
                                QSetSubtractFunction(ContainerSubtract<QSet, T, qtjambi_is_QSet>::function()),
                                QSetUniteFunction(ContainerUnite<QSet, T, qtjambi_is_QSet>::function()),
                                QSetValuesFunction(ContainerValues<QSet, T>::function())

                );
        if(scope && !scope->relatedNativeID()){
            scope->addFinalAction([env, result](){ qtjambi_invalidate_object(env, result); });
        }
        return result;
    }
};

template<bool c_is_pointer, typename T>
struct qtjambi_QLinkedList_caster<c_is_pointer, true, T>{
    typedef const QLinkedList<T> Container;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;

    static jobject castfwd(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
        jobject result = qtjambi_to_constQLinkedList(env,
                                     scope ? scope->relatedNativeID() : InvalidNativeID,
                                     ref_ptr<c_is_pointer, Container>::ref(in),
                                     qtjambi_type<T>::id(),
                                     ContainerBegin<QLinkedList, T>::function(),
                                     ContainerContains<QLinkedList, T>::function(),
                                     ContainerCountObject<QLinkedList, T>::function(),
                                     ContainerEnd<QLinkedList, T>::function(),
                                     ContainerEndsWith<QLinkedList, T>::function(),
                                     ContainerFirst<QLinkedList, T>::function(),
                                     ContainerLast<QLinkedList, T>::function(),
                                     ContainerEquals<QLinkedList, T, qtjambi_is_QLinkedList>::function(),
                                     ContainerSize<QLinkedList, T>::function(),
                                     ContainerStartsWith<QLinkedList, T>::function()
                                 );
        if(scope && !scope->relatedNativeID()){
            scope->addFinalAction([env, result](){ qtjambi_invalidate_object(env, result); });
        }
        return result;
    }
};

template<bool c_is_pointer, typename T>
struct qtjambi_QLinkedList_caster<c_is_pointer, false, T>{
    typedef QLinkedList<T> Container;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;
    typedef typename std::remove_reference<T>::type T_noref;
    typedef typename std::remove_cv<T_noref>::type T_noconst;

    static jobject castfwd(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
        jobject result = qtjambi_to_QLinkedList(env,
                                scope ? scope->relatedNativeID() : InvalidNativeID,
                                ref_ptr<c_is_pointer, Container>::ref(in),
                                qtjambi_type<T>::id(),
                                QLinkedListAppendFunction(ContainerAppend<QLinkedList, T>::function()),
                                QLinkedListBeginFunction(ContainerBegin<QLinkedList, T>::function()),
                                QLinkedListClearFunction(ContainerClear<QLinkedList, T>::function()),
                                QLinkedListContainsFunction(ContainerContains<QLinkedList, T>::function()),
                                QLinkedListCountObjectFunction(ContainerCountObject<QLinkedList, T>::function()),
                                QLinkedListEndFunction(ContainerEnd<QLinkedList, T>::function()),
                                QLinkedListEndsWithFunction(ContainerEndsWith<QLinkedList, T>::function()),
                                QLinkedListFirstFunction(ContainerFirst<QLinkedList, T>::function()),
                                QLinkedListLastFunction(ContainerLast<QLinkedList, T>::function()),
                                QLinkedListEqualFunction(ContainerEquals<QLinkedList, T, qtjambi_is_QLinkedList>::function()),
                                QLinkedListPrependFunction(ContainerPrepend<QLinkedList, T>::function()),
                                QLinkedListRemoveAllFunction(ContainerRemoveAll<QLinkedList, T>::function()),
                                QLinkedListRemoveFirstFunction(ContainerRemoveFirst<QLinkedList, T>::function()),
                                QLinkedListRemoveLastFunction(ContainerRemoveLast<QLinkedList, T>::function()),
                                QLinkedListRemoveOneFunction(ContainerRemoveOne<QLinkedList, T>::function()),
                                QLinkedListSizeFunction(ContainerSize<QLinkedList, T>::function()),
                                QLinkedListStartsWithFunction(ContainerStartsWith<QLinkedList, T>::function()),
                                QLinkedListTakeFirstFunction(ContainerTakeFirst<QLinkedList, T>::function()),
                                QLinkedListTakeLastFunction(ContainerTakeLast<QLinkedList, T>::function())
                );
        if(scope && !scope->relatedNativeID()){
            scope->addFinalAction([env, result](){ qtjambi_invalidate_object(env, result); });
        }
        return result;
    }
};

template<bool c_is_pointer, typename T>
struct qtjambi_QQueue_caster<c_is_pointer, true, T>{
    typedef const QQueue<T> Container;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;

    static jobject castfwd(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
        jobject result = qtjambi_to_constQQueue(env,
                                     scope ? scope->relatedNativeID() : InvalidNativeID,
                                     ref_ptr<c_is_pointer, Container>::ref(in),
                                     qtjambi_type<T>::id(),
                                     ContainerAt<QQueue, T>::function(),
                                     ContainerBegin<QQueue, T>::function(),
                                     ContainerContains<QQueue, T>::function(),
                                     ContainerCountObject<QQueue, T>::function(),
                                     ContainerEnd<QQueue, T>::function(),
                                     ContainerEndsWith<QQueue, T>::function(),
                                     ContainerIndexOf<QQueue, T>::function(),
                                     ContainerLastIndexOf<QQueue, T>::function(),
                                     ContainerMid<QQueue, T>::function(),
                                     ContainerEquals<QQueue, T, qtjambi_is_QQueue>::function(),
                                     ContainerSize<QQueue, T>::function(),
                                     ContainerStartsWith<QQueue, T>::function(),
                                     ContainerToSet<QQueue, T>::function(),
                                     ContainerValue<QQueue, T, supports_StandardConstructor<T>::value>::function(),
                                     ContainerValueDefault<QQueue, T>::function()
                                 );
        if(scope && !scope->relatedNativeID()){
            scope->addFinalAction([env, result](){ qtjambi_invalidate_object(env, result); });
        }
        return result;
    }
};

template<bool c_is_pointer, typename T>
struct qtjambi_QQueue_caster<c_is_pointer, false, T>{
    typedef QQueue<T> Container;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;
    typedef typename std::remove_reference<T>::type T_noref;
    typedef typename std::remove_cv<T_noref>::type T_noconst;

    static jobject castfwd(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
        jobject result = qtjambi_to_QQueue(env,
                                scope ? scope->relatedNativeID() : InvalidNativeID,
                                ref_ptr<c_is_pointer, Container>::ref(in),
                                qtjambi_type<T>::id(),
                                ContainerAppend<QQueue, T>::function(),
                                ContainerAppendList<QQueue, T, qtjambi_is_QQueue>::function(),
                                ContainerAt<QQueue, T>::function(),
                                ContainerBegin<QQueue, T>::function(),
                                ContainerClear<QQueue, T>::function(),
                                ContainerContains<QQueue, T>::function(),
                                ContainerCountObject<QQueue, T>::function(),
                                ContainerEnd<QQueue, T>::function(),
                                ContainerEndsWith<QQueue, T>::function(),
                                ContainerIndexOf<QQueue, T>::function(),
                                ContainerInsertAt<QQueue, T>::function(),
                                ContainerLastIndexOf<QQueue, T>::function(),
                                ContainerMid<QQueue, T>::function(),
                                ContainerMove<QQueue, T>::function(),
                                ContainerEquals<QQueue, T, qtjambi_is_QQueue>::function(),
                                ContainerPrepend<QQueue, T>::function(),
                                ContainerRemoveAll<QQueue, T>::function(),
                                ContainerRemoveAt<QQueue, T>::function(),
                                ContainerRemoveOne<QQueue, T>::function(),
                                ContainerReplace<QQueue, T>::function(),
                                ContainerReserve<QQueue, T>::function(),
                                ContainerSize<QQueue, T>::function(),
                                ContainerStartsWith<QQueue, T>::function(),
                                ContainerSwap<QQueue, T>::function(),
                                ContainerTakeAt<QQueue, T>::function(),
                                ContainerToSet<QQueue, T>::function(),
                                ContainerValue<QQueue, T, supports_StandardConstructor<T>::value>::function(),
                                ContainerValueDefault<QQueue, T>::function()
                );
        if(scope && !scope->relatedNativeID()){
            scope->addFinalAction([env, result](){ qtjambi_invalidate_object(env, result); });
        }
        return result;
    }
};

template<bool c_is_pointer, typename T>
struct qtjambi_QVector_caster<c_is_pointer, true, T>{
    typedef const QVector<T> Container;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;

    static jobject castfwd(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
        jobject result = qtjambi_to_constQVector(env,
                                     scope ? scope->relatedNativeID() : InvalidNativeID,
                                     ref_ptr<c_is_pointer, Container>::ref(in),
                                     qtjambi_type<T>::id(),
                                     ContainerAt<QVector, T>::function(),
                                     ContainerBegin<QVector, T>::function(),
                                     ContainerCapacity<QVector, T>::function(),
                                     ContainerContains<QVector, T>::function(),
                                     ContainerCountObject<QVector, T>::function(),
                                     ContainerEnd<QVector, T>::function(),
                                     ContainerEndsWith<QVector, T>::function(),
                                     ContainerIndexOf<QVector, T>::function(),
                                     ContainerLastIndexOf<QVector, T>::function(),
                                     ContainerMid<QVector, T>::function(),
                                     ContainerEquals<QVector, T, qtjambi_is_QVector>::function(),
                                     ContainerSize<QVector, T>::function(),
                                     ContainerStartsWith<QVector, T>::function(),
                                     ContainerToList<QVector, T>::function(),
                                     ContainerValue<QVector, T, supports_StandardConstructor<T>::value>::function(),
                                     ContainerValueDefault<QVector, T>::function()
                                 );
        if(scope && !scope->relatedNativeID()){
            scope->addFinalAction([env, result](){ qtjambi_invalidate_object(env, result); });
        }
        return result;
    }
};

template<bool c_is_pointer, typename T>
struct qtjambi_QVector_caster<c_is_pointer, false, T>{
    typedef QVector<T> Container;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;
    typedef typename std::remove_reference<T>::type T_noref;
    typedef typename std::remove_cv<T_noref>::type T_noconst;

    static jobject castfwd(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
        jobject result = qtjambi_to_QVector(env,
                                scope ? scope->relatedNativeID() : InvalidNativeID,
                                ref_ptr<c_is_pointer, Container>::ref(in),
                                qtjambi_type<T>::id(),
                                ContainerAppend<QVector, T, supports_StandardConstructor<T>::value>::function(),
                                ContainerAppendVector<QVector, T, qtjambi_is_QVector>::function(),
                                ContainerAt<QVector, T>::function(),
                                ContainerBegin<QVector, T>::function(),
                                ContainerCapacity<QVector, T>::function(),
                                ContainerClear<QVector, T, supports_StandardConstructor<T>::value>::function(),
                                ContainerContains<QVector, T>::function(),
                                ContainerCountObject<QVector, T>::function(),
                                ContainerEnd<QVector, T>::function(),
                                ContainerEndsWith<QVector, T>::function(),
                                ContainerFill<QVector, T>::function(),
                                ContainerIndexOf<QVector, T>::function(),
                                ContainerInsertAt<QVector, T, supports_StandardConstructor<T>::value>::function(),
                                ContainerInsertN<QVector, T>::function(),
                                ContainerLastIndexOf<QVector, T>::function(),
                                ContainerMid<QVector, T>::function(),
                                ContainerMove<QVector, T, supports_StandardConstructor<T>::value>::function(),
                                ContainerEquals<QVector, T, qtjambi_is_QVector>::function(),
                                ContainerPrepend<QVector, T, supports_StandardConstructor<T>::value>::function(),
                                ContainerRemoveAll<QVector, T>::function(),
                                ContainerRemoveI<QVector, T>::function(),
                                ContainerRemoveN<QVector, T>::function(),
                                ContainerRemoveOne<QVector, T>::function(),
                                ContainerReplace<QVector, T>::function(),
                                ContainerReserve<QVector, T>::function(),
                                ContainerResize<QVector, T>::function(),
                                ContainerShrinkToFit<QVector, T>::function(),
                                ContainerSize<QVector, T>::function(),
                                ContainerStartsWith<QVector, T>::function(),
                                ContainerTakeAt<QVector, T>::function(),
                                ContainerToList<QVector, T>::function(),
                                ContainerValue<QVector, T, supports_StandardConstructor<T>::value>::function(),
                                ContainerValueDefault<QVector, T>::function()
                );
        if(scope && !scope->relatedNativeID()){
            scope->addFinalAction([env, result](){ qtjambi_invalidate_object(env, result); });
        }
        return result;
    }
};

template<bool c_is_pointer, typename T>
struct qtjambi_QStack_caster<c_is_pointer, true, T>{
    typedef const QStack<T> Container;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;

    static jobject castfwd(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
        jobject result = qtjambi_to_constQStack(env,
                                     scope ? scope->relatedNativeID() : InvalidNativeID,
                                     ref_ptr<c_is_pointer, Container>::ref(in),
                                     qtjambi_type<T>::id(),
                                     ContainerAt<QStack, T>::function(),
                                     ContainerBegin<QStack, T>::function(),
                                     ContainerCapacity<QStack, T>::function(),
                                     ContainerContains<QStack, T>::function(),
                                     ContainerCountObject<QStack, T>::function(),
                                     ContainerEnd<QStack, T>::function(),
                                     ContainerEndsWith<QStack, T>::function(),
                                     ContainerIndexOf<QStack, T>::function(),
                                     ContainerLastIndexOf<QStack, T>::function(),
                                     ContainerMid<QStack, T>::function(),
                                     ContainerEquals<QStack, T, qtjambi_is_QStack>::function(),
                                     ContainerSize<QStack, T>::function(),
                                     ContainerStartsWith<QStack, T>::function(),
                                     ContainerToList<QStack, T>::function(),
                                     ContainerValue<QStack, T, supports_StandardConstructor<T>::value>::function(),
                                     ContainerValueDefault<QStack, T>::function()
                                 );
        if(scope && !scope->relatedNativeID()){
            scope->addFinalAction([env, result](){ qtjambi_invalidate_object(env, result); });
        }
        return result;
    }
};

template<bool c_is_pointer, typename T>
struct qtjambi_QStack_caster<c_is_pointer, false, T>{
    typedef QStack<T> Container;
    typedef typename std::conditional<c_is_pointer, typename std::add_pointer<Container>::type, typename std::add_lvalue_reference<Container>::type>::type Container_in;
    typedef typename std::remove_reference<T>::type T_noref;
    typedef typename std::remove_cv<T_noref>::type T_noconst;

    static jobject castfwd(JNIEnv *env, Container_in in, const char*, QtJambiScope* scope){
        jobject result = qtjambi_to_QStack(env,
                                scope ? scope->relatedNativeID() : InvalidNativeID,
                                ref_ptr<c_is_pointer, Container>::ref(in),
                                qtjambi_type<T>::id(),
                                ContainerAppend<QStack, T, supports_StandardConstructor<T>::value>::function(),
                                ContainerAppendVector<QStack, T, qtjambi_is_QStack>::function(),
                                ContainerAt<QStack, T>::function(),
                                ContainerBegin<QStack, T>::function(),
                                ContainerCapacity<QStack, T>::function(),
                                ContainerClear<QStack, T, supports_StandardConstructor<T>::value>::function(),
                                ContainerContains<QStack, T>::function(),
                                ContainerCountObject<QStack, T>::function(),
                                ContainerEnd<QStack, T>::function(),
                                ContainerEndsWith<QStack, T>::function(),
                                ContainerFill<QStack, T>::function(),
                                ContainerIndexOf<QStack, T>::function(),
                                ContainerInsertAt<QStack, T, supports_StandardConstructor<T>::value>::function(),
                                ContainerInsertN<QStack, T>::function(),
                                ContainerLastIndexOf<QStack, T>::function(),
                                ContainerMid<QStack, T>::function(),
                                ContainerMove<QStack, T, supports_StandardConstructor<T>::value>::function(),
                                ContainerEquals<QStack, T, qtjambi_is_QStack>::function(),
                                ContainerPrepend<QStack, T, supports_StandardConstructor<T>::value>::function(),
                                ContainerRemoveAll<QStack, T>::function(),
                                ContainerRemoveI<QStack, T>::function(),
                                ContainerRemoveN<QStack, T>::function(),
                                ContainerRemoveOne<QStack, T>::function(),
                                ContainerReplace<QStack, T>::function(),
                                ContainerReserve<QStack, T>::function(),
                                ContainerResize<QStack, T>::function(),
                                ContainerShrinkToFit<QStack, T>::function(),
                                ContainerSize<QStack, T>::function(),
                                ContainerStartsWith<QStack, T>::function(),
                                ContainerTakeAt<QStack, T>::function(),
                                ContainerToList<QStack, T>::function(),
                                ContainerValue<QStack, T, supports_StandardConstructor<T>::value>::function(),
                                ContainerValueDefault<QStack, T>::function()
                );
        if(scope && !scope->relatedNativeID()){
            scope->addFinalAction([env, result](){ qtjambi_invalidate_object(env, result); });
        }
        return result;
    }
};

template<bool has_scope, typename Container, bool is_const, typename T>
struct qtjambi_collection_fill{

};

template<bool has_scope, typename Container, typename T>
struct qtjambi_collection_fill<has_scope, Container, true, T>{
    static void fill(JNIEnv *env, const Container& container, jobject collection, QtJambiScope* scope){
        for(typename Container::const_iterator i = container.begin(); i!=container.end(); ++i){
            qtjambi_collection_add(env, collection, qtjambi_scoped_cast<has_scope,jobject,decltype(*i)>::cast(env, *i, nullptr, scope));
        }
    }
};

template<bool has_scope, typename Container, typename T>
struct qtjambi_collection_fill<has_scope, Container, false, T>{
    static void fill(JNIEnv *env, Container& container, jobject collection, QtJambiScope* scope){
        for(typename Container::iterator i = container.begin(); i!=container.end(); ++i){
            qtjambi_collection_add(env, collection, qtjambi_scoped_cast<has_scope,jobject,decltype(*i)>::cast(env, *i, nullptr, scope));
        }
    }
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_LIST_P_H
