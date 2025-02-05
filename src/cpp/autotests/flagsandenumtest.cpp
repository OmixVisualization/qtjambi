/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "flagsandenumtest.h"

FlagsAndEnumTest::FlagsAndEnumTest(QObject *parent) :
    QObject(parent)
{
    QObject::connect(this, SIGNAL(testSignal1(Qt::AlignmentFlag)), this, SLOT(testSlot1(Qt::AlignmentFlag)));
    QObject::connect(this, SIGNAL(testSignal2(Qt::Orientation)), this, SLOT(testSlot2(Qt::Orientation)));
    QObject::connect(this, SIGNAL(testSignal3(Qt::Alignment)), this, SLOT(testSlot3(Qt::Alignment)));
    if(!QObject::connect(this, SIGNAL(testSignal4(std::initializer_list<Qt::WidgetAttribute>)), this, SLOT(testSlot4(std::initializer_list<Qt::WidgetAttribute>)))){
        qWarning("unable to connect testSlot4(std::initializer_list<Qt::WidgetAttribute>)");
    }
    if(!QObject::connect(this, SIGNAL(testSignal5(std::initializer_list<int>)), this, SLOT(testSlot5(std::initializer_list<int>)))){
        qWarning("unable to connect testSlot5(std::initializer_list<int>)");
    }
}

void FlagsAndEnumTest::processResult(int result){
    Q_UNUSED(result)
}

void FlagsAndEnumTest::testSlot1(Qt::AlignmentFlag o){
    processResult(o);
}

void FlagsAndEnumTest::testSlot2(Qt::Orientation o){
    processResult(o);
}

void FlagsAndEnumTest::testSlot3(Qt::Alignment o){
    processResult(int(o));
}

void FlagsAndEnumTest::testSlot4(std::initializer_list<Qt::WidgetAttribute> o){
	int result = 0;
	const Qt::WidgetAttribute* attr = o.begin();
    for(quint64 i=0; i<o.size(); ++i){
        result += int(attr[i]);
	}
	setAttributes(o);
    processResult(result);
}

void FlagsAndEnumTest::testSlot5(std::initializer_list<int> o){
    int result = 0;
    const int* attr = o.begin();
    for(quint64 i=0; i<o.size(); ++i){
        result += int(attr[i]);
    }
    setInts(o);
    processResult(result);
}

void FlagsAndEnumTest::setAttributes(std::initializer_list<Qt::WidgetAttribute> attributes){
	m_attributes.clear();
	const Qt::WidgetAttribute* attr = attributes.begin();
    for(quint64 i=0; i<attributes.size(); ++i){
		m_attributes << attr[i];
	}
}

void FlagsAndEnumTest::setInts(std::initializer_list<int> iList){
    m_ints.clear();
    const int* attr = iList.begin();
    for(quint64 i=0; i<iList.size(); ++i){
        m_ints << attr[i];
    }
}
	
const QList<Qt::WidgetAttribute>& FlagsAndEnumTest::getAttributes() const{
	return m_attributes;
}

const QList<int>& FlagsAndEnumTest::getInts() const{
    return m_ints;
}

QObject* FlagsAndEnumTest::createHiddenObject(QObject* parent){
    return new HiddenObject(parent);
}

HiddenObject::HiddenObject(QObject *parent) : QObject(parent)
{
}

HiddenObject::~HiddenObject(){}

HiddenObject::HiddenEnumClass HiddenObject::hiddenEnum() const{ return m_hiddenEnumClass; }

void HiddenObject::setHiddenEnum(HiddenEnumClass hiddenEnumClass){
    m_hiddenEnumClass = hiddenEnumClass;
    emit hiddenEnumChanged(hiddenEnumClass);
}

HiddenObject::HiddenFlags HiddenObject::hiddenFlags() const{ return m_hiddenFlags; }

void HiddenObject::setHiddenFlags(HiddenFlags hiddenFlags){
    m_hiddenFlags = hiddenFlags;
    emit hiddenFlagsChanged(hiddenFlags);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 9, 0)
HiddenObject::HiddenEnumClass64 HiddenObject::hiddenEnum64() const{ return m_hiddenEnumClass64; }

void HiddenObject::setHiddenEnum64(HiddenEnumClass64 hiddenEnumClass){
    m_hiddenEnumClass64 = hiddenEnumClass;
    emit hiddenEnum64Changed(hiddenEnumClass);
}

HiddenObject::HiddenFlags64 HiddenObject::hiddenFlags64() const{ return m_hiddenFlags64; }

void HiddenObject::setHiddenFlags64(HiddenFlags64 hiddenFlags){
    m_hiddenFlags64 = hiddenFlags;
    emit hiddenFlags64Changed(hiddenFlags);
}
#endif

static const bool __init = []()->bool{
        qRegisterMetaType<HiddenObject::HiddenFlags>();
        qRegisterMetaType<HiddenObject::HiddenEnumClass>();
#if QT_VERSION >= QT_VERSION_CHECK(6, 9, 0)
        qRegisterMetaType<HiddenObject::HiddenFlags64>();
        qRegisterMetaType<HiddenObject::HiddenEnumClass64>();
#endif
        return true;
    }();

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)

#define SPAN_METHODS(Name, type, mutation)\
void SpanTest::set##Name(QSpan<const type> span) {\
    m_##Name = QList<type>(span.begin(), span.end());\
}\
\
QSpan<const type> SpanTest::get##Name() const {\
    return m_##Name;\
}\
\
void SpanTest::setMutable##Name(QSpan<type> span) {\
    m_##Name.clear();\
    for(type& value : span){\
        m_##Name << (mutation);\
    }\
}\
\
QSpan<type> SpanTest::getMutable##Name() {\
    return m_##Name;\
}

SPAN_METHODS(ByteArrays, QByteArray, value+="!")
SPAN_METHODS(Ints, int, ++value)
SPAN_METHODS(Longs, qint64, ++value)
SPAN_METHODS(Shorts, qint16, ++value)
SPAN_METHODS(Bytes, char, ++value)
SPAN_METHODS(Booleans, bool, value = !value)
SPAN_METHODS(Floats, float, ++value)
SPAN_METHODS(Doubles, double, ++value)
SPAN_METHODS(Strings, QString, value+=u'!')
SPAN_METHODS(Chars, QChar, value = QChar(value.unicode()+1))

SpanTest::SpanTest(QObject *parent) : QObject(parent) {}

int SpanTest::virtualMutableSpan(QSpan<int> mutableInts, int result){
    for(int& value : mutableInts){
        result += value++;
    }
    return result;
}

char SpanTest::callVirtualMutableSpan(QSpan<char> mutableInts, char init){
    return virtualMutableSpan(mutableInts, init);
}

char SpanTest::virtualMutableSpan(QSpan<char> mutableInts, char result){
    for(char& value : mutableInts){
        result += value++;
    }
    return result;
}

int SpanTest::callVirtualMutableSpan(QSpan<int> mutableInts, int init){
    return virtualMutableSpan(mutableInts, init);
}

qint64 SpanTest::virtualSpan(QSpan<const qint64> longs, qint64 result){
    for(qint64 value : longs){
        result += value;
    }
    return result;
}

qint64 SpanTest::callVirtualSpan(QSpan<const qint64> longs, qint64 result){
    return virtualSpan(longs, result);
}

void SpanTest::virtualMutableSpan(QSpan<QString> mutableStrings){
    for(QString& value : mutableStrings){
        value += u'!';
    }
}

void SpanTest::callVirtualMutableSpan(QSpan<QString> mutableStrings){
    virtualMutableSpan(mutableStrings);
}

quint64 SpanTest::virtualSpan(QSpan<const QByteArray> byteArrays){
    return byteArrays.size();
}

quint64 SpanTest::callVirtualSpan(QSpan<const QByteArray> byteArrays){
    return virtualSpan(byteArrays);
}

QSpan<const qint64> SpanTest::virtualSpanReturning(){
    return m_Longs;
}

QSpan<qint64> SpanTest::virtualMutableSpanReturning(){
    return m_Longs;
}

QSpan<qint64> SpanTest::callVirtualMutableSpanReturning(){
    return virtualMutableSpanReturning();
}

QSpan<const qint64> SpanTest::callVirtualSpanReturning(){
    return virtualSpanReturning();
}

#endif

