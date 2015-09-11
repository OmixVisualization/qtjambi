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
package org.qtjambi.extensions.awt;

import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.awt.*;
import java.awt.font.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.image.renderable.*;
import java.text.*;
import java.util.*;

public class QPainterGraphics extends Graphics2D {

    private static final boolean DEBUG_STATE = true;
    private static final boolean DEBUG_PAINT = true;
    private static final boolean DEBUG_PATH_CONVERT = true;

    private static final int PAINTER_STATE_PEN      = 0x0001;
    private static final int PAINTER_STATE_BRUSH    = 0x0002;
    private static final int PAINTER_STATE_MASK     = 0x0003;

    private static final int PAINT_MODE_COLOR       = 0x0010;
    private static final int PAINT_MODE_GRADIENT    = 0x0020;
    private static final int PAINT_MODE_MASK        = 0x0030;

    public QPainterGraphics(QPainter painter) {
        this.painter = painter;
    }

    public void draw(Shape s) {
        prepareDraw();
        QPainterPath path = shapeToQPainterPath(s);

        if (DEBUG_PAINT) {
            QRectF bounds = path.boundingRect();
            System.out.printf("(paint) draw() bounds: [%f, %f], width=%f, height=%f\n",
                    bounds.x(), bounds.y(), bounds.width(), bounds.height());
        }

        painter.drawPath(path);
    }

    public void fill(Shape s) {
        prepareFill();
        QPainterPath path = shapeToQPainterPath(s);
        if (DEBUG_PAINT) {
            QRectF bounds = path.boundingRect();
            System.out.printf("(paint) fill() bounds: [%f, %f], width=%f, height=%f\n",
                    bounds.x(), bounds.y(), bounds.width(), bounds.height());
        }
        painter.drawPath(path);
    }

    public boolean drawImage(Image img, AffineTransform xform, ImageObserver obs) {
        return false;
    }

    public void drawImage(BufferedImage img, BufferedImageOp op, int x, int y) {
    }

    public void drawRenderedImage(RenderedImage img, AffineTransform xform) {
    }

    public void drawString(String str, int x, int y) {
        painter.drawText(x, y, str);
    }

    public void drawString(String str, float x, float y) {
        painter.drawText(new QPointF(x, y), str);
    }

    public void drawString(AttributedCharacterIterator iterator, int x, int y) {

    }

    public void drawString(AttributedCharacterIterator iterator, float x, float y) {
    }

    public void drawGlyphVector(GlyphVector g, float x, float y) {
    }

    public boolean hit(Rectangle rect, Shape s, boolean onStroke) {
        return false;
    }

    public GraphicsConfiguration getDeviceConfiguration() {
        return null;
    }

    public void setComposite(Composite comp) {

    }

    public void setPaint(Paint paint) {
        this.paint = paint;

        int newState = 0;

        if (paint instanceof GradientPaint) {
            GradientPaint g = (GradientPaint) paint;
            newState = PAINT_MODE_GRADIENT;

            Point2D p1 = g.getPoint1();
            Point2D p2 = g.getPoint2();
            gradient = new QLinearGradient(p1.getX(), p1.getY(), p2.getX(), p2.getY());
            gradient.setColorAt(0, QColor.fromRgb(g.getColor1().getRGB()));
            gradient.setColorAt(1, QColor.fromRgb(g.getColor2().getRGB()));

        } else {

            newState = 0;
        }

        updateState(PAINT_MODE_MASK, newState);
    }

    public void setStroke(Stroke s) {
    }

    public void setRenderingHint(RenderingHints.Key hintKey, Object hintValue) {
        if (hintKey == RenderingHints.KEY_ANTIALIASING) {
            painter.setRenderHint(QPainter.RenderHint.Antialiasing,
                    hintValue == RenderingHints.VALUE_ANTIALIAS_ON);
        } else if (hintKey == RenderingHints.KEY_INTERPOLATION) {
            painter.setRenderHint(QPainter.RenderHint.SmoothPixmapTransform,
                    hintValue == RenderingHints.VALUE_INTERPOLATION_BILINEAR
                    || hintValue == RenderingHints.VALUE_INTERPOLATION_BICUBIC);
        }
    }

    public Object getRenderingHint(RenderingHints.Key hintKey) {
        return null;
    }

    public void setRenderingHints(Map<?, ?> hints) {
    }

    public void addRenderingHints(Map<?, ?> hints) {
    }

    public RenderingHints getRenderingHints() {
        return null;
    }

    public void translate(int x, int y) {
        painter.translate(x, y);
    }

    public void translate(double tx, double ty) {
        painter.translate(tx, ty);
    }

    public void rotate(double theta) {
        painter.rotate(theta);
    }

    public void rotate(double theta, double x, double y) {
        painter.translate(x, y);
        painter.rotate(theta);
        painter.translate(-x, -y);
    }

    public void scale(double sx, double sy) {
        painter.scale(sx, sy);
    }

    public void shear(double shx, double shy) {
        painter.shear(shx, shy);
    }

    public void transform(AffineTransform Tx) {

    }

    public void setTransform(AffineTransform Tx) {

    }

    public AffineTransform getTransform() {
        return null;
    }

    public Paint getPaint() {
        return null;
    }

    public Composite getComposite() {
        return null;
    }

    public void setBackground(Color color) {
    }

    public Color getBackground() {
        return null;
    }

    public Stroke getStroke() {
        return null;
    }

    public void clip(Shape s) {
    }

    public FontRenderContext getFontRenderContext() {
        return null;
    }

    public Graphics create() {
        return null;
    }

    public Color getColor() {
        return null;
    }

    public void setColor(Color c) {
        color.setRgb(c.getRed(), c.getGreen(), c.getBlue(), c.getAlpha());
        updateState(PAINT_MODE_MASK, PAINT_MODE_COLOR);
    }

    public void setPaintMode() {
    }

    public void setXORMode(Color c1) {
    }

    public Font getFont() {
        return null;
    }

    public void setFont(Font font) {
    }

    public FontMetrics getFontMetrics(Font f) {
        return null;
    }

    public Rectangle getClipBounds() {
        return null;
    }

    public void clipRect(int x, int y, int width, int height) {
        painter.setClipRect(x, y, width, height, Qt.ClipOperation.ReplaceClip);
    }

    public void setClip(int x, int y, int width, int height) {
        painter.setClipRect(x, y, width, height, Qt.ClipOperation.ReplaceClip);
    }

    public Shape getClip() {
        return null;
    }

    public void setClip(Shape clip) {
        QPainterPath path = shapeToQPainterPath(clip);
        painter.setClipPath(path);
    }

    public void copyArea(int x, int y, int width, int height, int dx, int dy) {
    }

    public void drawLine(int x1, int y1, int x2, int y2) {
        prepareDraw();
        if (DEBUG_PAINT) System.out.printf("(paint) drawLine: [%d, %d] [%d, %d]\n", x1, y1, x2, y2);
        painter.drawLine(x1, y1, x2, y2);
    }

    public void fillRect(int x, int y, int width, int height) {
        prepareFill();
        if (DEBUG_PAINT) System.out.printf("(paint) filling rect: [%d, %d] width=%d, height=%d\n", x, y, width, height);
        painter.drawRect(x, y, width, height);
    }

    public void clearRect(int x, int y, int width, int height) {

    }

    public void drawRoundRect(int x, int y, int width, int height, int arcWidth, int arcHeight) {
    }

    public void fillRoundRect(int x, int y, int width, int height, int arcWidth, int arcHeight) {
    }

    public void drawOval(int x, int y, int width, int height) {
        prepareDraw();
        painter.drawEllipse(x, y, width, height);
    }

    public void fillOval(int x, int y, int width, int height) {
        prepareFill();
        painter.drawEllipse(x, y, width, height);
    }

    public void drawArc(int x, int y, int width, int height, int startAngle, int arcAngle) {
        prepareDraw();
        painter.drawArc(x, y, width, height, startAngle * 16, arcAngle * 16);
    }

    public void fillArc(int x, int y, int width, int height, int startAngle, int arcAngle) {
        prepareFill();
        painter.drawArc(x, y, width, height, startAngle * 16, arcAngle * 16);
    }

    public void drawPolyline(int xPoints[], int yPoints[], int nPoints) {
        prepareDraw();
        QPolygon polygon = pointsToQPolygon(xPoints, yPoints, nPoints);
        painter.drawPolyline(polygon);
    }

    public void drawPolygon(int xPoints[], int yPoints[], int nPoints) {
        prepareDraw();
        QPolygon polygon = pointsToQPolygon(xPoints, yPoints, nPoints);
        painter.drawPolygon(polygon);
    }

    public void fillPolygon(int xPoints[], int yPoints[], int nPoints) {
        prepareFill();
        QPolygon polygon = pointsToQPolygon(xPoints, yPoints, nPoints);
        painter.drawPolygon(polygon);
    }

    public boolean drawImage(Image img, int x, int y, ImageObserver observer) {
        return false;
    }

    public boolean drawImage(Image img, int x, int y, int width, int height, ImageObserver observer) {
        return false;
    }

    public boolean drawImage(Image img, int x, int y, Color bgcolor, ImageObserver observer) {
        return false;
    }

    public boolean drawImage(Image img, int x, int y, int width, int height, Color bgcolor, ImageObserver observer) {
        return false;
    }

    public boolean drawImage(Image img, int dx1, int dy1, int dx2, int dy2, int sx1, int sy1, int sx2, int sy2, ImageObserver observer) {
        return false;
    }

    public boolean drawImage(Image img, int dx1, int dy1, int dx2, int dy2, int sx1, int sy1, int sx2, int sy2, Color bgcolor, ImageObserver observer) {
        return false;
    }

    public void dispose() {
    }

    public void drawRenderableImage(RenderableImage img, AffineTransform xform) {
    }

    private QPainterPath shapeToQPainterPath(Shape s) {
        PathIterator it = s.getPathIterator(null);
        QPainterPath path = new QPainterPath();

        double pts[] = new double[6];

        if (DEBUG_PATH_CONVERT) System.out.println("shape: " + s.getBounds() + ", done=" + it.isDone());

        while (!it.isDone()) {
            switch (it.currentSegment(pts)) {
                case PathIterator.SEG_MOVETO:
                    if (DEBUG_PATH_CONVERT) System.out.println(" -> moveTo(" + pts[0] + ", " + pts[1] + ")");
                    path.moveTo(pts[0], pts[1]);
                    break;
                case PathIterator.SEG_LINETO:
                    if (DEBUG_PATH_CONVERT) System.out.println(" -> lineTo(" + pts[0] + ", " + pts[1] + ")");
                    path.lineTo(pts[0], pts[1]);
                    break;
                case PathIterator.SEG_CUBICTO:
                    if (DEBUG_PATH_CONVERT) System.out.println(" -> cubicTo(" + pts[0] + ", " + pts[1] + " - " + pts[2] + ", " + pts[3] + " - " + pts[4] + ", " + pts[5] + ")");
                    path.cubicTo(pts[0], pts[1], pts[2], pts[3], pts[4], pts[5]);
                    break;
                case PathIterator.SEG_QUADTO:
                    if (DEBUG_PATH_CONVERT) System.out.println(" -> quadTo(" + pts[0] + ", " + pts[1] + " - " + pts[2] + ", " + pts[3] + ")");
                    path.quadTo(pts[0], pts[1], pts[2], pts[3]);
                    break;
                case PathIterator.SEG_CLOSE:
                    if (DEBUG_PATH_CONVERT) System.out.println(" -> close");
                    path.closeSubpath();
                    break;
            }
            it.next();
        }

        if (it.getWindingRule() == PathIterator.WIND_NON_ZERO)
            path.setFillRule(Qt.FillRule.WindingFill);

        return path;
    }

    private QPolygon pointsToQPolygon(int[] xPoints, int[] yPoints, int nPoints) {
        QPolygon polygon = new QPolygon();
        for (int i=0; i<nPoints; ++i)
            polygon.add(xPoints[i], yPoints[i]);
        return polygon;
    }

    private void prepareDraw() {
        if ((state & PAINTER_STATE_MASK) != PAINTER_STATE_PEN) {

            penBrush = prepareBrush(penBrush);
            pen.setBrush(penBrush);

            painter.setBrush(QBrush.NoBrush);
            painter.setPen(pen);
            updateState(PAINTER_STATE_MASK, PAINTER_STATE_PEN);
        }
    }

    private void prepareFill() {
        System.out.printf("prepareFill(): %x\n", state);
        if ((state & PAINTER_STATE_MASK) != PAINTER_STATE_BRUSH) {
            brush = prepareBrush(brush);

            painter.setPen(QPen.NoPen);
            painter.setBrush(brush);
            updateState(PAINTER_STATE_MASK, PAINTER_STATE_BRUSH);
        }
    }

    private QBrush prepareBrush(QBrush brush) {
        switch (state & PAINT_MODE_MASK) {
            case PAINT_MODE_COLOR:
                if (DEBUG_STATE) System.out.printf("(state) set brush to color: %x\n", color.rgba());
                brush.setColor(color);
                brush.setStyle(Qt.BrushStyle.SolidPattern);
                break;
            case PAINT_MODE_GRADIENT:
                if (DEBUG_STATE) System.out.printf("(state) set brush to gradient\n");
                brush = new QBrush(gradient);
                break;
        }

        return brush;
    }

    private void updateState(int clear, int set) {
        state &= ~clear;
        state |= set;
    }

    private QPainter painter;
    private QBrush brush = new QBrush();
    private QPen pen = new QPen();
    private QBrush penBrush = new QBrush();
    private QColor color = new QColor();
    private QGradient gradient;
    private int state;

    private Paint paint;

}
