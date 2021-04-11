/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtUtilities;
import io.qt.core.*;
import io.qt.gui.*;
import io.qt.widgets.*;

public class TestReferenceCountingQGraphicsWidget extends QApplicationTest{
	
	@BeforeClass
    public static void testInitialize() throws Exception {
        QApplicationTest.testInitialize();
		Assume.assumeThat(QGuiApplication.primaryScreen()!=null, QApplicationTest.trueMatcher("A screen is required to create a window."));
    }
	
	private static final int COUNT = 200;
	
	static void uncalled(){
		QGuiApplication.platformName();
	}
	
	@Test public void testActionsNull() {
        QGraphicsWidget w = new QGraphicsWidget();
        w.addAction(null);
        w.addActions(null);
        w.removeAction(null);
        assertEquals(0, w.actions().size());
	}
	
	@Test public void testAddActions() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QGraphicsWidget w = new QGraphicsWidget();
	        for (int i=0; i<COUNT; ++i) {
                QAction act = new QAction("action" + i, null);
                QtUtilities.getSignalOnDispose(act).connect(counter::incrementAndGet);
                w.addActions(QList.of(act));
                QApplication.processEvents();
                act = null;
	        }
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            System.gc();
	            System.runFinalization();
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
	        w = null;
		}
        for (int i = 0; i < 50 && counter.get()<COUNT; i++) {
            System.gc();
            System.runFinalization();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT, counter.get());
	}
	
	@Test public void testInsertActions() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QGraphicsWidget w = new QGraphicsWidget();
	        for (int i=0; i<COUNT; ++i) {
                QAction act = new QAction("action" + i, null);
                QtUtilities.getSignalOnDispose(act).connect(counter::incrementAndGet);
                w.insertActions(null, QList.of(act));
                QApplication.processEvents();
                act = null;
	        }
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            System.gc();
	            System.runFinalization();
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
	        w = null;
		}
        for (int i = 0; i < 50 && counter.get()<COUNT; i++) {
            System.gc();
            System.runFinalization();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT, counter.get());
	}
	
	@Test public void testInsertAction() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QGraphicsWidget w = new QGraphicsWidget();
	        for (int i=0; i<COUNT; ++i) {
                QAction act = new QAction("action" + i, null);
                QtUtilities.getSignalOnDispose(act).connect(counter::incrementAndGet);
                w.insertAction(null, act);
                QApplication.processEvents();
                act = null;
	        }
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            System.gc();
	            System.runFinalization();
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
	        w = null;
		}
        for (int i = 0; i < 50 && counter.get()<COUNT; i++) {
            System.gc();
            System.runFinalization();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT, counter.get());
	}
	
	@Test public void testAddActionTwice() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		QGraphicsWidget w = new QGraphicsWidget();
        for (int i=0; i<COUNT; ++i) {
            QAction act = new QAction("action" + i, null);
            QtUtilities.getSignalOnDispose(act).connect(counter::incrementAndGet);
            w.addAction(act);
            w.addAction(act);
            QApplication.processEvents();
            act = null;
        }
        for (int i = 0; i < 20 && counter.get()==0; i++) {
            System.gc();
            System.runFinalization();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(0, counter.get());
        List<QAction> actions = new ArrayList<>(w.actions());
        assertEquals(COUNT, actions.size());
        for (int i=0; i<COUNT; ++i) {
            assertTrue(actions.get(i) != null);
            assertEquals("action" + i, actions.get(i).text());
            w.removeAction(actions.get(i));
            QApplication.processEvents();
        }
        actions = null;
        for (int i = 0; i < 50 && counter.get()<COUNT; i++) {
            System.gc();
            System.runFinalization();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        assertEquals(0, w.actions().size());
        Assert.assertEquals(COUNT, counter.get());
	}
	
	@Test public void testAddAndRemoveAction() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		QGraphicsWidget w = new QGraphicsWidget();
        for (int i=0; i<COUNT; ++i) {
            QAction act = new QAction("action" + i, null);
            QtUtilities.getSignalOnDispose(act).connect(counter::incrementAndGet);
            w.addAction(act);
            QApplication.processEvents();
            act = null;
        }
        for (int i = 0; i < 20 && counter.get()==0; i++) {
            System.gc();
            System.runFinalization();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(0, counter.get());
        List<QAction> actions = new ArrayList<>(w.actions());
        assertEquals(COUNT, actions.size());
        for (int i=0; i<COUNT; ++i) {
            assertTrue(actions.get(i) != null);
            assertEquals("action" + i, actions.get(i).text());
            w.removeAction(actions.get(i));
            QApplication.processEvents();
        }
        actions = null;
        for (int i = 0; i < 50 && counter.get()<COUNT; i++) {
            System.gc();
            System.runFinalization();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT, counter.get());
	}
}
