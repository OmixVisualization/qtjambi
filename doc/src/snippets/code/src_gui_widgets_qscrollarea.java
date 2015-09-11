import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_widgets_qscrollarea {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        QLabel imageLabel = new QLabel();
        QImage image = new QImage("happyguy.png");
        imageLabel.setPixmap(QPixmap.fromImage(image));

        QScrollArea scrollArea = new QScrollArea();
        scrollArea.setBackgroundRole(QPalette.ColorRole.Dark);
        scrollArea.setWidget(imageLabel);
//! [0]
    }
}
