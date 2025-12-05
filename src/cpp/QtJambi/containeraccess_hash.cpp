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

#include "pch_p.h"

QT_WARNING_DISABLE_GCC("-Winaccessible-base")
QT_WARNING_DISABLE_CLANG("-Winaccessible-base")

QReadWriteLock* containerAccessLock();
QMap<int, QtMetaContainerPrivate::QMetaAssociationInterface>& metaAssociationHash();

QSharedPointer<AutoHashAccess> getHashAccess(const QtPrivate::QMetaTypeInterface *iface){
    return findContainerAccess(iface->typeId.loadAcquire()).dynamicCast<AutoHashAccess>();
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
void AutoHashAccess::debugStreamSetFn(const QtPrivate::QMetaTypeInterface *iface, QDebug &dbg, const void *ptr){
    if(QSharedPointer<class AutoHashAccess> access = getHashAccess(iface)){
        QtPrivate::printSequentialContainer(dbg, "QSet", ConstContainer{ptr, access.data()});
    }
}
void AutoHashAccess::dataStreamOutSetFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, const void *ptr){
    if(QSharedPointer<class AutoHashAccess> access = getHashAccess(iface)){
        QtPrivate::writeSequentialContainer(s, SetConstContainer{ptr, access.data()});
    }
}
void AutoHashAccess::dataStreamInSetFn(const QtPrivate::QMetaTypeInterface *iface, QDataStream &s, void *ptr){
    if(QSharedPointer<class AutoHashAccess> access = getHashAccess(iface)){
        SetContainer container(ptr, access.data());
        QtPrivate::readListBasedContainer(s, container);
    }
}

void AutoHashAccess::debugStream(QDebug &dbg, const void *ptr){
    QtPrivate::printAssociativeContainer(dbg, "QHash", ConstContainer{ptr, this});
}

void AutoHashAccess::dataStreamOut(QDataStream &s, const void *ptr){
    QtPrivate::writeAssociativeContainer(s, ConstContainer{ptr, this});
}

void AutoHashAccess::dataStreamIn(QDataStream &s, void *ptr){
    Container container(ptr, this);
    QtPrivate::readAssociativeContainer(s, container);
}

void* AutoHashAccess::constructContainer(JNIEnv*, void* result, const ContainerAndAccessInfo& container) {
    return constructContainer(result, container.container);
}

void* AutoHashAccess::constructContainer(void* result, void* container) {
    QHashData*& d = *reinterpret_cast<QHashData**>(container);
    result = new(result) const QHashData*(d);
    d = nullptr;
    return result;
}

std::pair<const QtPrivate::QMetaTypeInterface *,const QtPrivate::QMetaTypeInterface *> AutoHashAccess::metaTypes(){
    return {m_keyMetaType.iface(), m_valueMetaType.iface()};
}

AutoHashAccess::iterator AutoHashAccess::begin(const void* container){
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    QHashData* d = *map;
    return d ? d->begin(*this) : iterator(QHashData::iterator{*this});
}

AutoHashAccess::iterator AutoHashAccess::end(const void* container){
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    QHashData* d = *map;
    return d ? d->end(*this) : iterator(QHashData::iterator{*this});
}

AutoHashAccess::AutoHashAccess(const AutoHashAccess & other)
    :AbstractHashAccess(), AbstractNestedAssociativeAccess(),
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
      m_offset2(other.m_offset2),
      m_size(other.m_size),
      m_keyOwnerFunction(other.m_keyOwnerFunction),
      m_valueOwnerFunction(other.m_valueOwnerFunction),
      m_keyDataType(other.m_keyDataType),
      m_valueDataType(other.m_valueDataType)
{
}

AutoHashAccess::~AutoHashAccess(){
}

std::unique_ptr<AbstractHashAccess::KeyValueIterator> AutoHashAccess::keyValueIterator(const void* container) {
    class KeyValueIterator : public AbstractHashAccess::KeyValueIterator{
        AutoHashAccess* m_access;
        iterator current;
        iterator end;
        KeyValueIterator(const KeyValueIterator& other)
            :m_access(other.m_access),
            current(other.current),
            end(other.end) {}
    protected:
        AbstractAssociativeAccess* access() override {return m_access;}
    public:
        KeyValueIterator(AutoHashAccess* _access, QHashData* container)
            :m_access(_access),
            current(container ? container->begin(*_access) : iterator(*_access)), end(container ? container->end(*_access) : iterator(*_access))
        {}
        bool hasNext() override{
            return current!=end;
        }
        QPair<jobject,jobject> next(JNIEnv * env) override{
            jvalue k;
            k.l = nullptr;
            jvalue v;
            v.l = nullptr;
            m_access->m_keyInternalToExternalConverter(env, nullptr, current.key(), k, true);
            if(m_access->m_offset2!=0)
                m_access->m_valueInternalToExternalConverter(env, nullptr, current.value(), v, true);
            ++current;
            return {k.l, v.l};
        }
        QPair<const void*,const void*> next() override {
            void* key = current.key();
            void* value = current.value();
            ++current;
            if(m_access->m_keyDataType & AbstractContainerAccess::PointersMask){
                key = *reinterpret_cast<void**>(key);
            }
            if(m_access->m_valueDataType & AbstractContainerAccess::PointersMask){
                value = *reinterpret_cast<void**>(value);
            }
            return {key, value};
        }
        bool operator==(const AbstractMapAccess::KeyValueIterator& other) const override {
            return current==reinterpret_cast<const KeyValueIterator&>(other).current;
        }
        std::unique_ptr<AbstractAssociativeAccess::KeyValueIterator> clone() const override {
            return std::unique_ptr<AbstractAssociativeAccess::KeyValueIterator>(new KeyValueIterator(*this));
        }
        bool isConst() override{
            return true;
        }
        QPair<const void*,const void*> constNext() override {
            void* key = current.key();
            void* value = current.value();
            ++current;
            return {key, value};
        }
        QPair<const void*,void*> mutableNext() override {
            return {nullptr, nullptr};
        }
    };
    return std::unique_ptr<AbstractHashAccess::KeyValueIterator>(new KeyValueIterator(this, *reinterpret_cast<QHashData *const*>(container)));
}

std::unique_ptr<AbstractHashAccess::KeyValueIterator> AutoHashAccess::keyValueIterator(void* container) {
    class KeyValueIterator : public AbstractHashAccess::KeyValueIterator{
        AutoHashAccess* m_access;
        iterator current;
        iterator end;
        KeyValueIterator(const KeyValueIterator& other)
            :m_access(other.m_access),
            current(other.current),
            end(other.end) {}
    protected:
        AbstractAssociativeAccess* access() override {return m_access;}
    public:
        KeyValueIterator(AutoHashAccess* _access, QHashData* container)
            :m_access(_access),
            current(container ? container->begin(*_access) : iterator(*_access)), end(container ? container->end(*_access) : iterator(*_access))
        {}
        bool hasNext() override{
            return current!=end;
        }
        QPair<jobject,jobject> next(JNIEnv * env) override{
            jvalue k;
            k.l = nullptr;
            jvalue v;
            v.l = nullptr;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            m_access->m_keyInternalToExternalConverter(env, nullptr, reinterpret_cast<char*>(current)+m_access->m_offset1, k, true);
            if(m_access->m_offset2!=0)
                m_access->m_valueInternalToExternalConverter(env, nullptr, reinterpret_cast<char*>(current)+m_access->m_offset2, v, true);
            current = QHashData::nextNode(current);
#else
            m_access->m_keyInternalToExternalConverter(env, nullptr, current.key(), k, true);
            if(m_access->m_offset2!=0)
                m_access->m_valueInternalToExternalConverter(env, nullptr, current.value(), v, true);
            ++current;
#endif
            return {k.l, v.l};
        }
        QPair<const void*,const void*> next() override {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            void* key = reinterpret_cast<char*>(current)+m_access->m_offset1;
            void* value = reinterpret_cast<char*>(current)+m_access->m_offset2;
            current = QHashData::nextNode(current);
#else
            void* key = current.key();
            void* value = current.value();
            ++current;
#endif
            if(m_access->m_keyDataType & AbstractContainerAccess::PointersMask){
                key = *reinterpret_cast<void**>(key);
            }
            if(m_access->m_valueDataType & AbstractContainerAccess::PointersMask){
                value = *reinterpret_cast<void**>(value);
            }
            return {key, value};
        }
        bool operator==(const AbstractMapAccess::KeyValueIterator& other) const override {
            return current==reinterpret_cast<const KeyValueIterator&>(other).current;
        }
        std::unique_ptr<AbstractAssociativeAccess::KeyValueIterator> clone() const override {
            return std::unique_ptr<AbstractAssociativeAccess::KeyValueIterator>(new KeyValueIterator(*this));
        }
        bool isConst() override{
            return false;
        }
        QPair<const void*,const void*> constNext() override {
            void* key = current.key();
            void* value = current.value();
            ++current;
            return {key, value};
        }
        QPair<const void*,void*> mutableNext() override {
            void* key = current.key();
            void* value = current.value();
            ++current;
            return {key, value};
        }
    };
    return std::unique_ptr<AbstractHashAccess::KeyValueIterator>(new KeyValueIterator(this, *reinterpret_cast<QHashData **>(container)));
}

AutoHashAccess::AutoHashAccess(
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
    : AbstractHashAccess(), AbstractNestedAssociativeAccess(),
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
      m_align(m_valueMetaType.id()!=QMetaType::Void ? qMax(m_keyMetaType.alignOf(), m_valueMetaType.alignOf()) : m_keyMetaType.alignOf()),
      m_offset2(0),
      m_size(0),
      m_keyOwnerFunction(keyOwnerFunction),
      m_valueOwnerFunction(valueOwnerFunction),
      m_keyDataType(keyDataType),
      m_valueDataType(valueDataType)
{
    Q_ASSERT(m_keyInternalToExternalConverter);
    Q_ASSERT(m_keyExternalToInternalConverter);
    Q_ASSERT(m_keyMetaType.id()!=QMetaType::UnknownType
            && m_keyMetaType.id()!=QMetaType::Void);
    Q_ASSERT(m_valueMetaType.id()!=QMetaType::UnknownType);
    if(m_valueMetaType.id()!=QMetaType::Void){
        Q_ASSERT(m_valueInternalToExternalConverter);
        Q_ASSERT(m_valueExternalToInternalConverter);
        m_offset2 = m_keyMetaType.sizeOf();
        if(m_offset2%m_valueMetaType.alignOf()>0)
            m_offset2 += m_valueMetaType.alignOf()-m_offset2%m_valueMetaType.alignOf();
        m_size = m_offset2 + m_valueMetaType.sizeOf();
        if(m_size%m_align>0)
            m_size += m_align-m_size%m_align;
    }else{
        m_size = m_keyMetaType.sizeOf();
        if(m_size%m_align>0)
            m_size += m_align-m_size%m_align;
        m_offset2 = 0;
    }
}

void AutoHashAccess::dispose(){ delete this; }
AbstractHashAccess* AutoHashAccess::clone(){ return new AutoHashAccess(*this); }

const QObject* AutoHashAccess::getOwner(const void* container){
    if(hasOwnerFunction() && size(container)>0){
        auto iter = constKeyValueIterator(container);
        while(iter->hasNext()){
            auto current = iter->next();
            if(m_keyOwnerFunction){
                if(const QObject* owner = m_keyOwnerFunction(current.first))
                    return owner;
            }else if(m_valueOwnerFunction){
                if(const QObject* owner = m_valueOwnerFunction(current.second))
                    return owner;
            }else if(m_keyNestedContainerAccess){
                if(const QObject* owner = m_keyNestedContainerAccess->getOwner(current.first))
                    return owner;
            }else if(m_valueNestedContainerAccess){
                if(const QObject* owner = m_valueNestedContainerAccess->getOwner(current.second))
                    return owner;
            }
        }
    }
    return nullptr;
}

bool AutoHashAccess::hasOwnerFunction(){
    if(m_keyOwnerFunction && !(keyType() & PointersMask))
        return true;
    if(m_valueOwnerFunction && !(valueType() & PointersMask))
        return true;
    if(!(keyType() & PointersMask) && m_keyNestedContainerAccess && m_keyNestedContainerAccess->hasOwnerFunction())
        return true;
    if(!(valueType() & PointersMask) && m_valueNestedContainerAccess && m_valueNestedContainerAccess->hasOwnerFunction())
        return true;
    return false;
}

const QMetaType& AutoHashAccess::keyMetaType(){ return m_keyMetaType; }
const QMetaType& AutoHashAccess::valueMetaType(){ return m_valueMetaType; }

AbstractContainerAccess::DataType AutoHashAccess::keyType() {return m_keyDataType;}
AbstractContainerAccess::DataType AutoHashAccess::valueType() {return m_valueDataType;}

AbstractContainerAccess* AutoHashAccess::keyNestedContainerAccess() {
    return m_keyNestedContainerAccess ? m_keyNestedContainerAccess->clone() : nullptr;
}

AbstractContainerAccess* AutoHashAccess::valueNestedContainerAccess() {
    return m_valueNestedContainerAccess ? m_valueNestedContainerAccess->clone() : nullptr;
}

const QSharedPointer<AbstractContainerAccess>& AutoHashAccess::sharedKeyNestedContainerAccess(){
    return m_keyNestedContainerAccess;
}
const QSharedPointer<AbstractContainerAccess>& AutoHashAccess::sharedValueNestedContainerAccess(){
    return m_valueNestedContainerAccess;
}
bool AutoHashAccess::hasKeyNestedContainerAccess() {
    return !m_keyNestedContainerAccess.isNull();
}
bool AutoHashAccess::hasKeyNestedPointers() {
    if(hasKeyNestedContainerAccess()){
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
bool AutoHashAccess::hasValueNestedContainerAccess() {
    return !m_valueNestedContainerAccess.isNull();
}
bool AutoHashAccess::hasValueNestedPointers() {
    if(hasValueNestedContainerAccess()){
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

jint AutoHashAccess::size(JNIEnv *,const void* container){
    return jint(size(container));
}

qsizetype AutoHashAccess::size(const void* container){
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    return map && *map ? (*map)->size : 0;
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
    return isEquals(m_keyMetaType, key, key0);
}

bool AutoHashAccess::same_key(Node * node, uint h0, const void* key0){
    if(node->h==h0){
        void* key = reinterpret_cast<char*>(node)+m_offset1;
        return isEquals(m_keyMetaType, key, key0);
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
        m_keyInternalToExternalConverter(env, nullptr, reinterpret_cast<char*>(node)+m_offset1, jv, true);
    return jv.l;
}

jobject AutoHashAccess::nodeValue(JNIEnv * env, Node* node)
{
    jvalue jv;
    jv.l = nullptr;
    if(node)
        m_valueInternalToExternalConverter(env, nullptr, reinterpret_cast<char*>(node)+m_offset2, jv, true);
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
        while (node->next != e && isEquals(m_keyMetaType, nextKey, akey)){
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
    return !access->m_offset2 || isEquals(access->m_valueMetaType, reinterpret_cast<char*>(i) + access->m_offset2,  reinterpret_cast<char*>(o.i) + access->m_offset2);
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
#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
    seed = qGlobalQHashSeed();
#else
    seed = QHashSeed::globalSeed();
#endif
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

char* AutoHashAccess::iteratorValue(const iterator& it) const{
    return it.i.value();
}

AutoHashAccess::iterator& AutoHashAccess::incrementIterator(iterator& it) const{
    ++it.i;
    return it;
}

void AutoHashAccess::insert(void* container, const void* key, const void* value){
    QHashData ** map = reinterpret_cast<QHashData **>(container);
    detach(map);
    emplace(container, key, value);
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

void AutoHashAccess::insert(JNIEnv *env, const ContainerInfo& container, jobject key, jobject value){
    QHashData ** map = reinterpret_cast<QHashData **>(container.container);
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
        emplace(container.container, akey, env, value);
#endif
    }
}

bool AutoHashAccess::contains(const void* container, const void* key)
{
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    QHashData* d = *map;
    if(d && d->size>0){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        Node* e = reinterpret_cast<Node*>(d);
        return *findNode(map, key) != e;
#else
        return d->findNode(*this, key) != nullptr;
#endif
    }
    return false;
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

jobject AutoHashAccess::begin(JNIEnv * env, const ExtendedContainerInfo& container)
{
    QHashData ** map = reinterpret_cast<QHashData **>(container.container);
    detach(map);
    QHashData* d = *map;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return createIterator(env, container.nativeId, new void*(d->firstNode()));
#else
    return createIterator(env, container.nativeId, d ? new iterator(d->begin(*this)) : new iterator(*this));
#endif
}

jobject AutoHashAccess::end(JNIEnv * env, const ExtendedContainerInfo& container)
{
    QHashData ** map = reinterpret_cast<QHashData **>(container.container);
    detach(map);
    QHashData* d = *map;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    Node* e = reinterpret_cast<Node*>(d);
    return createIterator(env, container.nativeId, new void*(e));
#else
    return createIterator(env, container.nativeId, d ? new iterator(d->end(*this)) : new iterator(*this));
#endif
}

jobject AutoHashAccess::constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container)
{
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container.container);
    QHashData* d = *map;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return createConstIterator(env, container.nativeId, new void*(d->firstNode()));
#else
    return createConstIterator(env, container.nativeId, d ? new iterator(d->begin(*this)) : new iterator(*this));
#endif
}

jobject AutoHashAccess::constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container)
{
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container.container);
    QHashData* d = *map;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    Node* e = reinterpret_cast<Node*>(d);
    return createConstIterator(env, container.nativeId, new void*(e));
#else
    return createConstIterator(env, container.nativeId, d ? new iterator(d->end(*this)) : new iterator(*this));
#endif
}

void AutoHashAccess::clear(JNIEnv *, const ContainerInfo& container)
{
    clear(container.container);
}

bool AutoHashAccess::isDetached(const void* container){
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    return map && *map && !(*map)->ref.isShared();
}

void AutoHashAccess::detach(const ContainerInfo& container){
    QHashData ** map = reinterpret_cast<QHashData **>(container.container);
    detach(map);
}

bool AutoHashAccess::isSharedWith(const void* container, const void* container2){
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    QHashData *const* map2 = reinterpret_cast<QHashData *const*>(container2);
    if(map && map2){
        return (*map)==(*map2);
    }
    return false;
}

void AutoHashAccess::swap(JNIEnv *, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    QHashData *& map = *reinterpret_cast<QHashData **>(container.container);
    QHashData *& map2 = *reinterpret_cast<QHashData **>(container2.container);
#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
    qSwap(map, map2);
#else
    qt_ptr_swap(map, map2);
#endif
}

void AutoHashAccess::clear(void* container)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    const QHashData* empty = &QHashData::shared_null;
#else
    const QHashData* empty = nullptr;
#endif
    assign(container, &empty);
}

void* AutoHashAccess::constructContainer(JNIEnv*, void* result, const ConstContainerAndAccessInfo& container) {
    return constructContainer(result, container.container);
}

void* AutoHashAccess::constructContainer(void* result){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return new(result) const QHashData*(&QHashData::shared_null);
#else
    return new(result) const QHashData*(nullptr);
#endif
}

void* AutoHashAccess::constructContainer(void* result, const void* container) {
    result = constructContainer(result);
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

jobject AutoHashAccess::find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key)
{
    QHashData ** map = reinterpret_cast<QHashData **>(container.container);
    detach(map);
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
    return createIterator(env, container.nativeId, new void*(n ? n : e));
#else
    if (d && d->size>0){
        jvalue jv;
        jv.l = key;
        QtJambiScope scope;
        void* akey = nullptr;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            auto it = d->find(*this, akey);
            if (it.isUnused())
                it = d->end(*this);
            return createIterator(env, container.nativeId, new iterator(it));
        }
    }
    return createIterator(env, container.nativeId, new iterator(*this));
#endif
}

jobject AutoHashAccess::constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key)
{
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container.container);
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
    return createConstIterator(env, container.nativeId, new void*(n ? n : e));
#else
    if (d && d->size>0){
        jvalue jv;
        jv.l = key;
        QtJambiScope scope;
        void* akey = nullptr;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            auto it = d->find(*this, akey);
            if (it.isUnused())
                it = d->end(*this);
            return createConstIterator(env, container.nativeId, new iterator(it));
        }
    }
    return createConstIterator(env, container.nativeId, new iterator(*this));
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
            if(isEquals(m_valueMetaType, reinterpret_cast<char*>(n)+m_offset2, avalue)){
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
                m_keyInternalToExternalConverter(env, nullptr, i.key(), jv, true);
                return jv.l;
            }
            ++i;
        }
#endif
    }
    return defaultKey;
}

const void* AutoHashAccess::value(const void* container, const void* key, const void* defaultValue)
{
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    QHashData* d = *map;
    if(!d || d->size==0)
        return defaultValue;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    Node* e = reinterpret_cast<Node*>(d);
    Node *n = *findNode(map, key);
    return n!=e ? reinterpret_cast<char*>(n)+m_offset2 : defaultValue;
#else
    QHashData::iterator iter = d->find(*this, key);
    if (!iter.isUnused()){
        return iterator(iter).value();
    }
#endif
    return defaultValue;
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
            m_valueInternalToExternalConverter(env, nullptr, iterator(iter).value(), jv, true);
            return jv.l;
        }
#endif
    }
    return defaultValue;
}

void AutoHashAccess::assign(JNIEnv *, const ContainerInfo& container, const ConstContainerAndAccessInfo& other){
    assign(container.container, other.container);
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

IsBiContainerFunction AutoHashAccess::getIsBiContainerFunction(){
    return ContainerAPI::getAsQHash;
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
    if(!d && d2)
        return d2->size==0;
    if(!d2 && d)
        return d->size==0;
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
        } while (it != e && isEquals(m_keyMetaType, thisEqualRangeKey, otherEqualRangeKey));

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
    void* ptr{nullptr};
    bool deleteSet = false;
    if (!(*getIsBiContainerFunction())(env, other, keyMetaType(), valueMetaType(), ptr)) {
        deleteSet = true;
        ptr = operator new(sizeOf());
        constructContainer(ptr);
        jobject iterator = QtJambiAPI::entrySetIteratorOfJavaMap(env, other);
        while(QtJambiAPI::hasJavaIteratorNext(env, iterator)){
            jobject entry = QtJambiAPI::nextOfJavaIterator(env, iterator);
            insert(env, ContainerInfo{nullptr, ptr}, QtJambiAPI::keyOfJavaMapEntry(env, entry), QtJambiAPI::valueOfJavaMapEntry(env, entry));
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
    int newMetaType = QMetaType::fromName(typeName).id();
    if(newMetaType==QMetaType::UnknownType){
        QSharedPointer<AutoHashAccess> access(dynamic_cast<AutoHashAccess*>(this->clone()), &containerDisposer);
        auto kiface = m_keyMetaType.iface();
        auto viface = m_valueMetaType.iface();
        newMetaType = registerContainerMetaType(typeName,
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
        if(m_keyHashFunction && m_valueHashFunction){
            insertHashFunctionByMetaType(newMetaType,
                                            [access]
                                            (const void* ptr, size_t seed)->size_t{
                                                if(ptr){
                                                    QHashData *const* map = reinterpret_cast<QHashData *const*>(ptr);
                                                    QHashData* d = *map;
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
                                                    return hashCode;
                                                }else{
                                                    return 0;
                                                }
                                            });
        }

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
    }else{
        registerContainerAccess(newMetaType, this);
    }
    return newMetaType;
}

bool AutoHashAccess::isMulti() const{
    return false;
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)

QtMetaContainerPrivate::QMetaAssociationInterface* AutoHashAccess::createMetaAssociationInterface(int newMetaType){
    {
        QReadLocker locker(containerAccessLock());
        Q_UNUSED(locker)
        if(metaAssociationHash().contains(newMetaType))
            return &metaAssociationHash()[newMetaType];
    }
    using namespace QtMetaContainerPrivate;
    QMetaAssociationInterface* metaAssociationInterface;
    {
        QWriteLocker locker(containerAccessLock());
        metaAssociationInterface = &metaAssociationHash()[newMetaType];
    }
    QSharedPointer<class AutoHashAccess> access = getHashAccess(QMetaType(newMetaType).iface());
    metaAssociationInterface->keyMetaType = access->m_keyMetaType.iface();
    metaAssociationInterface->mappedMetaType = access->m_valueMetaType.iface();
    metaAssociationInterface->iteratorCapabilities = InputCapability | ForwardCapability;
    if(access->isMulti()){
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
        access->clear(c);
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
                        if(d){
                            switch (p) {
                            case QMetaContainerInterface::AtBegin:
                                return new iterator(d->begin(*access));
                            default:
                                return new iterator(d->end(*access));
                            }
                        }
                        return new iterator(*access);
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
    metaAssociationInterface->mappedAtKeyFn = access->isMulti() ? QMetaAssociationInterface::MappedAtKeyFn(nullptr) : qtjambi_function_pointer<16,void(const void *, const void *, void *)>([newMetaType](const void *c, const void *k, void *r) {
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
    metaAssociationInterface->setMappedAtKeyFn = access->isMulti() ? QMetaAssociationInterface::SetMappedAtKeyFn(nullptr) : qtjambi_function_pointer<16,void(void *, const void *, const void *)>([newMetaType](void *c, const void *k, const void *r) {
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
    metaAssociationInterface->setMappedAtIteratorFn = [](const void *i, const void *v) {
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


ContainerAndAccessInfo AutoHashAccess::keys(JNIEnv *env, const ConstContainerInfo& container)
{
    ContainerAndAccessInfo result;
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container.container);
    QHashData* d = *map;
    AbstractListAccess* listAccess = dynamic_cast<AbstractListAccess*>(ContainerAccessAPI::createContainerAccess(SequentialContainerType::QList, m_keyMetaType));
    if(!listAccess)
        listAccess = dynamic_cast<AbstractListAccess*>(ContainerAccessAPI::createContainerAccess(
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
                                                                           m_keyExternalToInternalConverter,
                                                                           m_keyNestedContainerAccess,
                                                                           m_keyOwnerFunction
                                                                           ));
    if(listAccess){
        CHECK_CONTAINER_ACCESS(env, listAccess)
        result.container = listAccess->createContainer();
        result.object = ContainerAPI::objectFromQList(env, result.container, listAccess);
        result.access = listAccess;
        jint idx = listAccess->size(env, result.container);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        Node* e = reinterpret_cast<Node*>(d);
        Node *n = d->firstNode();
        listAccess->reserve(env, result, d->size);
        while(n && n != e){
            if(listAccess->append(result.container, reinterpret_cast<char*>(n)+m_offset1)){
                idx++;
            }else{
                listAccess->insert(env, result, idx++, 1, nodeKey(env, n));
            }
            n = QHashData::nextNode(n);
        }
#else
        iterator e = d->end(*this);
        iterator n = d->begin(*this);
        while (n != e) {
            if(listAccess->append(result.container, n.key())){
                idx++;
            }else{
                jvalue jv;
                jv.l = nullptr;
                m_keyInternalToExternalConverter(env, nullptr, n.key(), jv, true);
                listAccess->insert(env, result, idx++, 1, jv.l);
            }
            ++n;
        }
#endif
    }
    return result;
}

ContainerAndAccessInfo AutoHashAccess::keys(JNIEnv *env, const ConstContainerInfo& container, jobject value)
{
    ContainerAndAccessInfo result;
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container.container);
    QHashData* d = *map;
    AbstractListAccess* listAccess = dynamic_cast<AbstractListAccess*>(ContainerAccessAPI::createContainerAccess(SequentialContainerType::QList, m_keyMetaType));
    if(!listAccess)
        listAccess = dynamic_cast<AbstractListAccess*>(ContainerAccessAPI::createContainerAccess(
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
                                                                           m_keyExternalToInternalConverter,
                                                                           m_keyNestedContainerAccess,
                                                                           m_keyOwnerFunction
                                                                           ));
    if(listAccess){
        CHECK_CONTAINER_ACCESS(env, listAccess)
        result.container = listAccess->createContainer();
        result.object = ContainerAPI::objectFromQList(env, result.container, listAccess);
        result.access = listAccess;
        QtJambiScope scope;
        jvalue _value;
        void *_qvaluePtr;
        _value.l = value;
        _qvaluePtr = nullptr;
        jint idx = listAccess->size(env, result.container);
        if(d && d->size>0 && m_valueExternalToInternalConverter(env, &scope, _value, _qvaluePtr, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            Node* e = reinterpret_cast<Node*>(d);
            Node *n = d->firstNode();
            while(n != e){
                void* value = reinterpret_cast<char*>(n)+m_offset2;
                if(isEquals(m_valueMetaType, value, _qvaluePtr)){
                    if(listAccess->append(result.container, reinterpret_cast<char*>(n)+m_offset1)){
                        idx++;
                    }else{
                        listAccess->insert(env, result, idx++, 1, nodeKey(env, n));
                    }
                }
                n = QHashData::nextNode(n);
            }
#else
        iterator e = d->end(*this);
        iterator n = d->begin(*this);
        while (n != e) {
            if(m_valueMetaType.equals(n.value(), _qvaluePtr)){
                if(listAccess->append(result.container, n.key())){
                    idx++;
                }else{
                    jvalue jv;
                    jv.l = nullptr;
                    m_keyInternalToExternalConverter(env, nullptr, n.key(), jv, true);
                    listAccess->insert(env, result, idx++, 1, jv.l);
                }
            }
            ++n;
        }
#endif
        }
    }
    return result;
}

jint AutoHashAccess::remove(JNIEnv *env, const ContainerInfo& container, jobject key)
{
    QHashData ** map = reinterpret_cast<QHashData **>(container.container);
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
                deleteNext = (next != e && isEquals(m_keyMetaType, key, reinterpret_cast<char*>(next)+m_offset1));
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

jobject AutoHashAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key)
{
    QHashData ** map = reinterpret_cast<QHashData **>(container.container);
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
            m_valueInternalToExternalConverter(env, nullptr, it.value(), jv, true);
            d->erase(*this, it);
            return jv.l;
        }
#endif
    }
    return nullptr;
}

ContainerAndAccessInfo AutoHashAccess::values(JNIEnv *env, const ConstContainerInfo& container)
{
    ContainerAndAccessInfo result;
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container.container);
    AbstractListAccess* listAccess = dynamic_cast<AbstractListAccess*>(ContainerAccessAPI::createContainerAccess(SequentialContainerType::QList, m_valueMetaType));
    if(!listAccess)
        listAccess = dynamic_cast<AbstractListAccess*>(ContainerAccessAPI::createContainerAccess(
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
                                                                           m_valueExternalToInternalConverter,
                                                                           m_valueNestedContainerAccess,
                                                                           m_valueOwnerFunction
                                                                           ));
    if(listAccess){
        CHECK_CONTAINER_ACCESS(env, listAccess)
        result.container = listAccess->createContainer();
        result.object = ContainerAPI::objectFromQList(env, result.container, listAccess);
        result.access = listAccess;
        QHashData* d = *map;
        jint idx = listAccess->size(env, result.container);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        Node *firstNode = d->firstNode();
        Node *lastNode = reinterpret_cast<Node*>(d);
        while(firstNode != lastNode){
            if(listAccess->append(result.container, reinterpret_cast<char*>(firstNode)+m_offset2)){
                idx++;
            }else{
                listAccess->insert(env, result, idx++, 1, nodeValue(env, firstNode));
            }
            firstNode = QHashData::nextNode(firstNode);
        }
#else
        iterator e = d->end(*this);
        iterator n = d->begin(*this);
        while (n != e) {
            if(listAccess->append(result.container, n.value())){
                idx++;
            }else{
                jvalue jv;
                jv.l = nullptr;
                m_valueInternalToExternalConverter(env, nullptr, n.value(), jv, true);
                listAccess->insert(env, result, idx++, 1, jv.l);
            }
            ++n;
        }
#endif
    }
    return result;
}

jobject AutoHashAccess::createConstIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr)
{
    if(m_offset2){
        AbstractAssociativeConstIteratorAccess* containerAccess = new AutoAssociativeConstIteratorAccess(m_valueInternalToExternalConverter,
                                                                         [](AutoAssociativeConstIteratorAccess*, void*ptr){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                            Node* cursor = *reinterpret_cast<Node**>(ptr);
                                                                            *reinterpret_cast<Node**>(ptr) = QHashData::nextNode(cursor);
#else
                                                                            iterator& cursor = *reinterpret_cast<iterator*>(ptr);
                                                                            ++cursor;
#endif
                                                                         },
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         [](AutoAssociativeConstIteratorAccess*, void*ptr){
                                                                            Node* cursor = *reinterpret_cast<Node**>(ptr);
                                                                            *reinterpret_cast<Node**>(ptr) = QHashData::previousNode(cursor);
                                                                         },
#else
                                                                         nullptr,
#endif
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                          [](AutoAssociativeConstIteratorAccess* access, const void*ptr)->const void*{
                                                                            const char* cursor = *reinterpret_cast<char*const*>(ptr);
                                                                            return cursor+access->valueOffset();
                                                                          },
#else
                                                                          [](AutoAssociativeConstIteratorAccess*, const void*ptr)->const void*{
                                                                            const iterator& cursor = *reinterpret_cast<const iterator*>(ptr);
                                                                            return cursor.value();
                                                                          },
#endif
                                                                         nullptr, // no less
                                                                         [](AutoAssociativeConstIteratorAccess*, const void*ptr1,const void*ptr2)->bool{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                            return *reinterpret_cast<Node*const*>(ptr1)==*reinterpret_cast<Node*const*>(ptr2);
#else
                                                                            return *reinterpret_cast<const iterator*>(ptr1)==*reinterpret_cast<const iterator*>(ptr2);
#endif
                                                                         },
                                                                         m_keyInternalToExternalConverter,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         [](AutoAssociativeConstIteratorAccess* access, const void*ptr)->const void*{
                                                                            char* cursor = *reinterpret_cast<char*const*>(ptr);
                                                                            return cursor+access->keyOffset();
                                                                         },
#else
                                                                        [](AutoAssociativeConstIteratorAccess*, const void*ptr)->const void*{
                                                                           const iterator& cursor = *reinterpret_cast<const iterator*>(ptr);
                                                                           return cursor.key();
                                                                        },
#endif
                                                                        m_keyMetaType,
                                                                        m_valueMetaType,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                        m_offset1, m_offset2
#else
                                                                        /*offsets not required*/ 0, 0
#endif
                                                );
        return QtJambiAPI::convertQAssociativeIteratorToJavaObject(env, ownerId, iteratorPtr, [](void* ptr,bool){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            delete reinterpret_cast<void**>(ptr);
#else
            delete reinterpret_cast<QHashData::iterator*>(ptr);
#endif
        }, containerAccess);
    }else{
        AutoSequentialConstIteratorAccess* containerAccess = new AutoSequentialConstIteratorAccess(m_keyInternalToExternalConverter,
                                                                         [](AutoSequentialConstIteratorAccess*, void*ptr){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                            Node* cursor = *reinterpret_cast<Node**>(ptr);
                                                                            *reinterpret_cast<Node**>(ptr) = QHashData::nextNode(cursor);
#else
                                                                            iterator& cursor = *reinterpret_cast<iterator*>(ptr);
                                                                            ++cursor;
#endif
                                                                         },
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         [](AutoSequentialConstIteratorAccess*, void*ptr){
                                                                            Node* cursor = *reinterpret_cast<Node**>(ptr);
                                                                            *reinterpret_cast<Node**>(ptr) = QHashData::previousNode(cursor);
                                                                         },
#else
                                                                         nullptr,
#endif
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         [](AutoSequentialConstIteratorAccess* access, const void*ptr)->const void*{
                                                                            const char* cursor = *reinterpret_cast<char*const*>(ptr);
                                                                            return cursor+access->offset();
                                                                         },
#else
                                                                         [](AutoSequentialConstIteratorAccess*, const void*ptr)->const void*{
                                                                               const iterator& cursor = *reinterpret_cast<const iterator*>(ptr);
                                                                               return cursor.key();
                                                                         },
#endif
                                                                         nullptr, // no less
                                                                         [](AutoSequentialConstIteratorAccess*, const void*ptr1,const void*ptr2)->bool{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                            return *reinterpret_cast<Node*const*>(ptr1)==*reinterpret_cast<Node*const*>(ptr2);
#else
                                                                            return *reinterpret_cast<const iterator*>(ptr1)==*reinterpret_cast<const iterator*>(ptr2);
#endif
                                                                         },
                                                                         m_valueMetaType,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         m_offset1
#else
                                                                         /*offset not required*/ 0
#endif
                                                );
        return QtJambiAPI::convertQSequentialIteratorToJavaObject(env, ownerId, iteratorPtr, [](void* ptr,bool){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            delete reinterpret_cast<void**>(ptr);
#else
            delete reinterpret_cast<iterator*>(ptr);
#endif
        }, containerAccess);
    }
}

jobject AutoHashAccess::createIterator(JNIEnv * env, QtJambiNativeID ownerId, void* iteratorPtr)
{
    if(m_offset2){
        AbstractAssociativeIteratorAccess* containerAccess = new AutoAssociativeIteratorAccess(m_valueInternalToExternalConverter,
                                                                         [](AutoAssociativeIteratorAccess*, void*ptr){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                            Node* cursor = *reinterpret_cast<Node**>(ptr);
                                                                            *reinterpret_cast<Node**>(ptr) = QHashData::nextNode(cursor);
#else
                                                                            iterator& cursor = *reinterpret_cast<iterator*>(ptr);
                                                                            ++cursor;
#endif
                                                                         },
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         [](AutoAssociativeIteratorAccess*, void*ptr){
                                                                            Node* cursor = *reinterpret_cast<Node**>(ptr);
                                                                            *reinterpret_cast<Node**>(ptr) = QHashData::previousNode(cursor);
                                                                         },
#else
                                                                         nullptr,
#endif
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                          [](AutoAssociativeIteratorAccess* access, const void*ptr)->const void*{
                                                                            const char* cursor = *reinterpret_cast<char*const*>(ptr);
                                                                            return cursor+access->valueOffset();
                                                                          },
#else
                                                                          [](AutoAssociativeIteratorAccess*, const void*ptr)->const void*{
                                                                            const iterator& cursor = *reinterpret_cast<const iterator*>(ptr);
                                                                            return cursor.value();
                                                                          },
#endif
                                                                         nullptr, // no less
                                                                         [](AutoAssociativeIteratorAccess*, const void*ptr1,const void*ptr2)->bool{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                            return *reinterpret_cast<Node*const*>(ptr1)==*reinterpret_cast<Node*const*>(ptr2);
#else
                                                                            return *reinterpret_cast<const iterator*>(ptr1)==*reinterpret_cast<const iterator*>(ptr2);
#endif
                                                                         },
                                                                         m_keyInternalToExternalConverter,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         [](AutoAssociativeIteratorAccess* access, const void*ptr)->const void*{
                                                                            char* cursor = *reinterpret_cast<char*const*>(ptr);
                                                                            return cursor+access->keyOffset();
                                                                         },
#else
                                                                        [](AutoAssociativeIteratorAccess*, const void*ptr)->const void*{
                                                                           const iterator& cursor = *reinterpret_cast<const iterator*>(ptr);
                                                                           return cursor.key();
                                                                        },
#endif
                                                                        m_valueExternalToInternalConverter,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                          [](AutoAssociativeIteratorAccess* access, void*ptr)->void*{
                                                                            char* cursor = *reinterpret_cast<char**>(ptr);
                                                                            return cursor+access->valueOffset();
                                                                          },
#else
                                                                          [](AutoAssociativeIteratorAccess*, void*ptr)->void*{
                                                                            iterator& cursor = *reinterpret_cast<iterator*>(ptr);
                                                                            return cursor.value();
                                                                          },
#endif
                                                                          m_keyMetaType,
                                                                          m_valueMetaType,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                          m_offset1, m_offset2
#else
                                                                          /*offsets not required*/ 0, 0
#endif
                                                );
        return QtJambiAPI::convertQAssociativeIteratorToJavaObject(env, ownerId, iteratorPtr, [](void* ptr,bool){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            delete reinterpret_cast<void**>(ptr);
#else
            delete reinterpret_cast<QHashData::iterator*>(ptr);
#endif
        }, containerAccess);
    }else{
        AbstractSequentialIteratorAccess* containerAccess = new AutoSequentialIteratorAccess(m_keyInternalToExternalConverter,
                                                                         [](AutoSequentialIteratorAccess*, void*ptr){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                            Node* cursor = *reinterpret_cast<Node**>(ptr);
                                                                            *reinterpret_cast<Node**>(ptr) = QHashData::nextNode(cursor);
#else
                                                                            iterator& cursor = *reinterpret_cast<iterator*>(ptr);
                                                                            ++cursor;
#endif
                                                                         },
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         [](AutoSequentialIteratorAccess*, void*ptr){
                                                                            Node* cursor = *reinterpret_cast<Node**>(ptr);
                                                                            *reinterpret_cast<Node**>(ptr) = QHashData::previousNode(cursor);
                                                                         },
#else
                                                                         nullptr,
#endif
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         [](AutoSequentialIteratorAccess* access, const void*ptr)->const void*{
                                                                            const char* cursor = *reinterpret_cast<char*const*>(ptr);
                                                                            return cursor+access->offset();
                                                                         },
#else
                                                                         [](AutoSequentialIteratorAccess*, const void*ptr)->const void*{
                                                                               const iterator& cursor = *reinterpret_cast<const iterator*>(ptr);
                                                                               return cursor.key();
                                                                         },
#endif
                                                                         nullptr, // no less
                                                                         [](AutoSequentialIteratorAccess*, const void*ptr1,const void*ptr2)->bool{
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                            return *reinterpret_cast<Node*const*>(ptr1)==*reinterpret_cast<Node*const*>(ptr2);
#else
                                                                            return *reinterpret_cast<const iterator*>(ptr1)==*reinterpret_cast<const iterator*>(ptr2);
#endif
                                                                         },
                                                                         m_valueExternalToInternalConverter,

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         [](AutoSequentialIteratorAccess* access, void*ptr)->void*{
                                                                            char* cursor = *reinterpret_cast<char**>(ptr);
                                                                            return cursor+access->offset();
                                                                         },
#else
                                                                         [](AutoSequentialIteratorAccess*, void*ptr)->void*{
                                                                               iterator& cursor = *reinterpret_cast<iterator*>(ptr);
                                                                               return cursor.key();
                                                                         },
#endif
                                                                         m_valueMetaType,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                         m_offset1
#else
                                                                         /*offset not required*/ 0
#endif
                                                );
        return QtJambiAPI::convertQSequentialIteratorToJavaObject(env, ownerId, iteratorPtr, [](void* ptr,bool){
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
void AutoHashAccess::reserve(JNIEnv *, const ContainerInfo& container, jint asize){
    QHashData ** map = reinterpret_cast<QHashData **>(container.container);
    QHashData*& d = *map;
    if (isDetached(container.container))
        d->rehash(*this, size_t(asize));
    else
        d = QHashData::detached(*this, d, size_t(asize));
}

void AutoHashAccess::eraseSpanEntry(char* value) const{
    m_valueMetaType.destruct(value);
}

void AutoHashAccess::copySpanEntry(char* value1, const char* value2) const{
    m_valueMetaType.construct(value1, value2);
}

bool AutoHashAccess::equalSpanEntries(const char* value1, const char* value2) const{
    return m_valueMetaType.equals(value1, value2);
}

KeyPointerRCAutoHashAccess::KeyPointerRCAutoHashAccess(KeyPointerRCAutoHashAccess& other)
    : AutoHashAccess(other), ReferenceCountingSetContainer() {}

KeyPointerRCAutoHashAccess* KeyPointerRCAutoHashAccess::clone(){
    return new KeyPointerRCAutoHashAccess(*this);
}

void KeyPointerRCAutoHashAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject set = Java::Runtime::HashSet::newInstance(env);
    auto iterator = constKeyValueIterator(container.container);
    while(iterator->hasNext()){
        auto content = iterator->next();
        jobject obj{nullptr};
        switch(keyType()){
        case PointerToQObject:
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(reinterpret_cast<const QObject*>(content.first))){
                obj = link->getJavaObjectLocalRef(env);
            }
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(keyMetaType())){
                if(FunctionalResolver resolver = registeredFunctionalResolver(*typeId)){
                    bool success = false;
                    obj = resolver(env, content.first, &success);
                    break;
                }
            }
            Q_FALLTHROUGH();
        case Pointer:
            for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(content.first)){
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

void KeyPointerRCAutoHashAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    AutoHashAccess::swap(env, container, container2);
    if(KeyPointerRCAutoHashAccess* access = dynamic_cast<KeyPointerRCAutoHashAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void KeyPointerRCAutoHashAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    AutoHashAccess::assign(env, container, container2);
    if(KeyPointerRCAutoHashAccess* access = dynamic_cast<KeyPointerRCAutoHashAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void KeyPointerRCAutoHashAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    AutoHashAccess::clear(env, container);
    clearRC(env, container.object);
}

void KeyPointerRCAutoHashAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    AutoHashAccess::insert(env, container, key, value);
    addUniqueRC(env, container.object, key);
}

jint KeyPointerRCAutoHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    jint result = AutoHashAccess::remove(env, container, key);
    removeRC(env, container.object, key, result);
    return result;
}

jobject KeyPointerRCAutoHashAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = AutoHashAccess::take(env, container, key);
    removeRC(env, container.object, key);
    return result;
}

ValuePointerRCAutoHashAccess::ValuePointerRCAutoHashAccess(ValuePointerRCAutoHashAccess& other)
    : AutoHashAccess(other), ReferenceCountingSetContainer() {}

ValuePointerRCAutoHashAccess* ValuePointerRCAutoHashAccess::clone(){
    return new ValuePointerRCAutoHashAccess(*this);
}

void ValuePointerRCAutoHashAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject set = Java::Runtime::HashSet::newInstance(env);
    auto iterator = constKeyValueIterator(container.container);
    while(iterator->hasNext()){
        auto content = iterator->next();
        jobject obj{nullptr};
        switch(valueType()){
        case PointerToQObject:
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(reinterpret_cast<const QObject*>(content.second))){
                obj = link->getJavaObjectLocalRef(env);
            }
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(valueMetaType())){
                if(FunctionalResolver resolver = registeredFunctionalResolver(*typeId)){
                    bool success = false;
                    obj = resolver(env, content.second, &success);
                    break;
                }
            }
            Q_FALLTHROUGH();
        case Pointer:
            for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(content.second)){
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

void ValuePointerRCAutoHashAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    AutoHashAccess::swap(env, container, container2);
    if(ValuePointerRCAutoHashAccess* access = dynamic_cast<ValuePointerRCAutoHashAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void ValuePointerRCAutoHashAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    AutoHashAccess::assign(env, container, container2);
    if(ValuePointerRCAutoHashAccess* access = dynamic_cast<ValuePointerRCAutoHashAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void ValuePointerRCAutoHashAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    AutoHashAccess::clear(env, container);
    clearRC(env, container.object);
}

void ValuePointerRCAutoHashAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    jobject oldValue = AutoHashAccess::value(env, container.container, key, nullptr);
    AutoHashAccess::insert(env, container, key, value);
    removeRC(env, container.object, oldValue);
    addRC(env, container.object, value);
}

jint ValuePointerRCAutoHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    jobject oldValue = AutoHashAccess::value(env, container.container, key, nullptr);
    jint result = AutoHashAccess::remove(env, container, key);
    if(result>0){
        removeRC(env, container.object, oldValue);
    }
    return result;
}

jobject ValuePointerRCAutoHashAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = AutoHashAccess::take(env, container, key);
    removeRC(env, container.object, result);
    return result;
}

PointersRCAutoHashAccess::PointersRCAutoHashAccess(PointersRCAutoHashAccess& other)
    : AutoHashAccess(other), ReferenceCountingMapContainer(other) {}

PointersRCAutoHashAccess* PointersRCAutoHashAccess::clone(){
    return new PointersRCAutoHashAccess(*this);
}

void PointersRCAutoHashAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject map = Java::QtJambi::ReferenceUtility$RCMap::newInstance(env);
    auto iterator = constKeyValueIterator(container.container);
    while(iterator->hasNext()){
        auto content = iterator->next();
        jobject key{nullptr};
        switch(keyType()){
        case PointerToQObject:
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(reinterpret_cast<const QObject*>(content.first))){
                key = link->getJavaObjectLocalRef(env);
            }
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(keyMetaType())){
                if(FunctionalResolver resolver = registeredFunctionalResolver(*typeId)){
                    bool success = false;
                    key = resolver(env, content.first, &success);
                    break;
                }
            }
            Q_FALLTHROUGH();
        case Pointer:
            for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(content.first)){
                key = link->getJavaObjectLocalRef(env);
                break;
            }
            break;
        default:
            break;
        }
        jobject value{nullptr};
        switch(valueType()){
        case PointerToQObject:
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(reinterpret_cast<const QObject*>(content.second))){
                value = link->getJavaObjectLocalRef(env);
            }
            break;
        case FunctionPointer:
            if(const std::type_info* typeId = getTypeByMetaType(valueMetaType())){
                if(FunctionalResolver resolver = registeredFunctionalResolver(*typeId)){
                    bool success = false;
                    value = resolver(env, content.second, &success);
                    break;
                }
            }
            Q_FALLTHROUGH();
        case Pointer:
            for(QSharedPointer<QtJambiLink> link : QtJambiLink::findLinksForPointer(content.second)){
                value = link->getJavaObjectLocalRef(env);
                break;
            }
            break;
        default:
            break;
        }
        Java::Runtime::Map::put(env, map, key, value);
    }
    clearRC(env, container.object);
    putAllRC(env, container.object, map);
}

void PointersRCAutoHashAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    AutoHashAccess::swap(env, container, container2);
    if(PointersRCAutoHashAccess* access = dynamic_cast<PointersRCAutoHashAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void PointersRCAutoHashAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    AutoHashAccess::assign(env, container, container2);
    if(PointersRCAutoHashAccess* access = dynamic_cast<PointersRCAutoHashAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void PointersRCAutoHashAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    AutoHashAccess::clear(env, container);
    clearRC(env, container.object);
}

void PointersRCAutoHashAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    AutoHashAccess::insert(env, container, key, value);
    putRC(env, container.object, key, value);
}

jint PointersRCAutoHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    jint result = AutoHashAccess::remove(env, container, key);
    if(result>0){
        removeRC(env, container.object, key);
    }
    return result;
}

jobject PointersRCAutoHashAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = AutoHashAccess::take(env, container, key);
    removeRC(env, container.object, key);
    return result;
}

NestedPointersRCAutoHashAccess::NestedPointersRCAutoHashAccess(NestedPointersRCAutoHashAccess& other)
    : AutoHashAccess(other), ReferenceCountingSetContainer() {}

NestedPointersRCAutoHashAccess* NestedPointersRCAutoHashAccess::clone(){
    return new NestedPointersRCAutoHashAccess(*this);
}

void NestedPointersRCAutoHashAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    if(size(env, container.container)==0){
        clearRC(env, container.object);
    }else{
        JniLocalFrame frame(env, 200);
        jobject set = Java::Runtime::HashSet::newInstance(env);
        auto access1 = keyNestedContainerAccess();
        auto access2 = valueNestedContainerAccess();
        auto iterator = constKeyValueIterator(container.container);
        while(iterator->hasNext()){
            auto current = iterator->next();
            unfoldAndAddContainer(env, set, current.first, keyType(), keyMetaType(), access1);
            unfoldAndAddContainer(env, set, current.second, valueType(), valueMetaType(), access2);
        }
        if(access1)
            access1->dispose();
        if(access2)
            access2->dispose();
        addAllRC(env, container.object, set);
    }
}

void NestedPointersRCAutoHashAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    AutoHashAccess::swap(env, container, container2);
    if(NestedPointersRCAutoHashAccess* access = dynamic_cast<NestedPointersRCAutoHashAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void NestedPointersRCAutoHashAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    AutoHashAccess::assign(env, container, container2);
    if(NestedPointersRCAutoHashAccess* access = dynamic_cast<NestedPointersRCAutoHashAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void NestedPointersRCAutoHashAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    AutoHashAccess::clear(env, container);
    clearRC(env, container.object);
}

void NestedPointersRCAutoHashAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    AutoHashAccess::insert(env, container, key, value);
    addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
    addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), value);
}

jint NestedPointersRCAutoHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    jint result = AutoHashAccess::remove(env, container, key);
    if(result>0){
        removeRC(env, container.object, key);
    }
    return result;
}

jobject NestedPointersRCAutoHashAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = AutoHashAccess::take(env, container, key);
    removeRC(env, container.object, key);
    return result;
}
