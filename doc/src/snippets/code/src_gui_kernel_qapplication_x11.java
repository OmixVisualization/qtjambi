import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_kernel_qapplication_x11 {
    public static void calculateHugeMandelbrot() {
    }

    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        QApplication.setOverrideCursor(new QCursor(Qt.CursorShape.WaitCursor));
        calculateHugeMandelbrot();              // lunch time...
        QApplication.restoreOverrideCursor();
//! [0]


    }
}
