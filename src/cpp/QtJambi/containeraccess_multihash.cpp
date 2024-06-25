/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
#include "java_p.h"

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
bool isEquals(const QMetaType& keyMetaType, const void * ptr, const void* ptr2);
#else
QT_WARNING_DISABLE_GCC("-Wstrict-aliasing")
QT_WARNING_DISABLE_CLANG("-Wstrict-aliasing")
#endif

bool AutoMultiHashAccess::isConstant() {return false;}

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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                int valueMetaType,
                size_t valueAlign,
#else
                const QMetaType& valueMetaType,
#endif
                const QtJambiUtils::QHashFunction& valueHashFunction,
                const QtJambiUtils::InternalToExternalConverter& valueInternalToExternalConverter,
                const QtJambiUtils::ExternalToInternalConverter& valueExternalToInternalConverter
        )
    : AbstractMultiHashAccess(), AutoHashAccess(
          keyMetaType,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
          keyAlign,
#endif
          keyHashFunction,
          keyInternalToExternalConverter,
          keyExternalToInternalConverter,
          valueMetaType,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
          valueAlign,
#endif
          valueHashFunction,
          valueInternalToExternalConverter,
          valueExternalToInternalConverter)
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
void AutoMultiHashAccess::reserve(JNIEnv * env, void* container, jint capacity){ AutoHashAccess::reserve(env, container, capacity); }

bool AutoMultiHashAccess::destructContainer(void* container) {
    return AutoHashAccess::destructContainer(container);
}

void* AutoMultiHashAccess::constructContainer(void* result, const void* container) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return AutoHashAccess::constructContainer(result, container);
#else
    result = new(result) MultiHashData;
#endif
    if(container)
        assign(result, container);
    return result;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void* AutoMultiHashAccess::constructContainer(void* result, void* container) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    return AutoHashAccess::constructContainer(result, container);
#else
    MultiHashData* _this = new(result) MultiHashData;
    MultiHashData* other = reinterpret_cast<MultiHashData*>(container);
    std::swap(_this->d, other->d);
    std::swap(_this->m_size, other->m_size);
#endif
    return result;
}
#endif

void AutoMultiHashAccess::assign(void* container, const void* other) {
    AutoHashAccess::assign(container, other);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    MultiHashData* map = reinterpret_cast<MultiHashData*>(container);
    const MultiHashData* map2 = reinterpret_cast<const MultiHashData*>(other);
    map->m_size = map2->m_size;
#endif
}

int AutoMultiHashAccess::registerContainer(const QByteArray& containerTypeName) {return AutoHashAccess::registerContainer(containerTypeName);}
void AutoMultiHashAccess::dispose() {delete this;}
void AutoMultiHashAccess::analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) {AutoHashAccess::analyzeEntries(container, analyzer, data);}
const QMetaType& AutoMultiHashAccess::keyMetaType() {return AutoHashAccess::keyMetaType();}
const QMetaType& AutoMultiHashAccess::valueMetaType() {return AutoHashAccess::valueMetaType();}
void AutoMultiHashAccess::clear(JNIEnv *env, void* container) {AutoHashAccess::clear(env, container);}
jboolean AutoMultiHashAccess::contains(JNIEnv *env, const void* container, jobject key) {return AutoHashAccess::contains(env, container, key);}
jint AutoMultiHashAccess::count(JNIEnv *env, const void* container, jobject key) {return AutoHashAccess::count(env, container, key);}
jobject AutoMultiHashAccess::begin(JNIEnv *env, QtJambiNativeID ownerId, void* container) {return AutoHashAccess::begin(env, ownerId, container);}
jobject AutoMultiHashAccess::end(JNIEnv *env, QtJambiNativeID ownerId, void* container) {return AutoHashAccess::end(env, ownerId, container);}
jobject AutoMultiHashAccess::find(JNIEnv *env, QtJambiNativeID ownerId, void* container, jobject key) {return AutoHashAccess::find(env, ownerId, container, key);}
jobject AutoMultiHashAccess::constBegin(JNIEnv *env, QtJambiNativeID ownerId, const void* container) {return AutoHashAccess::constBegin(env, ownerId, container);}
jobject AutoMultiHashAccess::constEnd(JNIEnv *env, QtJambiNativeID ownerId, const void* container) {return AutoHashAccess::constEnd(env, ownerId, container);}
jobject AutoMultiHashAccess::constFind(JNIEnv *env, QtJambiNativeID ownerId, const void* container, jobject key) {return AutoHashAccess::constFind(env, ownerId, container, key);}

void AutoMultiHashAccess::insert(JNIEnv *env, void* container, jobject key, jobject value){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QHashData ** map = reinterpret_cast<QHashData **>(container);
    QHashData*& d = *map;

    detach(map);

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
jobject AutoMultiHashAccess::keys(JNIEnv *env, const void* container) {return AutoHashAccess::keys(env, container);}
jobject AutoMultiHashAccess::keys(JNIEnv *env, const void* container, jobject value) {return AutoHashAccess::keys(env, container, value);}
jboolean AutoMultiHashAccess::equal(JNIEnv *env, const void* container, jobject other) {return AutoHashAccess::equal(env, container, other);}

jint AutoMultiHashAccess::remove(JNIEnv *env, void* container, jobject key) {
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    return AutoHashAccess::remove(env, container, key);
#else
    Q_UNUSED(env)
    MultiHashData * map = reinterpret_cast<MultiHashData*>(container);
    if (map->m_size==0) // prevents detaching shared null
        return 0;
    jvalue jv;
    jv.l = key;
    QtJambiScope scope;
    void* akey = nullptr;
    qsizetype n = 0;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        auto it = map->d->find(*this, akey);
        detach(&map->d);
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

jint AutoMultiHashAccess::size(JNIEnv *env, const void* container) {
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    return AutoHashAccess::size(env, container);
#else
    Q_UNUSED(env)
    const MultiHashData * map = reinterpret_cast<const MultiHashData*>(container);
    return map->m_size;
#endif
}

jobject AutoMultiHashAccess::take(JNIEnv *env, void* container, jobject key) {return AutoHashAccess::take(env, container, key);}
jobject AutoMultiHashAccess::value(JNIEnv *env, const void* container, jobject key,jobject defaultValue) {return AutoHashAccess::value(env, container, key, defaultValue);}
jobject AutoMultiHashAccess::values(JNIEnv *env, const void* container) {return AutoHashAccess::values(env, container);}
AbstractMultiHashAccess* AutoMultiHashAccess::clone() {return new AutoMultiHashAccess(*this);}

jobject AutoMultiHashAccess::uniqueKeys(JNIEnv *env, const void* container)
{
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    QHashData* d = *map;
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        Node *firstNode(d->firstNode());
        Node *lastNode(reinterpret_cast<Node*>(d));
        listAccess->reserve(env, listContainer, d->size);
        jint idx = listAccess->size(env, listContainer);
        while(firstNode != lastNode){
            void* key1 = reinterpret_cast<char*>(firstNode)+m_offset1;
            listAccess->insert(env, listContainer, idx++, 1, nodeKey(env, firstNode));
            firstNode = QHashData::nextNode(firstNode);
            while(firstNode != lastNode && same_key(firstNode, key1)){
                firstNode = QHashData::nextNode(firstNode);
            }
        }
#else
        QHashData::iterator e = d->end(*this);
        QHashData::iterator n = d->begin(*this);
        jint idx = listAccess->size(env, listContainer);
        while (n != e) {
            jvalue jv;
            jv.l = nullptr;
            m_keyInternalToExternalConverter(env, nullptr, n.key(), jv, true);
            listAccess->insert(env, listContainer, idx++, 1, jv.l);
            ++n;
        }
#endif
        return ContainerAccessAPI::objectFromQList(env, listContainer, listAccess);
    }
    return nullptr;
}

void AutoMultiHashAccess::unite(JNIEnv *env, void* container, jobject other)
{
    QHashData ** map = reinterpret_cast<QHashData **>(container);
    QHashData*& d = *map;
    void* ptr{nullptr};
    if (ContainerAPI::getAsQMultiHash(env, other, keyMetaType(), valueMetaType(), ptr)
            || ContainerAPI::getAsQHash(env, other, keyMetaType(), valueMetaType(), ptr)) {
        QHashData *const* map2 = reinterpret_cast<QHashData *const*>(ptr);
        QHashData* d2 = *map2;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        if (d == &QHashData::shared_null && d2->ref.ref()) {
            d = d2;
        } else {
            detach(map);
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
            detach(map);
        }
#endif
    }else{
        jobject iterator = QtJambiAPI::entrySetIteratorOfJavaMap(env, other);
        while(QtJambiAPI::hasJavaIteratorNext(env, iterator)){
            jobject entry = QtJambiAPI::nextOfJavaIterator(env, iterator);
            jobject value = QtJambiAPI::valueOfJavaMapEntry(env, entry);
            if(Java::Runtime::Collection::isInstanceOf(env, value)){
                jobject iterator2 = QtJambiAPI::iteratorOfJavaCollection(env, value);
                while(QtJambiAPI::hasJavaIteratorNext(env, iterator2)){
                    jobject cvalue = QtJambiAPI::nextOfJavaIterator(env, iterator2);
                    insert(env, container, QtJambiAPI::keyOfJavaMapEntry(env, entry), cvalue);
                }
            }
        }
    }
}

jobject AutoMultiHashAccess::values(JNIEnv *env, const void* container, jobject key)
{
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
        QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
        QHashData* d = *map;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        if(!d)
            return ContainerAccessAPI::objectFromQList(env, listContainer, listAccess);
#endif
        jvalue jv;
        jv.l = key;
        QtJambiScope scope;
        void* akey = nullptr;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            Node* e = reinterpret_cast<Node*>(d);
            Node *n = *findNode(map, akey);
            jint idx = listAccess->size(env, listContainer);
            if(n && n != e){
                do{
                    listAccess->insert(env, listContainer, idx++, 1, nodeValue(env, n));
                    n = QHashData::nextNode(n);
                }while(n != e && same_key(n, akey));
            }
#else
            iterator i = d->find(*this, akey);
            if(i.e && !i.i.isUnused()){
                multi_iterator& it = reinterpret_cast<multi_iterator&>(i);
                const Chain* chain = *it.e;
                jint idx = listAccess->size(env, listContainer);
                while(chain){
                    jvalue jv;
                    jv.l = nullptr;
                    m_valueInternalToExternalConverter(env, nullptr, chain->value(), jv, true);
                    listAccess->insert(env, listContainer, idx++, 1, jv.l);
                    chain = chain->next(*this);
                }
            }
#endif
        }
        return ContainerAccessAPI::objectFromQList(env, listContainer, listAccess);
    }
    return nullptr;
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

jobject AutoMultiHashAccess::find(JNIEnv *env, QtJambiNativeID ownerId, void* container, jobject key, jobject value)
{
    QHashData ** map = reinterpret_cast<QHashData **>(container);
    detach(map);
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
                        return createIterator(env, ownerId, new void*(firstNode));
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
                        return createIterator(env, ownerId, new iterator(i));
                    }
                    ++i;
                }
            }
        }
#endif
    }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    return createIterator(env, ownerId, new void*(reinterpret_cast<Node*>(d)));
#else
    return createIterator(env, ownerId, d ? new iterator(d->end(*this)) : new iterator(*this));
#endif
}

jobject AutoMultiHashAccess::constFind(JNIEnv *env, QtJambiNativeID ownerId, const void* container, jobject key, jobject value)
{
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
                        return createIterator(env, ownerId, new void*(firstNode));
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
                        return createIterator(env, ownerId, new iterator(i));
                    }
                    ++i;
                }
            }
        }
#endif
    }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    return createConstIterator(env, ownerId, new void*(reinterpret_cast<Node*>(d)));
#else
    return createConstIterator(env, ownerId, d ? new iterator(d->end(*this)) : new iterator(*this));
#endif
}

jint AutoMultiHashAccess::remove(JNIEnv *env, void* container, jobject key, jobject value)
{
    jint c = 0;
    QHashData ** map = reinterpret_cast<QHashData **>(container);
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

void AutoMultiHashAccess::replace(JNIEnv *env, void* container, jobject key, jobject value) {AutoHashAccess::insert(env, container, key, value);}

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
    const QtPrivate::QMetaTypeInterface *kiface = m_keyMetaType.iface();
    const QtPrivate::QMetaTypeInterface *viface = m_valueMetaType.iface();
    const MultiHashData * map = reinterpret_cast<const MultiHashData*>(ptr);
    QHashData* d = map->d;
    if(d){
        QHashData::iterator e = d->end(*this);
        QHashData::iterator n = d->begin(*this);
        s << quint32(map->m_size);
        if(m_offset2){
            while (n != e) {
                Chain* chain = *reinterpret_cast<Chain**>(n.value());
                QList<Chain*> chainsReverse;
                while(chain){
                    chainsReverse.prepend(chain);
                    chain = chain->next(*this);
                }
                for(Chain* chn : chainsReverse){
                    kiface->dataStreamOut(kiface, s, n.key());
                    viface->dataStreamOut(viface, s, chn->value());
                }
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

#endif
