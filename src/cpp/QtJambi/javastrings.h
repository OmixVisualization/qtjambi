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

#if !defined(QTJAMBI_JAVASTRINGS_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBI_JAVASTRINGS_H

#include "global.h"

class QTJAMBI_EXPORT J2CStringBuffer{
public:
    J2CStringBuffer(JNIEnv* env, jstring strg);
    ~J2CStringBuffer();
    const char* constData() const;
    inline operator const char*() const { return constData(); }
    QByteArray toByteArray() const;
    inline operator QByteArray() const { return toByteArray(); }
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    inline QByteArrayView toByteArrayView() const { return QByteArrayView(constData(), length()); }
    inline operator QByteArrayView() const { return toByteArrayView(); }
    inline QUtf8StringView toUtf8StringView() const { return QUtf8StringView(constData(), length()); }
    inline operator QUtf8StringView() const { return toUtf8StringView(); }
#endif
#if QT_VERSION >= QT_VERSION_CHECK(7,0,0)
    inline QLatin1StringView toLatin1StringView() const { return QLatin1StringView(constData(), length()); }
#endif
    inline QLatin1String toLatin1String() const { return QLatin1String(constData(), length()); }
    inline operator QLatin1String() const { return toLatin1String(); }
    inline const char* data() const { return constData(); }
    inline const char* data() { return constData(); }
    int length() const;
private:
    const jstring m_strg;
    const jsize m_length;
    const char* m_data;
    Q_DISABLE_COPY_MOVE(J2CStringBuffer)
};

class QTJAMBI_EXPORT JString2QChars{
public:
    JString2QChars(JNIEnv* env, jstring strg);
    ~JString2QChars();
    const QChar* constData() const;
    inline operator const QChar*() const { return constData(); }
    inline QStringView toStringView() const { return QStringView(constData(), length()); }
    inline operator QStringView() const { return toStringView(); }
    QString toString() const;
    inline operator QString() const { return toString(); }
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    inline QAnyStringView toAnyStringView() const { return QAnyStringView(constData(), length()); }
    inline operator QAnyStringView() const { return toAnyStringView(); }
#endif
    inline const QChar* data() const { return constData(); }
    inline const QChar* data() { return constData(); }
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    inline const char16_t* chars() const { return *this; }
    inline const char16_t* chars() { return *this; }
    operator const char16_t*() const;
#endif
    int length() const;
private:
    const jstring m_strg;
    const jsize m_length;
    const jchar* m_data;
    Q_DISABLE_COPY_MOVE(JString2QChars)
};

#endif // QTJAMBI_JAVASTRINGS_H
