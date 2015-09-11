/*   Ported from: doc.src.unicode.qdoc
<snip>
//! [0]
        label->setText("Password:");
//! [0]


//! [1]
        label->setText(tr("Password:"));
//! [1]


//! [2]
        QFile file(QString::fromLatin1("appicon.png"));
//! [2]


//! [3]
        QFile file(QLatin1String("appicon.png"));
//! [3]


</snip>
*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class doc_src_unicode {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        label.setText("Password:");
//! [0]


//! [1]
        label.setText(tr("Password:"));
//! [1]


//! [2]
        QFile file(QString.fromLatin1("appicon.png"));
//! [2]


//! [3]
        QFile file(QLatin1String("appicon.png"));
//! [3]


    }
}
