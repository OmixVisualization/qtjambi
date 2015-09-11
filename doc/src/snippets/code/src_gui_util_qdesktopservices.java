
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_util_qdesktopservices {


    public static void main(String args[]) {
        QApplication.initialize(args);

//! [0]
        QDesktopServices.setUrlHandler("help", new QDesktopServices.UrlHandler() {
            public void handleUrl(QUrl url) {
                // respond to url...
            }
            });
//! [0]


    /*
//! [1]
    mailto:user@foo.com?subject=Testody=Just a test
//! [1]
    */

    }
}
