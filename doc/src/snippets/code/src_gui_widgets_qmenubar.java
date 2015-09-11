import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_widgets_qmenubar {
    public static void main(String args[]) {
        QApplication.initialize(args);
    }
public void foo(QMenuBar menubar, QMenu fileMenu) {
//! [0]
      menubar.addMenu(fileMenu);
//! [0]
}

public void foo() {
//! [1]
        QMenuBar menuBar = new QMenuBar();
//! [1]


    }
}
