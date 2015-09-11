import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.xmlpatterns.QXmlFormatter;
import org.qtjambi.qt.xmlpatterns.QXmlQuery;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_xmlpatterns_api_qxmlformatter {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
   QXmlQuery query = new QXmlQuery();
   query.setQuery("doc('index.html')/html/body/p[1]");

   QXmlFormatterPointer formatter(new QXmlFormatter(query, myOutputDevice));
   formatter.setIndentationDepth(2);
   query.evaluateToReceiver(formatter);
//! [0]


    }
}
