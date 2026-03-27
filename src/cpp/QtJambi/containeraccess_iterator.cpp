/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
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
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "pch_p.h"

QT_WARNING_DISABLE_GCC("-Winaccessible-base")
QT_WARNING_DISABLE_CLANG("-Winaccessible-base")

template<typename Super>
AutoSequentialConstIteratorAccess<Super>::~AutoSequentialConstIteratorAccess() = default;

AutoSequentialConstIteratorAccess<AbstractSequentialConstIteratorAccess>* createAutoSequentialConstIteratorAccess(
        const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
        AutoSequentialConstIteratorAccess<AbstractSequentialConstIteratorAccess>::IncrementFn increment,
        AutoSequentialConstIteratorAccess<AbstractSequentialConstIteratorAccess>::DecrementFn decrement,
        AutoSequentialConstIteratorAccess<AbstractSequentialConstIteratorAccess>::ValueFn value,
        AutoSequentialConstIteratorAccess<AbstractSequentialConstIteratorAccess>::LessThanFn lessThan,
        AutoSequentialConstIteratorAccess<AbstractSequentialConstIteratorAccess>::EqualsFn equals,
        const QMetaType& valueMetaType,
        size_t offset
    ){
    return new AutoSequentialConstIteratorAccess<AbstractSequentialConstIteratorAccess>(internalToExternalConverter, increment, decrement, value, lessThan, equals, valueMetaType, offset);
}

template<typename Super>
AutoSequentialConstIteratorAccess<Super>::AutoSequentialConstIteratorAccess(
        const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
        IncrementFn increment,
        DecrementFn decrement,
        ValueFn value,
        LessThanFn lessThan,
        EqualsFn equals,
        const QMetaType& valueMetaType,
        size_t offset
    )
    : Super(),
      m_internalToExternalConverter(internalToExternalConverter),
      m_increment(increment),
      m_decrement(decrement),
      m_value(value),
      m_lessThan(lessThan),
      m_equals(equals),
      m_valueMetaType(valueMetaType),
      m_offset(offset)
{
    Q_ASSERT(m_value);
}

template<typename Super>
void AutoSequentialConstIteratorAccess<Super>::dispose() {delete this;}

template<typename Super>
AutoSequentialConstIteratorAccess<Super>* AutoSequentialConstIteratorAccess<Super>::clone()
{
    if constexpr(std::is_same_v<Super,AbstractSequentialConstIteratorAccess>){
        return new AutoSequentialConstIteratorAccess<Super>(
                    m_internalToExternalConverter,
                    m_increment,
                    m_decrement,
                    m_value,
                    m_lessThan,
                    m_equals,
                    m_valueMetaType,
                    m_offset);
    }else return nullptr;
}

template<typename Super>
jobject AutoSequentialConstIteratorAccess<Super>::value(JNIEnv * env, const void* iterator)
{
    const void* v = m_value(this, iterator);
    jvalue jval;
    jval.l = nullptr;
    if(m_internalToExternalConverter(env, nullptr, v, jval, true))
        return jval.l;
    return nullptr;
}

template<typename Super>
void AutoSequentialConstIteratorAccess<Super>::increment(JNIEnv *, void* iterator)
{
    m_increment(this, iterator);
}

template<typename Super>
void AutoSequentialConstIteratorAccess<Super>::decrement(JNIEnv *, void* iterator)
{
    m_decrement(this, iterator);
}

template<typename Super>
jboolean AutoSequentialConstIteratorAccess<Super>::lessThan(JNIEnv *, const void* iterator, const void* other)
{
    return m_lessThan(this, iterator, other);
}

template<typename Super>
bool AutoSequentialConstIteratorAccess<Super>::canLess()
{
    if(m_lessThan)
        return true;
    else return false;
}

template<typename Super>
jboolean AutoSequentialConstIteratorAccess<Super>::equals(JNIEnv *, const void* iterator, const void* other)
{
    return m_equals(this, iterator, other);
}

template<typename Super>
const QMetaType& AutoSequentialConstIteratorAccess<Super>::valueMetaType() {
    return m_valueMetaType;
}

template<typename Super>
AutoAssociativeConstIteratorAccess<Super>::~AutoAssociativeConstIteratorAccess(){}

template<typename Super>
AutoAssociativeConstIteratorAccess<Super>::AutoAssociativeConstIteratorAccess(
        const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
        IncrementFn increment,
        DecrementFn decrement,
        ValueFn value,
        LessThanFn lessThan,
        EqualsFn equals,
        const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
        KeyFn key,
        const QMetaType& keyMetaType,
        const QMetaType& valueMetaType,
        size_t keyOffset,
        size_t valueOffset
        )
    : AutoSequentialConstIteratorAccess<Super>(internalToExternalConverter,
                                        typename AutoSequentialConstIteratorAccess<Super>::IncrementFn(increment),
                                        typename AutoSequentialConstIteratorAccess<Super>::DecrementFn(decrement),
                                        typename AutoSequentialConstIteratorAccess<Super>::ValueFn(value),
                                        typename AutoSequentialConstIteratorAccess<Super>::LessThanFn(lessThan),
                                        typename AutoSequentialConstIteratorAccess<Super>::EqualsFn(equals),
                        valueMetaType, valueOffset),
      m_keyInternalToExternalConverter(keyInternalToExternalConverter),
      m_key(std::move(key)),
      m_keyMetaType(keyMetaType),
      m_keyOffset(keyOffset)
{
    Q_ASSERT(m_key);
}

template<typename Super>
AutoAssociativeConstIteratorAccess<Super>* AutoAssociativeConstIteratorAccess<Super>::clone(){
    if constexpr(std::is_same_v<Super,AbstractAssociativeConstIteratorAccess>){
        return new AutoAssociativeConstIteratorAccess<Super>(
                    this->m_internalToExternalConverter,
                    IncrementFn(this->m_increment),
                    DecrementFn(this->m_decrement),
                    ValueFn(this->m_value),
                    LessThanFn(this->m_lessThan),
                    EqualsFn(this->m_equals),
                    m_keyInternalToExternalConverter,
                    m_key,
                    m_keyMetaType,
                    this->m_valueMetaType,
                    m_keyOffset,
                    this->m_offset);
    }else return nullptr;
}

AutoAssociativeConstIteratorAccess<AbstractAssociativeConstIteratorAccess>* createAutoAssociativeConstIteratorAccess(
    const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
    AutoAssociativeConstIteratorAccess<AbstractAssociativeConstIteratorAccess>::IncrementFn increment,
    AutoAssociativeConstIteratorAccess<AbstractAssociativeConstIteratorAccess>::DecrementFn decrement,
    AutoAssociativeConstIteratorAccess<AbstractAssociativeConstIteratorAccess>::ValueFn value,
    AutoAssociativeConstIteratorAccess<AbstractAssociativeConstIteratorAccess>::LessThanFn lessThan,
    AutoAssociativeConstIteratorAccess<AbstractAssociativeConstIteratorAccess>::EqualsFn equals,
    const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
    AutoAssociativeConstIteratorAccess<AbstractAssociativeConstIteratorAccess>::KeyFn key,
    const QMetaType& keyMetaType,
    const QMetaType& valueMetaType,
    size_t keyOffset,
    size_t valueOffset
    ){
    return new AutoAssociativeConstIteratorAccess<AbstractAssociativeConstIteratorAccess>(
        internalToExternalConverter,
        increment,
        decrement,
        value,
        lessThan,
        equals,
        keyInternalToExternalConverter,
        key,
        keyMetaType,
        valueMetaType,
        keyOffset,
        valueOffset);
}

template<typename Super>
jobject AutoAssociativeConstIteratorAccess<Super>::key(JNIEnv * env, const void* iterator){
    const void* v = m_key(this, iterator);
    jvalue jval;
    jval.l = nullptr;
    if(m_keyInternalToExternalConverter(env, nullptr, v, jval, true))
        return jval.l;
    return nullptr;
}

template<typename Super>
const QMetaType& AutoAssociativeConstIteratorAccess<Super>::keyMetaType() {
    return m_keyMetaType;
}

AutoSequentialIteratorAccess::~AutoSequentialIteratorAccess() = default;
AutoSequentialIteratorAccess::AutoSequentialIteratorAccess(
        const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
        IncrementFn increment,
        DecrementFn decrement,
        ValueFn value,
        LessThanFn lessThan,
        EqualsFn equals,
        const QtJambiUtils::ExternalToInternalConverter& externalToInternalConverter,
        SetValueFn setValue,
        const QMetaType& valueMetaType,
        size_t offset
    )
    : AutoSequentialConstIteratorAccess<AbstractSequentialIteratorAccess>(internalToExternalConverter,
                                        AutoSequentialConstIteratorAccess<AbstractSequentialIteratorAccess>::IncrementFn(increment),
                                        AutoSequentialConstIteratorAccess<AbstractSequentialIteratorAccess>::DecrementFn(decrement),
                                        AutoSequentialConstIteratorAccess<AbstractSequentialIteratorAccess>::ValueFn(value),
                                        AutoSequentialConstIteratorAccess<AbstractSequentialIteratorAccess>::LessThanFn(lessThan),
                                        AutoSequentialConstIteratorAccess<AbstractSequentialIteratorAccess>::EqualsFn(equals),
                                        valueMetaType,
                                        offset),
      m_externalToInternalConverter(externalToInternalConverter),
      m_setValue(setValue)
{
    Q_ASSERT(m_value);
    Q_ASSERT(m_setValue);
}

AutoSequentialIteratorAccess* AutoSequentialIteratorAccess::clone()
{
    return new AutoSequentialIteratorAccess(
                m_internalToExternalConverter,
                IncrementFn(m_increment),
                DecrementFn(m_decrement),
                ValueFn(m_value),
                LessThanFn(m_lessThan),
                EqualsFn(m_equals),
                m_externalToInternalConverter,
                m_setValue,
                m_valueMetaType, m_offset);
}

void AutoSequentialIteratorAccess::setValue(JNIEnv * env, void* iterator, jobject newValue){
    void* newval = m_setValue(this, iterator);
    jvalue jval;
    jval.l = newValue;
    m_externalToInternalConverter(env, nullptr, jval, newval, jValueType::l);
}

jobject AutoSequentialIteratorAccess::value(JNIEnv * env, const void* iterator){return AutoSequentialConstIteratorAccess::value(env, iterator);}
void AutoSequentialIteratorAccess::increment(JNIEnv * env, void* iterator){AutoSequentialConstIteratorAccess::increment(env, iterator);}
void AutoSequentialIteratorAccess::decrement(JNIEnv * env, void* iterator){AutoSequentialConstIteratorAccess::decrement(env, iterator);}
jboolean AutoSequentialIteratorAccess::lessThan(JNIEnv * env, const void* iterator, const void* other){return AutoSequentialConstIteratorAccess::lessThan(env, iterator, other);}
bool AutoSequentialIteratorAccess::canLess(){return AutoSequentialConstIteratorAccess::canLess();}
jboolean AutoSequentialIteratorAccess::equals(JNIEnv * env, const void* iterator, const void* other){return AutoSequentialConstIteratorAccess::equals(env, iterator, other);}
const QMetaType& AutoSequentialIteratorAccess::valueMetaType() {
    return AutoSequentialConstIteratorAccess::valueMetaType();
}


AutoAssociativeIteratorAccess::~AutoAssociativeIteratorAccess(){}
AutoAssociativeIteratorAccess::AutoAssociativeIteratorAccess(
        const QtJambiUtils::InternalToExternalConverter& valueInternalToExternalConverter,
        IncrementFn increment,
        DecrementFn decrement,
        ValueFn value,
        LessThanFn lessThan,
        EqualsFn equals,
        const QtJambiUtils::InternalToExternalConverter& keyInternalToExternalConverter,
        KeyFn key,
        const QtJambiUtils::ExternalToInternalConverter& valueExternalToInternalConverter,
        SetValueFn setValue,
        const QMetaType& keyMetaType,
        const QMetaType& valueMetaType,
        size_t keyOffset,
        size_t valueOffset
        )
    : AutoAssociativeConstIteratorAccess<AbstractAssociativeIteratorAccess>(valueInternalToExternalConverter,
                         AutoAssociativeConstIteratorAccess<AbstractAssociativeIteratorAccess>::IncrementFn(increment),
                         AutoAssociativeConstIteratorAccess<AbstractAssociativeIteratorAccess>::DecrementFn(decrement),
                         AutoAssociativeConstIteratorAccess<AbstractAssociativeIteratorAccess>::ValueFn(value),
                         AutoAssociativeConstIteratorAccess<AbstractAssociativeIteratorAccess>::LessThanFn(lessThan),
                         AutoAssociativeConstIteratorAccess<AbstractAssociativeIteratorAccess>::EqualsFn(equals),
                        keyInternalToExternalConverter,
                        AutoAssociativeConstIteratorAccess<AbstractAssociativeIteratorAccess>::KeyFn(key),
                        keyMetaType,
                        valueMetaType,
                        keyOffset,
                        valueOffset),
      m_valueExternalToInternalConverter(valueExternalToInternalConverter),
      m_setValue(setValue)
{
    Q_ASSERT(setValue);
}

AutoAssociativeIteratorAccess* AutoAssociativeIteratorAccess::clone(){
    return new AutoAssociativeIteratorAccess(
                m_internalToExternalConverter,
                IncrementFn(m_increment),
                DecrementFn(m_decrement),
                ValueFn(m_value),
                LessThanFn(m_lessThan),
                EqualsFn(m_equals),
                m_keyInternalToExternalConverter,
                KeyFn(m_key),
                m_valueExternalToInternalConverter,
                m_setValue,
                m_keyMetaType,
                m_valueMetaType,
                m_keyOffset,
                m_offset);
}

void AutoAssociativeIteratorAccess::setValue(JNIEnv * env, void* iterator, jobject newValue){
    void* newval = m_setValue(this, iterator);
    jvalue jval;
    jval.l = newValue;
    m_valueExternalToInternalConverter(env, nullptr, jval, newval, jValueType::l);
}

jobject AutoAssociativeIteratorAccess::value(JNIEnv * env, const void* iterator){return AutoSequentialConstIteratorAccess::value(env, iterator);}
void AutoAssociativeIteratorAccess::increment(JNIEnv * env, void* iterator){AutoSequentialConstIteratorAccess::increment(env, iterator);}
void AutoAssociativeIteratorAccess::decrement(JNIEnv * env, void* iterator){AutoSequentialConstIteratorAccess::decrement(env, iterator);}
jboolean AutoAssociativeIteratorAccess::lessThan(JNIEnv * env, const void* iterator, const void* other){return AutoSequentialConstIteratorAccess::lessThan(env, iterator, other);}
bool AutoAssociativeIteratorAccess::canLess(){return AutoSequentialConstIteratorAccess::canLess();}
jboolean AutoAssociativeIteratorAccess::equals(JNIEnv * env, const void* iterator, const void* other){return AutoSequentialConstIteratorAccess::equals(env, iterator, other);}
jobject AutoAssociativeIteratorAccess::key(JNIEnv * env, const void* iterator){return AutoAssociativeConstIteratorAccess::key(env, iterator);}
const QMetaType& AutoAssociativeIteratorAccess::keyMetaType() {return AutoAssociativeConstIteratorAccess::keyMetaType();}
const QMetaType& AutoAssociativeIteratorAccess::valueMetaType() {return AutoAssociativeConstIteratorAccess::valueMetaType();}
