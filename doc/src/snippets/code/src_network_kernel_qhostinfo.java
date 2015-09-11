import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_network_kernel_qhostinfo {
    public class MyWidget extends QObject {

    public void foo1()
    {
//! [0]
        // To find the IP address of www.trolltech.com
        QHostInfo.lookupHost("www.trolltech.com",
                 this, "printResults(QHostInfo)");

        // To find the host name for 4.2.2.1
        QHostInfo.lookupHost("4.2.2.1",
                 this, "printResults(QHostInfo)");
//! [0]


//! [1]
        QHostInfo info = QHostInfo.fromName("www.trolltech.com");
//! [1]


//! [2]
        QHostInfo.lookupHost("www.kde.org",
                 this, "lookedUp(QHostInfo)");
//! [2]


//! [4]
        QHostInfo.lookupHost("4.2.2.1",
                 this, "lookedUp(QHostInfo)");
//! [4]

    }
    public void foo2()
    {

//! [5]
        QHostInfo info = new QHostInfo();
        // ...
        if (!info.addresses().isEmpty()) {
        QHostAddress address = info.addresses().get(0);
        // use the first IP address
        }
//! [5]

    }


//! [3]
        public void lookedUp(QHostInfo host) {
            if (host.error() != QHostInfo.HostInfoError.NoError) {
                System.out.println("Lookup failed:");
                System.out.println(host.errorString());
                return;
            }

            for (int c = 0; c < host.addresses().size(); c++) {
                QHostAddress address = host.addresses().get(c);
                System.out.println("Found address:");
                System.out.println(address.toString());
            }
        }
//! [3]

    }

}
