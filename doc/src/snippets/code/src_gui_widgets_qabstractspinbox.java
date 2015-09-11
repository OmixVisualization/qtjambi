import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_widgets_qabstractspinbox extends QWidget {
    public static void main(String args[]) {
        QApplication.initialize(args);
    }

    public void foo() {
//! [0]
        QSpinBox spinBox = new QSpinBox(this);
        spinBox.setRange(0, 100);
        spinBox.setWrapping(true);
        spinBox.setValue(100);
        spinBox.stepBy(1);
        // value is 0
//! [0]


    }
}
