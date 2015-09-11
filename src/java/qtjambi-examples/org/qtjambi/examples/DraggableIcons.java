
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


import org.qtjambi.qt.core.QByteArray;
import org.qtjambi.qt.core.QDataStream;
import org.qtjambi.qt.core.QIODevice;
import org.qtjambi.qt.core.QPoint;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QBrush;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QDrag;
import org.qtjambi.qt.gui.QDragEnterEvent;
import org.qtjambi.qt.gui.QDragMoveEvent;
import org.qtjambi.qt.gui.QDropEvent;
import org.qtjambi.qt.gui.QMouseEvent;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPixmap;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QFrame;
import org.qtjambi.qt.widgets.QHBoxLayout;
import org.qtjambi.qt.widgets.QLabel;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "Draggable Icons")
public class DraggableIcons extends QFrame
{

//![0]
    public DraggableIcons()
    {
        setMinimumSize(200, 200);
        setFrameStyle(QFrame.Shadow.Sunken.value() | QFrame.Shape.StyledPanel.value());
        setAcceptDrops(true);

        QLabel boatIcon = new QLabel(this);
        boatIcon.setPixmap(new QPixmap("classpath:org/qtjambi/examples/images/boat.png"));
        boatIcon.move(20, 20);

        QLabel carIcon = new QLabel(this);
        carIcon.setPixmap(new QPixmap("classpath:org/qtjambi/examples/images/car.png"));
        carIcon.move(120, 20);

        QLabel houseIcon = new QLabel(this);
        houseIcon.setPixmap(new QPixmap("classpath:org/qtjambi/examples/images/house.png"));
        houseIcon.move(20, 120);
    }
//![0]

    protected void dragEnterEvent(QDragEnterEvent event)
    {
        if (event.mimeData().hasFormat("application/x-dnditemdata")) {
            if (event.source().equals(this)) {
                event.setDropAction(Qt.DropAction.MoveAction);
                event.accept();
            } else {
                event.acceptProposedAction();
            }
        } else {
            event.ignore();
        }
    }

    protected void dragMoveEvent(QDragMoveEvent event)
    {
        if (event.mimeData().hasFormat("application/x-dnditemdata")) {
            if (event.source().equals(this)) {
                event.setDropAction(Qt.DropAction.MoveAction);
                event.accept();
            } else {
                event.acceptProposedAction();
            }
        } else {
            event.ignore();
        }
    }

    protected void dropEvent(QDropEvent event)
    {
        if (event.mimeData().hasFormat("application/x-dnditemdata")) {
            QByteArray itemData = event.mimeData().data("application/x-dnditemdata");
            QDataStream dataStream = new QDataStream(itemData, QIODevice.OpenModeFlag.ReadOnly);
        
            QPixmap pixmap = new QPixmap();
            QPoint offset = new QPoint();
            pixmap.readFrom(dataStream);
            offset.readFrom(dataStream);

            QLabel newIcon = new QLabel(this);
            newIcon.setPixmap(pixmap);
            newIcon.move(event.pos().subtract(offset));
            newIcon.show();
            newIcon.setAttribute(Qt.WidgetAttribute.WA_DeleteOnClose);

            if (event.source().equals(this)) {
                event.setDropAction(Qt.DropAction.MoveAction);
                event.accept();
            } else {
                event.acceptProposedAction();
            }
        } else {
            event.ignore();
        }
    }

//![1]
    protected void mousePressEvent(QMouseEvent event)
    {
        QLabel child = (QLabel) childAt(event.pos());
        if (child == null)
            return;

        QPixmap pixmap = child.pixmap();

        QByteArray itemData = new QByteArray();
        QDataStream dataStream = new QDataStream(itemData, QIODevice.OpenModeFlag.WriteOnly);
        pixmap.writeTo(dataStream);
        event.pos().subtract(child.pos()).writeTo(dataStream);

//! [1]

//! [2]
        org.qtjambi.qt.core.QMimeData mimeData = new org.qtjambi.qt.core.QMimeData();
        mimeData.setData("application/x-dnditemdata", itemData);
//! [2]
        
//! [3]
        QDrag drag = new QDrag(this);
        drag.setMimeData(mimeData);
        drag.setPixmap(pixmap);
        drag.setHotSpot(event.pos().subtract(child.pos()));
//! [3]

        QPixmap tempPixmap = new QPixmap(pixmap);
        QPainter painter = new QPainter();
        painter.begin(tempPixmap);
        painter.fillRect(pixmap.rect(), new QBrush(new QColor(127, 127, 127, 127)));
        painter.end();

        child.setPixmap(tempPixmap);

        if (drag.exec(new Qt.DropActions(Qt.DropAction.CopyAction,
            Qt.DropAction.MoveAction, Qt.DropAction.CopyAction)) == Qt.DropAction.MoveAction)
            child.close();
        else {
            child.show();
            child.setPixmap(pixmap);
        }
    }

    public static void main(String args[])
    {
        QApplication.initialize(args);

        QWidget mainWidget = new QWidget();
        QHBoxLayout horizontalLayout = new QHBoxLayout();
        horizontalLayout.addWidget(new DraggableIcons());
        horizontalLayout.addWidget(new DraggableIcons());

        mainWidget.setLayout(horizontalLayout);
        mainWidget.setWindowTitle("Draggable Icons");
        mainWidget.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
