import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_corelib_tools_qsize {
    public static void main(String args[]) {
        QApplication.initialize(args);

        if (true)
        {
//! [0]
        QSize t1 = new QSize(10, 12);
        t1.scale(60, 60, Qt.AspectRatioMode.IgnoreAspectRatio);
        // t1 is (60, 60)

        QSize t2 = new QSize(10, 12);
        t2.scale(60, 60, Qt.AspectRatioMode.KeepAspectRatio);
        // t2 is (50, 60)

        QSize t3 = new QSize(10, 12);
        t3.scale(60, 60, Qt.AspectRatioMode.KeepAspectRatioByExpanding);
        // t3 is (60, 72)
//! [0]
        }

        if (true)
        {
//! [1]
        QSize size = new QSize(100, 10);
        size.setWidth(size.width() + 20);

        // size becomes (120,10)
//! [1]
        }

        if (true)
        {
//! [2]
        QSize size = new QSize(100, 10);
        size.setHeight(size.height() + 5);

        // size becomes (100,15)
//! [2]
        }

        if (true)
        {
//! [3]
        QSize s = new QSize( 3, 7);
        QSize r = new QSize(-1, 4);
        s.add(r);

        // s becomes (2,11)

//! [3]
        }

        if (true)
        {
//! [4]
        QSize s = new QSize( 3, 7);
        QSize r = new QSize(-1, 4);
        s.subtract(r);

        // s becomes (4,3)
//! [4]
        }


        if (true)
        {
//! [5]
        QSizeF t1 = new QSizeF(10, 12);
        t1.scale(60, 60, Qt.AspectRatioMode.IgnoreAspectRatio);
        // t1 is (60, 60)

        QSizeF t2 = new QSizeF(10, 12);
        t2.scale(60, 60, Qt.AspectRatioMode.KeepAspectRatio);
        // t2 is (50, 60)

        QSizeF t3 = new QSizeF(10, 12);
        t3.scale(60, 60, Qt.AspectRatioMode.KeepAspectRatioByExpanding);
        // t3 is (60, 72)
//! [5]
        }

        if (true)
        {
//! [6]
        QSizeF size = new QSizeF(100.3, 10);
        size.setWidth(size.width() + 20.5);

         // size becomes (120.8,10)
//! [6]
        }

        if (true)
        {
//! [7]
        QSizeF size = new QSizeF(100, 10.2);
        size.setHeight(size.height() + 5.5);

        // size becomes (100,15.7)
//! [7]
        }

        if (true)
        {
//! [8]
        QSizeF s = new QSizeF( 3, 7);
        QSizeF r = new QSizeF(-1, 4);
        s.add(r);

        // s becomes (2,11)
//! [8]
        }

        if (true)
        {
//! [9]
        QSizeF s = new QSizeF( 3, 7);
        QSizeF r = new QSizeF(-1, 4);
        s.subtract(r);
        }
        // s becomes (4,3)
//! [9]

    }
}
