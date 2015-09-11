import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;


public class src_gui_painting_qregion extends QWidget {

    public static void main(String args[]) {
        QApplication.initialize(args);

        QWidget w = new src_gui_painting_qregion();
        w.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }

//! [0]
    protected void paintEvent(QPaintEvent e) {
        // r1: elliptic region
        QRegion r1 = new QRegion(new QRect(100, 100, 200, 80),
                                 QRegion.RegionType.Ellipse);

        // r2: rectangular region
        QRegion r2 = new QRegion(new QRect(100, 120, 90, 30));

        // r3: intersection
        QRegion r3 = r1.intersected(r2);

        QPainter painter = new QPainter(this);
        painter.setClipRegion(r3);

        // paint clipped graphics
    }
//! [0]
}
