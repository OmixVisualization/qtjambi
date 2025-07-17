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

#include "qtjambiapi.h"
#include <QtCore/QReadWriteLock>
#include <QtCore/QMap>
#include <QtCore/QSharedPointer>
#include <QtCore/QSequentialIterable>
#include "containeraccess_p.h"
#include "functionpointer.h"
#include "registryapi.h"
#include "registryutil_p.h"
#include "qtjambilink_p.h"
#include "java_p.h"
#include "coreapi.h"

QT_WARNING_DISABLE_GCC("-Winaccessible-base")
QT_WARNING_DISABLE_CLANG("-Winaccessible-base")

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)

AutoSpanAccess::AutoSpanAccess(
        const QMetaType& elementMetaType,
        const QtJambiUtils::QHashFunction& hashFunction,
        const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
        const QtJambiUtils::ExternalToInternalConverter& externalToInternalConverter,
        const QSharedPointer<AbstractContainerAccess>& elementNestedContainerAccess,
        PtrOwnerFunction elementOwnerFunction,
        AbstractContainerAccess::DataType elementDataType,
        bool isConst
        )
    : AbstractSpanAccess(), AbstractNestedSequentialAccess(),
      m_elementMetaType(elementMetaType),
      m_hashFunction(hashFunction),
      m_internalToExternalConverter(internalToExternalConverter),
      m_externalToInternalConverter(externalToInternalConverter),
      m_elementNestedContainerAccess(elementNestedContainerAccess),
      m_offset(0),
      m_elementOwnerFunction(elementOwnerFunction),
      m_elementDataType(elementDataType),
      m_isConst(isConst)
{
    Q_ASSERT(m_elementMetaType.id()!=QMetaType::UnknownType
            && m_elementMetaType.id()!=QMetaType::Void);
    m_offset = m_elementMetaType.sizeOf();
    if(m_offset%m_elementMetaType.alignOf()>0)
        m_offset += m_elementMetaType.alignOf()-m_offset%m_elementMetaType.alignOf();
}

AutoSpanAccess::AutoSpanAccess(const AutoSpanAccess& other)
  : AbstractSpanAccess(), AbstractNestedSequentialAccess(),
    m_elementMetaType(other.m_elementMetaType),
    m_hashFunction(other.m_hashFunction),
    m_internalToExternalConverter(other.m_internalToExternalConverter),
    m_externalToInternalConverter(other.m_externalToInternalConverter),
    m_elementNestedContainerAccess(other.m_elementNestedContainerAccess),
    m_offset(other.m_offset),
    m_elementOwnerFunction(other.m_elementOwnerFunction),
    m_elementDataType(other.m_elementDataType),
    m_isConst(other.m_isConst)
{
}

void AutoSpanAccess::dispose(){
    delete this;
}

std::unique_ptr<AbstractSpanAccess::ElementIterator> AutoSpanAccess::elementIterator(const void* container) {
    class ElementIterator : public AbstractSpanAccess::ElementIterator{
        AutoSpanAccess* m_access;
        const char* current;
        const char* end;
        ElementIterator(const ElementIterator& other)
            :m_access(other.m_access),
            current(other.current),
            end(other.end) {}
    protected:
        AbstractSequentialAccess* access() override { return m_access; }
    public:
        ElementIterator(AutoSpanAccess* _access, const QtJambiSpan* p)
            : AbstractSpanAccess::ElementIterator(),
            m_access(_access),
            current(reinterpret_cast<const char*>(p->begin)),
            end(current + p->size * m_access->m_offset)
        {
        }
        bool hasNext() override{
            return current!=end;
        }
        jobject next(JNIEnv * env) override{
            const void* data = current;
            current += m_access->m_offset;
            jvalue _value;
            _value.l = nullptr;
            m_access->m_internalToExternalConverter(env, nullptr, data, _value, true);
            return _value.l;
        }
        const void* next() override {
            const void* data = current;
            current += m_access->m_offset;
            if(m_access->m_elementDataType & AbstractContainerAccess::PointersMask){
                return *reinterpret_cast<void*const*>(data);
            }else{
                return data;
            }
        }
        bool isConst() override{
            return false;
        }
        const void* constNext() override {
            const void* data = current;
            current += m_access->m_offset;
            return data;
        }
        void* mutableNext() override {
            return nullptr;
        }
        bool operator==(const AbstractSequentialAccess::ElementIterator& other) const override {
            return current==reinterpret_cast<const ElementIterator&>(other).current;
        }
        std::unique_ptr<AbstractSequentialAccess::ElementIterator> clone() const override {
            return std::unique_ptr<AbstractSequentialAccess::ElementIterator>(new ElementIterator(*this));
        }
    };
    return std::unique_ptr<AbstractSpanAccess::ElementIterator>(new ElementIterator(this, reinterpret_cast<const QtJambiSpan*>(container)));
}

std::unique_ptr<AbstractSpanAccess::ElementIterator> AutoSpanAccess::elementIterator(void* container) {
    class ElementIterator : public AbstractSpanAccess::ElementIterator{
        AutoSpanAccess* m_access;
        const char* current;
        const char* end;
        ElementIterator(const ElementIterator& other)
            :m_access(other.m_access),
            current(other.current),
            end(other.end) {}
    protected:
        AbstractSequentialAccess* access() override { return m_access; }
    public:
        ElementIterator(AutoSpanAccess* _access, QtJambiSpan* p)
            : AbstractSpanAccess::ElementIterator(),
            m_access(_access),
            current(reinterpret_cast<const char*>(p->begin)),
            end(current + p->size * m_access->m_offset)
        {
        }
        bool hasNext() override{
            return current!=end;
        }
        jobject next(JNIEnv * env) override{
            const void* data = current;
            current += m_access->m_offset;
            jvalue _value;
            _value.l = nullptr;
            m_access->m_internalToExternalConverter(env, nullptr, data, _value, true);
            return _value.l;
        }
        const void* next() override {
            const void* data = current;
            current += m_access->m_offset;
            if(m_access->m_elementDataType & AbstractContainerAccess::PointersMask){
                return *reinterpret_cast<void*const*>(data);
            }else{
                return data;
            }
        }
        const void* constNext() override {
            const void* data = current;
            current += m_access->m_offset;
            return data;
        }
        bool isConst() override{
            return m_access->isConst();
        }
        void* mutableNext() override {
            if(!m_access->isConst()){
                void* data = const_cast<char*>(current);
                current += m_access->m_offset;
                return data;
            }else return nullptr;
        }
        bool operator==(const AbstractSequentialAccess::ElementIterator& other) const override {
            return current==reinterpret_cast<const ElementIterator&>(other).current;
        }
        std::unique_ptr<AbstractSequentialAccess::ElementIterator> clone() const override {
            return std::unique_ptr<AbstractSequentialAccess::ElementIterator>(new ElementIterator(*this));
        }
    };
    return std::unique_ptr<AbstractSpanAccess::ElementIterator>(new ElementIterator(this, reinterpret_cast<QtJambiSpan*>(container)));
}

AutoSpanAccess* AutoSpanAccess::clone(){
    return new AutoSpanAccess(*this);
}

size_t AutoSpanAccess::sizeOf() {
    return sizeof(QtJambiSpan);
}

void* AutoSpanAccess::constructContainer(JNIEnv*, void* result, const ConstContainerAndAccessInfo& container) {
    return constructContainer(result, container.container);
}

void* AutoSpanAccess::constructContainer(void* result){
    return new(result) QtJambiSpan();
}

void* AutoSpanAccess::constructContainer(void* result, const void* container){
    result = constructContainer(result);
    assign(result, container);
    return result;
}

bool AutoSpanAccess::destructContainer(void* container) {
    reinterpret_cast<QtJambiSpan*>(container)->~QtJambiSpan();
    return true;
}

void* AutoSpanAccess::constructContainer(JNIEnv*, void* result, const ContainerAndAccessInfo& container) {
    return constructContainer(result, container.container);
}

void* AutoSpanAccess::constructContainer(void* result, void* container){
    return new(result) QList<char>(std::move(*reinterpret_cast<QList<char>*>(container)));
}

bool AutoSpanAccess::isConst(){
    return m_isConst;
}

bool AutoSpanAccess::equals(const void* container1, const void* container2)
{
    const QtJambiSpan* p1 = reinterpret_cast<const QtJambiSpan*>(container1);
    const QtJambiSpan* p2 = reinterpret_cast<const QtJambiSpan*>(container2);
    if(p1->size!=p2->size)
        return false;
    if(p1->begin==p2->begin)
        return true;
    return true;
}

void AutoSpanAccess::debugStream(QDebug &dbg, const void *ptr)
{
    const QtJambiSpan* span = reinterpret_cast<const QtJambiSpan*>(ptr);
    if(span->size && span->begin)
        dbg << QString::asprintf("QSpan<%s%s>(%p,%lld)", isConst() ? "const " : "", elementMetaType().name(), span->begin, quint64(span->size));
    else
        dbg << QString::asprintf("QSpan<%s%s>()", isConst() ? "const " : "", elementMetaType().name());
}

AutoSpanAccess::iterator AutoSpanAccess::begin(const void* container) {
    const QtJambiSpan* p = reinterpret_cast<const QtJambiSpan*>(container);
    return iterator(m_offset, reinterpret_cast<char*>(const_cast<void*>(p->begin)));
}

AutoSpanAccess::iterator AutoSpanAccess::end(const void* container) {
    const QtJambiSpan* p = reinterpret_cast<const QtJambiSpan*>(container);
    return iterator(m_offset, reinterpret_cast<char*>(const_cast<void*>(p->begin)) + p->size * m_offset);
}

qsizetype AutoSpanAccess::size(const void* container)
{
    const QtJambiSpan* p = reinterpret_cast<const QtJambiSpan*>(container);
    return p->size;
}

AutoSpanAccess::iterator::iterator(size_t _offset, char* _ptr)
    : offset(_offset), ptr(_ptr){
}

bool AutoSpanAccess::iterator::operator<(const iterator& right) const{
    return right.ptr<ptr;
}
bool AutoSpanAccess::iterator::operator==(const iterator& right) const{
    return right.ptr==ptr;
}
const char* AutoSpanAccess::iterator::operator->() const{
    return ptr;
}
const char& AutoSpanAccess::iterator::operator*() const{
    return *ptr;
}
const char* AutoSpanAccess::iterator::data() const{
    return ptr;
}
char* AutoSpanAccess::iterator::operator->(){
    return ptr;
}
char& AutoSpanAccess::iterator::operator*(){
    return *ptr;
}
char* AutoSpanAccess::iterator::data(){
    return ptr;
}

AutoSpanAccess::iterator& AutoSpanAccess::iterator::operator++(){
    ptr += offset;
    return *this;
}

AutoSpanAccess::iterator AutoSpanAccess::iterator::operator++(int){
    iterator _this = *this;
    ptr += offset;
    return _this;
}

AutoSpanAccess::iterator& AutoSpanAccess::iterator::operator--(){
    ptr -= offset;
    return *this;
}

AutoSpanAccess::iterator AutoSpanAccess::iterator::operator--(int){
    iterator _this = *this;
    ptr -= offset;
    return _this;
}

typedef QMap<int, QtMetaContainerPrivate::QMetaSequenceInterface> MetaSequenceHash;

QReadWriteLock* containerAccessLock();
QMap<int, QtMetaContainerPrivate::QMetaSequenceInterface>& metaSequenceHash();

QSharedPointer<class AutoSpanAccess> getSpanAccess(const QtPrivate::QMetaTypeInterface *iface){
    return findContainerAccess(iface->typeId.loadAcquire()).dynamicCast<AutoSpanAccess>();
}
void AutoSpanAccess::defaultCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr){
    if(QSharedPointer<class AutoSpanAccess> access = getSpanAccess(iface)){
        access->constructContainer(ptr);
    }
}
void AutoSpanAccess::copyCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, const void *other){
    if(QSharedPointer<class AutoSpanAccess> access = getSpanAccess(iface)){
        access->constructContainer(ptr, other);
    }
}
void AutoSpanAccess::moveCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, void *other){
    if(QSharedPointer<class AutoSpanAccess> access = getSpanAccess(iface)){
        access->constructContainer(ptr, other);
    }
}
void AutoSpanAccess::dtor(const QtPrivate::QMetaTypeInterface *iface, void *ptr){
    if(QSharedPointer<class AutoSpanAccess> access = getSpanAccess(iface)){
        access->destructContainer(ptr);
    }
}
bool AutoSpanAccess::equalsFn(const QtPrivate::QMetaTypeInterface *iface, const void *ptr1, const void *ptr2){
    if(QSharedPointer<class AutoSpanAccess> access = getSpanAccess(iface)){
        return access->equals(ptr1, ptr2);
    }
    return false;
}
void AutoSpanAccess::debugStreamFn(const QtPrivate::QMetaTypeInterface *iface, QDebug &s, const void *ptr){
    if(QSharedPointer<class AutoSpanAccess> access = getSpanAccess(iface)){
        access->debugStream(s, ptr);
    }
}

QtMetaContainerPrivate::QMetaSequenceInterface* AutoSpanAccess::createMetaSequenceInterface(int newMetaType){
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
    QSharedPointer<class AutoSpanAccess> access = getSpanAccess(QMetaType(newMetaType).iface());
    metaSequenceInterface->valueMetaType = access->m_elementMetaType.iface();
    metaSequenceInterface->iteratorCapabilities = InputCapability | ForwardCapability | BiDirectionalCapability;
    metaSequenceInterface->addRemoveCapabilities = CanAddAtBegin|CanRemoveAtBegin|CanAddAtEnd|CanRemoveAtEnd;

    metaSequenceInterface->sizeFn = [](const void *c) -> qsizetype {
        if(!c)
            return 0;
        const QtJambiSpan* p = reinterpret_cast<const QtJambiSpan*>(c);
        return p->size;
    };
    metaSequenceInterface->clearFn = nullptr;
    struct MetaIterator{
        MetaIterator(char* _iterator, const QSharedPointer<class AutoSpanAccess>& _access) : iterator(_iterator), access(_access) {}
        char* iterator;
        QSharedPointer<class AutoSpanAccess> access;
    };
    metaSequenceInterface->createIteratorFn = qtjambi_function_pointer<16,void*(void *,QMetaContainerInterface::Position)>(
                [newMetaType](void *c, QMetaContainerInterface::Position p) -> void* {
                        if(!c)
                            return nullptr;
                        QSharedPointer<class AutoSpanAccess> access = getSpanAccess(QMetaType(newMetaType).iface());
                        if(!access)
                            return nullptr;
                        const QtJambiSpan* ap = reinterpret_cast<const QtJambiSpan*>(c);
                        switch (p) {
                        case QMetaContainerInterface::Unspecified:
                            return new MetaIterator(nullptr, access);
                        case QMetaContainerInterface::AtBegin:
                            return new MetaIterator(reinterpret_cast<char*>(const_cast<void*>(ap->begin)), access);
                            break;
                        case QMetaContainerInterface::AtEnd:
                            return new MetaIterator(reinterpret_cast<char*>(const_cast<void*>(ap->begin)) + ap->size * access->m_offset, access);
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
        QSharedPointer<class AutoSpanAccess> access = getSpanAccess(QMetaType(newMetaType).iface());
        if(!access)
            return;
        const QtJambiSpan* p = reinterpret_cast<const QtJambiSpan*>(container);
        Q_ASSERT_X(index >= 0 && index < p->size, "QSpan<T>::operator[index]", "index out of range");
        void* ptr = reinterpret_cast<char*>(const_cast<void*>(p->begin)) + index * access->m_offset;
        access->m_elementMetaType.destruct(r);
        access->m_elementMetaType.construct(r, ptr);
    }, newMetaType);
    metaSequenceInterface->setValueAtIndexFn = qtjambi_function_pointer<16,void(void *, qsizetype, const void *)>([newMetaType](void *container, qsizetype index, const void *e) {
        if(!container)
            return;
        QSharedPointer<class AutoSpanAccess> access = getSpanAccess(QMetaType(newMetaType).iface());
        if(!access)
            return;
        QtJambiSpan* p = reinterpret_cast<QtJambiSpan*>(container);
        Q_ASSERT_X(index >= 0 && index < p->size, "QList<T>::replace", "index out of range");
        void* target = reinterpret_cast<char*>(const_cast<void*>(p->begin)) + index * access->m_offset;
        access->m_elementMetaType.destruct(target);
        access->m_elementMetaType.construct(target, e);
    }, newMetaType);
    metaSequenceInterface->addValueFn = nullptr;
    metaSequenceInterface->removeValueFn = nullptr;
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

int AutoSpanAccess::registerContainer(const QByteArray& typeName)
{
    int newMetaType = QMetaType::fromName(typeName).id();
    if(newMetaType==QMetaType::UnknownType){
        QSharedPointer<AutoSpanAccess> access(new AutoSpanAccess(*this), &containerDisposer);
        auto iface = m_elementMetaType.iface();
        newMetaType = registerContainerMetaType(typeName,
                                       AutoSpanAccess::defaultCtr,
                                       AutoSpanAccess::copyCtr,
                                       AutoSpanAccess::moveCtr,
                                       AutoSpanAccess::dtor,
                                       (iface->equals
                                            || (iface->flags & QMetaType::IsPointer)
                                            || (iface->flags & QMetaType::IsEnumeration)) ? AutoSpanAccess::equalsFn : nullptr,
                                       nullptr,
                                       (iface->debugStream
                                            || (iface->flags & QMetaType::IsPointer)
                                            || (iface->flags & QMetaType::IsEnumeration)) ? AutoSpanAccess::debugStreamFn : nullptr,
                                       nullptr,
                                       nullptr,
                                       nullptr,
                                       uint(sizeOf()),
                                       ushort(alignof(QtJambiSpan)),
                                       QMetaType::UnknownType,
                                       QMetaType::NeedsConstruction
                                                   | QMetaType::NeedsDestruction
                                                   | QMetaType::RelocatableType,
                                       nullptr,
                                       nullptr,
                                       access);
        if(m_hashFunction){
            insertHashFunctionByMetaType(newMetaType,
                                            [access]
                                            (const void* ptr, hash_type seed)->hash_type{
                                                if(ptr){
                                                    hash_type hashCode = seed;
                                                    const QtJambiSpan* p = reinterpret_cast<const QtJambiSpan*>(ptr);
                                                    for(size_t i=0; i<size_t(p->size); ++i){
                                                        const void* element = reinterpret_cast<const char*>(p->begin) + i * access->m_offset;
                                                        hashCode = hashCode ^ (access->m_hashFunction(element, 0) + 0x9e3779b9 + (hashCode << 6) + (hashCode >> 2));
                                                    }
                                                    return hashCode;
                                                }else{
                                                    return 0;
                                                }
                                            });
        }
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
    }else{
        registerContainerAccess(newMetaType, this);
    }
    return newMetaType;
}

void AutoSpanAccess::assign(JNIEnv *, const ContainerInfo& container, const ConstContainerAndAccessInfo& other){
    assign(container.container, other.container);
}

void AutoSpanAccess::assign(void* container, const void* other)
{
    QtJambiSpan* p = reinterpret_cast<QtJambiSpan*>(container);
    const QtJambiSpan* p2 = reinterpret_cast<const QtJambiSpan*>(other);
    p->begin = p2->begin;
    p->size = p2->size;
}

const QMetaType& AutoSpanAccess::elementMetaType() {return m_elementMetaType;}

AbstractContainerAccess::DataType AutoSpanAccess::elementType() {return m_elementDataType;}

AbstractContainerAccess* AutoSpanAccess::elementNestedContainerAccess() {
    return m_elementNestedContainerAccess ? m_elementNestedContainerAccess->clone() : nullptr;
}
const QSharedPointer<AbstractContainerAccess>& AutoSpanAccess::sharedElementNestedContainerAccess(){
    return m_elementNestedContainerAccess;
}
bool AutoSpanAccess::hasNestedContainerAccess(){
    return !m_elementNestedContainerAccess.isNull();
}
bool AutoSpanAccess::hasNestedPointers(){
    if(hasNestedContainerAccess()){
        if(auto daccess = dynamic_cast<AbstractSequentialAccess*>(m_elementNestedContainerAccess.data())){
            return (daccess->elementType() & PointersMask) || daccess->hasNestedPointers();
        }else if(auto daccess = dynamic_cast<AbstractAssociativeAccess*>(m_elementNestedContainerAccess.data())){
            return (daccess->keyType() & PointersMask) || daccess->hasKeyNestedPointers() || (daccess->valueType() & PointersMask) || daccess->hasValueNestedPointers();
        }else if(auto daccess = dynamic_cast<AbstractPairAccess*>(m_elementNestedContainerAccess.data())){
            return (daccess->firstType() & PointersMask) || daccess->hasFirstNestedPointers() || (daccess->secondType() & PointersMask) || daccess->hasSecondNestedPointers();
        }
    }
    return false;
}

const QObject* AutoSpanAccess::getOwner(const void* container){
    if(hasOwnerFunction() && size(container)>0){
        auto iter = elementIterator(container);
        if(m_elementOwnerFunction){
            while(iter->hasNext()){
                const void* current = iter->next();
                if(const QObject* owner = m_elementOwnerFunction(current))
                    return owner;
            }
        }else if(m_elementNestedContainerAccess){
            while(iter->hasNext()){
                const void* current = iter->next();
                if(const QObject* owner = m_elementNestedContainerAccess->getOwner(current))
                    return owner;
            }
        }
    }
    return nullptr;
}

bool AutoSpanAccess::hasOwnerFunction(){
    if(m_elementOwnerFunction && !(elementType() & PointersMask))
        return true;
    if(!(elementType() & PointersMask) && m_elementNestedContainerAccess && m_elementNestedContainerAccess->hasOwnerFunction())
        return true;
    return false;
}

jobject AutoSpanAccess::createIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr)
{
    AbstractSequentialIteratorAccess* containerAccess = new AutoSequentialIteratorAccess(m_internalToExternalConverter,
            [](AutoSequentialIteratorAccess* containerAccess, void*ptr){
                char* cursor = *reinterpret_cast<char**>(ptr);
                *reinterpret_cast<char**>(ptr) = cursor+containerAccess->offset();
            },
            [](AutoSequentialIteratorAccess* containerAccess, void*ptr){
                char* cursor = *reinterpret_cast<char**>(ptr);
                *reinterpret_cast<char**>(ptr) = cursor-containerAccess->offset();
            },
            [](AutoSequentialIteratorAccess*,const void*ptr)->const void*{
                return *reinterpret_cast<char*const*>(ptr);
            },
            [](AutoSequentialIteratorAccess*,const void*ptr1,const void*ptr2)->bool{
                return *reinterpret_cast<char*const*>(ptr1)<*reinterpret_cast<char*const*>(ptr2);
            },
            [](AutoSequentialIteratorAccess*,const void*ptr1, const void*ptr2)->bool{
                return *reinterpret_cast<char*const*>(ptr1)==*reinterpret_cast<char*const*>(ptr2);
            },
            m_externalToInternalConverter,
            [](AutoSequentialIteratorAccess*,void*ptr)->void*{
                return *reinterpret_cast<void**>(ptr);
            },
            m_elementMetaType,
            m_offset
        );
    return QtJambiAPI::convertQSequentialIteratorToJavaObject(env, ownerId, iteratorPtr, [](void* ptr,bool){
            delete reinterpret_cast<void**>(ptr);
        }, containerAccess);
}

jobject AutoSpanAccess::createConstIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr)
{
    AutoSequentialConstIteratorAccess* containerAccess = new AutoSequentialConstIteratorAccess(m_internalToExternalConverter,
            [](AutoSequentialConstIteratorAccess* containerAccess, void*ptr){
                char* cursor = *reinterpret_cast<char**>(ptr);
                *reinterpret_cast<char**>(ptr) = cursor+containerAccess->offset();
            },
            [](AutoSequentialConstIteratorAccess* containerAccess, void*ptr){
                char* cursor = *reinterpret_cast<char**>(ptr);
                *reinterpret_cast<char**>(ptr) = cursor-containerAccess->offset();
            },
            [](AutoSequentialConstIteratorAccess*,const void*ptr)->const void*{
                return *reinterpret_cast<char*const*>(ptr);
            },
            [](AutoSequentialConstIteratorAccess*,const void*ptr1, const void*ptr2)->bool{
                return *reinterpret_cast<char*const*>(ptr1)<*reinterpret_cast<char*const*>(ptr2);
            },
            [](AutoSequentialConstIteratorAccess*,const void*ptr1, const void*ptr2)->bool{
                return *reinterpret_cast<char*const*>(ptr1)==*reinterpret_cast<char*const*>(ptr2);
            },
            m_elementMetaType,
            m_offset
        );
    return QtJambiAPI::convertQSequentialIteratorToJavaObject(env, ownerId, iteratorPtr, [](void* ptr,bool){
            delete reinterpret_cast<void**>(ptr);
        }, containerAccess);
}

jobject AutoSpanAccess::end(JNIEnv * env, const ExtendedContainerInfo& container)
{
    QtJambiSpan* p = reinterpret_cast<QtJambiSpan*>(container.container);
    void* iteratorPtr = new const char*(reinterpret_cast<const char*>(p->begin) + p->size * m_offset);
    return createIterator(env, container.nativeId, iteratorPtr);
}

jobject AutoSpanAccess::begin(JNIEnv * env, const ExtendedContainerInfo& container)
{
    if(m_isConst)
        return end(env, container);
    QtJambiSpan* p = reinterpret_cast<QtJambiSpan*>(container.container);
    void* iteratorPtr = new const char*(reinterpret_cast<const char*>(p->begin));
    return createIterator(env, container.nativeId, iteratorPtr);
}

jobject AutoSpanAccess::constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container)
{
    const QtJambiSpan* p = reinterpret_cast<const QtJambiSpan*>(container.container);
    void* iteratorPtr = new const char*(reinterpret_cast<const char*>(p->begin) + p->size * m_offset);
    return createConstIterator(env, container.nativeId, iteratorPtr);
}

jobject AutoSpanAccess::constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container)
{
    const QtJambiSpan* p = reinterpret_cast<const QtJambiSpan*>(container.container);
    void* iteratorPtr = new const char*(reinterpret_cast<const char*>(p->begin));
    return createConstIterator(env, container.nativeId, iteratorPtr);
}

jobject AutoSpanAccess::get(JNIEnv * env, const void* container, jint index)
{
    jvalue _value;
    _value.l = nullptr;
    const QtJambiSpan* p = reinterpret_cast<const QtJambiSpan*>(container);
    Q_ASSERT_X(index >= 0 && index < p->size, "QSpan<T>::operator[index]", "index out of range");
    const void* ptr = reinterpret_cast<const char*>(p->begin)+index*m_offset;
    if(m_internalToExternalConverter(env, nullptr, ptr, _value, true)){
        return _value.l;
    }
    return nullptr;
}

const void* AutoSpanAccess::get(const void* container, qsizetype index)
{
    const QtJambiSpan* p = reinterpret_cast<const QtJambiSpan*>(container);
    Q_ASSERT_X(index >= 0 && index < p->size, "QSpan<T>::operator[index]", "index out of range");
    return reinterpret_cast<const char*>(p->begin)+index*m_offset;
}

jint AutoSpanAccess::size(JNIEnv *, const void* container)
{
    const QtJambiSpan* p = reinterpret_cast<const QtJambiSpan*>(container);
    return jint(p->size);
}

jint AutoSpanAccess::size_bytes(JNIEnv *, const void* container)
{
    const QtJambiSpan* p = reinterpret_cast<const QtJambiSpan*>(container);
    return jint(p->size * m_elementMetaType.sizeOf());
}

bool AutoSpanAccess::set(JNIEnv * env, const ContainerInfo& container, jint index, jobject value)
{
    if(isConst())
        return false;
    QtJambiSpan* p = reinterpret_cast<QtJambiSpan*>(container.container);
    Q_ASSERT_X(index >= 0 && index < size_t(p->size), "QSpan<T>::operator[index]", "index out of range");
    void* target = reinterpret_cast<char*>(const_cast<void*>(p->begin))+index*m_offset;
    jvalue _value;
    _value.l = value;
    m_externalToInternalConverter(env, nullptr, _value, target, jValueType::l);
    return true;
}

bool AutoSpanAccess::set(void* container, qsizetype index, const void* value)
{
    if(isConst())
        return false;
    QtJambiSpan* p = reinterpret_cast<QtJambiSpan*>(container);
    Q_ASSERT_X(index >= 0 && index < p->size, "QSpan<T>::operator[index]", "index out of range");
    void* target = reinterpret_cast<char*>(const_cast<void*>(p->begin))+index*m_offset;
    m_elementMetaType.destruct(target);
    m_elementMetaType.construct(target, value);
    return true;
}

PointerRCAutoSpanAccess::PointerRCAutoSpanAccess(PointerRCAutoSpanAccess& other)
    : AutoSpanAccess(other), ReferenceCountingSetContainer() {}

PointerRCAutoSpanAccess* PointerRCAutoSpanAccess::clone(){
    return new PointerRCAutoSpanAccess(*this);
}

void PointerRCAutoSpanAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject set = Java::Runtime::ArrayList::newInstance(env);
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

void PointerRCAutoSpanAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    AutoSpanAccess::assign(env, container, container2);
    if(PointerRCAutoSpanAccess* access = dynamic_cast<PointerRCAutoSpanAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

bool PointerRCAutoSpanAccess::set(JNIEnv * env, const ContainerInfo& container, jint index, jobject value) {
    jobject oldValue = AutoSpanAccess::get(env, container.container, index);
    if(AutoSpanAccess::set(env, container, index, value)){
        removeRC(env, container.object, oldValue);
        addRC(env, container.object, value);
        return true;
    }
    return false;
}

NestedPointersRCAutoSpanAccess::NestedPointersRCAutoSpanAccess(NestedPointersRCAutoSpanAccess& other)
    : AutoSpanAccess(other), ReferenceCountingSetContainer() {}

NestedPointersRCAutoSpanAccess* NestedPointersRCAutoSpanAccess::clone(){
    return new NestedPointersRCAutoSpanAccess(*this);
}

void NestedPointersRCAutoSpanAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    AutoSpanAccess::assign(env, container, container2);
    updateRC(env, container);
}

void NestedPointersRCAutoSpanAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
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

bool NestedPointersRCAutoSpanAccess::set(JNIEnv * env, const ContainerInfo& container, jint index, jobject value) {
    if(AutoSpanAccess::set(env, container, index, value)){
        updateRC(env, container);
        return true;
    }
    return false;
}

#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)
