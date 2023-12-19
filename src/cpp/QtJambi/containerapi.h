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


#ifndef CONTAINERAPI_H
#define CONTAINERAPI_H

#include <functional>
#include <typeinfo>

#include "global.h"
#include "registryapi.h"
#include "utils.h"
#include "typetests.h"

enum class QtJambiNativeID : jlong;

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0) && defined(QLINKEDLIST_H)
QT_WARNING_DISABLE_GCC("-Wdeprecated-declarations")
QT_WARNING_DISABLE_DEPRECATED
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#define QTJAMBI_METATYPE_FROM_TYPE(T) QMetaType::fromType<typename std::remove_cv<T>::type>()
#define QTJAMBI_METATYPE_FROM_TYPE2(T) QMetaType::fromType<typename std::remove_cv<T>::type>()
#else
namespace QtJambiPrivate{

template <typename T, bool = QMetaTypeId2<T>::Defined>
struct MetaTypeRegistrationHelper{
    static int registerMetaType(){
        return QMetaTypeId2<T>::qt_metatype_id();
    }
};

template <typename T>
struct MetaTypeRegistrationHelper<T,false>{
    static int registerMetaType(){
        int id = QtPrivate::QMetaTypeIdHelper<T>::qt_metatype_id();
        if (id > QMetaType::UnknownType)
            return id;

        QByteArray normalizedTypeName = QMetaObject::normalizedType(QtJambiAPI::typeName(typeid(T)));
        id = QMetaType::type(normalizedTypeName);
        if (id > QMetaType::UnknownType)
            return id;
        QMetaType::TypeFlags flags(QtPrivate::QMetaTypeTypeFlags<T>::Flags);

        if (QtPrivate::MetaTypeDefinedHelper<T, QMetaTypeId2<T>::Defined && !QMetaTypeId2<T>::IsBuiltIn>::Defined)
            flags |= QMetaType::WasDeclaredAsMetaType;

        id = QMetaType::registerNormalizedType(normalizedTypeName,
                                       QtMetaTypePrivate::QMetaTypeFunctionHelper<T, std::is_arithmetic<T>::value || std::is_enum<T>::value || std::is_pointer<T>::value || supports_Destructor<T>::value>::Destruct,
                                       QtMetaTypePrivate::QMetaTypeFunctionHelper<T, std::is_arithmetic<T>::value || std::is_enum<T>::value || std::is_pointer<T>::value || (supports_StandardConstructor<T>::value && supports_CopyConstructor<T>::value)>::Construct,
                                       int(sizeof(T)),
                                       flags,
                                       QtPrivate::MetaObjectForType<T>::value());

        if (id > QMetaType::UnknownType) {
            QtPrivate::SequentialContainerConverterHelper<T>::registerConverter(id);
            QtPrivate::AssociativeContainerConverterHelper<T>::registerConverter(id);
            QtPrivate::MetaTypePairHelper<T>::registerConverter(id);
            QtPrivate::MetaTypeSmartPointerHelper<T>::registerConverter(id);
        }
        return id;
    }
};

template <typename T>
int registerMetaType(){
    return MetaTypeRegistrationHelper<typename std::remove_cv<T>::type>::registerMetaType();
}

}

#define QTJAMBI_METATYPE_FROM_TYPE(T) QtJambiPrivate::registerMetaType<T>()
#define QTJAMBI_METATYPE_FROM_TYPE2(T) QMetaType(QTJAMBI_METATYPE_FROM_TYPE(T))
#endif

typedef void(* SmartPointerDeleter)(void *, bool);
typedef void*(*SmartPointerGetter)(const void *);

namespace ContainerAPI{

QTJAMBI_EXPORT bool testQStack(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QTJAMBI_EXPORT bool testQVector(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType);
QTJAMBI_EXPORT bool testQLinkedList(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType);
#endif
QTJAMBI_EXPORT bool testQList(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType);
QTJAMBI_EXPORT bool testQSet(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType);
QTJAMBI_EXPORT bool testQQueue(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType);
QTJAMBI_EXPORT bool testQMultiMap(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType);
QTJAMBI_EXPORT bool testQMultiHash(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType);
QTJAMBI_EXPORT bool testQMap(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType);
QTJAMBI_EXPORT bool testQHash(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType);

QTJAMBI_EXPORT bool testQStack(JNIEnv *env, jobject collection, const std::type_info& expectedElementType, const QMetaType& expectedElementMetaType);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QTJAMBI_EXPORT bool testQVector(JNIEnv *env, jobject collection, const std::type_info& expectedElementType, const QMetaType& expectedElementMetaType);
QTJAMBI_EXPORT bool testQLinkedList(JNIEnv *env, jobject collection, const std::type_info& expectedElementType, const QMetaType& expectedElementMetaType);
#endif
QTJAMBI_EXPORT bool testQList(JNIEnv *env, jobject collection, const std::type_info& expectedElementType, const QMetaType& expectedElementMetaType);
QTJAMBI_EXPORT bool testQSet(JNIEnv *env, jobject collection, const std::type_info& expectedElementType, const QMetaType& expectedElementMetaType);
QTJAMBI_EXPORT bool testQQueue(JNIEnv *env, jobject collection, const std::type_info& expectedElementType, const QMetaType& expectedElementMetaType);
QTJAMBI_EXPORT bool testQMultiMap(JNIEnv *env, jobject mapObject, const std::type_info& expectedKeyType, const QMetaType& expectedKeyMetaType, const std::type_info& expectedValueType, const QMetaType& expectedValueMetaType);
QTJAMBI_EXPORT bool testQMultiHash(JNIEnv *env, jobject mapObject, const std::type_info& expectedKeyType, const QMetaType& expectedKeyMetaType, const std::type_info& expectedValueType, const QMetaType& expectedValueMetaType);
QTJAMBI_EXPORT bool testQMap(JNIEnv *env, jobject mapObject, const std::type_info& expectedKeyType, const QMetaType& expectedKeyMetaType, const std::type_info& expectedValueType, const QMetaType& expectedValueMetaType);
QTJAMBI_EXPORT bool testQHash(JNIEnv *env, jobject mapObject, const std::type_info& expectedKeyType, const QMetaType& expectedKeyMetaType, const std::type_info& expectedValueType, const QMetaType& expectedValueMetaType);

QTJAMBI_EXPORT bool getAsQStack(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType, void* &pointer);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QTJAMBI_EXPORT bool getAsQVector(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQLinkedList(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType, void* &pointer);
#endif
QTJAMBI_EXPORT bool getAsQList(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQSet(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQQueue(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQMultiMap(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQMultiHash(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQMap(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQHash(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType, void* &pointer);

QTJAMBI_EXPORT bool getAsQStack(JNIEnv *env, jobject collection, const std::type_info& expectedElementType, const QMetaType& expectedElementMetaType, void* &pointer);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QTJAMBI_EXPORT bool getAsQVector(JNIEnv *env, jobject collection, const std::type_info& expectedElementType, const QMetaType& expectedElementMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQLinkedList(JNIEnv *env, jobject collection, const std::type_info& expectedElementType, const QMetaType& expectedElementMetaType, void* &pointer);
#endif
QTJAMBI_EXPORT bool getAsQList(JNIEnv *env, jobject collection, const std::type_info& expectedElementType, const QMetaType& expectedElementMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQSet(JNIEnv *env, jobject collection, const std::type_info& expectedElementType, const QMetaType& expectedElementMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQQueue(JNIEnv *env, jobject collection, const std::type_info& expectedElementType, const QMetaType& expectedElementMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQMultiMap(JNIEnv *env, jobject mapObject, const std::type_info& expectedKeyType, const QMetaType& expectedKeyMetaType, const std::type_info& expectedValueType, const QMetaType& expectedValueMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQMultiHash(JNIEnv *env, jobject mapObject, const std::type_info& expectedKeyType, const QMetaType& expectedKeyMetaType, const std::type_info& expectedValueType, const QMetaType& expectedValueMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQMap(JNIEnv *env, jobject mapObject, const std::type_info& expectedKeyType, const QMetaType& expectedKeyMetaType, const std::type_info& expectedValueType, const QMetaType& expectedValueMetaType, void* &pointer);
QTJAMBI_EXPORT bool getAsQHash(JNIEnv *env, jobject mapObject, const std::type_info& expectedKeyType, const QMetaType& expectedKeyMetaType, const std::type_info& expectedValueType, const QMetaType& expectedValueMetaType, void* &pointer);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
template<typename T>
bool testQVector(JNIEnv *env, jobject collection){
    return testQVector(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T));
}

template<typename T>
bool testQLinkedList(JNIEnv *env, jobject collection){
    return testQLinkedList(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T));
}

template<typename T>
bool getAsQLinkedList(JNIEnv *env, jobject collection, QLinkedList<T> * &pointer){
    return getAsQLinkedList(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(pointer));
}

template<typename T>
bool getAsQVector(JNIEnv *env, jobject collection, QVector<T> * &pointer){
    return getAsQVector(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(pointer));
}
#endif

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

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
template<typename T>
struct AsList{
    typedef QList<T> Type1;
};

template<>
struct AsList<QString>{
    typedef QStringList Type1;
    typedef QList<QString> Type2;
};

template<typename T>
bool getAsQList(JNIEnv *env, jobject collection, typename AsList<T>::Type1 * &pointer){
    return getAsQList(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(pointer));
}
template<typename T>
bool getAsQList(JNIEnv *env, jobject collection, typename AsList<T>::Type2 * &pointer){
    return getAsQList(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(pointer));
}
#else
template<typename T>
bool getAsQList(JNIEnv *env, jobject collection, QList<T> * &pointer){
    return getAsQList(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(pointer));
}
#endif

template<typename T>
bool getAsQSet(JNIEnv *env, jobject collection, QSet<T> * &pointer){
    return getAsQSet(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(pointer));
}

template<typename T>
bool getAsQQueue(JNIEnv *env, jobject collection, QQueue<T> * &pointer){
    return getAsQQueue(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T), reinterpret_cast<void*&>(pointer));
}

template<typename K, typename V>
bool getAsQMultiMap(JNIEnv *env, jobject mapObject, QMultiMap<K,V> * &pointer){
    return getAsQMultiMap(env, mapObject, QtJambiPrivate::qtjambi_type<K>::id(), QTJAMBI_METATYPE_FROM_TYPE2(K), QtJambiPrivate::qtjambi_type<V>::id(), QTJAMBI_METATYPE_FROM_TYPE2(V), reinterpret_cast<void*&>(pointer));
}

template<typename K, typename V>
bool getAsQMultiHash(JNIEnv *env, jobject mapObject, QMultiHash<K,V> * &pointer){
    return getAsQMultiHash(env, mapObject, QtJambiPrivate::qtjambi_type<K>::id(), QTJAMBI_METATYPE_FROM_TYPE2(K), QtJambiPrivate::qtjambi_type<V>::id(), QTJAMBI_METATYPE_FROM_TYPE2(V), reinterpret_cast<void*&>(pointer));
}

template<typename K, typename V>
bool getAsQMap(JNIEnv *env, jobject mapObject, QMap<K,V> * &pointer){
    return getAsQMap(env, mapObject, QtJambiPrivate::qtjambi_type<K>::id(), QTJAMBI_METATYPE_FROM_TYPE2(K), QtJambiPrivate::qtjambi_type<V>::id(), QTJAMBI_METATYPE_FROM_TYPE2(V), reinterpret_cast<void*&>(pointer));
}

template<typename K, typename V>
bool getAsQHash(JNIEnv *env, jobject mapObject, QHash<K,V> * &pointer){
    return getAsQHash(env, mapObject, QtJambiPrivate::qtjambi_type<K>::id(), QTJAMBI_METATYPE_FROM_TYPE2(K), QtJambiPrivate::qtjambi_type<V>::id(), QTJAMBI_METATYPE_FROM_TYPE2(V), reinterpret_cast<void*&>(pointer));
}

} // namespace ContainerAPI

enum class SequentialContainerType{
    QList,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QVector,
    QLinkedList,
#endif
    QStack,
    QQueue,
    QSet
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
    virtual void dispose();
    virtual AbstractContainerAccess* clone() = 0;
    virtual void assign(void* container, const void* other) = 0;
    virtual size_t sizeOf() = 0;
    virtual void* constructContainer(void* placement, const void* copyOf = nullptr) = 0;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    virtual void* constructContainer(void* placement, void* move) = 0;
    void* createContainer(void* moved);
#endif
    virtual bool destructContainer(void* container) = 0;
    virtual int registerContainer(const QByteArray& containerTypeName) = 0;
    virtual const QObject* getOwner(const void* container);
    void* createContainer(const void* copy = nullptr);
    void deleteContainer(void* container);
    static bool isPointerType(const QMetaType& metaType);
    static AbstractContainerAccess* create(JNIEnv* env, SequentialContainerType containerType, const QMetaType& memberMetaType);
    static AbstractContainerAccess* create(JNIEnv* env, AssociativeContainerType mapType, const QMetaType& memberMetaType1, const QMetaType& memberMetaType2);
    static AbstractContainerAccess* create(JNIEnv* env, SequentialContainerType containerType,
                                                             const QMetaType& metaType,
                                                             size_t align, size_t size,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                              bool isStaticType,
#endif
                                                             bool isPointer,
                                                             const QHashFunction& hashFunction,
                                                             const InternalToExternalConverter& memberConverter,
                                                             const ExternalToInternalConverter& memberReConverter);
    static AbstractContainerAccess* create(JNIEnv* env, AssociativeContainerType mapType,
                                                            const QMetaType& memberMetaType1,
                                                            size_t align1, size_t size1,
                                                            bool isPointer1,
                                                            const QHashFunction& hashFunction1,
                                                            const InternalToExternalConverter& memberConverter1,
                                                            const ExternalToInternalConverter& memberReConverter1,
                                                            const QMetaType& memberMetaType2,
                                                            size_t align2, size_t size2,
                                                            bool isPointer2,
                                                            const QHashFunction& hashFunction2,
                                                            const InternalToExternalConverter& memberConverter2,
                                                            const ExternalToInternalConverter& memberReConverter2);

    Q_DISABLE_COPY_MOVE(AbstractContainerAccess)
protected:
    AbstractContainerAccess();
    virtual ~AbstractContainerAccess();
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    static bool isStaticType(const QMetaType&);
#endif
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
private:
    size_t sizeOf() final override;
    void* constructContainer(void* placement, const void* copyOf = nullptr) final override;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    void* constructContainer(void* placement, void* move) final override;
#endif
    bool destructContainer(void* container) final override;
    void assign(void*, const void*) final override;
    int registerContainer(const QByteArray&) final override;
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

class QTJAMBI_EXPORT AbstractListAccess : public AbstractContainerAccess{
protected:
    ~AbstractListAccess() override;
    AbstractListAccess();
public:
    AbstractListAccess* clone() override = 0;
    virtual void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) = 0;
    virtual bool isConstant() = 0;
    virtual const QMetaType& elementMetaType() = 0;
    virtual void appendList(JNIEnv * env, void* container, jobject list) = 0;
    virtual jobject at(JNIEnv * env, const void* container, jint index) = 0;
    virtual jobject value(JNIEnv * env, const void* container, jint index) = 0;
    virtual jobject value(JNIEnv * env, const void* container, jint index, jobject defaultValue) = 0;
    virtual void swapItemsAt(JNIEnv * env, void* container, jint index1, jint index2) = 0;
    virtual jboolean startsWith(JNIEnv * env, const void* container, jobject value) = 0;
    virtual jint size(JNIEnv * env, const void* container) = 0;
    virtual void reserve(JNIEnv * env, void* container, jint size) = 0;
    virtual void replace(JNIEnv * env, void* container, jint index, jobject value) = 0;
    virtual void remove(JNIEnv * env, void* container, jint index, jint n) = 0;
    virtual jint removeAll(JNIEnv * env, void* container, jobject value) = 0;
    virtual jboolean equal(JNIEnv * env, const void* container, jobject other) = 0;
    virtual void move(JNIEnv * env, void* container, jint index1, jint index2) = 0;
    virtual jobject mid(JNIEnv * env, const void* container, jint index1, jint index2) = 0;
    virtual jint lastIndexOf(JNIEnv * env, const void* container, jobject value, jint index) = 0;
    virtual void insert(JNIEnv * env, void* container, jint index, jint n, jobject value) = 0;
    virtual jint indexOf(JNIEnv * env, const void* container, jobject value, jint index) = 0;
    virtual jboolean endsWith(JNIEnv * env, const void* container, jobject value) = 0;
    virtual jint count(JNIEnv * env, const void* container, jobject value) = 0;
    virtual jboolean contains(JNIEnv * env, const void* container, jobject value) = 0;
    virtual void clear(JNIEnv * env, void* container) = 0;
    virtual jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) = 0;
    virtual jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) = 0;
    virtual jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) = 0;
    virtual jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) = 0;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    virtual jint capacity(JNIEnv * env, const void* container) = 0;
    virtual void fill(JNIEnv * env, void* container, jobject value, jint size) = 0;
    virtual void resize(JNIEnv * env, void* container, jint newSize) = 0;
    virtual void squeeze(JNIEnv * env, void* container) = 0;
#endif
    Q_DISABLE_COPY_MOVE(AbstractListAccess)
};

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
class QTJAMBI_EXPORT AbstractVectorAccess : public AbstractContainerAccess{
protected:
    ~AbstractVectorAccess() override;
    AbstractVectorAccess();
public:
    AbstractVectorAccess* clone() override = 0;
    virtual void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) = 0;
    virtual bool isConstant() = 0;
    virtual const QMetaType& elementMetaType() = 0;
    virtual void appendVector(JNIEnv * env, void* container, jobject list) = 0;
    virtual jobject at(JNIEnv * env, const void* container, jint index) = 0;
    virtual jobject value(JNIEnv * env, const void* container, jint index) = 0;
    virtual jobject value(JNIEnv * env, const void* container, jint index, jobject defaultValue) = 0;
    virtual void swapItemsAt(JNIEnv * env, void* container, jint index1, jint index2) = 0;
    virtual jboolean startsWith(JNIEnv * env, const void* container, jobject value) = 0;
    virtual jint size(JNIEnv * env, const void* container) = 0;
    virtual void reserve(JNIEnv * env, void* container, jint size) = 0;
    virtual void replace(JNIEnv * env, void* container, jint index, jobject value) = 0;
    virtual jint removeAll(JNIEnv * env, void* container, jobject value) = 0;
    virtual jboolean equal(JNIEnv * env, const void* container, jobject other) = 0;
    virtual void move(JNIEnv * env, void* container, jint index1, jint index2) = 0;
    virtual jobject mid(JNIEnv * env, const void* container, jint index1, jint index2) = 0;
    virtual jint lastIndexOf(JNIEnv * env, const void* container, jobject value, jint index) = 0;
    virtual jint indexOf(JNIEnv * env, const void* container, jobject value, jint index) = 0;
    virtual jboolean endsWith(JNIEnv * env, const void* container, jobject value) = 0;
    virtual jint count(JNIEnv * env, const void* container, jobject value) = 0;
    virtual jboolean contains(JNIEnv * env, const void* container, jobject value) = 0;
    virtual void clear(JNIEnv * env, void* container) = 0;
    virtual jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) = 0;
    virtual jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) = 0;
    virtual jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) = 0;
    virtual jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) = 0;
    virtual jint capacity(JNIEnv * env, const void* container) = 0;
    virtual void fill(JNIEnv * env, void* container, jobject value, jint size) = 0;
    virtual void remove(JNIEnv * env, void* container, jint index, jint n) = 0;
    virtual void insert(JNIEnv * env, void* container, jint index, jint n, jobject value) = 0;
    virtual void resize(JNIEnv * env, void* container, jint newSize) = 0;
    virtual void squeeze(JNIEnv * env, void* container) = 0;
    Q_DISABLE_COPY_MOVE(AbstractVectorAccess)
};

class QTJAMBI_EXPORT AbstractLinkedListAccess : public AbstractContainerAccess{
protected:
    ~AbstractLinkedListAccess() override;
    AbstractLinkedListAccess();
public:
    AbstractLinkedListAccess* clone() override = 0;
    virtual void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) = 0;
    virtual bool isConstant() = 0;
    virtual const QMetaType& elementMetaType() = 0;
    virtual void append(JNIEnv * env, void* container, jobject value) = 0;
    virtual jobject first(JNIEnv * env, const void* container) = 0;
    virtual jobject last(JNIEnv * env, const void* container) = 0;
    virtual void clear(JNIEnv * env, void* container) = 0;
    virtual jboolean contains(JNIEnv * env, const void* container, jobject value) = 0;
    virtual jint count(JNIEnv * env, const void* container, jobject value) = 0;
    virtual jboolean endsWith(JNIEnv * env, const void* container, jobject value) = 0;
    virtual jboolean equal(JNIEnv * env, const void* container, jobject other) = 0;
    virtual void prepend(JNIEnv * env, void* container, jobject value) = 0;
    virtual void removeFirst(JNIEnv * env, void* container) = 0;
    virtual jint removeAll(JNIEnv * env, void* container, jobject value) = 0;
    virtual void removeLast(JNIEnv * env, void* container) = 0;
    virtual jboolean removeOne(JNIEnv * env, void* container, jobject value) = 0;
    virtual jint size(JNIEnv * env, const void* container) = 0;
    virtual jboolean startsWith(JNIEnv * env, const void* container, jobject value) = 0;
    virtual jobject takeFirst(JNIEnv * env, void* container) = 0;
    virtual jobject takeLast(JNIEnv * env, void* container) = 0;
    virtual jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) = 0;
    virtual jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) = 0;
    virtual jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) = 0;
    virtual jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) = 0;
    Q_DISABLE_COPY_MOVE(AbstractLinkedListAccess)
};

#endif

class QTJAMBI_EXPORT AbstractSetAccess : public AbstractContainerAccess{
protected:
    ~AbstractSetAccess() override;
    AbstractSetAccess();
public:
    AbstractSetAccess* clone() override = 0;
    virtual void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) = 0;
    virtual bool isConstant() = 0;
    virtual const QMetaType& elementMetaType() = 0;
    virtual jint capacity(JNIEnv * env, const void* container) = 0;
    virtual void clear(JNIEnv * env, void* container) = 0;
    virtual jboolean contains(JNIEnv * env, const void* container, jobject value) = 0;
    virtual void insert(JNIEnv * env, void* container, jobject value) = 0;
    virtual void intersect(JNIEnv * env, void* container, jobject other) = 0;
    virtual jboolean intersects(JNIEnv * env, const void* container, jobject other) = 0;
    virtual jboolean equal(JNIEnv * env, const void* container, jobject other) = 0;
    virtual jboolean remove(JNIEnv * env, void* container, jobject value) = 0;
    virtual void reserve(JNIEnv * env, void* container, jint newSize) = 0;
    virtual jint size(JNIEnv * env, const void* container) = 0;
    virtual void subtract(JNIEnv * env, void* container, jobject other) = 0;
    virtual void unite(JNIEnv * env, void* container, jobject other) = 0;
    virtual jobject values(JNIEnv * env, const void* container) = 0;
    virtual jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) = 0;
    virtual jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) = 0;
    Q_DISABLE_COPY_MOVE(AbstractSetAccess)
};

class QTJAMBI_EXPORT AbstractMapAccess : public AbstractContainerAccess{
protected:
    ~AbstractMapAccess() override;
    AbstractMapAccess();
public:
    AbstractMapAccess* clone() override = 0;
    virtual void analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) = 0;
    virtual bool isConstant() = 0;
    virtual const QMetaType& keyMetaType() = 0;
    virtual const QMetaType& valueMetaType() = 0;
    virtual void clear(JNIEnv *,void*) = 0;
    virtual jboolean contains(JNIEnv *,const void*,jobject) = 0;
    virtual jint count(JNIEnv *,const void*,jobject) = 0;
    virtual jobject find(JNIEnv *,QtJambiNativeID,void*,jobject) = 0;
    virtual jobject constFind(JNIEnv *,QtJambiNativeID,const void*,jobject) = 0;
    virtual jobject first(JNIEnv *,const void*) = 0;
    virtual jobject firstKey(JNIEnv *,const void*) = 0;
    virtual void insert(JNIEnv *,void*,jobject,jobject) = 0;
    virtual jobject key(JNIEnv *,const void*,jobject,jobject) = 0;
    virtual jobject keys(JNIEnv *,const void*) = 0;
    virtual jobject keys(JNIEnv *,const void*,jobject) = 0;
    virtual jobject last(JNIEnv *,const void*) = 0;
    virtual jobject lastKey(JNIEnv *,const void*) = 0;
    virtual jobject constLowerBound(JNIEnv *,QtJambiNativeID,const void*,jobject) = 0;
    virtual jboolean equal(JNIEnv *,const void*,jobject) = 0;
    virtual jint remove(JNIEnv *,void*,jobject) = 0;
    virtual jint size(JNIEnv *,const void*) = 0;
    virtual jobject take(JNIEnv *,void*,jobject) = 0;
    virtual jobject constUpperBound(JNIEnv *,QtJambiNativeID,const void*,jobject) = 0;
    virtual jobject value(JNIEnv *,const void*,jobject,jobject) = 0;
    virtual jobject values(JNIEnv *,const void*) = 0;
    virtual bool keyLessThan(JNIEnv *,jobject,jobject) = 0;
    virtual jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) = 0;
    virtual jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) = 0;
    virtual jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) = 0;
    virtual jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) = 0;
};

class QTJAMBI_EXPORT AbstractMultiMapAccess : public AbstractMapAccess{
protected:
    ~AbstractMultiMapAccess() override;;
    AbstractMultiMapAccess();;
public:
    AbstractMultiMapAccess* clone() override = 0;
    bool isConstant() override = 0;
    using AbstractMapAccess::contains;
    using AbstractMapAccess::count;
    using AbstractMapAccess::find;
    using AbstractMapAccess::constFind;
    using AbstractMapAccess::remove;
    using AbstractMapAccess::values;
    virtual jobject uniqueKeys(JNIEnv *,const void*) = 0;
    virtual void unite(JNIEnv *,void*,jobject) = 0;
    virtual jobject values(JNIEnv *,const void*,jobject) = 0;
    virtual jboolean contains(JNIEnv *,const void*,jobject,jobject) = 0;
    virtual jint count(JNIEnv *,const void*,jobject,jobject) = 0;
    virtual jobject find(JNIEnv *,QtJambiNativeID,void*,jobject,jobject) = 0;
    virtual jobject constFind(JNIEnv *,QtJambiNativeID,const void*,jobject,jobject) = 0;
    virtual jint remove(JNIEnv *,void*,jobject,jobject) = 0;
    virtual void replace(JNIEnv *,void*,jobject,jobject) = 0;
};

class QTJAMBI_EXPORT AbstractHashAccess : public AbstractContainerAccess{
protected:
    ~AbstractHashAccess() override;;
    AbstractHashAccess();;
public:
    AbstractHashAccess* clone() override = 0;
    virtual void analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) = 0;
    virtual bool isConstant() = 0;
    virtual const QMetaType& keyMetaType() = 0;
    virtual const QMetaType& valueMetaType() = 0;
    virtual jint capacity(JNIEnv *,const void*) = 0;
    virtual void clear(JNIEnv *,void*) = 0;
    virtual jboolean contains(JNIEnv *,const void*,jobject) = 0;
    virtual jint count(JNIEnv *,const void*,jobject) = 0;
    virtual jobject find(JNIEnv *,QtJambiNativeID,void*,jobject) = 0;
    virtual jobject constFind(JNIEnv *,QtJambiNativeID,const void*,jobject) = 0;
    virtual void insert(JNIEnv *,void*,jobject,jobject) = 0;
    virtual jobject key(JNIEnv *,const void*,jobject,jobject) = 0;
    virtual jobject keys(JNIEnv *,const void*) = 0;
    virtual jobject keys(JNIEnv *,const void*,jobject) = 0;
    virtual jboolean equal(JNIEnv *,const void*,jobject) = 0;
    virtual jint remove(JNIEnv *,void*,jobject) = 0;
    virtual void reserve(JNIEnv *,void*,jint) = 0;
    virtual jint size(JNIEnv *,const void*) = 0;
    virtual jobject take(JNIEnv *,void*,jobject) = 0;
    virtual jobject value(JNIEnv *,const void*,jobject,jobject) = 0;
    virtual jobject values(JNIEnv *,const void*) = 0;
    virtual jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) = 0;
    virtual jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) = 0;
    virtual jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) = 0;
    virtual jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) = 0;
};

class QTJAMBI_EXPORT AbstractMultiHashAccess : public AbstractHashAccess{
protected:
    ~AbstractMultiHashAccess() override;;
    AbstractMultiHashAccess();;
public:
    AbstractMultiHashAccess* clone() override = 0;
    bool isConstant() override = 0;
    using AbstractHashAccess::contains;
    using AbstractHashAccess::count;
    using AbstractHashAccess::find;
    using AbstractHashAccess::constFind;
    using AbstractHashAccess::remove;
    using AbstractHashAccess::values;
    virtual jobject uniqueKeys(JNIEnv *,const void*) = 0;
    virtual void unite(JNIEnv *,void*,jobject) = 0;
    virtual jobject values(JNIEnv *,const void*,jobject) = 0;
    virtual jboolean contains(JNIEnv *,const void*,jobject,jobject) = 0;
    virtual jint count(JNIEnv *,const void*,jobject,jobject) = 0;
    virtual jobject find(JNIEnv *,QtJambiNativeID,void*,jobject,jobject) = 0;
    virtual jobject constFind(JNIEnv *,QtJambiNativeID,const void*,jobject,jobject) = 0;
    virtual jint remove(JNIEnv *,void*,jobject,jobject) = 0;
    virtual void replace(JNIEnv *,void*,jobject,jobject) = 0;
};

class QTJAMBI_EXPORT AbstractPairAccess : public AbstractContainerAccess{
protected:
    ~AbstractPairAccess() override;;
    AbstractPairAccess();;
public:
    AbstractPairAccess* clone() override = 0;
    virtual bool isConstant() = 0;
    virtual jobject first(JNIEnv *,const void*) = 0;
    virtual jobject second(JNIEnv *,const void*) = 0;
    virtual void setFirst(JNIEnv *,void*,jobject) = 0;
    virtual void setSecond(JNIEnv *,void*,jobject) = 0;
};

namespace ContainerAPI{

QTJAMBI_EXPORT QPair<void*,AbstractContainerAccess*> fromNativeId(QtJambiNativeID nativeId);

}//namespace ContainerAPI


namespace QtJambiPrivate{

template<template<typename> class Container>
struct SequentialContainerAnalyzer{
};

template<>
struct SequentialContainerAnalyzer<QList>{
    static constexpr SequentialContainerType type = SequentialContainerType::QList;
};

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#ifdef QVECTOR_H
template<>
struct SequentialContainerAnalyzer<QVector>{
    static constexpr SequentialContainerType type = SequentialContainerType::QVector;
};
#endif
#ifdef QLINKEDLIST_H
template<>
struct SequentialContainerAnalyzer<QLinkedList>{
    static constexpr SequentialContainerType type = SequentialContainerType::QLinkedList;
};
#endif
#endif

template<>
struct SequentialContainerAnalyzer<QStack>{
    static constexpr SequentialContainerType type = SequentialContainerType::QStack;
};

template<>
struct SequentialContainerAnalyzer<QQueue>{
    static constexpr SequentialContainerType type = SequentialContainerType::QQueue;
};

template<>
struct SequentialContainerAnalyzer<QSet>{
    static constexpr SequentialContainerType type = SequentialContainerType::QSet;
};

template<template<typename,typename> class Container>
struct AssociativeContainerAnalyzer{
};

template<>
struct AssociativeContainerAnalyzer<QMap>{
    static constexpr AssociativeContainerType type = AssociativeContainerType::QMap;
};

template<>
struct AssociativeContainerAnalyzer<QHash>{
    static constexpr AssociativeContainerType type = AssociativeContainerType::QHash;
};

template<>
struct AssociativeContainerAnalyzer<QMultiMap>{
    static constexpr AssociativeContainerType type = AssociativeContainerType::QMultiMap;
};

template<>
struct AssociativeContainerAnalyzer<QMultiHash>{
    static constexpr AssociativeContainerType type = AssociativeContainerType::QMultiHash;
};

template<>
struct AssociativeContainerAnalyzer<QPair>{
    static constexpr AssociativeContainerType type = AssociativeContainerType::QPair;
};

typedef hash_type(*HashWrapper)(const void* ptr, hash_type seed, QHashFunction hashFunction);
typedef hash_type(*HashWrapper2)(const void* ptr, hash_type seed, QHashFunction hashFunction1, QHashFunction hashFunction2);

template<typename Container>
hash_type sequentialContainerHash(const void* ptr, hash_type seed, QHashFunction hashFunction)
{
    const Container& container = *reinterpret_cast<const Container*>(ptr);
    hash_type prime = 31;
    hash_type result = seed;
    result = prime * result + qHash(container.size(), result);
    for(typename Container::const_iterator iter = container.begin();
        iter != container.end(); ++iter){
        result = prime * result + hashFunction(& (*iter), result);
    }
    return result;
}

template<typename Container>
hash_type associativeContainerHash(const void* ptr, hash_type seed, QHashFunction hashFunction1, QHashFunction hashFunction2)
{
    const Container& container = *reinterpret_cast<const Container*>(ptr);
    hash_type prime = 31;
    hash_type result = seed;
    result = prime * result + qHash(container.size(), result);
    for(typename Container::const_iterator iter = container.begin();
        iter != container.end(); ++iter){
        result = prime * result + hashFunction1(&iter.key(), result);
        result = prime * result + hashFunction2(&iter.value(), result);
    }
    return result;
}

template<typename Container>
hash_type pairHashWrapper(const void* ptr, hash_type seed, QHashFunction hashFunction1, QHashFunction hashFunction2){
    const Container& container = *reinterpret_cast<const Container*>(ptr);
    hash_type prime = 31;
    hash_type result = seed;
    result = prime * result + hashFunction1(&container.first, result);
    result = prime * result + hashFunction2(&container.second, result);
    return result;
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
template<typename T, bool enabled = QtJambiPrivate::supports_debugstream<T>::value>
struct DebugStreamFunction : public QtPrivate::AbstractDebugStreamFunction
{
    static QtPrivate::AbstractDebugStreamFunction* create(){ return new DebugStreamFunction(); }
private:
    DebugStreamFunction()
        : AbstractDebugStreamFunction(stream, destroy) {}
    static void stream(const QtPrivate::AbstractDebugStreamFunction *, QDebug& dbg, const void *r)
    {
        const T *rhs = static_cast<const T *>(r);
        operator<<(dbg, *rhs);
    }

    static void destroy(QtPrivate::AbstractDebugStreamFunction *_this)
    {
        delete static_cast<DebugStreamFunction *>(_this);
    }
};

template<typename T>
struct DebugStreamFunction<T,false>
{
    static QtPrivate::AbstractDebugStreamFunction* create(){ return nullptr; }
};

typedef const QtPrivate::AbstractConverterFunction*(*ConverterFunctionFactory1)(int metaTypeId);
typedef const QtPrivate::AbstractConverterFunction*(*ConverterFunctionFactory2)(int metaTypeId1, int metaTypeId2);

QTJAMBI_EXPORT int registerSequentialContainerType(const QByteArray& typeName,
                                                   size_t containerSize, size_t containerAlign,
                                                   bool isPointer,
                                                   const QMetaType& metaType,
                                                   QMetaType::Destructor destructor,
                                                   QMetaType::Constructor constructor,
                                                   QMetaType::SaveOperator saveOperator,
                                                   QMetaType::LoadOperator loadOperator,
                                                   const QtPrivate::AbstractDebugStreamFunction * debugStreamFunction,
                                                   const QtPrivate::AbstractComparatorFunction * comparator,
                                                   ConverterFunctionFactory1 createConverterFunction,
                                                   HashWrapper hashWrapper);

template<typename Container, size_t size>
int registerSequentialContainerType(const QByteArray& typeName, const QMetaType& metaType){
    int newMetaType = QMetaType::type(typeName);
    if(newMetaType==QMetaType::UnknownType){
        static QtPrivate::BuiltInEqualsComparatorFunction<Container> comparator;
        newMetaType = registerSequentialContainerType(typeName, sizeof(Container), Q_ALIGNOF(Container), size==0, metaType,
                                                      QtMetaTypePrivate::QMetaTypeFunctionHelper<Container>::Destruct,
                                                      QtMetaTypePrivate::QMetaTypeFunctionHelper<Container>::Construct,
                                                      QtMetaTypePrivate::QMetaTypeFunctionHelper<Container, size!=0>::Save,
                                                      QtMetaTypePrivate::QMetaTypeFunctionHelper<Container, size!=0>::Load,
                                                      DebugStreamFunction<Container>::create(),
                                                      &comparator,
                                                      [](int metaTypeId) -> const QtPrivate::AbstractConverterFunction* {
                                                              struct Converter : QtPrivate::AbstractConverterFunction
                                                              {
                                                                  explicit Converter(int metaType)
                                                                      : QtPrivate::AbstractConverterFunction(convert),
                                                                        m_metaType(metaType) {}
                                                                  static bool convert(const QtPrivate::AbstractConverterFunction *_this, const void *in, void *out)
                                                                  {
                                                                      QtMetaTypePrivate::QSequentialIterableImpl *t = static_cast<QtMetaTypePrivate::QSequentialIterableImpl *>(out);
                                                                      t->_iterable = in;
                                                                      t->_iterator = nullptr;
                                                                      t->_metaType_id = static_cast<const Converter *>(_this)->m_metaType;
                                                                      t->_metaType_flags = size==0;
                                                                      t->_iteratorCapabilities = QtMetaTypePrivate::ForwardCapability;
                                                                      t->_size = QtMetaTypePrivate::QSequentialIterableImpl::sizeImpl<Container>;
                                                                      t->_at = QtMetaTypePrivate::QSequentialIterableImpl::atImpl<Container>;
                                                                      t->_moveToBegin = QtMetaTypePrivate::QSequentialIterableImpl::moveToBeginImpl<Container>;
                                                                      t->_moveToEnd = QtMetaTypePrivate::QSequentialIterableImpl::moveToEndImpl<Container>;
                                                                      t->_advance = QtMetaTypePrivate::IteratorOwner<typename Container::const_iterator>::advance;
                                                                      t->_get = QtMetaTypePrivate::QSequentialIterableImpl::getImpl<Container>;
                                                                      t->_destroyIter = QtMetaTypePrivate::IteratorOwner<typename Container::const_iterator>::destroy;
                                                                      t->_equalIter = QtMetaTypePrivate::IteratorOwner<typename Container::const_iterator>::equal;
                                                                      t->_copyIter = QtMetaTypePrivate::IteratorOwner<typename Container::const_iterator>::assign;
                                                                      return true;
                                                                  }
                                                                  int m_metaType;
                                                              };
                                                              return new Converter(metaTypeId);
                                                          },
                                                      sequentialContainerHash<Container>);
    }
    return newMetaType;
}

QTJAMBI_EXPORT int registerAssociativeContainerType(const QByteArray& typeName,
                                                     size_t containerSize, size_t containerAlign,
                                                     bool isPointer1, const QMetaType& metaType1,
                                                     bool isPointer2, const QMetaType& metaType2,
                                                     QMetaType::Destructor destructor,
                                                     QMetaType::Constructor constructor,
                                                     QMetaType::SaveOperator saveOperator,
                                                     QMetaType::LoadOperator loadOperator,
                                                     const QtPrivate::AbstractDebugStreamFunction * debugStreamFunction,
                                                     const QtPrivate::AbstractComparatorFunction * comparator,
                                                     bool isPair,
                                                     ConverterFunctionFactory2 createConverterFunction,
                                                     HashWrapper2 hashWrapper);

template<typename Container, size_t size1, size_t size2>
int registerAssociativeContainerType(const QByteArray& typeName, const QMetaType& metaType1, const QMetaType& metaType2){
    int newMetaType = QMetaType::type(typeName);
    if(newMetaType==QMetaType::UnknownType){
        static QtPrivate::BuiltInEqualsComparatorFunction<Container> comparator;
        newMetaType = registerAssociativeContainerType(typeName, sizeof(Container), Q_ALIGNOF(Container), size1==0, metaType1, size2==0, metaType2,
                                                        QtMetaTypePrivate::QMetaTypeFunctionHelper<Container>::Destruct,
                                                        QtMetaTypePrivate::QMetaTypeFunctionHelper<Container>::Construct,
                                                        QtMetaTypePrivate::QMetaTypeFunctionHelper<Container, size1!=0 && size2!=0>::Save,
                                                        QtMetaTypePrivate::QMetaTypeFunctionHelper<Container, size1!=0 && size2!=0>::Load,
                                                        DebugStreamFunction<Container>::create(),
                                                        &comparator,
                                                        false,
                                                        [](int metaTypeId1, int metaTypeId2) -> const QtPrivate::AbstractConverterFunction* {
                                                                  struct Converter : QtPrivate::AbstractConverterFunction
                                                                  {
                                                                      explicit Converter(int metaType_key, int metaType_value)
                                                                          : QtPrivate::AbstractConverterFunction(convert),
                                                                            m_metaType_key(metaType_key), m_metaType_value(metaType_value) {}
                                                                      static bool convert(const QtPrivate::AbstractConverterFunction *_this, const void *in, void *out)
                                                                      {
                                                                          QtMetaTypePrivate::QAssociativeIterableImpl *t = static_cast<QtMetaTypePrivate::QAssociativeIterableImpl *>(out);
                                                                          t->_iterable = in;
                                                                          t->_iterator = nullptr;
                                                                          t->_metaType_id_key = static_cast<const Converter *>(_this)->m_metaType_key;
                                                                          t->_metaType_flags_key = size1==0;
                                                                          t->_metaType_id_value = static_cast<const Converter *>(_this)->m_metaType_value;
                                                                          t->_metaType_flags_value = size2==0;
                                                                          t->_size = QtMetaTypePrivate::QAssociativeIterableImpl::sizeImpl<Container>;
                                                                          t->_find = QtMetaTypePrivate::QAssociativeIterableImpl::findImpl<Container>;
                                                                          t->_begin = QtMetaTypePrivate::QAssociativeIterableImpl::beginImpl<Container>;
                                                                          t->_end = QtMetaTypePrivate::QAssociativeIterableImpl::endImpl<Container>;
                                                                          t->_advance = QtMetaTypePrivate::QAssociativeIterableImpl::advanceImpl<Container>;
                                                                          t->_getKey = QtMetaTypePrivate::QAssociativeIterableImpl::getKeyImpl<Container>;
                                                                          t->_getValue = QtMetaTypePrivate::QAssociativeIterableImpl::getValueImpl<Container>;
                                                                          t->_destroyIter = QtMetaTypePrivate::IteratorOwner<typename Container::const_iterator>::destroy;
                                                                          t->_equalIter = QtMetaTypePrivate::IteratorOwner<typename Container::const_iterator>::equal;
                                                                          t->_copyIter = QtMetaTypePrivate::IteratorOwner<typename Container::const_iterator>::assign;
                                                                          return true;
                                                                      }
                                                                      int m_metaType_key;
                                                                      int m_metaType_value;
                                                                  };
                                                                  return new Converter(metaTypeId1, metaTypeId2);
                                                              },
                                                              associativeContainerHash<Container>
                                                        );
    }
    return newMetaType;
}

template<typename Container>
QtMetaTypePrivate::VariantData getFirstImpl(const void * const *pair, int metaTypeId, uint flags)
{
    return QtMetaTypePrivate::VariantData(metaTypeId, &static_cast<const Container*>(*pair)->first, flags);
}

template<typename Container>
QtMetaTypePrivate::VariantData getSecondImpl(const void * const *pair, int metaTypeId, uint flags)
{
    return QtMetaTypePrivate::VariantData(metaTypeId, &static_cast<const Container*>(*pair)->second, flags);
}

template<typename Container, size_t size1, size_t size2>
int registerQPairType(const QByteArray& typeName, const QMetaType& metaType1, const QMetaType& metaType2){
    int newMetaType = QMetaType::type(typeName);
    if(newMetaType==QMetaType::UnknownType){
        static QtPrivate::BuiltInEqualsComparatorFunction<Container> comparator;
        newMetaType = registerAssociativeContainerType(typeName, sizeof(Container), Q_ALIGNOF(Container), size1==0, metaType1, size2==0, metaType2,
                                                        QtMetaTypePrivate::QMetaTypeFunctionHelper<Container>::Destruct,
                                                        QtMetaTypePrivate::QMetaTypeFunctionHelper<Container>::Construct,
                                                        QtMetaTypePrivate::QMetaTypeFunctionHelper<Container, size1!=0 && size2!=0>::Save,
                                                        QtMetaTypePrivate::QMetaTypeFunctionHelper<Container, size1!=0 && size2!=0>::Load,
                                                        DebugStreamFunction<Container>::create(),
                                                        &comparator,
                                                        true,
                                                        [](int metaType1Id, int metaType2Id) -> const QtPrivate::AbstractConverterFunction* {
                                                              struct Converter : QtPrivate::AbstractConverterFunction
                                                              {
                                                                  explicit Converter(int metaType_first, int metaType_second)
                                                                      : QtPrivate::AbstractConverterFunction(convert),
                                                                        m_metaType_first(metaType_first), m_metaType_second(metaType_second) {}
                                                                  static bool convert(const QtPrivate::AbstractConverterFunction *_this, const void *in, void *out)
                                                                  {
                                                                      struct _QPairVariantInterfaceImpl
                                                                      {
                                                                          const void *_pair;
                                                                          int _metaType_id_first;
                                                                          uint _metaType_flags_first;
                                                                          int _metaType_id_second;
                                                                          uint _metaType_flags_second;
                                                                          QtMetaTypePrivate::VariantData (*_getFirst)(const void * const *p, int metaTypeId, uint flags);
                                                                          QtMetaTypePrivate::VariantData (*_getSecond)(const void * const *p, int metaTypeId, uint flags);
                                                                      };

                                                                      _QPairVariantInterfaceImpl *t = static_cast<_QPairVariantInterfaceImpl *>(out);
                                                                      t->_pair = in;
                                                                      t->_metaType_id_first = static_cast<const Converter *>(_this)->m_metaType_first;
                                                                      t->_metaType_flags_first = size1==0;
                                                                      t->_metaType_id_second = static_cast<const Converter *>(_this)->m_metaType_second;
                                                                      t->_metaType_flags_second = size2==0;
                                                                      t->_getFirst = getFirstImpl<Container>;
                                                                      t->_getSecond = getSecondImpl<Container>;
                                                                      return true;
                                                                  }
                                                                  int m_metaType_first;
                                                                  int m_metaType_second;
                                                              };
                                                              return new Converter(metaType1Id, metaType2Id);
                                                        },
                                                        pairHashWrapper<Container>
                                                    );
    }
    return newMetaType;
}

#else
typedef bool(*MetaSequenceIteratorFactory)(void *src, void *target, const QtMetaContainerPrivate::QMetaSequenceInterface* iface);
typedef bool(*MetaSequenceConstIteratorFactory)(const void *src, void *target, const QtMetaContainerPrivate::QMetaSequenceInterface* iface);
typedef bool(*MetaAssociationIteratorFactory)(void *src, void *target, const QtMetaContainerPrivate::QMetaAssociationInterface* iface);
typedef bool(*MetaAssociationConstIteratorFactory)(const void *src, void *target, const QtMetaContainerPrivate::QMetaAssociationInterface* iface);
typedef bool(*PairAccessFactory)(const void *src, void *target, const QMetaType& metaType1, const QMetaType& metaType2);

QTJAMBI_EXPORT int registerSequentialContainerType(const QByteArray& typeName,
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
                                                   HashWrapper hashWrapper);

template<typename Container, size_t size>
static int registerSequentialContainerType(const QByteArray& typeName, const QMetaType& metaType){
    int newMetaType = QMetaType::fromName(typeName).id();
    if(newMetaType==QMetaType::UnknownType){
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
                                                sequentialContainerHash<Container>);
    }
    return newMetaType;
}

QTJAMBI_EXPORT int registerAssociativeContainerType(const QByteArray& typeName,
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
                                                     HashWrapper2 hashWrapper);

template<typename Container, size_t size1, size_t size2>
int registerAssociativeContainerType(const QByteArray& typeName, const QMetaType& metaType1, const QMetaType& metaType2){
    int newMetaType = QMetaType::fromName(typeName).id();
    if(newMetaType==QMetaType::UnknownType){
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
                                                        associativeContainerHash<Container>
        );
    }
    return newMetaType;
}

template<typename Container, size_t size1, size_t size2>
static int registerQPairType(const QByteArray& typeName, const QMetaType& metaType1, const QMetaType& metaType2){
    int newMetaType = QMetaType::fromName(typeName).id();
    if(newMetaType==QMetaType::UnknownType){
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
                                                        pairHashWrapper<Container>
                                                    );
    }
    return newMetaType;
}

#endif

}//namespace QtJambiPrivate

#endif // CONTAINERAPI_H

