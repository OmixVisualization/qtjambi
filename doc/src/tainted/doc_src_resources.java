/*   Ported from: doc.src.resources.qdoc
<snip>
//! [0]
        <file alias="cut-img.png">images/cut.png</file>
//! [0]


//! [1]
        <qresource prefix="/myresources">
            <file alias="cut-img.png">images/cut.png</file>
        </qresource>
//! [1]


//! [2]
        <qresource>
            <file>cut.jpg</file>
        </qresource>
        <qresource lang="fr">
            <file alias="cut.jpg">cut_fr.jpg</file>
        </qresource>
//! [2]


//! [3]
    rcc -binary myresource.qrc -o myresource.rcc
//! [3]


//! [4]
    QResource::registerResource("/path/to/myresource.rcc");
//! [4]


//! [5]
        int main(int argc, char *argv[])
        {
            QApplication app(argc, argv);
            Q_INIT_RESOURCE(graphlib);
            ...
            return app.exec();
        }
//! [5]


</snip>
*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class doc_src_resources {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        <file alias="cut-img.png">images/cut.png</file>
//! [0]


//! [1]
        <qresource prefix="/myresources">
            <file alias="cut-img.png">images/cut.png</file>
        </qresource>
//! [1]


//! [2]
        <qresource>
            <file>cut.jpg</file>
        </qresource>
        <qresource lang="fr">
            <file alias="cut.jpg">cut_fr.jpg</file>
        </qresource>
//! [2]


//! [3]
    rcc -binary myresource.qrc -o myresource.rcc
//! [3]


//! [4]
    QResource.registerResource("/path/to/myresource.rcc");
//! [4]


//! [5]
        int main(int argc, char rgv[])
        {
            QApplication app(argc, argv);
            Q_INIT_RESOURCE(graphlib);
            ...
            return app.exec();
        }
//! [5]


    }
}
