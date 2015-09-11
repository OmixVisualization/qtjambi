/*   Ported from: doc.src.qtestevent.qdoc
<snip>
//! [0]
    QTestEventList events;
    events.addKeyClick('a');
    events.addKeyClick(Qt::Key_Backspace);
    events.addDelay(200);

    QLineEdit *lineEdit = new QLineEdit(myParent);
    ...
    events.simulate(lineEdit);
    events.simulate(lineEdit);
//! [0]


</snip>
*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class doc_src_qtestevent {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
    QTestEventList events;
    events.addKeyClick('a');
    events.addKeyClick(Qt.Key_Backspace);
    events.addDelay(200);

    QLineEdit ineEdit = new QLineEdit(myParent);
    ...
    events.simulate(lineEdit);
    events.simulate(lineEdit);
//! [0]


    }
}
