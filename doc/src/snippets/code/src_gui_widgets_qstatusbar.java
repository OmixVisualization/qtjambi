/*   Ported from: src.gui.widgets.qstatusbar.cpp
*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_widgets_qstatusbar extends QMainWindow{

    class MyReadWriteIndication extends QWidget {}

    public void test() {
//! [0]
        statusBar().addWidget(new MyReadWriteIndication());
//! [0]
    }

    public static void main(String args[]) {
        QApplication.initialize(args);
        src_gui_widgets_qstatusbar bar = new src_gui_widgets_qstatusbar();
        bar.show();
        QApplication.execStatic();
        QApplication.shutdown();
    }
}
