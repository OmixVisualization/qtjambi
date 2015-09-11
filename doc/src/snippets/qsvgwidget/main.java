
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.svg.*;

class main
{

public static void main(String args[])
{
    QApplication.initialize(args);
//! [0]
    QSvgWidget window = new QSvgWidget(":/files/spheres.svg");
    window.show();
//! [0]
    QSvgRenderer renderer = window.renderer();
    QImage image = new QImage(150, 150, QImage.Format.Format_RGB32);
    QPainter painter = new QPainter();
    painter.begin(image);
    renderer.render(painter);
    painter.end();
    image.save("spheres.png", "PNG", 9);

    QApplication.execStatic();
    QApplication.shutdown();
}

}
