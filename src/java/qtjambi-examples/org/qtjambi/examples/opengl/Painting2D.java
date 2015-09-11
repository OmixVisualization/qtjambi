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
import org.qtjambi.qt.core.QPointF;
import org.qtjambi.qt.core.QRect;
import org.qtjambi.qt.core.QRectF;
import org.qtjambi.qt.core.QTimer;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QBrush;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QFont;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.gui.QLinearGradient;
import org.qtjambi.qt.gui.QPaintEvent;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPen;
import org.qtjambi.qt.opengl.QGLFormat;
import org.qtjambi.qt.opengl.QGLWidget;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QGridLayout;
import org.qtjambi.qt.widgets.QLabel;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "2D Painting")
//! [0]
public class  Painting2D extends QWidget
{
    private Helper helper;
//! [0]

//! [1]
    public Painting2D() {
        helper = new Helper();

        Widget widget = new Widget(helper, this);
        GLWidget openGL = new GLWidget(helper, this);
        QLabel nativeLabel = new QLabel(tr("Native"));
        nativeLabel.setAlignment(Qt.AlignmentFlag.AlignHCenter);
        QLabel openGLLabel = new QLabel(tr("OpenGL"));
        openGLLabel.setAlignment(Qt.AlignmentFlag.AlignHCenter);

        QGridLayout layout = new QGridLayout();
        layout.addWidget(widget, 0, 0);
        layout.addWidget(openGL, 0, 1);
        layout.addWidget(nativeLabel, 1, 0);
        layout.addWidget(openGLLabel, 1, 1);
        setLayout(layout);

        QTimer timer = new QTimer(this);
        timer.timeout.connect(openGL, "animate()");
        timer.timeout.connect(widget, "animate()");
        timer.setInterval(1);
        timer.start(50);

        setWindowTitle(tr("2D Painting on Native and OpenGL Widgets"));
        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));
    }
//! [1]

//! [2]
    class GLWidget extends QGLWidget {
        private Helper helper;
        private int elapsed;
//! [2]

//! [3]
        public GLWidget(Helper helper, QWidget parent) {
            super(new QGLFormat(), parent);

            this.helper = helper;
            setFixedSize(200, 200);
        }
//! [3]

//! [4]
        public void animate() {
            elapsed = (elapsed + ((QTimer) signalSender()).interval()) % 1000;
            repaint();
        }
//! [4]

        @Override
//! [5]
        protected void paintEvent(QPaintEvent event) {
            QPainter painter = new QPainter();
            painter.begin(this);
            painter.setRenderHint(QPainter.RenderHint.Antialiasing);
            helper.paint(painter, event, elapsed);
            painter.end();
        }
//! [5]
    }

//! [6]
    class Widget extends QWidget {
        private int elapsed;
        private Helper helper;
//! [6]

//! [7]
        public Widget(Helper helper, QWidget parent) {
            super(parent);
            this.helper = helper;
            setFixedSize(200, 200);
        }
//! [7]

//! [8]
        public void animate() {
            elapsed = (elapsed + ((QTimer) signalSender()).interval()) % 1000;
            repaint();
        }
//! [8]

        @Override
//! [9]
        public void paintEvent(QPaintEvent event) {
            QPainter painter = new QPainter();
            painter.begin(this);
            painter.setRenderHint(QPainter.RenderHint.Antialiasing);
            helper.paint(painter, event, elapsed);
            painter.end();
        }
//! [9]
    }

//! [10]
    class Helper {
        private QBrush background;
        private QBrush circleBrush;
        private QFont textFont;
        private QPen circlePen;
        private QPen textPen;
//! [10]

//! [11]
        public Helper() {
            QLinearGradient gradient =
                new QLinearGradient(new QPointF(50, -20), new QPointF(80, 20));
            gradient.setColorAt(0.0, new QColor(Qt.GlobalColor.white));
            gradient.setColorAt(1.0, new QColor(0xa6, 0xce, 0x39));

            background = new QBrush(new QColor(64, 32, 64));
            circleBrush = new QBrush(gradient);
            circlePen = new QPen(new QColor(Qt.GlobalColor.black));
            circlePen.setWidth(1);
            textPen = new QPen(new QColor(Qt.GlobalColor.white));

            textFont = new QFont();
            textFont.setPixelSize(50);
        }
//! [11]

//! [12]
        public void paint(QPainter painter, QPaintEvent event, int elapsed) {
            painter.fillRect(event.rect(), background);
            painter.translate(100, 100);
//! [12]

//! [13]
            painter.save();
            painter.setBrush(circleBrush);
            painter.setPen(circlePen);
            painter.rotate(elapsed * 0.030);

            double r = (elapsed)/1000.0;
            int n = 30;
            for (int i = 0; i < n; ++i) {
                painter.rotate(30);
                double radius = 0 + 120.0*((i+r)/n);
                double circleRadius = 1 + ((i+r)/n)*20;
                painter.drawEllipse(new QRectF(radius, -circleRadius,
                circleRadius*2, circleRadius*2));
            }
            painter.restore();
//! [13]

//! [14]
            painter.setPen(textPen);
            painter.setFont(textFont);
            painter.drawText(new QRect(-50, -50, 100, 100), Qt.AlignmentFlag.AlignCenter.value(), "Qt");
        }
//! [14]
    }

    public static void main(String args[]) {
        QApplication.initialize(args);

        Painting2D painting2d = new Painting2D();
        painting2d.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
