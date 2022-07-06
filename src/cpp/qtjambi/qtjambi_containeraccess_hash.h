#ifndef QTJAMBI_CONTAINERACCESS_HASH_H
#define QTJAMBI_CONTAINERACCESS_HASH_H

#include <QtCore/QHash>
#include <qtjambi/qtjambi_containeraccess.h>

namespace QtJambiPrivate {

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
            if(!analyzer(pointer_from<decltype(iter.key())>::from(iter.key()),
                         pointer_from<decltype(iter.value())>::from(iter.value()), data)){
                break;
            }
        }
    }

    void* createContainer() override {
        QTJAMBI_KEY_VALUE_LOCKER
        QHash<K,T>* list = new QHash<K,T>();
        return list;
    }
    void* copyContainer(const void* container) override {
        if(container){
            QTJAMBI_KEY_VALUE_LOCKER
            QHash<K,T>* list = new QHash<K,T>(*reinterpret_cast<const QHash<K,T>*>(container));
            return list;
        }
        return createContainer();
    }
    void assign(void* container, const void* other) override {
        QTJAMBI_KEY_VALUE_LOCKER
        (*reinterpret_cast<QHash<K,T>*>(container)) = (*reinterpret_cast<const QHash<K,T>*>(other));
    }
    void deleteContainer(void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        delete reinterpret_cast<QHash<K,T>*>(container);
    }
    int registerContainer(const QByteArray& containerTypeName) override {
        return qtjambi_register_bicontainer_type<QHash<K,T>, size1, size2>(containerTypeName, m_keyMetaTypeInfo.metaType(), m_valueMetaTypeInfo.metaType());
    }
    bool isConstant() override {return false;}
    const QMetaType& keyMetaType() override {return m_keyMetaTypeInfo.metaType();}
    const QMetaType& valueMetaType() override {return m_valueMetaTypeInfo.metaType();}

    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        AbstractBiIteratorAccess* access = new BiIteratorAccess<QHash, align1, size1, align2, size2>(
                    m_keyMetaTypeInfo,
                    m_keyInternalToExternalConverter,
                    m_valueMetaTypeInfo,
                    m_valueInternalToExternalConverter);
        jobject result = qtjambi_from_QMapIterator(env,
                                      ownerId,
                                      new typename QHash<K,T>::const_iterator(reinterpret_cast<const QHash<K,T> *>(container)->begin()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QHash<K,T>::const_iterator*>(ptr); },
                                      access);
        return result;
    }

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

    jobject end(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        AbstractBiIteratorAccess* access = new BiIteratorAccess<QHash, align1, size1, align2, size2>(
                    m_keyMetaTypeInfo,
                    m_keyInternalToExternalConverter,
                    m_valueMetaTypeInfo,
                    m_valueInternalToExternalConverter);
        jobject result = qtjambi_from_QMapIterator(env,
                                      ownerId,
                                      new typename QHash<K,T>::const_iterator(reinterpret_cast<const QHash<K,T> *>(container)->end()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QHash<K,T>::const_iterator*>(ptr); },
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
                AbstractBiIteratorAccess* access = new BiIteratorAccess<QHash, align1, size1, align2, size2>(
                            m_keyMetaTypeInfo,
                            m_keyInternalToExternalConverter,
                            m_valueMetaTypeInfo,
                            m_valueInternalToExternalConverter);
                result = qtjambi_from_QMapIterator(env,
                                              ownerId,
                                              new typename QHash<K,T>::const_iterator(reinterpret_cast<const QHash<K,T> *>(container)->find(_qkey)),
                                              [](void* ptr,bool){ delete reinterpret_cast<typename QHash<K,T>::const_iterator*>(ptr); },
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
        AbstractContainerAccess* access = qtjambi_create_container_access(env, ContainerType::QList, m_keyMetaTypeInfo.metaType());
        if(!access)
            access = qtjambi_create_container_access(env, ContainerType::QList, m_keyMetaTypeInfo.metaType(),
                                                                                align1, size1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                                qtjambi_is_static_type(m_keyMetaTypeInfo.metaType()),
#endif
                                                                                qtjambi_is_pointer_type(m_keyMetaTypeInfo.metaType()),
                                                                                m_keyMetaTypeInfo.hashFunction(),
                                                                                m_keyInternalToExternalConverter,
                                                                                m_keyExternalToInternalConverter
                                                                            );
        if(access){
            const void* keys = new QList<K>(reinterpret_cast<const QHash<K,T> *>(container)->keys());
            result = qtjambi_from_QList(env, keys, access);
        }
        return result;
    }

    jobject keys(JNIEnv * env, const void* container, jobject value) override {
        QTJAMBI_KEY_VALUE_LOCKER
        jobject result = nullptr;
        AbstractContainerAccess* access = qtjambi_create_container_access(env, ContainerType::QList, m_keyMetaTypeInfo.metaType());
        if(!access)
            access = qtjambi_create_container_access(env, ContainerType::QList, m_keyMetaTypeInfo.metaType(),
                                                                                 align1, size1,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                                                 qtjambi_is_static_type(m_keyMetaTypeInfo.metaType()),
#endif
                                                                                 qtjambi_is_pointer_type(m_keyMetaTypeInfo.metaType()),
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
                result = qtjambi_from_QList(env, keys, access);
            }
        }
        return result;
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        if (qtjambi_is_QHash(env, other, keyMetaType(), valueMetaType())) {
            if(QHash<K,T>* ptr = qtjambi_to_object<QHash<K,T>>(env, other)){
                QTJAMBI_KEY_VALUE_LOCKER
                bool equals = *reinterpret_cast<const QHash<K,T> *>(container)==*ptr;
                return equals;
            }
        }else{
            QTJAMBI_KEY_VALUE_LOCKER
            QHash<K,T> map;
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
                    T _qvalue = reinterpret_cast<const QHash<K,T> *>(container)->value(_qkey, _qdefaultValue);
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
        AbstractContainerAccess* access = qtjambi_create_container_access(env, ContainerType::QList, m_valueMetaTypeInfo.metaType());
        if(!access)
            access = qtjambi_create_container_access(env, ContainerType::QList, m_valueMetaTypeInfo.metaType(),
                                                      align2, size2,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                      qtjambi_is_static_type(m_valueMetaTypeInfo.metaType()),
#endif
                                                      qtjambi_is_pointer_type(m_valueMetaTypeInfo.metaType()),
                                                      m_valueMetaTypeInfo.hashFunction(),
                                                      m_valueInternalToExternalConverter,
                                                      m_valueExternalToInternalConverter
                                                    );
        if(access){
            const void* values = new QList<T>(reinterpret_cast<const QHash<K,T> *>(container)->values());
            result = qtjambi_from_QList(env, values, access);
        }
        return result;
    }
};

template<size_t align1, size_t size1, size_t align2, size_t size2>
struct BiContainerAccessFac<QHash,align1,size1,align2,size2>{
    static constexpr BiContainerAccessFactory factory(){
        return BiContainerAccessFactory(GenericHashAccess<align1,size1,align2,size2>::newInstance);
    }
};

}

#endif // QTJAMBI_CONTAINERACCESS_HASH_H
