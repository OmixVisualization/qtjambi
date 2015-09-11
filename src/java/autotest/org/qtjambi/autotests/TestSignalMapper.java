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
import static org.junit.Assert.assertTrue;

import org.junit.Before;
import org.junit.Test;

import org.qtjambi.qt.core.QEventLoop;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.core.QSignalMapper;
import org.qtjambi.qt.widgets.QGuiSignalMapper;
import org.qtjambi.qt.widgets.QWidget;
import org.qtjambi.qt.internal.QtJambiRuntime;

public class TestSignalMapper extends QApplicationTest {

    @Before
    public void setUp() {
        // This class is known to fail when we messed with this setting in a previous testcase running in the same JVM
        // The method run_mappedInt() in particular
        assertEquals("getObjectCacheMode != DEFAULT", QtJambiRuntime.OBJECT_CACHE_MODE_DEFAULT, QtJambiRuntime.getObjectCacheMode());
    }

    /**
     * Receiver class for the various mapped signals in this test.
     */
    private static class Receiver extends QObject {
        private int lastInteger;
        private String lastString;
        private QObject lastQObject;
        private QWidget lastQWidget;

        public int getLastInteger() {
            synchronized(this) {
                return lastInteger;
            }
        }
        public void slotInteger(int i) {
            synchronized(this) {
                lastInteger = i;
                notifyAll();
            }
        }

        public String getLastString() {
            synchronized(this) {
                return lastString;
            }
        }
        public void slotString(String s) {
            synchronized(this) {
                lastString = s;
                notifyAll();
            }
        }

        public QObject getLastQObject() {
            synchronized(this) {
                return lastQObject;
            }
        }
        public void slotQObject(QObject o) {
            synchronized(this) {
                lastQObject = o;
                notifyAll();
            }
        }

        public QWidget getLastQWidget() {
            synchronized(this) {
                return lastQWidget;
            }
        }
        public void slotQWidget(QWidget w) {
            synchronized(this) {
                lastQWidget = w;
                notifyAll();
            }
        }

        public void reset() {
            synchronized(this) {
                lastInteger = -1;
                lastString = null;
                lastQObject = null;
                lastQWidget = null;
                notifyAll();
            }
        }
    }

    /**
     * Emitter class for triggering the various mapped signals...
     */
    private static class Emitter extends QObject {
        Signal0 signal = new Signal0();

        public void emitSignal() {
            signal.emit();
        }
    }

    private static class SignalQuit extends QObject {
        Signal0 signal = new Signal0();

        public Signal0 getSignal0() {
            return signal;
        }
    }

    // Method requires: getObjectCacheMode == DEFAULT
    @Test
    public void run_mappedInt() throws InterruptedException {
        QSignalMapper mapper = new QSignalMapper();
        Receiver receiver = new Receiver();
        final Emitter emitters[] = new Emitter[10];

        for (int i = 0; i < emitters.length; ++i) {
            emitters[i] = new Emitter();
            emitters[i].signal.connect(mapper, "map()");
            mapper.setMapping(emitters[i], i);
        }
        mapper.mappedInteger.connect(receiver, "slotInteger(int)");

        for (int i = 0; i < emitters.length; ++i) {
            emitters[i].emitSignal();
            assertEquals("receiver.getLastInteger() same thread", i, receiver.getLastInteger());
        }
        receiver.reset();
        assertEquals("receiver.getLastInteger()!=-1", -1, receiver.getLastInteger());

        final SignalQuit signalQuit = new SignalQuit();
        Thread thread = new Thread("Receiver Thread") {
            @Override
            public void run() {
                try {
                    QEventLoop eventLoop = new QEventLoop();
                    signalQuit.getSignal0().connect(eventLoop, "quit()");
                    eventLoop.exec();
                } catch(Throwable t) {
                    t.printStackTrace();
                }
            }
        };

        receiver.moveToThread(thread);
        thread.start();

        receiver.reset();
        assertEquals("receiver.getLastInteger()!=-1", -1, receiver.getLastInteger());
        assertTrue("thread.isAlive()", thread.isAlive());  // make sure Thread did not die on us
        for (int i = 0; i < emitters.length; ++i) {
            emitters[i].emitSignal();

            try {
                // Use Java sleep/wake to speed this up
                synchronized(receiver) {
                    receiver.wait(1000); // was: Thread.sleep(100)
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            assertTrue("thread.isAlive()", thread.isAlive());  // make sure Thread did not die on us
            assertEquals("receiver.getLastInteger() diff thread", i, receiver.getLastInteger());
        }
        receiver.reset();
        assertEquals("receiver.getLastInteger()!=-1", -1, receiver.getLastInteger());

        // Shutdown the thread
        signalQuit.getSignal0().emit();
        thread.join(1000);
        assertFalse("thread.isAlive()", thread.isAlive());
    }

    @Test
    public void run_mappedString() {
        QSignalMapper mapper = new QSignalMapper();
        Receiver receiver = new Receiver();
        final Emitter emitters[] = new Emitter[10];

        for (int i = 0; i < emitters.length; ++i) {
            emitters[i] = new Emitter();
            emitters[i].signal.connect(mapper, "map()");
            mapper.setMapping(emitters[i], "id(" + i + ")");
        }
        mapper.mappedString.connect(receiver, "slotString(String)");

        for (int i = 0; i < emitters.length; ++i) {
            emitters[i].emitSignal();
            assertEquals("receiver.getLastString()", "id(" + i + ")", receiver.getLastString());
        }
    }

    @Test
    public void run_mappedQObject() {
        QSignalMapper mapper = new QSignalMapper();
        Receiver receiver = new Receiver();
        final Emitter emitters[] = new Emitter[10];

        for (int i = 0; i < emitters.length; ++i) {
            emitters[i] = new Emitter();
            emitters[i].signal.connect(mapper, "map()");
            mapper.setMapping(emitters[i], emitters[i]);
        }
        mapper.mappedQObject.connect(receiver, "slotQObject(QObject)");

        for (int i = 0; i < emitters.length; ++i) {
            emitters[i].emitSignal();
            assertEquals("receiver.getLastQObject()", emitters[i], receiver.getLastQObject());
        }
    }

    @SuppressWarnings("deprecation")
    @Test
    public void run_mappedQWidget() {
        QGuiSignalMapper mapper = new QGuiSignalMapper();
        Receiver receiver = new Receiver();
        final Emitter emitters[] = new Emitter[10];
        QWidget widgets[] = new QWidget[10];

        for (int i = 0; i < emitters.length; ++i) {
            emitters[i] = new Emitter();
            widgets[i] = new QWidget();
            emitters[i].signal.connect(mapper, "map()");
            mapper.setMapping(emitters[i], widgets[i]);
        }
        mapper.mappedQWidget.connect(receiver, "slotQWidget(QWidget)");

        for (int i = 0; i < emitters.length; ++i) {
            emitters[i].emitSignal();
            assertEquals("receiver.getLastQWidget()", widgets[i], receiver.getLastQWidget());
        }
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestSignalMapper.class.getName());
    }
}
