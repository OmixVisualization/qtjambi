/*   Ported from: src.corelib.kernel.qmetaobject.cpp
<snip>
//! [0]
        class MyClass
        {
            Q_OBJECT
            Q_CLASSINFO("author", "Sabrina Schweinsteiger")
            Q_CLASSINFO("url", "http://doc.moosesoft.co.uk/1.0/")

        public:
            ...
        };
//! [0]


//! [1]
    QByteArray normType = QMetaObject::normalizedType(" int    const  *");
    // normType is now "const int*"
//! [1]


//! [2]
        QMetaObject::invokeMethod(pushButton, "animateClick",
                                  Qt::QueuedConnection);
//! [2]


//! [3]
        QMetaObject::invokeMethod: Unable to handle unregistered datatype 'MyType'
//! [3]


//! [4]
        QString retVal;
        QMetaObject::invokeMethod(obj, "compute", Qt::DirectConnection,
                                  Q_RETURN_ARG(QString, retVal),
                                  Q_ARG(QString, "sqrt"),
                                  Q_ARG(int, 42),
                                  Q_ARG(double, 9.7));
//! [4]


//! [5]
        class MyClass
        {
            Q_OBJECT
            Q_CLASSINFO("author", "Sabrina Schweinsteiger")
            Q_CLASSINFO("url", "http://doc.moosesoft.co.uk/1.0/")

        public:
            ...
        };
//! [5]


</snip>
*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_corelib_kernel_qmetaobject {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        class MyClass
        {
            Q_OBJECT
            Q_CLASSINFO("author", "Sabrina Schweinsteiger")
            Q_CLASSINFO("url", "http://doc.moosesoft.co.uk/1.0/")

        public:
            ...
        };
//! [0]


//! [1]
    QByteArray normType = QMetaObject.normalizedType(" int     *");
    // normType is now "int*"
//! [1]


//! [2]
        QMetaObject.invokeMethod(pushButton, "animateClick",
                                  Qt.QueuedConnection);
//! [2]


//! [3]
        QMetaObject.invokeMethod: Unable to handle unregistered datatype 'MyType'
//! [3]


//! [4]
        StringsretVal;
        QMetaObject.invokeMethod(obj, "compute", Qt.DirectConnection,
                                  Q_RETURN_ARG(QString, retVal),
                                  Q_ARG(QString, "sqrt"),
                                  Q_ARG(int, 42),
                                  Q_ARG(double, 9.7));
//! [4]


//! [5]
        class MyClass
        {
            Q_OBJECT
            Q_CLASSINFO("author", "Sabrina Schweinsteiger")
            Q_CLASSINFO("url", "http://doc.moosesoft.co.uk/1.0/")

        public:
            ...
        };
//! [5]


    }
}
