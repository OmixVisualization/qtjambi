/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "qtjambiapi.h"
#include <QtCore/QReadWriteLock>
#include <QtCore/QMap>
#include <QtCore/QSharedPointer>
#include "containeraccess_p.h"
#include "functionpointer.h"
#include "registryutil_p.h"

AutoSequentialConstIteratorAccess::~AutoSequentialConstIteratorAccess() = default;
AutoSequentialConstIteratorAccess::AutoSequentialConstIteratorAccess(
        const QtJambiUtils::InternalToExternalConverter& internalToExternalConverter,
        IncrementFn increment,
        DecrementFn decrement,
        ValueFn value,
        LessThanFn lessThan,
        EqualsFn equals,
        const QMetaType& valueMetaType,
        size_t offset
    )
    : AbstractSequentialConstIteratorAccess(),
      m_internalToExternalConverter(internalToExternalConverter),
      m_increment(increment),
      m_decrement(decrement),
      m_value(value),
      m_lessThan(lessThan),
      m_equals(equals),
      m_valueMetaType(valueMetaType
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                          .id()
#endif
                      ),
      m_offset(offset)
{
    Q_ASSERT(m_value);
}

void AutoSequentialConstIteratorAccess::dispose() {delete this;}

AbstractSequentialConstIteratorAccess* AutoSequentialConstIteratorAccess::clone()
{
    return new AutoSequentialConstIteratorAccess(
                m_internalToExternalConverter,
                m_increment,
                m_decrement,
                m_value,
                m_lessThan,
                m_equals,
                m_valueMetaType,
                m_offset);
}

jobject AutoSequentialConstIteratorAccess::value(JNIEnv * env, const void* iterator)
{
    const void* v = m_value(this, iterator);
    jvalue jval;
    jval.l = nullptr;
    if(m_internalToExternalConverter(env, nullptr, v, jval, true))
        return jval.l;
    return nullptr;
}

void AutoSequentialConstIteratorAccess::increment(JNIEnv *, void* iterator)
{
    m_increment(this, iterator);
}

void AutoSequentialConstIteratorAccess::decrement(JNIEnv *, void* iterator)
{
    m_decrement(this, iterator);
}

jboolean AutoSequentialConstIteratorAccess::lessThan(JNIEnv *, const void* iterator, const void* other)
{
    return m_lessThan(this, iterator, other);
}

bool AutoSequentialConstIteratorAccess::canLess()
{
    if(m_lessThan)
        return true;
    else return false;
}

jboolean AutoSequentialConstIteratorAccess::equals(JNIEnv *, const void* iterator, const void* other)
{
    return m_equals(this, iterator, other);
}

size_t AutoSequentialConstIteratorAccess::offset(){
    return m_offset;
}

const QMetaType& AutoSequentialConstIteratorAccess::valueMetaType() {
    return m_valueMetaType;
}

AutoAssociativeConstIteratorAccess::~AutoAssociativeConstIteratorAccess(){}
AutoAssociativeConstIteratorAccess::AutoAssociativeConstIteratorAccess(
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
    : AbstractAssociativeConstIteratorAccess(),
      AutoSequentialConstIteratorAccess(internalToExternalConverter,
                                        AutoSequentialConstIteratorAccess::IncrementFn(increment),
                                        AutoSequentialConstIteratorAccess::DecrementFn(decrement),
                                        AutoSequentialConstIteratorAccess::ValueFn(value),
                                        AutoSequentialConstIteratorAccess::LessThanFn(lessThan),
                                        AutoSequentialConstIteratorAccess::EqualsFn(equals),
                        valueMetaType, valueOffset),
      m_keyInternalToExternalConverter(keyInternalToExternalConverter),
      m_key(std::move(key)),
      m_keyMetaType(keyMetaType
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        .id()
#endif
                    ),
      m_keyOffset(keyOffset)
{
    Q_ASSERT(m_key);
}

AbstractSequentialConstIteratorAccess* AutoAssociativeConstIteratorAccess::clone(){
    AbstractAssociativeConstIteratorAccess* access = new AutoAssociativeConstIteratorAccess(
                m_internalToExternalConverter,
                IncrementFn(m_increment),
                DecrementFn(m_decrement),
                ValueFn(m_value),
                LessThanFn(m_lessThan),
                EqualsFn(m_equals),
                m_keyInternalToExternalConverter,
                m_key,
                m_keyMetaType,
                m_valueMetaType,
                m_keyOffset,
                m_offset);
    return access;
}

jobject AutoAssociativeConstIteratorAccess::key(JNIEnv * env, const void* iterator){
    const void* v = m_key(this, iterator);
    jvalue jval;
    jval.l = nullptr;
    if(m_keyInternalToExternalConverter(env, nullptr, v, jval, true))
        return jval.l;
    return nullptr;
}

const QMetaType& AutoAssociativeConstIteratorAccess::keyMetaType() {
    return m_keyMetaType;
}

size_t AutoAssociativeConstIteratorAccess::keyOffset(){
    return m_keyOffset;
}
size_t AutoAssociativeConstIteratorAccess::valueOffset(){
    return m_offset;
}

jobject AutoAssociativeConstIteratorAccess::value(JNIEnv * env, const void* iterator){return AutoSequentialConstIteratorAccess::value(env, iterator);}
const QMetaType& AutoAssociativeConstIteratorAccess::valueMetaType(){return AutoSequentialConstIteratorAccess::valueMetaType();}
void AutoAssociativeConstIteratorAccess::increment(JNIEnv * env, void* iterator){AutoSequentialConstIteratorAccess::increment(env, iterator);}
void AutoAssociativeConstIteratorAccess::decrement(JNIEnv * env, void* iterator){AutoSequentialConstIteratorAccess::decrement(env, iterator);}
jboolean AutoAssociativeConstIteratorAccess::lessThan(JNIEnv * env, const void* iterator, const void* other){return AutoSequentialConstIteratorAccess::lessThan(env, iterator, other);}
bool AutoAssociativeConstIteratorAccess::canLess(){return AutoSequentialConstIteratorAccess::canLess();}
jboolean AutoAssociativeConstIteratorAccess::equals(JNIEnv * env, const void* iterator, const void* other){return AutoSequentialConstIteratorAccess::equals(env, iterator, other);}

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
    : AbstractSequentialIteratorAccess(),
      AutoSequentialConstIteratorAccess(internalToExternalConverter,
                                        AutoSequentialConstIteratorAccess::IncrementFn(increment),
                                        AutoSequentialConstIteratorAccess::DecrementFn(decrement),
                                        AutoSequentialConstIteratorAccess::ValueFn(value),
                                        AutoSequentialConstIteratorAccess::LessThanFn(lessThan),
                                        AutoSequentialConstIteratorAccess::EqualsFn(equals),
                                        valueMetaType,
                                        offset),
      m_externalToInternalConverter(externalToInternalConverter),
      m_setValue(setValue)
{
    Q_ASSERT(m_value);
    Q_ASSERT(m_setValue);
}

AbstractSequentialConstIteratorAccess* AutoSequentialIteratorAccess::clone()
{
    AbstractSequentialIteratorAccess* access = new AutoSequentialIteratorAccess(
                m_internalToExternalConverter,
                IncrementFn(m_increment),
                DecrementFn(m_decrement),
                ValueFn(m_value),
                LessThanFn(m_lessThan),
                EqualsFn(m_equals),
                m_externalToInternalConverter,
                m_setValue,
                m_valueMetaType, m_offset);
    return access;
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
    : AbstractAssociativeIteratorAccess(),
      AutoAssociativeConstIteratorAccess(valueInternalToExternalConverter,
                         AutoAssociativeConstIteratorAccess::IncrementFn(increment),
                         AutoAssociativeConstIteratorAccess::DecrementFn(decrement),
                         AutoAssociativeConstIteratorAccess::ValueFn(value),
                         AutoAssociativeConstIteratorAccess::LessThanFn(lessThan),
                         AutoAssociativeConstIteratorAccess::EqualsFn(equals),
                        keyInternalToExternalConverter,
                        AutoAssociativeConstIteratorAccess::KeyFn(key),
                        keyMetaType,
                        valueMetaType,
                        keyOffset,
                        valueOffset),
      m_valueExternalToInternalConverter(valueExternalToInternalConverter),
      m_setValue(setValue)
{
    Q_ASSERT(setValue);
}

AbstractSequentialConstIteratorAccess* AutoAssociativeIteratorAccess::clone(){
    AbstractAssociativeIteratorAccess* access = new AutoAssociativeIteratorAccess(
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
    return access;
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
