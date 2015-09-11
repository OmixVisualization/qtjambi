import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_corelib_kernel_qtimer {
        static QObject pp = null;
//! [0]
    public static void main(String args[])
    {
        QApplication.initialize(args);
        QTimer.singleShot(600000, pp, "quit()");
        // ...
        QApplication.execStatic();
        QApplication.shutdown();
    }
//! [0]


}
