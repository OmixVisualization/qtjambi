import org.qtjambi.qt.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.core.*;

class CustomScene extends QGraphicsScene
{

    public CustomScene()
    {
        addItem(new QGraphicsEllipseItem(new QRectF(10, 10, 30, 30)));
    }

//! [0]
    public void drawItems(QPainter painter, int numItems,
                          QGraphicsItem items[],
                          QStyleOptionGraphicsItem options[],
                          QWidget widget)
    {
        for (int i = 0; i < numItems; ++i) {
            // Draw the item
            painter.save();
            items[i].paint(painter, options[i], widget);
            painter.restore();
        }
    }
//! [0]

    public static void main(String args[])
    {
        QApplication.initialize(args);

        CustomScene scene = new CustomScene();
        QGraphicsView view = new QGraphicsView(scene);
        view.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
