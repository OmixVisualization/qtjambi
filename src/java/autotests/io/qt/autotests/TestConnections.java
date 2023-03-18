/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.NotSerializableException;
import java.io.ObjectOutputStream;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.Arrays;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Objects;
import java.util.Set;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QMisfittingSignatureException;
import io.qt.QNoSuchSlotException;
import io.qt.QtInvokable;
import io.qt.QtObject;
import io.qt.QtObjectInterface;
import io.qt.QtPointerType;
import io.qt.QtPropertyReader;
import io.qt.QtPropertyResetter;
import io.qt.QtPropertyWriter;
import io.qt.QtSignalEmitterInterface;
import io.qt.QtUninvokable;
import io.qt.autotests.generated.General;
import io.qt.autotests.generated.SignalsAndSlots;
import io.qt.core.QByteArray;
import io.qt.core.QCoreApplication;
import io.qt.core.QDeclarableSignals;
import io.qt.core.QEvent;
import io.qt.core.QInstanceMemberSignals;
import io.qt.core.QList;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaObject.Connection;
import io.qt.core.QMetaProperty;
import io.qt.core.QObject;
import io.qt.core.QRect;
import io.qt.core.QRectF;
import io.qt.core.QSize;
import io.qt.core.QStringList;
import io.qt.core.QUrl;
import io.qt.core.QVariant;
import io.qt.core.Qt;
import io.qt.gui.QClipboard;
import io.qt.gui.QDesktopServices;
import io.qt.gui.QGuiApplication;
import io.qt.internal.ClassAnalyzerUtility;
import io.qt.internal.TestUtility;
import io.qt.widgets.QApplication;
import io.qt.widgets.QColorDialog;
import io.qt.widgets.QGraphicsScene;
import io.qt.widgets.QLineEdit;
import io.qt.widgets.QPushButton;
import io.qt.widgets.QWidget;

public class TestConnections extends ApplicationInitializer
{
	static boolean hasSerializableLambdas = ClassAnalyzerUtility.serializeLambdaExpression((QMetaObject.Slot0)ApplicationInitializer::testInitializeWithWidgets) != null;
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
    public TestConnections()
    {
    }

    @Before
    public void setUp() {
        // This class is known to fail when we messed with this setting in a previous testcase running in the same JVM
        // The method run_queuedConnection() in particular
        assertEquals("getObjectCacheMode != DEFAULT", TestUtility.OBJECT_CACHE_MODE_DEFAULT, TestUtility.objectCacheMode());
    }

    @Test public void run_signalInNonQObject() {
        SignalsAndSlotsSubclass b = new SignalsAndSlotsSubclass();

        Exception e = null;
        try {
            SignalsAndSlotsSubclass.NonQObjectSubclass sc = b.getSubclassObject();
            sc.s.connect(this::reciveSignal);
        } catch (RuntimeException f) {
            e = f;
        }

        assertTrue(e != null);
        assertEquals(e.getMessage(), "Signals must be declared as members of QObject subclasses.");
    }

    public void reciveSignal(String s) {

    }

    class SignalEmitter implements QtSignalEmitterInterface, QInstanceMemberSignals {
        public final Signal2<String, Integer> mySignal = new Signal2<String, Integer>(this);
    }

    @Test public void run_connectFromNonQObjects() {
        NonQObject nonQObject = new NonQObject();
        SignalEmitter signalEmitter = new SignalEmitter();

        signalEmitter.mySignal.connect(nonQObject, "foobar(String, int)");

        signalEmitter.mySignal.emit("one-two-three", 123);

        assertEquals("one-two-three", nonQObject.receivedFoo);
        assertEquals(123, nonQObject.receivedBar);
        assertTrue(signalEmitter.mySignal.disconnect(nonQObject, "foobar(String, int)"));
    }

    @Test public void run_connectToNonQObjects() {
        SignalsAndSlotsSubclass sas = new SignalsAndSlotsSubclass();
        NonQObject o = new NonQObject();

        sas.signal6.connect(o, "foobar(String, int)");
        sas.signal6.emit("I have this many friends:", 100);

        assertEquals("I have this many friends:", o.receivedFoo);
        assertEquals(100, o.receivedBar);
    }

    @Test public void run_shouldNotConnectBySignatureToBlockedSlots()
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

    @Test public void run_shouldNotConnectByHandleToBlockedSlots()
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

        assertEquals(0, sas.java_slot4_called);
        sas.emit_signal_1();
        assertEquals(1, sas.java_slot4_called);
        sas.signal1.emit();
        assertEquals(2, sas.java_slot4_called);

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

        assertEquals(0, sas.slot1_1_called());
        sas.signal1.emit();
        assertEquals(1, sas.slot1_1_called());
        sas.emit_signal_1();
        assertEquals(2, sas.slot1_1_called());

        sas.signal1.connect(sas::slot1_2);

        assertEquals(0, sas.slot1_2_called());
        sas.signal1.emit();
        assertEquals(3, sas.slot1_1_called());
        assertEquals(1, sas.slot1_2_called());
        sas.emit_signal_1();
        assertEquals(4, sas.slot1_1_called());
        assertEquals(2, sas.slot1_2_called());
    }

    @Test public void run_cppEmitShouldEmitJavaSignal() {
        SignalsAndSlotsSubclass sas = new SignalsAndSlotsSubclass();

        sas.signal1.connect(sas::slot4);
        sas.signal1.connect(sas::slot4);

        assertEquals(0, sas.java_slot4_called);
        sas.emit_signal_1();
        assertEquals(2, sas.java_slot4_called);
    }

    @Test public void run_cppEmitShouldEmitUniqueConnectedJavaSignal() {
        SignalsAndSlotsSubclass sas = new SignalsAndSlotsSubclass();

        if(!hasSerializableLambdas) {
	        sas.signal1.connect(sas, "slot4()", Qt.ConnectionType.UniqueConnection);
	        sas.signal1.connect(sas, "slot4()", Qt.ConnectionType.UniqueConnection);
        }else {
            sas.signal1.connect(sas::slot4, Qt.ConnectionType.UniqueConnection);
            sas.signal1.connect(sas::slot4, Qt.ConnectionType.UniqueConnection);
        }

        assertEquals(0, sas.java_slot4_called);
        sas.emit_signal_1();
        assertEquals(1, sas.java_slot4_called);
    }

    @Test public void run_cppEmitShouldEmitJavaSignalEvenAfterDisconnectAll() {
        SignalsAndSlotsSubclass sas = new SignalsAndSlotsSubclass();
        
        sas.disconnect();

        sas.signal1.connect(sas::slot4);

        assertEquals(0, sas.java_slot4_called);
        sas.emit_signal_1();
        assertEquals(1, sas.java_slot4_called);
    }

    @Test public void run_javaEmitShouldEmitCppSignal() {
        SignalsAndSlots sas = new SignalsAndSlots();

        sas.connectSignal1ToSlot1_1();
        sas.connectSignal1ToSlot1_1();

        assertEquals(0, sas.slot1_1_called());
        sas.signal1.emit();
        assertEquals(2, sas.slot1_1_called());
    }

    @Test public void run_javaEmitShouldEmitCppSignalFP() {
        SignalsAndSlots sas = new SignalsAndSlots();

        sas.connectSignal1ToSlot1_1FP();
        sas.connectSignal1ToSlot1_1FP();

        assertEquals(0, sas.slot1_1_called());
        sas.signal1.emit();
        assertEquals(2, sas.slot1_1_called());
    }

    @Test public void run_javaDisconnectShouldDisconnectCpp() {
        SignalsAndSlots sas = new SignalsAndSlots();

        assertTrue(sas.connectSignal1ToSlot1_1());

        if(!hasSerializableLambdas) {
        	assertTrue(sas.signal1.disconnect(sas, "slot1_1()"));
        }else {
        	assertTrue(sas.signal1.disconnect(sas::slot1_1));
        }

        assertEquals(0, sas.slot1_1_called());
        sas.emit_signal_1();
        assertEquals(0, sas.slot1_1_called());
        sas.signal1.emit();
        assertEquals(0, sas.slot1_1_called());
    }

    @Test public void run_javaDisconnectShouldDisconnectCppFP() {
        SignalsAndSlots sas = new SignalsAndSlots();
        assertTrue(sas.connectSignal1ToSlot1_1FP());

        if(!hasSerializableLambdas) {
        	sas.signal1.disconnect(sas, "slot1_1()");
        }else {
        	sas.signal1.disconnect(sas::slot1_1);
        }

        assertEquals(0, sas.slot1_1_called());
        sas.emit_signal_1();
        assertEquals(1, sas.slot1_1_called());
        sas.signal1.emit();
        assertEquals(2, sas.slot1_1_called());
        sas.signal1.disconnect();
        sas.emit_signal_1();
        assertEquals(2, sas.slot1_1_called());
        sas.signal1.emit();
        assertEquals(2, sas.slot1_1_called());
    }

    @Test public void run_cppDisconnectShouldDisconnectJava() {
        SignalsAndSlots sas = new SignalsAndSlots();

        if(!hasSerializableLambdas) {
        	assertTrue(sas.signal1.connect(sas, "slot1_1()").isConnected());
        }else {
        	assertTrue(sas.signal1.connect(sas::slot1_1).isConnected());
        }

        assertTrue(sas.disconnectSignal1FromSlot1_1());

        assertEquals(0, sas.slot1_1_called());
        sas.signal1.emit();
        assertEquals(0, sas.slot1_1_called());
        sas.emit_signal_1();
        assertEquals(0, sas.slot1_1_called());
    }

    @Test public void run_cppDisconnectAllShouldDisconnectJava() {
        SignalsAndSlotsSubclass sas = new SignalsAndSlotsSubclass();

        sas.signal1.connect(sas::slot4);
        sas.signal1.connect(sas::slot1_1);
        sas.signal2.connect(sas::slot2);
        sas.disconnectAllFromObject();

        assertEquals(0, sas.slot1_1_called());
        assertEquals(0, sas.java_slot4_called);
        sas.signal1.emit();
        sas.emit_signal_1();
        assertEquals(0, sas.slot1_1_called());
        assertEquals(0, sas.java_slot4_called);

        assertEquals(0, sas.slot2_called());
        sas.emit_signal_2(10);
        sas.signal2.emit(10);
        assertEquals(0, sas.slot2_called());
    }

    @Test public void run_cppDisconnectAllShouldNotDisconnectJavaSignal() {
        SignalsAndSlotsSubclass sas = new SignalsAndSlotsSubclass();

        sas.disconnectAllFromObject();

        sas.signal1.connect(sas::slot4);
        sas.signal1.connect(sas::slot1_1);
        sas.signal2.connect(sas::slot2);
        
        assertEquals(0, sas.slot1_1_called());
        assertEquals(0, sas.java_slot4_called);
        sas.signal1.emit();
        assertEquals(1, sas.slot1_1_called());
        assertEquals(1, sas.java_slot4_called);
        sas.emit_signal_1();
        assertEquals(2, sas.slot1_1_called());
        assertEquals(2, sas.java_slot4_called);

        assertEquals(0, sas.slot2_called());
        sas.emit_signal_2(10);
        assertEquals(3, sas.slot2_called());
        assertEquals(20, sas.java_slot2_called);
        sas.signal2.emit(10);
        assertEquals(6, sas.slot2_called());
        assertEquals(40, sas.java_slot2_called);
    }

    @Test public void run_javaDisconnectAllShouldDisconnectCpp() {
        SignalsAndSlots sas = new SignalsAndSlots();

        sas.connectSignal1ToSlot1_1();
        sas.connectSignal1ToSlot1_1();
        sas.connectSignal2ToSlot2();
        sas.disconnect();

        assertEquals(0, sas.slot1_1_called());
        sas.emit_signal_1();
        sas.signal1.emit();
        assertEquals(0, sas.slot1_1_called());

        assertEquals(0, sas.slot2_called());
        sas.emit_signal_2(10);
        sas.signal2.emit(10);
        assertEquals(0, sas.slot2_called());
    }

    @Test public void run_javaDisconnectAllFPShouldDisconnectCppFP() {
        SignalsAndSlots sas = new SignalsAndSlots();

        sas.connectSignal1ToSlot1_1FP();
        sas.signal1.connect(sas::slot1_1);
        sas.disconnectAllFromSignal1FP();

        assertEquals(0, sas.slot1_1_called());
        sas.emit_signal_1();
        sas.signal1.emit();
        assertEquals(0, sas.slot1_1_called());
    }

    @Test public void run_javaDisconnectAllShouldDisconnectJava() {
        SignalsAndSlotsSubclass sas = new SignalsAndSlotsSubclass();

        sas.signal1.connect(sas::slot1_1);
        sas.signal1.connect(sas::slot4);
        sas.signal2.connect(sas::slot2);
        sas.disconnect();

        assertEquals(0, sas.slot1_1_called());
        assertEquals(0, sas.java_slot4_called);
        sas.emit_signal_1();
        sas.signal1.emit();
        assertEquals(0, sas.slot1_1_called());
        assertEquals(0, sas.java_slot4_called);

        assertEquals(0, sas.slot2_called());
        sas.emit_signal_2(10);
        sas.signal2.emit(10);
        assertEquals(0, sas.slot2_called());
    }

    @Test public void run_cppDisconnectSignalShouldDisconnectJava() {
        SignalsAndSlotsSubclass sas = new SignalsAndSlotsSubclass();

        sas.signal1.connect(sas::slot4);
        sas.signal1.connect(sas::slot1_1);
        sas.signal2.connect(sas::slot2);

        sas.disconnectAllFromSignal1();

        assertEquals(0, sas.slot1_1_called());
        assertEquals(0, sas.java_slot4_called);
        sas.emit_signal_1();
        sas.signal1.emit();
        assertEquals(0, sas.slot1_1_called());
        assertEquals(0, sas.java_slot4_called);

        assertEquals(0, sas.slot2_called());
        sas.emit_signal_2(10);
        sas.signal2.emit(11);
        assertEquals(6, sas.slot2_called());
        assertEquals(21 * 2, sas.java_slot2_called);
    }

    @Test public void run_javaDisconnectSignalShouldDisconnectJava() {
        SignalsAndSlotsSubclass sas = new SignalsAndSlotsSubclass();

        sas.signal1.connect(sas::slot4);
        sas.signal1.connect(sas::slot1_1);
        sas.signal2.connect(sas::slot2);

        sas.signal1.disconnect();

        assertEquals(0, sas.slot1_1_called());
        assertEquals(0, sas.java_slot4_called);
        sas.emit_signal_1();
        sas.signal1.emit();
        assertEquals(0, sas.slot1_1_called());
        assertEquals(0, sas.java_slot4_called);

        assertEquals(0, sas.slot2_called());
        sas.emit_signal_2(10);
        sas.signal2.emit(11);
        assertEquals(21*2, sas.java_slot2_called);
        assertEquals(6, sas.slot2_called());
    }

    @Test public void run_javaDisconnectSignalShouldDisconnectCpp() {
        SignalsAndSlotsSubclass sas = new SignalsAndSlotsSubclass();

        sas.connectSignal1ToSlot1_1();
        sas.connectSignal2ToSlot2();

        sas.signal1.disconnect();

        assertEquals(0, sas.slot2_called());
        sas.emit_signal_2(10);
        sas.signal2.emit(11);
        assertEquals(21*2, sas.java_slot2_called);
        assertEquals(6, sas.slot2_called());

        assertEquals(0, sas.slot1_1_called());
        assertEquals(0, sas.java_slot4_called);
        sas.emit_signal_1();
        sas.signal1.emit();
        assertEquals(0, sas.slot1_1_called());
        assertEquals(0, sas.java_slot4_called);

    }

    @Test public void run_cppDisconnectReceiverShouldDisconnectJava()
    {
        SignalsAndSlotsSubclass sas = new SignalsAndSlotsSubclass();
        SignalsAndSlots sas2 = new SignalsAndSlots();

        if(!hasSerializableLambdas) {
        	sas.signal1.connect(sas2, "slot1_1()");
	        sas.signal2.connect(sas2, "slot2(int)");
	        sas.signal1.connect(sas, "slot1_1()");
	        sas.signal2.connect(sas, "slot2(int)");
        }else {
	        sas.signal1.connect(sas2::slot1_1);
	        sas.signal2.connect(sas2::slot2);
	        sas.signal1.connect(sas::slot1_1);
	        sas.signal2.connect(sas::slot2);
        }
        sas.disconnectAllFromReceiver(sas2);

        assertEquals(0, sas.slot1_1_called());
        assertEquals(0, sas2.slot1_1_called());
        sas.signal1.emit();
        assertEquals(sas.slot1_1_called(), 1);
        assertEquals(0, sas2.slot1_1_called());
        sas.emit_signal_1();
        assertEquals(2, sas.slot1_1_called());
        assertEquals(0, sas2.slot1_1_called());
    }

    @Test public void run_javaDisconnectReceiverShouldDisconnectJava() {
        SignalsAndSlotsSubclass sasSub = new SignalsAndSlotsSubclass();
        SignalsAndSlots sasBase = new SignalsAndSlots();

        QMetaObject.Connection c1;
        QMetaObject.Connection c2;
        QMetaObject.Connection c3;
        QMetaObject.Connection c4;
        if(!hasSerializableLambdas) {
        	c1 = sasSub.signal1.connect(sasBase, "slot1_1()");
            c2 = sasSub.signal2.connect(sasBase, "slot2(int)");
            c3 = sasSub.signal1.connect(sasSub, "slot1_1()");
            c4 = sasSub.signal2.connect(((SignalsAndSlots)sasSub), "slot2(int)");
        }else {
            c1 = sasSub.signal1.connect(sasBase::slot1_1);
            c2 = sasSub.signal2.connect(sasBase::slot2);
            c3 = sasSub.signal1.connect(sasSub::slot1_1);
            c4 = sasSub.signal2.connect(((SignalsAndSlots)sasSub)::slot2);
        }
        assertTrue(c1 instanceof QtObject);
        assertTrue(c2 instanceof QtObject);
        assertTrue(c3 instanceof QtObject);
        assertTrue(c4 instanceof QtObject);

        sasSub.disconnect(sasBase);

        assertEquals(0, sasSub.slot1_1_called());
        assertEquals(0, sasBase.slot1_1_called());
        sasSub.signal1.emit();
        assertEquals(1, sasSub.slot1_1_called());
        assertEquals(0, sasBase.slot1_1_called());
        sasSub.emit_signal_1();
        assertEquals(2, sasSub.slot1_1_called());
        assertEquals(0, sasBase.slot1_1_called());
    }


    @Test public void run_javaDisconnectReceiverShouldDisconnectCpp() {
        SignalsAndSlotsSubclass sas = new SignalsAndSlotsSubclass();

        sas.connectSignal1ToSlot1_1();
        sas.connectSignal1ToSlot1_1();
        sas.connectSignal2ToSlot2();

        sas.disconnect(sas);

        assertEquals(0, sas.slot1_1_called());
        sas.signal1.emit();
        assertEquals(0, sas.slot1_1_called());
        sas.emit_signal_1();
        assertEquals(0, sas.slot1_1_called());
        assertEquals(0, sas.slot2_called());
        sas.signal2.emit(10);
        assertEquals(0, sas.slot2_called());
        sas.emit_signal_2(11);
        assertEquals(0, sas.slot2_called());
    }

    @Test public void run_cppDisconnectReceiverFromSignalShouldDisconnectJava() {
        SignalsAndSlots sas = new SignalsAndSlots();
        SignalsAndSlots sas2 = new SignalsAndSlots();
//        List<QMetaMethod> methods = sas.metaObject().methods();
//        methods.forEach(System.out::println);

        if(!hasSerializableLambdas) {
	        sas.signal1.connect(sas, "slot1_1()");
	        sas.signal1.connect(sas2, "slot1_1()");
        }else {
	        sas.signal1.connect(sas::slot1_1);
	        sas.signal1.connect(sas2::slot1_1);
        }

        sas.disconnectReceiverFromSignal1(sas2);

        assertEquals(0, sas.slot1_1_called());
        assertEquals(0, sas2.slot1_1_called());
        sas.signal1.emit();
        assertEquals(1, sas.slot1_1_called());
        sas.emit_signal_1();
        assertEquals(2, sas.slot1_1_called());
        assertEquals(0, sas2.slot1_1_called());
    }

    @Test public void run_javaDisconnectReceiverFromSignalShouldDisconnectJava() {
        SignalsAndSlots sas = new SignalsAndSlots();
        SignalsAndSlots sas2 = new SignalsAndSlots();

        if(!hasSerializableLambdas) {
	        sas.signal1.connect(sas, "slot1_1()");
	        sas.signal1.connect(sas2, "slot1_1()");
        }else {
	        sas.signal1.connect(sas::slot1_1);
	        sas.signal1.connect(sas2::slot1_1);
        }

        sas.signal1.disconnect(sas2);

        assertEquals(0, sas.slot1_1_called());
        assertEquals(0, sas2.slot1_1_called());
        sas.signal1.emit();
        sas.emit_signal_1();
        assertEquals(2, sas.slot1_1_called());
        assertEquals(0, sas2.slot1_1_called());
    }

    @Test public void run_javaDisconnectReceiverFromSignalShouldDisconnectCpp() {
        SignalsAndSlots sas = new SignalsAndSlots();
        SignalsAndSlots sas2 = new SignalsAndSlots();

        sas.connectSignal1ToSlot1_1In(sas2);
        sas.connectSignal1ToSlot1_1();

        sas.signal1.disconnect(sas2);

        assertEquals(0, sas.slot1_1_called());
        assertEquals(0, sas2.slot1_1_called());
        sas.signal1.emit();
        sas.emit_signal_1();
        assertEquals(2, sas.slot1_1_called());
        assertEquals(0, sas2.slot1_1_called());
    }
    
    @Test public void run_javaDisconnectReceiverFromFPConnectedSignalShouldDisconnectCpp() {
        SignalsAndSlots sas = new SignalsAndSlots();
        SignalsAndSlots sas2 = new SignalsAndSlots();

        sas.connectSignal1ToSlot1_1InFP(sas2);
        sas.connectSignal1ToSlot1_1FP();

        sas.signal1.disconnect(sas2);

        assertEquals(0, sas.slot1_1_called());
        assertEquals(0, sas2.slot1_1_called());
        sas.signal1.emit();
        sas.emit_signal_1();
        assertEquals(2, sas.slot1_1_called());
        assertEquals(0, sas2.slot1_1_called());
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

            obj1.signal1.connect(obj1::slot1_1);
            obj1.signal1.connect(obj1::slot1_2);
            obj1.signal1.connect(obj1::slot1_3);
            obj1.signal2.connect(obj1::slot2);
            obj1.signal3.connect(obj1::slot3);
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

            obj1.signal1.connect(obj2::slot1_1);
            obj1.signal1.connect(obj1::slot1_2);

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

            obj1.signal3.connect(obj1::slot3);

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


            obj1.signal2.connect(obj1::slot2);

            obj1.setupSignals(obj1, 2);

            obj1.emit_signal_2(13);
            assertEquals(obj1.java_slot2_called, 52);
            assertEquals(obj1.slot2_called(), 6);

            Accessor.emit_signal(obj1.signal2, 4);
            assertEquals(68, obj1.java_slot2_called);
            assertEquals(obj1.slot2_called(), 12);

            Connection c1 = obj1.signal3.connect(obj1::slot3_2);
            Connection c2 = obj1.signal3.connect(obj1::slot3_2);
            
            assertTrue(c1!=null);
            assertTrue(c2!=null);
//            assertFalse(c1 instanceof QtJambiObject);
//            assertFalse(c2 instanceof QtJambiObject);

            obj1.emit_signal_3("17");
            assertEquals(34, obj1.java_slot3_2_called);
            assertEquals(17, obj1.slot3_called());

            obj1.signal4.connect(obj1::slot4);
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
            assertEquals(2, obj1.slot1_1_called());
            assertEquals(3, obj1.slot1_2_called());
            assertEquals(3, obj1.slot1_3_called());
            assertEquals(2, obj1.java_slot4_called);

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

    QDeclarableSignals.Signal0 destroyed = new QDeclarableSignals.Signal0();

    @Test
    public void run_createDestroy() {
        QObject parent = new QObject();
        int count = 1000;
        destroyed.connect(MyQObject::increaseFinalized);
        MyQObject objects[] = new MyQObject[count];
        for (int i = 0; i < objects.length; ++i) {
            objects[i] = new MyQObject(parent);
            objects[i].destroyed.set(this.destroyed);

        }

        MyQObject.finalizedCount = 0;

        parent.dispose();
        ApplicationInitializer.runGC();
        try {
            Thread.sleep(600);
        } catch (Exception e) {
        }
        ;
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());

        assertEquals(count, MyQObject.finalizedCount);
    }

    @Test public void run_connectJavaQt()
    {
		Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
        QWidget widget = new QWidget();

        assertTrue(!widget.isVisible());
        assertTrue(widget.isEnabled());

        MyQObject obj = new MyQObject();
        if(!hasSerializableLambdas) {
	        obj.signalMyQObject.connect(widget, "show()");
	        obj.signalNoParams.connect(widget, "hide()");
	        obj.signalBoolean.connect(widget, "setEnabled(boolean)");
	        obj.signalInteger.connect(widget, "close()");
        }else {
            obj.signalMyQObject.connect(widget::show);
            obj.signalNoParams.connect(widget::hide);
            obj.signalBoolean.connect(widget::setEnabled);
            obj.signalInteger.connect(widget::close);
        }
        obj.javaSignalMyQObject(obj);
        assertTrue(widget.isVisible());

        obj.javaSignalboolean(false);
        assertTrue(!widget.isEnabled());

        obj.javaSignalBoolean(true); // both Boolean-functions emit the signalBoolean signal
        assertTrue(widget.isEnabled());

        obj.javaSignalNoParams();
        assertTrue(!widget.isVisible());

        if(!hasSerializableLambdas) {
	        assertTrue(obj.signalMyQObject.disconnect(widget, "show()"));
	        assertTrue(obj.signalNoParams.disconnect(widget, "hide()"));
	        assertTrue(obj.signalBoolean.disconnect(widget, "setEnabled(boolean)"));
        }else {
	        assertTrue(obj.signalMyQObject.disconnect(widget::show));
	        assertTrue(obj.signalNoParams.disconnect(widget::hide));
	        assertTrue(obj.signalBoolean.disconnect(widget::setEnabled));
        }
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
        if(!hasSerializableLambdas) {
        	assertTrue(obj.signalInteger.disconnect(widget, "close()"));
        }else {
        	assertTrue(obj.signalInteger.disconnect(widget::close));
        }

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
        assertEquals("widget.geometry.x", 200, rect.x());
        assertEquals("widget.geometry.y", 300, rect.y());
        assertEquals("widget.geometry.height", 400, rect.width());
        assertEquals("widget.geometry.width", 500, rect.height());
        QSize test = new QSize();
        assertEquals(-1, test.width());
        assertEquals(-1, test.height());

        QSize sz = new QSize(10, 20);
        assertEquals(10, sz.width());
        assertEquals(20, sz.height());
        b1.setIconSize(sz);
        QSize sz2 = b1.iconSize();
        assertEquals(sz2.width(), sz.width());
        assertEquals(sz2.height(), sz.height());

        obj.signalInteger.connect((QMetaObject.Slot0)b1::setFocus);
        obj.signalNoParams.connect(b2::setFocus);
        obj.signalDouble.connect(b1::hide);
        obj.signalDouble.connect(b2::hide);
        obj.signalDoubleInteger.connect(widget::close);
        obj.signalLong.connect(b1::show);
        obj.signalLong.connect(b2::show);
        obj.signalMixed1.connect(widget::hide);
        if(!hasSerializableLambdas) {
        	obj.signalString.connect(le, "setText(java.lang.String)");
        }else {
        	obj.signalString.connect(le::setText);
        }
        le.textChanged.connect(obj::javaSlotString);
        QApplication.instance().focusChanged.connect(obj::javaSlotFocusChanged);
        obj.signalQSize.connect(b2::setIconSize);

        obj.javaSignalint(123);

        assertEquals(b1, QApplication.focusWidget());
        assertTrue(QApplication.focusWidget() == b1);
        obj.javaSignalNoParams();
        assertEquals(b2, QApplication.focusWidget());
        assertTrue(QApplication.focusWidget() == b2);
        assertEquals(b1, obj.slotResult); // set by javaSlotFocusChanged()
        assertTrue(obj.slotResult == b1);
        assertEquals(b2, obj.slotResult2); // ditto
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
        assertEquals("Line edit text", le.text());
        assertEquals("Line edit text", obj.slotResult);

        if(!hasSerializableLambdas) {
        	assertTrue(obj.signalString.disconnect(le, "setText(java.lang.String)"));
            obj.signalString.connect(le, "selectAll()");
        }else {
        	assertTrue(obj.signalString.disconnect(le::setText));
            obj.signalString.connect(le::selectAll);
        }

        obj.javaSignalString("ABC");
        assertEquals("Line edit text", le.text());
        assertEquals("Line edit text", obj.slotResult);

        boolean[] dataChanged = {false};
        QClipboard clipboard = QApplication.clipboard();
        Connection connection = clipboard.dataChanged.connect(()->{dataChanged[0] = true;});
        if(!hasSerializableLambdas) {
	        assertTrue(obj.signalString.disconnect(le, "selectAll()"));
        }else {
	        assertTrue(obj.signalString.disconnect(le::selectAll));
	        obj.signalString.connect(le::cut);
	        obj.javaSignalString("DEF");
	        assertEquals(le.text(), "");
	        assertEquals(obj.slotResult, "");
	        QApplication.processEvents();

	        if(dataChanged[0]) {
		        obj.signalString.connect(le::paste);
		        obj.javaSignalString("GHI");
		        assertEquals("Line edit text", le.text());
		        assertEquals("Line edit text", obj.slotResult);
	        }
        }

        QObject.disconnect(connection);

        obj.javaSignalTwoParameters(123.0, 456); // the signalDoubleInteger signal
        assertTrue(!widget.isVisible());
        assertTrue(!b1.isVisible());
        assertTrue(!b2.isVisible());

        sz = new QSize(40, 60);
        assertEquals(40, sz.width());
        assertEquals(60, sz.height());
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
            obj2.signalBoolean.connect(obj::javaSlotboolean);

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

    private void test_javaToJavaConnect(String name, MyQObject sender,
    		 QMetaObject.AbstractSignal signal,
             String signalFunctionName,
             MyQObject receiver,
             String slot,
             Object[] parameters,
             Object expectedSlotResult,
             Boolean checkReturnedReference,
             Class<?>[] parameterTypes)
     {

    	QMetaMethod signalMethod = QMetaMethod.fromSignal(signal);
        System.out.println("signal: "+signalMethod.cppMethodSignature()+" = "+signalMethod.methodSignature()+" "+signalMethod.parameterClassTypes());
        receiver.metaObject().methods().forEach(m->{
     	   if(slot.startsWith(m.name().toString())) {
     		   System.out.println("receiver slot: "+m.cppMethodSignature()+" = "+m.methodSignature()+" "+m.parameterClassTypes());
     		   System.out.println("matches: "+QMetaObject.checkConnectArgs(signalMethod, m));
     	   }
        });
        Connection connection = signal.connect(receiver, slot);
        assertTrue(connection.isConnected());
        if (parameters == null)
            parameters = new Object[0];

        receiver.slotResult = null;
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
        assertTrue(!checkReturnedReference.booleanValue() || Objects.equals(receiver.slotResult, expectedSlotResult));
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
                   receiver, "javaSlotLong(Long)", params, params[0], false, parameterTypes);

        params = new Object[1];
        params[0] = Long.MIN_VALUE;
        sender = new MyQObject();
        receiver = new MyQObject();
        test_javaToJavaConnect("Long signal/slot MIN_VALUE", sender, sender.signalLong, "javaSignalLong",
                receiver, "javaSlotLong(Long)", params, params[0], false, parameterTypes);

        parameterTypes = new Class[1];
        parameterTypes[0] = Integer.class;

        params = new Object[1];
        params[0] = Integer.MAX_VALUE;
        test_javaToJavaConnect("Integer signal/slot MAX_VALUE", sender, sender.signalInteger, "javaSignalInteger",
                   receiver, "javaSlotInteger(Integer)", params, params[0], false, parameterTypes);

        params = new Object[1];
        params[0] = Integer.MIN_VALUE;
        sender = new MyQObject();
        receiver = new MyQObject();
        test_javaToJavaConnect("Integer signal/slot MIN_VALUE", sender, sender.signalInteger, "javaSignalInteger",
                receiver, "javaSlotInteger(Integer)", params, params[0], false, parameterTypes);

        parameterTypes = new Class[1];
        parameterTypes[0] = Boolean.class;

        params = new Object[1];
        params[0] = true;
        test_javaToJavaConnect("Boolean signal/slot true", sender, sender.signalBoolean, "javaSignalBoolean",
                receiver, "javaSlotBoolean(Boolean)", params, params[0], false, parameterTypes);

        params = new Object[1];
        params[0] = false;
        sender = new MyQObject();
        receiver = new MyQObject();
        test_javaToJavaConnect("Boolean signal/slot false", sender, sender.signalBoolean, "javaSignalBoolean",
                receiver, "javaSlotBoolean(Boolean)", params, params[0], false, parameterTypes);

        parameterTypes = new Class[1];
        parameterTypes[0] = Double.class;

        params = new Object[1];
        params[0] = Double.MAX_VALUE;
        test_javaToJavaConnect("Double signal/slot MAX_VALUE", sender, sender.signalDouble, "javaSignalDouble",
                receiver, "javaSlotDouble(Double)", params, params[0], false, parameterTypes);

        params = new Object[1];
        params[0] = Double.MIN_VALUE;
        sender = new MyQObject();
        receiver = new MyQObject();
        test_javaToJavaConnect("Double signal/slot MIN_VALUE", sender, sender.signalDouble, "javaSignalDouble",
                receiver, "javaSlotDouble(Double)", params, params[0], false, parameterTypes);

        parameterTypes = new Class[1];
        parameterTypes[0] = Short.class;

        params = new Object[1];
        params[0] = Short.MAX_VALUE;
        test_javaToJavaConnect("Short signal/slot MAX_VALUE", sender, sender.signalShort, "javaSignalShort",
                receiver, "javaSlotShort(Short)", params, params[0], false, parameterTypes);

        params = new Object[1];
        params[0] = Short.MIN_VALUE;
        sender = new MyQObject();
        receiver = new MyQObject();
        test_javaToJavaConnect("Short signal/slot MIN_VALUE", sender, sender.signalShort, "javaSignalShort",
                receiver, "javaSlotShort(Short)", params, params[0], false, parameterTypes);

        parameterTypes = new Class[1];
        parameterTypes[0] = Float.class;

        params = new Object[1];
        params[0] = Float.MAX_VALUE;
        test_javaToJavaConnect("Float signal/slot MAX_VALUE", sender, sender.signalFloat, "javaSignalFloat",
                receiver, "javaSlotFloat(Float)", params, params[0], false, parameterTypes);

        params = new Object[1];
        params[0] = Float.MIN_VALUE;
        sender = new MyQObject();
        receiver = new MyQObject();
        test_javaToJavaConnect("Float signal/slot MIN_VALUE", sender, sender.signalFloat, "javaSignalFloat",
                receiver, "javaSlotFloat(Float)", params, params[0], false, parameterTypes);

        parameterTypes = new Class[1];
        parameterTypes[0] = Character.class;

        params = new Object[1];
        params[0] = Character.MAX_VALUE;
        test_javaToJavaConnect("Character signal/slot MAX_VALUE", sender, sender.signalCharacter, "javaSignalCharacter",
                receiver, "javaSlotCharacter(Character)", params, params[0], false, parameterTypes);

        params = new Object[1];
        params[0] = Character.MIN_VALUE;
        sender = new MyQObject();
        receiver = new MyQObject();
        test_javaToJavaConnect("Character signal/slot MIN_VALUE", sender, sender.signalCharacter, "javaSignalCharacter",
                receiver, "javaSlotCharacter(java.lang.Character)", params, params[0], false, parameterTypes);

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
                  receiver, "javaSlotMixed1(String, double, int, Integer)", params, params[0], false, parameterTypes);

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
       params[0] = Integer.valueOf(654);
       sender = new MyQObject();
       receiver = new MyQObject();
       sender.signalInteger.connect(receiver.signalInteger2);
       test_javaToJavaConnect("Signal to signal to slot", sender, receiver.signalInteger2, "javaSignalInteger",
               receiver, "javaSlotInteger(Integer)", params, params[0], false, parameterTypes);

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

     private void test_borkedConnections(String name, MyQObject sender,
    		 QMetaObject.AbstractSignal signal,
             QObject receiver,
             String slotSignature,
             Class<?> expectedExceptionType,
             String expectedExceptionMessage)
     {

         Class<?> ce = null;
         String msg = null;
         try {
             signal.connect(receiver, slotSignature);
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
                 receiver, "javaSlotMixed1(String, double, int, Integer)", QMisfittingSignatureException.class, "Incompatible sender/receiver arguments ");

         test_borkedConnections("Wrong parameter types, narrowing", sender, sender.signalDouble,
                 receiver, "javaSlotInteger(Integer)", QMisfittingSignatureException.class, "Incompatible sender/receiver arguments ");

         test_borkedConnections("Wrong parameter types, widening", sender, sender.signalInteger,
                 receiver, "javaSlotDouble(Double)", QMisfittingSignatureException.class, "Incompatible sender/receiver arguments ");
     }

      // Method requires: getObjectCacheMode == DEFAULT
      @Test 
      public void run_queuedConnection() {
    	  QMetaObject.forType(MyQObject.class);
          MyQObject sender = new MyQObject();
          MyQObject receiver = new MyQObject();

          sender.signalBoolean.connect(receiver::javaSlotboolean, Qt.ConnectionType.QueuedConnection);
          sender.signalBoolean.connect(receiver::javaSlotBoolean, Qt.ConnectionType.QueuedConnection);
          sender.signalCharacter.connect(receiver::javaSlotchar, Qt.ConnectionType.QueuedConnection);
          sender.signalCharacter.connect(receiver::javaSlotCharacter, Qt.ConnectionType.QueuedConnection);
          sender.signalShort.connect(receiver::javaSlotshort, Qt.ConnectionType.QueuedConnection);
          sender.signalShort.connect(receiver::javaSlotShort, Qt.ConnectionType.QueuedConnection);
          sender.signalInteger.connect(receiver::javaSlotint, Qt.ConnectionType.QueuedConnection);
          sender.signalInteger.connect(receiver::javaSlotInteger, Qt.ConnectionType.QueuedConnection);
          sender.signalLong.connect(receiver::javaSlotlong, Qt.ConnectionType.QueuedConnection);
          sender.signalLong.connect(receiver::javaSlotLong, Qt.ConnectionType.QueuedConnection);
          sender.signalFloat.connect(receiver::javaSlotfloat, Qt.ConnectionType.QueuedConnection);
          sender.signalFloat.connect(receiver::javaSlotFloat, Qt.ConnectionType.QueuedConnection);
          sender.signalDouble.connect(receiver::javaSlotdouble, Qt.ConnectionType.QueuedConnection);
          sender.signalDouble.connect(receiver::javaSlotDouble, Qt.ConnectionType.QueuedConnection);
          sender.signalString.connect(receiver::javaSlotString, Qt.ConnectionType.QueuedConnection);

          // Boolean
          receiver.slotResult = null;
          sender.javaSignalboolean(true);
    	  assertEquals(null, receiver.slotResult);
    	  QCoreApplication.processEvents();
    	  assertEquals(Boolean.valueOf(true), receiver.slotResult);
          sender.javaSignalBoolean(false);
          QCoreApplication.processEvents();
          assertEquals(Boolean.valueOf(false), receiver.slotResult);

          // Byte ?

          // Character
          sender.javaSignalchar('x');
          QCoreApplication.processEvents();
          assertEquals(Character.valueOf('x'), receiver.slotResult);
          sender.javaSignalCharacter(Character.valueOf('y'));
          QCoreApplication.processEvents();
          assertEquals(Character.valueOf('y'), receiver.slotResult);

          // Shorts
          sender.javaSignalShort((short) 40);
          QCoreApplication.processEvents();
          assertEquals(Short.valueOf((short) 40), receiver.slotResult);
          sender.javaSignalShort((short) 41);
          QCoreApplication.processEvents();
          assertEquals(Short.valueOf((short) 41), receiver.slotResult);

          // Integer
          sender.javaSignalint(42);
          QCoreApplication.processEvents();
          assertEquals(Integer.valueOf(42), receiver.slotResult);
          sender.javaSignalInteger(43);
          QCoreApplication.processEvents();
          assertEquals(Integer.valueOf(43), receiver.slotResult);

          // Long
          sender.javaSignalLong((long)44);
          QCoreApplication.processEvents();
          assertEquals(Long.valueOf(44), receiver.slotResult);
          sender.javaSignalLong((long) 45);
          QCoreApplication.processEvents();
          assertEquals(Long.valueOf(45), receiver.slotResult);

          // Float
          sender.javaSignalFloat((float) 3.14);
          QCoreApplication.processEvents();
          assertEquals(Float.valueOf(3.14f), receiver.slotResult);
          sender.javaSignalFloat((float) 3.15);
          QCoreApplication.processEvents();
          assertEquals(Float.valueOf(3.15f), receiver.slotResult);

          // Double
          sender.javaSignalDouble(3.16);
          QCoreApplication.processEvents();
          assertEquals(Double.valueOf(3.16), receiver.slotResult);
          sender.javaSignalDouble(3.17);
          QCoreApplication.processEvents();
          assertEquals(Double.valueOf(3.17), receiver.slotResult);

          // Strings
          sender.javaSignalString("once upon a time...");
          QCoreApplication.processEvents();
          assertEquals(receiver.slotResult, "once upon a time...");
      }

    private class ConnectInEmitTester {
    	QDeclarableSignals.Signal0 signal = new QDeclarableSignals.Signal0();
        boolean gotThusFar;
        public void function() {
            try {
                signal.connect(this::function);
                gotThusFar = true;
            } catch (Exception e) { }
        }
    }

    @Test public void testConnectInEmit() {
        ConnectInEmitTester c = new ConnectInEmitTester();
        c.signal.connect(c::function);
        c.signal.emit();
        assertTrue(c.gotThusFar);
    }

    class ArrayConnection{

        public QDeclarableSignals.Signal1<String> test1 =  new QDeclarableSignals.Signal1<>(String.class);
        public QDeclarableSignals.Signal1<String[]> test2 = new QDeclarableSignals.Signal1<>(String[].class);
        public QDeclarableSignals.Signal1<String[][]> test3 = new QDeclarableSignals.Signal1<>(String[][].class);
        public QDeclarableSignals.Signal1<Byte[]> test4 = new QDeclarableSignals.Signal1<>(Byte[].class);

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

        test.test1.connect((QMetaObject.Slot1<String>)System.out::println);
        test.test2.connect(test::slot1);
        test.test3.connect(test::slot2);
        test.test4.connect(test::slot3);

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

    static class Emitter implements Runnable {
    	QDeclarableSignals.Signal0 signal = new QDeclarableSignals.Signal0();
        boolean goodExit = false;

        public void run() {
            for (int i=0; i<50000; ++i) {
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
        e.signal.connect(e::slot1);
        t.start();

        try {
        	QMetaObject.Slot0 slot = e::slot2;
            for (int i=0; i<50000; ++i) {
                e.signal.connect(slot);
                e.signal.disconnect(slot);
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

        sas.metaObject().property("cppProperty").reset(sas);
        assertEquals("it was the darkest and stormiest night evar", sas.property("cppProperty").toString());
    }

    @Test public void standardClassName() {
        SignalsAndSlots sas = new SignalsAndSlots();
        assertEquals("SignalsAndSlots", sas.metaObject().className());
        assertEquals("QObject", sas.metaObject().superClass().className());
    }

    @Test public void standardPropertyCount() {
        SignalsAndSlots sas = new SignalsAndSlots();

        assertEquals(2, sas.metaObject().propertyCount());
        assertEquals(1, sas.metaObject().superClass().propertyCount());
    }

    @Test public void standardPropertyNames() {
        SignalsAndSlots sas = new SignalsAndSlots();

        assertEquals(2, sas.metaObject().propertyCount());
        List<QMetaProperty> list = sas.metaObject().properties();
        assertEquals("objectName", list.get(0).name());
        assertEquals("cppProperty", list.get(1).name());
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

        sas.metaObject().property("javaProperty").reset(sas);
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

        assertEquals(5, sas.metaObject().propertyCount());
        assertEquals(3, sas.metaObject().superClass().propertyCount());
    }

    @Test public void className() {
        SignalsAndSlotsWithMoreProperties sas = new SignalsAndSlotsWithMoreProperties();

        assertEquals("io::qt::autotests::TestConnections::SignalsAndSlotsWithMoreProperties",
                     sas.metaObject().className());
        assertEquals("io::qt::autotests::TestConnections::SignalsAndSlotsWithProperties",
                sas.metaObject().superClass().className());
    }

    @Test public void propertyNames() {
        SignalsAndSlotsWithMoreProperties sas = new SignalsAndSlotsWithMoreProperties();

        assertEquals(5, sas.metaObject().propertyCount());
        QList<QMetaProperty> properties = sas.metaObject().properties();
        assertEquals("objectName expected at index 0: ", "objectName", properties.get(0).name());
        assertEquals("cppProperty expected at index 1: ", "cppProperty", properties.get(1).name());
        assertEquals("javaProperty expected at index 2: ", "javaProperty", properties.get(2).name());
        assertEquals("otherJavaProperty expected at index 3: ", "otherJavaProperty", properties.get(3).name());
        assertEquals("readOnlyProperty expected at index 4: ", "readOnlyProperty", properties.get(4).name());
    }

    private static class JavaSignal extends SignalsAndSlots {
        public final Signal2<String, QByteArray> aJavaSignal = new Signal2<String, QByteArray>();
    }

    @Test public void javaEmitShouldEmitCpp() {
        JavaSignal javaSignal = new JavaSignal();

        assertTrue(javaSignal.javaSignalToCppSlot());

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
		
		@SuppressWarnings("unused")
		public void varArgs(String... args) {
            received = args;
        }

        public final Signal1<String[]> zootBaz = new Signal1<String[]>();
    }

    @Test
    public void signatureOfJavaMethodsWithArrays() {
        ArrayQObject aqo = new ArrayQObject();

        String signature = metaObjectMethodSignature(aqo, "fooBar");
        assertEquals("fooBar(JObjectArrayWrapper)", signature);
		QMetaObject.invokeMethod(aqo, "fooBar", Qt.ConnectionType.DirectConnection, (Object)new String[] { "hello", "world" });
        assertTrue(aqo.received instanceof String[]);

        String array[] = (String[])aqo.received;
        assertEquals(2, array.length);

        assertEquals("hello", array[0]);
        assertEquals("world", array[1]);
    }

    @Test
    public void signatureOfJavaVarArgsMethodsWithArrays() {
        ArrayQObject aqo = new ArrayQObject();

        String signature = metaObjectMethodSignature(aqo, "varArgs");
        assertEquals("varArgs(JObjectArrayWrapper)", signature);
		QMetaObject.invokeMethod(aqo, "varArgs", Qt.ConnectionType.DirectConnection, (Object)new String[] { "hello", "world" });
        assertTrue(aqo.received instanceof String[]);

        String array[] = (String[])aqo.received;
        assertEquals(2, array.length);

        assertEquals("hello", array[0]);
        assertEquals("world", array[1]);
    }
    
    private static String metaObjectMethodSignature(QObject object, String name) {
    	for (QMetaMethod method : object.metaObject().methods()) {
            if (method.methodSignature().contains(name)) {
                return method.cppMethodSignature().toString();
            }
        }

        return "";
    }

    @Test
    public void signatureOfJavaSignalWithArrays() {
        ArrayQObject aqo = new ArrayQObject();

        String signature = metaObjectMethodSignature(aqo, "zootBaz");
        assertEquals("zootBaz(JObjectArrayWrapper)", signature);

		Object connection = aqo.zootBaz.connect(aqo::fooBar);
        assertTrue("zootBaz connected", connection!=null);
		
		aqo.zootBaz.emit(new String[] { "goodday", "planet" });
		assertEquals(String[].class, aqo.received!=null ? aqo.received.getClass() : null);

        String array[] = (String[])aqo.received;
        assertEquals(2, array.length);

        assertEquals("goodday", array[0]);
        assertEquals("planet", array[1]);
		
		aqo.received = null;
		QMetaObject.invokeMethod(aqo, "zootBaz", Qt.ConnectionType.DirectConnection, (Object)new String[] { "goodday", "planet" });
        assertEquals(String[].class, aqo.received!=null ? aqo.received.getClass() : null);

        array = (String[])aqo.received;
        assertEquals(2, array.length);

        assertEquals("goodday", array[0]);
        assertEquals("planet", array[1]);

    }

    static class RecursiveSignalEmission extends QPushButton {
        public static final int COUNT = 10;

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
		e.clicked.connect(e::slot);
		e.slot();
		assertEquals(RecursiveSignalEmission.COUNT, e.emitted);
    }

    private boolean changeSignalReceived;
    private List<? extends Object> changeSignalResult = null;
    public void receiveChangedSignal(List<? extends Object> list) {
        synchronized(TestConnections.class) {
        	changeSignalReceived = true;
            changeSignalResult = list;
        }
    }

    @Test
    public void genericsBasedSignals() {
        synchronized(TestConnections.class) {
        	changeSignalReceived = false;
            changeSignalResult = null;
        }
        QGraphicsScene scene = new QGraphicsScene();
        scene.changed.connect(this::receiveChangedSignal);

        scene.addEllipse(new QRectF(0, 0, 10, 10));

        QApplication.processEvents();

        synchronized(TestConnections.class) {
            // MacOSX fails this test Sun/Oracle JDK 1.6.0u29 without wrapping variable access inside synchronized block
        	assertTrue(changeSignalReceived);
            assertTrue(changeSignalResult instanceof List);
        }
    }
	
	//@Test
    public void testRecursionBlockingSignals() {
		class Sender extends QObject{
			final Signal0 test = new Signal0();
			{
				test.connect(this::recursionTest);
			}
			
			int recursionCounter;
			private void recursionTest() {
				if(recursionCounter++==0) {
					test.emit();
				}
			}
		}
		Sender sender = new Sender();
		sender.test.emit();
		assertEquals(1, sender.recursionCounter);
	}
	
	@Test
    public void testNativeConnectionDeletion() throws NoSuchFieldException, SecurityException {
		QObject obj = new QObject();
		QMetaObject.Connection connection = obj.objectNameChanged.connect(name->{});
		obj.dispose();
		if(connection instanceof QtObjectInterface) {
			assertFalse(((QtObjectInterface)connection).isDisposed());
			assertFalse(connection.isConnected());
		}
	}
	
	@Test
    public void test_destroyed_signal() throws InterruptedException {
		Set<QObject> received = new HashSet<>();
		Set<QObject> expected = new HashSet<>();
		boolean[] dialogDestroyedAsNull = {false};
		{
			QColorDialog dialog = new QColorDialog();
			for(QObject child : dialog.children()) {
				expected.add(child);
				child.destroyed.connect(received::add);
				assertTrue("not cpp ownership", General.internalAccess.isCppOwnership(child));
			}
			dialog.destroyed.connect(c->{
				dialogDestroyedAsNull[0] = c==null;
			});
			dialog = null;
		}
		ApplicationInitializer.runGC();
        Thread.sleep(50);
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		ApplicationInitializer.runGC();
        Thread.sleep(50);
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		ApplicationInitializer.runGC();
        Thread.sleep(50);
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		ApplicationInitializer.runGC();
        Thread.sleep(50);
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		assertTrue(dialogDestroyedAsNull[0]);
		assertEquals(expected.size(), received.size());
		for (QObject object : expected) {
			assertTrue(received.contains(object));
		}
	}
	
	static class DestroyResponseObject extends QObject{
		public void onDestroyed(QObject o){
			System.out.println("DestroyResponseObject@"+System.identityHashCode(o));
		}
	}
	
	@Test
    public void test_destroyed_signal_self() throws InterruptedException {
		{
			DestroyResponseObject d = new DestroyResponseObject();
			d.destroyed.connect(o->d.onDestroyed(o));
		}
		ApplicationInitializer.runGC();
        Thread.sleep(50);
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	}
	
	@Test
    public void test_destroyed_signal_queued() throws InterruptedException {
		QObject object = new QObject();
		object.destroyed.connect(o -> {
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "destroyed: "+System.identityHashCode(o));
		}, Qt.ConnectionType.QueuedConnection);
		ApplicationInitializer.runGC();
        Thread.sleep(50);
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
        QCoreApplication.processEvents();
        // nothing should happen
	}
	
	@Test public void testLambdaSlot() {
		QObject object = new QObject();
		boolean[] called = {false};
		object.objectNameChanged.connect(()->{
			called[0] = true;
		});
		object.setObjectName("A");
		assertTrue(called[0]);
		class TestObject implements QInstanceMemberSignals, QtSignalEmitterInterface{
			public final Signal1<Throwable> exceptionOccurred = new Signal1<>(this);
		}
		TestObject emitter = new TestObject();
		called[0] = false;
		emitter.exceptionOccurred.connect(()->{
			called[0] = true;
		});
		emitter.exceptionOccurred.emit(new RuntimeException());
		assertTrue(called[0]);
	}
	
	@SuppressWarnings({ "rawtypes", "unchecked" })
	@Test public void testGenericTypedArguments() {
		class Sender extends QObject{
			public final Signal1<List<QObject>> objects = new Signal1<>();
			public final Signal1<List<String>> strings = new Signal1<>();
			public final Signal1<QList<QObject>> objectQList = new Signal1<>();
			public final Signal1<QList<String>> stringQList = new Signal1<>();
			public final Signal1<QStringList> qstringList = new Signal1<>();
			public final Signal1<@QtPointerType QList<String>> stringQListPointer = new Signal1<>();
			public final Signal1<@QtPointerType QStringList> qstringListPointer = new Signal1<>();
		}
		
		@SuppressWarnings("unused")
		class Receiver extends QObject{
			List<QObject> objects;
			List<String> strings;
			QStringList stringList;
			public void receiveStrings(List<String> strings) {
				this.strings = strings;
			}
			public void receiveStringList(QStringList strings) {
				this.stringList = strings;
				this.strings = strings;
			}
			public void receiveStringListPointer(@QtPointerType QStringList strings) {
				this.stringList = strings;
				this.strings = strings;
			}
			public void receiveObjects(QList<QObject> objects) {
				this.objects = objects;
			}
			void reset() {
				strings = null;
				stringList = null;
				objects = null;
			}
		}
		Sender sender = new Sender();
		Receiver receiver = new Receiver();
//		sender.metaObject().methods().forEach(m->System.out.println(m.cppMethodSignature()));
//		receiver.metaObject().methods().forEach(m->System.out.println(m.cppMethodSignature()));
		QObject obj = new QObject();
		QStringList stringList = new QStringList("TEST");
		QList<String> stringList2 = new QList<>(Arrays.asList("TEST")); // force qlist not to be qstringlist
		try {
			sender.objects.connect(receiver, "receiveStrings(List)");
			Assert.assertFalse("QMisfittingSignatureException expected to be thrown", true);
		} catch (QMisfittingSignatureException e) {
		}
		if(hasSerializableLambdas) {
			try {
				QMetaObject.Slot1<List<String>> slot = receiver::receiveStrings;
				sender.objects.connect((QMetaObject.Slot1)slot);
				Assert.assertFalse("QMisfittingSignatureException expected to be thrown", true);
			} catch (QMisfittingSignatureException e) {
			}
		}
		QMetaObject.Connection con = sender.objects.connect(receiver, "receiveObjects(QList)");
		assertTrue(con.isConnected());
		sender.objects.emit(QList.of(obj));
		assertTrue(receiver.objects instanceof QList);
		assertEquals(1, receiver.objects.size());
		assertEquals(obj, receiver.objects.get(0));
		receiver.reset();
		assertTrue(QObject.disconnect(con));
		
		con = sender.strings.connect(receiver, "receiveStringList(QStringList)");
		assertTrue(con.isConnected());
		sender.strings.emit(Arrays.asList("TEST"));
		assertTrue(receiver.strings instanceof QList);
		assertEquals(1, receiver.strings.size());
		assertEquals("TEST", receiver.strings.get(0));
		receiver.reset();
		assertTrue(con!=null);
		assertTrue(QObject.disconnect(con));
		
		con = sender.objectQList.connect(receiver, "receiveObjects(QList)");
		assertTrue(con.isConnected());
		sender.objectQList.emit(QList.of(obj));
		assertTrue(receiver.objects instanceof QList);
		assertEquals(1, receiver.objects.size());
		assertEquals(obj, receiver.objects.get(0));
		receiver.objects = null;
		assertTrue(QObject.disconnect(con));
		
		con = sender.stringQList.connect(receiver, "receiveStrings(List)");
		assertTrue(con.isConnected());
		sender.stringQList.emit(stringList);
		assertTrue(receiver.strings instanceof QList);
		assertEquals(1, receiver.strings.size());
		assertEquals("TEST", receiver.strings.get(0));
		receiver.reset();
		assertTrue(con.isConnected());
		assertTrue(QObject.disconnect(con));

		con = sender.stringQList.connect(receiver, "receiveStringList(QStringList)");
		assertTrue(con.isConnected());
		sender.stringQList.emit(stringList);
		assertTrue(receiver.strings instanceof QList);
		assertEquals(1, receiver.strings.size());
		assertEquals("TEST", receiver.strings.get(0));
		receiver.reset();
		assertTrue(con.isConnected());
		assertTrue(QObject.disconnect(con));
		
		con = sender.qstringList.connect(receiver, "receiveStrings(List)");
		assertTrue(con.isConnected());
		sender.qstringList.emit(stringList);
		assertTrue(receiver.strings instanceof QList);
		assertEquals(1, receiver.strings.size());
		assertEquals("TEST", receiver.strings.get(0));
		receiver.reset();
		assertTrue(con.isConnected());
		assertTrue(QObject.disconnect(con));

		con = sender.qstringList.connect(receiver, "receiveStringList(QStringList)");
		assertTrue(con.isConnected());
		sender.qstringList.emit(stringList);
		assertTrue(receiver.strings instanceof QList);
		assertTrue(receiver.strings!=stringList);
		assertEquals(1, receiver.strings.size());
		assertEquals("TEST", receiver.strings.get(0));
		receiver.reset();
		assertTrue(con.isConnected());
		assertTrue(QObject.disconnect(con));
		
		con = sender.qstringListPointer.connect(receiver, "receiveStringList(QStringList)");
		assertTrue(con.isConnected());
		sender.qstringListPointer.emit(stringList);
		if(hasSerializableLambdas) {
			assertTrue(receiver.strings == stringList);
		}
		receiver.reset();
		assertTrue(con.isConnected());
		assertTrue(QObject.disconnect(con));
		
		con = sender.qstringListPointer.connect(receiver, "receiveStringListPointer(QStringList)");
		assertTrue(con.isConnected());
		sender.qstringListPointer.emit(stringList);
		if(hasSerializableLambdas) {
			assertTrue(receiver.strings == stringList);
		}
		receiver.reset();
		assertTrue(con.isConnected());
		assertTrue(QObject.disconnect(con));
		
		con = sender.stringQListPointer.connect(receiver, "receiveStringList(QStringList)");
		assertTrue(con.isConnected());
		sender.stringQListPointer.emit(stringList);
		if(hasSerializableLambdas) {
			assertTrue(receiver.strings == stringList);
		}
		receiver.reset();
		assertTrue(con.isConnected());
		assertTrue(QObject.disconnect(con));
		
		con = sender.stringQListPointer.connect(receiver, "receiveStringListPointer(QStringList)");
		assertTrue(con.isConnected());
		sender.stringQListPointer.emit(stringList2);
		assertTrue(receiver.stringList instanceof QStringList);
		if(hasSerializableLambdas) {
			assertTrue(receiver.stringList.isDisposed());
		}
		receiver.reset();
		assertTrue(con.isConnected());
		assertTrue(QObject.disconnect(con));
	}
	
	@Test(expected=NotSerializableException.class)
	public void testSignalSerialization() throws IOException {
		QObject object = new QObject();
		ByteArrayOutputStream bos = new ByteArrayOutputStream();
		ObjectOutputStream serializer = new ObjectOutputStream(bos);
		serializer.writeObject(object.destroyed);
	}
	
	@Test(expected=NotSerializableException.class)
	public void testSlotSerialization() throws IOException {
		QObject object = new QObject();
		QMetaObject.Slot0 slot = object::disposeLater;
		ByteArrayOutputStream bos = new ByteArrayOutputStream();
		ObjectOutputStream serializer = new ObjectOutputStream(bos);
		serializer.writeObject(slot);
	}
	
    public static void main(String args[]) {
    	org.junit.runner.JUnitCore.main(TestConnections.class.getName());
    }
}


class SignalsAndSlotsSubclass extends SignalsAndSlots
{
    public int java_slot2_called = 0;
    public int java_slot3_2_called = 0;
    public int java_slot4_called = 0;
    public int java_non_slot_called = 0;
    public int java_slot5_called = 0;

    public final Signal0 signal4 = new Signal0();

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

    @QtUninvokable()
    public void non_slot() {
        java_non_slot_called ++;
    }

}

class NonQObject {
	@QtInvokable
	private void foobar(String foo, int bar) {
        receivedBar = bar;
        receivedFoo = foo;
    }

    public String receivedFoo;
    public int receivedBar;
}
