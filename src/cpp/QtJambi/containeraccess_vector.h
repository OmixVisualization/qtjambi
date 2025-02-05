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

#ifndef CONTAINERACCESS_VECTOR_H
#define CONTAINERACCESS_VECTOR_H

#include <QtCore/QtGlobal>
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QtCore/QVector>
#include <QtCore/QStack>
#include <QtCore/QQueue>
#include "containeraccess.h"

#if defined(QTJAMBI_GENERIC_ACCESS)

namespace ContainerAccessAPI {

template<size_t _align, size_t _size>
class GenericVectorAccess : public AbstractVectorAccess, public AbstractNestedSequentialAccess {
    typedef typename std::conditional<_size==0, void*, ContainerElement<_size, 0, false, _align>>::type T;
    MetaTypeInfo<0,_size==0> m_elementMetaTypeInfo;
    QtJambiUtils::InternalToExternalConverter m_internalToExternalConverter;
    QtJambiUtils::ExternalToInternalConverter m_externalToInternalConverter;
    QSharedPointer<AbstractContainerAccess> m_elementNestedContainerAccess;
    AbstractContainerAccess::DataType m_elementDataType;
    PtrOwnerFunction m_elementOwnerFunction;
    GenericVectorAccess(
            const QMetaType& metaType,
            const QtJambiUtils::QHashFunction& hashFunction,
            const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
            const QtJambiUtils::ExternalToInternalConverter& externalToInternalConverter,
            const QSharedPointer<AbstractContainerAccess>& elementNestedContainerAccess,
            PtrOwnerFunction elementOwnerFunction
            )
        :   AbstractVectorAccess(), AbstractNestedSequentialAccess(),
          m_elementMetaTypeInfo(metaType, hashFunction),
          m_internalToExternalConverter(internalToExternalConverter),
          m_externalToInternalConverter(externalToInternalConverter),
          m_elementNestedContainerAccess(elementNestedContainerAccess),
          m_elementDataType(dataType(m_elementMetaTypeInfo.metaType(), m_elementNestedContainerAccess)),
          m_elementOwnerFunction(elementOwnerFunction)
    {
    }

    GenericVectorAccess(const GenericVectorAccess<_align, _size>& other)
        :   AbstractVectorAccess(), AbstractNestedSequentialAccess(),
          m_elementMetaTypeInfo(other.m_elementMetaTypeInfo),
          m_internalToExternalConverter(other.m_internalToExternalConverter),
          m_externalToInternalConverter(other.m_externalToInternalConverter),
          m_elementNestedContainerAccess(other.m_elementNestedContainerAccess),
          m_elementDataType(other.m_elementDataType),
          m_elementOwnerFunction(other.m_elementOwnerFunction)
    {
    }

public:
    static AbstractVectorAccess* newInstance(const QMetaType& metaType,
                                             const QtJambiUtils::QHashFunction& hashFunction,
                                             const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
                                             const QtJambiUtils::ExternalToInternalConverter& externalToInternalConverter,
                                             const QSharedPointer<AbstractContainerAccess>& elementNestedContainerAccess,
                                             PtrOwnerFunction elementOwnerFunction){
        return new GenericVectorAccess(metaType,
                                       hashFunction,
                                       internalToExternalConverter,
                                       externalToInternalConverter,
                                       elementNestedContainerAccess,
                                       elementOwnerFunction);
    }

    void dispose() override { delete this; }

    GenericVectorAccess<_align, _size>* clone() override{
        return new GenericVectorAccess<_align, _size>(*this);
    }

    size_t sizeOf() override {
        return sizeof(QVector<T>);
    }
    void assign(JNIEnv *, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        (*reinterpret_cast<QVector<T>*>(container.container)) = (*reinterpret_cast<const QVector<T>*>(other.container));
    }
    void assign(void* container, const void* other) override {
        QTJAMBI_ELEMENT_LOCKER(this);
            (*reinterpret_cast<QVector<T>*>(container)) = (*reinterpret_cast<const QVector<T>*>(other));
    }
    void* constructContainer(void* placement) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        return new(placement) QVector<T>();
    }
    void* constructContainer(void* placement, const void* copyOf) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        return new(placement) QVector<T>(*reinterpret_cast<const QVector<T>*>(copyOf));
    }
    void* constructContainer(JNIEnv *, void* placement, const ConstContainerAndAccessInfo& copyOf) override {
        return constructContainer(placement, copyOf.container);
    }
    bool destructContainer(void* container) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        reinterpret_cast<QVector<T>*>(container)->~QVector<T>();
        return true;
    }
    int registerContainer(const QByteArray& containerTypeName) override {
        return QtJambiPrivate::registerSequentialContainerType<QVector<T>, _size>(containerTypeName, m_elementMetaTypeInfo.metaType(), this);
    }
    const QMetaType& elementMetaType() override {return m_elementMetaTypeInfo.metaType();}

    DataType elementType() override {
        return dataType(m_elementMetaTypeInfo.metaType(), m_elementNestedContainerAccess);
    }

    AbstractContainerAccess* elementNestedContainerAccess() override {
        return m_elementNestedContainerAccess ? m_elementNestedContainerAccess->clone() : nullptr;
    }
    const QSharedPointer<AbstractContainerAccess>& sharedElementNestedContainerAccess() override {
        return m_elementNestedContainerAccess;
    }
    bool hasNestedContainerAccess() override {
        return !m_elementNestedContainerAccess.isNull();
    }
    bool hasNestedPointers() override {
        if(hasNestedContainerAccess()){
            if(auto daccess = dynamic_cast<AbstractSequentialAccess*>(m_elementNestedContainerAccess.data())){
                return (daccess->elementType() & PointersMask) || daccess->hasNestedPointers();
            }else if(auto daccess = dynamic_cast<AbstractAssociativeAccess*>(m_elementNestedContainerAccess.data())){
                return (daccess->keyType() & PointersMask) || daccess->hasKeyNestedPointers() || (daccess->valueType() & PointersMask) || daccess->hasValueNestedPointers();
            }else if(auto daccess = dynamic_cast<AbstractPairAccess*>(m_elementNestedContainerAccess.data())){
                return (daccess->firstType() & PointersMask) || daccess->hasFirstNestedPointers() || (daccess->secondType() & PointersMask) || daccess->hasSecondNestedPointers();
            }
        }
        return false;
    }

    jobject end(JNIEnv * env, const ExtendedContainerInfo& container) override {
        SequentialConstIteratorAccess<QVector, _align, _size, false, false>* access = new SequentialIteratorAccess<QVector, _align, _size, false>(
                    m_elementMetaTypeInfo, m_internalToExternalConverter, m_externalToInternalConverter);
        return QtJambiAPI::convertQSequentialIteratorToJavaObject(env,
                                      container.nativeId,
                                      new typename QVector<T>::iterator(reinterpret_cast<QVector<T> *>(container.container)->end()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QVector<T>::iterator*>(ptr); },
                                      access);
    }

    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override {
        SequentialConstIteratorAccess<QVector, _align, _size, false, false>* access = new SequentialIteratorAccess<QVector, _align, _size, false>(
                    m_elementMetaTypeInfo, m_internalToExternalConverter, m_externalToInternalConverter);
        return QtJambiAPI::convertQSequentialIteratorToJavaObject(env,
                                      container.nativeId,
                                      new typename QVector<T>::iterator(reinterpret_cast<QVector<T> *>(container.container)->begin()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QVector<T>::iterator*>(ptr); },
                                      access);
    }

    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        AbstractSequentialConstIteratorAccess* access = new SequentialConstIteratorAccess<QVector, _align, _size, false>(
                    m_elementMetaTypeInfo, m_internalToExternalConverter);
        return QtJambiAPI::convertQSequentialIteratorToJavaObject(env,
                                      container.nativeId,
                                      new typename QVector<T>::const_iterator(reinterpret_cast<const QVector<T> *>(container.container)->constEnd()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QVector<T>::const_iterator*>(ptr); },
                                      access);
    }

    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        AbstractSequentialConstIteratorAccess* access = new SequentialConstIteratorAccess<QVector, _align, _size, false>(
                    m_elementMetaTypeInfo, m_internalToExternalConverter);
        return QtJambiAPI::convertQSequentialIteratorToJavaObject(env,
                                      container.nativeId,
                                      new typename QVector<T>::const_iterator(reinterpret_cast<const QVector<T> *>(container.container)->constBegin()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QVector<T>::const_iterator*>(ptr); },
                                      access);
    }

    void appendVector(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& containerInfo) override {
        if (ContainerAPI::getAsQVector(env, containerInfo.object, elementMetaType(), containerInfo.container, containerInfo.access)) {
            QTJAMBI_ELEMENT_LOCKER(this);
            reinterpret_cast<QVector<T> *>(container.container)->append(*reinterpret_cast<QVector<T> *>(containerInfo.container));
        }else{
            jobject iter = QtJambiAPI::iteratorOfJavaIterable(env, containerInfo.object);
            while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                insert(env, container, size(env, container.container), 1, QtJambiAPI::nextOfJavaIterator(env, iter));
            }
        }
    }

    jobject at(JNIEnv * env, const void* container, jint index) override {

        QTJAMBI_ELEMENT_LOCKER(this);
        const T& result = reinterpret_cast<const QVector<T> *>(container)->at(index);

        jvalue _value;
        _value.l = nullptr;
        if(m_internalToExternalConverter(env, nullptr, &result, _value, true)){
            return _value.l;
        }
        return nullptr;
    }

    jobject value(JNIEnv * env, const void* container, jint index) override {
        jvalue _result;
        _result.l = nullptr;
        bool success = false;

        QTJAMBI_ELEMENT_LOCKER(this);
        {
            T qresult = reinterpret_cast<const QVector<T> *>(container)->value(index);
            success = m_internalToExternalConverter(env, nullptr, &qresult, _result, true);
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

        QTJAMBI_ELEMENT_LOCKER(this);
        {
            T _qdefaultValue;
            void *_qdefaultValuePtr = &_qdefaultValue;
            jvalue _defaultValue;
            _defaultValue.l = defaultValue;
            if(m_externalToInternalConverter(env, nullptr, _defaultValue, _qdefaultValuePtr, jValueType::l)){
                T qresult = reinterpret_cast<const QVector<T> *>(container)->value(index, _qdefaultValue);
                success = m_internalToExternalConverter(env, nullptr, &qresult, _result, true);
            }
        }

        if(success){
            return _result.l;
        }
        return nullptr;
    }

    void swapItemsAt(JNIEnv *, const ContainerInfo& container, jint index1, jint index2) override {

        QTJAMBI_ELEMENT_LOCKER(this);
        reinterpret_cast<QVector<T> *>(container.container)->swapItemsAt(index1, index2);

    }

    jboolean startsWith(JNIEnv * env, const void* container, jobject value) override {
        jvalue _value;
        _value.l = value;
        bool result = false;

        QTJAMBI_ELEMENT_LOCKER(this);
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                result = reinterpret_cast<const QVector<T> *>(container)->startsWith(_qvalue);
        }

        return result;
    }

    jint size(JNIEnv *, const void* container) override {

        QTJAMBI_ELEMENT_LOCKER(this);
        jint result = reinterpret_cast<const QVector<T> *>(container)->size();

        return result;
    }

    void reserve(JNIEnv *, const ContainerInfo& container, jint size) override {

        QTJAMBI_ELEMENT_LOCKER(this);
        reinterpret_cast<QVector<T> *>(container.container)->reserve(size);

    }

    void replace(JNIEnv * env, const ContainerInfo& container, jint index, jobject value) override {
        jvalue _value;
        _value.l = value;

        QTJAMBI_ELEMENT_LOCKER(this);
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                reinterpret_cast<QVector<T> *>(container.container)->replace(index, _qvalue);
        }

    }

    jint removeAll(JNIEnv * env, const ContainerInfo& container, jobject value) override {
        jvalue _value;
        _value.l = value;
        jint count = 0;

        QTJAMBI_ELEMENT_LOCKER(this);
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                count = reinterpret_cast<QVector<T> *>(container.container)->removeAll(_qvalue);
        }

        return count;
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        void* ptr{nullptr};
        if (ContainerAPI::getAsQVector(env, other, elementMetaType(), ptr)) {
            QTJAMBI_ELEMENT_LOCKER(this);
                return *reinterpret_cast<const QVector<T> *>(container)==*reinterpret_cast<const QVector<T> *>(ptr);
        }else{
            QTJAMBI_ELEMENT_LOCKER(this);
            QVector<T> list;
            jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, other);
            while(QtJambiAPI::hasJavaIteratorNext(env, iterator)){
                jvalue _value;
                _value.l = QtJambiAPI::nextOfJavaIterator(env, iterator);
                T _qvalue;
                void *_qvaluePtr = &_qvalue;
                if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                    list << _qvalue;
            }
            return *reinterpret_cast<const QVector<T> *>(container)==list;
        }
        return false;
    }

    void move(JNIEnv *, const ContainerInfo& container, jint index1, jint index2) override {

        QTJAMBI_ELEMENT_LOCKER(this);
        reinterpret_cast<QVector<T> *>(container.container)->move(index1, index2);

    }

    ContainerAndAccessInfo mid(JNIEnv * env, const ConstContainerAndAccessInfo& container, jint index1, jint index2) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        ContainerAndAccessInfo result;
        {
            result.container = new QVector<T>(reinterpret_cast<const QVector<T> *>(container.container)->mid(index1, index2));
            result.access = clone();
            result.object = ContainerAPI::objectFromQVector(env, result.container, result.access);
        }

        return result;
    }

    jint lastIndexOf(JNIEnv * env, const void* container, jobject value, jint index) override {
        int idx = -1;

        QTJAMBI_ELEMENT_LOCKER(this);
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

    jint indexOf(JNIEnv * env, const void* container, jobject value, jint index) override {
        int idx = -1;

        QTJAMBI_ELEMENT_LOCKER(this);
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

        QTJAMBI_ELEMENT_LOCKER(this);
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

        QTJAMBI_ELEMENT_LOCKER(this);
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

        QTJAMBI_ELEMENT_LOCKER(this);
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                result = reinterpret_cast<const QVector<T> *>(container)->contains(_qvalue);
        }

        return result;
    }

    void clear(JNIEnv *, const ContainerInfo& container) override {

        QTJAMBI_ELEMENT_LOCKER(this);
        reinterpret_cast<QVector<T> *>(container.container)->clear();

    }

    jint capacity(JNIEnv *, const void* container) override {

        QTJAMBI_ELEMENT_LOCKER(this);
        return reinterpret_cast<const QVector<T> *>(container)->capacity();

    }

    void fill(JNIEnv * env, const ContainerInfo& container, jobject value, jint size) override {
        jvalue _value;
        _value.l = value;

        QTJAMBI_ELEMENT_LOCKER(this);
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                reinterpret_cast<QVector<T> *>(container.container)->fill(_qvalue, size);
        }

    }

    void remove(JNIEnv *, const ContainerInfo& container, jint index, jint n) override {

        QTJAMBI_ELEMENT_LOCKER(this);
        reinterpret_cast<QVector<T> *>(container.container)->remove(index, n);

    }

    void insert(JNIEnv * env, const ContainerInfo& container, jint index, jint n, jobject value) override {
        jvalue _value;
        _value.l = value;

        QTJAMBI_ELEMENT_LOCKER(this);
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                reinterpret_cast<QVector<T> *>(container.container)->insert(index, n, _qvalue);
        }

    }

    void resize(JNIEnv *, const ContainerInfo& container, jint newSize) override {

        QTJAMBI_ELEMENT_LOCKER(this);
        reinterpret_cast<QVector<T> *>(container.container)->resize(newSize);

    }

    void squeeze(JNIEnv *, const ContainerInfo& container) override {

        QTJAMBI_ELEMENT_LOCKER(this);
        reinterpret_cast<QVector<T> *>(container.container)->squeeze();

    }

    bool isDetached(const void* container) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        return reinterpret_cast<const QVector<T> *>(container)->isDetached();
    }

    void detach(const ContainerInfo& container) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        reinterpret_cast<QVector<T> *>(container.container)->detach();
    }

    bool isSharedWith(const void* container, const void* container2) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        return reinterpret_cast<const QVector<T> *>(container)->isSharedWith(*reinterpret_cast<const QVector<T> *>(container2));
    }

    void swap(JNIEnv *, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        reinterpret_cast<QVector<T> *>(container.container)->swap(*reinterpret_cast<QVector<T> *>(container2.container));
    }

    class ElementIterator : public AbstractVectorAccess::ElementIterator{
    public:
        GenericVectorAccess* access;
        typename QVector<T>::ConstIterator current;
        typename QVector<T>::ConstIterator end;
        ElementIterator(GenericVectorAccess* _access, const void* container)
            : access(_access){
            QTJAMBI_ELEMENT_LOCKER(access);
            current = reinterpret_cast<const QVector<T>*>(container)->constBegin();
            end = reinterpret_cast<const QVector<T>*>(container)->constEnd();
        }
        ~ElementIterator() override {};
        bool hasNext() override {return current!=end;};
        jobject next(JNIEnv * env) override {
            QTJAMBI_ELEMENT_LOCKER(access);
            jvalue v;
            v.l = nullptr;
            access->m_internalToExternalConverter(env, nullptr, &*current, v, true);
            ++current;
            return v.l;
        }
        const void* next() override {
            QTJAMBI_ELEMENT_LOCKER(access);
            const void* pointer = QtJambiPrivate::ContainerContentDeref<T, _size==0>::deref(*current);
            ++current;
            return pointer;
        }
    };
    std::unique_ptr<AbstractVectorAccess::ElementIterator> elementIterator(const void* container) override {
        return std::unique_ptr<AbstractVectorAccess::ElementIterator>(new ElementIterator(this, container));
    }
};

template<size_t align, size_t size, bool isStatic>
struct SequentialContainerAccessFac<QVector,align,size,isStatic>{
    static constexpr SequentialContainerAccessFactory factory(){
        return SequentialContainerAccessFactory(GenericVectorAccess<align,size>::newInstance);
    }
};

template<size_t align, size_t size, bool isStatic>
struct SequentialContainerAccessFac<QStack,align,size,isStatic> : SequentialContainerAccessFac<QVector,align,size,isStatic>{
};

}

#endif

#endif //defined(QTJAMBI_GENERIC_ACCESS)

#endif // CONTAINERACCESS_VECTOR_H
