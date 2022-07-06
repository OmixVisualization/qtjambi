package io.qt.autotests;

import static io.qt.autotests.TestConcurrent.COUNT;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;

import java.util.ArrayList;
import java.util.List;

import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.Assume;
import org.junit.Test;

import io.qt.QNoImplementationException;
import io.qt.autotests.generated.FutureHandler;
import io.qt.concurrent.QtConcurrent;
import io.qt.concurrent.QtConcurrent.QTypedPromiseTaskBuilder1Arg1;
import io.qt.concurrent.QtConcurrent.QTypedTaskBuilder1Arg1;
import io.qt.core.QFuture;
import io.qt.core.QFutureInterface;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QPromise;
import io.qt.core.QThread;
import io.qt.core.QThreadPool;

public class TestConcurrentQt6 extends ApplicationInitializer {
	
	private static QThreadPool pool = new QThreadPool();
	
	@AfterClass
    public static void testDispose() throws Exception {
		if(pool.activeThreadCount()>0) {
			System.out.println("Waiting for threads to finish...");
			pool.waitForDone();
		}
		pool.dispose();
		pool = null;
		ApplicationInitializer.testDispose();
    }
	
	@Test
    public void testBlockingMappedWithThreadPool() {
        List<String> strings = new ArrayList<String>();
        for (int i=0; i<COUNT; ++i)
            strings.add("" + (i*i));

        List<Integer> results = QtConcurrent.blockingMapped(pool, strings, Integer::parseInt);

        assertEquals(COUNT, strings.size());
        assertEquals(COUNT, results.size());
        for (int i=0; i<results.size(); ++i)
            assertEquals(i*i, (int) results.get(i));
    }
	
	@Test
    public void testRunWithoutPromiseWithThreadPool() {
		int[] result = {0};
		QFuture<Void> future = QtConcurrent.run(pool, number->{
			QThread.msleep(200);
			result[0] = Integer.parseInt(number);
		}, "12340");
		future.waitForFinished();
        assertEquals(12340, result[0]);
	}
	
	@Test
    public void testRunWithPromiseWithThreadPool() {
		QFuture<Integer> future = QtConcurrent.run(pool, (QPromise<Integer> promise, String number)->{
			QThread.msleep(200);
			promise.addResult(Integer.parseInt(number));
			promise.addResult(Integer.parseInt(number.substring(1)));
			promise.addResult(Integer.parseInt(number.substring(2)));
			promise.addResult(Integer.parseInt(number.substring(3)));
		}, "12341");
		future.waitForFinished();
        assertEquals(4, future.resultCount());
        assertEquals(12341, (int) future.resultAt(0));
        assertEquals(2341, (int) future.resultAt(1));
        assertEquals(341, (int) future.resultAt(2));
        assertEquals(41, (int) future.resultAt(3));
        try {
			future.resultAt(4);
			assertFalse("IndexOutOfBoundsException expected", true);
		} catch (IndexOutOfBoundsException e) {
		}
	}
	
	@Test
    public void testTaskWithThreadPool() {
		QTypedTaskBuilder1Arg1<Integer, String> task = QtConcurrent.task((String number)->{
			QThread.msleep(200);
			return Integer.parseInt(number);
		}).withArguments("12342").onThreadPool(pool);
		QFuture<Integer> future = task.spawn();
        assertEquals(12342, (int) future.result());
	}
	
	@Test
    public void testTaskWithPromiseWithThreadPool() {
		QTypedPromiseTaskBuilder1Arg1<Integer, String> task = QtConcurrent.task((QPromise<Integer> promise, String number)->{
			QThread.msleep(200);
			promise.addResult(Integer.parseInt(number));
		}).withArguments("12343").onThreadPool(pool);
		QFuture<Integer> future = task.spawn();
        assertEquals(12343, (int) future.result());
	}
	
	@Test
    public void testRunWithoutPromise() {
		int[] result = {0};
		QFuture<Void> future = QtConcurrent.run(number->{
			QThread.msleep(200);
			result[0] = Integer.parseInt(number);
		}, "12344");
		future.waitForFinished();
        assertEquals(12344, result[0]);
	}
	
	@Test
    public void testRunWithPromise() {
		QFuture<Integer> future = QtConcurrent.run((QPromise<Integer> promise, String number)->{
			QThread.msleep(200);
			promise.addResult(Integer.parseInt(number));
			promise.addResult(Integer.parseInt(number.substring(1)));
			promise.addResult(Integer.parseInt(number.substring(2)));
			promise.addResult(Integer.parseInt(number.substring(3)));
		}, "12345");
		future.waitForFinished();
        assertEquals(4, future.resultCount());
        assertEquals(12345, (int) future.result());
        assertEquals(2345, (int) future.resultAt(1));
        assertEquals(345, (int) future.resultAt(2));
        assertEquals(45, (int) future.resultAt(3));
        try {
			future.resultAt(4);
			assertFalse("IndexOutOfBoundsException expected", true);
		} catch (IndexOutOfBoundsException e) {
		}
	}
	
	@Test
    public void testTask() {
		QTypedTaskBuilder1Arg1<Integer, String> task = QtConcurrent.task((String number)->{
			QThread.msleep(200);
			return Integer.parseInt(number);
		}).withArguments("12346");
		QFuture<Integer> future = task.spawn();
        assertEquals(12346, (int) future.result());
	}
	
	@Test
    public void testTaskWithPromise() {
		QTypedPromiseTaskBuilder1Arg1<Integer, String> task = QtConcurrent.task((QPromise<Integer> promise, String number)->{
			QThread.msleep(200);
			promise.addResult(Integer.parseInt(number));
			promise.addResult(Integer.parseInt(number.substring(1)));
			promise.addResult(Integer.parseInt(number.substring(2)));
			promise.addResult(Integer.parseInt(number.substring(3)));
		}).withArguments("12347");
		QFuture<Integer> future = task.spawn();
		future.waitForFinished();
        assertEquals(4, future.resultCount());
        assertEquals(12347, (int) future.result());
        assertEquals(2347, (int) future.resultAt(1));
        assertEquals(347, (int) future.resultAt(2));
        assertEquals(47, (int) future.resultAt(3));
        try {
			future.resultAt(4);
			assertFalse("IndexOutOfBoundsException expected", true);
		} catch (IndexOutOfBoundsException e) {
		}
	}
	
	@Test
    public void testNativePromiseToNativeCast() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
		QFuture<String> future = QtConcurrent.run(pool, (QPromise<String> promise)->{
			QThread.msleep(500);
			promise.addResult("Z");
			FutureHandler.fillStringPromise(promise);
			promise.addResult("E");
		});
		future.waitForFinished();
        assertEquals(6, future.resultCount());
        assertEquals("Z", future.result());
        assertEquals("A", future.resultAt(1));
        assertEquals("B", future.resultAt(2));
        assertEquals("C", future.resultAt(3));
        assertEquals("D", future.resultAt(4));
        assertEquals("E", future.resultAt(5));
	}
	
	@Test
    public void testJavaPromiseToNativeCast() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
		QPromise<String> promise = new QPromise<String>();
		QFuture<String> future = promise.future();
		promise.start();
		QtConcurrent.run(pool, ()->{
			try {
				QThread.msleep(500);
				promise.addResult("Z");
				FutureHandler.fillStringPromise(promise);
				promise.addResult("E");
			}finally {
				promise.finish();
			}
		});
		future.waitForFinished();
        assertEquals(6, future.resultCount());
        assertEquals("Z", future.result());
        assertEquals("A", future.resultAt(1));
        assertEquals("B", future.resultAt(2));
        assertEquals("C", future.resultAt(3));
        assertEquals("D", future.resultAt(4));
        assertEquals("E", future.resultAt(5));
	}
	
	@Test
	@SuppressWarnings({ "unchecked", "rawtypes" })
    public void testPromiseToNativeCast_mismatching_type() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
		{
			QPromise promise = QPromise.createVoidPromise();
			QFuture future = promise.future();
			promise.start();
			QtConcurrent.run(pool, ()->{
				try {
					QThread.msleep(500);
					promise.addResult("Z");
				}catch(Throwable t) {
					promise.setException(t);
				}finally {
					promise.finish();
				}
			});
			try{
				future.waitForFinished();
				assertFalse("Exception expected to be thrown.", true);
			}catch(QNoImplementationException e) {
				Assert.assertEquals("addResult(T) not available for QPromise<void>.", e.getMessage());
			}
		}
		
		{
			QPromise promise = QPromise.createVoidPromise();
			QFuture future = promise.future();
			promise.start();
			QtConcurrent.run(pool, ()->{
				try {
					QThread.msleep(500);
					FutureHandler.fillStringPromise(promise);
				}catch(Throwable t) {
					promise.setException(t);
				}finally {
					promise.finish();
				}
			});
			try{
				future.waitForFinished();
				assertFalse("Exception expected to be thrown.", true);
			}catch(IllegalArgumentException e) {
				Assert.assertEquals("Cannot cast QPromise<void> to QPromise<QString>.", e.getMessage());
			}
		}
		
		{
			QFutureInterface iface = FutureHandler.interfaceInt(); 
			QPromise promise = new QPromise(iface);
			QFuture future = promise.future();
			promise.start();
			QtConcurrent.run(pool, ()->{
				try {
					QThread.msleep(500);
					FutureHandler.fillStringPromise(promise);
				}catch(Throwable t) {
					promise.setException(t);
				}finally {
					promise.finish();
				}
			});
			try{
				future.waitForFinished();
				assertFalse("Exception expected to be thrown.", true);
			}catch(IllegalArgumentException e) {
				Assert.assertEquals("Cannot cast QPromise<int> to QPromise<QString>.", e.getMessage());
			}
		}
		
		{
			QFuture<Void> future = QtConcurrent.run(pool, (QPromise<Void> promise)->{
				try {
					QThread.msleep(500);
					((QPromise)promise).addResult("X");
				}catch(Throwable t) {
					promise.setException(t);
				}
			});
			try{
				future.waitForFinished();
				assertFalse("Exception expected to be thrown.", true);
			}catch(QNoImplementationException e) {
				Assert.assertEquals("addResult(T) not available for QPromise<void>.", e.getMessage());
			}
		}
		
		{
			QFuture<Void> future = QtConcurrent.run(pool, (QPromise<Void> promise)->{
				try {
					QThread.msleep(500);
					FutureHandler.fillObjectPromise((QPromise)promise);
				}catch(Throwable t) {
					promise.setException(t);
				}
			});
			try{
				future.waitForFinished();
				assertFalse("Exception expected to be thrown.", true);
			}catch(IllegalArgumentException e) {
				Assert.assertEquals("Cannot cast QPromise<void> to QPromise<QObject*>.", e.getMessage());
			}
		}
		
		{
			QFuture<Void> future = QtConcurrent.run(pool, (QPromise<Void> promise)->{
				try {
					QThread.msleep(500);
					FutureHandler.fillIntPromise((QPromise)promise);
				}catch(Throwable t) {
					promise.setException(t);
				}
			});
			try{
				future.waitForFinished();
				assertFalse("Exception expected to be thrown.", true);
			}catch(IllegalArgumentException e) {
				Assert.assertEquals("Cannot cast QPromise<void> to QPromise<int>.", e.getMessage());
			}
		}
		
		{
			QFuture<Void> future = QtConcurrent.run(pool, (QPromise<Void> promise)->{
				try {
					QThread.msleep(500);
					FutureHandler.fillVariantPromise((QPromise)promise);
				}catch(Throwable t) {
					promise.setException(t);
				}
			});
			try{
				future.waitForFinished();
				assertFalse("Exception expected to be thrown.", true);
			}catch(IllegalArgumentException e) {
				Assert.assertEquals("Cannot cast QPromise<void> to QPromise<QVariant>.", e.getMessage());
			}
		}
	}
	
	@Test
    public void testJavaQFutureWithPromise() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	List<String> result = FutureHandler.returnInTheFuture(QtConcurrent.run((QPromise<String> promise)->{
    		QThread.msleep(200);
    		promise.addResult("testJavaQFuture1");
    		promise.addResult("testJavaQFuture2");
    	}));
    	Assert.assertEquals(2, result.size());
    	Assert.assertEquals("testJavaQFuture1", result.get(0));
    	Assert.assertEquals("testJavaQFuture2", result.get(1));
    }
    
    @Test
    public void testSequentialJavaQFutureWithPromise() throws Throwable {
    	Assume.assumeTrue("Disabled unless you specify qtjambi.autotests.enable.criticals=true", Boolean.getBoolean("qtjambi.autotests.enable.criticals"));
    	Throwable[] throwable = {null};
    	QThread thread = QThread.create(()->FutureHandler.returnSequentialInTheFuture(QtConcurrent.run((QPromise<String> promise)->{
    		try {
				promise.setProgressValueAndText(1, "sending: testSequentialJavaQFuture1");
				QThread.msleep(200);
				promise.suspendIfRequested();
				promise.addResult("testSequentialJavaQFuture1");
				promise.setProgressValueAndText(2, "sending: testSequentialJavaQFuture2");
				QThread.msleep(20);
				promise.suspendIfRequested();
				promise.addResult("testSequentialJavaQFuture2");
				promise.setProgressValueAndText(3, "sending: testSequentialJavaQFuture3");
				QThread.msleep(20);
				promise.suspendIfRequested();
				promise.addResult("testSequentialJavaQFuture3");
				promise.setProgressValueAndText(4, "sending: testSequentialJavaQFuture4");
				QThread.msleep(20);
				promise.suspendIfRequested();
				promise.addResult("testSequentialJavaQFuture4");
				promise.setProgressValueAndText(5, "sending: testSequentialJavaQFuture5");
				QThread.msleep(20);
				promise.suspendIfRequested();
				promise.addResult("testSequentialJavaQFuture5");
			} catch (Throwable e) {
				throwable[0] = e;
			}
    	})));
    	thread.start();
    	thread.join(120000);
    	try {
	    	Assert.assertFalse("FutureHandler.returnSequentialInTheFuture does not return.", thread.isRunning());
	    	if(throwable[0]!=null)
	    		throw throwable[0];
    	}finally {
    		thread.interrupt();
    	}
    }
    
    @Test
    public void testExceptionJavaQFutureWithPromise() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	try {
	    	FutureHandler.throwInTheFuture(QtConcurrent.run((QPromise<String> promise)->{
	    		QThread.msleep(200);
	    		throw new RuntimeException();
	    	}));
	    	Assert.assertFalse("exception expected to be thrown", true);
		} catch (RuntimeException e) {
		}
    }
    
    @Test
    public void testSuspendingJavaQFuture() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	QFutureInterface<String> promise = new QFutureInterface<>();
		promise.reportStarted();
		boolean[] isSuspending = {false};
    	QtConcurrent.run(()->{
    		try {
    			int i=0;
        		while(!promise.isSuspending() && (i++<30))
        			QThread.msleep(200);
        		isSuspending[0] = promise.isSuspending();
    		}catch(Throwable e) {
    			promise.reportException(e);
    		}finally {
    			promise.reportFinished();
    		}
    	});
    	FutureHandler.suspendInTheFuture(promise.future());
    	Assert.assertTrue(isSuspending[0]);
    }
    
	@Test
    public void testResumeJavaQFuture() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	QFutureInterface<String> promise = new QFutureInterface<>();
		promise.reportStarted();
		promise.setSuspended(true);
		boolean[] isSuspended = {true};
    	QtConcurrent.run(()->{
    		try {
    			int i=0;
        		while(promise.isSuspended() && (i++<30))
        			QThread.msleep(200);
        		isSuspended[0] = promise.isSuspended();
    		}catch(Throwable e) {
    			promise.reportException(e);
    		}finally {
    			promise.reportFinished();
    		}
    	});
    	FutureHandler.resumeInTheFuture(promise.future());
    	Assert.assertFalse(isSuspended[0]);
    }
    
	@Test
    public void testSuspendingJavaQFuture_nested() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	QFutureInterface<String> promise = new QFutureInterface<>();
		promise.reportStarted();
		boolean[] isSuspending = {false};
    	QtConcurrent.run(()->{
    		try {
    			int i=0;
        		while(!promise.isSuspending() && (i++<30))
        			QThread.msleep(200);
        		isSuspending[0] = promise.isSuspending();
    		}catch(Throwable e) {
    			promise.reportException(e);
    		}finally {
    			promise.reportFinished();
    		}
    	});
    	FutureHandler.suspendInTheFuture(FutureHandler.forward(promise.future()));
    	Assert.assertTrue(isSuspending[0]);
    }
    
	@Test
    public void testResumeJavaQFuture_nested() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	QFutureInterface<String> promise = new QFutureInterface<>();
		promise.reportStarted();
		promise.setSuspended(true);
		boolean[] isSuspended = {true};
    	QtConcurrent.run(()->{
    		try {
    			int i=0;
        		while(promise.isSuspended() && (i++<30))
        			QThread.msleep(200);
        		isSuspended[0] = promise.isSuspended();
    		}catch(Throwable e) {
    			promise.reportException(e);
    		}finally {
    			promise.reportFinished();
    		}
    	});
    	FutureHandler.resumeInTheFuture(FutureHandler.forward(promise.future()));
    	Assert.assertFalse(isSuspended[0]);
    }
}
