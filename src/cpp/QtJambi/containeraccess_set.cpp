/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "qtjambiapi.h"
#include <QtCore/QReadWriteLock>
#include <QtCore/QMap>
#include <QtCore/QSharedPointer>
#include <QtCore/QSequentialIterable>
#include "containeraccess_p.h"
#include "functionpointer.h"
#include "utils_p.h"
#include "registryutil_p.h"
#include "qtjambilink_p.h"
#include "java_p.h"
#include "coreapi.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QT_WARNING_DISABLE_GCC("-Wstrict-aliasing")
QT_WARNING_DISABLE_CLANG("-Wstrict-aliasing")
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
QSharedPointer<class AutoHashAccess> getHashAccess(const QtPrivate::QMetaTypeInterface *iface);
#endif

AutoSetAccess::AutoSetAccess(const AutoSetAccess& other)
    :AbstractSetAccess(), AbstractNestedSequentialAccess(),
      m_hashAccess(other.m_hashAccess)
{}

AutoSetAccess::~AutoSetAccess(){}

AutoSetAccess::AutoSetAccess(
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
        )
    : AbstractSetAccess(), AbstractNestedSequentialAccess(),
      m_hashAccess(elementMetaType,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                   elementAlign,
#endif
                   hashFunction,
                   internalToExternalConverter,
                   externalToInternalConverter,
                   elementNestedContainerAccess,
                   elementOwnerFunction,
                   elementDataType,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                   QMetaType::Void,
                   0,
#else
                   QMetaType(QMetaType::Void),
#endif
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

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
void* AutoSetAccess::constructContainer(JNIEnv*, void* result, const ContainerAndAccessInfo& container) {
    return m_hashAccess.constructContainer(result, container.container);
}

void* AutoSetAccess::constructContainer(void* result, void* container){
    return m_hashAccess.constructContainer(result, container);
}
#endif

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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    int newMetaType = QMetaType::type(typeName);
#else
    int newMetaType = QMetaType::fromName(typeName).id();
#endif
    if(newMetaType==QMetaType::UnknownType){
        QSharedPointer<AutoHashAccess> access(reinterpret_cast<AutoHashAccess*>(m_hashAccess.clone()), &containerDisposer);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        auto iface = m_hashAccess.m_keyMetaType.iface();
#endif
        newMetaType = registerContainerMetaType(typeName,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
                                       qtjambi_function_pointer<16,void(void*)>([access](void* ptr){
                                            access->destructContainer(ptr);
                                       }, qHash(typeName)),
                                       [](void* result, const void * other) -> void* {
                                        if(other){
                                            QHashData *const* map2 = reinterpret_cast<QHashData *const*>(other);
                                            QHashData* d2 = *map2;
                                            if(d2!=&QHashData::shared_null && d2->ref.ref())
                                                return new (result) QHashData *(d2);
                                        }
                                        return new (result) QHashData const*(&QHashData::shared_null);
                                       },
#endif //QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
                                       uint(sizeOf()),
                                       ushort(alignof(QSet<char>)),
#else
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
#endif
                                       QMetaType::NeedsConstruction
                                                   | QMetaType::NeedsDestruction
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                                   | QMetaType::MovableType,
#else
                                                   | QMetaType::RelocatableType,
#endif
                                       nullptr,
                                       nullptr,
                                       access);
        if(m_hashAccess.m_keyHashFunction){
            insertHashFunctionByMetaType(newMetaType,
                                            [access]
                                            (const void* ptr, hash_type seed)->hash_type{
                                                if(ptr){
                                                    hash_type hashCode = seed;
                                                    QHashData *const* map = reinterpret_cast<QHashData *const*>(ptr);
                                                    QHashData* d = *map;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                    Node* e = reinterpret_cast<Node*>(d);
                                                    Node *n = d->firstNode();
                                                    while (n && n != e) {
                                                        void* key = reinterpret_cast<char*>(n)+access->m_offset1;
                                                        hashCode += access->m_keyHashFunction(key, 0);
                                                        n = QHashData::nextNode(n);
                                                    }
#else
                                                    hashCode = seed;
                                                    QHashData::iterator n = d->begin(*access);
                                                    QHashData::iterator e = d->end(*access);
                                                    while (n!=e) {
                                                        const void* element = n.key();
                                                        hashCode += access->m_keyHashFunction(element, 0);
                                                        ++n;
                                                    }
#endif
                                                    return hashCode;
                                                }else{
                                                    return 0;
                                                }
                                            });
        }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        {
            int to = qMetaTypeId<QtMetaTypePrivate::QSequentialIterableImpl>();
            struct ConverterFunction : QtPrivate::AbstractConverterFunction{
                int _metaType_id;
                uint _metaType_flags;
                struct IteratorCapabilities{
                    uint _iteratorCapabilities:4;
                    uint _revision:3;
                    uint _containerCapabilities:4;
                    uint _unused:21;
                };
                uint _iteratorCapabilities;
                QtMetaTypePrivate::QSequentialIterableImpl::sizeFunc _size;
                QtMetaTypePrivate::QSequentialIterableImpl::atFunc _at;
                QtMetaTypePrivate::QSequentialIterableImpl::moveIteratorFunc2 _moveTo;
                QtMetaTypePrivate::QSequentialIterableImpl::appendFunction _append;
                QtMetaTypePrivate::QSequentialIterableImpl::advanceFunc _advance;
                QtMetaTypePrivate::QSequentialIterableImpl::getFunc _get;
                QtMetaTypePrivate::QSequentialIterableImpl::destroyIterFunc _destroyIter;
                QtMetaTypePrivate::QSequentialIterableImpl::equalIterFunc _equalIter;
                QtMetaTypePrivate::QSequentialIterableImpl::copyIterFunc _copyIter;

                ConverterFunction(const QMetaType& elementMetaType, size_t offset1)
                    : QtPrivate::AbstractConverterFunction(convert),
                      _metaType_id(elementMetaType.id()),
                      _metaType_flags(AbstractContainerAccess::isPointerType(elementMetaType)),
                      _iteratorCapabilities(0),
                      _size([](const void *p)->int{
                                QHashData *const* map = reinterpret_cast<QHashData *const*>(p);
                                return (*map)->size;
                            }),
                      _at(nullptr),
                      _moveTo([](const void *p, void ** iterator, QtMetaTypePrivate::QSequentialIterableImpl::Position position){
                                    QHashData *const* map = reinterpret_cast<QHashData *const*>(p);
                                    QHashData* d = *map;
                                    Node* e = reinterpret_cast<Node*>(d);
                                    *iterator = position==QtMetaTypePrivate::QSequentialIterableImpl::ToBegin ? d->firstNode() : e;
                                }),
                      _append(nullptr),
                      _advance([](void **p, int n){
                                    Node* node = reinterpret_cast<Node*>(*p);
                                    if(n>0){
                                        for(int i=0; node!=nullptr && i<n; ++i){
                                            node = QHashData::nextNode(node);
                                        }
                                    }else if(n<0){
                                        for(int i=0; node!=nullptr && i>n; --i){
                                            node = QHashData::previousNode(node);
                                        }
                                    }
                                    *p = node;
                                }),
                      _get(qtjambi_function_pointer<16,QtMetaTypePrivate::VariantData(void*const*,int,uint)>([offset1](void * const *p, int metaTypeId, uint flags)->QtMetaTypePrivate::VariantData{
                                char* node = reinterpret_cast<char*>(*p);
                                return QtMetaTypePrivate::VariantData(metaTypeId, node+offset1, flags);
                            })),
                      _destroyIter([](void **){}),
                      _equalIter([](void * const *p, void * const *other)->bool{ return *p==*other; }),
                      _copyIter(nullptr)
                {
                    IteratorCapabilities* capabilities = reinterpret_cast<IteratorCapabilities*>(&_iteratorCapabilities);
                    capabilities->_revision = 1;
                    capabilities->_iteratorCapabilities = QtMetaTypePrivate::BiDirectionalCapability;
                    capabilities->_containerCapabilities = 0;
                }

                static bool convert(const AbstractConverterFunction *_f, const void *src, void*target){
                    const ConverterFunction* f = static_cast<const ConverterFunction*>(_f);
                    QtMetaTypePrivate::QSequentialIterableImpl* i = new(target) QtMetaTypePrivate::QSequentialIterableImpl();
                    i->_iterable = src;
                    i->_metaType_id = f->_metaType_id;
                    i->_metaType_flags = f->_metaType_flags;
                    i->_iteratorCapabilities = f->_iteratorCapabilities;
                    i->_size = f->_size;
                    i->_at = f->_at;
                    i->_moveTo = f->_moveTo;
                    i->_append = f->_append;
                    i->_advance = f->_advance;
                    i->_get = f->_get;
                    i->_destroyIter = f->_destroyIter;
                    i->_equalIter = f->_equalIter;
                    i->_copyIter = f->_copyIter;
                    return true;
                }
            };
            registerConverter(new ConverterFunction(m_hashAccess.m_keyMetaType, m_hashAccess.m_offset1), newMetaType, to);
        }
        if(QMetaType::hasRegisteredComparators(m_hashAccess.m_keyMetaType.id())
            || registeredComparator(m_hashAccess.m_keyMetaType.id())
            || m_hashAccess.m_keyMetaType.id()<QMetaType::User
            || AbstractContainerAccess::isPointerType(m_hashAccess.m_keyMetaType)){
            struct Comparator : QtPrivate::AbstractComparatorFunction{
                static bool equals(const AbstractComparatorFunction *_c, const void *a, const void *b){
                    const Comparator* c = static_cast<const Comparator*>(_c);
                    return c->access->equal(a, b);
                }
                static void destroy(AbstractComparatorFunction *_c){
                    delete static_cast<Comparator*>(_c);
                }
                QSharedPointer<AutoHashAccess> access;

                Comparator(const QSharedPointer<AutoHashAccess>& _access)
                    : QtPrivate::AbstractComparatorFunction(nullptr, equals, destroy),
                      access(_access)
                {
                }
            };
            RegistryAPI::registerComparator(new Comparator(access), newMetaType);
        }
        if(QMetaType::hasRegisteredDebugStreamOperator(m_hashAccess.m_keyMetaType.id())
                || registeredDebugStreamOperator(m_hashAccess.m_keyMetaType.id())
                || m_hashAccess.m_keyMetaType.id()<QMetaType::User
                || AbstractContainerAccess::isPointerType(m_hashAccess.m_keyMetaType)){
            struct DebugStreamFunction : QtPrivate::AbstractDebugStreamFunction{
                static void stream(const AbstractDebugStreamFunction *_f, QDebug& dbg, const void *ptr){
                    const DebugStreamFunction* f = static_cast<const DebugStreamFunction*>(_f);
                    QHashData *const* map = reinterpret_cast<QHashData *const*>(ptr);
                    QHashData* d = *map;
                    Node* e = reinterpret_cast<Node*>(d);
                    Node *n = d->firstNode();
                    int i=0;
                    dbg = dbg.nospace().noquote();
                    dbg << "QSet(";
                    while (n && n != e) {
                        if(i>0)
                            dbg << ", ";
                        void* key = reinterpret_cast<char*>(n)+f->access->m_offset1;
                        CoreAPI::appendToDebugStream(dbg, f->access->m_keyMetaType.id(), key);
                        n = QHashData::nextNode(n);
                        ++i;
                    }
                    dbg << ")";
                }
                static void destroy(AbstractDebugStreamFunction *_f){
                    delete static_cast<DebugStreamFunction*>(_f);
                }
                QSharedPointer<AutoHashAccess> access;

                DebugStreamFunction(const QSharedPointer<AutoHashAccess>& _access)
                    : QtPrivate::AbstractDebugStreamFunction(stream, destroy),
                      access(_access)
                {
                }
            };
            RegistryAPI::registerDebugStreamOperator(new DebugStreamFunction(access), newMetaType);
        }
        {
            int _metaType_id = m_hashAccess.m_keyMetaType.id();
            size_t offset1 = m_hashAccess.m_offset1;
            QMetaType::registerStreamOperators(newMetaType,
                                               qtjambi_function_pointer<16,void(QDataStream &, const void *)>(
                                                                               [_metaType_id,offset1](QDataStream &s, const void * ptr){
                                                                                    QHashData *const* map = reinterpret_cast<QHashData *const*>(ptr);
                                                                                    QHashData* d = *map;
                                                                                    Node* e = reinterpret_cast<Node*>(d);
                                                                                    Node *n = d->firstNode();
                                                                                    s << (*map)->size;
                                                                                    while (n && n != e) {
                                                                                        void* key = reinterpret_cast<char*>(n)+offset1;
                                                                                        QMetaType::save(s, _metaType_id, key);
                                                                                        n = QHashData::nextNode(n);
                                                                                    }
                                                                               }, _metaType_id),
                                               qtjambi_function_pointer<16,void(QDataStream &, void *)>(
                                                                               [access](QDataStream &s, void * p){
                                                                                    QHashData ** map = reinterpret_cast<QHashData **>(p);
                                                                                    int size = 0;
                                                                                    s >> size;
                                                                                    access->clear(nullptr, {nullptr, p});
                                                                                    access->reserve(nullptr, {nullptr, p}, size);
                                                                                    QHashData*& d = *map;
                                                                                    Node*& e = reinterpret_cast<Node*&>(d);
                                                                                    for(int i=0; i<size; ++i){
                                                                                        void* key = QMetaType::create(access->m_keyMetaType.id(), nullptr);
                                                                                        QMetaType::load(s, access->m_keyMetaType.id(), key);
                                                                                        uint h;
                                                                                        Node **node = access->findNode(map, key, &h);
                                                                                        if (*node == e) {
                                                                                            if (d->willGrow())
                                                                                                node = access->findNode(map, key, h);
                                                                                            access->createNode(d, h, key, nullptr, node);
                                                                                            continue;
                                                                                        }
                                                                                        QMetaType::destroy(access->m_keyMetaType.id(), key);
                                                                                   }
                                                                               }, _metaType_id)
                                               );
        }
#else
        const QMetaType to = QMetaType::fromType<QSequentialIterable>();
        QMetaType::registerMutableViewFunction([newMetaType](void *src, void *target)->bool{
            new(target) QIterable<QMetaSequence>(QMetaSequence(createMetaSequenceInterface(newMetaType)), reinterpret_cast<void **>(src));
            return true;
        }, QMetaType(newMetaType), to);
        QMetaType::registerConverterFunction([newMetaType](const void *src, void *target)->bool{
            new(target) QIterable<QMetaSequence>(QMetaSequence(createMetaSequenceInterface(newMetaType)), reinterpret_cast<void const*const*>(src));
            return true;
        }, QMetaType(newMetaType), to);
#endif
    }else{
        registerContainerAccess(newMetaType, this);
    }
    return newMetaType;
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)

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
#endif

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
    public:
        ElementIterator(std::unique_ptr<AbstractHashAccess::KeyValueIterator>&& _iter) : iter(std::move(_iter)) {}
        bool hasNext() override{
            return iter->hasNext();
        }
        jobject next(JNIEnv * env) override{
            return iter->next(env).first;
        }
        const void* next() override {
            return iter->next().first;
        }
    };
    return std::unique_ptr<AbstractSetAccess::ElementIterator>(new ElementIterator(m_hashAccess.keyValueIterator(container)));
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
