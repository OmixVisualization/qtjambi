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
#include "containeraccess_export_multihash.h"
#include "containeraccess_export_list.h"
#include "containeraccess_export_bytearraylist.h"
#include "containeraccess_export_stringlist.h"
#include "qtjambi_cast_container.h"

QT_WARNING_DISABLE_GCC("-Winaccessible-base")
QT_WARNING_DISABLE_CLANG("-Winaccessible-base")

QT_WARNING_DISABLE_GCC("-Wstrict-aliasing")
QT_WARNING_DISABLE_CLANG("-Wstrict-aliasing")

namespace QtSharedPointer{
ExternalRefCountData* ExternalRefCountWithCustomDeleter<AutoMultiHashAccess, NormalDeleter>::create(AutoMultiHashAccess * access, QtSharedPointer::NormalDeleter, DestroyerFn){
    access->m_refCount->strongref.ref();
    access->m_refCount->weakref.ref();
    return access->m_refCount;
}

ExternalRefCountData* ExternalRefCountWithCustomDeleter<const AutoMultiHashAccess, NormalDeleter>::create(const AutoMultiHashAccess * access, QtSharedPointer::NormalDeleter, DestroyerFn){
    access->m_refCount->strongref.ref();
    access->m_refCount->weakref.ref();
    return access->m_refCount;
}

template<>
struct ExternalRefCountWithCustomDeleter<AutoMultiHashAccess, ExternalRefCountData*>{
    typedef const void* DestroyerFn;
    static constexpr char safetyCheckDeleter = 0;
    static constexpr char deleter = 0;
    static constexpr inline ExternalRefCountData* create(AutoMultiHashAccess *, ExternalRefCountData* result, DestroyerFn){
        return result;
    }
};

template<>
struct ExternalRefCountWithCustomDeleter<const AutoMultiHashAccess, ExternalRefCountData*>{
    typedef const void* DestroyerFn;
    static constexpr char safetyCheckDeleter = 0;
    static constexpr char deleter = 0;
    static constexpr inline ExternalRefCountData* create(const AutoMultiHashAccess *, ExternalRefCountData* result, DestroyerFn){
        return result;
    }
};
}

AutoMultiHashAccess::AutoMultiHashAccess(const AutoMultiHashAccess & other)
    : AbstractMultiHashAccess(), AutoHashAccess(other)
          ,m_chainOffset(other.m_chainOffset)
          ,m_chainAlign(other.m_chainAlign)
          ,m_chainSize(other.m_chainSize)
{
}

AutoMultiHashAccess::~AutoMultiHashAccess(){}

AutoMultiHashAccess::AutoMultiHashAccess(
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
    : AbstractMultiHashAccess(), AutoHashAccess(
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
      ,m_chainOffset(0)
      ,m_chainAlign(qMax<size_t>(m_valueMetaType.alignOf(), alignof(void*)))
      ,m_chainSize(0)
{
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
}

void* AutoMultiHashAccess::createContainer(const void* copy){
    return AutoHashAccess::createContainer(copy);
}

void AutoMultiHashAccess::deleteContainer(void* deleteContainer){
    return AutoHashAccess::deleteContainer(deleteContainer);
}

void* AutoMultiHashAccess::constructContainer(void* result) {
    return new(result) MultiHashData;
}

void* AutoMultiHashAccess::constructContainer(void* result, const void* container) {
    result = new(result) MultiHashData;
    assign(result, container);
    return result;
}

void* AutoMultiHashAccess::constructContainer(JNIEnv*, void* result, const ConstContainerAndAccessInfo& container) {
    return constructContainer(result, container.container);
}

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

jboolean AutoMultiHashAccess::iteratorEquals(JNIEnv *, const void* ptr, AbstractSequentialConstIteratorAccess::IteratorType iteratorType, const ConstContainerAndAccessInfo& ptr2){
    if(ptr2.access->isSequentialConstIterator() && ptr2.access->isAutoAccess()){
        AbstractSequentialConstIteratorAccess::IteratorType iteratorType2 = static_cast<AbstractSequentialConstIteratorAccess*>(ptr2.access)->iteratorType();
        switch(iteratorType){
        case AbstractSequentialConstIteratorAccess::IteratorType::const_iterator: {
            using Iter1 = ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiHashAccess>, const_iterator>;
            switch(iteratorType2){
            case AbstractSequentialConstIteratorAccess::IteratorType::const_iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiHashAccess>, const_iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            case AbstractSequentialConstIteratorAccess::IteratorType::iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiHashAccess>, iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            case AbstractSequentialConstIteratorAccess::IteratorType::const_key_value_iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiHashAccess>, const_key_value_iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            case AbstractSequentialConstIteratorAccess::IteratorType::key_value_iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiHashAccess>, key_value_iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            default:
                break;
            }
        }break;
        case AbstractSequentialConstIteratorAccess::IteratorType::iterator: {
            using Iter1 = ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiHashAccess>, iterator>;
            switch(iteratorType2){
            case AbstractSequentialConstIteratorAccess::IteratorType::const_iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiHashAccess>, const_iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            case AbstractSequentialConstIteratorAccess::IteratorType::iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiHashAccess>, iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            case AbstractSequentialConstIteratorAccess::IteratorType::const_key_value_iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiHashAccess>, const_key_value_iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            case AbstractSequentialConstIteratorAccess::IteratorType::key_value_iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiHashAccess>, key_value_iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            default:
                break;
            }
        }break;
        case AbstractSequentialConstIteratorAccess::IteratorType::const_key_value_iterator: {
            using Iter1 = ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiHashAccess>, const_key_value_iterator>;
            switch(iteratorType2){
            case AbstractSequentialConstIteratorAccess::IteratorType::const_iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiHashAccess>, const_iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            case AbstractSequentialConstIteratorAccess::IteratorType::iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiHashAccess>, iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            case AbstractSequentialConstIteratorAccess::IteratorType::const_key_value_iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiHashAccess>, const_key_value_iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            case AbstractSequentialConstIteratorAccess::IteratorType::key_value_iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiHashAccess>, key_value_iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            default:
                break;
            }
        }break;
        case AbstractSequentialConstIteratorAccess::IteratorType::key_value_iterator: {
            using Iter1 = ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiHashAccess>, key_value_iterator>;
            switch(iteratorType2){
            case AbstractSequentialConstIteratorAccess::IteratorType::const_iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiHashAccess>, const_iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            case AbstractSequentialConstIteratorAccess::IteratorType::iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiHashAccess>, iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            case AbstractSequentialConstIteratorAccess::IteratorType::const_key_value_iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiHashAccess>, const_key_value_iterator>;
                return *reinterpret_cast<const Iter1*>(ptr)==*reinterpret_cast<const Iter2*>(ptr2.container);
            }break;
            case AbstractSequentialConstIteratorAccess::IteratorType::key_value_iterator: {
                using Iter2 = ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiHashAccess>, key_value_iterator>;
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

void* AutoMultiHashAccess::asIterator(void* iter, AbstractSequentialConstIteratorAccess::IteratorType iteratorType){
    switch(iteratorType){
    case AbstractSequentialConstIteratorAccess::IteratorType::const_iterator: {
        using Iterator = ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiHashAccess>, const_iterator>;
        return &reinterpret_cast<Iterator*>(iter)->iterator();
    }break;
    case AbstractSequentialConstIteratorAccess::IteratorType::iterator: {
        using Iterator = ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiHashAccess>, iterator>;
        return &reinterpret_cast<Iterator*>(iter)->iterator();
    }break;
    case AbstractSequentialConstIteratorAccess::IteratorType::const_key_value_iterator: {
        using Iterator = ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiHashAccess>, const_key_value_iterator>;
        return &reinterpret_cast<Iterator*>(iter)->iterator();
    }break;
    case AbstractSequentialConstIteratorAccess::IteratorType::key_value_iterator: {
        using Iterator = ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiHashAccess>, key_value_iterator>;
        return &reinterpret_cast<Iterator*>(iter)->iterator();
    }break;
    default:
        return nullptr;
    }
}

void AutoMultiHashAccess::assign(JNIEnv *, const ContainerInfo& container, const ConstContainerAndAccessInfo& other){
    assign(container.container, other.container);
}

void AutoMultiHashAccess::assign(void* container, const void* other) {
    AutoHashAccess::assign(container, other);
    MultiHashData* map = reinterpret_cast<MultiHashData*>(container);
    const MultiHashData* map2 = reinterpret_cast<const MultiHashData*>(other);
    map->m_size = map2->m_size;
}

void AutoMultiHashAccess::dispose() {
    AutoHashAccess::dispose();
}

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

IsBiContainerFunction AutoMultiHashAccess::getIsBiContainerFunction(){
    return ContainerAPI::getAsQMultiHash;
}

size_t AutoMultiHashAccess::sizeOf() const{
    return sizeof(QMultiHash<char,char>);
}

size_t AutoMultiHashAccess::alignOf() const{
    return alignof(QMultiHash<char,char>);
}

qsizetype AutoMultiHashAccess::remove(JNIEnv *env, const ContainerInfo& container, jobject key) {
    qsizetype c = 0;
    QHashData ** map = reinterpret_cast<QHashData **>(container.container);
    QHashData*& d = *map;
    if (d && d->size>0){
        detach(container);
        d = *map;
        jvalue jv;
        jv.l = key;
        void* akey = nullptr;
        QtJambiScope scope;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            QHashData::iterator i = d->find(*this, akey);
            if(!i.isUnused()){
                iterator it(i);
                multi_iterator& mit = reinterpret_cast<multi_iterator&>(it);
                Chain* previousChain = nullptr;
                Chain* chain = *mit.e;
                while(chain){
                    Chain* nextChain = chain->next(*this);
                    if(previousChain)
                        previousChain->next(*this) = nextChain;
                    else
                        *mit.e = nextChain;
                    chain->destroy(*this);
                    chain = nextChain;
                    ++c;
                }
                if(!chain)
                    d->erase(*this, it.i);
            }
        }
    }
    return c;
}

jobject AutoMultiHashAccess::value(JNIEnv *env, const void* container, jobject key,jobject defaultValue) {
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    QHashData* d = *map;
    if(!d || d->size==0)
        return defaultValue;
    jvalue jv;
    jv.l = key;
    QtJambiScope scope;
    void* akey = nullptr;
    if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
        QHashData::iterator i = d->find(*this, akey);
        if(!i.isUnused()){
            iterator it(i);
            multi_iterator& mit = reinterpret_cast<multi_iterator&>(it);
            Chain* chain = *mit.e;
            if(chain){
                jvalue jv;
                jv.l = nullptr;
                m_valueInternalToExternalConverter(env, nullptr, chain->value(), jv, true);
                return jv.l;
            }
        }
    }
    return defaultValue;
}

const void* AutoMultiHashAccess::value(const void* container, const void* key,const void* defaultValue) {
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    QHashData* d = *map;
    if(!d || d->size==0)
        return defaultValue;
    QHashData::iterator i = d->find(*this, key);
    if(!i.isUnused()){
        iterator it(i);
        multi_iterator& mit = reinterpret_cast<multi_iterator&>(it);
        Chain* chain = *mit.e;
        if(chain)
            return chain->value();
    }
    return defaultValue;
}

ContainerAndAccessInfo AutoMultiHashAccess::values(JNIEnv *env, const ConstContainerInfo& container) {
    return AutoHashAccess::values(env, container);
}

jobject AutoMultiHashAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = nullptr;
    QHashData ** map = reinterpret_cast<QHashData **>(container.container);
    QHashData*& d = *map;
    if (d && d->size>0){
        detach(container);
        d = *map;
        jvalue jv;
        jv.l = key;
        void* akey = nullptr;
        QtJambiScope scope;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            QHashData::iterator i = d->find(*this, akey);
            if(!i.isUnused()){
                iterator it(i);
                multi_iterator& mit = reinterpret_cast<multi_iterator&>(it);
                Chain* previousChain = nullptr;
                Chain* chain = *mit.e;
                //bool first = true;
                while(chain){
                    //if(first)
                    {
                        jvalue jv;
                        jv.l = nullptr;
                        m_valueInternalToExternalConverter(env, nullptr, chain->value(), jv, true);
                        result = jv.l;
                        //first = false;
                    }
                    Chain* nextChain = chain->next(*this);
                    if(previousChain)
                        previousChain->next(*this) = nextChain;
                    else
                        *mit.e = nextChain;
                    chain->destroy(*this);
                    chain = nextChain;
                }
                if(!chain)
                    d->erase(*this, it.i);
            }
        }
    }
    return result;
}

qsizetype AutoMultiHashAccess::size(const void* container){
    const MultiHashData * map = reinterpret_cast<const MultiHashData*>(container);
    return map->m_size;
}

qsizetype AutoMultiHashAccess::capacity(JNIEnv * env, const void* container){ return AutoHashAccess::capacity(env, container); }
void AutoMultiHashAccess::reserve(JNIEnv * env, const ContainerInfo& container, qsizetype capacity){ AutoHashAccess::reserve(env, container, capacity); }
void AutoMultiHashAccess::reserve(void* container, qsizetype size){ AutoHashAccess::reserve(container, size); }
bool AutoMultiHashAccess::destructContainer(void* container) { return AutoHashAccess::destructContainer(container); }
std::unique_ptr<AbstractHashAccess::KeyValueIterator> AutoMultiHashAccess::keyValueIterator(const void* container) { return AutoHashAccess::keyValueIterator(container); }
std::unique_ptr<AbstractHashAccess::KeyValueIterator> AutoMultiHashAccess::keyValueIterator(void* container) { return AutoHashAccess::keyValueIterator(container); }
QMetaType AutoMultiHashAccess::registerContainer(QByteArrayView containerTypeName) {return AutoHashAccess::registerContainer(containerTypeName);}
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
bool AutoMultiHashAccess::contains(const void* container, const void* key) {return AutoHashAccess::contains(container, key);}
bool AutoMultiHashAccess::isDetached(const void* container){ return AutoHashAccess::isDetached(container); }
void AutoMultiHashAccess::detach(const ContainerInfo& container){ AutoHashAccess::detach(container); }
bool AutoMultiHashAccess::isSharedWith(const void* container, const void* container2){ return AutoHashAccess::isSharedWith(container, container2); }
const QObject* AutoMultiHashAccess::getOwner(const void* container){ return AutoHashAccess::getOwner(container); }
bool AutoMultiHashAccess::hasOwnerFunction(){ return AutoHashAccess::hasOwnerFunction(); }
void AutoMultiHashAccess::insert(void* container, const void* key, const void* value){AutoHashAccess::insert(container, key, value);}
void AutoMultiHashAccess::insert(JNIEnv *env, const ContainerInfo& container, jobject key, jobject value){AutoHashAccess::insert(env, container, key, value);}
jobject AutoMultiHashAccess::key(JNIEnv *env, const void* container, jobject value, jobject defaultKey) { return AutoHashAccess::key(env, container, value, defaultKey); }
ContainerAndAccessInfo AutoMultiHashAccess::keys(JNIEnv *env, const ConstContainerInfo& container) {return AutoHashAccess::keys(env, container);}
ContainerAndAccessInfo AutoMultiHashAccess::keys(JNIEnv *env, const ConstContainerInfo& container, jobject value) {return AutoHashAccess::keys(env, container, value);}
jboolean AutoMultiHashAccess::equal(JNIEnv *env, const void* container, jobject other) {return AutoHashAccess::equal(env, container, other);}
qsizetype AutoMultiHashAccess::size(JNIEnv *env, const void* container)  {return AutoHashAccess::size(env, container);}
AbstractMultiHashAccess* AutoMultiHashAccess::clone() {return new AutoMultiHashAccess(*this);}

ContainerAndAccessInfo AutoMultiHashAccess::uniqueKeys(JNIEnv *env, const ConstContainerInfo& container)
{
    ContainerAndAccessInfo result;
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container.container);
    QHashData* d = *map;
    if (d && d->size>0){
        AbstractListAccess* listAccess{nullptr};
        {
            auto containerAccess = createContainerAccess(SequentialContainerType::QList, m_keyMetaType);
            if(containerAccess && containerAccess->isList())
                listAccess = static_cast<AbstractListAccess*>(containerAccess);
            else{
                containerAccess = createContainerAccess(
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
                if(containerAccess && containerAccess->isList())
                    listAccess = static_cast<AbstractListAccess*>(containerAccess);
            }
        }
        if(listAccess){
            result.container = listAccess->createContainer();
            result.object = ContainerAPI::objectFromQList(env, result.container, listAccess);
            result.access = listAccess;
            QHashData::iterator e = d->end(*this);
            QHashData::iterator n = d->begin(*this);
            qsizetype idx = listAccess->size(env, result.container);
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
        }
    }
    return result;
}

void AutoMultiHashAccess::unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other)
{
    if (ContainerAPI::getAsQMultiHash(env, other.object, keyMetaType(), valueMetaType(), other.container, other.access)
            || ContainerAPI::getAsQHash(env, other.object, keyMetaType(), valueMetaType(), other.container, other.access)) {
        QHashData ** map = reinterpret_cast<QHashData **>(container.container);
        QHashData*& d = *map;
        QHashData *const* map2 = reinterpret_cast<QHashData *const*>(other.container);
        QHashData* d2 = *map2;
        if(!d && d2 && d2->ref.ref()){
            d = d2;
        }else{
            detach(container);
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

ContainerAndAccessInfo AutoMultiHashAccess::values(JNIEnv *env, const ConstContainerInfo& container, jobject key)
{
    ContainerAndAccessInfo result;
    AbstractListAccess* listAccess{nullptr};
    {
        auto containerAccess = createContainerAccess(SequentialContainerType::QList, m_valueMetaType);
        if(containerAccess && containerAccess->isList())
            listAccess = static_cast<AbstractListAccess*>(containerAccess);
        else{
            containerAccess = createContainerAccess(
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
            if(containerAccess && containerAccess->isList())
                listAccess = static_cast<AbstractListAccess*>(containerAccess);
        }
    }
    if(listAccess){
        result.container = listAccess->createContainer();
        result.object = ContainerAPI::objectFromQList(env, result.container, listAccess);
        result.access = listAccess;
        QHashData *const* map = reinterpret_cast<QHashData *const*>(container.container);
        QHashData* d = *map;
        if(d){
            jvalue jv;
            jv.l = key;
            QtJambiScope scope;
            void* akey = nullptr;
            if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
                qsizetype idx = listAccess->size(env, result.container);
                QHashData::iterator i = d->find(*this, akey);
                if(!i.isUnused()){
                    iterator it(i);
                    multi_iterator& mit = reinterpret_cast<multi_iterator&>(it);
                    const Chain* chain = *mit.e;
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
            }
        }
    }
    return result;
}

jboolean AutoMultiHashAccess::contains(JNIEnv *env, const void* container, jobject key, jobject value) {
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    QHashData* d = *map;
    if (d && d->size>0){
        jvalue jv;
        jv.l = key;
        void* akey = nullptr;
        QtJambiScope scope;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            QHashData::iterator i = d->find(*this, akey);
            if(!i.isUnused()){
                jv.l = value;
                void* avalue = nullptr;
                if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
                    iterator it(i);
                    multi_iterator& mit = reinterpret_cast<multi_iterator&>(it);
                    const Chain* chain = *mit.e;
                    while(chain){
                        if(m_valueMetaType.equals(chain->value(), avalue)){
                            return true;
                        }
                        chain = chain->next(*this);
                    }
                }
            }
        }
    }
    return false;
}

qsizetype AutoMultiHashAccess::count(JNIEnv *env, const void* container, jobject key) {
    qsizetype c = 0;
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    QHashData* d = *map;
    if (d && d->size>0){
        jvalue jv;
        jv.l = key;
        void* akey = nullptr;
        QtJambiScope scope;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            QHashData::iterator i = d->find(*this, akey);
            if(!i.isUnused()){
                iterator it(i);
                multi_iterator& mit = reinterpret_cast<multi_iterator&>(it);
                const Chain* chain = *mit.e;
                while(chain){
                    ++c;
                    chain = chain->next(*this);
                }
            }
        }
    }
    return c;
}

qsizetype AutoMultiHashAccess::count(JNIEnv *env, const void* container, jobject key, jobject value)
{
    qsizetype c = 0;
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container);
    QHashData* d = *map;
    if (d && d->size>0){
        jvalue jv;
        jv.l = key;
        void* akey = nullptr;
        QtJambiScope scope;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            QHashData::iterator i = d->find(*this, akey);
            if(!i.isUnused()){
                jv.l = value;
                void* avalue = nullptr;
                if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
                    iterator it(i);
                    multi_iterator& mit = reinterpret_cast<multi_iterator&>(it);
                    const Chain* chain = *mit.e;
                    while(chain){
                        if(m_valueMetaType.equals(chain->value(), avalue)){
                            ++c;
                        }
                        chain = chain->next(*this);
                    }
                }
            }
        }
    }
    return c;
}

jobject AutoMultiHashAccess::keyBegin(JNIEnv * env, const ConstExtendedContainerInfo& container)
{
    key_iterator iter = keyBegin(container.container);
    return createIterator(env, ContainerIterator(std::move(iter), this, container));
}

jobject AutoMultiHashAccess::keyEnd(JNIEnv * env, const ConstExtendedContainerInfo& container)
{
    key_iterator iter = keyEnd(container.container);
    return createIterator(env, ContainerIterator(std::move(iter), this, container));
}

jobject AutoMultiHashAccess::keyValueBegin(JNIEnv * env, const ExtendedContainerInfo& container)
{
    key_value_iterator iter = keyValueBegin(container.container);
    return createIterator(env, ContainerIterator(std::move(iter), this, container));
}

jobject AutoMultiHashAccess::keyValueEnd(JNIEnv * env, const ExtendedContainerInfo& container)
{
    key_value_iterator iter = keyValueEnd(container.container);
    return createIterator(env, ContainerIterator(std::move(iter), this, container));
}

jobject AutoMultiHashAccess::constKeyValueBegin(JNIEnv * env, const ConstExtendedContainerInfo& container)
{
    const_key_value_iterator iter = constKeyValueBegin(container.container);
    return createIterator(env, ContainerIterator(std::move(iter), this, container));
}

jobject AutoMultiHashAccess::constKeyValueEnd(JNIEnv * env, const ConstExtendedContainerInfo& container)
{
    const_key_value_iterator iter = constKeyValueEnd(container.container);
    return createIterator(env, ContainerIterator(std::move(iter), this, container));
}

jobject AutoMultiHashAccess::begin(JNIEnv *env, const ExtendedContainerInfo& container) {
    iterator iter = begin(container.container);
    return createIterator(env, ContainerIterator(std::move(iter), this, container));
}
jobject AutoMultiHashAccess::end(JNIEnv *env, const ExtendedContainerInfo& container) {
    iterator iter = end(container.container);
    return createIterator(env, ContainerIterator(std::move(iter), this, container));
}
jobject AutoMultiHashAccess::find(JNIEnv *env, const ExtendedContainerInfo& container, jobject key) {
    QHashData ** map = reinterpret_cast<QHashData **>(container.container);
    QHashData* d = *map;
    if (d && d->size>0){
        detach(map);
        d = *map;
        jvalue jv;
        jv.l = key;
        QtJambiScope scope;
        void* akey = nullptr;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            auto it = d->find(*this, akey);
            if (it.isUnused())
                it = d->end(*this);
            iterator iter(it);
            return createIterator(env, ContainerIterator(std::move(iter), this, container));
        }
    }
    return end(env, container);
}
jobject AutoMultiHashAccess::constBegin(JNIEnv *env, const ConstExtendedContainerInfo& container) {
    const_iterator iter = begin(container.container);
    return createIterator(env, ContainerIterator(std::move(iter), this, container));
}
jobject AutoMultiHashAccess::constEnd(JNIEnv *env, const ConstExtendedContainerInfo& container) {
    const_iterator iter = end(container.container);
    return createIterator(env, ContainerIterator(std::move(iter), this, container));
}
jobject AutoMultiHashAccess::constFind(JNIEnv *env, const ConstExtendedContainerInfo& container, jobject key) {
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container.container);
    QHashData* d = *map;
    if (d && d->size>0){
        jvalue jv;
        jv.l = key;
        QtJambiScope scope;
        void* akey = nullptr;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            auto it = d->find(*this, akey);
            if (it.isUnused())
                it = d->end(*this);
            const_iterator iter(it);
            return createIterator(env, ContainerIterator(std::move(iter), this, container));
        }
    }
    return constEnd(env, container);
}

jobject AutoMultiHashAccess::find(JNIEnv *env, const ExtendedContainerInfo& container, jobject key, jobject value)
{
    QHashData ** map = reinterpret_cast<QHashData **>(container.container);
    QHashData* d = *map;
    if (d && d->size>0){
        detach(map);
        d = *map;
        jvalue jv;
        jv.l = key;
        void* akey = nullptr;
        QtJambiScope scope;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            iterator e = d->end(*this);
            QHashData::iterator i = d->find(*this, akey);
            if(i!=e.i){
                jv.l = value;
                void* avalue = nullptr;
                if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
                    while(i!=e.i){
                        iterator iter(i);
                        if(m_valueMetaType.equals(&iter.value(), avalue)){
                            return createIterator(env, ContainerIterator(std::move(iter), this, container));
                        }
                        ++i;
                    }
                }
            }
        }
    }
    return end(env, container);
}

jobject AutoMultiHashAccess::constFind(JNIEnv *env, const ConstExtendedContainerInfo& container, jobject key, jobject value)
{
    QHashData *const* map = reinterpret_cast<QHashData *const*>(container.container);
    QHashData* d = *map;
    if (d && d->size>0){
        jvalue jv;
        jv.l = key;
        void* akey = nullptr;
        QtJambiScope scope;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            const_iterator e = d->end(*this);
            const_iterator i = d->find(*this, akey);
            if(i!=e){
                jv.l = value;
                void* avalue = nullptr;
                if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
                    while(i!=e){
                        if(m_valueMetaType.equals(&i.value(), avalue)){
                            const_iterator iter(i);
                            return createIterator(env, ContainerIterator(std::move(iter), this, container));
                        }
                        ++i;
                    }
                }
            }
        }
    }
    return constEnd(env, container);
}

jobject AutoMultiHashAccess::createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiHashAccess>, const_iterator>&& iter)
{
    using Iter = std::remove_reference_t<decltype(iter)>;
    return QtJambiAPI::convertMultiHashIteratorToJavaObject(env,
                                                            new Iter(std::move(iter)),
                                                            &QtJambiAPI::deletePointer<Iter>,
                                                            new AutoAssociativeConstIteratorAccess<AutoMultiHashAccess,Iter>(m_valueInternalToExternalConverter,m_keyInternalToExternalConverter,m_keyMetaType,m_valueMetaType));
}

jobject AutoMultiHashAccess::createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiHashAccess>, iterator>&& iter)
{
    using Iter = std::remove_reference_t<decltype(iter)>;
    QSharedPointer<QtJambiLink> link = iter.storage().link();
    return QtJambiPrivate::convertMultiHashIteratorToJavaObject(env, link,
                                                                new Iter(std::move(iter)),
                                                                &QtJambiAPI::deletePointer<Iter>,
                                                                new AutoAssociativeIteratorAccess<AutoMultiHashAccess,Iter>(m_valueInternalToExternalConverter,m_valueExternalToInternalConverter,m_keyInternalToExternalConverter,m_keyMetaType,m_valueMetaType));
}

jobject AutoMultiHashAccess::createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerAccessLink<AutoMultiHashAccess>, key_value_iterator>&& iter)
{
    using Iter = std::remove_reference_t<decltype(iter)>;
    QSharedPointer<QtJambiLink> link = iter.storage().link();
    return QtJambiPrivate::convertMultiHashKeyValueIteratorToJavaObject(env, link,
                                                                        new Iter(std::move(iter)),
                                                                        &QtJambiAPI::deletePointer<Iter>,
                                                                        new AutoAssociativeIteratorAccess<AutoMultiHashAccess,Iter,AbstractSequentialConstIteratorAccess::IteratorType::key_value_iterator>(m_valueInternalToExternalConverter,m_valueExternalToInternalConverter,m_keyInternalToExternalConverter,m_keyMetaType,m_valueMetaType));
}

jobject AutoMultiHashAccess::createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiHashAccess>, const_key_value_iterator>&& iter)
{
    using Iter = std::remove_reference_t<decltype(iter)>;
    return QtJambiAPI::convertMultiHashKeyValueIteratorToJavaObject(env,
                                                                    new Iter(std::move(iter)),
                                                                    &QtJambiAPI::deletePointer<Iter>,
                                                                    new AutoAssociativeConstIteratorAccess<AutoMultiHashAccess,Iter,AbstractSequentialConstIteratorAccess::IteratorType::const_key_value_iterator>(m_valueInternalToExternalConverter,m_keyInternalToExternalConverter,m_keyMetaType,m_valueMetaType));
}

jobject AutoMultiHashAccess::createIterator(JNIEnv * env, ContainerIterator<QtJambiPrivate::ContainerClone<AutoMultiHashAccess>, key_iterator>&& iter)
{
    using Iter = std::remove_reference_t<decltype(iter)>;
    return QtJambiAPI::convertMultiHashKeyIteratorToJavaObject(env,
                                                               new Iter(std::move(iter)),
                                                               &QtJambiAPI::deletePointer<Iter>,
                                                               new AutoAssociativeConstIteratorAccess<AutoMultiHashAccess,Iter,AbstractSequentialConstIteratorAccess::IteratorType::key_iterator>(m_valueInternalToExternalConverter,m_keyInternalToExternalConverter,m_keyMetaType,m_valueMetaType));
}

qsizetype AutoMultiHashAccess::remove(JNIEnv *env, const ContainerInfo& container, jobject key, jobject value)
{
    qsizetype c = 0;
    QHashData ** map = reinterpret_cast<QHashData **>(container.container);
    QHashData*& d = *map;
    if (d && d->size>0){
        detach(container);
        d = *map;
        jvalue jv;
        jv.l = key;
        void* akey = nullptr;
        QtJambiScope scope;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            jv.l = value;
            void* avalue = nullptr;
            if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
                QHashData::iterator i = d->find(*this, akey);
                if(!i.isUnused()){
                    jv.l = value;
                    void* avalue = nullptr;
                    if(m_valueExternalToInternalConverter(env, &scope, jv, avalue, jValueType::l)){
                        iterator it(i);
                        multi_iterator& mit = reinterpret_cast<multi_iterator&>(it);
                        Chain* previousChain = nullptr;
                        Chain* chain = *mit.e;
                        while(chain){
                            if(m_valueMetaType.equals(chain->value(), avalue)){
                                Chain* nextChain = chain->next(*this);
                                if(previousChain)
                                    previousChain->next(*this) = nextChain;
                                else
                                    *mit.e = nextChain;
                                chain->destroy(*this);
                                chain = nextChain;
                                ++c;
                            }else{
                                chain = chain->next(*this);
                            }
                        }
                        if(!chain)
                            d->erase(*this, it.i);
                    }
                }
            }
        }
    }
    return c;
}

void AutoMultiHashAccess::replace(JNIEnv *env, const ContainerInfo& container, jobject key, jobject value) {
    QHashData ** map = reinterpret_cast<QHashData **>(container.container);
    QHashData* d = *map;
    if (d && d->size>0){
        detach(container);
        d = *map;
        jvalue jv;
        jv.l = key;
        QtJambiScope scope;
        void* akey = nullptr;
        if(m_keyExternalToInternalConverter(env, &scope, jv, akey, jValueType::l)){
            if (d && d->size>0){
                QHashData::iterator i = d->find(*this, akey);
                if(i.isUnused()){
                    emplace(container.container, akey, env, value);
                }else{
                    iterator it(i);
                    //multi_iterator& mit = reinterpret_cast<multi_iterator&>(i);
                    //Chain* chain = *mit.e;
                    jv.l = value;
                    void* avalue = &it.value();
                    m_valueExternalToInternalConverter(env, nullptr, jv, avalue, jValueType::l);
                }
            }
        }
    }
}

bool AutoMultiHashAccess::isMulti() const{
    return true;
}

char* AutoMultiHashAccess::Chain::value(){
    return reinterpret_cast<char*>(this);
}

const char* AutoMultiHashAccess::Chain::value() const{
    return reinterpret_cast<const char*>(this);
}

void AutoMultiHashAccess::Chain::destroy(const AutoMultiHashAccess& access){
    access.m_valueMetaType.destruct(value());
    if (access.m_align > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
#ifdef __cpp_sized_deallocation
        operator delete(this, access.m_size, std::align_val_t(access.m_align));
#else
        operator delete(this, std::align_val_t(access.m_align));
#endif
    } else {
#ifdef __cpp_sized_deallocation
        operator delete(this, access.m_size);
#else
        operator delete(this);
#endif
    }
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
        Chain *&_e = e->next(access);
        Chain *n = _e;
        _e = nullptr;
        ++nEntries;
        e->destroy(access);
        e = n;
    }
    return  nEntries;
}

void AutoMultiHashAccess::initializeIterator(abstract_iterator& _it) const{
    multi_iterator& it = reinterpret_cast<multi_iterator&>(_it);
    if (!it.i.atEnd()) {
        Chain*& chain = *reinterpret_cast<Chain**>(it.i.value());
        it.e = &chain;
        Q_ASSERT(it.e && *it.e);
    }
}

bool AutoMultiHashAccess::iteratorEquals(const abstract_iterator& it1, const abstract_iterator& it2) const{
    return it1.e==it2.e;
}

char& AutoMultiHashAccess::iteratorValue(const abstract_iterator& _it) const{
    const multi_iterator& it = reinterpret_cast<const multi_iterator&>(_it);
    Chain*& chain = *it.e;
    return *chain->value();
}

void AutoMultiHashAccess::incrementIterator(abstract_iterator& _it) const{
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
            if (m_chainAlign > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
                chain = reinterpret_cast<Chain*>(operator new(m_chainSize, std::align_val_t(m_chainAlign)));
            else
                chain = reinterpret_cast<Chain*>(operator new(m_chainSize));
            void* val = chain->value();
            m_valueMetaType.construct(val);
            m_valueExternalToInternalConverter(env, nullptr, jv, val, jValueType::l);
            chain->next(*this) = nullptr;
        }else{
            Chain*& chainPtr = *reinterpret_cast<Chain**>(result.it.value());
            Chain* newChain;
            if (m_chainAlign > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
                newChain = reinterpret_cast<Chain*>(operator new(m_chainSize, std::align_val_t(m_chainAlign)));
            else
                newChain = reinterpret_cast<Chain*>(operator new(m_chainSize));
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
            if (m_chainAlign > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
                chain = reinterpret_cast<Chain*>(operator new(m_chainSize, std::align_val_t(m_chainAlign)));
            else
                chain = reinterpret_cast<Chain*>(operator new(m_chainSize));
            void* val = chain->value();
            m_valueMetaType.construct(val, value);
            chain->next(*this) = nullptr;
        }else{
            Chain*& chainPtr = *reinterpret_cast<Chain**>(result.it.value());
            Chain* newChain;
            if (m_chainAlign > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
                newChain = reinterpret_cast<Chain*>(operator new(m_chainSize, std::align_val_t(m_chainAlign)));
            else
                newChain = reinterpret_cast<Chain*>(operator new(m_chainSize));
            void* val = newChain->value();
            m_valueMetaType.construct(val, value);
            newChain->next(*this) = qExchange(chainPtr, newChain);
        }
        ++map->m_size;
    }
}

void AutoMultiHashAccess::eraseSpanEntry(char* value, qsizetype* count) const{
    Chain*& chain = *reinterpret_cast<Chain**>(value);
    if(chain){
        qsizetype c = chain->free(*this);
        if(count)
            *count = c;
        chain = nullptr;
    }
}

void AutoMultiHashAccess::copySpanEntry(char* value1, const char* value2) const{
    Chain** chain1 = reinterpret_cast<Chain**>(value1);
    *chain1 = nullptr;
    const Chain* chain2 = *reinterpret_cast<Chain*const*>(value2);
    while(chain2){
        Chain* newChain;
        if (m_chainAlign > __STDCPP_DEFAULT_NEW_ALIGNMENT__)
            newChain = reinterpret_cast<Chain*>(operator new(m_chainSize, std::align_val_t(m_chainAlign)));
        else
            newChain = reinterpret_cast<Chain*>(operator new(m_chainSize));
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

AbstractReferenceCountingContainer* KeyPointerRCAutoMultiHashAccess::asRC() {return this;}

AbstractMultiHashAccess* KeyPointerRCAutoMultiHashAccess::clone(){
    return new KeyPointerRCAutoMultiHashAccess(*this);
}

void KeyPointerRCAutoMultiHashAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject set = Java::Runtime::HashSet::newInstance(env);
    auto iterator = AbstractMultiHashAccess::constKeyValueIterator(container.container);
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

qsizetype KeyPointerRCAutoMultiHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    qsizetype result = AutoMultiHashAccess::remove(env, container, key);
    if(result>0){
        removeRC(env, container.object, key);
    }
    return result;
}

qsizetype KeyPointerRCAutoMultiHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    qsizetype result = AutoMultiHashAccess::remove(env, container, key, value);
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

AbstractReferenceCountingContainer* ValuePointerRCAutoMultiHashAccess::asRC() {return this;}

AbstractMultiHashAccess* ValuePointerRCAutoMultiHashAccess::clone(){
    return new ValuePointerRCAutoMultiHashAccess(*this);
}

void ValuePointerRCAutoMultiHashAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject set = Java::Runtime::HashSet::newInstance(env);
    auto iterator = AbstractMultiHashAccess::constKeyValueIterator(container.container);
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

qsizetype ValuePointerRCAutoMultiHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    ContainerAndAccessInfo oldValues = AutoMultiHashAccess::values(env, container, key);
    qsizetype result = AutoMultiHashAccess::remove(env, container, key);
    if(result>0){
        jobject iter = QtJambiAPI::iteratorOfJavaIterable(env, oldValues.object);
        while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
            jobject value = QtJambiAPI::nextOfJavaIterator(env, iter);
            if(QtJambiAPI::sizeOfJavaCollection(env, AutoMultiHashAccess::keys(env, container, value).object)==0){
                removeRC(env, container.object, value);
            }
        }
    }
    return result;
}

jobject ValuePointerRCAutoMultiHashAccess::take(JNIEnv *env, const ContainerInfo& container, jobject key) {
    jobject result = AutoMultiHashAccess::take(env, container, key);
    if(QtJambiAPI::sizeOfJavaCollection(env, AutoMultiHashAccess::keys(env, container, result).object)==0){
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

qsizetype ValuePointerRCAutoMultiHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value){
    qsizetype result = AutoMultiHashAccess::remove(env, container, key, value);
    if(result>0)
        removeRC(env, container.object, value, result);
    return result;
}

PointersRCAutoMultiHashAccess::PointersRCAutoMultiHashAccess(PointersRCAutoMultiHashAccess& other)
    : AutoMultiHashAccess(other), ReferenceCountingMultiMapContainer(other) {}

AbstractReferenceCountingContainer* PointersRCAutoMultiHashAccess::asRC() {return this;}

AbstractMultiHashAccess* PointersRCAutoMultiHashAccess::clone(){
    return new PointersRCAutoMultiHashAccess(*this);
}

void PointersRCAutoMultiHashAccess::updateRC(JNIEnv * env, const ContainerInfo& container){
    JniLocalFrame frame(env, 200);
    jobject map = Java::QtJambi::ReferenceUtility$RCMap::newInstance(env);
    auto iterator = AbstractMultiHashAccess::constKeyValueIterator(container.container);
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

qsizetype PointersRCAutoMultiHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    qsizetype result = AutoMultiHashAccess::remove(env, container, key);
    updateRC(env, container);
    return result;
}

qsizetype PointersRCAutoMultiHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    qsizetype result = AutoMultiHashAccess::remove(env, container, key, value);
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

AbstractReferenceCountingContainer* NestedPointersRCAutoMultiHashAccess::asRC() {return this;}

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
        auto iterator = AbstractMultiHashAccess::constKeyValueIterator(container.container);
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

qsizetype NestedPointersRCAutoMultiHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key) {
    qsizetype result = AutoMultiHashAccess::remove(env, container, key);
    if(result>0){
        removeRC(env, container.object, key);
    }
    return result;
}

qsizetype NestedPointersRCAutoMultiHashAccess::remove(JNIEnv * env, const ContainerInfo& container, jobject key, jobject value) {
    qsizetype result = AutoMultiHashAccess::remove(env, container, key, value);
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

#if defined(Q_CC_MSVC) || defined(_LIBCPP_VERSION) || !defined(Q_OS_WIN)
template class QTJAMBI_EXPORT QMultiHashAccess<qint16,QByteArray>;
template class QTJAMBI_EXPORT QMultiHashAccess<QByteArray,QByteArray>;
#endif

AbstractMultiHashAccess* createMultiHashAccess(const QMetaType& memberMetaType1, const QMetaType& memberMetaType2){
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
            return QMultiHashAccess<qint16,QByteArray>::newInstance();
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
            break;
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
            return QMultiHashAccess<QByteArray,QByteArray>::newInstance();
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