import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_corelib_io_qdiriterator {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        QDirIterator it = new QDirIterator("/etc",
            new QDirIterator.IteratorFlags(QDirIterator.IteratorFlag.Subdirectories));
        while (it.hasNext()) {
            System.out.println(it.next());

            // /etc/.
            // /etc/..
            // /etc/X11
            // /etc/X11/fs
            // ...
        }
//! [0]


    }
}
