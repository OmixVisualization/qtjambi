import org.qtjambi.qt.gui.QApplication;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.opengl.QGLColormap;
import org.qtjambi.qt.opengl.QGLWidget;

public class src_opengl_qglcolormap {
//! [0]
    public static void main(String args[]) {
        QApplication.initialize(args);

        MyGLWidget widget = new MyGLWidget();  // a QGLWidget in color-index mode
        QGLColormap colormap = new QGLColormap();

        // This will fill the colormap with colors ranging from black to white.
        for (int i = 0; i < colormap.size(); i++)
            colormap.setEntry(i, new QColor(i, i, i).rgb());

        widget.setColormap(colormap);
        widget.show();
        QApplication.execStatic();
        QApplication.shutdown();
    }
//! [0]
}
