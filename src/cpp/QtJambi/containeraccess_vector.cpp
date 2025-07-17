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

#include <QtCore/QtGlobal>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

#include "qtjambiapi.h"
#include <QtCore/QMap>
#include <QtCore/QSharedPointer>
#include "containeraccess_p.h"
#include "functionpointer.h"
#include "registryutil_p.h"
#include "utils_p.h"
#include "qtjambilink_p.h"
#include "java_p.h"
#include "coreapi.h"

QT_WARNING_DISABLE_GCC("-Winaccessible-base")
QT_WARNING_DISABLE_CLANG("-Winaccessible-base")

AutoVectorAccess::AutoVectorAccess(
        int elementMetaType,
        size_t elementAlign,
        const QtJambiUtils::QHashFunction& hashFunction,
        const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
        const QtJambiUtils::ExternalToInternalConverter& externalToInternalConverter,
        const QSharedPointer<AbstractContainerAccess>& elementNestedContainerAccess,
        PtrOwnerFunction elementOwnerFunction,
        AbstractContainerAccess::DataType elementDataType
        )
    : AbstractVectorAccess(), AbstractNestedSequentialAccess(),
      m_elementMetaType(elementMetaType),
      m_elementAlign(elementAlign),
      m_hashFunction(hashFunction),
      m_internalToExternalConverter(internalToExternalConverter),
      m_externalToInternalConverter(externalToInternalConverter),
      m_elementNestedContainerAccess(elementNestedContainerAccess),
      m_offset(0),
      m_isComplex(!AbstractContainerAccess::isPointerType(m_elementMetaType)
                  &&  (m_elementMetaType.id()!=QMetaType::Int)
                  &&  (m_elementMetaType.id()!=QMetaType::UInt)
                  &&  (m_elementMetaType.id()!=QMetaType::Char)
                  &&  (m_elementMetaType.id()!=QMetaType::UChar)
                  &&  (m_elementMetaType.id()!=QMetaType::SChar)
                  &&  (m_elementMetaType.id()!=QMetaType::Nullptr)
                  &&  (m_elementMetaType.id()!=QMetaType::Long)
                  &&  (m_elementMetaType.id()!=QMetaType::ULong)
                  &&  (m_elementMetaType.id()!=QMetaType::LongLong)
                  &&  (m_elementMetaType.id()!=QMetaType::ULongLong)
                  &&  (m_elementMetaType.id()!=QMetaType::Bool)
                  &&  (m_elementMetaType.id()!=QMetaType::Double)
                  &&  (m_elementMetaType.id()!=QMetaType::Float)
                  &&  (m_elementMetaType.id()!=QMetaType::Short)
                  &&  (m_elementMetaType.id()!=QMetaType::UShort)
                  && !(m_elementMetaType.flags() & QMetaType::IsEnumeration)),
      m_elementOwnerFunction(elementOwnerFunction),
      m_elementDataType(elementDataType)
{
    Q_ASSERT(m_elementMetaType.id()!=QMetaType::UnknownType
            && m_elementMetaType.id()!=QMetaType::Void);
    m_offset = m_elementMetaType.sizeOf();
    if(m_offset%elementAlign>0)
        m_offset += elementAlign-m_offset%elementAlign;
}

AutoVectorAccess::AutoVectorAccess(const AutoVectorAccess& other)
  : AbstractVectorAccess(), AbstractNestedSequentialAccess(),
    m_elementMetaType(other.m_elementMetaType.id()),
    m_elementAlign(other.m_elementAlign),
    m_hashFunction(other.m_hashFunction),
    m_internalToExternalConverter(other.m_internalToExternalConverter),
    m_externalToInternalConverter(other.m_externalToInternalConverter),
    m_elementNestedContainerAccess(other.m_elementNestedContainerAccess),
    m_offset(other.m_offset),
    m_isComplex(other.m_isComplex),
    m_elementOwnerFunction(other.m_elementOwnerFunction),
    m_elementDataType(other.m_elementDataType)
{
}

AutoVectorAccess::~AutoVectorAccess(){}

const QMetaType& AutoVectorAccess::elementMetaType()
{
    return m_elementMetaType;
}

AbstractContainerAccess::DataType AutoVectorAccess::elementType() {return m_elementDataType;}

AbstractContainerAccess* AutoVectorAccess::elementNestedContainerAccess() {
    return m_elementNestedContainerAccess ? m_elementNestedContainerAccess->clone() : nullptr;
}
const QSharedPointer<AbstractContainerAccess>& AutoVectorAccess::sharedElementNestedContainerAccess(){
    return m_elementNestedContainerAccess;
}
bool AutoVectorAccess::hasNestedContainerAccess(){
    return !m_elementNestedContainerAccess.isNull();
}
bool AutoVectorAccess::hasNestedPointers(){
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

const QObject* AutoVectorAccess::getOwner(const void* container){
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

bool AutoVectorAccess::hasOwnerFunction(){
    if(m_elementOwnerFunction && !(elementType() & PointersMask))
        return true;
    if(!(elementType() & PointersMask) && m_elementNestedContainerAccess && m_elementNestedContainerAccess->hasOwnerFunction())
        return true;
    return false;
}

void AutoVectorAccess::dispose(){
    delete this;
}

AutoVectorAccess* AutoVectorAccess::clone(){
    return new AutoVectorAccess(*this);
}

size_t AutoVectorAccess::sizeOf(){
    return sizeof(QVector<char>);
}

void* AutoVectorAccess::constructContainer(JNIEnv*, void* result, const ConstContainerAndAccessInfo& container) {
    return constructContainer(result, container.container);
}

std::unique_ptr<AbstractVectorAccess::ElementIterator> AutoVectorAccess::elementIterator(const void* container) {
    class ElementIterator : public AbstractVectorAccess::ElementIterator{
        AutoVectorAccess* m_access;
        char* current;
        char* end;
        ElementIterator(const ElementIterator& other)
            :m_access(other.m_access),
            current(other.current),
            end(other.end) {}
    protected:
        AbstractSequentialAccess* access() override { return m_access; }
    public:
        ElementIterator(AutoVectorAccess* _access, QTypedArrayData<char>* container)
            : AbstractVectorAccess::ElementIterator(),
            m_access(_access),
            current(container->data()),
            end(container->data() + container->size * m_access->m_offset)
        {
        }
        bool hasNext() override{
            return current!=end;
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
            if(m_access->elementType() & AbstractContainerAccess::PointersMask){
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
        bool isConst() override{
            return true;
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
    return std::unique_ptr<AbstractVectorAccess::ElementIterator>(new ElementIterator(this, *reinterpret_cast<QTypedArrayData<char> *const*>(container)));
}

std::unique_ptr<AbstractVectorAccess::ElementIterator> AutoVectorAccess::elementIterator(void* container) {
    class ElementIterator : public AbstractVectorAccess::ElementIterator{
        AutoVectorAccess* m_access;
        char* current;
        char* end;
        ElementIterator(const ElementIterator& other)
            :m_access(other.m_access),
            current(other.current),
            end(other.end) {}
    protected:
        AbstractSequentialAccess* access() override { return m_access; }
    public:
        ElementIterator(AutoVectorAccess* _access, QTypedArrayData<char>* container)
            : AbstractVectorAccess::ElementIterator(),
            m_access(_access),
            current(container->data()),
            end(container->data() + container->size * m_access->m_offset)
        {
        }
        bool hasNext() override{
            return current!=end;
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
            if(m_access->elementType() & AbstractContainerAccess::PointersMask){
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
        bool isConst() override{
            return false;
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
    return std::unique_ptr<AbstractVectorAccess::ElementIterator>(new ElementIterator(this, *reinterpret_cast<QTypedArrayData<char> **>(container)));
}

void* AutoVectorAccess::constructContainer(void* placement){
    return new(placement) QArrayData*(QArrayData::sharedNull());
}

void* AutoVectorAccess::constructContainer(void* placement, const void* container){
    void* result = constructContainer(placement);
    QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(result);
    QTypedArrayData<char>*& d = *vector;
    QTypedArrayData<char>* d2 = *reinterpret_cast<QTypedArrayData<char> *const*>(container);
    if (d2->ref.ref()) {
        d = d2;
    } else {
        if (d2->capacityReserved) {
            d = reinterpret_cast<QTypedArrayData<char>*>(QArrayData::allocate(m_offset, qMax<size_t>(m_elementAlign, alignof(QArrayData)), d2->alloc));
            Q_CHECK_PTR(d);
            d->capacityReserved = true;
        } else {
            d = reinterpret_cast<QTypedArrayData<char>*>(QArrayData::allocate(m_offset, qMax<size_t>(m_elementAlign, alignof(QArrayData)), d2->size));
            Q_CHECK_PTR(d);
        }
        if (d->alloc) {
            char *ddata = d->data();
            char *xdata = d2->data();
            d->size = d2->size;
            for(int i=0, l=d->size; i<l; ++i){
                void* src = ddata + i*m_offset;
                void* dst = xdata + i*m_offset;
                m_elementMetaType.construct(dst, src);
            }
        }
    }
    return result;
}

bool AutoVectorAccess::destructContainer(void* container) {
    QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container);
    QTypedArrayData<char>*& d = *vector;
    if (!d->ref.deref()){
        for(int i = 0; i<d->size; ++i){
            m_elementMetaType.destruct(d->data() + i * m_offset);
        }
        QArrayData::deallocate(d, m_offset, qMax<size_t>(m_elementAlign, alignof(QArrayData)));
    }
    return true;
}

void AutoVectorAccess::assign(JNIEnv *, const ContainerInfo& container, const ConstContainerAndAccessInfo& other){
    assign(container.container, other.container);
}

void AutoVectorAccess::assign(void* container, const void* other)
{
    if(*reinterpret_cast<QTypedArrayData<char> **>(container)!=*reinterpret_cast<QTypedArrayData<char> *const*>(other)){
        void* tmp = createContainer(other);
        QVector<char> & vector = *reinterpret_cast<QVector<char> *>(container);
        QVector<char> & vector2 = *reinterpret_cast<QVector<char> *>(tmp);
        vector.swap(vector2);
        deleteContainer(tmp);
    }
}

int AutoVectorAccess::registerContainer(const QByteArray& typeName)
{
    int newMetaType = QMetaType::type(typeName);
    if(newMetaType==QMetaType::UnknownType){
        QSharedPointer<AutoVectorAccess> access(new AutoVectorAccess(*this), &containerDisposer);
        newMetaType = registerContainerMetaType(typeName,
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
                                       qtjambi_function_pointer<16,void(void*)>([access](void* ptr){
                                            access->destructContainer(ptr);
                                       }, qHash(typeName)),
                                       [](void* result, const void * other) -> void* {
                                            if(other){
                                                return new(result) QVector<char>(*reinterpret_cast<const QVector<char>*>(other));
                                            }else{
                                                return new(result) QVector<char>();
                                            }
                                       },
#endif //QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
                                       uint(sizeOf()),
                                       uint(alignof(QVector<char>)),
                                       QMetaType::NeedsConstruction
                                                   | QMetaType::NeedsDestruction
                                                   | QMetaType::MovableType,
                                       nullptr,
                                       nullptr,
                                       access);
        if(m_hashFunction){
            insertHashFunctionByMetaType(newMetaType,
                                            [access]
                                            (const void* ptr, hash_type seed)->hash_type{
                                                if(ptr){
                                                    hash_type hashCode = seed;
                                                    QTypedArrayData<char> *const* p = reinterpret_cast<QTypedArrayData<char> *const*>(ptr);
                                                    QTypedArrayData<char>* d = *p;
                                                    for(int i=0; i<d->size; ++i){
                                                        void* v = d->data() + i * access->m_offset;
                                                        hashCode = hashCode ^ (access->m_hashFunction(v, 0) + 0x9e3779b9 + (hashCode << 6) + (hashCode >> 2));
                                                    }
                                                    return hashCode;
                                                }else{
                                                    return 0;
                                                }
                                            });
        }
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

                ConverterFunction(const QMetaType& elementMetaType, size_t offset)
                    : QtPrivate::AbstractConverterFunction(convert),
                      _metaType_id(elementMetaType.id()),
                      _metaType_flags(AbstractContainerAccess::isPointerType(elementMetaType)),
                      _iteratorCapabilities(0),
                      _size([](const void *p)->int{
                                QTypedArrayData<char> *const* vector = reinterpret_cast<QTypedArrayData<char> *const*>(p);
                                QTypedArrayData<char>* d = *vector;
                                return d->size;
                            }),
                      _at(qtjambi_function_pointer<16,const void*(const void*, int)>([offset](const void *p, int index)->const void *{
                                QTypedArrayData<char> *const* vector = reinterpret_cast<QTypedArrayData<char> *const*>(p);
                                QTypedArrayData<char>* d = *vector;
                                void* v = d->data() + index * offset;
                                return v;
                            }, qHash(offset))),
                      _moveTo(qtjambi_function_pointer<16,void(const void*,void**,QtMetaTypePrivate::QSequentialIterableImpl::Position)>([offset](const void *p, void ** iter, QtMetaTypePrivate::QSequentialIterableImpl::Position position){
                                    QTypedArrayData<char> *const* vector = reinterpret_cast<QTypedArrayData<char> *const*>(p);
                                    QTypedArrayData<char>* d = *vector;
                                    *iter = position==QtMetaTypePrivate::QSequentialIterableImpl::ToBegin ? d->data() : d->data() + d->size * offset;
                                }, qHash(offset))),
                      _append(nullptr),
                      _advance([](void **p, int steps){
                                    void** iter = *reinterpret_cast<void***>(p);
                                    *reinterpret_cast<void***>(p) = (iter+steps);
                                }),
                      _get([]( void * const *p, int metaTypeId, uint flags)->QtMetaTypePrivate::VariantData{
                                void** iter = *reinterpret_cast<void**const*>(p);
                                return QtMetaTypePrivate::VariantData(metaTypeId, iter, flags);
                           }),
                      _destroyIter([](void **){}),
                      _equalIter([](void * const *p, void * const *other)->bool{
                                        void** iter1 = *reinterpret_cast<void**const*>(p);
                                        void** iter2 = *reinterpret_cast<void**const*>(other);
                                        return iter1==iter2;
                                }),
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
            registerConverter(new ConverterFunction(m_elementMetaType, m_offset), newMetaType, to);
        }
        if(QMetaType::hasRegisteredComparators(m_elementMetaType.id())
            || registeredComparator(m_elementMetaType.id())
            || m_elementMetaType.id()<QMetaType::User
            || AbstractContainerAccess::isPointerType(m_elementMetaType)){
            struct Comparator : QtPrivate::AbstractComparatorFunction{
                static bool lessThan(const AbstractComparatorFunction *_c, const void *a, const void *b){
                    const Comparator* c = static_cast<const Comparator*>(_c);
                    QTypedArrayData<char> *const* vector1 = reinterpret_cast<QTypedArrayData<char> *const*>(a);
                    QTypedArrayData<char> *const* vector2 = reinterpret_cast<QTypedArrayData<char> *const*>(b);
                    QTypedArrayData<char>* d1 = *vector1;
                    QTypedArrayData<char>* d2 = *vector2;
                    if(d1->size<d2->size)
                        return false;
                    if(d1->size>d2->size)
                        return true;
                    if(d1==d2)
                        return false;
                    for(int i = 0; i<d1->size; ++i){
                        void* element1 = d1->data() + i * c->m_offset;
                        void* element2 = d2->data() + i * c->m_offset;
                        if(!isLessThan(c->_metaType_id, element1, element2))
                            return false;
                    }
                    return true;
                }
                static bool equals(const AbstractComparatorFunction *_c, const void *a, const void *b){
                    const Comparator* c = static_cast<const Comparator*>(_c);
                    QTypedArrayData<char> *const* vector1 = reinterpret_cast<QTypedArrayData<char> *const*>(a);
                    QTypedArrayData<char> *const* vector2 = reinterpret_cast<QTypedArrayData<char> *const*>(b);
                    QTypedArrayData<char>* d1 = *vector1;
                    QTypedArrayData<char>* d2 = *vector2;
                    if(d1->size!=d2->size)
                        return false;
                    if(d1==d2)
                        return true;
                    for(int i = 0; i<d1->size; ++i){
                        void* element1 = d1->data() + i * c->m_offset;
                        void* element2 = d2->data() + i * c->m_offset;
                        if(!isEquals(c->_metaType_id, element1, element2))
                            return false;
                    }
                    return true;
                }
                static void destroy(AbstractComparatorFunction *_c){
                    delete static_cast<Comparator*>(_c);
                }
                QMetaType _metaType_id;
                size_t m_offset;

                Comparator(const QMetaType& keyMetaType, size_t offset)
                    : QtPrivate::AbstractComparatorFunction(lessThan, equals, destroy),
                      _metaType_id(keyMetaType.id()), m_offset(offset)
                {

                }
            };
            RegistryAPI::registerComparator(new Comparator(m_elementMetaType, m_offset), newMetaType);
        }
        if(QMetaType::hasRegisteredDebugStreamOperator(m_elementMetaType.id())
                || registeredDebugStreamOperator(m_elementMetaType.id())
                || m_elementMetaType.id()<QMetaType::User
                || AbstractContainerAccess::isPointerType(m_elementMetaType)){
            struct DebugStreamFunction : QtPrivate::AbstractDebugStreamFunction{
                static void stream(const AbstractDebugStreamFunction *_f, QDebug& dbg, const void *ptr){
                    const DebugStreamFunction* f = static_cast<const DebugStreamFunction*>(_f);
                    QTypedArrayData<char> *const* vector = reinterpret_cast<QTypedArrayData<char> *const*>(ptr);
                    QTypedArrayData<char>* d = *vector;
                    dbg = dbg.nospace().noquote();
                    dbg << "QVector(";
                    for(int i=0; i<d->size; ++i){
                        if(i>0)
                            dbg << ", ";
                        CoreAPI::appendToDebugStream(dbg, f->_metaType_id, d->data() + i * f->m_offset);
                    }
                    dbg << ")";
                }
                static void destroy(AbstractDebugStreamFunction *_f){
                    delete static_cast<DebugStreamFunction*>(_f);
                }
                int _metaType_id;
                size_t m_offset;

                DebugStreamFunction(const QMetaType& keyMetaType, size_t offset)
                    : QtPrivate::AbstractDebugStreamFunction(stream, destroy),
                      _metaType_id(keyMetaType.id()), m_offset(offset)
                {

                }
            };
            RegistryAPI::registerDebugStreamOperator(new DebugStreamFunction(m_elementMetaType, m_offset), newMetaType);
        }
        {
            QMetaType::registerStreamOperators(newMetaType,
                                               qtjambi_function_pointer<16,void(QDataStream &, const void *)>(
                                                                               [access](QDataStream &s, const void * ptr){
                                                                                    QTypedArrayData<char> *const* vector = reinterpret_cast<QTypedArrayData<char> *const*>(ptr);
                                                                                    QTypedArrayData<char>* d = *vector;
                                                                                    s << d->size;
                                                                                    for(int i=0; i<d->size; ++i){
                                                                                        void* element = d->data() + i * access->m_offset;
                                                                                        QMetaType::save(s, access->m_elementMetaType.id(), element);
                                                                                    }
                                                                               }, access->m_elementMetaType.id()),
                                               qtjambi_function_pointer<16,void(QDataStream &, void *)>(
                                                                               [access](QDataStream &s, void * ptr){
                                                                                   QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(ptr);
                                                                                   QTypedArrayData<char>*& d = *vector;
                                                                                   int size = 0;
                                                                                   s >> size;
                                                                                   if(size>0){
                                                                                       access->realloc(vector, d->size + size);
                                                                                       for(int i=0; i<size; ++i){
                                                                                            void* element = d->data() + i * access->m_offset;
                                                                                            QMetaType::construct(access->m_elementMetaType.id(), element, nullptr);
                                                                                            QMetaType::load(s, access->m_elementMetaType.id(), element);
                                                                                       }
                                                                                       d->size += size;
                                                                                   }
                                                                               }, access->m_elementMetaType.id())
                                               );
        }
    }else{
        registerContainerAccess(newMetaType, this);
    }
    return newMetaType;
}

jobject AutoVectorAccess::createConstIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr)
{
    AutoSequentialConstIteratorAccess* containerAccess = new AutoSequentialConstIteratorAccess(m_internalToExternalConverter,
                                                 [](AutoSequentialConstIteratorAccess* access, void*ptr){
                                                    char* cursor = *reinterpret_cast<char**>(ptr);
                                                    *reinterpret_cast<char**>(ptr) = cursor+access->offset();
                                                 },
                                                 [](AutoSequentialConstIteratorAccess* access, void*ptr){
                                                    char* cursor = *reinterpret_cast<char**>(ptr);
                                                    *reinterpret_cast<char**>(ptr) = cursor-access->offset();
                                                 },
                                                 [](AutoSequentialConstIteratorAccess*,const void*ptr)->const void*{
                                                    return *reinterpret_cast<void*const*>(ptr);
                                                 },
                                                 [](AutoSequentialConstIteratorAccess*,const void*ptr1,const void*ptr2)->bool{
                                                    return *reinterpret_cast<void*const*>(ptr1)<*reinterpret_cast<void*const*>(ptr2);
                                                 },
                                                 [](AutoSequentialConstIteratorAccess*,const void*ptr1,const void*ptr2)->bool{
                                                    return *reinterpret_cast<void*const*>(ptr1)==*reinterpret_cast<void*const*>(ptr2);
                                                 },
                                                 m_elementMetaType,
                                                 m_offset
                                            );
    return QtJambiAPI::convertQSequentialIteratorToJavaObject(env, ownerId, iteratorPtr, [](void* ptr,bool){
        delete reinterpret_cast<void**>(ptr);
    }, containerAccess);
}

jobject AutoVectorAccess::createIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr)
{
    AbstractSequentialIteratorAccess* containerAccess = new AutoSequentialIteratorAccess(m_internalToExternalConverter,
                                                                   [](AutoSequentialIteratorAccess* access, void*ptr){
                                                                      char* cursor = *reinterpret_cast<char**>(ptr);
                                                                      *reinterpret_cast<char**>(ptr) = cursor+access->offset();
                                                                   },
                                                                   [](AutoSequentialIteratorAccess* access, void*ptr){
                                                                      char* cursor = *reinterpret_cast<char**>(ptr);
                                                                      *reinterpret_cast<char**>(ptr) = cursor-access->offset();
                                                                   },
                                                                   [](AutoSequentialIteratorAccess*,const void*ptr)->const void*{
                                                                      return *reinterpret_cast<void*const*>(ptr);
                                                                   },
                                                                   [](AutoSequentialIteratorAccess*,const void*ptr1,const void*ptr2)->bool{
                                                                      return *reinterpret_cast<void*const*>(ptr1)<*reinterpret_cast<void*const*>(ptr2);
                                                                   },
                                                                   [](AutoSequentialIteratorAccess*,const void*ptr1,const void*ptr2)->bool{
                                                                      return *reinterpret_cast<void*const*>(ptr1)==*reinterpret_cast<void*const*>(ptr2);
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

bool AutoVectorAccess::isDetached(const void* container){
    return isDetached(reinterpret_cast<QTypedArrayData<char> *const*>(container));
}

void AutoVectorAccess::detach(const ContainerInfo& container){
    QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container.container);
    detach(vector);
}

bool AutoVectorAccess::isSharedWith(const void* container, const void* container2){
    QTypedArrayData<char> *const* vector = reinterpret_cast<QTypedArrayData<char> *const*>(container);
    QTypedArrayData<char> *const* vector2 = reinterpret_cast<QTypedArrayData<char> *const*>(container2);
    if(vector && vector2){
        return (*vector)==(*vector2);
    }
    return false;
}

void AutoVectorAccess::swap(JNIEnv *, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    QVector<char> & vector = *reinterpret_cast<QVector<char> *>(container.container);
    QVector<char> & vector2 = *reinterpret_cast<QVector<char> *>(container2.container);
    vector.swap(vector2);
}

jobject AutoVectorAccess::end(JNIEnv * env, const ExtendedContainerInfo& container)
{
    QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container.container);
    detach(vector);
    QTypedArrayData<char>* d = *vector;
    void* iteratorPtr = new void*(d->data() + d->size*m_offset);
    return createIterator(env, container.nativeId, iteratorPtr);
}

jobject AutoVectorAccess::begin(JNIEnv * env, const ExtendedContainerInfo& container)
{
    QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container.container);
    detach(vector);
    QTypedArrayData<char>* d = *vector;
    void* iteratorPtr = new void*(d->data());
    return createIterator(env, container.nativeId, iteratorPtr);
}

jobject AutoVectorAccess::constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container)
{
    QTypedArrayData<char> *const* vector = reinterpret_cast<QTypedArrayData<char> *const*>(container.container);
    QTypedArrayData<char>* d = *vector;
    void* iteratorPtr = new void*(d->data() + d->size*m_offset);
    return createConstIterator(env, container.nativeId, iteratorPtr);
}

jobject AutoVectorAccess::constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container)
{
    QTypedArrayData<char> *const* vector = reinterpret_cast<QTypedArrayData<char> *const*>(container.container);
    QTypedArrayData<char>* d = *vector;
    void* iteratorPtr = new void*(d->data());
    return createConstIterator(env, container.nativeId, iteratorPtr);
}

void AutoVectorAccess::appendVector(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo)
{
    if (ContainerAPI::getAsQVector(env, containerInfo.object, elementMetaType(), containerInfo.container, containerInfo.access)) {
        QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container.container);
        QTypedArrayData<char> ** vector2 = reinterpret_cast<QTypedArrayData<char> **>(containerInfo.container);
        QTypedArrayData<char>*& d = *vector;
        QTypedArrayData<char>*& d2 = *vector2;
        if (d2->size==0) {
            if (d == QArrayData::sharedNull()) {
                *vector = d2;
                d2->ref.ref();
            } else {
                uint newSize = d->size + d2->size;
                const bool isTooSmall = newSize > d->alloc;
                if (!isDetached(vector) || isTooSmall) {
                    QArrayData::AllocationOptions opt(isTooSmall ? QArrayData::Grow : QArrayData::Default);
                    realloc(vector, isTooSmall ? newSize : d->alloc, opt);
                }

                if (d->alloc) {
                    char *w = d->data() + newSize * m_offset;
                    char *i = d2->data() + d->size * m_offset;
                    char *b = d2->data();
                    while (i != b) {
                        i -= m_offset;
                        w -= m_offset;
                        m_elementMetaType.construct(w, i);
                    }
                    d->size = newSize;
                }
            }
        }
    }else{
        QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container.container);
        jobject iter = QtJambiAPI::iteratorOfJavaIterable(env, containerInfo.object);
        reserve(env, container, jint((*vector)->size + QtJambiAPI::sizeOfJavaCollection(env, containerInfo.object)));
        while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
            insert(env, container, (*vector)->size, 1, QtJambiAPI::nextOfJavaIterator(env, iter));
        }
    }
}

jobject AutoVectorAccess::at(JNIEnv * env, const void* container, jint index)
{
    jvalue _value;
    _value.l = nullptr;
    QTypedArrayData<char> *const* vector = reinterpret_cast<QTypedArrayData<char> *const*>(container);
    QTypedArrayData<char>* d = *vector;
    Q_ASSERT_X(index >= 0 && index < d->size, "QVector<T>::at", "index out of range");
    char* data = d->data();
    void* v = data + index*m_offset;
    if(m_internalToExternalConverter(env, nullptr, v, _value, true)){
        return _value.l;
    }
    return nullptr;
}

jobject AutoVectorAccess::value(JNIEnv * env, const void* container, jint index)
{
    jvalue _value;
    _value.l = nullptr;
    QTypedArrayData<char> *const* vector = reinterpret_cast<QTypedArrayData<char> *const*>(container);
    QTypedArrayData<char>* d = *vector;
    if(index >= 0 && index < d->size){
        char* data = d->data();
        void* v = data + index*m_offset;
        if(m_internalToExternalConverter(env, nullptr, v, _value, true)){
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

jobject AutoVectorAccess::value(JNIEnv * env, const void* container, jint index, jobject defaultValue)
{
    jvalue _value;
    _value.l = nullptr;
    QTypedArrayData<char> *const* vector = reinterpret_cast<QTypedArrayData<char> *const*>(container);
    QTypedArrayData<char>* d = *vector;
    if(index >= 0 && index < d->size){
        char* data = d->data();
        void* v = data + index*m_offset;
        if(m_internalToExternalConverter(env, nullptr, v, _value, true)){
            return _value.l;
        }
    }else{
        return defaultValue;
    }
    return nullptr;
}

void AutoVectorAccess::realloc(QTypedArrayData<char> ** vector, int aalloc, QArrayData::AllocationOptions options)
{
    QTypedArrayData<char>*& d = *vector;
    Q_ASSERT(aalloc >= d->size);
    QTypedArrayData<char> *x = d;

    QT_TRY {
        // allocate memory
        x = reinterpret_cast<QTypedArrayData<char>*>(QArrayData::allocate(m_offset, qMax<size_t>(m_elementAlign, alignof(QArrayData)), aalloc, options));
        Q_CHECK_PTR(x);
        // aalloc is bigger then 0 so it is not [un]sharedEmpty
#if !defined(QT_NO_UNSHARABLE_CONTAINERS)
        Q_ASSERT(x->ref.isSharable() || options.testFlag(QArrayData::Unsharable));
#endif
        Q_ASSERT(!x->ref.isStatic());
        x->size = d->size;

        char *ddata = d->data();
        char *xdata = x->data();

        for(int i=0, l=d->size; i<l; ++i){
            void* src = ddata + i*m_offset;
            void* dst = xdata + i*m_offset;
            m_elementMetaType.construct(dst, src);
        }

    } QT_CATCH (...) {
        QArrayData::deallocate(x, m_offset, qMax<size_t>(m_elementAlign, alignof(QArrayData)));
        QT_RETHROW;
    }
    x->capacityReserved = d->capacityReserved;

    Q_ASSERT(d != x);
    if (!d->ref.deref()) {
        char *ddata = d->data();
        for(int i=0, l=d->size; i<l; ++i){
            m_elementMetaType.destruct(ddata + i*m_offset);
        }
        QArrayData::deallocate(d, m_offset, qMax<size_t>(m_elementAlign, alignof(QArrayData)));
    }
    d = x;

    Q_ASSERT(d->data());
    Q_ASSERT(uint(d->size) <= d->alloc);
#if !defined(QT_NO_UNSHARABLE_CONTAINERS)
    Q_ASSERT(d != unsharableEmpty());
#endif
    Q_ASSERT(d != QArrayData::sharedNull());
    Q_ASSERT(d->alloc >= uint(aalloc));
}

QTypedArrayData<char>* AutoVectorAccess::unsharableEmpty(){
    return reinterpret_cast<QTypedArrayData<char>*>(QArrayData::allocate(m_offset, qMax<size_t>(m_elementAlign, alignof(QArrayData)), /* capacity */ 0, QArrayData::Unsharable));
}

bool AutoVectorAccess::isDetached(QTypedArrayData<char> *const* vector)
{
    QTypedArrayData<char>* d = *vector;
    return d && !d->ref.isShared();
}

void AutoVectorAccess::detach(QTypedArrayData<char> ** vector)
{
    if (!isDetached(vector)) {
#if !defined(QT_NO_UNSHARABLE_CONTAINERS)
        QTypedArrayData<char>*& d = *vector;
        if (!d->alloc)
            d = unsharableEmpty();
        else
#endif
            realloc(vector, int(d->alloc));
    }
    Q_ASSERT(isDetached(vector));
}

void AutoVectorAccess::swapItemsAt(JNIEnv *, const ContainerInfo& container, jint i, jint j)
{
    QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container.container);
    QTypedArrayData<char>*& d = *vector;
    Q_ASSERT_X(i >= 0 && i < d->size && j >= 0 && j < d->size,
                "QVector<T>::swap", "index out of range");
    detach(vector);
    char* data = d->data();
    void* element1 = data + i * m_offset;
    void* element2 = data + j * m_offset;
    void* tmp = m_elementMetaType.create(element1);
    m_elementMetaType.destruct(element1);
    m_elementMetaType.construct(element1, element2);
    m_elementMetaType.destruct(element2);
    m_elementMetaType.construct(element2, tmp);
    m_elementMetaType.destroy(tmp);
}

jboolean AutoVectorAccess::startsWith(JNIEnv * env, const void* container, jobject value)
{
    auto sz = size(env, container);
    if(sz>0){
        QtJambiScope scope;
        void *_qvaluePtr = nullptr;
        jvalue _value;
        _value.l = value;
        if(m_externalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l)){
            QTypedArrayData<char> *const* vector = reinterpret_cast<QTypedArrayData<char> *const*>(container);
            QTypedArrayData<char>* d = *vector;
            return isEquals(m_elementMetaType, _qvaluePtr, d->data());
        }
    }
    return false;
}

qsizetype AutoVectorAccess::size(const void* container)
{
    QTypedArrayData<char> *const* vector = reinterpret_cast<QTypedArrayData<char> *const*>(container);
    QTypedArrayData<char>* d = *vector;
    return d->size;
}

jint AutoVectorAccess::size(JNIEnv *, const void* container)
{
    QTypedArrayData<char> *const* vector = reinterpret_cast<QTypedArrayData<char> *const*>(container);
    QTypedArrayData<char>* d = *vector;
    return d->size;
}

void AutoVectorAccess::reserve(JNIEnv *, const ContainerInfo& container, jint asize)
{
    QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container.container);
    QTypedArrayData<char>*& d = *vector;
    if (asize > int(d->alloc))
        realloc(vector, asize);
    if (isDetached(vector)
#if !defined(QT_NO_UNSHARABLE_CONTAINERS)
            && d != unsharableEmpty()
#endif
            )
        d->capacityReserved = 1;
    Q_ASSERT(jint(d->alloc) >= asize);
}


void AutoVectorAccess::replace(JNIEnv * env, const ContainerInfo& container, jint index, jobject value)
{
        QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container.container);
        QTypedArrayData<char>*& d = *vector;
        Q_ASSERT_X(index >= 0 && index < d->size, "QVector<T>::replace", "index out of range");
        detach(vector);
        char* data = d->data();
        void* target = data + index * m_offset;
        jvalue _value;
        _value.l = value;
        m_externalToInternalConverter(env, nullptr, _value, target, jValueType::l);
}

void AutoVectorAccess::erase(QTypedArrayData<char> ** vector, char* abegin, char* aend)
{
    QTypedArrayData<char>*& d = *vector;
    const auto itemsToErase = aend - abegin;
    if(!itemsToErase)
        return;
    const auto itemsUntouched = abegin - d->data();

    // FIXME we could do a proper realloc, which copy constructs only needed data.
    // FIXME we are about to delete data - maybe it is good time to shrink?
    // FIXME the shrink is also an issue in removeLast, that is just a copy + reduce of this.
    if (d->alloc) {
        detach(vector);
        abegin = d->data() + itemsUntouched;
        aend = abegin + itemsToErase;
        char* moveBegin = abegin + itemsToErase;
        char* moveEnd = d->data() + d->size * m_offset;
        while (moveBegin != moveEnd) {
            m_elementMetaType.destruct(abegin);
            m_elementMetaType.construct(abegin, moveBegin);
            abegin += m_offset;
            moveBegin += m_offset;
        }
        if (abegin < d->data() + d->size * m_offset) {
            // destroy rest of instances
            while (abegin != d->data() + d->size*m_offset) {
                m_elementMetaType.destruct(abegin);
                abegin += m_offset;
            }
        }
        d->size -= int(itemsToErase/m_offset);
    }
}

jint AutoVectorAccess::removeAll(JNIEnv * env, const ContainerInfo& container, jobject value)
{
    QtJambiScope scope;
    jint removedCount = 0;
    void *_qvaluePtr = nullptr;
    jvalue _value;
    _value.l = value;
    if(m_externalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l)){
        QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container.container);
        QTypedArrayData<char>*& d = *vector;
        int index;
        do{
            index = -1;
            for(int i = 0; i<d->size; ++i){
                void* element = d->data() + i * m_offset;
                if(isEquals(m_elementMetaType, _qvaluePtr, element)){
                    erase(vector, d->data() + i * m_offset, d->data() + (i+1) * m_offset);
                    ++removedCount;
                    index = i;
                    break;
                }
            }
        }while(index>=0);
    }
    return removedCount;
}

jboolean AutoVectorAccess::equal(JNIEnv * env, const void* container, jobject other)
{
    QTypedArrayData<char> *const* vector = reinterpret_cast<QTypedArrayData<char> *const*>(container);
    QTypedArrayData<char>* d = *vector;
    void* ptr{nullptr};
    if (ContainerAPI::getAsQVector(env, other, elementMetaType(), ptr)) {
        QTypedArrayData<char> *const* vector2 = reinterpret_cast<QTypedArrayData<char> *const*>(ptr);
        QTypedArrayData<char>* d2 = *vector2;
        if(d->size!=d2->size)
            return false;
        if(d==d2)
            return true;
        for(int i = 0; i<d->size; ++i){
            void* element1 = d->data() + i * m_offset;
            void* element2 = d2->data() + i * m_offset;
            if(!isEquals(m_elementMetaType, element1, element2))
                return false;
        }
        return true;
    }else{
        if(d->size!=QtJambiAPI::sizeOfJavaCollection(env, other))
            return false;
        jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, other);
        size_t i = 0;
        while(QtJambiAPI::hasJavaIteratorNext(env, iterator)){
            QtJambiScope scope;
            jvalue _value;
            _value.l = QtJambiAPI::nextOfJavaIterator(env, iterator);
            void *_qvaluePtr = nullptr;
            if(!m_externalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l))
                return false;
            void* element = d->data() + i * m_offset;
            if(!isEquals(m_elementMetaType, element, _qvaluePtr))
                return false;
        }
        return true;
    }
    return false;
}

void AutoVectorAccess::move(JNIEnv *, const ContainerInfo& container, jint from, jint to)
{
    if (from == to)
        return;
    QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container.container);
    QTypedArrayData<char>*& d = *vector;
    Q_ASSERT_X(from >= 0 && from < d->size && to >= 0 && to < d->size,
               "QVector<T>::move", "index out of range");
    detach(vector);
    void* tmp = m_elementMetaType.create(d->data() + from * m_offset);
    for(jint i=from; i<to; ++i){
        m_elementMetaType.destruct(d->data() + i * m_offset);
        m_elementMetaType.construct(d->data() + i * m_offset, d->data() + (i+1) * m_offset);
    }
    m_elementMetaType.destruct(d->data() + to * m_offset);
    m_elementMetaType.construct(d->data() + to * m_offset, tmp);
    m_elementMetaType.destroy(tmp);
}

ContainerAndAccessInfo AutoVectorAccess::mid(JNIEnv * env, const ConstContainerAndAccessInfo& container, jint _pos, jint _len)
{
    ContainerAndAccessInfo result;
    using namespace QtPrivate;
    int pos = _pos;
    int alength = _len;
    QTypedArrayData<char> *const* vector = reinterpret_cast<QTypedArrayData<char> *const*>(container.container);
    QTypedArrayData<char>* d = *vector;
    switch (QContainerImplHelper::mid(d->size, &pos, &alength)) {
    case QContainerImplHelper::Null:
    case QContainerImplHelper::Empty:{
        result.access = container.access->clone();
        result.container = result.access->createContainer();
        result.object = ContainerAPI::objectFromQVector(env, result.container, result.access);
        return result;
    }
    case QContainerImplHelper::Full:{
        result.access = container.access->clone();
        result.container = result.access->createContainer();
        result.object = ContainerAPI::objectFromQVector(env, result.container, result.access);
        result.access->assign(env, result, container);
        return result;
    }
    case QContainerImplHelper::Subset:
        break;
    }
    if (alength <= 0){
        result.access = container.access->clone();
        result.container = result.access->createContainer();
        result.object = ContainerAPI::objectFromQVector(env, result.container, result.access);
        return result;
    }

    result.access = container.access->clone();
    result.container = result.access->createContainer();
    reserve(env, result, alength);
    QTypedArrayData<char> *const* midVector = reinterpret_cast<QTypedArrayData<char> *const*>(result.container);
    QTypedArrayData<char>* md = *midVector;
    for(int i=0; i<alength; ++i){
        m_elementMetaType.construct(md->data() + i * m_offset, d->data() + (pos+i) * m_offset);
    }
    md->size = alength;
    result.object = ContainerAPI::objectFromQVector(env, result.container, result.access);
    return result;
}

jint AutoVectorAccess::lastIndexOf(JNIEnv * env, const void* container, jobject value, jint index)
{
    QTypedArrayData<char> *const* vector = reinterpret_cast<QTypedArrayData<char> *const*>(container);
    QTypedArrayData<char>* d = *vector;
    jint sz = d->size;
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
            for(int i = index; i>=0; --i){
                if(isEquals(m_elementMetaType, _qvaluePtr, d->data() + i * m_offset))
                    return i;
            }
        }
    }
    return -1;
}

jint AutoVectorAccess::indexOf(JNIEnv * env, const void* container, jobject value, jint index)
{
    QTypedArrayData<char> *const* vector = reinterpret_cast<QTypedArrayData<char> *const*>(container);
    QTypedArrayData<char>* d = *vector;
    jint sz = d->size;
    if (index < 0)
        index = qMax<jint>(index + sz, 0);
    if (index < sz) {
        QtJambiScope scope;
        void *_qvaluePtr = nullptr;
        jvalue _value;
        _value.l = value;
        if(m_externalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l)){
            QTypedArrayData<char> *const* vector = reinterpret_cast<QTypedArrayData<char> *const*>(container);
            QTypedArrayData<char>* d = *vector;
            for(int i = index; i<sz; ++i){
                if(isEquals(m_elementMetaType, _qvaluePtr, d->data() + i * m_offset))
                    return i;
            }
        }
    }
    return -1;
}

jboolean AutoVectorAccess::endsWith(JNIEnv * env, const void* container, jobject value)
{
    auto sz = size(env, container);
    if(sz>0){
        QtJambiScope scope;
        void *_qvaluePtr = nullptr;
        jvalue _value;
        _value.l = value;
        if(m_externalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l)){
            QTypedArrayData<char> *const* vector = reinterpret_cast<QTypedArrayData<char> *const*>(container);
            QTypedArrayData<char>* d = *vector;
            char* data = d->data();
            void* v = data + (d->size-1)*m_offset;
            return isEquals(m_elementMetaType, _qvaluePtr, v);
        }
    }
    return false;
}

jint AutoVectorAccess::count(JNIEnv * env, const void* container, jobject value)
{
    QTypedArrayData<char> *const* vector = reinterpret_cast<QTypedArrayData<char> *const*>(container);
    QTypedArrayData<char>* d = *vector;
    if(d->size==0)
        return 0;
    jint result = 0;
    QtJambiScope scope;
    void *_qvaluePtr = nullptr;
    jvalue _value;
    _value.l = value;
    if(m_externalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l)){
        for(qsizetype i = 0; i<d->size; ++i){
            if(isEquals(m_elementMetaType, _qvaluePtr, d->data() + i * m_offset))
                ++result;
        }
    }
    return result;
}

jboolean AutoVectorAccess::contains(JNIEnv * env, const void* container, jobject value)
{
    QTypedArrayData<char> *const* vector = reinterpret_cast<QTypedArrayData<char> *const*>(container);
    QTypedArrayData<char>* d = *vector;
    if(d->size==0)
        return false;
    QtJambiScope scope;
    void *_qvaluePtr = nullptr;
    jvalue _value;
    _value.l = value;
    if(m_externalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l)){
        for(qsizetype i = 0; i<d->size; ++i){
            if(isEquals(m_elementMetaType, _qvaluePtr, d->data() + i * m_offset))
                return true;
        }
    }
    return false;
}

void AutoVectorAccess::clear(JNIEnv *, const ContainerInfo& container)
{
    QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container.container);
    QTypedArrayData<char>*& d = *vector;
    if (!d->size)
        return;
    for(int i = 0; i<d->size; ++i){
        m_elementMetaType.destruct(d->data() + i * m_offset);
    }
    d->size = 0;
}

jint AutoVectorAccess::capacity(JNIEnv *, const void* container){
    QTypedArrayData<char> *const* vector = reinterpret_cast<QTypedArrayData<char> *const*>(container);
    return (*vector)->alloc;
}

void AutoVectorAccess::fill(JNIEnv * env, const ContainerInfo& container, jobject value, jint asize){
    QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container.container);
    QTypedArrayData<char>*& d = *vector;
    resize(env, container, asize < 0 ? d->size : asize);
    if (d->size) {
        char *i = d->data()+d->size*m_offset;
        char *b = d->data();
        while (i != b){
            i -= m_offset;
            QtJambiScope scope;
            void *_qvaluePtr = i;
            jvalue _value;
            _value.l = value;
            m_externalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l);
        }
    }
}

void AutoVectorAccess::remove(JNIEnv *, const ContainerInfo& container, jint index, jint n){
    QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container.container);
    QTypedArrayData<char>*& d = *vector;
    if (index < 0 || index+n > d->size) {
        return;
    }
    erase(vector, d->data() + index * m_offset, d->data() + (index+n) * m_offset);
}

void AutoVectorAccess::insert(JNIEnv * env, const ContainerInfo& container, jint index, jint n, jobject value){
    QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container.container);
    QTypedArrayData<char>*& d = *vector;
    if (!isDetached(vector) || d->size + n > int(d->alloc))
        realloc(vector, d->size + n, QArrayData::Grow);
    char *b = d->data() + d->size * m_offset;
    char *i = d->data() + (d->size + n) * m_offset;
    while (i != b){
        i -= m_offset;
        m_elementMetaType.construct(i);
    }
    i = d->data() + d->size * m_offset;
    char *j = i + n*m_offset;
    b = d->data() + index * m_offset;
    while (i != b){
        i -= m_offset;
        j -= m_offset;
        m_elementMetaType.destruct(j);
        m_elementMetaType.construct(j, i);
    }
    i = b + n*m_offset;
    while (i != b){
        i -= m_offset;
        void* ptr = i;
        jvalue _value;
        _value.l = value;
        m_externalToInternalConverter(env, nullptr, _value, ptr, jValueType::l);
    }
    d->size += n;
}

void AutoVectorAccess::resize(JNIEnv *, const ContainerInfo& container, jint asize)
{
    QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container.container);
    QTypedArrayData<char>*& d = *vector;
    if (asize == d->size)
        return detach(vector);
    if (asize > int(d->alloc) || !isDetached(vector)) { // there is not enough space
        QArrayData::AllocationOptions opt = asize > int(d->alloc) ? QArrayData::Grow : QArrayData::Default;
        realloc(vector, qMax(jint(d->alloc), asize), opt);
    }
    if (asize < d->size){
        for(int i=asize; i<d->size; ++i){
            m_elementMetaType.destruct(d->data() + i * m_offset);
        }
    }else{
        for(int i=d->size; i<asize; ++i){
            m_elementMetaType.construct(d->data() + i * m_offset);
        }
    }
    d->size = asize;
}

void AutoVectorAccess::squeeze(JNIEnv *, const ContainerInfo& container){
    QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container.container);
    QTypedArrayData<char>*& d = *vector;
    if (d->size < int(d->alloc)) {
        if (!d->size) {
            QVector<char> empty;
            assign(container.container, &empty);
            return;
        }
        realloc(vector, d->size);
    }
    if (d->capacityReserved) {
        // capacity reserved in a read only memory would be useless
        // this checks avoid writing to such memory.
        d->capacityReserved = 0;
    }
}

PointerRCAutoVectorAccess* PointerRCAutoVectorAccess::clone(){
    return new PointerRCAutoVectorAccess(*this);
}

void PointerRCAutoVectorAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
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

void PointerRCAutoVectorAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    AutoVectorAccess::swap(env, container, container2);
    if(PointerRCAutoVectorAccess* access = dynamic_cast<PointerRCAutoVectorAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void PointerRCAutoVectorAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    AutoVectorAccess::assign(env, container, container2);
    if(PointerRCAutoVectorAccess* access = dynamic_cast<PointerRCAutoVectorAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void PointerRCAutoVectorAccess::appendVector(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) {
    AutoVectorAccess::appendVector(env, container, containerInfo);
    addAllRC(env, container.object, findContainer(env, containerInfo.object));
}

void PointerRCAutoVectorAccess::replace(JNIEnv * env, const ContainerInfo& container, jint index, jobject value) {
    jobject oldValue = AutoVectorAccess::at(env, container.container, index);
    AutoVectorAccess::replace(env, container, index, value);
    if(oldValue && !AutoVectorAccess::contains(env, container.container, oldValue))
        removeRC(env, container.object, oldValue);
    if(value)
        addRC(env, container.object, value);
}

jint PointerRCAutoVectorAccess::removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) {
    jint result = AutoVectorAccess::removeAll(env, container, value);
    removeRC(env, container.object, value, result);
    return result;
}

void PointerRCAutoVectorAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    AutoVectorAccess::clear(env, container);
    clearRC(env, container.object);
}

void PointerRCAutoVectorAccess::remove(JNIEnv * env, const ContainerInfo& container, jint index, jint n) {
    jint size = AutoVectorAccess::size(env, container.container);
    jobject removedValues = Java::Runtime::ArrayList::newInstance(env);
    for(jint i = index; i<=index+n && i<size; ++i){
        Java::Runtime::Collection::add(env, removedValues, AutoVectorAccess::at(env, container.container, i));
    }
    AutoVectorAccess::remove(env, container, index, n);
    jobject iter = Java::Runtime::Collection::iterator(env, removedValues);
    while(Java::Runtime::Iterator::hasNext(env, iter)){
        jobject value = Java::Runtime::Iterator::next(env, iter);
        removeRC(env, container.object, value);
    }
}

void PointerRCAutoVectorAccess::insert(JNIEnv * env, const ContainerInfo& container, jint index, jint n, jobject value) {
    AutoVectorAccess::insert(env, container, index, n, value);
    addRC(env, container.object, value);
}

void PointerRCAutoVectorAccess::fill(JNIEnv * env, const ContainerInfo& container, jobject value, jint size){
    jint oldSize = AutoVectorAccess::size(env, container.container);
    AutoVectorAccess::fill(env, container, value, size);
    for(;oldSize<size;++oldSize){
        addRC(env, container.object, value);
    }
}

NestedPointersRCAutoVectorAccess::NestedPointersRCAutoVectorAccess(NestedPointersRCAutoVectorAccess& other)
    : AutoVectorAccess(other), ReferenceCountingSetContainer() {}

NestedPointersRCAutoVectorAccess* NestedPointersRCAutoVectorAccess::clone(){
    return new NestedPointersRCAutoVectorAccess(*this);
}

void NestedPointersRCAutoVectorAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
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

void NestedPointersRCAutoVectorAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    AutoVectorAccess::swap(env, container, container2);
    if(NestedPointersRCAutoVectorAccess* access = dynamic_cast<NestedPointersRCAutoVectorAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void NestedPointersRCAutoVectorAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    AutoVectorAccess::assign(env, container, container2);
    if(NestedPointersRCAutoVectorAccess* access = dynamic_cast<NestedPointersRCAutoVectorAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void NestedPointersRCAutoVectorAccess::appendVector(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) {
    AutoVectorAccess::appendVector(env, container, containerInfo);
    updateRC(env, container);
}

jint NestedPointersRCAutoVectorAccess::removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) {
    jint result = AutoVectorAccess::removeAll(env, container, value);
    if(result>0){
        updateRC(env, container);
    }
    return result;
}

void NestedPointersRCAutoVectorAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    AutoVectorAccess::clear(env, container);
    clearRC(env, container.object);
}

void NestedPointersRCAutoVectorAccess::remove(JNIEnv * env, const ContainerInfo& container, jint index, jint n) {
    AutoVectorAccess::remove(env, container, index, n);
    updateRC(env, container);
}

void NestedPointersRCAutoVectorAccess::insert(JNIEnv * env, const ContainerInfo& container, jint index, jint n, jobject value) {
    AutoVectorAccess::insert(env, container, index, n, value);
    addNestedValueRC(env, container.object, elementType(), hasNestedPointers(), value);
}

void NestedPointersRCAutoVectorAccess::fill(JNIEnv * env, const ContainerInfo& container, jobject value, jint size){
    AutoVectorAccess::fill(env, container, value, size);
    addNestedValueRC(env, container.object, elementType(), hasNestedPointers(), value);
}

void NestedPointersRCAutoVectorAccess::replace(JNIEnv * env, const ContainerInfo& container, jint index, jobject value){
    AutoVectorAccess::replace(env, container, index, value);
    updateRC(env, container);
}

#endif
