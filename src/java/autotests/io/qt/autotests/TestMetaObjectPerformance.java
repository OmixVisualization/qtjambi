/****************************************************************************
**
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

import static io.qt.autotests.TestPerformance.*;

import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtInvokable;
import io.qt.autotests.generated.General;
import io.qt.core.QCoreApplication;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.Qt;
import io.qt.widgets.*;

public class TestMetaObjectPerformance extends ApplicationInitializer {
	
	static final int count = 25000;
	private static boolean hasSerializableLambdas;
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    	hasSerializableLambdas = General.internalAccess.serializeLambdaExpression((QMetaObject.Slot0)ApplicationInitializer::testInitializeWithWidgets) != null;
    }
	
	static class Widget extends QWidget{
		@QtInvokable
		public void setEnabled2(boolean b){
			b = false;
		}
	}
	
	@Test
    public void testNativeMethodCall() {
		QWidget widget = new QWidget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < count*1000; i++) {
			widget.setEnabled(true);
		}
		long t2 = System.currentTimeMillis();
		System.out.println("Calling C++ method took "+format.format((t2-t1)*1.)+"\u00b5s");
		widget.dispose();
	}
	
	@Test
    public void testJavaMethodCall() {
		Widget widget = new Widget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < count*1000; i++) {
			widget.setEnabled2(true);
		}
		long t2 = System.currentTimeMillis();
		System.out.println("Calling Java method took "+format.format((t2-t1)*1.)+"\u00b5s");
		widget.dispose();
	}
	
	@Test
    public void testNativeObjectMethodHandleDirectInvocation() {
		QWidget widget = new QWidget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < count; i++) {
			QMetaObject.invokeMethod(widget::setEnabled, true);
		}
		long t2 = System.currentTimeMillis();
		System.out.println("Directly invoking C++ object method handle took "+format.format((t2-t1)*1000.)+"\u00b5s");
		widget.dispose();
	}
	
	@Test
    public void testNativeClassMethodHandleDirectInvocation() {
		QWidget widget = new QWidget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < count; i++) {
			QMetaObject.invokeMethod(widget, QWidget::setEnabled, true);
		}
		long t2 = System.currentTimeMillis();
		System.out.println("Directly invoking C++ class method handle took "+format.format((t2-t1)*1000.)+"\u00b5s");
		widget.dispose();
	}
	
	@Test
    public void testContextualLambda1DirectInvocation() {
		QWidget widget = new QWidget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < count; i++) {
			QMetaObject.invokeMethod(widget, (w,b)->{w.setEnabled(b);}, true);
		}
		long t2 = System.currentTimeMillis();
		System.out.println("Directly invoking context-based lambda(1) took "+format.format((t2-t1)*1000.)+"\u00b5s");
		widget.dispose();
	}
	
	@Test
    public void testContextualLambda2DirectInvocation() {
		QWidget widget = new QWidget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < count; i++) {
			QMetaObject.invokeMethod(widget, b->{widget.setEnabled(b);}, true);
		}
		long t2 = System.currentTimeMillis();
		System.out.println("Directly invoking context-based lambda(2) took "+format.format((t2-t1)*1000.)+"\u00b5s");
		widget.dispose();
	}
	
	@Test
    public void testLambdaDirectInvocation() {
		QWidget widget = new QWidget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < count; i++) {
			QMetaObject.invokeMethod(b->{widget.setEnabled(b);}, true);
		}
		long t2 = System.currentTimeMillis();
		System.out.println("Directly invoking context-free lambda took "+format.format((t2-t1)*1000.)+"\u00b5s");
		widget.dispose();
	}
	
	@Test
    public void testNativeMethodSignatureDirectInvocation() {
		QWidget widget = new QWidget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < count; i++) {
			QMetaObject.invokeMethod(widget, "setEnabled(boolean)", true);
		}
		long t2 = System.currentTimeMillis();
		System.out.println("Directly invoking C++ method signature took "+format.format((t2-t1)*1000.)+"\u00b5s");
		widget.dispose();
	}
	
	@Test
    public void testNativeMethodNameDirectInvocation() {
		QWidget widget = new QWidget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < count; i++) {
			QMetaObject.invokeMethod(widget, "setEnabled", true);
		}
		long t2 = System.currentTimeMillis();
		System.out.println("Directly invoking C++ method name took "+format.format((t2-t1)*1000.)+"\u00b5s");
		widget.dispose();
	}
	
	@Test
    public void testJavaObjectMethodHandleDirectInvocation() {
		Widget widget = new Widget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < count; i++) {
			QMetaObject.invokeMethod(widget::setEnabled2, true);
		}
		long t2 = System.currentTimeMillis();
		System.out.println("Directly invoking Java object method handle took "+format.format((t2-t1)*1000.)+"\u00b5s");
		widget.dispose();
	}
	
	@Test
    public void testJavaClassMethodHandleDirectInvocation() {
		Widget widget = new Widget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < count; i++) {
			QMetaObject.invokeMethod(widget, Widget::setEnabled2, true);
		}
		long t2 = System.currentTimeMillis();
		System.out.println("Directly invoking Java class method handle took "+format.format((t2-t1)*1000.)+"\u00b5s");
		widget.dispose();
	}
	
	@Test
    public void testJavaMethodSignatureDirectInvocation() {
		Widget widget = new Widget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < count; i++) {
			QMetaObject.invokeMethod(widget, "setEnabled2(boolean)", true);
		}
		long t2 = System.currentTimeMillis();
		System.out.println("Directly invoking Java method signature took "+format.format((t2-t1)*1000.)+"\u00b5s");
		widget.dispose();
	}
	
	@Test
    public void testJavaMethodNameDirectInvocation() {
		Widget widget = new Widget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < count; i++) {
			QMetaObject.invokeMethod(widget, "setEnabled2", true);
		}
		long t2 = System.currentTimeMillis();
		System.out.println("Directly invoking Java method name took "+format.format((t2-t1)*1000.)+"\u00b5s");
		widget.dispose();
	}
	
	@Test
    public void testNativeMetaMethodDirectInvocation() {
		Assume.assumeTrue("The JVM does not support lambda serialization", hasSerializableLambdas);
		QWidget widget = new QWidget();
		QMetaMethod method = QMetaMethod.fromMethod(QWidget::setEnabled);
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < count; i++) {
			method.invoke(widget, true);
		}
		long t2 = System.currentTimeMillis();
		System.out.println("Directly invoking C++ meta method took "+format.format((t2-t1)*1000.)+"\u00b5s");
		widget.dispose();
	}
	
	@Test
    public void testJavaMetaMethodDirectInvocation() {
		Assume.assumeTrue("The JVM does not support lambda serialization", hasSerializableLambdas);
		Widget widget = new Widget();
		QMetaMethod method = QMetaMethod.fromMethod(Widget::setEnabled2);
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < count; i++) {
			method.invoke(widget, true);
		}
		long t2 = System.currentTimeMillis();
		System.out.println("Directly invoking Java meta method took "+format.format((t2-t1)*1000.)+"\u00b5s");
		widget.dispose();
	}
	
	@Test
    public void testNativeObjectMethodHandleQueuedInvocation() {
		Assume.assumeFalse("Don't run on Android", QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
		QWidget widget = new QWidget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < count; i++) {
			QMetaObject.invokeMethod(widget::setEnabled, Qt.ConnectionType.QueuedConnection, true);
		}
		long t2 = System.currentTimeMillis();
		System.out.println("Preparing queued invokation C++ object method handle took "+format.format((t2-t1)*1000.)+"\u00b5s");
		t1 = System.currentTimeMillis();
		QCoreApplication.processEvents();
		t2 = System.currentTimeMillis();
		System.out.println("Processing queued invokation C++ object method handle took "+format.format((t2-t1)*1000.)+"\u00b5s");
		widget.dispose();
	}
	
	@Test
    public void testNativeClassMethodHandleQueuedInvocation() {
		//Assume.assumeFalse("Don't run on Android", QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
		QWidget widget = new QWidget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < count; i++) {
			QMetaObject.invokeMethod(widget, QWidget::setEnabled, Qt.ConnectionType.QueuedConnection, true);
		}
		long t2 = System.currentTimeMillis();
		System.out.println("Preparing queued invokation C++ class method handle took "+format.format((t2-t1)*1000.)+"\u00b5s");
		t1 = System.currentTimeMillis();
		QCoreApplication.processEvents();
		t2 = System.currentTimeMillis();
		System.out.println("Processing queued invokation C++ class method handle took "+format.format((t2-t1)*1000.)+"\u00b5s");
		widget.dispose();
	}
	
	@Test
    public void testContextualLambda1QueuedInvocation() {
		Assume.assumeFalse("Don't run on Android", QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
		QWidget widget = new QWidget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < count; i++) {
			QMetaObject.<QWidget,Boolean>invokeMethod(widget, (w,b)->{w.setEnabled(b);}, Qt.ConnectionType.QueuedConnection, true);
		}
		long t2 = System.currentTimeMillis();
		System.out.println("Preparing queued invokation context-based lambda(1) took "+format.format((t2-t1)*1000.)+"\u00b5s");
		t1 = System.currentTimeMillis();
		QCoreApplication.processEvents();
		t2 = System.currentTimeMillis();
		System.out.println("Processing queued invokation context-based lambda(1) took "+format.format((t2-t1)*1000.)+"\u00b5s");
		widget.dispose();
	}
	
	@Test
    public void testContextualLambda2QueuedInvocation() {
		Assume.assumeFalse("Don't run on Android", QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
		QWidget widget = new QWidget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < count; i++) {
			QMetaObject.invokeMethod(widget, b->{widget.setEnabled(b);}, Qt.ConnectionType.QueuedConnection, true);
		}
		long t2 = System.currentTimeMillis();
		System.out.println("Preparing queued invokation context-based lambda(2) took "+format.format((t2-t1)*1000.)+"\u00b5s");
		t1 = System.currentTimeMillis();
		QCoreApplication.processEvents();
		t2 = System.currentTimeMillis();
		System.out.println("Processing queued invokation context-based lambda(2) took "+format.format((t2-t1)*1000.)+"\u00b5s");
		widget.dispose();
	}
	
	@Test
    public void testLambdaQueuedInvocation() {
		Assume.assumeFalse("Don't run on Android", QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
		QWidget widget = new QWidget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < count; i++) {
			QMetaObject.invokeMethod(b->{widget.setEnabled(b);}, Qt.ConnectionType.QueuedConnection, true);
		}
		long t2 = System.currentTimeMillis();
		System.out.println("Preparing queued invokation context-free lambda took "+format.format((t2-t1)*1000.)+"\u00b5s");
		t1 = System.currentTimeMillis();
		QCoreApplication.processEvents();
		t2 = System.currentTimeMillis();
		System.out.println("Processing queued invokation context-free lambda took "+format.format((t2-t1)*1000.)+"\u00b5s");
		widget.dispose();
	}
	
	@Test
    public void testNativeMethodSignatureQueuedInvocation() {
		QWidget widget = new QWidget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < count; i++) {
			QMetaObject.invokeMethod(widget, "setEnabled(boolean)", Qt.ConnectionType.QueuedConnection, true);
		}
		long t2 = System.currentTimeMillis();
		System.out.println("Preparing queued invokation C++ method signature took "+format.format((t2-t1)*1000.)+"\u00b5s");
		t1 = System.currentTimeMillis();
		QCoreApplication.processEvents();
		t2 = System.currentTimeMillis();
		System.out.println("Processing queued invokation C++ method signature took "+format.format((t2-t1)*1000.)+"\u00b5s");
		widget.dispose();
	}
	
	@Test
    public void testNativeMethodNameQueuedInvocation() {
		QWidget widget = new QWidget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < count; i++) {
			QMetaObject.invokeMethod(widget, "setEnabled", Qt.ConnectionType.QueuedConnection, true);
		}
		long t2 = System.currentTimeMillis();
		System.out.println("Preparing queued invokation C++ method name took "+format.format((t2-t1)*1000.)+"\u00b5s");
		t1 = System.currentTimeMillis();
		QCoreApplication.processEvents();
		t2 = System.currentTimeMillis();
		System.out.println("Processing queued invokation C++ method name took "+format.format((t2-t1)*1000.)+"\u00b5s");
		widget.dispose();
	}
	
	@Test
    public void testJavaObjectMethodHandleQueuedInvocation() {
		Assume.assumeFalse("Don't run on Android", QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
		Widget widget = new Widget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < count; i++) {
			QMetaObject.invokeMethod(widget::setEnabled2, Qt.ConnectionType.QueuedConnection, true);
		}
		long t2 = System.currentTimeMillis();
		System.out.println("Preparing queued invokation Java object method handle took "+format.format((t2-t1)*1000.)+"\u00b5s");
		t1 = System.currentTimeMillis();
		QCoreApplication.processEvents();
		t2 = System.currentTimeMillis();
		System.out.println("Processing queued invokation Java object method handle took "+format.format((t2-t1)*1000.)+"\u00b5s");
		widget.dispose();
	}
	
	@Test
    public void testJavaClassMethodHandleQueuedInvocation() {
		//Assume.assumeFalse("Don't run on Android", QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
		Widget widget = new Widget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < count; i++) {
			QMetaObject.invokeMethod(widget, Widget::setEnabled2, Qt.ConnectionType.QueuedConnection, true);
		}
		long t2 = System.currentTimeMillis();
		System.out.println("Preparing queued invokation Java class method handle took "+format.format((t2-t1)*1000.)+"\u00b5s");
		t1 = System.currentTimeMillis();
		QCoreApplication.processEvents();
		t2 = System.currentTimeMillis();
		System.out.println("Processing queued invokation Java class method handle took "+format.format((t2-t1)*1000.)+"\u00b5s");
		widget.dispose();
	}
	
	@Test
    public void testJavaMethodSignatureQueuedInvocation() {
		Widget widget = new Widget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < count; i++) {
			QMetaObject.invokeMethod(widget, "setEnabled2(boolean)", Qt.ConnectionType.QueuedConnection, true);
		}
		long t2 = System.currentTimeMillis();
		System.out.println("Preparing queued invokation Java method signature took "+format.format((t2-t1)*1000.)+"\u00b5s");
		t1 = System.currentTimeMillis();
		QCoreApplication.processEvents();
		t2 = System.currentTimeMillis();
		System.out.println("Processing queued invokation Java method signature took "+format.format((t2-t1)*1000.)+"\u00b5s");
		widget.dispose();
	}
	
	@Test
    public void testJavaMethodNameQueuedInvocation() {
		Widget widget = new Widget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < count; i++) {
			QMetaObject.invokeMethod(widget, "setEnabled2", Qt.ConnectionType.QueuedConnection, true);
		}
		long t2 = System.currentTimeMillis();
		System.out.println("Preparing queued invokation Java method name took "+format.format((t2-t1)*1000.)+"\u00b5s");
		t1 = System.currentTimeMillis();
		QCoreApplication.processEvents();
		t2 = System.currentTimeMillis();
		System.out.println("Processing queued invokation Java method name took "+format.format((t2-t1)*1000.)+"\u00b5s");
		widget.dispose();
	}
	
	@Test
    public void testNativeMetaMethodQueuedInvocation() {
		Assume.assumeTrue("The JVM does not support lambda serialization", hasSerializableLambdas);
		QWidget widget = new QWidget();
		QMetaMethod method = QMetaMethod.fromMethod(QWidget::setEnabled);
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < count; i++) {
			method.invoke(widget, Qt.ConnectionType.QueuedConnection, true);
		}
		long t2 = System.currentTimeMillis();
		System.out.println("Preparing queued invokation C++ meta method took "+format.format((t2-t1)*1000.)+"\u00b5s");
		t1 = System.currentTimeMillis();
		QCoreApplication.processEvents();
		t2 = System.currentTimeMillis();
		System.out.println("Processing queued invokation C++ meta method took "+format.format((t2-t1)*1000.)+"\u00b5s");
		widget.dispose();
	}
	
	@Test
    public void testJavaMetaMethodQueuedInvocation() {
		Assume.assumeTrue("The JVM does not support lambda serialization", hasSerializableLambdas);
		Widget widget = new Widget();
		QMetaMethod method = QMetaMethod.fromMethod(Widget::setEnabled2);
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < count; i++) {
			method.invoke(widget, Qt.ConnectionType.QueuedConnection, true);
		}
		long t2 = System.currentTimeMillis();
		System.out.println("Preparing queued invokation Java meta method took "+format.format((t2-t1)*1000.)+"\u00b5s");
		t1 = System.currentTimeMillis();
		QCoreApplication.processEvents();
		t2 = System.currentTimeMillis();
		System.out.println("Processing queued invokation Java meta method took "+format.format((t2-t1)*1000.)+"\u00b5s");
		widget.dispose();
	}
	
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestMetaObjectPerformance.class.getName());
    }
}
