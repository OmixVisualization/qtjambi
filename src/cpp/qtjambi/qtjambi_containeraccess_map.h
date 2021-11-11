#ifndef QTJAMBI_CONTAINERACCESS_MAP_H
#define QTJAMBI_CONTAINERACCESS_MAP_H

#include <QtCore/QHash>
#include <qtjambi/qtjambi_containeraccess.h>

namespace QtJambiPrivate {

template<size_t align1, size_t size1, size_t align2, size_t size2>
class GenericMapAccess : public AbstractMapAccess{
    typedef typename std::conditional<size1==0, void*, ContainerElement<size1, 0, false, align1>>::type K;
    typedef typename std::conditional<size2==0, void*, ContainerElement<size2, 1, false, align2>>::type T;
    MetaTypeInfo<0,size1==0> m_keyMetaTypeInfo;
    InternalToExternalConverter m_keyInternalToExternalConverter;
    ExternalToInternalConverter m_keyExternalToInternalConverter;
    MetaTypeInfo<1,size2==0> m_valueMetaTypeInfo;
    InternalToExternalConverter m_valueInternalToExternalConverter;
    ExternalToInternalConverter m_valueExternalToInternalConverter;
    GenericMapAccess(
            const QMetaType& keyMetaType,
            const QHashFunction& keyHashFunction,
            const InternalToExternalConverter& keyInternalToExternalConverter,
            const ExternalToInternalConverter& keyExternalToInternalConverter,
            const QMetaType& valueMetaType,
            const QHashFunction& valueHashFunction,
            const InternalToExternalConverter& valueInternalToExternalConverter,
            const ExternalToInternalConverter& valueExternalToInternalConverter
            )
        :   AbstractMapAccess(),
          m_keyMetaTypeInfo(keyMetaType, keyHashFunction),
          m_keyInternalToExternalConverter(keyInternalToExternalConverter),
          m_keyExternalToInternalConverter(keyExternalToInternalConverter),
          m_valueMetaTypeInfo(valueMetaType, valueHashFunction),
          m_valueInternalToExternalConverter(valueInternalToExternalConverter),
          m_valueExternalToInternalConverter(valueExternalToInternalConverter)
    {
    }

    GenericMapAccess(const GenericMapAccess<align1, size1, align2, size2>& other)
        :   AbstractMapAccess(),
          m_keyMetaTypeInfo(other.m_keyMetaTypeInfo),
          m_keyInternalToExternalConverter(other.m_keyInternalToExternalConverter),
          m_keyExternalToInternalConverter(other.m_keyExternalToInternalConverter),
          m_valueMetaTypeInfo(other.m_valueMetaTypeInfo),
          m_valueInternalToExternalConverter(other.m_valueInternalToExternalConverter),
          m_valueExternalToInternalConverter(other.m_valueExternalToInternalConverter)
    {
    }

public:
    static AbstractMapAccess* newInstance(const QMetaType& keyMetaType,
                                          const QHashFunction& keyHashFunction,
                                          const InternalToExternalConverter& keyInternalToExternalConverter,
                                          const ExternalToInternalConverter& keyExternalToInternalConverter,
                                          const QMetaType& valueMetaType,
                                          const QHashFunction& valueHashFunction,
                                          const InternalToExternalConverter& valueInternalToExternalConverter,
                                          const ExternalToInternalConverter& valueExternalToInternalConverter){
        return new GenericMapAccess(keyMetaType,
                                    keyHashFunction,
                                    keyInternalToExternalConverter,
                                    keyExternalToInternalConverter,
                                    valueMetaType,
                                    valueHashFunction,
                                    valueInternalToExternalConverter,
                                    valueExternalToInternalConverter);
    }

    void dispose() override { delete this; }

    GenericMapAccess<align1, size1, align2, size2>* clone() override{
        return new GenericMapAccess<align1, size1, align2, size2>(*this);
    }

    void analyzeEntries(const void* container, EntryAnalyzer analyzer, void* data) override {
        for(auto iter = reinterpret_cast<const QMap<K,T>*>(container)->begin();
            iter != reinterpret_cast<const QMap<K,T>*>(container)->end();
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
        QMap<K,T>* list = new QMap<K,T>();
        return list;
    }
    void* copyContainer(const void* container) override {
        if(container){
            QTJAMBI_KEY_VALUE_LOCKER
            QMap<K,T>* list = new QMap<K,T>(*reinterpret_cast<const QMap<K,T>*>(container));
            return list;
        }
        return createContainer();
    }
    void assign(void* container, const void* other) override {
        QTJAMBI_KEY_VALUE_LOCKER
        (*reinterpret_cast<QMap<K,T>*>(container)) = (*reinterpret_cast<const QMap<K,T>*>(other));
    }
    void deleteContainer(void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        delete reinterpret_cast<QMap<K,T>*>(container);
    }
    int registerContainer(const QByteArray& containerTypeName) override {
        return qtjambi_register_bicontainer_type<QMap<K,T>, size1, size2>(containerTypeName, m_keyMetaTypeInfo.metaType(), m_valueMetaTypeInfo.metaType());
    }
    PtrDeleterFunction containerDeleter() override {
        return nullptr;
    }
    bool isConstant() override {return false;}
    const QMetaType& keyMetaType() override {return m_keyMetaTypeInfo.metaType();}
    const QMetaType& valueMetaType() override {return m_valueMetaTypeInfo.metaType();}

    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        AbstractBiIteratorAccess* access = new BiIteratorAccess<QMap, align1, size1, align2, size2>(
                    m_keyMetaTypeInfo,
                    m_keyInternalToExternalConverter,
                    m_valueMetaTypeInfo,
                    m_valueInternalToExternalConverter);
        jobject result = qtjambi_from_QMapIterator(env,
                                      ownerId,
                                      new typename QMap<K,T>::const_iterator(reinterpret_cast<const QMap<K,T> *>(container)->begin()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QMap<K,T>::const_iterator*>(ptr); },
                                      access);
        return result;
    }

    void clear(JNIEnv *, void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        reinterpret_cast<QMap<K,T> *>(container)->clear();
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
                result = reinterpret_cast<const QMap<K,T> *>(container)->contains(_qkey);
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
                result = jint(reinterpret_cast<const QMap<K,T> *>(container)->count(_qkey));
        }
        return result;
    }

    jobject end(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        AbstractBiIteratorAccess* access = new BiIteratorAccess<QMap, align1, size1, align2, size2>(
                    m_keyMetaTypeInfo,
                    m_keyInternalToExternalConverter,
                    m_valueMetaTypeInfo,
                    m_valueInternalToExternalConverter);
        jobject result = qtjambi_from_QMapIterator(env,
                                      ownerId,
                                      new typename QMap<K,T>::const_iterator(reinterpret_cast<const QMap<K,T> *>(container)->end()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QMap<K,T>::const_iterator*>(ptr); },
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
                AbstractBiIteratorAccess* access = new BiIteratorAccess<QMap, align1, size1, align2, size2>(
                            m_keyMetaTypeInfo,
                            m_keyInternalToExternalConverter,
                            m_valueMetaTypeInfo,
                            m_valueInternalToExternalConverter);
                result = qtjambi_from_QMapIterator(env,
                                              ownerId,
                                              new typename QMap<K,T>::const_iterator(reinterpret_cast<const QMap<K,T> *>(container)->find(_qkey)),
                                              [](void* ptr,bool){ delete reinterpret_cast<typename QMap<K,T>::const_iterator*>(ptr); },
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
            const T& first = reinterpret_cast<const QMap<K,T> *>(container)->first();
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
            const K& firstKey = reinterpret_cast<const QMap<K,T> *>(container)->firstKey();
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
                    reinterpret_cast<QMap<K,T> *>(container)->insert(_qkey, _qvalue);
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
                    K _qkey = reinterpret_cast<const QMap<K,T> *>(container)->key(_qvalue, _qdefaultkey);
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
            const void* keys = new QList<K>(reinterpret_cast<const QMap<K,T> *>(container)->keys());
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
                const void* keys = new QList<K>(reinterpret_cast<const QMap<K,T> *>(container)->keys(_qvalue));
                result = qtjambi_from_QList(env, keys, factory(m_keyMetaTypeInfo.metaType(),
                                                               m_keyMetaTypeInfo.hashFunction(),
                                                               m_keyInternalToExternalConverter,
                                                               m_keyExternalToInternalConverter));
            }
        }
        return result;
    }

    jobject last(JNIEnv * env, const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        jobject result = nullptr;
        {
            const T& last = reinterpret_cast<const QMap<K,T> *>(container)->last();
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
            const K& lastKey = reinterpret_cast<const QMap<K,T> *>(container)->lastKey();
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
                AbstractBiIteratorAccess* access = new BiIteratorAccess<QMap, align1, size1, align2, size2>(
                            m_keyMetaTypeInfo,
                            m_keyInternalToExternalConverter,
                            m_valueMetaTypeInfo,
                            m_valueInternalToExternalConverter);
                result = qtjambi_from_QMapIterator(env,
                                              ownerId,
                                              new typename QMap<K,T>::const_iterator(reinterpret_cast<const QMap<K,T> *>(container)->lowerBound(_qkey)),
                                              [](void* ptr,bool){ delete reinterpret_cast<typename QMap<K,T>::const_iterator*>(ptr); },
                                              access);
            }
        }
        if(!result)
            result = end(env, ownerId, container);
        return result;
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        if (qtjambi_is_QMap(env, other, keyMetaType(), valueMetaType())) {
            if(QMap<K,T>* ptr = qtjambi_to_object<QMap<K,T>>(env, other)){
                QTJAMBI_KEY_VALUE_LOCKER
                bool equals = *reinterpret_cast<const QMap<K,T> *>(container)==*ptr;
                return equals;
            }
        }else{
            QTJAMBI_KEY_VALUE_LOCKER
            QMap<K,T> map;
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
            return *reinterpret_cast<const QMap<K,T> *>(container)==map;
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
                result = reinterpret_cast<QMap<K,T> *>(container)->remove(_qkey);
            }
        }
        return result;
    }

    jint size(JNIEnv *, const void* container) override {
        jint result = 0;
        QTJAMBI_KEY_VALUE_LOCKER
        result = reinterpret_cast<const QMap<K,T> *>(container)->size();
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
            T _qvalue = reinterpret_cast<QMap<K,T> *>(container)->take(_qkey);
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
                AbstractBiIteratorAccess* access = new BiIteratorAccess<QMap, align1, size1, align2, size2>(
                            m_keyMetaTypeInfo,
                            m_keyInternalToExternalConverter,
                            m_valueMetaTypeInfo,
                            m_valueInternalToExternalConverter);
                result = qtjambi_from_QMapIterator(env,
                                              ownerId,
                                              new typename QMap<K,T>::const_iterator(reinterpret_cast<const QMap<K,T> *>(container)->upperBound(_qkey)),
                                              [](void* ptr,bool){ delete reinterpret_cast<typename QMap<K,T>::const_iterator*>(ptr); },
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
                    T _qvalue = reinterpret_cast<const QMap<K,T> *>(container)->value(_qkey, _qdefaultValue);
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
            QList<T>* values = new QList<T>(reinterpret_cast<const QMap<K,T> *>(container)->values());
            result = qtjambi_from_QList(env, values, factory(m_valueMetaTypeInfo.metaType(),
                                                             m_valueMetaTypeInfo.hashFunction(),
                                                             m_valueInternalToExternalConverter,
                                                             m_valueExternalToInternalConverter));
        }
        return result;
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
struct BiContainerAccessFac<QMap,align1,size1,align2,size2>{
    static constexpr BiContainerAccessFactory factory(){
        return BiContainerAccessFactory(GenericMapAccess<align1,size1,align2,size2>::newInstance);
    }
};

}

#endif // QTJAMBI_CONTAINERACCESS_MAP_H
