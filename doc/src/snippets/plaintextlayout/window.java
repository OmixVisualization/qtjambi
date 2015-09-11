import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

public class window extends QWidget
{
    QFont font = new QFont();
    String text = "";

    public window()
    {
        text =  "Support for text rendering and layout in Qt 4 has been " +
                "redesigned around a system that allows textual content to " +
                "be represented in a more flexible way than was possible " +
                "with Qt 3. Qt 4 also provides a more convenient " +
                "programming interface for editing documents. These " +
                "improvements are made available through a reimplementation " +
                "of the existing text rendering engine, and the " +
                "introduction of several new classes. " +
                "See the relevant module overview for a detailed discussion " +
                "of this framework. The following sections provide a brief " +
                "overview of the main concepts behind Scribe.";

        setWindowTitle(tr("Plain Text Layout"));
    }

    protected void paintEvent(QPaintEvent event)
    {
    //! [0]
        QTextLayout textLayout = new QTextLayout(text, font);
        double margin = 10;
        double radius = Math.min(width()/2.0, height()/2.0) - margin;
        QFontMetrics fm =
            new QFontMetrics(font);

        double lineHeight = fm.height();
        double y = 0;

        textLayout.beginLayout();

        while (true) {
            // create a new line
            QTextLine line = textLayout.createLine();
            if (!line.isValid())
                break;

            double x1 = Math.max(0.0, Math.pow(Math.pow(radius,2)-Math.pow(radius-y,2), 0.5));
            double x2 = Math.max(0.0, Math.pow(Math.pow(radius,2)-Math.pow(radius-(y+lineHeight),2), 0.5));
            double x = Math.max(x1, x2) + margin;
            double lineWidth = (width() - margin) - x;

            line.setLineWidth(lineWidth);
            line.setPosition(new QPointF(x, margin+y));
            y += line.height();
        }

        textLayout.endLayout();

        QPainter painter = new QPainter();
        painter.begin(this);
        painter.setRenderHint(QPainter.RenderHint.Antialiasing);
        painter.fillRect(rect(),
            new QBrush(new QColor(Qt.GlobalColor.white)));
        painter.setBrush(new QBrush(new QColor(Qt.GlobalColor.black)));
        painter.setPen(new QPen(new QColor(Qt.GlobalColor.black)));
        textLayout.draw(painter, new QPointF(0,0), null);

        painter.setBrush(new QBrush(new QColor("#a6ce39")));
        painter.setPen(new QPen(new QColor(Qt.GlobalColor.black)));
        painter.drawEllipse(new QRectF(-radius, margin, 2*radius, 2*radius));
        painter.end();
    //! [0]
    }

    public static void main(String args[])
    {
        QApplication.initialize(args);

        new window().show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
