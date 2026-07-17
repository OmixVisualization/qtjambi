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


#ifndef CONTAINERAPI_H
#define CONTAINERAPI_H

#include <functional>
#include <typeinfo>

#include "global.h"
#include "registryapi.h"
#include "typetests.h"

enum class QtJambiNativeID : jlong;

class AbstractListAccess;
#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
class AbstractSpanAccess;
#endif //QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)

namespace ContainerAPI{

QTJAMBI_EXPORT bool testQStack(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType);
QTJAMBI_EXPORT bool testQList(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType);
QTJAMBI_EXPORT bool testQSet(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType);
QTJAMBI_EXPORT bool testQQueue(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType);
QTJAMBI_EXPORT bool testQMultiMap(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType);
QTJAMBI_EXPORT bool testQMultiHash(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType);
QTJAMBI_EXPORT bool testQMap(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType);
QTJAMBI_EXPORT bool testQHash(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType);

QTJAMBI_EXPORT bool testQStack(JNIEnv *env, jobject collection, const std::type_info& expectedElementType, const QMetaType& expectedElementMetaType);
QTJAMBI_EXPORT bool testQList(JNIEnv *env, jobject collection, const std::type_info& expectedElementType, const QMetaType& expectedElementMetaType);
QTJAMBI_EXPORT bool testQSet(JNIEnv *env, jobject collection, const std::type_info& expectedElementType, const QMetaType& expectedElementMetaType);
QTJAMBI_EXPORT bool testQQueue(JNIEnv *env, jobject collection, const std::type_info& expectedElementType, const QMetaType& expectedElementMetaType);
QTJAMBI_EXPORT bool testQMultiMap(JNIEnv *env, jobject mapObject, const std::type_info& expectedKeyType, const QMetaType& expectedKeyMetaType, const std::type_info& expectedValueType, const QMetaType& expectedValueMetaType);
QTJAMBI_EXPORT bool testQMultiHash(JNIEnv *env, jobject mapObject, const std::type_info& expectedKeyType, const QMetaType& expectedKeyMetaType, const std::type_info& expectedValueType, const QMetaType& expectedValueMetaType);
QTJAMBI_EXPORT bool testQMap(JNIEnv *env, jobject mapObject, const std::type_info& expectedKeyType, const QMetaType& expectedKeyMetaType, const std::type_info& expectedValueType, const QMetaType& expectedValueMetaType);
QTJAMBI_EXPORT bool testQHash(JNIEnv *env, jobject mapObject, const std::type_info& expectedKeyType, const QMetaType& expectedKeyMetaType, const std::type_info& expectedValueType, const QMetaType& expectedValueMetaType);

QTJAMBI_EXPORT bool getAsQStack(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQStack(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType, void* &pointer, AbstractContainerAccess*& access);
QTJAMBI_EXPORT bool getAsQList(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQList(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType, void* &pointer, AbstractContainerAccess*& access);
QTJAMBI_EXPORT bool getAsQSet(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQSet(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType, void* &pointer, AbstractContainerAccess*& access);
QTJAMBI_EXPORT bool getAsQQueue(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQQueue(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType, void* &pointer, AbstractContainerAccess*& access);
QTJAMBI_EXPORT bool getAsQMultiMap(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQMultiMap(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType, void* &pointer, AbstractContainerAccess*& access);
QTJAMBI_EXPORT bool getAsQMultiHash(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQMultiHash(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType, void* &pointer, AbstractContainerAccess*& access);
QTJAMBI_EXPORT bool getAsQMap(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQMap(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType, void* &pointer, AbstractContainerAccess*& access);
QTJAMBI_EXPORT bool getAsQHash(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQHash(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType, void* &pointer, AbstractContainerAccess*& access);

QTJAMBI_EXPORT bool getAsQStack(JNIEnv *env, jobject collection, const std::type_info& expectedElementType, const QMetaType& expectedElementMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQList(JNIEnv *env, jobject collection, const std::type_info& expectedElementType, const QMetaType& expectedElementMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQSet(JNIEnv *env, jobject collection, const std::type_info& expectedElementType, const QMetaType& expectedElementMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQQueue(JNIEnv *env, jobject collection, const std::type_info& expectedElementType, const QMetaType& expectedElementMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQMultiMap(JNIEnv *env, jobject mapObject, const std::type_info& expectedKeyType, const QMetaType& expectedKeyMetaType, const std::type_info& expectedValueType, const QMetaType& expectedValueMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQMultiHash(JNIEnv *env, jobject mapObject, const std::type_info& expectedKeyType, const QMetaType& expectedKeyMetaType, const std::type_info& expectedValueType, const QMetaType& expectedValueMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQMap(JNIEnv *env, jobject mapObject, const std::type_info& expectedKeyType, const QMetaType& expectedKeyMetaType, const std::type_info& expectedValueType, const QMetaType& expectedValueMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQHash(JNIEnv *env, jobject mapObject, const std::type_info& expectedKeyType, const QMetaType& expectedKeyMetaType, const std::type_info& expectedValueType, const QMetaType& expectedValueMetaType, void* &pointer);

#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
QTJAMBI_EXPORT bool testQSpan(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType);
QTJAMBI_EXPORT bool testQSpan(JNIEnv *env, jobject collection, const std::type_info& expectedElementType, const QMetaType& expectedElementMetaType);
QTJAMBI_EXPORT bool getAsQSpan(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQSpan(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType, void* &pointer, AbstractContainerAccess*& access);
QTJAMBI_EXPORT bool getAsQSpan(JNIEnv *env, jobject collection, const std::type_info& expectedElementType, const QMetaType& expectedElementMetaType, void* &pointer);

template<typename T>
bool getAsQSpan(JNIEnv *env, jobject collection, QList<T> * &pointer, AbstractContainerAccess*& access){
    return getAsQSpan(env, collection, QMetaType::fromType<std::remove_cv_t<T>>(), reinterpret_cast<void*&>(pointer), access);
}
template<typename T>
bool getAsQSpan(JNIEnv *env, jobject collection, QList<T> * &pointer){
    return getAsQSpan(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QMetaType::fromType<std::remove_cv_t<T>>(), reinterpret_cast<void*&>(pointer));
}
template<typename T>
bool testQSpan(JNIEnv *env, jobject collection){
    return testQSpan(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QMetaType::fromType<std::remove_cv_t<T>>());
}

QTJAMBI_EXPORT jobject objectFromQSpan(JNIEnv *__jni_env,
                                       void*& listPtr,
                                       AbstractContainerAccess*& setAccess);
QTJAMBI_EXPORT jobject objectFromQSpan(JNIEnv *__jni_env,
                                       void*& listPtr,
                                       AbstractSpanAccess*& setAccess);
#endif //QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)

QTJAMBI_EXPORT PtrOwnerFunction registeredOwnerFunction(const std::type_info& typeId);

template<typename T>
bool testQStack(JNIEnv *env, jobject collection){
    return testQStack(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QMetaType::fromType<std::remove_cv_t<T>>());
}

template<typename T>
bool testQList(JNIEnv *env, jobject collection){
    return testQList(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QMetaType::fromType<std::remove_cv_t<T>>());
}

template<typename T>
bool testQSet(JNIEnv *env, jobject collection){
    return testQSet(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QMetaType::fromType<std::remove_cv_t<T>>());
}

template<typename T>
bool testQQueue(JNIEnv *env, jobject collection){
    return testQQueue(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QMetaType::fromType<std::remove_cv_t<T>>());
}

template<typename K, typename V>
bool testQMultiMap(JNIEnv *env, jobject mapObject){
    return testQMultiMap(env, mapObject, QtJambiPrivate::qtjambi_type<K>::id(), QMetaType::fromType<std::remove_cv_t<K>>(), QtJambiPrivate::qtjambi_type<V>::id(), QMetaType::fromType<std::remove_cv_t<V>>());
}

template<typename K, typename V>
bool testQMultiHash(JNIEnv *env, jobject mapObject){
    return testQMultiHash(env, mapObject, QtJambiPrivate::qtjambi_type<K>::id(), QMetaType::fromType<std::remove_cv_t<K>>(), QtJambiPrivate::qtjambi_type<V>::id(), QMetaType::fromType<std::remove_cv_t<V>>());
}

template<typename K, typename V>
bool testQMap(JNIEnv *env, jobject mapObject){
    return testQMap(env, mapObject, QtJambiPrivate::qtjambi_type<K>::id(), QMetaType::fromType<std::remove_cv_t<K>>(), QtJambiPrivate::qtjambi_type<V>::id(), QMetaType::fromType<std::remove_cv_t<V>>());
}

template<typename K, typename V>
bool testQHash(JNIEnv *env, jobject mapObject){
    return testQHash(env, mapObject, QtJambiPrivate::qtjambi_type<K>::id(), QMetaType::fromType<std::remove_cv_t<K>>(), QtJambiPrivate::qtjambi_type<V>::id(), QMetaType::fromType<std::remove_cv_t<V>>());
}

template<typename T>
bool getAsQStack(JNIEnv *env, jobject collection, QStack<T> * &pointer){
    return getAsQStack(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QMetaType::fromType<std::remove_cv_t<T>>(), reinterpret_cast<void*&>(pointer));
}

template<typename T>
bool getAsQStack(JNIEnv *env, jobject collection, QStack<T> * &pointer, AbstractContainerAccess*& access){
    return getAsQStack(env, collection, QMetaType::fromType<std::remove_cv_t<T>>(), reinterpret_cast<void*&>(pointer), access);
}

QTJAMBI_EXPORT jobject objectFromQList(JNIEnv *__jni_env,
                                       void*& listPtr,
                                       AbstractContainerAccess*& setAccess);
QTJAMBI_EXPORT jobject objectFromQList(JNIEnv *__jni_env,
                                       void*& listPtr,
                                       AbstractListAccess*& setAccess);

template<typename T>
bool getAsQList(JNIEnv *env, jobject collection, QList<T> * &pointer, AbstractContainerAccess*& access){
    return getAsQList(env, collection, QMetaType::fromType<std::remove_cv_t<T>>(), reinterpret_cast<void*&>(pointer), access);
}
template<typename T>
bool getAsQList(JNIEnv *env, jobject collection, QList<T> * &pointer){
    return getAsQList(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QMetaType::fromType<std::remove_cv_t<T>>(), reinterpret_cast<void*&>(pointer));
}

template<typename T>
bool getAsQSet(JNIEnv *env, jobject collection, QSet<T> * &pointer, AbstractContainerAccess*& access){
    return getAsQSet(env, collection, QMetaType::fromType<std::remove_cv_t<T>>(), reinterpret_cast<void*&>(pointer), access);
}

template<typename T>
bool getAsQSet(JNIEnv *env, jobject collection, QSet<T> * &pointer){
    return getAsQSet(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QMetaType::fromType<std::remove_cv_t<T>>(), reinterpret_cast<void*&>(pointer));
}

template<typename T>
bool getAsQQueue(JNIEnv *env, jobject collection, QQueue<T> * &pointer, AbstractContainerAccess*& access){
    return getAsQQueue(env, collection, QMetaType::fromType<std::remove_cv_t<T>>(), reinterpret_cast<void*&>(pointer), access);
}

template<typename T>
bool getAsQQueue(JNIEnv *env, jobject collection, QQueue<T> * &pointer){
    return getAsQQueue(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QMetaType::fromType<std::remove_cv_t<T>>(), reinterpret_cast<void*&>(pointer));
}

template<typename K, typename V>
bool getAsQMultiMap(JNIEnv *env, jobject mapObject, QMultiMap<K,V> * &pointer, AbstractContainerAccess*& access){
    return getAsQMultiMap(env, mapObject, QMetaType::fromType<std::remove_cv_t<K>>(), QMetaType::fromType<std::remove_cv_t<V>>(), reinterpret_cast<void*&>(pointer), access);
}

template<typename K, typename V>
bool getAsQMultiMap(JNIEnv *env, jobject mapObject, QMultiMap<K,V> * &pointer){
    return getAsQMultiMap(env, mapObject, QtJambiPrivate::qtjambi_type<K>::id(), QMetaType::fromType<std::remove_cv_t<K>>(), QtJambiPrivate::qtjambi_type<V>::id(), QMetaType::fromType<std::remove_cv_t<V>>(), reinterpret_cast<void*&>(pointer));
}

template<typename K, typename V>
bool getAsQMultiHash(JNIEnv *env, jobject mapObject, QMultiHash<K,V> * &pointer, AbstractContainerAccess*& access){
    return getAsQMultiHash(env, mapObject, QMetaType::fromType<std::remove_cv_t<K>>(), QMetaType::fromType<std::remove_cv_t<V>>(), reinterpret_cast<void*&>(pointer), access);
}

template<typename K, typename V>
bool getAsQMultiHash(JNIEnv *env, jobject mapObject, QMultiHash<K,V> * &pointer){
    return getAsQMultiHash(env, mapObject, QtJambiPrivate::qtjambi_type<K>::id(), QMetaType::fromType<std::remove_cv_t<K>>(), QtJambiPrivate::qtjambi_type<V>::id(), QMetaType::fromType<std::remove_cv_t<V>>(), reinterpret_cast<void*&>(pointer));
}

template<typename K, typename V>
bool getAsQMap(JNIEnv *env, jobject mapObject, QMap<K,V> * &pointer, AbstractContainerAccess*& access){
    return getAsQMap(env, mapObject, QMetaType::fromType<std::remove_cv_t<K>>(), QMetaType::fromType<std::remove_cv_t<V>>(), reinterpret_cast<void*&>(pointer), access);
}

template<typename K, typename V>
bool getAsQMap(JNIEnv *env, jobject mapObject, QMap<K,V> * &pointer){
    return getAsQMap(env, mapObject, QtJambiPrivate::qtjambi_type<K>::id(), QMetaType::fromType<std::remove_cv_t<K>>(), QtJambiPrivate::qtjambi_type<V>::id(), QMetaType::fromType<std::remove_cv_t<V>>(), reinterpret_cast<void*&>(pointer));
}

template<typename K, typename V>
bool getAsQHash(JNIEnv *env, jobject mapObject, QHash<K,V> * &pointer, AbstractContainerAccess*& access){
    return getAsQHash(env, mapObject, QMetaType::fromType<std::remove_cv_t<K>>(), QMetaType::fromType<std::remove_cv_t<V>>(), reinterpret_cast<void*&>(pointer), access);
}

template<typename K, typename V>
bool getAsQHash(JNIEnv *env, jobject mapObject, QHash<K,V> * &pointer){
    return getAsQHash(env, mapObject, QtJambiPrivate::qtjambi_type<K>::id(), QMetaType::fromType<std::remove_cv_t<K>>(), QtJambiPrivate::qtjambi_type<V>::id(), QMetaType::fromType<std::remove_cv_t<V>>(), reinterpret_cast<void*&>(pointer));
}

} // namespace ContainerAPI

struct ContainerInfo{
    jobject object = nullptr;
    void* container = nullptr;
    ContainerInfo() = default;
    ContainerInfo(jobject _object, void* _container) : object(_object), container(_container) {}
};
struct ExtendedContainerInfo : ContainerInfo{
    QtJambiNativeID nativeId = QtJambiNativeID::Invalid;
    ExtendedContainerInfo() = default;
    ExtendedContainerInfo(jobject _object, void* _container, QtJambiNativeID _nativeId) : ContainerInfo(_object, _container), nativeId(_nativeId) {}
};
struct ContainerAndAccessInfo : ContainerInfo{
    AbstractContainerAccess* access = nullptr;
    ContainerAndAccessInfo() = default;
    ContainerAndAccessInfo(jobject _object) : ContainerInfo{_object, nullptr}{}
    ContainerAndAccessInfo(jobject _object, void* container, AbstractContainerAccess* _access) : ContainerInfo{_object, container}, access(_access) {}
};
struct ConstContainerInfo{
    jobject object = nullptr;
    const void* container = nullptr;
    ConstContainerInfo() = default;
    ConstContainerInfo(const ConstContainerInfo& other) : object(other.object), container(other.container) {}
    ConstContainerInfo(const ContainerInfo& other) : object(other.object), container(other.container) {}
    ConstContainerInfo(jobject _object, const void* _container) : object(_object), container(_container) {}
};
struct ConstExtendedContainerInfo : ConstContainerInfo{
    QtJambiNativeID nativeId = QtJambiNativeID::Invalid;
    ConstExtendedContainerInfo() = default;
    ConstExtendedContainerInfo(const ConstExtendedContainerInfo& other) : ConstContainerInfo(other), nativeId(other.nativeId) {}
    ConstExtendedContainerInfo(const ExtendedContainerInfo& other) : ConstContainerInfo(other), nativeId(other.nativeId) {}
    ConstExtendedContainerInfo(jobject _object, void* _container, QtJambiNativeID _nativeId) : ConstContainerInfo(_object, _container), nativeId(_nativeId) {}
};
struct ConstContainerAndAccessInfo : ConstContainerInfo{
    AbstractContainerAccess* access = nullptr;
    ConstContainerAndAccessInfo() = default;
    ConstContainerAndAccessInfo(jobject _object, const void* container, AbstractContainerAccess* _access) : ConstContainerInfo{_object, container}, access(_access) {}
};

namespace QtJambiPrivate{
template<typename, typename = void>
struct has_value_type : std::false_type {};
template<typename T>
struct has_value_type<T, std::void_t<typename T::value_type>> : std::true_type {};
template<typename T>
constexpr bool has_value_type_v = has_value_type<T>::value;

template<typename, typename = void>
struct has_difference_type : std::false_type {};
template<typename T>
struct has_difference_type<T, std::void_t<typename T::difference_type>> : std::true_type {};
template<typename T>
constexpr bool has_difference_type_v = has_difference_type<T>::value;

template<typename, typename = void>
struct has_iterator_category : std::false_type {};
template<typename T>
struct has_iterator_category<T, std::void_t<typename T::iterator_category>> : std::true_type {};
template<typename T>
constexpr bool has_iterator_category_v = has_iterator_category<T>::value;

template<typename Container, typename Iter,
         bool = std::is_pointer_v<Iter>,
         bool = has_value_type_v<Container>,
         bool = has_value_type_v<Iter>,
         bool = supports_value_v<const Iter>,
         bool = supports_deref_v<const Iter&>>
struct iter_value_type{
    using value_type = typename Iter::value_type;
};

template<typename Container, typename Iter, bool cv, bool iv, bool v, bool r>
struct iter_value_type<Container,Iter,true,cv,iv,v,r>{
    using value_type = std::remove_pointer_t<Iter>;
};

template<typename Container, typename Iter, bool v, bool r>
struct iter_value_type<Container,Iter,false,true,false,v,r>{
    using value_type = typename Container::value_type;
};

template<typename Container, typename Iter, bool r>
struct iter_value_type<Container,Iter,false,false,false,true,r>{
    using value_type = std::remove_reference_t<decltype(std::declval<const Iter&>().value())>;
};

template<typename Container, typename Iter>
struct iter_value_type<Container,Iter,false,false,false,false,true>{
    using value_type = std::remove_reference_t<decltype(*std::declval<const Iter&>())>;
};

template<typename Container, typename Iter>
struct iter_value_type<Container,Iter,false,false,false,false,false>{
    using value_type = void;
};

template<typename Container, typename Iter, bool = QtJambiPrivate::has_difference_type_v<std::iterator_traits<Iter>>>
struct iter_difference_type{
    using difference_type = typename std::iterator_traits<Iter>::difference_type;
};

template<typename Container, typename Iter>
struct iter_difference_type<Container,Iter,false>{
    using difference_type = size_t;
};

template<typename Iter, bool = QtJambiPrivate::has_iterator_category_v<std::iterator_traits<Iter>>>
struct iter_iterator_category{
    using iterator_category = typename std::iterator_traits<Iter>::iterator_category;
};

template<typename Iter>
struct iter_iterator_category<Iter,false>{
    using iterator_category = std::forward_iterator_tag;
};

template<typename Container, bool isShared = supports_isSharedWith_v<const Container,const Container&>>
struct ContainerSharedInfo{
    static constexpr bool is_shared = isShared;
    static bool isSharedWith(const Container&,const Container&){
        return true;//continue with iterator comparison
    }
};

template<typename Container>
struct ContainerSharedInfo<Container,true>{
    static constexpr bool is_shared = true;
    static bool isSharedWith(const Container& container, const Container& other){
        return container.isSharedWith(other);
    }
};

template<typename T>
struct ContainerSharedInfo<QSet<T>,false>{
    static constexpr bool is_shared = true;
    static bool isSharedWith(const QSet<T>& container, const QSet<T>& other){
        return reinterpret_cast<const QHash<T, QHashDummyValue>&>(container).isSharedWith(reinterpret_cast<const QHash<T, QHashDummyValue>&>(other));
    }
};

template<typename Container, typename Itererator>
struct ContainerIteratorConstBegin{
    static auto function(const Container& container){
        if constexpr(QtJambiPrivate::supports_constKeyValueBegin_v<Container>){
            if constexpr(QtJambiPrivate::supports_key_value_iterator_v<Container>){
                if constexpr(std::is_same_v<Itererator, typename Container::key_value_iterator>){
                    return container.constKeyValueBegin();
                }else{
                    if constexpr(QtJambiPrivate::supports_const_key_value_iterator_v<Container>){
                        if constexpr(std::is_same_v<Itererator, typename Container::const_key_value_iterator>){
                            return container.constKeyValueBegin();
                        }else{
                            return std::cbegin(container);
                        }
                    }else{
                        return std::cbegin(container);
                    }
                }
            }else{
                if constexpr(QtJambiPrivate::supports_const_key_value_iterator_v<Container>){
                    if constexpr(std::is_same_v<Itererator, typename Container::const_key_value_iterator>){
                        return container.constKeyValueBegin();
                    }else{
                        return std::cbegin(container);
                    }
                }else{
                    return std::cbegin(container);
                }
            }
        }else{
            return std::cbegin(container);
        }
    }
};

template<typename Container, typename Iterator>
struct ContainerIteratorConstEnd{
    static auto function(const Container& container){
        if constexpr(QtJambiPrivate::supports_constKeyValueEnd_v<Container>){
            if constexpr(QtJambiPrivate::supports_key_value_iterator_v<Container>){
                if constexpr(std::is_same_v<Iterator, typename Container::key_value_iterator>){
                    return container.constKeyValueEnd();
                }else{
                    if constexpr(QtJambiPrivate::supports_const_key_value_iterator_v<Container>){
                        if constexpr(std::is_same_v<Iterator, typename Container::const_key_value_iterator>){
                            return container.constKeyValueEnd();
                        }else{
                            return std::cend(container);
                        }
                    }else{
                        return std::cend(container);
                    }
                }
            }else{
                if constexpr(QtJambiPrivate::supports_const_key_value_iterator_v<Container>){
                    if constexpr(std::is_same_v<Iterator, typename Container::const_key_value_iterator>){
                        return container.constKeyValueEnd();
                    }else{
                        return std::cend(container);
                    }
                }else{
                    return std::cend(container);
                }
            }
        }else{
            return std::cend(container);
        }
    }
};

template<typename Container>
struct ContainerRef;

template<typename Container, typename Iterator>
struct ContainerIteratorConstBegin<ContainerRef<Container>,Iterator>{
    static auto function(const ContainerRef<Container>& container){
        if constexpr(QtJambiPrivate::supports_constKeyValueBegin_v<Container>){
            if constexpr(QtJambiPrivate::supports_key_value_iterator_v<Container>){
                if constexpr(std::is_same_v<Iterator, typename Container::key_value_iterator>){
                    return container.container().constKeyValueBegin();
                }else{
                    if constexpr(QtJambiPrivate::supports_const_key_value_iterator_v<Container>){
                        if constexpr(std::is_same_v<Iterator, typename Container::const_key_value_iterator>){
                            return container.container().constKeyValueBegin();
                        }else{
                            return std::cbegin(container.container());
                        }
                    }else{
                        return std::cbegin(container.container());
                    }
                }
            }else{
                if constexpr(QtJambiPrivate::supports_const_key_value_iterator_v<Container>){
                    if constexpr(std::is_same_v<Iterator, typename Container::const_key_value_iterator>){
                        return container.container().constKeyValueBegin();
                    }else{
                        return std::cbegin(container.container());
                    }
                }else{
                    return std::cbegin(container.container());
                }
            }
        }else{
            return std::cbegin(container.container());
        }
    }
};

template<typename Container, typename Iterator>
struct ContainerIteratorConstEnd<ContainerRef<Container>,Iterator>{
    static auto function(const ContainerRef<Container>& container){
        if constexpr(QtJambiPrivate::supports_constKeyValueEnd_v<Container>){
            if constexpr(QtJambiPrivate::supports_key_value_iterator_v<Container>){
                if constexpr(std::is_same_v<Iterator, typename Container::key_value_iterator>){
                    return container.container().constKeyValueEnd();
                }else{
                    if constexpr(QtJambiPrivate::supports_const_key_value_iterator_v<Container>){
                        if constexpr(std::is_same_v<Iterator, typename Container::const_key_value_iterator>){
                            return container.container().constKeyValueEnd();
                        }else{
                            return std::cend(container.container());
                        }
                    }else{
                        return std::cend(container.container());
                    }
                }
            }else{
                if constexpr(QtJambiPrivate::supports_const_key_value_iterator_v<Container>){
                    if constexpr(std::is_same_v<Iterator, typename Container::const_key_value_iterator>){
                        return container.container().constKeyValueEnd();
                    }else{
                        return std::cend(container.container());
                    }
                }else{
                    return std::cend(container.container());
                }
            }
        }else{
            return std::cend(container.container());
        }
    }
};

template<typename Container, typename Iterator>
struct ContainerIteratorConstBegin<Container,std::reverse_iterator<Iterator>>{
    static auto function(const Container& container){
        if constexpr(QtJambiPrivate::supports_crbegin_v<Container>){
            return container.crbegin();
        }else{
            return std::cbegin(container);
        }
    }
};

template<typename Container, typename Iterator>
struct ContainerIteratorConstEnd<Container,std::reverse_iterator<Iterator>>{
    static auto function(const Container& container){
        if constexpr(QtJambiPrivate::supports_crend_v<Container>){
            return container.crend();
        }else{
            return std::cend(container);
        }
    }
};

template<typename Container, typename Iterator>
struct ContainerIteratorConstBegin<ContainerRef<Container>,std::reverse_iterator<Iterator>>{
    static auto function(const ContainerRef<Container>& container){
        if constexpr(QtJambiPrivate::supports_crbegin_v<Container>){
            return container.crbegin();
        }else{
            return std::cbegin(container);
        }
    }
};

template<typename Container, typename Iterator>
struct ContainerIteratorConstEnd<ContainerRef<Container>,std::reverse_iterator<Iterator>>{
    static auto function(const ContainerRef<Container>& container){
        if constexpr(QtJambiPrivate::supports_crend_v<Container>){
            return container.crend();
        }else{
            return std::cend(container);
        }
    }
};

struct ContainerRefPrivate;

QTJAMBI_EXPORT QSharedPointer<ContainerRefPrivate> getContainerReference(QtJambiNativeID containerId);
QTJAMBI_EXPORT QtJambiNativeID nativeId(const QSharedPointer<ContainerRefPrivate>& container);
QTJAMBI_EXPORT void* getContainer(const QSharedPointer<ContainerRefPrivate>& container);
QTJAMBI_EXPORT bool compareEquals(const QSharedPointer<ContainerRefPrivate>&,const QSharedPointer<ContainerRefPrivate>& container);

template<typename Container,
         bool = supports_less_than_v<decltype(std::cbegin(std::declval<const Container&>()))&>
                || !supports_equal_v<decltype(std::cbegin(std::declval<const Container&>()))&>>
struct InitialItersContainer{
    InitialItersContainer(const Container& container)
        : m_initialBegin(std::cbegin(container)),
          m_initialEnd(std::cend(container))
    {}
    const auto& initialBegin() const {
        return m_initialBegin;
    }
    const auto& initialEnd() const {
        return m_initialEnd;
    }
private:
    std::remove_reference_t<decltype(std::cbegin(std::declval<const Container&>()))> m_initialBegin;
    std::remove_reference_t<decltype(std::cend(std::declval<const Container&>()))> m_initialEnd;
};

template<typename Container>
struct InitialItersContainer<Container,true>{
    InitialItersContainer(const Container&){}
};

template<typename Container>
struct ContainerRef : InitialItersContainer<Container> {
    ContainerRef(const Container& container, QtJambiNativeID containerId)
        : InitialItersContainer<Container>(container), d(getContainerReference(containerId)) {}
    ContainerRef(const Container& container, const ExtendedContainerInfo& ci)
        : InitialItersContainer<Container>(container), d(getContainerReference(ci.nativeId)) {}
    bool operator==(const ContainerRef& other) const {return compareEquals(d, other.d);}
    template<typename _Container = Container, std::enable_if_t<QtJambiPrivate::ContainerSharedInfo<_Container>::is_shared,bool> = true>
    bool isSharedWith(const ContainerRef& clone) const{
        if(compareEquals(d, clone.d))
            return true;
        return QtJambiPrivate::ContainerSharedInfo<_Container>::isSharedWith(container(), clone.container());
    }
    auto begin() const {
        return std::cbegin(container());
    }
    auto end() const {
        return std::cend(container());
    }
    template<typename _Container = Container, std::enable_if_t<QtJambiPrivate::supports_crbegin_v<const _Container&>,bool> = true>
    auto crbegin() const {
        return std::crbegin(container());
    }
    template<typename _Container = Container, std::enable_if_t<QtJambiPrivate::supports_crend_v<const _Container&>,bool> = true>
    auto crend() const {
        return std::crend(container());
    }
    template<typename _Container = Container, std::enable_if_t<QtJambiPrivate::supports_constKeyValueBegin_v<const _Container&>,bool> = true>
    auto constKeyValueBegin() const {
        return container().constKeyValueBegin();
    }
    template<typename _Container = Container, std::enable_if_t<QtJambiPrivate::supports_constKeyValueEnd_v<const _Container&>,bool> = true>
    auto constKeyValueEnd() const {
        return container().constKeyValueEnd();
    }
    const QSharedPointer<ContainerRefPrivate>& reference() const { return d; }
    QtJambiNativeID nativeId() const{ return QtJambiPrivate::nativeId(d); };
    const Container& container() const{ return *static_cast<const Container*>(QtJambiPrivate::getContainer(d)); };
    Container& container() { return *static_cast<Container*>(QtJambiPrivate::getContainer(d)); };
private:
    QSharedPointer<ContainerRefPrivate> d;
};

template<typename It, typename T, bool = QtJambiPrivate::supports_value_v<It>, bool = QtJambiPrivate::supports_deref_v<It>>
struct is_writable_iterator_impl : QtJambiPrivate::supports_assign<decltype(std::declval<It>().value()),T> {};
template<typename It, typename T>
struct is_writable_iterator_impl<It,T,false,true> : QtJambiPrivate::supports_assign<decltype(*std::declval<It>()),T> {};
template<typename It, typename T>
struct is_writable_iterator_impl<It,T,false,false> : std::false_type{};
template<typename It, typename T>
struct is_writable_iterator : is_writable_iterator_impl<It,T> {};
template<typename Key, typename V, typename Iter QT610_EXTRA_ARG(class Traits), typename T>
struct is_writable_iterator<QKeyValueIterator<Key,V,Iter QT610_EXTRA_ARG(Traits)>,T> : is_writable_iterator<Iter,T>{};
template<typename Key, typename V, typename Iter QT610_EXTRA_ARG(class Traits), typename K, typename T>
struct is_writable_iterator<QKeyValueIterator<Key,V,Iter QT610_EXTRA_ARG(Traits)>,std::pair<K,T>> : is_writable_iterator<Iter,T>{};
template<typename It, typename T>
static constexpr bool is_writable_iterator_v = is_writable_iterator<It,T>::value;
template<typename Container, typename Iter>
static constexpr bool is_writable_iterator_of_container_v = is_writable_iterator<Iter,typename iter_value_type<Container,Iter>::value_type>::value;

template<typename Container>
struct is_allowed_as_container : std::bool_constant<!std::is_base_of_v<AbstractContainerAccess,Container>>{};
template<typename Container>
static constexpr bool is_allowed_as_container_v = is_allowed_as_container<Container>::value;

template<typename Container, typename Iter, typename Begin = Iter, typename End = Iter>
struct iterator_comparable{
    static constexpr bool test(const Iter&, const Begin&, const End&){
        return true;
    }
};

template<typename Key, typename T, class Iterator QT610_EXTRA_ARG(class Traits)>
struct supports_decrement<QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>> : supports_decrement<Iterator>{};

template<typename Key, typename T, class Iterator QT610_EXTRA_ARG(class Traits)>
struct supports_increment<QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>> : supports_increment<Iterator>{};

template<typename Key, typename T, class Iterator QT610_EXTRA_ARG(class Traits)>
struct supports_suffix_decrement<QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>> : supports_suffix_decrement<Iterator>{};

template<typename Key, typename T, class Iterator QT610_EXTRA_ARG(class Traits)>
struct supports_suffix_increment<QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>> : supports_suffix_increment<Iterator>{};

template<typename Key, typename T, class Iterator QT610_EXTRA_ARG(class Traits)>
struct supports_equal<QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>> : supports_equal<Iterator>{};

template<typename Key, typename T, class Iterator QT610_EXTRA_ARG(class Traits)>
struct supports_not_equal<QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>> : supports_not_equal<Iterator>{};

template<typename Key, typename T, class Iterator QT610_EXTRA_ARG(class Traits)>
struct supports_decrement<QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>&> : supports_decrement<Iterator&>{};

template<typename Key, typename T, class Iterator QT610_EXTRA_ARG(class Traits)>
struct supports_increment<QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>&> : supports_increment<Iterator&>{};

template<typename Key, typename T, class Iterator QT610_EXTRA_ARG(class Traits)>
struct supports_suffix_decrement<QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>&> : supports_suffix_decrement<Iterator&>{};

template<typename Key, typename T, class Iterator QT610_EXTRA_ARG(class Traits)>
struct supports_suffix_increment<QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>&> : supports_suffix_increment<Iterator&>{};

template<typename Key, typename T, class Iterator QT610_EXTRA_ARG(class Traits)>
struct supports_equal<QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>&> : supports_equal<Iterator&>{};

template<typename Key, typename T, class Iterator QT610_EXTRA_ARG(class Traits)>
struct supports_not_equal<QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>&> : supports_not_equal<Iterator&>{};

template<typename Key, typename T, class Iterator QT610_EXTRA_ARG(class Traits)>
struct supports_decrement<const QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>&> : supports_decrement<const Iterator&>{};

template<typename Key, typename T, class Iterator QT610_EXTRA_ARG(class Traits)>
struct supports_increment<const QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>&> : supports_increment<const Iterator&>{};

template<typename Key, typename T, class Iterator QT610_EXTRA_ARG(class Traits)>
struct supports_suffix_decrement<const QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>&> : supports_suffix_decrement<const Iterator&>{};

template<typename Key, typename T, class Iterator QT610_EXTRA_ARG(class Traits)>
struct supports_suffix_increment<const QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>&> : supports_suffix_increment<const Iterator&>{};

template<typename Key, typename T, class Iterator QT610_EXTRA_ARG(class Traits)>
struct supports_equal<const QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>&> : supports_equal<const Iterator&>{};

template<typename Key, typename T, class Iterator QT610_EXTRA_ARG(class Traits)>
struct supports_not_equal<const QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>&> : supports_not_equal<const Iterator&>{};

template<typename Key, typename T, class Iterator, class Iterator2 QT610_EXTRA_ARG(class Traits)>
struct supports_equal<const QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>&,Iterator2> : supports_equal<const Iterator&,Iterator2>{};

template<typename Key, typename T, class Iterator, class Iterator2 QT610_EXTRA_ARG(class Traits)>
struct supports_not_equal<const QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>&,Iterator2> : supports_not_equal<const Iterator&,Iterator2>{};

template<typename Key, typename T, class Iterator, class Iterator1 QT610_EXTRA_ARG(class Traits)>
struct supports_equal<Iterator1, const QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>&> : supports_equal<Iterator1,const Iterator&>{};

template<typename Key, typename T, class Iterator, class Iterator1 QT610_EXTRA_ARG(class Traits)>
struct supports_not_equal<Iterator1, const QKeyValueIterator<Key,T,Iterator QT610_EXTRA_ARG(Traits)>&> : supports_not_equal<Iterator1,const Iterator&>{};

template<typename Container, typename Iter, typename _Iter>
struct iterator_equals : supports_equal<const Iter&, const _Iter&>{
    static constexpr bool function(const Iter& a, const _Iter& b){
        if constexpr(supports_equal_v<const Iter&, const _Iter&>)
            return a==b;
        else{
            Q_UNUSED(a)
            Q_UNUSED(b)
            return false;
        }
    }
};

template<typename K, typename T>
struct iterator_equals<QMap<K,T>, typename QMap<K,T>::const_iterator, typename QMap<K,T>::const_iterator> : std::true_type{
    static constexpr bool function(const typename QMap<K,T>::const_iterator& a, const typename QMap<K,T>::const_iterator& b){
        const typename std::map<K,T>::const_iterator& _a = *reinterpret_cast<const typename std::map<K,T>::const_iterator*>(&a);
        const typename std::map<K,T>::const_iterator& _b = *reinterpret_cast<const typename std::map<K,T>::const_iterator*>(&b);
#if defined(Q_CC_MSVC)
        if(_a._Getcont()!=_b._Getcont())
            return false;
#elif defined(_LIBCPP_VERSION)
#elif defined(__GLIBCXX__)
#endif
        return _a==_b;
    }
};

template<typename K, typename T>
struct iterator_equals<QMap<K,T>, typename QMap<K,T>::iterator, typename QMap<K,T>::const_iterator> : std::true_type{
    static constexpr bool function(const typename QMap<K,T>::iterator& a, const typename QMap<K,T>::const_iterator& b){
        const typename std::map<K,T>::iterator& _a = *reinterpret_cast<const typename std::map<K,T>::iterator*>(&a);
        const typename std::map<K,T>::const_iterator& _b = *reinterpret_cast<const typename std::map<K,T>::const_iterator*>(&b);
#if defined(Q_CC_MSVC)
        if(_a._Getcont()!=_b._Getcont())
            return false;
#elif defined(_LIBCPP_VERSION)
#elif defined(__GLIBCXX__)
#endif
        return _a==_b;
    }
};

template<typename K, typename T>
struct iterator_equals<QMap<K,T>, typename QMap<K,T>::const_iterator, typename QMap<K,T>::iterator> : std::true_type{
    static constexpr bool function(const typename QMap<K,T>::const_iterator& a, const typename QMap<K,T>::iterator& b){
        const typename std::map<K,T>::const_iterator& _a = *reinterpret_cast<const typename std::map<K,T>::const_iterator*>(&a);
        const typename std::map<K,T>::iterator& _b = *reinterpret_cast<const typename std::map<K,T>::iterator*>(&b);
#if defined(Q_CC_MSVC)
        if(_a._Getcont()!=_b._Getcont())
            return false;
#elif defined(_LIBCPP_VERSION)
#elif defined(__GLIBCXX__)
#endif
        return _a==_b;
    }
};

template<typename K, typename T>
struct iterator_equals<QMap<K,T>, typename QMap<K,T>::iterator, typename QMap<K,T>::iterator> : std::true_type{
    static constexpr bool function(const typename QMap<K,T>::iterator& a, const typename QMap<K,T>::iterator& b){
        const typename std::map<K,T>::iterator& _a = *reinterpret_cast<const typename std::map<K,T>::iterator*>(&a);
        const typename std::map<K,T>::iterator& _b = *reinterpret_cast<const typename std::map<K,T>::iterator*>(&b);
#if defined(Q_CC_MSVC)
        if(_a._Getcont()!=_b._Getcont())
            return false;
#elif defined(_LIBCPP_VERSION)
#elif defined(__GLIBCXX__)
#endif
        return _a==_b;
    }
};

template<typename K, typename T>
struct iterator_equals<QMultiMap<K,T>, typename QMultiMap<K,T>::const_iterator, typename QMultiMap<K,T>::const_iterator> : std::true_type{
    static constexpr bool function(const typename QMultiMap<K,T>::const_iterator& a, const typename QMultiMap<K,T>::const_iterator& b){
        const typename std::multimap<K,T>::const_iterator& _a = *reinterpret_cast<const typename std::multimap<K,T>::const_iterator*>(&a);
        const typename std::multimap<K,T>::const_iterator& _b = *reinterpret_cast<const typename std::multimap<K,T>::const_iterator*>(&b);
#if defined(Q_CC_MSVC)
        if(_a._Getcont()!=_b._Getcont())
            return false;
#elif defined(_LIBCPP_VERSION)
#elif defined(__GLIBCXX__)
#endif
        return _a==_b;
    }
};

template<typename K, typename T>
struct iterator_equals<QMultiMap<K,T>, typename QMultiMap<K,T>::iterator, typename QMultiMap<K,T>::const_iterator> : std::true_type{
    static constexpr bool function(const typename QMultiMap<K,T>::iterator& a, const typename QMultiMap<K,T>::const_iterator& b){
        const typename std::multimap<K,T>::iterator& _a = *reinterpret_cast<const typename std::multimap<K,T>::iterator*>(&a);
        const typename std::multimap<K,T>::const_iterator& _b = *reinterpret_cast<const typename std::multimap<K,T>::const_iterator*>(&b);
#if defined(Q_CC_MSVC)
        if(_a._Getcont()!=_b._Getcont())
            return false;
#elif defined(_LIBCPP_VERSION)
#elif defined(__GLIBCXX__)
#endif
        return _a==_b;
    }
};

template<typename K, typename T>
struct iterator_equals<QMultiMap<K,T>, typename QMultiMap<K,T>::const_iterator, typename QMultiMap<K,T>::iterator> : std::true_type{
    static constexpr bool function(const typename QMultiMap<K,T>::const_iterator& a, const typename QMultiMap<K,T>::iterator& b){
        const typename std::multimap<K,T>::const_iterator& _a = *reinterpret_cast<const typename std::multimap<K,T>::const_iterator*>(&a);
        const typename std::multimap<K,T>::iterator& _b = *reinterpret_cast<const typename std::multimap<K,T>::iterator*>(&b);
#if defined(Q_CC_MSVC)
        if(_a._Getcont()!=_b._Getcont())
            return false;
#elif defined(_LIBCPP_VERSION)
#elif defined(__GLIBCXX__)
#endif
        return _a==_b;
    }
};

template<typename K, typename T>
struct iterator_equals<QMultiMap<K,T>, typename QMultiMap<K,T>::iterator, typename QMultiMap<K,T>::iterator> : std::true_type{
    static constexpr bool function(const typename QMultiMap<K,T>::iterator& a, const typename QMultiMap<K,T>::iterator& b){
        const typename std::multimap<K,T>::iterator& _a = *reinterpret_cast<const typename std::multimap<K,T>::iterator*>(&a);
        const typename std::multimap<K,T>::iterator& _b = *reinterpret_cast<const typename std::multimap<K,T>::iterator*>(&b);
#if defined(Q_CC_MSVC)
        if(_a._Getcont()!=_b._Getcont())
            return false;
#elif defined(_LIBCPP_VERSION)
#elif defined(__GLIBCXX__)
#endif
        return _a==_b;
    }
};

template<typename Container, typename Key, typename T1, typename Iter1, typename Iter2 QT610_EXTRA_ARG(typename Traits1)>
struct iterator_equals<Container, QKeyValueIterator<Key,T1,Iter1 QT610_EXTRA_ARG(Traits1)>,Iter2> : supports_equal<const Iter1&, const Iter2&>{
    static bool function(const QKeyValueIterator<Key,T1,Iter1 QT610_EXTRA_ARG(Traits1)>& a, const Iter2& b){
        return iterator_equals<Container, Iter1,Iter2>::function(a.base(), b);
    }
};

template<typename Container, typename Key, typename T2, typename Iter1, typename Iter2 QT610_EXTRA_ARG(typename Traits2)>
struct iterator_equals<Container, Iter1,QKeyValueIterator<Key,T2,Iter2 QT610_EXTRA_ARG(Traits2)>> : supports_equal<const Iter1&, const Iter2&>{
    static bool function(const Iter1& a, const QKeyValueIterator<Key,T2,Iter2 QT610_EXTRA_ARG(Traits2)>& b){
        return iterator_equals<Container, Iter1,Iter2>::function(a, b.base());
    }
};

template<typename Container, typename Key, typename T1, typename T2, typename Iter1, typename Iter2 QT610_EXTRA_ARG(typename Traits1) QT610_EXTRA_ARG(typename Traits2)>
struct iterator_equals<Container, QKeyValueIterator<Key,T1,Iter1 QT610_EXTRA_ARG(Traits1)>,QKeyValueIterator<Key,T2,Iter2 QT610_EXTRA_ARG(Traits2)>> : supports_equal<const Iter1&, const Iter2&>{
    static bool function(const QKeyValueIterator<Key,T1,Iter1 QT610_EXTRA_ARG(Traits1)>& a, const QKeyValueIterator<Key,T2,Iter2 QT610_EXTRA_ARG(Traits2)>& b){
        if constexpr(std::is_same_v<Iter1,Iter2> && supports_equal_v<const Iter1&, const Iter2&>){
            return a==b;
        }else{
            return iterator_equals<Container, Iter1,Iter2>::function(a.base(), b.base());
        }
    }
};

template<typename Container, typename Iter1, typename Iter2>
struct iterator_equals<Container, std::reverse_iterator<Iter1>, std::reverse_iterator<Iter2>> : supports_equal<const Iter1&, const Iter2&>{
    static bool function(const std::reverse_iterator<Iter1>& a, const std::reverse_iterator<Iter2>& b){
        if constexpr(std::is_same_v<Iter1,Iter2> && supports_equal_v<const Iter1&, const Iter2&>){
            return a==b;
        }else{
            return iterator_equals<Container, Iter1,Iter2>::function(a.base(), b.base());
        }
    }
};
}

#define CONTAINER_ITERATOR_COMPARE \
if constexpr(std::is_same_v<storage_type,_Storage> && QtJambiPrivate::ContainerSharedInfo<storage_type>::is_shared){\
    if(!QtJambiPrivate::ContainerSharedInfo<storage_type>::isSharedWith(m_storage, o.m_storage))\
        return false;\
}

#define CONTAINER_ITERATOR_IMPL(...) \
template<typename _Container = container_type, typename _Iter = iterator_type, typename _Storage = storage_type>\
bool operator==(const ContainerIterator<_Container,_Iter,_Storage> &o) const { \
    if constexpr(QtJambiPrivate::iterator_equals<_Container,Iter,_Iter>::value){\
        __VA_ARGS__ \
        return QtJambiPrivate::iterator_equals<_Container,Iter,_Iter>::function(m_iterator, o.m_iterator); \
    }else{\
        Q_UNUSED(o)\
        return false;\
    }\
} \
template<typename _Iter = Iter, std::enable_if_t<QtJambiPrivate::supports_deref_v<const _Iter&>,bool> = true> \
decltype(auto) operator*() const { return *m_iterator; } \
template<typename _Iter = Iter, std::enable_if_t<QtJambiPrivate::supports_deref_v<_Iter&>,bool> = true> \
decltype(auto) operator*() { return *m_iterator; } \
template<typename _Iter = Iter, std::enable_if_t<QtJambiPrivate::supports_value_v<const _Iter&>,bool> = true> \
decltype(auto) value() const { return m_iterator.value(); } \
template<typename _Iter = Iter, std::enable_if_t<QtJambiPrivate::supports_value_v<_Iter&>,bool> = true> \
decltype(auto) value() { return m_iterator.value(); } \
template<typename _Iter = Iter, std::enable_if_t<QtJambiPrivate::supports_key_v<const _Iter&>,bool> = true> \
decltype(auto) key() const { return m_iterator.key(); } \
template<typename _Iter = Iter, std::enable_if_t<QtJambiPrivate::supports_key_v<_Iter&>,bool> = true> \
decltype(auto) key() { return m_iterator.key(); } \
template<typename _Iter = Iter, std::enable_if_t<QtJambiPrivate::supports_subscribe_v<const _Iter&,qsizetype>,bool> = true> \
decltype(auto) operator[](qsizetype j) const { return m_iterator[j]; } \
template<typename _Iter = Iter, std::enable_if_t<QtJambiPrivate::supports_subscribe_v<_Iter&,qsizetype>,bool> = true> \
decltype(auto) operator[](qsizetype j) { return m_iterator[j]; } \
bool operator!=(const ContainerIterator &o) const { return !operator==(o); } \
 \
template<typename _Iter = Iter, std::enable_if_t<QtJambiPrivate::supports_less_than_v<const _Iter&>,bool> = true> \
bool operator<(const ContainerIterator& other) const { return m_iterator < other.m_iterator; } \
template<typename _Iter = Iter, std::enable_if_t<QtJambiPrivate::supports_less_than_v<const _Iter&>,bool> = true> \
bool operator<=(const ContainerIterator& other) const { return m_iterator < other.m_iterator || operator==(other); } \
template<typename _Iter = Iter, std::enable_if_t<QtJambiPrivate::supports_less_than_v<const _Iter&>,bool> = true> \
bool operator>(const ContainerIterator& other) const { return !operator<=(other); } \
template<typename _Iter = Iter, std::enable_if_t<QtJambiPrivate::supports_less_than_v<const _Iter&>,bool> = true> \
bool operator>=(const ContainerIterator& other) const { return !operator<(other); } \
 \
template<typename _Iter = Iter, std::enable_if_t<QtJambiPrivate::supports_increment_v<_Iter&>,bool> = true> \
ContainerIterator &operator++() { ++m_iterator; return *this; } \
template<typename _Iter = Iter, std::enable_if_t<QtJambiPrivate::supports_suffix_increment_v<_Iter&>,bool> = true> \
ContainerIterator operator++(int) { ContainerIterator n = *this; ++m_iterator; return n; } \
template<typename _Iter = Iter, std::enable_if_t<QtJambiPrivate::supports_add_assign_v<_Iter&,qsizetype>,bool> = true> \
ContainerIterator &operator+=(qsizetype j) { m_iterator += j; return *this; } \
template<typename _Iter = Iter, std::enable_if_t<QtJambiPrivate::supports_add_v<const _Iter&,qsizetype>,bool> = true> \
ContainerIterator operator+(qsizetype j) { ContainerIterator n = *this; n+=j; return n; } \
 \
template<typename _Iter = Iter, std::enable_if_t<QtJambiPrivate::supports_decrement_v<_Iter&>,bool> = true> \
ContainerIterator &operator--() { m_iterator--; return *this; } \
template<typename _Iter = Iter, std::enable_if_t<QtJambiPrivate::supports_suffix_decrement_v<_Iter&>,bool> = true> \
ContainerIterator operator--(int) { ContainerIterator n = *this; m_iterator--; return n; } \
template<typename _Iter = Iter, std::enable_if_t<QtJambiPrivate::supports_subtract_assign_v<_Iter&,qsizetype>,bool> = true> \
ContainerIterator &operator-=(qsizetype j) { m_iterator -= j; return *this; } \
template<typename _Iter = Iter, std::enable_if_t<QtJambiPrivate::supports_subtract_v<const _Iter&,qsizetype>,bool> = true> \
ContainerIterator operator-(qsizetype j) const { ContainerIterator n = *this; n-=j; return n; } \
template<typename _Iter = Iter, std::enable_if_t<QtJambiPrivate::supports_subtract_v<const _Iter&,const _Iter&>,bool> = true> \
qsizetype operator-(const ContainerIterator<container_type,_Iter,storage_type>& j) const { return m_iterator - j.m_iterator; }

template<typename Container, typename Iter = typename Container::const_iterator, typename Storage = Container>
struct ContainerIterator{
    using container_type = Container;
    using storage_type = Storage;
    static constexpr bool is_shared = true;
    using iterator_type = Iter;
    using value_type = typename QtJambiPrivate::iter_value_type<Container,Iter>::value_type;
    using iterator_category = typename QtJambiPrivate::iter_iterator_category<Iter>::iterator_category;
    using difference_type = typename QtJambiPrivate::iter_difference_type<Container,Iter>::difference_type;
    using pointer = std::remove_reference_t<value_type>*;
    using reference = std::remove_reference_t<value_type>&;
private:
    static constexpr bool is_associative = QtJambiPrivate::supports_key_v<Iter> && QtJambiPrivate::supports_value_v<Iter>;
    static constexpr bool is_mutable = QtJambiPrivate::is_writable_iterator_v<Iter,value_type>;
    Iter m_iterator;
    Storage m_storage;
    template<typename,typename,typename>
    friend struct ContainerIterator;
public:
    ContainerIterator(Iter&& iter, const Storage& c)
        : m_iterator(std::move(iter)),
          m_storage(c) {}
    ContainerIterator(Iter&& iter, Storage&& c)
        : m_iterator(std::move(iter)),
        m_storage(std::move(c)) {}
    ContainerIterator(Iter&& iter, const Storage& c, QtJambiNativeID)
        : ContainerIterator(std::move(iter),c) {}
    ContainerIterator(Iter&& iter, const Storage& c, const ConstExtendedContainerInfo&)
        : ContainerIterator(std::move(iter),c) {}
    ContainerIterator(Iter&& iter, const Storage& c, jobject)
        : ContainerIterator(std::move(iter),c) {}
    template<typename _Storage = Storage, std::enable_if_t<QtJambiPrivate::supports_new_v<_Storage,Container&,const ExtendedContainerInfo&> && !std::is_const_v<Container>, bool> = true>
    ContainerIterator(Iter&& iter, Container& c, const ExtendedContainerInfo& ci)
        : ContainerIterator(std::move(iter), Storage(c, ci)) {}
    template<typename _Storage = Storage, std::enable_if_t<QtJambiPrivate::supports_new_v<_Storage,Container&,QtJambiNativeID> && !std::is_const_v<Container>, bool> = true>
    ContainerIterator(Iter&& iter, Container& c, QtJambiNativeID nid)
        : ContainerIterator(std::move(iter), Storage(c, nid)) {}
    template<typename _Storage = Storage, std::enable_if_t<QtJambiPrivate::supports_new_v<_Storage,Container&,jobject> && !std::is_const_v<Container>, bool> = true>
    ContainerIterator(Iter&& iter, Container& c, jobject obj)
        : ContainerIterator(std::move(iter), _Storage(c, obj)) {}
    template<typename _Storage = Storage, std::enable_if_t<QtJambiPrivate::supports_new_v<_Storage,const Container&,const ExtendedContainerInfo&>, bool> = true>
    ContainerIterator(Iter&& iter, const Container& c, const ExtendedContainerInfo& ci)
        : ContainerIterator(std::move(iter), Storage(c, ci)) {}
    template<typename _Storage = Storage, std::enable_if_t<QtJambiPrivate::supports_new_v<_Storage,const Container&,QtJambiNativeID>, bool> = true>
    ContainerIterator(Iter&& iter, const Container& c, QtJambiNativeID nid)
        : ContainerIterator(std::move(iter), Storage(c, nid)) {}
    template<typename _Storage = Storage, std::enable_if_t<QtJambiPrivate::supports_new_v<_Storage,const Container&,jobject>, bool> = true>
    ContainerIterator(Iter&& iter, const Container& c, jobject obj)
        : ContainerIterator(std::move(iter), _Storage(c, obj)) {}
    template<typename Arg, typename _Storage = Storage, std::enable_if_t<QtJambiPrivate::supports_new_v<_Storage,Arg*,const ConstExtendedContainerInfo&>, bool> = true>
    ContainerIterator(Iter&& iter, Arg* c, const ConstExtendedContainerInfo& ci)
        : ContainerIterator(std::move(iter), Storage(c, ci)) {}
    template<typename Arg, typename _Storage = Storage, std::enable_if_t<QtJambiPrivate::supports_new_v<_Storage,Arg*,const ExtendedContainerInfo&>, bool> = true>
    ContainerIterator(Iter&& iter, Arg* c, const ExtendedContainerInfo& ci)
        : ContainerIterator(std::move(iter), Storage(c, ci)) {}

    ContainerIterator(const ContainerIterator& other) = default;
    ContainerIterator(ContainerIterator&& other) = default;
    ContainerIterator& operator=(const ContainerIterator& other) = default;
    ContainerIterator& operator=(ContainerIterator&& other) = default;
    const Storage& storage() const { return m_storage; }
    const Iter& iterator() const { return m_iterator; }
    Iter& iterator() { return m_iterator; }
    bool isBegin() const {
        auto begin = QtJambiPrivate::ContainerIteratorConstBegin<Storage,Iter>::function(m_storage);
        using Begin = std::remove_const_t<std::remove_reference_t<decltype(begin)>>;
        if constexpr(QtJambiPrivate::iterator_equals<Container, Iter, Begin>::value){
            return QtJambiPrivate::iterator_equals<Container, Iter, Begin>::function(m_iterator, begin);
        }else{
            Q_UNUSED(begin)
            return false;
        }
    }
    bool isEnd() const {
        auto end = QtJambiPrivate::ContainerIteratorConstEnd<Storage,Iter>::function(m_storage);
        using End = std::remove_const_t<std::remove_reference_t<decltype(end)>>;
        if constexpr(QtJambiPrivate::iterator_equals<Container, Iter, End>::value){
            return QtJambiPrivate::iterator_equals<Container, Iter, End>::function(m_iterator, end);
        }else{
            Q_UNUSED(end)
            return false;
        }
    }
    bool isValid() const {
        auto end = QtJambiPrivate::ContainerIteratorConstEnd<Storage,Iter>::function(m_storage);
        using End = std::remove_const_t<std::remove_reference_t<decltype(end)>>;
        if constexpr(QtJambiPrivate::supports_less_than_v<const Iter&, const End&>){
            auto begin = QtJambiPrivate::ContainerIteratorConstBegin<Storage,Iter>::function(m_storage);
            using Begin = std::remove_const_t<std::remove_reference_t<decltype(begin)>>;
            if(QtJambiPrivate::iterator_comparable<Container, Iter, Begin, End>::test(m_iterator, begin, end)){
                return (begin<m_iterator || begin==m_iterator) && m_iterator<end;
            }else{
                Q_UNUSED(begin)
                return false;
            }
        }else if constexpr(QtJambiPrivate::supports_initialEnd_v<const Storage>
                             && QtJambiPrivate::supports_initialBegin_v<const Storage>){
            auto begin = QtJambiPrivate::ContainerIteratorConstBegin<Storage,Iter>::function(m_storage);
            using Begin = std::remove_const_t<std::remove_reference_t<decltype(begin)>>;
            using InitialBegin = std::remove_const_t<std::remove_reference_t<decltype(m_storage.initialBegin())>>;
            using InitialEnd = std::remove_const_t<std::remove_reference_t<decltype(m_storage.initialEnd())>>;
            return QtJambiPrivate::iterator_equals<Container, End, InitialEnd>::function(end, m_storage.initialEnd())
                   && QtJambiPrivate::iterator_equals<Container, Begin, InitialBegin>::function(begin, m_storage.initialBegin())
                && !QtJambiPrivate::iterator_equals<Container, Iter, End>::function(m_iterator, end);
        }else if constexpr(QtJambiPrivate::iterator_equals<Container, Iter, End>::value){
            return !QtJambiPrivate::iterator_equals<Container, Iter, End>::function(m_iterator, end);
        }else{
            return true;
        }
    }
    operator bool() const { return isValid(); }
    bool operator!() const { return !isValid(); }
    CONTAINER_ITERATOR_IMPL(CONTAINER_ITERATOR_COMPARE)
};

template<typename Container,typename Iter>
struct ContainerIterator<Container,Iter,QtJambiNativeID>{
    using container_type = Container;
    using storage_type = QtJambiNativeID;
    static constexpr bool is_shared = false;
    using iterator_type = Iter;
    using iterator_category = std::forward_iterator_tag;
    ContainerIterator(Iter&& iter, QtJambiNativeID c) : m_iterator(std::move(iter)), m_storage(c) {}
    ContainerIterator(Iter&& iter, const Container&, QtJambiNativeID c) : ContainerIterator(std::move(iter), c) {}
    ContainerIterator(Iter&& iter, const Container&, const ConstExtendedContainerInfo& c)
        : ContainerIterator(std::move(iter),c.nativeId) {}
    ContainerIterator(Iter&& iter, Container&, const ExtendedContainerInfo& c)
        : ContainerIterator(std::move(iter),c.nativeId) {}
    QtJambiNativeID storage() const { return m_storage; }
    const Iter& iterator() const { return m_iterator; }
    Iter& iterator() { return m_iterator; }
    Q_DISABLE_COPY_MOVE(ContainerIterator)
    CONTAINER_ITERATOR_IMPL()
    Iter m_iterator;
    QtJambiNativeID m_storage;
};

template<typename Container,typename Iter>
struct ContainerIterator<Container,Iter,jobject>{
    using container_type = Container;
    using storage_type = jobject;
    static constexpr bool is_shared = false;
    using iterator_type = Iter;
    using iterator_category = std::forward_iterator_tag;
    ContainerIterator(Iter&& iter, jobject c) : m_iterator(std::move(iter)), m_storage(c) {}
    ContainerIterator(Iter&& iter, const Container&, jobject c) : ContainerIterator(std::move(iter), c) {}
    ContainerIterator(Iter&& iter, const Container&, const ConstExtendedContainerInfo& c)
        : ContainerIterator(std::move(iter),c.object) {}
    ContainerIterator(Iter&& iter, Container&, const ExtendedContainerInfo& c)
        : ContainerIterator(std::move(iter),c.object) {}
    jobject storage() const { return m_storage; }
    const Iter& iterator() const { return m_iterator; }
    Iter& iterator() { return m_iterator; }
    Q_DISABLE_COPY_MOVE(ContainerIterator)
    CONTAINER_ITERATOR_IMPL()
    Iter m_iterator;
    jobject m_storage;
};

template<typename Container,typename Iter>
ContainerIterator(Iter&& iter, const Container& c) -> ContainerIterator<Container, Iter, Container>;

template<typename Container,typename Iter>
ContainerIterator(Iter&& iter, const Container& c, QtJambiNativeID nativeId) -> ContainerIterator<Container,
                                                                                                  Iter,
                                                                                                  std::conditional_t<QtJambiPrivate::ContainerSharedInfo<Container>::is_shared && QtJambiPrivate::is_copy_constructible_v<Container>,
                                                                                                                     Container, QtJambiPrivate::ContainerRef<Container>>>;

template<typename Container,typename Iter>
ContainerIterator(Iter&& iter, const Container& c, const ConstExtendedContainerInfo&) -> ContainerIterator<Container,
                                                                                                  Iter,
                                                                                                  std::conditional_t<QtJambiPrivate::ContainerSharedInfo<Container>::is_shared && QtJambiPrivate::is_copy_constructible_v<Container>,
                                                                                                                     Container, QtJambiPrivate::ContainerRef<Container>>>;

template<typename Container,typename Iter>
ContainerIterator(Iter&& iter, const Container& c, jobject o) -> ContainerIterator<Container,
                                                                                   Iter,
                                                                                   std::conditional_t<QtJambiPrivate::ContainerSharedInfo<Container>::is_shared && QtJambiPrivate::is_copy_constructible_v<Container>,
                                                                                                      Container, jobject>>;

template<typename Container,typename Iter>
ContainerIterator(Iter&& iter, Container& c, QtJambiNativeID nativeId) -> ContainerIterator<Container, Iter,
                                                                                            std::conditional_t<QtJambiPrivate::ContainerSharedInfo<Container>::is_shared,
                                                                                                               std::conditional_t<QtJambiPrivate::is_writable_iterator_of_container_v<Container, Iter>,
                                                                                                                                  QtJambiPrivate::ContainerRef<Container>,
                                                                                                                                  Container>,
                                                                                                               QtJambiPrivate::ContainerRef<Container>>>;
template<typename Container,typename Iter>
ContainerIterator(Iter&& iter, Container& c, const ExtendedContainerInfo&) -> ContainerIterator<Container, Iter,
                                                                                            std::conditional_t<QtJambiPrivate::ContainerSharedInfo<Container>::is_shared,
                                                                                                               std::conditional_t<QtJambiPrivate::is_writable_iterator_of_container_v<Container, Iter>,
                                                                                                                                  QtJambiPrivate::ContainerRef<Container>,
                                                                                                                                  Container>,
                                                                                                               QtJambiPrivate::ContainerRef<Container>>>;

namespace Iterators{
    template<typename Container>
    using const_iterator = ContainerIterator<Container,typename Container::const_iterator>;

    template<typename Container>
    using iterator = ContainerIterator<Container,typename Container::iterator>;

    template<typename Container>
    using const_reverse_iterator = ContainerIterator<Container,typename Container::const_reverse_iterator>;

    template<typename Container>
    using reverse_iterator = ContainerIterator<Container,typename Container::reverse_iterator>;

    template<typename Container>
    using const_key_value_iterator = ContainerIterator<Container,typename Container::const_key_value_iterator>;

    template<typename Container>
    using key_value_iterator = ContainerIterator<Container,typename Container::key_value_iterator>;

    template<typename Container>
    using sentinel = ContainerIterator<Container,typename Container::sentinel>;
}

enum class SequentialContainerType{
    QList,
    QStack,
    QQueue,
    QSet
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    ,QSpan
    ,QConstSpan
#endif
};

enum class AssociativeContainerType{
    QHash,
    QMap,
    QMultiHash,
    QMultiMap,
    QPair
};

class QTJAMBI_EXPORT AbstractContainerAccess{
public:
    enum DataType : quint8{
        Value,
        Pointer = 0x01,
        PointerToQObject = 0x02,
        FunctionPointer = 0x04,
        PointersMask = 0x0f
    };
    virtual void dispose();
    virtual AbstractContainerAccess* clone() = 0;
    virtual void assign(void* container, const void* other) = 0;
    virtual void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) = 0;
    virtual size_t sizeOf() const = 0;
    virtual size_t alignOf() const = 0;
    virtual void* constructContainer(void* placement) = 0;
    virtual void* constructContainer(void* placement, const void* copyOf) = 0;
    virtual void* constructContainer(JNIEnv * env, void* placement, const ConstContainerAndAccessInfo& copyOf) = 0;
    virtual void* constructContainer(void* placement, void* move) = 0;
    virtual void* constructContainer(JNIEnv * env, void* placement, const ContainerAndAccessInfo& move) = 0;
    void* createContainer(void* moved);
    void* createContainer(JNIEnv *env, const ContainerAndAccessInfo& moved);
    virtual bool destructContainer(void* container) = 0;
    virtual QMetaType registerContainer(QByteArrayView containerTypeName) = 0;
    virtual const QObject* getOwner(const void* container);
    virtual bool hasOwnerFunction();
    void* createContainer();
    void* createContainer(const void* copy);
    void* createContainer(JNIEnv *env, const ConstContainerAndAccessInfo& other);
    void deleteContainer(void* container);
    static bool isPointerType(const QMetaType& metaType);
    Q_DISABLE_COPY_MOVE(AbstractContainerAccess)
protected:
    static DataType dataType(const QMetaType& metaType, const QSharedPointer<AbstractContainerAccess>& access);
    AbstractContainerAccess();
    virtual ~AbstractContainerAccess();
    enum ContainerType{
        SequentialConstIterator = 0x000001,
        AssociativeConstIterator = 0x000002 | SequentialConstIterator,
        MutableIterable = 0x0000004,
        SequentialIterator = SequentialConstIterator | MutableIterable,
        AssociativeIterator = AssociativeConstIterator | MutableIterable,
        Sequential = 0x000010,
        Span = 0x000020 | Sequential,
        List = 0x000040 | Sequential,
        Set = 0x000080 | Sequential,
        Associative = 0x000100,
        Hash = 0x000200 | Associative,
        Map = 0x000400 | Associative,
        MultiAssociative = 0x000800,
        MultiHash = Hash | MultiAssociative,
        MultiMap = Map | MultiAssociative,
        Pair = 0x001000,
        AutoAccess = 0x010000,
        TemplateAccess = 0x020000
    };
    virtual ContainerType containerType() const = 0;
public:
    virtual class AbstractReferenceCountingContainer* asRC();
#define DECL_ACCESS_TYPE_TEST(Type) inline bool is##Type() const { return (containerType() & Type)==Type; }
    DECL_ACCESS_TYPE_TEST(SequentialConstIterator)
    DECL_ACCESS_TYPE_TEST(AssociativeConstIterator)
    DECL_ACCESS_TYPE_TEST(MutableIterable);
    DECL_ACCESS_TYPE_TEST(SequentialIterator)
    DECL_ACCESS_TYPE_TEST(AssociativeIterator)
    DECL_ACCESS_TYPE_TEST(Sequential)
    DECL_ACCESS_TYPE_TEST(Span)
    DECL_ACCESS_TYPE_TEST(List)
    DECL_ACCESS_TYPE_TEST(Set)
    DECL_ACCESS_TYPE_TEST(Associative)
    DECL_ACCESS_TYPE_TEST(Hash)
    DECL_ACCESS_TYPE_TEST(Map)
    DECL_ACCESS_TYPE_TEST(MultiAssociative)
    DECL_ACCESS_TYPE_TEST(MultiHash)
    DECL_ACCESS_TYPE_TEST(MultiMap)
    DECL_ACCESS_TYPE_TEST(Pair)
    DECL_ACCESS_TYPE_TEST(AutoAccess)
    DECL_ACCESS_TYPE_TEST(TemplateAccess)
    friend class AbstractSequentialConstIteratorAccess;
    friend class AbstractAssociativeConstIteratorAccess;
    friend class AbstractSequentialIteratorAccess;
    friend class AbstractAssociativeIteratorAccess;
    friend class AbstractSequentialAccess;
    friend class AbstractAssociativeAccess;
    friend class AbstractSpanAccess;
    friend class AbstractListAccess;
    friend class AbstractSetAccess;
    friend class AbstractHashAccess;
    friend class AbstractMapAccess;
    friend class AbstractMultiHashAccess;
    friend class AbstractMultiMapAccess;
    friend class AbstractPairAccess;
};

class QTJAMBI_EXPORT AbstractReferenceCountingContainer{
protected:
    AbstractReferenceCountingContainer() = default;
    virtual ~AbstractReferenceCountingContainer();
    virtual void updateRC(JNIEnv * env, const ContainerInfo& container) = 0;
    void swapRC(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2);
    jobject findContainer(JNIEnv * env, jobject container);
    static void unfoldAndAddContainer(JNIEnv * env, jobject set, const void* data, AbstractContainerAccess::DataType dataType, const QMetaType& metaType, AbstractContainerAccess* access);
    static void unfoldAndAddContainer(JNIEnv * env, jobject set, jobject value);
public:
    virtual class ReferenceCountingSetContainer* asRCSet();
    virtual class ReferenceCountingMapContainer* asRCMap();
    virtual class ReferenceCountingMultiMapContainer* asRCMultiMap();

    template<typename>
    friend class ContainerLink;
};

class QTJAMBI_EXPORT ReferenceCountingSetContainer : public AbstractReferenceCountingContainer{
protected:
    ReferenceCountingSetContainer() = default;
    jobject rcContainer(JNIEnv * env, jobject container);
    void assignRC(JNIEnv * env, jobject container, jobject container2);
    void assignUniqueRC(JNIEnv * env, jobject container, jobject container2);
    void clearRC(JNIEnv * env, jobject container);
    void addRC(JNIEnv * env, jobject container, jobject value);
    void addUniqueRC(JNIEnv * env, jobject container, jobject value);
    void removeRC(JNIEnv * env, jobject container, jobject value);
    void removeRC(JNIEnv * env, jobject container, jobject value, int n);
    void addAllRC(JNIEnv * env, jobject container, jobject container2);
    void addAllUniqueRC(JNIEnv * env, jobject container, jobject container2);
    void addNestedValueRC(JNIEnv * env, jobject container, AbstractContainerAccess::DataType dataType, bool isContainer, jobject value);
public:
    ReferenceCountingSetContainer* asRCSet() override final;
};

class QTJAMBI_EXPORT ReferenceCountingMapContainer : public AbstractReferenceCountingContainer{
protected:
    ReferenceCountingMapContainer() = default;
    jobject rcContainer(JNIEnv * env, jobject container);
    void clearRC(JNIEnv * env, jobject container);
    void assignRC(JNIEnv * env, jobject container, jobject container2);
    void putAllRC(JNIEnv * env, jobject container, jobject container2);
    void putRC(JNIEnv * env, jobject container, jobject key, jobject value);
    void removeRC(JNIEnv * env, jobject container, jobject key, int n = 1);
public:
    ReferenceCountingMapContainer* asRCMap() override final;
};

class QTJAMBI_EXPORT ReferenceCountingMultiMapContainer : public AbstractReferenceCountingContainer{
protected:
    ReferenceCountingMultiMapContainer() = default;
    jobject rcContainer(JNIEnv * env, jobject container);
    void clearRC(JNIEnv * env, jobject container);
    void assignRC(JNIEnv * env, jobject container, jobject container2);
    void putAllRC(JNIEnv * env, jobject container, jobject container2);
    void putRC(JNIEnv * env, jobject container, jobject key, jobject value);
    void removeRC(JNIEnv * env, jobject container, jobject key, int n = 1);
    void removeRC(JNIEnv * env, jobject container, jobject key, jobject value, int n = 1);
    static jobject newRCMultiMap(JNIEnv * env);
public:
    ReferenceCountingMultiMapContainer* asRCMultiMap() override final;
};

class QTJAMBI_EXPORT AbstractSequentialConstIteratorAccess : public AbstractContainerAccess{
protected:
    ~AbstractSequentialConstIteratorAccess() override;
    AbstractSequentialConstIteratorAccess();
    ContainerType containerType() const override;
public:
    enum class IteratorType{
        iterator,
        const_iterator,
        reverse_iterator,
        const_reverse_iterator,
        key_iterator,
        key_value_iterator,
        const_key_value_iterator,
        sentinel
    };
    enum class IteratorStorage{
        Clone,
        Ref
    };

    virtual IteratorType iteratorType() const;
    virtual IteratorStorage iteratorStorage() const;
    #define DECL_ITERATOR_TYPE_TEST(Type) inline bool is_##Type() const { return iteratorType()==IteratorType::Type; }
    DECL_ITERATOR_TYPE_TEST(iterator)
    DECL_ITERATOR_TYPE_TEST(const_iterator)
    DECL_ITERATOR_TYPE_TEST(key_iterator)
    DECL_ITERATOR_TYPE_TEST(key_value_iterator)
    DECL_ITERATOR_TYPE_TEST(const_key_value_iterator)
    DECL_ITERATOR_TYPE_TEST(reverse_iterator)
    DECL_ITERATOR_TYPE_TEST(const_reverse_iterator)
    DECL_ITERATOR_TYPE_TEST(sentinel)
    AbstractSequentialConstIteratorAccess* clone() override = 0;
    virtual std::optional<const void*> value(const void* iterator) = 0;
    virtual std::optional<jint> intValue(const void* iterator) = 0;
    virtual std::optional<jlong> longValue(const void* iterator) = 0;
    virtual std::optional<jshort> shortValue(const void* iterator) = 0;
    virtual std::optional<jbyte> byteValue(const void* iterator) = 0;
    virtual std::optional<jfloat> floatValue(const void* iterator) = 0;
    virtual std::optional<jdouble> doubleValue(const void* iterator) = 0;
    virtual std::optional<jchar> charValue(const void* iterator) = 0;
    virtual std::optional<jboolean> booleanValue(const void* iterator) = 0;
    virtual QVariant variantValue(const void* iterator) = 0;
    template<typename T = void>
    auto getValue(const void* iterator){
        if constexpr(std::is_same_v<T,jint>){
            return intValue(iterator);
        }else if constexpr(std::is_same_v<T,jlong>){
            return longValue(iterator);
        }else if constexpr(std::is_same_v<T,jshort>){
            return shortValue(iterator);
        }else if constexpr(std::is_same_v<T,jbyte>){
            return byteValue(iterator);
        }else if constexpr(std::is_same_v<T,jfloat>){
            return floatValue(iterator);
        }else if constexpr(std::is_same_v<T,jdouble>){
            return doubleValue(iterator);
        }else if constexpr(std::is_same_v<T,jchar>){
            return charValue(iterator);
        }else if constexpr(std::is_same_v<T,jboolean>){
            return booleanValue(iterator);
        }else if constexpr(std::is_same_v<T,QVariant>){
            return variantValue(iterator);
        }else{
            return value(iterator);
        }
    }
    virtual std::optional<size_t> distance(const void* iterator, const void* other) = 0;
    virtual jobject value(JNIEnv * env, const void* iterator) = 0;
    virtual void increment(JNIEnv * env, void* iterator) = 0;
    virtual void decrement(JNIEnv * env, void* iterator) = 0;
    virtual void increment(void* iterator) = 0;
    virtual void decrement(void* iterator) = 0;
    virtual void advance(JNIEnv * env, void* iterator, qsizetype n);
    virtual bool advance(void* iterator, qsizetype n);
    virtual jboolean lessThan(JNIEnv * env, const void* iterator, const void* other) = 0;
    virtual std::optional<bool> lessThan(const void* iterator, const void* other) = 0;
    virtual bool canLess() = 0;
    virtual bool canDistance() = 0;
    virtual bool isBidirectionalIterator() = 0;
    virtual bool isContiguousIterator() = 0;
    virtual bool isRandomAccessIterator() = 0;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    virtual AbstractSpanAccess* createSpanAccess() = 0;
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    virtual bool isBegin(JNIEnv *env, const void* iterator) = 0;
    virtual std::optional<bool> isBegin(const void* iterator) = 0;
    virtual bool isEnd(JNIEnv *env, const void* iterator) = 0;
    virtual std::optional<bool> isEnd(const void* iterator) = 0;
    virtual bool isValid(JNIEnv *env, const void* iterator) = 0;
    virtual std::optional<bool> isValid(const void* iterator) = 0;
    virtual jboolean equals(JNIEnv * env, const void* iterator, const void* other) = 0;
    virtual jboolean equals(JNIEnv * env, const void* iterator, const ConstContainerAndAccessInfo& other) = 0;
    virtual bool equals(const void* iterator, const void* other) = 0;
    virtual const QMetaType& valueMetaType() = 0;
    virtual void* constructContainer(void* placement, const void* copyOf) override = 0;
    virtual bool canCopy() const = 0;
    virtual void* asIterator(void* iterator) = 0;
    virtual bool findIterator(const void* iterator, const std::type_info& typeId, void* output) = 0;
    virtual std::pair<void*,AbstractSequentialConstIteratorAccess*> createConstIterator(const void* iterator) = 0;
private:
    void* constructContainer(void* placement) override;
    void* constructContainer(void* placement, void* move) override;
    void* constructContainer(JNIEnv *, void* result, const ConstContainerAndAccessInfo& container) override;
    void* constructContainer(JNIEnv *, void* result, const ContainerAndAccessInfo& move) override;
    bool destructContainer(void* container) override;
    void assign(void*, const void* ) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    QMetaType registerContainer(QByteArrayView) final override;
    Q_DISABLE_COPY_MOVE(AbstractSequentialConstIteratorAccess)
};

class QTJAMBI_EXPORT AbstractSequentialIteratorAccess : public AbstractSequentialConstIteratorAccess{
protected:
    ~AbstractSequentialIteratorAccess() override;
    AbstractSequentialIteratorAccess();
    ContainerType containerType() const override;
    IteratorType iteratorType() const override;
    IteratorStorage iteratorStorage() const override;
public:
    virtual void setValue(JNIEnv * env, void* iterator, jobject newValue) = 0;
    using AbstractSequentialConstIteratorAccess::value;
    virtual std::optional<void*> value(void* iterator) = 0;
    virtual bool setIntValue(void* iterator, jint value) = 0;
    virtual bool setLongValue(void* iterator, jlong value) = 0;
    virtual bool setShortValue(void* iterator, jshort value) = 0;
    virtual bool setByteValue(void* iterator, jbyte value) = 0;
    virtual bool setFloatValue(void* iterator, jfloat value) = 0;
    virtual bool setDoubleValue(void* iterator, jdouble value) = 0;
    virtual bool setCharValue(void* iterator, jchar value) = 0;
    virtual bool setBooleanValue(void* iterator, jboolean value) = 0;
    virtual bool setVariantValue(void* iterator, const QVariant& value) = 0;
    template<typename T>
    bool setValue(void* iterator, const T& v){
        if constexpr(std::is_same_v<T,jint>){
            return setIntValue(iterator, v);
        }else if constexpr(std::is_same_v<T,jlong>){
            return setLongValue(iterator, v);
        }else if constexpr(std::is_same_v<T,jshort>){
            return setShortValue(iterator, v);
        }else if constexpr(std::is_same_v<T,jbyte>){
            return setByteValue(iterator, v);
        }else if constexpr(std::is_same_v<T,jfloat>){
            return setFloatValue(iterator, v);
        }else if constexpr(std::is_same_v<T,jdouble>){
            return setDoubleValue(iterator, v);
        }else if constexpr(std::is_same_v<T,jchar>){
            return setCharValue(iterator, v);
        }else if constexpr(std::is_same_v<T,jboolean>){
            return setBooleanValue(iterator, v);
        }else if constexpr(std::is_same_v<T,QVariant>){
            return setVariantValue(iterator, v);
        }else{
            std::optional<void*> opt = value(iterator);
            if(opt.has_value()){
                *reinterpret_cast<T*>(opt.value()) = v;
            }
            return opt.has_value();
        }
    }
private:
    Q_DISABLE_COPY_MOVE(AbstractSequentialIteratorAccess)
};

class QTJAMBI_EXPORT AbstractAssociativeConstIteratorAccess : public AbstractSequentialConstIteratorAccess{
protected:
    ~AbstractAssociativeConstIteratorAccess() override;
    AbstractAssociativeConstIteratorAccess();
    ContainerType containerType() const override;
    IteratorType iteratorType() const override;
    IteratorStorage iteratorStorage() const override;
public:
    virtual jobject key(JNIEnv * env, const void* iterator) = 0;
    virtual std::optional<const void*> key(const void* iterator) = 0;
    virtual std::optional<jint> intKey(const void* iterator) = 0;
    virtual std::optional<jlong> longKey(const void* iterator) = 0;
    virtual std::optional<jshort> shortKey(const void* iterator) = 0;
    virtual std::optional<jbyte> byteKey(const void* iterator) = 0;
    virtual std::optional<jfloat> floatKey(const void* iterator) = 0;
    virtual std::optional<jdouble> doubleKey(const void* iterator) = 0;
    virtual std::optional<jchar> charKey(const void* iterator) = 0;
    virtual std::optional<jboolean> booleanKey(const void* iterator) = 0;
    virtual QVariant variantKey(const void* iterator) = 0;
    template<typename T = void>
    auto getKey(const void* iterator){
        if constexpr(std::is_same_v<T,jint>){
            return intKey(iterator);
        }else if constexpr(std::is_same_v<T,jlong>){
            return longKey(iterator);
        }else if constexpr(std::is_same_v<T,jshort>){
            return shortKey(iterator);
        }else if constexpr(std::is_same_v<T,jbyte>){
            return byteKey(iterator);
        }else if constexpr(std::is_same_v<T,jfloat>){
            return floatKey(iterator);
        }else if constexpr(std::is_same_v<T,jdouble>){
            return doubleKey(iterator);
        }else if constexpr(std::is_same_v<T,jchar>){
            return charKey(iterator);
        }else if constexpr(std::is_same_v<T,jboolean>){
            return booleanKey(iterator);
        }else if constexpr(std::is_same_v<T,QVariant>){
            return variantKey(iterator);
        }else{
            return key(iterator);
        }
    }
    virtual const QMetaType& keyMetaType() = 0;
    bool isContiguousIterator() override { return false; }
    bool isRandomAccessIterator() override { return false; }
private:
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    AbstractSpanAccess* createSpanAccess() final override {return nullptr;}
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    Q_DISABLE_COPY_MOVE(AbstractAssociativeConstIteratorAccess)
};

class QTJAMBI_EXPORT AbstractAssociativeIteratorAccess : public AbstractAssociativeConstIteratorAccess{
protected:
    ~AbstractAssociativeIteratorAccess() override;
    AbstractAssociativeIteratorAccess();
    ContainerType containerType() const override;
    IteratorType iteratorType() const override;
    IteratorStorage iteratorStorage() const override;
public:
    virtual void setValue(JNIEnv * env, void* iterator, jobject newValue) = 0;
    virtual bool setIntValue(void* iterator, jint value) = 0;
    virtual bool setLongValue(void* iterator, jlong value) = 0;
    virtual bool setShortValue(void* iterator, jshort value) = 0;
    virtual bool setByteValue(void* iterator, jbyte value) = 0;
    virtual bool setFloatValue(void* iterator, jfloat value) = 0;
    virtual bool setDoubleValue(void* iterator, jdouble value) = 0;
    virtual bool setCharValue(void* iterator, jchar value) = 0;
    virtual bool setBooleanValue(void* iterator, jboolean value) = 0;
    virtual bool setVariantValue(void* iterator, const QVariant& value) = 0;
    using AbstractAssociativeConstIteratorAccess::value;
    virtual std::optional<void*> value(void* iterator) = 0;
    template<typename T>
    bool setValue(void* iterator, const T& v){
        if constexpr(std::is_same_v<T,jint>){
            return setIntValue(iterator, v);
        }else if constexpr(std::is_same_v<T,jlong>){
            return setLongValue(iterator, v);
        }else if constexpr(std::is_same_v<T,jshort>){
            return setShortValue(iterator, v);
        }else if constexpr(std::is_same_v<T,jbyte>){
            return setByteValue(iterator, v);
        }else if constexpr(std::is_same_v<T,jfloat>){
            return setFloatValue(iterator, v);
        }else if constexpr(std::is_same_v<T,jdouble>){
            return setDoubleValue(iterator, v);
        }else if constexpr(std::is_same_v<T,jchar>){
            return setCharValue(iterator, v);
        }else if constexpr(std::is_same_v<T,jboolean>){
            return setBooleanValue(iterator, v);
        }else if constexpr(std::is_same_v<T,QVariant>){
            return setVariantValue(iterator, v);
        }else{
            std::optional<void*> opt = value(iterator);
            if(opt.has_value()){
                *reinterpret_cast<T*>(opt.value()) = v;
            }
            return opt.has_value();
        }
    }
private:
    Q_DISABLE_COPY_MOVE(AbstractAssociativeIteratorAccess)
};

typedef bool (*ElementAnalyzer)(const void* element, void* data);
typedef bool (*EntryAnalyzer)(const void* key, const void* value, void* data);

class QTJAMBI_EXPORT AbstractSequentialAccess : public AbstractContainerAccess{
protected:
    ~AbstractSequentialAccess() override;
    AbstractSequentialAccess();
    ContainerType containerType() const override;
public:
    AbstractSequentialAccess* clone() override = 0;
    virtual bool isDetached(const void* container) = 0;
    virtual void detach(const ContainerInfo& container) = 0;
    virtual bool isSharedWith(const void* container, const void* container2) = 0;
    virtual void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) = 0;
    virtual qsizetype size(JNIEnv * env, const void* container) = 0;
    virtual qsizetype size(const void* container) = 0;
    virtual void clear(JNIEnv * env, const ContainerInfo& container) = 0;
    virtual jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) = 0;
    virtual jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) = 0;
    virtual const QMetaType& elementMetaType() = 0;
    virtual DataType elementType() = 0;
    virtual AbstractContainerAccess* elementNestedContainerAccess() = 0;
    virtual bool hasNestedContainerAccess() = 0;
    virtual bool hasNestedPointers() = 0;
    class QTJAMBI_EXPORT ElementIterator{
    public:
        ElementIterator() = default;
        virtual ~ElementIterator();
    protected:
        virtual AbstractSequentialAccess* access() = 0;
    public:
        virtual const QMetaType& elementMetaType();
        virtual DataType elementType();
        virtual AbstractContainerAccess* elementNestedContainerAccess();
        virtual bool hasNestedContainerAccess();
        virtual bool hasNestedPointers();
        virtual bool hasNext() = 0;
        virtual jobject next(JNIEnv * env) = 0;
        virtual const void* next() = 0;
        virtual bool isConst() = 0;
        virtual const void* constNext() = 0;
        virtual void* mutableNext() = 0;
        virtual bool operator==(const ElementIterator& other) const = 0;
        virtual std::unique_ptr<ElementIterator> clone() const = 0;
        virtual std::function<jobject(JNIEnv*,const void*)> elementConverter() const = 0;
    };
    virtual std::unique_ptr<ElementIterator> elementIterator(const void*) = 0;
    virtual std::unique_ptr<ElementIterator> elementIterator(void*) = 0;
    inline std::unique_ptr<ElementIterator> constElementIterator(const void* container){
        return elementIterator(container);
    }
private:
    virtual class AbstractNestedSequentialAccess* asNested();
    Q_DISABLE_COPY_MOVE(AbstractSequentialAccess)
    friend void registerNestedAccess(struct QtJambiStorage* storage, class QWriteLocker &locker, AbstractContainerAccess* access);
};

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
class QTJAMBI_EXPORT AbstractSpanAccess : public AbstractSequentialAccess{
protected:
    ~AbstractSpanAccess() override;
    AbstractSpanAccess();
    ContainerType containerType() const override final;
public:
    AbstractSpanAccess* clone() override = 0;
    virtual bool isConst() = 0;
    virtual qsizetype size_bytes(JNIEnv * env, const void* container) = 0;
    virtual jobject get(JNIEnv *,const void*,qsizetype) = 0;
    virtual bool set(JNIEnv *,const ContainerInfo&,qsizetype,jobject) = 0;
    virtual const void* get(const void*,qsizetype) = 0;
    virtual bool set(void*,qsizetype,const void*) = 0;
    virtual jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) = 0;
    virtual jobject end(JNIEnv * env, const ExtendedContainerInfo& container) = 0;
    virtual jobject reverseBegin(JNIEnv * env, const ExtendedContainerInfo& container) = 0;
    virtual jobject reverseEnd(JNIEnv * env, const ExtendedContainerInfo& container) = 0;
    virtual jobject constReverseBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) = 0;
    virtual jobject constReverseEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) = 0;
    Q_DISABLE_COPY_MOVE(AbstractSpanAccess)
private:
    friend class WrapperSpanAccess;
    bool isDetached(const void* container) final override;
    void detach(const ContainerInfo& container) final override;
    bool isSharedWith(const void* container, const void* container2) final override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) final override;
    void clear(JNIEnv * env, const ContainerInfo& container) final override;
};
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)

class QTJAMBI_EXPORT AbstractListAccess : public AbstractSequentialAccess{
protected:
    ~AbstractListAccess() override;
    AbstractListAccess();
    ContainerType containerType() const override final;
public:
    AbstractListAccess* clone() override = 0;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    virtual AbstractSpanAccess* createSpanAccess(bool isConst) = 0;
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    virtual void appendList(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) = 0;
    virtual jobject at(JNIEnv * env, const void* container, qsizetype index) = 0;
    virtual const void* at(const void* container, qsizetype index) = 0;
    virtual void* at(void* container, qsizetype index) = 0;
    virtual jobject value(JNIEnv * env, const void* container, qsizetype index) = 0;
    virtual jobject value(JNIEnv * env, const void* container, qsizetype index, jobject defaultValue) = 0;
    virtual void swapItemsAt(JNIEnv * env, const ContainerInfo& container, qsizetype index1, qsizetype index2) = 0;
    virtual jboolean startsWith(JNIEnv * env, const void* container, jobject value) = 0;
    virtual void reserve(void* container, qsizetype size) = 0;
    virtual void reserve(JNIEnv * env, const ContainerInfo& container, qsizetype size) = 0;
    virtual void replace(JNIEnv * env, const ContainerInfo& container, qsizetype index, jobject value) = 0;
    virtual void replace(void* container, qsizetype index, const void* value) = 0;
    virtual void remove(JNIEnv * env, const ContainerInfo& container, qsizetype index, qsizetype n) = 0;
    virtual void remove(void* container, qsizetype pos, qsizetype n) = 0;
    virtual qsizetype removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) = 0;
    virtual jboolean equal(JNIEnv * env, const void* container, jobject other) = 0;
    virtual void move(JNIEnv * env, const ContainerInfo& container, qsizetype index1, qsizetype index2) = 0;
    virtual ContainerAndAccessInfo mid(JNIEnv * env, const ConstContainerAndAccessInfo& container, qsizetype index1, qsizetype index2) = 0;
    virtual qsizetype lastIndexOf(JNIEnv * env, const void* container, jobject value, qsizetype index) = 0;
    virtual void insert(JNIEnv * env, const ContainerInfo& container, qsizetype index, qsizetype n, jobject value) = 0;
    virtual void insert(void* container, qsizetype index, qsizetype n, const void* entry) = 0;
    virtual bool append(void* container, const void* entry) = 0;
    virtual qsizetype indexOf(JNIEnv * env, const void* container, jobject value, qsizetype index) = 0;
    virtual jboolean endsWith(JNIEnv * env, const void* container, jobject value) = 0;
    virtual qsizetype count(JNIEnv * env, const void* container, jobject value) = 0;
    virtual jboolean contains(JNIEnv * env, const void* container, jobject value) = 0;
    virtual jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) = 0;
    virtual jobject end(JNIEnv * env, const ExtendedContainerInfo& container) = 0;
    virtual jobject reverseBegin(JNIEnv * env, const ExtendedContainerInfo& container) = 0;
    virtual jobject reverseEnd(JNIEnv * env, const ExtendedContainerInfo& container) = 0;
    virtual jobject constReverseBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) = 0;
    virtual jobject constReverseEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) = 0;
    virtual void resize(void* container, qsizetype newSize) = 0;
    virtual qsizetype capacity(JNIEnv * env, const void* container) = 0;
    virtual void fill(JNIEnv * env, const ContainerInfo& container, jobject value, qsizetype size) = 0;
    virtual void resize(JNIEnv * env, const ContainerInfo& container, qsizetype newSize) = 0;
    virtual void squeeze(JNIEnv * env, const ContainerInfo& container) = 0;
    Q_DISABLE_COPY_MOVE(AbstractListAccess)
};

class QTJAMBI_EXPORT AbstractSetAccess : public AbstractSequentialAccess{
protected:
    ~AbstractSetAccess() override;
    AbstractSetAccess();
    ContainerType containerType() const override final;
public:
    AbstractSetAccess* clone() override = 0;
    virtual qsizetype capacity(JNIEnv * env, const void* container) = 0;
    virtual jboolean contains(JNIEnv * env, const void* container, jobject value) = 0;
    virtual void insert(void* container, const void* entry) = 0;
    virtual void insert(JNIEnv * env, const ContainerInfo& container, jobject value) = 0;
    virtual void intersect(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) = 0;
    virtual jboolean intersects(JNIEnv * env, const void* container, jobject other) = 0;
    virtual jboolean equal(JNIEnv * env, const void* container, jobject other) = 0;
    virtual jboolean remove(JNIEnv * env, const ContainerInfo& container, jobject value) = 0;
    virtual void reserve(void* container, qsizetype size) = 0;
    virtual void reserve(JNIEnv * env, const ContainerInfo& container, qsizetype newSize) = 0;
    virtual void subtract(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) = 0;
    virtual void unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) = 0;
    virtual ContainerAndAccessInfo values(JNIEnv * env, const ConstContainerInfo& container) = 0;
    Q_DISABLE_COPY_MOVE(AbstractSetAccess)
};

class QTJAMBI_EXPORT AbstractAssociativeAccess : public AbstractContainerAccess{
protected:
    ~AbstractAssociativeAccess() override;
    AbstractAssociativeAccess();
    ContainerType containerType() const override;
public:
    AbstractAssociativeAccess* clone() override = 0;
    virtual bool isDetached(const void* container) = 0;
    virtual void detach(const ContainerInfo& container) = 0;
    virtual bool isSharedWith(const void* container, const void* container2) = 0;
    virtual void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) = 0;
    virtual const QMetaType& keyMetaType() = 0;
    virtual const QMetaType& valueMetaType() = 0;
    virtual DataType keyType() = 0;
    virtual DataType valueType() = 0;
    virtual AbstractContainerAccess* keyNestedContainerAccess() = 0;
    virtual AbstractContainerAccess* valueNestedContainerAccess() = 0;
    virtual bool hasKeyNestedContainerAccess() = 0;
    virtual bool hasValueNestedContainerAccess() = 0;
    virtual bool hasKeyNestedPointers() = 0;
    virtual bool hasValueNestedPointers() = 0;
    virtual void clear(JNIEnv *, const ContainerInfo& container) = 0;
    virtual qsizetype size(JNIEnv *,const void*) = 0;
    virtual qsizetype size(const void* container) = 0;
    virtual jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) = 0;
    virtual jobject end(JNIEnv * env, const ExtendedContainerInfo& container) = 0;
    virtual jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) = 0;
    virtual jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) = 0;
    virtual jobject keyBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) = 0;
    virtual jobject keyEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) = 0;
    virtual jobject keyValueBegin(JNIEnv * env, const ExtendedContainerInfo& container) = 0;
    virtual jobject keyValueEnd(JNIEnv * env, const ExtendedContainerInfo& container) = 0;
    virtual jobject constKeyValueBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) = 0;
    virtual jobject constKeyValueEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) = 0;
    virtual jboolean contains(JNIEnv *,const void*,jobject) = 0;
    virtual bool contains(const void*,const void*) = 0;
    virtual qsizetype count(JNIEnv *,const void*,jobject) = 0;
    virtual jobject find(JNIEnv *,const ExtendedContainerInfo& container, jobject) = 0;
    virtual jobject constFind(JNIEnv *,const ConstExtendedContainerInfo&,jobject) = 0;
    virtual void insert(JNIEnv *, const ContainerInfo& container,jobject,jobject) = 0;
    virtual void insert(void* container,const void* key, const void* value) = 0;
    virtual jobject key(JNIEnv *,const void*,jobject,jobject) = 0;
    virtual ContainerAndAccessInfo keys(JNIEnv *, const ConstContainerInfo& container) = 0;
    virtual ContainerAndAccessInfo keys(JNIEnv *,const ConstContainerInfo& container,jobject) = 0;
    virtual jboolean equal(JNIEnv *,const void*,jobject) = 0;
    virtual qsizetype remove(JNIEnv *, const ContainerInfo& container,jobject) = 0;
    virtual jobject take(JNIEnv *, const ContainerInfo& container,jobject) = 0;
    virtual jobject value(JNIEnv *,const void*,jobject,jobject) = 0;
    virtual const void* value(const void*, const void*, const void* = nullptr) = 0;
    virtual ContainerAndAccessInfo values(JNIEnv *, const ConstContainerInfo& container) = 0;
    class QTJAMBI_EXPORT KeyValueIterator{
    public:
        KeyValueIterator() = default;
        virtual ~KeyValueIterator();
    protected:
        virtual AbstractAssociativeAccess* access() = 0;
    public:
        virtual const QMetaType& keyMetaType();
        virtual const QMetaType& valueMetaType();
        virtual DataType keyType();
        virtual DataType valueType();
        virtual AbstractContainerAccess* keyNestedContainerAccess();
        virtual AbstractContainerAccess* valueNestedContainerAccess();
        virtual bool hasKeyNestedContainerAccess();
        virtual bool hasValueNestedContainerAccess();
        virtual bool hasKeyNestedPointers();
        virtual bool hasValueNestedPointers();
        virtual bool hasNext() = 0;
        virtual QPair<jobject,jobject> next(JNIEnv * env) = 0;
        virtual bool isConst() = 0;
        virtual QPair<const void*,const void*> next() = 0;
        virtual QPair<const void*,const void*> constNext() = 0;
        virtual QPair<const void*,void*> mutableNext() = 0;
        virtual bool operator==(const KeyValueIterator& other) const = 0;
        virtual std::unique_ptr<KeyValueIterator> clone() const = 0;
        std::unique_ptr<AbstractSequentialAccess::ElementIterator> nextAsIterator();
        virtual std::function<jobject(JNIEnv*,const void*)> keyConverter() const = 0;
        virtual std::function<jobject(JNIEnv*,const void*)> valueConverter() const = 0;
    };
    virtual std::unique_ptr<KeyValueIterator> keyValueIterator(const void*) = 0;
    virtual std::unique_ptr<KeyValueIterator> keyValueIterator(void*) = 0;
    inline std::unique_ptr<KeyValueIterator> constKeyValueIterator(const void* container){
        return keyValueIterator(container);
    }
    static std::unique_ptr<AbstractSequentialAccess::ElementIterator> asKeyIterator(std::unique_ptr<KeyValueIterator>&& iter);
    static std::unique_ptr<AbstractSequentialAccess::ElementIterator> asValueIterator(std::unique_ptr<KeyValueIterator>&& iter);
private:
    virtual class AbstractNestedAssociativeAccess* asNested();
    friend void registerNestedAccess(struct QtJambiStorage* storage, class QWriteLocker &locker, AbstractContainerAccess* access);
};

class QTJAMBI_EXPORT AbstractHashAccess : public AbstractAssociativeAccess{
protected:
    ~AbstractHashAccess() override;;
    AbstractHashAccess();
    ContainerType containerType() const override;
public:
    AbstractHashAccess* clone() override = 0;
    virtual qsizetype capacity(JNIEnv *,const void*) = 0;
    virtual void reserve(void* container, qsizetype size) = 0;
    virtual void reserve(JNIEnv *, const ContainerInfo& container,qsizetype) = 0;
private:
};

class QTJAMBI_EXPORT AbstractMapAccess : public AbstractAssociativeAccess{
protected:
    ~AbstractMapAccess() override;
    AbstractMapAccess();
    ContainerType containerType() const override;
public:
    AbstractMapAccess* clone() override = 0;
    virtual jobject first(JNIEnv *,const void*) = 0;
    virtual jobject firstKey(JNIEnv *,const void*) = 0;
    virtual jobject last(JNIEnv *,const void*) = 0;
    virtual jobject lastKey(JNIEnv *,const void*) = 0;
    virtual jobject constLowerBound(JNIEnv *,const ConstExtendedContainerInfo&,jobject) = 0;
    virtual jobject constUpperBound(JNIEnv *,const ConstExtendedContainerInfo&,jobject) = 0;
    virtual jobject lowerBound(JNIEnv *,const ExtendedContainerInfo&,jobject) = 0;
    virtual jobject upperBound(JNIEnv *,const ExtendedContainerInfo&,jobject) = 0;
    virtual bool keyLessThan(JNIEnv *,jobject,jobject) = 0;
};

class QTJAMBI_EXPORT AbstractMultiMapAccess : public AbstractMapAccess{
protected:
    ~AbstractMultiMapAccess() override;;
    AbstractMultiMapAccess();;
    ContainerType containerType() const override final;
public:
    AbstractMultiMapAccess* clone() override = 0;
    using AbstractMapAccess::contains;
    using AbstractMapAccess::count;
    using AbstractMapAccess::find;
    using AbstractMapAccess::constFind;
    using AbstractMapAccess::remove;
    using AbstractMapAccess::values;
    virtual ContainerAndAccessInfo uniqueKeys(JNIEnv *, const ConstContainerInfo& container) = 0;
    virtual void unite(JNIEnv *, const ContainerInfo& container, ContainerAndAccessInfo&) = 0;
    virtual ContainerAndAccessInfo values(JNIEnv *,const ConstContainerInfo& container,jobject) = 0;
    virtual jboolean contains(JNIEnv *,const void*,jobject,jobject) = 0;
    virtual qsizetype count(JNIEnv *,const void*,jobject,jobject) = 0;
    virtual jobject find(JNIEnv *,const ExtendedContainerInfo& container, jobject,jobject) = 0;
    virtual jobject constFind(JNIEnv *,const ConstExtendedContainerInfo&,jobject,jobject) = 0;
    virtual qsizetype remove(JNIEnv *, const ContainerInfo& container, jobject,jobject) = 0;
    virtual void replace(JNIEnv *, const ContainerInfo& container, jobject,jobject) = 0;
};

class QTJAMBI_EXPORT AbstractMultiHashAccess : public AbstractHashAccess{
protected:
    ~AbstractMultiHashAccess() override;;
    AbstractMultiHashAccess();;
    ContainerType containerType() const override final;
public:
    AbstractMultiHashAccess* clone() override = 0;
    using AbstractHashAccess::contains;
    using AbstractHashAccess::count;
    using AbstractHashAccess::find;
    using AbstractHashAccess::constFind;
    using AbstractHashAccess::remove;
    using AbstractHashAccess::values;
    virtual ContainerAndAccessInfo uniqueKeys(JNIEnv *, const ConstContainerInfo& container) = 0;
    virtual void unite(JNIEnv *, const ContainerInfo& container, ContainerAndAccessInfo&) = 0;
    virtual ContainerAndAccessInfo values(JNIEnv *,const ConstContainerInfo& container,jobject) = 0;
    virtual jboolean contains(JNIEnv *,const void*,jobject,jobject) = 0;
    virtual qsizetype count(JNIEnv *,const void*,jobject,jobject) = 0;
    virtual jobject find(JNIEnv *,const ExtendedContainerInfo& container, jobject,jobject) = 0;
    virtual jobject constFind(JNIEnv *,const ConstExtendedContainerInfo&,jobject,jobject) = 0;
    virtual qsizetype remove(JNIEnv *, const ContainerInfo& container, jobject,jobject) = 0;
    virtual void replace(JNIEnv *, const ContainerInfo& container, jobject,jobject) = 0;
};

class QTJAMBI_EXPORT AbstractPairAccess : public AbstractContainerAccess{
protected:
    ~AbstractPairAccess() override;;
    AbstractPairAccess();
    ContainerType containerType() const override final;
public:
    AbstractPairAccess* clone() override = 0;
    virtual void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) = 0;
    virtual const QMetaType& firstMetaType() = 0;
    virtual const QMetaType& secondMetaType() = 0;
    virtual DataType firstType() = 0;
    virtual DataType secondType() = 0;
    virtual AbstractContainerAccess* firstNestedContainerAccess() = 0;
    virtual AbstractContainerAccess* secondNestedContainerAccess() = 0;
    virtual bool hasFirstNestedContainerAccess() = 0;
    virtual bool hasSecondNestedContainerAccess() = 0;
    virtual bool hasFirstNestedPointers() = 0;
    virtual bool hasSecondNestedPointers() = 0;
    virtual jobject first(JNIEnv *,const void*) = 0;
    virtual jobject second(JNIEnv *,const void*) = 0;
    virtual void setFirst(JNIEnv *,void*,jobject) = 0;
    virtual void setSecond(JNIEnv *,void*,jobject) = 0;
    virtual const void* first(const void*) = 0;
    virtual const void* second(const void*) = 0;
    virtual void* first(void*) = 0;
    virtual void* second(void*) = 0;
    virtual void setFirst(void*,const void*) = 0;
    virtual void setSecond(void*,const void*) = 0;
    virtual QPair<const void*,const void*> elements(const void*) = 0;
    virtual std::unique_ptr<AbstractAssociativeAccess::KeyValueIterator> keyValueIterator(const void*) = 0;
    virtual std::unique_ptr<AbstractAssociativeAccess::KeyValueIterator> keyValueIterator(void*) = 0;
    inline std::unique_ptr<AbstractAssociativeAccess::KeyValueIterator> constKeyValueIterator(const void* container){
        return keyValueIterator(container);
    }
    virtual std::unique_ptr<AbstractSequentialAccess::ElementIterator> elementIterator(const void*) = 0;
    virtual std::unique_ptr<AbstractSequentialAccess::ElementIterator> elementIterator(void*) = 0;
    inline std::unique_ptr<AbstractSequentialAccess::ElementIterator> constElementIterator(const void* container){
        return elementIterator(container);
    }
    virtual class AbstractNestedPairAccess* asNested();
    friend void registerNestedAccess(struct QtJambiStorage* storage, class QWriteLocker &locker, AbstractContainerAccess* access);
};

namespace ContainerAPI{

QTJAMBI_EXPORT QPair<void*,AbstractContainerAccess*> fromNativeId(QtJambiNativeID nativeId);

QTJAMBI_EXPORT QPair<void*,AbstractContainerAccess*> fromJavaOwner(JNIEnv *env, jobject object);

}//namespace ContainerAPI

#endif // CONTAINERAPI_H

