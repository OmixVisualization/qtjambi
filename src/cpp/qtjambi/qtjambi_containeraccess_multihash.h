#ifndef QTJAMBI_CONTAINERACCESS_MULTIHASH_H
#define QTJAMBI_CONTAINERACCESS_MULTIHASH_H

#include <QtCore/QMultiHash>
#include <qtjambi/qtjambi_containeraccess.h>

namespace QtJambiPrivate {

template<size_t align1, size_t size1, size_t align2, size_t size2>
class GenericMultiHashAccess : public AbstractMultiHashAccess{
    typedef typename std::conditional<size1==0, void*, ContainerElement<size1, 0, false, align1>>::type K;
    typedef typename std::conditional<size2==0, void*, ContainerElement<size2, 1, false, align2>>::type T;
    MetaTypeInfo<0,size1==0> m_keyMetaTypeInfo;
    InternalToExternalConverter m_keyInternalToExternalConverter;
    ExternalToInternalConverter m_keyExternalToInternalConverter;
    MetaTypeInfo<1,size2==0> m_valueMetaTypeInfo;
    InternalToExternalConverter m_valueInternalToExternalConverter;
    ExternalToInternalConverter m_valueExternalToInternalConverter;
    GenericMultiHashAccess(
            const QMetaType& keyMetaType,
            const QHashFunction& keyHashFunction,
            const InternalToExternalConverter& keyInternalToExternalConverter,
            const ExternalToInternalConverter& keyExternalToInternalConverter,
            const QMetaType& valueMetaType,
            const QHashFunction& valueHashFunction,
            const InternalToExternalConverter& valueInternalToExternalConverter,
            const ExternalToInternalConverter& valueExternalToInternalConverter
            )
        :   AbstractMultiHashAccess(),
          m_keyMetaTypeInfo(keyMetaType, keyHashFunction),
          m_keyInternalToExternalConverter(keyInternalToExternalConverter),
          m_keyExternalToInternalConverter(keyExternalToInternalConverter),
          m_valueMetaTypeInfo(valueMetaType, valueHashFunction),
          m_valueInternalToExternalConverter(valueInternalToExternalConverter),
          m_valueExternalToInternalConverter(valueExternalToInternalConverter)
    {
    }

    GenericMultiHashAccess(const GenericMultiHashAccess<align1, size1, align2, size2>& other)
        :   AbstractMultiHashAccess(),
          m_keyMetaTypeInfo(other.m_keyMetaTypeInfo),
          m_keyInternalToExternalConverter(other.m_keyInternalToExternalConverter),
          m_keyExternalToInternalConverter(other.m_keyExternalToInternalConverter),
          m_valueMetaTypeInfo(other.m_valueMetaTypeInfo),
          m_valueInternalToExternalConverter(other.m_valueInternalToExternalConverter),
          m_valueExternalToInternalConverter(other.m_valueExternalToInternalConverter)
    {
    }

public:
    static AbstractMultiHashAccess* newInstance(
                                          const QMetaType& keyMetaType,
                                          const QHashFunction& keyHashFunction,
                                          const InternalToExternalConverter& keyInternalToExternalConverter,
                                          const ExternalToInternalConverter& keyExternalToInternalConverter,
                                          const QMetaType& valueMetaType,
                                          const QHashFunction& valueHashFunction,
                                          const InternalToExternalConverter& valueInternalToExternalConverter,
                                          const ExternalToInternalConverter& valueExternalToInternalConverter){
        return new GenericMultiHashAccess(
                                    keyMetaType,
                                    keyHashFunction,
                                    keyInternalToExternalConverter,
                                    keyExternalToInternalConverter,
                                    valueMetaType,
                                    valueHashFunction,
                                    valueInternalToExternalConverter,
                                    valueExternalToInternalConverter);
    }

    void dispose() override { delete this; }

    void analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) override {
        for(auto iter = reinterpret_cast<const QMultiHash<K,T>*>(container)->begin();
            iter != reinterpret_cast<const QMultiHash<K,T>*>(container)->end();
            ++iter
            ){
            if(!analyzer(pointer_from<decltype(iter.key())>::from(iter.key()),
                         pointer_from<decltype(iter.value())>::from(iter.value()), data)){
                break;
            }
        }
    }

    GenericMultiHashAccess<align1, size1, align2, size2>* clone() override{
        return new GenericMultiHashAccess<align1, size1, align2, size2>(*this);
    }

    void* createContainer() override {
        QTJAMBI_KEY_VALUE_LOCKER
        QMultiHash<K,T>* list = new QMultiHash<K,T>();
        return list;
    }
    void* copyContainer(const void* container) override {
        if(container){
            QTJAMBI_KEY_VALUE_LOCKER
            QMultiHash<K,T>* list = new QMultiHash<K,T>(*reinterpret_cast<const QMultiHash<K,T>*>(container));
            return list;
        }
        return createContainer();
    }
    void assign(void* container, const void* other) override {
        QTJAMBI_KEY_VALUE_LOCKER
        (*reinterpret_cast<QMultiHash<K,T>*>(container)) = (*reinterpret_cast<const QMultiHash<K,T>*>(other));
    }
    void deleteContainer(void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        delete reinterpret_cast<QMultiHash<K,T>*>(container);
    }
    int registerContainer(const QByteArray& containerTypeName) override {
        return qtjambi_register_bicontainer_type<QMultiHash<K,T>, size1, size2>(containerTypeName, m_keyMetaTypeInfo.metaType(), m_valueMetaTypeInfo.metaType());
    }
    PtrDeleterFunction containerDeleter() override {
        return nullptr;
    }
    bool isConstant() override {return false;}
    const QMetaType& keyMetaType() override {return m_keyMetaTypeInfo.metaType();}
    const QMetaType& valueMetaType() override {return m_valueMetaTypeInfo.metaType();}

    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        AbstractBiIteratorAccess* access = new BiIteratorAccess<QMultiHash, align1, size1, align2, size2>(
                                                                        m_keyMetaTypeInfo,
                                                                        m_keyInternalToExternalConverter,
                                                                        m_valueMetaTypeInfo,
                                                                        m_valueInternalToExternalConverter);
        jobject result = qtjambi_from_QMapIterator(env,
                                      ownerId,
                                      new typename QMultiHash<K,T>::const_iterator(reinterpret_cast<const QMultiHash<K,T> *>(container)->begin()),
                                      [](void* ptr){ delete reinterpret_cast<typename QMultiHash<K,T>::const_iterator*>(ptr); },
                                      access);
        return result;
    }

    void clear(JNIEnv *, void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        reinterpret_cast<QMultiHash<K,T> *>(container)->clear();
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
                result = reinterpret_cast<const QMultiHash<K,T> *>(container)->contains(_qkey);
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
                result = jint(reinterpret_cast<const QMultiHash<K,T> *>(container)->count(_qkey));
        }
        return result;
    }

    jobject end(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        AbstractBiIteratorAccess* access = new BiIteratorAccess<QMultiHash, align1, size1, align2, size2>(
                                                                        m_keyMetaTypeInfo,
                                                                        m_keyInternalToExternalConverter,
                                                                        m_valueMetaTypeInfo,
                                                                        m_valueInternalToExternalConverter);
        jobject result = qtjambi_from_QMapIterator(env,
                                      ownerId,
                                      new typename QMultiHash<K,T>::const_iterator(reinterpret_cast<const QMultiHash<K,T> *>(container)->end()),
                                      [](void* ptr){ delete reinterpret_cast<typename QMultiHash<K,T>::const_iterator*>(ptr); },
                                      access);
        return result;
    }

    jobject find(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override {
        QTJAMBI_KEY_VALUE_LOCKER
        jobject result = nullptr;
        {
            jvalue _key;
            _key.l = key;
            K _qkey;
            void *_qkeyPtr = &_qkey;
            if(m_keyExternalToInternalConverter(env, nullptr, _key, _qkeyPtr, jValueType::l)){
                AbstractBiIteratorAccess* access = new BiIteratorAccess<QMultiHash, align1, size1, align2, size2>(
                                                                                m_keyMetaTypeInfo,
                                                                                m_keyInternalToExternalConverter,
                                                                                m_valueMetaTypeInfo,
                                                                                m_valueInternalToExternalConverter);
                result = qtjambi_from_QMapIterator(env,
                                              ownerId,
                                              new typename QMultiHash<K,T>::const_iterator(reinterpret_cast<const QMultiHash<K,T> *>(container)->find(_qkey)),
                                              [](void* ptr){ delete reinterpret_cast<typename QMultiHash<K,T>::const_iterator*>(ptr); },
                                              access);
            }
        }
        if(!result)
            result = end(env, ownerId, container);
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
                    reinterpret_cast<QMultiHash<K,T> *>(container)->insert(_qkey, _qvalue);
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
                    K _qkey = reinterpret_cast<const QMultiHash<K,T> *>(container)->key(_qvalue, _qkey);
                    jv.l = nullptr;
                    if(m_keyInternalToExternalConverter(env, nullptr, &_qkey, &jv, true)){
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
        if(ContainerAccessFactory factory = ContainerAccessFactories::getAccessFactory(ContainerType::QList, align1, size1,
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                                                                                       false
#else
                                                                                       qtjambi_is_static_type(m_keyMetaTypeInfo.metaType())
#endif
                                                                                       )){
            const void* keys = new QList<K>(reinterpret_cast<const QMultiHash<K,T> *>(container)->keys());
            result = qtjambi_from_QList(env, keys, factory(m_keyMetaTypeInfo.metaType(),
                                                           m_keyMetaTypeInfo.hashFunction(),
                                                           m_keyInternalToExternalConverter,
                                                           m_keyExternalToInternalConverter));
        }
        return result;
    }

    jobject keys(JNIEnv * env, const void* container, jobject value) override {
        QTJAMBI_KEY_VALUE_LOCKER
        jobject result = nullptr;
        if(ContainerAccessFactory factory = ContainerAccessFactories::getAccessFactory(ContainerType::QList, align1, size1,
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                                                                                       false
#else
                                                                                       qtjambi_is_static_type(m_keyMetaTypeInfo.metaType())
#endif
                                                                                       )){
            jvalue jv;
            jv.l = value;
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            if(m_valueExternalToInternalConverter(env, nullptr, jv, _qvaluePtr, jValueType::l)){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                const void* keys = new QList<K>(reinterpret_cast<const QMultiHash<K,T> *>(container)->keys(_qvalue));
#else
                QList<K>* _keys = new QList<K>();
                if(size1==0 || m_keyMetaTypeInfo.metaType().isEqualityComparable()){
                    typename QMultiHash<K,T>::const_iterator i = reinterpret_cast<const QMultiHash<K,T> *>(container)->begin();
                    typename QMultiHash<K,T>::const_iterator end = reinterpret_cast<const QMultiHash<K,T> *>(container)->end();
                    while (i != end) {
                        if(size1==0){
                            if(i.value() == _qvalue)
                                _keys->append(i.key());
                        }else if (m_keyMetaTypeInfo.metaType().equals(&i.value(), &_qvalue))
                            _keys->append(i.key());
                        ++i;
                    }
                }
                const void* keys = _keys;
#endif
                result = qtjambi_from_QList(env, keys, factory(m_keyMetaTypeInfo.metaType(),
                                                               m_keyMetaTypeInfo.hashFunction(),
                                                               m_keyInternalToExternalConverter,
                                                               m_keyExternalToInternalConverter));
            }
        }
        return result;
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        if (qtjambi_is_QMultiHash(env, other, keyMetaType(), valueMetaType())) {
            if(void* ptr = qtjambi_to_object(env, other)){
                QTJAMBI_KEY_VALUE_LOCKER
                return *reinterpret_cast<const QMultiHash<K,T> *>(container)==*reinterpret_cast<QMultiHash<K,T> *>(ptr);
            }
        }else{
            QTJAMBI_KEY_VALUE_LOCKER
            QMultiHash<K,T> map;
            jobject iterator = qtjambi_map_entryset_iterator(env, other);
            while(qtjambi_iterator_has_next(env, iterator)){
                jobject __java_entry = qtjambi_iterator_next(env, iterator);
                jvalue jv;
                jv.l = qtjambi_map$entry_key(env, __java_entry);
                K _qkey;
                void *_qkeyPtr = &_qkey;
                if(m_keyExternalToInternalConverter(env, nullptr, jv, _qkeyPtr, jValueType::l)){
                    jv.l = qtjambi_map$entry_value(env, __java_entry);
                    T _qvalue;
                    void *_qvaluePtr = &_qvalue;
                    if(m_valueExternalToInternalConverter(env, nullptr, jv, _qvaluePtr, jValueType::l)){
                        map.insert(_qkey, _qvalue);
                    }
                }
            }
            return *reinterpret_cast<const QMultiHash<K,T> *>(container)==map;
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
                result = reinterpret_cast<QMultiHash<K,T> *>(container)->remove(_qkey);
            }
        }
        return result;
    }

    jint size(JNIEnv *, const void* container) override {
        jint result = 0;
        QTJAMBI_KEY_VALUE_LOCKER
        result = reinterpret_cast<const QMultiHash<K,T> *>(container)->size();
        return result;
    }



    jint capacity(JNIEnv *, const void* container) override {
        jint result = 0;
        QTJAMBI_KEY_VALUE_LOCKER
        result = reinterpret_cast<const QMultiHash<K,T> *>(container)->capacity();
        return result;
    }

    void reserve(JNIEnv *, void* container, jint size) override {
        QTJAMBI_KEY_VALUE_LOCKER
        reinterpret_cast<QMultiHash<K,T> *>(container)->reserve(size);
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
                T _qvalue = reinterpret_cast<QMultiHash<K,T> *>(container)->take(_qkey);
                jvalue _value;
                _value.l = nullptr;
                if(m_valueInternalToExternalConverter(env, nullptr, &_qvalue, &_value, true)){
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
                    T _qvalue = reinterpret_cast<const QMultiHash<K,T> *>(container)->value(_qkey, _qdefaultValue);
                    jv.l = nullptr;
                    if(m_valueInternalToExternalConverter(env, nullptr, &_qvalue, &jv, true)){
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
        if(ContainerAccessFactory factory = ContainerAccessFactories::getAccessFactory(ContainerType::QList, align2, size2,
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                                                                                       false
#else
                                                                                       qtjambi_is_static_type(m_valueMetaTypeInfo.metaType())
#endif
                                                                                )){
            const void* values = new QList<T>(reinterpret_cast<const QMultiHash<K,T> *>(container)->values());
            result = qtjambi_from_QList(env, values, factory(m_valueMetaTypeInfo.metaType(),
                                                             m_valueMetaTypeInfo.hashFunction(),
                                                             m_valueInternalToExternalConverter,
                                                             m_valueExternalToInternalConverter));
        }
        return result;
    }

    jobject uniqueKeys(JNIEnv * env, const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        jobject result = nullptr;
        if(ContainerAccessFactory factory = ContainerAccessFactories::getAccessFactory(ContainerType::QList, align1, size1,
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                                                                                       false
#else
                                                                                       qtjambi_is_static_type(m_keyMetaTypeInfo.metaType())
#endif
                                                                                       )){
            const void* keys = new QList<K>(reinterpret_cast<const QMultiHash<K,T> *>(container)->uniqueKeys());
            result = qtjambi_from_QList(env, keys, factory(m_keyMetaTypeInfo.metaType(),
                                                           m_keyMetaTypeInfo.hashFunction(),
                                                           m_keyInternalToExternalConverter,
                                                           m_keyExternalToInternalConverter));
        }
        return result;
    }

    jobject values(JNIEnv * env, const void* container, jobject key) override {
        QTJAMBI_KEY_VALUE_LOCKER
        jobject result = nullptr;
        if(ContainerAccessFactory factory = ContainerAccessFactories::getAccessFactory(ContainerType::QList, align2, size2,
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                                                                                       false
#else
                                                                                       qtjambi_is_static_type(m_valueMetaTypeInfo.metaType())
#endif
                                                                                    )){
            jvalue jv;
            jv.l = key;
            K _qkey;
            void *_qkeyPtr = &_qkey;
            if(m_keyExternalToInternalConverter(env, nullptr, jv, _qkeyPtr, jValueType::l)){
                const void* values = new QList<T>(reinterpret_cast<const QMultiHash<K,T> *>(container)->values(_qkey));
                result = qtjambi_from_QList(env, values, factory(m_valueMetaTypeInfo.metaType(),
                                                                 m_valueMetaTypeInfo.hashFunction(),
                                                                 m_valueInternalToExternalConverter,
                                                                 m_valueExternalToInternalConverter));
            }
        }
        return result;
    }

    jboolean contains(JNIEnv *env, const void* container, jobject key, jobject value) override {
        QTJAMBI_KEY_VALUE_LOCKER
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
                    result = reinterpret_cast<const QMultiHash<K,T> *>(container)->contains(_qkey, _qvalue);
                }
            }
        }
        return result;
    }

    jint count(JNIEnv *env, const void* container, jobject key, jobject value) override {
        QTJAMBI_KEY_VALUE_LOCKER
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
                    result = jint(reinterpret_cast<const QMultiHash<K,T> *>(container)->count(_qkey, _qvalue));
                }
            }
        }
        return result;
    }

    jobject find(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key, jobject value) override {
        QTJAMBI_KEY_VALUE_LOCKER
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
                    AbstractBiIteratorAccess* access = new BiIteratorAccess<QMultiHash, align1, size1, align2, size2>(
                                                                                    m_keyMetaTypeInfo,
                                                                                    m_keyInternalToExternalConverter,
                                                                                    m_valueMetaTypeInfo,
                                                                                    m_valueInternalToExternalConverter);
                    result = qtjambi_from_QMapIterator(env,
                                                  ownerId,
                                                  new typename QMultiHash<K,T>::const_iterator(reinterpret_cast<const QMultiHash<K,T> *>(container)->find(_qkey, _qvalue)),
                                                  [](void* ptr){ delete reinterpret_cast<typename QMultiHash<K,T>::const_iterator*>(ptr); },
                                                  access);
                }
            }
        }
        if(!result)
            result = end(env, ownerId, container);
        return result;
    }

    jint remove(JNIEnv * env, void* container,jobject key, jobject value) override {
        QTJAMBI_KEY_VALUE_LOCKER
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
                    result = jint(reinterpret_cast<QMultiHash<K,T> *>(container)->remove(_qkey, _qvalue));
                }
            }
        }
        return result;
    }

    void replace(JNIEnv * env, void* container,jobject key, jobject value) override {
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
                    reinterpret_cast<QMultiHash<K,T> *>(container)->replace(_qkey, _qvalue);
                }
            }
        }
    }

    void unite(JNIEnv *env, void* container, jobject other) override {
        if (qtjambi_is_QMultiHash(env, other, keyMetaType(), valueMetaType())) {
            if(void* ptr = qtjambi_to_object(env, other)){
                QTJAMBI_KEY_VALUE_LOCKER
                reinterpret_cast<QMultiHash<K,T> *>(container)->unite(*reinterpret_cast<QMultiHash<K,T> *>(ptr));
            }
        }
    }
};

template<size_t align1, size_t size1, size_t align2, size_t size2>
struct BiContainerAccessFac<QMultiHash,align1,size1,align2,size2>{
    static constexpr BiContainerAccessFactory factory(){
        return BiContainerAccessFactory(GenericMultiHashAccess<align1,size1,align2,size2>::newInstance);
    }
};

}

#endif // QTJAMBI_CONTAINERACCESS_MULTIHASH_H
