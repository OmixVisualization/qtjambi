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

import org.qtjambi.qt.QSignalEmitter;
import org.qtjambi.qt.QVariant;
import org.qtjambi.qt.core.QByteArray;
import org.qtjambi.qt.core.QDir;
import org.qtjambi.qt.core.QPoint;
import org.qtjambi.qt.core.QRect;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QCloseEvent;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QImage;
import org.qtjambi.qt.gui.QImageWriter;
import org.qtjambi.qt.gui.QMouseEvent;
import org.qtjambi.qt.gui.QPaintEvent;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPen;
import org.qtjambi.qt.gui.QResizeEvent;
import org.qtjambi.qt.printsupport.QPrintDialog;
import org.qtjambi.qt.printsupport.QPrinter;
import org.qtjambi.qt.widgets.QAction;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QColorDialog;
import org.qtjambi.qt.widgets.QDialog;
import org.qtjambi.qt.widgets.QFileDialog;
import org.qtjambi.qt.widgets.QInputDialog;
import org.qtjambi.qt.widgets.QMainWindow;
import org.qtjambi.qt.widgets.QMenu;
import org.qtjambi.qt.widgets.QMessageBox;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample (name = "Scribble")
public class Scribble extends QMainWindow
{
    private ScribbleArea scribbleArea;

    private QMenu saveAsMenu;
    private QMenu fileMenu;
    private QMenu optionMenu;
    private QMenu helpMenu;

    private QAction openAct;
    private List<QAction> saveAsActs;
    private QAction exitAct;
    private QAction penColorAct;
    private QAction penWidthAct;
    private QAction printAct;
    private QAction clearScreenAct;
    private QAction aboutAct;
    private QAction aboutQtAct;

    public Scribble()
    {
        scribbleArea = new ScribbleArea();
        setCentralWidget(scribbleArea);

        createActions();
        createMenus();

        setWindowTitle(tr("Scribble"));
        resize(500, 500);
    }

    @Override
    protected void closeEvent(QCloseEvent event)
    {
        if (maybeSave()) {
            event.accept();
        } else {
            event.ignore();
        }
    }

    private void open()
    {
        if (maybeSave()) {
            String fileName = QFileDialog.getOpenFileName(this,
                                tr("Open File"), QDir.currentPath());
        if (fileName.length() != 0)
            scribbleArea.openImage(fileName);
        }
    }

    private void save()
    {
        QAction action = (QAction) QSignalEmitter.signalSender();
        QByteArray fileFormat = QVariant.toByteArray(action.data());
        saveFile(new String(fileFormat.toByteArray()));
    }

    private void penColor()
    {
        QColor newColor = QColorDialog.getColor(scribbleArea.penColor());
        if (newColor.isValid())
            scribbleArea.setPenColor(newColor);
    }

    private void penWidth()
    {

        Integer newWidth = QInputDialog.getInt(this, tr("Scribble"),
                                            tr("Select pen width:"),
                                            scribbleArea.penWidth(),
                                            1, 50, 1);
        if (newWidth != null)
            scribbleArea.setPenWidth(newWidth.intValue());
    }

    private boolean maybeSave()
    {
        if (scribbleArea.isModified()) {
           QMessageBox.StandardButton ret;
           ret = QMessageBox.warning(this, tr("Scribble"),
                              tr("The image has been modified.\n" +
                                 "Do you want to save your changes?"),
                              new QMessageBox.StandardButtons(QMessageBox.StandardButton.Save, QMessageBox.StandardButton.Discard));

            if (ret == QMessageBox.StandardButton.Save) {
                return saveFile("png");
            } else if (ret == QMessageBox.StandardButton.Cancel) {
                return false;
            }
        }
        return true;
    }

    private boolean saveFile(String fileFormat)
    {
        String initialPath = QDir.currentPath() + "/untitled." + fileFormat;

        String fileName = QFileDialog.getSaveFileName(this, tr("Save As"),
                                    initialPath,
                                    new QFileDialog.Filter(fileFormat.toUpperCase() + " Files (*."+
                                    fileFormat + ");;All Files (*)"));
        if (fileName.length() == 0) {
            return false;
        } else {
            return scribbleArea.saveImage(fileName, fileFormat);
        }
    }

    private void about()
    {
        QMessageBox.about(this, tr("About Scribble"),
            tr("<p>The <b>Scribble</b> example shows how to use QMainWindow as the "
               +"base widget for an application, and how to reimplement some of "
               +"QWidget's event handlers to receive the events generated for "
               +"the application's widgets:</p><p> We reimplement the mouse event "
               +"handlers to facilitate drawing, the paint event handler to "
               +"update the application and the resize event handler to optimize "
               +"the application's appearance. In addition we reimplement the "
               +"close event handler to intercept the close events before "
               +"terminating the application.</p><p> The example also demonstrates "
               +"how to use QPainter to draw an image in real time, as well as "
               +"to repaint widgets.</p>"));
    }

    private void createActions()
    {
        openAct = new QAction(tr("&Open..."), this);
        openAct.setShortcut(tr("Ctrl+O"));
        openAct.triggered.connect(this, "open()");

        saveAsActs = new LinkedList<QAction>();
        for (QByteArray format : QImageWriter.supportedImageFormats()) {
            String text = new String(format.toByteArray()).toUpperCase() + "...";

            QAction action = new QAction(text, this);
            action.setData(format);
            action.triggered.connect(this, "save()");
            saveAsActs.add(action);
        }

        printAct = new QAction(tr("&Print..."), this);
        printAct.triggered.connect(scribbleArea, "print()");

        exitAct = new QAction(tr("E&xit"), this);
        exitAct.setShortcut(tr("Ctrl+Q"));
        exitAct.triggered.connect(this, "close()");

        penColorAct = new QAction(tr("&Pen Color..."), this);
        penColorAct.triggered.connect(this, "penColor()");

        penWidthAct = new QAction(tr("Pen &Width..."), this);
        penWidthAct.triggered.connect(this, "penWidth()");

        clearScreenAct = new QAction(tr("&Clear Screen"), this);
        clearScreenAct.setShortcut(tr("Ctrl+L"));
        clearScreenAct.triggered.connect(scribbleArea, "clearImage()");

        aboutAct = new QAction(tr("&About"), this);
        aboutAct.triggered.connect(this, "about()");

        aboutQtAct = new QAction(tr("About &Qt"), this);
        aboutQtAct.triggered.connect(QApplication.instance(), "aboutQt()");
    }

    private void createMenus()
    {
        saveAsMenu = new QMenu(tr("&Save As"), this);
        for (QAction action : saveAsActs)
            saveAsMenu.addAction(action);

        fileMenu = new QMenu(tr("&File"), this);
        fileMenu.addAction(openAct);
        fileMenu.addMenu(saveAsMenu);
        fileMenu.addAction(printAct);
        fileMenu.addSeparator();
        fileMenu.addAction(exitAct);

        optionMenu = new QMenu(tr("&Options"), this);
        optionMenu.addAction(penColorAct);
        optionMenu.addAction(penWidthAct);
        optionMenu.addSeparator();
        optionMenu.addAction(clearScreenAct);

        helpMenu = new QMenu(tr("&Help"), this);
        helpMenu.addAction(aboutAct);
        helpMenu.addAction(aboutQtAct);

        menuBar().addMenu(fileMenu);
        menuBar().addMenu(optionMenu);
        menuBar().addMenu(helpMenu);
    }

    class ScribbleArea extends QWidget
    {
        private boolean modified;
        private boolean scribbling;
        private int myPenWidth;
        private QColor myPenColor;
        private QImage image;
        private QPoint lastPoint;

        public ScribbleArea()
        {
            image = new QImage();
            lastPoint = new QPoint();

            setAttribute(Qt.WidgetAttribute.WA_StaticContents);
            modified = false;
            scribbling = false;
            myPenWidth = 1;
            myPenColor = new QColor(Qt.GlobalColor.blue);
        }

        public boolean isModified()
        {
            return modified;
        }

        public QColor penColor()
        {
            return myPenColor;
        }

        public int penWidth()
        {
            return myPenWidth;
        }

        public boolean openImage(String fileName)
        {
            QImage loadedImage = new QImage();
            if (!loadedImage.load(fileName))
                return false;

            QSize newSize = loadedImage.size().expandedTo(size());
            loadedImage = resizeImage(loadedImage, newSize);
            image = loadedImage;
            modified = false;
            update();
            return true;
        }

        public boolean saveImage(String fileName, String fileFormat)
        {
            QImage visibleImage = image;
            visibleImage = resizeImage(visibleImage, size());

            if (visibleImage.save(fileName, fileFormat)) {
                modified = false;
                return true;
            } else {
                return false;
            }
        }

        public void setPenColor(QColor newColor)
        {
            myPenColor = newColor;

        }

        public void setPenWidth(int newWidth)
        {
            myPenWidth = newWidth;
        }

        public void clearImage()
        {
            image.fill(new QColor(Qt.GlobalColor.white).rgb());
            modified = true;
            update();
        }

        @Override
        protected void mousePressEvent(QMouseEvent event)
        {
            if (event.button() == Qt.MouseButton.LeftButton) {
                lastPoint = event.pos();
                scribbling = true;
            }
        }

        @Override
        protected void mouseMoveEvent(QMouseEvent event)
        {
            if ((event.buttons().isSet(Qt.MouseButton.LeftButton)) && scribbling)
                drawLineTo(event.pos());
        }

        @Override
        protected void mouseReleaseEvent(QMouseEvent event)
        {
            if (event.button() == Qt.MouseButton.LeftButton && scribbling) {
                drawLineTo(event.pos());
                scribbling = false;
            }
        }

        @Override
        protected void paintEvent(QPaintEvent event)
        {
            QPainter painter = new QPainter(this);
            painter.save();
            painter.drawImage(new QPoint(0, 0), image);
            painter.restore();
        }

        @Override
        protected void resizeEvent(QResizeEvent event)
        {
            if (width() > image.width() || height() > image.height()) {
                int newWidth = Math.max(width() + 128, image.width());
                int newHeight = Math.max(height() + 128, image.height());
                image = resizeImage(image, new QSize(newWidth, newHeight));
                update();
            }
            super.resizeEvent(event);
        }

        private void drawLineTo(QPoint endPoint)
        {
            QPainter painter = new QPainter(image);
            painter.setPen(new QPen(myPenColor, myPenWidth, Qt.PenStyle.SolidLine, Qt.PenCapStyle.RoundCap,
                                Qt.PenJoinStyle.RoundJoin));
            painter.drawLine(lastPoint, endPoint);
            modified = true;

            int rad = (myPenWidth / 2) + 2;
            update(new QRect(lastPoint, endPoint).normalized()
                                             .adjusted(-rad, -rad, +rad, +rad));
            lastPoint = endPoint;

            painter.end();
        }

        public QImage resizeImage(QImage image, QSize newSize)
        {
            if (image.size() == newSize)
                return image;

            QImage newImage = new QImage(newSize, QImage.Format.Format_RGB32);
            newImage.fill(new QColor(Qt.GlobalColor.white).rgb());
            QPainter painter = new QPainter(newImage);
            painter.drawImage(new QPoint(0, 0), image);
            painter.end();

            return newImage;
        }

        public void print()
        {
            QPrinter printer = new QPrinter(QPrinter.PrinterMode.HighResolution);

            QPrintDialog printDialog = new QPrintDialog(printer, this);
            if (printDialog.exec() == QDialog.DialogCode.Accepted.value()) {
                QPainter painter = new QPainter(printer);
                QRect rect = painter.viewport();
                QSize size = image.size();
                size.scale(rect.size(), Qt.AspectRatioMode.KeepAspectRatio);
                painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
                painter.setWindow(image.rect());
                painter.drawImage(0, 0, image);
                painter.end();
            }
        }
    }

    public static void main(String args[])
    {
        QApplication.initialize(args);

        new Scribble().show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
