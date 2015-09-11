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

public class GraphicsItem {

    private static class Heli extends QGraphicsItem {

        public Heli() {
            QConicalGradient g = new QConicalGradient();
            g.setColorAt(0, QColor.white);
            g.setColorAt(0.25, QColor.transparent);
            g.setColorAt(0.50, QColor.white);
            g.setColorAt(0.75, QColor.transparent);
            g.setColorAt(1.00, QColor.white);
            blades = new QBrush(g);

            body = new QPainterPath();
            body.moveTo(-5, 0);
            body.cubicTo(-5, -5,
                         10, -5,
                         20, 0);
            body.cubicTo(10, 5,
                         -5, 5,
                         -5, 0);
        }

        public void paint(QPainter p, QStyleOptionGraphicsItem option, QWidget widget) {
            p.setPen(QColor.black);
            p.setBrush(QColor.red);
            p.drawPath(body);

            p.setPen(QColor.white);
            p.setBrush(blades);
            p.rotate(count * 50);
            p.drawEllipse(-10, -10, 20, 20);

            ++count;
            long ctime = System.currentTimeMillis() - time;
            if (ctime > 2000) {
                double ops = count / (double) ctime;
                System.out.println("average over last 2 seconds is: " + (int) (ops * 1000) + "fps");
                time = System.currentTimeMillis();
                count = 0;
            }
        };

        public void moveAndUpdate() {
            rotate(1);
            translate(-3, 0);
            ensureVisible(bounds);
        }

        public QRectF boundingRect() { return bounds; }

        private QRectF bounds = new QRectF(-10, -10, 30, 20);
        private QPainterPath body;
        private QBrush blades;
        private int count;
        private long time;
    }

    public static void main(String args[]) {
        QApplication.initialize(args);

        QGraphicsScene scene = new QGraphicsScene();

        QPixmap pm = new QPixmap(32, 32);
        pm.fill(new QColor(255, 223, 101));
        QPainter p = new QPainter(pm);
        for (int i=0; i<100; ++i) {
            p.drawPoint((int)(Math.random() * 32), (int) (Math.random() * 32));
        }
        p.end();
        scene.setBackgroundBrush(new QBrush(pm));

        QGraphicsView view = new QGraphicsView(scene);
        view.setRenderHints(QPainter.RenderHint.Antialiasing);
        view.resize(1024, 768);
        view.show();

        //        view.setViewportUpdateMode(QGraphicsView.ViewportUpdateMode.FullViewportUpdate);
//         view.setCacheMode(QGraphicsView.CacheModeFlag.CacheBackground);

        Heli h = new Heli();

        QTimer t = new QTimer();
        t.setInterval(10);
        t.timeout.connect(h, "moveAndUpdate()");
        t.start();

        scene.addItem(h);

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
