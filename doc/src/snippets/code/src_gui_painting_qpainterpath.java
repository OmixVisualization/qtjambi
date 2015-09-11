import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_painting_qpainterpath extends QWidget {
    public static void main(String args[]) {
        QApplication.initialize(args);

        QWidget w = new src_gui_painting_qpainterpath();
        w.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }


    protected void paintEvent(QPaintEvent e) {

        {
//! [0]
        QPainterPath path = new QPainterPath();
        path.addRect(20, 20, 60, 60);

        path.moveTo(0, 0);
        path.cubicTo(99, 0,  50, 50,  99, 99);
        path.cubicTo(0, 99,  50, 50,  0, 0);

        QPainter painter = new QPainter(this);
        painter.fillRect(0, 0, 100, 100, new QBrush(QColor.white));
        painter.setPen(new QPen(new QColor(79, 106, 25), 1,
                                Qt.PenStyle.SolidLine,
                                Qt.PenCapStyle.FlatCap,
                                Qt.PenJoinStyle.MiterJoin));
        painter.setBrush(new QColor(122, 163, 39));

        painter.drawPath(path);
//! [0]
        }


        {
        QPointF c1 = new QPointF();
        QPointF c2 = new QPointF();
        QPointF endPoint = new QPointF();
//! [1]
        QLinearGradient myGradient = new QLinearGradient();
        QPen myPen = new QPen();

        QPainterPath myPath = new QPainterPath();
        myPath.cubicTo(c1, c2, endPoint);

        QPainter painter = new QPainter(this);
        painter.setBrush(myGradient);
        painter.setPen(myPen);
        painter.drawPath(myPath);
//! [1]
        }

        {
        QRectF boundingRect = new QRectF();
        double startAngle = 0;
        double sweepLength = 0;
//! [2]
        QLinearGradient myGradient = new QLinearGradient();
        QPen myPen = new QPen();

        QPointF center = new QPointF();

        QPainterPath myPath = new QPainterPath();
        myPath.moveTo(center);
        myPath.arcTo(boundingRect, startAngle,
                     sweepLength);

        QPainter painter = new QPainter(this);
        painter.setBrush(myGradient);
        painter.setPen(myPen);
        painter.drawPath(myPath);
//! [2]
        }


        {
//! [3]
        QLinearGradient myGradient = new QLinearGradient();
        QPen myPen = new QPen();
        QRectF myRectangle = new QRectF();

        QPainterPath myPath = new QPainterPath();
        myPath.addRect(myRectangle);

        QPainter painter = new QPainter(this);
        painter.setBrush(myGradient);
        painter.setPen(myPen);
        painter.drawPath(myPath);
//! [3]
        }

        {
//! [4]
        QLinearGradient myGradient = new QLinearGradient();
        QPen myPen = new QPen();
        QPolygonF myPolygon = new QPolygonF();

        QPainterPath myPath = new QPainterPath();
        myPath.addPolygon(myPolygon);

        QPainter painter = new QPainter(this);
        painter.setBrush(myGradient);
        painter.setPen(myPen);
        painter.drawPath(myPath);
//! [4]
        }

        {
//! [5]
        QLinearGradient myGradient = new QLinearGradient();
        QPen myPen = new QPen();
        QRectF boundingRectangle = new QRectF();

        QPainterPath myPath = new QPainterPath();
        myPath.addEllipse(boundingRectangle);

        QPainter painter = new QPainter(this);
        painter.setBrush(myGradient);
        painter.setPen(myPen);
        painter.drawPath(myPath);
//! [5]
        }

        {
        double x = 0, y = 0;
//! [6]
        QLinearGradient myGradient = new QLinearGradient();
        QPen myPen = new QPen();
        QFont myFont = font();
        QPointF baseline = new QPointF(x, y);

        QPainterPath myPath = new QPainterPath();
        myPath.addText(baseline, myFont, tr("Qt"));

        QPainter painter = new QPainter(this);
        painter.setBrush(myGradient);
        painter.setPen(myPen);
        painter.drawPath(myPath);
//! [6]
        }


    }
}
