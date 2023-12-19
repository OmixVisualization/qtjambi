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

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.*;

import io.qt.QThreadAffinityException;
import io.qt.core.QEventLoop;
import io.qt.core.QThread;
import io.qt.core.QTimer;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QPaintEvent;
import io.qt.gui.QPainter;
import io.qt.gui.QPaintingOutsidePaintEventException;
import io.qt.gui.QPixmap;
import io.qt.widgets.QWidget;

public class TestPaintOnWidget extends ApplicationInitializer{
	
	static {
		System.setProperty("io.qt.enable-thread-affinity-check", "true");
		System.setProperty("io.qt.enable-event-thread-affinity-check", "true");
	}
	
    @BeforeClass
    public static void testInitialize() throws Exception {
        ApplicationInitializer.testInitializeWithWidgets();
		Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
	}
	
	@Test
    public void testPaintingOnPixmapViaConstructor() {
    	QPixmap device = new QPixmap(128, 128);
    	QPainter painter = new QPainter(device);
    	painter.drawLine(1, 1, 5, 5);
    	try{
    		QPainter painter2 = new QPainter();
    		assertFalse(painter2.begin(device));
    	}finally {
    		painter.end();
    		device.dispose();
    	}
    }
	
	@Test
    public void testPaintingOnPixmapViaBegin() {
    	QPixmap device = new QPixmap(128, 128);
    	QPainter painter = new QPainter();
    	assertTrue(painter.begin(device));
    	painter.drawLine(1, 1, 5, 5);
    	try{
    		QPainter painter2 = new QPainter();
    		assertFalse(painter2.begin(device));
    	}finally {
    		painter.end();
    		device.dispose();
    	}
    }
	
	@Test(expected=QPaintingOutsidePaintEventException.class)
    public void testPaintingOutsideViaConstructor() {
    	QWidget widget = new QWidget();
    	widget.setVisible(true);
    	try {
	    	QPainter painter = new QPainter(widget);
	    	painter.drawLine(1, 1, 5, 5);
	    	painter.end();
    	}finally {
        	widget.setVisible(false);
    		widget.dispose();
    	}
    }
	
	@Test
    public void testPaintingOutsideViaBegin() {
    	QWidget widget = new QWidget();
    	widget.setVisible(true);
    	try {
	    	QPainter painter = new QPainter();
	    	boolean began = painter.begin(widget);
			if(began)
				painter.end();
			assertFalse(began);
    	}finally {
        	widget.setVisible(false);
    		widget.dispose();
    	}
    }
    
    @Test(expected=QThreadAffinityException.class)
    public void testPaintingInThreadViaConstructor() {
    	QThreadAffinityException[] exn = {null};
    	QEventLoop loop = new QEventLoop();
    	QWidget widget = new QWidget() {
    		@Override
			protected void paintEvent(QPaintEvent event) {
    	    	QThread t = QThread.create(()->{
    	    		try {
    					new QPainter(this);
    				} catch (QThreadAffinityException e) {
    					exn[0] = e;
    				}
    	    	});
    	    	t.start();
    	    	t.join();
    	    	loop.quit();
    	    	t.dispose();
    		}
    	};
    	widget.setVisible(true);
    	QTimer.singleShot(15000, loop::quit);
    	loop.exec();
    	widget.setVisible(false);
    	widget.dispose();
    	loop.dispose();
    	if(exn[0]!=null)
    		throw exn[0];
    }
    
    @Test(expected=QThreadAffinityException.class)
    public void testPaintingInThreadViaBegin() {
    	QThreadAffinityException[] exn = {null};
    	{
	    	QEventLoop loop = new QEventLoop();
	    	QWidget widget = new QWidget() {
	    		@Override
				protected void paintEvent(QPaintEvent event) {
	    	    	QThread t = QThread.create(()->{
	    	    		QPainter painter = new QPainter();
	    	    		try {
	    	    			painter.begin(this);
	    				} catch (QThreadAffinityException e) {
	    					exn[0] = e;
	    				}
	    	    	});
	    	    	t.start();
	    	    	t.join();
	    	    	loop.quit();
	    	    	t.dispose();
	    		}
	    	};
	    	widget.setVisible(true);
	    	QTimer.singleShot(15000, loop::quit);
	    	loop.exec();
	    	widget.setVisible(false);
	    	widget.dispose();
	    	loop.dispose();
	    }
    	ApplicationInitializer.runGC();
    	if(exn[0]!=null)
    		throw exn[0];
    }
    
    @Test
    public void testPaintingViaBeginUnended() {
    	QEventLoop loop = new QEventLoop();
    	QWidget widget = new QWidget() {
    		@Override
			protected void paintEvent(QPaintEvent event) {
	    		QPainter painter = new QPainter();
    			painter.begin(this);
    	    	loop.quit();
    		}
    	};
    	widget.setVisible(true);
    	QTimer.singleShot(15000, loop::quit);
    	loop.exec();
    	widget.dispose();
    	loop.dispose();
	}
    
    @Test
    public void testPaintingInsideViaConstructor() {
    	QPainter[] activePainter = {null};
    	QEventLoop loop = new QEventLoop();
    	QWidget widget = new QWidget() {
			@Override
			protected void paintEvent(QPaintEvent event) {
		    	QPainter painter = new QPainter(this);
		    	painter.drawLine(1, 1, 5, 5);
		    	activePainter[0] = painter;
				super.paintEvent(event);
    	    	loop.quit();
			}
    	};
    	widget.setVisible(true);
    	QTimer.singleShot(15000, loop::quit);
    	loop.exec();
    	widget.setVisible(false);
    	try {
	    	assertTrue(activePainter[0]!=null);
	    	assertTrue(activePainter[0].isDisposed());
    	}finally {
	    	widget.dispose();
	    	loop.dispose();
    	}
	}
    
    @Test
    public void testPaintingInsideViaBegin() {
    	QPainter[] activePainter = {null};
    	QEventLoop loop = new QEventLoop();
    	QWidget widget = new QWidget() {
			@Override
			protected void paintEvent(QPaintEvent event) {
		    	QPainter painter = new QPainter();
		    	assertTrue(painter.begin(this));
		    	painter.drawLine(1, 1, 5, 5);
		    	activePainter[0] = painter;
				super.paintEvent(event);
    	    	loop.quit();
			}
    	};
    	widget.setVisible(true);
    	QTimer.singleShot(15000, loop::quit);
    	loop.exec();
    	widget.setVisible(false);
    	try {
	    	assertTrue(activePainter[0]!=null);
	    	assertTrue(activePainter[0].isDisposed());
    	}finally {
	    	widget.dispose();
	    	loop.dispose();
    	}
	}
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestPaintOnWidget.class.getName());
    }
}
