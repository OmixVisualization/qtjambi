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


#ifndef CONTAINERACCESS_SET_H
#define CONTAINERACCESS_SET_H

#include <QtCore/QSet>
#include "containeraccess.h"

#if defined(QTJAMBI_GENERIC_ACCESS)

namespace ContainerAccessAPI {

template<size_t _align, size_t _size>
class GenericSetAccess : public AbstractSetAccess, public AbstractNestedSequentialAccess {
    typedef typename std::conditional<_size==0, void*, ContainerElement<_size, 0, false, _align>>::type T;
    MetaTypeInfo<0,_size==0> m_elementMetaTypeInfo;
    QtJambiUtils::InternalToExternalConverter m_internalToExternalConverter;
    QtJambiUtils::ExternalToInternalConverter m_externalToInternalConverter;
    QSharedPointer<AbstractContainerAccess> m_elementNestedContainerAccess;
    AbstractContainerAccess::DataType m_elementDataType;
    PtrOwnerFunction m_elementOwnerFunction;
    GenericSetAccess(
            const QMetaType& metaType,
            const QtJambiUtils::QHashFunction& hashFunction,
            const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
            const QtJambiUtils::ExternalToInternalConverter& externalToInternalConverter,
            const QSharedPointer<AbstractContainerAccess>& elementNestedContainerAccess,
            PtrOwnerFunction elementOwnerFunction
            )
        :   AbstractSetAccess(), AbstractNestedSequentialAccess(),
          m_elementMetaTypeInfo(metaType, hashFunction),
          m_internalToExternalConverter(internalToExternalConverter),
          m_externalToInternalConverter(externalToInternalConverter),
          m_elementNestedContainerAccess(elementNestedContainerAccess),
          m_elementDataType(dataType(m_elementMetaTypeInfo.metaType(), m_elementNestedContainerAccess)),
          m_elementOwnerFunction(elementOwnerFunction)
    {
    }

    GenericSetAccess(const GenericSetAccess<_align, _size>& other)
        :   AbstractSetAccess(), AbstractNestedSequentialAccess(),
          m_elementMetaTypeInfo(other.m_elementMetaTypeInfo),
          m_internalToExternalConverter(other.m_internalToExternalConverter),
          m_externalToInternalConverter(other.m_externalToInternalConverter),
          m_elementNestedContainerAccess(other.m_elementNestedContainerAccess),
          m_elementDataType(other.m_elementDataType),
          m_elementOwnerFunction(other.m_elementOwnerFunction)
    {
    }

public:
    static AbstractSetAccess* newInstance(const QMetaType& metaType,
                                          const QtJambiUtils::QHashFunction& hashFunction,
                                          const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
                                          const QtJambiUtils::ExternalToInternalConverter& externalToInternalConverter,
                                          const QSharedPointer<AbstractContainerAccess>& elementNestedContainerAccess,
                                          PtrOwnerFunction elementOwnerFunction){
        return new GenericSetAccess(metaType,
                                    hashFunction,
                                    internalToExternalConverter,
                                    externalToInternalConverter,
                                    elementNestedContainerAccess,
                                    elementOwnerFunction);
    }

    void dispose() override { delete this; }

    GenericSetAccess<_align, _size>* clone() override{
        return new GenericSetAccess<_align, _size>(*this);
    }

    size_t sizeOf() override {
        return sizeof(QSet<T>);
    }

    void assign(JNIEnv *, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        (*reinterpret_cast<QSet<T>*>(container.container)) = (*reinterpret_cast<const QSet<T>*>(other.container));
    }

    void assign(void* container, const void* other) override {
        QTJAMBI_ELEMENT_LOCKER(this);
            (*reinterpret_cast<QSet<T>*>(container)) = (*reinterpret_cast<const QSet<T>*>(other));
    }

    void* constructContainer(void* placement) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        return new(placement) QSet<T>();
    }

    void* constructContainer(void* placement, const void* copyOf) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        return new(placement) QSet<T>(*reinterpret_cast<const QSet<T>*>(copyOf));
    }
    void* constructContainer(JNIEnv *, void* placement, const ConstContainerAndAccessInfo& copyOf) override {
        return constructContainer(placement, copyOf.container);
    }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* move) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        return new(placement) QSet<T>(std::move(*reinterpret_cast<const QSet<T>*>(move)));
    }
    void* constructContainer(JNIEnv *, void* placement, const ContainerAndAccessInfo& move) override {
        return constructContainer(placement, move.container);
    }
#endif
    bool destructContainer(void* container) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        reinterpret_cast<QSet<T>*>(container)->~QSet<T>();
        return true;
    }

    int registerContainer(const QByteArray& containerTypeName) override {
        return QtJambiPrivate::registerSequentialContainerType<QSet<T>, _size>(containerTypeName, m_elementMetaTypeInfo.metaType(), this);
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

private:
    struct Iterator{
        typename QSet<T>::iterator i;
    };
public:

    jobject constEnd(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        AbstractSequentialConstIteratorAccess* access = new SequentialConstIteratorAccess<QSet, _align, _size, false>(
                    m_elementMetaTypeInfo, m_internalToExternalConverter);
        return QtJambiAPI::convertQSequentialIteratorToJavaObject(env,
                                      container.nativeId,
                                      new typename QSet<T>::const_iterator(reinterpret_cast<const QSet<T> *>(container.container)->constEnd()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QSet<T>::const_iterator*>(ptr); },
                                      access);
    }

    jobject constBegin(JNIEnv * env, const ConstExtendedContainerInfo& container) override {
        AbstractSequentialConstIteratorAccess* access = new SequentialConstIteratorAccess<QSet, _align, _size, false>(
                    m_elementMetaTypeInfo, m_internalToExternalConverter);
        return QtJambiAPI::convertQSequentialIteratorToJavaObject(env,
                                      container.nativeId,
                                      new typename QSet<T>::const_iterator(reinterpret_cast<const QSet<T> *>(container.container)->constBegin()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QSet<T>::const_iterator*>(ptr); },
                                      access);
    }

    void insert(JNIEnv * env, const ContainerInfo& container, jobject value) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            jvalue _value;
            _value.l = value;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l)){
                reinterpret_cast<QSet<T> *>(container.container)->insert(_qvalue);
            }
        }
    }

    void intersect(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override {
        if (ContainerAPI::getAsQSet(env, other.object, elementMetaType(), other.container, other.access)) {
            QTJAMBI_ELEMENT_LOCKER(this);
                reinterpret_cast<QSet<T> *>(container.container)->intersect(*reinterpret_cast<QSet<T> *>(other.container));
        }else{
            QTJAMBI_ELEMENT_LOCKER(this);
            QSet<T> list;
            jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, other.object);
            while(QtJambiAPI::hasJavaIteratorNext(env, iterator)){
                jvalue _value;
                _value.l = QtJambiAPI::nextOfJavaIterator(env, iterator);
                T _qvalue;
                void *_qvaluePtr = &_qvalue;
                if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                    list << _qvalue;
            }
            reinterpret_cast<QSet<T> *>(container.container)->intersect(list);
        }
    }

    void unite(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override {
        if (ContainerAPI::getAsQSet(env, other.object, elementMetaType(), other.container, other.access)) {
            QTJAMBI_ELEMENT_LOCKER(this);
                reinterpret_cast<QSet<T> *>(container.container)->unite(*reinterpret_cast<QSet<T> *>(other.container));
        }else{
            QTJAMBI_ELEMENT_LOCKER(this);
            QSet<T> list;
            jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, other.object);
            while(QtJambiAPI::hasJavaIteratorNext(env, iterator)){
                jvalue _value;
                _value.l = QtJambiAPI::nextOfJavaIterator(env, iterator);
                T _qvalue;
                void *_qvaluePtr = &_qvalue;
                if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                    list << _qvalue;
            }
            reinterpret_cast<QSet<T> *>(container.container)->unite(list);
        }
    }

    void subtract(JNIEnv * env, const ContainerInfo& container, ContainerAndAccessInfo& other) override {
        if (ContainerAPI::getAsQSet(env, other.object, elementMetaType(), other.container, other.access)) {
            QTJAMBI_ELEMENT_LOCKER(this);
            reinterpret_cast<QSet<T> *>(container.container)->subtract(*reinterpret_cast<QSet<T> *>(other.container));
        }else{
            QTJAMBI_ELEMENT_LOCKER(this);
            QSet<T> list;
            jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, other.object);
            while(QtJambiAPI::hasJavaIteratorNext(env, iterator)){
                jvalue _value;
                _value.l = QtJambiAPI::nextOfJavaIterator(env, iterator);
                T _qvalue;
                void *_qvaluePtr = &_qvalue;
                if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                    list << _qvalue;
            }
            reinterpret_cast<QSet<T> *>(container.container)->subtract(list);
        }
    }

    jboolean intersects(JNIEnv * env, const void* container, jobject other) override {
        void* ptr{nullptr};
        if (ContainerAPI::getAsQSet(env, other, elementMetaType(), ptr)) {
            QTJAMBI_ELEMENT_LOCKER(this);
                bool intersects = reinterpret_cast<const QSet<T> *>(container)->intersects(*reinterpret_cast<const QSet<T> *>(ptr));
            return intersects;
        }else{
            QTJAMBI_ELEMENT_LOCKER(this);
            QSet<T> list;
            jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, other);
            while(QtJambiAPI::hasJavaIteratorNext(env, iterator)){
                jvalue _value;
                _value.l = QtJambiAPI::nextOfJavaIterator(env, iterator);
                T _qvalue;
                void *_qvaluePtr = &_qvalue;
                if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                    list << _qvalue;
            }
            return reinterpret_cast<const QSet<T> *>(container)->intersects(list);
        }
        return false;
    }

    ContainerAndAccessInfo values(JNIEnv * env, const ConstContainerInfo& container) override {
        ContainerAndAccessInfo result;
        QTJAMBI_ELEMENT_LOCKER(this);
        result.access = ContainerAccessAPI::createContainerAccess(SequentialContainerType::QList, m_elementMetaTypeInfo.metaType());
        if(!result.access)
            result.access = ContainerAccessAPI::createContainerAccess(env, SequentialContainerType::QList,
                                                      m_elementMetaTypeInfo.metaType(),
                                                      _align, _size,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                      AbstractContainerAccess::isStaticType(m_elementMetaTypeInfo.metaType()),
#endif
                                                      AbstractContainerAccess::isPointerType(m_elementMetaTypeInfo.metaType()),
                                                      m_elementMetaTypeInfo.hashFunction(),
                                                      m_internalToExternalConverter,
                                                      m_externalToInternalConverter,
                                                      m_elementNestedContainerAccess,
                                                      m_elementOwnerFunction
                                                    );
        if(result.access){
            result.container = new QList<T>(reinterpret_cast<const QSet<T> *>(container.container)->values());
            result.object = ContainerAPI::objectFromQList(env, result.container, result.access);
        }
        return result;
    }

    jint size(JNIEnv *, const void* container) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        jint result = reinterpret_cast<const QSet<T> *>(container)->size();
        return result;
    }

    void reserve(JNIEnv *, const ContainerInfo& container, jint size) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        reinterpret_cast<QSet<T> *>(container.container)->reserve(size);
    }

    jboolean remove(JNIEnv * env, const ContainerInfo& container, jobject value) override {
        jvalue _value;
        _value.l = value;
        bool success = false;
        QTJAMBI_ELEMENT_LOCKER(this);
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                success = reinterpret_cast<QSet<T> *>(container.container)->remove(_qvalue);
        }
        return success;
    }

    jboolean equal(JNIEnv * env, const void* container, jobject other) override {
        void* ptr{nullptr};
        if (ContainerAPI::getAsQSet(env, other, elementMetaType(), ptr)) {
            QTJAMBI_ELEMENT_LOCKER(this);
            bool equals = *reinterpret_cast<const QSet<T> *>(container)==*reinterpret_cast<QSet<T> *>(ptr);
            return equals;
        }else{
            QTJAMBI_ELEMENT_LOCKER(this);
            QSet<T> list;
            jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, other);
            while(QtJambiAPI::hasJavaIteratorNext(env, iterator)){
                jvalue _value;
                _value.l = QtJambiAPI::nextOfJavaIterator(env, iterator);
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
        QTJAMBI_ELEMENT_LOCKER(this);
        {
            T _qvalue;
            void *_qvaluePtr = &_qvalue;
            if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                result = reinterpret_cast<const QSet<T> *>(container)->contains(_qvalue);
        }
        return result;
    }

    void clear(JNIEnv *, const ContainerInfo& container) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        reinterpret_cast<QSet<T> *>(container.container)->clear();
    }

    jint capacity(JNIEnv *, const void* container) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        return reinterpret_cast<const QSet<T> *>(container)->capacity();
    }

    bool isDetached(const void* container) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        return reinterpret_cast<const QSet<T> *>(container)->isDetached();
    }

    void detach(const ContainerInfo& container) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        reinterpret_cast<QSet<T> *>(container.container)->detach();
    }

    bool isSharedWith(const void* container, const void* container2) override {
        QTJAMBI_ELEMENT_LOCKER(this);
        return reinterpret_cast<const QHash<T,QHashDummyValue> *>(container)->isSharedWith(*reinterpret_cast<const QHash<T,QHashDummyValue> *>(container2));
    }

    void swap(JNIEnv *, const ContainerInfo& container, const ContainerAndAccessInfo& container2) override {
        QTJAMBI_ELEMENT_LOCKER(this);
            reinterpret_cast<QSet<T> *>(container.container)->swap(*reinterpret_cast<QSet<T> *>(container2.container));
    }

    class ElementIterator : public AbstractSetAccess::ElementIterator{
    public:
        GenericSetAccess* access;
        typename QSet<T>::ConstIterator current;
        typename QSet<T>::ConstIterator end;
        ElementIterator(GenericSetAccess* _access, const void* container)
            : access(_access){
            QTJAMBI_ELEMENT_LOCKER(access);
            current = reinterpret_cast<const QSet<T>*>(container)->constBegin();
            end = reinterpret_cast<const QSet<T>*>(container)->constEnd();
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
    std::unique_ptr<AbstractSetAccess::ElementIterator> elementIterator(const void* container) override {
        return std::unique_ptr<AbstractSetAccess::ElementIterator>(new ElementIterator(this, container));
    }
};

template<size_t align, size_t size, bool isStatic>
struct SequentialContainerAccessFac<QSet,align,size,isStatic>{
    static constexpr SequentialContainerAccessFactory factory(){
        return SequentialContainerAccessFactory(GenericSetAccess<align,size>::newInstance);
    }
};

}

#endif //defined(QTJAMBI_GENERIC_ACCESS)

#endif // CONTAINERACCESS_SET_H
