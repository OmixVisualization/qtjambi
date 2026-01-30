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

J2CStringBuffer::J2CStringBuffer(JNIEnv* env, jstring strg)
    : m_strg(strg),
    m_length(strg ? env->GetStringUTFLength(strg) : 0),
    m_data(strg ? env->GetStringUTFChars(strg, nullptr) : nullptr),
    m_env(env){
}

J2CStringBuffer::~J2CStringBuffer(){
    if(m_strg){
        m_env->ReleaseStringUTFChars(m_strg, m_data);
    }
}

const char* J2CStringBuffer::constData() const {return m_data;}

int J2CStringBuffer::length() const {return m_length;}

QByteArray J2CStringBuffer::toByteArray() const {
    return QByteArray(constData(), length());
}

struct J2CStringBufferPrivate{
    JObjectWrapper m_strg;
    const jsize m_length;
    const char* m_data;
};

PersistentJ2CStringBuffer::PersistentJ2CStringBuffer(JNIEnv* env, jstring strg)
    : m_data(new J2CStringBufferPrivate{JObjectWrapper(env, strg), strg ? env->GetStringUTFLength(strg) : 0, strg ? env->GetStringUTFChars(strg, nullptr) : nullptr}){
}

PersistentJ2CStringBuffer::~PersistentJ2CStringBuffer(){
    if(Q_LIKELY(m_data)){
        if(m_data->m_strg){
            if(JniEnvironment env{24}){
                env->ReleaseStringUTFChars(m_data->m_strg.typedObject<jstring>(env), m_data->m_data);
                m_data->m_strg.clear(env);
            }
        }
    }
}

void PersistentJ2CStringBuffer::clear(JNIEnv* env){
    if(Q_LIKELY(m_data)){
        if(m_data->m_strg){
            env->ReleaseStringUTFChars(m_data->m_strg.typedObject<jstring>(env), m_data->m_data);
            m_data->m_strg.clear(env);
        }
    }
}

const char* PersistentJ2CStringBuffer::constData() const {return m_data ? m_data->m_data : nullptr;}

int PersistentJ2CStringBuffer::length() const {return m_data ? m_data->m_length : 0;}

QByteArray PersistentJ2CStringBuffer::toByteArray() const {
    return QByteArray(constData(), length());
}

JString2QChars::JString2QChars(JNIEnv* env, jstring strg)
    : m_strg(strg),
    m_length(strg ? env->GetStringLength(strg) : 0),
    m_data(strg ? env->GetStringChars(strg, nullptr) : nullptr),
    m_env(env) {
}

JString2QChars::~JString2QChars(){
    if(m_strg){
        m_env->ReleaseStringChars(m_strg, m_data);
    }
}

QString JString2QChars::toString() const {
    return QString(constData(), length());
}

const QChar* JString2QChars::constData() const {return reinterpret_cast<const QChar*>(m_data);}

JString2QChars::operator const char16_t*() const {return reinterpret_cast<const char16_t*>(m_data);}

int JString2QChars::length() const {return m_length;}

struct JString2QCharsPrivate{
    JObjectWrapper m_strg;
    const jsize m_length;
    const jchar* m_data;
};

PersistentJString2QChars::PersistentJString2QChars(JNIEnv* env, jstring strg)
    : m_data(new JString2QCharsPrivate{JObjectWrapper(env, strg), strg ? env->GetStringLength(strg) : 0, strg ? env->GetStringChars(strg, nullptr) : nullptr})
{
}

PersistentJString2QChars::~PersistentJString2QChars(){
    if(m_data && m_data->m_strg){
        if(JniEnvironment env{24}){
            env->ReleaseStringChars(m_data->m_strg.typedObject<jstring>(env), m_data->m_data);
            m_data->m_strg.clear(env);
        }
    }
}

void PersistentJString2QChars::clear(JNIEnv* env){
    if(m_data && m_data->m_strg){
        env->ReleaseStringChars(m_data->m_strg.typedObject<jstring>(env), m_data->m_data);
        m_data->m_strg.clear(env);
    }
}

QString PersistentJString2QChars::toString() const {
    return QString(constData(), length());
}

const QChar* PersistentJString2QChars::constData() const {return m_data ? reinterpret_cast<const QChar*>(m_data->m_data) : nullptr;}

PersistentJString2QChars::operator const char16_t*() const {return m_data ? reinterpret_cast<const char16_t*>(m_data->m_data) : nullptr;}

int PersistentJString2QChars::length() const {return m_data ? m_data->m_length : 0;}
