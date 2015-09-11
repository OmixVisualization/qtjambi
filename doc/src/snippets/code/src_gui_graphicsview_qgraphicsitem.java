import javax.swing.text.html.HTMLDocument.HTMLReader.IsindexAction;

import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.QAction;
import org.qtjambi.qt.gui.QApplication;
import org.qtjambi.qt.gui.QCursor;
import org.qtjambi.qt.gui.QGraphicsEllipseItem;
import org.qtjambi.qt.gui.QGraphicsItemGroup;
import org.qtjambi.qt.gui.QGraphicsItemInterface;
import org.qtjambi.qt.gui.QGraphicsLineItem;
import org.qtjambi.qt.gui.QGraphicsRectItem;
import org.qtjambi.qt.gui.QGraphicsScene;
import org.qtjambi.qt.gui.QGraphicsSceneContextMenuEvent;
import org.qtjambi.qt.gui.QGraphicsSceneDragDropEvent;
import org.qtjambi.qt.gui.QGraphicsView;
import org.qtjambi.qt.gui.QLineF;
import org.qtjambi.qt.gui.QMenu;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPainterPath;
import org.qtjambi.qt.gui.QStyleOptionGraphicsItem;
import org.qtjambi.qt.gui.QTransform;
import org.qtjambi.qt.gui.QWidget;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_graphicsview_qgraphicsitem {

    QGraphicsItem item = null;
    QGraphicsView view = null;
    QGraphicsScene scene = null;

    public static void main(String args[]) {
        QApplication.initialize(args);
    }
//! [0]
    public class SimpleItem extends QGraphicsItem
    {
        public QRectF boundingRect() {
            double penWidth = 1;
            return new QRectF(-10 - penWidth / 2, -10 - penWidth / 2,
                          20 + penWidth / 2, 20 + penWidth / 2);
        }

        public void paint(QPainter painter, QStyleOptionGraphicsItem option,
                   QWidget widget) {
            painter.drawRoundRect(-10, -10, 20, 20, 5, 5);
        }
    }
//! [0]

    // Fake QGraphicsItem that has no abstract methods
    public class QGraphicsItem extends org.qtjambi.qt.gui.QGraphicsItem {
        double radius = 666;
        double diameter = 666;

        public QRectF boundingRect() {return null;}
        public void paint(QPainter arg0, QStyleOptionGraphicsItem arg1,QWidget arg2) {}
    }

//! [1]
    public class CustomItem extends QGraphicsItem
    {
       // ...
       public int type() {
           // Enable the use of qgraphicsitem_cast with this item.
           return enum_1.UserType.value() + 1;
       }
       // ...
    }
//! [1]

   public void snippetWrapper2() {
//! [2]
       item.setCursor(new QCursor(Qt.CursorShape.IBeamCursor));
//! [2]



//! [3]
       item.setCursor(new QCursor(Qt.CursorShape.IBeamCursor));
//! [3]


//! [4]
        QGraphicsRectItem rect = new QGraphicsRectItem();
        rect.setPos(100, 100);

        rect.sceneTransform().map(new QPointF(0, 0));
        // returns QPointF(100, 100);

        rect.sceneTransform().inverted().map(new QPointF(100, 100));
        // returns QPointF(0, 0);
//! [4]
   }

   public void snippetWrapper3() {
//! [5]
        QGraphicsRectItem rect = new QGraphicsRectItem();
        rect.setPos(100, 100);

        rect.deviceTransform(view.viewportTransform()).map(new QPointF(0, 0));
        // returns the item's (0, 0) point in view's viewport coordinates

        rect.deviceTransform(view.viewportTransform()).inverted().map(new QPointF(100, 100));
        // returns view's viewport's (100, 100) coordinate in item coordinates
//! [5]

        int x = 666;
        int y = 666;

//! [6]
        // Rotate an item 45 degrees around (0, 0).
        item.rotate(45);

        // Rotate an item 45 degrees around (x, y).
        item.setTransform(new QTransform().translate(x, y).rotate(45).translate(-x, -y));
//! [6]


//! [7]
        // Scale an item by 3x2 from its origin
        item.scale(3, 2);

        // Scale an item by 3x2 from (x, y)
        item.setTransform(new QTransform().translate(x, y).scale(3, 2).translate(-x, -y));
//! [7]
   }


//! [8]
    public class CircleItem extends QGraphicsItem
    {
        // ...
        public QRectF boundingRect() {
            double penWidth = 1;
            return new QRectF(-radius - penWidth / 2, -radius - penWidth / 2,
                          diameter + penWidth, diameter + penWidth);
        }
      // ...
    }
//! [8]


//! [9]
    public class RoundItem extends QGraphicsItem
    {
        // ...
        public QPainterPath shape() {
            QPainterPath path = new QPainterPath();
            path.addEllipse(boundingRect());
            return path;
        }
        // ...
    }
//! [9]


//! [10]
    public class RoundRectItem extends QGraphicsItem
    {
        // ...
        public void paint(QPainter painter,
                QStyleOptionGraphicsItem option,
                QWidget widget) {
            painter.drawRoundRect(-10, -10, 20, 20, 5, 5);
            }
        // ...
    }
//! [10]

    public class ButtonItem extends QGraphicsItem {}

    public void snippetWrapper4() {
//! [11]
        int ObjectName = 0;

        QGraphicsItemInterface item = scene.itemAt(100, 50);
        if (item.data(ObjectName).toString().length() == 0) {
            if (item instanceof ButtonItem)
                item.setData(ObjectName, "Button");
        }
//! [11]
    }

    public void snippetWrapper5() {
//! [12]
        QGraphicsScene scene = new QGraphicsScene();
        QGraphicsEllipseItem ellipse = scene.addEllipse(new QRectF(-10, -10, 20, 20));
        QGraphicsLineItem line = scene.addLine(new QLineF(-10, -10, 20, 20));

        line.installSceneEventFilter(ellipse);
        // line's events are filtered by ellipse's sceneEventFilter() function.

        ellipse.installSceneEventFilter(line);
        // ellipse's events are filtered by line's sceneEventFilter() function.
//! [12]
    }

    public class ClassWrapper1 {

//! [13]
        public class CutsomItem extends QGraphicsItem
        {
            // ...
            public void contextMenuEvent(QGraphicsSceneContextMenuEvent event) {
                QMenu menu = new QMenu();
                QAction emoveAction = menu.addAction("Remove");
                QAction arkAction = menu.addAction("Mark");
                QAction electedAction = menu.exec(event.screenPos());
                // ...
            }
            // ...
        }
//! [13]
    }

    public class ClassWrapper2 {
//! [14]
        public class CutsomItem extends QGraphicsItem
        {
            public CutsomItem()
            {
                setAcceptDrops(true);
                // ...
            }

            public void dragEnterEvent(QGraphicsSceneDragDropEvent event)
            {
                event.setAccepted(event.mimeData().hasFormat("text/plain"));
            }
            // ...
        }
//! [14]
    }

    public class ClassWrapper3 {

        public class CutsomItem extends QGraphicsItem {
//! [15]
            public Object itemChange(GraphicsItemChange change, Object value)
            {
                if (change == GraphicsItemChange.ItemPositionChange && scene() != null) {
                    // value is the new position.
                    QPointF newPos = (QPointF)value;
                    QRectF rect = scene().sceneRect();
                    if (!rect.contains(newPos)) {
                        // Keep the item inside the scene rect.
                        newPos.setX(Math.min(rect.right(), Math.max(newPos.x(), rect.left())));
                        newPos.setY(Math.min(rect.bottom(), Math.max(newPos.y(), rect.top())));
                        return newPos;
                    }
                }

                return super.itemChange(change, value);
            }
//! [15]
    }

    }

    public class ClassWrapper4 {
//! [16]
        public class CircleItem extends QGraphicsItem {

            // ...
            public void setRadius(double newRadius)
            {
                if (radius != newRadius) {
                    prepareGeometryChange();
                    radius = newRadius;
                }
            }
        }
//! [16]
    }

    public void snippetWrapper6() {
//! [17]
      // Group all selected items together
      QGraphicsItemGroup group = scene.createItemGroup(scene.selectedItems());

      // Destroy the group, and delete the group item
      scene.destroyItemGroup(group);
//! [17]
    }
}
