/*   Ported from: doc.src.qnamespace.qdoc
<snip>
//! [0]
        QObject::connect: Cannot queue arguments of type 'MyType'
//! [0]


//! [1]
    enum CustomEventPriority
    {
        // An important event
        ImportantEventPriority = Qt::HighEventPriority,

        // A more important event
        MoreImportantEventPriority = ImportantEventPriority + 1,

        // A critical event
        CriticalEventPriority = 100 * MoreImportantEventPriority,

        // Not that important
        StatusEventPriority = Qt::LowEventPriority,

        // These are less important than Status events
        IdleProcessingDoneEventPriority = StatusEventPriority - 1
    };
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


public class doc_src_qnamespace {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        QObject.connect: Cannot queue arguments of type 'MyType'
//! [0]


//! [1]
    enum CustomEventPriority
    {
        // An important event
        ImportantEventPriority = Qt.HighEventPriority,

        // A more important event
        MoreImportantEventPriority = ImportantEventPriority + 1,

        // A critical event
        CriticalEventPriority = 100 * MoreImportantEventPriority,

        // Not that important
        StatusEventPriority = Qt.LowEventPriority,

        // These are less important than Status events
        IdleProcessingDoneEventPriority = StatusEventPriority - 1
    };
//! [1]


    }
}
