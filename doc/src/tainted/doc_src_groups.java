/*   Ported from: doc.src.groups.qdoc
<snip>
//! [0]
        void QPen::setStyle(Qt::PenStyle style)
        {
            detach();           // detach from common data
            d->style = style;   // set the style member
        }

        void QPen::detach()
        {
            if (d->ref != 1) {
                ...             // perform a deep copy
            }
        }
//! [0]


//! [1]
        QPixmap p1, p2;
        p1.load("image.bmp");
        p2 = p1;                        // p1 and p2 share data

        QPainter paint;
        paint.begin(&p2);               // cuts p2 loose from p1
        paint.drawText(0,50, "Hi");
        paint.end();
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


public class doc_src_groups {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        void QPen.setStyle(Qt.PenStyle style)
        {
            detach();           // detach from common data
            d.style = style;   // set the style member
        }

        void QPen.detach()
        {
            if (d.ref != 1) {
                ...             // perform a deep copy
            }
        }
//! [0]


//! [1]
        QPixmap p1, p2;
        p1.load("image.bmp");
        p2 = p1;                        // p1 and p2 share data

        QPainter paint;
        paint.begin(2);               // cuts p2 loose from p1
        paint.drawText(0,50, "Hi");
        paint.end();
//! [1]


    }
}
