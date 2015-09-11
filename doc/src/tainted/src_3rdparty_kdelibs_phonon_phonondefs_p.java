/*   Ported from: src.3rdparty.kdelibs.phonon.phonondefs_p.h
<snip>
//! [0]

     * ParentClassPrivate::setupBackendObject();
     * m_iface->setPropertyA(d->propertyA);
     * m_iface->setPropertyB(d->propertyB);
     * //! [0]


//! [1]

     * ParentClassPrivate::setupBackendObject();
     * m_iface->setPropertyA(d->propertyA);
     * m_iface->setPropertyB(d->propertyB);
     * //! [1]


</snip>
*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_3rdparty_kdelibs_phonon_phonondefs_p {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]

     * ParentClassPrivate.setupBackendObject();
     * m_iface.setPropertyA(d.propertyA);
     * m_iface.setPropertyB(d.propertyB);
     * //! [0]


//! [1]

     * ParentClassPrivate.setupBackendObject();
     * m_iface.setPropertyA(d.propertyA);
     * m_iface.setPropertyB(d.propertyB);
     * //! [1]


    }
}
