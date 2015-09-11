/*   Ported from: src.gui.widgets.qspinbox.cpp
*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_widgets_qspinbox {
    public static void main(String args[]) {
        QApplication.initialize(args);
        QSpinBox spinbox = new QSpinBox();
//! [0]
        spinbox.setPrefix("$");
//! [0]


//! [1]
        spinbox.setSuffix(" km");
//! [1]

    int minimum=0, maximum=10;
//! [2]
        spinbox.setRange(minimum, maximum);
//! [2]


//! [3]
        spinbox.setMinimum(minimum);
        spinbox.setMaximum(maximum);
//! [3]


//! [4]
        spinbox.setPrefix("$");
//! [4]


//! [5]
        spinbox.setSuffix(" km");
//! [5]


//! [6]
        spinbox.setRange(minimum, maximum);
//! [6]


//! [7]
        spinbox.setMinimum(minimum);
        spinbox.setMaximum(maximum);
//! [7]

    }
}
