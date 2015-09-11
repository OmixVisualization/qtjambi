/*   Ported from: doc.src.uic.qdoc
<snip>
//! [0]
        uic [options] <uifile>
//! [0]


//! [1]
        ui_%.h: %.ui
                uic $< -o $@
//! [1]


//! [2]
        ui_foo.h: foo.ui
                uic $< -o $@
//! [2]


</snip>
*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class doc_src_uic {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        uic [options] <uifile>
//! [0]


//! [1]
        ui_%.h: %.ui
                uic $< -o $@
//! [1]


//! [2]
        ui_foo.h: foo.ui
                uic $< -o $@
//! [2]


    }
}
