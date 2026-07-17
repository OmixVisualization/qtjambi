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

import java.util.HashSet;
import java.util.Set;
import java.util.concurrent.atomic.AtomicBoolean;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import io.qt.QtUtilities;
import io.qt.autotests.generated.General;
import io.qt.autotests.generated.OrdinaryDestroyed;
import io.qt.autotests.generated.OrdinarySuperclass;
import io.qt.autotests.generated.QObjectDestroyed;
import io.qt.core.QEvent;
import io.qt.core.QObject;
import io.qt.core.QRect;
import io.qt.core.Qt;
import io.qt.gui.QPaintEvent;
import io.qt.widgets.QApplication;

public class TestOwnership extends ApplicationInitializer {
    @Before
    public void setUp() {
        QApplication.processEvents();
    }

    @After
    public void tearDown() {
        QApplication.processEvents();
        QApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
        
        ApplicationInitializer.runGC();
    }

    // Tests the ownership transfer that we need to have objects like
    // QEvent stay alive after they are posted to the event queue...
    // The verification here is basically that the vm doesn't crash.
    private static class OwnershipTransferReceiver extends QObject {
        public QEvent.Type event_id;

        public QRect rect;

        @Override
        public boolean event(QEvent e) {
            event_id = e.type();

            if(event_id==QEvent.Type.Paint) {
	            QPaintEvent pe = null;
	            try {
	                pe = (QPaintEvent) e;
	                rect = pe.rect();
	            } catch (Exception ex) {
	                ex.printStackTrace();
	            }
            }

            return super.event(e);
        }
    }

    private static class CustomPaintEvent extends QPaintEvent {
        public static AtomicBoolean finalized = new AtomicBoolean();
        
        private static void onFinalize() {
        	finalized.set(true);
        }

        public CustomPaintEvent(QRect rect) {
            super(rect);
            QtUtilities.getSignalOnDispose(this).connect(CustomPaintEvent::onFinalize);
        }
    }

    @Test
    public void testPostEvent() {
    	java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "run_testOwnershipTransfer() BEGIN");
        OwnershipTransferReceiver receiver = new OwnershipTransferReceiver();

//        for (int i = 0; i < 3; ++i) 
        {	// was: i < 1  // it appears we leak receiver
            QRect rect = new QRect(1, 2, 3, 4);
            QApplication.postEvent(receiver, new CustomPaintEvent(rect));
        }

        // To attempt deletion of the QPaintEvent, should not happen at
        // this time...
        ApplicationInitializer.runGC();
        assertEquals(CustomPaintEvent.finalized.get(), false);

        // Process the event, thus also deleting it...
        QApplication.processEvents();

        // To provoke collection of the java side of the object, now
        // that C++ has released its hold on it
        ApplicationInitializer.runGC();
        try {
            for(int i = 0; i < 60; i++) {
                if(CustomPaintEvent.finalized.get())
                    break;
                Thread.sleep(10);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "run_testOwnershipTransfer() TEST");
        assertEquals(CustomPaintEvent.finalized.get(), true);

        // Sanity check the data...
        assertEquals(receiver.event_id, QEvent.Type.Paint);
        assertEquals(1, receiver.rect.x());
        assertEquals(2, receiver.rect.y());
        assertEquals(3, receiver.rect.width());
        assertEquals(4, receiver.rect.height());
java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "run_testOwnershipTransfer() END");
    }
    
    @Test
    public void test_GC_versus_access_SplitOwnership() throws InterruptedException {
    	Set<Integer> hashes = new HashSet<>();
    	for (int i = 0; i < 50 && hashes.size()<=1; i++) {
    		{
				ApplicationInitializer.runGC();
	    		QObject globalObject = OrdinaryDestroyed.getGlobalQObjectSplitOwnership();
	    		assertTrue(globalObject!=null);
	    		assertTrue(General.internalAccess.isSplitOwnership(globalObject));
	    		hashes.add(System.identityHashCode(globalObject));
	    		globalObject = null;
    		}
			Thread.yield();
    		Thread.sleep(25);
		}
    	assertTrue("Expect to create more than one java objects for global QObject with split ownership.", hashes.size()>1);
    }

    @Test
    public void testDestructionCppDelete_Ordinary(){
    	DisposeCounter counter = new DisposeCounter();
        OrdinarySubclass sc = new OrdinarySubclass(counter);
        OrdinaryDestroyed.deleteFromCpp(sc);
        assertEquals(1, counter.disposedCount());
        assertTrue(sc.isDisposed());
    }

    @Test
    public void testDestructionByVirtualDestructor_Ordinary(){
    	DisposeCounter counter = new DisposeCounter();
        OrdinarySuperclass sc = new OrdinarySubclass(counter);
        OrdinaryDestroyed.deleteFromCppOther(sc);
        assertEquals(1, counter.disposedCount());
        assertTrue(sc.isDisposed());
    }

    @Test
    public void testDestructionByDispose_QObject()
    {
        DisposeCounter counter = new DisposeCounter();
        QObjectSubclass qobject = new QObjectSubclass(counter, null);
        qobject.dispose();
        assertEquals(1, counter.disposedCount());
        assertTrue(qobject.isDisposed());
    }

    @Test
    public void testDestructionByParent_QObject()
    {
        DisposeCounter counter = new DisposeCounter();
        QObject parent = new QObject();
        QObject qobject = new QObjectSubclass(counter, parent);
        parent.dispose();
        assertEquals(1, counter.disposedCount());
        assertTrue(qobject.isDisposed());
    }

    @Test
    public void testDestructionByDisposeLater_QObject()
    {
        DisposeCounter counter = new DisposeCounter();
        QObject qobject = new QObjectSubclass(counter, null);
        qobject.disposeLater();
        QApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());

        assertEquals(1, counter.disposedCount());
        assertTrue(qobject.isDisposed());
    }

    @Test
    public void testDestructionCppDelete_QObject()
    {
        DisposeCounter counter = new DisposeCounter();
        QObjectSubclass qobject = new QObjectSubclass(counter, null);
        QObjectDestroyed.deleteFromCpp(qobject);
        assertEquals(1, counter.disposedCount());
        assertTrue(qobject.isDisposed());
    }

    @Test
    public void testDestructionByVirtualDestructor_QObject(){
        DisposeCounter counter = new DisposeCounter();
        QObject qobject = new QObjectSubclass(counter, null);
        QObjectDestroyed.deleteFromCppOther(qobject);
        assertEquals(1, counter.disposedCount());
        assertTrue(qobject.isDisposed());
    }
    
    @Test
    public void testDestructionByJavaGC(){
        DisposeCounter counter = new DisposeCounter();
        {
            new OrdinarySubclass(counter);
        }
        try {
            for(int i = 0; i < 60; i++) {  
                ApplicationInitializer.runGC();
                synchronized(this) {
                    if(counter.disposedCount() != 0)
                        break;
                }
                Thread.sleep(10);
            }
        } catch(Exception e) {
            e.printStackTrace();
        }

        assertEquals(1, counter.disposedCount());
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestOwnership.class.getName());
    }
}

class OrdinarySubclass extends OrdinaryDestroyed {
    public OrdinarySubclass(DisposeCounter destroyCounter) {
    	super(destroyCounter);
    	io.qt.QtUtilities.getSignalOnDispose(this).connect(destroyCounter::onDisposed, Qt.ConnectionType.DirectConnection);
    }
}

class QObjectSubclass extends QObjectDestroyed {
    public QObjectSubclass(DisposeCounter counter, QObject parent) {
        super(counter, parent);
        destroyed.connect(counter::onDisposed, Qt.ConnectionType.DirectConnection);
    }
}