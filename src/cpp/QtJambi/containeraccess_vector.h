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

#ifndef CONTAINERACCESS_VECTOR_H
#define CONTAINERACCESS_VECTOR_H

#include <QtCore/QtGlobal>
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QtCore/QVector>
#include <QtCore/QStack>
#include <QtCore/QQueue>
#include "containeraccess.h"

namespace ContainerAccessAPI {

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
            if(!analyzer(QtJambiPrivate::pointer_from<decltype(*iter)>::from(*iter), data)){
                break;
            }
        }
    }

    GenericVectorAccess<_align, _size>* clone() override{
        return new GenericVectorAccess<_align, _size>(*this);
    }

    size_t sizeOf() override {
        return sizeof(QVector<T>);
    }
    void assign(void* container, const void* other) override {

        QTJAMBI_ELEMENT_LOCKER
        (*reinterpret_cast<QVector<T>*>(container)) = (*reinterpret_cast<const QVector<T>*>(other));

    }
    void* constructContainer(void* placement, const void* copyOf = nullptr) override {
        QTJAMBI_ELEMENT_LOCKER
        if(copyOf){
            return new(placement) QVector<T>(*reinterpret_cast<const QVector<T>*>(copyOf));
        }else{
            return new(placement) QVector<T>();
        }
    }
    bool destructContainer(void* container) override {
        QTJAMBI_ELEMENT_LOCKER
        reinterpret_cast<QVector<T>*>(container)->~QVector<T>();
        return true;
    }
    int registerContainer(const QByteArray& containerTypeName) override {
        return QtJambiPrivate::registerSequentialContainerType<QVector<T>, _size>(containerTypeName, m_elementMetaTypeInfo.metaType());
    }
    bool isConstant() override {return false;}
    const QMetaType& elementMetaType() override {return m_elementMetaTypeInfo.metaType();}

    jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) override {
        SequentialConstIteratorAccess<QVector, _align, _size, false, false>* access = new SequentialIteratorAccess<QVector, _align, _size, false>(
                    m_elementMetaTypeInfo, m_internalToExternalConverter, m_externalToInternalConverter);
        return QtJambiAPI::convertQSequentialIteratorToJavaObject(env,
                                      ownerId,
                                      new typename QVector<T>::iterator(reinterpret_cast<QVector<T> *>(container)->end()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QVector<T>::iterator*>(ptr); },
                                      access);
    }

    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) override {
        SequentialConstIteratorAccess<QVector, _align, _size, false, false>* access = new SequentialIteratorAccess<QVector, _align, _size, false>(
                    m_elementMetaTypeInfo, m_internalToExternalConverter, m_externalToInternalConverter);
        return QtJambiAPI::convertQSequentialIteratorToJavaObject(env,
                                      ownerId,
                                      new typename QVector<T>::iterator(reinterpret_cast<QVector<T> *>(container)->begin()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QVector<T>::iterator*>(ptr); },
                                      access);
    }

    jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        AbstractSequentialConstIteratorAccess* access = new SequentialConstIteratorAccess<QVector, _align, _size, false>(
                    m_elementMetaTypeInfo, m_internalToExternalConverter);
        return QtJambiAPI::convertQSequentialIteratorToJavaObject(env,
                                      ownerId,
                                      new typename QVector<T>::const_iterator(reinterpret_cast<const QVector<T> *>(container)->constEnd()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QVector<T>::const_iterator*>(ptr); },
                                      access);
    }

    jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        AbstractSequentialConstIteratorAccess* access = new SequentialConstIteratorAccess<QVector, _align, _size, false>(
                    m_elementMetaTypeInfo, m_internalToExternalConverter);
        return QtJambiAPI::convertQSequentialIteratorToJavaObject(env,
                                      ownerId,
                                      new typename QVector<T>::const_iterator(reinterpret_cast<const QVector<T> *>(container)->constBegin()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QVector<T>::const_iterator*>(ptr); },
                                      access);
    }

    void appendVector(JNIEnv * env, void* container, jobject list) override {
        void* ptr{nullptr};
        if (ContainerAPI::getAsQVector(env, list, elementMetaType(), ptr)) {
            QTJAMBI_ELEMENT_LOCKER
            reinterpret_cast<QVector<T> *>(container)->append(*reinterpret_cast<QVector<T> *>(ptr));
        }else{
            jobject iter = QtJambiAPI::iteratorOfJavaCollection(env, list);
            while(QtJambiAPI::hasJavaIteratorNext(env, iter)){
                insert(env, container, size(env, container), 1, QtJambiAPI::nextOfJavaIterator(env, iter));
            }
        }
    }

    jobject at(JNIEnv * env, const void* container, jint index) override {

        QTJAMBI_ELEMENT_LOCKER
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

        QTJAMBI_ELEMENT_LOCKER
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

        QTJAMBI_ELEMENT_LOCKER
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

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        void* ptr{nullptr};
        if (ContainerAPI::getAsQVector(env, other, elementMetaType(), ptr)) {
            QTJAMBI_ELEMENT_LOCKER
                return *reinterpret_cast<const QVector<T> *>(container)==*reinterpret_cast<const QVector<T> *>(ptr);
        }else{
            QTJAMBI_ELEMENT_LOCKER
            QVector<T> list;
            jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, other);
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

    void move(JNIEnv *, void* container, jint index1, jint index2) override {

        QTJAMBI_ELEMENT_LOCKER
        reinterpret_cast<QVector<T> *>(container)->move(index1, index2);

    }

    jobject mid(JNIEnv * env, const void* container, jint index1, jint index2) override {

        QTJAMBI_ELEMENT_LOCKER
        jobject result = nullptr;
        {
            const void* mid = new QVector<T>(reinterpret_cast<const QVector<T> *>(container)->mid(index1, index2));
            result = objectFromQVector(env, mid, clone());
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
#endif // CONTAINERACCESS_VECTOR_H
