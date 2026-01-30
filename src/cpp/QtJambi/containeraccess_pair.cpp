/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

QSharedPointer<class AutoPairAccess> getPairAccess(const QtPrivate::QMetaTypeInterface *iface){
    return findContainerAccess(QMetaType(iface)).dynamicCast<AutoPairAccess>();
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

AutoPairAccess::AutoPairAccess(const AutoPairAccess& other)
    : AbstractPairAccess(), AbstractNestedPairAccess(),
      m_keyMetaType(other.m_keyMetaType),
      m_keyHashFunction(other.m_keyHashFunction),
      m_keyInternalToExternalConverter(other.m_keyInternalToExternalConverter),
      m_keyExternalToInternalConverter(other.m_keyExternalToInternalConverter),
      m_keyNestedContainerAccess(other.m_keyNestedContainerAccess),
      m_valueMetaType(other.m_valueMetaType),
      m_valueHashFunction(other.m_valueHashFunction),
      m_valueInternalToExternalConverter(other.m_valueInternalToExternalConverter),
      m_valueExternalToInternalConverter(other.m_valueExternalToInternalConverter),
      m_valueNestedContainerAccess(other.m_valueNestedContainerAccess),
      m_align(other.m_align),
      m_offset(other.m_offset),
      m_size(other.m_size),
      m_keyOwnerFunction(other.m_keyOwnerFunction),
      m_valueOwnerFunction(other.m_valueOwnerFunction),
      m_keyDataType(other.m_keyDataType),
      m_valueDataType(other.m_valueDataType)
{
}

AutoPairAccess::AutoPairAccess(
        const QMetaType& keyMetaType,
        const QtJambiUtils::QHashFunction& keyHashFunction,
        const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
        const QtJambiUtils::ExternalToInternalConverter& keyExternalToInternalConverter,
        const QSharedPointer<AbstractContainerAccess>& keyNestedContainerAccess,
        PtrOwnerFunction keyOwnerFunction,
        AbstractContainerAccess::DataType keyDataType,
        const QMetaType& valueMetaType,
        const QtJambiUtils::QHashFunction& valueHashFunction,
        const QtJambiUtils::InternalToExternalConverter& valueInternalToExternalConverter,
        const QtJambiUtils::ExternalToInternalConverter& valueExternalToInternalConverter,
        const QSharedPointer<AbstractContainerAccess>& valueNestedContainerAccess,
        PtrOwnerFunction valueOwnerFunction,
        AbstractContainerAccess::DataType valueDataType
        )
    :   AbstractPairAccess(), AbstractNestedPairAccess(),
      m_keyMetaType(keyMetaType),
      m_keyHashFunction(keyHashFunction),
      m_keyInternalToExternalConverter(keyInternalToExternalConverter),
      m_keyExternalToInternalConverter(keyExternalToInternalConverter),
      m_keyNestedContainerAccess(keyNestedContainerAccess),
      m_valueMetaType(valueMetaType),
      m_valueHashFunction(valueHashFunction),
      m_valueInternalToExternalConverter(valueInternalToExternalConverter),
      m_valueExternalToInternalConverter(valueExternalToInternalConverter),
      m_valueNestedContainerAccess(valueNestedContainerAccess),
      m_align(qMax(m_keyMetaType.alignOf(), m_valueMetaType.alignOf())),
      m_offset(0),
      m_size(0),
      m_keyOwnerFunction(keyOwnerFunction),
      m_valueOwnerFunction(valueOwnerFunction),
      m_keyDataType(keyDataType),
      m_valueDataType(valueDataType)
{
    Q_ASSERT(m_keyMetaType.id()!=QMetaType::UnknownType
            && m_keyMetaType.id()!=QMetaType::Void);
    Q_ASSERT(m_valueMetaType.id()!=QMetaType::UnknownType
            && m_valueMetaType.id()!=QMetaType::Void);
    m_offset = m_keyMetaType.sizeOf();
    if(m_offset%m_valueMetaType.alignOf()>0)
        m_offset += m_valueMetaType.alignOf()-m_offset%m_valueMetaType.alignOf();
    m_size = m_offset + m_valueMetaType.sizeOf();
    if(m_size%m_align>0)
        m_size += m_align-m_size%m_align;
}

void AutoPairAccess::dispose() { delete this; }

AutoPairAccess* AutoPairAccess::clone(){
    return new AutoPairAccess(*this);
}

size_t AutoPairAccess::sizeOf() const {
    return m_size;
}

size_t AutoPairAccess::alignOf() const {
    return m_align;
}

void* AutoPairAccess::constructContainer(JNIEnv*, void* result, const ConstContainerAndAccessInfo& container) {
    return constructContainer(result, container.container);
}

void* AutoPairAccess::constructContainer(void* result) {
    m_keyMetaType.construct(result);
    m_valueMetaType.construct(reinterpret_cast<char*>(result)+m_offset);
    return result;
}

void* AutoPairAccess::constructContainer(void* result, const void* container) {
    m_keyMetaType.construct(result, container);
    m_valueMetaType.construct(reinterpret_cast<char*>(result)+m_offset, reinterpret_cast<const char*>(container)+m_offset);
    return result;
}

void* AutoPairAccess::constructContainer(JNIEnv*, void* result, const ContainerAndAccessInfo& container) {
    return constructContainer(result, container.container);
}

void* AutoPairAccess::constructContainer(void* result, void* container) {
    const QtPrivate::QMetaTypeInterface *iface = m_keyMetaType.iface();
    if(iface->moveCtr)
        iface->moveCtr(iface, result, container);
    else
        memcpy(result, container, iface->size);
    iface = m_valueMetaType.iface();
    if(iface->moveCtr)
        iface->moveCtr(iface, reinterpret_cast<char*>(result)+m_offset, reinterpret_cast<char*>(container)+m_offset);
    else
        memcpy(reinterpret_cast<char*>(result)+m_offset, reinterpret_cast<char*>(container)+m_offset, iface->size);
    return result;
}

bool AutoPairAccess::equals(const void* p1, const void* p2) {
    if(!m_keyMetaType.equals(p1, p2))
        return false;
    return m_valueMetaType.equals(reinterpret_cast<const char*>(p1)+m_offset, reinterpret_cast<const char*>(p2)+m_offset);
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

void AutoPairAccess::assign(JNIEnv *, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) {
    assign(container.container, other.container);
}

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

void AutoPairAccess::swap(JNIEnv *, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    if(container2.container){
        void* v1 = reinterpret_cast<char*>(container.container)+m_offset;
        void* v2 = reinterpret_cast<char*>(container2.container)+m_offset;
        auto kiface = m_keyMetaType.iface();
        if(kiface->moveCtr){
            void* tmpFirst;
            if (kiface->alignment > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
                tmpFirst = operator new(kiface->size, std::align_val_t(kiface->alignment));
            else
                tmpFirst = operator new(kiface->size);
            kiface->moveCtr(kiface, tmpFirst, container.container);
            m_keyMetaType.destruct(container.container);
            kiface->moveCtr(kiface, container.container, container2.container);
            m_keyMetaType.destruct(container2.container);
            kiface->moveCtr(kiface, container2.container, tmpFirst);
            m_keyMetaType.destroy(tmpFirst);
        }else{
            void* tmpFirst = m_keyMetaType.create(container.container);
            m_keyMetaType.destruct(container.container);
            m_keyMetaType.construct(container.container, container2.container);
            m_keyMetaType.destruct(container2.container);
            m_keyMetaType.construct(container2.container, tmpFirst);
            m_keyMetaType.destroy(tmpFirst);
        }
        auto viface = m_valueMetaType.iface();
        if(viface->moveCtr){
            void* tmpSecond;
            if (viface->alignment > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
                tmpSecond = operator new(viface->size, std::align_val_t(viface->alignment));
            else
                tmpSecond = operator new(viface->size);
            viface->moveCtr(viface, tmpSecond, v1);
            m_valueMetaType.destruct(v1);
            viface->moveCtr(viface, v1, v2);
            m_valueMetaType.destruct(v2);
            viface->moveCtr(viface, v2, tmpSecond);
            m_valueMetaType.destroy(tmpSecond);
        }else{
            void* tmpSecond = m_valueMetaType.create(v1);
            m_valueMetaType.destruct(v1);
            m_valueMetaType.construct(v1, v2);
            m_valueMetaType.destruct(v2);
            m_valueMetaType.construct(v2, tmpSecond);
            m_valueMetaType.destroy(tmpSecond);
        }
    }
}

bool AutoPairAccess::destructContainer(void* container) {
    void* v1 = reinterpret_cast<char*>(container)+m_offset;
    m_keyMetaType.destruct(container);
    m_valueMetaType.destruct(v1);
    return true;
}

QMetaType AutoPairAccess::registerContainer(const QByteArray& typeName) {
    QMetaType newMetaType = QMetaType::fromName(typeName);
    if(!newMetaType.isValid()){
        if(typeName.startsWith("QPair<")){
            newMetaType = QMetaType::fromName("std::p"+typeName.mid(2));
        }else if(typeName.startsWith("std::pair<")){
            newMetaType = QMetaType::fromName("QP"+typeName.mid(6));
        }
    }
    if(!newMetaType.isValid()){
        QSharedPointer<AutoPairAccess> access(new AutoPairAccess(*this), &containerDisposer);
        auto kiface = m_keyMetaType.iface();
        auto viface = m_valueMetaType.iface();
        newMetaType = registerContainerMetaType(typeName,
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
                                       QMetaType::NeedsConstruction
                                                   | QMetaType::NeedsDestruction
                                                   | QMetaType::RelocatableType,
                                       nullptr,
                                       nullptr,
                                       access);
        if(m_keyHashFunction && m_valueHashFunction){
            QtJambiUtils::QHashFunction keyHash = m_keyHashFunction;
            QtJambiUtils::QHashFunction valueHash = m_valueHashFunction;
            size_t offset = m_offset;
            insertHashFunctionByMetaType(newMetaType.iface(),
                                            [offset, keyHash, valueHash]
                                            (const void* ptr, size_t seed)->size_t{
                                                if(ptr){
                                                    size_t pairSeed = seed;
                                                    pairSeed = pairSeed ^ (keyHash(ptr, 0) + 0x9e3779b9 + (pairSeed << 6) + (pairSeed >> 2));
                                                    pairSeed = pairSeed ^ (valueHash(reinterpret_cast<const char*>(ptr)+offset, 0) + 0x9e3779b9 + (pairSeed << 6) + (pairSeed >> 2));
                                                    return pairSeed;
                                                }else{
                                                    return 0;
                                                }
                                            });
        }
        {
            const QMetaType to = QMetaType::fromType<QtMetaTypePrivate::QPairVariantInterfaceImpl>();
            QMetaType::ConverterFunction o = [newMetaType](const void *src, void *target)->bool{
                QtMetaTypePrivate::QPairVariantInterfaceImpl* impl = new(target) QtMetaTypePrivate::QPairVariantInterfaceImpl();
                QSharedPointer<class AutoPairAccess> access = getPairAccess(newMetaType.iface());
                impl->_pair = src;
                impl->_metaType_first = access->m_keyMetaType;
                impl->_metaType_second = access->m_valueMetaType;
                impl->_getFirst = qtjambi_function_pointer<16,void(const void * const *, void *)>(
                            [newMetaType](const void * const *pair, void *dataPtr){
                                if(pair){
                                    if(QSharedPointer<class AutoPairAccess> access = getPairAccess(newMetaType.iface())){
                                        const void* fst = reinterpret_cast<const char*>(*pair);
                                        access->m_keyMetaType.construct(dataPtr, fst);
                                    }
                                }
                    }, size_t(newMetaType.iface()));
                impl->_getSecond = qtjambi_function_pointer<16,void(const void * const *, void *)>(
                            [newMetaType](const void * const *pair, void *dataPtr) {
                                if(pair){
                                    if(QSharedPointer<class AutoPairAccess> access = getPairAccess(newMetaType.iface())){
                                        const void* snd = reinterpret_cast<const char*>(*pair)+access->m_offset;
                                        access->m_valueMetaType.construct(dataPtr, snd);
                                    }
                                }
                    }, size_t(newMetaType.iface()));
                return true;
            };
            QMetaType::registerConverterFunction(o, newMetaType, to);
        }
    }else{
        registerContainerAccess(newMetaType, this);
    }
    return newMetaType;
}

jobject AutoPairAccess::first(JNIEnv * env, const void* container) {
    jobject result = nullptr;
    {
        jvalue _first;
        _first.l = nullptr;
        if(m_keyInternalToExternalConverter(env, nullptr, container, _first, true)){
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
        if(m_valueInternalToExternalConverter(env, nullptr, snd, _value, true)){
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

AbstractContainerAccess::DataType AutoPairAccess::firstType() {
    return m_keyDataType;
}

AbstractContainerAccess::DataType AutoPairAccess::secondType() {
    return m_valueDataType;
}

QPair<const void*,const void*> AutoPairAccess::elements(const void* container) {
    const void* key = container;
    const void* value = reinterpret_cast<const char*>(container)+m_offset;
    if(m_keyDataType & AbstractContainerAccess::PointersMask){
        key = *reinterpret_cast<void*const*>(key);
    }
    if(m_valueDataType & AbstractContainerAccess::PointersMask){
        value = *reinterpret_cast<void*const*>(value);
    }
    return {key, value};
}

const QMetaType& AutoPairAccess::firstMetaType() {return m_keyMetaType;}
const QMetaType& AutoPairAccess::secondMetaType() {return m_valueMetaType;}

AbstractContainerAccess* AutoPairAccess::firstNestedContainerAccess() {
    return m_keyNestedContainerAccess ? m_keyNestedContainerAccess->clone() : nullptr;
}

AbstractContainerAccess* AutoPairAccess::secondNestedContainerAccess() {
    return m_valueNestedContainerAccess ? m_valueNestedContainerAccess->clone() : nullptr;
}

const QSharedPointer<AbstractContainerAccess>& AutoPairAccess::sharedFirstNestedContainerAccess(){
    return m_keyNestedContainerAccess;
}
const QSharedPointer<AbstractContainerAccess>& AutoPairAccess::sharedSecondNestedContainerAccess(){
    return m_valueNestedContainerAccess;
}
bool AutoPairAccess::hasFirstNestedContainerAccess() {
    return !m_keyNestedContainerAccess.isNull();
}
bool AutoPairAccess::hasFirstNestedPointers() {
    if(hasFirstNestedContainerAccess()){
        if(auto daccess = dynamic_cast<AbstractSequentialAccess*>(m_keyNestedContainerAccess.data())){
            return (daccess->elementType() & PointersMask) || daccess->hasNestedPointers();
        }else if(auto daccess = dynamic_cast<AbstractAssociativeAccess*>(m_keyNestedContainerAccess.data())){
            return (daccess->keyType() & PointersMask) || daccess->hasKeyNestedPointers() || (daccess->valueType() & PointersMask) || daccess->hasValueNestedPointers();
        }else if(auto daccess = dynamic_cast<AbstractPairAccess*>(m_keyNestedContainerAccess.data())){
            return (daccess->firstType() & PointersMask) || daccess->hasFirstNestedPointers() || (daccess->secondType() & PointersMask) || daccess->hasSecondNestedPointers();
        }
    }
    return false;
}
bool AutoPairAccess::hasSecondNestedContainerAccess() {
    return !m_valueNestedContainerAccess.isNull();
}
bool AutoPairAccess::hasSecondNestedPointers() {
    if(hasSecondNestedContainerAccess()){
        if(auto daccess = dynamic_cast<AbstractSequentialAccess*>(m_valueNestedContainerAccess.data())){
            return (daccess->elementType() & PointersMask) || daccess->hasNestedPointers();
        }else if(auto daccess = dynamic_cast<AbstractAssociativeAccess*>(m_valueNestedContainerAccess.data())){
            return (daccess->keyType() & PointersMask) || daccess->hasKeyNestedPointers() || (daccess->valueType() & PointersMask) || daccess->hasValueNestedPointers();
        }else if(auto daccess = dynamic_cast<AbstractPairAccess*>(m_valueNestedContainerAccess.data())){
            return (daccess->firstType() & PointersMask) || daccess->hasFirstNestedPointers() || (daccess->secondType() & PointersMask) || daccess->hasSecondNestedPointers();
        }
    }
    return false;
}

const QObject* AutoPairAccess::getOwner(const void* container){
    if(hasOwnerFunction()){
        auto el = elements(container);
        if(m_keyOwnerFunction){
            if(const QObject* owner = m_keyOwnerFunction(el.first))
                return owner;
        }else if(m_valueOwnerFunction){
            if(const QObject* owner = m_valueOwnerFunction(el.second))
                return owner;
        }else if(m_keyNestedContainerAccess){
            if(const QObject* owner = m_keyNestedContainerAccess->getOwner(el.first))
                return owner;
        }else if(m_valueNestedContainerAccess){
            if(const QObject* owner = m_valueNestedContainerAccess->getOwner(el.second))
                return owner;
        }
    }
    return nullptr;
}

bool AutoPairAccess::hasOwnerFunction(){
    if(m_keyOwnerFunction && !(firstType() & PointersMask))
        return true;
    if(m_valueOwnerFunction && !(secondType() & PointersMask))
        return true;
    if(!(firstType() & PointersMask) && m_keyNestedContainerAccess && m_keyNestedContainerAccess->hasOwnerFunction())
        return true;
    if(!(secondType() & PointersMask) && m_valueNestedContainerAccess && m_valueNestedContainerAccess->hasOwnerFunction())
        return true;
    return false;
}

std::unique_ptr<AbstractSequentialAccess::ElementIterator> AutoPairAccess::elementIterator(const void* container) {
    class ElementIterator : public AbstractSequentialAccess::ElementIterator{
        AutoPairAccess* m_access;
        const void* container;
        uint index = 0;
        ElementIterator(const ElementIterator& other)
            :m_access(other.m_access),
            container(other.container) {}
    protected:
        AbstractSequentialAccess* access() override { return nullptr; }
    public:
        ElementIterator(AutoPairAccess* _access, const void* p)
            : AbstractListAccess::ElementIterator(),
            m_access(_access),
            container(p)
        {
        }
        const QMetaType& elementMetaType() override {
            switch(index){
            case 0:
                return m_access->firstMetaType();
            default:
                return m_access->secondMetaType();
            }
        }
        DataType elementType() override {
            switch(index){
            case 0:
                return m_access->firstType();
            default:
                return m_access->secondType();
            }
        }
        AbstractContainerAccess* elementNestedContainerAccess() override {
            switch(index){
            case 0:
                return m_access->firstNestedContainerAccess();
            case 1:
                return m_access->secondNestedContainerAccess();
            default:
                return nullptr;
            }
        }
        bool hasNestedContainerAccess() override {
            return elementNestedContainerAccess();
        }
        bool hasNestedPointers() override {
            switch(index){
            case 0:
                return m_access->hasFirstNestedPointers();
            default:
                return m_access->hasSecondNestedPointers();
            }
        }
        bool hasNext() override {return index<2;};
        bool isConst() override{
            return true;
        }
        jobject next(JNIEnv * env) override{
            switch(index){
            case 0:
                ++index;
                return m_access->first(env, container);
            case 1:
                ++index;
                return m_access->second(env, container);
            default:
                return nullptr;
            }
        }
        const void* next() override {
            if(elementType() & AbstractContainerAccess::PointersMask){
                return *reinterpret_cast<void*const*>(constNext());
            }else{
                return constNext();
            }
        }
        const void* constNext() override {
            switch(index){
            case 0:
                ++index;
                return container;
            case 1:
                ++index;
                return reinterpret_cast<const char*>(container)+m_access->m_offset;
            default:
                return nullptr;
            }
        }
        void* mutableNext() override {
            return nullptr;
        }
        bool operator==(const AbstractSequentialAccess::ElementIterator& other) const override {
            return container==reinterpret_cast<const ElementIterator&>(other).container && index==reinterpret_cast<const ElementIterator&>(other).index;
        }
        std::unique_ptr<AbstractSequentialAccess::ElementIterator> clone() const override {
            return std::unique_ptr<AbstractSequentialAccess::ElementIterator>(new ElementIterator(*this));
        }
    };
    return std::unique_ptr<AbstractSequentialAccess::ElementIterator>(new ElementIterator(this, container));
}

std::unique_ptr<AbstractSequentialAccess::ElementIterator> AutoPairAccess::elementIterator(void* container) {
    class ElementIterator : public AbstractSequentialAccess::ElementIterator{
        AutoPairAccess* m_access;
        void* container;
        uint index = 0;
        ElementIterator(const ElementIterator& other)
            :m_access(other.m_access),
            container(other.container) {}
    protected:
        AbstractSequentialAccess* access() override { return nullptr; }
    public:
        ElementIterator(AutoPairAccess* _access, void* p)
            : AbstractListAccess::ElementIterator(),
            m_access(_access),
            container(p)
        {
        }
        const QMetaType& elementMetaType() override {
            switch(index){
            case 0:
                return m_access->firstMetaType();
            default:
                return m_access->secondMetaType();
            }
        }
        DataType elementType() override {
            switch(index){
            case 0:
                return m_access->firstType();
            default:
                return m_access->secondType();
            }
        }
        AbstractContainerAccess* elementNestedContainerAccess() override {
            switch(index){
            case 0:
                return m_access->firstNestedContainerAccess();
            case 1:
                return m_access->secondNestedContainerAccess();
            default:
                return nullptr;
            }
        }
        bool hasNestedContainerAccess() override {
            return elementNestedContainerAccess();
        }
        bool hasNestedPointers() override {
            switch(index){
            case 0:
                return m_access->hasFirstNestedPointers();
            default:
                return m_access->hasSecondNestedPointers();
            }
        }
        bool hasNext() override {return index<2;};
        bool isConst() override{
            return false;
        }
        jobject next(JNIEnv * env) override{
            switch(index){
            case 0:
                ++index;
                return m_access->first(env, container);
            case 1:
                ++index;
                return m_access->second(env, container);
            default:
                return nullptr;
            }
        }
        const void* next() override {
            if(elementType() & AbstractContainerAccess::PointersMask){
                return *reinterpret_cast<void*const*>(constNext());
            }else{
                return constNext();
            }
        }
        const void* constNext() override {
            switch(index){
            case 0:
                ++index;
                return container;
            case 1:
                ++index;
                return reinterpret_cast<const char*>(container)+m_access->m_offset;
            default:
                return nullptr;
            }
        }
        void* mutableNext() override {
            switch(index){
            case 0:
                ++index;
                return container;
            case 1:
                ++index;
                return reinterpret_cast<char*>(container)+m_access->m_offset;
            default:
                return nullptr;
            }
        }
        bool operator==(const AbstractSequentialAccess::ElementIterator& other) const override {
            return container==reinterpret_cast<const ElementIterator&>(other).container && index==reinterpret_cast<const ElementIterator&>(other).index;
        }
        std::unique_ptr<AbstractSequentialAccess::ElementIterator> clone() const override {
            return std::unique_ptr<AbstractSequentialAccess::ElementIterator>(new ElementIterator(*this));
        }
    };
    return std::unique_ptr<AbstractSequentialAccess::ElementIterator>(new ElementIterator(this, container));
}

std::unique_ptr<AbstractHashAccess::KeyValueIterator> AutoPairAccess::keyValueIterator(void* container) {
    class KeyValueIterator : public AbstractHashAccess::KeyValueIterator{
        AutoPairAccess* m_access;
        void* container;
        KeyValueIterator(const KeyValueIterator& other)
            :m_access(other.m_access),
            container(other.container) {}
    protected:
        AbstractAssociativeAccess* access() override {return nullptr;}
    public:
        KeyValueIterator(AutoPairAccess* _access, void* _container)
            :m_access(_access), container(_container)
        {}
        const QMetaType& keyMetaType() override {return m_access->firstMetaType();}
        const QMetaType& valueMetaType() override {return m_access->secondMetaType();}
        DataType keyType() override {return m_access->firstType();}
        DataType valueType() override {return m_access->secondType();}
        AbstractContainerAccess* keyNestedContainerAccess() override {return m_access->firstNestedContainerAccess();}
        AbstractContainerAccess* valueNestedContainerAccess() override {return m_access->secondNestedContainerAccess();}
        bool hasKeyNestedContainerAccess() override {return m_access->hasFirstNestedContainerAccess();}
        bool hasValueNestedContainerAccess() override {return m_access->hasSecondNestedContainerAccess();}
        bool hasKeyNestedPointers() override {return m_access->hasFirstNestedPointers();}
        bool hasValueNestedPointers() override {return m_access->hasSecondNestedPointers();}
        bool hasNext() override{
            return container;
        }
        QPair<jobject,jobject> next(JNIEnv * env) override{
            QPair<jobject,jobject> result;
            if(container){
                result.first = m_access->first(env, container);
                result.second = m_access->second(env, container);
                container = nullptr;
            }
            return result;
        }
        QPair<const void*,const void*> next() override {
            if(container){
                QPair<const void*,const void*> result = constNext();
                if(keyType() & AbstractContainerAccess::PointersMask){
                    result.first = *reinterpret_cast<void*const*>(result.first);
                }
                if(valueType() & AbstractContainerAccess::PointersMask){
                    result.second = *reinterpret_cast<void*const*>(result.second);
                }
                return result;
            }else{
                return {nullptr, nullptr};
            }
        }
        bool operator==(const AbstractMapAccess::KeyValueIterator& other) const override {
            return container==reinterpret_cast<const KeyValueIterator&>(other).container;
        }
        std::unique_ptr<AbstractAssociativeAccess::KeyValueIterator> clone() const override {
            return std::unique_ptr<AbstractAssociativeAccess::KeyValueIterator>(new KeyValueIterator(*this));
        }
        bool isConst() override{
            return false;
        }
        QPair<const void*,const void*> constNext() override {
            QPair<const void*,const void*> result;
            if(container){
                result.first = &container;
                const char* snd = reinterpret_cast<const char*>(container);
                result.second = &snd[m_access->m_offset];
                container = nullptr;
            }
            return result;
        }
        QPair<const void*,void*> mutableNext() override {
            QPair<const void*,void*> result;
            if(container){
                result.first = &container;
                char* snd = reinterpret_cast<char*>(container);
                result.second = &snd[m_access->m_offset];
                container = nullptr;
            }
            return result;
        }
    };
    return std::unique_ptr<AbstractHashAccess::KeyValueIterator>(new KeyValueIterator(this, container));
}

std::unique_ptr<AbstractHashAccess::KeyValueIterator> AutoPairAccess::keyValueIterator(const void* container) {
    class KeyValueIterator : public AbstractHashAccess::KeyValueIterator{
        AutoPairAccess* m_access;
        const void* container;
        KeyValueIterator(const KeyValueIterator& other)
            :m_access(other.m_access),
            container(other.container) {}
    protected:
        AbstractAssociativeAccess* access() override {return nullptr;}
    public:
        KeyValueIterator(AutoPairAccess* _access, const void* _container)
            :m_access(_access), container(_container)
        {}
        const QMetaType& keyMetaType() override {return m_access->firstMetaType();}
        const QMetaType& valueMetaType() override {return m_access->secondMetaType();}
        DataType keyType() override {return m_access->firstType();}
        DataType valueType() override {return m_access->secondType();}
        AbstractContainerAccess* keyNestedContainerAccess() override {return m_access->firstNestedContainerAccess();}
        AbstractContainerAccess* valueNestedContainerAccess() override {return m_access->secondNestedContainerAccess();}
        bool hasKeyNestedContainerAccess() override {return m_access->hasFirstNestedContainerAccess();}
        bool hasValueNestedContainerAccess() override {return m_access->hasSecondNestedContainerAccess();}
        bool hasKeyNestedPointers() override {return m_access->hasFirstNestedPointers();}
        bool hasValueNestedPointers() override {return m_access->hasSecondNestedPointers();}
        bool hasNext() override{
            return container;
        }
        QPair<jobject,jobject> next(JNIEnv * env) override{
            QPair<jobject,jobject> result;
            if(container){
                result.first = m_access->first(env, container);
                result.second = m_access->second(env, container);
                container = nullptr;
            }
            return result;
        }
        QPair<const void*,const void*> next() override {
            if(container){
                QPair<const void*,const void*> result = constNext();
                if(keyType() & AbstractContainerAccess::PointersMask){
                    result.first = *reinterpret_cast<void*const*>(result.first);
                }
                if(valueType() & AbstractContainerAccess::PointersMask){
                    result.second = *reinterpret_cast<void*const*>(result.second);
                }
                return result;
            }else{
                return {nullptr, nullptr};
            }
        }
        bool operator==(const AbstractMapAccess::KeyValueIterator& other) const override {
            return container==reinterpret_cast<const KeyValueIterator&>(other).container;
        }
        std::unique_ptr<AbstractAssociativeAccess::KeyValueIterator> clone() const override {
            return std::unique_ptr<AbstractAssociativeAccess::KeyValueIterator>(new KeyValueIterator(*this));
        }
        bool isConst() override{
            return true;
        }
        QPair<const void*,const void*> constNext() override {
            QPair<const void*,const void*> result;
            if(container){
                result.first = &container;
                const char* snd = reinterpret_cast<const char*>(container);
                result.second = &snd[m_access->m_offset];
                container = nullptr;
            }
            return result;
        }
        QPair<const void*,void*> mutableNext() override {
            return {nullptr, nullptr};
        }
    };
    return std::unique_ptr<AbstractHashAccess::KeyValueIterator>(new KeyValueIterator(this, container));
}


