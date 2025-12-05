/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
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
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "pch_p.h"

QT_WARNING_DISABLE_GCC("-Winaccessible-base")
QT_WARNING_DISABLE_CLANG("-Winaccessible-base")

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QT_WARNING_DISABLE_GCC("-Wstrict-aliasing")
QT_WARNING_DISABLE_CLANG("-Wstrict-aliasing")
#endif

QSharedPointer<class AutoHashAccess> getHashAccess(const QtPrivate::QMetaTypeInterface *iface);

AutoSetAccess::AutoSetAccess(const AutoSetAccess& other)
    :AbstractSetAccess(), AbstractNestedSequentialAccess(),
      m_hashAccess(other.m_hashAccess)
{}

AutoSetAccess::~AutoSetAccess(){}

AutoSetAccess::AutoSetAccess(
                const QMetaType& elementMetaType,
                const QtJambiUtils::QHashFunction& hashFunction,
                const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
                const QtJambiUtils::ExternalToInternalConverter& externalToInternalConverter,
                const QSharedPointer<AbstractContainerAccess>& elementNestedContainerAccess,
                PtrOwnerFunction elementOwnerFunction,
                AbstractContainerAccess::DataType elementDataType
        )
    : AbstractSetAccess(), AbstractNestedSequentialAccess(),
      m_hashAccess(elementMetaType,
                   hashFunction,
                   internalToExternalConverter,
                   externalToInternalConverter,
                   elementNestedContainerAccess,
                   elementOwnerFunction,
                   elementDataType,
                   QMetaType(QMetaType::Void),
                   {},
                   {},
                   {},
                   {},
                   nullptr,
                   AbstractContainerAccess::Value)
{
}

void* AutoSetAccess::constructContainer(JNIEnv*, void* result, const ConstContainerAndAccessInfo& container) {
    return m_hashAccess.constructContainer(result, container.container);
}

void* AutoSetAccess::constructContainer(void* result, const void* container){
    return m_hashAccess.constructContainer(result, container);
}

void* AutoSetAccess::constructContainer(void* result){
    return m_hashAccess.constructContainer(result);
}

void* AutoSetAccess::constructContainer(JNIEnv*, void* result, const ContainerAndAccessInfo& container) {
    return m_hashAccess.constructContainer(result, container.container);
}

void* AutoSetAccess::constructContainer(void* result, void* container){
    return m_hashAccess.constructContainer(result, container);
}

size_t AutoSetAccess::sizeOf(){
    return m_hashAccess.sizeOf();
}

void AutoSetAccess::assign(void* container, const void* other){
    m_hashAccess.assign(container, other);
}

void AutoSetAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other){
    m_hashAccess.assign(env, container, other);
}

bool AutoSetAccess::destructContainer(void* container){
    return m_hashAccess.destructContainer(container);
}

int AutoSetAccess::registerContainer(const QByteArray& typeName)
{
    int newMetaType = QMetaType::fromName(typeName).id();
    if(newMetaType==QMetaType::UnknownType){
        QSharedPointer<AutoHashAccess> access(reinterpret_cast<AutoHashAccess*>(m_hashAccess.clone()), &containerDisposer);
        auto iface = m_hashAccess.m_keyMetaType.iface();
        newMetaType = registerContainerMetaType(typeName,
                                       AutoHashAccess::defaultCtr,
                                       AutoHashAccess::copyCtr,
                                       AutoHashAccess::moveCtr,
                                       AutoHashAccess::dtor,
                                       (iface->equals
                                            || (iface->flags & QMetaType::IsPointer)
                                            || (iface->flags & QMetaType::IsEnumeration)) ? AutoHashAccess::equalsFn : nullptr,
                                       nullptr,
                                       (iface->debugStream
                                            || (iface->flags & QMetaType::IsPointer)
                                            || (iface->flags & QMetaType::IsEnumeration)) ? AutoHashAccess::debugStreamSetFn : nullptr,
                                       (iface->dataStreamOut || (iface->flags & QMetaType::IsEnumeration)) ? AutoHashAccess::dataStreamOutSetFn : nullptr,
                                       (iface->dataStreamIn || (iface->flags & QMetaType::IsEnumeration)) ? AutoHashAccess::dataStreamInSetFn : nullptr,
                                       nullptr,
                                       uint(sizeOf()),
                                       ushort(alignof(QSet<char>)),
                                       QMetaType::UnknownType,
                                       QMetaType::NeedsConstruction
                                                   | QMetaType::NeedsDestruction
                                                   | QMetaType::RelocatableType,
                                       nullptr,
                                       nullptr,
                                       access);
        if(m_hashAccess.m_keyHashFunction){
            insertHashFunctionByMetaType(newMetaType,
                                            [access]
                                            (const void* ptr, size_t seed)->size_t{
                                                if(ptr){
                                                    size_t hashCode = seed;
                                                    QHashData *const* map = reinterpret_cast<QHashData *const*>(ptr);
                                                    QHashData* d = *map;
                                                    hashCode = seed;
                                                    QHashData::iterator n = d->begin(*access);
                                                    QHashData::iterator e = d->end(*access);
                                                    while (n!=e) {
                                                        const void* element = n.key();
                                                        hashCode += access->m_keyHashFunction(element, 0);
                                                        ++n;
                                                    }
                                                    return hashCode;
                                                }else{
                                                    return 0;
                                                }
                                            });
        }
        const QMetaType to = QMetaType::fromType<QSequentialIterable>();
        QMetaType::registerMutableViewFunction([newMetaType](void *src, void *target)->bool{
            new(target) QIterable<QMetaSequence>(QMetaSequence(createMetaSequenceInterface(newMetaType)), reinterpret_cast<void **>(src));
            return true;
        }, QMetaType(newMetaType), to);
        QMetaType::registerConverterFunction([newMetaType](const void *src, void *target)->bool{
            new(target) QIterable<QMetaSequence>(QMetaSequence(createMetaSequenceInterface(newMetaType)), reinterpret_cast<void const*const*>(src));
            return true;
        }, QMetaType(newMetaType), to);
    }else{
        registerContainerAccess(newMetaType, this);
    }
    return newMetaType;
}

QReadWriteLock* containerAccessLock();
QMap<int, QtMetaContainerPrivate::QMetaSequenceInterface>& metaSequenceHash();

QtMetaContainerPrivate::QMetaSequenceInterface* AutoSetAccess::createMetaSequenceInterface(int newMetaType){
    {
        QReadLocker locker(containerAccessLock());
        Q_UNUSED(locker)
        if(metaSequenceHash().contains(newMetaType))
           return &metaSequenceHash()[newMetaType];
    }
    using namespace QtMetaContainerPrivate;
    QMetaSequenceInterface* metaSequenceInterface;
    {
        QWriteLocker locker(containerAccessLock());
        metaSequenceInterface = &metaSequenceHash()[newMetaType];
    }
    QSharedPointer<class AutoHashAccess> access = getHashAccess(QMetaType(newMetaType).iface());
    metaSequenceInterface->valueMetaType = access->m_keyMetaType.iface();
    metaSequenceInterface->iteratorCapabilities = InputCapability | ForwardCapability;
    metaSequenceInterface->addRemoveCapabilities = CanAddAtBegin|CanRemoveAtBegin|CanAddAtEnd|CanRemoveAtEnd;
    metaSequenceInterface->sizeFn = [](const void *c) -> qsizetype {
        if(!c)
            return 0;
        QHashData *const* map = reinterpret_cast<QHashData *const*>(c);
        QHashData* d = *map;
        return !d ? 0 : d->size;
    };

    metaSequenceInterface->clearFn = qtjambi_function_pointer<16,void(void *)>(
                [newMetaType](void *c) {
                    if(!c)
                        return;
                    QSharedPointer<class AutoHashAccess> access = getHashAccess(QMetaType(newMetaType).iface());
                    if(!access)
                        return;
                    access->clear(c);
                }, newMetaType);
    metaSequenceInterface->createIteratorFn = qtjambi_function_pointer<16,void*(void *,QMetaContainerInterface::Position)>(
                [newMetaType](void *c, QMetaContainerInterface::Position p) -> void* {
                        if(!c)
                            return nullptr;
                        QSharedPointer<class AutoHashAccess> access = getHashAccess(QMetaType(newMetaType).iface());
                        if(!access)
                            return nullptr;
                        QHashData ** map = reinterpret_cast<QHashData **>(c);
                        access->detach(map);
                        QHashData* d = *map;
                        switch (p) {
                        case QMetaContainerInterface::Unspecified:
                            return new iterator(d->end(*access));
                        case QMetaContainerInterface::AtBegin:
                            return new iterator(d->begin(*access));
                            break;
                        case QMetaContainerInterface::AtEnd:
                            return new iterator(d->end(*access));
                            break;
                        }
                        return new iterator(*access);
                    }
                ), access->m_keyMetaType.id();
    metaSequenceInterface->destroyIteratorFn = [](const void *i) {
                    delete static_cast<const iterator *>(i);
                };
    metaSequenceInterface->compareIteratorFn = [](const void *i, const void *j) {
                    return *static_cast<const iterator *>(i) == *static_cast<const iterator *>(j);
                };
    metaSequenceInterface->copyIteratorFn = [](void *i, const void *j) {
                    *static_cast<iterator *>(i) = *static_cast<const iterator *>(j);
                };
    metaSequenceInterface->advanceIteratorFn = [](void *i, qsizetype step) {
                    iterator& it = *reinterpret_cast<iterator*>(i);
                    if(step>0){
                        for(;step>0; --step)
                            ++it;
                    }
                };
    metaSequenceInterface->diffIteratorFn = [](const void *i, const void *j) -> qsizetype {
                    iterator it = *reinterpret_cast<const iterator*>(i);
                    iterator jt = *reinterpret_cast<const iterator*>(j);
                    iterator end(*it.i.access);
                    if(it==jt || it==end)
                        return 0;
                    int counter = 0;
                    while(it!=jt){
                        ++counter;
                        ++it;
                        if(it==end && jt!=end)
                            return 0;
                    }
                    return counter;
                };
    metaSequenceInterface->createConstIteratorFn = qtjambi_function_pointer<16,void*(const void *,QMetaContainerInterface::Position)>(
        [newMetaType](const void *c, QMetaContainerInterface::Position p) -> void* {
            if(!c)
                return nullptr;
            QSharedPointer<class AutoHashAccess> access = getHashAccess(QMetaType(newMetaType).iface());
            if(!access)
                return nullptr;
            QHashData *const* map = reinterpret_cast<QHashData *const*>(c);
            QHashData* d = *map;
            if(d){
                switch (p) {
                case QMetaContainerInterface::Unspecified:
                    return new iterator(d->end(*access));
                case QMetaContainerInterface::AtBegin:
                    return new iterator(d->begin(*access));
                    break;
                case QMetaContainerInterface::AtEnd:
                    return new iterator(d->end(*access));
                    break;
                }
            }
            return new iterator(*access);
        }
        ), access->m_keyMetaType.id();
    metaSequenceInterface->destroyConstIteratorFn = metaSequenceInterface->destroyIteratorFn;
    metaSequenceInterface->compareConstIteratorFn = metaSequenceInterface->compareIteratorFn;
    metaSequenceInterface->copyConstIteratorFn = metaSequenceInterface->copyIteratorFn;
    metaSequenceInterface->advanceConstIteratorFn = metaSequenceInterface->advanceIteratorFn;
    metaSequenceInterface->diffConstIteratorFn = metaSequenceInterface->diffIteratorFn;
    metaSequenceInterface->valueAtIndexFn = nullptr;
    metaSequenceInterface->setValueAtIndexFn = nullptr;
    metaSequenceInterface->addValueFn = qtjambi_function_pointer<16,void(void *, const void *, QMetaSequenceInterface::Position)>(
                [newMetaType](void *container, const void *e, QMetaSequenceInterface::Position) {
                    if(!container)
                        return;
                    QSharedPointer<class AutoHashAccess> access = getHashAccess(QMetaType(newMetaType).iface());
                    if(!access)
                        return;
                    access->emplace(container, e, nullptr);
                }, newMetaType);
    metaSequenceInterface->removeValueFn = qtjambi_function_pointer<16,void(void *, QMetaSequenceInterface::Position)>([newMetaType](void *c, QMetaSequenceInterface::Position position) {
        if(!c)
            return;
        QSharedPointer<class AutoHashAccess> access = getHashAccess(QMetaType(newMetaType).iface());
        if(!access)
            return;
        QHashData ** map = reinterpret_cast<QHashData **>(c);
        QHashData*& d = *map;
        QHashData::iterator it = d->begin(*access);
        if (!it.isUnused()){
            if(position==QMetaSequenceInterface::AtEnd){
                QHashData::iterator end = d->end(*access);
                QHashData::iterator copy(it);
                while(it!=end){
                    copy = it;
                    ++it;
                }
                it = copy;
            }
            access->detach(map);
            it = d->detachedIterator(*access, it);
            d->erase(*access, it);
        }
    }, newMetaType);
    metaSequenceInterface->valueAtIteratorFn = [](const void *i, void *r) {
        const iterator* it = reinterpret_cast<const iterator*>(i);
        it->i.access->m_keyMetaType.destruct(r);
        it->i.access->m_keyMetaType.construct(r, it->key());
    };
    metaSequenceInterface->setValueAtIteratorFn = nullptr;
    metaSequenceInterface->insertValueAtIteratorFn = nullptr;
    metaSequenceInterface->valueAtConstIteratorFn = metaSequenceInterface->valueAtIteratorFn;
    metaSequenceInterface->eraseValueAtIteratorFn = nullptr;
    metaSequenceInterface->eraseRangeAtIteratorFn = nullptr;
    return metaSequenceInterface;
}

void AutoSetAccess::dispose(){ delete this; }

AutoSetAccess* AutoSetAccess::clone(){ return new AutoSetAccess(*this); }

bool AutoSetAccess::isDetached(const void* container){
    return m_hashAccess.isDetached(container);
}

void AutoSetAccess::detach(const ContainerInfo& container){
    m_hashAccess.detach(container);
}

bool AutoSetAccess::isSharedWith(const void* container, const void* container2){
    return m_hashAccess.isSharedWith(container, container2);
}

void AutoSetAccess::swap(JNIEnv *env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    m_hashAccess.swap(env, container, container2);
}

const QMetaType& AutoSetAccess::elementMetaType(){ return m_hashAccess.keyMetaType(); }
AbstractContainerAccess::DataType AutoSetAccess::elementType(){ return m_hashAccess.keyType(); }

AbstractContainerAccess* AutoSetAccess::elementNestedContainerAccess() {
    return m_hashAccess.keyNestedContainerAccess();
}
const QSharedPointer<AbstractContainerAccess>& AutoSetAccess::sharedElementNestedContainerAccess(){
    return m_hashAccess.sharedKeyNestedContainerAccess();
}
bool AutoSetAccess::hasNestedContainerAccess(){
    return m_hashAccess.hasKeyNestedContainerAccess();
}
bool AutoSetAccess::hasNestedPointers(){
    return m_hashAccess.hasKeyNestedPointers();
}

const QObject* AutoSetAccess::getOwner(const void* container){
    return m_hashAccess.getOwner(container);
}

bool AutoSetAccess::hasOwnerFunction(){
    return m_hashAccess.hasOwnerFunction();
}

jobject AutoSetAccess::constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container)
{
    return m_hashAccess.constBegin(env, container);
}

jobject AutoSetAccess::constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container)
{
    return m_hashAccess.constEnd(env, container);
}

jint AutoSetAccess::capacity(JNIEnv * env, const void* container)
{
    return m_hashAccess.capacity(env, container);
}

void AutoSetAccess::clear(JNIEnv * env, const ContainerInfo& container)
{
    return m_hashAccess.clear(env, container);
}

jboolean AutoSetAccess::contains(JNIEnv * env, const void* container, jobject value)
{
    return m_hashAccess.contains(env, container, value);
}

void AutoSetAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject value)
{
    m_hashAccess.insert(env, container, value, nullptr);
}

void AutoSetAccess::intersect(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other)
{
    void* ptr{nullptr};
    bool deleteSet = false;
    if (ContainerAPI::getAsQSet(env, other.object, elementMetaType(), other.container, other.access)) {
        ptr = other.container;
    }else{
        deleteSet = true;
        auto hashAccess = m_hashAccess.clone();
        ptr = hashAccess->createContainer();
        jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, other.object);
        while(QtJambiAPI::hasJavaIteratorNext(env, iterator)){
            hashAccess->insert(env, {nullptr, ptr}, QtJambiAPI::nextOfJavaIterator(env, iterator), nullptr);
        }
        hashAccess->dispose();
    }
    QHashData ** map = reinterpret_cast<QHashData **>(container.container);
    m_hashAccess.detach(map);
    QHashData*& d = *map;
    QHashData ** map2 = reinterpret_cast<QHashData **>(ptr);
    QHashData*& d2 = *map2;
    void* copy1;
    void* copy2;
    if ((d ? d->size : 0) <= (d2 ? d2->size : 0)) {
        copy1 = m_hashAccess.createContainer(reinterpret_cast<const void *>(container.container));
        copy2 = m_hashAccess.createContainer(reinterpret_cast<const void *>(ptr));
        m_hashAccess.detach(reinterpret_cast<QHashData **>(copy1));
        m_hashAccess.detach(reinterpret_cast<QHashData **>(copy2));
    }else{
        copy1 = m_hashAccess.createContainer(reinterpret_cast<const void *>(ptr));
        copy2 = m_hashAccess.createContainer(reinterpret_cast<const void *>(container.container));
        m_hashAccess.detach(reinterpret_cast<QHashData **>(copy1));
        m_hashAccess.detach(reinterpret_cast<QHashData **>(copy2));
        m_hashAccess.assign(container.container, copy1);
    }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    Node*& e = reinterpret_cast<Node*&>(d);
    Node*& e1 = *reinterpret_cast<Node**>(copy1);
    Node*& e2 = *reinterpret_cast<Node**>(copy2);
    Node *n1 = (*reinterpret_cast<QHashData **>(copy1))->firstNode();
    bool hasShrunk = false;
    while (n1 != e1) {
        void* key = reinterpret_cast<char*>(n1) + m_hashAccess.m_offset1;
        Node** found2 = m_hashAccess.findNode(reinterpret_cast<QHashData **>(copy2), key);
        if (*found2 == e2){
            Node **found = m_hashAccess.findNode(map, key);
            if(*found != e){
                bool deleteNext = true;
                do {
                    Node *next = (*found)->next;
                    deleteNext = (next != e && isEquals(m_hashAccess.m_keyMetaType, key, reinterpret_cast<char*>(next)+m_hashAccess.m_offset1));
                    m_hashAccess.deleteNode(d, *found);
                    *found = next;
                    --d->size;
                } while (deleteNext);
                hasShrunk = true;
            }
        }
        n1 = QHashData::nextNode(n1);
    }
    if(hasShrunk)
        d->hasShrunk();
#else
    QHashData* copy1D = *reinterpret_cast<QHashData **>(copy1);
    QHashData* copy2D = *reinterpret_cast<QHashData **>(copy2);
    QHashData::iterator i = copy1D ? copy1D->begin(m_hashAccess) : QHashData::iterator(m_hashAccess);
    QHashData::iterator e = copy1D ? copy1D->end(m_hashAccess) : QHashData::iterator(m_hashAccess);

    while (i != e) {
        auto it = copy2D->find(m_hashAccess, i.key());
        if (it.isUnused()){
            auto rm = d->find(m_hashAccess, i.key());
            if(!rm.isUnused())
                d->erase(m_hashAccess, rm);
        }
        ++i;
    }
#endif
    m_hashAccess.deleteContainer(copy1);
    m_hashAccess.deleteContainer(copy2);
    if(deleteSet){
        m_hashAccess.deleteContainer(ptr);
    }
}

jboolean AutoSetAccess::intersects(JNIEnv * env, const void* container, jobject other)
{
    void* ptr{nullptr};
    bool deleteSet = false;
    if (!ContainerAPI::getAsQSet(env, other, elementMetaType(), ptr)) {
        deleteSet = true;
        auto hashAccess = m_hashAccess.clone();
        ptr = hashAccess->createContainer();
        jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, other);
        while(QtJambiAPI::hasJavaIteratorNext(env, iterator)){
            hashAccess->insert(env, {nullptr, ptr}, QtJambiAPI::nextOfJavaIterator(env, iterator), nullptr);
        }
        hashAccess->dispose();
    }
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    QHashData* d = *map;
    QHashData *const* map2 = reinterpret_cast<QHashData *const*>(ptr);
    QHashData* d2 = *map2;

    const bool otherIsBigger = d2->size > d->size;
    QHashData*smallestSet = otherIsBigger ? d : d2;
    QHashData*biggestSet = otherIsBigger ? d2 : d;

    bool result = false;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    const bool equalSeeds = d->seed == d2->seed;
    Node* eb = reinterpret_cast<Node*>(biggestSet);
    Node* e = reinterpret_cast<Node*>(smallestSet);
    Node* n = smallestSet->firstNode();

    if (Q_LIKELY(equalSeeds)) {
        // If seeds are equal we take the fast path so no hash is recalculated.
        while (n != e) {
            Node** n2 = m_hashAccess.findNode(&biggestSet, reinterpret_cast<char*>(n) + m_hashAccess.m_offset1, n->h);
            if (*n2 != eb){
                result = true;
                break;
            }
            n = QHashData::nextNode(n);
        }
    } else {
        while (n != e) {
            Node** n2 = m_hashAccess.findNode(&biggestSet, reinterpret_cast<char*>(n) + m_hashAccess.m_offset1);
            if (*n2 != eb){
                result = true;
                break;
            }
            n = QHashData::nextNode(n);
        }
    }
#else
    QHashData::iterator i = smallestSet ? smallestSet->begin(m_hashAccess) : QHashData::iterator(m_hashAccess);
    QHashData::iterator e = smallestSet ? smallestSet->end(m_hashAccess) : QHashData::iterator(m_hashAccess);

    while (i != e) {
        if (biggestSet->findNode(m_hashAccess, i.key())!=nullptr)
            return true;
        ++i;
    }
#endif
    if(deleteSet)
        m_hashAccess.deleteContainer(ptr);
    return result;
}

jboolean AutoSetAccess::equal(JNIEnv * env, const void* container, jobject other)
{
    void* ptr{nullptr};
    bool deleteSet = false;
    if (!ContainerAPI::getAsQSet(env, other, elementMetaType(), ptr)) {
        deleteSet = true;
        auto hashAccess = m_hashAccess.clone();
        ptr = hashAccess->createContainer();
        jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, other);
        while(QtJambiAPI::hasJavaIteratorNext(env, iterator)){
            hashAccess->insert(env, {nullptr, ptr}, QtJambiAPI::nextOfJavaIterator(env, iterator), nullptr);
        }
        hashAccess->dispose();
    }
    bool result = m_hashAccess.equal(container, ptr);
    if(deleteSet)
        m_hashAccess.deleteContainer(ptr);
    return result;
}

jboolean AutoSetAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject value)
{
    return m_hashAccess.remove(env, container, value);
}

void AutoSetAccess::reserve(JNIEnv * env, const ContainerInfo& container, jint newSize)
{
    m_hashAccess.reserve(env, container, newSize);
}

jint AutoSetAccess::size(JNIEnv * env, const void* container)
{
    return m_hashAccess.size(env, container);
}

qsizetype AutoSetAccess::size(const void* container)
{
    return m_hashAccess.size(container);
}

void AutoSetAccess::subtract(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other)
{
    void* ptr{nullptr};
    bool deleteSet = false;
    if (ContainerAPI::getAsQSet(env, other.object, elementMetaType(), other.container, other.access)) {
        ptr = other.container;
    }else{
        deleteSet = true;
        auto hashAccess = m_hashAccess.clone();
        ptr = hashAccess->createContainer();
        jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, other.object);
        while(QtJambiAPI::hasJavaIteratorNext(env, iterator)){
            hashAccess->insert(env, {nullptr, ptr}, QtJambiAPI::nextOfJavaIterator(env, iterator), nullptr);
        }
        hashAccess->dispose();
    }
    QHashData ** map = reinterpret_cast<QHashData **>(container.container);
    m_hashAccess.detach(map);
    QHashData*& d = *map;
    QHashData *const* map2 = reinterpret_cast<QHashData *const*>(ptr);
    QHashData* d2 = *map2;
    if(d==d2){
        if(d)
            m_hashAccess.clear(env, container);
    }else{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        Node*& e = reinterpret_cast<Node*&>(d);
        Node*& e2 = reinterpret_cast<Node*&>(d2);
        Node* n2 = d2->firstNode();
        bool hasShrunk = false;
        while (n2 != e2) {
            void* key = reinterpret_cast<char*>(n2) + m_hashAccess.m_offset1;
            Node **found = m_hashAccess.findNode(map, key);
            if(*found != e){
                bool deleteNext = true;
                do {
                    Node *next = (*found)->next;
                    deleteNext = (next != e && isEquals(m_hashAccess.m_keyMetaType, key, reinterpret_cast<char*>(next)+m_hashAccess.m_offset1));
                    m_hashAccess.deleteNode(d, *found);
                    *found = next;
                    --d->size;
                } while (deleteNext);
                hasShrunk = true;
            }
            n2 = QHashData::nextNode(n2);
        }
        if(hasShrunk)
            d->hasShrunk();
#else
        QHashData::iterator i = d2 ? d2->begin(m_hashAccess) : QHashData::iterator(m_hashAccess);
        QHashData::iterator e = d2 ? d2->end(m_hashAccess) : QHashData::iterator(m_hashAccess);

        while (i != e) {
            auto it = d->find(m_hashAccess, i.key());
            if (!it.isUnused()){
                d->erase(m_hashAccess, it);
            }
            ++i;
        }
#endif
    }

    if(deleteSet)
        m_hashAccess.deleteContainer(ptr);
}

void AutoSetAccess::unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other)
{
    if (ContainerAPI::getAsQSet(env, other.object, elementMetaType(), other.container, other.access)) {
        QHashData ** map = reinterpret_cast<QHashData **>(container.container);
        m_hashAccess.detach(map);
        QHashData*& d = *map;
        QHashData *const* map2 = reinterpret_cast<QHashData *const*>(other.container);
        QHashData* d2 = *map2;
        if(d && d2 && d!=d2){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            Node* e = reinterpret_cast<Node*>(d);
            Node* e2 = reinterpret_cast<Node*>(d2);
            Node* n2 = d2->firstNode();
            while (n2 != e2) {
                void* akey = reinterpret_cast<char*>(n2) + m_hashAccess.m_offset1;
                uint h;
                Node** node = m_hashAccess.findNode(map, akey, &h);
                if (*node == e){
                    if (d->willGrow())
                        node = m_hashAccess.findNode(map, akey, h);
                    m_hashAccess.createNode(d, h, akey, nullptr, node);
                }
                n2 = QHashData::nextNode(n2);
            }
#else
            QHashData::iterator i = d2 ? d2->begin(m_hashAccess) : QHashData::iterator(m_hashAccess);
            QHashData::iterator e = d2 ? d2->end(m_hashAccess) : QHashData::iterator(m_hashAccess);
            while (i != e) {
                m_hashAccess.emplace(map, i.key(), env, nullptr);
                ++i;
            }
#endif
        }
    }else{
        jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, other.object);
        while(QtJambiAPI::hasJavaIteratorNext(env, iterator)){
            m_hashAccess.insert(env, container, QtJambiAPI::nextOfJavaIterator(env, iterator), nullptr);
        }
    }
}

ContainerAndAccessInfo AutoSetAccess::values(JNIEnv * env, const ConstContainerInfo& container)
{
    return m_hashAccess.keys(env, container);
}

std::unique_ptr<AbstractSetAccess::ElementIterator> AutoSetAccess::elementIterator(const void* container) {
    class ElementIterator : public AbstractSetAccess::ElementIterator{
        std::unique_ptr<AbstractHashAccess::KeyValueIterator> iter;
        ElementIterator(const ElementIterator& other)
            :iter(other.iter->clone()) {}
    protected:
        AbstractSequentialAccess* access() override { return nullptr; }
    public:
        const QMetaType& elementMetaType() override { return iter->keyMetaType(); }
        DataType elementType() override { return iter->keyType(); }
        AbstractContainerAccess* elementNestedContainerAccess() override { return iter->keyNestedContainerAccess(); }
        bool hasNestedContainerAccess() override { return iter->hasKeyNestedContainerAccess(); }
        bool hasNestedPointers() override { return iter->hasKeyNestedPointers(); }
        ElementIterator(std::unique_ptr<AbstractHashAccess::KeyValueIterator>&& _iter) : iter(std::move(_iter)) {}
        bool hasNext() override{
            return iter->hasNext();
        }
        bool isConst() override{
            return true;
        }
        const void* constNext() override {
            return iter->constNext().first;
        }
        void* mutableNext() override {
            return nullptr;
        }
        jobject next(JNIEnv * env) override{
            return iter->next(env).first;
        }
        const void* next() override {
            return iter->next().first;
        }
        bool operator==(const AbstractSequentialAccess::ElementIterator& other) const override {
            return iter==reinterpret_cast<const ElementIterator&>(other).iter;
        }
        std::unique_ptr<AbstractSequentialAccess::ElementIterator> clone() const override {
            return std::unique_ptr<AbstractSequentialAccess::ElementIterator>(new ElementIterator(*this));
        }
    };
    return std::unique_ptr<AbstractSetAccess::ElementIterator>(new ElementIterator(m_hashAccess.keyValueIterator(container)));
}

std::unique_ptr<AbstractSetAccess::ElementIterator> AutoSetAccess::elementIterator(void* container) {
    return constElementIterator(container);
}

PointerRCAutoSetAccess::PointerRCAutoSetAccess(PointerRCAutoSetAccess& other)
    : AutoSetAccess(other), ReferenceCountingSetContainer() {}

PointerRCAutoSetAccess* PointerRCAutoSetAccess::clone(){
    return new PointerRCAutoSetAccess(*this);
}

void PointerRCAutoSetAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject set = Java::Runtime::HashSet::newInstance(env);
    auto iterator = elementIterator(container.container);
    while(iterator->hasNext()){
        const void* content = iterator->next();
        jobject obj{nullptr};
        switch(elementType()){
        case PointerToQObject:
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(reinterpret_cast<const QObject*>(content))){
                obj = link->getJavaObjectLocalRef(env);
            }
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(elementMetaType())){
                if(FunctionalResolver resolver = registeredFunctionalResolver(*typeId)){
                    bool success = false;
                    obj = resolver(env, content, &success);
                    break;
                }
            }
            Q_FALLTHROUGH();
        case Pointer:
            for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(content)){
                obj = link->getJavaObjectLocalRef(env);
                break;
            }
            break;
        default:
            break;
        }
        if(obj)
            Java::Runtime::Collection::add(env, set, obj);
    }
    clearRC(env, container.object);
    addAllRC(env, container.object, set);
}

void PointerRCAutoSetAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    AutoSetAccess::swap(env, container, container2);
    if(PointerRCAutoSetAccess* access = dynamic_cast<PointerRCAutoSetAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void PointerRCAutoSetAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    AutoSetAccess::assign(env, container, container2);
    if(PointerRCAutoSetAccess* access = dynamic_cast<PointerRCAutoSetAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void PointerRCAutoSetAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    AutoSetAccess::clear(env, container);
    clearRC(env, container.object);
}

void PointerRCAutoSetAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject value){
    AutoSetAccess::insert(env, container, value);
    addUniqueRC(env, container.object, value);
}

jboolean PointerRCAutoSetAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject value){
    jboolean result = AutoSetAccess::remove(env, container, value);
    if(value)
        removeRC(env, container.object, value);
    return result;
}

void PointerRCAutoSetAccess::intersect(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other){
    AutoSetAccess::intersect(env, container, other);
    updateRC(env, container);
}

void PointerRCAutoSetAccess::subtract(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other){
    AutoSetAccess::subtract(env, container, other);
    updateRC(env, container);
}

void PointerRCAutoSetAccess::unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other){
    AutoSetAccess::unite(env, container, other);
    updateRC(env, container);
}

NestedPointersRCAutoSetAccess::NestedPointersRCAutoSetAccess(NestedPointersRCAutoSetAccess& other)
    : AutoSetAccess(other), ReferenceCountingSetContainer() {}

NestedPointersRCAutoSetAccess* NestedPointersRCAutoSetAccess::clone(){
    return new NestedPointersRCAutoSetAccess(*this);
}

void NestedPointersRCAutoSetAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    if(size(env, container.container)==0){
        clearRC(env, container.object);
    }else{
        JniLocalFrame frame(env, 200);
        jobject set = Java::Runtime::HashSet::newInstance(env);
        auto access = elementNestedContainerAccess();
        auto iterator = elementIterator(container.container);
        while(iterator->hasNext()){
            unfoldAndAddContainer(env, set, iterator->next(), elementType(), elementMetaType(), access);
        }
        if(access)
            access->dispose();
        addAllRC(env, container.object, set);
    }
}

void NestedPointersRCAutoSetAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    AutoSetAccess::swap(env, container, container2);
    if(NestedPointersRCAutoSetAccess* access = dynamic_cast<NestedPointersRCAutoSetAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void NestedPointersRCAutoSetAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    AutoSetAccess::assign(env, container, container2);
    updateRC(env, container);
}

void NestedPointersRCAutoSetAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    AutoSetAccess::clear(env, container);
    clearRC(env, container.object);
}

void NestedPointersRCAutoSetAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject value){
    AutoSetAccess::insert(env, container, value);
    addNestedValueRC(env, container.object, elementType(), hasNestedPointers(), value);
}

jboolean NestedPointersRCAutoSetAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject value){
    jboolean result = AutoSetAccess::remove(env, container, value);
    updateRC(env, container);
    return result;
}

void NestedPointersRCAutoSetAccess::intersect(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other){
    AutoSetAccess::intersect(env, container, other);
    updateRC(env, container);
}

void NestedPointersRCAutoSetAccess::subtract(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other){
    AutoSetAccess::subtract(env, container, other);
    updateRC(env, container);
}

void NestedPointersRCAutoSetAccess::unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other){
    AutoSetAccess::unite(env, container, other);
    updateRC(env, container);
}
