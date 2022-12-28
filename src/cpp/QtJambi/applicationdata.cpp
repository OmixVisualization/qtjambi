/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
#include "coreapi.h"

ApplicationData::ApplicationData(JNIEnv *env, jobjectArray array)
    : QtJambiObjectData(), m_size(int(env->GetArrayLength(array))), m_chars(m_size>0 ? new char*[size_t(m_size)] : nullptr)
{
    for(jsize i=0; i<m_size; ++i){
        jstring strg = jstring(env->GetObjectArrayElement(array, i));
        if(strg){
            jsize length = env->GetStringUTFLength(strg);
            if(length>=0){
                m_chars[i] = new char[size_t(length+1)];
                m_chars[i][length] = 0;
                env->GetStringUTFRegion(strg, 0, length, m_chars[i]);
            }else{
                m_chars[i] = nullptr;
            }
        }else{
            m_chars[i] = new char[1];
            m_chars[i][0] = 0;
        }
    }
}

ApplicationData::~ApplicationData(){
    for(jsize i=0; i<m_size; ++i){
        delete[] m_chars[i];
    }
    delete[] m_chars;
}

char** ApplicationData::chars(){
    return m_chars;
}

int& ApplicationData::size(){
    return m_size;
}

QTJAMBI_OBJECTUSERDATA_ID_IMPL(,ApplicationData::)
