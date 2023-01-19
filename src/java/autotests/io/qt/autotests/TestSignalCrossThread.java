/****************************************************************************
**
** Copyright (C) 2011 Darryl L. Miles.  All rights reserved.
** Copyright (C) 2011 D L Miles Consulting Ltd.  All rights reserved.
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
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package io.qt.autotests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.lang.Thread.UncaughtExceptionHandler;
import java.util.HashMap;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.Test;

import io.qt.QNoNativeResourcesException;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QEventLoop;
import io.qt.core.QMetaObject;
import io.qt.core.QObject;
import io.qt.core.QThread;
import io.qt.core.QTimer;
import io.qt.core.Qt;

// This testcase attempts to validate that the APIs allow and emit/deliver
//  signals across threads correctly.
//
// This testcase (with the old broken code) used to throw a thread affinity exc
//  QThreadAffinityException when it tried to call:
//  QSignalEmitterInternal.this.signalsBlocked()
//  from QSignalEmitterInternal.emit_helper().  This check correctly checks access
//  to the #signalsBlocked() method.  However this inhibited sending signals auto
//  AutoConnection/QueuedConnection to marshal across threads.
// Now it is possible to use -Dio.qt.thread-check=false to stop the
//  thread affinity checking and this caused direct connections setup between two
//  different QObject with different thread affinity to be delivered.
//
// So the solution is to:
//   Not use the signalsBlocked() API call until the Java<>Java delivery phase (after
//    it has confirm same thread affinity of receiver and current thread)
//   Correctly detect when a cross-thread situation exist and cause it to marshal via
//    QMetaEvent.
//   Detect that you have attempted to setup a DirectConnection type but at the time of
//    emit the thread affinity is not correct (you called emit from the wrong thread)
//    and generate a warning.
//
// This testcase was a bit of a sandbox to investigate the problem and provide a
//  testcase for the future.
//
// Improvements/TODO:
//   Use a single MyNotifier (instead of 2, but I named them now)
//   Test scenarios where the opposite side setup the Connection with connect()
//   Remove the need for runOut() and runIn() as we have ability to on demand setup
//    QEventLoop in runOut() now.  So merge these 2 methods.
//  Verify the QMessage output for attempting DirectConnection and not be able to.
public class TestSignalCrossThread extends ApplicationInitializer implements UncaughtExceptionHandler {

	private Thread mainThread;
	
	@AfterClass
    public static void afterClass() throws Exception {
    	testDispose();
    }

	@Before
	public void setUp() {
		mainThread = Thread.currentThread();
	}

	@After
	public void tearDown() {
	}

	// /////////////////////////////////////////////////
	// Tests start

	// Source implementation
	static class MyImpl extends QObject implements Runnable {
		public QMetaObject.AbstractPublicSignal1<Object> signalReceiver;

		public Object dataObject;
		private Notifiable notifyObject;

		private boolean emit;
		private boolean processEvents;
		private boolean shutdown;
		private boolean slotInvoked;
		private Qt.ConnectionType connectionType;

		public final Signal1<Object> signal = new Signal1<Object>();
		private QEventLoop eventLoop;

		public MyImpl(Notifiable notifyObject, Signal1<Object> signalReceiver, Qt.ConnectionType connectionType) {
			this.notifyObject = notifyObject;
			this.signalReceiver = signalReceiver;
			this.connectionType = connectionType;
		}

		public void run() {
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "run() start " + QThread.currentThread() + " " + this.thread());
			if(signalReceiver == null)
				runIn();
			else
				runOut();
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "run() finish " + QThread.currentThread() + " " + this.thread());
			// object must be deleted before thread is deleted!
			dispose();
		}
		private void runIn() {
			// create and connect up from inside the thread
			if(signalReceiver == null) {
				//signal = new Signal1<Object>();
				if(TestConnections.hasSerializableLambdas)
					signal.connect(this::testThreadCallback, connectionType);
				else
					signal.connect(this, "testThreadCallback(Object)", connectionType);
				signalReceiver = signal;
				java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "runIn(connect testThreadCallback(Object)) " + QThread.currentThread() + " " + this.thread());
			}

			eventLoop = new QEventLoop();
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "runIn(eventLoop.parent=" + eventLoop.parent() + ") " + QThread.currentThread() + " " + this.thread());
			if(notifyObject != null)
				notifyObject.doNotify(1);
			while(true) {
				synchronized (this) {
					if(shutdown)
						break;
				}
				eventLoop.processEvents();
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
				synchronized (this) {
					if(shutdown)
						break;
					if(notifyObject != null)
						notifyObject.doNotify(2);
					try {
						wait();
					} catch(InterruptedException e) {
						e.printStackTrace();
					}
				}
			}
			eventLoop = null;
		}
		private void runOut() {
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "runOut(eventLoop=" + eventLoop + ") " + Thread.currentThread() + " " + this.thread());
			if(notifyObject != null)
				notifyObject.doNotify(1);
			while(true) {
				boolean doEmit = false;
				boolean doProcessEvents = false;
				synchronized (this) {
					if(shutdown)
						break;
					if(emit) {
						emit = false;
						doEmit = true;
					}
					if(processEvents) {
						processEvents = false;
						doProcessEvents = true;
					}
				}
				if(doEmit) {
					java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "emit(" + dataObject + ") from subthread " + Thread.currentThread() + " " + this.thread());
					signalReceiver.emit(dataObject);
					if(notifyObject != null)
						notifyObject.doNotify(4);
				}
				if(doProcessEvents) {
					java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "processEvents() from subthread " + Thread.currentThread() + " " + this.thread());
					if(eventLoop == null)
						eventLoop = new QEventLoop();
					eventLoop.processEvents();
					QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
					if(notifyObject != null)
						notifyObject.doNotify(6);
				}
				synchronized (this) {
					if(shutdown)
						break;
					if(notifyObject != null)
						notifyObject.doNotify(2);
					if(!emit && !processEvents) {
						java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "runOut() SLEEP " + Thread.currentThread() + " " + this.thread());
						try {
							wait();
						} catch (InterruptedException e) {
							e.printStackTrace();
							break;
						}
						java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "runOut() WAKE " + Thread.currentThread() + " " + this.thread());
					}
				}
			}
			eventLoop = null;
		}
		public void signalShutdown() {
			QEventLoop tmpEventLoop;
			synchronized (this) {
				shutdown = true;
				tmpEventLoop = eventLoop;  // take reference before it is nulled
				notifyAll();
			}
			if(tmpEventLoop != null) {
				String myThread = null;
				try {
					myThread = " "+this.thread();
				} catch (QNoNativeResourcesException e) {
					myThread = " object disposed";
				}
				java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "signalShutdown(thread=" + tmpEventLoop.thread() + ") " + Thread.currentThread() + myThread);
				java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "signalShutdown(" + tmpEventLoop + ") " + Thread.currentThread() + myThread);
				// postEvent() does not work, maybe because there is no parent/child relationship
				//  between QEventLoops.
				//QCoreApplication.postEvent(tmpEventLoop, new QEvent(Type.Quit));
				// Part of the point of this testcase was to highlight the problem with
				// cross-thread signal/slot and invoking this eventLoop.quit(); method
				// triggers the bug that is not hopefully fixed.  So I opted to use
				// QEvent::Quit instead.
				try {
					tmpEventLoop.quit();  // thread unsafe but works (if you remove the thread guard)
				} catch(Exception e) {
					//e.printStackTrace();
				}
				// QMetaObject.invokeMethod(tmpEventLoop, "quit", Qt.ConnectionType.QueuedConnection);
			}
		}
		public void signalEmit(Object dataObject) {
			synchronized (this) {
				emit = true;
				this.dataObject = dataObject;
				notifyAll();
			}
			if(notifyObject != null)
				notifyObject.doNotify(3);
		}
		public void signalProcessEvents() {
			synchronized (this) {
				processEvents = true;
				notifyAll();
			}
			if(notifyObject != null)
				notifyObject.doNotify(5);
		}

		protected void testThreadCallback(Object o) {
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testThreadCallback(" + o + ") " + Thread.currentThread() + " " + this.thread());
			synchronized (this) {
				this.dataObject = o;
				this.slotInvoked = true;
				notifyAll();
			}
			if(notifyObject != null)
				notifyObject.doNotify(7);
		}
		protected void reset() {
			synchronized (this) {
				dataObject = null;
				slotInvoked = false;
			}
		}
		protected boolean isSlotInvoked() {
			synchronized (this) {
				return slotInvoked;
			}
		}
		protected boolean isResultExactly(Object o) {
			synchronized (this) {
				if(dataObject == o)
					return true;
				return false;
			}
		}
	}
	static class MyReceiver extends QObject {
		public final Signal1<Object> signal = new Signal1<Object>();
		private Object result;
		private Notifiable notifyObject;
		protected MyReceiver(Notifiable notifyObject) {
			this.notifyObject = notifyObject;
		}
		public void connect(Qt.ConnectionType connectionType) {
			if(TestConnections.hasSerializableLambdas)
				signal.connect(this::recvThreadCallback, connectionType);
			else
				signal.connect(this, "recvThreadCallback(Object)", connectionType);
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "MyRecever() connect recvThreadCallback(Object)" + Thread.currentThread() + " " + this.thread());
		}
		protected void recvThreadCallback(Object o) {  // renamed to avoice confusion
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "recvThreadCallback(" + o + ") " + Thread.currentThread() + " " + this.thread());
			synchronized (this) {
				this.result = o;
				notifyAll();
			}
			if(notifyObject != null)
				notifyObject.doNotify(8);
		}
		protected void reset() {
			synchronized (this) {
				result = null;
			}
		}
		protected boolean isResultExactly(Object o) {
			synchronized (this) {
				if(result == o)
					return true;
				return false;
			}
		}
	}

	interface Notifiable {
		void doNotify(int kind);
	}
	static class MyNotifiable implements Notifiable {
		private String name;
		public MyNotifiable(String name) {
			this.name = name;
		}
		public String toString() {
			return name;
		}
		// 1 = thread started and ready
		// 2 = thread polled state and entering wait (can fire multiple times)
		// 3 = request emit()
		// 4 = did emit()
		// 5 = request processEvents()
		// 6 = did processEvents()
		// 7 = completed/inside callback: testThreadCallback
		// 8 = completed/inside callback: recvThreadCallback
		private int didKind = 0;
		public void doNotify(int kind) {
			synchronized (this) {
				java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "doNotify(" + kind + "; change=" + ((didKind < kind) ? "true" : "false") + "; on=" + this + ") " + Thread.currentThread());
				if(didKind < kind)
					didKind = kind;
				notifyAll();
			}
		}
		protected void doNotiftyReset() {
			synchronized (this) {
				didKind = 0;
			}
		}
		public int getNotified() {
			synchronized (this) {
				return didKind;
			}
		}
		protected boolean isNotified(int kind) {
			synchronized (this) {
				java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "isNotified(at=" + didKind + "; want=" + kind + "; on=" + this + ") " + Thread.currentThread());
				if(didKind >= kind)
					return true;
			}
			return false;
		}
		public boolean waitForNotify(long timeout, int kind) {
			long millis = System.currentTimeMillis();
			long expiry = millis + timeout;
			while(true) {
				long left = expiry - System.currentTimeMillis();
				if(left <= 0)
					break;
				synchronized (this) {
					if(didKind >= kind)
						return true;
					try {
						wait(left);  // we wake when delivery was performed
					} catch (InterruptedException e) {
						StringWriter writer = new StringWriter();
						e.printStackTrace(new PrintWriter(writer));
						assertTrue(writer.toString(), false);
					}
					if(didKind >= kind)
						return true;
				}
			}
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "waitForNotify(is=" + didKind + "; want=" + kind + "; on=" + this + ") EXPIRED after " + timeout + " " + Thread.currentThread());
			return false;
		}
	}

	private void helperThreadOut(Qt.ConnectionType connectionType) {
		try {
			MyNotifiable myRecvNotifiable = new MyNotifiable("myRecvNotifiable");
			MyNotifiable mySendNotifiable = new MyNotifiable("mySendNotifiable");
	
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "helperThreadOut() connectionType=" + connectionType + "; " + Thread.currentThread());
	
			// we test emitting out from another thread into callback on this thread
			// myReceiver has thread affinity with this testcase thread
			MyReceiver myReceiver = new MyReceiver(myRecvNotifiable);
			myReceiver.connect(connectionType);
			assertTrue(myReceiver.isResultExactly(null));
	
			// myImpl has thread affinity with the new thread being spawned
			MyImpl myImpl = new MyImpl(mySendNotifiable, myReceiver.signal, connectionType);
			Thread thread = new Thread(myImpl);
			thread.setName("subThreadOut");
			thread.setUncaughtExceptionHandler(this);
//			myImpl.moveToThread(thread);  // not required
	
			helperImplThreadOut(thread::start, myReceiver, myImpl, myRecvNotifiable, mySendNotifiable, connectionType);
	
			assertEquals("caught exception", null, uncaughtExceptions.get(thread));
			assertTrue("thread is not alive: "+thread.getState(), thread.isAlive());
			myImpl.signalShutdown();
			QEventLoop loop = new QEventLoop();
			QTimer.singleShot(2000, loop::quit);
			loop.exec();
			thread.join(1000);
			assertEquals("caught exception", null, uncaughtExceptions.get(thread));
			assertFalse("thread is alive: "+thread.getState(), thread.isAlive());
		}catch(AssertionError e) {
			throw e;
		}catch(Throwable e) {
			StringWriter writer = new StringWriter();
			PrintWriter s = new PrintWriter(writer);
			e.printStackTrace(s);
			fail(writer.toString());
		}
	}

	private void helperQThreadOut(Qt.ConnectionType connectionType) {
		try {
			MyNotifiable myRecvNotifiable = new MyNotifiable("myRecvNotifiable");
			MyNotifiable mySendNotifiable = new MyNotifiable("mySendNotifiable");
	
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "helperQThreadOut() connectionType=" + connectionType + "; " + Thread.currentThread());
	
			// we test emitting out from another thread into callback on this thread
			// myReceiver has thread affinity with this testcase thread
			MyReceiver myReceiver = new MyReceiver(myRecvNotifiable);
			myReceiver.connect(connectionType);
			assertTrue(myReceiver.isResultExactly(null));
			assertEquals(QThread.currentThread(), myReceiver.thread());
	
			// myImpl has thread affinity with the new thread being spawned
			MyImpl myImpl = new MyImpl(mySendNotifiable, myReceiver.signal, connectionType);
			QThread thread = QThread.create(myImpl);
			thread.setName("subQThreadOut");
			thread.setUncaughtExceptionHandler(this);
			myImpl.moveToThread(thread);  // not required
	
			helperImplThreadOut(thread::start, myReceiver, myImpl, myRecvNotifiable, mySendNotifiable, connectionType);
	
			assertEquals("caught exception", null, uncaughtExceptions.get(thread.javaThread()));
			assertTrue("thread is not alive: "+thread, thread.isAlive());
			myImpl.signalShutdown();
			QEventLoop loop = new QEventLoop();
			QTimer.singleShot(2000, loop::quit);
			loop.exec();
			thread.join(4000);
			assertEquals("caught exception", null, uncaughtExceptions.get(thread.javaThread()));
			assertFalse("thread is not alive: "+thread, thread.isAlive());
		}catch(AssertionError e) {
			throw e;
		}catch(Throwable e) {
			StringWriter writer = new StringWriter();
			PrintWriter s = new PrintWriter(writer);
			e.printStackTrace(s);
			fail(writer.toString());
		}
	}

	private void helperImplThreadOut(Runnable threadStart, MyReceiver myReceiver, MyImpl myImpl, MyNotifiable myRecvNotifiable, MyNotifiable mySendNotifiable, Qt.ConnectionType connectionType) {
		Object o = new Object();

		threadStart.run();
		assertTrue("waitForNotify", mySendNotifiable.waitForNotify(1000, 2));
		assertEquals(2, mySendNotifiable.getNotified());
		assertEquals(0, myRecvNotifiable.getNotified());

		myImpl.signalEmit(o);  // tell other there to emit
		if(connectionType == Qt.ConnectionType.BlockingQueuedConnection) {
			assertTrue("waitForNotify", mySendNotifiable.waitForNotify(1000, 3));  // wait for subordinate thread
			synchronized(o) {
				try {
					o.wait(1000);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}else {
			assertTrue("waitForNotify", mySendNotifiable.waitForNotify(1000, 4));  // wait for subordinate thread
		}

		assertEquals(0, myRecvNotifiable.getNotified());  // verify it is not delivered yet
		// After waiting for state 4 and then exhaust our event queue (causing signal delivery)
		QEventLoop loop = new QEventLoop();
		QTimer.singleShot(2000, loop::quit);
		loop.exec();

		// This was testing other interactions on the main problem
		//myImpl.signalProcessEvents();
		//assertTrue(mySendNotifiable.waitForNotify(1000, 5));
		//assertTrue(mySendNotifiable.waitForNotify(1000, 6));
		//while(QApplication.hasPendingEvents())
		//	QApplication.processEvents();

		if(connectionType == Qt.ConnectionType.DirectConnection) {
			// FIXME: We need to check that a WARNING/ERROR log message was emitted but right now
			//  we are using stderr to send the message.
			assertEquals(0, myRecvNotifiable.getNotified());  // verify it never gets delivered
		} else {
			assertTrue("waitForNotify", myRecvNotifiable.waitForNotify(4000, 8));  // wait for subordinate thread QEventLoop
			assertTrue(myReceiver.isResultExactly(o));  // check delivery occurred
			assertTrue(myRecvNotifiable.isNotified(8));
		}
	}

	private void helperThreadIn(Qt.ConnectionType connectionType) {
		MyNotifiable myRecvNotifiable = new MyNotifiable("myRecvNotifiable");

		java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "helperThreadIn() connectionType=" + connectionType + "; " + Thread.currentThread());

		MyImpl myImpl = new MyImpl(myRecvNotifiable, null, connectionType);
		Thread thread = new Thread(myImpl);
		thread.setName("subThreadIn");
		thread.setUncaughtExceptionHandler(this);

		helperImplThreadIn(thread::start, myImpl, myRecvNotifiable, connectionType);

		assertEquals("caught exception", null, uncaughtExceptions.get(thread));
		assertTrue("thread is not alive: "+thread.getState(), thread.isAlive());
		myImpl.signalShutdown();
		QEventLoop loop = new QEventLoop();
		QTimer.singleShot(2000, loop::quit);
		loop.exec();
		try {
			thread.join(1000);
		} catch (InterruptedException e) {
			e.printStackTrace();
			assertTrue(false);
		}
		assertEquals("caught exception", null, uncaughtExceptions.get(thread));
		assertFalse("thread is not alive: "+thread.getState(), thread.isAlive());
	}

	private void helperQThreadIn(Qt.ConnectionType connectionType) {
		MyNotifiable myRecvNotifiable = new MyNotifiable("myRecvNotifiable");

		java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "helperQThreadIn() connectionType=" + connectionType + "; " + Thread.currentThread());

		MyImpl myImpl = new MyImpl(myRecvNotifiable, null, connectionType);
		QThread thread = QThread.create(myImpl);
		thread.setName("subQThreadIn");
		thread.setUncaughtExceptionHandler(this);

		helperImplThreadIn(thread::start, myImpl, myRecvNotifiable, connectionType);

		assertEquals("caught exception", null, uncaughtExceptions.get(thread.javaThread()));
		assertTrue("thread is not alive: "+thread, thread.isAlive());
		myImpl.signalShutdown();
		QEventLoop loop = new QEventLoop();
		QTimer.singleShot(2000, loop::quit);
		loop.exec();
		try {
			thread.join(1000);
		} catch (Throwable e) {
			e.printStackTrace();
			assertTrue(false);
		}
		assertEquals("caught exception", null, uncaughtExceptions.get(thread.javaThread()));
		assertFalse("thread is not alive: "+thread, thread.isAlive());
	}

	private void helperImplThreadIn(Runnable threadStart, MyImpl myImpl, MyNotifiable myRecvNotifiable, Qt.ConnectionType connectionType) {
		Object o = new Object();

		threadStart.run();
		assertTrue("waitForNotify", myRecvNotifiable.waitForNotify(2000, 2));
		assertEquals(2, myRecvNotifiable.getNotified());

		java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "emit(" + o + ") " + Thread.currentThread());
		myImpl.signal.emit(o);  // do delivery (or queue delivery) from this thread
		QEventLoop loop = new QEventLoop();
		QTimer.singleShot(2000, loop::quit);
		loop.exec();
		// during delivery the other thread will perform delivery in its QEventLoop right away
		assertTrue("waitForNotify", myRecvNotifiable.waitForNotify(2000, 7));
		assertTrue(myImpl.isSlotInvoked());
		assertTrue(myImpl.isResultExactly(o));
	}

	// ///////////////////////////////////////////////
	
	@Test
	public void testThreadOutAuto() {
		helperThreadOut(Qt.ConnectionType.AutoConnection);
	}

	@Test
	public void testThreadInAuto() {
		helperThreadIn(Qt.ConnectionType.AutoConnection);
	}

	@Test
	public void testQThreadOutAuto() {
		helperQThreadOut(Qt.ConnectionType.AutoConnection);
	}

	@Test
	public void testQThreadInAuto() {
		helperQThreadIn(Qt.ConnectionType.AutoConnection);
	}

	@Test
	public void testThreadOutQueued() {
		helperThreadOut(Qt.ConnectionType.QueuedConnection);
	}

	@Test
	public void testThreadInQueued() {
		helperThreadIn(Qt.ConnectionType.QueuedConnection);
	}

	@Test
	public void testQThreadOutQueued() {
		helperQThreadOut(Qt.ConnectionType.QueuedConnection);
	}

	@Test
	public void testQThreadInQueued() {
		helperQThreadIn(Qt.ConnectionType.QueuedConnection);
	}

//	@Test this test does not work with native BlockingQueuedConnection
	public void testThreadOutDirect() {
		helperThreadOut(Qt.ConnectionType.DirectConnection);
	}

	@Test
	public void testThreadInDirect() {
		helperThreadIn(Qt.ConnectionType.DirectConnection);
	}

//	@Test this test does not work with native BlockingQueuedConnection
	public void testQThreadOutDirect() {
		helperQThreadOut(Qt.ConnectionType.DirectConnection);
	}

	@Test
	public void testQThreadInDirect() {
		helperQThreadIn(Qt.ConnectionType.DirectConnection);
	}

	@Test
	public void testThreadOutBlockingQueued() {
		helperThreadOut(Qt.ConnectionType.BlockingQueuedConnection);
	}

//	@Test this test does not work with native BlockingQueuedConnection
	public void testThreadInBlockingQueued() {
		helperThreadIn(Qt.ConnectionType.BlockingQueuedConnection);
	}

	@Test
	public void testQThreadOutBlockingQueued() {
		helperQThreadOut(Qt.ConnectionType.BlockingQueuedConnection);
	}

//	@Test this test does not work with native BlockingQueuedConnection
	public void testQThreadInBlockingQueued() {
		helperQThreadIn(Qt.ConnectionType.BlockingQueuedConnection);
	}

	public void uncaughtException(Thread t, Throwable e) {
		System.err.println("uncaughtException() thread=" + t);
		StringWriter writer = new StringWriter();
		PrintWriter s = new PrintWriter(writer);
		e.printStackTrace(s);
		uncaughtExceptions.put(t, writer.toString());

		Thread tmpMainThread = mainThread;
		if(tmpMainThread != null)
			tmpMainThread.interrupt();
	}
	
	private HashMap<Thread,String> uncaughtExceptions = new HashMap<>();
	
	public static void main(String[] args) {
		org.junit.runner.JUnitCore.main(TestSignalCrossThread.class.getName());
	}
}
