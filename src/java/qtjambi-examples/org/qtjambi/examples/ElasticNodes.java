/****************************************************************************
**
**  (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/


package org.qtjambi.examples;

import java.util.ArrayList;
import java.util.List;
import java.util.Vector;

import org.qtjambi.qt.core.QLineF;
import org.qtjambi.qt.core.QPointF;
import org.qtjambi.qt.core.QRectF;
import org.qtjambi.qt.core.QTimerEvent;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QBrush;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QFont;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.gui.QKeyEvent;
import org.qtjambi.qt.gui.QLinearGradient;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPainterPath;
import org.qtjambi.qt.gui.QPen;
import org.qtjambi.qt.gui.QPolygonF;
import org.qtjambi.qt.gui.QRadialGradient;
import org.qtjambi.qt.gui.QTransform;
import org.qtjambi.qt.gui.QWheelEvent;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QGraphicsItem;
import org.qtjambi.qt.widgets.QGraphicsItemInterface;
import org.qtjambi.qt.widgets.QGraphicsScene;
import org.qtjambi.qt.widgets.QGraphicsSceneMouseEvent;
import org.qtjambi.qt.widgets.QGraphicsView;
import org.qtjambi.qt.widgets.QStyle;
import org.qtjambi.qt.widgets.QStyleOptionGraphicsItem;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "Elastic Nodes")
public class ElasticNodes extends QGraphicsView {

    public static void main(String args[]) {
        QApplication.initialize(args);
        ElasticNodes elasticNodes = new ElasticNodes();
        elasticNodes.show();
        QApplication.execStatic();
        QApplication.shutdown();
    }

    private int timerId;
    private Node centerNode;
    private Vector<Node> nodes = new Vector<Node>();

    private static final QBrush BRUSH_DARK_GRAY = new QBrush(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.darkGray));
    private static final QPen QPEN_EDGE = new QPen(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.black), 1, Qt.PenStyle.SolidLine, Qt.PenCapStyle.RoundCap, Qt.PenJoinStyle.RoundJoin);
    private static final QPen QPEN_BLACK = new QPen(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.black), 0);

    private static QRadialGradient GRADIENT_SUNKEN;
    private static QRadialGradient GRADIENT_NORMAL;

    private static QPainterPath NODE_SHAPE;
    static{
        NODE_SHAPE = new QPainterPath();
        NODE_SHAPE.addEllipse(-10, -10, 20, 20);

        GRADIENT_SUNKEN = new QRadialGradient(-3, -3, 10);
        GRADIENT_SUNKEN.setCenter(3, 3);
        GRADIENT_SUNKEN.setFocalPoint(3, 3);
        GRADIENT_SUNKEN.setColorAt(1, new QColor(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.yellow)).lighter(120));
        GRADIENT_SUNKEN.setColorAt(0, new QColor(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.darkYellow)).lighter(120));

        GRADIENT_NORMAL = new QRadialGradient(-3, -3, 10);
        GRADIENT_NORMAL.setColorAt(0, new QColor(org.qtjambi.qt.core.Qt.GlobalColor.yellow));
        GRADIENT_NORMAL.setColorAt(1, new QColor(org.qtjambi.qt.core.Qt.GlobalColor.darkYellow));
    }


    public ElasticNodes() {
        QGraphicsScene scene = new QGraphicsScene(this);
        scene.setItemIndexMethod(QGraphicsScene.ItemIndexMethod.NoIndex);
        scene.setSceneRect(-200, -200, 400, 400);
        setScene(scene);

        setCacheMode(new QGraphicsView.CacheMode(QGraphicsView.CacheModeFlag.CacheBackground));

        setRenderHint(QPainter.RenderHint.Antialiasing);
        setTransformationAnchor(QGraphicsView.ViewportAnchor.AnchorUnderMouse);
        setResizeAnchor(QGraphicsView.ViewportAnchor.AnchorViewCenter);

        Node node1 = new Node(this);
        Node node2 = new Node(this);
        Node node3 = new Node(this);
        Node node4 = new Node(this);
        centerNode = new Node(this);
        Node node6 = new Node(this);
        Node node7 = new Node(this);
        Node node8 = new Node(this);
        Node node9 = new Node(this);
        scene.addItem(node1);
        scene.addItem(node2);
        scene.addItem(node3);
        scene.addItem(node4);
        scene.addItem(centerNode);
        scene.addItem(node6);
        scene.addItem(node7);
        scene.addItem(node8);
        scene.addItem(node9);
        scene.addItem(new Edge(node1, node2));
        scene.addItem(new Edge(node2, node3));
        scene.addItem(new Edge(node2, centerNode));
        scene.addItem(new Edge(node3, node6));
        scene.addItem(new Edge(node4, node1));
        scene.addItem(new Edge(node4, centerNode));
        scene.addItem(new Edge(centerNode, node6));
        scene.addItem(new Edge(centerNode, node8));
        scene.addItem(new Edge(node6, node9));
        scene.addItem(new Edge(node7, node4));
        scene.addItem(new Edge(node8, node7));
        scene.addItem(new Edge(node9, node8));

        node1.setPos(-50, -50);
        node2.setPos(0, -50);
        node3.setPos(50, -50);
        node4.setPos(-50, 0);
        centerNode.setPos(0, 0);
        node6.setPos(50, 0);
        node7.setPos(-50, 50);
        node8.setPos(0, 50);
        node9.setPos(50, 50);

        scale(0.8, 0.8);



        for (QGraphicsItemInterface item : scene().items()) {
            if (item instanceof Node)
                nodes.add((Node) item);
        }


        setMinimumSize(400, 400);
        setWindowTitle(tr("Elastic Nodes"));
        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));
    }

    private void itemMoved() {
        if (timerId == 0)
            timerId = startTimer(1000 / 25);
    }

    @Override
    protected void keyPressEvent(QKeyEvent event) {
        Qt.Key key = Qt.Key.resolve(event.key());
        switch (key) {
        case Key_Up:
            centerNode.moveBy(0, -20);
            break;
        case Key_Down:
            centerNode.moveBy(0, 20);
            break;
        case Key_Left:
            centerNode.moveBy(-20, 0);
            break;
        case Key_Right:
            centerNode.moveBy(20, 0);
            break;
        case Key_Plus:
            scaleView(1.2);
            break;
        case Key_Minus:
            scaleView(1 / 1.2);
            break;
        case Key_Space:
        case Key_Enter:
            for (QGraphicsItemInterface item : scene().items()) {
                if (item instanceof Node)
                    item.setPos(-150 + Math.random() * 300, -150 + Math.random() * 300);
            }
            break;
        default:
            super.keyPressEvent(event);
        }
    }

    @Override
    protected void timerEvent(QTimerEvent event) {
       for (Node node : nodes)
            node.calculateForces();

        boolean itemsMoved = false;
        for (Node node : nodes) {
            if (node.advance())
                itemsMoved = true;
        }

        if (!itemsMoved) {
            killTimer(timerId);
            timerId = 0;
        }
    }

    @Override
    protected void wheelEvent(QWheelEvent event) {
        scaleView(Math.pow(2, -event.delta() / 240.0));
    }

    @Override
    protected void drawBackground(QPainter painter, QRectF rect) {
        // Shadow
        QRectF sceneRect = this.sceneRect();
        QRectF rightShadow = new QRectF(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
        QRectF bottomShadow = new QRectF(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
        if (rightShadow.intersects(rect) || rightShadow.contains(rect))
            painter.fillRect(rightShadow, BRUSH_DARK_GRAY);
        if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
            painter.fillRect(bottomShadow, BRUSH_DARK_GRAY);

        // Fill
        QLinearGradient gradient = new QLinearGradient(sceneRect.topLeft(), sceneRect.bottomRight());
        gradient.setColorAt(0, new QColor(org.qtjambi.qt.core.Qt.GlobalColor.white));
        gradient.setColorAt(1, new QColor(org.qtjambi.qt.core.Qt.GlobalColor.lightGray));
        painter.fillRect(rect.intersected(sceneRect), new QBrush(gradient));
        painter.setBrush(new QBrush(Qt.BrushStyle.NoBrush));
        painter.drawRect(sceneRect);

        // Text
        QRectF textRect = new QRectF(sceneRect.left() + 4, sceneRect.top() + 4, sceneRect.width() - 4, sceneRect.height() - 4);
        String message = tr("Click and drag the nodes around, and zoom with the mouse wheel or the '+' and '-' keys");

        QFont font = painter.font();
        font.setBold(true);
        font.setPointSize(14);
        painter.setFont(font);
        painter.setPen(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.lightGray));
        painter.drawText(textRect.translated(2, 2), message);
        painter.setPen(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.black));
        painter.drawText(textRect, message);
    }

    private void scaleView(double scaleFactor) {
        QTransform m = transform();
        m.scale(scaleFactor, scaleFactor);
        double factor = m.mapRect(new QRectF(0, 0, 1, 1)).width();
        if (factor < 0.07 || factor > 100)
            return;

        scale(scaleFactor, scaleFactor);
    }

    public class Node extends QGraphicsItem {

        private Vector<Edge> edgeList = new Vector<Edge>();
        private QPointF newPos;
        private ElasticNodes graph;
        private double adjust = 2;
        private QRectF boundingRect = new QRectF(-10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);

        Node(ElasticNodes graphWidget) {
            graph = graphWidget;

            List<QGraphicsItem.GraphicsItemFlag> flagList = new ArrayList<QGraphicsItem.GraphicsItemFlag>();
            flagList.add(QGraphicsItem.GraphicsItemFlag.ItemIsMovable);
            // Bug#81 Since Qt 4.6 it is necessary to set: QGraphicsItem.GraphicsItemFlag.ItemSendsGeometryChanges
            //  in order to receive ItemPositionChange on #itemChange(GraphicsItemChange change, Object value).
            // We do this via reflection so as source is compatible with older 4.5.x.
            QGraphicsItem.GraphicsItemFlag flag = QGraphicsItem.GraphicsItemFlag.valueOf("ItemSendsGeometryChanges");
            if (flag != null)
                flagList.add(flag);
            QGraphicsItem.GraphicsItemFlag[] flagA = flagList.toArray(new QGraphicsItem.GraphicsItemFlag[flagList.size()]);
            setFlags(flagA);

            setZValue(1);
            newPos = pos();
        }

        private void addEdge(Edge edge) {
            edgeList.add(edge);
            edge.adjust();
        }

        private void calculateForces() {
            if (scene() == null || scene().mouseGrabberItem() == this) {
                newPos = pos();
                return;
            }
            // Sum up all forces pushing this item away
            double xvel = 0;
            double yvel = 0;
            for (Node node : nodes) {
                QPointF deltaPoint = mapFromItem(node, 0, 0);
                double dx = -deltaPoint.x();
                double dy = -deltaPoint.y();
                double l = 2.0 * (dx * dx + dy * dy);
                if (l > 0) {
                    xvel += (dx * 150.0) / l;
                    yvel += (dy * 150.0) / l;
                }
            }

            // Now subtract all forces pulling items together
            double weight = (edgeList.size() + 1) * 10;
            for (Edge edge : edgeList) {
                QPointF pos;
                if (edge.sourceNode() == this)
                    pos = mapFromItem(edge.destNode(), 0, 0);
                else
                    pos = mapFromItem(edge.sourceNode(), 0, 0);
                xvel += pos.x() / weight;
                yvel += pos.y() / weight;
            }

            if (Math.abs(xvel) < 0.1 && Math.abs(yvel) < 0.1)
                xvel = yvel = 0;

            QRectF sceneRect = scene().sceneRect();
            newPos.setX(Math.min(Math.max(newPos.x() + xvel, sceneRect.left() + 10), sceneRect.right() - 10));
            newPos.setY(Math.min(Math.max(newPos.y() + yvel, sceneRect.top() + 10), sceneRect.bottom() - 10));
        }

        private boolean advance() {
            if (newPos == pos())
                return false;

            setPos(newPos);
            return true;
        }

        @Override
        public QRectF boundingRect() {
            return boundingRect;
        }

        @Override
        public QPainterPath shape() {
            return NODE_SHAPE;
        }

        @Override
        public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {
            painter.setPen(Qt.PenStyle.NoPen);
            painter.setBrush(QColor.fromRgba(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.black).rgb() & 0x7fffffff));
            painter.drawEllipse(-7, -7, 20, 20);

            if ((option.state().isSet(QStyle.StateFlag.State_Sunken))) {
                painter.setBrush(GRADIENT_SUNKEN);
            } else {
                painter.setBrush(GRADIENT_NORMAL);
            }

            painter.setPen(QPEN_BLACK);
            painter.drawEllipse(-10, -10, 20, 20);
        }

        @Override
        public Object itemChange(GraphicsItemChange change, Object value) {
            switch (change) {
            case ItemPositionChange:
                for (Edge edge : edgeList)
                    edge.adjust();
                graph.itemMoved();
                break;
            default:
                break;
            }

            return super.itemChange(change, value);
        }

        @Override
        public void mousePressEvent(QGraphicsSceneMouseEvent event) {
            update();
            super.mousePressEvent(event);
        }

        @Override
        public void mouseReleaseEvent(QGraphicsSceneMouseEvent event) {
            update();
            super.mouseReleaseEvent(event);
        }
    }

    public class Edge extends QGraphicsItem {
        private Node source;
        private Node dest;

        private QPointF sourcePoint = new QPointF();
        private QPointF destPoint = new QPointF();
        private double arrowSize = 10;
        private double penWidth = 1;
        private double extra = (penWidth + arrowSize) / 2.0;

        private QRectF boundingRect = new QRectF();


        QPointF sourceArrowP1 = new QPointF();
        QPointF sourceArrowP2 = new QPointF();
        QPointF destArrowP1 = new QPointF();
        QPointF destArrowP2 = new QPointF();

        QPolygonF pol1 = new QPolygonF();
        QPolygonF pol2 = new QPolygonF();

        public Edge(Node sourceNode, Node destNode) {
            // setAcceptedMouseButtons(LeftButton);
            source = sourceNode;
            dest = destNode;
            source.addEdge(this);
            dest.addEdge(this);
            adjust();
        }

        private Node sourceNode() {
            return source;
        }

        private Node destNode() {
            return dest;
        }

        private void adjust() {
            double dx = source.pos().x()-dest.pos().x();
            double dy = source.pos().y()-dest.pos().y();

            double length = Math.sqrt(dx*dx+dy*dy);
            if (length == 0.0) return;

            double paddingX = dx/length*10;
            double paddingY = dy/length*10;

            prepareGeometryChange();
            sourcePoint.setX(source.pos().x() - paddingX);
            sourcePoint.setY(source.pos().y() - paddingY);

            destPoint.setX(dest.pos().x() + paddingX);
            destPoint.setY(dest.pos().y() + paddingY);

            boundingRect.setBottomLeft(source.pos());
            boundingRect.setTopRight(dest.pos());

            boundingRect = boundingRect.normalized();

            boundingRect.adjust(-extra, -extra, extra, extra);
        }

        @Override
        public QRectF boundingRect() {
            return boundingRect;
        }

        @Override
        public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {

            if (source == null || dest == null)
                return;

            // Draw the line itself
            QLineF line = new QLineF(sourcePoint, destPoint);

            painter.setPen(QPEN_EDGE);
            painter.drawLine(line);

            // Draw the arrows if there's enough room
            double angle;
            if (line.length() > 0)
                angle = Math.acos(line.dx() / line.length());
            else
                angle = 0;

            if (line.dy() >= 0)
                angle = (Math.PI * 2) - angle;

            sourceArrowP1.setX(sourcePoint.x() + Math.sin(angle + Math.PI / 3) * arrowSize);
            sourceArrowP1.setY(sourcePoint.y() + Math.cos(angle + Math.PI / 3) * arrowSize);

            sourceArrowP2.setX(sourcePoint.x() + Math.sin(angle + Math.PI - Math.PI / 3) * arrowSize);
            sourceArrowP2.setY(sourcePoint.y() + Math.cos(angle + Math.PI - Math.PI / 3) * arrowSize);

            destArrowP1.setX(destPoint.x() + Math.sin(angle - Math.PI / 3) * arrowSize);
            destArrowP1.setY(destPoint.y() + Math.cos(angle - Math.PI / 3) * arrowSize);

            destArrowP2.setX(destPoint.x() + Math.sin(angle - Math.PI + Math.PI / 3) * arrowSize);
            destArrowP2.setY(destPoint.y() + Math.cos(angle - Math.PI + Math.PI / 3) * arrowSize);

            pol1.clear();
            pol2.clear();

            pol1.append(line.p1());
            pol1.append(sourceArrowP1);
            pol1.append(sourceArrowP2);

            pol2.append(line.p2());
            pol2.append(destArrowP1);
            pol2.append(destArrowP2);

            painter.setBrush(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.black));
            painter.drawPolygon(pol1);
            painter.drawPolygon(pol2);
        }
    }
}
