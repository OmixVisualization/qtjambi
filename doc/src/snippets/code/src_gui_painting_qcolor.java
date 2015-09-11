import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_painting_qcolor extends QWidget {

    protected void paintEvent(QPaintEvent e) {
        QPainter painter = new QPainter(this);

//! [0]
        // Specify semi-transparent red
        painter.setBrush(new QColor(255, 0, 0, 127));
        painter.drawRect(0, 0, width()/2, height());

        // Specify semi-transparent blue
        painter.setBrush(new QColor(0, 0, 255, 127));
        painter.drawRect(0, 0, width(), height()/2);
//! [0]

    }

    public static void main(String args[]) {
        QApplication.initialize(args);

        QWidget w = new src_gui_painting_qcolor();
        w.show();

        QApplication.execStatic();
        QApplication.shutdown();

    }
}
