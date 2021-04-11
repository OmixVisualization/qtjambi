#ifndef QTJAMBI_CONTAINERACCESS_MULTIMAP_H
#define QTJAMBI_CONTAINERACCESS_MULTIMAP_H

#include <QtCore/QMultiMap>
#include <qtjambi/qtjambi_containeraccess.h>

namespace QtJambiPrivate {

template<size_t align1, size_t size1, size_t align2, size_t size2>
class GenericMultiMapAccess : public AbstractMultiMapAccess{
    typedef typename std::conditional<size1==0, void*, ContainerElement<size1, 0, false, align1>>::type K;
    typedef typename std::conditional<size2==0, void*, ContainerElement<size2, 1, false, align2>>::type T;
    MetaTypeInfo<0,size1==0> m_keyMetaTypeInfo;
    InternalToExternalConverter m_keyInternalToExternalConverter;
    ExternalToInternalConverter m_keyExternalToInternalConverter;
    MetaTypeInfo<1,size2==0> m_valueMetaTypeInfo;
    InternalToExternalConverter m_valueInternalToExternalConverter;
    ExternalToInternalConverter m_valueExternalToInternalConverter;
    GenericMultiMapAccess(
            const QMetaType& keyMetaType,
            const QHashFunction& keyHashFunction,
            const InternalToExternalConverter& keyInternalToExternalConverter,
            const ExternalToInternalConverter& keyExternalToInternalConverter,
            const QMetaType& valueMetaType,
            const QHashFunction& valueHashFunction,
            const InternalToExternalConverter& valueInternalToExternalConverter,
            const ExternalToInternalConverter& valueExternalToInternalConverter
            )
        :   AbstractMultiMapAccess(),
          m_keyMetaTypeInfo(keyMetaType, keyHashFunction),
          m_keyInternalToExternalConverter(keyInternalToExternalConverter),
          m_keyExternalToInternalConverter(keyExternalToInternalConverter),
          m_valueMetaTypeInfo(valueMetaType, valueHashFunction),
          m_valueInternalToExternalConverter(valueInternalToExternalConverter),
          m_valueExternalToInternalConverter(valueExternalToInternalConverter)
    {
    }

    GenericMultiMapAccess(const GenericMultiMapAccess<align1, size1, align2, size2>& other)
        :   AbstractMultiMapAccess(),
          m_keyMetaTypeInfo(other.m_keyMetaTypeInfo),
          m_keyInternalToExternalConverter(other.m_keyInternalToExternalConverter),
          m_keyExternalToInternalConverter(other.m_keyExternalToInternalConverter),
          m_valueMetaTypeInfo(other.m_valueMetaTypeInfo),
          m_valueInternalToExternalConverter(other.m_valueInternalToExternalConverter),
          m_valueExternalToInternalConverter(other.m_valueExternalToInternalConverter)
    {
    }

public:
    static AbstractMultiMapAccess* newInstance(
                                          const QMetaType& keyMetaType,
                                          const QHashFunction& keyHashFunction,
                                          const InternalToExternalConverter& keyInternalToExternalConverter,
                                          const ExternalToInternalConverter& keyExternalToInternalConverter,
                                          const QMetaType& valueMetaType,
                                          const QHashFunction& valueHashFunction,
                                          const InternalToExternalConverter& valueInternalToExternalConverter,
                                          const ExternalToInternalConverter& valueExternalToInternalConverter){
        return new GenericMultiMapAccess(
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

    GenericMultiMapAccess<align1, size1, align2, size2>* clone() override{
        return new GenericMultiMapAccess<align1, size1, align2, size2>(*this);
    }

    void analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) override {
        for(auto iter = reinterpret_cast<const QMultiMap<K,T>*>(container)->begin();
            iter != reinterpret_cast<const QMultiMap<K,T>*>(container)->end();
            ++iter
            ){
            if(!analyzer(pointer_from<decltype(iter.key())>::from(iter.key()),
                         pointer_from<decltype(iter.value())>::from(iter.value()), data)){
                break;
            }
        }
    }

    void* createContainer() override {
        QTJAMBI_KEY_VALUE_LOCKER
        QMultiMap<K,T>* list = new QMultiMap<K,T>();
        return list;
    }
    void* copyContainer(const void* container) override {
        if(container){
            QTJAMBI_KEY_VALUE_LOCKER
            QMultiMap<K,T>* list = new QMultiMap<K,T>(*reinterpret_cast<const QMultiMap<K,T>*>(container));
            return list;
        }
        return createContainer();
    }
    void assign(void* container, const void* other) override {
        QTJAMBI_KEY_VALUE_LOCKER
        (*reinterpret_cast<QMultiMap<K,T>*>(container)) = (*reinterpret_cast<const QMultiMap<K,T>*>(other));
    }
    void deleteContainer(void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        delete reinterpret_cast<QMultiMap<K,T>*>(container);
    }
    int registerContainer(const QByteArray& containerTypeName) override {
        return qtjambi_register_bicontainer_type<QMultiMap<K,T>, size1, size2>(containerTypeName, m_keyMetaTypeInfo.metaType(), m_valueMetaTypeInfo.metaType());
    }
    PtrDeleterFunction containerDeleter() override {
        return nullptr;
    }
    bool isConstant() override {return false;}
    const QMetaType& keyMetaType() override {return m_keyMetaTypeInfo.metaType();}
    const QMetaType& valueMetaType() override {return m_valueMetaTypeInfo.metaType();}

    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        AbstractBiIteratorAccess* access = new BiIteratorAccess<QMultiMap, align1, size1, align2, size2>(
                    m_keyMetaTypeInfo,
                    m_keyInternalToExternalConverter,
                    m_valueMetaTypeInfo,
                    m_valueInternalToExternalConverter);
        jobject result = qtjambi_from_QMapIterator(env,
                                      ownerId,
                                      new typename QMultiMap<K,T>::const_iterator(reinterpret_cast<const QMultiMap<K,T> *>(container)->begin()),
                                      [](void* ptr){ delete reinterpret_cast<typename QMultiMap<K,T>::const_iterator*>(ptr); },
                                      access);
        return result;
    }

    void clear(JNIEnv *, void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        reinterpret_cast<QMultiMap<K,T> *>(container)->clear();
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
                result = reinterpret_cast<const QMultiMap<K,T> *>(container)->contains(_qkey);
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
                result = jint(reinterpret_cast<const QMultiMap<K,T> *>(container)->count(_qkey));
        }
        return result;
    }

    jobject end(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        AbstractBiIteratorAccess* access = new BiIteratorAccess<QMultiMap, align1, size1, align2, size2>(
                    m_keyMetaTypeInfo,
                    m_keyInternalToExternalConverter,
                    m_valueMetaTypeInfo,
                    m_valueInternalToExternalConverter);
        jobject result = qtjambi_from_QMapIterator(env,
                                      ownerId,
                                      new typename QMultiMap<K,T>::const_iterator(reinterpret_cast<const QMultiMap<K,T> *>(container)->end()),
                                      [](void* ptr){ delete reinterpret_cast<typename QMultiMap<K,T>::const_iterator*>(ptr); },
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
                AbstractBiIteratorAccess* access = new BiIteratorAccess<QMultiMap, align1, size1, align2, size2>(
                            m_keyMetaTypeInfo,
                            m_keyInternalToExternalConverter,
                            m_valueMetaTypeInfo,
                            m_valueInternalToExternalConverter);
                result = qtjambi_from_QMapIterator(env,
                                              ownerId,
                                              new typename QMultiMap<K,T>::const_iterator(reinterpret_cast<const QMultiMap<K,T> *>(container)->find(_qkey)),
                                              [](void* ptr){ delete reinterpret_cast<typename QMultiMap<K,T>::const_iterator*>(ptr); },
                                              access);
            }
        }
        if(!result)
            result = end(env, ownerId, container);
        return result;
    }

    jobject first(JNIEnv * env, const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        jobject result = nullptr;
        {
            const T& first = reinterpret_cast<const QMultiMap<K,T> *>(container)->first();
            jvalue _value;
            _value.l = nullptr;
            if(m_valueInternalToExternalConverter(env, nullptr, &first, &_value, true)){
                result = _value.l;
            }
        }
        return result;
    }

    jobject firstKey(JNIEnv * env, const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        jobject result = nullptr;
        {
            const K& firstKey = reinterpret_cast<const QMultiMap<K,T> *>(container)->firstKey();
            jvalue _value;
            _value.l = nullptr;
            if(m_keyInternalToExternalConverter(env, nullptr, &firstKey, &_value, true)){
                result = _value.l;
            }
        }
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
                    reinterpret_cast<QMultiMap<K,T> *>(container)->insert(_qkey, _qvalue);
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
                    K _qkey = reinterpret_cast<const QMultiMap<K,T> *>(container)->key(_qvalue, _qkey);
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
            const void* keys = new QList<K>(reinterpret_cast<const QMultiMap<K,T> *>(container)->keys());
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
        {
            jvalue jv;
            jv.l = value;
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            if(m_valueExternalToInternalConverter(env, nullptr, jv, _qvaluePtr, jValueType::l)){
                if(ContainerAccessFactory factory = ContainerAccessFactories::getAccessFactory(ContainerType::QList, align1, size1,
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                                                                                                false
#else
                                                                                                qtjambi_is_static_type(m_keyMetaTypeInfo.metaType())
#endif
                                                                                               )){
                    const void* keys = new QList<K>(reinterpret_cast<const QMultiMap<K,T> *>(container)->keys(_qvalue));
                    result = qtjambi_from_QList(env, keys, factory(m_keyMetaTypeInfo.metaType(),
                                                                   m_keyMetaTypeInfo.hashFunction(),
                                                                   m_keyInternalToExternalConverter,
                                                                   m_keyExternalToInternalConverter));
                }
            }
        }
        return result;
    }

    jobject last(JNIEnv * env, const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        jobject result = nullptr;
        {
            const T& last = reinterpret_cast<const QMultiMap<K,T> *>(container)->last();
            jvalue _value;
            _value.l = nullptr;
            if(m_valueInternalToExternalConverter(env, nullptr, &last, &_value, true)){
                result = _value.l;
            }
        }
        return result;
    }

    jobject lastKey(JNIEnv * env, const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        jobject result = nullptr;
        {
            const K& lastKey = reinterpret_cast<const QMultiMap<K,T> *>(container)->lastKey();
            jvalue _value;
            _value.l = nullptr;
            if(m_keyInternalToExternalConverter(env, nullptr, &lastKey, &_value, true)){
                result = _value.l;
            }
        }
        return result;
    }

    jobject lowerBound(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override {
        QTJAMBI_KEY_VALUE_LOCKER
        jobject result = nullptr;
        {
            jvalue _key;
            _key.l = key;
            K _qkey;
            void *_qkeyPtr = &_qkey;
            if(m_keyExternalToInternalConverter(env, nullptr, _key, _qkeyPtr, jValueType::l)){
                AbstractBiIteratorAccess* access = new BiIteratorAccess<QMultiMap, align1, size1, align2, size2>(
                            m_keyMetaTypeInfo,
                            m_keyInternalToExternalConverter,
                            m_valueMetaTypeInfo,
                            m_valueInternalToExternalConverter);
                result = qtjambi_from_QMapIterator(env,
                                              ownerId,
                                              new typename QMultiMap<K,T>::const_iterator(reinterpret_cast<const QMultiMap<K,T> *>(container)->lowerBound(_qkey)),
                                              [](void* ptr){ delete reinterpret_cast<typename QMultiMap<K,T>::const_iterator*>(ptr); },
                                              access);
            }
        }
        if(!result)
            result = end(env, ownerId, container);
        return result;
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        if (qtjambi_is_QMultiMap(env, other, keyMetaType(), valueMetaType())) {
            if(void* ptr = qtjambi_to_object(env, other)){
                QTJAMBI_KEY_VALUE_LOCKER
                bool equals = *reinterpret_cast<const QMultiMap<K,T> *>(container)==*reinterpret_cast<QMultiMap<K,T> *>(ptr);
                return equals;
            }
        }else{
            QTJAMBI_KEY_VALUE_LOCKER
            QMultiMap<K,T> map;
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
            return *reinterpret_cast<const QMultiMap<K,T> *>(container)==map;
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
                result = reinterpret_cast<QMultiMap<K,T> *>(container)->remove(_qkey);
            }
        }
        return result;
    }

    jint size(JNIEnv *, const void* container) override {
        jint result = 0;
        QTJAMBI_KEY_VALUE_LOCKER
        result = reinterpret_cast<const QMultiMap<K,T> *>(container)->size();
        return result;
    }

    jobject take(JNIEnv *env, void* container, jobject key) override {
        QTJAMBI_KEY_VALUE_LOCKER
        jobject result = nullptr;
        jvalue _key;
        _key.l = key;
        K _qkey;
        void *_qkeyPtr = &_qkey;
        if(m_keyExternalToInternalConverter(env, nullptr, _key, _qkeyPtr, jValueType::l)){
            T _qvalue = reinterpret_cast<QMultiMap<K,T> *>(container)->take(_qkey);
            jvalue _value;
            _value.l = nullptr;
            if(m_valueInternalToExternalConverter(env, nullptr, &_qvalue, &_value, true)){
                result = _value.l;
            }
        }
        return result;
    }

    jobject upperBound(JNIEnv * env, QtJambiNativeID ownerId, const void* container, jobject key) override {
        QTJAMBI_KEY_VALUE_LOCKER
        jobject result = nullptr;
        {
            jvalue _key;
            _key.l = key;
            K _qkey;
            void *_qkeyPtr = &_qkey;
            if(m_keyExternalToInternalConverter(env, nullptr, _key, _qkeyPtr, jValueType::l)){
                AbstractBiIteratorAccess* access = new BiIteratorAccess<QMultiMap, align1, size1, align2, size2>(
                            m_keyMetaTypeInfo,
                            m_keyInternalToExternalConverter,
                            m_valueMetaTypeInfo,
                            m_valueInternalToExternalConverter);
                result = qtjambi_from_QMapIterator(env,
                                              ownerId,
                                              new typename QMultiMap<K,T>::const_iterator(reinterpret_cast<const QMultiMap<K,T> *>(container)->upperBound(_qkey)),
                                              [](void* ptr){ delete reinterpret_cast<typename QMultiMap<K,T>::const_iterator*>(ptr); },
                                              access);
            }
        }
        if(!result)
            result = end(env, ownerId, container);
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
                    T _qvalue = reinterpret_cast<const QMultiMap<K,T> *>(container)->value(_qkey, _qdefaultValue);
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
            const void* values = new QList<T>(reinterpret_cast<const QMultiMap<K,T> *>(container)->values());
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
            const void* keys = new QList<K>(reinterpret_cast<const QMultiMap<K,T> *>(container)->uniqueKeys());
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
                const void* values = new QList<T>(reinterpret_cast<const QMultiMap<K,T> *>(container)->values(_qkey));
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
                    result = reinterpret_cast<const QMultiMap<K,T> *>(container)->contains(_qkey, _qvalue);
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
                    result = jint(reinterpret_cast<const QMultiMap<K,T> *>(container)->count(_qkey, _qvalue));
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
                    AbstractBiIteratorAccess* access = new BiIteratorAccess<QMultiMap, align1, size1, align2, size2>(
                                m_keyMetaTypeInfo,
                                m_keyInternalToExternalConverter,
                                m_valueMetaTypeInfo,
                                m_valueInternalToExternalConverter);
                    result = qtjambi_from_QMapIterator(env,
                                                  ownerId,
                                                  new typename QMultiMap<K,T>::const_iterator(reinterpret_cast<const QMultiMap<K,T> *>(container)->find(_qkey, _qvalue)),
                                                  [](void* ptr){ delete reinterpret_cast<typename QMultiMap<K,T>::const_iterator*>(ptr); },
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
                    result = jint(reinterpret_cast<QMultiMap<K,T> *>(container)->remove(_qkey, _qvalue));
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
                    reinterpret_cast<QMultiMap<K,T> *>(container)->replace(_qkey, _qvalue);
                }
            }
        }
    }

    void unite(JNIEnv *env, void* container, jobject other) override {
        if (qtjambi_is_QMultiMap(env, other, keyMetaType(), valueMetaType())) {
            if(void* ptr = qtjambi_to_object(env, other)){
                QTJAMBI_KEY_VALUE_LOCKER
                reinterpret_cast<QMultiMap<K,T> *>(container)->unite(*reinterpret_cast<QMultiMap<K,T> *>(ptr));
            }
        }
    }

    bool keyLessThan(JNIEnv *env, jobject key1, jobject key2) override {
        {
            QTJAMBI_KEY_VALUE_LOCKER
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
};

template<size_t align1, size_t size1, size_t align2, size_t size2>
struct BiContainerAccessFac<QMultiMap,align1,size1,align2,size2>{
    static constexpr BiContainerAccessFactory factory(){
        return BiContainerAccessFactory(GenericMultiMapAccess<align1,size1,align2,size2>::newInstance);
    }
};

}

#endif // QTJAMBI_CONTAINERACCESS_MULTIMAP_H
