/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import static io.qt.core.QtGlobal.*;

import org.junit.*;

import io.qt.canvaspainter.*;
import io.qt.core.*;
import io.qt.gui.*;
import io.qt.widgets.*;

public class TestCanvasPainter extends ApplicationInitializer{
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
	@org.junit.Before
	public void setUp() throws Exception {
		
	}
	
	@org.junit.After
	public void tearDown() throws Exception {
		
	}
	
	static class CanvasWidget extends QCanvasPainterWidget{
		public CanvasWidget(){
		    setFillColor(Qt.GlobalColor.white);
		}
		public void initializeResources(QCanvasPainter p) {
		    var flags = QCanvasPainter.ImageFlag.Repeat.combined(QCanvasPainter.ImageFlag.GenerateMipmaps);
		    m_image.assign(p.addImage(new QImage(":/io/qt/autotests/qt-translucent.png"), flags));
		}
		public void paint(QCanvasPainter p) {
		    float size = qMin(width(), height());
		    float centerX = width() / 2;
		    float centerY = height() / 2;
		    // Paint the background circle
		    QCanvasRadialGradient gradient1 = new QCanvasRadialGradient(centerX, centerY - size * 0.1f, size * 0.6f);
		    gradient1.setStartColor(new QColor(0x909090));
		    gradient1.setEndColor(new QColor(0x404040));
		    p.beginPath();
		    p.circle(new QPointF(centerX, centerY), size * 0.46f);
		    p.setFillStyle(gradient1);
		    p.fill();
		    p.setStrokeStyle(new QColor(0x202020));
		    p.setLineWidth(size * 0.02f);
		    p.stroke();
		    
		    // Hello text
		    p.setTextAlign(QCanvasPainter.TextAlign.Center);
		    p.setTextBaseline(QCanvasPainter.TextBaseline.Middle);
		    QFont font1 = new QFont();
		    font1.setWeight(QFont.Weight.Bold);
		    font1.setItalic(true);
		    font1.setPixelSize(qRound(size * 0.08f));
		    p.setFont(font1);
		    p.setFillStyle(new QColor(0xB0D040));
		    p.fillText("HELLO", centerX, centerY - size * 0.18f);

		    // QCanvasPainter text
		    QFont font2 = new QFont();
		    font2.setWeight(QFont.Weight.Thin);
		    font2.setPixelSize(qRound(size * 0.11f));
		    p.setFont(font2);
		    p.fillText("Qt Canvas Painter", centerX, centerY - size * 0.08f);

		    // Paint heart
		    QCanvasImagePattern pattern = new QCanvasImagePattern(m_image, centerX, centerY, size * 0.08f, size * 0.05f);
		    p.setFillStyle(pattern);
		    
		    p.setLineCap(QCanvasPainter.LineCap.Round);
		    p.setStrokeStyle(new QColor(0xB0D040));
		    p.beginPath();
		    p.moveTo(centerX, centerY + size * 0.3f);
		    p.bezierCurveTo(centerX - size * 0.25f, centerY + size * 0.1f,
		                     centerX - size * 0.05f, centerY + size * 0.05f,
		                     centerX, centerY + size * 0.15f);
		    p.bezierCurveTo(centerX + size * 0.05f, centerY + size * 0.05f,
		                     centerX + size * 0.25f, centerY + size * 0.1f,
		                     centerX, centerY + size * 0.3f);
		    p.stroke();
		    p.fill();
		}
		public void graphicsResourcesInvalidated() {
			m_image.assign(null);
		}

		private final QCanvasImage m_image = new QCanvasImage();
	};
	
	static class MainWindow extends QMainWindow
	{
		public MainWindow(){
		    mdi = new QMdiArea();
		    setCentralWidget(mdi);

		    createCanvasWidget();

		    QMenu fileMenu = menuBar().addMenu(tr("&File"));
		    fileMenu.addAction(tr("&New widget"),
		                        new QKeySequence(QKeySequence.StandardKey.New),
		                        this, MainWindow::createCanvasWidget);
		    fileMenu.addAction(tr("E&xit"),
		    					new QKeySequence(QKeySequence.StandardKey.Quit),
		    					QCoreApplication::quit);
		}

		private void createCanvasWidget(){
		    CanvasWidget canvasWidget = new CanvasWidget();
		    mdi.addSubWindow(canvasWidget).resize(500, 500);
		    canvasWidget.show();
		}

		private final QMdiArea mdi;
	};

	
	@org.junit.Test
	public void test() {
		MainWindow mainWindow = new MainWindow();
	    mainWindow.resize(1280, 720);
	    mainWindow.show();
	    QTimer.singleShot(5000, QCoreApplication.instance(), QCoreApplication::quit);
	    QCoreApplication.exec();
	}
}
