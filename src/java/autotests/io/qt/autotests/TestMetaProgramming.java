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

import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;
import java.util.Set;
import java.util.concurrent.atomic.AtomicBoolean;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QUnsuccessfulInvocationException;
import io.qt.QtExtensibleEnum;
import io.qt.QtInvokable;
import io.qt.QtPropertyMember;
import io.qt.QtPropertyReader;
import io.qt.QtPropertyWriter;
import io.qt.QtShortEnumerator;
import io.qt.autotests.generated.FlagsAndEnumTest;
import io.qt.autotests.generated.FunctionManager;
import io.qt.autotests.generated.General;
import io.qt.core.QCoreApplication;
import io.qt.core.QEasingCurve;
import io.qt.core.QEvent;
import io.qt.core.QFileInfo;
import io.qt.core.QList;
import io.qt.core.QMetaEnum;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaObject.AbstractPrivateSignal1;
import io.qt.core.QMetaProperty;
import io.qt.core.QMetaType;
import io.qt.core.QMetaType.GenericFlags;
import io.qt.core.QModelIndex;
import io.qt.core.QObject;
import io.qt.core.QRunnable;
import io.qt.core.QSet;
import io.qt.core.QThread;
import io.qt.core.QUrl;
import io.qt.core.QVariant;
import io.qt.core.Qt;
import io.qt.gui.QColor;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QStandardItemModel;
import io.qt.internal.ClassAnalyzerUtility;
import io.qt.widgets.QAbstractButton;
import io.qt.widgets.QAbstractSpinBox;
import io.qt.widgets.QApplication;
import io.qt.widgets.QCheckBox;
import io.qt.widgets.QComboBox;
import io.qt.widgets.QDoubleSpinBox;
import io.qt.widgets.QPushButton;
import io.qt.widgets.QSpinBox;
import io.qt.widgets.QWidget;

public class TestMetaProgramming extends ApplicationInitializer {
	
	private static boolean hasSerializableLambdas;
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    	hasSerializableLambdas = ClassAnalyzerUtility.serializeLambdaExpression((QMetaObject.Slot0)ApplicationInitializer::testInitializeWithWidgets) != null;
    }
	
	@Test
    public void testInvokeConstructorDefaultArg() throws NoSuchMethodException {
		QMetaMethod defaultArgConstructor = QObject.staticMetaObject.constructor();
		Assert.assertTrue(defaultArgConstructor!=null);
		Object result = QObject.staticMetaObject.newInstance();
		Assert.assertTrue(result instanceof QObject);
	}
	
    @Test
    public void testUserPropertyInQt() {
        QAbstractButton b = new QCheckBox();

        QMetaProperty property = b.metaObject().userProperty();
        assertEquals("checked", property.name());
        assertEquals(true, property.isUser());
    }
    
    @Test
    public void testSignalAsMetaMethod() {
    	QPushButton button = new QPushButton();
    	assertEquals(button.pressed, QMetaMethod.fromSignal(button.pressed).toSignal(button));
    }
    
    @Test
    public void testUnknownEnums_MethodsSignalsProperties() {
//    	QAbstractItemView.staticMetaObject.enumerators().forEach(enm -> { System.out.println(( enm.isFlag() ? "flags: " : "enum:  " ) + " " + enm.name() + " " + Arrays.toString(enm.entries())); } );
    	QObject hiddenType = FlagsAndEnumTest.createHiddenObject(null);
//    	hiddenType.metaObject().enumerators().forEach(enm -> { System.out.println(enm.name() + " " + Arrays.toString(enm.entries())); } );
//    	hiddenType.metaObject().methods().forEach(mt -> { System.out.println(mt.methodType() + ": " + mt.returnClassType() + " " + mt.enclosingMetaObject().className() + "::" + mt.methodSignature() + "   " + mt.cppMethodSignature() ); } );
//    	System.out.println();
    	List<QMetaType.GenericEnumerator> hiddenClassEnums = new ArrayList<>();
    	List<QMetaType.GenericFlags> hiddenFlags = new ArrayList<>();
    	{
	    	AbstractPrivateSignal1<QMetaType.GenericEnumerator> sig = QMetaObject.findSignal(hiddenType, "hiddenEnumChanged", QMetaType.GenericEnumerator.class);
	    	Assert.assertTrue(sig!=null);
	    	sig.connect(hiddenClassEnums::add);
	    }
    	{
	    	AbstractPrivateSignal1<GenericFlags> sig = QMetaObject.findSignal(hiddenType, "hiddenFlagsChanged", QMetaType.GenericFlags.class);
	    	Assert.assertTrue(sig!=null);
	    	sig.connect(hiddenFlags::add);
    	}
    	QMetaMethod method = hiddenType.metaObject().method("setHiddenEnum", QMetaType.GenericEnumerator.class);
    	Assert.assertTrue(method!=null && method.isValid());
    	method.invoke(hiddenType, QVariant.convert(5, method.parameterType(0)));
    	method.invoke(hiddenType, QVariant.convert(9, method.parameterType(0)));
    	Assert.assertEquals(2, hiddenClassEnums.size());
    	Assert.assertEquals(5, hiddenClassEnums.get(0).value());
    	Assert.assertEquals(9, hiddenClassEnums.get(1).value());
    	
    	method = hiddenType.metaObject().method("setHiddenFlags", QMetaType.GenericFlags.class);
    	Assert.assertTrue(method!=null && method.isValid());
    	method.invoke(hiddenType, 6);
    	method.invoke(hiddenType, 2);
    	Assert.assertEquals(2, hiddenFlags.size());
    	Assert.assertEquals(6, hiddenFlags.get(0).value());
    	Assert.assertEquals(2, hiddenFlags.get(1).value());
    	
    	method = hiddenType.metaObject().method("hiddenEnum");
    	Assert.assertTrue(method!=null && method.isValid());
    	Assert.assertEquals(9, ((QMetaType.GenericEnumerator)method.invoke(hiddenType)).value());

    	method = hiddenType.metaObject().method("hiddenFlags");
    	Assert.assertTrue(method!=null && method.isValid());
    	Assert.assertEquals(2, ((QMetaType.GenericFlags)method.invoke(hiddenType)).value());
    	
    	Assert.assertTrue(hiddenType.metaObject().property("hiddenEnum")!=null);
    	Assert.assertTrue(hiddenType.metaObject().property("hiddenFlags")!=null);
    	Assert.assertEquals(9, ((QMetaType.GenericEnumerator)hiddenType.property("hiddenEnum")).value());
    	Assert.assertEquals(2, ((QMetaType.GenericFlags)hiddenType.property("hiddenFlags")).value());

    	hiddenType.setProperty("hiddenEnum", 89);
    	Assert.assertEquals(3, hiddenClassEnums.size());
    	Assert.assertEquals(5, hiddenClassEnums.get(0).value());
    	Assert.assertEquals(9, hiddenClassEnums.get(1).value());
    	Assert.assertEquals(89, hiddenClassEnums.get(2).value());
    	
    	hiddenType.setProperty("hiddenFlags", 79);
    	Assert.assertEquals(3, hiddenFlags.size());
    	Assert.assertEquals(6, hiddenFlags.get(0).value());
    	Assert.assertEquals(2, hiddenFlags.get(1).value());
    	Assert.assertEquals(79, hiddenFlags.get(2).value());
    }
	
	static class InvocationTest extends QObject{
		public final Signal1<QObject> qobjectChanged = new Signal1<>();
		public final Signal1<Object> variantChanged = new Signal1<>();
		public final Signal1<java.util.Date> javaDateChanged = new Signal1<>();
		public final Signal1<QRunnable> runnableChanged = new Signal1<>();
		public final Signal1<QWidget> widgetChanged = new Signal1<>();
		public final Signal1<QComboBox> comboBoxChanged = new Signal1<>();
		public final Signal1<QEasingCurve.EasingFunction> functionalChanged = new Signal1<>();
		public final Signal1<String> textChanged = new Signal1<>();
		public final Signal1<QColor> colorChanged = new Signal1<>();
		public final Signal1<Integer> iChanged = new Signal1<>();
		public final Signal1<Double> dChanged = new Signal1<>();
		public final Signal1<Float> fChanged = new Signal1<>();
		public final Signal1<List<QFileInfo>> listChanged = new Signal1<>();
		public final Signal1<Set<QAbstractSpinBox>> setChanged = new Signal1<>();
		
		@QtPropertyMember
		QObject qobject;
		
		@QtPropertyMember
		Object variant;
		
		@QtPropertyMember
		java.util.Date javaDate;
		
		@QtPropertyMember
		QRunnable runnable;
		
		@QtPropertyMember
		QWidget widget;
		
		@QtPropertyMember
		QComboBox comboBox;
		
		@QtPropertyMember
		QEasingCurve.EasingFunction functional;
		
		@QtPropertyMember
		String text;
		
		@QtPropertyMember
		QColor color;
		
		@QtPropertyMember
		int i;
		
		@QtPropertyMember
		double d;
		
		@QtPropertyMember
		float f;
		
		@QtPropertyMember
		QList<QFileInfo> list;
		
		@QtPropertyMember
		QSet<QAbstractSpinBox> set;
		
		private final AtomicBoolean invoked = new AtomicBoolean(false);
		private QThread invokingThread = null;
		private Object parameter;
		
		@QtInvokable
        public final void invokableSlot0() {
        	invokingThread = QThread.currentThread();
        	this.invoked.set(true);
        }
		
		@QtInvokable
        public final String invokableMethod3(double d, float f, int i) {
        	invokingThread = QThread.currentThread();
        	this.invoked.set(true);
        	return "TEST"+i;
        }
		
		@QtInvokable
        public final void test(QObject v) {
        	this.parameter = v;
        	this.invoked.set(true);
        }
		
		@QtInvokable
        public final void test(Object v) {
        	this.parameter = v;
        	this.invoked.set(true);
        }
		
		@QtInvokable
        public final void test(java.util.Date v) {
        	this.parameter = v;
        	this.invoked.set(true);
        }
		
		@QtInvokable
        public final void test(QRunnable v) {
        	this.parameter = v;
        	this.invoked.set(true);
        }
		
		@QtInvokable
        public final void test(QWidget v) {
        	this.parameter = v;
        	this.invoked.set(true);
        }
		
		@QtInvokable
        public final void test(QComboBox v) {
        	this.parameter = v;
        	this.invoked.set(true);
        }
		
		@QtInvokable
        public final void test(QEasingCurve.EasingFunction v) {
        	this.parameter = v;
        	v.applyAsDouble(1);
        	this.invoked.set(true);
        }
		
		@QtInvokable
        public final void test(String v) {
        	this.parameter = v;
        	this.invoked.set(true);
        }
		
		@QtInvokable
        public final void test(QColor v) {
        	this.parameter = v;
        	this.invoked.set(true);
        }
		
		@QtInvokable
        public final void test(int v) {
        	this.parameter = v;
        	this.invoked.set(true);
        }
		
		@QtInvokable
        public final void test(double v) {
        	this.parameter = v;
        	this.invoked.set(true);
        }
		
		@QtInvokable
        public final void test(float v) {
        	this.parameter = v;
        	this.invoked.set(true);
        }
		
		@QtInvokable
        public final void test(QList<QFileInfo> v) {
        	this.parameter = v;
        	this.invoked.set(true);
        }
		
		@QtInvokable
        public final void test(QSet<QAbstractSpinBox> v) {
        	this.parameter = v;
        	this.invoked.set(true);
        }
		
		@QtInvokable
        public final void test(FunctionManager.StringConsumer v) {
        	this.parameter = v;
        	v.accept("TEST");
        	this.invoked.set(true);
        }
	}
	
	@Test
    public void testQMetaMethod_invoke_Slot0_Queued() {
		InvocationTest b = new InvocationTest();
		QMetaMethod invokableSlot = b.metaObject().method("invokableSlot0");
    	Assert.assertTrue(invokableSlot!=null);
		invokableSlot.invoke(b, Qt.ConnectionType.QueuedConnection);
		Assert.assertFalse(b.invoked.get());
		QCoreApplication.sendPostedEvents(b);
		Assert.assertTrue(b.invoked.get());
		Assert.assertEquals(QThread.currentThread(), b.invokingThread);
	}
	
	@Test
    public void testQMetaMethod_invoke_Slot0_Direct() {
		InvocationTest b = new InvocationTest();
		QMetaMethod invokableSlot = b.metaObject().method("invokableSlot0");
    	Assert.assertTrue(invokableSlot!=null);
		invokableSlot.invoke(b, Qt.ConnectionType.DirectConnection);
		Assert.assertTrue(b.invoked.get());
		Assert.assertEquals(QThread.currentThread(), b.invokingThread);
	}
	
	@Test
    public void testQMetaMethod_invoke_Slot0_Auto() {
		InvocationTest b = new InvocationTest();
		QMetaMethod invokableSlot = b.metaObject().method("invokableSlot0");
    	Assert.assertTrue(invokableSlot!=null);
		invokableSlot.invoke(b, Qt.ConnectionType.AutoConnection);
		Assert.assertTrue(b.invoked.get());
		Assert.assertEquals(QThread.currentThread(), b.invokingThread);
	}
	
	@Test
    public void testQMetaMethod_invoke_Slot0_BlockingQueued_threaded() {
		InvocationTest b = new InvocationTest();
		QThread thread = new QThread();
		try {
			b.moveToThread(thread);
			thread.start();
			QMetaMethod invokableSlot = b.metaObject().method("invokableSlot0");
	    	Assert.assertTrue(invokableSlot!=null);
			invokableSlot.invoke(b, Qt.ConnectionType.BlockingQueuedConnection);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(thread, b.invokingThread);
		}finally {
			thread.quit();
		}
	}
	
	@Test
    public void testQMetaMethod_invoke_Slot0_Queued_threaded() throws InterruptedException {
		InvocationTest b = new InvocationTest();
		QThread thread = new QThread();
		try {
			b.moveToThread(thread);
			thread.start();
			QMetaMethod invokableSlot = b.metaObject().method("invokableSlot0");
	    	Assert.assertTrue(invokableSlot!=null);
			invokableSlot.invoke(b, Qt.ConnectionType.QueuedConnection);
			while(!b.invoked.get())
				Thread.sleep(5);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(thread, b.invokingThread);
		}finally {
			thread.quit();
		}
	}
	
	@Test
    public void testQMetaMethod_invoke_Slot0_Direct_threaded() throws InterruptedException {
		InvocationTest b = new InvocationTest();
		QThread thread = new QThread();
		try {
			b.moveToThread(thread);
			thread.start();
			QMetaMethod invokableSlot = b.metaObject().method("invokableSlot0");
	    	Assert.assertTrue(invokableSlot!=null);
			invokableSlot.invoke(b, Qt.ConnectionType.DirectConnection);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(QThread.currentThread(), b.invokingThread);
		}finally {
			thread.quit();
		}
	}
	
	@Test
    public void testQMetaMethod_invoke_Slot0_Auto_threaded() throws InterruptedException {
		InvocationTest b = new InvocationTest();
		QThread thread = new QThread();
		try {
			b.moveToThread(thread);
			thread.start();
			QMetaMethod invokableSlot = b.metaObject().method("invokableSlot0");
	    	Assert.assertTrue(invokableSlot!=null);
			invokableSlot.invoke(b, Qt.ConnectionType.AutoConnection);
			while(!b.invoked.get())
				Thread.sleep(5);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(thread, b.invokingThread);
		}finally {
			thread.quit();
		}
	}
	
	@Test
    public void testQMetaMethod_invoke_Slot0_BlockingQueued() {
		InvocationTest b = new InvocationTest();
		QMetaMethod invokableSlot = b.metaObject().method("invokableSlot0");
    	Assert.assertTrue(invokableSlot!=null);
		try {
			invokableSlot.invoke(b, Qt.ConnectionType.BlockingQueuedConnection);
			Assert.fail("Expected to throw a QUnsuccessfulInvocationException.");
		} catch (QUnsuccessfulInvocationException e) {
			Assert.assertEquals("Blocking-queued invocation on object whose thread is the current thread is not allowed.", e.getMessage());
		}
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Slot0_Queued() {
		Assume.assumeTrue(hasSerializableLambdas);
		InvocationTest b = new InvocationTest();
    	QMetaObject.invokeMethod(b::invokableSlot0, Qt.ConnectionType.QueuedConnection);
		Assert.assertFalse(b.invoked.get());
		QCoreApplication.sendPostedEvents(b);
		Assert.assertTrue(b.invoked.get());
		Assert.assertEquals(QThread.currentThread(), b.invokingThread);
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Slot0_Direct() {
		InvocationTest b = new InvocationTest();
		QMetaObject.invokeMethod(b::invokableSlot0, Qt.ConnectionType.DirectConnection);
		Assert.assertTrue(b.invoked.get());
		Assert.assertEquals(QThread.currentThread(), b.invokingThread);
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Slot0_Auto() {
		InvocationTest b = new InvocationTest();
		QMetaObject.invokeMethod(b::invokableSlot0, Qt.ConnectionType.AutoConnection);
		Assert.assertTrue(b.invoked.get());
		Assert.assertEquals(QThread.currentThread(), b.invokingThread);
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Slot0_BlockingQueued_threaded() {
		Assume.assumeTrue(hasSerializableLambdas);
		InvocationTest b = new InvocationTest();
		QThread thread = new QThread();
		try {
			b.moveToThread(thread);
			thread.start();
			QMetaObject.invokeMethod(b::invokableSlot0, Qt.ConnectionType.BlockingQueuedConnection);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(thread, b.invokingThread);
		}finally {
			thread.quit();
		}
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Slot0_Queued_threaded() throws InterruptedException {
		Assume.assumeTrue(hasSerializableLambdas);
		InvocationTest b = new InvocationTest();
		QThread thread = new QThread();
		try {
			b.moveToThread(thread);
			thread.start();
			QMetaObject.invokeMethod(b::invokableSlot0, Qt.ConnectionType.QueuedConnection);
			while(!b.invoked.get())
				Thread.sleep(5);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(thread, b.invokingThread);
		}finally {
			thread.quit();
		}
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Slot0_Direct_threaded() throws InterruptedException {
		Assume.assumeTrue(hasSerializableLambdas);
		InvocationTest b = new InvocationTest();
		QThread thread = new QThread();
		try {
			b.moveToThread(thread);
			thread.start();
			QMetaObject.invokeMethod(b::invokableSlot0, Qt.ConnectionType.DirectConnection);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(QThread.currentThread(), b.invokingThread);
		}finally {
			thread.quit();
		}
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Slot0_Auto_threaded() throws InterruptedException {
		Assume.assumeTrue(hasSerializableLambdas);
		InvocationTest b = new InvocationTest();
		QThread thread = new QThread();
		try {
			b.moveToThread(thread);
			thread.start();
			QMetaObject.invokeMethod(b::invokableSlot0, Qt.ConnectionType.AutoConnection);
			while(!b.invoked.get())
				Thread.sleep(5);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(thread, b.invokingThread);
		}finally {
			thread.quit();
		}
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Slot0_BlockingQueued() {
		Assume.assumeTrue(hasSerializableLambdas);
		InvocationTest b = new InvocationTest();
		try {
			QMetaObject.invokeMethod(b::invokableSlot0, Qt.ConnectionType.BlockingQueuedConnection);
			Assert.fail("Expected to throw a QUnsuccessfulInvocationException.");
		} catch (QUnsuccessfulInvocationException e) {
		}
	}
	
	@Test
    public void testQMetaMethod_invoke_Method3_Queued() {
		InvocationTest b = new InvocationTest();
		QMetaMethod invokableMethod = b.metaObject().method("invokableMethod3", double.class, float.class, int.class);
    	Assert.assertTrue(invokableMethod!=null);
    	try {
    		invokableMethod.invoke(b, Qt.ConnectionType.QueuedConnection, 2.0, 4f, 7);
			Assert.fail("Expected to throw a QUnsuccessfulInvocationException.");
		} catch (QUnsuccessfulInvocationException e) {
			Assert.assertEquals("Unable to invoke methods with return values in queued connections.", e.getMessage());
		}
	}
	
	@Test
    public void testQMetaMethod_invoke_Method3_Direct() {
		InvocationTest b = new InvocationTest();
		QMetaMethod invokableMethod = b.metaObject().method("invokableMethod3", double.class, float.class, int.class);
    	Assert.assertTrue(invokableMethod!=null);
    	Object result = invokableMethod.invoke(b, Qt.ConnectionType.DirectConnection, 2.0, 4f, 7);
		Assert.assertTrue(b.invoked.get());
		Assert.assertEquals(QThread.currentThread(), b.invokingThread);
		Assert.assertEquals("TEST7", result);
	}
	
	@Test
    public void testQMetaMethod_invoke_Method3_Auto() {
		InvocationTest b = new InvocationTest();
		QMetaMethod invokableMethod = b.metaObject().method("invokableMethod3", double.class, float.class, int.class);
    	Assert.assertTrue(invokableMethod!=null);
    	Object result = invokableMethod.invoke(b, Qt.ConnectionType.AutoConnection, 2.0, 4f, 7);
		Assert.assertTrue(b.invoked.get());
		Assert.assertEquals(QThread.currentThread(), b.invokingThread);
		Assert.assertEquals("TEST7", result);
	}
	
	@Test
    public void testQMetaMethod_invoke_Method3_BlockingQueued_threaded() {
		InvocationTest b = new InvocationTest();
		QThread thread = new QThread();
		try {
			b.moveToThread(thread);
			thread.start();
			QMetaMethod invokableMethod = b.metaObject().method("invokableMethod3", double.class, float.class, int.class);
	    	Assert.assertTrue(invokableMethod!=null);
	    	Object result = invokableMethod.invoke(b, Qt.ConnectionType.BlockingQueuedConnection, 2.0, 4f, 7);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(thread, b.invokingThread);
			Assert.assertEquals("TEST7", result);
		}finally {
			thread.quit();
		}
	}
	
	@Test
    public void testQMetaMethod_invoke_Method3_Queued_threaded() throws InterruptedException {
		InvocationTest b = new InvocationTest();
		QThread thread = new QThread();
		try {
			b.moveToThread(thread);
			thread.start();
			QMetaMethod invokableMethod = b.metaObject().method("invokableMethod3", double.class, float.class, int.class);
	    	Assert.assertTrue(invokableMethod!=null);
	    	invokableMethod.invoke(b, Qt.ConnectionType.QueuedConnection, 2.0, 4f, 7);
			Assert.fail("Expected to throw a QUnsuccessfulInvocationException.");
		} catch (QUnsuccessfulInvocationException e) {
			Assert.assertEquals("Unable to invoke methods with return values in queued connections.", e.getMessage());
		}finally {
			thread.quit();
		}
	}
	
	@Test
    public void testQMetaMethod_invoke_Method3_Direct_threaded() throws InterruptedException {
		InvocationTest b = new InvocationTest();
		QThread thread = new QThread();
		try {
			b.moveToThread(thread);
			thread.start();
			QMetaMethod invokableMethod = b.metaObject().method("invokableMethod3", double.class, float.class, int.class);
	    	Assert.assertTrue(invokableMethod!=null);
	    	Object result = invokableMethod.invoke(b, Qt.ConnectionType.DirectConnection, 2.0, 4f, 7);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(QThread.currentThread(), b.invokingThread);
			Assert.assertEquals("TEST7", result);
		}finally {
			thread.quit();
		}
	}
	
	@Test
    public void testQMetaMethod_invoke_Method3_Auto_threaded() throws InterruptedException {
		InvocationTest b = new InvocationTest();
		QThread thread = new QThread();
		try {
			b.moveToThread(thread);
			thread.start();
			QMetaMethod invokableMethod = b.metaObject().method("invokableMethod3", double.class, float.class, int.class);
	    	Assert.assertTrue(invokableMethod!=null);
	    	invokableMethod.invoke(b, Qt.ConnectionType.AutoConnection, 2.0, 4f, 7);
			Assert.fail("Expected to throw a QUnsuccessfulInvocationException.");
		} catch (QUnsuccessfulInvocationException e) {
			Assert.assertEquals("Unable to invoke methods with return values in queued connections (auto connection with different threads).", e.getMessage());
		}finally {
			thread.quit();
		}
	}
	
	@Test
    public void testQMetaMethod_invoke_Method3_BlockingQueued() {
		InvocationTest b = new InvocationTest();
		QMetaMethod invokableMethod = b.metaObject().method("invokableMethod3", double.class, float.class, int.class);
    	Assert.assertTrue(invokableMethod!=null);
		try {
			invokableMethod.invoke(b, Qt.ConnectionType.BlockingQueuedConnection, 2.0, 4f, 7);
			Assert.fail("Expected to throw a QUnsuccessfulInvocationException.");
		} catch (QUnsuccessfulInvocationException e) {
			Assert.assertEquals("Blocking-queued invocation on object whose thread is the current thread is not allowed.", e.getMessage());
		}
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Method3_Queued() {
		Assume.assumeTrue(hasSerializableLambdas);
		InvocationTest b = new InvocationTest();
		try {
	    	QMetaObject.invokeMethod(b::invokableMethod3, Qt.ConnectionType.QueuedConnection, 2.0, 4f, 7);
			Assert.fail("Expected to throw a QUnsuccessfulInvocationException.");
		} catch (QUnsuccessfulInvocationException e) {
			Assert.assertEquals("Unable to invoke methods with return values in queued connections.", e.getMessage());
		}
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Method3_Direct() {
		Assume.assumeTrue(hasSerializableLambdas);
		InvocationTest b = new InvocationTest();
		String result = QMetaObject.invokeMethod(b::invokableMethod3, Qt.ConnectionType.DirectConnection, 2.0, 4f, 7);
		Assert.assertTrue(b.invoked.get());
		Assert.assertEquals(QThread.currentThread(), b.invokingThread);
		Assert.assertEquals("TEST7", result);
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Method3_Auto() {
		Assume.assumeTrue(hasSerializableLambdas);
		InvocationTest b = new InvocationTest();
		String result = QMetaObject.invokeMethod(b::invokableMethod3, Qt.ConnectionType.AutoConnection, 2.0, 4f, 7);
		Assert.assertTrue(b.invoked.get());
		Assert.assertEquals(QThread.currentThread(), b.invokingThread);
		Assert.assertEquals("TEST7", result);
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Method3_BlockingQueued_threaded() {
		Assume.assumeTrue(hasSerializableLambdas);
		InvocationTest b = new InvocationTest();
		QThread thread = new QThread();
		try {
			b.moveToThread(thread);
			thread.start();
			String result = QMetaObject.invokeMethod(b::invokableMethod3, Qt.ConnectionType.BlockingQueuedConnection, 2.0, 4f, 7);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(thread, b.invokingThread);
			Assert.assertEquals("TEST7", result);
		}finally {
			thread.quit();
		}
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Method3_Queued_threaded() throws InterruptedException {
		Assume.assumeTrue(hasSerializableLambdas);
		InvocationTest b = new InvocationTest();
		QThread thread = new QThread();
		try {
			b.moveToThread(thread);
			thread.start();
			QMetaObject.invokeMethod(b::invokableMethod3, Qt.ConnectionType.QueuedConnection, 2.0, 4f, 7);
			Assert.fail("Expected to throw a QUnsuccessfulInvocationException.");
		} catch (QUnsuccessfulInvocationException e) {
			Assert.assertEquals("Unable to invoke methods with return values in queued connections.", e.getMessage());
		}finally {
			thread.quit();
		}
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Method3_Direct_threaded() throws InterruptedException {
		Assume.assumeTrue(hasSerializableLambdas);
		InvocationTest b = new InvocationTest();
		QThread thread = new QThread();
		try {
			b.moveToThread(thread);
			thread.start();
			String result = QMetaObject.invokeMethod(b::invokableMethod3, Qt.ConnectionType.DirectConnection, 2.0, 4f, 7);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(QThread.currentThread(), b.invokingThread);
			Assert.assertEquals("TEST7", result);
		}finally {
			thread.quit();
		}
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Method3_Auto_threaded() throws InterruptedException {
		Assume.assumeTrue(hasSerializableLambdas);
		InvocationTest b = new InvocationTest();
		QThread thread = new QThread();
		try {
			b.moveToThread(thread);
			thread.start();
			QMetaObject.invokeMethod(b::invokableMethod3, Qt.ConnectionType.AutoConnection, 2.0, 4f, 7);
			Assert.fail("Expected to throw a QUnsuccessfulInvocationException.");
		} catch (QUnsuccessfulInvocationException e) {
			Assert.assertEquals("Unable to invoke methods with return values in queued connections (auto connection with different threads).", e.getMessage());
		}finally {
			thread.quit();
		}
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Method3_BlockingQueued() {
		Assume.assumeTrue(hasSerializableLambdas);
		InvocationTest b = new InvocationTest();
		try {
			QMetaObject.invokeMethod(b::invokableMethod3, Qt.ConnectionType.BlockingQueuedConnection, 2.0, 4f, 7);
			Assert.fail("Expected to throw a QUnsuccessfulInvocationException.");
		} catch (QUnsuccessfulInvocationException e) {
			Assert.assertEquals("Blocking-queued invocation on object whose thread is the current thread is not allowed.", e.getMessage());
		}
	}
	
	@Test
    public void testQMetaObject_invokeMethod_ReturningLambda_Direct() {
		String result = QMetaObject.invokeMethod((d,f,i)->{ return "TEST"+i; }, Qt.ConnectionType.DirectConnection, 2.0, 4f, 7);
		Assert.assertEquals("TEST7", result);
	}
	
	@Test
    public void testQMetaObject_invokeMethod_ReturningLambda_Auto() {
		String result = QMetaObject.invokeMethod((d,f,i)->{ return "TEST"+i; }, Qt.ConnectionType.AutoConnection, 2.0, 4f, 7);
		Assert.assertEquals("TEST7", result);
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Lambda_Queued() {
		boolean[] invoked = {false};
		QMetaObject.invokeMethod((d,f,i)->{invoked[0] = true;}, Qt.ConnectionType.QueuedConnection, 2.0, 4f, 7);
		QApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		Assert.assertTrue(invoked[0]);
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Lambda_BlockingQueued() {
		try {
			QMetaObject.invokeMethod((d,f,i)->{}, Qt.ConnectionType.BlockingQueuedConnection, 2.0, 4f, 7);
			Assert.fail("Expected to throw a QUnsuccessfulInvocationException.");
		} catch (QUnsuccessfulInvocationException e) {
			Assert.assertEquals("Blocking-queued invocation of method on not allowed without thread affinity.", e.getMessage());
		}
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Lambda_Direct() {
		QMetaObject.invokeMethod((d,f,i)->{}, Qt.ConnectionType.DirectConnection, 2.0, 4f, 7);
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Lambda_Auto() {
		QMetaObject.invokeMethod((d,f,i)->{}, Qt.ConnectionType.AutoConnection, 2.0, 4f, 7);
	}
	
	@Test
    public void testQMetaObject_invokeMethod_ReturningLambda_Queued() {
		try {
			QMetaObject.invokeMethod((d,f,i)->{ return "TEST"+i; }, Qt.ConnectionType.QueuedConnection, 2.0, 4f, 7);
			Assert.fail("Expected to throw a QUnsuccessfulInvocationException.");
		} catch (QUnsuccessfulInvocationException e) {
			Assert.assertEquals("Unable to invoke methods with return values in queued connections.", e.getMessage());
		}
	}
	
	@Test
    public void testQMetaObject_invokeMethod_ReturningLambda_BlockingQueued() {
		try {
			QMetaObject.invokeMethod((d,f,i)->{ return "TEST"+i; }, Qt.ConnectionType.BlockingQueuedConnection, 2.0, 4f, 7);
			Assert.fail("Expected to throw a QUnsuccessfulInvocationException.");
		} catch (QUnsuccessfulInvocationException e) {
			Assert.assertEquals("Blocking-queued invocation of method on not allowed without thread affinity.", e.getMessage());
		}
	}
    
	private static class ConvertibleProperties extends QObject {
		
		@QtExtensibleEnum
		public enum ConvertibleEnum implements QtShortEnumerator{
			None((short)0),
			E1((short)1),
			E2((short)2),
			E3((short)3),
			E4((short)4),
			E5((short)5);
			
			private final short value;

			private ConvertibleEnum(short value) {
				this.value = value;
			}

			@Override
			public short value() {
				return value;
			}
		}
		
		private final QStandardItemModel model;
		private QModelIndex index;
		private QMetaMethod mtd;
		private List<Integer> intList = Arrays.asList(1,5,8,2,4);
		private QUrl.FormattingOptions formattingOptions = new QUrl.FormattingOptions(QUrl.UrlFormattingOption.RemoveFilename, QUrl.ComponentFormattingOption.EncodeUnicode);
		private ConvertibleEnum convertibleEnum = ConvertibleEnum.None;
		private QMetaObject.Connection connection;

		public ConvertibleProperties() {
			super();
			model = new QStandardItemModel(this);
			model.setRowCount(300);
			model.setColumnCount(300);
			connection = model.dataChanged.connect(d->{});
		}
		
        @QtPropertyReader
        public final QModelIndex currentIndex() { return model.index(2, 2); }
        
        @QtInvokable
        public final void setIndex(QModelIndex index) {
        	this.index = index;
        }

        @QtPropertyReader
        public final QMetaMethod metaMethod() {
        	return QObject.staticMetaObject.methods().get(0); 
    	}
        
        @QtInvokable
        public final void setMtd(QMetaMethod mtd) {
        	this.mtd = mtd;
        }

        @QtPropertyReader
		public List<Integer> intList() {
			return intList;
		}

        @QtPropertyWriter
		public void setIntList(List<Integer> intList) {
			this.intList = intList;
		}

        @QtPropertyReader
		public QUrl.FormattingOptions formattingOptions() {
			return formattingOptions;
		}

        @QtPropertyWriter
		public void setFormattingOptions(QUrl.FormattingOptions formattingOptions) {
			this.formattingOptions = formattingOptions;
		}

        @QtPropertyReader
		public ConvertibleEnum convertibleEnum() {
			return convertibleEnum;
		}

        @QtPropertyWriter
		public void setConvertibleEnum(ConvertibleEnum convertibleEnum) {
			this.convertibleEnum = convertibleEnum;
		}

        @QtPropertyReader
		public QMetaObject.Connection connection() {
			return connection;
		}

        @QtPropertyWriter
		public void setConnection(QMetaObject.Connection connection) {
			this.connection = connection;
		}
	}
	
    @Test
    public void testConvertibleProperties() {
    	ConvertibleProperties b = new ConvertibleProperties();
    	QMetaMethod metaMethod;
    	QMetaProperty metaProperty;
    	Object result;
    	{
        	metaMethod = b.metaObject().method("metaMethod");
        	Assert.assertTrue(metaMethod!=null);
        	result = metaMethod.invoke(b);
        	Assert.assertTrue(result instanceof QMetaMethod);
    	}
    	{
    		metaProperty = b.metaObject().property("metaMethod");
        	Assert.assertTrue(metaProperty!=null);
        	result = metaProperty.read(b);
        	Assert.assertTrue(result instanceof QMetaMethod);
    	}
    	{
        	metaMethod = b.metaObject().method("currentIndex");
        	Assert.assertTrue(metaMethod!=null);
        	result = metaMethod.invoke(b);
        	Assert.assertTrue(result instanceof QModelIndex);
    		QModelIndex idx = (QModelIndex)result;
    		Assert.assertEquals(2, idx.row());
    		Assert.assertEquals(2, idx.column());
    	}
    	{
    		metaProperty = b.metaObject().property("currentIndex");
        	Assert.assertTrue(metaProperty!=null);
        	result = metaProperty.read(b);
        	Assert.assertTrue(result instanceof QModelIndex);
    		QModelIndex idx = (QModelIndex)result;
    		Assert.assertEquals(2, idx.row());
    		Assert.assertEquals(2, idx.column());
    	}
    	{
        	metaMethod = b.metaObject().method("formattingOptions");
        	Assert.assertTrue(metaMethod!=null);
        	result = metaMethod.invoke(b);
        	Assert.assertTrue(result instanceof QUrl.FormattingOptions);
        	Assert.assertEquals(new QUrl.FormattingOptions(QUrl.UrlFormattingOption.RemoveFilename, QUrl.ComponentFormattingOption.EncodeUnicode), result);
    	}
    	{
        	metaMethod = b.metaObject().method("setFormattingOptions", QUrl.FormattingOptions.class);
        	Assert.assertTrue(metaMethod!=null);
        	metaMethod.invoke(b, new QUrl.FormattingOptions(QUrl.UrlFormattingOption.PreferLocalFile));
        	Assert.assertEquals(new QUrl.FormattingOptions(QUrl.UrlFormattingOption.PreferLocalFile), b.formattingOptions);
    	}
    	{
    		metaProperty = b.metaObject().property("formattingOptions");
        	Assert.assertTrue(metaProperty!=null);
        	result = metaProperty.read(b);
        	Assert.assertTrue(result instanceof QUrl.FormattingOptions);
        	Assert.assertEquals(new QUrl.FormattingOptions(QUrl.UrlFormattingOption.PreferLocalFile), result);
        	metaProperty.write(b, new QUrl.FormattingOptions(QUrl.UrlFormattingOption.RemoveFilename, QUrl.ComponentFormattingOption.EncodeUnicode));
        	Assert.assertEquals(new QUrl.FormattingOptions(QUrl.UrlFormattingOption.RemoveFilename, QUrl.ComponentFormattingOption.EncodeUnicode), b.formattingOptions);
    	}
    	{
        	metaMethod = b.metaObject().method("intList");
        	Assert.assertTrue(metaMethod!=null);
        	result = metaMethod.invoke(b);
        	Assert.assertTrue(result instanceof List);
        	Assert.assertEquals(result, Arrays.asList(1,5,8,2,4));
    	}
    	{
    		metaProperty = b.metaObject().property("intList");
        	Assert.assertTrue(metaProperty!=null);
        	result = metaProperty.read(b);
        	Assert.assertTrue(result instanceof List);
        	Assert.assertEquals(result, Arrays.asList(1,5,8,2,4));
    	}
    	{
        	metaMethod = b.metaObject().method("setIntList", List.class);
        	Assert.assertTrue(metaMethod!=null);
        	metaMethod.invoke(b, new ArrayList<>());
        	Assert.assertEquals(QList.class, General.internalAccess.getClass(b.intList));
    	}
    	{
    		metaProperty = b.metaObject().property("intList");
        	Assert.assertTrue(metaProperty!=null);
        	metaProperty.write(b, new LinkedList<>());
    		Assert.assertTrue(List.class.isAssignableFrom(General.internalAccess.getClass(b.intList)));
    	}
    	{
        	metaMethod = b.metaObject().method("setIndex", QModelIndex.class);
        	Assert.assertTrue(metaMethod!=null);
        	metaMethod.invoke(b, b.model.index(234, 97));
    		Assert.assertEquals(234, b.index.row());
    		Assert.assertEquals(97, b.index.column());
    	}
    	{
        	metaMethod = b.metaObject().method("setMtd", QMetaMethod.class);
        	Assert.assertTrue(metaMethod!=null);
        	QMetaMethod mtd = QGuiApplication.staticMetaObject.methods().get(5);
        	metaMethod.invoke(b, mtd);
    		Assert.assertEquals(mtd, b.mtd);
    	}
    	{
    		List<QMetaEnum> enumerators = b.metaObject().enumerators();
    		Assert.assertEquals(1, enumerators.size());
    		Assert.assertEquals("ConvertibleEnum", enumerators.get(0).name());
    		Assert.assertEquals(ConvertibleProperties.ConvertibleEnum.E5, enumerators.get(0).resolve(ConvertibleProperties.ConvertibleEnum.E5.value()));
    		QtShortEnumerator entry = (QtShortEnumerator)enumerators.get(0).resolve(12345);
    		Assert.assertEquals(12345, entry.value());
    	}
    	{
        	metaMethod = b.metaObject().method("setConvertibleEnum", ConvertibleProperties.ConvertibleEnum.class);
        	Assert.assertTrue(metaMethod!=null);
        	metaMethod.invoke(b, ConvertibleProperties.ConvertibleEnum.E4);
        	Assert.assertEquals(ConvertibleProperties.ConvertibleEnum.E4, b.convertibleEnum);
    	}
    	{
        	metaMethod = b.metaObject().method("convertibleEnum");
        	Assert.assertTrue(metaMethod!=null);
        	result = metaMethod.invoke(b);
        	Assert.assertTrue(result instanceof ConvertibleProperties.ConvertibleEnum);
        	Assert.assertEquals(result, ConvertibleProperties.ConvertibleEnum.E4);
    	}
    	{
        	metaProperty = b.metaObject().property("convertibleEnum");
        	Assert.assertTrue(metaProperty!=null);
        	result = metaProperty.read(b);
        	Assert.assertTrue(result instanceof ConvertibleProperties.ConvertibleEnum);
        	Assert.assertEquals(result, ConvertibleProperties.ConvertibleEnum.E4);
    	}
    	{
        	metaMethod = b.metaObject().method("connection");
        	Assert.assertTrue(metaMethod!=null);
        	result = metaMethod.invoke(b);
        	Assert.assertTrue(result instanceof QMetaObject.Connection);
        	QMetaObject.Connection con = (QMetaObject.Connection)result;
        	Assert.assertTrue(b.connection().isConnected());
        	Assert.assertTrue(con.isConnected());
        	Assert.assertEquals(b.model, con.sender());
    	}
    	{
        	metaProperty = b.metaObject().property("connection");
        	Assert.assertTrue(metaProperty!=null);
        	result = metaProperty.read(b);
        	Assert.assertTrue(result instanceof QMetaObject.Connection);
        	QMetaObject.Connection con = (QMetaObject.Connection)result;
        	Assert.assertEquals(b.model, con.sender());
        	con = b.model.itemChanged.connect(i->{});
        	metaProperty.write(b, con);
        	Assert.assertEquals(b.connection, con);
    	}
    }
    
    @Test
    public void testQMetaMethod_invoke_IllegalArguments() {
		InvocationTest b = new InvocationTest();
		b.metaObject().methods().forEach(m->{if("test".equals(m.name().toString()) || m.methodType()==QMetaMethod.MethodType.Signal)System.out.println(m.methodSignature()+" - "+m.cppMethodSignature());});
		b.metaObject().properties().forEach(p->System.out.println(p.typeName()+" "+p.name()+" ==> "+p.notifySignal().cppMethodSignature()));
		QMetaMethod invokableMethod;
		
		{
			invokableMethod = b.metaObject().method("test", double.class);
	    	Assert.assertTrue(invokableMethod!=null);
			b.invoked.set(false);
			b.parameter = null;
			invokableMethod.invoke(b, 2.5);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(Double.valueOf(2.5), b.parameter);
			b.invoked.set(false);
			b.parameter = null;
			invokableMethod.invoke(b, 2);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(Double.valueOf(2), b.parameter);
			b.invoked.set(false);
			b.parameter = null;
			try {
				invokableMethod.invoke(b, "2.0");
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.String, expected: double", e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, new Object());
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Object, expected: double", e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
		}
		
		{
			invokableMethod = b.metaObject().method("test", float.class);
	    	Assert.assertTrue(invokableMethod!=null);
			b.invoked.set(false);
			b.parameter = null;
			invokableMethod.invoke(b, 2.5f);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(Float.valueOf(2.5f), b.parameter);
			b.invoked.set(false);
			b.parameter = null;
			invokableMethod.invoke(b, 2);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(Float.valueOf(2), b.parameter);
			b.invoked.set(false);
			b.parameter = null;
			try {
				invokableMethod.invoke(b, "2");
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.String, expected: float", e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, new Object());
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Object, expected: float", e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
		}
		
		{
			invokableMethod = b.metaObject().method("test", int.class);
	    	Assert.assertTrue(invokableMethod!=null);
			b.invoked.set(false);
			b.parameter = null;
			invokableMethod.invoke(b, 2);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(Integer.valueOf(2), b.parameter);
			b.invoked.set(false);
			b.parameter = null;
			try {
				invokableMethod.invoke(b, 2.0);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Double, expected: int", e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, "");
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.String, expected: int", e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, new Object());
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Object, expected: int", e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			Object value = new QColor(Qt.GlobalColor.darkBlue);
			try {
				invokableMethod.invoke(b, value);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: "+value.getClass().getName()+", expected: int", e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
		}
		
		{
			invokableMethod = b.metaObject().method("test", Object.class);
	    	Assert.assertTrue(invokableMethod!=null);
			b.invoked.set(false);
			b.parameter = null;
			invokableMethod.invoke(b, 2.5);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(2.5, b.parameter);
			b.invoked.set(false);
			b.parameter = null;
			invokableMethod.invoke(b, 2);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(2, b.parameter);
			b.invoked.set(false);
			b.parameter = null;
			invokableMethod.invoke(b, "2.0");
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals("2.0", b.parameter);
			b.invoked.set(false);
			b.parameter = null;
			Object value = new Object();
			invokableMethod.invoke(b, value);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(value, b.parameter);
			b.invoked.set(false);
			b.parameter = null;
		}
		
		{
			Class<?> parameter = QObject.class;
			invokableMethod = b.metaObject().method("test", parameter);
	    	Assert.assertTrue(invokableMethod!=null);
			b.invoked.set(false);
			b.parameter = null;
			Object value = new QWidget();
			invokableMethod.invoke(b, value);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(value, b.parameter);
			b.invoked.set(false);
			b.parameter = null;
			value = QCoreApplication.instance();
			invokableMethod.invoke(b, value);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(value, b.parameter);
			b.invoked.set(false);
			b.parameter = null;
			try {
				invokableMethod.invoke(b, 2.5);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Double, expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, 2);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Integer, expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, "2.0");
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.String, expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				value = new QColor(Qt.GlobalColor.darkBlue);
				invokableMethod.invoke(b, value);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: "+value.getClass().getName()+", expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, new Object());
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Object, expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, (QRunnable)()->{});
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: io.qt.core.QRunnable, expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, (QEasingCurve.EasingFunction)d->d);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: io.qt.core.QEasingCurve.EasingFunction, expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
		}
		
		{
			Class<?> parameter = QWidget.class;
			invokableMethod = b.metaObject().method("test", parameter);
	    	Assert.assertTrue(invokableMethod!=null);
			b.invoked.set(false);
			b.parameter = null;
			Object value = new QWidget();
			invokableMethod.invoke(b, value);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(value, b.parameter);
			b.invoked.set(false);
			b.parameter = null;
			try {
				value = QCoreApplication.instance();
				invokableMethod.invoke(b, value);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: "+value.getClass().getName()+", expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, 2.5);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Double, expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, 2);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Integer, expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, "2.0");
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.String, expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, new Object());
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Object, expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				value = new QColor(Qt.GlobalColor.darkBlue);
				invokableMethod.invoke(b, value);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: "+value.getClass().getName()+", expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, (QRunnable)()->{});
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: io.qt.core.QRunnable, expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, (QEasingCurve.EasingFunction)d->d);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: io.qt.core.QEasingCurve.EasingFunction, expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
		}
		
		{
			Class<?> parameter = QComboBox.class;
			invokableMethod = b.metaObject().method("test", parameter);
	    	Assert.assertTrue(invokableMethod!=null);
			b.invoked.set(false);
			b.parameter = null;
			Object value = new QComboBox();
			invokableMethod.invoke(b, value);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(value, b.parameter);
			b.invoked.set(false);
			b.parameter = null;
			try {
				value = new QWidget();
				invokableMethod.invoke(b, value);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: "+value.getClass().getName()+", expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				value = QCoreApplication.instance();
				invokableMethod.invoke(b, value);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: "+value.getClass().getName()+", expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, 2.5);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Double, expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, 2);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Integer, expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, "2.0");
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.String, expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, new Object());
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Object, expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				value = new QColor(Qt.GlobalColor.darkBlue);
				invokableMethod.invoke(b, value);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: "+value.getClass().getName()+", expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, (QRunnable)()->{});
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: io.qt.core.QRunnable, expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, (QEasingCurve.EasingFunction)d->d);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: io.qt.core.QEasingCurve.EasingFunction, expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
		}
		
		{
			Class<?> parameter = QRunnable.class;
			invokableMethod = b.metaObject().method("test", parameter);
	    	Assert.assertTrue(invokableMethod!=null);
			b.invoked.set(false);
			b.parameter = null;
			Object value = (QRunnable)()->{};
			invokableMethod.invoke(b, value);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(value, b.parameter);
			b.invoked.set(false);
			b.parameter = null;
			value = new QRunnable() {
				@Override
				public void run() {
				}
			};
			invokableMethod.invoke(b, value);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(value, b.parameter);
			b.invoked.set(false);
			b.parameter = null;
			try {
				value = new QWidget();
				invokableMethod.invoke(b, value);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: "+value.getClass().getName()+", expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				value = QCoreApplication.instance();
				invokableMethod.invoke(b, value);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: "+value.getClass().getName()+", expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, 2.5);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Double, expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, 2);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Integer, expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				value = new QColor(Qt.GlobalColor.darkBlue);
				invokableMethod.invoke(b, value);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: "+value.getClass().getName()+", expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, "2.0");
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.String, expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, new Object());
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Object, expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, (QEasingCurve.EasingFunction)d->d);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: io.qt.core.QEasingCurve.EasingFunction, expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
		}
		
		{
			Class<?> parameter;
			Object value;
			parameter = FunctionManager.StringConsumer.class;
			invokableMethod = b.metaObject().method("test", parameter);
	    	Assert.assertTrue(invokableMethod!=null);
			b.invoked.set(false);
			b.parameter = null;
			value = (FunctionManager.StringConsumer)s->{System.out.println(s);};
			invokableMethod.invoke(b, value);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(value, b.parameter);
			b.invoked.set(false);
			b.parameter = null;
			parameter = QEasingCurve.EasingFunction.class;
			invokableMethod = b.metaObject().method("test", parameter);
	    	Assert.assertTrue(invokableMethod!=null);
			b.invoked.set(false);
			b.parameter = null;
			value = (QEasingCurve.EasingFunction)d->d;
			invokableMethod.invoke(b, value);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(value, b.parameter);
			b.invoked.set(false);
			b.parameter = null;
			value = new QEasingCurve.EasingFunction() {
				@Override
				public double applyAsDouble(double progress) {
					return 0;
				}
			};
			invokableMethod.invoke(b, value);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(value, b.parameter);
			b.invoked.set(false);
			b.parameter = null;
			try {
				value = new QWidget();
				invokableMethod.invoke(b, value);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: "+value.getClass().getName()+", expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				value = QCoreApplication.instance();
				invokableMethod.invoke(b, value);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: "+value.getClass().getName()+", expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				value = new QColor(Qt.GlobalColor.darkBlue);
				invokableMethod.invoke(b, value);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: "+value.getClass().getName()+", expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, 2.5);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Double, expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, 2);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Integer, expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				value = new QColor(Qt.GlobalColor.darkBlue);
				invokableMethod.invoke(b, value);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: "+value.getClass().getName()+", expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, "2.0");
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.String, expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, new Object());
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Object, expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, (QRunnable)()->{});
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: io.qt.core.QRunnable, expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
		}
		
		{
			Class<?> parameter = String.class;
			invokableMethod = b.metaObject().method("test", parameter);
	    	Assert.assertTrue(invokableMethod!=null);
			b.invoked.set(false);
			b.parameter = null;
			Object value = "test";
			invokableMethod.invoke(b, value);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(value, b.parameter);
			b.invoked.set(false);
			b.parameter = null;
			try {
				value = new QWidget();
				invokableMethod.invoke(b, value);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: "+value.getClass().getName()+", expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				value = QCoreApplication.instance();
				invokableMethod.invoke(b, value);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: "+value.getClass().getName()+", expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				value = new QColor(Qt.GlobalColor.darkBlue);
				invokableMethod.invoke(b, value);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: "+value.getClass().getName()+", expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, 2.5);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Double, expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, 2);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Integer, expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, new Object());
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Object, expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, (QRunnable)()->{});
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: io.qt.core.QRunnable, expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
		}
		
		{
			Class<?> parameter = QColor.class;
			invokableMethod = b.metaObject().method("test", parameter);
	    	Assert.assertTrue(invokableMethod!=null);
			b.invoked.set(false);
			b.parameter = null;
			Object value = new QColor(Qt.GlobalColor.darkBlue);
			invokableMethod.invoke(b, value);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(value, b.parameter);
			b.invoked.set(false);
			b.parameter = null;
			try {
				value = new QWidget();
				invokableMethod.invoke(b, value);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: "+value.getClass().getName()+", expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				value = QCoreApplication.instance();
				invokableMethod.invoke(b, value);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: "+value.getClass().getName()+", expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, 2.5);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Double, expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, 2);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Integer, expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, "2.0");
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.String, expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, new Object());
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Object, expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, (QRunnable)()->{});
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: io.qt.core.QRunnable, expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, (QEasingCurve.EasingFunction)d->d);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: io.qt.core.QEasingCurve.EasingFunction, expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
		}
		
		{
			Class<?> parameter = QList.class;
			invokableMethod = b.metaObject().method("test", parameter);
	    	Assert.assertTrue(invokableMethod!=null);
			b.invoked.set(false);
			b.parameter = null;
			Object value = QList.of(new QFileInfo());
			invokableMethod.invoke(b, value);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(value, b.parameter);
			b.invoked.set(false);
			b.parameter = null;
			try {
				value = QList.of(Qt.GlobalColor.darkGreen);
				invokableMethod.invoke(b, value);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong collection content given: io.qt.core.Qt.GlobalColor, expected: io.qt.core.QFileInfo", e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				value = new QWidget();
				invokableMethod.invoke(b, value);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: "+value.getClass().getName()+", expected: java.util.Collection", e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				value = QCoreApplication.instance();
				invokableMethod.invoke(b, value);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: "+value.getClass().getName()+", expected: java.util.Collection", e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, 2.5);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Double, expected: java.util.Collection", e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, 2);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Integer, expected: java.util.Collection", e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, "2.0");
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.String, expected: java.util.Collection", e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, new Object());
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Object, expected: java.util.Collection", e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, (QRunnable)()->{});
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: io.qt.core.QRunnable, expected: java.util.Collection", e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, (QEasingCurve.EasingFunction)d->d);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: io.qt.core.QEasingCurve.EasingFunction, expected: java.util.Collection", e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
		}
		{
			Class<?> parameter = QSet.class;
			invokableMethod = b.metaObject().method("test", parameter);
	    	Assert.assertTrue(invokableMethod!=null);
			b.invoked.set(false);
			b.parameter = null;
			Object value = QSet.of(new QSpinBox(), new QDoubleSpinBox());
			invokableMethod.invoke(b, value);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(value, b.parameter);
			b.invoked.set(false);
			b.parameter = null;
			try {
				value = QSet.of(new QWidget());
				invokableMethod.invoke(b, value);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong collection content given: io.qt.widgets.QWidget, expected: io.qt.widgets.QAbstractSpinBox", e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				value = new QWidget();
				invokableMethod.invoke(b, value);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: "+value.getClass().getName()+", expected: java.util.Collection", e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				value = QCoreApplication.instance();
				invokableMethod.invoke(b, value);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: "+value.getClass().getName()+", expected: java.util.Collection", e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, 2.5);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Double, expected: java.util.Collection", e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, 2);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Integer, expected: java.util.Collection", e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, "2.0");
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.String, expected: java.util.Collection", e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, new Object());
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Object, expected: java.util.Collection", e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, (QRunnable)()->{});
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: io.qt.core.QRunnable, expected: java.util.Collection", e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, (QEasingCurve.EasingFunction)d->d);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: io.qt.core.QEasingCurve.EasingFunction, expected: java.util.Collection", e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
		}
		
		{
			Class<?> parameter = java.util.Date.class;
			invokableMethod = b.metaObject().method("test", parameter);
	    	Assert.assertTrue(invokableMethod!=null);
			b.invoked.set(false);
			b.parameter = null;
			Object value = new java.util.Date(5);
			invokableMethod.invoke(b, value);
			Assert.assertTrue(b.invoked.get());
			Assert.assertEquals(value, b.parameter);
			b.invoked.set(false);
			b.parameter = null;
			try {
				value = new QWidget();
				invokableMethod.invoke(b, value);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: "+value.getClass().getName()+", expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				value = QCoreApplication.instance();
				invokableMethod.invoke(b, value);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: "+value.getClass().getName()+", expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, 2.5);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Double, expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, 2);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Integer, expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, "2.0");
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.String, expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, new Object());
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: java.lang.Object, expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, (QRunnable)()->{});
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: io.qt.core.QRunnable, expected: "+parameter.getName().replace('$', '.'), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
			try {
				invokableMethod.invoke(b, (QEasingCurve.EasingFunction)d->d);
				Assert.fail("IllegalArgumentException expected to be thrown.");
			} catch (IllegalArgumentException e) {
				Assert.assertEquals("Wrong argument given: io.qt.core.QEasingCurve.EasingFunction, expected: "+parameter.getName(), e.getMessage());
			}
			Assert.assertFalse(b.invoked.get());
			Assert.assertEquals(null, b.parameter);
		}
	}
    
    static class JavaProperties{
    	private String string;
    	private QObject object;
    	private JavaProperties other;
    	@QtPropertyReader
		public String getString() {
			return string;
		}
    	@QtPropertyWriter
		public void setString(String string) {
			this.string = string;
		}
    	@QtPropertyReader
		public QObject getObject() {
			return object;
		}
    	@QtPropertyWriter
		public void setObject(QObject object) {
			this.object = object;
		}
    	@QtPropertyReader
		public JavaProperties getOther() {
			return other;
		}
    	@QtPropertyWriter
		public void setOther(JavaProperties other) {
			this.other = other;
		}
    }
    
    @Test
    public void testPropertiesReadWriteOnGadget() {
    	JavaProperties javaProperties = new JavaProperties();
    	QMetaObject metaObject = QMetaObject.forType(JavaProperties.class);
//    	metaObject.properties().forEach(p->System.out.println(p.typeName()+" "+p.name()));
    	QMetaProperty objectProperty = metaObject.property("object");
    	QMetaProperty otherProperty = metaObject.property("other");
    	QMetaProperty stringProperty = metaObject.property("string");
    	QObject object = new QObject();
    	objectProperty.writeOnGadget(javaProperties, object);
    	Assert.assertEquals(object, javaProperties.getObject());
    	Assert.assertEquals(object, objectProperty.readOnGadget(javaProperties));
    	String string = "TEST";
    	stringProperty.writeOnGadget(javaProperties, string);
    	Assert.assertEquals(string, javaProperties.getString());
    	Assert.assertEquals(string, stringProperty.readOnGadget(javaProperties));
    	JavaProperties other = new JavaProperties();
    	otherProperty.writeOnGadget(javaProperties, other);
    	Assert.assertEquals(other, javaProperties.getOther());
    	Assert.assertEquals(other, otherProperty.readOnGadget(javaProperties));
    	try {
			otherProperty.writeOnGadget(javaProperties, object);
			Assert.fail();
		} catch (IllegalArgumentException e) {
		}
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestMetaProgramming.class.getName());
    }
}
