#ifndef QTJAMBI_CONTAINERACCESS_SET_H
#define QTJAMBI_CONTAINERACCESS_SET_H

#include <QtCore/QSet>
#include <qtjambi/qtjambi_containeraccess.h>

namespace QtJambiPrivate {

template<size_t _align, size_t _size>
class GenericSetAccess : public AbstractSetAccess{
    typedef typename std::conditional<_size==0, void*, ContainerElement<_size, 0, false, _align>>::type T;
    MetaTypeInfo<0,_size==0> m_elementMetaTypeInfo;
    InternalToExternalConverter m_internalToExternalConverter;
    ExternalToInternalConverter m_externalToInternalConverter;
    GenericSetAccess(
            const QMetaType& metaType,
            const QHashFunction& hashFunction,
            const InternalToExternalConverter& internalToExternalConverter,
            const ExternalToInternalConverter& externalToInternalConverter
            )
        :   AbstractSetAccess(),
          m_elementMetaTypeInfo(metaType, hashFunction),
          m_internalToExternalConverter(internalToExternalConverter),
          m_externalToInternalConverter(externalToInternalConverter)
    {
    }

    GenericSetAccess(const GenericSetAccess<_align, _size>& other)
        :   AbstractSetAccess(),
          m_elementMetaTypeInfo(other.m_elementMetaTypeInfo),
          m_internalToExternalConverter(other.m_internalToExternalConverter),
          m_externalToInternalConverter(other.m_externalToInternalConverter)
    {
    }

public:
    static AbstractSetAccess* newInstance(const QMetaType& metaType,
                                           const QHashFunction& hashFunction,
                                           const InternalToExternalConverter& internalToExternalConverter,
                                           const ExternalToInternalConverter& externalToInternalConverter){
        return new GenericSetAccess(metaType,
                                      hashFunction,
                                      internalToExternalConverter,
                                      externalToInternalConverter);
    }

    void dispose() override { delete this; }

    void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) override {
        for(auto iter = reinterpret_cast<const QSet<T>*>(container)->begin();
            iter != reinterpret_cast<const QSet<T>*>(container)->end();
            ++iter
            ){
            if(!analyzer(pointer_from<decltype(*iter)>::from(*iter), data)){
                break;
            }
        }
    }

    GenericSetAccess<_align, _size>* clone() override{
        return new GenericSetAccess<_align, _size>(*this);
    }
    void* createContainer() override {
        QTJAMBI_ELEMENT_LOCKER
        QSet<T>* list = new QSet<T>();
        return list;
    }

    void* copyContainer(const void* container) override {
        if(container){
            QTJAMBI_ELEMENT_LOCKER
            QSet<T>* list = new QSet<T>(*reinterpret_cast<const QSet<T>*>(container));
            return list;
        }
        return createContainer();
    }

    void assign(void* container, const void* other) override {
        QTJAMBI_ELEMENT_LOCKER
        (*reinterpret_cast<QSet<T>*>(container)) = (*reinterpret_cast<const QSet<T>*>(other));
    }

    void deleteContainer(void* container) override {
        QTJAMBI_ELEMENT_LOCKER
        delete reinterpret_cast<QSet<T>*>(container);
    }

    int registerContainer(const QByteArray& containerTypeName) override {
        return qtjambi_register_container_type<QSet<T>, _size>(containerTypeName, m_elementMetaTypeInfo.metaType());
    }

    bool isConstant() override {return false;}
    const QMetaType& elementMetaType() override {return m_elementMetaTypeInfo.metaType();}

    jobject end(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        AbstractIteratorAccess* access = new IteratorAccess<QSet, _align, _size, false>(
                    m_elementMetaTypeInfo, m_internalToExternalConverter);
        return qtjambi_from_QIterator(env,
                                      ownerId,
                                      new typename QSet<T>::const_iterator(reinterpret_cast<const QSet<T> *>(container)->end()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QSet<T>::const_iterator*>(ptr); },
                                      access);
    }

    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        AbstractIteratorAccess* access = new IteratorAccess<QSet, _align, _size, false>(
                    m_elementMetaTypeInfo, m_internalToExternalConverter);
        return qtjambi_from_QIterator(env,
                                      ownerId,
                                      new typename QSet<T>::const_iterator(reinterpret_cast<const QSet<T> *>(container)->begin()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QSet<T>::const_iterator*>(ptr); },
                                      access);
    }

    void insert(JNIEnv * env, void* container, jobject value) override {
        QTJAMBI_ELEMENT_LOCKER
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            jvalue _value;
            _value.l = value;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l)){
                reinterpret_cast<QSet<T> *>(container)->insert(_qvalue);
            }
        }
    }

    void intersect(JNIEnv * env, void* container, jobject other) override {
        if (qtjambi_is_QSet(env, other, elementMetaType())) {
            if(QSet<T>* ptr = qtjambi_to_object<QSet<T>>(env, other)){
                QTJAMBI_ELEMENT_LOCKER
                reinterpret_cast<QSet<T> *>(container)->intersect(*ptr);
            }
        }else{
            QTJAMBI_ELEMENT_LOCKER
            QSet<T> list;
            jobject iterator = qtjambi_collection_iterator(env, other);
            while(qtjambi_iterator_has_next(env, iterator)){
                jvalue _value;
                _value.l = qtjambi_iterator_next(env, iterator);
                T _qvalue;
                void *_qvaluePtr = &_qvalue;
                if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                    list << _qvalue;
            }
            reinterpret_cast<QSet<T> *>(container)->intersect(list);
        }
    }

    void unite(JNIEnv * env, void* container, jobject other) override {
        if (qtjambi_is_QSet(env, other, elementMetaType())) {
            if(QSet<T>* ptr = qtjambi_to_object<QSet<T>>(env, other)){
                QTJAMBI_ELEMENT_LOCKER
                reinterpret_cast<QSet<T> *>(container)->unite(*ptr);
            }
        }else{
            QTJAMBI_ELEMENT_LOCKER
            QSet<T> list;
            jobject iterator = qtjambi_collection_iterator(env, other);
            while(qtjambi_iterator_has_next(env, iterator)){
                jvalue _value;
                _value.l = qtjambi_iterator_next(env, iterator);
                T _qvalue;
                void *_qvaluePtr = &_qvalue;
                if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                    list << _qvalue;
            }
            reinterpret_cast<QSet<T> *>(container)->unite(list);
        }
    }

    void subtract(JNIEnv * env, void* container, jobject other) override {
        if (qtjambi_is_QSet(env, other, elementMetaType())) {
            if(QSet<T>* ptr = qtjambi_to_object<QSet<T>>(env, other)){
                QTJAMBI_ELEMENT_LOCKER
                reinterpret_cast<QSet<T> *>(container)->subtract(*ptr);
            }
        }else{
            QTJAMBI_ELEMENT_LOCKER
            QSet<T> list;
            jobject iterator = qtjambi_collection_iterator(env, other);
            while(qtjambi_iterator_has_next(env, iterator)){
                jvalue _value;
                _value.l = qtjambi_iterator_next(env, iterator);
                T _qvalue;
                void *_qvaluePtr = &_qvalue;
                if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                    list << _qvalue;
            }
            reinterpret_cast<QSet<T> *>(container)->subtract(list);
        }
    }

    jboolean intersects(JNIEnv * env, const void* container, jobject other) override {
        if (qtjambi_is_QSet(env, other, elementMetaType())) {
            if(QSet<T>* ptr = qtjambi_to_object<QSet<T>>(env, other)){
                QTJAMBI_ELEMENT_LOCKER
                bool intersects = reinterpret_cast<const QSet<T> *>(container)->intersects(*ptr);
                return intersects;
            }
        }else{
            QTJAMBI_ELEMENT_LOCKER
            QSet<T> list;
            jobject iterator = qtjambi_collection_iterator(env, other);
            while(qtjambi_iterator_has_next(env, iterator)){
                jvalue _value;
                _value.l = qtjambi_iterator_next(env, iterator);
                T _qvalue;
                void *_qvaluePtr = &_qvalue;
                if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                    list << _qvalue;
            }
            return reinterpret_cast<const QSet<T> *>(container)->intersects(list);
        }
        return false;
    }

    jobject values(JNIEnv * env, const void* container) override {
        QTJAMBI_ELEMENT_LOCKER
        jobject result = nullptr;
        AbstractContainerAccess* access = qtjambi_create_container_access(env, ContainerType::QList, m_elementMetaTypeInfo.metaType());
        if(!access)
            access = qtjambi_create_container_access(env, ContainerType::QList,
                                                      m_elementMetaTypeInfo.metaType(),
                                                      _align, _size,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                      qtjambi_is_static_type(m_elementMetaTypeInfo.metaType()),
#endif
                                                      qtjambi_is_pointer_type(m_elementMetaTypeInfo.metaType()),
                                                      m_elementMetaTypeInfo.hashFunction(),
                                                      m_internalToExternalConverter,
                                                      m_externalToInternalConverter
                                                    );
        if(access){
            const void* qresult = new QList<T>(reinterpret_cast<const QSet<T> *>(container)->values());
            result = qtjambi_from_QList(env, qresult, access);
        }
        return result;
    }

    jint size(JNIEnv *, const void* container) override {
        QTJAMBI_ELEMENT_LOCKER
        jint result = reinterpret_cast<const QSet<T> *>(container)->size();
        return result;
    }

    void reserve(JNIEnv *, void* container, jint size) override {
        QTJAMBI_ELEMENT_LOCKER
        reinterpret_cast<QSet<T> *>(container)->reserve(size);
    }

    jboolean remove(JNIEnv * env, void* container, jobject value) override {
        jvalue _value;
        _value.l = value;
        bool success = false;
        QTJAMBI_ELEMENT_LOCKER
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                success = reinterpret_cast<QSet<T> *>(container)->remove(_qvalue);
        }
        return success;
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        if (qtjambi_is_QSet(env, other, elementMetaType())) {
            if(QSet<T>* ptr = qtjambi_to_object<QSet<T>>(env, other)){
                QTJAMBI_ELEMENT_LOCKER
                bool equals = *reinterpret_cast<const QSet<T> *>(container)==*ptr;
                return equals;
            }
        }else{
            QTJAMBI_ELEMENT_LOCKER
            QSet<T> list;
            jobject iterator = qtjambi_collection_iterator(env, other);
            while(qtjambi_iterator_has_next(env, iterator)){
                jvalue _value;
                _value.l = qtjambi_iterator_next(env, iterator);
                T _qvalue;
                void *_qvaluePtr = &_qvalue;
                if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                    list << _qvalue;
            }
            return *reinterpret_cast<const QSet<T> *>(container)==list;
        }
        return false;
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
                result = reinterpret_cast<const QSet<T> *>(container)->contains(_qvalue);
        }
        return result;
    }

    void clear(JNIEnv *, void* container) override {
        QTJAMBI_ELEMENT_LOCKER
        reinterpret_cast<QSet<T> *>(container)->clear();
    }

    jint capacity(JNIEnv *, const void* container) override {
        QTJAMBI_ELEMENT_LOCKER
        return reinterpret_cast<const QSet<T> *>(container)->capacity();
    }
};

template<size_t align, size_t size, bool isStatic>
struct ContainerAccessFac<QSet,align,size,isStatic>{
    static constexpr ContainerAccessFactory factory(){
        return ContainerAccessFactory(GenericSetAccess<align,size>::newInstance);
    }
};

}
#endif // QTJAMBI_CONTAINERACCESS_SET_H
