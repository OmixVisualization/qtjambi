/*   Ported from: src.corelib.codecs.qtextcodecplugin.cpp
<snip>
//! [0]
        QList<QByteArray> MyCodecPlugin::names() const
        {
            return QList<QByteArray> << "IBM01140" << "hp15-tw";
        }

        QTextCodec *MyCodecPlugin::createForName(const QByteArray &name)
        {
            if (name == "IBM01140") {
                return new Ibm01140Codec;
            } else if (name == "hp15-tw") {
                return new Hp15TwCodec;
            }
            return 0;
        }
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


public class src_corelib_codecs_qtextcodecplugin {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        QList<QByteArray> MyCodecPlugin.names()
        {
            return QList<QByteArray> << "IBM01140" << "hp15-tw";
        }

        QTextCodec yCodecPlugin.createForName(QByteArray ame)
        {
            if (name == "IBM01140") {
                return new Ibm01140Codec;
            } else if (name == "hp15-tw") {
                return new Hp15TwCodec;
            }
            return 0;
        }
//! [0]


    }
}
