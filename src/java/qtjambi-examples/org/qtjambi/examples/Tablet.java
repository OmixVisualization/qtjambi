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

import org.qtjambi.qt.core.QDir;
import org.qtjambi.qt.core.QEvent;
import org.qtjambi.qt.core.QPoint;
import org.qtjambi.qt.core.QRect;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QBrush;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QImage;
import org.qtjambi.qt.gui.QPaintEvent;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPen;
import org.qtjambi.qt.gui.QTabletEvent;
import org.qtjambi.qt.widgets.QAction;
import org.qtjambi.qt.widgets.QActionGroup;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QColorDialog;
import org.qtjambi.qt.widgets.QFileDialog;
import org.qtjambi.qt.widgets.QMainWindow;
import org.qtjambi.qt.widgets.QMenu;
import org.qtjambi.qt.widgets.QMessageBox;
import org.qtjambi.qt.widgets.QWidget;

//! [0]
public class Tablet extends QMainWindow
{
    public static TabletCanvas myCanvas;

    private QAction brushColorAction;

    private QActionGroup alphaChannelGroup;
    private QAction alphaChannelPressureAction;
    private QAction alphaChannelTiltAction;
    private QAction noAlphaChannelAction;

    private QActionGroup colorSaturationGroup;
    private QAction colorSaturationVTiltAction;
    private QAction colorSaturationHTiltAction;
    private QAction colorSaturationPressureAction;
    private QAction noColorSaturationAction;

    private QActionGroup lineWidthGroup;
    private QAction lineWidthPressureAction;
    private QAction lineWidthTiltAction;
    private QAction lineWidthFixedAction;

    private QAction exitAction;
    private QAction saveAction;
    private QAction loadAction;
//! [0]

    private QAction aboutAction;
    private QAction aboutQtAction;

    private QMenu fileMenu;
    private QMenu brushMenu;
    private QMenu tabletMenu;
    private QMenu helpMenu;
    private QMenu colorSaturationMenu;
    private QMenu lineWidthMenu;
    private QMenu alphaChannelMenu;

    public enum AlphaChannelType { AlphaPressure, AlphaTilt, NoAlpha }
    public enum ColorSaturationType { SaturationVTilt, SaturationHTilt,
                                   SaturationPressure, NoSaturation }
    public enum LineWidthType { LineWidthPressure, LineWidthTilt, NoLineWidth }

//! [1]
    public Tablet()
    {
        myCanvas = new TabletCanvas();

        createActions();
        createMenus();

        myCanvas.setColor(new QColor(Qt.GlobalColor.red));
        myCanvas.setLineWidthType(LineWidthType.LineWidthPressure);
        myCanvas.setAlphaChannelType(AlphaChannelType.NoAlpha);
        myCanvas.setColorSaturationType(ColorSaturationType.NoSaturation);

        setWindowTitle(tr("Tablet Example"));
        setCentralWidget(myCanvas);
    }
//! [1]

//! [2]
    public void brushColorAct()
    {
        QColor color = QColorDialog.getColor(myCanvas.color());

        if (color.isValid())
            myCanvas.setColor(color);
    }
//! [2]

//! [3]
    public void alphaActionTriggered(QAction action)
    {
        if (action.equals(alphaChannelPressureAction)) {
            myCanvas.setAlphaChannelType(AlphaChannelType.AlphaPressure);
        } else if (action.equals(alphaChannelTiltAction)) {
            myCanvas.setAlphaChannelType(AlphaChannelType.AlphaTilt);
        } else {
            myCanvas.setAlphaChannelType(AlphaChannelType.NoAlpha);
        }
    }
//! [3]

//! [4]
    public void lineWidthActionTriggered(QAction action)
    {
        if (action.equals(lineWidthPressureAction)) {
            myCanvas.setLineWidthType(LineWidthType.LineWidthPressure);
        } else if (action.equals(lineWidthTiltAction)) {
            myCanvas.setLineWidthType(LineWidthType.LineWidthTilt);
        } else {
            myCanvas.setLineWidthType(LineWidthType.NoLineWidth);
        }
    }
//! [4]

//! [5]
    public void saturationActionTriggered(QAction action)
    {
        if (action.equals(colorSaturationVTiltAction)) {
            myCanvas.setColorSaturationType(ColorSaturationType.SaturationVTilt);
        } else if (action.equals(colorSaturationHTiltAction)) {
            myCanvas.setColorSaturationType(ColorSaturationType.SaturationHTilt);
        } else if (action.equals(colorSaturationPressureAction)) {
            myCanvas.setColorSaturationType(ColorSaturationType.SaturationPressure);
        } else {
            myCanvas.setColorSaturationType(ColorSaturationType.NoSaturation);
        }
    }
//! [5]

//! [6]
    public void saveAct()
    {
        String path = QDir.currentPath() + tr("/untitled.png");
        String fileName = QFileDialog.getSaveFileName(this, tr("Save Picture"),
                                 path);

        if (!myCanvas.saveImage(fileName))
            QMessageBox.information(this, tr("Error Saving Picture"),
                                          tr("Could not save the image"));
    }
//! [6]

//! [7]
    public void loadAct()
    {
        String fileName = QFileDialog.getOpenFileName(this, tr("Open Picture"),
                                                            QDir.currentPath());

        if (!myCanvas.loadImage(fileName))
            QMessageBox.information(this, tr("Error Opening Picture"),
                                          tr("Could not open picture"));
    }
//! [7]

//! [8]
    public void aboutAct()
    {
        QMessageBox.about(this, tr("About Tablet Example"),
                       tr("This example shows use of a Wacom tablet in Jambi Qt"));
    }
//! [8]

//! [9]
    private void createActions()
    {
//! [9]
        brushColorAction = new QAction(tr("&Brush Color..."), this);
        brushColorAction.setShortcut(tr("Ctrl+C"));
        brushColorAction.triggered.connect(this, "brushColorAct()");

//! [10]
        alphaChannelPressureAction = new QAction(tr("&Pressure"), this);
        alphaChannelPressureAction.setCheckable(true);

        alphaChannelTiltAction = new QAction(tr("&Tilt"), this);
        alphaChannelTiltAction.setCheckable(true);

        noAlphaChannelAction = new QAction(tr("No Alpha Channel"), this);
        noAlphaChannelAction.setCheckable(true);
        noAlphaChannelAction.setChecked(true);

        alphaChannelGroup = new QActionGroup(this);
        alphaChannelGroup.addAction(alphaChannelPressureAction);
        alphaChannelGroup.addAction(alphaChannelTiltAction);
        alphaChannelGroup.addAction(noAlphaChannelAction);
        alphaChannelGroup.triggered.connect(this, "alphaActionTriggered(QAction)");

//! [10]
        colorSaturationVTiltAction = new QAction(tr("&Vertical Tilt"), this);
        colorSaturationVTiltAction.setCheckable(true);

        colorSaturationHTiltAction = new QAction(tr("&Horizontal Tilt"), this);
        colorSaturationHTiltAction.setCheckable(true);

        colorSaturationPressureAction = new QAction(tr("&Pressure"), this);
        colorSaturationPressureAction.setCheckable(true);

        noColorSaturationAction = new QAction(tr("&No Color Saturation"), this);
        noColorSaturationAction.setCheckable(true);
        noColorSaturationAction.setChecked(true);

        colorSaturationGroup = new QActionGroup(this);
        colorSaturationGroup.addAction(colorSaturationVTiltAction);
        colorSaturationGroup.addAction(colorSaturationHTiltAction);
        colorSaturationGroup.addAction(colorSaturationPressureAction);
        colorSaturationGroup.addAction(noColorSaturationAction);
        colorSaturationGroup.triggered.connect(this, "saturationActionTriggered(QAction)");

        lineWidthPressureAction = new QAction(tr("&Pressure"), this);
        lineWidthPressureAction.setCheckable(true);
        lineWidthPressureAction.setChecked(true);
        lineWidthTiltAction = new QAction(tr("&Tilt"), this);
        lineWidthTiltAction.setCheckable(true);

        lineWidthFixedAction = new QAction(tr("&Fixed"), this);
        lineWidthFixedAction.setCheckable(true);

        lineWidthGroup = new QActionGroup(this);
        lineWidthGroup.addAction(lineWidthPressureAction);
        lineWidthGroup.addAction(lineWidthTiltAction);
        lineWidthGroup.addAction(lineWidthFixedAction);
        lineWidthGroup.triggered.connect(this, "lineWidthActionTriggered(QAction)");

        exitAction = new QAction(tr("E&xit"), this);
        exitAction.setShortcut(tr("Ctrl+X"));
        exitAction.triggered.connect(this, "close()");

        loadAction = new QAction(tr("&Open..."), this);
        loadAction.setShortcut(tr("Ctrl+O"));
        loadAction.triggered.connect(this, "loadAct()");

        saveAction = new QAction(tr("&Save As..."), this);
        saveAction.setShortcut(tr("Ctrl+S"));
        saveAction.triggered.connect(this, "saveAct()");

        aboutAction = new QAction(tr("A&bout"), this);
        aboutAction.setShortcut(tr("Ctrl+B"));
        aboutAction.triggered.connect(this, "aboutAct()");

        aboutQtAction = new QAction(tr("About &Qt Jambi"), this);
        aboutQtAction.setShortcut(tr("Ctrl+Q"));
        aboutQtAction.triggered.connect(QApplication.instance(), "aboutQt()");
//! [11]
    }
//! [11]

//! [12]
    private void createMenus()
    {
        fileMenu = menuBar().addMenu(tr("&File"));
        fileMenu.addAction(loadAction);
        fileMenu.addAction(saveAction);
        fileMenu.addSeparator();
        fileMenu.addAction(exitAction);

        brushMenu = menuBar().addMenu(tr("&Brush"));
        brushMenu.addAction(brushColorAction);

        tabletMenu = menuBar().addMenu(tr("&Tablet"));

        lineWidthMenu = tabletMenu.addMenu(tr("&Line Width"));
        lineWidthMenu.addAction(lineWidthPressureAction);
        lineWidthMenu.addAction(lineWidthTiltAction);
        lineWidthMenu.addAction(lineWidthFixedAction);

        alphaChannelMenu = tabletMenu.addMenu(tr("&Alpha Channel"));
        alphaChannelMenu.addAction(alphaChannelPressureAction);
        alphaChannelMenu.addAction(alphaChannelTiltAction);
        alphaChannelMenu.addAction(noAlphaChannelAction);

        colorSaturationMenu = tabletMenu.addMenu(tr("&Color Saturation"));
        colorSaturationMenu.addAction(colorSaturationVTiltAction);
        colorSaturationMenu.addAction(colorSaturationHTiltAction);
        colorSaturationMenu.addAction(noColorSaturationAction);

        helpMenu = menuBar().addMenu("&Help");
        helpMenu.addAction(aboutAction);
        helpMenu.addAction(aboutQtAction);
    }
//! [12]

//! [13]
    class TabletCanvas extends QWidget
    {
        private AlphaChannelType alphaChannelType;
        private ColorSaturationType colorSaturationType;
        private LineWidthType lineWidthType;
        private QTabletEvent.TabletDevice myTabletDevice;
        private QColor myColor;

        private QImage image;
        private QBrush myBrush;
        private QPen myPen;
        private boolean deviceDown;
        private QPoint polyLine[] = new QPoint[3];
//! [13]

//! [14]
        public TabletCanvas()
        {
            myBrush = new QBrush();
            myPen = new QPen();
            image = new QImage(500, 500, QImage.Format.Format_ARGB32);
            QPainter painter = new QPainter(image);
            painter.fillRect(0, 0, 499, 499, new QBrush(new QColor(Qt.GlobalColor.white)));
            setAutoFillBackground(true);
            deviceDown = false;
            myColor = new QColor(Qt.GlobalColor.red);
            myTabletDevice = QTabletEvent.TabletDevice.Stylus;
            alphaChannelType = AlphaChannelType.NoAlpha;
            colorSaturationType = ColorSaturationType.NoSaturation;
            lineWidthType = LineWidthType.LineWidthPressure;
        }
//! [14]

        public void setAlphaChannelType(AlphaChannelType type)
        {
            alphaChannelType = type;
        }

        public void setColorSaturationType(ColorSaturationType type)
        {
            colorSaturationType = type;
        }

        public void setLineWidthType(LineWidthType type)
        {
            lineWidthType = type;
        }

        public void setColor(QColor color)
        {
            myColor = color;
        }

        public QColor color()
        {
            return myColor;
        }

        public void setTabletDevice(QTabletEvent.TabletDevice device)
        {
            myTabletDevice = device;
        }

//! [15]
        boolean saveImage(String file)
        {
            return image.save(file);
        }
//! [15]

//! [16]
        boolean loadImage(String file)
        {
            boolean success = image.load(file);

            if (success) {
                update();
                return true;
            }
            return false;
        }
//! [16]

        @Override
//! [17]
        protected void tabletEvent(QTabletEvent event)
        {
            switch (event.type()) {
                case TabletPress:
                    if (deviceDown)
                        deviceDown = true;
                    break;
                case TabletRelease:
                    if (!deviceDown)
                        deviceDown = false;
                    break;
                case TabletMove:
                    polyLine[2] = polyLine[1];
                    polyLine[1] = polyLine[0];
                    polyLine[0] = event.pos();

                    if (deviceDown) {
                        updateBrush(event);
                        QPainter painter = new QPainter(image);
                        paintImage(painter, event);
                    }
                    break;
                default:
                    break;
            }
            update();
        }
//! [17]

        @Override
//! [18]
        protected void paintEvent(QPaintEvent event)
        {
            QPainter painter = new QPainter(this);
            painter.drawImage(new QPoint(0, 0), image);
        }
//! [18]

//! [19]
        private void paintImage(QPainter painter, QTabletEvent event)
        {
            QPoint brushAdjust = new QPoint(10, 10);

            switch (myTabletDevice) {
                case Stylus:
                    painter.setBrush(myBrush);
                    painter.setPen(myPen);
                    painter.drawLine(polyLine[1], event.pos());
                    break;
                case Airbrush:
                    myBrush.setColor(myColor);
                    myBrush.setStyle(brushPattern(event.pressure()));
                    painter.setPen(Qt.PenStyle.NoPen);
                    painter.setBrush(myBrush);

                    for (int i = 0; i < 3; ++i) {
                        painter.drawEllipse(new QRect(polyLine[i].subtract(brushAdjust),
                                                      polyLine[i].add(brushAdjust)));
                    }
                    break;
                default:
                    System.err.println("Unsupported tablet device.");
            }
        }
//! [19]

//! [20]
        private Qt.BrushStyle brushPattern(double value)
        {
            int pattern = ((int) (value * 100.0)) % 7;

            switch (pattern) {
                case 0:
                    return Qt.BrushStyle.SolidPattern;
                case 1:
                    return Qt.BrushStyle.Dense1Pattern;
                case 2:
                    return Qt.BrushStyle.Dense2Pattern;
                case 3:
                    return Qt.BrushStyle.Dense3Pattern;
               case 4:
                    return Qt.BrushStyle.Dense4Pattern;
                case 5:
                    return Qt.BrushStyle.Dense5Pattern;
                case 6:
                    return Qt.BrushStyle.Dense6Pattern;
                default:
                    return Qt.BrushStyle.Dense7Pattern;
            }
        }
//! [20]

//! [21]
        private void updateBrush(QTabletEvent event)
        {
            int hue, value, alpha;
            hue = myColor.hue();
            value = myColor.value();
            alpha = myColor.alpha();

            int vValue = (int) (((event.yTilt() + 60.0) / 120.0) * 255);
            int hValue = (int) (((event.xTilt() + 60.0) / 120.0) * 255);
//! [21] //! [22]

            switch (alphaChannelType) {
                case AlphaPressure:
                    myColor.setAlpha((int) (event.pressure() * 255.0));
                    break;
                case AlphaTilt:
                    myColor.setAlpha(Math.max(Math.abs(vValue - 127), Math.abs(hValue - 127)));
                    break;
                default:
                    myColor.setAlpha(255);
            }

//! [22] //! [23]
            switch (colorSaturationType) {
                case SaturationVTilt:
                    myColor.setHsv(hue, vValue, value, alpha);
                    break;
                case SaturationHTilt:
                    myColor.setHsv(hue, hValue, value, alpha);
                    break;
                case SaturationPressure:
                    myColor.setHsv(hue, (int) (event.pressure() * 255.0), value, alpha);
                    break;
                default:
                    ;
            }

//! [23] //! [24]
            switch (lineWidthType) {
                case LineWidthPressure:
                    myPen.setWidthF(event.pressure() * 10 + 1);
                    break;
                case LineWidthTilt:
                    myPen.setWidthF(Math.max(Math.abs(vValue - 127), Math.abs(hValue - 127)) / 12);
                    break;
                default:
                    myPen.setWidthF(1);
            }

//! [24] //! [25]
            if (event.pointerType().equals(QTabletEvent.PointerType.Eraser)) {
                myBrush.setColor(new QColor(Qt.GlobalColor.white));
                myPen.setColor(new QColor(Qt.GlobalColor.white));
                myPen.setWidthF(event.pressure() * 10 + 1);
            } else {
                myBrush.setColor(myColor);
                myPen.setColor(myColor);
            }
        }
//! [25]
    }

    public class TabletApplication extends QApplication
    {
        public TabletApplication(String args[])
        {
            super(args);
        }

        @Override
//! [26]
        public boolean event(QEvent event)
        {
            if (event.type().equals(QEvent.Type.TabletEnterProximity) ||
                event.type().equals(QEvent.Type.TabletLeaveProximity)) {
                Tablet.myCanvas.setTabletDevice(((QTabletEvent) event).device());
                return true;
            }
            return super.event(event);
        }
//! [26]
    }

//! [27]
    public static void main(String args[])
    {
        Tablet.TabletApplication.initialize(args);

        Tablet mainWindow = new Tablet();
        mainWindow.resize(500, 500);
        mainWindow.show();

        Tablet.TabletApplication.execStatic();
    }
//! [27]
}
