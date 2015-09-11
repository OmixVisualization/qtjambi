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

package org.qtjambi.examples;

import org.qtjambi.qt.core.QPoint;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.QTime;
import org.qtjambi.qt.core.QTimer;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QHideEvent;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.gui.QPaintEvent;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPen;
import org.qtjambi.qt.gui.QPolygon;
import org.qtjambi.qt.gui.QShowEvent;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "Analog Clock")
//! [0]
public class AnalogClock
    extends QWidget
{
    static QPolygon hourHand = new QPolygon();
    static QPolygon minuteHand = new QPolygon();
    static {
        hourHand.append(new QPoint(7, 8));
        hourHand.append(new QPoint(-7, 8));
        hourHand.append(new QPoint(0, -40));

        minuteHand.append(new QPoint(7, 8));
        minuteHand.append(new QPoint(-7, 8));
        minuteHand.append(new QPoint(0, -70));
    }

    QTimer m_timer = new QTimer(this);
//! [0]

//! [1]
    public AnalogClock() {
        this(null);
    }
//! [1]

    public AnalogClock(QWidget parent) {
        super(parent);
        m_timer.timeout.connect(this, "update()");

        setWindowTitle("Analog clock");
        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));
        resize(200, 200);
    }

    @Override
//! [2]
    protected void paintEvent(QPaintEvent e)
    {
        QColor hourColor = new QColor(127, 0, 127);
        QColor minuteColor = new QColor(0, 127, 127, 191);

        int side = width() < height() ? width() : height();

        QTime time = QTime.currentTime();
//! [2]

//! [3]
        QPainter painter = new QPainter(this);
        painter.setRenderHint(QPainter.RenderHint.Antialiasing);
        painter.translate(width() / 2, height() / 2);
        painter.scale(side / 200.0f, side / 200.0f);
//! [3]

//! [4]
        painter.setPen(new QPen(Qt.PenStyle.NoPen));
        painter.setBrush(hourColor);
//! [4]

//! [5]
        painter.save();
        painter.rotate(30.0f * ((time.hour() + time.minute() / 60.0f)));
        painter.drawConvexPolygon(hourHand);
        painter.restore();
//! [5]

//! [6]
        painter.setPen(hourColor);

        for (int i=0; i<12; ++i) {
            painter.drawLine(88, 0, 96, 0);
            painter.rotate(30.0f);
        }
//! [6]

//! [7]
        painter.setPen(new QPen(Qt.PenStyle.NoPen));
        painter.setBrush(minuteColor);

        painter.save();
        painter.rotate(6.0f * (time.minute() + time.second() / 60.0f));
        painter.drawConvexPolygon(minuteHand);
        painter.restore();
//! [7]

//! [8]
        painter.setPen(minuteColor);

        for (int j=0; j<60; ++j) {
            if ((j % 5) != 0)
                painter.drawLine(92, 0, 96, 0);
            painter.rotate(6.0f);
        }
//! [8]
    }

    @Override
    public QSize sizeHint() {
        return new QSize(200, 200);
    }

    @Override
//! [9]
    public void showEvent(QShowEvent e) {
        m_timer.start(1000);
    }

    @Override
    public void hideEvent(QHideEvent e) {
        m_timer.stop();
//! [9] //! [10]
    }
//! [10]

//! [11]
    static public void main(String args[])
    {
        QApplication.initialize(args);
        AnalogClock w = new AnalogClock();

        w.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
//! [11]
