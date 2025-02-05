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
#include "containeraccess_p.h"
#include "functionpointer.h"
#include "registryutil_p.h"
#include "qtjambilink_p.h"
#include "java_p.h"

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
bool isEquals(const QMetaType& keyMetaType, const void * ptr, const void* ptr2);
#else
QT_WARNING_DISABLE_GCC("-Wstrict-aliasing")
QT_WARNING_DISABLE_CLANG("-Wstrict-aliasing")
#endif

AutoMultiHashAccess::AutoMultiHashAccess(const AutoMultiHashAccess & other)
    : AbstractMultiHashAccess(), AutoHashAccess(other)
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
          ,m_chainOffset(other.m_chainOffset)
          ,m_chainAlign(other.m_chainAlign)
          ,m_chainSize(other.m_chainSize)
#endif
{
}

AutoMultiHashAccess::~AutoMultiHashAccess(){}

AutoMultiHashAccess::AutoMultiHashAccess(
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                int keyMetaType,
                size_t keyAlign,
#else
                const QMetaType& keyMetaType,
#endif
                const QtJambiUtils::QHashFunction& keyHashFunction,
                const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
                const QtJambiUtils::ExternalToInternalConverter& keyExternalToInternalConverter,
                const QSharedPointer<AbstractContainerAccess>& keyNestedContainerAccess,
                PtrOwnerFunction keyOwnerFunction,
                AbstractContainerAccess::DataType keyDataType,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                int valueMetaType,
                size_t valueAlign,
#else
                const QMetaType& valueMetaType,
#endif
                const QtJambiUtils::QHashFunction& valueHashFunction,
                const QtJambiUtils::InternalToExternalConverter& valueInternalToExternalConverter,
                const QtJambiUtils::ExternalToInternalConverter& valueExternalToInternalConverter,
                const QSharedPointer<AbstractContainerAccess>& valueNestedContainerAccess,
                PtrOwnerFunction valueOwnerFunction,
                AbstractContainerAccess::DataType valueDataType
        )
    : AbstractMultiHashAccess(), AutoHashAccess(
          keyMetaType,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
          keyAlign,
#endif
          keyHashFunction,
          keyInternalToExternalConverter,
          keyExternalToInternalConverter,
          keyNestedContainerAccess,
          keyOwnerFunction,
          keyDataType,
          valueMetaType,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
          valueAlign,
#endif
          valueHashFunction,
          valueInternalToExternalConverter,
          valueExternalToInternalConverter,
          valueNestedContainerAccess,
          valueOwnerFunction,
          valueDataType)
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
      ,m_chainOffset(0)
      ,m_chainAlign(qMax<size_t>(m_valueMetaType.alignOf(), alignof(void*)))
      ,m_chainSize(0)
#endif
{
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    m_align = qMax(m_align, alignof(void*));
    m_offset2 = m_keyMetaType.sizeOf();
    if(m_offset2 % alignof(void*)>0)
        m_offset2 += alignof(void*)-m_offset2 % alignof(void*);
    m_size = m_offset2 + sizeof(void*);
    if(m_size%m_align>0)
        m_size += m_align-m_size%m_align;
    m_chainOffset = m_valueMetaType.sizeOf();
    if(m_chainOffset % alignof(void*)>0)
        m_chainOffset += alignof(void*)-m_chainOffset % alignof(void*);
    m_chainSize = m_chainOffset + sizeof(void*);
    if(m_chainSize % m_chainAlign > 0)
        m_chainSize += m_chainAlign - m_chainSize % m_chainAlign;
#endif
}

jint AutoMultiHashAccess::capacity(JNIEnv * env, const void* container){ return AutoHashAccess::capacity(env, container); }
void AutoMultiHashAccess::reserve(JNIEnv * env, const ContainerInfo& container, jint capacity){ AutoHashAccess::reserve(env, container, capacity); }

bool AutoMultiHashAccess::destructContainer(void* container) {
    return AutoHashAccess::destructContainer(container);
}

void* AutoMultiHashAccess::constructContainer(JNIEnv*, void* result, const ConstContainerAndAccessInfo& container) {
    return constructContainer(result, container.container);
}

void* AutoMultiHashAccess::constructContainer(void* result) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return AutoHashAccess::constructContainer(result);
#else
    return new(result) MultiHashData;
#endif
}

void* AutoMultiHashAccess::constructContainer(void* result, const void* container) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return AutoHashAccess::constructContainer(result, container);
#else
    result = new(result) MultiHashData;
    assign(result, container);
    return result;
#endif
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void* AutoMultiHashAccess::constructContainer(JNIEnv*, void* result, const ContainerAndAccessInfo& container) {
    return constructContainer(result, container.container);
}

void* AutoMultiHashAccess::constructContainer(void* result, void* container) {
    MultiHashData* _this = new(result) MultiHashData;
    MultiHashData* other = reinterpret_cast<MultiHashData*>(container);
    std::swap(_this->d, other->d);
    std::swap(_this->m_size, other->m_size);
    return result;
}
#endif

void AutoMultiHashAccess::assign(JNIEnv *, const ContainerInfo& container, const ConstContainerAndAccessInfo& other){
    assign(container.container, other.container);
}

void AutoMultiHashAccess::assign(void* container, const void* other) {
    AutoHashAccess::assign(container, other);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    MultiHashData* map = reinterpret_cast<MultiHashData*>(container);
    const MultiHashData* map2 = reinterpret_cast<const MultiHashData*>(other);
    map->m_size = map2->m_size;
#endif
}

std::unique_ptr<AbstractHashAccess::KeyValueIterator> AutoMultiHashAccess::keyValueIterator(const void* container) { return AutoHashAccess::keyValueIterator(container); }
int AutoMultiHashAccess::registerContainer(const QByteArray& containerTypeName) {return AutoHashAccess::registerContainer(containerTypeName);}
void AutoMultiHashAccess::dispose() {delete this;}
const QMetaType& AutoMultiHashAccess::keyMetaType() {return AutoHashAccess::keyMetaType();}
const QMetaType& AutoMultiHashAccess::valueMetaType() {return AutoHashAccess::valueMetaType();}
AbstractContainerAccess::DataType AutoMultiHashAccess::keyType() {return AutoHashAccess::keyType();}
AbstractContainerAccess::DataType AutoMultiHashAccess::valueType() {return AutoHashAccess::valueType();}
AbstractContainerAccess* AutoMultiHashAccess::keyNestedContainerAccess() {return AutoHashAccess::keyNestedContainerAccess();}
AbstractContainerAccess* AutoMultiHashAccess::valueNestedContainerAccess() {return AutoHashAccess::valueNestedContainerAccess();}
const QSharedPointer<AbstractContainerAccess>& AutoMultiHashAccess::sharedKeyNestedContainerAccess() {return AutoHashAccess::sharedKeyNestedContainerAccess();}
const QSharedPointer<AbstractContainerAccess>& AutoMultiHashAccess::sharedValueNestedContainerAccess() {return AutoHashAccess::sharedValueNestedContainerAccess();}
bool AutoMultiHashAccess::hasKeyNestedContainerAccess() {return AutoHashAccess::hasKeyNestedContainerAccess();}
bool AutoMultiHashAccess::hasKeyNestedPointers() {return AutoHashAccess::hasKeyNestedPointers();}
bool AutoMultiHashAccess::hasValueNestedContainerAccess() {return AutoHashAccess::hasValueNestedContainerAccess();}
bool AutoMultiHashAccess::hasValueNestedPointers() {return AutoHashAccess::hasValueNestedPointers();}
void AutoMultiHashAccess::clear(JNIEnv *env, const ContainerInfo& container) {AutoHashAccess::clear(env, container);}
jboolean AutoMultiHashAccess::contains(JNIEnv *env, const void* container, jobject key) {return AutoHashAccess::contains(env, container, key);}
jint AutoMultiHashAccess::count(JNIEnv *env, const void* container, jobject key) {return AutoHashAccess::count(env, container, key);}
jobject AutoMultiHashAccess::begin(JNIEnv *env, const ExtendedContainerInfo& container) {return AutoHashAccess::begin(env, container);}
jobject AutoMultiHashAccess::end(JNIEnv *env, const ExtendedContainerInfo& container) {return AutoHashAccess::end(env, container);}
jobject AutoMultiHashAccess::find(JNIEnv *env, const ExtendedContainerInfo& container, jobject key) {return AutoHashAccess::find(env, container, key);}
jobject AutoMultiHashAccess::constBegin(JNIEnv *env, const ConstExtendedContainerInfo& container) {return AutoHashAccess::constBegin(env, container);}
jobject AutoMultiHashAccess::constEnd(JNIEnv *env, const ConstExtendedContainerInfo& container) {return AutoHashAccess::constEnd(env, container);}
jobject AutoMultiHashAccess::constFind(JNIEnv *env, const ConstExtendedContainerInfo& container, jobject key) {return AutoHashAccess::constFind(env, container, key);}
bool AutoMultiHashAccess::isDetached(const void* container){ return AutoHashAccess::isDetached(container); }
void AutoMultiHashAccess::detach(const ContainerInfo& container){ AutoHashAccess::detach(container); }
bool AutoMultiHashAccess::isSharedWith(const void* container, const void* container2){ return AutoHashAccess::isSharedWith(container, container2); }
const QObject* AutoMultiHashAccess::getOwner(const void* container){ return AutoHashAccess::getOwner(container); }
bool AutoMultiHashAccess::hasOwnerFunction(){ return AutoHashAccess::hasOwnerFunction(); }

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
void AutoMultiHashAccess::swap(JNIEnv *env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){AutoHashAccess::swap(env, container, container2);}
#else
void AutoMultiHashAccess::swap(JNIEnv *, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    MultiHashData *& map = *reinterpret_cast<MultiHashData **>(container.container);
    MultiHashData *& map2 = *reinterpret_cast<MultiHashData **>(container2.container);
    if(map && map2){
#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
        qSwap(map->d, map2->d);
#else
        qt_ptr_swap(map->d, map2->d);
#endif
        std::swap(map->m_size, map2->m_size);
    }else{
#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
        qSwap(map, map2);
#else
        qt_ptr_swap(map, map2);
#endif
    }
}
#endif

void AutoMultiHashAccess::insert(JNIEnv *env, const ContainerInfo& container, jobject key, jobject value){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    detach(container);
    QHashData ** map = reinterpret_cast<QHashData **>(container.container);
    QHashData*& d = *map;

    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l);
    d->willGrow();

    uint h;
    Node **nextNode = findNode(map, akey, &h);
    createNode(d, h, akey, env, value, nextNode);
#else
    AutoHashAccess::insert(env, container, key, value);
#endif
}

IsBiContainerFunction AutoMultiHashAccess::getIsBiContainerFunction(){
    return ContainerAPI::getAsQMultiHash;
}

size_t AutoMultiHashAccess::sizeOf() {
    return sizeof(QMultiHash<char,char>);
}

ushort AutoMultiHashAccess::alignOf() const{
    return ushort(sizeof(QMultiHash<char,char>));
}

jobject AutoMultiHashAccess::key(JNIEnv *env, const void* container, jobject value, jobject defaultKey) { return AutoHashAccess::key(env, container, value, defaultKey); }
ContainerAndAccessInfo AutoMultiHashAccess::keys(JNIEnv *env, const ConstContainerInfo& container) {return AutoHashAccess::keys(env, container);}
ContainerAndAccessInfo AutoMultiHashAccess::keys(JNIEnv *env, const ConstContainerInfo& container, jobject value) {return AutoHashAccess::keys(env, container, value);}
jboolean AutoMultiHashAccess::equal(JNIEnv *env, const void* container, jobject other) {return AutoHashAccess::equal(env, container, other);}
jint AutoMultiHashAccess::size(JNIEnv *env, const void* container)  {return AutoHashAccess::size(env, container);}

jint AutoMultiHashAccess::remove(JNIEnv *env, const ContainerInfo& container, jobject key) {
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    return AutoHashAccess::remove(env, container, key);
#else
    Q_UNUSED(env)
    MultiHashData * map = reinterpret_cast<MultiHashData*>(container.container);
    if (map->m_size==0) // prevents detaching shared null
        return 0;
    jvalue jv;
    jv.l = key;
    QtJambiScope scope;
    void* akey = nullptr;
    qsizetype n = 0;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        auto it = map->d->find(*this, akey);
        detach(container);
        it = map->d->detachedIterator(*this, it);

        if (it.isUnused())
            return 0;
        Chain*& chain = *reinterpret_cast<Chain**>(it.value());
        n = chain->free(*this);
        chain = nullptr;
        m_size -= n;
        Q_ASSERT(map->m_size >= 0);
        map->d->erase(*this, it);
    }
    return n;
#endif
}

qsizetype AutoMultiHashAccess::size(const void* container){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    return AutoHashAccess::size(container);
#else
    const MultiHashData * map = reinterpret_cast<const MultiHashData*>(container);
    return map->m_size;
#endif
}

jobject AutoMultiHashAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {return AutoHashAccess::take(env, container, key);}
jobject AutoMultiHashAccess::value(JNIEnv *env, const void* container, jobject key,jobject defaultValue) {return AutoHashAccess::value(env, container, key, defaultValue);}
ContainerAndAccessInfo AutoMultiHashAccess::values(JNIEnv *env, const ConstContainerInfo& container) {return AutoHashAccess::values(env, container);}
AbstractMultiHashAccess* AutoMultiHashAccess::clone() {return new AutoMultiHashAccess(*this);}

ContainerAndAccessInfo AutoMultiHashAccess::uniqueKeys(JNIEnv *env, const ConstContainerInfo& container)
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        Node *firstNode(d->firstNode());
        Node *lastNode(reinterpret_cast<Node*>(d));
        listAccess->reserve(env, result, d->size);
        jint idx = listAccess->size(env, result.container);
        while(firstNode != lastNode){
            void* key1 = reinterpret_cast<char*>(firstNode)+m_offset1;
            if(listAccess->append(result.container, key1)){
                idx++;
            }else{
                listAccess->insert(env, result, idx++, 1, nodeKey(env, firstNode));
            }
            firstNode = QHashData::nextNode(firstNode);
            while(firstNode != lastNode && same_key(firstNode, key1)){
                firstNode = QHashData::nextNode(firstNode);
            }
        }
#else
        QHashData::iterator e = d->end(*this);
        QHashData::iterator n = d->begin(*this);
        jint idx = listAccess->size(env, result.container);
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

void AutoMultiHashAccess::unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other)
{
    QHashData ** map = reinterpret_cast<QHashData **>(container.container);
    QHashData*& d = *map;
    if (ContainerAPI::getAsQMultiHash(env, other.object, keyMetaType(), valueMetaType(), other.container, other.access)
            || ContainerAPI::getAsQHash(env, other.object, keyMetaType(), valueMetaType(), other.container, other.access)) {
        QHashData *const* map2 = reinterpret_cast<QHashData *const*>(other.container);
        QHashData* d2 = *map2;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        if (d == &QHashData::shared_null && d2->ref.ref()) {
            d = d2;
        } else {
            detach(container);
            Node *firstNode2(reinterpret_cast<Node*>(d2));
            Node *lastNode2(d2->firstNode());
            while (firstNode2 != lastNode2) {
                firstNode2 = QHashData::previousNode(firstNode2);
                void* key2 = reinterpret_cast<char*>(firstNode2)+m_offset1;
                void* value2 = reinterpret_cast<char*>(firstNode2)+m_offset2;

                uint h;
                Node **nextNode = findNode(map, key2, &h);
                createNode(d, h, key2, value2, nextNode);
            }
        }
#else
        if(!d && d2 && d2->ref.ref()){
            d = d2;
        }else{
            detach(container);
        }
#endif
    }else{
        jobject iterator = QtJambiAPI::entrySetIteratorOfJavaMap(env, other.object);
        while(QtJambiAPI::hasJavaIteratorNext(env, iterator)){
            jobject entry = QtJambiAPI::nextOfJavaIterator(env, iterator);
            jobject value = QtJambiAPI::valueOfJavaMapEntry(env, entry);
            if(Java::Runtime::Collection::isInstanceOf(env, value)){
                jobject iterator2 = QtJambiAPI::iteratorOfJavaIterable(env, value);
                while(QtJambiAPI::hasJavaIteratorNext(env, iterator2)){
                    jobject cvalue = QtJambiAPI::nextOfJavaIterator(env, iterator2);
                    insert(env, container, QtJambiAPI::keyOfJavaMapEntry(env, entry), cvalue);
                }
            }
        }
    }
}

ContainerAndAccessInfo AutoMultiHashAccess::values(JNIEnv *env, const ConstContainerInfo& container, jobject key)
{
    ContainerAndAccessInfo result;
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
        QHashData *const* map = reinterpret_cast<QHashData *const*>(container.container);
        QHashData* d = *map;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        if(d)
#endif
        {
            jvalue jv;
            jv.l = key;
            QtJambiScope scope;
            void* akey = nullptr;
            if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
                jint idx = listAccess->size(env, result.container);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                Node* e = reinterpret_cast<Node*>(d);
                Node *n = *findNode(map, akey);
                if(n && n != e){
                    do{
                        if(listAccess->append(result.container, reinterpret_cast<char*>(n)+m_offset2)){
                            idx++;
                        }else{
                            listAccess->insert(env, result, idx++, 1, nodeValue(env, n));
                        }
                        n = QHashData::nextNode(n);
                    }while(n != e && same_key(n, akey));
                }
#else
                iterator i = d->find(*this, akey);
                if(i.e && !i.i.isUnused()){
                    multi_iterator& it = reinterpret_cast<multi_iterator&>(i);
                    const Chain* chain = *it.e;
                    while(chain){
                        if(listAccess->append(result.container, chain->value())){
                            idx++;
                        }else{
                            jvalue jv;
                            jv.l = nullptr;
                            m_valueInternalToExternalConverter(env, nullptr, chain->value(), jv, true);
                            listAccess->insert(env, result, idx++, 1, jv.l);
                        }
                        chain = chain->next(*this);
                    }
                }
#endif
            }
        }
    }
    return result;
}

jboolean AutoMultiHashAccess::contains(JNIEnv *env, const void* container, jobject key, jobject value) {
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    QHashData* d = *map;
    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        if(Node *n = *findNode(map, akey)){
            jv.l = value;
            void* avalue = nullptr;
            if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
                Node *lastNode(reinterpret_cast<Node*>(d));
                do{
                    if(isEquals(m_valueMetaType, avalue, reinterpret_cast<char*>(n)+m_offset2))
                        return true;
                    n = QHashData::nextNode(n);
                }while(n != lastNode && same_key(n, akey));
            }
        }
#else
        iterator i = d->find(*this, akey);
        if(i.e && !i.i.isUnused()){
            jv.l = value;
            void* avalue = nullptr;
            if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
                multi_iterator& it = reinterpret_cast<multi_iterator&>(i);
                const Chain* chain = *it.e;
                while(chain){
                    if(m_valueMetaType.equals(chain->value(), avalue)){
                        return true;
                    }
                    chain = chain->next(*this);
                }
            }
        }
#endif
    }
    return false;
}

jint AutoMultiHashAccess::count(JNIEnv *env, const void* container, jobject key, jobject value)
{
    jint c = 0;
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    QHashData* d = *map;
    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        if(Node *firstNode = *findNode(map, akey)){
            jv.l = value;
            void* avalue = nullptr;
            if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
                Node *lastNode(reinterpret_cast<Node*>(d));
                do{
                    if(isEquals(m_valueMetaType, avalue, reinterpret_cast<char*>(firstNode)+m_offset2))
                        ++c;
                    firstNode = QHashData::nextNode(firstNode);
                }while(firstNode != lastNode && same_key(firstNode, akey));
            }
        }
#else
        iterator i = d->find(*this, akey);
        if(i.e && !i.i.isUnused()){
            jv.l = value;
            void* avalue = nullptr;
            if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
                multi_iterator& it = reinterpret_cast<multi_iterator&>(i);
                const Chain* chain = *it.e;
                while(chain){
                    if(m_valueMetaType.equals(chain->value(), avalue)){
                        ++c;
                    }
                    chain = chain->next(*this);
                }
            }
        }
#endif
    }
    return c;
}

jobject AutoMultiHashAccess::find(JNIEnv *env, const ExtendedContainerInfo& container, jobject key, jobject value)
{
    detach(container);
    QHashData ** map = reinterpret_cast<QHashData **>(container.container);
    QHashData* d = *map;
    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        if(Node *firstNode = *findNode(map, akey)){
            jv.l = value;
            void* avalue = nullptr;
            if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
                Node *lastNode(reinterpret_cast<Node*>(d));
                do{
                    if(isEquals(m_valueMetaType, avalue, reinterpret_cast<char*>(firstNode)+m_offset2))
                        return createIterator(env, container.nativeId, new void*(firstNode));
                    firstNode = QHashData::nextNode(firstNode);
                }while(firstNode != lastNode && same_key(firstNode, akey));
            }
        }
#else
        iterator e = d->end(*this);
        iterator i = d->find(*this, akey);
        if(i!=e){
            jv.l = value;
            void* avalue = nullptr;
            if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
                while(i!=e){
                    if(m_valueMetaType.equals(i.value(), avalue)){
                        return createIterator(env, container.nativeId, new iterator(i));
                    }
                    ++i;
                }
            }
        }
#endif
    }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    return createIterator(env, container.nativeId, new void*(reinterpret_cast<Node*>(d)));
#else
    return createIterator(env, container.nativeId, d ? new iterator(d->end(*this)) : new iterator(*this));
#endif
}

jobject AutoMultiHashAccess::constFind(JNIEnv *env, const ConstExtendedContainerInfo& container, jobject key, jobject value)
{
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container.container);
    QHashData* d = *map;
    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        if(Node *firstNode = *findNode(map, akey)){
            jv.l = value;
            void* avalue = nullptr;
            if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
                Node *lastNode(reinterpret_cast<Node*>(d));
                do{
                    if(isEquals(m_valueMetaType, avalue, reinterpret_cast<char*>(firstNode)+m_offset2))
                        return createIterator(env, container.nativeId, new void*(firstNode));
                    firstNode = QHashData::nextNode(firstNode);
                }while(firstNode != lastNode && same_key(firstNode, akey));
            }
        }
#else
        iterator e = d->end(*this);
        iterator i = d->find(*this, akey);
        if(i!=e){
            jv.l = value;
            void* avalue = nullptr;
            if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
                while(i!=e){
                    if(m_valueMetaType.equals(i.value(), avalue)){
                        return createIterator(env, container.nativeId, new iterator(i));
                    }
                    ++i;
                }
            }
        }
#endif
    }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    return createConstIterator(env, container.nativeId, new void*(reinterpret_cast<Node*>(d)));
#else
    return createConstIterator(env, container.nativeId, d ? new iterator(d->end(*this)) : new iterator(*this));
#endif
}

jint AutoMultiHashAccess::remove(JNIEnv *env, const ContainerInfo& container, jobject key, jobject value)
{
    jint c = 0;
    QHashData ** map = reinterpret_cast<QHashData **>(container.container);
    QHashData*& d = *map;
    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        jv.l = value;
        void* avalue = nullptr;
        if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            bool found;
            do{
                found = false;
                if(Node *firstNode = *findNode(map, akey)){
                    Node *lastNode(reinterpret_cast<Node*>(d));
                    do{
                        if(isEquals(m_valueMetaType, avalue, reinterpret_cast<char*>(firstNode)+m_offset2)){
                            ++c;
                            deleteNode(d, firstNode);
                            found = true;
                            break;
                        }
                        firstNode = QHashData::nextNode(firstNode);
                    }while(firstNode != lastNode && same_key(firstNode, akey));
                }
            }while(found);
#else
            iterator i = d->find(*this, akey);
            if(i.e && !i.i.isUnused()){
                jv.l = value;
                void* avalue = nullptr;
                if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
                    multi_iterator& it = reinterpret_cast<multi_iterator&>(i);
                    Chain* previousChain = nullptr;
                    Chain* chain = *it.e;
                    while(chain){
                        if(m_valueMetaType.equals(chain->value(), avalue)){
                            Chain* nextChain = chain->next(*this);
                            if(previousChain)
                                previousChain->next(*this) = nextChain;
                            else
                                *it.e = nextChain;
                            chain->destroy(*this);
                            chain = nextChain;
                            ++c;
                        }else{
                            chain = chain->next(*this);
                        }
                    }
                }
            }
#endif
        }
    }
    return c;
}

void AutoMultiHashAccess::replace(JNIEnv *env, const ContainerInfo& container, jobject key, jobject value) {
    detach(container);
    QHashData ** map = reinterpret_cast<QHashData **>(container.container);
    QHashData* d = *map;
    jvalue jv;
    jv.l = key;
    QtJambiScope scope;
    void* akey = nullptr;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        Node* e = reinterpret_cast<Node*>(d);
        Node *n = nullptr;
        if(d->size>0){
            n = *findNode(map, akey);
        }
        if(n && n!=e){
            jv.l = value;
            void* avalue = reinterpret_cast<char*>(n) + m_offset2;
            m_valueExternalToInternalConverter(env, nullptr, jv, avalue, jValueType::l);
        }else{
            d->willGrow();
            uint h;
            Node **nextNode = findNode(map, akey, &h);
            createNode(d, h, akey, env, value, nextNode);
        }
#else
        if (d && d->size>0){
            auto it = d->find(*this, akey);
            if (it.isUnused() || it==d->end(*this)){
                emplace(container.container, akey, env, value);
            }else{
                jv.l = value;
                void* avalue = it.value();
                m_valueExternalToInternalConverter(env, nullptr, jv, avalue, jValueType::l);
            }
        }
#endif
    }
}

bool AutoMultiHashAccess::isMulti() const{
    return true;
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
char* AutoMultiHashAccess::Chain::value(){
    return reinterpret_cast<char*>(this);
}

const char* AutoMultiHashAccess::Chain::value() const{
    return reinterpret_cast<const char*>(this);
}

void AutoMultiHashAccess::Chain::destroy(const AutoMultiHashAccess& access){
    access.m_valueMetaType.destruct(value());
    operator delete(this);
}

AutoMultiHashAccess::Chain*& AutoMultiHashAccess::Chain::next(const AutoMultiHashAccess& access){
    return *reinterpret_cast<Chain**>(value() + access.m_chainOffset);
}

const AutoMultiHashAccess::Chain* AutoMultiHashAccess::Chain::next(const AutoMultiHashAccess& access) const{
    return *reinterpret_cast<Chain*const*>(value() + access.m_chainOffset);
}

qsizetype AutoMultiHashAccess::Chain::free(const AutoMultiHashAccess& access){
    qsizetype nEntries = 0;
    Chain *e = this;
    while (e) {
        Chain *n = e->next(access);
        ++nEntries;
        e->destroy(access);
        e = n;
    }
    return  nEntries;
}

void AutoMultiHashAccess::initializeIterator(iterator& _it) const{
    multi_iterator& it = reinterpret_cast<multi_iterator&>(_it);
    if (!it.i.atEnd()) {
        Chain*& chain = *reinterpret_cast<Chain**>(it.i.value());
        it.e = &chain;
        Q_ASSERT(it.e && *it.e);
    }
}

bool AutoMultiHashAccess::iteratorEquals(const iterator& it1, const iterator& it2) const{
    return it1.e==it2.e;
}

char* AutoMultiHashAccess::iteratorValue(const iterator& _it) const{
    const multi_iterator& it = reinterpret_cast<const multi_iterator&>(_it);
    Chain*& chain = *it.e;
    return chain->value();
}

AutoHashAccess::iterator& AutoMultiHashAccess::incrementIterator(iterator& _it) const{
    multi_iterator& it = reinterpret_cast<multi_iterator&>(_it);
    Q_ASSERT(it.e && *it.e);
    it.e = &(*it.e)->next(*this);
    Q_ASSERT(it.e);
    if (!*it.e) {
        ++it.i;
        if(it.i.atEnd()){
            it.e = nullptr;
        }else{
            Chain*& chain = *reinterpret_cast<Chain**>(it.i.value());
            it.e = &chain;
        }
    }
    return _it;
}

void AutoMultiHashAccess::emplace(void* container, const void* akey, JNIEnv *env, jobject value){
    MultiHashData* map = reinterpret_cast<MultiHashData*>(container);
    if(map->d){
        auto result = map->d->findOrInsert(*this, akey);
        jvalue jv;
        jv.l = value;
        if (!result.initialized){
            m_keyMetaType.construct(result.it.key(), akey);
            Chain*& chain = *reinterpret_cast<Chain**>(result.it.value());
            chain = reinterpret_cast<Chain*>(operator new(m_chainSize));
            void* val = chain->value();
            m_valueMetaType.construct(val);
            m_valueExternalToInternalConverter(env, nullptr, jv, val, jValueType::l);
            chain->next(*this) = nullptr;
        }else{
            Chain*& chainPtr = *reinterpret_cast<Chain**>(result.it.value());
            Chain* newChain = reinterpret_cast<Chain*>(operator new(m_chainSize));
            void* val = newChain->value();
            m_valueMetaType.construct(val);
            m_valueExternalToInternalConverter(env, nullptr, jv, val, jValueType::l);
            newChain->next(*this) = qExchange(chainPtr, newChain);
        }
        ++map->m_size;
    }
}

void AutoMultiHashAccess::emplace(void* container, const void* akey, const void* value){
    MultiHashData* map = reinterpret_cast<MultiHashData*>(container);
    if(map->d){
        auto result = map->d->findOrInsert(*this, akey);
        if (!result.initialized){
            m_keyMetaType.construct(result.it.key(), akey);
            Chain*& chain = *reinterpret_cast<Chain**>(result.it.value());
            chain = reinterpret_cast<Chain*>(operator new(m_chainSize));
            void* val = chain->value();
            m_valueMetaType.construct(val, value);
            chain->next(*this) = nullptr;
        }else{
            Chain*& chainPtr = *reinterpret_cast<Chain**>(result.it.value());
            Chain* newChain = reinterpret_cast<Chain*>(operator new(m_chainSize));
            void* val = newChain->value();
            m_valueMetaType.construct(val, value);
            newChain->next(*this) = qExchange(chainPtr, newChain);
        }
        ++map->m_size;
    }
}

void AutoMultiHashAccess::eraseSpanEntry(char* value) const{
    Chain*& chain = *reinterpret_cast<Chain**>(value);
    if(chain){
        chain->free(*this);
        chain = nullptr;
    }
}

void AutoMultiHashAccess::copySpanEntry(char* value1, const char* value2) const{
    Chain** chain1 = reinterpret_cast<Chain**>(value1);
    *chain1 = nullptr;
    const Chain* chain2 = *reinterpret_cast<Chain*const*>(value2);
    while(chain2){
        Chain* newChain = reinterpret_cast<Chain*>(operator new(m_chainSize));
        m_valueMetaType.construct(newChain->value(), chain2->value());
        newChain->next(*this) = *chain1;
        *chain1 = newChain;
        chain2 = chain2->next(*this);
    }
}

bool AutoMultiHashAccess::equalSpanEntries(const char* value1, const char* value2) const{
    const Chain* chain1 = *reinterpret_cast<Chain*const*>(value1);
    while(chain1){
        const Chain* chain2 = *reinterpret_cast<Chain*const*>(value2);
        while(chain2){
            if(m_valueMetaType.equals(chain1->value(), chain2->value()))
                break;
            chain2 = chain2->next(*this);
        }
        if(!chain2)
            return false;
        chain1 = chain1->next(*this);
    }
    return true;
}

void AutoMultiHashAccess::dataStreamOut(QDataStream &s, const void *ptr){
    QtPrivate::writeAssociativeMultiContainer(s, ConstContainer{ptr, this});
}

void AutoMultiHashAccess::debugStream(QDebug &dbg, const void *ptr){
    QtPrivate::printAssociativeContainer(dbg, "QMultiHash", ConstContainer{ptr, this});
}
#endif


void KeyPointerRCAutoMultiHashAccess::unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) {
    AutoMultiHashAccess::unite(env, container, other);
    updateRC(env, container);
}

void ValuePointerRCAutoMultiHashAccess::unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) {
    AutoMultiHashAccess::unite(env, container, other);
    updateRC(env, container);
}

KeyPointerRCAutoMultiHashAccess::KeyPointerRCAutoMultiHashAccess(KeyPointerRCAutoMultiHashAccess& other)
    : AutoMultiHashAccess(other), ReferenceCountingSetContainer() {}

AbstractMultiHashAccess* KeyPointerRCAutoMultiHashAccess::clone(){
    return new KeyPointerRCAutoMultiHashAccess(*this);
}

void KeyPointerRCAutoMultiHashAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject set = Java::Runtime::HashSet::newInstance(env);
    auto iterator = keyValueIterator(container.container);
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

void KeyPointerRCAutoMultiHashAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    AutoMultiHashAccess::swap(env, container, container2);
    if(KeyPointerRCAutoMultiHashAccess* access = dynamic_cast<KeyPointerRCAutoMultiHashAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void KeyPointerRCAutoMultiHashAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    AutoMultiHashAccess::assign(env, container, container2);
    if(KeyPointerRCAutoMultiHashAccess* access = dynamic_cast<KeyPointerRCAutoMultiHashAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void KeyPointerRCAutoMultiHashAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    AutoMultiHashAccess::clear(env, container);
    clearRC(env, container.object);
}

void KeyPointerRCAutoMultiHashAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    AutoMultiHashAccess::insert(env, container, key, value);
    addRC(env, container.object, key);
}

jint KeyPointerRCAutoMultiHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    jint result = AutoMultiHashAccess::remove(env, container, key);
    if(result>0){
        removeRC(env, container.object, key);
    }
    return result;
}

jint KeyPointerRCAutoMultiHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    jint result = AutoMultiHashAccess::remove(env, container, key, value);
    if(result>0){
        removeRC(env, container.object, key);
    }
    return result;
}

jobject KeyPointerRCAutoMultiHashAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = AutoMultiHashAccess::take(env, container, key);
    removeRC(env, container.object, key);
    return result;
}

void KeyPointerRCAutoMultiHashAccess::replace(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    AutoMultiHashAccess::replace(env, container, key, value);
}

ValuePointerRCAutoMultiHashAccess::ValuePointerRCAutoMultiHashAccess(ValuePointerRCAutoMultiHashAccess& other)
    : AutoMultiHashAccess(other), ReferenceCountingSetContainer() {}

AbstractMultiHashAccess* ValuePointerRCAutoMultiHashAccess::clone(){
    return new ValuePointerRCAutoMultiHashAccess(*this);
}

void ValuePointerRCAutoMultiHashAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject set = Java::Runtime::HashSet::newInstance(env);
    auto iterator = keyValueIterator(container.container);
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

void ValuePointerRCAutoMultiHashAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    AutoMultiHashAccess::swap(env, container, container2);
    if(ValuePointerRCAutoMultiHashAccess* access = dynamic_cast<ValuePointerRCAutoMultiHashAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void ValuePointerRCAutoMultiHashAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    AutoMultiHashAccess::assign(env, container, container2);
    if(ValuePointerRCAutoMultiHashAccess* access = dynamic_cast<ValuePointerRCAutoMultiHashAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void ValuePointerRCAutoMultiHashAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    AutoMultiHashAccess::clear(env, container);
    clearRC(env, container.object);
}

void ValuePointerRCAutoMultiHashAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    AutoMultiHashAccess::insert(env, container, key, value);
    addRC(env, container.object, value);
}

jint ValuePointerRCAutoMultiHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    ContainerAndAccessInfo oldValues = AutoMultiHashAccess::values(env, container, key);
    jint result = AutoMultiHashAccess::remove(env, container, key);
    if(result>0){
        jobject iter = Java::Runtime::Collection::iterator(env, oldValues.object);
        while(Java::Runtime::Iterator::hasNext(env, iter)){
            jobject value = Java::Runtime::Iterator::next(env, iter);
            if(Java::Runtime::Collection::size(env, AutoMultiHashAccess::keys(env, container, value).object)==0){
                removeRC(env, container.object, value);
            }
        }
    }
    return result;
}

jobject ValuePointerRCAutoMultiHashAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = AutoMultiHashAccess::take(env, container, key);
    if(Java::Runtime::Collection::size(env, AutoMultiHashAccess::keys(env, container, result).object)==0){
        removeRC(env, container.object, result);
    }
    return result;
}

void ValuePointerRCAutoMultiHashAccess::replace(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    jobject oldValue = AutoMultiHashAccess::value(env, container.container, key, nullptr);
    AutoMultiHashAccess::replace(env, container, key, value);
    removeRC(env, container.object, oldValue);
    addRC(env, container.object, value);
}

jint ValuePointerRCAutoMultiHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value){
    jint result = AutoMultiHashAccess::remove(env, container, key, value);
    if(result>0)
        removeRC(env, container.object, value, result);
    return result;
}

PointersRCAutoMultiHashAccess::PointersRCAutoMultiHashAccess(PointersRCAutoMultiHashAccess& other)
    : AutoMultiHashAccess(other), ReferenceCountingMultiMapContainer(other) {}

AbstractMultiHashAccess* PointersRCAutoMultiHashAccess::clone(){
    return new PointersRCAutoMultiHashAccess(*this);
}

void PointersRCAutoMultiHashAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject map = Java::QtJambi::ReferenceUtility$RCMap::newInstance(env);
    auto iterator = keyValueIterator(container.container);
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

void PointersRCAutoMultiHashAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    AutoMultiHashAccess::swap(env, container, container2);
    if(PointersRCAutoMultiHashAccess* access = dynamic_cast<PointersRCAutoMultiHashAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void PointersRCAutoMultiHashAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    AutoMultiHashAccess::assign(env, container, container2);
    updateRC(env, container);
}

void PointersRCAutoMultiHashAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    AutoMultiHashAccess::clear(env, container);
    updateRC(env, container);
}

void PointersRCAutoMultiHashAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    AutoMultiHashAccess::insert(env, container, key, value);
    updateRC(env, container);
}

void PointersRCAutoMultiHashAccess::replace(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    AutoMultiHashAccess::replace(env, container, key, value);
    updateRC(env, container);
}

void PointersRCAutoMultiHashAccess::unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) {
    AutoMultiHashAccess::unite(env, container, other);
    updateRC(env, container);
}

jint PointersRCAutoMultiHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    jint result = AutoMultiHashAccess::remove(env, container, key);
    updateRC(env, container);
    return result;
}

jint PointersRCAutoMultiHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    jint result = AutoMultiHashAccess::remove(env, container, key, value);
    updateRC(env, container);
    return result;
}

jobject PointersRCAutoMultiHashAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = AutoMultiHashAccess::take(env, container, key);
    removeRC(env, key, result);
    return result;
}

NestedPointersRCAutoMultiHashAccess::NestedPointersRCAutoMultiHashAccess(NestedPointersRCAutoMultiHashAccess& other)
    : AutoMultiHashAccess(other), ReferenceCountingSetContainer() {}

AbstractMultiHashAccess* NestedPointersRCAutoMultiHashAccess::clone(){
    return new NestedPointersRCAutoMultiHashAccess(*this);
}

void NestedPointersRCAutoMultiHashAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    if(size(env, container.container)==0){
        clearRC(env, container.object);
    }else{
        JniLocalFrame frame(env, 200);
        jobject set = Java::Runtime::HashSet::newInstance(env);
        auto access1 = keyNestedContainerAccess();
        auto access2 = valueNestedContainerAccess();
        auto iterator = keyValueIterator(container.container);
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

void NestedPointersRCAutoMultiHashAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    AutoMultiHashAccess::swap(env, container, container2);
    if(NestedPointersRCAutoMultiHashAccess* access = dynamic_cast<NestedPointersRCAutoMultiHashAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void NestedPointersRCAutoMultiHashAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    AutoMultiHashAccess::assign(env, container, container2);
    if(NestedPointersRCAutoMultiHashAccess* access = dynamic_cast<NestedPointersRCAutoMultiHashAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void NestedPointersRCAutoMultiHashAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    AutoMultiHashAccess::clear(env, container);
    clearRC(env, container.object);
}

void NestedPointersRCAutoMultiHashAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    AutoMultiHashAccess::insert(env, container, key, value);
    addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
    addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), value);
}

void NestedPointersRCAutoMultiHashAccess::replace(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    AutoMultiHashAccess::replace(env, container, key, value);
    updateRC(env, container);
}

void NestedPointersRCAutoMultiHashAccess::unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) {
    AutoMultiHashAccess::unite(env, container, other);
    updateRC(env, container);
}

jint NestedPointersRCAutoMultiHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    jint result = AutoMultiHashAccess::remove(env, container, key);
    if(result>0){
        removeRC(env, container.object, key);
    }
    return result;
}

jint NestedPointersRCAutoMultiHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    jint result = AutoMultiHashAccess::remove(env, container, key, value);
    if(result>0){
        updateRC(env, container);
    }
    return result;
}

jobject NestedPointersRCAutoMultiHashAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = AutoMultiHashAccess::take(env, container, key);
    updateRC(env, container);
    return result;
}
