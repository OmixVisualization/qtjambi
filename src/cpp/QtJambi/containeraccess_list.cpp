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


AutoListAccess::AutoListAccess(
        const QMetaType& elementMetaType,
        const QtJambiUtils::QHashFunction& hashFunction,
        const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
        const QtJambiUtils::ExternalToInternalConverter& externalToInternalConverter,
        const QSharedPointer<AbstractContainerAccess>& elementNestedContainerAccess,
        PtrOwnerFunction elementOwnerFunction,
        AbstractContainerAccess::DataType elementDataType
        )
    : AbstractListAccess(), AbstractNestedSequentialAccess(),
      m_elementMetaType(elementMetaType),
      m_hashFunction(hashFunction),
      m_internalToExternalConverter(internalToExternalConverter),
      m_externalToInternalConverter(externalToInternalConverter),
      m_elementNestedContainerAccess(elementNestedContainerAccess),
      m_offset(0),
      m_elementOwnerFunction(elementOwnerFunction),
      m_elementDataType(elementDataType)
{
    Q_ASSERT(m_elementMetaType.id()!=QMetaType::UnknownType
            && m_elementMetaType.id()!=QMetaType::Void);
    m_offset = m_elementMetaType.sizeOf();
    if(m_offset%m_elementMetaType.alignOf()>0)
        m_offset += m_elementMetaType.alignOf()-m_offset%m_elementMetaType.alignOf();
}

AutoListAccess::AutoListAccess(const AutoListAccess& other)
  : AbstractListAccess(), AbstractNestedSequentialAccess(),
    m_elementMetaType(other.m_elementMetaType),
    m_hashFunction(other.m_hashFunction),
    m_internalToExternalConverter(other.m_internalToExternalConverter),
    m_externalToInternalConverter(other.m_externalToInternalConverter),
    m_elementNestedContainerAccess(other.m_elementNestedContainerAccess),
    m_offset(other.m_offset),
    m_elementOwnerFunction(other.m_elementOwnerFunction),
    m_elementDataType(other.m_elementDataType)
{
}

void AutoListAccess::dispose(){
    delete this;
}

std::unique_ptr<AbstractListAccess::ElementIterator> AutoListAccess::elementIterator(const void* container) {
    class ElementIterator : public AbstractListAccess::ElementIterator{
        AutoListAccess* m_access;
        char* current;
        char* end;
        ElementIterator(const ElementIterator& other)
            :m_access(other.m_access),
            current(other.current),
            end(other.end) {}
    protected:
        AbstractSequentialAccess* access() override { return m_access; }
    public:
        ElementIterator(AutoListAccess* _access, const QListData* p)
            : AbstractListAccess::ElementIterator(),
            m_access(_access),
            current(p->ptr),
            end(p->ptr + p->size * m_access->m_offset)
        {
        }
        bool hasNext() override{
            return current!=end;
        }
        bool isConst() override{
            return true;
        }
        jobject next(JNIEnv * env) override{
            void* data = current;
            current += m_access->m_offset;
            jvalue _value;
            _value.l = nullptr;
            m_access->m_internalToExternalConverter(env, nullptr, data, _value, true);
            return _value.l;
        }
        const void* next() override {
            void* data = current;
            current += m_access->m_offset;
            if(m_access->m_elementDataType & AbstractContainerAccess::PointersMask){
                return *reinterpret_cast<void**>(data);
            }else{
                return data;
            }
        }
        const void* constNext() override {
            void* data = current;
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
    return std::unique_ptr<AbstractListAccess::ElementIterator>(new ElementIterator(this, reinterpret_cast<const QListData*>(container)));
}

std::unique_ptr<AbstractListAccess::ElementIterator> AutoListAccess::elementIterator(void* container) {
    class ElementIterator : public AbstractListAccess::ElementIterator{
        AutoListAccess* m_access;
        char* current;
        char* end;
        ElementIterator(const ElementIterator& other)
            :m_access(other.m_access),
            current(other.current),
            end(other.end) {}
    protected:
        AbstractSequentialAccess* access() override { return m_access; }
    public:
        ElementIterator(AutoListAccess* _access, QListData* p)
            : AbstractListAccess::ElementIterator(),
            m_access(_access),
            current(p->ptr),
            end(p->ptr + p->size * m_access->m_offset)
        {
        }
        bool hasNext() override{
            return current!=end;
        }
        bool isConst() override{
            return false;
        }
        jobject next(JNIEnv * env) override{
            void* data = current;
            current += m_access->m_offset;
            jvalue _value;
            _value.l = nullptr;
            m_access->m_internalToExternalConverter(env, nullptr, data, _value, true);
            return _value.l;
        }
        const void* next() override {
            void* data = current;
            current += m_access->m_offset;
            if(m_access->m_elementDataType & AbstractContainerAccess::PointersMask){
                return *reinterpret_cast<void**>(data);
            }else{
                return data;
            }
        }
        const void* constNext() override {
            void* data = current;
            current += m_access->m_offset;
            return data;
        }
        void* mutableNext() override {
            void* data = current;
            current += m_access->m_offset;
            return data;
        }
        bool operator==(const AbstractSequentialAccess::ElementIterator& other) const override {
            return current==reinterpret_cast<const ElementIterator&>(other).current;
        }
        std::unique_ptr<AbstractSequentialAccess::ElementIterator> clone() const override {
            return std::unique_ptr<AbstractSequentialAccess::ElementIterator>(new ElementIterator(*this));
        }
    };
    return std::unique_ptr<AbstractListAccess::ElementIterator>(new ElementIterator(this, reinterpret_cast<QListData*>(container)));
}

AutoListAccess* AutoListAccess::clone(){
    return new AutoListAccess(*this);
}

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
AutoSpanAccess* AutoListAccess::createSpanAccess(bool isConst){
    return new AutoSpanAccess(
            m_elementMetaType,
            m_hashFunction,
            m_internalToExternalConverter,
            m_externalToInternalConverter,
            m_elementNestedContainerAccess,
            m_elementOwnerFunction,
            m_elementDataType,
            isConst
        );
}
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)

size_t AutoListAccess::sizeOf() const {
    return sizeof(QList<char>);
}

size_t AutoListAccess::alignOf() const {
    return alignof(QList<char>);
}

void* AutoListAccess::constructContainer(JNIEnv*, void* result, const ConstContainerAndAccessInfo& container) {
    return constructContainer(result, container.container);
}

void* AutoListAccess::constructContainer(void* result){
    return new(result) QList<char>();
}

void* AutoListAccess::constructContainer(void* result, const void* container){
    result = constructContainer(result);
    assign(result, container);
    return result;
}

bool AutoListAccess::destructContainer(void* container) {
    QListData* p = reinterpret_cast<QListData*>(container);
    if (!p->size)
        return true;
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
    return true;
}

void* AutoListAccess::constructContainer(JNIEnv*, void* result, const ContainerAndAccessInfo& container) {
    return constructContainer(result, container.container);
}

void* AutoListAccess::constructContainer(void* result, void* container){
    return new(result) QList<char>(std::move(*reinterpret_cast<QList<char>*>(container)));
}

bool AutoListAccess::equals(const void* container1, const void* container2)
{
    const QListData* p1 = reinterpret_cast<const QListData*>(container1);
    const QListData* p2 = reinterpret_cast<const QListData*>(container2);
    if(p1->size!=p2->size)
        return false;
    if(p1->ptr==p2->ptr)
        return true;
    for(qsizetype i = 0; i<p1->size; ++i){
        void* element1 = p1->ptr + i * m_offset;
        void* element2 = p2->ptr + i * m_offset;
        if(!m_elementMetaType.equals(element1, element2))
            return false;
    }
    return true;
}

void AutoListAccess::debugStream(QDebug &dbg, const void *ptr)
{
    QtPrivate::printSequentialContainer(dbg, "QList", ConstContainer{ptr, this});
}

void AutoListAccess::dataStreamOut(QDataStream &s, const void *ptr)
{
    QtPrivate::writeSequentialContainer(s, ConstContainer{ptr, this});
}

void AutoListAccess::dataStreamIn(QDataStream &s, void *ptr)
{
    Container container(ptr, this);
    QtPrivate::readArrayBasedContainer(s, container);
}

AutoListAccess::iterator AutoListAccess::begin(const void* container) {
    const QListData* p = reinterpret_cast<const QListData*>(container);
    return iterator(m_offset, p->ptr);
}

AutoListAccess::iterator AutoListAccess::end(const void* container) {
    const QListData* p = reinterpret_cast<const QListData*>(container);
    return iterator(m_offset, p->ptr + p->size * m_offset);
}

AutoListAccess::iterator::iterator(size_t _offset, char* _ptr)
    : offset(_offset), ptr(_ptr){
}

bool AutoListAccess::iterator::operator<(const iterator& right) const{
    return right.ptr<ptr;
}
bool AutoListAccess::iterator::operator==(const iterator& right) const{
    return right.ptr==ptr;
}
const char* AutoListAccess::iterator::operator->() const{
    return ptr;
}
const char& AutoListAccess::iterator::operator*() const{
    return *ptr;
}
const char* AutoListAccess::iterator::data() const{
    return ptr;
}
char* AutoListAccess::iterator::operator->(){
    return ptr;
}
char& AutoListAccess::iterator::operator*(){
    return *ptr;
}
char* AutoListAccess::iterator::data(){
    return ptr;
}

AutoListAccess::iterator& AutoListAccess::iterator::operator++(){
    ptr += offset;
    return *this;
}

AutoListAccess::iterator AutoListAccess::iterator::operator++(int){
    iterator _this = *this;
    ptr += offset;
    return _this;
}

AutoListAccess::iterator& AutoListAccess::iterator::operator--(){
    ptr -= offset;
    return *this;
}

AutoListAccess::iterator AutoListAccess::iterator::operator--(int){
    iterator _this = *this;
    ptr -= offset;
    return _this;
}

QSharedPointer<class AutoListAccess> getListAccess(const QtPrivate::QMetaTypeInterface *iface){
    return findContainerAccess(QMetaType(iface)).dynamicCast<AutoListAccess>();
}
QSharedPointer<class AutoListAccess> getListAccess(const QMetaType& metaType){
    return findContainerAccess(metaType).dynamicCast<AutoListAccess>();
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

QtMetaContainerPrivate::QMetaSequenceInterface* AutoListAccess::createMetaSequenceInterface(QMetaType newMetaType){
    using namespace QtMetaContainerPrivate;
    QMetaSequenceInterface* metaSequenceInterface{nullptr};
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->lock());
        if(storage->metaSequencesByMetaType().contains(newMetaType.iface()))
           return &storage->metaSequencesByMetaType()[newMetaType.iface()];
    }
    {
        QWriteLocker locker(storage->lock());
        metaSequenceInterface = &storage->metaSequencesByMetaType()[newMetaType.iface()];
    }
    QSharedPointer<class AutoListAccess> access = getListAccess(newMetaType);
    metaSequenceInterface->valueMetaType = access->m_elementMetaType.iface();
    metaSequenceInterface->iteratorCapabilities = InputCapability | ForwardCapability | BiDirectionalCapability;
    metaSequenceInterface->addRemoveCapabilities = CanAddAtBegin|CanRemoveAtBegin|CanAddAtEnd|CanRemoveAtEnd;

    metaSequenceInterface->sizeFn = [](const void *c) -> qsizetype {
        if(!c)
            return 0;
        const QListData* p = reinterpret_cast<const QListData*>(c);
        return p->size;
    };

    metaSequenceInterface->clearFn = qtjambi_function_pointer<16,void(void *)>([newMetaType](void *c) {
                                        if(!c)
                                            return;
                                        QSharedPointer<class AutoListAccess> access = getListAccess(newMetaType);
                                        if(!access)
                                            return;
                                        access->clear(c);
                                    }, size_t(newMetaType.iface()));
    struct MetaIterator{
        MetaIterator(char* _iterator, const QSharedPointer<class AutoListAccess>& _access) : iterator(_iterator), access(_access) {}
        char* iterator;
        QSharedPointer<class AutoListAccess> access;
    };
    metaSequenceInterface->createIteratorFn = qtjambi_function_pointer<16,void*(void *,QMetaContainerInterface::Position)>(
                [newMetaType](void *c, QMetaContainerInterface::Position p) -> void* {
                        if(!c)
                            return nullptr;
                        QSharedPointer<class AutoListAccess> access = getListAccess(newMetaType);
                        if(!access)
                            return nullptr;
                        QListData* ap = reinterpret_cast<QListData*>(c);
                        access->detach(ap);
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
                        return new MetaIterator(nullptr, access);
                    }, size_t(newMetaType.iface()));
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
    metaSequenceInterface->createConstIteratorFn = qtjambi_function_pointer<16,void*(const void *,QMetaContainerInterface::Position)>(
        [newMetaType](const void *c, QMetaContainerInterface::Position p) -> void* {
            if(!c)
                return nullptr;
            QSharedPointer<class AutoListAccess> access = getListAccess(newMetaType);
            if(!access)
                return nullptr;
            const QListData* ap = reinterpret_cast<const QListData*>(c);
            if(ap){
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
            }
            return new MetaIterator(nullptr, access);
        }, size_t(newMetaType.iface()));
    metaSequenceInterface->destroyConstIteratorFn = metaSequenceInterface->destroyIteratorFn;
    metaSequenceInterface->compareConstIteratorFn = metaSequenceInterface->compareIteratorFn;
    metaSequenceInterface->copyConstIteratorFn = metaSequenceInterface->copyIteratorFn;
    metaSequenceInterface->advanceConstIteratorFn = metaSequenceInterface->advanceIteratorFn;
    metaSequenceInterface->diffConstIteratorFn = metaSequenceInterface->diffIteratorFn;
    metaSequenceInterface->valueAtIndexFn = qtjambi_function_pointer<16,void(const void *, qsizetype, void *)>([newMetaType](const void *container, qsizetype index, void *r) {
        if(!container)
            return;
        QSharedPointer<class AutoListAccess> access = getListAccess(newMetaType);
        if(!access)
            return;
        const QListData* p = reinterpret_cast<const QListData*>(container);
        Q_ASSERT_X(index >= 0 && index < p->size, "QList<T>::at", "index out of range");
        void* ptr = p->ptr+index*access->m_offset;
        access->m_elementMetaType.destruct(r);
        access->m_elementMetaType.construct(r, ptr);
    }, size_t(newMetaType.iface()));
    metaSequenceInterface->setValueAtIndexFn = qtjambi_function_pointer<16,void(void *, qsizetype, const void *)>([newMetaType](void *container, qsizetype index, const void *e) {
        if(!container)
            return;
        QSharedPointer<class AutoListAccess> access = getListAccess(newMetaType);
        if(!access)
            return;
        QListData* p = reinterpret_cast<QListData*>(container);
        if(index == p->size){
            access->emplace(p, index, e, 1);
        }else{
            Q_ASSERT_X(index >= 0 && index < p->size, "QList<T>::replace", "index out of range");
            access->detach(p);
            void* target = p->ptr + index * access->m_offset;
            access->m_elementMetaType.destruct(target);
            access->m_elementMetaType.construct(target, e);
        }
    }, size_t(newMetaType.iface()));
    metaSequenceInterface->addValueFn = qtjambi_function_pointer<16,void(void *, const void *, QMetaSequenceInterface::Position)>([newMetaType](void *container, const void *e, QMetaSequenceInterface::Position position) {
        if(!container)
            return;
        QSharedPointer<class AutoListAccess> access = getListAccess(newMetaType);
        if(!access)
            return;
        QListData* p = reinterpret_cast<QListData*>(container);
        if(position==QMetaSequenceInterface::AtBegin){
            access->emplace(p, 0, e, 1);
        }else{
            access->emplace(p, p->size, e, 1);
        }
    }, size_t(newMetaType.iface()));
    metaSequenceInterface->removeValueFn = qtjambi_function_pointer<16,void(void *, QMetaSequenceInterface::Position)>([newMetaType](void *container, QMetaSequenceInterface::Position position) {
        if(!container)
            return;
        QSharedPointer<class AutoListAccess> access = getListAccess(newMetaType);
        if(!access)
            return;
        QListData* p = reinterpret_cast<QListData*>(container);
        if(position==QMetaSequenceInterface::AtBegin){
            access->remove(container, 0, 1);
        }else{
            access->remove(container, p->size-1, 1);
        }
    }, size_t(newMetaType.iface()));
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

QMetaType AutoListAccess::registerContainer(const QByteArray& typeName)
{
    QMetaType newMetaType = QMetaType::fromName(typeName);
    if(!newMetaType.isValid()){
        QSharedPointer<AutoListAccess> access(new AutoListAccess(*this), &containerDisposer);
        auto iface = m_elementMetaType.iface();
        newMetaType = registerContainerMetaType(typeName,
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
                                       uint(sizeOf()),
                                       ushort(alignof(QList<char>)),
                                       QMetaType::UnknownType,
                                       QMetaType::NeedsConstruction
                                                   | QMetaType::NeedsDestruction
#if QT_VERSION < QT_VERSION_CHECK(6, 5, 0)
                                                    | QMetaType::MovableType,
#else
                                                    | QMetaType::RelocatableType,
#endif
                                       nullptr,
                                       nullptr,
                                       access);
        if(m_hashFunction){
            insertHashFunctionByMetaType(newMetaType.iface(),
                                            [access]
                                            (const void* ptr, size_t seed)->size_t{
                                                if(ptr){
                                                    size_t hashCode = seed;
                                                    const QListData* p = reinterpret_cast<const QListData*>(ptr);
                                                    for(size_t i=0; i<size_t(p->size); ++i){
                                                        const void* element = p->ptr + i * access->m_offset;
                                                        hashCode = hashCode ^ (access->m_hashFunction(element, 0) + 0x9e3779b9 + (hashCode << 6) + (hashCode >> 2));
                                                    }
                                                    return hashCode;
                                                }else{
                                                    return 0;
                                                }
                                            });
        }
#if QT_VERSION >= QT_VERSION_CHECK(6,11,0)
        typedef QMetaSequence::Iterable SequentialIterable;
#else
        typedef QSequentialIterable SequentialIterable;
#endif
        const QMetaType to = QMetaType::fromType<SequentialIterable>();
        QMetaType::registerMutableViewFunction([newMetaType](void *src, void *target)->bool{
            new(target) SequentialIterable(QMetaSequence(createMetaSequenceInterface(newMetaType)), reinterpret_cast<void **>(src));
            return true;
        }, QMetaType(newMetaType), to);
        QMetaType::registerConverterFunction([newMetaType](const void *src, void *target)->bool{
            new(target) SequentialIterable(QMetaSequence(createMetaSequenceInterface(newMetaType)), reinterpret_cast<void const*const*>(src));
            return true;
        }, QMetaType(newMetaType), to);
    }else{
        registerContainerAccess(newMetaType, this);
    }
    return newMetaType;
}

void AutoListAccess::assign(JNIEnv *, const ContainerInfo& container, const ConstContainerAndAccessInfo& other){
    assign(container.container, other.container);
}

void AutoListAccess::assign(void* container, const void* other)
{
    QListData* p = reinterpret_cast<QListData*>(container);
    const QListData* p2 = reinterpret_cast<const QListData*>(other);
    QListData detached(allocate(p2->size));
    for(qsizetype i = 0; i<p2->size; ++i){
        const void* source = p2->ptr + i * m_offset;
        void* target = detached.ptr + i * m_offset;
        m_elementMetaType.construct(target, source);
        ++detached.size;
    }
    if (detached->ptr)
        detached.setFlag(QArrayData::CapacityReserved);
    swapAndDestroy(p, std::move(detached));
}

const QMetaType& AutoListAccess::elementMetaType() {return m_elementMetaType;}

AbstractContainerAccess::DataType AutoListAccess::elementType() {return m_elementDataType;}

bool AutoListAccess::isDetached(const void* container){
    const QListData* p = reinterpret_cast<const QListData*>(container);
    return p && !p->isShared();
}

AbstractContainerAccess* AutoListAccess::elementNestedContainerAccess() {
    return m_elementNestedContainerAccess ? m_elementNestedContainerAccess->clone() : nullptr;
}
const QSharedPointer<AbstractContainerAccess>& AutoListAccess::sharedElementNestedContainerAccess(){
    return m_elementNestedContainerAccess;
}
bool AutoListAccess::hasNestedContainerAccess(){
    return !m_elementNestedContainerAccess.isNull();
}
bool AutoListAccess::hasNestedPointers(){
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

const QObject* AutoListAccess::getOwner(const void* container){
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

bool AutoListAccess::hasOwnerFunction(){
    if(m_elementOwnerFunction && !(elementType() & PointersMask))
        return true;
    if(!(elementType() & PointersMask) && m_elementNestedContainerAccess && m_elementNestedContainerAccess->hasOwnerFunction())
        return true;
    return false;
}

void AutoListAccess::detach(const ContainerInfo& container){
    QListData* p = reinterpret_cast<QListData*>(container.container);
    detach(p);
}

bool AutoListAccess::isSharedWith(const void* container, const void* container2){
    const QListData* p = reinterpret_cast<const QListData*>(container);
    const QListData* p2 = reinterpret_cast<const QListData*>(container2);
    return p->isSharedWith(*p2);
}

void AutoListAccess::swap(JNIEnv *, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    QListData& p = *reinterpret_cast<QListData*>(container.container);
    QListData& p2 = *reinterpret_cast<QListData*>(container2.container);
    p.swap(p2);
}

jobject AutoListAccess::createIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr)
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

jobject AutoListAccess::createConstIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr)
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

jobject AutoListAccess::end(JNIEnv * env, const ExtendedContainerInfo& container)
{
    QListData* p = reinterpret_cast<QListData*>(container.container);
    detach(p);
    void* iteratorPtr = new char*(reinterpret_cast<char*>(p->ptr + p->size * m_offset));
    return createIterator(env, container.nativeId, iteratorPtr);
}

jobject AutoListAccess::begin(JNIEnv * env, const ExtendedContainerInfo& container)
{
    QListData* p = reinterpret_cast<QListData*>(container.container);
    detach(p);
    void* iteratorPtr = new char*(p->ptr);
    return createIterator(env, container.nativeId, iteratorPtr);
}

jobject AutoListAccess::constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container)
{
    const QListData* p = reinterpret_cast<const QListData*>(container.container);
    void* iteratorPtr = new char*(reinterpret_cast<char*>(p->ptr + p->size * m_offset));
    return createConstIterator(env, container.nativeId, iteratorPtr);
}

jobject AutoListAccess::constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container)
{
    const QListData* p = reinterpret_cast<const QListData*>(container.container);
    void* iteratorPtr = new char*(p->ptr);
    return createConstIterator(env, container.nativeId, iteratorPtr);
}

void AutoListAccess::appendList(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo)
{
    if (ContainerAPI::getAsQList(env, containerInfo.object, elementMetaType(), containerInfo.container, containerInfo.access)) {
        QListData* p = reinterpret_cast<QListData*>(container.container);
        QListData* p2 = reinterpret_cast<QListData*>(containerInfo.container);
        detachAndGrow(p, QArrayData::GrowsAtEnd, p2->size, nullptr, nullptr);
        Q_ASSERT(freeSpaceAtEnd(p) >= p2->size);
        void *where = createHole(p, QArrayData::GrowsAtEnd, p->size, p2->size-1);
        for(size_t i=0; i<size_t(p2->size); ++i){
            m_elementMetaType.construct(reinterpret_cast<char*>(where) + i * m_offset, p2->ptr + i * m_offset);
        }
    }else{
        jobject iter = QtJambiAPI::iteratorOfJavaIterable(env, containerInfo.object);
        jint idx = size(env, container.container);
        reserve(env, container, idx + QtJambiAPI::sizeOfJavaCollection(env, containerInfo.object));
        while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
            insert(env, container, idx++, 1, QtJambiAPI::nextOfJavaIterator(env, iter));
        }
    }
}

const void* AutoListAccess::at(const void* container, qsizetype index)
{
    const QListData* p = reinterpret_cast<const QListData*>(container);
    Q_ASSERT_X(index >= 0 && index < p->size, "QList<T>::at", "index out of range");
    return p->ptr+index*m_offset;
}

void* AutoListAccess::at(void* container, qsizetype index)
{
    QListData* p = reinterpret_cast<QListData*>(container);
    Q_ASSERT_X(index >= 0 && index < p->size, "QList<T>::at", "index out of range");
    detach(p);
    return p->ptr+index*m_offset;
}

jobject AutoListAccess::at(JNIEnv * env, const void* container, jint index)
{
    jvalue _value;
    _value.l = nullptr;
    const QListData* p = reinterpret_cast<const QListData*>(container);
    Q_ASSERT_X(index >= 0 && index < p->size, "QList<T>::at", "index out of range");
    void* ptr = p->ptr+index*m_offset;
    if(m_internalToExternalConverter(env, nullptr, ptr, _value, true)){
        return _value.l;
    }
    return nullptr;
}

jobject AutoListAccess::value(JNIEnv * env, const void* container, jint index)
{
    jvalue _value;
    _value.l = nullptr;
    const QListData* p = reinterpret_cast<const QListData*>(container);
    if(index >= 0 && index < p->size){
        void* ptr = p->ptr+index*m_offset;
        if(m_internalToExternalConverter(env, nullptr, ptr, _value, true)){
            return _value.l;
        }
    }else{
        void* ptr = m_elementMetaType.create();
        bool success = m_internalToExternalConverter(env, nullptr, ptr, _value, true);
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
    const QListData* p = reinterpret_cast<const QListData*>(container);
    if(index >= 0 && index < p->size){
        void* ptr = p->ptr+index*m_offset;
        if(m_internalToExternalConverter(env, nullptr, ptr, _value, true)){
            return _value.l;
        }
    }else{
        return defaultValue;
    }
    return nullptr;
}

void AutoListAccess::swapItemsAt(JNIEnv *, const ContainerInfo& container, jint i, jint j)
{
    QListData* p = reinterpret_cast<QListData*>(container.container);
    Q_ASSERT_X(i >= 0 && i < p->size && j >= 0 && j < p->size,
               "QList<T>::swap", "index out of range");
    detach(p);
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
            const QListData* p = reinterpret_cast<const QListData*>(container);
            return m_elementMetaType.equals(_qvaluePtr, p->ptr);
        }
    }
    return false;
}

qsizetype AutoListAccess::size(const void* container)
{
    const QListData* p = reinterpret_cast<const QListData*>(container);
    return p->size;
}

jint AutoListAccess::size(JNIEnv *, const void* container)
{
    const QListData* p = reinterpret_cast<const QListData*>(container);
    return jint(p->size);
}

void AutoListAccess::reserve(JNIEnv *, const ContainerInfo& container, jint asize)
{
    reserve(container.container, asize);
}

void AutoListAccess::reserve(void* container, qsizetype asize)
{
    QListData* p = reinterpret_cast<QListData*>(container);
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
    QListData detached(allocate(newSize));
    for(qsizetype i = 0; i<p->size; ++i){
        void* source = p->ptr + i * m_offset;
        void* target = detached.ptr + i * m_offset;
        m_elementMetaType.construct(target, source);
        ++detached.size;
    }
    if (detached->ptr)
        detached.setFlag(QArrayData::CapacityReserved);
    swapAndDestroy(p, std::move(detached));
}

void AutoListAccess::replace(void* container, qsizetype index, const void* value)
{
    QListData* p = reinterpret_cast<QListData*>(container);
    Q_ASSERT_X(index >= 0 && index < p->size, "QList<T>::replace", "index out of range");
    detach(p);
    void* target = p->ptr + index * m_offset;
    m_elementMetaType.destruct(target);
    m_elementMetaType.construct(target, value);
}

void AutoListAccess::replace(JNIEnv * env, const ContainerInfo& container, jint index, jobject value)
{
    QListData* p = reinterpret_cast<QListData*>(container.container);
    Q_ASSERT_X(index >= 0 && index < p->size, "QList<T>::replace", "index out of range");
    detach(p);
    void* target = p->ptr + index * m_offset;
    jvalue _value;
    _value.l = value;
    //void *_qvaluePtr;
    if(m_externalToInternalConverter(env, nullptr, _value, target, jValueType::l)){
        //m_elementMetaType.destruct(target);
        //m_elementMetaType.construct(target, _qvaluePtr);
    }
}

void AutoListAccess::remove(JNIEnv *, const ContainerInfo& container, jint index, jint n){
    remove(container.container, index, n);
}
void AutoListAccess::remove(void* container, qsizetype index, qsizetype n)
{
    QListData* p = reinterpret_cast<QListData*>(container);
    if(n==0)
        return;
    detach(p);
    qsizetype nmb = qMin<qsizetype>(n, p->size-index);
    for(qsizetype i = 0; i<nmb; ++i){
        m_elementMetaType.destruct(p->ptr + (index+i) * m_offset);
    }
    p->size -= nmb;
    if(p->size>0){
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
        }else{
            if(auto moveCtr = m_elementMetaType.iface()->moveCtr){
                for(qsizetype i = index-1; i>=0; --i){
                    void* target = p->ptr + (i+nmb) * m_offset;
                    void* source = p->ptr + i * m_offset;
                    moveCtr(m_elementMetaType.iface(), target, source);
                    m_elementMetaType.destruct(source);
                }
            }else{
                for(qsizetype i = index-1; i>=0; --i){
                    void* target = p->ptr + (i+nmb) * m_offset;
                    void* source = p->ptr + i * m_offset;
                    m_elementMetaType.construct(target, source);
                    m_elementMetaType.destruct(source);
                }
            }
            p->ptr += nmb*m_offset;
        }
    }
}

jint AutoListAccess::removeAll(JNIEnv * env, const ContainerInfo& container, jobject value)
{
    QtJambiScope scope;
    jint removedCount = 0;
    void *_qvaluePtr = nullptr;
    jvalue _value;
    _value.l = value;
    if(m_externalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l)){
        const QListData* p = reinterpret_cast<const QListData*>(container.container);
        qsizetype index;
        do{
            index = -1;
            for(qsizetype i = 0; i<p->size; ++i){
                void* element = p->ptr + i * m_offset;
                if(m_elementMetaType.equals(_qvaluePtr, element)){
                    remove(env, container, i, 1);
                    ++removedCount;
                    index = i;
                    break;
                }
            }
        }while(index>=0);
    }
    return removedCount;
}

jboolean AutoListAccess::equal(JNIEnv * env, const void* container, jobject other)
{
    void* ptr{nullptr};
    if (ContainerAPI::getAsQList(env, other, elementMetaType(), ptr)) {
        const QListData* p = reinterpret_cast<const QListData*>(container);
        const QListData* p2 = reinterpret_cast<const QListData*>(ptr);
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
    }else{
        const QListData* p = reinterpret_cast<const QListData*>(container);
        if(p->size!=QtJambiAPI::sizeOfJavaCollection(env, other))
            return false;
        jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, other);
        size_t i = 0;
        QtJambiScope scope;
        jvalue _value;
        void *_qvaluePtr = nullptr;
        while(QtJambiAPI::hasJavaIteratorNext(env, iterator)){
            _value.l = QtJambiAPI::nextOfJavaIterator(env, iterator);
            _qvaluePtr = nullptr;
            if(!m_externalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l))
                return false;
            void* element = p->ptr + (i++) * m_offset;
            if(!m_elementMetaType.equals(element, _qvaluePtr))
                return false;
        }
        return true;
    }
    return false;
}

void AutoListAccess::move(JNIEnv *, const ContainerInfo& container, jint from, jint to)
{
    if (from == to)
        return;
    QListData* p = reinterpret_cast<QListData*>(container.container);
    Q_ASSERT_X(from >= 0 && from < p->size && to >= 0 && to < p->size,
               "QList<T>::move", "index out of range");
    detach(p);
    void* tmp = m_elementMetaType.create(p->ptr + from * m_offset);
    for(jint i=from; i<to; ++i){
        m_elementMetaType.destruct(p->ptr + i * m_offset);
        m_elementMetaType.construct(p->ptr + i * m_offset, p->ptr + (i+1) * m_offset);
    }
    m_elementMetaType.destruct(p->ptr + to * m_offset);
    m_elementMetaType.construct(p->ptr + to * m_offset, tmp);
    m_elementMetaType.destroy(tmp);
}

ContainerAndAccessInfo AutoListAccess::mid(JNIEnv * env, const ConstContainerAndAccessInfo& container, jint _pos, jint _len)
{
    ContainerAndAccessInfo result;
    using namespace QtPrivate;
    const QListData* p = reinterpret_cast<const QListData*>(container.container);
    qsizetype pos = _pos;
    qsizetype len = _len;
    switch (QContainerImplHelper::mid(p->size, &pos, &len)) {
    case QContainerImplHelper::Null:
    case QContainerImplHelper::Empty:{
        result.access = container.access->clone();
        result.container = result.access->createContainer();
        result.object = ContainerAPI::objectFromQList(env, result.container, result.access);
        return result;
    }
    case QContainerImplHelper::Full:{
        result.access = container.access->clone();
        result.container = result.access->createContainer();
        result.object = ContainerAPI::objectFromQList(env, result.container, result.access);
        result.access->assign(env, result, container);
        return result;
    }
    case QContainerImplHelper::Subset:
        break;
    }
    QListData* newList = new QListData(allocate(len));
    for(qsizetype i = 0; i<len; ++i){
        void* source = p->ptr + (pos+i) * m_offset;
        void* target = newList->ptr + i * m_offset;
        m_elementMetaType.construct(target, source);
        ++newList->size;
    }
    result.container = newList;
    result.access = container.access->clone();
    result.object = ContainerAPI::objectFromQList(env, result.container, result.access);
    return result;
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
            const QListData* p = reinterpret_cast<const QListData*>(container);
            for(qsizetype i = qsizetype(index); i>=0; --i){
                void* element = p->ptr + i * m_offset;
                if(m_elementMetaType.equals(_qvaluePtr, element))
                    return i;
            }
        }
    }
    return -1;
}

bool AutoListAccess::append(void* container, const void* value){
    QListData* p = reinterpret_cast<QListData*>(container);
    emplace(p, p->size, value, 1);
    return true;
}

void AutoListAccess::insert(JNIEnv * env, const ContainerInfo& container, jint index, jint n, jobject value)
{
    QListData* p = reinterpret_cast<QListData*>(container.container);
    emplace(p, env, index, value, n);
}

void AutoListAccess::insert(void* container, qsizetype index, qsizetype n, const void* value)
{
    QListData* p = reinterpret_cast<QListData*>(container);
    emplace(p, index, value, n);
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
            const QListData* p = reinterpret_cast<const QListData*>(container);
            for(qsizetype i = qsizetype(index); i<sz; ++i){
                void* element = p->ptr + i * m_offset;
                if(m_elementMetaType.equals(_qvaluePtr, element))
                    return i;
            }
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
            const QListData* p = reinterpret_cast<const QListData*>(container);
            void* element = p->ptr + (sz-1) * m_offset;
            return m_elementMetaType.equals(_qvaluePtr, element);
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
        const QListData* p = reinterpret_cast<const QListData*>(container);
        for(qsizetype i = 0; i<sz; ++i){
            void* element = p->ptr + i * m_offset;
            if(m_elementMetaType.equals(_qvaluePtr, element))
                ++result;
        }
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
        const QListData* p = reinterpret_cast<const QListData*>(container);
        for(qsizetype i = 0; i<sz; ++i){
            void* element = p->ptr + i * m_offset;
            if(m_elementMetaType.equals(_qvaluePtr, element))
                return true;
        }
    }
    return false;
}

void AutoListAccess::clear(JNIEnv *, const ContainerInfo& container)
{
    clear(container.container);
}

void AutoListAccess::clear(void* container)
{
    QListData* p = reinterpret_cast<QListData*>(container);
    if (!p->size)
        return;
    if (p->needsDetach()) {
        // must allocate memory
        QListData detached(allocate(p->allocatedCapacity()));
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
}

void AutoListAccess::emplace(QListData* p, JNIEnv * env, jint index, jobject value, jint n)
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

void AutoListAccess::emplace(QListData* p, qsizetype index, const void* value, qsizetype n)
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

std::pair<const QtPrivate::QMetaTypeInterface *,const QtPrivate::QMetaTypeInterface *> AutoListAccess::metaTypes(){
    return {m_elementMetaType.iface(), nullptr};
}

void AutoListAccess::swapAndDestroy(QListData* p, QListData&& detached){
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

qsizetype AutoListAccess::freeSpaceAtBegin(const QListData* p){
    if (p->d == nullptr)
        return 0;
    qsizetype result = p->ptr - QTypedArrayData<char>::dataStart(p->d, qMax(size_t(m_elementMetaType.alignOf()), alignof(QArrayData)));
    return result / m_offset;
}

qsizetype AutoListAccess::freeSpaceAtEnd(const QListData* p){
    if (p->d == nullptr)
        return 0;
    return p->d->constAllocatedCapacity() - freeSpaceAtBegin(p) - p->size;
}

void AutoListAccess::detachAndGrow(QListData* p, QArrayData::GrowthPosition where, qsizetype n, const void **,
                   QListData *old)
{
    if (!p->needsDetach()) {
        if (!n || (where == QArrayData::GrowsAtBeginning && freeSpaceAtBegin(p) >= n)
            || (where == QArrayData::GrowsAtEnd && freeSpaceAtEnd(p) >= n))
            return;
    }

    reallocateAndGrow(p, where, n, old);
}

void AutoListAccess::reallocateAndGrow(QListData* p, QArrayData::GrowthPosition where, qsizetype n, QListData *old)
{
    if(m_elementMetaType.flags() & QMetaType::RelocatableType && size_t(m_elementMetaType.alignOf()) <= alignof(std::max_align_t)) {
        if (where == QArrayData::GrowsAtEnd && !old && !p->needsDetach() && n > 0) {
            reallocate(p, p->constAllocatedCapacity() - freeSpaceAtEnd(p) + n, QArrayData::Grow); // fast path
            return;
        }
    }

    QListData dp(allocateGrow(*p, n, where));
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

auto AutoListAccess::allocateGrow(const QListData &from, qsizetype n, QArrayData::GrowthPosition position) -> QListData
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
#if __has_builtin(__builtin_assume_aligned)
    switch(m_elementMetaType.alignOf()){
    case 1:
        if constexpr (Q_ALIGNOF(QArrayData)>1){
            dataPtr = __builtin_assume_aligned(dataPtr, Q_ALIGNOF(QArrayData));
        }else{
            dataPtr = __builtin_assume_aligned(dataPtr, 1);
        }
        break;
    case 2:
        if constexpr (Q_ALIGNOF(QArrayData)>2){
            dataPtr = __builtin_assume_aligned(dataPtr, Q_ALIGNOF(QArrayData));
        }else{
            dataPtr = __builtin_assume_aligned(dataPtr, 2);
        }
        break;
    case 4:
        if constexpr (Q_ALIGNOF(QArrayData)>4){
            dataPtr = __builtin_assume_aligned(dataPtr, Q_ALIGNOF(QArrayData));
        }else{
            dataPtr = __builtin_assume_aligned(dataPtr, 4);
        }
        break;
    case 8:
        if constexpr (Q_ALIGNOF(QArrayData)>8){
            dataPtr = __builtin_assume_aligned(dataPtr, Q_ALIGNOF(QArrayData));
        }else{
            dataPtr = __builtin_assume_aligned(dataPtr, 8);
        }
        break;
    }
#endif
    const bool valid = header != nullptr && dataPtr != nullptr;
    if (!valid)
        return QListData(reinterpret_cast<QTypedArrayData<char>*>(header), reinterpret_cast<char*>(dataPtr));

    // Idea: * when growing backwards, adjust pointer to prepare free space at the beginning
    //       * when growing forward, adjust by the previous data pointer offset
    char* cdataPtr = reinterpret_cast<char*>(dataPtr);
    cdataPtr += ((position == QArrayData::GrowsAtBeginning)
                     ? n + qMax(0, (header->alloc - from.size - n) / 2)
                     : freeSpaceAtBegin(&from)) * m_offset;
    dataPtr = cdataPtr;
    header->flags = from.flags();
    return QListData(reinterpret_cast<QTypedArrayData<char>*>(header), reinterpret_cast<char*>(dataPtr));
}

void AutoListAccess::reallocate(QListData* p, qsizetype capacity, QArrayData::AllocationOption option){
    QPair<QArrayData *, void *> pair =
        QArrayData::reallocateUnaligned(p->d, p->ptr, m_elementMetaType.sizeOf(), capacity, option);
    Q_CHECK_PTR(pair.second);
    Q_ASSERT(pair.first != nullptr);
    p->d = reinterpret_cast<QTypedArrayData<char>*>(pair.first);
    p->ptr = reinterpret_cast<char*>(pair.second);
}

void AutoListAccess::detach(QListData* p, QListData *old)
{
    if(p->needsDetach())
        reallocateAndGrow(p, QArrayData::GrowsAtEnd, 0, old);
}

void *AutoListAccess::createHole(QListData *p, QArrayData::GrowthPosition pos, qsizetype where, qsizetype n){
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

auto AutoListAccess::allocate(qsizetype capacity, QArrayData::AllocationOption option) -> QListData {
    QArrayData *header;
    void *dataPtr = QArrayData::allocate(&header, m_elementMetaType.sizeOf(),
                                         qMax<size_t>(m_elementMetaType.alignOf(), alignof(QArrayData)),
                                         capacity,
                                         option);
#if __has_builtin(__builtin_assume_aligned)
    switch(m_elementMetaType.alignOf()){
    case 1:
        if constexpr (Q_ALIGNOF(QArrayData)>1){
            dataPtr = __builtin_assume_aligned(dataPtr, Q_ALIGNOF(QArrayData));
        }else{
            dataPtr = __builtin_assume_aligned(dataPtr, 1);
        }
        break;
    case 2:
        if constexpr (Q_ALIGNOF(QArrayData)>2){
            dataPtr = __builtin_assume_aligned(dataPtr, Q_ALIGNOF(QArrayData));
        }else{
            dataPtr = __builtin_assume_aligned(dataPtr, 2);
        }
        break;
    case 4:
        if constexpr (Q_ALIGNOF(QArrayData)>4){
            dataPtr = __builtin_assume_aligned(dataPtr, Q_ALIGNOF(QArrayData));
        }else{
            dataPtr = __builtin_assume_aligned(dataPtr, 4);
        }
        break;
    case 8:
        if constexpr (Q_ALIGNOF(QArrayData)>8){
            dataPtr = __builtin_assume_aligned(dataPtr, Q_ALIGNOF(QArrayData));
        }else{
            dataPtr = __builtin_assume_aligned(dataPtr, 8);
        }
        break;
    }
#endif
    return QListData(reinterpret_cast<QTypedArrayData<char>*>(header), reinterpret_cast<char*>(dataPtr));
}

jint AutoListAccess::capacity(JNIEnv *, const void* container)
{
    const QListData* p = reinterpret_cast<const QListData*>(container);
    return qsizetype(p->constAllocatedCapacity());
}

void AutoListAccess::fill(JNIEnv * env, const ContainerInfo& container, jobject value, jint newSize)
{
    QListData* p = reinterpret_cast<QListData*>(container.container);
    if (newSize == -1)
        newSize = p->size;
    jvalue _value;
    _value.l = value;
    if (p->needsDetach() || newSize > p->constAllocatedCapacity()) {
        // must allocate memory
        qsizetype sz = p->detachCapacity(newSize);
        QListData detached(allocate(sz));
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

void AutoListAccess::resize(void* container, qsizetype newSize)
{
    QListData* p = reinterpret_cast<QListData*>(container);
    if (p->needsDetach() || newSize > p->constAllocatedCapacity() - freeSpaceAtBegin(p)) {
        detachAndGrow(p, QArrayData::GrowsAtEnd, newSize - p->size, nullptr, nullptr);
        if (newSize > p->size){
            for(size_t i=p->size; i<size_t(newSize); ++i){
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

void AutoListAccess::resize(JNIEnv * env, const ContainerInfo& container, jint newSize)
{
    QListData* p = reinterpret_cast<QListData*>(container.container);
    if (p->needsDetach() || newSize > capacity(env, container.container) - freeSpaceAtBegin(p)) {
        detachAndGrow(p, QArrayData::GrowsAtEnd, newSize - p->size, nullptr, nullptr);
        if (newSize > p->size){
            for(size_t i=p->size; i<size_t(newSize); ++i){
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

void AutoListAccess::squeeze(JNIEnv *env, const ContainerInfo& container)
{
    QListData* p = reinterpret_cast<QListData*>(container.container);
    if (!p->isMutable())
        return;
    if (p->needsDetach() || p->size < capacity(env, container.container)) {
        // must allocate memory
        QListData detached(allocate(p->size));
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

PointerRCAutoListAccess::PointerRCAutoListAccess(PointerRCAutoListAccess& other)
    : AutoListAccess(other), ReferenceCountingSetContainer() {}

PointerRCAutoListAccess* PointerRCAutoListAccess::clone(){
    return new PointerRCAutoListAccess(*this);
}

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
AutoSpanAccess* PointerRCAutoListAccess::createSpanAccess(bool isConst){
    if(isConst){
        return new AutoSpanAccess(
            m_elementMetaType,
            m_hashFunction,
            m_internalToExternalConverter,
            m_externalToInternalConverter,
            m_elementNestedContainerAccess,
            m_elementOwnerFunction,
            m_elementDataType,
            true
            );
    }else{
        return new PointerRCAutoSpanAccess(
                m_elementMetaType,
                m_hashFunction,
                m_internalToExternalConverter,
                m_externalToInternalConverter,
                m_elementNestedContainerAccess,
                m_elementOwnerFunction,
                m_elementDataType
            );
    }
}
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)

void PointerRCAutoListAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
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

void PointerRCAutoListAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    AutoListAccess::swap(env, container, container2);
    if(PointerRCAutoListAccess* access = dynamic_cast<PointerRCAutoListAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void PointerRCAutoListAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    AutoListAccess::assign(env, container, container2);
    if(PointerRCAutoListAccess* access = dynamic_cast<PointerRCAutoListAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void PointerRCAutoListAccess::appendList(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) {
    AutoListAccess::appendList(env, container, containerInfo);
    addAllRC(env, container.object, findContainer(env, containerInfo.object));
}

void PointerRCAutoListAccess::replace(JNIEnv * env, const ContainerInfo& container, jint index, jobject value) {
    jobject oldValue = AutoListAccess::at(env, container.container, index);
    AutoListAccess::replace(env, container, index, value);
    if(oldValue && !AutoListAccess::contains(env, container.container, oldValue))
        removeRC(env, container.object, oldValue);
    if(value)
        addRC(env, container.object, value);
}

jint PointerRCAutoListAccess::removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) {
    jint result = AutoListAccess::removeAll(env, container, value);
    removeRC(env, container.object, value, result);
    return result;
}

void PointerRCAutoListAccess::insert(JNIEnv * env, const ContainerInfo& container, jint index, jint n, jobject value) {
    AutoListAccess::insert(env, container, index, n, value);
    if(value)
        addRC(env, container.object, value);
}

void PointerRCAutoListAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    AutoListAccess::clear(env, container);
    clearRC(env, container.object);
}

void PointerRCAutoListAccess::remove(JNIEnv * env, const ContainerInfo& container, jint index, jint n) {
    if(n==1){
        jobject oldValue = AutoListAccess::at(env, container.container, index);
        AutoListAccess::remove(env, container, index, n);
        removeRC(env, container.object, oldValue);
    }else{
        jint size = AutoListAccess::size(env, container.container);
        jobject removedValues = Java::Runtime::ArrayList::newInstance(env);
        for(jint i = index; i<=index+n && i<size; ++i){
            Java::Runtime::Collection::add(env, removedValues, AutoListAccess::at(env, container.container, i));
        }
        AutoListAccess::remove(env, container, index, n);
        jobject iter = Java::Runtime::Collection::iterator(env, removedValues);
        while(Java::Runtime::Iterator::hasNext(env, iter)){
            jobject value = Java::Runtime::Iterator::next(env, iter);
            removeRC(env, container.object, value);
        }
    }
}

void PointerRCAutoListAccess::fill(JNIEnv * env, const ContainerInfo& container, jobject value, jint size){
    jint oldSize = AutoListAccess::size(env, container.container);
    AutoListAccess::fill(env, container, value, size);
    for(;oldSize<size;++oldSize){
        addRC(env, container.object, value);
    }
}

NestedPointersRCAutoListAccess::NestedPointersRCAutoListAccess(NestedPointersRCAutoListAccess& other)
    : AutoListAccess(other), ReferenceCountingSetContainer() {}

NestedPointersRCAutoListAccess* NestedPointersRCAutoListAccess::clone(){
    return new NestedPointersRCAutoListAccess(*this);
}

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
AutoSpanAccess* NestedPointersRCAutoListAccess::createSpanAccess(bool isConst){
    if(isConst){
        return new AutoSpanAccess(
            m_elementMetaType,
            m_hashFunction,
            m_internalToExternalConverter,
            m_externalToInternalConverter,
            m_elementNestedContainerAccess,
            m_elementOwnerFunction,
            m_elementDataType,
            true
            );
    }else{
        return new NestedPointersRCAutoSpanAccess(
            m_elementMetaType,
            m_hashFunction,
            m_internalToExternalConverter,
            m_externalToInternalConverter,
            m_elementNestedContainerAccess,
            m_elementOwnerFunction,
            m_elementDataType
            );
    }
}
#endif //QT_VERSION >= QT_VERSION_CHECK(6,7,0)


void NestedPointersRCAutoListAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    AutoListAccess::swap(env, container, container2);
    if(NestedPointersRCAutoListAccess* access = dynamic_cast<NestedPointersRCAutoListAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void NestedPointersRCAutoListAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    AutoListAccess::assign(env, container, container2);
    updateRC(env, container);
}

void NestedPointersRCAutoListAccess::appendList(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) {
    AutoListAccess::appendList(env, container, containerInfo);
    updateRC(env, container);
}

void NestedPointersRCAutoListAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
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

void NestedPointersRCAutoListAccess::replace(JNIEnv * env, const ContainerInfo& container, jint index, jobject value) {
    AutoListAccess::replace(env, container, index, value);
    updateRC(env, container);
}

jint NestedPointersRCAutoListAccess::removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) {
    jint result = AutoListAccess::removeAll(env, container, value);
    if(result>0){
        updateRC(env, container);
    }
    return result;
}

ContainerAndAccessInfo NestedPointersRCAutoListAccess::mid(JNIEnv * env, const ConstContainerAndAccessInfo& container, jint index1, jint index2) {
    ContainerAndAccessInfo result = AutoListAccess::mid(env, container, index1, index2);
    return result;
}

void NestedPointersRCAutoListAccess::insert(JNIEnv * env, const ContainerInfo& container, jint index, jint n, jobject value) {
    AutoListAccess::insert(env, container, index, n, value);
    addNestedValueRC(env, container.object, elementType(), hasNestedPointers(), value);
}

void NestedPointersRCAutoListAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    AutoListAccess::clear(env, container);
    clearRC(env, container.object);
}

void NestedPointersRCAutoListAccess::remove(JNIEnv * env, const ContainerInfo& container, jint index, jint n) {
    AutoListAccess::remove(env, container, index, n);
    updateRC(env, container);
}

void NestedPointersRCAutoListAccess::fill(JNIEnv * env, const ContainerInfo& container, jobject value, jint size){
    AutoListAccess::fill(env, container, value, size);
    addNestedValueRC(env, container.object, elementType(), hasNestedPointers(), value);
}

