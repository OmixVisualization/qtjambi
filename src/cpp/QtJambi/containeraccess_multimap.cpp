/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
#include "qtjambi_cast.h"
#include "containeraccess_export_multimap.h"
#include "containeraccess_export_list.h"
#include "containeraccess_export_bytearraylist.h"
#include "containeraccess_export_stringlist.h"

QT_WARNING_DISABLE_GCC("-Winaccessible-base")
QT_WARNING_DISABLE_CLANG("-Winaccessible-base")

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

AutoMultiMapAccess::AutoMultiMapAccess(const AutoMultiMapAccess & other)
    : AbstractMultiMapAccess(), AutoMapAccess(other)
{
}

AutoMultiMapAccess::~AutoMultiMapAccess(){}

AutoMultiMapAccess::AutoMultiMapAccess(
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
    : AbstractMultiMapAccess(),
      AutoMapAccess(
          keyMetaType,
          keyHashFunction,
          keyInternalToExternalConverter,
          keyExternalToInternalConverter,
          keyNestedContainerAccess,
          keyOwnerFunction,
          keyDataType,
          valueMetaType,
          valueHashFunction,
          valueInternalToExternalConverter,
          valueExternalToInternalConverter,
          valueNestedContainerAccess,
          valueOwnerFunction,
          valueDataType)
{
}

void* AutoMultiMapAccess::createContainer(const void* copy){
    return AutoMapAccess::createContainer(copy);
}

void AutoMultiMapAccess::deleteContainer(void* deleteContainer){
    return AutoMapAccess::deleteContainer(deleteContainer);
}

void AutoMultiMapAccess::dispose() {delete this;}

void AutoMultiMapAccess::insert(JNIEnv *env, const ContainerInfo& container, jobject key, jobject value){
    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        insertOrAssign(AutoMapAccess::detach(*reinterpret_cast<MapDataPointer*>(container.container)), akey, env, value);
    }
}

void AutoMultiMapAccess::insert(void* container, const void* key, const void* value){
    insertOrAssign(AutoMapAccess::detach(*reinterpret_cast<MapDataPointer*>(container)), key, value);
}

IsBiContainerFunction AutoMultiMapAccess::getIsBiContainerFunction(){
    return ContainerAPI::getAsQMultiMap;
}

bool AutoMultiMapAccess::isMulti() const{
    return true;
}

AbstractMultiMapAccess* AutoMultiMapAccess::clone() {
    return new AutoMultiMapAccess(*this);
}

void* AutoMultiMapAccess::constructContainer(JNIEnv* env, void* result, const ContainerAndAccessInfo& container) {return AutoMapAccess::constructContainer(env, result, container);}
void* AutoMultiMapAccess::constructContainer(void* result, void* container) {return AutoMapAccess::constructContainer(result, container);}
void* AutoMultiMapAccess::constructContainer(JNIEnv*env, void* result, const ConstContainerAndAccessInfo& container) {return AutoMapAccess::constructContainer(env, result, container);}
void* AutoMultiMapAccess::constructContainer(void* result) {return AutoMapAccess::constructContainer(result);}
void* AutoMultiMapAccess::constructContainer(void* result, const void* container) {return AutoMapAccess::constructContainer(result, container);}
void AutoMultiMapAccess::assign(void* container, const void* other){AutoMapAccess::assign(container, other);}
void AutoMultiMapAccess::assign(JNIEnv *env, const ContainerInfo& container, const ConstContainerAndAccessInfo& other){AutoMapAccess::assign(env, container, other);}
bool AutoMultiMapAccess::destructContainer(void* container) {return AutoMapAccess::destructContainer(container);}
QMetaType AutoMultiMapAccess::registerContainer(QByteArrayView containerTypeName) {return AutoMapAccess::registerContainer(containerTypeName);}
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
bool AutoMultiMapAccess::contains(const void* container, const void* key) {return AutoMapAccess::contains(container, key);}
jobject AutoMultiMapAccess::first(JNIEnv *env, const void* container) {return AutoMapAccess::first(env, container);}
jobject AutoMultiMapAccess::firstKey(JNIEnv *env, const void* container) {return AutoMapAccess::firstKey(env, container);}
bool AutoMultiMapAccess::isDetached(const void* container){ return AutoMapAccess::isDetached(container); }
void AutoMultiMapAccess::detach(const ContainerInfo& container){ AutoMapAccess::detach(container); }
bool AutoMultiMapAccess::isSharedWith(const void* container, const void* container2){ return AutoMapAccess::isSharedWith(container, container2); }
void AutoMultiMapAccess::swap(JNIEnv *env, const ContainerInfo& container, const ContainerAndAccessInfo& container2){AutoMapAccess::swap(env, container, container2);}
const QObject* AutoMultiMapAccess::getOwner(const void* container){ return AutoMapAccess::getOwner(container); }
bool AutoMultiMapAccess::hasOwnerFunction(){ return AutoMapAccess::hasOwnerFunction(); }
std::unique_ptr<AbstractMapAccess::KeyValueIterator> AutoMultiMapAccess::keyValueIterator(const void* container) { return AutoMapAccess::keyValueIterator(container); }
std::unique_ptr<AbstractMapAccess::KeyValueIterator> AutoMultiMapAccess::keyValueIterator(void* container) { return AutoMapAccess::keyValueIterator(container); }
jobject AutoMultiMapAccess::key(JNIEnv *env, const void* container, jobject value, jobject defaultKey) { return AutoMapAccess::key(env, container, value, defaultKey); }
ContainerAndAccessInfo AutoMultiMapAccess::keys(JNIEnv *env, const ConstContainerInfo& container) {return AutoMapAccess::keys(env, container);}
ContainerAndAccessInfo AutoMultiMapAccess::keys(JNIEnv *env, const ConstContainerInfo& container, jobject value) {return AutoMapAccess::keys(env, container, value);}
jobject AutoMultiMapAccess::last(JNIEnv *env, const void* container) {return AutoMapAccess::last(env, container);}
jobject AutoMultiMapAccess::lastKey(JNIEnv *env, const void* container) {return AutoMapAccess::lastKey(env, container);}
jboolean AutoMultiMapAccess::equal(JNIEnv *env, const void* container, jobject other) {return AutoMapAccess::equal(env, container, other);}
qsizetype AutoMultiMapAccess::remove(JNIEnv *env, const ContainerInfo& container, jobject key) {return AutoMapAccess::remove(env, container, key);}
qsizetype AutoMultiMapAccess::size(JNIEnv *env, const void* container) {return AutoMapAccess::size(env, container);}
qsizetype AutoMultiMapAccess::size(const void* container)  {return AutoMapAccess::size(container);}
jobject AutoMultiMapAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {return AutoMapAccess::take(env, container, key);}
jobject AutoMultiMapAccess::value(JNIEnv *env, const void* container, jobject key,jobject defaultValue) {return AutoMapAccess::value(env, container, key, defaultValue);}
const void* AutoMultiMapAccess::value(const void* container, const void* key,const void* defaultValue) {return AutoMapAccess::value(container, key, defaultValue);}
ContainerAndAccessInfo AutoMultiMapAccess::values(JNIEnv *env, const ConstContainerInfo& container) {return AutoMapAccess::values(env, container);}
bool AutoMultiMapAccess::keyLessThan(JNIEnv *env, jobject k1, jobject k2) {return AutoMapAccess::keyLessThan(env, k1, k2);}

jobject AutoMultiMapAccess::createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiMapAccess>, const_iterator>&& iter)
{
    using Iter = std::remove_reference_t<decltype(iter)>;
    return QtJambiAPI::convertMultiMapIteratorToJavaObject(env,
                                                           new Iter(std::move(iter)),
                                                           &QtJambiAPI::deletePointer<Iter>,
                                                           new AutoAssociativeConstIteratorAccess<AutoMultiMapAccess,Iter>(m_valueInternalToExternalConverter,m_keyInternalToExternalConverter,m_keyMetaType,m_valueMetaType));
}

jobject AutoMultiMapAccess::createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiMapAccess>, iterator>&& iter)
{
    using Iter = std::remove_reference_t<decltype(iter)>;
    QSharedPointer<QtJambiLink> link = iter.storage().link();
    return QtJambiPrivate::convertMultiMapIteratorToJavaObject(env, link,
                                                               new Iter(std::move(iter)),
                                                               &QtJambiAPI::deletePointer<Iter>,
                                                               new AutoAssociativeIteratorAccess<AutoMultiMapAccess,Iter>(m_valueInternalToExternalConverter,m_valueExternalToInternalConverter,m_keyInternalToExternalConverter,m_keyMetaType,m_valueMetaType));
}

jobject AutoMultiMapAccess::createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiMapAccess>, key_value_iterator>&& iter)
{
    using Iter = std::remove_reference_t<decltype(iter)>;
    QSharedPointer<QtJambiLink> link = iter.storage().link();
    return QtJambiPrivate::convertMultiMapKeyValueIteratorToJavaObject(env, link,
                                                                       new Iter(std::move(iter)),
                                                                       &QtJambiAPI::deletePointer<Iter>,
                                                                       new AutoAssociativeIteratorAccess<AutoMultiMapAccess,Iter,AbstractSequentialConstIteratorAccess::IteratorType::key_value_iterator>(m_valueInternalToExternalConverter,m_valueExternalToInternalConverter,m_keyInternalToExternalConverter,m_keyMetaType,m_valueMetaType));
}

jobject AutoMultiMapAccess::createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiMapAccess>, const_key_value_iterator>&& iter)
{
    using Iter = std::remove_reference_t<decltype(iter)>;
    return QtJambiAPI::convertMultiMapKeyValueIteratorToJavaObject(env, new Iter(std::move(iter)),
                                                                   &QtJambiAPI::deletePointer<Iter>,
                                                                   new AutoAssociativeConstIteratorAccess<AutoMultiMapAccess,Iter,AbstractSequentialConstIteratorAccess::IteratorType::const_key_value_iterator>(m_valueInternalToExternalConverter,m_keyInternalToExternalConverter,m_keyMetaType,m_valueMetaType));
}

jobject AutoMultiMapAccess::createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiMapAccess>, key_iterator>&& iter)
{
    using Iter = std::remove_reference_t<decltype(iter)>;
    return QtJambiAPI::convertMultiMapKeyIteratorToJavaObject(env, new Iter(std::move(iter)),
                                                              &QtJambiAPI::deletePointer<Iter>,
                                                              new AutoAssociativeConstIteratorAccess<AutoMultiMapAccess,Iter,AbstractSequentialConstIteratorAccess::IteratorType::key_iterator>(m_valueInternalToExternalConverter,m_keyInternalToExternalConverter,m_keyMetaType,m_valueMetaType));
}

jobject AutoMultiMapAccess::keyBegin(JNIEnv * env, const ConstExtendedContainerInfo& container)
{
    key_iterator iter = keyBegin(container.container);
    return createIterator(env, ContainerIterator(std::move(iter), this, container));
}

jobject AutoMultiMapAccess::keyEnd(JNIEnv * env, const ConstExtendedContainerInfo& container)
{
    key_iterator iter = keyEnd(container.container);
    return createIterator(env, ContainerIterator(std::move(iter), this, container));
}

jobject AutoMultiMapAccess::keyValueBegin(JNIEnv * env, const ExtendedContainerInfo& container)
{
    key_value_iterator iter = keyValueBegin(container.container);
    return createIterator(env, ContainerIterator(std::move(iter), this, container));
}

jobject AutoMultiMapAccess::keyValueEnd(JNIEnv * env, const ExtendedContainerInfo& container)
{
    key_value_iterator iter = keyValueEnd(container.container);
    return createIterator(env, ContainerIterator(std::move(iter), this, container));
}

jobject AutoMultiMapAccess::constKeyValueBegin(JNIEnv * env, const ConstExtendedContainerInfo& container)
{
    const_key_value_iterator iter = constKeyValueBegin(container.container);
    return createIterator(env, ContainerIterator(std::move(iter), this, container));
}

jobject AutoMultiMapAccess::constKeyValueEnd(JNIEnv * env, const ConstExtendedContainerInfo& container)
{
    const_key_value_iterator iter = constKeyValueEnd(container.container);
    return createIterator(env, ContainerIterator(std::move(iter), this, container));
}

jobject AutoMultiMapAccess::begin(JNIEnv *env, const ExtendedContainerInfo& container) {
    iterator iter = begin(container.container);
    return createIterator(env, ContainerIterator(std::move(iter), this, container));
}

jobject AutoMultiMapAccess::end(JNIEnv *env, const ExtendedContainerInfo& container) {
    iterator iter = end(container.container);
    return createIterator(env, ContainerIterator(std::move(iter), this, container));
}

jobject AutoMultiMapAccess::find(JNIEnv *env, const ExtendedContainerInfo& container, jobject key) {
    if(MapDataPointer& d = *reinterpret_cast<MapDataPointer*>(container.container)){
        jvalue jv;
        jv.l = key;
        QtJambiScope scope;
        void* akey = nullptr;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            detach(d);
            iterator iter(find(*d, akey), m_offset1, m_offset2);
            return createIterator(env, ContainerIterator(std::move(iter), this, container));
        }
    }
    return end(env, container);
}

jobject AutoMultiMapAccess::constBegin(JNIEnv *env, const ConstExtendedContainerInfo& container) {
    const_iterator iter = begin(container.container);
    return createIterator(env, ContainerIterator(std::move(iter), this, container));
}

jobject AutoMultiMapAccess::constEnd(JNIEnv *env, const ConstExtendedContainerInfo& container) {
    const_iterator iter = end(container.container);
    return createIterator(env, ContainerIterator(std::move(iter), this, container));
}

jobject AutoMultiMapAccess::constFind(JNIEnv *env, const ConstExtendedContainerInfo& container, jobject key) {
    if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container.container)){
        jvalue jv;
        jv.l = key;
        QtJambiScope scope;
        void* akey = nullptr;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            const_iterator iter(find(*d, akey), m_offset1, m_offset2);
            return createIterator(env, ContainerIterator(std::move(iter), this, container));
        }
    }
    return constEnd(env, container);
}

jobject AutoMultiMapAccess::constLowerBound(JNIEnv *env, const ConstExtendedContainerInfo& container, jobject key) {
    if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container.container)){
        jvalue jv;
        jv.l = key;
        QtJambiScope scope;
        void* akey = nullptr;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            const_iterator iter(node_iterator(findLowerBound(*d, akey)), m_offset1, m_offset2);
            return createIterator(env, ContainerIterator(std::move(iter), this, container));
        }
    }
    return constEnd(env, container);
}

jobject AutoMultiMapAccess::constUpperBound(JNIEnv *env, const ConstExtendedContainerInfo& container, jobject key) {
    if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container.container)){
        jvalue jv;
        jv.l = key;
        QtJambiScope scope;
        void* akey = nullptr;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            const_iterator iter(node_iterator(findUpperBound(*d, akey)), m_offset1, m_offset2);
            return createIterator(env, ContainerIterator(std::move(iter), this, container));
        }
    }
    return constEnd(env, container);
}

jobject AutoMultiMapAccess::lowerBound(JNIEnv *env, const ExtendedContainerInfo& container, jobject key) {
    if(MapDataPointer& d = *reinterpret_cast<MapDataPointer*>(container.container)){
        jvalue jv;
        jv.l = key;
        QtJambiScope scope;
        void* akey = nullptr;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            detach(d);
            iterator iter(node_iterator(findLowerBound(*d, akey)), m_offset1, m_offset2);
            return createIterator(env, ContainerIterator(std::move(iter), this, container));
        }
    }
    return end(env, container);
}

jobject AutoMultiMapAccess::upperBound(JNIEnv *env, const ExtendedContainerInfo& container, jobject key) {
    if(MapDataPointer& d = *reinterpret_cast<MapDataPointer*>(container.container)){
        jvalue jv;
        jv.l = key;
        QtJambiScope scope;
        void* akey = nullptr;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            detach(d);
            iterator iter(node_iterator(findUpperBound(*d, akey)), m_offset1, m_offset2);
            return createIterator(env, ContainerIterator(std::move(iter), this, container));
        }
    }
    return end(env, container);
}

ContainerAndAccessInfo AutoMultiMapAccess::uniqueKeys(JNIEnv *env, const ConstContainerInfo& container)
{
    ContainerAndAccessInfo result;
    auto containerAccess = createContainerAccess(
        env,
        SequentialContainerType::QList,
        m_keyMetaType,
        m_keyMetaType.alignOf(),
        m_keyMetaType.sizeOf(),
        AbstractContainerAccess::isPointerType(m_keyMetaType),
        m_keyHashFunction,
        m_keyInternalToExternalConverter,
        m_keyExternalToInternalConverter,
        m_keyNestedContainerAccess,
        m_keyOwnerFunction
        );
    if(containerAccess && containerAccess->isList()){
        AbstractListAccess* listAccess = static_cast<AbstractListAccess*>(containerAccess);
        result.container = listAccess->createContainer();
        result.object = ContainerAPI::objectFromQList(env, result.container, listAccess);
        result.access = listAccess;
        qsizetype idx = listAccess->size(env, result.container);
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
                    if(!m_keyMetaType.equals(key, key2))
                        break;
                    ++iter1;
                }
            }
        }
    }
    return result;
}

void AutoMultiMapAccess::unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other)
{
    if (ContainerAPI::getAsQMultiMap(env, other.object, keyMetaType(), valueMetaType(), other.container, other.access)
            || ContainerAPI::getAsQMap(env, other.object, keyMetaType(), valueMetaType(), other.container, other.access)) {
        MapData& d = AutoMapAccess::detach(*reinterpret_cast<MapDataPointer*>(container.container));
        if(const MapDataPointer& d2 = *reinterpret_cast<const MapDataPointer*>(other.container)){
            node_iterator end2 = AutoMapAccess::end(*d2);
            node_iterator iter = AutoMapAccess::begin(*d2);
            while(iter!=end2){
                AutoMapAccess::insertOrAssign(d, iter->data(m_offset1), iter->data(m_offset2));
                ++iter;
            }
        }
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
        auto containerAccess = createContainerAccess(
            env,
            SequentialContainerType::QList,
            m_valueMetaType,
            m_valueMetaType.alignOf(),
            m_valueMetaType.sizeOf(),
            AbstractContainerAccess::isPointerType(m_valueMetaType),
            m_valueHashFunction,
            m_valueInternalToExternalConverter,
            m_valueExternalToInternalConverter,
            m_valueNestedContainerAccess,
            m_valueOwnerFunction
            );
        if(containerAccess && containerAccess->isList()){
            AbstractListAccess* listAccess = static_cast<AbstractListAccess*>(containerAccess);
            result.container = listAccess->createContainer();
            result.object = ContainerAPI::objectFromQList(env, result.container, listAccess);
            result.access = listAccess;
            qsizetype idx = listAccess->size(env, result.container);
            if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container.container)){
                node_iterator i = AutoMapAccess::find(*d, akey);
                node_iterator begin1 = AutoMapAccess::begin(*d);
                node_iterator end1 = AutoMapAccess::end(*d);
                if(i != end1){
                    do{
                        if(i!=begin1){
                            --i;
                            if(!m_keyMetaType.equals(akey, i->data(m_offset1))){
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
                        if(!m_keyMetaType.equals(akey, i->data(m_offset1))){
                            break;
                        }
                    }
                }
            }
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
            if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container)){
                node_iterator i = AutoMapAccess::find(*d, akey);
                node_iterator end1 = AutoMapAccess::end(*d);
                while(i != end1){
                    if(m_valueMetaType.equals(avalue, i->data(m_offset2)))
                        return true;
                    ++i;
                    if(!m_keyMetaType.equals(akey, i->data(m_offset1)))
                        break;
                }
            }
        }
    }
    return false;
}

qsizetype AutoMultiMapAccess::count(JNIEnv *env, const void* container, jobject key, jobject value)
{
    qsizetype c = 0;
    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        jv.l = value;
        void* avalue = nullptr;
        if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
            if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container)){
                node_iterator i = AutoMapAccess::find(*d, akey);
                node_iterator end1 = AutoMapAccess::end(*d);
                while(i != end1){
                    if(m_valueMetaType.equals(avalue, i->data(m_offset2)))
                        ++c;
                    ++i;
                    if(!m_keyMetaType.equals(akey, i->data(m_offset1)))
                        break;
                }
            }
        }
    }
    return c;
}

qsizetype AutoMultiMapAccess::count(JNIEnv *env, const void* container, jobject key) {
    qsizetype result = 0;
    if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container)){
        jvalue jv;
        jv.l = key;
        QtJambiScope scope;
        void* akey = nullptr;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            node_iterator i = find(*d, akey);
            node_iterator _end = end(*d);
            while(i!=_end){
                ++result;
                const void* trykey = i->data(m_offset1);
                if(!m_keyMetaType.equals(trykey, akey)){
                    --result;
                    break;
                }
                ++i;
            }
        }
    }
    return result;
}

jobject AutoMultiMapAccess::find(JNIEnv *env, const ExtendedContainerInfo& container, jobject key, jobject value)
{
    if(MapDataPointer& d = *reinterpret_cast<MapDataPointer*>(container.container)){
        jvalue jv;
        jv.l = key;
        void* akey = nullptr;
        QtJambiScope scope;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            jv.l = value;
            void* avalue = nullptr;
            if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
                detach(d);
                node_iterator i = AutoMapAccess::find(*d, akey);
                node_iterator end1 = AutoMapAccess::end(*d);
                while(i != end1){
                    if(m_valueMetaType.equals(avalue, i->data(m_offset2))){
                        iterator iter(i, m_offset1, m_offset2);
                        return createIterator(env, ContainerIterator(std::move(iter), this, container));
                    }
                    ++i;
                    if(!m_keyMetaType.equals(akey, i->data(m_offset1)))
                        break;
                }
            }
        }
    }
    return end(env, container);
}

jobject AutoMultiMapAccess::constFind(JNIEnv *env, const ConstExtendedContainerInfo& container, jobject key, jobject value)
{
    if(const MapDataPointer& d = *reinterpret_cast<const MapDataPointer*>(container.container)){
        jvalue jv;
        jv.l = key;
        void* akey = nullptr;
        QtJambiScope scope;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            jv.l = value;
            void* avalue = nullptr;
            if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
                node_iterator i = AutoMapAccess::find(*d, akey);
                node_iterator end1 = AutoMapAccess::end(*d);
                while(i != end1){
                    if(m_valueMetaType.equals(avalue, i->data(m_offset2))) {
                        const_iterator iter(i, m_offset1, m_offset2);
                        return createIterator(env, ContainerIterator(std::move(iter), this, container));
                    }
                    ++i;
                    if(!m_keyMetaType.equals(akey, i->data(m_offset1)))
                        break;
                }
            }
        }
    }
    return constEnd(env, container);
}

jboolean AutoMultiMapAccess::iteratorEquals(JNIEnv *, const void* ptr, AbstractSequentialConstIteratorAccess::IteratorType iteratorType, const ConstContainerAndAccessInfo& ptr2){
    if(ptr2.access->isSequentialConstIterator() && ptr2.access->isAutoAccess()){
        AbstractSequentialConstIteratorAccess::IteratorType iteratorType2 = static_cast<AbstractSequentialConstIteratorAccess*>(ptr2.access)->iteratorType();
        switch(iteratorType){
        case AbstractSequentialConstIteratorAccess::IteratorType::const_iterator: {
            using Iter1 = ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiMapAccess>, const_iterator>;
            switch(iteratorType2){
            case AbstractSequentialConstIteratorAccess::IteratorType::const_iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiMapAccess>, const_iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            case AbstractSequentialConstIteratorAccess::IteratorType::iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiMapAccess>, iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            case AbstractSequentialConstIteratorAccess::IteratorType::const_key_value_iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiMapAccess>, const_key_value_iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            case AbstractSequentialConstIteratorAccess::IteratorType::key_value_iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiMapAccess>, key_value_iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            default:
                break;
            }
        }break;
        case AbstractSequentialConstIteratorAccess::IteratorType::iterator: {
            using Iter1 = ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiMapAccess>, iterator>;
            switch(iteratorType2){
            case AbstractSequentialConstIteratorAccess::IteratorType::const_iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiMapAccess>, const_iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            case AbstractSequentialConstIteratorAccess::IteratorType::iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiMapAccess>, iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            case AbstractSequentialConstIteratorAccess::IteratorType::const_key_value_iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiMapAccess>, const_key_value_iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            case AbstractSequentialConstIteratorAccess::IteratorType::key_value_iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiMapAccess>, key_value_iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            default:
                break;
            }
        }break;
        case AbstractSequentialConstIteratorAccess::IteratorType::const_key_value_iterator: {
            using Iter1 = ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiMapAccess>, const_key_value_iterator>;
            switch(iteratorType2){
            case AbstractSequentialConstIteratorAccess::IteratorType::const_iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiMapAccess>, const_iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            case AbstractSequentialConstIteratorAccess::IteratorType::iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiMapAccess>, iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            case AbstractSequentialConstIteratorAccess::IteratorType::const_key_value_iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiMapAccess>, const_key_value_iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            case AbstractSequentialConstIteratorAccess::IteratorType::key_value_iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiMapAccess>, key_value_iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            default:
                break;
            }
        }break;
        case AbstractSequentialConstIteratorAccess::IteratorType::key_value_iterator: {
            using Iter1 = ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiMapAccess>, key_value_iterator>;
            switch(iteratorType2){
            case AbstractSequentialConstIteratorAccess::IteratorType::const_iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiMapAccess>, const_iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            case AbstractSequentialConstIteratorAccess::IteratorType::iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiMapAccess>, iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            case AbstractSequentialConstIteratorAccess::IteratorType::const_key_value_iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiMapAccess>, const_key_value_iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            case AbstractSequentialConstIteratorAccess::IteratorType::key_value_iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiMapAccess>, key_value_iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            default:
                break;
            }
        }break;
        default:
            break;
        }
    }
    return false;
}

void* AutoMultiMapAccess::asIterator(void* iter, AbstractSequentialConstIteratorAccess::IteratorType iteratorType){
    switch(iteratorType){
    case AbstractSequentialConstIteratorAccess::IteratorType::const_iterator: {
        using Iterator = ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiMapAccess>, const_iterator>;
        return &reinterpret_cast<Iterator*>(iter)->iterator();
    }break;
    case AbstractSequentialConstIteratorAccess::IteratorType::iterator: {
        using Iterator = ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiMapAccess>, iterator>;
        return &reinterpret_cast<Iterator*>(iter)->iterator();
    }break;
    case AbstractSequentialConstIteratorAccess::IteratorType::const_key_value_iterator: {
        using Iterator = ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiMapAccess>, const_key_value_iterator>;
        return &reinterpret_cast<Iterator*>(iter)->iterator();
    }break;
    case AbstractSequentialConstIteratorAccess::IteratorType::key_value_iterator: {
        using Iterator = ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiMapAccess>, key_value_iterator>;
        return &reinterpret_cast<Iterator*>(iter)->iterator();
    }break;
    default:
        return nullptr;
    }
}

qsizetype AutoMultiMapAccess::remove(JNIEnv *env, const ContainerInfo& container, jobject key, jobject value)
{
    qsizetype c = 0;
    jvalue jv;
    jv.l = key;
    void* akey = nullptr;
    QtJambiScope scope;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        jv.l = value;
        void* avalue = nullptr;
        if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
            if(MapDataPointer& d = *reinterpret_cast<MapDataPointer*>(container.container)){
                if(!d.isShared()){
                    c = erase(*d, akey, avalue);
                }else{
                    MapData *newData = createMapData();
                    c = copyIfNotEquivalentTo(*newData, *d, akey, avalue);
                    d.reset(newData);
                }
            }
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
        MapData& data = AutoMapAccess::detach(*reinterpret_cast<MapDataPointer*>(container.container));
        AutoMapAccess::node_iterator iter = AutoMapAccess::find(data, akey);
        if(iter!=AutoMapAccess::end(data)){
            void* target = iter->data(m_offset2);
            jv.l = value;
            m_valueExternalToInternalConverter(env, &scope, jv, target, jValueType::l);
        }else{
            insertOrAssign(data, akey, env, value);
        }
    }
}

size_t AutoMultiMapAccess::sizeOf() const{
    return sizeof(QMultiMap<char,char>);
}

size_t AutoMultiMapAccess::alignOf() const{
    return alignof(QMultiMap<char,char>);
}

KeyPointerRCAutoMultiMapAccess::KeyPointerRCAutoMultiMapAccess(KeyPointerRCAutoMultiMapAccess& other)
    : AutoMultiMapAccess(other), ReferenceCountingSetContainer() {}

AbstractReferenceCountingContainer* KeyPointerRCAutoMultiMapAccess::asRC() {return this;}

AbstractMultiMapAccess* KeyPointerRCAutoMultiMapAccess::clone(){
    return new KeyPointerRCAutoMultiMapAccess(*this);
}

void KeyPointerRCAutoMultiMapAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject set = Java::Runtime::HashSet::newInstance(env);
    auto iterator = AbstractMultiMapAccess::constKeyValueIterator(container.container);
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
            QtJambiAPI::addToJavaCollection(env, set, obj);
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

qsizetype KeyPointerRCAutoMultiMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    qsizetype result = AutoMultiMapAccess::remove(env, container, key);
    removeRC(env, container.object, key, result);
    return result;
}

qsizetype KeyPointerRCAutoMultiMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    qsizetype result = AutoMultiMapAccess::remove(env, container, key, value);
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

AbstractReferenceCountingContainer* ValuePointerRCAutoMultiMapAccess::asRC() {return this;}

AbstractMultiMapAccess* ValuePointerRCAutoMultiMapAccess::clone(){
    return new ValuePointerRCAutoMultiMapAccess(*this);
}

void ValuePointerRCAutoMultiMapAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject set = Java::Runtime::HashSet::newInstance(env);
    auto iterator = AbstractMultiMapAccess::constKeyValueIterator(container.container);
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
            QtJambiAPI::addToJavaCollection(env, set, obj);
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

qsizetype ValuePointerRCAutoMultiMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    ContainerAndAccessInfo oldValues = AutoMultiMapAccess::values(env, container, key);
    qsizetype result = AutoMultiMapAccess::remove(env, container, key);
    if(result>0){
        jobject iter = QtJambiAPI::iteratorOfJavaIterable(env, oldValues.object);
        while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
            jobject value = QtJambiAPI::nextOfJavaIterator(env, iter);
            if(QtJambiAPI::sizeOfJavaCollection(env, AutoMultiMapAccess::keys(env, container, value).object)==0){
                removeRC(env, container.object, value);
            }
        }
    }
    return result;
}

jobject ValuePointerRCAutoMultiMapAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = AutoMultiMapAccess::take(env, container, key);
    if(QtJambiAPI::sizeOfJavaCollection(env, AutoMultiMapAccess::keys(env, container, result).object)==0){
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

qsizetype ValuePointerRCAutoMultiMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value){
    qsizetype result = AutoMultiMapAccess::remove(env, container, key, value);
    removeRC(env, container.object, value, result);
    return result;
}

PointersRCAutoMultiMapAccess::PointersRCAutoMultiMapAccess(PointersRCAutoMultiMapAccess& other)
    : AutoMultiMapAccess(other), ReferenceCountingMultiMapContainer(other) {}

AbstractReferenceCountingContainer* PointersRCAutoMultiMapAccess::asRC() {return this;}

AbstractMultiMapAccess* PointersRCAutoMultiMapAccess::clone(){
    return new PointersRCAutoMultiMapAccess(*this);
}

void PointersRCAutoMultiMapAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject map = Java::QtJambi::ReferenceUtility$RCMap::newInstance(env);
    auto iterator = AbstractMultiMapAccess::constKeyValueIterator(container.container);
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

qsizetype PointersRCAutoMultiMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    qsizetype result = AutoMultiMapAccess::remove(env, container, key);
    removeRC(env, container.object, key, 1);
    return result;
}

qsizetype PointersRCAutoMultiMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    qsizetype result = AutoMultiMapAccess::remove(env, container, key, value);
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

AbstractReferenceCountingContainer* NestedPointersRCAutoMultiMapAccess::asRC() {return this;}

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
        auto iterator = AbstractMultiMapAccess::constKeyValueIterator(container.container);
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

qsizetype NestedPointersRCAutoMultiMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    qsizetype result = AutoMultiMapAccess::remove(env, container, key);
    if(result>0){
        updateRC(env, container);
    }
    return result;
}

qsizetype NestedPointersRCAutoMultiMapAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    qsizetype result = AutoMultiMapAccess::remove(env, container, key, value);
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

#if defined(Q_CC_MSVC) || defined(_LIBCPP_VERSION) || !defined(Q_OS_WIN)
template class QTJAMBI_EXPORT QMultiMapAccess<qint32,QString>;
template class QTJAMBI_EXPORT QMultiMapAccess<QString,QUrl>;
template class QTJAMBI_EXPORT QMultiMapAccess<QString,QVariant>;
template class QTJAMBI_EXPORT QMultiMapAccess<QByteArray,QByteArray>;
#endif

AbstractMultiMapAccess* createMultiMapAccess(const QMetaType& memberMetaType1, const QMetaType& memberMetaType2){
    switch(memberMetaType1.id()){
    case QMetaType::Type::Bool:
        switch(memberMetaType2.id()){
        case QMetaType::Type::Bool:
            break;
        case QMetaType::Char:
        case QMetaType::SChar:
        case QMetaType::UChar:
            break;
        case QMetaType::Short:
        case QMetaType::UShort:
            break;
        case QMetaType::Int:
        case QMetaType::UInt:
            break;
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
            break;
        case QMetaType::Double:
            break;
        case QMetaType::Float:
            break;
        case QMetaType::QChar:
            break;
        case QMetaType::Char16:
            break;
        case QMetaType::Char32:
            break;
        case QMetaType::QString:
            break;
        case QMetaType::QByteArray:
            break;
        case QMetaType::QVariant:
            break;
        case QMetaType::QObjectStar:
            break;
        default:
            break;
        }
        break;
    case QMetaType::Char:
    case QMetaType::SChar:
    case QMetaType::UChar:
        switch(memberMetaType2.id()){
        case QMetaType::Type::Bool:
            break;
        case QMetaType::Char:
        case QMetaType::SChar:
        case QMetaType::UChar:
            break;
        case QMetaType::Short:
        case QMetaType::UShort:
            break;
        case QMetaType::Int:
        case QMetaType::UInt:
            break;
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
            break;
        case QMetaType::Double:
            break;
        case QMetaType::Float:
            break;
        case QMetaType::QChar:
            break;
        case QMetaType::Char16:
            break;
        case QMetaType::Char32:
            break;
        case QMetaType::QString:
            break;
        case QMetaType::QByteArray:
            break;
        case QMetaType::QVariant:
            break;
        case QMetaType::QObjectStar:
            break;
        default:
            break;
        }
        break;
    case QMetaType::Short:
    case QMetaType::UShort:
        switch(memberMetaType2.id()){
        case QMetaType::Type::Bool:
            break;
        case QMetaType::Char:
        case QMetaType::SChar:
        case QMetaType::UChar:
            break;
        case QMetaType::Short:
        case QMetaType::UShort:
            break;
        case QMetaType::Int:
        case QMetaType::UInt:
            break;
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
            break;
        case QMetaType::Double:
            break;
        case QMetaType::Float:
            break;
        case QMetaType::QChar:
            break;
        case QMetaType::Char16:
            break;
        case QMetaType::Char32:
            break;
        case QMetaType::QString:
            break;
        case QMetaType::QByteArray:
            break;
        case QMetaType::QVariant:
            break;
        case QMetaType::QObjectStar:
            break;
        default:
            break;
        }
        break;
    case QMetaType::Int:
    case QMetaType::UInt:
        switch(memberMetaType2.id()){
        case QMetaType::Type::Bool:
            break;
        case QMetaType::Char:
        case QMetaType::SChar:
        case QMetaType::UChar:
            break;
        case QMetaType::Short:
        case QMetaType::UShort:
            break;
        case QMetaType::Int:
        case QMetaType::UInt:
            break;
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
            break;
        case QMetaType::Double:
            break;
        case QMetaType::Float:
            break;
        case QMetaType::QChar:
            break;
        case QMetaType::Char16:
            break;
        case QMetaType::Char32:
            break;
        case QMetaType::QString:
            return QMultiMapAccess<qint32,QString>::newInstance();
        case QMetaType::QByteArray:
            break;
        case QMetaType::QVariant:
            break;
        case QMetaType::QObjectStar:
            break;
        default:
            break;
        }
        break;
    case QMetaType::LongLong:
    case QMetaType::ULongLong:
        switch(memberMetaType2.id()){
        case QMetaType::Type::Bool:
            break;
        case QMetaType::Char:
        case QMetaType::SChar:
        case QMetaType::UChar:
            break;
        case QMetaType::Short:
        case QMetaType::UShort:
            break;
        case QMetaType::Int:
        case QMetaType::UInt:
            break;
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
            break;
        case QMetaType::Double:
            break;
        case QMetaType::Float:
            break;
        case QMetaType::QChar:
            break;
        case QMetaType::Char16:
            break;
        case QMetaType::Char32:
            break;
        case QMetaType::QString:
            break;
        case QMetaType::QByteArray:
            break;
        case QMetaType::QVariant:
            break;
        case QMetaType::QObjectStar:
            break;
        default:
            break;
        }
        break;
    case QMetaType::Double:
        switch(memberMetaType2.id()){
        case QMetaType::Type::Bool:
            break;
        case QMetaType::Char:
        case QMetaType::SChar:
        case QMetaType::UChar:
            break;
        case QMetaType::Short:
        case QMetaType::UShort:
            break;
        case QMetaType::Int:
        case QMetaType::UInt:
            break;
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
            break;
        case QMetaType::Double:
            break;
        case QMetaType::Float:
            break;
        case QMetaType::QChar:
            break;
        case QMetaType::Char16:
            break;
        case QMetaType::Char32:
            break;
        case QMetaType::QString:
            break;
        case QMetaType::QByteArray:
            break;
        case QMetaType::QVariant:
            break;
        case QMetaType::QObjectStar:
            break;
        default:
            break;
        }
        break;
    case QMetaType::Float:
        switch(memberMetaType2.id()){
        case QMetaType::Type::Bool:
            break;
        case QMetaType::Char:
        case QMetaType::SChar:
        case QMetaType::UChar:
            break;
        case QMetaType::Short:
        case QMetaType::UShort:
            break;
        case QMetaType::Int:
        case QMetaType::UInt:
            break;
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
            break;
        case QMetaType::Double:
            break;
        case QMetaType::Float:
            break;
        case QMetaType::QChar:
            break;
        case QMetaType::Char16:
            break;
        case QMetaType::Char32:
            break;
        case QMetaType::QString:
            break;
        case QMetaType::QByteArray:
            break;
        case QMetaType::QVariant:
            break;
        case QMetaType::QObjectStar:
            break;
        default:
            break;
        }
        break;
    case QMetaType::QChar:
        switch(memberMetaType2.id()){
        case QMetaType::Type::Bool:
            break;
        case QMetaType::Char:
        case QMetaType::SChar:
        case QMetaType::UChar:
            break;
        case QMetaType::Short:
        case QMetaType::UShort:
            break;
        case QMetaType::Int:
        case QMetaType::UInt:
            break;
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
            break;
        case QMetaType::Double:
            break;
        case QMetaType::Float:
            break;
        case QMetaType::QChar:
            break;
        case QMetaType::Char16:
            break;
        case QMetaType::Char32:
            break;
        case QMetaType::QString:
            break;
        case QMetaType::QByteArray:
            break;
        case QMetaType::QVariant:
            break;
        case QMetaType::QObjectStar:
            break;
        default:
            break;
        }
        break;
    case QMetaType::Char16:
        switch(memberMetaType2.id()){
        case QMetaType::Type::Bool:
            break;
        case QMetaType::Char:
        case QMetaType::SChar:
        case QMetaType::UChar:
            break;
        case QMetaType::Short:
        case QMetaType::UShort:
            break;
        case QMetaType::Int:
        case QMetaType::UInt:
            break;
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
            break;
        case QMetaType::Double:
            break;
        case QMetaType::Float:
            break;
        case QMetaType::QChar:
            break;
        case QMetaType::Char16:
            break;
        case QMetaType::Char32:
            break;
        case QMetaType::QString:
            break;
        case QMetaType::QByteArray:
            break;
        case QMetaType::QVariant:
            break;
        case QMetaType::QObjectStar:
            break;
        default:
            break;
        }
        break;
    case QMetaType::Char32:
        switch(memberMetaType2.id()){
        case QMetaType::Type::Bool:
            break;
        case QMetaType::Char:
        case QMetaType::SChar:
        case QMetaType::UChar:
            break;
        case QMetaType::Short:
        case QMetaType::UShort:
            break;
        case QMetaType::Int:
        case QMetaType::UInt:
            break;
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
            break;
        case QMetaType::Double:
            break;
        case QMetaType::Float:
            break;
        case QMetaType::QChar:
            break;
        case QMetaType::Char16:
            break;
        case QMetaType::Char32:
            break;
        case QMetaType::QString:
            break;
        case QMetaType::QByteArray:
            break;
        case QMetaType::QVariant:
            break;
        case QMetaType::QObjectStar:
            break;
        default:
            break;
        }
        break;
    case QMetaType::QString:
        switch(memberMetaType2.id()){
        case QMetaType::Type::Bool:
            break;
        case QMetaType::Char:
        case QMetaType::SChar:
        case QMetaType::UChar:
            break;
        case QMetaType::Short:
        case QMetaType::UShort:
            break;
        case QMetaType::Int:
        case QMetaType::UInt:
            break;
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
            break;
        case QMetaType::Double:
            break;
        case QMetaType::Float:
            break;
        case QMetaType::QChar:
            break;
        case QMetaType::Char16:
            break;
        case QMetaType::Char32:
            break;
        case QMetaType::QString:
            break;
        case QMetaType::QByteArray:
            break;
        case QMetaType::QVariant:
            return QMultiMapAccess<QString,QVariant>::newInstance();
        case QMetaType::QUrl:
            return QMultiMapAccess<QString,QUrl>::newInstance();
        case QMetaType::QObjectStar:
            break;
        default:
            break;
        }
        break;
    case QMetaType::QByteArray:
        switch(memberMetaType2.id()){
        case QMetaType::Type::Bool:
            break;
        case QMetaType::Char:
        case QMetaType::SChar:
        case QMetaType::UChar:
            break;
        case QMetaType::Short:
        case QMetaType::UShort:
            break;
        case QMetaType::Int:
        case QMetaType::UInt:
            break;
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
            break;
        case QMetaType::Double:
            break;
        case QMetaType::Float:
            break;
        case QMetaType::QChar:
            break;
        case QMetaType::Char16:
            break;
        case QMetaType::Char32:
            break;
        case QMetaType::QString:
            break;
        case QMetaType::QByteArray:
            return QMultiMapAccess<QByteArray,QByteArray>::newInstance();
        case QMetaType::QVariant:
            break;
        case QMetaType::QObjectStar:
            break;
        default:
            break;
        }
        break;
    case QMetaType::QVariant:
        switch(memberMetaType2.id()){
        case QMetaType::Type::Bool:
            break;
        case QMetaType::Char:
        case QMetaType::SChar:
        case QMetaType::UChar:
            break;
        case QMetaType::Short:
        case QMetaType::UShort:
            break;
        case QMetaType::Int:
        case QMetaType::UInt:
            break;
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
            break;
        case QMetaType::Double:
            break;
        case QMetaType::Float:
            break;
        case QMetaType::QChar:
            break;
        case QMetaType::Char16:
            break;
        case QMetaType::Char32:
            break;
        case QMetaType::QString:
            break;
        case QMetaType::QByteArray:
            break;
        case QMetaType::QVariant:
            break;
        case QMetaType::QObjectStar:
            break;
        default:
            break;
        }
        break;
    case QMetaType::QObjectStar:
        switch(memberMetaType2.id()){
        case QMetaType::Type::Bool:
            break;
        case QMetaType::Char:
        case QMetaType::SChar:
        case QMetaType::UChar:
            break;
        case QMetaType::Short:
        case QMetaType::UShort:
            break;
        case QMetaType::Int:
        case QMetaType::UInt:
            break;
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
            break;
        case QMetaType::Double:
            break;
        case QMetaType::Float:
            break;
        case QMetaType::QChar:
            break;
        case QMetaType::Char16:
            break;
        case QMetaType::Char32:
            break;
        case QMetaType::QString:
            break;
        case QMetaType::QByteArray:
            break;
        case QMetaType::QVariant:
            break;
        case QMetaType::QObjectStar:
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return nullptr;
}