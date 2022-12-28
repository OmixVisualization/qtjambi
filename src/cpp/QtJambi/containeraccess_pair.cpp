#include "qtjambiapi.h"
#include <QtCore/QReadWriteLock>
#include <QtCore/QMap>
#include <QtCore/QSharedPointer>
#include "containeraccess_p.h"
#include "functionpointer.h"
#include "registryutil_p.h"
#include "utils_p.h"
#include "coreapi.h"

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
typedef QMap<int, QSharedPointer<class AutoPairAccess>> PairAccessHash;
Q_GLOBAL_STATIC(PairAccessHash, gPairAccessHash)
Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gLock, (QReadWriteLock::Recursive))

QSharedPointer<class AutoPairAccess> getPairAccess(const QtPrivate::QMetaTypeInterface *iface){
    QSharedPointer<class AutoPairAccess> access;
    {
        QReadLocker locker(gLock());
        access = gPairAccessHash->value(iface->typeId.loadAcquire());
    }
    return access;
}
void AutoPairAccess::defaultCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr){
    if(QSharedPointer<class AutoPairAccess> access = getPairAccess(iface)){
        access->constructContainer(ptr);
    }
}
void AutoPairAccess::copyCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, const void *other){
    if(QSharedPointer<class AutoPairAccess> access = getPairAccess(iface)){
        access->constructContainer(ptr, other);
    }
}
void AutoPairAccess::moveCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, void *other){
    if(QSharedPointer<class AutoPairAccess> access = getPairAccess(iface)){
        access->constructContainer(ptr, other);
    }
}
void AutoPairAccess::dtor(const QtPrivate::QMetaTypeInterface *iface, void *ptr){
    if(QSharedPointer<class AutoPairAccess> access = getPairAccess(iface)){
        access->destructContainer(ptr);
    }
}
bool AutoPairAccess::equalsFn(const QtPrivate::QMetaTypeInterface *iface, const void *ptr1, const void *ptr2){
    if(QSharedPointer<class AutoPairAccess> access = getPairAccess(iface)){
        return access->equals(ptr1, ptr2);
    }
    return false;
}
void AutoPairAccess::debugStreamFn(const QtPrivate::QMetaTypeInterface *iface, QDebug &s, const void *ptr){
    if(QSharedPointer<class AutoPairAccess> access = getPairAccess(iface)){
        access->debugStream(s, ptr);
    }
}
void AutoPairAccess::dataStreamOutFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, const void *ptr){
    if(QSharedPointer<class AutoPairAccess> access = getPairAccess(iface)){
        access->dataStreamOut(s, ptr);
    }
}
void AutoPairAccess::dataStreamInFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, void *ptr){
    if(QSharedPointer<class AutoPairAccess> access = getPairAccess(iface)){
        access->dataStreamIn(s, ptr);
    }
}
#endif

AutoPairAccess::AutoPairAccess(const AutoPairAccess& other)
    : AbstractPairAccess(),
      m_keyMetaType(other.m_keyMetaType
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                       .id()),
      m_keyAlign(other.m_keyAlign
#endif
                                  ),
      m_keyHashFunction(other.m_keyHashFunction),
      m_keyInternalToExternalConverter(other.m_keyInternalToExternalConverter),
      m_keyExternalToInternalConverter(other.m_keyExternalToInternalConverter),
      m_valueMetaType(other.m_valueMetaType
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                          .id()),
      m_valueAlign(other.m_valueAlign
#endif
                                     ),
      m_valueHashFunction(other.m_valueHashFunction),
      m_valueInternalToExternalConverter(other.m_valueInternalToExternalConverter),
      m_valueExternalToInternalConverter(other.m_valueExternalToInternalConverter),
      m_align(other.m_align),
      m_offset(other.m_offset),
      m_size(other.m_size)
{
}

AutoPairAccess::AutoPairAccess(
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        int keyMetaType,
        size_t keyAlign,
#else
        const QMetaType& keyMetaType,
#endif
        const QHashFunction& keyHashFunction,
        const InternalToExternalConverter& keyInternalToExternalConverter,
        const ExternalToInternalConverter& keyExternalToInternalConverter,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        int valueMetaType,
        size_t valueAlign,
#else
        const QMetaType& valueMetaType,
#endif
        const QHashFunction& valueHashFunction,
        const InternalToExternalConverter& valueInternalToExternalConverter,
        const ExternalToInternalConverter& valueExternalToInternalConverter
        )
    :   AbstractPairAccess(),
      m_keyMetaType(keyMetaType),
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
      m_keyAlign(keyAlign),
#endif
      m_keyHashFunction(keyHashFunction),
      m_keyInternalToExternalConverter(keyInternalToExternalConverter),
      m_keyExternalToInternalConverter(keyExternalToInternalConverter),
      m_valueMetaType(valueMetaType),
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
      m_valueAlign(valueAlign),
#endif
      m_valueHashFunction(valueHashFunction),
      m_valueInternalToExternalConverter(valueInternalToExternalConverter),
      m_valueExternalToInternalConverter(valueExternalToInternalConverter),
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
      m_align(qMax(keyAlign, valueAlign)),
#else
      m_align(qMax(m_keyMetaType.alignOf(), m_valueMetaType.alignOf())),
#endif
      m_offset(0),
      m_size(0)
{
    Q_ASSERT(m_keyMetaType.id()!=QMetaType::UnknownType
            && m_keyMetaType.id()!=QMetaType::Void);
    Q_ASSERT(m_valueMetaType.id()!=QMetaType::UnknownType
            && m_valueMetaType.id()!=QMetaType::Void);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    m_offset = m_keyMetaType.sizeOf();
    if(m_offset%m_valueAlign>0)
        m_offset += m_valueAlign-m_offset%m_valueAlign;
    m_size = m_offset + m_valueMetaType.sizeOf();
    if(m_size%m_align>0)
        m_size += m_align-m_size%m_align;
#else
    m_offset = m_keyMetaType.sizeOf();
    if(m_offset%m_valueMetaType.alignOf()>0)
        m_offset += m_valueMetaType.alignOf()-m_offset%m_valueMetaType.alignOf();
    m_size = m_offset + m_valueMetaType.sizeOf();
    if(m_size%m_align>0)
        m_size += m_align-m_size%m_align;
#endif
}

void AutoPairAccess::dispose() { delete this; }

AutoPairAccess* AutoPairAccess::clone(){
    return new AutoPairAccess(*this);
}

size_t AutoPairAccess::sizeOf(){
    return m_size;
}

void* AutoPairAccess::constructContainer(void* result, const void* container) {
    m_keyMetaType.construct(result, container);
    m_valueMetaType.construct(reinterpret_cast<char*>(result)+m_offset, container ? reinterpret_cast<const char*>(container)+m_offset : nullptr);
    return result;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void* AutoPairAccess::constructContainer(void* result, void* container) {
    const QtPrivate::QMetaTypeInterface *iface = m_keyMetaType.iface();
    iface->moveCtr(iface, result, container);
    iface = m_valueMetaType.iface();
    iface->moveCtr(iface, reinterpret_cast<char*>(result)+m_offset, reinterpret_cast<char*>(container)+m_offset);
    return result;
}

bool AutoPairAccess::equals(const void* p1, const void* p2) {
    const QtPrivate::QMetaTypeInterface *iface = m_keyMetaType.iface();
    if(!iface->equals(iface, p1, p2))
        return false;
    iface = m_valueMetaType.iface();
    return iface->equals(iface, reinterpret_cast<const char*>(p1)+m_offset, reinterpret_cast<const char*>(p2)+m_offset);
}

void AutoPairAccess::debugStream(QDebug &s, const void *ptr){
    const QtPrivate::QMetaTypeInterface *iface = m_keyMetaType.iface();
    s = s.nospace().noquote();
    s << "std::pair(";
    if(iface->debugStream)
        iface->debugStream(iface, s, ptr);
    else if(iface->flags & QMetaType::IsPointer){
        if(iface->metaObjectFn && iface->metaObjectFn(iface))
            s << iface->metaObjectFn(iface)->className() << "(";
        else if(QLatin1String(iface->name).endsWith('*'))
            s << QLatin1String(iface->name).chopped(1) << "(";
        else
            s << iface->name << "(";
        s << "0x" << QString::number(*reinterpret_cast<const qint64*>(ptr), 16);
        s << ")";
    }else if(iface->flags & QMetaType::IsEnumeration){
        s << iface->name << "(";
        switch(iface->size){
        case 1: s << *reinterpret_cast<const qint8*>(ptr); break;
        case 2: s << *reinterpret_cast<const qint16*>(ptr); break;
        case 4: s << *reinterpret_cast<const qint32*>(ptr); break;
        case 8: s << *reinterpret_cast<const qint64*>(ptr); break;
        default: break;
        }
        s << ")";
    }else
        s << QVariant(m_keyMetaType, ptr);
    s << ",";
    ptr = reinterpret_cast<const char*>(ptr)+m_offset;
    iface = m_valueMetaType.iface();
    if(iface->debugStream)
        iface->debugStream(iface, s, ptr);
    else if(iface->flags & QMetaType::IsPointer){
        if(iface->metaObjectFn && iface->metaObjectFn(iface))
            s << iface->metaObjectFn(iface)->className() << "(";
        else if(QLatin1String(iface->name).endsWith('*'))
            s << QLatin1String(iface->name).chopped(1) << "(";
        else
            s << iface->name << "(";
        s << "0x" << QString::number(*reinterpret_cast<const qint64*>(ptr), 16);
        s << ")";
    }else if(iface->flags & QMetaType::IsEnumeration){
        s << iface->name << "(";
        switch(iface->size){
        case 1: s << *reinterpret_cast<const qint8*>(ptr); break;
        case 2: s << *reinterpret_cast<const qint16*>(ptr); break;
        case 4: s << *reinterpret_cast<const qint32*>(ptr); break;
        case 8: s << *reinterpret_cast<const qint64*>(ptr); break;
        default: break;
        }
        s << ")";
    }else
        s << QVariant(m_valueMetaType, ptr);
    s << ")";
}

void AutoPairAccess::dataStreamOut(QDataStream &s, const void *ptr){
    const QtPrivate::QMetaTypeInterface *iface = m_keyMetaType.iface();
    if(iface->dataStreamOut)
        iface->dataStreamOut(iface, s, ptr);
    else if(iface->flags & QMetaType::IsEnumeration){
        switch(iface->size){
        case 1: s << *reinterpret_cast<const qint8*>(ptr); break;
        case 2: s << *reinterpret_cast<const qint16*>(ptr); break;
        case 4: s << *reinterpret_cast<const qint32*>(ptr); break;
        case 8: s << *reinterpret_cast<const qint64*>(ptr); break;
        default: break;
        }
    }else
        QVariant(m_keyMetaType, ptr).save(s);
    iface = m_valueMetaType.iface();
    ptr = reinterpret_cast<const char*>(ptr)+m_offset;
    if(iface->dataStreamOut)
        iface->dataStreamOut(iface, s, ptr);
    else if(iface->flags & QMetaType::IsEnumeration){
        switch(iface->size){
        case 1: s << *reinterpret_cast<const qint8*>(ptr); break;
        case 2: s << *reinterpret_cast<const qint16*>(ptr); break;
        case 4: s << *reinterpret_cast<const qint32*>(ptr); break;
        case 8: s << *reinterpret_cast<const qint64*>(ptr); break;
        default: break;
        }
    }else
        QVariant(m_valueMetaType, ptr).save(s);
}

void AutoPairAccess::dataStreamIn(QDataStream &s, void *ptr){
    const QtPrivate::QMetaTypeInterface *iface = m_keyMetaType.iface();
    if(iface->dataStreamIn){
        iface->dataStreamIn(iface, s, ptr);
    }else if(iface->flags & QMetaType::IsEnumeration){
        switch(iface->size){
        case 1: s >> *reinterpret_cast<qint8*>(ptr); break;
        case 2: s >> *reinterpret_cast<qint16*>(ptr); break;
        case 4: s >> *reinterpret_cast<qint32*>(ptr); break;
        case 8: s >> *reinterpret_cast<qint64*>(ptr); break;
        default: break;
        }
    }else{
        if(iface->dtor)
            iface->dtor(iface, ptr);
        QVariant v(m_keyMetaType);
        v.load(s);
        m_keyMetaType.construct(ptr, v.data());
    }
    iface = m_valueMetaType.iface();
    ptr = reinterpret_cast<char*>(ptr)+m_offset;
    if(iface->dataStreamIn){
        iface->dataStreamIn(iface, s, ptr);
    }else if(iface->flags & QMetaType::IsEnumeration){
        switch(iface->size){
        case 1: s >> *reinterpret_cast<qint8*>(ptr); break;
        case 2: s >> *reinterpret_cast<qint16*>(ptr); break;
        case 4: s >> *reinterpret_cast<qint32*>(ptr); break;
        case 8: s >> *reinterpret_cast<qint64*>(ptr); break;
        default: break;
        }
    }else{
        if(iface->dtor)
            iface->dtor(iface, ptr);
        QVariant v(m_valueMetaType);
        v.load(s);
        m_valueMetaType.construct(ptr, v.data());
    }
}
#endif

void AutoPairAccess::assign(void* container, const void* other) {
    if(other){
        void* v1 = reinterpret_cast<char*>(container)+m_offset;
        const void* v2 = reinterpret_cast<const char*>(other)+m_offset;
        m_keyMetaType.destruct(container);
        m_keyMetaType.construct(container, other);
        m_valueMetaType.destruct(v1);
        m_valueMetaType.construct(v1, v2);
    }
}

bool AutoPairAccess::destructContainer(void* container) {
    void* v1 = reinterpret_cast<char*>(container)+m_offset;
    m_keyMetaType.destruct(container);
    m_valueMetaType.destruct(v1);
    return true;
}

int AutoPairAccess::registerContainer(const QByteArray& typeName) {
    int newMetaType = QMetaType::type(typeName);
    if(newMetaType==QMetaType::UnknownType){
        if(typeName.startsWith("QPair<")){
            newMetaType = QMetaType::type("std::p"+typeName.mid(2));
        }else if(typeName.startsWith("std::pair<")){
            newMetaType = QMetaType::type("QP"+typeName.mid(6));
        }
    }
    if(newMetaType==QMetaType::UnknownType){
        QSharedPointer<AutoPairAccess> access(new AutoPairAccess(*this), &containerDisposer);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        auto kiface = m_keyMetaType.iface();
        auto viface = m_valueMetaType.iface();
#endif
        newMetaType = registerContainerMetaType(typeName,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                       qtjambi_function_pointer<16,void(void*)>([access](void* ptr){
                                            access->destructContainer(ptr);
                                       }, qHash(typeName)),
                                       qtjambi_function_pointer<16,void*(void *, const void *)>([access](void* ptr, const void * other) -> void* {
                                            return access->constructContainer(ptr, other);
                                       }, qHash(typeName)),
                                       uint(m_size),
                                       uint(m_align),
#else
                                       (kiface->defaultCtr || !(kiface->flags & QMetaType::NeedsConstruction)) && (viface->defaultCtr || !(viface->flags & QMetaType::NeedsConstruction)) ? AutoPairAccess::defaultCtr : nullptr,
                                       (kiface->copyCtr || !(kiface->flags & QMetaType::NeedsConstruction)) && (viface->copyCtr || !(viface->flags & QMetaType::NeedsConstruction)) ? AutoPairAccess::copyCtr : nullptr,
                                       (kiface->moveCtr || !(kiface->flags & QMetaType::NeedsConstruction)) && (viface->moveCtr || !(viface->flags & QMetaType::NeedsConstruction)) ? AutoPairAccess::moveCtr : nullptr,
                                       AutoPairAccess::dtor,
                                       (kiface->equals
                                                    || (kiface->flags & QMetaType::IsPointer)
                                                    || (kiface->flags & QMetaType::IsEnumeration))
                                                && (viface->equals
                                                    || (viface->flags & QMetaType::IsPointer)
                                                    || (viface->flags & QMetaType::IsEnumeration)) ? AutoPairAccess::equalsFn : nullptr,
                                       nullptr,
                                       (kiface->debugStream
                                                    || (kiface->flags & QMetaType::IsPointer)
                                                    || (kiface->flags & QMetaType::IsEnumeration))
                                                && (viface->debugStream
                                                    || (viface->flags & QMetaType::IsPointer)
                                                    || (viface->flags & QMetaType::IsEnumeration)) ? AutoPairAccess::debugStreamFn : nullptr,
                                       (kiface->dataStreamOut
                                                    || (kiface->flags & QMetaType::IsEnumeration))
                                                && (viface->dataStreamOut
                                                    || (viface->flags & QMetaType::IsEnumeration)) ? AutoPairAccess::dataStreamOutFn : nullptr,
                                       (kiface->dataStreamIn
                                                    || (kiface->flags & QMetaType::IsEnumeration))
                                                && (viface->dataStreamIn
                                                    || (viface->flags & QMetaType::IsEnumeration)) ? AutoPairAccess::dataStreamInFn : nullptr,
                                       nullptr,
                                       uint(m_size),
                                       ushort(m_align),
                                       QMetaType::UnknownType,
#endif
                                       QMetaType::NeedsConstruction
                                                   | QMetaType::NeedsDestruction
                                                   | QMetaType::MovableType,
                                       nullptr,
                                       nullptr);
        if(m_keyHashFunction && m_valueHashFunction){
            QHashFunction keyHash = m_keyHashFunction;
            QHashFunction valueHash = m_valueHashFunction;
            size_t offset = m_offset;
            insertHashFunctionByMetaType(newMetaType,
                                            [offset, keyHash, valueHash]
                                            (const void* ptr, hash_type seed)->hash_type{
                                                if(ptr){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                                    uint h1 = keyHash(ptr, seed);
                                                    uint h2 = valueHash(reinterpret_cast<const char*>(ptr)+offset, seed);
                                                    return ((h1 << 16) | (h1 >> 16)) ^ h2 ^ seed;
#else
                                                    hash_type pairSeed = seed;
                                                    pairSeed = pairSeed ^ (keyHash(ptr, 0) + 0x9e3779b9 + (pairSeed << 6) + (pairSeed >> 2));
                                                    pairSeed = pairSeed ^ (valueHash(reinterpret_cast<const char*>(ptr)+offset, 0) + 0x9e3779b9 + (pairSeed << 6) + (pairSeed >> 2));
                                                    return pairSeed;
#endif
                                                }else{
                                                    return 0;
                                                }
                                            });
        }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        {
            int to = qMetaTypeId<QtMetaTypePrivate::QPairVariantInterfaceImpl>();
            typedef QtMetaTypePrivate::VariantData (*getFunc)(const void * const *p, int metaTypeId, uint flags);
            struct ConverterFunction : QtPrivate::AbstractConverterFunction{
                int _metaType_id_first;
                uint _metaType_flags_first;
                int _metaType_id_second;
                uint _metaType_flags_second;
                getFunc _getFirst;
                getFunc _getSecond;

                ConverterFunction(const QMetaType& keyMetaType, const QMetaType& valueMetaType, size_t offset)
                    : QtPrivate::AbstractConverterFunction(convert),
                      _metaType_id_first(keyMetaType.id()),
                      _metaType_flags_first(AbstractContainerAccess::isPointerType(keyMetaType)),
                      _metaType_id_second(valueMetaType.id()),
                      _metaType_flags_second(AbstractContainerAccess::isPointerType(valueMetaType))
                {
                    _getFirst = [](const void * const *p, int metaTypeId, uint flags) -> QtMetaTypePrivate::VariantData {
                                    return QtMetaTypePrivate::VariantData(metaTypeId, p, flags);
                                };
                    _getSecond = qtjambi_function_pointer<16,QtMetaTypePrivate::VariantData(const void * const *, int, uint)>(
                                [offset](const void * const *p, int metaTypeId, uint flags) -> QtMetaTypePrivate::VariantData {
                                    const void* snd = reinterpret_cast<const char*>(p)+offset;
                                    return QtMetaTypePrivate::VariantData(metaTypeId, snd, flags);
                                }, hash_type(offset));
                }

                static bool convert(const AbstractConverterFunction *_f, const void *src, void*target){
                    const ConverterFunction* f = static_cast<const ConverterFunction*>(_f);
                    struct PairVariantInterfaceImpl
                    {
                        const void *_pair;
                        int _metaType_id_first;
                        uint _metaType_flags_first;
                        int _metaType_id_second;
                        uint _metaType_flags_second;
                        getFunc _getFirst;
                        getFunc _getSecond;
                    };
                    new(target) PairVariantInterfaceImpl{src,
                                f->_metaType_id_first,
                                f->_metaType_flags_first,
                                f->_metaType_id_second,
                                f->_metaType_flags_second,
                                f->_getFirst,
                                f->_getSecond};
                    return true;
                }
            };
            registerConverter(new ConverterFunction(m_keyMetaType, m_valueMetaType, m_offset), newMetaType, to);
        }
        if((QMetaType::hasRegisteredComparators(m_keyMetaType.id())
            || registeredComparator(m_keyMetaType.id())
            || m_keyMetaType.id()<QMetaType::User
            || AbstractContainerAccess::isPointerType(m_keyMetaType))
                && (QMetaType::hasRegisteredComparators(m_valueMetaType.id())
                || registeredComparator(m_valueMetaType.id())
                || m_valueMetaType.id()<QMetaType::User
                || AbstractContainerAccess::isPointerType(m_valueMetaType))){
            struct Comparator : QtPrivate::AbstractComparatorFunction{
                static bool lessThan(const AbstractComparatorFunction *_c, const void *a, const void *b){
                    const Comparator* c = static_cast<const Comparator*>(_c);
                    const void* a2 = reinterpret_cast<const char*>(a)+c->m_offset;
                    const void* b2 = reinterpret_cast<const char*>(b)+c->m_offset;
                    if(isLessThan(c->_metaType_id_first, a, b))
                        return true;
                    return isLessThan(c->_metaType_id_second, a2, b2);
                }
                static bool equals(const AbstractComparatorFunction *_c, const void *a, const void *b){
                    const Comparator* c = static_cast<const Comparator*>(_c);
                    const void* a2 = reinterpret_cast<const char*>(a)+c->m_offset;
                    const void* b2 = reinterpret_cast<const char*>(b)+c->m_offset;
                    return isEquals(c->_metaType_id_first, a, b) && isEquals(c->_metaType_id_second, a2, b2);
                }
                static void destroy(AbstractComparatorFunction *_c){
                    delete static_cast<Comparator*>(_c);
                }
                QMetaType _metaType_id_first;
                QMetaType _metaType_id_second;
                size_t m_offset;

                Comparator(const QMetaType& keyMetaType, const QMetaType& valueMetaType, size_t offset)
                    : QtPrivate::AbstractComparatorFunction(lessThan, equals, destroy),
                      _metaType_id_first(keyMetaType.id()),
                      _metaType_id_second(valueMetaType.id()),
                      m_offset(offset)
                {

                }
            };
            RegistryAPI::registerComparator(new Comparator(m_keyMetaType, m_valueMetaType, m_offset), newMetaType);
        }
        if((QMetaType::hasRegisteredDebugStreamOperator(m_keyMetaType.id())
                && QMetaType::hasRegisteredDebugStreamOperator(m_valueMetaType.id()))
                || (registeredDebugStreamOperator(m_keyMetaType.id())
                    && registeredDebugStreamOperator(m_valueMetaType.id()))
                || (m_keyMetaType.id()<QMetaType::User
                    && m_valueMetaType.id()<QMetaType::User)
                || (AbstractContainerAccess::isPointerType(m_keyMetaType)
                    || AbstractContainerAccess::isPointerType(m_valueMetaType))){
            struct DebugStreamFunction : QtPrivate::AbstractDebugStreamFunction{
                static void stream(const AbstractDebugStreamFunction *_f, QDebug& d, const void *ptr){
                    const DebugStreamFunction* f = static_cast<const DebugStreamFunction*>(_f);
                    const void* ptr2 = reinterpret_cast<const char*>(ptr)+f->m_offset;
                    d << "QPair(";
                    CoreAPI::appendToDebugStream(d, f->_metaType_id_first, ptr);
                    d << ",";
                    CoreAPI::appendToDebugStream(d, f->_metaType_id_second, ptr2);
                    d << ")";
                }
                static void destroy(AbstractDebugStreamFunction *_f){
                    delete static_cast<DebugStreamFunction*>(_f);
                }
                int _metaType_id_first;
                int _metaType_id_second;
                size_t m_offset;

                DebugStreamFunction(const QMetaType& keyMetaType, const QMetaType& valueMetaType, size_t offset)
                    : QtPrivate::AbstractDebugStreamFunction(stream, destroy),
                      _metaType_id_first(keyMetaType.id()),
                      _metaType_id_second(valueMetaType.id()),
                      m_offset(offset)
                {

                }
            };
            RegistryAPI::registerDebugStreamOperator(new DebugStreamFunction(m_keyMetaType, m_valueMetaType, m_offset), newMetaType);
        }
        {
            int _metaType_id_first = m_keyMetaType.id();
            int _metaType_id_second = m_valueMetaType.id();
            size_t offset = m_offset;
            QMetaType::registerStreamOperators(newMetaType,
                                               qtjambi_function_pointer<16,void(QDataStream &, const void *)>(
                                                                               [_metaType_id_first, _metaType_id_second, offset](QDataStream &s, const void * p){
                                                                                   const void* snd = reinterpret_cast<const char*>(p)+offset;
                                                                                   if(QMetaType::save(s, _metaType_id_first, p))
                                                                                      QMetaType::save(s, _metaType_id_second, snd);
                                                                               }, newMetaType),
                                               qtjambi_function_pointer<16,void(QDataStream &, void *)>(
                                                                               [_metaType_id_first, _metaType_id_second, offset](QDataStream &s, void * p){
                                                                                   void* snd = reinterpret_cast<char*>(p)+offset;
                                                                                   if(QMetaType::load(s, _metaType_id_first, p))
                                                                                      QMetaType::load(s, _metaType_id_second, snd);
                                                                               }, newMetaType)
                                               );
        }
#else
        {
            const QMetaType to = QMetaType::fromType<QtMetaTypePrivate::QPairVariantInterfaceImpl>();
            QMetaType::ConverterFunction o = [newMetaType](const void *src, void *target)->bool{
                QtMetaTypePrivate::QPairVariantInterfaceImpl* impl = new(target) QtMetaTypePrivate::QPairVariantInterfaceImpl();
                QSharedPointer<class AutoPairAccess> access = getPairAccess(QMetaType(newMetaType).iface());
                impl->_pair = src;
                impl->_metaType_first = access->m_keyMetaType;
                impl->_metaType_second = access->m_valueMetaType;
                impl->_getFirst = qtjambi_function_pointer<16,void(const void * const *, void *)>(
                            [newMetaType](const void * const *pair, void *dataPtr){
                                if(pair){
                                    if(QSharedPointer<class AutoPairAccess> access = getPairAccess(QMetaType(newMetaType).iface())){
                                        const void* fst = reinterpret_cast<const char*>(*pair);
                                        access->m_keyMetaType.construct(dataPtr, fst);
                                    }
                                }
                            }, newMetaType);
                impl->_getSecond = qtjambi_function_pointer<16,void(const void * const *, void *)>(
                            [newMetaType](const void * const *pair, void *dataPtr) {
                                if(pair){
                                    if(QSharedPointer<class AutoPairAccess> access = getPairAccess(QMetaType(newMetaType).iface())){
                                        const void* snd = reinterpret_cast<const char*>(*pair)+access->m_offset;
                                        access->m_valueMetaType.construct(dataPtr, snd);
                                    }
                                }
                            }, newMetaType);
                return true;
            };
            QMetaType::registerConverterFunction(o, QMetaType(newMetaType), to);
        }
        {
            QWriteLocker locker(gLock());
            gPairAccessHash->insert(newMetaType, access);
        }
#endif
    }
    return newMetaType;
}

jobject AutoPairAccess::first(JNIEnv * env, const void* container) {
    jobject result = nullptr;
    {
        jvalue _first;
        _first.l = nullptr;
        if(m_keyInternalToExternalConverter(env, nullptr, container, &_first, true)){
            result = _first.l;
        }
    }
    return result;
}

void AutoPairAccess::setFirst(JNIEnv *env, void* container, jobject first) {
    jvalue jv;
    jv.l = first;
    m_keyExternalToInternalConverter(env, nullptr, jv, container, jValueType::l);
}

jobject AutoPairAccess::second(JNIEnv * env, const void* container) {
    const void* snd = reinterpret_cast<const char*>(container)+m_offset;
    jobject result = nullptr;
    {
        jvalue _value;
        _value.l = nullptr;
        if(m_valueInternalToExternalConverter(env, nullptr, snd, &_value, true)){
            result = _value.l;
        }
    }
    return result;
}

void AutoPairAccess::setSecond(JNIEnv *env, void* container, jobject second) {
    void* snd = reinterpret_cast<char*>(container)+m_offset;
    jvalue jv;
    jv.l = second;
    m_valueExternalToInternalConverter(env, nullptr, jv, snd, jValueType::l);
}
