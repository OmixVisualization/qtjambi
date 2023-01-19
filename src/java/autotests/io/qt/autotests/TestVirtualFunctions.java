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

import java.lang.reflect.Method;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QNoImplementationException;
import io.qt.autotests.generated.AbstractClass;
import io.qt.autotests.generated.AnotherNonAbstractSubclass;
import io.qt.autotests.generated.FunctionalTest;
import io.qt.autotests.generated.SetupLayout;
import io.qt.autotests.generated.TestInterfaceObject;
import io.qt.core.QObject;
import io.qt.core.QRect;
import io.qt.core.QSize;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QImage;
import io.qt.gui.QPaintDevice;
import io.qt.gui.QPaintEngine;
import io.qt.gui.QPainter;
import io.qt.widgets.QLayoutItem;
import io.qt.widgets.QProxyStyle;
import io.qt.widgets.QPushButton;
import io.qt.widgets.QSpacerItem;
import io.qt.widgets.QVBoxLayout;
import io.qt.widgets.QWidget;

public class TestVirtualFunctions extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }

	static class JavaNonAbstractSubclass extends AbstractClass {
	    @Override
	    public void abstractFunction(String something) {
	        setS("Even more " + something);
	    }
	
	    @Override
	    public AbstractClass getAbstractClass() {
	        return new JavaNonAbstractSubclass();
	    }
	
		@Override
		public boolean setReferenceCountTest(QObject object) {
			return false;
		}
	}
	
	static class MyLayout extends QVBoxLayout {
	    @Override
	    public QLayoutItem itemAt(int index) {
	        return super.itemAt(index);
	    }
	
	    @Override
	    public void addItem(QLayoutItem arg__0) {
	        super.addItem(arg__0);
	    }
	
	    @Override
	    public int count() {
	        return super.count();
	    }
	
	    @Override
	    public void setGeometry(QRect arg__0) {
	        super.setGeometry(arg__0);
	    }
	
	    @Override
	    public QSize sizeHint() {
	        return super.sizeHint();
	    }
	
	    @Override
	    public QLayoutItem takeAt(int index__0) {
	        return super.takeAt(index__0);
	    }
	}
	
	static class WidgetClass1 extends QWidget {
    	{
    		Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
    	}
        public void setJavaSizeHint(QSize size) {
            m_size = size;
        }

        @Override
        public QSize sizeHint() {
            return m_size;
        }

        private QSize m_size = new QSize(0, 0);
    }

	static class WidgetClass2 extends QWidget {
    	{
    		Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
    	}
        public void setJavaSizeHint(QSize size) {
            m_size = size;
        }

    	@SuppressWarnings("unused")
        private QSize m_size = new QSize(0, 0);
    }

    @Test
    public void run_testOverridingMethodsThatReturnInterfaceTypes() {
    	Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
        QWidget topLevel = new QWidget();
        QPushButton button1 = new QPushButton("Test", topLevel);

        MyLayout layout = new MyLayout();
        layout.addWidget(button1);
        topLevel.setLayout(layout);
        topLevel.dispose();
    }

    @Test
    public void run_testNonQObjectsInCustomLayout() {
    	Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
        QWidget topLevel = new QWidget();
        QSpacerItem spacer = new QSpacerItem(10, 10);

        MyLayout layout = new MyLayout();
        layout.addItem(spacer);
        topLevel.setLayout(layout);
        topLevel.show();
        topLevel.dispose();
    }

    @Test
    public void run_testNonQObjectsInCustomLayoutAddedFromCpp() {
    	Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
        QWidget topLevel = new QWidget();
        MyLayout layout = new MyLayout();
        topLevel.setLayout(layout);

        SetupLayout.setupLayout(layout);

        assertEquals(layout.count(), 4);
        topLevel.show();
        topLevel.dispose();
    }
    
    @Test
    public void testNonVirtualFunctionOnNonShellObject() {
    	TestInterfaceObject obj = TestInterfaceObject.create("");
    	obj.nonVirtualMethod();
    }
    
    @Test
    public void testVirtualFunctionOnNonShellObject() {
    	TestInterfaceObject obj = TestInterfaceObject.create("");
    	obj.virtualMethod();
    }
    
    @Test
    public void testVirtualImplementedFunctionOnNonShellObject() {
    	TestInterfaceObject obj = TestInterfaceObject.create("");
    	obj.virtualImplementedMethod();
    }
    
    @Test
    public void testProtectedVirtualFunctionOnNonShellObject() throws Exception{
    	TestInterfaceObject obj = TestInterfaceObject.create("");
    	Method method = TestInterfaceObject.class.getDeclaredMethod("protectedVirtualMethod");
    	method.setAccessible(true);
    	method.invoke(obj);
    }
    
    @Test
    public void testVirtualInterfaceFunctionOnNonShellObject() {
    	TestInterfaceObject obj = TestInterfaceObject.create("");
    	obj.virtualInterfaceMethod();
    }
    
    @Test
    public void testVirtualImplementedInterfaceFunctionOnNonShellObject() {
    	TestInterfaceObject obj = TestInterfaceObject.create("");
    	obj.virtualImplementedInterfaceMethod();
    }
    
    @Test
    public void testProtectedVirtualInterfaceFunctionOnNonShellObject() throws Exception{
    	TestInterfaceObject obj = TestInterfaceObject.create("");
    	Method method = TestInterfaceObject.class.getDeclaredMethod("protectedVirtualInterfaceMethod");
    	method.setAccessible(true);
    	method.invoke(obj);
    }
    
    @Test
    public void testProtectedVirtualImplementedInterfaceFunctionOnNonShellObject() throws Exception{
    	TestInterfaceObject obj = TestInterfaceObject.create("");
    	Method method = TestInterfaceObject.class.getDeclaredMethod("protectedImplementedVirtualInterfaceMethod");
    	method.setAccessible(true);
    	method.invoke(obj);
    }

    @Test
    public void run_testOneSubclass() {
        WidgetClass1 w = new WidgetClass1();
        w.setJavaSizeHint(new QSize(123, 456));
        assertEquals(w.sizeHint().width(), 123);
        assertEquals(w.sizeHint().height(), 456);
    }

    @Test
    public void run_testTwoSubclasses() {
        WidgetClass1 w1 = new WidgetClass1();
        w1.setJavaSizeHint(new QSize(123, 456));

        assertEquals(w1.sizeHint().width(), 123);
        assertEquals(w1.sizeHint().height(), 456);

        WidgetClass2 w2 = new WidgetClass2();
        w2.setJavaSizeHint(new QSize(654, 321));
        QWidget w = new QWidget();
        assertEquals(w2.sizeHint().width(), w.sizeHint().width());
        assertEquals(w2.sizeHint().height(), w.sizeHint().height());
    }

    // A QObject subclass to call super.paintEngine();
    private interface CallCounter {
        public int callCount();
    }

    private static class Widget extends QWidget implements CallCounter {
        public int called;

        public int callCount() {
            return called;
        }

        @Override
        public QPaintEngine paintEngine() {
            ++called;
            if (called > 1) {
                return null;
            }
            return super.paintEngine();
        }
    }

    // A non QObject subclass to call super.paintEngine();
    private static class Image extends QImage implements CallCounter {
        public int called;

        public Image() {
            super(100, 100, QImage.Format.Format_ARGB32_Premultiplied);
        }

        public int callCount() {
            return called;
        }

        @Override
        public QPaintEngine paintEngine() {
            ++called;
            if (called > 1) {
                return null;
            }
            return super.paintEngine();
        }
    }

    /**
     * The purpose of this test is to verify that we can do things like
     * super.paintEngine() in a QPaintDevice subclass and not get recursion.
     */
    @Test
    public void run_testSupercall() {

        QPaintDevice[] testDevices = new QPaintDevice[] { new Widget(), new Image() };
        for (int i = 0; i < testDevices.length; i++) {
            QPaintDevice device = testDevices[i];
            QPainter p = new QPainter();
            p.begin(device);
            p.end();
            assertEquals(((CallCounter) device).callCount(), 1);
        }
    }

    /**
     * The purpose of this test is to verify that we are calling virtual
     * functions using the correct environment for the current thread. We create
     * a QObject in the main thread and pass it to an executing thread and
     * trigger a virtual function call
     */
    private static class PaintThread extends Thread {
        public Image image;

        @Override
        public void run() {
            QPainter p = new QPainter();
            p.begin(image);
            p.end();
        }
    }

    @Test
    public void run_testEnvironment() {
        PaintThread thread = new PaintThread();
        thread.image = new Image();
        thread.start();
        try {
            thread.join();
        } catch (Exception e) {
            e.printStackTrace();
        }

        assertEquals(((CallCounter) thread.image).callCount(), 1);
    }

    @Test
    @SuppressWarnings("deprecation")
    public void run_abstractClasses() {
        AnotherNonAbstractSubclass obj = new AnotherNonAbstractSubclass();

        obj.setS("a string");
        try {
            obj.abstractFunction("a super-string");
            assertTrue(false); // we should never get here
        } catch (QNoImplementationException e) {
            obj.setS("a non-super string");
        }
        assertEquals(obj.getS(), "a non-super string");

        obj.doVirtualCall(obj, "a super-string");
        assertEquals(obj.getS(), "Not a super-string");

        AbstractClass cls = obj.getAbstractClass();
        assertTrue(cls.getClass().getName().endsWith("$ConcreteWrapper"));

        cls.abstractFunction("my super-string");
        assertEquals(cls.getS(), "my super-string");
        assertEquals(cls.getAbstractClass(), null);
        obj.doVirtualCall(cls, "my non-super string");
        assertEquals(cls.getS(), "my non-super string");

        JavaNonAbstractSubclass foo = new JavaNonAbstractSubclass();
        assertTrue(foo.getAbstractClass() instanceof JavaNonAbstractSubclass);

        foo.abstractFunction("of my super strings");
        assertEquals(foo.getS(), "Even more of my super strings");

        obj.doVirtualCall(foo, "of my non-super strings");
        assertEquals(foo.getS(), "Even more of my non-super strings");
    }

	@SuppressWarnings("unused")
    private boolean myVirtualFunctionWasCalled;
    
    @Test
    public void functionalTest() {
    	FunctionalTest functionalTest = new FunctionalTest();
    	FunctionalTest.TestFunction1 ft1;
    	assertEquals(279*9, functionalTest.convert(ft1 = (i,b)->i*9));
    	ApplicationInitializer.runGC();
    	assertEquals(9*4, functionalTest.last1().applyAsInt(4, true));
    	ft1.dispose();
    	FunctionalTest.TestFunction1 ft = (i,b)->b ? i*9 : -1;
    	assertEquals(4*9, functionalTest.convert(ft, 4, true));
    	ApplicationInitializer.runGC();
    	assertEquals(-1, functionalTest.convert(ft, 4, false));
    	FunctionalTest.TestFunction1 last1 = functionalTest.last1();
    	ApplicationInitializer.runGC();
    	assertEquals(4*9, last1.applyAsInt(4, true));
    	FunctionalTest.TestFunction1 anyTestFunction1 = functionalTest.anyTestFunction1();
    	assertEquals(10, anyTestFunction1.applyAsInt(5, true));
    	assertEquals(-100, functionalTest.anyTestFunction1().applyAsInt(5, false));
    	FunctionalTest.TestFunction2 ft2 = (o)->o.objectName()+"#";
    	assertEquals("TestObject#", functionalTest.convert(ft2));
    	QObject obj = new QObject();
    	obj.setObjectName("TestObject2");
    	assertEquals("TestObject2#", functionalTest.convert(ft2, obj));
    	obj.setObjectName("TestObject3");
    	assertEquals("TestObject3", functionalTest.anyTestFunction2().apply(obj));
//    	instances.add(new WeakReference<>(ft));
//    	instances.add(new WeakReference<>(ft2));
    	ft.dispose();
    	ft2.dispose();
    }
    
    @Test(expected=RuntimeException.class)
    public void functionalTestWithException() {
    	FunctionalTest functionalTest = new FunctionalTest();
    	FunctionalTest.TestFunction1 f1 = null;
    	try {
    		functionalTest.convert(f1 = (i,b)->{throw new RuntimeException();});
    	}finally{
	//    	instances.add(new WeakReference<>(f1));
	    	f1.dispose();
    	}
    }
    
    @Test(expected=RuntimeException.class)
    public void functionalTestWithException2() {
    	FunctionalTest functionalTest = new FunctionalTest();
    	FunctionalTest.TestFunction1 f1 = null;
    	try {
			functionalTest.convert(f1 = (i,b)->{throw new RuntimeException();});
			Assert.assertFalse(true);
		} catch (Throwable e) {
			Assert.assertTrue(e instanceof RuntimeException);
		}
    	try {
	    	functionalTest.last1().applyAsInt(4, true);
    	}finally {
    		//    	instances.add(new WeakReference<>(f1));
    		f1.dispose();
    	}
    }
    
    static class Style extends QProxyStyle{
    }
    
    @Test
    public void testOverriddenAbstract() {
    	new Style();
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestVirtualFunctions.class.getName());
    }
}
