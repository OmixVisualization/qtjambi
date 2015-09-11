/*   Ported from: doc.src.qtmac-as-native.qdoc
<snip>
//! [0]
        qmake -spec macx-xcode project.pro
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


public class doc_src_qtmac-as-native {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        qmake -spec macx-xcode project.pro
//! [0]


    }
}
