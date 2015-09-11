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
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import org.junit.AfterClass;
import org.junit.Before;
import org.junit.Test;

import org.qtjambi.qt.core.QCoreApplication;
import org.qtjambi.qt.core.QEvent;
import org.qtjambi.qt.core.QEventLoop;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.internal.QtJambiRuntime;

public class TestThreads extends QApplicationTest {
    @Before
    public void setUp() {
        // This class is known to fail when we messed with this setting in a previous testcase running in the same JVM
        // The method run_pingPongSignalSlot() in particular
        assertEquals("getObjectCacheMode != DEFAULT", QtJambiRuntime.getObjectCacheMode(), QtJambiRuntime.OBJECT_CACHE_MODE_DEFAULT);
    }

    @AfterClass
    public static void testDispose() throws Exception {
        PingPong.ID_PING = null;   // clean up static reference to QObject
        QApplicationTest.testDispose();
    }

    /**
     */
    private static class AffinityTester implements Runnable {
        public boolean ok = false;

        public void run() {
            QObject object = new QObject();
            ok = object.thread() == Thread.currentThread();
        }
    }

    @Test
    public void run_affinity() throws InterruptedException {
        AffinityTester tester = new AffinityTester();
        Thread thread = new Thread(tester);
        thread.start();
        thread.join(2500);
        assertFalse("thread.isAlive()", thread.isAlive());
        assertTrue("tester.ok", tester.ok);
    }

    /*************************************************************************/

    private static class PingPong extends QObject {
        /* Removed 'final' and made public (from private) so we can cleanup on destruction and get clean memory leak check */
        public static /*final*/ QEvent.Type ID_PING = QEvent.Type.resolve(QEvent.Type.User.value() + 1);
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
            affinityOk &= Thread.currentThread() == thread();
            return super.event(e);
        }
    }

    private static class PingPongRunner extends Thread {
        public PingPong object;
        public boolean ping;
        public boolean goFlag;  // startup sync
        public PingPongRunner(boolean ping) {
            setDaemon(true);
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
            object.done.connect(loop, "quit()");
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

            ping.start();
            pong.start();

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
            for(int i = 0; i < 60; i++) {
                if(!ping.isAlive())
                    break;
                if(pingNumPings == ping.object.numPings) {
                    same++;
                } else {
                    pingNumPings = ping.object.numPings;
                    same = 0;
                }
                if(same > 5)
                    break;
                ping.join(500);
            }
            pong.join(1500);

            assertFalse("ping.isAlive()", ping.isAlive());
            assertFalse("pong.isAlive()", pong.isAlive());

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
            affinityOk &= thread() == Thread.currentThread();
        }

        public void firePing() {
            ping.emit();
        }
    }

    private static class PingPongSSRunner extends Thread {
        public PingPongSS object;
        public boolean ping;
        public boolean ready;
        public PingPongSSRunner(boolean ping) {
            setDaemon(true);
            this.ping = ping;
        }
        @Override
        public void run() {
            PingPongSS tmpObject = new PingPongSS();
            synchronized(this) {
                object = tmpObject;
                notifyAll();
            }

            QEventLoop loop = new QEventLoop();
            object.done.connect(loop, "quit()");

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

        ping.start();
        pong.start();

        ping.startupReady(500);  // wait for object to be initalized
        pong.startupReady(500);

        assertNotNull("ping.object", ping.object);
        assertNotNull("pong.object", pong.object);

        ping.object.other = pong.object;
        pong.object.other = ping.object;

        ping.object.ping.connect(pong.object, "pong()");
        pong.object.ping.connect(ping.object, "pong()");

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
        for(int i = 0; i < 60; i++) {
            if(!ping.isAlive())
                break;
            if(pingNumPings == ping.object.numPings) {
                same++;
            } else {
                pingNumPings = ping.object.numPings;
                same = 0;
            }
            if(same > 5)
                break;
            ping.join(500);
        }
        pong.join(1500);

        assertFalse("ping.isAlive()", ping.isAlive());
        assertFalse("pong.isAlive()", pong.isAlive());

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
        Thread currentThread = Thread.currentThread();

        {
            QObject object = new QObject();
            QObject child = new QObject(object);
            assertEquals(object.thread(), currentThread);
            assertEquals(child.thread(), currentThread);
            object.moveToThread(null);
            assertEquals(object.thread(), null);
            assertEquals(child.thread(), null);
            object.moveToThread(currentThread);
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

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestThreads.class.getName());
    }
}
