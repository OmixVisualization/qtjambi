import org.qtjambi.qt.core.QIODevice;
import org.qtjambi.qt.gui.QApplication;
import org.qtjambi.qt.xmlpatterns.QXmlQuery;
import org.qtjambi.qt.xmlpatterns.QXmlSerializer;


public class src_xmlpatterns_api_qxmlserializer {
    public static void main(String args[]) {
        QApplication.initialize(args);
        QIODevice myOutputDevice;
//! [0]
  QXmlQuery query = new QXmlQuery();
  query.setQuery("doc('index.html')/html/body/p[1]");

  QXmlSerializerPointer serializer = new QXmlSerializerPointer(new QXmlSerializer(query, myOutputDevice));
  query.evaluateToReceiver(serializer);
//! [0]


    }
}
