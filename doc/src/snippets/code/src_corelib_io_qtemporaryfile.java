import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_corelib_io_qtemporaryfile {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        {
            QTemporaryFile file = new QTemporaryFile();
            if (file.open()) {
                // file.fileName() returns the unique file name
            }

            // the QTemporaryFile destructor removes the temporary file
        }
//! [0]


    }
}
