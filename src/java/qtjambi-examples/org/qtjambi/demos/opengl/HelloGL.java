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

import javax.media.opengl.GL;
import javax.media.opengl.GL2;
import javax.media.opengl.GLContext;
import javax.media.opengl.GLDrawableFactory;

import org.qtjambi.examples.QtJambiExample;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.widgets.*;
import org.qtjambi.qt.opengl.QGLWidget;

class GLWidget extends QGLWidget
{
    int object;
    int xRot;
    int yRot;
    int zRot;
    QPoint lastPos = new QPoint();
    QColor trolltechGreen = new QColor();
    QColor trolltechPurple = new QColor();
    GL2 func = null;
    GLContext ctx = null;


    public Signal1<Integer> xRotationChanged = new Signal1<Integer>();
    public Signal1<Integer> yRotationChanged = new Signal1<Integer>();
    public Signal1<Integer> zRotationChanged = new Signal1<Integer>();

    public GLWidget(QWidget parent) {
        super(parent);
        object = 0;
        xRot = 0;
        yRot = 0;
        zRot = 0;

        trolltechGreen = QColor.fromCmykF(0.40, 0.0, 1.0, 0.0);
        trolltechPurple = QColor.fromCmykF(0.39, 0.39, 0.0, 0.0);
    }

    @Override
    protected void disposed()
    {
        func.glDeleteLists(object, 1);
    }

    @Override
    public QSize minimumSizeHint()
    {
        return new QSize(50, 50);
    }

    @Override
    public QSize sizeHint()
    {
        return new QSize(400, 400);
    }

    void setXRotation(int _angle)
    {
        int angle[] = { _angle };
        normalizeAngle(angle);

        if (angle[0] != xRot) {
            xRot = angle[0];
            xRotationChanged.emit(xRot);
            updateGL();
        }
    }

    void setYRotation(int _angle)
    {
        int angle[] = { _angle };
        normalizeAngle(angle);

        if (angle[0] != yRot) {
            yRot = angle[0];
            yRotationChanged.emit(yRot);
            updateGL();
        }
    }

    void setZRotation(int _angle)
    {
        int angle[] = { _angle };
        normalizeAngle(angle);

        if (angle[0] != zRot) {
            zRot = angle[0];
            zRotationChanged.emit(zRot);
            updateGL();
        }
    }

    @Override
    protected void initializeGL()
    {
        GLDrawableFactory factory = GLDrawableFactory.getDesktopFactory();
        ctx = factory.createExternalGLContext();
        func = (GL2)ctx.getGL();

        qglClearColor(trolltechPurple.darker());
        object = makeObject();

        func.glShadeModel(GL2.GL_FLAT);
        func.glEnable(GL.GL_DEPTH_TEST);
        func.glEnable(GL.GL_CULL_FACE);

    }

    @Override
    protected void paintGL()
    {
        func.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);
        func.glLoadIdentity();
        func.glTranslated(0.0, 0.0, -10.0);
        func.glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
        func.glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
        func.glRotated(zRot / 16.0, 0.0, 0.0, 1.0);
        func.glCallList(object);
    }

    @Override
    protected void resizeGL(int width, int height)
    {
        int side = Math.min(width, height);
        func.glViewport((width - side) / 2, (height - side) / 2, side, side);

        func.glMatrixMode(GL2.GL_PROJECTION);
        func.glLoadIdentity();
        func.glOrtho(-0.5, +0.5, +0.5, -0.5, 4.0, 15.0);
        func.glMatrixMode(GL2.GL_MODELVIEW);
    }

    @Override
    protected void mousePressEvent(QMouseEvent event)
    {
        lastPos = event.pos();
    }

    @Override
    protected void mouseMoveEvent(QMouseEvent event)
    {
        int dx = event.x() - lastPos.x();
        int dy = event.y() - lastPos.y();

        if (event.buttons().isSet(Qt.MouseButton.LeftButton)) {
            setXRotation(xRot + 8 * dy);
            setYRotation(yRot + 8 * dx);
        } else if (event.buttons().isSet(Qt.MouseButton.RightButton)) {
            setXRotation(xRot + 8 * dy);
            setZRotation(zRot + 8 * dx);
        }
        lastPos = event.pos();
    }

    int makeObject()
    {
        int list = func.glGenLists(1);
        func.glNewList(list, GL2.GL_COMPILE);

        func.glBegin(GL2.GL_QUADS);

        double x1 = 0.06;

        double y1 = -0.14;
        double x2 = +0.14;
        double y2 = -0.06;
        double x3 = +0.08;
        double y3 = +0.00;
        double x4 = +0.30;
        double y4 = +0.22;

        quad(x1, y1, x2, y2, y2, x2, y1, x1);
        quad(x3, y3, x4, y4, y4, x4, y3, x3);

        extrude(x1, y1, x2, y2);
        extrude(x2, y2, y2, x2);
        extrude(y2, x2, y1, x1);
        extrude(y1, x1, x1, y1);
        extrude(x3, y3, x4, y4);
        extrude(x4, y4, y4, x4);
        extrude(y4, x4, y3, x3);

        final double Pi = 3.14159265358979323846;
        final int NumSectors = 200;

        for (int i=0; i<NumSectors; ++i) {
            double angle1 = (i * 2 * Pi) / NumSectors;
            double x5 = 0.30 * Math.sin(angle1);
            double y5 = 0.30 * Math.cos(angle1);
            double x6 = 0.20 * Math.sin(angle1);
            double y6 = 0.20 * Math.cos(angle1);

            double angle2 = ((i + 1) * 2 * Pi) / NumSectors;
            double x7 = 0.20 * Math.sin(angle2);
            double y7 = 0.20 * Math.cos(angle2);
            double x8 = 0.30 * Math.sin(angle2);
            double y8 = 0.30 * Math.cos(angle2);

            quad(x5, y5, x6, y6, x7, y7, x8, y8);

            extrude(x6, y6, x7, y7);
            extrude(x8, y8, x5, y5);
        }

        func.glEnd();
        func.glEndList();

        return list;
    }

    void quad(double x1, double y1, double x2, double y2,
              double x3, double y3, double x4, double y4)
    {
        qglColor(trolltechGreen);

        func.glVertex3d(x1, y1, -0.05);
        func.glVertex3d(x2, y2, -0.05);
        func.glVertex3d(x3, y3, -0.05);
        func.glVertex3d(x4, y4, -0.05);

        func.glVertex3d(x4, y4, +0.05);
        func.glVertex3d(x3, y3, +0.05);
        func.glVertex3d(x2, y2, +0.05);
        func.glVertex3d(x1, y1, +0.05);
    }

    void extrude(double x1, double y1, double x2, double y2)
    {
        qglColor(trolltechGreen.darker(250 + (int)(100 * x1)));
        func.glVertex3d(x1, y1, +0.05);
        func.glVertex3d(x2, y2, +0.05);
        func.glVertex3d(x2, y2, -0.05);
        func.glVertex3d(x1, y1, -0.05);

    }

    void normalizeAngle(int angle[])
    {
        while (angle[0] < 0)
            angle[0] += 360 * 16;
        while (angle[0] > 360 * 16)
            angle[0] -= 360 * 16;
    }
}

@QtJambiExample(name = "Open GL",
                canInstantiate = "call-static-method:checkJoglSupport")
public class HelloGL extends QWidget
{
    GLWidget glWidget = null;
    QSlider xSlider = null;
    QSlider ySlider = null;
    QSlider zSlider = null;

    QSlider createSlider(String setterSlot)
    {
        QSlider slider = new QSlider(Qt.Orientation.Vertical);
        slider.setRange(0, 360 * 16);
        slider.setSingleStep(16);
        slider.setPageStep(15 * 16);
        slider.setTickInterval(15 * 16);
        slider.setTickPosition(QSlider.TickPosition.TicksAbove);
        slider.valueChanged.connect(glWidget, setterSlot);
        return slider;
    }

    public HelloGL() { this(null); }

    public HelloGL(QWidget w)
    {
        glWidget = new GLWidget(w);

        xSlider = createSlider("setXRotation(int)");
        glWidget.xRotationChanged.connect(xSlider, "setValue(int)");

        ySlider = createSlider("setYRotation(int)");
        glWidget.yRotationChanged.connect(ySlider, "setValue(int)");

        zSlider = createSlider("setZRotation(int)");
        glWidget.zRotationChanged.connect(zSlider, "setValue(int)");

        QHBoxLayout mainLayout = new QHBoxLayout();
        mainLayout.addWidget(glWidget);
        mainLayout.addWidget(xSlider);
        mainLayout.addWidget(ySlider);
        mainLayout.addWidget(zSlider);
        setLayout(mainLayout);

        xSlider.setValue(15 * 16);
        ySlider.setValue(345 * 16);
        zSlider.setValue(0 * 16);

        setWindowTitle(tr("Hello GL"));
        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));
    }

    public static boolean checkJoglSupport() {
        try {
            Class.forName("javax.media.opengl.GL2");
            return true;
        } catch (Exception e) {
        }
        return false;
    }

    public static void main(String args[])
    {
        QApplication.initialize(args);

        if (!checkJoglSupport()) {
            QMessageBox.critical(null, "OpenGL Missing", "This Example requires OpenGL for Java\nAvalable at: <i>https://jogl.dev.java.net/</i>");
            return;
        }

        HelloGL window = new HelloGL();
        window.show();
        QApplication.execStatic();
        QApplication.shutdown();
        window.dispose();
    }
}

