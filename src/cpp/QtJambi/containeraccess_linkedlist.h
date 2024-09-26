/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#if defined(QTJAMBI_GENERIC_ACCESS)

namespace ContainerAccessAPI {

template<size_t _align, size_t _size>
class GenericLinkedListAccess : public AbstractLinkedListAccess, public AbstractNestedSequentialAccess {
    typedef typename std::conditional<_size==0, void*, ContainerElement<_size, 0, false, _align>>::type T;
    MetaTypeInfo<0,_size==0> m_elementMetaTypeInfo;
    QtJambiUtils::InternalToExternalConverter m_internalToExternalConverter;
    QtJambiUtils::ExternalToInternalConverter m_externalToInternalConverter;
    QSharedPointer<AbstractContainerAccess> m_elementNestedContainerAccess;
    AbstractContainerAccess::DataType m_elementDataType;
    PtrOwnerFunction m_elementOwnerFunction;
    GenericLinkedListAccess(
            const QMetaType& metaType,
            const QtJambiUtils::QHashFunction& hashFunction,
            const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
            const QtJambiUtils::ExternalToInternalConverter& externalToInternalConverter,
            const QSharedPointer<AbstractContainerAccess>& elementNestedContainerAccess,
            PtrOwnerFunction elementOwnerFunction
            )
        :   AbstractLinkedListAccess(), AbstractNestedSequentialAccess(),
          m_elementMetaTypeInfo(metaType, hashFunction),
          m_internalToExternalConverter(internalToExternalConverter),
          m_externalToInternalConverter(externalToInternalConverter),
          m_elementNestedContainerAccess(elementNestedContainerAccess),
          m_elementDataType(dataType(m_elementMetaTypeInfo.metaType(), m_elementNestedContainerAccess)),
          m_elementOwnerFunction(elementOwnerFunction)
    {
    }

    GenericLinkedListAccess(const GenericLinkedListAccess<_align, _size>& other)
        :   AbstractLinkedListAccess(), AbstractNestedSequentialAccess(),
          m_elementMetaTypeInfo(other.m_elementMetaTypeInfo),
          m_internalToExternalConverter(other.m_internalToExternalConverter),
          m_externalToInternalConverter(other.m_externalToInternalConverter),
          m_elementNestedContainerAccess(other.m_elementNestedContainerAccess),
          m_elementDataType(other.m_elementDataType),
          m_elementOwnerFunction(other.m_elementOwnerFunction)
    {
    }

public:
    static AbstractLinkedListAccess* newInstance(const QMetaType& metaType,
                                                 const QtJambiUtils::QHashFunction& hashFunction,
                                                 const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
                                                 const QtJambiUtils::ExternalToInternalConverter& externalToInternalConverter,
                                                 const QSharedPointer<AbstractContainerAccess>& elementNestedContainerAccess,
                                                 PtrOwnerFunction elementOwnerFunction){
        return new GenericLinkedListAccess(
                                           metaType,
                                           hashFunction,
                                           internalToExternalConverter,
                                           externalToInternalConverter,
                                           elementNestedContainerAccess,
                                           elementOwnerFunction);
    }

    void dispose() override { delete this; }

    GenericLinkedListAccess<_align, _size>* clone() override{
        return new GenericLinkedListAccess<_align, _size>(*this);
    }

    size_t sizeOf() override {
        return sizeof(QLinkedList<T>);
    }

    void* constructContainer(JNIEnv *, void* placement, const ConstContainerAndAccessInfo& copyOf) override {
        return constructContainer(placement, copyOf.container);
    }
    void* constructContainer(void* placement) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        return new(placement) QLinkedList<T>();
    }
    void* constructContainer(void* placement, const void* copyOf) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        return new(placement) QLinkedList<T>(*reinterpret_cast<const QLinkedList<T>*>(copyOf));
    }
    bool destructContainer(void* container) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        reinterpret_cast<QLinkedList<T>*>(container)->~QLinkedList<T>();
        return true;
    }
    void assign(JNIEnv *, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        (*reinterpret_cast<QLinkedList<T>*>(container.container)) = (*reinterpret_cast<const QLinkedList<T>*>(other.container));
    }
    void assign(void* container, const void* other) override {
        QTJAMBI_ELEMENT_LOCKER(this);
            (*reinterpret_cast<QLinkedList<T>*>(container)) = (*reinterpret_cast<const QLinkedList<T>*>(other));
    }
    int registerContainer(const QByteArray& containerTypeName) override {
        return QtJambiPrivate::registerSequentialContainerType<QLinkedList<T>, _size>(containerTypeName, m_elementMetaTypeInfo.metaType(), this);
    }
    const QMetaType& elementMetaType() override {return m_elementMetaTypeInfo.metaType();}

    DataType elementType() override {
        return m_elementDataType;
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
        SequentialConstIteratorAccess<QLinkedList, _align, _size, false, false>* access = new SequentialIteratorAccess<QLinkedList, _align, _size, false>(
                    m_elementMetaTypeInfo, m_internalToExternalConverter, m_externalToInternalConverter);
        return QtJambiAPI::convertQSequentialIteratorToJavaObject(env,
                                      container.nativeId,
                                      new typename QLinkedList<T>::iterator(reinterpret_cast<QLinkedList<T> *>(container.container)->end()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QLinkedList<T>::iterator*>(ptr); },
                                      access);
    }

    jobject begin(JNIEnv * env, const ExtendedContainerInfo& container) override {
        SequentialConstIteratorAccess<QLinkedList, _align, _size, false, false>* access = new SequentialIteratorAccess<QLinkedList, _align, _size, false>(
                    m_elementMetaTypeInfo, m_internalToExternalConverter, m_externalToInternalConverter);
        return QtJambiAPI::convertQSequentialIteratorToJavaObject(env,
                                      container.nativeId,
                                      new typename QLinkedList<T>::iterator(reinterpret_cast<QLinkedList<T> *>(container.container)->begin()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QLinkedList<T>::iterator*>(ptr); },
                                      access);
    }

    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        AbstractSequentialConstIteratorAccess* access = new SequentialConstIteratorAccess<QLinkedList, _align, _size, false>(
                    m_elementMetaTypeInfo, m_internalToExternalConverter);
        return QtJambiAPI::convertQSequentialIteratorToJavaObject(env,
                                      container.nativeId,
                                      new typename QLinkedList<T>::const_iterator(reinterpret_cast<const QLinkedList<T> *>(container.container)->constEnd()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QLinkedList<T>::const_iterator*>(ptr); },
                                      access);
    }

    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        AbstractSequentialConstIteratorAccess* access = new SequentialConstIteratorAccess<QLinkedList, _align, _size, false>(
                    m_elementMetaTypeInfo, m_internalToExternalConverter);
        return QtJambiAPI::convertQSequentialIteratorToJavaObject(env,
                                      container.nativeId,
                                      new typename QLinkedList<T>::const_iterator(reinterpret_cast<const QLinkedList<T> *>(container.container)->constBegin()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QLinkedList<T>::const_iterator*>(ptr); },
                                      access);
    }

    void append(JNIEnv * env, const ContainerInfo& container, jobject value) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            jvalue _value;
            _value.l = value;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l)){
                reinterpret_cast<QLinkedList<T> *>(container.container)->append(_qvalue);
            }
        }
    }

    jobject first(JNIEnv * env, const void* container) override {
        QTJAMBI_ELEMENT_LOCKER(this);
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
        QTJAMBI_ELEMENT_LOCKER(this);
        {
            T qresult = reinterpret_cast<const QLinkedList<T> *>(container)->last();
            success = m_internalToExternalConverter(env, nullptr, &qresult, _result, true);
        }
        if(success){
            return _result.l;
        }
        return nullptr;
    }

    jobject takeFirst(JNIEnv * env, const ContainerInfo& container) override {
        jvalue _value;
        _value.l = nullptr;
        bool success = false;
        QTJAMBI_ELEMENT_LOCKER(this);
        {
            T result = reinterpret_cast<QLinkedList<T> *>(container.container)->takeFirst();
            success = m_internalToExternalConverter(env, nullptr, &result, _value, true);
        }
        if(success){
            return _value.l;
        }
        return nullptr;
    }

    jobject takeLast(JNIEnv * env, const ContainerInfo& container) override {
        jvalue _value;
        _value.l = nullptr;
        bool success = false;
        QTJAMBI_ELEMENT_LOCKER(this);
        {
            T result = reinterpret_cast<QLinkedList<T> *>(container.container)->takeLast();
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
        QTJAMBI_ELEMENT_LOCKER(this);
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                result = reinterpret_cast<const QLinkedList<T> *>(container)->startsWith(_qvalue);
        }
        return result;
    }

    jint size(JNIEnv *, const void* container) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        jint result = reinterpret_cast<const QLinkedList<T> *>(container)->size();
        return result;
    }

    void removeFirst(JNIEnv *, const ContainerInfo& container) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        reinterpret_cast<QLinkedList<T> *>(container.container)->removeFirst();
    }

    void removeLast(JNIEnv *, const ContainerInfo& container) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        reinterpret_cast<QLinkedList<T> *>(container.container)->removeLast();
    }

    jboolean removeOne(JNIEnv * env, const ContainerInfo& container, jobject value) override {
        jvalue _value;
        _value.l = value;
        bool success = false;
        QTJAMBI_ELEMENT_LOCKER(this);
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                success = reinterpret_cast<QLinkedList<T> *>(container.container)->removeOne(_qvalue);
        }
        return success;
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
                count = reinterpret_cast<QLinkedList<T> *>(container.container)->removeAll(_qvalue);
        }
        return count;
    }

    void prepend(JNIEnv * env, const ContainerInfo& container, jobject value) override {
        jvalue _value;
        _value.l = value;
        QTJAMBI_ELEMENT_LOCKER(this);
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                reinterpret_cast<QLinkedList<T> *>(container.container)->prepend(_qvalue);
        }
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        void* ptr{nullptr};
        if (ContainerAPI::getAsQLinkedList(env, other, elementMetaType(), ptr)) {
            QTJAMBI_ELEMENT_LOCKER(this);
            bool equals = *reinterpret_cast<const QLinkedList<T> *>(container)==*reinterpret_cast<const QLinkedList<T> *>(ptr);
            return equals;
        }else{
            QTJAMBI_ELEMENT_LOCKER(this);
            QLinkedList<T> list;
            jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, other);
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
        QTJAMBI_ELEMENT_LOCKER(this);
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
        QTJAMBI_ELEMENT_LOCKER(this);
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
        QTJAMBI_ELEMENT_LOCKER(this);
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                result = reinterpret_cast<const QLinkedList<T> *>(container)->contains(_qvalue);
        }
        return result;
    }

    void clear(JNIEnv *, const ContainerInfo& container) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        reinterpret_cast<QLinkedList<T> *>(container.container)->clear();
    }

    bool isDetached(const void* container) override {
        QTJAMBI_ELEMENT_LOCKER(this);
            return reinterpret_cast<const QLinkedList<T> *>(container)->isDetached();
    }

    void detach(const ContainerInfo& container) override {
        QTJAMBI_ELEMENT_LOCKER(this);
            reinterpret_cast<QLinkedList<T> *>(container.container)->detach();
    }

    bool isSharedWith(const void* container, const void* container2) override {
        QTJAMBI_ELEMENT_LOCKER(this);
            return reinterpret_cast<const QLinkedList<T> *>(container)->isSharedWith(*reinterpret_cast<const QLinkedList<T> *>(container2));
    }

    void swap(JNIEnv *, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override {
        QTJAMBI_ELEMENT_LOCKER(this);
            reinterpret_cast<QLinkedList<T> *>(container.container)->swap(*reinterpret_cast<QLinkedList<T> *>(container2.container));
    }

    class ElementIterator : public AbstractLinkedListAccess::ElementIterator{
    public:
        GenericLinkedListAccess* access;
        typename QLinkedList<T>::ConstIterator current;
        typename QLinkedList<T>::ConstIterator end;
        ElementIterator(GenericLinkedListAccess* _access, const void* container)
            : access(_access){
            QTJAMBI_ELEMENT_LOCKER(access);
            current = reinterpret_cast<const QLinkedList<T>*>(container)->constBegin();
            end = reinterpret_cast<const QLinkedList<T>*>(container)->constEnd();
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
    std::unique_ptr<AbstractLinkedListAccess::ElementIterator> elementIterator(const void* container) override {
        return std::unique_ptr<AbstractLinkedListAccess::ElementIterator>(new ElementIterator(this, container));
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

#endif //defined(QTJAMBI_GENERIC_ACCESS)

#endif // CONTAINERACCESS_LINKEDLIST_H
