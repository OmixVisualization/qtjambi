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

import static org.junit.Assert.*;

import java.util.Arrays;

import org.junit.Assert;
import org.junit.Test;

import io.qt.QtPrimitiveType;
import io.qt.core.QDeclarableSignals;
import io.qt.core.QInstanceMemberSignals;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QObject;
import io.qt.core.QStaticMemberSignals;
import io.qt.widgets.QAction;
import io.qt.widgets.QCompleter;

public class TestSignals extends QApplicationTest{
	
	public static class StaticMemberSignalOwner{
		public final static QStaticMemberSignals.Signal2<Integer, String> testSinal = new QStaticMemberSignals.Signal2<>();
	}
	
	public static class NonStaticMemberSignalOwner{
		public final QStaticMemberSignals.Signal2<Integer, String> testSinal = new QStaticMemberSignals.Signal2<>();
	}
	
	public static class MemberSignalOwner extends QObject{
		public final Signal2<@QtPrimitiveType Integer, String> testSinal = new Signal2<>();
	}
	
	public static class InstanceMemberSignalOwnerQObject extends QObject implements QInstanceMemberSignals{
		public final QInstanceMemberSignals.Signal1<String> testSinal = new QInstanceMemberSignals.Signal1<>(this);
	}
	
	public static class DeclarableSignalOwnerQObject extends QObject{
		public final QDeclarableSignals.Signal1<String> testSinal = new QDeclarableSignals.Signal1<>(String.class);
	}
	
	@Test
    public void testInstanceMemberSignalInQObject() {
		InstanceMemberSignalOwnerQObject o = new InstanceMemberSignalOwnerQObject();
		QCompleter receiver = new QCompleter();
		o.testSinal.connect(receiver::setCompletionPrefix);
		assertEquals("", receiver.completionPrefix());
		o.testSinal.emit("test1");
		assertEquals("test1", receiver.completionPrefix());
		QMetaMethod m = QMetaMethod.fromSignal(o.testSinal);
		assertTrue(m!=null);
		m.invoke(o, "test2");
		assertEquals("test2", receiver.completionPrefix());
	}
	
	@Test
    public void testDeclarableMemberSignalInQObject() {
		DeclarableSignalOwnerQObject o = new DeclarableSignalOwnerQObject();
		QCompleter receiver = new QCompleter();
		o.testSinal.connect(receiver::setCompletionPrefix);
		assertEquals("", receiver.completionPrefix());
		o.testSinal.emit("test1");
		assertEquals("test1", receiver.completionPrefix());
		QMetaMethod m = QMetaMethod.fromSignal(o.testSinal);
		assertTrue(m==null);
	}
	
    @Test
    public void testStaticMemberSignal() {
		assertEquals(Arrays.asList(Integer.class, String.class), StaticMemberSignalOwner.testSinal.argumentTypes());
    }
    
    @Test
    public void testMemberSignal() {
		assertEquals(Arrays.asList(int.class, String.class), new MemberSignalOwner().testSinal.argumentTypes());
    }
    
    @Test
    public void testNonStaticMemberSignal() {
    	try {
			assertEquals(Arrays.asList(Integer.class, String.class), new NonStaticMemberSignalOwner().testSinal.argumentTypes());
			Assert.assertTrue(false);
		} catch (RuntimeException e) {
			assertEquals(RuntimeException.class, e.getClass());
			assertEquals("Static signals must be declared as static members of a class.", e.getMessage());
		}
    }
    
    @Test
    public void testNonStaticLocalSignal() {
    	try {
	    	QStaticMemberSignals.Signal2<@QtPrimitiveType Integer, String> testSinal = new QStaticMemberSignals.Signal2<>();
	    	assertEquals(Arrays.asList(int.class, String.class), testSinal.argumentTypes());
			Assert.assertTrue(false);
		} catch (RuntimeException e) {
			assertEquals(RuntimeException.class, e.getClass());
			assertEquals("Static signals must be declared as static members of a class.", e.getMessage());
		}
    }
    
    @Test
    public void testLocalSignal() {
		assertEquals(Arrays.asList(int.class, String.class), new QDeclarableSignals.Signal2<>("testSinal", int.class, String.class).argumentTypes());
    }
    
    @Test
    public void testQObjectSignal_connect_to_QObject() {
    	QObject sender = new QObject();
    	class Receiver extends QObject{
    		String name;
    		public void receiveName(String name) {
    			this.name = name;
    		}
    	}
    	Receiver receiver = new Receiver();
    	sender.objectNameChanged.connect(receiver::receiveName);
    	sender.setObjectName("TEST");
    	assertEquals("TEST", receiver.name);
    	sender.objectNameChanged.disconnect(receiver::receiveName);
    	sender.setObjectName("TEST2");
    	assertEquals("TEST", receiver.name);
    }
    
    @Test
    public void testQObjectSignal_connect_to_Object() {
    	QObject sender = new QObject();
    	class Receiver{
    		String name;
    		public void receiveName(String name) {
    			this.name = name;
    		}
    	}
    	Receiver receiver = new Receiver();
    	sender.objectNameChanged.connect(receiver::receiveName);
    	sender.setObjectName("TEST");
    	assertEquals("TEST", receiver.name);
    	sender.objectNameChanged.disconnect(receiver::receiveName);
    	sender.setObjectName("TEST2");
    	assertEquals("TEST", receiver.name);
    }
    
    @Test
    public void testQObjectSignal_connect_to_Lambda() {
    	QObject sender = new QObject();
    	class Receiver{
    		String name;
    		public void receiveName(String name, int i) {
    			this.name = name;
    		}
    	}
    	Receiver receiver = new Receiver();
    	QMetaObject.Slot1<String> slot = name -> receiver.receiveName(name, receiver.hashCode());
    	sender.objectNameChanged.connect(slot);
    	sender.setObjectName("TEST");
    	assertEquals("TEST", receiver.name);
    	sender.objectNameChanged.disconnect(slot);
    	sender.setObjectName("TEST2");
    	assertEquals("TEST", receiver.name);
    }
    
    @Test
    public void test_custom_QObject_signal_connected_to_QObject() {
    	class Sender extends QObject{
    		public final Signal1<String> testSignal = new Signal1<>();
    	}
    	Sender sender = new Sender();
    	class Receiver extends QObject{
    		String name;
    		public void receiveName(String name) {
    			this.name = name;
    		}
    	}
    	Receiver receiver = new Receiver();
    	sender.testSignal.connect(receiver::receiveName);
    	sender.testSignal.emit("TEST");
    	assertEquals("TEST", receiver.name);
    	sender.testSignal.disconnect(receiver::receiveName);
    	sender.testSignal.emit("TEST2");
    	assertEquals("TEST", receiver.name);
    }
    
    @Test
    public void test_custom_QObject_signal_connected_to_Object() {
    	class Sender extends QObject{
    		public final Signal1<String> testSignal = new Signal1<>();
    	}
    	Sender sender = new Sender();
    	class Receiver{
    		String name;
    		public void receiveName(String name) {
    			this.name = name;
    		}
    	}
    	Receiver receiver = new Receiver();
    	sender.testSignal.connect(receiver::receiveName);
    	sender.testSignal.emit("TEST");
    	assertEquals("TEST", receiver.name);
    	sender.testSignal.disconnect(receiver::receiveName);
    	sender.testSignal.emit("TEST2");
    	assertEquals("TEST", receiver.name);
    }
    
    @Test
    public void test_custom_QObject_anyarg_signal_connected_to_QObject() {
		class Data{}
    	class Sender extends QObject{
    		public final Signal1<Data> testSignal = new Signal1<>();
    	}
    	Sender sender = new Sender();
    	class Receiver extends QObject{
    		Data data;
    		public void receiveData(Data data) {
    			this.data = data;
    		}
    	}
    	Data data = new Data();
    	Receiver receiver = new Receiver();
    	sender.testSignal.connect(receiver::receiveData);
    	sender.testSignal.emit(data);
    	assertEquals(data, receiver.data);
    	sender.testSignal.disconnect(receiver::receiveData);
    	sender.testSignal.emit(new Data());
    	assertEquals(data, receiver.data);
    }
    
    @Test
    public void test_custom_QObject_anyarg_signal_connected_to_Object() {
		class Data{}
    	class Sender extends QObject{
    		public final Signal1<Data> testSignal = new Signal1<>();
    	}
    	Sender sender = new Sender();
    	class Receiver{
    		Data data;
    		public void receiveData(Data data) {
    			this.data = data;
    		}
    	}
    	Data data = new Data();
    	Receiver receiver = new Receiver();
    	sender.testSignal.connect(receiver::receiveData);
    	sender.testSignal.emit(data);
    	assertEquals(data, receiver.data);
    	sender.testSignal.disconnect(receiver::receiveData);
    	sender.testSignal.emit(new Data());
    	assertEquals(data, receiver.data);
    }
    
    @Test
    public void testQActionSignal_connect_to_QObject() {
    	QAction sender = new QAction();
    	class Receiver extends QObject{
    		boolean toggled;
    		public void toggled(boolean toggled) {
    			this.toggled = toggled;
    		}
    	}
    	Receiver receiver = new Receiver();
    	sender.toggled.connect(receiver::toggled);
    	sender.toggled.emit(true);
    	assertEquals(true, receiver.toggled);
    	sender.toggled.disconnect(receiver::toggled);
    	sender.toggled.emit(false);
    	assertEquals(true, receiver.toggled);
    }
    
    @Test
    public void testQActionSignal_connect_to_Object() {
    	QAction sender = new QAction();
    	class Receiver{
    		boolean toggled;
    		public void toggled(boolean toggled) {
    			this.toggled = toggled;
    		}
    	}
    	Receiver receiver = new Receiver();
    	sender.toggled.connect(receiver::toggled);
    	sender.toggled.emit(true);
    	assertEquals(true, receiver.toggled);
    	sender.toggled.disconnect(receiver::toggled);
    	sender.toggled.emit(false);
    	assertEquals(true, receiver.toggled);
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestSignals.class.getName());
    }
}
