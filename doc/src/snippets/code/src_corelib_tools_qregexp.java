import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;
import java.util.*;


public class src_corelib_tools_qregexp {
    public static void main(String args[]) {
        QApplication.initialize(args);
    }
    public void foo1()
    {
//! [0]
        QRegExp rx = new QRegExp("(\\d+)");
        String str = "Offsets: 12 14 99 231 7";
        LinkedList<String> list = new LinkedList<String>();
        int pos = 0;

        while ((pos = rx.indexIn(str, pos)) != -1) {
            list.add(rx.cap(1));
            pos += rx.matchedLength();
        }
        // list: ["12", "14", "99", "231", "7"]
//! [0]
    }

    public void foo2()
    {
//! [1]
        QRegExp rx = new QRegExp("*.txt");
        rx.setPatternSyntax(QRegExp.PatternSyntax.Wildcard);
        rx.exactMatch("README.txt");        // returns true
        rx.exactMatch("welcome.txt.bak");   // returns false
//! [1]
    }


//! [2]
// QRegExp
//! [2]


    public void foo3()
    {
//! [3]
        QRegExp mark = new QRegExp("\\b"      // word boundary
                 + "[Mm]ark"  // the word we want to match
                   );
//! [3]
    }

    public void foo4()
    {
//! [4]
        QRegExp rx = new QRegExp("^\\d\\d?$");  // match integers 0 to 99
        rx.indexIn("123");                      // returns -1 (no match)
        rx.indexIn("-6");                       // returns -1 (no match)
        rx.indexIn("6");                        // returns 0 (matched as position 0)
//! [4]
    }

    public void foo5()
    {
//! [5]
        QRegExp rx = new QRegExp("^\\S+$"); // match strings without whitespace
        rx.indexIn("Hello world");          // returns -1 (no match)
        rx.indexIn("This_is-OK");           // returns 0 (matched at position 0)
//! [5]
    }

    public void foo6()
    {
//! [6]
        QRegExp rx = new QRegExp("\\b(mail|letter|correspondence)\\b");
        rx.indexIn("I sent you an email");     // returns -1 (no match)
        rx.indexIn("Please write the letter"); // returns 17
//! [6]


//! [7]
        String captured = rx.cap(1); // captured == "letter"
//! [7]
    }

    public void foo7()
    {
//! [8]
        /*QRegExp rx = new QRegExp("&(?!amp;)"); // match ampersands but not mp;
        String line1 = "This & that";
        line1.replace(rx, "mp;");
        // line1 == "This mp; that"
        String line2 = "His mp; hers & theirs";
        line2.replace(rx, "mp;");
        // line2 == "His mp; hers mp; theirs"*/
//! [8]
    }

    public void foo8()
    {
//! [9]
        String str = "One Eric another Eirik, and an Ericsson. "
               + "How many Eiriks, Eric?";
        QRegExp rx = new QRegExp("\\b(Eric|Eirik)\\b"); // match Eric or Eirik
        int pos = 0;    // where we are in the string
        int count = 0;  // how many Eric and Eirik's we've counted
        while (pos >= 0) {
            pos = rx.indexIn(str, pos);
            if (pos >= 0) {
                ++pos;      // move along in str
                ++count;    // count our Eric or Eirik
            }
        }
//! [9]


//! [10]
        str = "Nokia\twww.trolltech.com\tNorway";
        String company, web, country;
        rx.setPattern("^([^\t]+)\t([^\t]+)\t([^\t]+)$");
        if (rx.indexIn(str) != -1) {
            company = rx.cap(1);
            web = rx.cap(2);
            country = rx.cap(3);
        }
//! [10]


//! [11]
        String[] field = str.split("\t");
//! [11]
    }

    public void foo9()
    {
//! [12]
        QRegExp rx = new QRegExp("*.html");
        rx.setPatternSyntax(QRegExp.PatternSyntax.Wildcard);
        rx.exactMatch("index.html");                // returns true
        rx.exactMatch("default.htm");               // returns false
        rx.exactMatch("readme.txt");                // returns false
//! [12]
    }

    public void foo10()
    {
//! [13]
        String str = "offsets: 1.23 .50 71.00 6.00";
        QRegExp rx = new QRegExp("\\d*\\.\\d+"); // primitive floating point matching
        int count = 0;
        int pos = 0;
        while ((pos = rx.indexIn(str, pos)) != -1) {
            ++count;
            pos += rx.matchedLength();
        }
        // pos will be 9, 14, 18 and finally 24; count will end up as 4
//! [13]
    }


    public void foo11()
    {
//! [14]
        QRegExp rx = new QRegExp("(\\d+)(\\s*)(cm|inch(es)?)");
        int pos = rx.indexIn("Length: 36 inches");
        List<String> list = rx.capturedTexts();
        // list is now ("36 inches", "36", " ", "inches", "es")
//! [14]
    }


    public void foo12()
    {
//! [15]
        QRegExp rx = new QRegExp("(\\d+)(?:\\s*)(cm|inch(?:es)?)");
        int pos = rx.indexIn("Length: 36 inches");
        List<String> list = rx.capturedTexts();
        // list is now ("36 inches", "36", "inches")
//! [15]
    }

    public void foo12b()
    {
        QRegExp rx = new QRegExp("(\\d+)(?:\\s*)(cm|inch(?:es)?)");
//! [16]
        List<String> list = rx.capturedTexts();
        ListIterator<String> it = list.listIterator();
        while (it.hasNext()) {
        String t = it.next();
            //myProcessing();
        }
//! [16]
    }


    public void foo13()
    {
//! [17]
        QRegExp rxlen = new QRegExp("(\\d+)(?:\\s*)(cm|inch)");
        int pos = rxlen.indexIn("Length: 189cm");
        if (pos > -1) {
            String value = rxlen.cap(1); // "189"
            String unit = rxlen.cap(2);  // "cm"
            // ...
        }
//! [17]
    }


    public void foo14()
    {
//! [18]
        QRegExp rx = new QRegExp("/([a-z]+)/([a-z]+)");
        rx.indexIn("Output /dev/null");   // returns 7 (position of /dev/null)
        rx.pos(0);                        // returns 7 (position of /dev/null)
        rx.pos(1);                        // returns 8 (position of dev)
        rx.pos(2);                        // returns 12 (position of null)
//! [18]
    }


    public void foo15()
    {
    String s1;
    String s2;
//! [19]
        s1 = QRegExp.escape("bingo");   // s1 == "bingo"
        s2 = QRegExp.escape("f(x)");    // s2 == "f\\(x\\)"
//! [19]
    }

    public void foo16()
    {
    String name = new String();
    String alias = new String();
//! [20]
        QRegExp rx = new QRegExp("(" + QRegExp.escape(name) +
                 "|" + QRegExp.escape(alias) + ")");
//! [20]
    }
}
