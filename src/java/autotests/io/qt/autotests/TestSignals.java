/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import io.qt.QNoNativeResourcesException;
import io.qt.QSignalAccessException;
import io.qt.QSignalDeclarationException;
import io.qt.QSignalInitializationException;
import io.qt.QtObjectInterface;
import io.qt.QtPrimitiveType;
import io.qt.QtSignalEmitterInterface;
import io.qt.QtUninvokable;
import io.qt.autotests.generated.General;
import io.qt.core.QCoreApplication;
import io.qt.core.QDeclarableSignals;
import io.qt.core.QEvent;
import io.qt.core.QInstanceMemberSignals;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QObject;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QStaticMemberSignals;
import io.qt.core.QThread;
import io.qt.core.Qt;
import io.qt.gui.*;
import io.qt.gui.QColor;
import io.qt.widgets.*;
import io.qt.widgets.QWidget;
import io.qt.widgets.QWizard;

@SuppressWarnings("unused")
public class TestSignals extends ApplicationInitializer{
	
	private static boolean hasSerializableLambdas;
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    	hasSerializableLambdas = General.internalAccess.serializeLambdaExpression((QMetaObject.Slot0)ApplicationInitializer::testInitializeWithWidgets) != null;
    }
	
	public static class QObjectSignalOwner extends QObject{
		final Signal1<String> testSignal = new Signal1<>();
	}
	
	public static class QObjectLightweightSignalOwner extends QObject{
		@QtUninvokable
		final Signal1<String> testSignal = new Signal1<>();
		@QtUninvokable
		final PrivateSignal1<String> testPrivateSignal = new PrivateSignal1<>();
		final Signal1<String> testInvokableSignal = new Signal1<>();
		final PrivateSignal1<String> testPrivateInvokableSignal = new PrivateSignal1<>();
		@QtUninvokable
		final Signal9<  String,
						@QtPrimitiveType Integer,
						@QtPrimitiveType Long,
						@QtPrimitiveType Byte,
						@QtPrimitiveType Double,
						@QtPrimitiveType Float,
						@QtPrimitiveType Boolean,
						@QtPrimitiveType Character,
						@QtPrimitiveType Short> signal9 = new Signal9<>();
		final Signal9<  String,
						@QtPrimitiveType Integer,
						@QtPrimitiveType Long,
						@QtPrimitiveType Byte,
						@QtPrimitiveType Double,
						@QtPrimitiveType Float,
						@QtPrimitiveType Boolean,
						@QtPrimitiveType Character,
						@QtPrimitiveType Short> invokableSignal9 = new Signal9<>();
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
    public void testQObjectLightweightSignal() {
		QObjectLightweightSignalOwner obj = new QObjectLightweightSignalOwner();
		Assert.assertFalse(obj.metaObject().method("testSignal", String.class).isValid());
		class Receiver extends QObject{
    		String value = "";
    		public void receive(String value) {
    			this.value = value;
    		}
    		
    		public void receive9(String t,
									int i,
									long l,
									byte b,
									double d,
									float f,
									boolean z,
									char c,
									short s) {
    			System.out.println("MemberFunction: "+t);
    		}
    	}
    	Receiver receiver1 = new Receiver();
    	QMetaObject.Connection connection1 = obj.testSignal.connect(receiver1::receive);
    	Assert.assertTrue(connection1.isConnected());
    	Assert.assertFalse(connection1 instanceof QtObjectInterface);
    	Assert.assertTrue(obj.testSignal.isConnected());
    	Assert.assertEquals(1, obj.testSignal.receivers());
    	Receiver receiver2 = new Receiver();
    	QMetaObject.Connection connection2 = obj.testSignal.connect(receiver2::receive);
    	Assert.assertTrue(connection2.isConnected());
    	Assert.assertFalse(connection2 instanceof QtObjectInterface);
    	Assert.assertTrue(obj.testSignal.isConnected());
    	Assert.assertEquals(2, obj.testSignal.receivers());
    	String[] array = {null};
    	obj.testSignal.connect(text->array[0] = text);
    	class Receiver3{
    		Receiver3(QObjectLightweightSignalOwner obj){
        		int i = 5;
    			obj.testSignal.connect(text->receive(text, i));
    		}
    		String value = "";
    		public void receive(String value, int i) {
    			this.value = value;
    		}
    	}
    	Receiver3 receiver3 = new Receiver3(obj);
    	obj.testSignal.connect(t->receiver2.receive(t));
    	obj.testSignal.emit("TEST1");
    	Assert.assertEquals("TEST1", receiver1.value);
    	Assert.assertEquals("TEST1", receiver2.value);
    	receiver1.dispose();
		Assert.assertFalse(connection1.isConnected());
    	obj.testSignal.emit("TEST2");
		Assert.assertEquals("TEST1", receiver1.value);
    	Assert.assertEquals("TEST2", receiver2.value);
    	obj.signal9.connect(receiver2::receive9);
    	obj.signal9.connect((t,i,l,b,d,f,z,c,s)->System.out.println("Lambda: "+t));
    	obj.signal9.emit("TEST9", 9, 9l, (byte)9, 9., 9.f, true, '9', (short)9);
    	obj.dispose();
    	try {
	    	obj.testSignal.emit("TEST3");
			Assert.fail("QNoNativeResourcesException expected to be thrown");
		}catch(QNoNativeResourcesException e) {
		}
    	if(!QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android))
    		Assert.assertEquals("TEST1", receiver1.value);
    	Assert.assertEquals("TEST2", receiver2.value);
    	Assert.assertFalse(connection2.isConnected());
    	QObjectSignalOwner obj2 = new QObjectSignalOwner();
    	obj2.dispose();
    	try {
    		obj2.testSignal.emit("TEST");
    		Assert.fail("QNoNativeResourcesException expected to be thrown");
    	}catch(QNoNativeResourcesException e) {
    	}
    	
    	obj = new QObjectLightweightSignalOwner();
    	receiver1 = new Receiver();
    	connection1 = obj.testSignal.connect(receiver1::receive, Qt.ConnectionType.QueuedConnection);
    	Assert.assertTrue(connection1.isConnected());
    	obj.testSignal.emit("TEST1");
    	Assert.assertTrue(obj.testSignal.isConnected());
    	Assert.assertEquals(1, obj.testSignal.receivers());
    	Assert.assertEquals("", receiver1.value);
    	QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
    	Assert.assertEquals("TEST1", receiver1.value);
    	QMetaObject.Connection connection3 = obj.objectNameChanged.connect(receiver1::receive);
    	Assert.assertTrue(connection3.isConnected());
    	Assert.assertTrue(connection3 instanceof QtObjectInterface);
    	QMetaObject.Connection connection3Clone = connection3.clone();
    	Assert.assertEquals(connection3, connection3Clone);
    	Assert.assertEquals(connection3.hashCode(), connection3Clone.hashCode());
    	Assert.assertTrue(obj.objectNameChanged.isConnected());
    	Assert.assertEquals(1, obj.objectNameChanged.receivers());
    	obj.disconnect();
    	Assert.assertFalse(obj.testSignal.isConnected());
    	Assert.assertEquals(0, obj.testSignal.receivers());
    	Assert.assertFalse(obj.objectNameChanged.isConnected());
    	Assert.assertEquals(0, obj.objectNameChanged.receivers());
    	Assert.assertFalse(connection3.isConnected());
    	Assert.assertEquals(connection3, connection3Clone);
    	Assert.assertEquals(connection3.hashCode(), connection3Clone.hashCode());
    	
    	{
    		class Receiver4{
    			String value = "";
        		public void receive(String value) {
        			this.value = value;
        		}
    		}
    		Receiver4 receiver = new Receiver4();
    		@SuppressWarnings("serial")
			QMetaObject.Slot1<String> slot = new QMetaObject.Slot1<String>(){
				@Override
				public void invoke(String a) throws Throwable {
				}
    		};
    		QMetaObject.Connection conn = obj.testInvokableSignal.connect(receiver::receive, Qt.ConnectionType.QueuedConnection);
    		obj.testInvokableSignal.connect(slot);
    		obj.testInvokableSignal.emit("TEST4");
    		if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android))
    			Assert.assertTrue(obj.testInvokableSignal.disconnect(conn));
    		else
    			Assert.assertTrue(obj.testInvokableSignal.disconnect(receiver::receive));
    		Assert.assertTrue(obj.testInvokableSignal.disconnect(slot));
    		receiver = new Receiver4();
    		obj.testInvokableSignal.connect(receiver::receive, Qt.ConnectionType.QueuedConnection);
    		obj.dispose();
    		receiver = null;
    		System.gc();
//    		System.out.println();
    	}
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
    	Assume.assumeTrue(General.internalAccess.useAnnotatedType());
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
    		String value;
    		public void receive(String value) {
    			this.value = value;
    		}
    	}
    	Receiver receiver = new Receiver();
    	QMetaObject.Connection connection = sender.objectNameChanged.connect(receiver::receive);
    	sender.setObjectName("TEST");
    	assertEquals("TEST", receiver.value);
    	if(hasSerializableLambdas)
    		sender.objectNameChanged.disconnect(receiver::receive);
    	else
    		sender.objectNameChanged.disconnect(connection);
    	sender.setObjectName("TEST2");
    	assertEquals("TEST", receiver.value);
    }
    
    @Test
    public void testQObjectSignal_connect_to_Object() {
    	QObject sender = new QObject();
    	class Receiver{
    		String value;
    		public void receive(String value) {
    			this.value = value;
    		}
    	}
    	Receiver receiver = new Receiver();
    	QMetaObject.Connection connection = sender.objectNameChanged.connect(receiver::receive);
    	sender.setObjectName("TEST");
    	assertEquals("TEST", receiver.value);
    	if(hasSerializableLambdas)
    		sender.objectNameChanged.disconnect(receiver::receive);
    	else
    		sender.objectNameChanged.disconnect(connection);
    	sender.setObjectName("TEST2");
    	assertEquals("TEST", receiver.value);
    }
    
    @Test
    public void testQObjectSignal_connect_to_Lambda() {
    	QObject sender = new QObject();
    	class Receiver{
    		String value;
    		public void receiveName(String value, int i) {
    			this.value = value;
    		}
    	}
    	Receiver receiver = new Receiver();
    	QMetaObject.Slot1<String> slot = name -> receiver.receiveName(name, receiver.hashCode());
    	sender.objectNameChanged.connect(slot);
    	sender.setObjectName("TEST");
    	assertEquals("TEST", receiver.value);
    	sender.objectNameChanged.disconnect(slot);
    	sender.setObjectName("TEST2");
    	assertEquals("TEST", receiver.value);
    }
    
    @Test
    public void test_custom_QObject_signal_connected_to_QObject() {
    	class Sender extends QObject{
    		public final Signal1<String> testSignal = new Signal1<>();
    	}
    	Sender sender = new Sender();
    	class Receiver extends QObject{
    		String value;
    		public void receive(String value) {
    			this.value = value;
    		}
    	}
    	Receiver receiver = new Receiver();
    	QMetaObject.Connection connection = sender.testSignal.connect(receiver::receive);
    	sender.testSignal.emit("TEST");
    	assertEquals("TEST", receiver.value);
    	if(hasSerializableLambdas)
    		sender.testSignal.disconnect(receiver::receive);
    	else
    		sender.testSignal.disconnect(connection);
    	sender.testSignal.emit("TEST2");
    	assertEquals("TEST", receiver.value);
    }
    
    @Test
    public void test_custom_QObject_signal_connected_to_Object() {
    	class Sender extends QObject{
    		public final Signal1<String> testSignal = new Signal1<>();
    	}
    	Sender sender = new Sender();
    	class Receiver{
    		String value;
    		public void receive(String value) {
    			this.value = value;
    		}
    	}
    	Receiver receiver = new Receiver();
    	QMetaObject.Connection connection = sender.testSignal.connect(receiver::receive);
    	sender.testSignal.emit("TEST");
    	assertEquals("TEST", receiver.value);
    	if(hasSerializableLambdas)
    		sender.testSignal.disconnect(receiver::receive);
    	else
    		sender.testSignal.disconnect(connection);
    	sender.testSignal.emit("TEST2");
    	assertEquals("TEST", receiver.value);
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
    
    @Test
    public void testRuntimeConnect(){
    	QMetaObject.Connection connection;
    	QDeclarableSignals.Signal2<?,?> sg;
    	if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)) {
    		@SuppressWarnings({ "rawtypes", "unchecked" })
			QDeclarableSignals.Signal2<String, ClassLoader> loadLibrary = new QDeclarableSignals.Signal2(String.class, ClassLoader.class);
    		sg = loadLibrary;
    		connection = loadLibrary.connect(Runtime.getRuntime(), "load(String, ClassLoader)");
    	}else {
    		@SuppressWarnings({ "rawtypes", "unchecked" })
    		QDeclarableSignals.Signal2<Class<?>, String> loadLibrary = new QDeclarableSignals.Signal2(Class.class, String.class);
    		sg = loadLibrary;
    		connection = loadLibrary.connect(Runtime.getRuntime(), "load0(Class, String)");
    	}
    	assertTrue(connection.isConnected());
    }
    
    public static class InstanceSignalSender implements QInstanceMemberSignals, QtSignalEmitterInterface{
    	public final Signal1<@QtPrimitiveType Integer> intValueChanged = new Signal1<>(this);
    	public final Signal1<@QtPrimitiveType Short> shortValueChanged = new Signal1<>(this);
    }
    
    public static class InstanceSignalReceiver extends QObject{
    	private double doubleValue;

		public double getDoubleValue() {
			return doubleValue;
		}

		public void setDoubleValue(double doubleValue) {
			this.doubleValue = doubleValue;
		}
		
		public void setIntValue(int intValue) {
			this.doubleValue = intValue;
		}
    }
    
    @Test
    public void testInstanceSignal() {
    	InstanceSignalSender sender = new InstanceSignalSender();
    	InstanceSignalReceiver receiver = new InstanceSignalReceiver();
    	sender.intValueChanged.connect(i->{
    		receiver.setDoubleValue(i);
    	});
    	QThread thread = QThread.create(()->{
    		sender.intValueChanged.emit(5);
    	});
    	thread.start();
    	thread.join(2000);
    	QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
    	QApplication.sendPostedEvents();
    	QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
    	QApplication.sendPostedEvents();
    	assertEquals(5.0, receiver.getDoubleValue(), 0.001);
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestSignals.class.getName());
    }
}
