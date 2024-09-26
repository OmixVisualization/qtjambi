﻿/****************************************************************************
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

namespace QtJambiAPI{
template<class Container>
Container createIterable(typename Container::const_iterator begin, typename Container::size_type size);
}

class QTJAMBI_EXPORT JBufferConstData
{
public:
    JBufferConstData(JNIEnv *env, jobject buffer_object);

    ~JBufferConstData();

    qsizetype size() const;
    qsizetype capacity() const;
    qsizetype position() const;
    qsizetype limit() const;
    bool isDirect() const;
    bool isReadOnly() const;
    bool isBuffering() const;

    const void* data() const;
    const void* constData() const;

    void commit(JNIEnv *env);

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

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    template<class T>
    QSpan<const T> span() const { return data<T>() ? QSpan<const T>(data<T>(), size<T>()) : QSpan<const T>(); }
    template<class T>
    operator QSpan<const T> () const { return span<T>(); }
#endif
    template<class T>
    operator std::initializer_list<T> () const {
        return QtJambiAPI::createIterable<std::initializer_list<T>>(constData<T>(), size<T>());
    }
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
    jboolean m_is_copy;
    Q_DISABLE_COPY(JBufferConstData)
};

class QTJAMBI_EXPORT JBufferData: public JBufferConstData
{
public:
    using JBufferConstData::isBuffer;
    using JBufferConstData::isReadOnlyBuffer;
    using JBufferConstData::size;
    using JBufferConstData::capacity;
    using JBufferConstData::position;
    using JBufferConstData::limit;
    using JBufferConstData::isBuffering;
    using JBufferConstData::isReadOnly;
    using JBufferConstData::data;
    using JBufferConstData::constData;
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    using JBufferConstData::span;
#endif
    JBufferData(JNIEnv *env, jobject buffer_object);

    ~JBufferData();

    void* data();

    void commit(JNIEnv *env);

    template<class T>
    inline T* data() {return reinterpret_cast<T*>(data());}
    template<class T>
    inline operator T*() {return data<T>();}

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    template<class T>
    operator QSpan<T> () { return data<T>() ? QSpan<T>(data<T>(), size<T>()) : QSpan<T>(); }
    template<class T>
    operator QSpan<const T> () const { return data<const T>() ? QSpan<const T>(data<const T>(), size<const T>()) : QSpan<const T>(); }
#endif
    template<class T>
    operator std::initializer_list<T> () const {
        return QtJambiAPI::createIterable<std::initializer_list<T>>(constData<T>(), size<T>());
    }
private:
    Q_DISABLE_COPY(JBufferData)
};

class AbstractDataJBuffer{
public:
    AbstractDataJBuffer(JNIEnv *env, void* data, qsizetype capacity);
    AbstractDataJBuffer(JNIEnv *env, const void* data, qsizetype capacity);
    AbstractDataJBuffer(JNIEnv *env, char* data, qsizetype capacity);
    AbstractDataJBuffer(JNIEnv *env, const char* data, qsizetype capacity);
    AbstractDataJBuffer(JNIEnv *env, unsigned char* data, qsizetype capacity);
    AbstractDataJBuffer(JNIEnv *env, const unsigned char* data, qsizetype capacity);
    AbstractDataJBuffer(JNIEnv *env, qint16* data, qsizetype capacity);
    AbstractDataJBuffer(JNIEnv *env, const qint16* data, qsizetype capacity);
    AbstractDataJBuffer(JNIEnv *env, quint16* data, qsizetype capacity);
    AbstractDataJBuffer(JNIEnv *env, const quint16* data, qsizetype capacity);
    AbstractDataJBuffer(JNIEnv *env, qint32* data, qsizetype capacity);
    AbstractDataJBuffer(JNIEnv *env, const qint32* data, qsizetype capacity);
    AbstractDataJBuffer(JNIEnv *env, quint32* data, qsizetype capacity);
    AbstractDataJBuffer(JNIEnv *env, const quint32* data, qsizetype capacity);
    AbstractDataJBuffer(JNIEnv *env, qint64* data, qsizetype capacity);
    AbstractDataJBuffer(JNIEnv *env, const qint64* data, qsizetype capacity);
    AbstractDataJBuffer(JNIEnv *env, quint64* data, qsizetype capacity);
    AbstractDataJBuffer(JNIEnv *env, const quint64* data, qsizetype capacity);
    AbstractDataJBuffer(JNIEnv *env, float* data, qsizetype capacity);
    AbstractDataJBuffer(JNIEnv *env, const float* data, qsizetype capacity);
    AbstractDataJBuffer(JNIEnv *env, double* data, qsizetype capacity);
    AbstractDataJBuffer(JNIEnv *env, const double* data, qsizetype capacity);
    AbstractDataJBuffer(JNIEnv *env, QChar* data, qsizetype capacity);
    AbstractDataJBuffer(JNIEnv *env, const QChar* data, qsizetype capacity);
    AbstractDataJBuffer(JNIEnv *env, wchar_t* data, qsizetype capacity);
    AbstractDataJBuffer(JNIEnv *env, const wchar_t* data, qsizetype capacity);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    AbstractDataJBuffer(JNIEnv *env, char16_t* data, qsizetype capacity);
    AbstractDataJBuffer(JNIEnv *env, const char16_t* data, qsizetype capacity);
    AbstractDataJBuffer(JNIEnv *env, char32_t* data, qsizetype capacity);
    AbstractDataJBuffer(JNIEnv *env, const char32_t* data, qsizetype capacity);
#endif
    ~AbstractDataJBuffer();
    static QTJAMBI_EXPORT void setLimit(JNIEnv *env, jobject buffer, jsize limit);
private:
    jobject m_buffer_object;
    friend class LocalDataJBuffer;
    friend class DataJBuffer;
};

class QTJAMBI_EXPORT LocalDataJBuffer : public AbstractDataJBuffer{
public:
    LocalDataJBuffer(JNIEnv *env, void* data, qsizetype capacity);
    LocalDataJBuffer(JNIEnv *env, const void* data, qsizetype capacity);
    LocalDataJBuffer(JNIEnv *env, char* data, qsizetype capacity);
    LocalDataJBuffer(JNIEnv *env, const char* data, qsizetype capacity);
    LocalDataJBuffer(JNIEnv *env, unsigned char* data, qsizetype capacity);
    LocalDataJBuffer(JNIEnv *env, const unsigned char* data, qsizetype capacity);
    LocalDataJBuffer(JNIEnv *env, qint16* data, qsizetype capacity);
    LocalDataJBuffer(JNIEnv *env, const qint16* data, qsizetype capacity);
    LocalDataJBuffer(JNIEnv *env, quint16* data, qsizetype capacity);
    LocalDataJBuffer(JNIEnv *env, const quint16* data, qsizetype capacity);
    LocalDataJBuffer(JNIEnv *env, qint32* data, qsizetype capacity);
    LocalDataJBuffer(JNIEnv *env, const qint32* data, qsizetype capacity);
    LocalDataJBuffer(JNIEnv *env, quint32* data, qsizetype capacity);
    LocalDataJBuffer(JNIEnv *env, const quint32* data, qsizetype capacity);
    LocalDataJBuffer(JNIEnv *env, qint64* data, qsizetype capacity);
    LocalDataJBuffer(JNIEnv *env, const qint64* data, qsizetype capacity);
    LocalDataJBuffer(JNIEnv *env, quint64* data, qsizetype capacity);
    LocalDataJBuffer(JNIEnv *env, const quint64* data, qsizetype capacity);
    LocalDataJBuffer(JNIEnv *env, float* data, qsizetype capacity);
    LocalDataJBuffer(JNIEnv *env, const float* data, qsizetype capacity);
    LocalDataJBuffer(JNIEnv *env, double* data, qsizetype capacity);
    LocalDataJBuffer(JNIEnv *env, const double* data, qsizetype capacity);
    LocalDataJBuffer(JNIEnv *env, QChar* data, qsizetype capacity);
    LocalDataJBuffer(JNIEnv *env, const QChar* data, qsizetype capacity);
    LocalDataJBuffer(JNIEnv *env, wchar_t* data, qsizetype capacity);
    LocalDataJBuffer(JNIEnv *env, const wchar_t* data, qsizetype capacity);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    LocalDataJBuffer(JNIEnv *env, char16_t* data, qsizetype capacity);
    LocalDataJBuffer(JNIEnv *env, const char16_t* data, qsizetype capacity);
    LocalDataJBuffer(JNIEnv *env, char32_t* data, qsizetype capacity);
    LocalDataJBuffer(JNIEnv *env, const char32_t* data, qsizetype capacity);
#endif
    ~LocalDataJBuffer();
    using AbstractDataJBuffer::setLimit;
    void setLimit(jsize limit);
    jobject buffer() const;
    jobject take();
private:
    JNIEnv *m_env;
};

class QTJAMBI_EXPORT DataJBuffer : public AbstractDataJBuffer{
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
    DataJBuffer(JNIEnv *env, wchar_t* data, qsizetype capacity);
    DataJBuffer(JNIEnv *env, const wchar_t* data, qsizetype capacity);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    DataJBuffer(JNIEnv *env, char16_t* data, qsizetype capacity);
    DataJBuffer(JNIEnv *env, const char16_t* data, qsizetype capacity);
    DataJBuffer(JNIEnv *env, char32_t* data, qsizetype capacity);
    DataJBuffer(JNIEnv *env, const char32_t* data, qsizetype capacity);
#endif
    ~DataJBuffer();
    using AbstractDataJBuffer::setLimit;
    void setLimit(JNIEnv *env, jsize limit);
    jobject buffer(JNIEnv *env) const;
    jobject take(JNIEnv *env);
    void clear(JNIEnv *env);
};

#endif // QTJAMBI_JAVABUFFERS_H
