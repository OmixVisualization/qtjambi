import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_corelib_io_qfile {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        QFile file = new QFile();
        QDir.setCurrent("/tmp");
        file.setFileName("readme.txt");
        QDir.setCurrent("/home");
        file.open(QIODevice.OpenModeFlag.ReadOnly);      // opens "/home/readme.txt" under Unix
//! [0]

    }
}
