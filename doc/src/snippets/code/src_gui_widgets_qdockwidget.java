import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_widgets_qdockwidget extends QWidget {
    public static void main(String args[]) {
        QApplication.initialize(args);
    }

    public void foo() {
//! [0]
       QDockWidget dockWidget = (QDockWidget)parentWidget();
       if (dockWidget.features().isSet(QDockWidget.DockWidgetFeature.DockWidgetVerticalTitleBar)) {
           // I need to be vertical
       } else {
           // I need to be horizontal
       }
//! [0]


    }
}
