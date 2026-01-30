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

import java.util.Set;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;

import io.qt.QNoNativeResourcesException;
import io.qt.autotests.generated.General;
import io.qt.autotests.generated.SignalReceiver;
import io.qt.autotests.generated.ThreadFactory;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QEventLoop;
import io.qt.core.QObject;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QSemaphore;
import io.qt.core.QThread;
import io.qt.core.QTimer;
import io.qt.core.Qt;
import io.qt.internal.TestUtility;

public class TestQThreadAdoption extends ApplicationInitializer{
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitialize();
    }
	
	private static class Stopper extends QObject{
		public final Signal0 stop = new Signal0();
	}
	
	private static void initializeThread(AtomicReference<QThread> qthreads, AtomicReference<Thread> jthreads, AtomicInteger ownership, AtomicInteger running, AtomicInteger gcQtThreadObjectsCounter, AtomicInteger gcJavaThreadObjectsCounter, SignalReceiver destroyedReceiver, SignalReceiver finished, boolean isAdopted) {
		QThread currentQThread = QThread.currentThread();
		Thread currentThread = Thread.currentThread();
		qthreads.set(currentQThread);
		jthreads.set(currentThread);
		if(General.internalAccess.isJavaOwnership(currentQThread))
			ownership.set(1);
		else if(General.internalAccess.isCppOwnership(currentQThread))
			ownership.set(2);
		else if(General.internalAccess.isSplitOwnership(currentQThread))
			ownership.set(3);
		currentQThread.destroyed.connect(destroyedReceiver, "receiveSignal()", Qt.ConnectionType.DirectConnection);
		currentQThread.finished.connect(finished, "receiveSignal()", Qt.ConnectionType.DirectConnection);
		currentQThread.javaThread();
		@SuppressWarnings("unused")
		Object cleanable = General.internalAccess.registerCleaner(currentQThread, gcQtThreadObjectsCounter::incrementAndGet);
		cleanable = General.internalAccess.registerCleaner(currentThread, gcJavaThreadObjectsCounter::incrementAndGet);
		cleanable = null;
		running.incrementAndGet();
		currentQThread = null;
		currentThread = null;
	}
	
	@org.junit.Test
	public void testNativeCreatedNativeStartedQLoopThread() throws InterruptedException {
		AtomicInteger running = new AtomicInteger(0);
		AtomicInteger gcQtThreadObjectsCounter = new AtomicInteger(0);
		AtomicInteger gcJavaThreadObjectsCounter = new AtomicInteger(0);
		SignalReceiver finishedReceiver = new SignalReceiver();
		SignalReceiver destroyedReceiver = new SignalReceiver();
		AtomicReference<QThread> qthreads = new AtomicReference<>();
		AtomicReference<Thread> jthreads = new AtomicReference<>();
		AtomicInteger ownership = new AtomicInteger();
		Stopper stopper = new Stopper();
		// QThread 0 is natively started looping Qt thread adopted by Java
		QThread thread;
		QSemaphore semaphore = new QSemaphore();
		{
			thread = ThreadFactory.startQLoopThread();
			Assert.assertTrue("QThread NativeCreatedNativeStartedQLoopThread has not split ownership", General.internalAccess.isSplitOwnership(thread));
			thread.setObjectName("thread-NativeCreatedNativeStartedQLoopThread");
			QObject obj = new QObject();
			obj.destroyed.connect(()->{
				initializeThread(qthreads, jthreads, ownership, running, gcQtThreadObjectsCounter, gcJavaThreadObjectsCounter, destroyedReceiver, finishedReceiver, false);
				QTimer.singleShot(1, semaphore::release);
			}, Qt.ConnectionType.DirectConnection);
			obj.moveToThread(thread);
			obj.disposeLater();
			stopper.stop.connect(thread::quit, Qt.ConnectionType.DirectConnection);
		}
		
		int i=0;
		semaphore.acquire();
		Assert.assertEquals(1, running.get());
		Set<Thread> runningThreads = Thread.getAllStackTraces().keySet();
		{
			Assert.assertTrue(jthreads.get()!=null);
			Assert.assertTrue(runningThreads.contains(jthreads.get()));
			Assert.assertEquals(Thread.State.RUNNABLE, jthreads.get().getState());
			Assert.assertEquals("Java thread NativeCreatedNativeStartedQLoopThread is not associated to Qt thread", qthreads.get(), QThread.thread(jthreads.get()));
			Assert.assertEquals("Qt thread NativeCreatedNativeStartedQLoopThread is not associated to Java thread", jthreads.get(), qthreads.get().javaThread());
		}
		Assert.assertEquals(thread, qthreads.get());
		thread = null;
		{
			Assert.assertTrue("QThread NativeCreatedNativeStartedQLoopThread is null", qthreads.get()!=null);
			Assert.assertEquals("QThread NativeCreatedNativeStartedQLoopThread has wrong thread-affinity", QThread.currentThread(), qthreads.get().thread());
			Assert.assertFalse("QThread NativeCreatedNativeStartedQLoopThread is disposed", qthreads.get().isDisposed());
			Assert.assertTrue("QThread NativeCreatedNativeStartedQLoopThread not alive", qthreads.get().isAlive());
			Assert.assertTrue("QThread NativeCreatedNativeStartedQLoopThread not running", qthreads.get().isRunning());
			Assert.assertFalse("QThread NativeCreatedNativeStartedQLoopThread is finished", qthreads.get().isFinished());
			Assert.assertEquals(jthreads.get(), qthreads.get().javaThread());
			Assert.assertFalse("QThread NativeCreatedNativeStartedQLoopThread has Cpp ownership", General.internalAccess.isCppOwnership(qthreads.get()));
			Assert.assertTrue("running QThread NativeCreatedNativeStartedQLoopThread has not split ownership", ownership.get()==3);
		}
		stopper.stop.emit();
		stopper.stop.disconnect();
		finishedReceiver.waitForReleased(2000);
		
		i = 0;
		while(i<20) {
			Thread.yield();
			Thread.sleep(150);
			runGC();
			++i;
		}
		{
			try {
				if(qthreads.get().isRunning()) {
					i = 0;
					while(i<20 && qthreads.get().isRunning()) {
						Thread.yield();
						Thread.sleep(150);
						runGC();
						++i;
					}
				}
			}catch(Throwable t){}
		}
		runningThreads = Thread.getAllStackTraces().keySet();
		{
			try {
				if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)
						|| "windows-mingw-x64".equals(TestUtility.osArchName()))
					Assert.assertTrue("post-mortem Java thread NativeCreatedNativeStartedQLoopThread state==RUNNABLE", Thread.State.RUNNABLE!=jthreads.get().getState());
				else
					Assert.assertEquals("post-mortem Java thread NativeCreatedNativeStartedQLoopThread state", Thread.State.TERMINATED, jthreads.get().getState());
			} catch (AssertionError e) {
				StackTraceElement[] stackTraces = Thread.getAllStackTraces().get(jthreads.get());
				if(stackTraces!=null) {
					AssertionError error = new AssertionError("Thread stack");
					error.setStackTrace(stackTraces);
					e.addSuppressed(error);
					throw e;
				}else {
					runningThreads = Thread.getAllStackTraces().keySet();
				}
			}
			Assert.assertFalse("post-mortem Java thread NativeCreatedNativeStartedQLoopThread is still running", runningThreads.contains(jthreads.get()));
			if(!qthreads.get().isDisposed()) {
				Assert.assertEquals("post-mortem Java thread NativeCreatedNativeStartedQLoopThread is not associated to Qt thread", qthreads.get(), QThread.thread(jthreads.get()));
				Assert.assertEquals("post-mortem Qt thread NativeCreatedNativeStartedQLoopThread is not associated to Java thread", jthreads.get(), qthreads.get().javaThread());
			}else {
				Assert.assertEquals("post-mortem Java thread NativeCreatedNativeStartedQLoopThread is still associated to Qt thread", null, QThread.thread(jthreads.get()));
			}
			jthreads.set(null);
		}
		runningThreads = null;
		{
			Assert.assertTrue("QThread NativeCreatedNativeStartedQLoopThread is null", qthreads.get()!=null);
			Assert.assertFalse("post-mortem QThread NativeCreatedNativeStartedQLoopThread still alive", qthreads.get().isAlive());
			Assert.assertFalse("post-mortem QThread NativeCreatedNativeStartedQLoopThread still running", qthreads.get().isRunning());
			Assert.assertTrue("post-mortem QThread NativeCreatedNativeStartedQLoopThread not finished", qthreads.get().isFinished());
			switch(ownership.get()) {
			case 1:
				Assert.assertTrue("post-mortem QThread NativeCreatedNativeStartedQLoopThread not Java ownership", General.internalAccess.isJavaOwnership(qthreads.get()));
				break;
			case 2:
				Assert.assertTrue("post-mortem QThread NativeCreatedNativeStartedQLoopThread not Cpp ownership", General.internalAccess.isCppOwnership(qthreads.get()));
				break;
			case 3:
				Assert.assertTrue("post-mortem QThread NativeCreatedNativeStartedQLoopThread not split ownership", General.internalAccess.isSplitOwnership(qthreads.get()));
				break;
			}
			qthreads.set(null);
		}
		QEventLoop loop = new QEventLoop();
		{
			QTimer timer = new QTimer();
			long t1 = System.currentTimeMillis();
			timer.timeout.connect(()->{
				Thread.yield();
				Thread.sleep(250);
				runGC();
				QCoreApplication.processEvents();
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
				if(finishedReceiver.received() || System.currentTimeMillis()-t1>15000) {
					loop.quit();
				}
			});
			timer.setInterval(300);
			timer.start();
			loop.exec();
			timer.dispose();
		}
		{
			QTimer timer = new QTimer();
			long t1 = System.currentTimeMillis();
			timer.timeout.connect(()->{
				Thread.yield();
				Thread.sleep(250);
				runGC();
				QCoreApplication.processEvents();
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
				if(gcQtThreadObjectsCounter.get()==1 || System.currentTimeMillis()-t1>15000) {
					loop.quit();
				}
			});
			timer.setInterval(300);
			timer.start();
			loop.exec();
			timer.dispose();
		}
		{
			QTimer timer = new QTimer();
			long t1 = System.currentTimeMillis();
			timer.timeout.connect(()->{
				Thread.yield();
				Thread.sleep(250);
				runGC();
				QCoreApplication.processEvents();
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
				if(gcJavaThreadObjectsCounter.get()==1 || System.currentTimeMillis()-t1>15000) {
					loop.quit();
				}
			});
			timer.setInterval(300);
			timer.start();
			loop.exec();
			timer.dispose();
		}
		Assert.assertEquals("Java thread object NativeCreatedNativeStartedQLoopThread not GC-removed", 1, gcJavaThreadObjectsCounter.get());
		Assert.assertTrue("QWorkerThread has not been deleted", destroyedReceiver.received());
		Assert.assertEquals("QThread object NativeCreatedNativeStartedQLoopThread  not GC-removed", 1, gcQtThreadObjectsCounter.get());
	}
	
	@org.junit.Test
	public void testNativeCreatedNativeStartedQWorkerThread() throws InterruptedException {
		AtomicInteger running = new AtomicInteger(0);
		AtomicInteger gcQtThreadObjectsCounter = new AtomicInteger(0);
		AtomicInteger gcJavaThreadObjectsCounter = new AtomicInteger(0);
		SignalReceiver finishedReceiver = new SignalReceiver();
		SignalReceiver destroyedReceiver = new SignalReceiver();
		AtomicReference<QThread> qthreads = new AtomicReference<>();
		AtomicReference<Thread> jthreads = new AtomicReference<>();
		AtomicReference<Throwable> throwable = new AtomicReference<>();
		AtomicInteger ownership = new AtomicInteger();
		Stopper stopper = new Stopper();
		try {
			// QThread 1 is natively started Qt thread adopted by Java
			QSemaphore semaphore = new QSemaphore();
			QThread thread;
			{
				thread = ThreadFactory.startQWorkerThread(()->{
					try {
						initializeThread(qthreads, jthreads, ownership, running, gcQtThreadObjectsCounter, gcJavaThreadObjectsCounter, destroyedReceiver, finishedReceiver, false);
						QEventLoop loop = new QEventLoop();
						QTimer.singleShot(1, semaphore::release);
						stopper.stop.connect(loop::quit, Qt.ConnectionType.DirectConnection);
						loop.exec();
						loop.dispose();
					}catch(Throwable t) {
						throwable.set(t);
					}					
				});
				Assert.assertTrue("QThread NativeCreatedNativeStartedQWorkerThread has not split ownership", General.internalAccess.isSplitOwnership(thread));
				thread.setObjectName("thread-NativeCreatedNativeStartedQWorkerThread");
			}
			
			int i=0;
			semaphore.acquire();
			Assert.assertEquals(1, running.get());
			Set<Thread> runningThreads = Thread.getAllStackTraces().keySet();
			{
				Assert.assertTrue(jthreads.get()!=null);
				Assert.assertTrue(runningThreads.contains(jthreads.get()));
				Assert.assertEquals(null, throwable.get());
				throwable.set(null);
				Assert.assertEquals(Thread.State.RUNNABLE, jthreads.get().getState());
				Assert.assertEquals("Java thread NativeCreatedNativeStartedQWorkerThread is not associated to Qt thread", qthreads.get(), QThread.thread(jthreads.get()));
				Assert.assertEquals("Qt thread NativeCreatedNativeStartedQWorkerThread is not associated to Java thread", jthreads.get(), qthreads.get().javaThread());
			}
			Assert.assertEquals(thread, qthreads.get());
			thread = null;
			{
				Assert.assertTrue("QThread NativeCreatedNativeStartedQWorkerThread is null", qthreads.get()!=null);
				Assert.assertEquals("QThread NativeCreatedNativeStartedQWorkerThread has wrong thread-affinity", QThread.currentThread(), qthreads.get().thread());
				Assert.assertFalse("QThread NativeCreatedNativeStartedQWorkerThread is disposed", qthreads.get().isDisposed());
				Assert.assertTrue("QThread NativeCreatedNativeStartedQWorkerThread not alive", qthreads.get().isAlive());
				Assert.assertTrue("QThread NativeCreatedNativeStartedQWorkerThread not running", qthreads.get().isRunning());
				Assert.assertFalse("QThread NativeCreatedNativeStartedQWorkerThread is finished", qthreads.get().isFinished());
				Assert.assertEquals(jthreads.get(), qthreads.get().javaThread());
				Assert.assertFalse("QThread NativeCreatedNativeStartedQWorkerThread has Cpp ownership", General.internalAccess.isCppOwnership(qthreads.get()));
				Assert.assertTrue("running QThread NativeCreatedNativeStartedQWorkerThread has not split ownership", ownership.get()==3);
			}
			stopper.stop.emit();
			stopper.stop.disconnect();
			finishedReceiver.waitForReleased(2000);
			
			i = 0;
			while(i<20) {
				Thread.yield();
				Thread.sleep(150);
				runGC();
				++i;
			}
			{
				try {
					if(qthreads.get().isRunning()) {
						i = 0;
						while(i<20 && qthreads.get().isRunning()) {
							Thread.yield();
							Thread.sleep(150);
							runGC();
							++i;
						}
					}
				}catch(Throwable t){}
			}
			runningThreads = Thread.getAllStackTraces().keySet();
			{
				try {
					if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)
							|| "windows-mingw-x64".equals(TestUtility.osArchName()))
						Assert.assertTrue("post-mortem Java thread NativeCreatedNativeStartedQWorkerThread state==RUNNABLE", Thread.State.RUNNABLE!=jthreads.get().getState());
					else
						Assert.assertEquals("post-mortem Java thread NativeCreatedNativeStartedQWorkerThread state", Thread.State.TERMINATED, jthreads.get().getState());
				} catch (AssertionError e) {
					StackTraceElement[] stackTraces = Thread.getAllStackTraces().get(jthreads.get());
					if(stackTraces!=null) {
						AssertionError error = new AssertionError("Thread stack");
						error.setStackTrace(stackTraces);
						e.addSuppressed(error);
						throw e;
					}else {
						runningThreads = Thread.getAllStackTraces().keySet();
					}
				}
				Assert.assertFalse("post-mortem Java thread NativeCreatedNativeStartedQWorkerThread is still running", runningThreads.contains(jthreads.get()));
				if(!qthreads.get().isDisposed()) {
					Assert.assertEquals("post-mortem Java thread NativeCreatedNativeStartedQWorkerThread is not associated to Qt thread", qthreads.get(), QThread.thread(jthreads.get()));
					Assert.assertEquals("post-mortem Qt thread NativeCreatedNativeStartedQWorkerThread is not associated to Java thread", jthreads.get(), qthreads.get().javaThread());
				}else {
					Assert.assertEquals("post-mortem Java thread NativeCreatedNativeStartedQWorkerThread is still associated to Qt thread", null, QThread.thread(jthreads.get()));
				}
				jthreads.set(null);
			}
			runningThreads = null;
			{
				Assert.assertTrue("QThread NativeCreatedNativeStartedQWorkerThread is null", qthreads.get()!=null);
				Assert.assertFalse("post-mortem QThread NativeCreatedNativeStartedQWorkerThread still alive", qthreads.get().isAlive());
				Assert.assertFalse("post-mortem QThread NativeCreatedNativeStartedQWorkerThread still running", qthreads.get().isRunning());
				Assert.assertTrue("post-mortem QThread NativeCreatedNativeStartedQWorkerThread not finished", qthreads.get().isFinished());
				switch(ownership.get()) {
				case 1:
					Assert.assertTrue("post-mortem QThread NativeCreatedNativeStartedQWorkerThread not Java ownership", General.internalAccess.isJavaOwnership(qthreads.get()));
					break;
				case 2:
					Assert.assertTrue("post-mortem QThread NativeCreatedNativeStartedQWorkerThread not Cpp ownership", General.internalAccess.isCppOwnership(qthreads.get()));
					break;
				case 3:
					Assert.assertTrue("post-mortem QThread NativeCreatedNativeStartedQWorkerThread not split ownership", General.internalAccess.isSplitOwnership(qthreads.get()));
					break;
				}
				qthreads.set(null);
			}
			QEventLoop loop = new QEventLoop();
			{
				QTimer timer = new QTimer();
				long t1 = System.currentTimeMillis();
				timer.timeout.connect(()->{
					Thread.yield();
					Thread.sleep(250);
					runGC();
					QCoreApplication.processEvents();
					QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
					if(finishedReceiver.received() || System.currentTimeMillis()-t1>15000) {
						loop.quit();
					}
				});
				timer.setInterval(300);
				timer.start();
				loop.exec();
				timer.dispose();
			}
			{
				QTimer timer = new QTimer();
				long t1 = System.currentTimeMillis();
				timer.timeout.connect(()->{
					Thread.yield();
					Thread.sleep(250);
					runGC();
					QCoreApplication.processEvents();
					QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
					if(gcQtThreadObjectsCounter.get()==1 || System.currentTimeMillis()-t1>15000) {
						loop.quit();
					}
				});
				timer.setInterval(300);
				timer.start();
				loop.exec();
				timer.dispose();
			}
			{
				QTimer timer = new QTimer();
				long t1 = System.currentTimeMillis();
				timer.timeout.connect(()->{
					Thread.yield();
					Thread.sleep(250);
					runGC();
					QCoreApplication.processEvents();
					QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
					if(gcJavaThreadObjectsCounter.get()==1 || System.currentTimeMillis()-t1>15000) {
						loop.quit();
					}
				});
				timer.setInterval(300);
				timer.start();
				loop.exec();
				timer.dispose();
			}
			Assert.assertEquals("Java thread object NativeCreatedNativeStartedQWorkerThread not GC-removed", 1, gcJavaThreadObjectsCounter.get());
			Assert.assertTrue("NativeCreatedNativeStartedQWorkerThread has not been deleted", destroyedReceiver.received());
			Assert.assertEquals("QThread object NativeCreatedNativeStartedQWorkerThread  not GC-removed", 1, gcQtThreadObjectsCounter.get());
		}catch(Error | RuntimeException e) {
			Throwable t = throwable.get();
			if(t!=null) {
				e.addSuppressed(t);
			}
			throw e;
		}
	}
	
	@org.junit.Test
	public void testStdWorkerThread() throws InterruptedException {
		Assume.assumeFalse("Always fails on MINGW", "windows-mingw-x64".equals(TestUtility.osArchName()) || QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
		AtomicInteger running = new AtomicInteger(0);
		AtomicInteger gcQtThreadObjectsCounter = new AtomicInteger(0);
		AtomicInteger gcJavaThreadObjectsCounter = new AtomicInteger(0);
		SignalReceiver finishedReceiver = new SignalReceiver();
		SignalReceiver destroyedReceiver = new SignalReceiver();
		AtomicReference<QThread> qthreads = new AtomicReference<>();
		AtomicReference<Thread> jthreads = new AtomicReference<>();
		AtomicReference<Throwable> throwable = new AtomicReference<>();
		AtomicInteger ownership = new AtomicInteger();
		Stopper stopper = new Stopper();
		try {
			// QThread 2 is natively started C++ thread adopted by Qt and by Java
			QThread thread;
			QSemaphore semaphore = new QSemaphore();
			{
				thread = ThreadFactory.startNativeWorkerThread(()->{
					try {
						initializeThread(qthreads, jthreads, ownership, running, gcQtThreadObjectsCounter, gcJavaThreadObjectsCounter, destroyedReceiver, finishedReceiver, true);
						QEventLoop loop = new QEventLoop();
						QTimer.singleShot(1, semaphore::release);
						stopper.stop.connect(loop::quit, Qt.ConnectionType.DirectConnection);
						loop.exec();
						loop.dispose();
					}catch(Throwable t) {
						throwable.set(t);
					}
				});
				Assert.assertTrue("QThread StdWorkerThread has not split ownership", General.internalAccess.isSplitOwnership(thread));
				thread.setObjectName("thread-StdWorkerThread");
			}
			
			int i=0;
			semaphore.acquire();
			Assert.assertEquals(1, running.get());
			Set<Thread> runningThreads = Thread.getAllStackTraces().keySet();
			Assert.assertEquals(thread, qthreads.get());
			{
				Assert.assertTrue(jthreads.get()!=null);
				Assert.assertTrue(runningThreads.contains(jthreads.get()));
				Assert.assertEquals(null, throwable.get());
				throwable.set(null);
				Assert.assertEquals(Thread.State.RUNNABLE, jthreads.get().getState());
				Assert.assertEquals("Java thread StdWorkerThread is not associated to Qt thread", qthreads.get(), QThread.thread(jthreads.get()));
				Assert.assertEquals("Qt thread StdWorkerThread is not associated to Java thread", jthreads.get(), qthreads.get().javaThread());
			}
			thread = null;
			{
				Assert.assertTrue("QThread StdWorkerThread is null", qthreads.get()!=null);
				Assert.assertEquals("QThread StdWorkerThread has wrong thread-affinity", qthreads.get(), qthreads.get().thread());
				Assert.assertFalse("QThread StdWorkerThread is disposed", qthreads.get().isDisposed());
				Assert.assertTrue("QThread StdWorkerThread not alive", qthreads.get().isAlive());
				Assert.assertTrue("QThread StdWorkerThread not running", qthreads.get().isRunning());
				Assert.assertFalse("QThread StdWorkerThread is finished", qthreads.get().isFinished());
				Assert.assertEquals(jthreads.get(), qthreads.get().javaThread());
				Assert.assertFalse("QThread StdWorkerThread has Cpp ownership", General.internalAccess.isCppOwnership(qthreads.get()));
				Assert.assertTrue("running QThread StdWorkerThread has not split ownership", ownership.get()==3);
			}
			stopper.stop.emit();
			stopper.stop.disconnect();
			finishedReceiver.waitForReleased(2000);
			
			i = 0;
			while(i<20) {
				Thread.yield();
				Thread.sleep(150);
				runGC();
				++i;
			}
			{
				try {
					if(qthreads.get().isRunning()) {
						i = 0;
						while(i<20 && qthreads.get().isRunning()) {
							Thread.yield();
							Thread.sleep(150);
							runGC();
							++i;
						}
					}
				}catch(Throwable t){}
			}
			runningThreads = Thread.getAllStackTraces().keySet();
			i = 0;
			while(i<20) {
				Thread.yield();
				Thread.sleep(150);
				runGC();
				++i;
			}
			{
				try {
					if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)
							|| "windows-mingw-x64".equals(TestUtility.osArchName()))
						Assert.assertTrue("post-mortem Java thread StdWorkerThread state==RUNNABLE", Thread.State.RUNNABLE!=jthreads.get().getState());
					else
						Assert.assertEquals("post-mortem Java thread StdWorkerThread state", Thread.State.TERMINATED, jthreads.get().getState());
				} catch (AssertionError e) {
					StackTraceElement[] stackTraces = Thread.getAllStackTraces().get(jthreads.get());
					if(stackTraces!=null) {
						AssertionError error = new AssertionError("Thread stack");
						error.setStackTrace(stackTraces);
						e.addSuppressed(error);
						throw e;
					}else {
						runningThreads = Thread.getAllStackTraces().keySet();
					}
				}
				Assert.assertFalse("post-mortem Java thread StdWorkerThread is still running", runningThreads.contains(jthreads.get()));
				if(!qthreads.get().isDisposed()) {
					Assert.assertEquals("post-mortem Java thread StdWorkerThread is not associated to Qt thread", qthreads.get(), QThread.thread(jthreads.get()));
					Assert.assertTrue("post-mortem Qt thread StdWorkerThread is not associated to Java thread", jthreads.get()==qthreads.get().javaThread() || qthreads.get().javaThread()==null);
				}else {
					Assert.assertEquals("post-mortem Java thread StdWorkerThread is still associated to Qt thread", null, QThread.thread(jthreads.get()));
				}
				jthreads.set(null);
			}
			runningThreads = null;
			{
				Assert.assertTrue("QThread StdWorkerThread is null", qthreads.get()!=null);
				try {
					// threads2 and thread3 might already be deleted or
					// are deleted in parallel to this test
					Assert.assertFalse("post-mortem QThread StdWorkerThread still alive", qthreads.get().isAlive());
					Assert.assertFalse("post-mortem QThread StdWorkerThread still running", qthreads.get().isRunning());
					Assert.assertTrue("post-mortem QThread StdWorkerThread not finished", qthreads.get().isFinished());
					switch(ownership.get()) {
					case 1:
						Assert.assertTrue("post-mortem QThread StdWorkerThread not Java ownership", General.internalAccess.isJavaOwnership(qthreads.get()));
						break;
					case 2:
						Assert.assertTrue("post-mortem QThread StdWorkerThread not Cpp ownership", General.internalAccess.isCppOwnership(qthreads.get()));
						break;
					case 3:
						Assert.assertTrue("post-mortem QThread StdWorkerThread not split ownership", General.internalAccess.isSplitOwnership(qthreads.get()));
						break;
					}
				}catch(QNoNativeResourcesException e){
				}
				qthreads.set(null);
			}
			QEventLoop loop = new QEventLoop();
			{
				QTimer timer = new QTimer();
				long t1 = System.currentTimeMillis();
				timer.timeout.connect(()->{
					Thread.yield();
					Thread.sleep(250);
					runGC();
					QCoreApplication.processEvents();
					QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
					if(finishedReceiver.received() || System.currentTimeMillis()-t1>15000) {
						loop.quit();
					}
				});
				timer.setInterval(300);
				timer.start();
				loop.exec();
				timer.dispose();
			}
			{
				QTimer timer = new QTimer();
				long t1 = System.currentTimeMillis();
				timer.timeout.connect(()->{
					Thread.yield();
					Thread.sleep(250);
					runGC();
					QCoreApplication.processEvents();
					QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
					if(gcQtThreadObjectsCounter.get()==1 || System.currentTimeMillis()-t1>15000) {
						loop.quit();
					}
				});
				timer.setInterval(300);
				timer.start();
				loop.exec();
				timer.dispose();
			}
			{
				QTimer timer = new QTimer();
				long t1 = System.currentTimeMillis();
				timer.timeout.connect(()->{
					Thread.yield();
					Thread.sleep(250);
					runGC();
					QCoreApplication.processEvents();
					QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
					if(gcJavaThreadObjectsCounter.get()==1 || System.currentTimeMillis()-t1>15000) {
						loop.quit();
					}
				});
				timer.setInterval(300);
				timer.start();
				loop.exec();
				timer.dispose();
			}
			Assert.assertEquals("Java thread object StdWorkerThread not GC-removed", 1, gcJavaThreadObjectsCounter.get());
			Assert.assertTrue("StdWorkerThread has not been deleted natively", destroyedReceiver.received());
			Assert.assertEquals("QThread object StdWorkerThread  not GC-removed", 1, gcQtThreadObjectsCounter.get());
		}catch(Error | RuntimeException e) {
			Throwable t = throwable.get();
			if(t!=null) {
				e.addSuppressed(t);
			}
			throw e;
		}
	}
	
	@org.junit.Test
	public void testStdLoopThread() throws InterruptedException {
		Assume.assumeFalse("Always fails on MINGW", "windows-mingw-x64".equals(TestUtility.osArchName()) || QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
		AtomicInteger running = new AtomicInteger(0);
		AtomicInteger gcQtThreadObjectsCounter = new AtomicInteger(0);
		AtomicInteger gcJavaThreadObjectsCounter = new AtomicInteger(0);
		SignalReceiver finishedReceiver = new SignalReceiver();
		SignalReceiver destroyedReceiver = new SignalReceiver();
		AtomicReference<QThread> qthreads = new AtomicReference<>();
		AtomicReference<Thread> jthreads = new AtomicReference<>();
		AtomicInteger ownership = new AtomicInteger();
		Stopper stopper = new Stopper();
		// QThread 2 is natively started C++ thread adopted by Qt and by Java
		QThread thread;
		QSemaphore semaphore = new QSemaphore();
		{
			thread = ThreadFactory.startNativeLoopThread();
			Assert.assertTrue("QThread StdLoopThread has not split ownership", General.internalAccess.isSplitOwnership(thread));
			thread.setObjectName("thread-StdLoopThread");
			QObject obj = new QObject();
			obj.destroyed.connect(()->{
				initializeThread(qthreads, jthreads, ownership, running, gcQtThreadObjectsCounter, gcJavaThreadObjectsCounter, destroyedReceiver, finishedReceiver, false);
				QTimer.singleShot(1, semaphore::release);
			}, Qt.ConnectionType.DirectConnection);
			obj.moveToThread(thread);
			obj.disposeLater();
			stopper.stop.connect(thread::quit, Qt.ConnectionType.DirectConnection);
		}
		
		int i=0;
		semaphore.acquire();
		Assert.assertEquals(1, running.get());
		Set<Thread> runningThreads = Thread.getAllStackTraces().keySet();
		{
			Assert.assertTrue(jthreads.get()!=null);
			Assert.assertEquals("Java thread StdLoopThread is not associated to Qt thread", qthreads.get(), QThread.thread(jthreads.get()));
		}
		Assert.assertEquals(thread, qthreads.get());
		thread = null;
		{
			Assert.assertTrue("QThread StdLoopThread is null", qthreads.get()!=null);
			Assert.assertEquals("QThread StdLoopThread has wrong thread-affinity", qthreads.get(), qthreads.get().thread());
			Assert.assertFalse("QThread StdLoopThread is disposed", qthreads.get().isDisposed());
			Assert.assertTrue("QThread StdLoopThread not alive", qthreads.get().isAlive());
			Assert.assertTrue("QThread StdLoopThread not running", qthreads.get().isRunning());
			Assert.assertFalse("QThread StdLoopThread is finished", qthreads.get().isFinished());
			Assert.assertFalse("QThread StdLoopThread has Cpp ownership", General.internalAccess.isCppOwnership(qthreads.get()));
			Assert.assertTrue("running QThread StdLoopThread has not split ownership", ownership.get()==3);
		}
		stopper.stop.emit();
		stopper.stop.disconnect();
		finishedReceiver.waitForReleased(2000);
		
		i = 0;
		while(i<20) {
			Thread.yield();
			Thread.sleep(150);
			runGC();
			++i;
		}
		{
			try {
				if(qthreads.get().isRunning()) {
					i = 0;
					while(i<20 && qthreads.get().isRunning()) {
						Thread.yield();
						Thread.sleep(150);
						runGC();
						++i;
					}
				}
			}catch(Throwable t){}
		}
		runningThreads = Thread.getAllStackTraces().keySet();
		{
			try {
				if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)
						|| "windows-mingw-x64".equals(TestUtility.osArchName()))
					Assert.assertTrue("post-mortem Java thread StdLoopThread state==RUNNABLE", Thread.State.RUNNABLE!=jthreads.get().getState());
				else
					Assert.assertEquals("post-mortem Java thread StdLoopThread state", Thread.State.TERMINATED, jthreads.get().getState());
			} catch (AssertionError e) {
				StackTraceElement[] stackTraces = Thread.getAllStackTraces().get(jthreads.get());
				if(stackTraces!=null) {
					AssertionError error = new AssertionError("Thread stack");
					error.setStackTrace(stackTraces);
					e.addSuppressed(error);
					throw e;
				}else {
					runningThreads = Thread.getAllStackTraces().keySet();
				}
			}
			Assert.assertFalse("post-mortem Java thread StdLoopThread is still running", runningThreads.contains(jthreads.get()));
			if(!qthreads.get().isDisposed()) {
				Assert.assertEquals("post-mortem Java thread StdLoopThread is not associated to Qt thread", qthreads.get(), QThread.thread(jthreads.get()));
				Assert.assertTrue("post-mortem Qt thread StdLoopThread is not associated to Java thread", jthreads.get()==qthreads.get().javaThread() || qthreads.get().javaThread()==null);
			}else {
				Assert.assertEquals("post-mortem Java thread StdLoopThread is still associated to Qt thread", null, QThread.thread(jthreads.get()));
			}
			jthreads.set(null);
		}
		runningThreads = null;
		{
			Assert.assertTrue("QThread StdLoopThread is null", qthreads.get()!=null);
			try {
				// threads2 and thread3 might already be deleted or
				// are deleted in parallel to this test
				Assert.assertFalse("post-mortem QThread StdLoopThread still alive", qthreads.get().isAlive());
				Assert.assertFalse("post-mortem QThread StdLoopThread still running", qthreads.get().isRunning());
				Assert.assertTrue("post-mortem QThread StdLoopThread not finished", qthreads.get().isFinished());
				switch(ownership.get()) {
				case 1:
					Assert.assertTrue("post-mortem QThread StdLoopThread not Java ownership", General.internalAccess.isJavaOwnership(qthreads.get()));
					break;
				case 2:
					Assert.assertTrue("post-mortem QThread StdLoopThread not Cpp ownership", General.internalAccess.isCppOwnership(qthreads.get()));
					break;
				case 3:
					Assert.assertTrue("post-mortem QThread StdLoopThread not split ownership", General.internalAccess.isSplitOwnership(qthreads.get()));
					break;
				}
			}catch(QNoNativeResourcesException e){
			}
			qthreads.set(null);
		}
		QEventLoop loop = new QEventLoop();
		{
			QTimer timer = new QTimer();
			long t1 = System.currentTimeMillis();
			timer.timeout.connect(()->{
				Thread.yield();
				Thread.sleep(250);
				runGC();
				QCoreApplication.processEvents();
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
				if(finishedReceiver.received() || System.currentTimeMillis()-t1>15000) {
					loop.quit();
				}
			});
			timer.setInterval(300);
			timer.start();
			loop.exec();
			timer.dispose();
		}
		{
			QTimer timer = new QTimer();
			long t1 = System.currentTimeMillis();
			timer.timeout.connect(()->{
				Thread.yield();
				Thread.sleep(250);
				runGC();
				QCoreApplication.processEvents();
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
				if(gcQtThreadObjectsCounter.get()==1 || System.currentTimeMillis()-t1>15000) {
					loop.quit();
				}
			});
			timer.setInterval(300);
			timer.start();
			loop.exec();
			timer.dispose();
		}
		{
			QTimer timer = new QTimer();
			long t1 = System.currentTimeMillis();
			timer.timeout.connect(()->{
				Thread.yield();
				Thread.sleep(250);
				runGC();
				QCoreApplication.processEvents();
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
				if(gcJavaThreadObjectsCounter.get()==1 || System.currentTimeMillis()-t1>15000) {
					loop.quit();
				}
			});
			timer.setInterval(300);
			timer.start();
			loop.exec();
			timer.dispose();
		}
		Assert.assertEquals("Java thread object StdLoopThread not GC-removed", 1, gcJavaThreadObjectsCounter.get());
		Assert.assertTrue("StdLoopThread has not been deleted natively", destroyedReceiver.received());
		Assert.assertEquals("QThread object StdLoopThread  not GC-removed", 1, gcQtThreadObjectsCounter.get());
	}
	
	@org.junit.Test
	public void testJavaThread() throws InterruptedException {
		AtomicInteger running = new AtomicInteger(0);
		AtomicInteger gcQtThreadObjectsCounter = new AtomicInteger(0);
		AtomicInteger gcJavaThreadObjectsCounter = new AtomicInteger(0);
		SignalReceiver finishedReceiver = new SignalReceiver();
		SignalReceiver destroyedReceiver = new SignalReceiver();
		AtomicReference<QThread> qthreads = new AtomicReference<>();
		AtomicReference<Thread> jthreads = new AtomicReference<>();
		AtomicInteger ownership = new AtomicInteger();
		Stopper stopper = new Stopper();
		// QThread 3 is Java thread adopted by Qt
		Thread thread;
		QSemaphore semaphore = new QSemaphore();
		{
			thread = new Thread(()->{
				initializeThread(qthreads, jthreads, ownership, running, gcQtThreadObjectsCounter, gcJavaThreadObjectsCounter, destroyedReceiver, finishedReceiver, false);
				QEventLoop loop = new QEventLoop();
				QTimer.singleShot(1, semaphore::release);
				stopper.stop.connect(loop::quit, Qt.ConnectionType.DirectConnection);
				loop.exec();
				loop.dispose();
			});
			thread.setName("thread-JavaThread");
			thread.start();
		}
		
		int i=0;
		semaphore.acquire();
		Assert.assertEquals(1, running.get());
		Set<Thread> runningThreads = Thread.getAllStackTraces().keySet();
		Assert.assertTrue(jthreads.get()!=null);
		Assert.assertTrue(runningThreads.contains(jthreads.get()));
		Assert.assertEquals(Thread.State.RUNNABLE, jthreads.get().getState());
		Assert.assertEquals("Java thread JavaThread is not associated to Qt thread", qthreads.get(), QThread.thread(jthreads.get()));
		Assert.assertEquals("Qt thread JavaThread is not associated to Java thread", jthreads.get(), qthreads.get().javaThread());
		Assert.assertEquals(thread, jthreads.get());
		thread = null;
		Assert.assertTrue("QThread JavaThread is null", qthreads.get()!=null);
		Assert.assertEquals("QThread JavaThread has wrong thread-affinity", qthreads.get(), qthreads.get().thread());
		Assert.assertFalse("QThread JavaThread is disposed", qthreads.get().isDisposed());
		Assert.assertTrue("QThread JavaThread not alive", qthreads.get().isAlive());
		Assert.assertTrue("QThread JavaThread not running", qthreads.get().isRunning());
		Assert.assertFalse("QThread JavaThread is finished", qthreads.get().isFinished());
		Assert.assertEquals(jthreads.get(), qthreads.get().javaThread());
		Assert.assertFalse("QThread JavaThread has Cpp ownership", General.internalAccess.isCppOwnership(qthreads.get()));
		Assert.assertTrue("running QThread JavaThread has not split ownership", ownership.get()==3);
		
		stopper.stop.emit();
		stopper.stop.disconnect();
		finishedReceiver.waitForReleased(2000);
		
		i = 0;
		while(i<20) {
			Thread.yield();
			Thread.sleep(150);
			runGC();
			++i;
		}
		try {
			if(qthreads.get().isRunning()) {
				i = 0;
				while(i<20 && qthreads.get().isRunning()) {
					Thread.yield();
					Thread.sleep(150);
					runGC();
					++i;
				}
			}
		}catch(Throwable t){}
		i = 0;
		while(i<20) {
			Thread.yield();
			Thread.sleep(150);
			runGC();
			++i;
		}
		runningThreads = Thread.getAllStackTraces().keySet();
		{
			try {
				if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)
						|| "windows-mingw-x64".equals(TestUtility.osArchName()))
					Assert.assertTrue("post-mortem Java thread JavaThread state==RUNNABLE", Thread.State.RUNNABLE!=jthreads.get().getState());
				else
					Assert.assertEquals("post-mortem Java thread JavaThread state", Thread.State.TERMINATED, jthreads.get().getState());
			} catch (AssertionError e) {
				StackTraceElement[] stackTraces = Thread.getAllStackTraces().get(jthreads.get());
				if(stackTraces!=null) {
					AssertionError error = new AssertionError("Thread stack");
					error.setStackTrace(stackTraces);
					e.addSuppressed(error);
					throw e;
				}else {
					runningThreads = Thread.getAllStackTraces().keySet();
				}
			}
			Assert.assertFalse("post-mortem Java thread JavaThread is still running", runningThreads.contains(jthreads.get()));
			QThread reverse = QThread.thread(jthreads.get());
			if(reverse!=null) {
				Assert.assertTrue("post-mortem Java thread JavaThread is not associated to Qt thread", qthreads.get()==reverse);
				Assert.assertEquals("post-mortem Qt thread JavaThread is not associated to Java thread", jthreads.get(), qthreads.get().javaThread());
			}
			jthreads.set(null);
		}
		runningThreads = null;
		Assert.assertTrue("QThread JavaThread is null", qthreads.get()!=null);
		try {
			// threads2 and thread3 might already be deleted or
			// are deleted in parallel to this test
			Assert.assertFalse("post-mortem QThread JavaThread still alive", qthreads.get().isAlive());
			Assert.assertFalse("post-mortem QThread JavaThread still running", qthreads.get().isRunning());
			Assert.assertTrue("post-mortem QThread JavaThread not finished", qthreads.get().isFinished());
			switch(ownership.get()) {
			case 1:
				Assert.assertTrue("post-mortem QThread JavaThread not Java ownership", General.internalAccess.isJavaOwnership(qthreads.get()));
				break;
			case 2:
				Assert.assertTrue("post-mortem QThread JavaThread not Cpp ownership", General.internalAccess.isCppOwnership(qthreads.get()));
				break;
			case 3:
				Assert.assertTrue("post-mortem QThread JavaThread not split ownership", General.internalAccess.isSplitOwnership(qthreads.get()));
				break;
			}
		}catch(QNoNativeResourcesException e){
		}
		qthreads.set(null);
		QEventLoop loop = new QEventLoop();
		{
			QTimer timer = new QTimer();
			long t1 = System.currentTimeMillis();
			timer.timeout.connect(()->{
				Thread.yield();
				Thread.sleep(250);
				runGC();
				QCoreApplication.processEvents();
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
				if(finishedReceiver.received() || System.currentTimeMillis()-t1>15000) {
					loop.quit();
				}
			});
			timer.setInterval(300);
			timer.start();
			loop.exec();
			timer.dispose();
		}
		{
			QTimer timer = new QTimer();
			long t1 = System.currentTimeMillis();
			timer.timeout.connect(()->{
				Thread.yield();
				Thread.sleep(250);
				runGC();
				QCoreApplication.processEvents();
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
				if(gcQtThreadObjectsCounter.get()==1 || System.currentTimeMillis()-t1>15000) {
					loop.quit();
				}
			});
			timer.setInterval(300);
			timer.start();
			loop.exec();
			timer.dispose();
		}
		{
			QTimer timer = new QTimer();
			long t1 = System.currentTimeMillis();
			timer.timeout.connect(()->{
				Thread.yield();
				Thread.sleep(250);
				runGC();
				QCoreApplication.processEvents();
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
				if(gcJavaThreadObjectsCounter.get()==1 || System.currentTimeMillis()-t1>15000) {
					loop.quit();
				}
			});
			timer.setInterval(300);
			timer.start();
			loop.exec();
			timer.dispose();
		}
		Assert.assertEquals("Java thread object JavaThread not GC-removed", 1, gcJavaThreadObjectsCounter.get());
		Assert.assertTrue("JavaThread has not been deleted natively", destroyedReceiver.received());
		Assert.assertEquals("QThread object JavaThread  not GC-removed", 1, gcQtThreadObjectsCounter.get());
	}
	
	@org.junit.Test
	public void testJavaCreatedJavaStartedQWorkerThread() throws InterruptedException {
		AtomicInteger running = new AtomicInteger(0);
		AtomicInteger gcQtThreadObjectsCounter = new AtomicInteger(0);
		AtomicInteger gcJavaThreadObjectsCounter = new AtomicInteger(0);
		SignalReceiver finishedReceiver = new SignalReceiver();
		SignalReceiver destroyedReceiver = new SignalReceiver();
		AtomicReference<QThread> qthreads = new AtomicReference<>();
		AtomicReference<Thread> jthreads = new AtomicReference<>();
		AtomicReference<Throwable> throwable = new AtomicReference<>();
		AtomicInteger ownership = new AtomicInteger();
		Stopper stopper = new Stopper();
		try {
			// QThread 4 is Java-started Qt thread adopted by Java
			QSemaphore semaphore = new QSemaphore();
			QThread thread;
			{
				thread = QThread.create(()->{
					try {
						initializeThread(qthreads, jthreads, ownership, running, gcQtThreadObjectsCounter, gcJavaThreadObjectsCounter, destroyedReceiver, finishedReceiver, false);
						QEventLoop loop = new QEventLoop();
						stopper.stop.connect(loop::quit, Qt.ConnectionType.DirectConnection);
						QTimer.singleShot(1, semaphore::release);
						loop.exec();
						loop.dispose();
					}catch(Throwable t) {
						throwable.set(t);
					}
				});
				Assert.assertTrue("QThread JavaCreatedJavaStartedQWorkerThread has not java ownership", General.internalAccess.isJavaOwnership(thread));
				thread.setName("thread-JavaCreatedJavaStartedQWorkerThread");
				thread.start();
				Assert.assertTrue("QThread JavaCreatedJavaStartedQWorkerThread has not java ownership", General.internalAccess.isJavaOwnership(thread));
			}
			int i=0;
			semaphore.acquire();
			Assert.assertEquals(1, running.get());
			Set<Thread> runningThreads = Thread.getAllStackTraces().keySet();
			Assert.assertTrue(jthreads.get()!=null);
			Assert.assertTrue(runningThreads.contains(jthreads.get()));
			Assert.assertEquals(null, throwable.get());
			throwable.set(null);
			Assert.assertEquals(Thread.State.RUNNABLE, jthreads.get().getState());
			Assert.assertEquals("Java thread JavaCreatedJavaStartedQWorkerThread is not associated to Qt thread", qthreads.get(), QThread.thread(jthreads.get()));
			Assert.assertEquals("Qt thread JavaCreatedJavaStartedQWorkerThread is not associated to Java thread", jthreads.get(), qthreads.get().javaThread());
			Assert.assertEquals(thread, qthreads.get());
			thread = null;
			{
				Assert.assertTrue("QThread JavaCreatedJavaStartedQWorkerThread is null", qthreads.get()!=null);
				Assert.assertEquals("QThread JavaCreatedJavaStartedQWorkerThread has wrong thread-affinity", QThread.currentThread(), qthreads.get().thread());
				Assert.assertFalse("QThread JavaCreatedJavaStartedQWorkerThread is disposed", qthreads.get().isDisposed());
				Assert.assertTrue("QThread JavaCreatedJavaStartedQWorkerThread not alive", qthreads.get().isAlive());
				Assert.assertTrue("QThread JavaCreatedJavaStartedQWorkerThread not running", qthreads.get().isRunning());
				Assert.assertFalse("QThread JavaCreatedJavaStartedQWorkerThread is finished", qthreads.get().isFinished());
				Assert.assertEquals(jthreads.get(), qthreads.get().javaThread());
				Assert.assertFalse("QThread JavaCreatedJavaStartedQWorkerThread has Cpp ownership", General.internalAccess.isCppOwnership(qthreads.get()));
				Assert.assertTrue("running QThread JavaCreatedJavaStartedQWorkerThread has not Java ownership", ownership.get()==1);
			}
			
			stopper.stop.emit();
			stopper.stop.disconnect();
			finishedReceiver.waitForReleased(2000);
			
			i = 0;
			while(i<20) {
				Thread.yield();
				Thread.sleep(150);
				runGC();
				++i;
			}
			{
				try {
					if(qthreads.get().isRunning()) {
						i = 0;
						while(i<20 && qthreads.get().isRunning()) {
							Thread.yield();
							Thread.sleep(150);
							runGC();
							++i;
						}
					}
				}catch(Throwable t){}
			}
			runningThreads = Thread.getAllStackTraces().keySet();
			{
				try {
					if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)
							|| "windows-mingw-x64".equals(TestUtility.osArchName()))
						Assert.assertTrue("post-mortem Java thread JavaCreatedJavaStartedQWorkerThread state==RUNNABLE", Thread.State.RUNNABLE!=jthreads.get().getState());
					else
						Assert.assertEquals("post-mortem Java thread JavaCreatedJavaStartedQWorkerThread state", Thread.State.TERMINATED, jthreads.get().getState());
				} catch (AssertionError e) {
					StackTraceElement[] stackTraces = Thread.getAllStackTraces().get(jthreads.get());
					if(stackTraces!=null) {
						AssertionError error = new AssertionError("Thread stack");
						error.setStackTrace(stackTraces);
						e.addSuppressed(error);
						e.printStackTrace();
						throw e;
					}else {
						runningThreads = Thread.getAllStackTraces().keySet();
					}
				}
				Assert.assertFalse("post-mortem Java thread JavaCreatedJavaStartedQWorkerThread is still running", runningThreads.contains(jthreads.get()));
				if(!qthreads.get().isDisposed()) {
					Assert.assertEquals("post-mortem Java thread JavaCreatedJavaStartedQWorkerThread is not associated to Qt thread", qthreads.get(), QThread.thread(jthreads.get()));
					Assert.assertEquals("post-mortem Qt thread JavaCreatedJavaStartedQWorkerThread is not associated to Java thread", jthreads.get(), qthreads.get().javaThread());
				}else {
					Assert.assertEquals("post-mortem Java thread JavaCreatedJavaStartedQWorkerThread is still associated to Qt thread", null, QThread.thread(jthreads.get()));
				}
				jthreads.set(null);
			}
			runningThreads = null;
			{
				Assert.assertTrue("QThread JavaCreatedJavaStartedQWorkerThread is null", qthreads.get()!=null);
				Assert.assertFalse("post-mortem QThread JavaCreatedJavaStartedQWorkerThread still alive", qthreads.get().isAlive());
				Assert.assertFalse("post-mortem QThread JavaCreatedJavaStartedQWorkerThread still running", qthreads.get().isRunning());
				Assert.assertTrue("post-mortem QThread JavaCreatedJavaStartedQWorkerThread not finished", qthreads.get().isFinished());
				switch(ownership.get()) {
				case 1:
					Assert.assertTrue("post-mortem QThread JavaCreatedJavaStartedQWorkerThread not Java ownership", General.internalAccess.isJavaOwnership(qthreads.get()));
					break;
				case 2:
					Assert.assertTrue("post-mortem QThread JavaCreatedJavaStartedQWorkerThread not Cpp ownership", General.internalAccess.isCppOwnership(qthreads.get()));
					break;
				case 3:
					Assert.assertTrue("post-mortem QThread JavaCreatedJavaStartedQWorkerThread not split ownership", General.internalAccess.isSplitOwnership(qthreads.get()));
					break;
				}
				qthreads.set(null);
			}
			QEventLoop loop = new QEventLoop();
			{
				QTimer timer = new QTimer();
				long t1 = System.currentTimeMillis();
				timer.timeout.connect(()->{
					Thread.yield();
					Thread.sleep(250);
					runGC();
					QCoreApplication.processEvents();
					QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
					if(finishedReceiver.received() || System.currentTimeMillis()-t1>15000) {
						loop.quit();
					}
				});
				timer.setInterval(300);
				timer.start();
				loop.exec();
				timer.dispose();
			}
			{
				QTimer timer = new QTimer();
				long t1 = System.currentTimeMillis();
				timer.timeout.connect(()->{
					Thread.yield();
					Thread.sleep(250);
					runGC();
					QCoreApplication.processEvents();
					QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
					if(gcQtThreadObjectsCounter.get()==1 || System.currentTimeMillis()-t1>15000) {
						loop.quit();
					}
				});
				timer.setInterval(300);
				timer.start();
				loop.exec();
				timer.dispose();
			}
			{
				QTimer timer = new QTimer();
				long t1 = System.currentTimeMillis();
				timer.timeout.connect(()->{
					Thread.yield();
					Thread.sleep(250);
					runGC();
					QCoreApplication.processEvents();
					QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
					if(gcJavaThreadObjectsCounter.get()==1 || System.currentTimeMillis()-t1>15000) {
						loop.quit();
					}
				});
				timer.setInterval(300);
				timer.start();
				loop.exec();
				timer.dispose();
			}
			if(!QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android))
				Assert.assertEquals("Java thread object JavaCreatedJavaStartedQWorkerThread not GC-removed", 1, gcJavaThreadObjectsCounter.get());
			Assert.assertTrue("JavaCreatedJavaStartedQWorkerThread has not been deleted natively", destroyedReceiver.received());
			Assert.assertEquals("QThread object JavaCreatedJavaStartedQWorkerThread  not GC-removed", 1, gcQtThreadObjectsCounter.get());
		}catch(Error | RuntimeException e) {
			Throwable t = throwable.get();
			if(t!=null) {
				e.addSuppressed(t);
			}
			throw e;
		}
	}
	
	@org.junit.Test
	public void testJavaCreatedJavaStartedQLoopThread() throws InterruptedException {
		AtomicInteger running = new AtomicInteger(0);
		AtomicInteger gcQtThreadObjectsCounter = new AtomicInteger(0);
		AtomicInteger gcJavaThreadObjectsCounter = new AtomicInteger(0);
		SignalReceiver finishedReceiver = new SignalReceiver();
		SignalReceiver destroyedReceiver = new SignalReceiver();
		AtomicReference<QThread> qthreads = new AtomicReference<>();
		AtomicReference<Thread> jthreads = new AtomicReference<>();
		AtomicInteger ownership = new AtomicInteger();
		Stopper stopper = new Stopper();
		// QThread 5 is Java-started looping Qt thread adopted by Java
		QThread thread;
		QSemaphore semaphore = new QSemaphore();
		{
			thread = new QThread();
			thread.setObjectName("thread-JavaCreatedJavaStartedQLoopThread");
			{
				QObject obj = new QObject();
				obj.destroyed.connect(()->{
					initializeThread(qthreads, jthreads, ownership, running, gcQtThreadObjectsCounter, gcJavaThreadObjectsCounter, destroyedReceiver, finishedReceiver, false);
					QTimer.singleShot(1, semaphore::release);
				});
				obj.moveToThread(thread);
				obj.disposeLater();
			}
			Assert.assertTrue("QThread JavaCreatedJavaStartedQLoopThread has not java ownership", General.internalAccess.isJavaOwnership(thread));
			thread.start();
			stopper.stop.connect(thread::quit, Qt.ConnectionType.DirectConnection);
			Assert.assertTrue("QThread JavaCreatedJavaStartedQLoopThread has not java ownership", General.internalAccess.isJavaOwnership(thread));
		}
		int i=0;
		semaphore.acquire();
		Assert.assertEquals(1, running.get());
		Set<Thread> runningThreads = Thread.getAllStackTraces().keySet();
		Assert.assertTrue(jthreads.get()!=null);
		Assert.assertTrue(runningThreads.contains(jthreads.get()));
		Assert.assertEquals(Thread.State.RUNNABLE, jthreads.get().getState());
		Assert.assertEquals("Java thread JavaCreatedJavaStartedQLoopThread is not associated to Qt thread", qthreads.get(), QThread.thread(jthreads.get()));
		Assert.assertEquals("Qt thread JavaCreatedJavaStartedQLoopThread is not associated to Java thread", jthreads.get(), qthreads.get().javaThread());
		Assert.assertEquals(thread, qthreads.get());
		thread = null;
		Assert.assertTrue("QThread JavaCreatedJavaStartedQLoopThread is null", qthreads.get()!=null);
		Assert.assertEquals("QThread JavaCreatedJavaStartedQLoopThread has wrong thread-affinity", QThread.currentThread(), qthreads.get().thread());
		Assert.assertFalse("QThread JavaCreatedJavaStartedQLoopThread is disposed", qthreads.get().isDisposed());
		Assert.assertTrue("QThread JavaCreatedJavaStartedQLoopThread not alive", qthreads.get().isAlive());
		Assert.assertTrue("QThread JavaCreatedJavaStartedQLoopThread not running", qthreads.get().isRunning());
		Assert.assertFalse("QThread JavaCreatedJavaStartedQLoopThread is finished", qthreads.get().isFinished());
		Assert.assertEquals(jthreads.get(), qthreads.get().javaThread());
		Assert.assertFalse("QThread JavaCreatedJavaStartedQLoopThread has Cpp ownership", General.internalAccess.isCppOwnership(qthreads.get()));
		Assert.assertTrue("running QThread JavaCreatedJavaStartedQLoopThread has not Java ownership", ownership.get()==1);

		stopper.stop.emit();
		stopper.stop.disconnect();
		finishedReceiver.waitForReleased(2000);
		
		i = 0;
		while(i<20) {
			Thread.yield();
			Thread.sleep(150);
			runGC();
			++i;
		}
		{
			try {
				if(qthreads.get().isRunning()) {
					i = 0;
					while(i<20 && qthreads.get().isRunning()) {
						Thread.yield();
						Thread.sleep(150);
						runGC();
						++i;
					}
				}
			}catch(Throwable t){}
		}
		runningThreads = Thread.getAllStackTraces().keySet();
		{
			try {
				if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)
						|| "windows-mingw-x64".equals(TestUtility.osArchName()))
					Assert.assertTrue("post-mortem Java thread JavaCreatedJavaStartedQLoopThread state==RUNNABLE", Thread.State.RUNNABLE!=jthreads.get().getState());
				else
					Assert.assertEquals("post-mortem Java thread JavaCreatedJavaStartedQLoopThread state", Thread.State.TERMINATED, jthreads.get().getState());
			} catch (AssertionError e) {
				StackTraceElement[] stackTraces = Thread.getAllStackTraces().get(jthreads.get());
				if(stackTraces!=null) {
					AssertionError error = new AssertionError("Thread stack");
					error.setStackTrace(stackTraces);
					e.addSuppressed(error);
					throw e;
				}else {
					runningThreads = Thread.getAllStackTraces().keySet();
				}
			}
			Assert.assertFalse("post-mortem Java thread JavaCreatedJavaStartedQLoopThread is still running", runningThreads.contains(jthreads.get()));
			if(!qthreads.get().isDisposed()) {
				Assert.assertEquals("post-mortem Java thread JavaCreatedJavaStartedQLoopThread is not associated to Qt thread", qthreads.get(), QThread.thread(jthreads.get()));
				Assert.assertEquals("post-mortem Qt thread JavaCreatedJavaStartedQLoopThread is not associated to Java thread", jthreads.get(), qthreads.get().javaThread());
			}else {
				Assert.assertEquals("post-mortem Java thread JavaCreatedJavaStartedQLoopThread is still associated to Qt thread", null, QThread.thread(jthreads.get()));
			}
			jthreads.set(null);
		}
		runningThreads = null;
		{
			Assert.assertTrue("QThread JavaCreatedJavaStartedQLoopThread is null", qthreads.get()!=null);
			Assert.assertFalse("post-mortem QThread JavaCreatedJavaStartedQLoopThread still alive", qthreads.get().isAlive());
			Assert.assertFalse("post-mortem QThread JavaCreatedJavaStartedQLoopThread still running", qthreads.get().isRunning());
			Assert.assertTrue("post-mortem QThread JavaCreatedJavaStartedQLoopThread not finished", qthreads.get().isFinished());
			switch(ownership.get()) {
			case 1:
				Assert.assertTrue("post-mortem QThread JavaCreatedJavaStartedQLoopThread not Java ownership", General.internalAccess.isJavaOwnership(qthreads.get()));
				break;
			case 2:
				Assert.assertTrue("post-mortem QThread JavaCreatedJavaStartedQLoopThread not Cpp ownership", General.internalAccess.isCppOwnership(qthreads.get()));
				break;
			case 3:
				Assert.assertTrue("post-mortem QThread JavaCreatedJavaStartedQLoopThread not split ownership", General.internalAccess.isSplitOwnership(qthreads.get()));
				break;
			}
			qthreads.set(null);
		}
		QEventLoop loop = new QEventLoop();
		{
			QTimer timer = new QTimer();
			long t1 = System.currentTimeMillis();
			timer.timeout.connect(()->{
				Thread.yield();
				Thread.sleep(250);
				runGC();
				QCoreApplication.processEvents();
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
				if(finishedReceiver.received() || System.currentTimeMillis()-t1>15000) {
					loop.quit();
				}
			});
			timer.setInterval(300);
			timer.start();
			loop.exec();
			timer.dispose();
		}
		{
			QTimer timer = new QTimer();
			long t1 = System.currentTimeMillis();
			timer.timeout.connect(()->{
				Thread.yield();
				Thread.sleep(250);
				runGC();
				QCoreApplication.processEvents();
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
				if(gcQtThreadObjectsCounter.get()==1 || System.currentTimeMillis()-t1>15000) {
					loop.quit();
				}
			});
			timer.setInterval(300);
			timer.start();
			loop.exec();
			timer.dispose();
		}
		{
			QTimer timer = new QTimer();
			long t1 = System.currentTimeMillis();
			timer.timeout.connect(()->{
				Thread.yield();
				Thread.sleep(250);
				runGC();
				QCoreApplication.processEvents();
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
				if(gcJavaThreadObjectsCounter.get()==1 || System.currentTimeMillis()-t1>15000) {
					loop.quit();
				}
			});
			timer.setInterval(300);
			timer.start();
			loop.exec();
			timer.dispose();
		}
		Assert.assertEquals("Java thread object JavaCreatedJavaStartedQLoopThread not GC-removed", 1, gcJavaThreadObjectsCounter.get());
		Assert.assertTrue("JavaCreatedJavaStartedQLoopThread has not been deleted natively", destroyedReceiver.received());
		Assert.assertEquals("QThread object JavaCreatedJavaStartedQLoopThread  not GC-removed", 1, gcQtThreadObjectsCounter.get());
	}
	
	@org.junit.Test
	public void testNativeCreatedJavaStartedQLoopThread() throws InterruptedException {
		AtomicInteger running = new AtomicInteger(0);
		AtomicInteger gcQtThreadObjectsCounter = new AtomicInteger(0);
		AtomicInteger gcJavaThreadObjectsCounter = new AtomicInteger(0);
		SignalReceiver finishedReceiver = new SignalReceiver();
		SignalReceiver destroyedReceiver = new SignalReceiver();
		AtomicReference<QThread> qthreads = new AtomicReference<>();
		AtomicReference<Thread> jthreads = new AtomicReference<>();
		AtomicInteger ownership = new AtomicInteger();
		Stopper stopper = new Stopper();
		// QThread 6 is natively created looping Qt thread adopted by Java
		QThread thread;
		QSemaphore semaphore = new QSemaphore();
		{
			thread = ThreadFactory.createQLoopThread();
			thread.setObjectName("thread-NativeCreatedJavaStartedQLoopThread");
			{
				QObject obj = new QObject();
				obj.destroyed.connect(()->{
					QTimer.singleShot(1, semaphore::release);
					initializeThread(qthreads, jthreads, ownership, running, gcQtThreadObjectsCounter, gcJavaThreadObjectsCounter, destroyedReceiver, finishedReceiver, false);
				});
				obj.moveToThread(thread);
				obj.disposeLater();
			}
			Assert.assertTrue("QThread NativeCreatedJavaStartedQLoopThread has not java ownership", General.internalAccess.isJavaOwnership(thread));
			thread.start();
			stopper.stop.connect(thread::quit, Qt.ConnectionType.DirectConnection);
			Assert.assertTrue("QThread NativeCreatedJavaStartedQLoopThread has not java ownership", General.internalAccess.isJavaOwnership(thread));
		}
		int i=0;
		semaphore.acquire();
		Assert.assertEquals(1, running.get());
		Set<Thread> runningThreads = Thread.getAllStackTraces().keySet();
		{
			Assert.assertTrue(jthreads.get()!=null);
			Assert.assertTrue(runningThreads.contains(jthreads.get()));
			Assert.assertEquals(Thread.State.RUNNABLE, jthreads.get().getState());
			Assert.assertEquals("Java thread NativeCreatedJavaStartedQLoopThread is not associated to Qt thread", qthreads.get(), QThread.thread(jthreads.get()));
			Assert.assertEquals("Qt thread NativeCreatedJavaStartedQLoopThread is not associated to Java thread", jthreads.get(), qthreads.get().javaThread());
		}
		Assert.assertEquals(thread, qthreads.get());
		thread = null;
		{
			Assert.assertTrue("QThread NativeCreatedJavaStartedQLoopThread is null", qthreads.get()!=null);
			Assert.assertEquals("QThread NativeCreatedJavaStartedQLoopThread has wrong thread-affinity", QThread.currentThread(), qthreads.get().thread());
			Assert.assertFalse("QThread NativeCreatedJavaStartedQLoopThread is disposed", qthreads.get().isDisposed());
			Assert.assertTrue("QThread NativeCreatedJavaStartedQLoopThread not alive", qthreads.get().isAlive());
			Assert.assertTrue("QThread NativeCreatedJavaStartedQLoopThread not running", qthreads.get().isRunning());
			Assert.assertFalse("QThread NativeCreatedJavaStartedQLoopThread is finished", qthreads.get().isFinished());
			Assert.assertEquals(jthreads.get(), qthreads.get().javaThread());
			Assert.assertFalse("QThread NativeCreatedJavaStartedQLoopThread has Cpp ownership", General.internalAccess.isCppOwnership(qthreads.get()));
			Assert.assertTrue("running QThread NativeCreatedJavaStartedQLoopThread has not Java ownership", ownership.get()==1);
		}
		
		stopper.stop.emit();
		stopper.stop.disconnect();
		finishedReceiver.waitForReleased(2000);
		
		i = 0;
		while(i<20) {
			Thread.yield();
			Thread.sleep(150);
			runGC();
			++i;
		}
		{
			try {
				if(qthreads.get().isRunning()) {
					i = 0;
					while(i<20 && qthreads.get().isRunning()) {
						Thread.yield();
						Thread.sleep(150);
						runGC();
						++i;
					}
				}
			}catch(Throwable t){}
		}
		runningThreads = Thread.getAllStackTraces().keySet();
		{
			try {
				if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)
						|| "windows-mingw-x64".equals(TestUtility.osArchName()))
					Assert.assertTrue("post-mortem Java thread NativeCreatedJavaStartedQLoopThread state==RUNNABLE", Thread.State.RUNNABLE!=jthreads.get().getState());
				else
					Assert.assertEquals("post-mortem Java thread NativeCreatedJavaStartedQLoopThread state", Thread.State.TERMINATED, jthreads.get().getState());
			} catch (AssertionError e) {
				StackTraceElement[] stackTraces = Thread.getAllStackTraces().get(jthreads.get());
				if(stackTraces!=null) {
					AssertionError error = new AssertionError("Thread stack");
					error.setStackTrace(stackTraces);
					e.addSuppressed(error);
					throw e;
				}else {
					runningThreads = Thread.getAllStackTraces().keySet();
				}
			}
			Assert.assertFalse("post-mortem Java thread NativeCreatedJavaStartedQLoopThread is still running", runningThreads.contains(jthreads.get()));
			if(!qthreads.get().isDisposed()) {
				Assert.assertEquals("post-mortem Java thread NativeCreatedJavaStartedQLoopThread is not associated to Qt thread", qthreads.get(), QThread.thread(jthreads.get()));
				Assert.assertEquals("post-mortem Qt thread NativeCreatedJavaStartedQLoopThread is not associated to Java thread", jthreads.get(), qthreads.get().javaThread());
			}else {
				Assert.assertEquals("post-mortem Java thread NativeCreatedJavaStartedQLoopThread is still associated to Qt thread", null, QThread.thread(jthreads.get()));
			}
			jthreads.set(null);
		}
		runningThreads = null;
		{
			Assert.assertTrue("QThread NativeCreatedJavaStartedQLoopThread is null", qthreads.get()!=null);
			Assert.assertFalse("post-mortem QThread NativeCreatedJavaStartedQLoopThread still alive", qthreads.get().isAlive());
			Assert.assertFalse("post-mortem QThread NativeCreatedJavaStartedQLoopThread still running", qthreads.get().isRunning());
			Assert.assertTrue("post-mortem QThread NativeCreatedJavaStartedQLoopThread not finished", qthreads.get().isFinished());
			switch(ownership.get()) {
			case 1:
				Assert.assertTrue("post-mortem QThread NativeCreatedJavaStartedQLoopThread not Java ownership", General.internalAccess.isJavaOwnership(qthreads.get()));
				break;
			case 2:
				Assert.assertTrue("post-mortem QThread NativeCreatedJavaStartedQLoopThread not Cpp ownership", General.internalAccess.isCppOwnership(qthreads.get()));
				break;
			case 3:
				Assert.assertTrue("post-mortem QThread NativeCreatedJavaStartedQLoopThread not split ownership", General.internalAccess.isSplitOwnership(qthreads.get()));
				break;
			}
			qthreads.set(null);
		}
		QEventLoop loop = new QEventLoop();
		{
			QTimer timer = new QTimer();
			long t1 = System.currentTimeMillis();
			timer.timeout.connect(()->{
				Thread.yield();
				Thread.sleep(250);
				runGC();
				QCoreApplication.processEvents();
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
				if(finishedReceiver.received() || System.currentTimeMillis()-t1>15000) {
					loop.quit();
				}
			});
			timer.setInterval(300);
			timer.start();
			loop.exec();
			timer.dispose();
		}
		{
			QTimer timer = new QTimer();
			long t1 = System.currentTimeMillis();
			timer.timeout.connect(()->{
				Thread.yield();
				Thread.sleep(250);
				runGC();
				QCoreApplication.processEvents();
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
				if(gcQtThreadObjectsCounter.get()==1 || System.currentTimeMillis()-t1>15000) {
					loop.quit();
				}
			});
			timer.setInterval(300);
			timer.start();
			loop.exec();
			timer.dispose();
		}
		{
			QTimer timer = new QTimer();
			long t1 = System.currentTimeMillis();
			timer.timeout.connect(()->{
				Thread.yield();
				Thread.sleep(250);
				runGC();
				QCoreApplication.processEvents();
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
				if(gcJavaThreadObjectsCounter.get()==1 || System.currentTimeMillis()-t1>15000) {
					loop.quit();
				}
			});
			timer.setInterval(300);
			timer.start();
			loop.exec();
			timer.dispose();
		}
		Assert.assertEquals("Java thread object NativeCreatedJavaStartedQLoopThread not GC-removed", 1, gcJavaThreadObjectsCounter.get());
		Assert.assertTrue("NativeCreatedJavaStartedQLoopThread has not been deleted natively", destroyedReceiver.received());
		Assert.assertEquals("QThread object NativeCreatedJavaStartedQLoopThread  not GC-removed", 1, gcQtThreadObjectsCounter.get());
	}
	
	@org.junit.Test
	public void testNativeCreatedJavaStartedQWorkerThread() throws InterruptedException {
		AtomicInteger running = new AtomicInteger(0);
		AtomicInteger gcQtThreadObjectsCounter = new AtomicInteger(0);
		AtomicInteger gcJavaThreadObjectsCounter = new AtomicInteger(0);
		SignalReceiver finishedReceiver = new SignalReceiver();
		SignalReceiver destroyedReceiver = new SignalReceiver();
		AtomicReference<QThread> qthreads = new AtomicReference<>();
		AtomicReference<Thread> jthreads = new AtomicReference<>();
		AtomicReference<Throwable> throwable = new AtomicReference<>();
		AtomicInteger ownership = new AtomicInteger();
		Stopper stopper = new Stopper();
		try {
			// QThread 7 is natively created Qt thread adopted by Java
			QSemaphore semaphore = new QSemaphore();
			QThread thread;
			{
				thread = ThreadFactory.createQWorkerThread(()->{
					try {
						initializeThread(qthreads, jthreads, ownership, running, gcQtThreadObjectsCounter, gcJavaThreadObjectsCounter, destroyedReceiver, finishedReceiver, false);
						QEventLoop loop = new QEventLoop();
						QTimer.singleShot(1, semaphore::release);
						stopper.stop.connect(loop::quit, Qt.ConnectionType.DirectConnection);
						loop.exec();
						loop.dispose();
					}catch(Throwable t) {
						throwable.set(t);
					}					
				});
				Assert.assertTrue("QThread NativeCreatedJavaStartedQWorkerThread has not java ownership", General.internalAccess.isJavaOwnership(thread));
				thread.setObjectName("thread-NativeCreatedJavaStartedQWorkerThread");
				thread.start();
				Assert.assertTrue("QThread NativeCreatedJavaStartedQWorkerThread has not java ownership", General.internalAccess.isJavaOwnership(thread));
			}
			int i=0;
			semaphore.acquire();
			Assert.assertEquals(1, running.get());
			Set<Thread> runningThreads = Thread.getAllStackTraces().keySet();
			{
				Assert.assertTrue(jthreads.get()!=null);
				Assert.assertTrue(runningThreads.contains(jthreads.get()));
				Assert.assertEquals(null, throwable.get());
				throwable.set(null);
				Assert.assertEquals(Thread.State.RUNNABLE, jthreads.get().getState());
				Assert.assertEquals("Java thread NativeCreatedJavaStartedQWorkerThread is not associated to Qt thread", qthreads.get(), QThread.thread(jthreads.get()));
				Assert.assertEquals("Qt thread NativeCreatedJavaStartedQWorkerThread is not associated to Java thread", jthreads.get(), qthreads.get().javaThread());
			}
			Assert.assertEquals(thread, qthreads.get());
			thread = null;
			{
				Assert.assertTrue("QThread NativeCreatedJavaStartedQWorkerThread is null", qthreads.get()!=null);
				Assert.assertEquals("QThread NativeCreatedJavaStartedQWorkerThread has wrong thread-affinity", QThread.currentThread(), qthreads.get().thread());
				Assert.assertFalse("QThread NativeCreatedJavaStartedQWorkerThread is disposed", qthreads.get().isDisposed());
				Assert.assertTrue("QThread NativeCreatedJavaStartedQWorkerThread not alive", qthreads.get().isAlive());
				Assert.assertTrue("QThread NativeCreatedJavaStartedQWorkerThread not running", qthreads.get().isRunning());
				Assert.assertFalse("QThread NativeCreatedJavaStartedQWorkerThread is finished", qthreads.get().isFinished());
				Assert.assertEquals(jthreads.get(), qthreads.get().javaThread());
				Assert.assertFalse("QThread NativeCreatedJavaStartedQWorkerThread has Cpp ownership", General.internalAccess.isCppOwnership(qthreads.get()));
				Assert.assertTrue("running QThread NativeCreatedJavaStartedQWorkerThread has not Java ownership", ownership.get()==1);
			}
			
			stopper.stop.emit();
			stopper.stop.disconnect();
			finishedReceiver.waitForReleased(2000);
			
			i = 0;
			while(i<20) {
				Thread.yield();
				Thread.sleep(150);
				runGC();
				++i;
			}
			{
				try {
					if(qthreads.get().isRunning()) {
						i = 0;
						while(i<20 && qthreads.get().isRunning()) {
							Thread.yield();
							Thread.sleep(150);
							runGC();
							++i;
						}
					}
				}catch(Throwable t){}
			}
			runningThreads = Thread.getAllStackTraces().keySet();
			{
				try {
					if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)
							|| "windows-mingw-x64".equals(TestUtility.osArchName()))
						Assert.assertTrue("post-mortem Java thread NativeCreatedJavaStartedQWorkerThread state==RUNNABLE", Thread.State.RUNNABLE!=jthreads.get().getState());
					else
						Assert.assertEquals("post-mortem Java thread NativeCreatedJavaStartedQWorkerThread state", Thread.State.TERMINATED, jthreads.get().getState());
				} catch (AssertionError e) {
					StackTraceElement[] stackTraces = Thread.getAllStackTraces().get(jthreads.get());
					if(stackTraces!=null) {
						AssertionError error = new AssertionError("Thread stack");
						error.setStackTrace(stackTraces);
						e.addSuppressed(error);
						throw e;
					}else {
						runningThreads = Thread.getAllStackTraces().keySet();
					}
				}
				Assert.assertFalse("post-mortem Java thread NativeCreatedJavaStartedQWorkerThread is still running", runningThreads.contains(jthreads.get()));
				if(!qthreads.get().isDisposed()) {
					Assert.assertEquals("post-mortem Java thread NativeCreatedJavaStartedQWorkerThread is not associated to Qt thread", qthreads.get(), QThread.thread(jthreads.get()));
					Assert.assertEquals("post-mortem Qt thread NativeCreatedJavaStartedQWorkerThread is not associated to Java thread", jthreads.get(), qthreads.get().javaThread());
				}else {
					Assert.assertEquals("post-mortem Java thread NativeCreatedJavaStartedQWorkerThread is still associated to Qt thread", null, QThread.thread(jthreads.get()));
				}
				jthreads.set(null);
			}
			runningThreads = null;
			{
				Assert.assertTrue("QThread NativeCreatedJavaStartedQWorkerThread is null", qthreads.get()!=null);
				Assert.assertFalse("post-mortem QThread NativeCreatedJavaStartedQWorkerThread still alive", qthreads.get().isAlive());
				Assert.assertFalse("post-mortem QThread NativeCreatedJavaStartedQWorkerThread still running", qthreads.get().isRunning());
				Assert.assertTrue("post-mortem QThread NativeCreatedJavaStartedQWorkerThread not finished", qthreads.get().isFinished());
				switch(ownership.get()) {
				case 1:
					Assert.assertTrue("post-mortem QThread NativeCreatedJavaStartedQWorkerThread not Java ownership", General.internalAccess.isJavaOwnership(qthreads.get()));
					break;
				case 2:
					Assert.assertTrue("post-mortem QThread NativeCreatedJavaStartedQWorkerThread not Cpp ownership", General.internalAccess.isCppOwnership(qthreads.get()));
					break;
				case 3:
					Assert.assertTrue("post-mortem QThread NativeCreatedJavaStartedQWorkerThread not split ownership", General.internalAccess.isSplitOwnership(qthreads.get()));
					break;
				}
				qthreads.set(null);
			}
			QEventLoop loop = new QEventLoop();
			{
				QTimer timer = new QTimer();
				long t1 = System.currentTimeMillis();
				timer.timeout.connect(()->{
					Thread.yield();
					Thread.sleep(250);
					runGC();
					QCoreApplication.processEvents();
					QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
					if(finishedReceiver.received() || System.currentTimeMillis()-t1>15000) {
						loop.quit();
					}
				});
				timer.setInterval(300);
				timer.start();
				loop.exec();
				timer.dispose();
			}
			{
				QTimer timer = new QTimer();
				long t1 = System.currentTimeMillis();
				timer.timeout.connect(()->{
					Thread.yield();
					Thread.sleep(250);
					runGC();
					QCoreApplication.processEvents();
					QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
					if(gcQtThreadObjectsCounter.get()==1 || System.currentTimeMillis()-t1>15000) {
						loop.quit();
					}
				});
				timer.setInterval(300);
				timer.start();
				loop.exec();
				timer.dispose();
			}
			{
				QTimer timer = new QTimer();
				long t1 = System.currentTimeMillis();
				timer.timeout.connect(()->{
					Thread.yield();
					Thread.sleep(250);
					runGC();
					QCoreApplication.processEvents();
					QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
					if(gcJavaThreadObjectsCounter.get()==1 || System.currentTimeMillis()-t1>15000) {
						loop.quit();
					}
				});
				timer.setInterval(300);
				timer.start();
				loop.exec();
				timer.dispose();
			}
			if(!QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android))
				Assert.assertEquals("Java thread object NativeCreatedJavaStartedQWorkerThread not GC-removed", 1, gcJavaThreadObjectsCounter.get());
			Assert.assertTrue("NativeCreatedJavaStartedQWorkerThread has not been deleted natively", destroyedReceiver.received());
			Assert.assertEquals("QThread object NativeCreatedJavaStartedQWorkerThread  not GC-removed", 1, gcQtThreadObjectsCounter.get());
		}catch(Error | RuntimeException e) {
			Throwable t = throwable.get();
			if(t!=null) {
				e.addSuppressed(t);
			}
			throw e;
		}			
	}
}
