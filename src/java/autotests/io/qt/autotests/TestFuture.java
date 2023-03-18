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

import java.util.Arrays;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.Test;

import io.qt.QMissingVirtualOverridingException;
import io.qt.QNoImplementationException;
import io.qt.QtPrivateOverride;
import io.qt.autotests.generated.FutureHandler;
//import io.qt.concurrent.QtConcurrent;
import io.qt.core.QEventLoop;
import io.qt.core.QException;
import io.qt.core.QFuture;
import io.qt.core.QFutureInterface;
import io.qt.core.QFutureInterfaceBase;
import io.qt.core.QFutureWatcher;
import io.qt.core.QFutureWatcherBase;
import io.qt.core.QLibraryInfo;
import io.qt.core.QObject;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QUnhandledException;

public class TestFuture extends ApplicationInitializer {
	
	static class IncompleteFutureWatcher extends QFutureWatcherBase{
	}
	
	static class CompleteFutureWatcher extends IncompleteFutureWatcher{
		
		private QFutureInterfaceBase ifc = new QFutureInterfaceBase();
		
		@QtPrivateOverride
		private QFutureInterfaceBase futureInterface() {
			return ifc;
		}
	}
	
    @Test
    public void testSubclassingFutureWatcher() {
    	try {
			new IncompleteFutureWatcher();
			Assert.assertFalse("exception expected to be thrown", true);
		} catch (QMissingVirtualOverridingException e) {
		}
    	new CompleteFutureWatcher();
    }
	
    @Test
    public void testNativeQFuture() {
    	QFuture<String> future = FutureHandler.returnInTheFuture(Arrays.asList("testNativeQFuture1", "testNativeQFuture_nested"), 200);
    	future.waitForFinished();
		Assert.assertFalse(future.isCanceled());
    	Assert.assertEquals(2, future.resultCount());
    	Assert.assertEquals("testNativeQFuture1", future.resultAt(0));
    	Assert.assertEquals("testNativeQFuture_nested", future.resultAt(1));
    }
    
    @Test
    public void testNativeQFuture_nested() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	QFuture<String> future = FutureHandler.forward(FutureHandler.returnInTheFuture(Arrays.asList("testNativeQFuture1", "testNativeQFuture_nested"), 200));
    	future.waitForFinished();
		Assert.assertFalse(future.isCanceled());
    	Assert.assertEquals(2, future.resultCount());
    	Assert.assertEquals("testNativeQFuture1", future.resultAt(0));
    	Assert.assertEquals("testNativeQFuture_nested", future.resultAt(1));
    }
    
    @Test
    public void testJavaQFutureWatcher() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	QFutureWatcher<String> watcher = new QFutureWatcher<>();
    	QEventLoop loop = new QEventLoop();
    	int[] count = {0};
    	watcher.resultReadyAt.connect(i-> count[0] = Math.max(count[0], i));
    	watcher.resultsReadyAt.connect((a,b)-> count[0] = Math.max(count[0], b));
    	watcher.finished.connect(loop::quit);
    	watcher.setFuture(FutureHandler.returnInTheFuture(Arrays.asList("testNativeQFutureWatcher1",
    																	"testNativeQFutureWatcher2",
    																	"testNativeQFutureWatcher3",
    																	"testNativeQFutureWatcher4",
    																	"testNativeQFutureWatcher5",
    																	"testNativeQFutureWatcher6"), 500));
    	loop.exec();
		Assert.assertFalse(watcher.isCanceled());
    	Assert.assertEquals(6, count[0]);
    	Assert.assertEquals("testNativeQFutureWatcher1", watcher.resultAt(0));
    	Assert.assertEquals("testNativeQFutureWatcher2", watcher.resultAt(1));
    	Assert.assertEquals("testNativeQFutureWatcher3", watcher.resultAt(2));
    	Assert.assertEquals("testNativeQFutureWatcher4", watcher.resultAt(3));
    	Assert.assertEquals("testNativeQFutureWatcher5", watcher.resultAt(4));
    	Assert.assertEquals("testNativeQFutureWatcher6", watcher.resultAt(5));
    }
    
    @Test
    public void testJavaQFutureWatcher_nested() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	QFutureWatcher<String> watcher = new QFutureWatcher<>();
    	QEventLoop loop = new QEventLoop();
    	int[] count = {0};
    	watcher.resultReadyAt.connect(i-> count[0] = Math.max(count[0], i));
    	watcher.resultsReadyAt.connect((a,b)-> count[0] = Math.max(count[0], b));
    	watcher.finished.connect(loop::quit);
    	watcher.setFuture(FutureHandler.forward(FutureHandler.returnInTheFuture(Arrays.asList("testNativeQFutureWatcher1",
    																	"testNativeQFutureWatcher2",
    																	"testNativeQFutureWatcher3",
    																	"testNativeQFutureWatcher4",
    																	"testNativeQFutureWatcher5",
    																	"testNativeQFutureWatcher6"), 500)));
    	loop.exec();
		Assert.assertFalse(watcher.isCanceled());
    	Assert.assertEquals(6, count[0]);
    	Assert.assertEquals("testNativeQFutureWatcher1", watcher.resultAt(0));
    	Assert.assertEquals("testNativeQFutureWatcher2", watcher.resultAt(1));
    	Assert.assertEquals("testNativeQFutureWatcher3", watcher.resultAt(2));
    	Assert.assertEquals("testNativeQFutureWatcher4", watcher.resultAt(3));
    	Assert.assertEquals("testNativeQFutureWatcher5", watcher.resultAt(4));
    	Assert.assertEquals("testNativeQFutureWatcher6", watcher.resultAt(5));
    }
    
    @Test
    public void testNativeQFutureWatcher() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	QFutureWatcher<String> watcher = FutureHandler.watcherString();
    	try {
	    	QEventLoop loop = new QEventLoop();
	    	int[] count = {0};
	    	watcher.resultReadyAt.connect(i-> count[0] = Math.max(count[0], i));
	    	watcher.resultsReadyAt.connect((a,b)-> count[0] = Math.max(count[0], b));
	    	watcher.finished.connect(loop::quit);
	    	QFuture<String> future = FutureHandler.returnInTheFuture(Arrays.asList("testNativeQFutureWatcher1",
					"testNativeQFutureWatcher2",
					"testNativeQFutureWatcher3",
					"testNativeQFutureWatcher4",
					"testNativeQFutureWatcher5",
					"testNativeQFutureWatcher6"), 500);
	    	watcher.setFuture(future);
	    	loop.exec();
			Assert.assertFalse(watcher.isCanceled());
	    	Assert.assertEquals(6, count[0]);
	    	Assert.assertEquals("testNativeQFutureWatcher1", watcher.resultAt(0));
	    	Assert.assertEquals("testNativeQFutureWatcher2", watcher.resultAt(1));
	    	Assert.assertEquals("testNativeQFutureWatcher3", watcher.resultAt(2));
	    	Assert.assertEquals("testNativeQFutureWatcher4", watcher.resultAt(3));
	    	Assert.assertEquals("testNativeQFutureWatcher5", watcher.resultAt(4));
	    	Assert.assertEquals("testNativeQFutureWatcher6", watcher.resultAt(5));
    	}finally {
    		watcher.dispose();
    	}
    }
    
    @Test
    public void testJavaToNativeQFutureWatcher_String() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	QFutureWatcher<String> watcher = new QFutureWatcher<>();
    	watcher.setFuture(FutureHandler.returnInTheFuture(Arrays.asList("testJavaToNativeQFutureWatcher_String"), 500));
    	Assert.assertEquals("testJavaToNativeQFutureWatcher_String", FutureHandler.checkWatcherString(watcher));
    }
    
    @SuppressWarnings({ "unchecked", "rawtypes" })
	@Test
    public void testJavaToNativeQFutureWatcher_Void() {
    	QFutureWatcher watcher = new QFutureWatcher();
    	watcher.setFuture(FutureHandler.finishInTheFuture(500));
    	try{
    		FutureHandler.checkWatcherString(watcher);
			Assert.assertFalse("Exception expected to be thrown.", true);
		} catch (IllegalArgumentException e) {
			Assert.assertEquals("Cannot cast QFutureWatcher<void> to QFutureWatcher<QString>.", e.getMessage());
    	}
    	FutureHandler.checkWatcherVoid(watcher);
    	Assert.assertEquals(0, watcher.children().size());
    	watcher.setFuture(FutureHandler.returnInTheFuture(Arrays.asList("testJavaToNativeQFutureWatcher_Void"), 500));
    	try{
    		Assert.assertEquals(null, FutureHandler.checkWatcherObject(watcher));
			Assert.assertFalse("Exception expected to be thrown.", true);
		} catch (IllegalArgumentException e) {
			Assert.assertEquals("Cannot cast QFutureWatcher<QString> to QFutureWatcher<QObject*>.", e.getMessage());
    	}
    }
    
    @SuppressWarnings({ "unchecked", "rawtypes" })
	@Test
    public void testNativeQFutureWatcher_void() {
    	QFutureWatcher<Void> watcher = FutureHandler.watcherVoid();
    	try {
	    	QEventLoop loop = new QEventLoop();
	    	int[] count = {0};
	    	watcher.resultReadyAt.connect(i-> count[0] = Math.max(count[0], i));
	    	watcher.resultsReadyAt.connect((a,b)-> count[0] = Math.max(count[0], b));
	    	watcher.finished.connect(loop::quit);
	    	QFuture<String> future = FutureHandler.returnInTheFuture(Arrays.asList("testNativeQFutureWatcher1",
					"testNativeQFutureWatcher2",
					"testNativeQFutureWatcher3",
					"testNativeQFutureWatcher4",
					"testNativeQFutureWatcher5",
					"testNativeQFutureWatcher6"), 500);
	    	watcher.setFuture((QFuture)future);
	    	loop.exec();
			Assert.assertFalse(watcher.isCanceled());
	    	Assert.assertEquals(6, count[0]);
	    	try {
	    		watcher.resultAt(0);
				Assert.assertFalse("Exception expected to be thrown.", true);
			} catch (QNoImplementationException e) {
				Assert.assertEquals("resultAt(int) not available for QFutureWatcher<void>.", e.getMessage());
			}
    	}finally {
    		watcher.dispose();
    	}
    }
    
    @Test
    public void testNativeQFuture_Void() {
    	QFuture<Void> future = FutureHandler.finishInTheFuture(200);
    	future.waitForFinished();
		Assert.assertFalse(future.isCanceled());
		try {
			future.result();
			Assert.assertFalse("exception expected to be thrown", true);
		} catch (QNoImplementationException e) {
			Assert.assertEquals("result() not available for QFuture<void>.", e.getMessage());
		}
    }
    
    @Test
	@SuppressWarnings({ "unchecked", "rawtypes" })
    public void test_type_missmatch() {
    	{
    		QFuture<Integer> future = (QFuture<Integer>)(QFuture)FutureHandler.finishInTheFuture(200);
        	Assert.assertTrue(future.isStarted());
        	future = FutureHandler.forwardInt(future);
        	Assert.assertFalse(future.isStarted());
    	}
    	
    	{
    		QFutureInterface<Integer> promise = new QFutureInterface<>();
    		promise.reportStarted();
    		QFuture<QObject> future = (QFuture<QObject>)(QFuture)promise.future();
        	Assert.assertTrue(future.isStarted());
        	future = FutureHandler.forwardObject(future);
        	promise.reportResult(8);
        	promise.reportResult(16);
        	promise.reportResult(5);
        	promise.reportFinished();
        	Assert.assertEquals(3, future.resultCount());
        	Assert.assertEquals(null, future.result());
    	}
    	
    	{
    		QFutureInterface<String> promise = new QFutureInterface<>();
    		promise.reportStarted();
    		QFuture<Integer> future = (QFuture<Integer>)(QFuture)promise.future();
        	Assert.assertTrue(future.isStarted());
        	future = FutureHandler.forwardInt(future);
        	promise.reportResult("5");
        	promise.reportFinished();
        	Assert.assertEquals(1, future.resultCount());
        	Assert.assertEquals(Integer.valueOf(5), future.result());
    	}
    }
    
    @Test
    public void testExceptionNativeQFuture() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	QFuture<String> future = FutureHandler.throwInTheFuture(Arrays.asList("testNativeQFuture1", "testNativeQFuture_nested"), 200);
    	try {
			future.waitForFinished();
			Assert.assertFalse("exception expected to be thrown", true);
		} catch (QException e) {
			Assert.assertEquals("An exception has been thrown in native code.", e.getMessage());
		}
    	Assert.assertTrue(future.isCanceled());
    }
    
    @Test
    public void testExceptionNativeQFuture_nested() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	QFuture<String> future = FutureHandler.forward(FutureHandler.throwInTheFuture(Arrays.asList("testNativeQFuture1", "testNativeQFuture_nested"), 200));
    	try {
			future.waitForFinished();
			Assert.assertFalse("exception expected to be thrown", true);
		} catch (QException e) {
			Assert.assertEquals("An exception has been thrown in native code.", e.getMessage());
		}
    	Assert.assertTrue(future.isCanceled());
    }
    
    @Test
    public void testExceptionNativeQFuture_Void() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	boolean isQt5 = QLibraryInfo.version().majorVersion()==5;
    	/*
    	for (int i = 0; i < 9; i++) {
    		QFuture<Void> future = FutureHandler.throwInTheFuture(20, i);
	    	try {
	    		future.waitForFinished();
			} catch (Exception e) {
				System.out.println(i+": "+e.getClass().getName()+" -> "+e.getMessage());
			}
		}*/
    	QFuture<Void> future = FutureHandler.throwInTheFuture(200);
    	try {
    		future.waitForFinished();
        	Assert.assertFalse("exception expected to be thrown", true);
		} catch (QUnhandledException e) {
			Assert.assertFalse("should not reach this", true);
		} catch (QException e) {
			Assert.assertEquals("An exception has been thrown in native code.", e.getMessage());
		}
		Assert.assertTrue(future.isCanceled());
		
		future = FutureHandler.throwInTheFuture(200, /*exceptionType*/ 1);
    	try {
    		future.waitForFinished();
        	Assert.assertFalse("exception expected to be thrown", true);
		} catch (QUnhandledException e) {
			Assert.assertFalse("should not reach this", true);
		} catch (QException e) {
			Assert.assertEquals("An exception (FutureException) has been thrown in native code.", e.getMessage());
		}
		Assert.assertTrue(future.isCanceled());
		
		future = FutureHandler.throwInTheFuture(200, /*exceptionType*/ 2);
    	try {
    		future.waitForFinished();
        	Assert.assertFalse("exception expected to be thrown", true);
		} catch (QUnhandledException e) {
			Assert.assertFalse("should not reach this", true);
		} catch (QException e) {
			Assert.assertEquals("An exception (FutureError) has been thrown in native code.", e.getMessage());
		}
		Assert.assertTrue(future.isCanceled());
		
		future = FutureHandler.throwInTheFuture(200, /*exceptionType*/ 3);
    	try {
    		future.waitForFinished();
        	Assert.assertFalse("exception expected to be thrown", true);
		} catch (QUnhandledException e) {
			if(isQt5)
				Assert.assertEquals("An exception has been thrown in native code.", e.getMessage());
			else
				Assert.assertEquals("An exception (std::invalid_argument) has been thrown in native code: X", e.getMessage());
		}
		Assert.assertTrue(future.isCanceled());
		
		future = FutureHandler.throwInTheFuture(200, /*exceptionType*/ 4);
    	try {
    		future.waitForFinished();
        	Assert.assertFalse("exception expected to be thrown", true);
		} catch (QUnhandledException e) {
			if(isQt5)
				Assert.assertEquals("An exception has been thrown in native code.", e.getMessage());
			else
				Assert.assertEquals("An exception (std::underflow_error) has been thrown in native code: X", e.getMessage());
		}
		Assert.assertTrue(future.isCanceled());
		
		future = FutureHandler.throwInTheFuture(200, /*exceptionType*/ 5);
    	try {
    		future.waitForFinished();
        	Assert.assertFalse("exception expected to be thrown", true);
		} catch (QUnhandledException e) {
			if(isQt5)
				Assert.assertEquals("An exception has been thrown in native code.", e.getMessage());
			else
				Assert.assertTrue(e.getMessage().startsWith("An exception (std::bad_alloc) has been thrown in native code"));
		}
		Assert.assertTrue(future.isCanceled());
		
		future = FutureHandler.throwInTheFuture(200, /*exceptionType*/ 6);
    	try {
    		future.waitForFinished();
        	Assert.assertFalse("exception expected to be thrown", true);
		} catch (QUnhandledException e) {
			Assert.assertEquals("An exception has been thrown in native code.", e.getMessage());
		}
		Assert.assertTrue(future.isCanceled());
		
		future = FutureHandler.throwInTheFuture(200, /*exceptionType*/ 7);
    	try {
    		future.waitForFinished();
        	Assert.assertFalse("exception expected to be thrown", true);
		} catch (QUnhandledException e) {
			Assert.assertEquals("An exception has been thrown in native code.", e.getMessage());
		}
		Assert.assertTrue(future.isCanceled());
		
		future = FutureHandler.throwInTheFuture(200, /*exceptionType*/ 8);
    	try {
    		future.waitForFinished();
        	Assert.assertFalse("exception expected to be thrown", true);
		} catch (QUnhandledException e) {
			Assert.assertEquals("An exception has been thrown in native code.", e.getMessage());
		}
		Assert.assertTrue(future.isCanceled());
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestFuture.class.getName());
    }
}
