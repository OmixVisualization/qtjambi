import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

public class brush extends QWidget
{

    public static void main(String args[])
    {
        QWidget anyPaintDevice = new QWidget();

        if (true)
        {
            // PEN SNIPPET
            QPainter painter = new QPainter();
            QPen pen = new QPen(new QColor(Qt.GlobalColor.red), 2); // red solid line, 2 pixels wide
            painter.begin(anyPaintDevice);        // paint something
            painter.setPen(pen);                  // set the red, wide pen
            painter.drawRect(40,30, 200,100);     // draw a rectangle
            painter.setPen(new QPen(new QColor(Qt.GlobalColor.blue)));  // set blue pen, 0 pixel width
            painter.drawLine(40,30, 240,130);     // draw a diagonal in rectangle
            painter.end();                        // painting done
        }

        if (true)
        {
            // BRUSH SNIPPET
            QPainter painter = new QPainter();
            QBrush brush = new QBrush(new QColor(Qt.GlobalColor.yellow));  // yellow solid pattern
            painter.begin(anyPaintDevice);   // paint something
            painter.setBrush(brush);         // set the yellow brush
            painter.setPen(new QPen(Qt.PenStyle.NoPen));  // do not draw outline
            painter.drawRect(40,30, 200,100); // draw filled rectangle
            painter.setBrush(new QBrush(Qt.BrushStyle.NoBrush));        // do not fill
            painter.setPen(new QPen(new QColor(Qt.GlobalColor.black))); // set black pen, 0 pixel width
            painter.drawRect(10,10, 30,20);                         // draw rectangle outline
            painter.end();                                          // painting done
        }

        // LINEAR
    //! [0]
        QLinearGradient linearGrad = new QLinearGradient(new QPointF(100, 100),
                                                         new QPointF(200, 200));
        linearGrad.setColorAt(0, new QColor(Qt.GlobalColor.black));
        linearGrad.setColorAt(1, new QColor(Qt.GlobalColor.white));
    //! [0]

        // RADIAL
    //! [1]
        QRadialGradient radialGrad =
            new QRadialGradient(new QPointF(100, 100), 100);
        radialGrad.setColorAt(0, new QColor(Qt.GlobalColor.red));
        radialGrad.setColorAt(0.5, new QColor(Qt.GlobalColor.blue));
        radialGrad.setColorAt(1, new QColor(Qt.GlobalColor.green));
    //! [1]
    }
}

