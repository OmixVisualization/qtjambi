/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "polymorphictype.h"

class UnmappedCustomStyleOption: public QStyleOption
{
public:
    UnmappedCustomStyleOption(const UnmappedCustomStyleOption &other) : QStyleOption(other)
    {
        type = QStyleOption::SO_CustomBase + 2;
        m_something_else = other.m_something_else;
    }
    UnmappedCustomStyleOption() : QStyleOption(1, QStyleOption::SO_CustomBase + 2) { }
    int m_something_else;
};

CustomEvent::CustomEvent(int something) : QEvent(customType()) { m_something = something; }

QEvent::Type CustomEvent::customType(){
    static QEvent::Type customType = QEvent::Type(QEvent::registerEventType());
    return customType;
}

CustomStyleOption::CustomStyleOption(const CustomStyleOption &other) : QStyleOption(other)
{
    type = QStyleOption::SO_CustomBase + 1;
    m_something = other.m_something;
}
CustomStyleOption::CustomStyleOption() { m_something = 0; }
CustomStyleOption::CustomStyleOption(int something) : QStyleOption(1, QStyleOption::SO_CustomBase + 1) { m_something = something; }

QEvent *PolymorphicType::getPaintEvent() { return new QPaintEvent(QRect(10, 10, 10, 10)); }
QEvent *PolymorphicType::getCustomEvent(int i) { return new CustomEvent(i); }
void PolymorphicType::sendPaintEvent(QWidget *w) { QApplication::sendEvent(w, getPaintEvent()); }
void PolymorphicType::sendCustomEvent(QWidget *w, int i) { QApplication::sendEvent(w, getCustomEvent(i)); }

QStyleOption *PolymorphicType::getButtonStyleOption() { return new QStyleOptionButton(); }
QStyleOption *PolymorphicType::getCustomStyleOption(int i) { return new CustomStyleOption(i); }
QStyleOption *PolymorphicType::getUnmappedCustomStyleOption() { return new UnmappedCustomStyleOption(); }
void PolymorphicType::sendButtonStyleOption(QWidget *w)
{
    QImage img;
    QPainter p(&img);
    w->style()->drawControl(QStyle::ControlElement(QStyle::CE_PushButton),
                            getButtonStyleOption(),
                            &p);
}
void PolymorphicType::sendCustomStyleOption(QWidget *w, int i)
{
    QImage img;
    QPainter p(&img);
    w->style()->drawControl(QStyle::ControlElement(QStyle::CE_CustomBase + 1),
                            getCustomStyleOption(i),
                            &p);
}
void PolymorphicType::sendUnmappedCustomStyleOption(QWidget *w)
{
    QImage img;
    QPainter p(&img);
    w->style()->drawControl(QStyle::ControlElement(QStyle::CE_CustomBase + 2),
                            getUnmappedCustomStyleOption(),
                            &p);
}
