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

#ifndef CONTAINERACCESS_SEQUENTIAL_H
#define CONTAINERACCESS_SEQUENTIAL_H

#include "utils.h"
#include "qtjambi_cast_iterator.h"
#include "typetests.h"
#include "containerapi.h"

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
#include <QtCore/QSpan>
#endif

enum class QtJambiNativeID : jlong;

namespace QtJambiPrivate {

QTJAMBI_EXPORT jobject findFunctionPointerObject(JNIEnv *env, const void * pointer, const std::type_info& typeId);

typedef bool (*IsContainerFunction)(JNIEnv *, jobject, const std::type_info&, const QMetaType&, void*& pointer);
typedef bool (*IsContainerAccessFunction)(JNIEnv *, jobject, const QMetaType&, void*& pointer, AbstractContainerAccess*& access);

template<template<typename T> class Container, typename T, bool isPointer, bool = ((is_copy_constructible_v<T> && is_default_constructible_v<T>)
                                                                                   || std::is_trivially_copyable_v<T>
                                                                                   || is_shared_data<Container<T>>::value) && is_copy_constructible_v<Container<T>>>
struct CloneContainer{
    static constexpr CopyFunction function = nullptr;
};

template<template<typename T> class Container, typename T>
struct CloneContainer<Container,T,false,true>{
    static void* clone(const void* ptr) { return new Container<T>(*reinterpret_cast<const Container<T>*>(ptr)); }
    static constexpr CopyFunction function = &clone;
};

template<template<typename T> class Container, typename T>
struct DeleteContainer{
    static void del(void* ptr,bool) { delete reinterpret_cast<Container<T>*>(ptr); }
    static constexpr PtrDeleterFunction function = &del;
};

typedef AbstractContainerAccess*(*NewContainerAccessFunction)();

template<typename T>
struct ContainerContentType{
    static constexpr AbstractContainerAccess::DataType type = AbstractContainerAccess::Value;
    static constexpr bool isContainer = false;
    static constexpr bool needsReferenceCounting = false;
    static constexpr bool needsOwnerCheck = true;
    static constexpr AbstractContainerAccess* accessFactory(){ return nullptr; }
};

template<typename T, bool isPointer = (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0>
struct ContainerContentDeref{
    static auto deref(const T& value){
        return &value;
    }
};

template<typename T>
struct ContainerContentDeref<T, true>{
    static auto deref(T value){
        return value;
    }
};

template<typename R, typename...Args>
struct ContainerContentDeref<R(*)(Args...), true>{
    static const void* deref(R(*value)(Args...)){
        union{
            const void* result;
            R(*value)(Args...);
        }u;
        u.value = value;
        return u.result;
    }
};

template<typename R, typename... Args>
struct ContainerContentType<R(*)(Args...)>{
    static constexpr AbstractContainerAccess::DataType type = AbstractContainerAccess::FunctionPointer;
    static constexpr bool isContainer = false;
    static constexpr bool needsReferenceCounting = true;
    static constexpr bool needsOwnerCheck = false;
    static constexpr AbstractContainerAccess* accessFactory(){ return nullptr; }
};

template<typename T>
struct ContainerContentType<T*>{
    static constexpr AbstractContainerAccess::DataType type = std::is_base_of<QObject, T>::value ? AbstractContainerAccess::PointerToQObject : AbstractContainerAccess::Pointer;
    static constexpr bool isContainer = false;
    static constexpr bool needsReferenceCounting = true;
    static constexpr bool needsOwnerCheck = false;
    static constexpr AbstractContainerAccess* accessFactory(){ return nullptr; }
};

template<template<typename T> class Container, typename T, bool = supports_equal<T>::value>
struct ContainerContains{
    static jboolean function(JNIEnv * env, const void* ptr, jobject object) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return container->contains(::qtjambi_cast<T>(env, object));
    }
};

template<template<typename T> class Container, typename T>
struct ContainerContains<Container, T, false>{
    static jboolean function(JNIEnv * env, const void*, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "contains(value)" QTJAMBI_STACKTRACEINFO );
        return false;
    }
};

template<template<typename T> class Container, typename T, IsContainerFunction isContainer>
struct ContainerIntersects{
    static jboolean function(JNIEnv * env, const void* ptr, jobject other) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        std::unique_ptr<Container<T> > __qt_scoped_pointer;
        Container<T> *__qt_other_pointer = nullptr;
        if (other!= nullptr) {
            if (!isContainer(env, other, qtjambi_type<T>::id(), QMetaType::fromType<std::remove_cv_t<T>>(), reinterpret_cast<void*&>(__qt_other_pointer))) {
                __qt_scoped_pointer.reset(new Container<T> ());
                __qt_other_pointer = __qt_scoped_pointer.get();
                jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, other);
                while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                    jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                    (*__qt_other_pointer) << ::qtjambi_cast<T>(env, element);
                }
            }
        }else{
            __qt_scoped_pointer.reset(new Container<T> ());
            __qt_other_pointer = __qt_scoped_pointer.get();
        }
        return container->intersects(*__qt_other_pointer);
    }
};

template<template<typename T> class Container, typename T, IsContainerAccessFunction isContainer>
struct ContainerIntersect{
    static void function(JNIEnv * env, const ContainerInfo& ptr, ContainerAndAccessInfo& other) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        std::unique_ptr<Container<T> > __qt_scoped_pointer;
        Container<T> *__qt_other_pointer = nullptr;
        if (other.object != nullptr) {
            if (isContainer(env, other.object, QMetaType::fromType<std::remove_cv_t<T>>(), reinterpret_cast<void*&>(__qt_other_pointer), other.access)) {
                other.container = __qt_other_pointer;
            }else{
                __qt_scoped_pointer.reset(new Container<T> ());
                __qt_other_pointer = __qt_scoped_pointer.get();
                jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, other.object);
                while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                    jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                    (*__qt_other_pointer) << ::qtjambi_cast<T>(env, element);
                }
            }
        }else{
            __qt_scoped_pointer.reset(new Container<T> ());
            __qt_other_pointer = __qt_scoped_pointer.get();
        }
        container->intersect(*__qt_other_pointer);
    }
};

template<template<typename T> class Container, typename T, IsContainerAccessFunction isContainer>
struct ContainerUnite{
    static void function(JNIEnv * env, const ContainerInfo& ptr, ContainerAndAccessInfo& other) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        std::unique_ptr<Container<T> > __qt_scoped_pointer;
        Container<T> *__qt_other_pointer = nullptr;
        if (other.object != nullptr) {
            if (isContainer(env, other.object, QMetaType::fromType<std::remove_cv_t<T>>(), reinterpret_cast<void*&>(__qt_other_pointer), other.access)) {
                other.container = __qt_other_pointer;
            }else{
                __qt_scoped_pointer.reset(new Container<T> ());
                __qt_other_pointer = __qt_scoped_pointer.get();
                jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, other.object);
                while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                    jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                    (*__qt_other_pointer) << ::qtjambi_cast<T>(env, element);
                }
            }
        }else{
            __qt_scoped_pointer.reset(new Container<T> ());
            __qt_other_pointer = __qt_scoped_pointer.get();
        }
        container->unite(*__qt_other_pointer);
    }
};

template<template<typename T> class Container, typename T, IsContainerAccessFunction isContainer>
struct ContainerSubtract{
    static void function(JNIEnv * env, const ContainerInfo& ptr, ContainerAndAccessInfo& other) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        std::unique_ptr<Container<T> > __qt_scoped_pointer;
        Container<T> *__qt_other_pointer = nullptr;
        if (other.object != nullptr) {
            if (isContainer(env, other.object, QMetaType::fromType<std::remove_cv_t<T>>(), reinterpret_cast<void*&>(__qt_other_pointer), other.access)) {
                other.container = __qt_other_pointer;
            }else{
                __qt_scoped_pointer.reset(new Container<T> ());
                __qt_other_pointer = __qt_scoped_pointer.get();
                jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, other.object);
                while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                    jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                    (*__qt_other_pointer) << ::qtjambi_cast<T>(env, element);
                }
            }
        }else{
            __qt_scoped_pointer.reset(new Container<T> ());
            __qt_other_pointer = __qt_scoped_pointer.get();
        }
        container->subtract(*__qt_other_pointer);
    }
};

template<template<typename T> class Container, typename T, bool = supports_equal<T>::value>
struct ContainerCountObject{
    static qsizetype function(JNIEnv * env, const void*, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "count(value)" QTJAMBI_STACKTRACEINFO );
        return 0;
    }
};

template<template<typename T> class Container, typename T>
struct ContainerCountObject<Container, T, true>{
    static qsizetype function(JNIEnv * env, const void* ptr, jobject object) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return container->count(::qtjambi_cast<T>(env, object));
    }
};

template<template<typename T> class Container, typename T>
struct ContainerBegin{
    static jobject function(JNIEnv *env, const ExtendedContainerInfo& ptr) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        return ::qtjambi_cast<jobject>(env, ptr.nativeId, container->begin());
    }
};

template<template<typename T> class Container, typename T>
struct ContainerEnd{
    static jobject function(JNIEnv *env, const ExtendedContainerInfo& ptr) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        return ::qtjambi_cast<jobject>(env, ptr.nativeId, container->end());
    }
};

template<template<typename T> class Container, typename T>
struct ContainerConstBegin{
    static jobject function(JNIEnv *env, const ConstExtendedContainerInfo& ptr) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr.container);
        return ::qtjambi_cast<jobject>(env, ptr.nativeId, container->constBegin());
    }
};

template<template<typename T> class Container, typename T>
struct ContainerConstEnd{
    static jobject function(JNIEnv *env, const ConstExtendedContainerInfo& ptr) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr.container);
        return ::qtjambi_cast<jobject>(env, ptr.nativeId, container->constEnd());
    }
};

template<template<typename T> class Container, typename T, bool = supports_equal<T>::value>
struct ContainerEndsWith{
    static jboolean function(JNIEnv * env, const void*, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "endsWith(value)" QTJAMBI_STACKTRACEINFO );
        return false;
    }
};

template<template<typename T> class Container, typename T>
struct ContainerEndsWith<Container, T, true>{
    static jboolean function(JNIEnv * env, const void* ptr, jobject object) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return container->endsWith(::qtjambi_cast<T>(env, object));
    }
};

template<template<typename T> class Container, typename T, bool = supports_equal<T>::value>
struct ContainerIndexOf{
    static qsizetype function(JNIEnv * env, const void*, jobject, qsizetype) {
        JavaException::raiseUnsupportedOperationException(env, "indexOf(value, index)" QTJAMBI_STACKTRACEINFO );
        return 0;
    }
};

template<template<typename T> class Container, typename T>
struct ContainerIndexOf<Container, T, true>{
    static qsizetype function(JNIEnv * env, const void* ptr, jobject object, qsizetype idx) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return container->indexOf(::qtjambi_cast<T>(env, object), int(idx));
    }
};

template<template<typename T> class Container, typename T, bool = supports_equal<T>::value>
struct ContainerLastIndexOf{
    static qsizetype function(JNIEnv * env, const void*, jobject, qsizetype) {
        JavaException::raiseUnsupportedOperationException(env, "lastIndexOf(value,index)" QTJAMBI_STACKTRACEINFO );
        return 0;
    }
};

template<template<typename T> class Container, typename T>
struct ContainerLastIndexOf<Container, T, true>{
    static qsizetype function(JNIEnv * env, const void* ptr, jobject object, qsizetype idx) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return container->lastIndexOf(::qtjambi_cast<T>(env, object), int(idx));
    }
};

template<template<typename T> class Container, jobject(*objectFromContainer)(JNIEnv *, void*&, AbstractContainerAccess*&), typename T, bool = is_default_constructible_v<T>>
struct ContainerMid{
    static ContainerAndAccessInfo function(JNIEnv * env, const ConstContainerAndAccessInfo&, qsizetype, qsizetype) {
        JavaException::raiseUnsupportedOperationException(env, "mid(index1, index2)" QTJAMBI_STACKTRACEINFO );
        return {};
    }
};

template<template<typename T> class Container, jobject(*objectFromContainer)(JNIEnv *, void*&, AbstractContainerAccess*&), typename T>
struct ContainerMid<Container, objectFromContainer, T, true>{
    static ContainerAndAccessInfo function(JNIEnv * env, const ConstContainerAndAccessInfo& ptr, qsizetype idx1, qsizetype idx2) {
        ContainerAndAccessInfo result;
        const Container<T> *container = static_cast<const Container<T> *>(ptr.container);
        Container<T>* mid = new Container<T>(container->mid(idx1, idx2));
        result.access = ptr.access->clone();
        result.container = mid;
        result.object = objectFromContainer(env, result.container, result.access);
        return result;
    }
};

template<template<typename T> class Container, typename T, IsContainerFunction is_Container1_fct, bool = supports_equal<T>::value>
struct ContainerEquals{
    static jboolean function(JNIEnv * env, const void*, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "operator==(other)" QTJAMBI_STACKTRACEINFO );
        return false;
    }
};

template<template<typename T> class Container, typename T, IsContainerFunction is_Container1_fct>
struct ContainerEquals<Container, T, is_Container1_fct, true>{
    static jboolean function(JNIEnv * env, const void* ptr, jobject other) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        std::unique_ptr<Container<T> > __qt_scoped_pointer;
        Container<T> *__qt_other_pointer = nullptr;
        if (other!= nullptr) {
            if (!is_Container1_fct(env, other, qtjambi_type<T>::id(), QMetaType::fromType<std::remove_cv_t<T>>(), reinterpret_cast<void*&>(__qt_other_pointer))) {
                __qt_scoped_pointer.reset(new Container<T> ());
                __qt_other_pointer = __qt_scoped_pointer.get();
                jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, other);
                while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                    jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                    (*__qt_other_pointer) << ::qtjambi_cast<T>(env, element);
                }
            }
        }else{
            __qt_scoped_pointer.reset(new Container<T> ());
            __qt_other_pointer = __qt_scoped_pointer.get();
        }
        const Container<T>& __qt_other = *__qt_other_pointer;
        return (*container)==__qt_other;
    }
};

template<template<typename T> class Container, typename T>
struct ContainerSize{
    static qsizetype function(JNIEnv * env, const void* ptr) {
        Q_UNUSED(env)
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return container->size();
    }
    static qsizetype function(const void* ptr) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return container->size();
    }
};

template<template<typename T> class Container, typename T, bool = supports_equal<T>::value>
struct ContainerStartsWith{
    static jboolean function(JNIEnv * env, const void*, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "startsWith(value)" QTJAMBI_STACKTRACEINFO );
        return false;
    }
};

template<template<typename T> class Container, typename T>
struct ContainerStartsWith<Container, T, true>{
    static jboolean function(JNIEnv * env, const void* ptr, jobject object) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return container->startsWith(::qtjambi_cast<T>(env, object));
    }
};

template<template<typename T> class Container, typename T, bool = is_default_constructible_v<T>>
struct SequentialContainerValue{
    static jobject function(JNIEnv * env, const void*, qsizetype) {
        JavaException::raiseUnsupportedOperationException(env, "value(i)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename T> class Container, typename T>
struct SequentialContainerValue<Container, T, true>{
    static jobject function(JNIEnv * env, const void* ptr, qsizetype index) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return ::qtjambi_cast<jobject>(env, container->value(index));
    }
};

template<template<typename T> class Container, typename T, bool = is_copy_constructible_v<T> || is_move_constructible_v<T>>
struct ContainerValueDefault{
    static jobject function(JNIEnv * env, const void*, qsizetype, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "value(i,defaultValue)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename T> class Container, typename T>
struct ContainerValueDefault<Container, T, true>{
    static jobject function(JNIEnv * env, const void* ptr, qsizetype index, jobject object) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return ::qtjambi_cast<jobject>(env, container->value(index, ::qtjambi_cast<T>(env, object)));
    }
};

template<template<typename T> class Container, typename T, bool = is_copy_constructible_v<T> || is_move_constructible_v<T>>
struct ContainerAppend{
    static void function(JNIEnv * env, const ContainerInfo&, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "append(value)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename T> class Container, typename T>
struct ContainerAppend<Container, T, true>{
    static void function(JNIEnv * env, const ContainerInfo& ptr, jobject object) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->append(::qtjambi_cast<T>(env, object));
    }
};

template<template<typename T> class Container, typename T, bool = is_default_constructible_v<T>>
struct ContainerFill{
    static void function(JNIEnv * env, const ContainerInfo&, jobject, qsizetype) {
        JavaException::raiseUnsupportedOperationException(env, "fill(value,size)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename T> class Container, typename T>
struct ContainerFill<Container, T, true>{
    static void function(JNIEnv * env, const ContainerInfo& ptr, jobject object, qsizetype size) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->fill(::qtjambi_cast<T>(env, object), size);
    }
};

template<typename T, bool = is_copy_constructible_v<T> || is_move_constructible_v<T>>
struct ContainerAppendList{
    static void function(JNIEnv * env, const ContainerInfo&, ContainerAndAccessInfo&) {
        JavaException::raiseUnsupportedOperationException(env, "QList::append(list)" QTJAMBI_STACKTRACEINFO );
    }
};

template<typename T>
struct ContainerAppendList<T, true>{
    static void function(JNIEnv * env, const ContainerInfo& ptr, ContainerAndAccessInfo& containerInfo) {
        QList<T> *container = static_cast<QList<T> *>(ptr.container);
        std::unique_ptr<QList<T> > __qt_scoped_pointer;
        QList<T> *__qt_object_pointer = nullptr;
        if (containerInfo.object!=nullptr) {
            if (ContainerAPI::getAsQList<T>(env, containerInfo.object, __qt_object_pointer, containerInfo.access)) {
                containerInfo.container = __qt_object_pointer;
            }else{
                __qt_scoped_pointer.reset(new QList<T>());
                __qt_object_pointer = __qt_scoped_pointer.get();
                jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, containerInfo.object);
                while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                    jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                    __qt_object_pointer->append(::qtjambi_cast<T>(env, element));
                }
            }
        }else{
            __qt_scoped_pointer.reset(new QList<T> ());
            __qt_object_pointer = __qt_scoped_pointer.get();
        }
        container->append(*__qt_object_pointer);
    }
};

template<template<typename T> class Container, typename T>
struct ContainerClear{
    static void function(JNIEnv *, const ContainerInfo& ptr) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->clear();
    }
};

template<template<typename T> class Container, typename T, bool = is_copy_constructible_v<T> && is_default_constructible_v<T>>
struct ContainerInsertAt{
    static void function(JNIEnv * env, const ContainerInfo&, qsizetype, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "insert(i,value)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename T> class Container, typename T>
struct ContainerInsertAt<Container, T, true>{
    static void function(JNIEnv * env, const ContainerInfo& ptr, qsizetype i, jobject value) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->insert(i, ::qtjambi_cast<T>(env, value));
    }
};

template<template<typename T> class Container, typename T, bool = is_copy_constructible_v<T> && is_default_constructible_v<T>>
struct ContainerInsertN{
    static void function(JNIEnv * env, const ContainerInfo&, qsizetype, qsizetype, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "insert(i,n,value)" QTJAMBI_STACKTRACEINFO );
    }
    static void function(void*, qsizetype, qsizetype, const void*) {
    }
};

template<template<typename T> class Container, typename T>
struct ContainerInsertN<Container, T, true>{
    static void function(JNIEnv * env, const ContainerInfo& ptr, qsizetype i, qsizetype n, jobject value) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->insert(i, n, ::qtjambi_cast<T>(env, value));
    }
    static void function(void* ptr, qsizetype index, qsizetype n, const void* entry) {
        Container<T> *container = static_cast<Container<T> *>(ptr);
        container->insert(index, n, *reinterpret_cast<const T*>(entry));
    }
};

template<template<typename T> class Container, typename T>
struct ContainerInsert{
    static void function(JNIEnv * env, const ContainerInfo& ptr, jobject value) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->insert(::qtjambi_cast<T>(env, value));
    }
};

template<template<typename T> class Container, typename T, bool = is_copy_constructible_v<T> && is_default_constructible_v<T>>
struct ContainerMove{
    static void function(JNIEnv * env, const ContainerInfo&, qsizetype, qsizetype) {
        JavaException::raiseUnsupportedOperationException(env, "move(index1, index2)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename T> class Container, typename T>
struct ContainerMove<Container, T, true>{
    static void function(JNIEnv * env, const ContainerInfo& ptr, qsizetype idx1, qsizetype idx2) {
        Q_UNUSED(env)
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->move(idx1, idx2);
    }
};

template<template<typename T> class Container, typename T, bool = is_copy_constructible_v<T> || is_move_constructible_v<T>>
struct ContainerPrepend{
    static void function(JNIEnv * env, const ContainerInfo&, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "prepend(value)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename T> class Container, typename T>
struct ContainerPrepend<Container, T, true>{
    static void function(JNIEnv * env, const ContainerInfo& ptr, jobject object) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->prepend(::qtjambi_cast<T>(env, object));
    }
};

template<template<typename T> class Container, typename T, bool = supports_equal<T>::value>
struct ContainerRemoveAll{
    static qsizetype function(JNIEnv * env, const ContainerInfo&, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "removeAll(value)" QTJAMBI_STACKTRACEINFO );
        return 0;
    }
};

template<template<typename T> class Container, typename T>
struct ContainerRemoveAll<Container, T, true>{
    static qsizetype function(JNIEnv * env, const ContainerInfo& ptr, jobject object) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        return container->removeAll(::qtjambi_cast<T>(env, object));
    }
};

template<template<typename T> class Container, typename T>
struct ContainerRemoveAt{
    static void function(JNIEnv * env, const ContainerInfo& ptr, qsizetype idx) {
        Q_UNUSED(env)
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->removeAt(idx);
    }
    static void function(void* ptr, qsizetype idx) {
        Container<T> *container = static_cast<Container<T> *>(ptr);
        container->removeAt(idx);
    }
};

template<template<typename T> class Container, typename T, bool = is_default_constructible_v<T>>
struct ContainerRemoveI{
    static void function(JNIEnv * env, const ContainerInfo&, qsizetype) {
        JavaException::raiseUnsupportedOperationException(env, "remove(index)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename T> class Container, typename T>
struct ContainerRemoveI<Container, T, true>{
    static void function(JNIEnv * env, const ContainerInfo& ptr, qsizetype idx) {
        Q_UNUSED(env)
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->remove(idx);
    }
};

template<template<typename T> class Container, typename T, bool = is_default_constructible_v<T>>
struct ContainerRemoveN{
    static void function(JNIEnv * env, const ContainerInfo&, qsizetype, qsizetype) {
        JavaException::raiseUnsupportedOperationException(env, "remove(index,n)" QTJAMBI_STACKTRACEINFO );
    }
    static void function(void*, qsizetype, qsizetype) {}
};

template<template<typename T> class Container, typename T>
struct ContainerRemoveN<Container, T, true>{
    static void function(JNIEnv * env, const ContainerInfo& ptr, qsizetype idx, qsizetype n) {
        Q_UNUSED(env)
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->remove(idx, n);
    }
    static void function(void* ptr, qsizetype idx, qsizetype n) {
        Container<T> *container = static_cast<Container<T> *>(ptr);
        container->remove(idx, n);
    }
};

template<template<typename T> class Container, typename T, bool = supports_equal<T>::value>
struct ContainerRemoveOne{
    static jboolean function(JNIEnv * env, const ContainerInfo&, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "removeOne(value)" QTJAMBI_STACKTRACEINFO );
        return false;
    }
};

template<template<typename T> class Container, typename T>
struct ContainerRemoveOne<Container, T, true>{
    static jboolean function(JNIEnv * env, const ContainerInfo& ptr, jobject object) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        return container->removeOne(::qtjambi_cast<T>(env, object));
    }
};

template<template<typename T> class Container, typename T, bool = supports_equal<T>::value>
struct ContainerRemove{
    static jboolean function(JNIEnv * env, const ContainerInfo&, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "remove(value)" QTJAMBI_STACKTRACEINFO );
        return false;
    }
};

template<template<typename T> class Container, typename T>
struct ContainerRemove<Container, T, true>{
    static jboolean function(JNIEnv * env, const ContainerInfo& ptr, jobject object) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        return container->remove(::qtjambi_cast<T>(env, object));
    }
};

template<template<typename T> class Container, typename T, bool = is_default_constructible_v<T> && !std::is_const<T>::value>
struct ContainerReplace{
    static void function(JNIEnv * env, const ContainerInfo&, qsizetype,jobject) {
        JavaException::raiseUnsupportedOperationException(env, "replace(index,value)" QTJAMBI_STACKTRACEINFO );
    }
    static void function(void*, qsizetype, const void*) {}
};

template<template<typename T> class Container, typename T>
struct ContainerReplace<Container, T, true>{
    static void function(JNIEnv * env, const ContainerInfo& ptr, qsizetype idx,jobject newObject) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->replace(idx, ::qtjambi_cast<T>(env, newObject));
    }

    static void function(void* ptr, qsizetype idx, const void* newObject) {
        Container<T> *container = static_cast<Container<T> *>(ptr);
        container->replace(idx, *reinterpret_cast<const T*>(newObject));
    }
};

template<template<typename T> class Container, typename T, bool = is_default_constructible_v<T>>
struct ContainerReserve{
    static void function(JNIEnv * env, const ContainerInfo&, qsizetype) {
        JavaException::raiseUnsupportedOperationException(env, "reserve(size)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename T> class Container, typename T>
struct ContainerReserve<Container, T, true>{
    static void function(JNIEnv * env, const ContainerInfo& ptr, qsizetype size) {
        Q_UNUSED(env)
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->reserve(size);
    }
};

template<template<typename T> class Container, typename T, bool = is_default_constructible_v<T> && !std::is_const<T>::value>
struct ContainerResize{
    static void function(JNIEnv * env, const ContainerInfo&, qsizetype) {
        JavaException::raiseUnsupportedOperationException(env, "resize(size)" QTJAMBI_STACKTRACEINFO );
    }
    static void function(void*, qsizetype) {
    }
};

template<template<typename T> class Container, typename T>
struct ContainerResize<Container, T, true>{
    static void function(JNIEnv * env, const ContainerInfo& ptr, qsizetype size) {
        Q_UNUSED(env)
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->resize(size);
    }
    static void function(void* ptr, qsizetype size) {
        Container<T> *container = static_cast<Container<T> *>(ptr);
        container->resize(size);
    }
};

template<template<typename T> class Container, typename T, bool = is_default_constructible_v<T>>
struct ContainerSqueeze{
    static void function(JNIEnv * env, const ContainerInfo&) {
        JavaException::raiseUnsupportedOperationException(env, "squeeze()" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename T> class Container, typename T>
struct ContainerSqueeze<Container, T, true>{
    static void function(JNIEnv * env, const ContainerInfo& ptr) {
        Q_UNUSED(env)
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->squeeze();
    }
};

template<template<typename T> class Container, typename T>
struct ContainerSwap{
    static void function(JNIEnv * env, const ContainerInfo& ptr, qsizetype idx1, qsizetype idx2) {
        Q_UNUSED(env)
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->swapItemsAt(idx1, idx2);
    }
};

template<typename T>
PtrOwnerFunction registeredOwnerFunction(){
    static PtrOwnerFunction ownerFunction = ContainerAPI::registeredOwnerFunction(typeid(T));
    return ownerFunction;
}

template<typename T, typename Super>
class ReferenceCountingSequentialSetAccess : public Super, public ReferenceCountingSetContainer{
protected:
    ReferenceCountingSequentialSetAccess(){}
public:
    AbstractReferenceCountingContainer* asRC() override {return this;}
    void updateRC(JNIEnv * env, const ContainerInfo& container) override {
        Super* _this = this;
        JniLocalFrame frame(env, 200);
        jobject set{nullptr};
        if constexpr(QtJambiPrivate::ContainerContentType<T>::type==AbstractContainerAccess::PointerToQObject){
            set = QtJambiAPI::newJavaArrayList(env);
            auto iterator = _this->constElementIterator(container.container);
            while(iterator->hasNext()){
                const void* content = iterator->next();
                if(jobject obj = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content)))
                    QtJambiAPI::addToJavaCollection(env, set, obj);
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<T>::type==AbstractContainerAccess::FunctionPointer){
            set = QtJambiAPI::newJavaArrayList(env);
            auto iterator = _this->constElementIterator(container.container);
            while(iterator->hasNext()){
                const void* content = iterator->next();
                if(jobject obj = findFunctionPointerObject(env, content, typeid(T)))
                    QtJambiAPI::addToJavaCollection(env, set, obj);
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<T>::type==AbstractContainerAccess::Pointer){
            set = QtJambiAPI::newJavaArrayList(env);
            auto iterator = _this->constElementIterator(container.container);
            while(iterator->hasNext()){
                const void* content = iterator->next();
                if(jobject obj = QtJambiAPI::findObject(env, content))
                    QtJambiAPI::addToJavaCollection(env, set, obj);
            }
        }else if constexpr(QtJambiPrivate::ContainerContentType<T>::isContainer && ContainerContentType<T>::needsReferenceCounting){
            set = QtJambiAPI::newJavaHashSet(env);
            if(AbstractContainerAccess* access = ContainerContentType<T>::accessFactory()){
                auto iterator = _this->constElementIterator(container.container);
                while(iterator->hasNext()){
                    AbstractReferenceCountingContainer::unfoldAndAddContainer(env, set, iterator->next(), ContainerContentType<T>::type, _this->elementMetaType(), access);
                }
                access->dispose();
            }
        }else{
            //Q_STATIC_ASSERT_X(false, "No reference counting required");
            return;
        }
        clearRC(env, container.object);
        addAllRC(env, container.object, set);
    }
};

template<typename T, typename Super>
class OwnerFunctionSequentialAccess : public Super{
protected:
    OwnerFunctionSequentialAccess(){}
public:
    const QObject* getOwner(const void* container) override{
        Super* _this = this;
        if constexpr(QtJambiPrivate::ContainerContentType<T>::isContainer){
            const QObject* owner = nullptr;
            auto iter = _this->constElementIterator(container);
            if(iter->hasNext()){
                if(AbstractContainerAccess* elementNestedContainerAccess = ContainerContentType<T>::accessFactory()){
                    while(iter->hasNext()){
                        const void* current = iter->next();
                        owner = elementNestedContainerAccess->getOwner(current);
                        if(owner)
                            break;
                    }
                    elementNestedContainerAccess->dispose();
                }
            }
            return owner;
        }else{
            PtrOwnerFunction ownerFunction = registeredOwnerFunction<T>();
            if(ownerFunction){
                auto iter = _this->constElementIterator(container);
                while(iter->hasNext()){
                    const void* current = iter->next();
                    if(const QObject* owner = ownerFunction(current))
                        return owner;
                }
            }
            return nullptr;
        }
    }
    bool hasOwnerFunction() override{
        if constexpr(QtJambiPrivate::ContainerContentType<T>::isContainer){
            if(AbstractContainerAccess* elementNestedContainerAccess = ContainerContentType<T>::accessFactory()){
                bool result = elementNestedContainerAccess->hasOwnerFunction();
                elementNestedContainerAccess->dispose();
                return result;
            }
            return false;
        }else{
            return registeredOwnerFunction<T>()!=nullptr;
        }
    }
};

template<typename T, typename Super,
         bool T_is_pointer = (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0 /*true*/>
struct SequentialAccessSuperclassDecider{
    // this is the superclass if container contains pointers:
    typedef ReferenceCountingSequentialSetAccess<T,Super> type;
};

template<typename T, typename Super,
         bool T_needsOwnerCheck = ContainerContentType<T>::needsOwnerCheck,
         bool T_needsReferenceCounting = ContainerContentType<T>::needsReferenceCounting>
struct SequentialAccessSuperclassDecider_ContainerType{
    typedef ReferenceCountingSequentialSetAccess<T,Super> type;
};

template<typename T, typename Super,
         bool T_is_container = ContainerContentType<T>::isContainer /*true*/>
struct SequentialAccessSuperclassDecider_IsContainer : SequentialAccessSuperclassDecider_ContainerType<T,Super>{
};

template<typename T, typename Super>
struct SequentialAccessSuperclassDecider_ContainerType<T,Super,false,false>{
    typedef Super type;
};

template<typename T, typename Super>
struct SequentialAccessSuperclassDecider_ContainerType<T,Super,true,false>{
    typedef OwnerFunctionSequentialAccess<T,Super> type;
};

template<typename T, typename Super>
struct SequentialAccessSuperclassDecider_ContainerType<T,Super,false,true>{
    typedef ReferenceCountingSequentialSetAccess<T,Super> type;
};

template<typename T, typename Super>
struct SequentialAccessSuperclassDecider_IsContainer<T,Super,false> : SequentialAccessSuperclassDecider_ContainerType<T,Super>{
};

template<typename T, typename Super>
struct SequentialAccessSuperclassDecider<T,Super,false> : SequentialAccessSuperclassDecider_IsContainer<T,Super>{
};

} // namespace QtJambiPrivate

template<typename T>
class QListAccess : public QtJambiPrivate::SequentialAccessSuperclassDecider<T,AbstractListAccess>::type{
    typedef typename QtJambiPrivate::SequentialAccessSuperclassDecider<T,AbstractListAccess>::type Super;
protected:
    QListAccess(){}
public:
    static AbstractListAccess* newInstance(){
        static QListAccess<T> instance;
        return &instance;
    }

    AbstractListAccess* clone() override{
        return this;
    }

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    AbstractSpanAccess* createSpanAccess(bool isConst) override;
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)

    bool isDetached(const void* container) override{
        return reinterpret_cast<const QList<T> *>(container)->isDetached();
    }

    void detach(const ContainerInfo& container) override{
        reinterpret_cast<QList<T> *>(container.container)->detach();
    }

    bool isSharedWith(const void* container, const void* container2) override{
        return reinterpret_cast<const QList<T> *>(container)->isSharedWith(*reinterpret_cast<const QList<T> *>(container2));
    }

    void swap(JNIEnv *env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override{
        reinterpret_cast<QList<T> *>(container.container)->swap(*reinterpret_cast<QList<T> *>(container2.container));
        if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
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

    bool append(void* container, const void* entry) override{
        reinterpret_cast<QList<T> *>(container)->append(*reinterpret_cast<const T*>(entry));
        if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::isContainer){
                if(JniEnvironment env{100}){
                    if(jobject object = QtJambiAPI::findObject(env, container))
                        Super::updateRC(env, {object, container});
                }
            }else{
                if(JniEnvironment env{100}){
                    if(jobject object = QtJambiAPI::findObject(env, container)){
                        if(jobject valueObj = QtJambiAPI::findObject(env, entry))
                            Super::addRC(env, object, valueObj);
                    }
                }
            }
        }
        return true;
    }

    const QMetaType& elementMetaType() override {
        static QMetaType type(QMetaType::fromType<std::remove_cv_t<T>>());
        return type;
    }

    AbstractContainerAccess::DataType elementType() override{
        return QtJambiPrivate::ContainerContentType<T>::type;
    }

    AbstractContainerAccess* elementNestedContainerAccess() override {
        return QtJambiPrivate::ContainerContentType<T>::accessFactory();
    }
    bool hasNestedContainerAccess() override {
        return QtJambiPrivate::ContainerContentType<T>::isContainer;
    }
    bool hasNestedPointers() override {
        return QtJambiPrivate::ContainerContentType<T>::isContainer && QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting;
    }

    size_t sizeOf() const override {
        return sizeof(QList<T>);
    }
    size_t alignOf() const override {
        return alignof(QList<T>);
    }
    void* constructContainer(void* placement) override {
        return new(placement) QList<T>();
    }
    void* constructContainer(void* placement, const void* copyOf) override {
        return new(placement) QList<T>(*reinterpret_cast<const QList<T>*>(copyOf));
    }
    void* constructContainer(JNIEnv *, void* placement, const ConstContainerAndAccessInfo& copyOf) override {
        return constructContainer(placement, copyOf.container);
    }
    void* constructContainer(void* placement, void* move) override {
        return new(placement) QList<T>(std::move(*reinterpret_cast<const QList<T>*>(move)));
    }
    void* constructContainer(JNIEnv *, void* placement, const ContainerAndAccessInfo& move) override {
        return constructContainer(placement, move.container);
    }
    bool destructContainer(void* container) override {
        reinterpret_cast<QList<T>*>(container)->~QList<T>();
        return true;
    }
    void assign(void* container, const void* other) override {
        (*reinterpret_cast<QList<T>*>(container)) = (*reinterpret_cast<const QList<T>*>(other));
        if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            if(JniEnvironment env{100}){
                if(jobject object = QtJambiAPI::findObject(env, container))
                    Super::updateRC(env, {object, container});
            }
        }
    }
    void assign(JNIEnv *env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override {
        (*reinterpret_cast<QList<T>*>(container.container)) = (*reinterpret_cast<const QList<T>*>(other.container));
        if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
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
        return QtJambiPrivate::container_registry_impl<QList<T>>::register_container(containerTypeName, this);
    }

    jobject at(JNIEnv * env, const void* container, qsizetype index) override {
        if constexpr(QtJambiPrivate::is_default_constructible_v<T>){
            return ::qtjambi_cast<jobject>(env, static_cast<const QList<T> *>(container)->at(index));
        }else{
            JavaException::raiseUnsupportedOperationException(env, "at(i)" QTJAMBI_STACKTRACEINFO );
            return nullptr;
        }
    }

    const void* at(const void* ptr, qsizetype index) override {
        if constexpr(QtJambiPrivate::is_default_constructible_v<T>){
            const QList<T> *container = static_cast<const QList<T> *>(ptr);
            return &container->at(index);
        }else{
            return nullptr;
        }
    }

    void* at(void* ptr, qsizetype index) override {
        if constexpr(QtJambiPrivate::is_default_constructible_v<T>){
            QList<T> &container = *static_cast<QList<T> *>(ptr);
            T& result = container[index];
            return &result;
        }else{
            return nullptr;
        }
    }

    jobject value(JNIEnv * env, const void* container, qsizetype index) override {
        return QtJambiPrivate::SequentialContainerValue<QList, T>::function(env, container, index);
    }

    jobject value(JNIEnv * env, const void* container, qsizetype index, jobject defaultValue) override {
        return QtJambiPrivate::ContainerValueDefault<QList, T>::function(env, container, index, defaultValue);
    }

    jboolean startsWith(JNIEnv * env, const void* container, jobject value) override {
        return QtJambiPrivate::ContainerStartsWith<QList, T>::function(env, container, value);
    }

    qsizetype size(JNIEnv * env, const void* container) override {
        return QtJambiPrivate::ContainerSize<QList, T>::function(env, container);
    }

    qsizetype size(const void* container) override {
        return QtJambiPrivate::ContainerSize<QList, T>::function(container);
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        return QtJambiPrivate::ContainerEquals<QList, T, ContainerAPI::getAsQList>::function(env, container, other);
    }

    ContainerAndAccessInfo mid(JNIEnv * env, const ConstContainerAndAccessInfo& container, qsizetype index1, qsizetype index2) override {
        return QtJambiPrivate::ContainerMid<QList, ContainerAPI::objectFromQList, T>::function(env, container, index1, index2);
    }

    qsizetype lastIndexOf(JNIEnv * env, const void* container, jobject value, qsizetype index) override {
        return QtJambiPrivate::ContainerLastIndexOf<QList, T>::function(env, container, value, index);
    }

    qsizetype indexOf(JNIEnv * env, const void* container, jobject value, qsizetype index) override {
        return QtJambiPrivate::ContainerIndexOf<QList, T>::function(env, container, value, index);
    }

    jboolean endsWith(JNIEnv * env, const void* container, jobject value) override {
        return QtJambiPrivate::ContainerEndsWith<QList, T>::function(env, container, value);
    }

    qsizetype count(JNIEnv * env, const void* container, jobject value) override {
        return QtJambiPrivate::ContainerCountObject<QList, T>::function(env, container, value);
    }

    jboolean contains(JNIEnv * env, const void* container, jobject value) override {
        return QtJambiPrivate::ContainerContains<QList, T>::function(env, container, value);
    }

    bool contains(const void* container, const void* value) {
        return QtJambiPrivate::ContainerContains<QList, T>::function(container, value);
    }

    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        return QtJambiPrivate::ContainerConstBegin<QList, T>::function(env, container);
    }

    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        return QtJambiPrivate::ContainerConstEnd<QList, T>::function(env, container);
    }

    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override {
        return QtJambiPrivate::ContainerBegin<QList, T>::function(env, container);
    }

    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override {
        return QtJambiPrivate::ContainerEnd<QList, T>::function(env, container);
    }

    void appendList(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) override {
        QtJambiPrivate::ContainerAppendList<T>::function(env, container, containerInfo);
        if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::isContainer){
                Super::updateRC(env, container);
            }else{
                Super::addAllRC(env, container.object, Super::findContainer(env, containerInfo.object));
            }
        }
    }

    void swapItemsAt(JNIEnv * env, const ContainerInfo& container, qsizetype index1, qsizetype index2) override {
        QtJambiPrivate::ContainerSwap<QList, T>::function(env, container, index1, index2);
    }

    void reserve(JNIEnv * env, const ContainerInfo& container, qsizetype size) override {
        QtJambiPrivate::ContainerReserve<QList, T>::function(env, container, size);
    }

    void replace(void* container, qsizetype index, const void* value) override {
        if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::isContainer){
                QtJambiPrivate::ContainerReplace<QList, T>::function(container, index, value);
                if(JniEnvironment env{100}){
                    if(jobject object = QtJambiAPI::findObject(env, container))
                        Super::updateRC(env, {object, container});
                }
            }else{
                if(JniEnvironment env{100}){
                    jobject oldValue = at(env, container, index);
                    QtJambiPrivate::ContainerReplace<QList, T>::function(container, index, value);
                    if(jobject object = QtJambiAPI::findObject(env, container)){
                        if(oldValue && !contains(env, container, oldValue))
                            Super::removeRC(env, object, oldValue);
                        if(jobject valueObj = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(value)))
                            Super::addRC(env, object, valueObj);
                    }
                }else{
                    QtJambiPrivate::ContainerReplace<QList, T>::function(container, index, value);
                }
            }
        }else{
            QtJambiPrivate::ContainerReplace<QList, T>::function(container, index, value);
        }
    }

    void replace(JNIEnv * env, const ContainerInfo& container, qsizetype index, jobject value) override {
        if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::isContainer){
                QtJambiPrivate::ContainerReplace<QList, T>::function(env, container, index, value);
                Super::updateRC(env, container);
            }else{
                jobject oldValue = at(env, container.container, index);
                QtJambiPrivate::ContainerReplace<QList, T>::function(env, container, index, value);
                if(oldValue && !contains(env, container.container, oldValue))
                    Super::removeRC(env, container.object, oldValue);
                if(value)
                    Super::addRC(env, container.object, value);
            }
        }else{
            QtJambiPrivate::ContainerReplace<QList, T>::function(env, container, index, value);
        }
    }

    void remove(void* container, qsizetype index, qsizetype n) override {
        if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::isContainer){
                QtJambiPrivate::ContainerRemoveN<QList, T>::function(container, index, n);
                if(JniEnvironment env{100}){
                    if(jobject object = QtJambiAPI::findObject(env, container))
                        Super::updateRC(env, {object,container});
                }
            }else{
                if(JniEnvironment env{100}){
                    if(jobject object = QtJambiAPI::findObject(env, container)){
                        if(n==1){
                            jobject oldValue = at(env, container, index);
                            QtJambiPrivate::ContainerRemoveN<QList, T>::function(container, index, n);
                            Super::removeRC(env, object, oldValue);
                        }else{
                            qsizetype _size = size(env, container);
                            jobject removedValues = QtJambiAPI::newJavaArrayList(env);
                            for(qsizetype i = index; i<=index+n && i<_size; ++i){
                                QtJambiAPI::addToJavaCollection(env, removedValues, at(env, container, i));
                            }
                            QtJambiPrivate::ContainerRemoveN<QList, T>::function(container, index, n);
                            jobject iter = QtJambiAPI::iteratorOfJavaIterable(env, removedValues);
                            while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                                jobject value = QtJambiAPI::nextOfJavaIterator(env, iter);
                                Super::removeRC(env, object, value);
                            }
                        }
                    }
                }
            }
        }else{
            QtJambiPrivate::ContainerRemoveN<QList, T>::function(container, index, n);
        }
    }

    void remove(JNIEnv * env, const ContainerInfo& container, qsizetype index, qsizetype n) override {
        if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::isContainer){
                QtJambiPrivate::ContainerRemoveN<QList, T>::function(env, container, index, n);
                Super::updateRC(env, container);
            }else{
                if(n==1){
                    jobject oldValue = at(env, container.container, index);
                    QtJambiPrivate::ContainerRemoveN<QList, T>::function(env, container, index, n);
                    Super::removeRC(env, container.object, oldValue);
                }else{
                    qsizetype _size = size(env, container.container);
                    jobject removedValues = QtJambiAPI::newJavaArrayList(env);
                    for(qsizetype i = index; i<=index+n && i<_size; ++i){
                        QtJambiAPI::addToJavaCollection(env, removedValues, at(env, container.container, i));
                    }
                    QtJambiPrivate::ContainerRemoveN<QList, T>::function(env, container, index, n);
                    jobject iter = QtJambiAPI::iteratorOfJavaIterable(env, removedValues);
                    while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                        jobject value = QtJambiAPI::nextOfJavaIterator(env, iter);
                        Super::removeRC(env, container.object, value);
                    }
                }
            }
        }else{
            QtJambiPrivate::ContainerRemoveN<QList, T>::function(env, container, index, n);
        }
    }

    qsizetype removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) override {
        qsizetype result = QtJambiPrivate::ContainerRemoveAll<QList, T>::function(env, container, value);
        if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::isContainer){
                if(result>0)
                    Super::updateRC(env, container);
            }else{
                Super::removeRC(env, container.object, value, result);
            }
        }
        return result;
    }

    void move(JNIEnv * env, const ContainerInfo& container, qsizetype index1, qsizetype index2) override {
        QtJambiPrivate::ContainerMove<QList, T>::function(env, container, index1, index2);
    }

    void insert(JNIEnv * env, const ContainerInfo& container, qsizetype index, qsizetype n, jobject value) override {
        QtJambiPrivate::ContainerInsertN<QList, T>::function(env, container, index, n, value);
        if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::isContainer){
                Super::updateRC(env, container);
            }else{
                Super::addRC(env, container.object, value);
            }
        }
    }

    void insert(void* container, qsizetype index, qsizetype n, const void* value) override {
        QtJambiPrivate::ContainerInsertN<QList, T>::function(container, index, n, value);
        if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::isContainer){
                if(JniEnvironment env{100}){
                    if(jobject object = QtJambiAPI::findObject(env, container))
                        Super::updateRC(env, {object, container});
                }
            }else{
                if(JniEnvironment env{100}){
                    if(jobject object = QtJambiAPI::findObject(env, container)){
                        if(jobject valueObj = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(value)))
                            Super::addRC(env, object, valueObj);
                    }
                }
            }
        }
    }

    void clear(JNIEnv * env, const ContainerInfo& container) override {
        QtJambiPrivate::ContainerClear<QList, T>::function(env, container);
        if constexpr(QtJambiPrivate::ContainerContentType<QList<T>>::needsReferenceCounting){
            Super::clearRC(env, container.object);
        }
    }

    qsizetype capacity(JNIEnv * env, const void* container) override {
        Q_UNUSED(env)
        return static_cast<const QList<T> *>(container)->capacity();
    }

    void fill(JNIEnv * env, const ContainerInfo& container, jobject value, qsizetype _size) override {
        if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::isContainer){
                QtJambiPrivate::ContainerFill<QList, T>::function(env, container, value, _size);
                Super::updateRC(env, container);
            }else{
                qsizetype oldSize = size(env, container.container);
                QtJambiPrivate::ContainerFill<QList, T>::function(env, container, value, _size);
                for(;oldSize<_size;++oldSize){
                    Super::addRC(env, container.object, value);
                }
            }
        }else{
            QtJambiPrivate::ContainerFill<QList, T>::function(env, container, value, _size);
        }
    }

    void resize(JNIEnv * env, const ContainerInfo& container, qsizetype newSize) override {
        QtJambiPrivate::ContainerResize<QList, T>::function(env, container, newSize);
    }

    void resize(void* container, qsizetype newSize) override {
        QtJambiPrivate::ContainerResize<QList, T>::function(container, newSize);
    }

    void squeeze(JNIEnv * env, const ContainerInfo& container) override {
        QtJambiPrivate::ContainerSqueeze<QList, T>::function(env, container);
    }
private:
    template<bool is_const>
    class ElementIterator : public AbstractListAccess::ElementIterator{
        using Container = std::conditional_t<is_const, const QList<T>, QList<T>>;
        using iterator = decltype(std::declval<Container>().begin());
        QListAccess<T>* m_access;
        iterator current;
        iterator end;
        ElementIterator(const ElementIterator& other)
            :m_access(other.m_access),
            current(other.current),
            end(other.end) {}
    protected:
        AbstractSequentialAccess* access() override { return m_access; }
    public:
        ElementIterator(QListAccess<T>* _access, Container& container)
            : m_access(_access),
            current(container.begin()),
            end(container.end()) {}
        ~ElementIterator() override {};
        bool hasNext() override {return current!=end;};
        jobject next(JNIEnv * env) override {
            jobject obj = ::qtjambi_cast<jobject>(env, *current);
            ++current;
            return obj;
        }
        const void* next() override {
            const void* result = QtJambiPrivate::ContainerContentDeref<T>::deref(*current);
            ++current;
            return result;
        }
        const void* constNext() override {
            const void* result = &*current;
            ++current;
            return result;
        };
        bool isConst() override{
            return is_const;
        }
        void* mutableNext() override {
            if constexpr(!is_const){
                void* result = &*current;
                ++current;
                return result;
            }else{
                return nullptr;
            }
        }

        bool operator==(const AbstractSequentialAccess::ElementIterator& other) const override {
            return current==reinterpret_cast<const ElementIterator&>(other).current;
        }
        std::unique_ptr<AbstractSequentialAccess::ElementIterator> clone() const override {
            return std::unique_ptr<AbstractSequentialAccess::ElementIterator>(new ElementIterator(*this));
        }
        std::function<jobject(JNIEnv*,const void*)> elementConverter() const override {
            return [](JNIEnv* env,const void* pointer) -> jobject{
                return ::qtjambi_cast<jobject>(env, reinterpret_cast<const T*>(pointer));
            };
        }
    };
public:
    std::unique_ptr<AbstractListAccess::ElementIterator> elementIterator(const void* container) override {
        return std::unique_ptr<AbstractListAccess::ElementIterator>(new ElementIterator<true>(this, *reinterpret_cast<const QList<T>*>(container)));
    }
    std::unique_ptr<AbstractListAccess::ElementIterator> elementIterator(void* container) override {
        return std::unique_ptr<AbstractListAccess::ElementIterator>(new ElementIterator<false>(this, *reinterpret_cast<QList<T>*>(container)));
    }
};

template<typename T>
class QSetAccess : public QtJambiPrivate::SequentialAccessSuperclassDecider<T,AbstractSetAccess>::type{
    typedef typename QtJambiPrivate::SequentialAccessSuperclassDecider<T,AbstractSetAccess>::type Super;
protected:
    QSetAccess(){}
public:
    static AbstractSetAccess* newInstance(){
        static QSetAccess<T> instance;
        return &instance;
    }

    AbstractSetAccess* clone() override{
        return this;
    }

    bool isDetached(const void* container) override{
        return reinterpret_cast<const QSet<T> *>(container)->isDetached();
    }

    void detach(const ContainerInfo& container) override{
        reinterpret_cast<QSet<T> *>(container.container)->detach();
    }

    bool isSharedWith(const void* container, const void* container2) override{
        return reinterpret_cast<const QHash<T,QHashDummyValue> *>(container)->isSharedWith(*reinterpret_cast<const QHash<T,QHashDummyValue> *>(container2));
    }

    void swap(JNIEnv *env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override{
        reinterpret_cast<QSet<T> *>(container.container)->swap(*reinterpret_cast<QSet<T> *>(container2.container));
        if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
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

    const QMetaType& elementMetaType() override {
        static QMetaType type(QMetaType::fromType<std::remove_cv_t<T>>());
        return type;
    }

    AbstractContainerAccess::DataType elementType() override{
        return QtJambiPrivate::ContainerContentType<T>::type;
    }

    AbstractContainerAccess* elementNestedContainerAccess() override {
        return QtJambiPrivate::ContainerContentType<T>::accessFactory();
    }
    bool hasNestedContainerAccess() override {
        return QtJambiPrivate::ContainerContentType<T>::isContainer;
    }
    bool hasNestedPointers() override {
        return QtJambiPrivate::ContainerContentType<T>::isContainer && QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting;
    }

    void assign(void* container, const void* other) override {
        (*reinterpret_cast<QSet<T>*>(container)) = (*reinterpret_cast<const QSet<T>*>(other));
    }
    void assign(JNIEnv *env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override {
        (*reinterpret_cast<QSet<T>*>(container.container)) = (*reinterpret_cast<const QSet<T>*>(other.container));
        if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
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
        return sizeof(QSet<T>);
    }
    size_t alignOf() const override {
        return alignof(QSet<T>);
    }
    void* constructContainer(void* placement) override {
        return new(placement) QSet<T>();
    }
    void* constructContainer(void* placement, const void* copyOf) override {
        return new(placement) QSet<T>(*reinterpret_cast<const QSet<T>*>(copyOf));
    }
    void* constructContainer(JNIEnv *, void* placement, const ConstContainerAndAccessInfo& copyOf) override {
        return constructContainer(placement, copyOf.container);
    }
    void* constructContainer(void* placement, void* move) override {
        return new(placement) QSet<T>(std::move(*reinterpret_cast<const QSet<T>*>(move)));
    }
    void* constructContainer(JNIEnv *, void* placement, const ContainerAndAccessInfo& move) override {
        return constructContainer(placement, move.container);
    }
    bool destructContainer(void* container) override {
        reinterpret_cast<QSet<T>*>(container)->~QSet<T>();
        return true;
    }

    QMetaType registerContainer(QByteArrayView containerTypeName) override {
        return QtJambiPrivate::container_registry_impl<QSet<T>>::register_container(containerTypeName, this);
    }

    qsizetype capacity(JNIEnv * env, const void* container) override {
        Q_UNUSED(env)
        return static_cast<const QSet<T> *>(container)->capacity();
    }

    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        return QtJambiPrivate::ContainerConstBegin<QSet, T>::function(env, container);
    }

    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        return QtJambiPrivate::ContainerConstEnd<QSet, T>::function(env, container);
    }

    jboolean contains(JNIEnv * env, const void* container, jobject value) override {
        return QtJambiPrivate::ContainerContains<QSet, T>::function(env, container, value);
    }

    jboolean intersects(JNIEnv * env, const void* container, jobject other) override {
        return QtJambiPrivate::ContainerIntersects<QSet, T, ContainerAPI::getAsQSet>::function(env, container, other);
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        return QtJambiPrivate::ContainerEquals<QSet, T, ContainerAPI::getAsQSet>::function(env, container, other);
    }

    qsizetype size(JNIEnv * env, const void* container) override {
        return QtJambiPrivate::ContainerSize<QSet, T>::function(env, container);
    }

    qsizetype size(const void* container) override {
        return QtJambiPrivate::ContainerSize<QSet, T>::function(container);
    }

    ContainerAndAccessInfo values(JNIEnv * env, const ConstContainerInfo& container) override {
        ContainerAndAccessInfo result;
        const QSet<T>  *set = static_cast<const QSet<T> *>(container.container);
        QList<T>* list = new QList<T>(set->values());
        result.container = list;
        result.access = QListAccess<T>::newInstance();
        result.object = ContainerAPI::objectFromQList(env, result.container, result.access);
        if(result.object){
            QtJambiAPI::setJavaOwnership(env, result.object);
        }else{
            delete list;
            result.access->dispose();
            result.container = nullptr;
        }
        return result;
    }

    void clear(JNIEnv * env, const ContainerInfo& container) override {
        QtJambiPrivate::ContainerClear<QSet, T>::function(env, container);
        if constexpr(QtJambiPrivate::ContainerContentType<QSet<T>>::needsReferenceCounting){
            Super::clearRC(env, container.object);
        }
    }
    void insert(JNIEnv * env, const ContainerInfo& container, jobject value) override {
        QtJambiPrivate::ContainerInsert<QSet, T>::function(env, container, value);
        if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::isContainer){
                Super::updateRC(env, container);
            }else{
                Super::addRC(env, container.object, value);
            }
        }
    }
    void intersect(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override {
        QtJambiPrivate::ContainerIntersect<QSet, T, ContainerAPI::getAsQSet>::function(env, container, other);
        if constexpr(QtJambiPrivate::ContainerContentType<QSet<T>>::needsReferenceCounting){
            Super::clearRC(env, container.object);
        }
    }
    jboolean remove(JNIEnv * env, const ContainerInfo& container, jobject value) override {
        jboolean result = QtJambiPrivate::ContainerRemove<QSet, T>::function(env, container, value);
        if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
            if constexpr(QtJambiPrivate::ContainerContentType<T>::isContainer){
                if(result)
                    Super::updateRC(env, container);
            }else{
                if(result)
                    Super::removeRC(env, container.object, value);
            }
        }
        return result;
    }
    void reserve(JNIEnv * env, const ContainerInfo& container, qsizetype newSize) override {
        QtJambiPrivate::ContainerReserve<QSet, T>::function(env, container, newSize);
    }
    void subtract(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override {
        QtJambiPrivate::ContainerSubtract<QSet, T, ContainerAPI::getAsQSet>::function(env, container, other);
        if constexpr(QtJambiPrivate::ContainerContentType<QSet<T>>::needsReferenceCounting){
            Super::clearRC(env, container.object);
        }
    }
    void unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override {
        QtJambiPrivate::ContainerUnite<QSet, T, ContainerAPI::getAsQSet>::function(env, container, other);
        if constexpr(QtJambiPrivate::ContainerContentType<QSet<T>>::needsReferenceCounting){
            Super::clearRC(env, container.object);
        }
    }
private:
    class ElementIterator : public AbstractSetAccess::ElementIterator{
        QSetAccess<T>* m_access;
        typename QSet<T>::ConstIterator current;
        typename QSet<T>::ConstIterator end;
        ElementIterator(const ElementIterator& other)
            :m_access(other.m_access),
            current(other.current),
            end(other.end) {}
    protected:
        AbstractSequentialAccess* access() override { return m_access; }
    public:
        ElementIterator(QSetAccess<T>* _access, const QSet<T>& container)
            :m_access(_access),
            current(container.begin()),
            end(container.end()) {}
        ~ElementIterator() override {};
        bool hasNext() override {return current!=end;};
        jobject next(JNIEnv * env) override {
            jobject obj = ::qtjambi_cast<jobject>(env, *current);
            ++current;
            return obj;
        }
        const void* next() override {
            const void* result = QtJambiPrivate::ContainerContentDeref<T>::deref(*current);
            ++current;
            return result;
        };
        const void* constNext() override {
            const void* result = &*current;
            ++current;
            return result;
        };
        bool isConst() override{
            return true;
        }
        void* mutableNext() override {
            return nullptr;
        }
        bool operator==(const AbstractSequentialAccess::ElementIterator& other) const override {
            return current==reinterpret_cast<const ElementIterator&>(other).current;
        }
        std::unique_ptr<AbstractSequentialAccess::ElementIterator> clone() const override {
            return std::unique_ptr<AbstractSequentialAccess::ElementIterator>(new ElementIterator(*this));
        }
        std::function<jobject(JNIEnv*,const void*)> elementConverter() const override {
            return [](JNIEnv* env,const void* pointer) -> jobject{
                return ::qtjambi_cast<jobject>(env, reinterpret_cast<const T*>(pointer));
            };
        }
    };
public:
    std::unique_ptr<AbstractSetAccess::ElementIterator> elementIterator(const void* container) override {
        return std::unique_ptr<AbstractSetAccess::ElementIterator>(new ElementIterator(this, *reinterpret_cast<const QSet<T>*>(container)));
    }
    std::unique_ptr<AbstractSetAccess::ElementIterator> elementIterator(void* container) override {
        return std::unique_ptr<AbstractSetAccess::ElementIterator>(new ElementIterator(this, *reinterpret_cast<QSet<T>*>(container)));
    }
};

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
template<typename T, std::size_t E = q20::dynamic_extent>
class QSpanAccess : public QtJambiPrivate::SequentialAccessSuperclassDecider<T,AbstractSpanAccess>::type{
    typedef typename QtJambiPrivate::SequentialAccessSuperclassDecider<T,AbstractSpanAccess>::type Super;
protected:
    QSpanAccess(){}
public:
    static AbstractSpanAccess* newInstance(){
        static QSpanAccess<T,E> instance;
        return &instance;
    }

    AbstractSpanAccess* clone() override{
        return this;
    }

    bool isConst() override{
        return std::is_const_v<T>;
    }

    const QMetaType& elementMetaType() override {
        static QMetaType type(QMetaType::fromType<std::remove_cv_t<T>>());
        return type;
    }

    AbstractContainerAccess::DataType elementType() override{
        return QtJambiPrivate::ContainerContentType<T>::type;
    }

    AbstractContainerAccess* elementNestedContainerAccess() override {
        return QtJambiPrivate::ContainerContentType<T>::accessFactory();
    }
    bool hasNestedContainerAccess() override {
        return QtJambiPrivate::ContainerContentType<T>::isContainer;
    }
    bool hasNestedPointers() override {
        return QtJambiPrivate::ContainerContentType<T>::isContainer && QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting;
    }

    void assign(void* container, const void* other) override {
        (*reinterpret_cast<QSpan<T,E>*>(container)) = (*reinterpret_cast<const QSpan<T,E>*>(other));
    }
    void assign(JNIEnv *env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override {
        (*reinterpret_cast<QSpan<T,E>*>(container.container)) = (*reinterpret_cast<const QSpan<T,E>*>(other.container));
        if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
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
        return sizeof(QSpan<T,E>);
    }
    size_t alignOf() const override {
        return alignof(QSpan<T,E>);
    }
    void* constructContainer(void* placement) override {
        return new(placement) QSpan<T,E>();
    }
    void* constructContainer(void* placement, const void* copyOf) override {
        return new(placement) QSpan<T,E>(*reinterpret_cast<const QSpan<T,E>*>(copyOf));
    }
    void* constructContainer(JNIEnv *, void* placement, const ConstContainerAndAccessInfo& copyOf) override {
        return constructContainer(placement, copyOf.container);
    }
    void* constructContainer(void* placement, void* move) override {
        return new(placement) QSpan<T,E>(std::move(*reinterpret_cast<const QSpan<T,E>*>(move)));
    }
    void* constructContainer(JNIEnv *, void* placement, const ContainerAndAccessInfo& move) override {
        return constructContainer(placement, move.container);
    }
    bool destructContainer(void* container) override {
        reinterpret_cast<QSpan<T,E>*>(container)->~QSpan<T,E>();
        return true;
    }

    QMetaType registerContainer(QByteArrayView containerTypeName) override {
        return QtJambiPrivate::container_registry_impl<QSpan<T,E>>::register_container(containerTypeName, this);
    }

    qsizetype size(JNIEnv *, const void* container) override {
        const QSpan<T,E> *span = static_cast<const QSpan<T,E> *>(container);
        return span->size();
    }

    qsizetype size(const void* container) override {
        const QSpan<T,E> *span = static_cast<const QSpan<T,E> *>(container);
        return span->size();
    }

    qsizetype size_bytes(JNIEnv *, const void* container) override {
        const QSpan<T,E> *span = static_cast<const QSpan<T,E> *>(container);
        return span->size_bytes();
    }

    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        const QSpan<T,E> *span = static_cast<const QSpan<T,E> *>(container.container);
        return ::qtjambi_cast<jobject>(env, container.nativeId, span->cbegin());
    }

    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        const QSpan<T,E> *span = static_cast<const QSpan<T,E> *>(container.container);
        return ::qtjambi_cast<jobject>(env, container.nativeId, span->cend());
    }

    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override {
        QSpan<T,E> *span = static_cast<QSpan<T,E> *>(container.container);
        return ::qtjambi_cast<jobject>(env, container.nativeId, span->begin());
    }

    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override {
        QSpan<T,E> *span = static_cast<QSpan<T,E> *>(container.container);
        return ::qtjambi_cast<jobject>(env, container.nativeId, span->end());
    }

    jobject get(JNIEnv * env, const void* container, qsizetype index) override {
        const QSpan<T,E> &span = *static_cast<const QSpan<T,E> *>(container);
        return ::qtjambi_cast<jobject>(env, span[index]);
    }

    const void* get(const void* container, qsizetype index) override {
        const QSpan<T,E> &span = *static_cast<const QSpan<T,E> *>(container);
        return &span[index];
    }

    bool set(JNIEnv * env, const ContainerInfo& container, qsizetype index, jobject value) override {
        if constexpr(std::is_const_v<T>){
            Q_UNUSED(container);
            Q_UNUSED(index);
            Q_UNUSED(value);
            Q_UNUSED(env);
            return false;
        }else{
            const QSpan<T,E> &span = *static_cast<const QSpan<T,E> *>(container.container);
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                if constexpr(QtJambiPrivate::ContainerContentType<T>::isContainer){
                    span[index] = ::qtjambi_cast<T>(env, value);
                    Super::updateRC(env, container);
                }else{
                    jobject oldValue = ::qtjambi_cast<jobject>(env, span[index]);
                    span[index] = ::qtjambi_cast<T>(env, value);
                    Super::removeRC(env, container.object, oldValue);
                    Super::addRC(env, container.object, value);
                }
            }else{
                span[index] = ::qtjambi_cast<T>(env, value);
            }
            return true;
        }
    }
    bool set(void* container, qsizetype index, const void* value) override {
        if constexpr(std::is_const_v<T>){
            Q_UNUSED(container);
            Q_UNUSED(index);
            Q_UNUSED(value);
            return false;
        }else{
            const QSpan<T,E> &span = *static_cast<const QSpan<T,E> *>(container);
            if constexpr(QtJambiPrivate::ContainerContentType<T>::needsReferenceCounting){
                if(JniEnvironment env{100}){
                    if(jobject object = QtJambiAPI::findObject(env, container)){
                        if constexpr(QtJambiPrivate::ContainerContentType<T>::isContainer){
                            span[index] = *static_cast<const T *>(value);
                            Super::updateRC(env, ContainerInfo{object, container});
                            return true;
                        }else{
                            const T& replace = *static_cast<const T *>(value);
                            jobject oldValue = ::qtjambi_cast<jobject>(env, span[index]);
                            jobject newValue = ::qtjambi_cast<jobject>(env, replace);
                            span[index] = replace;
                            Super::removeRC(env, object, oldValue);
                            Super::addRC(env, object, newValue);
                            return true;
                        }
                    }
                }
            }
            span[index] = *static_cast<const T *>(value);
            return true;
        }
    }
private:
    template<bool is_const>
    class ElementIterator : public AbstractSpanAccess::ElementIterator{
        using Container = std::conditional_t<is_const, const QSpan<T,E>, QSpan<T,E>>;
        using iterator = decltype(std::declval<Container>().begin());
        QSpanAccess<T,E>* m_access;
        iterator current;
        iterator end;
        ElementIterator(const ElementIterator& other)
            :m_access(other.m_access),
            current(other.current),
            end(other.end) {}
    protected:
        AbstractSequentialAccess* access() override { return m_access; }
    public:
        ElementIterator(QSpanAccess<T,E>* _access, Container& container)
            :m_access(_access),
            current(container.begin()),
            end(container.end()) {}
        ~ElementIterator() override {};
        bool hasNext() override {return current!=end;};
        jobject next(JNIEnv * env) override {
            jobject obj = ::qtjambi_cast<jobject>(env, *current);
            ++current;
            return obj;
        }
        const void* next() override {
            const void* result = QtJambiPrivate::ContainerContentDeref<T>::deref(*current);
            ++current;
            return result;
        };
        const void* constNext() override {
            const void* result = &*current;
            ++current;
            return result;
        };
        bool isConst() override{
            return is_const && m_access->isConst();
        }
        void* mutableNext() override {
            if constexpr(!is_const && !std::is_const_v<T>){
                void* result = &*current;
                ++current;
                return result;
            }else{
                return nullptr;
            }
        }
        bool operator==(const AbstractSequentialAccess::ElementIterator& other) const override {
            return current==reinterpret_cast<const ElementIterator&>(other).current;
        }
        std::unique_ptr<AbstractSequentialAccess::ElementIterator> clone() const override {
            return std::unique_ptr<AbstractSequentialAccess::ElementIterator>(new ElementIterator(*this));
        }
        std::function<jobject(JNIEnv*,const void*)> elementConverter() const override {
            return [](JNIEnv* env,const void* pointer) -> jobject{
                return ::qtjambi_cast<jobject>(env, reinterpret_cast<const T*>(pointer));
            };
        }
    };
public:
    std::unique_ptr<AbstractSpanAccess::ElementIterator> elementIterator(const void* container) override {
        return std::unique_ptr<AbstractSpanAccess::ElementIterator>(new ElementIterator<true>(this, *reinterpret_cast<const QSpan<T,E>*>(container)));
    }
    std::unique_ptr<AbstractSpanAccess::ElementIterator> elementIterator(void* container) override {
        return std::unique_ptr<AbstractSpanAccess::ElementIterator>(new ElementIterator<false>(this, *reinterpret_cast<QSpan<T,E>*>(container)));
    }
};

template<typename T>
AbstractSpanAccess* QListAccess<T>::createSpanAccess(bool isConst){
    if(isConst){
        return QSpanAccess<const T>::newInstance();
    }else{
        return QSpanAccess<T>::newInstance();
    }
}

#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)

namespace QtJambiPrivate {

template<typename T>
struct ContainerContentType<QList<T>>{
    static constexpr AbstractContainerAccess::DataType type = AbstractContainerAccess::Value;
    static constexpr bool isContainer = true;
    static constexpr bool needsReferenceCounting = (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask) || ContainerContentType<T>::needsReferenceCounting;
    static constexpr bool needsOwnerCheck = ContainerContentType<T>::needsOwnerCheck;
    static constexpr AbstractContainerAccess* accessFactory(){return QListAccess<T>::newInstance();};
};

template<typename T>
struct ContainerContentType<QSet<T>>{
    static constexpr AbstractContainerAccess::DataType type = AbstractContainerAccess::Value;
    static constexpr bool isContainer = true;
    static constexpr bool needsReferenceCounting = (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask) || ContainerContentType<T>::needsReferenceCounting;
    static constexpr bool needsOwnerCheck = ContainerContentType<T>::needsOwnerCheck;
    static constexpr AbstractContainerAccess* accessFactory(){return QSetAccess<T>::newInstance();};
};

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
template<typename T, std::size_t E>
struct ContainerContentType<QSpan<T,E>>{
    static constexpr AbstractContainerAccess::DataType type = AbstractContainerAccess::Value;
    static constexpr bool isContainer = true;
    static constexpr bool needsReferenceCounting = (QtJambiPrivate::ContainerContentType<T>::type & AbstractContainerAccess::PointersMask) || ContainerContentType<T>::needsReferenceCounting;
    static constexpr bool needsOwnerCheck = ContainerContentType<T>::needsOwnerCheck;
    static constexpr AbstractContainerAccess* accessFactory(){return QSpanAccess<T,E>::newInstance();};
};
#endif

} // namespace QtJambiPrivate

#endif // CONTAINERACCESS_SEQUENTIAL_H
