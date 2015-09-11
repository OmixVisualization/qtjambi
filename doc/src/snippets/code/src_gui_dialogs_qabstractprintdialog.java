import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_dialogs_qabstractprintdialog {
    public static void main(String args[]) {
        QApplication.initialize(args);
        QWidget parent = new QWidget();
        QPrinter printer = new QPrinter();
//! [0]
        QPrintDialog printDialog = new QPrintDialog(printer, parent);
        if (printDialog.exec() == QDialog.DialogCode.Accepted.value()) {
            // print ...
        }
//! [0]


    }
}
