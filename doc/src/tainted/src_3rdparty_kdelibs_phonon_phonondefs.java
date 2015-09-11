/*   Ported from: src.3rdparty.kdelibs.phonon.phonondefs.h
<snip>
//! [0]

 * class AbstractEffect : public QObject
 * {
 *   Q _OBJECT
 *   Q_PROPERTY(int propertyA READ propertyA WRITE setPropertyA)
 *   PHONON_ABSTRACTBASE(AbstractEffect)
 *   public:
 *     int propertyA() const;
 *     void setPropertyA(int);
 * };
 * //! [0]


//! [1]

 * class EffectSettings : public QObject
 * {
 *   Q _OBJECT
 *   Q_PROPERTY(int propertyA READ propertyA WRITE setPropertyA)
 *   PHONON_OBJECT(EffectSettings)
 *   public:
 *     int propertyA() const;
 *     void setPropertyA(int);
 * };
 * //! [1]


//! [2]

 * class ConcreteEffect : public AbstractEffect
 * {
 *   Q _OBJECT
 *   Q_PROPERTY(int propertyB READ propertyB WRITE setPropertyB)
 *   PHONON_HEIR(ConcreteEffect)
 *   public:
 *     int propertyB() const;
 *     void setPropertyB(int);
 * };
 * //! [2]


</snip>
*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_3rdparty_kdelibs_phonon_phonondefs {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]

 * class AbstractEffect : public QObject
 * {
 *   Q _OBJECT
 *   Q_PROPERTY(int propertyA READ propertyA WRITE setPropertyA)
 *   PHONON_ABSTRACTBASE(AbstractEffect)
 *   public:
 *     int propertyA();
 *     void setPropertyA(int);
 * };
 * //! [0]


//! [1]

 * class EffectSettings : public QObject
 * {
 *   Q _OBJECT
 *   Q_PROPERTY(int propertyA READ propertyA WRITE setPropertyA)
 *   PHONON_OBJECT(EffectSettings)
 *   public:
 *     int propertyA();
 *     void setPropertyA(int);
 * };
 * //! [1]


//! [2]

 * class ConcreteEffect : public AbstractEffect
 * {
 *   Q _OBJECT
 *   Q_PROPERTY(int propertyB READ propertyB WRITE setPropertyB)
 *   PHONON_HEIR(ConcreteEffect)
 *   public:
 *     int propertyB();
 *     void setPropertyB(int);
 * };
 * //! [2]


    }
}
