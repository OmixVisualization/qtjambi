/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.lang.ref.PhantomReference;
import java.lang.ref.Reference;
import java.lang.ref.ReferenceQueue;
import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

import io.qt.QtUtilities;
import io.qt.autotests.generated.MoccedObject;
import io.qt.autotests.generated.MoccedSub1Object;
import io.qt.autotests.generated.MoccedSub2Object;
import io.qt.autotests.generated.UnMoccedObject;
import io.qt.autotests.generated.UnMoccedSub1Object;
import io.qt.autotests.generated.UnMoccedSub2Object;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QFile;
import io.qt.core.QObject;
import io.qt.core.QRegularExpression;
import io.qt.core.QThread;
import io.qt.internal.TestUtility;
import io.qt.widgets.QApplication;

public class TestQObject extends ApplicationInitializer {
	
    private static final int TIMEOUT = 2000;
    private static final Utils.GC_MODE gcMode = Utils.GC_MODE.AGGRESIVE;

    private static class TestObject extends QObject {
        public TestObject(QObject parent) {
            super(parent);
        }
    }

    private QObject root;
    private QFile file1, file2;
    private TestObject test1, test2, child11, child12, child21, child22;

    @Before
    public void setUp() {
    	TestUtility.setObjectCacheMode(TestUtility.OBJECT_CACHE_MODE_DISABLE);

        root = new QObject();
        root.setObjectName("root");

        file1 = new QFile(root);
        file1.setObjectName("file1");

        file2 = new QFile(root);
        file2.setObjectName("file2");

        test1 = new TestObject(root);
        test1.setObjectName("test1");

        test2 = new TestObject(root);
        test2.setObjectName("test2");

        child11 = new TestObject(test1);
        child11.setObjectName("child11");

        child12 = new TestObject(test1);
        child12.setObjectName("child");

        child21 = new TestObject(test2);
        child21.setObjectName("child21");

        child22 = new TestObject(test2);
        child22.setObjectName("child");
    }

    @After
    public void tearDown() {
        root = null;
        file1 = null;
        file2 = null;
        test1 = null;
        test2 = null;
        child11 = null;
        child12 = null;
        child21 = null;
        child22 = null;
        // We need to undo this unusual setting to give any test cases that run after
        // within the same JVM a change of succeeding.
        TestUtility.setObjectCacheMode(TestUtility.OBJECT_CACHE_MODE_DEFAULT);
    }

    @Test
    public void run_children() {
        {
            List<QObject> c = root.findChildren();
            assertEquals(8, c.size());

            assertTrue(c.contains(file1));
            assertTrue(c.contains(file2));
            assertTrue(c.contains(test1));
            assertTrue(c.contains(test2));
            assertTrue(c.contains(child11));
            assertTrue(c.contains(child12));
            assertTrue(c.contains(child21));
            assertTrue(c.contains(child22));
        }

        {
            List<QFile> c = root.findChildren(QFile.class);
            assertEquals(c.size(), 2);

            assertTrue(c.contains(file1));
            assertTrue(c.contains(file2));
        }

        {
            List<QObject> c = root.findChildren(QObject.class, "child");

            assertEquals(c.size(), 2);
            assertTrue(c.contains(child12));
            assertTrue(c.contains(child22));
        }

        {
            List<QObject> c = root.findChildren(QObject.class, new QRegularExpression("child"));

            assertEquals(c.size(), 4);
            assertTrue(c.contains(child11));
            assertTrue(c.contains(child12));
            assertTrue(c.contains(child21));
            assertTrue(c.contains(child22));
        }
    }

    @Test
    public void run_findChild() {
        assertEquals(root.findChild(QFile.class, "file1"), file1);
        assertNotNull(root.findChild(QFile.class));
        assertNotNull(root.findChild());
    }
    
    public static class DyingObjectCounter {
        private ReferenceQueue<DyingObject> weakReferenceQueue = new ReferenceQueue<DyingObject>();
        private ReferenceQueue<DyingObject> phantomReferenceQueue = new ReferenceQueue<DyingObject>();
        private Map<WeakReference<DyingObject>,Integer> weakReferenceMap = new HashMap<WeakReference<DyingObject>,Integer>();
        private Map<PhantomReference<DyingObject>,Integer> phantomReferenceMap = new HashMap<PhantomReference<DyingObject>,Integer>();
        private List<Integer> alive = new ArrayList<Integer>();
        private List<Integer> aliveAndUnderTest = new ArrayList<Integer>();

        @SuppressWarnings("unused")
		public void waitForEmpty(int timeout) {
            // we need to wait for gc to collect the parent.
            long expireTime = 0;
            if(timeout >= 0)
                expireTime = System.currentTimeMillis() + timeout;
            java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINER, " waitForEmpty(timeout=" + timeout + "; expireTime=" + expireTime + ")");

            int aliveSize;
            int aliveAndUnderTestSize;
            int weakReferenceMapSize;
            int phantomReferenceMapSize;
            while(true) {
//                Utils.releaseNativeResources();
                Utils.gc(gcMode, 3);

                Reference<? extends DyingObject> thisWr;
                while((thisWr = weakReferenceQueue.poll()) != null) {
                    Integer tmpId;
                    synchronized(DyingObject.class) {
                        tmpId = weakReferenceMap.remove(thisWr);
                    }
                    java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINER, " weakReferenceQueue.remove(): dequeued id=" + tmpId);
                }
                Reference<? extends DyingObject> thisPr;
                while((thisPr = phantomReferenceQueue.poll()) != null) {
                    Integer tmpId;
                    synchronized(DyingObject.class) {
                        tmpId = phantomReferenceMap.remove(thisPr);
                    }
                    java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINER, " phantomReferenceQueue.remove(): dequeued id=" + tmpId);
                }

                // Re-evaluate state
                aliveSize = -1;
                aliveAndUnderTestSize = -1;
                weakReferenceMapSize = -1;
                phantomReferenceMapSize = -1;
                synchronized(DyingObject.class) {
                    final int len = aliveAndUnderTest.size();
//                    int i;
//                    for(i = 0; i < len; i++) {
//                        Integer integerObject = aliveAndUnderTest.get(i);
//                    }
                    aliveSize = alive.size();
                    aliveAndUnderTestSize = len;
                    weakReferenceMapSize = weakReferenceMap.size();
                    phantomReferenceMapSize = phantomReferenceMap.size();
                }
                if(aliveAndUnderTestSize == 0)
                    return;	// we're done

                long left = 0;
                long timeNow = System.currentTimeMillis();
                if(expireTime > timeNow)
                    left = expireTime - timeNow;
                if(left <= 100)
                    java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINER, " timeout=" + timeout + "; expireTime=" + expireTime + "; left=" + left);
                if(left <= 0)
                    break;	// expireTime met or exceeded
                try {
                    if(left > 10)
                        left = 10;	// clamp
                    Thread.sleep(left);
                } catch (Exception eat) {
                }
            }

            // Report on status
            java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, ()->"alive=" + alive + "; aliveAndUnderTest=" + aliveAndUnderTest +
                        "; weakReferenceMap=" + weakReferenceMap.values() + "; phantomReferenceMap=" + phantomReferenceMap.values());
        }
        public void clear() {
            synchronized(DyingObject.class) {
                aliveAndUnderTest.clear();
            }
        }
        public int getSize() {
            synchronized(DyingObject.class) {
                return aliveAndUnderTest.size();
            }
        }

        private void oneObject() {
            oneObject_helper();
            Utils.gc(gcMode);
        }

        private void oneObject_helper() {
            DyingObject tmp = new DyingObject(this, "oneObject_helper");
            assertEquals(1, getSize());
            noop(tmp);
            tmp = null;
        }

        private void objectWithParent() {
            objectWithParent_helper();
            Utils.gc(gcMode);
            "".chars();
        }

        private void objectWithParent_helper() {
            DyingObject tmp = new DyingObject(this, "objectWithParent_helper child");
            DyingObject tmp2 = new DyingObject(this, "objectWithParent_helper parent");
            tmp.setParent(tmp2);
            noop(tmp);
            assertEquals(2, this.getSize());
            tmp = null;
            tmp2 = null;
        }

        private void objectWithUnParent() {
            objectWithUnParent_helper();
            Utils.gc(gcMode);
        }

        private void objectWithUnParent_helper() {
            DyingObject p = new DyingObject(this, "objectWithUnParent_helper parent");
            DyingObject o = new DyingObject(this, "objectWithUnParent_helper unparented child");
            o.setParent(p);
            o.setParent(null);
            assertEquals(2, this.getSize());
            p = null;
            o = null;
        }

        private Runnable runnable_disposeInThread = () -> new DyingObject(this, "runnable_disposeInThread").dispose();

        private Runnable runnable_oneObject = this::oneObject;

        private Runnable runnable_objectWithParent = this::objectWithParent;

        private Runnable runnable_objectWithUnParent = this::objectWithUnParent;
    }

    public static class DyingObject extends QObject {
        public DyingObject(DyingObjectCounter counter, String objectName) {
        	this.setObjectName(objectName);
        	this.objectName = objectName;
        	instances.add(new WeakReference<>(this));
            java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "DyingObject#ctor() thread=" + thread() + "; id=" + id);

            WeakReference<DyingObject> wr = new WeakReference<DyingObject>(this, counter.weakReferenceQueue);
            PhantomReference<DyingObject> pr = new PhantomReference<DyingObject>(this, counter.phantomReferenceQueue);
            synchronized(DyingObject.class) {
            	counter.weakReferenceMap.put(wr, id);
            	counter.phantomReferenceMap.put(pr, id);
            	counter.aliveAndUnderTest.add(id);
            	counter.alive.add(id);
            }
            Object _id = id;
            QtUtilities.getSignalOnDispose(this).connect(()->{
            	java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "DyingObject#dtor() id=" + _id);
            	synchronized(DyingObject.class) {
            		counter.aliveAndUnderTest.remove(_id);
            		counter.alive.remove(_id);
                }
            });
        }

        // This should be WeakReference/SoftReference
        private static int counter = 0;

        public final Integer id = getIdNext();
        public final String objectName;

        public static Integer getIdNext() {
            int idNext;
            synchronized(DyingObject.class) {
                counter++;
                idNext = counter;
            }
            return Integer.valueOf(idNext);
        }
    }

    private static void noop(Object o) {
        if(o != null)
            o.toString();
    }

    private static void threadExecutor(Runnable r, boolean qthread) {
    	if(qthread) {
    		QThread t = QThread.create(r);
    		t.setObjectName("threadExecutor");
            t.start();
            try {
                t.join(1000);
            } catch (Exception e) { e.printStackTrace(); }
    	}else {
    		Thread t = new Thread(r);
    		t.setName("threadExecutor");
            t.start();
            try {
                t.join(1000);
            } catch (Exception e) { e.printStackTrace(); }
    	}
    }
    
    private static final int eventype = QEvent.registerEventType();
    
    private static QThread indirectThreadExecutor(final Runnable r, String testName){
		QThread nativeThread = new QThread();
		nativeThread.setObjectName("QThread_"+testName);
		nativeThread.start();
		final int[] counter = {0};
        QObject execObject = new QObject(){
			@Override
			public boolean event(QEvent event) {
				if(event.type().value()==eventype){
					synchronized(counter){
						counter[0]++;
						r.run();
						counter[0]++;
						counter.notifyAll();
					}
					event.accept();
					return true;
				}
				return super.event(event);
			}
        };
        execObject.setObjectName(execObject.getClass().getName()+"@"+testName);
        execObject.moveToThread(nativeThread);
        synchronized(counter){
        	try {
        		QApplication.postEvent(execObject, new QEvent(QEvent.Type.resolve(eventype)));
        		counter.wait(1000);
			} catch (Exception e) {
				e.printStackTrace();
				Assert.fail(e.getMessage());
			}
        }
        synchronized(counter){
        	assertEquals(2, counter[0]);
        }
        return nativeThread;
    }

    @Test
    public void disposal_oneObject() {
    	System.out.println("TestQObject.disposal_oneObject() BEGIN");
    	DyingObjectCounter counter = new DyingObjectCounter();
    	counter.oneObject();

        // we need to wait for gc to collect the parent.
        counter.waitForEmpty(TIMEOUT);
        System.out.println("TestQObject.disposal_oneObject() END");
        assertEquals("alive and under test", 0, counter.getSize());
    }

    @Test
    public void disposal_objectWithParent() {
    	DyingObjectCounter counter = new DyingObjectCounter();
    	counter.objectWithParent();

        // we need to wait for gc to collect the parent.
        counter.waitForEmpty(TIMEOUT);

        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
        assertEquals("alive and under test", 0, counter.getSize());
    }

    @Test
    public void disposal_objectWithUnParent() {
    	DyingObjectCounter counter = new DyingObjectCounter();
    	counter.objectWithUnParent();
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());

        // we need to wait for gc to collect the parent.
        counter.waitForEmpty(TIMEOUT);

        assertEquals("alive and under test", 0, counter.getSize());
    }

    @Test
    public void disposal_disposeInThread() {
    	DyingObjectCounter counter = new DyingObjectCounter();
        threadExecutor(counter.runnable_disposeInThread, false);
        counter.waitForEmpty(TIMEOUT);
        assertEquals("alive and under test", 0, counter.getSize());

        threadExecutor(counter.runnable_disposeInThread, true);
        counter.waitForEmpty(TIMEOUT);
        assertEquals("alive and under test", 0, counter.getSize());
        
        QThread nativeThread = indirectThreadExecutor(counter.runnable_disposeInThread, "disposal_disposeInThread");
        counter.waitForEmpty(TIMEOUT);
        assertEquals("alive and under test", 0, counter.getSize());
        nativeThread.quit();
    }

    @Test
    public void disposal_gcInQThread_oneObject() {
    	DyingObjectCounter counter = new DyingObjectCounter();
        threadExecutor(counter.runnable_oneObject, true);
        counter.waitForEmpty(TIMEOUT);
        assertEquals("alive and under test", 0, counter.getSize());
    }

    @Test
    public void disposal_gcInQThread_objectWithParent() {
    	DyingObjectCounter counter = new DyingObjectCounter();
        threadExecutor(counter.runnable_objectWithParent, true);

        // we need to wait for gc to collect the parent.
        counter.waitForEmpty(TIMEOUT);

        assertEquals("alive and under test", 0, counter.getSize());
    }

    @Test
    public void disposal_gcInQThread_objectWithUnParent() {
    	DyingObjectCounter counter = new DyingObjectCounter();
        threadExecutor(counter.runnable_objectWithUnParent, true);

        // we need to wait for gc to collect the parent.
        counter.waitForEmpty(TIMEOUT);

        assertEquals("alive and under test", 0, counter.getSize());
    }
    
    @Test
    public void eventsPostedInNativeQThread() {
    	int eventype = QEvent.registerEventType();
    	QThread nativeThread = new QThread();
    	nativeThread.setObjectName("QThread_eventsPostedInNativeQThread");
    	nativeThread.start();
    	final int[] counter = {0};
        QObject execObject = new QObject(){
			@Override
			public boolean event(QEvent event) {
				if(event.type().value()==eventype){
					synchronized(counter){
						counter[0]++;
						counter.notifyAll();
					}
				}
				return super.event(event);
			}
        };
        execObject.setObjectName("eventsPostedInNativeQThread");
        execObject.moveToThread(nativeThread);
        
        synchronized(counter){
        	try {
        		QApplication.postEvent(execObject, new QEvent(QEvent.Type.resolve(eventype)));
        		counter.wait(1000);
			} catch (Exception e) {
				e.printStackTrace();
			}
        }
        nativeThread.quit();
        synchronized(counter){
        	assertEquals(1, counter[0]);
        }
    }
    
    @Test
    public void disposal_gcInEventQThread_oneObject() {
        DyingObjectCounter counter = new DyingObjectCounter();
        QThread nativeThread = indirectThreadExecutor(counter.runnable_oneObject, "disposal_gcInEventQThread_oneObject");
        counter.waitForEmpty(TIMEOUT);
        assertEquals("alive and under test", 0, counter.getSize());
        nativeThread.quit();
    }

    @Test
    public void disposal_gcInEventQThread_objectWithParent() {
        DyingObjectCounter counter = new DyingObjectCounter();
        QThread nativeThread = indirectThreadExecutor(counter.runnable_objectWithParent, "disposal_gcInEventQThread_objectWithParent");
        
        // we need to wait for gc to collect the parent.
        counter.waitForEmpty(TIMEOUT);
        
        assertEquals("alive and under test", 0, counter.getSize());
        
        nativeThread.quit();
    }

    @Test
    public void disposal_gcInEventQThread_objectWithUnParent() {
        DyingObjectCounter counter = new DyingObjectCounter();
        QThread nativeThread = indirectThreadExecutor(counter.runnable_objectWithUnParent, "disposal_gcInEventQThread_objectWithUnParent");

        // we need to wait for gc to collect the parent.
        counter.waitForEmpty(TIMEOUT);

        assertEquals("alive and under test", 0, counter.getSize());
        nativeThread.quit();
    }

    @Test
    public void disposal_gcInThread_oneObject() {
        // Will warn about leaking the C++ object.
        DyingObjectCounter counter = new DyingObjectCounter();
        threadExecutor(counter.runnable_oneObject, false);
        counter.waitForEmpty(TIMEOUT);
        assertEquals("alive and under test", 0, counter.getSize());
    }

    @Test
    public void disposal_gcInThread_objectWithParent() {
        // Will warn once about leaking the C++ object. The child will not be collected
        DyingObjectCounter counter = new DyingObjectCounter();
        threadExecutor(counter.runnable_objectWithParent, false);

        try {
            Thread.sleep(TIMEOUT); // make sure we do an effort to make this test fail.
        } catch (Exception e) {
            e.printStackTrace();
        }
        assertEquals("alive and under test", 0, counter.getSize());
    }

    @Test
    public void disposal_gcInThread_objectWithUnParent() {
        // Will warn twice about leaking the C++ object, but both objects will be collected
        DyingObjectCounter counter = new DyingObjectCounter();
        threadExecutor(counter.runnable_objectWithUnParent, false);

        // we need to wait for gc to collect the parent.
        counter.waitForEmpty(TIMEOUT);

        assertEquals("alive and under test", 0, counter.getSize());
    }
    
    @Test
    public void testUnMocced() {
    	QObject object = UnMoccedObject.create(1);
    	assertTrue(object!=null);
    	assertEquals(MoccedObject.class, object.getClass());
    	object = UnMoccedObject.create(2);
    	assertTrue(object!=null);
    	assertEquals(MoccedSub1Object.class, object.getClass());
    	object = UnMoccedObject.create(3);
    	assertTrue(object!=null);
    	assertEquals(MoccedSub2Object.class, object.getClass());
    	object = UnMoccedObject.create(4);
    	assertTrue(object!=null);
    	assertEquals(MoccedSub1Object.class, object.getClass());
    	object = UnMoccedObject.create(5);
    	assertTrue(object!=null);
    	assertEquals(MoccedSub2Object.class, object.getClass());
    	
    	object = UnMoccedObject.create(-1);
    	assertTrue(object!=null);
    	assertEquals(UnMoccedObject.class, object.getClass());
    	object = UnMoccedObject.create(-2);
    	assertTrue(object!=null);
    	assertEquals(UnMoccedSub1Object.class, object.getClass());
    	object = UnMoccedObject.create(-3);
    	assertTrue(object!=null);
    	assertEquals(UnMoccedSub2Object.class, object.getClass());
    	object = UnMoccedObject.create(-4);
    	assertTrue(object!=null);
    	assertEquals(UnMoccedSub1Object.class, object.getClass());
    	object = UnMoccedObject.create(-5);
    	assertTrue(object!=null);
    	assertEquals(UnMoccedSub2Object.class, object.getClass());
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQObject.class.getName());
    }
}
