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
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.fail;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.concurrent.atomic.AtomicBoolean;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QSignalAccessException;
import io.qt.QSignalDeclarationException;
import io.qt.QSignalInitializationException;
import io.qt.QtPrimitiveType;
import io.qt.QtSignalEmitterInterface;
import io.qt.core.QDeclarableSignals;
import io.qt.core.QInstanceMemberSignals;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QObject;
import io.qt.core.QStaticMemberSignals;
import io.qt.core.Qt;
import io.qt.gui.*;
import io.qt.gui.QColor;
import io.qt.internal.ClassAnalyzerUtility;
import io.qt.widgets.*;
import io.qt.widgets.QWidget;
import io.qt.widgets.QWizard;

@SuppressWarnings("unused")
public class TestSignals extends ApplicationInitializer{
	
	private static boolean hasSerializableLambdas;
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    	hasSerializableLambdas = ClassAnalyzerUtility.serializeLambdaExpression((QMetaObject.Slot0)ApplicationInitializer::testInitializeWithWidgets) != null;
    }
	
	public static class QObjectSignalOwner extends QObject{
		final Signal1<String> testSignal = new Signal1<>();
	}
	
	public static class QObjectNonFinalSignalOwner extends QObject{
		Signal1<String> testSignal = new Signal1<>();
	}
	
	public static class QObjectStaticSignalOwner extends QObject{
		static Signal1<String> testSignal;
	}
	
	public static class StaticMemberSignalOwner{
		public final static QStaticMemberSignals.Signal2<Integer, String> testSignal = new QStaticMemberSignals.Signal2<>();
	}
	
	public static class NonStaticMemberSignalOwner{
		public final QStaticMemberSignals.Signal2<Integer, String> testSignal = new QStaticMemberSignals.Signal2<>();
	}
	
	public static class MemberSignalOwner extends QObject{
		public final Signal2<@QtPrimitiveType Integer, String> testSignal = new Signal2<>();
	}
	
	public static class InstanceMemberSignalOwnerQObject extends QObject implements QInstanceMemberSignals{
		public final QInstanceMemberSignals.Signal1<String> testSignal = new QInstanceMemberSignals.Signal1<>(this);
	}
	
	public static class DeclarableSignalOwnerQObject extends QObject{
		public final QDeclarableSignals.Signal1<String> testSignal = new QDeclarableSignals.Signal1<>(String.class);
	}
	
	public static class GenericSender<E> extends QObject {
        public final Signal1<E> genericSignal = new Signal1<>();
        public void genericSignal(E value) {
        	this.value = value;
        }
        private E value;
        public GenericSender(){
        	genericSignal.connect(this::genericSignal);
        }
    }
	
	public static class GenericReceiver<E> extends QObject {
        private E value;
		public E value() {
			return value;
		}
		public void setValue(E value) {
			this.value = value;
		}
    }
	
	private static Object[] receivedK = {null};
	
	private static <K> void receive(K value) {
		receivedK[0] = value;
	}
	
	@Test
    public void testGenericSignal() {
		GenericSender<String> sender = new GenericSender<>();
		GenericSender<String> sender2 = new GenericSender<>();
		GenericReceiver<String> receiver = new GenericReceiver<>();
		String[] result = {null};
		QMetaObject.Connection connection = sender.genericSignal.connect(v->{result[0] = v;});
		QMetaObject.Connection connection2 = sender2.genericSignal.connect(sender.genericSignal);
		sender.genericSignal.emit("TEST");
		Assert.assertEquals("TEST", result[0]);
		result[0] = null;
		sender2.genericSignal.emit("TEST2");
		Assert.assertEquals("TEST2", result[0]);
		result[0] = null;
		
//		try {
//			((QObject.Signal1<Integer>)(QObject.Signal1<?>)sender2.genericSignal).emit(5);
//			Assert.fail();
//		} catch (ClassCastException e1) {
//		}
		
		sender.genericSignal.disconnect(connection);
		connection = sender.genericSignal.connect(receiver::setValue);
		sender.genericSignal.emit("TEST3");
		Assert.assertEquals("TEST3", receiver.value());
		sender.genericSignal.disconnect(connection);

		connection = sender.genericSignal.connect(e ->receiver.setValue(e));
		sender.genericSignal.emit("TEST4");
		Assert.assertEquals("TEST4", receiver.value());
		sender.genericSignal.disconnect(connection);
		
		connection = sender.genericSignal.connect(TestSignals::receive);
		sender.genericSignal.emit("TEST5");
		Assert.assertEquals("TEST5", receivedK[0]);
		receivedK[0] = null;
		sender.genericSignal.disconnect(connection);
	}
	
	@Test
    public void testQObjectSignal() {
		try {
			new QObjectStaticSignalOwner();
		} catch (RuntimeException e) {
			assertEquals(QSignalDeclarationException.class, e.getClass());
			assertEquals("Modifier 'static' not allowed for signal QObjectStaticSignalOwner.testSignal. Use QStaticMemberSignals instead to declare a static signal.", e.getMessage());
		}
		try {
			new QObjectNonFinalSignalOwner();
		} catch (RuntimeException e) {
			assertEquals(QSignalDeclarationException.class, e.getClass());
			assertEquals("Missing modifier 'final' at signal QObjectNonFinalSignalOwner.testSignal.", e.getMessage());
		}
		QObjectSignalOwner obj2 = new QObjectSignalOwner();
		obj2.metaObject().methods().forEach(m->java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, m.cppMethodSignature().toString()));
	}
	
	@Test
    public void testInstanceMemberSignalInQObject() {
		InstanceMemberSignalOwnerQObject o = new InstanceMemberSignalOwnerQObject();
		QCompleter receiver = new QCompleter();
		o.testSignal.connect(receiver::setCompletionPrefix);
		assertEquals("", receiver.completionPrefix());
		o.testSignal.emit("test1");
		assertEquals("test1", receiver.completionPrefix());
		QMetaMethod m = QMetaMethod.fromSignal(o.testSignal);
		assertTrue(m.isValid());
		m.invoke(o, "test2");
		assertEquals("test2", receiver.completionPrefix());
	}
	
	@Test
    public void testDeclarableMemberSignalInQObject() {
		DeclarableSignalOwnerQObject o = new DeclarableSignalOwnerQObject();
		QCompleter receiver = new QCompleter();
		o.testSignal.connect(receiver::setCompletionPrefix);
		assertEquals("", receiver.completionPrefix());
		o.testSignal.emit("test1");
		assertEquals("test1", receiver.completionPrefix());
		QMetaMethod m = QMetaMethod.fromSignal(o.testSignal);
		assertFalse(m.isValid());
	}
	
    @Test
    public void testStaticMemberSignal() {
		assertEquals(Arrays.asList(Integer.class, String.class), StaticMemberSignalOwner.testSignal.argumentTypes());
    }
    
    @Test
    public void testMemberSignal() {
    	Assume.assumeTrue(ClassAnalyzerUtility.useAnnotatedType);
		assertEquals(Arrays.asList(int.class, String.class), new MemberSignalOwner().testSignal.argumentTypes());
    }
    
    @Test
    public void testNonStaticMemberSignal() {
    	try {
			assertEquals(Arrays.asList(Integer.class, String.class), new NonStaticMemberSignalOwner().testSignal.argumentTypes());
			Assert.assertTrue(false);
		} catch (RuntimeException e) {
			assertEquals(QSignalInitializationException.class, e.getClass());
			assertEquals("Static signals must be declared as static members of a class.", e.getMessage());
		}
    }
    
    @Test
    public void testNonStaticLocalSignal() {
    	try {
	    	QStaticMemberSignals.Signal2<@QtPrimitiveType Integer, String> testSignal = new QStaticMemberSignals.Signal2<>();
	    	assertEquals(Arrays.asList(int.class, String.class), testSignal.argumentTypes());
			Assert.assertTrue(false);
		} catch (RuntimeException e) {
			assertEquals(QSignalInitializationException.class, e.getClass());
			assertEquals("Static signals must be declared as static members of a class.", e.getMessage());
		}
    }
    
    @Test
    public void testLocalSignal() {
		assertEquals(Arrays.asList(int.class, String.class), new QDeclarableSignals.Signal2<>("testSignal", int.class, String.class).argumentTypes());
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
    	QMetaObject.Connection connection = sender.objectNameChanged.connect(receiver::receiveName);
    	sender.setObjectName("TEST");
    	assertEquals("TEST", receiver.name);
    	if(hasSerializableLambdas)
    		sender.objectNameChanged.disconnect(receiver::receiveName);
    	else
    		sender.objectNameChanged.disconnect(connection);
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
    	QMetaObject.Connection connection = sender.objectNameChanged.connect(receiver::receiveName);
    	sender.setObjectName("TEST");
    	assertEquals("TEST", receiver.name);
    	if(hasSerializableLambdas)
    		sender.objectNameChanged.disconnect(receiver::receiveName);
    	else
    		sender.objectNameChanged.disconnect(connection);
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
    	QMetaObject.Connection connection = sender.testSignal.connect(receiver::receiveName);
    	sender.testSignal.emit("TEST");
    	assertEquals("TEST", receiver.name);
    	if(hasSerializableLambdas)
    		sender.testSignal.disconnect(receiver::receiveName);
    	else
    		sender.testSignal.disconnect(connection);
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
    	QMetaObject.Connection connection = sender.testSignal.connect(receiver::receiveName);
    	sender.testSignal.emit("TEST");
    	assertEquals("TEST", receiver.name);
    	if(hasSerializableLambdas)
    		sender.testSignal.disconnect(receiver::receiveName);
    	else
    		sender.testSignal.disconnect(connection);
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
    	QMetaObject.Connection connection = sender.testSignal.connect(receiver::receiveData);
    	sender.testSignal.emit(data);
    	assertEquals(data, receiver.data);
    	if(hasSerializableLambdas)
    		sender.testSignal.disconnect(receiver::receiveData);
    	else
    		sender.testSignal.disconnect(connection);
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
    	QMetaObject.Connection connection = sender.testSignal.connect(receiver::receiveData);
    	sender.testSignal.emit(data);
    	assertEquals(data, receiver.data);
    	if(hasSerializableLambdas)
    		sender.testSignal.disconnect(receiver::receiveData);
    	else
    		sender.testSignal.disconnect(connection);
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
    	QMetaObject.Connection connection = sender.toggled.connect(receiver::toggled);
    	sender.toggled.emit(true);
    	assertEquals(true, receiver.toggled);
    	if(hasSerializableLambdas)
    		sender.toggled.disconnect(receiver::toggled);
    	else
    		sender.toggled.disconnect(connection);
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
    	QMetaObject.Connection connection = sender.toggled.connect(receiver::toggled);
    	sender.toggled.emit(true);
    	assertEquals(true, receiver.toggled);
    	if(hasSerializableLambdas)
    		sender.toggled.disconnect(receiver::toggled);
    	else
    		sender.toggled.disconnect(connection);
    	sender.toggled.emit(false);
    	assertEquals(true, receiver.toggled);
    }
    
    @Test
    public void testVirtualSlotOverrideByNonSlot() {
    	if(hasSerializableLambdas) {
	    	QMetaMethod method = QMetaMethod.fromMethod(QWizard::setVisible);
	    	Assert.assertTrue(method.isValid());
    	}
    	class Object extends QObject{
    		public final Signal1<Boolean> visibility = new Signal1<>();
    	}
    	Object object = new Object();
    	QWidget widget = new QWidget();
    	object.visibility.connect(widget::setVisible);
    	widget = new QWizard();
    	object.visibility.connect(widget::setVisible);
    	QWizard wizard = new QWizard();
    	object.visibility.connect(wizard, "setVisible(boolean)");
    	object.visibility.connect(wizard::setVisible);
    }
    
    @Test
    public void testArgumentLambdaConnection() {
    	class Object extends QObject{
    		public final Signal1<Boolean> visibility = new Signal1<>();
    	}
    	int arg = 5;
    	Object object = new Object();
    	object.visibility.connect(v->{
    		if(v)
    			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "arg="+arg);
    		else
    			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "arg="+this);
    	});
    }
    
	@SuppressWarnings("serial")
	public static class NotifyingList<T> extends ArrayList<T> implements QtSignalEmitterInterface, QInstanceMemberSignals {
		public final PrivateSignal1<T> added = new PrivateSignal1<>(this);

		public boolean add(T t) {
			if (super.add(t)) {
				QInstanceMemberSignals.emit(added, t);
				return true;
			}
			return false;
		}
	}
    
    @Test
    public void testInstanceMemberSignal() {
    	NotifyingList<QColor> colorList = new NotifyingList<>();
    	QColor[] result = {null};
    	colorList.added.connect(color->{
    		result[0] = color;
    	});
    	System.out.println(colorList.added.argumentTypes());
    	QColor darkBlue = new QColor(Qt.GlobalColor.darkBlue);
    	colorList.add(darkBlue);
    	assertEquals(darkBlue, result[0]);
    	try {
			QInstanceMemberSignals.emit(colorList.added, null);
			fail();
		} catch (QSignalAccessException e) {
		}
    }
    
    @Test
    public void testCustomSlotImpl() {
    	class Object extends QObject{
    		public final Signal0 mySignal = new Signal0();
    	}
    	int arg = 5;
    	Object object = new Object();
    	AtomicBoolean invoked = new AtomicBoolean();
    	object.mySignal.connect(new QMetaObject.Slot0() {
			private static final long serialVersionUID = 1L;

			@Override
			public void invoke() throws Throwable {
				invoked.set(true);
			}
		});
    	object.mySignal.emit();
    	assertTrue(invoked.get());
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestSignals.class.getName());
    }
}
