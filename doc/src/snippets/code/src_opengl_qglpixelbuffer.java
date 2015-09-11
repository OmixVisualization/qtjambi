import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;
import org.qtjambi.qt.opengl.*;


public class src_opengl_qglpixelbuffer {
    public static void main(String args[]) {
        QApplication.initialize(args);
    {
//! [0]
        QGLPixelBuffer pbuffer = new QGLPixelBuffer(256, 256);
        //...
        pbuffer.makeCurrent();
        int dynamicTexture = pbuffer.generateDynamicTexture();
        pbuffer.bindToDynamicTexture(dynamicTexture);
        //...
        pbuffer.releaseFromDynamicTexture();
//! [0]
    }
    {
//! [1]
        QGLPixelBuffer pbuffer = new QGLPixelBuffer(256, 256);
        // ...
        pbuffer.makeCurrent();
        int dynamicTexture = pbuffer.generateDynamicTexture();
        // ...
        pbuffer.updateDynamicTexture(dynamicTexture);
//! [1]
    }

    }
}
