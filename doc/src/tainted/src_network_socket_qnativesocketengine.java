/*   Ported from: src.network.socket.qnativesocketengine.cpp
<snip>
//! [0]
        QNativeSocketEngine socketLayer;
        socketLayer.initialize(QAbstractSocket::TcpSocket, QAbstractSocket::IPv4Protocol);
        socketLayer.connectToHost(QHostAddress::LocalHost, 22);
        // returns false

        socketLayer.waitForWrite();
        socketLayer.connectToHost(QHostAddress::LocalHost, 22);
        // returns true
//! [0]


//! [1]
        QNativeSocketEngine socketLayer;
        socketLayer.bind(QHostAddress::Any, 4000);
        socketLayer.listen();
        if (socketLayer.waitForRead()) {
            int clientSocket = socketLayer.accept();
            // a client is connected
        }
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


public class src_network_socket_qnativesocketengine {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        QNativeSocketEngine socketLayer;
        socketLayer.initialize(QAbstractSocket.TcpSocket, QAbstractSocket.IPv4Protocol);
        socketLayer.connectToHost(QHostAddress.LocalHost, 22);
        // returns false

        socketLayer.waitForWrite();
        socketLayer.connectToHost(QHostAddress.LocalHost, 22);
        // returns true
//! [0]


//! [1]
        QNativeSocketEngine socketLayer;
        socketLayer.bind(QHostAddress.Any, 4000);
        socketLayer.listen();
        if (socketLayer.waitForRead()) {
            int clientSocket = socketLayer.accept();
            // a client is connected
        }
//! [1]


    }
}
