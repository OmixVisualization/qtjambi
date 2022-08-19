#include <QtCore/QtGlobal>

QT_WARNING_DISABLE_DEPRECATED

#include "qtjambi_core.h"
#include <QtCore/QReadWriteLock>
#include <QtCore/QMap>
#include <QtCore/QSharedPointer>
#include <QtCore/QLinkedList>
#include "qtjambi_containeraccess_p.h"
#include "qtjambi_functionpointer.h"
#include "qtjambi_registry_p.h"

QDebug& debug_stream(QDebug& debug, uint typeId, const void* ptr);
const QtPrivate::AbstractComparatorFunction * qtjambi_registered_comparator(int typeId);
const QtPrivate::AbstractDebugStreamFunction * qtjambi_registered_debugstream_operator(int typeId);
bool is_equals(const QMetaType& keyMetaType, const void * ptr, const void* ptr2);
bool is_less(const QMetaType& keyMetaType, const void * ptr, const void* ptr2);

AutoLinkedListAccess::AutoLinkedListAccess(
        int elementMetaType,
        size_t elementAlign,
        const QHashFunction& hashFunction,
        const InternalToExternalConverter& internalToExternalConverter,
        const ExternalToInternalConverter& externalToInternalConverter
        )
    : AbstractLinkedListAccess(),
      m_elementMetaType(elementMetaType),
      m_hashFunction(hashFunction),
      m_internalToExternalConverter(internalToExternalConverter),
      m_externalToInternalConverter(externalToInternalConverter),
      m_offset(0)
{
    Q_ASSERT(m_elementMetaType.id()!=QMetaType::UnknownType
            && m_elementMetaType.id()!=QMetaType::Void);
    m_offset = 2*sizeof(void*);
    if(m_offset%elementAlign>0)
        m_offset += elementAlign-m_offset%elementAlign;
}

AutoLinkedListAccess::AutoLinkedListAccess(const AutoLinkedListAccess& other)
  : AbstractLinkedListAccess(),
    m_elementMetaType(other.m_elementMetaType.id()),
    m_hashFunction(other.m_hashFunction),
    m_internalToExternalConverter(other.m_internalToExternalConverter),
    m_externalToInternalConverter(other.m_externalToInternalConverter),
    m_offset(other.m_offset)
{
}

AutoLinkedListAccess* AutoLinkedListAccess::clone(){
    return new AutoLinkedListAccess(*this);
}

void AutoLinkedListAccess::analyzeElements(const void* container, ElementAnalyzer analyzer, void* data){
    QLinkedListData*const* linkedList = reinterpret_cast<QLinkedListData*const*>(container);
    QLinkedListData *d = *linkedList;
    Node* e = reinterpret_cast<Node*>(d);
    Node *i = e->n;
    while (i != e) {
        if(!analyzer(&i->t, data))
            break;
        i = i->n;
    }
}

void AutoLinkedListAccess::dispose()
{
    delete this;
}

size_t AutoLinkedListAccess::sizeOf(){
    return sizeof(QLinkedList<char>);
}

void* AutoLinkedListAccess::constructContainer(void* placement, const void* container)
{
    placement = new(placement) QLinkedList<char>();
    if(container)
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
        newMetaType = registerMetaType(typeName,
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
                                       uint(sizeOf()),
                                       uint(alignof(QLinkedList<char>)),
                                       QMetaType::NeedsConstruction
                                                   | QMetaType::NeedsDestruction
                                                   | QMetaType::MovableType,
                                       nullptr,
                                       nullptr);
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
                      _metaType_flags(qtjambi_is_pointer_type(elementMetaType)),
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
                    IteratorCapabilities& capabilities = reinterpret_cast<IteratorCapabilities&>(_iteratorCapabilities);
                    capabilities._revision = 1;
                    capabilities._iteratorCapabilities = QtMetaTypePrivate::RandomAccessCapability;
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
            qtjambi_register_converter(new ConverterFunction(m_elementMetaType), newMetaType, to);
        }
        if(QMetaType::hasRegisteredComparators(m_elementMetaType.id())
            || qtjambi_registered_comparator(m_elementMetaType.id())
            || m_elementMetaType.id()<QMetaType::User
            || qtjambi_is_pointer_type(m_elementMetaType)){
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
                        if(!is_less(c->_metaType_id, &i->t, &il->t))
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
                        if(!is_equals(c->_metaType_id, &i->t, &il->t))
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
            qtjambi_register_comparator(new Comparator(m_elementMetaType), newMetaType);
        }
        if(QMetaType::hasRegisteredDebugStreamOperator(m_elementMetaType.id())
                || qtjambi_registered_debugstream_operator(m_elementMetaType.id())
                || m_elementMetaType.id()<QMetaType::User
                || qtjambi_is_pointer_type(m_elementMetaType)){
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
                        debug_stream(dbg, f->_metaType_id, &i->t);
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
            qtjambi_register_debugstream_operator(new DebugStreamFunction(m_elementMetaType), newMetaType);
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
    }
    return newMetaType;
}

const QMetaType& AutoLinkedListAccess::elementMetaType(){
    return m_elementMetaType;
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

void AutoLinkedListAccess::append(JNIEnv * env, void* container, jobject value)
{
    const int sizeOf = m_elementMetaType.sizeOf() + 2 * sizeof(void*);
    QLinkedListData** linkedList = reinterpret_cast<QLinkedListData**>(container);
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
    if(m_internalToExternalConverter(env, nullptr, &reinterpret_cast<const Node*>(d)->n->t, &_value, true))
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
    if(m_internalToExternalConverter(env, nullptr, &reinterpret_cast<const Node*>(d)->p->t, &_value, true))
        return _value.l;
    return nullptr;
}

jobject AutoLinkedListAccess::createIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr)
{
    AutoIteratorAccess* containerAccess = new AutoIteratorAccess(m_internalToExternalConverter,
                                                 [](void*ptr){
                                                    Node* cursor = *reinterpret_cast<Node**>(ptr);
                                                    *reinterpret_cast<Node**>(ptr) = cursor->n;
                                                 },
                                                 [](void*ptr){
                                                    Node* cursor = *reinterpret_cast<Node**>(ptr);
                                                    *reinterpret_cast<Node**>(ptr) = cursor->p;
                                                 },
                                                 std::function<const void*(void*)>([](void*ptr)->const void*{
                                                    return &(*reinterpret_cast<Node**>(ptr))->t;
                                                 }),
                                                 {},
                                                 [](void*ptr1,void*ptr2)->bool{
                                                    return *reinterpret_cast<Node**>(ptr1)==*reinterpret_cast<Node**>(ptr2);
                                                 }
                                            );
    return qtjambi_from_QIterator(env, ownerId, iteratorPtr, [](void* ptr,bool){
        delete reinterpret_cast<void**>(ptr);
    }, containerAccess);
}

jobject AutoLinkedListAccess::begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container)
{
    QLinkedListData*const* linkedList = reinterpret_cast<QLinkedListData*const*>(container);
    QLinkedListData* d = *linkedList;
    return createIterator(env, ownerId, new void*(reinterpret_cast<const Node*>(d)->n));
}

jobject AutoLinkedListAccess::end(JNIEnv * env, QtJambiNativeID ownerId, const void* container)
{
    QLinkedListData*const* linkedList = reinterpret_cast<QLinkedListData*const*>(container);
    QLinkedListData* d = *linkedList;
    return createIterator(env, ownerId, new void*(d));
}

void AutoLinkedListAccess::clear(JNIEnv *, void* container)
{
    QLinkedListData** linkedList = reinterpret_cast<QLinkedListData**>(container);
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
            if(is_equals(m_elementMetaType, &i->t, ptr))
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
            if(is_equals(m_elementMetaType, &i->t, ptr))
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
        if(is_equals(m_elementMetaType, &e->p->t, ptr))
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
        if(is_equals(m_elementMetaType, &e->n->t, ptr))
            return true;
    }
    return false;
}

jboolean AutoLinkedListAccess::equal(JNIEnv * env, const void* container, jobject other)
{
    QLinkedListData*const* linkedList = reinterpret_cast<QLinkedListData*const*>(container);
    QLinkedListData* d = *linkedList;
    Node* e = reinterpret_cast<Node*>(d);
    if (qtjambi_is_QLinkedList(env, other, elementMetaType())) {
        if(void* ptr = qtjambi_to_object(env, other)){
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
                if (! (is_equals(m_elementMetaType, &i->t, &il->t)))
                    return false;
                i = i->n;
                il = il->n;
            }
            return true;
        }
    }else{
        if(d->size!=qtjambi_collection_size(env, other))
            return false;
        jobject iterator = qtjambi_collection_iterator(env, other);
        Node *i = e->n;
        while(qtjambi_iterator_has_next(env, iterator)){
            QtJambiScope scope;
            jvalue _value;
            _value.l = qtjambi_iterator_next(env, iterator);
            void *_qvaluePtr = nullptr;
            if(!m_externalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l))
                return false;
            if(!is_equals(m_elementMetaType, &i->t, _qvaluePtr))
                return false;
            i = i->n;
        }
        return true;
    }
    return false;
}

void AutoLinkedListAccess::prepend(JNIEnv * env, void* container, jobject value)
{
    const int sizeOf = m_elementMetaType.sizeOf() + 2 * sizeof(void*);
    QLinkedListData** linkedList = reinterpret_cast<QLinkedListData**>(container);
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

void AutoLinkedListAccess::removeFirst(JNIEnv *, void* container)
{
    QLinkedListData** linkedList = reinterpret_cast<QLinkedListData**>(container);
    QLinkedListData*& d = *linkedList;
    Node* e = reinterpret_cast<Node*>(d);
    detach(d);
    erase(d, e->n);
}

void AutoLinkedListAccess::removeLast(JNIEnv *, void* container)
{
    QLinkedListData** linkedList = reinterpret_cast<QLinkedListData**>(container);
    QLinkedListData*& d = *linkedList;
    Node* e = reinterpret_cast<Node*>(d);
    detach(d);
    erase(d, e->p);
}

jint AutoLinkedListAccess::removeAll(JNIEnv * env, void* container, jobject value)
{
    jint counter = 0;
    QtJambiScope scope;
    jvalue _value;
    _value.l = value;
    void* ptr = nullptr;
    if(m_externalToInternalConverter(env, &scope, _value, ptr, jValueType::l)){
        QLinkedListData** linkedList = reinterpret_cast<QLinkedListData**>(container);
        QLinkedListData*& d = *linkedList;
        Node* e = reinterpret_cast<Node*>(d);
        detach(d);
        Node *i = e->n;
        while (i != e) {
            if (is_equals(m_elementMetaType, &i->t, ptr)) {
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

jboolean AutoLinkedListAccess::removeOne(JNIEnv * env, void* container, jobject value)
{
    QtJambiScope scope;
    jvalue _value;
    _value.l = value;
    void* ptr = nullptr;
    if(m_externalToInternalConverter(env, &scope, _value, ptr, jValueType::l)){
        QLinkedListData** linkedList = reinterpret_cast<QLinkedListData**>(container);
        QLinkedListData*& d = *linkedList;
        Node* e = reinterpret_cast<Node*>(d);
        detach(d);
        Node *i = e;
        while ((i = i->n) != e){
            if(is_equals(m_elementMetaType, &i->t, ptr)){
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

jobject AutoLinkedListAccess::takeFirst(JNIEnv * env, void* container)
{
    jobject result = first(env, container);
    removeFirst(env, container);
    return result;
}

jobject AutoLinkedListAccess::takeLast(JNIEnv * env, void* container)
{
    jobject result = last(env, container);
    removeLast(env, container);
    return result;
}
