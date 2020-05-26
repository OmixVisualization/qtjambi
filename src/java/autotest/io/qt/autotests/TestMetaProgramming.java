/****************************************************************************
**
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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

import org.junit.Assert;
import org.junit.Test;

import io.qt.QUnsuccessfulInvocationException;
import io.qt.QtExtensibleEnum;
import io.qt.QtInvokable;
import io.qt.QtPropertyDesignable;
import io.qt.QtPropertyNotify;
import io.qt.QtPropertyReader;
import io.qt.QtPropertyResetter;
import io.qt.QtPropertyUser;
import io.qt.QtPropertyWriter;
import io.qt.QtShortEnumerator;
import io.qt.autotests.generated.FlagsAndEnumTest;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QMetaEnum;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaObject.AbstractPrivateSignal1;
import io.qt.core.QMetaProperty;
import io.qt.core.QMetaType;
import io.qt.core.QMetaType.GenericFlags;
import io.qt.core.QModelIndex;
import io.qt.core.QObject;
import io.qt.core.QThread;
import io.qt.core.QUrl;
import io.qt.core.QVariant;
import io.qt.core.Qt;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QStandardItemModel;
import io.qt.widgets.QAbstractButton;
import io.qt.widgets.QApplication;
import io.qt.widgets.QCheckBox;
import io.qt.widgets.QPushButton;

public class TestMetaProgramming extends QApplicationTest {
	
	private static class FullOfPropertiesSuper extends QObject {

		public FullOfPropertiesSuper() {
			super();
		}
		
        @QtPropertyReader()
        public final int readOnlyProperty() { return 0; }

        @QtPropertyWriter(enabled=false)
        public final void setReadOnlyProperty(int i) { }
	}
	
    private static class FullOfProperties extends FullOfPropertiesSuper {
    	@QtInvokable
    	public FullOfProperties(String test) {
			super();
		}
		@QtPropertyNotify(name="ordinaryProperty")
    	public final Signal1<Integer> ordinaryPropertyChanged = new Signal1<>();
        private boolean isDesignableTest;
        private FullOfProperties(boolean isDesignableTest) {
            this.isDesignableTest = isDesignableTest;
        }

        @SuppressWarnings("unused")
        public final int ordinaryProperty() { 
        	return 8; 
    	}
        @SuppressWarnings("unused")
        public final void setOrdinaryProperty(int i) { 
        }

        @QtPropertyReader(name="annotatedProperty")
        public final int fooBar() { return 0; }

        @QtPropertyWriter(name="annotatedProperty")
        public final void fooBarSetIt(int i) { }

        @QtPropertyReader()
        public final int ordinaryReadOnlyProperty() { return 0; }

        @QtPropertyDesignable(value="false")
        public final int ordinaryNonDesignableProperty() { return 0; }

        @SuppressWarnings("unused")
        public final void setOrdinaryNonDesignableProperty(int i) { }

        @QtPropertyDesignable(value="false")
        @QtPropertyReader(name="annotatedNonDesignableProperty")
        public final int fooBarXyz() { return 0; }
        @QtPropertyWriter()
        public final void setAnnotatedNonDesignableProperty(int i) { }

        @SuppressWarnings("unused")
        public final boolean hasBooleanProperty() { return false; }
        @SuppressWarnings("unused")
        public final void setBooleanProperty(boolean b) { }

        @SuppressWarnings("unused")
        public final boolean isOtherBooleanProperty() { return false; }
        @SuppressWarnings("unused")
        public final void setOtherBooleanProperty(boolean b) { }

        @QtPropertyReader
        public final int resettableProperty() { return 0; }

        @QtPropertyWriter
        public final void setResettableProperty(int i) { }

        @QtPropertyResetter(name="resettableProperty")
        public final void resetResettableProperty() { }

        @QtPropertyDesignable(value="test")
        public final int testDesignableProperty() { 
        	return 0; 
    	}
        @SuppressWarnings("unused")
		public final void setTestDesignableProperty(int i) { 
        }

        @QtPropertyReader
        @QtPropertyUser
        public final int annotatedUserProperty() { return 0; }

        @QtPropertyWriter
        public final void setAnnotatedUserProperty(int i) {}

        @QtPropertyUser
        public final int myUserProperty() { return 0; }
        @SuppressWarnings("unused")
        public final void setMyUserProperty(int i) {}


        @SuppressWarnings("unused")
        public boolean test() {
            return isDesignableTest;
        }
        
        static boolean staticScriptableMethodInvoked;
        
        @QtInvokable
        public static void staticScriptableMethod() {
        	staticScriptableMethodInvoked = true;
        }
    }

    private static class ExpectedValues {
        private boolean writable;
        private boolean resettable;
        private boolean designable;
        private boolean user;
        private boolean hasNotifySignal;
        private String name;

        private ExpectedValues(String name, boolean writable, boolean resettable, boolean designable, boolean user, boolean hasNotifySignal) {
            this.name = name;
            this.writable = writable;
            this.resettable = resettable;
            this.designable = designable;
            this.user = user;
            this.hasNotifySignal = hasNotifySignal;
        }
    }

    @Test
    public void testMetaProperties() {
        ExpectedValues expectedValues[] = {
                new ExpectedValues("testDesignableProperty", true, false, true, false, false),
                new ExpectedValues("ordinaryProperty", true, false, true, false, true),
                new ExpectedValues("annotatedProperty", true, false, true, false, false),
                new ExpectedValues("ordinaryReadOnlyProperty", false, false, true, false, false),
                new ExpectedValues("readOnlyProperty", false, false, true, false, false),
                new ExpectedValues("ordinaryNonDesignableProperty", true, false, false, false, false),
                new ExpectedValues("annotatedNonDesignableProperty", true, false, false, false, false),
                new ExpectedValues("booleanProperty", true, false, true, false, false),
                new ExpectedValues("otherBooleanProperty", true, false, true, false, false),
                new ExpectedValues("resettableProperty", true, true, true, false, false),
                new ExpectedValues("objectName", true, false, true, false, true),
                new ExpectedValues("myUserProperty", true, false, true, true, false),
                new ExpectedValues("annotatedUserProperty", true, false, true, true, false)
        };

        FullOfProperties fop = new FullOfProperties(true);
        for (ExpectedValues e : expectedValues) {
//            Utils.println(1, "Current property: " + e.name);
            QMetaProperty property = fop.metaObject().property(e.name);
            assertTrue(property!=null);
            assertEquals(property.name(), e.writable, property.isWritable());
            assertEquals(property.name(), e.resettable, property.isResettable());
            assertEquals(property.name(), e.designable, property.isDesignable(fop));
            assertEquals(property.name(), e.user, property.isUser());
            assertEquals(property.name(), e.hasNotifySignal, property.hasNotifySignal());
            if(property.hasNotifySignal()) {
            	switch(property.name()) {
            	case "ordinaryProperty":
            		assertEquals(property.name(), fop.ordinaryPropertyChanged, property.notifySignal(fop)); break;
            	case "objectName":
            		assertEquals(property.name(), fop.objectNameChanged, property.notifySignal(fop)); break;
            	}
            }
        }
        List<QMetaProperty> properties = fop.metaObject().properties();
        assertEquals(expectedValues.length, properties.size());
    }

    @Test
    public void testFunctionDesignableProperty() {
        {
            FullOfProperties fop = new FullOfProperties(true);
            QMetaProperty property = fop.metaObject().property("testDesignableProperty");
            assertTrue(property!=null);
            assertTrue(property.isDesignable(fop));
        }

        {
            FullOfProperties fop = new FullOfProperties(false);
            QMetaProperty property = fop.metaObject().property("testDesignableProperty");
            assertTrue(property!=null);
            assertFalse(property.isDesignable(fop));
        }
    }

    @Test
    public void testUserPropertyInQt() {
        QAbstractButton b = new QCheckBox();

        QMetaProperty property = b.metaObject().userProperty();
        assertEquals("checked", property.name());
        assertEquals(true, property.isUser());
    }
    
    @Test
    public void testMetaMethodCall() throws NoSuchMethodException {
    	FullOfProperties fop = new FullOfProperties(true);
    	QObject object = fop.metaObject().newInstance("test");
    	assertTrue(object instanceof FullOfProperties);
    	QMetaObject.forType(FullOfProperties.class).method("staticScriptableMethod").invoke(fop);
    	assertTrue(FullOfProperties.staticScriptableMethodInvoked);
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
//    	hiddenType.metaObject().methods().forEach(mt -> { System.out.println(mt.methodType() + ": " + mt.returnType() + " " + mt.enclosingMetaObject().className() + "::" + mt.methodSignature() ); } );
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
    	Assert.assertTrue(method!=null);
    	method.invoke(hiddenType, QVariant.convert(5, method.parameterMetaType(0)));
    	method.invoke(hiddenType, QVariant.convert(9, method.parameterMetaType(0)));
    	Assert.assertEquals(2, hiddenClassEnums.size());
    	Assert.assertEquals(5, hiddenClassEnums.get(0).value());
    	Assert.assertEquals(9, hiddenClassEnums.get(1).value());
    	
    	method = hiddenType.metaObject().method("setHiddenFlags", QMetaType.GenericFlags.class);
    	Assert.assertTrue(method!=null);
    	method.invoke(hiddenType, 6);
    	method.invoke(hiddenType, 2);
    	Assert.assertEquals(2, hiddenFlags.size());
    	Assert.assertEquals(6, hiddenFlags.get(0).value());
    	Assert.assertEquals(2, hiddenFlags.get(1).value());
    	
    	method = hiddenType.metaObject().method("hiddenEnum");
    	Assert.assertTrue(method!=null);
    	Assert.assertEquals(9, ((QMetaType.GenericEnumerator)method.invoke(hiddenType)).value());

    	method = hiddenType.metaObject().method("hiddenFlags");
    	Assert.assertTrue(method!=null);
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
		private boolean invoked = false;
		private QThread invokingThread = null;
		
		@QtInvokable
        public final void invokableSlot0() {
        	this.invoked = true;
        	invokingThread = QThread.currentThread();
        }
		
		@QtInvokable
        public final String invokableMethod3(double d, float f, int i) {
        	this.invoked = true;
        	invokingThread = QThread.currentThread();
        	return "TEST"+i;
        }
	}
	
	@Test
    public void testQMetaMethod_invoke_Slot0_Queued() {
		InvocationTest b = new InvocationTest();
		QMetaMethod invokableSlot = b.metaObject().method("invokableSlot0");
    	Assert.assertTrue(invokableSlot!=null);
		invokableSlot.invoke(b, Qt.ConnectionType.QueuedConnection);
		Assert.assertFalse(b.invoked);
		QCoreApplication.sendPostedEvents(b);
		Assert.assertTrue(b.invoked);
		Assert.assertEquals(QThread.currentThread(), b.invokingThread);
	}
	
	@Test
    public void testQMetaMethod_invoke_Slot0_Direct() {
		InvocationTest b = new InvocationTest();
		QMetaMethod invokableSlot = b.metaObject().method("invokableSlot0");
    	Assert.assertTrue(invokableSlot!=null);
		invokableSlot.invoke(b, Qt.ConnectionType.DirectConnection);
		Assert.assertTrue(b.invoked);
		Assert.assertEquals(QThread.currentThread(), b.invokingThread);
	}
	
	@Test
    public void testQMetaMethod_invoke_Slot0_Auto() {
		InvocationTest b = new InvocationTest();
		QMetaMethod invokableSlot = b.metaObject().method("invokableSlot0");
    	Assert.assertTrue(invokableSlot!=null);
		invokableSlot.invoke(b, Qt.ConnectionType.AutoConnection);
		Assert.assertTrue(b.invoked);
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
			Assert.assertTrue(b.invoked);
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
			while(!b.invoked)
				Thread.sleep(5);
			Assert.assertTrue(b.invoked);
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
			Assert.assertTrue(b.invoked);
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
			while(!b.invoked)
				Thread.sleep(5);
			Assert.assertTrue(b.invoked);
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
			Assert.assertFalse("Expected to throw a QUnsuccessfulInvocationException.", false);
		} catch (QUnsuccessfulInvocationException e) {
		}
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Slot0_Queued() {
		InvocationTest b = new InvocationTest();
    	QMetaObject.invokeMethod(b::invokableSlot0, Qt.ConnectionType.QueuedConnection);
		Assert.assertFalse(b.invoked);
		QCoreApplication.sendPostedEvents(b);
		Assert.assertTrue(b.invoked);
		Assert.assertEquals(QThread.currentThread(), b.invokingThread);
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Slot0_Direct() {
		InvocationTest b = new InvocationTest();
		QMetaObject.invokeMethod(b::invokableSlot0, Qt.ConnectionType.DirectConnection);
		Assert.assertTrue(b.invoked);
		Assert.assertEquals(QThread.currentThread(), b.invokingThread);
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Slot0_Auto() {
		InvocationTest b = new InvocationTest();
		QMetaObject.invokeMethod(b::invokableSlot0, Qt.ConnectionType.AutoConnection);
		Assert.assertTrue(b.invoked);
		Assert.assertEquals(QThread.currentThread(), b.invokingThread);
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Slot0_BlockingQueued_threaded() {
		InvocationTest b = new InvocationTest();
		QThread thread = new QThread();
		try {
			b.moveToThread(thread);
			thread.start();
			QMetaObject.invokeMethod(b::invokableSlot0, Qt.ConnectionType.BlockingQueuedConnection);
			Assert.assertTrue(b.invoked);
			Assert.assertEquals(thread, b.invokingThread);
		}finally {
			thread.quit();
		}
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Slot0_Queued_threaded() throws InterruptedException {
		InvocationTest b = new InvocationTest();
		QThread thread = new QThread();
		try {
			b.moveToThread(thread);
			thread.start();
			QMetaObject.invokeMethod(b::invokableSlot0, Qt.ConnectionType.QueuedConnection);
			while(!b.invoked)
				Thread.sleep(5);
			Assert.assertTrue(b.invoked);
			Assert.assertEquals(thread, b.invokingThread);
		}finally {
			thread.quit();
		}
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Slot0_Direct_threaded() throws InterruptedException {
		InvocationTest b = new InvocationTest();
		QThread thread = new QThread();
		try {
			b.moveToThread(thread);
			thread.start();
			QMetaObject.invokeMethod(b::invokableSlot0, Qt.ConnectionType.DirectConnection);
			Assert.assertTrue(b.invoked);
			Assert.assertEquals(QThread.currentThread(), b.invokingThread);
		}finally {
			thread.quit();
		}
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Slot0_Auto_threaded() throws InterruptedException {
		InvocationTest b = new InvocationTest();
		QThread thread = new QThread();
		try {
			b.moveToThread(thread);
			thread.start();
			QMetaObject.invokeMethod(b::invokableSlot0, Qt.ConnectionType.AutoConnection);
			while(!b.invoked)
				Thread.sleep(5);
			Assert.assertTrue(b.invoked);
			Assert.assertEquals(thread, b.invokingThread);
		}finally {
			thread.quit();
		}
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Slot0_BlockingQueued() {
		InvocationTest b = new InvocationTest();
		try {
			QMetaObject.invokeMethod(b::invokableSlot0, Qt.ConnectionType.BlockingQueuedConnection);
			Assert.assertFalse("Expected to throw a QUnsuccessfulInvocationException.", false);
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
			Assert.assertFalse("Expected to throw a QUnsuccessfulInvocationException.", false);
		} catch (QUnsuccessfulInvocationException e) {
		}
	}
	
	@Test
    public void testQMetaMethod_invoke_Method3_Direct() {
		InvocationTest b = new InvocationTest();
		QMetaMethod invokableMethod = b.metaObject().method("invokableMethod3", double.class, float.class, int.class);
    	Assert.assertTrue(invokableMethod!=null);
    	Object result = invokableMethod.invoke(b, Qt.ConnectionType.DirectConnection, 2.0, 4f, 7);
		Assert.assertTrue(b.invoked);
		Assert.assertEquals(QThread.currentThread(), b.invokingThread);
		Assert.assertEquals("TEST7", result);
	}
	
	@Test
    public void testQMetaMethod_invoke_Method3_Auto() {
		InvocationTest b = new InvocationTest();
		QMetaMethod invokableMethod = b.metaObject().method("invokableMethod3", double.class, float.class, int.class);
    	Assert.assertTrue(invokableMethod!=null);
    	Object result = invokableMethod.invoke(b, Qt.ConnectionType.AutoConnection, 2.0, 4f, 7);
		Assert.assertTrue(b.invoked);
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
			Assert.assertTrue(b.invoked);
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
			Assert.assertFalse("Expected to throw a QUnsuccessfulInvocationException.", false);
		} catch (QUnsuccessfulInvocationException e) {
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
			Assert.assertTrue(b.invoked);
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
			Assert.assertFalse("Expected to throw a QUnsuccessfulInvocationException.", false);
		} catch (QUnsuccessfulInvocationException e) {
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
			Assert.assertFalse("Expected to throw a QUnsuccessfulInvocationException.", false);
		} catch (QUnsuccessfulInvocationException e) {
		}
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Method3_Queued() {
		InvocationTest b = new InvocationTest();
		try {
	    	QMetaObject.invokeMethod(b::invokableMethod3, Qt.ConnectionType.QueuedConnection, 2.0, 4f, 7);
			Assert.assertFalse("Expected to throw a QUnsuccessfulInvocationException.", false);
		} catch (QUnsuccessfulInvocationException e) {
		}
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Method3_Direct() {
		InvocationTest b = new InvocationTest();
		String result = QMetaObject.invokeMethod(b::invokableMethod3, Qt.ConnectionType.DirectConnection, 2.0, 4f, 7);
		Assert.assertTrue(b.invoked);
		Assert.assertEquals(QThread.currentThread(), b.invokingThread);
		Assert.assertEquals("TEST7", result);
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Method3_Auto() {
		InvocationTest b = new InvocationTest();
		String result = QMetaObject.invokeMethod(b::invokableMethod3, Qt.ConnectionType.AutoConnection, 2.0, 4f, 7);
		Assert.assertTrue(b.invoked);
		Assert.assertEquals(QThread.currentThread(), b.invokingThread);
		Assert.assertEquals("TEST7", result);
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Method3_BlockingQueued_threaded() {
		InvocationTest b = new InvocationTest();
		QThread thread = new QThread();
		try {
			b.moveToThread(thread);
			thread.start();
			String result = QMetaObject.invokeMethod(b::invokableMethod3, Qt.ConnectionType.BlockingQueuedConnection, 2.0, 4f, 7);
			Assert.assertTrue(b.invoked);
			Assert.assertEquals(thread, b.invokingThread);
			Assert.assertEquals("TEST7", result);
		}finally {
			thread.quit();
		}
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Method3_Queued_threaded() throws InterruptedException {
		InvocationTest b = new InvocationTest();
		QThread thread = new QThread();
		try {
			b.moveToThread(thread);
			thread.start();
			QMetaObject.invokeMethod(b::invokableMethod3, Qt.ConnectionType.QueuedConnection, 2.0, 4f, 7);
			Assert.assertFalse("Expected to throw a QUnsuccessfulInvocationException.", false);
		} catch (QUnsuccessfulInvocationException e) {
		}finally {
			thread.quit();
		}
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Method3_Direct_threaded() throws InterruptedException {
		InvocationTest b = new InvocationTest();
		QThread thread = new QThread();
		try {
			b.moveToThread(thread);
			thread.start();
			String result = QMetaObject.invokeMethod(b::invokableMethod3, Qt.ConnectionType.DirectConnection, 2.0, 4f, 7);
			Assert.assertTrue(b.invoked);
			Assert.assertEquals(QThread.currentThread(), b.invokingThread);
			Assert.assertEquals("TEST7", result);
		}finally {
			thread.quit();
		}
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Method3_Auto_threaded() throws InterruptedException {
		InvocationTest b = new InvocationTest();
		QThread thread = new QThread();
		try {
			b.moveToThread(thread);
			thread.start();
			QMetaObject.invokeMethod(b::invokableMethod3, Qt.ConnectionType.AutoConnection, 2.0, 4f, 7);
			Assert.assertFalse("Expected to throw a QUnsuccessfulInvocationException.", false);
		} catch (QUnsuccessfulInvocationException e) {
		}finally {
			thread.quit();
		}
	}
	
	@Test
    public void testQMetaObject_invokeMethod_Method3_BlockingQueued() {
		InvocationTest b = new InvocationTest();
		try {
			QMetaObject.invokeMethod(b::invokableMethod3, Qt.ConnectionType.BlockingQueuedConnection, 2.0, 4f, 7);
			Assert.assertFalse("Expected to throw a QUnsuccessfulInvocationException.", false);
		} catch (QUnsuccessfulInvocationException e) {
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
			Assert.assertFalse("Expected to throw a QUnsuccessfulInvocationException.", false);
		} catch (QUnsuccessfulInvocationException e) {
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
			Assert.assertFalse("Expected to throw a QUnsuccessfulInvocationException.", false);
		} catch (QUnsuccessfulInvocationException e) {
		}
	}
	
	@Test
    public void testQMetaObject_invokeMethod_ReturningLambda_BlockingQueued() {
		try {
			QMetaObject.invokeMethod((d,f,i)->{ return "TEST"+i; }, Qt.ConnectionType.BlockingQueuedConnection, 2.0, 4f, 7);
			Assert.assertFalse("Expected to throw a QUnsuccessfulInvocationException.", false);
		} catch (QUnsuccessfulInvocationException e) {
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
        	Assert.assertEquals(ArrayList.class, b.intList.getClass());
    	}
    	{
    		metaProperty = b.metaObject().property("intList");
        	Assert.assertTrue(metaProperty!=null);
        	metaProperty.write(b, new LinkedList<>());
        	Assert.assertEquals(LinkedList.class, b.intList.getClass());
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

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestMetaProgramming.class.getName());
    }
}
