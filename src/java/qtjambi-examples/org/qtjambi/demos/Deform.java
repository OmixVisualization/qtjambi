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

package org.qtjambi.demos;

import java.util.LinkedList;
import java.util.List;

import org.qtjambi.examples.QtJambiExample;
import org.qtjambi.qt.core.QBasicTimer;
import org.qtjambi.qt.core.QEvent;
import org.qtjambi.qt.core.QLineF;
import org.qtjambi.qt.core.QPointF;
import org.qtjambi.qt.core.QRect;
import org.qtjambi.qt.core.QRectF;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.QTime;
import org.qtjambi.qt.core.QTimerEvent;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QBrush;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QFont;
import org.qtjambi.qt.gui.QFontMetrics;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.gui.QImage;
import org.qtjambi.qt.gui.QMouseEvent;
import org.qtjambi.qt.gui.QPaintEvent;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPainterPath;
import org.qtjambi.qt.gui.QPen;
import org.qtjambi.qt.gui.QPixmap;
import org.qtjambi.qt.gui.QRadialGradient;
import org.qtjambi.qt.gui.QTransform;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QGroupBox;
import org.qtjambi.qt.widgets.QHBoxLayout;
import org.qtjambi.qt.widgets.QLineEdit;
import org.qtjambi.qt.widgets.QPushButton;
import org.qtjambi.qt.widgets.QSizePolicy;
import org.qtjambi.qt.widgets.QSlider;
import org.qtjambi.qt.widgets.QVBoxLayout;
import org.qtjambi.qt.widgets.QWidget;

class PainterPathElements
{
    public QPainterPath path;
    public QPainterPath.Element elements[];
}

class PathDeformRenderer extends ArthurFrame
{
    QBasicTimer m_repaintTimer = new QBasicTimer();
    QTime m_repaintTracker = new QTime();
    List<PainterPathElements> m_paths = new LinkedList<PainterPathElements>();
    List<QPointF> m_advances = new LinkedList<QPointF>();
    QRectF m_pathBounds = new QRectF();
    String m_text = "";
    QPixmap m_lens_pixmap = new QPixmap();
    QImage m_lens_image = new QImage();
    int m_fontSize = 0;
    boolean m_animated = false;
    double m_intensity = 0.0;
    double m_radius = 0.0;
    QPointF m_pos = new QPointF();
    QPointF m_offset = new QPointF();
    QPointF m_direction = new QPointF();
    boolean m_textDirty = true;

    public PathDeformRenderer(QWidget parent)
    {
        super(parent);

        m_radius = 100;
        m_pos = new QPointF(m_radius, m_radius);
        m_direction = new QPointF(1, 1);
        m_fontSize = 24;
        m_animated = true;
        m_repaintTimer.start(25, this);
        m_repaintTracker.start();
        m_intensity = 1;

        generateLensPixmap();
    }

    public void setFontSize(int fontSize) { m_fontSize = fontSize; setText(m_text); }
    @Override
    public QSize sizeHint() { return new QSize(600, 500); }
    public boolean animated() { return m_animated; }
    public int radius() { return (int)m_radius; }
    public int fontSize() { return m_fontSize; }
    public int intensity() { return (int)m_intensity; }
    public String text() { return m_text; }

    public void setText(String text)
    {
        m_text = text;
        m_textDirty = true;
        update();
    }

    private void makeTextPaths()
    {
        QFont f = new QFont("times new roman,utopia");
        f.setStyleStrategy(QFont.StyleStrategy.ForceOutline);
        f.setPointSize(m_fontSize);
        f.setStyleHint(QFont.StyleHint.Times);

        QFontMetrics fm = new QFontMetrics(f);
        m_paths.clear();
        m_pathBounds = new QRectF();

        QPointF advance = new QPointF(0, 0);

        List<QPainterPath> paths = new LinkedList<QPainterPath>();

        boolean do_quick = true;
        for (int i=0; i<m_text.length(); ++i) {
            if (m_text.charAt(i) >= 0x4ff && m_text.charAt(i) <= 0x1e00) {
                do_quick = false;
                break;
            }
        }

        if (do_quick) {
            for (int i=0; i<m_text.length(); ++i) {
                QPainterPath path = new QPainterPath();
                path.addText(advance, f, m_text.substring(i, i + 1));
                m_pathBounds = m_pathBounds.united(path.boundingRect());
                paths.add(path);
                advance.add(new QPointF(fm.width(m_text.substring(i, i + 1)), 0));
            }
        } else {
            QPainterPath path = new QPainterPath();
            path.addText(advance, f, m_text);
            m_pathBounds = m_pathBounds.united(path.boundingRect());
            paths.add(path);
        }

        QTransform m = new QTransform(1, 0, 0, 1, -m_pathBounds.x(), -m_pathBounds.y());
        for (int i=0; i<paths.size(); ++i)
            addPath(m.map(paths.get(i)));

        m_textDirty = false;
    }

    private void addPath(QPainterPath path)
    {
        PainterPathElements p = new PainterPathElements();
        p.path = path;

        p.elements = new QPainterPath.Element[path.elementCount()];
        for (int i=0; i<path.elementCount(); ++i) {
            p.elements[i] = path.elementAt(i);
        }

        m_paths.add(p);
    }

    static QRect circle_bounds(QPointF center, double radius, double compensation)
    {
        return new QRect((int)Math.round(center.x() - radius - compensation),
                         (int)Math.round(center.y() - radius - compensation),
                         (int)Math.round((radius + compensation) * 2),
                         (int)Math.round((radius + compensation) * 2));
    }


    final int LENS_EXTENT = 10;
    void generateLensPixmap()
    {
        double rad = m_radius + LENS_EXTENT;

        QRect bounds = circle_bounds(new QPointF(), rad, 0);

        QPainter painter = new QPainter();
        if (preferImage()) {
            m_lens_image = new QImage(bounds.size(), QImage.Format.Format_ARGB32_Premultiplied);
            m_lens_image.fill(0);
            painter.begin(m_lens_image);
        } else {
            m_lens_pixmap = new QPixmap(bounds.size());
            m_lens_pixmap.fill(new QColor(0, 0, 0, 0));
            painter.begin(m_lens_pixmap);
        }

        QRadialGradient gr = new QRadialGradient(rad, rad, rad, 3 * rad / 5, 3 * rad / 5);
        gr.setColorAt(0.0, new QColor(255, 255, 255, 191));
        gr.setColorAt(0.2, new QColor(255, 255, 127, 191));
        gr.setColorAt(0.9, new QColor(150, 150, 200, 63));
        gr.setColorAt(0.95, new QColor(0, 0, 0, 127));
        gr.setColorAt(1, new QColor(0, 0, 0, 0));

        painter.setRenderHint(QPainter.RenderHint.Antialiasing);
        painter.setBrush(new QBrush(gr));
        painter.setPen(new QPen(Qt.PenStyle.NoPen));
        painter.drawEllipse(0, 0, bounds.width(), bounds.height());
        painter.end();
    }

    public void setAnimated(boolean animated)
    {
        m_animated = animated;

        if (m_animated) {
            m_repaintTimer.start(25, this);
            m_repaintTracker.start();
        } else {
            m_repaintTimer.stop();
        }
    }

    @Override
    protected void paintEvent(QPaintEvent e)
    {
        if (m_textDirty)
            makeTextPaths();
        super.paintEvent(e);
    }

    @Override
    protected void timerEvent(QTimerEvent e)
    {
        if (e.timerId() == m_repaintTimer.timerId()) {
            if ((new QLineF(new QPointF(0,0), m_direction)).length() > 1)
                m_direction.multiply(0.995);

            double time = m_repaintTracker.restart();

            QRect rectBefore = circle_bounds(m_pos, m_radius, m_fontSize);

            double dx = m_direction.x();
            double dy = m_direction.y();

            if (time > 0) {
                dx = dx * time * 0.1;
                dy = dy * time * 0.1;
            }

            m_pos.add(new QPointF(dx, dy));

            if (m_pos.x() - m_radius < 0) {
                m_direction.setX(-m_direction.x());
                m_pos.setX(m_radius);
            } else if (m_pos.x() + m_radius > width()) {
                m_direction.setX(-m_direction.x());
                m_pos.setX(width() - m_radius);
            }

            if (m_pos.y() - m_radius < 0) {
                m_direction.setY(-m_direction.y());
                m_pos.setY(m_radius);
            } else if (m_pos.y() + m_radius > height()) {
                m_direction.setY(-m_direction.y());
                m_pos.setY(height() - m_radius);
            }

            QRect rectAfter = circle_bounds(m_pos, m_radius, m_fontSize);
            update(rectBefore.united(rectAfter));
            QApplication.sync();
        }
    }

    @Override
    protected void mousePressEvent(QMouseEvent e)
    {
        setDescriptionEnabled(false);

        m_repaintTimer.stop();
        m_offset = new QPointF();

        if ((new QLineF(m_pos, new QPointF(e.pos()))).length() <= m_radius) {
            m_offset = new QPointF(m_pos.x(), m_pos.y());
            m_offset.subtract(new QPointF(e.pos()));
        }

        mouseMoveEvent(e);
    }

    @Override
    protected void mouseReleaseEvent(QMouseEvent e)
    {
        if (e.buttons().isSet(Qt.MouseButton.NoButton) && m_animated) {
            m_repaintTimer.start(25, this);
            m_repaintTracker.start();
        }
    }

    @Override
    protected void mouseMoveEvent(QMouseEvent e)
    {
        QRect rectBefore = circle_bounds(m_pos, m_radius, m_fontSize);
        if (e.type() == QEvent.Type.MouseMove) {
            QPointF epos = new QPointF(e.pos());
            epos.add(m_offset);
            QLineF line = new QLineF(m_pos, epos);
            line.setLength(line.length() * .1);
            QPointF dir = new QPointF(line.dx(), line.dy());
            m_direction.add(dir);
            m_direction.multiply(0.5);
        }

        m_pos = new QPointF(e.pos());
        m_pos.add(m_offset);
        QRect rectAfter = circle_bounds(m_pos, m_radius, m_fontSize);

        update(rectBefore.united(rectAfter));
    }

    private void deformElement(QPainterPath.Element e, QPointF offset, double pts[])
    {
        double flip = m_intensity;

        double x = e.x() + offset.x();
        double y = e.y() + offset.y();

        double dx = x - m_pos.x();
        double dy = y - m_pos.y();
        double len = m_radius - Math.sqrt(dx * dx + dy * dy);

        if (len > 0) {
            x = x + flip * dx * len / m_radius;
            y = y + flip * dy * len / m_radius;
        }

        pts[0] = x;
        pts[1] = y;
    }

    protected QPainterPath lensDeform(PainterPathElements source, QPointF offset)
    {
        QPainterPath path = new QPainterPath();
        double pts[] = new double[2];

        for (int i=0; i<source.elements.length;) {
            QPainterPath.Element e = source.elements[i++];

            if (e.isLineTo()) {
                deformElement(e, offset, pts);
                path.lineTo(pts[0], pts[1]);
            } else if (e.isMoveTo()) {
                deformElement(e, offset, pts);
                path.moveTo(pts[0], pts[1]);
            } else if (e.isCurveTo()) {
                deformElement(e, offset, pts);
                double c1x = pts[0], c1y = pts[1];

                e = source.elements[i++];
                deformElement(e, offset, pts);
                double c2x = pts[0], c2y = pts[1];

                e = source.elements[i++];
                deformElement(e, offset, pts);
                double ex = pts[0], ey = pts[1];

                path.cubicTo(c1x, c1y, c2x, c2y, ex, ey);
            }
        }

        return path;
    }

    @Override
    public void paint(QPainter painter)
    {
        int pad_x = 5;
        int pad_y = 5;

        int skip_x = (int)Math.round(m_pathBounds.width() + pad_x + m_fontSize / 2);
        int skip_y = (int)Math.round(m_pathBounds.height() + pad_y);

        painter.setPen(new QPen(Qt.PenStyle.NoPen));
        painter.setBrush(new QBrush(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.black)));

        QRectF clip = painter.clipPath().boundingRect();
        int overlap = pad_x / 2;

        for (int start_y=0; start_y<height(); start_y += skip_y) {
            if (start_y > clip.bottom())
                break;

            int start_x = -overlap;
            for (;start_x < width(); start_x += skip_x) {
                if (start_y + skip_y >= clip.top()
                 && start_x + skip_x >= clip.left()
                 && start_x <= clip.right()) {
                    for (int i=0; i<m_paths.size(); ++i) {
                        QPainterPath path = lensDeform(m_paths.get(i), new QPointF(start_x, start_y));
                        painter.drawPath(path);
                    }
                }

            }
            overlap = skip_x - (start_x - width());
        }
        if (preferImage()) {
            painter.drawImage((int)(m_pos.x() - m_radius - LENS_EXTENT), (int)(m_pos.y() - m_radius - LENS_EXTENT), m_lens_image);
        } else {
            painter.drawPixmap((int)(m_pos.x() - m_radius - LENS_EXTENT), (int)(m_pos.y() - m_radius - LENS_EXTENT), m_lens_pixmap);
        }
    }

    public void setRadius(int radius)
    {
        double max = Math.max(m_radius, radius);
        m_radius = radius;

        generateLensPixmap();
        if (!m_animated || m_radius < max)
            update(circle_bounds(m_pos, max, m_fontSize));
    }

    public void setIntensity(int intensity)
    {
        m_intensity = intensity / 100.0;
        if (!m_animated)
            update(circle_bounds(m_pos, m_radius, m_fontSize));
    }
}

@QtJambiExample(name = "Deform")
public class Deform extends QWidget
{
    private PathDeformRenderer m_renderer;

    public Deform(QWidget parent)
    {
        super(parent);

        setWindowTitle("Vector deformation");
        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));

        m_renderer = new PathDeformRenderer(this);
        m_renderer.setSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Expanding);

        QGroupBox mainGroup = new QGroupBox(this);
        mainGroup.setTitle("Vector Deformation");

        QGroupBox radiusGroup = new QGroupBox(mainGroup);
        radiusGroup.setTitle("Lens radius");
        QSlider radiusSlider = new QSlider(Qt.Orientation.Horizontal, radiusGroup);
        radiusSlider.setRange(50, 150);
        radiusSlider.setSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Fixed);

        QGroupBox deformGroup = new QGroupBox(mainGroup);
        deformGroup.setTitle("Deformation");
        QSlider deformSlider = new QSlider(Qt.Orientation.Horizontal, deformGroup);
        deformSlider.setRange(-100, 100);
        deformSlider.setSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Fixed);

        QGroupBox fontSizeGroup = new QGroupBox(mainGroup);
        fontSizeGroup.setTitle("Font Size");
        QSlider fontSizeSlider = new QSlider(Qt.Orientation.Horizontal, fontSizeGroup);
        fontSizeSlider.setRange(16, 200);
        fontSizeSlider.setSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Fixed);

        QGroupBox textGroup = new QGroupBox(mainGroup);
        textGroup.setTitle("Text");

        QLineEdit textInput = new QLineEdit(textGroup);

        QPushButton animateButton = new QPushButton(mainGroup);
        animateButton.setText("Animated");
        animateButton.setCheckable(true);

        QPushButton showSourceButton = new QPushButton(mainGroup);
        showSourceButton.setText("Show Source");

        QPushButton whatsThisButton = new QPushButton(mainGroup);
        whatsThisButton.setText("What's This?");
        whatsThisButton.setCheckable(true);

        QHBoxLayout mainLayout = new QHBoxLayout(this);
        mainLayout.addWidget(m_renderer);
        mainLayout.addWidget(mainGroup);
        mainGroup.setFixedWidth(180);

        QVBoxLayout mainGroupLayout = new QVBoxLayout(mainGroup);
        mainGroupLayout.addWidget(radiusGroup);
        mainGroupLayout.addWidget(deformGroup);
        mainGroupLayout.addWidget(fontSizeGroup);
        mainGroupLayout.addWidget(textGroup);
        mainGroupLayout.addWidget(animateButton);
        mainGroupLayout.addStretch(1);
        mainGroupLayout.addWidget(showSourceButton);
        mainGroupLayout.addWidget(whatsThisButton);

        QVBoxLayout radiusGroupLayout = new QVBoxLayout(radiusGroup);
        radiusGroupLayout.addWidget(radiusSlider);

        QVBoxLayout deformGroupLayout = new QVBoxLayout(deformGroup);
        deformGroupLayout.addWidget(deformSlider);

        QVBoxLayout fontSizeGroupLayout = new QVBoxLayout(fontSizeGroup);
        fontSizeGroupLayout.addWidget(fontSizeSlider);

        QVBoxLayout textGroupLayout = new QVBoxLayout(textGroup);
        textGroupLayout.addWidget(textInput);

        textInput.textChanged.connect(m_renderer, "setText(String)");
        radiusSlider.valueChanged.connect(m_renderer, "setRadius(int)");
        deformSlider.valueChanged.connect(m_renderer, "setIntensity(int)");
        fontSizeSlider.valueChanged.connect(m_renderer, "setFontSize(int)");
        animateButton.clicked.connect(m_renderer, "setAnimated(boolean)");
        whatsThisButton.clicked.connect(m_renderer, "setDescriptionEnabled(boolean)");
        showSourceButton.clicked.connect(m_renderer, "showSource()");
        m_renderer.descriptionEnabledChanged.connect(whatsThisButton, "setChecked(boolean)");

        animateButton.animateClick();
        deformSlider.setValue(80);
        radiusSlider.setValue(100);
        fontSizeSlider.setValue(100);
        textInput.setText("Qt Jambi");

        m_renderer.loadSourceFile("classpath:org/qtjambi/demos/Deform.java");
        m_renderer.loadDescription("classpath:org/qtjambi/demos/Deform.html");
        m_renderer.setDescriptionEnabled(false);
    }

    public static void main(String args[])
    {
        QApplication.initialize(args);

        Deform w = new Deform(null);
        w.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}

