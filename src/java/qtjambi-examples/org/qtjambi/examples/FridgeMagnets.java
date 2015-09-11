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

import org.qtjambi.qt.core.QByteArray;
import org.qtjambi.qt.core.QDataStream;
import org.qtjambi.qt.core.QFile;
import org.qtjambi.qt.core.QIODevice;
import org.qtjambi.qt.core.QPoint;
import org.qtjambi.qt.core.QRect;
import org.qtjambi.qt.core.QRectF;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.QTextStream;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QDrag;
import org.qtjambi.qt.gui.QDragEnterEvent;
import org.qtjambi.qt.gui.QDragMoveEvent;
import org.qtjambi.qt.gui.QDropEvent;
import org.qtjambi.qt.gui.QFont;
import org.qtjambi.qt.gui.QFontMetrics;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.gui.QImage;
import org.qtjambi.qt.gui.QMouseEvent;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPalette;
import org.qtjambi.qt.gui.QPixmap;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QLabel;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "Drag and Drop")
//! [0]
public class FridgeMagnets extends QWidget {
//! [0]

//! [1]
    public static void main(String args[]) {
        QApplication.initialize(args);
        FridgeMagnets fridgeMagnets = new FridgeMagnets(null);
        fridgeMagnets.show();
        QApplication.execStatic();
        QApplication.shutdown();
    }
//! [1]

//! [2]
    public FridgeMagnets(QWidget parent) {
        super(parent);
        QFile dictionaryFile;
        dictionaryFile = new QFile("classpath:org/qtjambi/examples/words.txt");
        dictionaryFile.open(QIODevice.OpenModeFlag.ReadOnly);
        QTextStream inputStream = new QTextStream(dictionaryFile);
//! [2]

//! [3]
        int x = 5;
        int y = 5;

        while (!inputStream.atEnd()) {
            String word = "";
            word = inputStream.readLine();
            if (!word.equals("")) {
                DragLabel wordLabel = new DragLabel(word, this);
                wordLabel.move(x, y);
                wordLabel.show();
                x += wordLabel.sizeHint().width() + 2;
                if (x >= 245) {
                    x = 5;
                    y += wordLabel.sizeHint().height() + 2;
                }
            }
        }
//! [3]
        inputStream.dispose();
        dictionaryFile.dispose();

//! [4]
        QPalette newPalette = palette();
        newPalette.setColor(QPalette.ColorRole.Window, new QColor(org.qtjambi.qt.core.Qt.GlobalColor.white));
        setPalette(newPalette);

        setMinimumSize(400, Math.max(200, y));
        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));
        setWindowTitle(tr("Fridge Magnets"));
//! [4]

//! [5]
        setAcceptDrops(true);
    }
//! [5]

    @Override
//! [6]
    public void dragEnterEvent(QDragEnterEvent event) {
//! [6] //! [7]
        if (event.mimeData().hasFormat("application/x-fridgemagnet")) {
            if (children().contains(event.source())) {
                event.setDropAction(Qt.DropAction.MoveAction);
                event.accept();
            } else {
                event.acceptProposedAction();
//! [7] //! [8]
            }
//! [8] //! [9]
        } else if (event.mimeData().hasText()) {
            event.acceptProposedAction();
        } else {
            event.ignore();
        }
    }
//! [9]

    @Override
//! [10]
    public void dragMoveEvent(QDragMoveEvent event) {
        if (event.mimeData().hasFormat("application/x-fridgemagnet")) {
            if (children().contains(event.source())) {
                event.setDropAction(Qt.DropAction.MoveAction);
                event.accept();
            } else {
                event.acceptProposedAction();
            }
        } else if (event.mimeData().hasText()) {
            event.acceptProposedAction();
        } else {
            event.ignore();
        }
    }
//! [10]

    @Override
//! [11]
    public void dropEvent(QDropEvent event) {
        if (event.mimeData().hasFormat("application/x-fridgemagnet")) {
            org.qtjambi.qt.core.QMimeData mime = event.mimeData();
//! [11] //! [12]
            QByteArray itemData = mime.data("application/x-fridgemagnet");
            QDataStream dataStream = new QDataStream(itemData,
                   new QIODevice.OpenMode(QIODevice.OpenModeFlag.ReadOnly));

            String text = dataStream.readString();
            QPoint offset = new QPoint();
            offset.readFrom(dataStream);

            DragLabel newLabel = new DragLabel(text, this);
            newLabel.move(new QPoint(event.pos().x() - offset.x(),
                                     event.pos().y() - offset.y()));
            newLabel.show();

            if (children().contains(event.source())) {
                event.setDropAction(Qt.DropAction.MoveAction);
                event.accept();
            } else {
                event.acceptProposedAction();
//! [12] //! [13]
            }
//! [13] //! [14]
        } else if (event.mimeData().hasText()) {
            String[] pieces = event.mimeData().text().split("\\s+");
            QPoint position = event.pos();

            for (String piece : pieces) {
                if (piece.equals(""))
                    continue;

                DragLabel newLabel = new DragLabel(piece, this);
                newLabel.move(position);
                newLabel.show();

                position.add(new QPoint(newLabel.width(), 0));
            }

            event.acceptProposedAction();
        } else {
            event.ignore();
        }
    }
//! [14]

//! [15]
    class DragLabel extends QLabel {
        private String labelText;

        public DragLabel(final String text, QWidget parent) {
            super(parent);

            QFontMetrics metrics = new QFontMetrics(font());
            QSize size = metrics.size(12, text);
            QImage image = new QImage(size.width() + 12, size.height() + 12,
                    QImage.Format.Format_ARGB32_Premultiplied);
            image.fill(0);

            QFont font = new QFont();
            font.setStyleStrategy(QFont.StyleStrategy.ForceOutline);
//! [15]

//! [16]
            QPainter painter = new QPainter();
            painter.begin(image);
            painter.setRenderHint(QPainter.RenderHint.Antialiasing);
            painter.setBrush(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.white));
            QRectF frame = new QRectF(0.5, 0.5, image.width() - 1,
                                      image.height() - 1);
            painter.drawRoundRect(frame, 10 * 100 / image.width(), 10 * 100 / image.height());

            painter.setFont(font);
            painter.setBrush(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.black));

            QRect rectangle = new QRect(new QPoint(6, 6), size);
            painter.drawText(rectangle, Qt.AlignmentFlag.AlignCenter.value(),
                             text);
            painter.end();
//! [16]

//! [17]
            setPixmap(QPixmap.fromImage(image));
            labelText = text;
        }
//! [17]

        @Override
//! [18]
        public void mousePressEvent(QMouseEvent event) {
            QByteArray itemData = new QByteArray();
            QDataStream dataStream;
            dataStream = new QDataStream(itemData,
                    new QIODevice.OpenMode(QIODevice.OpenModeFlag.WriteOnly));

            dataStream.writeString(labelText);
            QPoint position = new QPoint(event.pos().x() - rect().topLeft().x(),
                                         event.pos().y() - rect().topLeft().y());
            position.writeTo(dataStream);
//! [18]

//! [19]
            org.qtjambi.qt.core.QMimeData mimeData = new org.qtjambi.qt.core.QMimeData();
            mimeData.setData("application/x-fridgemagnet", itemData);
            mimeData.setText(labelText);
//! [19]

//! [20]
            QDrag drag = new QDrag(this);
            drag.setMimeData(mimeData);

            drag.setHotSpot(new QPoint(event.pos().x() - rect().topLeft().x(),
                                       event.pos().y() - rect().topLeft().y()));
            drag.setPixmap(pixmap());

            hide();
//! [20]

//! [21]
            if (drag.exec(Qt.DropAction.MoveAction) == Qt.DropAction.MoveAction)
                close();
            else
                show();
        }
//! [21]
    }
//! [22]
}
//! [22]
