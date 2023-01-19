/****************************************************************************
**
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
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import io.qt.QtPrimitiveType;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaObject.Connection;
import io.qt.core.QObject;
import io.qt.core.QOperatingSystemVersion;

public class TestDefaultSignals extends ApplicationInitializer {
    private static class Sender extends QObject {
    	public final Signal2Default1<String, @QtPrimitiveType Integer> signal21 = new Signal2Default1<>(()->7654);
    	public final Signal2Default2<String, @QtPrimitiveType Integer> signal22 = new Signal2Default2<>(()->"No arg", ()->12345);
    }
    
    private static class Receiver{
    	boolean received;
    	String received1;
    	Integer received2;
    	
    	void receive2(String s, int i) {
    		received1 = s;
    		received2 = i;
    		received = true;
    	}
    	
    	void receive1(String s) {
    		received1 = s;
    		received = true;
    	}
    	
    	void receive0() {
    		received = true;
    	}
    	
    	void reset() {
    		received1 = null;
    		received2 = null;
    		received = false;
    	}
    }
    
	@SuppressWarnings("unused")
    private static class QReceiver extends QObject{
    	boolean received;
    	String received1;
    	Integer received2;
    	QObject destroyed;
    	int destroyedCalled;
    	
    	void receive2(String s, int i) {
    		received1 = s;
    		received2 = i;
    		received = true;
    	}
    	
		void receive1(String s) {
    		received1 = s;
    		received = true;
    	}
		
		void onDestroyed(QObject obj) {
			destroyed = obj;
			destroyedCalled = 2;
		}
		
		void onDestroyed() {
			destroyed = null;
			destroyedCalled = 1;
		}
    }
    
    @Test
    public void testNativeDefaultSignalInvocation() {
    	QObject sent = new QObject();
    	Sender sender = new Sender();
//    	sender.metaObject().methods().forEach(m->System.out.println(m.methodIndex()+": "+m.cppMethodSignature()));
    	QMetaMethod method = sender.metaObject().method("signal22(String)");
    	QReceiver receiver = new QReceiver();
    	QMetaObject.Connection con = sender.signal22.connect(receiver::receive2);
    	assertTrue(con.isConnected());
    	assertEquals(QMetaMethod.fromSignal(sender.signal22), io.qt.internal.TestUtility.signal(con));
    	method.invoke(sender, "TEST");
    	assertTrue("did not receive", receiver.received);
    	assertEquals(Integer.valueOf(12345), receiver.received2);
    	assertEquals("TEST", receiver.received1);
    	receiver.received = false;
    	receiver.received1 = null;
    	receiver.received2 = null;
    	sender.signal22.emit();
    	assertTrue(receiver.received);
    	assertEquals(Integer.valueOf(12345), receiver.received2);
    	assertEquals("No arg", receiver.received1);
    	QObject.disconnect(con);
    	
    	receiver.received = false;
    	receiver.received1 = null;
    	receiver.received2 = null;
    	con = QObject.connect(sender, "signal22(String)", receiver, "receive1(String)");
    	assertTrue(con!=null);
    	assertEquals(QMetaMethod.fromSignal(sender.signal22), io.qt.internal.TestUtility.signal(con));
    	method.invoke(sender, "TEST");
    	assertTrue("did not receive", receiver.received);
    	assertEquals(null, receiver.received2);
    	assertEquals("TEST", receiver.received1);
    	QObject.disconnect(con);
//    	sender.metaObject().methods().forEach(m->System.out.println(m.cppMethodSignature()));
    	
    	con = QObject.connect(sender, "destroyed()", receiver, "onDestroyed()");
    	assertTrue(con!=null);
    	assertEquals(QMetaMethod.fromSignal(sender.destroyed), io.qt.internal.TestUtility.signal(con));
    	method = sender.metaObject().method("destroyed()");
    	method.invoke(sender);
    	assertEquals(1, receiver.destroyedCalled);
    	assertEquals(null, receiver.destroyed);
    	receiver.destroyedCalled = 0;
    	receiver.destroyed = null;
    	sender.destroyed.emit(sent);
    	assertEquals(1, receiver.destroyedCalled);
    	assertEquals(null, receiver.destroyed);
    	QObject.disconnect(con);
    	
    	con = QObject.connect(sender, "destroyed(QObject)", receiver, "onDestroyed(QObject)");
    	assertTrue(con!=null);
    	assertEquals(QMetaMethod.fromSignal(sender.destroyed), io.qt.internal.TestUtility.signal(con));
    	receiver.destroyedCalled = 0;
    	receiver.destroyed = null;
    	sender.destroyed.emit(sent);
    	assertEquals(2, receiver.destroyedCalled);
    	assertEquals(sent, receiver.destroyed);
    	receiver.destroyedCalled = 0;
    	receiver.destroyed = null;
    	method.invoke(sender);
    	assertEquals(2, receiver.destroyedCalled);
    	assertEquals(null, receiver.destroyed);
    	QObject.disconnect(con);
    }

    @Test
    public void test_emit_DefaultSignal() {
    	Sender sender = new Sender();
    	Receiver receiver = new Receiver();
    	Connection connection1 = sender.signal21.connect(receiver::receive1);
    	Connection connection2 = sender.signal22.connect(receiver::receive2);
    	assertTrue("signal22 not connected", connection2!=null && connection2.isConnected());
    	assertTrue("signal21 not connected", connection1!=null && connection1.isConnected());
    	
    	assertNull("received1 not null", receiver.received1);
    	assertNull("received2 not null", receiver.received2);
    	assertFalse("received", receiver.received);
    	sender.signal22.emit("TEST", 5);
    	assertEquals("received1", "TEST", receiver.received1);
    	assertEquals("received2", Integer.valueOf(5), receiver.received2);
    	assertTrue("received", receiver.received);
    	receiver.reset();
    	
    	assertNull("received1 not null", receiver.received1);
    	assertNull("received2 not null", receiver.received2);
    	assertFalse("did receive unexpectedly", receiver.received);
    	sender.signal22.emit("TEST");
    	assertEquals("TEST", receiver.received1);
    	assertEquals(Integer.valueOf(12345), receiver.received2);
    	assertTrue("did not receive", receiver.received);
    	receiver.reset();
    	
    	assertNull("received1 not null", receiver.received1);
    	assertNull("received2 not null", receiver.received2);
    	assertFalse("did receive unexpectedly", receiver.received);
    	sender.signal22.emit();
    	assertEquals("No arg", receiver.received1);
    	assertEquals(Integer.valueOf(12345), receiver.received2);
    	assertTrue("did not receive", receiver.received);
    	receiver.reset();
    	
    	assertNull("received1 not null", receiver.received1);
    	assertNull("received2 not null", receiver.received2);
    	assertFalse("did receive unexpectedly", receiver.received);
    	sender.signal21.emit("TEST", 5);
    	assertEquals("TEST", receiver.received1);
    	assertNull("received2 not null", receiver.received2);
    	assertTrue("did not receive", receiver.received);
    	receiver.reset();
    	
    	assertNull("received1 not null", receiver.received1);
    	assertNull("received2 not null", receiver.received2);
    	assertFalse("did receive unexpectedly", receiver.received);
    	sender.signal21.emit("TEST");
    	assertEquals("TEST", receiver.received1);
    	assertNull("received2 not null", receiver.received2);
    	assertTrue("did not receive", receiver.received);
    	receiver.reset();
    	
    	if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)) {
    		assertTrue(QObject.disconnect(connection1));
    		assertTrue(QObject.disconnect(connection2));
    	}else {
	    	assertTrue(sender.signal22.disconnect(receiver::receive2));
	    	assertTrue(sender.signal21.disconnect(receiver::receive1));
    	}
    	assertNull("received1 not null", receiver.received1);
    	assertNull("received2 not null", receiver.received2);
    	assertFalse("did receive unexpectedly", receiver.received);
    	sender.signal21.emit("TEST");
    	sender.signal22.emit("TEST");
    	assertNull("received1 not null", receiver.received1);
    	assertNull("received2 not null", receiver.received2);
    	assertFalse("did receive unexpectedly", receiver.received);

    	connection1 = sender.signal21.connect(receiver::receive0);
    	connection2 = sender.signal22.connect(receiver::receive1);
    	assertTrue("signal22 not connected", connection2!=null && connection2.isConnected());
    	assertTrue("signal21 not connected", connection1!=null && connection1.isConnected());
    	
    	assertNull("received1 not null", receiver.received1);
    	assertNull("received2 not null", receiver.received2);
    	assertFalse("did receive unexpectedly", receiver.received);
    	sender.signal22.emit("TEST", 5);
    	assertEquals("TEST", receiver.received1);
    	assertNull("received2 not null", receiver.received2);
    	assertTrue("did not receive", receiver.received);
    	receiver.reset();
    	
    	assertNull("received1 not null", receiver.received1);
    	assertNull("received2 not null", receiver.received2);
    	assertFalse("did receive unexpectedly", receiver.received);
    	sender.signal22.emit("TEST");
    	assertEquals("TEST", receiver.received1);
    	assertNull("received2 not null", receiver.received2);
    	assertTrue("did not receive", receiver.received);
    	receiver.reset();
    	
    	assertNull("received1 not null", receiver.received1);
    	assertNull("received2 not null", receiver.received2);
    	assertFalse("did receive unexpectedly", receiver.received);
    	sender.signal22.emit();
    	assertEquals("No arg", receiver.received1);
    	assertNull("received2 not null", receiver.received2);
    	assertTrue("did not receive", receiver.received);
    	receiver.reset();
    	
    	assertNull("received1 not null", receiver.received1);
    	assertNull("received2 not null", receiver.received2);
    	assertFalse("did receive unexpectedly", receiver.received);
    	sender.signal21.emit("TEST", 5);
    	assertNull("received1 not null", receiver.received1);
    	assertNull("received2 not null", receiver.received2);
    	assertTrue("did not receive", receiver.received);
    	receiver.reset();
    	
    	assertNull("received1 not null", receiver.received1);
    	assertNull("received2 not null", receiver.received2);
    	assertFalse("did receive unexpectedly", receiver.received);
    	sender.signal21.emit("TEST");
    	assertNull("received1 not null", receiver.received1);
    	assertNull("received2 not null", receiver.received2);
    	assertTrue("did not receive", receiver.received);
    	receiver.reset();    	
    	
    	if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)) {
    		assertTrue(QObject.disconnect(connection1));
    		assertTrue(QObject.disconnect(connection2));
    	}else {
	    	assertTrue(sender.signal22.disconnect(receiver::receive1));
	    	assertTrue(sender.signal21.disconnect(receiver::receive0));
    	}
    	
    	assertNull("received1 not null", receiver.received1);
    	assertNull("received2 not null", receiver.received2);
    	assertFalse("did receive unexpectedly", receiver.received);
    	sender.signal21.emit("TEST");
    	sender.signal22.emit("TEST");
    	assertNull("received1 not null", receiver.received1);
    	assertNull("received2 not null", receiver.received2);
    	assertFalse("did receive unexpectedly", receiver.received);

    	connection2 = sender.signal22.connect(receiver::receive0);
    	assertTrue(connection2!=null && connection2.isConnected());
    	
    	assertNull("received1 not null", receiver.received1);
    	assertNull("received2 not null", receiver.received2);
    	assertFalse("did receive unexpectedly", receiver.received);
    	sender.signal22.emit("TEST", 5);
    	assertNull("received1 not null", receiver.received1);
    	assertNull("received2 not null", receiver.received2);
    	assertTrue("did not receive", receiver.received);
    	receiver.reset();
    	
    	assertNull("received1 not null", receiver.received1);
    	assertNull("received2 not null", receiver.received2);
    	assertFalse("did receive unexpectedly", receiver.received);
    	sender.signal22.emit("TEST");
    	assertNull("received1 not null", receiver.received1);
    	assertNull("received2 not null", receiver.received2);
    	assertTrue("did not receive", receiver.received);
    	receiver.reset();
    	
    	assertNull("received1 not null", receiver.received1);
    	assertNull("received2 not null", receiver.received2);
    	assertFalse("did receive unexpectedly", receiver.received);
    	sender.signal22.emit();
    	assertNull("received1 not null", receiver.received1);
    	assertNull("received2 not null", receiver.received2);
    	assertTrue("did not receive", receiver.received);
    	receiver.reset();
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestDefaultSignals.class.getName());
    }
}
