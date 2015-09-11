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

package org.qtjambi.demos.opengl;

import java.util.List;
import java.util.Vector;

import org.qtjambi.examples.QtJambiExample;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.core.Qt.ConnectionType;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.opengl.*;
import org.qtjambi.qt.printsupport.QPrintDialog;
import org.qtjambi.qt.printsupport.QPrinter;
import org.qtjambi.qt.widgets.*;

@QtJambiExample(name = "40000 Chips")
public class Chips extends QWidget {

    QGraphicsScene scene;
    QSplitter h1Splitter;
    QSplitter h2Splitter;
    Signal0 populateScene = new Signal0();

    public Chips(QWidget parent) {
        scene = new QGraphicsScene();

        h1Splitter = new QSplitter();
        h2Splitter = new QSplitter();

        QSplitter vSplitter = new QSplitter();
        vSplitter.setOrientation(Qt.Orientation.Vertical);
        vSplitter.addWidget(h1Splitter);
        vSplitter.addWidget(h2Splitter);

        View view = new View("Top left view");
        view.view().setScene(scene);
        h1Splitter.addWidget(view);

        view = new View("Top right view");
        view.view().setScene(scene);
        h1Splitter.addWidget(view);

        view = new View("Bottom left view");
        view.view().setScene(scene);
        h2Splitter.addWidget(view);

        view = new View("Bottom right view");
        view.view().setScene(scene);
        h2Splitter.addWidget(view);

        QHBoxLayout layout = new QHBoxLayout();
        layout.addWidget(vSplitter);
        setLayout(layout);

        setWindowTitle(tr("Chip Demo"));
        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));

        populateScene.connect(this, "populateScene()", ConnectionType.QueuedConnection);
        populateScene.emit();
    }

    private void populateScene() {
        QImage image = new QImage("classpath:/org/qtjambi/images/qt4logo.png");

        // Populate scene
        int xx = 0;
        int nitems = 0;
        for (int i = -11000; i < 11000; i += 110) {
            ++xx;
            int yy = 0;
            for (int j = -7000; j < 7000; j += 70) {
                ++yy;
                double x = (i + 11000) / 22000.0;
                double y = (j + 7000) / 14000.0;

                QColor color = new QColor(image.pixel((int) (image.width() * x), (int) (image.height() * y)));
                QGraphicsItem item = new Chip(color, xx, yy);
                item.setPos(new QPointF(i, j));
                scene.addItem(item);

                ++nitems;
            }
            QApplication.processEvents();
        }
    }

    public static void main(String args[]) {
        QApplication.initialize(args);

        Chips w = new Chips(null);
        w.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }

    class Chip extends QGraphicsItem {

        int x, y;
        QColor color;
        List<QPointF> stuff = new Vector<QPointF>();

        Chip(final QColor color, int x, int y) {
            this.x = x;
            this.y = y;
            this.color = color;
            setZValue((x + y) % 2);

            setFlags(new GraphicsItemFlags(GraphicsItemFlag.ItemIsSelectable, GraphicsItemFlag.ItemIsMovable));
            setAcceptHoverEvents(true);
        }

        @Override
        public QRectF boundingRect() {
            return new QRectF(0, 0, 110, 70);
        }

        @Override
        public QPainterPath shape() {
            QPainterPath path = new QPainterPath();
            path.addRect(14, 14, 82, 42);
            return path;
        }

        @Override
        public void paint(QPainter painter, final QStyleOptionGraphicsItem option, QWidget widget) {

            QColor fillColor = (option.state().isSet(QStyle.StateFlag.State_Selected)) ? color.darker(150) : color;
            if (option.state().isSet(QStyle.StateFlag.State_MouseOver))
                fillColor = fillColor.lighter(125);

            if (option.levelOfDetail() < 0.2) {
                if (option.levelOfDetail() < 0.125) {
                    painter.fillRect(new QRectF(0, 0, 110, 70), new QBrush(fillColor));
                    return;
                }

                painter.setPen(new QPen(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.black), 0));
                painter.setBrush(fillColor);
                painter.drawRect(13, 13, 97, 57);
                return;
            }

            QPen oldPen = painter.pen();
            QPen pen = oldPen;
            int width = 0;
            if (option.state().isSet(QStyle.StateFlag.State_Selected))
                width += 2;

            pen.setWidth(width);
            painter.setBrush(new QBrush(fillColor.darker(option.state().isSet(QStyle.StateFlag.State_Sunken) ? 120 : 100)));

            painter.drawRect(new QRect(14, 14, 79, 39));
            if (option.levelOfDetail() >= 1) {
                painter.setPen(new QPen(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.gray), 1));
                painter.drawLine(15, 54, 94, 54);
                painter.drawLine(94, 53, 94, 15);
                painter.setPen(new QPen(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.black), 0));
            }

            // Draw text
            if (option.levelOfDetail() >= 2) {
                QFont font = new QFont("Times", 10);
                font.setStyleStrategy(QFont.StyleStrategy.ForceOutline);
                painter.setFont(font);
                painter.setRenderHint(QPainter.RenderHint.TextAntialiasing, false);
                painter.save();
                painter.scale(0.1, 0.1);
                painter.drawText(170, 180, String.format("Model: VSC-2000 (Very Small Chip) at %1$s %2$s", x, y));
                painter.drawText(170, 200, String.format("Serial number: DLWR-WEER-123L-ZZ33-SDSJ", x, y));
                painter.drawText(170, 220, String.format("Manufacturer: Chip Manufacturer", x, y));
                painter.restore();
            }

            // Draw lines
            List<QLineF> lines = new Vector<QLineF>(36);
            if (option.levelOfDetail() >= 0.5) {
                for (int i = 0; i <= 10; i += (option.levelOfDetail() > 0.5 ? 1 : 2)) {
                    lines.add(new QLineF(18 + 7 * i, 13, 18 + 7 * i, 5));
                    lines.add(new QLineF(18 + 7 * i, 54, 18 + 7 * i, 62));
                }
                for (int i = 0; i <= 6; i += (option.levelOfDetail() > 0.5 ? 1 : 2)) {
                    lines.add(new QLineF(5, 18 + i * 5, 13, 18 + i * 5));
                    lines.add(new QLineF(94, 18 + i * 5, 102, 18 + i * 5));
                }
            }
            if (option.levelOfDetail() >= 0.4) {
                lines.add(new QLineF(25, 35, 35, 35));
                lines.add(new QLineF(35, 30, 35, 40));
                lines.add(new QLineF(35, 30, 45, 35));
                lines.add(new QLineF(35, 40, 45, 35));
                lines.add(new QLineF(45, 30, 45, 40));
                lines.add(new QLineF(45, 35, 55, 35));
            }
            painter.drawLinesF(lines);

            // Draw red ink
            if (stuff.size() > 1) {
                painter.setPen(new QPen(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.red), 1, Qt.PenStyle.SolidLine, Qt.PenCapStyle.RoundCap, Qt.PenJoinStyle.RoundJoin));
                painter.setBrush(new QBrush(Qt.BrushStyle.NoBrush));
                QPainterPath path = new QPainterPath();
                path.moveTo(stuff.get(0));
                for (int i = 1; i < stuff.size(); ++i)
                    path.lineTo(stuff.get(i));
                painter.drawPath(path);
            }
        }

        @Override
        public void mousePressEvent(QGraphicsSceneMouseEvent event) {
            super.mousePressEvent(event);
            update();
        }

        @Override
        public void mouseMoveEvent(QGraphicsSceneMouseEvent event) {
            if (event.modifiers().isSet(Qt.KeyboardModifier.ShiftModifier)) {
                stuff.add(event.pos());
                update();
                return;
            }
            super.mouseMoveEvent(event);
        }

        @Override
        public void mouseReleaseEvent(QGraphicsSceneMouseEvent event) {
            super.mouseReleaseEvent(event);
            update();
        }
    }

    class View extends QFrame {

        QGraphicsView graphicsView;
        QLabel label;
        QToolButton openGlButton;
        QToolButton antialiasButton;
        QToolButton printButton;
        QToolButton resetButton;
        QSlider zoomSlider;
        QSlider rotateSlider;

        View(final String name) {
            this(name, null);
        }

        View(final String name, QWidget parent)

        {
            super(parent);
            setFrameStyle(QFrame.Shadow.Sunken.value() | QFrame.Shape.StyledPanel.value());
            graphicsView = new QGraphicsView();
            graphicsView.setRenderHint(QPainter.RenderHint.Antialiasing, false);
            graphicsView.setDragMode(QGraphicsView.DragMode.RubberBandDrag);
            graphicsView.setViewportUpdateMode(QGraphicsView.ViewportUpdateMode.SmartViewportUpdate);

            int size = style().pixelMetric(QStyle.PixelMetric.PM_ToolBarIconSize);
            QSize iconSize = new QSize(size, size);

            QToolButton zoomInIcon = new QToolButton();
            zoomInIcon.setAutoRepeat(true);
            zoomInIcon.setAutoRepeatInterval(33);
            zoomInIcon.setAutoRepeatDelay(0);
            zoomInIcon.setIcon(new QIcon(new QPixmap("classpath:/org/qtjambi/images/zoomin.png")));
            zoomInIcon.setIconSize(iconSize);
            QToolButton zoomOutIcon = new QToolButton();
            zoomOutIcon.setAutoRepeat(true);
            zoomOutIcon.setAutoRepeatInterval(33);
            zoomOutIcon.setAutoRepeatDelay(0);
            zoomOutIcon.setIcon(new QIcon(new QPixmap("classpath:/org/qtjambi/images/zoomout.png")));
            zoomOutIcon.setIconSize(iconSize);
            zoomSlider = new QSlider();
            zoomSlider.setMinimum(0);
            zoomSlider.setMaximum(500);
            zoomSlider.setValue(250);
            zoomSlider.setTickPosition(QSlider.TickPosition.TicksBelow);

            // Zoom slider layout
            QVBoxLayout zoomSliderLayout = new QVBoxLayout();
            zoomSliderLayout.addWidget(zoomInIcon);
            zoomSliderLayout.addWidget(zoomSlider);
            zoomSliderLayout.addWidget(zoomOutIcon);

            QToolButton rotateLeftIcon = new QToolButton();
            rotateLeftIcon.setIcon(new QIcon(new QPixmap("classpath:/org/qtjambi/images/rotateleft.png")));
            rotateLeftIcon.setIconSize(iconSize);
            QToolButton rotateRightIcon = new QToolButton();
            rotateRightIcon.setIcon(new QIcon(new QPixmap("classpath:/org/qtjambi/images/rotateright.png")));
            rotateRightIcon.setIconSize(iconSize);
            rotateSlider = new QSlider();
            rotateSlider.setOrientation(Qt.Orientation.Horizontal);
            rotateSlider.setMinimum(-360);
            rotateSlider.setMaximum(360);
            rotateSlider.setValue(0);
            rotateSlider.setTickPosition(QSlider.TickPosition.TicksBelow);

            // Rotate slider layout
            QHBoxLayout rotateSliderLayout = new QHBoxLayout();
            rotateSliderLayout.addWidget(rotateLeftIcon);
            rotateSliderLayout.addWidget(rotateSlider);
            rotateSliderLayout.addWidget(rotateRightIcon);

            resetButton = new QToolButton();
            resetButton.setText(tr("0"));
            resetButton.setEnabled(false);

            // Label layout
            QHBoxLayout labelLayout = new QHBoxLayout();
            label = new QLabel(name);
            antialiasButton = new QToolButton();
            antialiasButton.setText(tr("Antialiasing"));
            antialiasButton.setCheckable(true);
            antialiasButton.setChecked(false);
            openGlButton = new QToolButton();
            openGlButton.setText(tr("OpenGL"));
            openGlButton.setCheckable(true);
            openGlButton.setEnabled(QGLFormat.hasOpenGL());

            printButton = new QToolButton();
            printButton.setIcon(new QIcon(new QPixmap("classpath:/org/qtjambi/images/" + "fileprint.png")));

            labelLayout.addWidget(label);
            labelLayout.addStretch();
            labelLayout.addWidget(antialiasButton);
            labelLayout.addWidget(openGlButton);
            labelLayout.addWidget(printButton);

            QGridLayout topLayout = new QGridLayout();
            topLayout.addLayout(labelLayout, 0, 0);
            topLayout.addWidget(graphicsView, 1, 0);
            topLayout.addLayout(zoomSliderLayout, 1, 1);
            topLayout.addLayout(rotateSliderLayout, 2, 0);
            topLayout.addWidget(resetButton, 2, 1);
            setLayout(topLayout);

            resetButton.clicked.connect(this, "resetView()");
            zoomSlider.valueChanged.connect(this, "setupMatrix()");
            rotateSlider.valueChanged.connect(this, "setupMatrix()");
            graphicsView.verticalScrollBar().valueChanged.connect(this, "setResetButtonEnabled()");
            graphicsView.horizontalScrollBar().valueChanged.connect(this, "setResetButtonEnabled()");
            antialiasButton.toggled.connect(this, "toggleAntialiasing()");
            openGlButton.toggled.connect(this, "toggleOpenGL()");
            rotateLeftIcon.clicked.connect(this, "rotateLeft()");
            rotateRightIcon.clicked.connect(this, "rotateRight()");
            zoomInIcon.clicked.connect(this, "zoomIn()");
            zoomOutIcon.clicked.connect(this, "zoomOut()");
            printButton.clicked.connect(this, "print()");

            setupMatrix();
        }

        QGraphicsView view() {
            return graphicsView;
        }

        void resetView() {
            zoomSlider.setValue(250);
            rotateSlider.setValue(0);
            setupMatrix();
            graphicsView.ensureVisible(new QRectF(0, 0, 0, 0));

            resetButton.setEnabled(false);
        }

        void setResetButtonEnabled() {
            resetButton.setEnabled(true);
        }

        void setupMatrix() {
            double scale = Math.pow(2.0, (zoomSlider.value() - 250) / 50.0);

            QTransform matrix = new QTransform();
            matrix.scale(scale, scale);
            matrix.rotate(rotateSlider.value());

            graphicsView.setTransform(matrix);
            setResetButtonEnabled();
        }

        void toggleOpenGL() {
            graphicsView.setViewport(openGlButton.isChecked() ? new QGLWidget(new QGLFormat(new QGL.FormatOptions(QGL.FormatOption.SampleBuffers)))
                    : new QWidget());

        }

        void toggleAntialiasing() {
            graphicsView.setRenderHint(QPainter.RenderHint.Antialiasing, antialiasButton.isChecked());
        }

        void print() {
            QPrinter printer = new QPrinter();
            QPrintDialog dialog = new QPrintDialog(printer, this);
            if (dialog.exec() == QDialog.DialogCode.Accepted.value()) {
                QPainter painter = new QPainter(printer);
                graphicsView.render(painter);
                painter.end();
            }
        }

        void zoomIn() {
            zoomSlider.setValue(zoomSlider.value() + 1);
        }

        void zoomOut() {
            zoomSlider.setValue(zoomSlider.value() - 1);
        }

        void rotateLeft() {
            rotateSlider.setValue(rotateSlider.value() - 10);
        }

        void rotateRight() {
            rotateSlider.setValue(rotateSlider.value() + 10);
        }
    }
}
