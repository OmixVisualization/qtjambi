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

#ifndef QTJAMBI_CAST_IMPL_CONTAINER_SEQUENTIAL_H
#define QTJAMBI_CAST_IMPL_CONTAINER_SEQUENTIAL_H

#include "qtjambi_cast_impl_util.h"
#include "qtjambi_cast_impl_container_iterator.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0) && defined(QLINKEDLIST_H)
QT_WARNING_DISABLE_GCC("-Wdeprecated-declarations")
QT_WARNING_DISABLE_DEPRECATED
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
#include <QtCore/QSpan>
#endif

namespace RegistryAPI{
template<typename T>
int registerMetaType(const QByteArray& typeName);
}

enum class QtJambiNativeID : jlong;

namespace QtJambiPrivate {

template<bool has_scope, template<typename T> class Container, typename T>
struct IntermediateSequentialContainer : Container<T>{
    IntermediateSequentialContainer(JNIEnv *env, jobject object, QtJambiScope* scope = nullptr)
        : Container<T>(), m_scope(scope), m_object(env->NewWeakGlobalRef(object)){}
    ~IntermediateSequentialContainer(){
        QTJAMBI_TRY_ANY{
            if(JniEnvironment env{200}){
                QTJAMBI_TRY{
                    jobject object = env->NewLocalRef(m_object);
                    env->DeleteWeakGlobalRef(jweak(m_object));
                    if(!env->IsSameObject(object, nullptr)){
                        QtJambiAPI::clearJavaCollection(env, object);
                        for(typename Container<T>::const_iterator i = Container<T>::constBegin(); i!=Container<T>::constEnd(); ++i){
                            JniLocalFrame f(env, 32);
                            jobject val = qtjambi_scoped_cast<has_scope,jobject,decltype(*i)>::cast(env, *i, nullptr, m_scope);
                            QtJambiAPI::addToJavaCollection(env, object, val);
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
    QtJambiScope* m_scope;
    jobject m_object;
};

typedef bool (*IsContainerFunction)(JNIEnv *, jobject, const std::type_info&, const QMetaType&, void*& pointer);
typedef bool (*IsContainerAccessFunction)(JNIEnv *, jobject, const QMetaType&, void*& pointer, AbstractContainerAccess*& access);

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

    const QMetaType& valueMetaType() override{
        typedef typename std::remove_reference<decltype(*std::declval<Iterator>())>::type T;
        static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(T));
        return type;
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

    const QMetaType& valueMetaType() override{
        return QSequentialConstIteratorAccess<Iterator>::valueMetaType();
    }

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

template<template<typename T> class Container, typename T, bool isPointer, bool = ((is_copy_constructible<T>::value && is_default_constructible<T>::value)
                                                                                   || std::is_trivially_copyable<T>::value
                                                                                   || is_shared_data<Container<T>>::value) && is_copy_constructible<Container<T>>::value>
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

template<typename T, bool isPointer = (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0>
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

template<template<typename T> class Container, typename T, bool = is_default_constructible<T>::value>
struct ContainerAt{
    static jobject function(JNIEnv * env, const void*, jint) {
        JavaException::raiseUnsupportedOperationException(env, "at(i)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
    static const void* function(const void*, qsizetype) {
        return nullptr;
    }
    static void* function(void*, qsizetype) {
        return nullptr;
    }
};

template<template<typename T> class Container, typename T>
struct ContainerAt<Container,T,true>{
    static jobject function(JNIEnv * env, const void* ptr, jint idx) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return qtjambi_scoped_cast<false,jobject,const T>::cast(env, container->at(int(idx)), nullptr, nullptr);
    }
    static const void* function(const void* ptr, qsizetype idx) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr);
        return &container->at(decltype(container->size())(idx));
    }
    static void* function(void* ptr, qsizetype idx) {
        Container<T> *container = static_cast<Container<T> *>(ptr);
        return &(*container)[decltype(container->size())(idx)];
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
                jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, other);
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

template<template<typename T> class Container, typename T, IsContainerAccessFunction isContainer>
struct ContainerIntersect{
    static void function(JNIEnv * env, const ContainerInfo& ptr, ContainerAndAccessInfo& other) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        std::unique_ptr<Container<T> > __qt_scoped_pointer;
        Container<T> *__qt_other_pointer = nullptr;
        if (other.object != nullptr) {
            if (isContainer(env, other.object, QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(__qt_other_pointer), other.access)) {
                other.container = __qt_other_pointer;
            }else{
                __qt_scoped_pointer.reset(new Container<T> ());
                __qt_other_pointer = __qt_scoped_pointer.get();
                jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, other.object);
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

template<template<typename T> class Container, typename T, IsContainerAccessFunction isContainer>
struct ContainerUnite{
    static void function(JNIEnv * env, const ContainerInfo& ptr, ContainerAndAccessInfo& other) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        std::unique_ptr<Container<T> > __qt_scoped_pointer;
        Container<T> *__qt_other_pointer = nullptr;
        if (other.object != nullptr) {
            if (isContainer(env, other.object, QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(__qt_other_pointer), other.access)) {
                other.container = __qt_other_pointer;
            }else{
                __qt_scoped_pointer.reset(new Container<T> ());
                __qt_other_pointer = __qt_scoped_pointer.get();
                jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, other.object);
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

template<template<typename T> class Container, typename T, IsContainerAccessFunction isContainer>
struct ContainerSubtract{
    static void function(JNIEnv * env, const ContainerInfo& ptr, ContainerAndAccessInfo& other) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        std::unique_ptr<Container<T> > __qt_scoped_pointer;
        Container<T> *__qt_other_pointer = nullptr;
        if (other.object != nullptr) {
            if (isContainer(env, other.object, QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(__qt_other_pointer), other.access)) {
                other.container = __qt_other_pointer;
            }else{
                __qt_scoped_pointer.reset(new Container<T> ());
                __qt_other_pointer = __qt_scoped_pointer.get();
                jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, other.object);
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
    static jobject function(JNIEnv *env, const ExtendedContainerInfo& ptr) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        return qtjambi_sequential_iterator_caster<decltype(container->begin())>::cast(env, ptr.nativeId, container->begin());
    }
};

template<template<typename T> class Container, typename T>
struct ContainerEnd{
    static jobject function(JNIEnv *env, const ExtendedContainerInfo& ptr) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        return qtjambi_sequential_iterator_caster<decltype(container->end())>::cast(env, ptr.nativeId, container->end());
    }
};

template<template<typename T> class Container, typename T>
struct ContainerConstBegin{
    static jobject function(JNIEnv *env, const ConstExtendedContainerInfo& ptr) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr.container);
        return qtjambi_sequential_iterator_caster<decltype(container->constBegin())>::cast(env, ptr.nativeId, container->constBegin());
    }
};

template<template<typename T> class Container, typename T>
struct ContainerConstEnd{
    static jobject function(JNIEnv *env, const ConstExtendedContainerInfo& ptr) {
        const Container<T> *container = static_cast<const Container<T> *>(ptr.container);
        return qtjambi_sequential_iterator_caster<decltype(container->constEnd())>::cast(env, ptr.nativeId, container->constEnd());
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

template<template<typename T> class Container, jobject(*objectFromContainer)(JNIEnv *, void*&, AbstractContainerAccess*&), typename T, bool = is_default_constructible<T>::value>
struct ContainerMid{
    static ContainerAndAccessInfo function(JNIEnv * env, const ConstContainerAndAccessInfo&, jint, jint) {
        JavaException::raiseUnsupportedOperationException(env, "mid(index1, index2)" QTJAMBI_STACKTRACEINFO );
        return {};
    }
};

template<template<typename T> class Container, jobject(*objectFromContainer)(JNIEnv *, void*&, AbstractContainerAccess*&), typename T>
struct ContainerMid<Container, objectFromContainer, T, true>{
    static ContainerAndAccessInfo function(JNIEnv * env, const ConstContainerAndAccessInfo& ptr, jint idx1, jint idx2) {
        ContainerAndAccessInfo result;
        const Container<T> *container = static_cast<const Container<T> *>(ptr.container);
        Container<T>* mid = new Container<T>(container->mid(int(idx1), int(idx2)));
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
            if (!is_Container1_fct(env, other, qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(__qt_other_pointer))) {
                __qt_scoped_pointer.reset(new Container<T> ());
                __qt_other_pointer = __qt_scoped_pointer.get();
                jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, other);
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
        return container->startsWith(qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr));
    }
};

template<template<typename T> class Container, typename T, bool = is_default_constructible<T>::value>
struct SequentialContainerValue{
    static jobject function(JNIEnv * env, const void*, jint) {
        JavaException::raiseUnsupportedOperationException(env, "value(i)" QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
};

template<template<typename T> class Container, typename T>
struct SequentialContainerValue<Container, T, true>{
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
    static void function(JNIEnv * env, const ContainerInfo&, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "append(value)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename T> class Container, typename T>
struct ContainerAppend<Container, T, true>{
    static void function(JNIEnv * env, const ContainerInfo& ptr, jobject object) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->append(qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr));
    }
};

template<template<typename T> class Container, typename T, bool = is_default_constructible<T>::value>
struct ContainerFill{
    static void function(JNIEnv * env, const ContainerInfo&, jobject, jint) {
        JavaException::raiseUnsupportedOperationException(env, "fill(value,size)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename T> class Container, typename T>
struct ContainerFill<Container, T, true>{
    static void function(JNIEnv * env, const ContainerInfo& ptr, jobject object, jint size) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->fill(qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr), size);
    }
};

template<typename T, bool = is_copy_constructible<T>::value || std::is_move_constructible<T>::value>
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
    static void function(JNIEnv * env, const ContainerInfo&, ContainerAndAccessInfo&) {
        JavaException::raiseUnsupportedOperationException(env, "QVector::append(vector)" QTJAMBI_STACKTRACEINFO );
    }
};

template<typename T>
struct ContainerAppendVector<T, true>{
    static void function(JNIEnv * env, const ContainerInfo& ptr, ContainerAndAccessInfo& containerInfo) {
        QVector<T> *container = static_cast<QVector<T> *>(ptr.container);
        std::unique_ptr<QVector<T> > __qt_scoped_pointer;
        QVector<T> *__qt_object_pointer = nullptr;
        if (containerInfo.object!= nullptr) {
            if (ContainerAPI::getAsQVector<T>(env, containerInfo.object, __qt_object_pointer, containerInfo.access)) {
                containerInfo.container = __qt_object_pointer;
            }else{
                __qt_scoped_pointer.reset(new QVector<T>());
                __qt_object_pointer = __qt_scoped_pointer.get();
                jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, containerInfo.object);
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
    static void function(JNIEnv *, const ContainerInfo& ptr) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->clear();
    }
};

template<template<typename T> class Container, typename T, bool = is_copy_constructible<T>::value && is_default_constructible<T>::value>
struct ContainerInsertAt{
    static void function(JNIEnv * env, const ContainerInfo&, jint, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "insert(i,value)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename T> class Container, typename T>
struct ContainerInsertAt<Container, T, true>{
    static void function(JNIEnv * env, const ContainerInfo& ptr, jint i, jobject value) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->insert(i, qtjambi_scoped_cast<false,T,jobject>::cast(env, value, nullptr, nullptr));
    }
};

template<template<typename T> class Container, typename T, bool = is_copy_constructible<T>::value && is_default_constructible<T>::value>
struct ContainerInsertN{
    static void function(JNIEnv * env, const ContainerInfo&, jint, jint, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "insert(i,n,value)" QTJAMBI_STACKTRACEINFO );
    }
    static void function(void*, qsizetype, qsizetype, const void*) {
    }
};

template<template<typename T> class Container, typename T>
struct ContainerInsertN<Container, T, true>{
    static void function(JNIEnv * env, const ContainerInfo& ptr, jint i, jint n, jobject value) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->insert(i, n, qtjambi_scoped_cast<false,T,jobject>::cast(env, value, nullptr, nullptr));
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
        container->insert(qtjambi_scoped_cast<false,T,jobject>::cast(env, value, nullptr, nullptr));
    }
};

template<template<typename T> class Container, typename T, bool = is_copy_constructible<T>::value && is_default_constructible<T>::value>
struct ContainerMove{
    static void function(JNIEnv * env, const ContainerInfo&, jint, jint) {
        JavaException::raiseUnsupportedOperationException(env, "move(index1, index2)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename T> class Container, typename T>
struct ContainerMove<Container, T, true>{
    static void function(JNIEnv * env, const ContainerInfo& ptr, jint idx1, jint idx2) {
        Q_UNUSED(env)
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->move(int(idx1), int(idx2));
    }
};

template<template<typename T> class Container, typename T, bool = is_copy_constructible<T>::value || std::is_move_constructible<T>::value>
struct ContainerPrepend{
    static void function(JNIEnv * env, const ContainerInfo&, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "prepend(value)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename T> class Container, typename T>
struct ContainerPrepend<Container, T, true>{
    static void function(JNIEnv * env, const ContainerInfo& ptr, jobject object) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->prepend(qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr));
    }
};

template<template<typename T> class Container, typename T, bool = supports_equal<T>::value>
struct ContainerRemoveAll{
    static jint function(JNIEnv * env, const ContainerInfo&, jobject) {
        JavaException::raiseUnsupportedOperationException(env, "removeAll(value)" QTJAMBI_STACKTRACEINFO );
        return 0;
    }
};

template<template<typename T> class Container, typename T>
struct ContainerRemoveAll<Container, T, true>{
    static jint function(JNIEnv * env, const ContainerInfo& ptr, jobject object) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        return container->removeAll(qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr));
    }
};

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
template<template<typename T> class Container, typename T>
struct ContainerTakeFirst{
    static jobject function(JNIEnv * env, const ContainerInfo& ptr) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        return qtjambi_scoped_cast<false,jobject,const T>::cast(env, container->takeFirst(), nullptr, nullptr);
    }
};

template<template<typename T> class Container, typename T>
struct ContainerTakeLast{
    static jobject function(JNIEnv * env, const ContainerInfo& ptr) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        return qtjambi_scoped_cast<false,jobject,const T>::cast(env, container->takeLast(), nullptr, nullptr);
    }
};

template<template<typename T> class Container, typename T>
struct ContainerRemoveFirst{
    static void function(JNIEnv * env, const ContainerInfo& ptr) {
        Q_UNUSED(env)
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->removeFirst();
    }
};

template<template<typename T> class Container, typename T>
struct ContainerRemoveLast{
    static void function(JNIEnv * env, const ContainerInfo& ptr) {
        Q_UNUSED(env)
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->removeLast();
    }
};
#endif

template<template<typename T> class Container, typename T>
struct ContainerRemoveAt{
    static void function(JNIEnv * env, const ContainerInfo& ptr, jint idx) {
        Q_UNUSED(env)
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->removeAt(idx);
    }
    static void function(void* ptr, qsizetype idx) {
        Container<T> *container = static_cast<Container<T> *>(ptr);
        container->removeAt(idx);
    }
};

template<template<typename T> class Container, typename T, bool = is_default_constructible<T>::value>
struct ContainerRemoveI{
    static void function(JNIEnv * env, const ContainerInfo&, jint) {
        JavaException::raiseUnsupportedOperationException(env, "remove(index)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename T> class Container, typename T>
struct ContainerRemoveI<Container, T, true>{
    static void function(JNIEnv * env, const ContainerInfo& ptr, jint idx) {
        Q_UNUSED(env)
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->remove(idx);
    }
};

template<template<typename T> class Container, typename T, bool = is_default_constructible<T>::value>
struct ContainerRemoveN{
    static void function(JNIEnv * env, const ContainerInfo&, jint, jint) {
        JavaException::raiseUnsupportedOperationException(env, "remove(index,n)" QTJAMBI_STACKTRACEINFO );
    }
    static void function(void*, qsizetype, qsizetype) {}
};

template<template<typename T> class Container, typename T>
struct ContainerRemoveN<Container, T, true>{
    static void function(JNIEnv * env, const ContainerInfo& ptr, jint idx, jint n) {
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
        return container->removeOne(qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr));
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
        return container->remove(qtjambi_scoped_cast<false,T,jobject>::cast(env, object, nullptr, nullptr));
    }
};

template<template<typename T> class Container, typename T, bool = is_default_constructible<T>::value && !std::is_const<T>::value>
struct ContainerReplace{
    static void function(JNIEnv * env, const ContainerInfo&, jint,jobject) {
        JavaException::raiseUnsupportedOperationException(env, "replace(index,value)" QTJAMBI_STACKTRACEINFO );
    }
    static void function(void*, qsizetype, const void*) {}
};

template<template<typename T> class Container, typename T>
struct ContainerReplace<Container, T, true>{
    static void function(JNIEnv * env, const ContainerInfo& ptr, jint idx,jobject newObject) {
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->replace(idx, qtjambi_scoped_cast<false,T,jobject>::cast(env, newObject, nullptr, nullptr));
    }

    static void function(void* ptr, qsizetype idx, const void* newObject) {
        Container<T> *container = static_cast<Container<T> *>(ptr);
        container->replace(idx, *reinterpret_cast<const T*>(newObject));
    }
};

template<template<typename T> class Container, typename T, bool = is_default_constructible<T>::value>
struct ContainerReserve{
    static void function(JNIEnv * env, const ContainerInfo&, jint) {
        JavaException::raiseUnsupportedOperationException(env, "reserve(size)" QTJAMBI_STACKTRACEINFO );
    }
};

template<template<typename T> class Container, typename T>
struct ContainerReserve<Container, T, true>{
    static void function(JNIEnv * env, const ContainerInfo& ptr, jint size) {
        Q_UNUSED(env)
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->reserve(size);
    }
};

template<template<typename T> class Container, typename T, bool = is_default_constructible<T>::value && !std::is_const<T>::value>
struct ContainerResize{
    static void function(JNIEnv * env, const ContainerInfo&, jint) {
        JavaException::raiseUnsupportedOperationException(env, "resize(size)" QTJAMBI_STACKTRACEINFO );
    }
    static void function(void*, qsizetype) {
    }
};

template<template<typename T> class Container, typename T>
struct ContainerResize<Container, T, true>{
    static void function(JNIEnv * env, const ContainerInfo& ptr, jint size) {
        Q_UNUSED(env)
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
        container->resize(size);
    }
    static void function(void* ptr, qsizetype size) {
        Container<T> *container = static_cast<Container<T> *>(ptr);
        container->resize(size);
    }
};

template<template<typename T> class Container, typename T, bool = is_default_constructible<T>::value>
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
    static void function(JNIEnv * env, const ContainerInfo& ptr, jint idx1, jint idx2) {
        Q_UNUSED(env)
        Container<T> *container = static_cast<Container<T> *>(ptr.container);
#if QT_VERSION < QT_VERSION_CHECK(5,13,0)
        container->swap(int(idx1), int(idx2));
#else
        container->swapItemsAt(int(idx1), int(idx2));
#endif
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
    void updateRC(JNIEnv * env, const ContainerInfo& container) override {
        Super* _this = this;
        JniLocalFrame frame(env, 200);
        jobject set{nullptr};
        if constexpr(ContainerContentType<T>::type==AbstractContainerAccess::PointerToQObject){
            set = QtJambiAPI::newJavaArrayList(env);
            auto iterator = _this->constElementIterator(container.container);
            while(iterator->hasNext()){
                const void* content = iterator->next();
                if(jobject obj = QtJambiAPI::findObject(env, reinterpret_cast<const QObject*>(content)))
                    Java::Runtime::Collection::add(env, set, obj);
            }
        }else if constexpr(ContainerContentType<T>::type==AbstractContainerAccess::FunctionPointer){
            set = QtJambiAPI::newJavaArrayList(env);
            auto iterator = _this->constElementIterator(container.container);
            while(iterator->hasNext()){
                const void* content = iterator->next();
                if(jobject obj = QtJambiAPI::findFunctionPointerObject(env, content, typeid(T)))
                    Java::Runtime::Collection::add(env, set, obj);
            }
        }else if constexpr(ContainerContentType<T>::type==AbstractContainerAccess::Pointer){
            set = QtJambiAPI::newJavaArrayList(env);
            auto iterator = _this->constElementIterator(container.container);
            while(iterator->hasNext()){
                const void* content = iterator->next();
                if(jobject obj = QtJambiAPI::findObject(env, content))
                    Java::Runtime::Collection::add(env, set, obj);
            }
        }else if constexpr(ContainerContentType<T>::isContainer && ContainerContentType<T>::needsReferenceCounting){
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
        if constexpr(ContainerContentType<T>::isContainer){
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
        if constexpr(ContainerContentType<T>::isContainer){
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
         bool T_is_pointer = (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask)!=0 /*true*/>
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

template<typename T>
class QListAccess : public SequentialAccessSuperclassDecider<T,AbstractListAccess>::type{
    typedef typename SequentialAccessSuperclassDecider<T,AbstractListAccess>::type Super;
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
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if(ReferenceCountingSetContainer* access = dynamic_cast<ReferenceCountingSetContainer*>(container2.access)){
                Q_UNUSED(access)
                if(container2.access!=this)
                    Super::swapRC(env, container, container2);
            }else{
                Super::updateRC(env, container);
            }
        }else{
            Q_UNUSED(env);
        }
    }

    bool append(void* container, const void* entry) override{
        reinterpret_cast<QList<T> *>(container)->append(*reinterpret_cast<const T*>(entry));
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if constexpr(ContainerContentType<T>::isContainer){
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
        static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(T));
        return type;
    }

    AbstractContainerAccess::DataType elementType() override{
        return ContainerContentType<T>::type;
    }

    AbstractContainerAccess* elementNestedContainerAccess() override {
        return ContainerContentType<T>::accessFactory();
    }
    bool hasNestedContainerAccess() override {
        return ContainerContentType<T>::isContainer;
    }
    bool hasNestedPointers() override {
        return ContainerContentType<T>::isContainer && ContainerContentType<T>::needsReferenceCounting;
    }

    size_t sizeOf() override {
        return sizeof(QList<T>);
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* move) override {
        return new(placement) QList<T>(std::move(*reinterpret_cast<const QList<T>*>(move)));
    }
    void* constructContainer(JNIEnv *, void* placement, const ContainerAndAccessInfo& move) override {
        return constructContainer(placement, move.container);
    }
#endif
    bool destructContainer(void* container) override {
        reinterpret_cast<QList<T>*>(container)->~QList<T>();
        return true;
    }
    void assign(void* container, const void* other) override {
        (*reinterpret_cast<QList<T>*>(container)) = (*reinterpret_cast<const QList<T>*>(other));
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if(JniEnvironment env{100}){
                if(jobject object = QtJambiAPI::findObject(env, container))
                    Super::updateRC(env, {object, container});
            }
        }
    }
    void assign(JNIEnv *env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override {
        (*reinterpret_cast<QList<T>*>(container.container)) = (*reinterpret_cast<const QList<T>*>(other.container));
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if(ReferenceCountingSetContainer* access = dynamic_cast<ReferenceCountingSetContainer*>(other.access)){
                Q_UNUSED(access)
                if(other.access!=this)
                    ReferenceCountingSetContainer::assignRC(env, container.object, other.object);
            }else{
                Super::updateRC(env, container);
            }
        }else{
            Q_UNUSED(env);
        }
    }
    int registerContainer(const QByteArray& containerTypeName) override {
        return RegistryAPI::registerMetaType<QList<T>>(containerTypeName, this);
    }

    jobject at(JNIEnv * env, const void* container, jint index) override {
        return ContainerAt<QList, T>::function(env, container, index);
    }

    const void* at(const void* container, qsizetype index) override {
        return ContainerAt<QList, T>::function(container, index);
    }

    void* at(void* container, qsizetype index) override {
        return ContainerAt<QList, T>::function(container, index);
    }

    jobject value(JNIEnv * env, const void* container, jint index) override {
        return SequentialContainerValue<QList, T>::function(env, container, index);
    }

    jobject value(JNIEnv * env, const void* container, jint index, jobject defaultValue) override {
        return ContainerValueDefault<QList, T>::function(env, container, index, defaultValue);
    }

    jboolean startsWith(JNIEnv * env, const void* container, jobject value) override {
        return ContainerStartsWith<QList, T>::function(env, container, value);
    }

    jint size(JNIEnv * env, const void* container) override {
        return ContainerSize<QList, T>::function(env, container);
    }

    qsizetype size(const void* container) override {
        return ContainerSize<QList, T>::function(container);
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        return ContainerEquals<QList, T, ContainerAPI::getAsQList>::function(env, container, other);
    }

    ContainerAndAccessInfo mid(JNIEnv * env, const ConstContainerAndAccessInfo& container, jint index1, jint index2) override {
        return ContainerMid<QList, ContainerAPI::objectFromQList, T>::function(env, container, index1, index2);
    }

    jint lastIndexOf(JNIEnv * env, const void* container, jobject value, jint index) override {
        return ContainerLastIndexOf<QList, T>::function(env, container, value, index);
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

    bool contains(const void* container, const void* value) {
        return ContainerContains<QList, T>::function(container, value);
    }

    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        return ContainerConstBegin<QList, T>::function(env, container);
    }

    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        return ContainerConstEnd<QList, T>::function(env, container);
    }

    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override {
        return ContainerBegin<QList, T>::function(env, container);
    }

    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override {
        return ContainerEnd<QList, T>::function(env, container);
    }

    void appendList(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) override {
        ContainerAppendList<T>::function(env, container, containerInfo);
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if constexpr(ContainerContentType<T>::isContainer){
                Super::updateRC(env, container);
            }else{
                Super::addAllRC(env, container.object, Super::findContainer(env, containerInfo.object));
            }
        }
    }

    void swapItemsAt(JNIEnv * env, const ContainerInfo& container, jint index1, jint index2) override {
        ContainerSwap<QList, T>::function(env, container, index1, index2);
    }

    void reserve(JNIEnv * env, const ContainerInfo& container, jint size) override {
        ContainerReserve<QList, T>::function(env, container, size);
    }

    void replace(void* container, qsizetype index, const void* value) override {
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if constexpr(ContainerContentType<T>::isContainer){
                ContainerReplace<QList, T>::function(container, index, value);
                if(JniEnvironment env{100}){
                    if(jobject object = QtJambiAPI::findObject(env, container))
                        Super::updateRC(env, {object, container});
                }
            }else{
                if(JniEnvironment env{100}){
                    jobject oldValue = at(env, container, jint(index));
                    ContainerReplace<QList, T>::function(container, index, value);
                    if(jobject object = QtJambiAPI::findObject(env, container)){
                        if(oldValue && !contains(env, container, oldValue))
                            Super::removeRC(env, object, oldValue);
                        if(jobject valueObj = QtJambiAPI::findObject(env, *reinterpret_cast<void*const*>(value)))
                            Super::addRC(env, object, valueObj);
                    }
                }else{
                    ContainerReplace<QList, T>::function(container, index, value);
                }
            }
        }else{
            ContainerReplace<QList, T>::function(container, index, value);
        }
    }

    void replace(JNIEnv * env, const ContainerInfo& container, jint index, jobject value) override {
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if constexpr(ContainerContentType<T>::isContainer){
                ContainerReplace<QList, T>::function(env, container, index, value);
                Super::updateRC(env, container);
            }else{
                jobject oldValue = at(env, container.container, index);
                ContainerReplace<QList, T>::function(env, container, index, value);
                if(oldValue && !contains(env, container.container, oldValue))
                    Super::removeRC(env, container.object, oldValue);
                if(value)
                    Super::addRC(env, container.object, value);
            }
        }else{
            ContainerReplace<QList, T>::function(env, container, index, value);
        }
    }

    void remove(void* container, qsizetype index, qsizetype n) override {
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if constexpr(ContainerContentType<T>::isContainer){
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                ContainerRemoveN<QList, T>::function(container, index, n);
#else
                Q_ASSERT(n==1);
                ContainerRemoveAt<QList, T>::function(container, index);
#endif
                if(JniEnvironment env{100}){
                    if(jobject object = QtJambiAPI::findObject(env, container))
                        Super::updateRC(env, {object,container});
                }
            }else{
                if(JniEnvironment env{100}){
                    if(jobject object = QtJambiAPI::findObject(env, container)){
                        if(n==1){
                            jobject oldValue = at(env, container, index);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                           ContainerRemoveN<QList, T>::function(container, index, n);
#else
                            Q_ASSERT(n==1);
                            ContainerRemoveAt<QList, T>::function(container, index);
#endif
                            Super::removeRC(env, object, oldValue);
                        }else{
                            jint _size = size(env, container);
                            jobject removedValues = QtJambiAPI::newJavaArrayList(env);
                            for(jint i = index; i<=index+n && i<_size; ++i){
                                Java::Runtime::Collection::add(env, removedValues, at(env, container, i));
                            }
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                           ContainerRemoveN<QList, T>::function(container, index, n);
#else
                            Q_ASSERT(n==1);
                            ContainerRemoveAt<QList, T>::function(container, index);
#endif
                            jobject iter = Java::Runtime::Collection::iterator(env, removedValues);
                            while(Java::Runtime::Iterator::hasNext(env, iter)){
                                jobject value = Java::Runtime::Iterator::next(env, iter);
                                Super::removeRC(env, object, value);
                            }
                        }
                    }
                }
            }
        }else{
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
            ContainerRemoveN<QList, T>::function(container, index, n);
#else
            Q_ASSERT(n==1);
            ContainerRemoveAt<QList, T>::function(container, index);
#endif
        }
    }

    void remove(JNIEnv * env, const ContainerInfo& container, jint index, jint n) override {
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if constexpr(ContainerContentType<T>::isContainer){
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                ContainerRemoveN<QList, T>::function(env, container, index, n);
#else
                Q_ASSERT(n==1);
                ContainerRemoveAt<QList, T>::function(env, container, index);
#endif
                Super::updateRC(env, container);
            }else{
                if(n==1){
                    jobject oldValue = at(env, container.container, index);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                    ContainerRemoveN<QList, T>::function(env, container, index, n);
#else
                    Q_ASSERT(n==1);
                    ContainerRemoveAt<QList, T>::function(env, container, index);
#endif
                    Super::removeRC(env, container.object, oldValue);
                }else{
                    jint _size = size(env, container.container);
                    jobject removedValues = QtJambiAPI::newJavaArrayList(env);
                    for(jint i = index; i<=index+n && i<_size; ++i){
                        Java::Runtime::Collection::add(env, removedValues, at(env, container.container, i));
                    }
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                    ContainerRemoveN<QList, T>::function(env, container, index, n);
#else
                    Q_ASSERT(n==1);
                    ContainerRemoveAt<QList, T>::function(env, container, index);
#endif
                    jobject iter = Java::Runtime::Collection::iterator(env, removedValues);
                    while(Java::Runtime::Iterator::hasNext(env, iter)){
                        jobject value = Java::Runtime::Iterator::next(env, iter);
                        Super::removeRC(env, container.object, value);
                    }
                }
            }
        }else{
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
            ContainerRemoveN<QList, T>::function(env, container, index, n);
#else
            Q_ASSERT(n==1);
            ContainerRemoveAt<QList, T>::function(env, container, index);
#endif
        }
    }

    jint removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) override {
        jint result = ContainerRemoveAll<QList, T>::function(env, container, value);
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if constexpr(ContainerContentType<T>::isContainer){
                if(result>0)
                    Super::updateRC(env, container);
            }else{
                Super::removeRC(env, container.object, value, result);
            }
        }
        return result;
    }

    void move(JNIEnv * env, const ContainerInfo& container, jint index1, jint index2) override {
        ContainerMove<QList, T>::function(env, container, index1, index2);
    }

    void insert(JNIEnv * env, const ContainerInfo& container, jint index, jint n, jobject value) override {
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        ContainerInsertN<QList, T>::function(env, container, index, n, value);
#else
        Q_ASSERT(n==1);
        ContainerInsertAt<QList, T>::function(env, container, index, value);
#endif
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if constexpr(ContainerContentType<T>::isContainer){
                Super::updateRC(env, container);
            }else{
                Super::addRC(env, container.object, value);
            }
        }
    }

    void insert(void* container, qsizetype index, qsizetype n, const void* value) override {
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        ContainerInsertN<QList, T>::function(container, index, n, value);
#else
        Q_ASSERT(n==1);
        ContainerInsertAt<QList, T>::function(container, index, value);
#endif
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if constexpr(ContainerContentType<T>::isContainer){
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
        ContainerClear<QList, T>::function(env, container);
        if constexpr(ContainerContentType<QList<T>>::needsReferenceCounting){
            Super::clearRC(env, container.object);
        }
    }

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    jint capacity(JNIEnv * env, const void* container) override {
        return ContainerCapacity<QList, T>::function(env, container);
    }

    void fill(JNIEnv * env, const ContainerInfo& container, jobject value, jint _size) override {
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if constexpr(ContainerContentType<T>::isContainer){
                ContainerFill<QList, T>::function(env, container, value, _size);
                Super::updateRC(env, container);
            }else{
                jint oldSize = size(env, container.container);
                ContainerFill<QList, T>::function(env, container, value, _size);
                for(;oldSize<_size;++oldSize){
                    Super::addRC(env, container.object, value);
                }
            }
        }else{
            ContainerFill<QList, T>::function(env, container, value, _size);
        }
    }

    void resize(JNIEnv * env, const ContainerInfo& container, jint newSize) override {
        ContainerResize<QList, T>::function(env, container, newSize);
    }

    void resize(void* container, qsizetype newSize) override {
        ContainerResize<QList, T>::function(container, newSize);
    }

    void squeeze(JNIEnv * env, const ContainerInfo& container) override {
        ContainerSqueeze<QList, T>::function(env, container);
    }
#endif
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
            jobject obj = qtjambi_scoped_cast<false,jobject,const T&>::cast(env, *current, nullptr, nullptr);
            ++current;
            return obj;
        }
        const void* next() override {
            const void* result = ContainerContentDeref<T>::deref(*current);
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
    };
public:
    std::unique_ptr<AbstractListAccess::ElementIterator> elementIterator(const void* container) override {
        return std::unique_ptr<AbstractListAccess::ElementIterator>(new ElementIterator<true>(this, *reinterpret_cast<const QList<T>*>(container)));
    }
    std::unique_ptr<AbstractListAccess::ElementIterator> elementIterator(void* container) override {
        return std::unique_ptr<AbstractListAccess::ElementIterator>(new ElementIterator<false>(this, *reinterpret_cast<QList<T>*>(container)));
    }
};

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#ifdef QVECTOR_H
template<typename T>
class QVectorAccess : public SequentialAccessSuperclassDecider<T,AbstractVectorAccess>::type{
    typedef typename SequentialAccessSuperclassDecider<T,AbstractVectorAccess>::type Super;
protected:
    QVectorAccess(){}
public:
    static AbstractVectorAccess* newInstance(){
        static QVectorAccess<T> instance;
        return &instance;
    }

    AbstractVectorAccess* clone() override{
        return this;
    }

    bool isDetached(const void* container) override{
        return reinterpret_cast<const QVector<T> *>(container)->isDetached();
    }

    void detach(const ContainerInfo& container) override{
        reinterpret_cast<QVector<T> *>(container.container)->detach();
    }

    bool isSharedWith(const void* container, const void* container2) override{
        return reinterpret_cast<const QVector<T> *>(container)->isSharedWith(*reinterpret_cast<const QVector<T> *>(container2));
    }

    void swap(JNIEnv *env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override{
        reinterpret_cast<QVector<T> *>(container.container)->swap(*reinterpret_cast<QVector<T> *>(container2.container));
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if(ReferenceCountingSetContainer* access = dynamic_cast<ReferenceCountingSetContainer*>(container2.access)){
                Q_UNUSED(access)
                if(container2.access!=this)
                    Super::swapRC(env, container, container2);
            }else{
                Super::updateRC(env, container);
            }
        }else{
            Q_UNUSED(env);
        }
    }

    const QMetaType& elementMetaType() override {
        static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(T));
        return type;
    }

    AbstractContainerAccess::DataType elementType() override{
        return ContainerContentType<T>::type;
    }

    AbstractContainerAccess* elementNestedContainerAccess() override {
        return ContainerContentType<T>::accessFactory();
    }
    bool hasNestedContainerAccess() override {
        return ContainerContentType<T>::isContainer;
    }
    bool hasNestedPointers() override {
        return ContainerContentType<T>::isContainer && ContainerContentType<T>::needsReferenceCounting;
    }

    void assign(void* container, const void* other) override {
        (*reinterpret_cast<QVector<T>*>(container)) = (*reinterpret_cast<const QVector<T>*>(other));
    }
    void assign(JNIEnv *env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override {
        (*reinterpret_cast<QVector<T>*>(container.container)) = (*reinterpret_cast<const QVector<T>*>(other.container));
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if(ReferenceCountingSetContainer* access = dynamic_cast<ReferenceCountingSetContainer*>(other.access)){
                Q_UNUSED(access)
                if(other.access!=this)
                    ReferenceCountingSetContainer::assignRC(env, container.object, other.object);
            }else{
                Super::updateRC(env, container);
            }
        }else{
            Q_UNUSED(env);
        }
    }
    size_t sizeOf() override {
        return sizeof(QVector<T>);
    }
    void* constructContainer(void* placement) override {
        return new(placement) QVector<T>();
    }
    void* constructContainer(void* placement, const void* copyOf) override {
        return new(placement) QVector<T>(*reinterpret_cast<const QVector<T>*>(copyOf));
    }
    void* constructContainer(JNIEnv *, void* placement, const ConstContainerAndAccessInfo&copyOf) override {
        return constructContainer(placement, copyOf.container);
    }
    bool destructContainer(void* container) override {
        reinterpret_cast<QVector<T>*>(container)->~QVector<T>();
        return true;
    }
    int registerContainer(const QByteArray& containerTypeName) override {
        return RegistryAPI::registerMetaType<QVector<T>>(containerTypeName, this);
    }

    jobject at(JNIEnv * env, const void* container, jint index) override {
        return ContainerAt<QVector, T>::function(env, container, index);
    }

    jobject value(JNIEnv * env, const void* container, jint index) override {
        return SequentialContainerValue<QVector, T>::function(env, container, index);
    }

    jobject value(JNIEnv * env, const void* container, jint index, jobject defaultValue) override {
        return ContainerValueDefault<QVector, T>::function(env, container, index, defaultValue);
    }

    jboolean startsWith(JNIEnv * env, const void* container, jobject value) override {
        return ContainerStartsWith<QVector, T>::function(env, container, value);
    }

    jint size(JNIEnv * env, const void* container) override {
        return ContainerSize<QVector, T>::function(env, container);
    }

    qsizetype size(const void* container) override {
        return ContainerSize<QVector, T>::function(container);
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        return ContainerEquals<QVector, T, ContainerAPI::getAsQVector>::function(env, container, other);
    }

    ContainerAndAccessInfo mid(JNIEnv * env, const ConstContainerAndAccessInfo& container, jint index1, jint index2) override {
        return ContainerMid<QVector, ContainerAPI::objectFromQVector, T>::function(env, container, index1, index2);
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

    jint capacity(JNIEnv * env, const void* container) override {
        return ContainerCapacity<QVector, T>::function(env, container);
    }

    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        return ContainerConstBegin<QVector, T>::function(env, container);
    }

    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        return ContainerConstEnd<QVector, T>::function(env, container);
    }

    void appendVector(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) override {
        ContainerAppendVector<T>::function(env, container, containerInfo);
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if constexpr(ContainerContentType<T>::isContainer){
                Super::updateRC(env, container);
            }else{
                Super::addAllRC(env, container.object, Super::findContainer(env, containerInfo.object));
            }
        }
    }

    void swapItemsAt(JNIEnv * env, const ContainerInfo& container, jint index1, jint index2) override {
        ContainerSwap<QVector, T>::function(env, container, index1, index2);
    }

    void reserve(JNIEnv * env, const ContainerInfo& container, jint size) override {
        ContainerReserve<QVector, T>::function(env, container, size);
    }

    void replace(JNIEnv * env, const ContainerInfo& container, jint index, jobject value) override {
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if constexpr(ContainerContentType<T>::isContainer){
                ContainerReplace<QVector, T>::function(env, container, index, value);
                Super::updateRC(env, container);
            }else{
                jobject oldValue = at(env, container.container, index);
                ContainerReplace<QVector, T>::function(env, container, index, value);
                if(oldValue && !contains(env, container.container, oldValue))
                    Super::removeRC(env, container.object, oldValue);
                if(value)
                    Super::addRC(env, container.object, value);
            }
        }else{
            ContainerReplace<QVector, T>::function(env, container, index, value);
        }
    }

    jint removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) override {
        jint result = ContainerRemoveAll<QVector, T>::function(env, container, value);
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if constexpr(ContainerContentType<T>::isContainer){
                if(result>0)
                    Super::updateRC(env, container);
            }else{
                Super::removeRC(env, container.object, value, result);
            }
        }
        return result;
    }

    void move(JNIEnv * env, const ContainerInfo& container, jint index1, jint index2) override {
        ContainerMove<QVector, T>::function(env, container, index1, index2);
    }

    void clear(JNIEnv * env, const ContainerInfo& container) override {
        ContainerClear<QVector, T>::function(env, container);
        if constexpr(ContainerContentType<QVector<T>>::needsReferenceCounting){
            Super::clearRC(env, container.object);
        }
    }

    void fill(JNIEnv * env, const ContainerInfo& container, jobject value, jint _size) override {
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if constexpr(ContainerContentType<T>::isContainer){
                ContainerFill<QVector, T>::function(env, container, value, _size);
                Super::updateRC(env, container);
            }else{
                jint oldSize = size(env, container.container);
                ContainerFill<QVector, T>::function(env, container, value, _size);
                for(;oldSize<_size;++oldSize){
                    Super::addRC(env, container.object, value);
                }
            }
        }else {
            ContainerFill<QVector, T>::function(env, container, value, _size);
        }
    }

    void remove(JNIEnv * env, const ContainerInfo& container, jint index, jint n) override {
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if constexpr(ContainerContentType<T>::isContainer){
                ContainerRemoveN<QVector, T>::function(env, container, index, n);
                Super::updateRC(env, container);
            }else{
                if(n==1){
                    jobject oldValue = at(env, container.container, index);
                    ContainerRemoveN<QVector, T>::function(env, container, index, n);
                    Super::removeRC(env, container.object, oldValue);
                }else{
                    jint _size = size(env, container.container);
                    jobject removedValues = QtJambiAPI::newJavaArrayList(env);
                    for(jint i = index; i<=index+n && i<_size; ++i){
                        Java::Runtime::Collection::add(env, removedValues, at(env, container.container, i));
                    }
                    ContainerRemoveN<QVector, T>::function(env, container, index, n);
                    jobject iter = Java::Runtime::Collection::iterator(env, removedValues);
                    while(Java::Runtime::Iterator::hasNext(env, iter)){
                        jobject value = Java::Runtime::Iterator::next(env, iter);
                        Super::removeRC(env, container.object, value);
                    }
                }
            }
        }else{
            ContainerRemoveN<QVector, T>::function(env, container, index, n);
        }
    }

    void insert(JNIEnv * env, const ContainerInfo& container, jint index, jint n, jobject value) override {
        ContainerInsertN<QVector, T>::function(env, container, index, n, value);
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if constexpr(ContainerContentType<T>::isContainer){
                Super::updateRC(env, container);
            }else{
                Super::addRC(env, container.object, value);
            }
        }
    }
    void resize(JNIEnv * env, const ContainerInfo& container, jint newSize) override {
        ContainerResize<QVector, T>::function(env, container, newSize);
    }
    void squeeze(JNIEnv * env, const ContainerInfo& container) override {
        ContainerSqueeze<QVector, T>::function(env, container);
    }

    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override {
        return ContainerBegin<QVector, T>::function(env, container);
    }

    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override {
        return ContainerEnd<QVector, T>::function(env, container);
    }
private:
    template<bool is_const>
    class ElementIterator : public AbstractVectorAccess::ElementIterator{
        using Container = std::conditional_t<is_const, const QVector<T>, QVector<T>>;
        using iterator = decltype(std::declval<Container>().begin());
        QVectorAccess<T>* m_access;
        iterator current;
        iterator end;
        ElementIterator(const ElementIterator& other)
            :m_access(other.m_access),
            current(other.current),
            end(other.end) {}
    protected:
        AbstractSequentialAccess* access() override { return m_access; }
    public:
        ElementIterator(QVectorAccess<T>* _access, Container& container)
            : m_access(_access),
            current(container.begin()),
            end(container.end()) {}
        ~ElementIterator() override {};
        bool hasNext() override {return current!=end;};
        jobject next(JNIEnv * env) override {
            jobject obj = qtjambi_scoped_cast<false,jobject,const T&>::cast(env, *current, nullptr, nullptr);
            ++current;
            return obj;
        }
        const void* next() override {
            const void* result = ContainerContentDeref<T>::deref(*current);
            ++current;
            return result;
        };
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
    };
public:
    std::unique_ptr<AbstractVectorAccess::ElementIterator> elementIterator(const void* container) override {
        return std::unique_ptr<AbstractVectorAccess::ElementIterator>(new ElementIterator<true>(this, *reinterpret_cast<const QVector<T>*>(container)));
    }
    std::unique_ptr<AbstractVectorAccess::ElementIterator> elementIterator(void* container) override {
        return std::unique_ptr<AbstractVectorAccess::ElementIterator>(new ElementIterator<false>(this, *reinterpret_cast<QVector<T>*>(container)));
    }
};
#endif // def QVECTOR_H

#ifdef QLINKEDLIST_H
template<typename T>
class QLinkedListAccess : public SequentialAccessSuperclassDecider<T,AbstractLinkedListAccess>::type{
    typedef typename SequentialAccessSuperclassDecider<T,AbstractLinkedListAccess>::type Super;
protected:
    QLinkedListAccess(){}
public:
    static AbstractLinkedListAccess* newInstance(){
        static QLinkedListAccess<T> instance;
        return &instance;
    }

    AbstractLinkedListAccess* clone() override{
        return this;
    }

    bool isDetached(const void* container) override{
        return reinterpret_cast<const QLinkedList<T> *>(container)->isDetached();
    }

    void detach(const ContainerInfo& container) override{
        reinterpret_cast<QLinkedList<T> *>(container.container)->detach();
    }

    bool isSharedWith(const void* container, const void* container2) override{
        return reinterpret_cast<const QLinkedList<T> *>(container)->isSharedWith(*reinterpret_cast<const QLinkedList<T> *>(container2));
    }

    void swap(JNIEnv *env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override{
        reinterpret_cast<QLinkedList<T> *>(container.container)->swap(*reinterpret_cast<QLinkedList<T> *>(container2.container));
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if(ReferenceCountingSetContainer* access = dynamic_cast<ReferenceCountingSetContainer*>(container2.access)){
                Q_UNUSED(access)
                if(container2.access!=this)
                    Super::swapRC(env, container, container2);
            }else{
                Super::updateRC(env, container);
            }
        }else{
            Q_UNUSED(env);
        }
    }

    const QMetaType& elementMetaType() override {
        static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(T));
        return type;
    }

    AbstractContainerAccess::DataType elementType() override{
        return ContainerContentType<T>::type;
    }

    AbstractContainerAccess* elementNestedContainerAccess() override {
        return ContainerContentType<T>::accessFactory();
    }
    bool hasNestedContainerAccess() override {
        return ContainerContentType<T>::isContainer;
    }
    bool hasNestedPointers() override {
        return ContainerContentType<T>::isContainer && ContainerContentType<T>::needsReferenceCounting;
    }

    void assign(void* container, const void* other) override {
        (*reinterpret_cast<QLinkedList<T>*>(container)) = (*reinterpret_cast<const QLinkedList<T>*>(other));
    }
    void assign(JNIEnv *env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override {
        (*reinterpret_cast<QLinkedList<T>*>(container.container)) = (*reinterpret_cast<const QLinkedList<T>*>(other.container));
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if(ReferenceCountingSetContainer* access = dynamic_cast<ReferenceCountingSetContainer*>(other.access)){
                Q_UNUSED(access)
                if(other.access!=this)
                    ReferenceCountingSetContainer::assignRC(env, container.object, other.object);
            }else{
                Super::updateRC(env, container);
            }
        }else{
            Q_UNUSED(env);
        }
    }
    size_t sizeOf() override {
        return sizeof(QLinkedList<T>);
    }
    void* constructContainer(void* placement) override {
        return new(placement) QLinkedList<T>();
    }
    void* constructContainer(void* placement, const void* copyOf) override {
        return new(placement) QLinkedList<T>(*reinterpret_cast<const QLinkedList<T>*>(copyOf));
    }
    void* constructContainer(JNIEnv *, void* placement, const ConstContainerAndAccessInfo& copyOf) override {
        return constructContainer(placement, copyOf.container);
    }
    bool destructContainer(void* container) override {
        reinterpret_cast<QLinkedList<T>*>(container)->~QLinkedList<T>();
        return true;
    }
    int registerContainer(const QByteArray& containerTypeName) override {
        return RegistryAPI::registerMetaType<QLinkedList<T>>(containerTypeName, this);
    }

    jobject first(JNIEnv * env, const void* container) override {
        return ContainerFirst<QLinkedList, T>::function(env, container);
    }

    jobject last(JNIEnv * env, const void* container) override {
        return ContainerLast<QLinkedList, T>::function(env, container);
    }

    jboolean contains(JNIEnv * env, const void* container, jobject value) override {
        return ContainerContains<QLinkedList, T>::function(env, container, value);
    }

    jint count(JNIEnv * env, const void* container, jobject value) override {
        return ContainerCountObject<QLinkedList, T>::function(env, container, value);
    }

    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        return ContainerConstBegin<QLinkedList, T>::function(env, container);
    }

    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        return ContainerConstEnd<QLinkedList, T>::function(env, container);
    }

    jboolean endsWith(JNIEnv * env, const void* container, jobject value) override {
        return ContainerEndsWith<QLinkedList, T>::function(env, container, value);
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        return ContainerEquals<QLinkedList, T, ContainerAPI::getAsQLinkedList>::function(env, container, other);
    }

    jint size(JNIEnv * env, const void* container) override {
        return ContainerSize<QLinkedList, T>::function(env, container);
    }

    qsizetype size(const void* container) override {
        return ContainerSize<QLinkedList, T>::function(container);
    }

    jboolean startsWith(JNIEnv * env, const void* container, jobject value) override {
        return ContainerStartsWith<QLinkedList, T>::function(env, container, value);
    }

    void append(JNIEnv * env, const ContainerInfo& container, jobject value) override {
        ContainerAppend<QLinkedList, T>::function(env, container, value);
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if constexpr(ContainerContentType<T>::isContainer){
                Super::updateRC(env, container);
            }else{
                Super::addRC(env, container.object, value);
            }
        }
    }

    void clear(JNIEnv * env, const ContainerInfo& container) override {
        ContainerClear<QLinkedList, T>::function(env, container);
        if constexpr(ContainerContentType<QLinkedList<T>>::needsReferenceCounting){
            Super::clearRC(env, container.object);
        }
    }

    void prepend(JNIEnv * env, const ContainerInfo& container, jobject value) override {
        ContainerPrepend<QLinkedList, T>::function(env, container, value);
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if constexpr(ContainerContentType<T>::isContainer){
                Super::updateRC(env, container);
            }else{
                Super::addRC(env, container.object, value);
            }
        }
    }

    void removeFirst(JNIEnv * env, const ContainerInfo& container) override {
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if constexpr(ContainerContentType<T>::isContainer){
                ContainerRemoveFirst<QLinkedList, T>::function(env, container);
                Super::updateRC(env, container);
            }else{
                jobject oldValue = first(env, container.container);
                ContainerRemoveFirst<QLinkedList, T>::function(env, container);
                Super::removeRC(env, container.object, oldValue);
            }
        }else{
            ContainerRemoveFirst<QLinkedList, T>::function(env, container);
        }
    }

    jint removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) override {
        jint result = ContainerRemoveAll<QLinkedList, T>::function(env, container, value);
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if constexpr(ContainerContentType<T>::isContainer){
                if(result>0)
                    Super::updateRC(env, container);
            }else{
                Super::removeRC(env, container.object, value, result);
            }
        }
        return result;
    }

    void removeLast(JNIEnv * env, const ContainerInfo& container) override {
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if constexpr(ContainerContentType<T>::isContainer){
                ContainerRemoveLast<QLinkedList, T>::function(env, container);
                Super::updateRC(env, container);
            }else{
                jobject oldValue = last(env, container.container);
                ContainerRemoveLast<QLinkedList, T>::function(env, container);
                Super::removeRC(env, container.object, oldValue);
            }
        }else{
            ContainerRemoveLast<QLinkedList, T>::function(env, container);
        }
    }

    jboolean removeOne(JNIEnv * env, const ContainerInfo& container, jobject value) override {
        jboolean result = ContainerRemoveOne<QLinkedList, T>::function(env, container, value);
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if constexpr(ContainerContentType<T>::isContainer){
                if(result)
                    Super::updateRC(env, container);
            }else{
                if(result)
                    Super::removeRC(env, container.object, value);
            }
        }
        return result;
    }

    jobject takeFirst(JNIEnv * env, const ContainerInfo& container) override {
        jobject result = ContainerTakeFirst<QLinkedList, T>::function(env, container);
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if constexpr(ContainerContentType<T>::isContainer){
                Super::updateRC(env, container);
            }else{
                Super::removeRC(env, container.object, result);
            }
        }
        return result;
    }

    jobject takeLast(JNIEnv * env, const ContainerInfo& container) override {
        jobject result = ContainerTakeLast<QLinkedList, T>::function(env, container);
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if constexpr(ContainerContentType<T>::isContainer){
                Super::updateRC(env, container);
            }else{
                Super::removeRC(env, container.object, result);
            }
        }
        return result;
    }

    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override {
        return ContainerBegin<QLinkedList, T>::function(env, container);
    }

    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override {
        return ContainerEnd<QLinkedList, T>::function(env, container);
    }
private:
    class ElementIterator : public AbstractLinkedListAccess::ElementIterator{
        QLinkedListAccess<T>* m_access;
        typename QLinkedList<T>::ConstIterator current;
        typename QLinkedList<T>::ConstIterator end;
        ElementIterator(const ElementIterator& other)
            :m_access(other.m_access),
            current(other.current),
            end(other.end) {}
    protected:
        AbstractSequentialAccess* access() override { return m_access; }
    public:
        ElementIterator(QLinkedListAccess<T>* _access, const QLinkedList<T>& container)
            : m_access(_access),
            current(container.constBegin()),
            end(container.constEnd()) {}
        ~ElementIterator() override {};
        bool hasNext() override {return current!=end;};
        jobject next(JNIEnv * env) override {
            jobject obj = qtjambi_scoped_cast<false,jobject,const T&>::cast(env, *current, nullptr, nullptr);
            ++current;
            return obj;
        }
        const void* next() override {
            const void* result = ContainerContentDeref<T>::deref(*current);
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
    };
public:
    std::unique_ptr<AbstractLinkedListAccess::ElementIterator> elementIterator(const void* container) override {
        return std::unique_ptr<AbstractLinkedListAccess::ElementIterator>(new ElementIterator(this, *reinterpret_cast<const QLinkedList<T>*>(container)));
    }
    std::unique_ptr<AbstractLinkedListAccess::ElementIterator> elementIterator(void* container) override {
        return std::unique_ptr<AbstractLinkedListAccess::ElementIterator>(new ElementIterator(this, *reinterpret_cast<QLinkedList<T>*>(container)));
    }
};
#endif
#endif

template<template<typename T> class Container, typename T>
struct ContainerValues{
    static ContainerAndAccessInfo function(JNIEnv * env, const void* ptr) {
        ContainerAndAccessInfo result;
        const Container<T>  *container = static_cast<const Container<T> *>(ptr);
        result.container = new QList<T>(container->values());
        result.access = QListAccess<T>::newInstance();
        result.object = ContainerAPI::objectFromQList(env, result.container, result.access);
        return result;
    }
};

template<typename T>
class QSetAccess : public SequentialAccessSuperclassDecider<T,AbstractSetAccess>::type{
    typedef typename SequentialAccessSuperclassDecider<T,AbstractSetAccess>::type Super;
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
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if(ReferenceCountingSetContainer* access = dynamic_cast<ReferenceCountingSetContainer*>(container2.access)){
                Q_UNUSED(access)
                if(container2.access!=this)
                    Super::swapRC(env, container, container2);
            }else{
                Super::updateRC(env, container);
            }
        }else{
            Q_UNUSED(env);
        }
    }

    const QMetaType& elementMetaType() override {
        static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(T));
        return type;
    }

    AbstractContainerAccess::DataType elementType() override{
        return ContainerContentType<T>::type;
    }

    AbstractContainerAccess* elementNestedContainerAccess() override {
        return ContainerContentType<T>::accessFactory();
    }
    bool hasNestedContainerAccess() override {
        return ContainerContentType<T>::isContainer;
    }
    bool hasNestedPointers() override {
        return ContainerContentType<T>::isContainer && ContainerContentType<T>::needsReferenceCounting;
    }

    void assign(void* container, const void* other) override {
        (*reinterpret_cast<QSet<T>*>(container)) = (*reinterpret_cast<const QSet<T>*>(other));
    }
    void assign(JNIEnv *env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override {
        (*reinterpret_cast<QSet<T>*>(container.container)) = (*reinterpret_cast<const QSet<T>*>(other.container));
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if(ReferenceCountingSetContainer* access = dynamic_cast<ReferenceCountingSetContainer*>(other.access)){
                Q_UNUSED(access)
                if(other.access!=this)
                    ReferenceCountingSetContainer::assignRC(env, container.object, other.object);
            }else{
                Super::updateRC(env, container);
            }
        }else{
            Q_UNUSED(env);
        }
    }
    size_t sizeOf() override {
        return sizeof(QSet<T>);
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* move) override {
        return new(placement) QSet<T>(std::move(*reinterpret_cast<const QSet<T>*>(move)));
    }
    void* constructContainer(JNIEnv *, void* placement, const ContainerAndAccessInfo& move) override {
        return constructContainer(placement, move.container);
    }
#endif
    bool destructContainer(void* container) override {
        reinterpret_cast<QSet<T>*>(container)->~QSet<T>();
        return true;
    }

    int registerContainer(const QByteArray& containerTypeName) override {
        return RegistryAPI::registerMetaType<QSet<T>>(containerTypeName, this);
    }

    jint capacity(JNIEnv * env, const void* container) override {
        return ContainerCapacity<QSet, T>::function(env, container);
    }

    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        return ContainerConstBegin<QSet, T>::function(env, container);
    }

    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        return ContainerConstEnd<QSet, T>::function(env, container);
    }

    jboolean contains(JNIEnv * env, const void* container, jobject value) override {
        return ContainerContains<QSet, T>::function(env, container, value);
    }

    jboolean intersects(JNIEnv * env, const void* container, jobject other) override {
        return ContainerIntersects<QSet, T, ContainerAPI::getAsQSet>::function(env, container, other);
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        return ContainerEquals<QSet, T, ContainerAPI::getAsQSet>::function(env, container, other);
    }

    jint size(JNIEnv * env, const void* container) override {
        return ContainerSize<QSet, T>::function(env, container);
    }

    qsizetype size(const void* container) override {
        return ContainerSize<QSet, T>::function(container);
    }

    ContainerAndAccessInfo values(JNIEnv * env, const ConstContainerInfo& container) override {
        return ContainerValues<QSet, T>::function(env, container.container);
    }

    void clear(JNIEnv * env, const ContainerInfo& container) override {
        ContainerClear<QSet, T>::function(env, container);
        if constexpr(ContainerContentType<QSet<T>>::needsReferenceCounting){
            Super::clearRC(env, container.object);
        }
    }
    void insert(JNIEnv * env, const ContainerInfo& container, jobject value) override {
        ContainerInsert<QSet, T>::function(env, container, value);
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if constexpr(ContainerContentType<T>::isContainer){
                Super::updateRC(env, container);
            }else{
                Super::addRC(env, container.object, value);
            }
        }
    }
    void intersect(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override {
        ContainerIntersect<QSet, T, ContainerAPI::getAsQSet>::function(env, container, other);
        if constexpr(ContainerContentType<QSet<T>>::needsReferenceCounting){
            Super::clearRC(env, container.object);
        }
    }
    jboolean remove(JNIEnv * env, const ContainerInfo& container, jobject value) override {
        jboolean result = ContainerRemove<QSet, T>::function(env, container, value);
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if constexpr(ContainerContentType<T>::isContainer){
                if(result)
                    Super::updateRC(env, container);
            }else{
                if(result)
                    Super::removeRC(env, container.object, value);
            }
        }
        return result;
    }
    void reserve(JNIEnv * env, const ContainerInfo& container, jint newSize) override {
        ContainerReserve<QSet, T>::function(env, container, newSize);
    }
    void subtract(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override {
        ContainerSubtract<QSet, T, ContainerAPI::getAsQSet>::function(env, container, other);
        if constexpr(ContainerContentType<QSet<T>>::needsReferenceCounting){
            Super::clearRC(env, container.object);
        }
    }
    void unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override {
        ContainerUnite<QSet, T, ContainerAPI::getAsQSet>::function(env, container, other);
        if constexpr(ContainerContentType<QSet<T>>::needsReferenceCounting){
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
            jobject obj = qtjambi_scoped_cast<false,jobject,const T&>::cast(env, *current, nullptr, nullptr);
            ++current;
            return obj;
        }
        const void* next() override {
            const void* result = ContainerContentDeref<T>::deref(*current);
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
class QSpanAccess : public SequentialAccessSuperclassDecider<T,AbstractSpanAccess>::type{
    typedef typename SequentialAccessSuperclassDecider<T,AbstractSpanAccess>::type Super;
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
        static QMetaType type(QTJAMBI_METATYPE_FROM_TYPE(T));
        return type;
    }

    AbstractContainerAccess::DataType elementType() override{
        return ContainerContentType<T>::type;
    }

    AbstractContainerAccess* elementNestedContainerAccess() override {
        return ContainerContentType<T>::accessFactory();
    }
    bool hasNestedContainerAccess() override {
        return ContainerContentType<T>::isContainer;
    }
    bool hasNestedPointers() override {
        return ContainerContentType<T>::isContainer && ContainerContentType<T>::needsReferenceCounting;
    }

    void assign(void* container, const void* other) override {
        (*reinterpret_cast<QSpan<T,E>*>(container)) = (*reinterpret_cast<const QSpan<T,E>*>(other));
    }
    void assign(JNIEnv *env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override {
        (*reinterpret_cast<QSpan<T,E>*>(container.container)) = (*reinterpret_cast<const QSpan<T,E>*>(other.container));
        if constexpr(ContainerContentType<T>::needsReferenceCounting){
            if(ReferenceCountingSetContainer* access = dynamic_cast<ReferenceCountingSetContainer*>(other.access)){
                Q_UNUSED(access)
                if(other.access!=this)
                    ReferenceCountingSetContainer::assignRC(env, container.object, other.object);
            }else{
                Super::updateRC(env, container);
            }
        }else{
            Q_UNUSED(env);
        }
    }
    size_t sizeOf() override {
        return sizeof(QSpan<T,E>);
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* move) override {
        return new(placement) QSpan<T,E>(std::move(*reinterpret_cast<const QSpan<T,E>*>(move)));
    }
    void* constructContainer(JNIEnv *, void* placement, const ContainerAndAccessInfo& move) override {
        return constructContainer(placement, move.container);
    }
#endif
    bool destructContainer(void* container) override {
        reinterpret_cast<QSpan<T,E>*>(container)->~QSpan<T,E>();
        return true;
    }

    int registerContainer(const QByteArray& containerTypeName) override {
        return RegistryAPI::registerMetaType<QSpan<T,E>>(containerTypeName, this);
    }

    jint size(JNIEnv *, const void* container) override {
        const QSpan<T,E> *span = static_cast<const QSpan<T,E> *>(container);
        return jint(span->size());
    }

    qsizetype size(const void* container) override {
        const QSpan<T,E> *span = static_cast<const QSpan<T,E> *>(container);
        return span->size();
    }

    jint size_bytes(JNIEnv *, const void* container) override {
        const QSpan<T,E> *span = static_cast<const QSpan<T,E> *>(container);
        return jint(span->size_bytes());
    }

    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        const QSpan<T,E> *span = static_cast<const QSpan<T,E> *>(container.container);
        return qtjambi_sequential_iterator_caster<decltype(span->cbegin())>::cast(env, container.nativeId, span->cbegin());
    }

    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        const QSpan<T,E> *span = static_cast<const QSpan<T,E> *>(container.container);
        return qtjambi_sequential_iterator_caster<decltype(span->cend())>::cast(env, container.nativeId, span->cend());
    }

    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override {
        QSpan<T,E> *span = static_cast<QSpan<T,E> *>(container.container);
        return qtjambi_sequential_iterator_caster<decltype(span->begin())>::cast(env, container.nativeId, span->begin());
    }

    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override {
        QSpan<T,E> *span = static_cast<QSpan<T,E> *>(container.container);
        return qtjambi_sequential_iterator_caster<decltype(span->end())>::cast(env, container.nativeId, span->end());
    }

    jobject get(JNIEnv * env, const void* container, jint index) override {
        const QSpan<T,E> &span = *static_cast<const QSpan<T,E> *>(container);
        return qtjambi_scoped_cast<false,jobject,const T>::cast(env, span[index], nullptr, nullptr);
    }

    const void* get(const void* container, qsizetype index) override {
        const QSpan<T,E> &span = *static_cast<const QSpan<T,E> *>(container);
        return &span[index];
    }

    bool set(JNIEnv * env, const ContainerInfo& container, jint index, jobject value) override {
        if constexpr(std::is_const_v<T>){
            Q_UNUSED(container);
            Q_UNUSED(index);
            Q_UNUSED(value);
            Q_UNUSED(env);
            return false;
        }else{
            const QSpan<T,E> &span = *static_cast<const QSpan<T,E> *>(container.container);
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                if constexpr(ContainerContentType<T>::isContainer){
                    span[index] = qtjambi_scoped_cast<false,T,jobject>::cast(env, value, nullptr, nullptr);
                    Super::updateRC(env, container);
                }else{
                    jobject oldValue = qtjambi_scoped_cast<false,jobject,const T>::cast(env, span[index], nullptr, nullptr);
                    span[index] = qtjambi_scoped_cast<false,T,jobject>::cast(env, value, nullptr, nullptr);
                    Super::removeRC(env, container.object, oldValue);
                    Super::addRC(env, container.object, value);
                }
            }else{
                span[index] = qtjambi_scoped_cast<false,T,jobject>::cast(env, value, nullptr, nullptr);
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
            if constexpr(ContainerContentType<T>::needsReferenceCounting){
                if(JniEnvironment env{100}){
                    if(jobject object = QtJambiAPI::findObject(env, container)){
                        if constexpr(ContainerContentType<T>::isContainer){
                            span[index] = *static_cast<const T *>(value);
                            Super::updateRC(env, ContainerInfo{object, container});
                            return true;
                        }else{
                            const T& replace = *static_cast<const T *>(value);
                            jobject oldValue = qtjambi_scoped_cast<false,jobject,const T>::cast(env, span[index], nullptr, nullptr);
                            jobject newValue = qtjambi_scoped_cast<false,jobject,const T>::cast(env, replace, nullptr, nullptr);
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
            jobject obj = qtjambi_scoped_cast<false,jobject,const T&>::cast(env, *current, nullptr, nullptr);
            ++current;
            return obj;
        }
        const void* next() override {
            const void* result = ContainerContentDeref<T>::deref(*current);
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

template<typename T>
struct ContainerContentType<QList<T>>{
    static constexpr AbstractContainerAccess::DataType type = AbstractContainerAccess::Value;
    static constexpr bool isContainer = true;
    static constexpr bool needsReferenceCounting = (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask) || ContainerContentType<T>::needsReferenceCounting;
    static constexpr bool needsOwnerCheck = ContainerContentType<T>::needsOwnerCheck;
    static constexpr AbstractContainerAccess* accessFactory(){return QListAccess<T>::newInstance();};
};

template<typename T>
struct ContainerContentType<QSet<T>>{
    static constexpr AbstractContainerAccess::DataType type = AbstractContainerAccess::Value;
    static constexpr bool isContainer = true;
    static constexpr bool needsReferenceCounting = (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask) || ContainerContentType<T>::needsReferenceCounting;
    static constexpr bool needsOwnerCheck = ContainerContentType<T>::needsOwnerCheck;
    static constexpr AbstractContainerAccess* accessFactory(){return QSetAccess<T>::newInstance();};
};

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
template<typename T>
struct ContainerContentType<QVector<T>>{
    static constexpr AbstractContainerAccess::DataType type = AbstractContainerAccess::Value;
    static constexpr bool isContainer = true;
    static constexpr bool needsReferenceCounting = (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask) || ContainerContentType<T>::needsReferenceCounting;
    static constexpr bool needsOwnerCheck = ContainerContentType<T>::needsOwnerCheck;
    static constexpr AbstractContainerAccess* accessFactory(){return QVectorAccess<T>::newInstance();};
};

#ifdef QLINKEDLIST_H
template<typename T>
struct ContainerContentType<QLinkedList<T>>{
    static constexpr AbstractContainerAccess::DataType type = AbstractContainerAccess::Value;
    static constexpr bool isContainer = true;
    static constexpr bool needsReferenceCounting = (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask) || ContainerContentType<T>::needsReferenceCounting;
    static constexpr bool needsOwnerCheck = ContainerContentType<T>::needsOwnerCheck;
    static constexpr AbstractContainerAccess* accessFactory(){return QLinkedListAccess<T>::newInstance();};
};
#endif

template<template<typename T> class Container, typename T>
struct ContainerToList{
    static jobject function(JNIEnv * env, const void* ptr) {
        const Container<T>  *container = static_cast<const Container<T> *>(ptr);
        return qtjambi_scoped_cast<false,jobject,const QList<T>>::cast(env, container->toList(), nullptr, nullptr);
    }
};
#elif QT_VERSION >= QT_VERSION_CHECK(6,7,0)
template<typename T, std::size_t E>
struct ContainerContentType<QSpan<T,E>>{
    static constexpr AbstractContainerAccess::DataType type = AbstractContainerAccess::Value;
    static constexpr bool isContainer = true;
    static constexpr bool needsReferenceCounting = (ContainerContentType<T>::type & AbstractContainerAccess::PointersMask) || ContainerContentType<T>::needsReferenceCounting;
    static constexpr bool needsOwnerCheck = ContainerContentType<T>::needsOwnerCheck;
    static constexpr AbstractContainerAccess* accessFactory(){return QSpanAccess<T,E>::newInstance();};
};
#endif

} // namespace QtJambiPrivate

#endif // QTJAMBI_CAST_IMPL_CONTAINER_SEQUENTIAL_H
