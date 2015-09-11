import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_xml_sax_qxml {
    public static void main(String args[]) {
        QApplication.initialize(args);
        QXmlSimpleReader xmlReader = new QXmlSimpleReader();
//! [0]
        xmlReader.setFeature("http://xml.org/sax/features/namespace-prefixes", true);
//! [0]

    }
}
