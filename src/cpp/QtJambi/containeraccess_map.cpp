#include "qtjambiapi.h"
#include <QtCore/QReadWriteLock>
#include <QtCore/QMap>
#include <QtCore/QSharedPointer>
#include "containeraccess_p.h"
#include "functionpointer.h"
#include "registryutil_p.h"
#include "java_p.h"
#include "utils_p.h"
#include "coreapi.h"

AutoMapAccess::AutoMapAccess(const AutoMapAccess & other)
    :AbstractMapAccess(),
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
      m_offset1(other.m_offset1),
      m_offset2(other.m_offset2),
      m_size(other.m_size)
{
}

AutoMapAccess::~AutoMapAccess(){}

AutoMapAccess::AutoMapAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                int keyMetaType,
                size_t keyAlign,
#else
                const QMetaType& keyMetaType,
#endif
                const QHashFunction& keyHashFunction,
                const InternalToExternalConverter& keyInternalToExternalConverter,
                const ExternalToInternalConverter& keyExternalToInternalConverter,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                int valueMetaType,
                size_t valueAlign,
#else
                const QMetaType& valueMetaType,
#endif
                const QHashFunction& valueHashFunction,
                const InternalToExternalConverter& valueInternalToExternalConverter,
                const ExternalToInternalConverter& valueExternalToInternalConverter
        )
    : AbstractMapAccess(),
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
      m_offset1(0),
      m_offset2(0),
      m_size(0)
{
    Q_ASSERT(m_keyInternalToExternalConverter);
    Q_ASSERT(m_keyExternalToInternalConverter);
    Q_ASSERT(m_valueInternalToExternalConverter);
    Q_ASSERT(m_valueExternalToInternalConverter);
    Q_ASSERT(m_keyMetaType.id()!=QMetaType::UnknownType
            && m_keyMetaType.id()!=QMetaType::Void);
    Q_ASSERT(m_valueMetaType.id()!=QMetaType::UnknownType
            && m_valueMetaType.id()!=QMetaType::Void);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    m_offset1 = sizeof(QMapNodeBase);
    if(m_offset1%m_keyAlign>0)
        m_offset1 += m_keyAlign-m_offset1%m_keyAlign;
    m_offset2 = m_offset1 + m_keyMetaType.sizeOf();
    if(m_offset2%m_valueAlign>0)
        m_offset2 += m_valueAlign-m_offset2%m_valueAlign;
    m_size = m_offset2 + m_valueMetaType.sizeOf();
    if(m_size%m_align>0)
        m_size += m_align-m_size%m_align;
#else
    if(m_offset1%m_keyMetaType.alignOf()>0)
        m_offset1 += m_keyMetaType.alignOf()-m_offset1%m_keyMetaType.alignOf();
    m_offset2 = m_offset1 + m_keyMetaType.sizeOf();
    if(m_offset2%m_valueMetaType.alignOf()>0)
        m_offset2 += m_valueMetaType.alignOf()-m_offset2%m_valueMetaType.alignOf();
    m_size = m_offset2 + m_valueMetaType.sizeOf();
    if(m_size%m_align>0)
        m_size += m_align-m_size%m_align;
#endif
}

void AutoMapAccess::dispose(){ delete this; }
bool AutoMapAccess::isConstant() {return false;}
AbstractMapAccess* AutoMapAccess::clone(){ return new AutoMapAccess(*this); }
const QMetaType& AutoMapAccess::keyMetaType(){ return m_keyMetaType; }
const QMetaType& AutoMapAccess::valueMetaType(){ return m_valueMetaType; }

jint AutoMapAccess::size(JNIEnv *,const void* container){
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    return (*map)->size;
}

void AutoMapAccess::insert(JNIEnv *env, void* container, jobject key, jobject value){
    QMapDataBase ** map = reinterpret_cast<QMapDataBase **>(container);
    QMapDataBase*& d = *map;

    detach(map);

    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l);

    Node *n = rootNode(map);
    Node *y = endNode(map);
    Node *lastNode = nullptr;
    bool  left = true;
    while (n) {
        y = n;
        if (!qMapLessThanKey(*n, akey)) {
            lastNode = n;
            left = true;
            n = n->left;
        } else {
            left = false;
            n = n->right;
        }
    }
    if (lastNode && !qMapLessThanKey(akey, *lastNode)) {
        assignValue(env, value, lastNode);
        return;
    }
    Node* newNode = d->createNode(int(m_size), int(m_align), y, left);
    char* newNodeData = reinterpret_cast<char*>(newNode);
    m_keyMetaType.construct(newNodeData + m_offset1, akey);
    m_valueMetaType.construct(newNodeData + m_offset2);
    assignValue(env, value, newNode);
}

jboolean AutoMapAccess::contains(JNIEnv *env,const void* container, jobject key)
{
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    QMapDataBase * d = *map;
    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l))
        return findNode(d, akey) != nullptr;
    return false;
}

jobject AutoMapAccess::begin(JNIEnv * env, QtJambiNativeID ownerId, void* container)
{
    QMapDataBase ** map = reinterpret_cast<QMapDataBase **>(container);
    detach(map);
    return createIterator(env, ownerId, new void*(beginNode(map)));
}

jobject AutoMapAccess::end(JNIEnv * env, QtJambiNativeID ownerId, void* container)
{
    QMapDataBase ** map = reinterpret_cast<QMapDataBase **>(container);
    detach(map);
    return createIterator(env, ownerId, new void*(endNode(map)));
}

jobject AutoMapAccess::find(JNIEnv * env, QtJambiNativeID ownerId, void* container, jobject key)
{
    QMapDataBase ** map = reinterpret_cast<QMapDataBase **>(container);
    detach(map);
    QMapDataBase* d = *map;
    jvalue jv;
    jv.l = key;
    QtJambiScope scope;
    void* akey = nullptr;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        Node *n = findNode(d, akey);
        return createIterator(env, ownerId, new void*(n ? n : endNode(map)));
    }
    return createIterator(env, ownerId, new void*(endNode(map)));
}

jobject AutoMapAccess::constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container)
{
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    return createIterator(env, ownerId, new void*(beginNode(map)));
}

jobject AutoMapAccess::constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container)
{
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    return createIterator(env, ownerId, new void*(endNode(map)));
}

jobject AutoMapAccess::constFind(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key)
{
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    QMapDataBase* d = *map;
    jvalue jv;
    jv.l = key;
    QtJambiScope scope;
    void* akey = nullptr;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        Node *n = findNode(d, akey);
        return createIterator(env, ownerId, new void*(n ? n : endNode(map)));
    }
    return createIterator(env, ownerId, new void*(endNode(map)));
}

jobject AutoMapAccess::constLowerBound(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key)
{
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    QMapDataBase * d = *map;
    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        Node *lb = rootNode(&d) ? lowerBound(rootNode(&d), akey) : nullptr;
        if (!lb)
            lb = endNode(&d);
        return createIterator(env, ownerId, new void*(lb));
    }
    return createIterator(env, ownerId, new void*(endNode(map)));
}

jobject AutoMapAccess::constUpperBound(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key)
{
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    QMapDataBase * d = *map;
    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        Node *ub = rootNode(&d) ? upperBound(rootNode(&d), akey) : nullptr;
        if (!ub)
            ub = endNode(&d);
        return createIterator(env, ownerId, new void*(ub));
    }
    return createIterator(env, ownerId, new void*(endNode(map)));
}

bool AutoMapAccess::keyLessThan(JNIEnv * env, jobject key1, jobject key2)
{
    jvalue jv;
    jv.l = key1;
    QtJambiScope scope;
    void* akey1 = nullptr;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey1, jValueType::l)){
        jv.l = key2;
        void* akey2 = nullptr;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey2, jValueType::l)){
            return isLessThan(m_keyMetaType, akey1, akey2);
        }
    }
    return false;
}

void AutoMapAccess::clear(JNIEnv *, void* container)
{
    const QMapDataBase* empty = &QMapDataBase::shared_null;
    assign(container, &empty);
}

void* AutoMapAccess::constructContainer(void* result, const void* container)
{
    new(result) const QMapDataBase*(&QMapDataBase::shared_null);
    if(container)
        assign(result, container);
    return result;
}

bool AutoMapAccess::destructContainer(void* container){
    QMapDataBase ** map = reinterpret_cast<QMapDataBase **>(container);
    QMapDataBase*& d = *map;
    if (!d->ref.deref()) destroy(d);
    return true;
}

jint AutoMapAccess::count(JNIEnv *env, const void* container, jobject key)
{
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    QMapDataBase* d = *map;
    jint result = 0;
    jvalue jv;
    jv.l = key;
    QtJambiScope scope;
    void* akey = nullptr;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        Node *firstNode(nullptr);
        Node *lastNode(nullptr);
        nodeRange(d, akey, &firstNode, &lastNode);

        while (firstNode != lastNode) {
            ++result;
            firstNode = firstNode->nextNode();
        }
    }
    return result;
}

jobject AutoMapAccess::first(JNIEnv * env, const void* container)
{
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    QMapDataBase* d = *map;
    if(d->size==0)
        return nullptr;
    Node* first = beginNode(map);
    return nodeValue(env, first);
}

jobject AutoMapAccess::firstKey(JNIEnv *env, const void* container)
{
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    QMapDataBase* d = *map;
    if(d->size==0)
        return nullptr;
    Node* first = beginNode(map);
    return nodeKey(env, first);
}

jobject AutoMapAccess::last(JNIEnv * env, const void* container)
{
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    QMapDataBase* d = *map;
    if(d->size==0)
        return nullptr;
    Node* end = endNode(map);
    return nodeValue(env, end->previousNode());
}

jobject AutoMapAccess::lastKey(JNIEnv *env, const void* container)
{
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    QMapDataBase* d = *map;
    if(d->size==0)
        return nullptr;
    Node* end = endNode(map);
    return nodeKey(env, end->previousNode());
}

jobject AutoMapAccess::key(JNIEnv *env, const void* container, jobject value, jobject defaultKey)
{
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    jvalue jv;
    jv.l = value;
    QtJambiScope scope;
    void* avalue = nullptr;
    if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
        Node *firstNode(beginNode(map));
        Node *lastNode(endNode(map));
        while (firstNode != lastNode) {
            if(isEquals(m_valueMetaType, reinterpret_cast<char*>(firstNode)+m_offset2, avalue)){
                return nodeKey(env, firstNode);
            }
            firstNode = firstNode->nextNode();
        }
    }
    return defaultKey;
}

jobject AutoMapAccess::value(JNIEnv *env, const void* container, jobject key, jobject defaultValue)
{
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    QMapDataBase* d = *map;
    jvalue jv;
    jv.l = key;
    QtJambiScope scope;
    void* akey = nullptr;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        Node *n = findNode(d, akey);
        return n ? nodeValue(env, n) : defaultValue;
    }else{
        return defaultValue;
    }
}

void AutoMapAccess::assign(void* container, const void* other)
{
    QMapDataBase ** map = reinterpret_cast<QMapDataBase **>(container);
    QMapDataBase*& d = *map;
    QMapDataBase *const* map2 = reinterpret_cast<QMapDataBase *const*>(other);
    QMapDataBase* d2 = *map2;
    if(d!=d2 && d2->ref.ref()){
        if (!d->ref.deref())
            destroy(d);
        d = d2;
    }
}

void AutoMapAccess::analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data)
{
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    Node *firstNode(beginNode(map));
    Node *lastNode(endNode(map));
    while (firstNode != lastNode) {
        void* key = reinterpret_cast<char*>(firstNode)+m_offset1;
        void* value = reinterpret_cast<char*>(firstNode)+m_offset2;
        firstNode = firstNode->nextNode();
        if(!analyzer(key, value, data))
            break;
    }
}

IsBiContainerFunction AutoMapAccess::getIsBiContainerFunction(){
    return ContainerAPI::testQMap;
}

bool AutoMapAccess::equal(const void* a, const void* b){
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(a);
    QMapDataBase* d = *map;
    QMapDataBase *const* map2 = reinterpret_cast<QMapDataBase *const*>(b);
    QMapDataBase* d2 = *map2;
    if(d==d2)
        return true;
    if(d->size!=d2->size)
        return false;
    Node *firstNode(beginNode(map));
    Node *lastNode(endNode(map));
    Node *firstNode2(beginNode(map2));
    Node *lastNode2(endNode(map2));
    while (firstNode != lastNode && firstNode2 != lastNode2) {
        void* key = reinterpret_cast<char*>(firstNode)+m_offset1;
        void* key2 = reinterpret_cast<char*>(firstNode2)+m_offset1;
        if(!isEquals(m_keyMetaType, key, key2))
            return false;
        void* value = reinterpret_cast<char*>(firstNode)+m_offset2;
        void* value2 = reinterpret_cast<char*>(firstNode2)+m_offset2;
        if(!isEquals(m_valueMetaType, value, value2))
            return false;
        firstNode = firstNode->nextNode();
        firstNode2 = firstNode2->nextNode();
    }
    return true;
}

jboolean AutoMapAccess::equal(JNIEnv *env, const void* container, jobject other)
{
    if ((*getIsBiContainerFunction())(env, other, keyMetaType(), valueMetaType())) {
        if(void* ptr = QtJambiAPI::convertJavaObjectToNative(env, other)){
            return equal(container, ptr);
        }
        return false;
    }else{
        QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
        QMapDataBase* d = *map;
        if(d->size!=Java::Runtime::Map::size(env, other))
            return false;
        jobject iterator = QtJambiAPI::entrySetIteratorOfJavaMap(env, other);
        QtJambiScope scope;
        jvalue _value;
        void *_qvaluePtr;
        Node *firstNode(beginNode(map));
        Node *lastNode(endNode(map));
        while(firstNode != lastNode && QtJambiAPI::hasJavaIteratorNext(env, iterator)){
            jobject entry = QtJambiAPI::nextOfJavaIterator(env, iterator);
            _value.l = QtJambiAPI::keyOfJavaMapEntry(env, entry);
            _qvaluePtr = nullptr;
            if(!m_keyExternalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l))
                return false;
            void* key = reinterpret_cast<char*>(firstNode)+m_offset1;
            if(!isEquals(m_keyMetaType, key, _qvaluePtr))
                return false;
            _value.l = QtJambiAPI::valueOfJavaMapEntry(env, entry);
            _qvaluePtr = nullptr;
            if(!m_valueExternalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l))
                return false;
            void* value = reinterpret_cast<char*>(firstNode)+m_offset2;
            if(!isEquals(m_valueMetaType, value, _qvaluePtr))
                return false;
            firstNode = firstNode->nextNode();
        }
        return firstNode == lastNode && !QtJambiAPI::hasJavaIteratorNext(env, iterator);
    }
}

int AutoMapAccess::registerContainer(const QByteArray& typeName)
{
    int newMetaType = QMetaType::type(typeName);
    if(newMetaType==QMetaType::UnknownType){
        QSharedPointer<AutoMapAccess> access(dynamic_cast<AutoMapAccess*>(this->clone()), &containerDisposer);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        auto kiface = m_keyMetaType.iface();
        auto viface = m_valueMetaType.iface();
#endif
        newMetaType = registerContainerMetaType(typeName,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                       qtjambi_function_pointer<16,void(void*)>([access](void* ptr){
                                            access->destructContainer(ptr);
                                       }, qHash(typeName)),
                                       [](void* ptr, const void * other) -> void* {
                                            if(other){
                                                QMapDataBase *const* map2 = reinterpret_cast<QMapDataBase *const*>(other);
                                                QMapDataBase* d2 = *map2;
                                                if(d2!=&QMapDataBase::shared_null && d2->ref.ref())
                                                    return new (ptr) QMapDataBase *(d2);
                                            }
                                            return new (ptr) QMapDataBase const*(&QMapDataBase::shared_null);
                                       },
                                       uint(sizeOf()),
                                       alignOf(),
#else
                                       AutoMapAccess::defaultCtr,
                                       AutoMapAccess::copyCtr,
                                       AutoMapAccess::moveCtr,
                                       AutoMapAccess::dtor,
                                       (kiface->equals
                                                    || (kiface->flags & QMetaType::IsPointer)
                                                    || (kiface->flags & QMetaType::IsEnumeration))
                                                && (viface->equals
                                                    || (viface->flags & QMetaType::IsPointer)
                                                    || (viface->flags & QMetaType::IsEnumeration)) ? AutoMapAccess::equalsFn : nullptr,
                                       nullptr,
                                       (kiface->debugStream
                                                    || (kiface->flags & QMetaType::IsPointer)
                                                    || (kiface->flags & QMetaType::IsEnumeration))
                                                && (viface->debugStream
                                                    || (viface->flags & QMetaType::IsPointer)
                                                    || (viface->flags & QMetaType::IsEnumeration)) ? AutoMapAccess::debugStreamFn : nullptr,
                                       (kiface->dataStreamOut
                                                    || (kiface->flags & QMetaType::IsEnumeration))
                                                && (viface->dataStreamOut
                                                    || (viface->flags & QMetaType::IsEnumeration)) ? AutoMapAccess::dataStreamOutFn : nullptr,
                                       (kiface->dataStreamIn
                                                    || (kiface->flags & QMetaType::IsEnumeration))
                                                && (viface->dataStreamIn
                                                    || (viface->flags & QMetaType::IsEnumeration)) ? AutoMapAccess::dataStreamInFn : nullptr,
                                       nullptr,
                                       uint(sizeOf()),
                                       alignOf(),
                                       QMetaType::UnknownType,
#endif
                                       QMetaType::NeedsConstruction
                                                   | QMetaType::NeedsDestruction
                                                   | QMetaType::MovableType,
                                       nullptr,
                                       nullptr);
        if(m_keyHashFunction && m_valueHashFunction){
            insertHashFunctionByMetaType(newMetaType,
                                            [access]
                                            (const void* ptr, hash_type seed)->hash_type{
                                                if(ptr){
                                                    hash_type hashCode = seed;
                                                    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(ptr);
                                                    Node *firstNode(beginNode(map));
                                                    Node *lastNode(endNode(map));
                                                    while (firstNode != lastNode) {
                                                        void* key = reinterpret_cast<char*>(firstNode)+access->m_offset1;
                                                        void* value = reinterpret_cast<char*>(firstNode)+access->m_offset2;
                                                        hash_type heyHash = access->m_keyHashFunction(key, 0);
                                                        hash_type valueHash = access->m_valueHashFunction(value, 0);
                                                        hash_type pairSeed = 0;
                                                        pairSeed = pairSeed ^ (heyHash + 0x9e3779b9 + (pairSeed << 6) + (pairSeed >> 2));
                                                        pairSeed = pairSeed ^ (valueHash + 0x9e3779b9 + (pairSeed << 6) + (pairSeed >> 2));
                                                        hashCode = hashCode + pairSeed;
                                                        firstNode = firstNode->nextNode();
                                                    }
                                                    return hashCode;
                                                }else{
                                                    return 0;
                                                }
                                            });
        }

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        {
            int to = qMetaTypeId<QtMetaTypePrivate::QAssociativeIterableImpl>();
            struct ConverterFunction : QtPrivate::AbstractConverterFunction{
                int _metaType_id_first;
                uint _metaType_flags_first;
                int _metaType_id_second;
                uint _metaType_flags_second;
                QtMetaTypePrivate::QAssociativeIterableImpl::sizeFunc _size;
                QtMetaTypePrivate::QAssociativeIterableImpl::findFunc _find;
                QtMetaTypePrivate::QAssociativeIterableImpl::beginFunc _begin;
                QtMetaTypePrivate::QAssociativeIterableImpl::beginFunc _end;
                QtMetaTypePrivate::QAssociativeIterableImpl::advanceFunc _advance;
                QtMetaTypePrivate::QAssociativeIterableImpl::getFunc _getKey;
                QtMetaTypePrivate::QAssociativeIterableImpl::getFunc _getValue;
                QtMetaTypePrivate::QAssociativeIterableImpl::destroyIterFunc _destroyIter;
                QtMetaTypePrivate::QAssociativeIterableImpl::equalIterFunc _equalIter;
                QtMetaTypePrivate::QAssociativeIterableImpl::copyIterFunc _copyIter;

                ConverterFunction(QSharedPointer<AutoMapAccess> access, int newMetaType, const QMetaType& keyMetaType, const QMetaType& valueMetaType, size_t offset1, size_t offset2)
                    : QtPrivate::AbstractConverterFunction(convert),
                      _metaType_id_first(keyMetaType.id()),
                      _metaType_flags_first(AbstractContainerAccess::isPointerType(keyMetaType)),
                      _metaType_id_second(valueMetaType.id()),
                      _metaType_flags_second(AbstractContainerAccess::isPointerType(valueMetaType)),
                      _size([](const void *p) -> int {
                                QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(p);
                                return (*map)->size;
                            }),
                      _find(qtjambi_function_pointer<16,void(const void *, const void *, void **)>([access](const void *container, const void *key, void **iterator){
                                QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
                                QMapDataBase* d = *map;
                                *iterator = access->findNode(d, key);
                            }, newMetaType)),
                      _begin([](const void *p, void ** iterator){
                                QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(p);
                                *iterator = beginNode(map);
                            }),
                      _end([](const void *p, void ** iterator){
                                QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(p);
                                *iterator = endNode(map);
                            }),
                      _advance([](void **p, int n){
                                Node* node = reinterpret_cast<Node*>(*p);
                                for(int i=0; node!=nullptr && i<n; ++i){
                                    node = node->nextNode();
                                }
                                *p = node;
                            }),
                      _getKey(qtjambi_function_pointer<16,QtMetaTypePrivate::VariantData(void*const*,int,uint)>([offset1](void * const *p, int metaTypeId, uint flags)->QtMetaTypePrivate::VariantData{
                                char* node = reinterpret_cast<char*>(*p);
                                return QtMetaTypePrivate::VariantData(metaTypeId, node+offset1, flags);
                            })),
                      _getValue(qtjambi_function_pointer<16,QtMetaTypePrivate::VariantData(void*const*,int,uint)>([offset2](void * const *p, int metaTypeId, uint flags)->QtMetaTypePrivate::VariantData{
                                char* node = reinterpret_cast<char*>(*p);
                                return QtMetaTypePrivate::VariantData(metaTypeId, node+offset2, flags);
                            })),
                      _destroyIter([](void **){}),
                      _equalIter([](void * const *p, void * const *other)->bool{ return *p==*other; }),
                      _copyIter([](void ** iter1, void * const *iter2){
                                    *reinterpret_cast<Node**>(iter1) = *reinterpret_cast<Node*const*>(iter2);
                                })
                {
                }

                static bool convert(const AbstractConverterFunction *_f, const void *src, void*target){
                    const ConverterFunction* f = static_cast<const ConverterFunction*>(_f);
                    QtMetaTypePrivate::QAssociativeIterableImpl* a = new(target) QtMetaTypePrivate::QAssociativeIterableImpl();
                    a->_iterable = src;
                    a->_metaType_id_key = f->_metaType_id_first;
                    a->_metaType_flags_key = f->_metaType_flags_first;
                    a->_metaType_id_value = f->_metaType_id_second;
                    a->_metaType_flags_value = f->_metaType_flags_second;
                    a->_size = f->_size;
                    a->_find = f->_find;
                    a->_begin = f->_begin;
                    a->_end = f->_end;
                    a->_advance = f->_advance;
                    a->_getKey = f->_getKey;
                    a->_getValue = f->_getValue;
                    a->_destroyIter = f->_destroyIter;
                    a->_equalIter = f->_equalIter;
                    a->_copyIter = f->_copyIter;
                    return true;
                }
            };
            registerConverter(new ConverterFunction(access, newMetaType, m_keyMetaType, m_valueMetaType, m_offset1, m_offset2), newMetaType, to);
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
                static bool equals(const AbstractComparatorFunction *_c, const void *a, const void *b){
                    const Comparator* c = static_cast<const Comparator*>(_c);
                    return c->access->equal(a, b);
                }
                static void destroy(AbstractComparatorFunction *_c){
                    delete static_cast<Comparator*>(_c);
                }
                QSharedPointer<AutoMapAccess> access;

                Comparator(const QSharedPointer<AutoMapAccess>& _access)
                    : QtPrivate::AbstractComparatorFunction(nullptr, equals, destroy),
                      access(_access)
                {

                }
            };
            RegistryAPI::registerComparator(new Comparator(access), newMetaType);
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
                    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(ptr);
                    Node *firstNode(beginNode(map));
                    Node *lastNode(endNode(map));
                    d = d.nospace().noquote();
                    if(dynamic_cast<AutoMultiMapAccess*>(f->access.data()))
                        d << "QMultiMap";
                    else
                        d << "QMap";
                    d << "(";
                    while (firstNode != lastNode) {
                        void* key = reinterpret_cast<char*>(firstNode)+f->access->m_offset1;
                        void* value = reinterpret_cast<char*>(firstNode)+f->access->m_offset2;
                        d << "(";
                        CoreAPI::appendToDebugStream(d, f->access->m_keyMetaType.id(), key);
                        d << ",";
                        CoreAPI::appendToDebugStream(d, f->access->m_valueMetaType.id(), value);
                        d << ")";
                        firstNode = firstNode->nextNode();
                    }
                    d << ")";
                }
                static void destroy(AbstractDebugStreamFunction *_f){
                    delete static_cast<DebugStreamFunction*>(_f);
                }
                QSharedPointer<AutoMapAccess> access;

                DebugStreamFunction(const QSharedPointer<AutoMapAccess>& _access)
                    : QtPrivate::AbstractDebugStreamFunction(stream, destroy),
                      access(_access)
                {

                }
            };
            RegistryAPI::registerDebugStreamOperator(new DebugStreamFunction(access), newMetaType);
        }
        {
            size_t offset1 = m_offset1;
            size_t offset2 = m_offset2;
            int keyMetaType_id = m_keyMetaType.id();
            int valueMetaType_id = m_valueMetaType.id();
            QMetaType::registerStreamOperators(newMetaType,
                                               qtjambi_function_pointer<16,void(QDataStream &, const void *)>(
                                                                               [keyMetaType_id,valueMetaType_id,offset1,offset2](QDataStream &s, const void * p){
                                                                                       QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(p);
                                                                                       Node *firstNode(beginNode(map));
                                                                                       Node *lastNode(endNode(map));
                                                                                       s << quint32((*map)->size);
                                                                                       while (firstNode != lastNode) {
                                                                                           void* key = reinterpret_cast<char*>(firstNode)+offset1;
                                                                                           void* value = reinterpret_cast<char*>(firstNode)+offset2;
                                                                                           QMetaType::save(s, keyMetaType_id, key);
                                                                                           QMetaType::save(s, valueMetaType_id, value);
                                                                                           firstNode = firstNode->nextNode();
                                                                                       }
                                                                               }, newMetaType),
                                               qtjambi_function_pointer<16,void(QDataStream &, void *)>(
                                                                               [access](QDataStream &s, void * p){
                                                                                    QMapDataBase ** map = reinterpret_cast<QMapDataBase **>(p);
                                                                                    const QMapDataBase* empty = &QMapDataBase::shared_null;
                                                                                    access->assign(p, &empty);
                                                                                    access->detach(map);
                                                                                    quint32 size = 0;
                                                                                    s >> size;
                                                                                    for(quint32 i=0; i<size; ++i){
                                                                                        void* key = QMetaType::create(access->m_keyMetaType.id(), nullptr);
                                                                                        QMetaType::load(s, access->m_keyMetaType.id(), key);

                                                                                        Node *n = rootNode(map);
                                                                                        Node *y = endNode(map);
                                                                                        bool  left = true;
                                                                                        while (n) {
                                                                                            y = n;
                                                                                            if (!isLessThan(access->m_keyMetaType, reinterpret_cast<const char*>(n) + access->m_offset1, key)) {
                                                                                                left = true;
                                                                                                n = n->left;
                                                                                            } else {
                                                                                                left = false;
                                                                                                n = n->right;
                                                                                            }
                                                                                        }
                                                                                        Node* newNode = (*map)->createNode(int(access->m_size), int(access->m_align), y, left);
                                                                                        char* newNodeData = reinterpret_cast<char*>(newNode);
                                                                                        QMetaType::construct(access->m_keyMetaType.id(), newNodeData + access->m_offset1, key);
                                                                                        QMetaType::destroy(access->m_keyMetaType.id(), key);
                                                                                        QMetaType::construct(access->m_valueMetaType.id(), newNodeData + access->m_offset2, nullptr);
                                                                                        QMetaType::load(s, access->m_valueMetaType.id(), newNodeData + access->m_offset2);
                                                                                    }
                                                                               }, newMetaType)
                                               );
        }
#else
        {
            const QMetaType to = QMetaType::fromType<QtMetaTypePrivate::QPairVariantInterfaceImpl>();
            QMetaType::ConverterFunction o = [access](const void *src, void *target)->bool{
                QtMetaTypePrivate::QPairVariantInterfaceImpl* impl = new(target) QtMetaTypePrivate::QPairVariantInterfaceImpl();
                impl->_pair = src;
                impl->_metaType_first = access->m_keyMetaType;
                impl->_metaType_second = access->m_valueMetaType;
                impl->_getFirst = qtjambi_function_pointer<16,void(const void * const *, void *)>(
                            [access](const void * const *pair, void *dataPtr){
                                const void* fst = reinterpret_cast<const char*>(*pair);
                                access->m_keyMetaType.construct(dataPtr, fst);
                            }, newMetaType);
                impl->_getSecond = qtjambi_function_pointer<16,void(const void * const *, void *)>(
                            [access](const void * const *pair, void *dataPtr) {
                                const void* snd = reinterpret_cast<const char*>(*pair)+access->m_offset;
                                access->m_valueMetaType.construct(dataPtr, snd);
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

jobject AutoMapAccess::keys(JNIEnv *env, const void* container)
{
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    QMapDataBase* d = *map;
    AbstractListAccess* listAccess = dynamic_cast<AbstractListAccess*>(AbstractContainerAccess::create(env, SequentialContainerType::QList, m_keyMetaType));
    if(!listAccess)
        listAccess = dynamic_cast<AbstractListAccess*>(AbstractContainerAccess::create(
                                                                           env,
                                                                           SequentialContainerType::QList,
                                                                           m_keyMetaType,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                           m_keyAlign,
                                                                           m_keyMetaType.sizeOf(),
                                                                           AbstractContainerAccess::isStaticType(m_keyMetaType),
#else
                                                                           m_keyMetaType.alignOf(),
                                                                           m_keyMetaType.sizeOf(),
#endif
                                                                           AbstractContainerAccess::isPointerType(m_keyMetaType),
                                                                           m_keyHashFunction,
                                                                           m_keyInternalToExternalConverter,
                                                                           m_keyExternalToInternalConverter
                                                                           ));
    if(listAccess){
        void* listContainer = listAccess->createContainer();
        listAccess = checkContainerAccess(env, listAccess);
        Node *firstNode(beginNode(map));
        Node *lastNode(endNode(map));
        listAccess->reserve(env, listContainer, d->size);
        jint idx = listAccess->size(env, listContainer);
        while(firstNode != lastNode){
            listAccess->insert(env, listContainer, idx++, 1, nodeKey(env, firstNode));
            firstNode = firstNode->nextNode();
        }
        return ContainerAccessAPI::objectFromQList(env, listContainer, listAccess);
    }
    return nullptr;
}

jobject AutoMapAccess::keys(JNIEnv *env, const void* container, jobject value)
{
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    AbstractListAccess* listAccess = dynamic_cast<AbstractListAccess*>(AbstractContainerAccess::create(env, SequentialContainerType::QList, m_keyMetaType));
    if(!listAccess)
        listAccess = dynamic_cast<AbstractListAccess*>(AbstractContainerAccess::create(
                                                                           env,
                                                                           SequentialContainerType::QList,
                                                                           m_keyMetaType,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                           m_keyAlign,
                                                                           m_keyMetaType.sizeOf(),
                                                                           AbstractContainerAccess::isStaticType(m_keyMetaType),
#else
                                                                           m_keyMetaType.alignOf(),
                                                                           m_keyMetaType.sizeOf(),
#endif
                                                                           AbstractContainerAccess::isPointerType(m_keyMetaType),
                                                                           m_keyHashFunction,
                                                                           m_keyInternalToExternalConverter,
                                                                           m_keyExternalToInternalConverter
                                                                           ));
    if(listAccess){
        QtJambiScope scope;
        jvalue _value;
        void *_qvaluePtr;
        _value.l = value;
        _qvaluePtr = nullptr;
        void* listContainer = listAccess->createContainer();
        if((*map)->size>0 && m_valueExternalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l)){
            listAccess = checkContainerAccess(env, listAccess);
            Node *firstNode(beginNode(map));
            Node *lastNode(endNode(map));
            jint idx = listAccess->size(env, listContainer);
            while(firstNode != lastNode){
                void* value = reinterpret_cast<char*>(firstNode)+m_offset2;
                if(isEquals(m_valueMetaType, value, _qvaluePtr))
                    listAccess->insert(env, listContainer, idx++, 1, nodeKey(env, firstNode));
                firstNode = firstNode->nextNode();
            }
        }
        return ContainerAccessAPI::objectFromQList(env, listContainer, listAccess);
    }
    return nullptr;
}

jint AutoMapAccess::remove(JNIEnv *env, void* container, jobject key)
{
    QMapDataBase ** map = reinterpret_cast<QMapDataBase **>(container);
    QMapDataBase*& d = *map;
    jvalue jv;
    jv.l = key;
    QtJambiScope scope;
    void* akey = nullptr;
    int n = 0;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        detach(map);
        while (Node *node = findNode(d, akey)) {
            deleteNode(d, node);
            ++n;
        }
    }
    return n;
}

jobject AutoMapAccess::take(JNIEnv *env, void* container, jobject key)
{
    QMapDataBase ** map = reinterpret_cast<QMapDataBase **>(container);
    QMapDataBase*& d = *map;
    jvalue jv;
    jv.l = key;
    QtJambiScope scope;
    void* akey = nullptr;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        detach(map);
        Node *node = findNode(d, akey);
        jobject result = node ? nodeValue(env, node) : nullptr;
        deleteNode(d, node);
        return result;
    }
    return nullptr;
}

jobject AutoMapAccess::values(JNIEnv *env, const void* container)
{
    QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
    AbstractListAccess* listAccess = dynamic_cast<AbstractListAccess*>(AbstractContainerAccess::create(env, SequentialContainerType::QList, m_valueMetaType));
    if(!listAccess)
        listAccess = dynamic_cast<AbstractListAccess*>(AbstractContainerAccess::create(
                                                                           env,
                                                                           SequentialContainerType::QList,
                                                                           m_valueMetaType,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                           m_valueAlign,
                                                                           m_valueMetaType.sizeOf(),
                                                                           AbstractContainerAccess::isStaticType(m_valueMetaType),
#else
                                                                           m_valueMetaType.alignOf(),
                                                                           m_valueMetaType.sizeOf(),
#endif
                                                                           AbstractContainerAccess::isPointerType(m_valueMetaType),
                                                                           m_valueHashFunction,
                                                                           m_valueInternalToExternalConverter,
                                                                           m_valueExternalToInternalConverter
                                                                           ));
    if(listAccess){
        void* listContainer = listAccess->createContainer();
        listAccess = checkContainerAccess(env, listAccess);
        Node *firstNode(beginNode(map));
        Node *lastNode(endNode(map));
        jint idx = listAccess->size(env, listContainer);
        while(firstNode != lastNode){
            listAccess->insert(env, listContainer, idx++, 1, nodeValue(env, firstNode));
            firstNode = firstNode->nextNode();
        }
        return ContainerAccessAPI::objectFromQList(env, listContainer, listAccess);
    }
    return nullptr;
}

void AutoMapAccess::deleteNode(QMapDataBase * d, Node* node){
    m_keyMetaType.destruct(reinterpret_cast<char*>(node) + m_offset1);
    m_valueMetaType.destruct(reinterpret_cast<char*>(node) + m_offset2);
    d->freeNodeAndRebalance(node);
}

void AutoMapAccess::detach(QMapDataBase ** map){
    QMapDataBase* d = *map;
    if (d->ref.isShared()) detach_helper(map);
}

void AutoMapAccess::detach_helper(QMapDataBase ** map){
    QMapDataBase*& d = *map;
    QMapDataBase *x = QMapDataBase::createData();
    if (d->header.left) {
        x->header.left = copyNode(static_cast<Node *>(d->header.left), x);
        x->header.left->setParent(&x->header);
    }
    if (!d->ref.deref())
        destroy(d);
    d = x;
    d->recalcMostLeftNode();
}

void AutoMapAccess::destroy(QMapDataBase* data){
    if (rootNode(&data)) {
        destroySubTree(rootNode(&data));
        data->freeTree(data->header.left, int(m_align));
    }
    QMapDataBase::freeData(data);
}

void AutoMapAccess::destroySubTree(Node* node)
{
    m_keyMetaType.destruct(reinterpret_cast<char*>(node) + m_offset1);
    m_valueMetaType.destruct(reinterpret_cast<char*>(node) + m_offset2);
    if (node->left)
        destroySubTree(node->left);
    if (node->right)
        destroySubTree(node->right);
}

AutoMapAccess::Node* AutoMapAccess::copyNode(Node* node, QMapDataBase* d){
    Node* n = d->createNode(int(m_size), int(m_align), nullptr, false);
    char* nodeData = reinterpret_cast<char*>(node);
    char* nData = reinterpret_cast<char*>(n);
    QT_TRY {
        m_keyMetaType.construct(nData+m_offset1, nodeData+m_offset1);
        QT_TRY {
            m_valueMetaType.construct(nData+m_offset2, nodeData+m_offset2);
        } QT_CATCH(...) {
            m_keyMetaType.destruct(nData + m_offset1);
            QT_RETHROW;
        }
    } QT_CATCH(...) {
        d->freeNodeAndRebalance(n);
        QT_RETHROW;
    }
    n->setColor(node->color());
    if (node->left) {
        n->left = copyNode(node->left, d);
        n->left->setParent(n);
    } else {
        n->left = nullptr;
    }
    if (node->right) {
        n->right = copyNode(node->right, d);
        n->right->setParent(n);
    } else {
        n->right = nullptr;
    }
    return n;
}

AutoMapAccess::Node* AutoMapAccess::rootNode(QMapDataBase *const* map){
    QMapDataBase* d = *map;
    return reinterpret_cast<Node*>(d->header.left);
}

AutoMapAccess::Node* AutoMapAccess::beginNode(QMapDataBase *const* map){
    QMapDataBase* d = *map;
    if (rootNode(map)) return static_cast<Node*>(d->mostLeftNode); return endNode(map);
    return reinterpret_cast<Node*>(d->header.left);
}

AutoMapAccess::Node* AutoMapAccess::endNode(QMapDataBase *const* map){
    QMapDataBase* d = *map;
    return reinterpret_cast<Node*>(&d->header);
}

AutoMapAccess::Node* AutoMapAccess::lowerBound(Node* n, const void* akey){
    Node *lastNode = nullptr;
    while (n) {
        if (!qMapLessThanKey(*n, akey)) {
            lastNode = n;
            n = n->left;
        } else {
            n = n->right;
        }
    }
    return lastNode;
}

AutoMapAccess::Node* AutoMapAccess::upperBound(Node* n, const void* akey){
    Node *lastNode = nullptr;
    while (n) {
        if (qMapLessThanKey(akey, *n)) {
            lastNode = n;
            n = n->left;
        } else {
            n = n->right;
        }
    }
    return lastNode;
}

AutoMapAccess::Node* AutoMapAccess::findNode(QMapDataBase * d, const void* akey){
    if (Node *r = rootNode(&d)) {
        Node *lb = lowerBound(r, akey);
        if (lb && !qMapLessThanKey(akey, *lb))
            return lb;
    }
    return nullptr;
}

void AutoMapAccess::nodeRange(QMapDataBase* base, const void* akey, Node **firstNode, Node **lastNode){
    Node *n = rootNode(&base);
    Node *l = endNode(&base);
    while (n) {
        if (qMapLessThanKey(akey, *n)) {
            l = n;
            n = n->left;
        } else if (qMapLessThanKey(*n, akey)) {
            n = n->right;
        } else {
            *firstNode = n->left ? lowerBound(n->left, akey) : nullptr;
            if (!*firstNode)
                *firstNode = n;
            *lastNode = n->right ? upperBound(n->right, akey) : nullptr;
            if (!*lastNode)
                *lastNode = l;
            return;
        }
    }
    *firstNode = *lastNode = l;
}

void AutoMapAccess::assignValue(JNIEnv * env,jobject value, Node* node){
    jvalue jv;
    jv.l = value;
    void* _qt_value = reinterpret_cast<char*>(node) + m_offset2;
    m_valueExternalToInternalConverter(env, nullptr, jv, _qt_value, jValueType::l);
}

jobject AutoMapAccess::nodeKey(JNIEnv * env, Node* node)
{
    jvalue jv;
    jv.l = nullptr;
    if(node)
        m_keyInternalToExternalConverter(env, nullptr, reinterpret_cast<char*>(node)+m_offset1, &jv, true);
    return jv.l;
}

jobject AutoMapAccess::nodeValue(JNIEnv * env, Node* node)
{
    jvalue jv;
    jv.l = nullptr;
    if(node)
        m_valueInternalToExternalConverter(env, nullptr, reinterpret_cast<char*>(node)+m_offset2, &jv, true);
    return jv.l;
}

bool AutoMapAccess::qMapLessThanKey(const void* akey, const Node& right){
    return isLessThan(m_keyMetaType, akey, reinterpret_cast<const char*>(&right) + m_offset1);
}

bool AutoMapAccess::qMapLessThanKey(const Node& left, const void* akey){
    return isLessThan(m_keyMetaType, reinterpret_cast<const char*>(&left) + m_offset1, akey);
}

jobject AutoMapAccess::createConstIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr)
{
    size_t offset1 = m_offset1;
    size_t offset2 = m_offset2;
    AutoAssociativeConstIteratorAccess* containerAccess = new AutoAssociativeConstIteratorAccess(m_valueInternalToExternalConverter,
                                                                     [](void*ptr){
                                                                        Node* cursor = *reinterpret_cast<Node**>(ptr);
                                                                        *reinterpret_cast<Node**>(ptr) = cursor->nextNode();
                                                                     },
                                                                     [](void*ptr){
                                                                        Node* cursor = *reinterpret_cast<Node**>(ptr);
                                                                        *reinterpret_cast<Node**>(ptr) = cursor->previousNode();
                                                                     },
                                                                     [offset2](const void*ptr)->const void*{
                                                                        char* cursor = *reinterpret_cast<char*const*>(ptr);
                                                                        return cursor+offset2;
                                                                     },
                                                                     {},
                                                                     [](const void*ptr1,const void*ptr2)->bool{
                                                                        return *reinterpret_cast<Node*const*>(ptr1)==*reinterpret_cast<Node*const*>(ptr2);
                                                                     },
                                                                     m_keyInternalToExternalConverter,
                                                                     [offset1](const void*ptr)->const void*{
                                                                        char* cursor = *reinterpret_cast<char*const*>(ptr);
                                                                        return cursor+offset1;
                                                                     }
                                            );
    return QtJambiAPI::convertQAssociativeIteratorToJavaObject(env, ownerId, iteratorPtr, [](void* ptr,bool){
        delete reinterpret_cast<void**>(ptr);
    }, containerAccess);
}

jobject AutoMapAccess::createIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr)
{
    size_t offset1 = m_offset1;
    size_t offset2 = m_offset2;
    AbstractAssociativeIteratorAccess* containerAccess = new AutoAssociativeIteratorAccess(m_valueInternalToExternalConverter,
                                                                     [](void*ptr){
                                                                        Node* cursor = *reinterpret_cast<Node**>(ptr);
                                                                        *reinterpret_cast<Node**>(ptr) = cursor->nextNode();
                                                                     },
                                                                     [](void*ptr){
                                                                        Node* cursor = *reinterpret_cast<Node**>(ptr);
                                                                        *reinterpret_cast<Node**>(ptr) = cursor->previousNode();
                                                                     },
                                                                     [offset2](const void*ptr)->const void*{
                                                                        char* cursor = *reinterpret_cast<char*const*>(ptr);
                                                                        return cursor+offset2;
                                                                     },
                                                                     {},
                                                                     [](const void*ptr1,const void*ptr2)->bool{
                                                                        return *reinterpret_cast<Node*const*>(ptr1)==*reinterpret_cast<Node*const*>(ptr2);
                                                                     },
                                                                     m_keyInternalToExternalConverter,
                                                                     [offset1](const void*ptr)->const void*{
                                                                        char* cursor = *reinterpret_cast<char*const*>(ptr);
                                                                        return cursor+offset1;
                                                                     },
                                                                     m_valueExternalToInternalConverter,
                                                                     [offset2](void*ptr)->void*{
                                                                        char* cursor = *reinterpret_cast<char**>(ptr);
                                                                        return cursor+offset2;
                                                                     }
                                            );
    return QtJambiAPI::convertQAssociativeIteratorToJavaObject(env, ownerId, iteratorPtr, [](void* ptr,bool){
        delete reinterpret_cast<void**>(ptr);
    }, containerAccess);
}

size_t AutoMapAccess::sizeOf() {
    return sizeof(QMap<char,char>);
}

ushort AutoMapAccess::alignOf() const{
    return ushort(sizeof(QMap<char,char>));
}
