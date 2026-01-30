/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

#ifndef POLYMORPHICTYPE_H
#define POLYMORPHICTYPE_H

#include <QtWidgets/QStyleOption>
#include <QtWidgets/QApplication>
#include <QtCore/QEvent>
#include <QtGui/QPaintEvent>
#include <QtWidgets/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QImage>

class PolymorphicType
{
public:
    static QEvent *getPaintEvent();
    static QEvent *getCustomEvent(int i);
    static void sendPaintEvent(QWidget *w);
    static void sendCustomEvent(QWidget *w, int i);

    static QStyleOption *getButtonStyleOption();
    static QStyleOption *getCustomStyleOption(int i);
    static QStyleOption *getUnmappedCustomStyleOption();
    static void sendButtonStyleOption(QWidget *w);
    static void sendCustomStyleOption(QWidget *w, int i);
    static void sendUnmappedCustomStyleOption(QWidget *w);
};

class CustomEvent: public QEvent
{
public:
    CustomEvent(int something);
    static QEvent::Type customType();
    int m_something;
};

class CustomStyleOption: public QStyleOption
{
public:
    CustomStyleOption(const CustomStyleOption &other);
    CustomStyleOption();
    CustomStyleOption(int something);
    int m_something;
};

#endif //POLYMORPHICTYPE_H
