#ifndef QTJAMBI_CONTAINERACCESS_PAIR_H
#define QTJAMBI_CONTAINERACCESS_PAIR_H

#include <QtCore/QPair>
#include <qtjambi/qtjambi_containeraccess.h>

namespace QtJambiPrivate {

template<size_t align1, size_t size1, size_t align2, size_t size2>
class GenericPairAccess : public AbstractPairAccess{
    typedef typename std::conditional<size1==0, void*, ContainerElement<size1, 0, false, align1>>::type K;
    typedef typename std::conditional<size2==0, void*, ContainerElement<size2, 1, false, align2>>::type T;
    MetaTypeInfo<0,size1==0> m_keyMetaTypeInfo;
    InternalToExternalConverter m_keyInternalToExternalConverter;
    ExternalToInternalConverter m_keyExternalToInternalConverter;
    MetaTypeInfo<1,size2==0> m_valueMetaTypeInfo;
    InternalToExternalConverter m_valueInternalToExternalConverter;
    ExternalToInternalConverter m_valueExternalToInternalConverter;
    GenericPairAccess(
            const QMetaType& keyMetaType,
            const QHashFunction& keyHashFunction,
            const InternalToExternalConverter& keyInternalToExternalConverter,
            const ExternalToInternalConverter& keyExternalToInternalConverter,
            const QMetaType& valueMetaType,
            const QHashFunction& valueHashFunction,
            const InternalToExternalConverter& valueInternalToExternalConverter,
            const ExternalToInternalConverter& valueExternalToInternalConverter
            )
        :   AbstractPairAccess(),
          m_keyMetaTypeInfo(keyMetaType, keyHashFunction),
          m_keyInternalToExternalConverter(keyInternalToExternalConverter),
          m_keyExternalToInternalConverter(keyExternalToInternalConverter),
          m_valueMetaTypeInfo(valueMetaType, valueHashFunction),
          m_valueInternalToExternalConverter(valueInternalToExternalConverter),
          m_valueExternalToInternalConverter(valueExternalToInternalConverter)
    {
    }

    GenericPairAccess(const GenericPairAccess<align1, size1, align2, size2>& other)
        :   AbstractPairAccess(),
          m_keyMetaTypeInfo(other.m_keyMetaTypeInfo),
          m_keyInternalToExternalConverter(other.m_keyInternalToExternalConverter),
          m_keyExternalToInternalConverter(other.m_keyExternalToInternalConverter),
          m_valueMetaTypeInfo(other.m_valueMetaTypeInfo),
          m_valueInternalToExternalConverter(other.m_valueInternalToExternalConverter),
          m_valueExternalToInternalConverter(other.m_valueExternalToInternalConverter)
    {
    }

public:
    static AbstractPairAccess* newInstance(
                                          const QMetaType& keyMetaType,
                                          const QHashFunction& keyHashFunction,
                                          const InternalToExternalConverter& keyInternalToExternalConverter,
                                          const ExternalToInternalConverter& keyExternalToInternalConverter,
                                          const QMetaType& valueMetaType,
                                          const QHashFunction& valueHashFunction,
                                          const InternalToExternalConverter& valueInternalToExternalConverter,
                                          const ExternalToInternalConverter& valueExternalToInternalConverter){
        return new GenericPairAccess(
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

    GenericPairAccess<align1, size1, align2, size2>* clone() override{
        return new GenericPairAccess<align1, size1, align2, size2>(*this);
    }

    size_t sizeOf() override {
        return sizeof(QPair<K,T>);
    }
    void* constructContainer(void* placement, const void* copyOf = nullptr) override {
        QTJAMBI_KEY_VALUE_LOCKER
        if(copyOf){
            return new(placement) QPair<K,T>(*reinterpret_cast<const QPair<K,T>*>(copyOf));
        }else{
            return new(placement) QPair<K,T>();
        }
    }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* move) override {
        QTJAMBI_KEY_VALUE_LOCKER
        return new(placement) QPair<K,T>(std::move(*reinterpret_cast<const QPair<K,T>*>(move)));
    }
#endif
    bool destructContainer(void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        reinterpret_cast<QPair<K,T>*>(container)->~QPair<K,T>();
        return true;
    }
    void assign(void* container, const void* other) override {
        QTJAMBI_KEY_VALUE_LOCKER
        (*reinterpret_cast<QPair<K,T>*>(container)) = (*reinterpret_cast<const QPair<K,T>*>(other));
    }
    int registerContainer(const QByteArray& containerTypeName) override {
        return qtjambi_register_qpair_type<QPair<K,T>, size1, size2>(containerTypeName, m_keyMetaTypeInfo.metaType(), m_valueMetaTypeInfo.metaType());
    }
    bool isConstant() override {return false;}

    jobject first(JNIEnv * env, const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        jobject result = nullptr;
        {
            jvalue _first;
            _first.l = nullptr;
            if(m_keyInternalToExternalConverter(env, nullptr, &reinterpret_cast<const QPair<K,T> *>(container)->first, &_first, true)){
                result = _first.l;
            }
        }
        return result;
    }

    void setFirst(JNIEnv *env, void* container, jobject first) override {
        QTJAMBI_KEY_VALUE_LOCKER
        {
            jvalue jv;
            jv.l = first;
            void *_qkeyPtr = &reinterpret_cast<QPair<K,T> *>(container)->first;
            m_keyExternalToInternalConverter(env, nullptr, jv, _qkeyPtr, jValueType::l);
        }
    }

    jobject second(JNIEnv * env, const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
        jobject result = nullptr;
        {
            jvalue _value;
            _value.l = nullptr;
            if(m_valueInternalToExternalConverter(env, nullptr, &reinterpret_cast<const QPair<K,T> *>(container)->second, &_value, true)){
                result = _value.l;
            }
        }
        return result;
    }

    void setSecond(JNIEnv *env, void* container, jobject second) override {
        QTJAMBI_KEY_VALUE_LOCKER
        {
            jvalue jv;
            jv.l = second;
            void *_qsecondPtr = &reinterpret_cast<QPair<K,T> *>(container)->second;
            m_valueExternalToInternalConverter(env, nullptr, jv, _qsecondPtr, jValueType::l);
        }
    }
};

template<size_t align1, size_t size1, size_t align2, size_t size2>
struct BiContainerAccessFac<QPair,align1,size1,align2,size2>{
    static constexpr BiContainerAccessFactory factory(){
        return BiContainerAccessFactory(GenericPairAccess<align1,size1,align2,size2>::newInstance);
    }
};

}

#endif // QTJAMBI_CONTAINERACCESS_PAIR_H
