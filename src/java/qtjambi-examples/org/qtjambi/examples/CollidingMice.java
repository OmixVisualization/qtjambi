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

import java.util.List;
import java.util.Random;

import org.qtjambi.qt.core.QLineF;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.core.QPointF;
import org.qtjambi.qt.core.QRectF;
import org.qtjambi.qt.core.QTimer;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QBrush;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPainterPath;
import org.qtjambi.qt.gui.QPixmap;
import org.qtjambi.qt.gui.QPolygonF;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QGraphicsItem;
import org.qtjambi.qt.widgets.QGraphicsItemInterface;
import org.qtjambi.qt.widgets.QGraphicsScene;
import org.qtjambi.qt.widgets.QGraphicsView;
import org.qtjambi.qt.widgets.QGridLayout;
import org.qtjambi.qt.widgets.QStyleOptionGraphicsItem;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "Colliding Mice")
//! [0]
public class CollidingMice extends QWidget {

    static final int MOUSE_COUNT = 7;
//! [0]
    private Signal0 stop = new Signal0();
    private Signal0 start = new Signal0();

//! [1]
    public static void main(String args[]) {
        QApplication.initialize(args);

        CollidingMice collidingMice = new CollidingMice(null);
        collidingMice.show();
        QApplication.execStatic();
        QApplication.shutdown();
    }
//! [1]

//! [2]
    public CollidingMice(QWidget parent) {
        super(parent);

        QGraphicsScene scene = new QGraphicsScene(this);
//! [2] //! [3]
        scene.setSceneRect(-300, -300, 600, 600);
//! [3] //! [4]
        scene.setItemIndexMethod(QGraphicsScene.ItemIndexMethod.NoIndex);
//! [4]

//! [5]
        for (int i = 0; i < MOUSE_COUNT; ++i) {
            Mouse mouse = new Mouse(this);
            mouse.setPos(Math.sin((i * 6.28) / MOUSE_COUNT) * 200,
                         Math.cos((i * 6.28) / MOUSE_COUNT) * 200);
            scene.addItem(mouse);
        }
//! [5]

//! [6]
        QGraphicsView view = new QGraphicsView(scene);
        view.setRenderHint(QPainter.RenderHint.Antialiasing);
        view.setBackgroundBrush(new QBrush(new QPixmap(
//! [6]
                "classpath:org/qtjambi/examples/images/cheese.png")));
//! [7]
        view.setCacheMode(new QGraphicsView.CacheMode(
//! [7]
                QGraphicsView.CacheModeFlag.CacheBackground));
//! [8]
        view.setDragMode(QGraphicsView.DragMode.ScrollHandDrag);
//! [8]
        view.setViewportUpdateMode(QGraphicsView.ViewportUpdateMode.FullViewportUpdate);

//! [9]
        QGridLayout layout = new QGridLayout();
        layout.addWidget(view, 0, 0);
        setLayout(layout);

        setWindowTitle("Colliding Mice");
        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));
        resize(400, 300);
    }
//! [9]

//! [10]
    public class Mouse extends QGraphicsItem {

        double angle = 0;
        double speed = 0;
        double mouseEyeDirection = 0;
        QColor color = null;
        Random generator = new Random();

        static final double TWO_PI = Math.PI * 2;

        public Mouse(QObject parent) {
            color = new QColor(generator.nextInt(256), generator.nextInt(256),
                               generator.nextInt(256));
            setRotation(generator.nextDouble() * 360);
//! [10]

//! [11]
            QTimer timer = new QTimer(CollidingMice.this);
            timer.start(1000/33);
//! [11]
            timer.timeout.connect(this, "move()");
            start.connect(timer, "start()");
            stop.connect(timer, "stop()");
//! [12]
        }
//! [12]

        private double adjust = 0.5;
        private QRectF boundingRect = new QRectF(-20 - adjust, -22 - adjust,
                                                 40 + adjust, 83 + adjust);
        @Override
//! [13]
        public QRectF boundingRect() {
            return boundingRect;
        }
//! [13]

        QPainterPath shape = new QPainterPath();
        {
            shape.addRect(-10, -20, 20, 40);
        }
        @Override
//! [14]
        public QPainterPath shape() {
            return shape;
        }
//! [14]

        QBrush brush = new QBrush(Qt.BrushStyle.SolidPattern);
        QPainterPath tail = new QPainterPath(new QPointF(0, 20));
        {
            tail.cubicTo(-5, 22, -5, 22, 0, 25);
            tail.cubicTo(5, 27, 5, 32, 0, 30);
            tail.cubicTo(-5, 32, -5, 42, 0, 35);
        }

        private QRectF pupilRect1 = new QRectF(-8 + mouseEyeDirection, -17, 4, 4);
        private QRectF pupilRect2 = new QRectF(4 + mouseEyeDirection, -17, 4, 4);

        @Override
//! [15]
        public void paint(QPainter painter,
                          QStyleOptionGraphicsItem styleOptionGraphicsItem,
                          QWidget widget) {

            // Body
            painter.setBrush(color);
            painter.drawEllipse(-10, -20, 20, 40);

            // Eyes
            brush.setColor(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.white));
            painter.setBrush(brush);
            painter.drawEllipse(-10, -17, 8, 8);
            painter.drawEllipse(2, -17, 8, 8);

            // Nose
            brush.setColor(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.black));
            painter.setBrush(brush);
            painter.drawEllipse(-2, -22, 4, 4);

            // Pupils
            painter.drawEllipse(pupilRect1);
            painter.drawEllipse(pupilRect2);

            // Ears
            if (scene().collidingItems(this).isEmpty())
                brush.setColor(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.darkYellow));
            else
                brush.setColor(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.red));
            painter.setBrush(brush);

            painter.drawEllipse(-17, -12, 16, 16);
            painter.drawEllipse(1, -12, 16, 16);

            // Tail
            painter.setBrush(new QBrush(Qt.BrushStyle.NoBrush));
            painter.drawPath(tail);
        }
//! [15]

        private QPolygonF polygon = new QPolygonF();
        private QPointF origo = new QPointF(0, 0);
//! [16]
        public void move() {
//! [16]
            // Don't move too far away
//! [17]
            QLineF lineToCenter = new QLineF(origo,
                                             mapFromScene(0, 0));
            if (lineToCenter.length() > 150) {
                double angleToCenter = Math.acos(lineToCenter.dx()
                                                 / lineToCenter.length());
                if (lineToCenter.dy() < 0)
                    angleToCenter = TWO_PI - angleToCenter;
                angleToCenter = normalizeAngle((Math.PI - angleToCenter)
                                               + Math.PI / 2);

                if (angleToCenter < Math.PI && angleToCenter > Math.PI / 4) {
                    // Rotate left
                    angle += (angle < -Math.PI / 2) ? 0.25 : -0.25;
                } else if (angleToCenter >= Math.PI
                           && angleToCenter < (Math.PI + Math.PI / 2
                                               + Math.PI / 4)) {
                    // Rotate right
                    angle += (angle < Math.PI / 2) ? 0.25 : -0.25;
                }
            } else if (Math.sin(angle) < 0) {
                angle += 0.25;
            } else if (Math.sin(angle) > 0) {
                angle -= 0.25;
            }

//! [17]
            // Try not to crash with any other mice

//! [18]
            polygon.clear();
            polygon.append(mapToScene(0, 0));
            polygon.append(mapToScene(-30, -50));
            polygon.append(mapToScene(30, -50));

            List<QGraphicsItemInterface> dangerMice = scene().items(polygon);
            for (QGraphicsItemInterface item : dangerMice) {
                if (item == this)
                    continue;

                QLineF lineToMouse = new QLineF(origo,
                                                mapFromItem(item, 0, 0));
                double angleToMouse = Math.acos(lineToMouse.dx()
                                                / lineToMouse.length());
                if (lineToMouse.dy() < 0)
                    angleToMouse = TWO_PI - angleToMouse;
                angleToMouse = normalizeAngle((Math.PI - angleToMouse)
                                              + Math.PI / 2);

                if (angleToMouse >= 0 && angleToMouse < (Math.PI / 2)) {
                    // Rotate right
                    angle += 0.5;
                } else if (angleToMouse <= TWO_PI
                           && angleToMouse > (TWO_PI - Math.PI / 2)) {
                    // Rotate left
                    angle -= 0.5;
                }
            }

//! [18]
            // Add some random movement
//! [19]
            if (dangerMice.size() < 1 && generator.nextDouble() < 0.1) {
                if (generator.nextDouble() > 0.5)
                    angle += generator.nextDouble() / 5;
                else
                    angle -= generator.nextDouble() / 5;
            }
//! [19]

//! [20]
            speed += (-50 + generator.nextDouble() * 100) / 100.0;

            double dx = Math.sin(angle) * 10;
            mouseEyeDirection = (Math.abs(dx / 5) < 1) ? 0 : dx / 5;

            setRotation(dx);
            setPos(mapToParent(0, -(3 + Math.sin(speed) * 3)));
//! [20] //! [21]
        }
//! [21]

//! [22]
        private double normalizeAngle(double angle) {
            while (angle < 0)
                angle += TWO_PI;
            while (angle > TWO_PI)
                angle -= TWO_PI;
            return angle;
//! [22] //! [23]
        }
//! [23]
    }
//! [24]
}
//! [24]
