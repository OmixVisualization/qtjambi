import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_network_access_qhttp {
    public static void main(String args[]) {
        QApplication.initialize(args);
    /*
//! [0]
    content-type: text/html
//! [0]
    */
/*
//! [1]
    header.setValue("content-type", "text/html");
    StringscontentType = header.value("content-type");
//! [1]
*/

//! [2]
    QHttp http = new QHttp();
    QHttpRequestHeader header = new QHttpRequestHeader("GET", "/index.html");
    header.setValue("Host", "www.trolltech.com");
    http.setHost("www.trolltech.com", 80);
    http.request(header);
//! [2]


//! [3]
    http.setHost("www.trolltech.com", 80); // id == 1
    http.get("/index.html");           // id == 2
//! [3]

/*
//! [4]
    requestStarted(1)
    requestFinished(1, false)

    requestStarted(2)
    stateChanged(Connecting)
    stateChanged(Sending)
    dataSendProgress(77, 77)
    stateChanged(Reading)
    responseHeaderReceived(responseheader)
    dataReadProgress(5388, 0)
    readyRead(responseheader)
    dataReadProgress(18300, 0)
    readyRead(responseheader)
    stateChanged(Connected)
    requestFinished(2, false)

    done(false)

    stateChanged(Closing)
    stateChanged(Unconnected)
//! [4]
*/

QByteArray data = new QByteArray();
//! [5]
    http.setHost("www.foo.bar", 80);       // id == 1
    http.get("/index.html");           // id == 2
    http.post("register.html", data);  // id == 3
//! [5]


/*
//! [6]
    requestStarted(1)
    requestFinished(1, false)

    requestStarted(2)
    stateChanged(HostLookup)
    requestFinished(2, true)

    done(true)

    stateChanged(Unconnected)
//! [6]
*/

class foo extends QObject {
    private QHttp http;
    private void display(QByteArray a) {}
//! [7]
      public void getTicks()
      {
        http = new QHttp(this);
        http.done.connect(this, "showPage()");
        http.setProxy("proxy.example.com", 3128);
        http.setHost("ticker.example.com", 80);
        http.get("/ticks.asp");
      }

      void showPage()
      {
        display(http.readAll());
      }
//! [7]
    } //class

    }
}
