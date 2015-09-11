import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_widgets_qlineedit extends QLineEdit {
    public static void main(String args[]) {
        QApplication.initialize(args);
    }
//! [0]
        protected void contextMenuEvent(QContextMenuEvent event)
        {
            QMenu menu = createStandardContextMenu();
            menu.addAction(tr("My Menu Item"));
            //...
            menu.exec(event.globalPos());
        }
//! [0]

}
