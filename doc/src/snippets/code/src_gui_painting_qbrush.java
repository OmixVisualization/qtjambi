import org.qtjambi.qt.gui.*;

public class src_gui_painting_qbrush extends QWidget {

    protected void paintEvent(QPaintEvent e) {
//! [0]
        QPainter painter = new QPainter(this);

        painter.setBrush(QColor.cyan);
        painter.setPen(QColor.darkCyan);
        painter.drawRect(0, 0, 100,100);

        painter.setBrush(QBrush.NoBrush);
        painter.setPen(QColor.darkGreen);
        painter.drawRect(40, 40, 100, 100);
//! [0]

    }

    public static void main(String args[]) {
        QApplication.initialize(args);

        QWidget w = new src_gui_painting_qbrush();
        w.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
