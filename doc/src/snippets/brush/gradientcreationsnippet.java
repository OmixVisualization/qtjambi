import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

public class gradientcreationsnippet
{
    public static void main(String args[])
    {
        QApplication.initialize(args);

        //! [0]
        QRadialGradient gradient =
            new QRadialGradient(50, 50, 50, 50, 50);
        gradient.setColorAt(0, QColor.fromRgbF(0, 1, 0, 1));
        gradient.setColorAt(1, QColor.fromRgbF(0, 0, 0, 0));

        QBrush brush = new QBrush(gradient);
        //! [0]

        QWidget widget = new QWidget();
        QPalette palette = new QPalette();
        palette.setBrush(widget.backgroundRole(), brush);
        widget.setPalette(palette);
        widget.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
