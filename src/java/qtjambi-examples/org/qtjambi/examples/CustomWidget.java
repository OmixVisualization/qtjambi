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

import org.qtjambi.qt.QtBlockedSlot;
import org.qtjambi.qt.QtPropertyDesignable;
import org.qtjambi.qt.QtPropertyReader;
import org.qtjambi.qt.QtPropertyResetter;
import org.qtjambi.qt.QtPropertyWriter;
import org.qtjambi.qt.core.QPointF;
import org.qtjambi.qt.core.QRect;
import org.qtjambi.qt.core.QRectF;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.QTimeLine;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QBrush;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QFont;
import org.qtjambi.qt.gui.QFontMetrics;
import org.qtjambi.qt.gui.QGradient;
import org.qtjambi.qt.gui.QLinearGradient;
import org.qtjambi.qt.gui.QPaintEvent;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPainterPath;
import org.qtjambi.qt.gui.QPen;
import org.qtjambi.qt.gui.QRadialGradient;
import org.qtjambi.qt.gui.QResizeEvent;
import org.qtjambi.qt.gui.QTransform;
import org.qtjambi.qt.widgets.QWidget;

public class CustomWidget extends QWidget {

    public enum SpeedUnit {
        MilesPerHour("mph"),
        KilometersPerHour("km/h");

        private String title;
        private SpeedUnit(String title) {
            this.title = title;
        }

        public String title() { return title; }
    }

    //
    // Constructors
    //
    public CustomWidget() { this(null); }
    public CustomWidget(QWidget parent) { super(parent); }

    //
    // Caches for commonly used stuff
    //
    private int x = rect().x();
    private int y = rect().y();
    private int w = rect().width();
    private int h = rect().height();
    private QRect rect = rect();
    private QBrush gradient = null;
    private QRectF innerCircle = null;
    private QRectF outerCircle = null;
    private QPainterPath innerCirclePath = null;
    private QPainterPath needlePath = null;
    private QBrush radialGradient = null;
    private QPainterPath framePath = null;
    private QPen speedBarPen = new QPen(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.black), 4);
    private QTimeLine timeLine = new QTimeLine(1, this); {
        timeLine.valueChanged.connect(this, "animateSpeed(double)");
        timeLine.finished.connect(this, "noLongerAnimating()");
    }

    //
    // Signals
    //
    public Signal0 propertyChanged = new Signal0(); {
        propertyChanged.connect(this, "update()");
    }

    public Signal1<Integer> speedChanged = new Signal1<Integer>();


    /**
     * Calculates the size of the frame.
     * @return The width of the frame.
     */
    private static final double FRAMING_SIZE = 0.01f;
    private double framingSize() {
        return FRAMING_SIZE * (double)outerCircle().width();
    }

    //
    // topSpeed property
    //
    private static final int MAX_SPEED = 260;
    private int maxSpeed = MAX_SPEED;

    @QtPropertyReader(name="topSpeed")
    @QtBlockedSlot public final int maxSpeed() {
        return maxSpeed;
    }

    @QtPropertyWriter(name="topSpeed")
    public final void setMaxSpeed(int maxSpeed) {
        this.maxSpeed = maxSpeed; propertyChanged.emit();
    }

    @QtPropertyResetter(name="topSpeed")
    public final void resetMaxSpeed() {
        setMaxSpeed(MAX_SPEED);
    }

    //
    // animationSpeed property
    //
    private static final double ANIMATION_SPEED = 10.0f;
    private double animationSpeed = ANIMATION_SPEED;

    public final double animationSpeed() { return animationSpeed; }

    public final void setAnimationSpeed(double animationSpeed) {
        this.animationSpeed = animationSpeed;
        propertyChanged.emit();
    }

    @QtPropertyResetter(name="animationSpeed")
    public final void resetAnimationSpeed() {
        setAnimationSpeed(ANIMATION_SPEED);
    }

    private int tail[] = new int[TAIL_LENGTH];
    private void resetTail() {
        int newTail[] = new int[tailLength()];
        for (int i=0; i<(tail.length > newTail.length ? newTail.length : tail.length); ++i)
            newTail[i] = tail[i];
        tail = newTail;
    }

    //
    // tailLength property
    //
    private static final int TAIL_LENGTH = 10;
    private int tailLength = TAIL_LENGTH;

    @QtBlockedSlot public final int tailLength() { return tailLength; }

    public final void setTailLength(int tailLength) {
        if (tailLength < 0)
            return ;

        this.tailLength = tailLength;
        resetTail();
        propertyChanged.emit();
    }

    @QtPropertyResetter(name="tailLength")
    public final void resetTailLength() { setTailLength(TAIL_LENGTH); }

    //
    // currentSpeed property
    //
    private boolean isAnimating = false;
    private int startTail;
    private int endTail;
    private int oldSpeed;
    private int diff = 0;
    private int requestedSpeed = 0;
    private int currentSpeed = 0;

    @QtPropertyDesignable(value="false")
    @QtBlockedSlot public int currentSpeed() { return currentSpeed; }

    public void setCurrentSpeed(int currentSpeed) {
        oldSpeed = this.currentSpeed;
        requestedSpeed = currentSpeed;
        diff = requestedSpeed - oldSpeed;

        startTail = 0;
        endTail = 0;
        isAnimating = true;

        if (timeLine.state() == QTimeLine.State.Running)
            timeLine.stop();

        timeLine.setDuration((int)(Math.abs(diff) * animationSpeed()) + 1);
        timeLine.start();
    }

    //
    // skip property
    //
    private static final int SKIP = 20;
    private int skip = SKIP;

    @QtBlockedSlot public final int skip() {
        return skip;
    }

    public final void setSkip(int skip) { this.skip = skip; propertyChanged.emit(); }

    @QtPropertyResetter(name="skip")
    public final void resetSkip() { setSkip(SKIP); }

    //
    // unit property
    //
    private SpeedUnit unit = SpeedUnit.KilometersPerHour;

    @QtBlockedSlot public SpeedUnit unit() { return unit; }

    public void setUnit(SpeedUnit unit) { this.unit = unit; propertyChanged.emit(); }

    //
    // startAngle property
    //
    private static final double START_ANGLE = -30.0f;
    private double startAngle = START_ANGLE;

    @QtBlockedSlot public final double startAngle() { return startAngle; }

    public final void setStartAngle(double startAngle) { this.startAngle = startAngle; propertyChanged.emit(); }

    @QtPropertyResetter(name="startAngle")
    public final void resetStartAngle() { setStartAngle(START_ANGLE); }

    //
    // endAngle property
    //
    private static final double END_ANGLE = 210.0f;
    private double endAngle = END_ANGLE;

    @QtBlockedSlot public final double endAngle() { return endAngle; }

    public final void setEndAngle(double endAngle) { this.endAngle = endAngle; propertyChanged.emit(); }

    @QtPropertyResetter(name="endAngle")
    public final void resetEndAngle() { setEndAngle(endAngle); }

    //
    // font property
    //
    private QFont font = font();
    @QtBlockedSlot public final QFont speedFont() { return font; }

    public final void setSpeedFont(QFont font) { this.font = font; propertyChanged.emit(); }

    @QtPropertyResetter(name="speedFont")
    public final void resetSpeedFont() { setSpeedFont(font()); }

    //
    // highlightLight property
    //
    private static final QColor HIGHLIGHT_LIGHT = QColor.fromRgba(0xffffffff);
    private QColor highlightLight = HIGHLIGHT_LIGHT;

    @QtBlockedSlot public final QColor highlightLight() { return highlightLight; }

    public final void setHighlightLight(QColor color) { highlightLight = color; propertyChanged.emit(); }

    @QtPropertyResetter(name="highlightLight")
    public final void resetHighlightLight() { setHighlightLight(HIGHLIGHT_LIGHT); }

    //
    // highlightDark property
    //
    private static final QColor HIGHLIGHT_DARK = QColor.fromRgba(0x00ffffff);
    private QColor highlightDark = HIGHLIGHT_DARK;

    @QtBlockedSlot public final QColor highlightDark() { return highlightDark; }

    public final void setHighlightDark(QColor color) { highlightDark = color; propertyChanged.emit(); }

    @QtPropertyResetter(name="highlightDark")
    public final void resetHighlightDark() { setHighlightDark(HIGHLIGHT_DARK); }

    //
    // needleColor property
    //
    private static final QColor NEEDLE_COLOR = new QColor(org.qtjambi.qt.core.Qt.GlobalColor.red);
    private QColor needleColor = NEEDLE_COLOR;

    @QtBlockedSlot public final QColor needleColor() { return needleColor; }

    public final void setNeedleColor(QColor needleColor) { this.needleColor = needleColor; propertyChanged.emit(); }

    @QtPropertyResetter(name="needleColor")
    public final void resetNeedleColor() { setNeedleColor(NEEDLE_COLOR); }

    //
    // backgroundColor property
    //
    private static final QColor BACKGROUND_COLOR = new QColor(org.qtjambi.qt.core.Qt.GlobalColor.white);
    private QColor backgroundColor = BACKGROUND_COLOR;

    @QtBlockedSlot public final QColor backgroundColor() { return backgroundColor; }

    public final void setBackgroundColor(QColor backgroundColor) {
        this.backgroundColor = backgroundColor;
        propertyChanged.emit();
    }

    @QtPropertyResetter(name="backgroundColor")
    public final void resetBackgroundColor() { setBackgroundColor(BACKGROUND_COLOR); }

    //
    // needleFrameColor property
    //
    private static final QColor NEEDLE_FRAME_COLOR = new QColor(org.qtjambi.qt.core.Qt.GlobalColor.black);
    private QColor needleFrameColor = NEEDLE_FRAME_COLOR;

    @QtBlockedSlot public final QColor needleFrameColor() { return needleFrameColor; }

    public final void setNeedleFrameColor(QColor needleFrameColor) { this.needleFrameColor = needleFrameColor; propertyChanged.emit(); }

    @QtPropertyResetter(name="needleFrameColor")
    public final void resetNeedleFrameColor() { setNeedleFrameColor(NEEDLE_FRAME_COLOR); }

    //
    // frameColorDark property
    //
    private static final QColor FRAME_COLOR_DARK = QColor.fromRgb(0x00514f48);
    private QColor frameColorDark = FRAME_COLOR_DARK;

    @QtBlockedSlot public final QColor frameColorDark() { return frameColorDark; }

    public final void setFrameColorDark(QColor frameColorDark) { this.frameColorDark = frameColorDark; propertyChanged.emit(); }

    @QtPropertyResetter(name="frameColorDark")
    public final void resetFrameColorDark() { setFrameColorDark(FRAME_COLOR_DARK); }

    //
    // frameColorLight property
    //
    private static final QColor FRAME_COLOR_LIGHT = QColor.fromRgb(0x00a49a92);
    private QColor frameColorLight = FRAME_COLOR_LIGHT;

    @QtBlockedSlot public final QColor frameColorLight() { return frameColorLight; }

    public final void setFrameColorLight(QColor frameColorLight) { this.frameColorLight = frameColorLight; propertyChanged.emit(); }

    @QtPropertyResetter(name="frameColorLight")
    public final void resetFrameColorLight() { setFrameColorLight(FRAME_COLOR_LIGHT); }

    @Override
    protected void resizeEvent(QResizeEvent e) {
        this.rect = rect();
        x = rect.x();
        y = rect.y();
        w = rect.width();
        h = rect.height();

        resetAll();
    }

    private void resetAll() {
        resetLinearGradient();
        resetInnerCircle();
        resetOuterCircle();
        resetFramePath();
        resetRadialGradient();
        resetNeedlePath();
    }

    private QSize sizeHint = new QSize(200, 200);
    @Override
    public QSize sizeHint() {
        return sizeHint;
    }

    @Override
    protected void paintEvent(QPaintEvent e) {
        QPainter p = new QPainter();

        p.begin(this); {
            p.setRenderHint(QPainter.RenderHint.Antialiasing, true);

            drawFrame(p);
            drawPlate(p);

            p.end();
        }
    }

    private void resetLinearGradient() {
        gradient = null;
    }
    private QBrush linearGradient() {
        if (gradient == null) {
            QRectF rect = outerCircle();
            QLinearGradient g = new QLinearGradient(new QPointF(rect.x() + rect.width() / 2.0, rect.top()),
                                                    new QPointF(rect.x() + rect.width() / 2.0, rect.top() + framingSize() / 2.0));
            g.setColorAt(0.0, highlightLight());
            g.setColorAt(1.0, highlightDark());
            gradient = new QBrush(g);
        }
        return gradient;
    }

    private QRectF outerCircle() {
        if (outerCircle == null) {
            if (w > h)
                outerCircle = new QRectF(x + w/2 - h/2, y, h, h);
            else
                outerCircle = new QRectF(x, y + h/2 - w/2, w, w);
            // Compensate for the antialiased edges of the drawing by shrinking the rect by 1 pixel
            outerCircle = outerCircle.adjusted(1, 1, -1, -1);
        }
        return outerCircle;
    }
    private void resetOuterCircle() {
        outerCircle = null;
    }

    private QRectF innerCircle() {
        if (innerCircle == null) {
            QRectF outerCircle = outerCircle();
            innerCircle = new QRectF(outerCircle.x() + framingSize(), outerCircle.y() + framingSize(),
                                     outerCircle.width() - framingSize()*2,
                                     outerCircle.height() - framingSize()*2);
        }

        return innerCircle;
    }
    private QPainterPath innerCirclePath() {
        if (innerCirclePath == null) {
            innerCirclePath = new QPainterPath();
            innerCirclePath.addEllipse(innerCircle());
        }

        return innerCirclePath;
    }
    private void resetInnerCircle() {
        innerCircle = null;
        innerCirclePath = null;
    }

    private QPainterPath needlePath() {
        if (needlePath == null) {
            QRectF innerCircle = innerCircle();
            needlePath = new QPainterPath();
            needlePath.moveTo(0.0, 0.0);

            double tipLength = (double)innerCircle.width() / 50.0f;
            needlePath.lineTo(-innerCircle.width() / 2.0 + tipLength, -tipLength);
            needlePath.lineTo(-innerCircle.width() / 2.0, 0.0);
            needlePath.lineTo(-innerCircle.width() / 2.0 + tipLength, tipLength);
            needlePath.lineTo(0.0, 0.0);
        }

        return needlePath;
    }
    private void resetNeedlePath() {
        needlePath = null;
    }

    private QRectF cachedTextRect = new QRectF();
    private void drawPlate(QPainter p) {
        p.save();

        QPainterPath path = innerCirclePath();
        p.fillPath(path, new QBrush(backgroundColor()));
        p.setClipPath(path);

        QRectF innerCircle = innerCircle();
        double x = (double)innerCircle.x() + (double)innerCircle.width() / 2.0f;
        double y = (double)innerCircle.y() + (double)innerCircle.height() / 2.0f;

        // Draw the speed bars
        int skip = skip();
        double step = ((double)skip / (double)maxSpeed()) * (endAngle() - startAngle());
        QFont font = speedFont();
        p.setFont(font);

        QFontMetrics fm = new QFontMetrics(font);
        {
            if (step > 0.0) {
                p.save();
                p.translate(x,y);

                speedBarPen.setWidthF(innerCircle.width() / 100.0f);
                p.setPen(speedBarPen);

                double angle;
                int speed = 0;
                p.rotate(startAngle());
                for (angle=startAngle(); angle<=endAngle(); angle += step) {
                    p.drawLine((int)(-innerCircle.width() / 2.0f) + (int)(innerCircle.width() / 30.0f), 0, (int)(-innerCircle.width() / 2.0f) + (int)(innerCircle.width() / 14.0f), 0);

                    String speedString = "" + speed;
                    QSize size = fm.size(0, speedString);
                    int x1 = (int)(-innerCircle.width() / 2.0f) + (int)(innerCircle.width() / 14.0f);
                    cachedTextRect.setCoords(x1, 0, x1 + size.width(), size.height());

                    p.drawText(cachedTextRect, "" + speed);

                    speed += skip;
                    p.rotate(step);
                }
                p.restore();
            }
        }


        p.translate(x, y);
        p.setFont(font);
        QSize size = fm.size(0, unit().title());
        cachedTextRect.setCoords(-size.width() / 2.0, 0, size.width() / 2.0, size.height());
        p.drawText(cachedTextRect, unit().title());

        p.restore();

        p.setPen(new QPen(Qt.PenStyle.NoPen));
        p.setBrush(new QBrush(needleColor()));

        int i=startTail;
        while (i != endTail) {
            p.save();

            matrix.reset();
            matrix.translate(x, y);
            matrix.rotate(angleOfSpeed(tail[i]));

            int len = i >= startTail ? i - startTail : i + tail.length - startTail;
            p.setOpacity((needleColor().alphaF() / (tail.length+1)) * len);
            QPainterPath temporaryPath = matrix.map(needlePath());
            p.drawPath(temporaryPath);

            if ((++i) >= tail.length)
                i = 0;
            p.restore();
        }

        if (!isAnimating && startTail != endTail) {
            if ((++startTail) >= tail.length) {
                startTail = 0;
            }
            update();
        }

        matrix.reset();
        matrix.translate(x, y);
        matrix.rotate(angleOfSpeed(currentSpeed));

        p.setPen(needleFrameColor());
        QPainterPath temporaryPath = matrix.map(needlePath());
        p.drawPath(temporaryPath);
    }

    private void drawHighlights(QPainter p) {
        p.setPen(new QPen(Qt.PenStyle.NoPen));
        p.setBrush(linearGradient());
        p.setClipPath(framePath());
        p.drawEllipse(outerCircle());
    }

    private QPainterPath framePath() {
        if (framePath == null) {
            framePath = new QPainterPath();
            framePath.addEllipse(outerCircle());
            {
                QPainterPath innerEllipse = new QPainterPath();
                innerEllipse.addEllipse(innerCircle());
                framePath = framePath.subtracted(innerEllipse);
            }
        }

        return framePath;
    }
    private void resetFramePath() {
        framePath = null;
    }

    private QBrush radialGradient() {
        if (radialGradient == null) {
            QRadialGradient g = new QRadialGradient(new QPointF(rect.center()), rect.width() / 2.0);
            g.setColorAt(0.0, frameColorDark());
            g.setColorAt(1.0, frameColorLight());
            g.setCoordinateMode(QGradient.CoordinateMode.ObjectBoundingMode);
            radialGradient = new QBrush(g);
        }

        return radialGradient;
    }
    private void resetRadialGradient() {
        radialGradient = null;
    }

    private void drawFrame(QPainter p) {
        p.save();

        p.setPen(new QPen(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.black), 2));
        p.setBrush(radialGradient());
        p.drawPath(framePath());

        drawHighlights(p);
        p.restore();
    }

    private double angleOfSpeed(int speed) {
        int skip = skip();
        double step = ((double)skip / (double)maxSpeed()) * (endAngle() - startAngle());
        return startAngle() + step * ((double)speed / (double)skip);
    }

    private void noLongerAnimating() {
        isAnimating = false;
    }

    private QTransform matrix = new QTransform();

    private void animateSpeed(double value) {
        if (tail.length > 0) {
            tail[endTail++] = currentSpeed;
            if (endTail >= tail.length)
                endTail = 0;

            if (endTail <= startTail) {
                startTail = endTail + 1;
                if (startTail >= tail.length)
                    startTail = 0;
            }
        }

        currentSpeed = (int)(oldSpeed + diff * value);

        propertyChanged.emit();
        speedChanged.emit(currentSpeed);
    }


}
