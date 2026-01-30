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

import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.util.concurrent.atomic.AtomicReference;

import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QFuture;
import io.qt.core.QFutureInterface;
import io.qt.core.QLibraryInfo;
import io.qt.core.QObject;
import io.qt.core.QSemaphore;
import io.qt.core.QThread;
import io.qt.core.QThreadPool;
import io.qt.core.QVersionNumber;
import io.qt.core.QtFuture;

public class TestQFutureQt61 extends ApplicationInitializer {
	private static QObject context;
	private static QThread thread;
	private static QThreadPool pool;
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitialize();
		pool = new QThreadPool();
    	context = new QObject();
    	thread = new QThread();
    	thread.setObjectName("ContextThread");
    	context.moveToThread(thread);
    	thread.start();
	}
	
	@AfterClass
    public static void testDispose() throws Exception {
		pool.waitForDone(-1);
		pool.disposeLater();
		pool = null;
		context.disposeLater();
		context = null;
		thread.quit();
		thread = null;
		ApplicationInitializer.testDispose();
	}
	
	static QFuture<Void> compute(QThreadPool pool, QSemaphore semaphore, QFuture.Runnable supplier){
		QFutureInterface<Void> promise = QFutureInterface.createVoidFutureInterface();
		promise.setThreadPool(pool);
		promise.reportStarted();
		pool.start(()->{
			if(semaphore!=null) {
				semaphore.acquire();
			}else {
				QThread.sleep(1);
			}
			try {
				if(!promise.isCanceled()) {
					try {
						supplier.run();
					}catch(Throwable t) {
						promise.reportException(t);
					}
				}
			}finally {
				promise.reportFinished();
			}
		});
		return promise.future();
	}
	
	static <T> QFuture<T> compute(QThreadPool pool, QSemaphore semaphore, QFuture.Supplier<T> supplier){
		QFutureInterface<T> promise = new QFutureInterface<>();
		promise.setThreadPool(pool);
		promise.reportStarted();
		pool.start(()->{
			if(semaphore!=null) {
				semaphore.acquire();
			}else {
				QThread.sleep(1);
			}
			try {
				if(!promise.isCanceled()) {
					try {
						T result = supplier.get();
						promise.reportResult(result);
					}catch(Throwable t) {
						promise.reportException(t);
					}
				}
			}finally {
				promise.reportFinished();
			}
		});
		return promise.future();
	}
	
	static QFuture<String> continuation(QFuture<String> future){
		return future.onCanceled(()->"Canceled")
			.onFailed(Error.class, e->"Error")
			.onFailed(IndexOutOfBoundsException.class, e->"IndexOutOfBoundsException")
			.onFailed(NullPointerException.class, ()->"NullPointerException")
			.onFailed(RuntimeException.class, e->"RuntimeException")
			.onFailed(IOException.class, ()->"IOException")
			.onFailed(Exception.class, e->"Exception")
			.onFailed(Throwable.class, e->"Throwable");
	}
	
	static QFuture<String> continuation(QObject context, QFuture<String> future){
		return future.onCanceled(context, ()->"Canceled-"+QThread.currentThread().objectName())
			.onFailed(context, Error.class, e->"Error-"+QThread.currentThread().objectName())
			.onFailed(context, IndexOutOfBoundsException.class, e->"IndexOutOfBoundsException-"+QThread.currentThread().objectName())
			.onFailed(context, NullPointerException.class, ()->"NullPointerException-"+QThread.currentThread().objectName())
			.onFailed(context, RuntimeException.class, e->"RuntimeException-"+QThread.currentThread().objectName())
			.onFailed(context, IOException.class, ()->"IOException-"+QThread.currentThread().objectName())
			.onFailed(context, Exception.class, e->"Exception-"+QThread.currentThread().objectName())
			.onFailed(context, Throwable.class, e->"Throwable-"+QThread.currentThread().objectName());
	}
	
	static QFuture<Void> continuation(QFuture<?> future, AtomicReference<String> result){
		return future.onCanceled(()->result.set("Canceled"))
			.onFailed(Error.class, e->{result.set("Error");})
			.onFailed(IndexOutOfBoundsException.class, e->{result.set("IndexOutOfBoundsException");})
			.onFailed(NullPointerException.class, ()->{result.set("NullPointerException");})
			.onFailed(RuntimeException.class, e->{result.set("RuntimeException");})
			.onFailed(IOException.class, ()->{result.set("IOException");})
			.onFailed(Exception.class, e->{result.set("Exception");})
			.onFailed(Throwable.class, e->{result.set("Throwable");});
	}
	
	static QFuture<Void> continuation(QObject context, QFuture<?> future, AtomicReference<String> result){
		return future.onCanceled(context, ()->result.set("Canceled-"+QThread.currentThread().objectName()))
			.onFailed(context, Error.class, e->{result.set("Error-"+QThread.currentThread().objectName());})
			.onFailed(context, IndexOutOfBoundsException.class, e->{result.set("IndexOutOfBoundsException-"+QThread.currentThread().objectName());})
			.onFailed(context, NullPointerException.class, ()->{result.set("NullPointerException-"+QThread.currentThread().objectName());})
			.onFailed(context, RuntimeException.class, e->{result.set("RuntimeException-"+QThread.currentThread().objectName());})
			.onFailed(context, IOException.class, ()->{result.set("IOException-"+QThread.currentThread().objectName());})
			.onFailed(context, Exception.class, e->{result.set("Exception-"+QThread.currentThread().objectName());})
			.onFailed(context, Throwable.class, e->{result.set("Throwable-"+QThread.currentThread().objectName());});
	}
	
	static QFuture<Void> onFailed1(QFuture<?> future, AtomicReference<String> result){
		return future.onFailed(e->{result.set("Direct-"+e.getClass().getSimpleName());})
			.onFailed(()->{result.set("PlainFailed");});
	}
	
	static QFuture<Void> onFailed1(QObject context, QFuture<?> future, AtomicReference<String> result){
		return future.onFailed(context, e->{result.set("Direct-"+e.getClass().getSimpleName()+"-"+QThread.currentThread().objectName());})
			.onFailed(context, ()->{result.set("PlainFailed-"+QThread.currentThread().objectName());});
	}
	
	static QFuture<Void> onFailed2(QFuture<?> future, AtomicReference<String> result){
		return future.onFailed(()->{result.set("PlainFailed");})
				.onFailed(e->{result.set("Direct-"+e.getClass().getSimpleName());});
	}
	
	static QFuture<Void> onFailed2(QObject context, QFuture<?> future, AtomicReference<String> result){
		return future.onFailed(context, ()->{result.set("PlainFailed-"+QThread.currentThread().objectName());})
				.onFailed(context, e->{result.set("Direct-"+e.getClass().getSimpleName()+"-"+QThread.currentThread().objectName());});
	}
	
	static QFuture<Void> failingOnFailed(QFuture<?> future, AtomicReference<String> result){
		return future.onFailed((QFuture.Consumer<Throwable>)e->{
					throw new InvocationTargetException(e);
				}).onFailed(e->{result.set("Cascaded-"+e.getClass().getSimpleName()+"-"+e.getCause().getClass().getSimpleName());});
	}
	
	static QFuture<Void> failingOnFailed(QObject context, QFuture<?> future, AtomicReference<String> result){
		return future.onFailed(context, (QFuture.Consumer<Throwable>)e->{
					throw new InvocationTargetException(e);
				}).onFailed(context, e->{result.set("Cascaded-"+e.getClass().getSimpleName()+"-"+e.getCause().getClass().getSimpleName()+"-"+QThread.currentThread().objectName());});
	}
	
	static QFuture<Void> failingOnCanceled(QFuture<?> future, AtomicReference<String> result){
		return future.onCanceled(()->{
					throw new InterruptedException();
				}).onFailed(e->{result.set("Cascaded-"+e.getClass().getSimpleName());});
	}
	
	static QFuture<Void> failingOnCanceled(QObject context, QFuture<?> future, AtomicReference<String> result){
		return future.onCanceled(()->{
					throw new InterruptedException();
				}).onFailed(context, e->{result.set("Cascaded-"+e.getClass().getSimpleName()+"-"+QThread.currentThread().objectName());});
	}
	
    @Test
    public void testSimple() {
    	QFuture<String> future1 = compute(QThreadPool.globalInstance(), null, ()->"Test");
    	future1.waitForFinished();
    	Assert.assertEquals("Test", future1.result());
    }
    
    @Test
    public void testThenSync() {
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<String> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->"12345");
    	QFuture<Integer> future2 = future1.then((QFuture<String> future)->{return Integer.parseInt(future.result());});
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals((Integer)12345, future2.result());
    	
    	future1 = compute(QThreadPool.globalInstance(), semaphore, ()->"123456");
    	future2 = future1.then((String strg)->{return Integer.parseInt(strg);});
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals((Integer)123456, future2.result());
    	
    	future1 = compute(QThreadPool.globalInstance(), semaphore, ()->"123456");
    	future2 = future1.then(()->{return 1234;});
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals((Integer)1234, future2.result());
    	
    	future1 = compute(QThreadPool.globalInstance(), semaphore, ()->"123456");
    	int[] result = {0};
    	QFuture<Void> future3 = future1.then(()->{result[0] = 12341234;});
    	semaphore.release();
    	future3.waitForFinished();
    	Assert.assertEquals(12341234, result[0]);
    	
    	future1 = compute(QThreadPool.globalInstance(), semaphore, ()->"54637");
    	future3 = future1.then((QFuture<String> future)->{result[0] = Integer.parseInt(future.result());});
    	semaphore.release();
    	future3.waitForFinished();
    	Assert.assertEquals(54637, result[0]);
    }
    
    @Test
    public void testThenContext() {
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Integer> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->12345);
    	QFuture<String> future2 = future1.then(context, (QFuture<Integer> future)->{return QThread.currentThread().objectName() + "-" + future.result();});
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("ContextThread-12345", future2.result());
    	
    	future1 = compute(QThreadPool.globalInstance(), semaphore, ()->123456);
    	future2 = future1.then(context, (Integer i)->{return QThread.currentThread().objectName() + "-" + i;});
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("ContextThread-123456", future2.result());
    	
    	future1 = compute(QThreadPool.globalInstance(), semaphore, ()->123456);
    	future2 = future1.then(context, ()->{return QThread.currentThread().objectName() + "-1234";});
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("ContextThread-1234", future2.result());
    	
    	future1 = compute(QThreadPool.globalInstance(), semaphore, ()->123456);
    	String[] result = {null};
    	QFuture<Void> future3 = future1.then(context, ()->{result[0] = QThread.currentThread().objectName() + "-12341234";});
    	semaphore.release();
    	future3.waitForFinished();
    	Assert.assertEquals("ContextThread-12341234", result[0]);
    	
    	future1 = compute(QThreadPool.globalInstance(), semaphore, ()->54637);
    	future3 = future1.then(context, (QFuture<Integer> future)->{result[0] = QThread.currentThread().objectName() + "-" + future.result();});
    	semaphore.release();
    	future3.waitForFinished();
    	Assert.assertEquals("ContextThread-54637", result[0]);
    }
    
    @Test
    public void testThenPool() {
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Integer> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->12345);
    	QFuture<String> future2 = future1.then(pool, (QFuture<Integer> future)->{return (pool.contains(QThread.currentThread()) ? "PooledThread-" : "") + future.result();});
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("PooledThread-12345", future2.result());
    	
    	future1 = compute(QThreadPool.globalInstance(), semaphore, ()->123456);
    	future2 = future1.then(pool, (Integer i)->{return (pool.contains(QThread.currentThread()) ? "PooledThread-" : "") + i;});
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("PooledThread-123456", future2.result());
    	
    	future1 = compute(QThreadPool.globalInstance(), semaphore, ()->123456);
    	future2 = future1.then(pool, ()->{return (pool.contains(QThread.currentThread()) ? "PooledThread-" : "") + "1234";});
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("PooledThread-1234", future2.result());
    	
    	future1 = compute(QThreadPool.globalInstance(), semaphore, ()->123456);
    	String[] result = {null};
    	QFuture<Void> future3 = future1.then(pool, ()->{result[0] = (pool.contains(QThread.currentThread()) ? "PooledThread-" : "") + "12341234";});
    	semaphore.release();
    	future3.waitForFinished();
    	Assert.assertEquals("PooledThread-12341234", result[0]);
    	
    	future1 = compute(QThreadPool.globalInstance(), semaphore, ()->54637);
    	future3 = future1.then(pool, (QFuture<Integer> future)->{result[0] = (pool.contains(QThread.currentThread()) ? "PooledThread-" : "") + future.result();});
    	semaphore.release();
    	future3.waitForFinished();
    	Assert.assertEquals("PooledThread-54637", result[0]);
    }
    
    @Test
    public void testThenLaunch() {
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Integer> future1 = compute(pool, semaphore, ()->12345);
    	QFuture<String> future2 = future1.then(QtFuture.Launch.Async, (QFuture<Integer> future)->{return (QThreadPool.globalInstance().contains(QThread.currentThread()) ? "PooledThread-" : "") + future.result();});
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("PooledThread-12345", future2.result());
    	
    	future1 = compute(pool, semaphore, ()->123456);
    	future2 = future1.then(QtFuture.Launch.Async, (Integer i)->{return (QThreadPool.globalInstance().contains(QThread.currentThread()) ? "PooledThread-" : "") + i;});
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("PooledThread-123456", future2.result());
    	
    	future1 = compute(QThreadPool.globalInstance(), semaphore, ()->123456);
    	future2 = future1.then(QtFuture.Launch.Async, ()->{return (QThreadPool.globalInstance().contains(QThread.currentThread()) ? "PooledThread-" : "") + "1234";});
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("PooledThread-1234", future2.result());
    	
    	future1 = compute(QThreadPool.globalInstance(), semaphore, ()->123456);
    	String[] result = {null};
    	QFuture<Void> future3 = future1.then(QtFuture.Launch.Async, ()->{result[0] = (QThreadPool.globalInstance().contains(QThread.currentThread()) ? "PooledThread-" : "") + "12341234";});
    	semaphore.release();
    	future3.waitForFinished();
    	Assert.assertEquals("PooledThread-12341234", result[0]);
    	
    	future1 = compute(QThreadPool.globalInstance(), semaphore, ()->54637);
    	future3 = future1.then(QtFuture.Launch.Async, (QFuture<Integer> future)->{result[0] = (QThreadPool.globalInstance().contains(QThread.currentThread()) ? "PooledThread-" : "") + future.result();});
    	semaphore.release();
    	future3.waitForFinished();
    	Assert.assertEquals("PooledThread-54637", result[0]);
    	
    	future1 = compute(pool, semaphore, ()->12345);
    	future2 = future1.then(QtFuture.Launch.Inherit, (QFuture<Integer> future)->{return (pool.contains(QThread.currentThread()) ? "PooledThread-" : "") + future.result();});
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("PooledThread-12345", future2.result());
    	
    	future1 = compute(pool, semaphore, ()->123456);
    	future2 = future1.then(QtFuture.Launch.Inherit, (Integer i)->{return (pool.contains(QThread.currentThread()) ? "PooledThread-" : "") + i;});
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("PooledThread-123456", future2.result());
    	
    	future1 = compute(pool, semaphore, ()->123456);
    	future2 = future1.then(QtFuture.Launch.Inherit, ()->{return (pool.contains(QThread.currentThread()) ? "PooledThread-" : "") + "1234";});
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("PooledThread-1234", future2.result());
    	
    	future1 = compute(pool, semaphore, ()->123456);
    	future3 = future1.then(QtFuture.Launch.Inherit, ()->{result[0] = (pool.contains(QThread.currentThread()) ? "PooledThread-" : "") + "12341234";});
    	semaphore.release();
    	future3.waitForFinished();
    	Assert.assertEquals("PooledThread-12341234", result[0]);
    	
    	future1 = compute(pool, semaphore, ()->54637);
    	future3 = future1.then(QtFuture.Launch.Inherit, (QFuture<Integer> future)->{result[0] = (pool.contains(QThread.currentThread()) ? "PooledThread-" : "") + future.result();});
    	semaphore.release();
    	future3.waitForFinished();
    	Assert.assertEquals("PooledThread-54637", result[0]);
    }
    
    @Test
    public void testOnCanceledSync1() {
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<String> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->"Test");
    	QFuture<String> future2 = continuation(future1);
    	future1.cancel();
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Canceled", future2.result());
    }
    
    @Test
    public void testOnFailedSync1Error() {
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<String> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new Error();});
    	QFuture<String> future2 = continuation(future1);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Error", future2.result());
    }
    
    @Test
    public void testOnFailedSync1IOException() {
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<String> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new IOException();});
    	QFuture<String> future2 = continuation(future1);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("IOException", future2.result());
    }
    
    @Test
    public void testOnFailedSync1IndexOutOfBoundsException() {
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<String> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new IndexOutOfBoundsException();});
    	QFuture<String> future2 = continuation(future1);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("IndexOutOfBoundsException", future2.result());
    }
    
    @Test
    public void testOnFailedSync1NullPointerException() {
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<String> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new NullPointerException();});
    	QFuture<String> future2 = continuation(future1);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("NullPointerException", future2.result());
    }
    
    @Test
    public void testOnCanceledContext1() {
    	Assume.assumeTrue(QLibraryInfo.version().compareTo(new QVersionNumber(6,0,0))>0);
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<String> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->"Test");
    	QFuture<String> future2 = continuation(context, future1);
    	future1.cancel();
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Canceled-ContextThread", future2.result());
    }
    
    @Test
    public void testOnFailedContext1Error() {
    	Assume.assumeTrue(QLibraryInfo.version().compareTo(new QVersionNumber(6,0,0))>0);
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<String> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new Error();});
    	QFuture<String> future2 = continuation(context, future1);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Error-ContextThread", future2.result());
    }
    
    @Test
    public void testOnFailedContext1IOException() {
    	Assume.assumeTrue(QLibraryInfo.version().compareTo(new QVersionNumber(6,0,0))>0);
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<String> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new IOException();});
    	QFuture<String> future2 = continuation(context, future1);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("IOException-ContextThread", future2.result());
    }
    
    @Test
    public void testOnFailedContext1IndexOutOfBoundsException() {
    	Assume.assumeTrue(QLibraryInfo.version().compareTo(new QVersionNumber(6,0,0))>0);
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<String> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new IndexOutOfBoundsException();});
    	QFuture<String> future2 = continuation(context, future1);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("IndexOutOfBoundsException-ContextThread", future2.result());
    }
    
    @Test
    public void testOnFailedContext1NullPointerException() {
    	Assume.assumeTrue(QLibraryInfo.version().compareTo(new QVersionNumber(6,0,0))>0);
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<String> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new NullPointerException();});
    	QFuture<String> future2 = continuation(context, future1);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("NullPointerException-ContextThread", future2.result());
    }
    
    @Test
    public void testOnCanceledSync2() {
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->'Q');
    	QFuture<Void> future2 = continuation(future1, result);
    	future1.cancel();
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Canceled", result.get());
    }
    
    @Test
    public void testOnCanceledContext2() {
    	Assume.assumeTrue(QLibraryInfo.version().compareTo(new QVersionNumber(6,0,0))>0);
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->'Q');
    	QFuture<Void> future2 = continuation(context, future1, result);
    	future1.cancel();
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Canceled-ContextThread", result.get());
    }
    
    @Test
    public void testOnFailedSync2Error() {
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new Error();});
    	QFuture<Void> future2 = continuation(future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Error", result.get());
    }
    
    @Test
    public void testOnFailedSync2IOException() {
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new IOException();});
    	QFuture<Void> future2 = continuation(future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("IOException", result.get());
    }
    
    @Test
    public void testOnFailedSync2IndexOutOfBoundsException() {
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new IndexOutOfBoundsException();});
    	QFuture<Void> future2 = continuation(future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("IndexOutOfBoundsException", result.get());
    }
    
    @Test
    public void testOnFailedSync2NullPointerException() {
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new NullPointerException();});
    	QFuture<Void> future2 = continuation(future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("NullPointerException", result.get());
    }
    
    @Test
    public void testOnFailedContext2Error() {
    	Assume.assumeTrue(QLibraryInfo.version().compareTo(new QVersionNumber(6,0,0))>0);
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new Error();});
    	QFuture<Void> future2 = continuation(context, future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Error-ContextThread", result.get());
    }
    
    @Test
    public void testOnFailedContext2IOException() {
    	Assume.assumeTrue(QLibraryInfo.version().compareTo(new QVersionNumber(6,0,0))>0);
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new IOException();});
    	QFuture<Void> future2 = continuation(context, future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("IOException-ContextThread", result.get());
    }
    
    @Test
    public void testOnFailedContext2IndexOutOfBoundsException() {
    	Assume.assumeTrue(QLibraryInfo.version().compareTo(new QVersionNumber(6,0,0))>0);
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new IndexOutOfBoundsException();});
    	QFuture<Void> future2 = continuation(context, future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("IndexOutOfBoundsException-ContextThread", result.get());
    }
    
    @Test
    public void testOnFailedContext2NullPointerException() {
    	Assume.assumeTrue(QLibraryInfo.version().compareTo(new QVersionNumber(6,0,0))>0);
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new NullPointerException();});
    	QFuture<Void> future2 = continuation(context, future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("NullPointerException-ContextThread", result.get());
    }
    
    @Test
    public void testOnCanceledSync3() {
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Void> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{});
    	QFuture<Void> future2 = continuation(future1, result);
    	future1.cancel();
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Canceled", result.get());
    }
    
    @Test
    public void testOnFailedSync3Error() {
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Void> future1 = compute(QThreadPool.globalInstance(), semaphore, (QFuture.Runnable)()->{throw new Error();});
    	QFuture<Void> future2 = continuation(future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Error", result.get());
    }
    
    @Test
    public void testOnFailedSync3IOException() {
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Void> future1 = compute(QThreadPool.globalInstance(), semaphore, (QFuture.Runnable)()->{throw new IOException();});
    	QFuture<Void> future2 = continuation(future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("IOException", result.get());
    }
    
    @Test
    public void testOnFailedSync3IndexOutOfBoundsException() {
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Void> future1 = compute(QThreadPool.globalInstance(), semaphore, (QFuture.Runnable)()->{throw new IndexOutOfBoundsException();});
    	QFuture<Void> future2 = continuation(future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("IndexOutOfBoundsException", result.get());
    }
    
    @Test
    public void testOnFailedSync3NullPointerException() {
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Void> future1 = compute(QThreadPool.globalInstance(), semaphore, (QFuture.Runnable)()->{throw new NullPointerException();});
    	QFuture<Void> future2 = continuation(future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("NullPointerException", result.get());
    }
    
    @Test
    public void testOnCanceledContext3() {
    	Assume.assumeTrue(QLibraryInfo.version().compareTo(new QVersionNumber(6,0,0))>0);
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Void> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{});
    	QFuture<Void> future2 = continuation(context, future1, result);
    	future1.cancel();
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Canceled-ContextThread", result.get());
    }
    
    @Test
    public void testOnFailedContext3Error() {
    	Assume.assumeTrue(QLibraryInfo.version().compareTo(new QVersionNumber(6,0,0))>0);
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Void> future1 = compute(QThreadPool.globalInstance(), semaphore, (QFuture.Runnable)()->{throw new Error();});
    	QFuture<Void> future2 = continuation(context, future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Error-ContextThread", result.get());
    }
    
    @Test
    public void testOnFailedContext3IOException() {
    	Assume.assumeTrue(QLibraryInfo.version().compareTo(new QVersionNumber(6,0,0))>0);
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Void> future1 = compute(QThreadPool.globalInstance(), semaphore, (QFuture.Runnable)()->{throw new IOException();});
    	QFuture<Void> future2 = continuation(context, future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("IOException-ContextThread", result.get());
    }
    
    @Test
    public void testOnFailedContext3IndexOutOfBoundsException() {
    	Assume.assumeTrue(QLibraryInfo.version().compareTo(new QVersionNumber(6,0,0))>0);
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Void> future1 = compute(QThreadPool.globalInstance(), semaphore, (QFuture.Runnable)()->{throw new IndexOutOfBoundsException();});
    	QFuture<Void> future2 = continuation(context, future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("IndexOutOfBoundsException-ContextThread", result.get());
    }
    
    @Test
    public void testOnFailedContext3NullPointerException() {
    	Assume.assumeTrue(QLibraryInfo.version().compareTo(new QVersionNumber(6,0,0))>0);
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Void> future1 = compute(QThreadPool.globalInstance(), semaphore, (QFuture.Runnable)()->{throw new NullPointerException();});
    	QFuture<Void> future2 = continuation(context, future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("NullPointerException-ContextThread", result.get());
    }
    
    @Test
    public void testOnFailedSync4Error() {
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new Error();});
    	QFuture<Void> future2 = onFailed1(future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Direct-Error", result.get());
    }
    
    @Test
    public void testOnFailedSync4IOException() {
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new IOException();});
    	QFuture<Void> future2 = onFailed1(future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Direct-IOException", result.get());
    }
    
    @Test
    public void testOnFailedSync4IndexOutOfBoundsException() {
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new IndexOutOfBoundsException();});
    	QFuture<Void> future2 = onFailed1(future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Direct-IndexOutOfBoundsException", result.get());
    }
    
    @Test
    public void testOnFailedSync4NullPointerException() {
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new NullPointerException();});
    	QFuture<Void> future2 = onFailed1(future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Direct-NullPointerException", result.get());
    }
    
    @Test
    public void testOnFailedContext4Error() {
    	Assume.assumeTrue(QLibraryInfo.version().compareTo(new QVersionNumber(6,0,0))>0);
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new Error();});
    	QFuture<Void> future2 = onFailed1(context, future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Direct-Error-ContextThread", result.get());
    }
    
    @Test
    public void testOnFailedContext4IOException() {
    	Assume.assumeTrue(QLibraryInfo.version().compareTo(new QVersionNumber(6,0,0))>0);
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new IOException();});
    	QFuture<Void> future2 = onFailed1(context, future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Direct-IOException-ContextThread", result.get());
    }
    
    @Test
    public void testOnFailedContext4IndexOutOfBoundsException() {
    	Assume.assumeTrue(QLibraryInfo.version().compareTo(new QVersionNumber(6,0,0))>0);
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new IndexOutOfBoundsException();});
    	QFuture<Void> future2 = onFailed1(context, future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Direct-IndexOutOfBoundsException-ContextThread", result.get());
    }
    
    @Test
    public void testOnFailedContext4NullPointerException() {
    	Assume.assumeTrue(QLibraryInfo.version().compareTo(new QVersionNumber(6,0,0))>0);
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new NullPointerException();});
    	QFuture<Void> future2 = onFailed1(context, future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Direct-NullPointerException-ContextThread", result.get());
    }
    
    @Test
    public void testOnFailedSync5Error() {
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new Error();});
    	QFuture<Void> future2 = onFailed2(future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("PlainFailed", result.get());
    }
    
    @Test
    public void testOnFailedSync5IOException() {
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new IOException();});
    	QFuture<Void> future2 = onFailed2(future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("PlainFailed", result.get());
    }
    
    @Test
    public void testOnFailedSync5IndexOutOfBoundsException() {
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new IndexOutOfBoundsException();});
    	QFuture<Void> future2 = onFailed2(future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("PlainFailed", result.get());
    }
    
    @Test
    public void testOnFailedSync5NullPointerException() {
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new NullPointerException();});
    	QFuture<Void> future2 = onFailed2(future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("PlainFailed", result.get());
    }
    
    @Test
    public void testOnFailedContext5Error() {
    	Assume.assumeTrue(QLibraryInfo.version().compareTo(new QVersionNumber(6,0,0))>0);
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new Error();});
    	QFuture<Void> future2 = onFailed2(context, future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("PlainFailed-ContextThread", result.get());
    }
    
    @Test
    public void testOnFailedContext5IOException() {
    	Assume.assumeTrue(QLibraryInfo.version().compareTo(new QVersionNumber(6,0,0))>0);
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new IOException();});
    	QFuture<Void> future2 = onFailed2(context, future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("PlainFailed-ContextThread", result.get());
    }
    
    @Test
    public void testOnFailedContext5IndexOutOfBoundsException() {
    	Assume.assumeTrue(QLibraryInfo.version().compareTo(new QVersionNumber(6,0,0))>0);
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new IndexOutOfBoundsException();});
    	QFuture<Void> future2 = onFailed2(context, future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("PlainFailed-ContextThread", result.get());
    }
    
    @Test
    public void testOnFailedContext5NullPointerException() {
    	Assume.assumeTrue(QLibraryInfo.version().compareTo(new QVersionNumber(6,0,0))>0);
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new NullPointerException();});
    	QFuture<Void> future2 = onFailed2(context, future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("PlainFailed-ContextThread", result.get());
    }
    
    @Test
    public void testOnFailedSync6Error() {
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new Error();});
    	QFuture<Void> future2 = failingOnFailed(future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Cascaded-InvocationTargetException-Error", result.get());
    }
    
    @Test
    public void testOnFailedSync6IOException() {
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new IOException();});
    	QFuture<Void> future2 = failingOnFailed(future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Cascaded-InvocationTargetException-IOException", result.get());
    }
    
    @Test
    public void testOnFailedSync6IndexOutOfBoundsException() {
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new IndexOutOfBoundsException();});
    	QFuture<Void> future2 = failingOnFailed(future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Cascaded-InvocationTargetException-IndexOutOfBoundsException", result.get());
    }
    
    @Test
    public void testOnFailedSync6NullPointerException() {
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new NullPointerException();});
    	QFuture<Void> future2 = failingOnFailed(future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Cascaded-InvocationTargetException-NullPointerException", result.get());
    }
    
    @Test
    public void testOnFailedContext6Error() {
    	Assume.assumeTrue(QLibraryInfo.version().compareTo(new QVersionNumber(6,0,0))>0);
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new Error();});
    	QFuture<Void> future2 = failingOnFailed(context, future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Cascaded-InvocationTargetException-Error-ContextThread", result.get());
    }
    
    @Test
    public void testOnFailedContext6IOException() {
    	Assume.assumeTrue(QLibraryInfo.version().compareTo(new QVersionNumber(6,0,0))>0);
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new IOException();});
    	QFuture<Void> future2 = failingOnFailed(context, future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Cascaded-InvocationTargetException-IOException-ContextThread", result.get());
    }
    
    @Test
    public void testOnFailedContext6IndexOutOfBoundsException() {
    	Assume.assumeTrue(QLibraryInfo.version().compareTo(new QVersionNumber(6,0,0))>0);
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new IndexOutOfBoundsException();});
    	QFuture<Void> future2 = failingOnFailed(context, future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Cascaded-InvocationTargetException-IndexOutOfBoundsException-ContextThread", result.get());
    }
    
    @Test
    public void testOnFailedContext6NullPointerException() {
    	Assume.assumeTrue(QLibraryInfo.version().compareTo(new QVersionNumber(6,0,0))>0);
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Character> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{throw new NullPointerException();});
    	QFuture<Void> future2 = failingOnFailed(context, future1, result);
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Cascaded-InvocationTargetException-NullPointerException-ContextThread", result.get());
    }
    
    @Test
    public void testOnCanceledSync6() {
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Void> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{});
    	QFuture<Void> future2 = failingOnCanceled(future1, result);
    	future1.cancel();
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Cascaded-InterruptedException", result.get());
    }
    
    @Test
    public void testOnCanceledContext6() {
    	AtomicReference<String> result = new AtomicReference<>();
    	QSemaphore semaphore = new QSemaphore();
    	QFuture<Void> future1 = compute(QThreadPool.globalInstance(), semaphore, ()->{});
    	QFuture<Void> future2 = failingOnCanceled(context, future1, result);
    	future1.cancel();
    	semaphore.release();
    	future2.waitForFinished();
    	Assert.assertEquals("Cascaded-InterruptedException-ContextThread", result.get());
    }
	
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQFutureQt61.class.getName());
    }
}
