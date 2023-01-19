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

#if !defined(QTJAMBI_JAVABUFFERS_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBI_JAVABUFFERS_H

#include "global.h"

class QTJAMBI_EXPORT JBufferConstData
{
public:
    JBufferConstData(JNIEnv *env, jobject buffer_object, bool purgeOnDelete = true);

    ~JBufferConstData();

    const void* data() const;

    const void* constData() const;

    qint64 size() const;

    inline operator const void*() const {return constData();}

    template<class T>
    inline operator const T*() const {return reinterpret_cast<const T*>(constData());}
protected:
    jobject m_buffer_object;
    qint64 m_size;
    void* m_data;
    bool m_purgeOnDelete;
    bool m_isdirect;
    Q_DISABLE_COPY(JBufferConstData)
};

class QTJAMBI_EXPORT JBufferData: public JBufferConstData
{
public:
    JBufferData(JNIEnv *env, jobject buffer_object);

    ~JBufferData();

    void* data();

    inline operator void*(){return data();}

    template<class T>
    operator T*(){return reinterpret_cast<T*>(data());}
private:
    Q_DISABLE_COPY(JBufferData)
};

#endif // QTJAMBI_JAVABUFFERS_H
