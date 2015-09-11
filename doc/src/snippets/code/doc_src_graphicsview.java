import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.opengl.*;

public class doc_src_graphicsview {

//! [2]
            public class View extends QGraphicsView
            {

            //...

            public void zoomIn() { scale(1.2, 1.2); }
            public void zoomOut() { scale(1 / 1.2, 1 / 1.2); }
            public void rotateLeft() { rotate(-10); }
            public void rotateRight() { rotate(10); }

            //...
            }
//! [2]


    static void myPopulateScene(QGraphicsScene s)
    {
    }

    public static void main(String args[]) {
        QApplication.initialize(args);

        if (true)
        {
//! [0]
            QGraphicsScene scene = new QGraphicsScene();
            QGraphicsRectItem rect = scene.addRect(new QRectF(0, 0, 100, 100));

            QGraphicsItemInterface item = scene.itemAt(50, 50);
            // item == rect
//! [0]
            if (item != null && rect != null) {}

        }



        if (true)
        {
//! [1]
            QGraphicsScene scene = new QGraphicsScene();
            myPopulateScene(scene);

            QGraphicsView view = new QGraphicsView(scene);
            view.show();
//! [1]
        }


        if (true)
        {
//! [3]
            QGraphicsScene scene = new QGraphicsScene();
            scene.addRect(new QRectF(0, 0, 100, 200), new QPen(QColor.black), new QBrush(QColor.green));

            QPrinter printer = new QPrinter();
            if (new QPrintDialog(printer).exec() == QDialog.DialogCode.Accepted.value()) {
                QPainter painter = new QPainter(printer);
                painter.setRenderHint(QPainter.RenderHint.Antialiasing);
                scene.render(painter);
            }
//! [3]
        }



        if (true)
        {
//! [4]
            QGraphicsScene scene = new QGraphicsScene();
            scene.addRect(new QRectF(0, 0, 100, 200), new QPen(QColor.black), new QBrush(QColor.green));

            QPixmap pixmap = new QPixmap();
            QPainter painter = new QPainter(pixmap);
            painter.setRenderHint(QPainter.RenderHint.Antialiasing);
            scene.render(painter);
            painter.end();

            pixmap.save("scene.png");
//! [4]
        }



        if (true)
        {
            QGraphicsScene scene = new QGraphicsScene();
//! [6]
            QGraphicsView view = new QGraphicsView(scene);
            view.setViewport(new QGLWidget(new QGLFormat(
                new QGL.FormatOptions(QGL.FormatOption.SampleBuffers))));
//! [6]
         }

        }

            class MyItem extends QGraphicsItem
            {

//! [5]
            public void mousePressEvent(QGraphicsSceneMouseEvent event)
            {
                org.qtjambi.qt.core.QMimeData data = new org.qtjambi.qt.core.QMimeData();
                data.setColorData(QColor.green);

                QDrag drag = new QDrag(event.widget());
                drag.setMimeData(data);
                //drag.start();
            }
//! [5]
            public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {}
            public QRectF boundingRect() { return null; }
            }

}
