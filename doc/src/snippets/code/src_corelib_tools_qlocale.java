import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_corelib_tools_qlocale {
    public static void main(String args[]) {
        QApplication.initialize(args);

        if (true)
        {
//! [0]
        QLocale egyptian = new QLocale(QLocale.Language.Arabic, QLocale.Country.Egypt);
        String s1 = egyptian.toString(1.571429E+07, (byte) 'e');
        String s2 = egyptian.toString(10);

        double d = egyptian.toDouble(s1);
        int i = egyptian.toInt(s2);
//! [0]
        }

        if (true)
        {
//! [1]
        QLocale.setDefault(new QLocale(QLocale.Language.Hebrew, QLocale.Country.Israel));
        QLocale hebrew = new QLocale(); // Constructs a default QLocale
        String s1 = hebrew.toString(15714.3, (byte) 'e');

        double d;

        d = hebrew.toDouble("1234,56");   // d == 0.0
        d = hebrew.toDouble("1234.56");   // d == 1234.56
//! [1]
        }

        if (true)
        {
//! [2]
        QLocale korean = new QLocale("ko");
        QLocale swiss = new QLocale("de_CH");
//! [2]
        }

        if (true)
        {
//! [3]
        double d;

        QLocale c = new QLocale(QLocale.Language.C);
        d = c.toDouble( "1234.56");  // d == 1234.56
        d = c.toDouble( "1,234.56"); // d == 1234.56
        d = c.toDouble( "1234,56");  // ok == 0.0

        QLocale german = new QLocale(QLocale.Language.German);
        d = german.toDouble( "1234,56");  // d == 1234.56
        d = german.toDouble( "1.234,56"); // d == 1234.56
        d = german.toDouble( "1234.56");  // d == 0.0

        d = german.toDouble( "1.234");    // d == 1234.0
//! [3]
        }

    }
}
