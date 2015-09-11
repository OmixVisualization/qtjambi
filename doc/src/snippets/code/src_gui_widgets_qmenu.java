import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_widgets_qmenu {
    public static void main(String args[]) {
        QApplication.initialize(args);
      QMenu menu = new QMenu();
//! [0]
      menu.exec(QCursor.pos());
//! [0]


      QWidget somewidget = new QWidget();
//! [1]
      menu.exec(somewidget.mapToGlobal(new QPoint(0,0)));
//! [1]
}


public void x(QMenu menu, QWidget somewidget, QMouseEvent e) {
//! [2]
      menu.exec(e.globalPos());
//! [2]


//! [3]
      menu.exec(QCursor.pos());
//! [3]


//! [4]
      menu.exec(somewidget.mapToGlobal(new QPoint(0, 0)));
//! [4]

//! [5]
      menu.exec(e.globalPos());
//! [5]
}

public void y(java.util.List<QAction> actions, QPoint pos) {
//! [6]
       QMenu menu = new QMenu();
       QAction at = actions.get(0); // Assumes actions is not empty
       for (QAction a: actions)
          menu.addAction(a);
       menu.exec(pos, at);
//! [6]


    }
}
