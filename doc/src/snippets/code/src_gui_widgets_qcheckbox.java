import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_widgets_qcheckbox extends QWidget {
    public static void main(String args[]) {
        QApplication.initialize(args);
    }

    public void createCheckBox() {
//! [0]
        QCheckBox checkbox = new QCheckBox("C&ase sensitive", this);
//! [0]
    }
}
