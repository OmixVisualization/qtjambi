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


#ifndef CONTAINERACCESS_P_H
#define CONTAINERACCESS_P_H

#include <QtCore/QtGlobal>
#include <QtCore/QThreadStorage>

QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wextra")
QT_WARNING_DISABLE_CLANG("-Wextra")
QT_WARNING_DISABLE_GCC("-Winaccessible-base")
QT_WARNING_DISABLE_CLANG("-Winaccessible-base")

#include "containeraccess.h"
#include "jobjectwrapper.h"
#include "utils_p.h"

void registerContainerConverter(SequentialContainerType collectionType, const QMetaType& containerMetaType, const QMetaType& elementMetaType);
void registerContainerConverter(AssociativeContainerType mapType, const QMetaType& containerMetaType, const QMetaType& keyMetaType, const QMetaType& valueMetaType);
void registerContainerConverter(QSharedPointer<AbstractPairAccess> pairAccess, const QMetaType& containerMetaType);

#if defined(QTJAMBI_GENERIC_ACCESS)
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
#define CHECK_CONTAINER_ACCESS(env, containerAccess) containerAccess = checkContainerAccess(env, containerAccess);
#else
#define CHECK_CONTAINER_ACCESS(env, containerAccess)
#endif //defined(QTJAMBI_GENERIC_ACCESS)

void containerDisposer(AbstractContainerAccess* _access);

int registerContainerMetaType(const QByteArray& typeName,
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
                     QMetaType::Destructor destructor,
                     QMetaType::Constructor constructor,
#elif QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
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
                     AfterRegistrationFunction afterRegistrationFunction,
                     const QSharedPointer<AbstractContainerAccess>& sharedAccess);
QSharedPointer<AbstractContainerAccess> findContainerAccess(int metaType);

void insertHashFunctionByMetaType(int type, const QtJambiUtils::QHashFunction& fct);

void insertHashFunctionByMetaType(int type, QtJambiUtils::QHashFunction&& fct);

class AbstractWrapperContainerAccess{
protected:
    AbstractWrapperContainerAccess() = default;
    virtual ~AbstractWrapperContainerAccess();
    Q_DISABLE_COPY_MOVE(AbstractWrapperContainerAccess)
public:
    virtual AbstractContainerAccess* wrappedAccess() = 0;
};

class WrapperListAccess : public AbstractListAccess, public AbstractWrapperContainerAccess {
public:
    WrapperListAccess(AbstractListAccess* containerAccess);
    ~WrapperListAccess() override;
    bool isDetached(const void* container) final override;
    void detach(const ContainerInfo& container) final override;
    bool isSharedWith(const void* container, const void* container2) final override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    bool append(void* container, const void* entry) override;
    AbstractListAccess* clone() override;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    AbstractSpanAccess* createSpanAccess(bool isConst) override;
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    void dispose() final override;
    size_t sizeOf() final override;
    void* constructContainer(void* placement) final override;
    void* constructContainer(void* placement, const void* container) override;
    void* constructContainer(JNIEnv * env, void* placement, const ConstContainerAndAccessInfo& copyOf) override;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    void* constructContainer(void* placement, void* move) override;
    void* constructContainer(JNIEnv * env, void* placement, const ContainerAndAccessInfo& move) override;
#endif
    bool destructContainer(void* container) final override;
    void assign(void*, const void* ) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    int registerContainer(const QByteArray& containerTypeName) final override;
    const QMetaType& elementMetaType() final override;
    DataType elementType() override;
    AbstractContainerAccess* elementNestedContainerAccess() override;
    bool hasNestedContainerAccess() override;
    bool hasNestedPointers() override;
    const QObject* getOwner(const void* container) override;
    bool hasOwnerFunction() override;
    void appendList(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) override;
    jobject at(JNIEnv * env, const void* container, jint index) override;
    const void* at(const void* container, qsizetype index) override;
    void* at(void* container, qsizetype index) override;
    jobject value(JNIEnv * env, const void* container, jint index) override;
    jobject value(JNIEnv * env, const void* container, jint index, jobject defaultValue) override;
    void swapItemsAt(JNIEnv * env, const ContainerInfo& container, jint index1, jint index2) override;
    jboolean startsWith(JNIEnv * env, const void* container, jobject value) override;
    jint size(JNIEnv * env, const void* container) override;
    qsizetype size(const void* container)override;
    void reserve(JNIEnv * env, const ContainerInfo& container, jint size) override;
    void replace(JNIEnv * env, const ContainerInfo& container, jint index, jobject value) override;
    void replace(void* container, qsizetype index, const void* value) override;
    jint removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    jboolean equal(JNIEnv * env, const void* container, jobject other) override;
    void move(JNIEnv * env, const ContainerInfo& container, jint index1, jint index2) override;
    ContainerAndAccessInfo mid(JNIEnv * env, const ConstContainerAndAccessInfo& container, jint index1, jint index2) override;
    jint lastIndexOf(JNIEnv * env, const void* container, jobject value, jint index) override;
    jint indexOf(JNIEnv * env, const void* container, jobject value, jint index) override;
    jboolean endsWith(JNIEnv * env, const void* container, jobject value) override;
    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jint count(JNIEnv * env, const void* container, jobject value) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void remove(JNIEnv * env, const ContainerInfo& container, jint index, jint n) override;
    void remove(void* container, qsizetype pos, qsizetype n) override;
    void insert(JNIEnv * env, const ContainerInfo& container, jint index, jint n, jobject value) override;
    void insert(void* container, qsizetype index, qsizetype n, const void* entry) override;

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    void resize(void* container, qsizetype newSize) override;
    jint capacity(JNIEnv * env, const void* container) override;
    void fill(JNIEnv * env, const ContainerInfo& container, jobject value, jint size) override;
    void resize(JNIEnv * env, const ContainerInfo& container, jint newSize) override;
    void squeeze(JNIEnv * env, const ContainerInfo& container) override;
#endif
    std::unique_ptr<ElementIterator> elementIterator(const void* container) final override;
    std::unique_ptr<ElementIterator> elementIterator(void* container) final override;
    AbstractListAccess* wrappedAccess() override;
    Q_DISABLE_COPY_MOVE(WrapperListAccess)
private:
    AbstractListAccess* m_containerAccess;
};

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
class WrapperVectorAccess : public AbstractVectorAccess, public AbstractWrapperContainerAccess {
public:
    WrapperVectorAccess(AbstractVectorAccess* containerAccess);
    ~WrapperVectorAccess() override;
    AbstractVectorAccess* clone() override;
    void dispose() override;
    bool isDetached(const void* container) final override;
    void detach(const ContainerInfo& container) final override;
    bool isSharedWith(const void* container, const void* container2) final override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(void*, const void* ) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    size_t sizeOf() final override;
    void* constructContainer(void* placement) final override;
    void* constructContainer(void* placement, const void* container) override;
    void* constructContainer(JNIEnv * env, void* placement, const ConstContainerAndAccessInfo& copyOf) override;
    bool destructContainer(void* container) final override;
    int registerContainer(const QByteArray& containerTypeName) override;
    const QMetaType& elementMetaType() override;
    DataType elementType() override;
    AbstractContainerAccess* elementNestedContainerAccess() override;
    bool hasNestedContainerAccess() override;
    bool hasNestedPointers() override;
    const QObject* getOwner(const void* container) override;
    bool hasOwnerFunction() override;
    void appendVector(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) override;
    jobject at(JNIEnv * env, const void* container, jint index) override;
    jobject value(JNIEnv * env, const void* container, jint index) override;
    jobject value(JNIEnv * env, const void* container, jint index, jobject defaultValue) override;
    void swapItemsAt(JNIEnv * env, const ContainerInfo& container, jint index1, jint index2) override;
    jboolean startsWith(JNIEnv * env, const void* container, jobject value) override;
    jint size(JNIEnv * env, const void* container) override;
    void reserve(JNIEnv * env, const ContainerInfo& container, jint size) override;
    void replace(JNIEnv * env, const ContainerInfo& container, jint index, jobject value) override;
    jint removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    jboolean equal(JNIEnv * env, const void* container, jobject other) override;
    void move(JNIEnv * env, const ContainerInfo& container, jint index1, jint index2) override;
    ContainerAndAccessInfo mid(JNIEnv * env, const ConstContainerAndAccessInfo& container, jint index1, jint index2) override;
    jint lastIndexOf(JNIEnv * env, const void* container, jobject value, jint index) override;
    jint indexOf(JNIEnv * env, const void* container, jobject value, jint index) override;
    jboolean endsWith(JNIEnv * env, const void* container, jobject value) override;
    jint count(JNIEnv * env, const void* container, jobject value) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jint capacity(JNIEnv * env, const void* container) override;
    void fill(JNIEnv * env, const ContainerInfo& container, jobject value, jint size) override;
    void remove(JNIEnv * env, const ContainerInfo& container, jint index, jint n) override;
    void insert(JNIEnv * env, const ContainerInfo& container, jint index, jint n, jobject value) override;
    void resize(JNIEnv * env, const ContainerInfo& container, jint newSize) override;
    void squeeze(JNIEnv * env, const ContainerInfo& container) override;
    Q_DISABLE_COPY_MOVE(WrapperVectorAccess)
    std::unique_ptr<ElementIterator> elementIterator(const void* container) final override;
    std::unique_ptr<ElementIterator> elementIterator(void* container) final override;
    AbstractVectorAccess* wrappedAccess() override;
private:
    AbstractVectorAccess* m_containerAccess;
};

class WrapperLinkedListAccess : public AbstractLinkedListAccess, public AbstractWrapperContainerAccess {
public:
    WrapperLinkedListAccess(AbstractLinkedListAccess* containerAccess);
    ~WrapperLinkedListAccess() override;
    AbstractLinkedListAccess* clone() override;
    void dispose() override;
    const QObject* getOwner(const void* container) override;
    bool hasOwnerFunction() override;
    bool isDetached(const void* container) final override;
    void detach(const ContainerInfo& container) final override;
    bool isSharedWith(const void* container, const void* container2) final override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(void*, const void* ) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    size_t sizeOf() final override;
    void* constructContainer(void* placement) final override;
    void* constructContainer(void* placement, const void* container) override;
    void* constructContainer(JNIEnv * env, void* placement, const ConstContainerAndAccessInfo& copyOf) override;
    bool destructContainer(void* container) final override;
    int registerContainer(const QByteArray& containerTypeName) override;
    const QMetaType& elementMetaType() override;
    DataType elementType() override;
    AbstractContainerAccess* elementNestedContainerAccess() override;
    bool hasNestedContainerAccess() override;
    bool hasNestedPointers() override;
    void append(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    jobject first(JNIEnv * env, const void* container) override;
    jobject last(JNIEnv * env, const void* container) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    jint count(JNIEnv * env, const void* container, jobject value) override;
    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jboolean endsWith(JNIEnv * env, const void* container, jobject value) override;
    jboolean equal(JNIEnv * env, const void* container, jobject other) override;
    void prepend(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void removeFirst(JNIEnv * env, const ContainerInfo& container) override;
    jint removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void removeLast(JNIEnv * env, const ContainerInfo& container) override;
    jboolean removeOne(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    jint size(JNIEnv * env, const void* container) override;
    jboolean startsWith(JNIEnv * env, const void* container, jobject value) override;
    jobject takeFirst(JNIEnv * env, const ContainerInfo& container) override;
    jobject takeLast(JNIEnv * env, const ContainerInfo& container) override;
    std::unique_ptr<ElementIterator> elementIterator(const void* container) final override;
    std::unique_ptr<ElementIterator> elementIterator(void* container) final override;
    AbstractLinkedListAccess* wrappedAccess() override;
    Q_DISABLE_COPY_MOVE(WrapperLinkedListAccess)
private:
    AbstractLinkedListAccess* m_containerAccess;
};
#endif

class WrapperSetAccess : public AbstractSetAccess, public AbstractWrapperContainerAccess {
public:
    WrapperSetAccess(AbstractSetAccess* containerAccess);
    ~WrapperSetAccess() override;
    AbstractSetAccess* clone() override;
    void dispose() override;
    bool isDetached(const void* container) final override;
    void detach(const ContainerInfo& container) final override;
    bool isSharedWith(const void* container, const void* container2) final override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(void*, const void* ) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    size_t sizeOf() final override;
    void* constructContainer(void* placement) final override;
    void* constructContainer(void* placement, const void* container) override;
    void* constructContainer(JNIEnv * env, void* placement, const ConstContainerAndAccessInfo& copyOf) override;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    void* constructContainer(void* placement, void* move) override;
    void* constructContainer(JNIEnv * env, void* placement, const ContainerAndAccessInfo& move) override;
#endif
    bool destructContainer(void* container) final override;
    int registerContainer(const QByteArray& containerTypeName) override;
    const QMetaType& elementMetaType() override;
    DataType elementType() override;
    AbstractContainerAccess* elementNestedContainerAccess() override;
    bool hasNestedContainerAccess() override;
    bool hasNestedPointers() override;
    const QObject* getOwner(const void* container) override;
    bool hasOwnerFunction() override;
    jint capacity(JNIEnv * env, const void* container) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void intersect(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    jboolean intersects(JNIEnv * env, const void* container, jobject other) override;
    jboolean equal(JNIEnv * env, const void* container, jobject other) override;
    jboolean remove(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void reserve(JNIEnv * env, const ContainerInfo& container, jint newSize) override;
    jint size(JNIEnv * env, const void* container) override;
    qsizetype size(const void* container) override;
    void subtract(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    ContainerAndAccessInfo values(JNIEnv * env, const ConstContainerInfo& container) override;
    std::unique_ptr<ElementIterator> elementIterator(const void* container) final override;
    std::unique_ptr<ElementIterator> elementIterator(void* container) final override;
    AbstractSetAccess* wrappedAccess() override;
    Q_DISABLE_COPY_MOVE(WrapperSetAccess)
private:
    AbstractSetAccess* m_containerAccess;
};

class WrapperMapAccess : public AbstractMapAccess, public AbstractWrapperContainerAccess {
public:
    WrapperMapAccess(AbstractMapAccess* containerAccess);
    ~WrapperMapAccess() override;
    AbstractMapAccess* clone() override;
    void dispose() override;
    void assign(void*, const void* ) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    bool isDetached(const void* container) final override;
    void detach(const ContainerInfo& container) final override;
    bool isSharedWith(const void* container, const void* container2) final override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    size_t sizeOf() final override;
    void* constructContainer(void* placement) final override;
    void* constructContainer(void* placement, const void* container) override;
    void* constructContainer(JNIEnv * env, void* placement, const ConstContainerAndAccessInfo& copyOf) override;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    void* constructContainer(void* placement, void* move) override;
    void* constructContainer(JNIEnv * env, void* placement, const ContainerAndAccessInfo& move) override;
#endif
    bool destructContainer(void* container) final override;
    int registerContainer(const QByteArray& containerTypeName) override;
    const QMetaType& keyMetaType() override;
    const QMetaType& valueMetaType() override;
    DataType keyType() override;
    DataType valueType() override;
    AbstractContainerAccess* keyNestedContainerAccess() override;
    AbstractContainerAccess* valueNestedContainerAccess() override;
    bool hasKeyNestedContainerAccess() override;
    bool hasValueNestedContainerAccess() override;
    bool hasKeyNestedPointers() override;
    bool hasValueNestedPointers() override;
    const QObject* getOwner(const void* container) override;
    bool hasOwnerFunction() override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    jint count(JNIEnv * env, const void* container, jobject key) override;
    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key) override;
    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override;
    jobject first(JNIEnv * env, const void* container) override;
    jobject firstKey(JNIEnv * env, const void* container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override;
    ContainerAndAccessInfo keys(JNIEnv * env, const ConstContainerInfo& container) override;
    ContainerAndAccessInfo keys(JNIEnv * env, const ConstContainerInfo& container, jobject value) override;
    jobject last(JNIEnv * env, const void* container) override;
    jobject lastKey(JNIEnv * env, const void* container) override;
    jobject constLowerBound(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override;
    jboolean equal(JNIEnv * env, const void* container, jobject other) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jint size(JNIEnv * env, const void* container) override;
    qsizetype size(const void* container)override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    jobject constUpperBound(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override;
    jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override;
    ContainerAndAccessInfo values(JNIEnv * env, const ConstContainerInfo& container) override;
    bool keyLessThan(JNIEnv *env, jobject key1, jobject key2) override;
    std::unique_ptr<KeyValueIterator> keyValueIterator(const void* container) final override;
    std::unique_ptr<KeyValueIterator> keyValueIterator(void* container) override;
    bool contains(const void*,const void*) override;
    void insert(void* container,const void* key, const void* value) override;
    const void* value(const void*, const void*, const void*) override;
    AbstractMapAccess* wrappedAccess() override;
    Q_DISABLE_COPY_MOVE(WrapperMapAccess)
private:
    AbstractMapAccess* m_containerAccess;
};

class WrapperMultiMapAccess : public AbstractMultiMapAccess, public AbstractWrapperContainerAccess {
public:
    WrapperMultiMapAccess(AbstractMultiMapAccess* containerAccess);
    ~WrapperMultiMapAccess() override;
    AbstractMultiMapAccess* clone() override;
    void dispose() override;
    bool isDetached(const void* container) final override;
    void detach(const ContainerInfo& container) final override;
    bool isSharedWith(const void* container, const void* container2) final override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(void*, const void* ) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    size_t sizeOf() final override;
    void* constructContainer(void* placement) final override;
    void* constructContainer(void* placement, const void* container) override;
    void* constructContainer(JNIEnv * env, void* placement, const ConstContainerAndAccessInfo& copyOf) override;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    void* constructContainer(void* placement, void* move) override;
    void* constructContainer(JNIEnv * env, void* placement, const ContainerAndAccessInfo& move) override;
#endif
    bool destructContainer(void* container) final override;
    int registerContainer(const QByteArray& containerTypeName) override;
    const QMetaType& keyMetaType() override;
    const QMetaType& valueMetaType() override;
    DataType keyType() override;
    DataType valueType() override;
    AbstractContainerAccess* keyNestedContainerAccess() override;
    AbstractContainerAccess* valueNestedContainerAccess() override;
    bool hasKeyNestedContainerAccess() override;
    bool hasValueNestedContainerAccess() override;
    bool hasKeyNestedPointers() override;
    bool hasValueNestedPointers() override;
    const QObject* getOwner(const void* container) override;
    bool hasOwnerFunction() override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    jint count(JNIEnv * env,const void* container, jobject key) override;
    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key) override;
    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override;
    jobject first(JNIEnv * env, const void* container) override;
    jobject firstKey(JNIEnv * env, const void* container) override;
    void insert(JNIEnv *env, const ContainerInfo& container,jobject key,jobject value) override;
    jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override;
    ContainerAndAccessInfo keys(JNIEnv * env, const ConstContainerInfo& container) override;
    ContainerAndAccessInfo keys(JNIEnv * env, const ConstContainerInfo& container, jobject value) override;
    jobject last(JNIEnv * env, const void* container) override;
    jobject lastKey(JNIEnv * env, const void* container) override;
    jobject constLowerBound(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override;
    jboolean equal(JNIEnv * env, const void* container, jobject other) override;
    jint remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) override;
    jint size(JNIEnv * env, const void* container) override;
    qsizetype size(const void* container)override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    jobject constUpperBound(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override;
    jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override;
    ContainerAndAccessInfo values(JNIEnv * env, const ConstContainerInfo& container) override;
    bool keyLessThan(JNIEnv *env, jobject key1, jobject key2) override;
    ContainerAndAccessInfo uniqueKeys(JNIEnv * env, const ConstContainerInfo& container) override;
    void unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    ContainerAndAccessInfo values(JNIEnv * env, const ConstContainerInfo& container, jobject key) override;
    jboolean contains(JNIEnv * env, const void* container, jobject key, jobject value) override;
    jint count(JNIEnv *env, const void* container, jobject key, jobject value) override;
    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key, jobject value) override;
    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key, jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject value) override;
    std::unique_ptr<AbstractMapAccess::KeyValueIterator> keyValueIterator(const void* container) override;
    std::unique_ptr<AbstractMapAccess::KeyValueIterator> keyValueIterator(void* container) override;
    bool contains(const void*,const void*) override;
    void insert(void* container,const void* key, const void* value) override;
    const void* value(const void*, const void*, const void*) override;
    AbstractMultiMapAccess* wrappedAccess() override;
    Q_DISABLE_COPY_MOVE(WrapperMultiMapAccess)
private:
    AbstractMultiMapAccess* m_containerAccess;
};

class WrapperHashAccess : public AbstractHashAccess, public AbstractWrapperContainerAccess {
public:
    WrapperHashAccess(AbstractHashAccess* containerAccess);
    ~WrapperHashAccess() override;
    AbstractHashAccess* clone() override;
    void dispose() override;
    bool isDetached(const void* container) final override;
    void detach(const ContainerInfo& container) final override;
    bool isSharedWith(const void* container, const void* container2) final override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    size_t sizeOf() final override;
    void* constructContainer(void* placement) final override;
    void* constructContainer(void* placement, const void* container) override;
    void* constructContainer(JNIEnv * env, void* placement, const ConstContainerAndAccessInfo& copyOf) override;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    void* constructContainer(void* placement, void* move) override;
    void* constructContainer(JNIEnv * env, void* placement, const ContainerAndAccessInfo& move) override;
#endif
    bool destructContainer(void* container) final override;
    void assign(void*, const void* ) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    int registerContainer(const QByteArray& containerTypeName) override;
    const QMetaType& keyMetaType() override;
    const QMetaType& valueMetaType() override;
    DataType keyType() override;
    DataType valueType() override;
    AbstractContainerAccess* keyNestedContainerAccess() override;
    AbstractContainerAccess* valueNestedContainerAccess() override;
    bool hasKeyNestedContainerAccess() override;
    bool hasValueNestedContainerAccess() override;
    bool hasKeyNestedPointers() override;
    bool hasValueNestedPointers() override;
    const QObject* getOwner(const void* container) override;
    bool hasOwnerFunction() override;
    jint capacity(JNIEnv * env,const void* container) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    jint count(JNIEnv * env, const void* container, jobject key) override;
    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key) override;
    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override;
    void insert(JNIEnv *env, const ContainerInfo& container,jobject key,jobject value) override;
    jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override;
    ContainerAndAccessInfo keys(JNIEnv * env, const ConstContainerInfo& container) override;
    ContainerAndAccessInfo keys(JNIEnv * env, const ConstContainerInfo& container, jobject value) override;
    jboolean equal(JNIEnv * env, const void* container, jobject other) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    void reserve(JNIEnv * env, const ContainerInfo& container, jint newSize) override;
    jint size(JNIEnv * env, const void* container) override;
    qsizetype size(const void* container)override;
    jobject take(JNIEnv *env, const ContainerInfo& container, jobject key) override;
    jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override;
    ContainerAndAccessInfo values(JNIEnv * env, const ConstContainerInfo& container) override;
    bool contains(const void*,const void*) override;
    void insert(void* container,const void* key, const void* value) override;
    const void* value(const void*, const void*, const void*) override;
    std::unique_ptr<KeyValueIterator> keyValueIterator(const void* container) final override;
    std::unique_ptr<KeyValueIterator> keyValueIterator(void* container) override;
    AbstractHashAccess* wrappedAccess() override;
    Q_DISABLE_COPY_MOVE(WrapperHashAccess)
private:
    AbstractHashAccess* m_containerAccess;
};

class WrapperMultiHashAccess : public AbstractMultiHashAccess, public AbstractWrapperContainerAccess {
public:
    WrapperMultiHashAccess(AbstractMultiHashAccess* containerAccess);
    ~WrapperMultiHashAccess() override;
    AbstractMultiHashAccess* clone() override;
    bool isDetached(const void* container) final override;
    void detach(const ContainerInfo& container) final override;
    bool isSharedWith(const void* container, const void* container2) final override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void dispose() override;
    void assign(void*, const void* ) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    size_t sizeOf() final override;
    void* constructContainer(void* placement) final override;
    void* constructContainer(void* placement, const void* container) override;
    void* constructContainer(JNIEnv * env, void* placement, const ConstContainerAndAccessInfo& copyOf) override;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    void* constructContainer(void* placement, void* move) override;
    void* constructContainer(JNIEnv * env, void* placement, const ContainerAndAccessInfo& move) override;
#endif
    bool destructContainer(void* container) final override;
    int registerContainer(const QByteArray& containerTypeName) override;
    const QMetaType& keyMetaType() override;
    const QMetaType& valueMetaType() override;
    DataType keyType() override;
    DataType valueType() override;
    AbstractContainerAccess* keyNestedContainerAccess() override;
    AbstractContainerAccess* valueNestedContainerAccess() override;
    bool hasKeyNestedContainerAccess() override;
    bool hasValueNestedContainerAccess() override;
    bool hasKeyNestedPointers() override;
    bool hasValueNestedPointers() override;
    const QObject* getOwner(const void* container) override;
    bool hasOwnerFunction() override;
    jint capacity(JNIEnv * env,const void* container) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    jint count(JNIEnv * env, const void* container, jobject key) override;
    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key) override;
    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override;
    void insert(JNIEnv *env, const ContainerInfo& container,jobject key,jobject value) override;
    jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override;
    ContainerAndAccessInfo keys(JNIEnv * env, const ConstContainerInfo& container) override;
    ContainerAndAccessInfo keys(JNIEnv * env, const ConstContainerInfo& container, jobject value) override;
    jboolean equal(JNIEnv * env, const void* container, jobject other) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    void reserve(JNIEnv * env, const ContainerInfo& container, jint newSize) override;
    jint size(JNIEnv * env, const void* container) override;
    qsizetype size(const void* container)override;
    bool contains(const void*,const void*) override;
    void insert(void* container,const void* key, const void* value) override;
    const void* value(const void*, const void*, const void*) override;
    jobject take(JNIEnv *env, const ContainerInfo& container, jobject key) override;
    jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override;
    ContainerAndAccessInfo values(JNIEnv * env, const ConstContainerInfo& container) override;
    ContainerAndAccessInfo values(JNIEnv * env, const ConstContainerInfo& container, jobject key) override;
    jboolean contains(JNIEnv * env, const void* container, jobject key, jobject value) override;
    jint count(JNIEnv *env, const void* container, jobject key, jobject value) override;
    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key, jobject value) override;
    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key, jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) override;
    void replace(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) override;
    ContainerAndAccessInfo uniqueKeys(JNIEnv * env, const ConstContainerInfo& container) override;
    void unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    std::unique_ptr<AbstractHashAccess::KeyValueIterator> keyValueIterator(const void* container) override;
    std::unique_ptr<AbstractHashAccess::KeyValueIterator> keyValueIterator(void* container) override;
    AbstractMultiHashAccess* wrappedAccess() override;
    Q_DISABLE_COPY_MOVE(WrapperMultiHashAccess)
private:
    AbstractMultiHashAccess* m_containerAccess;
};

class PointerRCListAccess : public WrapperListAccess, public ReferenceCountingSetContainer{
private:
    PointerRCListAccess(PointerRCListAccess& _this);
public:
    ~PointerRCListAccess() override;
    PointerRCListAccess(AbstractListAccess* containerAccess);
    PointerRCListAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void appendList(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) override;
    void replace(JNIEnv * env, const ContainerInfo& container, jint index, jobject value) override;
    jint removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void remove(JNIEnv * env, const ContainerInfo& container, jint index, jint n) override;
    void insert(JNIEnv * env, const ContainerInfo& container, jint index, jint n, jobject value) override;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    void fill(JNIEnv * env, const ContainerInfo& container, jobject value, jint size) override;
#endif
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
};

class PointerRCSetAccess : public WrapperSetAccess, public ReferenceCountingSetContainer{
private:
    PointerRCSetAccess(PointerRCSetAccess& _this);
public:
    ~PointerRCSetAccess() override;
    PointerRCSetAccess(AbstractSetAccess* containerAccess);
    PointerRCSetAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void insert(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    jboolean remove(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void intersect(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void subtract(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
};

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
class PointerRCLinkedListAccess : public WrapperLinkedListAccess, public ReferenceCountingSetContainer{
private:
    PointerRCLinkedListAccess(PointerRCLinkedListAccess& _this);
public:
    ~PointerRCLinkedListAccess() override;
    PointerRCLinkedListAccess(AbstractLinkedListAccess* containerAccess);
    PointerRCLinkedListAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void append(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void prepend(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void removeFirst(JNIEnv * env, const ContainerInfo& container) override;
    void removeLast(JNIEnv * env, const ContainerInfo& container) override;
    jint removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    jboolean removeOne(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    jobject takeFirst(JNIEnv * env, const ContainerInfo& container) override;
    jobject takeLast(JNIEnv * env, const ContainerInfo& container) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
};

class PointerRCVectorAccess : public WrapperVectorAccess, public ReferenceCountingSetContainer{
private:
    PointerRCVectorAccess(PointerRCVectorAccess& _this);
public:
    ~PointerRCVectorAccess() override;
    PointerRCVectorAccess(AbstractVectorAccess* containerAccess);
    PointerRCVectorAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void appendVector(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) override;
    void replace(JNIEnv * env, const ContainerInfo& container, jint index, jobject value) override;
    jint removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void remove(JNIEnv * env, const ContainerInfo& container, jint index, jint n) override;
    void insert(JNIEnv * env, const ContainerInfo& container, jint index, jint n, jobject value) override;
    void fill(JNIEnv * env, const ContainerInfo& container, jobject value, jint size) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
};
#endif

class KeyPointerRCMapAccess : public WrapperMapAccess, public ReferenceCountingSetContainer{
private:
    KeyPointerRCMapAccess(KeyPointerRCMapAccess& _this);
public:
    ~KeyPointerRCMapAccess() override;
    KeyPointerRCMapAccess(AbstractMapAccess* containerAccess);
    KeyPointerRCMapAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
};

class KeyPointerRCMultiMapAccess : public WrapperMultiMapAccess, public ReferenceCountingSetContainer{
private:
    KeyPointerRCMultiMapAccess(KeyPointerRCMultiMapAccess& _this);
public:
    ~KeyPointerRCMultiMapAccess() override;
    KeyPointerRCMultiMapAccess(AbstractMultiMapAccess* containerAccess);
    KeyPointerRCMultiMapAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    jint remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) override;
    void unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject value) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
};

class KeyPointerRCHashAccess : public WrapperHashAccess, public ReferenceCountingSetContainer{
private:
    KeyPointerRCHashAccess(KeyPointerRCHashAccess& _this);
public:
    ~KeyPointerRCHashAccess() override;
    KeyPointerRCHashAccess(AbstractHashAccess* containerAccess);
    KeyPointerRCHashAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
};

class KeyPointerRCMultiHashAccess : public WrapperMultiHashAccess, public ReferenceCountingSetContainer{
private:
    KeyPointerRCMultiHashAccess(KeyPointerRCMultiHashAccess& _this);
public:
    ~KeyPointerRCMultiHashAccess() override;
    KeyPointerRCMultiHashAccess(AbstractMultiHashAccess* containerAccess);
    KeyPointerRCMultiHashAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    jint remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) override;
    void unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject value) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
};

class ValuePointerRCMapAccess : public WrapperMapAccess, public ReferenceCountingSetContainer{
private:
    ValuePointerRCMapAccess(ValuePointerRCMapAccess& _this);
public:
    ~ValuePointerRCMapAccess() override;
    ValuePointerRCMapAccess(AbstractMapAccess* containerAccess);
    ValuePointerRCMapAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
};

class ValuePointerRCMultiMapAccess : public WrapperMultiMapAccess, public ReferenceCountingSetContainer{
private:
    ValuePointerRCMultiMapAccess(ValuePointerRCMultiMapAccess& _this);
public:
    ~ValuePointerRCMultiMapAccess() override = default;
    ValuePointerRCMultiMapAccess(AbstractMultiMapAccess* containerAccess);
    ValuePointerRCMultiMapAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    jint remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) override;
    void unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject value) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
};

class ValuePointerRCHashAccess : public WrapperHashAccess, public ReferenceCountingSetContainer{
private:
    ValuePointerRCHashAccess(ValuePointerRCHashAccess& _this);
public:
    ~ValuePointerRCHashAccess() override = default;
    ValuePointerRCHashAccess(AbstractHashAccess* containerAccess);
    ValuePointerRCHashAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
};

class ValuePointerRCMultiHashAccess : public WrapperMultiHashAccess, public ReferenceCountingSetContainer{
private:
    ValuePointerRCMultiHashAccess(ValuePointerRCMultiHashAccess& _this);
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
public:
    ~ValuePointerRCMultiHashAccess() override = default;
    ValuePointerRCMultiHashAccess(AbstractMultiHashAccess* containerAccess);
    ValuePointerRCMultiHashAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    jint remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) override;
    void unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject value) override;
};

class PointersRCMapAccess : public WrapperMapAccess, public ReferenceCountingMapContainer{
private:
    PointersRCMapAccess(PointersRCMapAccess& _this);
public:
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
    ~PointersRCMapAccess() override = default;
    PointersRCMapAccess(AbstractMapAccess* containerAccess);
    PointersRCMapAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
};

class PointersRCMultiMapAccess : public WrapperMultiMapAccess, public ReferenceCountingMultiMapContainer{
private:
    PointersRCMultiMapAccess(PointersRCMultiMapAccess& _this);
public:
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
    ~PointersRCMultiMapAccess() override = default;
    PointersRCMultiMapAccess(AbstractMultiMapAccess* containerAccess);
    PointersRCMultiMapAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    jint remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) override;
    void unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject value) override;
};

class PointersRCHashAccess : public WrapperHashAccess, public ReferenceCountingMapContainer{
private:
    PointersRCHashAccess(PointersRCHashAccess& _this);
public:
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
    ~PointersRCHashAccess() override = default;
    PointersRCHashAccess(AbstractHashAccess* containerAccess);
    PointersRCHashAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
};

class PointersRCMultiHashAccess : public WrapperMultiHashAccess, public ReferenceCountingMultiMapContainer{
private:
    PointersRCMultiHashAccess(PointersRCMultiHashAccess& _this);
public:
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
    ~PointersRCMultiHashAccess() override = default;
    PointersRCMultiHashAccess(AbstractMultiHashAccess* containerAccess);
    PointersRCMultiHashAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    jint remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) override;
    void unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject value) override;
};

class NestedPointersRCListAccess : public WrapperListAccess, public ReferenceCountingSetContainer{
private:
    NestedPointersRCListAccess(NestedPointersRCListAccess& _this);
public:
    ~NestedPointersRCListAccess() override = default;
    NestedPointersRCListAccess(AbstractListAccess* containerAccess);
    NestedPointersRCListAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void appendList(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) override;
    void replace(JNIEnv * env, const ContainerInfo& container, jint index, jobject value) override;
    jint removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    ContainerAndAccessInfo mid(JNIEnv * env, const ConstContainerAndAccessInfo& container, jint index1, jint index2) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void remove(JNIEnv * env, const ContainerInfo& container, jint index, jint n) override;
    void insert(JNIEnv * env, const ContainerInfo& container, jint index, jint n, jobject value) override;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    void fill(JNIEnv * env, const ContainerInfo& container, jobject value, jint size) override;
#endif
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
};

class NestedPointersRCSetAccess : public WrapperSetAccess, public ReferenceCountingSetContainer{
private:
    NestedPointersRCSetAccess(NestedPointersRCSetAccess& _this);
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
public:
    ~NestedPointersRCSetAccess() override = default;
    NestedPointersRCSetAccess(AbstractSetAccess* containerAccess);
    NestedPointersRCSetAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void insert(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    jboolean remove(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void intersect(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void subtract(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
};

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
class NestedPointersRCLinkedListAccess : public WrapperLinkedListAccess, public ReferenceCountingSetContainer{
private:
    NestedPointersRCLinkedListAccess(NestedPointersRCLinkedListAccess& _this);
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
public:
    ~NestedPointersRCLinkedListAccess() override = default;
    NestedPointersRCLinkedListAccess(AbstractLinkedListAccess* containerAccess);
    NestedPointersRCLinkedListAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void append(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void prepend(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void removeFirst(JNIEnv * env, const ContainerInfo& container) override;
    void removeLast(JNIEnv * env, const ContainerInfo& container) override;
    jint removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    jobject takeFirst(JNIEnv * env, const ContainerInfo& container) override;
    jobject takeLast(JNIEnv * env, const ContainerInfo& container) override;
    jboolean removeOne(JNIEnv * env, const ContainerInfo& container, jobject value) override;
};

class NestedPointersRCVectorAccess : public WrapperVectorAccess, public ReferenceCountingSetContainer{
private:
    NestedPointersRCVectorAccess(NestedPointersRCVectorAccess& _this);
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
public:
    ~NestedPointersRCVectorAccess() override = default;
    NestedPointersRCVectorAccess(AbstractVectorAccess* containerAccess);
    NestedPointersRCVectorAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void appendVector(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) override;
    void replace(JNIEnv * env, const ContainerInfo& container, jint index, jobject value) override;
    jint removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void remove(JNIEnv * env, const ContainerInfo& container, jint index, jint n) override;
    void insert(JNIEnv * env, const ContainerInfo& container, jint index, jint n, jobject value) override;
    void fill(JNIEnv * env, const ContainerInfo& container, jobject value, jint size) override;
};
#endif

class NestedPointersRCMapAccess : public WrapperMapAccess, public ReferenceCountingSetContainer{
private:
    NestedPointersRCMapAccess(NestedPointersRCMapAccess& _this);
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
public:
    ~NestedPointersRCMapAccess() override = default;
    NestedPointersRCMapAccess(AbstractMapAccess* containerAccess);
    NestedPointersRCMapAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
};

class NestedPointersRCMultiMapAccess : public WrapperMultiMapAccess, public ReferenceCountingSetContainer{
private:
    NestedPointersRCMultiMapAccess(NestedPointersRCMultiMapAccess& _this);
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
public:
    ~NestedPointersRCMultiMapAccess() override = default;
    NestedPointersRCMultiMapAccess(AbstractMultiMapAccess* containerAccess);
    NestedPointersRCMultiMapAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    jint remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) override;
    void unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject value) override;
};

class NestedPointersRCHashAccess : public WrapperHashAccess, public ReferenceCountingSetContainer{
private:
    NestedPointersRCHashAccess(NestedPointersRCHashAccess& _this);
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
public:
    ~NestedPointersRCHashAccess() override = default;
    NestedPointersRCHashAccess(AbstractHashAccess* containerAccess);
    NestedPointersRCHashAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
};

class NestedPointersRCMultiHashAccess : public WrapperMultiHashAccess, public ReferenceCountingSetContainer{
private:
    NestedPointersRCMultiHashAccess(NestedPointersRCMultiHashAccess& _this);
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
public:
    ~NestedPointersRCMultiHashAccess() override = default;
    NestedPointersRCMultiHashAccess(AbstractMultiHashAccess* containerAccess);
    NestedPointersRCMultiHashAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    jint remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) override;
    void unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject value) override;
};

class AutoPairAccess : public AbstractPairAccess, public AbstractNestedPairAccess {
    QMetaType m_keyMetaType;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    size_t m_keyAlign;
#endif
    QtJambiUtils::QHashFunction m_keyHashFunction;
    QtJambiUtils::InternalToExternalConverter m_keyInternalToExternalConverter;
    QtJambiUtils::ExternalToInternalConverter m_keyExternalToInternalConverter;
    QSharedPointer<AbstractContainerAccess> m_keyNestedContainerAccess;
    QMetaType m_valueMetaType;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    size_t m_valueAlign;
#endif
    QtJambiUtils::QHashFunction m_valueHashFunction;
    QtJambiUtils::InternalToExternalConverter m_valueInternalToExternalConverter;
    QtJambiUtils::ExternalToInternalConverter m_valueExternalToInternalConverter;
    QSharedPointer<AbstractContainerAccess> m_valueNestedContainerAccess;
    size_t m_align;
    size_t m_offset;
    size_t m_size;
    PtrOwnerFunction m_keyOwnerFunction;
    PtrOwnerFunction m_valueOwnerFunction;
    AbstractContainerAccess::DataType m_keyDataType;
    AbstractContainerAccess::DataType m_valueDataType;

    AutoPairAccess(const AutoPairAccess& other);

public:
    AutoPairAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            int keyMetaType,
            size_t keyAlign,
#else
            const QMetaType& keyMetaType,
#endif
            const QtJambiUtils::QHashFunction& keyHashFunction,
            const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
            const QtJambiUtils::ExternalToInternalConverter& keyExternalToInternalConverter,
            const QSharedPointer<AbstractContainerAccess>& keyNestedContainerAccess,
            PtrOwnerFunction keyOwnerFunction,
            AbstractContainerAccess::DataType keyDataType,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            int valueMetaType,
            size_t valueAlign,
#else
            const QMetaType& valueMetaType,
#endif
            const QtJambiUtils::QHashFunction& valueHashFunction,
            const QtJambiUtils::InternalToExternalConverter& valueInternalToExternalConverter,
            const QtJambiUtils::ExternalToInternalConverter& valueExternalToInternalConverter,
            const QSharedPointer<AbstractContainerAccess>& valueNestedContainerAccess,
            PtrOwnerFunction valueOwnerFunction,
            AbstractContainerAccess::DataType valueDataType
            );

    void dispose() override;
    AutoPairAccess* clone() override;
    const QObject* getOwner(const void* container) override;
    bool hasOwnerFunction() override;
    bool destructContainer(void* container) override;
    void* constructContainer(void* placement) override;
    void* constructContainer(void* placement, const void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ConstContainerAndAccessInfo& container) override;
    size_t sizeOf() override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* result, void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ContainerAndAccessInfo& container) override;
private:
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
    void assign(void*, const void* ) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    int registerContainer(const QByteArray& typeName) override;
    jobject first(JNIEnv * env, const void* container) override;
    void setFirst(JNIEnv *env, void* container, jobject first) override;
    jobject second(JNIEnv * env, const void* container) override;
    void setSecond(JNIEnv *env, void* container, jobject second) override;

    QPair<const void*,const void*> elements(const void* container) override;
    const QMetaType& firstMetaType() override;
    const QMetaType& secondMetaType() override;
    DataType firstType() override;
    DataType secondType() override;
    AbstractContainerAccess* firstNestedContainerAccess() override;
    AbstractContainerAccess* secondNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedFirstNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedSecondNestedContainerAccess() override;
    bool hasFirstNestedContainerAccess() override;
    bool hasSecondNestedContainerAccess() override;
    bool hasFirstNestedPointers() override;
    bool hasSecondNestedPointers() override;
    std::unique_ptr<AbstractAssociativeAccess::KeyValueIterator> keyValueIterator(const void*) override;
    std::unique_ptr<AbstractAssociativeAccess::KeyValueIterator> keyValueIterator(void*) override;
    std::unique_ptr<AbstractSequentialAccess::ElementIterator> elementIterator(const void*) override;
    std::unique_ptr<AbstractSequentialAccess::ElementIterator> elementIterator(void*) override;
};

class AutoSequentialConstIteratorAccess : public AbstractSequentialConstIteratorAccess{
public:
    typedef void(*IncrementFn)(AutoSequentialConstIteratorAccess*,void*);
    typedef void(*DecrementFn)(AutoSequentialConstIteratorAccess*,void*);
    typedef const void*(*ValueFn)(AutoSequentialConstIteratorAccess*,const void*);
    typedef bool(*LessThanFn)(AutoSequentialConstIteratorAccess*,const void*,const void*);
    typedef bool(*EqualsFn)(AutoSequentialConstIteratorAccess*,const void*,const void*);
private:
    QtJambiUtils::InternalToExternalConverter m_internalToExternalConverter;
    IncrementFn m_increment;
    DecrementFn m_decrement;
    ValueFn m_value;
    LessThanFn m_lessThan;
    EqualsFn m_equals;
    QMetaType m_valueMetaType;
    size_t m_offset;
public:
    ~AutoSequentialConstIteratorAccess() override;
    AutoSequentialConstIteratorAccess(
            const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
            IncrementFn increment,
            DecrementFn decrement,
            ValueFn value,
            LessThanFn lessThan,
            EqualsFn equals,
            const QMetaType& valueMetaType,
            size_t offset
        );
    size_t offset();
    void dispose() override;
    AbstractSequentialConstIteratorAccess* clone() override;
    jobject value(JNIEnv * env, const void* iterator) override;
    void increment(JNIEnv * env, void* iterator) override;
    void decrement(JNIEnv * env, void* iterator) override;
    jboolean lessThan(JNIEnv * env, const void* iterator, const void* other) override;
    bool canLess() override;
    jboolean equals(JNIEnv * env, const void* iterator, const void* other) override;
    const QMetaType& valueMetaType() override;
private:
    Q_DISABLE_COPY_MOVE(AutoSequentialConstIteratorAccess)
    friend class AutoAssociativeConstIteratorAccess;
    friend class AutoSequentialIteratorAccess;
    friend class AutoAssociativeIteratorAccess;
};

class AutoSequentialIteratorAccess : public virtual AbstractSequentialIteratorAccess, public AutoSequentialConstIteratorAccess{
public:
    typedef void(*IncrementFn)(AutoSequentialIteratorAccess*,void*);
    typedef void(*DecrementFn)(AutoSequentialIteratorAccess*,void*);
    typedef const void*(*ValueFn)(AutoSequentialIteratorAccess*,const void*);
    typedef bool(*LessThanFn)(AutoSequentialIteratorAccess*,const void*,const void*);
    typedef bool(*EqualsFn)(AutoSequentialIteratorAccess*,const void*,const void*);
    typedef void*(*SetValueFn)(AutoSequentialIteratorAccess*,void*);
    ~AutoSequentialIteratorAccess() override;
    AutoSequentialIteratorAccess(
            const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
            IncrementFn increment,
            DecrementFn decrement,
            ValueFn value,
            LessThanFn lessThan,
            EqualsFn equals,
            const QtJambiUtils::ExternalToInternalConverter& externalToInternalConverter,
            SetValueFn setValue,
            const QMetaType& valueMetaType,
            size_t offset
        );
    void setValue(JNIEnv * env, void* iterator, jobject newValue) override;
    jobject value(JNIEnv * env, const void* iterator) override;
    void increment(JNIEnv * env, void* iterator) override;
    void decrement(JNIEnv * env, void* iterator) override;
    jboolean lessThan(JNIEnv * env, const void* iterator, const void* other) override;
    bool canLess() override;
    jboolean equals(JNIEnv * env, const void* iterator, const void* other) override;
    const QMetaType& valueMetaType() override;
    AbstractSequentialConstIteratorAccess* clone() override;
private:
    Q_DISABLE_COPY_MOVE(AutoSequentialIteratorAccess)
    QtJambiUtils::ExternalToInternalConverter m_externalToInternalConverter;
    SetValueFn m_setValue;
};

class AutoAssociativeConstIteratorAccess : public virtual AbstractAssociativeConstIteratorAccess, public AutoSequentialConstIteratorAccess{
public:
    typedef void(*IncrementFn)(AutoAssociativeConstIteratorAccess*,void*);
    typedef void(*DecrementFn)(AutoAssociativeConstIteratorAccess*,void*);
    typedef const void*(*ValueFn)(AutoAssociativeConstIteratorAccess*,const void*);
    typedef bool(*LessThanFn)(AutoAssociativeConstIteratorAccess*,const void*,const void*);
    typedef bool(*EqualsFn)(AutoAssociativeConstIteratorAccess*,const void*,const void*);
    typedef const void*(*KeyFn)(AutoAssociativeConstIteratorAccess*,const void*);
private:
    QtJambiUtils::InternalToExternalConverter m_keyInternalToExternalConverter;
    KeyFn m_key;
    QMetaType m_keyMetaType;
    size_t m_keyOffset;
public:
    ~AutoAssociativeConstIteratorAccess() override;
    AutoAssociativeConstIteratorAccess(
            const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
            IncrementFn increment,
            DecrementFn decrement,
            ValueFn value,
            LessThanFn lessThan,
            EqualsFn equals,
            const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
            KeyFn key,
            const QMetaType& keyMetaType,
            const QMetaType& valueMetaType,
            size_t keyOffset,
            size_t valueOffset
        );
    jobject value(JNIEnv * env, const void* iterator) override;
    void increment(JNIEnv * env, void* iterator) override;
    void decrement(JNIEnv * env, void* iterator) override;
    jboolean lessThan(JNIEnv * env, const void* iterator, const void* other) override;
    bool canLess() override;
    jboolean equals(JNIEnv * env, const void* iterator, const void* other) override;
    AbstractSequentialConstIteratorAccess* clone() override;
    jobject key(JNIEnv * env, const void* iterator) override;
    const QMetaType& keyMetaType() override;
    const QMetaType& valueMetaType() override;
    size_t keyOffset();
    size_t valueOffset();
private:
    Q_DISABLE_COPY_MOVE(AutoAssociativeConstIteratorAccess)
    friend class AutoAssociativeIteratorAccess;
};

class AutoAssociativeIteratorAccess : public virtual AbstractAssociativeIteratorAccess, public AutoAssociativeConstIteratorAccess{
    typedef void(*IncrementFn)(AutoAssociativeIteratorAccess*,void*);
    typedef void(*DecrementFn)(AutoAssociativeIteratorAccess*,void*);
    typedef const void*(*ValueFn)(AutoAssociativeIteratorAccess*,const void*);
    typedef bool(*LessThanFn)(AutoAssociativeIteratorAccess*,const void*,const void*);
    typedef bool(*EqualsFn)(AutoAssociativeIteratorAccess*,const void*,const void*);
    typedef void*(*SetValueFn)(AutoAssociativeIteratorAccess*,void*);
    typedef const void*(*KeyFn)(AutoAssociativeIteratorAccess*,const void*);
public:
    ~AutoAssociativeIteratorAccess() override;
    AutoAssociativeIteratorAccess(
            const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
            IncrementFn increment,
            DecrementFn decrement,
            ValueFn value,
            LessThanFn lessThan,
            EqualsFn equals,
            const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
            KeyFn key,
            const QtJambiUtils::ExternalToInternalConverter& valueExternalToInternalConverter,
            SetValueFn setValue,
            const QMetaType& keyMetaType,
            const QMetaType& valueMetaType,
            size_t keyOffset,
            size_t valueOffset
            );
    void setValue(JNIEnv * env, void* iterator, jobject newValue) override;
    jobject value(JNIEnv * env, const void* iterator) override;
    void increment(JNIEnv * env, void* iterator) override;
    void decrement(JNIEnv * env, void* iterator) override;
    jboolean lessThan(JNIEnv * env, const void* iterator, const void* other) override;
    bool canLess() override;
    jboolean equals(JNIEnv * env, const void* iterator, const void* other) override;
    AbstractSequentialConstIteratorAccess* clone() override;
    jobject key(JNIEnv * env, const void* iterator) override;
    const QMetaType& keyMetaType() override;
    const QMetaType& valueMetaType() override;
private:
    Q_DISABLE_COPY_MOVE(AutoAssociativeIteratorAccess)
    QtJambiUtils::ExternalToInternalConverter m_valueExternalToInternalConverter;
    SetValueFn m_setValue;
};

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
class AutoListAccess;
class AutoMapAccess;
class AutoHashAccess;

namespace QtJambiPrivate{

struct ConstRef{
    const void *ptr;
    const QtPrivate::QMetaTypeInterface *iface;
};

struct Ref{
    Ref();
    const QtPrivate::QMetaTypeInterface *iface;
    mutable void *ptr;
private:
    static thread_local QList<std::pair<const QtPrivate::QMetaTypeInterface *,const QtPrivate::QMetaTypeInterface *>> refMetaTypes;
    friend AutoListAccess;
    friend AutoMapAccess;
    friend AutoHashAccess;
    friend struct SecondRef;
    template<typename Access>
    friend struct AbstractMutableContainer;
protected:
    Ref(const QtPrivate::QMetaTypeInterface *_iface);
};

struct SecondRef : Ref{
    SecondRef();
};

template<typename Access, typename Container = const void*>
struct AbstractContainerBase{
    Container container;
    Access* access;
    AbstractContainerBase(Container _container, Access* _access) : container(_container), access(_access) {
    }
    qint64 size() const{
        return access->size(container);
    }
    Q_DISABLE_COPY_MOVE(AbstractContainerBase)
};

template<typename Access>
struct AbstractMutableContainer : AbstractContainerBase<Access, void*>{
    AbstractMutableContainer(void* _container, Access* _access) : AbstractContainerBase<Access, void*>{_container, _access} {
        QtJambiPrivate::Ref::refMetaTypes.append(access->metaTypes());
    }
    ~AbstractMutableContainer(){
        QtJambiPrivate::Ref::refMetaTypes.takeLast();
    }
    using AbstractContainerBase<Access, void*>::access;
    using AbstractContainerBase<Access, void*>::container;
    void clear(){
        access->clear(container);
    }
};

template<typename Access>
struct ConstAssociativeContainer : AbstractContainerBase<Access>{
    using AbstractContainerBase<Access>::AbstractContainerBase;
    using AbstractContainerBase<Access>::access;
    using AbstractContainerBase<Access>::container;
    typedef QtJambiPrivate::Ref key_type;
    typedef QtJambiPrivate::SecondRef mapped_type;
    struct ConstIterator{
        typedef typename Access::iterator iterator;
        iterator iter;
        Access* access;
        ConstIterator(Access* _access) : iter(), access(_access) {}
        ConstIterator(const iterator& _i, Access* _access) : iter(_i), access(_access) {}
        ConstIterator(const ConstIterator& iter) : iter(iter.iter), access(iter.access) {}
        using iterator_category = std::forward_iterator_tag;
        using value_type      = QtJambiPrivate::ConstRef;
        using difference_type = qsizetype;
        using pointer         = QtJambiPrivate::ConstRef*;
        using reference       = QtJambiPrivate::ConstRef&;
        ConstIterator& operator=(const ConstIterator& o) {
            iter = o.iter;
            access = o.access;
            return *this;
        }
        ConstIterator& operator++() noexcept {
            ++iter;
            return *this;
        }
        ConstIterator operator++(int) noexcept {
            ConstIterator _this = *this;
            ++iter;
            return _this;
        }
        bool operator==(ConstIterator other) const noexcept{
            return iter==other.iter;
        }
        bool operator!=(ConstIterator other) const noexcept { return !(*this == other); }
        QtJambiPrivate::ConstRef key()const{
            return QtJambiPrivate::ConstRef{iter.key(), access->metaTypes().first};
        }
        QtJambiPrivate::ConstRef value()const{
            return QtJambiPrivate::ConstRef{iter.value(), access->metaTypes().second};
        }
        QtJambiPrivate::ConstRef operator*()const {return key();}
    };
    typedef ConstIterator const_iterator;
    ConstIterator constBegin() const{
        return ConstIterator(access->begin(container), access);
    }
    ConstIterator constEnd() const{
        return ConstIterator(access->end(container), access);
    }
    ConstIterator begin() const {return constBegin();}
    ConstIterator end() const {return constEnd();}
};

template<typename Access>
struct AssociativeContainer : AbstractMutableContainer<Access>{
    using AbstractMutableContainer<Access>::AbstractMutableContainer;
    using AbstractMutableContainer<Access>::access;
    using AbstractMutableContainer<Access>::container;
    typedef QtJambiPrivate::Ref key_type;
    typedef QtJambiPrivate::SecondRef mapped_type;
    void insert(const QtJambiPrivate::Ref& key, const QtJambiPrivate::SecondRef& value){
        access->insert(container, key.ptr, value.ptr);
    }
};

template<typename Access>
struct ConstSequentialContainer : AbstractContainerBase<Access>{
    using AbstractContainerBase<Access>::AbstractContainerBase;
    using AbstractContainerBase<Access>::access;
    using AbstractContainerBase<Access>::container;
    typedef QtJambiPrivate::ConstRef value_type;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using reference = value_type &;
    using const_reference = const value_type &;
    using size_type = qsizetype;
    using difference_type = qptrdiff;

    struct ConstIterator{
        using iterator_category = std::forward_iterator_tag;
        using value_type      = QtJambiPrivate::ConstRef;
        using difference_type = qsizetype;
        using pointer         = QtJambiPrivate::ConstRef*;
        using reference       = QtJambiPrivate::ConstRef&;
        typename Access::iterator iter;
        Access* access;
        ConstIterator(typename Access::iterator _i, Access* _access) : iter(_i), access(_access) {}

        ConstIterator(const ConstIterator& iter) : iter(iter.iter), access(iter.access) {}

        ConstIterator& operator=(const ConstIterator& o) {
            iter = o.iter;
            access = o.access;
            return *this;
        }
        ConstIterator& operator++() noexcept{
            ++iter;
            return *this;
        }
        ConstIterator operator++(int) noexcept{
            ConstIterator result = this;
            ++iter;
            return result;
        }
        bool operator==(ConstIterator other) const noexcept{
            return access == other.access && iter == other.iter;
        }
        inline bool operator!=(ConstIterator other) const noexcept { return !(*this == other); }
        QtJambiPrivate::ConstRef operator*()const{
            return QtJambiPrivate::ConstRef{&*iter, access->metaTypes().first};
        }
    };
    typedef ConstIterator const_iterator;
    ConstIterator constBegin() const{
        return ConstIterator(access->begin(container), access);
    }
    ConstIterator constEnd() const{
        return ConstIterator(access->end(container), access);
    }
    ConstIterator begin() const {return constBegin();}
    ConstIterator end() const {return constEnd();}
};

template<typename Access>
struct SequentialContainer : AbstractMutableContainer<Access>{
    using AbstractMutableContainer<Access>::AbstractMutableContainer;
    using AbstractMutableContainer<Access>::access;
    using AbstractMutableContainer<Access>::container;
    void append(const QtJambiPrivate::Ref& value){
        access->append(container, value.ptr);
    }
    void reserve(qsizetype size){
        access->reserve(container, size);
    }
    SequentialContainer& operator<<(const QtJambiPrivate::Ref& value){
        access->insert(container, value.ptr);
        return *this;
    }
    typedef QtJambiPrivate::Ref value_type;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using reference = value_type &;
    using const_reference = const value_type &;
    using size_type = qsizetype;
    using difference_type = qptrdiff;
};

QDataStream& operator<<(QDataStream& stream, const ConstRef& ref);
QDebug& operator<<(QDebug& sbg, const ConstRef& ref);
QDataStream& operator>>(QDataStream& s, Ref& ref);
bool operator==(const ConstRef& ref1, const ConstRef& ref2);

}

#endif

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
class AutoSpanAccess : public AbstractSpanAccess, public AbstractNestedSequentialAccess {
    QMetaType m_elementMetaType;
    QtJambiUtils::QHashFunction m_hashFunction;
    QtJambiUtils::InternalToExternalConverter m_internalToExternalConverter;
    QtJambiUtils::ExternalToInternalConverter m_externalToInternalConverter;
    QSharedPointer<AbstractContainerAccess> m_elementNestedContainerAccess;
    size_t m_offset;
    PtrOwnerFunction m_elementOwnerFunction;
    AbstractContainerAccess::DataType m_elementDataType;
    bool m_isConst;
    struct iterator{
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type      = char*;
        using difference_type = qsizetype;
        using pointer         = char*;
        using reference       = char&;

        iterator(size_t _offset, char* _ptr = nullptr);
        iterator(const iterator&) = default;
        iterator& operator++();
        iterator operator++(int);
        iterator& operator--();
        iterator operator--(int);
        bool operator<(const iterator& right) const;
        bool operator==(const iterator& right) const;
        inline bool operator!=(const iterator& right) const { return !operator==(right);}
        const char* operator->() const;
        const char& operator*() const;
        const char* data() const;
        char* operator->();
        char& operator*();
        char* data();
    private:
        size_t offset;
        char* ptr;
    };
    using ConstContainer = QtJambiPrivate::ConstSequentialContainer<AutoSpanAccess>;
    using Container = QtJambiPrivate::SequentialContainer<AutoSpanAccess>;
    iterator begin(const void* container);
    iterator end(const void* container);
protected:
    AutoSpanAccess(const AutoSpanAccess& other);
public:
    AutoSpanAccess(
        const QMetaType& elementMetaType,
        const QtJambiUtils::QHashFunction& hashFunction,
        const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
        const QtJambiUtils::ExternalToInternalConverter& externalToInternalConverter,
        const QSharedPointer<AbstractContainerAccess>& elementNestedContainerAccess,
        PtrOwnerFunction elementOwnerFunction,
        AbstractContainerAccess::DataType elementDataType,
        bool isConst = false
        );

    void dispose() override;
    AutoSpanAccess* clone() override;
    const QObject* getOwner(const void* container) override;
    bool hasOwnerFunction() override;
    bool destructContainer(void* container) override;
    void* constructContainer(void* placement) override;
    void* constructContainer(void* placement, const void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ConstContainerAndAccessInfo& container) override;
    size_t sizeOf() override;
    void* constructContainer(void* result, void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ContainerAndAccessInfo& container) override;
private:
    bool equals(const void* p1, const void* p2);
    void debugStream(QDebug &s, const void *ptr);
    static void defaultCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr);
    static void copyCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, const void *other);
    static void moveCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, void *other);
    static void dtor(const QtPrivate::QMetaTypeInterface *iface, void *ptr);
    static bool equalsFn(const QtPrivate::QMetaTypeInterface *iface, const void *ptr1, const void *ptr2);
    static void debugStreamFn(const QtPrivate::QMetaTypeInterface *iface, QDebug &s, const void *ptr);
    static QtMetaContainerPrivate::QMetaSequenceInterface* createMetaSequenceInterface(int newMetaType);
    qsizetype size(const void* container) override;
    jobject createIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr);
    jobject createConstIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr);
public:
    void assign(void*, const void* ) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    int registerContainer(const QByteArray& typeName) override;
    bool isConst() override;
    jint size(JNIEnv * env, const void* container) override;
    jint size_bytes(JNIEnv * env, const void* container) override;
    jobject get(JNIEnv *,const void*,jint) override;
    bool set(JNIEnv *,const ContainerInfo&,jint,jobject) override;
    const void* get(const void*,qsizetype) override;
    bool set(void*,qsizetype,const void*) override;
    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override;
    std::unique_ptr<AbstractSpanAccess::ElementIterator> elementIterator(void* container) override;
    std::unique_ptr<AbstractSpanAccess::ElementIterator> elementIterator(const void* container) override;

    const QMetaType& elementMetaType() override;
    DataType elementType() override;
    AbstractContainerAccess* elementNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedElementNestedContainerAccess() override;
    bool hasNestedContainerAccess() override;
    bool hasNestedPointers() override;
};

class PointerRCAutoSpanAccess : public AutoSpanAccess, public ReferenceCountingSetContainer{
private:
    PointerRCAutoSpanAccess(PointerRCAutoSpanAccess& _this);
public:
    using AutoSpanAccess::AutoSpanAccess;
    PointerRCAutoSpanAccess* clone() override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    bool set(JNIEnv *,const ContainerInfo&,jint,jobject) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
};

class NestedPointersRCAutoSpanAccess : public AutoSpanAccess, public ReferenceCountingSetContainer{
private:
    NestedPointersRCAutoSpanAccess(NestedPointersRCAutoSpanAccess& _this);
public:
    using AutoSpanAccess::AutoSpanAccess;
    NestedPointersRCAutoSpanAccess* clone() override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    bool set(JNIEnv *,const ContainerInfo&,jint,jobject) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
};

#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)

class AutoListAccess : public AbstractListAccess, public AbstractNestedSequentialAccess {
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    typedef QArrayDataPointer<char> QListData;
#endif
    QMetaType m_elementMetaType;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    bool m_isLargeOrStaticType;
#endif
    QtJambiUtils::QHashFunction m_hashFunction;
    QtJambiUtils::InternalToExternalConverter m_internalToExternalConverter;
    QtJambiUtils::ExternalToInternalConverter m_externalToInternalConverter;
    QSharedPointer<AbstractContainerAccess> m_elementNestedContainerAccess;
    size_t m_offset;
    PtrOwnerFunction m_elementOwnerFunction;
    AbstractContainerAccess::DataType m_elementDataType;
protected:
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
            const QtJambiUtils::QHashFunction& hashFunction,
            const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
            const QtJambiUtils::ExternalToInternalConverter& externalToInternalConverter,
            const QSharedPointer<AbstractContainerAccess>& elementNestedContainerAccess,
            PtrOwnerFunction elementOwnerFunction,
            AbstractContainerAccess::DataType elementDataType
            );

    void dispose() override;
    bool isDetached(const void* container) override;
    void detach(const ContainerInfo& container) override;
    bool isSharedWith(const void* container, const void* container2) override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    AutoListAccess* clone() override;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    AutoSpanAccess* createSpanAccess(bool isConst) override;
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    const QObject* getOwner(const void* container) override;
    bool hasOwnerFunction() override;
    void* constructContainer(void* placement) override;
    void* constructContainer(void* placement, const void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ConstContainerAndAccessInfo& container) override;
    bool destructContainer(void* container) override;
    size_t sizeOf() override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* result, void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ContainerAndAccessInfo& container) override;
    struct iterator{
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type      = char*;
        using difference_type = qsizetype;
        using pointer         = char*;
        using reference       = char&;

        iterator(size_t _offset, char* _ptr = nullptr);
        iterator(const iterator&) = default;
        iterator& operator++();
        iterator operator++(int);
        iterator& operator--();
        iterator operator--(int);
        bool operator<(const iterator& right) const;
        bool operator==(const iterator& right) const;
        inline bool operator!=(const iterator& right) const { return !operator==(right);}
        const char* operator->() const;
        const char& operator*() const;
        const char* data() const;
        char* operator->();
        char& operator*();
        char* data();
    private:
        size_t offset;
        char* ptr;
    };
private:
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

    using ConstContainer = QtJambiPrivate::ConstSequentialContainer<AutoListAccess>;
    using Container = QtJambiPrivate::SequentialContainer<AutoListAccess>;
    iterator begin(const void* container);
    iterator end(const void* container);
#endif
public:
    void assign(void* container, const void* other) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    int registerContainer(const QByteArray& containerTypeName) override;
    const QMetaType& elementMetaType() override;
    DataType elementType() override;
    AbstractContainerAccess* elementNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedElementNestedContainerAccess() override;
    bool hasNestedContainerAccess() override;
    bool hasNestedPointers() override;
    jobject createIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr);
    jobject createConstIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void reserve(void*,qsizetype);
    std::pair<const QtPrivate::QMetaTypeInterface *,const QtPrivate::QMetaTypeInterface *> metaTypes();
#else
#endif
    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    void appendList(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) override;
    jobject at(JNIEnv * env, const void* container, jint index) override;
    const void* at(const void* container, qsizetype index) override;
    void* at(void* container, qsizetype index) override;
    jobject value(JNIEnv * env, const void* container, jint index) override;
    jobject value(JNIEnv * env, const void* container, jint index, jobject defaultValue) override;
    void swapItemsAt(JNIEnv *, const ContainerInfo& container, jint index1, jint index2) override;
    jboolean startsWith(JNIEnv * env, const void* container, jobject value) override;
    jint size(JNIEnv *, const void* container) override;
    void reserve(JNIEnv *, const ContainerInfo& container, jint size) override;
    void replace(JNIEnv * env, const ContainerInfo& container, jint index, jobject value) override;
    void replace(void* container, qsizetype index, const void* value) override;
    void remove(JNIEnv *, const ContainerInfo& container, jint index, jint n) override;
    void remove(void* container, qsizetype index, qsizetype n) override;
    jint removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    jboolean equal(JNIEnv * env, const void* container, jobject other) override;
    void move(JNIEnv *, const ContainerInfo& container, jint index1, jint index2) override;
    ContainerAndAccessInfo mid(JNIEnv * env, const ConstContainerAndAccessInfo& container, jint index1, jint index2) override;
    jint lastIndexOf(JNIEnv * env, const void* container, jobject value, jint index) override;
    jint indexOf(JNIEnv * env, const void* container, jobject value, jint index) override;
    jboolean endsWith(JNIEnv * env, const void* container, jobject value) override;
    jint count(JNIEnv * env, const void* container, jobject value) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    void clear(JNIEnv *, const ContainerInfo& container) override;
    void clear(void* container);
    void insert(JNIEnv * env, const ContainerInfo& container, jint index, jint n, jobject value) override;
    void insert(void* container, qsizetype index, qsizetype n, const void* entry) override;
    bool append(void* container, const void* value) override;
    inline void insert(void* container, const void* value){
        append(container, value);
    }
    qsizetype size(const void* container) override;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    void resize(void* container, qsizetype newSize) override;
    jint capacity(JNIEnv *, const void* container) override;
    void fill(JNIEnv * env, const ContainerInfo& container, jobject value, jint size) override;
    void resize(JNIEnv *, const ContainerInfo& container, jint newSize) override;
    void squeeze(JNIEnv *, const ContainerInfo& container) override;
#endif
    std::unique_ptr<AbstractListAccess::ElementIterator> elementIterator(const void* container) override;
    std::unique_ptr<AbstractListAccess::ElementIterator> elementIterator(void* container) override;
private:
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    static QtMetaContainerPrivate::QMetaSequenceInterface* createMetaSequenceInterface(int newMetaType);
    void emplace(QListData* p, JNIEnv * env, jint index, jobject value, jint n);
    void emplace(QListData* p, qsizetype index, const void* value, qsizetype n);
    void *createHole(QListData *p, QArrayData::GrowthPosition pos, qsizetype where, qsizetype n);
    qsizetype freeSpaceAtBegin(const QListData* p);
    qsizetype freeSpaceAtEnd(const QListData* p);
    void detach(QListData* p, QListData *old = nullptr);
    void detachAndGrow(QListData* p, QArrayData::GrowthPosition where, qsizetype n, const void **data,
                       QListData *old);
    void reallocate(QListData* p, qsizetype alloc, QArrayData::AllocationOption option);
    void reallocateAndGrow(QListData* p, QArrayData::GrowthPosition, qsizetype n,
                           QListData *old = nullptr);
    QListData allocateGrow(const QListData &from, qsizetype n, QArrayData::GrowthPosition position);
    QListData allocate(qsizetype capacity, QArrayData::AllocationOption option = QArrayData::KeepSize);
    void swapAndDestroy(QListData* p, QListData&& other);
    friend Container;
    friend ConstContainer;
#else
    typedef struct{void* v;} Node;
    void dealloc(QListData* p);
    Node* detach_helper_grow(QListData* p, int i, int c);
    void detach(QListData* p);
    void detach_helper(QListData* p);
    void detach_helper(QListData* p, int alloc);
    void node_copy(Node *from, Node *to, Node *src);
    void node_destruct(Node *node);
    void node_destruct(Node *from, Node *to);
#endif
    friend class PointerRCAutoListAccess;
    friend class NestedPointersRCAutoListAccess;
};

typedef bool (*IsBiContainerFunction)(JNIEnv *, jobject, const QMetaType&, const QMetaType&, void*& ptr);

class AutoMapAccess : public AbstractMapAccess, public AbstractNestedAssociativeAccess {
    QMetaType m_keyMetaType;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    size_t m_keyAlign;
#endif
    QtJambiUtils::QHashFunction m_keyHashFunction;
    QtJambiUtils::InternalToExternalConverter m_keyInternalToExternalConverter;
    QtJambiUtils::ExternalToInternalConverter m_keyExternalToInternalConverter;
    QSharedPointer<AbstractContainerAccess> m_keyNestedContainerAccess;
    QMetaType m_valueMetaType;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    size_t m_valueAlign;
#endif
    QtJambiUtils::QHashFunction m_valueHashFunction;
    QtJambiUtils::InternalToExternalConverter m_valueInternalToExternalConverter;
    QtJambiUtils::ExternalToInternalConverter m_valueExternalToInternalConverter;
    QSharedPointer<AbstractContainerAccess> m_valueNestedContainerAccess;
    size_t m_align;
    size_t m_offset1;
    size_t m_offset2;
    size_t m_size;
    PtrOwnerFunction m_keyOwnerFunction;
    PtrOwnerFunction m_valueOwnerFunction;
    AbstractContainerAccess::DataType m_keyDataType;
    AbstractContainerAccess::DataType m_valueDataType;
protected:
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
                    const QtJambiUtils::QHashFunction& keyHashFunction,
                    const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
                    const QtJambiUtils::ExternalToInternalConverter& keyExternalToInternalConverter,
                    const QSharedPointer<AbstractContainerAccess>& keyNestedContainerAccess,
                    PtrOwnerFunction keyOwnerFunction,
                    AbstractContainerAccess::DataType keyDataType,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                    int valueMetaType,
                    size_t valueAlign,
#else
                    const QMetaType& valueMetaType,
#endif
                    const QtJambiUtils::QHashFunction& valueHashFunction,
                    const QtJambiUtils::InternalToExternalConverter& valueInternalToExternalConverter,
                    const QtJambiUtils::ExternalToInternalConverter& valueExternalToInternalConverter,
                    const QSharedPointer<AbstractContainerAccess>& valueNestedContainerAccess,
                    PtrOwnerFunction valueOwnerFunction,
                    AbstractContainerAccess::DataType valueDataType
            );
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void assign(void* container, const void* other) override;
    int registerContainer(const QByteArray& containerTypeName) override;
    void dispose() override;
    AbstractMapAccess* clone() override;
    const QObject* getOwner(const void* container) override;
    bool hasOwnerFunction() override;
    bool isDetached(const void* container) override;
    void detach(const ContainerInfo& container) override;
    bool isSharedWith(const void* container, const void* container2) override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    const QMetaType& keyMetaType() override;
    const QMetaType& valueMetaType() override;
    DataType keyType() override;
    DataType valueType() override;
    AbstractContainerAccess* keyNestedContainerAccess() override;
    AbstractContainerAccess* valueNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedKeyNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedValueNestedContainerAccess() override;
    bool hasKeyNestedContainerAccess() override;
    bool hasValueNestedContainerAccess() override;
    bool hasKeyNestedPointers() override;
    bool hasValueNestedPointers() override;
    void clear(JNIEnv *,const ContainerInfo&) override;
    void clear(void*);
    bool contains(const void*,const void*) override;
    void insert(void* container,const void* key, const void* value) override;
    const void* value(const void*, const void*, const void*) override;
    jboolean contains(JNIEnv *,const void*,jobject) override;
    jint count(JNIEnv *,const void*,jobject) override;
    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key) override;
    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override;
    jobject first(JNIEnv *,const void*) override;
    jobject firstKey(JNIEnv *,const void*) override;
    void insert(JNIEnv *,const ContainerInfo&,jobject,jobject) override;
    jobject key(JNIEnv *,const void*,jobject,jobject) override;
    ContainerAndAccessInfo keys(JNIEnv *,const ConstContainerInfo&) override;
    ContainerAndAccessInfo keys(JNIEnv *,const ConstContainerInfo&,jobject) override;
    jobject last(JNIEnv *,const void*) override;
    jobject lastKey(JNIEnv *,const void*) override;
    jobject constLowerBound(JNIEnv *,const ConstExtendedContainerInfo& container,jobject) override;
    jboolean equal(JNIEnv *,const void*,jobject) override;
    jint remove(JNIEnv *,const ContainerInfo&,jobject) override;
    jint size(JNIEnv *,const void*) override;
    qsizetype size(const void* container) override;
    jobject take(JNIEnv *,const ContainerInfo&,jobject) override;
    jobject constUpperBound(JNIEnv *,const ConstExtendedContainerInfo& container,jobject) override;
    jobject value(JNIEnv *,const void*,jobject,jobject) override;
    ContainerAndAccessInfo values(JNIEnv *,const ConstContainerInfo&) override;
    bool keyLessThan(JNIEnv *,jobject,jobject) override;
    bool destructContainer(void* container) override;
    size_t sizeOf() override;
    void* constructContainer(void* placement) override;
    void* constructContainer(void* placement, const void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ConstContainerAndAccessInfo& container) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    static void defaultCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr);
    static void copyCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, const void *other);
    static void moveCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, void *other);
    static void dtor(const QtPrivate::QMetaTypeInterface *iface, void *ptr);
    static bool equalsFn(const QtPrivate::QMetaTypeInterface *iface, const void *ptr1, const void *ptr2);
    static void debugStreamFn(const QtPrivate::QMetaTypeInterface *iface, QDebug &s, const void *ptr);
    static void dataStreamOutFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, const void *ptr);
    static void dataStreamInFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, void *ptr);
    virtual void debugStream(QDebug &s, const void *ptr);
    virtual void dataStreamOut(QDataStream &s, const void *ptr);
    void dataStreamIn(QDataStream &s, void *ptr);
    void* constructContainer(void* result, void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ContainerAndAccessInfo& container) override;
    std::pair<const QtPrivate::QMetaTypeInterface *,const QtPrivate::QMetaTypeInterface *> metaTypes();
#endif
private:
    virtual IsBiContainerFunction getIsBiContainerFunction();
    virtual bool isMulti() const;
    virtual ushort alignOf() const;
    bool equal(const void* containerA, const void* containerB);
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
    jobject nodeKey(JNIEnv * env, Node* node);
    jobject nodeValue(JNIEnv * env, Node* node);
#else
    static QtMetaContainerPrivate::QMetaAssociationInterface* createMetaAssociationInterface(int newMetaType);

    enum Colors {
        Red,
        Black
    };

#if defined(_LIBCPP_VERSION)
    struct TreeNode;
    struct TreeEndNode{
        TreeNode* left = nullptr;
    };

    struct TreeNode : TreeEndNode{
        TreeNode* right = nullptr;
        TreeEndNode* parent = nullptr;
        bool color = Red;
#elif defined(Q_CC_MSVC)
    struct TreeNode{
        TreeNode* left = nullptr;
        TreeNode* parent = nullptr;
        TreeNode* right = nullptr;
        char color = Red;
        char isNil = false;
#elif defined(__GLIBCXX__)
    struct TreeNode{
        Colors color = Red;
        TreeNode* parent = nullptr;
        TreeNode* left = nullptr;
        TreeNode* right = nullptr;
#endif
        void* data(qsizetype offset);
        const void* data(qsizetype offset) const;
    };
#if defined(Q_CC_MSVC)
#if _ITERATOR_DEBUG_LEVEL == 0
    struct MapData : QSharedData, std::_Container_base0{
#else
    struct MapData : QSharedData, std::_Container_base12{
#endif
        TreeNode* head = nullptr;
        quint64 size = 0;
    };
#else
    struct MapData : QSharedData{
#if defined(_LIBCPP_VERSION)
        TreeNode* begin = nullptr;
        mutable TreeEndNode header;
        std::size_t size = 0;
        TreeNode* end();
#elif defined(__GLIBCXX__)
        quintptr compare = 0;
        mutable TreeNode header;
        std::size_t size = 0;
#endif
    };
#endif
    typedef QtPrivate::QExplicitlySharedDataPointerV2<MapData> MapDataPointer;

    struct node_iterator{
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type      = TreeNode;
        using difference_type = qsizetype;
        using pointer         = TreeNode*;
        using reference       = TreeNode&;

        node_iterator(TreeNode* _node = nullptr);
        node_iterator(const node_iterator&);
        inline node_iterator& operator=(const node_iterator& o) {
            node = o.node;
            return *this;
        }
        node_iterator& operator++();
        node_iterator operator++(int);
        node_iterator& operator--();
        node_iterator operator--(int);
        bool operator==(const node_iterator& right) const;
        bool operator!=(const node_iterator& right) const;
        TreeNode* operator->() const;
        TreeNode& operator*() const;
        inline TreeNode* data() const{return node;};
    private:
        TreeNode* next(TreeNode*);
        TreeNode* prev(TreeNode*);
        TreeNode* node;
    };

    struct iterator{
        node_iterator m_iter;
        size_t m_offset1 = 0;
        size_t m_offset2 = 0;
        iterator();
        iterator(node_iterator iter, size_t offset1, size_t offset2);
        iterator(const iterator&) = default;
        inline iterator& operator=(const iterator& o) {
            m_iter = o.m_iter;
            m_offset1 = o.m_offset1;
            m_offset2 = o.m_offset2;
            return *this;
        }
        iterator& operator++();
        iterator operator++(int);
        iterator& operator--();
        iterator operator--(int);
        bool operator==(const iterator& right) const;
        inline bool operator!=(const iterator& right) const {return !(*this==right);}
        void* key()const;
        void* value()const;
    };

    struct extended_iterator : node_iterator{
        extended_iterator(const void* _container, const QSharedPointer<class AutoMapAccess>& _access, TreeNode* _node = nullptr);
        extended_iterator(const void* _container, const QSharedPointer<class AutoMapAccess>& _access, const node_iterator& other);
        extended_iterator(const extended_iterator&) = default;
        const void* container;
        QSharedPointer<class AutoMapAccess> access;
    };

    using ConstContainer = QtJambiPrivate::ConstAssociativeContainer<AutoMapAccess>;

    using Container = QtJambiPrivate::AssociativeContainer<AutoMapAccess>;
    friend Container;
    friend ConstContainer;

    MapData& detach(MapDataPointer& container);
    void copyNode(QList<TreeNode*>& nextNodes, QHash<TreeNode*,TreeNode*> &nodesMap, TreeNode* node);
    MapData* createMapData();
    void clear(MapData& data);
    node_iterator end(const MapData& data);
    node_iterator begin(const MapData& data);
    iterator end(const void* container);
    iterator begin(const void* container);
    void eraseTree(MapData& data, TreeNode* node);
    qsizetype erase(MapData& data, const void* key);
    qsizetype erase(MapData& data, const void* key, const void* value);
    qsizetype erase(MapData& data, QPair<TreeNode*,TreeNode*> pair);
    void destroy(TreeNode*);
    TreeNode* extract(MapData& data, node_iterator iter);
    void insertOrAssign(MapData&,const void*,JNIEnv *,jobject value);
    void insertOrAssign(MapData&,const void*,const void* value);
    jint copyIfNotEquivalentTo(MapData& data, const MapData& copyFrom, const void* key);
    jint copyIfNotEquivalentTo(MapData& data, const MapData& copyFrom, const void* key, const void* value);
    node_iterator erase(MapData& data, node_iterator iter);
    TreeNode* erase(MapData& data, node_iterator first, node_iterator last);
    QPair<TreeNode*,TreeNode*> eqrange(const MapData& data, const void* key);
    node_iterator find(const MapData& data, const void* key);
#if defined(Q_CC_MSVC)
    enum class TreeChild {
        Right,
        Left,
        Unused
    };

    struct Location{
        TreeNode* parent = nullptr;
        TreeChild child = TreeChild::Unused;
    };

    struct TreeFindResult{
        Location location;
        TreeNode* bound = nullptr;
        inline operator TreeNode*() {return bound;}
    };
    TreeFindResult findLowerBound(const MapData& data, const void* key);
    TreeNode* insertNode(MapData& data, Location loc, TreeNode* newNode);
    static TreeNode* treeMin(TreeNode*);
    static TreeNode* treeMax(TreeNode*);
    void rotateLeft(MapData& data, TreeNode*node);
    void rotateRight(MapData& data, TreeNode*node);
    void orphanPtr(MapData& data, const TreeNode* node);
#elif defined(_LIBCPP_VERSION)
    TreeNode* findLowerBound(const MapData& data, const void* key);
    TreeNode*& findLeafHigh(MapData& data, TreeEndNode*& parent, const void* key);
    TreeNode*& findLeafLow(MapData& data, TreeEndNode*& parent, const void* key);
    TreeNode*& findLeaf(MapData& data, node_iterator node, TreeEndNode*& parent, const void* key);
    TreeNode*& findEqual(MapData& data, TreeEndNode*& parent, const void* key);
    TreeNode*& findEqualHint(MapData& data, node_iterator node, TreeEndNode*& parent, TreeNode*& dummy, const void* key);
#elif defined(__GLIBCXX__)
    TreeNode* findLowerBound(const MapData& data, const void* key);
    std::pair<TreeNode*, TreeNode*> getInsertUniquePos(MapData& data, const void* key);
    std::pair<TreeNode*, TreeNode*> getInsertHintUniquePos(MapData& data, TreeNode* node, const void* key);
    std::pair<TreeNode*, TreeNode*> getInsertEqualPos(MapData& data, const void* key);
    std::pair<TreeNode*, TreeNode*> getInsertHintEqualPos(MapData& data, TreeNode* node, const void* key);
#endif
    TreeNode* findUpperBound(const MapData& data, const void* key);
#endif

    jobject createIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr);
    jobject createConstIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr);
    friend class AutoMultiMapAccess;
public:
    std::unique_ptr<AbstractMapAccess::KeyValueIterator> keyValueIterator(const void* container) override;
    std::unique_ptr<AbstractMapAccess::KeyValueIterator> keyValueIterator(void* container) override;
};

class AutoMultiMapAccess : public virtual AbstractMultiMapAccess, public AutoMapAccess{
protected:
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
                    const QtJambiUtils::QHashFunction& keyHashFunction,
                    const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
                    const QtJambiUtils::ExternalToInternalConverter& keyExternalToInternalConverter,
                    const QSharedPointer<AbstractContainerAccess>& keyNestedContainerAccess,
                    PtrOwnerFunction keyOwnerFunction,
                    AbstractContainerAccess::DataType keyDataType,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                    int valueMetaType,
                    size_t valueAlign,
#else
                    const QMetaType& valueMetaType,
#endif
                    const QtJambiUtils::QHashFunction& valueHashFunction,
                    const QtJambiUtils::InternalToExternalConverter& valueInternalToExternalConverter,
                    const QtJambiUtils::ExternalToInternalConverter& valueExternalToInternalConverter,
                    const QSharedPointer<AbstractContainerAccess>& valueNestedContainerAccess,
                    PtrOwnerFunction valueOwnerFunction,
                    AbstractContainerAccess::DataType valueDataType
            );
    void assign(void*, const void* ) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    int registerContainer(const QByteArray& containerTypeName) override;
    void dispose() override;
    bool isDetached(const void* container) override;
    void detach(const ContainerInfo& container) override;
    bool isSharedWith(const void* container, const void* container2) override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    const QMetaType& keyMetaType() override;
    const QMetaType& valueMetaType() override;
    DataType keyType() override;
    DataType valueType() override;
    AbstractContainerAccess* keyNestedContainerAccess() override;
    AbstractContainerAccess* valueNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedKeyNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedValueNestedContainerAccess() override;
    bool hasKeyNestedContainerAccess() override;
    bool hasValueNestedContainerAccess() override;
    bool hasKeyNestedPointers() override;
    bool hasValueNestedPointers() override;
    void clear(JNIEnv *,const ContainerInfo&) override;
    jboolean contains(JNIEnv *,const void*,jobject) override;
    jint count(JNIEnv *,const void*,jobject) override;
    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key) override;
    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override;
    jobject first(JNIEnv *,const void*) override;
    jobject firstKey(JNIEnv *,const void*) override;
    void insert(JNIEnv *,const ContainerInfo&,jobject,jobject) override;
    jobject key(JNIEnv *,const void*,jobject,jobject) override;
    ContainerAndAccessInfo keys(JNIEnv *,const ConstContainerInfo&) override;
    ContainerAndAccessInfo keys(JNIEnv *,const ConstContainerInfo&,jobject) override;
    jobject last(JNIEnv *,const void*) override;
    jobject lastKey(JNIEnv *,const void*) override;
    jobject constLowerBound(JNIEnv *,const ConstExtendedContainerInfo& container,jobject) override;
    jboolean equal(JNIEnv *,const void*,jobject) override;
    jint remove(JNIEnv *,const ContainerInfo&,jobject) override;
    jint size(JNIEnv *,const void*) override;
    qsizetype size(const void* container) override;
    jobject take(JNIEnv *,const ContainerInfo&,jobject) override;
    jobject constUpperBound(JNIEnv *,const ConstExtendedContainerInfo& container,jobject) override;
    jobject value(JNIEnv *,const void*,jobject,jobject) override;
    ContainerAndAccessInfo values(JNIEnv *,const ConstContainerInfo&) override;
    bool keyLessThan(JNIEnv *,jobject,jobject) override;
    AbstractMultiMapAccess* clone() override;
    const QObject* getOwner(const void* container) override;
    bool hasOwnerFunction() override;
    ContainerAndAccessInfo uniqueKeys(JNIEnv *,const ConstContainerInfo&) override;
    void unite(JNIEnv *,const ContainerInfo&,ContainerAndAccessInfo&) override;
    ContainerAndAccessInfo values(JNIEnv *,const ConstContainerInfo&,jobject) override;
    jboolean contains(JNIEnv *,const void*,jobject,jobject) override;
    jint count(JNIEnv *,const void*,jobject,jobject) override;
    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key, jobject value) override;
    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key, jobject value) override;
    jint remove(JNIEnv *,const ContainerInfo&,jobject,jobject) override;
    void replace(JNIEnv *,const ContainerInfo&,jobject,jobject) override;
    bool contains(const void*,const void*) override;
    void insert(void* container,const void* key, const void* value) override;
    const void* value(const void*, const void*, const void*) override;
    size_t sizeOf() override;
    void* constructContainer(void* placement) override;
    void* constructContainer(void* placement, const void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ConstContainerAndAccessInfo& container) override;
    bool destructContainer(void* container) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* result, void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ContainerAndAccessInfo& container) override;
    void dataStreamOut(QDataStream &s, const void *ptr) override;
    void debugStream(QDebug &s, const void *ptr) override;
#endif
    std::unique_ptr<AbstractMapAccess::KeyValueIterator> keyValueIterator(const void* container) override;
    std::unique_ptr<AbstractMapAccess::KeyValueIterator> keyValueIterator(void* container) override;
private:
    ushort alignOf() const override;
    IsBiContainerFunction getIsBiContainerFunction() override;
    bool isMulti() const override;
};

class AutoHashAccess : public AbstractHashAccess, public AbstractNestedAssociativeAccess {
    QMetaType m_keyMetaType;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    size_t m_keyAlign;
#endif
    QtJambiUtils::QHashFunction m_keyHashFunction;
    QtJambiUtils::InternalToExternalConverter m_keyInternalToExternalConverter;
    QtJambiUtils::ExternalToInternalConverter m_keyExternalToInternalConverter;
    QSharedPointer<AbstractContainerAccess> m_keyNestedContainerAccess;
    QMetaType m_valueMetaType;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    size_t m_valueAlign;
#endif
    QtJambiUtils::QHashFunction m_valueHashFunction;
    QtJambiUtils::InternalToExternalConverter m_valueInternalToExternalConverter;
    QtJambiUtils::ExternalToInternalConverter m_valueExternalToInternalConverter;
    QSharedPointer<AbstractContainerAccess> m_valueNestedContainerAccess;
    size_t m_align;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    size_t m_offset1;
#endif
    size_t m_offset2;
    size_t m_size;
    PtrOwnerFunction m_keyOwnerFunction;
    PtrOwnerFunction m_valueOwnerFunction;
    AbstractContainerAccess::DataType m_keyDataType;
    AbstractContainerAccess::DataType m_valueDataType;
protected:
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
                    const QtJambiUtils::QHashFunction& keyHashFunction,
                    const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
                    const QtJambiUtils::ExternalToInternalConverter& keyExternalToInternalConverter,
                    const QSharedPointer<AbstractContainerAccess>& keyNestedContainerAccess,
                    PtrOwnerFunction keyOwnerFunction,
                    AbstractContainerAccess::DataType keyDataType,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                    int valueMetaType,
                    size_t valueAlign,
#else
                    const QMetaType& valueMetaType,
#endif
                    const QtJambiUtils::QHashFunction& valueHashFunction,
                    const QtJambiUtils::InternalToExternalConverter& valueInternalToExternalConverter,
                    const QtJambiUtils::ExternalToInternalConverter& valueExternalToInternalConverter,
                    const QSharedPointer<AbstractContainerAccess>& valueNestedContainerAccess,
                    PtrOwnerFunction valueOwnerFunction,
                    AbstractContainerAccess::DataType valueDataType
            );
    bool isDetached(const void* container) override;
    void detach(const ContainerInfo& container) override;
    bool isSharedWith(const void* container, const void* container2) override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    virtual void assign(void* container, const void* other) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    int registerContainer(const QByteArray& containerTypeName) override;
    void dispose() override;
    AbstractHashAccess* clone() override;
    const QObject* getOwner(const void* container) override;
    bool hasOwnerFunction() override;
    const QMetaType& keyMetaType() override;
    const QMetaType& valueMetaType() override;
    DataType keyType() override;
    DataType valueType() override;
    AbstractContainerAccess* keyNestedContainerAccess() override;
    AbstractContainerAccess* valueNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedKeyNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedValueNestedContainerAccess() override;
    bool hasKeyNestedContainerAccess() override;
    bool hasValueNestedContainerAccess() override;
    bool hasKeyNestedPointers() override;
    bool hasValueNestedPointers() override;
    jint capacity(JNIEnv *,const void*) override;
    void clear(JNIEnv *,const ContainerInfo&) override;
    void clear(void*);
    jboolean contains(JNIEnv *,const void*,jobject) override;
    jint count(JNIEnv *,const void*,jobject) override;
    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key) override;
    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override;
    void insert(JNIEnv *,const ContainerInfo&,jobject,jobject) override;
    jobject key(JNIEnv *,const void*,jobject,jobject) override;
    ContainerAndAccessInfo keys(JNIEnv *,const ConstContainerInfo&) override;
    ContainerAndAccessInfo keys(JNIEnv *,const ConstContainerInfo&,jobject) override;
    jboolean equal(JNIEnv *,const void*,jobject) override;
    jint remove(JNIEnv *,const ContainerInfo&,jobject) override;
    void reserve(JNIEnv *,const ContainerInfo&,jint) override;
    jint size(JNIEnv *,const void*) override;
    qsizetype size(const void*) override;
    jobject take(JNIEnv *,const ContainerInfo&,jobject) override;
    jobject value(JNIEnv *,const void*,jobject,jobject) override;
    ContainerAndAccessInfo values(JNIEnv *,const ConstContainerInfo&) override;
    void* constructContainer(void* placement) override;
    void* constructContainer(void* placement, const void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ConstContainerAndAccessInfo& container) override;
    bool destructContainer(void* container) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* result, void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ContainerAndAccessInfo& container) override;
    std::pair<const QtPrivate::QMetaTypeInterface *,const QtPrivate::QMetaTypeInterface *> metaTypes();
#endif
    bool contains(const void*,const void*) override;
    void insert(void* container,const void* key, const void* value = nullptr) override;
    const void* value(const void*, const void*, const void*) override;
    size_t sizeOf() override;
    std::unique_ptr<AbstractHashAccess::KeyValueIterator> keyValueIterator(const void* container) override;
    std::unique_ptr<AbstractHashAccess::KeyValueIterator> keyValueIterator(void* container) override;
private:
    virtual ushort alignOf() const;
    virtual IsBiContainerFunction getIsBiContainerFunction();
    jobject createIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr);
    jobject createConstIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr);
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
    virtual void debugStream(QDebug &s, const void *ptr);
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
        inline char& operator*() const noexcept {return *i.key();}
        char* value() const noexcept;
        friend AutoHashAccess;
        friend class AutoMultiHashAccess;
        friend class AutoSetAccess;
    };

    using ConstContainer = QtJambiPrivate::ConstAssociativeContainer<AutoHashAccess>;
    using Container = QtJambiPrivate::AssociativeContainer<AutoHashAccess>;
    using SetConstContainer = QtJambiPrivate::ConstSequentialContainer<AutoHashAccess>;
    using SetContainer = QtJambiPrivate::SequentialContainer<AutoHashAccess>;

    iterator begin(const void* container);
    iterator end(const void* container);
    virtual bool equalSpanEntries(const char* value1, const char* value2) const;
    virtual void copySpanEntry(char* value1, const char* value2) const;
    virtual void eraseSpanEntry(char* value) const;
    virtual void emplace(void* container, const void* akey, JNIEnv *env, jobject value);
    virtual void emplace(void* container, const void* akey, const void* value);
    virtual char* iteratorValue(const iterator& it) const;
    virtual iterator& incrementIterator(iterator& it) const;
    virtual void initializeIterator(iterator& it) const;
    virtual bool iteratorEquals(const iterator& it1, const iterator& it2) const;
    static void defaultCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr);
    static void copyCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, const void *other);
    static void moveCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, void *other);
    static void dtor(const QtPrivate::QMetaTypeInterface *iface, void *ptr);
    static bool equalsFn(const QtPrivate::QMetaTypeInterface *iface, const void *ptr1, const void *ptr2);
    static void debugStreamFn(const QtPrivate::QMetaTypeInterface *iface, QDebug &s, const void *ptr);
    static void dataStreamOutFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, const void *ptr);
    static void dataStreamInFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, void *ptr);
    static void debugStreamSetFn(const QtPrivate::QMetaTypeInterface *iface, QDebug &s, const void *ptr);
    static void dataStreamOutSetFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, const void *ptr);
    static void dataStreamInSetFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, void *ptr);
    friend iterator;
    friend Container;
    friend ConstContainer;
    friend SetContainer;
    friend SetConstContainer;
#endif
    virtual bool isMulti() const;
    bool equal(const void* a, const void* b);
    void detach(QHashData ** map);

    friend class AutoMultiHashAccess;
    friend class AutoSetAccess;
};

class AutoMultiHashAccess : public virtual AbstractMultiHashAccess, public AutoHashAccess{
protected:
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
                    const QtJambiUtils::QHashFunction& keyHashFunction,
                    const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
                    const QtJambiUtils::ExternalToInternalConverter& keyExternalToInternalConverter,
                    const QSharedPointer<AbstractContainerAccess>& keyNestedContainerAccess,
                    PtrOwnerFunction keyOwnerFunction,
                    AbstractContainerAccess::DataType keyDataType,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                    int valueMetaType,
                    size_t valueAlign,
#else
                    const QMetaType& valueMetaType,
#endif
                    const QtJambiUtils::QHashFunction& valueHashFunction,
                    const QtJambiUtils::InternalToExternalConverter& valueInternalToExternalConverter,
                    const QtJambiUtils::ExternalToInternalConverter& valueExternalToInternalConverter,
                    const QSharedPointer<AbstractContainerAccess>& valueNestedContainerAccess,
                    PtrOwnerFunction valueOwnerFunction,
                    AbstractContainerAccess::DataType valueDataType
            );
    void assign(void* container, const void* other) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    int registerContainer(const QByteArray& containerTypeName) override;
    void dispose() override;
    AbstractMultiHashAccess* clone() override;
    const QObject* getOwner(const void* container) override;
    bool hasOwnerFunction() override;
    ContainerAndAccessInfo uniqueKeys(JNIEnv *,const ConstContainerInfo&) override;
    void unite(JNIEnv *,const ContainerInfo&,ContainerAndAccessInfo&) override;
    bool isDetached(const void* container) override;
    void detach(const ContainerInfo& container) override;
    bool isSharedWith(const void* container, const void* container2) override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    const QMetaType& keyMetaType() override;
    const QMetaType& valueMetaType() override;
    DataType keyType() override;
    DataType valueType() override;
    AbstractContainerAccess* keyNestedContainerAccess() override;
    AbstractContainerAccess* valueNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedKeyNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedValueNestedContainerAccess() override;
    bool hasKeyNestedContainerAccess() override;
    bool hasValueNestedContainerAccess() override;
    bool hasKeyNestedPointers() override;
    bool hasValueNestedPointers() override;
    jint capacity(JNIEnv *,const void*) override;
    void clear(JNIEnv *,const ContainerInfo&) override;
    jboolean contains(JNIEnv *,const void*,jobject) override;
    jint count(JNIEnv *,const void*,jobject) override;
    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key) override;
    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override;
    void insert(JNIEnv *,const ContainerInfo&,jobject,jobject) override;
    jobject key(JNIEnv *,const void*,jobject,jobject) override;
    ContainerAndAccessInfo keys(JNIEnv *,const ConstContainerInfo&) override;
    ContainerAndAccessInfo keys(JNIEnv *,const ConstContainerInfo&,jobject) override;
    jboolean equal(JNIEnv *,const void*,jobject) override;
    jint remove(JNIEnv *,const ContainerInfo&,jobject) override;
    void reserve(JNIEnv *,const ContainerInfo&,jint) override;
    qsizetype size(const void*) override;
    jint size(JNIEnv *,const void*) override;
    jobject take(JNIEnv *,const ContainerInfo&,jobject) override;
    jobject value(JNIEnv *,const void*,jobject,jobject) override;
    ContainerAndAccessInfo values(JNIEnv *,const ConstContainerInfo&) override;
    ContainerAndAccessInfo values(JNIEnv *,const ConstContainerInfo&,jobject) override;
    jboolean contains(JNIEnv *,const void*,jobject,jobject) override;
    bool contains(const void*,const void*) override;
    void insert(void* container,const void* key, const void* value) override;
    const void* value(const void*, const void*, const void*) override;
    jint count(JNIEnv *,const void*,jobject,jobject) override;
    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key, jobject value) override;
    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key, jobject value) override;
    jint remove(JNIEnv *,const ContainerInfo&,jobject,jobject) override;
    void replace(JNIEnv *,const ContainerInfo&,jobject,jobject) override;
    bool destructContainer(void* container) override;
    void* constructContainer(void* placement) override;
    void* constructContainer(void* placement, const void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ConstContainerAndAccessInfo& container) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* result, void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ContainerAndAccessInfo& container) override;
#endif
    size_t sizeOf() override;
    std::unique_ptr<AbstractHashAccess::KeyValueIterator> keyValueIterator(const void* container) override;
    std::unique_ptr<AbstractHashAccess::KeyValueIterator> keyValueIterator(void* container) override;
private:
    friend class AutoHashAccess;
    ushort alignOf() const override;
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
    void debugStream(QDebug &s, const void *ptr) override;
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
    bool isMulti() const override;
};

class AutoSetAccess : public AbstractSetAccess, public AbstractNestedSequentialAccess {
    AutoHashAccess m_hashAccess;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    QSharedPointer<QtMetaContainerPrivate::QMetaSequenceInterface> m_metaSequenceInterface;
#endif
protected:
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
                    const QtJambiUtils::QHashFunction& hashFunction,
                    const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
                    const QtJambiUtils::ExternalToInternalConverter& externalToInternalConverter,
                    const QSharedPointer<AbstractContainerAccess>& elementNestedContainerAccess,
                    PtrOwnerFunction elementOwnerFunction,
                    AbstractContainerAccess::DataType elementDataType
            );
    void assign(void*, const void* ) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    int registerContainer(const QByteArray& containerTypeName) override;
    void dispose() override;
    AutoSetAccess* clone() override;
    const QObject* getOwner(const void* container) override;
    bool hasOwnerFunction() override;
    bool isDetached(const void* container) override;
    void detach(const ContainerInfo& container) override;
    bool isSharedWith(const void* container, const void* container2) override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    const QMetaType& elementMetaType() override;
    DataType elementType() override;
    AbstractContainerAccess* elementNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedElementNestedContainerAccess() override;
    bool hasNestedContainerAccess() override;
    bool hasNestedPointers() override;
    jint capacity(JNIEnv * env, const void* container) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void intersect(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    jboolean intersects(JNIEnv * env, const void* container, jobject other) override;
    jboolean equal(JNIEnv * env, const void* container, jobject other) override;
    jboolean remove(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void reserve(JNIEnv * env, const ContainerInfo& container, jint newSize) override;
    jint size(JNIEnv * env, const void* container) override;
    qsizetype size(const void* container) override;
    void subtract(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    ContainerAndAccessInfo values(JNIEnv * env, const ConstContainerInfo& container) override;
    void* constructContainer(void* placement) override;
    void* constructContainer(void* placement, const void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ConstContainerAndAccessInfo& container) override;
    bool destructContainer(void* container) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* result, void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ContainerAndAccessInfo& container) override;
#endif
    size_t sizeOf() override;
    std::unique_ptr<AbstractSetAccess::ElementIterator> elementIterator(const void* container) override;
    std::unique_ptr<AbstractSetAccess::ElementIterator> elementIterator(void* container) override;
private:
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    typedef QHashData::Node Node;
#else
    static QtMetaContainerPrivate::QMetaSequenceInterface* createMetaSequenceInterface(int newMetaType);
    typedef AutoHashAccess::QHashData QHashData;
    typedef AutoHashAccess::iterator iterator;
#endif
};

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)

#include <QtCore/QVector>

class AutoVectorAccess : public AbstractVectorAccess, public AbstractNestedSequentialAccess {
    QMetaType m_elementMetaType;
    size_t m_elementAlign;
    QtJambiUtils::QHashFunction m_hashFunction;
    QtJambiUtils::InternalToExternalConverter m_internalToExternalConverter;
    QtJambiUtils::ExternalToInternalConverter m_externalToInternalConverter;
    QSharedPointer<AbstractContainerAccess> m_elementNestedContainerAccess;
    size_t m_offset;
    bool m_isComplex;
    PtrOwnerFunction m_elementOwnerFunction;
    AbstractContainerAccess::DataType m_elementDataType;

    jobject createIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr);
    jobject createConstIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr);
    void detach(QTypedArrayData<char> ** vector);
    bool isDetached(QTypedArrayData<char> *const* vector);
    void realloc(QTypedArrayData<char> ** vector, int aalloc, QArrayData::AllocationOptions options = QArrayData::Default);
    QTypedArrayData<char>* unsharableEmpty();
    void erase(QTypedArrayData<char> ** vector, char* begin, char* end);
protected:
    AutoVectorAccess(const AutoVectorAccess&);
public:
    ~AutoVectorAccess() override;
    AutoVectorAccess(
                    int elementMetaType,
                    size_t elementAlign,
                    const QtJambiUtils::QHashFunction& hashFunction,
                    const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
                    const QtJambiUtils::ExternalToInternalConverter& externalToInternalConverter,
                    const QSharedPointer<AbstractContainerAccess>& elementNestedContainerAccess,
                    PtrOwnerFunction elementOwnerFunction,
                    AbstractContainerAccess::DataType elementDataType
            );
    size_t sizeOf() override;
    void* constructContainer(JNIEnv * env, void* result, const ConstContainerAndAccessInfo& container) override;
    void* constructContainer(void* placement) override;
    void* constructContainer(void* placement, const void* container) override;
    void assign(void* container, const void* other) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    bool destructContainer(void* container) override;
    int registerContainer(const QByteArray& containerTypeName) override;
    void dispose() override;
    bool isDetached(const void* container) override;
    void detach(const ContainerInfo& container) override;
    bool isSharedWith(const void* container, const void* container2) override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    AutoVectorAccess* clone() override;
    const QObject* getOwner(const void* container) override;
    bool hasOwnerFunction() override;
    const QMetaType& elementMetaType() override;
    DataType elementType() override;
    AbstractContainerAccess* elementNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedElementNestedContainerAccess() override;
    bool hasNestedContainerAccess() override;
    bool hasNestedPointers() override;
    void appendVector(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) override;
    jobject at(JNIEnv * env, const void* container, jint index) override;
    jobject value(JNIEnv * env, const void* container, jint index) override;
    jobject value(JNIEnv * env, const void* container, jint index, jobject defaultValue) override;
    void swapItemsAt(JNIEnv * env, const ContainerInfo& container, jint index1, jint index2) override;
    jboolean startsWith(JNIEnv * env, const void* container, jobject value) override;
    jint size(JNIEnv * env, const void* container) override;
    qsizetype size(const void* container) override;
    void reserve(JNIEnv * env, const ContainerInfo& container, jint size) override;
    void replace(JNIEnv * env, const ContainerInfo& container, jint index, jobject value) override;
    jint removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    jboolean equal(JNIEnv * env, const void* container, jobject other) override;
    void move(JNIEnv * env, const ContainerInfo& container, jint index1, jint index2) override;
    ContainerAndAccessInfo mid(JNIEnv * env, const ConstContainerAndAccessInfo& container, jint index1, jint index2) override;
    jint lastIndexOf(JNIEnv * env, const void* container, jobject value, jint index) override;
    jint indexOf(JNIEnv * env, const void* container, jobject value, jint index) override;
    jboolean endsWith(JNIEnv * env, const void* container, jobject value) override;
    jint count(JNIEnv * env, const void* container, jobject value) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jint capacity(JNIEnv * env, const void* container) override;
    void fill(JNIEnv * env, const ContainerInfo& container, jobject value, jint size) override;
    void remove(JNIEnv * env, const ContainerInfo& container, jint index, jint n) override;
    void insert(JNIEnv * env, const ContainerInfo& container, jint index, jint n, jobject value) override;
    void resize(JNIEnv * env, const ContainerInfo& container, jint newSize) override;
    void squeeze(JNIEnv * env, const ContainerInfo& container) override;
    std::unique_ptr<AbstractVectorAccess::ElementIterator> elementIterator(const void* container) override;
    std::unique_ptr<AbstractVectorAccess::ElementIterator> elementIterator(void* container) override;
};

#include <QtCore/QLinkedList>

class AutoLinkedListAccess : public AbstractLinkedListAccess, public AbstractNestedSequentialAccess {
    QMetaType m_elementMetaType;
    QtJambiUtils::QHashFunction m_hashFunction;
    QtJambiUtils::InternalToExternalConverter m_internalToExternalConverter;
    QtJambiUtils::ExternalToInternalConverter m_externalToInternalConverter;
    QSharedPointer<AbstractContainerAccess> m_elementNestedContainerAccess;
    size_t m_offset;
    PtrOwnerFunction m_elementOwnerFunction;
    AbstractContainerAccess::DataType m_elementDataType;

    typedef QLinkedListNode<char> Node;
protected:
    AutoLinkedListAccess(const AutoLinkedListAccess& other);
public:
    AutoLinkedListAccess(
            int elementMetaType,
            size_t elementAlign,
            const QtJambiUtils::QHashFunction& hashFunction,
            const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
            const QtJambiUtils::ExternalToInternalConverter& externalToInternalConverter,
            const QSharedPointer<AbstractContainerAccess>& elementNestedContainerAccess,
            PtrOwnerFunction elementOwnerFunction,
            AbstractContainerAccess::DataType elementDataType
            );
    AutoLinkedListAccess* clone() override;
    const QObject* getOwner(const void* container) override;
    bool hasOwnerFunction() override;
    size_t sizeOf() override;
    void* constructContainer(void* placement) override;
    void* constructContainer(void* placement, const void* container) override;
    void* constructContainer(JNIEnv * env, void* result, const ConstContainerAndAccessInfo& container) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void assign(void* container, const void* other) override;
    bool destructContainer(void* container) override;
    int registerContainer(const QByteArray& containerTypeName) override;
    void dispose() override;
    bool isDetached(const void* container) override;
    void detach(const ContainerInfo& container) override;
    bool isSharedWith(const void* container, const void* container2) override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    const QMetaType& elementMetaType() override;
    DataType elementType() override;
    AbstractContainerAccess* elementNestedContainerAccess() override;
    const QSharedPointer<AbstractContainerAccess>& sharedElementNestedContainerAccess() override;
    bool hasNestedContainerAccess() override;
    bool hasNestedPointers() override;
    void append(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    jobject first(JNIEnv * env, const void* container) override;
    jobject last(JNIEnv * env, const void* container) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    jint count(JNIEnv * env, const void* container, jobject value) override;
    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override;
    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override;
    jboolean endsWith(JNIEnv * env, const void* container, jobject value) override;
    jboolean equal(JNIEnv * env, const void* container, jobject other) override;
    void prepend(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void removeFirst(JNIEnv * env, const ContainerInfo& container) override;
    jint removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void removeLast(JNIEnv * env, const ContainerInfo& container) override;
    jboolean removeOne(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    jint size(JNIEnv * env, const void* container) override;
    qsizetype size(const void* container) override;
    jboolean startsWith(JNIEnv * env, const void* container, jobject value) override;
    jobject takeFirst(JNIEnv * env, const ContainerInfo& container) override;
    jobject takeLast(JNIEnv * env, const ContainerInfo& container) override;
    std::unique_ptr<AbstractLinkedListAccess::ElementIterator> elementIterator(const void* container) override;
    std::unique_ptr<AbstractLinkedListAccess::ElementIterator> elementIterator(void* container) override;
private:
    jobject createIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr);
    jobject createConstIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr);
    void detach(QLinkedListData*& d);
    void detach_helper(QLinkedListData*& d);
    QLinkedList<char>::iterator detach_helper2(QLinkedListData*& d, QLinkedList<char>::iterator iter);
    void freeData(QLinkedListData*& v);
    void erase(QLinkedListData*& d, Node *i);
};

#endif

class PointerRCAutoListAccess : public AutoListAccess, public ReferenceCountingSetContainer{
private:
    PointerRCAutoListAccess(PointerRCAutoListAccess& _this);
public:
    using AutoListAccess::AutoListAccess;
    PointerRCAutoListAccess* clone() override;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    AutoSpanAccess* createSpanAccess(bool isConst) override;
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void appendList(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) override;
    void replace(JNIEnv * env, const ContainerInfo& container, jint index, jobject value) override;
    jint removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void remove(JNIEnv * env, const ContainerInfo& container, jint index, jint n) override;
    void insert(JNIEnv * env, const ContainerInfo& container, jint index, jint n, jobject value) override;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    void fill(JNIEnv * env, const ContainerInfo& container, jobject value, jint size) override;
#endif
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
};

class NestedPointersRCAutoListAccess : public AutoListAccess, public ReferenceCountingSetContainer{
private:
    NestedPointersRCAutoListAccess(NestedPointersRCAutoListAccess& _this);
public:
    using AutoListAccess::AutoListAccess;
    NestedPointersRCAutoListAccess* clone() override;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    AutoSpanAccess* createSpanAccess(bool isConst) override;
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void appendList(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) override;
    void replace(JNIEnv * env, const ContainerInfo& container, jint index, jobject value) override;
    jint removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    ContainerAndAccessInfo mid(JNIEnv * env, const ConstContainerAndAccessInfo& container, jint index1, jint index2) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void remove(JNIEnv * env, const ContainerInfo& container, jint index, jint n) override;
    void insert(JNIEnv * env, const ContainerInfo& container, jint index, jint n, jobject value) override;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    void fill(JNIEnv * env, const ContainerInfo& container, jobject value, jint size) override;
#endif
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
};

class PointerRCAutoSetAccess : public AutoSetAccess, public ReferenceCountingSetContainer{
private:
    PointerRCAutoSetAccess(PointerRCAutoSetAccess& _this);
public:
    using AutoSetAccess::AutoSetAccess;
    PointerRCAutoSetAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void insert(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    jboolean remove(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void intersect(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void subtract(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
};

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
class PointerRCAutoLinkedListAccess : public AutoLinkedListAccess, public ReferenceCountingSetContainer{
private:
    PointerRCAutoLinkedListAccess(PointerRCAutoLinkedListAccess& _this);
public:
    using AutoLinkedListAccess::AutoLinkedListAccess;
    PointerRCAutoLinkedListAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void append(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void prepend(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void removeFirst(JNIEnv * env, const ContainerInfo& container) override;
    void removeLast(JNIEnv * env, const ContainerInfo& container) override;
    jint removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    jboolean removeOne(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    jobject takeFirst(JNIEnv * env, const ContainerInfo& container) override;
    jobject takeLast(JNIEnv * env, const ContainerInfo& container) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
};

class PointerRCAutoVectorAccess : public AutoVectorAccess, public ReferenceCountingSetContainer{
private:
    PointerRCAutoVectorAccess(PointerRCVectorAccess& _this);
public:
    using AutoVectorAccess::AutoVectorAccess;
    PointerRCAutoVectorAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void appendVector(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) override;
    void replace(JNIEnv * env, const ContainerInfo& container, jint index, jobject value) override;
    jint removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void remove(JNIEnv * env, const ContainerInfo& container, jint index, jint n) override;
    void insert(JNIEnv * env, const ContainerInfo& container, jint index, jint n, jobject value) override;
    void fill(JNIEnv * env, const ContainerInfo& container, jobject value, jint size) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
};
#endif

class KeyPointerRCAutoMapAccess : public AutoMapAccess, public ReferenceCountingSetContainer{
private:
    KeyPointerRCAutoMapAccess(KeyPointerRCAutoMapAccess& _this);
public:
    using AutoMapAccess::AutoMapAccess;
    KeyPointerRCAutoMapAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
};

class KeyPointerRCAutoMultiMapAccess : public AutoMultiMapAccess, public ReferenceCountingSetContainer{
private:
    KeyPointerRCAutoMultiMapAccess(KeyPointerRCAutoMultiMapAccess& _this);
public:
    using AutoMultiMapAccess::AutoMultiMapAccess;
    AbstractMultiMapAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    jint remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) override;
    void unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject value) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
};

class KeyPointerRCAutoHashAccess : public AutoHashAccess, public ReferenceCountingSetContainer{
private:
    KeyPointerRCAutoHashAccess(KeyPointerRCAutoHashAccess& _this);
public:
    using AutoHashAccess::AutoHashAccess;
    KeyPointerRCAutoHashAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
};

class KeyPointerRCAutoMultiHashAccess : public AutoMultiHashAccess, public ReferenceCountingSetContainer{
private:
    KeyPointerRCAutoMultiHashAccess(KeyPointerRCAutoMultiHashAccess& _this);
public:
    using AutoMultiHashAccess::AutoMultiHashAccess;
    AbstractMultiHashAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    jint remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) override;
    void unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject value) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
};

class ValuePointerRCAutoMapAccess : public AutoMapAccess, public ReferenceCountingSetContainer{
private:
    ValuePointerRCAutoMapAccess(ValuePointerRCAutoMapAccess& _this);
public:
    using AutoMapAccess::AutoMapAccess;
    ValuePointerRCAutoMapAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
};

class ValuePointerRCAutoMultiMapAccess : public AutoMultiMapAccess, public ReferenceCountingSetContainer{
private:
    ValuePointerRCAutoMultiMapAccess(ValuePointerRCAutoMultiMapAccess& _this);
public:
    using AutoMultiMapAccess::AutoMultiMapAccess;
    AbstractMultiMapAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    jint remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) override;
    void unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject value) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
};

class ValuePointerRCAutoHashAccess : public AutoHashAccess, public ReferenceCountingSetContainer{
private:
    ValuePointerRCAutoHashAccess(ValuePointerRCAutoHashAccess& _this);
public:
    using AutoHashAccess::AutoHashAccess;
    ValuePointerRCAutoHashAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
};

class ValuePointerRCAutoMultiHashAccess : public AutoMultiHashAccess, public ReferenceCountingSetContainer{
private:
    ValuePointerRCAutoMultiHashAccess(ValuePointerRCAutoMultiHashAccess& _this);
public:
    using AutoMultiHashAccess::AutoMultiHashAccess;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
    AbstractMultiHashAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    jint remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) override;
    void unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject value) override;
};

class PointersRCAutoMapAccess : public AutoMapAccess, public ReferenceCountingMapContainer{
private:
    PointersRCAutoMapAccess(PointersRCAutoMapAccess& _this);
public:
    using AutoMapAccess::AutoMapAccess;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
    PointersRCAutoMapAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
};

class PointersRCAutoMultiMapAccess : public AutoMultiMapAccess, public ReferenceCountingMultiMapContainer{
private:
    PointersRCAutoMultiMapAccess(PointersRCAutoMultiMapAccess& _this);
public:
    using AutoMultiMapAccess::AutoMultiMapAccess;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
    AbstractMultiMapAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    jint remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) override;
    void unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject value) override;
};

class PointersRCAutoHashAccess : public AutoHashAccess, public ReferenceCountingMapContainer{
private:
    PointersRCAutoHashAccess(PointersRCAutoHashAccess& _this);
public:
    using AutoHashAccess::AutoHashAccess;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
    PointersRCAutoHashAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
};

class PointersRCAutoMultiHashAccess : public AutoMultiHashAccess, public ReferenceCountingMultiMapContainer{
private:
    PointersRCAutoMultiHashAccess(PointersRCAutoMultiHashAccess& _this);
public:
    using AutoMultiHashAccess::AutoMultiHashAccess;
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
    AbstractMultiHashAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    jint remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) override;
    void unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject value) override;
};

class NestedPointersRCAutoSetAccess : public AutoSetAccess, public ReferenceCountingSetContainer{
private:
    NestedPointersRCAutoSetAccess(NestedPointersRCAutoSetAccess& _this);
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
public:
    using AutoSetAccess::AutoSetAccess;
    NestedPointersRCAutoSetAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void insert(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    jboolean remove(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void intersect(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void subtract(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
};

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
class NestedPointersRCAutoLinkedListAccess : public AutoLinkedListAccess, public ReferenceCountingSetContainer{
private:
    NestedPointersRCAutoLinkedListAccess(NestedPointersRCAutoLinkedListAccess& _this);
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
public:
    using AutoLinkedListAccess::AutoLinkedListAccess;
    NestedPointersRCAutoLinkedListAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void append(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void prepend(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void removeFirst(JNIEnv * env, const ContainerInfo& container) override;
    void removeLast(JNIEnv * env, const ContainerInfo& container) override;
    jint removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    jobject takeFirst(JNIEnv * env, const ContainerInfo& container) override;
    jobject takeLast(JNIEnv * env, const ContainerInfo& container) override;
    jboolean removeOne(JNIEnv * env, const ContainerInfo& container, jobject value) override;
};

class NestedPointersRCAutoVectorAccess : public AutoVectorAccess, public ReferenceCountingSetContainer{
private:
    NestedPointersRCAutoVectorAccess(NestedPointersRCAutoVectorAccess& _this);
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
public:
    using AutoVectorAccess::AutoVectorAccess;
    NestedPointersRCAutoVectorAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void appendVector(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) override;
    void replace(JNIEnv * env, const ContainerInfo& container, jint index, jobject value) override;
    jint removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void remove(JNIEnv * env, const ContainerInfo& container, jint index, jint n) override;
    void insert(JNIEnv * env, const ContainerInfo& container, jint index, jint n, jobject value) override;
    void fill(JNIEnv * env, const ContainerInfo& container, jobject value, jint size) override;
};
#endif

class NestedPointersRCAutoMapAccess : public AutoMapAccess, public ReferenceCountingSetContainer{
private:
    NestedPointersRCAutoMapAccess(NestedPointersRCAutoMapAccess& _this);
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
public:
    using AutoMapAccess::AutoMapAccess;
    NestedPointersRCAutoMapAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
};

class NestedPointersRCAutoMultiMapAccess : public AutoMultiMapAccess, public ReferenceCountingSetContainer{
private:
    NestedPointersRCAutoMultiMapAccess(NestedPointersRCAutoMultiMapAccess& _this);
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
public:
    using AutoMultiMapAccess::AutoMultiMapAccess;
    AbstractMultiMapAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    jint remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) override;
    void unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject value) override;
};

class NestedPointersRCAutoHashAccess : public AutoHashAccess, public ReferenceCountingSetContainer{
private:
    NestedPointersRCAutoHashAccess(NestedPointersRCAutoHashAccess& _this);
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
public:
    using AutoHashAccess::AutoHashAccess;
    NestedPointersRCAutoHashAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
};

class NestedPointersRCAutoMultiHashAccess : public AutoMultiHashAccess, public ReferenceCountingSetContainer{
private:
    NestedPointersRCAutoMultiHashAccess(NestedPointersRCAutoMultiHashAccess& _this);
    void updateRC(JNIEnv * env, const ContainerInfo& container) override;
public:
    using AutoMultiHashAccess::AutoMultiHashAccess;
    AbstractMultiHashAccess* clone() override;
    void swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override;
    void assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override;
    void clear(JNIEnv * env, const ContainerInfo& container) override;
    void insert(JNIEnv * env, const ContainerInfo& container,jobject key,jobject value) override;
    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key) override;
    jobject take(JNIEnv *env, const ContainerInfo& container,jobject key) override;
    jint remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) override;
    void unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override;
    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject value) override;
};

#endif // CONTAINERACCESS_P_H
