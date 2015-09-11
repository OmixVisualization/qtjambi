import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import java.lang.Math;


public class src_corelib_tools_qpoint {
    public static void main(String args[]) {
        QApplication.initialize(args);
        {
//! [0]
        QPoint p = new QPoint();

        p.setX(p.x() + 1);
        p.add(new QPoint(1, 0));
//! [0]
        }


        // The following two code snippets are for methods that don't exist
        // in Qt Jambi.
//! [1]
//        QPoint p = new QPoint(1, 2);
//        p.rx()--;   // p becomes (0, 2)
//! [1]


//! [2]
//        QPoint p = new QPoint(1, 2);
//        p.ry()++;   // p becomes (1, 3)
//! [2]

        {
//! [3]
        QPoint p = new QPoint( 3, 7);
        QPoint q = new QPoint(-1, 4);
        p.add(q);    // p becomes (2, 11)
//! [3]
        }

        {
//! [4]
        QPoint p = new QPoint( 3, 7);
        QPoint q = new QPoint(-1, 4);
        p.subtract(q);    // p becomes (4, 3)
//! [4]
        }

        {
//! [5]
        QPoint p = new QPoint(-1, 4);
        p.multiply(2.5);    // p becomes (-3, 10)
//! [5]
        }

        {
//! [6]
        QPoint p = new QPoint(-3, 10);
        p.divide(2.5);           // p becomes (-1, 4)
//! [6]
        }

        new Object() {
//! [7]
        protected QPoint oldPosition = new QPoint();

        protected void mouseMoveEvent(QMouseEvent event)
        {
            QPoint point = new QPoint(event.pos().x(), event.pos().y());
            point.subtract(oldPosition);
            if (point.manhattanLength() > 3) {
                // the mouse has moved more than 3 pixels since the oldPosition
            }
        }
//! [7]
        };

        {
        QPoint p = new QPoint();
//! [8]
        double trueLength = Math.hypot(p.x(), p.y());
//! [8]
        }

        {
//! [9]
        QPointF p = new QPointF();

        p.setX(p.x() + 1.0);
        p.add(new QPointF(1.0, 0.0));
//! [9]
        }

        // The following two code snippets are for methods that don't exist
        // in Qt Jambi.
//! [10]
//         QPointF p = new QPointF(1.1, 2.5);
//         p.rx()--;   // p becomes (0.1, 2.5)
//! [10]


//! [11]
//        QPointF p = new QPointF(1.1, 2.5);
//        p.ry()++;   // p becomes (1.1, 3.5)
//! [11]

        {
//! [12]
        QPointF p = new QPointF( 3.1, 7.1);
        QPointF q = new QPointF(-1.0, 4.1);
        p.add(q);    // p becomes (2.1, 11.2)
//! [12]
        }

        {
//! [13]
        QPointF p = new QPointF( 3.1, 7.1);
        QPointF q = new QPointF(-1.0, 4.1);
        p.subtract(q);    // p becomes (4.1, 3.0)
//! [13]
        }

        {
//! [14]
         QPointF p = new QPointF(-1.1, 4.1);
         p.multiply(2.5);    // p becomes (-2.75, 10.25)
//! [14]
        }

        {
//! [15]
        QPointF p = new QPointF(-2.75, 10.25);
        p.divide(2.5);           // p becomes (-1.1,4.1)
//! [15]
        }
    }
}
