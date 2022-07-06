#include "qtjambi_core.h"
#include <QtCore/QReadWriteLock>
#include <QtCore/QMap>
#include <QtCore/QSharedPointer>
#include <QtCore/QSequentialIterable>
#include "qtjambi_containeraccess_p.h"
#include "qtjambi_functionpointer.h"
#include "qtjambi_registry_p.h"

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
QDebug& debug_stream(QDebug& debug, uint typeId, const void* ptr);
const QtPrivate::AbstractComparatorFunction * qtjambi_registered_comparator(int typeId);
const QtPrivate::AbstractDebugStreamFunction * qtjambi_registered_debugstream_operator(int typeId);
bool is_equals(const QMetaType& keyMetaType, const void * ptr, const void* ptr2);
bool is_less(const QMetaType& keyMetaType, const void * ptr, const void* ptr2);
#endif

AutoListAccess::AutoListAccess(
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
        )
    : AbstractListAccess(),
      m_elementMetaType(elementMetaType),
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
      m_isLargeOrStaticType(isStaticType || size_t(QMetaType::sizeOf(elementMetaType))>sizeof(void*)),
#endif
      m_hashFunction(hashFunction),
      m_internalToExternalConverter(internalToExternalConverter),
      m_externalToInternalConverter(externalToInternalConverter),
      m_offset(0)
{
    Q_ASSERT(m_elementMetaType.id()!=QMetaType::UnknownType
            && m_elementMetaType.id()!=QMetaType::Void);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    m_offset = m_elementMetaType.sizeOf();
    if(m_offset%elementAlign>0)
        m_offset += elementAlign-m_offset%elementAlign;
#else
    m_offset = m_elementMetaType.sizeOf();
    if(m_offset%m_elementMetaType.alignOf()>0)
        m_offset += m_elementMetaType.alignOf()-m_offset%m_elementMetaType.alignOf();
#endif
}

AutoListAccess::AutoListAccess(const AutoListAccess& other)
  : AbstractListAccess(),
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    m_elementMetaType(other.m_elementMetaType.id()),
    m_isLargeOrStaticType(other.m_isLargeOrStaticType),
#else
    m_elementMetaType(other.m_elementMetaType),
#endif
    m_hashFunction(other.m_hashFunction),
    m_internalToExternalConverter(other.m_internalToExternalConverter),
    m_externalToInternalConverter(other.m_externalToInternalConverter),
    m_offset(other.m_offset)
{
}

void AutoListAccess::dispose(){
    delete this;
}

void AutoListAccess::analyzeElements(const void* container, ElementAnalyzer analyzer, void* data){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    const QListData* p = reinterpret_cast<const QListData*>(container);
    if(m_isLargeOrStaticType){
        for(int i=0; i<p->size(); ++i){
            Node* v = reinterpret_cast<Node*>(p->at(i));
            if(!analyzer(v->v, data))
                break;
        }
    }else{
        for(int i=0; i<p->size(); ++i){
            void** v = p->at(i);
            if(!analyzer(v, data))
                break;
        }
    }
#else
    const QArrayDataPointer<char>* p = reinterpret_cast<const QArrayDataPointer<char>*>(container);
    for(qsizetype i = 0; i<p->size; ++i){
        void* element = p->ptr + i * m_offset;
        if(!analyzer(element, data))
                break;
    }
#endif
}

AutoListAccess* AutoListAccess::clone(){
    return new AutoListAccess(*this);
}

void* AutoListAccess::constructContainer(void* result, const void* container){
    result = new(result) QList<char>();
    if(container)
        assign(result, container);
    return result;
}

void* AutoListAccess::createContainer(){
    return new QList<char>();
}

void* AutoListAccess::copyContainer(const void* container){
    void* result = createContainer();
    if(container)
        assign(result, container);
    return result;
}

void AutoListAccess::destructContainer(void* container) {
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QListData* p = reinterpret_cast<QListData*>(container);
    if (!p->d->ref.deref())
        dealloc(p);
#else
    QArrayDataPointer<char>* p = reinterpret_cast<QArrayDataPointer<char>*>(container);
    if (!p->size)
        return;
    if(!p->deref()){
        for(qsizetype i = 0; i<p->size; ++i){
            void* target = p->ptr + i * m_offset;
            m_elementMetaType.destruct(target);
        }
        QArrayData::deallocate(p->d, m_elementMetaType.sizeOf(), qMax<qsizetype>(m_elementMetaType.alignOf(), alignof(QArrayData)));
    }
    p->d = nullptr;
    p->size = 0;
    p->ptr = nullptr;
    reinterpret_cast<QList<char>*>(container)->~QList();
#endif
}

void AutoListAccess::deleteContainer(void* container)
{
    destructContainer(container);
    operator delete(container);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

void* AutoListAccess::constructContainer(void* result, void* container){
    return new(result) QList<char>(std::move(*reinterpret_cast<QList<char>*>(container)));
}

bool AutoListAccess::equals(const void* container1, const void* container2)
{
    const QtPrivate::QMetaTypeInterface *iface = m_elementMetaType.iface();
    const QArrayDataPointer<char>* p1 = reinterpret_cast<const QArrayDataPointer<char>*>(container1);
    const QArrayDataPointer<char>* p2 = reinterpret_cast<const QArrayDataPointer<char>*>(container2);
    if(p1->size!=p2->size)
        return false;
    if(p1->ptr==p2->ptr)
        return true;
    for(qsizetype i = 0; i<p1->size; ++i){
        void* element1 = p1->ptr + i * m_offset;
        void* element2 = p2->ptr + i * m_offset;
        if(!iface->equals(iface, element1, element2))
            return false;
    }
    return true;
}

void AutoListAccess::debugStream(QDebug &s, const void *container)
{
    const QtPrivate::QMetaTypeInterface *iface = m_elementMetaType.iface();
    const QArrayDataPointer<char>* p = reinterpret_cast<const QArrayDataPointer<char>*>(container);
    s = s.nospace().noquote();
    s << "QList(";
    for(qsizetype i = 0; i<p->size; ++i){
        const void* element = p->ptr + i * m_offset;
        if(i>0)
            s << ", ";
        iface->debugStream(iface, s, element);
    }
    s << ")";
}

void AutoListAccess::dataStreamOut(QDataStream &s, const void *container)
{
    const QtPrivate::QMetaTypeInterface *iface = m_elementMetaType.iface();
    const QArrayDataPointer<char>* p = reinterpret_cast<const QArrayDataPointer<char>*>(container);
    s << quint32(p->size);
    for(qsizetype i = 0; i<p->size; ++i){
        const void* element = p->ptr + i * m_offset;
        iface->dataStreamOut(iface, s, element);
    }
}

void AutoListAccess::dataStreamIn(QDataStream &s, void *container)
{
    const QtPrivate::QMetaTypeInterface *iface = m_elementMetaType.iface();
    QArrayDataPointer<char>* p = reinterpret_cast<QArrayDataPointer<char>*>(container);
    quint32 size;
    s >> size;

    QArrayDataPointer<char> detached(allocate(size));
    for(quint32 i = 0; i<size; ++i){
        void* target = detached.ptr + i * m_offset;
        iface->dataStreamIn(iface, s, target);
        ++detached.size;
    }
    if (detached->ptr)
        detached.setFlag(QArrayData::CapacityReserved);
    swapAndDestroy(p, std::move(detached));
}
#endif

void AutoListAccess::assign(void* container, const void* other)
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QListData* p = reinterpret_cast<QListData*>(container);
    const QListData* p2 = reinterpret_cast<const QListData*>(other);
    QListData tmp;
    tmp.d = p->d;
    p->d = p2->d;
    p2->d->ref.ref();
    if (!tmp.d->ref.deref())
        dealloc(&tmp);
#else
    QArrayDataPointer<char>* p = reinterpret_cast<QArrayDataPointer<char>*>(container);
    const QArrayDataPointer<char>* p2 = reinterpret_cast<const QArrayDataPointer<char>*>(other);
    QArrayDataPointer<char> detached(allocate(p2->size));
    for(qsizetype i = 0; i<p2->size; ++i){
        const void* source = p2->ptr + i * m_offset;
        void* target = detached.ptr + i * m_offset;
        m_elementMetaType.construct(target, source);
        ++detached.size;
    }
    if (detached->ptr)
        detached.setFlag(QArrayData::CapacityReserved);
    swapAndDestroy(p, std::move(detached));
#endif
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
typedef QMap<int, QSharedPointer<class AutoListAccess>> AccessHash;
Q_GLOBAL_STATIC(AccessHash, gAccessHash)
typedef QMap<int, QtMetaContainerPrivate::QMetaSequenceInterface> MetaSequenceHash;
Q_GLOBAL_STATIC(MetaSequenceHash, gMetaSequenceHash)
Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gLock, (QReadWriteLock::Recursive))

QReadWriteLock* _gLock(){
    return gLock();
}

QMap<int, QtMetaContainerPrivate::QMetaSequenceInterface>& _gMetaSequenceHash(){
    return *gMetaSequenceHash();
}

QSharedPointer<class AutoListAccess> getListAccess(const QtPrivate::QMetaTypeInterface *iface){
    QSharedPointer<class AutoListAccess> access;
    {
        QReadLocker locker(gLock());
        access = gAccessHash->value(iface->typeId.loadAcquire());
    }
    return access;
}
void AutoListAccess::defaultCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr){
    if(QSharedPointer<class AutoListAccess> access = getListAccess(iface)){
        access->constructContainer(ptr);
    }
}
void AutoListAccess::copyCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, const void *other){
    if(QSharedPointer<class AutoListAccess> access = getListAccess(iface)){
        access->constructContainer(ptr, other);
    }
}
void AutoListAccess::moveCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, void *other){
    if(QSharedPointer<class AutoListAccess> access = getListAccess(iface)){
        access->constructContainer(ptr, other);
    }
}
void AutoListAccess::dtor(const QtPrivate::QMetaTypeInterface *iface, void *ptr){
    if(QSharedPointer<class AutoListAccess> access = getListAccess(iface)){
        access->destructContainer(ptr);
    }
}
bool AutoListAccess::equalsFn(const QtPrivate::QMetaTypeInterface *iface, const void *ptr1, const void *ptr2){
    if(QSharedPointer<class AutoListAccess> access = getListAccess(iface)){
        return access->equals(ptr1, ptr2);
    }
    return false;
}
void AutoListAccess::debugStreamFn(const QtPrivate::QMetaTypeInterface *iface, QDebug &s, const void *ptr){
    if(QSharedPointer<class AutoListAccess> access = getListAccess(iface)){
        access->debugStream(s, ptr);
    }
}
void AutoListAccess::dataStreamOutFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, const void *ptr){
    if(QSharedPointer<class AutoListAccess> access = getListAccess(iface)){
        access->dataStreamOut(s, ptr);
    }
}
void AutoListAccess::dataStreamInFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, void *ptr){
    if(QSharedPointer<class AutoListAccess> access = getListAccess(iface)){
        access->dataStreamIn(s, ptr);
    }
}

QtMetaContainerPrivate::QMetaSequenceInterface* AutoListAccess::createMetaSequenceInterface(int newMetaType){
    {
        QReadLocker locker(gLock());
        Q_UNUSED(locker)
        if(gMetaSequenceHash->contains(newMetaType))
           return &(*gMetaSequenceHash)[newMetaType];
    }
    using namespace QtMetaContainerPrivate;
    QMetaSequenceInterface* metaSequenceInterface;
    {
        QWriteLocker locker(gLock());
        metaSequenceInterface = &(*gMetaSequenceHash)[newMetaType];
    }
    QSharedPointer<class AutoListAccess> access = getListAccess(QMetaType(newMetaType).iface());
    metaSequenceInterface->valueMetaType = access->m_elementMetaType.iface();
    metaSequenceInterface->iteratorCapabilities = InputCapability | ForwardCapability | BiDirectionalCapability;
    metaSequenceInterface->addRemoveCapabilities = CanAddAtBegin|CanRemoveAtBegin|CanAddAtEnd|CanRemoveAtEnd;

    metaSequenceInterface->sizeFn = [](const void *c) -> qsizetype {
        if(!c)
            return 0;
        const QArrayDataPointer<char>* p = reinterpret_cast<const QArrayDataPointer<char>*>(c);
        return p->size;
    };

    metaSequenceInterface->clearFn = qtjambi_function_pointer<16,void(void *)>([newMetaType](void *c) {
                                        if(!c)
                                            return;
                                        QSharedPointer<class AutoListAccess> access = getListAccess(QMetaType(newMetaType).iface());
                                        if(!access)
                                            return;
                                        access->clear(nullptr, c);
                                    }, newMetaType);
    struct MetaIterator{
        MetaIterator(char* _iterator, const QSharedPointer<class AutoListAccess>& _access) : iterator(_iterator), access(_access) {}
        char* iterator;
        QSharedPointer<class AutoListAccess> access;
    };
    metaSequenceInterface->createIteratorFn = qtjambi_function_pointer<16,void*(void *,QMetaContainerInterface::Position)>(
                [newMetaType](void *c, QMetaContainerInterface::Position p) -> void* {
                        if(!c)
                            return nullptr;
                        QSharedPointer<class AutoListAccess> access = getListAccess(QMetaType(newMetaType).iface());
                        if(!access)
                            return nullptr;
                        const QArrayDataPointer<char>* ap = reinterpret_cast<const QArrayDataPointer<char>*>(c);
                        switch (p) {
                        case QMetaContainerInterface::Unspecified:
                            return new MetaIterator(nullptr, access);
                        case QMetaContainerInterface::AtBegin:
                            return new MetaIterator(ap->ptr, access);
                            break;
                        case QMetaContainerInterface::AtEnd:
                            return new MetaIterator(reinterpret_cast<char*>(ap->ptr + ap->size * access->m_offset), access);
                            break;
                        }
                        return nullptr;
                    }, newMetaType);
    metaSequenceInterface->destroyIteratorFn = [](const void *i) {
                    delete static_cast<const MetaIterator *>(i);
                };
    metaSequenceInterface->compareIteratorFn = [](const void *i, const void *j) {
                    return static_cast<const MetaIterator *>(i)->iterator == static_cast<const MetaIterator *>(j)->iterator;
                };
    metaSequenceInterface->copyIteratorFn = [](void *i, const void *j) {
                    static_cast<MetaIterator *>(i)->iterator = static_cast<const MetaIterator *>(j)->iterator;
                    static_cast<MetaIterator *>(i)->access = static_cast<const MetaIterator *>(j)->access;
                };
    metaSequenceInterface->advanceIteratorFn = [](void *i, qsizetype step) {
                    static_cast<MetaIterator*>(i)->iterator += step * static_cast<MetaIterator*>(i)->access->m_offset;
                };
    metaSequenceInterface->diffIteratorFn = [](const void *i, const void *j) -> qsizetype {
                    return std::distance(static_cast<const MetaIterator*>(j)->iterator,
                                         static_cast<const MetaIterator*>(i)->iterator)/static_cast<const MetaIterator*>(j)->access->m_offset;
                };
    metaSequenceInterface->createConstIteratorFn = QMetaSequenceInterface::CreateConstIteratorFn(metaSequenceInterface->createIteratorFn);
    metaSequenceInterface->destroyConstIteratorFn = metaSequenceInterface->destroyIteratorFn;
    metaSequenceInterface->compareConstIteratorFn = metaSequenceInterface->compareIteratorFn;
    metaSequenceInterface->copyConstIteratorFn = metaSequenceInterface->copyIteratorFn;
    metaSequenceInterface->advanceConstIteratorFn = metaSequenceInterface->advanceIteratorFn;
    metaSequenceInterface->diffConstIteratorFn = metaSequenceInterface->diffIteratorFn;
    metaSequenceInterface->valueAtIndexFn = qtjambi_function_pointer<16,void(const void *, qsizetype, void *)>([newMetaType](const void *container, qsizetype index, void *r) {
        if(!container)
            return;
        QSharedPointer<class AutoListAccess> access = getListAccess(QMetaType(newMetaType).iface());
        if(!access)
            return;
        const QArrayDataPointer<char>* p = reinterpret_cast<const QArrayDataPointer<char>*>(container);
        Q_ASSERT_X(index >= 0 && index < p->size, "QList<T>::at", "index out of range");
        void* ptr = p->ptr+index*access->m_offset;
        access->m_elementMetaType.destruct(r);
        access->m_elementMetaType.construct(r, ptr);
    }, newMetaType);
    metaSequenceInterface->setValueAtIndexFn = qtjambi_function_pointer<16,void(void *, qsizetype, const void *)>([newMetaType](void *container, qsizetype index, const void *e) {
        if(!container)
            return;
        QSharedPointer<class AutoListAccess> access = getListAccess(QMetaType(newMetaType).iface());
        if(!access)
            return;
        QArrayDataPointer<char>* p = reinterpret_cast<QArrayDataPointer<char>*>(container);
        if(index == p->size){
            access->emplace(p, index, e, 1);
        }else{
            Q_ASSERT_X(index >= 0 && index < p->size, "QList<T>::replace", "index out of range");
            access->detach(p);
            void* target = p->ptr + index * access->m_offset;
            access->m_elementMetaType.destruct(target);
            access->m_elementMetaType.construct(target, e);
        }
    }, newMetaType);
    metaSequenceInterface->addValueFn = qtjambi_function_pointer<16,void(void *, const void *, QMetaSequenceInterface::Position)>([newMetaType](void *container, const void *e, QMetaSequenceInterface::Position position) {
        if(!container)
            return;
        QSharedPointer<class AutoListAccess> access = getListAccess(QMetaType(newMetaType).iface());
        if(!access)
            return;
        QArrayDataPointer<char>* p = reinterpret_cast<QArrayDataPointer<char>*>(container);
        if(position==QMetaSequenceInterface::AtBegin){
            access->emplace(p, 0, e, 1);
        }else{
            access->emplace(p, p->size, e, 1);
        }
    }, newMetaType);
    metaSequenceInterface->removeValueFn = qtjambi_function_pointer<16,void(void *, QMetaSequenceInterface::Position)>([newMetaType](void *container, QMetaSequenceInterface::Position position) {
        if(!container)
            return;
        QSharedPointer<class AutoListAccess> access = getListAccess(QMetaType(newMetaType).iface());
        if(!access)
            return;
        QArrayDataPointer<char>* p = reinterpret_cast<QArrayDataPointer<char>*>(container);
        if(position==QMetaSequenceInterface::AtBegin){
            access->removeAt(nullptr, container, 0);
        }else{
            access->removeAt(nullptr, container, p->size-1);
        }
    }, newMetaType);
    metaSequenceInterface->valueAtIteratorFn = [](const void *i, void *r) {
        const MetaIterator* it = static_cast<const MetaIterator*>(i);
        it->access->m_elementMetaType.destruct(r);
        it->access->m_elementMetaType.construct(r, it->iterator);
    };
    metaSequenceInterface->setValueAtIteratorFn = [](const void *i, const void *r) {
        const MetaIterator* it = static_cast<const MetaIterator*>(i);
        it->access->m_elementMetaType.destruct(it->iterator);
        it->access->m_elementMetaType.construct(it->iterator, r);
    };
    metaSequenceInterface->insertValueAtIteratorFn = nullptr;
    metaSequenceInterface->valueAtConstIteratorFn = metaSequenceInterface->valueAtIteratorFn;
    metaSequenceInterface->eraseValueAtIteratorFn = nullptr;
    metaSequenceInterface->eraseRangeAtIteratorFn = nullptr;
    return metaSequenceInterface;
}
#endif

int AutoListAccess::registerContainer(const QByteArray& typeName)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    int newMetaType = QMetaType::type(typeName);
#else
    int newMetaType = QMetaType::fromName(typeName).id();
#endif
    if(newMetaType==QMetaType::UnknownType){
        QSharedPointer<AutoListAccess> access(new AutoListAccess(*this), &containerDisposer);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        auto iface = m_elementMetaType.iface();
#endif
        newMetaType = registerMetaType(typeName,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                       qtjambi_function_pointer<16,void(void*)>([access](void* ptr){
                                            access->destructContainer(ptr);
                                       }, qHash(typeName)),
                                       [](void* result, const void * other) -> void* {
                                            if(other){
                                                return new(result) QList<char>(*reinterpret_cast<const QList<char>*>(other));
                                            }else{
                                                return new(result) QList<char>();
                                            }
                                       },
                                       uint(sizeof(QList<char>)),
                                       uint(alignof(QList<char>)),
#else
                                       AutoListAccess::defaultCtr,
                                       AutoListAccess::copyCtr,
                                       AutoListAccess::moveCtr,
                                       AutoListAccess::dtor,
                                       (iface->equals
                                            || (iface->flags & QMetaType::IsPointer)
                                            || (iface->flags & QMetaType::IsEnumeration)) ? AutoListAccess::equalsFn : nullptr,
                                       nullptr,
                                       (iface->debugStream
                                            || (iface->flags & QMetaType::IsPointer)
                                            || (iface->flags & QMetaType::IsEnumeration)) ? AutoListAccess::debugStreamFn : nullptr,
                                       (iface->dataStreamOut || (iface->flags & QMetaType::IsEnumeration)) ? AutoListAccess::dataStreamOutFn : nullptr,
                                       (iface->dataStreamIn || (iface->flags & QMetaType::IsEnumeration)) ? AutoListAccess::dataStreamInFn : nullptr,
                                       nullptr,
                                       uint(sizeof(QList<char>)),
                                       ushort(alignof(QList<char>)),
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
                                       nullptr);
        if(m_hashFunction){
            insertHashFunctionByMetaType(newMetaType,
                                            [access]
                                            (const void* ptr, hash_type seed)->hash_type{
                                                if(ptr){
                                                    hash_type hashCode = seed;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                    const QListData* p = reinterpret_cast<const QListData*>(ptr);
                                                    if(access->m_isLargeOrStaticType){
                                                        for(int i=0; i<p->size(); ++i){
                                                            Node* v = reinterpret_cast<Node*>(p->at(i));
                                                            hashCode = hashCode ^ (access->m_hashFunction(v->v, 0) + 0x9e3779b9 + (hashCode << 6) + (hashCode >> 2));
                                                        }
                                                    }else{
                                                        for(int i=0; i<p->size(); ++i){
                                                            void** v = p->at(i);
                                                            hashCode = hashCode ^ (access->m_hashFunction(v, 0) + 0x9e3779b9 + (hashCode << 6) + (hashCode >> 2));
                                                        }
                                                    }
#else
                                                    const QArrayDataPointer<char>* p = reinterpret_cast<const QArrayDataPointer<char>*>(ptr);
                                                    for(size_t i=0; i<size_t(p->size); ++i){
                                                        const void* element = p->ptr + i * access->m_offset;
                                                        hashCode = hashCode ^ (access->m_hashFunction(element, 0) + 0x9e3779b9 + (hashCode << 6) + (hashCode >> 2));
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

                ConverterFunction(const QMetaType& elementMetaType, bool isLargeOrStaticType)
                    : QtPrivate::AbstractConverterFunction(convert),
                      _metaType_id(elementMetaType.id()),
                      _metaType_flags(qtjambi_is_pointer_type(elementMetaType)),
                      _iteratorCapabilities(0),
                      _size([](const void *p)->int{
                                const QListData* d = reinterpret_cast<const QListData*>(p);
                                return d->size();
                            }),
                      _at(isLargeOrStaticType ? QtMetaTypePrivate::QSequentialIterableImpl::atFunc([](const void *p, int index)->const void *{
                                                    const QListData* d = reinterpret_cast<const QListData*>(p);
                                                    void* v = d->at(index);
                                                    return reinterpret_cast<Node *>(v)->v;
                                                })
                                              : QtMetaTypePrivate::QSequentialIterableImpl::atFunc([](const void *p, int index)->const void *{
                                                    const QListData* d = reinterpret_cast<const QListData*>(p);
                                                    void* v = d->at(index);
                                                    return v;
                                                })),
                      _moveTo([](const void *p, void ** iter, QtMetaTypePrivate::QSequentialIterableImpl::Position position){
                                    const QListData* d = reinterpret_cast<const QListData*>(p);
                                    *iter = position==QtMetaTypePrivate::QSequentialIterableImpl::ToBegin ? d->begin() : d->end();
                                }),
                      _append(nullptr),
                      _advance([](void **p, int steps){
                                    void** iter = *reinterpret_cast<void***>(p);
                                    *reinterpret_cast<void***>(p) = (iter+steps);
                                }),
                      _get(isLargeOrStaticType ? QtMetaTypePrivate::QSequentialIterableImpl::getFunc([]( void * const *p, int metaTypeId, uint flags)->QtMetaTypePrivate::VariantData{
                                                    void** iter = *reinterpret_cast<void**const*>(p);
                                                    return QtMetaTypePrivate::VariantData(metaTypeId, reinterpret_cast<Node *>(iter)->v, flags);
                                                 })
                                               : QtMetaTypePrivate::QSequentialIterableImpl::getFunc([]( void * const *p, int metaTypeId, uint flags)->QtMetaTypePrivate::VariantData{
                                                    void** iter = *reinterpret_cast<void**const*>(p);
                                                    return QtMetaTypePrivate::VariantData(metaTypeId, iter, flags);
                                                 })),
                      _destroyIter([](void **){}),
                      _equalIter([](void * const *p, void * const *other)->bool{
                                        void** iter1 = *reinterpret_cast<void**const*>(p);
                                        void** iter2 = *reinterpret_cast<void**const*>(other);
                                        return iter1==iter2;
                                }),
                      _copyIter(nullptr)
                {
                    IteratorCapabilities& capabilities = reinterpret_cast<IteratorCapabilities&>(_iteratorCapabilities);
                    capabilities._revision = 1;
                    capabilities._iteratorCapabilities = QtMetaTypePrivate::BiDirectionalCapability;
                    capabilities._containerCapabilities = 0;
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
            qtjambi_register_converter(new ConverterFunction(m_elementMetaType, m_isLargeOrStaticType), newMetaType, to);
        }
        if(QMetaType::hasRegisteredComparators(m_elementMetaType.id())
            || qtjambi_registered_comparator(m_elementMetaType.id())
            || m_elementMetaType.id()<QMetaType::User
            || qtjambi_is_pointer_type(m_elementMetaType)){
            struct Comparator : QtPrivate::AbstractComparatorFunction{
                static bool lessThan(const AbstractComparatorFunction *_c, const void *a, const void *b){
                    const Comparator* c = static_cast<const Comparator*>(_c);
                    const QListData* p = reinterpret_cast<const QListData*>(a);
                    const QListData* p2 = reinterpret_cast<const QListData*>(b);
                    if(p->size()<p2->size())
                        return false;
                    if(p->size()>p2->size())
                        return true;
                    if(p->d==p2->d)
                        return false;
                    for(int i = 0; i<p->size(); ++i){
                        void* element1 = p->at(i);
                        void* element2 = p2->at(i);
                        if (c->m_isLargeOrStaticType) {
                            element1 = reinterpret_cast<Node *>(element1)->v;
                            element2 = reinterpret_cast<Node *>(element2)->v;
                        }
                        if(!is_less(c->_metaType_id, element1, element2))
                            return false;
                    }
                    return true;
                }
                static bool equals(const AbstractComparatorFunction *_c, const void *a, const void *b){
                    const Comparator* c = static_cast<const Comparator*>(_c);
                    const QListData* p = reinterpret_cast<const QListData*>(a);
                    const QListData* p2 = reinterpret_cast<const QListData*>(b);
                    if(p->size()!=p2->size())
                        return false;
                    if(p->d==p2->d)
                        return true;
                    for(int i = 0; i<p->size(); ++i){
                        void* element1 = p->at(i);
                        void* element2 = p2->at(i);
                        if (c->m_isLargeOrStaticType) {
                            element1 = reinterpret_cast<Node *>(element1)->v;
                            element2 = reinterpret_cast<Node *>(element2)->v;
                        }
                        if(!is_equals(c->_metaType_id, element1, element2))
                            return false;
                    }
                    return true;
                }
                static void destroy(AbstractComparatorFunction *_c){
                    delete static_cast<Comparator*>(_c);
                }
                QMetaType _metaType_id;
                bool m_isLargeOrStaticType;

                Comparator(const QMetaType& keyMetaType, bool isLargeOrStaticType)
                    : QtPrivate::AbstractComparatorFunction(lessThan, equals, destroy),
                      _metaType_id(keyMetaType.id()),
                      m_isLargeOrStaticType(isLargeOrStaticType)
                {

                }
            };
            qtjambi_register_comparator(new Comparator(m_elementMetaType, m_isLargeOrStaticType), newMetaType);
        }
        if(QMetaType::hasRegisteredDebugStreamOperator(m_elementMetaType.id())
                || qtjambi_registered_debugstream_operator(m_elementMetaType.id())
                || m_elementMetaType.id()<QMetaType::User
                || qtjambi_is_pointer_type(m_elementMetaType)){
            struct DebugStreamFunction : QtPrivate::AbstractDebugStreamFunction{
                static void stream(const AbstractDebugStreamFunction *_f, QDebug& d, const void *ptr){
                    const DebugStreamFunction* f = static_cast<const DebugStreamFunction*>(_f);
                    const QListData* p = reinterpret_cast<const QListData*>(ptr);
                    d = d.nospace().noquote();
                    d << "(";
                    for(int i=0; i<p->size(); ++i){
                        if(i>0)
                            d << ", ";
                        void* element = p->at(i);
                        if (f->m_isLargeOrStaticType)
                            element = reinterpret_cast<Node *>(element)->v;
                        debug_stream(d, f->_metaType_id, element);
                    }
                    d << ")";
                }
                static void destroy(AbstractDebugStreamFunction *_f){
                    delete static_cast<DebugStreamFunction*>(_f);
                }
                int _metaType_id;
                bool m_isLargeOrStaticType;

                DebugStreamFunction(const QMetaType& keyMetaType, bool isLargeOrStaticType)
                    : QtPrivate::AbstractDebugStreamFunction(stream, destroy),
                      _metaType_id(keyMetaType.id()),
                      m_isLargeOrStaticType(isLargeOrStaticType)
                {

                }
            };
            qtjambi_register_debugstream_operator(new DebugStreamFunction(m_elementMetaType, m_isLargeOrStaticType), newMetaType);
        }
        {
            int _metaType_id = m_elementMetaType.id();
            if(m_isLargeOrStaticType){
                QMetaType::registerStreamOperators(newMetaType,
                                                   qtjambi_function_pointer<16,void(QDataStream &, const void *)>(
                                                                                   [_metaType_id](QDataStream &s, const void * ptr){
                                                                                        const QListData* p = reinterpret_cast<const QListData*>(ptr);
                                                                                        s << quint32(p->size());
                                                                                        for(int i=0; i<p->size(); ++i){
                                                                                            void* element = p->at(i);
                                                                                            element = reinterpret_cast<Node *>(element)->v;
                                                                                            QMetaType::save(s, _metaType_id, element);
                                                                                        }
                                                                                   }, _metaType_id),
                                                   qtjambi_function_pointer<16,void(QDataStream &, void *)>(
                                                                                   [access](QDataStream &s, void * ptr){
                                                                                       QListData* p = reinterpret_cast<QListData*>(ptr);
                                                                                       quint32 size = 0;
                                                                                       s >> size;
                                                                                       access->clear(nullptr, ptr);
                                                                                       access->reserve(nullptr, ptr, size);
                                                                                       for(quint32 i=0; i<size; ++i){
                                                                                           Node * element = reinterpret_cast<Node *>(p->append());
                                                                                           element->v = QMetaType::create(access->m_elementMetaType.id(), nullptr);
                                                                                           if(!QMetaType::load(s, access->m_elementMetaType.id(), element->v)){
                                                                                               // should clear and break
                                                                                           }
                                                                                       }
                                                                                       p->size();
                                                                                   }, _metaType_id)
                                                   );
            }else{
                QMetaType::registerStreamOperators(newMetaType,
                                                   qtjambi_function_pointer<16,void(QDataStream &, const void *)>(
                                                                                   [_metaType_id](QDataStream &s, const void * ptr){
                                                                                        const QListData* p = reinterpret_cast<const QListData*>(ptr);
                                                                                        s << quint32(p->size());
                                                                                        for(int i=0; i<p->size(); ++i){
                                                                                            void* element = p->at(i);
                                                                                            QMetaType::save(s, _metaType_id, element);
                                                                                        }
                                                                                   }, _metaType_id),
                                                   qtjambi_function_pointer<16,void(QDataStream &, void *)>(
                                                                                   [access](QDataStream &s, void * ptr){
                                                                                       QListData* p = reinterpret_cast<QListData*>(ptr);
                                                                                       quint32 size = 0;
                                                                                       s >> size;
                                                                                       access->clear(nullptr, ptr);
                                                                                       access->reserve(nullptr, ptr, size);
                                                                                       for(quint32 i=0; i<size; ++i){
                                                                                            void* element = p->append();
                                                                                            QMetaType::construct(access->m_elementMetaType.id(), element, nullptr);
                                                                                            if(!QMetaType::load(s, access->m_elementMetaType.id(), element)){
                                                                                                // should clear and break
                                                                                            }
                                                                                       }
                                                                                   }, _metaType_id)
                                                   );
            }
        }
#else
        {
            const QMetaType to = QMetaType::fromType<QSequentialIterable>();
            QMetaType::registerMutableViewFunction([newMetaType](void *src, void *target)->bool{
                new(target) QIterable<QMetaSequence>(QMetaSequence(createMetaSequenceInterface(newMetaType)), reinterpret_cast<void **>(src));
                return true;
            }, QMetaType(newMetaType), to);
            QMetaType::registerConverterFunction([newMetaType](const void *src, void *target)->bool{
                new(target) QIterable<QMetaSequence>(QMetaSequence(createMetaSequenceInterface(newMetaType)), reinterpret_cast<void const*const*>(src));
                return true;
            }, QMetaType(newMetaType), to);
        }
        {
            QWriteLocker locker(gLock());
            gAccessHash->insert(newMetaType, access);
        }
#endif
    }
    return newMetaType;
}

jobject AutoListAccess::createIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr)
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    AutoIteratorAccess* containerAccess = new AutoIteratorAccess(m_internalToExternalConverter,
                                                 [](void*ptr){
                                                    void** cursor = *reinterpret_cast<void***>(ptr);
                                                    *reinterpret_cast<void***>(ptr) = cursor+1;
                                                 },
                                                 [](void*ptr){
                                                    void** cursor = *reinterpret_cast<void***>(ptr);
                                                    *reinterpret_cast<void***>(ptr) = cursor-1;
                                                 },
                                                 m_isLargeOrStaticType
                                                 ? std::function<const void*(void*)>([](void*ptr)->const void*{
                                                      return (*reinterpret_cast<Node**>(ptr))->v;
                                                   })
                                                 : std::function<const void*(void*)>([](void*ptr)->const void*{
                                                      return *reinterpret_cast<void***>(ptr);
                                                    }),
                                                 [](void*ptr1,void*ptr2)->bool{
                                                    return *reinterpret_cast<void***>(ptr1)<*reinterpret_cast<void***>(ptr2);
                                                 },
                                                 [](void*ptr1,void*ptr2)->bool{
                                                    return *reinterpret_cast<void***>(ptr1)==*reinterpret_cast<void***>(ptr2);
                                                 }
                                            );
#else
    size_t offset = m_offset;
    AutoIteratorAccess* containerAccess = new AutoIteratorAccess(m_internalToExternalConverter,
                                                                 [offset](void*ptr){
                                                                    char* cursor = *reinterpret_cast<char**>(ptr);
                                                                    *reinterpret_cast<char**>(ptr) = cursor+offset;
                                                                 },
                                                                 [offset](void*ptr){
                                                                    char* cursor = *reinterpret_cast<char**>(ptr);
                                                                    *reinterpret_cast<char**>(ptr) = cursor-offset;
                                                                 },
                                                                 [](void*ptr)->const void*{
                                                                    return *reinterpret_cast<char**>(ptr);
                                                                 },
                                                                 [](void*ptr1,void*ptr2)->bool{
                                                                    return *reinterpret_cast<char**>(ptr1)<*reinterpret_cast<char**>(ptr2);
                                                                 },
                                                                 [](void*ptr1,void*ptr2)->bool{
                                                                    return *reinterpret_cast<char**>(ptr1)==*reinterpret_cast<char**>(ptr2);
                                                                 }
                                                                );
#endif
    return qtjambi_from_QIterator(env, ownerId, iteratorPtr, [](void* ptr,bool){
        delete reinterpret_cast<void**>(ptr);
    }, containerAccess);
}

jobject AutoListAccess::end(JNIEnv * env, QtJambiNativeID ownerId, const void* container)
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    const QListData* p = reinterpret_cast<const QListData*>(container);
    void* iteratorPtr = new void*(p->end());
#else
    const QArrayDataPointer<char>* p = reinterpret_cast<const QArrayDataPointer<char>*>(container);
    void* iteratorPtr = new void*(reinterpret_cast<char*>(p->ptr + p->size * m_offset));
#endif
    return createIterator(env, ownerId, iteratorPtr);
}

jobject AutoListAccess::begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container)
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    const QListData* p = reinterpret_cast<const QListData*>(container);
    void* iteratorPtr = new void*(p->begin());
#else
    const QArrayDataPointer<char>* p = reinterpret_cast<const QArrayDataPointer<char>*>(container);
    void* iteratorPtr = new void*(p->ptr);
#endif
    return createIterator(env, ownerId, iteratorPtr);
}

void AutoListAccess::append(JNIEnv * env, void* container, jobject value)
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QListData* p = reinterpret_cast<QListData*>(container);
    if (p->d->ref.isShared()) {
        Node *n = detach_helper_grow(p, INT_MAX, 1);
        QT_TRY {
            node_construct(n, env, value);
        } QT_CATCH(...) {
            --p->d->end;
            QT_RETHROW;
        }
    } else {
        if (m_isLargeOrStaticType) {
            Node *n = reinterpret_cast<Node *>(p->append());
            QT_TRY {
                node_construct(n, env, value);
            } QT_CATCH(...) {
                --p->d->end;
                QT_RETHROW;
            }
        } else {
            Node *n, copy;
            node_construct(&copy, env, value); // t might be a reference to an object in the array
            QT_TRY {
                n = reinterpret_cast<Node *>(p->append());
            } QT_CATCH(...) {
                node_destruct(&copy);
                QT_RETHROW;
            }
            *n = copy;
        }
    }
#else
    QArrayDataPointer<char>* p = reinterpret_cast<QArrayDataPointer<char>*>(container);
    emplace(p, env, p->size, value, 1);
#endif
}

void AutoListAccess::appendList(JNIEnv * env, void* container, jobject list)
{
    if (qtjambi_is_QList(env, list, elementMetaType())) {
        if(void* ptr = qtjambi_to_object(env, list)){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            QListData* p = reinterpret_cast<QListData*>(container);
            QListData* p2 = reinterpret_cast<QListData*>(ptr);
            if (!p2->isEmpty()) {
                if (p->d == &QListData::shared_null) {
                    p->d = p2->d;
                    p->d->ref.ref();
                } else {
                    Node *n = (p->d->ref.isShared())
                              ? detach_helper_grow(p, INT_MAX, p2->size())
                              : reinterpret_cast<Node *>(p->append(*p2));
                    QT_TRY {
                        node_copy(n, reinterpret_cast<Node *>(p->end()),
                                  reinterpret_cast<Node *>(p2->begin()));
                    } QT_CATCH(...) {
                        // restore the old end
                        p->d->end -= int(reinterpret_cast<Node *>(p->end()) - n);
                        QT_RETHROW;
                    }
                }
            }
#else
            QArrayDataPointer<char>* p = reinterpret_cast<QArrayDataPointer<char>*>(container);
            QArrayDataPointer<char>* p2 = reinterpret_cast<QArrayDataPointer<char>*>(ptr);
            detachAndGrow(p, QArrayData::GrowsAtEnd, p2->size, nullptr, nullptr);
            Q_ASSERT(freeSpaceAtEnd(p) >= p2->size);
            void *where = createHole(p, QArrayData::GrowsAtEnd, p->size, p2->size-1);
            for(size_t i=0; i<size_t(p2->size); ++i){
                m_elementMetaType.construct(reinterpret_cast<char*>(where) + i * m_offset, p2->ptr + i * m_offset);
            }
#endif
        }
    }else{
        jobject iter = qtjambi_collection_iterator(env, list);
        while(qtjambi_iterator_has_next(env, iter)){
            append(env, container, qtjambi_iterator_next(env, iter));
        }
    }
}

jobject AutoListAccess::at(JNIEnv * env, const void* container, jint index)
{
    jvalue _value;
    _value.l = nullptr;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    const QListData* p = reinterpret_cast<const QListData*>(container);
    Q_ASSERT_X(index >= 0 && index < p->size(), "QList<T>::at", "index out of range");
    void** v = p->at(index);
    if(m_isLargeOrStaticType){
        if(m_internalToExternalConverter(env, nullptr, reinterpret_cast<Node*>(v)->v, &_value, true)){
            return _value.l;
        }
    }else{
        if(m_internalToExternalConverter(env, nullptr, v, &_value, true)){
            return _value.l;
        }
    }
#else
    const QArrayDataPointer<char>* p = reinterpret_cast<const QArrayDataPointer<char>*>(container);
    Q_ASSERT_X(index >= 0 && index < p->size, "QList<T>::at", "index out of range");
    void* ptr = p->ptr+index*m_offset;
    if(m_internalToExternalConverter(env, nullptr, ptr, &_value, true)){
        return _value.l;
    }
#endif
    return nullptr;
}

jobject AutoListAccess::value(JNIEnv * env, const void* container, jint index)
{
    jvalue _value;
    _value.l = nullptr;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    const QListData* p = reinterpret_cast<const QListData*>(container);
    if(index >= 0 && index < p->size()){
        void** v = p->at(index);
        if(m_isLargeOrStaticType){
            if(m_internalToExternalConverter(env, nullptr, reinterpret_cast<Node*>(v)->v, &_value, true)){
                return _value.l;
            }
        }else{
            if(m_internalToExternalConverter(env, nullptr, v, &_value, true)){
                return _value.l;
            }
        }
#else
    const QArrayDataPointer<char>* p = reinterpret_cast<const QArrayDataPointer<char>*>(container);
    if(index >= 0 && index < p->size){
        void* ptr = p->ptr+index*m_offset;
        if(m_internalToExternalConverter(env, nullptr, ptr, &_value, true)){
            return _value.l;
        }
#endif
    }else{
        void* ptr = m_elementMetaType.create();
        bool success = m_internalToExternalConverter(env, nullptr, ptr, &_value, true);
        m_elementMetaType.destroy(ptr);
        if(success)
            return _value.l;
    }
    return nullptr;
}

jobject AutoListAccess::value(JNIEnv * env, const void* container, jint index, jobject defaultValue)
{
    jvalue _value;
    _value.l = nullptr;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    const QListData* p = reinterpret_cast<const QListData*>(container);
    if(index >= 0 && index < p->size()){
        void** v = p->at(index);
        if(m_isLargeOrStaticType){
            if(m_internalToExternalConverter(env, nullptr, reinterpret_cast<Node*>(v)->v, &_value, true)){
                return _value.l;
            }
        }else{
            if(m_internalToExternalConverter(env, nullptr, v, &_value, true)){
                return _value.l;
            }
        }
#else
    const QArrayDataPointer<char>* p = reinterpret_cast<const QArrayDataPointer<char>*>(container);
    if(index >= 0 && index < p->size){
        void* ptr = p->ptr+index*m_offset;
        if(m_internalToExternalConverter(env, nullptr, ptr, &_value, true)){
            return _value.l;
        }
#endif
    }else{
        return defaultValue;
    }
    return nullptr;
}

void AutoListAccess::swapItemsAt(JNIEnv *, void* container, jint i, jint j)
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QListData* p = reinterpret_cast<QListData*>(container);
    Q_ASSERT_X(i >= 0 && i < p->size() && j >= 0 && j < p->size(),
                "QList<T>::swap", "index out of range");
    detach(p);
    qSwap(p->d->array[p->d->begin + i], p->d->array[p->d->begin + j]);
#else
    QArrayDataPointer<char>* p = reinterpret_cast<QArrayDataPointer<char>*>(container);
    void* element1 = p->ptr + i * m_offset;
    void* element2 = p->ptr + j * m_offset;
    if(m_elementMetaType.iface()->moveCtr){
        void* tmp = m_elementMetaType.create(element1);
        m_elementMetaType.destruct(element1);
        m_elementMetaType.iface()->moveCtr(m_elementMetaType.iface(), element1, element2);
        m_elementMetaType.destruct(element2);
        m_elementMetaType.iface()->moveCtr(m_elementMetaType.iface(), element2, tmp);
        m_elementMetaType.destroy(tmp);
    }else{
        void* tmp = m_elementMetaType.create(element1);
        m_elementMetaType.destruct(element1);
        m_elementMetaType.construct(element1, element2);
        m_elementMetaType.destruct(element2);
        m_elementMetaType.construct(element2, tmp);
        m_elementMetaType.destroy(tmp);
    }
#endif
}

jboolean AutoListAccess::startsWith(JNIEnv * env, const void* container, jobject value)
{
    auto sz = size(env, container);
    if(sz>0){
        QtJambiScope scope;
        void *_qvaluePtr = nullptr;
        jvalue _value;
        _value.l = value;
        if(m_externalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            const QListData* p = reinterpret_cast<const QListData*>(container);
            void** v = p->at(0);
            if(m_isLargeOrStaticType){
                return is_equals(m_elementMetaType, _qvaluePtr, reinterpret_cast<Node*>(v)->v);
            }else{
                return is_equals(m_elementMetaType, _qvaluePtr, v);
            }
#else
            const QArrayDataPointer<char>* p = reinterpret_cast<const QArrayDataPointer<char>*>(container);
            return m_elementMetaType.equals(_qvaluePtr, p->ptr);
#endif
        }
    }
    return false;
}

jint AutoListAccess::size(JNIEnv *, const void* container)
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    const QListData* p = reinterpret_cast<const QListData*>(container);
    return p->size();
#else
    const QArrayDataPointer<char>* p = reinterpret_cast<const QArrayDataPointer<char>*>(container);
    return p->size;
#endif
}

void AutoListAccess::reserve(JNIEnv *, void* container, jint asize)
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QListData* p = reinterpret_cast<QListData*>(container);
    if (p->d->alloc < asize) {
        if (p->d->ref.isShared())
            detach_helper(p, asize);
        else
            p->realloc(asize);
    }
#else
    QArrayDataPointer<char>* p = reinterpret_cast<QArrayDataPointer<char>*>(container);
    // capacity() == 0 for immutable data, so this will force a detaching below
    if (asize <= p->constAllocatedCapacity() - freeSpaceAtBegin(p)) {
        if (p->flags() & QArrayData::CapacityReserved)
            return;  // already reserved, don't shrink
        if (!p->isShared()) {
            // accept current allocation, don't shrink
            p->setFlag(QArrayData::CapacityReserved);
            return;
        }
    }

    auto newSize = qMax(asize, p->size);
    QArrayDataPointer<char> detached(allocate(newSize));
    for(qsizetype i = 0; i<p->size; ++i){
        void* source = p->ptr + i * m_offset;
        void* target = detached.ptr + i * m_offset;
        m_elementMetaType.construct(target, source);
        ++detached.size;
    }
    if (detached->ptr)
        detached.setFlag(QArrayData::CapacityReserved);
    swapAndDestroy(p, std::move(detached));
#endif
}


void AutoListAccess::replace(JNIEnv * env, void* container, jint index, jobject value)
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        QListData* p = reinterpret_cast<QListData*>(container);
        Q_ASSERT_X(index >= 0 && index < p->size(), "QList<T>::replace", "index out of range");
        detach(p);
        void* target;
        if(m_isLargeOrStaticType){
            target = reinterpret_cast<Node *>(p->at(index))->v;
        }else{
            target = p->at(index);
        }
#else
        QArrayDataPointer<char>* p = reinterpret_cast<QArrayDataPointer<char>*>(container);
        Q_ASSERT_X(index >= 0 && index < p->size, "QList<T>::replace", "index out of range");
        detach(p);
        void* target = p->ptr + index * m_offset;
#endif
        jvalue _value;
        _value.l = value;
        //void *_qvaluePtr;
        if(m_externalToInternalConverter(env, nullptr, _value, target, jValueType::l)){
            //m_elementMetaType.destruct(target);
            //m_elementMetaType.construct(target, _qvaluePtr);
        }
}

jboolean AutoListAccess::removeOne(JNIEnv * env, void* container, jobject value)
{
    jint index = indexOf(env, container, value, 0);
    if(index>=0)
        removeAt(env, container, index);
    return index>=0;
}

void AutoListAccess::removeAt(JNIEnv *, void* container, jint index)
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QListData* p = reinterpret_cast<QListData*>(container);
    if (index < 0 || index >= p->size()) {
#if !defined(QT_NO_DEBUG)
        qWarning("QList::removeAt(): Index out of range.");
#endif
        return;
    }
    detach(p);
    node_destruct(reinterpret_cast<Node *>(p->at(index))); p->remove(index);
#else
    QArrayDataPointer<char>* p = reinterpret_cast<QArrayDataPointer<char>*>(container);
    detach(p);
    m_elementMetaType.destruct(p->ptr + index * m_offset);
    if(index>p->size/2.){
        if(auto moveCtr = m_elementMetaType.iface()->moveCtr){
            for(qsizetype i = index; i<p->size; ++i){
                void* target = p->ptr + i * m_offset;
                void* source = p->ptr + (i+1) * m_offset;
                moveCtr(m_elementMetaType.iface(), target, source);
                m_elementMetaType.destruct(source);
            }
        }else{
            for(qsizetype i = index; i<p->size; ++i){
                void* target = p->ptr + i * m_offset;
                void* source = p->ptr + (i+1) * m_offset;
                m_elementMetaType.construct(target, source);
                m_elementMetaType.destruct(source);
            }
        }
    }else{
        if(auto moveCtr = m_elementMetaType.iface()->moveCtr){
            for(qsizetype i = index; i>0; --i){
                void* target = p->ptr + i * m_offset;
                void* source = p->ptr + (i-1) * m_offset;
                moveCtr(m_elementMetaType.iface(), target, source);
                m_elementMetaType.destruct(source);
            }
        }else{
            for(qsizetype i = index; i>0; --i){
                void* target = p->ptr + i * m_offset;
                void* source = p->ptr + (i-1) * m_offset;
                m_elementMetaType.construct(target, source);
                m_elementMetaType.destruct(source);
            }
        }
        p->ptr += m_offset;
    }
    --p->size;
#endif
}

jint AutoListAccess::removeAll(JNIEnv * env, void* container, jobject value)
{
    QtJambiScope scope;
    jint removedCount = 0;
    void *_qvaluePtr = nullptr;
    jvalue _value;
    _value.l = value;
    if(m_externalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        const QListData* p = reinterpret_cast<const QListData*>(container);
        int index;
        do{
            index = -1;
            for(int i = 0; i<p->size(); ++i){
                void* element;
                if(m_isLargeOrStaticType){
                    element = reinterpret_cast<Node *>(p->at(i))->v;
                }else{
                    element = p->at(i);
                }
                if(is_equals(m_elementMetaType, _qvaluePtr, element)){
                    removeAt(env, container, i);
                    ++removedCount;
                    index = i;
                    break;
                }
            }
        }while(index>=0);
#else
        const QArrayDataPointer<char>* p = reinterpret_cast<const QArrayDataPointer<char>*>(container);
        qsizetype index;
        do{
            index = -1;
            for(qsizetype i = 0; i<p->size; ++i){
                void* element = p->ptr + i * m_offset;
                if(m_elementMetaType.equals(_qvaluePtr, element)){
                    removeAt(env, container, i);
                    ++removedCount;
                    index = i;
                    break;
                }
            }
        }while(index>=0);
#endif
    }
    return removedCount;
}

void AutoListAccess::prepend(JNIEnv * env, void* container, jobject value)
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QListData* p = reinterpret_cast<QListData*>(container);
    if (p->d->ref.isShared()) {
        Node *n = detach_helper_grow(p, 0, 1);
        QT_TRY {
            node_construct(n, env, value);
        } QT_CATCH(...) {
            ++p->d->begin;
            QT_RETHROW;
        }
    } else {
        if (m_isLargeOrStaticType) {
            Node *n = reinterpret_cast<Node *>(p->prepend());
            QT_TRY {
                node_construct(n, env, value);
            } QT_CATCH(...) {
                ++p->d->begin;
                QT_RETHROW;
            }
        } else {
            Node *n, copy;
            node_construct(&copy, env, value); // t might be a reference to an object in the array
            QT_TRY {
                n = reinterpret_cast<Node *>(p->prepend());
            } QT_CATCH(...) {
                node_destruct(&copy);
                QT_RETHROW;
            }
            *n = copy;
        }
    }
#else
    QArrayDataPointer<char>* p = reinterpret_cast<QArrayDataPointer<char>*>(container);
    emplace(p, env, 0, value, 1);
#endif
}

jboolean AutoListAccess::equal(JNIEnv * env, const void* container, jobject other)
{
    if (qtjambi_is_QList(env, other, elementMetaType())) {
        if(void* ptr = qtjambi_to_object(env, other)){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            const QListData* p = reinterpret_cast<const QListData*>(container);
            const QListData* p2 = reinterpret_cast<const QListData*>(ptr);
            if(p->size()!=p2->size())
                return false;
            if(p->d==p2->d)
                return true;
            for(int i = 0; i<p->size(); ++i){
                void* element1 = p->at(i);
                void* element2 = p2->at(i);
                if (m_isLargeOrStaticType) {
                    element1 = reinterpret_cast<Node *>(element1)->v;
                    element2 = reinterpret_cast<Node *>(element2)->v;
                }
                if(!is_equals(m_elementMetaType, element1, element2))
                    return false;
            }
            return true;
#else
            const QArrayDataPointer<char>* p = reinterpret_cast<const QArrayDataPointer<char>*>(container);
            const QArrayDataPointer<char>* p2 = reinterpret_cast<const QArrayDataPointer<char>*>(ptr);
            if(p->size!=p2->size)
                return false;
            if(p->ptr==p2->ptr)
                return true;
            for(qsizetype i = 0; i<p->size; ++i){
                void* element1 = p->ptr + i * m_offset;
                void* element2 = p2->ptr + i * m_offset;
                if(!m_elementMetaType.equals(element1, element2))
                    return false;
            }
            return true;
#endif
        }
    }else{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        const QListData* p = reinterpret_cast<const QListData*>(container);
        if(p->size()!=qtjambi_collection_size(env, other))
            return false;
        jobject iterator = qtjambi_collection_iterator(env, other);
        int i = 0;
        QtJambiScope scope;
        jvalue _value;
        void *_qvaluePtr;
        while(qtjambi_iterator_has_next(env, iterator)){
            _value.l = qtjambi_iterator_next(env, iterator);
            _qvaluePtr = nullptr;
            if(!m_externalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l))
                return false;
            void* element = p->at(i++);
            if (m_isLargeOrStaticType) {
                element = reinterpret_cast<Node *>(element)->v;
            }
            if(!is_equals(m_elementMetaType, element, _qvaluePtr))
                return false;
        }
        return true;
#else
        const QArrayDataPointer<char>* p = reinterpret_cast<const QArrayDataPointer<char>*>(container);
        if(p->size!=qtjambi_collection_size(env, other))
            return false;
        jobject iterator = qtjambi_collection_iterator(env, other);
        size_t i = 0;
        while(qtjambi_iterator_has_next(env, iterator)){
            QtJambiScope scope;
            jvalue _value;
            _value.l = qtjambi_iterator_next(env, iterator);
            void *_qvaluePtr = nullptr;
            if(!m_externalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l))
                return false;
            void* element = p->ptr + (i++) * m_offset;
            if(!m_elementMetaType.equals(element, _qvaluePtr))
                return false;
        }
        return true;
#endif
    }
    return false;
}

void AutoListAccess::move(JNIEnv *, void* container, jint from, jint to)
{
    if (from == to)
        return;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QListData* p = reinterpret_cast<QListData*>(container);
    Q_ASSERT_X(from >= 0 && from < p->size() && to >= 0 && to < p->size(),
               "QList<T>::move", "index out of range");
    detach(p);
    p->move(from, to);
#else
    QArrayDataPointer<char>* p = reinterpret_cast<QArrayDataPointer<char>*>(container);
    detach(p);
    void* tmp = m_elementMetaType.create(p->ptr + from * m_offset);
    for(jint i=from; i<to; ++i){
        m_elementMetaType.destruct(p->ptr + i * m_offset);
        m_elementMetaType.construct(p->ptr + i * m_offset, p->ptr + (i+1) * m_offset);
    }
    m_elementMetaType.destruct(p->ptr + to * m_offset);
    m_elementMetaType.construct(p->ptr + to * m_offset, tmp);
    m_elementMetaType.destroy(tmp);
#endif
}

jobject AutoListAccess::mid(JNIEnv * env, const void* container, jint _pos, jint _len)
{
    using namespace QtPrivate;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    int pos = _pos;
    int alength = _len;
    const QListData* p = reinterpret_cast<const QListData*>(container);
    switch (QContainerImplHelper::mid(p->size(), &pos, &alength)) {
    case QContainerImplHelper::Null:
    case QContainerImplHelper::Empty:
        return qtjambi_from_QList(env, createContainer(), clone());
    case QContainerImplHelper::Full:
        return qtjambi_from_QList(env, copyContainer(container), this);
    case QContainerImplHelper::Subset:
        break;
    }
    if (alength <= 0)
        return qtjambi_from_QList(env, createContainer(), clone());

    QListData* cpy = new QListData();
    cpy->d = const_cast<QListData::Data *>(&QListData::shared_null);
    cpy->detach(alength);
    cpy->d->end = alength;
    QT_TRY {
        node_copy(reinterpret_cast<Node *>(cpy->begin()),
                      reinterpret_cast<Node *>(cpy->end()),
                      reinterpret_cast<Node *>(p->begin() + pos));
    } QT_CATCH(...) {
        // restore the old end
        cpy->d->end = 0;
        QT_RETHROW;
    }
    return qtjambi_from_QList(env, cpy, clone());
#else
    const QArrayDataPointer<char>* p = reinterpret_cast<const QArrayDataPointer<char>*>(container);
    qsizetype pos = _pos;
    qsizetype len = _len;
    switch (QContainerImplHelper::mid(p->size, &pos, &len)) {
    case QContainerImplHelper::Null:
    case QContainerImplHelper::Empty:
        return qtjambi_from_QList(env, createContainer(), clone());
    case QContainerImplHelper::Full:
        return qtjambi_from_QList(env, copyContainer(container), this);
    case QContainerImplHelper::Subset:
        break;
    }
    QArrayDataPointer<char>* newList = new QArrayDataPointer<char>(allocate(len));
    for(qsizetype i = 0; i<len; ++i){
        void* source = p->ptr + (pos+i) * m_offset;
        void* target = newList->ptr + i * m_offset;
        m_elementMetaType.construct(target, source);
        ++newList->size;
    }
    return qtjambi_from_QList(env, newList, clone());
#endif
}

jint AutoListAccess::lastIndexOf(JNIEnv * env, const void* container, jobject value, jint index)
{
    jint sz = size(env, container);
    if (index < 0)
        index += sz;
    else if (index >= sz)
        index = sz - 1;
    if (index >= 0) {
        QtJambiScope scope;
        void *_qvaluePtr = nullptr;
        jvalue _value;
        _value.l = value;
        if(m_externalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            const QListData* p = reinterpret_cast<const QListData*>(container);
            for(int i = index; i>=0; --i){
                void** v = p->at(i);
                if(m_isLargeOrStaticType){
                    if(is_equals(m_elementMetaType, _qvaluePtr, reinterpret_cast<Node*>(v)->v))
                        return i;
                }else{
                    if(is_equals(m_elementMetaType, _qvaluePtr, v))
                        return i;
                }
            }
#else
            const QArrayDataPointer<char>* p = reinterpret_cast<const QArrayDataPointer<char>*>(container);
            for(qsizetype i = qsizetype(index-1); i>=0; --i){
                void* element = p->ptr + i * m_offset;
                if(m_elementMetaType.equals(_qvaluePtr, element))
                    return i;
            }
#endif
        }
    }
    return -1;
}

void AutoListAccess::insert(JNIEnv * env, void* container, jint index, jobject value)
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QListData* p = reinterpret_cast<QListData*>(container);
    if (p->d->ref.isShared()) {
        Node *n = detach_helper_grow(p, index, 1);
        QT_TRY {
            node_construct(n, env, value);
        } QT_CATCH(...) {
            p->remove(index);
            QT_RETHROW;
        }
    } else {
        if (m_isLargeOrStaticType) {
            Node *n = reinterpret_cast<Node *>(p->insert(index));
            QT_TRY {
                node_construct(n, env, value);
            } QT_CATCH(...) {
                p->remove(index);
                QT_RETHROW;
            }
        } else {
            Node *n, copy;
            node_construct(&copy, env, value); // t might be a reference to an object in the array
            QT_TRY {
                n = reinterpret_cast<Node *>(p->insert(index));
            } QT_CATCH(...) {
                node_destruct(&copy);
                QT_RETHROW;
            }
            *n = copy;
        }
    }
#else
    QArrayDataPointer<char>* p = reinterpret_cast<QArrayDataPointer<char>*>(container);
    emplace(p, env, index, value, 1);
#endif
}

jint AutoListAccess::indexOf(JNIEnv * env, const void* container, jobject value, jint index)
{
    jint sz = size(env, container);
    if (index < 0)
        index = qMax<jint>(index + sz, 0);
    if (index < sz) {
        QtJambiScope scope;
        void *_qvaluePtr = nullptr;
        jvalue _value;
        _value.l = value;
        if(m_externalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            const QListData* p = reinterpret_cast<const QListData*>(container);
            for(int i = index; i<sz; ++i){
                void** v = p->at(i);
                if(m_isLargeOrStaticType){
                    if(is_equals(m_elementMetaType, _qvaluePtr, reinterpret_cast<Node*>(v)->v))
                        return i;
                }else{
                    if(is_equals(m_elementMetaType, _qvaluePtr, v))
                        return i;
                }
            }
#else
            const QArrayDataPointer<char>* p = reinterpret_cast<const QArrayDataPointer<char>*>(container);
            for(qsizetype i = qsizetype(index); i<sz; ++i){
                void* element = p->ptr + i * m_offset;
                if(m_elementMetaType.equals(_qvaluePtr, element))
                    return i;
            }
#endif
        }
    }
    return -1;
}

jboolean AutoListAccess::endsWith(JNIEnv * env, const void* container, jobject value)
{
    auto sz = size(env, container);
    if(sz>0){
        QtJambiScope scope;
        void *_qvaluePtr = nullptr;
        jvalue _value;
        _value.l = value;
        if(m_externalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            const QListData* p = reinterpret_cast<const QListData*>(container);
            void** v = p->at(sz-1);
            if(m_isLargeOrStaticType){
                return is_equals(m_elementMetaType, _qvaluePtr, reinterpret_cast<Node*>(v)->v);
            }else{
                return is_equals(m_elementMetaType, _qvaluePtr, v);
            }
#else
            const QArrayDataPointer<char>* p = reinterpret_cast<const QArrayDataPointer<char>*>(container);
            void* element = p->ptr + (sz-1) * m_offset;
            return m_elementMetaType.equals(_qvaluePtr, element);
#endif
        }
    }
    return false;
}

jint AutoListAccess::count(JNIEnv * env, const void* container, jobject value)
{
    jint result = 0;
    auto sz = size(env, container);
    QtJambiScope scope;
    void *_qvaluePtr = nullptr;
    jvalue _value;
    _value.l = value;
    if(m_externalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        const QListData* p = reinterpret_cast<const QListData*>(container);
        for(qsizetype i = 0; i<sz; ++i){
            void** v = p->at(i);
            if(m_isLargeOrStaticType){
                if(is_equals(m_elementMetaType, _qvaluePtr, reinterpret_cast<Node*>(v)->v))
                    ++result;
            }else{
                if(is_equals(m_elementMetaType, _qvaluePtr, v))
                    ++result;
            }
        }
#else
        const QArrayDataPointer<char>* p = reinterpret_cast<const QArrayDataPointer<char>*>(container);
        for(qsizetype i = 0; i<sz; ++i){
            void* element = p->ptr + i * m_offset;
            if(m_elementMetaType.equals(_qvaluePtr, element))
                ++result;
        }
#endif
    }
    return result;
}

jboolean AutoListAccess::contains(JNIEnv * env, const void* container, jobject value)
{
    auto sz = size(env, container);
    QtJambiScope scope;
    void *_qvaluePtr = nullptr;
    jvalue _value;
    _value.l = value;
    if(m_externalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        const QListData* p = reinterpret_cast<const QListData*>(container);
        for(qsizetype i = 0; i<sz; ++i){
            void** v = p->at(i);
            if(m_isLargeOrStaticType){
                if(is_equals(m_elementMetaType, _qvaluePtr, reinterpret_cast<Node*>(v)->v))
                    return true;
            }else{
                if(is_equals(m_elementMetaType, _qvaluePtr, v))
                    return true;
            }
        }
#else
        const QArrayDataPointer<char>* p = reinterpret_cast<const QArrayDataPointer<char>*>(container);
        for(qsizetype i = 0; i<sz; ++i){
            void* element = p->ptr + i * m_offset;
            if(m_elementMetaType.equals(_qvaluePtr, element))
                return true;
        }
#endif
    }
    return false;
}

void AutoListAccess::clear(JNIEnv *, void* container)
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QListData* p = reinterpret_cast<QListData*>(container);
    QListData::Data* tmp;
    tmp = p->d;
    p->d = const_cast<QListData::Data *>(&QListData::shared_null);
    if(!tmp->ref.deref()){
        QListData x{tmp};
        dealloc(&x);
    }
#else
    QArrayDataPointer<char>* p = reinterpret_cast<QArrayDataPointer<char>*>(container);
    if (!p->size)
        return;
    if (p->needsDetach()) {
        // must allocate memory
        QArrayDataPointer<char> detached(allocate(p->allocatedCapacity()));
        for(qsizetype i = 0; i<p->size; ++i){
            void* source = p->ptr + i * m_offset;
            void* target = detached.ptr + i * m_offset;
            m_elementMetaType.construct(target, source);
            ++detached.size;
        }
        swapAndDestroy(p, std::move(detached));
    } else {
        for(qsizetype i = 0; i<p->size; ++i){
            void* target = p->ptr + i * m_offset;
            m_elementMetaType.destruct(target);
        }
        p->size = 0;
        p->d = nullptr;
        p->ptr = nullptr;
    }
#endif
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
void AutoListAccess::emplace(QArrayDataPointer<char>* p, JNIEnv * env, jint index, jobject value, jint n)
{
    if(n<=0)
        return;
    jvalue _value;
    _value.l = value;
    bool detach = p->needsDetach();
    if (!detach) {
        if(index == p->size && freeSpaceAtEnd(p)>=n){
            for(jint i=0; i<n; ++i){
                void* _qvaluePtr = p->ptr+p->size*m_offset;
                m_elementMetaType.construct(_qvaluePtr);
                m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l);
                ++p->size;
            }
            return;
        }
        if (index == 0 && freeSpaceAtBegin(p)>=n) {
            p->ptr -= n*m_offset;
            for(jint i=0; i<n; ++i){
                void* _qvaluePtr = p->ptr+i*m_offset;
                m_elementMetaType.construct(_qvaluePtr);
                m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l);
                ++p->size;
            }
            return;
        }
    }
    const bool growsAtBegin = p->size != 0 && index == 0;
    QArrayData::GrowthPosition pos = growsAtBegin ? QArrayData::GrowsAtBeginning : QArrayData::GrowsAtEnd;
    detachAndGrow(p, pos, n, nullptr, nullptr);
    void *where = createHole(p, pos, index, n);
    for(size_t i=0; i<size_t(n); ++i){
        void *_where = reinterpret_cast<char*>(where) + i * m_offset;
        m_elementMetaType.construct(_where);
        m_externalToInternalConverter(env, nullptr, _value, _where, jValueType::l);
    }
}

void AutoListAccess::emplace(QArrayDataPointer<char>* p, qsizetype index, const void* value, qsizetype n)
{
    if(n<=0)
        return;
    bool detach = p->needsDetach();
    if (!detach) {
        if(index == p->size && freeSpaceAtEnd(p)>=n){
            for(qsizetype i=0; i<n; ++i){
                void* _qvaluePtr = p->ptr+p->size*m_offset;
                m_elementMetaType.construct(_qvaluePtr, value);
                ++p->size;
            }
            return;
        }
        if (index == 0 && freeSpaceAtBegin(p)>=n) {
            p->ptr -= n*m_offset;
            for(qsizetype i=0; i<n; ++i){
                void* _qvaluePtr = p->ptr+i*m_offset;
                m_elementMetaType.construct(_qvaluePtr, value);
                ++p->size;
            }
            return;
        }
    }
    const bool growsAtBegin = p->size != 0 && index == 0;
    QArrayData::GrowthPosition pos = growsAtBegin ? QArrayData::GrowsAtBeginning : QArrayData::GrowsAtEnd;
    detachAndGrow(p, pos, n, nullptr, nullptr);
    void *where = createHole(p, pos, index, n);
    for(size_t i=0; i<size_t(n); ++i){
        void *_where = reinterpret_cast<char*>(where) + i * m_offset;
        m_elementMetaType.construct(_where, value);
    }
}

void AutoListAccess::swapAndDestroy(QArrayDataPointer<char>* p, QArrayDataPointer<char>&& detached){
    p->swap(detached);
    if(!detached.deref()){
        for(qsizetype i = 0; i<detached.size; ++i){
            void* target = detached.ptr + i * m_offset;
            m_elementMetaType.destruct(target);
        }
        QArrayData::deallocate(detached.d, m_elementMetaType.sizeOf(), qMax<qsizetype>(m_elementMetaType.alignOf(), alignof(QArrayData)));
    }
    detached.d = nullptr;
    detached.size = 0;
    detached.ptr = nullptr;
}

qsizetype AutoListAccess::freeSpaceAtBegin(const QArrayDataPointer<char>* p){
    if (p->d == nullptr)
        return 0;
    qsizetype result = p->ptr - QTypedArrayData<char>::dataStart(p->d, qMax(size_t(m_elementMetaType.alignOf()), alignof(QArrayData)));
    return result / m_offset;
}

qsizetype AutoListAccess::freeSpaceAtEnd(const QArrayDataPointer<char>* p){
    if (p->d == nullptr)
        return 0;
    return p->d->constAllocatedCapacity() - freeSpaceAtBegin(p) - p->size;
}

void AutoListAccess::detachAndGrow(QArrayDataPointer<char>* p, QArrayData::GrowthPosition where, qsizetype n, const void **,
                   QArrayDataPointer<char> *old)
{
    if (!p->needsDetach()) {
        if (!n || (where == QArrayData::GrowsAtBeginning && freeSpaceAtBegin(p) >= n)
            || (where == QArrayData::GrowsAtEnd && freeSpaceAtEnd(p) >= n))
            return;
    }

    reallocateAndGrow(p, where, n, old);
}

QArrayDataPointer<char> AutoListAccess::allocateGrow(const QArrayDataPointer<char> &from, qsizetype n, QArrayData::GrowthPosition position)
{
    // calculate new capacity. We keep the free capacity at the side that does not have to grow
    // to avoid quadratic behavior with mixed append/prepend cases

    // use qMax below, because constAllocatedCapacity() can be 0 when using fromRawData()
    qsizetype minimalCapacity = qMax(from.size, from.constAllocatedCapacity()) + n;
    // subtract the free space at the side we want to allocate. This ensures that the total size requested is
    // the existing allocation at the other side + size + n.
    minimalCapacity -= (position == QArrayData::GrowsAtEnd) ? freeSpaceAtEnd(&from) : freeSpaceAtBegin(&from);
    qsizetype capacity = from.detachCapacity(minimalCapacity);
    const bool grows = capacity > from.constAllocatedCapacity();
    QArrayData *header;
    void *dataPtr = QArrayData::allocate(&header, m_elementMetaType.sizeOf(),
                                         qMax<size_t>(m_elementMetaType.alignOf(), alignof(QArrayData)),
                                         capacity,
                                         grows ? QArrayData::Grow : QArrayData::KeepSize);
#if !defined(Q_OS_ANDROID) && __has_builtin(__builtin_assume_aligned)
    switch(m_elementMetaType.alignOf()){
    case 1:
        dataPtr = __builtin_assume_aligned(dataPtr, qMax(size_t(1), Q_ALIGNOF(QArrayData)));
        break;
    case 2:
        dataPtr = __builtin_assume_aligned(dataPtr, qMax(size_t(2), Q_ALIGNOF(QArrayData)));
        break;
    case 3:
        dataPtr = __builtin_assume_aligned(dataPtr, qMax(size_t(3), Q_ALIGNOF(QArrayData)));
        break;
    case 4:
        dataPtr = __builtin_assume_aligned(dataPtr, qMax(size_t(4), Q_ALIGNOF(QArrayData)));
        break;
    case 8:
        dataPtr = __builtin_assume_aligned(dataPtr, qMax(size_t(8), Q_ALIGNOF(QArrayData)));
        break;
    }
#endif
    const bool valid = header != nullptr && dataPtr != nullptr;
    if (!valid)
        return QArrayDataPointer<char>(reinterpret_cast<QTypedArrayData<char>*>(header), reinterpret_cast<char*>(dataPtr));

    // Idea: * when growing backwards, adjust pointer to prepare free space at the beginning
    //       * when growing forward, adjust by the previous data pointer offset
    char* cdataPtr = reinterpret_cast<char*>(dataPtr);
    cdataPtr += ((position == QArrayData::GrowsAtBeginning)
            ? n + qMax(0, (header->alloc - from.size - n) / 2)
            : freeSpaceAtBegin(&from)) * m_offset;
    dataPtr = cdataPtr;
    header->flags = from.flags();
    return QArrayDataPointer<char>(reinterpret_cast<QTypedArrayData<char>*>(header), reinterpret_cast<char*>(dataPtr));
}

void AutoListAccess::reallocate(QArrayDataPointer<char>* p, qsizetype capacity, QArrayData::AllocationOption option){
    QPair<QArrayData *, void *> pair =
            QArrayData::reallocateUnaligned(p->d, p->ptr, m_elementMetaType.sizeOf(), capacity, option);
    Q_CHECK_PTR(pair.second);
    Q_ASSERT(pair.first != nullptr);
    p->d = reinterpret_cast<QTypedArrayData<char>*>(pair.first);
    p->ptr = reinterpret_cast<char*>(pair.second);
}

void AutoListAccess::reallocateAndGrow(QArrayDataPointer<char>* p, QArrayData::GrowthPosition where, qsizetype n, QArrayDataPointer<char> *old)
{
    if(m_elementMetaType.flags() & QMetaType::RelocatableType && size_t(m_elementMetaType.alignOf()) <= alignof(std::max_align_t)) {
        if (where == QArrayData::GrowsAtEnd && !old && !p->needsDetach() && n > 0) {
            reallocate(p, p->constAllocatedCapacity() - freeSpaceAtEnd(p) + n, QArrayData::Grow); // fast path
            return;
        }
    }

    QArrayDataPointer<char> dp(allocateGrow(*p, n, where));
    if (n > 0)
        Q_CHECK_PTR(dp.data());
    if (where == QArrayData::GrowsAtBeginning) {
        Q_ASSERT(freeSpaceAtBegin(&dp) >= n);
    } else {
        Q_ASSERT(freeSpaceAtEnd(&dp) >= n);
    }
    if (p->size) {
        size_t toCopy = p->size;
        if (n < 0)
            toCopy += n;
        char* ptr = reinterpret_cast<char*>(dp->ptr);
        if (p->needsDetach() || old){
            for(size_t i=0; i<toCopy; ++i){
                void* source = p->ptr+i*m_offset;
                void* target = ptr+i*m_offset;
                m_elementMetaType.construct(target, source);
                ++dp.size;
            }
        }else{
            if(m_elementMetaType.iface()->moveCtr){
                for(size_t i=0; i<toCopy; ++i){
                    void* source = p->ptr+i*m_offset;
                    void* target = ptr+i*m_offset;
                    m_elementMetaType.iface()->moveCtr(m_elementMetaType.iface(), target, source);
                    ++dp.size;
                }
            }else{
                for(size_t i=0; i<toCopy; ++i){
                    void* source = p->ptr+i*m_offset;
                    void* target = ptr+i*m_offset;
                    m_elementMetaType.construct(target, source);
                    ++dp.size;
                }
            }
        }
        Q_ASSERT(size_t(dp.size) == toCopy);
    }

    p->swap(dp);
    if (old)
        old->swap(dp);
    if(!dp.deref()){
        for(qsizetype i = 0; i<dp.size; ++i){
            void* target = dp.ptr + i * m_offset;
            m_elementMetaType.destruct(target);
        }
        QArrayData::deallocate(dp.d, m_elementMetaType.sizeOf(), qMax<qsizetype>(m_elementMetaType.alignOf(), alignof(QArrayData)));
    }
    dp.d = nullptr;
    dp.size = 0;
    dp.ptr = nullptr;
}

void AutoListAccess::detach(QArrayDataPointer<char>* p, QArrayDataPointer<char> *old)
{
    if(p->needsDetach())
        reallocateAndGrow(p, QArrayData::GrowsAtEnd, 0, old);
}

void *AutoListAccess::createHole(QArrayDataPointer<char> *p, QArrayData::GrowthPosition pos, qsizetype where, qsizetype n){
    Q_ASSERT((pos == QArrayData::GrowsAtBeginning && n <= freeSpaceAtBegin(p)) ||
             (pos == QArrayData::GrowsAtEnd && n <= freeSpaceAtEnd(p)));

    char *insertionPoint = reinterpret_cast<char*>(p->ptr) + where * m_offset;
    if (pos == QArrayData::GrowsAtEnd) {
        if (where < p->size)
            ::memmove(static_cast<void *>(insertionPoint + n * m_elementMetaType.sizeOf()), static_cast<void *>(insertionPoint), (p->size - where) * m_elementMetaType.sizeOf());
    } else {
        Q_ASSERT(where == 0);
        p->ptr = reinterpret_cast<char*>(p->ptr) - n * m_offset;
        insertionPoint -= n * m_offset;
    }
    p->size += n;
    return insertionPoint;
}

QArrayDataPointer<char> AutoListAccess::allocate(qsizetype capacity, QArrayData::AllocationOption option){
    QArrayData *header;
    void *dataPtr = QArrayData::allocate(&header, m_elementMetaType.sizeOf(),
                                         qMax<size_t>(m_elementMetaType.alignOf(), alignof(QArrayData)),
                                         capacity,
                                         option);
#if !defined(Q_OS_ANDROID) && __has_builtin(__builtin_assume_aligned)
    switch(m_elementMetaType.alignOf()){
    case 1:
        dataPtr = __builtin_assume_aligned(dataPtr, qMax(size_t(1), Q_ALIGNOF(QArrayData)));
        break;
    case 2:
        dataPtr = __builtin_assume_aligned(dataPtr, qMax(size_t(2), Q_ALIGNOF(QArrayData)));
        break;
    case 3:
        dataPtr = __builtin_assume_aligned(dataPtr, qMax(size_t(3), Q_ALIGNOF(QArrayData)));
        break;
    case 4:
        dataPtr = __builtin_assume_aligned(dataPtr, qMax(size_t(4), Q_ALIGNOF(QArrayData)));
        break;
    case 8:
        dataPtr = __builtin_assume_aligned(dataPtr, qMax(size_t(8), Q_ALIGNOF(QArrayData)));
        break;
    }
#endif
    return QArrayDataPointer<char>(reinterpret_cast<QTypedArrayData<char>*>(header), reinterpret_cast<char*>(dataPtr));
}

jint AutoListAccess::capacity(JNIEnv *, const void* container)
{
    const QArrayDataPointer<char>* p = reinterpret_cast<const QArrayDataPointer<char>*>(container);
    return qsizetype(p->constAllocatedCapacity());
}

void AutoListAccess::fill(JNIEnv * env, void* container, jobject value, jint newSize)
{
    QArrayDataPointer<char>* p = reinterpret_cast<QArrayDataPointer<char>*>(container);
    if (newSize == -1)
        newSize = p->size;
    jvalue _value;
    _value.l = value;
    if (p->needsDetach() || newSize > p->constAllocatedCapacity()) {
        // must allocate memory
        qsizetype sz = p->detachCapacity(newSize);
        QArrayDataPointer<char> detached(allocate(sz));
        for(qsizetype i = 0; i<sz; ++i){
            void* target = detached.ptr + i * m_offset;
            m_elementMetaType.construct(target);
            m_externalToInternalConverter(env, nullptr, _value, target, jValueType::l);
            ++detached.size;
        }
        swapAndDestroy(p, std::move(detached));
    } else {
        for(qsizetype i = 0; i<p->size; ++i){
            void* target = p->ptr + i * m_offset;
            m_elementMetaType.destruct(target);
        }
        for(qsizetype i = 0; i<newSize; ++i){
            void* target = p->ptr + i * m_offset;
            m_elementMetaType.construct(target);
            m_externalToInternalConverter(env, nullptr, _value, target, jValueType::l);
        }
        p->size = newSize;
    }
}

void AutoListAccess::remove(JNIEnv *, void* container, jint index, jint n)
{
    if(n==0)
        return;
    QArrayDataPointer<char>* p = reinterpret_cast<QArrayDataPointer<char>*>(container);
    detach(p);
    qsizetype nmb = qMin<qsizetype>(n, p->size-index);
    for(qsizetype i = 0; i<nmb; ++i){
        m_elementMetaType.destruct(p->ptr + (index+i) * m_offset);
    }
    if(index>p->size/2.){
        if(auto moveCtr = m_elementMetaType.iface()->moveCtr){
            for(qsizetype i = index; i<p->size; ++i){
                void* target = p->ptr + i * m_offset;
                void* source = p->ptr + (i+nmb) * m_offset;
                moveCtr(m_elementMetaType.iface(), target, source);
                m_elementMetaType.destruct(source);
            }
        }else{
            for(qsizetype i = index; i<p->size; ++i){
                void* target = p->ptr + i * m_offset;
                void* source = p->ptr + (i+nmb) * m_offset;
                m_elementMetaType.construct(target, source);
                m_elementMetaType.destruct(source);
            }
        }
    }else if(nmb!=p->size){
        if(auto moveCtr = m_elementMetaType.iface()->moveCtr){
            for(qsizetype i = index; i>0; --i){
                void* target = p->ptr + i * m_offset;
                void* source = p->ptr + (i-nmb) * m_offset;
                moveCtr(m_elementMetaType.iface(), target, source);
                m_elementMetaType.destruct(source);
            }
        }else{
            for(qsizetype i = index; i>0; --i){
                void* target = p->ptr + i * m_offset;
                void* source = p->ptr + (i-nmb) * m_offset;
                m_elementMetaType.construct(target, source);
                m_elementMetaType.destruct(source);
            }
        }
        p->ptr += nmb*m_offset;
    }
    p->size -= nmb;
}

void AutoListAccess::insert(JNIEnv * env, void* container, jint index, jint n, jobject value)
{
    QArrayDataPointer<char>* p = reinterpret_cast<QArrayDataPointer<char>*>(container);
    emplace(p, env, index, value, n);
}

void AutoListAccess::resize(JNIEnv * env, void* container, jint newSize)
{
    QArrayDataPointer<char>* p = reinterpret_cast<QArrayDataPointer<char>*>(container);
    if (p->needsDetach() || newSize > capacity(env, container) - freeSpaceAtBegin(p)) {
        detachAndGrow(p, QArrayData::GrowsAtEnd, newSize - p->size, nullptr, nullptr);
        if (newSize > p->size){
            for(size_t i=0; i<size_t(newSize); ++i){
                void* source = p->ptr+i*m_offset;
                m_elementMetaType.construct(source);
            }
            p->size = newSize;
        }
    } else if (newSize < p->size) {
        for(size_t i=p->size; i>=size_t(newSize); --i){
            void* source = p->ptr+(i-1)*m_offset;
            m_elementMetaType.destruct(source);
        }
        p->size = newSize;
    }
}

void AutoListAccess::squeeze(JNIEnv *env, void* container)
{
    QArrayDataPointer<char>* p = reinterpret_cast<QArrayDataPointer<char>*>(container);
    if (!p->isMutable())
        return;
    if (p->needsDetach() || p->size < capacity(env, container)) {
        // must allocate memory
        QArrayDataPointer<char> detached(allocate(p->size));
        if (p->size) {
            if (p->needsDetach() || !m_elementMetaType.iface()->moveCtr){
                for(qsizetype i = 0; i<p->size; ++i){
                    void* source = p->ptr + i * m_offset;
                    void* target = detached.ptr + i * m_offset;
                    m_elementMetaType.construct(target, source);
                    ++detached.size;
                }
            }else{
                for(qsizetype i = 0; i<p->size; ++i){
                    void* source = p->ptr + i * m_offset;
                    void* target = detached.ptr + i * m_offset;
                    m_elementMetaType.iface()->moveCtr(m_elementMetaType.iface(), target, source);
                    ++detached.size;
                }
            }
        }
        swapAndDestroy(p, std::move(detached));
    }
    // We're detached so this is fine
    p->clearFlag(QArrayData::CapacityReserved);
}
#else

void AutoListAccess::detach(QListData* p)
{
    if (p->d->ref.isShared())
        detach_helper(p, p->d->alloc);
}

void AutoListAccess::detach_helper(QListData* p)
{
    detach_helper(p, p->d->alloc);
}

void AutoListAccess::detach_helper(QListData* p, int alloc)
{
    Node *n = reinterpret_cast<Node *>(p->begin());
    QListData::Data *x = p->detach(alloc);
    QT_TRY {
        node_copy(reinterpret_cast<Node *>(p->begin()), reinterpret_cast<Node *>(p->end()), n);
    } QT_CATCH(...) {
        p->dispose();
        p->d = x;
        QT_RETHROW;
    }

    if (!x->ref.deref()){
        QListData px{x};
        dealloc(&px);
    }
}

void AutoListAccess::dealloc(QListData* p){
    if(m_isLargeOrStaticType){
        for(int i=0; i<p->size(); ++i){
            Node* v = reinterpret_cast<Node*>(p->at(i));
            m_elementMetaType.destruct(v->v);
        }
    }else{
        for(int i=0; i<p->size(); ++i){
            void** v = p->at(i);
            m_elementMetaType.destruct(v);
        }
    }
    QListData::dispose(p->d);
    p->d = const_cast<QListData::Data *>(&QListData::shared_null);
}

void AutoListAccess::node_copy(Node *from, Node *to, Node *src){
    Node *current = from;
    if (m_isLargeOrStaticType) {
        QT_TRY {
            while(current != to) {
                current->v = m_elementMetaType.create(src->v);
                ++current;
                ++src;
            }
        } QT_CATCH(...) {
            while (current-- != from)
                m_elementMetaType.destroy(current->v);
            QT_RETHROW;
        }

    } else {
        QT_TRY {
            while(current != to) {
                m_elementMetaType.construct(current, src);
                ++current;
                ++src;
            }
        } QT_CATCH(...) {
            while (current-- != from)
                m_elementMetaType.destruct(current);
            QT_RETHROW;
        }
    }
}

void AutoListAccess::node_destruct(Node *node){
    if (m_isLargeOrStaticType)
        m_elementMetaType.destroy(node->v);
    else
        m_elementMetaType.destruct(node);
}

void AutoListAccess::node_destruct(Node *from, Node *to){
    if (m_isLargeOrStaticType)
        while(from != to) --to, m_elementMetaType.destroy(to->v);
    else
        while (from != to) --to, m_elementMetaType.destruct(to);
}

void AutoListAccess::node_construct(Node *n, JNIEnv * env, jobject value){
    jvalue _value;
    _value.l = value;
    void* _qvaluePtr;
    if (m_isLargeOrStaticType) {
        n->v = m_elementMetaType.create();
        _qvaluePtr = n->v;
    }else{
        _qvaluePtr = n;
        m_elementMetaType.construct(_qvaluePtr);
    }
    m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l);
}

AutoListAccess::Node* AutoListAccess::detach_helper_grow(QListData* p, int i, int c)
{
    Node *n = reinterpret_cast<Node *>(p->begin());
    QListData::Data *x = p->detach_grow(&i, c);
    QT_TRY {
        node_copy(reinterpret_cast<Node *>(p->begin()),
                  reinterpret_cast<Node *>(p->begin() + i), n);
    } QT_CATCH(...) {
        p->dispose();
        p->d = x;
        QT_RETHROW;
    }
    QT_TRY {
        node_copy(reinterpret_cast<Node *>(p->begin() + i + c),
                  reinterpret_cast<Node *>(p->end()), n + i);
    } QT_CATCH(...) {
        node_destruct(reinterpret_cast<Node *>(p->begin()),
                      reinterpret_cast<Node *>(p->begin() + i));
        p->dispose();
        p->d = x;
        QT_RETHROW;
    }

    if (!x->ref.deref()){
        QListData px{x};
        dealloc(&px);
    }
    return reinterpret_cast<Node *>(p->begin() + i);
}

#endif

