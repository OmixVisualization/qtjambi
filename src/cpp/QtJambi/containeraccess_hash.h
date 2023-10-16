/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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


#ifndef CONTAINERACCESS_HASH_H
#define CONTAINERACCESS_HASH_H

#include <QtCore/QHash>
#include "containeraccess.h"

namespace ContainerAccessAPI {

template<size_t align1, size_t size1, size_t align2, size_t size2>
class GenericHashAccess : public AbstractHashAccess{
    typedef typename std::conditional<size1==0, void*, ContainerElement<size1, 0, false, align1>>::type K;
    typedef typename std::conditional<size2==0, void*, ContainerElement<size2, 1, false, align2>>::type T;
    MetaTypeInfo<0,size1==0> m_keyMetaTypeInfo;
    InternalToExternalConverter m_keyInternalToExternalConverter;
    ExternalToInternalConverter m_keyExternalToInternalConverter;
    MetaTypeInfo<1,size2==0> m_valueMetaTypeInfo;
    InternalToExternalConverter m_valueInternalToExternalConverter;
    ExternalToInternalConverter m_valueExternalToInternalConverter;
    GenericHashAccess(
            const QMetaType& keyMetaType,
            const QHashFunction& keyHashFunction,
            const InternalToExternalConverter& keyInternalToExternalConverter,
            const ExternalToInternalConverter& keyExternalToInternalConverter,
            const QMetaType& valueMetaType,
            const QHashFunction& valueHashFunction,
            const InternalToExternalConverter& valueInternalToExternalConverter,
            const ExternalToInternalConverter& valueExternalToInternalConverter
            )
        :   AbstractHashAccess(),
          m_keyMetaTypeInfo(keyMetaType, keyHashFunction),
          m_keyInternalToExternalConverter(keyInternalToExternalConverter),
          m_keyExternalToInternalConverter(keyExternalToInternalConverter),
          m_valueMetaTypeInfo(valueMetaType, valueHashFunction),
          m_valueInternalToExternalConverter(valueInternalToExternalConverter),
          m_valueExternalToInternalConverter(valueExternalToInternalConverter)
    {
    }

    GenericHashAccess(const GenericHashAccess<align1, size1, align2, size2>& other)
        :   AbstractHashAccess(),
          m_keyMetaTypeInfo(other.m_keyMetaTypeInfo),
          m_keyInternalToExternalConverter(other.m_keyInternalToExternalConverter),
          m_keyExternalToInternalConverter(other.m_keyExternalToInternalConverter),
          m_valueMetaTypeInfo(other.m_valueMetaTypeInfo),
          m_valueInternalToExternalConverter(other.m_valueInternalToExternalConverter),
          m_valueExternalToInternalConverter(other.m_valueExternalToInternalConverter)
    {
    }

public:
    static AbstractHashAccess* newInstance(const QMetaType& keyMetaType,
                                          const QHashFunction& keyHashFunction,
                                          const InternalToExternalConverter& keyInternalToExternalConverter,
                                          const ExternalToInternalConverter& keyExternalToInternalConverter,
                                          const QMetaType& valueMetaType,
                                          const QHashFunction& valueHashFunction,
                                          const InternalToExternalConverter& valueInternalToExternalConverter,
                                          const ExternalToInternalConverter& valueExternalToInternalConverter){
        return new GenericHashAccess(keyMetaType,
                                    keyHashFunction,
                                     keyInternalToExternalConverter,
                                    keyExternalToInternalConverter,
                                    valueMetaType,
                                    valueHashFunction,
                                     valueInternalToExternalConverter,
                                    valueExternalToInternalConverter);
    }

    void dispose() override { delete this; }

    GenericHashAccess<align1, size1, align2, size2>* clone() override{
        return new GenericHashAccess<align1, size1, align2, size2>(*this);
    }

    void analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) override {
        for(auto iter = reinterpret_cast<const QHash<K,T>*>(container)->begin();
            iter != reinterpret_cast<const QHash<K,T>*>(container)->end();
            ++iter
            ){
            if(!analyzer(QtJambiPrivate::pointer_from<decltype(iter.key())>::from(iter.key()),
                         QtJambiPrivate::pointer_from<decltype(iter.value())>::from(iter.value()), data)){
                break;
            }
        }
    }
    size_t sizeOf() override {
        return sizeof(QHash<K,T>);
    }
    void assign(void* container, const void* other) override {
        QTJAMBI_KEY_VALUE_LOCKER
        (*reinterpret_cast<QHash<K,T>*>(container)) = (*reinterpret_cast<const QHash<K,T>*>(other));
    }
    void* constructContainer(void* placement, const void* copyOf = nullptr) override {
        QTJAMBI_KEY_VALUE_LOCKER
        if(copyOf){
            return new(placement) QHash<K,T>(*reinterpret_cast<const QHash<K,T>*>(copyOf));
        }else{
            return new(placement) QHash<K,T>();
        }
    }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* move) override {
        QTJAMBI_KEY_VALUE_LOCKER
        return new(placement) QHash<K,T>(std::move(*reinterpret_cast<const QHash<K,T>*>(move)));
    }
#endif
    bool destructContainer(void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        reinterpret_cast<QHash<K,T>*>(container)->~QHash<K,T>();
        return true;
    }
    int registerContainer(const QByteArray& containerTypeName) override {
        return QtJambiPrivate::registerAssociativeContainerType<QHash<K,T>, size1, size2>(containerTypeName, m_keyMetaTypeInfo.metaType(), m_valueMetaTypeInfo.metaType());
    }
    bool isConstant() override {return false;}
    const QMetaType& keyMetaType() override {return m_keyMetaTypeInfo.metaType();}
    const QMetaType& valueMetaType() override {return m_valueMetaTypeInfo.metaType();}

    void clear(JNIEnv *, void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        reinterpret_cast<QHash<K,T> *>(container)->clear();
    }

    jboolean contains(JNIEnv * env, const void* container, jobject key) override {
        QTJAMBI_KEY_VALUE_LOCKER
        bool result = false;
        {
            jvalue _key;
            _key.l = key;
            K _qkey;
            void *_qkeyPtr = &_qkey;
            if(m_keyExternalToInternalConverter(env, nullptr, _key, _qkeyPtr, jValueType::l))
                result = reinterpret_cast<const QHash<K,T> *>(container)->contains(_qkey);
        }
        return result;
    }

    jint count(JNIEnv * env, const void* container, jobject key) override {
        QTJAMBI_KEY_VALUE_LOCKER
        jint result = 0;
        {
            jvalue _key;
            _key.l = key;
            K _qkey;
            void *_qkeyPtr = &_qkey;
            if(m_keyExternalToInternalConverter(env, nullptr, _key, _qkeyPtr, jValueType::l))
                result = jint(reinterpret_cast<const QHash<K,T> *>(container)->count(_qkey));
        }
        return result;
    }

private:
    struct Iterator{
        typename QHash<K,T>::iterator i;
    };
public:

    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        AssociativeConstIteratorAccess<QHash, align1, size1, align2, size2, false>* access = new AssociativeIteratorAccess<QHash, align1, size1, align2, size2>(
                    m_keyMetaTypeInfo,
                    m_keyInternalToExternalConverter,
                    m_valueMetaTypeInfo,
                    m_valueInternalToExternalConverter,
                    m_valueExternalToInternalConverter);
        jobject result = QtJambiAPI::convertQAssociativeIteratorToJavaObject(env,
                                      ownerId,
                                      new Iterator{reinterpret_cast<QHash<K,T> *>(container)->begin()},
                                      [](void* ptr,bool){ delete reinterpret_cast<Iterator*>(ptr); },
                                      access);
        return result;
    }

    jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        AssociativeConstIteratorAccess<QHash, align1, size1, align2, size2, false>* access = new AssociativeIteratorAccess<QHash, align1, size1, align2, size2>(
                    m_keyMetaTypeInfo,
                    m_keyInternalToExternalConverter,
                    m_valueMetaTypeInfo,
                    m_valueInternalToExternalConverter,
                    m_valueExternalToInternalConverter);
        jobject result = QtJambiAPI::convertQAssociativeIteratorToJavaObject(env,
                                      ownerId,
                                      new Iterator{reinterpret_cast<QHash<K,T> *>(container)->end()},
                                      [](void* ptr,bool){ delete reinterpret_cast<Iterator*>(ptr); },
                                      access);
        return result;
    }

    jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        AbstractAssociativeConstIteratorAccess* access = new AssociativeConstIteratorAccess<QHash, align1, size1, align2, size2>(
                    m_keyMetaTypeInfo,
                    m_keyInternalToExternalConverter,
                    m_valueMetaTypeInfo,
                    m_valueInternalToExternalConverter);
        jobject result = QtJambiAPI::convertQAssociativeIteratorToJavaObject(env,
                                      ownerId,
                                      new typename QHash<K,T>::const_iterator(reinterpret_cast<const QHash<K,T> *>(container)->constBegin()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QHash<K,T>::const_iterator*>(ptr); },
                                      access);
        return result;
    }

    jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        AbstractAssociativeConstIteratorAccess* access = new AssociativeConstIteratorAccess<QHash, align1, size1, align2, size2>(
                    m_keyMetaTypeInfo,
                    m_keyInternalToExternalConverter,
                    m_valueMetaTypeInfo,
                    m_valueInternalToExternalConverter);
        jobject result = QtJambiAPI::convertQAssociativeIteratorToJavaObject(env,
                                      ownerId,
                                      new typename QHash<K,T>::const_iterator(reinterpret_cast<const QHash<K,T> *>(container)->constEnd()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QHash<K,T>::const_iterator*>(ptr); },
                                      access);
        return result;
    }

    jobject find(JNIEnv * env, QtJambiNativeID ownerId, void* container, jobject key) override {
        QTJAMBI_KEY_VALUE_LOCKER
        jobject result = nullptr;
        {
            jvalue _key;
            _key.l = key;
            K _qkey;
            void *_qkeyPtr = &_qkey;
            if(m_keyExternalToInternalConverter(env, nullptr, _key, _qkeyPtr, jValueType::l)){
                AssociativeConstIteratorAccess<QHash, align1, size1, align2, size2, false>* access = new AssociativeIteratorAccess<QHash, align1, size1, align2, size2>(
                            m_keyMetaTypeInfo,
                            m_keyInternalToExternalConverter,
                            m_valueMetaTypeInfo,
                            m_valueInternalToExternalConverter,
                            m_valueExternalToInternalConverter);
                result = QtJambiAPI::convertQAssociativeIteratorToJavaObject(env,
                                              ownerId,
                                              new Iterator{reinterpret_cast<QHash<K,T> *>(container)->find(_qkey)},
                                              [](void* ptr,bool){ delete reinterpret_cast<Iterator*>(ptr); },
                                              access);
            }
        }
        if(!result)
            result = end(env, ownerId, container);
        return result;
    }

    jobject constFind(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override {
        QTJAMBI_KEY_VALUE_LOCKER
        jobject result = nullptr;
        {
            jvalue _key;
            _key.l = key;
            K _qkey;
            void *_qkeyPtr = &_qkey;
            if(m_keyExternalToInternalConverter(env, nullptr, _key, _qkeyPtr, jValueType::l)){
                AbstractAssociativeConstIteratorAccess* access = new AssociativeConstIteratorAccess<QHash, align1, size1, align2, size2>(
                            m_keyMetaTypeInfo,
                            m_keyInternalToExternalConverter,
                            m_valueMetaTypeInfo,
                            m_valueInternalToExternalConverter);
                result = QtJambiAPI::convertQAssociativeIteratorToJavaObject(env,
                                              ownerId,
                                              new typename QHash<K,T>::const_iterator(reinterpret_cast<const QHash<K,T> *>(container)->constFind(_qkey)),
                                              [](void* ptr,bool){ delete reinterpret_cast<typename QHash<K,T>::const_iterator*>(ptr); },
                                              access);
            }
        }
        if(!result)
            result = constEnd(env, ownerId, container);
        return result;
    }

    void insert(JNIEnv *env, void* container, jobject key, jobject value) override {
        QTJAMBI_KEY_VALUE_LOCKER
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
                    reinterpret_cast<QHash<K,T> *>(container)->insert(_qkey, _qvalue);
                }
            }
        }
    }

    jobject key(JNIEnv * env, const void* container, jobject value, jobject defaultKey) override {
        QTJAMBI_KEY_VALUE_LOCKER
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
                    K _qkey = reinterpret_cast<const QHash<K,T> *>(container)->key(_qvalue, _qdefaultkey);
                    jv.l = nullptr;
                    if(m_keyInternalToExternalConverter(env, nullptr, &_qkey, jv, true)){
                        result = jv.l;
                    }
                }

            }
        }
        return result;
    }

    jobject keys(JNIEnv * env, const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        jobject result = nullptr;
        AbstractContainerAccess* access = AbstractContainerAccess::create(env, SequentialContainerType::QList, m_keyMetaTypeInfo.metaType());
        if(!access)
            access = AbstractContainerAccess::create(env, SequentialContainerType::QList, m_keyMetaTypeInfo.metaType(),
                                                                                align1, size1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                                AbstractContainerAccess::isStaticType(m_keyMetaTypeInfo.metaType()),
#endif
                                                                                AbstractContainerAccess::isPointerType(m_keyMetaTypeInfo.metaType()),
                                                                                m_keyMetaTypeInfo.hashFunction(),
                                                                                m_keyInternalToExternalConverter,
                                                                                m_keyExternalToInternalConverter
                                                                            );
        if(access){
            const void* keys = new QList<K>(reinterpret_cast<const QHash<K,T> *>(container)->keys());
            result = objectFromQList(env, keys, access);
        }
        return result;
    }

    jobject keys(JNIEnv * env, const void* container, jobject value) override {
        QTJAMBI_KEY_VALUE_LOCKER
        jobject result = nullptr;
        AbstractContainerAccess* access = AbstractContainerAccess::create(env, SequentialContainerType::QList, m_keyMetaTypeInfo.metaType());
        if(!access)
            access = AbstractContainerAccess::create(env, SequentialContainerType::QList, m_keyMetaTypeInfo.metaType(),
                                                                                 align1, size1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                                 AbstractContainerAccess::isStaticType(m_keyMetaTypeInfo.metaType()),
#endif
                                                                                 AbstractContainerAccess::isPointerType(m_keyMetaTypeInfo.metaType()),
                                                                                 m_keyMetaTypeInfo.hashFunction(),
                                                                                 m_keyInternalToExternalConverter,
                                                                                 m_keyExternalToInternalConverter
                                                                            );
        if(access){
            jvalue jv;
            jv.l = value;
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            if(m_valueExternalToInternalConverter(env, nullptr, jv, _qvaluePtr, jValueType::l)){
                const void* keys = new QList<K>(reinterpret_cast<const QHash<K,T> *>(container)->keys(_qvalue));
                result = objectFromQList(env, keys, access);
            }
        }
        return result;
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        void* ptr{nullptr};
        if (ContainerAPI::getAsQHash(env, other, keyMetaType(), valueMetaType(), ptr)) {
            QTJAMBI_KEY_VALUE_LOCKER
            bool equals = *reinterpret_cast<const QHash<K,T> *>(container)==*reinterpret_cast<const QHash<K,T> *>(ptr);
            return equals;
        }else{
            QTJAMBI_KEY_VALUE_LOCKER
            QHash<K,T> map;
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
            return *reinterpret_cast<const QHash<K,T> *>(container)==map;
        }
        return false;
    }

    jint remove(JNIEnv * env, void* container,jobject key) override {
        QTJAMBI_KEY_VALUE_LOCKER
        jint result = 0;
        {
            jvalue _key;
            _key.l = key;
            K _qkey;
            void *_qkeyPtr = &_qkey;
            if(m_keyExternalToInternalConverter(env, nullptr, _key, _qkeyPtr, jValueType::l)){
                result = reinterpret_cast<QHash<K,T> *>(container)->remove(_qkey);
            }
        }
        return result;
    }

    jint size(JNIEnv *, const void* container) override {
        jint result = 0;
        QTJAMBI_KEY_VALUE_LOCKER
        result = reinterpret_cast<const QHash<K,T> *>(container)->size();
        return result;
    }

    jint capacity(JNIEnv *, const void* container) override {
        jint result = 0;
        QTJAMBI_KEY_VALUE_LOCKER
        result = reinterpret_cast<const QHash<K,T> *>(container)->capacity();
        return result;
    }

    void reserve(JNIEnv *, void* container, jint size) override {
        QTJAMBI_KEY_VALUE_LOCKER
        reinterpret_cast<QHash<K,T> *>(container)->reserve(size);
    }

    jobject take(JNIEnv *env, void* container, jobject key) override {
        QTJAMBI_KEY_VALUE_LOCKER
        jobject result = nullptr;
        {
            jvalue _key;
            _key.l = key;
            K _qkey;
            void *_qkeyPtr = &_qkey;
            if(m_keyExternalToInternalConverter(env, nullptr, _key, _qkeyPtr, jValueType::l)){
                T _qvalue = reinterpret_cast<QHash<K,T> *>(container)->take(_qkey);
                jvalue _value;
                _value.l = nullptr;
                if(m_valueInternalToExternalConverter(env, nullptr, &_qvalue, _value, true)){
                    result = _value.l;
                }
            }
        }
        return result;
    }

    jobject value(JNIEnv * env, const void* container, jobject key, jobject defaultValue) override {
        jobject result = nullptr;
        QTJAMBI_KEY_VALUE_LOCKER
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
                    T _qvalue = reinterpret_cast<const QHash<K,T> *>(container)->value(_qkey, _qdefaultValue);
                    jv.l = nullptr;
                    if(m_valueInternalToExternalConverter(env, nullptr, &_qvalue, jv, true)){
                        result = jv.l;
                    }
                }
            }
        }
        return result;
    }

    jobject values(JNIEnv * env, const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        jobject result = nullptr;
        AbstractContainerAccess* access = AbstractContainerAccess::create(env, SequentialContainerType::QList, m_valueMetaTypeInfo.metaType());
        if(!access)
            access = AbstractContainerAccess::create(env, SequentialContainerType::QList, m_valueMetaTypeInfo.metaType(),
                                                      align2, size2,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                      AbstractContainerAccess::isStaticType(m_valueMetaTypeInfo.metaType()),
#endif
                                                      AbstractContainerAccess::isPointerType(m_valueMetaTypeInfo.metaType()),
                                                      m_valueMetaTypeInfo.hashFunction(),
                                                      m_valueInternalToExternalConverter,
                                                      m_valueExternalToInternalConverter
                                                    );
        if(access){
            const void* values = new QList<T>(reinterpret_cast<const QHash<K,T> *>(container)->values());
            result = objectFromQList(env, values, access);
        }
        return result;
    }
};

template<size_t align1, size_t size1, size_t align2, size_t size2>
struct AssociativeContainerAccessFac<QHash,align1,size1,align2,size2>{
    static constexpr AssociativeContainerAccessFactory factory(){
        return AssociativeContainerAccessFactory(GenericHashAccess<align1,size1,align2,size2>::newInstance);
    }
};

}

#endif // CONTAINERACCESS_HASH_H
