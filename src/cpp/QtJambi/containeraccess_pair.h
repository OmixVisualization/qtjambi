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


#ifndef CONTAINERACCESS_PAIR_H
#define CONTAINERACCESS_PAIR_H

#include <QtCore/QPair>
#include "containeraccess.h"

#if defined(QTJAMBI_GENERIC_ACCESS)

namespace ContainerAccessAPI {

template<size_t align1, size_t size1, size_t align2, size_t size2>
class GenericPairAccess : public AbstractPairAccess, public AbstractNestedPairAccess {
    typedef typename std::conditional<size1==0, void*, ContainerElement<size1, 0, false, align1>>::type K;
    typedef typename std::conditional<size2==0, void*, ContainerElement<size2, 1, false, align2>>::type T;
    MetaTypeInfo<0,size1==0> m_keyMetaTypeInfo;
    QtJambiUtils::InternalToExternalConverter m_keyInternalToExternalConverter;
    QtJambiUtils::ExternalToInternalConverter m_keyExternalToInternalConverter;
    QSharedPointer<AbstractContainerAccess> m_keyNestedContainerAccess;
    MetaTypeInfo<1,size2==0> m_valueMetaTypeInfo;
    QtJambiUtils::InternalToExternalConverter m_valueInternalToExternalConverter;
    QtJambiUtils::ExternalToInternalConverter m_valueExternalToInternalConverter;
    QSharedPointer<AbstractContainerAccess> m_valueNestedContainerAccess;
    AbstractContainerAccess::DataType m_keyDataType;
    AbstractContainerAccess::DataType m_valueDataType;
    GenericPairAccess(
            const QMetaType& keyMetaType,
            const QtJambiUtils::QHashFunction& keyHashFunction,
            const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
            const QtJambiUtils::ExternalToInternalConverter& keyExternalToInternalConverter,
            const QSharedPointer<AbstractContainerAccess>& keyNestedContainerAccess,
            const QMetaType& valueMetaType,
            const QtJambiUtils::QHashFunction& valueHashFunction,
            const QtJambiUtils::InternalToExternalConverter& valueInternalToExternalConverter,
            const QtJambiUtils::ExternalToInternalConverter& valueExternalToInternalConverter,
            const QSharedPointer<AbstractContainerAccess>& valueNestedContainerAccess
            )
        :   AbstractPairAccess(), AbstractNestedPairAccess(),
          m_keyMetaTypeInfo(keyMetaType, keyHashFunction),
          m_keyInternalToExternalConverter(keyInternalToExternalConverter),
          m_keyExternalToInternalConverter(keyExternalToInternalConverter),
          m_keyNestedContainerAccess(keyNestedContainerAccess),
          m_valueMetaTypeInfo(valueMetaType, valueHashFunction),
          m_valueInternalToExternalConverter(valueInternalToExternalConverter),
          m_valueExternalToInternalConverter(valueExternalToInternalConverter),
          m_valueNestedContainerAccess(valueNestedContainerAccess),
          m_keyDataType(dataType(keyMetaType, m_keyNestedContainerAccess)),
          m_valueDataType(dataType(valueMetaType, m_valueNestedContainerAccess))
    {
    }

    GenericPairAccess(const GenericPairAccess<align1, size1, align2, size2>& other)
        :   AbstractPairAccess(), AbstractNestedPairAccess(),
          m_keyMetaTypeInfo(other.m_keyMetaTypeInfo),
          m_keyInternalToExternalConverter(other.m_keyInternalToExternalConverter),
          m_keyExternalToInternalConverter(other.m_keyExternalToInternalConverter),
          m_keyNestedContainerAccess(other.m_keyNestedContainerAccess),
          m_valueMetaTypeInfo(other.m_valueMetaTypeInfo),
          m_valueInternalToExternalConverter(other.m_valueInternalToExternalConverter),
          m_valueExternalToInternalConverter(other.m_valueExternalToInternalConverter),
          m_valueNestedContainerAccess(other.m_valueNestedContainerAccess),
          m_keyDataType(other.m_keyDataType),
          m_valueDataType(other.m_valueDataType)
    {
    }

public:
    static AbstractPairAccess* newInstance(
                                          const QMetaType& keyMetaType,
                                          const QtJambiUtils::QHashFunction& keyHashFunction,
                                          const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
                                          const QtJambiUtils::ExternalToInternalConverter& keyExternalToInternalConverter,
                                          const QSharedPointer<AbstractContainerAccess>& keyNestedContainerAccess,
                                          const QMetaType& valueMetaType,
                                          const QtJambiUtils::QHashFunction& valueHashFunction,
                                          const QtJambiUtils::InternalToExternalConverter& valueInternalToExternalConverter,
                                          const QtJambiUtils::ExternalToInternalConverter& valueExternalToInternalConverter,
                                          const QSharedPointer<AbstractContainerAccess>& valueNestedContainerAccess){
        return new GenericPairAccess(
                                    keyMetaType,
                                    keyHashFunction,
                                    keyInternalToExternalConverter,
                                    keyExternalToInternalConverter,
                                    keyNestedContainerAccess,
                                    valueMetaType,
                                    valueHashFunction,
                                    valueInternalToExternalConverter,
                                    valueExternalToInternalConverter,
                                    valueNestedContainerAccess);
    }

    void dispose() override { delete this; }

    GenericPairAccess<align1, size1, align2, size2>* clone() override{
        return new GenericPairAccess<align1, size1, align2, size2>(*this);
    }

    size_t sizeOf() override {
        return sizeof(QPair<K,T>);
    }
    void* constructContainer(void* placement) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        return new(placement) QPair<K,T>();
    }
    void* constructContainer(void* placement, const void* copyOf) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        return new(placement) QPair<K,T>(*reinterpret_cast<const QPair<K,T>*>(copyOf));
    }
    void* constructContainer(JNIEnv *, void* placement, const ConstContainerAndAccessInfo& copyOf) override {
        return constructContainer(placement, copyOf.container);
    }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void* constructContainer(void* placement, void* move) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        return new(placement) QPair<K,T>(std::move(*reinterpret_cast<const QPair<K,T>*>(move)));
    }
    void* constructContainer(JNIEnv *, void* placement, const ContainerAndAccessInfo& move) override {
        return constructContainer(placement, move.container);
    }
#endif
    bool destructContainer(void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        reinterpret_cast<QPair<K,T>*>(container)->~QPair<K,T>();
        return true;
    }
    void assign(JNIEnv *, const ContainerInfo& container, const ConstContainerAndAccessInfo& other) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        (*reinterpret_cast<QPair<K,T>*>(container.container)) = (*reinterpret_cast<const QPair<K,T>*>(other.container));
    }
    void assign(void* container, const void* other) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
            (*reinterpret_cast<QPair<K,T>*>(container)) = (*reinterpret_cast<const QPair<K,T>*>(other));
    }
    int registerContainer(const QByteArray& containerTypeName) override {
        return QtJambiPrivate::registerQPairType<QPair<K,T>, size1, size2>(containerTypeName, m_keyMetaTypeInfo.metaType(), m_valueMetaTypeInfo.metaType(), this);
    }

    jobject first(JNIEnv * env, const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        jobject result = nullptr;
        {
            jvalue _first;
            _first.l = nullptr;
            if(m_keyInternalToExternalConverter(env, nullptr, &reinterpret_cast<const QPair<K,T> *>(container)->first, _first, true)){
                result = _first.l;
            }
        }
        return result;
    }

    void setFirst(JNIEnv *env, void* container, jobject first) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        {
            jvalue jv;
            jv.l = first;
            void *_qkeyPtr = &reinterpret_cast<QPair<K,T> *>(container)->first;
            m_keyExternalToInternalConverter(env, nullptr, jv, _qkeyPtr, jValueType::l);
        }
    }

    jobject second(JNIEnv * env, const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        jobject result = nullptr;
        {
            jvalue _value;
            _value.l = nullptr;
            if(m_valueInternalToExternalConverter(env, nullptr, &reinterpret_cast<const QPair<K,T> *>(container)->second, _value, true)){
                result = _value.l;
            }
        }
        return result;
    }

    void setSecond(JNIEnv *env, void* container, jobject second) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        {
            jvalue jv;
            jv.l = second;
            void *_qsecondPtr = &reinterpret_cast<QPair<K,T> *>(container)->second;
            m_valueExternalToInternalConverter(env, nullptr, jv, _qsecondPtr, jValueType::l);
        }
    }

    QPair<const void*,const void*> elements(const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER(this);
        const void* key = m_keyDataType==AbstractContainerAccess::Value ? nullptr :
                              QtJambiPrivate::ContainerContentDeref<K, size1==0>::deref(reinterpret_cast<const QPair<K,T> *>(container)->first);
        const void* value = m_valueDataType==AbstractContainerAccess::Value ? nullptr :
                                QtJambiPrivate::ContainerContentDeref<T, size2==0>::deref(reinterpret_cast<const QPair<K,T> *>(container)->second);
        return {key, value};
    }

    const QMetaType& firstMetaType() override {return m_keyMetaTypeInfo.metaType();}
    const QMetaType& secondMetaType() override {return m_valueMetaTypeInfo.metaType();}

    DataType firstType() override{
        return m_keyDataType;
    }

    DataType secondType() override{
        return m_valueDataType;
    }

    AbstractContainerAccess* firstNestedContainerAccess() override {
        return m_keyNestedContainerAccess ? m_keyNestedContainerAccess->clone() : nullptr;
    }

    AbstractContainerAccess* secondNestedContainerAccess() override {
        return m_valueNestedContainerAccess ? m_valueNestedContainerAccess->clone() : nullptr;
    }

    const QSharedPointer<AbstractContainerAccess>& sharedFirstNestedContainerAccess() override {
        return m_keyNestedContainerAccess;
    }
    const QSharedPointer<AbstractContainerAccess>& sharedSecondNestedContainerAccess() override {
        return m_valueNestedContainerAccess;
    }
    bool hasFirstNestedContainerAccess() override {
        return !m_keyNestedContainerAccess.isNull();
    }
    bool hasFirstNestedPointers() override {
        if(hasFirstNestedContainerAccess()){
            if(auto daccess = dynamic_cast<AbstractSequentialAccess*>(m_keyNestedContainerAccess.data())){
                return (daccess->elementType() & PointersMask) || daccess->hasNestedPointers();
            }else if(auto daccess = dynamic_cast<AbstractAssociativeAccess*>(m_keyNestedContainerAccess.data())){
                return (daccess->keyType() & PointersMask) || daccess->hasKeyNestedPointers() || (daccess->valueType() & PointersMask) || daccess->hasValueNestedPointers();
            }else if(auto daccess = dynamic_cast<AbstractPairAccess*>(m_keyNestedContainerAccess.data())){
                return (daccess->firstType() & PointersMask) || daccess->hasFirstNestedPointers() || (daccess->secondType() & PointersMask) || daccess->hasSecondNestedPointers();
            }
        }
        return false;
    }
    bool hasSecondNestedContainerAccess() override {
        return !m_valueNestedContainerAccess.isNull();
    }
    bool hasSecondNestedPointers() override {
        if(hasSecondNestedContainerAccess()){
            if(auto daccess = dynamic_cast<AbstractSequentialAccess*>(m_valueNestedContainerAccess.data())){
                return (daccess->elementType() & PointersMask) || daccess->hasNestedPointers();
            }else if(auto daccess = dynamic_cast<AbstractAssociativeAccess*>(m_valueNestedContainerAccess.data())){
                return (daccess->keyType() & PointersMask) || daccess->hasKeyNestedPointers() || (daccess->valueType() & PointersMask) || daccess->hasValueNestedPointers();
            }else if(auto daccess = dynamic_cast<AbstractPairAccess*>(m_valueNestedContainerAccess.data())){
                return (daccess->firstType() & PointersMask) || daccess->hasFirstNestedPointers() || (daccess->secondType() & PointersMask) || daccess->hasSecondNestedPointers();
            }
        }
        return false;
    }
};

template<size_t align1, size_t size1, size_t align2, size_t size2>
struct AssociativeContainerAccessFac<QPair,align1,size1,align2,size2>{
    static constexpr AssociativeContainerAccessFactory factory(){
        return AssociativeContainerAccessFactory(GenericPairAccess<align1,size1,align2,size2>::newInstance);
    }
};

}

#endif //defined(QTJAMBI_GENERIC_ACCESS)

#endif // CONTAINERACCESS_PAIR_H
