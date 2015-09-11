/*   Ported from: doc.src.qtsvg.qdoc
<snip>
//! [0]
      #include <QtSvg>
//! [0]


//! [1]
      QT += svg
//! [1]


</snip>
*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class doc_src_qtsvg {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
      #include <QtSvg>
//! [0]


//! [1]
      QT += svg
//! [1]


    }
}
