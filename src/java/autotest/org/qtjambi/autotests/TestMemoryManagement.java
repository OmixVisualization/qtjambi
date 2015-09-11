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
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.assertNotNull;

import java.lang.ref.Reference;
import java.lang.ref.ReferenceQueue;
import java.lang.ref.PhantomReference;
import java.lang.ref.WeakReference;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Ignore;
import org.junit.Test;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.qtjambi.qt.QNoNativeResourcesException;
import org.qtjambi.qt.core.QCoreApplication;
import org.qtjambi.qt.core.QEvent;
import org.qtjambi.qt.core.QEventLoop;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.internal.QtJambiDebugTools;
import org.qtjambi.qt.internal.QtJambiObject;

// Attempt at complete test for general memory leaks and crashes
// Should test that all the general cases work as intended by default.
//
// We have extra methods in here to make sure that the stack frame which an
// object under test has had a live strong reference in is ended before we
// start to look/wait for text result condition to occur.  Since the
// compiler is allowed to keep hold of references longer then the code
// might indicate.  To defeat the compiler for sure we could use a Util
// class and pass in the abstract type as interface or we could implement
// the method call in that interface via reflection.
@Ignore("abstract and base Java class")
public abstract class TestMemoryManagement {
    private ReferenceQueue<Object> weakReferenceQueue = new ReferenceQueue<Object>();
    private ReferenceQueue<Object> phantomReferenceQueue = new ReferenceQueue<Object>();
    private Map<WeakReference<Object>,Integer> weakReferenceMap = new HashMap<WeakReference<Object>,Integer>();
    private Map<PhantomReference<Object>,Integer> phantomReferenceMap = new HashMap<PhantomReference<Object>,Integer>();
    private List<Integer> alive = new ArrayList<Integer>();
    private List<Integer> aliveAndUnderTest = new ArrayList<Integer>();
    private int counter = 0;

    @BeforeClass
    public static void testInitialize() throws Exception {
        Utils.println(2, "TestMemoryManagement.testInitialize(): begin");
        QApplication.initialize(new String[] {});
        // To fix this ensure that you set buildpath.properties: qtjambi.debug-tools = true
        // then rebuild both QtJambi and the autotests with the C compiler define -D QTJAMBI_DEBUG_TOOLS
        //
        // This checks the QtJambi was compiled with -D QTJAMBI_DEBUG_TOOLS
        if(!QtJambiDebugTools.hasDebugTools())
            throw new RuntimeException("Prerequisite 1 of 2: These tests can only be run when \"Qt Jambi\" is compiled with DEFINES += QTJAMBI_DEBUG_TOOLS");
        // This checks the autotests was compiled with -D QTJAMBI_DEBUG_TOOLS
        if(!QtJambiUnittestTools.hasDebugTools())
            throw new RuntimeException("Prerequisite 2 of 2: These tests can only be run when \"Qt Jambi autotests\" is compiled with DEFINES += QTJAMBI_DEBUG_TOOLS");
        Utils.println(2, "TestMemoryManagement.testInitialize(): done");
    }

    @AfterClass
    public static void testDispose() throws Exception {
        Utils.println(2, "TestMemoryManagement.testDispose(): begin");
        if(Utils.releaseNativeResources() > 0) {
            System.gc();
            System.runFinalization();
        }
        QApplication.processEvents();
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDelete.value());
        QApplication.quit();
        System.err.flush();
        System.out.flush();
        QApplication app = QApplication.instance();
        if(app != null)
            app.dispose();
        try {
            Utils.println(3, "TestMemoryManagement.testDispose(): done  app="+app); 
        } catch(QNoNativeResourcesException e) {
            Utils.println(3, "TestMemoryManagement.testDispose(): done  org.qtjambi.qt.QNoNativeResourcesException: app="+e.getMessage()); 
        }
        app = null;		// kill hard-reference
        Utils.println(3, "TestMemoryManagement.testDispose(): shutdown PRE");
        QApplication.shutdown();
        Utils.println(3, "TestMemoryManagement.testDispose(): shutdown POST");

        QtJambiUnittestTools.getObjectCount(1, 0);  // fflush(stdout)
        QtJambiUnittestTools.getObjectCount(2, 0);  // fflush(stderr)
        int objectCount = QtJambiUnittestTools.getObjectCount(3, 0);  // QtJambiLink::QtJambiLink_dump()
        QtJambiUnittestTools.getObjectCount(2, 0);  // fflush(stderr)
        Utils.println(3, "TestMemoryManagement.testDispose(): end objectCount="+objectCount);

        if(objectCount == 0)
            return;  // optimization due to class loading causing some references to be set

        QtJambiUnittestTools.clearStaticReferences();
        System.gc();
        System.runFinalization();
        System.gc();
        System.runFinalization();

        QtJambiUnittestTools.getObjectCount(1, 0);  // fflush(stdout)
        QtJambiUnittestTools.getObjectCount(2, 0);  // fflush(stderr)
        objectCount = QtJambiUnittestTools.getObjectCount(3, 0);  // QtJambiLink::QtJambiLink_dump()
        QtJambiUnittestTools.getObjectCount(2, 0);  // fflush(stderr)
        Utils.println(3, "TestMemoryManagement.testDispose(): end objectCount="+objectCount);
    }

    @Before
    public void setUp() {
        initialize();
    }

    @After
    public void tearDown() {
        uninitialize();
    }

    public Integer getIdNext() {
        int idNext;
        synchronized(TestMemoryManagement.class) {
            counter++;
            idNext = counter;
        }
        return Integer.valueOf(idNext);
    }
    public void clearGcReferences() {
        synchronized(TestMemoryManagement.class) {
            aliveAndUnderTest.clear();
        }
    }
    public int getSize() {
        synchronized(TestMemoryManagement.class) {
            return aliveAndUnderTest.size();
        }
    }

    protected void accountingForObject(Object o) {
        assertNotNull(o);

        Integer id = getIdNext();

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
        synchronized(TestMemoryManagement.class) {
            weakReferenceMap.put(wr, id);
            phantomReferenceMap.put(pr, id);
            aliveAndUnderTest.add(id);
            alive.add(id);
        }
    }

    // Types: Normal object type with shell class
    //        Normal object type without shell class
    //        Normal value type
    //        QObject
    //        GUI object type
    //        GUI value type
    //
    // Creation: In Java
    //           In Native
    //
    // Ownership: Java Ownership
    //            Split Ownership
    //            Native Ownership
    //
    // Ways to delete: finalization
    //                 dispose
    //                 Deleted in native (split and native ownership)
    //                 invalidate (non-Java-Ownership only)
    //
    // Things to check:
    //    1. disposed() is called when it needs to be
    //    2. The destructor of the class is called (when applicable, i.e. not for non-polymorphic types
    //       that are deleted from C++) Both for the destructor of the shell class and the destructor
    //       function of the class (if it doesn't have a virtual destructor)
    //    3. The QtJambiLink for the object is deleted.
    //    4. The Java object is finalized.
    //
    // For these tests you need to compile Qt Jambi with DEFINES += QTJAMBI_DEBUG_TOOLS so that you
    // get compiled-in invocation counts.

    protected final void resetAll() {
        QtJambiDebugTools.reset_destructorFunctionCalledCount();
        QtJambiDebugTools.reset_disposeCalledCount();
        QtJambiDebugTools.reset_finalizedCount();
        QtJambiDebugTools.reset_linkConstructedCount();
        QtJambiDebugTools.reset_linkDestroyedCount();
        QtJambiDebugTools.reset_objectInvalidatedCount();
        QtJambiDebugTools.reset_shellDestructorCalledCount();
        QtJambiDebugTools.reset_userDataDestroyedCount();
        clearGcReferences();

        System.gc();
        System.runFinalization();
        if(Utils.releaseNativeResources() > 0) {
            System.gc();
            System.runFinalization();
        }

        QApplication.processEvents();
        if(Utils.releaseNativeResources() > 0) {
            System.gc();
            System.runFinalization();
        }
    }


    private static final int TIME_LIMIT = 1000;

    protected final void test(String className,
            int finalizedCount,
            int destructorFunctionCalledCount,
            int disposeCalledCount,
            int linkConstructedCount,
            int linkDestroyedCount,
            int objectInvalidatedCount,
            int shellDestructorCalledCount,
            int userDataDestroyedCount) {

        assertEquals("finalizedCount",                finalizedCount,                                        QtJambiDebugTools.finalizedCount(className));
        assertEquals("destructorFunctionCalledCount", !isQObject() ? destructorFunctionCalledCount : 0,      QtJambiDebugTools.destructorFunctionCalledCount(className));
        assertEquals("disposeCalledCount",            disposeCalledCount,                                    QtJambiDebugTools.disposeCalledCount(className));
        assertEquals("linkConstructedCount",          linkConstructedCount,                                  QtJambiDebugTools.linkConstructedCount(className));
        assertEquals("linkDestroyedCount",            linkDestroyedCount,                                    QtJambiDebugTools.linkDestroyedCount(className));
        assertEquals("objectInvalidatedCount",        objectInvalidatedCount,                                QtJambiDebugTools.objectInvalidatedCount(className));
        assertEquals("shellDestructorCalledCount",    hasShellDestructor() ? shellDestructorCalledCount : 0, QtJambiDebugTools.shellDestructorCalledCount(className));
        assertEquals("userDataDestroyedCount",        isQObject() ? userDataDestroyedCount : 0,              QtJambiDebugTools.userDataDestroyedCount(className));

    }

    protected abstract void initialize();

    protected abstract void uninitialize();

    protected abstract QtJambiObject createInstanceInJava();

    protected abstract QtJambiObject createInstanceInNative();

    protected abstract void deleteLastInstance();

    protected abstract QtJambiObject invalidateObject(QtJambiObject obj, final boolean returnReference);

    protected abstract boolean hasShellDestructor();

    protected abstract String className();

    protected abstract boolean hasVirtualDestructor();

    protected boolean isValueType() {
        return false;
    }

    protected boolean needsEventProcessing() {
        return false;
    }

    protected boolean isQObject() {
        return false;
    }

    protected boolean supportsSplitOwnership() {
        return true;
    }

    @Test
    public void finalize_CreatedInJava_JavaOwnership() {
        resetAll();

        {
            createInstanceInJava();
        }

        gcAndWait(TIME_LIMIT, null, 1, 1, 1, 1, 1);

        test(className(), 1, 1, 0, 1, 1, 1, 1, 1);

    }

    @Test
    public void finalize_NotCreatedInJava_SplitOwnership() {
        if (supportsSplitOwnership()) {
            resetAll();

            {
                createInstanceInNativeWithSplitOwnership();
            }

            gcAndWait(TIME_LIMIT, null, 1, 1, 1, null, null);

            test(className(), 1, 0, 0, 1, 1, 1, 0, 0);
        }
    }

    private void createInstanceInNativeWithSplitOwnership() {
        createInstanceInNative();
    }

    @Test
    public void finalize_NotCreatedInJava_JavaOwnership() {
        resetAll();

        {
            createInstanceInNativeWithJavaOwnership();
        }

        gcAndWait(TIME_LIMIT, null, 1, 1, 1, null, 1);

        test(className(), 1, 1, 0, 1, 1, 1, 0, 1);
    }

    private void createInstanceInNativeWithJavaOwnership() {
        QtJambiObject obj = createInstanceInNative();
        obj.setJavaOwnership();
    }

    @Test
    public void dispose_CreatedInJava_JavaOwnership() {
        resetAll();

        {
            createInstanceInJavaAndDisposeIt();
        }

        gcAndWait(TIME_LIMIT, null, 1, 1, 1, 1, 1);

        test(className(), 1, 1, 1, 1, 1, 1, 1, 1);
    }

    private void createInstanceInJavaAndDisposeIt() {
        {
            QtJambiObject obj = createInstanceInJava();
            obj.dispose();
        }

        test(className(), 0, 1, 1, 1, 1, 1, 1, 1);
    }

    @Test
    public void dispose_CreatedInJava_CppOwnership() {
        resetAll();

        {
            createInJavaDisableGCAndDispose();
        }

        gcAndWait(TIME_LIMIT, null, 1, 1, 1, 1, 1);

        test(className(), 1, 1, 1, 1, 1, 1, 1, 1);
    }

    private void createInJavaDisableGCAndDispose() {
        {
            QtJambiObject obj = createInstanceInJava();
            obj.disableGarbageCollection();
            obj.dispose();
        }

        test(className(), 0, 1, 1, 1, 1, 1, 1, 1);
    }

    @Test
    public void dispose_NotCreatedInJava_SplitOwnership() {
        if (supportsSplitOwnership()) {
            resetAll();

            {
                createInNativeAndDispose();
            }

            gcAndWait(TIME_LIMIT, null, 1, 1, 1, null, null);

            test(className(), 1, 1, 1, 1, 1, 1, 0, 0);
        }
    }

    private void createInNativeAndDispose() {
        {
            QtJambiObject obj = createInstanceInNative();
            obj.dispose();
        }

        test(className(), 0, 1, 1, 1, 1, 1, 0, 0);
    }

    @Test
    public void dispose_NotCreatedInJava_JavaOwnership() {
        resetAll();

        {
            createInNativeSetJavaOwnershipAndDispose();
        }

        gcAndWait(TIME_LIMIT, null, 1, 1, 1, null, 1);

        test(className(), 1, 1, 1, 1, 1, 1, 0, 1);
    }

    private void createInNativeSetJavaOwnershipAndDispose() {
        {
            QtJambiObject obj = createInstanceInNative();
            obj.setJavaOwnership();
            obj.dispose();
        }

        test(className(), 0, 1, 1, 1, 1, 1, 0, 1);
    }

    @Test
    public void dispose_NotCreatedInJava_CppOwnership() {
        resetAll();

        {
            createInNativeDisableGCAndDispose();
        }

        gcAndWait(TIME_LIMIT, null, 1, 1, 1, null, 1);

        test(className(), 1, 1, 1, 1, 1, 1, 0, 1);
    }

    private void createInNativeDisableGCAndDispose() {
        {
            QtJambiObject obj = createInstanceInNative();
            obj.disableGarbageCollection();
            obj.dispose();
        }

        test(className(), 0, 1, 1, 1, 1, 1, 0, 1);
    }

    @Test
    public void nativeDelete_CreatedInJava_CppOwnership() {
        resetAll();

        {
            createInJavaDisableGCAndDeleteInNative();
        }


        if (hasVirtualDestructor()) {
            gcAndWait(TIME_LIMIT, null, 1, 1, 1, 1, 1);
            test(className(), 1, 0, 0, 1, 1, 1, 1, 1);
        } else {
            gcAndWait(TIME_LIMIT, Long.valueOf(250), null, 1, null, null, null);	// FIXME nothing to test/wait for ?
            test(className(), 0, 0, 0, 1, 0, 0, 0, 0);
        }

    }

    private void createInJavaDisableGCAndDeleteInNative() {
        {
            QtJambiObject obj = createInstanceInJava();
            obj.disableGarbageCollection();

            deleteLastInstance();
            if (hasVirtualDestructor())
                assertEquals(0L, obj.nativeId());
        }

        if (hasVirtualDestructor())
            test(className(), 0, 0, 0, 1, 1, 1, 1, 1);
        else
            test(className(), 0, 0, 0, 1, 0, 0, 0, 0);
    }

    // Many objects leak in this test. Cases that lead to this scenario
    // must be specially handled to avoid memory leaks.
    @Test
    public void nativeDelete_NotCreatedInJava_CppOwnership() {
Utils.println(15, "nativeDelete_NotCreatedInJava_CppOwnership(): MARK1");
        resetAll();

Utils.println(15, "nativeDelete_NotCreatedInJava_CppOwnership(): MARK2");
        {
            createInNativeDisableGCAndDeleteInNative();
        }
Utils.println(15, "nativeDelete_NotCreatedInJava_CppOwnership(): MARK3");

        if(isQObject()) {
            gcAndWait(TIME_LIMIT, null, 1, 1, 1, null, 1);
            test(className(), 1, 0, 0, 1, 1, 1, 0, 1);
        } else {
            gcAndWait(TIME_LIMIT, null, null, 1, null, null, 1);	// userDataDestroyedCount
            test(className(), 0, 0, 0, 1, 0, 0, 0, 1);
        }
    }

    // method exists to ensure object reference created here is discarded for GC
    private void createInNativeDisableGCAndDeleteInNative_internal() {
        QtJambiObject obj = createInstanceInNative();
        obj.disableGarbageCollection();
    }

    private void createInNativeDisableGCAndDeleteInNative() {
Utils.println(15, "createInNativeDisableGCAndDeleteInNative() MARK1 class=" + getClass().getName());
        createInNativeDisableGCAndDeleteInNative_internal();
Utils.println(15, "createInNativeDisableGCAndDeleteInNative() MARK2");
        test(className(), 0, 0, 0, 1, 0, 0, 0, 0);
Utils.println(15, "createInNativeDisableGCAndDeleteInNative() MARK3");

        deleteLastInstance();
        test(className(), 0, 0, 0, 1, isQObject() ? 1 : 0, isQObject() ? 1 : 0, 0, 1);
    }

    private boolean gcAndWait(long timeLimitMillis, Long excessLimitMillis, Integer finalizedCount, Integer linkConstructedCount, Integer linkDestroyedCount, Integer shellDestructorCalledCount, Integer userDataDestroyedCount) {
        boolean obtainGoal = false;	// did we meet conditions within timelimit?

        long startTime = System.currentTimeMillis();
        long excessTime = 0;
        long elapsed = 0;

        int currentFinalizedCount;
        int currentLinkConstructedCount;
        int currentLinkDestroyedCount;
        int currentShellDestructorCalledCount;
        int currentUserDataDestroyedCount;
        boolean loop = true;
        String debugPrefix = " gcAndWait(" + timeLimitMillis +
            ", " + Utils.toStringOrNull(excessLimitMillis) +
            ", " + Utils.toStringOrNull(finalizedCount) +
            ", " + Utils.toStringOrNull(linkConstructedCount) +
            ", " + Utils.toStringOrNull(linkDestroyedCount) +
            ", " + Utils.toStringOrNull(shellDestructorCalledCount) +
            ", " + Utils.toStringOrNull(userDataDestroyedCount) + ")";
        while(loop) {
            try {
                if (needsEventProcessing()) {
                    Utils.println(5, debugPrefix + ": QApplication.processEvents()");
                    QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDelete.value());
                    QApplication.processEvents();
                }

                Utils.releaseNativeResources();
                System.gc();
                System.runFinalization();

                Reference<? extends Object> thisWr;
                while((thisWr = weakReferenceQueue.poll()) != null) {
                    Integer tmpId;
                    synchronized(TestMemoryManagement.class) {
                        tmpId = weakReferenceMap.remove(thisWr);
                    }
                    Utils.println(5, " weakReferenceQueue.remove(): dequeued id=" + tmpId);
                }
                Reference<? extends Object> thisPr;
                while((thisPr = phantomReferenceQueue.poll()) != null) {
                    Integer tmpId;
                    synchronized(TestMemoryManagement.class) {
                        tmpId = phantomReferenceMap.remove(thisPr);
                    }
                    Utils.println(5, " phantomReferenceQueue.remove(): dequeued id=" + tmpId);
                }

                if(!obtainGoal) {	// Try to obtainGoal
                    currentFinalizedCount = QtJambiDebugTools.finalizedCount(className());
                    currentLinkConstructedCount = QtJambiDebugTools.linkConstructedCount(className());
                    currentLinkDestroyedCount = QtJambiDebugTools.linkDestroyedCount(className());
                    currentShellDestructorCalledCount = QtJambiDebugTools.shellDestructorCalledCount(className());
                    currentUserDataDestroyedCount = QtJambiDebugTools.userDataDestroyedCount(className());

                    boolean loopFinalizedCount = false;
                    if(finalizedCount != null) {
                        if(finalizedCount.intValue() > currentFinalizedCount)
                            loopFinalizedCount = true;
                    }
                    boolean loopLinkConstructedCount = false;
                    if(linkConstructedCount != null) {
                        if(linkConstructedCount.intValue() > currentLinkConstructedCount)
                            loopLinkConstructedCount = true;
                    }
                    boolean loopLinkDestroyedCount = false;
                    if(linkDestroyedCount != null) {
                        if(linkDestroyedCount.intValue() > currentLinkDestroyedCount)
                            loopLinkDestroyedCount = true;
                    }
                    boolean loopShellDestructorCalledCount = false;
                    if(shellDestructorCalledCount != null) {
                        if(shellDestructorCalledCount.intValue() > currentShellDestructorCalledCount)
                            loopShellDestructorCalledCount = true;
                    }
                    boolean loopUserDataDestroyedCount = false;
                    if(userDataDestroyedCount != null) {
                        if(userDataDestroyedCount.intValue() > currentUserDataDestroyedCount)
                            loopUserDataDestroyedCount = true;
                    }

                    if(!loopFinalizedCount && !loopLinkConstructedCount && !loopLinkDestroyedCount && !loopShellDestructorCalledCount && !loopUserDataDestroyedCount) {
                        obtainGoal = true;	// we have met the callers goals
                        excessTime = System.currentTimeMillis(); // reset this for excessLimitMillis
                    }

                    Utils.println(5, debugPrefix + ": elapsed=" + elapsed + "; end loop" +
                        "; currentFinalizedCount=" + currentFinalizedCount +
                        "; currentLinkConstructedCount=" + currentLinkConstructedCount +
                        "; currentLinkDestroyedCount=" + currentLinkDestroyedCount +
                        "; currentShellDestructorCalledCount=" + currentShellDestructorCalledCount +
                        "; currentUserDataDestroyedCount=" + currentUserDataDestroyedCount +
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
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        // Report on status
        if(Utils.isDebugLevel(4)) {
            // Print array format [1, 2, 3]
            synchronized(TestMemoryManagement.class) {
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

    @Test
    public void nativeDelete_NotCreatedInJava_SplitOwnership() {
        if (supportsSplitOwnership()) {
            resetAll();

            {
                createInNativeDeleteInNative();
            }

            gcAndWait(TIME_LIMIT, null, 1, 1, 1, null, 1);

            test(className(), 1, 0, 0, 1, 1, 1, 0, 1);
        }
    }

    private void createInNativeDeleteInNative() {
        {
            QtJambiObject obj = createInstanceInNative();
        }

        deleteLastInstance();
        test(className(), 0, 0, 0, 1, 0, 0, 0, 1);
    }

    @Test
    public void invalidate_NotCreatedInJava_JavaOwnership() {
        resetAll();

        {
            createInNativeSetJavaOwnershipAndInvalidate();
        }

        if(isQObject()) {
            gcAndWait(TIME_LIMIT, null, 1, 1, 1, null, 1);

            test(className(), 1, isValueType() ? 2 : 1, 0, 1, 1, 1, 0, 1);
        } else {
            gcAndWait(TIME_LIMIT, null, 2, 2, 2, null, 1);

            test(className(), 2, isValueType() ? 2 : 1, 0, 2, 2, 2, 0, 1);
        }
    }

    private void createInNativeSetJavaOwnershipAndInvalidate() {
        {
            QtJambiObject obj = createInstanceInNative();
            QtJambiObject obj2 = invalidateObject(obj, true);

            // Java owned objects are not invalidated
            assertTrue(0L != obj2.nativeId());
        }

        test(className(), 0, 0, 0, isQObject() ? 1 : 2, 0, 0, 0, 0);
    }

    // Note that there are two Java objects in use here,
    // because there is an extra wrapper created in
    // the virtual call from C++ to Java in invalidateObject().
    @Test
    public void invalidate_NotCreatedInJava_SplitOwnership() {
        if (supportsSplitOwnership()) {
            resetAll();

            {
                createInNativeAndInvalidate();
            }

            gcAndWait(TIME_LIMIT, null, 2, 2, 2, null, null);

            test(className(), 2, 0, 0, 2, 2, 2, 0, 0);
        }
    }

    private void createInNativeAndInvalidate() {
        {
            QtJambiObject obj = createInstanceInNative();
            invalidateObject(obj, false);
        }

        test(className(), 0, 0, 0, 2, 1, 1, 0, 0);
    }
}
