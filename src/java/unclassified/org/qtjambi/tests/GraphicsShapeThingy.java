/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

package org.qtjambi.tests;

import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.core.*;


public class GraphicsShapeThingy extends QGraphicsView {

    private enum MouseMode {
        None,
        Add,
        Select,
    }

    private QPainterPath path;

    private QGraphicsPathItem pathItem;
    private QGraphicsLineItem lineItem;

    private MouseMode mouseMode = MouseMode.None;

    public GraphicsShapeThingy() {

        QGraphicsScene scene = new QGraphicsScene();
        setScene(scene);

        // some initial setup...
        path = new QPainterPath();
        path.moveTo(100, 100);
        path.lineTo(200, 100);
        path.lineTo(300, 150);

        pathItem = scene.addPath(path);
        lineItem = scene.addLine(300, 150, 300, 150, new QPen(QColor.black, 0, Qt.PenStyle.DotLine));

        addControlPoint(100, 100);
        addControlPoint(200, 100);
        addControlPoint(300, 150);
    }


    private QGraphicsItemInterface addControlPoint(double x, double y) {
        QGraphicsRectItem rect = new QGraphicsRectItem(-2.5, -2.5, 5, 5) {
                public Object itemChange(QGraphicsItem.GraphicsItemChange change, Object value) {
                    System.out.println(change + ", " + value);

                    if (change == QGraphicsItem.GraphicsItemChange.ItemSelectedChange) {
                        setBrush(new QBrush(isSelected() ? QColor.white : QColor.red));
                    }

                    return super.itemChange(change, value);
                }
            };
        rect.setBrush(new QBrush(QColor.white));
        rect.setFlag(QGraphicsItem.GraphicsItemFlag.ItemIsSelectable, true);
        rect.setPos(x, y);
        scene().addItem(rect);



        return rect;
    }

    protected void keyPressEvent(QKeyEvent e) {
        if (mouseMode == MouseMode.None) {
            if (e.key() == Qt.Key.Key_Control.value()) {
                mouseMode = MouseMode.Add;

            }
        }
    }

    protected void keyReleaseEvent(QKeyEvent e) {
        if (e.key() == Qt.Key.Key_Control.value() && mouseMode == MouseMode.Add) {
            mouseMode = MouseMode.None;

            // Collaps the current adding line
            QLineF line = lineItem.line();
            lineItem.setLine(line.x1(), line.y1(), line.x1(), line.y1());
        }
    }

    protected void mousePressEvent(QMouseEvent e) {
        if (mouseMode == MouseMode.None) {
            // Enter selection mode...
            setDragMode(DragMode.RubberBandDrag);
        }

        super.mousePressEvent(e);
    }

    protected void mouseMoveEvent(QMouseEvent e) {
        if (mouseMode == MouseMode.Add) {
            // Update the end point of the line
            QPointF pos = mapToScene(e.pos());
            QLineF line = lineItem.line();
            lineItem.setLine(line.x1(), line.y1(), pos.x(), pos.y());
        }

        super.mouseMoveEvent(e);
    }

    protected void mouseReleaseEvent(QMouseEvent e) {
        if (mouseMode == MouseMode.Add) {
            // push the end point of the line to the path.
            QLineF line = lineItem.line();
            path.lineTo(line.x2(), line.y2());
            pathItem.setPath(path);

            // make the end point of the path the start of the line
            lineItem.setLine(line.x2(), line.y2(), line.x2(), line.y2());

            addControlPoint(line.x2(), line.y2());
        } else if (mouseMode == MouseMode.Select) {
            // Unset selection mode
            setDragMode(DragMode.NoDrag);
        }

        super.mouseReleaseEvent(e);
    }

    public static void main(String args[]) {
        QApplication.initialize(args);

        QGraphicsView view = new GraphicsShapeThingy();

        view.setRenderHints(QPainter.RenderHint.Antialiasing);
        view.resize(1024, 768);
        view.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}

/*

  protected void addVertex(QPointF pos){
      QGraphicsSvgItem Vertex = new QGraphicsSvgItem(){
              public java.lang.Object itemChange(QGraphicsItem.GraphicsItemChange change,
                                                 java.lang.Object value) {
                  System.out.println("State changed. "+"Has been selected"+this.isSelected());
                  if(this.isSelected())
                      System.out.println("Has been selected ");

                  //if(change == QGraphicsItem.GraphicsItemChange.ItemSelectedChange)
                    //System.out.println("Has been selected ");

                  super.itemChange(change, value);
                  return value;
              }
          };
      Vertex.setSharedRenderer(renderer);
      Vertex.setZValue(1);
      Vertex.setElementId("layer1");
      Vertex.setPos(pos.subtract(Vertex.boundingRect().bottomRight().divide(2)));
      scene.addItem(Vertex);
  }

*/