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
import static org.junit.Assert.fail;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.LinkedList;
import java.util.List;

import org.junit.Before;
import org.junit.Test;

import org.qtjambi.autotests.generated.SignalsAndSlots;
import org.qtjambi.qt.QNoSuchSlotException;
import org.qtjambi.qt.QSignalEmitter;
import org.qtjambi.qt.QVariant;
import org.qtjambi.qt.QtBlockedSlot;
import org.qtjambi.qt.QtInfo;
import org.qtjambi.qt.QtPropertyReader;
import org.qtjambi.qt.QtPropertyResetter;
import org.qtjambi.qt.QtPropertyWriter;
import org.qtjambi.qt.core.QByteArray;
import org.qtjambi.qt.core.QCoreApplication;
import org.qtjambi.qt.core.QEvent;
import org.qtjambi.qt.core.QEventLoop;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.core.QRect;
import org.qtjambi.qt.core.QRectF;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.QUrl;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.gui.QDesktopServices;
import org.qtjambi.qt.widgets.QGraphicsScene;
import org.qtjambi.qt.widgets.QLineEdit;
import org.qtjambi.qt.widgets.QPushButton;
import org.qtjambi.qt.widgets.QWidget;
import org.qtjambi.qt.internal.QtJambiRuntime;
import org.qtjambi.qt.osinfo.OSInfo;

class SignalsAndSlotsSubclass extends SignalsAndSlots
{
    public int java_slot2_called = 0;
    public int java_slot3_2_called = 0;
    public int java_slot4_called = 0;
    public int java_non_slot_called = 0;
    public int java_slot5_called = 0;

    public Signal0 signal4 = new Signal0();

    public class NonQObjectSubclass {
        public Signal1<String> s = new Signal1<String>();
    }

    public NonQObjectSubclass getSubclassObject() {
        return new NonQObjectSubclass();
    }

    public void emit_signal_4() { signal4.emit(); }

    public void slot4() { java_slot4_called++; }

    @Override
    public void slot2(int i)
    {
        java_slot2_called += i * 2;
        super.slot2(3);
    }

    public void slot3_2(String k)
    {
        java_slot3_2_called += Integer.parseInt(k);
    }

    public void unnormalized_signature(String s, int i)
    {
        java_slot5_called = i * 3;
    }

    @QtBlockedSlot()
    public void non_slot() {
        java_non_slot_called ++;
    }

}

class NonQObject {
    private void foobar(String foo, int bar) {
        receivedBar = bar;
        receivedFoo = foo;
    }

    public String receivedFoo;
    public int receivedBar;
}

public class TestConnections extends QApplicationTest implements Qt
{
    public TestConnections()
    {
    }

    @Before
    public void setUp() {
        // This class is known to fail when we messed with this setting in a previous testcase running in the same JVM
        // The method run_queuedConnection() in particular
        assertEquals("getObjectCacheMode != DEFAULT", QtJambiRuntime.OBJECT_CACHE_MODE_DEFAULT, QtJambiRuntime.getObjectCacheMode());
    }

    @Test public void run_signalInNonQObject() {
        SignalsAndSlotsSubclass b = new SignalsAndSlotsSubclass();

        Exception e = null;
        try {
            SignalsAndSlotsSubclass.NonQObjectSubclass sc = b.getSubclassObject();
            sc.s.connect(this, "reciveSignal(String)");
        } catch (RuntimeException f) {
            e = f;
        }

        assertTrue(e != null);
        assertEquals(e.getMessage(), "Signals must be declared as members of QSignalEmitter subclasses");
    }

    public void reciveSignal(String s) {

    }

    class SignalEmitter extends QSignalEmitter {
        public Signal2<String, Integer> mySignal = new Signal2<String, Integer>();
    }

    @Test public void run_connectFromNonQObjects() {
        NonQObject nonQObject = new NonQObject();
        SignalEmitter signalEmitter = new SignalEmitter();

        signalEmitter.mySignal.connect(nonQObject, "foobar(String, int)");

        signalEmitter.mySignal.emit("one-two-three", 123);

        assertEquals("one-two-three", nonQObject.receivedFoo);
        assertEquals(123, nonQObject.receivedBar);
    }

    @Test public void run_connectToNonQObjects() {
        SignalsAndSlotsSubclass sas = new SignalsAndSlotsSubclass();
        NonQObject o = new NonQObject();

        sas.signal6.connect(o, "foobar(String, int)");
        sas.signal6.emit("I have this many friends:", 100);

        assertEquals(o.receivedFoo, "I have this many friends:");
        assertEquals(o.receivedBar, 100);
    }

    @Test public void run_shouldAlsoConnectToUnnormalizedSignatures() {
        SignalsAndSlotsSubclass sas = new SignalsAndSlotsSubclass();
        assertEquals(0, sas.java_slot5_called);
        assertEquals(0, sas.slot1_1_called());

        sas.connectSignal6ToUnnormalizedSignature();
        sas.signal6.connect(sas, "slot1_1()");
        sas.signal6.emit("abc", 11);

        assertEquals(sas.java_slot5_called, 33);
        assertEquals(sas.slot1_1_called(), 1);
    }

    @Test public void run_shouldNotConnectToBlockedSlots()
    {
        SignalsAndSlotsSubclass sas = new SignalsAndSlotsSubclass();

        QNoSuchSlotException f = null;
        try {
            sas.signal1.connect(sas, "non_slot()");
        } catch (QNoSuchSlotException e) {
            f = e;
        }

        assertTrue(f != null);

        assertEquals(0, sas.java_non_slot_called);
        sas.signal1.emit();
        assertEquals(0, sas.java_non_slot_called);
    }

    @Test public void run_unknownConnectionShouldFail() {
        SignalsAndSlots sas = new SignalsAndSlots();

        assertTrue(!sas.connectSignal1ToSlot4());
        assertTrue(!sas.connectSignal4ToSlot1_1());
    }

    @Test public void run_unknownCppConnectionShouldConnectInJavaAnyway() {
        SignalsAndSlotsSubclass sas = new SignalsAndSlotsSubclass();

        assertTrue(sas.connectSignal1ToSlot4());

        assertEquals(sas.java_slot4_called, 0);
        sas.emit_signal_1();
        assertEquals(sas.java_slot4_called, 1);
        sas.signal1.emit();
        assertEquals(sas.java_slot4_called, 2);

        sas = new SignalsAndSlotsSubclass();
        assertTrue(sas.connectSignal4ToSlot1_1());
        assertEquals(0, sas.slot1_1_called());
        sas.signal4.emit();
        assertEquals(1, sas.slot1_1_called());

        sas = new SignalsAndSlotsSubclass();
        assertTrue(sas.connectSignal4ToSlot4());
        assertEquals(0, sas.java_slot4_called);
        sas.signal4.emit();
        assertEquals(1, sas.java_slot4_called);

    }


    @Test public void run_cppCreatedShouldMapToJava() {
        SignalsAndSlots sas = SignalsAndSlots.createConnectedObject();
        assertTrue(sas != null);

        assertEquals(sas.slot1_1_called(), 0);
        sas.signal1.emit();
        assertEquals(sas.slot1_1_called(), 1);
        sas.emit_signal_1();
        assertEquals(sas.slot1_1_called(), 2);

        sas.signal1.connect(sas, "slot1_2()");

        assertEquals(sas.slot1_2_called(), 0);
        sas.signal1.emit();
        assertEquals(sas.slot1_1_called(), 3);
        assertEquals(sas.slot1_2_called(), 1);
        sas.emit_signal_1();
        assertEquals(sas.slot1_1_called(), 4);
        assertEquals(sas.slot1_2_called(), 2);
    }

    @Test public void run_cppEmitShouldEmitJavaSignal() {
        SignalsAndSlotsSubclass sas = new SignalsAndSlotsSubclass();

        sas.signal1.connect(sas, "slot4()");
        sas.signal1.connect(sas, "slot4()");

        assertEquals(sas.java_slot4_called, 0);
        sas.emit_signal_1();
        assertEquals(sas.java_slot4_called, 2);
    }

    @Test public void run_javaEmitShouldEmitCppSignal() {
        SignalsAndSlots sas = new SignalsAndSlots();

        sas.connectSignal1ToSlot1_1();
        sas.connectSignal1ToSlot1_1();

        assertEquals(sas.slot1_1_called(), 0);
        sas.signal1.emit();
        assertEquals(sas.slot1_1_called(), 2);
    }

    @Test public void run_javaDisconnectShouldDisconnectCpp() {
        SignalsAndSlots sas = new SignalsAndSlots();

        sas.connectSignal1ToSlot1_1();

        sas.signal1.disconnect(sas, "slot1_1()");

        assertEquals(sas.slot1_1_called(), 0);
        sas.emit_signal_1();
        assertEquals(sas.slot1_1_called(), 0);
        sas.signal1.emit();
        assertEquals(sas.slot1_1_called(), 0);
    }

    @Test public void run_cppDisconnectShouldDisconnectJava() {
        SignalsAndSlots sas = new SignalsAndSlots();

        sas.signal1.connect(sas, "slot1_1()");

        sas.disconnectSignal1FromSlot1_1();

        assertEquals(sas.slot1_1_called(), 0);
        sas.signal1.emit();
        assertEquals(sas.slot1_1_called(), 0);
        sas.emit_signal_1();
        assertEquals(sas.slot1_1_called(), 0);
    }

    @Test public void run_cppDisconnectAllShouldDisconnectJava() {
        SignalsAndSlotsSubclass sas = new SignalsAndSlotsSubclass();

        sas.signal1.connect(sas, "slot4()");
        sas.signal1.connect(sas, "slot1_1()");
        sas.signal2.connect(sas, "slot2(int)");
        sas.disconnectAllFromObject();

        assertEquals(sas.slot1_1_called(), 0);
        assertEquals(sas.java_slot4_called, 0);
        sas.signal1.emit();
        sas.emit_signal_1();
        assertEquals(sas.slot1_1_called(), 0);
        assertEquals(sas.java_slot4_called, 0);

        assertEquals(sas.slot2_called(), 0);
        sas.emit_signal_2(10);
        sas.signal2.emit(10);
        assertEquals(sas.slot2_called(), 0);
    }

    @Test public void run_javaDisconnectAllShouldDisconnectCpp() {
        SignalsAndSlots sas = new SignalsAndSlots();

        sas.connectSignal1ToSlot1_1();
        sas.connectSignal1ToSlot1_1();
        sas.connectSignal2ToSlot2();
        sas.disconnect();

        assertEquals(sas.slot1_1_called(), 0);
        sas.emit_signal_1();
        sas.signal1.emit();
        assertEquals(sas.slot1_1_called(), 0);

        assertEquals(sas.slot2_called(), 0);
        sas.emit_signal_2(10);
        sas.signal2.emit(10);
        assertEquals(sas.slot2_called(), 0);
    }

    @Test public void run_javaDisconnectAllShouldDisconnectJava() {
        SignalsAndSlotsSubclass sas = new SignalsAndSlotsSubclass();

        sas.signal1.connect(sas, "slot1_1()");
        sas.signal1.connect(sas, "slot4()");
        sas.signal2.connect(sas, "slot2(int)");
        sas.disconnect();

        assertEquals(sas.slot1_1_called(), 0);
        assertEquals(sas.java_slot4_called, 0);
        sas.emit_signal_1();
        sas.signal1.emit();
        assertEquals(sas.slot1_1_called(), 0);
        assertEquals(sas.java_slot4_called, 0);

        assertEquals(sas.slot2_called(), 0);
        sas.emit_signal_2(10);
        sas.signal2.emit(10);
        assertEquals(sas.slot2_called(), 0);
    }

    @Test public void run_cppDisconnectSignalShouldDisconnectJava() {
        SignalsAndSlotsSubclass sas = new SignalsAndSlotsSubclass();

        sas.signal1.connect(sas, "slot4()");
        sas.signal1.connect(sas, "slot1_1()");
        sas.signal2.connect(sas, "slot2(int)");

        sas.disconnectAllFromSignal1();

        assertEquals(sas.slot1_1_called(), 0);
        assertEquals(sas.java_slot4_called, 0);
        sas.emit_signal_1();
        sas.signal1.emit();
        assertEquals(sas.slot1_1_called(), 0);
        assertEquals(sas.java_slot4_called, 0);

        assertEquals(sas.slot2_called(), 0);
        sas.emit_signal_2(10);
        sas.signal2.emit(11);
        assertEquals(sas.slot2_called(), 6);
        assertEquals(sas.java_slot2_called, 21 * 2);
    }

    @Test public void run_javaDisconnectSignalShouldDisconnectJava() {
        SignalsAndSlotsSubclass sas = new SignalsAndSlotsSubclass();

        sas.signal1.connect(sas, "slot4()");
        sas.signal1.connect(sas, "slot1_1()");
        sas.signal2.connect(sas, "slot2(int)");

        sas.signal1.disconnect();

        assertEquals(sas.slot1_1_called(), 0);
        assertEquals(sas.java_slot4_called, 0);
        sas.emit_signal_1();
        sas.signal1.emit();
        assertEquals(sas.slot1_1_called(), 0);
        assertEquals(sas.java_slot4_called, 0);

        assertEquals(sas.slot2_called(), 0);
        sas.emit_signal_2(10);
        sas.signal2.emit(11);
        assertEquals(sas.java_slot2_called, 21*2);
        assertEquals(sas.slot2_called(), 6);
    }

    @Test public void run_javaDisconnectSignalShouldDisconnectCpp() {
        SignalsAndSlotsSubclass sas = new SignalsAndSlotsSubclass();

        sas.connectSignal1ToSlot1_1();
        sas.connectSignal2ToSlot2();

        sas.signal1.disconnect();

        assertEquals(sas.slot2_called(), 0);
        sas.emit_signal_2(10);
        sas.signal2.emit(11);
        assertEquals(sas.java_slot2_called, 21*2);
        assertEquals(sas.slot2_called(), 6);

        assertEquals(sas.slot1_1_called(), 0);
        assertEquals(sas.java_slot4_called, 0);
        sas.emit_signal_1();
        sas.signal1.emit();
        assertEquals(sas.slot1_1_called(), 0);
        assertEquals(sas.java_slot4_called, 0);

    }

    @Test public void run_cppDisconnectReceiverShouldDisconnectJava()
    {
        SignalsAndSlotsSubclass sas = new SignalsAndSlotsSubclass();
        SignalsAndSlots sas2 = new SignalsAndSlots();

        sas.signal1.connect(sas2, "slot1_1()");
        sas.signal2.connect(sas2, "slot2(int)");
        sas.signal1.connect(sas, "slot1_1()");
        sas.signal2.connect(sas, "slot2(int)");

        sas.disconnectAllFromReceiver(sas2);

        assertEquals(sas.slot1_1_called(), 0);
        assertEquals(sas2.slot1_1_called(), 0);
        sas.signal1.emit();
        assertEquals(sas.slot1_1_called(), 1);
        assertEquals(sas2.slot1_1_called(), 0);
        sas.emit_signal_1();
        assertEquals(sas.slot1_1_called(), 2);
        assertEquals(sas2.slot1_1_called(), 0);
    }

    @Test public void run_javaDisconnectReceiverShouldDisconnectJava() {
        SignalsAndSlotsSubclass sas = new SignalsAndSlotsSubclass();
        SignalsAndSlots sas2 = new SignalsAndSlots();

        sas.signal1.connect(sas2, "slot1_1()");
        sas.signal2.connect(sas2, "slot2(int)");
        sas.signal1.connect(sas, "slot1_1()");
        sas.signal2.connect(sas, "slot2(int)");

        sas.disconnect(sas2);

        assertEquals(sas.slot1_1_called(), 0);
        assertEquals(sas2.slot1_1_called(), 0);
        sas.signal1.emit();
        assertEquals(sas.slot1_1_called(), 1);
        assertEquals(sas2.slot1_1_called(), 0);
        sas.emit_signal_1();
        assertEquals(sas.slot1_1_called(), 2);
        assertEquals(sas2.slot1_1_called(), 0);
    }


    @Test public void run_javaDisconnectReceiverShouldDisconnectCpp() {
        SignalsAndSlotsSubclass sas = new SignalsAndSlotsSubclass();

        sas.connectSignal1ToSlot1_1();
        sas.connectSignal1ToSlot1_1();
        sas.connectSignal2ToSlot2();

        sas.disconnect(sas);

        assertEquals(sas.slot1_1_called(), 0);
        sas.signal1.emit();
        assertEquals(sas.slot1_1_called(), 0);
        sas.emit_signal_1();
        assertEquals(sas.slot1_1_called(), 0);
        assertEquals(sas.slot2_called(), 0);
        sas.signal2.emit(10);
        assertEquals(sas.slot2_called(), 0);
        sas.emit_signal_2(11);
        assertEquals(sas.slot2_called(), 0);
    }

    @Test public void run_cppDisconnectReceiverFromSignalShouldDisconnectJava() {
        SignalsAndSlots sas = new SignalsAndSlots();
        SignalsAndSlots sas2 = new SignalsAndSlots();

        sas.signal1.connect(sas, "slot1_1()");
        sas.signal1.connect(sas2, "slot1_1()");

        sas.disconnectReceiverFromSignal1(sas2);

        assertEquals(sas.slot1_1_called(), 0);
        assertEquals(sas2.slot1_1_called(), 0);
        sas.signal1.emit();
        sas.emit_signal_1();
        assertEquals(sas.slot1_1_called(), 2);
        assertEquals(sas2.slot1_1_called(), 0);
    }

    @Test public void run_javaDisconnectReceiverFromSignalShouldDisconnectJava() {
        SignalsAndSlots sas = new SignalsAndSlots();
        SignalsAndSlots sas2 = new SignalsAndSlots();

        sas.signal1.connect(sas, "slot1_1()");
        sas.signal1.connect(sas2, "slot1_1()");

        sas.signal1.disconnect(sas2);

        assertEquals(sas.slot1_1_called(), 0);
        assertEquals(sas2.slot1_1_called(), 0);
        sas.signal1.emit();
        sas.emit_signal_1();
        assertEquals(sas.slot1_1_called(), 2);
        assertEquals(sas2.slot1_1_called(), 0);
    }

    @Test public void run_javaDisconnectReceiverFromSignalShouldDisconnectCpp() {
        SignalsAndSlots sas = new SignalsAndSlots();
        SignalsAndSlots sas2 = new SignalsAndSlots();

        sas.connectSignal1ToSlot1_1In(sas2);
        sas.connectSignal1ToSlot1_1();

        sas.signal1.disconnect(sas2);

        assertEquals(sas.slot1_1_called(), 0);
        assertEquals(sas2.slot1_1_called(), 0);
        sas.signal1.emit();
        sas.emit_signal_1();
        assertEquals(sas.slot1_1_called(), 2);
        assertEquals(sas2.slot1_1_called(), 0);
    }

    @Test public void run_CppSignals()
    {
        {
            SignalsAndSlots obj1 = new SignalsAndSlots();
            assertEquals(obj1.slot1_1_called(), 0);
            assertEquals(obj1.slot1_2_called(), 0);
            assertEquals(obj1.slot1_3_called(), 0);
            assertEquals(obj1.slot2_called(), 0);
            assertEquals(obj1.slot3_called(), 0);

            SignalsAndSlots obj2 = new SignalsAndSlots();
            assertEquals(obj2.slot1_1_called(), 0);
            assertEquals(obj2.slot1_2_called(), 0);
            assertEquals(obj2.slot1_3_called(), 0);
            assertEquals(obj2.slot2_called(), 0);
            assertEquals(obj2.slot3_called(), 0);

            obj1.setupSignals(obj2, 0);
            assertEquals(obj1.slot1_1_called(), 0);
            assertEquals(obj1.slot1_2_called(), 0);
            assertEquals(obj1.slot1_3_called(), 0);
            assertEquals(obj1.slot2_called(), 0);
            assertEquals(obj1.slot3_called(), 0);
            assertEquals(obj2.slot1_1_called(), 0);
            assertEquals(obj2.slot1_2_called(), 0);
            assertEquals(obj2.slot1_3_called(), 0);
            assertEquals(obj2.slot2_called(), 0);
            assertEquals(obj2.slot3_called(), 0);

            SignalsAndSlots obj3 = new SignalsAndSlots();
            obj1.setupSignals(obj3, 1);
            assertEquals(obj1.slot1_1_called(), 0);
            assertEquals(obj1.slot1_2_called(), 0);
            assertEquals(obj1.slot1_3_called(), 0);
            assertEquals(obj1.slot2_called(), 0);
            assertEquals(obj1.slot3_called(), 0);
            assertEquals(obj3.slot1_1_called(), 0);
            assertEquals(obj3.slot1_2_called(), 0);
            assertEquals(obj3.slot1_3_called(), 0);
            assertEquals(obj3.slot2_called(), 0);
            assertEquals(obj3.slot3_called(), 0);

            obj1.signal1.connect(obj1, "slot1_1()");
            obj1.signal1.connect(obj1, "slot1_2()");
            obj1.signal1.connect(obj1, "slot1_3()");
            obj1.signal2.connect(obj1, "slot2(int)");
            obj1.signal3.connect(obj1, "slot3(String)");
            assertEquals(obj1.slot1_1_called(), 0);
            assertEquals(obj1.slot1_2_called(), 0);
            assertEquals(obj1.slot1_3_called(), 0);
            assertEquals(obj1.slot2_called(), 0);
            assertEquals(obj1.slot3_called(), 0);
            assertEquals(obj2.slot1_1_called(), 0);
            assertEquals(obj2.slot1_2_called(), 0);
            assertEquals(obj2.slot1_3_called(), 0);
            assertEquals(obj2.slot2_called(), 0);
            assertEquals(obj2.slot3_called(), 0);

            Accessor.emit_signal(obj1.signal1);
            assertEquals(obj1.slot1_1_called(), 1);
            assertEquals(obj1.slot1_2_called(), 1);
            assertEquals(obj1.slot1_3_called(), 1);

            assertEquals(obj2.slot1_1_called(), 1);
            assertEquals(obj2.slot1_2_called(), 0);
            assertEquals(obj2.slot1_3_called(), 0);

            assertEquals(obj3.slot1_1_called(), 1);
            assertEquals(obj3.slot1_2_called(), 1);
            assertEquals(obj3.slot1_3_called(), 0);

            Accessor.emit_signal(obj1.signal1);
            assertEquals(obj1.slot1_1_called(), 2);
            assertEquals(obj1.slot1_2_called(), 2);
            assertEquals(obj1.slot1_3_called(), 2);

            assertEquals(obj2.slot1_1_called(), 2);
            assertEquals(obj2.slot1_2_called(), 0);
            assertEquals(obj2.slot1_3_called(), 0);

            assertEquals(obj3.slot1_1_called(), 2);
            assertEquals(obj3.slot1_2_called(), 2);
            assertEquals(obj3.slot1_3_called(), 0);

            obj1.signal1.connect(obj2, "slot1_1()");
            obj1.signal1.connect(obj1, "slot1_2()");

            Accessor.emit_signal(obj1.signal1);
            assertEquals(obj1.slot1_1_called(), 3);
            assertEquals(obj1.slot1_2_called(), 4);
            assertEquals(obj1.slot1_3_called(), 3);

            assertEquals(obj2.slot1_1_called(), 4);
            assertEquals(obj2.slot1_2_called(), 0);
            assertEquals(obj2.slot1_3_called(), 0);

            assertEquals(obj3.slot1_1_called(), 3);
            assertEquals(obj3.slot1_2_called(), 3);
            assertEquals(obj3.slot1_3_called(), 0);

            obj1.dispose();
            obj2.dispose();
            obj3.dispose();
        }

        {
            SignalsAndSlots obj1 = new SignalsAndSlots();
            obj1.setupSignals(obj1, 2);

            obj1.signal3.connect(obj1, "slot3(String)");

            obj1.signal2.emit(15);
            assertEquals(obj1.slot1_1_called(), 0);
            assertEquals(obj1.slot1_2_called(), 0);
            assertEquals(obj1.slot1_3_called(), 0);
            assertEquals(obj1.slot2_called(), 15);
            assertEquals(obj1.slot3_called(), 0);

            obj1.emit_signal_2(20);
            assertEquals(obj1.slot1_1_called(), 0);
            assertEquals(obj1.slot1_2_called(), 0);
            assertEquals(obj1.slot1_3_called(), 0);
            assertEquals(obj1.slot2_called(), 35);
            assertEquals(obj1.slot3_called(), 0);

            obj1.emit_signal_3("11");
            assertEquals(obj1.slot1_1_called(), 0);
            assertEquals(obj1.slot1_2_called(), 0);
            assertEquals(obj1.slot1_3_called(), 0);
            assertEquals(obj1.slot2_called(), 35);
            assertEquals(obj1.slot3_called(), 22);

            obj1.dispose();
        }

        {
            SignalsAndSlotsSubclass obj1 = new SignalsAndSlotsSubclass();


            obj1.signal2.connect(obj1, "slot2(int)");

            obj1.setupSignals(obj1, 2);

            obj1.emit_signal_2(13);
            assertEquals(obj1.java_slot2_called, 52);
            assertEquals(obj1.slot2_called(), 6);

            Accessor.emit_signal(obj1.signal2, 4);
            assertEquals(obj1.java_slot2_called, 68);
            assertEquals(obj1.slot2_called(), 12);

            obj1.signal3.connect(obj1, "slot3_2(String)");
            obj1.signal3.connect(obj1, "slot3_2(String)");

            obj1.emit_signal_3("17");
            assertEquals(obj1.java_slot3_2_called, 34);
            assertEquals(obj1.slot3_called(), 17);

            obj1.signal4.connect(obj1, "slot4()");
            obj1.signal1.connect(obj1.signal4);

            obj1.emit_signal_1();
            assertEquals(obj1.slot1_1_called(), 1);
            assertEquals(obj1.slot1_2_called(), 1);
            assertEquals(obj1.slot1_3_called(), 1);
            assertEquals(obj1.java_slot4_called, 1);

            assertTrue(obj1.signal1.disconnect(obj1.signal4));

            obj1.signal4.connect(obj1.signal1);
            obj1.emit_signal_4();
            assertEquals(obj1.java_slot4_called, 2);
            assertEquals(obj1.slot1_1_called(), 2);
            assertEquals(obj1.slot1_2_called(), 2);
            assertEquals(obj1.slot1_3_called(), 2);

            obj1.disconnectSignals(obj1);


            obj1.emit_signal_1();
            assertEquals(obj1.slot1_1_called(), 2);
            assertEquals(obj1.slot1_2_called(), 3);
            assertEquals(obj1.slot1_3_called(), 3);
            assertEquals(obj1.java_slot4_called, 2);

            obj1.emit_signal_4();
            assertEquals(obj1.slot1_1_called(), 2);
            assertEquals(obj1.slot1_2_called(), 4);
            assertEquals(obj1.slot1_3_called(), 4);
            assertEquals(obj1.java_slot4_called, 3);

            obj1.disconnectAllFromObject();

            obj1.emit_signal_4();
            obj1.emit_signal_1();
            assertEquals(obj1.slot1_1_called(), 2);
            assertEquals(obj1.slot1_2_called(), 4);
            assertEquals(obj1.slot1_3_called(), 4);
            assertEquals(obj1.java_slot4_called, 3);

        }
    }

    Signal0 destroyed = new Signal0();

    @Test
    public void run_createDestroy() {
        MyQObject something = null;

        QObject parent = new QObject();
        something = new MyQObject();

        destroyed.connect(something, "increaseFinalized()");
        MyQObject objects[] = new MyQObject[1000];
        for (int i = 0; i < 1000; ++i) {
            objects[i] = new MyQObject(parent);
            objects[i].destroyed = this.destroyed;

        }

        MyQObject.finalizedCount = 0;

        int count = 1000;

        parent.dispose();
        System.gc();
        try {
            Thread.sleep(600);
        } catch (Exception e) {
        }
        ;
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDelete.value());

        assertEquals(MyQObject.finalizedCount, count);
    }

    @Test public void run_connectJavaQt()
    {
        QWidget widget = new QWidget();

        assertTrue(!widget.isVisible());
        assertTrue(widget.isEnabled());

        MyQObject obj = new MyQObject();
        obj.signalMyQObject.connect(widget, "show()");
        obj.signalNoParams.connect(widget, "hide()");
        obj.signalBoolean.connect(widget, "setEnabled(boolean)");
        obj.signalInteger.connect(widget, "close()");
        obj.javaSignalMyQObject(obj);
        assertTrue(widget.isVisible());

        obj.javaSignalboolean(false);
        assertTrue(!widget.isEnabled());

        obj.javaSignalBoolean(true); // both Boolean-functions emit the signalBoolean signal
        assertTrue(widget.isEnabled());

        obj.javaSignalNoParams();
        assertTrue(!widget.isVisible());

        assertTrue(obj.signalMyQObject.disconnect(widget, "show()"));
        assertTrue(obj.signalNoParams.disconnect(widget, "hide()"));
        assertTrue(obj.signalBoolean.disconnect(widget, "setEnabled(boolean)"));

        obj.javaSignalboolean(false);
        assertTrue(widget.isEnabled());

        obj.javaSignalboolean(false);
        assertTrue(widget.isEnabled());

        obj.javaSignalMyQObject(obj);
        assertTrue(!widget.isVisible());

        widget.show();
        obj.javaSignalNoParams();
        assertTrue(widget.isVisible());

        obj.javaSignalint(10);
        assertTrue(obj.signalInteger.disconnect(widget, "close()"));

        widget = new QWidget();
        QPushButton b1 = new QPushButton(widget);
        QPushButton b2 = new QPushButton(widget);
        QLineEdit le = new QLineEdit(widget);
        assertTrue(!widget.isVisible());
        assertTrue(!b1.isVisible());
        assertTrue(!b2.isVisible());
        assertTrue(!le.isVisible());
        assertTrue(widget.isEnabled());
        assertTrue(b1.isEnabled());
        assertTrue(b2.isEnabled());
        assertTrue(le.isEnabled());

        widget.show();
        QApplication.setActiveWindow(widget);
        assertTrue(widget.isVisible());
        assertTrue(b1.isVisible());
        assertTrue(b2.isVisible());
        assertTrue(le.isVisible());
        widget.setGeometry(new QRect(200, 300, 400, 500));
        QRect rect = widget.geometry();
        assertEquals(rect.x(), 200);
        assertEquals(rect.y(), 300);
        assertEquals(rect.width(), 400);
        assertEquals(rect.height(), 500);
        QSize test = new QSize();
        assertEquals(test.width(), -1);
        assertEquals(test.height(), -1);

        QSize sz = new QSize(10, 20);
        assertEquals(sz.width(), 10);
        assertEquals(sz.height(), 20);
        b1.setIconSize(sz);
        QSize sz2 = b1.iconSize();
        assertEquals(sz2.width(), sz.width());
        assertEquals(sz2.height(), sz.height());

        obj.signalInteger.connect(b1, "setFocus()");
        obj.signalNoParams.connect(b2, "setFocus()");
        obj.signalDouble.connect(b1, "hide()");
        obj.signalDouble.connect(b2, "hide()");
        obj.signalDoubleInteger.connect(widget, "close()");
        obj.signalLong.connect(b1, "show()");
        obj.signalLong.connect(b2, "show()");
        obj.signalMixed1.connect(widget, "hide()");
        obj.signalString.connect(le, "setText(String)");
        le.textChanged.connect(obj, "javaSlotString(String)");
        QApplication.instance().focusChanged.connect(obj, "javaSlotFocusChanged(QWidget, QWidget)");
        obj.signalQSize.connect(b2, "setIconSize(QSize)");

        obj.javaSignalint(123);

        assertEquals(QApplication.focusWidget(), b1);
        assertTrue(QApplication.focusWidget() == b1);
        obj.javaSignalNoParams();
        assertEquals(QApplication.focusWidget(), b2);
        assertTrue(QApplication.focusWidget() == b2);
        assertEquals(obj.slotResult, b1); // set by javaSlotFocusChanged()
        assertTrue(obj.slotResult == b1);
        assertEquals(obj.slotResult2, b2); // ditto
        assertTrue(obj.slotResult2 == b2);

        obj.javaSignalDouble(1.0);
        assertTrue(widget.isVisible());
        assertTrue(!b1.isVisible());
        assertTrue(!b2.isVisible());
        assertTrue(le.isVisible());

        obj.javaSignalLong(123L);
        assertTrue(widget.isVisible());
        assertTrue(b1.isVisible());
        assertTrue(b2.isVisible());
        assertTrue(le.isVisible());

        b1.setText("button 1");
        b2.setText("button 2");
        assertEquals(b1.text(), "button 1");
        assertEquals(b2.text(), "button 2");
        assertTrue(!b1.text().equals(b2.text()));

        obj.javaSignalString("Line edit text");
        assertEquals(le.text(), "Line edit text");
        assertEquals(obj.slotResult, "Line edit text");


        assertTrue(obj.signalString.disconnect(le, "setText(String)"));
        obj.signalString.connect(le, "selectAll()");

        obj.javaSignalString("ABC");
        assertEquals(le.text(), "Line edit text");
        assertEquals(obj.slotResult, "Line edit text");

        assertTrue(obj.signalString.disconnect(le, "selectAll()"));
        obj.signalString.connect(le, "cut()");

        obj.javaSignalString("DEF");
        assertEquals(le.text(), "");
        assertEquals(obj.slotResult, "");

        obj.signalString.connect(le, "paste()");
        obj.javaSignalString("GHI");
        assertEquals(le.text(), "Line edit text");
        assertEquals(obj.slotResult, "Line edit text");

        obj.javaSignalTwoParameters(123.0, 456); // the signalDoubleInteger signal
        assertTrue(!widget.isVisible());
        assertTrue(!b1.isVisible());
        assertTrue(!b2.isVisible());

        sz = new QSize(40, 60);
        assertEquals(sz.width(), 40);
        assertEquals(sz.height(), 60);
        // CHECKME: This maybe leaking the QSize object ?
        obj.javaSignalQSize(sz);

        QSize sz3 = b2.iconSize();
        assertEquals(sz3.width(), sz.width());
        assertEquals(sz3.height(), sz.height());
        assertEquals(sz, sz3); // should this be exactly the same instance ?

        {
            obj = new MyQObject();
            MyQObject obj2 = new MyQObject();

            obj.signalBoolean.connect(obj2.signalBoolean);
            obj2.signalBoolean.connect(obj, "javaSlotboolean(boolean)");

            obj.javaSignalboolean(false);
            assertTrue(obj.slotResult instanceof Boolean);
            assertEquals(obj.slotResult, false);

            obj.javaSignalboolean(true);
            assertTrue(obj.slotResult instanceof Boolean);
            assertEquals(obj.slotResult, true);

            obj.blockSignals(true);
            obj.javaSignalboolean(false);
            assertTrue(obj.slotResult instanceof Boolean);
            assertEquals(obj.slotResult, true);

            obj.blockSignals(false);
            obj.javaSignalboolean(false);
            assertTrue(obj.slotResult instanceof Boolean);
            assertEquals(obj.slotResult, false);

            obj2.blockSignals(true);
            obj.javaSignalboolean(true);
            assertTrue(obj.slotResult instanceof Boolean);
            assertEquals(obj.slotResult, false);
        }
    }

     public void test_javaToJavaConnect(String name, MyQObject sender,
             Object signal,
             String signalFunctionName,
             MyQObject receiver,
             String slot,
             Object[] parameters,
             Object expectedSlotResult,
             Boolean checkReturnedReference,
             Class<?>[] parameterTypes)
     {

        ((QSignalEmitter.AbstractSignal)signal).connect(receiver, slot);
        if (parameters == null)
            parameters = new Object[0];

        try {
            Method m = sender.getClass().getMethod(signalFunctionName, parameterTypes);
            m.invoke(sender, parameters);
        } catch (Throwable e) {

            if (e instanceof InvocationTargetException) {
                InvocationTargetException ite = (InvocationTargetException)e;
                e = ite.getTargetException();
            }

            fail(e.toString());
        }

        assertEquals(expectedSlotResult, receiver.slotResult);
        assertTrue(!checkReturnedReference.booleanValue() || receiver.slotResult == expectedSlotResult);
     }

     @Test
     public void run_javaToJavaConnect()
     {

        Class<?>[] parameterTypes = new Class[1];
        parameterTypes[0] = Long.class;

        Object[] params = new Object[1];
        params[0] = Long.MAX_VALUE;

        MyQObject sender = new MyQObject();
        MyQObject receiver = new MyQObject();
        test_javaToJavaConnect("Long signal/slot MAX_VALUE", sender, sender.signalLong, "javaSignalLong",
                   receiver, "javaSlotLong(Long)", params, params[0], true, parameterTypes);

        params = new Object[1];
        params[0] = Long.MIN_VALUE;
        sender = new MyQObject();
        receiver = new MyQObject();
        test_javaToJavaConnect("Long signal/slot MIN_VALUE", sender, sender.signalLong, "javaSignalLong",
                receiver, "javaSlotLong(Long)", params, params[0], true, parameterTypes);

        parameterTypes = new Class[1];
        parameterTypes[0] = Integer.class;

        params = new Object[1];
        params[0] = Integer.MAX_VALUE;
        test_javaToJavaConnect("Integer signal/slot MAX_VALUE", sender, sender.signalInteger, "javaSignalInteger",
                   receiver, "javaSlotInteger(Integer)", params, params[0], true, parameterTypes);

        params = new Object[1];
        params[0] = Integer.MIN_VALUE;
        sender = new MyQObject();
        receiver = new MyQObject();
        test_javaToJavaConnect("Integer signal/slot MIN_VALUE", sender, sender.signalInteger, "javaSignalInteger",
                receiver, "javaSlotInteger(Integer)", params, params[0], true, parameterTypes);

        parameterTypes = new Class[1];
        parameterTypes[0] = Boolean.class;

        params = new Object[1];
        params[0] = true;
        test_javaToJavaConnect("Boolean signal/slot true", sender, sender.signalBoolean, "javaSignalBoolean",
                receiver, "javaSlotBoolean(Boolean)", params, params[0], true, parameterTypes);

        params = new Object[1];
        params[0] = false;
        sender = new MyQObject();
        receiver = new MyQObject();
        test_javaToJavaConnect("Boolean signal/slot false", sender, sender.signalBoolean, "javaSignalBoolean",
                receiver, "javaSlotBoolean(Boolean)", params, params[0], true, parameterTypes);

        parameterTypes = new Class[1];
        parameterTypes[0] = Double.class;

        params = new Object[1];
        params[0] = Double.MAX_VALUE;
        test_javaToJavaConnect("Double signal/slot MAX_VALUE", sender, sender.signalDouble, "javaSignalDouble",
                receiver, "javaSlotDouble(Double)", params, params[0], true, parameterTypes);

        params = new Object[1];
        params[0] = Double.MIN_VALUE;
        sender = new MyQObject();
        receiver = new MyQObject();
        test_javaToJavaConnect("Double signal/slot MIN_VALUE", sender, sender.signalDouble, "javaSignalDouble",
                receiver, "javaSlotDouble(Double)", params, params[0], true, parameterTypes);

        parameterTypes = new Class[1];
        parameterTypes[0] = Short.class;

        params = new Object[1];
        params[0] = Short.MAX_VALUE;
        test_javaToJavaConnect("Short signal/slot MAX_VALUE", sender, sender.signalShort, "javaSignalShort",
                receiver, "javaSlotShort(Short)", params, params[0], true, parameterTypes);

        params = new Object[1];
        params[0] = Short.MIN_VALUE;
        sender = new MyQObject();
        receiver = new MyQObject();
        test_javaToJavaConnect("Short signal/slot MIN_VALUE", sender, sender.signalShort, "javaSignalShort",
                receiver, "javaSlotShort(Short)", params, params[0], true, parameterTypes);

        parameterTypes = new Class[1];
        parameterTypes[0] = Float.class;

        params = new Object[1];
        params[0] = Float.MAX_VALUE;
        test_javaToJavaConnect("Float signal/slot MAX_VALUE", sender, sender.signalFloat, "javaSignalFloat",
                receiver, "javaSlotFloat(Float)", params, params[0], true, parameterTypes);

        params = new Object[1];
        params[0] = Float.MIN_VALUE;
        sender = new MyQObject();
        receiver = new MyQObject();
        test_javaToJavaConnect("Float signal/slot MIN_VALUE", sender, sender.signalFloat, "javaSignalFloat",
                receiver, "javaSlotFloat(Float)", params, params[0], true, parameterTypes);

        parameterTypes = new Class[1];
        parameterTypes[0] = Character.class;

        params = new Object[1];
        params[0] = Character.MAX_VALUE;
        test_javaToJavaConnect("Character signal/slot MAX_VALUE", sender, sender.signalCharacter, "javaSignalCharacter",
                receiver, "javaSlotCharacter(Character)", params, params[0], true, parameterTypes);

        params = new Object[1];
        params[0] = Character.MIN_VALUE;
        sender = new MyQObject();
        receiver = new MyQObject();
        test_javaToJavaConnect("Character signal/slot MIN_VALUE", sender, sender.signalCharacter, "javaSignalCharacter",
                receiver, "javaSlotCharacter(java.lang.Character)", params, params[0], true, parameterTypes);

        params = new Object[1];
        params[0] = new MyQObject();
         parameterTypes = new Class[1];
        parameterTypes[0] = params[0].getClass();
        test_javaToJavaConnect("MyQObject signal/slot", sender, sender.signalMyQObject, "javaSignalMyQObject",
                receiver, "javaSlotMyQObject(MyQObject)", params, params[0], true, parameterTypes);

       // Signal has more parameters than slot
       parameterTypes = new Class[2];
       parameterTypes[0] = double.class;
       parameterTypes[1] = int.class;
       params = new Object[2];
       params[0] = Double.MIN_VALUE;
       params[1] = Integer.MAX_VALUE;
       test_javaToJavaConnect("Signal two parameters/slot one parameter",
                  sender, sender.signalDoubleInteger, "javaSignalTwoParameters",
                  receiver, "javaSlotdouble(double)", params, params[0], false, parameterTypes);

       // No parameters
       parameterTypes = new Class[0];
       sender = new MyQObject();
       receiver = new MyQObject();
       test_javaToJavaConnect("No parameters, void returning signal/slot",
                  sender, sender.signalNoParams, "javaSignalNoParams",
                  receiver, "javaSlotNoParams()", null, "Slot no params", false, parameterTypes);

       // Mixed parameter types (PODs and complex types)
       parameterTypes = new Class[4];
       parameterTypes[0] = String.class;
       parameterTypes[1] = double.class;
       parameterTypes[2] = int.class;
       parameterTypes[3] = Integer.class;
       params = new Object[4];
       params[0] = "A String";
       params[1] = Double.MAX_VALUE;
       params[2] = Integer.MAX_VALUE;
       params[3] = Integer.MAX_VALUE;
       test_javaToJavaConnect("Mixed parameters, returning Integer",
                  sender, sender.signalMixed1, "javaSignalMixed1",
                  receiver, "javaSlotMixed1(String, double, int, Integer)", params, params[0], true, parameterTypes);

       // QObject array
       parameterTypes = new Class[1];
       parameterTypes[0] = QObject[].class;
       params = new Object[1];
       QObject array[] = new QObject[10];
       for (int i=0; i<10; ++i)
           array[i] = new QObject(sender);
       params[0] = array;
       test_javaToJavaConnect("QObject array", sender, sender.signalQObjectArray, "javaSignalQObjectArray",
               receiver, "javaSlotQObjectArray(QObject [])", params, params[0], true, parameterTypes);

       // Many arrays
       parameterTypes = new Class[3];
       Integer[][][] param = new Integer[1][1][1];
       parameterTypes[0] = param[0][0].getClass();
       parameterTypes[1] = param[0].getClass();
       parameterTypes[2] = param.getClass();
       params = new Object[3];
       params[0] = param[0][0];
       params[1] = param[0];
       params[2] = param;
       test_javaToJavaConnect("Many integer arrays", sender, sender.signalManyArrays, "javaSignalManyArrays",
               receiver, "javaSlotManyArrays(java.lang.Integer[], Integer [] [], Integer[]   [] [    ])", params, param, true, parameterTypes);

       // Signal -> Signal
       parameterTypes = new Class[1];
       parameterTypes[0] = Integer.class;
       params = new Object[1];
       params[0] = new Integer(654);
       sender = new MyQObject();
       receiver = new MyQObject();
       sender.signalInteger.connect(receiver.signalInteger2);
       test_javaToJavaConnect("Signal to signal to slot", sender, receiver.signalInteger2, "javaSignalInteger",
               receiver, "javaSlotInteger(Integer)", params, params[0], true, parameterTypes);

       // MyQObject -> QObject
       parameterTypes = new Class[1];
       parameterTypes[0] = MyQObject.class;
       params = new Object[1];
       params[0] = new MyQObject();
       sender = new MyQObject();
       receiver = new MyQObject();
       test_javaToJavaConnect("MyQObject -> QObject cast", sender, sender.signalMyQObject, "javaSignalMyQObject",
               receiver, "javaSlotQObject(QObject)", params, params[0], true, parameterTypes);

       // List<String>
       parameterTypes = new Class[1];
       parameterTypes[0] = List.class;
       params = new Object[1];

       LinkedList<String> stringList = new LinkedList<String>();
       stringList.add("You got you got you got what I need");
       params[0] = stringList;
       test_javaToJavaConnect("String list parameter", sender, sender.signalStringList, "javaSignalStringList",
               receiver, "javaSlotStringList(List)", params, stringList.get(0), true, parameterTypes);

       // List<List<String>>
       parameterTypes = new Class[1];
       parameterTypes[0] = List.class;
       List<LinkedList<String>> stringListList = new LinkedList<LinkedList<String>>();
       stringList.add("Now shake that thang");
       stringListList.add(stringList);
       params = new Object[1];
       params[0] = stringListList;
       test_javaToJavaConnect("List of list of String", sender, sender.signalStringListList, "javaSignalStringListList",
               receiver, "javaSlotStringListList(List)", params, stringList.get(1), true, parameterTypes);
     }

     public void test_borkedConnections(String name, MyQObject sender,
             Object signal,
             QObject receiver,
             String slotSignature,
             Class<?> expectedExceptionType,
             String expectedExceptionMessage)
     {

         Class<?> ce = null;
         String msg = null;
         try {
             ((QSignalEmitter.AbstractSignal)signal).connect(receiver, slotSignature);
         } catch (Exception e) {
             ce = e.getClass();
             msg = e.getMessage();
         }
         assertEquals(expectedExceptionType, ce);
         if (expectedExceptionMessage != null)
             assertEquals(msg.substring(0, expectedExceptionMessage.length()), expectedExceptionMessage);
     }

     @Test
     public void run_borkedConnections()
     {

         MyQObject sender = new MyQObject();
         MyQObject receiver = new MyQObject(null);

         test_borkedConnections("Slot signature with return type", sender, sender.signalBoolean,
                    receiver, "Boolean javaSlotBoolean(Boolean)",
                    RuntimeException.class, "Do not specify return type in slot signature");

         test_borkedConnections("Slot signature with missing end parenthesis", sender, sender.signalBoolean,
                    receiver, "javaSlotBoolean(Boolean",
                    RuntimeException.class, "Wrong syntax in slot signature");

         test_borkedConnections("Slot signature with unknown argument type", sender, sender.signalBoolean,
                 receiver, "javaSlotBoolean(ean)",
                 QNoSuchSlotException.class, null);

         test_borkedConnections("Slot signature with unknown slot name", sender, sender.signalBoolean,
                    receiver, "javaSlotBool(Boolean)", QNoSuchSlotException.class, null);

         test_borkedConnections("Null signal", sender, null, receiver, "javaSlotBoolean(Boolean)",
                 NullPointerException.class, null);

         test_borkedConnections("Null slot signature", sender, sender.signalBoolean, receiver, null, NullPointerException.class, null);

         test_borkedConnections("Null receiver", sender, sender.signalBoolean, null, "javaSlotBoolean(Boolean)",
                 NullPointerException.class, null);

         test_borkedConnections("Too many slot params", sender, sender.signalBoolean,
                 receiver, "javaSlotMixed1(String, double, int, Integer)", RuntimeException.class, "Signature of signal");

         test_borkedConnections("Wrong parameter types, narrowing", sender, sender.signalDouble,
                 receiver, "javaSlotInteger(Integer)", RuntimeException.class, "Signature of signal");

         test_borkedConnections("Wrong parameter types, widening", sender, sender.signalInteger,
                 receiver, "javaSlotDouble(Double)", RuntimeException.class, "Signature of signal");
     }

      // Method requires: getObjectCacheMode == DEFAULT
      @Test public void run_queuedConnection() {
          MyQObject sender = new MyQObject();
          MyQObject receiver = new MyQObject();

          sender.signalBoolean.connect(receiver, "javaSlotboolean(boolean)", Qt.ConnectionType.QueuedConnection);
          sender.signalBoolean.connect(receiver, "javaSlotBoolean(java.lang.Boolean)", Qt.ConnectionType.QueuedConnection);
          sender.signalCharacter.connect(receiver, "javaSlotchar(char)", Qt.ConnectionType.QueuedConnection);
          sender.signalCharacter.connect(receiver, "javaSlotCharacter(java.lang.Character)", Qt.ConnectionType.QueuedConnection);
          sender.signalShort.connect(receiver, "javaSlotshort(short)", Qt.ConnectionType.QueuedConnection);
          sender.signalShort.connect(receiver, "javaSlotShort(java.lang.Short)", Qt.ConnectionType.QueuedConnection);
          sender.signalInteger.connect(receiver, "javaSlotint(int)", Qt.ConnectionType.QueuedConnection);
          sender.signalInteger.connect(receiver, "javaSlotInteger(java.lang.Integer)", Qt.ConnectionType.QueuedConnection);
          sender.signalLong.connect(receiver, "javaSlotlong(long)", Qt.ConnectionType.QueuedConnection);
          sender.signalLong.connect(receiver, "javaSlotLong(java.lang.Long)", Qt.ConnectionType.QueuedConnection);
          sender.signalFloat.connect(receiver, "javaSlotfloat(float)", Qt.ConnectionType.QueuedConnection);
          sender.signalFloat.connect(receiver, "javaSlotFloat(java.lang.Float)", Qt.ConnectionType.QueuedConnection);
          sender.signalDouble.connect(receiver, "javaSlotdouble(double)", Qt.ConnectionType.QueuedConnection);
          sender.signalDouble.connect(receiver, "javaSlotDouble(java.lang.Double)", Qt.ConnectionType.QueuedConnection);
          sender.signalString.connect(receiver, "javaSlotString(java.lang.String)", Qt.ConnectionType.QueuedConnection);

          // Boolean
          receiver.slotResult = null;
          sender.javaSignalboolean(true);
          if(QtInfo.version()<0x00040800){
	          if(OSInfo.isMacOS() == false) {
	              // MacOSX: http://www.qtcentre.org/threads/35603-Qt-QueuedConnection-difference-on-platforms
	              // The line below appears to fail on the mac, it appears to be testing that the delivery has
	              // been queued/deferred into the event queue.  But on the mac it appears to be immediately delivered.
	              assertEquals(receiver.slotResult, null);
	          } else {
	//              assertEquals(receiver.slotResult, new Boolean(true));
	          }
	          QCoreApplication.processEvents();
	          if(OSInfo.isMacOS() == false)
	              assertEquals(receiver.slotResult, new Boolean(true));
	          else
	              assertEquals(receiver.slotResult, null);  // huh?
          }else{
        	  assertEquals(null, receiver.slotResult);
        	  QCoreApplication.processEvents();
        	  assertEquals(new Boolean(true), receiver.slotResult);
          }
          sender.javaSignalBoolean(false);
          QCoreApplication.processEvents();
          assertEquals(receiver.slotResult, new Boolean(false));

          // Byte ?

          // Character
          sender.javaSignalchar('x');
          QCoreApplication.processEvents();
          assertEquals(receiver.slotResult, new Character('x'));
          sender.javaSignalCharacter(new Character('y'));
          QCoreApplication.processEvents();
          assertEquals(receiver.slotResult, new Character('y'));

          // Shorts
          sender.javaSignalShort((short) 40);
          QCoreApplication.processEvents();
          assertEquals(receiver.slotResult, new Short((short) 40));
          sender.javaSignalShort((short) 41);
          QCoreApplication.processEvents();
          assertEquals(receiver.slotResult, new Short((short) 41));

          // Integer
          sender.javaSignalint(42);
          QCoreApplication.processEvents();
          assertEquals(receiver.slotResult, new Integer(42));
          sender.javaSignalInteger(43);
          QCoreApplication.processEvents();
          assertEquals(receiver.slotResult, new Integer(43));

          // Long
          sender.javaSignalLong((long)44);
          QCoreApplication.processEvents();
          assertEquals(receiver.slotResult, new Long(44));
          sender.javaSignalLong((long) 45);
          QCoreApplication.processEvents();
          assertEquals(receiver.slotResult, new Long(45));

          // Float
          sender.javaSignalFloat((float) 3.14);
          QCoreApplication.processEvents();
          assertEquals(receiver.slotResult, new Float(3.14));
          sender.javaSignalFloat((float) 3.15);
          QCoreApplication.processEvents();
          assertEquals(receiver.slotResult, new Float(3.15));

          // Double
          sender.javaSignalDouble(3.16);
          QCoreApplication.processEvents();
          assertEquals(receiver.slotResult, new Double(3.16));
          sender.javaSignalDouble(3.17);
          QCoreApplication.processEvents();
          assertEquals(receiver.slotResult, new Double(3.17));

          // Strings
          sender.javaSignalString("once upon a time...");
          QCoreApplication.processEvents();
          assertEquals(receiver.slotResult, "once upon a time...");
      }

    private class ConnectInEmitTester extends QSignalEmitter {
        Signal0 signal = new Signal0();
        boolean gotThusFar;
        public void function() {
            try {
                signal.connect(this, "function()");
                gotThusFar = true;
            } catch (Exception e) { }
        }
    }

    @Test public void testConnectInEmit() {
        ConnectInEmitTester c = new ConnectInEmitTester();
        c.signal.connect(c, "function()");
        c.signal.emit();
        assertTrue(c.gotThusFar);
    }

    class ArrayConnection extends SignalEmitter{

        public Signal1<String> test1 =  new Signal1<String>();
        public Signal1<String[]> test2 = new Signal1<String[]>();
        public Signal1<String[][]> test3 = new Signal1<String[][]>();
        public Signal1<Byte[]> test4 = new Signal1<Byte[]>();

        String[] a = {"one", "two"};
        String[][] b = {a, a};
        Byte[] c = {1, 2, 3};


        public void slot1(String[] str) {
            for (int i = 0; i < str.length; i++) {
                assertEquals(a[i], str[i]);
            }
        }

        public void slot2(String[][] str) {
            for (int i = 0; i < str.length; i++) {
                String[] strings = str[i];
                for (int j = 0; j < strings.length; j++) {
                    assertEquals(a[j], strings[j]);
                }
            }
        }

        public void slot3(Byte[] bb) {
            for (int i = 0; i < bb.length; i++) {
                assertEquals(c[i], bb[i]);
            }
        }
    }

    @Test public void testConnectArrays() {
        ArrayConnection test = new ArrayConnection();

        test.test1.connect(System.out, "println(java.lang.String)");
        test.test2.connect(test, "slot1(String[])");
        test.test3.connect(test, "slot2(java.lang.String[][])");
        test.test4.connect(test, "slot3(Byte[])");

        try {
            test.test3.connect(test, "slot2(String[][][])");
            assertTrue(false);
        } catch (RuntimeException e) {}

        System.out.println("We should print \"hello\" below this line.");
        test.test1.emit("hello");
        System.out.println("We should print \"hello\" above this line.");

        test.test2.emit(test.a);
        test.test3.emit(test.b);
        test.test4.emit(test.c);
    }


    private String path = "not/the/right/path";
    @Test public void testUrlHandler() {
        QDesktopServices.setUrlHandler("superscheme", new QDesktopServices.UrlHandler() {

            public void handleUrl(QUrl url) {
                path = url.path();
            }

        });

        QUrl url = new QUrl("superscheme:host.com/my/super/scheme/path");

        QDesktopServices.openUrl(url);
        assertEquals(url.path(), path);

        QDesktopServices.unsetUrlHandler("superscheme");
        QDesktopServices.setUrlHandler("superscheme", new QDesktopServices.UrlHandler() {

            public void handleUrl(QUrl url) {
                path = url.toDisplayString();
            }

        });
        QDesktopServices.openUrl(new QUrl("superscheme:superhost.com/not/a/valid"));
        assertEquals("superscheme:superhost.com/not/a/valid", path);

        QDesktopServices.setUrlHandler("superscheme", null);
    }

    static class Emitter extends QSignalEmitter implements Runnable {
        Signal0 signal = new Signal0();
        boolean goodExit = false;

        public void run() {
            for (int i=0; i<100000; ++i) {
                signal.emit();
            }
            goodExit = true;
        }

        public void slot1() { }
        public void slot2() { }
    }

    // FIXME: This takes 52 seconds (could be just the volume) but should be moved into a
    //  test on its own so it can be excluded and/or does not negative impact the other
    //  tests.
    @Test public void concurrentEmitConnect() {
        Emitter e = new Emitter();
        Thread t = new Thread(e);
        e.signal.connect(e, "slot1()");
        t.start();

        try {
            for (int i=0; i<100000; ++i) {
                e.signal.connect(e, "slot2()");
                e.signal.disconnect(e, "slot2()");
            }
            t.join();
        } catch(Exception ex) {
            ex.printStackTrace();
        }

        assertTrue(e.goodExit);
        assertFalse(t.isAlive());  // make sure its dead
    }

    @Test public void standardReadWriteResetProperty() {
        SignalsAndSlots sas = new SignalsAndSlots();

        sas.setByteArrayProperty("cppProperty", new QByteArray("it was a dark and stormy night"));
        QByteArray ba = QVariant.toByteArray(sas.property("cppProperty"));
        assertEquals("it was a dark and stormy night", ba.toString());

        sas.setProperty("cppProperty", new QByteArray("it was a stormy, dark night"));
        ba = sas.byteArrayProperty("cppProperty");
        assertEquals("it was a stormy, dark night", ba.toString());

        sas.resetProperty("cppProperty");
        assertEquals("it was the darkest and stormiest night evar", sas.property("cppProperty").toString());
    }

    @Test public void standardClassName() {
        SignalsAndSlots sas = new SignalsAndSlots();

        assertEquals("SignalsAndSlots", sas.classNameFromMetaObject());
        assertEquals("QObject", sas.classNameOfSuperClassFromMetaObject());
    }

    @Test public void standardPropertyCount() {
        SignalsAndSlots sas = new SignalsAndSlots();

        assertEquals(2, sas.propertyCountFromMetaObject());
        assertEquals(1, sas.propertyCountOfSuperClassFromMetaObject());
    }

    @Test public void standardPropertyNames() {
        SignalsAndSlots sas = new SignalsAndSlots();

        List<String> list = sas.propertyNamesFromMetaObject();
        assertEquals(2, list.size());
        assertEquals("objectName", list.get(0));
        assertEquals("cppProperty", list.get(1));
    }

    private static class SignalsAndSlotsWithProperties extends SignalsAndSlots {
        private QByteArray javaProperty;

        @QtPropertyReader(name="javaProperty")
        public QByteArray javaProperty() {
            return javaProperty;
        }

        @QtPropertyWriter(name="javaProperty")
        public void setJavaProperty(QByteArray ba) {
            javaProperty = ba;
        }

        @QtPropertyResetter(name="javaProperty")
        public void resetJavaProperty() {
            javaProperty = new QByteArray("HAY GUYS I INVENTED INTERNET");
        }

    }

    private static class SignalsAndSlotsWithMoreProperties extends SignalsAndSlotsWithProperties {
        private QByteArray otherJavaProperty;

        @QtPropertyReader(name="otherJavaProperty")
        public QByteArray otherJavaProperty() {
            return otherJavaProperty;
        }

        @QtPropertyWriter(name="otherJavaProperty")
        public void setOtherJavaProperty(QByteArray ba) {
            otherJavaProperty = ba;
        }

        @QtPropertyResetter(name="otherJavaProperty")
        public void resetOtherJavaProperty() {
            otherJavaProperty = new QByteArray("HAY GUYS I INVENTED INTERNET");
        }

        @QtPropertyReader(name="readOnlyProperty")
        public QByteArray readOnlyProperty() {
            return new QByteArray("it was a darker and stormier night than yesterday night, which was also dark and stormy");
        }
    }

    @Test public void readWriteResetProperty() {
        SignalsAndSlotsWithMoreProperties sas = new SignalsAndSlotsWithMoreProperties();

        sas.setByteArrayProperty("javaProperty", new QByteArray("it was a dark and stormy night"));
        QByteArray ba = QVariant.toByteArray(sas.property("javaProperty"));
        assertEquals("it was a dark and stormy night", ba.toString());
        assertEquals("it was a dark and stormy night", sas.javaProperty().toString());

        sas.setProperty("javaProperty", new QByteArray("it was a stormy, dark night"));
        ba = sas.byteArrayProperty("javaProperty");
        assertEquals("it was a stormy, dark night", sas.javaProperty().toString());
        assertEquals("it was a stormy, dark night", ba.toString());

        sas.resetProperty("javaProperty");
        assertEquals("HAY GUYS I INVENTED INTERNET", sas.property("javaProperty").toString());
        assertEquals("HAY GUYS I INVENTED INTERNET", sas.javaProperty().toString());

        sas.setByteArrayProperty("otherJavaProperty", new QByteArray("it was a cold and dry night"));
        assertEquals("it was a cold and dry night", sas.property("otherJavaProperty").toString());
        assertEquals("it was a cold and dry night", sas.byteArrayProperty("otherJavaProperty").toString());
        assertEquals("it was a cold and dry night", sas.otherJavaProperty().toString());
    }

    @Test public void readOnlyProperty() {
        SignalsAndSlotsWithMoreProperties sas = new SignalsAndSlotsWithMoreProperties();
        assertEquals("it was a darker and stormier night than yesterday night, which was also dark and stormy", sas.byteArrayProperty("readOnlyProperty").toString());
    }

    @Test public void propertyCount() {
        SignalsAndSlotsWithMoreProperties sas = new SignalsAndSlotsWithMoreProperties();

        assertEquals(5, sas.propertyCountFromMetaObject());
        assertEquals(3, sas.propertyCountOfSuperClassFromMetaObject());
    }

    @Test public void className() {
        SignalsAndSlotsWithMoreProperties sas = new SignalsAndSlotsWithMoreProperties();

        assertEquals("org::qtjambi::autotests::TestConnections$SignalsAndSlotsWithMoreProperties",
                     sas.classNameFromMetaObject());
        assertEquals("org::qtjambi::autotests::TestConnections$SignalsAndSlotsWithProperties",
                sas.classNameOfSuperClassFromMetaObject());
    }

    @Test public void propertyNames() {
        SignalsAndSlotsWithMoreProperties sas = new SignalsAndSlotsWithMoreProperties();

        List<String> propertyNames = sas.propertyNamesFromMetaObject();
        assertEquals(5, propertyNames.size());
        assertEquals("objectName", propertyNames.get(0));
        assertEquals("cppProperty", propertyNames.get(1));
        assertEquals("javaProperty", propertyNames.get(2));
        assertEquals("readOnlyProperty", propertyNames.get(3));
        assertEquals("otherJavaProperty", propertyNames.get(4));

    }

    private static class JavaSignal extends SignalsAndSlots {
        public Signal2<String, QByteArray> aJavaSignal = new Signal2<String, QByteArray>();
    }

    @Test public void javaEmitShouldEmitCpp() {
        JavaSignal javaSignal = new JavaSignal();

        javaSignal.javaSignalToCppSlot();

        assertEquals("", javaSignal.received_string());
        assertEquals("", javaSignal.received_bytearray().toString());

        javaSignal.aJavaSignal.emit("where are we?", new QByteArray("what the hell is going on?"));

        assertEquals("where are we?", javaSignal.received_string());
        assertEquals("what the hell is going on?", javaSignal.received_bytearray().toString());
    }

    private static class ArrayQObject extends QObject {
        public Object received = null;

        public void fooBar(String args[]) {
            received = args;
        }

        public Signal1<String[]> zootBaz = new Signal1<String[]>();
    }

    @Test
    public void signatureOfJavaMethodsWithArrays() {
        ArrayQObject aqo = new ArrayQObject();

        String signature = SignalsAndSlots.metaObjectMethodSignature(aqo, "fooBar");
        assertEquals("fooBar(JObjectWrapper)", signature);

        assertTrue(SignalsAndSlots.invokeMethod(aqo, "fooBar", new String[] { "hello", "world" }));
        assertTrue(aqo.received instanceof String[]);

        String array[] = (String[])aqo.received;
        assertEquals(2, array.length);

        assertEquals("hello", array[0]);
        assertEquals("world", array[1]);
    }

    @Test
    public void signatureOfJavaSignalWithArrays() {
        ArrayQObject aqo = new ArrayQObject();

        String signature = SignalsAndSlots.metaObjectMethodSignature(aqo, "zootBaz");
        assertEquals("zootBaz(JObjectWrapper)", signature);

        aqo.zootBaz.connect(aqo, "fooBar(String [])");

        assertTrue(SignalsAndSlots.invokeMethod(aqo, "zootBaz", new String[] { "goodday", "planet" }));
        assertTrue(aqo.received instanceof String[]);

        String array[] = (String[])aqo.received;
        assertEquals(2, array.length);

        assertEquals("goodday", array[0]);
        assertEquals("planet", array[1]);

    }

    static class RecursiveSignalEmission extends QPushButton {
        public static final int COUNT = 100;

        public int emitted = 0;
        public void slot() {
            if (++emitted < COUNT)
                clicked.emit(true);
        }
    }

    @Test
    public void recursiveSignalEmission() {
		RecursiveSignalEmission e = new RecursiveSignalEmission();
		assertEquals(0, e.emitted);
		e.clicked.connect(e, "slot()");
		e.slot();
		assertEquals(RecursiveSignalEmission.COUNT, e.emitted);
    }

    private boolean changeSignalReceived = false;
    public void receiveChangedSignal(List<? extends Object> list) {
        synchronized(TestConnections.class) {
            changeSignalReceived = true;
        }
    }

    @Test
    public void genericsBasedSignals() {
        synchronized(TestConnections.class) {
            changeSignalReceived = false;
        }
        QGraphicsScene scene = new QGraphicsScene();
        scene.changed.connect(this, "receiveChangedSignal(List)");

        scene.addEllipse(new QRectF(0, 0, 10, 10));

        QApplication.processEvents();

        synchronized(TestConnections.class) {
            // MacOSX fails this test Sun/Oracle JDK 1.6.0u29 without wrapping variable access inside synchronized block
            assertTrue(changeSignalReceived);
        }
    }

    public static void main(String args[]) {
    	org.junit.runner.JUnitCore.main(TestConnections.class.getName());
    }
}
