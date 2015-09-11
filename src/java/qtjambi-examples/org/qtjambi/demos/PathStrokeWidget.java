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
package org.qtjambi.demos;

import java.util.Iterator;
import java.util.List;
import java.util.Vector;

import org.qtjambi.examples.QtJambiExample;
import org.qtjambi.qt.core.QBasicTimer;
import org.qtjambi.qt.core.QLineF;
import org.qtjambi.qt.core.QPointF;
import org.qtjambi.qt.core.QRectF;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.QTimerEvent;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QBrush;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.gui.QMouseEvent;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPainterPath;
import org.qtjambi.qt.gui.QPainterPathStroker;
import org.qtjambi.qt.gui.QPalette;
import org.qtjambi.qt.gui.QPen;
import org.qtjambi.qt.gui.QPixmap;
import org.qtjambi.qt.gui.QPolygonF;
import org.qtjambi.qt.gui.QTransform;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QGroupBox;
import org.qtjambi.qt.widgets.QHBoxLayout;
import org.qtjambi.qt.widgets.QPushButton;
import org.qtjambi.qt.widgets.QRadioButton;
import org.qtjambi.qt.widgets.QSizePolicy;
import org.qtjambi.qt.widgets.QSlider;
import org.qtjambi.qt.widgets.QVBoxLayout;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "Path Stroke")
public class PathStrokeWidget extends QWidget {

    public static void main(String args[]) {
        QApplication.initialize(args);

        PathStrokeWidget pathStrokeWidget = new PathStrokeWidget();
        pathStrokeWidget.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }

    private PathStrokeRendrer m_renderer;

    public enum PathMode {
        CurveMode, LineMode
    }

    public PathStrokeWidget() {
        setWindowTitle(tr("Path Stroking"));

        // Setting up palette.
        QPalette pal = palette();
        setPalette(pal);

        // Widget finalruction and property setting
        m_renderer = new PathStrokeRendrer(this);

        QGroupBox mainGroup = new QGroupBox(this);
        mainGroup.setFixedWidth(180);
        mainGroup.setTitle("Path Stroking");

        QGroupBox capGroup = new QGroupBox(mainGroup);
        capGroup.setAttribute(Qt.WidgetAttribute.WA_ContentsPropagated);
        QRadioButton flatCap = new QRadioButton(capGroup);
        QRadioButton squareCap = new QRadioButton(capGroup);
        QRadioButton roundCap = new QRadioButton(capGroup);
        capGroup.setTitle("Cap Style");
        flatCap.setText("Flat Cap");
        squareCap.setText("Square Cap");
        roundCap.setText("Round Cap");

        QGroupBox joinGroup = new QGroupBox(mainGroup);
        joinGroup.setAttribute(Qt.WidgetAttribute.WA_ContentsPropagated);
        QRadioButton bevelJoin = new QRadioButton(joinGroup);
        QRadioButton miterJoin = new QRadioButton(joinGroup);
        QRadioButton roundJoin = new QRadioButton(joinGroup);
        joinGroup.setTitle("Join Style");
        bevelJoin.setText("Bevel Join");
        miterJoin.setText("Miter Join");
        roundJoin.setText("Round Join");

        QGroupBox styleGroup = new QGroupBox(mainGroup);
        styleGroup.setAttribute(Qt.WidgetAttribute.WA_ContentsPropagated);
        QRadioButton solidLine = new QRadioButton(styleGroup);
        QRadioButton dashLine = new QRadioButton(styleGroup);
        QRadioButton dotLine = new QRadioButton(styleGroup);
        QRadioButton dashDotLine = new QRadioButton(styleGroup);
        QRadioButton dashDotDotLine = new QRadioButton(styleGroup);
        QRadioButton customDashLine = new QRadioButton(styleGroup);
        styleGroup.setTitle("Pen Style");

        QPixmap line_solid = new QPixmap("classpath:/org/qtjambi/demos/PathStroke/line_solid.png");
        solidLine.setIcon(new QIcon(line_solid));
        solidLine.setIconSize(line_solid.size());
        QPixmap line_dashed = new QPixmap("classpath:/org/qtjambi/demos/PathStroke/line_dashed.png");
        dashLine.setIcon(new QIcon(line_dashed));
        dashLine.setIconSize(line_dashed.size());
        QPixmap line_dotted = new QPixmap("classpath:/org/qtjambi/demos/PathStroke/line_dotted.png");
        dotLine.setIcon(new QIcon(line_dotted));
        dotLine.setIconSize(line_dotted.size());
        QPixmap line_dash_dot = new QPixmap("classpath:/org/qtjambi/demos/PathStroke/line_dash_dot.png");
        dashDotLine.setIcon(new QIcon(line_dash_dot));
        dashDotLine.setIconSize(line_dash_dot.size());
        QPixmap line_dash_dot_dot = new QPixmap("classpath:/org/qtjambi/demos/PathStroke/line_dash_dot_dot.png");
        dashDotDotLine.setIcon(new QIcon(line_dash_dot_dot));
        dashDotDotLine.setIconSize(line_dash_dot_dot.size());
        customDashLine.setText("Custom Style");

        int fixedHeight = bevelJoin.sizeHint().height();
        solidLine.setFixedHeight(fixedHeight);
        dashLine.setFixedHeight(fixedHeight);
        dotLine.setFixedHeight(fixedHeight);
        dashDotLine.setFixedHeight(fixedHeight);
        dashDotDotLine.setFixedHeight(fixedHeight);

        QGroupBox pathModeGroup = new QGroupBox(mainGroup);
        pathModeGroup.setAttribute(Qt.WidgetAttribute.WA_ContentsPropagated);
        QRadioButton curveMode = new QRadioButton(pathModeGroup);
        QRadioButton lineMode = new QRadioButton(pathModeGroup);
        pathModeGroup.setTitle("Path composed of");
        curveMode.setText("Curves");
        lineMode.setText("Lines");

        QGroupBox penWidthGroup = new QGroupBox(mainGroup);
        penWidthGroup.setAttribute(Qt.WidgetAttribute.WA_ContentsPropagated);
        QSlider penWidth = new QSlider(Qt.Orientation.Horizontal, penWidthGroup);
        penWidth.setSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Fixed);
        penWidthGroup.setTitle("Pen Width");
        penWidth.setRange(0, 500);

        QPushButton animated = new QPushButton(mainGroup);
        animated.setText("Animate");
        animated.setCheckable(true);
        // #endif

        QPushButton showSourceButton = new QPushButton(mainGroup);
        showSourceButton.setText("Show Source");

        QPushButton whatsThisButton = new QPushButton(mainGroup);
        whatsThisButton.setText("What's This?");
        whatsThisButton.setCheckable(true);

        // Layouting
        QHBoxLayout viewLayout = new QHBoxLayout(this);
        viewLayout.addWidget(m_renderer);
        viewLayout.addWidget(mainGroup);

        QVBoxLayout mainGroupLayout = new QVBoxLayout(mainGroup);
        mainGroupLayout.setMargin(3);
        mainGroupLayout.addWidget(capGroup);
        mainGroupLayout.addWidget(joinGroup);
        mainGroupLayout.addWidget(styleGroup);
        mainGroupLayout.addWidget(penWidthGroup);
        mainGroupLayout.addWidget(pathModeGroup);
        mainGroupLayout.addWidget(animated);
        mainGroupLayout.addStretch(1);
        mainGroupLayout.addWidget(showSourceButton);
        mainGroupLayout.addWidget(whatsThisButton);

        QVBoxLayout capGroupLayout = new QVBoxLayout(capGroup);
        capGroupLayout.addWidget(flatCap);
        capGroupLayout.addWidget(squareCap);
        capGroupLayout.addWidget(roundCap);

        QVBoxLayout joinGroupLayout = new QVBoxLayout(joinGroup);
        joinGroupLayout.addWidget(bevelJoin);
        joinGroupLayout.addWidget(miterJoin);
        joinGroupLayout.addWidget(roundJoin);

        QVBoxLayout styleGroupLayout = new QVBoxLayout(styleGroup);
        styleGroupLayout.addWidget(solidLine);
        styleGroupLayout.addWidget(dashLine);
        styleGroupLayout.addWidget(dotLine);
        styleGroupLayout.addWidget(dashDotLine);
        styleGroupLayout.addWidget(dashDotDotLine);
        styleGroupLayout.addWidget(customDashLine);

        QVBoxLayout pathModeGroupLayout = new QVBoxLayout(pathModeGroup);
        pathModeGroupLayout.addWidget(curveMode);
        pathModeGroupLayout.addWidget(lineMode);

        QVBoxLayout penWidthLayout = new QVBoxLayout(penWidthGroup);
        penWidthLayout.addWidget(penWidth);

        // Set up connections
        penWidth.valueChanged.connect(m_renderer, "setPenWidth(int)");

        animated.toggled.connect(m_renderer, "setAnimation(boolean)");

        flatCap.clicked.connect(m_renderer, "setFlatCap()");
        squareCap.clicked.connect(m_renderer, "setSquareCap()");
        roundCap.clicked.connect(m_renderer, "setRoundCap()");

        bevelJoin.clicked.connect(m_renderer, "setBevelJoin()");
        miterJoin.clicked.connect(m_renderer, "setMiterJoin()");
        roundJoin.clicked.connect(m_renderer, "setRoundJoin()");

        curveMode.clicked.connect(m_renderer, "setCurveMode()");
        lineMode.clicked.connect(m_renderer, "setLineMode()");

        solidLine.clicked.connect(m_renderer, "setSolidLine()");
        dashLine.clicked.connect(m_renderer, "setDashLine()");
        dotLine.clicked.connect(m_renderer, "setDotLine()");
        dashDotLine.clicked.connect(m_renderer, "setDashDotLine()");
        dashDotDotLine.clicked.connect(m_renderer, "setDashDotDotLine()");
        customDashLine.clicked.connect(m_renderer, "setCustomDashLine()");

        showSourceButton.clicked.connect(m_renderer, "showSource()");
        whatsThisButton.clicked.connect(m_renderer, "setDescriptionEnabled(boolean)");
        m_renderer.descriptionEnabledChanged.connect(whatsThisButton, "setChecked(boolean)");

        // Set the defaults
        animated.setChecked(true);
        flatCap.setChecked(true);
        bevelJoin.setChecked(true);
        penWidth.setValue(50);
        curveMode.setChecked(true);
        solidLine.setChecked(true);

        m_renderer.loadSourceFile("classpath:/org/qtjambi/demos/PathStrokeWidget.java");
        m_renderer.loadDescription("classpath:/org/qtjambi/demos/PathStroke/pathstroke.html");
    }

    public class PathStrokeRendrer extends ArthurFrame {

        private QBasicTimer m_timer = new QBasicTimer();
        private PathMode m_pathMode;
        private boolean m_wasAnimated;
        private double m_penWidth;
        private int m_pointSize;
        private int m_activePoint;
        private Vector<QPointF> m_points = new Vector<QPointF>();
        private Vector<QPointF> m_vectors = new Vector<QPointF>();
        private Qt.PenJoinStyle m_joinStyle;
        private Qt.PenCapStyle m_capStyle;
        private Qt.PenStyle m_penStyle;

        void setPenWidth(int penWidth) {
            m_penWidth = penWidth / 10.0;
            update();
        }

        void setFlatCap() {
            m_capStyle = Qt.PenCapStyle.FlatCap;
            update();
        }

        void setSquareCap() {
            m_capStyle = Qt.PenCapStyle.SquareCap;
            update();
        }

        void setRoundCap() {
            m_capStyle = Qt.PenCapStyle.RoundCap;
            update();
        }

        void setBevelJoin() {
            m_joinStyle = Qt.PenJoinStyle.BevelJoin;
            update();
        }

        void setMiterJoin() {
            m_joinStyle = Qt.PenJoinStyle.MiterJoin;
            update();
        }

        void setRoundJoin() {
            m_joinStyle = Qt.PenJoinStyle.RoundJoin;
            update();
        }

        void setCurveMode() {
            m_pathMode = PathMode.CurveMode;
            update();
        }

        void setLineMode() {
            m_pathMode = PathMode.LineMode;
            update();
        }

        void setSolidLine() {
            m_penStyle = Qt.PenStyle.SolidLine;
            update();
        }

        void setDashLine() {
            m_penStyle = Qt.PenStyle.DashLine;
            update();
        }

        void setDotLine() {
            m_penStyle = Qt.PenStyle.DotLine;
            update();
        }

        void setDashDotLine() {
            m_penStyle = Qt.PenStyle.DashDotLine;
            update();
        }

        void setDashDotDotLine() {
            m_penStyle = Qt.PenStyle.DashDotDotLine;
            update();
        }

        void setCustomDashLine() {
            m_penStyle = Qt.PenStyle.NoPen;
            update();
        }

        @Override
        public QSize sizeHint() {
            return new QSize(500, 500);
        }

        public boolean animation() {
            return m_timer.isActive();
        }

        public double realPenWidth() {
            return m_penWidth;
        }

        public void setRealPenWidth(double penWidth) {
            m_penWidth = penWidth;
            update();
        }

        public PathStrokeRendrer(QWidget parent) {

            super(parent);
            m_pointSize = 10;
            m_activePoint = -1;
            m_capStyle = Qt.PenCapStyle.FlatCap;
            m_joinStyle = Qt.PenJoinStyle.BevelJoin;
            m_pathMode = PathMode.CurveMode;
            m_penWidth = 1;
            m_penStyle = Qt.PenStyle.SolidLine;
            m_wasAnimated = true;
            setSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Expanding);

        }

        @Override
        public void paint(QPainter painter) {
            if (m_points.isEmpty())
                initializePoints();

            painter.setRenderHint(QPainter.RenderHint.Antialiasing);

            painter.setPen(Qt.PenStyle.NoPen);

            // finalruct the path
            QPainterPath path = new QPainterPath();
            path.moveTo(m_points.elementAt(0));

            if (m_pathMode == PathMode.LineMode) {
                for (int i = 1; i < m_points.size(); ++i) {
                    path.lineTo(m_points.elementAt(i));
                }
            } else {
                int i = 1;
                while (i + 2 < m_points.size()) {
                    path.cubicTo(m_points.elementAt(i), m_points.elementAt(i + 1), m_points
                            .elementAt(i + 2));
                    i += 3;
                }
                while (i < m_points.size()) {
                    path.lineTo(m_points.elementAt(i));
                    ++i;
                }
            }

            // Draw the path
            {
                QColor lg = new QColor(org.qtjambi.qt.core.Qt.GlobalColor.red);

                // The "custom" pen
                if (m_penStyle == Qt.PenStyle.NoPen) {
                    QPainterPathStroker stroker = new QPainterPathStroker();
                    stroker.setWidth(m_penWidth);
                    stroker.setJoinStyle(m_joinStyle);
                    stroker.setCapStyle(m_capStyle);

                    List<Double> dashes = new Vector<Double>();
                    Double space = new Double(4);
                    dashes.add(new Double(1.0));
                    dashes.add(space);
                    dashes.add(new Double(3.0));
                    dashes.add(space);
                    dashes.add(new Double(9.0));
                    dashes.add(space);
                    dashes.add(new Double(27.0));
                    dashes.add(space);
                    dashes.add(new Double(9.0));
                    dashes.add(space);
                    dashes.add(new Double(3.0));
                    dashes.add(space);

                    stroker.setDashPattern(dashes);
                    QPainterPath stroke = stroker.createStroke(path);
                    painter.fillPath(stroke, new QBrush(lg));

                } else {
                    QPen pen = new QPen(lg, m_penWidth, m_penStyle, m_capStyle, m_joinStyle);
                    painter.strokePath(path, pen);
                }
            }

            if (true) {
                // Draw the control points
                painter.setPen(new QColor(50, 100, 120, 200));
                painter.setBrush(new QColor(200, 200, 210, 120));
                for (int i = 0; i < m_points.size(); ++i) {
                    QPointF pos = m_points.elementAt(i);
                    painter.drawEllipse(new QRectF(pos.x() - m_pointSize, pos.y() - m_pointSize,
                            m_pointSize * 2, m_pointSize * 2));
                }
                painter.setPen(new QPen(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.lightGray), 0, Qt.PenStyle.SolidLine));
                painter.setBrush(Qt.BrushStyle.NoBrush);

                QPolygonF pol = new QPolygonF();
                for (Iterator<QPointF> iter = m_points.iterator(); iter.hasNext();) {
                    QPointF element = iter.next();
                    pol.append(element);
                }

                painter.drawPolyline(pol);
            }
        }

        void initializePoints() {
            final int count = 7;
            m_points.clear();
            m_vectors.clear();

            QTransform m = new QTransform();
            double rot = 360 / count;
            QPointF center = new QPointF(width() / 2, height() / 2);
            QTransform vm = new QTransform();
            vm.shear(2, -1);
            vm.scale(3, 3);

            for (int i = 0; i < count; ++i) {
                m_vectors.add(m.multiplied(vm).map(new QPointF(.1f, .25f)));
                m.rotate(rot);
                m_points.add(m.map(new QPointF(0 + center.x(), 100 + center.y())));

            }
        }

        public void updatePoints() {
            double pad = 10;
            double left = pad;
            double right = width() - pad;
            double top = pad;
            double bottom = height() - pad;

            assert (m_points.size() == m_vectors.size());
            for (int i = 0; i < m_points.size(); ++i) {

                if (i == m_activePoint)
                    continue;

                QPointF pos = m_points.elementAt(i);
                QPointF vec = m_vectors.elementAt(i);
                pos.add(vec);
                if (pos.x() < left || pos.x() > right) {
                    vec.setX(-vec.x());
                    pos.setX(pos.x() < left ? left : right);
                }
                if (pos.y() < top || pos.y() > bottom) {
                    vec.setY(-vec.y());
                    pos.setY(pos.y() < top ? top : bottom);
                }
                m_points.set(i, pos);
                m_vectors.set(i, vec);
            }
            update();
        }

        @Override
        public void mousePressEvent(QMouseEvent e) {
            setDescriptionEnabled(false);
            m_activePoint = -1;
            float distance = -1;
            for (int i = 0; i < m_points.size(); ++i) {
                float d = (float) new QLineF(new QPointF(e.pos()), m_points.elementAt(i)).length();
                if ((distance < 0 && d < 8 * m_pointSize) || d < distance) {
                    distance = d;
                    m_activePoint = i;
                }
            }

            if (m_activePoint != -1) {
                m_wasAnimated = m_timer.isActive();
                setAnimation(false);
                mouseMoveEvent(e);
            }
        }

        @Override
        public void mouseMoveEvent(QMouseEvent e) {
            if (m_activePoint >= 0 && m_activePoint < m_points.size()) {
                m_points.set(m_activePoint, new QPointF(e.pos()));
                update();
            }
        }

        @Override
        public void mouseReleaseEvent(QMouseEvent e) {
            m_activePoint = -1;
            setAnimation(m_wasAnimated);
        }

        @Override
        public void timerEvent(QTimerEvent e) {
            if (e.timerId() == m_timer.timerId()) {
                updatePoints();
                QApplication.sync();
            }
        }

        public void setAnimation(boolean animation) {
            m_timer.stop();

            if (animation) {
                m_timer.start(25, this);
            }
        }
    }
}
