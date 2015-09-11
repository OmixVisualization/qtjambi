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
import org.qtjambi.qt.core.QFile;
import org.qtjambi.qt.core.QIODevice;
import org.qtjambi.qt.core.QPoint;
import org.qtjambi.qt.core.QTextStream;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QDrag;
import org.qtjambi.qt.gui.QDragEnterEvent;
import org.qtjambi.qt.gui.QDropEvent;
import org.qtjambi.qt.gui.QMouseEvent;
import org.qtjambi.qt.gui.QPalette;
import org.qtjambi.qt.gui.QPixmap;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QFrame;
import org.qtjambi.qt.widgets.QLabel;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "Draggable Text")
public class DraggableText extends QWidget
{
    public DraggableText()
    {
        QFile dictionaryFile = new QFile("classpath:org/qtjambi/examples/dictionary/words.txt");
        dictionaryFile.open(QIODevice.OpenModeFlag.ReadOnly);
        QTextStream inputStream = new QTextStream(dictionaryFile);

        int x = 5;
        int y = 5;

        while (!inputStream.atEnd()) {
            String word = inputStream.readString();
            if (!word.equals("")) {
                DragLabel wordLabel = new DragLabel(word, this);
                wordLabel.move(x, y);
                wordLabel.show();
                x += wordLabel.width() + 2;
                if (x >= 195) {
                    x = 5;
                    y += wordLabel.height() + 2;
                }
            }
        }

        QPalette newPalette = palette();
        newPalette.setColor(QPalette.ColorRole.Window, new QColor(org.qtjambi.qt.core.Qt.GlobalColor.white));
        setPalette(newPalette);

        setAcceptDrops(true);
        setMinimumSize(400, Math.max(200, y));
        setWindowTitle(tr("Draggable Text"));
}
    protected void dragEnterEvent(QDragEnterEvent event)
    {
        if (event.mimeData().hasText()) {
            if (children().contains(event.source())) {
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
        if (event.mimeData().hasText()) {
            org.qtjambi.qt.core.QMimeData mime = event.mimeData();
            String pieces[] = mime.text().split("\\s+");
            QPoint position = event.pos();
            QPoint hotSpot = new QPoint();

            String hotSpotPos[] = mime.data("application/x-hotspot").toString().split("\\s");
            if (hotSpotPos.length == 2) {
                hotSpot.setX(Integer.parseInt(hotSpotPos[0]));
                hotSpot.setY(Integer.parseInt(hotSpotPos[1]));
            }

            for (String piece : pieces) {
                DragLabel newLabel = new DragLabel(piece, this);
                newLabel.move(position.subtract(hotSpot));
                newLabel.show();

                position.add(new QPoint(newLabel.width(), 0));
            }

            if (children().contains(event.source())) {
                event.setDropAction(Qt.DropAction.MoveAction);
                event.accept();
            } else {
                event.acceptProposedAction();
            }
        } else {
            event.ignore();
        }
    }

    class DragLabel extends QLabel
    {
        public DragLabel(String text, QWidget parent)
        {
            super(parent);

            setText(text);
            setAutoFillBackground(true);
            setFrameShape(QFrame.Shape.Panel);
            setFrameShadow(QFrame.Shadow.Raised);
        }

        protected void mousePressEvent(QMouseEvent event)
        {
            QPoint hotSpot = event.pos();

            org.qtjambi.qt.core.QMimeData mimeData = new org.qtjambi.qt.core.QMimeData();
            mimeData.setText(text());
            mimeData.setData("application/x-hotspot",
                              new QByteArray(String.valueOf(hotSpot.x())
                              + " " + String.valueOf(hotSpot.y())));

            QPixmap pixmap = new QPixmap(size());
            render(pixmap);

            QDrag drag = new QDrag(this);
            drag.setMimeData(mimeData);
            drag.setPixmap(pixmap);
            drag.setHotSpot(hotSpot);

            Qt.DropAction dropAction = drag.exec(
                new Qt.DropActions(Qt.DropAction.CopyAction,
                                 Qt.DropAction.MoveAction,
                                 Qt.DropAction.CopyAction));

            if (dropAction == Qt.DropAction.MoveAction) {
                close();
                update();
            }
        }
    }

    public static void main(String args[])
    {
        QApplication.initialize(args);

        new DraggableText().show();

        QApplication.execStatic();
        QApplication.shutdown();
    }

}
