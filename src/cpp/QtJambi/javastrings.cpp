/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "javastrings.h"
#include "jnienvironment.h"

J2CStringBuffer::J2CStringBuffer(JNIEnv* env, jstring strg)
    : m_strg(strg ? jstring(env->NewGlobalRef(strg)) : nullptr),
      m_length(strg ? env->GetStringUTFLength(strg) : 0),
      m_data(strg ? env->GetStringUTFChars(strg, nullptr) : nullptr) {
}

J2CStringBuffer::~J2CStringBuffer(){
    if(m_strg){
        if(JniEnvironment env{100}){
            env->ReleaseStringUTFChars(m_strg, m_data);
            env->DeleteGlobalRef(m_strg);
        }
    }
}

const char* J2CStringBuffer::constData() const {return m_data;}

int J2CStringBuffer::length() const {return m_length;}

QByteArray J2CStringBuffer::toByteArray() const {
    return QByteArray(constData(), length());
}

JString2QChars::JString2QChars(JNIEnv* env, jstring strg)
    : m_strg(strg ? jstring(env->NewGlobalRef(strg)) : nullptr),
      m_length(strg ? env->GetStringLength(strg) : 0),
      m_data(strg ? env->GetStringChars(strg, nullptr) : nullptr) {
}

JString2QChars::~JString2QChars(){
    if(m_strg){
        if(JniEnvironment env{100}){
            env->ReleaseStringChars(m_strg, m_data);
            env->DeleteGlobalRef(m_strg);
        }
    }
}

QString JString2QChars::toString() const {
    return QString(constData(), length());
}

const QChar* JString2QChars::constData() const {return reinterpret_cast<const QChar*>(m_data);}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
JString2QChars::operator const char16_t*() const {return reinterpret_cast<const char16_t*>(m_data);}
#endif

int JString2QChars::length() const {return m_length;}
