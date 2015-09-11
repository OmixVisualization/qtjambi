import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_kernel_qevent {
    public static void main(String args[]) {
        QApplication.initialize(args);


    }
    class MyWidget extends QWidget
    {
        void scrollHorizontally(int i) {}
        void scrollVertically(int i) {}

//! [0]
        public void wheelEvent(QWheelEvent event)
        {
            int numDegrees = event.delta() / 8;
            int numSteps = numDegrees / 15;

            if (event.orientation().equals(Qt.Orientation.Horizontal)) {
                scrollHorizontally(numSteps);
            } else {
                scrollVertically(numSteps);
            }
            event.accept();
        }
//! [0]
    }
}
