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


#ifndef CONTAINERACCESS_LINKEDLIST_H
#define CONTAINERACCESS_LINKEDLIST_H

#include <QtCore/QtGlobal>
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)

#include <QtCore/QLinkedList>
#include "containeraccess.h"

namespace ContainerAccessAPI {

template<size_t _align, size_t _size>
class GenericLinkedListAccess : public AbstractLinkedListAccess{
    typedef typename std::conditional<_size==0, void*, ContainerElement<_size, 0, false, _align>>::type T;
    MetaTypeInfo<0,_size==0> m_elementMetaTypeInfo;
    InternalToExternalConverter m_internalToExternalConverter;
    ExternalToInternalConverter m_externalToInternalConverter;
    GenericLinkedListAccess(
            const QMetaType& metaType,
            const QHashFunction& hashFunction,
            const InternalToExternalConverter& internalToExternalConverter,
            const ExternalToInternalConverter& externalToInternalConverter
            )
        :   AbstractLinkedListAccess(),
          m_elementMetaTypeInfo(metaType, hashFunction),
          m_internalToExternalConverter(internalToExternalConverter),
          m_externalToInternalConverter(externalToInternalConverter)
    {
    }

    GenericLinkedListAccess(const GenericLinkedListAccess<_align, _size>& other)
        :   AbstractLinkedListAccess(),
          m_elementMetaTypeInfo(other.m_elementMetaTypeInfo),
          m_internalToExternalConverter(other.m_internalToExternalConverter),
          m_externalToInternalConverter(other.m_externalToInternalConverter)
    {
    }

public:
    static AbstractLinkedListAccess* newInstance(const QMetaType& metaType,
                                           const QHashFunction& hashFunction,
                                           const InternalToExternalConverter& internalToExternalConverter,
                                           const ExternalToInternalConverter& externalToInternalConverter){
        return new GenericLinkedListAccess(
                                     metaType,
                                     hashFunction,
                                     internalToExternalConverter,
                                     externalToInternalConverter);
    }

    void dispose() override { delete this; }

    GenericLinkedListAccess<_align, _size>* clone() override{
        return new GenericLinkedListAccess<_align, _size>(*this);
    }

    size_t sizeOf() override {
        return sizeof(QLinkedList<T>);
    }

    void analyzeElements(const void* container, ElementAnalyzer analyzer, void* data) override {
        for(auto iter = reinterpret_cast<const QLinkedList<T>*>(container)->begin();
            iter != reinterpret_cast<const QLinkedList<T>*>(container)->end();
            ++iter
            ){
            if(!analyzer(QtJambiPrivate::pointer_from<decltype(*iter)>::from(*iter), data)){
                break;
            }
        }
    }
    void* constructContainer(void* placement, const void* copyOf = nullptr) override {
        QTJAMBI_ELEMENT_LOCKER
        if(copyOf){
            return new(placement) QLinkedList<T>(*reinterpret_cast<const QLinkedList<T>*>(copyOf));
        }else{
            return new(placement) QLinkedList<T>();
        }
    }
    bool destructContainer(void* container) override {
        QTJAMBI_ELEMENT_LOCKER
        reinterpret_cast<QLinkedList<T>*>(container)->~QLinkedList<T>();
        return true;
    }
    void assign(void* container, const void* other) override {
        QTJAMBI_ELEMENT_LOCKER
        (*reinterpret_cast<QLinkedList<T>*>(container)) = (*reinterpret_cast<const QLinkedList<T>*>(other));
    }
    int registerContainer(const QByteArray& containerTypeName) override {
        return QtJambiPrivate::registerSequentialContainerType<QLinkedList<T>, _size>(containerTypeName, m_elementMetaTypeInfo.metaType());
    }
    bool isConstant() override {return false;}
    const QMetaType& elementMetaType() override {return m_elementMetaTypeInfo.metaType();}

    jobject end(JNIEnv * env, QtJambiNativeID ownerId, void* container) override {
        SequentialConstIteratorAccess<QLinkedList, _align, _size, false, false>* access = new SequentialIteratorAccess<QLinkedList, _align, _size, false>(
                    m_elementMetaTypeInfo, m_internalToExternalConverter, m_externalToInternalConverter);
        return QtJambiAPI::convertQSequentialIteratorToJavaObject(env,
                                      ownerId,
                                      new typename QLinkedList<T>::iterator(reinterpret_cast<QLinkedList<T> *>(container)->end()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QLinkedList<T>::iterator*>(ptr); },
                                      access);
    }

    jobject begin(JNIEnv * env, QtJambiNativeID ownerId, void* container) override {
        SequentialConstIteratorAccess<QLinkedList, _align, _size, false, false>* access = new SequentialIteratorAccess<QLinkedList, _align, _size, false>(
                    m_elementMetaTypeInfo, m_internalToExternalConverter, m_externalToInternalConverter);
        return QtJambiAPI::convertQSequentialIteratorToJavaObject(env,
                                      ownerId,
                                      new typename QLinkedList<T>::iterator(reinterpret_cast<QLinkedList<T> *>(container)->begin()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QLinkedList<T>::iterator*>(ptr); },
                                      access);
    }

    jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        AbstractSequentialConstIteratorAccess* access = new SequentialConstIteratorAccess<QLinkedList, _align, _size, false>(
                    m_elementMetaTypeInfo, m_internalToExternalConverter);
        return QtJambiAPI::convertQSequentialIteratorToJavaObject(env,
                                      ownerId,
                                      new typename QLinkedList<T>::const_iterator(reinterpret_cast<const QLinkedList<T> *>(container)->constEnd()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QLinkedList<T>::const_iterator*>(ptr); },
                                      access);
    }

    jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        AbstractSequentialConstIteratorAccess* access = new SequentialConstIteratorAccess<QLinkedList, _align, _size, false>(
                    m_elementMetaTypeInfo, m_internalToExternalConverter);
        return QtJambiAPI::convertQSequentialIteratorToJavaObject(env,
                                      ownerId,
                                      new typename QLinkedList<T>::const_iterator(reinterpret_cast<const QLinkedList<T> *>(container)->constBegin()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QLinkedList<T>::const_iterator*>(ptr); },
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
                reinterpret_cast<QLinkedList<T> *>(container)->append(_qvalue);
            }
        }
    }

    jobject first(JNIEnv * env, const void* container) override {
        QTJAMBI_ELEMENT_LOCKER
        const T& result = reinterpret_cast<const QLinkedList<T> *>(container)->first();
        jvalue _value;
        _value.l = nullptr;
        if(m_internalToExternalConverter(env, nullptr, &result, _value, true)){
            return _value.l;
        }
        return nullptr;
    }

    jobject last(JNIEnv * env, const void* container) override {
        jvalue _result;
        _result.l = nullptr;
        bool success = false;
        QTJAMBI_ELEMENT_LOCKER
        {
            T qresult = reinterpret_cast<const QLinkedList<T> *>(container)->last();
            success = m_internalToExternalConverter(env, nullptr, &qresult, _result, true);
        }
        if(success){
            return _result.l;
        }
        return nullptr;
    }

    jobject takeFirst(JNIEnv * env, void* container) override {
        jvalue _value;
        _value.l = nullptr;
        bool success = false;
        QTJAMBI_ELEMENT_LOCKER
        {
            T result = reinterpret_cast<QLinkedList<T> *>(container)->takeFirst();
            success = m_internalToExternalConverter(env, nullptr, &result, _value, true);
        }
        if(success){
            return _value.l;
        }
        return nullptr;
    }

    jobject takeLast(JNIEnv * env, void* container) override {
        jvalue _value;
        _value.l = nullptr;
        bool success = false;
        QTJAMBI_ELEMENT_LOCKER
        {
            T result = reinterpret_cast<QLinkedList<T> *>(container)->takeLast();
            success = m_internalToExternalConverter(env, nullptr, &result, _value, true);
        }
        if(success){
            return _value.l;
        }
        return nullptr;
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
                result = reinterpret_cast<const QLinkedList<T> *>(container)->startsWith(_qvalue);
        }
        return result;
    }

    jint size(JNIEnv *, const void* container) override {
        QTJAMBI_ELEMENT_LOCKER
        jint result = reinterpret_cast<const QLinkedList<T> *>(container)->size();
        return result;
    }

    void removeFirst(JNIEnv *, void* container) override {
        QTJAMBI_ELEMENT_LOCKER
        reinterpret_cast<QLinkedList<T> *>(container)->removeFirst();
    }

    void removeLast(JNIEnv *, void* container) override {
        QTJAMBI_ELEMENT_LOCKER
        reinterpret_cast<QLinkedList<T> *>(container)->removeLast();
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
                success = reinterpret_cast<QLinkedList<T> *>(container)->removeOne(_qvalue);
        }
        return success;
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
                count = reinterpret_cast<QLinkedList<T> *>(container)->removeAll(_qvalue);
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
                reinterpret_cast<QLinkedList<T> *>(container)->prepend(_qvalue);
        }
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        void* ptr{nullptr};
        if (ContainerAPI::getAsQLinkedList(env, other, elementMetaType(), ptr)) {
            QTJAMBI_ELEMENT_LOCKER
            bool equals = *reinterpret_cast<const QLinkedList<T> *>(container)==*reinterpret_cast<const QLinkedList<T> *>(ptr);
            return equals;
        }else{
            QTJAMBI_ELEMENT_LOCKER
            QLinkedList<T> list;
            jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, other);
            while(QtJambiAPI::hasJavaIteratorNext(env, iterator)){
                jvalue _value;
                _value.l = QtJambiAPI::nextOfJavaIterator(env, iterator);
                T _qvalue;
                void *_qvaluePtr = &_qvalue;
                if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                    list << _qvalue;
            }
            return *reinterpret_cast<const QLinkedList<T> *>(container)==list;
        }
        return false;
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
                result = reinterpret_cast<const QLinkedList<T> *>(container)->endsWith(_qvalue);
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
                result = reinterpret_cast<const QLinkedList<T> *>(container)->count(_qvalue);
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
                result = reinterpret_cast<const QLinkedList<T> *>(container)->contains(_qvalue);
        }
        return result;
    }

    void clear(JNIEnv *, void* container) override {
        QTJAMBI_ELEMENT_LOCKER
        reinterpret_cast<QLinkedList<T> *>(container)->clear();
    }
};

template<size_t align, size_t size, bool isStatic>
struct SequentialContainerAccessFac<QLinkedList,align,size,isStatic>{
    static constexpr SequentialContainerAccessFactory factory(){
        return SequentialContainerAccessFactory(GenericLinkedListAccess<align,size>::newInstance);
    }
};

}

#endif

#endif // CONTAINERACCESS_LINKEDLIST_H
