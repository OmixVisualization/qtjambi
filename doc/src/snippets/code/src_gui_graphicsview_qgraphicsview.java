import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;

class CustomView1 extends QGraphicsView {
//! [5]
    protected void mousePressEvent(QMouseEvent event)
    {
        System.out.println("There are " + items(event.pos()).size()
                           + " items at position " + mapToScene(event.pos()));
    }
//! [5]
}

class CustomView2 extends QGraphicsView {
//! [6]
    protected void mousePressEvent(QMouseEvent event)
    {
        QGraphicsItemInterface item = itemAt(event.pos());
        if (item != null) {
            System.out.println("You clicked on item " + item);
        } else {
            System.out.println("You didn't click on an item.");
        }
    }
//! [6]
}


public class src_gui_graphicsview_qgraphicsview {
    private static void f0() {
//! [0]
        QGraphicsScene scene = new QGraphicsScene();
        scene.addText("Hello, world!");

        QGraphicsView view = new QGraphicsView(scene);
        view.show();
//! [0]
    }

    private static void f1() {
//! [1]
        QGraphicsScene scene = new QGraphicsScene();
        scene.addRect(new QRectF(-10, -10, 20, 20));

        QGraphicsView view = new QGraphicsView(scene);
        view.setRenderHints(QPainter.RenderHint.Antialiasing, QPainter.RenderHint.SmoothPixmapTransform);
        view.show();
//! [1]
    }

    private static void f2() {
//! [2]
        QGraphicsView view = new QGraphicsView();
        view.setBackgroundBrush(new QBrush(new QImage(":/images/backgroundtile.png")));
        view.setCacheMode(QGraphicsView.CacheModeFlag.CacheBackground);
//! [2]
    }

    private static void f3() {
//! [3]
        QGraphicsScene scene = new QGraphicsScene();
        scene.addText("GraphicsView rotated clockwise");

        QGraphicsView view = new QGraphicsView(scene);
        view.rotate(90); // the text is rendered with a 90 degree clockwise rotation
        view.show();
//! [3]
    }

    private static void f4() {
//! [4]
        QGraphicsScene scene = new QGraphicsScene();
        // scene.addItem(...

        QGraphicsView view = new QGraphicsView(scene);
        view.show();
        // ...

        QPrinter printer = new QPrinter(QPrinter.PrinterMode.HighResolution);
        printer.setPageSize(QPrinter.PageSize.A4);
        QPainter painter = new QPainter(printer);

        // print, fitting the viewport contents into a full page
        view.render(painter);

        // print the upper half of the viewport into the lower.
        // half of the page.
        QRect viewport = view.viewport().rect();
        view.render(painter,
                    new QRectF(0, printer.height() / 2, printer.width(), printer.height() / 2),
                    viewport.adjusted(0, 0, 0, -viewport.height() / 2)
        );

//! [4]
    }

    private static void f7() {
//! [7]
        QGraphicsScene scene = new QGraphicsScene();
        scene.addText("GraphicsView rotated clockwise");

        QGraphicsView view = new QGraphicsView(scene);
        view.rotate(90); // the text is rendered with a 90 degree clockwise rotation
        view.show();
//! [7]
    }
}
