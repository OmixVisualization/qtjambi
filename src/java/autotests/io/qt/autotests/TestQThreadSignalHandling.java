/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QEventLoop;
import io.qt.core.QObject;
import io.qt.core.QThread;
import io.qt.internal.TestUtility;

public class TestQThreadSignalHandling extends ApplicationInitializer{
	
	static {
		System.setProperty("io.qt.enable-thread-affinity-check", "true");
		System.setProperty("io.qt.enable-event-thread-affinity-check", "true");
	}
	
    @Before
    public void setUp() {
        // This class is known to fail when we messed with this setting in a previous testcase running in the same JVM
        // The method run_pingPongSignalSlot() in particular
        assertEquals("getObjectCacheMode != DEFAULT", TestUtility.objectCacheMode(), TestUtility.OBJECT_CACHE_MODE_DEFAULT);
    }
    
    @BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitializeWithWidgets();
    }

    @AfterClass
    public static void testDispose() throws Exception {
        PingPong.ID_PING = null;   // clean up static reference to QObject
        ApplicationInitializer.testDispose();
    }

    /*************************************************************************/

    private static class PingPong extends QObject {
        /* Removed 'final' and made public (from private) so we can cleanup on destruction and get clean memory leak check */
        public static /*final*/ QEvent.Type ID_PING = QEvent.Type.resolve(QEvent.registerEventType());
        public int numPings = 0;
        public PingPong other;
        public boolean affinityOk = true;
        final Signal0 done = new Signal0();
        @Override
        public boolean event(QEvent e) {
            if (e.type() == ID_PING) {
                int thisNumPings;
                synchronized (this) {
                    numPings++;
                    thisNumPings = numPings;
                }
                QCoreApplication.postEvent(other, new QEvent(ID_PING));
                if (thisNumPings == 10) {
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

            assertEquals("ping.object.numPings", 10, ping.object.numPings);
            assertEquals("pong.object.numPings", 10, pong.object.numPings);

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
        final Signal0 done = new Signal0();
        final Signal0 ping = new Signal0();

        @SuppressWarnings("unused")
		public void pong() {
            int thisNumPings;
            synchronized (this) {
                numPings++;
                thisNumPings = numPings;
            }
            ping.emit();
            if (thisNumPings == 10)
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
            object.done.connect(loop, "quit()");
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
                long step = millis / 100;
                for(long w = 0; w<millis; w+=step) {
	                try { wait(100); } catch (InterruptedException e) { };
	                if(object != null)
	                    return true;
                }
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

        ping.startupReady(15000);  // wait for object to be initalized
        pong.startupReady(15000);

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

        assertEquals("ping.object.numPings", 10, ping.object.numPings);
        assertEquals("pong.object.numPings", 10, pong.object.numPings);

        assertTrue("ping.object.affinityOk", ping.object.affinityOk);
        assertTrue("ping.object.affinityOk", ping.object.affinityOk);
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQThreadSignalHandling.class.getName());
    }
}
