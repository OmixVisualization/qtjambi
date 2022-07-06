/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

import static org.junit.Assert.*;

import java.util.*;
import java.util.concurrent.atomic.AtomicInteger;

import org.junit.*;

import io.qt.*;
import io.qt.core.*;
import io.qt.gui.QGuiApplication;
import io.qt.widgets.*;

public class TestReferenceCountingQLayout extends ApplicationInitializer{
	
	@BeforeClass
    public static void testInitialize() throws Exception {
        ApplicationInitializer.testInitialize();
		Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
    }
	
	private static final int COUNT = 20;
	
	@Test public void testStackedLayoutAddAndRemoveWidget() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		QStackedLayout layout = new QStackedLayout();
        List<QWidget> widgets = new ArrayList<>();
        for (int i=0; i<COUNT; ++i) {
        	QWidget w = new QWidget();
        	w.setVisible(false);
            QtUtilities.getSignalOnDispose(w).connect(counter::incrementAndGet);
            layout.addWidget(w);
            QApplication.processEvents();
            widgets.add(w);
            w = null;
        }
        for (int i = 0; i < 20 && counter.get()==0; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(0, counter.get());
        assertEquals(COUNT, layout.count());
        for (int i=0; i<COUNT; ++i) {
            layout.removeWidget(widgets.get(i));
            QApplication.processEvents();
        }
        widgets = null;
        for (int i = 0; i < 50 && counter.get()<COUNT; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT, counter.get());
	}
	
	@Test public void testStackedLayoutAddWidget() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		QStackedLayout layout = new QStackedLayout();
        for (int i=0; i<COUNT; ++i) {
        	QWidget w = new QWidget();
            QtUtilities.getSignalOnDispose(w).connect(counter::incrementAndGet);
            layout.addWidget(w);
            QApplication.processEvents();
            w = null;
        }
        for (int i = 0; i < 20 && counter.get()==0; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(0, counter.get());
        assertEquals(COUNT, layout.count());
	}
	
	@Test public void testGridLayoutAddWidget() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		QGridLayout layout = new QGridLayout();
        for (int i=0; i<COUNT; ++i) {
        	QWidget w = new QWidget();
            QtUtilities.getSignalOnDispose(w).connect(counter::incrementAndGet);
            layout.addWidget(w, 0, i, 1, 1);
            QApplication.processEvents();
            w = null;
        }
        for (int i = 0; i < 20 && counter.get()==0; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(0, counter.get());
        assertEquals(COUNT, layout.count());
	}
	
	@Test public void testFormLayoutAddWidget() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		QFormLayout layout = new QFormLayout();
        for (int i=0; i<COUNT; ++i) {
        	QWidget w = new QWidget();
            QtUtilities.getSignalOnDispose(w).connect(counter::incrementAndGet);
            layout.addRow("widget" + i, w);
            QApplication.processEvents();
            w = null;
        }
        for (int i = 0; i < 20 && counter.get()==0; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(0, counter.get());
        assertEquals(COUNT*2, layout.count());
	}
	
	@Test public void testBoxLayoutAddWidget() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		QHBoxLayout layout = new QHBoxLayout();
        for (int i=0; i<COUNT; ++i) {
        	QWidget w = new QWidget();
            QtUtilities.getSignalOnDispose(w).connect(counter::incrementAndGet);
            layout.addWidget(w, i);
            QApplication.processEvents();
            w = null;
        }
        for (int i = 0; i < 20 && counter.get()==0; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(0, counter.get());
        assertEquals(COUNT, layout.count());
	}
	
	@Test public void testStackedLayoutAddStackedWidget() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		QStackedLayout layout = new QStackedLayout();
        for (int i=0; i<COUNT; ++i) {
        	QWidget w = new QWidget();
            QtUtilities.getSignalOnDispose(w).connect(counter::incrementAndGet);
            layout.stackWidget(w);
            QApplication.processEvents();
            w = null;
        }
        for (int i = 0; i < 20 && counter.get()==0; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(0, counter.get());
        assertEquals(COUNT, layout.count());
	}
	
	@Test 
	public void testStackedLayoutAddWidgetTwice() {
		QStackedLayout layout = new QStackedLayout();
    	QWidget w = new QWidget();
        layout.addWidget(w);
        try {
        	layout.addWidget(w);
        	Assert.fail("IllegalArgumentException expected to be thrown.");
        }catch(IllegalArgumentException e) {}
        try {
        	layout.insertWidget(1, w);
        	Assert.fail("IllegalArgumentException expected to be thrown.");
        }catch(IllegalArgumentException e) {}
	}
	
	@Test 
	public void testFormLayoutAddWidgetTwice() {
		QFormLayout layout = new QFormLayout();
    	QWidget w = new QWidget();
        layout.addWidget(w);
        try {
        	layout.addRow(w);
        	Assert.fail("IllegalArgumentException expected to be thrown.");
        }catch(IllegalArgumentException e) {}
        try {
        	layout.insertRow(1, w);
        	Assert.fail("IllegalArgumentException expected to be thrown.");
        }catch(IllegalArgumentException e) {}
	}
	
	@Test 
	public void testGridLayoutAddWidgetTwice() {
		QGridLayout layout = new QGridLayout();
    	QWidget w = new QWidget();
        layout.addWidget(w);
        try {
        	layout.addWidget(w, 1, 2);
        	Assert.fail("IllegalArgumentException expected to be thrown.");
        }catch(IllegalArgumentException e) {}
        try {
        	layout.addWidget(w, 2, 3, 4, 5);
        	Assert.fail("IllegalArgumentException expected to be thrown.");
        }catch(IllegalArgumentException e) {}
	}
	
	@Test 
	public void testBoxLayoutAddWidgetTwice() {
		QHBoxLayout layout = new QHBoxLayout();
    	QWidget w = new QWidget();
        layout.addWidget(w, 2);
        try {
        	layout.addWidget(w, 2);
        	Assert.fail("IllegalArgumentException expected to be thrown.");
        }catch(IllegalArgumentException e) {}
        try {
        	layout.addWidget(w);
        	Assert.fail("IllegalArgumentException expected to be thrown.");
        }catch(IllegalArgumentException e) {}
	}
}
