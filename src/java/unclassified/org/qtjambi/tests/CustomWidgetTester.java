/****************************************************************************
**
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

package org.qtjambi.tests;

import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

public class CustomWidgetTester extends QWidget {

    public Signal1<String> textChanged = new Signal1<String>();
    public Signal1<String> positionChanged = new Signal1<String>();

    public CustomWidgetTester(QWidget parent) {
        super(parent);
        resetText();
    }

    public CustomWidgetTester() {
        this(null);
    }


    public String text() {
        return text;
    }


    public void setText(String text) {
        this.text = text;
        update();
        textChanged.emit(text);
    }


    @QtPropertyResetter
    public void resetText() {
        setText("Qt Jambi Dummy Widget..");
    }


    public QPoint position() {
        return position;
    }

    public boolean canDesignText() {
        return position.x() >= 50;
    }


    public void setPosition(QPoint position) {
        this.position = position;
        update();
        positionChanged.emit("x=" + position.x() + ", y=" + position.y());
    }


    public void setPositionX(int x) { setPosition(new QPoint(x, position.y())); }
    public void setPositionY(int y) { setPosition(new QPoint(position.x(), y)); }

    @Override
    public QSize sizeHint() {
        return new QSize(200, 200);
    }

    @Override
    protected void paintEvent(QPaintEvent e) {
        QPainter p = new QPainter();
        p.begin(this);

        p.setPen(new QPen(palette().brush(QPalette.ColorRole.WindowText), 0));

        p.drawText(position.x(), position.y(), text);

        p.end();
    }

    private String text;
    private QPoint position = new QPoint(50, 50);

    public static void main(String args[]) throws Exception {


    }
}
