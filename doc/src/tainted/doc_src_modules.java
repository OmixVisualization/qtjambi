/*   Ported from: doc.src.modules.qdoc
<snip>
//! [0]
        QT -= gui
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


public class doc_src_modules {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        QT -= gui
//! [0]


    }
}
