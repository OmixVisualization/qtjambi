#ifndef QTJAMBI_CONTAINERACCESS_P_H
#define QTJAMBI_CONTAINERACCESS_P_H

#include <QtCore/QtGlobal>

QT_WARNING_DISABLE_DEPRECATED

#include "qtjambi_containeraccess.h"
#include "qtjambi_jobjectwrapper.h"
#include <QtCore/QThreadStorage>

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

int registerMetaType(const QByteArray& typeName,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                     QMetaType::Destructor destructor,
                     QMetaType::Constructor constructor,
#else
                     QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr,
                     QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr,
                     QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr,
                     QtPrivate::QMetaTypeInterface::DtorFn dtor,
                     QtPrivate::QMetaTypeInterface::EqualsFn equals,
                     QtPrivate::QMetaTypeInterface::LessThanFn lessThan,
                     QtPrivate::QMetaTypeInterface::DebugStreamFn debugStream,
                     QtPrivate::QMetaTypeInterface::DataStreamOutFn dataStreamOutFn,
                     QtPrivate::QMetaTypeInterface::DataStreamInFn dataStreamInFn,
                     QtPrivate::QMetaTypeInterface::LegacyRegisterOp legacyRegisterOp,
#endif
                     uint size,
                     ushort align,
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                     int builtInTypeId,
#endif
                     QMetaType::TypeFlags flags,
                     const QMetaObject *metaObject,
                     AfterRegistrationFunction afterRegistrationFunction);
void insertHashFunctionByMetaType(int type, const QHashFunction& fct);

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
    RCSet();
    virtual ~RCSet();
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
    RCMap();
    virtual ~RCMap();
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
    RCMultiMap();
    virtual ~RCMultiMap();
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
    ~PointerRCListAccess() override;
    PointerRCListAccess(AbstractListAccess* containerAccess);
    PointerRCListAccess* clone() override;
    void assign(void* container, const void* other) override;
    void* copyContainer(const void* container) override;
    void append(JNIEnv * env, void* container, jobject value) override;
    void appendList(JNIEnv * env, void* container, jobject list) override;
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
    ~PointerRCSetAccess() override;
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
    ~PointerRCLinkedListAccess() override;
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
    ~PointerRCVectorAccess() override;
    PointerRCVectorAccess(AbstractVectorAccess* containerAccess);
    PointerRCVectorAccess* clone() override;
    void assign(void* container, const void* other) override;
    void* copyContainer(const void* container) override;
    void append(JNIEnv * env, void* container, jobject value) override;
    void appendVector(JNIEnv * env, void* container, jobject list) override;
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
    ~KeyPointerRCMapAccess() override;
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
    ~KeyPointerRCMultiMapAccess() override;
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
    ~KeyPointerRCHashAccess() override;
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
    ~KeyPointerRCMultiHashAccess() override;
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
    ~ValuePointerRCMapAccess() override;
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

class AutoPairAccess : public AbstractPairAccess{
    QMetaType m_keyMetaType;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    size_t m_keyAlign;
#endif
    QHashFunction m_keyHashFunction;
    InternalToExternalConverter m_keyInternalToExternalConverter;
    ExternalToInternalConverter m_keyExternalToInternalConverter;
    QMetaType m_valueMetaType;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    size_t m_valueAlign;
#endif
    QHashFunction m_valueHashFunction;
    InternalToExternalConverter m_valueInternalToExternalConverter;
    ExternalToInternalConverter m_valueExternalToInternalConverter;
    size_t m_align;
    size_t m_offset;
    size_t m_size;

    AutoPairAccess(const AutoPairAccess& other);

public:
    AutoPairAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            int keyMetaType,
            size_t keyAlign,
#else
            const QMetaType& keyMetaType,
#endif
            const QHashFunction& keyHashFunction,
            const InternalToExternalConverter& keyInternalToExternalConverter,
            const ExternalToInternalConverter& keyExternalToInternalConverter,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            int valueMetaType,
            size_t valueAlign,
#else
            const QMetaType& valueMetaType,
#endif
            const QHashFunction& valueHashFunction,
            const InternalToExternalConverter& valueInternalToExternalConverter,
            const ExternalToInternalConverter& valueExternalToInternalConverter
            );

    void dispose() override;
    AutoPairAccess* clone() override;
private:
    void* constructContainer(void* result, const void* container = nullptr);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* result, void* container);
    bool equals(const void* p1, const void* p2);
    void debugStream(QDebug &s, const void *ptr);
    void dataStreamOut(QDataStream &s, const void *ptr);
    void dataStreamIn(QDataStream &s, void *ptr);
    static void defaultCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr);
    static void copyCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, const void *other);
    static void moveCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, void *other);
    static void dtor(const QtPrivate::QMetaTypeInterface *iface, void *ptr);
    static bool equalsFn(const QtPrivate::QMetaTypeInterface *iface, const void *ptr1, const void *ptr2);
    static void debugStreamFn(const QtPrivate::QMetaTypeInterface *iface, QDebug &s, const void *ptr);
    static void dataStreamOutFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, const void *ptr);
    static void dataStreamInFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, void *ptr);
#endif
public:
    void* createContainer() override;
    void* copyContainer(const void* container) override;
    void assign(void* container, const void* other) override;
    void destructContainer(void* container);
    void deleteContainer(void* container) override;
    int registerContainer(const QByteArray& typeName) override;
    bool isConstant() override {return false;}
    jobject first(JNIEnv * env, const void* container) override;
    void setFirst(JNIEnv *env, void* container, jobject first) override;
    jobject second(JNIEnv * env, const void* container) override;
    void setSecond(JNIEnv *env, void* container, jobject second) override;
};

class AutoIteratorAccess : public AbstractIteratorAccess{
    InternalToExternalConverter m_internalToExternalConverter;
    std::function<void(void*)> m_increment;
    std::function<void(void*)> m_decrement;
    std::function<const void*(void*)> m_value;
    std::function<bool(void*,void*)> m_lessThan;
    std::function<bool(void*,void*)> m_equals;
public:
    ~AutoIteratorAccess() override;
    AutoIteratorAccess(
            const InternalToExternalConverter& internalToExternalConverter,
            std::function<void(void*)>&& increment,
            std::function<void(void*)>&& decrement,
            std::function<const void*(void*)>&& value,
            std::function<bool(void*,void*)>&& lessThan,
            std::function<bool(void*,void*)>&& equals
        );
    void dispose() override;
    AbstractIteratorAccess* clone() override;
    jobject value(JNIEnv * env, void* iterator) override;
    void increment(JNIEnv * env, void* iterator) override;
    void decrement(JNIEnv * env, void* iterator) override;
    jboolean lessThan(JNIEnv * env, void* iterator, void* other) override;
    bool canLess() override;
    jboolean equals(JNIEnv * env, void* iterator, void* other) override;
private:
    Q_DISABLE_COPY_MOVE(AutoIteratorAccess)
    friend class AutoBiIteratorAccess;
};

class AutoBiIteratorAccess : public virtual AbstractBiIteratorAccess, public virtual AutoIteratorAccess{
    InternalToExternalConverter m_keyInternalToExternalConverter;
    std::function<const void*(void*)> m_key;
public:
    ~AutoBiIteratorAccess() override;
    AutoBiIteratorAccess(
            const InternalToExternalConverter& internalToExternalConverter,
            std::function<void(void*)>&& increment,
            std::function<void(void*)>&& decrement,
            std::function<const void*(void*)>&& value,
            std::function<bool(void*,void*)>&& lessThan,
            std::function<bool(void*,void*)>&& equals,
            const InternalToExternalConverter& keyInternalToExternalConverter,
            std::function<const void*(void*)>&& key
            );
    jobject value(JNIEnv * env, void* iterator) override;
    void increment(JNIEnv * env, void* iterator) override;
    void decrement(JNIEnv * env, void* iterator) override;
    jboolean lessThan(JNIEnv * env, void* iterator, void* other) override;
    bool canLess() override;
    jboolean equals(JNIEnv * env, void* iterator, void* other) override;
    AbstractBiIteratorAccess* clone() override;
    jobject key(JNIEnv * env, void* iterator) override;
private:
    Q_DISABLE_COPY_MOVE(AutoBiIteratorAccess)
};

class AutoListAccess : public AbstractListAccess{
    QMetaType m_elementMetaType;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    bool m_isLargeOrStaticType;
#endif
    QHashFunction m_hashFunction;
    InternalToExternalConverter m_internalToExternalConverter;
    ExternalToInternalConverter m_externalToInternalConverter;
    size_t m_offset;

    AutoListAccess(const AutoListAccess& other);
public:
    AutoListAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            int elementMetaType,
            size_t elementAlign,
            bool isStaticType,
#else
            const QMetaType& elementMetaType,
#endif
            const QHashFunction& hashFunction,
            const InternalToExternalConverter& internalToExternalConverter,
            const ExternalToInternalConverter& externalToInternalConverter
            );

    void dispose() override;
    void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) override;
    AutoListAccess* clone() override;
    void* createContainer() override;
    void* copyContainer(const void* container) override;
private:
    void* constructContainer(void* result, const void* container = nullptr);
    void destructContainer(void* container);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* result, void* container);
    bool equals(const void* p1, const void* p2);
    void debugStream(QDebug &s, const void *ptr);
    void dataStreamOut(QDataStream &s, const void *ptr);
    void dataStreamIn(QDataStream &s, void *ptr);
    static void defaultCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr);
    static void copyCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, const void *other);
    static void moveCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, void *other);
    static void dtor(const QtPrivate::QMetaTypeInterface *iface, void *ptr);
    static bool equalsFn(const QtPrivate::QMetaTypeInterface *iface, const void *ptr1, const void *ptr2);
    static void debugStreamFn(const QtPrivate::QMetaTypeInterface *iface, QDebug &s, const void *ptr);
    static void dataStreamOutFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, const void *ptr);
    static void dataStreamInFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, void *ptr);
#endif
public:
    void assign(void* container, const void* other) override;
    void deleteContainer(void* container) override;
    int registerContainer(const QByteArray& containerTypeName) override;
    bool isConstant() override {return false;}
    const QMetaType& elementMetaType() override {return m_elementMetaType;}
    jobject createIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr);
    jobject end(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    void append(JNIEnv * env, void* container, jobject value) override;
    void appendList(JNIEnv * env, void* container, jobject list) override;
    jobject at(JNIEnv * env, const void* container, jint index) override;
    jobject value(JNIEnv * env, const void* container, jint index) override;
    jobject value(JNIEnv * env, const void* container, jint index, jobject defaultValue) override;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    jobject toSet(JNIEnv *,const void*) override {return nullptr;}
#endif
    void swapItemsAt(JNIEnv *, void* container, jint index1, jint index2) override;
    jboolean startsWith(JNIEnv * env, const void* container, jobject value) override;
    jint size(JNIEnv *, const void* container) override;
    void reserve(JNIEnv *, void* container, jint size) override;
    void replace(JNIEnv * env, void* container, jint index, jobject value) override;
    jboolean removeOne(JNIEnv * env, void* container, jobject value) override;
    void removeAt(JNIEnv *, void* container, jint index) override;
    jint removeAll(JNIEnv * env, void* container, jobject value) override;
    void prepend(JNIEnv * env, void* container, jobject value) override;
    jboolean equal(JNIEnv * env, const void* container, jobject other) override;
    void move(JNIEnv *, void* container, jint index1, jint index2) override;
    jobject mid(JNIEnv * env, const void* container, jint index1, jint index2) override;
    jint lastIndexOf(JNIEnv * env, const void* container, jobject value, jint index) override;
    void insert(JNIEnv * env, void* container, jint index, jobject value) override;
    jint indexOf(JNIEnv * env, const void* container, jobject value, jint index) override;
    jboolean endsWith(JNIEnv * env, const void* container, jobject value) override;
    jint count(JNIEnv * env, const void* container, jobject value) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    void clear(JNIEnv *, void* container) override;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    jint capacity(JNIEnv *, const void* container) override;
    void fill(JNIEnv * env, void* container, jobject value, jint size) override;
    void remove(JNIEnv *, void* container, jint index, jint n) override;
    void insert(JNIEnv * env, void* container, jint index, jint n, jobject value) override;
    void resize(JNIEnv *, void* container, jint newSize) override;
    void squeeze(JNIEnv *, void* container) override;
#endif
private:
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    static QtMetaContainerPrivate::QMetaSequenceInterface* createMetaSequenceInterface(int newMetaType);
    void emplace(QArrayDataPointer<char>* p, JNIEnv * env, jint index, jobject value, jint n);
    void emplace(QArrayDataPointer<char>* p, qsizetype index, const void* value, qsizetype n);
    void *createHole(QArrayDataPointer<char> *p, QArrayData::GrowthPosition pos, qsizetype where, qsizetype n);
    qsizetype freeSpaceAtBegin(const QArrayDataPointer<char>* p);
    qsizetype freeSpaceAtEnd(const QArrayDataPointer<char>* p);
    void detach(QArrayDataPointer<char>* p, QArrayDataPointer<char> *old = nullptr);
    void detachAndGrow(QArrayDataPointer<char>* p, QArrayData::GrowthPosition where, qsizetype n, const void **data,
                       QArrayDataPointer<char> *old);
    void reallocate(QArrayDataPointer<char>* p, qsizetype alloc, QArrayData::AllocationOption option);
    void reallocateAndGrow(QArrayDataPointer<char>* p, QArrayData::GrowthPosition, qsizetype n,
                           QArrayDataPointer<char> *old = nullptr);
    QArrayDataPointer<char> allocateGrow(const QArrayDataPointer<char> &from, qsizetype n, QArrayData::GrowthPosition position);
    QArrayDataPointer<char> allocate(qsizetype capacity, QArrayData::AllocationOption option = QArrayData::KeepSize);
    void swapAndDestroy(QArrayDataPointer<char>* p, QArrayDataPointer<char>&& other);
#else
    typedef struct{void* v;} Node;
    void dealloc(QListData* p);
    Node* detach_helper_grow(QListData* p, int i, int c);
    void detach(QListData* p);
    void detach_helper(QListData* p);
    void detach_helper(QListData* p, int alloc);
    void node_construct(Node *n, JNIEnv * env, jobject value);
    void node_copy(Node *from, Node *to, Node *src);
    void node_destruct(Node *node);
    void node_destruct(Node *from, Node *to);
#endif
};

typedef bool (*IsBiContainerFunction)(JNIEnv *, jobject, const QMetaType&, const QMetaType&);

class AutoMapAccess : public AbstractMapAccess{
    QMetaType m_keyMetaType;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    size_t m_keyAlign;
#endif
    QHashFunction m_keyHashFunction;
    InternalToExternalConverter m_keyInternalToExternalConverter;
    ExternalToInternalConverter m_keyExternalToInternalConverter;
    QMetaType m_valueMetaType;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    size_t m_valueAlign;
#endif
    QHashFunction m_valueHashFunction;
    InternalToExternalConverter m_valueInternalToExternalConverter;
    ExternalToInternalConverter m_valueExternalToInternalConverter;
    size_t m_align;
    size_t m_offset1;
    size_t m_offset2;
    size_t m_size;

    AutoMapAccess(const AutoMapAccess &);
public:
    ~AutoMapAccess() override;
    AutoMapAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                    int keyMetaType,
                    size_t keyAlign,
#else
                    const QMetaType& keyMetaType,
#endif
                    const QHashFunction& keyHashFunction,
                    const InternalToExternalConverter& keyInternalToExternalConverter,
                    const ExternalToInternalConverter& keyExternalToInternalConverter,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                    int valueMetaType,
                    size_t valueAlign,
#else
                    const QMetaType& valueMetaType,
#endif
                    const QHashFunction& valueHashFunction,
                    const InternalToExternalConverter& valueInternalToExternalConverter,
                    const ExternalToInternalConverter& valueExternalToInternalConverter
            );
    void* createContainer() override;
    void* copyContainer(const void* container) override;
    void assign(void* container, const void* other) override;
    void deleteContainer(void* container) override;
    int registerContainer(const QByteArray& containerTypeName) override;
    void dispose() override;
    AbstractMapAccess* clone() override;
    void analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) override;
    bool isConstant() override;
    const QMetaType& keyMetaType() override;
    const QMetaType& valueMetaType() override;
    jobject begin(JNIEnv *,QtJambiNativeID,const void*) override;
    void clear(JNIEnv *,void*) override;
    jboolean contains(JNIEnv *,const void*,jobject) override;
    jint count(JNIEnv *,const void*,jobject) override;
    jobject end(JNIEnv *,QtJambiNativeID,const void*) override;
    jobject find(JNIEnv *,QtJambiNativeID,const void*,jobject) override;
    jobject first(JNIEnv *,const void*) override;
    jobject firstKey(JNIEnv *,const void*) override;
    void insert(JNIEnv *,void*,jobject,jobject) override;
    jobject key(JNIEnv *,const void*,jobject,jobject) override;
    jobject keys(JNIEnv *,const void*) override;
    jobject keys(JNIEnv *,const void*,jobject) override;
    jobject last(JNIEnv *,const void*) override;
    jobject lastKey(JNIEnv *,const void*) override;
    jobject lowerBound(JNIEnv *,QtJambiNativeID,const void*,jobject) override;
    jboolean equal(JNIEnv *,const void*,jobject) override;
    jint remove(JNIEnv *,void*,jobject) override;
    jint size(JNIEnv *,const void*) override;
    jobject take(JNIEnv *,void*,jobject) override;
    jobject upperBound(JNIEnv *,QtJambiNativeID,const void*,jobject) override;
    jobject value(JNIEnv *,const void*,jobject,jobject) override;
    jobject values(JNIEnv *,const void*) override;
    bool keyLessThan(JNIEnv *,jobject,jobject) override;
private:
    virtual IsBiContainerFunction getIsBiContainerFunction();
    virtual uint sizeOf() const;
    virtual ushort alignOf() const;
    bool equal(const void* containerA, const void* containerB);
    void destructContainer(void* container);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    void detach(QMapDataBase ** map);
    void detach_helper(QMapDataBase ** map);
    typedef QMapNodeBase Node;
    static Node* rootNode(QMapDataBase *const* map);
    static Node* beginNode(QMapDataBase *const* map);
    static Node* endNode(QMapDataBase *const* map);
    void deleteNode(QMapDataBase * d, Node* node);
    Node* copyNode(Node* node, QMapDataBase* base);
    void destroy(QMapDataBase* data);
    void destroySubTree(Node* node);
    bool qMapLessThanKey(const void* left, const Node& right);
    bool qMapLessThanKey(const Node& left, const void* rignt);
    void assignValue(JNIEnv *,jobject, Node*);
    Node* findNode(QMapDataBase * d, const void* akey);
    Node* lowerBound(Node* node, const void* akey);
    Node* upperBound(Node* node, const void* akey);
    void nodeRange(QMapDataBase* base, const void* akey, Node **firstNode, Node **lastNode);
    jobject createIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr);
    jobject nodeKey(JNIEnv * env, Node* node);
    jobject nodeValue(JNIEnv * env, Node* node);
#endif
    friend class AutoMultiMapAccess;
};

class AutoMultiMapAccess : public virtual AbstractMultiMapAccess, public AutoMapAccess{
    AutoMultiMapAccess(const AutoMultiMapAccess&);
public:
    ~AutoMultiMapAccess() override;
    AutoMultiMapAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                    int keyMetaType,
                    size_t keyAlign,
#else
                    const QMetaType& keyMetaType,
#endif
                    const QHashFunction& keyHashFunction,
                    const InternalToExternalConverter& keyInternalToExternalConverter,
                    const ExternalToInternalConverter& keyExternalToInternalConverter,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                    int valueMetaType,
                    size_t valueAlign,
#else
                    const QMetaType& valueMetaType,
#endif
                    const QHashFunction& valueHashFunction,
                    const InternalToExternalConverter& valueInternalToExternalConverter,
                    const ExternalToInternalConverter& valueExternalToInternalConverter
            );
    void* createContainer() override;
    void* copyContainer(const void* container) override;
    void assign(void* container, const void* other) override;
    void deleteContainer(void* container) override;
    int registerContainer(const QByteArray& containerTypeName) override;
    void dispose() override;
    bool isConstant() override {return false;}
    void analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) override;
    const QMetaType& keyMetaType() override;
    const QMetaType& valueMetaType() override;
    jobject begin(JNIEnv *,QtJambiNativeID,const void*) override;
    void clear(JNIEnv *,void*) override;
    jboolean contains(JNIEnv *,const void*,jobject) override;
    jint count(JNIEnv *,const void*,jobject) override;
    jobject end(JNIEnv *,QtJambiNativeID,const void*) override;
    jobject find(JNIEnv *,QtJambiNativeID,const void*,jobject) override;
    jobject first(JNIEnv *,const void*) override;
    jobject firstKey(JNIEnv *,const void*) override;
    void insert(JNIEnv *,void*,jobject,jobject) override;
    jobject key(JNIEnv *,const void*,jobject,jobject) override;
    jobject keys(JNIEnv *,const void*) override;
    jobject keys(JNIEnv *,const void*,jobject) override;
    jobject last(JNIEnv *,const void*) override;
    jobject lastKey(JNIEnv *,const void*) override;
    jobject lowerBound(JNIEnv *,QtJambiNativeID,const void*,jobject) override;
    jboolean equal(JNIEnv *,const void*,jobject) override;
    jint remove(JNIEnv *,void*,jobject) override;
    jint size(JNIEnv *,const void*) override;
    jobject take(JNIEnv *,void*,jobject) override;
    jobject upperBound(JNIEnv *,QtJambiNativeID,const void*,jobject) override;
    jobject value(JNIEnv *,const void*,jobject,jobject) override;
    jobject values(JNIEnv *,const void*) override;
    bool keyLessThan(JNIEnv *,jobject,jobject) override;
    AbstractMultiMapAccess* clone() override;
    jobject uniqueKeys(JNIEnv *,const void*) override;
    void unite(JNIEnv *,void*,jobject) override;
    jobject values(JNIEnv *,const void*,jobject) override;
    jboolean contains(JNIEnv *,const void*,jobject,jobject) override;
    jint count(JNIEnv *,const void*,jobject,jobject) override;
    jobject find(JNIEnv *,QtJambiNativeID,const void*,jobject,jobject) override;
    jint remove(JNIEnv *,void*,jobject,jobject) override;
    void replace(JNIEnv *,void*,jobject,jobject) override;
private:
    uint sizeOf() const override;
    ushort alignOf() const override;
    IsBiContainerFunction getIsBiContainerFunction() override;
};

class AutoHashAccess : public AbstractHashAccess{
    QMetaType m_keyMetaType;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    size_t m_keyAlign;
#endif
    QHashFunction m_keyHashFunction;
    InternalToExternalConverter m_keyInternalToExternalConverter;
    ExternalToInternalConverter m_keyExternalToInternalConverter;
    QMetaType m_valueMetaType;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    size_t m_valueAlign;
#endif
    QHashFunction m_valueHashFunction;
    InternalToExternalConverter m_valueInternalToExternalConverter;
    ExternalToInternalConverter m_valueExternalToInternalConverter;
    size_t m_align;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    size_t m_offset1;
#endif
    size_t m_offset2;
    size_t m_size;

    AutoHashAccess(const AutoHashAccess&);
public:
    ~AutoHashAccess() override;
    AutoHashAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                    int keyMetaType,
                    size_t keyAlign,
#else
                    const QMetaType& keyMetaType,
#endif
                    const QHashFunction& keyHashFunction,
                    const InternalToExternalConverter& keyInternalToExternalConverter,
                    const ExternalToInternalConverter& keyExternalToInternalConverter,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                    int valueMetaType,
                    size_t valueAlign,
#else
                    const QMetaType& valueMetaType,
#endif
                    const QHashFunction& valueHashFunction,
                    const InternalToExternalConverter& valueInternalToExternalConverter,
                    const ExternalToInternalConverter& valueExternalToInternalConverter
            );
    bool isConstant() override;
    void* createContainer() override;
    void* copyContainer(const void* container) override;
    void assign(void* container, const void* other) override;
    void deleteContainer(void* container) override;
    int registerContainer(const QByteArray& containerTypeName) override;
    void dispose() override;
    AbstractHashAccess* clone() override;
    void analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) override;
    const QMetaType& keyMetaType() override;
    const QMetaType& valueMetaType() override;
    jobject begin(JNIEnv *,QtJambiNativeID,const void*) override;
    jint capacity(JNIEnv *,const void*) override;
    void clear(JNIEnv *,void*) override;
    jboolean contains(JNIEnv *,const void*,jobject) override;
    jint count(JNIEnv *,const void*,jobject) override;
    jobject end(JNIEnv *,QtJambiNativeID,const void*) override;
    jobject find(JNIEnv *,QtJambiNativeID,const void*,jobject) override;
    void insert(JNIEnv *,void*,jobject,jobject) override;
    jobject key(JNIEnv *,const void*,jobject,jobject) override;
    jobject keys(JNIEnv *,const void*) override;
    jobject keys(JNIEnv *,const void*,jobject) override;
    jboolean equal(JNIEnv *,const void*,jobject) override;
    jint remove(JNIEnv *,void*,jobject) override;
    void reserve(JNIEnv *,void*,jint) override;
    jint size(JNIEnv *,const void*) override;
    jobject take(JNIEnv *,void*,jobject) override;
    jobject value(JNIEnv *,const void*,jobject,jobject) override;
    jobject values(JNIEnv *,const void*) override;
private:
    virtual uint sizeOf() const;
    virtual ushort alignOf() const;
    virtual IsBiContainerFunction getIsBiContainerFunction();
    jobject createIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr);
    void destructContainer(void* container);
    virtual void* constructContainer(void* result, const void* container = nullptr);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    typedef QHashData::Node Node;
    static inline Node *concrete(QHashData::Node *node) {
        return reinterpret_cast<Node *>(node);
    }
    static void deleteNode2(QHashData::Node *node);
    static void duplicateNode(QHashData::Node *originalNode, void *newNode);

    struct Iterator{
        struct Value{
            AutoHashAccess* access;
            Node* i;
            Value(AutoHashAccess* _access, Node* _i);
            bool operator==(const Value &o) const;
            bool operator!=(const Value &o) const;
        };
        typedef std::forward_iterator_tag iterator_category;
        typedef qptrdiff difference_type;
        typedef Value value_type;
        typedef const Value *pointer;
        typedef const Value &reference;

        AutoHashAccess* access;
        Node* i;
        Iterator(const Iterator& it);
        Iterator(AutoHashAccess* _access, Node* _i);
        Iterator &operator++();
        Iterator operator++(int);
        Iterator &operator--();
        Iterator operator--(int);
        bool operator==(const Iterator &o) const;
        bool operator!=(const Iterator &o) const;
        const Value &operator*() const;
    };

    Iterator const_iterator(Node * node);

    int alignOfNode();
    void freeData(QHashData *d);
    void detach_helper(QHashData ** map);
    Node **findNode(QHashData *const* map, const void* akey, uint *ahp = nullptr);
    Node **findNode(QHashData *const* map, const void* akey, uint h);
    bool same_key(Node * node, const void* key0);
    bool same_key(Node * node, uint h0, const void* key0);
    Node *createNode(QHashData* d, uint ah, const void* akey, JNIEnv* env, jobject value, Node **anextNode);
    Node *createNode(QHashData* d, uint ah, const void* akey, QDataStream &s, Node **anextNode);
    Node *createNode(QHashData* d, uint ah, const void* akey, const void* avalue, Node **anextNode);
    void deleteNode(QHashData* d, Node *node);
    jobject nodeKey(JNIEnv * env, Node* node);
    jobject nodeValue(JNIEnv * env, Node* node);
    QPair<Iterator,Iterator> equal_range(QHashData *const* map, const void* akey);
    static QThreadStorage<quintptr> currentAccess;
#else
    static QtMetaContainerPrivate::QMetaAssociationInterface* createMetaAssociationInterface(int newMetaType);
    void* constructContainer(void* result, void* container);
    void debugStream(QDebug &s, const void *ptr);
    virtual void dataStreamOut(QDataStream &s, const void *ptr);
    void dataStreamIn(QDataStream &s, void *ptr);
    struct Span {
        enum {
            NEntries = 128,
            LocalBucketMask = (NEntries - 1),
            UnusedEntry = 0xff
        };
        unsigned char offsets[NEntries];
        char *entries = nullptr;
        unsigned char allocated = 0;
        unsigned char nextFree = 0;

        Span() noexcept
        {
            memset(offsets, UnusedEntry, sizeof(offsets));
        }
        ~Span()
        {
        }

        void freeData(const AutoHashAccess& access);
        char* insert(const AutoHashAccess& access, size_t i);
        void addStorage(const AutoHashAccess& access);
        size_t offset(size_t i) const noexcept;
        bool hasNode(size_t i) const noexcept;

        char* at(const AutoHashAccess& access, size_t i) noexcept;
        char* at(const AutoHashAccess& access, size_t i) const noexcept;
        char* atOffset(const AutoHashAccess& access, size_t o) noexcept;
        char* atOffset(const AutoHashAccess& access, size_t o) const noexcept;
        void erase(const AutoHashAccess& access, size_t bucket);

        void moveLocal(size_t from, size_t to) noexcept;
        void moveFromSpan(const AutoHashAccess& access, Span &fromSpan, size_t fromIndex, size_t to);
    };

    struct QHashData{
        struct iterator{
            const AutoHashAccess* access;
            struct QHashData const* d = nullptr;
            size_t bucket = 0;

            iterator(const AutoHashAccess& _access, struct QHashData const* _d = nullptr, size_t _bucket = 0)
                : access(&_access), d(_d), bucket(_bucket) {}

            iterator(const iterator& iter) : access(iter.access), d(iter.d), bucket(iter.bucket) {}

            iterator& operator=(const iterator& iter) {
                access = iter.access;
                d = iter.d;
                bucket = iter.bucket;
                return *this;
            }

            size_t span() const noexcept;
            size_t index() const noexcept;
            bool isUnused() const noexcept;
            char* node() const noexcept;
            char* key() const noexcept { return node(); };
            char* value() const noexcept { return node() + access->m_offset2; };
            bool atEnd() const noexcept;

            iterator operator++() noexcept;
            bool operator==(iterator other) const noexcept;
            inline bool operator!=(iterator other) const noexcept { return !(*this == other); }
        };

        QtPrivate::RefCount ref = {{1}};
        size_t size = 0;
        size_t numBuckets = 0;
        size_t seed = 0;
        Span *spans = nullptr;
        QHashData(const AutoHashAccess& access, size_t reserve = 0);
        QHashData(const AutoHashAccess& access, const QHashData &other, size_t reserved = 0);
        void destroy(const AutoHashAccess& access);
        static QHashData* detached(const AutoHashAccess& access, QHashData* data, size_t size = 0);

        iterator find(const AutoHashAccess& access, const void *key) const noexcept;

        size_t nextBucket(size_t bucket) const noexcept
        {
            ++bucket;
            if (bucket == numBuckets)
                bucket = 0;
            return bucket;
        }

        bool shouldGrow() const noexcept
        {
            return size >= (numBuckets >> 1);
        }

        void rehash(const AutoHashAccess& access, size_t sizeHint = 0);

        struct InsertionResult
        {
            iterator it;
            bool initialized;
        };

        InsertionResult findOrInsert(const AutoHashAccess& access, const void*key) noexcept;

        char* findNode(const AutoHashAccess& access, const void *key) const noexcept;

        iterator detachedIterator(const AutoHashAccess& access, iterator other) const noexcept;

        iterator begin(const AutoHashAccess& access) const noexcept;

        iterator end(const AutoHashAccess& access) const noexcept;

        iterator erase(const AutoHashAccess& access, iterator it);
    };

    class iterator{
        QHashData::iterator i;
        void* e = nullptr;
    public:
        iterator(const QHashData::iterator& _i);

        iterator(const iterator& iter) : i(iter.i), e(iter.e) {}

        iterator& operator=(const iterator& iter) {
            i = iter.i;
            e = iter.e;
            return *this;
        }
        iterator& operator++() noexcept;
        bool operator==(iterator other) const noexcept;
        inline bool operator!=(iterator other) const noexcept { return !(*this == other); }
        inline char* key() const noexcept {return i.key();}
        char* value() const noexcept;
        friend AutoHashAccess;
        friend class AutoMultiHashAccess;
        friend class AutoSetAccess;
    };

    virtual bool equalSpanEntries(const char* value1, const char* value2) const;
    virtual void copySpanEntry(char* value1, const char* value2) const;
    virtual void eraseSpanEntry(char* value) const;
    virtual void emplace(void* container, const void* akey, JNIEnv *env, jobject value);
    virtual void emplace(void* container, const void* akey, const void* value);
    virtual char* iteratorValue(const iterator& it) const;
    virtual iterator& incrementIterator(iterator& it) const;
    virtual void initializeIterator(iterator& it) const;
    virtual bool iteratorEquals(const iterator& it1, const iterator& it2) const;
    bool isDetached(QHashData *const* map) const noexcept;
    static void defaultCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr);
    static void copyCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, const void *other);
    static void moveCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, void *other);
    static void dtor(const QtPrivate::QMetaTypeInterface *iface, void *ptr);
    static bool equalsFn(const QtPrivate::QMetaTypeInterface *iface, const void *ptr1, const void *ptr2);
    static void debugStreamFn(const QtPrivate::QMetaTypeInterface *iface, QDebug &s, const void *ptr);
    static void dataStreamOutFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, const void *ptr);
    static void dataStreamInFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, void *ptr);
    friend iterator;
#endif
    bool equal(const void* a, const void* b);
    void detach(QHashData ** map);

    friend class AutoMultiHashAccess;
    friend class AutoSetAccess;
};

class AutoMultiHashAccess : public virtual AbstractMultiHashAccess, public AutoHashAccess{
    AutoMultiHashAccess(const AutoMultiHashAccess&);
public:
    ~AutoMultiHashAccess() override;
    AutoMultiHashAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                    int keyMetaType,
                    size_t keyAlign,
#else
                    const QMetaType& keyMetaType,
#endif
                    const QHashFunction& keyHashFunction,
                    const InternalToExternalConverter& keyInternalToExternalConverter,
                    const ExternalToInternalConverter& keyExternalToInternalConverter,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                    int valueMetaType,
                    size_t valueAlign,
#else
                    const QMetaType& valueMetaType,
#endif
                    const QHashFunction& valueHashFunction,
                    const InternalToExternalConverter& valueInternalToExternalConverter,
                    const ExternalToInternalConverter& valueExternalToInternalConverter
            );
    void* createContainer() override;
    void* copyContainer(const void* container) override;
    void assign(void* container, const void* other) override;
    void deleteContainer(void* container) override;
    int registerContainer(const QByteArray& containerTypeName) override;
    void dispose() override;
    AbstractMultiHashAccess* clone() override;
    jobject uniqueKeys(JNIEnv *,const void*) override;
    void unite(JNIEnv *,void*,jobject) override;
    bool isConstant() override;
    void analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) override;
    const QMetaType& keyMetaType() override;
    const QMetaType& valueMetaType() override;
    jobject begin(JNIEnv *,QtJambiNativeID,const void*) override;
    jint capacity(JNIEnv *,const void*) override;
    void clear(JNIEnv *,void*) override;
    jboolean contains(JNIEnv *,const void*,jobject) override;
    jint count(JNIEnv *,const void*,jobject) override;
    jobject end(JNIEnv *,QtJambiNativeID,const void*) override;
    jobject find(JNIEnv *,QtJambiNativeID,const void*,jobject) override;
    void insert(JNIEnv *,void*,jobject,jobject) override;
    jobject key(JNIEnv *,const void*,jobject,jobject) override;
    jobject keys(JNIEnv *,const void*) override;
    jobject keys(JNIEnv *,const void*,jobject) override;
    jboolean equal(JNIEnv *,const void*,jobject) override;
    jint remove(JNIEnv *,void*,jobject) override;
    void reserve(JNIEnv *,void*,jint) override;
    jint size(JNIEnv *,const void*) override;
    jobject take(JNIEnv *,void*,jobject) override;
    jobject value(JNIEnv *,const void*,jobject,jobject) override;
    jobject values(JNIEnv *,const void*) override;
    jobject values(JNIEnv *,const void*,jobject) override;
    jboolean contains(JNIEnv *,const void*,jobject,jobject) override;
    jint count(JNIEnv *,const void*,jobject,jobject) override;
    jobject find(JNIEnv *,QtJambiNativeID,const void*,jobject,jobject) override;
    jint remove(JNIEnv *,void*,jobject,jobject) override;
    void replace(JNIEnv *,void*,jobject,jobject) override;
private:
    friend class AutoHashAccess;
    uint sizeOf() const override;
    ushort alignOf() const override;
    void* constructContainer(void* result, const void* container = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    typedef AutoHashAccess::QHashData QHashData;
    struct MultiHashData{
        QHashData* d = nullptr;
        qsizetype m_size = 0;
    };
    size_t m_chainOffset;
    size_t m_chainAlign;
    size_t m_chainSize;
    struct Chain{
        char* value();
        const char* value() const;
        Chain*& next(const AutoMultiHashAccess& access);
        const Chain* next(const AutoMultiHashAccess& access) const;
        qsizetype free(const AutoMultiHashAccess& access);
        void destroy(const AutoMultiHashAccess& access);
    };
    struct multi_iterator{
        QHashData::iterator i;
        Chain** e;
    };
    void dataStreamOut(QDataStream &s, const void *ptr) override;
    bool equalSpanEntries(const char* value1, const char* value2) const override;
    void copySpanEntry(char* value1, const char* value2) const override;
    void eraseSpanEntry(char* value) const override;
    void emplace(void* container, const void* akey, JNIEnv *env, jobject value) override;
    void emplace(void* container, const void* akey, const void* value) override;
    void initializeIterator(iterator& it) const override;
    char* iteratorValue(const iterator& it) const override;
    iterator& incrementIterator(iterator& it) const override;
    bool iteratorEquals(const iterator& it1, const iterator& it2) const override;
#endif
    IsBiContainerFunction getIsBiContainerFunction() override;
};

class AutoSetAccess : public AbstractSetAccess{
    AutoHashAccess m_hashAccess;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    QSharedPointer<QtMetaContainerPrivate::QMetaSequenceInterface> m_metaSequenceInterface;
#endif
    AutoSetAccess(const AutoSetAccess&);
public:
    ~AutoSetAccess() override;
    AutoSetAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                    int elementMetaType,
                    size_t elementAlign,
#else
                    const QMetaType& elementMetaType,
#endif
                    const QHashFunction& hashFunction,
                    const InternalToExternalConverter& internalToExternalConverter,
                    const ExternalToInternalConverter& externalToInternalConverter
            );
    void* createContainer() override;
    void* constructContainer(void* result, const void* container = nullptr);
    void* copyContainer(const void* container) override;
    void assign(void* container, const void* other) override;
    void deleteContainer(void* container) override;
    int registerContainer(const QByteArray& containerTypeName) override;
    void dispose() override;
    AutoSetAccess* clone() override;
    void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) override;
    bool isConstant() override {return false;}
    const QMetaType& elementMetaType() override;
    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jint capacity(JNIEnv * env, const void* container) override;
    void clear(JNIEnv * env, void* container) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    jobject end(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    void insert(JNIEnv * env, void* container, jobject value) override;
    void intersect(JNIEnv * env, void* container, jobject other) override;
    jboolean intersects(JNIEnv * env, const void* container, jobject other) override;
    jboolean equal(JNIEnv * env, const void* container, jobject other) override;
    jboolean remove(JNIEnv * env, void* container, jobject value) override;
    void reserve(JNIEnv * env, void* container, jint newSize) override;
    jint size(JNIEnv * env, const void* container) override;
    void subtract(JNIEnv * env, void* container, jobject other) override;
    void unite(JNIEnv * env, void* container, jobject other) override;
    jobject values(JNIEnv * env, const void* container) override;
private:
    void destructContainer(void* container);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    typedef QHashData::Node Node;
#else
    void* constructContainer(void* result, void* container);
    static QtMetaContainerPrivate::QMetaSequenceInterface* createMetaSequenceInterface(int newMetaType);
    typedef AutoHashAccess::QHashData QHashData;
    typedef AutoHashAccess::iterator iterator;
#endif
};

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)

#include <QtCore/QVector>

class AutoVectorAccess : public AbstractVectorAccess{
    QMetaType m_elementMetaType;
    size_t m_elementAlign;
    QHashFunction m_hashFunction;
    InternalToExternalConverter m_internalToExternalConverter;
    ExternalToInternalConverter m_externalToInternalConverter;
    size_t m_offset;
    bool m_isComplex;

    AutoVectorAccess(const AutoVectorAccess&);
    jobject createIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr);
    void destructContainer(void* container);
    void detach(QTypedArrayData<char> ** vector);
    bool isDetached(QTypedArrayData<char> *const* vector);
    void realloc(QTypedArrayData<char> ** vector, int aalloc, QArrayData::AllocationOptions options = QArrayData::Default);
    QTypedArrayData<char>* unsharableEmpty();
    void erase(QTypedArrayData<char> ** vector, char* begin, char* end);
public:
    ~AutoVectorAccess() override;
    AutoVectorAccess(
                    int elementMetaType,
                    size_t elementAlign,
                    const QHashFunction& hashFunction,
                    const InternalToExternalConverter& internalToExternalConverter,
                    const ExternalToInternalConverter& externalToInternalConverter
            );
    void* createContainer() override;
    void* copyContainer(const void* container) override;
    void assign(void* container, const void* other) override;
    void deleteContainer(void* container) override;
    int registerContainer(const QByteArray& containerTypeName) override;
    void dispose() override;
    AutoVectorAccess* clone() override;
    void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) override;
    bool isConstant() override {return false;}
    const QMetaType& elementMetaType() override;
    void append(JNIEnv * env, void* container, jobject value) override;
    void appendVector(JNIEnv * env, void* container, jobject list) override;
    jobject at(JNIEnv * env, const void* container, jint index) override;
    jobject value(JNIEnv * env, const void* container, jint index) override;
    jobject value(JNIEnv * env, const void* container, jint index, jobject defaultValue) override;
    jobject toSet(JNIEnv *,const void*) override{return nullptr;}
    void swapItemsAt(JNIEnv * env, void* container, jint index1, jint index2) override;
    jboolean startsWith(JNIEnv * env, const void* container, jobject value) override;
    jint size(JNIEnv * env, const void* container) override;
    void reserve(JNIEnv * env, void* container, jint size) override;
    void replace(JNIEnv * env, void* container, jint index, jobject value) override;
    jboolean removeOne(JNIEnv * env, void* container, jobject value) override;
    void removeAt(JNIEnv * env, void* container, jint index) override;
    jint removeAll(JNIEnv * env, void* container, jobject value) override;
    void prepend(JNIEnv * env, void* container, jobject value) override;
    jboolean equal(JNIEnv * env, const void* container, jobject other) override;
    void move(JNIEnv * env, void* container, jint index1, jint index2) override;
    jobject mid(JNIEnv * env, const void* container, jint index1, jint index2) override;
    jint lastIndexOf(JNIEnv * env, const void* container, jobject value, jint index) override;
    void insert(JNIEnv * env, void* container, jint index, jobject value) override;
    jint indexOf(JNIEnv * env, const void* container, jobject value, jint index) override;
    jboolean endsWith(JNIEnv * env, const void* container, jobject value) override;
    jobject end(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jint count(JNIEnv * env, const void* container, jobject value) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    void clear(JNIEnv * env, void* container) override;
    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jint capacity(JNIEnv * env, const void* container) override;
    void fill(JNIEnv * env, void* container, jobject value, jint size) override;
    void remove(JNIEnv * env, void* container, jint index, jint n) override;
    void insert(JNIEnv * env, void* container, jint index, jint n, jobject value) override;
    void resize(JNIEnv * env, void* container, jint newSize) override;
    void squeeze(JNIEnv * env, void* container) override;
};

#include <QtCore/QLinkedList>

class AutoLinkedListAccess : public AbstractLinkedListAccess{
    QMetaType m_elementMetaType;
    QHashFunction m_hashFunction;
    InternalToExternalConverter m_internalToExternalConverter;
    ExternalToInternalConverter m_externalToInternalConverter;
    size_t m_offset;

    AutoLinkedListAccess(const AutoLinkedListAccess& other);
public:
    AutoLinkedListAccess(
            int elementMetaType,
            size_t elementAlign,
            const QHashFunction& hashFunction,
            const InternalToExternalConverter& internalToExternalConverter,
            const ExternalToInternalConverter& externalToInternalConverter
            );
    AutoLinkedListAccess* clone() override;
    void* createContainer() override;
    void* copyContainer(const void* container) override;
    void assign(void* container, const void* other) override;
    void deleteContainer(void* container) override;
    int registerContainer(const QByteArray& containerTypeName) override;
    bool isConstant() override {return false;}
    void dispose() override;
    void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) override;
    const QMetaType& elementMetaType() override;
    void append(JNIEnv * env, void* container, jobject value) override;
    jobject first(JNIEnv * env, const void* container) override;
    jobject last(JNIEnv * env, const void* container) override;
    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    void clear(JNIEnv * env, void* container) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    jint count(JNIEnv * env, const void* container, jobject value) override;
    jobject end(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jboolean endsWith(JNIEnv * env, const void* container, jobject value) override;
    jboolean equal(JNIEnv * env, const void* container, jobject other) override;
    void prepend(JNIEnv * env, void* container, jobject value) override;
    void removeFirst(JNIEnv * env, void* container) override;
    jint removeAll(JNIEnv * env, void* container, jobject value) override;
    void removeLast(JNIEnv * env, void* container) override;
    jboolean removeOne(JNIEnv * env, void* container, jobject value) override;
    jint size(JNIEnv * env, const void* container) override;
    jboolean startsWith(JNIEnv * env, const void* container, jobject value) override;
    jobject takeFirst(JNIEnv * env, void* container) override;
    jobject takeLast(JNIEnv * env, void* container) override;
private:
    typedef QLinkedListNode<char> Node;
    void destructContainer(void* container);
    jobject createIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr);
    void detach(QLinkedListData*& d);
    void detach_helper(QLinkedListData*& d);
    QLinkedList<char>::iterator detach_helper2(QLinkedListData*& d, QLinkedList<char>::iterator iter);
    void freeData(QLinkedListData*& v);
    void erase(QLinkedListData*& d, Node *i);
};

#endif

#endif // QTJAMBI_CONTAINERACCESS_P_H
