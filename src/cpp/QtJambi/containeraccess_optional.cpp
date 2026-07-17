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

QSharedPointer<class OptionalAccess> getOptionalAccess(const QtPrivate::QMetaTypeInterface *iface){
    return findContainerAccess(QMetaType(iface)).staticCast<OptionalAccess>();
}
void OptionalAccess::defaultCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr){
    if(QSharedPointer<class OptionalAccess> access = getOptionalAccess(iface)){
        access->constructContainer(ptr);
    }
}
void OptionalAccess::copyCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, const void *other){
    if(QSharedPointer<class OptionalAccess> access = getOptionalAccess(iface)){
        access->constructContainer(ptr, other);
    }
}
void OptionalAccess::moveCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, void *other){
    if(QSharedPointer<class OptionalAccess> access = getOptionalAccess(iface)){
        access->constructContainer(ptr, other);
    }
}
void OptionalAccess::dtor(const QtPrivate::QMetaTypeInterface *iface, void *ptr){
    if(QSharedPointer<class OptionalAccess> access = getOptionalAccess(iface)){
        access->destructContainer(ptr);
    }
}
bool OptionalAccess::equalsFn(const QtPrivate::QMetaTypeInterface *iface, const void *ptr1, const void *ptr2){
    if(QSharedPointer<class OptionalAccess> access = getOptionalAccess(iface)){
        return access->equals(ptr1, ptr2);
    }
    return false;
}
void OptionalAccess::debugStreamFn(const QtPrivate::QMetaTypeInterface *iface, QDebug &s, const void *ptr){
    if(QSharedPointer<class OptionalAccess> access = getOptionalAccess(iface)){
        access->debugStream(s, ptr);
    }
}
void OptionalAccess::dataStreamOutFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, const void *ptr){
    if(QSharedPointer<class OptionalAccess> access = getOptionalAccess(iface)){
        access->dataStreamOut(s, ptr);
    }
}
void OptionalAccess::dataStreamInFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, void *ptr){
    if(QSharedPointer<class OptionalAccess> access = getOptionalAccess(iface)){
        access->dataStreamIn(s, ptr);
    }
}

OptionalAccess::OptionalAccess(const OptionalAccess& other)
    : AbstractPairAccess(), AbstractNestedPairAccess(),
      m_keyMetaType(other.m_keyMetaType),
      m_keyHashFunction(other.m_keyHashFunction),
      m_keyInternalToExternalConverter(other.m_keyInternalToExternalConverter),
      m_keyExternalToInternalConverter(other.m_keyExternalToInternalConverter),
      m_keyNestedContainerAccess(other.m_keyNestedContainerAccess),
      m_align(other.m_align),
      m_offset(other.m_offset),
      m_size(other.m_size),
      m_keyOwnerFunction(other.m_keyOwnerFunction),
      m_keyDataType(other.m_keyDataType)
{
}

OptionalAccess::OptionalAccess(
        const QMetaType& keyMetaType,
        const QtJambiUtils::QHashFunction& keyHashFunction,
        const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
        const QtJambiUtils::ExternalToInternalConverter& keyExternalToInternalConverter,
        const QSharedPointer<AbstractContainerAccess>& keyNestedContainerAccess,
        PtrOwnerFunction keyOwnerFunction,
        AbstractContainerAccess::DataType keyDataType)
    :   AbstractPairAccess(), AbstractNestedPairAccess(),
      m_keyMetaType(keyMetaType),
      m_keyHashFunction(keyHashFunction),
      m_keyInternalToExternalConverter(keyInternalToExternalConverter),
      m_keyExternalToInternalConverter(keyExternalToInternalConverter),
      m_keyNestedContainerAccess(keyNestedContainerAccess),
      m_align(qMax<size_t>(m_keyMetaType.alignOf(), alignof(bool))),
      m_offset(0),
      m_size(0),
      m_keyOwnerFunction(keyOwnerFunction),
      m_keyDataType(keyDataType)
{
    Q_ASSERT(m_keyMetaType.id()!=QMetaType::UnknownType
            && m_keyMetaType.id()!=QMetaType::Void);
    m_offset = m_keyMetaType.sizeOf();
    if(m_offset%alignof(bool)>0)
        m_offset += alignof(bool)-m_offset%alignof(bool);
    m_size = m_offset + sizeof(bool);
    if(m_size%m_align>0)
        m_size += m_align-m_size%m_align;
}

AbstractNestedPairAccess* OptionalAccess::asNested() { return this; }

void OptionalAccess::dispose() { delete this; }

OptionalAccess* OptionalAccess::clone(){
    return new OptionalAccess(*this);
}

size_t OptionalAccess::sizeOf() const {
    return m_size;
}

size_t OptionalAccess::alignOf() const {
    return m_align;
}

void* OptionalAccess::constructContainer(JNIEnv*, void* result, const ConstContainerAndAccessInfo& container) {
    return constructContainer(result, container.container);
}

void* OptionalAccess::constructContainer(void* result) {
    *reinterpret_cast<bool*>(reinterpret_cast<char*>(result)+m_offset) = false;
    return result;
}

void* OptionalAccess::constructContainer(void* result, const void* container) {
    bool hasValue = *reinterpret_cast<const bool*>(reinterpret_cast<const char*>(container)+m_offset);
    *reinterpret_cast<bool*>(reinterpret_cast<char*>(result)+m_offset) = hasValue;
    if(hasValue){
        m_keyMetaType.construct(result, container);
    }
    return result;
}

void* OptionalAccess::constructContainer(JNIEnv*, void* result, const ContainerAndAccessInfo& container) {
    return constructContainer(result, container.container);
}

void* OptionalAccess::constructContainer(void* result, void* container) {
    bool hasValue = *reinterpret_cast<const bool*>(reinterpret_cast<const char*>(container)+m_offset);
    *reinterpret_cast<bool*>(reinterpret_cast<char*>(result)+m_offset) = hasValue;
    if(hasValue){
        const QtPrivate::QMetaTypeInterface *iface = m_keyMetaType.iface();
        if(iface->moveCtr)
            iface->moveCtr(iface, result, container);
        else
            memcpy(result, container, iface->size);
    }
    return result;
}

bool OptionalAccess::equals(const void* p1, const void* p2) {
    bool hasValue1 = *reinterpret_cast<const bool*>(reinterpret_cast<const char*>(p1)+m_offset);
    bool hasValue2 = *reinterpret_cast<const bool*>(reinterpret_cast<const char*>(p2)+m_offset);
    if(!hasValue1 && !hasValue2){
        return true;
    }else if(hasValue1 && hasValue2){
        if(m_keyMetaType.equals(p1, p2))
            return true;
    }
    return false;
}

void OptionalAccess::debugStream(QDebug &s, const void *ptr){
    bool hasValue = *reinterpret_cast<const bool*>(reinterpret_cast<const char*>(ptr)+m_offset);
    if(hasValue){
        const QtPrivate::QMetaTypeInterface *iface = m_keyMetaType.iface();
        s = s.nospace().noquote();
        s << "std::optional(";
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
        s << ")";
    }else{
        s = s.nospace().noquote();
        s << "std::nullopt()";
    }
}

void OptionalAccess::dataStreamOut(QDataStream &s, const void *ptr){
    bool hasValue = *reinterpret_cast<const bool*>(reinterpret_cast<const char*>(ptr)+m_offset);
    s << hasValue;
    if(hasValue){
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
    }
}

void OptionalAccess::dataStreamIn(QDataStream &s, void *ptr){
    const QtPrivate::QMetaTypeInterface *iface = m_keyMetaType.iface();
    bool& hasValue = *reinterpret_cast<bool*>(reinterpret_cast<char*>(ptr)+m_offset);
    if(hasValue)
        m_keyMetaType.destruct(ptr);
    s >> hasValue;
    if(hasValue){
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
    }
}

void OptionalAccess::assign(JNIEnv *, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) {
    assign(container.container, other.container);
}

void OptionalAccess::assign(void* container, const void* other) {
    if(other){
        bool& hasValue1 = *reinterpret_cast<bool*>(reinterpret_cast<char*>(container)+m_offset);
        bool hasValue2 = *reinterpret_cast<const bool*>(reinterpret_cast<const char*>(other)+m_offset);
        if(hasValue1)
            m_keyMetaType.destruct(container);
        if(hasValue2)
            m_keyMetaType.construct(container, other);
        hasValue1 = hasValue2;
    }
}

void OptionalAccess::swap(JNIEnv *, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    if(container2.container){
        bool& hasValue1 = *reinterpret_cast<bool*>(reinterpret_cast<char*>(container.container)+m_offset);
        bool& hasValue2 = *reinterpret_cast<bool*>(reinterpret_cast<char*>(container2.container)+m_offset);
        auto kiface = m_keyMetaType.iface();
        if(hasValue1 && hasValue2){
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
        }else if(hasValue1!=hasValue2){
            if(hasValue1){
                m_keyMetaType.construct(container2.container, container.container);
                m_keyMetaType.destruct(container.container);
            }else{
                m_keyMetaType.construct(container.container, container2.container);
                m_keyMetaType.destruct(container2.container);
            }
            std::swap(hasValue1, hasValue2);
        }
    }
}

bool OptionalAccess::destructContainer(void* container) {
    bool& hasValue = *reinterpret_cast<bool*>(reinterpret_cast<char*>(container)+m_offset);
    if(hasValue)
        m_keyMetaType.destruct(container);
    hasValue = false;
    return true;
}

QMetaType OptionalAccess::registerContainer(QByteArrayView typeName) {
    QMetaType newMetaType = QMetaType::fromName(typeName);
    if(!newMetaType.isValid()){
        QSharedPointer<OptionalAccess> access(new OptionalAccess(*this), &containerDisposer);
        auto kiface = m_keyMetaType.iface();
        newMetaType = registerContainerMetaType(typeName,
                                       (kiface->defaultCtr || !(kiface->flags & QMetaType::NeedsConstruction)) ? OptionalAccess::defaultCtr : nullptr,
                                       (kiface->copyCtr || !(kiface->flags & QMetaType::NeedsConstruction)) ? OptionalAccess::copyCtr : nullptr,
                                       (kiface->moveCtr || !(kiface->flags & QMetaType::NeedsConstruction)) ? OptionalAccess::moveCtr : nullptr,
                                       OptionalAccess::dtor,
                                       (kiface->equals
                                                    || (kiface->flags & QMetaType::IsPointer)
                                                    || (kiface->flags & QMetaType::IsEnumeration)) ? OptionalAccess::equalsFn : nullptr,
                                       nullptr,
                                       (kiface->debugStream
                                                    || (kiface->flags & QMetaType::IsPointer)
                                                    || (kiface->flags & QMetaType::IsEnumeration)) ? OptionalAccess::debugStreamFn : nullptr,
                                       (kiface->dataStreamOut
                                                    || (kiface->flags & QMetaType::IsEnumeration)) ? OptionalAccess::dataStreamOutFn : nullptr,
                                       (kiface->dataStreamIn
                                                    || (kiface->flags & QMetaType::IsEnumeration)) ? OptionalAccess::dataStreamInFn : nullptr,
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
        if(m_keyHashFunction){
            QtJambiUtils::QHashFunction keyHash = m_keyHashFunction;
            size_t offset = m_offset;
            insertHashFunctionByMetaType(newMetaType.iface(),
                                            [offset, keyHash]
                                            (const void* ptr, size_t seed)->size_t{
                                                if(ptr){
                                                    size_t pairSeed = seed;
                                                    bool hasValue = *reinterpret_cast<const bool*>(reinterpret_cast<const char*>(ptr)+offset);
                                                    pairSeed = pairSeed ^ (qHash(hasValue, 0) + 0x9e3779b9 + (pairSeed << 6) + (pairSeed >> 2));
                                                    if(hasValue)
                                                        pairSeed = pairSeed ^ (keyHash(ptr, 0) + 0x9e3779b9 + (pairSeed << 6) + (pairSeed >> 2));
                                                    return pairSeed;
                                                }else{
                                                    return 0;
                                                }
                                            });
        }
    }else{
        registerContainerAccess(newMetaType, this);
    }
    return newMetaType;
}

bool OptionalAccess::hasValue(const void* container){
    return *reinterpret_cast<const bool*>(reinterpret_cast<const char*>(container)+m_offset);
}

jobject OptionalAccess::first(JNIEnv * env, const void* container) {
    if(*reinterpret_cast<const bool*>(reinterpret_cast<const char*>(container)+m_offset)){
        jvalue _first;
        _first.l = nullptr;
        if(m_keyInternalToExternalConverter(env, nullptr, container, _first, true)){
            return _first.l;
        }
    }
    return nullptr;
}

void OptionalAccess::setFirst(JNIEnv *env, void* container, jobject value) {
    bool& hasValue = *reinterpret_cast<bool*>(reinterpret_cast<char*>(container)+m_offset);
    if(value){
        if(!hasValue){
            m_keyMetaType.construct(container);
            hasValue = true;
        }
        jvalue jv;
        jv.l = value;
        m_keyExternalToInternalConverter(env, nullptr, jv, container, jValueType::l);
    }else if(hasValue){
        m_keyMetaType.destruct(container);
        hasValue = false;
    }
}

const void* OptionalAccess::first(const void* container) {
    return *reinterpret_cast<const bool*>(reinterpret_cast<const char*>(container)+m_offset) ? container : nullptr;
}

void* OptionalAccess::first(void* container) {
    return *reinterpret_cast<const bool*>(reinterpret_cast<const char*>(container)+m_offset) ? container : nullptr;
}

void OptionalAccess::setFirst(void* container, const void* value) {
    bool& hasValue = *reinterpret_cast<bool*>(reinterpret_cast<char*>(container)+m_offset);
    if(value){
        if(!hasValue){
            m_keyMetaType.construct(container);
            hasValue = true;
        }
        m_keyMetaType.construct(container, value);
    }else if(hasValue){
        m_keyMetaType.destruct(container);
        hasValue = false;
    }
}

const void* OptionalAccess::second(const void*) {
    return nullptr;
}

void* OptionalAccess::second(void*) {
    return nullptr;
}

void OptionalAccess::setSecond(void*, const void*) {
}

jobject OptionalAccess::second(JNIEnv *, const void*) {
    return nullptr;
}

void OptionalAccess::setSecond(JNIEnv *, void*, jobject) {
}

AbstractContainerAccess::DataType OptionalAccess::firstType() {
    return m_keyDataType;
}

AbstractContainerAccess::DataType OptionalAccess::secondType() {
    return AbstractContainerAccess::Value;
}

QPair<const void*,const void*> OptionalAccess::elements(const void* container) {
    const void* key = container;
    const void* value = reinterpret_cast<const char*>(container)+m_offset;
    if(!*reinterpret_cast<const bool*>(value)){
        key = nullptr;
    }else if(m_keyDataType & AbstractContainerAccess::PointersMask){
        key = *reinterpret_cast<void*const*>(key);
    }
    return {key, value};
}

const QMetaType& OptionalAccess::firstMetaType() {return m_keyMetaType;}
const QMetaType& OptionalAccess::secondMetaType() {
    static QMetaType valueMetaType(QMetaType::Bool);
    return valueMetaType;
}

AbstractContainerAccess* OptionalAccess::firstNestedContainerAccess() {
    return m_keyNestedContainerAccess ? m_keyNestedContainerAccess->clone() : nullptr;
}

AbstractContainerAccess* OptionalAccess::secondNestedContainerAccess() {
    return nullptr;
}

const QSharedPointer<AbstractContainerAccess>& OptionalAccess::sharedFirstNestedContainerAccess(){
    return m_keyNestedContainerAccess;
}
const QSharedPointer<AbstractContainerAccess>& OptionalAccess::sharedSecondNestedContainerAccess(){
    static QSharedPointer<AbstractContainerAccess> valueNestedContainerAccess;
    return valueNestedContainerAccess;
}
bool OptionalAccess::hasFirstNestedContainerAccess() {
    return !m_keyNestedContainerAccess.isNull();
}
bool OptionalAccess::hasFirstNestedPointers() {
    if(hasFirstNestedContainerAccess()){
        if(m_keyNestedContainerAccess->isSequential()){
            auto daccess = static_cast<AbstractSequentialAccess*>(m_keyNestedContainerAccess.data());
            return (daccess->elementType() & PointersMask) || daccess->hasNestedPointers();
        }else if(m_keyNestedContainerAccess->isAssociative()){
            auto daccess = static_cast<AbstractAssociativeAccess*>(m_keyNestedContainerAccess.data());
            return (daccess->keyType() & PointersMask) || daccess->hasKeyNestedPointers() || (daccess->valueType() & PointersMask) || daccess->hasValueNestedPointers();
        }else if(m_keyNestedContainerAccess->isPair()){
            auto daccess = static_cast<AbstractPairAccess*>(m_keyNestedContainerAccess.data());
            return (daccess->firstType() & PointersMask) || daccess->hasFirstNestedPointers() || (daccess->secondType() & PointersMask) || daccess->hasSecondNestedPointers();
        }
    }
    return false;
}
bool OptionalAccess::hasSecondNestedContainerAccess() {
    return false;
}
bool OptionalAccess::hasSecondNestedPointers() {
    return false;
}

const QObject* OptionalAccess::getOwner(const void* container){
    if(hasOwnerFunction()){
        auto el = elements(container);
        if(el.first){
            if(m_keyOwnerFunction){
                if(const QObject* owner = m_keyOwnerFunction(el.first))
                    return owner;
            }else if(m_keyNestedContainerAccess){
                if(const QObject* owner = m_keyNestedContainerAccess->getOwner(el.first))
                    return owner;
            }
        }
    }
    return nullptr;
}

bool OptionalAccess::hasOwnerFunction(){
    if(m_keyOwnerFunction && !(firstType() & PointersMask))
        return true;
    if(!(firstType() & PointersMask) && m_keyNestedContainerAccess && m_keyNestedContainerAccess->hasOwnerFunction())
        return true;
    return false;
}

std::unique_ptr<AbstractSequentialAccess::ElementIterator> OptionalAccess::elementIterator(const void* container) {
    class ElementIterator : public AbstractSequentialAccess::ElementIterator{
        OptionalAccess* m_access;
        const void* container;
        bool hasValue;
        ElementIterator(const ElementIterator& other)
            :m_access(other.m_access),
            container(other.container),
            hasValue(m_access->hasValue(container)) {}
    protected:
        AbstractSequentialAccess* access() override { return nullptr; }
    public:
        ElementIterator(OptionalAccess* _access, const void* p)
            : AbstractListAccess::ElementIterator(),
            m_access(_access),
            container(p),
            hasValue(m_access->hasValue(container))
        {
        }
        const QMetaType& elementMetaType() override {
            return m_access->firstMetaType();
        }
        DataType elementType() override {
            return m_access->firstType();
        }
        AbstractContainerAccess* elementNestedContainerAccess() override {
            return m_access->firstNestedContainerAccess();
        }
        bool hasNestedContainerAccess() override {
            return elementNestedContainerAccess();
        }
        bool hasNestedPointers() override {
            return m_access->hasFirstNestedPointers();
        }
        bool hasNext() override {return hasValue; };
        bool isConst() override{
            return true;
        }
        jobject next(JNIEnv * env) override{
            if(hasValue){
                hasValue = false;
                return m_access->first(env, container);
            }else{
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
            if(hasValue){
                hasValue = false;
                return container;
            }else{
                return nullptr;
            }
        }
        void* mutableNext() override {
            return nullptr;
        }
        bool operator==(const AbstractSequentialAccess::ElementIterator& other) const override {
            return container==reinterpret_cast<const ElementIterator&>(other).container && hasValue==reinterpret_cast<const ElementIterator&>(other).hasValue;
        }
        std::unique_ptr<AbstractSequentialAccess::ElementIterator> clone() const override {
            return std::unique_ptr<AbstractSequentialAccess::ElementIterator>(new ElementIterator(*this));
        }
        std::function<jobject(JNIEnv*,const void*)> elementConverter() const override {
            if(m_access->m_keyDataType & AbstractContainerAccess::PointersMask){
                return [internalToExternalConverter = m_access->m_keyInternalToExternalConverter](JNIEnv* env,const void* pointer) -> jobject{
                    jvalue _value;
                    _value.l = nullptr;
                    internalToExternalConverter(env, nullptr, &pointer, _value, true);
                    return _value.l;
                };
            }else if (JObjectValueWrapper::isValueType(m_access->m_keyMetaType)
                       || isNativeWrapperMetaType(m_access->m_keyMetaType)
                       || isJObjectWrappedMetaType(m_access->m_keyMetaType)) {
                return [](JNIEnv* env,const void* pointer) -> jobject{
                    return reinterpret_cast<const JObjectWrapper*>(pointer)->object(env);
                };
            }else{
                return [internalToExternalConverter = m_access->m_keyInternalToExternalConverter](JNIEnv* env,const void* pointer) -> jobject{
                    jvalue _value;
                    _value.l = nullptr;
                    internalToExternalConverter(env, nullptr, pointer, _value, true);
                    return _value.l;
                };
            }
        }
    };
    return std::unique_ptr<AbstractSequentialAccess::ElementIterator>(new ElementIterator(this, container));
}

std::unique_ptr<AbstractSequentialAccess::ElementIterator> OptionalAccess::elementIterator(void* container) {
    class ElementIterator : public AbstractSequentialAccess::ElementIterator{
        OptionalAccess* m_access;
        void* container;
        bool hasValue;
        ElementIterator(const ElementIterator& other)
            :m_access(other.m_access),
            container(other.container),
            hasValue(m_access->hasValue(container)) {}
    protected:
        AbstractSequentialAccess* access() override { return nullptr; }
    public:
        ElementIterator(OptionalAccess* _access, void* p)
            : AbstractListAccess::ElementIterator(),
            m_access(_access),
            container(p),
            hasValue(m_access->hasValue(container))
        {
        }
        const QMetaType& elementMetaType() override {
            return m_access->firstMetaType();
        }
        DataType elementType() override {
            return m_access->firstType();
        }
        AbstractContainerAccess* elementNestedContainerAccess() override {
            return m_access->firstNestedContainerAccess();
        }
        bool hasNestedContainerAccess() override {
            return elementNestedContainerAccess();
        }
        bool hasNestedPointers() override {
            return m_access->hasFirstNestedPointers();
        }
        bool hasNext() override {return hasValue;};
        bool isConst() override{
            return false;
        }
        jobject next(JNIEnv * env) override{
            if(hasValue){
                hasValue = false;
                return m_access->first(env, container);
            }else{
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
            if(hasValue){
                hasValue = false;
                return container;
            }else{
                return nullptr;
            }
        }
        void* mutableNext() override {
            if(hasValue){
                hasValue = false;
                return container;
            }else{
                return nullptr;
            }
        }
        bool operator==(const AbstractSequentialAccess::ElementIterator& other) const override {
            return container==reinterpret_cast<const ElementIterator&>(other).container && hasValue==reinterpret_cast<const ElementIterator&>(other).hasValue;
        }
        std::unique_ptr<AbstractSequentialAccess::ElementIterator> clone() const override {
            return std::unique_ptr<AbstractSequentialAccess::ElementIterator>(new ElementIterator(*this));
        }
        std::function<jobject(JNIEnv*,const void*)> elementConverter() const override {
            if(m_access->m_keyDataType & AbstractContainerAccess::PointersMask){
                return [internalToExternalConverter = m_access->m_keyInternalToExternalConverter](JNIEnv* env,const void* pointer) -> jobject{
                    jvalue _value;
                    _value.l = nullptr;
                    internalToExternalConverter(env, nullptr, &pointer, _value, true);
                    return _value.l;
                };
            }else if (JObjectValueWrapper::isValueType(m_access->m_keyMetaType)
                       || isNativeWrapperMetaType(m_access->m_keyMetaType)
                       || isJObjectWrappedMetaType(m_access->m_keyMetaType)) {
                return [](JNIEnv* env,const void* pointer) -> jobject{
                    return reinterpret_cast<const JObjectWrapper*>(pointer)->object(env);
                };
            }else{
                return [internalToExternalConverter = m_access->m_keyInternalToExternalConverter](JNIEnv* env,const void* pointer) -> jobject{
                    jvalue _value;
                    _value.l = nullptr;
                    internalToExternalConverter(env, nullptr, pointer, _value, true);
                    return _value.l;
                };
            }
        }
    };
    return std::unique_ptr<AbstractSequentialAccess::ElementIterator>(new ElementIterator(this, container));
}

std::unique_ptr<AbstractHashAccess::KeyValueIterator> OptionalAccess::keyValueIterator(void* container) {
    class KeyValueIterator : public AbstractHashAccess::KeyValueIterator{
        OptionalAccess* m_access;
        void* container;
        KeyValueIterator(const KeyValueIterator& other)
            :m_access(other.m_access),
            container(other.container) {}
    protected:
        AbstractAssociativeAccess* access() override {return nullptr;}
    public:
        KeyValueIterator(OptionalAccess* _access, void* _container)
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
        std::function<jobject(JNIEnv*,const void*)> keyConverter() const override {
            if(m_access->m_keyDataType & AbstractContainerAccess::PointersMask){
                return [internalToExternalConverter = m_access->m_keyInternalToExternalConverter](JNIEnv* env,const void* pointer) -> jobject{
                    jvalue _value;
                    _value.l = nullptr;
                    internalToExternalConverter(env, nullptr, &pointer, _value, true);
                    return _value.l;
                };
            }else if (JObjectValueWrapper::isValueType(m_access->m_keyMetaType)
                       || isNativeWrapperMetaType(m_access->m_keyMetaType)
                       || isJObjectWrappedMetaType(m_access->m_keyMetaType)) {
                return [](JNIEnv* env,const void* pointer) -> jobject{
                    return reinterpret_cast<const JObjectWrapper*>(pointer)->object(env);
                };
            }else{
                return [internalToExternalConverter = m_access->m_keyInternalToExternalConverter](JNIEnv* env,const void* pointer) -> jobject{
                    jvalue _value;
                    _value.l = nullptr;
                    internalToExternalConverter(env, nullptr, pointer, _value, true);
                    return _value.l;
                };
            }
        }
        std::function<jobject(JNIEnv*,const void*)> valueConverter() const override {
            return [](JNIEnv* env,const void* pointer) -> jobject{
                return Java::Runtime::Boolean::valueOf(env, *reinterpret_cast<const bool*>(pointer));
            };
        }
    };
    return std::unique_ptr<AbstractHashAccess::KeyValueIterator>(new KeyValueIterator(this, container));
}

std::unique_ptr<AbstractHashAccess::KeyValueIterator> OptionalAccess::keyValueIterator(const void* container) {
    class KeyValueIterator : public AbstractHashAccess::KeyValueIterator{
        OptionalAccess* m_access;
        const void* container;
        KeyValueIterator(const KeyValueIterator& other)
            :m_access(other.m_access),
            container(other.container) {}
    protected:
        AbstractAssociativeAccess* access() override {return nullptr;}
    public:
        KeyValueIterator(OptionalAccess* _access, const void* _container)
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
        std::function<jobject(JNIEnv*,const void*)> keyConverter() const override {
            if(m_access->m_keyDataType & AbstractContainerAccess::PointersMask){
                return [internalToExternalConverter = m_access->m_keyInternalToExternalConverter](JNIEnv* env,const void* pointer) -> jobject{
                    jvalue _value;
                    _value.l = nullptr;
                    internalToExternalConverter(env, nullptr, &pointer, _value, true);
                    return _value.l;
                };
            }else if (JObjectValueWrapper::isValueType(m_access->m_keyMetaType)
                       || isNativeWrapperMetaType(m_access->m_keyMetaType)
                       || isJObjectWrappedMetaType(m_access->m_keyMetaType)) {
                return [](JNIEnv* env,const void* pointer) -> jobject{
                    return reinterpret_cast<const JObjectWrapper*>(pointer)->object(env);
                };
            }else{
                return [internalToExternalConverter = m_access->m_keyInternalToExternalConverter](JNIEnv* env,const void* pointer) -> jobject{
                    jvalue _value;
                    _value.l = nullptr;
                    internalToExternalConverter(env, nullptr, pointer, _value, true);
                    return _value.l;
                };
            }
        }
        std::function<jobject(JNIEnv*,const void*)> valueConverter() const override {
            return [](JNIEnv* env,const void* pointer) -> jobject{
                return Java::Runtime::Boolean::valueOf(env, *reinterpret_cast<const bool*>(pointer));
            };
        }
    };
    return std::unique_ptr<AbstractHashAccess::KeyValueIterator>(new KeyValueIterator(this, container));
}


