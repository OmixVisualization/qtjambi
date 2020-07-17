/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

import java.util.concurrent.atomic.AtomicBoolean;

import org.junit.AfterClass;
import org.junit.Before;
import org.junit.Test;

import io.qt.QNoNativeResourcesException;
import io.qt.QThreadAffinityException;
import io.qt.QtUtilities;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QEventLoop;
import io.qt.core.QObject;
import io.qt.core.QThread;
import io.qt.core.QVariantAnimation;
import io.qt.core.Qt;
import io.qt.internal.QtJambiDebugTools;
import io.qt.internal.QtJambiInternal;
import io.qt.internal.QtJambiThreadUtility;
import io.qt.widgets.QWidget;

public class TestThreads extends QApplicationTest {
    @Before
    public void setUp() {
        // This class is known to fail when we messed with this setting in a previous testcase running in the same JVM
        // The method run_pingPongSignalSlot() in particular
        assertEquals("getObjectCacheMode != DEFAULT", QtJambiDebugTools.getObjectCacheMode(), QtJambiDebugTools.OBJECT_CACHE_MODE_DEFAULT);
    }

    @AfterClass
    public static void testDispose() throws Exception {
        PingPong.ID_PING = null;   // clean up static reference to QObject
        QApplicationTest.testDispose();
    }

    @Test
    public void run_affinity_JavaThread() throws InterruptedException {
        boolean[] ok = {false};
        Thread thread = new Thread(()->{
            QObject object = new QObject();
            ok[0] = object.thread() == QThread.currentThread();
//            object.dispose();
        });
        thread.start();
        thread.join(2500);
        assertFalse("thread.isAlive()", thread.isAlive());
        assertTrue("tester.ok", ok[0]);
    }
    
    @Test
    public void run_affinity_QThread() throws InterruptedException {
        boolean[] ok = {false};
        QThread thread = QThread.create(()->{
            QObject object = new QObject();
            ok[0] = object.thread() == QThread.currentThread();
//            object.dispose();
        });
        thread.start();
        thread.join(2500);
        assertFalse("thread.isAlive()", thread.isAlive());
        assertTrue("tester.ok", ok[0]);
    }
    
    @Test
    public void testJavaThreadDeletion() throws InterruptedException {
    	AtomicBoolean threadDisposed = new AtomicBoolean();
		AtomicBoolean qobjectDisposed = new AtomicBoolean();
		AtomicBoolean qthreadCleaned = new AtomicBoolean();
		AtomicBoolean qthreadFinished = new AtomicBoolean();
		AtomicBoolean qthreadDestroyed = new AtomicBoolean();
		AtomicBoolean qobjectDestroyed = new AtomicBoolean();
		AtomicBoolean threadCleaned = new AtomicBoolean();
    	{
	    	Thread thread = new Thread(()->{
	    		try {
	    			QThread qthread = QThread.currentThread();
	    			qthread.finished.connect(()->{
	    				qthreadFinished.set(true);
    				});
	    			QtUtilities.getSignalOnDispose(qthread).connect(()->threadDisposed.set(true), Qt.ConnectionType.DirectConnection);
					qthread.destroyed.connect(()->qthreadDestroyed.set(true), Qt.ConnectionType.DirectConnection);
	    			QtJambiInternal.registerCleaner(qthread, ()->qthreadCleaned.set(true));
					QObject object = new QObject();
					QtUtilities.getSignalOnDispose(object).connect(()->qobjectDisposed.set(true), Qt.ConnectionType.DirectConnection);
					object.destroyed.connect(()->qobjectDestroyed.set(true), Qt.ConnectionType.DirectConnection);
				} catch (Throwable e) {
					e.printStackTrace();
				}
	    	});
	    	thread.setName("\0\0");
	    	QtJambiInternal.registerCleaner(thread, ()->threadCleaned.set(true));
	    	thread.setDaemon(true);
	    	thread.start();
	    	thread = null;
    	}
		Thread.yield();
		System.gc();
		Thread.sleep(100);
    	for (int i = 0; i < 200; i++) {
    		if(threadCleaned.get()) {
        		Thread.yield();
        		Thread.sleep(100);
    			break;
    		}
    		Thread.yield();
    		System.gc();
    		Thread.sleep(10);
		}
		Thread.yield();
		System.gc();
		Thread.yield();
		Thread.sleep(100);
		System.gc();
		Thread.yield();
    	assertTrue("qobjectDisposed", qobjectDisposed.get());
    	assertTrue("qobjectDestroyed", qobjectDestroyed.get());
    	assertTrue("qthreadDisposed", threadDisposed.get());
    	assertTrue("qthreadDestroyed", qthreadDestroyed.get());
    	assertTrue("qthreadFinished", qthreadFinished.get());
    	assertTrue("threadCleaned", threadCleaned.get());
    	assertTrue("qthreadCleaned", qthreadCleaned.get());
    }
    
    @Test
    public void testQThreadDeletion() throws InterruptedException {
		AtomicBoolean threadDisposed = new AtomicBoolean();
		AtomicBoolean qobjectDisposed = new AtomicBoolean();
		AtomicBoolean threadDestroyed = new AtomicBoolean();
		AtomicBoolean qobjectDestroyed = new AtomicBoolean();
    	{
	    	QThread thread = QThread.create(()->{
	    		QObject object = new QObject();
	    		QtUtilities.getSignalOnDispose(object).connect(()->qobjectDisposed.set(true), Qt.ConnectionType.DirectConnection);
	    		object.destroyed.connect(()->qobjectDestroyed.set(true));
	    	});
	    	QtUtilities.getSignalOnDispose(thread).connect(()->threadDisposed.set(true), Qt.ConnectionType.DirectConnection);
    		thread.destroyed.connect(()->threadDestroyed.set(true), Qt.ConnectionType.DirectConnection);
	    	thread.setDaemon(true);
	    	thread.start();
    	}
		Thread.yield();
		System.gc();
		Thread.sleep(100);
    	for (int i = 0; i < 200; i++) {
    		if(threadDestroyed.get() || threadDisposed.get())
    			i = 200;
    		Thread.yield();
    		System.gc();
    		Thread.sleep(10);
        	QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		}
    	Thread.sleep(100);
		System.gc();
		Thread.yield();
    	QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
    	assertTrue("qobjectDisposed", qobjectDisposed.get());
    	assertTrue("qobjectDestroyed", qobjectDestroyed.get());
    	assertTrue("threadDisposed", threadDisposed.get());
    	assertTrue("threadDestroyed", threadDestroyed.get());
    }
    
    @Test
    public void testEventThreadDeletion() throws InterruptedException {
    	AtomicBoolean threadFinished = new AtomicBoolean();
		AtomicBoolean threadDisposed = new AtomicBoolean();
		AtomicBoolean qobjectDisposed = new AtomicBoolean();
		AtomicBoolean threadDestroyed = new AtomicBoolean();
		AtomicBoolean qobjectDestroyed = new AtomicBoolean();
    	{
	    	QThread thread = new QThread() {};
    		QObject object = new QObject();
    		QtUtilities.getSignalOnDispose(object).connect(()->qobjectDisposed.set(true), Qt.ConnectionType.DirectConnection);
    		object.destroyed.connect(()->qobjectDestroyed.set(true), Qt.ConnectionType.DirectConnection);
    		object.destroyed.connect(thread::quit, Qt.ConnectionType.DirectConnection);
    		object.moveToThread(thread);
    		object.disposeLater();
    		
    		QtUtilities.getSignalOnDispose(thread).connect(()->threadDisposed.set(true), Qt.ConnectionType.DirectConnection);
    		thread.destroyed.connect(()->threadDestroyed.set(true), Qt.ConnectionType.DirectConnection);
	    	thread.setDaemon(true);
	    	thread.finished.connect(()->threadFinished.set(true), Qt.ConnectionType.DirectConnection);
	    	thread.start();
    	}
		Thread.yield();
		Thread.sleep(100);
		System.gc();
    	for (int i = 0; i < 200; i++) {
    		if(threadFinished.get())
    			i = 200;
    		Thread.yield();
    		System.gc();
    		Thread.sleep(10);
        	QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		}
		Thread.yield();
    	Thread.sleep(100);
		System.gc();
    	QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
    	assertTrue("qobjectDestroyed", qobjectDestroyed.get());
    	assertTrue("qobjectDisposed", qobjectDisposed.get());
    	assertTrue("threadDestroyed", threadDestroyed.get());
    	assertTrue("threadDisposed", threadDisposed.get());
    }

    /*************************************************************************/

    private static class PingPong extends QObject {
        /* Removed 'final' and made public (from private) so we can cleanup on destruction and get clean memory leak check */
        public static /*final*/ QEvent.Type ID_PING = QEvent.Type.resolve(QEvent.registerEventType());
        public int numPings = 0;
        public PingPong other;
        public boolean affinityOk = true;
        Signal0 done = new Signal0();
        @Override
        public boolean event(QEvent e) {
            if (e.type() == ID_PING) {
                int thisNumPings;
                synchronized (this) {
                    numPings++;
                    thisNumPings = numPings;
                }
                QCoreApplication.postEvent(other, new QEvent(ID_PING));
                if (thisNumPings == 100) {
                    done.emit();
                }
            }
            affinityOk &= QThread.currentThread() == thread();
            return super.event(e);
        }
    }

    private static class PingPongRunner implements Runnable {
        public PingPong object;
        public boolean ping;
        public boolean goFlag;  // startup sync
        public PingPongRunner(boolean ping) {
            this.ping = ping;
        }
        @Override
        public void run() {
            synchronized (this) {
                object = new PingPong();
                notifyAll(); // let the main thread set the
                if (object.other == null)
                    try { wait(); } catch (InterruptedException e) { };
            }

            if (ping) {
                synchronized (this) {
                    while (true) {
                        if (goFlag)
                            break;
                        // It is important the pinger doesn't start any the pong will receive
                        try { wait(); } catch (InterruptedException e) { };
                    }
                }
                QCoreApplication.postEvent(object.other, new QEvent(PingPong.ID_PING));
            }
            QEventLoop loop = new QEventLoop();
            object.done.connect(loop::quit);
            loop.exec();
        }

        public synchronized void go() {
            goFlag = true;
            notifyAll();
        }

        public synchronized void setOtherObject(PingPong ppr) {
            object.other = ppr;
            notifyAll();
        }

        public synchronized void waitUntilObjectCreated() {
            if (object == null)
                try { wait(); } catch (InterruptedException e) { };
        }
    }

    @Test
    public void run_pingPong() {
        try {
            PingPongRunner ping = new PingPongRunner(true);
            PingPongRunner pong = new PingPongRunner(false);

            QThread pingThread = QThread.create(ping);
            pingThread.setDaemon(true);
            pingThread.start();
            QThread pongThread = QThread.create(pong);
            pongThread.setDaemon(true);
            pongThread.start();

            ping.waitUntilObjectCreated();
            pong.waitUntilObjectCreated();

            assertNotNull("ping.object != null", ping.object);
            assertNotNull("pong.object != null", pong.object);

            ping.setOtherObject(pong.object);
            pong.setOtherObject(ping.object);

            ping.go();

            // This is designed to give it more time to keep making progress
            int pingNumPings = -1;
            int same = 0;
            for(int i = 0; i < 120; i++) {
                if(!pingThread.isAlive())
                    break;
                if(pingNumPings == ping.object.numPings) {
                    same++;
                } else {
                    pingNumPings = ping.object.numPings;
                    same = 0;
                }
                if(same > 5)
                    break;
                pingThread.join(500);
            }
            pongThread.join(1500);

            assertFalse("ping.isAlive()", pingThread.isAlive());
            assertFalse("pong.isAlive()", pongThread.isAlive());

            assertEquals("ping.object.numPings", 100, ping.object.numPings);
            assertEquals("pong.object.numPings", 100, pong.object.numPings);

            assertTrue("ping.object.affinityOk", ping.object.affinityOk);
            assertTrue("ping.object.affinityOk", ping.object.affinityOk);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


    private static class PingPongSS extends QObject {
        public int numPings = 0;
        @SuppressWarnings("unused")
		public PingPongSS other;
        public boolean affinityOk = true;
        Signal0 done = new Signal0();
        Signal0 ping = new Signal0();

        public void pong() {
            int thisNumPings;
            synchronized (this) {
                numPings++;
                thisNumPings = numPings;
            }
            ping.emit();
            if (thisNumPings == 100)
                done.emit();
            affinityOk &= thread() == QThread.currentThread();
        }

        public void firePing() {
            ping.emit();
        }
    }

    private static class PingPongSSRunner implements Runnable {
        public PingPongSS object;
        public boolean ping;
        public boolean ready;
        public PingPongSSRunner(boolean ping) {
            this.ping = ping;
        }
        @Override
        public void run() {
        	String threadName = QThread.currentThread().objectName();
            PingPongSS tmpObject = new PingPongSS();
            System.err.println(threadName+" REACH 1");
            synchronized(this) {
                object = tmpObject;
                notifyAll();
            }
            System.err.println(threadName+" REACH 2");

            QEventLoop loop = new QEventLoop();
            System.err.println(threadName+" REACH 3");
            object.done.connect(loop::quit);
            System.err.println(threadName+" REACH 4");

            // Wait for startup sync
            while(true) {
                synchronized(this) {
                    try {
                        wait(25);
                    } catch (InterruptedException eat) {
                    }
                    if(ready) {
                        break;  // lets go
                    }
                }
            }
            if (ping) {
                object.firePing();
            }
            loop.exec();
        }
        public boolean startupReady(long millis) {
            synchronized(this) {
                if(object != null)
                    return true;
                try { wait(millis); } catch (InterruptedException e) { };
                if(object != null)
                    return true;
            }
            return false;
        }
    }

    // Method requires: getObjectCacheMode == DEFAULT
    @Test
    public void run_pingPongSignalSlot() throws Exception {
        PingPongSSRunner ping = new PingPongSSRunner(true);
        PingPongSSRunner pong = new PingPongSSRunner(false);

        QThread pingThread = QThread.create(ping);
        pingThread.setName("pingThread");
        pingThread.setObjectName("pingThread");
        pingThread.setDaemon(true);
        pingThread.start();
        QThread pongThread = QThread.create(pong);
        pongThread.setName("pongThread");
        pongThread.setObjectName("pongThread");
        pongThread.setDaemon(true);
        pongThread.start();

        ping.startupReady(1500);  // wait for object to be initalized
        pong.startupReady(1500);

        assertNotNull("ping.object", ping.object);
        assertNotNull("pong.object", pong.object);

        ping.object.other = pong.object;
        pong.object.other = ping.object;

        ping.object.ping.connect(pong.object::pong);
        pong.object.ping.connect(ping.object::pong);

        synchronized(ping) {
            synchronized(pong) {
                ping.ready = true;
                ping.notifyAll();
                pong.ready = true;
                pong.notifyAll();
            }
        }

        // This is designed to give it more time to keep making progress
        int pingNumPings = -1;
        int same = 0;
        for(int i = 0; i < 240; i++) {
            if(!pingThread.isAlive())
                break;
            if(pingNumPings == ping.object.numPings) {
                same++;
            } else {
                pingNumPings = ping.object.numPings;
                same = 0;
            }
            if(same > 5)
                break;
            pingThread.join(500);
        }
        pongThread.join(1500);

        assertFalse("pingThread.isAlive()", pingThread.isAlive());
        assertFalse("pongThread.isAlive()", pongThread.isAlive());

        assertEquals("ping.object.numPings", 100, ping.object.numPings);
        assertEquals("pong.object.numPings", 100, pong.object.numPings);

        assertTrue("ping.object.affinityOk", ping.object.affinityOk);
        assertTrue("ping.object.affinityOk", ping.object.affinityOk);
    }



    /*private static class MoveToThreadObject extends QObject {
        Thread timerEventThread;
        Thread customEventThread;
        Thread slotThread;

        Signal0 theSignal = new Signal0();
        Signal0 done = new Signal0();

        MoveToThreadObject() {
            this(null);
        }
        MoveToThreadObject(QObject parent) {
            super(parent);
        }

        protected void customEvent(QEvent e)
        {
            if (customEventThread != null)
                throw new RuntimeException("customEventThread expected to be null at this point");
            customEventThread = Thread.currentThread();
            theSignal.emit();
        }

        protected void timerEvent(QTimerEvent e)
        {
            if (timerEventThread != null)
                throw new RuntimeException("timerEventThread expected to be null at this point");
            timerEventThread = Thread.currentThread();
            theSignal.emit();
        }

        void theSlot()
        {
            if (slotThread != null)
                throw new RuntimeException("slotThread expected to be null at this point");
            slotThread = Thread.currentThread();
            theSignal.emit();
        }

        public void emitDone() {
            done.emit();
        }
    }*/



    /*private static class MoveToThreadThread extends QObject implements Runnable {
        Signal0 started = new Signal0();
        Thread thread = new Thread(this);
        QEventLoop loop;
        public void start() {
            QEventLoop eventLoop = new QEventLoop();
            started.connect(eventLoop, "quit()");

            thread.start();

            // wait for thread to start
            eventLoop.exec();
        }

        public void run() {
            started.emit();
            loop = new QEventLoop();
            loop.exec();
        }
    }
    */

    @Test
    public void run_moveToThread() throws Exception
    {
        Object currentThread = QThread.currentThread();

        {
            QObject object = new QObject();
            QObject child = new QObject(object);
            assertEquals(object.thread(), currentThread);
            assertEquals(child.thread(), currentThread);
            object.moveToThread(null);
            assertEquals(object.thread(), null);
            assertEquals(child.thread(), null);
            object.moveToThread(QtJambiThreadUtility.castToThread(currentThread));
            assertEquals(object.thread(), currentThread);
            assertEquals(child.thread(), currentThread);
            object.moveToThread(null);
            assertEquals(object.thread(), null);
            assertEquals(child.thread(), null);
            // can delete an object with no thread anywhere
            object.dispose();
        }

        /*
        {
            MoveToThreadThread thread;
            thread.start();

            QObject object = new QObject();
            QObject child = new QObject(object);
            QObject opointer = object;
            QObject cpointer = object;

            assertEquals(object.thread(), currentThread);
            assertEquals(child.thread(), currentThread);
            object.moveToThread(thread);
            assertEquals(object.thread(), thread);
            assertEquals(child.thread(), thread);

            connect(object.destroyed, new Slot(thread, "quit()"), Qt.DirectConnection);
            object.deleteLater();
            thread.wait();

            assertTrue(opointer == 0);
            assertTrue(cpointer == 0);
        }
        */

//        {
//            // make sure posted events are moved with the object
//            MoveToThreadThread thread = new MoveToThreadThread();
//            thread.start();
//
//            MoveToThreadObject object = new MoveToThreadObject();
//            MoveToThreadObject child = new MoveToThreadObject(object);
//
//            QObject.connect(object.theSignal, new Slot(thread.loop, "quit()"), Qt.DirectConnection);
//            QCoreApplication.postEvent(child, new QEvent(QEvent.User));
//            QCoreApplication.postEvent(object, new QEvent(QEvent.User));
//
//            assertEquals(object.thread(), currentThread);
//            assertEquals(child.thread(), currentThread);
//            object.moveToThread(thread.thread);
//            assertEquals(object.thread(), thread);
//            assertEquals(child.thread(), thread);
//
//            thread.thread.join();
//
//            assertEquals(object.customEventThread, thread);
//            assertEquals(child.customEventThread, thread);
//
//            thread.start();
//            QObject.connect(object.done, new Slot(thread.loop, "quit()"), Qt.DirectConnection);
//            object.emitDone();
//            thread.thread.join();
//        }
//
//        {
//            // make sure timers are moved with the object
//            MoveToThreadThread thread = new MoveToThreadThread();
//            thread.start();
//
//            MoveToThreadObject object = new MoveToThreadObject();
//            MoveToThreadObject child = new MoveToThreadObject(object);
//
//            QObject.connect(object.theSignal, new Slot(thread.loop, "quit()"), Qt.DirectConnection);
//            child.startTimer(90);
//            object.startTimer(100);
//
//            assertEquals(object.thread(), currentThread);
//            assertEquals(child.thread(), currentThread);
//            object.moveToThread(thread.thread);
//            assertEquals(object.thread(), thread);
//            assertEquals(child.thread(), thread);
//
//            thread.thread.join();
//
//            assertEquals(object.timerEventThread, thread);
//            assertEquals(child.timerEventThread, thread);
//
//            thread.start();
//            QObject.connect(object.done, new Slot(thread.loop, "quit()"), Qt.DirectConnection);
//            object.emitDone();
//            thread.thread.join();
//        }

        /*
        {
            // make sure socket notifiers are moved with the object
            MoveToThreadThread thread;
            thread.start();

            QTcpServer server;
            assertTrue(server.listen(QHostAddress.LocalHost, 0));
            QTcpSocket *socket = new QTcpSocket();
            MoveToThreadObject *child = new MoveToThreadObject(socket);
            connect(socket, SIGNAL(disconnected()), child, SLOT(theSlot()), Qt.DirectConnection);
            connect(child, SIGNAL(theSignal()), thread, SLOT(quit()), Qt::DirectConnection);

            socket.connectToHost(server.serverAddress(), server.serverPort());

            server.waitForNewConnection();
            QTcpSocket *serverSocket = server.nextPendingConnection();
            assertTrue("serverSocket", serverSocket);

            socket.waitForConnected();

            assertEquals(socket.thread(), currentThread);
            socket.moveToThread(thread);
            assertEquals(socket.thread(), thread);

            serverSocket.close();

            assertTrue(thread.join(10000));

            assertEquals(child.slotThread, (Thread *)thread);

            thread.start();
            connect(socket, SIGNAL(destroyed()), thread, SLOT(quit()), Qt.DirectConnection);
            socket.deleteLater();
            thread.join();
        }
        */
    }
    
    @Test
    public void run_adoptedThread() throws Exception
    {
    	QObject movedObject = new QObject();
    	movedObject.destroyed.connect(()->{
    		System.out.println("dispose movedObject");
    	});
    	QObject _movedObject = movedObject;
    	QThread thread = QThread.create(()->{
    		QObject threadInObject = new QObject(_movedObject);
    		threadInObject.destroyed.connect(()->{
    			System.out.println("dispose threadInObject");
    		});
    	});
    	movedObject.moveToThread(thread);
    	thread.destroyed.connect(()->System.out.println("dispose thread"));
    	thread.start();
    	thread = null;
    	movedObject = null;
    	System.gc();
    	System.runFinalization();
    	QCoreApplication.processEvents();
    }
    
    @Test(expected=QThreadAffinityException.class)
    public void run_affinity_breach() throws Exception
    {
    	QObject object = new QObject();
    	QThread thread = new QThread();
    	try {
	    	object.moveToThread(thread);
	    	object.startTimer(90);
    	}finally {
//    		object.dispose();
    	}
    }
    
    @Test(expected=QThreadAffinityException.class)
    public void run_affinity_breach_parent() throws Exception
    {
    	QObject object = new QObject();
    	QThread thread = new QThread();
    	try {
	    	object.moveToThread(thread);
	    	new QObject(object);
    	}finally {
//    		object.dispose();
    	}
    }
    
    @Test(expected=QThreadAffinityException.class)
    public void run_widget_affinity_breach() throws Exception
    {
    	QThreadAffinityException[] exn = {null};
    	QThread thread = QThread.create(()->{
    		try {
				new QWidget();
			} catch (QThreadAffinityException e) {
				exn[0] = e;
			}
    	});
    	thread.start();
    	thread.join();
    	if(exn[0]!=null)
    		throw exn[0];
    }
    
    @Test(expected=QNoNativeResourcesException.class)
    public void run_affinity_breach_no_native() throws Exception
    {
    	QObject object = new QObject();
    	QThread thread = new QThread();
    	object.moveToThread(thread);
    	object.dispose();
    	object.setObjectName("test");
    }
    
    @Test(expected=QNoNativeResourcesException.class)
    public void run_no_native() throws Exception
    {
    	QObject object = new QObject();
    	object.dispose();
    	object.setObjectName("test");
    }
    
    @Test
    public void run_qthread_terminated() throws Exception
    {
    	Thread[] jthread = {null, null};
    	QThread thread = QThread.create(()->{
    		jthread[0] = QThread.currentThread().javaThread();
    		jthread[1] = Thread.currentThread();
    		new Thread(()->{}).start();
    	});
    	thread.start();
    	thread.join();
    	assertEquals(jthread[0], jthread[1]);
    	assertFalse(thread.isRunning());
    	assertEquals(jthread[1].getState(), Thread.State.TERMINATED);
    }
    
    @Test
    public void test_thread_affinity()
    {
    	QVariantAnimation parent = new QVariantAnimation();
    	QVariantAnimation child = new QVariantAnimation();
    	parent.valueChanged.connect(child.valueChanged);
    	Object[] result = {null};
    	child.valueChanged.connect(o->result[0] = o);
    	Throwable[] t = {null};
    	QThread thread = QThread.create(()->{
    		parent.valueChanged.emit("TEST");
    		try {
				parent.setProperty("", "");
			} catch (Throwable e) {
				t[0] = e;
			}
    	});
    	thread.start();
    	thread.join();
    	QCoreApplication.sendPostedEvents();
    	assertTrue(t[0] instanceof QThreadAffinityException);
    	assertEquals("TEST", result[0]);
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestThreads.class.getName());
    }
}
