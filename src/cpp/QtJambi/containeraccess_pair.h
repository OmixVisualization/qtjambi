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


#ifndef CONTAINERACCESS_PAIR_H
#define CONTAINERACCESS_PAIR_H

#include <QtCore/QPair>
#include "containeraccess.h"

namespace ContainerAccessAPI {

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
        return QtJambiPrivate::registerQPairType<QPair<K,T>, size1, size2>(containerTypeName, m_keyMetaTypeInfo.metaType(), m_valueMetaTypeInfo.metaType());
    }
    bool isConstant() override {return false;}

    jobject first(JNIEnv * env, const void* container) override {
        QTJAMBI_KEY_VALUE_LOCKER
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
            if(m_valueInternalToExternalConverter(env, nullptr, &reinterpret_cast<const QPair<K,T> *>(container)->second, _value, true)){
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
struct AssociativeContainerAccessFac<QPair,align1,size1,align2,size2>{
    static constexpr AssociativeContainerAccessFactory factory(){
        return AssociativeContainerAccessFactory(GenericPairAccess<align1,size1,align2,size2>::newInstance);
    }
};

}

#endif // CONTAINERACCESS_PAIR_H
