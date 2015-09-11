/*   Ported from: doc.src.appicon.qdoc
<snip>
//! [0]
        IDI_ICON1               ICON    DISCARDABLE     "myappico.ico"
//! [0]


//! [1]
        RC_FILE = myapp.rc
//! [1]


//! [2]
        ICON = myapp.icns
//! [2]


//! [3]
        kde-config --path icon
//! [3]


//! [4]
        gnome-config --datadir
//! [4]


</snip>
*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class doc_src_appicon {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        IDI_ICON1               ICON    DISCARDABLE     "myappico.ico"
//! [0]


//! [1]
        RC_FILE = myapp.rc
//! [1]


//! [2]
        ICON = myapp.icns
//! [2]


//! [3]
        kde-config --path icon
//! [3]


//! [4]
        gnome-config --datadir
//! [4]


    }
}
