#ifndef QTJAMBI_CONTAINERACCESS_VECTOR_H
#define QTJAMBI_CONTAINERACCESS_VECTOR_H

#include <QtCore/QtGlobal>
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QtCore/QVector>
#include <QtCore/QStack>
#include <QtCore/QQueue>
#include <qtjambi/qtjambi_containeraccess.h>

namespace QtJambiPrivate {

template<size_t _align, size_t _size>
class GenericVectorAccess : public AbstractVectorAccess{
    typedef typename std::conditional<_size==0, void*, ContainerElement<_size, 0, false, _align>>::type T;
    MetaTypeInfo<0,_size==0> m_elementMetaTypeInfo;
    InternalToExternalConverter m_internalToExternalConverter;
    ExternalToInternalConverter m_externalToInternalConverter;
    GenericVectorAccess(
            const QMetaType& metaType,
            const QHashFunction& hashFunction,
            const InternalToExternalConverter& internalToExternalConverter,
            const ExternalToInternalConverter& externalToInternalConverter
            )
        :   AbstractVectorAccess(),
          m_elementMetaTypeInfo(metaType, hashFunction),
          m_internalToExternalConverter(internalToExternalConverter),
          m_externalToInternalConverter(externalToInternalConverter)
    {
    }

    GenericVectorAccess(const GenericVectorAccess<_align, _size>& other)
        :   AbstractVectorAccess(),
          m_elementMetaTypeInfo(other.m_elementMetaTypeInfo),
          m_internalToExternalConverter(other.m_internalToExternalConverter),
          m_externalToInternalConverter(other.m_externalToInternalConverter)
    {
    }

public:
    static AbstractVectorAccess* newInstance(const QMetaType& metaType,
                                           const QHashFunction& hashFunction,
                                           const InternalToExternalConverter& internalToExternalConverter,
                                           const ExternalToInternalConverter& externalToInternalConverter){
        return new GenericVectorAccess(metaType,
                                      hashFunction,
                                      internalToExternalConverter,
                                      externalToInternalConverter);
    }

    void dispose() override { delete this; }

    void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) override {
        for(auto iter = reinterpret_cast<const QVector<T>*>(container)->begin();
            iter != reinterpret_cast<const QVector<T>*>(container)->end();
            ++iter
            ){
            if(!analyzer(pointer_from<decltype(*iter)>::from(*iter), data)){
                break;
            }
        }
    }

    GenericVectorAccess<_align, _size>* clone() override{
        return new GenericVectorAccess<_align, _size>(*this);
    }
    void* createContainer() override {

        QTJAMBI_ELEMENT_LOCKER
        QVector<T>* list = new QVector<T>();

        return list;
    }
    void* copyContainer(const void* container) override {
        if(container){

            QTJAMBI_ELEMENT_LOCKER
            QVector<T>* list = new QVector<T>(*reinterpret_cast<const QVector<T>*>(container));

            return list;
        }
        return createContainer();
    }
    void assign(void* container, const void* other) override {

        QTJAMBI_ELEMENT_LOCKER
        (*reinterpret_cast<QVector<T>*>(container)) = (*reinterpret_cast<const QVector<T>*>(other));

    }
    void deleteContainer(void* container) override {

        QTJAMBI_ELEMENT_LOCKER
        delete reinterpret_cast<QVector<T>*>(container);

    }
    int registerContainer(const QByteArray& containerTypeName) override {
        return qtjambi_register_container_type<QVector<T>, _size>(containerTypeName, m_elementMetaTypeInfo.metaType());
    }
    bool isConstant() override {return false;}
    const QMetaType& elementMetaType() override {return m_elementMetaTypeInfo.metaType();}

    jobject end(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        AbstractIteratorAccess* access = new IteratorAccess<QVector, _align, _size, false>(
                    m_elementMetaTypeInfo, m_internalToExternalConverter);
        return qtjambi_from_QIterator(env,
                                      ownerId,
                                      new typename QVector<T>::const_iterator(reinterpret_cast<const QVector<T> *>(container)->end()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QVector<T>::const_iterator*>(ptr); },
                                      access);
    }

    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        AbstractIteratorAccess* access = new IteratorAccess<QVector, _align, _size, false>(
                    m_elementMetaTypeInfo, m_internalToExternalConverter);
        return qtjambi_from_QIterator(env,
                                      ownerId,
                                      new typename QVector<T>::const_iterator(reinterpret_cast<const QVector<T> *>(container)->begin()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QVector<T>::const_iterator*>(ptr); },
                                      access);
    }

    void append(JNIEnv * env, void* container, jobject value) override {

        QTJAMBI_ELEMENT_LOCKER
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            jvalue _value;
            _value.l = value;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l)){
                reinterpret_cast<QVector<T> *>(container)->append(_qvalue);
            }
        }

    }

    void appendVector(JNIEnv * env, void* container, jobject list) override {
        if (qtjambi_is_QVector(env, list, elementMetaType())) {
            if(QVector<T>* ptr = qtjambi_to_object<QVector<T>>(env, list)){
                QTJAMBI_ELEMENT_LOCKER
                reinterpret_cast<QVector<T> *>(container)->append(*ptr);
            }
        }else{
            jobject iter = qtjambi_collection_iterator(env, list);
            while(qtjambi_iterator_has_next(env, iter)){
                append(env, container, qtjambi_iterator_next(env, iter));
            }
        }
    }

    jobject at(JNIEnv * env, const void* container, jint index) override {

        QTJAMBI_ELEMENT_LOCKER
        const T& result = reinterpret_cast<const QVector<T> *>(container)->at(index);

        jvalue _value;
        _value.l = nullptr;
        if(m_internalToExternalConverter(env, nullptr, &result, &_value, true)){
            return _value.l;
        }
        return nullptr;
    }

    jobject value(JNIEnv * env, const void* container, jint index) override {
        jvalue _result;
        _result.l = nullptr;
        bool success = false;

        QTJAMBI_ELEMENT_LOCKER
        {
            T qresult = reinterpret_cast<const QVector<T> *>(container)->value(index);
            success = m_internalToExternalConverter(env, nullptr, &qresult, &_result, true);
        }

        if(success){
            return _result.l;
        }
        return nullptr;
    }

    jobject value(JNIEnv * env, const void* container, jint index, jobject defaultValue) override {
        jvalue _result;
        _result.l = nullptr;
        bool success = false;

        QTJAMBI_ELEMENT_LOCKER
        {
            T _qdefaultValue;
            void *_qdefaultValuePtr = &_qdefaultValue;
            jvalue _defaultValue;
            _defaultValue.l = defaultValue;
            if(m_externalToInternalConverter(env, nullptr, _defaultValue, _qdefaultValuePtr, jValueType::l)){
                T qresult = reinterpret_cast<const QVector<T> *>(container)->value(index, _qdefaultValue);
                success = m_internalToExternalConverter(env, nullptr, &qresult, &_result, true);
            }
        }

        if(success){
            return _result.l;
        }
        return nullptr;
    }

    jobject toSet(JNIEnv * env,const void* container) override {

        QTJAMBI_ELEMENT_LOCKER
        jobject result = nullptr;
        AbstractContainerAccess* access = qtjambi_create_container_access(env, ContainerType::QSet, m_elementMetaTypeInfo.metaType());
        if(!access)
            access = qtjambi_create_container_access(env, ContainerType::QSet,
                                                      m_elementMetaTypeInfo.metaType(),
                                                      _align, _size,
                                                      qtjambi_is_static_type(m_elementMetaTypeInfo.metaType()),
                                                      qtjambi_is_pointer_type(m_elementMetaTypeInfo.metaType()),
                                                      m_elementMetaTypeInfo.hashFunction(),
                                                      m_internalToExternalConverter,
                                                      m_externalToInternalConverter
                                                    );
        if(access){
            const void* set = new QSet<T>(reinterpret_cast<const QVector<T> *>(container)->begin(), reinterpret_cast<const QVector<T> *>(container)->end());
            result = qtjambi_from_QSet(env, set, access);
        }

        return result;
    }

    void swapItemsAt(JNIEnv *, void* container, jint index1, jint index2) override {

        QTJAMBI_ELEMENT_LOCKER
        reinterpret_cast<QVector<T> *>(container)->swapItemsAt(index1, index2);

    }

    jboolean startsWith(JNIEnv * env, const void* container, jobject value) override {
        jvalue _value;
        _value.l = value;
        bool result = false;

        QTJAMBI_ELEMENT_LOCKER
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                result = reinterpret_cast<const QVector<T> *>(container)->startsWith(_qvalue);
        }

        return result;
    }

    jint size(JNIEnv *, const void* container) override {

        QTJAMBI_ELEMENT_LOCKER
        jint result = reinterpret_cast<const QVector<T> *>(container)->size();

        return result;
    }

    void reserve(JNIEnv *, void* container, jint size) override {

        QTJAMBI_ELEMENT_LOCKER
        reinterpret_cast<QVector<T> *>(container)->reserve(size);

    }

    void replace(JNIEnv * env, void* container, jint index, jobject value) override {
        jvalue _value;
        _value.l = value;

        QTJAMBI_ELEMENT_LOCKER
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                reinterpret_cast<QVector<T> *>(container)->replace(index, _qvalue);
        }

    }

    jboolean removeOne(JNIEnv * env, void* container, jobject value) override {
        jvalue _value;
        _value.l = value;
        bool success = false;

        QTJAMBI_ELEMENT_LOCKER
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                success = reinterpret_cast<QVector<T> *>(container)->removeOne(_qvalue);
        }

        return success;
    }

    void removeAt(JNIEnv *, void* container, jint index) override {

        QTJAMBI_ELEMENT_LOCKER
        reinterpret_cast<QVector<T> *>(container)->removeAt(index);

    }

    jint removeAll(JNIEnv * env, void* container, jobject value) override {
        jvalue _value;
        _value.l = value;
        jint count = 0;

        QTJAMBI_ELEMENT_LOCKER
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                count = reinterpret_cast<QVector<T> *>(container)->removeAll(_qvalue);
        }

        return count;
    }

    void prepend(JNIEnv * env, void* container, jobject value) override {
        jvalue _value;
        _value.l = value;

        QTJAMBI_ELEMENT_LOCKER
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                reinterpret_cast<QVector<T> *>(container)->prepend(_qvalue);
        }

    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        if (qtjambi_is_QVector(env, other, elementMetaType())) {
            if(QVector<T>* ptr = qtjambi_to_object<QVector<T>>(env, other)){
                QTJAMBI_ELEMENT_LOCKER
                return *reinterpret_cast<const QVector<T> *>(container)==*ptr;
            }
        }else{
            QTJAMBI_ELEMENT_LOCKER
            QVector<T> list;
            jobject iterator = qtjambi_collection_iterator(env, other);
            while(qtjambi_iterator_has_next(env, iterator)){
                jvalue _value;
                _value.l = qtjambi_iterator_next(env, iterator);
                T _qvalue;
                void *_qvaluePtr = &_qvalue;
                if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                    list << _qvalue;
            }
            return *reinterpret_cast<const QVector<T> *>(container)==list;
        }
        return false;
    }

    void move(JNIEnv *, void* container, jint index1, jint index2) override {

        QTJAMBI_ELEMENT_LOCKER
        reinterpret_cast<QVector<T> *>(container)->move(index1, index2);

    }

    jobject mid(JNIEnv * env, const void* container, jint index1, jint index2) override {

        QTJAMBI_ELEMENT_LOCKER
        jobject result = nullptr;
        {
            const void* mid = new QVector<T>(reinterpret_cast<const QVector<T> *>(container)->mid(index1, index2));
            result = qtjambi_from_QVector(env, mid, clone());
        }

        return result;
    }

    jint lastIndexOf(JNIEnv * env, const void* container, jobject value, jint index) override {
        int idx = -1;

        QTJAMBI_ELEMENT_LOCKER
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            jvalue _value;
            _value.l = value;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                idx = reinterpret_cast<const QVector<T> *>(container)->lastIndexOf(_qvalue, index);
        }

        return idx;
    }

    void insert(JNIEnv * env, void* container, jint index, jobject value) override {

        QTJAMBI_ELEMENT_LOCKER
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            jvalue _value;
            _value.l = value;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                reinterpret_cast<QVector<T> *>(container)->insert(index, _qvalue);
        }

    }

    jint indexOf(JNIEnv * env, const void* container, jobject value, jint index) override {
        int idx = -1;

        QTJAMBI_ELEMENT_LOCKER
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            jvalue _value;
            _value.l = value;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                idx = reinterpret_cast<const QVector<T> *>(container)->indexOf(_qvalue, index);
        }

        return idx;
    }

    jboolean endsWith(JNIEnv * env, const void* container, jobject value) override {
        jvalue _value;
        _value.l = value;
        bool result = false;

        QTJAMBI_ELEMENT_LOCKER
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                result = reinterpret_cast<const QVector<T> *>(container)->endsWith(_qvalue);
        }

        return result;
    }

    jint count(JNIEnv * env, const void* container, jobject value) override {
        jvalue _value;
        _value.l = value;
        int result = 0;

        QTJAMBI_ELEMENT_LOCKER
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                result = reinterpret_cast<const QVector<T> *>(container)->count(_qvalue);
        }

        return result;
    }

    jboolean contains(JNIEnv * env, const void* container, jobject value) override {
        jvalue _value;
        _value.l = value;
        bool result = false;

        QTJAMBI_ELEMENT_LOCKER
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                result = reinterpret_cast<const QVector<T> *>(container)->contains(_qvalue);
        }

        return result;
    }

    void clear(JNIEnv *, void* container) override {

        QTJAMBI_ELEMENT_LOCKER
        reinterpret_cast<QVector<T> *>(container)->clear();

    }

    jint capacity(JNIEnv *, const void* container) override {

        QTJAMBI_ELEMENT_LOCKER
        return reinterpret_cast<const QVector<T> *>(container)->capacity();

    }

    void fill(JNIEnv * env, void* container, jobject value, jint size) override {
        jvalue _value;
        _value.l = value;

        QTJAMBI_ELEMENT_LOCKER
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                reinterpret_cast<QVector<T> *>(container)->fill(_qvalue, size);
        }

    }

    void remove(JNIEnv *, void* container, jint index, jint n) override {

        QTJAMBI_ELEMENT_LOCKER
        reinterpret_cast<QVector<T> *>(container)->remove(index, n);

    }

    void insert(JNIEnv * env, void* container, jint index, jint n, jobject value) override {
        jvalue _value;
        _value.l = value;

        QTJAMBI_ELEMENT_LOCKER
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                reinterpret_cast<QVector<T> *>(container)->insert(index, n, _qvalue);
        }

    }

    void resize(JNIEnv *, void* container, jint newSize) override {

        QTJAMBI_ELEMENT_LOCKER
        reinterpret_cast<QVector<T> *>(container)->resize(newSize);

    }

    void squeeze(JNIEnv *, void* container) override {

        QTJAMBI_ELEMENT_LOCKER
        reinterpret_cast<QVector<T> *>(container)->squeeze();

    }
};

template<size_t align, size_t size, bool isStatic>
struct ContainerAccessFac<QVector,align,size,isStatic>{
    static constexpr ContainerAccessFactory factory(){
        return ContainerAccessFactory(GenericVectorAccess<align,size>::newInstance);
    }
};

template<size_t align, size_t size, bool isStatic>
struct ContainerAccessFac<QStack,align,size,isStatic> : ContainerAccessFac<QVector,align,size,isStatic>{
};

}

#endif
#endif // QTJAMBI_CONTAINERACCESS_VECTOR_H
