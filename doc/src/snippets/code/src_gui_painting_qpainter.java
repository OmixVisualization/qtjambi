import java.util.Arrays;

import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;


public class src_gui_painting_qpainter extends QWidget {
//! [0]
    @Override
    protected void paintEvent(QPaintEvent e) {
        QPainter painter = new QPainter(this);
        painter.setPen(QColor.blue);
        painter.setFont(new QFont("Arial", 30));
        painter.drawText(rect(), Qt.AlignmentFlag.AlignCenter.value(), "Qt");
        painter.end();
    }
//! [0]
}

class painterfoobar extends QWidget {
    QLine line;
//! [1]
    @Override
    protected void paintEvent(QPaintEvent e) {
        QPainter p = new QPainter();
        p.begin(this);
        p.drawLine(line);        // drawing code
        p.end();
    }
//! [1]
}

class painterfoobar2 extends painterfoobar {
//! [2]
    @Override
    protected void paintEvent(QPaintEvent e) {
        QPainter p = new QPainter(this);
        p.drawLine(line);        // drawing code
        p.end();
    }
//! [2]


    public void abc(QPainter painter) {
        QWidget myWidget = null;
        QPainter painter2 = null;

//! [3]
        painter.begin(null); // impossible - paint device cannot be null

        QPixmap image = new QPixmap(0, 0);
        painter.begin(image); // impossible - image.isNull() == true;

        painter.begin(myWidget);
        painter2.begin(myWidget); // impossible - only one painter at a time
//! [3]
    }


    void setWorldMatrix(QMatrix matrix, boolean b) {}

//! [4]
    void rotate(double angle) {
        QMatrix matrix = new QMatrix();
        matrix.rotate(angle);
        setWorldMatrix(matrix, true);
    }
//! [4]


    void abcdef() {
        {
//! [5]
    QPainterPath path = new QPainterPath();
    path.moveTo(20, 80);
    path.lineTo(20, 30);
    path.cubicTo(80, 0, 50, 50, 80, 80);

    QPainter painter = new QPainter(this);
    painter.drawPath(path);
    painter.end();
//! [5]
        }

    {
//! [6]
    QLineF line = new QLineF(10.0, 80.0, 90.0, 20.0);

    QPainter painter = new QPainter(this);
    painter.drawLine(line);
    painter.end();
//! [6]
    }

    {
//! [7]
        QRectF rectangle = new QRectF(10.0, 20.0, 80.0, 60.0);

        QPainter painter = new QPainter(this);
        painter.drawRect(rectangle);
        painter.end();
//! [7]
    }

    {
//! [8]
        QRectF rectangle = new QRectF(10.0, 20.0, 80.0, 60.0);

        QPainter painter = new QPainter(this);
        painter.drawRoundedRect(rectangle, 20.0, 15.0);
        painter.end();
//! [8]
    }

    {
//! [9]
        QRectF rectangle = new QRectF(10.0, 20.0, 80.0, 60.0);

        QPainter painter = new QPainter(this);
        painter.drawEllipse(rectangle);
        painter.end();
//! [9]
    }

    {
//! [10]
    QRectF rectangle = new QRectF(10.0, 20.0, 80.0, 60.0);
    int startAngle = 30 * 16;
    int spanAngle = 120 * 16;

    QPainter painter = new QPainter(this);
    painter.drawArc(rectangle, startAngle, spanAngle);
    painter.end();
//! [10]
    }

    {
//! [11]
        QRectF rectangle = new QRectF(10.0, 20.0, 80.0, 60.0);
        int startAngle = 30 * 16;
        int spanAngle = 120 * 16;

        QPainter painter = new QPainter(this);
        painter.drawPie(rectangle, startAngle, spanAngle);
        painter.end();
//! [11]
    }

    {
//! [12]
        QRectF rectangle = new QRectF(10.0, 20.0, 80.0, 60.0);
        int startAngle = 30 * 16;
        int spanAngle = 120 * 16;

        QPainter painter = new QPainter(this);
        painter.drawChord(rectangle, startAngle, spanAngle);
        painter.end();
//! [12]
    }

    {
//! [13]
        final QPointF points[] = {
            new QPointF(10.0, 80.0),
            new QPointF(20.0, 10.0),
            new QPointF(80.0, 30.0),
        };

        QPainter painter = new QPainter(this);
        painter.drawPolyline(new QPolygonF(Arrays.asList(points)));
        painter.end();
//! [13]
    }

    {
//! [14]
    final QPointF points[] = {
        new QPointF(10.0, 80.0),
        new QPointF(20.0, 10.0),
        new QPointF(80.0, 30.0),
        new QPointF(90.0, 70.0)
    };

    QPainter painter = new QPainter(this);
    painter.drawPolygon(new QPolygonF(Arrays.asList(points)));
    painter.end();
//! [14]
    }

    {
//! [15]
    final QPointF points[] = {
        new QPointF(10.0, 80.0),
        new QPointF(20.0, 10.0),
        new QPointF(80.0, 30.0),
        new QPointF(90.0, 70.0)
    };

    QPainter painter = new QPainter(this);
    painter.drawConvexPolygon(new QPolygonF(Arrays.asList(points)));
    painter.end();
//! [15]
    }

    {
//! [16]
    QRectF target = new QRectF(10.0, 20.0, 80.0, 60.0);
    QRectF source = new QRectF(0.0, 0.0, 70.0, 40.0);
    QPixmap pixmap = new QPixmap("classpath:myPixmap.png");

    QPainter painter = new QPainter(this);
    painter.drawPixmap(target, pixmap, source);
    painter.end();
//! [16]
    }

    {
        QRectF rect = null;
//! [17]
        QPainter painter = new QPainter(this);
        painter.drawText(rect, Qt.AlignmentFlag.AlignCenter.value(), tr("Qt by\nTrolltech"));
        painter.end();
//! [17]
    }

    {
//! [18]
        QPicture picture = new QPicture();
        picture.load("drawing.pic");

        QPainter painter = new QPainter(this);
        painter.drawPicture(0, 0, picture);
        painter.end();
//! [18]
    }
    }


    public void fillRect(int a, int b) {}
    public int background()
    {
        int rectangle = 0;
//! [19]
        fillRect(rectangle, background());
//! [19]


    {
//! [20]
    QRectF target = new QRectF(10.0, 20.0, 80.0, 60.0);
    QRectF source = new QRectF(0.0, 0.0, 70.0, 40.0);
    QImage image = new QImage("classpath:/images/myImage.png");

    QPainter painter = new QPainter(this);
    painter.drawImage(target, image, source);
    painter.end();
//! [20]
    }
    return 0;
    }
}
