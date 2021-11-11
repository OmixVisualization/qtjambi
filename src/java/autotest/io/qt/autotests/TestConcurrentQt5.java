package io.qt.autotests;

import org.junit.Assert;
import org.junit.Test;

import io.qt.autotests.generated.FutureHandler;
import io.qt.concurrent.QtConcurrent;
import io.qt.core.QFutureInterface;
import io.qt.core.QThread;

public class TestConcurrentQt5 extends QApplicationTest {
	@Test
    public void testSuspendedJavaQFuture() {
    	QFutureInterface<String> promise = new QFutureInterface<>();
		promise.reportStarted();
		boolean[] isPaused = {false};
    	QtConcurrent.run(()->{
    		try {
        		while(!promise.isPaused())
        			QThread.msleep(200);
        		isPaused[0] = promise.isPaused();
    		}catch(Throwable e) {
    			promise.reportException(e);
    		}finally {
    			promise.reportFinished();
    		}
    	});
    	FutureHandler.suspendInTheFuture(promise.future());
    	Assert.assertTrue(isPaused[0]);
    }
    
	@Test
    public void testResumeJavaQFuture() {
    	QFutureInterface<String> promise = new QFutureInterface<>();
		promise.reportStarted();
		promise.setPaused(true);
		boolean[] isPaused = {true};
    	QtConcurrent.run(()->{
    		try {
        		while(promise.isPaused())
        			QThread.msleep(200);
        		isPaused[0] = promise.isPaused();
    		}catch(Throwable e) {
    			promise.reportException(e);
    		}finally {
    			promise.reportFinished();
    		}
    	});
    	FutureHandler.resumeInTheFuture(promise.future());
    	Assert.assertFalse(isPaused[0]);
    }
    
	@Test
    public void testSuspendedJavaQFuture_nested() {
    	QFutureInterface<String> promise = new QFutureInterface<>();
		promise.reportStarted();
		boolean[] isPaused = {false};
    	QtConcurrent.run(()->{
    		try {
        		while(!promise.isPaused())
        			QThread.msleep(200);
        		isPaused[0] = promise.isPaused();
    		}catch(Throwable e) {
    			promise.reportException(e);
    		}finally {
    			promise.reportFinished();
    		}
    	});
    	FutureHandler.suspendInTheFuture(FutureHandler.forward(promise.future()));
    	Assert.assertTrue(isPaused[0]);
    }
    
	@Test
    public void testResumeJavaQFuture_nested() {
    	QFutureInterface<String> promise = new QFutureInterface<>();
		promise.reportStarted();
		promise.setPaused(true);
		boolean[] isPaused = {true};
    	QtConcurrent.run(()->{
    		try {
        		while(promise.isPaused())
        			QThread.msleep(200);
        		isPaused[0] = promise.isPaused();
    		}catch(Throwable e) {
    			promise.reportException(e);
    		}finally {
    			promise.reportFinished();
    		}
    	});
    	FutureHandler.resumeInTheFuture(FutureHandler.forward(promise.future()));
    	Assert.assertFalse(isPaused[0]);
    }
}
