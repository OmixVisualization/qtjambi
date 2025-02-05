/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import static org.junit.Assert.assertTrue;

import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QEventLoop;
import io.qt.core.QTimer;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QPainter;
import io.qt.opengl.QOpenGLWindow;
import io.qt.opengl.widgets.QOpenGLWidget;

public class TestPaintOnWidgetQt6 extends ApplicationInitializer {
	
    @BeforeClass
    public static void testInitialize() throws Exception {
        ApplicationInitializer.testInitializeWithWidgets();
		Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
	}
    
    @Test
    public void testPaintingInWidgetPaintGLViaBegin() {
    	QPainter[] activePainter = {null};
    	QEventLoop loop = new QEventLoop();
    	QOpenGLWidget widget = new QOpenGLWidget() {
			@Override
			protected void paintGL() {
		    	try {
					QPainter painter = new QPainter();
			    	assertTrue(painter.begin(this));
			    	painter.drawLine(1, 1, 5, 5);
			    	activePainter[0] = painter;
		    	}finally {
		    		loop.quit();
		    	}
			}
    	};
    	widget.setVisible(true);
    	QTimer.singleShot(5000, loop::quit);
    	loop.exec();
    	widget.setVisible(false);
    	assertTrue("paintGL() not called", activePainter[0]!=null);
    	assertTrue(activePainter[0].isDisposed());
    }
    
    @Test
    public void testPaintingInWidgetPaintGLViaConstructor() {
    	QPainter[] activePainter = {null};
    	QEventLoop loop = new QEventLoop();
    	QOpenGLWidget widget = new QOpenGLWidget() {
			@Override
			protected void paintGL() {
		    	try {
					QPainter painter = new QPainter(this);
			    	painter.drawLine(1, 1, 5, 5);
			    	activePainter[0] = painter;
		    	}finally {
		    		loop.quit();
		    	}
			}
    	};
    	widget.setVisible(true);
    	QTimer.singleShot(5000, loop::quit);
    	loop.exec();
    	widget.setVisible(false);
    	assertTrue("paintGL() not called", activePainter[0]!=null);
    	assertTrue(activePainter[0].isDisposed());
    }
    
    @Test
    public void testPaintingInWindowPaintGLViaBegin() {
    	QPainter[] activePainter = {null};
    	QEventLoop loop = new QEventLoop();
    	QOpenGLWindow widget = new QOpenGLWindow() {
			@Override
			protected void paintGL() {
		    	try {
					QPainter painter = new QPainter();
			    	assertTrue(painter.begin(this));
			    	painter.drawLine(1, 1, 5, 5);
			    	activePainter[0] = painter;
		    	}finally {
		    		loop.quit();
		    	}
			}
    	};
    	widget.setVisible(true);
    	QTimer.singleShot(5000, loop::quit);
    	loop.exec();
    	widget.setVisible(false);
    	assertTrue("paintGL() not called", activePainter[0]!=null);
    	assertTrue(activePainter[0].isDisposed());
    }
    
    @Test
    public void testPaintingInWindowPaintGLViaConstructor() {
    	QPainter[] activePainter = {null};
    	QEventLoop loop = new QEventLoop();
    	QOpenGLWindow widget = new QOpenGLWindow() {
			@Override
			protected void paintGL() {
		    	try {
					QPainter painter = new QPainter(this);
			    	painter.drawLine(1, 1, 5, 5);
			    	activePainter[0] = painter;
		    	}finally {
		    		loop.quit();
		    	}
			}
    	};
    	widget.setVisible(true);
    	QTimer.singleShot(5000, loop::quit);
    	loop.exec();
    	widget.setVisible(false);
    	assertTrue("paintGL() not called", activePainter[0]!=null);
    	assertTrue(activePainter[0].isDisposed());
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestPaintOnWidgetQt6.class.getName());
    }
}
