import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_widgets_qworkspace {
    public static void main(String args[]) {
        QApplication.initialize(args);
    {
    class MainWindow extends QMainWindow
    {
        QWorkspace workspace;
//! [0]
    public MainWindow()
    {
        workspace = new QWorkspace();
        setCentralWidget(workspace);
        // ...
    }
//! [0]
    }
    }

    }
}
