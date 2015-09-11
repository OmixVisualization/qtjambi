import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_network_kernel_qhostaddress {
    public static void main(String args[]) {
        QApplication.initialize(args);
    QHostAddress hostAddr = new QHostAddress();
//! [0]
        QIPv6Address addr = hostAddr.toIPv6Address();
        // addr contains 16 unsigned characters

        for (int i = 0; i < 16; ++i) {
            // process addr.c[i]
        }
//! [0]


    }
}
