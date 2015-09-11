import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;


public class doc_src_coordsys extends QWidget {

    public static void main(String args[]) {
        QApplication.initialize(args);
    }

    public void m0() {
//! [0]
    QPainter painter = new QPainter(this);

    painter.setPen(new QColor(Qt.GlobalColor.darkGreen));
    painter.drawRect(1, 2, 6, 4);
//! [0]
    }

    public void m1() {
//! [1]
    QPainter painter = new QPainter(this);

    painter.setPen(new QColor(Qt.GlobalColor.darkGreen));
    painter.drawLine(2, 7, 6, 1);
//! [1]
    }

    public void m2() {
//! [2]
    QPainter painter = new QPainter(this);
    painter.setRenderHint(
        QPainter.RenderHint.Antialiasing);
    painter.setPen(new QColor(Qt.GlobalColor.darkGreen));
    painter.drawRect(1, 2, 6, 4);
//! [2]
    }

    public void m3() {
//! [3]
    QPainter painter = new QPainter(this);
    painter.setRenderHint(
        QPainter.RenderHint.Antialiasing);
    painter.setPen(new QColor(Qt.GlobalColor.darkGreen));
    painter.drawLine(2, 7, 6, 1);
//! [3]
    }

    public void m4() {
//! [4]
        QPainter painter = new QPainter(this);
        painter.setWindow(new QRect(-50, -50, 100, 100));
//! [4]
    }

    public void m5() {
//! [5]
        QPainter painter = new QPainter(this);

        int side = Math.min(width(), height());
        int x = (width() - side / 2);
        int y = (height() - side / 2);
        painter.setViewport(x, y, side, side);
//! [5]
    }
}
