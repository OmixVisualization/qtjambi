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

#ifndef QTJAMBI_CAST_IMPL_CONTAINER_SEQUENTIAL_H
#define QTJAMBI_CAST_IMPL_CONTAINER_SEQUENTIAL_H

#include "qtjambi_cast_impl_util.h"
#include "qtjambi_cast_impl_container_iterator.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0) && defined(QLINKEDLIST_H)
QT_WARNING_DISABLE_GCC("-Wdeprecated-declarations")
QT_WARNING_DISABLE_DEPRECATED
#endif

namespace RegistryAPI{
template<typename T>
int registerMetaType(const QByteArray& typeName);
}

enum class QtJambiNativeID : jlong;

namespace QtJambiPrivate {

template<template<typename T> class Container, typename T>
struct IntermediateSequentialContainer : Container<T>{
    IntermediateSequentialContainer(JNIEnv *env, jobject object, QtJambiScope& scope) : Container<T>(), m_scope(scope), m_object(env->NewWeakGlobalRef(object)){}
    ~IntermediateSequentialContainer(){
        QTJAMBI_TRY_ANY{
            if(JniEnvironment env{200}){
                QTJAMBI_TRY{
                    jobject object = env->NewLocalRef(m_object);
                    if(!env->IsSameObject(object, nullptr)){
                        QtJambiAPI::clearJavaCollection(env, object);
                        for(typename Container<T>::const_iterator i = Container<T>::constBegin(); i!=Container<T>::constEnd(); ++i){
                            QtJambiAPI::addToJavaCollection(env, object, qtjambi_scoped_cast<true,jobject,decltype(*i)>::cast(env, *i, nullptr, &m_scope));
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

typedef bool (*IsContainerFunction)(JNIEnv *, jobject, const std::type_info&, const QMetaType&, void*& pointer);

template<typename Iterator>
class QSequentialConstIteratorAccess : public AbstractConstIteratorAccess<Iterator>{
protected:
    QSequentialConstIteratorAccess(){}
public:
    static QSequentialConstIteratorAccess<Iterator>* newInstance(){
        static QSequentialConstIteratorAccess<Iterator> instance;
        return &instance;
    }

    AbstractSequentialConstIteratorAccess* clone() override{
        return this;
    }

    void dispose() override {}

    jobject value(JNIEnv * env, const void* ptr) override {
        const Iterator* iterator = static_cast<const Iterator*>(ptr);
        const auto& value = *(*iterator);
        return qtjambi_scoped_cast<false,jobject,decltype(value)>::cast(env, value, nullptr, nullptr);
    }
};

template<typename Iterator>
class QSequentialIteratorAccess : public virtual QSequentialConstIteratorAccess<Iterator>, public virtual AbstractSequentialIteratorAccess{
private:
    QSequentialIteratorAccess(){}
public:
    static QSequentialConstIteratorAccess<Iterator>* newInstance(){
        static QSequentialIteratorAccess<Iterator> instance;
        return &instance;
    }

    AbstractConstIteratorAccess<Iterator>* clone() override{
        return this;
    }

    void dispose() override {}

    jobject value(JNIEnv * env, const void* ptr) override {
        return QSequentialConstIteratorAccess<Iterator>::value(env, ptr);
    }

    void increment(JNIEnv *env, void* iterator) override {
        QSequentialConstIteratorAccess<Iterator>::increment(env, iterator);
    }
    void decrement(JNIEnv *env, void* iterator) override {
        QSequentialConstIteratorAccess<Iterator>::decrement(env, iterator);
    }
    jboolean lessThan(JNIEnv *env, const void* iterator, const void* other) override {
        return QSequentialConstIteratorAccess<Iterator>::lessThan(env, iterator, other);
    }
    bool canLess() override {
        return QSequentialConstIteratorAccess<Iterator>::canLess();
    }
    jboolean equals(JNIEnv *env, const void* ptr, const void* ptr2) override {
        return QSequentialConstIteratorAccess<Iterator>::equals(env, ptr, ptr2);
    }

    void setValue(JNIEnv * env, void* ptr, jobject newValue) override {
        Iterator* iterator = static_cast<Iterator*>(ptr);
        *(*iterator) = qtjambi_scoped_cast<false,typename std::remove_reference<decltype(*(*iterator))>::type,jobject>::cast(env, newValue, nullptr, nullptr);
    }
};

template<typename Iterator> struct supports_mutableIterator : std::conditional<std::is_reference<decltype(*std::declval<Iterator>())>::value
                                                                && !std::is_const<typename std::remove_reference<decltype(*std::declval<Iterator>())>::type>::value, std::true_type, std::false_type>::type{};

template<typename Iterator, bool isMutable = supports_mutableIterator<Iterator>::value>
struct qtjambi_sequential_iterator_caster{
    static jobject cast(JNIEnv * env, QtJambiNativeID __list_nativeId, typename std::conditional<std::is_pointer<Iterator>::value, Iterator, const Iterator&>::type iter){
        return QtJambiAPI::convertQSequentialIteratorToJavaObject(env, __list_nativeId,
            new Iterator(iter),
            [](void* ptr,bool) {
                Iterator* iterator = static_cast<Iterator*>(ptr);
                delete iterator;
            },
            QSequentialConstIteratorAccess<Iterator>::newInstance()
        );
    }
};

template<typename Iterator>
struct qtjambi_sequential_iterator_caster<Iterator,true>{
    static jobject cast(JNIEnv * env, QtJambiNativeID __list_nativeId, typename std::conditional<std::is_pointer<Iterator>::value, Iterator, const Iterator&>::type iter){
        return QtJambiAPI::convertQSequentialIteratorToJavaObject(env, __list_nativeId,
            new Iterator(iter),
            [](void* ptr,bool) {
                Iterator* iterator = static_cast<Iterator*>(ptr);
                delete iterator;
            },
            QSequentialIteratorAccess<Iterator>::newInstance()
        );
    }
};

template<template<typename T> class Container, typename T, bool isPointer, bool = is_copy_constructible<T>::value && is_default_constructible<T>::value && is_copy_constructible<Container<T>>::value>
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


#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
template<template<typename T> class Container, typename T>
struct ContainerToList{
    static jobject function(JNIEnv * env, const void* ptr) {
        const Container<T>  *container = static_cast<const Container<T> *>(ptr);
        return qtjambi_scoped_cast<false,jobject,const QList<T>>::cast(env, container->toList(), nullptr, nullptr);
    }
};
#endif

template<template<typename T> class Container, typename T, bool = is_default_constructible<T>::value>
struct ContainerAt{
    static jobject function(JNIEnv * env, const void*, jint) {
        JavaException::raiseUnsupportedOperationException(env, "at(i)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename T> class Container, typename T>
struct ContainerAt<Container,T,true>{
    static jobject function(JNIEnv * env, const void* ptr, jint idx) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return qtjambi_scoped_cast<false,jobject,const T>::cast(env, container->at(int(idx)), nullptr, nullptr);
    }
};

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
template<template<typename T> class Container, typename T>
struct ContainerFirst{
    static jobject function(JNIEnv * env, const void* ptr) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return qtjambi_scoped_cast<false,jobject,const T>::cast(env, container->first(), nullptr, nullptr);
    }
};

template<template<typename T> class Container, typename T>
struct ContainerLast{
    static jobject function(JNIEnv * env, const void* ptr) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return qtjambi_scoped_cast<false,jobject,const T>::cast(env, container->last(), nullptr, nullptr);
    }
};
#endif

template<template<typename T> class Container, typename T>
struct ContainerCapacity{
    static jint function(JNIEnv *env, const void* ptr) {
        Q_UNUSED(env)
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return jint(container->capacity());
    }
};

template<template<typename T> class Container, typename T, bool = supports_equal<T>::value>
struct ContainerContains{
    static jboolean function(JNIEnv * env, const void* ptr, jobject object) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return container->contains(qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr));
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
            if (!isContainer(env, other, qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(__qt_other_pointer))) {
                __qt_scoped_pointer.reset(new Container<T> ());
                __qt_other_pointer = __qt_scoped_pointer.get();
                jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, other);
                while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                    jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                    (*__qt_other_pointer) << qtjambi_scoped_cast<false,T,jobject>::cast(env, element, nullptr, nullptr);
                }
            }
        }else{
            __qt_scoped_pointer.reset(new Container<T> ());
            __qt_other_pointer = __qt_scoped_pointer.get();
        }
        return container->intersects(*__qt_other_pointer);
    }
};

template<template<typename T> class Container, typename T, IsContainerFunction isContainer>
struct ContainerIntersect{
    static void function(JNIEnv * env, void* ptr, jobject other) {
        Container<T> *container = static_cast<Container<T> *>(ptr);
        std::unique_ptr<Container<T> > __qt_scoped_pointer;
        Container<T> *__qt_other_pointer = nullptr;
        if (other!= nullptr) {
            if (!isContainer(env, other, qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(__qt_other_pointer))) {
                __qt_scoped_pointer.reset(new Container<T> ());
                __qt_other_pointer = __qt_scoped_pointer.get();
                jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, other);
                while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                    jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                    (*__qt_other_pointer) << qtjambi_scoped_cast<false,T,jobject>::cast(env, element, nullptr, nullptr);
                }
            }
        }else{
            __qt_scoped_pointer.reset(new Container<T> ());
            __qt_other_pointer = __qt_scoped_pointer.get();
        }
        container->intersect(*__qt_other_pointer);
    }
};

template<template<typename T> class Container, typename T, IsContainerFunction isContainer>
struct ContainerUnite{
    static void function(JNIEnv * env, void* ptr, jobject other) {
        Container<T> *container = static_cast<Container<T> *>(ptr);
        std::unique_ptr<Container<T> > __qt_scoped_pointer;
        Container<T> *__qt_other_pointer = nullptr;
        if (other!= nullptr) {
            if (!isContainer(env, other, qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(__qt_other_pointer))) {
                __qt_scoped_pointer.reset(new Container<T> ());
                __qt_other_pointer = __qt_scoped_pointer.get();
                jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, other);
                while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                    jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                    (*__qt_other_pointer) << qtjambi_scoped_cast<false,T,jobject>::cast(env, element, nullptr, nullptr);
                }
            }
        }else{
            __qt_scoped_pointer.reset(new Container<T> ());
            __qt_other_pointer = __qt_scoped_pointer.get();
        }
        container->unite(*__qt_other_pointer);
    }
};

template<template<typename T> class Container, typename T, IsContainerFunction isContainer>
struct ContainerSubtract{
    static void function(JNIEnv * env, void* ptr, jobject other) {
        Container<T> *container = static_cast<Container<T> *>(ptr);
        std::unique_ptr<Container<T> > __qt_scoped_pointer;
        Container<T> *__qt_other_pointer = nullptr;
        if (other!= nullptr) {
            if (!isContainer(env, other, qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(__qt_other_pointer))) {
                __qt_scoped_pointer.reset(new Container<T> ());
                __qt_other_pointer = __qt_scoped_pointer.get();
                jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, other);
                while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                    jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                    (*__qt_other_pointer) << qtjambi_scoped_cast<false,T,jobject>::cast(env, element, nullptr, nullptr);
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
    static jint function(JNIEnv * env, const void*, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "count(value)" QTJAMBI_STACKTRACEINFO );
        return 0;
    }
};

template<template<typename T> class Container, typename T>
struct ContainerCountObject<Container, T, true>{
    static jint function(JNIEnv * env, const void* ptr, jobject object) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return container->count(qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr));
    }
};

template<template<typename T> class Container, typename T>
struct ContainerBegin{
    static jobject function(JNIEnv *env, QtJambiNativeID __list_nativeId, void* ptr) {
        Container<T> *container = static_cast<Container<T> *>(ptr);
        return qtjambi_sequential_iterator_caster<decltype(container->begin())>::cast(env, __list_nativeId, container->begin());
    }
};

template<template<typename T> class Container, typename T>
struct ContainerEnd{
    static jobject function(JNIEnv *env, QtJambiNativeID __list_nativeId, void* ptr) {
        Container<T> *container = static_cast<Container<T> *>(ptr);
        return qtjambi_sequential_iterator_caster<decltype(container->end())>::cast(env, __list_nativeId, container->end());
    }
};

template<template<typename T> class Container, typename T>
struct ContainerConstBegin{
    static jobject function(JNIEnv *env, QtJambiNativeID __list_nativeId, const void* ptr) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return qtjambi_sequential_iterator_caster<decltype(container->constBegin())>::cast(env, __list_nativeId, container->constBegin());
    }
};

template<template<typename T> class Container, typename T>
struct ContainerConstEnd{
    static jobject function(JNIEnv *env, QtJambiNativeID __list_nativeId, const void* ptr) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return qtjambi_sequential_iterator_caster<decltype(container->constEnd())>::cast(env, __list_nativeId, container->constEnd());
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
        return container->endsWith(qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr));
    }
};

template<template<typename T> class Container, typename T, bool = supports_equal<T>::value>
struct ContainerIndexOf{
    static jint function(JNIEnv * env, const void*, jobject, jint) {
        JavaException::raiseUnsupportedOperationException(env, "indexOf(value, index)" QTJAMBI_STACKTRACEINFO );
        return 0;
    }
};

template<template<typename T> class Container, typename T>
struct ContainerIndexOf<Container, T, true>{
    static jint function(JNIEnv * env, const void* ptr, jobject object, jint idx) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return container->indexOf(qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr), int(idx));
    }
};

template<template<typename T> class Container, typename T, bool = supports_equal<T>::value>
struct ContainerLastIndexOf{
    static jint function(JNIEnv * env, const void*, jobject, jint) {
        JavaException::raiseUnsupportedOperationException(env, "lastIndexOf(value,index)" QTJAMBI_STACKTRACEINFO );
        return 0;
    }
};

template<template<typename T> class Container, typename T>
struct ContainerLastIndexOf<Container, T, true>{
    static jint function(JNIEnv * env, const void* ptr, jobject object, jint idx) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return container->lastIndexOf(qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr), int(idx));
    }
};

template<template<typename T> class Container, typename T, bool = is_default_constructible<T>::value>
struct ContainerMid{
    static jobject function(JNIEnv * env, const void*, jint, jint) {
        JavaException::raiseUnsupportedOperationException(env, "mid(index1, index2)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename T> class Container, typename T>
struct ContainerMid<Container, T, true>{
    static jobject function(JNIEnv * env, const void* ptr, jint idx1, jint idx2) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return qtjambi_scoped_cast<false,jobject,typename std::add_const<decltype(container->mid(int(idx1), int(idx2)))>::type>::cast(env, container->mid(int(idx1), int(idx2)), nullptr, nullptr);
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
            if (!is_Container1_fct(env, other, qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(__qt_other_pointer))) {
                __qt_scoped_pointer.reset(new Container<T> ());
                __qt_other_pointer = __qt_scoped_pointer.get();
                jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, other);
                while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                    jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                    (*__qt_other_pointer) << qtjambi_scoped_cast<false,T,jobject>::cast(env, element, nullptr, nullptr);
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
    static jint function(JNIEnv * env, const void* ptr) {
        Q_UNUSED(env)
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
        return container->startsWith(qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr));
    }
};

template<template<typename T> class Container, typename T, bool = is_default_constructible<T>::value>
struct ContainerValue{
    static jobject function(JNIEnv * env, const void*, jint) {
        JavaException::raiseUnsupportedOperationException(env, "value(i)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename T> class Container, typename T>
struct ContainerValue<Container, T, true>{
    static jobject function(JNIEnv * env, const void* ptr, jint index) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return qtjambi_scoped_cast<false,jobject,const T>::cast(env, container->value(int(index)), nullptr, nullptr);
    }
};

template<template<typename T> class Container, typename T, bool = is_copy_constructible<T>::value || std::is_move_constructible<T>::value>
struct ContainerValueDefault{
    static jobject function(JNIEnv * env, const void*, jint, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "value(i,defaultValue)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename T> class Container, typename T>
struct ContainerValueDefault<Container, T, true>{
    static jobject function(JNIEnv * env, const void* ptr, jint index, jobject object) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return qtjambi_scoped_cast<false,jobject,const T>::cast(env, container->value(index, qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr)), nullptr, nullptr);
    }
};

template<template<typename T> class Container, typename T, bool = is_copy_constructible<T>::value || std::is_move_constructible<T>::value>
struct ContainerAppend{
    static void function(JNIEnv * env, void*, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "append(value)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename T> class Container, typename T>
struct ContainerAppend<Container, T, true>{
    static void function(JNIEnv * env, void* ptr, jobject object) {
        Container<T> *container = static_cast<Container<T> *>(ptr);
        container->append(qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr));
    }
};

template<template<typename T> class Container, typename T, bool = is_default_constructible<T>::value>
struct ContainerFill{
    static void function(JNIEnv * env, void*, jobject, jint) {
        JavaException::raiseUnsupportedOperationException(env, "fill(value,size)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename T> class Container, typename T>
struct ContainerFill<Container, T, true>{
    static void function(JNIEnv * env, void* ptr, jobject object, jint size) {
        Container<T> *container = static_cast<Container<T> *>(ptr);
        container->fill(qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr), size);
    }
};

template<typename T, bool = is_copy_constructible<T>::value || std::is_move_constructible<T>::value>
struct ContainerAppendList{
    static void function(JNIEnv * env, void*, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "QList::append(list)" QTJAMBI_STACKTRACEINFO );
    }
};

template<typename T>
struct ContainerAppendList<T, true>{
    static void function(JNIEnv * env, void* ptr, jobject object) {
        QList<T> *container = static_cast<QList<T> *>(ptr);
        std::unique_ptr<QList<T> > __qt_scoped_pointer;
        QList<T> *__qt_object_pointer = nullptr;
        if (object!= nullptr) {
            if (!ContainerAPI::getAsQList<T>(env, object, __qt_object_pointer)) {
                __qt_scoped_pointer.reset(new QList<T>());
                __qt_object_pointer = __qt_scoped_pointer.get();
                jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, object);
                while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                    jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                    __qt_object_pointer->append(qtjambi_scoped_cast<false,T,jobject>::cast(env, element, nullptr, nullptr));
                }
            }
        }else{
            __qt_scoped_pointer.reset(new QList<T> ());
            __qt_object_pointer = __qt_scoped_pointer.get();
        }
        container->append(*__qt_object_pointer);
    }
};

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
template<typename T, bool = is_copy_constructible<T>::value || std::is_move_constructible<T>::value>
struct ContainerAppendVector{
    static void function(JNIEnv * env, void*, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "QVector::append(vector)" QTJAMBI_STACKTRACEINFO );
    }
};

template<typename T>
struct ContainerAppendVector<T, true>{
    static void function(JNIEnv * env, void* ptr, jobject object) {
        QVector<T> *container = static_cast<QVector<T> *>(ptr);
        std::unique_ptr<QVector<T> > __qt_scoped_pointer;
        QVector<T> *__qt_object_pointer = nullptr;
        if (object!= nullptr) {
            if (!ContainerAPI::getAsQVector(env, object, qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(__qt_object_pointer))) {
                __qt_scoped_pointer.reset(new QVector<T>());
                __qt_object_pointer = __qt_scoped_pointer.get();
                jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, object);
                while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                    jobject element = QtJambiAPI::nextOfJavaIterator(env, iterator);
                    __qt_object_pointer->append(qtjambi_scoped_cast<false,T,jobject>::cast(env, element, nullptr, nullptr));
                }
            }
        }else{
            __qt_scoped_pointer.reset(new QVector<T> ());
            __qt_object_pointer = __qt_scoped_pointer.get();
        }
        container->append(*__qt_object_pointer);
    }
};
#endif

template<template<typename T> class Container, typename T>
struct ContainerClear{
    static void function(JNIEnv *, void* ptr) {
        Container<T> *container = static_cast<Container<T> *>(ptr);
        container->clear();
    }
};

template<template<typename T> class Container, typename T, bool = is_copy_constructible<T>::value && is_default_constructible<T>::value>
struct ContainerInsertAt{
    static void function(JNIEnv * env, void*, jint, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "insert(i,value)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename T> class Container, typename T>
struct ContainerInsertAt<Container, T, true>{
    static void function(JNIEnv * env, void* ptr, jint i, jobject value) {
        Container<T> *container = static_cast<Container<T> *>(ptr);
        container->insert(i, qtjambi_scoped_cast<false,T,jobject>::cast(env, value, nullptr, nullptr));
    }
};

template<template<typename T> class Container, typename T, bool = is_copy_constructible<T>::value && is_default_constructible<T>::value>
struct ContainerInsertN{
    static void function(JNIEnv * env, void*, jint, jint, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "insert(i,n,value)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename T> class Container, typename T>
struct ContainerInsertN<Container, T, true>{
    static void function(JNIEnv * env, void* ptr, jint i, jint n, jobject value) {
        Container<T> *container = static_cast<Container<T> *>(ptr);
        container->insert(i, n, qtjambi_scoped_cast<false,T,jobject>::cast(env, value, nullptr, nullptr));
    }
};

template<template<typename T> class Container, typename T>
struct ContainerInsert{
    static void function(JNIEnv * env, void* ptr, jobject value) {
        Container<T> *container = static_cast<Container<T> *>(ptr);
        container->insert(qtjambi_scoped_cast<false,T,jobject>::cast(env, value, nullptr, nullptr));
    }
};

template<template<typename T> class Container, typename T, bool = is_copy_constructible<T>::value && is_default_constructible<T>::value>
struct ContainerMove{
    static void function(JNIEnv * env, void*, jint, jint) {
        JavaException::raiseUnsupportedOperationException(env, "move(index1, index2)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename T> class Container, typename T>
struct ContainerMove<Container, T, true>{
    static void function(JNIEnv * env, void* ptr, jint idx1, jint idx2) {
        Q_UNUSED(env)
        Container<T> *container = static_cast<Container<T> *>(ptr);
        container->move(int(idx1), int(idx2));
    }
};

template<template<typename T> class Container, typename T, bool = is_copy_constructible<T>::value || std::is_move_constructible<T>::value>
struct ContainerPrepend{
    static void function(JNIEnv * env, void*, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "prepend(value)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename T> class Container, typename T>
struct ContainerPrepend<Container, T, true>{
    static void function(JNIEnv * env, void* ptr, jobject object) {
        Container<T> *container = static_cast<Container<T> *>(ptr);
        container->prepend(qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr));
    }
};

template<template<typename T> class Container, typename T, bool = supports_equal<T>::value>
struct ContainerRemoveAll{
    static jint function(JNIEnv * env, void*, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "removeAll(value)" QTJAMBI_STACKTRACEINFO );
        return 0;
    }
};

template<template<typename T> class Container, typename T>
struct ContainerRemoveAll<Container, T, true>{
    static jint function(JNIEnv * env, void* ptr, jobject object) {
        Container<T> *container = static_cast<Container<T> *>(ptr);
        return container->removeAll(qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr));
    }
};

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
template<template<typename T> class Container, typename T>
struct ContainerTakeFirst{
    static jobject function(JNIEnv * env, void* ptr) {
        Container<T> *container = static_cast<Container<T> *>(ptr);
        return qtjambi_scoped_cast<false,jobject,const T>::cast(env, container->takeFirst(), nullptr, nullptr);
    }
};

template<template<typename T> class Container, typename T>
struct ContainerTakeLast{
    static jobject function(JNIEnv * env, void* ptr) {
        Container<T> *container = static_cast<Container<T> *>(ptr);
        return qtjambi_scoped_cast<false,jobject,const T>::cast(env, container->takeLast(), nullptr, nullptr);
    }
};

template<template<typename T> class Container, typename T>
struct ContainerRemoveFirst{
    static void function(JNIEnv * env, void* ptr) {
        Q_UNUSED(env)
        Container<T> *container = static_cast<Container<T> *>(ptr);
        container->removeFirst();
    }
};

template<template<typename T> class Container, typename T>
struct ContainerRemoveLast{
    static void function(JNIEnv * env, void* ptr) {
        Q_UNUSED(env)
        Container<T> *container = static_cast<Container<T> *>(ptr);
        container->removeLast();
    }
};
#endif

template<template<typename T> class Container, typename T>
struct ContainerRemoveAt{
    static void function(JNIEnv * env, void* ptr, jint idx) {
        Q_UNUSED(env)
        Container<T> *container = static_cast<Container<T> *>(ptr);
        container->removeAt(idx);
    }
};

template<template<typename T> class Container, typename T, bool = is_default_constructible<T>::value>
struct ContainerRemoveI{
    static void function(JNIEnv * env, void*, jint) {
        JavaException::raiseUnsupportedOperationException(env, "remove(index)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename T> class Container, typename T>
struct ContainerRemoveI<Container, T, true>{
    static void function(JNIEnv * env, void* ptr, jint idx) {
        Q_UNUSED(env)
        Container<T> *container = static_cast<Container<T> *>(ptr);
        container->remove(idx);
    }
};

template<template<typename T> class Container, typename T, bool = is_default_constructible<T>::value>
struct ContainerRemoveN{
    static void function(JNIEnv * env, void*, jint, jint) {
        JavaException::raiseUnsupportedOperationException(env, "remove(index,n)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename T> class Container, typename T>
struct ContainerRemoveN<Container, T, true>{
    static void function(JNIEnv * env, void* ptr, jint idx, jint n) {
        Q_UNUSED(env)
        Container<T> *container = static_cast<Container<T> *>(ptr);
        container->remove(idx, n);
    }
};

template<template<typename T> class Container, typename T, bool = supports_equal<T>::value>
struct ContainerRemoveOne{
    static jboolean function(JNIEnv * env, void*, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "removeOne(value)" QTJAMBI_STACKTRACEINFO );
        return false;
    }
};

template<template<typename T> class Container, typename T>
struct ContainerRemoveOne<Container, T, true>{
    static jboolean function(JNIEnv * env, void* ptr, jobject object) {
        Container<T> *container = static_cast<Container<T> *>(ptr);
        return container->removeOne(qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr));
    }
};

template<template<typename T> class Container, typename T, bool = supports_equal<T>::value>
struct ContainerRemove{
    static jboolean function(JNIEnv * env, void*, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "remove(value)" QTJAMBI_STACKTRACEINFO );
        return false;
    }
};

template<template<typename T> class Container, typename T>
struct ContainerRemove<Container, T, true>{
    static jboolean function(JNIEnv * env, void* ptr, jobject object) {
        Container<T> *container = static_cast<Container<T> *>(ptr);
        return container->remove(qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr));
    }
};

template<template<typename T> class Container, typename T, bool = is_default_constructible<T>::value>
struct ContainerReplace{
    static void function(JNIEnv * env, void*, jint,jobject) {
        JavaException::raiseUnsupportedOperationException(env, "replace(index,value)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename T> class Container, typename T>
struct ContainerReplace<Container, T, true>{
    static void function(JNIEnv * env, void* ptr, jint idx,jobject newObject) {
        Container<T> *container = static_cast<Container<T> *>(ptr);
        container->replace(idx, qtjambi_scoped_cast<false,T,jobject>::cast(env, newObject, nullptr, nullptr));
    }
};

template<template<typename T> class Container, typename T, bool = is_default_constructible<T>::value>
struct ContainerReserve{
    static void function(JNIEnv * env, void*, jint) {
        JavaException::raiseUnsupportedOperationException(env, "reserve(size)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename T> class Container, typename T>
struct ContainerReserve<Container, T, true>{
    static void function(JNIEnv * env, void* ptr, jint size) {
        Q_UNUSED(env)
        Container<T> *container = static_cast<Container<T> *>(ptr);
        container->reserve(size);
    }
};

template<template<typename T> class Container, typename T, bool = is_default_constructible<T>::value>
struct ContainerResize{
    static void function(JNIEnv * env, void*, jint) {
        JavaException::raiseUnsupportedOperationException(env, "resize(size)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename T> class Container, typename T>
struct ContainerResize<Container, T, true>{
    static void function(JNIEnv * env, void* ptr, jint size) {
        Q_UNUSED(env)
        Container<T> *container = static_cast<Container<T> *>(ptr);
        container->resize(size);
    }
};

template<template<typename T> class Container, typename T, bool = is_default_constructible<T>::value>
struct ContainerSqueeze{
    static void function(JNIEnv * env, void*) {
        JavaException::raiseUnsupportedOperationException(env, "squeeze()" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename T> class Container, typename T>
struct ContainerSqueeze<Container, T, true>{
    static void function(JNIEnv * env, void* ptr) {
        Q_UNUSED(env)
        Container<T> *container = static_cast<Container<T> *>(ptr);
        container->squeeze();
    }
};

template<template<typename T> class Container, typename T>
struct ContainerSwap{
    static void function(JNIEnv * env, void* ptr, jint idx1, jint idx2) {
        Q_UNUSED(env)
        Container<T> *container = static_cast<Container<T> *>(ptr);
#if QT_VERSION < QT_VERSION_CHECK(5,13,0)
        container->swap(int(idx1), int(idx2));
#else
        container->swapItemsAt(int(idx1), int(idx2));
#endif
    }
};

template<template<typename T> class Container, typename T>
struct ContainerValues{
    static jobject function(JNIEnv * env, const void* ptr) {
        const Container<T>  *container = static_cast<const Container<T> *>(ptr);
        return qtjambi_scoped_cast<false,jobject,const QList<T>>::cast(env, container->values(), nullptr, nullptr);
    }
};

template<typename T, bool isConst = false>
class QListAccess : public AbstractListAccess{
protected:
    QListAccess(){}
public:
    static AbstractListAccess* newInstance(){
        static QListAccess<T, isConst> instance;
        return &instance;
    }

    AbstractListAccess* clone() override{
        return this;
    }

    bool isConstant() override {return true;}
    const QMetaType& elementMetaType() override {static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(T)); return type;}
    size_t sizeOf() override {
        return sizeof(QList<T>);
    }
    void* constructContainer(void* placement, const void* copyOf = nullptr) override {
        if(copyOf){
            return new(placement) QList<T>(*reinterpret_cast<const QList<T>*>(copyOf));
        }else{
            return new(placement) QList<T>();
        }
    }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* move) override {
        return new(placement) QList<T>(std::move(*reinterpret_cast<const QList<T>*>(move)));
    }
#endif
    bool destructContainer(void* container) override {
        reinterpret_cast<QList<T>*>(container)->~QList<T>();
        return true;
    }
    void assign(void* container, const void* other) override { (*reinterpret_cast<QList<T>*>(container)) = (*reinterpret_cast<const QList<T>*>(other)); }
    int registerContainer(const QByteArray& containerTypeName) override {
        return RegistryAPI::registerMetaType<QList<T>>(containerTypeName);
    }

    void appendList(JNIEnv * env, void*, jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QList::append" QTJAMBI_STACKTRACEINFO );
    }

    jobject at(JNIEnv * env, const void* container, jint index) override {
        return ContainerAt<QList, T>::function(env, container, index);
    }

    jobject value(JNIEnv * env, const void* container, jint index) override {
        return ContainerValue<QList, T>::function(env, container, index);
    }

    jobject value(JNIEnv * env, const void* container, jint index, jobject defaultValue) override {
        return ContainerValueDefault<QList, T>::function(env, container, index, defaultValue);
    }

    void swapItemsAt(JNIEnv * env, void*, jint, jint) override {
        JavaException::raiseUnsupportedOperationException(env, "QList::swapItemsAt" QTJAMBI_STACKTRACEINFO );
    }

    jboolean startsWith(JNIEnv * env, const void* container, jobject value) override {
        return ContainerStartsWith<QList, T>::function(env, container, value);
    }

    jint size(JNIEnv * env, const void* container) override {
        return ContainerSize<QList, T>::function(env, container);
    }

    void reserve(JNIEnv * env, void*, jint) override {
        JavaException::raiseUnsupportedOperationException(env, "QList::reserve" QTJAMBI_STACKTRACEINFO );
    }

    void replace(JNIEnv * env, void*, jint, jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QList::replace" QTJAMBI_STACKTRACEINFO );
    }

    void remove(JNIEnv * env, void*, jint, jint n) override {
        if(n==1)
            JavaException::raiseUnsupportedOperationException(env, "QList::removeAt" QTJAMBI_STACKTRACEINFO );
        else
            JavaException::raiseUnsupportedOperationException(env, "QList::remove" QTJAMBI_STACKTRACEINFO );
    }

    jint removeAll(JNIEnv * env, void*, jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QList::removeAll" QTJAMBI_STACKTRACEINFO );
        return 0;
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        return ContainerEquals<QList, T, ContainerAPI::getAsQList>::function(env, container, other);
    }

    void move(JNIEnv * env, void*, jint, jint) override {
        JavaException::raiseUnsupportedOperationException(env, "QList::move" QTJAMBI_STACKTRACEINFO );
    }

    jobject mid(JNIEnv * env, const void* container, jint index1, jint index2) override {
        return ContainerMid<QList, T>::function(env, container, index1, index2);
    }

    jint lastIndexOf(JNIEnv * env, const void* container, jobject value, jint index) override {
        return ContainerLastIndexOf<QList, T>::function(env, container, value, index);
    }

    void insert(JNIEnv * env, void*, jint, jint, jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QList::insert" QTJAMBI_STACKTRACEINFO );
    }

    jint indexOf(JNIEnv * env, const void* container, jobject value, jint index) override {
        return ContainerIndexOf<QList, T>::function(env, container, value, index);
    }

    jboolean endsWith(JNIEnv * env, const void* container, jobject value) override {
        return ContainerEndsWith<QList, T>::function(env, container, value);
    }

    jint count(JNIEnv * env, const void* container, jobject value) override {
        return ContainerCountObject<QList, T>::function(env, container, value);
    }

    jboolean contains(JNIEnv * env, const void* container, jobject value) override {
        return ContainerContains<QList, T>::function(env, container, value);
    }

    void clear(JNIEnv * env, void*) override {
        JavaException::raiseUnsupportedOperationException(env, "QList::clear" QTJAMBI_STACKTRACEINFO );
    }

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    jint capacity(JNIEnv * env, const void* container) override {
        return ContainerCapacity<QList, T>::function(env, container);
    }

    void fill(JNIEnv * env, void*, jobject, jint) override {
        JavaException::raiseUnsupportedOperationException(env, "QList::fill" QTJAMBI_STACKTRACEINFO );
    }

    void resize(JNIEnv * env, void*, jint) override {
        JavaException::raiseUnsupportedOperationException(env, "QList::resize" QTJAMBI_STACKTRACEINFO );
    }
    void squeeze(JNIEnv * env, void*) override {
        JavaException::raiseUnsupportedOperationException(env, "QList::squeeze" QTJAMBI_STACKTRACEINFO );
    }
#endif

    jobject begin(JNIEnv * env, QtJambiNativeID, void*) override {
        JavaException::raiseUnsupportedOperationException(env, "QList::begin" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }

    jobject end(JNIEnv * env, QtJambiNativeID, void*) override {
        JavaException::raiseUnsupportedOperationException(env, "QList::end" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }

    jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        return ContainerConstBegin<QList, T>::function(env, ownerId, container);
    }

    jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        return ContainerConstEnd<QList, T>::function(env, ownerId, container);
    }

    void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) override {
        for(typename pointer_from<T>::In element : *reinterpret_cast<const QList<T>*>(container)){
            if(!analyzer(pointer_from<T>::from(element), data)){
                break;
            }
        }
    }
};

template<typename T>
class QListAccess<T, false> : public QListAccess<T, true>{
protected:
    QListAccess(){}
public:
    static AbstractListAccess* newInstance(){
        static QListAccess<T, false> instance;
        return &instance;
    }

    AbstractListAccess* clone() override{
        return this;
    }

    bool isConstant() override {return false;}

    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) override {
        return ContainerBegin<QList, T>::function(env, ownerId, container);
    }

    jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) override {
        return ContainerEnd<QList, T>::function(env, ownerId, container);
    }

    void appendList(JNIEnv * env, void* container, jobject list) override {
        ContainerAppendList<T>::function(env, container, list);
    }

    void swapItemsAt(JNIEnv * env, void* container, jint index1, jint index2) override {
        ContainerSwap<QList, T>::function(env, container, index1, index2);
    }

    void reserve(JNIEnv * env, void* container, jint size) override {
        ContainerReserve<QList, T>::function(env, container, size);
    }

    void replace(JNIEnv * env, void* container, jint index, jobject value) override {
        ContainerReplace<QList, T>::function(env, container, index, value);
    }

    void remove(JNIEnv * env, void* container, jint index, jint n) override {
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        ContainerRemoveN<QList, T>::function(env, container, index, n);
#else
        Q_ASSERT(n==1);
        ContainerRemoveAt<QList, T>::function(env, container, index);
#endif
    }

    jint removeAll(JNIEnv * env, void* container, jobject value) override {
        return ContainerRemoveAll<QList, T>::function(env, container, value);
    }

    void move(JNIEnv * env, void* container, jint index1, jint index2) override {
        ContainerMove<QList, T>::function(env, container, index1, index2);
    }

    void insert(JNIEnv * env, void* container, jint index, jint n, jobject value) override {
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        ContainerInsertN<QList, T>::function(env, container, index, n, value);
#else
        Q_ASSERT(n==1);
        ContainerInsertAt<QList, T>::function(env, container, index, value);
#endif
    }

    void clear(JNIEnv * env, void* container) override {
        ContainerClear<QList, T>::function(env, container);
    }

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    void fill(JNIEnv * env, void* container, jobject value, jint size) override {
        ContainerFill<QList, T>::function(env, container, value, size);
    }
    void resize(JNIEnv * env, void* container, jint newSize) override {
        ContainerResize<QList, T>::function(env, container, newSize);
    }
    void squeeze(JNIEnv * env, void* container) override {
        ContainerSqueeze<QList, T>::function(env, container);
    }
#endif
};

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#ifdef QVECTOR_H
template<typename T, bool isConst = false>
class QVectorAccess : public AbstractVectorAccess{
protected:
    QVectorAccess(){}
public:
    static AbstractVectorAccess* newInstance(){
        static QVectorAccess<T, isConst> instance;
        return &instance;
    }

    AbstractVectorAccess* clone() override{
        return this;
    }

    bool isConstant() override {return true;}
    const QMetaType& elementMetaType() override {static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(T)); return type;}
    void assign(void* container, const void* other) override { (*reinterpret_cast<QVector<T>*>(container)) = (*reinterpret_cast<const QVector<T>*>(other)); }
    size_t sizeOf() override {
        return sizeof(QVector<T>);
    }
    void* constructContainer(void* placement, const void* copyOf = nullptr) override {
        if(copyOf){
            return new(placement) QVector<T>(*reinterpret_cast<const QVector<T>*>(copyOf));
        }else{
            return new(placement) QVector<T>();
        }
    }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* move) override {
        return new(placement) QVector<T>(std::move(*reinterpret_cast<const QVector<T>*>(move)));
    }
#endif
    bool destructContainer(void* container) override {
        reinterpret_cast<QVector<T>*>(container)->~QVector<T>();
        return true;
    }
    int registerContainer(const QByteArray& containerTypeName) override {
        return RegistryAPI::registerMetaType<QVector<T>>(containerTypeName);
    }

    void appendVector(JNIEnv * env, void*, jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QVector::append" QTJAMBI_STACKTRACEINFO );
    }

    jobject at(JNIEnv * env, const void* container, jint index) override {
        return ContainerAt<QVector, T>::function(env, container, index);
    }

    jobject value(JNIEnv * env, const void* container, jint index) override {
        return ContainerValue<QVector, T>::function(env, container, index);
    }

    jobject value(JNIEnv * env, const void* container, jint index, jobject defaultValue) override {
        return ContainerValueDefault<QVector, T>::function(env, container, index, defaultValue);
    }

    void swapItemsAt(JNIEnv * env, void*, jint, jint) override {
        JavaException::raiseUnsupportedOperationException(env, "QVector::swapItemsAt" QTJAMBI_STACKTRACEINFO );
    }

    jboolean startsWith(JNIEnv * env, const void* container, jobject value) override {
        return ContainerStartsWith<QVector, T>::function(env, container, value);
    }

    jint size(JNIEnv * env, const void* container) override {
        return ContainerSize<QVector, T>::function(env, container);
    }

    void reserve(JNIEnv * env, void*, jint) override {
        JavaException::raiseUnsupportedOperationException(env, "QVector::reserve" QTJAMBI_STACKTRACEINFO );
    }

    void replace(JNIEnv * env, void*, jint, jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QVector::replace" QTJAMBI_STACKTRACEINFO );
    }

    jint removeAll(JNIEnv * env, void*, jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QVector::removeAll" QTJAMBI_STACKTRACEINFO );
        return 0;
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        return ContainerEquals<QVector, T, ContainerAPI::getAsQVector>::function(env, container, other);
    }

    void move(JNIEnv * env, void*, jint, jint) override {
        JavaException::raiseUnsupportedOperationException(env, "QVector::move" QTJAMBI_STACKTRACEINFO );
    }

    jobject mid(JNIEnv * env, const void* container, jint index1, jint index2) override {
        return ContainerMid<QVector, T>::function(env, container, index1, index2);
    }

    jint lastIndexOf(JNIEnv * env, const void* container, jobject value, jint index) override {
        return ContainerLastIndexOf<QVector, T>::function(env, container, value, index);
    }

    jint indexOf(JNIEnv * env, const void* container, jobject value, jint index) override {
        return ContainerIndexOf<QVector, T>::function(env, container, value, index);
    }

    jboolean endsWith(JNIEnv * env, const void* container, jobject value) override {
        return ContainerEndsWith<QVector, T>::function(env, container, value);
    }

    jint count(JNIEnv * env, const void* container, jobject value) override {
        return ContainerCountObject<QVector, T>::function(env, container, value);
    }

    jboolean contains(JNIEnv * env, const void* container, jobject value) override {
        return ContainerContains<QVector, T>::function(env, container, value);
    }

    void clear(JNIEnv * env, void*) override {
        JavaException::raiseUnsupportedOperationException(env, "QVector::clear" QTJAMBI_STACKTRACEINFO );
    }

    jint capacity(JNIEnv * env, const void* container) override {
        return ContainerCapacity<QVector, T>::function(env, container);
    }

    void fill(JNIEnv * env, void*, jobject, jint) override {
        JavaException::raiseUnsupportedOperationException(env, "QVector::fill" QTJAMBI_STACKTRACEINFO );
    }

    void remove(JNIEnv * env, void*, jint, jint) override {
        JavaException::raiseUnsupportedOperationException(env, "QVector::remove" QTJAMBI_STACKTRACEINFO );
    }

    void insert(JNIEnv * env, void*, jint, jint, jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QVector::insert" QTJAMBI_STACKTRACEINFO );
    }
    void resize(JNIEnv * env, void*, jint) override {
        JavaException::raiseUnsupportedOperationException(env, "QVector::resize" QTJAMBI_STACKTRACEINFO );
    }
    void squeeze(JNIEnv * env, void*) override {
        JavaException::raiseUnsupportedOperationException(env, "QVector::squeeze" QTJAMBI_STACKTRACEINFO );
    }

    jobject begin(JNIEnv * env, QtJambiNativeID, void*) override {
        JavaException::raiseUnsupportedOperationException(env, "QVector::begin" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }

    jobject end(JNIEnv * env, QtJambiNativeID, void*) override {
        JavaException::raiseUnsupportedOperationException(env, "QVector::end" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }

    jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        return ContainerConstBegin<QVector, T>::function(env, ownerId, container);
    }

    jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        return ContainerConstEnd<QVector, T>::function(env, ownerId, container);
    }

    void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) override {
        for(typename pointer_from<T>::In element : *reinterpret_cast<const QVector<T>*>(container)){
            if(!analyzer(pointer_from<T>::from(element), data)){
                break;
            }
        }
    }
};

template<typename T>
class QVectorAccess<T, false> : public QVectorAccess<T, true>{
protected:
    QVectorAccess(){}
public:
    static AbstractVectorAccess* newInstance(){
        static QVectorAccess<T, false> instance;
        return &instance;
    }

    AbstractVectorAccess* clone() override{
        return this;
    }

    bool isConstant() override {return false;}

    void appendVector(JNIEnv * env, void* container, jobject list) override {
        ContainerAppendVector<T>::function(env, container, list);
    }

    void swapItemsAt(JNIEnv * env, void* container, jint index1, jint index2) override {
        ContainerSwap<QVector, T>::function(env, container, index1, index2);
    }

    void reserve(JNIEnv * env, void* container, jint size) override {
        ContainerReserve<QVector, T>::function(env, container, size);
    }

    void replace(JNIEnv * env, void* container, jint index, jobject value) override {
        ContainerReplace<QVector, T>::function(env, container, index, value);
    }

    jint removeAll(JNIEnv * env, void* container, jobject value) override {
        return ContainerRemoveAll<QVector, T>::function(env, container, value);
    }

    void move(JNIEnv * env, void* container, jint index1, jint index2) override {
        ContainerMove<QVector, T>::function(env, container, index1, index2);
    }

    void clear(JNIEnv * env, void* container) override {
        ContainerClear<QVector, T>::function(env, container);
    }

    void fill(JNIEnv * env, void* container, jobject value, jint size) override {
        ContainerFill<QVector, T>::function(env, container, value, size);
    }

    void remove(JNIEnv * env, void* container, jint index, jint n) override {
        ContainerRemoveN<QVector, T>::function(env, container, index, n);
    }

    void insert(JNIEnv * env, void* container, jint index, jint n, jobject value) override {
        ContainerInsertN<QVector, T>::function(env, container, index, n, value);
    }
    void resize(JNIEnv * env, void* container, jint newSize) override {
        ContainerResize<QVector, T>::function(env, container, newSize);
    }
    void squeeze(JNIEnv * env, void* container) override {
        ContainerSqueeze<QVector, T>::function(env, container);
    }

    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) override {
        return ContainerBegin<QVector, T>::function(env, ownerId, container);
    }

    jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) override {
        return ContainerEnd<QVector, T>::function(env, ownerId, container);
    }
};
#endif

#ifdef QLINKEDLIST_H
template<typename T, bool isConst = false>
class QLinkedListAccess : public AbstractLinkedListAccess{
protected:
    QLinkedListAccess(){}
public:
    static AbstractLinkedListAccess* newInstance(){
        static QLinkedListAccess<T, isConst> instance;
        return &instance;
    }

    AbstractLinkedListAccess* clone() override{
        return this;
    }

    bool isConstant() override {return true;}
    const QMetaType& elementMetaType() override {static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(T)); return type;}
    void assign(void* container, const void* other) override { (*reinterpret_cast<QLinkedList<T>*>(container)) = (*reinterpret_cast<const QLinkedList<T>*>(other)); }
    size_t sizeOf() override {
        return sizeof(QLinkedList<T>);
    }
    void* constructContainer(void* placement, const void* copyOf = nullptr) override {
        if(copyOf){
            return new(placement) QLinkedList<T>(*reinterpret_cast<const QLinkedList<T>*>(copyOf));
        }else{
            return new(placement) QLinkedList<T>();
        }
    }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* move) override {
        return new(placement) QLinkedList<T>(std::move(*reinterpret_cast<const QLinkedList<T>*>(move)));
    }
#endif
    bool destructContainer(void* container) override {
        reinterpret_cast<QLinkedList<T>*>(container)->~QLinkedList<T>();
        return true;
    }
    int registerContainer(const QByteArray& containerTypeName) override {
        return RegistryAPI::registerMetaType<QLinkedList<T>>(containerTypeName);
    }

    void append(JNIEnv * env, void*, jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QLinkedList::append" QTJAMBI_STACKTRACEINFO );
    }

    jobject first(JNIEnv * env, const void* container) override {
        return ContainerFirst<QLinkedList, T>::function(env, container);
    }

    jobject last(JNIEnv * env, const void* container) override {
        return ContainerLast<QLinkedList, T>::function(env, container);
    }

    void clear(JNIEnv * env, void*) override {
        JavaException::raiseUnsupportedOperationException(env, "QLinkedList::clear" QTJAMBI_STACKTRACEINFO );
    }

    jboolean contains(JNIEnv * env, const void* container, jobject value) override {
        return ContainerContains<QLinkedList, T>::function(env, container, value);
    }

    jint count(JNIEnv * env, const void* container, jobject value) override {
        return ContainerCountObject<QLinkedList, T>::function(env, container, value);
    }

    jobject begin(JNIEnv * env, QtJambiNativeID, void*) override {
        JavaException::raiseUnsupportedOperationException(env, "QLinkedList::begin" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }

    jobject end(JNIEnv * env, QtJambiNativeID, void*) override {
        JavaException::raiseUnsupportedOperationException(env, "QLinkedList::end" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }

    jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        return ContainerConstBegin<QLinkedList, T>::function(env, ownerId, container);
    }

    jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        return ContainerConstEnd<QLinkedList, T>::function(env, ownerId, container);
    }

    jboolean endsWith(JNIEnv * env, const void* container, jobject value) override {
        return ContainerEndsWith<QLinkedList, T>::function(env, container, value);
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        return ContainerEquals<QLinkedList, T, ContainerAPI::getAsQLinkedList>::function(env, container, other);
    }

    void prepend(JNIEnv * env, void*, jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QLinkedList::prepend" QTJAMBI_STACKTRACEINFO );
    }

    void removeFirst(JNIEnv * env, void*) override {
        JavaException::raiseUnsupportedOperationException(env, "QLinkedList::removeFirst" QTJAMBI_STACKTRACEINFO );
    }

    jint removeAll(JNIEnv * env, void*, jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QLinkedList::removeAll" QTJAMBI_STACKTRACEINFO );
        return 0;
    }

    void removeLast(JNIEnv * env, void*) override {
        JavaException::raiseUnsupportedOperationException(env, "QLinkedList::removeLast" QTJAMBI_STACKTRACEINFO );
    }

    jboolean removeOne(JNIEnv * env, void*, jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QLinkedList::removeOne" QTJAMBI_STACKTRACEINFO );
        return false;
    }

    jint size(JNIEnv * env, const void* container) override {
        return ContainerSize<QLinkedList, T>::function(env, container);
    }

    jboolean startsWith(JNIEnv * env, const void* container, jobject value) override {
        return ContainerStartsWith<QLinkedList, T>::function(env, container, value);
    }

    jobject takeFirst(JNIEnv * env, void*) override {
        JavaException::raiseUnsupportedOperationException(env, "QLinkedList::takeFirst" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }

    jobject takeLast(JNIEnv * env, void*) override {
        JavaException::raiseUnsupportedOperationException(env, "QLinkedList::takeLast" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }

    void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) override {
        for(typename pointer_from<T>::In element : *reinterpret_cast<const QLinkedList<T>*>(container)){
            if(!analyzer(pointer_from<T>::from(element), data)){
                break;
            }
        }
    }
};

template<typename T>
class QLinkedListAccess<T, false> : public QLinkedListAccess<T, true>{
protected:
    QLinkedListAccess(){}
public:
    static AbstractLinkedListAccess* newInstance(){
        static QLinkedListAccess<T, false> instance;
        return &instance;
    }

    AbstractLinkedListAccess* clone() override{
        return this;
    }

    bool isConstant() override {return false;}

    void append(JNIEnv * env, void* container, jobject value) override {
        ContainerAppend<QLinkedList, T>::function(env, container, value);
    }

    void clear(JNIEnv * env, void* container) override {
        ContainerClear<QLinkedList, T>::function(env, container);
    }

    void prepend(JNIEnv * env, void* container, jobject value) override {
        ContainerPrepend<QLinkedList, T>::function(env, container, value);
    }

    void removeFirst(JNIEnv * env, void* container) override {
        ContainerRemoveFirst<QLinkedList, T>::function(env, container);
    }

    jint removeAll(JNIEnv * env, void* container, jobject value) override {
        return ContainerRemoveAll<QLinkedList, T>::function(env, container, value);
    }

    void removeLast(JNIEnv * env, void* container) override {
        ContainerRemoveLast<QLinkedList, T>::function(env, container);
    }

    jboolean removeOne(JNIEnv * env, void* container, jobject value) override {
        return ContainerRemoveOne<QLinkedList, T>::function(env, container, value);
    }

    jobject takeFirst(JNIEnv * env, void* container) override {
        return ContainerTakeFirst<QLinkedList, T>::function(env, container);
    }

    jobject takeLast(JNIEnv * env, void* container) override {
        return ContainerTakeLast<QLinkedList, T>::function(env, container);
    }

    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) override {
        return ContainerBegin<QLinkedList, T>::function(env, ownerId, container);
    }

    jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) override {
        return ContainerEnd<QLinkedList, T>::function(env, ownerId, container);
    }
};
#endif
#endif

template<typename T, bool isConst = false>
class QSetAccess : public AbstractSetAccess{
protected:
    QSetAccess(){}
public:
    static AbstractSetAccess* newInstance(){
        static QSetAccess<T, isConst> instance;
        return &instance;
    }

    AbstractSetAccess* clone() override{
        return this;
    }

    bool isConstant() override {return true;}
    const QMetaType& elementMetaType() override {static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(T)); return type;}
    void assign(void* container, const void* other) override { (*reinterpret_cast<QSet<T>*>(container)) = (*reinterpret_cast<const QSet<T>*>(other)); }
    size_t sizeOf() override {
        return sizeof(QSet<T>);
    }
    void* constructContainer(void* placement, const void* copyOf = nullptr) override {
        if(copyOf){
            return new(placement) QSet<T>(*reinterpret_cast<const QSet<T>*>(copyOf));
        }else{
            return new(placement) QSet<T>();
        }
    }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* move) override {
        return new(placement) QSet<T>(std::move(*reinterpret_cast<const QSet<T>*>(move)));
    }
#endif
    bool destructContainer(void* container) override {
        reinterpret_cast<QSet<T>*>(container)->~QSet<T>();
        return true;
    }

    int registerContainer(const QByteArray& containerTypeName) override {
        return RegistryAPI::registerMetaType<QSet<T>>(containerTypeName);
    }

    jint capacity(JNIEnv * env, const void* container) override {
        return ContainerCapacity<QSet, T>::function(env, container);
    }

    jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        return ContainerConstBegin<QSet, T>::function(env, ownerId, container);
        return nullptr;
    }

    jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        return ContainerConstEnd<QSet, T>::function(env, ownerId, container);
    }

    void clear(JNIEnv * env, void*) override {
        JavaException::raiseUnsupportedOperationException(env, "QSet::clear()" QTJAMBI_STACKTRACEINFO );
    }

    jboolean contains(JNIEnv * env, const void* container, jobject value) override {
        return ContainerContains<QSet, T>::function(env, container, value);
    }

    void insert(JNIEnv * env, void*, jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QSet::insert(value)" QTJAMBI_STACKTRACEINFO );
    }

    void intersect(JNIEnv * env, void*, jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QSet::intersect(other)" QTJAMBI_STACKTRACEINFO );
    }

    jboolean intersects(JNIEnv * env, const void* container, jobject other) override {
        return ContainerIntersects<QSet, T, ContainerAPI::getAsQSet>::function(env, container, other);
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        return ContainerEquals<QSet, T, ContainerAPI::getAsQSet>::function(env, container, other);
    }

    jboolean remove(JNIEnv * env, void*, jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QSet::remove(value)" QTJAMBI_STACKTRACEINFO );
        return false;
    }

    void reserve(JNIEnv * env, void*, jint) override {
        JavaException::raiseUnsupportedOperationException(env, "QSet::reserve(newSize)" QTJAMBI_STACKTRACEINFO );
    }

    jint size(JNIEnv * env, const void* container) override {
        return ContainerSize<QSet, T>::function(env, container);
    }

    void subtract(JNIEnv * env, void*, jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QSet::subtract(other)" QTJAMBI_STACKTRACEINFO );
    }

    void unite(JNIEnv * env, void*, jobject) override {
        JavaException::raiseUnsupportedOperationException(env, "QSet::unite(other)" QTJAMBI_STACKTRACEINFO );
    }

    jobject values(JNIEnv * env, const void* container) override {
        return ContainerValues<QSet, T>::function(env, container);
    }

    void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) override {
        for(typename pointer_from<T>::In element : *reinterpret_cast<const QSet<T>*>(container)){
            if(!analyzer(pointer_from<T>::from(element), data)){
                break;
            }
        }
    }
};

template<typename T>
class QSetAccess<T, false> : public QSetAccess<T, true>{
protected:
    QSetAccess(){}
public:
    static AbstractSetAccess* newInstance(){
        static QSetAccess<T, false> instance;
        return &instance;
    }

    AbstractSetAccess* clone() override{
        return this;
    }

    bool isConstant() override {return false;}

    void clear(JNIEnv * env, void* container) override {
        ContainerClear<QSet, T>::function(env, container);
    }
    void insert(JNIEnv * env, void* container, jobject value) override {
        ContainerInsert<QSet, T>::function(env, container, value);
    }
    void intersect(JNIEnv * env, void* container, jobject other) override {
        ContainerIntersect<QSet, T, ContainerAPI::getAsQSet>::function(env, container, other);
    }
    jboolean remove(JNIEnv * env, void* container, jobject value) override {
        return ContainerRemove<QSet, T>::function(env, container, value);
    }
    void reserve(JNIEnv * env, void* container, jint newSize) override {
        ContainerReserve<QSet, T>::function(env, container, newSize);
    }
    void subtract(JNIEnv * env, void* container, jobject other) override {
        ContainerSubtract<QSet, T, ContainerAPI::getAsQSet>::function(env, container, other);
    }
    void unite(JNIEnv * env, void* container, jobject other) override {
        ContainerUnite<QSet, T, ContainerAPI::getAsQSet>::function(env, container, other);
    }
};

template<bool has_scope, typename Container, bool is_const, typename T>
struct qtjambi_collection_fill{

};

template<bool has_scope, typename Container, typename T>
struct qtjambi_collection_fill<has_scope, Container, true, T>{
    static void fill(JNIEnv *env, const Container& container, jobject collection, QtJambiScope* scope){
        for(typename Container::const_iterator i = container.begin(); i!=container.end(); ++i){
            QtJambiAPI::addToJavaCollection(env, collection, qtjambi_scoped_cast<has_scope,jobject,decltype(*i)>::cast(env, *i, nullptr, scope));
        }
    }
};

template<bool has_scope, typename Container, typename T>
struct qtjambi_collection_fill<has_scope, Container, false, T>{
    static void fill(JNIEnv *env, Container& container, jobject collection, QtJambiScope* scope){
        for(typename Container::iterator i = container.begin(); i!=container.end(); ++i){
            QtJambiAPI::addToJavaCollection(env, collection, qtjambi_scoped_cast<has_scope,jobject,decltype(*i)>::cast(env, *i, nullptr, scope));
        }
    }
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_IMPL_CONTAINER_SEQUENTIAL_H
