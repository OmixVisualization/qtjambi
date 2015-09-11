import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_network_ssl_qsslcertificate {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
    for (QSslCertificate cert : QSslCertificate.fromPath("C:/ssl/certificate.*.pem",
                                                             QRegExp.Wildcard)) {
        qDebug() << cert.issuerInfo(QSslCertificate.Organization);
    }
//! [0]


    }
}
