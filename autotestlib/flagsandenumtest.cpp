/****************************************************************************
**
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
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
	
const QList<Qt::WidgetAttribute>& FlagsAndEnumTest::getAttributes(){
	return m_attributes;
}

const QList<int>& FlagsAndEnumTest::getInts(){
    return m_ints;
}

QObject* FlagsAndEnumTest::createHiddenObject(QObject* parent){
    return new HiddenObject(parent);
}

HiddenObject::HiddenObject(QObject *parent) : QObject(parent), m_hiddenFlags(), m_hiddenEnumClass(HiddenEnumClass::None)
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

static const bool __init = []()->bool{
        qRegisterMetaType<HiddenObject::HiddenFlags>();
        qRegisterMetaType<HiddenObject::HiddenEnumClass>();
        return true;
    }();

