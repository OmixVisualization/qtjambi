/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.lang.ref.WeakReference;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;

import io.qt.autotests.generated.General;
import io.qt.autotests.generated.SignalReceiver;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QEventLoop;
import io.qt.core.QObject;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QThread;
import io.qt.core.QTimeLine;
import io.qt.core.QTimer;
import io.qt.core.Qt;

public class TestQThread extends ApplicationInitializer{
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitialize();
    }
	
	private QThread qthread1;
	private QThread qthread2;
	private QThread qthread3;
	private QThread qthread4;
	private QThread qthread5;
	private ThreadGroup threads;
	private Runnable r;
	private Runnable stacked;
	private String threadName = "myThread";

	@org.junit.Before
	public void setUp() {
		if(QCoreApplication.instance()==null){
			QCoreApplication.initialize(new String[]{"TestQThread"});
		}
		threads = new ThreadGroup("qtjambiThreads");
		qthread1 = QThread.create(r);
		qthread2 = QThread.create(threads, qthread1 instanceof Runnable ? (Runnable)qthread1 : r);
		qthread3 = QThread.create(r, threadName);
		qthread4 = QThread.create(threads, r, threadName);
		qthread5 = QThread.create(threads, stacked, threadName, 1);
		instances.add(new WeakReference<>(qthread1));
		instances.add(new WeakReference<>(qthread2));
		instances.add(new WeakReference<>(qthread3));
		instances.add(new WeakReference<>(qthread4));
		instances.add(new WeakReference<>(qthread5));
	}

	@org.junit.After
	public void tearDown() {
		qthread1 = null;
		qthread2 = null;
		qthread3 = null;
		qthread4 = null;
		qthread5 = null;
	}

	@org.junit.Test
	public void testGroupConstructor() {
		assertEquals("qthread2.getThreadGroup()", qthread2.getThreadGroup(), threads);
	}

	@org.junit.Test
	public void testNameConstructor() {
		assertEquals("qthread3.getName()", threadName, qthread3.getName());
	}

	@org.junit.Test
	public void testNameGroupConstructor() {
		assertEquals("qthread4.getThreadGroup()", threads, qthread4.getThreadGroup());
		assertEquals("qthread4.getName()", threadName, qthread4.getName());
	}

	/*
	 * @org.junit.Test public void testNameGroupStackSize() { }
	 */

	@org.junit.Test
	public void testRun() {
		Object _qthread1 = qthread1;
		if(_qthread1 instanceof Thread) { // only compiles if QThreads are java Threads
			((Thread)_qthread1).run();
			assertFalse("qthread1.isAlive()", qthread1.isAlive());
		}
	}

	@org.junit.Test
	public void testInit() {
		assertNotNull("qthread1.starting", qthread1.started);
		assertNotNull("qthread1.finished", qthread1.finished);
	}

	@org.junit.Test
	public void testSignalMoveToThread() throws InterruptedException{
		final QObject[] parent = {null};
		final QTimeLine[] timeLine = {null};
		final AtomicBoolean threadFinished = new AtomicBoolean(false);
		final AtomicInteger finished = new AtomicInteger(0);
		final QThread currentThread = QThread.currentThread();
		QThread signalTestThread = QThread.create(()->{
			parent[0] = new QObject();
			parent[0].setObjectName("testSignalMoveToThread-parent");
			timeLine[0] = new QTimeLine();
			timeLine[0].setObjectName("testSignalMoveToThread-child");
			timeLine[0].setParent(parent[0]);
			timeLine[0].finished.connect(finished::incrementAndGet);
			parent[0].moveToThread(currentThread);
			threadFinished.set(true);
			instances.add(new WeakReference<>(timeLine[0]));
			instances.add(new WeakReference<>(parent[0]));
		});
		signalTestThread.setDaemon(true);
		signalTestThread.setName("QThread-Test");
		signalTestThread.start();
		assertTrue(signalTestThread.isAlive());
		signalTestThread.join(20000);
		QCoreApplication.processEvents(); // hopefully reconnecting signal wrapper
		assertNotNull("parent[0]", parent[0]);
		assertNotNull("timeLine[0]", timeLine[0]);
		assertTrue("threadFinished", threadFinished.get());
		assertEquals(currentThread, parent[0].thread());
		assertEquals(currentThread, timeLine[0].thread());
		assertEquals(parent[0], timeLine[0].parent());
		timeLine[0].setLoopCount(1);
		timeLine[0].setDuration(1);
		timeLine[0].start();
		while(timeLine[0].state()==QTimeLine.State.Running){
			QCoreApplication.processEvents();
		}
		assertEquals("sending finished signal through threads", 1, finished.get());
		signalTestThread.dispose();
	}
	
	@org.junit.Test
	public void testEventThread() throws InterruptedException{
		int eventype = QEvent.registerEventType();
		final AtomicBoolean reached = new AtomicBoolean(false);
		QObject object = new QObject(){
			public boolean event(QEvent event){
				if(event.type().value()==eventype){
					reached.set(true);
					event.accept();
					((QThread)QThread.currentThread()).quit();
					dispose();
					return true;
				}
				return super.event(event);
			}
		};
		final AtomicInteger finished = new AtomicInteger(0);
		QThread thread = new QThread();
		thread.setDaemon(true);
		thread.setName("QThread-Test");
		thread.finished.connect(finished::incrementAndGet);
		thread.start();
		object.moveToThread(thread);
		Thread.sleep(200);
		assertTrue(thread.isAlive());
		QCoreApplication.postEvent(object, new QEvent(QEvent.Type.resolve(eventype)));
		thread.join(2000);
		assertTrue("sending event through threads", reached.get());
		assertEquals("sending QThread finished signal after thread finish", 1, finished.get());
		thread.dispose();
	}
	
	private static class TestObject extends QObject{
		AtomicInteger ai = new AtomicInteger(0);

		public final int get() {
			return ai.get();
		}

		public final int incrementAndGet() {
			return ai.incrementAndGet();
		}
	}
	
	@org.junit.Test
	public void testThreadSignalsToQObject() throws Throwable{
		TestObject finished = new TestObject();
		TestObject started = new TestObject();
		TestObject running = new TestObject();
		Throwable[] throwable = {null};
		Thread jthread = new Thread(()->{
			try {
				QThread.currentThread().started.connect(started::incrementAndGet);
				QThread.currentThread().finished.connect(finished::incrementAndGet);
				running.incrementAndGet();
			}catch(Throwable t) {
				throwable[0] = t;
			}
		});
		jthread.setName("testThreadSignalsToQObject");
		jthread.setDaemon(true);
		jthread.start();
		jthread.join(2000);
		if(throwable[0]!=null)
			throw throwable[0];
		assertEquals("started counter", 0, started.get());
		assertEquals("running counter", 1, running.get());
		assertEquals("finished counter", 0, finished.get());
		long t = System.currentTimeMillis();
		while(finished.get()==0) {
			QCoreApplication.processEvents();
			Thread.sleep(50);
			if(t+15000<System.currentTimeMillis())
				break;
		}
		assertEquals("started counter", 0, started.get());
		assertEquals("running counter", 1, running.get());
		assertEquals("finished counter", 1, finished.get());
		instances.add(new WeakReference<>(QThread.thread(jthread)));
		instances.add(new WeakReference<>(finished));
		instances.add(new WeakReference<>(started));
		instances.add(new WeakReference<>(running));
	}
	
	@org.junit.Test
	public void testQThreadSignals() throws InterruptedException{
		AtomicInteger finished = new AtomicInteger(0);
		AtomicInteger started = new AtomicInteger(0);
		AtomicInteger running = new AtomicInteger(0);
		QThread jthread = QThread.create(running::incrementAndGet);
		jthread.started.connect(started::incrementAndGet);
		jthread.finished.connect(finished::incrementAndGet);
		jthread.start();
		jthread.join(2000);
		int i=0;
		while(finished.get()==0 && i<10000) {
			Thread.sleep(50);
			++i;
		}
		assertEquals("started counter", 1, started.get());
		assertEquals("running counter", 1, running.get());
		assertEquals("finished counter", 1, finished.get());
		instances.add(new WeakReference<>(jthread));
	}
	
	@org.junit.Test
//	@SuppressWarnings("deprecation")
	public void testQThreadSignalsToQObject() throws InterruptedException{
		TestObject finished = new TestObject();
		TestObject started = new TestObject();
		TestObject running = new TestObject();
		QThread jthread = QThread.create(running::incrementAndGet);
		jthread.started.connect(started::incrementAndGet);
		jthread.finished.connect(finished::incrementAndGet);
		jthread.start();
		jthread.join(2000);
		assertEquals("started counter", 0, started.get());
		assertEquals("running counter", 1, running.get());
		assertEquals("finished counter", 0, finished.get());
		int i=0;
		while(finished.get()==0 && i<10000) {
			Thread.sleep(50);
			++i;
			QCoreApplication.processEvents();
		}
		assertEquals("started counter", 1, started.get());
		assertEquals("running counter", 1, running.get());
		assertEquals("finished counter", 1, finished.get());
		instances.add(new WeakReference<>(jthread));
		instances.add(new WeakReference<>(finished));
		instances.add(new WeakReference<>(started));
		instances.add(new WeakReference<>(running));
	}
	
	@org.junit.Test
	public void testQThreadJavaInterrupt() throws InterruptedException, ClassNotFoundException{
		Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
		AtomicBoolean continued = new AtomicBoolean();
		AtomicBoolean interrupted = new AtomicBoolean();
		AtomicBoolean interruptionRequested = new AtomicBoolean();
		QThread thread = QThread.create(()->{
			try {
				synchronized (TestQThread.class) {
					TestQThread.class.wait();
				}
				continued.set(true);
			} catch (InterruptedException e) {
				interrupted.set(true);
			}finally {
				try {
					Thread.sleep(10);
				} catch (InterruptedException e) {
				}
				interruptionRequested.set(QThread.currentThread().isInterruptionRequested());
			}
		});
		try {
			thread.start();
			Thread.sleep(100);
			thread.join(100);
			assertTrue(thread.isRunning());
			assertFalse(thread.isFinished());
			assertTrue(thread.javaThread()!=null);
			thread.javaThread().interrupt();
//			thread.requestInterruption();
			thread.join(2000);
			assertTrue(interrupted.get());
			assertFalse(continued.get());
			assertTrue("not interruptionRequested", interruptionRequested.get());
		}finally {
			synchronized(TestQThread.class) {
				TestQThread.class.notifyAll();
			}
			instances.add(new WeakReference<>(thread));
		}
	}
	
	@org.junit.Test
	public void testQThreadRequestInterruption() throws InterruptedException, ClassNotFoundException{
		AtomicBoolean continued = new AtomicBoolean();
		AtomicBoolean done = new AtomicBoolean();
		AtomicBoolean interrupted = new AtomicBoolean();
		AtomicBoolean interruptionRequested = new AtomicBoolean();
		QThread thread = QThread.create(()->{
			try {
				synchronized (TestQThread.class) {
					TestQThread.class.wait();
				}
				continued.set(true);
			} catch (InterruptedException e) {
				interrupted.set(true);
			}finally {
				try {
					Thread.sleep(10);
				} catch (InterruptedException e) {
				}finally {
					interruptionRequested.set(QThread.currentThread().isInterruptionRequested());
					done.set(true);
				}
			}
		});
		try {
			thread.start();
			Thread jthread;
			do {
				Thread.yield();
				Thread.sleep(100);
				jthread = thread.javaThread();
			}while(jthread==null || jthread.getState()!=Thread.State.WAITING);
			thread.requestInterruption();
			thread.join(2000);
			assertTrue(done.get());
			assertFalse(continued.get());
			assertTrue(interruptionRequested.get());
			assertTrue(interrupted.get());
		}finally {
			synchronized(TestQThread.class) {
				TestQThread.class.notifyAll();
			}
			instances.add(new WeakReference<>(thread));
		}
	}
	
	@org.junit.Test
	public void testThreadJavaInterrupt() throws InterruptedException, ClassNotFoundException{
		Assume.assumeFalse("Cannot install interruptor on Android.", QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
		AtomicBoolean continued = new AtomicBoolean();
		AtomicBoolean interrupted = new AtomicBoolean();
		AtomicBoolean interruptionRequested = new AtomicBoolean();
		Thread thread = new Thread(()->{
			QThread.currentThread();
			try {
				synchronized (TestQThread.class) {
					TestQThread.class.wait();
				}
				continued.set(true);
			} catch (InterruptedException e) {
				interrupted.set(true);
			}finally {
				try {
					Thread.sleep(10);
				} catch (InterruptedException e) {
				}
				interruptionRequested.set(QThread.currentThread().isInterruptionRequested());
			}
		});
		try {
			thread.start();
			Thread.sleep(500);
			assertTrue(thread.isAlive());
			while(QThread.thread(thread)==null) {
				Thread.sleep(100);
			}
			thread.interrupt();
			thread.join(2000);
			assertTrue(interrupted.get());
			assertFalse(continued.get());
			assertTrue("is interruptionRequested", interruptionRequested.get());
		}finally {
			synchronized(TestQThread.class) {
				TestQThread.class.notifyAll();
			}
			instances.add(new WeakReference<>(QThread.thread(thread)));
		}
	}
	
	@org.junit.Test
	public void testThreadRequestInterruption() throws InterruptedException, ClassNotFoundException{
		AtomicBoolean continued = new AtomicBoolean();
		AtomicBoolean interrupted = new AtomicBoolean();
		AtomicBoolean interruptionRequested = new AtomicBoolean();
		Thread thread = new Thread(()->{
			QThread.currentThread();
			try {
				synchronized (TestQThread.class) {
					TestQThread.class.wait();
				}
				continued.set(true);
			} catch (InterruptedException e) {
				interrupted.set(true);
			}finally {
				try {
					Thread.sleep(10);
				} catch (InterruptedException e) {
				}
				interruptionRequested.set(QThread.currentThread().isInterruptionRequested());
			}
		});
		try {
			thread.start();
			Thread.sleep(100);
			assertTrue(thread.isAlive());
			while(QThread.thread(thread)==null) {
				Thread.sleep(100);
			}
			QThread.thread(thread).requestInterruption();
			thread.join(2000);
			assertTrue(interrupted.get());
			assertFalse(continued.get());
			assertTrue(interruptionRequested.get());
		}finally {
			synchronized(TestQThread.class) {
				TestQThread.class.notifyAll();
			}
			instances.add(new WeakReference<>(QThread.thread(thread)));
		}
	}
	
	@org.junit.Test
	public void testDeleteTopLevelRunnerThread() throws Throwable {
		WeakReference<QThread> reference;
		WeakReference<Thread> jreference;
		{
			AtomicBoolean finished = new AtomicBoolean();
			boolean[] javaOwnership = {false};
			Thread[] javaThreads = {null,null};
			QThread[] qtarray = {null};
			Throwable[] exception = {null};
			QThread thread = QThread.create(()->{
				Thread currentJThread = Thread.currentThread();
				QThread currentThread = QThread.currentThread();
				Thread javaThread = null;
				try {
					qtarray[0] = currentThread;
					javaOwnership[0] = General.internalAccess.isJavaOwnership(currentThread);
					javaThreads[0] = currentJThread;
					javaThread = currentThread.javaThread();
					javaThreads[1] = javaThread;
				}catch(Throwable t) {
					exception[0] = t;
				}finally {
					currentThread = null;
					currentJThread = null;
					javaThread = null;
				}
			});
			thread.finished.connect(()->finished.set(true));
			reference = new WeakReference<>(thread);
			thread.start();
			do {
				Thread.yield();
				Thread.sleep(100);
			}while(!finished.get());
			if(exception[0]!=null)
				throw exception[0];
			Assert.assertTrue(javaThreads[0]!=null);
			Assert.assertEquals(javaThreads[0], javaThreads[1]);
			jreference = new WeakReference<>(javaThreads[0]);
			Assert.assertEquals(thread, qtarray[0]);
			Assert.assertTrue(javaOwnership[0]);
			Assert.assertTrue(General.internalAccess.isJavaOwnership(thread));
			Assert.assertTrue(finished.get());
			Assert.assertFalse(thread.isAlive());
			if(javaThreads[0].getState()!=Thread.State.TERMINATED) {
				javaThreads[0].interrupt();
				int i = 0;
				while(i<20 && javaThreads[0].getState()!=Thread.State.TERMINATED) {
					Thread.yield();
					Thread.sleep(150);
					runGC();
					++i;
				}
			}
			javaThreads[0] = null;
			javaThreads[1] = null;
			qtarray[0] = null;
			thread = null;
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
				if(reference.get()==null || System.currentTimeMillis()-t1>20000) {
					loop.quit();
				}
			});
			timer.setInterval(300);
			timer.start();
			loop.exec();
			timer.dispose();
		}
		QThread remaining = reference.get();
		if(remaining!=null && !remaining.isDisposed()) {
			Assert.assertEquals("QThread reference is not null and existing with parent", null, remaining.parent());
			Assert.assertTrue("QThread reference is not null and existing with cpp ownership", General.internalAccess.isJavaOwnership(remaining));
			Assert.fail("QThread reference is not null and existing");
		}
		Assert.assertTrue("QThread reference is not null but disposed", null==remaining);
		Assert.assertTrue("Thread reference is not", null==jreference.get());
	}
	
	@org.junit.Test
	public void testDeleteChildRunnerThread() throws Throwable {
		QObject parent = new QObject();
		WeakReference<QThread> reference;
		WeakReference<Thread> jreference;
		{
			AtomicBoolean finished = new AtomicBoolean();
			boolean[] cppOwnership = {false};
			Thread[] javaThreads = {null,null};
			QThread[] qtarray = {null};
			Throwable[] exception = {null};
			QThread thread = QThread.create(()->{
				QThread currentThread = QThread.currentThread();
				try {
					qtarray[0] = currentThread;
					cppOwnership[0] = General.internalAccess.isCppOwnership(currentThread);
					javaThreads[0] = Thread.currentThread();
					javaThreads[1] = currentThread.javaThread();
				}catch(Throwable t) {
					exception[0] = t;
				}finally {
					currentThread = null;
				}
			}, parent);
			thread.setDaemon(true);
			reference = new WeakReference<>(thread);
			thread.finished.connect(()->finished.set(true));
			thread.start();
			thread.join(2000);
			if(exception[0]!=null)
				throw exception[0];
			Assert.assertTrue(javaThreads[0]!=null);
			Assert.assertTrue("not a daemon", javaThreads[0].isDaemon());
			Assert.assertEquals(javaThreads[0], javaThreads[1]);
			jreference = new WeakReference<>(javaThreads[0]);
			Assert.assertEquals(thread, qtarray[0]);
			Assert.assertTrue(cppOwnership[0]);
			Assert.assertTrue(General.internalAccess.isCppOwnership(thread));
			Assert.assertTrue(finished.get());
			Assert.assertFalse(thread.isAlive());
			if(javaThreads[0].getState()!=Thread.State.TERMINATED) {
				javaThreads[0].interrupt();
				int i = 0;
				while(i<20 && javaThreads[0].getState()!=Thread.State.TERMINATED) {
					Thread.yield();
					Thread.sleep(150);
					runGC();
					++i;
				}
			}
			javaThreads[0] = null;
			javaThreads[1] = null;
			qtarray[0] = null;
			thread = null;
		}
		ApplicationInitializer.runGC();
		Assert.assertTrue(reference.get()!=null);
		Assert.assertEquals(parent, reference.get().parent());
		parent = null;
		QEventLoop loop = new QEventLoop();
		{
			QTimer timer = new QTimer();
			long t1 = System.currentTimeMillis();
			timer.timeout.connect(()->{
				runGC();
				if(reference.get()==null || System.currentTimeMillis()-t1>20000) {
					loop.quit();
				}
			});
			timer.setInterval(300);
			timer.start();
			loop.exec();
			timer.dispose();
		}
		QThread remaining = reference.get();
		Assert.assertTrue("QThread reference is not null and existing", remaining==null || remaining.isDisposed());
		Assert.assertTrue("QThread reference is not null but disposed", null==remaining);
		Assert.assertTrue("Thread reference is not", null==jreference.get());
	}
	
	@org.junit.Test
	public void testDeleteTopLevelEventThread() throws Throwable {
		WeakReference<QThread> reference;
		WeakReference<Thread> jreference;
		AtomicBoolean finished = new AtomicBoolean();
		{
			boolean[] javaOwnership = {false};
			Thread[] javaThreads = {null,null};
			QThread[] qtarray = {null};
			Throwable[] exception = {null};
			QObject obj = new QObject() {
				@Override
				public boolean event(QEvent event) {
					if(event.type()==QEvent.Type.DeferredDispose) {
						QThread currentThread = QThread.currentThread();
						try {
							qtarray[0] = currentThread;
							javaOwnership[0] = General.internalAccess.isJavaOwnership(currentThread);
							javaThreads[0] = Thread.currentThread();
							javaThreads[1] = currentThread.javaThread();
						}catch(Throwable t) {
							exception[0] = t;
						}finally {
							currentThread.exit();
							currentThread = null;
						}
					}
					return super.event(event);
				}
			};
			QThread thread = new QThread();
			reference = new WeakReference<>(thread);
			obj.moveToThread(thread);
			obj.disposeLater();
			thread.finished.connect(()->finished.set(true));
			thread.start();
			thread.join(2000);
			if(exception[0]!=null)
				throw exception[0];
			int i = 0;
			while(i<20) {
				Thread.yield();
				Thread.sleep(150);
				runGC();
				++i;
			}
			Assert.assertTrue(javaThreads[0]!=null);
			Assert.assertEquals(javaThreads[0], javaThreads[1]);
			jreference = new WeakReference<>(javaThreads[0]);
			Assert.assertEquals(thread, qtarray[0]);
			Assert.assertTrue(javaOwnership[0]);
			Assert.assertEquals(null, thread.parent());
			Assert.assertTrue(General.internalAccess.isJavaOwnership(thread));
			Assert.assertTrue(finished.get());
			Assert.assertFalse(thread.isAlive());
			if(javaThreads[0].getState()!=Thread.State.TERMINATED)
				javaThreads[0].interrupt();
			javaThreads[0] = null;
			javaThreads[1] = null;
			qtarray[0] = null;
			thread = null;
			obj = null;
		}
		QEventLoop loop = new QEventLoop();
		{
			QTimer timer = new QTimer();
			long t1 = System.currentTimeMillis();
			timer.timeout.connect(()->{
				runGC();
				if(reference.get()==null || System.currentTimeMillis()-t1>20000) {
					loop.quit();
				}
			});
			timer.setInterval(300);
			timer.start();
			loop.exec();
			timer.dispose();
		}
		QThread remaining = reference.get();
		Assert.assertTrue("QThread reference is not null and existing", remaining==null || remaining.isDisposed());
		Assert.assertTrue("QThread reference is not null but disposed", null==remaining);
		Assert.assertTrue("Thread reference is not", null==jreference.get());
	}
	
	@org.junit.Test
	public void testDeleteChildEventThread() throws Throwable {
		WeakReference<QThread> reference;
		WeakReference<Thread> jreference;
		{
			QObject parent = new QObject();
			{
				boolean[] cppOwnership = {false};
				Thread[] javaThreads = {null,null};
				QThread[] qtarray = {null};
				Throwable[] exception = {null};
				QObject obj = new QObject() {
					@Override
					public boolean event(QEvent event) {
						if(event.type()==QEvent.Type.DeferredDispose) {
							QThread currentThread = QThread.currentThread();
							try {
								qtarray[0] = currentThread;
								cppOwnership[0] = General.internalAccess.isCppOwnership(currentThread);
								javaThreads[0] = Thread.currentThread();
								javaThreads[1] = currentThread.javaThread();
							}catch(Throwable t) {
								exception[0] = t;
							}finally {
								currentThread.exit();
								currentThread = null;
							}
						}
						return super.event(event);
					}
				};
				QThread thread = new QThread(parent);
				reference = new WeakReference<>(thread);
				obj.moveToThread(thread);
				obj.disposeLater();
				thread.start();
				thread.join(2000);
				if(exception[0]!=null) {
					qtarray[0] = null;
					thread = null;
					throw exception[0];
				}
				Assert.assertTrue(javaThreads[0]!=null);
				Assert.assertEquals(javaThreads[0], javaThreads[1]);
				jreference = new WeakReference<>(javaThreads[0]);
				Assert.assertEquals(thread, qtarray[0]);
				Assert.assertTrue(cppOwnership[0]);
				Assert.assertTrue(General.internalAccess.isCppOwnership(thread));
				javaThreads[0] = null;
				javaThreads[1] = null;
				qtarray[0] = null;
				thread = null;
				obj = null;
			}
			ApplicationInitializer.runGC();
			Assert.assertTrue(reference.get()!=null);
			Assert.assertEquals(parent, reference.get().parent());
			parent = null;
		}
		QEventLoop loop = new QEventLoop();
		{
			QTimer timer = new QTimer();
			long t1 = System.currentTimeMillis();
			timer.timeout.connect(()->{
				runGC();
				if(reference.get()==null || System.currentTimeMillis()-t1>20000) {
					loop.quit();
				}
			});
			timer.setInterval(300);
			timer.start();
			loop.exec();
			timer.dispose();
		}
		Thread javaThread = jreference.get();
		if(javaThread!=null && javaThread.getState()!=Thread.State.TERMINATED) {
			javaThread.interrupt();
			javaThread = null;
			int i = 0;
			do{
				Thread.yield();
				Thread.sleep(150);
				runGC();
				++i;
				javaThread = jreference.get();
			}while(i<20 && javaThread!=null && javaThread.getState()!=Thread.State.TERMINATED);
		}
		QThread remaining = reference.get();
		Assert.assertTrue("QThread reference is not null and existing", remaining==null || remaining.isDisposed());
		Assert.assertTrue("QThread reference is not null but disposed", null==remaining);
		Assert.assertTrue("Thread reference is not null", null==jreference.get());
	}
	
	@org.junit.Test
	public void testDeleteAdoptedThread() throws Throwable {
		SignalReceiver finished = new SignalReceiver();
		AtomicBoolean threadCleaned = new AtomicBoolean();
		AtomicBoolean qthreadCleaned = new AtomicBoolean();
		{
			boolean[] splitOwnership = {false};
			Thread[] javaThreads = {null,null};
			QThread[] qtarray = {null};
			Throwable[] exception = {null};
			Thread thread = new Thread(()->{
				QThread currentThread = QThread.currentThread();
				currentThread.finished.connect(finished, "receiveSignal()", Qt.ConnectionType.DirectConnection);
				try {
					qtarray[0] = currentThread;
					splitOwnership[0] = General.internalAccess.isSplitOwnership(currentThread);
					javaThreads[0] = Thread.currentThread();
					javaThreads[1] = currentThread.javaThread();
					General.internalAccess.registerCleaner(currentThread, ()->qthreadCleaned.set(true));
				}catch(Throwable t) {
					exception[0] = t;
				}finally {
					currentThread.exit();
					currentThread = null;
				}
				System.out.println("Adopted Thread about to finish...");
			});
			General.internalAccess.registerCleaner(thread, ()->threadCleaned.set(true));
			thread.start();
			thread.join(2000);
			if(exception[0]!=null)
				throw exception[0];
			Assert.assertEquals(thread, javaThreads[0]);
			Assert.assertEquals(javaThreads[0], javaThreads[1]);
			Assert.assertTrue(splitOwnership[0]);
			Assert.assertFalse("QThread is null", qtarray[0]==null);
			for (int i = 0; i < 120 && !finished.received(); i++) {
				Thread.yield();
				Thread.sleep(100);
			}
			Assert.assertTrue("Thread termintated without emitting QThread::finished", finished.received());
//			Assert.assertTrue("QThread is disposed", qtarray[0].isDisposed());
			javaThreads[0] = null;
			javaThreads[1] = null;
			qtarray[0] = null;
			thread = null;
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
				if(qthreadCleaned.get() || System.currentTimeMillis()-t1>20000) {
					loop.quit();
				}
			});
			timer.setInterval(300);
			timer.start();
			loop.exec();
			timer.dispose();
		}
		Assert.assertTrue("Thread has not been deleted", threadCleaned.get());
		Assert.assertTrue("QThread has not been deleted", qthreadCleaned.get());
	}
}
