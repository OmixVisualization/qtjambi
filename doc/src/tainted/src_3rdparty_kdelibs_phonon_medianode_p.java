/*   Ported from: src.3rdparty.kdelibs.phonon.medianode_p.h
<snip>
//! [0]

        * Q_Q(ClassName);
        * m_iface = Factory::createClassName(this);
        * return m_iface;
        * //! [0]


</snip>
*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_3rdparty_kdelibs_phonon_medianode_p {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]

        * Q_Q(ClassName);
        * m_iface = Factory.createClassName(this);
        * return m_iface;
        * //! [0]


    }
}
