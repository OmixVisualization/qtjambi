/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/


#ifndef CONTAINERACCESS_MULTIMAP_H
#define CONTAINERACCESS_MULTIMAP_H

#include <QtCore/QMultiMap>
#include "containeraccess.h"

#if defined(QTJAMBI_GENERIC_ACCESS)

namespace ContainerAccessAPI {

template<size_t align1, size_t size1, size_t align2, size_t size2>
class GenericMultiMapAccess : public AbstractMultiMapAccess, public AbstractNestedAssociativeAccess {
    typedef typename std::conditional<size1==0, void*, ContainerElement<size1, 0, false, align1>>::type K;
    typedef typename std::conditional<size2==0, void*, ContainerElement<size2, 1, false, align2>>::type T;
    MetaTypeInfo<0,size1==0> m_keyMetaTypeInfo;
    QtJambiUtils::InternalToExternalConverter m_keyInternalToExternalConverter;
    QtJambiUtils::ExternalToInternalConverter m_keyExternalToInternalConverter;
    QSharedPointer<AbstractContainerAccess> m_keyNestedContainerAccess;
    MetaTypeInfo<1,size2==0> m_valueMetaTypeInfo;
    QtJambiUtils::InternalToExternalConverter m_valueInternalToExternalConverter;
    QtJambiUtils::ExternalToInternalConverter m_valueExternalToInternalConverter;
    QSharedPointer<AbstractContainerAccess> m_valueNestedContainerAccess;
    AbstractContainerAccess::DataType m_keyDataType;
    AbstractContainerAccess::DataType m_valueDataType;
    PtrOwnerFunction m_keyOwnerFunction;
    PtrOwnerFunction m_valueOwnerFunction;
    GenericMultiMapAccess(
            const QMetaType& keyMetaType,
            const QtJambiUtils::QHashFunction& keyHashFunction,
            const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
            const QtJambiUtils::ExternalToInternalConverter& keyExternalToInternalConverter,
            const QSharedPointer<AbstractContainerAccess>& keyNestedContainerAccess,
            PtrOwnerFunction keyOwnerFunction,
            const QMetaType& valueMetaType,
            const QtJambiUtils::QHashFunction& valueHashFunction,
            const QtJambiUtils::InternalToExternalConverter& valueInternalToExternalConverter,
            const QtJambiUtils::ExternalToInternalConverter& valueExternalToInternalConverter,
            const QSharedPointer<AbstractContainerAccess>& valueNestedContainerAccess,
            PtrOwnerFunction valueOwnerFunction
            )
        :   AbstractMultiMapAccess(), AbstractNestedAssociativeAccess(),
          m_keyMetaTypeInfo(keyMetaType, keyHashFunction),
          m_keyInternalToExternalConverter(keyInternalToExternalConverter),
          m_keyExternalToInternalConverter(keyExternalToInternalConverter),
          m_keyNestedContainerAccess(keyNestedContainerAccess),
          m_valueMetaTypeInfo(valueMetaType, valueHashFunction),
          m_valueInternalToExternalConverter(valueInternalToExternalConverter),
          m_valueExternalToInternalConverter(valueExternalToInternalConverter),
          m_valueNestedContainerAccess(valueNestedContainerAccess),
          m_keyDataType(dataType(keyMetaType, m_keyNestedContainerAccess)),
          m_valueDataType(dataType(valueMetaType, m_valueNestedContainerAccess)),
          m_keyOwnerFunction(keyOwnerFunction),
          m_valueOwnerFunction(valueOwnerFunction)
    {
    }

    GenericMultiMapAccess(const GenericMultiMapAccess<align1, size1, align2, size2>& other)
        :   AbstractMultiMapAccess(), AbstractNestedAssociativeAccess(),
          m_keyMetaTypeInfo(other.m_keyMetaTypeInfo),
          m_keyInternalToExternalConverter(other.m_keyInternalToExternalConverter),
          m_keyExternalToInternalConverter(other.m_keyExternalToInternalConverter),
          m_keyNestedContainerAccess(other.m_keyNestedContainerAccess),
          m_valueMetaTypeInfo(other.m_valueMetaTypeInfo),
          m_valueInternalToExternalConverter(other.m_valueInternalToExternalConverter),
          m_valueExternalToInternalConverter(other.m_valueExternalToInternalConverter),
          m_valueNestedContainerAccess(other.m_valueNestedContainerAccess),
          m_keyDataType(other.m_keyDataType),
          m_valueDataType(other.m_valueDataType),
          m_keyOwnerFunction(other.m_keyOwnerFunction),
          m_valueOwnerFunction(other.m_valueOwnerFunction)
    {
    }

public:
    static AbstractMultiMapAccess* newInstance(
                                          const QMetaType& keyMetaType,
                                          const QtJambiUtils::QHashFunction& keyHashFunction,
                                          const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
                                          const QtJambiUtils::ExternalToInternalConverter& keyExternalToInternalConverter,
                                          const QSharedPointer<AbstractContainerAccess>& keyNestedContainerAccess,
                                          PtrOwnerFunction keyOwnerFunction,
                                          const QMetaType& valueMetaType,
                                          const QtJambiUtils::QHashFunction& valueHashFunction,
                                          const QtJambiUtils::InternalToExternalConverter& valueInternalToExternalConverter,
                                          const QtJambiUtils::ExternalToInternalConverter& valueExternalToInternalConverter,
                                          const QSharedPointer<AbstractContainerAccess>& valueNestedContainerAccess,
                                          PtrOwnerFunction valueOwnerFunction){
        return new GenericMultiMapAccess(
                                    keyMetaType,
                                    keyHashFunction,
                                    keyInternalToExternalConverter,
                                    keyExternalToInternalConverter,
                                    keyNestedContainerAccess,
                                    keyOwnerFunction,
                                    valueMetaType,
                                    valueHashFunction,
                                    valueInternalToExternalConverter,
                                    valueExternalToInternalConverter,
                                    valueNestedContainerAccess,
                                    valueOwnerFunction);
    }

    void dispose() override { delete this; }

    GenericMultiMapAccess<align1, size1, align2, size2>* clone() override{
        return new GenericMultiMapAccess<align1, size1, align2, size2>(*this);
    }

    size_t sizeOf() override {
        return sizeof(QMultiMap<K,T>);
    }
    void assign(JNIEnv *, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        (*reinterpret_cast<QMultiMap<K,T>*>(container.container)) = (*reinterpret_cast<const QMultiMap<K,T>*>(other.container));
    }
    void assign(void* container, const void* other) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
            (*reinterpret_cast<QMultiMap<K,T>*>(container)) = (*reinterpret_cast<const QMultiMap<K,T>*>(other));
    }
    void* constructContainer(void* placement) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        return new(placement) QMultiMap<K,T>();
    }
    void* constructContainer(void* placement, const void* copyOf) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        return new(placement) QMultiMap<K,T>(*reinterpret_cast<const QMultiMap<K,T>*>(copyOf));
    }
    void* constructContainer(JNIEnv *, void* placement, const ConstContainerAndAccessInfo& copyOf) override {
        return constructContainer(placement, copyOf.container);
    }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* move) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        return new(placement) QMultiMap<K,T>(std::move(*reinterpret_cast<const QMultiMap<K,T>*>(move)));
    }
    void* constructContainer(JNIEnv *, void* placement, const ContainerAndAccessInfo& move) override {
        return constructContainer(placement, move.container);
    }
#endif
    bool destructContainer(void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        reinterpret_cast<QMultiMap<K,T>*>(container)->~QMultiMap<K,T>();
        return true;
    }
    int registerContainer(const QByteArray& containerTypeName) override {
        return QtJambiPrivate::registerAssociativeContainerType<QMultiMap<K,T>, size1, size2>(containerTypeName, m_keyMetaTypeInfo.metaType(), m_valueMetaTypeInfo.metaType(), this);
    }
    const QMetaType& keyMetaType() override {
        return m_keyMetaTypeInfo.metaType();
    }

    const QMetaType& valueMetaType() override {
        return m_valueMetaTypeInfo.metaType();
    }

    DataType keyType() override{
        return m_keyDataType;
    }

    DataType valueType() override{
        return m_valueDataType;
    }

    AbstractContainerAccess* keyNestedContainerAccess() override {
        return m_keyNestedContainerAccess ? m_keyNestedContainerAccess->clone() : nullptr;
    }

    AbstractContainerAccess* valueNestedContainerAccess() override {
        return m_valueNestedContainerAccess ? m_valueNestedContainerAccess->clone() : nullptr;
    }

    const QSharedPointer<AbstractContainerAccess>& sharedKeyNestedContainerAccess() override {
        return m_keyNestedContainerAccess;
    }
    const QSharedPointer<AbstractContainerAccess>& sharedValueNestedContainerAccess() override {
        return m_valueNestedContainerAccess;
    }
    bool hasKeyNestedContainerAccess() override {
        return !m_keyNestedContainerAccess.isNull();
    }
    bool hasKeyNestedPointers() override {
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
    bool hasValueNestedContainerAccess() override {
        return !m_valueNestedContainerAccess.isNull();
    }
    bool hasValueNestedPointers() override {
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

    void clear(JNIEnv *, const ContainerInfo& container) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        reinterpret_cast<QMultiMap<K,T> *>(container.container)->clear();
    }

    jboolean contains(JNIEnv * env, const void* container, jobject key) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        bool result = false;
        {
            jvalue _key;
            _key.l = key;
            K _qkey;
            void *_qkeyPtr = &_qkey;
            if(m_keyExternalToInternalConverter(env, nullptr, _key, _qkeyPtr, jValueType::l))
                result = reinterpret_cast<const QMultiMap<K,T> *>(container)->contains(_qkey);
        }
        return result;
    }

    jint count(JNIEnv * env, const void* container, jobject key) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        jint result = 0;
        {
            jvalue _key;
            _key.l = key;
            K _qkey;
            void *_qkeyPtr = &_qkey;
            if(m_keyExternalToInternalConverter(env, nullptr, _key, _qkeyPtr, jValueType::l))
                result = jint(reinterpret_cast<const QMultiMap<K,T> *>(container)->count(_qkey));
        }
        return result;
    }

    jobject first(JNIEnv * env, const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        jobject result = nullptr;
        {
            const T& first = reinterpret_cast<const QMultiMap<K,T> *>(container)->first();
            jvalue _value;
            _value.l = nullptr;
            if(m_valueInternalToExternalConverter(env, nullptr, &first, _value, true)){
                result = _value.l;
            }
        }
        return result;
    }

    jobject firstKey(JNIEnv * env, const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        jobject result = nullptr;
        {
            const K& firstKey = reinterpret_cast<const QMultiMap<K,T> *>(container)->firstKey();
            jvalue _value;
            _value.l = nullptr;
            if(m_keyInternalToExternalConverter(env, nullptr, &firstKey, _value, true)){
                result = _value.l;
            }
        }
        return result;
    }

    void insert(JNIEnv *env, const ContainerInfo& container, jobject key, jobject value) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        {
            jvalue jv;
            jv.l = key;
            K _qkey;
            void *_qkeyPtr = &_qkey;
            if(m_keyExternalToInternalConverter(env, nullptr, jv, _qkeyPtr, jValueType::l)){
                jv.l = value;
                T _qvalue;
                void *_qvaluePtr = &_qvalue;
                if(m_valueExternalToInternalConverter(env, nullptr, jv, _qvaluePtr, jValueType::l)){
                    reinterpret_cast<QMultiMap<K,T> *>(container.container)->insert(_qkey, _qvalue);
                }

            }
        }
    }

    jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        jobject result = nullptr;
        {
            jvalue jv;
            jv.l = defaultKey;
            K _qdefaultkey;
            void *_qkeyPtr = &_qdefaultkey;
            if(m_keyExternalToInternalConverter(env, nullptr, jv, _qkeyPtr, jValueType::l)){
                jv.l = value;
                T _qvalue;
                void *_qvaluePtr = &_qvalue;
                if(m_valueExternalToInternalConverter(env, nullptr, jv, _qvaluePtr, jValueType::l)){
                    K _qkey = reinterpret_cast<const QMultiMap<K,T> *>(container)->key(_qvalue, _qdefaultkey);
                    jv.l = nullptr;
                    if(m_keyInternalToExternalConverter(env, nullptr, &_qkey, jv, true)){
                        result = jv.l;
                    }
                }

            }
        }
        return result;
    }

    ContainerAndAccessInfo keys(JNIEnv * env, const ConstContainerInfo& container) override {
        ContainerAndAccessInfo result;
        QTJAMBI_KEY_VALUE_LOCKER(this);
        result.access = ContainerAccessAPI::createContainerAccess(SequentialContainerType::QList, m_keyMetaTypeInfo.metaType());
        if(!result.access)
            result.access = ContainerAccessAPI::createContainerAccess(env, SequentialContainerType::QList,
                                                     m_keyMetaTypeInfo.metaType(),
                                                     align1, size1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                     AbstractContainerAccess::isStaticType(m_keyMetaTypeInfo.metaType()),
#endif
                                                     AbstractContainerAccess::isPointerType(m_keyMetaTypeInfo.metaType()),
                                                     m_keyMetaTypeInfo.hashFunction(),
                                                     m_keyInternalToExternalConverter,
                                                     m_keyExternalToInternalConverter,
                                                     m_keyNestedContainerAccess,
                                                     m_keyOwnerFunction
                                                    );
        if(result.access){
            result.container = new QList<K>(reinterpret_cast<const QMultiMap<K,T> *>(container.container)->keys());
            result.object = ContainerAPI::objectFromQList(env, result.container, result.access);
        }
        return result;
    }

    ContainerAndAccessInfo keys(JNIEnv * env, const ConstContainerInfo& container, jobject value) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        ContainerAndAccessInfo result;
        {
            jvalue jv;
            jv.l = value;
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            if(m_valueExternalToInternalConverter(env, nullptr, jv, _qvaluePtr, jValueType::l)){
                result.access = ContainerAccessAPI::createContainerAccess(SequentialContainerType::QList, m_keyMetaTypeInfo.metaType());
                if(!result.access)
                    result.access = ContainerAccessAPI::createContainerAccess(env, SequentialContainerType::QList,
                                                             m_keyMetaTypeInfo.metaType(),
                                                             align1, size1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                             AbstractContainerAccess::isStaticType(m_keyMetaTypeInfo.metaType()),
#endif
                                                             AbstractContainerAccess::isPointerType(m_keyMetaTypeInfo.metaType()),
                                                             m_keyMetaTypeInfo.hashFunction(),
                                                             m_keyInternalToExternalConverter,
                                                             m_keyExternalToInternalConverter,
                                                             m_keyNestedContainerAccess,
                                                             m_keyOwnerFunction
                                                          );
                if(result.access){
                    result.container = new QList<K>(reinterpret_cast<const QMultiMap<K,T> *>(container.container)->keys(_qvalue));
                    result.object = ContainerAPI::objectFromQList(env, result.container, result.access);
                }
            }
        }
        return result;
    }

    jobject last(JNIEnv * env, const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        jobject result = nullptr;
        {
            const T& last = reinterpret_cast<const QMultiMap<K,T> *>(container)->last();
            jvalue _value;
            _value.l = nullptr;
            if(m_valueInternalToExternalConverter(env, nullptr, &last, _value, true)){
                result = _value.l;
            }
        }
        return result;
    }

    jobject lastKey(JNIEnv * env, const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        jobject result = nullptr;
        {
            const K& lastKey = reinterpret_cast<const QMultiMap<K,T> *>(container)->lastKey();
            jvalue _value;
            _value.l = nullptr;
            if(m_keyInternalToExternalConverter(env, nullptr, &lastKey, _value, true)){
                result = _value.l;
            }
        }
        return result;
    }

    jobject constLowerBound(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        jobject result = nullptr;
        {
            jvalue _key;
            _key.l = key;
            K _qkey;
            void *_qkeyPtr = &_qkey;
            if(m_keyExternalToInternalConverter(env, nullptr, _key, _qkeyPtr, jValueType::l)){
                AbstractAssociativeConstIteratorAccess* access = new AssociativeConstIteratorAccess<QMultiMap, align1, size1, align2, size2>(
                            m_keyMetaTypeInfo,
                            m_keyInternalToExternalConverter,
                            m_valueMetaTypeInfo,
                            m_valueInternalToExternalConverter);
                result = QtJambiAPI::convertQAssociativeIteratorToJavaObject(env,
                                              container.nativeId,
                                              new typename QMultiMap<K,T>::const_iterator(reinterpret_cast<const QMultiMap<K,T> *>(container.container)->lowerBound(_qkey)),
                                              [](void* ptr,bool){ delete reinterpret_cast<typename QMultiMap<K,T>::const_iterator*>(ptr); },
                                              access);
            }
        }
        if(!result)
            result = constEnd(env, container);
        return result;
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        void* ptr{nullptr};
        if (ContainerAPI::getAsQMultiMap(env, other, keyMetaType(), valueMetaType(), ptr)) {
            QTJAMBI_KEY_VALUE_LOCKER(this);
            bool equals = *reinterpret_cast<const QMultiMap<K,T> *>(container)==*reinterpret_cast<const QMultiMap<K,T> *>(ptr);
            return equals;
        }else{
            QTJAMBI_KEY_VALUE_LOCKER(this);
            QMultiMap<K,T> map;
            jobject iterator = QtJambiAPI::entrySetIteratorOfJavaMap(env, other);
            while(QtJambiAPI::hasJavaIteratorNext(env, iterator)){
                jobject __java_entry = QtJambiAPI::nextOfJavaIterator(env, iterator);
                jvalue jv;
                jv.l = QtJambiAPI::keyOfJavaMapEntry(env, __java_entry);
                K _qkey;
                void *_qkeyPtr = &_qkey;
                if(m_keyExternalToInternalConverter(env, nullptr, jv, _qkeyPtr, jValueType::l)){
                    jv.l = QtJambiAPI::valueOfJavaMapEntry(env, __java_entry);
                    T _qvalue;
                    void *_qvaluePtr = &_qvalue;
                    if(m_valueExternalToInternalConverter(env, nullptr, jv, _qvaluePtr, jValueType::l)){
                        map.insert(_qkey, _qvalue);
                    }
                }
            }
            return *reinterpret_cast<const QMultiMap<K,T> *>(container)==map;
        }
        return false;
    }

    jint remove(JNIEnv * env, const ContainerInfo& container, jobject key) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        jint result = 0;
        {
            jvalue _key;
            _key.l = key;
            K _qkey;
            void *_qkeyPtr = &_qkey;
            if(m_keyExternalToInternalConverter(env, nullptr, _key, _qkeyPtr, jValueType::l)){
                result = reinterpret_cast<QMultiMap<K,T> *>(container.container)->remove(_qkey);
            }
        }
        return result;
    }

    jint size(JNIEnv *, const void* container) override {
        jint result = 0;
        QTJAMBI_KEY_VALUE_LOCKER(this);
        result = reinterpret_cast<const QMultiMap<K,T> *>(container)->size();
        return result;
    }

    jobject take(JNIEnv *env, const ContainerInfo& container, jobject key) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        jobject result = nullptr;
        jvalue _key;
        _key.l = key;
        K _qkey;
        void *_qkeyPtr = &_qkey;
        if(m_keyExternalToInternalConverter(env, nullptr, _key, _qkeyPtr, jValueType::l)){
            T _qvalue = reinterpret_cast<QMultiMap<K,T> *>(container.container)->take(_qkey);
            jvalue _value;
            _value.l = nullptr;
            if(m_valueInternalToExternalConverter(env, nullptr, &_qvalue, _value, true)){
                result = _value.l;
            }
        }
        return result;
    }

    jobject constUpperBound(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        jobject result = nullptr;
        {
            jvalue _key;
            _key.l = key;
            K _qkey;
            void *_qkeyPtr = &_qkey;
            if(m_keyExternalToInternalConverter(env, nullptr, _key, _qkeyPtr, jValueType::l)){
                AbstractAssociativeConstIteratorAccess* access = new AssociativeConstIteratorAccess<QMultiMap, align1, size1, align2, size2>(
                            m_keyMetaTypeInfo,
                            m_keyInternalToExternalConverter,
                            m_valueMetaTypeInfo,
                            m_valueInternalToExternalConverter);
                result = QtJambiAPI::convertQAssociativeIteratorToJavaObject(env,
                                              container.nativeId,
                                              new typename QMultiMap<K,T>::const_iterator(reinterpret_cast<const QMultiMap<K,T> *>(container.container)->upperBound(_qkey)),
                                              [](void* ptr,bool){ delete reinterpret_cast<typename QMultiMap<K,T>::const_iterator*>(ptr); },
                                              access);
            }
        }
        if(!result)
            result = constEnd(env, container);
        return result;
    }

    jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override {
        jobject result = nullptr;
        QTJAMBI_KEY_VALUE_LOCKER(this);
        {
            jvalue jv;
            jv.l = key;
            K _qkey;
            void *_qkeyPtr = &_qkey;
            if(m_keyExternalToInternalConverter(env, nullptr, jv, _qkeyPtr, jValueType::l)){
                jv.l = defaultValue;
                T _qdefaultValue;
                void *_qdefaultValuePtr = &_qdefaultValue;
                if(m_valueExternalToInternalConverter(env, nullptr, jv, _qdefaultValuePtr, jValueType::l)){
                    T _qvalue = reinterpret_cast<const QMultiMap<K,T> *>(container)->value(_qkey, _qdefaultValue);
                    jv.l = nullptr;
                    if(m_valueInternalToExternalConverter(env, nullptr, &_qvalue, jv, true)){
                        result = jv.l;
                    }
                }
            }
        }
        return result;
    }

    ContainerAndAccessInfo values(JNIEnv * env, const ConstContainerInfo& container) override {
        ContainerAndAccessInfo result;
        QTJAMBI_KEY_VALUE_LOCKER(this);
        result.access = ContainerAccessAPI::createContainerAccess(SequentialContainerType::QList, m_valueMetaTypeInfo.metaType());
        if(!result.access)
            result.access = ContainerAccessAPI::createContainerAccess(env, SequentialContainerType::QList,
                                                      m_valueMetaTypeInfo.metaType(),
                                                      align2, size2,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                      AbstractContainerAccess::isStaticType(m_valueMetaTypeInfo.metaType()),
#endif
                                                      AbstractContainerAccess::isPointerType(m_valueMetaTypeInfo.metaType()),
                                                      m_valueMetaTypeInfo.hashFunction(),
                                                      m_valueInternalToExternalConverter,
                                                      m_valueExternalToInternalConverter,
                                                      m_valueNestedContainerAccess,
                                                      m_valueOwnerFunction
                                                     );
        if(result.access){
            result.container = new QList<T>(reinterpret_cast<const QMultiMap<K,T> *>(container.container)->values());
            result.object = ContainerAPI::objectFromQList(env, result.container, result.access);
        }
        return result;
    }

    ContainerAndAccessInfo uniqueKeys(JNIEnv * env, const ConstContainerInfo& container) override {
        ContainerAndAccessInfo result;
        QTJAMBI_KEY_VALUE_LOCKER(this);
        result.access = ContainerAccessAPI::createContainerAccess(SequentialContainerType::QList, m_keyMetaTypeInfo.metaType());
        if(!result.access)
            result.access = ContainerAccessAPI::createContainerAccess(env, SequentialContainerType::QList,
                                                     m_keyMetaTypeInfo.metaType(),
                                                     align1, size1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                     AbstractContainerAccess::isStaticType(m_keyMetaTypeInfo.metaType()),
#endif
                                                     AbstractContainerAccess::isPointerType(m_keyMetaTypeInfo.metaType()),
                                                     m_keyMetaTypeInfo.hashFunction(),
                                                     m_keyInternalToExternalConverter,
                                                     m_keyExternalToInternalConverter,
                                                     m_keyNestedContainerAccess,
                                                     m_keyOwnerFunction
                                                   );
        if(result.access){
            result.container = new QList<K>(reinterpret_cast<const QMultiMap<K,T> *>(container.container)->uniqueKeys());
            result.object = ContainerAPI::objectFromQList(env, result.container, result.access);
        }
        return result;
    }

    ContainerAndAccessInfo values(JNIEnv * env, const ConstContainerInfo& container, jobject key) override {
        ContainerAndAccessInfo result;
        QTJAMBI_KEY_VALUE_LOCKER(this);
        result.access = ContainerAccessAPI::createContainerAccess(SequentialContainerType::QList, m_valueMetaTypeInfo.metaType());
        if(!result.access)
            result.access = ContainerAccessAPI::createContainerAccess(env, SequentialContainerType::QList,
                                                      m_valueMetaTypeInfo.metaType(),
                                                      align2, size2,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                      AbstractContainerAccess::isStaticType(m_valueMetaTypeInfo.metaType()),
#endif
                                                      AbstractContainerAccess::isPointerType(m_valueMetaTypeInfo.metaType()),
                                                      m_valueMetaTypeInfo.hashFunction(),
                                                      m_valueInternalToExternalConverter,
                                                      m_valueExternalToInternalConverter,
                                                      m_valueNestedContainerAccess,
                                                      m_valueOwnerFunction
                                                    );
        if(result.access){
            jvalue jv;
            jv.l = key;
            K _qkey;
            void *_qkeyPtr = &_qkey;
            if(m_keyExternalToInternalConverter(env, nullptr, jv, _qkeyPtr, jValueType::l)){
                result.container = new QList<T>(reinterpret_cast<const QMultiMap<K,T> *>(container.container)->values(_qkey));
                result.object = ContainerAPI::objectFromQList(env, result.container, result.access);
            }
        }
        return result;
    }

    jboolean contains(JNIEnv *env, const void* container, jobject key, jobject value) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        bool result = false;
        {
            jvalue jv;
            jv.l = key;
            K _qkey;
            void *_qkeyPtr = &_qkey;
            if(m_keyExternalToInternalConverter(env, nullptr, jv, _qkeyPtr, jValueType::l)){
                jv.l = value;
                T _qvalue;
                void *_qvaluePtr = &_qvalue;
                if(m_valueExternalToInternalConverter(env, nullptr, jv, _qvaluePtr, jValueType::l)){
                    result = reinterpret_cast<const QMultiMap<K,T> *>(container)->contains(_qkey, _qvalue);
                }
            }
        }
        return result;
    }

    jint count(JNIEnv *env, const void* container, jobject key, jobject value) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        jint result = 0;
        {
            jvalue jv;
            jv.l = key;
            K _qkey;
            void *_qkeyPtr = &_qkey;
            if(m_keyExternalToInternalConverter(env, nullptr, jv, _qkeyPtr, jValueType::l)){
                jv.l = value;
                T _qvalue;
                void *_qvaluePtr = &_qvalue;
                if(m_valueExternalToInternalConverter(env, nullptr, jv, _qvaluePtr, jValueType::l)){
                    result = jint(reinterpret_cast<const QMultiMap<K,T> *>(container)->count(_qkey, _qvalue));
                }
            }
        }
        return result;
    }

private:
    struct Iterator{
        typename QMultiMap<K,T>::iterator i;
    };
public:

    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        AssociativeConstIteratorAccess<QMultiMap, align1, size1, align2, size2, false>* access = new AssociativeIteratorAccess<QMultiMap, align1, size1, align2, size2>(
                    m_keyMetaTypeInfo,
                    m_keyInternalToExternalConverter,
                    m_valueMetaTypeInfo,
                    m_valueInternalToExternalConverter,
                    m_valueExternalToInternalConverter);
        jobject result = QtJambiAPI::convertQAssociativeIteratorToJavaObject(env,
                                      container.nativeId,
                                      new Iterator{reinterpret_cast<QMultiMap<K,T> *>(container.container)->begin()},
                                      [](void* ptr,bool){ delete reinterpret_cast<Iterator*>(ptr); },
                                      access);
        return result;
    }

    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        AssociativeConstIteratorAccess<QMultiMap, align1, size1, align2, size2, false>* access = new AssociativeIteratorAccess<QMultiMap, align1, size1, align2, size2>(
                    m_keyMetaTypeInfo,
                    m_keyInternalToExternalConverter,
                    m_valueMetaTypeInfo,
                    m_valueInternalToExternalConverter,
                    m_valueExternalToInternalConverter);
        jobject result = QtJambiAPI::convertQAssociativeIteratorToJavaObject(env,
                                      container.nativeId,
                                      new Iterator{reinterpret_cast<QMultiMap<K,T> *>(container.container)->end()},
                                      [](void* ptr,bool){ delete reinterpret_cast<Iterator*>(ptr); },
                                      access);
        return result;
    }

    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        AbstractAssociativeConstIteratorAccess* access = new AssociativeConstIteratorAccess<QMultiMap, align1, size1, align2, size2>(
                    m_keyMetaTypeInfo,
                    m_keyInternalToExternalConverter,
                    m_valueMetaTypeInfo,
                    m_valueInternalToExternalConverter);
        jobject result = QtJambiAPI::convertQAssociativeIteratorToJavaObject(env,
                                      container.nativeId,
                                      new typename QMultiMap<K,T>::const_iterator(reinterpret_cast<const QMultiMap<K,T> *>(container.container)->constBegin()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QMultiMap<K,T>::const_iterator*>(ptr); },
                                      access);
        return result;
    }

    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        AbstractAssociativeConstIteratorAccess* access = new AssociativeConstIteratorAccess<QMultiMap, align1, size1, align2, size2>(
                    m_keyMetaTypeInfo,
                    m_keyInternalToExternalConverter,
                    m_valueMetaTypeInfo,
                    m_valueInternalToExternalConverter);
        jobject result = QtJambiAPI::convertQAssociativeIteratorToJavaObject(env,
                                      container.nativeId,
                                      new typename QMultiMap<K,T>::const_iterator(reinterpret_cast<const QMultiMap<K,T> *>(container.container)->constEnd()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QMultiMap<K,T>::const_iterator*>(ptr); },
                                      access);
        return result;
    }

    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        jobject result = nullptr;
        {
            jvalue _key;
            _key.l = key;
            K _qkey;
            void *_qkeyPtr = &_qkey;
            if(m_keyExternalToInternalConverter(env, nullptr, _key, _qkeyPtr, jValueType::l)){
                AssociativeConstIteratorAccess<QMultiMap, align1, size1, align2, size2, false>* access = new AssociativeIteratorAccess<QMultiMap, align1, size1, align2, size2>(
                            m_keyMetaTypeInfo,
                            m_keyInternalToExternalConverter,
                            m_valueMetaTypeInfo,
                            m_valueInternalToExternalConverter,
                            m_valueExternalToInternalConverter);
                result = QtJambiAPI::convertQAssociativeIteratorToJavaObject(env,
                                              container.nativeId,
                                              new Iterator{reinterpret_cast<QMultiMap<K,T> *>(container.container)->find(_qkey)},
                                              [](void* ptr,bool){ delete reinterpret_cast<Iterator*>(ptr); },
                                              access);
            }
        }
        if(!result)
            result = end(env, container);
        return result;
    }

    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        jobject result = nullptr;
        {
            jvalue _key;
            _key.l = key;
            K _qkey;
            void *_qkeyPtr = &_qkey;
            if(m_keyExternalToInternalConverter(env, nullptr, _key, _qkeyPtr, jValueType::l)){
                AbstractAssociativeConstIteratorAccess* access = new AssociativeConstIteratorAccess<QMultiMap, align1, size1, align2, size2>(
                            m_keyMetaTypeInfo,
                            m_keyInternalToExternalConverter,
                            m_valueMetaTypeInfo,
                            m_valueInternalToExternalConverter);
                result = QtJambiAPI::convertQAssociativeIteratorToJavaObject(env,
                                              container.nativeId,
                                              new typename QMultiMap<K,T>::const_iterator(reinterpret_cast<const QMultiMap<K,T> *>(container.container)->constFind(_qkey)),
                                              [](void* ptr,bool){ delete reinterpret_cast<typename QMultiMap<K,T>::const_iterator*>(ptr); },
                                              access);
            }
        }
        if(!result)
            result = constEnd(env, container);
        return result;
    }

    jobject find(JNIEnv * env, const ExtendedContainerInfo& container, jobject key, jobject value) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        jobject result = nullptr;
        {
            jvalue jv;
            jv.l = key;
            K _qkey;
            void *_qkeyPtr = &_qkey;
            if(m_keyExternalToInternalConverter(env, nullptr, jv, _qkeyPtr, jValueType::l)){
                jv.l = value;
                T _qvalue;
                void *_qvaluePtr = &_qvalue;
                if(m_valueExternalToInternalConverter(env, nullptr, jv, _qvaluePtr, jValueType::l)){
                    AssociativeConstIteratorAccess<QMultiMap, align1, size1, align2, size2, false>* access = new AssociativeIteratorAccess<QMultiMap, align1, size1, align2, size2>(
                                                                                    m_keyMetaTypeInfo,
                                                                                    m_keyInternalToExternalConverter,
                                                                                    m_valueMetaTypeInfo,
                                                                                    m_valueInternalToExternalConverter,
                                                                                    m_valueExternalToInternalConverter);
                    result = QtJambiAPI::convertQAssociativeIteratorToJavaObject(env,
                                                  container.nativeId,
                                                  new Iterator{reinterpret_cast<QMultiMap<K,T> *>(container.container)->find(_qkey, _qvalue)},
                                                  [](void* ptr,bool){ delete reinterpret_cast<Iterator*>(ptr); },
                                                  access);
                }
            }
        }
        if(!result)
            result = end(env, container);
        return result;
    }

    jobject constFind(JNIEnv * env, const ConstExtendedContainerInfo& container, jobject key, jobject value) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        jobject result = nullptr;
        {
            jvalue jv;
            jv.l = key;
            K _qkey;
            void *_qkeyPtr = &_qkey;
            if(m_keyExternalToInternalConverter(env, nullptr, jv, _qkeyPtr, jValueType::l)){
                jv.l = value;
                T _qvalue;
                void *_qvaluePtr = &_qvalue;
                if(m_valueExternalToInternalConverter(env, nullptr, jv, _qvaluePtr, jValueType::l)){
                    AbstractAssociativeConstIteratorAccess* access = new AssociativeConstIteratorAccess<QMultiMap, align1, size1, align2, size2>(
                                                                                    m_keyMetaTypeInfo,
                                                                                    m_keyInternalToExternalConverter,
                                                                                    m_valueMetaTypeInfo,
                                                                                    m_valueInternalToExternalConverter);
                    result = QtJambiAPI::convertQAssociativeIteratorToJavaObject(env,
                                                  container.nativeId,
                                                  new typename QMultiMap<K,T>::const_iterator(reinterpret_cast<const QMultiMap<K,T> *>(container.container)->constFind(_qkey, _qvalue)),
                                                  [](void* ptr,bool){ delete reinterpret_cast<typename QMultiMap<K,T>::const_iterator*>(ptr); },
                                                  access);
                }
            }
        }
        if(!result)
            result = constEnd(env, container);
        return result;
    }

    jint remove(JNIEnv * env, const ContainerInfo& container,jobject key, jobject value) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        jint result = 0;
        {
            jvalue jv;
            jv.l = key;
            K _qkey;
            void *_qkeyPtr = &_qkey;
            if(m_keyExternalToInternalConverter(env, nullptr, jv, _qkeyPtr, jValueType::l)){
                jv.l = value;
                T _qvalue;
                void *_qvaluePtr = &_qvalue;
                if(m_valueExternalToInternalConverter(env, nullptr, jv, _qvaluePtr, jValueType::l)){
                    result = jint(reinterpret_cast<QMultiMap<K,T> *>(container.container)->remove(_qkey, _qvalue));
                }
            }
        }
        return result;
    }

    void replace(JNIEnv * env, const ContainerInfo& container,jobject key, jobject value) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        {
            jvalue jv;
            jv.l = key;
            K _qkey;
            void *_qkeyPtr = &_qkey;
            if(m_keyExternalToInternalConverter(env, nullptr, jv, _qkeyPtr, jValueType::l)){
                jv.l = value;
                T _qvalue;
                void *_qvaluePtr = &_qvalue;
                if(m_valueExternalToInternalConverter(env, nullptr, jv, _qvaluePtr, jValueType::l)){
                    reinterpret_cast<QMultiMap<K,T> *>(container.container)->replace(_qkey, _qvalue);
                }
            }
        }
    }

    void unite(JNIEnv *env, const ContainerInfo& container, ContainerAndAccessInfo& other) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        if (ContainerAPI::getAsQMultiMap(env, other.object, keyMetaType(), valueMetaType(), other.container, other.access)) {
            reinterpret_cast<QMultiMap<K,T> *>(container.container)->unite(*reinterpret_cast<QMultiMap<K,T> *>(other.container));
        }else{
            QMultiMap<K,T> tmp;
            jobject iterator = QtJambiAPI::entrySetIteratorOfJavaMap(env, other.object);
            while(QtJambiAPI::hasJavaIteratorNext(env, iterator)) {
                jobject entry = QtJambiAPI::nextOfJavaIterator(env, iterator);
                jobject key = QtJambiAPI::keyOfJavaMapEntry(env, entry);
                jobject valCollection = QtJambiAPI::valueOfJavaMapEntry(env, entry);
                jobject iterator2 = QtJambiAPI::iteratorOfJavaIterable(env, valCollection);
                while(QtJambiAPI::hasJavaIteratorNext(env, iterator2)) {
                    jobject val = QtJambiAPI::nextOfJavaIterator(env, iterator2);
                    jvalue jv;
                    jv.l = key;
                    K _qkey;
                    void *_qkeyPtr = &_qkey;
                    if(m_keyExternalToInternalConverter(env, nullptr, jv, _qkeyPtr, jValueType::l)){
                        jv.l = val;
                        T _qvalue;
                        void *_qvaluePtr = &_qvalue;
                        if(m_valueExternalToInternalConverter(env, nullptr, jv, _qvaluePtr, jValueType::l)){
                            tmp.insert(_qkey, _qvalue);
                        }
                    }
                }
            }
            reinterpret_cast<QMultiMap<K,T> *>(container.container)->unite(tmp);
        }
    }

    bool keyLessThan(JNIEnv *env, jobject key1, jobject key2) override {
        {
            QTJAMBI_KEY_VALUE_LOCKER(this);
            jvalue jv;
            jv.l = key1;
            K _qkey1;
            void *_qkey1Ptr = &_qkey1;
            if(m_keyExternalToInternalConverter(env, nullptr, jv, _qkey1Ptr, jValueType::l)){
                jv.l = key2;
                K _qkey2;
                void *_qkey2Ptr = &_qkey2;
                if(m_keyExternalToInternalConverter(env, nullptr, jv, _qkey2Ptr, jValueType::l)){
                    return _qkey1 < _qkey2;
                }
            }
        }
        return false;
    }

    bool isDetached(const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        return reinterpret_cast<const QMultiMap<K,T> *>(container)->isDetached();
    }

    void detach(const ContainerInfo& container) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        reinterpret_cast<QMultiMap<K,T> *>(container.container)->detach();
    }

    bool isSharedWith(const void* container, const void* container2) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        return reinterpret_cast<const QMultiMap<K,T> *>(container)->isSharedWith(*reinterpret_cast<const QMultiMap<K,T> *>(container2));
    }

    void swap(JNIEnv *, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        reinterpret_cast<QMultiMap<K,T> *>(container.container)->swap(*reinterpret_cast<QMultiMap<K,T> *>(container2.container));
    }

    class KeyValueIterator : public AbstractMapAccess::KeyValueIterator{
    public:
        GenericMultiMapAccess* access;
        typename QMultiMap<K,T>::ConstIterator current;
        typename QMultiMap<K,T>::ConstIterator end;
        KeyValueIterator(GenericMultiMapAccess* _access, const void* container)
            : access(_access){
            QTJAMBI_KEY_VALUE_LOCKER(access);
            current = reinterpret_cast<const QMultiMap<K,T>*>(container)->constBegin();
            end = reinterpret_cast<const QMultiMap<K,T>*>(container)->constEnd();
        }
        ~KeyValueIterator() override {};
        bool hasNext() override {return current!=end;};
        QPair<jobject,jobject> next(JNIEnv * env) override {
            QTJAMBI_KEY_VALUE_LOCKER(access);
            jvalue k;
            k.l = nullptr;
            jvalue v;
            v.l = nullptr;
            access->m_keyInternalToExternalConverter(env, nullptr, &current.key(), k, true);
            access->m_valueInternalToExternalConverter(env, nullptr, &current.value(), v, true);
            ++current;
            return {k.l, v.l};
        };
        QPair<const void*,const void*> next() override {
            QTJAMBI_KEY_VALUE_LOCKER(access);
            const void* key = access->m_keyDataType==AbstractContainerAccess::Value ? nullptr :
                                  QtJambiPrivate::ContainerContentDeref<K, size1==0>::deref(current.key());
            const void* value = access->m_valueDataType==AbstractContainerAccess::Value ? nullptr :
                                    QtJambiPrivate::ContainerContentDeref<T, size2==0>::deref(current.value());
            ++current;
            return {key, value};
        }
    };
    std::unique_ptr<AbstractMapAccess::KeyValueIterator> keyValueIterator(const void* container) override {
        return std::unique_ptr<AbstractMapAccess::KeyValueIterator>(new KeyValueIterator(this, container));
    }
};

template<size_t align1, size_t size1, size_t align2, size_t size2>
struct AssociativeContainerAccessFac<QMultiMap,align1,size1,align2,size2>{
    static constexpr AssociativeContainerAccessFactory factory(){
        return AssociativeContainerAccessFactory(GenericMultiMapAccess<align1,size1,align2,size2>::newInstance);
    }
};

}

#endif //defined(QTJAMBI_GENERIC_ACCESS)

#endif // CONTAINERACCESS_MULTIMAP_H
