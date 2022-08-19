#include "qtjambi_core.h"
#include <QtCore/QReadWriteLock>
#include <QtCore/QMap>
#include <QtCore/QSharedPointer>
#include "qtjambi_containeraccess_p.h"
#include "qtjambi_functionpointer.h"
#include "qtjambi_registry_p.h"

QDebug& debug_stream(QDebug& debug, uint typeId, const void* ptr);
const QtPrivate::AbstractComparatorFunction * qtjambi_registered_comparator(int typeId);
const QtPrivate::AbstractDebugStreamFunction * qtjambi_registered_debugstream_operator(int typeId);
bool is_equals(const QMetaType& keyMetaType, const void * ptr, const void* ptr2);
bool is_less(const QMetaType& keyMetaType, const void * ptr, const void* ptr2);

AutoVectorAccess::AutoVectorAccess(
        int elementMetaType,
        size_t elementAlign,
        const QHashFunction& hashFunction,
        const InternalToExternalConverter& internalToExternalConverter,
        const ExternalToInternalConverter& externalToInternalConverter
        )
    : AbstractVectorAccess(),
      m_elementMetaType(elementMetaType),
      m_elementAlign(elementAlign),
      m_hashFunction(hashFunction),
      m_internalToExternalConverter(internalToExternalConverter),
      m_externalToInternalConverter(externalToInternalConverter),
      m_offset(0),
      m_isComplex(!qtjambi_is_pointer_type(m_elementMetaType)
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
                  && !(m_elementMetaType.flags() & QMetaType::IsEnumeration))
{
    Q_ASSERT(m_elementMetaType.id()!=QMetaType::UnknownType
            && m_elementMetaType.id()!=QMetaType::Void);
    m_offset = m_elementMetaType.sizeOf();
    if(m_offset%elementAlign>0)
        m_offset += elementAlign-m_offset%elementAlign;
}

AutoVectorAccess::AutoVectorAccess(const AutoVectorAccess& other)
  : AbstractVectorAccess(),
    m_elementMetaType(other.m_elementMetaType.id()),
    m_elementAlign(other.m_elementAlign),
    m_hashFunction(other.m_hashFunction),
    m_internalToExternalConverter(other.m_internalToExternalConverter),
    m_externalToInternalConverter(other.m_externalToInternalConverter),
    m_offset(other.m_offset),
    m_isComplex(other.m_isComplex)
{
}

AutoVectorAccess::~AutoVectorAccess(){}

const QMetaType& AutoVectorAccess::elementMetaType()
{
    return m_elementMetaType;
}

void AutoVectorAccess::dispose(){
    delete this;
}

void AutoVectorAccess::analyzeElements(const void* container, ElementAnalyzer analyzer, void* data){
    QTypedArrayData<char> *const* vector = reinterpret_cast<QTypedArrayData<char> *const*>(container);
    QTypedArrayData<char>* d = *vector;
    for(int i=0; i<d->size; ++i){
        if(!analyzer(d->data() + i * m_offset, data))
            break;
    }
}

AutoVectorAccess* AutoVectorAccess::clone(){
    return new AutoVectorAccess(*this);
}

size_t AutoVectorAccess::sizeOf(){
    return sizeof(QVector<char>);
}

void* AutoVectorAccess::constructContainer(void* placement, const void* container){
    void* result = new(placement) QArrayData*(QArrayData::sharedNull());
    if(container)
        assign(result, container);
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

void AutoVectorAccess::assign(void* container, const void* other)
{
    QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container);
    QTypedArrayData<char> *const* vector2 = reinterpret_cast<QTypedArrayData<char> *const*>(other);
    QTypedArrayData<char>*& d = *vector;
    QTypedArrayData<char>* d2 = *vector2;
    if (d == QArrayData::sharedNull()) {
        *vector = d2;
        d2->ref.ref();
    } else {
        for(int i = 0; i<d->size; ++i){
            m_elementMetaType.destruct(d->data() + i * m_offset);
        }
        uint newSize = d2->size;
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
        }else{
            d->size = 0;
        }
    }
}

int AutoVectorAccess::registerContainer(const QByteArray& typeName)
{
    int newMetaType = QMetaType::type(typeName);
    if(newMetaType==QMetaType::UnknownType){
        QSharedPointer<AutoVectorAccess> access(new AutoVectorAccess(*this), &containerDisposer);
        newMetaType = registerMetaType(typeName,
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
                                       uint(sizeOf()),
                                       uint(alignof(QVector<char>)),
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
                      _metaType_flags(qtjambi_is_pointer_type(elementMetaType)),
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
            qtjambi_register_converter(new ConverterFunction(m_elementMetaType, m_offset), newMetaType, to);
        }
        if(QMetaType::hasRegisteredComparators(m_elementMetaType.id())
            || qtjambi_registered_comparator(m_elementMetaType.id())
            || m_elementMetaType.id()<QMetaType::User
            || qtjambi_is_pointer_type(m_elementMetaType)){
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
                        if(!is_less(c->_metaType_id, element1, element2))
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
                        if(!is_equals(c->_metaType_id, element1, element2))
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
            qtjambi_register_comparator(new Comparator(m_elementMetaType, m_offset), newMetaType);
        }
        if(QMetaType::hasRegisteredDebugStreamOperator(m_elementMetaType.id())
                || qtjambi_registered_debugstream_operator(m_elementMetaType.id())
                || m_elementMetaType.id()<QMetaType::User
                || qtjambi_is_pointer_type(m_elementMetaType)){
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
                        debug_stream(dbg, f->_metaType_id, d->data() + i * f->m_offset);
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
            qtjambi_register_debugstream_operator(new DebugStreamFunction(m_elementMetaType, m_offset), newMetaType);
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
    }
    return newMetaType;
}

jobject AutoVectorAccess::createIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr)
{
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
    return qtjambi_from_QIterator(env, ownerId, iteratorPtr, [](void* ptr,bool){
        delete reinterpret_cast<void**>(ptr);
    }, containerAccess);
}

jobject AutoVectorAccess::end(JNIEnv * env, QtJambiNativeID ownerId, const void* container)
{
    QTypedArrayData<char> *const* vector = reinterpret_cast<QTypedArrayData<char> *const*>(container);
    QTypedArrayData<char>* d = *vector;
    void* iteratorPtr = new void*(d->data() + d->size*m_offset);
    return createIterator(env, ownerId, iteratorPtr);
}

jobject AutoVectorAccess::begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container)
{
    QTypedArrayData<char> *const* vector = reinterpret_cast<QTypedArrayData<char> *const*>(container);
    QTypedArrayData<char>* d = *vector;
    void* iteratorPtr = new void*(d->data());
    return createIterator(env, ownerId, iteratorPtr);
}

void AutoVectorAccess::appendVector(JNIEnv * env, void* container, jobject list)
{
    if (qtjambi_is_QVector(env, list, elementMetaType())) {
        if(void* ptr = qtjambi_to_object(env, list)){
            QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container);
            QTypedArrayData<char> ** vector2 = reinterpret_cast<QTypedArrayData<char> **>(ptr);
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
        }
    }else{
        QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container);
        jobject iter = qtjambi_collection_iterator(env, list);
        while(qtjambi_iterator_has_next(env, iter)){
            insert(env, container, (*vector)->size, 1, qtjambi_iterator_next(env, iter));
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
    if(m_internalToExternalConverter(env, nullptr, v, &_value, true)){
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
        if(m_internalToExternalConverter(env, nullptr, v, &_value, true)){
            return _value.l;
        }
    }else{
        void* ptr = m_elementMetaType.create();
        bool success = m_internalToExternalConverter(env, nullptr, ptr, &_value, true);
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
        if(m_internalToExternalConverter(env, nullptr, v, &_value, true)){
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
    return !d->ref.isShared();
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

void AutoVectorAccess::swapItemsAt(JNIEnv *, void* container, jint i, jint j)
{
    QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container);
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
            return is_equals(m_elementMetaType, _qvaluePtr, d->data());
        }
    }
    return false;
}

jint AutoVectorAccess::size(JNIEnv *, const void* container)
{
    QTypedArrayData<char> *const* vector = reinterpret_cast<QTypedArrayData<char> *const*>(container);
    QTypedArrayData<char>* d = *vector;
    return d->size;
}

void AutoVectorAccess::reserve(JNIEnv *, void* container, jint asize)
{
    QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container);
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


void AutoVectorAccess::replace(JNIEnv * env, void* container, jint index, jobject value)
{
        QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container);
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

jint AutoVectorAccess::removeAll(JNIEnv * env, void* container, jobject value)
{
    QtJambiScope scope;
    jint removedCount = 0;
    void *_qvaluePtr = nullptr;
    jvalue _value;
    _value.l = value;
    if(m_externalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l)){
        QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container);
        QTypedArrayData<char>*& d = *vector;
        int index;
        do{
            index = -1;
            for(int i = 0; i<d->size; ++i){
                void* element = d->data() + i * m_offset;
                if(is_equals(m_elementMetaType, _qvaluePtr, element)){
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
    if (qtjambi_is_QVector(env, other, elementMetaType())) {
        if(void* ptr = qtjambi_to_object(env, other)){
            QTypedArrayData<char> *const* vector2 = reinterpret_cast<QTypedArrayData<char> *const*>(ptr);
            QTypedArrayData<char>* d2 = *vector2;
            if(d->size!=d2->size)
                return false;
            if(d==d2)
                return true;
            for(int i = 0; i<d->size; ++i){
                void* element1 = d->data() + i * m_offset;
                void* element2 = d2->data() + i * m_offset;
                if(!is_equals(m_elementMetaType, element1, element2))
                    return false;
            }
            return true;
        }
    }else{
        if(d->size!=qtjambi_collection_size(env, other))
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
            void* element = d->data() + i * m_offset;
            if(!is_equals(m_elementMetaType, element, _qvaluePtr))
                return false;
        }
        return true;
    }
    return false;
}

void AutoVectorAccess::move(JNIEnv *, void* container, jint from, jint to)
{
    if (from == to)
        return;
    QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container);
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

jobject AutoVectorAccess::mid(JNIEnv * env, const void* container, jint _pos, jint _len)
{
    using namespace QtPrivate;
    int pos = _pos;
    int alength = _len;
    QTypedArrayData<char> *const* vector = reinterpret_cast<QTypedArrayData<char> *const*>(container);
    QTypedArrayData<char>* d = *vector;
    switch (QContainerImplHelper::mid(d->size, &pos, &alength)) {
    case QContainerImplHelper::Null:
    case QContainerImplHelper::Empty:
        return qtjambi_from_QVector(env, createContainer(), clone());
    case QContainerImplHelper::Full:
        return qtjambi_from_QVector(env, createContainer(container), this);
    case QContainerImplHelper::Subset:
        break;
    }
    if (alength <= 0)
        return qtjambi_from_QVector(env, createContainer(), clone());

    void* midResult = createContainer();
    reserve(env, midResult, alength);
    QTypedArrayData<char> *const* midVector = reinterpret_cast<QTypedArrayData<char> *const*>(midResult);
    QTypedArrayData<char>* md = *midVector;
    for(int i=0; i<alength; ++i){
        m_elementMetaType.construct(md->data() + i * m_offset, d->data() + (pos+i) * m_offset);
    }
    md->size = alength;
    return qtjambi_from_QVector(env, midResult, clone());
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
                if(is_equals(m_elementMetaType, _qvaluePtr, d->data() + i * m_offset))
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
                if(is_equals(m_elementMetaType, _qvaluePtr, d->data() + i * m_offset))
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
            return is_equals(m_elementMetaType, _qvaluePtr, v);
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
            if(is_equals(m_elementMetaType, _qvaluePtr, d->data() + i * m_offset))
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
            if(is_equals(m_elementMetaType, _qvaluePtr, d->data() + i * m_offset))
                return true;
        }
    }
    return false;
}

void AutoVectorAccess::clear(JNIEnv *, void* container)
{
    QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container);
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

void AutoVectorAccess::fill(JNIEnv * env, void* container, jobject value, jint asize){
    QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container);
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

void AutoVectorAccess::remove(JNIEnv *, void* container, jint index, jint n){
    QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container);
    QTypedArrayData<char>*& d = *vector;
    if (index < 0 || index+n > d->size) {
#if !defined(QT_NO_DEBUG)
        qWarning("QVector::removeAt(): Index out of range.");
#endif
        return;
    }
    erase(vector, d->data() + index * m_offset, d->data() + (index+n) * m_offset);
}

void AutoVectorAccess::insert(JNIEnv * env, void* container, jint index, jint n, jobject value){
    QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container);
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

void AutoVectorAccess::resize(JNIEnv *, void* container, jint asize)
{
    QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container);
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

void AutoVectorAccess::squeeze(JNIEnv *, void* container){
    QTypedArrayData<char> ** vector = reinterpret_cast<QTypedArrayData<char> **>(container);
    QTypedArrayData<char>*& d = *vector;
    if (d->size < int(d->alloc)) {
        if (!d->size) {
            QVector<char> empty;
            assign(container, &empty);
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
