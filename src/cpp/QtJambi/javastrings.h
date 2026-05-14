/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtCore/QString>
#include "global.h"
#include <QtCore/QScopedPointer>
#include "exception.h"
#include "scope.h"

class QTJAMBI_EXPORT J2CStringBuffer{
public:
    J2CStringBuffer(JNIEnv* env, jstring strg);
    ~J2CStringBuffer();
    const char* constData() const;
    inline operator const char*() const { return constData(); }
    QByteArray toByteArray() const;
    inline operator QByteArray() const { return toByteArray(); }
    inline QByteArrayView toByteArrayView() const { return QByteArrayView(constData(), length()); }
    inline operator QByteArrayView() const { return toByteArrayView(); }
    inline QUtf8StringView toUtf8StringView() const { return QUtf8StringView(constData(), length()); }
    inline operator QUtf8StringView() const { return toUtf8StringView(); }
    inline QAnyStringView toAnyStringView() const { return QAnyStringView(constData(), length()); }
    inline operator QAnyStringView() const { return toAnyStringView(); }
#if QT_VERSION >= QT_VERSION_CHECK(7,0,0)
    inline QLatin1StringView toLatin1StringView() const { return QLatin1StringView(constData(), length()); }
#endif
    inline QLatin1String toLatin1String() const { return QLatin1String(constData(), length()); }
    inline operator QLatin1String() const { return toLatin1String(); }
    inline std::string_view toStdStringView() const { return std::string_view(constData(), length()); }
    inline operator std::string_view() const { return toStdStringView(); }
    inline std::string toStdString() const { return std::string(constData(), length()); }
    inline operator std::string() const { return toStdString(); }
#if defined(__cpp_char8_t)
    inline std::u8string_view toStdU8StringView() const { return std::u8string_view(reinterpret_cast<const char8_t*>(constData()), length()); }
    inline operator std::u8string_view() const { return toStdU8StringView(); }
    inline std::u8string toStdU8String() const { return std::u8string(reinterpret_cast<const char8_t*>(constData()), length()); }
    inline operator std::u8string() const { return toStdU8String(); }
#endif
    inline const char* data() const { return constData(); }
    inline const char* data() { return constData(); }
    int length() const;
private:
    const jstring m_strg;
    const jsize m_length;
    const char* m_data;
    JNIEnv* m_env;
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
    Q_DISABLE_COPY_MOVE(J2CStringBuffer)
};

class QTJAMBI_EXPORT PersistentJ2CStringBuffer{
public:
    PersistentJ2CStringBuffer(JNIEnv* env, jstring strg);
    ~PersistentJ2CStringBuffer();
    void clear(JNIEnv* env);
    const char* constData() const;
    inline operator const char*() const { return constData(); }
    QByteArray toByteArray() const;
    inline operator QByteArray() const { return toByteArray(); }
    inline QByteArrayView toByteArrayView() const { return QByteArrayView(constData(), length()); }
    inline operator QByteArrayView() const { return toByteArrayView(); }
    inline QUtf8StringView toUtf8StringView() const { return QUtf8StringView(constData(), length()); }
    inline operator QUtf8StringView() const { return toUtf8StringView(); }
    inline QAnyStringView toAnyStringView() const { return QAnyStringView(constData(), length()); }
    inline operator QAnyStringView() const { return toAnyStringView(); }
#if QT_VERSION >= QT_VERSION_CHECK(7,0,0)
    inline QLatin1StringView toLatin1StringView() const { return QLatin1StringView(constData(), length()); }
#endif
    inline QLatin1String toLatin1String() const { return QLatin1String(constData(), length()); }
    inline operator QLatin1String() const { return toLatin1String(); }
    inline std::string_view toStdStringView() const { return std::string_view(constData(), length()); }
    inline operator std::string_view() const { return toStdStringView(); }
    inline std::string toStdString() const { return std::string(constData(), length()); }
    inline operator std::string() const { return toStdString(); }
#if defined(__cpp_char8_t)
    inline std::u8string_view toStdStringView() const { return std::u8string_view(reinterpret_cast<const char8_t*>(constData()), length()); }
    inline operator std::u8string_view() const { return toStdU8StringView(); }
    inline std::u8string toStdU8String() const { return std::u8string(reinterpret_cast<const char8_t*>(constData()), length()); }
    inline operator std::u8string() const { return toStdU8String(); }
#endif
    inline const char* data() const { return constData(); }
    inline const char* data() { return constData(); }
    int length() const;
private:
    QScopedPointer<struct J2CStringBufferPrivate> m_data;
    Q_DISABLE_COPY_MOVE(PersistentJ2CStringBuffer)
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
    inline QAnyStringView toAnyStringView() const { return QAnyStringView(constData(), length()); }
    inline operator QAnyStringView() const { return toAnyStringView(); }
    inline std::u16string_view toStdU16StringView() const { return std::u16string_view(chars(), length()); }
    inline operator std::u16string_view() const { return toStdU16StringView(); }
    inline std::u16string toStdU16String() const { return std::u16string(chars(), length()); }
    inline operator std::u16string() const { return toStdU16String(); }
    inline const QChar* data() const { return constData(); }
    inline const QChar* data() { return constData(); }
    inline const char16_t* chars() const { return *this; }
    inline const char16_t* chars() { return *this; }
    operator const char16_t*() const;
    int length() const;
private:
    const jstring m_strg;
    const jsize m_length;
    const jchar* m_data;
    JNIEnv* m_env;
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
    Q_DISABLE_COPY_MOVE(JString2QChars)
};

class QTJAMBI_EXPORT PersistentJString2QChars{
public:
    PersistentJString2QChars(JNIEnv* env, jstring strg);
    ~PersistentJString2QChars();
    void clear(JNIEnv* env);
    const QChar* constData() const;
    inline operator const QChar*() const { return constData(); }
    inline QStringView toStringView() const { return QStringView(constData(), length()); }
    inline operator QStringView() const { return toStringView(); }
    QString toString() const;
    inline operator QString() const { return toString(); }
    inline QAnyStringView toAnyStringView() const { return QAnyStringView(constData(), length()); }
    inline operator QAnyStringView() const { return toAnyStringView(); }
    inline std::u16string_view toStdU16StringView() const { return std::u16string_view(chars(), length()); }
    inline operator std::u16string_view() const { return toStdU16StringView(); }
    inline std::u16string toStdU16String() const { return std::u16string(chars(), length()); }
    inline operator std::u16string() const { return toStdU16String(); }
    inline const QChar* data() const { return constData(); }
    inline const QChar* data() { return constData(); }
    inline const char16_t* chars() const { return *this; }
    inline const char16_t* chars() { return *this; }
    operator const char16_t*() const;
    int length() const;
private:
    QScopedPointer<struct JString2QCharsPrivate> m_data;
    Q_DISABLE_COPY_MOVE(PersistentJString2QChars)
};

namespace QtJambiPrivate {

template<typename NativeType, typename Output>
struct convert_jstring_to_qchars{
    static constexpr Output convert(JNIEnv* env, QtJambiScope& scope, jstring strg){
        PersistentJString2QChars* buffer = new PersistentJString2QChars(env, strg);
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        scope.addDeletion(buffer);
        if constexpr(std::is_same_v<Output,QAnyStringView>){
            return buffer->toAnyStringView();
        }else if constexpr(std::is_same_v<Output,QStringView>){
            return buffer->toStringView();
        }else return *buffer;
    }
    template<typename I, bool fixSize, bool allowEmpty = true>
    static constexpr Output convert(JNIEnv* env, QtJambiScope& scope, I size, jstring strg){
        PersistentJString2QChars* buffer = new PersistentJString2QChars(env, strg);
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        scope.addDeletion(buffer);
        if constexpr(fixSize){
            if constexpr(allowEmpty){
                if(size>=0 && buffer->length()>0 && I(buffer->length())<size){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast string of size %1. Expected size: %2").arg(QString::number(buffer->length()), QString::number(size)) QTJAMBI_STACKTRACEINFO );
                }
            }else{
                if(size>=0 && I(buffer->length())<size){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast string of size %1. Expected size: %2").arg(QString::number(buffer->length()), QString::number(size)) QTJAMBI_STACKTRACEINFO );
                }
            }
        }else{
            size = I(buffer->length());
        }
        if constexpr(std::is_same_v<Output,QAnyStringView>){
            return buffer->toAnyStringView();
        }else if constexpr(std::is_same_v<Output,QStringView>){
            return buffer->toStringView();
        }else return *buffer;
    }
};

template<typename NativeType, typename Output>
struct convert_jstring_to_chars{
    static constexpr Output convert(JNIEnv* env, QtJambiScope& scope, jstring strg){
        PersistentJ2CStringBuffer* buffer = new PersistentJ2CStringBuffer(env, strg);
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        scope.addDeletion(buffer);
        if constexpr(std::is_same_v<Output,QAnyStringView>){
            return buffer->toAnyStringView();
        }else if constexpr(std::is_same_v<Output,QByteArrayView>){
            return buffer->toByteArrayView();
        }else if constexpr(std::is_same_v<Output,QUtf8StringView>){
            return buffer->toUtf8StringView();
#if QT_VERSION >= QT_VERSION_CHECK(7,0,0)
        }else if constexpr(std::is_same_v<Output,QLatin1StringView>){
            return buffer->toLatin1StringView();
#endif
        }else if constexpr(std::is_same_v<Output,QLatin1String>){
            return buffer->toLatin1String();
        }else return *buffer;
    }
    template<typename I, bool fixSize, bool allowEmpty = true>
    static constexpr Output convert(JNIEnv* env, QtJambiScope& scope, I size, jstring strg){
        PersistentJ2CStringBuffer* buffer = new PersistentJ2CStringBuffer(env, strg);
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        scope.addDeletion(buffer);
        if constexpr(fixSize){
            if constexpr(allowEmpty){
                if(size>=0 && buffer->length()>0 && I(buffer->length())<size){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast string of size %1. Expected size: %2").arg(QString::number(buffer->length()), QString::number(size)) QTJAMBI_STACKTRACEINFO );
                }
            }else{
                if(size>=0 && I(buffer->length())<size){
                    JavaException::raiseIllegalArgumentException(env, QStringLiteral("Cannot cast string of size %1. Expected size: %2").arg(QString::number(buffer->length()), QString::number(size)) QTJAMBI_STACKTRACEINFO );
                }
            }
        }else{
            size = std::remove_reference_t<I>(buffer->length());
        }
        if constexpr(std::is_same_v<Output,QAnyStringView>){
            return buffer->toAnyStringView();
        }else if constexpr(std::is_same_v<Output,QByteArrayView>){
            return buffer->toByteArrayView();
        }else if constexpr(std::is_same_v<Output,QUtf8StringView>){
            return buffer->toUtf8StringView();
#if QT_VERSION >= QT_VERSION_CHECK(7,0,0)
        }else if constexpr(std::is_same_v<Output,QLatin1StringView>){
            return buffer->toLatin1StringView();
#endif
        }else if constexpr(std::is_same_v<Output,QLatin1String>){
            return buffer->toLatin1String();
        }else return *buffer;
    }
};

} // namespace QtJambiPrivate

#endif // QTJAMBI_JAVASTRINGS_H
