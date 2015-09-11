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

import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

class PaintWidget extends QWidget {

    ArrayList<QPolygon> polygons = new ArrayList<QPolygon>();
    ArrayList<QColor> colors = new ArrayList<QColor>();
    QPolygon current = new QPolygon();
    QColor m_current_color = QColor.red;

    public PaintWidget() {
        this(null);
    }

    public PaintWidget(Qt.WindowFlags flags) {
        super(null, flags);
        this.setGeometry(new QRect(100, 100, 32 * 16 + 2, 480));
    }

    @Override
    public void mousePressEvent(QMouseEvent e) {
        current = new QPolygon();
    }

    @Override
    public void mouseMoveEvent(QMouseEvent e) {
        current.append(new QPoint(e.x(), e.y()));
        update();
    }

    @Override
    public void contextMenuEvent(QContextMenuEvent e)
    {
        QMenu menu = new QMenu(this);
        QAction act = menu.addAction("Oh, nothing");
        act.triggered.connect(menu, "close()");
        act = menu.addAction("Quit");
        act.triggered.connect(QApplication.instance(), "quit()");

        menu.exec(e.globalPos());
    }

    @Override
    public void mouseReleaseEvent(QMouseEvent e) {
        polygons.add(current);
        colors.add(m_current_color);
        current = null;
        update();
    }

    public void setColor(QColor c) {
        m_current_color = c;
        update();
    }

    @Override
    public void paintEvent(QPaintEvent e) {
        QPainter p = new QPainter();
        p.begin(this);

        p.setRenderHint(QPainter.RenderHint.Antialiasing);

        p.setBrush(new QBrush(new QColor(255, 255, 255)));
        p.drawRect(0, 0, width() - 1, height() - 1);

        p.setPen(Qt.PenStyle.NoPen);
        Iterator<QPolygon> it = polygons.iterator();
        Iterator<QColor> color_it = colors.iterator();
        while (it.hasNext()) {
            QPolygon stored_polygon = it.next();
            QColor stored_color = color_it.next();
            p.setPen(stored_color);
            stored_color = new QColor(stored_color.red(), stored_color.green(), stored_color.blue(), 63);
            p.setBrush(new QBrush(stored_color));
            p.drawPolygon(stored_polygon);
        }

        if (current != null) {
            p.setPen(m_current_color);
            p.drawPolyline(current);
        }

        p.setPen(Qt.PenStyle.NoPen);
        p.setBrush(new QBrush(m_current_color));

        p.drawRect(1, height() - 11, width() - 2, 10);
    p.end();
    }

    int foo;

    /*
    private static void dumpWidget(QWidget w) {
        QRect r = w.geometry();
        System.out.println(w.getClass().getName() + " x: " + r.x() + ", y: "
                + r.y() + ", w: " + r.width() + ", h: " + r.height());
    }
    */

    public static void main(String args[]) {
        QApplication.initialize(args);
        PaintWidget pw = new PaintWidget();
        pw.show();
        QApplication.execStatic();
        QApplication.shutdown();
    }
}
