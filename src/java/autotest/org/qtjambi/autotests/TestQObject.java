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
import static org.junit.Assert.assertTrue;

import java.lang.ref.Reference;
import java.lang.ref.ReferenceQueue;
import java.lang.ref.PhantomReference;
import java.lang.ref.WeakReference;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import org.qtjambi.qt.QThread;
import org.qtjambi.qt.core.QCoreApplication;
import org.qtjambi.qt.core.QEvent;
import org.qtjambi.qt.core.QFile;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.core.QRegExp;
import org.qtjambi.qt.internal.QtJambiRuntime;

public class TestQObject extends QApplicationTest {
    private static final int TIMEOUT = 10000;
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
        QtJambiRuntime.setObjectCacheMode(QtJambiRuntime.OBJECT_CACHE_MODE_DISABLE);

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
        QtJambiRuntime.setObjectCacheMode(QtJambiRuntime.OBJECT_CACHE_MODE_DEFAULT);
    }

    @Test
    public void run_findChildren() {
        {
            List<QObject> c = root.findChildren();
            assertEquals(c.size(), 8);

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
            List<QObject> c = root.findChildren(QFile.class);
            assertEquals(c.size(), 2);

            assertTrue(c.contains(file1));
            assertTrue(c.contains(file2));
        }

        {
            List<QObject> c = root.findChildren(null, "child");

            assertEquals(c.size(), 2);
            assertTrue(c.contains(child12));
            assertTrue(c.contains(child22));
        }

        {
            List<QObject> c = root.findChildren(null, new QRegExp("child"));

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

    public static class DyingObject extends QObject {
        public DyingObject() {
            Utils.println(4, "DyingObject#ctor() thread=" + thread() + "; id=" + id);

            WeakReference<DyingObject> wr = new WeakReference<DyingObject>(this, weakReferenceQueue);
            PhantomReference<DyingObject> pr = new PhantomReference<DyingObject>(this, phantomReferenceQueue);
            synchronized(DyingObject.class) {
                weakReferenceMap.put(wr, id);
                phantomReferenceMap.put(pr, id);
                aliveAndUnderTest.add(id);
                alive.add(id);
            }
        }

        @Override
        public void disposed() {
            synchronized(DyingObject.class) {
                aliveAndUnderTest.remove((Integer)id);
                alive.remove((Integer)id);
            }
        }

        @Override
        protected void finalize() {
            try {
                Utils.println(4, "DyingObject#dtor() thread=" + thread() + "; id=" + id);
            } finally {
                super.finalize();
            }
        }

        static public void waitForEmpty(int timeout) {
            // we need to wait for gc to collect the parent.
            long expireTime = 0;
            if(timeout >= 0)
                expireTime = System.currentTimeMillis() + timeout;
            Utils.println(5, " waitForEmpty(timeout=" + timeout + "; expireTime=" + expireTime + ")");

            int aliveSize;
            int aliveAndUnderTestSize;
            int weakReferenceMapSize;
            int phantomReferenceMapSize;
            while(true) {
//                Utils.releaseNativeResources();
                Utils.gc(gcMode, 3);
//                System.runFinalization();

                Reference<? extends DyingObject> thisWr;
                while((thisWr = weakReferenceQueue.poll()) != null) {
                    Integer tmpId;
                    synchronized(DyingObject.class) {
                        tmpId = weakReferenceMap.remove(thisWr);
                    }
                    Utils.println(5, " weakReferenceQueue.remove(): dequeued id=" + tmpId);
                }
                Reference<? extends DyingObject> thisPr;
                while((thisPr = phantomReferenceQueue.poll()) != null) {
                    Integer tmpId;
                    synchronized(DyingObject.class) {
                        tmpId = phantomReferenceMap.remove(thisPr);
                    }
                    Utils.println(5, " phantomReferenceQueue.remove(): dequeued id=" + tmpId);
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
                    Utils.println(5, " timeout=" + timeout + "; expireTime=" + expireTime + "; left=" + left);
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
            if(Utils.isDebugLevel(4)) {
                // Print array format [1, 2, 3]
                synchronized(DyingObject.class) {
                    String aliveString = Utils.listToString(alive);
                    String aliveAndUnderTestString = Utils.listToString(aliveAndUnderTest);
                    String weakReferenceMapString = Utils.mapValueToString(weakReferenceMap);
                    String phantomReferenceMapString = Utils.mapValueToString(phantomReferenceMap);
                    Utils.println(4, "alive=" + aliveString + "; aliveAndUnderTest=" + aliveAndUnderTestString +
                            "; weakReferenceMapSize=" + weakReferenceMapString + "; phantomReferenceMapSize=" + phantomReferenceMapString);
                }
            }
        }

        // This should be WeakReference/SoftReference
        private static ReferenceQueue<DyingObject> weakReferenceQueue = new ReferenceQueue<DyingObject>();
        private static ReferenceQueue<DyingObject> phantomReferenceQueue = new ReferenceQueue<DyingObject>();
        private static Map<WeakReference<DyingObject>,Integer> weakReferenceMap = new HashMap<WeakReference<DyingObject>,Integer>();
        private static Map<PhantomReference<DyingObject>,Integer> phantomReferenceMap = new HashMap<PhantomReference<DyingObject>,Integer>();
        private static List<Integer> alive = new ArrayList<Integer>();
        private static List<Integer> aliveAndUnderTest = new ArrayList<Integer>();
        private static int counter = 0;

        public final Integer id = getIdNext();

        public static Integer getIdNext() {
            int idNext;
            synchronized(DyingObject.class) {
                counter++;
                idNext = counter;
            }
            return Integer.valueOf(idNext);
        }
        public static void clear() {
            synchronized(DyingObject.class) {
                aliveAndUnderTest.clear();
            }
        }
        public static int getSize() {
            synchronized(DyingObject.class) {
                return aliveAndUnderTest.size();
            }
        }
    }

    private static void noop(Object o) {
        if(o != null)
            o.toString();
    }

    private static void oneObject() {
        oneObject_helper();
        Utils.gc(gcMode);
    }

    private static void oneObject_helper() {
        DyingObject tmp = new DyingObject();
        noop(tmp);
    }

    private static void objectWithParent() {
        objectWithParent_helper();
        Utils.gc(gcMode);
    }

    private static void objectWithParent_helper() {
        DyingObject tmp = new DyingObject();
        tmp.setParent(new DyingObject());
        noop(tmp);
    }

    private static void objectWithUnParent() {
        objectWithUnParent_helper();
        Utils.gc(gcMode);
    }

    private static void objectWithUnParent_helper() {
        DyingObject p = new DyingObject();
        DyingObject o = new DyingObject();
        o.setParent(p);
        o.setParent(null);
    }

    private static void threadExecutor(Runnable r, boolean qthread) {
        Thread t = qthread ? new QThread(r) : new Thread(r);
        t.start();
        try {
            t.join(1000);
        } catch (Exception e) { e.printStackTrace(); }
    }

    @Test
    public void disposal_oneObject() {
        DyingObject.clear();
        oneObject();

        // we need to wait for gc to collect the parent.
        DyingObject.waitForEmpty(TIMEOUT);

        assertEquals(0, DyingObject.getSize());
    }

    @Test
    public void disposal_objectWithParent() {
        DyingObject.clear();
        objectWithParent();

        // we need to wait for gc to collect the parent.
        DyingObject.waitForEmpty(TIMEOUT);

        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDelete.value());
        assertEquals(0, DyingObject.getSize());
    }

    @Test
    public void disposal_objectWithUnParent() {
        DyingObject.clear();
        objectWithUnParent();
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDelete.value());

        // we need to wait for gc to collect the parent.
        DyingObject.waitForEmpty(TIMEOUT);

        assertEquals(0, DyingObject.getSize());
    }

    @Test
    public void disposal_disposeInThread() {
        DyingObject.clear();
        threadExecutor(runnable_disposeInThread, false);
        DyingObject.waitForEmpty(TIMEOUT);
        assertEquals(0, DyingObject.getSize());

        threadExecutor(runnable_disposeInThread, true);
        DyingObject.waitForEmpty(TIMEOUT);
        assertEquals(0, DyingObject.getSize());
    }

    @Test
    public void disposal_gcInQThread_oneObject() {
        DyingObject.clear();
        threadExecutor(runnable_oneObject, true);
        DyingObject.waitForEmpty(TIMEOUT);
        assertEquals(0, DyingObject.getSize());
    }

    @Test
    public void disposal_gcInQThread_objectWithParent() {
        DyingObject.clear();
        threadExecutor(runnable_objectWithParent, true);

        // we need to wait for gc to collect the parent.
        DyingObject.waitForEmpty(TIMEOUT);

        assertEquals(0, DyingObject.getSize());
    }

    @Test
    public void disposal_gcInQThread_objectWithUnParent() {
        DyingObject.clear();
        threadExecutor(runnable_objectWithUnParent, true);

        // we need to wait for gc to collect the parent.
        DyingObject.waitForEmpty(TIMEOUT);

        assertEquals(0, DyingObject.getSize());
    }

    @Test
    public void disposal_gcInThread_oneObject() {
        // Will warn about leaking the C++ object.
        DyingObject.clear();
        threadExecutor(runnable_oneObject, false);
        DyingObject.waitForEmpty(TIMEOUT);
        assertEquals(0, DyingObject.getSize());
    }

    @Test
    public void disposal_gcInThread_objectWithParent() {
        // Will warn once about leaking the C++ object. The child will not be collected
        DyingObject.clear();
        threadExecutor(runnable_objectWithParent, false);


        try {
            Thread.sleep(TIMEOUT); // make sure we do an effort to make this test fail.
        } catch (Exception e) {
            e.printStackTrace();
        }

        assertEquals(1, DyingObject.getSize());
    }

    @Test
    public void disposal_gcInThread_objectWithUnParent() {
        // Will warn twice about leaking the C++ object, but both objects will be collected
        DyingObject.clear();
        threadExecutor(runnable_objectWithUnParent, false);

        // we need to wait for gc to collect the parent.
        DyingObject.waitForEmpty(TIMEOUT);

        assertEquals(0, DyingObject.getSize());
    }

    private Runnable runnable_disposeInThread = new Runnable() {
        public void run() { new DyingObject().dispose(); }
    };

    private Runnable runnable_oneObject = new Runnable() {
        public void run() { oneObject(); }
    };

    private Runnable runnable_objectWithParent = new Runnable() {
        public void run() { objectWithParent(); }
    };

    private Runnable runnable_objectWithUnParent = new Runnable() {
        public void run() { objectWithUnParent(); }
    };

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQObject.class.getName());
    }
}
