/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
    JBufferConstData(JNIEnv *env, jobject buffer_object);

    ~JBufferConstData();

    qsizetype size() const;
    qsizetype capacity() const;
    qsizetype position() const;
    qsizetype limit() const;
    inline bool isDirect() const {return m_isdirect;}
    inline bool isReadOnly() const {return m_readOnly;}

public:
    const void* data() const;
    const void* constData() const;

    template<class T>
    inline qsizetype size() const {return size()*sizeof(char)/sizeof(T);}
    template<class T>
    inline qsizetype capacity() const {return capacity()*sizeof(char)/sizeof(T);}
    template<class T>
    inline qsizetype position() const {return position()*sizeof(char)/sizeof(T);}
    template<class T>
    inline qsizetype limit() const {return limit()*sizeof(char)/sizeof(T);}
    template<class T>
    inline const T* data() const {return reinterpret_cast<const T*>(data());}
    template<class T>
    inline const T* constData() const {return reinterpret_cast<const T*>(data());}
    template<class T>
    inline operator const T*() const {return data<T>();}

    void* take();

    static bool isBuffer(JNIEnv *env, jobject obj);
    static bool isReadOnlyBuffer(JNIEnv *env, jobject buffer);
protected:

    jobject m_buffer_object;
    qsizetype m_capacity;
    qsizetype m_position;
    qsizetype m_limit;
    char* m_data;
    bool m_isdirect;
    bool m_readOnly;
    Q_DISABLE_COPY(JBufferConstData)
};

class QTJAMBI_EXPORT JBufferData: public JBufferConstData
{
public:
    JBufferData(JNIEnv *env, jobject buffer_object);

    ~JBufferData();

    void* data();

    template<class T>
    inline T* data() {return reinterpret_cast<T*>(data());}
    template<class T>
    inline operator T*() {return data<T>();}
private:
    Q_DISABLE_COPY(JBufferData)
};

class QTJAMBI_EXPORT DataJBuffer{
public:
    DataJBuffer(JNIEnv *env, void* data, qsizetype capacity);
    DataJBuffer(JNIEnv *env, const void* data, qsizetype capacity);
    DataJBuffer(JNIEnv *env, char* data, qsizetype capacity);
    DataJBuffer(JNIEnv *env, const char* data, qsizetype capacity);
    DataJBuffer(JNIEnv *env, unsigned char* data, qsizetype capacity);
    DataJBuffer(JNIEnv *env, const unsigned char* data, qsizetype capacity);
    DataJBuffer(JNIEnv *env, qint16* data, qsizetype capacity);
    DataJBuffer(JNIEnv *env, const qint16* data, qsizetype capacity);
    DataJBuffer(JNIEnv *env, quint16* data, qsizetype capacity);
    DataJBuffer(JNIEnv *env, const quint16* data, qsizetype capacity);
    DataJBuffer(JNIEnv *env, qint32* data, qsizetype capacity);
    DataJBuffer(JNIEnv *env, const qint32* data, qsizetype capacity);
    DataJBuffer(JNIEnv *env, quint32* data, qsizetype capacity);
    DataJBuffer(JNIEnv *env, const quint32* data, qsizetype capacity);
    DataJBuffer(JNIEnv *env, qint64* data, qsizetype capacity);
    DataJBuffer(JNIEnv *env, const qint64* data, qsizetype capacity);
    DataJBuffer(JNIEnv *env, quint64* data, qsizetype capacity);
    DataJBuffer(JNIEnv *env, const quint64* data, qsizetype capacity);
    DataJBuffer(JNIEnv *env, float* data, qsizetype capacity);
    DataJBuffer(JNIEnv *env, const float* data, qsizetype capacity);
    DataJBuffer(JNIEnv *env, double* data, qsizetype capacity);
    DataJBuffer(JNIEnv *env, const double* data, qsizetype capacity);
    DataJBuffer(JNIEnv *env, QChar* data, qsizetype capacity);
    DataJBuffer(JNIEnv *env, const QChar* data, qsizetype capacity);
    ~DataJBuffer();
    void setLimit(jsize limit);
    static void setLimit(JNIEnv *env, jobject buffer, jsize limit);
    jobject buffer() const;
    jobject take();
private:
    JNIEnv *m_env;
    jobject m_buffer_object;
};

#endif // QTJAMBI_JAVABUFFERS_H
