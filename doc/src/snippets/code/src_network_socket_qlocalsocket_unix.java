import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.network.*;


public class src_network_socket_qlocalsocket_unix {
    public static void main(String args[]) {
        QApplication.initialize(args);
        QLocalSocket socket = null;
//! [0]
        socket.connectToServer("market");
        if (socket.waitForConnected(1000))
            System.out.println("Connected!");
//! [0]


//! [1]
        socket.disconnectFromServer();
        if (socket.waitForDisconnected(1000))
            System.out.println("Disconnected!");
//! [1]


    }
}
