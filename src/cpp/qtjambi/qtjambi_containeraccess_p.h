#ifndef QTJAMBI_CONTAINERACCESS_P_H
#define QTJAMBI_CONTAINERACCESS_P_H

#include "qtjambi_containeraccess.h"
#include "qtjambi_jobjectwrapper.h"

void qtjambi_register_container_converter(ContainerType collectionType, const QMetaType& containerMetaType, const QMetaType& elementMetaType);
void qtjambi_register_container_converter(MapType mapType, const QMetaType& containerMetaType, const QMetaType& keyMetaType, const QMetaType& valueMetaType);
void qtjambi_register_container_converter(QSharedPointer<AbstractPairAccess> pairAccess, const QMetaType& containerMetaType);

AbstractListAccess* checkContainerAccess(JNIEnv * env, AbstractListAccess* containerAccess);
AbstractSetAccess* checkContainerAccess(JNIEnv * env, AbstractSetAccess* containerAccess);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
AbstractLinkedListAccess* checkContainerAccess(JNIEnv * env, AbstractLinkedListAccess* containerAccess);
AbstractVectorAccess* checkContainerAccess(JNIEnv * env, AbstractVectorAccess* containerAccess);
#endif
AbstractHashAccess* checkContainerAccess(JNIEnv * env, AbstractHashAccess* containerAccess);
AbstractMapAccess* checkContainerAccess(JNIEnv * env, AbstractMapAccess* containerAccess);
AbstractMultiHashAccess* checkContainerAccess(JNIEnv * env, AbstractMultiHashAccess* containerAccess);
AbstractMultiMapAccess* checkContainerAccess(JNIEnv * env, AbstractMultiMapAccess* containerAccess);
void containerDisposer(AbstractContainerAccess* _access);

class WrapperListAccess : public AbstractListAccess{
public:
    inline WrapperListAccess(AbstractListAccess* containerAccess)
        : AbstractListAccess(), m_containerAccess(containerAccess) {}

    inline ~WrapperListAccess() override{
        m_containerAccess->dispose();
        m_containerAccess = nullptr;
    }

    inline AbstractListAccess* clone() override{
        return m_containerAccess->clone();
    }

    inline void dispose() override { delete this; }

    inline void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) override{
        m_containerAccess->analyzeElements(container, analyzer, data);
    }

    inline void* createContainer() override{
        return m_containerAccess->createContainer();
    }

    inline void* copyContainer(const void* container) override{
        return m_containerAccess->copyContainer(container);
    }

    inline void assign(void* container, const void* other) override{
        m_containerAccess->assign(container, other);
    }

    inline void deleteContainer(void* container) override{
        m_containerAccess->deleteContainer(container);
    }

    inline int registerContainer(const QByteArray& containerTypeName) override{
        return m_containerAccess->registerContainer(containerTypeName);
    }

    inline PtrDeleterFunction containerDeleter() override{
        return m_containerAccess->containerDeleter();
    }

    inline bool isConstant() override{
        return m_containerAccess->isConstant();
    }

    inline const QMetaType& elementMetaType() override{
        return m_containerAccess->elementMetaType();
    }

    inline void append(JNIEnv * env, void* container, jobject value) override{
        m_containerAccess->append(env, container, value);
    }

    inline void appendList(JNIEnv * env, void* container, jobject list) override{
        m_containerAccess->appendList(env, container, list);
    }

    inline jobject at(JNIEnv * env, const void* container, jint index) override{
        return m_containerAccess->at(env, container, index);
    }

    inline jobject value(JNIEnv * env, const void* container, jint index) override{
        return m_containerAccess->value(env, container, index);
    }

    inline jobject value(JNIEnv * env, const void* container, jint index, jobject defaultValue) override{
        return m_containerAccess->value(env, container, index, defaultValue);
    }

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    inline jobject toSet(JNIEnv * env,const void* container) override{
        return m_containerAccess->toSet(env, container);
    }
#endif

    inline jobject takeAt(JNIEnv * env, void* container, jint index) override{
        return m_containerAccess->takeAt(env, container, index);
    }

    inline void swapItemsAt(JNIEnv * env, void* container, jint index1, jint index2) override{
        m_containerAccess->swapItemsAt(env, container, index1, index2);
    }

    inline jboolean startsWith(JNIEnv * env, const void* container, jobject value) override{
        return m_containerAccess->startsWith(env, container, value);
    }

    inline jint size(JNIEnv * env, const void* container) override{
        return m_containerAccess->size(env, container);
    }

    inline void reserve(JNIEnv * env, void* container, jint size) override{
        return m_containerAccess->reserve(env, container, size);
    }

    inline void replace(JNIEnv * env, void* container, jint index, jobject value) override{
        m_containerAccess->replace(env, container, index, value);
    }

    inline jboolean removeOne(JNIEnv * env, void* container, jobject value) override{
        return m_containerAccess->removeOne(env, container, value);
    }

    inline void removeAt(JNIEnv * env, void* container, jint index) override{
        m_containerAccess->removeAt(env, container, index);
    }

    inline jint removeAll(JNIEnv * env, void* container, jobject value) override{
        return m_containerAccess->removeAll(env, container, value);
    }

    inline void prepend(JNIEnv * env, void* container, jobject value) override{
        m_containerAccess->prepend(env, container, value);
    }

    inline jboolean equal(JNIEnv * env, const void* container, jobject other) override{
        return m_containerAccess->equal(env, container, other);
    }

    inline void move(JNIEnv * env, void* container, jint index1, jint index2) override{
        m_containerAccess->move(env, container, index1, index2);
    }

    inline jobject mid(JNIEnv * env, const void* container, jint index1, jint index2) override{
        return m_containerAccess->mid(env, container, index1, index2);
    }

    inline jint lastIndexOf(JNIEnv * env, const void* container, jobject value, jint index) override{
        return m_containerAccess->lastIndexOf(env, container, value, index);
    }

    inline void insert(JNIEnv * env, void* container, jint index, jobject value) override{
        m_containerAccess->insert(env, container, index, value);
    }

    inline jint indexOf(JNIEnv * env, const void* container, jobject value, jint index) override{
        return m_containerAccess->indexOf(env, container, value, index);
    }

    inline jboolean endsWith(JNIEnv * env, const void* container, jobject value) override{
        return m_containerAccess->endsWith(env, container, value);
    }

    inline jobject end(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override{
        return m_containerAccess->end(env, ownerId, container);
    }

    inline jint count(JNIEnv * env, const void* container, jobject value) override{
        return m_containerAccess->count(env, container, value);
    }

    inline jboolean contains(JNIEnv * env, const void* container, jobject value) override{
        return m_containerAccess->contains(env, container, value);
    }

    inline void clear(JNIEnv * env, void* container) override{
        m_containerAccess->clear(env, container);
    }

    inline jobject begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override{
        return m_containerAccess->begin(env, ownerId, container);
    }

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    inline jint capacity(JNIEnv * env, const void* container) override{
        return m_containerAccess->capacity(env, container);
    }

    inline void fill(JNIEnv * env, void* container, jobject value, jint size) override{
        m_containerAccess->fill(env, container, value, size);
    }

    inline void remove(JNIEnv * env, void* container, jint index, jint n) override{
        m_containerAccess->remove(env, container, index, n);
    }

    inline void insert(JNIEnv * env, void* container, jint index, jint n, jobject value) override{
        m_containerAccess->insert(env, container, index, n, value);
    }

    inline void resize(JNIEnv * env, void* container, jint newSize) override{
        return m_containerAccess->resize(env, container, newSize);
    }

    inline void squeeze(JNIEnv * env, void* container) override{
        return m_containerAccess->squeeze(env, container);
    }
#endif
    Q_DISABLE_COPY_MOVE(WrapperListAccess)
private:
    AbstractListAccess* m_containerAccess;
};

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
class WrapperVectorAccess : public AbstractVectorAccess{
public:
    inline WrapperVectorAccess(AbstractVectorAccess* containerAccess)
        : AbstractVectorAccess(), m_containerAccess(containerAccess) {}

    inline ~WrapperVectorAccess() override{
        m_containerAccess->dispose();
        m_containerAccess = nullptr;
    }

    inline AbstractVectorAccess* clone() override{
        return m_containerAccess->clone();
    }

    inline void dispose() override { delete this; }

    inline void* createContainer() override{
        return m_containerAccess->createContainer();
    }

    inline void* copyContainer(const void* container) override{
        return m_containerAccess->copyContainer(container);
    }

    inline void assign(void* container, const void* other) override{
        m_containerAccess->assign(container, other);
    }

    inline void deleteContainer(void* container) override{
        m_containerAccess->deleteContainer(container);
    }

    inline int registerContainer(const QByteArray& containerTypeName) override{
        return m_containerAccess->registerContainer(containerTypeName);
    }

    inline PtrDeleterFunction containerDeleter() override{
        return m_containerAccess->containerDeleter();
    }

    inline bool isConstant() override{
        return m_containerAccess->isConstant();
    }

    inline const QMetaType& elementMetaType() override{
        return m_containerAccess->elementMetaType();
    }

    inline void append(JNIEnv * env, void* container, jobject value) override{
        m_containerAccess->append(env, container, value);
    }

    inline void appendVector(JNIEnv * env, void* container, jobject list) override{
        m_containerAccess->appendVector(env, container, list);
    }

    inline jobject at(JNIEnv * env, const void* container, jint index) override{
        return m_containerAccess->at(env, container, index);
    }

    inline jobject value(JNIEnv * env, const void* container, jint index) override{
        return m_containerAccess->value(env, container, index);
    }

    inline jobject value(JNIEnv * env, const void* container, jint index, jobject defaultValue) override{
        return m_containerAccess->value(env, container, index, defaultValue);
    }

    inline jobject toSet(JNIEnv * env,const void* container) override{
        return m_containerAccess->toSet(env, container);
    }

    inline jobject takeAt(JNIEnv * env, void* container, jint index) override{
        return m_containerAccess->takeAt(env, container, index);
    }

    inline void swapItemsAt(JNIEnv * env, void* container, jint index1, jint index2) override{
        m_containerAccess->swapItemsAt(env, container, index1, index2);
    }

    inline jboolean startsWith(JNIEnv * env, const void* container, jobject value) override{
        return m_containerAccess->startsWith(env, container, value);
    }

    inline jint size(JNIEnv * env, const void* container) override{
        return m_containerAccess->size(env, container);
    }

    inline void reserve(JNIEnv * env, void* container, jint size) override{
        return m_containerAccess->reserve(env, container, size);
    }

    inline void replace(JNIEnv * env, void* container, jint index, jobject value) override{
        m_containerAccess->replace(env, container, index, value);
    }

    inline jboolean removeOne(JNIEnv * env, void* container, jobject value) override{
        return m_containerAccess->removeOne(env, container, value);
    }

    inline void removeAt(JNIEnv * env, void* container, jint index) override{
        m_containerAccess->removeAt(env, container, index);
    }

    inline jint removeAll(JNIEnv * env, void* container, jobject value) override{
        return m_containerAccess->removeAll(env, container, value);
    }

    inline void prepend(JNIEnv * env, void* container, jobject value) override{
        m_containerAccess->prepend(env, container, value);
    }

    inline jboolean equal(JNIEnv * env, const void* container, jobject other) override{
        return m_containerAccess->equal(env, container, other);
    }

    inline void move(JNIEnv * env, void* container, jint index1, jint index2) override{
        m_containerAccess->move(env, container, index1, index2);
    }

    inline jobject mid(JNIEnv * env, const void* container, jint index1, jint index2) override{
        return m_containerAccess->mid(env, container, index1, index2);
    }

    inline jint lastIndexOf(JNIEnv * env, const void* container, jobject value, jint index) override{
        return m_containerAccess->lastIndexOf(env, container, value, index);
    }

    inline void insert(JNIEnv * env, void* container, jint index, jobject value) override{
        m_containerAccess->insert(env, container, index, value);
    }

    inline jint indexOf(JNIEnv * env, const void* container, jobject value, jint index) override{
        return m_containerAccess->indexOf(env, container, value, index);
    }

    inline jboolean endsWith(JNIEnv * env, const void* container, jobject value) override{
        return m_containerAccess->endsWith(env, container, value);
    }

    inline jobject end(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override{
        return m_containerAccess->end(env, ownerId, container);
    }

    inline jint count(JNIEnv * env, const void* container, jobject value) override{
        return m_containerAccess->count(env, container, value);
    }

    inline jboolean contains(JNIEnv * env, const void* container, jobject value) override{
        return m_containerAccess->contains(env, container, value);
    }

    inline void clear(JNIEnv * env, void* container) override{
        m_containerAccess->clear(env, container);
    }

    inline jobject begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override{
        return m_containerAccess->begin(env, ownerId, container);
    }

    inline jint capacity(JNIEnv * env, const void* container) override{
        return m_containerAccess->capacity(env, container);
    }

    inline void fill(JNIEnv * env, void* container, jobject value, jint size) override{
        m_containerAccess->fill(env, container, value, size);
    }

    inline void remove(JNIEnv * env, void* container, jint index, jint n) override{
        m_containerAccess->remove(env, container, index, n);
    }

    inline void insert(JNIEnv * env, void* container, jint index, jint n, jobject value) override{
        m_containerAccess->insert(env, container, index, n, value);
    }

    inline void resize(JNIEnv * env, void* container, jint newSize) override{
        return m_containerAccess->resize(env, container, newSize);
    }

    inline void squeeze(JNIEnv * env, void* container) override{
        return m_containerAccess->squeeze(env, container);
    }

    inline void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) override{
        m_containerAccess->analyzeElements(container, analyzer, data);
    }

    Q_DISABLE_COPY_MOVE(WrapperVectorAccess)
private:
    AbstractVectorAccess* m_containerAccess;
};

class WrapperLinkedListAccess : public AbstractLinkedListAccess{
public:
    inline WrapperLinkedListAccess(AbstractLinkedListAccess* containerAccess)
        : AbstractLinkedListAccess(), m_containerAccess(containerAccess) {}

    inline ~WrapperLinkedListAccess() override{
        m_containerAccess->dispose();
        m_containerAccess = nullptr;
    }

    inline AbstractLinkedListAccess* clone() override{
        return m_containerAccess->clone();
    }

    inline void dispose() override { delete this; }

    inline void* createContainer() override{
        return m_containerAccess->createContainer();
    }

    inline void* copyContainer(const void* container) override{
        return m_containerAccess->copyContainer(container);
    }

    inline void assign(void* container, const void* other) override{
        m_containerAccess->assign(container, other);
    }

    inline void deleteContainer(void* container) override{
        m_containerAccess->deleteContainer(container);
    }

    inline int registerContainer(const QByteArray& containerTypeName) override{
        return m_containerAccess->registerContainer(containerTypeName);
    }

    inline PtrDeleterFunction containerDeleter() override{
        return m_containerAccess->containerDeleter();
    }

    inline bool isConstant() override{
        return m_containerAccess->isConstant();
    }

    inline const QMetaType& elementMetaType() override{
        return m_containerAccess->elementMetaType();
    }

    inline void append(JNIEnv * env, void* container, jobject value) override{
        m_containerAccess->append(env, container, value);
    }

    inline jobject first(JNIEnv * env, const void* container) override{
        return m_containerAccess->first(env, container);
    }

    inline jobject last(JNIEnv * env, const void* container) override{
        return m_containerAccess->last(env, container);
    }

    inline jobject begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override{
        return m_containerAccess->begin(env, ownerId, container);
    }

    inline void clear(JNIEnv * env, void* container) override{
        m_containerAccess->clear(env, container);
    }

    inline jboolean contains(JNIEnv * env, const void* container, jobject value) override{
        return m_containerAccess->contains(env, container, value);
    }

    inline jint count(JNIEnv * env, const void* container, jobject value) override{
        return m_containerAccess->count(env, container, value);
    }

    inline jobject end(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override{
        return m_containerAccess->end(env, ownerId, container);
    }

    inline jboolean endsWith(JNIEnv * env, const void* container, jobject value) override{
        return m_containerAccess->endsWith(env, container, value);
    }

    inline jboolean equal(JNIEnv * env, const void* container, jobject other) override{
        return m_containerAccess->equal(env, container, other);
    }

    inline void prepend(JNIEnv * env, void* container, jobject value) override{
        return m_containerAccess->prepend(env, container, value);
    }

    inline void removeFirst(JNIEnv * env, void* container) override{
        m_containerAccess->removeFirst(env, container);
    }

    inline jint removeAll(JNIEnv * env, void* container, jobject value) override{
        return m_containerAccess->removeAll(env, container, value);
    }

    inline void removeLast(JNIEnv * env, void* container) override{
        m_containerAccess->removeLast(env, container);
    }

    inline jboolean removeOne(JNIEnv * env, void* container, jobject value) override{
        return m_containerAccess->removeOne(env, container, value);
    }

    inline jint size(JNIEnv * env, const void* container) override{
        return m_containerAccess->size(env, container);
    }

    inline jboolean startsWith(JNIEnv * env, const void* container, jobject value) override{
        return m_containerAccess->startsWith(env, container, value);
    }

    inline jobject takeFirst(JNIEnv * env, void* container) override{
        return m_containerAccess->takeFirst(env, container);
    }

    inline jobject takeLast(JNIEnv * env, void* container) override{
        return m_containerAccess->takeLast(env, container);
    }

    inline void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) override{
        m_containerAccess->analyzeElements(container, analyzer, data);
    }

    Q_DISABLE_COPY_MOVE(WrapperLinkedListAccess)
private:
    AbstractLinkedListAccess* m_containerAccess;
};
#endif

class WrapperSetAccess : public AbstractSetAccess{
public:
    inline WrapperSetAccess(AbstractSetAccess* containerAccess)
        : AbstractSetAccess(), m_containerAccess(containerAccess) {}

    inline ~WrapperSetAccess() override{
        m_containerAccess->dispose();
        m_containerAccess = nullptr;
    }

    inline AbstractSetAccess* clone() override{
        return m_containerAccess->clone();
    }

    inline void dispose() override { delete this; }

    inline void* createContainer() override{
        return m_containerAccess->createContainer();
    }

    inline void* copyContainer(const void* container) override{
        return m_containerAccess->copyContainer(container);
    }

    inline void assign(void* container, const void* other) override{
        m_containerAccess->assign(container, other);
    }

    inline void deleteContainer(void* container) override{
        m_containerAccess->deleteContainer(container);
    }

    inline int registerContainer(const QByteArray& containerTypeName) override{
        return m_containerAccess->registerContainer(containerTypeName);
    }

    inline PtrDeleterFunction containerDeleter() override{
        return m_containerAccess->containerDeleter();
    }

    inline bool isConstant() override{
        return m_containerAccess->isConstant();
    }

    inline const QMetaType& elementMetaType() override{
        return m_containerAccess->elementMetaType();
    }

    inline jobject begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override{
        return m_containerAccess->begin(env, ownerId, container);
    }

    inline jint capacity(JNIEnv * env, const void* container) override{
        return m_containerAccess->capacity(env, container);
    }

    inline void clear(JNIEnv * env, void* container) override{
        m_containerAccess->clear(env, container);
    }

    inline jboolean contains(JNIEnv * env, const void* container, jobject value) override{
        return m_containerAccess->contains(env, container, value);
    }

    inline jobject end(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override{
        return m_containerAccess->end(env, ownerId, container);
    }

    inline void insert(JNIEnv * env, void* container, jobject value) override{
        m_containerAccess->insert(env, container, value);
    }

    inline void intersect(JNIEnv * env, void* container, jobject other) override{
        m_containerAccess->intersect(env, container, other);
    }

    inline jboolean intersects(JNIEnv * env, const void* container, jobject other) override{
        return m_containerAccess->intersects(env, container, other);
    }

    inline jboolean equal(JNIEnv * env, const void* container, jobject other) override{
        return m_containerAccess->equal(env, container, other);
    }

    inline jboolean remove(JNIEnv * env, void* container, jobject value) override{
        return m_containerAccess->remove(env, container, value);
    }

    inline void reserve(JNIEnv * env, void* container, jint newSize) override{
        m_containerAccess->reserve(env, container, newSize);
    }

    inline jint size(JNIEnv * env, const void* container) override{
        return m_containerAccess->size(env, container);
    }

    inline void subtract(JNIEnv * env, void* container, jobject other) override{
        m_containerAccess->subtract(env, container, other);
    }

    inline void unite(JNIEnv * env, void* container, jobject other) override{
        m_containerAccess->unite(env, container, other);
    }

    inline jobject values(JNIEnv * env, const void* container) override{
        return m_containerAccess->values(env, container);
    }

    inline void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) override{
        m_containerAccess->analyzeElements(container, analyzer, data);
    }

    Q_DISABLE_COPY_MOVE(WrapperSetAccess)
private:
    AbstractSetAccess* m_containerAccess;
};

class WrapperMapAccess : public AbstractMapAccess{
public:
    inline WrapperMapAccess(AbstractMapAccess* containerAccess)
        : AbstractMapAccess(), m_containerAccess(containerAccess) {}

    inline ~WrapperMapAccess() override{
        m_containerAccess->dispose();
        m_containerAccess = nullptr;
    }

    inline AbstractMapAccess* clone() override{
        return m_containerAccess->clone();
    }

    inline void dispose() override { delete this; }

    inline void* createContainer() override{
        return m_containerAccess->createContainer();
    }

    inline void* copyContainer(const void* container) override{
        return m_containerAccess->copyContainer(container);
    }

    inline void assign(void* container, const void* other) override{
        m_containerAccess->assign(container, other);
    }

    inline void deleteContainer(void* container) override{
        m_containerAccess->deleteContainer(container);
    }

    inline int registerContainer(const QByteArray& containerTypeName) override{
        return m_containerAccess->registerContainer(containerTypeName);
    }

    inline PtrDeleterFunction containerDeleter() override{
        return m_containerAccess->containerDeleter();
    }

    inline bool isConstant() override{
        return m_containerAccess->isConstant();
    }

    inline const QMetaType& keyMetaType() override{
        return m_containerAccess->keyMetaType();
    }

    inline const QMetaType& valueMetaType() override{
        return m_containerAccess->valueMetaType();
    }

    inline jobject begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override{
        return m_containerAccess->begin(env, ownerId, container);
    }

    inline void clear(JNIEnv * env, void* container) override{
        m_containerAccess->clear(env, container);
    }

    inline jboolean contains(JNIEnv * env, const void* container, jobject value) override{
        return m_containerAccess->contains(env, container, value);
    }

    inline jint count(JNIEnv * env, const void* container, jobject key) override {
         return m_containerAccess->count(env, container, key);
    }

    inline jobject end(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override{
        return m_containerAccess->end(env, ownerId, container);
    }

    inline jobject find(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override {
         return m_containerAccess->find(env, ownerId, container, key);
    }

    inline jobject first(JNIEnv * env, const void* container) override {
         return m_containerAccess->first(env, container);
    }

    inline jobject firstKey(JNIEnv * env, const void* container) override {
         return m_containerAccess->firstKey(env, container);
    }

    inline void insert(JNIEnv * env, void* container,jobject key,jobject value) override {
        m_containerAccess->insert(env, container, key, value);
    }

    inline jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override {
         return m_containerAccess->key(env, container, value, defaultKey);
    }

    inline jobject keys(JNIEnv * env, const void* container) override {
         return m_containerAccess->keys(env, container);
    }

    inline jobject keys(JNIEnv * env, const void* container, jobject value) override {
         return m_containerAccess->keys(env, container, value);
    }

    inline jobject last(JNIEnv * env, const void* container) override {
         return m_containerAccess->last(env, container);
    }

    inline jobject lastKey(JNIEnv * env, const void* container) override {
         return m_containerAccess->lastKey(env, container);
    }

    inline jobject lowerBound(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override {
         return m_containerAccess->lowerBound(env, ownerId, container, key);
    }

    inline jboolean equal(JNIEnv * env, const void* container, jobject other) override {
         return m_containerAccess->equal(env, container, other);
    }

    inline jint remove(JNIEnv * env, void* container,jobject key) override {
        return m_containerAccess->remove(env, container, key);
    }

    inline jint size(JNIEnv * env, const void* container) override {
         return m_containerAccess->size(env, container);
    }

    inline jobject take(JNIEnv *env, void* container,jobject key) override {
        return m_containerAccess->take(env, container, key);
    }

    inline jobject upperBound(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override {
         return m_containerAccess->upperBound(env, ownerId, container, key);
    }

    inline jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override {
         return m_containerAccess->value(env, container, key, defaultValue);
    }

    inline jobject values(JNIEnv * env, const void* container) override {
         return m_containerAccess->values(env, container);
    }

    inline bool keyLessThan(JNIEnv *env, jobject key1, jobject key2) override {
        return m_containerAccess->keyLessThan(env, key1, key2);
    }

    inline void analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) override {
        m_containerAccess->analyzeEntries(container, analyzer, data);
    }

    Q_DISABLE_COPY_MOVE(WrapperMapAccess)
private:
    AbstractMapAccess* m_containerAccess;
};

class WrapperMultiMapAccess : public AbstractMultiMapAccess{
public:
    inline WrapperMultiMapAccess(AbstractMultiMapAccess* containerAccess)
        : AbstractMultiMapAccess(), m_containerAccess(containerAccess) {}

    inline ~WrapperMultiMapAccess() override{
        m_containerAccess->dispose();
        m_containerAccess = nullptr;
    }

    inline AbstractMultiMapAccess* clone() override{
        return m_containerAccess->clone();
    }

    inline void dispose() override { delete this; }

    inline void* createContainer() override{
        return m_containerAccess->createContainer();
    }

    inline void* copyContainer(const void* container) override{
        return m_containerAccess->copyContainer(container);
    }

    inline void assign(void* container, const void* other) override{
        m_containerAccess->assign(container, other);
    }

    inline void deleteContainer(void* container) override{
        m_containerAccess->deleteContainer(container);
    }

    inline int registerContainer(const QByteArray& containerTypeName) override{
        return m_containerAccess->registerContainer(containerTypeName);
    }

    inline PtrDeleterFunction containerDeleter() override{
        return m_containerAccess->containerDeleter();
    }

    inline bool isConstant() override{
        return m_containerAccess->isConstant();
    }

    inline const QMetaType& keyMetaType() override{
        return m_containerAccess->keyMetaType();
    }

    inline const QMetaType& valueMetaType() override{
        return m_containerAccess->valueMetaType();
    }

    inline jobject begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override{
        return m_containerAccess->begin(env, ownerId, container);
    }

    inline void clear(JNIEnv * env, void* container) override{
        m_containerAccess->clear(env, container);
    }

    inline jboolean contains(JNIEnv * env, const void* container, jobject value) override{
        return m_containerAccess->contains(env, container, value);
    }

    inline jint count(JNIEnv * env,const void* container, jobject key) override{
        return m_containerAccess->count(env, container, key);
    }

    inline jobject end(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override{
        return m_containerAccess->end(env, ownerId, container);
    }

    inline jobject find(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override {
         return m_containerAccess->find(env, ownerId, container, key);
    }

    inline jobject first(JNIEnv * env, const void* container) override {
         return m_containerAccess->first(env, container);
    }

    inline jobject firstKey(JNIEnv * env, const void* container) override {
         return m_containerAccess->firstKey(env, container);
    }

    inline void insert(JNIEnv *env, void* container,jobject key,jobject value) override {
        m_containerAccess->insert(env, container, key, value);
    }

    inline jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override {
         return m_containerAccess->key(env, container, value, defaultKey);
    }

    inline jobject keys(JNIEnv * env, const void* container) override {
         return m_containerAccess->keys(env, container);
    }

    inline jobject keys(JNIEnv * env, const void* container, jobject value) override {
         return m_containerAccess->keys(env, container, value);
    }

    inline jobject last(JNIEnv * env, const void* container) override {
         return m_containerAccess->last(env, container);
    }

    inline jobject lastKey(JNIEnv * env, const void* container) override {
         return m_containerAccess->lastKey(env, container);
    }

    inline jobject lowerBound(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override {
         return m_containerAccess->lowerBound(env, ownerId, container, key);
    }

    inline jboolean equal(JNIEnv * env, const void* container, jobject other) override {
         return m_containerAccess->equal(env, container, other);
    }

    inline jint remove(JNIEnv * env, void* container, jobject key, jobject value) override {
        return m_containerAccess->remove(env, container, key, value);
    }

    inline jint size(JNIEnv * env, const void* container) override {
         return m_containerAccess->size(env, container);
    }

    inline jobject take(JNIEnv *env, void* container,jobject key) override {
        return m_containerAccess->take(env, container, key);
    }

    inline jobject upperBound(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override {
         return m_containerAccess->upperBound(env, ownerId, container, key);
    }

    inline jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override {
         return m_containerAccess->value(env, container, key, defaultValue);
    }

    inline jobject values(JNIEnv * env, const void* container) override {
         return m_containerAccess->values(env, container);
    }

    inline bool keyLessThan(JNIEnv *env, jobject key1, jobject key2) override {
        return m_containerAccess->keyLessThan(env, key1, key2);
    }

    inline jobject uniqueKeys(JNIEnv * env, const void* container) override {
        return m_containerAccess->uniqueKeys(env, container);
    }

    inline void unite(JNIEnv *env, void* container, jobject other) override {
         m_containerAccess->unite(env, container, other);
    }

    inline jobject values(JNIEnv * env, const void* container, jobject key) override {
        return m_containerAccess->values(env, container, key);
    }

    inline jboolean contains(JNIEnv * env, const void* container, jobject key, jobject value) override{
        return m_containerAccess->contains(env, container, key, value);
    }

    inline jint count(JNIEnv *env, const void* container, jobject key, jobject value) override {
         return m_containerAccess->count(env, container, key, value);
    }

    inline jobject find(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key, jobject value) override {
        return m_containerAccess->find(env, ownerId, container, key, value);
    }

    inline jint remove(JNIEnv * env, void* container,jobject key) override {
         return m_containerAccess->remove(env, container, key);
    }

    inline void replace(JNIEnv * env, void* container,jobject key, jobject value) override {
        m_containerAccess->replace(env, container, key, value);
    }

    inline void analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) override {
        m_containerAccess->analyzeEntries(container, analyzer, data);
    }

    Q_DISABLE_COPY_MOVE(WrapperMultiMapAccess)
private:
    AbstractMultiMapAccess* m_containerAccess;
};

class WrapperHashAccess : public AbstractHashAccess{
public:
    inline WrapperHashAccess(AbstractHashAccess* containerAccess)
        : AbstractHashAccess(), m_containerAccess(containerAccess) {}

    inline ~WrapperHashAccess() override{
        m_containerAccess->dispose();
        m_containerAccess = nullptr;
    }

    inline AbstractHashAccess* clone() override{
        return m_containerAccess->clone();
    }

    inline void dispose() override { delete this; }

    inline void* createContainer() override{
        return m_containerAccess->createContainer();
    }

    inline void* copyContainer(const void* container) override{
        return m_containerAccess->copyContainer(container);
    }

    inline void assign(void* container, const void* other) override{
        m_containerAccess->assign(container, other);
    }

    inline void deleteContainer(void* container) override{
        m_containerAccess->deleteContainer(container);
    }

    inline int registerContainer(const QByteArray& containerTypeName) override{
        return m_containerAccess->registerContainer(containerTypeName);
    }

    inline PtrDeleterFunction containerDeleter() override{
        return m_containerAccess->containerDeleter();
    }

    inline bool isConstant() override{
        return m_containerAccess->isConstant();
    }

    inline const QMetaType& keyMetaType() override{
        return m_containerAccess->keyMetaType();
    }

    inline const QMetaType& valueMetaType() override{
        return m_containerAccess->valueMetaType();
    }

    inline jobject begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override{
        return m_containerAccess->begin(env, ownerId, container);
    }

    inline jint capacity(JNIEnv * env,const void* container) override{
        return m_containerAccess->capacity(env, container);
    }

    inline void clear(JNIEnv * env, void* container) override{
        m_containerAccess->clear(env, container);
    }

    inline jboolean contains(JNIEnv * env, const void* container, jobject value) override{
        return m_containerAccess->contains(env, container, value);
    }

    inline jint count(JNIEnv * env, const void* container, jobject key) override {
        return m_containerAccess->count(env, container, key);
    }

    inline jobject end(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override{
        return m_containerAccess->end(env, ownerId, container);
    }

    inline jobject find(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override {
        return m_containerAccess->find(env, ownerId, container, key);
    }

    inline void insert(JNIEnv *env, void* container,jobject key,jobject value) override {
        m_containerAccess->insert(env, container, key, value);
    }

    inline jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override {
        return m_containerAccess->key(env, container, value, defaultKey);
    }

    inline jobject keys(JNIEnv * env, const void* container) override {
        return m_containerAccess->keys(env, container);
    }

    inline jobject keys(JNIEnv * env, const void* container, jobject value) override {
        return m_containerAccess->keys(env, container, value);
    }

    inline jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        return m_containerAccess->equal(env, container, other);
    }

    inline jint remove(JNIEnv * env, void* container,jobject key) override {
        return m_containerAccess->remove(env, container, key);
    }

    inline void reserve(JNIEnv * env,void* container, jint newSize) override {
        m_containerAccess->reserve(env, container, newSize);
    }

    inline jint size(JNIEnv * env, const void* container) override {
        return m_containerAccess->size(env, container);
    }

    inline jobject take(JNIEnv *env, void* container, jobject key) override {
        return m_containerAccess->take(env, container, key);
    }

    inline jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override {
        return m_containerAccess->value(env, container, key, defaultValue);
    }

    inline jobject values(JNIEnv * env, const void* container) override {
        return m_containerAccess->values(env, container);
    }

    inline void analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) override {
        m_containerAccess->analyzeEntries(container, analyzer, data);
    }

    Q_DISABLE_COPY_MOVE(WrapperHashAccess)
private:
    AbstractHashAccess* m_containerAccess;
};

class WrapperMultiHashAccess : public AbstractMultiHashAccess{
public:
    inline WrapperMultiHashAccess(AbstractMultiHashAccess* containerAccess)
        : AbstractMultiHashAccess(), m_containerAccess(containerAccess) {}

    inline ~WrapperMultiHashAccess() override{
        m_containerAccess->dispose();
        m_containerAccess = nullptr;
    }

    inline AbstractMultiHashAccess* clone() override{
        return m_containerAccess->clone();
    }

    inline void dispose() override { delete this; }

    inline void* createContainer() override{
        return m_containerAccess->createContainer();
    }

    inline void* copyContainer(const void* container) override{
        return m_containerAccess->copyContainer(container);
    }

    inline void assign(void* container, const void* other) override{
        m_containerAccess->assign(container, other);
    }

    inline void deleteContainer(void* container) override{
        m_containerAccess->deleteContainer(container);
    }

    inline int registerContainer(const QByteArray& containerTypeName) override{
        return m_containerAccess->registerContainer(containerTypeName);
    }

    inline PtrDeleterFunction containerDeleter() override{
        return m_containerAccess->containerDeleter();
    }

    inline bool isConstant() override{
        return m_containerAccess->isConstant();
    }

    inline const QMetaType& keyMetaType() override{
        return m_containerAccess->keyMetaType();
    }

    inline const QMetaType& valueMetaType() override{
        return m_containerAccess->valueMetaType();
    }

    inline jobject begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override{
        return m_containerAccess->begin(env, ownerId, container);
    }

    inline jint capacity(JNIEnv * env,const void* container) override{
        return m_containerAccess->capacity(env, container);
    }

    inline void clear(JNIEnv * env, void* container) override{
        m_containerAccess->clear(env, container);
    }

    inline jboolean contains(JNIEnv * env, const void* container, jobject value) override{
        return m_containerAccess->contains(env, container, value);
    }

    inline jint count(JNIEnv * env, const void* container, jobject key) override {
        return m_containerAccess->count(env, container, key);
    }

    inline jobject end(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override{
        return m_containerAccess->end(env, ownerId, container);
    }

    inline jobject find(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override {
        return m_containerAccess->find(env, ownerId, container, key);
    }

    inline void insert(JNIEnv *env, void* container,jobject key,jobject value) override {
        m_containerAccess->insert(env, container, key, value);
    }

    inline jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override {
        return m_containerAccess->key(env, container, value, defaultKey);
    }

    inline jobject keys(JNIEnv * env, const void* container) override {
        return m_containerAccess->keys(env, container);
    }

    inline jobject keys(JNIEnv * env, const void* container, jobject value) override {
        return m_containerAccess->keys(env, container, value);
    }

    inline jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        return m_containerAccess->equal(env, container, other);
    }

    inline jint remove(JNIEnv * env, void* container,jobject key) override {
        return m_containerAccess->remove(env, container, key);
    }

    inline void reserve(JNIEnv * env,void* container, jint newSize) override {
        m_containerAccess->reserve(env, container, newSize);
    }

    inline jint size(JNIEnv * env, const void* container) override {
        return m_containerAccess->size(env, container);
    }

    inline jobject take(JNIEnv *env, void* container, jobject key) override {
        return m_containerAccess->take(env, container, key);
    }

    inline jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override {
        return m_containerAccess->value(env, container, key, defaultValue);
    }

    inline jobject values(JNIEnv * env, const void* container) override {
        return m_containerAccess->values(env, container);
    }

    inline jobject values(JNIEnv * env, const void* container, jobject key) override {
        return m_containerAccess->values(env, container, key);
    }

    inline jboolean contains(JNIEnv * env, const void* container, jobject key, jobject value) override{
        return m_containerAccess->contains(env, container, key, value);
    }

    inline jint count(JNIEnv *env, const void* container, jobject key, jobject value) override {
        return m_containerAccess->count(env, container, key, value);
    }

    inline jobject find(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key, jobject value) override {
        return m_containerAccess->find(env, ownerId, container, key, value);
    }

    inline jint remove(JNIEnv * env, void* container, jobject key, jobject value) override {
         return m_containerAccess->remove(env, container, key, value);
    }

    inline void replace(JNIEnv * env, void* container, jobject key, jobject value) override {
        m_containerAccess->replace(env, container, key, value);
    }

    inline jobject uniqueKeys(JNIEnv * env,const void* container) override {
        return m_containerAccess->uniqueKeys(env, container);
    }

    inline void unite(JNIEnv * env, void* container, jobject other) override {
        m_containerAccess->unite(env, container, other);
    }

    inline void analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) override {
        m_containerAccess->analyzeEntries(container, analyzer, data);
    }
    Q_DISABLE_COPY_MOVE(WrapperMultiHashAccess)
private:
    AbstractMultiHashAccess* m_containerAccess;
};

class RCSet{
public:
    RCSet() = default;
    virtual ~RCSet() = default;
protected:
    RCSet(const RCSet&);
    void assignRC(JNIEnv * env, const RCSet& other);
    jobject rcSet(JNIEnv * env);
    void clearRC(JNIEnv * env);
    void addRC(JNIEnv * env, jobject value);
    void addAllRC(JNIEnv * env, jobject value);
    void removeRC(JNIEnv * env, jobject value);
private:
    JObjectWrapper m_rcSet;
};

class RCMap{
public:
    RCMap() = default;
    virtual ~RCMap() = default;
protected:
    RCMap(JNIEnv * env, jobject otherRCMap);
    void assignRC(JNIEnv * env, const RCMap& other);
    jobject rcMap(JNIEnv * env);
    void clearRC(JNIEnv * env);
    void putRC(JNIEnv * env, jobject key, jobject value);
    void putAllRC(JNIEnv * env, jobject values);
    void removeRC(JNIEnv * env, jobject key);
private:
    JObjectWrapper m_rcMap;
};

class RCMultiMap{
public:
    RCMultiMap() = default;
    virtual ~RCMultiMap() = default;
protected:
    RCMultiMap(JNIEnv * env, jobject otherRCMap);
    void assignRC(JNIEnv * env, const RCMultiMap& other);
    void uniteRC(JNIEnv * env, const RCMultiMap& other);
    jobject rcMap(JNIEnv * env);
    void clearRC(JNIEnv * env);
    void putRC(JNIEnv * env, jobject key, jobject value);
    void putAllRC(JNIEnv * env, jobject values);
    void removeRC(JNIEnv * env, jobject key);
    void removeRC(JNIEnv * env, jobject key, jobject value);
    void replaceRC(JNIEnv * env, jobject key, jobject value);
private:
    JObjectWrapper m_rcMap;
};

class PointerRCListAccess : public WrapperListAccess, public RCSet{
private:
    PointerRCListAccess(PointerRCListAccess& _this);
public:
    ~PointerRCListAccess() override = default;
    PointerRCListAccess(AbstractListAccess* containerAccess);
    PointerRCListAccess* clone() override;
    void assign(void* container, const void* other) override;
    void* copyContainer(const void* container) override;
    void append(JNIEnv * env, void* container, jobject value) override;
    void appendList(JNIEnv * env, void* container, jobject list) override;
    jobject takeAt(JNIEnv * env, void* container, jint index) override;
    void replace(JNIEnv * env, void* container, jint index, jobject value) override;
    jboolean removeOne(JNIEnv * env, void* container, jobject value) override;
    void removeAt(JNIEnv * env, void* container, jint index) override;
    jint removeAll(JNIEnv * env, void* container, jobject value) override;
    void prepend(JNIEnv * env, void* container, jobject value) override;
    jobject mid(JNIEnv * env, const void* container, jint index1, jint index2) override;
    void insert(JNIEnv * env, void* container, jint index, jobject value) override;
    void clear(JNIEnv * env, void* container) override;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    void remove(JNIEnv * env, void* container, jint index, jint n) override;
    void insert(JNIEnv * env, void* container, jint index, jint n, jobject value) override;
    void fill(JNIEnv * env, void* container, jobject value, jint size) override;
#endif
};

class PointerRCSetAccess : public WrapperSetAccess, public RCSet{
private:
    PointerRCSetAccess(PointerRCSetAccess& _this);
public:
    ~PointerRCSetAccess() override = default;
    PointerRCSetAccess(AbstractSetAccess* containerAccess);
    PointerRCSetAccess* clone() override;
    void assign(void* container, const void* other) override;
    void* copyContainer(const void* container) override;
    void insert(JNIEnv * env, void* container, jobject value) override;
    jboolean remove(JNIEnv * env, void* container, jobject value) override;
    void clear(JNIEnv * env, void* container) override;
    void intersect(JNIEnv * env, void* container, jobject other) override;
    void subtract(JNIEnv * env, void* container, jobject other) override;
    void unite(JNIEnv * env, void* container, jobject other) override;
};

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
class PointerRCLinkedListAccess : public WrapperLinkedListAccess, public RCSet{
private:
    PointerRCLinkedListAccess(PointerRCLinkedListAccess& _this);
public:
    ~PointerRCLinkedListAccess() override = default;
    PointerRCLinkedListAccess(AbstractLinkedListAccess* containerAccess);
    PointerRCLinkedListAccess* clone() override;
    void assign(void* container, const void* other) override;
    void* copyContainer(const void* container) override;
    void clear(JNIEnv * env, void* container) override;
    void append(JNIEnv * env, void* container, jobject value) override;
    void prepend(JNIEnv * env, void* container, jobject value) override;
    void removeFirst(JNIEnv * env, void* container) override;
    void removeLast(JNIEnv * env, void* container) override;
    jint removeAll(JNIEnv * env, void* container, jobject value) override;
    jboolean removeOne(JNIEnv * env, void* container, jobject value) override;
    jobject takeFirst(JNIEnv * env, void* container) override;
    jobject takeLast(JNIEnv * env, void* container) override;
};

class PointerRCVectorAccess : public WrapperVectorAccess, public RCSet{
private:
    PointerRCVectorAccess(PointerRCVectorAccess& _this);
public:
    ~PointerRCVectorAccess() override = default;
    PointerRCVectorAccess(AbstractVectorAccess* containerAccess);
    PointerRCVectorAccess* clone() override;
    void assign(void* container, const void* other) override;
    void* copyContainer(const void* container) override;
    void append(JNIEnv * env, void* container, jobject value) override;
    void appendVector(JNIEnv * env, void* container, jobject list) override;
    jobject takeAt(JNIEnv * env, void* container, jint index) override;
    void replace(JNIEnv * env, void* container, jint index, jobject value) override;
    jboolean removeOne(JNIEnv * env, void* container, jobject value) override;
    void removeAt(JNIEnv * env, void* container, jint index) override;
    jint removeAll(JNIEnv * env, void* container, jobject value) override;
    void prepend(JNIEnv * env, void* container, jobject value) override;
    jobject mid(JNIEnv * env, const void* container, jint index1, jint index2) override;
    void insert(JNIEnv * env, void* container, jint index, jobject value) override;
    void clear(JNIEnv * env, void* container) override;
    void remove(JNIEnv * env, void* container, jint index, jint n) override;
    void insert(JNIEnv * env, void* container, jint index, jint n, jobject value) override;
    void fill(JNIEnv * env, void* container, jobject value, jint size) override;
};
#endif

class KeyPointerRCMapAccess : public WrapperMapAccess, public RCSet{
private:
    KeyPointerRCMapAccess(KeyPointerRCMapAccess& _this);
public:
    ~KeyPointerRCMapAccess() override = default;
    KeyPointerRCMapAccess(AbstractMapAccess* containerAccess);
    KeyPointerRCMapAccess* clone() override;
    void assign(void* container, const void* other) override;
    void* copyContainer(const void* container) override;
    void clear(JNIEnv * env, void* container) override;
    void insert(JNIEnv * env, void* container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, void* container,jobject key) override;
    jobject take(JNIEnv *env, void* container,jobject key) override;
};

class KeyPointerRCMultiMapAccess : public WrapperMultiMapAccess, public RCSet{
private:
    KeyPointerRCMultiMapAccess(KeyPointerRCMultiMapAccess& _this);
public:
    ~KeyPointerRCMultiMapAccess() override = default;
    KeyPointerRCMultiMapAccess(AbstractMultiMapAccess* containerAccess);
    KeyPointerRCMultiMapAccess* clone() override;
    void assign(void* container, const void* other) override;
    void* copyContainer(const void* container) override;
    void clear(JNIEnv * env, void* container) override;
    void insert(JNIEnv * env, void* container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, void* container,jobject key) override;
    jobject take(JNIEnv *env, void* container,jobject key) override;
    jint remove(JNIEnv * env, void* container, jobject key, jobject value) override;
    void unite(JNIEnv *env, void* container, jobject other) override;
    void replace(JNIEnv * env, void* container,jobject key, jobject value) override;
};

class KeyPointerRCHashAccess : public WrapperHashAccess, public RCSet{
private:
    KeyPointerRCHashAccess(KeyPointerRCHashAccess& _this);
public:
    ~KeyPointerRCHashAccess() override = default;
    KeyPointerRCHashAccess(AbstractHashAccess* containerAccess);
    KeyPointerRCHashAccess* clone() override;
    void assign(void* container, const void* other) override;
    void* copyContainer(const void* container) override;
    void clear(JNIEnv * env, void* container) override;
    void insert(JNIEnv * env, void* container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, void* container,jobject key) override;
    jobject take(JNIEnv *env, void* container,jobject key) override;
};

class KeyPointerRCMultiHashAccess : public WrapperMultiHashAccess, public RCSet{
private:
    KeyPointerRCMultiHashAccess(KeyPointerRCMultiHashAccess& _this);
public:
    ~KeyPointerRCMultiHashAccess() override = default;
    KeyPointerRCMultiHashAccess(AbstractMultiHashAccess* containerAccess);
    KeyPointerRCMultiHashAccess* clone() override;
    void assign(void* container, const void* other) override;
    void* copyContainer(const void* container) override;
    void clear(JNIEnv * env, void* container) override;
    void insert(JNIEnv * env, void* container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, void* container,jobject key) override;
    jobject take(JNIEnv *env, void* container,jobject key) override;
    jint remove(JNIEnv * env, void* container, jobject key, jobject value) override;
    void unite(JNIEnv *env, void* container, jobject other) override;
    void replace(JNIEnv * env, void* container,jobject key, jobject value) override;
};

class ValuePointerRCMapAccess : public WrapperMapAccess, public RCSet{
private:
    ValuePointerRCMapAccess(ValuePointerRCMapAccess& _this);
public:
    ~ValuePointerRCMapAccess() override = default;
    ValuePointerRCMapAccess(AbstractMapAccess* containerAccess);
    ValuePointerRCMapAccess* clone() override;
    void assign(void* container, const void* other) override;
    void* copyContainer(const void* container) override;
    void clear(JNIEnv * env, void* container) override;
    void insert(JNIEnv * env, void* container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, void* container,jobject key) override;
    jobject take(JNIEnv *env, void* container,jobject key) override;
};

class ValuePointerRCMultiMapAccess : public WrapperMultiMapAccess, public RCSet{
private:
    ValuePointerRCMultiMapAccess(ValuePointerRCMultiMapAccess& _this);
public:
    ~ValuePointerRCMultiMapAccess() override = default;
    ValuePointerRCMultiMapAccess(AbstractMultiMapAccess* containerAccess);
    ValuePointerRCMultiMapAccess* clone() override;
    void assign(void* container, const void* other) override;
    void* copyContainer(const void* container) override;
    void clear(JNIEnv * env, void* container) override;
    void insert(JNIEnv * env, void* container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, void* container,jobject key) override;
    jobject take(JNIEnv *env, void* container,jobject key) override;
    jint remove(JNIEnv * env, void* container, jobject key, jobject value) override;
    void unite(JNIEnv *env, void* container, jobject other) override;
    void replace(JNIEnv * env, void* container,jobject key, jobject value) override;
};

class ValuePointerRCHashAccess : public WrapperHashAccess, public RCSet{
private:
    ValuePointerRCHashAccess(ValuePointerRCHashAccess& _this);
public:
    ~ValuePointerRCHashAccess() override = default;
    ValuePointerRCHashAccess(AbstractHashAccess* containerAccess);
    ValuePointerRCHashAccess* clone() override;
    void assign(void* container, const void* other) override;
    void* copyContainer(const void* container) override;
    void clear(JNIEnv * env, void* container) override;
    void insert(JNIEnv * env, void* container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, void* container,jobject key) override;
    jobject take(JNIEnv *env, void* container,jobject key) override;
};

class ValuePointerRCMultiHashAccess : public WrapperMultiHashAccess, public RCSet{
private:
    ValuePointerRCMultiHashAccess(ValuePointerRCMultiHashAccess& _this);
public:
    ~ValuePointerRCMultiHashAccess() override = default;
    ValuePointerRCMultiHashAccess(AbstractMultiHashAccess* containerAccess);
    ValuePointerRCMultiHashAccess* clone() override;
    void assign(void* container, const void* other) override;
    void* copyContainer(const void* container) override;
    void clear(JNIEnv * env, void* container) override;
    void insert(JNIEnv * env, void* container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, void* container,jobject key) override;
    jobject take(JNIEnv *env, void* container,jobject key) override;
    jint remove(JNIEnv * env, void* container, jobject key, jobject value) override;
    void unite(JNIEnv *env, void* container, jobject other) override;
    void replace(JNIEnv * env, void* container,jobject key, jobject value) override;
};

class PointersRCMapAccess : public WrapperMapAccess, public RCMap{
private:
    PointersRCMapAccess(PointersRCMapAccess& _this);
public:
    ~PointersRCMapAccess() override = default;
    PointersRCMapAccess(AbstractMapAccess* containerAccess);
    PointersRCMapAccess* clone() override;
    void assign(void* container, const void* other) override;
    void* copyContainer(const void* container) override;
    void clear(JNIEnv * env, void* container) override;
    void insert(JNIEnv * env, void* container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, void* container,jobject key) override;
    jobject take(JNIEnv *env, void* container,jobject key) override;
};

class PointersRCMultiMapAccess : public WrapperMultiMapAccess, public RCMultiMap{
private:
    PointersRCMultiMapAccess(PointersRCMultiMapAccess& _this);
public:
    ~PointersRCMultiMapAccess() override = default;
    PointersRCMultiMapAccess(AbstractMultiMapAccess* containerAccess);
    PointersRCMultiMapAccess* clone() override;
    void assign(void* container, const void* other) override;
    void* copyContainer(const void* container) override;
    void clear(JNIEnv * env, void* container) override;
    void insert(JNIEnv * env, void* container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, void* container,jobject key) override;
    jobject take(JNIEnv *env, void* container,jobject key) override;
    jint remove(JNIEnv * env, void* container, jobject key, jobject value) override;
    void unite(JNIEnv *env, void* container, jobject other) override;
    void replace(JNIEnv * env, void* container,jobject key, jobject value) override;
};

class PointersRCHashAccess : public WrapperHashAccess, public RCMap{
private:
    PointersRCHashAccess(PointersRCHashAccess& _this);
public:
    ~PointersRCHashAccess() override = default;
    PointersRCHashAccess(AbstractHashAccess* containerAccess);
    PointersRCHashAccess* clone() override;
    void assign(void* container, const void* other) override;
    void* copyContainer(const void* container) override;
    void clear(JNIEnv * env, void* container) override;
    void insert(JNIEnv * env, void* container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, void* container,jobject key) override;
    jobject take(JNIEnv *env, void* container,jobject key) override;
};

class PointersRCMultiHashAccess : public WrapperMultiHashAccess, public RCMultiMap{
private:
    PointersRCMultiHashAccess(PointersRCMultiHashAccess& _this);
public:
    ~PointersRCMultiHashAccess() override = default;
    PointersRCMultiHashAccess(AbstractMultiHashAccess* containerAccess);
    PointersRCMultiHashAccess* clone() override;
    void assign(void* container, const void* other) override;
    void* copyContainer(const void* container) override;
    void clear(JNIEnv * env, void* container) override;
    void insert(JNIEnv * env, void* container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, void* container,jobject key) override;
    jobject take(JNIEnv *env, void* container,jobject key) override;
    jint remove(JNIEnv * env, void* container, jobject key, jobject value) override;
    void unite(JNIEnv * env, void* container, jobject other) override;
    void replace(JNIEnv * env, void* container,jobject key, jobject value) override;
};

class NestedPointersRCListAccess : public WrapperListAccess, public RCSet{
private:
    NestedPointersRCListAccess(NestedPointersRCListAccess& _this);
    void updateRC(JNIEnv * env, const void* container);
public:
    ~NestedPointersRCListAccess() override = default;
    NestedPointersRCListAccess(AbstractListAccess* containerAccess);
    NestedPointersRCListAccess* clone() override;
    void assign(void* container, const void* other) override;
    void* copyContainer(const void* container) override;
    void append(JNIEnv * env, void* container, jobject value) override;
    void appendList(JNIEnv * env, void* container, jobject list) override;
    jobject takeAt(JNIEnv * env, void* container, jint index) override;
    void replace(JNIEnv * env, void* container, jint index, jobject value) override;
    jboolean removeOne(JNIEnv * env, void* container, jobject value) override;
    void removeAt(JNIEnv * env, void* container, jint index) override;
    jint removeAll(JNIEnv * env, void* container, jobject value) override;
    void prepend(JNIEnv * env, void* container, jobject value) override;
    jobject mid(JNIEnv * env, const void* container, jint index1, jint index2) override;
    void insert(JNIEnv * env, void* container, jint index, jobject value) override;
    void clear(JNIEnv * env, void* container) override;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    void remove(JNIEnv * env, void* container, jint index, jint n) override;
    void insert(JNIEnv * env, void* container, jint index, jint n, jobject value) override;
    void fill(JNIEnv * env, void* container, jobject value, jint size) override;
#endif
};

class NestedPointersRCSetAccess : public WrapperSetAccess, public RCSet{
private:
    NestedPointersRCSetAccess(NestedPointersRCSetAccess& _this);
    void updateRC(JNIEnv * env, const void* container);
public:
    ~NestedPointersRCSetAccess() override = default;
    NestedPointersRCSetAccess(AbstractSetAccess* containerAccess);
    NestedPointersRCSetAccess* clone() override;
    void assign(void* container, const void* other) override;
    void* copyContainer(const void* container) override;
    void insert(JNIEnv * env, void* container, jobject value) override;
    jboolean remove(JNIEnv * env, void* container, jobject value) override;
    void clear(JNIEnv * env, void* container) override;
    void intersect(JNIEnv * env, void* container, jobject other) override;
    void subtract(JNIEnv * env, void* container, jobject other) override;
    void unite(JNIEnv * env, void* container, jobject other) override;
};

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
class NestedPointersRCLinkedListAccess : public WrapperLinkedListAccess, public RCSet{
private:
    NestedPointersRCLinkedListAccess(NestedPointersRCLinkedListAccess& _this);
    void updateRC(JNIEnv * env, const void* container);
public:
    ~NestedPointersRCLinkedListAccess() override = default;
    NestedPointersRCLinkedListAccess(AbstractLinkedListAccess* containerAccess);
    NestedPointersRCLinkedListAccess* clone() override;
    void assign(void* container, const void* other) override;
    void* copyContainer(const void* container) override;
    void clear(JNIEnv * env, void* container) override;
    void append(JNIEnv * env, void* container, jobject value) override;
    void prepend(JNIEnv * env, void* container, jobject value) override;
    void removeFirst(JNIEnv * env, void* container) override;
    void removeLast(JNIEnv * env, void* container) override;
    jint removeAll(JNIEnv * env, void* container, jobject value) override;
    jboolean removeOne(JNIEnv * env, void* container, jobject value) override;
    jobject takeFirst(JNIEnv * env, void* container) override;
    jobject takeLast(JNIEnv * env, void* container) override;
};

class NestedPointersRCVectorAccess : public WrapperVectorAccess, public RCSet{
private:
    NestedPointersRCVectorAccess(NestedPointersRCVectorAccess& _this);
    void updateRC(JNIEnv * env, const void* container);
public:
    ~NestedPointersRCVectorAccess() override = default;
    NestedPointersRCVectorAccess(AbstractVectorAccess* containerAccess);
    NestedPointersRCVectorAccess* clone() override;
    void assign(void* container, const void* other) override;
    void* copyContainer(const void* container) override;
    void append(JNIEnv * env, void* container, jobject value) override;
    void appendVector(JNIEnv * env, void* container, jobject list) override;
    jobject takeAt(JNIEnv * env, void* container, jint index) override;
    void replace(JNIEnv * env, void* container, jint index, jobject value) override;
    jboolean removeOne(JNIEnv * env, void* container, jobject value) override;
    void removeAt(JNIEnv * env, void* container, jint index) override;
    jint removeAll(JNIEnv * env, void* container, jobject value) override;
    void prepend(JNIEnv * env, void* container, jobject value) override;
    jobject mid(JNIEnv * env, const void* container, jint index1, jint index2) override;
    void insert(JNIEnv * env, void* container, jint index, jobject value) override;
    void clear(JNIEnv * env, void* container) override;
    void remove(JNIEnv * env, void* container, jint index, jint n) override;
    void insert(JNIEnv * env, void* container, jint index, jint n, jobject value) override;
    void fill(JNIEnv * env, void* container, jobject value, jint size) override;
};
#endif

class NestedPointersRCMapAccess : public WrapperMapAccess, public RCSet{
private:
    NestedPointersRCMapAccess(NestedPointersRCMapAccess& _this);
    void updateRC(JNIEnv * env, const void* container);
public:
    ~NestedPointersRCMapAccess() override = default;
    NestedPointersRCMapAccess(AbstractMapAccess* containerAccess);
    NestedPointersRCMapAccess* clone() override;
    void assign(void* container, const void* other) override;
    void* copyContainer(const void* container) override;
    void clear(JNIEnv * env, void* container) override;
    void insert(JNIEnv * env, void* container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, void* container,jobject key) override;
    jobject take(JNIEnv *env, void* container,jobject key) override;
};

class NestedPointersRCMultiMapAccess : public WrapperMultiMapAccess, public RCSet{
private:
    NestedPointersRCMultiMapAccess(NestedPointersRCMultiMapAccess& _this);
    void updateRC(JNIEnv * env, const void* container);
public:
    ~NestedPointersRCMultiMapAccess() override = default;
    NestedPointersRCMultiMapAccess(AbstractMultiMapAccess* containerAccess);
    NestedPointersRCMultiMapAccess* clone() override;
    void assign(void* container, const void* other) override;
    void* copyContainer(const void* container) override;
    void clear(JNIEnv * env, void* container) override;
    void insert(JNIEnv * env, void* container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, void* container,jobject key) override;
    jobject take(JNIEnv *env, void* container,jobject key) override;
    jint remove(JNIEnv * env, void* container, jobject key, jobject value) override;
    void unite(JNIEnv *env, void* container, jobject other) override;
    void replace(JNIEnv * env, void* container,jobject key, jobject value) override;
};

class NestedPointersRCHashAccess : public WrapperHashAccess, public RCSet{
private:
    NestedPointersRCHashAccess(NestedPointersRCHashAccess& _this);
    void updateRC(JNIEnv * env, const void* container);
public:
    ~NestedPointersRCHashAccess() override = default;
    NestedPointersRCHashAccess(AbstractHashAccess* containerAccess);
    NestedPointersRCHashAccess* clone() override;
    void assign(void* container, const void* other) override;
    void* copyContainer(const void* container) override;
    void clear(JNIEnv * env, void* container) override;
    void insert(JNIEnv * env, void* container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, void* container,jobject key) override;
    jobject take(JNIEnv *env, void* container,jobject key) override;
};

class NestedPointersRCMultiHashAccess : public WrapperMultiHashAccess, public RCSet{
private:
    NestedPointersRCMultiHashAccess(NestedPointersRCMultiHashAccess& _this);
    void updateRC(JNIEnv * env, const void* container);
public:
    ~NestedPointersRCMultiHashAccess() override = default;
    NestedPointersRCMultiHashAccess(AbstractMultiHashAccess* containerAccess);
    NestedPointersRCMultiHashAccess* clone() override;
    void assign(void* container, const void* other) override;
    void* copyContainer(const void* container) override;
    void clear(JNIEnv * env, void* container) override;
    void insert(JNIEnv * env, void* container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, void* container,jobject key) override;
    jobject take(JNIEnv *env, void* container,jobject key) override;
    jint remove(JNIEnv * env, void* container, jobject key, jobject value) override;
    void unite(JNIEnv * env, void* container, jobject other) override;
    void replace(JNIEnv * env, void* container,jobject key, jobject value) override;
};

#endif // QTJAMBI_CONTAINERACCESS_P_H
