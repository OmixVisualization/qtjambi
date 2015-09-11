/*   Ported from: doc.src.qtopengl.qdoc
<snip>
//! [0]
        #include <QtOpenGL>
//! [0]


//! [1]
        QT += opengl
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


public class doc_src_qtopengl {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        #include <QtOpenGL>
//! [0]


//! [1]
        QT += opengl
//! [1]


    }
}
