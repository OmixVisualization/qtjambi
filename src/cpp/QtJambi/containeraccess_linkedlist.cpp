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

QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Winaccessible-base")
QT_WARNING_DISABLE_CLANG("-Winaccessible-base")


#include "qtjambiapi.h"
#include <QtCore/QReadWriteLock>
#include <QtCore/QMap>
#include <QtCore/QSharedPointer>
#include <QtCore/QLinkedList>
#include "containeraccess_p.h"
#include "functionpointer.h"
#include "registryutil_p.h"
#include "qtjambilink_p.h"
#include "java_p.h"
#include "utils_p.h"
#include "coreapi.h"

AutoLinkedListAccess::AutoLinkedListAccess(
        int elementMetaType,
        size_t elementAlign,
        const QtJambiUtils::QHashFunction& hashFunction,
        const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
        const QtJambiUtils::ExternalToInternalConverter& externalToInternalConverter,
        const QSharedPointer<AbstractContainerAccess>& elementNestedContainerAccess,
        PtrOwnerFunction elementOwnerFunction,
        AbstractContainerAccess::DataType elementDataType
        )
    : AbstractLinkedListAccess(), AbstractNestedSequentialAccess(),
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
    m_offset = 2*sizeof(void*);
    if(m_offset%elementAlign>0)
        m_offset += elementAlign-m_offset%elementAlign;
}

AutoLinkedListAccess::AutoLinkedListAccess(const AutoLinkedListAccess& other)
  : AbstractLinkedListAccess(), AbstractNestedSequentialAccess(),
    m_elementMetaType(other.m_elementMetaType.id()),
    m_hashFunction(other.m_hashFunction),
    m_internalToExternalConverter(other.m_internalToExternalConverter),
    m_externalToInternalConverter(other.m_externalToInternalConverter),
    m_elementNestedContainerAccess(other.m_elementNestedContainerAccess),
    m_offset(other.m_offset),
    m_elementOwnerFunction(other.m_elementOwnerFunction),
    m_elementDataType(other.m_elementDataType)
{
}

std::unique_ptr<AbstractLinkedListAccess::ElementIterator> AutoLinkedListAccess::elementIterator(const void* container) {
    class ElementIterator : public AbstractLinkedListAccess::ElementIterator{
        AutoLinkedListAccess* m_access;
        Node* current;
        Node* end;
        ElementIterator(const ElementIterator& other)
            :m_access(other.m_access),
            current(other.current),
            end(other.end) {}
    protected:
        AbstractSequentialAccess* access() override { return m_access; }
    public:
        ElementIterator(AutoLinkedListAccess* _access, QLinkedListData* d)
            : AbstractLinkedListAccess::ElementIterator(),
            m_access(_access),
            current(reinterpret_cast<Node*>(d)->n),
            end(reinterpret_cast<Node*>(d))
        {}
        bool hasNext() override{
            return current!=end;
        }
        jobject next(JNIEnv * env) override{
            void* data = &current->t;
            current = current->n;
            jvalue _value;
            _value.l = nullptr;
            m_access->m_internalToExternalConverter(env, nullptr, data, _value, true);
            return _value.l;
        }
        const void* next() override {
            void* data = &current->t;
            current = current->n;
            if(m_access->elementType() & AbstractLinkedListAccess::PointersMask){
                return *reinterpret_cast<void**>(data);
            }else{
                return data;
            }
        }
        bool isConst() override{
            return true;
        }
        const void* constNext() override {
            void* data = &current->t;
            current = current->n;
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
    return std::unique_ptr<AbstractLinkedListAccess::ElementIterator>(new ElementIterator(this, *reinterpret_cast<QLinkedListData*const*>(container)));
}

std::unique_ptr<AbstractLinkedListAccess::ElementIterator> AutoLinkedListAccess::elementIterator(void* container) {
    class ElementIterator : public AbstractLinkedListAccess::ElementIterator{
        AutoLinkedListAccess* m_access;
        Node* current;
        Node* end;
        ElementIterator(const ElementIterator& other)
            :m_access(other.m_access),
            current(other.current),
            end(other.end) {}
    protected:
        AbstractSequentialAccess* access() override { return m_access; }
    public:
        ElementIterator(AutoLinkedListAccess* _access, QLinkedListData* d)
            : AbstractLinkedListAccess::ElementIterator(),
            m_access(_access),
            current(reinterpret_cast<Node*>(d)->n),
            end(reinterpret_cast<Node*>(d))
        {}
        bool hasNext() override{
            return current!=end;
        }
        jobject next(JNIEnv * env) override{
            void* data = &current->t;
            current = current->n;
            jvalue _value;
            _value.l = nullptr;
            m_access->m_internalToExternalConverter(env, nullptr, data, _value, true);
            return _value.l;
        }
        const void* next() override {
            void* data = &current->t;
            current = current->n;
            if(m_access->elementType() & AbstractLinkedListAccess::PointersMask){
                return *reinterpret_cast<void**>(data);
            }else{
                return data;
            }
        }
        bool isConst() override{
            return false;
        }
        const void* constNext() override {
            void* data = &current->t;
            current = current->n;
            return data;
        }
        void* mutableNext() override {
            void* data = &current->t;
            current = current->n;
            return data;
        }

        bool operator==(const AbstractSequentialAccess::ElementIterator& other) const override {
            return current==reinterpret_cast<const ElementIterator&>(other).current;
        }
        std::unique_ptr<AbstractSequentialAccess::ElementIterator> clone() const override {
            return std::unique_ptr<AbstractSequentialAccess::ElementIterator>(new ElementIterator(*this));
        }
    };
    return std::unique_ptr<AbstractLinkedListAccess::ElementIterator>(new ElementIterator(this, *reinterpret_cast<QLinkedListData**>(container)));
}

AutoLinkedListAccess* AutoLinkedListAccess::clone(){
    return new AutoLinkedListAccess(*this);
}

void AutoLinkedListAccess::dispose()
{
    delete this;
}

size_t AutoLinkedListAccess::sizeOf(){
    return sizeof(QLinkedList<char>);
}

void* AutoLinkedListAccess::constructContainer(JNIEnv*, void* result, const ConstContainerAndAccessInfo& container) {
    return constructContainer(result, container.container);
}

void* AutoLinkedListAccess::constructContainer(void* placement)
{
    return new(placement) QLinkedList<char>();
}

void* AutoLinkedListAccess::constructContainer(void* placement, const void* container)
{
    placement = constructContainer(placement);
    assign(placement, container);
    return placement;
}

bool AutoLinkedListAccess::destructContainer(void* container)
{
    QLinkedListData** linkedList = reinterpret_cast<QLinkedListData**>(container);
    QLinkedListData *&d = *linkedList;
    if (!d->ref.deref())
        freeData(d);
    return true;
}

void AutoLinkedListAccess::assign(JNIEnv *, const ContainerInfo& container, const ConstContainerAndAccessInfo& other){
    assign(container.container, other.container);
}

void AutoLinkedListAccess::assign(void* container, const void* other)
{
    QLinkedListData** linkedList = reinterpret_cast<QLinkedListData**>(container);
    QLinkedListData*const* linkedList2 = reinterpret_cast<QLinkedListData*const*>(other);
    if (*linkedList != *linkedList2) {
        QLinkedListData *o = *linkedList2;
        o->ref.ref();
        if (!(*linkedList)->ref.deref())
            freeData(*linkedList);
        *linkedList = o;
        if (!(*linkedList)->sharable)
            detach_helper(*linkedList);
    }
}

int AutoLinkedListAccess::registerContainer(const QByteArray& typeName)
{
    int newMetaType = QMetaType::type(typeName);
    if(newMetaType==QMetaType::UnknownType){
        QSharedPointer<AutoLinkedListAccess> access(new AutoLinkedListAccess(*this), &containerDisposer);
        newMetaType = registerContainerMetaType(typeName,
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
                                       qtjambi_function_pointer<16,void(void*)>([access](void* ptr){
                                            access->destructContainer(ptr);
                                       }, qHash(typeName)),
                                       [](void* result, const void * other) -> void* {
                                            if(other){
                                                return new(result) QLinkedList<char>(*reinterpret_cast<const QLinkedList<char>*>(other));
                                            }else{
                                                return new(result) QLinkedList<char>();
                                            }
                                       },
#endif //QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
                                       uint(sizeOf()),
                                       uint(alignof(QLinkedList<char>)),
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
                                                    QLinkedListData*const* linkedList = reinterpret_cast<QLinkedListData*const*>(ptr);
                                                    QLinkedListData* d = *linkedList;
                                                    Node* e = reinterpret_cast<Node*>(d);
                                                    Node *i = e;
                                                    while ((i = i->n) != e){
                                                        hashCode = hashCode ^ (access->m_hashFunction(&i->t, 0) + 0x9e3779b9 + (hashCode << 6) + (hashCode >> 2));
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

                struct Iterator{
                    Node* e;
                    Node* i;
                };

                ConverterFunction(const QMetaType& elementMetaType)
                    : QtPrivate::AbstractConverterFunction(convert),
                      _metaType_id(elementMetaType.id()),
                      _metaType_flags(AbstractContainerAccess::isPointerType(elementMetaType)),
                      _iteratorCapabilities(0),
                      _size([](const void *p)->int{
                                QLinkedListData*const* linkedList = reinterpret_cast<QLinkedListData*const*>(p);
                                QLinkedListData* d = *linkedList;
                                return d->size;
                           }),
                      _at([](const void *p, int index)->const void *{
                                QLinkedListData*const* linkedList = reinterpret_cast<QLinkedListData*const*>(p);
                                QLinkedListData* d = *linkedList;
                                Node* e = reinterpret_cast<Node*>(d);
                                Node *i = e->n;
                                while (i != e) {
                                    if(index==0)
                                        return &i->t;
                                    --index;
                                    i = i->n;
                                }
                                return nullptr;
                            }),
                      _moveTo([](const void *p, void ** iter, QtMetaTypePrivate::QSequentialIterableImpl::Position position){
                                    QLinkedListData*const* linkedList = reinterpret_cast<QLinkedListData*const*>(p);
                                    QLinkedListData* d = *linkedList;
                                    Node* e = reinterpret_cast<Node*>(d);
                                    *iter = new Iterator{e, position==QtMetaTypePrivate::QSequentialIterableImpl::ToBegin ? e->n : e};
                                }),
                      _append(nullptr),
                      _advance([](void **p, int steps){
                                    Iterator* iter = *reinterpret_cast<Iterator**>(p);
                                    while (iter->i != iter->e && steps!=0) {
                                        if(steps<0){
                                            iter->i = iter->i->p;
                                            ++steps;
                                        }else{
                                            iter->i = iter->i->n;
                                            --steps;
                                        }
                                    }
                                }),
                      _get([]( void * const *p, int metaTypeId, uint flags)->QtMetaTypePrivate::VariantData{
                                const Iterator* iter = *reinterpret_cast<Iterator*const*>(p);
                                return QtMetaTypePrivate::VariantData(metaTypeId, &iter->i->t, flags);
                            }),
                      _destroyIter([](void **p){delete *reinterpret_cast<Iterator**>(p);}),
                      _equalIter([](void * const *p, void * const *other)->bool{
                                        const Iterator* iter1 = *reinterpret_cast<Iterator*const*>(p);
                                        const Iterator* iter2 = *reinterpret_cast<Iterator*const*>(other);
                                        return iter1->e==iter2->e && iter1->i==iter2->i;
                                }),
                      _copyIter(nullptr)
                {
                    IteratorCapabilities* capabilities = reinterpret_cast<IteratorCapabilities*>(&_iteratorCapabilities);
                    capabilities->_revision = 1;
                    capabilities->_iteratorCapabilities = QtMetaTypePrivate::RandomAccessCapability;
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
            registerConverter(new ConverterFunction(m_elementMetaType), newMetaType, to);
        }
        if(QMetaType::hasRegisteredComparators(m_elementMetaType.id())
            || registeredComparator(m_elementMetaType.id())
            || m_elementMetaType.id()<QMetaType::User
            || AbstractContainerAccess::isPointerType(m_elementMetaType)){
            struct Comparator : QtPrivate::AbstractComparatorFunction{
                static bool lessThan(const AbstractComparatorFunction *_c, const void *a, const void *b){
                    const Comparator* c = static_cast<const Comparator*>(_c);
                    QLinkedListData*const* linkedListA = reinterpret_cast<QLinkedListData*const*>(a);
                    QLinkedListData* dA = *linkedListA;
                    QLinkedListData*const* linkedListB = reinterpret_cast<QLinkedListData*const*>(b);
                    QLinkedListData* dB = *linkedListB;
                    if(dA->size<dB->size)
                        return false;
                    if(dA->size>dB->size)
                        return true;
                    if(dA==dB)
                        return false;
                    Node* eA = reinterpret_cast<Node*>(dA);
                    Node* eB = reinterpret_cast<Node*>(dB);
                    Node *i = eA->n;
                    Node *il = eB->n;
                    while (i != eA) {
                        if(!isLessThan(c->_metaType_id, &i->t, &il->t))
                            return false;
                        i = i->n;
                        il = il->n;
                    }
                    return true;
                }
                static bool equals(const AbstractComparatorFunction *_c, const void *a, const void *b){
                    const Comparator* c = static_cast<const Comparator*>(_c);
                    QLinkedListData*const* linkedListA = reinterpret_cast<QLinkedListData*const*>(a);
                    QLinkedListData* dA = *linkedListA;
                    QLinkedListData*const* linkedListB = reinterpret_cast<QLinkedListData*const*>(b);
                    QLinkedListData* dB = *linkedListB;
                    if(dA==dB)
                        return true;
                    if(dA->size<dB->size)
                        return false;
                    Node* eA = reinterpret_cast<Node*>(dA);
                    Node* eB = reinterpret_cast<Node*>(dB);
                    Node *i = eA->n;
                    Node *il = eB->n;
                    while (i != eA) {
                        if(!isEquals(c->_metaType_id, &i->t, &il->t))
                            return false;
                        i = i->n;
                        il = il->n;
                    }
                    return true;
                }
                static void destroy(AbstractComparatorFunction *_c){
                    delete static_cast<Comparator*>(_c);
                }
                QMetaType _metaType_id;

                Comparator(const QMetaType& keyMetaType)
                    : QtPrivate::AbstractComparatorFunction(lessThan, equals, destroy),
                      _metaType_id(keyMetaType.id())
                {

                }
            };
            RegistryAPI::registerComparator(new Comparator(m_elementMetaType), newMetaType);
        }
        if(QMetaType::hasRegisteredDebugStreamOperator(m_elementMetaType.id())
                || registeredDebugStreamOperator(m_elementMetaType.id())
                || m_elementMetaType.id()<QMetaType::User
                || AbstractContainerAccess::isPointerType(m_elementMetaType)){
            struct DebugStreamFunction : QtPrivate::AbstractDebugStreamFunction{
                static void stream(const AbstractDebugStreamFunction *_f, QDebug& dbg, const void *ptr){
                    const DebugStreamFunction* f = static_cast<const DebugStreamFunction*>(_f);
                    QLinkedListData*const* linkedList = reinterpret_cast<QLinkedListData*const*>(ptr);
                    QLinkedListData* d = *linkedList;
                    Node* e = reinterpret_cast<Node*>(d);
                    Node *i = e->n;
                    dbg = dbg.nospace().noquote();
                    dbg << "QLinkedList(";
                    bool first = true;
                    while (i != e) {
                        if(first){
                            dbg << ", ";
                            first = false;
                        }
                        CoreAPI::appendToDebugStream(dbg, f->_metaType_id, &i->t);
                        i = i->n;
                    }
                    dbg << ")";
                }
                static void destroy(AbstractDebugStreamFunction *_f){
                    delete static_cast<DebugStreamFunction*>(_f);
                }
                int _metaType_id;

                DebugStreamFunction(const QMetaType& keyMetaType)
                    : QtPrivate::AbstractDebugStreamFunction(stream, destroy),
                      _metaType_id(keyMetaType.id())
                {

                }
            };
            RegistryAPI::registerDebugStreamOperator(new DebugStreamFunction(m_elementMetaType), newMetaType);
        }
        {
            int _metaType_id = m_elementMetaType.id();
            QMetaType::registerStreamOperators(newMetaType,
                                               qtjambi_function_pointer<16,void(QDataStream &, const void *)>(
                                                                               [_metaType_id](QDataStream &s, const void * ptr){
                                                                                   QLinkedListData*const* linkedList = reinterpret_cast<QLinkedListData*const*>(ptr);
                                                                                   QLinkedListData* d = *linkedList;
                                                                                   Node* e = reinterpret_cast<Node*>(d);
                                                                                   Node *i = e->n;
                                                                                   s << d->size;
                                                                                   while (i != e) {
                                                                                       QMetaType::save(s, _metaType_id, &i->t);
                                                                                       i = i->n;
                                                                                   }
                                                                               }, _metaType_id),
                                               qtjambi_function_pointer<16,void(QDataStream &, void *)>(
                                                                               [access](QDataStream &s, void * ptr){
                                                                                   const int sizeOf = QMetaType::sizeOf(access->m_elementMetaType.sizeOf()) + 2 * sizeof(void*);
                                                                                   QLinkedListData** linkedList = reinterpret_cast<QLinkedListData**>(ptr);
                                                                                   QLinkedListData*& d = *linkedList;
                                                                                   access->detach(d);
                                                                                   Node* e = reinterpret_cast<Node*>(d);
                                                                                   int size = 0;
                                                                                   s >> size;
                                                                                   for(int idx=0; idx<size; ++idx){
                                                                                       Node *i = reinterpret_cast<Node*>(operator new(sizeOf));
                                                                                       QMetaType::construct(access->m_elementMetaType.sizeOf(), &i->t, nullptr);
                                                                                       QMetaType::load(s, access->m_elementMetaType.sizeOf(), &i->t);
                                                                                       i->n = e;
                                                                                       i->p = e->p;
                                                                                       i->p->n = i;
                                                                                       e->p = i;
                                                                                   }
                                                                                   d->size+=size;
                                                                               }, _metaType_id)
                                               );
        }
    }else{
        registerContainerAccess(newMetaType, this);
    }
    return newMetaType;
}

const QMetaType& AutoLinkedListAccess::elementMetaType(){
    return m_elementMetaType;
}

AbstractContainerAccess::DataType AutoLinkedListAccess::elementType() {return m_elementDataType;}

AbstractContainerAccess* AutoLinkedListAccess::elementNestedContainerAccess() {
    return m_elementNestedContainerAccess ? m_elementNestedContainerAccess->clone() : nullptr;
}
const QSharedPointer<AbstractContainerAccess>& AutoLinkedListAccess::sharedElementNestedContainerAccess(){
    return m_elementNestedContainerAccess;
}
bool AutoLinkedListAccess::hasNestedContainerAccess(){
    return !m_elementNestedContainerAccess.isNull();
}
bool AutoLinkedListAccess::hasNestedPointers(){
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

const QObject* AutoLinkedListAccess::getOwner(const void* container){
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

bool AutoLinkedListAccess::hasOwnerFunction(){
    if(m_elementOwnerFunction && !(elementType() & PointersMask))
        return true;
    if(!(elementType() & PointersMask) && m_elementNestedContainerAccess && m_elementNestedContainerAccess->hasOwnerFunction())
        return true;
    return false;
}

void AutoLinkedListAccess::detach_helper(QLinkedListData*& d)
{
    detach_helper2(d, reinterpret_cast<Node*>(d));
}

void AutoLinkedListAccess::freeData(QLinkedListData*& x){
    Node *y = reinterpret_cast<Node*>(x);
    Node *i = y->n;
    Q_ASSERT(x->ref.atomic.loadRelaxed() == 0);
    while (i != y) {
        Node *n = i;
        i = i->n;
        m_elementMetaType.destruct(&n->t);
        operator delete(n);
    }
    delete x;
}

QLinkedList<char>::iterator AutoLinkedListAccess::detach_helper2(QLinkedListData*& d, QLinkedList<char>::iterator orgite)
{
    // detach and convert orgite to an iterator in the detached instance
    const int sizeOf = m_elementMetaType.sizeOf() + 2 * sizeof(void*);
    bool isEndIterator = (quintptr(orgite.i) == quintptr(d));
    union { QLinkedListData *d; Node *e; } x;
    x.d = new QLinkedListData;
    x.d->ref.initializeOwned();
    x.d->size = d->size;
    x.d->sharable = true;
    Node *original = reinterpret_cast<Node*>(d)->n;
    Node *copy = x.e;
    Node *org = orgite.i;

    while (original != org) {
        QT_TRY {
            copy->n = reinterpret_cast<Node*>(operator new(sizeOf));
            m_elementMetaType.construct(&copy->n->t, &original->t);
            copy->n->p = copy;
            original = original->n;
            copy = copy->n;
        } QT_CATCH(...) {
            copy->n = x.e;
            Q_ASSERT(!x.d->ref.deref()); // Don't trigger assert in free
            freeData(x.d);
            QT_RETHROW;
        }
    }
    QLinkedList<char>::iterator r(copy);
    while (quintptr(original) != quintptr(d)) {
        QT_TRY {
            copy->n = reinterpret_cast<Node*>(operator new(sizeOf));
            m_elementMetaType.construct(&copy->n->t, &original->t);
            copy->n->p = copy;
            original = original->n;
            copy = copy->n;
        } QT_CATCH(...) {
            copy->n = x.e;
            Q_ASSERT(!x.d->ref.deref()); // Don't trigger assert in free
            freeData(x.d);
            QT_RETHROW;
        }
    }
    copy->n = x.e;
    x.e->p = copy;
    if (!d->ref.deref())
        freeData(d);
    d = x.d;
    if (!isEndIterator)
        ++r; // since we stored the element right before the original node.
    return r;
}

void AutoLinkedListAccess::detach(QLinkedListData*& d){
    if (d->ref.isShared()) detach_helper2(d, reinterpret_cast<Node*>(d));
}

void AutoLinkedListAccess::append(JNIEnv * env, const ContainerInfo& container, jobject value)
{
    const int sizeOf = m_elementMetaType.sizeOf() + 2 * sizeof(void*);
    QLinkedListData** linkedList = reinterpret_cast<QLinkedListData**>(container.container);
    QLinkedListData*& d = *linkedList;
    detach(d);
    Node*& e = reinterpret_cast<Node*&>(d);
    Node *i = reinterpret_cast<Node*>(operator new(sizeOf));
    m_elementMetaType.construct(&i->t);
    jvalue _value;
    _value.l = value;
    void* ptr = &i->t;
    m_externalToInternalConverter(env, nullptr, _value, ptr, jValueType::l);
    i->n = e;
    i->p = e->p;
    i->p->n = i;
    e->p = i;
    d->size++;
}

jobject AutoLinkedListAccess::first(JNIEnv * env, const void* container)
{
    QLinkedListData*const* linkedList = reinterpret_cast<QLinkedListData*const*>(container);
    QLinkedListData* d = *linkedList;
    Q_ASSERT(d->size>0);
    jvalue _value;
    _value.l = nullptr;
    if(m_internalToExternalConverter(env, nullptr, &reinterpret_cast<const Node*>(d)->n->t, _value, true))
        return _value.l;
    return nullptr;
}

jobject AutoLinkedListAccess::last(JNIEnv * env, const void* container)
{
    QLinkedListData*const* linkedList = reinterpret_cast<QLinkedListData*const*>(container);
    QLinkedListData* d = *linkedList;
    Q_ASSERT(d->size>0);
    jvalue _value;
    _value.l = nullptr;
    if(m_internalToExternalConverter(env, nullptr, &reinterpret_cast<const Node*>(d)->p->t, _value, true))
        return _value.l;
    return nullptr;
}

jobject AutoLinkedListAccess::createIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr)
{
    AbstractSequentialIteratorAccess* containerAccess = new AutoSequentialIteratorAccess(m_internalToExternalConverter,
                                                 [](AutoSequentialIteratorAccess*,void*ptr){
                                                    Node* cursor = *reinterpret_cast<Node**>(ptr);
                                                    *reinterpret_cast<Node**>(ptr) = cursor->n;
                                                 },
                                                 [](AutoSequentialIteratorAccess*,void*ptr){
                                                    Node* cursor = *reinterpret_cast<Node**>(ptr);
                                                    *reinterpret_cast<Node**>(ptr) = cursor->p;
                                                 },
                                                 [](AutoSequentialIteratorAccess*,const void*ptr)->const void*{
                                                    return &(*reinterpret_cast<Node*const*>(ptr))->t;
                                                 },
                                                 {},
                                                 [](AutoSequentialIteratorAccess*,const void*ptr1,const void*ptr2)->bool{
                                                    return *reinterpret_cast<Node*const*>(ptr1)==*reinterpret_cast<Node*const*>(ptr2);
                                                 },
                                                 m_externalToInternalConverter,
                                                 [](AutoSequentialIteratorAccess*,void*ptr)->void*{
                                                    return &(*reinterpret_cast<Node**>(ptr))->t;
                                                 },
                                                 m_elementMetaType,
                                                 /*offset not required*/ 0
                                            );
    return QtJambiAPI::convertQSequentialIteratorToJavaObject(env, ownerId, iteratorPtr, [](void* ptr,bool){
        delete reinterpret_cast<void**>(ptr);
    }, containerAccess);
}

jobject AutoLinkedListAccess::createConstIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr)
{
    AutoSequentialConstIteratorAccess* containerAccess = new AutoSequentialConstIteratorAccess(m_internalToExternalConverter,
                                                 [](AutoSequentialConstIteratorAccess*,void*ptr){
                                                    Node* cursor = *reinterpret_cast<Node**>(ptr);
                                                    *reinterpret_cast<Node**>(ptr) = cursor->n;
                                                 },
                                                 [](AutoSequentialConstIteratorAccess*,void*ptr){
                                                    Node* cursor = *reinterpret_cast<Node**>(ptr);
                                                    *reinterpret_cast<Node**>(ptr) = cursor->p;
                                                 },
                                                 [](AutoSequentialConstIteratorAccess*,const void*ptr)->const void*{
                                                    return &(*reinterpret_cast<Node*const*>(ptr))->t;
                                                 },
                                                 {},
                                                 [](AutoSequentialConstIteratorAccess*,const void*ptr1,const void*ptr2)->bool{
                                                    return *reinterpret_cast<Node*const*>(ptr1)==*reinterpret_cast<Node*const*>(ptr2);
                                                 },
                                                 m_elementMetaType,
                                                 /*offset not required*/ 0
                                            );
    return QtJambiAPI::convertQSequentialIteratorToJavaObject(env, ownerId, iteratorPtr, [](void* ptr,bool){
        delete reinterpret_cast<void**>(ptr);
    }, containerAccess);
}

bool AutoLinkedListAccess::isDetached(const void* container){
    QLinkedListData *const* vector = reinterpret_cast<QLinkedListData *const*>(container);
    return vector && *vector && !(*vector)->ref.isShared();
}

void AutoLinkedListAccess::detach(const ContainerInfo& container){
    QLinkedListData ** vector = reinterpret_cast<QLinkedListData **>(container.container);
    detach(*vector);
}

bool AutoLinkedListAccess::isSharedWith(const void* container, const void* container2){
    QLinkedListData *const* vector = reinterpret_cast<QLinkedListData *const*>(container);
    QLinkedListData *const* vector2 = reinterpret_cast<QLinkedListData *const*>(container2);
    if(vector && vector2){
        return (*vector)==(*vector2);
    }
    return false;
}

void AutoLinkedListAccess::swap(JNIEnv *, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    QLinkedListData *& vector = *reinterpret_cast<QLinkedListData **>(container.container);
    QLinkedListData *& vector2 = *reinterpret_cast<QLinkedListData **>(container2.container);
    qSwap(vector, vector2);
}

jobject AutoLinkedListAccess::begin(JNIEnv * env, const ExtendedContainerInfo& container)
{
    QLinkedListData** linkedList = reinterpret_cast<QLinkedListData**>(container.container);
    detach(*linkedList);
    QLinkedListData* d = *linkedList;
    return createIterator(env, container.nativeId, new void*(reinterpret_cast<const Node*>(d)->n));
}

jobject AutoLinkedListAccess::end(JNIEnv * env, const ExtendedContainerInfo& container)
{
    QLinkedListData** linkedList = reinterpret_cast<QLinkedListData**>(container.container);
    detach(*linkedList);
    QLinkedListData* d = *linkedList;
    return createIterator(env, container.nativeId, new void*(d));
}

jobject AutoLinkedListAccess::constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container)
{
    QLinkedListData*const* linkedList = reinterpret_cast<QLinkedListData*const*>(container.container);
    QLinkedListData* d = *linkedList;
    return createConstIterator(env, container.nativeId, new void*(reinterpret_cast<const Node*>(d)->n));
}

jobject AutoLinkedListAccess::constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container)
{
    QLinkedListData*const* linkedList = reinterpret_cast<QLinkedListData*const*>(container.container);
    QLinkedListData* d = *linkedList;
    return createConstIterator(env, container.nativeId, new void*(d));
}

void AutoLinkedListAccess::clear(JNIEnv *, const ContainerInfo& container)
{
    QLinkedListData** linkedList = reinterpret_cast<QLinkedListData**>(container.container);
    QLinkedListData*& d = *linkedList;
    if(d!=&QLinkedListData::shared_null){
        if (!d->ref.deref())
            freeData(d);
        *linkedList = const_cast<QLinkedListData *>(&QLinkedListData::shared_null);
    }
}

jboolean AutoLinkedListAccess::contains(JNIEnv * env, const void* container, jobject value)
{
    QtJambiScope scope;
    jvalue _value;
    _value.l = value;
    void* ptr = nullptr;
    if(m_externalToInternalConverter(env, &scope, _value, ptr, jValueType::l)){
        QLinkedListData*const* linkedList = reinterpret_cast<QLinkedListData*const*>(container);
        QLinkedListData* d = *linkedList;
        Node* e = reinterpret_cast<Node*>(d);
        Node *i = e;
        while ((i = i->n) != e){
            if(isEquals(m_elementMetaType, &i->t, ptr))
                return true;
        }
    }
    return false;
}

jint AutoLinkedListAccess::count(JNIEnv * env, const void* container, jobject value)
{
    jint counter=0;
    QtJambiScope scope;
    jvalue _value;
    _value.l = value;
    void* ptr = nullptr;
    if(m_externalToInternalConverter(env, &scope, _value, ptr, jValueType::l)){
        QLinkedListData*const* linkedList = reinterpret_cast<QLinkedListData*const*>(container);
        QLinkedListData* d = *linkedList;
        Node* e = reinterpret_cast<Node*>(d);
        Node *i = e;
        while ((i = i->n) != e){
            if(isEquals(m_elementMetaType, &i->t, ptr))
                ++counter;
        }
    }
    return counter;
}

jboolean AutoLinkedListAccess::endsWith(JNIEnv * env, const void* container, jobject value)
{
    QtJambiScope scope;
    jvalue _value;
    _value.l = value;
    void* ptr = nullptr;
    if(m_externalToInternalConverter(env, &scope, _value, ptr, jValueType::l)){
        QLinkedListData*const* linkedList = reinterpret_cast<QLinkedListData*const*>(container);
        QLinkedListData* d = *linkedList;
        Node* e = reinterpret_cast<Node*>(d);
        if(isEquals(m_elementMetaType, &e->p->t, ptr))
            return true;
    }
    return false;
}

jboolean AutoLinkedListAccess::startsWith(JNIEnv * env, const void* container, jobject value)
{
    QtJambiScope scope;
    jvalue _value;
    _value.l = value;
    void* ptr = nullptr;
    if(m_externalToInternalConverter(env, &scope, _value, ptr, jValueType::l)){
        QLinkedListData*const* linkedList = reinterpret_cast<QLinkedListData*const*>(container);
        QLinkedListData* d = *linkedList;
        Node* e = reinterpret_cast<Node*>(d);
        if(isEquals(m_elementMetaType, &e->n->t, ptr))
            return true;
    }
    return false;
}

jboolean AutoLinkedListAccess::equal(JNIEnv * env, const void* container, jobject other)
{
    QLinkedListData*const* linkedList = reinterpret_cast<QLinkedListData*const*>(container);
    QLinkedListData* d = *linkedList;
    Node* e = reinterpret_cast<Node*>(d);
    void* ptr{nullptr};
    if (ContainerAPI::getAsQLinkedList(env, other, elementMetaType(), ptr)) {
        QLinkedListData*const* linkedList2 = reinterpret_cast<QLinkedListData*const*>(ptr);
        QLinkedListData* d2 = *linkedList2;
        Node* e2 = reinterpret_cast<Node*>(d2);
        if (d->size != d2->size)
            return false;
        if (e == e2)
            return true;
        Node *i = e->n;
        Node *il = e2->n;
        while (i != e) {
            if (! (isEquals(m_elementMetaType, &i->t, &il->t)))
                return false;
            i = i->n;
            il = il->n;
        }
        return true;
    }else{
        if(d->size!=QtJambiAPI::sizeOfJavaCollection(env, other))
            return false;
        jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, other);
        Node *i = e->n;
        while(QtJambiAPI::hasJavaIteratorNext(env, iterator)){
            QtJambiScope scope;
            jvalue _value;
            _value.l = QtJambiAPI::nextOfJavaIterator(env, iterator);
            void *_qvaluePtr = nullptr;
            if(!m_externalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l))
                return false;
            if(!isEquals(m_elementMetaType, &i->t, _qvaluePtr))
                return false;
            i = i->n;
        }
        return true;
    }
    return false;
}

void AutoLinkedListAccess::prepend(JNIEnv * env, const ContainerInfo& container, jobject value)
{
    const int sizeOf = m_elementMetaType.sizeOf() + 2 * sizeof(void*);
    QLinkedListData** linkedList = reinterpret_cast<QLinkedListData**>(container.container);
    QLinkedListData*& d = *linkedList;
    Node* e = reinterpret_cast<Node*>(d);
    detach(d);
    Node *i = reinterpret_cast<Node*>(operator new(sizeOf));
    m_elementMetaType.construct(&i->t);
    jvalue _value;
    _value.l = value;
    void* ptr = &i->t;
    m_externalToInternalConverter(env, nullptr, _value, ptr, jValueType::l);
    i->n = e->n;
    i->p = e;
    i->n->p = i;
    e->n = i;
    d->size++;

}

void AutoLinkedListAccess::erase(QLinkedListData*& d, Node *i){
    Node* e = reinterpret_cast<Node*>(d);
    if (i != e) {
        Node *n = i;
        i->n->p = i->p;
        i->p->n = i->n;
        i = i->n;
        m_elementMetaType.destruct(&n->t);
        operator delete(n);
        d->size--;
    }
}

void AutoLinkedListAccess::removeFirst(JNIEnv *, const ContainerInfo& container)
{
    QLinkedListData** linkedList = reinterpret_cast<QLinkedListData**>(container.container);
    QLinkedListData*& d = *linkedList;
    Node* e = reinterpret_cast<Node*>(d);
    detach(d);
    erase(d, e->n);
}

void AutoLinkedListAccess::removeLast(JNIEnv *, const ContainerInfo& container)
{
    QLinkedListData** linkedList = reinterpret_cast<QLinkedListData**>(container.container);
    QLinkedListData*& d = *linkedList;
    Node* e = reinterpret_cast<Node*>(d);
    detach(d);
    erase(d, e->p);
}

jint AutoLinkedListAccess::removeAll(JNIEnv * env, const ContainerInfo& container, jobject value)
{
    jint counter = 0;
    QtJambiScope scope;
    jvalue _value;
    _value.l = value;
    void* ptr = nullptr;
    if(m_externalToInternalConverter(env, &scope, _value, ptr, jValueType::l)){
        QLinkedListData** linkedList = reinterpret_cast<QLinkedListData**>(container.container);
        QLinkedListData*& d = *linkedList;
        Node* e = reinterpret_cast<Node*>(d);
        detach(d);
        Node *i = e->n;
        while (i != e) {
            if (isEquals(m_elementMetaType, &i->t, ptr)) {
                Node *n = i;
                i->n->p = i->p;
                i->p->n = i->n;
                i = i->n;
                m_elementMetaType.destruct(&n->t);
                operator delete(n);
                counter++;
            } else {
                i = i->n;
            }
        }
        d->size-=counter;
    }
    return counter;
}

jboolean AutoLinkedListAccess::removeOne(JNIEnv * env, const ContainerInfo& container, jobject value)
{
    QtJambiScope scope;
    jvalue _value;
    _value.l = value;
    void* ptr = nullptr;
    if(m_externalToInternalConverter(env, &scope, _value, ptr, jValueType::l)){
        QLinkedListData** linkedList = reinterpret_cast<QLinkedListData**>(container.container);
        QLinkedListData*& d = *linkedList;
        Node* e = reinterpret_cast<Node*>(d);
        detach(d);
        Node *i = e;
        while ((i = i->n) != e){
            if(isEquals(m_elementMetaType, &i->t, ptr)){
                erase(d, i);
                return true;
            }
        }
    }
    return false;
}

jint AutoLinkedListAccess::size(JNIEnv *, const void* container){
    QLinkedListData*const* linkedList = reinterpret_cast<QLinkedListData*const*>(container);
    QLinkedListData* d = *linkedList;
    return d->size;
}

qsizetype AutoLinkedListAccess::size(const void* container){
    QLinkedListData*const* linkedList = reinterpret_cast<QLinkedListData*const*>(container);
    QLinkedListData* d = *linkedList;
    return d->size;
}

jobject AutoLinkedListAccess::takeFirst(JNIEnv * env, const ContainerInfo& container)
{
    jobject result = first(env, container.container);
    removeFirst(env, container);
    return result;
}

jobject AutoLinkedListAccess::takeLast(JNIEnv * env, const ContainerInfo& container)
{
    jobject result = last(env, container.container);
    removeLast(env, container);
    return result;
}

PointerRCAutoLinkedListAccess::PointerRCAutoLinkedListAccess(PointerRCAutoLinkedListAccess& other)
    : AutoLinkedListAccess(other), ReferenceCountingSetContainer() {}

PointerRCAutoLinkedListAccess* PointerRCAutoLinkedListAccess::clone(){
    return new PointerRCAutoLinkedListAccess(*this);
}

void PointerRCAutoLinkedListAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    AutoLinkedListAccess::swap(env, container, container2);
    if(PointerRCAutoLinkedListAccess* access = dynamic_cast<PointerRCAutoLinkedListAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void PointerRCAutoLinkedListAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    AutoLinkedListAccess::assign(env, container, container2);
    if(PointerRCAutoLinkedListAccess* access = dynamic_cast<PointerRCAutoLinkedListAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void PointerRCAutoLinkedListAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
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

void PointerRCAutoLinkedListAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    AutoLinkedListAccess::clear(env, container);
    clearRC(env, container.object);
}

void PointerRCAutoLinkedListAccess::append(JNIEnv * env, const ContainerInfo& container, jobject value) {
    AutoLinkedListAccess::append(env, container, value);
    if(value)
        addRC(env, container.object, value);
}

void PointerRCAutoLinkedListAccess::prepend(JNIEnv * env, const ContainerInfo& container, jobject value) {
    AutoLinkedListAccess::prepend(env, container, value);
    if(value)
        addRC(env, container.object, value);
}

jboolean PointerRCAutoLinkedListAccess::removeOne(JNIEnv * env, const ContainerInfo& container, jobject value) {
    jboolean result = AutoLinkedListAccess::removeOne(env, container, value);
    if(result && !AutoLinkedListAccess::contains(env, container.container, value)){
        removeRC(env, container.object, value);
    }
    return result;
}

void PointerRCAutoLinkedListAccess::removeFirst(JNIEnv * env, const ContainerInfo& container) {
    jobject oldValue = AutoLinkedListAccess::first(env, container.container);
    AutoLinkedListAccess::removeFirst(env, container);
    if(!AutoLinkedListAccess::contains(env, container.container, oldValue))
        removeRC(env, container.object, oldValue);
}

void PointerRCAutoLinkedListAccess::removeLast(JNIEnv * env, const ContainerInfo& container) {
    jobject oldValue = AutoLinkedListAccess::last(env, container.container);
    AutoLinkedListAccess::removeLast(env, container);
    if(!AutoLinkedListAccess::contains(env, container.container, oldValue))
        removeRC(env, container.object, oldValue);
}

jint PointerRCAutoLinkedListAccess::removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) {
    jint result = AutoLinkedListAccess::removeAll(env, container, value);
    removeRC(env, container.object, value, result);
    return result;
}

jobject PointerRCAutoLinkedListAccess::takeFirst(JNIEnv * env, const ContainerInfo& container) {
    jobject result = AutoLinkedListAccess::takeFirst(env, container);
    removeRC(env, container.object, result);
    return result;
}

jobject PointerRCAutoLinkedListAccess::takeLast(JNIEnv * env, const ContainerInfo& container) {
    jobject result = AutoLinkedListAccess::takeLast(env, container);
    removeRC(env, container.object, result);
    return result;
}

NestedPointersRCAutoLinkedListAccess::NestedPointersRCAutoLinkedListAccess(NestedPointersRCAutoLinkedListAccess& other)
    : AutoLinkedListAccess(other), ReferenceCountingSetContainer() {}

NestedPointersRCAutoLinkedListAccess* NestedPointersRCAutoLinkedListAccess::clone(){
    return new NestedPointersRCAutoLinkedListAccess(*this);
}

void NestedPointersRCAutoLinkedListAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
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

void NestedPointersRCAutoLinkedListAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    AutoLinkedListAccess::swap(env, container, container2);
    if(NestedPointersRCAutoLinkedListAccess* access = dynamic_cast<NestedPointersRCAutoLinkedListAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void NestedPointersRCAutoLinkedListAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    AutoLinkedListAccess::assign(env, container, container2);
    if(NestedPointersRCAutoLinkedListAccess* access = dynamic_cast<NestedPointersRCAutoLinkedListAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void NestedPointersRCAutoLinkedListAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    AutoLinkedListAccess::clear(env, container);
    clearRC(env, container.object);
}

void NestedPointersRCAutoLinkedListAccess::append(JNIEnv * env, const ContainerInfo& container, jobject value) {
    AutoLinkedListAccess::append(env, container, value);
    addNestedValueRC(env, container.object, elementType(), hasNestedPointers(), value);
}

void NestedPointersRCAutoLinkedListAccess::prepend(JNIEnv * env, const ContainerInfo& container, jobject value) {
    AutoLinkedListAccess::prepend(env, container, value);
    addNestedValueRC(env, container.object, elementType(), hasNestedPointers(), value);
}

jboolean NestedPointersRCAutoLinkedListAccess::removeOne(JNIEnv * env, const ContainerInfo& container, jobject value) {
    jboolean result = AutoLinkedListAccess::removeOne(env, container, value);
    if(result){
        updateRC(env, container);
    }
    return result;
}

void NestedPointersRCAutoLinkedListAccess::removeFirst(JNIEnv * env, const ContainerInfo& container) {
    AutoLinkedListAccess::removeFirst(env, container);
    updateRC(env, container);
}

void NestedPointersRCAutoLinkedListAccess::removeLast(JNIEnv * env, const ContainerInfo& container) {
    AutoLinkedListAccess::removeLast(env, container);
    updateRC(env, container);
}

jint NestedPointersRCAutoLinkedListAccess::removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) {
    jint result = AutoLinkedListAccess::removeAll(env, container, value);
    if(result>0){
        updateRC(env, container);
    }
    return result;
}

jobject NestedPointersRCAutoLinkedListAccess::takeFirst(JNIEnv * env, const ContainerInfo& container) {
    jobject result = AutoLinkedListAccess::takeFirst(env, container);
    updateRC(env, container);
    return result;
}

jobject NestedPointersRCAutoLinkedListAccess::takeLast(JNIEnv * env, const ContainerInfo& container) {
    jobject result = AutoLinkedListAccess::takeLast(env, container);
    updateRC(env, container);
    return result;
}

#endif
