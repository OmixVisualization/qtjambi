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


#ifndef CONTAINERACCESS_P_H
#define CONTAINERACCESS_P_H

#include <QtCore/QtGlobal>
#include <QtCore/QThreadStorage>

QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wextra")
QT_WARNING_DISABLE_CLANG("-Wextra")

#include "containeraccess.h"
#include "jobjectwrapper.h"
#include "utils_p.h"

void registerContainerConverter(SequentialContainerType collectionType, const QMetaType& containerMetaType, const QMetaType& elementMetaType);
void registerContainerConverter(AssociativeContainerType mapType, const QMetaType& containerMetaType, const QMetaType& keyMetaType, const QMetaType& valueMetaType);
void registerContainerConverter(QSharedPointer<AbstractPairAccess> pairAccess, const QMetaType& containerMetaType);

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

int registerContainerMetaType(const QByteArray& typeName,
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
    WrapperListAccess(AbstractListAccess* containerAccess);
    ~WrapperListAccess() override;
    AbstractListAccess* clone() override;
    void dispose() final override;
    void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) final override;
    size_t sizeOf() final override;
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    void* constructContainer(void* placement, void* move) override;
#endif
    bool destructContainer(void* container) final override;
    void assign(void* container, const void* other) override;
    int registerContainer(const QByteArray& containerTypeName) final override;
    bool isConstant() final override;
    const QMetaType& elementMetaType() final override;
    void appendList(JNIEnv * env, void* container, jobject list) override;
    jobject at(JNIEnv * env, const void* container, jint index) override;
    jobject value(JNIEnv * env, const void* container, jint index) override;
    jobject value(JNIEnv * env, const void* container, jint index, jobject defaultValue) override;
    void swapItemsAt(JNIEnv * env, void* container, jint index1, jint index2) override;
    jboolean startsWith(JNIEnv * env, const void* container, jobject value) override;
    jint size(JNIEnv * env, const void* container) override;
    void reserve(JNIEnv * env, void* container, jint size) override;
    void replace(JNIEnv * env, void* container, jint index, jobject value) override;
    jint removeAll(JNIEnv * env, void* container, jobject value) override;
    jboolean equal(JNIEnv * env, const void* container, jobject other) override;
    void move(JNIEnv * env, void* container, jint index1, jint index2) override;
    jobject mid(JNIEnv * env, const void* container, jint index1, jint index2) override;
    jint lastIndexOf(JNIEnv * env, const void* container, jobject value, jint index) override;
    jint indexOf(JNIEnv * env, const void* container, jobject value, jint index) override;
    jboolean endsWith(JNIEnv * env, const void* container, jobject value) override;
    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) override;
    jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) override;
    jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jint count(JNIEnv * env, const void* container, jobject value) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    void clear(JNIEnv * env, void* container) override;
    void remove(JNIEnv * env, void* container, jint index, jint n) override;
    void insert(JNIEnv * env, void* container, jint index, jint n, jobject value) override;

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    jint capacity(JNIEnv * env, const void* container) override;
    void fill(JNIEnv * env, void* container, jobject value, jint size) override;
    void resize(JNIEnv * env, void* container, jint newSize) override;
    void squeeze(JNIEnv * env, void* container) override;
#endif
    Q_DISABLE_COPY_MOVE(WrapperListAccess)
private:
    AbstractListAccess* m_containerAccess;
};

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
class WrapperVectorAccess : public AbstractVectorAccess{
public:
    WrapperVectorAccess(AbstractVectorAccess* containerAccess);
    ~WrapperVectorAccess() override;
    AbstractVectorAccess* clone() override;
    void dispose() override;
    void assign(void* container, const void* other) override;
    size_t sizeOf() final override;
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    void* constructContainer(void* placement, void* move) override;
#endif
    bool destructContainer(void* container) final override;
    int registerContainer(const QByteArray& containerTypeName) override;
    bool isConstant() override;
    const QMetaType& elementMetaType() override;
    void appendVector(JNIEnv * env, void* container, jobject list) override;
    jobject at(JNIEnv * env, const void* container, jint index) override;
    jobject value(JNIEnv * env, const void* container, jint index) override;
    jobject value(JNIEnv * env, const void* container, jint index, jobject defaultValue) override;
    void swapItemsAt(JNIEnv * env, void* container, jint index1, jint index2) override;
    jboolean startsWith(JNIEnv * env, const void* container, jobject value) override;
    jint size(JNIEnv * env, const void* container) override;
    void reserve(JNIEnv * env, void* container, jint size) override;
    void replace(JNIEnv * env, void* container, jint index, jobject value) override;
    jint removeAll(JNIEnv * env, void* container, jobject value) override;
    jboolean equal(JNIEnv * env, const void* container, jobject other) override;
    void move(JNIEnv * env, void* container, jint index1, jint index2) override;
    jobject mid(JNIEnv * env, const void* container, jint index1, jint index2) override;
    jint lastIndexOf(JNIEnv * env, const void* container, jobject value, jint index) override;
    jint indexOf(JNIEnv * env, const void* container, jobject value, jint index) override;
    jboolean endsWith(JNIEnv * env, const void* container, jobject value) override;
    jint count(JNIEnv * env, const void* container, jobject value) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    void clear(JNIEnv * env, void* container) override;
    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) override;
    jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) override;
    jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jint capacity(JNIEnv * env, const void* container) override;
    void fill(JNIEnv * env, void* container, jobject value, jint size) override;
    void remove(JNIEnv * env, void* container, jint index, jint n) override;
    void insert(JNIEnv * env, void* container, jint index, jint n, jobject value) override;
    void resize(JNIEnv * env, void* container, jint newSize) override;
    void squeeze(JNIEnv * env, void* container) override;
    void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) override;
    Q_DISABLE_COPY_MOVE(WrapperVectorAccess)
private:
    AbstractVectorAccess* m_containerAccess;
};

class WrapperLinkedListAccess : public AbstractLinkedListAccess{
public:
    WrapperLinkedListAccess(AbstractLinkedListAccess* containerAccess);
    ~WrapperLinkedListAccess() override;
    AbstractLinkedListAccess* clone() override;
    void dispose() override;
    void assign(void* container, const void* other) override;
    size_t sizeOf() final override;
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
    bool destructContainer(void* container) final override;
    int registerContainer(const QByteArray& containerTypeName) override;
    bool isConstant() override;
    const QMetaType& elementMetaType() override;
    void append(JNIEnv * env, void* container, jobject value) override;
    jobject first(JNIEnv * env, const void* container) override;
    jobject last(JNIEnv * env, const void* container) override;
    void clear(JNIEnv * env, void* container) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    jint count(JNIEnv * env, const void* container, jobject value) override;
    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) override;
    jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) override;
    jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
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
    void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) override;
    Q_DISABLE_COPY_MOVE(WrapperLinkedListAccess)
private:
    AbstractLinkedListAccess* m_containerAccess;
};
#endif

class WrapperSetAccess : public AbstractSetAccess{
public:
    WrapperSetAccess(AbstractSetAccess* containerAccess);
    ~WrapperSetAccess() override;
    AbstractSetAccess* clone() override;
    void dispose() override;
    void assign(void* container, const void* other) override;
    size_t sizeOf() final override;
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    void* constructContainer(void* placement, void* move) override;
#endif
    bool destructContainer(void* container) final override;
    int registerContainer(const QByteArray& containerTypeName) override;
    bool isConstant() override;
    const QMetaType& elementMetaType() override;
    jint capacity(JNIEnv * env, const void* container) override;
    void clear(JNIEnv * env, void* container) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
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
    void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) override;
    Q_DISABLE_COPY_MOVE(WrapperSetAccess)
private:
    AbstractSetAccess* m_containerAccess;
};

class WrapperMapAccess : public AbstractMapAccess{
public:
    WrapperMapAccess(AbstractMapAccess* containerAccess);
    ~WrapperMapAccess() override;
    AbstractMapAccess* clone() override;
    void dispose() override;
    void assign(void* container, const void* other) override;
    size_t sizeOf() final override;
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    void* constructContainer(void* placement, void* move) override;
#endif
    bool destructContainer(void* container) final override;
    int registerContainer(const QByteArray& containerTypeName) override;
    bool isConstant() override;
    const QMetaType& keyMetaType() override;
    const QMetaType& valueMetaType() override;
    void clear(JNIEnv * env, void* container) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    jint count(JNIEnv * env, const void* container, jobject key) override;
    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) override;
    jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) override;
    jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jobject find(JNIEnv * env, QtJambiNativeID ownerId, void* container, jobject key) override;
    jobject constFind(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override;
    jobject first(JNIEnv * env, const void* container) override;
    jobject firstKey(JNIEnv * env, const void* container) override;
    void insert(JNIEnv * env, void* container,jobject key,jobject value) override;
    jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override;
    jobject keys(JNIEnv * env, const void* container) override;
    jobject keys(JNIEnv * env, const void* container, jobject value) override;
    jobject last(JNIEnv * env, const void* container) override;
    jobject lastKey(JNIEnv * env, const void* container) override;
    jobject constLowerBound(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override;
    jboolean equal(JNIEnv * env, const void* container, jobject other) override;
    jint remove(JNIEnv * env, void* container,jobject key) override;
    jint size(JNIEnv * env, const void* container) override;
    jobject take(JNIEnv *env, void* container,jobject key) override;
    jobject constUpperBound(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override;
    jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override;
    jobject values(JNIEnv * env, const void* container) override;
    bool keyLessThan(JNIEnv *env, jobject key1, jobject key2) override;
    void analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) override;
    Q_DISABLE_COPY_MOVE(WrapperMapAccess)
private:
    AbstractMapAccess* m_containerAccess;
};

class WrapperMultiMapAccess : public AbstractMultiMapAccess{
public:
    WrapperMultiMapAccess(AbstractMultiMapAccess* containerAccess);
    ~WrapperMultiMapAccess() override;
    AbstractMultiMapAccess* clone() override;
    void dispose() override;
    void assign(void* container, const void* other) override;
    size_t sizeOf() final override;
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    void* constructContainer(void* placement, void* move) override;
#endif
    bool destructContainer(void* container) final override;
    int registerContainer(const QByteArray& containerTypeName) override;
    bool isConstant() override;
    const QMetaType& keyMetaType() override;
    const QMetaType& valueMetaType() override;
    void clear(JNIEnv * env, void* container) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    jint count(JNIEnv * env,const void* container, jobject key) override;
    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) override;
    jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) override;
    jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jobject find(JNIEnv * env, QtJambiNativeID ownerId, void* container, jobject key) override;
    jobject constFind(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override;
    jobject first(JNIEnv * env, const void* container) override;
    jobject firstKey(JNIEnv * env, const void* container) override;
    void insert(JNIEnv *env, void* container,jobject key,jobject value) override;
    jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override;
    jobject keys(JNIEnv * env, const void* container) override;
    jobject keys(JNIEnv * env, const void* container, jobject value) override;
    jobject last(JNIEnv * env, const void* container) override;
    jobject lastKey(JNIEnv * env, const void* container) override;
    jobject constLowerBound(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override;
    jboolean equal(JNIEnv * env, const void* container, jobject other) override;
    jint remove(JNIEnv * env, void* container, jobject key, jobject value) override;
    jint size(JNIEnv * env, const void* container) override;
    jobject take(JNIEnv *env, void* container,jobject key) override;
    jobject constUpperBound(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override;
    jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override;
    jobject values(JNIEnv * env, const void* container) override;
    bool keyLessThan(JNIEnv *env, jobject key1, jobject key2) override;
    jobject uniqueKeys(JNIEnv * env, const void* container) override;
    void unite(JNIEnv *env, void* container, jobject other) override;
    jobject values(JNIEnv * env, const void* container, jobject key) override;
    jboolean contains(JNIEnv * env, const void* container, jobject key, jobject value) override;
    jint count(JNIEnv *env, const void* container, jobject key, jobject value) override;
    jobject find(JNIEnv * env, QtJambiNativeID ownerId, void* container, jobject key, jobject value) override;
    jobject constFind(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key, jobject value) override;
    jint remove(JNIEnv * env, void* container,jobject key) override;
    void replace(JNIEnv * env, void* container,jobject key, jobject value) override;
    void analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) override;
    Q_DISABLE_COPY_MOVE(WrapperMultiMapAccess)
private:
    AbstractMultiMapAccess* m_containerAccess;
};

class WrapperHashAccess : public AbstractHashAccess{
public:
    WrapperHashAccess(AbstractHashAccess* containerAccess);
    ~WrapperHashAccess() override;
    AbstractHashAccess* clone() override;
    void dispose() override;
    size_t sizeOf() final override;
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    void* constructContainer(void* placement, void* move) override;
#endif
    bool destructContainer(void* container) final override;
    void assign(void* container, const void* other) override;
    int registerContainer(const QByteArray& containerTypeName) override;
    bool isConstant() override;
    const QMetaType& keyMetaType() override;
    const QMetaType& valueMetaType() override;
    jint capacity(JNIEnv * env,const void* container) override;
    void clear(JNIEnv * env, void* container) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    jint count(JNIEnv * env, const void* container, jobject key) override;
    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) override;
    jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) override;
    jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jobject find(JNIEnv * env, QtJambiNativeID ownerId, void* container, jobject key) override;
    jobject constFind(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override;
    void insert(JNIEnv *env, void* container,jobject key,jobject value) override;
    jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override;
    jobject keys(JNIEnv * env, const void* container) override;
    jobject keys(JNIEnv * env, const void* container, jobject value) override;
    jboolean equal(JNIEnv * env, const void* container, jobject other) override;
    jint remove(JNIEnv * env, void* container,jobject key) override;
    void reserve(JNIEnv * env,void* container, jint newSize) override;
    jint size(JNIEnv * env, const void* container) override;
    jobject take(JNIEnv *env, void* container, jobject key) override;
    jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override;
    jobject values(JNIEnv * env, const void* container) override;
    void analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) override;
    Q_DISABLE_COPY_MOVE(WrapperHashAccess)
private:
    AbstractHashAccess* m_containerAccess;
};

class WrapperMultiHashAccess : public AbstractMultiHashAccess{
public:
    WrapperMultiHashAccess(AbstractMultiHashAccess* containerAccess);
    ~WrapperMultiHashAccess() override;
    AbstractMultiHashAccess* clone() override;
    void dispose() override;
    void assign(void* container, const void* other) override;
    size_t sizeOf() final override;
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    void* constructContainer(void* placement, void* move) override;
#endif
    bool destructContainer(void* container) final override;
    int registerContainer(const QByteArray& containerTypeName) override;
    bool isConstant() override;
    const QMetaType& keyMetaType() override;
    const QMetaType& valueMetaType() override;
    jint capacity(JNIEnv * env,const void* container) override;
    void clear(JNIEnv * env, void* container) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    jint count(JNIEnv * env, const void* container, jobject key) override;
    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) override;
    jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) override;
    jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jobject find(JNIEnv * env, QtJambiNativeID ownerId, void* container, jobject key) override;
    jobject constFind(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override;
    void insert(JNIEnv *env, void* container,jobject key,jobject value) override;
    jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override;
    jobject keys(JNIEnv * env, const void* container) override;
    jobject keys(JNIEnv * env, const void* container, jobject value) override;
    jboolean equal(JNIEnv * env, const void* container, jobject other) override;
    jint remove(JNIEnv * env, void* container,jobject key) override;
    void reserve(JNIEnv * env,void* container, jint newSize) override;
    jint size(JNIEnv * env, const void* container) override;
    jobject take(JNIEnv *env, void* container, jobject key) override;
    jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override;
    jobject values(JNIEnv * env, const void* container) override;
    jobject values(JNIEnv * env, const void* container, jobject key) override;
    jboolean contains(JNIEnv * env, const void* container, jobject key, jobject value) override;
    jint count(JNIEnv *env, const void* container, jobject key, jobject value) override;
    jobject find(JNIEnv * env, QtJambiNativeID ownerId, void* container, jobject key, jobject value) override;
    jobject constFind(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key, jobject value) override;
    jint remove(JNIEnv * env, void* container, jobject key, jobject value) override;
    void replace(JNIEnv * env, void* container, jobject key, jobject value) override;
    jobject uniqueKeys(JNIEnv * env,const void* container) override;
    void unite(JNIEnv * env, void* container, jobject other) override;
    void analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) override;
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
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
    void appendList(JNIEnv * env, void* container, jobject list) override;
    void replace(JNIEnv * env, void* container, jint index, jobject value) override;
    jint removeAll(JNIEnv * env, void* container, jobject value) override;
    jobject mid(JNIEnv * env, const void* container, jint index1, jint index2) override;
    void clear(JNIEnv * env, void* container) override;
    void remove(JNIEnv * env, void* container, jint index, jint n) override;
    void insert(JNIEnv * env, void* container, jint index, jint n, jobject value) override;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    void* constructContainer(void* placement, void* moved) override;
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
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* moved) override;
#endif
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
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
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
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
    void appendVector(JNIEnv * env, void* container, jobject list) override;
    void replace(JNIEnv * env, void* container, jint index, jobject value) override;
    jint removeAll(JNIEnv * env, void* container, jobject value) override;
    jobject mid(JNIEnv * env, const void* container, jint index1, jint index2) override;
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
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* moved) override;
#endif
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
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* moved) override;
#endif
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
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* moved) override;
#endif
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
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* moved) override;
#endif
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
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* moved) override;
#endif
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
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* moved) override;
#endif
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
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* moved) override;
#endif
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
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* moved) override;
#endif
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
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* moved) override;
#endif
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
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* moved) override;
#endif
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
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* moved) override;
#endif
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
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* moved) override;
#endif
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
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* moved) override;
#endif
    void appendList(JNIEnv * env, void* container, jobject list) override;
    void replace(JNIEnv * env, void* container, jint index, jobject value) override;
    jint removeAll(JNIEnv * env, void* container, jobject value) override;
    jobject mid(JNIEnv * env, const void* container, jint index1, jint index2) override;
    void clear(JNIEnv * env, void* container) override;
    void remove(JNIEnv * env, void* container, jint index, jint n) override;
    void insert(JNIEnv * env, void* container, jint index, jint n, jobject value) override;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
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
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* moved) override;
#endif
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
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
    void clear(JNIEnv * env, void* container) override;
    void append(JNIEnv * env, void* container, jobject value) override;
    void prepend(JNIEnv * env, void* container, jobject value) override;
    void removeFirst(JNIEnv * env, void* container) override;
    void removeLast(JNIEnv * env, void* container) override;
    jint removeAll(JNIEnv * env, void* container, jobject value) override;
    jobject takeFirst(JNIEnv * env, void* container) override;
    jobject takeLast(JNIEnv * env, void* container) override;
    jboolean removeOne(JNIEnv * env, void* container, jobject value) override;
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
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
    void appendVector(JNIEnv * env, void* container, jobject list) override;
    void replace(JNIEnv * env, void* container, jint index, jobject value) override;
    jint removeAll(JNIEnv * env, void* container, jobject value) override;
    jobject mid(JNIEnv * env, const void* container, jint index1, jint index2) override;
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
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* moved) override;
#endif
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
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* moved) override;
#endif
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
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* moved) override;
#endif
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
    void* constructContainer(void* placement, const void* copyOf = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* moved) override;
#endif
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
    bool destructContainer(void* container) override;
    void* constructContainer(void* result, const void* container = nullptr) override;
    size_t sizeOf() override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* result, void* container) override;
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
    void assign(void* container, const void* other) override;
    int registerContainer(const QByteArray& typeName) override;
    bool isConstant() override {return false;}
    jobject first(JNIEnv * env, const void* container) override;
    void setFirst(JNIEnv *env, void* container, jobject first) override;
    jobject second(JNIEnv * env, const void* container) override;
    void setSecond(JNIEnv *env, void* container, jobject second) override;
};

class AutoSequentialConstIteratorAccess : public AbstractSequentialConstIteratorAccess{
public:
    typedef std::function<void(void*)> IncrementFn;
    typedef std::function<void(void*)> DecrementFn;
    typedef std::function<const void*(const void*)> ValueFn;
    typedef std::function<bool(const void*,const void*)> LessThanFn;
    typedef std::function<bool(const void*,const void*)> EqualsFn;
private:
    InternalToExternalConverter m_internalToExternalConverter;
    IncrementFn m_increment;
    DecrementFn m_decrement;
    ValueFn m_value;
    LessThanFn m_lessThan;
    EqualsFn m_equals;
public:
    ~AutoSequentialConstIteratorAccess() override;
    AutoSequentialConstIteratorAccess(
            const InternalToExternalConverter& internalToExternalConverter,
            IncrementFn increment,
            DecrementFn decrement,
            ValueFn value,
            LessThanFn lessThan,
            EqualsFn equals
        );
    void dispose() override;
    AbstractSequentialConstIteratorAccess* clone() override;
    jobject value(JNIEnv * env, const void* iterator) override;
    void increment(JNIEnv * env, void* iterator) override;
    void decrement(JNIEnv * env, void* iterator) override;
    jboolean lessThan(JNIEnv * env, const void* iterator, const void* other) override;
    bool canLess() override;
    jboolean equals(JNIEnv * env, const void* iterator, const void* other) override;
private:
    Q_DISABLE_COPY_MOVE(AutoSequentialConstIteratorAccess)
    friend class AutoAssociativeConstIteratorAccess;
    friend class AutoSequentialIteratorAccess;
    friend class AutoAssociativeIteratorAccess;
};

class AutoSequentialIteratorAccess : public virtual AbstractSequentialIteratorAccess, public AutoSequentialConstIteratorAccess{
public:
    typedef std::function<void*(void*)> SetValueFn;
    ~AutoSequentialIteratorAccess() override;
    AutoSequentialIteratorAccess(
            const InternalToExternalConverter& internalToExternalConverter,
            IncrementFn increment,
            DecrementFn decrement,
            ValueFn value,
            LessThanFn lessThan,
            EqualsFn equals,
            const ExternalToInternalConverter& externalToInternalConverter,
            SetValueFn setValue
        );
    void setValue(JNIEnv * env, void* iterator, jobject newValue) override;
    jobject value(JNIEnv * env, const void* iterator) override;
    void increment(JNIEnv * env, void* iterator) override;
    void decrement(JNIEnv * env, void* iterator) override;
    jboolean lessThan(JNIEnv * env, const void* iterator, const void* other) override;
    bool canLess() override;
    jboolean equals(JNIEnv * env, const void* iterator, const void* other) override;
    AbstractSequentialConstIteratorAccess* clone() override;
private:
    Q_DISABLE_COPY_MOVE(AutoSequentialIteratorAccess)
    ExternalToInternalConverter m_externalToInternalConverter;
    SetValueFn m_setValue;
};

class AutoAssociativeConstIteratorAccess : public virtual AbstractAssociativeConstIteratorAccess, public AutoSequentialConstIteratorAccess{
public:
    typedef std::function<const void*(const void*)> KeyFn;
private:
    InternalToExternalConverter m_keyInternalToExternalConverter;
    KeyFn m_key;
public:
    ~AutoAssociativeConstIteratorAccess() override;
    AutoAssociativeConstIteratorAccess(
            const InternalToExternalConverter& internalToExternalConverter,
            IncrementFn increment,
            DecrementFn decrement,
            ValueFn value,
            LessThanFn lessThan,
            EqualsFn equals,
            const InternalToExternalConverter& keyInternalToExternalConverter,
            KeyFn key
            );
    jobject value(JNIEnv * env, const void* iterator) override;
    void increment(JNIEnv * env, void* iterator) override;
    void decrement(JNIEnv * env, void* iterator) override;
    jboolean lessThan(JNIEnv * env, const void* iterator, const void* other) override;
    bool canLess() override;
    jboolean equals(JNIEnv * env, const void* iterator, const void* other) override;
    AbstractSequentialConstIteratorAccess* clone() override;
    jobject key(JNIEnv * env, const void* iterator) override;
private:
    Q_DISABLE_COPY_MOVE(AutoAssociativeConstIteratorAccess)
    friend class AutoAssociativeIteratorAccess;
};

class AutoAssociativeIteratorAccess : public virtual AbstractAssociativeIteratorAccess, public AutoAssociativeConstIteratorAccess{
    typedef std::function<void*(void*)> SetValueFn;
public:
    ~AutoAssociativeIteratorAccess() override;
    AutoAssociativeIteratorAccess(
            const InternalToExternalConverter& internalToExternalConverter,
            IncrementFn increment,
            DecrementFn decrement,
            ValueFn value,
            LessThanFn lessThan,
            EqualsFn equals,
            const InternalToExternalConverter& keyInternalToExternalConverter,
            KeyFn key,
            const ExternalToInternalConverter& valueExternalToInternalConverter,
            SetValueFn setValue
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
private:
    Q_DISABLE_COPY_MOVE(AutoAssociativeIteratorAccess)
    ExternalToInternalConverter m_valueExternalToInternalConverter;
    SetValueFn m_setValue;
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
    void* constructContainer(void* result, const void* container = nullptr) override;
    bool destructContainer(void* container) override;
    size_t sizeOf() override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* result, void* container) override;
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
    void assign(void* container, const void* other) override;
    int registerContainer(const QByteArray& containerTypeName) override;
    bool isConstant() override {return false;}
    const QMetaType& elementMetaType() override {return m_elementMetaType;}
    jobject createIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr);
    jobject createConstIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr);
    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) override;
    jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) override;
    jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    void appendList(JNIEnv * env, void* container, jobject list) override;
    jobject at(JNIEnv * env, const void* container, jint index) override;
    jobject value(JNIEnv * env, const void* container, jint index) override;
    jobject value(JNIEnv * env, const void* container, jint index, jobject defaultValue) override;
    void swapItemsAt(JNIEnv *, void* container, jint index1, jint index2) override;
    jboolean startsWith(JNIEnv * env, const void* container, jobject value) override;
    jint size(JNIEnv *, const void* container) override;
    void reserve(JNIEnv *, void* container, jint size) override;
    void replace(JNIEnv * env, void* container, jint index, jobject value) override;
    void remove(JNIEnv *, void* container, jint index, jint n) override;
    jint removeAll(JNIEnv * env, void* container, jobject value) override;
    jboolean equal(JNIEnv * env, const void* container, jobject other) override;
    void move(JNIEnv *, void* container, jint index1, jint index2) override;
    jobject mid(JNIEnv * env, const void* container, jint index1, jint index2) override;
    jint lastIndexOf(JNIEnv * env, const void* container, jobject value, jint index) override;
    jint indexOf(JNIEnv * env, const void* container, jobject value, jint index) override;
    jboolean endsWith(JNIEnv * env, const void* container, jobject value) override;
    jint count(JNIEnv * env, const void* container, jobject value) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    void clear(JNIEnv *, void* container) override;
    void insert(JNIEnv * env, void* container, jint index, jint n, jobject value) override;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    jint capacity(JNIEnv *, const void* container) override;
    void fill(JNIEnv * env, void* container, jobject value, jint size) override;
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

typedef bool (*IsBiContainerFunction)(JNIEnv *, jobject, const QMetaType&, const QMetaType&, void*& ptr);

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
    void assign(void* container, const void* other) override;
    int registerContainer(const QByteArray& containerTypeName) override;
    void dispose() override;
    AbstractMapAccess* clone() override;
    void analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) override;
    bool isConstant() override;
    const QMetaType& keyMetaType() override;
    const QMetaType& valueMetaType() override;
    void clear(JNIEnv *,void*) override;
    jboolean contains(JNIEnv *,const void*,jobject) override;
    jint count(JNIEnv *,const void*,jobject) override;
    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) override;
    jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) override;
    jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jobject find(JNIEnv * env, QtJambiNativeID ownerId, void* container, jobject key) override;
    jobject constFind(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override;
    jobject first(JNIEnv *,const void*) override;
    jobject firstKey(JNIEnv *,const void*) override;
    void insert(JNIEnv *,void*,jobject,jobject) override;
    jobject key(JNIEnv *,const void*,jobject,jobject) override;
    jobject keys(JNIEnv *,const void*) override;
    jobject keys(JNIEnv *,const void*,jobject) override;
    jobject last(JNIEnv *,const void*) override;
    jobject lastKey(JNIEnv *,const void*) override;
    jobject constLowerBound(JNIEnv *,QtJambiNativeID,const void*,jobject) override;
    jboolean equal(JNIEnv *,const void*,jobject) override;
    jint remove(JNIEnv *,void*,jobject) override;
    jint size(JNIEnv *,const void*) override;
    jobject take(JNIEnv *,void*,jobject) override;
    jobject constUpperBound(JNIEnv *,QtJambiNativeID,const void*,jobject) override;
    jobject value(JNIEnv *,const void*,jobject,jobject) override;
    jobject values(JNIEnv *,const void*) override;
    bool keyLessThan(JNIEnv *,jobject,jobject) override;
    bool destructContainer(void* container) override;
    size_t sizeOf() override;
    void* constructContainer(void* result, const void* container = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    static void defaultCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr);
    static void copyCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, const void *other);
    static void moveCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, void *other);
    static void dtor(const QtPrivate::QMetaTypeInterface *iface, void *ptr);
    static bool equalsFn(const QtPrivate::QMetaTypeInterface *iface, const void *ptr1, const void *ptr2);
    static void debugStreamFn(const QtPrivate::QMetaTypeInterface *iface, QDebug &s, const void *ptr);
    static void dataStreamOutFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, const void *ptr);
    static void dataStreamInFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, void *ptr);
    void debugStream(QDebug &s, const void *ptr);
    virtual void dataStreamOut(QDataStream &s, const void *ptr);
    void dataStreamIn(QDataStream &s, void *ptr);
    void* constructContainer(void* result, void* container) override;
#endif
private:
    virtual IsBiContainerFunction getIsBiContainerFunction();
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
#endif
    jobject createIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr);
    jobject createConstIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr);
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
    void assign(void* container, const void* other) override;
    int registerContainer(const QByteArray& containerTypeName) override;
    void dispose() override;
    bool isConstant() override {return false;}
    void analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) override;
    const QMetaType& keyMetaType() override;
    const QMetaType& valueMetaType() override;
    void clear(JNIEnv *,void*) override;
    jboolean contains(JNIEnv *,const void*,jobject) override;
    jint count(JNIEnv *,const void*,jobject) override;
    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) override;
    jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) override;
    jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jobject find(JNIEnv * env, QtJambiNativeID ownerId, void* container, jobject key) override;
    jobject constFind(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override;
    jobject first(JNIEnv *,const void*) override;
    jobject firstKey(JNIEnv *,const void*) override;
    void insert(JNIEnv *,void*,jobject,jobject) override;
    jobject key(JNIEnv *,const void*,jobject,jobject) override;
    jobject keys(JNIEnv *,const void*) override;
    jobject keys(JNIEnv *,const void*,jobject) override;
    jobject last(JNIEnv *,const void*) override;
    jobject lastKey(JNIEnv *,const void*) override;
    jobject constLowerBound(JNIEnv *,QtJambiNativeID,const void*,jobject) override;
    jboolean equal(JNIEnv *,const void*,jobject) override;
    jint remove(JNIEnv *,void*,jobject) override;
    jint size(JNIEnv *,const void*) override;
    jobject take(JNIEnv *,void*,jobject) override;
    jobject constUpperBound(JNIEnv *,QtJambiNativeID,const void*,jobject) override;
    jobject value(JNIEnv *,const void*,jobject,jobject) override;
    jobject values(JNIEnv *,const void*) override;
    bool keyLessThan(JNIEnv *,jobject,jobject) override;
    AbstractMultiMapAccess* clone() override;
    jobject uniqueKeys(JNIEnv *,const void*) override;
    void unite(JNIEnv *,void*,jobject) override;
    jobject values(JNIEnv *,const void*,jobject) override;
    jboolean contains(JNIEnv *,const void*,jobject,jobject) override;
    jint count(JNIEnv *,const void*,jobject,jobject) override;
    jobject find(JNIEnv * env, QtJambiNativeID ownerId, void* container, jobject key, jobject value) override;
    jobject constFind(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key, jobject value) override;
    jint remove(JNIEnv *,void*,jobject,jobject) override;
    void replace(JNIEnv *,void*,jobject,jobject) override;
    size_t sizeOf() override;
    void* constructContainer(void* result, const void* container = nullptr) override;
    bool destructContainer(void* container) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* result, void* container) override;
#endif
private:
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
    void assign(void* container, const void* other) override;
    int registerContainer(const QByteArray& containerTypeName) override;
    void dispose() override;
    AbstractHashAccess* clone() override;
    void analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) override;
    const QMetaType& keyMetaType() override;
    const QMetaType& valueMetaType() override;
    jint capacity(JNIEnv *,const void*) override;
    void clear(JNIEnv *,void*) override;
    jboolean contains(JNIEnv *,const void*,jobject) override;
    jint count(JNIEnv *,const void*,jobject) override;
    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) override;
    jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) override;
    jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jobject find(JNIEnv * env, QtJambiNativeID ownerId, void* container, jobject key) override;
    jobject constFind(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override;
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
    void* constructContainer(void* result, const void* container = nullptr) override;
    bool destructContainer(void* container) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* result, void* container) override;
#endif
    size_t sizeOf() override;
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
    void assign(void* container, const void* other) override;
    int registerContainer(const QByteArray& containerTypeName) override;
    void dispose() override;
    AbstractMultiHashAccess* clone() override;
    jobject uniqueKeys(JNIEnv *,const void*) override;
    void unite(JNIEnv *,void*,jobject) override;
    bool isConstant() override;
    void analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) override;
    const QMetaType& keyMetaType() override;
    const QMetaType& valueMetaType() override;
    jint capacity(JNIEnv *,const void*) override;
    void clear(JNIEnv *,void*) override;
    jboolean contains(JNIEnv *,const void*,jobject) override;
    jint count(JNIEnv *,const void*,jobject) override;
    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) override;
    jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) override;
    jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jobject find(JNIEnv * env, QtJambiNativeID ownerId, void* container, jobject key) override;
    jobject constFind(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override;
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
    jobject find(JNIEnv * env, QtJambiNativeID ownerId, void* container, jobject key, jobject value) override;
    jobject constFind(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key, jobject value) override;
    jint remove(JNIEnv *,void*,jobject,jobject) override;
    void replace(JNIEnv *,void*,jobject,jobject) override;
    bool destructContainer(void* container) override;
    void* constructContainer(void* result, const void* container = nullptr) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* result, void* container) override;
#endif
    size_t sizeOf() override;
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
    void assign(void* container, const void* other) override;
    int registerContainer(const QByteArray& containerTypeName) override;
    void dispose() override;
    AutoSetAccess* clone() override;
    void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) override;
    bool isConstant() override {return false;}
    const QMetaType& elementMetaType() override;
    jint capacity(JNIEnv * env, const void* container) override;
    void clear(JNIEnv * env, void* container) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
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
    void* constructContainer(void* result, const void* container = nullptr) override;
    bool destructContainer(void* container) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* result, void* container) override;
#endif
    size_t sizeOf() override;
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
    jobject createConstIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr);
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
    size_t sizeOf() override;
    void* constructContainer(void* placement, const void* container = nullptr) override;
    void assign(void* container, const void* other) override;
    bool destructContainer(void* container) override;
    int registerContainer(const QByteArray& containerTypeName) override;
    void dispose() override;
    AutoVectorAccess* clone() override;
    void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) override;
    bool isConstant() override {return false;}
    const QMetaType& elementMetaType() override;
    void appendVector(JNIEnv * env, void* container, jobject list) override;
    jobject at(JNIEnv * env, const void* container, jint index) override;
    jobject value(JNIEnv * env, const void* container, jint index) override;
    jobject value(JNIEnv * env, const void* container, jint index, jobject defaultValue) override;
    void swapItemsAt(JNIEnv * env, void* container, jint index1, jint index2) override;
    jboolean startsWith(JNIEnv * env, const void* container, jobject value) override;
    jint size(JNIEnv * env, const void* container) override;
    void reserve(JNIEnv * env, void* container, jint size) override;
    void replace(JNIEnv * env, void* container, jint index, jobject value) override;
    jint removeAll(JNIEnv * env, void* container, jobject value) override;
    jboolean equal(JNIEnv * env, const void* container, jobject other) override;
    void move(JNIEnv * env, void* container, jint index1, jint index2) override;
    jobject mid(JNIEnv * env, const void* container, jint index1, jint index2) override;
    jint lastIndexOf(JNIEnv * env, const void* container, jobject value, jint index) override;
    jint indexOf(JNIEnv * env, const void* container, jobject value, jint index) override;
    jboolean endsWith(JNIEnv * env, const void* container, jobject value) override;
    jint count(JNIEnv * env, const void* container, jobject value) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    void clear(JNIEnv * env, void* container) override;
    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) override;
    jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) override;
    jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
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
    size_t sizeOf() override;
    void* constructContainer(void* placement, const void* container) override;
    void assign(void* container, const void* other) override;
    bool destructContainer(void* container) override;
    int registerContainer(const QByteArray& containerTypeName) override;
    bool isConstant() override {return false;}
    void dispose() override;
    void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) override;
    const QMetaType& elementMetaType() override;
    void append(JNIEnv * env, void* container, jobject value) override;
    jobject first(JNIEnv * env, const void* container) override;
    jobject last(JNIEnv * env, const void* container) override;
    void clear(JNIEnv * env, void* container) override;
    jboolean contains(JNIEnv * env, const void* container, jobject value) override;
    jint count(JNIEnv * env, const void* container, jobject value) override;
    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) override;
    jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) override;
    jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
    jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override;
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
    jobject createIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr);
    jobject createConstIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr);
    void detach(QLinkedListData*& d);
    void detach_helper(QLinkedListData*& d);
    QLinkedList<char>::iterator detach_helper2(QLinkedListData*& d, QLinkedList<char>::iterator iter);
    void freeData(QLinkedListData*& v);
    void erase(QLinkedListData*& d, Node *i);
};

#endif

#endif // CONTAINERACCESS_P_H
