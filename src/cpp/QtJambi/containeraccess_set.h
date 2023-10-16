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


#ifndef CONTAINERACCESS_SET_H
#define CONTAINERACCESS_SET_H

#include <QtCore/QSet>
#include "containeraccess.h"

namespace ContainerAccessAPI {

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
            if(!analyzer(QtJambiPrivate::pointer_from<decltype(*iter)>::from(*iter), data)){
                break;
            }
        }
    }

    GenericSetAccess<_align, _size>* clone() override{
        return new GenericSetAccess<_align, _size>(*this);
    }

    size_t sizeOf() override {
        return sizeof(QSet<T>);
    }

    void assign(void* container, const void* other) override {
        QTJAMBI_ELEMENT_LOCKER
        (*reinterpret_cast<QSet<T>*>(container)) = (*reinterpret_cast<const QSet<T>*>(other));
    }

    void* constructContainer(void* placement, const void* copyOf = nullptr) override {
        QTJAMBI_ELEMENT_LOCKER
        if(copyOf){
            return new(placement) QSet<T>(*reinterpret_cast<const QSet<T>*>(copyOf));
        }else{
            return new(placement) QSet<T>();
        }
    }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* move) override {
        QTJAMBI_ELEMENT_LOCKER
        return new(placement) QSet<T>(std::move(*reinterpret_cast<const QSet<T>*>(move)));
    }
#endif
    bool destructContainer(void* container) override {
        QTJAMBI_ELEMENT_LOCKER
        reinterpret_cast<QSet<T>*>(container)->~QSet<T>();
        return true;
    }

    int registerContainer(const QByteArray& containerTypeName) override {
        return QtJambiPrivate::registerSequentialContainerType<QSet<T>, _size>(containerTypeName, m_elementMetaTypeInfo.metaType());
    }

    bool isConstant() override {return false;}
    const QMetaType& elementMetaType() override {return m_elementMetaTypeInfo.metaType();}

private:
    struct Iterator{
        typename QSet<T>::iterator i;
    };
public:

    jobject constEnd(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        AbstractSequentialConstIteratorAccess* access = new SequentialConstIteratorAccess<QSet, _align, _size, false>(
                    m_elementMetaTypeInfo, m_internalToExternalConverter);
        return QtJambiAPI::convertQSequentialIteratorToJavaObject(env,
                                      ownerId,
                                      new typename QSet<T>::const_iterator(reinterpret_cast<const QSet<T> *>(container)->constEnd()),
                                      [](void* ptr,bool){ delete reinterpret_cast<typename QSet<T>::const_iterator*>(ptr); },
                                      access);
    }

    jobject constBegin(JNIEnv * env, QtJambiNativeID ownerId, const void* container) override {
        AbstractSequentialConstIteratorAccess* access = new SequentialConstIteratorAccess<QSet, _align, _size, false>(
                    m_elementMetaTypeInfo, m_internalToExternalConverter);
        return QtJambiAPI::convertQSequentialIteratorToJavaObject(env,
                                      ownerId,
                                      new typename QSet<T>::const_iterator(reinterpret_cast<const QSet<T> *>(container)->constBegin()),
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
        void* ptr{nullptr};
        if (ContainerAPI::getAsQSet(env, other, elementMetaType(), ptr)) {
            QTJAMBI_ELEMENT_LOCKER
                reinterpret_cast<QSet<T> *>(container)->intersect(*reinterpret_cast<QSet<T> *>(ptr));
        }else{
            QTJAMBI_ELEMENT_LOCKER
            QSet<T> list;
            jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, other);
            while(QtJambiAPI::hasJavaIteratorNext(env, iterator)){
                jvalue _value;
                _value.l = QtJambiAPI::nextOfJavaIterator(env, iterator);
                T _qvalue;
                void *_qvaluePtr = &_qvalue;
                if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                    list << _qvalue;
            }
            reinterpret_cast<QSet<T> *>(container)->intersect(list);
        }
    }

    void unite(JNIEnv * env, void* container, jobject other) override {
        void* ptr{nullptr};
        if (ContainerAPI::getAsQSet(env, other, elementMetaType(), ptr)) {
            QTJAMBI_ELEMENT_LOCKER
                reinterpret_cast<QSet<T> *>(container)->unite(*reinterpret_cast<QSet<T> *>(ptr));
        }else{
            QTJAMBI_ELEMENT_LOCKER
            QSet<T> list;
            jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, other);
            while(QtJambiAPI::hasJavaIteratorNext(env, iterator)){
                jvalue _value;
                _value.l = QtJambiAPI::nextOfJavaIterator(env, iterator);
                T _qvalue;
                void *_qvaluePtr = &_qvalue;
                if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                    list << _qvalue;
            }
            reinterpret_cast<QSet<T> *>(container)->unite(list);
        }
    }

    void subtract(JNIEnv * env, void* container, jobject other) override {
        void* ptr{nullptr};
        if (ContainerAPI::getAsQSet(env, other, elementMetaType(), ptr)) {
            QTJAMBI_ELEMENT_LOCKER
            reinterpret_cast<QSet<T> *>(container)->subtract(*reinterpret_cast<QSet<T> *>(ptr));
        }else{
            QTJAMBI_ELEMENT_LOCKER
            QSet<T> list;
            jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, other);
            while(QtJambiAPI::hasJavaIteratorNext(env, iterator)){
                jvalue _value;
                _value.l = QtJambiAPI::nextOfJavaIterator(env, iterator);
                T _qvalue;
                void *_qvaluePtr = &_qvalue;
                if(m_externalToInternalConverter(env, nullptr, _value, _qvaluePtr, jValueType::l))
                    list << _qvalue;
            }
            reinterpret_cast<QSet<T> *>(container)->subtract(list);
        }
    }

    jboolean intersects(JNIEnv * env, const void* container, jobject other) override {
        void* ptr{nullptr};
        if (ContainerAPI::getAsQSet(env, other, elementMetaType(), ptr)) {
            QTJAMBI_ELEMENT_LOCKER
                bool intersects = reinterpret_cast<const QSet<T> *>(container)->intersects(*reinterpret_cast<const QSet<T> *>(ptr));
            return intersects;
        }else{
            QTJAMBI_ELEMENT_LOCKER
            QSet<T> list;
            jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, other);
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

    jobject values(JNIEnv * env, const void* container) override {
        QTJAMBI_ELEMENT_LOCKER
        jobject result = nullptr;
        AbstractContainerAccess* access = AbstractContainerAccess::create(env, SequentialContainerType::QList, m_elementMetaTypeInfo.metaType());
        if(!access)
            access = AbstractContainerAccess::create(env, SequentialContainerType::QList,
                                                      m_elementMetaTypeInfo.metaType(),
                                                      _align, _size,
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                                                      AbstractContainerAccess::isStaticType(m_elementMetaTypeInfo.metaType()),
#endif
                                                      AbstractContainerAccess::isPointerType(m_elementMetaTypeInfo.metaType()),
                                                      m_elementMetaTypeInfo.hashFunction(),
                                                      m_internalToExternalConverter,
                                                      m_externalToInternalConverter
                                                    );
        if(access){
            const void* qresult = new QList<T>(reinterpret_cast<const QSet<T> *>(container)->values());
            result = objectFromQList(env, qresult, access);
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
        void* ptr{nullptr};
        if (ContainerAPI::getAsQSet(env, other, elementMetaType(), ptr)) {
            QTJAMBI_ELEMENT_LOCKER
            bool equals = *reinterpret_cast<const QSet<T> *>(container)==*reinterpret_cast<QSet<T> *>(ptr);
            return equals;
        }else{
            QTJAMBI_ELEMENT_LOCKER
            QSet<T> list;
            jobject iterator = QtJambiAPI::iteratorOfJavaCollection(env, other);
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
struct SequentialContainerAccessFac<QSet,align,size,isStatic>{
    static constexpr SequentialContainerAccessFactory factory(){
        return SequentialContainerAccessFactory(GenericSetAccess<align,size>::newInstance);
    }
};

}
#endif // CONTAINERACCESS_SET_H
