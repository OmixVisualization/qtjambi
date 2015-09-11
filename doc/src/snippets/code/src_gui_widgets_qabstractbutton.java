import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_widgets_qabstractbutton extends QObject {
    public static void main(String args[]) {
        QApplication.initialize(args);
    }

void snipped()
{
//! [0]
        QPushButton button = new QPushButton(tr("Ro&ck && Roll"));
//! [0]


//! [1]
        button.setIcon(new QIcon(":/images/print.png"));
        button.setShortcut(tr("Alt+F7"));
//! [1]
}

//! [2]
void reactToToggle(boolean checked)
{
   if (checked) {
      // Examine the new button states ...
   }
}
//! [2]
}
