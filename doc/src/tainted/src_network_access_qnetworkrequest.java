/*   Ported from: src.network.access.qnetworkrequest.cpp
<snip>
//! [0]
      request.setRawHeader("Last-Modified", "Sun, 06 Nov 1994 08:49:37 GMT");
//! [0]


</snip>
*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_network_access_qnetworkrequest {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
      request.setRawHeader("Last-Modified", "Sun, 06 Nov 1994 08:49:37 GMT");
//! [0]


    }
}
