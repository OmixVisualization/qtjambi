/*   Ported from: src.network.kernel.qnetworkproxy.cpp
<snip>
//! [0]
        QNetworkProxy proxy;
        proxy.setType(QNetworkProxy::Socks5Proxy);
        proxy.setHostName("proxy.example.com");
        proxy.setPort(1080);
        proxy.setUser("username");
        proxy.setPassword("password");
        QNetworkProxy::setApplicationProxy(proxy);
//! [0]


//! [1]
        serverSocket->setProxy(QNetworkProxy::NoProxy);
//! [1]


</snip>
*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_network_kernel_qnetworkproxy {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        QNetworkProxy proxy;
        proxy.setType(QNetworkProxy.Socks5Proxy);
        proxy.setHostName("proxy.example.com");
        proxy.setPort(1080);
        proxy.setUser("username");
        proxy.setPassword("password");
        QNetworkProxy.setApplicationProxy(proxy);
//! [0]


//! [1]
        serverSocket.setProxy(QNetworkProxy.NoProxy);
//! [1]


    }
}
