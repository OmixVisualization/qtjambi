import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_network_socket_qtcpserver {
    public static void main(String args[]) {
        QApplication.initialize(args);
        QTcpServer server = new QTcpServer();
//! [0]
        server.setProxy(new QNetworkProxy(QNetworkProxy.ProxyType.NoProxy, new String(), 0));
//! [0]


    }
}
