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

import org.qtjambi.qt.core.QPoint;
import org.qtjambi.qt.core.QRect;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QBrush;
import org.qtjambi.qt.gui.QCloseEvent;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QCursor;
import org.qtjambi.qt.gui.QFontMetrics;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.gui.QImage;
import org.qtjambi.qt.gui.QKeyEvent;
import org.qtjambi.qt.gui.QMouseEvent;
import org.qtjambi.qt.gui.QPaintEvent;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPainter.RenderHint;
import org.qtjambi.qt.gui.QPen;
import org.qtjambi.qt.gui.QResizeEvent;
import org.qtjambi.qt.gui.QTransform;
import org.qtjambi.qt.gui.QWheelEvent;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "Mandelbrot")
//! [0]
public class Mandelbrot extends QWidget {

    private RenderThread thread = new RenderThread();
    private QImage pixmap = new QImage();
    private QPoint pixmapOffset = new QPoint();
    private QPoint lastDragPosition = new QPoint();
    private double centerX;
    private double centerY;
    private double pixmapScale;
    private double currentScale;

    private boolean abort = false;

    final double DefaultCenterX = -0.637011f;
    final double DefaultCenterY = -0.0395159f;
    final double DefaultScale = 0.00403897f;

    final double ZoomInFactor = 0.8f;
    final double ZoomOutFactor = 1 / ZoomInFactor;
    final int ScrollStep = 20;

    private Signal2<QImage, Double> renderedImage =
            new Signal2<QImage, Double>();
//! [0]

//! [1]
    public static void main(String args[]) {
        QApplication.initialize(args);
        Mandelbrot mainWindow = new Mandelbrot();
        mainWindow.show();
        QApplication.execStatic();
        QApplication.shutdown();
    }
//! [1]

//! [2]
    public Mandelbrot() {
        this(null);
    }
//! [2]

    @Override
    protected void disposed() {
        synchronized (thread) {
            abort = true;
            try {
                thread.join();
            } catch (InterruptedException e) { }
        }
    }

    public Mandelbrot(QWidget widget) {
        super(widget);
        centerX = DefaultCenterX;
        centerY = DefaultCenterY;
        pixmapScale = DefaultScale;
        currentScale = DefaultScale;

        renderedImage.connect(this, "updatePixmap(QImage, Double)");

        setWindowTitle(tr("Mandelbrot"));
        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));
        setCursor(new QCursor(Qt.CursorShape.CrossCursor));
        resize(550, 400);
    }

    @Override
//! [3]
    public void paintEvent(QPaintEvent event) {
        QPainter painter = new QPainter();
        painter.begin(this);
        painter.setRenderHint(RenderHint.SmoothPixmapTransform);
        painter.fillRect(rect(), new QBrush(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.black)));

        if (pixmap.isNull()) {
            String message = tr("Rendering initial image, please wait...");
            painter.setPen(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.white));
            painter.drawText(rect(), Qt.AlignmentFlag.AlignCenter.value(),
                             message);
            painter.end();

            return;
        }
//! [3] //! [4]
        if (currentScale == pixmapScale) {
            painter.drawImage(pixmapOffset, pixmap);
        } else {
            double scaleFactor = pixmapScale / currentScale;
            int newWidth = (int) (pixmap.width() * scaleFactor);
            int newHeight = (int) (pixmap.height() * scaleFactor);

            int newX = pixmapOffset.x() + (pixmap.width() - newWidth) / 2;
            int newY = pixmapOffset.y() + (pixmap.height() - newHeight) / 2;

            painter.save();
            painter.translate(newX, newY);
            painter.scale(scaleFactor, scaleFactor);

            QTransform invertedMatrix = painter.worldTransform().inverted();
            QRect exposed = invertedMatrix.mapRect(rect());
            exposed = exposed.adjusted(-1, -1, 1, 1);

            if(scaleFactor>=0)
                painter.drawImage(pixmapOffset, pixmap);
            else
                painter.drawImage(exposed, pixmap, exposed);

            painter.restore();
//! [4] //! [5]
        }
//! [5]

//! [6]
        String text = tr("Use mouse wheel to zoom.")
                      + tr("Press and hold left mouse button to scroll.");
        QFontMetrics metrics = painter.fontMetrics();
        int textWidth = metrics.width(text);
        int offset = (width() - textWidth) / 2;

        painter.setPen(new QPen(Qt.PenStyle.NoPen));
        painter.setBrush(new QColor(0, 0, 0, 127));
        painter.drawRect(offset - 5, 0, textWidth + 10, metrics.lineSpacing() + 5);
        painter.setPen(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.white));
        painter.drawText(offset, metrics.leading() + metrics.ascent(), text);
        painter.end();
    }
//! [6]

    @Override
//! [7]
    public void resizeEvent(QResizeEvent event) {
        thread.render(centerX, centerY, currentScale, size());
    }
//! [7]

    @Override
//! [8]
    protected void closeEvent(QCloseEvent event) {
        synchronized (thread) {
            abort = true;
            thread.notify();
        }
        super.closeEvent(event);
    }
//! [8]

    @Override
//! [9]
    public void keyPressEvent(QKeyEvent event) {
        Qt.Key key = Qt.Key.resolve(event.key());
        switch (key) {
        case Key_Plus:
            zoom(ZoomInFactor);
            break;
        case Key_Minus:
            zoom(ZoomOutFactor);
            break;
        case Key_Left:
            scroll(-ScrollStep, 0);
            break;
        case Key_Right:
            scroll(+ScrollStep, 0);
            break;
        case Key_Down:
            scroll(0, -ScrollStep);
            break;
        case Key_Up:
            scroll(0, +ScrollStep);
            break;
        default:
            super.keyPressEvent(event);
        }
    }
//! [9]

    @Override
//! [10]
    public void wheelEvent(QWheelEvent event) {
        int numDegrees = event.delta() / 8;
        double numSteps = numDegrees / 15.0f;
        zoom(Math.pow(ZoomInFactor, numSteps));
    }
//! [10]

    @Override
//! [11]
    public void mousePressEvent(QMouseEvent event) {
        if (event.button() == Qt.MouseButton.LeftButton)
            lastDragPosition = event.pos();
    }
//! [11]

    @Override
//! [12]
    public void mouseMoveEvent(QMouseEvent event) {
        if (event.buttons().isSet(Qt.MouseButton.LeftButton)) {
            pixmapOffset.add(event.pos());
            pixmapOffset.subtract(lastDragPosition);

            lastDragPosition = event.pos();
            update();
        }
    }
//! [12]

    @Override
//! [13]
    public void mouseReleaseEvent(QMouseEvent event) {
        if (event.button() == Qt.MouseButton.LeftButton) {
            pixmapOffset.add(event.pos());
            pixmapOffset.subtract(lastDragPosition);
            lastDragPosition = new QPoint();

            int deltaX = (width() - pixmap.width()) / 2 - pixmapOffset.x();
            int deltaY = (height() - pixmap.height()) / 2 - pixmapOffset.y();
            scrollImage(deltaX, deltaY);
        }
    }
//! [13]

//! [14]
    private void updatePixmap(QImage image, Double scaleFactor) {
        if (!lastDragPosition.isNull())
            return;
        pixmap = image;
        pixmapOffset = new QPoint();
        lastDragPosition = new QPoint();
        pixmapScale = scaleFactor;

        QApplication.invokeLater(new Runnable() {
            public void run() {
                synchronized (thread) {
                    if(!abort)
                        update();
                }
            }
        });
    }
//! [14]

//! [15]
    protected void zoom(double zoomFactor) {
        currentScale *= zoomFactor;
        update();
        thread.render(centerX, centerY, currentScale, size());
    }
//! [15]

//! [16]
    public void scrollImage(int deltaX, int deltaY) {
        centerX += deltaX * currentScale;
        centerY += deltaY * currentScale;
        update();
        thread.render(centerX, centerY, currentScale, size());
    }
//! [16]

//! [17]
    private class RenderThread extends Thread {
        private double centerX;
        private double centerY;
        private double scaleFactor;
        private QSize resultSize;
        private boolean restart;

        final int ColormapSize = 512;
        int[] colormap = new int[ColormapSize];

        RenderThread() {
//! [17] //! [18]
            restart = false;

            for (int i = 0; i < ColormapSize; ++i) {
                double wave = 380.0 + (i * 400.0 / ColormapSize);
                colormap[i] = rgbFromWaveLength(wave);
            }
        }
//! [18]

//! [19]
        synchronized void render(double centerX, double centerY,
                                 double scaleFactor, QSize resultSize) {

            this.centerX = centerX;
            this.centerY = centerY;
            this.scaleFactor = scaleFactor;
            this.resultSize = resultSize;

            if (!isAlive()) {
                start();
            } else {
                restart = true;
                notify();
            }
        }
//! [19]

        @Override
//! [20]
        public void run() {
            QSize resultSize;
            double scaleFactor;
            double centerX;
            double centerY;

            while (true) {
//! [20] //! [21]
                synchronized (this) {
//! [21] //! [22]
                    resultSize = this.resultSize;
                    scaleFactor = this.scaleFactor;
                    centerX = this.centerX;
                    centerY = this.centerY;
                }
//! [22]

//! [23]
                int halfWidth = resultSize.width() / 2;
//! [23] //! [24]
                int halfHeight = resultSize.height() / 2;
                QImage.Format format = QImage.Format.Format_RGB32;
//! [24] //! [25]
                QImage image = new QImage(resultSize, format);

                final int NumPasses = 8;
                int pass = 0;
                while (pass < NumPasses) {
                    final int MaxIterations = (1 << (2 * pass + 6)) + 32;
                    final int Limit = 4;
                    boolean allBlack = true;

                    for (int y = -halfHeight; y < halfHeight; ++y) {
                        if (restart)
                            break;
                        if (abort)
                            return;

                        double ay = centerY + (y * scaleFactor);

                        for (int x = -halfWidth; x < halfWidth; ++x) {
                            double ax = centerX + (x * scaleFactor);
                            double a1 = ax;
                            double b1 = ay;
                            int numIterations = 0;

                            do {
                                ++numIterations;
                                double a2 = (a1 * a1) - (b1 * b1) + ax;
                                double b2 = (2 * a1 * b1) + ay;
                                if ((a2 * a2) + (b2 * b2) > Limit)
                                    break;

                                ++numIterations;
                                a1 = (a2 * a2) - (b2 * b2) + ax;
                                b1 = (2 * a2 * b2) + ay;
                                if ((a1 * a1) + (b1 * b1) > Limit)
                                    break;
                            } while (numIterations < MaxIterations);

                            if (numIterations < MaxIterations) {
                                int index = numIterations % ColormapSize;
                                image.setPixel(x + halfWidth, y + halfHeight,
                                               colormap[index]);

                                allBlack = false;
                            } else {
                                image.setPixel(x + halfWidth, y + halfHeight,
                                               0xff000000);
                            }
                        }
                    }
                    if (allBlack && pass == 0) {
                        pass = 4;
                    } else {
                        synchronized (this) {
                        if (!restart) {
                            //renderedImage.emit(image, scaleFactor);
                            updatePixmap(image, scaleFactor);
                        }
                        }
                        ++pass;
                    }
                }
//! [25] //! [26]
                synchronized (this) {
//! [26] //! [27]
                    if (!restart)
                        try {
                            wait();
                        } catch (InterruptedException event) {
                            event.printStackTrace();
                        }
                    restart = false;
                }
            }
        }
//! [27]

//! [28]
        int rgbFromWaveLength(double wave) {
            double red= 0.0;
            double green = 0.0;
            double blue = 0.0;

            if (wave >= 380.0 && wave <= 440.0) {
                red = -1.0 * (wave - 440.0) / (440.0 - 380.0);
                blue = 1.0;
            } else if (wave >= 440.0 && wave <= 490.0) {
                green = (wave - 440.0) / (490.0 - 440.0);
                blue = 1.0;
            } else if (wave >= 490.0 && wave <= 510.0) {
                green = 1.0;
                blue = -1.0 * (wave - 510.0) / (510.0 - 490.0);
            } else if (wave >= 510.0 && wave <= 580.0) {
                red = (wave - 510.0) / (580.0 - 510.0);
                green = 1.0;
            } else if (wave >= 580.0 && wave <= 645.0) {
                red = 1.0;
                green = -1.0 * (wave - 645.0) / (645.0 - 580.0);
            } else if (wave >= 645.0 && wave <= 780.0) {
                red= 1.0;
            }

            double s = 1.0;
            if (wave > 700.0)
                s = 0.3 + 0.7 * (780.0 - wave) / (780.0 - 700.0);
            else if (wave < 420.0)
                s = 0.3 + 0.7 * (wave - 380.0) / (420.0 - 380.0);

            red = Math.pow(red * s, 0.8) * 255;
            green = Math.pow(green * s, 0.8) * 255;
            blue = Math.pow(blue * s, 0.8) * 255;

            QColor color = new QColor((int) red, (int) green, (int) blue);

            return color.rgb();
        }
    }
//! [28] //! [29]
}
//! [29]
