import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;


public class src_corelib_codecs_qtextcodec {
    public static void main(String args[]) {
        QApplication.initialize(args);
    {
//! [0]
        QByteArray encodedString = new QByteArray("...");
        QTextCodec codec = QTextCodec.codecForName("KOI8-R");
        String string = codec.toUnicode(encodedString);
//! [0]
        if (string != null) {}
    }

    {
//! [1]
        String string = new String("...");
        QTextCodec codec = QTextCodec.codecForName("KOI8-R");
        QByteArray encodedString = codec.fromUnicode(string);
//! [1]
        if (encodedString != null) {}
    }

    {
//! [2]
        QTextCodec codec = QTextCodec.codecForName("Shift-JIS");
        QTextDecoder decoder = codec.makeDecoder();

        String string = new String();
        while (new_data_available()) {
            QByteArray chunk = get_new_data();
            string += decoder.toUnicode(chunk);
        }
//! [2]
    }

    }
    public static boolean new_data_available() { return true;}
    public static QByteArray get_new_data() { return new QByteArray(); }
}
