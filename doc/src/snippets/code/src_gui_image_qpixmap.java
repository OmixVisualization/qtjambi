import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_image_qpixmap {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        String start_xpm[] = {
            "16 15 8 1",
            "a c #cec6bd",
            // ...
            };
//! [0]


//! [1]
        QPixmap myPixmap = new QPixmap();
        myPixmap.setMask(myPixmap.createHeuristicMask());
//! [1]


    }
}
