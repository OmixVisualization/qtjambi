/****************************************************************************
 **
 **  (C) 1992-2009 Nokia. All rights reserved.
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
import org.qtjambi.qt.core.Qt.MouseButton;
import org.qtjambi.qt.core.Qt.WindowFlags;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QMouseEvent;
import org.qtjambi.qt.gui.QPaintEvent;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPen;
import org.qtjambi.qt.gui.QPolygon;
import org.qtjambi.qt.gui.QRegion;
import org.qtjambi.qt.gui.QResizeEvent;
import org.qtjambi.qt.widgets.QAction;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "Shaped Clock")
//! [0]
public class ShapedClock extends QWidget {
//! [0]

//! [1]
    public static void main(String args[]) {
        QApplication.initialize(args);
        ShapedClock shapedClock = new ShapedClock(null);
        shapedClock.show();
        QApplication.execStatic();
        QApplication.shutdown();
    }
//! [1]

//! [2]
    private QPoint dragPosition = new QPoint();
    private static QPolygon hourHand;
    private static QPolygon minuteHand;

    public ShapedClock(QWidget parent) {
        super(parent, new WindowFlags(Qt.WindowType.FramelessWindowHint));
        QTimer timer = new QTimer(this);
        timer.timeout.connect(this, "update()");
        timer.start(1000);

        hourHand = new QPolygon();
        hourHand.append(new QPoint(7, 8));
        hourHand.append(new QPoint(-7, 8));
        hourHand.append(new QPoint(0, -40));

        minuteHand = new QPolygon();
        minuteHand.append(new QPoint(7, 8));
        minuteHand.append(new QPoint(-7, 8));
        minuteHand.append(new QPoint(0, -70));

        setWindowTitle(tr("Shaped Analog Clock"));

        setContextMenuPolicy(Qt.ContextMenuPolicy.ActionsContextMenu);
        QAction quitAction = new QAction(tr("E&xit"), this);
        quitAction.setShortcut("Ctrl+Q");
        quitAction.triggered.connect(this, "close()");
        addAction(quitAction);

        setToolTip(tr("Drag the clock with the left mouse button.\n" +
                      "Use the right mouse button to open a context menu."));
    }
//! [2]

    @Override
//! [3]
    public void mousePressEvent(QMouseEvent event) {
        if (event.button() == MouseButton.LeftButton) {
            QPoint topLeft = frameGeometry().topLeft();
            dragPosition.setX(event.globalPos().x() - topLeft.x());
            dragPosition.setY(event.globalPos().y() - topLeft.y());
            event.accept();
        }
    }
//! [3]

    @Override
//! [4]
    public void mouseMoveEvent(QMouseEvent event) {
        if (event.buttons().isSet(MouseButton.LeftButton)) {
            move(new QPoint(event.globalPos().x() - dragPosition.x(),
                            event.globalPos().y() - dragPosition.y()));
            event.accept();
        }
    }
//! [4]

    @Override
//! [5]
    public void paintEvent(QPaintEvent event) {

        QColor hourColor = new QColor(127, 0, 127);
        QColor minuteColor = new QColor(0, 127, 127, 191);

        int side = Math.min(width(), height());
        QTime time = QTime.currentTime();

        QPainter painter = new QPainter(this);
        painter.setRenderHint(QPainter.RenderHint.Antialiasing);
        painter.translate(width() / 2, height() / 2);
        painter.scale(side / 200.0, side / 200.0);

        painter.setPen(new QPen(Qt.PenStyle.NoPen));
        painter.setBrush(hourColor);

        painter.save();
        painter.rotate(30.0 * ((time.hour() + time.minute() / 60.0)));
        painter.drawConvexPolygon(hourHand);
        painter.restore();

        painter.setPen(hourColor);

        for (int i = 0; i < 12; ++i) {
            painter.drawLine(88, 0, 96, 0);
            painter.rotate(30.0);
        }

        painter.setPen(new QPen(Qt.PenStyle.NoPen));
        painter.setBrush(minuteColor);

        painter.save();
        painter.rotate(6.0 * (time.minute() + time.second() / 60.0));
        painter.drawConvexPolygon(minuteHand);
        painter.restore();

        painter.setPen(minuteColor);

        for (int j = 0; j < 60; ++j) {
            if ((j % 5) != 0)
                painter.drawLine(92, 0, 96, 0);
            painter.rotate(6.0);
        }
    }
//! [5]

    @Override
//! [6]
    public void resizeEvent(QResizeEvent event) {
        int side = Math.min(width(), height());
        QRegion maskedRegion;
        maskedRegion = new QRegion((width() - side) / 2, (height() - side) / 2,
                                    side, side, QRegion.RegionType.Ellipse);
        setMask(maskedRegion);
    }
//! [6]

    @Override
//! [7]
    public QSize sizeHint() {
        return new QSize(100, 100);
    }
//! [7] //! [8]
}
//! [8]
