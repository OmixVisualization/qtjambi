/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
import static org.junit.Assert.assertNotNull;

import java.lang.ref.PhantomReference;
import java.lang.ref.Reference;
import java.lang.ref.ReferenceQueue;
import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import io.qt.autotests.generated.DestroyCounter;
import io.qt.autotests.generated.OrdinaryDestroyed;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QThread;
import io.qt.gui.QTextCursor;
import io.qt.gui.QTextDocument;
import io.qt.internal.QtJambiDebugTools;
import io.qt.widgets.QApplication;

class MyOrdinaryDestroyed extends OrdinaryDestroyed {
    public final Integer id;

    MyOrdinaryDestroyed(DisposeCounter counter, Integer id) {
    	super(counter);
        this.id = id;
        io.qt.QtUtilities.getSignalOnDispose(this).connect(( (Runnable) counter::onDisposed )::run);
        Utils.println(5, "MyOrdinaryDestroyed.ctor() " + getClass().getName() + "@" + System.identityHashCode(this) + "; thread=" + Thread.currentThread().getId() + "; id=" + id);
    }

    public Integer getId() {
        return id;
    }

    @Override
    public OrdinaryDestroyed virtualGetObjectCppOwnership(DestroyCounter counter) {
        MyOrdinaryDestroyed o = new MyOrdinaryDestroyed((DisposeCounter)counter, TestDestruction.getIdNext());
        TestDestruction.accountingForNewObject(o);
        return o;
    }

    @Override
    public OrdinaryDestroyed virtualGetObjectJavaOwnership(DestroyCounter counter) {
        MyOrdinaryDestroyed o = new MyOrdinaryDestroyed((DisposeCounter)counter, TestDestruction.getIdNext());
        TestDestruction.accountingForNewObject(o);
        return o;
    }

    @Override
    public void virtualSetDefaultOwnership(OrdinaryDestroyed arg__1) {
    	TestDestruction.accountingForNewObject(arg__1);
        // nothing
    }
}

public class TestDestruction extends QApplicationTest {
    private static ReferenceQueue<OrdinaryDestroyed> weakReferenceQueue = new ReferenceQueue<>();
    private static ReferenceQueue<OrdinaryDestroyed> phantomReferenceQueue = new ReferenceQueue<>();
    private static Map<WeakReference<OrdinaryDestroyed>,Integer> weakReferenceMap = new HashMap<>();
    private static Map<PhantomReference<OrdinaryDestroyed>,Integer> phantomReferenceMap = new HashMap<>();
    private static List<Integer> alive = new ArrayList<Integer>();
    private static List<Integer> aliveAndUnderTest = new ArrayList<Integer>();
    private static int counter = 0;

    @Before
    public void setUp() {
        // This class is known to fail when we messed with this setting in a previous testcase running in the same JVM
        assertEquals("getObjectCacheMode != DEFAULT", QtJambiDebugTools.getObjectCacheMode(), QtJambiDebugTools.OBJECT_CACHE_MODE_DEFAULT);
        QApplication.processEvents();
        clearGcReferences();
    }

    @After
    public void tearDown() {
        QApplication.processEvents();
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());

        System.gc();
        System.runFinalization();
    }

    public static Integer getIdNext() {
        int idNext;
        synchronized(TestDestruction.class) {
            counter++;
            idNext = counter;
        }
        return Integer.valueOf(idNext);
    }
    public void clearGcReferences() {
        synchronized(TestDestruction.class) {
            aliveAndUnderTest.clear();
        }
    }
    public int getSize() {
        synchronized(TestDestruction.class) {
            return aliveAndUnderTest.size();
        }
    }

    protected static void accountingForNewObject(MyOrdinaryDestroyed o) {
        assertNotNull(o);

        Integer id = o.getId();

        accountingForNewObject(o, id);
    }

    static void accountingForNewObject(OrdinaryDestroyed o) {
        assertNotNull(o);

        accountingForNewObject(o, null);
    }

    // This does not have "id" field, so make one up for it
    private static void accountingForNewObject(OrdinaryDestroyed o, Integer id) {
        assertNotNull(o);

        if(id == null)
            id = getIdNext();

        if(Utils.isDebugLevel(4)) {
            String className = o.getClass().getName();
            String shortClassName = className;
            int i = shortClassName.lastIndexOf('.');
            if(i > 0)
                shortClassName = shortClassName.substring(i + 1);
            Utils.println(4, shortClassName + ".ctor " + className + "@" + System.identityHashCode(o) + "; thread=" + Thread.currentThread().getId() + "; id=" + id);
        }
        WeakReference<OrdinaryDestroyed> wr = new WeakReference<>(o, weakReferenceQueue);
        PhantomReference<OrdinaryDestroyed> pr = new PhantomReference<>(o, phantomReferenceQueue);
        synchronized(TestDestruction.class) {
            weakReferenceMap.put(wr, id);
            phantomReferenceMap.put(pr, id);
            aliveAndUnderTest.add(id);
            alive.add(id);
        }
        QApplicationTest.instances.add(new WeakReference<>(o));
    }

    @SuppressWarnings("unused")
	private boolean gcAndWait(DisposeCounter counter, long timeLimitMillis) {
        return gcAndWait(counter, timeLimitMillis, null, null, null);
    }

    private boolean gcAndWait(DisposeCounter counter, long timeLimitMillis, Long excessLimitMillis, Integer disposedCount, Integer destroyedCount) {
        boolean obtainGoal = false;	// did we meet conditions within timelimit?

        long startTime = System.currentTimeMillis();
        long excessTime = 0;
        long elapsed = 0;

        int currentDisposedCount;
        int currentDestroyedCount;
        boolean loop = true;
        String debugPrefix = " gcAndWait(" + timeLimitMillis +
            ", " + Utils.toStringOrNull(excessLimitMillis) +
            ", " + Utils.toStringOrNull(disposedCount) +
            ", " + Utils.toStringOrNull(destroyedCount) + ")";
        while(loop) {
            try {
//                if(needsEventProcessing()) {
//                    Utils.println(5, debugPrefix + ": QApplication.processEvents()");
//                    QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
////                    QApplication.processEvents();
//                }

                System.gc();
                System.runFinalization();

                Reference<? extends Object> thisWr;
                while((thisWr = weakReferenceQueue.poll()) != null) {
                    Integer tmpId;
                    synchronized(TestDestruction.class) {
                        tmpId = weakReferenceMap.remove(thisWr);
                    }
                    Utils.println(5, " weakReferenceQueue.remove(): dequeued id=" + tmpId);
                }
                Reference<? extends Object> thisPr;
                while((thisPr = phantomReferenceQueue.poll()) != null) {
                    Integer tmpId;
                    synchronized(TestDestruction.class) {
                        tmpId = phantomReferenceMap.remove(thisPr);
                    }
                    Utils.println(5, " phantomReferenceQueue.remove(): dequeued id=" + tmpId);
                }

                if(!obtainGoal) {	// Try to obtainGoal
                    currentDisposedCount = counter.disposedCount();
                    currentDestroyedCount = counter.destroyedCount();

                    boolean loopDisposedCount = false;
                    if(disposedCount != null) {
                        if(disposedCount.intValue() > currentDisposedCount)
                            loopDisposedCount = true;
                    }
                    boolean loopDestroyedCount = false;
                    if(destroyedCount != null) {
                        if(destroyedCount.intValue() > currentDestroyedCount)
                            loopDestroyedCount = true;
                    }

                    if(!loopDisposedCount && !loopDestroyedCount) {
                        obtainGoal = true;	// we have met the callers goals
                        excessTime = System.currentTimeMillis();	// reset this for excessLimitMillis
                    }

                    Utils.println(5, debugPrefix + ": elapsed=" + elapsed + "; end loop" +
                        "; currentDisposedCount=" + currentDisposedCount +
                        "; currentDestroyedCount=" + currentDestroyedCount +
                        "; obtainGoal=" + obtainGoal
                        );
                }

                if(obtainGoal) {
                    elapsed = System.currentTimeMillis() - excessTime;
                    if(excessLimitMillis != null)
                        loop = elapsed < excessLimitMillis.intValue();
                    else
                        loop = false;
                } else {
                    elapsed = System.currentTimeMillis() - startTime;
                    loop = elapsed < timeLimitMillis;
                }
Utils.println(15, debugPrefix + ": elapsed=" + elapsed + "; loop="+loop+"; obtainGoal="+obtainGoal);

                if(loop)
                    Thread.sleep(10);
            } catch(Exception e) {	// exceptions are an idiotic concept; yeah in c++
                e.printStackTrace();
            }
        }

        // Report on status
        if(Utils.isDebugLevel(4)) {
            // Print array format [1, 2, 3]
            synchronized(TestDestruction.class) {
                String aliveString = Utils.listToString(alive);
                String aliveAndUnderTestString = Utils.listToString(aliveAndUnderTest);
                String weakReferenceMapString = Utils.mapValueToString(weakReferenceMap);
                String phantomReferenceMapString = Utils.mapValueToString(phantomReferenceMap);
                Utils.println(4, "elapsed=" + elapsed + "; alive=" + aliveString + "; aliveAndUnderTest=" + aliveAndUnderTestString +
                        "; weakReferenceMapSize=" + weakReferenceMapString + "; phantomReferenceMapSize=" + phantomReferenceMapString);
            }
        }

        return obtainGoal;
    }

    // method exists to ensure object reference created here is discarded for GC
    private void testJavaCreationJavaOwnership_internal(DisposeCounter counter) {
        MyOrdinaryDestroyed d = new MyOrdinaryDestroyed(counter, getIdNext());

        accountingForNewObject(d);
    }

    @Test
    public void testJavaCreationJavaOwnership() {
    	DisposeCounter counter = new DisposeCounter();
        testJavaCreationJavaOwnership_internal(counter);

        gcAndWait(counter, 500, (long)50, 1, 1);

        assertEquals("disposed count", 1, counter.disposedCount());
        assertEquals("destroyed count", 1, counter.destroyedCount());
    }

    // method exists to ensure object reference created here is discarded for GC
    private void testJavaCreationCppOwnership_internal(DisposeCounter counter, MyOrdinaryDestroyed dontBeDeleted) {
        // Garbage collection has now been disabled on d
        OrdinaryDestroyed d = MyOrdinaryDestroyed.callGetObjectCppOwnership(counter, dontBeDeleted);

        accountingForNewObject(d);
    }

    // Method requires: getObjectCacheMode == DEFAULT
    @Test
    public void testJavaCreationCppOwnership() {
        DisposeCounter counter = new DisposeCounter();

        MyOrdinaryDestroyed dontBeDeleted = new MyOrdinaryDestroyed(counter, getIdNext());
        accountingForNewObject(dontBeDeleted);

        testJavaCreationCppOwnership_internal(counter, dontBeDeleted);

        gcAndWait(counter, 500, (long)50, 0, 0);		// Need to give it all the time

        assertEquals("disposed count", 0, counter.disposedCount());
        assertEquals("destroyed count", 0, counter.destroyedCount());

        // CHECKME: Can we undo that hold and test for release?
    }

    // method exists to ensure object reference created here is discarded for GC
    private void testJavaCreationDefaultOwnershipThroughNative_internal(DisposeCounter counter, MyOrdinaryDestroyed dontBeDeleted) {
        // Garbage collection has now been disabled on d
        OrdinaryDestroyed d = MyOrdinaryDestroyed.callGetObjectCppOwnership(counter, dontBeDeleted);

        // Set default ownership on d (should be "java")
        MyOrdinaryDestroyed.setDefaultOwnership(d);

        accountingForNewObject(d);
    }

    // Method requires: getObjectCacheMode == DEFAULT
    @Test
    public void testJavaCreationDefaultOwnershipThroughNative() {
        DisposeCounter counter = new DisposeCounter();

        MyOrdinaryDestroyed dontBeDeleted = new MyOrdinaryDestroyed(counter, getIdNext());
        accountingForNewObject(dontBeDeleted);

        testJavaCreationDefaultOwnershipThroughNative_internal(counter, dontBeDeleted);

        gcAndWait(counter, 500, (long)50, 1, 1);

        assertEquals("disposed count", 1, counter.disposedCount());
        assertEquals("destroyed count", 1, counter.destroyedCount());
    }

    // method exists to ensure object reference created here is discarded for GC
    private void testJavaCreationDefaultOwnershipThroughShell_internal(DisposeCounter counter, MyOrdinaryDestroyed dontBeDeleted) {
        // Garbage collection has now been disabled on d
        OrdinaryDestroyed d = MyOrdinaryDestroyed.callGetObjectCppOwnership(counter, dontBeDeleted);

        // Set default ownership on d (should be "java")
        MyOrdinaryDestroyed.callSetDefaultOwnership(dontBeDeleted, d);

        accountingForNewObject(d);
    }

    // Method requires: getObjectCacheMode == DEFAULT
    @Test
    public void testJavaCreationDefaultOwnershipThroughShell() {
        DisposeCounter counter = new DisposeCounter();

        MyOrdinaryDestroyed dontBeDeleted = new MyOrdinaryDestroyed(counter, getIdNext());
        accountingForNewObject(dontBeDeleted);

        testJavaCreationDefaultOwnershipThroughShell_internal(counter, dontBeDeleted);

        gcAndWait(counter, 500, (long)50, 1, 1);

        assertEquals("disposed count", 1, counter.disposedCount());
        assertEquals("destroyed count", 1, counter.destroyedCount());
    }

    // method exists to ensure object reference created here is discarded for GC
    private void testCppCreationSplitOwnership_internal(DisposeCounter counter) {
        // Split ownership: Java object should be collected, but the c++ object should not be baleeted
        OrdinaryDestroyed d = MyOrdinaryDestroyed.getObjectSplitOwnership(counter);
        io.qt.QtUtilities.getSignalOnDispose(d).connect(( (Runnable) counter::onDisposed )::run);

        accountingForNewObject(d);
    }

    @Test
    public void testCppCreationSplitOwnership() {
        DisposeCounter counter = new DisposeCounter();

        testCppCreationSplitOwnership_internal(counter);

        gcAndWait(counter, 500, (long)50, 1, 0);	// Need to give it time for destroy

        assertEquals("disposed count", 1, counter.disposedCount());
        assertEquals("destroyed count", 0, counter.destroyedCount());

        // FIXME: Undo what we did above and ensure it does actually go then
    }

    // method exists to ensure object reference created here is discarded for GC
    private void testCppCreationCppOwnership_internal(DisposeCounter counter) {
        OrdinaryDestroyed d = MyOrdinaryDestroyed.getObjectCppOwnership(counter);

        accountingForNewObject(d);
    }

    @Test
    public void testCppCreationCppOwnership() {
        DisposeCounter counter = new DisposeCounter();

        testCppCreationCppOwnership_internal(counter);

        gcAndWait(counter, 500, (long)50, 0, 0);		// Need to give it all the time

        assertEquals("disposed count", 0, counter.disposedCount());
        assertEquals("destroyed count", 0, counter.destroyedCount());

        // FIXME: Undo what we did above and ensure it does actually go then
    }

    // method exists to ensure object reference created here is discarded for GC
    private void testCppCreationJavaOwnership_internal(DisposeCounter counter) {
        OrdinaryDestroyed d = MyOrdinaryDestroyed.getObjectJavaOwnership(counter);
        io.qt.QtUtilities.getSignalOnDispose(d).connect(( (Runnable) counter::onDisposed )::run);

        accountingForNewObject(d);
    }

    @Test
    public void testCppCreationJavaOwnership() {
        DisposeCounter counter = new DisposeCounter();

        testCppCreationJavaOwnership_internal(counter);

        gcAndWait(counter, 500, (long)50, 1, 1);

        assertEquals("disposed count", 1, counter.disposedCount());
        assertEquals("destroyed count", 1, counter.destroyedCount());
    }

    // method exists to ensure object reference created here is discarded for GC
    private void testCppCreationDefaultOwnershipThroughShell_internal(DisposeCounter counter, MyOrdinaryDestroyed dontBeDeleted) {
        // Garbage collection has now been disabled on d
        OrdinaryDestroyed d = MyOrdinaryDestroyed.getObjectCppOwnership(counter);

        // Set default ownership on d (should be "split")
        MyOrdinaryDestroyed.callSetDefaultOwnership(dontBeDeleted, d);

        accountingForNewObject(d);
    }

    @Test
    public void testCppCreationDefaultOwnershipThroughShell() {
        DisposeCounter counter = new DisposeCounter();

        MyOrdinaryDestroyed dontBeDeleted = new MyOrdinaryDestroyed(counter, getIdNext());
        accountingForNewObject(dontBeDeleted);

        testCppCreationDefaultOwnershipThroughShell_internal(counter, dontBeDeleted);

        gcAndWait(counter, 500, (long)50, 1, 0);	// Need to give it time for destroy

        assertEquals("disposed count", 0, counter.disposedCount());
        assertEquals("destroyed count", 0, counter.destroyedCount());

        // FIXME: Undo what we did above and ensure it does actually go then
    }

    // method exists to ensure object reference created here is discarded for GC
    private void testCppCreationDefaultOwnershipThroughNative_internal(DisposeCounter counter, MyOrdinaryDestroyed dontBeDeleted) {
        // Garbage collection has now been disabled on d
        OrdinaryDestroyed d = MyOrdinaryDestroyed.getObjectCppOwnership(counter);

        // Set default ownership on d (should be "split")
        MyOrdinaryDestroyed.setDefaultOwnership(d);

        accountingForNewObject(d);
    }

    @Test
    public void testCppCreationDefaultOwnershipThroughNative() {
        DisposeCounter counter = new DisposeCounter();

        MyOrdinaryDestroyed dontBeDeleted = new MyOrdinaryDestroyed(counter, getIdNext());
        accountingForNewObject(dontBeDeleted);

        testCppCreationDefaultOwnershipThroughNative_internal(counter, dontBeDeleted);
        
        gcAndWait(counter, 500, (long)50, 1, 0);	// Need to give it time for destory

        assertEquals("disposed count", 0, counter.disposedCount());
        assertEquals("destroyed count", 0, counter.destroyedCount());

        if(counter.disposedCount() > 1) {    // PS. Expression never true here.
            System.err.println("Avoid reordering of code which may cause dontBeDeleted to be deleted: "
                                + dontBeDeleted);
        }
    }
    
    @Test
    public void testOwnedValueObjectThreadedDisposeWithoutCrash() throws InterruptedException {
    	for (int k = 0; k < 20; k++) {
    		Utils.println(5, "running test "+(k+1));
    		QTextDocument document = new QTextDocument();
        	List<QThread> threads = new ArrayList<>();
        	List<List<QTextCursor>> threadDatas = new ArrayList<>();
        	int numberOfThreads = 20;
        	int numberOfCursors = 1000;
        	Utils.println(5, "create QTextCursors");
        	for (int i = 0; i < numberOfThreads; i++) {
        		List<QTextCursor> cursors = new ArrayList<>();
        		for (int j = 0; j < numberOfCursors; j++) {
        			QTextCursor c = new QTextCursor(document);
        			cursors.add(c);
        			cursors.add(c.clone());
    			}
        		threadDatas.add(cursors);
    		}
        	Utils.println(5, "create deleter threads");
        	for (List<QTextCursor> cursors : threadDatas) {
        		threads.add(QThread.create(()->{
        			for(QTextCursor c : cursors) {
        				c.dispose();
        		    	QThread.yieldCurrentThread();
        			}
        		}));
    		}
        	Utils.println(5, "start deleter threads");
        	for (QThread thread : threads) {
    			thread.start();
    		}
        	QThread.sleep(2);
        	Utils.println(5, "delete QTextDocument");
        	document.dispose();
        	QThread.yieldCurrentThread();
        	Utils.println(5, "wait for finished deleting");
	    	System.gc();
	    	System.runFinalization();
    		QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
        	for (QThread thread : threads) {
    			thread.join();
    		}
		}
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestDestruction.class.getName());
    }
}
