/*   Ported from: src.corelib.thread.qatomic.h
<snip>
//! [0]
        MyClass &MyClass:operator=(const MyClass &other)
        { qAtomicAssign(d, other.d); return *this; }
//! [0]


//! [1]
        void MyClass::detach()
        { qAtomicDetach(d); }
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


public class src_corelib_thread_qatomic {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        MyClass yClass:operator=(MyClass ther)
        { qAtomicAssign(d, other.d); return his; }
//! [0]


//! [1]
        void MyClass.detach()
        { qAtomicDetach(d); }
//! [1]


    }
}
