import org.qtjambi.qt.core.QByteArray;
import org.qtjambi.qt.core.QUrl;
import org.qtjambi.qt.gui.QApplication;
import org.qtjambi.qt.network.QFtp;


public class src_corelib_io_qurl {
    public static void main(String args[]) {
        QApplication.initialize(args);
        {
            code();
//! [0]
        QUrl url = new QUrl("http://www.example.com/List of holidays.xml");
        // url.toEncoded() == "http://www.example.com/List%20of%20holidays.xml"
//! [0]
        }


//! [1]
        QUrl url = QUrl.fromEncoded(new QByteArray("http://www.trolltech.com/List%20of%20holidays.xml"));
//! [1]

    }
//! [2]
        private boolean checkUrl(QUrl url) {
            if (!url.isValid()) {
                System.out.println("Invalid URL: " + url);
                return false;
            }

            return true;
        }
//! [2]

        static void code() {
            QUrl url = new QUrl();
//! [3]
        QFtp ftp = new QFtp();
        ftp.connectToHost(url.host(), (char)url.port(21));
//! [3]

/*
//! [4]
        http://www.example.com/cgi-bin/drawgraph.cgi?type-pie/color-green
//! [4]
*/

//! [5]
         QUrl baseUrl = new QUrl("http://www.trolltech.com/support");
         QUrl relativeUrl = new QUrl("../products/solutions");
         System.out.println(baseUrl.resolved(relativeUrl).toString());
         // prints "http://www.trolltech.com/products/solutions"
//! [5]


//! [6]
         QByteArray ba = QUrl.toPercentEncoding("{a fishy string?}", new QByteArray("{}"), new QByteArray("s"));
         System.out.println(ba);
         // prints "{a fi%73hy %73tring%3F}"
//! [6]


    }
}
