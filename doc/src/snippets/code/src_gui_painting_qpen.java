import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;


public class src_gui_painting_qpen extends QWidget {
    public static void main(String args[]) {
        QApplication.initialize(args);

        QWidget w = new src_gui_painting_qpen();
        w.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }

    protected void paintEvent(QPaintEvent e) {

        {
//! [0]
        QPainter painter = new QPainter(this);
        QPen pen = new QPen(QColor.green, 3, Qt.PenStyle.DashDotLine, Qt.PenCapStyle.RoundCap, Qt.PenJoinStyle.RoundJoin);
        painter.setPen(pen);
//! [0]
        }

        {
//! [1]
        QPainter painter = new QPainter(this);
        QPen pen = new QPen();  // creates a default pen

        pen.setStyle(Qt.PenStyle.DashDotLine);
        pen.setWidth(3);
        pen.setBrush(new QBrush(QColor.green));
        pen.setCapStyle(Qt.PenCapStyle.RoundCap);
        pen.setJoinStyle(Qt.PenJoinStyle.RoundJoin);

        painter.setPen(pen);
//! [1]
        }

        {
//! [2]
        QPen pen = new QPen();
        List<Double> dashes = new ArrayList<Double>();

        double space = 4;

        dashes.add(1.0);
        dashes.add(space);
        dashes.add(3.0);
        dashes.add(space);
        dashes.add(9.0);
        dashes.add(space);
        dashes.add(27.0);
        dashes.add(space);
        dashes.add(9.0);

        pen.setDashPattern(dashes);
//! [2]
        }


//! [3]
        QPen pen = new QPen();
        List<Double> dashes = new ArrayList<Double>();

        double space = 4;

        dashes.add(1.0);
        dashes.add(space);
        dashes.add(3.0);
        dashes.add(space);
        dashes.add(9.0);
        dashes.add(space);
        dashes.add(27.0);
        dashes.add(space);
        dashes.add(9.0);

        pen.setDashPattern(dashes);
//! [3]


    }
}
