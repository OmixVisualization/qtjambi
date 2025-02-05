/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

import static org.junit.Assert.*;

import java.util.HashSet;
import java.util.Set;
import java.util.concurrent.atomic.AtomicReference;

import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QNoNativeResourcesException;
import io.qt.QtUtilities;
import io.qt.autotests.generated.General;
import io.qt.autotests.generated.UnMoccedObject;
import io.qt.core.*;
import io.qt.internal.TestUtility;
import io.qt.widgets.QColorDialog;

public class TestQObjectDestroyed extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
    
    @Test
    public void testDestroyOnDispose() {
    	QObject object = UnMoccedObject.create(1);
    	AtomicReference<QObject> destroyedObject = new AtomicReference<>();
    	object.destroyed.connect(destroyedObject::set);
    	object.dispose();
    	assertEquals(object, destroyedObject.get());
    	object = null;
    	destroyedObject.set(null);
    	
    	QThread thread = new QThread();
    	thread.setObjectName("Thread");
    	thread.destroyed.connect(obj -> {
    		if(obj instanceof QThread) {
    			QThread trd = (QThread)obj;
    			System.out.println(trd.objectName());
    		}
    	});
    	thread.dispose();
    }
    
    @Test
    public void testDestroyedOnGC() {
    	final QTimer timer = new QTimer();
    	timer.setInterval(200);
    	timer.timeout.connect(()->{
    		System.gc();
    		QCoreApplication.processEvents();
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
    	});
    	timer.start();
    	final QEventLoop loop = new QEventLoop();
    	AtomicReference<QObject> destroyedObject = new AtomicReference<>();
    	boolean[] received = {false,false};
    	{
	    	QObject object = UnMoccedObject.create(1);
	    	QtUtilities.getSignalOnDispose(object).connect(()->received[1] = true);
	    	object.destroyed.connect(o->{
	    		if(!loop.isDisposed()) {
//		    		System.out.println("destroyed("+o+")");
		    		received[0] = true;
		    		destroyedObject.set(o);
					loop.quit();
	    		}
	    	});
	    	object = null;
    	}
    	QTimer.singleShot(10000, loop::quit);
    	loop.exec();
    	while(TestUtility.hasDeleteLaterEvents()) {
            QCoreApplication.processEvents();
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());	        	
        }
    	loop.dispose();
    	assertTrue("destroyed signal not received", received[0]);
    	assertTrue("disposed signal not received", received[1]);
    	assertTrue(destroyedObject.get()!=null);
    	assertTrue(destroyedObject.get().isDisposed());
    }
	
	@Test
    public void testDestroyChildren() throws InterruptedException {
		Set<QObject> received = new HashSet<>();
		Set<QObject> expected = new HashSet<>();
		int[] dialogDestroyed = {0};
		{
			QColorDialog dialog = new QColorDialog();
			for(QObject child : dialog.children()) {
				expected.add(child);
				child.destroyed.connect(received::add);
				assertTrue("not cpp ownership", General.internalAccess.isCppOwnership(child));
			}
			dialog.destroyed.connect(c->{
				dialogDestroyed[0] = c==null ? -1 : 1;
			});
			dialog = null;
		}
		ApplicationInitializer.runGC();
        Thread.sleep(50);
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		ApplicationInitializer.runGC();
        Thread.sleep(50);
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		ApplicationInitializer.runGC();
        Thread.sleep(50);
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		ApplicationInitializer.runGC();
        Thread.sleep(50);
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		assertEquals(1, dialogDestroyed[0]);
		assertEquals(expected.size(), received.size());
		for (QObject object : expected) {
			assertTrue(received.contains(object));
		}
	}
	
	static class DestroyResponseObject extends QObject{
		public void onDestroyed(QObject o){
			System.out.println("DestroyResponseObject@"+System.identityHashCode(o));
		}
		static void test(boolean[] disposed, boolean[] received) {
			DestroyResponseObject d = new DestroyResponseObject();
			QtUtilities.getSignalOnDispose(d).connect(()->disposed[0] = true);
			d.destroyed.connect(o->{
				received[0] = true;
				d.onDestroyed(o);
			});
		}
	}
	
	@Test
    public void testSelf() throws InterruptedException {
		Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
		boolean[] disposed = {false};
		boolean[] received = {false};
		DestroyResponseObject.test(disposed, received);
		ApplicationInitializer.runGC();
        Thread.sleep(50);
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		ApplicationInitializer.runGC();
        Thread.sleep(50);
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		ApplicationInitializer.runGC();
        Thread.sleep(50);
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		ApplicationInitializer.runGC();
        Thread.sleep(50);
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		ApplicationInitializer.runGC();
        Thread.sleep(50);
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		ApplicationInitializer.runGC();
        Thread.sleep(50);
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
        assertTrue(disposed[0]);
        assertFalse(received[0]);
	}
	
	@Test
    public void testDestroyedSignalQueued() throws InterruptedException {
		QObject object = new QObject();
		object.destroyed.connect(o -> {
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "destroyed: "+System.identityHashCode(o));
		}, Qt.ConnectionType.QueuedConnection);
		ApplicationInitializer.runGC();
        Thread.sleep(50);
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
        QCoreApplication.processEvents();
        // nothing should happen
	}
    
    @Test(expected=QNoNativeResourcesException.class)
    public void run_no_native() throws Exception
    {
    	QObject object = new QObject();
    	object.dispose();
    	object.setObjectName("test");
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQObjectDestroyed.class.getName());
    }
}
