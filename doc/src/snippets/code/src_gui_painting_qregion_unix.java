import org.qtjambi.qt.gui.*;

public class src_gui_painting_qregion_unix {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        QRegion r1 = new QRegion(10, 10, 20, 20);
        r1.isEmpty();               // false

        QRegion r2 = new QRegion(40, 40, 20, 20);
        QRegion r3 = new QRegion();
        r3.isEmpty();               // true

        r3 = r1.intersected(r2);    // r3: intersection of r1 and r2
        r3.isEmpty();               // true

        r3 = r1.united(r2);         // r3: union of r1 and r2
        r3.isEmpty();               // false
//! [0]


    }
}
