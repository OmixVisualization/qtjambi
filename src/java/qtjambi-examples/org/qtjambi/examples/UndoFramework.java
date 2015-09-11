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

package org.qtjambi.examples;

import java.util.LinkedList;
import java.util.List;
import java.util.Random;

import org.qtjambi.qt.core.QCoreApplication;
import org.qtjambi.qt.core.QPointF;
import org.qtjambi.qt.core.QRectF;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QBrush;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QKeySequence;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPolygonF;
import org.qtjambi.qt.gui.QTransform;
import org.qtjambi.qt.widgets.QAction;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QDialog;
import org.qtjambi.qt.widgets.QGraphicsItem;
import org.qtjambi.qt.widgets.QGraphicsItemInterface;
import org.qtjambi.qt.widgets.QGraphicsPolygonItem;
import org.qtjambi.qt.widgets.QGraphicsScene;
import org.qtjambi.qt.widgets.QGraphicsSceneMouseEvent;
import org.qtjambi.qt.widgets.QGraphicsView;
import org.qtjambi.qt.widgets.QMainWindow;
import org.qtjambi.qt.widgets.QMenu;
import org.qtjambi.qt.widgets.QMessageBox;
import org.qtjambi.qt.widgets.QUndoCommand;
import org.qtjambi.qt.widgets.QUndoStack;
import org.qtjambi.qt.widgets.QUndoView;
import org.qtjambi.qt.widgets.QVBoxLayout;

@QtJambiExample(name = "Undo Framework")
//! [0]
public class UndoFramework extends QMainWindow
{
    private QAction deleteAction;
    private QAction addBoxAction;
    private QAction addTriangleAction;
    private QAction undoAction;
    private QAction redoAction;
    private QAction exitAction;
    private QAction aboutAction;

    private QMenu fileMenu;
    private QMenu editMenu;
    private QMenu itemMenu;
    private QMenu helpMenu;

    private DiagramScene diagramScene;
    private QUndoStack undoStack;
    private QUndoView undoView;
//! [0]

    public static int itemCount = 0;

    public enum DiagramType { Box, Triangle }

//! [1]
    public UndoFramework()
    {
        undoStack = new QUndoStack();

        createActions();
        createMenus();

        undoStack.canRedoChanged.connect(redoAction, "setEnabled(boolean)");
        undoStack.canUndoChanged.connect(undoAction, "setEnabled(boolean)");

        createUndoView();

        diagramScene = new DiagramScene();
        diagramScene.setSceneRect(new QRectF(0, 0, 500, 500));

        diagramScene.itemMoved.connect(this, "itemMoved(UndoFramework$DiagramItem,QPointF)");

        setWindowTitle("Undo Framework");
        QGraphicsView view = new QGraphicsView(diagramScene);
        setCentralWidget(view);
        resize(700, 500);
    }
//! [1]

//! [2]
    private void createUndoView()
    {
        undoView = new QUndoView(undoStack);
        undoView.setWindowTitle(tr("Command List"));
        undoView.setAttribute(Qt.WidgetAttribute.WA_QuitOnClose, false);

        QDialog dialog = new QDialog(this);
        QVBoxLayout layout = new QVBoxLayout(dialog);
        layout.setContentsMargins(0, 0, 0, 0);
        layout.addWidget(undoView);
        dialog.show();
    }
//! [2]

//! [3]
    private void createActions()
    {
        deleteAction = new QAction(tr("&Delete Item"), this);
        deleteAction.setShortcut(tr("Del"));
        deleteAction.triggered.connect(this, "deleteItem()");
//! [3]

        addBoxAction = new QAction(tr("Add &Box"), this);
        addBoxAction.setShortcut(tr("Ctrl+O"));
        addBoxAction.triggered.connect(this, "addBox()");

        addTriangleAction = new QAction(tr("Add &Triangle"), this);
        addTriangleAction.setShortcut(tr("Ctrl+T"));
        addTriangleAction.triggered.connect(this, "addTriangle()");

//! [4]
        undoAction = new QAction(tr("&Undo"), this);
        undoAction.setShortcut(tr("Ctrl+Z"));
        undoAction.setEnabled(false);
        undoAction.triggered.connect(undoStack, "undo()");

        redoAction = new QAction(tr("&Redo"), this);
        List<QKeySequence> redoShortcuts = new LinkedList<QKeySequence>();
        redoShortcuts.add(new QKeySequence(tr("Ctrl+Y")));
        redoShortcuts.add(new QKeySequence(tr("Shift+Ctrl+Z")));
        redoAction.setShortcuts(redoShortcuts);
        redoAction.setEnabled(false);
        redoAction.triggered.connect(undoStack, "redo()");
//! [4]

        exitAction = new QAction(tr("E&xit"), this);
        exitAction.setShortcut(tr("Ctrl+Q"));
        exitAction.triggered.connect(this, "close()");

        aboutAction = new QAction(tr("&About"), this);
        List<QKeySequence> aboutShortcuts = new LinkedList<QKeySequence>();
        aboutShortcuts.add(new QKeySequence(tr("Ctrl+A")));
        aboutShortcuts.add(new QKeySequence(tr("Ctrl+B")));
        aboutAction.setShortcuts(aboutShortcuts);
        aboutAction.triggered.connect(this, "about()");
    }

//! [5]
    private void createMenus()
    {
//! [5]
        fileMenu = menuBar().addMenu(tr("&File"));
        fileMenu.addAction(exitAction);

//! [6]
        editMenu = menuBar().addMenu(tr("&Edit"));
        editMenu.addAction(undoAction);
        editMenu.addAction(redoAction);
        editMenu.addSeparator();
        editMenu.addAction(deleteAction);
        editMenu.aboutToShow.connect(this, "itemMenuAboutToShow()");
        editMenu.aboutToHide.connect(this, "itemMenuAboutToHide()");

//! [6]
        itemMenu = menuBar().addMenu(tr("&Item"));
        itemMenu.addAction(addBoxAction);
        itemMenu.addAction(addTriangleAction);

        helpMenu = menuBar().addMenu(tr("&About"));
        helpMenu.addAction(aboutAction);
//! [7]
    }
//! [7]

//! [8]
    public void itemMoved(DiagramItem movedItem, QPointF oldPosition)
    {
        undoStack.push(new MoveCommand(movedItem, oldPosition));
    }
//! [8]

//! [9]
    private void deleteItem()
    {
        if (diagramScene.selectedItems().isEmpty())
            return;

        QUndoCommand deleteCommand = new DeleteCommand(diagramScene);
        undoStack.push(deleteCommand);
    }
//! [9]

//! [10]
    private void itemMenuAboutToHide()
    {
        deleteAction.setEnabled(true);
    }
//! [10]

//! [11]
    private void itemMenuAboutToShow()
    {
        undoAction.setText(tr("Undo ") + undoStack.undoText());
        redoAction.setText(tr("Redo ") + undoStack.redoText());
        deleteAction.setEnabled(!diagramScene.selectedItems().isEmpty());
    }
//! [11]

//! [12]
    private void addBox()
    {
        QUndoCommand addCommand = new AddCommand(DiagramType.Box, diagramScene);
        undoStack.push(addCommand);
    }
//! [12]

//! [13]
    private void addTriangle()
    {
        QUndoCommand addCommand = new AddCommand(DiagramType.Triangle,
                                                 diagramScene);
        undoStack.push(addCommand);
    }
//! [13]

//! [14]
    private void about()
    {
        QMessageBox.about(this, tr("About Undo"),
                          tr("The <b>Undo</b> example demonstrates how to " +
                          "use Qt's undo framework."));
    }
//! [14]

    class DiagramItem extends QGraphicsPolygonItem
    {

        private QPolygonF boxPolygon;
        private QPolygonF trianglePolygon;

        public DiagramItem(DiagramType diagramType)
        {
            boxPolygon = new QPolygonF();
            trianglePolygon = new QPolygonF();

            if (diagramType == DiagramType.Box) {
                boxPolygon.add(new QPointF(0, 0));
                boxPolygon.add(new QPointF(0, 30));
                boxPolygon.add(new QPointF(30, 30));
                boxPolygon.add(new QPointF(30, 0));
                boxPolygon.add(new QPointF(0, 0));
                setPolygon(boxPolygon);
            } else {
                trianglePolygon.add(new QPointF(15, 0));
                trianglePolygon.add(new QPointF(30, 30));
                trianglePolygon.add(new QPointF(0, 30));
                trianglePolygon.add(new QPointF(15, 0));
                setPolygon(trianglePolygon);
            }

            Random random = new Random();
            QColor color = new QColor(random.nextInt(256), random.nextInt(256), random.nextInt(256));
            QBrush brush = new QBrush(color);
            setBrush(brush);
            setFlag(QGraphicsItem.GraphicsItemFlag.ItemIsSelectable, true);
            setFlag(QGraphicsItem.GraphicsItemFlag.ItemIsMovable, true);
        }

        public DiagramType diagramType() {
            return polygon().subtracted(boxPolygon).isEmpty() ? DiagramType.Box : DiagramType.Triangle;
        }
    }

//! [15]
    class DiagramScene extends QGraphicsScene
    {
        public Signal2<DiagramItem,QPointF> itemMoved =
            new Signal2<DiagramItem,QPointF>();

        private DiagramItem movingItem;
        private QPointF oldPos;
//! [15]

        public DiagramScene()
        {
            movingItem = null;
        }

        @Override
        public void mousePressEvent(QGraphicsSceneMouseEvent event)
        {
            QPointF mousePos = new QPointF(event.buttonDownScenePos(Qt.MouseButton.LeftButton).x(),
                             event.buttonDownScenePos(Qt.MouseButton.LeftButton).y());
            movingItem = (DiagramItem) itemAt(mousePos.x(), mousePos.y(), new QTransform());

            if (movingItem != null && event.button() == Qt.MouseButton.LeftButton) {
                oldPos = movingItem.pos();
            }
            super.mousePressEvent(event);
        }

        @Override
        public void mouseReleaseEvent(QGraphicsSceneMouseEvent event)
        {
            if (movingItem != null && event.button() == Qt.MouseButton.LeftButton) {
                if (oldPos != movingItem.pos()) {
                    itemMoved.emit(movingItem, oldPos);
                }
                movingItem = null;
            }
            super.mouseReleaseEvent(event);
        }

        @Override
        protected void drawBackground(QPainter painter, QRectF rect)
        {
            double startY = rect.top() - Math.IEEEremainder(rect.top(), 30.0);
            for (; startY < rect.bottom(); startY += 30.0)
                painter.drawLine(new QPointF(rect.left(), startY),
                                 new QPointF(rect.right(), startY));

            double startX = rect.left() - Math.IEEEremainder(rect.left(), 30.0);
            for (; startX < rect.right(); startX += 30.0)
                painter.drawLine(new QPointF(startX, rect.top()),
                                 new QPointF(startX, rect.bottom()));
        }
//! [16]
    }
//! [16]

//! [17]
    class DeleteCommand extends QUndoCommand
    {
        private DiagramItem myDiagramItem;
        private QGraphicsScene myGraphicsScene;
//! [17]

//! [18]
        public DeleteCommand(QGraphicsScene scene)
        {
            myGraphicsScene = scene;
            List<QGraphicsItemInterface> list = myGraphicsScene.selectedItems();
            list.get(0).setSelected(false);
            myDiagramItem = (DiagramItem) list.get(0);
            setText("Delete " + UndoFramework.createCommandString(myDiagramItem, myDiagramItem.pos()));
        }
//! [18]

        @Override
        public void redo()
        {
            myGraphicsScene.removeItem(myDiagramItem);
        }

        @Override
//! [19]
        public void undo()
        {
            myGraphicsScene.addItem(myDiagramItem);
            myGraphicsScene.update();
        }
//! [19]
    }

//! [20]
    class MoveCommand extends QUndoCommand
    {
        private DiagramItem myDiagramItem;
        private QPointF myOldPos;
        private QPointF newPos;
//! [20]


//! [21]
        public MoveCommand(DiagramItem diagramItem, QPointF oldPos)
        {
            myDiagramItem = diagramItem;
            newPos = diagramItem.pos();
            myOldPos = oldPos;
        }
//! [21]

        @Override
        public int id() { return 1; }

        @Override
//! [22]
        public void undo()
        {
            myDiagramItem.setPos(myOldPos);
            myDiagramItem.scene().update();
            setText(tr("Move " + UndoFramework.createCommandString(myDiagramItem, newPos)));
       }

        @Override
//! [23]
        public void redo()
        {
            myDiagramItem.setPos(newPos);
            setText(tr("Move " + UndoFramework.createCommandString(myDiagramItem, newPos)));
        }
//! [22] //! [23]

        @Override
//! [24]
        public boolean mergeWith(QUndoCommand other)
        {
            MoveCommand moveCommand = (MoveCommand) other;
            DiagramItem item = moveCommand.myDiagramItem;

            if (!myDiagramItem.equals(item))
                return false;

            newPos = item.pos();
            setText(tr("Move " + UndoFramework.createCommandString(myDiagramItem, newPos)));

            return true;
        }
//! [24]
    }

//! [25]
    class AddCommand extends QUndoCommand
    {
        private DiagramItem myDiagramItem;
        private QGraphicsScene myGraphicsScene;
        private QPointF initialPosition;
//! [25]

//! [26]
        public AddCommand(DiagramType addType, QGraphicsScene scene)
        {
            myGraphicsScene = scene;
            myDiagramItem = new DiagramItem(addType);
            initialPosition = new QPointF((UndoFramework.itemCount * 15) % (int) scene.width(),
                              (UndoFramework.itemCount * 15) % (int) scene.height());
            scene.update();
            ++UndoFramework.itemCount;
            setText(tr("Add " + UndoFramework.createCommandString(myDiagramItem, initialPosition)));
        }
//! [26]

        @Override
//! [27]
        public void redo()
        {
            myGraphicsScene.addItem(myDiagramItem);
            myDiagramItem.setPos(initialPosition);
            myGraphicsScene.clearSelection();
            myGraphicsScene.update();
        }
//! [27]

        @Override
//! [28]
        public void undo()
        {
            myGraphicsScene.removeItem(myDiagramItem);
            myGraphicsScene.update();
        }
//! [28]
    }

    public static String createCommandString(DiagramItem item, QPointF pos)
    {
        return QCoreApplication.translate("DiagramItem", (item.diagramType() == DiagramType.Box ? "Box" : "Triangle") +
               " at (" + pos.x() + ", " + pos.y() + ")");
    }

//! [29]
    public static void main(String args[])
    {
        QApplication.initialize(args);

        UndoFramework mainWindow = new UndoFramework();
        mainWindow.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
//! [29]
}
