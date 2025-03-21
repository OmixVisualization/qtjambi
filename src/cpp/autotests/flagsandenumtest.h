/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

#ifndef FLAGSANDENUMTEST_H
#define FLAGSANDENUMTEST_H

#ifndef QTJAMBI_GENERATOR_RUNNING
#include <QtJambi/global.h>
#endif
#include <QtCore/QObject>
#include "general.h"

class HiddenObject : public QObject
{
    Q_OBJECT
public:
    HiddenObject(QObject *parent = nullptr);
    ~HiddenObject();

    enum HiddenEnum{
        None = 0, Entry1 = 1, Entry2 = 2, Entry3 = 4, Entry4 = 8
    };
    Q_DECLARE_FLAGS(HiddenFlags, HiddenEnum)
    Q_FLAG(HiddenFlags)

    enum class HiddenEnumClass{
        None = 0, Entry1 = 1, Entry2 = 2, Entry3 = 4, Entry4 = 8
    };
    Q_ENUM(HiddenEnumClass)

    Q_PROPERTY(HiddenObject::HiddenFlags hiddenFlags READ hiddenFlags WRITE setHiddenFlags NOTIFY hiddenFlagsChanged)
    Q_PROPERTY(HiddenObject::HiddenEnumClass hiddenEnum READ hiddenEnum WRITE setHiddenEnum NOTIFY hiddenEnumChanged)

    Q_INVOKABLE HiddenObject::HiddenEnumClass hiddenEnum() const;
    Q_INVOKABLE void setHiddenEnum(HiddenObject::HiddenEnumClass hiddenEnumClass);
    Q_INVOKABLE HiddenObject::HiddenFlags hiddenFlags() const;
    Q_INVOKABLE void setHiddenFlags(HiddenObject::HiddenFlags hiddenFlags);

Q_SIGNALS:
    void hiddenEnumChanged(HiddenObject::HiddenEnumClass hiddenEnumClass);
    void hiddenFlagsChanged(HiddenObject::HiddenFlags hiddenFlags);
private:
    HiddenFlags m_hiddenFlags = {};
    HiddenEnumClass m_hiddenEnumClass = HiddenEnumClass::None;

#if QT_VERSION >= QT_VERSION_CHECK(6, 9, 0)
public:
    enum HiddenEnum64 : qint64{
        None_64 = 0, Entry1_64 = 1, Entry2_64 = 2, Entry3_64 = 4, Entry4_64 = 8
    };
    Q_DECLARE_FLAGS(HiddenFlags64, HiddenEnum64)
    Q_FLAG(HiddenFlags64)

    enum class HiddenEnumClass64 : qint64{
        None = 0, Entry1 = 1, Entry2 = 2, Entry3 = 4, Entry4 = 8
    };
    Q_ENUM(HiddenEnumClass64)

    Q_PROPERTY(HiddenObject::HiddenFlags64 hiddenFlags64 READ hiddenFlags64 WRITE setHiddenFlags64 NOTIFY hiddenFlags64Changed)
    Q_PROPERTY(HiddenObject::HiddenEnumClass64 hiddenEnum64 READ hiddenEnum64 WRITE setHiddenEnum64 NOTIFY hiddenEnum64Changed)

    Q_INVOKABLE HiddenObject::HiddenEnumClass64 hiddenEnum64() const;
    Q_INVOKABLE void setHiddenEnum64(HiddenObject::HiddenEnumClass64 hiddenEnumClass);
    Q_INVOKABLE HiddenObject::HiddenFlags64 hiddenFlags64() const;
    Q_INVOKABLE void setHiddenFlags64(HiddenObject::HiddenFlags64 hiddenFlags);

Q_SIGNALS:
    void hiddenEnum64Changed(HiddenObject::HiddenEnumClass64 hiddenEnumClass);
    void hiddenFlags64Changed(HiddenObject::HiddenFlags64 hiddenFlags);
private:
    HiddenFlags64 m_hiddenFlags64 = {};
    HiddenEnumClass64 m_hiddenEnumClass64 = HiddenEnumClass64::None;
#endif

private:
    Q_DISABLE_COPY(HiddenObject)
};

Q_DECLARE_METATYPE(HiddenObject*)
Q_DECLARE_METATYPE(HiddenObject::HiddenFlags)
Q_DECLARE_METATYPE(HiddenObject::HiddenEnumClass)
Q_DECLARE_OPERATORS_FOR_FLAGS(HiddenObject::HiddenFlags)

#if QT_VERSION >= QT_VERSION_CHECK(6, 9, 0)
Q_DECLARE_METATYPE(HiddenObject::HiddenFlags64)
Q_DECLARE_METATYPE(HiddenObject::HiddenEnumClass64)
Q_DECLARE_OPERATORS_FOR_FLAGS(HiddenObject::HiddenFlags64)
#endif

class FlagsAndEnumTest : public QObject
{
    Q_OBJECT
public:
#if QT_VERSION >= QT_VERSION_CHECK(6, 9, 0)
    enum Enum64 : qint64{
        None = 0, Entry1 = 1, Entry2 = 2, Entry3 = 4, Entry4 = 8
    };
    Q_DECLARE_FLAGS(Flags64, Enum64)
    Q_FLAG(Flags64)
#endif

    explicit FlagsAndEnumTest(QObject *parent = nullptr);

    virtual void processResult(int result);

	virtual void setAttributes(std::initializer_list<Qt::WidgetAttribute> attributes);
    virtual void setInts(std::initializer_list<int> iList);
    const QList<Qt::WidgetAttribute>& getAttributes() const;
    const QList<int>& getInts() const;

    static QObject* createHiddenObject(QObject* parent);
    
public slots:
    void testSlot1(Qt::AlignmentFlag o);
    void testSlot2(Qt::Orientation o);
    void testSlot3(Qt::Alignment o);
    void testSlot4(std::initializer_list<Qt::WidgetAttribute> o);
    void testSlot5(std::initializer_list<int> o);
signals:
    void testSignal1(Qt::AlignmentFlag);
    void testSignal2(Qt::Orientation);
    void testSignal3(Qt::Alignment);
	void testSignal4(std::initializer_list<Qt::WidgetAttribute>);
    void testSignal5(std::initializer_list<int>);

private:
	QList<Qt::WidgetAttribute> m_attributes;
    QList<int> m_ints;
};

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
template<typename T>
struct QtPrivate::QEqualityOperatorForType<std::initializer_list<T>,true>
{
    static bool equals(const QMetaTypeInterface *, const void *a, const void *b)
    { return initializer_list_equals<T>(*reinterpret_cast<const std::initializer_list<T> *>(a), *reinterpret_cast<const std::initializer_list<T> *>(b)); }
};
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)

#define SPAN_METHODS(Name, type)\
public:\
    void set##Name(QSpan<const type> span);\
    void setMutable##Name(QSpan<type> span);\
    QSpan<const type> get##Name() const;\
    QSpan<type> getMutable##Name();\
private:\
    QList<type> m_##Name;

class SpanTest : public QObject
{
    Q_OBJECT
public:
    struct AlignedStruct{
        qint64 longValue;
        qint32 intValue;
        char byteValue;
        bool operator==(const AlignedStruct& other)const{
            return other.byteValue==byteValue
                   && other.intValue==intValue
                   && other.longValue==longValue;
        }
        QString toString()const{
            return QString::asprintf("AlignedStruct(j=%lld, i=%d, b=%hhd)", longValue, intValue, byteValue);
        }
    };
    explicit SpanTest(QObject *parent = nullptr);

    virtual void virtualMutableSpan(QSpan<QString> mutableStrings);
    void callVirtualMutableSpan(QSpan<QString> mutableStrings);

    virtual quint64 virtualSpan(QSpan<const QByteArray> byteArrays);
    quint64 callVirtualSpan(QSpan<const QByteArray> byteArrays);

    virtual int virtualMutableSpan(QSpan<int> mutableInts, int init);
    int callVirtualMutableSpan(QSpan<int> mutableInts, int init = 0);

    virtual char virtualMutableSpan(QSpan<char> mutableInts, char init);
    char callVirtualMutableSpan(QSpan<char> mutableInts, char init = 0);

    virtual qint64 virtualSpan(QSpan<const qint64> longs, qint64 init);
    qint64 callVirtualSpan(QSpan<const qint64> longs, qint64 init = 0);

    virtual QSpan<const qint64> virtualSpanReturning();
    QSpan<const qint64> callVirtualSpanReturning();

    virtual QSpan<qint64> virtualMutableSpanReturning();
    QSpan<qint64> callVirtualMutableSpanReturning();

    SPAN_METHODS(ByteArrays, QByteArray)
    SPAN_METHODS(Booleans, bool)
    SPAN_METHODS(Chars, QChar)
    SPAN_METHODS(Strings, QString)
    SPAN_METHODS(Ints, int)
    SPAN_METHODS(Bytes, char)
    SPAN_METHODS(Longs, qint64)
    SPAN_METHODS(Shorts, qint16)
    SPAN_METHODS(Floats, float)
    SPAN_METHODS(Doubles, double)
};

#undef SPAN_METHODS

#endif

#endif // FLAGSANDENUMTEST_H
