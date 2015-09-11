import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

public class matrix
{

    private static final class SimpleTransformation extends QWidget
    {

    //! [0]
    protected void paintEvent(QPaintEvent event)
    {
        QPainter painter = new QPainter(this);
        painter.setPen(new QPen(QColor.blue, 1, Qt.PenStyle.DashLine));
        painter.drawRect(0, 0, 100, 100);

        painter.rotate(45);

        painter.setFont(new QFont("Helvetica", 24));
        painter.setPen(new QPen(QColor.black, 1));
        painter.drawText(20, 10, "QMatrix");
    }
    //! [0]
    }

    private static final class CombinedTransformation extends QWidget
    {
    //! [1]
    protected void paintEvent(QPaintEvent event)
    {
        QPainter painter = new QPainter(this);
        painter.setPen(new QPen(QColor.blue, 1,
                                Qt.PenStyle.DashLine));
        painter.drawRect(0, 0, 100, 100);

        QMatrix matrix = new QMatrix();
        matrix.translate(50, 50);
        matrix.rotate(45);
        matrix.scale(0.5, 1.0);
        painter.setWorldMatrix(matrix);

        painter.setFont(new QFont("Helvetica", 24));
        painter.setPen(new QPen(QColor.black, 1));
        painter.drawText(20, 10, "QMatrix");
    }
    //! [1]
    }

    private static final class BasicOperations extends QWidget
    {
    //! [2]
    protected void paintEvent(QPaintEvent event)
    {
        double pi = 3.14;

        double a    = pi/180 * 45.0;
        double sina = Math.sin(a);
        double cosa = Math.cos(a);

        QMatrix translationMatrix = new QMatrix(1, 0, 0, 1, 50.0, 50.0);
        QMatrix rotationMatrix = new QMatrix(cosa, sina, -sina, cosa, 0, 0);
        QMatrix scalingMatrix = new QMatrix(0.5, 0, 0, 1.0, 0, 0);

        QMatrix matrix = new QMatrix();
        matrix =  scalingMatrix.multiply(rotationMatrix.multiply(translationMatrix));

        QPainter painter = new QPainter(this);
        painter.setPen(new QPen(QColor.blue, 1, Qt.PenStyle.DashLine));
        painter.drawRect(0, 0, 100, 100);

        painter.setWorldMatrix(matrix);

        painter.setFont(new QFont("Helvetica", 24));
        painter.setPen(new QPen(QColor.black, 1));
        painter.drawText(20, 10, "QMatrix");
    }
    //! [2]
    }

    public static void main(String args[])
    {
        QApplication.initialize(args);

        QWidget widget = new QWidget();

        SimpleTransformation simpleWidget = new SimpleTransformation();
        CombinedTransformation combinedWidget = new CombinedTransformation();
        BasicOperations basicWidget = new BasicOperations();

        QVBoxLayout layout = new QVBoxLayout();
        layout.addWidget(simpleWidget);
        layout.addWidget(combinedWidget);
        layout.addWidget(basicWidget);
        widget.setLayout(layout);

        widget.show();
        widget.resize(130, 350);

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
