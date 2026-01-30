/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.autotests.generated.General;
import io.qt.core.*;
import io.qt.gui.*;
import io.qt.widgets.*;

public class TestReferenceCountingQWidget extends ApplicationInitializer{
	
	@BeforeClass
    public static void testInitialize() throws Exception {
        ApplicationInitializer.testInitializeWithWidgets();
		Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
    }
	
	private static final int COUNT = 200;
	
	static void uncalled(){
		QGuiApplication.platformName();
	}
	
	@Test public void testQWidgetActionsNull() {
        QWidget w = new QWidget();
        w.addAction((QAction)null);
        w.addActions(null);
        w.removeAction(null);
        assertEquals(0, w.actions().size());
	}
	
	@Test public void testQWidgetAddActions() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QWidget w = new QWidget();
	        for (int i=0; i<COUNT; ++i) {
                QAction act = new QAction("action" + i, null);
                General.internalAccess.registerCleaner(act, counter::incrementAndGet);
                w.addActions(QList.of(act));
                QApplication.processEvents();
                act = null;
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
	        List<QAction> actions = w.actions();
	        assertEquals(COUNT, actions.size());
	        for (int i=0; i<COUNT; ++i) {
	            assertTrue(actions.get(i) != null);
	            assertEquals("action" + i, actions.get(i).text());
	        }
            General.internalAccess.registerCleaner(w, counter::incrementAndGet);
	        w = null;
		}
        for (int i = 0; i < 50 && counter.get()<COUNT+1; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT+1, counter.get());
	}
	
	@Test public void testQWidgetInsertActions() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QWidget w = new QWidget();
	        for (int i=0; i<COUNT; ++i) {
                QAction act = new QAction("action" + i, null);
                General.internalAccess.registerCleaner(act, counter::incrementAndGet);
                w.insertActions(null, QList.of(act));
                QApplication.processEvents();
                act = null;
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
	        List<QAction> actions = w.actions();
	        assertEquals(COUNT, actions.size());
	        for (int i=0; i<COUNT; ++i) {
	            assertTrue(actions.get(i) != null);
	            assertEquals("action" + i, actions.get(i).text());
	        }
            General.internalAccess.registerCleaner(w, counter::incrementAndGet);
	        w = null;
		}
        for (int i = 0; i < 60 && counter.get()<COUNT+1; i++) {
            ApplicationInitializer.runGC();
            Thread.yield();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT+1, counter.get());
	}
	
	@Test public void testQWidgetInsertAction() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QWidget w = new QWidget();
	        for (int i=0; i<COUNT; ++i) {
                QAction act = new QAction("action" + i, null);
                General.internalAccess.registerCleaner(act, counter::incrementAndGet);
                w.insertAction(null, act);
                QApplication.processEvents();
                act = null;
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
	        QList<QAction> actions = w.actions();
	        assertEquals(COUNT, actions.size());
	        for (int i=0; i<COUNT; ++i) {
	            assertTrue(actions.get(i) != null);
	            assertEquals("action" + i, actions.get(i).text());
	        }
	        actions = null;
            General.internalAccess.registerCleaner(w, counter::incrementAndGet);
	        w = null;
		}
        for (int i = 0; i < 50 && counter.get()<COUNT+1; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT+1, counter.get());
	}
	
	@Test public void testQWidgetAddActionTwice() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		QWidget w = new QWidget();
        for (int i=0; i<COUNT; ++i) {
            QAction act = new QAction("action" + i, null);
            General.internalAccess.registerCleaner(act, counter::incrementAndGet);
            w.addAction(act);
            w.addAction(act);
            QApplication.processEvents();
            act = null;
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
        QList<QAction> actions = w.actions();
        assertEquals(COUNT, actions.size());
        for (int i=0; i<COUNT; ++i) {
        	QAction action = actions.removeFirst();
            assertTrue(action != null);
            assertEquals("action" + i, action.text());
            w.removeAction(action);
            assertTrue("QAction does not have java ownership", General.internalAccess.isJavaOwnership(action));
            action = null;
            QApplication.processEvents();
        }
        actions = null;
        for (int i = 0; i < 120 && counter.get()<COUNT; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        assertEquals(0, w.actions().size());
        Assert.assertEquals(COUNT, counter.get());
	}
	
	@Test public void testQWidgetAddAndRemoveAction() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		QWidget w = new QWidget();
        for (int i=0; i<COUNT; ++i) {
            QAction act = new QAction("action" + i, null);
            General.internalAccess.registerCleaner(act, counter::incrementAndGet);
            w.addAction(act);
            QApplication.processEvents();
            act = null;
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
        QList<QAction> actions = w.actions();
        assertEquals(COUNT, actions.size());
        for (int i=0; i<COUNT; ++i) {
        	QAction action = actions.removeFirst();
            assertTrue(action != null);
            assertEquals("action" + i, action.text());
            w.removeAction(action);
            assertTrue("QAction does not have java ownership", General.internalAccess.isJavaOwnership(action));
            action = null;
            QApplication.processEvents();
        }
        actions = null;
        for (int i = 0; i < 120 && counter.get()<COUNT; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        assertEquals(0, w.actions().size());
        Assert.assertEquals(COUNT, counter.get());
	}
	
	@Test 
	public final void testQWidgetSetEffect() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QWidget o = new QWidget();
	        {
	        	QGraphicsEffect effect = new QGraphicsBlurEffect();
	        	General.internalAccess.registerCleaner(effect, counter::incrementAndGet);
	        	o.setGraphicsEffect(effect);
	            effect = null;
	        }
	        
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        assertTrue(o.graphicsEffect() != null);
	        o = null;
		}
		for (int i = 0; i < 50 && counter.get()==0; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(1, counter.get());
        ApplicationInitializer.runGC();
	}
	
	@Test 
	public final void testQWidgetSetEffectNull() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QWidget o = new QWidget();
	        {
	        	QGraphicsEffect effect = new QGraphicsBlurEffect();
	        	General.internalAccess.registerCleaner(effect, counter::incrementAndGet);
	        	o.setGraphicsEffect(effect);
	        	o.setGraphicsEffect(null);
	            effect = null;
	        }
	        
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(1, counter.get());
	        o = null;
		}
        ApplicationInitializer.runGC();
	}
	
	@Test 
	public final void testQWidgetSetStyle() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QWidget w = new QWidget();
	        {
	        	QStyle style = new QCommonStyle();
	        	General.internalAccess.registerCleaner(style, counter::incrementAndGet);
	        	w.setStyle(style);
	            style = null;
	        }
	        
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        assertTrue(w.style() != null);
	        w = null;
		}
		for (int i = 0; i < 50 && counter.get()==0; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(1, counter.get());
        ApplicationInitializer.runGC();
	}
	
	@Test 
	public final void testQWidgetSetStyleNull() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QWidget w = new QWidget();
	        {
	        	QStyle style = new QCommonStyle();
	        	General.internalAccess.registerCleaner(style, counter::incrementAndGet);
	        	w.setStyle(style);
	        	w.setStyle(null);
	            style = null;
	        }
	        
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(1, counter.get());
	        w = null;
		}
        ApplicationInitializer.runGC();
	}
}
