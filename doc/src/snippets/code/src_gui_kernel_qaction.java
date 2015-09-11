import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_kernel_qaction {
    public static void main(String args[]) {
    QApplication.initialize(args);
    QWidget window = new QWidget();
    QIcon SomeIcon = new QIcon("myicon.xpm");
//! [0]
    QApplication.initialize(args);
    QApplication.setAttribute(Qt.ApplicationAttribute.AA_DontShowIconsInMenus);  // Icons are *no longer shown* in menus
    // ...
    QAction myAction = new QAction(window);
    // ...
    myAction.setIcon(SomeIcon);
    myAction.setIconVisibleInMenu(true);   // Icon *will* be shown in menus for *this* action.
//! [0]
    }
}
