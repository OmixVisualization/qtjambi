import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_widgets_qsplashscreen {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
       QPixmap pixmap = new QPixmap(":/splash.png");
       QSplashScreen splash = new QSplashScreen(pixmap);
       splash.show();

       // Loading some items ...
       splash.showMessage("Loaded modules");

       QApplication.processEvents();

       // Establishing connections ...
       splash.showMessage("Established connections");

       QApplication.processEvents();
//! [0]


    }
}
