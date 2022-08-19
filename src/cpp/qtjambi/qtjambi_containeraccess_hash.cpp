#include "qtjambi_core.h"
#include <QtCore/QReadWriteLock>
#include <QtCore/QMap>
#include <QtCore/QSharedPointer>
#include <QtCore/QAssociativeIterable>
#include "qtjambi_containeraccess_p.h"
#include "qtjambi_functionpointer.h"
#include "qtjambi_registry_p.h"

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
QDebug& debug_stream(QDebug& debug, uint typeId, const void* ptr);
const QtPrivate::AbstractComparatorFunction * qtjambi_registered_comparator(int typeId);
const QtPrivate::AbstractDebugStreamFunction * qtjambi_registered_debugstream_operator(int typeId);
bool qtjambi_is_static_type(const QMetaType& metaType);
bool is_equals(const QMetaType& keyMetaType, const void * ptr, const void* ptr2);

QThreadStorage<quintptr> AutoHashAccess::currentAccess;
#else
typedef QMap<int, QSharedPointer<class AutoHashAccess>> HashAccessHash;
Q_GLOBAL_STATIC(HashAccessHash, gHashAccessHash)
typedef QMap<int, QtMetaContainerPrivate::QMetaAssociationInterface> MetaAssociationHash;
Q_GLOBAL_STATIC(MetaAssociationHash, gMetaAssociationHash)
Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gLock, (QReadWriteLock::Recursive))

void registerAccess(int newMetaType, const QSharedPointer<class AutoHashAccess>& access){
    QWriteLocker locker(gLock());
    gHashAccessHash->insert(newMetaType, access);
}

QSharedPointer<class AutoHashAccess> getHashAccess(const QtPrivate::QMetaTypeInterface *iface){
    QSharedPointer<class AutoHashAccess> access;
    {
        QReadLocker locker(gLock());
        access = gHashAccessHash->value(iface->typeId.loadAcquire());
    }
    return access;
}
void AutoHashAccess::defaultCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr){
    if(QSharedPointer<class AutoHashAccess> access = getHashAccess(iface)){
        access->constructContainer(ptr);
    }
}
void AutoHashAccess::copyCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, const void *other){
    if(QSharedPointer<class AutoHashAccess> access = getHashAccess(iface)){
        access->constructContainer(ptr, other);
    }
}
void AutoHashAccess::moveCtr(const QtPrivate::QMetaTypeInterface *iface, void *ptr, void *other){
    if(QSharedPointer<class AutoHashAccess> access = getHashAccess(iface)){
        access->constructContainer(ptr, other);
    }
}
void AutoHashAccess::dtor(const QtPrivate::QMetaTypeInterface *iface, void *ptr){
    if(QSharedPointer<class AutoHashAccess> access = getHashAccess(iface)){
        access->destructContainer(ptr);
    }
}
bool AutoHashAccess::equalsFn(const QtPrivate::QMetaTypeInterface *iface, const void *ptr1, const void *ptr2){
    if(QSharedPointer<class AutoHashAccess> access = getHashAccess(iface)){
        return access->equal(ptr1, ptr2);
    }
    return false;
}
void AutoHashAccess::debugStreamFn(const QtPrivate::QMetaTypeInterface *iface, QDebug &s, const void *ptr){
    if(QSharedPointer<class AutoHashAccess> access = getHashAccess(iface)){
        access->debugStream(s, ptr);
    }
}
void AutoHashAccess::dataStreamOutFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, const void *ptr){
    if(QSharedPointer<class AutoHashAccess> access = getHashAccess(iface)){
        access->dataStreamOut(s, ptr);
    }
}
void AutoHashAccess::dataStreamInFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, void *ptr){
    if(QSharedPointer<class AutoHashAccess> access = getHashAccess(iface)){
        access->dataStreamIn(s, ptr);
    }
}

void* AutoHashAccess::constructContainer(void* result, void* container) {
    QHashData*& d = *reinterpret_cast<QHashData**>(container);
    result = new(result) const QHashData*(d);
    d = nullptr;
    return result;
}

void AutoHashAccess::debugStream(QDebug &dbg, const void *ptr){
    const QtPrivate::QMetaTypeInterface *kiface = m_keyMetaType.iface();
    const QtPrivate::QMetaTypeInterface *viface = m_valueMetaType.iface();
    QHashData *const* map = reinterpret_cast<QHashData *const*>(ptr);
    QHashData* d = *map;
    dbg = dbg.nospace().noquote();
    if(m_offset2){
        if(dynamic_cast<AutoMultiHashAccess*>(this))
            dbg << "QMultiHash";
        else
            dbg << "QHash";
        dbg << "(";
        if(d){
            iterator e = d->end(*this);
            iterator n = d->begin(*this);
            int i=0;
            while (n != e) {
                if(i>0)
                    dbg << "; ";
                kiface->debugStream(kiface, dbg, n.key());
                dbg << ",";
                viface->debugStream(viface, dbg, n.value());
                ++n;
                ++i;
            }
        }
        dbg << ")";
    }else{
        dbg << "QSet(";
        if(d){
            iterator e = d->end(*this);
            iterator n = d->begin(*this);
            int i=0;
            while (n != e) {
                if(i>0)
                    dbg << ", ";
                kiface->debugStream(kiface, dbg, n.key());
                ++n;
                ++i;
            }
        }
        dbg << ")";
    }
}

void AutoHashAccess::dataStreamOut(QDataStream &s, const void *ptr){
    const QtPrivate::QMetaTypeInterface *kiface = m_keyMetaType.iface();
    const QtPrivate::QMetaTypeInterface *viface = m_valueMetaType.iface();
    QHashData *const* map = reinterpret_cast<QHashData *const*>(ptr);
    QHashData* d = *map;
    if(d){
        QHashData::iterator e = d->end(*this);
        QHashData::iterator n = d->begin(*this);
        s << quint32(d->size);
        if(m_offset2){
            while (n != e) {
                kiface->dataStreamOut(kiface, s, n.key());
                viface->dataStreamOut(viface, s, n.value());
                ++n;
            }
        }else{
            while (n != e) {
                kiface->dataStreamOut(kiface, s, n.key());
                ++n;
            }
        }
    }
}

void AutoHashAccess::dataStreamIn(QDataStream &s, void *ptr){
    const QtPrivate::QMetaTypeInterface *kiface = m_keyMetaType.iface();
    const QtPrivate::QMetaTypeInterface *viface = m_valueMetaType.iface();
    QHashData ** map = reinterpret_cast<QHashData **>(ptr);
    QHashData*& d = *map;
    quint32 size = 0;
    s >> size;
    if(d){
        const QHashData* empty = nullptr;
        assign(ptr, &empty);
    }
    detach(map);
    if(m_offset2){
        for(size_t i=0; i<size; ++i){
            void* key = operator new(kiface->size);
            if(kiface->defaultCtr)
                kiface->defaultCtr(kiface, key);
            kiface->dataStreamIn(kiface, s, key);
            void* value = operator new(viface->size);
            if(viface->defaultCtr)
                viface->defaultCtr(viface, value);
            viface->dataStreamIn(viface, s, value);
            emplace(ptr, key, value);
            if(kiface->dtor)
                kiface->dtor(kiface, key);
            operator delete(key);
            if(viface->dtor)
                viface->dtor(viface, value);
            operator delete(value);
        }
    }else{
        for(size_t i=0; i<size; ++i){
            void* key = operator new(kiface->size);
            if(kiface->defaultCtr)
                kiface->defaultCtr(kiface, key);
            kiface->dataStreamIn(kiface, s, key);
            emplace(ptr, key, nullptr);
            if(kiface->dtor)
                kiface->dtor(kiface, key);
            operator delete(key);
        }
    }
}
#endif

AutoHashAccess::AutoHashAccess(const AutoHashAccess & other)
    :AbstractHashAccess(),
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
      m_offset1(other.m_offset1),
#endif
      m_offset2(other.m_offset2),
      m_size(other.m_size)
{
}

AutoHashAccess::~AutoHashAccess(){
}

AutoHashAccess::AutoHashAccess(
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
    : AbstractHashAccess(),
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
      m_offset1(0),
#else
      m_align(m_valueMetaType.id()!=QMetaType::Void ? qMax(m_keyMetaType.alignOf(), m_valueMetaType.alignOf()) : m_keyMetaType.alignOf()),
#endif
      m_offset2(0),
      m_size(0)
{
    Q_ASSERT(m_keyInternalToExternalConverter);
    Q_ASSERT(m_keyExternalToInternalConverter);
    Q_ASSERT(m_keyMetaType.id()!=QMetaType::UnknownType
            && m_keyMetaType.id()!=QMetaType::Void);
    Q_ASSERT(m_valueMetaType.id()!=QMetaType::UnknownType);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    m_offset1 = sizeof(void*);
    if(m_offset1%alignof(uint)>0)
        m_offset1 += alignof(uint)-m_offset1%alignof(uint);
    m_offset1 += sizeof(uint);
    if(m_offset1%m_keyAlign>0)
        m_offset1 += m_keyAlign-m_offset1%m_keyAlign;
#endif
    if(m_valueMetaType.id()!=QMetaType::Void){
        Q_ASSERT(m_valueInternalToExternalConverter);
        Q_ASSERT(m_valueExternalToInternalConverter);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        m_offset2 = m_offset1 + m_keyMetaType.sizeOf();
        if(m_offset2%m_valueAlign>0)
            m_offset2 += m_valueAlign-m_offset2%m_valueAlign;
        m_size = m_offset2 + m_valueMetaType.sizeOf();
        if(m_size%m_align>0)
            m_size += m_align-m_size%m_align;
#else
        m_offset2 = m_keyMetaType.sizeOf();
        if(m_offset2%m_valueMetaType.alignOf()>0)
            m_offset2 += m_valueMetaType.alignOf()-m_offset2%m_valueMetaType.alignOf();
        m_size = m_offset2 + m_valueMetaType.sizeOf();
        if(m_size%m_align>0)
            m_size += m_align-m_size%m_align;
#endif
    }else{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        m_size = m_offset1 + m_keyMetaType.sizeOf();
        if(m_size%m_align>0)
            m_size += m_align-m_size%m_align;
#else
        m_size = m_keyMetaType.sizeOf();
        if(m_size%m_align>0)
            m_size += m_align-m_size%m_align;
#endif
        m_offset2 = 0;
    }
}

void AutoHashAccess::dispose(){ delete this; }
bool AutoHashAccess::isConstant() {return false;}
AbstractHashAccess* AutoHashAccess::clone(){ return new AutoHashAccess(*this); }
const QMetaType& AutoHashAccess::keyMetaType(){ return m_keyMetaType; }
const QMetaType& AutoHashAccess::valueMetaType(){ return m_valueMetaType; }

jint AutoHashAccess::size(JNIEnv *,const void* container){
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    return map && *map ? jint((*map)->size) : 0;
}

void AutoHashAccess::detach(QHashData ** map){
    QHashData*& d = *map;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    if (d->ref.isShared()) detach_helper(map);
#else
    if (!d || d->ref.isShared()) d = QHashData::detached(*this, d);
#endif
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
int AutoHashAccess::alignOfNode() { return qMax(int(sizeof(void*)), int(m_align)); }

void AutoHashAccess::deleteNode2(QHashData::Node *node){
    const AutoHashAccess* _this = reinterpret_cast<const AutoHashAccess*>(currentAccess.localData());
    Q_ASSERT(_this);
    char* nodeData = reinterpret_cast<char*>(node);
    _this->m_keyMetaType.destruct(nodeData+_this->m_offset1);
    if(_this->m_offset2)
        _this->m_valueMetaType.destruct(nodeData+_this->m_offset2);
}

void AutoHashAccess::duplicateNode(QHashData::Node *originalNode, void *newNode){
    const AutoHashAccess* _this = reinterpret_cast<const AutoHashAccess*>(currentAccess.localData());
    Q_ASSERT(_this);
    char* originalNodeData = reinterpret_cast<char*>(originalNode);
    QHashData::Node* _newNode = new (newNode) QHashData::Node();
    _newNode->h = originalNode->h;
    _newNode->next = nullptr;
    char* newNodeData = reinterpret_cast<char*>(newNode);
    _this->m_keyMetaType.construct(newNodeData+_this->m_offset1, originalNodeData+_this->m_offset1);
    if(_this->m_offset2)
        _this->m_valueMetaType.construct(newNodeData+_this->m_offset2, originalNodeData+_this->m_offset2);
}

void AutoHashAccess::freeData(QHashData *d){
    quintptr oldLocalData = currentAccess.localData();
    currentAccess.setLocalData(quintptr(this));
    d->free_helper(deleteNode2);
    currentAccess.setLocalData(oldLocalData);
}

void AutoHashAccess::detach_helper(QHashData ** map){
    QHashData*& d = *map;
    quintptr oldLocalData = currentAccess.localData();
    currentAccess.setLocalData(quintptr(this));
    QHashData *x = d->detach_helper(duplicateNode, deleteNode2, int(m_size), alignOfNode());
    currentAccess.setLocalData(oldLocalData);
    if (!d->ref.deref())
        freeData(d);
    d = x;
}

AutoHashAccess::Node **AutoHashAccess::findNode(QHashData *const* map, const void* akey, uint *ahp){
    uint h = 0;
    QHashData* d = *map;
    if (d->numBuckets || ahp) {
        h = m_keyHashFunction(akey, d->seed);
        if (ahp)
            *ahp = h;
    }
    return findNode(map, akey, h);
}

AutoHashAccess::Node **AutoHashAccess::findNode(QHashData *const* map, const void* akey, uint h){
    QHashData *const& d = *map;
    Node *const& e = reinterpret_cast<Node*const&>(d);
    Node **node;

    if (d->numBuckets) {
        node = reinterpret_cast<Node **>(&d->buckets[h % d->numBuckets]);
        Q_ASSERT(*node == e || (*node)->next);
        while (*node != e && !same_key((*node), h, akey))
            node = &(*node)->next;
    } else {
        node = const_cast<Node **>(&e);
    }
    return node;
}

bool AutoHashAccess::same_key(Node * node, const void* key0){
    void* key = reinterpret_cast<char*>(node)+m_offset1;
    return is_equals(m_keyMetaType, key, key0);
}

bool AutoHashAccess::same_key(Node * node, uint h0, const void* key0){
    if(node->h==h0){
        void* key = reinterpret_cast<char*>(node)+m_offset1;
        return is_equals(m_keyMetaType, key, key0);
    }
    return false;
}

AutoHashAccess::Node *AutoHashAccess::createNode(QHashData* d, uint ah, const void* akey, JNIEnv* env, jobject value, Node **anextNode){
    Node* node = reinterpret_cast<Node*>(d->allocateNode(alignOfNode()));
    m_keyMetaType.construct(reinterpret_cast<char*>(node) + m_offset1, akey);
    if(m_offset2){
        void* avalue = reinterpret_cast<char*>(node) + m_offset2;
        m_valueMetaType.construct(avalue);
        jvalue jv;
        jv.l = value;
        m_valueExternalToInternalConverter(env, nullptr, jv, avalue, jValueType::l);
    }
    node->h = ah;
    node->next = *anextNode;
    *anextNode = node;
    ++d->size;
    return node;
}

AutoHashAccess::Node *AutoHashAccess::createNode(QHashData* d, uint ah, const void* akey, QDataStream &s, Node **anextNode){
    Node* node = reinterpret_cast<Node*>(d->allocateNode(alignOfNode()));
    m_keyMetaType.construct(reinterpret_cast<char*>(node) + m_offset1, akey);
    if(m_offset2){
        void* avalue = reinterpret_cast<char*>(node) + m_offset2;
        m_valueMetaType.construct(avalue);
        QMetaType::load(s, m_valueMetaType.id(), avalue);
    }
    node->h = ah;
    node->next = *anextNode;
    *anextNode = node;
    ++d->size;
    return node;
}

AutoHashAccess::Node *AutoHashAccess::createNode(QHashData* d, uint ah, const void* akey, const void* avalue, Node **anextNode){
    Node* node = reinterpret_cast<Node*>(d->allocateNode(alignOfNode()));
    m_keyMetaType.construct(reinterpret_cast<char*>(node) + m_offset1, akey);
    if(m_offset2)
        m_valueMetaType.construct(reinterpret_cast<char*>(node) + m_offset2, avalue);
    node->h = ah;
    node->next = *anextNode;
    *anextNode = node;
    ++d->size;
    return node;
}

void AutoHashAccess::deleteNode(QHashData* d, Node *node)
{
    quintptr oldLocalData = currentAccess.localData();
    currentAccess.setLocalData(quintptr(this));
    deleteNode2(node);
    d->freeNode(node);
    currentAccess.setLocalData(oldLocalData);
}

jobject AutoHashAccess::nodeKey(JNIEnv * env, Node* node)
{
    jvalue jv;
    jv.l = nullptr;
    if(node)
        m_keyInternalToExternalConverter(env, nullptr, reinterpret_cast<char*>(node)+m_offset1, &jv, true);
    return jv.l;
}

jobject AutoHashAccess::nodeValue(JNIEnv * env, Node* node)
{
    jvalue jv;
    jv.l = nullptr;
    if(node)
        m_valueInternalToExternalConverter(env, nullptr, reinterpret_cast<char*>(node)+m_offset2, &jv, true);
    return jv.l;
}

QPair<AutoHashAccess::Iterator,AutoHashAccess::Iterator> AutoHashAccess::equal_range(QHashData *const* map, const void* akey){
    QHashData* d = *map;
    Node* e = reinterpret_cast<Node*>(d);
    Node *node = *findNode(map, akey);
    Iterator firstIt = const_iterator(node);

    if (node != e) {
        // equal keys must hash to the same value and so they all
        // end up in the same bucket. So we can use node->next,
        // which only works within a bucket, instead of (out-of-line)
        // QHashData::nextNode()
        char* nextKey = reinterpret_cast<char*>(node->next) + m_offset1;
        while (node->next != e && is_equals(m_keyMetaType, nextKey, akey)){
            node = node->next;
            nextKey = reinterpret_cast<char*>(node->next) + m_offset1;
        }

        // 'node' may be the last node in the bucket. To produce the end iterator, we'd
        // need to enter the next bucket in this case, so we need to use
        // QHashData::nextNode() here, which, unlike node->next above, can move between
        // buckets.
        node = QHashData::nextNode(node);
    }

    return qMakePair(firstIt, const_iterator(node));
}

AutoHashAccess::Iterator::Value::Value(AutoHashAccess* _access, Node* _i) : access(_access), i(_i){}
bool AutoHashAccess::Iterator::Value::operator==(const Value &o) const {
    return !access->m_offset2 || is_equals(access->m_valueMetaType, reinterpret_cast<char*>(i) + access->m_offset2,  reinterpret_cast<char*>(o.i) + access->m_offset2);
}

bool AutoHashAccess::Iterator::Value::operator!=(const Value &o) const {
    return !operator==(o);
}

AutoHashAccess::Iterator::Iterator(const Iterator& it) : access(it.access), i(it.i){}
AutoHashAccess::Iterator::Iterator(AutoHashAccess* _access, Node* _i) : access(_access), i(_i){}
AutoHashAccess::Iterator &AutoHashAccess::Iterator::operator++() {
    i = QHashData::nextNode(i);
    return *this;
}

AutoHashAccess::Iterator AutoHashAccess::Iterator::operator++(int) {
    Iterator r = *this;
    i = QHashData::nextNode(i);
    return r;
}

inline AutoHashAccess::Iterator &AutoHashAccess::Iterator::operator--() {
    i = QHashData::previousNode(i);
    return *this;
}

inline AutoHashAccess::Iterator AutoHashAccess::Iterator::operator--(int) {
    Iterator r = *this;
    i = QHashData::previousNode(i);
    return r;
}

bool AutoHashAccess::Iterator::operator==(const Iterator &o) const { return i == o.i; }
bool AutoHashAccess::Iterator::operator!=(const Iterator &o) const { return i != o.i; }
const AutoHashAccess::Iterator::Value &AutoHashAccess::Iterator::operator*() const { return *reinterpret_cast<const Value*>(this); }

AutoHashAccess::Iterator AutoHashAccess::const_iterator(Node * node){
    return Iterator(this, node);
}

#else

char* AutoHashAccess::Span::at(const AutoHashAccess& access, size_t i) noexcept
{
    Q_ASSERT(i < NEntries);
    Q_ASSERT(offsets[i] != UnusedEntry);

    return entries + access.m_size * offsets[i];
}

char* AutoHashAccess::Span::at(const AutoHashAccess& access, size_t i) const noexcept
{
    Q_ASSERT(i < NEntries);
    Q_ASSERT(offsets[i] != UnusedEntry);

    return entries + access.m_size * offsets[i];
}

char* AutoHashAccess::Span::atOffset(const AutoHashAccess& access, size_t o) noexcept
{
    Q_ASSERT(o < allocated);

    return entries+access.m_size * o;
}
char* AutoHashAccess::Span::atOffset(const AutoHashAccess& access, size_t o) const noexcept
{
    Q_ASSERT(o < allocated);

    return entries+access.m_size * o;
}

void AutoHashAccess::Span::freeData(const AutoHashAccess& access){
    if (entries) {
        for (auto o : offsets) {
            if (o != UnusedEntry){
                char* entry = entries + access.m_size * o;
                access.m_keyMetaType.destruct(entry);
                if(access.m_offset2>0)
                    access.eraseSpanEntry(entry + access.m_offset2);
            }
        }
        operator delete(entries);
        entries = nullptr;
    }
}

void AutoHashAccess::Span::addStorage(const AutoHashAccess& access){
    Q_ASSERT(allocated < NEntries);
    Q_ASSERT(nextFree == allocated);
    // the hash table should always be between 25 and 50% full
    // this implies that we on average have between 32 and 64 entries
    // in here. The likelihood of having below 16 entries is very small,
    // so start with that and increment by 16 each time we need to add
    // some more space
    const size_t increment = NEntries / 8;
    size_t alloc = allocated + increment;
    char *newEntries = reinterpret_cast<char *>(operator new(alloc * access.m_size));

    // we only add storage if the previous storage was fully filled, so
    // simply copy the old data over
    for (size_t i = 0; i < allocated; ++i) {
        access.m_keyMetaType.construct(newEntries + i * access.m_size, entries + i * access.m_size);
        if(access.m_offset2>0)
            access.copySpanEntry(newEntries + i * access.m_size + access.m_offset2, entries + i * access.m_size +access.m_offset2);
        access.m_keyMetaType.destruct(entries + i * access.m_size);
        if(access.m_offset2>0)
            access.eraseSpanEntry(entries + i * access.m_size + access.m_offset2);
    }
    for (size_t i = allocated; i < allocated + increment; ++i) {
        *reinterpret_cast<unsigned char *>(newEntries + i * access.m_size) = uchar(i + 1);
    }
    operator delete(entries);
    entries = newEntries;
    allocated = uchar(alloc);
}

char* AutoHashAccess::Span::insert(const AutoHashAccess& access, size_t i)
{
    Q_ASSERT(i < NEntries);
    Q_ASSERT(offsets[i] == UnusedEntry);
    if (nextFree == allocated)
        addStorage(access);
    unsigned char entry = nextFree;
    Q_ASSERT(entry < allocated);
    nextFree = *reinterpret_cast<unsigned char*>(entries + entry * access.m_size);
    offsets[i] = entry;
    return entries + entry * access.m_size;
}

size_t AutoHashAccess::Span::offset(size_t i) const noexcept{
    return offsets[i];
}

bool AutoHashAccess::Span::hasNode(size_t i) const noexcept
{
    return (offsets[i] != UnusedEntry);
}

void AutoHashAccess::Span::erase(const AutoHashAccess& access, size_t bucket)
{
    Q_ASSERT(bucket < NEntries);
    Q_ASSERT(offsets[bucket] != UnusedEntry);

    unsigned char entry = offsets[bucket];
    offsets[bucket] = UnusedEntry;

    access.m_keyMetaType.destruct(entries + entry * access.m_size);
    if(access.m_offset2)
        access.eraseSpanEntry(entries + entry * access.m_size + access.m_offset2);
    *reinterpret_cast<unsigned char*>(entries + entry * access.m_size) = nextFree;
    nextFree = entry;
}

void AutoHashAccess::Span::moveLocal(size_t from, size_t to) noexcept
{
    Q_ASSERT(offsets[from] != UnusedEntry);
    Q_ASSERT(offsets[to] == UnusedEntry);
    offsets[to] = offsets[from];
    offsets[from] = UnusedEntry;
}
void AutoHashAccess::Span::moveFromSpan(const AutoHashAccess& access, Span &fromSpan, size_t fromIndex, size_t to)
{
    Q_ASSERT(to < NEntries);
    Q_ASSERT(offsets[to] == UnusedEntry);
    Q_ASSERT(fromIndex < NEntries);
    Q_ASSERT(fromSpan.offsets[fromIndex] != UnusedEntry);
    if (nextFree == allocated)
        addStorage(access);
    Q_ASSERT(nextFree < allocated);
    offsets[to] = nextFree;
    char* toEntry = entries + nextFree * access.m_size;
    nextFree = *toEntry;

    size_t fromOffset = fromSpan.offsets[fromIndex];
    fromSpan.offsets[fromIndex] = UnusedEntry;
    char* fromEntry = fromSpan.entries + fromOffset * access.m_size;

    access.m_keyMetaType.construct(toEntry, fromEntry);
    if(access.m_offset2)
        access.copySpanEntry(toEntry + access.m_offset2, fromEntry + access.m_offset2);
    access.m_keyMetaType.destruct(fromEntry);
    if(access.m_offset2)
        access.eraseSpanEntry(fromEntry + access.m_offset2);
    *fromEntry = fromSpan.nextFree;
    fromSpan.nextFree = static_cast<unsigned char>(fromOffset);
}

void AutoHashAccess::QHashData::destroy(const AutoHashAccess& access){
    size_t nSpans = (numBuckets + Span::LocalBucketMask) / Span::NEntries;
    for(size_t i = 0 ; i<nSpans; ++i){
        spans[i].freeData(access);
    }
    delete[] spans;
    spans = nullptr;
    size = 0;
    delete this;
}

AutoHashAccess::QHashData::QHashData(const AutoHashAccess&, size_t reserve)
{
    numBuckets = QHashPrivate::GrowthPolicy::bucketsForCapacity(reserve);
    size_t nSpans = (numBuckets + Span::LocalBucketMask) / Span::NEntries;
    spans = new Span[nSpans];
    seed = QHashSeed::globalSeed();
}

AutoHashAccess::QHashData::QHashData(const AutoHashAccess& access, const QHashData &other, size_t reserved)
    : size(other.size),
      numBuckets(other.numBuckets),
      seed(other.seed)
{
    if (reserved)
        numBuckets = QHashPrivate::GrowthPolicy::bucketsForCapacity(qMax(size, reserved));
    bool resized = numBuckets != other.numBuckets;
    size_t nSpans = (numBuckets + Span::LocalBucketMask) / Span::NEntries;
    spans = new Span[nSpans];
    size_t otherNSpans = (other.numBuckets + Span::LocalBucketMask) / Span::NEntries;

    for (size_t s = 0; s < otherNSpans; ++s) {
        const Span &span = other.spans[s];
        for (size_t index = 0; index < Span::NEntries; ++index) {
            if (!span.hasNode(index))
                continue;
            const char* n = span.at(access, index);
            iterator it = resized ? find(access, n) : iterator{ access, this, s*Span::NEntries + index };
            Q_ASSERT(it.isUnused());
            char* newNode = spans[it.span()].insert(access, it.index());
            access.m_keyMetaType.construct(newNode, n);
            if(access.m_offset2>0){
                access.copySpanEntry(reinterpret_cast<char*>(newNode)+access.m_offset2, n+access.m_offset2);
            }
        }
    }
}

AutoHashAccess::QHashData::iterator AutoHashAccess::QHashData::find(const AutoHashAccess& access, const void* key) const noexcept
{
    Q_ASSERT(numBuckets > 0);
    size_t hash = access.m_keyHashFunction(key, seed);
    size_t bucket = QHashPrivate::GrowthPolicy::bucketForHash(numBuckets, hash);
    // loop over the buckets until we find the entry we search for
    // or an empty slot, in which case we know the entry doesn't exist
    while (true) {
        // Split the bucket into the indexex of span array, and the local
        // offset inside the span
        size_t span = bucket / Span::NEntries;
        size_t index = bucket & Span::LocalBucketMask;
        Span &s = spans[span];
        size_t offset = s.offset(index);
        if (offset == Span::UnusedEntry) {
            return iterator{ access, this, bucket };
        } else {
            char *n = s.atOffset(access, offset);
            if (access.m_keyMetaType.equals(n, key))
                return iterator{ access, this, bucket };
        }
        bucket = nextBucket(bucket);
    }
}

size_t AutoHashAccess::QHashData::iterator::span() const noexcept { return bucket / Span::NEntries; }
size_t AutoHashAccess::QHashData::iterator::index() const noexcept { return bucket & Span::LocalBucketMask; }
bool AutoHashAccess::QHashData::iterator::isUnused() const noexcept { return !d->spans[span()].hasNode(index()); }

char*AutoHashAccess::QHashData::iterator::node() const noexcept
{
    Q_ASSERT(!isUnused());
    return d->spans[span()].at(*access, index());
}
bool AutoHashAccess::QHashData::iterator::atEnd() const noexcept { return !d; }


AutoHashAccess::QHashData::iterator AutoHashAccess::QHashData::iterator::operator++() noexcept
{
    while (true) {
        ++bucket;
        if (bucket == d->numBuckets) {
            d = nullptr;
            bucket = 0;
            break;
        }
        if (!isUnused())
            break;
    }
    return *this;
}

AutoHashAccess::iterator::iterator(const QHashData::iterator& _i)
    : i(_i) {
    i.access->initializeIterator(*this);
}

bool AutoHashAccess::QHashData::iterator::operator==(iterator other) const noexcept
{ return d == other.d && bucket == other.bucket; }

AutoHashAccess::iterator& AutoHashAccess::iterator::operator++() noexcept{
    return i.access->incrementIterator(*this);
}

char* AutoHashAccess::iterator::value() const noexcept{
    return i.access->iteratorValue(*this);
}

bool AutoHashAccess::iterator::operator==(iterator other) const noexcept
{ return i.access->iteratorEquals(*this, other); }

void AutoHashAccess::initializeIterator(iterator&) const{}

bool AutoHashAccess::iteratorEquals(const iterator& it1, const iterator& it2) const{
    return it1.i==it2.i;
}

AutoHashAccess::QHashData::iterator AutoHashAccess::QHashData::detachedIterator(const AutoHashAccess& access, iterator other) const noexcept
{
    return iterator{access, this, other.bucket};
}

AutoHashAccess::QHashData::iterator AutoHashAccess::QHashData::begin(const AutoHashAccess& access) const noexcept
{
    iterator it{ access, this, 0 };
    if (it.isUnused())
        ++it;
    return it;
}

AutoHashAccess::QHashData::iterator AutoHashAccess::QHashData::end(const AutoHashAccess& access) const noexcept
{
    return iterator(access);
}

AutoHashAccess::QHashData::iterator AutoHashAccess::QHashData::erase(const AutoHashAccess& access, iterator it)
{
    size_t bucket = it.bucket;
    size_t span = bucket / Span::NEntries;
    size_t index = bucket & Span::LocalBucketMask;
    Q_ASSERT(spans[span].hasNode(index));
    spans[span].erase(access, index);
    --size;

    // re-insert the following entries to avoid holes
    size_t hole = bucket;
    size_t next = bucket;
    while (true) {
        next = nextBucket(next);
        size_t nextSpan = next / Span::NEntries;
        size_t nextIndex = next & Span::LocalBucketMask;
        if (!spans[nextSpan].hasNode(nextIndex))
            break;
        size_t hash = access.m_keyHashFunction(spans[nextSpan].at(access, nextIndex), seed);
        size_t newBucket = QHashPrivate::GrowthPolicy::bucketForHash(numBuckets, hash);
        while (true) {
            if (newBucket == next) {
                // nothing to do, item is at the right plae
                break;
            } else if (newBucket == hole) {
                // move into hole
                size_t holeSpan = hole / Span::NEntries;
                size_t holeIndex = hole & Span::LocalBucketMask;
                if (nextSpan == holeSpan) {
                    spans[holeSpan].moveLocal(nextIndex, holeIndex);
                } else {
                    // move between spans, more expensive
                    spans[holeSpan].moveFromSpan(access, spans[nextSpan], nextIndex, holeIndex);
                }
                hole = next;
                break;
            }
            newBucket = nextBucket(newBucket);
        }
    }

    // return correct position of the next element
    if (bucket == numBuckets - 1 || !spans[span].hasNode(index))
        ++it;
    return it;
}

AutoHashAccess::QHashData* AutoHashAccess::QHashData::detached(const AutoHashAccess& access, QHashData* d, size_t size){
    if (!d)
        return new QHashData(access, size);
    QHashData *dd = new QHashData(access, *d, size);
    if (!d->ref.deref())
        delete d;
    return dd;
}

void AutoHashAccess::QHashData::rehash(const AutoHashAccess& access, size_t sizeHint)
{
    if (sizeHint == 0)
        sizeHint = size;
    size_t newBucketCount = QHashPrivate::GrowthPolicy::bucketsForCapacity(sizeHint);

    Span *oldSpans = spans;
    size_t oldBucketCount = numBuckets;
    size_t nSpans = (newBucketCount + Span::LocalBucketMask) / Span::NEntries;
    spans = new Span[nSpans];
    numBuckets = newBucketCount;
    size_t oldNSpans = (oldBucketCount + Span::LocalBucketMask) / Span::NEntries;

    for (size_t s = 0; s < oldNSpans; ++s) {
        Span &span = oldSpans[s];
        for (size_t index = 0; index < Span::NEntries; ++index) {
            if (!span.hasNode(index))
                continue;
            char* n = span.at(access, index);
            iterator it = find(access, n);
            Q_ASSERT(it.isUnused());
            void *newNode = spans[it.span()].insert(access, it.index());
            access.m_keyMetaType.construct(newNode, n);
            if(access.m_offset2)
                access.copySpanEntry(reinterpret_cast<char*>(newNode) + access.m_offset2, n + access.m_offset2);
        }
        span.freeData(access);
    }
    delete[] oldSpans;
}

AutoHashAccess::QHashData::InsertionResult AutoHashAccess::QHashData::findOrInsert(const AutoHashAccess& access, const void*key) noexcept
{
    iterator it{access};
    if (numBuckets > 0) {
        it = find(access, key);
        if (!it.isUnused())
            return { it, true };
    }
    if (shouldGrow()) {
        rehash(access, size + 1);
        it = find(access, key); // need to get a new iterator after rehashing
    }
    Q_ASSERT(it.d);
    Q_ASSERT(it.isUnused());
    spans[it.span()].insert(access, it.index());
    ++size;
    return { it, false };
}

bool AutoHashAccess::isDetached(QHashData *const* map) const noexcept {
    QHashData* d = *map;
    return d && !d->ref.isShared();
}

char* AutoHashAccess::iteratorValue(const iterator& it) const{
    return it.i.value();
}

AutoHashAccess::iterator& AutoHashAccess::incrementIterator(iterator& it) const{
    ++it.i;
    return it;
}

void AutoHashAccess::emplace(void* container, const void* akey, JNIEnv *env, jobject value){
    QHashData ** map = reinterpret_cast<QHashData **>(container);
    QHashData*& d = *map;
    if(d){
        auto result = d->findOrInsert(*this, akey);
        jvalue jv;
        jv.l = value;
        if (!result.initialized){
            m_keyMetaType.construct(result.it.key(), akey);
            if(m_offset2){
                void* avalue = result.it.value();
                m_valueMetaType.construct(avalue);
                m_valueExternalToInternalConverter(env, nullptr, jv, avalue, jValueType::l);
            }
        }else if(m_offset2){
            void* avalue = result.it.value();
            m_valueExternalToInternalConverter(env, nullptr, jv, avalue, jValueType::l);
        }
    }
}

void AutoHashAccess::emplace(void* container, const void* akey, const void* value){
    QHashData ** map = reinterpret_cast<QHashData **>(container);
    QHashData*& d = *map;
    if(d){
        auto result = d->findOrInsert(*this, akey);
        if (!result.initialized){
            m_keyMetaType.construct(result.it.key(), akey);
            if(m_offset2){
                void* avalue = result.it.value();
                m_valueMetaType.construct(avalue, value);
            }
        }else if(m_offset2){
            void* avalue = result.it.value();
            m_valueMetaType.destruct(avalue);
            m_valueMetaType.construct(avalue, value);
        }
    }
}

char* AutoHashAccess::QHashData::findNode(const AutoHashAccess& access, const void *key) const noexcept
{
    if (!size)
        return nullptr;
    iterator it = find(access, key);
    if (it.isUnused())
        return nullptr;
    return it.node();
}

#endif

void AutoHashAccess::insert(JNIEnv *env, void* container, jobject key, jobject value){
    QHashData ** map = reinterpret_cast<QHashData **>(container);
    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        detach(map);
        uint h;
        QHashData*& d = *map;
        Node* e = reinterpret_cast<Node*>(d);
        Node **node = findNode(map, akey, &h);
        if (*node == e) {
            if (d->willGrow())
                node = findNode(map, akey, h);
            createNode(d, h, akey, env, value, node);
            return;
        }
        if(m_offset2){
            jv.l = value;
            void* avalue = reinterpret_cast<char*>(*node) + m_offset2;
            m_valueExternalToInternalConverter(env, nullptr, jv, avalue, jValueType::l);
        }
#else
        detach(map);
        emplace(container, akey, env, value);
#endif
    }
}

jboolean AutoHashAccess::contains(JNIEnv *env, const void* container, jobject key)
{
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    QHashData* d = *map;
    if(d && d->size>0){
        jvalue jv;
        jv.l = key;
        void* akey = nullptr;
        QtJambiScope scope;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            Node* e = reinterpret_cast<Node*>(d);
            return *findNode(map, akey) != e;
#else
            return d->findNode(*this, akey) != nullptr;
#endif
        }
    }
    return false;
}

jobject AutoHashAccess::begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container)
{
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    QHashData* d = *map;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return createIterator(env, ownerId, new void*(d->firstNode()));
#else
    return createIterator(env, ownerId, d ? new iterator(d->begin(*this)) : new iterator(*this));
#endif
}

jobject AutoHashAccess::end(JNIEnv * env, QtJambiNativeID ownerId, const void* container)
{
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    QHashData* d = *map;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    Node* e = reinterpret_cast<Node*>(d);
    return createIterator(env, ownerId, new void*(e));
#else
    return createIterator(env, ownerId, d ? new iterator(d->end(*this)) : new iterator(*this));
#endif
}

void AutoHashAccess::clear(JNIEnv *, void* container)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    const QHashData* empty = &QHashData::shared_null;
#else
    const QHashData* empty = nullptr;
#endif
    assign(container, &empty);
}

void* AutoHashAccess::constructContainer(void* result, const void* container) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    result = new(result) const QHashData*(&QHashData::shared_null);
#else
    result = new(result) const QHashData*(nullptr);
#endif
    if(container)
        assign(result, container);
    return result;
}

bool AutoHashAccess::destructContainer(void* container){
    QHashData ** map = reinterpret_cast<QHashData **>(container);
    QHashData*& d = *map;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    if (!d->ref.deref()) freeData(d);
#else
    if (d && !d->ref.deref()){
        d->destroy(*this);
        d = nullptr;
    }
#endif
    return true;
}

jint AutoHashAccess::count(JNIEnv *env, const void* container, jobject key)
{
    jint result = 0;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    QHashData* d = *map;
    if(d->size>0){
        Node* e = reinterpret_cast<Node*>(d);
        jvalue jv;
        jv.l = key;
        QtJambiScope scope;
        void* akey = nullptr;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            Node *node = *findNode(map, akey);
            if (node != e) {
                do {
                    ++result;
                } while ((node = node->next) != e && same_key(node, akey));
            }
        }
    }
#else
    result = contains(env, container, key) ? 1 : 0;
#endif
    return result;
}

jobject AutoHashAccess::find(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key)
{
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    QHashData* d = *map;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    Node* e = reinterpret_cast<Node*>(d);
    Node *n = nullptr;
    if(d->size>0){
        jvalue jv;
        jv.l = key;
        QtJambiScope scope;
        void* akey = nullptr;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            n = *findNode(map, akey);
        }
    }
    return createIterator(env, ownerId, new void*(n ? n : e));
#else
    if (d && d->size>0){
        jvalue jv;
        jv.l = key;
        QtJambiScope scope;
        void* akey = nullptr;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            auto it = d->find(*this, key);
            if (it.isUnused())
                it = d->end(*this);
            return createIterator(env, ownerId, new iterator(it));
        }
    }
    return createIterator(env, ownerId, new iterator(*this));
#endif
}

jobject AutoHashAccess::key(JNIEnv *env, const void* container, jobject value, jobject defaultKey)
{
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    QHashData* d = *map;
    if(!d || d->size==0)
        return defaultKey;
    jvalue jv;
    jv.l = value;
    QtJambiScope scope;
    void* avalue = nullptr;
    if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        Node *n = d->firstNode();
        Node* e = reinterpret_cast<Node*>(d);
        while (n && n!=e) {
            if(is_equals(m_valueMetaType, reinterpret_cast<char*>(n)+m_offset2, avalue)){
                return nodeKey(env, n);
            }
            n = QHashData::nextNode(n);
        }
#else
        iterator i = d->begin(*this);
        while (i != d->end(*this)) {
            if (m_valueMetaType.equals(i.value(), value)){
                jvalue jv;
                jv.l = nullptr;
                m_keyInternalToExternalConverter(env, nullptr, i.key(), &jv, true);
                return jv.l;
            }
            ++i;
        }
#endif
    }
    return defaultKey;
}

jobject AutoHashAccess::value(JNIEnv *env, const void* container, jobject key, jobject defaultValue)
{
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    QHashData* d = *map;
    if(!d || d->size==0)
        return defaultValue;
    jvalue jv;
    jv.l = key;
    QtJambiScope scope;
    void* akey = nullptr;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        Node* e = reinterpret_cast<Node*>(d);
        Node *n = *findNode(map, akey);
        return n!=e ? nodeValue(env, n) : defaultValue;
#else
        QHashData::iterator iter = d->find(*this, akey);
        if (!iter.isUnused()){
            jvalue jv;
            jv.l = nullptr;
            m_valueInternalToExternalConverter(env, nullptr, iterator(iter).value(), &jv, true);
            return jv.l;
        }
#endif
    }
    return defaultValue;
}

void AutoHashAccess::assign(void* container, const void* other)
{
    QHashData ** map = reinterpret_cast<QHashData **>(container);
    QHashData*& d = *map;
    QHashData *const* map2 = reinterpret_cast<QHashData *const*>(other);
    QHashData* d2 = *map2;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    if(d!=d2 && d2->ref.ref()){
        if (!d->ref.deref())
            freeData(d);
        d = d2;
    }
#else
    if (d != d2) {
        QHashData *o = d2;
        if (o)
            o->ref.ref();
        if (d && !d->ref.deref()){
            d->destroy(*this);
            d = nullptr;
        }
        d = o;
    }
#endif
}

void AutoHashAccess::analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data)
{
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    QHashData* d = *map;
    if(d && d->size>0){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        Node *n = d->firstNode();
        Node* e = reinterpret_cast<Node*>(d);
        while (n && n!=e) {
            void* key = reinterpret_cast<char*>(n)+m_offset1;
            void* value = reinterpret_cast<char*>(n)+m_offset2;
            if(!analyzer(key, value, data))
                break;
            n = QHashData::nextNode(n);
        }
#else
        iterator n = d->begin(*this);
        iterator e = d->end(*this);
        while (n!=e) {
            if(!analyzer(n.key(), n.value(), data))
                break;
            ++n;
        }
#endif
    }
}

IsBiContainerFunction AutoHashAccess::getIsBiContainerFunction(){
    return qtjambi_is_QHash;
}

size_t AutoHashAccess::sizeOf(){
    return sizeof(QHash<char,char>);
}

ushort AutoHashAccess::alignOf() const{
    return ushort(sizeof(QHash<char,char>));
}

bool AutoHashAccess::equal(const void* a, const void* b){
    QHashData *const* map = reinterpret_cast<QHashData *const*>(a);
    QHashData* d = *map;
    QHashData *const* map2 = reinterpret_cast<QHashData *const*>(b);
    QHashData* d2 = *map2;
    if(d==d2)
        return true;
    if((!d && d2) || (!d2 && d))
        return false;
    if(d->size!=d2->size)
        return false;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    Node* e = reinterpret_cast<Node*>(d);
    Node *it = d->firstNode();

    while (it != e) {
        // Build two equal ranges for i.key(); one for *this and one for other.
        // For *this we can avoid a lookup via equal_range, as we know the beginning of the range.
        Iterator thisEqualRangeStart = const_iterator(it);
        char* thisEqualRangeKey = reinterpret_cast<char*>(it)+m_offset1;
        char* otherEqualRangeKey;
        qsizetype n = 0;
        do {
            it = QHashData::nextNode(it);
            ++n;
            otherEqualRangeKey = reinterpret_cast<char*>(it)+m_offset1;
        } while (it != e && is_equals(m_keyMetaType, thisEqualRangeKey, otherEqualRangeKey));

        const auto otherEqualRange = equal_range(map2, thisEqualRangeKey);

        if (n != std::distance(otherEqualRange.first, otherEqualRange.second))
            return false;

        // Keys in the ranges are equal by construction; this checks only the values.
        if (!qt_is_permutation(thisEqualRangeStart, const_iterator(it), otherEqualRange.first, otherEqualRange.second))
            return false;
    }
#else
    QHashData::iterator end = d->end(*this);
    QHashData::iterator end2 = d2->end(*this);
    for (QHashData::iterator it = d2->begin(*this); it != end2; ++it) {
        QHashData::iterator i = d->find(*this, it.key());
        if (i == end || i.isUnused() || (m_offset2>0 && !equalSpanEntries(i.value(), it.value())))
            return false;
    }
#endif
    return true;
}

jboolean AutoHashAccess::equal(JNIEnv *env, const void* container, jobject other)
{
    void* ptr;
    bool deleteSet = false;
    if ((*getIsBiContainerFunction())(env, other, keyMetaType(), valueMetaType())) {
        ptr = qtjambi_to_object(env, other);
    }else{
        deleteSet = true;
        ptr = operator new(sizeOf());
        constructContainer(ptr);
        jobject iterator = qtjambi_map_entryset_iterator(env, other);
        while(qtjambi_iterator_has_next(env, iterator)){
            jobject entry = qtjambi_iterator_next(env, iterator);
            insert(env, ptr, qtjambi_map$entry_key(env, entry), qtjambi_map$entry_value(env, entry));
        }
    }
    bool result = equal(container, ptr);
    if(deleteSet){
        destructContainer(ptr);
        operator delete(ptr);
    }
    return result;
}

int AutoHashAccess::registerContainer(const QByteArray& typeName)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    int newMetaType = QMetaType::type(typeName);
#else
    int newMetaType = QMetaType::fromName(typeName).id();
#endif
    if(newMetaType==QMetaType::UnknownType){
        QSharedPointer<AutoHashAccess> access(dynamic_cast<AutoHashAccess*>(this->clone()), &containerDisposer);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        auto kiface = m_keyMetaType.iface();
        auto viface = m_valueMetaType.iface();
#endif
        newMetaType = registerMetaType(typeName,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                       qtjambi_function_pointer<16,void(void*)>([access](void* ptr){
                                            access->destructContainer(ptr);
                                       }, qHash(typeName)),
                                       [](void* ptr, const void * other) -> void* {
                                            if(other){
                                                QHashData *const* map2 = reinterpret_cast<QHashData *const*>(other);
                                                QHashData* d2 = *map2;
                                                if(d2!=&QHashData::shared_null && d2->ref.ref())
                                                    return new (ptr) QHashData *(d2);
                                            }
                                            return new (ptr) QHashData const*(&QHashData::shared_null);
                                       },
                                       uint(sizeOf()),
                                       alignOf(),
#else
                                       AutoHashAccess::defaultCtr,
                                       AutoHashAccess::copyCtr,
                                       AutoHashAccess::moveCtr,
                                       AutoHashAccess::dtor,
                                       (kiface->equals
                                                    || (kiface->flags & QMetaType::IsPointer)
                                                    || (kiface->flags & QMetaType::IsEnumeration))
                                                && (viface->equals
                                                    || (viface->flags & QMetaType::IsPointer)
                                                    || (viface->flags & QMetaType::IsEnumeration)) ? AutoHashAccess::equalsFn : nullptr,
                                       nullptr,
                                       (kiface->debugStream
                                                    || (kiface->flags & QMetaType::IsPointer)
                                                    || (kiface->flags & QMetaType::IsEnumeration))
                                                && (viface->debugStream
                                                    || (viface->flags & QMetaType::IsPointer)
                                                    || (viface->flags & QMetaType::IsEnumeration)) ? AutoHashAccess::debugStreamFn : nullptr,
                                       (kiface->dataStreamOut
                                                    || (kiface->flags & QMetaType::IsEnumeration))
                                                && (viface->dataStreamOut
                                                    || (viface->flags & QMetaType::IsEnumeration)) ? AutoHashAccess::dataStreamOutFn : nullptr,
                                       (kiface->dataStreamIn
                                                    || (kiface->flags & QMetaType::IsEnumeration))
                                                && (viface->dataStreamIn
                                                    || (viface->flags & QMetaType::IsEnumeration)) ? AutoHashAccess::dataStreamInFn : nullptr,
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
                                                    QHashData *const* map = reinterpret_cast<QHashData *const*>(ptr);
                                                    QHashData* d = *map;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                                    hash_type hashCode = seed;
                                                    Node* e = reinterpret_cast<Node*>(d);
                                                    Node *n = d->firstNode();
                                                    while (n && n != e) {
                                                        void* key = reinterpret_cast<char*>(n)+access->m_offset1;
                                                        void* value = reinterpret_cast<char*>(n)+access->m_offset2;
                                                        hash_type heyHash = access->m_keyHashFunction(key, 0);
                                                        hash_type valueHash = access->m_valueHashFunction(value, 0);
                                                        hash_type pairSeed = 0;
                                                        pairSeed = pairSeed ^ (heyHash + 0x9e3779b9 + (pairSeed << 6) + (pairSeed >> 2));
                                                        pairSeed = pairSeed ^ (valueHash + 0x9e3779b9 + (pairSeed << 6) + (pairSeed >> 2));
                                                        hashCode = hashCode + pairSeed;
                                                        n = QHashData::nextNode(n);
                                                    }
#else
                                                    size_t hashCode = 0;
                                                    if(d){
                                                        iterator e = d->end(*access);
                                                        iterator n = d->begin(*access);
                                                        while (n != e) {
                                                            size_t h = seed ^ (access->m_keyHashFunction(n.key(), 0) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
                                                            hashCode += h ^ (access->m_valueHashFunction(n.value(), 0) + 0x9e3779b9 + (h << 6) + (h >> 2));
                                                            ++n;
                                                        }
                                                    }
#endif
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

                ConverterFunction(QSharedPointer<AutoHashAccess> access, int newMetaType, int keyMetaType, bool keyIsPointer, int valueMetaType, bool valueIsPointer, size_t offset1, size_t offset2)
                    : QtPrivate::AbstractConverterFunction(convert),
                      _metaType_id_first(keyMetaType),
                      _metaType_flags_first(keyIsPointer),
                      _metaType_id_second(valueMetaType),
                      _metaType_flags_second(valueIsPointer),
                      _size([](const void *container) -> int {
                                QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
                                return (*map)->size;
                            }),
                      _find(qtjambi_function_pointer<16,void(const void *, const void *, void **)>([access](const void *container, const void *key, void **iterator){
                                QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
                                *iterator = access->findNode(map, key);
                            }, newMetaType)),
                      _begin([](const void *container, void ** iterator){
                                QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
                                QHashData* d = *map;
                                *iterator = d->firstNode();
                            }),
                      _end([](const void *container, void ** iterator){
                                QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
                                QHashData* d = *map;
                                Node* e = reinterpret_cast<Node*>(d);
                                *iterator = e;
                            }),
                      _advance([](void **iter, int n){
                                Node* node = reinterpret_cast<Node*>(*iter);
                                if(n>0){
                                    for(int i=0; node!=nullptr && i<n; ++i){
                                        node = QHashData::nextNode(node);
                                    }
                                }else if(n<0){
                                    for(int i=0; node!=nullptr && i>n; --i){
                                        node = QHashData::previousNode(node);
                                    }
                                }
                                *iter = node;
                            }),
                      _getKey(qtjambi_function_pointer<16,QtMetaTypePrivate::VariantData(void*const*,int,uint)>([offset1](void * const *iter, int metaTypeId, uint flags)->QtMetaTypePrivate::VariantData{
                                char* node = reinterpret_cast<char*>(*iter);
                                return QtMetaTypePrivate::VariantData(metaTypeId, node+offset1, flags);
                            }, hash_type(offset1))),
                      _getValue(qtjambi_function_pointer<16,QtMetaTypePrivate::VariantData(void*const*,int,uint)>([offset2](void * const *iter, int metaTypeId, uint flags)->QtMetaTypePrivate::VariantData{
                                char* node = reinterpret_cast<char*>(*iter);
                                return QtMetaTypePrivate::VariantData(metaTypeId, node+offset2, flags);
                            }, hash_type(offset1))),
                      _destroyIter([](void **){}),
                      _equalIter([](void * const *iter, void * const *other)->bool{ return *iter==*other; }),
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
            qtjambi_register_converter(new ConverterFunction(access, newMetaType, m_keyMetaType.id(), qtjambi_is_pointer_type(m_keyMetaType), m_valueMetaType.id(), qtjambi_is_pointer_type(m_valueMetaType), m_offset1, m_offset2), newMetaType, to);
        }
        if((QMetaType::hasRegisteredComparators(m_keyMetaType.id())
            || qtjambi_registered_comparator(m_keyMetaType.id())
            || m_keyMetaType.id()<QMetaType::User
            || qtjambi_is_pointer_type(m_keyMetaType))
                && (QMetaType::hasRegisteredComparators(m_valueMetaType.id())
                || qtjambi_registered_comparator(m_valueMetaType.id())
                || m_valueMetaType.id()<QMetaType::User
                || qtjambi_is_pointer_type(m_valueMetaType))){
            struct Comparator : QtPrivate::AbstractComparatorFunction{
                static bool equals(const AbstractComparatorFunction *_c, const void *a, const void *b){
                    const Comparator* c = static_cast<const Comparator*>(_c);
                    return c->access->equal(a, b);
                }
                static void destroy(AbstractComparatorFunction *_c){
                    delete static_cast<Comparator*>(_c);
                }
                QSharedPointer<AutoHashAccess> access;

                Comparator(const QSharedPointer<AutoHashAccess>& _access)
                    : QtPrivate::AbstractComparatorFunction(nullptr, equals, destroy),
                      access(_access)
                {

                }
            };
            qtjambi_register_comparator(new Comparator(access), newMetaType);
        }
        if((QMetaType::hasRegisteredDebugStreamOperator(m_keyMetaType.id())
                && QMetaType::hasRegisteredDebugStreamOperator(m_valueMetaType.id()))
                || (qtjambi_registered_debugstream_operator(m_keyMetaType.id())
                    && qtjambi_registered_debugstream_operator(m_valueMetaType.id()))
                || (m_keyMetaType.id()<QMetaType::User
                    && m_valueMetaType.id()<QMetaType::User)
                || (qtjambi_is_pointer_type(m_keyMetaType)
                    || qtjambi_is_pointer_type(m_valueMetaType))){
            struct DebugStreamFunction : QtPrivate::AbstractDebugStreamFunction{
                static void stream(const AbstractDebugStreamFunction *_f, QDebug& dbg, const void *ptr){
                    const DebugStreamFunction* f = static_cast<const DebugStreamFunction*>(_f);
                    QHashData *const* map = reinterpret_cast<QHashData *const*>(ptr);
                    QHashData* d = *map;
                    Node* e = reinterpret_cast<Node*>(d);
                    Node *n = d->firstNode();
                    dbg = dbg.nospace().noquote();
                    if(dynamic_cast<AutoMultiHashAccess*>(f->access.data()))
                        dbg << "QMultiHash";
                    else
                        dbg << "QHash";
                    dbg << "(";
                    while (n && n != e) {
                        void* key = reinterpret_cast<char*>(n)+f->access->m_offset1;
                        void* value = reinterpret_cast<char*>(n)+f->access->m_offset2;
                        dbg << "(";
                        debug_stream(dbg, f->access->m_keyMetaType.id(), key);
                        dbg << ",";
                        debug_stream(dbg, f->access->m_valueMetaType.id(), value);
                        dbg << ")";
                        n = QHashData::nextNode(n);
                    }
                    dbg << ")";
                }
                static void destroy(AbstractDebugStreamFunction *_f){
                    delete static_cast<DebugStreamFunction*>(_f);
                }
                QSharedPointer<AutoHashAccess> access;

                DebugStreamFunction(const QSharedPointer<AutoHashAccess>& _access)
                    : QtPrivate::AbstractDebugStreamFunction(stream, destroy),
                      access(_access)
                {
                }
            };
            qtjambi_register_debugstream_operator(new DebugStreamFunction(access), newMetaType);
        }
        {
            size_t offset1 = m_offset1;
            size_t offset2 = m_offset2;
            int keyMetaType_id = m_keyMetaType.id();
            int valueMetaType_id = m_valueMetaType.id();
            QMetaType::registerStreamOperators(newMetaType,
                                               qtjambi_function_pointer<16,void(QDataStream &, const void *)>(
                                                                               [keyMetaType_id,valueMetaType_id,offset1,offset2](QDataStream &s, const void * p){
                                                                                       QHashData *const* map = reinterpret_cast<QHashData *const*>(p);
                                                                                       QHashData* d = *map;
                                                                                       Node* e = reinterpret_cast<Node*>(d);
                                                                                       Node *n = d->firstNode();
                                                                                       s << quint32((*map)->size);
                                                                                       while (n && n != e) {
                                                                                           void* key = reinterpret_cast<char*>(n)+offset1;
                                                                                           void* value = reinterpret_cast<char*>(n)+offset2;
                                                                                           QMetaType::save(s, keyMetaType_id, key);
                                                                                           QMetaType::save(s, valueMetaType_id, value);
                                                                                           n = QHashData::nextNode(n);
                                                                                       }
                                                                               }, newMetaType),
                                               qtjambi_function_pointer<16,void(QDataStream &, void *)>(
                                                                               [access](QDataStream &s, void * p){
                                                                                    QHashData ** map = reinterpret_cast<QHashData **>(p);
                                                                                    QHashData*& d = *map;
                                                                                    const QHashData* empty = &QHashData::shared_null;
                                                                                    access->assign(p, &empty);
                                                                                    access->detach(map);
                                                                                    Node* e = reinterpret_cast<Node*>(d);
                                                                                    quint32 size = 0;
                                                                                    s >> size;
                                                                                    for(quint32 i=0; i<size; ++i){
                                                                                        void* key = QMetaType::create(access->m_keyMetaType.id(), nullptr);
                                                                                        QMetaType::load(s, access->m_keyMetaType.id(), key);
                                                                                        uint h;
                                                                                        Node **node = access->findNode(map, key, &h);
                                                                                        if (*node == e || dynamic_cast<AutoMultiHashAccess*>(access.data())) {
                                                                                            if (d->willGrow())
                                                                                                node = access->findNode(map, key, h);
                                                                                            access->createNode(d, h, key, s, node);
                                                                                            continue;
                                                                                        }
                                                                                        void* avalue = reinterpret_cast<char*>(*node) + access->m_offset2;
                                                                                        access->m_valueMetaType.construct(avalue);
                                                                                        QMetaType::load(s, access->m_valueMetaType.id(), avalue);
                                                                                        QMetaType::destroy(access->m_keyMetaType.id(), key);
                                                                                    }
                                                                               }, newMetaType)
                                               );
        }
#else
        {
            const QMetaType to = QMetaType::fromType<QAssociativeIterable>();
            QMetaType::registerMutableViewFunction([newMetaType](void *src, void *target)->bool{
                new(target) QIterable<QMetaAssociation>(QMetaAssociation(createMetaAssociationInterface(newMetaType)), reinterpret_cast<void **>(src));
                return true;
            }, QMetaType(newMetaType), to);
            QMetaType::registerConverterFunction([newMetaType](const void *src, void *target)->bool{
                new(target) QIterable<QMetaAssociation>(QMetaAssociation(createMetaAssociationInterface(newMetaType)), reinterpret_cast<void const*const*>(src));
                return true;
            }, QMetaType(newMetaType), to);
        }
        registerAccess(newMetaType, access);
#endif
    }
    return newMetaType;
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
QtMetaContainerPrivate::QMetaAssociationInterface* AutoHashAccess::createMetaAssociationInterface(int newMetaType){
    {
        QReadLocker locker(gLock());
        Q_UNUSED(locker)
        if(gMetaAssociationHash->contains(newMetaType))
           return &(*gMetaAssociationHash)[newMetaType];
    }
    using namespace QtMetaContainerPrivate;
    QMetaAssociationInterface* metaAssociationInterface;
    {
        QWriteLocker locker(gLock());
        metaAssociationInterface = &(*gMetaAssociationHash)[newMetaType];
    }
    QSharedPointer<class AutoHashAccess> access = getHashAccess(QMetaType(newMetaType).iface());
    metaAssociationInterface->keyMetaType = access->m_keyMetaType.iface();
    metaAssociationInterface->mappedMetaType = access->m_valueMetaType.iface();
    metaAssociationInterface->iteratorCapabilities = InputCapability | ForwardCapability;
    if(dynamic_cast<AutoMultiHashAccess*>(access.data())){
        metaAssociationInterface->sizeFn = [](const void *c) -> qsizetype {
            if(!c)
                return 0;
            AutoMultiHashAccess::MultiHashData const* map = reinterpret_cast<AutoMultiHashAccess::MultiHashData const*>(c);
            return map->m_size;
        };
    }else{
        metaAssociationInterface->sizeFn = [](const void *c) -> qsizetype {
            if(!c)
                return 0;
            QHashData *const* map = reinterpret_cast<QHashData *const*>(c);
            QHashData* d = *map;
            return !d ? 0 : d->size;
        };
    }

    metaAssociationInterface->clearFn = qtjambi_function_pointer<16,void(void *)>([newMetaType](void *c) {
        if(!c)
            return;
        QSharedPointer<class AutoHashAccess> access = getHashAccess(QMetaType(newMetaType).iface());
        if(!access)
            return;
        access->clear(nullptr, c);
    }, newMetaType);
    metaAssociationInterface->createIteratorFn = qtjambi_function_pointer<16,void*(void *,QMetaContainerInterface::Position)>(
                [newMetaType](void *c, QMetaContainerInterface::Position p) -> void* {
                        if(!c)
                            return nullptr;
                        QSharedPointer<class AutoHashAccess> access = getHashAccess(QMetaType(newMetaType).iface());
                        if(!access)
                            return nullptr;
                        QHashData ** map = reinterpret_cast<QHashData **>(c);
                        access->detach(map);
                        QHashData*& d = *map;
                        switch (p) {
                        case QMetaContainerInterface::AtBegin:
                            return new iterator(d->begin(*access));
                        default:
                            return new iterator(d->end(*access));
                        }
                        return nullptr;
                    }, newMetaType
                );
    metaAssociationInterface->destroyConstIteratorFn = [](const void *i) {
                    delete static_cast<const iterator *>(i);
                };
    metaAssociationInterface->compareConstIteratorFn = [](const void *i, const void *j) {
                    return *static_cast<const iterator *>(i) == *static_cast<const iterator *>(j);
                };
    metaAssociationInterface->copyConstIteratorFn = [](void *i, const void *j) {
                    *static_cast<iterator *>(i) = *static_cast<const iterator *>(j);
                };
    metaAssociationInterface->advanceConstIteratorFn = [](void *i, qsizetype step) {
                    iterator& it = *reinterpret_cast<iterator*>(i);
                    if(step>0){
                        for(;step>0; --step)
                            ++it;
                    }
                };
    metaAssociationInterface->diffConstIteratorFn = [](const void *i, const void *j) -> qsizetype {
                    iterator it = *reinterpret_cast<const iterator*>(i);
                    iterator jt = *reinterpret_cast<const iterator*>(j);
                    iterator end(*it.i.access);
                    if(it==jt || it==end)
                        return 0;
                    int counter = 0;
                    while(it!=jt){
                        ++counter;
                        ++it;
                        if(it==end && jt!=end)
                            return 0;
                    }
                    return counter;
                };
    metaAssociationInterface->createConstIteratorFn = qtjambi_function_pointer<16,void*(const void *,QMetaContainerInterface::Position)>(
                [newMetaType](const void *c, QMetaContainerInterface::Position p) -> void* {
                        if(!c)
                            return nullptr;
                        QSharedPointer<class AutoHashAccess> access = getHashAccess(QMetaType(newMetaType).iface());
                        if(!access)
                            return nullptr;
                        QHashData *const* map = reinterpret_cast<QHashData *const*>(c);
                        QHashData* d = *map;
                        switch (p) {
                        case QMetaContainerInterface::AtBegin:
                            return new iterator(d->begin(*access));
                        default:
                            return new iterator(d->end(*access));
                        }
                        return nullptr;
                    }, newMetaType
                );
    metaAssociationInterface->destroyIteratorFn = metaAssociationInterface->destroyConstIteratorFn;
    metaAssociationInterface->compareIteratorFn = metaAssociationInterface->compareConstIteratorFn;
    metaAssociationInterface->copyIteratorFn = metaAssociationInterface->copyConstIteratorFn;
    metaAssociationInterface->advanceIteratorFn = metaAssociationInterface->advanceConstIteratorFn;
    metaAssociationInterface->diffIteratorFn = metaAssociationInterface->diffConstIteratorFn;
    metaAssociationInterface->insertKeyFn = qtjambi_function_pointer<16,void(void *, const void *)>([newMetaType](void *c, const void *k) {
        if(!c)
            return;
        QSharedPointer<class AutoHashAccess> access = getHashAccess(QMetaType(newMetaType).iface());
        if(!access)
            return;
        QHashData ** map = reinterpret_cast<QHashData **>(c);
        access->detach(map);
        access->emplace(c, k, nullptr);
    }, newMetaType);
    metaAssociationInterface->removeKeyFn = qtjambi_function_pointer<16,void(void *, const void *)>([newMetaType](void *c, const void *k) {
        if(!c)
            return;
        QSharedPointer<class AutoHashAccess> access = getHashAccess(QMetaType(newMetaType).iface());
        if(!access)
            return;
        QHashData ** map = reinterpret_cast<QHashData **>(c);
        QHashData*& d = *map;
        auto it = d->find(*access, k);
        access->detach(map);
        it = d->detachedIterator(*access, it);

        if (!it.isUnused()){
            d->erase(*access, it);
        }
    }, newMetaType);
    metaAssociationInterface->containsKeyFn = qtjambi_function_pointer<16,bool(const void *, const void*)>([newMetaType](const void *c, const void* k) -> bool {
        if(!c)
            return false;
        QSharedPointer<class AutoHashAccess> access = getHashAccess(QMetaType(newMetaType).iface());
        QHashData *const* map = reinterpret_cast<QHashData *const*>(c);
        QHashData* d = *map;
        return d->findNode(*access, k) != nullptr;
    }, newMetaType);
    metaAssociationInterface->mappedAtKeyFn = dynamic_cast<AutoMultiHashAccess*>(access.data()) ? QMetaAssociationInterface::MappedAtKeyFn(nullptr) : qtjambi_function_pointer<16,void(const void *, const void *, void *)>([newMetaType](const void *c, const void *k, void *r) {
        if(!c)
            return;
        QSharedPointer<class AutoHashAccess> access = getHashAccess(QMetaType(newMetaType).iface());
        if(!access)
            return;
        QHashData *const* map = reinterpret_cast<QHashData *const*>(c);
        QHashData* d = *map;
        QHashData::iterator iter = d->find(*access, k);
        if (!iter.isUnused()){
            access->m_valueMetaType.destruct(r);
            access->m_valueMetaType.construct(r, iterator(iter).value());
        }
    }, newMetaType);
    metaAssociationInterface->setMappedAtKeyFn = dynamic_cast<AutoMultiHashAccess*>(access.data()) ? QMetaAssociationInterface::SetMappedAtKeyFn(nullptr) : qtjambi_function_pointer<16,void(void *, const void *, const void *)>([newMetaType](void *c, const void *k, const void *r) {
        if(!c)
            return;
        QSharedPointer<class AutoHashAccess> access = getHashAccess(QMetaType(newMetaType).iface());
        if(!access)
            return;
        QHashData ** map = reinterpret_cast<QHashData **>(c);
        access->detach(map);
        access->emplace(c, k, r);
    }, newMetaType);
    metaAssociationInterface->createIteratorAtKeyFn = qtjambi_function_pointer<16,void*(void *, const void *)>([newMetaType](void *c, const void *k) ->void* {
        if(!c)
            return nullptr;
        QSharedPointer<class AutoHashAccess> access = getHashAccess(QMetaType(newMetaType).iface());
        if(!access)
            return nullptr;
        QHashData ** map = reinterpret_cast<QHashData **>(c);
        access->detach(map);
        QHashData*& d = *map;
        return new iterator(d->find(*access, k));
    }, newMetaType);
    metaAssociationInterface->createConstIteratorAtKeyFn = qtjambi_function_pointer<16,void*(const void *, const void *)>([newMetaType](const void *c, const void *k) ->void* {
        if(!c)
            return nullptr;
        QSharedPointer<class AutoHashAccess> access = getHashAccess(QMetaType(newMetaType).iface());
        if(!access)
            return nullptr;
        QHashData *const* map = reinterpret_cast<QHashData *const*>(c);
        QHashData* d = *map;
        return new iterator(d->find(*access, k));
    }, newMetaType);
    metaAssociationInterface->keyAtIteratorFn = [](const void *i, void *k) {
        const iterator* it = reinterpret_cast<const iterator*>(i);
        it->i.access->m_keyMetaType.destruct(k);
        it->i.access->m_keyMetaType.construct(k, it->key());
    };
    metaAssociationInterface->keyAtConstIteratorFn = metaAssociationInterface->keyAtIteratorFn;
    metaAssociationInterface->mappedAtIteratorFn = [](const void *i, void *k) {
        const iterator* it = reinterpret_cast<const iterator*>(i);
        it->i.access->m_valueMetaType.destruct(k);
        it->i.access->m_valueMetaType.construct(k, it->value());
    };
    metaAssociationInterface->mappedAtConstIteratorFn = metaAssociationInterface->mappedAtIteratorFn;
    metaAssociationInterface->setMappedAtIteratorFn = dynamic_cast<AutoMultiHashAccess*>(access.data()) ? QMetaAssociationInterface::SetMappedAtIteratorFn(nullptr) : [](const void *i, const void *v) {
        const iterator* it = reinterpret_cast<const iterator*>(i);
        it->i.access->m_valueMetaType.destruct(it->value());
        it->i.access->m_valueMetaType.construct(it->value(), v);
    };
    metaAssociationInterface->eraseKeyAtIteratorFn = [](void *c, const void *i) {
        const iterator* it = reinterpret_cast<const iterator*>(i);
        AutoHashAccess* access = const_cast<AutoHashAccess*>(it->i.access);
        QHashData ** map = reinterpret_cast<QHashData **>(c);
        QHashData*& d = *map;
        access->detach(map);
        QHashData::iterator _i = d->detachedIterator(*access, it->i);
        if (!_i.isUnused())
            d->erase(*access, _i);
    };
    return metaAssociationInterface;
}
#endif


jobject AutoHashAccess::keys(JNIEnv *env, const void* container)
{
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    QHashData* d = *map;
    AbstractListAccess* listAccess = dynamic_cast<AbstractListAccess*>(qtjambi_create_container_access(env, ContainerType::QList, m_keyMetaType));
    if(!listAccess)
        listAccess = dynamic_cast<AbstractListAccess*>(qtjambi_create_container_access(
                                                                           env,
                                                                           ContainerType::QList,
                                                                           m_keyMetaType,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                           m_keyAlign,
                                                                           m_keyMetaType.sizeOf(),
                                                                           qtjambi_is_static_type(m_keyMetaType),
#else
                                                                           m_keyMetaType.alignOf(),
                                                                           m_keyMetaType.sizeOf(),
#endif
                                                                           qtjambi_is_pointer_type(m_keyMetaType),
                                                                           m_keyHashFunction,
                                                                           m_keyInternalToExternalConverter,
                                                                           m_keyExternalToInternalConverter
                                                                           ));
    if(listAccess){
        void* listContainer = operator new(listAccess->sizeOf());
        listAccess->constructContainer(listContainer);
        listAccess = checkContainerAccess(env, listAccess);
        jint idx = listAccess->size(env, listContainer);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        Node* e = reinterpret_cast<Node*>(d);
        Node *n = d->firstNode();
        listAccess->reserve(env, listContainer, d->size);
        while(n && n != e){
            listAccess->insert(env, listContainer, idx++, 1, nodeKey(env, n));
            n = QHashData::nextNode(n);
        }
#else
        iterator e = d->end(*this);
        iterator n = d->begin(*this);
        while (n != e) {
            jvalue jv;
            jv.l = nullptr;
            m_keyInternalToExternalConverter(env, nullptr, n.key(), &jv, true);
            listAccess->insert(env, listContainer, idx++, 1, jv.l);
            ++n;
        }
#endif
        return qtjambi_from_QList(env, listContainer, listAccess);
    }
    return nullptr;
}

jobject AutoHashAccess::keys(JNIEnv *env, const void* container, jobject value)
{
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    QHashData* d = *map;
    AbstractListAccess* listAccess = dynamic_cast<AbstractListAccess*>(qtjambi_create_container_access(env, ContainerType::QList, m_keyMetaType));
    if(!listAccess)
        listAccess = dynamic_cast<AbstractListAccess*>(qtjambi_create_container_access(
                                                                           env,
                                                                           ContainerType::QList,
                                                                           m_keyMetaType,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                           m_keyAlign,
                                                                           m_keyMetaType.sizeOf(),
                                                                           qtjambi_is_static_type(m_keyMetaType),
#else
                                                                           m_keyMetaType.alignOf(),
                                                                           m_keyMetaType.sizeOf(),
#endif
                                                                           qtjambi_is_pointer_type(m_keyMetaType),
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
        void* listContainer = operator new(listAccess->sizeOf());
        listAccess->constructContainer(listContainer);
        listAccess = checkContainerAccess(env, listAccess);
        jint idx = listAccess->size(env, listContainer);
        if(d && d->size>0 && m_valueExternalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            Node* e = reinterpret_cast<Node*>(d);
            Node *n = d->firstNode();
            while(n != e){
                void* value = reinterpret_cast<char*>(n)+m_offset2;
                if(is_equals(m_valueMetaType, value, _qvaluePtr))
                    listAccess->insert(env, listContainer, idx++, 1, nodeKey(env, n));
                n = QHashData::nextNode(n);
            }
#else
        iterator e = d->end(*this);
        iterator n = d->begin(*this);
        while (n != e) {
            if(m_valueMetaType.equals(n.value(), _qvaluePtr)){
                jvalue jv;
                jv.l = nullptr;
                m_keyInternalToExternalConverter(env, nullptr, n.key(), &jv, true);
                listAccess->insert(env, listContainer, idx++, 1, jv.l);
            }
            ++n;
        }
#endif
        }
        return qtjambi_from_QList(env, listContainer, listAccess);
    }
    return nullptr;
}

jint AutoHashAccess::remove(JNIEnv *env, void* container, jobject key)
{
    QHashData ** map = reinterpret_cast<QHashData **>(container);
    QHashData*& d = *map;
    if(!d || d->size==0)
        return 0;
    jvalue jv;
    jv.l = key;
    QtJambiScope scope;
    void* akey = nullptr;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        auto oldSize = d->size;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        detach(map);
        Node* e = reinterpret_cast<Node*>(d);
        Node **node = findNode(map, akey);
        if (*node != e) {
            bool deleteNext = true;
            do {
                Node *next = (*node)->next;
                deleteNext = (next != e && is_equals(m_keyMetaType, key, reinterpret_cast<char*>(next)+m_offset1));
                deleteNode(d, *node);
                *node = next;
                --d->size;
            } while (deleteNext);
            d->hasShrunk();
        }
#else
        auto it = d->find(*this, akey);
        detach(map);
        it = d->detachedIterator(*this, it);

        if (!it.isUnused()){
            d->erase(*this, it);
        }
#endif
        return jint(oldSize - d->size);
    }
    return 0;
}

jobject AutoHashAccess::take(JNIEnv *env, void* container, jobject key)
{
    QHashData ** map = reinterpret_cast<QHashData **>(container);
    QHashData*& d = *map;
    jvalue jv;
    jv.l = key;
    QtJambiScope scope;
    void* akey = nullptr;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        detach(map);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        Node* e = reinterpret_cast<Node*>(d);
        Node **node = findNode(map, akey);
        if (*node != e) {
            jobject result = *node ? nodeValue(env, *node) : nullptr;
            Node *next = (*node)->next;
            deleteNode(d, *node);
            *node = next;
            --d->size;
            d->hasShrunk();
            return result;
        }
#else
        auto it = d->find(*this, akey);
        detach(map);
        it = d->detachedIterator(*this, it);

        if (!it.isUnused()){
            jvalue jv;
            jv.l = nullptr;
            m_valueInternalToExternalConverter(env, nullptr, it.value(), &jv, true);
            d->erase(*this, it);
            return jv.l;
        }
#endif
    }
    return nullptr;
}

jobject AutoHashAccess::values(JNIEnv *env, const void* container)
{
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    AbstractListAccess* listAccess = dynamic_cast<AbstractListAccess*>(qtjambi_create_container_access(env, ContainerType::QList, m_valueMetaType));
    if(!listAccess)
        listAccess = dynamic_cast<AbstractListAccess*>(qtjambi_create_container_access(
                                                                           env,
                                                                           ContainerType::QList,
                                                                           m_valueMetaType,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                           m_valueAlign,
                                                                           m_valueMetaType.sizeOf(),
                                                                           qtjambi_is_static_type(m_valueMetaType),
#else
                                                                           m_valueMetaType.alignOf(),
                                                                           m_valueMetaType.sizeOf(),
#endif
                                                                           qtjambi_is_pointer_type(m_valueMetaType),
                                                                           m_valueHashFunction,
                                                                           m_valueInternalToExternalConverter,
                                                                           m_valueExternalToInternalConverter
                                                                           ));
    if(listAccess){
        void* listContainer = operator new(listAccess->sizeOf());
        listAccess->constructContainer(listContainer);
        listAccess = checkContainerAccess(env, listAccess);
        QHashData* d = *map;
        jint idx = listAccess->size(env, listContainer);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        Node *firstNode = d->firstNode();
        Node *lastNode = reinterpret_cast<Node*>(d);
        while(firstNode != lastNode){
            listAccess->insert(env, listContainer, idx++, 1, nodeValue(env, firstNode));
            firstNode = QHashData::nextNode(firstNode);
        }
#else
        iterator e = d->end(*this);
        iterator n = d->begin(*this);
        while (n != e) {
            jvalue jv;
            jv.l = nullptr;
            m_valueInternalToExternalConverter(env, nullptr, n.value(), &jv, true);
            listAccess->insert(env, listContainer, idx++, 1, jv.l);
            ++n;
        }
#endif
        return qtjambi_from_QList(env, listContainer, listAccess);
    }
    return nullptr;
}

jobject AutoHashAccess::createIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr)
{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    size_t offset1 = m_offset1;
#endif
    if(m_offset2){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        size_t offset2 = m_offset2;
#endif
        AbstractBiIteratorAccess* containerAccess = new AutoBiIteratorAccess(m_valueInternalToExternalConverter,
                                                                         [](void*ptr){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                            Node* cursor = *reinterpret_cast<Node**>(ptr);
                                                                            *reinterpret_cast<Node**>(ptr) = QHashData::nextNode(cursor);
#else
                                                                            iterator& cursor = *reinterpret_cast<iterator*>(ptr);
                                                                            ++cursor;
#endif
                                                                         },
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         [](void*ptr){
                                                                            Node* cursor = *reinterpret_cast<Node**>(ptr);
                                                                            *reinterpret_cast<Node**>(ptr) = QHashData::previousNode(cursor);
                                                                         },
#else
                                                                         {},
#endif
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                          [offset2](void*ptr)->const void*{
                                                                            char* cursor = *reinterpret_cast<char**>(ptr);
                                                                            return cursor+offset2;
                                                                          },
#else
                                                                          [](void*ptr)->const void*{
                                                                            iterator& cursor = *reinterpret_cast<iterator*>(ptr);
                                                                            return cursor.value();
                                                                          },
#endif
                                                                         {}, // no less
                                                                         [](void*ptr1,void*ptr2)->bool{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                            return *reinterpret_cast<Node**>(ptr1)==*reinterpret_cast<Node**>(ptr2);
#else
                                                                            return *reinterpret_cast<iterator*>(ptr1)==*reinterpret_cast<iterator*>(ptr2);
#endif
                                                                         },
                                                                         m_keyInternalToExternalConverter,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         [offset1](void*ptr)->const void*{
                                                                            char* cursor = *reinterpret_cast<char**>(ptr);
                                                                            return cursor+offset1;
                                                                         }
#else
                                                                        [](void*ptr)->const void*{
                                                                           iterator& cursor = *reinterpret_cast<iterator*>(ptr);
                                                                           return cursor.key();
                                                                        }
#endif
                                                );
        return qtjambi_from_QMapIterator(env, ownerId, iteratorPtr, [](void* ptr,bool){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            delete reinterpret_cast<void**>(ptr);
#else
            delete reinterpret_cast<QHashData::iterator*>(ptr);
#endif
        }, containerAccess);
    }else{
        AutoIteratorAccess* containerAccess = new AutoIteratorAccess(m_keyInternalToExternalConverter,
                                                                         [](void*ptr){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                            Node* cursor = *reinterpret_cast<Node**>(ptr);
                                                                            *reinterpret_cast<Node**>(ptr) = QHashData::nextNode(cursor);
#else
                                                                            iterator& cursor = *reinterpret_cast<iterator*>(ptr);
                                                                            ++cursor;
#endif
                                                                         },
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         [](void*ptr){
                                                                            Node* cursor = *reinterpret_cast<Node**>(ptr);
                                                                            *reinterpret_cast<Node**>(ptr) = QHashData::previousNode(cursor);
                                                                         },
#else
                                                                         {},
#endif
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         [offset1](void*ptr)->const void*{
                                                                            char* cursor = *reinterpret_cast<char**>(ptr);
                                                                            return cursor+offset1;
                                                                         },
#else
                                                                         [](void*ptr)->const void*{
                                                                               iterator& cursor = *reinterpret_cast<iterator*>(ptr);
                                                                               return cursor.key();
                                                                         },
#endif
                                                                         {}, // no less
                                                                         [](void*ptr1,void*ptr2)->bool{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                            return *reinterpret_cast<Node**>(ptr1)==*reinterpret_cast<Node**>(ptr2);
#else
                                                                            return *reinterpret_cast<iterator*>(ptr1)==*reinterpret_cast<iterator*>(ptr2);
#endif
                                                                         }
                                                );
        return qtjambi_from_QIterator(env, ownerId, iteratorPtr, [](void* ptr,bool){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            delete reinterpret_cast<void**>(ptr);
#else
            delete reinterpret_cast<iterator*>(ptr);
#endif
        }, containerAccess);
    }
}

jint AutoHashAccess::capacity(JNIEnv *, const void* container){
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    QHashData* d = *map;
    return d ? jint(d->numBuckets) : 0;
}
void AutoHashAccess::reserve(JNIEnv *, void* container, jint asize){
    QHashData ** map = reinterpret_cast<QHashData **>(container);
    QHashData*& d = *map;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    detach(map);
    d->rehash(-qMax<int>(asize, 1));
#else
    if (isDetached(map))
        d->rehash(*this, size_t(asize));
    else
        d = QHashData::detached(*this, d, size_t(asize));
#endif
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
void AutoHashAccess::eraseSpanEntry(char* value) const{
    m_valueMetaType.destruct(value);
}

void AutoHashAccess::copySpanEntry(char* value1, const char* value2) const{
    m_valueMetaType.construct(value1, value2);
}

bool AutoHashAccess::equalSpanEntries(const char* value1, const char* value2) const{
    return m_valueMetaType.equals(value1, value2);
}
#endif
