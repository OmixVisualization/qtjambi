/*   Ported from: doc.src.qt4-arthur.qdoc
*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


class doc_src_qt4 extends QWidget
{
    public static void main(String args[]) {
        QApplication.initialize(args);
    }

    public void paintEvent(QPaintEvent paintEvent) {
    {
    QPainter painter = new QPainter();
    {
    //! [0]
    QLinearGradient gradient = new QLinearGradient(0, 0, 100, 100);
    gradient.setColorAt(0, QColor.red);
    gradient.setColorAt(0.5, QColor.green);
    gradient.setColorAt(1, QColor.blue);
    painter.setBrush(gradient);
    painter.drawRect(0, 0, 100, 100);
//! [0]
    }
    {
//! [1]
    QRadialGradient gradient = new QRadialGradient(50, 50, 50, 30, 30);
    gradient.setColorAt(0.2, QColor.white);
    gradient.setColorAt(0.8, QColor.green);
    gradient.setColorAt(1, QColor.black);
    painter.setBrush(gradient);
    painter.drawEllipse(0, 0, 100, 100);
//! [1]
    }
    {
//! [2]
    QConicalGradient gradient = new QConicalGradient(60, 40, 0);
    gradient.setColorAt(0, QColor.black);
    gradient.setColorAt(0.4, QColor.green);
    gradient.setColorAt(0.6, QColor.white);
    gradient.setColorAt(1, QColor.black);
    painter.setBrush(gradient);
    painter.drawEllipse(0, 0, 100, 100);
//! [2]
    }
//! [3]
    // Specfiy semi-transparent red
    painter.setBrush(new QColor(255, 0, 0, 127));
    painter.drawRect(0, 0, width()/2, height());

    // Specify semi-transparend blue
    painter.setBrush(new QColor(0, 0, 255, 127));
    painter.drawRect(0, 0, width(), height()/2);
//! [3]
//! [4]

    // One line without anti-aliasing
    painter.drawLine(0, 0, width()/2, height());

    // One line with anti-aliasing
    painter.setRenderHint(QPainter.RenderHint.Antialiasing);
    painter.drawLine(width()/2, 0, width()/2, height());
//! [4]

    /*
//! [5]
    QPainterPath path = new QPainterPath();
    path.addRect(20, 20, 60, 60);
    path.addBezier(0, 0,  99, 0,  50, 50,  99, 99);
    path.addBezier(99, 99,  0, 99,  50, 50,  0, 0);
    painter.drawPath(path);
//! [5]
 *
 */
    }

/*
//! [6]
    QPixmap buffer(size());
    QPainter painter(&buffer);

    // Paint code here

    painter.end();
    bitBlt(this, 0, 0, buffer);
//! [6]

//! [7]
    QPainter painter = new QPainter(this);

    // Paint code here

    painter.end();
//! [7]
*/


    QWidget unbufferedWidget = new QWidget();
//! [8]
    unbufferedWidget.setAttribute(Qt.WidgetAttribute.WA_PaintOnScreen);
//! [8]

    {
    int fontSize = 10;
    QPainter painter = new QPainter(this);
    String text = "text";
//! [9]
    QLinearGradient gradient = new QLinearGradient(0, 0, 100, 100);
    gradient.setColorAt(0, QColor.blue);
    gradient.setColorAt(1, QColor.red);
    painter.setPen(new QPen(new QBrush(gradient), 0));
    for (int y=fontSize; y<100; y += fontSize)
        painter.drawText(0, y, text);
//! [9]
    }
    {
//! [10]
    QImage image = new QImage(100, 100, QImage.Format.Format_RGB32);
    QPainter painter = new QPainter(image);

    // painter commands.

    painter.end();
//! [10]
    }

    }
}
