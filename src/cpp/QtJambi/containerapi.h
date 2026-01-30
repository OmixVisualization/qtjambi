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
#include "typeutils.h"
#include "typetests.h"

enum class QtJambiNativeID : jlong;

#define QTJAMBI_METATYPE_FROM_TYPE(T) QMetaType::fromType<typename std::remove_cv<T>::type>()
#define QTJAMBI_METATYPE_FROM_TYPE2(T) QMetaType::fromType<typename std::remove_cv<T>::type>()

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
    return getAsQSpan(env, collection, QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(pointer), access);
}
template<typename T>
bool getAsQSpan(JNIEnv *env, jobject collection, QList<T> * &pointer){
    return getAsQSpan(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(pointer));
}
template<typename T>
bool testQSpan(JNIEnv *env, jobject collection){
    return testQSpan(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T));
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
    return testQStack(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T));
}

template<typename T>
bool testQList(JNIEnv *env, jobject collection){
    return testQList(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T));
}

template<typename T>
bool testQSet(JNIEnv *env, jobject collection){
    return testQSet(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T));
}

template<typename T>
bool testQQueue(JNIEnv *env, jobject collection){
    return testQQueue(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T));
}

template<typename K, typename V>
bool testQMultiMap(JNIEnv *env, jobject mapObject){
    return testQMultiMap(env, mapObject, QtJambiPrivate::qtjambi_type<K>::id(), QTJAMBI_METATYPE_FROM_TYPE2(K), QtJambiPrivate::qtjambi_type<V>::id(), QTJAMBI_METATYPE_FROM_TYPE2(V));
}

template<typename K, typename V>
bool testQMultiHash(JNIEnv *env, jobject mapObject){
    return testQMultiHash(env, mapObject, QtJambiPrivate::qtjambi_type<K>::id(), QTJAMBI_METATYPE_FROM_TYPE2(K), QtJambiPrivate::qtjambi_type<V>::id(), QTJAMBI_METATYPE_FROM_TYPE2(V));
}

template<typename K, typename V>
bool testQMap(JNIEnv *env, jobject mapObject){
    return testQMap(env, mapObject, QtJambiPrivate::qtjambi_type<K>::id(), QTJAMBI_METATYPE_FROM_TYPE2(K), QtJambiPrivate::qtjambi_type<V>::id(), QTJAMBI_METATYPE_FROM_TYPE2(V));
}

template<typename K, typename V>
bool testQHash(JNIEnv *env, jobject mapObject){
    return testQHash(env, mapObject, QtJambiPrivate::qtjambi_type<K>::id(), QTJAMBI_METATYPE_FROM_TYPE2(K), QtJambiPrivate::qtjambi_type<V>::id(), QTJAMBI_METATYPE_FROM_TYPE2(V));
}

template<typename T>
bool getAsQStack(JNIEnv *env, jobject collection, QStack<T> * &pointer){
    return getAsQStack(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(pointer));
}

template<typename T>
bool getAsQStack(JNIEnv *env, jobject collection, QStack<T> * &pointer, AbstractContainerAccess*& access){
    return getAsQStack(env, collection, QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(pointer), access);
}

QTJAMBI_EXPORT jobject objectFromQList(JNIEnv *__jni_env,
                                       void*& listPtr,
                                       AbstractContainerAccess*& setAccess);
QTJAMBI_EXPORT jobject objectFromQList(JNIEnv *__jni_env,
                                       void*& listPtr,
                                       AbstractListAccess*& setAccess);

template<typename T>
bool getAsQList(JNIEnv *env, jobject collection, QList<T> * &pointer, AbstractContainerAccess*& access){
    return getAsQList(env, collection, QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(pointer), access);
}
template<typename T>
bool getAsQList(JNIEnv *env, jobject collection, QList<T> * &pointer){
    return getAsQList(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(pointer));
}

template<typename T>
bool getAsQSet(JNIEnv *env, jobject collection, QSet<T> * &pointer, AbstractContainerAccess*& access){
    return getAsQSet(env, collection, QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(pointer), access);
}

template<typename T>
bool getAsQSet(JNIEnv *env, jobject collection, QSet<T> * &pointer){
    return getAsQSet(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(pointer));
}

template<typename T>
bool getAsQQueue(JNIEnv *env, jobject collection, QQueue<T> * &pointer, AbstractContainerAccess*& access){
    return getAsQQueue(env, collection, QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(pointer), access);
}

template<typename T>
bool getAsQQueue(JNIEnv *env, jobject collection, QQueue<T> * &pointer){
    return getAsQQueue(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(pointer));
}

template<typename K, typename V>
bool getAsQMultiMap(JNIEnv *env, jobject mapObject, QMultiMap<K,V> * &pointer, AbstractContainerAccess*& access){
    return getAsQMultiMap(env, mapObject, QTJAMBI_METATYPE_FROM_TYPE2(K), QTJAMBI_METATYPE_FROM_TYPE2(V), reinterpret_cast<void*&>(pointer), access);
}

template<typename K, typename V>
bool getAsQMultiMap(JNIEnv *env, jobject mapObject, QMultiMap<K,V> * &pointer){
    return getAsQMultiMap(env, mapObject, QtJambiPrivate::qtjambi_type<K>::id(), QTJAMBI_METATYPE_FROM_TYPE2(K), QtJambiPrivate::qtjambi_type<V>::id(), QTJAMBI_METATYPE_FROM_TYPE2(V), reinterpret_cast<void*&>(pointer));
}

template<typename K, typename V>
bool getAsQMultiHash(JNIEnv *env, jobject mapObject, QMultiHash<K,V> * &pointer, AbstractContainerAccess*& access){
    return getAsQMultiHash(env, mapObject, QTJAMBI_METATYPE_FROM_TYPE2(K), QTJAMBI_METATYPE_FROM_TYPE2(V), reinterpret_cast<void*&>(pointer), access);
}

template<typename K, typename V>
bool getAsQMultiHash(JNIEnv *env, jobject mapObject, QMultiHash<K,V> * &pointer){
    return getAsQMultiHash(env, mapObject, QtJambiPrivate::qtjambi_type<K>::id(), QTJAMBI_METATYPE_FROM_TYPE2(K), QtJambiPrivate::qtjambi_type<V>::id(), QTJAMBI_METATYPE_FROM_TYPE2(V), reinterpret_cast<void*&>(pointer));
}

template<typename K, typename V>
bool getAsQMap(JNIEnv *env, jobject mapObject, QMap<K,V> * &pointer, AbstractContainerAccess*& access){
    return getAsQMap(env, mapObject, QTJAMBI_METATYPE_FROM_TYPE2(K), QTJAMBI_METATYPE_FROM_TYPE2(V), reinterpret_cast<void*&>(pointer), access);
}

template<typename K, typename V>
bool getAsQMap(JNIEnv *env, jobject mapObject, QMap<K,V> * &pointer){
    return getAsQMap(env, mapObject, QtJambiPrivate::qtjambi_type<K>::id(), QTJAMBI_METATYPE_FROM_TYPE2(K), QtJambiPrivate::qtjambi_type<V>::id(), QTJAMBI_METATYPE_FROM_TYPE2(V), reinterpret_cast<void*&>(pointer));
}

template<typename K, typename V>
bool getAsQHash(JNIEnv *env, jobject mapObject, QHash<K,V> * &pointer, AbstractContainerAccess*& access){
    return getAsQHash(env, mapObject, QTJAMBI_METATYPE_FROM_TYPE2(K), QTJAMBI_METATYPE_FROM_TYPE2(V), reinterpret_cast<void*&>(pointer), access);
}

template<typename K, typename V>
bool getAsQHash(JNIEnv *env, jobject mapObject, QHash<K,V> * &pointer){
    return getAsQHash(env, mapObject, QtJambiPrivate::qtjambi_type<K>::id(), QTJAMBI_METATYPE_FROM_TYPE2(K), QtJambiPrivate::qtjambi_type<V>::id(), QTJAMBI_METATYPE_FROM_TYPE2(V), reinterpret_cast<void*&>(pointer));
}

} // namespace ContainerAPI

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
    virtual QMetaType registerContainer(const QByteArray& containerTypeName) = 0;
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
};

class QTJAMBI_EXPORT AbstractReferenceCountingContainer{
protected:
    AbstractReferenceCountingContainer() = default;
    virtual ~AbstractReferenceCountingContainer();
public:
    virtual void updateRC(JNIEnv * env, const ContainerInfo& container) = 0;
    void swapRC(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2);
    jobject findContainer(JNIEnv * env, jobject container);
    static void unfoldAndAddContainer(JNIEnv * env, jobject set, const void* data, AbstractContainerAccess::DataType dataType, const QMetaType& metaType, AbstractContainerAccess* access);
    static void unfoldAndAddContainer(JNIEnv * env, jobject set, jobject value);
};

class QTJAMBI_EXPORT ReferenceCountingSetContainer : public AbstractReferenceCountingContainer{
public:
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
};

class QTJAMBI_EXPORT ReferenceCountingMapContainer : public AbstractReferenceCountingContainer{
public:
    ReferenceCountingMapContainer() = default;
    jobject rcContainer(JNIEnv * env, jobject container);
    void clearRC(JNIEnv * env, jobject container);
    void assignRC(JNIEnv * env, jobject container, jobject container2);
    void putAllRC(JNIEnv * env, jobject container, jobject container2);
    void putRC(JNIEnv * env, jobject container, jobject key, jobject value);
    void removeRC(JNIEnv * env, jobject container, jobject key, int n = 1);
};

class QTJAMBI_EXPORT ReferenceCountingMultiMapContainer : public AbstractReferenceCountingContainer{
public:
    ReferenceCountingMultiMapContainer() = default;
    jobject rcContainer(JNIEnv * env, jobject container);
    void clearRC(JNIEnv * env, jobject container);
    void assignRC(JNIEnv * env, jobject container, jobject container2);
    void putAllRC(JNIEnv * env, jobject container, jobject container2);
    void putRC(JNIEnv * env, jobject container, jobject key, jobject value);
    void removeRC(JNIEnv * env, jobject container, jobject key, int n = 1);
    void removeRC(JNIEnv * env, jobject container, jobject key, jobject value, int n = 1);
    static jobject newRCMultiMap(JNIEnv * env);
};

class QTJAMBI_EXPORT AbstractSequentialConstIteratorAccess : public AbstractContainerAccess{
protected:
    ~AbstractSequentialConstIteratorAccess() override;
    AbstractSequentialConstIteratorAccess();
public:
    AbstractSequentialConstIteratorAccess* clone() override = 0;
    virtual jobject value(JNIEnv * env, const void* iterator) = 0;
    virtual void increment(JNIEnv * env, void* iterator) = 0;
    virtual void decrement(JNIEnv * env, void* iterator) = 0;
    virtual jboolean lessThan(JNIEnv * env, const void* iterator, const void* other) = 0;
    virtual bool canLess() = 0;
    virtual jboolean equals(JNIEnv * env, const void* iterator, const void* other) = 0;
    virtual const QMetaType& valueMetaType() = 0;
private:
    size_t sizeOf() const final override;
    size_t alignOf() const final override;
    void* constructContainer(void* placement) final override;
    void* constructContainer(void* placement, const void* copyOf) final override;
    void* constructContainer(JNIEnv *, void* result, const ConstContainerAndAccessInfo& container) final override;
    void* constructContainer(void* placement, void* move) final override;
    void* constructContainer(JNIEnv *, void* result, const ContainerAndAccessInfo& move) final override;
    bool destructContainer(void* container) final override;
    void assign(void*, const void* ) final override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) final override;
    QMetaType registerContainer(const QByteArray&) final override;
    Q_DISABLE_COPY_MOVE(AbstractSequentialConstIteratorAccess)
};

class QTJAMBI_EXPORT AbstractSequentialIteratorAccess : public AbstractSequentialConstIteratorAccess{
protected:
    ~AbstractSequentialIteratorAccess() override;
    AbstractSequentialIteratorAccess();
public:
    virtual void setValue(JNIEnv * env, void* iterator, jobject newValue) = 0;
private:
    Q_DISABLE_COPY_MOVE(AbstractSequentialIteratorAccess)
};

class QTJAMBI_EXPORT AbstractAssociativeConstIteratorAccess : public AbstractSequentialConstIteratorAccess{
protected:
    ~AbstractAssociativeConstIteratorAccess() override;
    AbstractAssociativeConstIteratorAccess();
public:
    virtual jobject key(JNIEnv * env, const void* iterator) = 0;
    virtual const QMetaType& keyMetaType() = 0;
private:
    Q_DISABLE_COPY_MOVE(AbstractAssociativeConstIteratorAccess)
};

class QTJAMBI_EXPORT AbstractAssociativeIteratorAccess : public AbstractAssociativeConstIteratorAccess{
protected:
    ~AbstractAssociativeIteratorAccess() override;
    AbstractAssociativeIteratorAccess();
public:
    virtual void setValue(JNIEnv * env, void* iterator, jobject newValue) = 0;
private:
    Q_DISABLE_COPY_MOVE(AbstractAssociativeIteratorAccess)
};

typedef bool (*ElementAnalyzer)(const void* element, void* data);
typedef bool (*EntryAnalyzer)(const void* key, const void* value, void* data);

class QTJAMBI_EXPORT AbstractSequentialAccess : public AbstractContainerAccess{
protected:
    ~AbstractSequentialAccess() override;
    AbstractSequentialAccess();
public:
    AbstractSequentialAccess* clone() override = 0;
    virtual bool isDetached(const void* container) = 0;
    virtual void detach(const ContainerInfo& container) = 0;
    virtual bool isSharedWith(const void* container, const void* container2) = 0;
    virtual void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) = 0;
    virtual jint size(JNIEnv * env, const void* container) = 0;
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
    };
    virtual std::unique_ptr<ElementIterator> elementIterator(const void*) = 0;
    virtual std::unique_ptr<ElementIterator> elementIterator(void*) = 0;
    inline std::unique_ptr<ElementIterator> constElementIterator(const void* container){
        return elementIterator(container);
    }
    Q_DISABLE_COPY_MOVE(AbstractSequentialAccess)
};

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
class QTJAMBI_EXPORT AbstractSpanAccess : public AbstractSequentialAccess{
protected:
    ~AbstractSpanAccess() override;
    AbstractSpanAccess();
public:
    AbstractSpanAccess* clone() override = 0;
    virtual bool isConst() = 0;
    virtual jint size_bytes(JNIEnv * env, const void* container) = 0;
    virtual jobject get(JNIEnv *,const void*,jint) = 0;
    virtual bool set(JNIEnv *,const ContainerInfo&,jint,jobject) = 0;
    virtual const void* get(const void*,qsizetype) = 0;
    virtual bool set(void*,qsizetype,const void*) = 0;
    virtual jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) = 0;
    virtual jobject end(JNIEnv * env, const ExtendedContainerInfo& container) = 0;
    Q_DISABLE_COPY_MOVE(AbstractSpanAccess)
private:
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
public:
    AbstractListAccess* clone() override = 0;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    virtual AbstractSpanAccess* createSpanAccess(bool isConst) = 0;
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    virtual void appendList(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) = 0;
    virtual jobject at(JNIEnv * env, const void* container, jint index) = 0;
    virtual const void* at(const void* container, qsizetype index) = 0;
    virtual void* at(void* container, qsizetype index) = 0;
    virtual jobject value(JNIEnv * env, const void* container, jint index) = 0;
    virtual jobject value(JNIEnv * env, const void* container, jint index, jobject defaultValue) = 0;
    virtual void swapItemsAt(JNIEnv * env, const ContainerInfo& container, jint index1, jint index2) = 0;
    virtual jboolean startsWith(JNIEnv * env, const void* container, jobject value) = 0;
    virtual void reserve(JNIEnv * env, const ContainerInfo& container, jint size) = 0;
    virtual void replace(JNIEnv * env, const ContainerInfo& container, jint index, jobject value) = 0;
    virtual void replace(void* container, qsizetype index, const void* value) = 0;
    virtual void remove(JNIEnv * env, const ContainerInfo& container, jint index, jint n) = 0;
    virtual void remove(void* container, qsizetype pos, qsizetype n) = 0;
    virtual jint removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) = 0;
    virtual jboolean equal(JNIEnv * env, const void* container, jobject other) = 0;
    virtual void move(JNIEnv * env, const ContainerInfo& container, jint index1, jint index2) = 0;
    virtual ContainerAndAccessInfo mid(JNIEnv * env, const ConstContainerAndAccessInfo& container, jint index1, jint index2) = 0;
    virtual jint lastIndexOf(JNIEnv * env, const void* container, jobject value, jint index) = 0;
    virtual void insert(JNIEnv * env, const ContainerInfo& container, jint index, jint n, jobject value) = 0;
    virtual void insert(void* container, qsizetype index, qsizetype n, const void* entry) = 0;
    virtual bool append(void* container, const void* entry) = 0;
    virtual jint indexOf(JNIEnv * env, const void* container, jobject value, jint index) = 0;
    virtual jboolean endsWith(JNIEnv * env, const void* container, jobject value) = 0;
    virtual jint count(JNIEnv * env, const void* container, jobject value) = 0;
    virtual jboolean contains(JNIEnv * env, const void* container, jobject value) = 0;
    virtual jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) = 0;
    virtual jobject end(JNIEnv * env, const ExtendedContainerInfo& container) = 0;
    virtual void resize(void* container, qsizetype newSize) = 0;
    virtual jint capacity(JNIEnv * env, const void* container) = 0;
    virtual void fill(JNIEnv * env, const ContainerInfo& container, jobject value, jint size) = 0;
    virtual void resize(JNIEnv * env, const ContainerInfo& container, jint newSize) = 0;
    virtual void squeeze(JNIEnv * env, const ContainerInfo& container) = 0;
    Q_DISABLE_COPY_MOVE(AbstractListAccess)
};

class QTJAMBI_EXPORT AbstractSetAccess : public AbstractSequentialAccess{
protected:
    ~AbstractSetAccess() override;
    AbstractSetAccess();
public:
    AbstractSetAccess* clone() override = 0;
    virtual jint capacity(JNIEnv * env, const void* container) = 0;
    virtual jboolean contains(JNIEnv * env, const void* container, jobject value) = 0;
    virtual void insert(JNIEnv * env, const ContainerInfo& container, jobject value) = 0;
    virtual void intersect(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) = 0;
    virtual jboolean intersects(JNIEnv * env, const void* container, jobject other) = 0;
    virtual jboolean equal(JNIEnv * env, const void* container, jobject other) = 0;
    virtual jboolean remove(JNIEnv * env, const ContainerInfo& container, jobject value) = 0;
    virtual void reserve(JNIEnv * env, const ContainerInfo& container, jint newSize) = 0;
    virtual void subtract(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) = 0;
    virtual void unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) = 0;
    virtual ContainerAndAccessInfo values(JNIEnv * env, const ConstContainerInfo& container) = 0;
    Q_DISABLE_COPY_MOVE(AbstractSetAccess)
};

class QTJAMBI_EXPORT AbstractAssociativeAccess : public AbstractContainerAccess{
protected:
    ~AbstractAssociativeAccess() override;
    AbstractAssociativeAccess();
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
    virtual jint size(JNIEnv *,const void*) = 0;
    virtual qsizetype size(const void* container) = 0;
    virtual jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) = 0;
    virtual jobject end(JNIEnv * env, const ExtendedContainerInfo& container) = 0;
    virtual jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) = 0;
    virtual jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) = 0;
    virtual jboolean contains(JNIEnv *,const void*,jobject) = 0;
    virtual bool contains(const void*,const void*) = 0;
    virtual jint count(JNIEnv *,const void*,jobject) = 0;
    virtual jobject find(JNIEnv *,const ExtendedContainerInfo& container, jobject) = 0;
    virtual jobject constFind(JNIEnv *,const ConstExtendedContainerInfo&,jobject) = 0;
    virtual void insert(JNIEnv *, const ContainerInfo& container,jobject,jobject) = 0;
    virtual void insert(void* container,const void* key, const void* value) = 0;
    virtual jobject key(JNIEnv *,const void*,jobject,jobject) = 0;
    virtual ContainerAndAccessInfo keys(JNIEnv *, const ConstContainerInfo& container) = 0;
    virtual ContainerAndAccessInfo keys(JNIEnv *,const ConstContainerInfo& container,jobject) = 0;
    virtual jboolean equal(JNIEnv *,const void*,jobject) = 0;
    virtual jint remove(JNIEnv *, const ContainerInfo& container,jobject) = 0;
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
    };
    virtual std::unique_ptr<KeyValueIterator> keyValueIterator(const void*) = 0;
    virtual std::unique_ptr<KeyValueIterator> keyValueIterator(void*) = 0;
    inline std::unique_ptr<KeyValueIterator> constKeyValueIterator(const void* container){
        return keyValueIterator(container);
    }
    static std::unique_ptr<AbstractSequentialAccess::ElementIterator> asKeyIterator(std::unique_ptr<KeyValueIterator>&& iter);
    static std::unique_ptr<AbstractSequentialAccess::ElementIterator> asValueIterator(std::unique_ptr<KeyValueIterator>&& iter);
};

class QTJAMBI_EXPORT AbstractHashAccess : public AbstractAssociativeAccess{
protected:
    ~AbstractHashAccess() override;;
    AbstractHashAccess();;
public:
    AbstractHashAccess* clone() override = 0;
    virtual jint capacity(JNIEnv *,const void*) = 0;
    virtual void reserve(JNIEnv *, const ContainerInfo& container,jint) = 0;
};

class QTJAMBI_EXPORT AbstractMapAccess : public AbstractAssociativeAccess{
protected:
    ~AbstractMapAccess() override;
    AbstractMapAccess();
public:
    AbstractMapAccess* clone() override = 0;
    virtual jobject first(JNIEnv *,const void*) = 0;
    virtual jobject firstKey(JNIEnv *,const void*) = 0;
    virtual jobject last(JNIEnv *,const void*) = 0;
    virtual jobject lastKey(JNIEnv *,const void*) = 0;
    virtual jobject constLowerBound(JNIEnv *,const ConstExtendedContainerInfo&,jobject) = 0;
    virtual jobject constUpperBound(JNIEnv *,const ConstExtendedContainerInfo&,jobject) = 0;
    virtual bool keyLessThan(JNIEnv *,jobject,jobject) = 0;
};

class QTJAMBI_EXPORT AbstractMultiMapAccess : public AbstractMapAccess{
protected:
    ~AbstractMultiMapAccess() override;;
    AbstractMultiMapAccess();;
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
    virtual jint count(JNIEnv *,const void*,jobject,jobject) = 0;
    virtual jobject find(JNIEnv *,const ExtendedContainerInfo& container, jobject,jobject) = 0;
    virtual jobject constFind(JNIEnv *,const ConstExtendedContainerInfo&,jobject,jobject) = 0;
    virtual jint remove(JNIEnv *, const ContainerInfo& container, jobject,jobject) = 0;
    virtual void replace(JNIEnv *, const ContainerInfo& container, jobject,jobject) = 0;
};

class QTJAMBI_EXPORT AbstractMultiHashAccess : public AbstractHashAccess{
protected:
    ~AbstractMultiHashAccess() override;;
    AbstractMultiHashAccess();;
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
    virtual jint count(JNIEnv *,const void*,jobject,jobject) = 0;
    virtual jobject find(JNIEnv *,const ExtendedContainerInfo& container, jobject,jobject) = 0;
    virtual jobject constFind(JNIEnv *,const ConstExtendedContainerInfo&,jobject,jobject) = 0;
    virtual jint remove(JNIEnv *, const ContainerInfo& container, jobject,jobject) = 0;
    virtual void replace(JNIEnv *, const ContainerInfo& container, jobject,jobject) = 0;
};

class QTJAMBI_EXPORT AbstractPairAccess : public AbstractContainerAccess{
protected:
    ~AbstractPairAccess() override;;
    AbstractPairAccess();
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
};

namespace ContainerAPI{

QTJAMBI_EXPORT QPair<void*,AbstractContainerAccess*> fromNativeId(QtJambiNativeID nativeId);

QTJAMBI_EXPORT QPair<void*,AbstractContainerAccess*> fromJavaOwner(JNIEnv *env, jobject object);

}//namespace ContainerAPI


#if defined(QTJAMBI_GENERIC_ACCESS)
namespace QtJambiPrivate{

typedef size_t(*HashWrapper)(const void* ptr, size_t seed, QtJambiUtils::QHashFunction hashFunction);
typedef size_t(*HashWrapper2)(const void* ptr, size_t seed, QtJambiUtils::QHashFunction hashFunction1, QtJambiUtils::QHashFunction hashFunction2);

template<typename Container>
size_t sequentialContainerHash(const void* ptr, size_t seed, QtJambiUtils::QHashFunction hashFunction)
{
    const Container& container = *reinterpret_cast<const Container*>(ptr);
    size_t prime = 31;
    size_t result = seed;
    result = prime * result + qHash(container.size(), result);
    for(typename Container::const_iterator iter = container.cbegin();
        iter != container.cend(); ++iter){
        result = prime * result + hashFunction(& (*iter), result);
    }
    return result;
}

template<typename Container>
size_t associativeContainerHash(const void* ptr, size_t seed, QtJambiUtils::QHashFunction hashFunction1, QtJambiUtils::QHashFunction hashFunction2)
{
    const Container& container = *reinterpret_cast<const Container*>(ptr);
    size_t prime = 31;
    size_t result = seed;
    result = prime * result + qHash(container.size(), result);
    for(typename Container::const_iterator iter = container.begin();
         iter != container.end(); ++iter){
        result = prime * result + hashFunction1(&iter.key(), result);
        result = prime * result + hashFunction2(&iter.value(), result);
    }
    return result;
}

template<typename Container>
size_t pairHashWrapper(const void* ptr, size_t seed, QtJambiUtils::QHashFunction hashFunction1, QtJambiUtils::QHashFunction hashFunction2){
    const Container& container = *reinterpret_cast<const Container*>(ptr);
    size_t prime = 31;
    size_t result = seed;
    result = prime * result + hashFunction1(&container.first, result);
    result = prime * result + hashFunction2(&container.second, result);
    return result;
}

typedef bool(*MetaSequenceIteratorFactory)(void *src, void *target, const QtMetaContainerPrivate::QMetaSequenceInterface* iface);
typedef bool(*MetaSequenceConstIteratorFactory)(const void *src, void *target, const QtMetaContainerPrivate::QMetaSequenceInterface* iface);
typedef bool(*MetaAssociationIteratorFactory)(void *src, void *target, const QtMetaContainerPrivate::QMetaAssociationInterface* iface);
typedef bool(*MetaAssociationConstIteratorFactory)(const void *src, void *target, const QtMetaContainerPrivate::QMetaAssociationInterface* iface);
typedef bool(*PairAccessFactory)(const void *src, void *target, const QMetaType& metaType1, const QMetaType& metaType2);

QTJAMBI_EXPORT QMetaType registerSequentialContainerType(const QByteArray& typeName,
                                                   size_t containerSize, size_t containerAlign,
                                                   bool isPointer, const QMetaType& metaType,
                                                   QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr,
                                                   QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr,
                                                   QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr,
                                                   QtPrivate::QMetaTypeInterface::DtorFn dtor,
                                                   QtPrivate::QMetaTypeInterface::EqualsFn equals,
                                                   QtPrivate::QMetaTypeInterface::LessThanFn lessThan,
                                                   QtPrivate::QMetaTypeInterface::DebugStreamFn debugStream,
                                                   QtPrivate::QMetaTypeInterface::DataStreamOutFn dataStreamOutFn,
                                                   QtPrivate::QMetaTypeInterface::DataStreamInFn dataStreamInFn,
                                                   const QtMetaContainerPrivate::QMetaSequenceInterface *defaultInterface,
                                                   MetaSequenceIteratorFactory createMetaSequenceIterator,
                                                   MetaSequenceConstIteratorFactory createMetaSequenceConstIterator,
                                                   HashWrapper hashWrapper,
                                                   AbstractSequentialAccess* access);

QTJAMBI_EXPORT QMetaType registerAssociativeContainerType(const QByteArray& typeName,
                                                    size_t containerSize, size_t containerAlign,
                                                    bool isPointer1, const QMetaType& metaType1,
                                                    bool isPointer2, const QMetaType& metaType2,
                                                    QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr,
                                                    QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr,
                                                    QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr,
                                                    QtPrivate::QMetaTypeInterface::DtorFn dtor,
                                                    QtPrivate::QMetaTypeInterface::EqualsFn equals,
                                                    QtPrivate::QMetaTypeInterface::LessThanFn lessThan,
                                                    QtPrivate::QMetaTypeInterface::DebugStreamFn debugStream,
                                                    QtPrivate::QMetaTypeInterface::DataStreamOutFn dataStreamOutFn,
                                                    QtPrivate::QMetaTypeInterface::DataStreamInFn dataStreamInFn,
                                                    const QtMetaContainerPrivate::QMetaAssociationInterface *defaultInterface,
                                                    MetaAssociationIteratorFactory createMetaAssociationIterator,
                                                    MetaAssociationConstIteratorFactory createMetaAssociationConstIterator,
                                                    PairAccessFactory pairAccessFactory,
                                                    HashWrapper2 hashWrapper,
                                                    AbstractContainerAccess* access);

template<typename Container, size_t size>
QMetaType registerSequentialContainerType(const QByteArray& typeName, const QMetaType& metaType, AbstractSequentialAccess* access){
    QMetaType newMetaType = QMetaType::fromName(typeName);
    if(!newMetaType.isValid()){
        static const QtMetaContainerPrivate::QMetaSequenceInterface defaultInterface = QtMetaContainerPrivate::QMetaSequenceInterface(QtMetaContainerPrivate::QMetaSequenceForContainer<Container>());
        newMetaType = registerSequentialContainerType(typeName, sizeof(Container), Q_ALIGNOF(Container), size==0, metaType,
                                                QMetaTypeInterfaceFunctions<Container>::defaultCtr,
                                                QMetaTypeInterfaceFunctions<Container>::copyCtr,
                                                QMetaTypeInterfaceFunctions<Container>::moveCtr,
                                                QMetaTypeInterfaceFunctions<Container>::dtor,
                                                QtPrivate::QEqualityOperatorForType<Container>::equals,
                                                QtPrivate::QLessThanOperatorForType<Container>::lessThan,
                                                QtPrivate::QDebugStreamOperatorForType<Container>::debugStream,
                                                QtPrivate::QDataStreamOperatorForType<Container>::dataStreamOut,
                                                QtPrivate::QDataStreamOperatorForType<Container>::dataStreamIn,
                                                &defaultInterface,
                                                [](void *src, void *target, const QtMetaContainerPrivate::QMetaSequenceInterface* iface) ->bool {
                                                    *reinterpret_cast<QIterable<QMetaSequence>*>(target) = QIterable<QMetaSequence>(QMetaSequence(iface), reinterpret_cast<Container*>(src));
                                                    return true;
                                                },
                                                [](const void *src, void *target, const QtMetaContainerPrivate::QMetaSequenceInterface* iface) ->bool {
                                                    *reinterpret_cast<QIterable<QMetaSequence>*>(target) = QIterable<QMetaSequence>(QMetaSequence(iface), reinterpret_cast<const Container*>(src));
                                                    return true;
                                                },
                                                sequentialContainerHash<Container>,
                                                access
                                            );
    }
    return newMetaType;
}

template<typename Container, size_t size1, size_t size2>
QMetaType registerAssociativeContainerType(const QByteArray& typeName, const QMetaType& metaType1, const QMetaType& metaType2, AbstractAssociativeAccess* access){
    QMetaType newMetaType = QMetaType::fromName(typeName);
    if(!newMetaType.isValid()){
        static const QtMetaContainerPrivate::QMetaAssociationInterface defaultInterface = QtMetaContainerPrivate::QMetaAssociationInterface(QtMetaContainerPrivate::QMetaAssociationForContainer<Container>());
        newMetaType = registerAssociativeContainerType(typeName, sizeof(Container), Q_ALIGNOF(Container), size1==0, metaType1, size2==0, metaType2,
                                                        QMetaTypeInterfaceFunctions<Container>::defaultCtr,
                                                        QMetaTypeInterfaceFunctions<Container>::copyCtr,
                                                        QMetaTypeInterfaceFunctions<Container>::moveCtr,
                                                        QMetaTypeInterfaceFunctions<Container>::dtor,
                                                        QtPrivate::QEqualityOperatorForType<Container>::equals,
                                                        QtPrivate::QLessThanOperatorForType<Container>::lessThan,
                                                        QtPrivate::QDebugStreamOperatorForType<Container>::debugStream,
                                                        QtPrivate::QDataStreamOperatorForType<Container>::dataStreamOut,
                                                        QtPrivate::QDataStreamOperatorForType<Container>::dataStreamIn,
                                                        &defaultInterface,
                                                        [](void *src, void *target, const QtMetaContainerPrivate::QMetaAssociationInterface* iface) ->bool {
                                                            *reinterpret_cast<QIterable<QMetaAssociation>*>(target) = QIterable<QMetaAssociation>(QMetaAssociation(iface), reinterpret_cast<Container*>(src));
                                                            return true;
                                                        },
                                                        [](const void *src, void *target, const QtMetaContainerPrivate::QMetaAssociationInterface* iface) ->bool {
                                                            *reinterpret_cast<QIterable<QMetaAssociation>*>(target) = QIterable<QMetaAssociation>(QMetaAssociation(iface), reinterpret_cast<const Container*>(src));
                                                            return true;
                                                        },
                                                        nullptr,
                                                        associativeContainerHash<Container>,
                                                        access
        );
    }
    return newMetaType;
}

template<typename Container, size_t size1, size_t size2>
static QMetaType registerQPairType(const QByteArray& typeName, const QMetaType& metaType1, const QMetaType& metaType2, AbstractPairAccess* access){
    QMetaType newMetaType = QMetaType::fromName(typeName);
    if(!newMetaType.isValid()){
        newMetaType = registerAssociativeContainerType(typeName, sizeof(Container), Q_ALIGNOF(Container), size1==0, metaType1, size2==0, metaType2,
                                                        QMetaTypeInterfaceFunctions<Container>::defaultCtr,
                                                        QMetaTypeInterfaceFunctions<Container>::copyCtr,
                                                        QMetaTypeInterfaceFunctions<Container>::moveCtr,
                                                        QMetaTypeInterfaceFunctions<Container>::dtor,
                                                        QtPrivate::QEqualityOperatorForType<Container>::equals,
                                                        QtPrivate::QLessThanOperatorForType<Container>::lessThan,
                                                        QtPrivate::QDebugStreamOperatorForType<Container>::debugStream,
                                                        QtPrivate::QDataStreamOperatorForType<Container>::dataStreamOut,
                                                        QtPrivate::QDataStreamOperatorForType<Container>::dataStreamIn,
                                                        nullptr,
                                                        nullptr,
                                                        nullptr,
                                                        [](const void *src, void *target, const QMetaType& metaType1, const QMetaType& metaType2) ->bool {
                                                              QtMetaTypePrivate::QPairVariantInterfaceImpl* result = reinterpret_cast<QtMetaTypePrivate::QPairVariantInterfaceImpl*>(target);
                                                              *result = QtMetaTypePrivate::QPairVariantInterfaceImpl(reinterpret_cast<const Container*>(src));
                                                              result->_metaType_first = metaType1;
                                                              result->_metaType_second = metaType2;
                                                              return true;
                                                        },
                                                        pairHashWrapper<Container>,
                                                        access
                                                    );
    }
    return newMetaType;
}

}//namespace QtJambiPrivate

#endif //defined(QTJAMBI_GENERIC_ACCESS)

#endif // CONTAINERAPI_H

