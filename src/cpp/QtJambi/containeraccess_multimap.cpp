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

QT_WARNING_DISABLE_GCC("-Winaccessible-base")
QT_WARNING_DISABLE_CLANG("-Winaccessible-base")

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
bool isEquals(const QMetaType& keyMetaType, const void * ptr, const void* ptr2);
#else
struct Container{
    const void *ptr;
    AutoMapAccess* access;
};

void AutoMultiMapAccess::dataStreamOut(QDataStream &s, const void *ptr){
    QtPrivate::writeAssociativeMultiContainer(s, ConstContainer{ptr, this});
}

void AutoMultiMapAccess::debugStream(QDebug &dbg, const void *ptr){
    QtPrivate::printAssociativeContainer(dbg, "QMultiMap", ConstContainer{ptr, this});
}

void* AutoMultiMapAccess::constructContainer(JNIEnv* env, void* result, const ContainerAndAccessInfo& container) {return AutoMapAccess::constructContainer(env, result, container);}
void* AutoMultiMapAccess::constructContainer(void* result, void* container) {return AutoMapAccess::constructContainer(result, container);}
#endif


AutoMultiMapAccess::AutoMultiMapAccess(const AutoMultiMapAccess & other)
    : AbstractMultiMapAccess(), AutoMapAccess(other)
{
}

AutoMultiMapAccess::~AutoMultiMapAccess(){}

AutoMultiMapAccess::AutoMultiMapAccess(
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
    : AbstractMultiMapAccess(),
      AutoMapAccess(
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
{
}

void* AutoMultiMapAccess::constructContainer(JNIEnv*env, void* result, const ConstContainerAndAccessInfo& container) {return AutoMapAccess::constructContainer(env, result, container);}
void* AutoMultiMapAccess::constructContainer(void* result) {return AutoMapAccess::constructContainer(result);}
void* AutoMultiMapAccess::constructContainer(void* result, const void* container) {return AutoMapAccess::constructContainer(result, container);}
void AutoMultiMapAccess::assign(void* container, const void* other){AutoMapAccess::assign(container, other);}
void AutoMultiMapAccess::assign(JNIEnv *env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other){AutoMapAccess::assign(env, container, other);}
bool AutoMultiMapAccess::destructContainer(void* container) {return AutoMapAccess::destructContainer(container);}
int AutoMultiMapAccess::registerContainer(const QByteArray& containerTypeName) {return AutoMapAccess::registerContainer(containerTypeName);}
void AutoMultiMapAccess::dispose() {delete this;}
const QMetaType& AutoMultiMapAccess::keyMetaType() {return AutoMapAccess::keyMetaType();}
const QMetaType& AutoMultiMapAccess::valueMetaType() {return AutoMapAccess::valueMetaType();}
AbstractContainerAccess::DataType AutoMultiMapAccess::keyType() {return AutoMapAccess::keyType();}
AbstractContainerAccess::DataType AutoMultiMapAccess::valueType() {return AutoMapAccess::valueType();}
AbstractContainerAccess* AutoMultiMapAccess::keyNestedContainerAccess() {return AutoMapAccess::keyNestedContainerAccess();}
AbstractContainerAccess* AutoMultiMapAccess::valueNestedContainerAccess() {return AutoMapAccess::valueNestedContainerAccess();}
bool AutoMultiMapAccess::hasKeyNestedContainerAccess() {return AutoMapAccess::hasKeyNestedContainerAccess();}
bool AutoMultiMapAccess::hasKeyNestedPointers() {return AutoMapAccess::hasKeyNestedPointers();}
bool AutoMultiMapAccess::hasValueNestedContainerAccess() {return AutoMapAccess::hasValueNestedContainerAccess();}
bool AutoMultiMapAccess::hasValueNestedPointers() {return AutoMapAccess::hasValueNestedPointers();}
const QSharedPointer<AbstractContainerAccess>& AutoMultiMapAccess::sharedKeyNestedContainerAccess() {return AutoMapAccess::sharedKeyNestedContainerAccess();}
const QSharedPointer<AbstractContainerAccess>& AutoMultiMapAccess::sharedValueNestedContainerAccess() {return AutoMapAccess::sharedValueNestedContainerAccess();}
void AutoMultiMapAccess::clear(JNIEnv *env, const ContainerInfo& container) {AutoMapAccess::clear(env, container);}
jboolean AutoMultiMapAccess::contains(JNIEnv *env, const void* container, jobject key) {return AutoMapAccess::contains(env, container, key);}
jint AutoMultiMapAccess::count(JNIEnv *env, const void* container, jobject key) {return AutoMapAccess::count(env, container, key);}
jobject AutoMultiMapAccess::begin(JNIEnv *env, const ExtendedContainerInfo& container) {return AutoMapAccess::begin(env, container);}
jobject AutoMultiMapAccess::end(JNIEnv *env, const ExtendedContainerInfo& container) {return AutoMapAccess::end(env, container);}
jobject AutoMultiMapAccess::find(JNIEnv *env, const ExtendedContainerInfo& container, jobject key) {return AutoMapAccess::find(env, container, key);}
jobject AutoMultiMapAccess::constBegin(JNIEnv *env, const ConstExtendedContainerInfo& container) {return AutoMapAccess::constBegin(env, container);}
jobject AutoMultiMapAccess::constEnd(JNIEnv *env, const ConstExtendedContainerInfo& container) {return AutoMapAccess::constEnd(env, container);}
jobject AutoMultiMapAccess::constFind(JNIEnv *env, const ConstExtendedContainerInfo& container, jobject key) {return AutoMapAccess::constFind(env, container, key);}
jobject AutoMultiMapAccess::first(JNIEnv *env, const void* container) {return AutoMapAccess::first(env, container);}
jobject AutoMultiMapAccess::firstKey(JNIEnv *env, const void* container) {return AutoMapAccess::firstKey(env, container);}
bool AutoMultiMapAccess::isDetached(const void* container){ return AutoMapAccess::isDetached(container); }
void AutoMultiMapAccess::detach(const ContainerInfo& container){ AutoMapAccess::detach(container); }
bool AutoMultiMapAccess::isSharedWith(const void* container, const void* container2){ return AutoMapAccess::isSharedWith(container, container2); }
void AutoMultiMapAccess::swap(JNIEnv *env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){AutoMapAccess::swap(env, container, container2);}
const QObject* AutoMultiMapAccess::getOwner(const void* container){ return AutoMapAccess::getOwner(container); }
bool AutoMultiMapAccess::hasOwnerFunction(){ return AutoMapAccess::hasOwnerFunction(); }

void AutoMultiMapAccess::insert(JNIEnv *env, const ContainerInfo& container, jobject key, jobject value){
    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)        
        detach(container);
        QMapDataBase ** map = reinterpret_cast<QMapDataBase **>(container.container);
        QMapDataBase*& d = *map;


        Node *n = rootNode(map);
        Node *y = endNode(map);
        bool  left = true;
        while (n) {
            y = n;
            left = !qMapLessThanKey(*n, akey);
            n = left ? n->left : n->right;
        }
        Node* newNode = d->createNode(int(m_size), int(m_align), y, left);
        char* newNodeData = reinterpret_cast<char*>(newNode);
        m_keyMetaType.construct(newNodeData + m_offset1, akey);
        m_valueMetaType.construct(newNodeData + m_offset2);
        assignValue(env, value, newNode);
#else
        insertOrAssign(AutoMapAccess::detach(*reinterpret_cast<MapDataPointer*>(container.container)), akey, env, value);
#endif
    }
}

IsBiContainerFunction AutoMultiMapAccess::getIsBiContainerFunction(){
    return ContainerAPI::getAsQMultiMap;
}

bool AutoMultiMapAccess::isMulti() const{
    return true;
}

std::unique_ptr<AbstractMapAccess::KeyValueIterator> AutoMultiMapAccess::keyValueIterator(const void* container) { return AutoMapAccess::keyValueIterator(container); }
jobject AutoMultiMapAccess::key(JNIEnv *env, const void* container, jobject value, jobject defaultKey) { return AutoMapAccess::key(env, container, value, defaultKey); }
ContainerAndAccessInfo AutoMultiMapAccess::keys(JNIEnv *env, const ConstContainerInfo& container) {return AutoMapAccess::keys(env, container);}
ContainerAndAccessInfo AutoMultiMapAccess::keys(JNIEnv *env, const ConstContainerInfo& container, jobject value) {return AutoMapAccess::keys(env, container, value);}
jobject AutoMultiMapAccess::last(JNIEnv *env, const void* container) {return AutoMapAccess::last(env, container);}
jobject AutoMultiMapAccess::lastKey(JNIEnv *env, const void* container) {return AutoMapAccess::lastKey(env, container);}
jboolean AutoMultiMapAccess::equal(JNIEnv *env, const void* container, jobject other) {return AutoMapAccess::equal(env, container, other);}
jint AutoMultiMapAccess::remove(JNIEnv *env, const ContainerInfo& container, jobject key) {return AutoMapAccess::remove(env, container, key);}
jint AutoMultiMapAccess::size(JNIEnv *env, const void* container) {return AutoMapAccess::size(env, container);}
jobject AutoMultiMapAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {return AutoMapAccess::take(env, container, key);}
jobject AutoMultiMapAccess::constLowerBound(JNIEnv *env, const ConstExtendedContainerInfo& container, jobject key) {return AutoMapAccess::constLowerBound(env, container, key);}
jobject AutoMultiMapAccess::constUpperBound(JNIEnv *env, const ConstExtendedContainerInfo& container, jobject key) {return AutoMapAccess::constUpperBound(env, container, key);}
jobject AutoMultiMapAccess::value(JNIEnv *env, const void* container, jobject key,jobject defaultValue) {return AutoMapAccess::value(env, container, key, defaultValue);}
ContainerAndAccessInfo AutoMultiMapAccess::values(JNIEnv *env, const ConstContainerInfo& container) {return AutoMapAccess::values(env, container);}
bool AutoMultiMapAccess::keyLessThan(JNIEnv *env, jobject k1, jobject k2) {return AutoMapAccess::keyLessThan(env, k1, k2);}
AbstractMultiMapAccess* AutoMultiMapAccess::clone() {return new AutoMultiMapAccess(*this);}

ContainerAndAccessInfo AutoMultiMapAccess::uniqueKeys(JNIEnv *env, const ConstContainerInfo& container)
{
    ContainerAndAccessInfo result;
    AbstractListAccess* listAccess = dynamic_cast<AbstractListAccess*>(ContainerAccessAPI::createContainerAccess(
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
        QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container.container);
        QMapDataBase* d = *map;
        Node *firstNode(beginNode(map));
        Node *lastNode(endNode(map));
        listAccess->reserve(env, result, d->size);
        while(firstNode != lastNode){
            void* key1 = reinterpret_cast<char*>(firstNode)+m_offset1;
            if(listAccess->append(result.container, key1)){
                idx++;
            }else{
                listAccess->insert(env, result, idx++, 1, nodeKey(env, firstNode));
            }
            firstNode = firstNode->nextNode();
            while(firstNode != lastNode && !qMapLessThanKey(key1, *firstNode)){
                firstNode = firstNode->nextNode();
            }
        }
#else
        if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container.container)){
            listAccess->reserve(env, result, d->size);
            node_iterator end1 = AutoMapAccess::end(*d);
            node_iterator iter1 = AutoMapAccess::begin(*d);
            jvalue jv;
            while(iter1!=end1){
                jv.l = nullptr;
                const void* key = iter1->data(m_offset1);
                if(listAccess->append(result.container, key)){
                    idx++;
                }else{
                    m_keyInternalToExternalConverter(env, nullptr, key, jv, true);
                    listAccess->insert(env, result, idx++, 1, jv.l);
                }
                ++iter1;
                while(iter1!=end1){
                    const void* key2 = iter1->data(m_offset1);
                    if(m_keyMetaType.compare(key, key2)!=0)
                        break;
                    ++iter1;
                }
            }
        }
#endif
    }
    return result;
}

void AutoMultiMapAccess::unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other)
{
    if (ContainerAPI::getAsQMultiMap(env, other.object, keyMetaType(), valueMetaType(), other.container, other.access)
            || ContainerAPI::getAsQMap(env, other.object, keyMetaType(), valueMetaType(), other.container, other.access)) {
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        QMapDataBase ** map = reinterpret_cast<QMapDataBase **>(container.container);
        QMapDataBase*& d = *map;
        QMapDataBase *const* map2 = reinterpret_cast<QMapDataBase *const*>(other.container);
        Node *firstNode2(beginNode(map2));
        Node *lastNode2(endNode(map2));
        while (firstNode2 != lastNode2) {
            void* key2 = reinterpret_cast<char*>(firstNode2)+m_offset1;
            void* value2 = reinterpret_cast<char*>(firstNode2)+m_offset2;

            Node *n = rootNode(map);
            Node *y = endNode(map);
            bool  left = true;
            while (n) {
                y = n;
                left = !qMapLessThanKey(*n, key2);
                n = left ? n->left : n->right;
            }
            Node* newNode = d->createNode(int(m_size), int(m_align), y, left);
            char* newNodeData = reinterpret_cast<char*>(newNode);
            m_keyMetaType.construct(newNodeData + m_offset1, key2);
            m_valueMetaType.construct(newNodeData + m_offset2, value2);
            firstNode2 = firstNode2->nextNode();
        }
#else
        MapData& d = AutoMapAccess::detach(*reinterpret_cast<MapDataPointer*>(container.container));
        if(const MapDataPointer& d2 = *reinterpret_cast<const MapDataPointer*>(other.container)){
            node_iterator end2 = AutoMapAccess::end(*d2);
            node_iterator iter = AutoMapAccess::begin(*d2);
            while(iter!=end2){
                AutoMapAccess::insertOrAssign(d, iter->data(m_offset1), iter->data(m_offset2));
                ++iter;
            }
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

ContainerAndAccessInfo AutoMultiMapAccess::values(JNIEnv *env, const ConstContainerInfo& container, jobject key)
{
    ContainerAndAccessInfo result;
    jvalue jv;
    jv.l = key;
    QtJambiScope scope;
    void* akey = nullptr;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        AbstractListAccess* listAccess = dynamic_cast<AbstractListAccess*>(ContainerAccessAPI::createContainerAccess(
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
            jint idx = listAccess->size(env, result.container);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container.container);
            QMapDataBase* d = *map;
            if(Node *firstNode = findNode(d, akey)){
                Node *lastNode(endNode(map));
                do{
                    if(listAccess->append(result.container, reinterpret_cast<char*>(firstNode)+m_offset2)){
                        idx++;
                    }else{
                        listAccess->insert(env, result, idx++, 1, nodeValue(env, firstNode));
                    }
                    firstNode = firstNode->nextNode();
                }while(firstNode != lastNode && !qMapLessThanKey(akey, *firstNode));
            }
#else
            if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container.container)){
                node_iterator i = AutoMapAccess::find(*d, akey);
                node_iterator begin1 = AutoMapAccess::begin(*d);
                node_iterator end1 = AutoMapAccess::end(*d);
                if(i != end1){
                    do{
                        if(i!=begin1){
                            --i;
                            if(m_keyMetaType.compare(akey, i->data(m_offset1))!=0){
                                ++i;
                                break;
                            }
                        }else{
                            break;
                        }
                    }while(true);
                }
                while(i != end1){
                    jv.l = nullptr;
                    if(listAccess->append(result.container, i->data(m_offset2))){
                        idx++;
                    }else{
                        m_valueInternalToExternalConverter(env, nullptr, i->data(m_offset2), jv, true);
                        listAccess->insert(env, result, idx++, 1, jv.l);
                    }
                    ++i;
                    if(i != end1){
                        if(m_keyMetaType.compare(akey, i->data(m_offset1))!=0){
                            break;
                        }
                    }
                }
            }
#endif
        }
    }
    return result;
}

jboolean AutoMultiMapAccess::contains(JNIEnv *env, const void* container, jobject key, jobject value) {
    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        jv.l = value;
        void* avalue = nullptr;
        if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
            QMapDataBase* d = *map;
            if(Node *firstNode = findNode(d, akey)){
                Node *lastNode(endNode(map));
                do{
                    if(isEquals(m_valueMetaType, avalue, reinterpret_cast<char*>(firstNode)+m_offset2))
                        return true;
                    firstNode = firstNode->nextNode();
                }while(firstNode != lastNode && !qMapLessThanKey(akey, *firstNode));
            }
#else
            if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container)){
                node_iterator i = AutoMapAccess::find(*d, akey);
                node_iterator end1 = AutoMapAccess::end(*d);
                while(i != end1){
                    if(m_valueMetaType.compare(avalue, i->data(m_offset2))==0)
                        return true;
                    ++i;
                    if(m_keyMetaType.compare(akey, i->data(m_offset1))!=0)
                        break;
                }
            }
#endif
        }
    }
    return false;
}

jint AutoMultiMapAccess::count(JNIEnv *env, const void* container, jobject key, jobject value)
{
    jint c = 0;
    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        jv.l = value;
        void* avalue = nullptr;
        if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container);
            QMapDataBase* d = *map;
            if(Node *firstNode = findNode(d, akey)){
                Node *lastNode(endNode(map));
                do{
                    if(isEquals(m_valueMetaType, avalue, reinterpret_cast<char*>(firstNode)+m_offset2))
                        ++c;
                    firstNode = firstNode->nextNode();
                }while(firstNode != lastNode && !qMapLessThanKey(akey, *firstNode));
            }
#else
            if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container)){
                node_iterator i = AutoMapAccess::find(*d, akey);
                node_iterator end1 = AutoMapAccess::end(*d);
                while(i != end1){
                    if(m_valueMetaType.compare(avalue, i->data(m_offset2))==0)
                        ++c;
                    ++i;
                    if(m_keyMetaType.compare(akey, i->data(m_offset1))!=0)
                        break;
                }
            }
#endif
        }
    }
    return c;
}

jobject AutoMultiMapAccess::find(JNIEnv *env, const ExtendedContainerInfo& container, jobject key, jobject value)
{
    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        jv.l = value;
        void* avalue = nullptr;
        if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            detach(container);
            QMapDataBase ** map = reinterpret_cast<QMapDataBase **>(container.container);
            QMapDataBase* d = *map;
            if(Node *firstNode = findNode(d, akey)){
                Node *lastNode(endNode(map));
                do{
                    if(isEquals(m_valueMetaType, avalue, reinterpret_cast<char*>(firstNode)+m_offset2))
                        return createIterator(env, container.nativeId, new void*(firstNode));
                    firstNode = firstNode->nextNode();
                }while(firstNode != lastNode && !qMapLessThanKey(akey, *firstNode));
            }
#else
            if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container.container)){
                node_iterator i = AutoMapAccess::find(*d, akey);
                node_iterator end1 = AutoMapAccess::end(*d);
                while(i != end1){
                    if(m_valueMetaType.compare(avalue, i->data(m_offset2))==0)
                        return createIterator(env, container.nativeId, new iterator(i, m_offset1, m_offset1));
                    ++i;
                    if(m_keyMetaType.compare(akey, i->data(m_offset1))!=0)
                        break;
                }
            }
#endif
        }
    }
    return end(env, container);
}

jobject AutoMultiMapAccess::constFind(JNIEnv *env, const ConstExtendedContainerInfo& container, jobject key, jobject value)
{
    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        jv.l = value;
        void* avalue = nullptr;
        if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            QMapDataBase *const* map = reinterpret_cast<QMapDataBase *const*>(container.container);
            QMapDataBase* d = *map;
            if(Node *firstNode = findNode(d, akey)){
                Node *lastNode(endNode(map));
                do{
                    if(isEquals(m_valueMetaType, avalue, reinterpret_cast<char*>(firstNode)+m_offset2))
                        return createConstIterator(env, container.nativeId, new void*(firstNode));
                    firstNode = firstNode->nextNode();
                }while(firstNode != lastNode && !qMapLessThanKey(akey, *firstNode));
            }
#else
            if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container.container)){
                node_iterator i = AutoMapAccess::find(*d, akey);
                node_iterator end1 = AutoMapAccess::end(*d);
                while(i != end1){
                    if(m_valueMetaType.compare(avalue, i->data(m_offset2))==0)
                        return createConstIterator(env, container.nativeId, new iterator(i, m_offset1, m_offset2));
                    ++i;
                    if(m_keyMetaType.compare(akey, i->data(m_offset1))!=0)
                        break;
                }
            }
#endif
        }
    }
    return constEnd(env, container);
}

jint AutoMultiMapAccess::remove(JNIEnv *env, const ContainerInfo& container, jobject key, jobject value)
{
    jint c = 0;
    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        jv.l = value;
        void* avalue = nullptr;
        if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            detach(container);
            QMapDataBase ** map = reinterpret_cast<QMapDataBase **>(container.container);
            QMapDataBase*& d = *map;
            bool found;
            do{
                found = false;
                if(Node *firstNode = findNode(d, akey)){
                    Node *lastNode(endNode(map));
                    do{
                        if(isEquals(m_valueMetaType, avalue, reinterpret_cast<char*>(firstNode)+m_offset2)){
                            ++c;
                            deleteNode(d, firstNode);
                            found = true;
                            break;
                        }
                        firstNode = firstNode->nextNode();
                    }while(firstNode != lastNode && !qMapLessThanKey(akey, *firstNode));
                }
            }while(found);
#else
            if(MapDataPointer& d = *reinterpret_cast<MapDataPointer*>(container.container)){
                if(!d.isShared()){
                    c = jint(erase(*d, akey, avalue));
                }else{
                    MapData *newData = createMapData();
                    c = copyIfNotEquivalentTo(*newData, *d, akey, avalue);
                    d.reset(newData);
                }
            }
#endif
        }
    }
    return c;
}

void AutoMultiMapAccess::replace(JNIEnv *env, const ContainerInfo& container, jobject key, jobject value) {
    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        detach(container);
        QMapDataBase ** map = reinterpret_cast<QMapDataBase **>(container.container);
        QMapDataBase*& d = *map;

        {
            Node *n = findNode(d, akey);
            Node *y = endNode(map);
            if(n!=y){
                assignValue(env, value, n);
            }else{
                n = rootNode(map);
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
        }
#else
        MapData& data = AutoMapAccess::detach(*reinterpret_cast<MapDataPointer*>(container.container));
        AutoMapAccess::node_iterator iter = AutoMapAccess::find(data, akey);
        if(iter!=AutoMapAccess::end(data)){
            void* target = iter->data(m_offset2);
            jv.l = value;
            m_valueExternalToInternalConverter(env, &scope, jv, target, jValueType::l);
        }else{
            insertOrAssign(data, akey, env, value);
        }
#endif
    }
}

size_t AutoMultiMapAccess::sizeOf() {
    return sizeof(QMultiMap<char,char>);
}

ushort AutoMultiMapAccess::alignOf() const{
    return ushort(sizeof(QMultiMap<char,char>));
}

KeyPointerRCAutoMultiMapAccess::KeyPointerRCAutoMultiMapAccess(KeyPointerRCAutoMultiMapAccess& other)
    : AutoMultiMapAccess(other), ReferenceCountingSetContainer() {}

AbstractMultiMapAccess* KeyPointerRCAutoMultiMapAccess::clone(){
    return new KeyPointerRCAutoMultiMapAccess(*this);
}

void KeyPointerRCAutoMultiMapAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
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

void KeyPointerRCAutoMultiMapAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    AutoMultiMapAccess::swap(env, container, container2);
    if(KeyPointerRCAutoMultiMapAccess* access = dynamic_cast<KeyPointerRCAutoMultiMapAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void KeyPointerRCAutoMultiMapAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    AutoMultiMapAccess::assign(env, container, container2);
    if(KeyPointerRCAutoMultiMapAccess* access = dynamic_cast<KeyPointerRCAutoMultiMapAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void KeyPointerRCAutoMultiMapAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    AutoMultiMapAccess::clear(env, container);
    clearRC(env, container.object);
}

void KeyPointerRCAutoMultiMapAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    AutoMultiMapAccess::insert(env, container, key, value);
    addRC(env, container.object, key);
}

jint KeyPointerRCAutoMultiMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    jint result = AutoMultiMapAccess::remove(env, container, key);
    removeRC(env, container.object, key, result);
    return result;
}

jint KeyPointerRCAutoMultiMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    jint result = AutoMultiMapAccess::remove(env, container, key, value);
    removeRC(env, container.object, key, result);
    return result;
}

jobject KeyPointerRCAutoMultiMapAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = AutoMultiMapAccess::take(env, container, key);
    removeRC(env, container.object, key);
    return result;
}

void KeyPointerRCAutoMultiMapAccess::replace(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    AutoMultiMapAccess::replace(env, container, key, value);
    addRC(env, container.object, key);
}

void KeyPointerRCAutoMultiMapAccess::unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) {
    AutoMultiMapAccess::unite(env, container, other);
    updateRC(env, container);
}

ValuePointerRCAutoMultiMapAccess::ValuePointerRCAutoMultiMapAccess(ValuePointerRCAutoMultiMapAccess& other)
    : AutoMultiMapAccess(other), ReferenceCountingSetContainer() {}

AbstractMultiMapAccess* ValuePointerRCAutoMultiMapAccess::clone(){
    return new ValuePointerRCAutoMultiMapAccess(*this);
}

void ValuePointerRCAutoMultiMapAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
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

void ValuePointerRCAutoMultiMapAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    AutoMultiMapAccess::swap(env, container, container2);
    if(ValuePointerRCAutoMultiMapAccess* access = dynamic_cast<ValuePointerRCAutoMultiMapAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void ValuePointerRCAutoMultiMapAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    AutoMultiMapAccess::assign(env, container, container2);
    if(ValuePointerRCAutoMultiMapAccess* access = dynamic_cast<ValuePointerRCAutoMultiMapAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void ValuePointerRCAutoMultiMapAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    AutoMultiMapAccess::clear(env, container);
    clearRC(env, container.object);
}

void ValuePointerRCAutoMultiMapAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    AutoMultiMapAccess::insert(env, container, key, value);
    addRC(env, container.object, value);
}

jint ValuePointerRCAutoMultiMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    ContainerAndAccessInfo oldValues = AutoMultiMapAccess::values(env, container, key);
    jint result = AutoMultiMapAccess::remove(env, container, key);
    if(result>0){
        jobject iter = Java::Runtime::Collection::iterator(env, oldValues.object);
        while(Java::Runtime::Iterator::hasNext(env, iter)){
            jobject value = Java::Runtime::Iterator::next(env, iter);
            if(Java::Runtime::Collection::size(env, AutoMultiMapAccess::keys(env, container, value).object)==0){
                removeRC(env, container.object, value);
            }
        }
    }
    return result;
}

jobject ValuePointerRCAutoMultiMapAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = AutoMultiMapAccess::take(env, container, key);
    if(Java::Runtime::Collection::size(env, AutoMultiMapAccess::keys(env, container, result).object)==0){
        removeRC(env, container.object, result);
    }
    return result;
}

void ValuePointerRCAutoMultiMapAccess::replace(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    jobject oldValue = AutoMultiMapAccess::value(env, container.container, key, nullptr);
    AutoMultiMapAccess::replace(env, container, key, value);
    removeRC(env, container.object, oldValue);
    addRC(env, container.object, value);
}

void ValuePointerRCAutoMultiMapAccess::unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) {
    AutoMultiMapAccess::unite(env, container, other);
    updateRC(env, container);
}

jint ValuePointerRCAutoMultiMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value){
    jint result = AutoMultiMapAccess::remove(env, container, key, value);
    removeRC(env, container.object, value, result);
    return result;
}

PointersRCAutoMultiMapAccess::PointersRCAutoMultiMapAccess(PointersRCAutoMultiMapAccess& other)
    : AutoMultiMapAccess(other), ReferenceCountingMultiMapContainer(other) {}

AbstractMultiMapAccess* PointersRCAutoMultiMapAccess::clone(){
    return new PointersRCAutoMultiMapAccess(*this);
}

void PointersRCAutoMultiMapAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
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

void PointersRCAutoMultiMapAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    AutoMultiMapAccess::swap(env, container, container2);
    if(PointersRCAutoMultiMapAccess* access = dynamic_cast<PointersRCAutoMultiMapAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void PointersRCAutoMultiMapAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    AutoMultiMapAccess::assign(env, container, container2);
    if(PointersRCAutoMultiMapAccess* access = dynamic_cast<PointersRCAutoMultiMapAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void PointersRCAutoMultiMapAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    AutoMultiMapAccess::clear(env, container);
    clearRC(env, container.object);
}

void PointersRCAutoMultiMapAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    AutoMultiMapAccess::insert(env, container, key, value);
    putRC(env, container.object, key, value);
}

void PointersRCAutoMultiMapAccess::replace(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    AutoMultiMapAccess::replace(env, container, key, value);
    removeRC(env, container.object, key);
    putRC(env, container.object, key, value);
}

void PointersRCAutoMultiMapAccess::unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) {
    AutoMultiMapAccess::unite(env, container, other);
    updateRC(env, container);
}

jint PointersRCAutoMultiMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    jint result = AutoMultiMapAccess::remove(env, container, key);
    removeRC(env, container.object, key, 1);
    return result;
}

jint PointersRCAutoMultiMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    jint result = AutoMultiMapAccess::remove(env, container, key, value);
    removeRC(env, container.object, key, value, result);
    return result;
}

jobject PointersRCAutoMultiMapAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = AutoMultiMapAccess::take(env, container, key);
    removeRC(env, key, result);
    return result;
}

NestedPointersRCAutoMultiMapAccess::NestedPointersRCAutoMultiMapAccess(NestedPointersRCAutoMultiMapAccess& other)
    : AutoMultiMapAccess(other), ReferenceCountingSetContainer() {}

AbstractMultiMapAccess* NestedPointersRCAutoMultiMapAccess::clone(){
    return new NestedPointersRCAutoMultiMapAccess(*this);
}

void NestedPointersRCAutoMultiMapAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
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

void NestedPointersRCAutoMultiMapAccess::swap(JNIEnv * env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){
    AutoMultiMapAccess::swap(env, container, container2);
    if(NestedPointersRCAutoMultiMapAccess* access = dynamic_cast<NestedPointersRCAutoMultiMapAccess*>(container2.access)){
        if(access!=this)
            swapRC(env, container, container2);
    }else{
        updateRC(env, container);
    }
}

void NestedPointersRCAutoMultiMapAccess::assign(JNIEnv * env, const ContainerInfo& container, const ConstContainerAndAccessInfo& container2){
    AutoMultiMapAccess::assign(env, container, container2);
    if(NestedPointersRCAutoMultiMapAccess* access = dynamic_cast<NestedPointersRCAutoMultiMapAccess*>(container2.access)){
        if(access!=this)
            assignRC(env, container.object, container2.object);
    }else{
        updateRC(env, container);
    }
}

void NestedPointersRCAutoMultiMapAccess::clear(JNIEnv * env, const ContainerInfo& container) {
    AutoMultiMapAccess::clear(env, container);
    clearRC(env, container.object);
}

void NestedPointersRCAutoMultiMapAccess::insert(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    AutoMultiMapAccess::insert(env, container, key, value);
    addNestedValueRC(env, container.object, keyType(), hasKeyNestedPointers(), key);
    addNestedValueRC(env, container.object, valueType(), hasValueNestedPointers(), value);
}

void NestedPointersRCAutoMultiMapAccess::replace(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    AutoMultiMapAccess::replace(env, container, key, value);
    updateRC(env, container);
}


void NestedPointersRCAutoMultiMapAccess::unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) {
    AutoMultiMapAccess::unite(env, container, other);
    updateRC(env, container);
}

jint NestedPointersRCAutoMultiMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    jint result = AutoMultiMapAccess::remove(env, container, key);
    if(result>0){
        updateRC(env, container);
    }
    return result;
}

jint NestedPointersRCAutoMultiMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    jint result = AutoMultiMapAccess::remove(env, container, key, value);
    if(result>0){
        updateRC(env, container);
    }
    return result;
}

jobject NestedPointersRCAutoMultiMapAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = AutoMultiMapAccess::take(env, container, key);
    updateRC(env, container);
    return result;
}
