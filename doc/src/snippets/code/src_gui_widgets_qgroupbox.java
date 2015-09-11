import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_widgets_qgroupbox {
    public static void main(String args[]) {
        QApplication.initialize(args);
    }

public void foo(QGroupBox g) {
//! [0]
        g.setTitle("&User information");
//! [0]


    }
}
