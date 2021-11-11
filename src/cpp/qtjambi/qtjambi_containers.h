/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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


#ifndef QTJAMBI_CONTAINERS_H
#define QTJAMBI_CONTAINERS_H

#include <functional>
#include <typeinfo>

#include "qtjambi_global.h"
#include "qtjambi_utils.h"
#include "qtjambi_typetests.h"

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

        QByteArray normalizedTypeName = QMetaObject::normalizedType(qtjambi_type_name(typeid(T)));
        id = QMetaType::type(normalizedTypeName);
        if (id > QMetaType::UnknownType)
            return id;
        QMetaType::TypeFlags flags(QtPrivate::QMetaTypeTypeFlags<T>::Flags);

        if (QtPrivate::MetaTypeDefinedHelper<T, QMetaTypeId2<T>::Defined && !QMetaTypeId2<T>::IsBuiltIn>::Defined)
            flags |= QMetaType::WasDeclaredAsMetaType;

        id = QMetaType::registerNormalizedType(normalizedTypeName,
                                       QtMetaTypePrivate::QMetaTypeFunctionHelper<T, supports_StandardConstructor<T>::value && supports_CopyConstructor<T>::value>::Destruct,
                                       QtMetaTypePrivate::QMetaTypeFunctionHelper<T, supports_Destructor<T>::value>::Construct,
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
int qtjambiRegisterMetaType(){
    return MetaTypeRegistrationHelper<typename std::remove_cv<T>::type>::registerMetaType();
}

}

#define QTJAMBI_METATYPE_FROM_TYPE(T) QtJambiPrivate::qtjambiRegisterMetaType<T>()
#define QTJAMBI_METATYPE_FROM_TYPE2(T) QMetaType(QTJAMBI_METATYPE_FROM_TYPE(T))
#endif

QTJAMBI_EXPORT bool qtjambi_is_QStack(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QTJAMBI_EXPORT bool qtjambi_is_QVector(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType);
QTJAMBI_EXPORT bool qtjambi_is_QLinkedList(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType);
#endif
QTJAMBI_EXPORT bool qtjambi_is_QList(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType);
QTJAMBI_EXPORT bool qtjambi_is_QSet(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType);
QTJAMBI_EXPORT bool qtjambi_is_QQueue(JNIEnv *env, jobject collection, const QMetaType& expectedElementMetaType);
QTJAMBI_EXPORT bool qtjambi_is_QMultiMap(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType);
QTJAMBI_EXPORT bool qtjambi_is_QMultiHash(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType);
QTJAMBI_EXPORT bool qtjambi_is_QMap(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType);
QTJAMBI_EXPORT bool qtjambi_is_QHash(JNIEnv *env, jobject mapObject, const QMetaType& expectedKeyMetaType, const QMetaType& expectedValueMetaType);

QTJAMBI_EXPORT bool qtjambi_is_QStack(JNIEnv *env, jobject collection, const std::type_info& expectedElementType, const QMetaType& expectedElementMetaType);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QTJAMBI_EXPORT bool qtjambi_is_QVector(JNIEnv *env, jobject collection, const std::type_info& expectedElementType, const QMetaType& expectedElementMetaType);
QTJAMBI_EXPORT bool qtjambi_is_QLinkedList(JNIEnv *env, jobject collection, const std::type_info& expectedElementType, const QMetaType& expectedElementMetaType);
#endif
QTJAMBI_EXPORT bool qtjambi_is_QList(JNIEnv *env, jobject collection, const std::type_info& expectedElementType, const QMetaType& expectedElementMetaType);
QTJAMBI_EXPORT bool qtjambi_is_QSet(JNIEnv *env, jobject collection, const std::type_info& expectedElementType, const QMetaType& expectedElementMetaType);
QTJAMBI_EXPORT bool qtjambi_is_QQueue(JNIEnv *env, jobject collection, const std::type_info& expectedElementType, const QMetaType& expectedElementMetaType);
QTJAMBI_EXPORT bool qtjambi_is_QMultiMap(JNIEnv *env, jobject mapObject, const std::type_info& expectedKeyType, const QMetaType& expectedKeyMetaType, const std::type_info& expectedValueType, const QMetaType& expectedValueMetaType);
QTJAMBI_EXPORT bool qtjambi_is_QMultiHash(JNIEnv *env, jobject mapObject, const std::type_info& expectedKeyType, const QMetaType& expectedKeyMetaType, const std::type_info& expectedValueType, const QMetaType& expectedValueMetaType);
QTJAMBI_EXPORT bool qtjambi_is_QMap(JNIEnv *env, jobject mapObject, const std::type_info& expectedKeyType, const QMetaType& expectedKeyMetaType, const std::type_info& expectedValueType, const QMetaType& expectedValueMetaType);
QTJAMBI_EXPORT bool qtjambi_is_QHash(JNIEnv *env, jobject mapObject, const std::type_info& expectedKeyType, const QMetaType& expectedKeyMetaType, const std::type_info& expectedValueType, const QMetaType& expectedValueMetaType);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
template<typename T>
bool qtjambi_is_QVector(JNIEnv *env, jobject collection){
    return qtjambi_is_QVector(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T));
}

template<typename T>
bool qtjambi_is_QLinkedList(JNIEnv *env, jobject collection){
    return qtjambi_is_QLinkedList(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T));
}
#endif

template<typename T>
bool qtjambi_is_QStack(JNIEnv *env, jobject collection){
    return qtjambi_is_QStack(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T));
}

template<typename T>
bool qtjambi_is_QList(JNIEnv *env, jobject collection){
    return qtjambi_is_QList(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T));
}

template<typename T>
bool qtjambi_is_QSet(JNIEnv *env, jobject collection){
    return qtjambi_is_QSet(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T));
}

template<typename T>
bool qtjambi_is_QQueue(JNIEnv *env, jobject collection){
    return qtjambi_is_QQueue(env, collection, QtJambiPrivate::qtjambi_type<T>::id(), QTJAMBI_METATYPE_FROM_TYPE2(T));
}

template<typename K, typename V>
bool qtjambi_is_QMultiMap(JNIEnv *env, jobject mapObject){
    return qtjambi_is_QMultiMap(env, mapObject, QtJambiPrivate::qtjambi_type<K>::id(), QTJAMBI_METATYPE_FROM_TYPE2(K), QtJambiPrivate::qtjambi_type<V>::id(), QTJAMBI_METATYPE_FROM_TYPE2(V));
}

template<typename K, typename V>
bool qtjambi_is_QMultiHash(JNIEnv *env, jobject mapObject){
    return qtjambi_is_QMultiHash(env, mapObject, QtJambiPrivate::qtjambi_type<K>::id(), QTJAMBI_METATYPE_FROM_TYPE2(K), QtJambiPrivate::qtjambi_type<V>::id(), QTJAMBI_METATYPE_FROM_TYPE2(V));
}

template<typename K, typename V>
bool qtjambi_is_QMap(JNIEnv *env, jobject mapObject){
    return qtjambi_is_QMap(env, mapObject, QtJambiPrivate::qtjambi_type<K>::id(), QTJAMBI_METATYPE_FROM_TYPE2(K), QtJambiPrivate::qtjambi_type<V>::id(), QTJAMBI_METATYPE_FROM_TYPE2(V));
}

template<typename K, typename V>
bool qtjambi_is_QHash(JNIEnv *env, jobject mapObject){
    return qtjambi_is_QHash(env, mapObject, QtJambiPrivate::qtjambi_type<K>::id(), QTJAMBI_METATYPE_FROM_TYPE2(K), QtJambiPrivate::qtjambi_type<V>::id(), QTJAMBI_METATYPE_FROM_TYPE2(V));
}

typedef void(* PointerDeleter)(void *, bool);
typedef void*(*PointerGetter)(const void *);

enum class ListType{
    QList, QQueue
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    , QStack
#endif
};

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
enum class VectorType{
    QVector, QStack
};
#endif

class QTJAMBI_EXPORT AbstractContainerAccess{
public:
    virtual void dispose();
    virtual AbstractContainerAccess* clone() = 0;
    virtual void* createContainer() = 0;
    virtual void* copyContainer(const void* container) = 0;
    virtual void assign(void* container, const void* other) = 0;
    virtual void deleteContainer(void* container) = 0;
    virtual int registerContainer(const QByteArray& containerTypeName) = 0;
    virtual const QObject* getOwner(const void* container);
    virtual PtrDeleterFunction containerDeleter() = 0;
    Q_DISABLE_COPY_MOVE(AbstractContainerAccess)
protected:
    AbstractContainerAccess();
    virtual ~AbstractContainerAccess();
};

class QTJAMBI_EXPORT AbstractIteratorAccess : public AbstractContainerAccess{
protected:
    ~AbstractIteratorAccess() override = default;
    AbstractIteratorAccess() = default;
public:
    AbstractIteratorAccess* clone() override = 0;
    virtual jobject value(JNIEnv * env, void* iterator) = 0;
    virtual void increment(JNIEnv * env, void* iterator) = 0;
    virtual void decrement(JNIEnv * env, void* iterator) = 0;
    virtual jboolean lessThan(JNIEnv * env, void* iterator, void* other) = 0;
    virtual bool canLess() = 0;
    virtual jboolean equals(JNIEnv * env, void* iterator, void* other) = 0;
private:
    void* createContainer() override {return nullptr;}
    void* copyContainer(const void*) override {return nullptr;}
    void assign(void*, const void*) override {}
    void deleteContainer(void*) override {}
    int registerContainer(const QByteArray&) override {return QMetaType::UnknownType;}
    PtrDeleterFunction containerDeleter() override {return nullptr;}
    Q_DISABLE_COPY_MOVE(AbstractIteratorAccess)
};

class QTJAMBI_EXPORT AbstractBiIteratorAccess : public AbstractIteratorAccess{
protected:
    ~AbstractBiIteratorAccess() override = default;
    AbstractBiIteratorAccess() = default;
public:
    AbstractBiIteratorAccess* clone() override = 0;
    virtual jobject key(JNIEnv * env, void* iterator) = 0;
private:
    void* createContainer() override {return nullptr;}
    void* copyContainer(const void*) override {return nullptr;}
    void assign(void*, const void*) override {}
    void deleteContainer(void*) override {}
    int registerContainer(const QByteArray&) override {
        return QMetaType::UnknownType;
    }
    PtrDeleterFunction containerDeleter() override {return nullptr;}
    Q_DISABLE_COPY_MOVE(AbstractBiIteratorAccess)
};

typedef bool (*ElementAnalyzer)(const void* element, void* data);
typedef bool (*EntryAnalyzer)(const void* key, const void* value, void* data);

class QTJAMBI_EXPORT AbstractListAccess : public AbstractContainerAccess{
protected:
    ~AbstractListAccess() override = default;
    AbstractListAccess() = default;
public:
    AbstractListAccess* clone() override = 0;
    virtual void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) = 0;
    virtual bool isConstant() = 0;
    virtual const QMetaType& elementMetaType() = 0;
    virtual void append(JNIEnv * env, void* container, jobject value) = 0;
    virtual void appendList(JNIEnv * env, void* container, jobject list) = 0;
    virtual jobject at(JNIEnv * env, const void* container, jint index) = 0;
    virtual jobject value(JNIEnv * env, const void* container, jint index) = 0;
    virtual jobject value(JNIEnv * env, const void* container, jint index, jobject defaultValue) = 0;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    virtual jobject toSet(JNIEnv * env,const void* container) = 0;
#endif
    virtual jobject takeAt(JNIEnv * env, void* container, jint index) = 0;
    virtual void swapItemsAt(JNIEnv * env, void* container, jint index1, jint index2) = 0;
    virtual jboolean startsWith(JNIEnv * env, const void* container, jobject value) = 0;
    virtual jint size(JNIEnv * env, const void* container) = 0;
    virtual void reserve(JNIEnv * env, void* container, jint size) = 0;
    virtual void replace(JNIEnv * env, void* container, jint index, jobject value) = 0;
    virtual jboolean removeOne(JNIEnv * env, void* container, jobject value) = 0;
    virtual void removeAt(JNIEnv * env, void* container, jint index) = 0;
    virtual jint removeAll(JNIEnv * env, void* container, jobject value) = 0;
    virtual void prepend(JNIEnv * env, void* container, jobject value) = 0;
    virtual jboolean equal(JNIEnv * env, const void* container, jobject other) = 0;
    virtual void move(JNIEnv * env, void* container, jint index1, jint index2) = 0;
    virtual jobject mid(JNIEnv * env, const void* container, jint index1, jint index2) = 0;
    virtual jint lastIndexOf(JNIEnv * env, const void* container, jobject value, jint index) = 0;
    virtual void insert(JNIEnv * env, void* container, jint index, jobject value) = 0;
    virtual jint indexOf(JNIEnv * env, const void* container, jobject value, jint index) = 0;
    virtual jboolean endsWith(JNIEnv * env, const void* container, jobject value) = 0;
    virtual jobject end(JNIEnv * env, QtJambiNativeID ownerId, const void* container) = 0;
    virtual jint count(JNIEnv * env, const void* container, jobject value) = 0;
    virtual jboolean contains(JNIEnv * env, const void* container, jobject value) = 0;
    virtual void clear(JNIEnv * env, void* container) = 0;
    virtual jobject begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) = 0;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    virtual jint capacity(JNIEnv * env, const void* container) = 0;
    virtual void fill(JNIEnv * env, void* container, jobject value, jint size) = 0;
    virtual void remove(JNIEnv * env, void* container, jint index, jint n) = 0;
    virtual void insert(JNIEnv * env, void* container, jint index, jint n, jobject value) = 0;
    virtual void resize(JNIEnv * env, void* container, jint newSize) = 0;
    virtual void squeeze(JNIEnv * env, void* container) = 0;
#endif
    Q_DISABLE_COPY_MOVE(AbstractListAccess)
};

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
class QTJAMBI_EXPORT AbstractVectorAccess : public AbstractContainerAccess{
protected:
    ~AbstractVectorAccess() override = default;
    AbstractVectorAccess() = default;
public:
    AbstractVectorAccess* clone() override = 0;
    virtual void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) = 0;
    virtual bool isConstant() = 0;
    virtual const QMetaType& elementMetaType() = 0;
    virtual void append(JNIEnv * env, void* container, jobject value) = 0;
    virtual void appendVector(JNIEnv * env, void* container, jobject list) = 0;
    virtual jobject at(JNIEnv * env, const void* container, jint index) = 0;
    virtual jobject value(JNIEnv * env, const void* container, jint index) = 0;
    virtual jobject value(JNIEnv * env, const void* container, jint index, jobject defaultValue) = 0;
    virtual jobject toSet(JNIEnv * env,const void* container) = 0;
    virtual jobject takeAt(JNIEnv * env, void* container, jint index) = 0;
    virtual void swapItemsAt(JNIEnv * env, void* container, jint index1, jint index2) = 0;
    virtual jboolean startsWith(JNIEnv * env, const void* container, jobject value) = 0;
    virtual jint size(JNIEnv * env, const void* container) = 0;
    virtual void reserve(JNIEnv * env, void* container, jint size) = 0;
    virtual void replace(JNIEnv * env, void* container, jint index, jobject value) = 0;
    virtual jboolean removeOne(JNIEnv * env, void* container, jobject value) = 0;
    virtual void removeAt(JNIEnv * env, void* container, jint index) = 0;
    virtual jint removeAll(JNIEnv * env, void* container, jobject value) = 0;
    virtual void prepend(JNIEnv * env, void* container, jobject value) = 0;
    virtual jboolean equal(JNIEnv * env, const void* container, jobject other) = 0;
    virtual void move(JNIEnv * env, void* container, jint index1, jint index2) = 0;
    virtual jobject mid(JNIEnv * env, const void* container, jint index1, jint index2) = 0;
    virtual jint lastIndexOf(JNIEnv * env, const void* container, jobject value, jint index) = 0;
    virtual void insert(JNIEnv * env, void* container, jint index, jobject value) = 0;
    virtual jint indexOf(JNIEnv * env, const void* container, jobject value, jint index) = 0;
    virtual jboolean endsWith(JNIEnv * env, const void* container, jobject value) = 0;
    virtual jobject end(JNIEnv * env, QtJambiNativeID ownerId, const void* container) = 0;
    virtual jint count(JNIEnv * env, const void* container, jobject value) = 0;
    virtual jboolean contains(JNIEnv * env, const void* container, jobject value) = 0;
    virtual void clear(JNIEnv * env, void* container) = 0;
    virtual jobject begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) = 0;
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
    ~AbstractLinkedListAccess() override = default;
    AbstractLinkedListAccess() = default;
public:
    AbstractLinkedListAccess* clone() override = 0;
    virtual void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) = 0;
    virtual bool isConstant() = 0;
    virtual const QMetaType& elementMetaType() = 0;
    virtual void append(JNIEnv * env, void* container, jobject value) = 0;
    virtual jobject first(JNIEnv * env, const void* container) = 0;
    virtual jobject last(JNIEnv * env, const void* container) = 0;
    virtual jobject begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) = 0;
    virtual void clear(JNIEnv * env, void* container) = 0;
    virtual jboolean contains(JNIEnv * env, const void* container, jobject value) = 0;
    virtual jint count(JNIEnv * env, const void* container, jobject value) = 0;
    virtual jobject end(JNIEnv * env, QtJambiNativeID ownerId, const void* container) = 0;
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
    Q_DISABLE_COPY_MOVE(AbstractLinkedListAccess)
};

#endif

class QTJAMBI_EXPORT AbstractSetAccess : public AbstractContainerAccess{
protected:
    ~AbstractSetAccess() override = default;
    AbstractSetAccess() = default;
public:
    AbstractSetAccess* clone() override = 0;
    virtual void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) = 0;
    virtual bool isConstant() = 0;
    virtual const QMetaType& elementMetaType() = 0;
    virtual jobject begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) = 0;
    virtual jint capacity(JNIEnv * env, const void* container) = 0;
    virtual void clear(JNIEnv * env, void* container) = 0;
    virtual jboolean contains(JNIEnv * env, const void* container, jobject value) = 0;
    virtual jobject end(JNIEnv * env, QtJambiNativeID ownerId, const void* container) = 0;
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
    Q_DISABLE_COPY_MOVE(AbstractSetAccess)
};

class QTJAMBI_EXPORT AbstractMapAccess : public AbstractContainerAccess{
protected:
    ~AbstractMapAccess() override = default;
    AbstractMapAccess() = default;
public:
    AbstractMapAccess* clone() override = 0;
    virtual void analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) = 0;
    virtual bool isConstant() = 0;
    virtual const QMetaType& keyMetaType() = 0;
    virtual const QMetaType& valueMetaType() = 0;
    virtual jobject begin(JNIEnv *,QtJambiNativeID,const void*) = 0;
    virtual void clear(JNIEnv *,void*) = 0;
    virtual jboolean contains(JNIEnv *,const void*,jobject) = 0;
    virtual jint count(JNIEnv *,const void*,jobject) = 0;
    virtual jobject end(JNIEnv *,QtJambiNativeID,const void*) = 0;
    virtual jobject find(JNIEnv *,QtJambiNativeID,const void*,jobject) = 0;
    virtual jobject first(JNIEnv *,const void*) = 0;
    virtual jobject firstKey(JNIEnv *,const void*) = 0;
    virtual void insert(JNIEnv *,void*,jobject,jobject) = 0;
    virtual jobject key(JNIEnv *,const void*,jobject,jobject) = 0;
    virtual jobject keys(JNIEnv *,const void*) = 0;
    virtual jobject keys(JNIEnv *,const void*,jobject) = 0;
    virtual jobject last(JNIEnv *,const void*) = 0;
    virtual jobject lastKey(JNIEnv *,const void*) = 0;
    virtual jobject lowerBound(JNIEnv *,QtJambiNativeID,const void*,jobject) = 0;
    virtual jboolean equal(JNIEnv *,const void*,jobject) = 0;
    virtual jint remove(JNIEnv *,void*,jobject) = 0;
    virtual jint size(JNIEnv *,const void*) = 0;
    virtual jobject take(JNIEnv *,void*,jobject) = 0;
    virtual jobject upperBound(JNIEnv *,QtJambiNativeID,const void*,jobject) = 0;
    virtual jobject value(JNIEnv *,const void*,jobject,jobject) = 0;
    virtual jobject values(JNIEnv *,const void*) = 0;
    virtual bool keyLessThan(JNIEnv *,jobject,jobject) = 0;
};

class QTJAMBI_EXPORT AbstractMultiMapAccess : public AbstractMapAccess{
protected:
    ~AbstractMultiMapAccess() override = default;
    AbstractMultiMapAccess() = default;
public:
    AbstractMultiMapAccess* clone() override = 0;
    bool isConstant() override = 0;
    using AbstractMapAccess::contains;
    using AbstractMapAccess::count;
    using AbstractMapAccess::find;
    using AbstractMapAccess::remove;
    using AbstractMapAccess::values;
    virtual jobject uniqueKeys(JNIEnv *,const void*) = 0;
    virtual void unite(JNIEnv *,void*,jobject) = 0;
    virtual jobject values(JNIEnv *,const void*,jobject) = 0;
    virtual jboolean contains(JNIEnv *,const void*,jobject,jobject) = 0;
    virtual jint count(JNIEnv *,const void*,jobject,jobject) = 0;
    virtual jobject find(JNIEnv *,QtJambiNativeID,const void*,jobject,jobject) = 0;
    virtual jint remove(JNIEnv *,void*,jobject,jobject) = 0;
    virtual void replace(JNIEnv *,void*,jobject,jobject) = 0;
};

class QTJAMBI_EXPORT AbstractHashAccess : public AbstractContainerAccess{
protected:
    ~AbstractHashAccess() override = default;
    AbstractHashAccess() = default;
public:
    AbstractHashAccess* clone() override = 0;
    virtual void analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) = 0;
    virtual bool isConstant() = 0;
    virtual const QMetaType& keyMetaType() = 0;
    virtual const QMetaType& valueMetaType() = 0;
    virtual jobject begin(JNIEnv *,QtJambiNativeID,const void*) = 0;
    virtual jint capacity(JNIEnv *,const void*) = 0;
    virtual void clear(JNIEnv *,void*) = 0;
    virtual jboolean contains(JNIEnv *,const void*,jobject) = 0;
    virtual jint count(JNIEnv *,const void*,jobject) = 0;
    virtual jobject end(JNIEnv *,QtJambiNativeID,const void*) = 0;
    virtual jobject find(JNIEnv *,QtJambiNativeID,const void*,jobject) = 0;
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
};

class QTJAMBI_EXPORT AbstractMultiHashAccess : public AbstractHashAccess{
protected:
    ~AbstractMultiHashAccess() override = default;
    AbstractMultiHashAccess() = default;
public:
    AbstractMultiHashAccess* clone() override = 0;
    bool isConstant() override = 0;
    using AbstractHashAccess::contains;
    using AbstractHashAccess::count;
    using AbstractHashAccess::find;
    using AbstractHashAccess::remove;
    using AbstractHashAccess::values;
    virtual jobject uniqueKeys(JNIEnv *,const void*) = 0;
    virtual void unite(JNIEnv *,void*,jobject) = 0;
    virtual jobject values(JNIEnv *,const void*,jobject) = 0;
    virtual jboolean contains(JNIEnv *,const void*,jobject,jobject) = 0;
    virtual jint count(JNIEnv *,const void*,jobject,jobject) = 0;
    virtual jobject find(JNIEnv *,QtJambiNativeID,const void*,jobject,jobject) = 0;
    virtual jint remove(JNIEnv *,void*,jobject,jobject) = 0;
    virtual void replace(JNIEnv *,void*,jobject,jobject) = 0;
};

class QTJAMBI_EXPORT AbstractPairAccess : public AbstractContainerAccess{
protected:
    ~AbstractPairAccess() override = default;
    AbstractPairAccess() = default;
public:
    AbstractPairAccess* clone() override = 0;
    virtual bool isConstant() = 0;
    virtual jobject first(JNIEnv *,const void*) = 0;
    virtual jobject second(JNIEnv *,const void*) = 0;
    virtual void setFirst(JNIEnv *,void*,jobject) = 0;
    virtual void setSecond(JNIEnv *,void*,jobject) = 0;
};

QTJAMBI_EXPORT QPair<void*,AbstractContainerAccess*> qtjambi_container_from_nativeId(QtJambiNativeID nativeId);

QTJAMBI_EXPORT void qtjambi_initialize_native_container(JNIEnv *env, jclass callingClass, jobject object, ConstructorFunction constructorFunction,
                                                     size_t size, const std::type_info& typeId, bool isShell,
                                                     AbstractContainerAccess* containerAccess,
                                                     jvalue* arguments);

QTJAMBI_EXPORT jobject qtjambi_from_QIterator(JNIEnv *env,
                           QtJambiNativeID owner,
                           void* iteratorPtr,
                           PtrDeleterFunction destructor_function,
                           AbstractIteratorAccess* access);

QTJAMBI_EXPORT jobject qtjambi_from_QMapIterator(JNIEnv *env,
                           QtJambiNativeID owner,
                           void* iteratorPtr,
                           PtrDeleterFunction destructor_function,
                           AbstractBiIteratorAccess* access);

QTJAMBI_EXPORT jobject qtjambi_from_QList(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     CopyFunction copyFunction,
                                     PtrDeleterFunction deleter,
                                     ListType listType,
                                     AbstractListAccess* listAccess
                                );

QTJAMBI_EXPORT jobject qtjambi_from_QList(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     ListType listType,
                                     AbstractListAccess* listAccess
                                );

QTJAMBI_EXPORT void initialize_QList(JNIEnv *env, jobject object, jclass elementType, QtJambiNativeID elementMetaType, jobject other);

QTJAMBI_EXPORT jobject qtjambi_from_QStringList(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     CopyFunction copyFunction,
                                     PtrDeleterFunction deleter,
                                     bool isConstant
                                );

QTJAMBI_EXPORT jobject qtjambi_from_QStringList(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     bool isConstant
                                );

QTJAMBI_EXPORT jobject qtjambi_from_QSet(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     CopyFunction copyFunction,
                                     PtrDeleterFunction deleter,
                                     AbstractSetAccess* setAccess
                                );

QTJAMBI_EXPORT jobject qtjambi_from_QSet(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     AbstractSetAccess* setAccess
                                );

QTJAMBI_EXPORT void initialize_QSet(JNIEnv *env, jobject object, jclass elementType, QtJambiNativeID elementMetaType, jobject other);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QTJAMBI_EXPORT jobject qtjambi_from_QLinkedList(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     CopyFunction copyFunction,
                                     PtrDeleterFunction deleter,
                                     AbstractLinkedListAccess* linkedListAccess
                                );

QTJAMBI_EXPORT jobject qtjambi_from_QLinkedList(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     AbstractLinkedListAccess* linkedListAccess
                                );

QTJAMBI_EXPORT void initialize_QLinkedList(JNIEnv *env, jobject object, jclass elementType, QtJambiNativeID elementMetaType, jobject other);

QTJAMBI_EXPORT jobject qtjambi_from_QVector(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     CopyFunction copyFunction,
                                     PtrDeleterFunction deleter,
                                     VectorType vectorType,
                                     AbstractVectorAccess* vectorAccess
                                );

QTJAMBI_EXPORT jobject qtjambi_from_QVector(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     VectorType vectorType,
                                     AbstractVectorAccess* vectorAccess
                                );

QTJAMBI_EXPORT void initialize_QVector(JNIEnv *env, jobject object, jclass elementType, QtJambiNativeID elementMetaType, jobject other);
#endif


QTJAMBI_EXPORT jobject qtjambi_from_QHash(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     CopyFunction copyFunction,
                                     PtrDeleterFunction deleter,
                                     AbstractHashAccess* hashAccess
                                );

QTJAMBI_EXPORT jobject qtjambi_from_QHash(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     AbstractHashAccess* hashAccess
                                );

QTJAMBI_EXPORT void initialize_QHash(JNIEnv *env, jobject object, jclass keyType, QtJambiNativeID keyMetaType, jclass valueType, QtJambiNativeID valueMetaType, jobject other);

QTJAMBI_EXPORT jobject qtjambi_from_QMultiHash(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     CopyFunction copyFunction,
                                     PtrDeleterFunction deleter,
                                     AbstractMultiHashAccess* multiHashAccess
                                );

QTJAMBI_EXPORT jobject qtjambi_from_QMultiHash(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     AbstractMultiHashAccess* multiHashAccess
                                );

QTJAMBI_EXPORT void initialize_QMultiHash(JNIEnv *env, jobject object, jclass keyType, QtJambiNativeID keyMetaType, jclass valueType, QtJambiNativeID valueMetaType, jobject other);

QTJAMBI_EXPORT jobject qtjambi_from_QMap(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     CopyFunction copyFunction,
                                     PtrDeleterFunction deleter,
                                     AbstractMapAccess* mapAccess
                                );

QTJAMBI_EXPORT jobject qtjambi_from_QMap(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     AbstractMapAccess* mapAccess
                                );

QTJAMBI_EXPORT void initialize_QMap(JNIEnv *env, jobject object, jclass keyType, QtJambiNativeID keyMetaType, jclass valueType, QtJambiNativeID valueMetaType, jobject other);

QTJAMBI_EXPORT jobject qtjambi_from_QMultiMap(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     CopyFunction copyFunction,
                                     PtrDeleterFunction deleter,
                                     AbstractMultiMapAccess* mapAccess
                                );


QTJAMBI_EXPORT jobject qtjambi_from_QMultiMap(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     AbstractMultiMapAccess* mapAccess
                                );

QTJAMBI_EXPORT void initialize_QMultiMap(JNIEnv *env, jobject object, jclass keyType, QtJambiNativeID keyMetaType, jclass valueType, QtJambiNativeID valueMetaType, jobject other);

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
QTJAMBI_EXPORT bool qtjambi_is_static_type(const QMetaType&);
#endif

#endif // QTJAMBI_CONTAINERS_H

