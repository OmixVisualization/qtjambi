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

package org.qtjambi.examples.opengl;

import org.qtjambi.examples.QtJambiExample;
import org.qtjambi.qt.QSignalEmitter.Signal0;
import org.qtjambi.qt.core.QLineF;
import org.qtjambi.qt.core.QPointF;
import org.qtjambi.qt.core.QRectF;
import org.qtjambi.qt.core.QTimeLine;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QBrush;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.gui.QKeyEvent;
import org.qtjambi.qt.gui.QLinearGradient;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPalette;
import org.qtjambi.qt.gui.QPen;
import org.qtjambi.qt.gui.QPixmap;
import org.qtjambi.qt.gui.QResizeEvent;
import org.qtjambi.qt.gui.QTransform;
import org.qtjambi.qt.opengl.QGL;
import org.qtjambi.qt.opengl.QGLFormat;
import org.qtjambi.qt.opengl.QGLWidget;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QGraphicsItem;
import org.qtjambi.qt.widgets.QGraphicsItemInterface;
import org.qtjambi.qt.widgets.QGraphicsProxyWidget;
import org.qtjambi.qt.widgets.QGraphicsRectItem;
import org.qtjambi.qt.widgets.QGraphicsScene;
import org.qtjambi.qt.widgets.QGraphicsView;
import org.qtjambi.qt.widgets.QGraphicsView.CacheModeFlag;
import org.qtjambi.qt.widgets.QGraphicsView.ViewportUpdateMode;
import org.qtjambi.qt.widgets.QStyleOptionGraphicsItem;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample (name = "Pad Navigator")
public class PadNavigator extends QGraphicsView {

    private static class RoundRectItem extends QGraphicsRectItem {

        private QColor col;
        private QTimeLine timeLine = new QTimeLine(75);
        private double lastVal = 0;
        private double opa = 1;
        private QGraphicsProxyWidget proxyWidget;
        private QPixmap pix;

        public Signal0 activated = new Signal0();

        public RoundRectItem(QRectF rect, QColor color, QWidget embeddedWidget) {
            super(rect);
            col = color;

            timeLine.valueChanged.connect(this, "updateValue(double)");

            if (embeddedWidget != null) {
                proxyWidget = new QGraphicsProxyWidget(this);
                proxyWidget.setFocusPolicy(Qt.FocusPolicy.StrongFocus);
                proxyWidget.setWidget(embeddedWidget);
            }
        }

        public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {
            QTransform x = painter.worldTransform();

            QLineF unit = x.map(new QLineF(0, 0, 1, 1));
            if (unit.p1().x() > unit.p2().x() || unit.p1().y() > unit.p2().y()) {
                if (proxyWidget != null && proxyWidget.isVisible()) {
                    proxyWidget.hide();
                    proxyWidget.setGeometry(rect());
                }
                return;
            }

            if (proxyWidget != null && !proxyWidget.isVisible()) {
                proxyWidget.show();
                proxyWidget.setFocus(Qt.FocusReason.OtherFocusReason);
            }

            if (proxyWidget != null && !proxyWidget.pos().isNull())
                proxyWidget.setGeometry(boundingRect().adjusted(25, 25, -25, -25));

            painter.setOpacity(roundRectOpacity());
            painter.setPen(new QPen(Qt.PenStyle.NoPen));
            painter.setBrush(new QColor(0, 0, 0, 64));
            painter.drawRoundRect(rect().translated(2, 2));

            if (proxyWidget == null) {
                QLinearGradient gradient = new QLinearGradient(rect().topLeft(), rect().bottomRight());
                gradient.setColorAt(0, col);
                gradient.setColorAt(1, col.darker((int) (200 + lastVal * 50)));
                painter.setBrush(gradient);
            } else {
                painter.setBrush(col);
            }

            painter.setPen(new QPen(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.black), 1));
            painter.drawRoundRect(rect());

            if (pix != null) {
                painter.scale(1.95, 1.95);
                painter.drawPixmap(-pix.width() / 2, -pix.height() / 2, pix);
            }
        }

        public QRectF boundingRect() {
            double penW = 0.5;
            double shadowW = 2.0;
            return rect().adjusted(-penW, -penW, penW + shadowW, penW + shadowW);
        }

        public void setPixmap(QPixmap pixmap) {
            pix = pixmap;
            if (scene() != null && isVisible())
                update(null);
        }

        public final double roundRectOpacity() {
            double opacity = opa;
            if (parentItem() != null)
                opacity += ((RoundRectItem) parentItem()).roundRectOpacity();
            return opacity;
        }

        public final void setRoundRectOpacity(double opacity) {
            opa = opacity;
            update(null);
        }


        public void keyPressEvent(QKeyEvent event) {
            if (event.isAutoRepeat() || event.key() != Qt.Key.Key_Return.value()
                    || (timeLine.state() == QTimeLine.State.Running && timeLine.direction() == QTimeLine.Direction.Forward)) {
                super.keyPressEvent(event);
                return;
            }

            timeLine.stop();
            timeLine.setDirection(QTimeLine.Direction.Forward);
            timeLine.start();
            activated.emit();
        }

        public void keyReleaseEvent(QKeyEvent event) {
            if (event.key() != Qt.Key.Key_Return.value()) {
                super.keyReleaseEvent(event);
                return;
            }

            timeLine.stop();
            timeLine.setDirection(QTimeLine.Direction.Backward);
            timeLine.start();
        }

        public void updateValue(double value) {
            lastVal = value;
            if (proxyWidget == null)
                setTransform(new QTransform().scale(1 - value / 10.0, 1 - value / 10.0), false);
        }

    }

    public PadNavigator() {
    this(3, 3);
    }


    public PadNavigator(int width, int height) {
        this.width = width;
        this.height = height;

        setHorizontalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff);
        setCacheMode(CacheModeFlag.CacheBackground);
        setViewportUpdateMode(ViewportUpdateMode.FullViewportUpdate);
        setRenderHints(QPainter.RenderHint.Antialiasing, QPainter.RenderHint.SmoothPixmapTransform);

        setBackgroundBrush(new QBrush(new QPixmap("classpath:org/qtjambi/images/blue_angle_swirl.jpg")));

        setViewport(new QGLWidget(new QGLFormat(new QGL.FormatOptions(QGL.FormatOption.SampleBuffers))));

        selectionTimeLine = new QTimeLine(150, this);
        flipTimeLine = new QTimeLine(500, this);

        QRectF bounds = new QRectF((-width / 2.0) * 150, (-height / 2.0) * 150, width * 150, height * 150);
        scene = new QGraphicsScene(bounds, this);
        setScene(scene);

        baseItem = new RoundRectItem(bounds, new QColor(226, 255, 92, 64), null);
        baseItem.setRoundRectOpacity(1);
        scene.addItem(baseItem);

        QWidget embed = new QWidget();
        ui = new Ui_BackSide();
        ui.setupUi(embed);
        ui.hostName.setFocus(Qt.FocusReason.OtherFocusReason);

        backItem = new RoundRectItem(bounds, palette().color(QPalette.ColorRole.Window), embed);
        backItem.setTransform(new QTransform().rotate(180, Qt.Axis.YAxis), false);
        backItem.setParentItem(baseItem);

        selectionItem = new RoundRectItem(new QRectF(-60, -60, 120, 120), new QColor(org.qtjambi.qt.core.Qt.GlobalColor.gray), null);
        selectionItem.setParentItem(baseItem);
        selectionItem.setZValue(-1);
        selectionItem.setPos(posForLocation(0, 0));
        startPos = selectionItem.pos();

        grid = new QGraphicsItemInterface[height][width];

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                RoundRectItem item = new RoundRectItem(new QRectF(-54, -54, 108, 108),
                                                       new QColor(214, 240, 110, 128),
                                                       null);
                item.setPos(posForLocation(x, y));

                item.setParentItem(baseItem);
                item.setFlag(QGraphicsItem.GraphicsItemFlag.ItemIsFocusable, true);

                grid[y][x] = item;

                switch ((int) (Math.random() * 9)) {
                case 0: item.setPixmap(new QPixmap("classpath:org/qtjambi/images/kontact_contacts.png")); break;
                case 1: item.setPixmap(new QPixmap("classpath:org/qtjambi/images/kontact_journal.png")); break;
                case 2: item.setPixmap(new QPixmap("classpath:org/qtjambi/images/kontact_notes.png")); break;
                case 3: item.setPixmap(new QPixmap("classpath:org/qtjambi/images/kopeteavailable.png")); break;
                case 4: item.setPixmap(new QPixmap("classpath:org/qtjambi/images/metacontact_online.png")); break;
                case 5: item.setPixmap(new QPixmap("classpath:org/qtjambi/images/minitools.png")); break;
                case 6: item.setPixmap(new QPixmap("classpath:org/qtjambi/images/kontact_journal.png")); break;
                case 7: item.setPixmap(new QPixmap("classpath:org/qtjambi/images/kontact_contacts.png")); break;
                case 8: item.setPixmap(new QPixmap("classpath:org/qtjambi/images/kopeteavailable.png")); break;
                default:
                    break;
                }

                item.activated.connect(this, "flip()");
            }
        }

        grid[0][0].setFocus(Qt.FocusReason.OtherFocusReason);

        backItem.activated.connect(this, "flip()");
        selectionTimeLine.valueChanged.connect(this, "updateSelectionStep(double)");
        flipTimeLine.valueChanged.connect(this, "updateFlipStep(double)");

        updateSelectionStep(0);

        setWindowTitle(tr("Pad Navigator Example"));
        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));
    }

    public void keyPressEvent(QKeyEvent event) {
        if (event.key() == Qt.Key.Key_Return.value() || flipped) {
            super.keyPressEvent(event);
            return;
        }

        selectedX = (selectedX + width
                    + (event.key() == Qt.Key.Key_Right.value() ? 1 : 0)
                    - (event.key() == Qt.Key.Key_Left.value() ? 1 : 0)) % width;

        selectedY = (selectedY + height
                    + (event.key() == Qt.Key.Key_Down.value() ? 1 : 0)
                    - (event.key() == Qt.Key.Key_Up.value() ? 1 : 0)) % height;

        grid[selectedY][selectedX].setFocus(Qt.FocusReason.OtherFocusReason);

        selectionTimeLine.stop();
        startPos = selectionItem.pos();
        endPos = posForLocation(selectedX, selectedY);
        selectionTimeLine.start();
    }

    public void resizeEvent(QResizeEvent event) {
        super.resizeEvent(event);
        fitInView(scene.sceneRect(), Qt.AspectRatioMode.KeepAspectRatio);
    }

    public void updateSelectionStep(double val) {
        double nx = startPos.x() + (endPos.x() - startPos.x()) * val;
        double ny = startPos.y() + (endPos.y() - startPos.y()) * val;

        QPointF newPos = new QPointF(nx, ny);
        selectionItem.setPos(newPos);

        QTransform transform = new QTransform();
        yrot = newPos.x() / 6.0;
        xrot = newPos.y() / 6.0;
        transform.rotate(newPos.x() / 6.0, Qt.Axis.YAxis);
        transform.rotate(newPos.y() / 6.0, Qt.Axis.XAxis);
        baseItem.setTransform(transform, false);
    }

    public void updateFlipStep(double val) {
        double finalxrot = xrot - xrot * val;
        double finalyrot;
        if (flipLeft)
            finalyrot = yrot - yrot * val - 180 * val;
        else
            finalyrot = yrot - yrot * val + 180 * val;

        QTransform transform = new QTransform();
        transform.rotate(finalyrot, Qt.Axis.YAxis);
        transform.rotate(finalxrot, Qt.Axis.XAxis);
        double scale = 1 - Math.sin(3.14 * val) * 0.3;
        transform.scale(scale, scale);
        baseItem.setTransform(transform, false);
    }

    public void flip() {
        if (flipTimeLine.state() == QTimeLine.State.Running)
            return;

        if (flipTimeLine.currentValue() == 0) {
            flipTimeLine.setDirection(QTimeLine.Direction.Forward);
            flipTimeLine.start();
            flipped = true;
            flipLeft = selectionItem.pos().x() < 0;
        } else {
            flipTimeLine.setDirection(QTimeLine.Direction.Backward);
            flipTimeLine.start();
            flipped = false;
            setFocus();
            selectionItem.setFocus(Qt.FocusReason.OtherFocusReason);
        }
    }

    public QPointF posForLocation(int x, int y) {
        return new QPointF(x * 150 - (width - 1) * 75,
                           y * 150 - (height - 1) * 75);
    }



    private QGraphicsScene scene;
    private RoundRectItem selectionItem;
    private RoundRectItem baseItem;
    private RoundRectItem backItem;


    private QTimeLine selectionTimeLine;
    private QTimeLine flipTimeLine;

    private int selectedX = 0;
    private int selectedY = 0;
    private QGraphicsItemInterface grid[][];

    private QPointF startPos;
    private QPointF endPos = new QPointF();
    private double xrot, yrot;

    private int width;
    private int height;

    private boolean flipped = false;
    private boolean flipLeft = true;

    private Ui_BackSide ui;



    public static void main(String args[]) {
        QApplication.initialize(args);

        PadNavigator pad = new PadNavigator(3, 3);
        pad.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
