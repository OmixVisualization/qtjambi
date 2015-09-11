import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.QApplication;
import org.qtjambi.qt.xmlpatterns.*;


public class src_xmlpatterns_api_qxmlquery {
    public static void main(String args[]) {
        QApplication.initialize(args);
        String localName = "myName";
        QIODevice device;
        {
            QXmlItem value = new QXmlItem();
            QXmlQuery query = new QXmlQuery();
//! [0]
    query.bindVariable(new QXmlName(query.namePool(), localName), value);
//! [0]
        }


//! [1]
    QByteArray myDocument = new QByteArray();
    QBuffer buffer = new QBuffer(myDocument); // This is a QIODevice.
    buffer.open(QIODevice.OpenModeFlag.ReadOnly);
    QXmlQuery query = new QXmlQuery();
    query.bindVariable("myDocument", buffer);
    query.setQuery("declare variable $myDocument external; doc($myDocument)");
//! [1]

//! [2]
    query.bindVariable(new QXmlName(query.namePool(), localName), device);
//! [2]


    }
}
