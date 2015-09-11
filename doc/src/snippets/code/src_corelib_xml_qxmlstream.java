import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;


public class src_corelib_xml_qxmlstream {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        QXmlStreamReader xml = new QXmlStreamReader();
        //...
        while (!xml.atEnd()) {
            xml.readNext();
            // do processing ...
        }
        if (xml.hasError()) {
            // do error handling ...
        }
//! [0]


        new QXmlStreamWriter() {
            private final void _writeTextElement(String qualifiedName,
                                                 String text) {
//! [1]
        writeStartElement(qualifiedName);
        writeCharacters(text);
        writeEndElement();
//! [1]
            }


            private final void _writeTextElement(String namespaceUri,
                                                 String name,
                                                 String text) {
//! [2]
        writeStartElement(namespaceUri, name);
        writeCharacters(text);
        writeEndElement();
//! [2]
            }
        };
    }
}
