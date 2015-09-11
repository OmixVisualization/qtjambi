import javax.media.opengl.GL;
import javax.media.opengl.GLContext;
import javax.media.opengl.GLDrawableFactory;

import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;
import org.qtjambi.qt.opengl.*;


public class src_opengl_qgl {
    public static void main(String args[]) {
        QApplication.initialize(args);
    {
//! [0]
    QGLFormat fmt = new QGLFormat();
    fmt.setAlpha(true);
    fmt.setStereo(true);
    QGLFormat.setDefaultFormat(fmt);
//! [0]
    }
    {
        class MyGLWidget extends QGLWidget {
        public MyGLWidget(QGLFormat format)
        {
        super(format);
        }
    }
//! [1]
    QGLFormat fmt = new QGLFormat();
    fmt.setDoubleBuffer(false);                 // single buffer
    fmt.setDirectRendering(false);              // software rendering
    MyGLWidget myWidget = new MyGLWidget(fmt);
//! [1]
    }

    {
        class MyGLWidget extends QGLWidget {
        public MyGLWidget(QGLFormat format)
        {
        super(format);
        }
    }
//! [2]
    QGLFormat fmt = new QGLFormat();
    fmt.setOverlay(true);
    fmt.setStereo(true);
    MyGLWidget myWidget = new MyGLWidget(fmt);
    if (!myWidget.format().stereo()) {
        // ok, goggles off
        if (!myWidget.format().hasOverlay()) {
        throw new RuntimeException("Cool hardware required");
        }
    }
//! [2]
    }

    {
//! [3]
    // The rendering in MyGLWidget depends on using
    // stencil buffer and alpha channel
        class MyGLWidget extends QGLWidget {
        public MyGLWidget(QWidget parent)
        {
        super(new QGLFormat(new QGL.FormatOptions(QGL.FormatOption.StencilBuffer, QGL.FormatOption.AlphaChannel)), parent);
        if (!format().stencil())
            System.err.println("Could not get stencil buffer; results will be suboptimal");
        if (!format().alpha())
            System.err.println("Could not get alpha channel; results will be suboptimal");
        // ...
        }
    }
//! [3]
    }

    {
//! [4]
    QApplication.initialize(args);
    QGLFormat f = new QGLFormat();
    f.setDoubleBuffer(false);
    QGLFormat.setDefaultFormat(f);
//! [4]

    }
    {
//! [5]
    QGLFormat f = QGLFormat.defaultOverlayFormat();
    f.setDoubleBuffer(true);
    QGLFormat.setDefaultOverlayFormat(f);
//! [5]
    }

    {
    class MyGLWidget extends QGLWidget {
    public MyGLWidget(QGLFormat format)
    {
        super(format);
    }
    }
//! [6]
    // ...continued from above
    MyGLWidget myWidget = new MyGLWidget(new QGLFormat(new QGL.FormatOptions(QGL.FormatOption.HasOverlay)));
    if (myWidget.format().hasOverlay()) {
        // Yes, we got an overlay, let's check _its_ format:
        QGLContext olContext = myWidget.overlayContext();
        if (olContext.format().doubleBuffer())
        ; // yes, we got a double buffered overlay
        else
        ; // no, only single buffered overlays are available
    }
//! [6]
    }

    {
//! [7]
    //  ...
    QGLFormat f = new QGLFormat();
    f.setStereo(true);
    QGLContext cx = new QGLContext(f);
    if (!cx.create())
        throw new RuntimeException("No OpenGL support, or cannot render on the specified paintdevice.");
    if (!cx.format().stereo())
        throw new RuntimeException("Could not create stereo context.");
//! [7]
    }

    {
//! [8]
    class MyGLDrawer extends QGLWidget
    {
        GL func = null;
        GLContext ctx = null;

        public MyGLDrawer(QWidget parent)
        {
        super(parent);
        }

        protected void initializeGL()
        {
        GLDrawableFactory factory = GLDrawableFactory.getFactory();
        ctx = factory.createExternalGLContext();
        func = ctx.getGL();

        // Set up the rendering context, define display lists etc.:
        // ...
        func.glClearColor(0, 0, 0, 0);
        func.glEnable(GL.GL_DEPTH_TEST);
        // ...
        }

        protected void resizeGL(int w, int h)
        {
        // setup viewport, projection etc.:
        func.glViewport(0, 0, w, h);
        // ...
        func.glFrustum(0.0, 1.0, 0.0, 1.0, 1.0, 2.0);
        // ...
        }

        protected void paintGL()
        {
        // draw the scene:
        // ...
        func.glRotatef(10, 0, 1, 0);
        func.glMaterialf(GL.GL_FRONT, GL.GL_SHININESS, 1);
        func.glBegin(GL.GL_QUADS);
        func.glVertex3f(1, 1, 1);
        func.glVertex3f(1, 2, 1);
        // ...
        func.glEnd();
        // ...
        }

    }
//! [8]
    }
    }
}
