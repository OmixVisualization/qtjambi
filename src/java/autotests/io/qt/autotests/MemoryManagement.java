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
import java.util.Objects;

import org.junit.After;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtObject;
import io.qt.autotests.generated.General;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.internal.TestUtility;
import io.qt.widgets.QApplication;

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
//@Ignore("abstract and base Java class")
public abstract class MemoryManagement extends ApplicationInitializer{
    private ReferenceQueue<Object> weakReferenceQueue = new ReferenceQueue<Object>();
    private ReferenceQueue<Object> phantomReferenceQueue = new ReferenceQueue<Object>();
    private Map<WeakReference<Object>,Integer> weakReferenceMap = new HashMap<WeakReference<Object>,Integer>();
    private Map<PhantomReference<Object>,Integer> phantomReferenceMap = new HashMap<PhantomReference<Object>,Integer>();
    private List<Integer> alive = new ArrayList<Integer>();
    private List<Integer> aliveAndUnderTest = new ArrayList<Integer>();
    private int counter = 0;
    
    @Before
    public void beforeTest() {
    	DebugTools.enableCounters();
    }
    
    @After
    public void afterTest() {
    	DebugTools.disableCounters();
    }

    @BeforeClass
    public static void testInitialize() throws Exception {
    	org.junit.Assume.assumeTrue("These tests can only run in debug mode or when \"QtJambi\" is compiled with DEFINES += QTJAMBI_DEBUG_TOOLS", TestUtility.hasDebugTools() || TestUtility.isDebugBuild());
        ApplicationInitializer.testInitialize();
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
        synchronized(MemoryManagement.class) {
            counter++;
            idNext = counter;
        }
        return Integer.valueOf(idNext);
    }
    public void clearGcReferences() {
        synchronized(MemoryManagement.class) {
            aliveAndUnderTest.clear();
        }
    }
    public int getSize() {
        synchronized(MemoryManagement.class) {
            return aliveAndUnderTest.size();
        }
    }

    protected void accountingForObject(Object o) {
        assertNotNull(o);

        Integer id = getIdNext();

        {
        	Integer _id = id;
            java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, ()->{
            	String className = o.getClass().getName();
                String shortClassName = className;
                int i = shortClassName.lastIndexOf('.');
                if(i > 0)
                    shortClassName = shortClassName.substring(i + 1);
            	return shortClassName + ".ctor " + className + "@" + System.identityHashCode(o) + "; thread=" + Thread.currentThread().getId() + "; id=" + _id;
            });
        }
        WeakReference<Object> wr = new WeakReference<Object>(o, weakReferenceQueue);
        PhantomReference<Object> pr = new PhantomReference<Object>(o, phantomReferenceQueue);
        synchronized(MemoryManagement.class) {
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
        clearGcReferences();

        ApplicationInitializer.runGC();

        QApplication.processEvents();
        
        DebugTools.reset_destructorFunctionCalledCount();
        DebugTools.reset_disposeCalledCount();
        DebugTools.reset_cleanCallerCount();
        DebugTools.reset_linkConstructedCount();
        DebugTools.reset_linkDestroyedCount();
        DebugTools.reset_objectInvalidatedCount();
        DebugTools.reset_shellDestructorCalledCount();
        DebugTools.reset_shellDestroyedCount();
        DebugTools.reset_userDataDestroyedCount();
        DebugTools.reset_pointerContainerDestroyedCount();
    }


    private static final int TIME_LIMIT = 1000;

    protected final void test(String className,
    		WeakReference<QtObject> ref,
    		boolean isFinalized,
            int cleanCallerCount,
            int destructorFunctionCalledCount,
            int disposeCalledCount,
            int linkConstructedCount,
            int linkDestroyedCount,
            int objectInvalidatedCount,
            int shellDestructorCalledCount,
            int userDataDestroyedCount,
            int shellDestroyedCount,
            int pointerContainerDestroyedCount) {
    	if(isFinalized) {
    		assertEquals("isFinalized", null, ref.get());
    	}

    	List<AssertionError> errors = new ArrayList<>();
        try {
			assertEquals("cleanCallerCount",                cleanCallerCount,                                    DebugTools.cleanCallerCount(className));
		} catch (AssertionError e) {
			errors.add(e);
		}
        try {
        	assertEquals("destructorFunctionCalledCount", destructorFunctionCalledCount,      					 DebugTools.destructorFunctionCalledCount(className));
		} catch (AssertionError e) {
			errors.add(e);
		}
        try {
        	assertEquals("disposeCalledCount",            disposeCalledCount,                                    DebugTools.disposeCalledCount(className));
		} catch (AssertionError e) {
			errors.add(e);
		}
        try {
        	assertEquals("linkConstructedCount",          linkConstructedCount,                                  DebugTools.linkConstructedCount(className));
		} catch (AssertionError e) {
			errors.add(e);
		}
        try {
        	assertEquals("linkDestroyedCount",            linkDestroyedCount,                                    DebugTools.linkDestroyedCount(className));
		} catch (AssertionError e) {
			errors.add(e);
		}
        try {
        	assertEquals("objectInvalidatedCount",        objectInvalidatedCount,                                DebugTools.objectInvalidatedCount(className));
		} catch (AssertionError e) {
			errors.add(e);
		}
        try {
        	assertEquals("shellDestructorCalledCount",    shellDestructorCalledCount, 							 DebugTools.shellDestructorCalledCount(className));
		} catch (AssertionError e) {
			errors.add(e);
		}
        try {
        	assertEquals("userDataDestroyedCount",        userDataDestroyedCount,				                DebugTools.userDataDestroyedCount(className));
		} catch (AssertionError e) {
			errors.add(e);
		}
        try {
        	assertEquals("shellDestroyedCount",        shellDestroyedCount,              						DebugTools.shellDestroyedCount(className));
		} catch (AssertionError e) {
			errors.add(e);
		}
        try {
        	assertEquals("pointerContainerDestroyedCount",        pointerContainerDestroyedCount,               DebugTools.pointerContainerDestroyedCount(className));
		} catch (AssertionError e) {
			errors.add(e);
		}
        if(!errors.isEmpty()) {
        	if(errors.size()==1) {
            	throw errors.get(0);
            }else {
            	StringBuilder stringBuilder = new StringBuilder("Errors:");
            	errors.forEach(e->{
        			stringBuilder.append('\n');
            		stringBuilder.append(e.getMessage());
            	});
            	throw new AssertionError(stringBuilder.toString());
            }
        }
    }

    protected abstract void initialize();

    protected abstract void uninitialize();

    protected abstract QtObject createInstanceInJava();
    
    protected abstract WeakReference<QtObject> createInstanceInJavaWithException();
    
    protected abstract QtObject createInstanceInNative();
    
    protected abstract void deleteLastInstance();

    protected abstract QtObject invalidateObject(QtObject obj, final boolean returnReference);

    protected abstract boolean hasShellDestructor();

    protected abstract String className();
    
    protected String className2() {
    	return className();
    }

    protected abstract boolean hasVirtualDestructor();
    
    protected abstract boolean isSharedPointer();
    
    protected boolean hasInterfaces() {
    	return false;
    }

    protected boolean isValueType() {
        return false;
    }

    protected boolean needsEventProcessing() {
        return false;
    }

    protected boolean isQObject() {
        return false;
    }

    @Test
    public void finalize_CreatedInJava_JavaOwnership() {
        resetAll();
        WeakReference<QtObject> ref;
        {
            ref = new WeakReference<QtObject>(createInstanceInJava());
        }

        gcAndWait(TIME_LIMIT, null, 1, 1, 1, 1, 1);

        test(className(), ref, true, 
        		1, //cleanCallerCount
        		isQObject() ? 0 : 1, //destructorFunctionCalledCount
        		0, //disposeCalledCount
        		isSharedPointer() ? 2 : 1, //linkConstructedCount
				isSharedPointer() ? 2 : 1, //linkDestroyedCount
				isSharedPointer() ? 2 : 1, //objectInvalidatedCount
        		!hasShellDestructor() ? 0 : (hasInterfaces() ? 3 : 1), //shellDestructorCalledCount
				!isQObject() ? 0: 1, //userDataDestroyedCount
        		hasShellDestructor() ? 1 : 0, //shellDestroyedCount
				isSharedPointer() && isQObject() ? 1 : 0 //pointerContainerDestroyedCount
			);

    }

    public void finalize_NotCreatedInJava_SplitOwnership() {
        resetAll();
        WeakReference<QtObject> ref;
        {
            ref = createInstanceInNativeWithSplitOwnership();
        }

        gcAndWait(TIME_LIMIT, null, 1, 1, 1, null, null);

        test(className(), ref, true, 
        		1, //cleanCallerCount
        		isSharedPointer() ? 1 : 0, //destructorFunctionCalledCount
        		0, //disposeCalledCount
        		1, //linkConstructedCount
        		1, //linkDestroyedCount
        		1, //objectInvalidatedCount
        		0, //shellDestructorCalledCount
        		0, //userDataDestroyedCount
        		0, //shellDestroyedCount
        		0 //pointerContainerDestroyedCount
    		);
    }

    private WeakReference<QtObject> createInstanceInNativeWithSplitOwnership() {
        return new WeakReference<QtObject>(Objects.requireNonNull(createInstanceInNative(), "createInstanceInNative"));
    }

    public void finalize_NotCreatedInJava_JavaOwnership() {
        resetAll();

        WeakReference<QtObject> ref;
        {
            ref = createInstanceInNativeWithJavaOwnership();
        }

        gcAndWait(TIME_LIMIT, null, 1, 1, 1, null, 1);

        test(className(), ref, true,
        		1, //cleanCallerCount
        		isQObject() ? 0 : 1, //destructorFunctionCalledCount
        		0, //disposeCalledCount
        		1, //linkConstructedCount
        		1, //linkDestroyedCount
        		1, //objectInvalidatedCount
        		0, //shellDestructorCalledCount
        		!isQObject() ? 0: 1, //userDataDestroyedCount
        		0, //shellDestroyedCount
        		isSharedPointer() && isQObject() ? 1 : 0 //pointerContainerDestroyedCount
    		);
    }

    private WeakReference<QtObject> createInstanceInNativeWithJavaOwnership() {
        QtObject obj = Objects.requireNonNull(createInstanceInNative(), "createInstanceInNative");
        General.internalAccess.setJavaOwnership(obj);
        return new WeakReference<QtObject>(obj);
    }

    @Test
    public void dispose_CreatedInJava_JavaOwnership() {
        resetAll();

        WeakReference<QtObject> ref;
        {
            ref = createInstanceInJavaAndDisposeIt();
        }

        gcAndWait(TIME_LIMIT, null, 1, 1, 1, 1, 1);

        test(className(), ref, true,
        		0, //cleanCallerCount
        		isQObject() ? 0 : 1, //destructorFunctionCalledCount
        		1, //disposeCalledCount
        		isSharedPointer() ? 2 : 1, //linkConstructedCount
				isSharedPointer() ? 2 : 1, //linkDestroyedCount
				isSharedPointer() ? 2 : 1, //objectInvalidatedCount
        		!hasShellDestructor() ? 0 : (hasInterfaces() ? 3 : 1), //shellDestructorCalledCount
				!isQObject() ? 0: 1, //userDataDestroyedCount
				hasShellDestructor() ? 1 : 0, //shellDestroyedCount
				isSharedPointer() && isQObject() ? 1 : 0 //pointerContainerDestroyedCount
			);
    }

    private WeakReference<QtObject> createInstanceInJavaAndDisposeIt() {
    	WeakReference<QtObject> ref;
    	{
            QtObject obj = createInstanceInJava();
            ref = new WeakReference<QtObject>(obj);
            obj.dispose();
        }

        test(className(), ref, false,
        		0, //cleanCallerCount
        		isQObject() ? 0 : 1, //destructorFunctionCalledCount
        		1, //disposeCalledCount
        		isSharedPointer() ? 2 : 1, //linkConstructedCount
				isSharedPointer() ? 2 : 1, //linkDestroyedCount
				isSharedPointer() ? 2 : 1, //objectInvalidatedCount
        		!hasShellDestructor() ? 0 : (hasInterfaces() ? 3 : 1), //shellDestructorCalledCount
				!isQObject() ? 0: 1, //userDataDestroyedCount
				hasShellDestructor() ? 1 : 0, //shellDestroyedCount
				isSharedPointer() && isQObject() ? 1 : 0 //pointerContainerDestroyedCount
			);
        return ref;
    }

    @Test
    public void dispose_CreatedInJava_CppOwnership() {
        resetAll();

        WeakReference<QtObject> ref;
        {
            ref = createInJavaDisableGCAndDispose();
        }

        gcAndWait(TIME_LIMIT, null, 1, 1, 1, 1, 1);

        test(className(), ref, true, 
        		0, //cleanCallerCount
        		isQObject() ? 0 : 1, //destructorFunctionCalledCount
        		1, //disposeCalledCount
        		isSharedPointer() ? 2 : 1, //linkConstructedCount
				isSharedPointer() ? 2 : 1, //linkDestroyedCount
				isSharedPointer() ? 2 : 1, //objectInvalidatedCount
        		!hasShellDestructor() ? 0 : (hasInterfaces() ? 3 : 1), //shellDestructorCalledCount
				!isQObject() ? 0: 1, //userDataDestroyedCount
				hasShellDestructor() ? 1 : 0, //shellDestroyedCount
				isSharedPointer() && isQObject() ? 1 : 0 //pointerContainerDestroyedCount
			);
    }

    private WeakReference<QtObject> createInJavaDisableGCAndDispose() {
    	WeakReference<QtObject> ref;
    	{
            QtObject obj = createInstanceInJava();
            ref = new WeakReference<QtObject>(obj);
            General.internalAccess.setCppOwnership(obj);
            obj.dispose();
        }

        test(className(), ref, false, 
        		0, //cleanCallerCount
        		isQObject() ? 0 : 1, //destructorFunctionCalledCount
        		1, //disposeCalledCount
        		isSharedPointer() ? 2 : 1, //linkConstructedCount
				isSharedPointer() ? 2 : 1, //linkDestroyedCount
        		isSharedPointer() ? 2 : 1, //objectInvalidatedCount
        		!hasShellDestructor() ? 0 : (hasInterfaces() ? 3 : 1), //shellDestructorCalledCount
				!isQObject() ? 0: 1, //userDataDestroyedCount
				hasShellDestructor() ? 1 : 0, //shellDestroyedCount
				isSharedPointer() && isQObject() ? 1 : 0 //pointerContainerDestroyedCount
			);
        return ref;
    }

    public void dispose_NotCreatedInJava_SplitOwnership() {
        resetAll();

        WeakReference<QtObject> ref;
        {
            ref = createInNativeAndDispose();
        }

        gcAndWait(TIME_LIMIT, null, 1, 1, 1, null, null);

        test(className(), ref, true, 
        		0, //cleanCallerCount
        		isQObject() ? 0 : 1, //destructorFunctionCalledCount
        		1, //disposeCalledCount
        		1, //linkConstructedCount
        		1, //linkDestroyedCount
        		1, //objectInvalidatedCount
        		0, //shellDestructorCalledCount
        		0, //userDataDestroyedCount
        		0, //shellDestroyedCount
        		0 //pointerContainerDestroyedCount
    		);
    }

    private WeakReference<QtObject> createInNativeAndDispose() {
    	WeakReference<QtObject> ref;
    	{
            QtObject obj = Objects.requireNonNull(createInstanceInNative(), "createInstanceInNative");
            ref = new WeakReference<QtObject>(obj);
            obj.dispose();
        }

        test(className(), ref, false, 
        		0, //cleanCallerCount
        		isQObject() ? 0 : 1, //destructorFunctionCalledCount
        		1, //disposeCalledCount
        		1, //linkConstructedCount
        		1, //linkDestroyedCount
        		1, //objectInvalidatedCount
        		0, //shellDestructorCalledCount
        		0, //userDataDestroyedCount
        		0, //shellDestroyedCount
        		0 //pointerContainerDestroyedCount
    		);
        return ref;
    }

    public void dispose_NotCreatedInJava_JavaOwnership() {
        resetAll();

        WeakReference<QtObject> ref;
        {
            ref = createInNativeSetJavaOwnershipAndDispose();
        }

        gcAndWait(TIME_LIMIT, null, 1, 1, 1, null, 1);

        test(className(), ref, true,
        		0, //cleanCallerCount
        		isQObject() ? 0 : 1, //destructorFunctionCalledCount
        		1, //disposeCalledCount
        		1, //linkConstructedCount
        		1, //linkDestroyedCount
        		1, //objectInvalidatedCount
        		0, //shellDestructorCalledCount
        		!isQObject() ? 0: 1, //userDataDestroyedCount
        		0, //shellDestroyedCount
        		isSharedPointer() && isQObject() ? 1 : 0 //pointerContainerDestroyedCount
    		);
    }

    private WeakReference<QtObject> createInNativeSetJavaOwnershipAndDispose() {
    	WeakReference<QtObject> ref;
    	{
            QtObject obj = Objects.requireNonNull(createInstanceInNative(), "createInstanceInNative");
            ref = new WeakReference<QtObject>(obj);
            General.internalAccess.setJavaOwnership(obj);
            obj.dispose();
        }

        test(className(), ref, false, 
        		0, //cleanCallerCount
        		isQObject() ? 0 : 1, //destructorFunctionCalledCount
        		1, //disposeCalledCount
        		1, //linkConstructedCount
        		1, //linkDestroyedCount
        		1, //objectInvalidatedCount
        		0, //shellDestructorCalledCount
        		!isQObject() ? 0: 1, //userDataDestroyedCount
        		0, //shellDestroyedCount
        		isSharedPointer() && isQObject() ? 1 : 0 //pointerContainerDestroyedCount
    		);
        return ref;
    }

    public void dispose_NotCreatedInJava_CppOwnership() {
        resetAll();

        WeakReference<QtObject> ref;
        {
            ref = createInNativeDisableGCAndDispose();
        }

        gcAndWait(TIME_LIMIT, null, 1, 1, 1, null, 1);

        test(className(), ref, true, 
        		0, //cleanCallerCount
        		isQObject() ? 0 : 1, //destructorFunctionCalledCount
        		1, //disposeCalledCount
        		1, //linkConstructedCount
        		1, //linkDestroyedCount
        		1, //objectInvalidatedCount
        		0, //shellDestructorCalledCount
        		!isQObject() ? 0: 1, //userDataDestroyedCount
        		0, //shellDestroyedCount
        		isSharedPointer() && isQObject() ? 1 : 0 //pointerContainerDestroyedCount
    		);
    }

    private WeakReference<QtObject> createInNativeDisableGCAndDispose() {
    	WeakReference<QtObject> ref;
    	{
            QtObject obj = Objects.requireNonNull(createInstanceInNative(), "createInstanceInNative");
            Objects.requireNonNull(obj, "createInstanceInNative() needs to be not null.");
            General.internalAccess.setCppOwnership(obj);
            ref = new WeakReference<QtObject>(obj);
            obj.dispose();
        }

        test(className(), ref, false, 
        		0, //cleanCallerCount
        		isQObject() ? 0 : 1, //destructorFunctionCalledCount
        		1, //disposeCalledCount
        		1, //linkConstructedCount
        		1, //linkDestroyedCount
        		1, //objectInvalidatedCount
        		0, //shellDestructorCalledCount
        		!isQObject() ? 0: 1, //userDataDestroyedCount
        		0, //shellDestroyedCount
        		isSharedPointer() && isQObject() ? 1 : 0 //pointerContainerDestroyedCount
    		);
        return ref;
    }

    public void nativeDelete_CreatedInJava_CppOwnership() {
        resetAll();

        WeakReference<QtObject> ref;
        {
            ref = createInJavaDisableGCAndDeleteInNative();
        }


    	try {
	        if (hasVirtualDestructor()) {
	            gcAndWait(TIME_LIMIT, null, 1, 1, 1, 1, 1);
	            test(className(), ref, false, 
	            		0, //cleanCallerCount
	            		0, //destructorFunctionCalledCount
	            		0, //disposeCalledCount
	            		1, //linkConstructedCount
	            		1, //linkDestroyedCount
	            		1, //objectInvalidatedCount
	            		!hasShellDestructor() ? 0 : (hasInterfaces() ? 3 : 1), //shellDestructorCalledCount
        				!isQObject() ? 0: 1, //userDataDestroyedCount
        				hasShellDestructor() ? 1 : 0, //shellDestroyedCount
						isSharedPointer() && isQObject() ? 1 : 0 //pointerContainerDestroyedCount
					);
	        } else {
	            gcAndWait(TIME_LIMIT, Long.valueOf(250), null, 1, null, null, null);	// FIXME nothing to test/wait for ?
	            test(className(), ref, false, 
	            		0, //cleanCallerCount
	            		0, //destructorFunctionCalledCount
	            		0, //disposeCalledCount
	            		1, //linkConstructedCount
	            		0, //linkDestroyedCount
	            		0, //objectInvalidatedCount
	            		0, //shellDestructorCalledCount
	            		0, //userDataDestroyedCount
	            		0, //shellDestroyedCount
	            		0 //pointerContainerDestroyedCount
            		);
	        }
    	}finally {
    		General.internalAccess.invalidateObject(ref.get());
    	}
    }

    private WeakReference<QtObject> createInJavaDisableGCAndDeleteInNative() {
    	WeakReference<QtObject> ref;
    	{
            QtObject obj = createInstanceInJava();
            ref = new WeakReference<QtObject>(obj);
            General.internalAccess.setCppOwnership(obj);

            deleteLastInstance();
            if (hasVirtualDestructor())
                assertTrue(obj.isDisposed());
        }

        if (hasVirtualDestructor())
            test(className(), ref, false, 
            		0, //cleanCallerCount
            		0, //destructorFunctionCalledCount
            		0, //disposeCalledCount
            		1, //linkConstructedCount
            		1, //linkDestroyedCount
            		1, //objectInvalidatedCount
            		!hasShellDestructor() ? 0 : (hasInterfaces() ? 3 : 1), //shellDestructorCalledCount
    				!isQObject() ? 0: 1, //userDataDestroyedCount
    				hasShellDestructor() ? 1 : 0, //shellDestroyedCount
					isSharedPointer() && isQObject() ? 1 : 0 //pointerContainerDestroyedCount
				);
        else
            test(className(), ref, false, 
            		0, //cleanCallerCount
            		0, //destructorFunctionCalledCount
            		0, //disposeCalledCount
            		1, //linkConstructedCount
            		0, //linkDestroyedCount
            		0, //objectInvalidatedCount
            		0, //shellDestructorCalledCount
            		0, //userDataDestroyedCount
            		0, //shellDestroyedCount
            		0 //pointerContainerDestroyedCount
        		);
        return ref;
    }

    // Many objects leak in this test. Cases that lead to this scenario
    // must be specially handled to avoid memory leaks.
    public void nativeDelete_NotCreatedInJava_CppOwnership() {
    	WeakReference<QtObject> ref;
    	java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINEST, "nativeDelete_NotCreatedInJava_CppOwnership(): MARK1");
        resetAll();

        java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINEST, "nativeDelete_NotCreatedInJava_CppOwnership(): MARK2");
        {
            ref = createInNativeDisableGCAndDeleteInNative();
        }
        java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINEST, "nativeDelete_NotCreatedInJava_CppOwnership(): MARK3");

        try {
	        if(isQObject()) {
	            gcAndWait(TIME_LIMIT, null, 1, 1, 1, null, 1);
	            test(className(), ref, false, 
	            		0, //cleanCallerCount
	            		0, //destructorFunctionCalledCount
	            		0, //disposeCalledCount
	            		1, //linkConstructedCount
	            		1, //linkDestroyedCount
	            		1, //objectInvalidatedCount
	            		0, //shellDestructorCalledCount
	            		!isQObject() ? 0: 1, //userDataDestroyedCount
	            		0, //shellDestroyedCount
	            		0 //pointerContainerDestroyedCount
            		);
	        } else {
	            gcAndWait(TIME_LIMIT, null, null, 1, null, null, 1);	// userDataDestroyedCount
	            test(className(), ref, false, 
	            		0, //cleanCallerCount
	            		0, //destructorFunctionCalledCount
	            		0, //disposeCalledCount
	            		1, //linkConstructedCount
	            		0, //linkDestroyedCount
	            		0, //objectInvalidatedCount
	            		0, //shellDestructorCalledCount
	            		!isQObject() ? 0: 1, //userDataDestroyedCount
	            		0, //shellDestroyedCount
	            		0 //pointerContainerDestroyedCount
            		);
	        }
    	}finally {
    		General.internalAccess.invalidateObject(ref.get());
    	}
    }

    // method exists to ensure object reference created here is discarded for GC
    private WeakReference<QtObject> createInNativeDisableGCAndDeleteInNative_internal() {
        QtObject obj = Objects.requireNonNull(createInstanceInNative(), "createInstanceInNative");
        WeakReference<QtObject> ref = new WeakReference<QtObject>(obj);
        General.internalAccess.setCppOwnership(obj);
        return ref;
    }

    private WeakReference<QtObject> createInNativeDisableGCAndDeleteInNative() {
		java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINEST, "createInNativeDisableGCAndDeleteInNative() MARK1 class=" + getClass().getName());
		WeakReference<QtObject> ref = createInNativeDisableGCAndDeleteInNative_internal();
		java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINEST, "createInNativeDisableGCAndDeleteInNative() MARK2");
        test(className(), ref, false, 
        		0, //cleanCallerCount
        		0, //destructorFunctionCalledCount
        		0, //disposeCalledCount
        		1, //linkConstructedCount
        		0, //linkDestroyedCount
        		0, //objectInvalidatedCount
        		0, //shellDestructorCalledCount
        		0, //userDataDestroyedCount
        		0, //shellDestroyedCount
        		0 //pointerContainerDestroyedCount
    		);
        java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINEST, "createInNativeDisableGCAndDeleteInNative() MARK3");

        deleteLastInstance();
        test(className(), ref, false, 
        		0, //cleanCallerCount
        		0, //destructorFunctionCalledCount
        		0, //disposeCalledCount
        		1, //linkConstructedCount
        		isQObject() ? 1 : 0, //linkDestroyedCount
				isQObject() ? 1 : 0, //objectInvalidatedCount
				0, //shellDestructorCalledCount
				!isQObject() ? 0: 1, //userDataDestroyedCount
				0, //shellDestroyedCount
				0 //pointerContainerDestroyedCount
			);
        return ref;
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
            ", " + excessLimitMillis +
            ", " + finalizedCount +
            ", " + linkConstructedCount +
            ", " + linkDestroyedCount +
            ", " + shellDestructorCalledCount +
            ", " + userDataDestroyedCount + ")";
        while(loop) {
            try {
                if (needsEventProcessing()) {
                    java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINER, debugPrefix + ": QApplication.processEvents()");
                    QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
                    QApplication.processEvents();
                }

                ApplicationInitializer.runGC();

                if (needsEventProcessing()) {
                    java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINER, debugPrefix + ": QApplication.processEvents()");
                    QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
                    QApplication.processEvents();
                }
                
                Reference<? extends Object> thisWr;
                while((thisWr = weakReferenceQueue.poll()) != null) {
                    Integer tmpId;
                    synchronized(MemoryManagement.class) {
                        tmpId = weakReferenceMap.remove(thisWr);
                    }
                    java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINER, " weakReferenceQueue.remove(): dequeued id=" + tmpId);
                }
                Reference<? extends Object> thisPr;
                while((thisPr = phantomReferenceQueue.poll()) != null) {
                    Integer tmpId;
                    synchronized(MemoryManagement.class) {
                        tmpId = phantomReferenceMap.remove(thisPr);
                    }
                    java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINER, " phantomReferenceQueue.remove(): dequeued id=" + tmpId);
                }

                if(!obtainGoal) {	// Try to obtainGoal
                    currentFinalizedCount = DebugTools.cleanCallerCount(className());
                    currentLinkConstructedCount = DebugTools.linkConstructedCount(className());
                    currentLinkDestroyedCount = DebugTools.linkDestroyedCount(className());
                    currentShellDestructorCalledCount = DebugTools.shellDestructorCalledCount(className());
                    currentUserDataDestroyedCount = DebugTools.userDataDestroyedCount(className());

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

                    java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINER, debugPrefix + ": elapsed=" + elapsed + "; end loop" +
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
                java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINEST, debugPrefix + ": elapsed=" + elapsed + "; loop="+loop+"; obtainGoal="+obtainGoal);

                if(loop)
                    Thread.sleep(10);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        // Report on status
        java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "elapsed=" + elapsed + "; alive=" + alive + "; aliveAndUnderTest=" + aliveAndUnderTest +
                "; weakReferenceMapSize=" + weakReferenceMap.values() + "; phantomReferenceMapSize=" + phantomReferenceMap.values());

        return obtainGoal;
    }

    public void nativeDelete_NotCreatedInJava_SplitOwnership() {
        resetAll();

        WeakReference<QtObject> ref;
        {
            ref = createInNativeDeleteInNative();
        }

        gcAndWait(TIME_LIMIT, null, 1, 1, 1, null, 1);

        test(className(), ref, true, 
        		1, //cleanCallerCount
        		0, //destructorFunctionCalledCount
        		0, //disposeCalledCount
        		1, //linkConstructedCount
        		1, //linkDestroyedCount
        		1, //objectInvalidatedCount
        		0, //shellDestructorCalledCount
        		!isQObject() ? 0: 1, //userDataDestroyedCount
        		0, //shellDestroyedCount
        		0 //pointerContainerDestroyedCount
    		);
    }

    private WeakReference<QtObject> createInNativeDeleteInNative() {
    	WeakReference<QtObject> ref;
    	{
            QtObject obj = Objects.requireNonNull(createInstanceInNative(), "createInstanceInNative");
        	ref = new WeakReference<QtObject>(obj);
        }

        deleteLastInstance();
        test(className(), ref, false,
        		0, //cleanCallerCount
        		0, //destructorFunctionCalledCount
        		0, //disposeCalledCount
        		1, //linkConstructedCount
        		0, //linkDestroyedCount
        		0, //objectInvalidatedCount
        		0, //shellDestructorCalledCount
        		!isQObject() ? 0: 1, //userDataDestroyedCount
        		0, //shellDestroyedCount
        		0 //pointerContainerDestroyedCount
    		);
        return ref;
    }

    public void invalidate_NotCreatedInJava_JavaOwnership() {
        resetAll();

        WeakReference<QtObject> ref;
        {
            ref = createInNativeSetJavaOwnershipAndInvalidate();
        }

        if(isQObject()) {
            gcAndWait(TIME_LIMIT, null, 1, 1, 1, null, 1);

            test(className(), ref, true,
            		1, //cleanCallerCount
            		0, //destructorFunctionCalledCount
            		0, //disposeCalledCount
            		1, //linkConstructedCount
            		1, //linkDestroyedCount
            		1, //objectInvalidatedCount
            		0, //shellDestructorCalledCount
            		1, //userDataDestroyedCount
            		0, //shellDestroyedCount
            		isSharedPointer() && isQObject() ? 1 : 0 //pointerContainerDestroyedCount
        		);
        } else if(isSharedPointer()) {
            gcAndWait(TIME_LIMIT, null, 1, 1, 1, null, 1);

            test(className(), ref, true,
            		1, //cleanCallerCount
            		1, //destructorFunctionCalledCount
            		0, //disposeCalledCount
            		1, //linkConstructedCount
            		1, //linkDestroyedCount
            		1, //objectInvalidatedCount
            		0, //shellDestructorCalledCount
            		0, //userDataDestroyedCount
            		0, //shellDestroyedCount
            		0 //pointerContainerDestroyedCount
        		);
        } else {
            gcAndWait(TIME_LIMIT, null, 2, 2, 2, null, 1);

            test(className(), ref, true,
            		2, //cleanCallerCount
            		1, //destructorFunctionCalledCount
            		0, //disposeCalledCount
            		2, //linkConstructedCount
            		2, //linkDestroyedCount
            		2, //objectInvalidatedCount
            		0, //shellDestructorCalledCount
            		0, //userDataDestroyedCount
            		0, //shellDestroyedCount
            		0 //pointerContainerDestroyedCount
        		);
        }
    }

    private WeakReference<QtObject> createInNativeSetJavaOwnershipAndInvalidate() {
    	WeakReference<QtObject> ref;
    	{
            QtObject obj = Objects.requireNonNull(createInstanceInNative(), "createInstanceInNative");
            ref = new WeakReference<QtObject>(obj);
            QtObject obj2 = invalidateObject(obj, true);

            // Java owned objects are not invalidated
            if(isSharedPointer()) {
                assertEquals(obj, obj2);
            }else {
            	assertTrue(obj2!=null && !obj2.isDisposed());
            }
        }

        test(className(), ref, false,
        		0, //cleanCallerCount
        		0, //destructorFunctionCalledCount
        		0, //disposeCalledCount
        		isQObject() || isSharedPointer() ? 1 : 2, //linkConstructedCount
        		0, //linkDestroyedCount
        		0, //objectInvalidatedCount
        		0, //shellDestructorCalledCount
        		0, //userDataDestroyedCount
        		0, //shellDestroyedCount
        		0 //pointerContainerDestroyedCount
    		);
        return ref;
    }
    
    public void invalidate_CreatedInJava_DefaultOwnership() {
        resetAll();

        WeakReference<QtObject> ref;
        {
            ref = createInJavaAndInvalidate();
        }

        if(isQObject()) {
            gcAndWait(TIME_LIMIT, null, 1, 1, 1, null, 1);

            test(className(), ref, true,
            		1, //cleanCallerCount
            		0, //destructorFunctionCalledCount
            		0, //disposeCalledCount
            		isSharedPointer() ? 2 : 1, //linkConstructedCount
    				isSharedPointer() ? 2 : 1, //linkDestroyedCount
					isSharedPointer() ? 2 : 1, //objectInvalidatedCount
            		hasInterfaces() ? 3 : 1, //shellDestructorCalledCount
            		1, //userDataDestroyedCount
            		1, //shellDestroyedCount
            		isSharedPointer() && isQObject() ? 1 : 0 //pointerContainerDestroyedCount
        		);
        } else if(isSharedPointer()) {
            gcAndWait(TIME_LIMIT, null, 1, 1, 1, null, 1);

            test(className(), ref, true,
            		1, //cleanCallerCount
            		1, //destructorFunctionCalledCount
            		0, //disposeCalledCount
            		2, //linkConstructedCount
            		2, //linkDestroyedCount
            		2, //objectInvalidatedCount
            		hasShellDestructor() ? (hasInterfaces() ? 3 : 1) : 0, //shellDestructorCalledCount
            		0, //userDataDestroyedCount
            		hasShellDestructor() ? 1 : 0, //shellDestroyedCount
            		0 //pointerContainerDestroyedCount
        		);
        } else {
            gcAndWait(TIME_LIMIT, null, 2, 2, 2, null, 1);

            test(className(), ref, true,
            		1, //cleanCallerCount
            		1, //destructorFunctionCalledCount
            		0, //disposeCalledCount
            		1, //linkConstructedCount
            		1, //linkDestroyedCount
            		1, //objectInvalidatedCount
            		hasShellDestructor() ? (hasInterfaces() ? 3 : 1) : 0, //shellDestructorCalledCount
            		0, //userDataDestroyedCount
            		hasShellDestructor() ? 1 : 0, //shellDestroyedCount
            		0 //pointerContainerDestroyedCount
        		);
        }
    }
    
    private WeakReference<QtObject> createInJavaAndInvalidate() {
    	WeakReference<QtObject> ref;
    	{
            QtObject obj = createInstanceInJava();
            ref = new WeakReference<QtObject>(obj);
            QtObject obj2 = invalidateObject(obj, true);

            // Java owned objects are not invalidated
            assertTrue(obj2!=null && !obj2.isDisposed());
            assertEquals(obj, obj2);
        }

        test(className(), ref, false,
        		0, //cleanCallerCount
        		0, //destructorFunctionCalledCount
        		0, //disposeCalledCount
        		isSharedPointer() ? 2 : 1, //linkConstructedCount
				isSharedPointer() ? 1 : 0, //linkDestroyedCount
				isSharedPointer() ? 1 : 0, //objectInvalidatedCount
        		0, //shellDestructorCalledCount
        		0, //userDataDestroyedCount
        		0, //shellDestroyedCount
        		0 //pointerContainerDestroyedCount
    		);
        return ref;
    }

    // Note that there are two Java objects in use here,
    // because there is an extra wrapper created in
    // the virtual call from C++ to Java in invalidateObject().
    public void invalidate_NotCreatedInJava_SplitOwnership() {
        resetAll();

        WeakReference<QtObject> ref;
        {
            ref = createInNativeAndInvalidate();
        }

        gcAndWait(TIME_LIMIT, null, 2, 2, 2, null, null);

        test(className(), ref, true,
        		1, //cleanCallerCount
        		isSharedPointer() ? 1 : 0, //destructorFunctionCalledCount
        		0, //disposeCalledCount
        		isSharedPointer() ? 1 : 2, //linkConstructedCount
				isSharedPointer() ? 1 : 2, //linkDestroyedCount
				isSharedPointer() ? 1 : 2, //objectInvalidatedCount
        		0, //shellDestructorCalledCount
        		0, //userDataDestroyedCount
        		0, //shellDestroyedCount
        		0 //pointerContainerDestroyedCount
    		);
    }

    private WeakReference<QtObject> createInNativeAndInvalidate() {
    	WeakReference<QtObject> ref;
    	{
            QtObject obj = Objects.requireNonNull(createInstanceInNative(), "createInstanceInNative");
            if(isSharedPointer()) {
            	assertTrue(General.internalAccess.isJavaOwnership(obj));
            }else{
            	assertTrue(General.internalAccess.isSplitOwnership(obj));
            }
            ref = new WeakReference<QtObject>(obj);
            invalidateObject(obj, false);
        }

        test(className(), ref, false,
        		0, //cleanCallerCount
        		0, //destructorFunctionCalledCount
        		0, //disposeCalledCount
        		isSharedPointer() ? 1 : 2, //linkConstructedCount
				isSharedPointer() ? 0 : 1, //linkDestroyedCount
				isSharedPointer() ? 0 : 1, //objectInvalidatedCount
        		0, //shellDestructorCalledCount
        		0, //userDataDestroyedCount
        		0, //shellDestroyedCount
        		0 //pointerContainerDestroyedCount
    		);
        return ref;
    }
    
    public void finalize_CreatedInJavaWithException_JavaOwnership() {
        resetAll();
        WeakReference<QtObject> ref;
        {
            ref = createInstanceInJavaWithException();
        }

        gcAndWait(TIME_LIMIT, null, 1, 1, 1, 1, 1);

        test(className2(), ref, true, 
        		1, //cleanCallerCount
        		isQObject() ? 0 : 1, //destructorFunctionCalledCount
        		0, //disposeCalledCount
        		isSharedPointer() ? 2 : 1, //linkConstructedCount
				isSharedPointer() ? 2 : 1, //linkDestroyedCount
				isSharedPointer() ? 2 : 1, //objectInvalidatedCount
        		!hasShellDestructor() ? 0 : (hasInterfaces() ? 3 : 1), //shellDestructorCalledCount
				!isQObject() ? 0: 1, //userDataDestroyedCount
        		hasShellDestructor() ? 1 : 0, //shellDestroyedCount
				isSharedPointer() && isQObject() ? 1 : 0 //pointerContainerDestroyedCount
			);

    }
}
