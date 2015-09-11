/*   Ported from: doc.src.exportedfunctions.qdoc
<snip>
//! [0]
        #ifdef Q_WS_X11
        void qt_x11_wait_for_window_manager(QWidget *widget);
        #endif

        int main(int argc, char *argv[])
        {
            QApplication app(argc, argv);
            ...
            window.show();
        #ifdef Q_WS_X11
            qt_x11_wait_for_window_manager(&window);
        #endif
            ...
            return app.exec();
        }
//! [0]


</snip>
*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class doc_src_exportedfunctions {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        #ifdef Q_WS_X11
        void qt_x11_wait_for_window_manager(QWidget idget);
        #endif

        int main(int argc, char rgv[])
        {
            QApplication app(argc, argv);
            ...
            window.show();
        #ifdef Q_WS_X11
            qt_x11_wait_for_window_manager(indow);
        #endif
            ...
            return app.exec();
        }
//! [0]


    }
}
