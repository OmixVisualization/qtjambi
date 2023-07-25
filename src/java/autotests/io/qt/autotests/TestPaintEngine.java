/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
package io.qt.autotests;

import static org.junit.Assert.assertEquals;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.NonNull;
import io.qt.Nullable;
import io.qt.StrictNonNull;
import io.qt.autotests.generated.OtherCustomPaintEngine;
import io.qt.core.QLine;
import io.qt.core.QLineF;
import io.qt.core.QPoint;
import io.qt.core.QPointF;
import io.qt.core.QRect;
import io.qt.core.QRectF;
import io.qt.gui.QFont;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QPaintDevice;
import io.qt.gui.QPaintEngine;
import io.qt.gui.QPaintEngineState;
import io.qt.gui.QPainter;
import io.qt.gui.QPixmap;

public class TestPaintEngine extends ApplicationInitializer {
	
    @BeforeClass
    public static void testInitialize() throws Exception {
        ApplicationInitializer.testInitializeWithWidgets();
		Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
	}
    
    public static class UpdatingPaintEngine extends QPaintEngine{
    	
    	private final QFont[] fontResult;

		public UpdatingPaintEngine(QFont[] fontResult) {
			super();
			this.fontResult = fontResult;
		}

		@Override
		public boolean begin(@Nullable QPaintDevice pdev) {
			return true;
		}

		@Override
		public void drawPixmap(@NonNull QRectF r, @NonNull QPixmap pm, @NonNull QRectF sr) {
		}

		@Override
		public boolean end() {
			return true;
		}

		@Override
		public @NonNull Type type() {
			return Type.Raster;
		}

		@Override
		public void updateState(@StrictNonNull QPaintEngineState state) {
			if(state.state().testFlag(QPaintEngine.DirtyFlag.DirtyFont)) {
				fontResult[0] = state.font();
			}
		}

		@Override
		public void drawPolygon(@NonNull QPointF[] points, @NonNull PolygonDrawMode mode) {
		}
    }
    
    @Test
    public void testPaintEngineState() {
    	QFont f = new QFont("Arial", 56);
    	QFont[] fontResult = {null};
    	QPaintDevice device = new QPaintDevice() {
    		final UpdatingPaintEngine engine = new UpdatingPaintEngine(fontResult);
    		@Override
			public @Nullable QPaintEngine paintEngine() {
				return engine;
			}
		};
    	QPainter painter = new QPainter(device);
    	painter.setFont(f);
    	painter.drawRect(1, 2, 3, 4);
    	painter.end();
    	Assert.assertEquals(f.toString(), ""+fontResult[0]);
    }
    
 // For ARM float/double conversion values must be within precision of a float
    private static final double DELTA = 0.000001;

    public static class OverrideCustomPaintEngine extends io.qt.autotests.generated.CustomPaintEngine {
        private int length = 0;
        private Object object = null;

        void setArray(Object array[]) {
            if (array == null) {
                length = -1;
                object = null;
            } else {
                length = array.length;
                object = array.length > 0 ? array[0] : null;
            }
        }
        
        @Override
        public boolean begin(QPaintDevice pdev) {
            return true;
        }

        @Override
        public void drawPixmap(QRectF r, QPixmap pm, QRectF sr) {
        }

        @Override
        public boolean end() {
            return true;
        }

        @Override
        public Type type() {
            return QPaintEngine.Type.resolve(QPaintEngine.Type.User.value() + 1, this.getClass().getSimpleName());
        }

        @Override
        public void updateState(QPaintEngineState state) {

        }

        /**
         * drawLines(const QLine *, int)
         */

        @Override
        public void drawLines(QLine... lines) {
            setArray(lines);
        }

        /**
         * drawLines(const QLineF *, int)
         **/

        @Override
        public void drawLines(QLineF... lines) {
            setArray(lines);
        }

        /**
         * drawRects(const QRectF *, int)
         **/

        @Override
        public void drawRects(QRectF... lines) {
            setArray(lines);
        }

        /**
         * drawRects(const QRect *, int)
         **/

        @Override
        public void drawRects(QRect... lines) {
            setArray(lines);
        }

        /**
         * drawPoints(const QPoint *, int)
         **/

        @Override
        public void drawPoints(QPoint... lines) {
            setArray(lines);
        }

        /**
         * drawPoints(const QPointF *, int)
         **/

        @Override
        public void drawPoints(QPointF... lines) {
            setArray(lines);
        }

        /**
         * drawPolygon(const QPointF *, int)
         **/

        @Override
        public void drawPolygon(QPointF lines[], QPaintEngine.PolygonDrawMode mode) {
            setArray(lines);
        }

        /**
         * drawPolygon(const QPoint *, int)
         **/

        @Override
        public void drawPolygon(QPoint lines[], QPaintEngine.PolygonDrawMode mode) {
            setArray(lines);
        }
    }

    @Test
    public void testVirtualDrawLines() {
    	OverrideCustomPaintEngine engine = new OverrideCustomPaintEngine();
        engine.callDrawLines(new QLine(1, 2, 3, 4), 11);

        assertEquals(11, engine.length);

        QLine line = (QLine) engine.object;
        assertEquals(1, line.x1());
        assertEquals(2, line.y1());
        assertEquals(3, line.x2());
        assertEquals(4, line.y2());
    }

    @Test
    public void testVirtualDrawLinesNull() {
    	OverrideCustomPaintEngine engine = new OverrideCustomPaintEngine();
        engine.callDrawLines(null, 0);

        assertEquals(0, engine.length);
        assertEquals(null, engine.object);
    }

    @Test
    public void testDrawLines() {
        OtherCustomPaintEngine p = new OtherCustomPaintEngine();

        QLine lines[] = new QLine[13];
        lines[0] = new QLine(2, 3, 4, 5);
        for (int i=1; i<lines.length; ++i)
            lines[i] = new QLine();

        p.drawLines(lines);

        assertEquals(13, p.length());

        QLine line = p.line();
        assertEquals(2, line.x1());
        assertEquals(3, line.y1());
        assertEquals(4, line.x2());
        assertEquals(5, line.y2());
    }

    @Test
    public void testDrawLinesNull() {
        OtherCustomPaintEngine p = new OtherCustomPaintEngine();

        p.drawLines((QLine[]) null);

        assertEquals(-1, p.length());
    }

    @Test
    public void testVirtualDrawLineFs() {
    	OverrideCustomPaintEngine engine = new OverrideCustomPaintEngine();
        engine.callDrawLineFs(new QLineF(1.2, 2.3, 3.4, 4.5), 11);

        assertEquals(11, engine.length);

        QLineF line = (QLineF) engine.object;
        assertEquals(1.2, line.x1(), DELTA);	// ARM expected:<1.2> but was:<1.2000000476837158>
        assertEquals(2.3, line.y1(), DELTA);	// ARM expected:<2.3> but was:<2.299999952316284>
        assertEquals(3.4, line.x2(), DELTA);	// ARM expected:<3.4> but was:<3.4000000953674316>
        assertEquals(4.5, line.y2(), 0.0);
    }

    @Test
    public void testVirtualDrawLineFsNull() {
    	OverrideCustomPaintEngine engine = new OverrideCustomPaintEngine();
        engine.callDrawLineFs(null, 0);

        assertEquals(0, engine.length);
        assertEquals(null, engine.object);
    }

    @Test
    public void testDrawLineFs() {
        OtherCustomPaintEngine p = new OtherCustomPaintEngine();

        QLineF lines[] = new QLineF[13];
        lines[0] = new QLineF(2.3, 3.4, 4.5, 5.6);
        for (int i=1; i<lines.length; ++i)
            lines[i] = new QLineF();

        p.drawLines(lines);

        assertEquals(13, p.length());

        QLineF line = p.lineF();
        assertEquals(2.3, line.x1(), DELTA);	// ARM expected:<2.3> but was:<2.299999952316284>
        assertEquals(3.4, line.y1(), DELTA);	// ARM expected:<3.4> but was:<3.4000000953674316>
        assertEquals(4.5, line.x2(), 0.0);
        assertEquals(5.6, line.y2(), DELTA);	// ARM expected:<5.6> but was:<5.599999904632568>
    }

    @Test
    public void testDrawLineFsNull() {
        OtherCustomPaintEngine p = new OtherCustomPaintEngine();

        p.drawLines((QLineF[]) null);

        assertEquals(-1, p.length());
    }

    @Test
    public void testVirtualDrawRectFs() {
    	OverrideCustomPaintEngine engine = new OverrideCustomPaintEngine();
        engine.callDrawRectFs(new QRectF(1.2, 2.3, 3.4, 4.5), 11);

        assertEquals(11, engine.length);

        QRectF rect = (QRectF) engine.object;
        assertEquals(1.2, rect.x(), DELTA);	// ARM expected:<1.2> but was:<1.2000000476837158>
        assertEquals(2.3, rect.y(), DELTA);	// ARM expected:<2.3> but was:<2.299999952316284>
        assertEquals(3.4, rect.width(), DELTA);	// ARM expected:<3.4> but was:<3.4000000953674316>
        assertEquals(4.5, rect.height(), 0.0);
    }

    @Test
    public void testVirtualDrawRectFsNull() {
    	OverrideCustomPaintEngine engine = new OverrideCustomPaintEngine();
        engine.callDrawRectFs(null, 0);

        assertEquals(0, engine.length);
        assertEquals(null, engine.object);
    }

    @Test
    public void testDrawRectFs() {
        OtherCustomPaintEngine p = new OtherCustomPaintEngine();

        QRectF rects[] = new QRectF[13];
        rects[0] = new QRectF(2.3, 3.4, 4.5, 5.6);
        for (int i=1; i<rects.length; ++i)
            rects[i] = new QRectF();

        p.drawRects(rects);

        assertEquals(13, p.length());

        QRectF rect = p.rectF();
        assertEquals(2.3, rect.x(), DELTA);	// ARM expected:<2.3> but was:<2.299999952316284>
        assertEquals(3.4, rect.y(), DELTA);	// ARM expected:<3.4> but was:<3.4000000953674316>
        assertEquals(4.5, rect.width(), 0.0);
        assertEquals(5.6, rect.height(), DELTA);// ARM expected:<5.6> but was:<5.599999904632568>
    }

    @Test
    public void testDrawRectFsNull() {
        OtherCustomPaintEngine p = new OtherCustomPaintEngine();

        p.drawRects((QRectF[]) null);

        assertEquals(-1, p.length());
    }

    @Test
    public void testVirtualDrawRects() {
    	OverrideCustomPaintEngine engine = new OverrideCustomPaintEngine();
        engine.callDrawRects(new QRect(1, 2, 3, 4), 11);

        assertEquals(11, engine.length);

        QRect rect = (QRect) engine.object;
        assertEquals(1, rect.x());
        assertEquals(2, rect.y());
        assertEquals(3, rect.width());
        assertEquals(4, rect.height());
    }

    @Test
    public void testVirtualDrawRectsNull() {
    	OverrideCustomPaintEngine engine = new OverrideCustomPaintEngine();
        engine.callDrawRects(null, 0);

        assertEquals(0, engine.length);
        assertEquals(null, engine.object);
    }

    @Test
    public void testDrawRects() {
        OtherCustomPaintEngine p = new OtherCustomPaintEngine();

        QRect rects[] = new QRect[13];
        rects[0] = new QRect(2, 3, 4, 5);
        for (int i=1; i<rects.length; ++i)
            rects[i] = new QRect();

        p.drawRects(rects);

        assertEquals(13, p.length());

        QRect rect = p.rect();
        assertEquals(2, rect.x());
        assertEquals(3, rect.y());
        assertEquals(4, rect.width());
        assertEquals(5, rect.height());
    }

    @Test
    public void testDrawRectsNull() {
        OtherCustomPaintEngine p = new OtherCustomPaintEngine();

        p.drawRects((QRect[]) null);

        assertEquals(-1, p.length());
    }

    @Test
    public void testVirtualDrawPoints() {
    	OverrideCustomPaintEngine engine = new OverrideCustomPaintEngine();
        engine.callDrawPoints(new QPoint(1, 2), 11);

        assertEquals(11, engine.length);

        QPoint point = (QPoint) engine.object;
        assertEquals(1, point.x());
        assertEquals(2, point.y());
    }

    @Test
    public void testVirtualDrawPointsNull() {
    	OverrideCustomPaintEngine engine = new OverrideCustomPaintEngine();
        engine.callDrawPoints(null, 0);

        assertEquals(0, engine.length);
        assertEquals(null, engine.object);
    }

    @Test
    public void testDrawPoints() {
        OtherCustomPaintEngine p = new OtherCustomPaintEngine();

        QPoint points[] = new QPoint[13];
        points[0] = new QPoint(2, 3);
        for (int i=1; i<points.length; ++i)
            points[i] = new QPoint();

        p.drawPoints(points);

        assertEquals(13, p.length());

        QPoint point = p.point();
        assertEquals(2, point.x());
        assertEquals(3, point.y());
    }

    @Test
    public void testDrawPointsNull() {
        OtherCustomPaintEngine p = new OtherCustomPaintEngine();

        p.drawPoints((QPoint[]) null);

        assertEquals(-1, p.length());
    }

    @Test
    public void testVirtualDrawPointFs() {
    	OverrideCustomPaintEngine engine = new OverrideCustomPaintEngine();
        engine.callDrawPointFs(new QPointF(1.0, 2.0), 11);

        assertEquals(11, engine.length);

        QPointF pointF = (QPointF) engine.object;
        assertEquals(1.0, pointF.x(), 0.0);
        assertEquals(2.0, pointF.y(), 0.0);
    }

    @Test
    public void testVirtualDrawPointFsNull() {
    	OverrideCustomPaintEngine engine = new OverrideCustomPaintEngine();
        engine.callDrawPointFs(null, 0);

        assertEquals(0, engine.length);
        assertEquals(null, engine.object);
    }

    @Test
    public void testDrawPointFs() {
        OtherCustomPaintEngine p = new OtherCustomPaintEngine();

        QPointF pointFs[] = new QPointF[13];
        pointFs[0] = new QPointF(2.0, 3.0);
        for (int i=1; i<pointFs.length; ++i)
            pointFs[i] = new QPointF();

        p.drawPoints(pointFs);

        assertEquals(13, p.length());

        QPointF pointF = p.pointF();
        assertEquals(2.0, pointF.x(), 0.0);
        assertEquals(3.0, pointF.y(), 0.0);
    }

    @Test
    public void testDrawPointFsNull() {
        OtherCustomPaintEngine p = new OtherCustomPaintEngine();

        p.drawPoints((QPointF[]) null);

        assertEquals(-1, p.length());
    }

    @Test
    public void testVirtualDrawPolygonFs() {
    	OverrideCustomPaintEngine engine = new OverrideCustomPaintEngine();
        engine.callDrawPolygonFs(new QPointF(1.0, 2.0), 11);

        assertEquals(11, engine.length);

        QPointF pointF = (QPointF) engine.object;
        assertEquals(1.0, pointF.x(), 0.0);
        assertEquals(2.0, pointF.y(), 0.0);
    }

    @Test
    public void testVirtualDrawPolygonFsNull() {
    	OverrideCustomPaintEngine engine = new OverrideCustomPaintEngine();
        engine.callDrawPolygonFs(null, 0);

        assertEquals(0, engine.length);
        assertEquals(null, engine.object);
    }

    @Test
    public void testDrawPolygonFs() {
        OtherCustomPaintEngine p = new OtherCustomPaintEngine();

        QPointF pointFs[] = new QPointF[13];
        pointFs[0] = new QPointF(2.0, 3.0);
        for (int i=1; i<pointFs.length; ++i)
            pointFs[i] = new QPointF();

        p.drawPolygon(pointFs, QPaintEngine.PolygonDrawMode.OddEvenMode);

        assertEquals(13, p.length());

        QPointF pointF = p.pointF();
        assertEquals(2.0, pointF.x(), 0.0);
        assertEquals(3.0, pointF.y(), 0.0);
    }

    @Test
    public void testVirtualDrawPolygons() {
    	OverrideCustomPaintEngine engine = new OverrideCustomPaintEngine();
        engine.callDrawPolygons(new QPoint(1, 2), 11);

        assertEquals(11, engine.length);

        QPoint point = (QPoint) engine.object;
        assertEquals(1, point.x());
        assertEquals(2, point.y());
    }

    @Test
    public void testVirtualDrawPolygonsNull() {
    	OverrideCustomPaintEngine engine = new OverrideCustomPaintEngine();
        engine.callDrawPolygons(null, 0);

        assertEquals(0, engine.length);
        assertEquals(null, engine.object);
    }

    @Test
    public void testDrawPolygons() {
        OtherCustomPaintEngine p = new OtherCustomPaintEngine();

        QPoint points[] = new QPoint[13];
        points[0] = new QPoint(2, 3);
        for (int i=1; i<points.length; ++i)
            points[i] = new QPoint();

        p.drawPolygon(points, QPaintEngine.PolygonDrawMode.OddEvenMode);

        assertEquals(13, p.length());

        QPoint point = p.point();
        assertEquals(2, point.x());
        assertEquals(3, point.y());
    }

    @Test
    public void testDrawPolygonsNull() {
        OtherCustomPaintEngine p = new OtherCustomPaintEngine();

        p.drawPolygon((QPoint[]) null, QPaintEngine.PolygonDrawMode.OddEvenMode);

        assertEquals(-1, p.length());
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestPaintEngine.class.getName());
    }
}
