import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_corelib_tools_qbytearray {
    public static void main(String args[]) {
        QApplication.initialize(args);

        {
//! [0]
        QByteArray ba = new QByteArray("Hello");
//! [0]


/*
 * Commented out as operator[] is not avalable in Qt/Jambi
 *
//! [1]
        QByteArray ba;
        ba.resize(5);
        ba[0] = 0x3c;
        ba[1] = 0xb8;
        ba[2] = 0x64;
        ba[3] = 0x18;
        ba[4] = 0xca;
//! [1]
*/


//! [2]
        for (int i = 0; i < ba.size(); ++i) {
            if (ba.at(i) >= 'a' && ba.at(i) <= 'f')
                System.out.println("Found character in range [a-f]");
        }
//! [2]
        }


        {
//! [3]
        QByteArray x = new QByteArray("and");
        x.prepend("rock ");         // x == "rock and"
        x.append(" roll");          // x == "rock and roll"
        x.replace(5, 3, new QByteArray("&"));       // x == "rock & roll"
//! [3]
        }


        {
//! [4]
        QByteArray ba = new QByteArray("We must be <b>bold</b>, very <b>bold</b>");
        int j = 0;
        while ((j = ba.indexOf("<b>", j)) != -1) {
            System.out.println("Found <b> tag at index position " + j);
            ++j;
        }
//! [4]
        }


        {
//! [5]
        new QByteArray().isNull();          // returns true
        new QByteArray().isEmpty();         // returns true

        new QByteArray("").isNull();        // returns false
        new QByteArray("").isEmpty();       // returns true

        new QByteArray("abc").isNull();     // returns false
        new QByteArray("abc").isEmpty();    // returns false
//! [5]
        }


        {
//! [6]
        QByteArray ba = new QByteArray("Hello");
        int n = ba.size();          // n == 5
        byte a = ba.at(0);               // returns 'H'
        byte b = ba.at(4);               // returns 'o'
        byte c = ba.at(5);               // returns '\0'
//! [6]
        }


        {
//! [7]
        new QByteArray().isEmpty();         // returns true
        new QByteArray("").isEmpty();       // returns true
        new QByteArray("abc").isEmpty();    // returns false
//! [7]
        }


        {
//! [8]
        QByteArray ba = new QByteArray("Hello world");
        QNativePointer data = ba.data();

        int i = 0;
        while (data.byteAt(i) != 0) {
            System.out.println("[" + data.byteAt(i) + "]");
            ++i;
        }
//! [8]
        }


/*
 * Commented out since operator[] is not available in Qt/Jambi
 *
//! [9]
        QByteArray ba;
        for (int i = 0; i < 10; ++i)
            ba[i] = 'A' + i;
        // ba == "ABCDEFGHIJ"
//! [9]
*/


        {
//! [10]
        QByteArray ba = new QByteArray("Stockholm");
        ba.truncate(5);             // ba == "Stock"
//! [10]
        }


        {
//! [11]
        QByteArray ba = new QByteArray("STARTTLS\r\n");
        ba.chop(2);                 // ba == "STARTTLS"
//! [11]
        }


/*
 * Commented out since operator+= is not available in Qt/Jambi
 *
//! [12]
        QByteArray x("free");
        QByteArray y("dom");
        x += y;
        // x == "freedom"
//! [12]
*/



        {
//! [13]
        new QByteArray().isNull();          // returns true
        new QByteArray("").isNull();        // returns false
        new QByteArray("abc").isNull();     // returns false
//! [13]
        }


        {
//! [14]
        QByteArray ba = new QByteArray("Istambul");
        ba.fill((byte)'o');
        // ba == "oooooooo"

        ba.fill((byte)'X', 2);
        // ba == "XX"
//! [14]
        }


        {
//! [15]
        QByteArray x = new QByteArray("ship");
        QByteArray y = new QByteArray("air");
        x.prepend(y);
        // x == "airship"
//! [15]
        }


        {
//! [16]
        QByteArray x = new QByteArray("free");
        QByteArray y = new QByteArray("dom");
        x.append(y);
        // x == "freedom"
//! [16]
        }


        {
//! [17]
        QByteArray ba = new QByteArray("Meal");
        ba.insert(1, new QByteArray("ontr"));
        // ba == "Montreal"
//! [17]
        }


        {
//! [18]
        QByteArray ba = new QByteArray("Montreal");
        ba.remove(1, 4);
        // ba == "Meal"
//! [18]
        }


        {
//! [19]
        QByteArray x = new QByteArray("Say yes!");
        QByteArray y = new QByteArray("no");
        x.replace(4, 3, y);
        // x == "Say no!"
//! [19]
        }


        {
//! [20]
        QByteArray ba = new QByteArray("colour behaviour flavour neighbour");
        ba.replace(new QByteArray("ou"), new QByteArray("o"));
        // ba == "color behavior flavor neighbor"
//! [20]
        }


        {
//! [21]
        QByteArray x = new QByteArray("sticky question");
        QByteArray y = new QByteArray("sti");
        x.indexOf(y);               // returns 0
        x.indexOf(y, 1);            // returns 10
        x.indexOf(y, 10);           // returns 10
        x.indexOf(y, 11);           // returns -1
//! [21]
        }


        {
//! [22]
        QByteArray ba = new QByteArray("ABCBA");
        ba.indexOf("B");            // returns 1
        ba.indexOf("B", 1);         // returns 1
        ba.indexOf("B", 2);         // returns 3
        ba.indexOf("X");            // returns -1
//! [22]
        }


        {
//! [23]
        QByteArray x = new QByteArray("crazy azimuths");
        QByteArray y = new QByteArray("azy");
        x.lastIndexOf(y);           // returns 6
        x.lastIndexOf(y, 6);        // returns 6
        x.lastIndexOf(y, 5);        // returns 2
        x.lastIndexOf(y, 1);        // returns -1
//! [23]
        }


        {
//! [24]
        QByteArray ba = new QByteArray("ABCBA");
        ba.lastIndexOf("B");        // returns 3
        ba.lastIndexOf("B", 3);     // returns 3
        ba.lastIndexOf("B", 2);     // returns 1
        ba.lastIndexOf("X");        // returns -1
//! [24]
        }


        {
//! [25]
        QByteArray url = new QByteArray("ftp://ftp.trolltech.com/");
        if (url.startsWith("ftp:"))
            System.out.println("foo");
            // ...
//! [25]
        }


        {
//! [26]
        QByteArray url = new QByteArray("http://www.trolltech.com/index.html");
        if (url.endsWith(".html"))
            System.out.println("foo");
            // ...
//! [26]
        }


        {
//! [27]
        QByteArray x = new QByteArray("Pineapple");
        QByteArray y = x.left(4);
        // y == "Pine"
//! [27]
        }


        {
//! [28]
        QByteArray x = new QByteArray("Pineapple");
        QByteArray y = x.right(5);
        // y == "apple"
//! [28]
        }


        {
//! [29]
        QByteArray x = new QByteArray("Five pineapples");
        QByteArray y = x.mid(5, 4);     // y == "pine"
        QByteArray z = x.mid(5);        // z == "pineapples"
//! [29]
        }


        {
//! [30]
        QByteArray x = new QByteArray("TROlltECH");
        QByteArray y = x.toLower();
        // y == "trolltech"
//! [30]
        }


        {
//! [31]
        QByteArray x = new QByteArray("TROlltECH");
        QByteArray y = x.toUpper();
        // y == "TROLLTECH"
//! [31]
        }


        {
//! [32]
        QByteArray ba = new QByteArray("  lots\t of\nwhitespace\r\n ");
        ba = ba.simplified();
        // ba == "lots of whitespace";
//! [32]
        }


        {
//! [33]
        QByteArray ba = new QByteArray("  lots\t of\nwhitespace\r\n ");
        ba = ba.trimmed();
        // ba == "lots\t of\nwhitespace";
//! [33]
        }


        {
//! [34]
        QByteArray x = new QByteArray("apple");
        QByteArray y = x.leftJustified(8, (byte)'.', false);   // y == "apple..."
//! [34]
        }


        {
//! [35]
        QByteArray x = new QByteArray("apple");
        QByteArray y = x.rightJustified(8, (byte)'.', false);    // y == "...apple"
//! [35]
        }


        {
//! [36]
        QByteArray str = new QByteArray("FF");
        int hex = str.toInt(16);     // hex == 255
        int dec = str.toInt();     // dec == 0
//! [36]
        }


/*
 * Commented out as toLong() is not part of the Qt/Jambi API
 *
//! [37]
        QByteArray str("FF");
        boolean ok;
        long hex = str.toLong(&ok, 16);   // hex == 255, ok == true
        long dec = str.toLong(&ok, 10);   // dec == 0, ok == false
//! [37]
*/


        {
//! [38]
        QByteArray string = new QByteArray("1234.56");
        double a = string.toDouble();   // a == 1234.56
//! [38]
        }


        {
//! [39]
        QByteArray text = new QByteArray("Qt is great!");
        text.toBase64();        // returns "UXQgaXMgZ3JlYXQh"
//! [39]
        }


        {
//! [40]
        QByteArray ba = new QByteArray();
        int n = 63;
        ba.setNum(n);           // ba == "63"
        ba.setNum(n, 16);       // ba == "3f"
//! [40]
        }


        {
//! [41]
        int n = 63;
        QByteArray.number(n);              // returns "63"
        QByteArray.number(n, 16);          // returns "3f"
        QByteArray.number(n, 16).toUpper();  // returns "3F"
//! [41]
        }


        {
//! [42]
        QByteArray ba = QByteArray.number(12.3456, (byte)'E', 3);
        // ba == 1.235E+01
//! [42]
        }


/*
 * Commented out as fromRawData() is not part of Qt/Jambi API
 *
//! [43]
        static const char mydata[] = {
            0x00, 0x00, 0x03, 0x84, 0x78, 0x9c, 0x3b, 0x76,
            0xec, 0x18, 0xc3, 0x31, 0x0a, 0xf1, 0xcc, 0x99,
            ...
            0x6d, 0x5b
        };


        QByteArray data = QByteArray::fromRawData(mydata, sizeof(mydata));
        QDataStream in(&data, QIODevice::ReadOnly);
        ...
//! [43]
*/


        {
//! [44]
        QByteArray text = QByteArray.fromBase64(new QByteArray("UXQgaXMgZ3JlYXQh"));
        text.data();            // returns "Qt is great!"
//! [44]
        }


        {
//! [45]
        QByteArray text = QByteArray.fromHex(new QByteArray("517420697320677265617421"));
        text.data();            // returns "Qt is great!"
//! [45]
        }


    }
}
