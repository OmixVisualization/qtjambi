/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package org.qtjambi.autotests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

import java.lang.ref.Reference;
import java.lang.ref.ReferenceQueue;
import java.lang.ref.PhantomReference;
import java.lang.ref.WeakReference;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.qtjambi.autotests.generated.OrdinaryDestroyed;
import org.qtjambi.qt.core.QCoreApplication;
import org.qtjambi.qt.core.QEvent;
import org.qtjambi.qt.core.QEventLoop;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.internal.QtJambiRuntime;

class MyOrdinaryDestroyed extends OrdinaryDestroyed {
    public final Integer id;

    MyOrdinaryDestroyed(Integer id) {
        this.id = id;
        Utils.println(5, "MyOrdinaryDestroyed.ctor() " + getClass().getName() + "@" + System.identityHashCode(this) + "; thread=" + Thread.currentThread().getId() + "; id=" + id);
    }

    public Integer getId() {
        return id;
    }

    @Override
    public OrdinaryDestroyed virtualGetObjectCppOwnership() {
        MyOrdinaryDestroyed o = new MyOrdinaryDestroyed(TestDestruction.getIdNext());
        TestDestruction.accountingForNewObject(o);
        return o;
    }

    @Override
    public OrdinaryDestroyed virtualGetObjectJavaOwnership() {
        MyOrdinaryDestroyed o = new MyOrdinaryDestroyed(TestDestruction.getIdNext());
        TestDestruction.accountingForNewObject(o);
        return o;
    }

    @Override
    public void virtualSetDefaultOwnership(OrdinaryDestroyed arg__1) {
        // nothing
    }

    @Override
    protected void finalize() {
        try {
            Utils.println(5, "MyOrdinaryDestroyed.dtor() " + getClass().getName() + "@" + System.identityHashCode(this) + "; thread=" +  Thread.currentThread().getId() + "; id=" + id);
        } finally {
            super.finalize();
        }
    }
}

public class TestDestruction extends QApplicationTest {
    private static ReferenceQueue<Object> weakReferenceQueue = new ReferenceQueue<Object>();
    private static ReferenceQueue<Object> phantomReferenceQueue = new ReferenceQueue<Object>();
    private static Map<WeakReference<Object>,Integer> weakReferenceMap = new HashMap<WeakReference<Object>,Integer>();
    private static Map<PhantomReference<Object>,Integer> phantomReferenceMap = new HashMap<PhantomReference<Object>,Integer>();
    private static List<Integer> alive = new ArrayList<Integer>();
    private static List<Integer> aliveAndUnderTest = new ArrayList<Integer>();
    private static int counter = 0;

    @Before
    public void setUp() {
        // This class is known to fail when we messed with this setting in a previous testcase running in the same JVM
        assertEquals("getObjectCacheMode != DEFAULT", QtJambiRuntime.getObjectCacheMode(), QtJambiRuntime.OBJECT_CACHE_MODE_DEFAULT);
        QApplication.processEvents();
        clearGcReferences();
    }

    @After
    public void tearDown() {
        QApplication.processEvents();
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDelete.value());

        System.gc();
        System.runFinalization();
        if(Utils.releaseNativeResources() > 0) {
            System.gc();
            System.runFinalization();
        }
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

    private static void accountingForNewObject(OrdinaryDestroyed o) {
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
        WeakReference<Object> wr = new WeakReference<Object>(o, weakReferenceQueue);
        PhantomReference<Object> pr = new PhantomReference<Object>(o, phantomReferenceQueue);
        synchronized(TestDestruction.class) {
            weakReferenceMap.put(wr, id);
            phantomReferenceMap.put(pr, id);
            aliveAndUnderTest.add(id);
            alive.add(id);
        }
    }

    private boolean gcAndWait(long timeLimitMillis) {
        return gcAndWait(timeLimitMillis, null, null, null);
    }

    private boolean gcAndWait(long timeLimitMillis, Long excessLimitMillis, Integer disposedCount, Integer destroyedCount) {
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
//                    QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDelete.value());
////                    QApplication.processEvents();
//                }

                Utils.releaseNativeResources();
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
                    currentDisposedCount = MyOrdinaryDestroyed.getDisposedCount();
                    currentDestroyedCount = MyOrdinaryDestroyed.destroyedCount();

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

    private void reset() {
        synchronized(MyOrdinaryDestroyed.class) {
            MyOrdinaryDestroyed.setDisposedCount(0);
            MyOrdinaryDestroyed.setDestroyedCount(0);
        }
    }

    // method exists to ensure object reference created here is discarded for GC
    private void testJavaCreationJavaOwnership_internal() {
        MyOrdinaryDestroyed d = new MyOrdinaryDestroyed(getIdNext());

        accountingForNewObject(d);
    }

    @Test
    public void testJavaCreationJavaOwnership() {
        reset();

        testJavaCreationJavaOwnership_internal();

        gcAndWait(5000, (long)250, 1, 1);

        assertEquals(1, MyOrdinaryDestroyed.getDisposedCount());
        assertEquals(1, MyOrdinaryDestroyed.destroyedCount());
    }

    // method exists to ensure object reference created here is discarded for GC
    private void testJavaCreationCppOwnership_internal(MyOrdinaryDestroyed dontBeDeleted) {
        // Garbage collection has now been disabled on d
        OrdinaryDestroyed d = MyOrdinaryDestroyed.callGetObjectCppOwnership(dontBeDeleted);

        accountingForNewObject(d);
    }

    // Method requires: getObjectCacheMode == DEFAULT
    @Test
    public void testJavaCreationCppOwnership() {
        reset();

        MyOrdinaryDestroyed dontBeDeleted = new MyOrdinaryDestroyed(getIdNext());
        accountingForNewObject(dontBeDeleted);

        testJavaCreationCppOwnership_internal(dontBeDeleted);

        gcAndWait(5000, (long)2500, 0, 0);		// Need to give it all the time

        assertEquals(0, MyOrdinaryDestroyed.getDisposedCount());
        assertEquals(0, MyOrdinaryDestroyed.destroyedCount());

        // CHECKME: Can we undo that hold and test for release?
    }

    // method exists to ensure object reference created here is discarded for GC
    private void testJavaCreationDefaultOwnershipThroughNative_internal(MyOrdinaryDestroyed dontBeDeleted) {
        // Garbage collection has now been disabled on d
        OrdinaryDestroyed d = MyOrdinaryDestroyed.callGetObjectCppOwnership(dontBeDeleted);

        // Set default ownership on d (should be "java")
        MyOrdinaryDestroyed.setDefaultOwnership(d);

        accountingForNewObject(d);
    }

    // Method requires: getObjectCacheMode == DEFAULT
    @Test
    public void testJavaCreationDefaultOwnershipThroughNative() {
        reset();

        MyOrdinaryDestroyed dontBeDeleted = new MyOrdinaryDestroyed(getIdNext());
        accountingForNewObject(dontBeDeleted);

        testJavaCreationDefaultOwnershipThroughNative_internal(dontBeDeleted);

        gcAndWait(5000, (long)250, 1, 1);

        assertEquals(1, MyOrdinaryDestroyed.getDisposedCount());
        assertEquals(1, MyOrdinaryDestroyed.destroyedCount());
    }

    // method exists to ensure object reference created here is discarded for GC
    private void testJavaCreationDefaultOwnershipThroughShell_internal(MyOrdinaryDestroyed dontBeDeleted) {
        // Garbage collection has now been disabled on d
        OrdinaryDestroyed d = MyOrdinaryDestroyed.callGetObjectCppOwnership(dontBeDeleted);

        // Set default ownership on d (should be "java")
        MyOrdinaryDestroyed.callSetDefaultOwnership(dontBeDeleted, d);

        accountingForNewObject(d);
    }

    // Method requires: getObjectCacheMode == DEFAULT
    @Test
    public void testJavaCreationDefaultOwnershipThroughShell() {
        reset();

        MyOrdinaryDestroyed dontBeDeleted = new MyOrdinaryDestroyed(getIdNext());
        accountingForNewObject(dontBeDeleted);

        testJavaCreationDefaultOwnershipThroughShell_internal(dontBeDeleted);

        gcAndWait(5000, (long)250, 1, 1);

        assertEquals(1, MyOrdinaryDestroyed.getDisposedCount());
        assertEquals(1, MyOrdinaryDestroyed.destroyedCount());
    }

    // method exists to ensure object reference created here is discarded for GC
    private void testCppCreationSplitOwnership_internal() {
        // Split ownership: Java object should be collected, but the c++ object should not be baleeted
        OrdinaryDestroyed d = MyOrdinaryDestroyed.getObjectSplitOwnership();

        accountingForNewObject(d);
    }

    @Test
    public void testCppCreationSplitOwnership() {
        reset();

        testCppCreationSplitOwnership_internal();

        gcAndWait(5000, (long)500, 1, 0);	// Need to give it time for destroy

        assertEquals(1, MyOrdinaryDestroyed.getDisposedCount());
        assertEquals(0, MyOrdinaryDestroyed.destroyedCount());

        // FIXME: Undo what we did above and ensure it does actually go then
    }

    // method exists to ensure object reference created here is discarded for GC
    private void testCppCreationCppOwnership_internal() {
        OrdinaryDestroyed d = MyOrdinaryDestroyed.getObjectCppOwnership();

        accountingForNewObject(d);
    }

    @Test
    public void testCppCreationCppOwnership() {
        reset();

        testCppCreationCppOwnership_internal();

        gcAndWait(5000, (long)2500, 0, 0);		// Need to give it all the time

        assertEquals(0, MyOrdinaryDestroyed.getDisposedCount());
        assertEquals(0, MyOrdinaryDestroyed.destroyedCount());

        // FIXME: Undo what we did above and ensure it does actually go then
    }

    // method exists to ensure object reference created here is discarded for GC
    private void testCppCreationJavaOwnership_internal() {
        OrdinaryDestroyed d = MyOrdinaryDestroyed.getObjectJavaOwnership();

        accountingForNewObject(d);
    }

    @Test
    public void testCppCreationJavaOwnership() {
        reset();

        testCppCreationJavaOwnership_internal();

        gcAndWait(5000, (long)250, 1, 1);

        assertEquals(1, MyOrdinaryDestroyed.getDisposedCount());
        assertEquals(1, MyOrdinaryDestroyed.destroyedCount());
    }

    // method exists to ensure object reference created here is discarded for GC
    private void testCppCreationDefaultOwnershipThroughShell_internal(MyOrdinaryDestroyed dontBeDeleted) {
        // Garbage collection has now been disabled on d
        OrdinaryDestroyed d = MyOrdinaryDestroyed.getObjectCppOwnership();

        // Set default ownership on d (should be "split")
        MyOrdinaryDestroyed.callSetDefaultOwnership(dontBeDeleted, d);

        accountingForNewObject(d);
    }

    @Test
    public void testCppCreationDefaultOwnershipThroughShell() {
        reset();

        MyOrdinaryDestroyed dontBeDeleted = new MyOrdinaryDestroyed(getIdNext());
        accountingForNewObject(dontBeDeleted);

        testCppCreationDefaultOwnershipThroughShell_internal(dontBeDeleted);

        gcAndWait(5000, (long)500, 1, 0);	// Need to give it time for destroy

        assertEquals(1, MyOrdinaryDestroyed.getDisposedCount());
        assertEquals(0, MyOrdinaryDestroyed.destroyedCount());

        // FIXME: Undo what we did above and ensure it does actually go then
    }

    // method exists to ensure object reference created here is discarded for GC
    private void testCppCreationDefaultOwnershipThroughNative_internal(MyOrdinaryDestroyed dontBeDeleted) {
        // Garbage collection has now been disabled on d
        OrdinaryDestroyed d = MyOrdinaryDestroyed.getObjectCppOwnership();

        // Set default ownership on d (should be "split")
        MyOrdinaryDestroyed.setDefaultOwnership(d);

        accountingForNewObject(d);
    }

    @Test
    public void testCppCreationDefaultOwnershipThroughNative() {
        reset();

        MyOrdinaryDestroyed dontBeDeleted = new MyOrdinaryDestroyed(getIdNext());
        accountingForNewObject(dontBeDeleted);

        testCppCreationDefaultOwnershipThroughNative_internal(dontBeDeleted);

        gcAndWait(5000, (long)500, 1, 0);	// Need to give it time for destory

        assertEquals(1, MyOrdinaryDestroyed.getDisposedCount());
        assertEquals(0, MyOrdinaryDestroyed.destroyedCount());

        if(MyOrdinaryDestroyed.getDisposedCount() > 1) {    // PS. Expression never true here.
            System.err.println("Avoid reordering of code which may cause dontBeDeleted to be deleted: "
                                + dontBeDeleted);
        }
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestDestruction.class.getName());
    }
}
