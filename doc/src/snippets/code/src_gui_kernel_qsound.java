import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_kernel_qsound {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        QSound.play("mysounds/bells.wav");
//! [0]


//! [1]
        QSound bells = new QSound("mysounds/bells.wav");
        bells.play();
//! [1]


    }
}
