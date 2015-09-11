import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_widgets_qframe extends QWidget {
    public static void main(String args[]) {
        QApplication.initialize(args);
    }

public void foo() {
//! [0]
    QLabel label = new QLabel(this);
    label.setFrameStyle(QFrame.Shape.Panel.value() | QFrame.Shadow.Raised.value());
    label.setLineWidth(2);

    QProgressBar pbar = new QProgressBar(this);
    label.setFrameStyle(QFrame.Shape.NoFrame.value());
//! [0]


    }
}
