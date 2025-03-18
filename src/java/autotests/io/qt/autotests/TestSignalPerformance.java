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

import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtInvokable;
import io.qt.QtObject;
import io.qt.QtObjectInterface;
import io.qt.autotests.TestSignalSlotWithCustomTypes.ReceiverObject1;
import io.qt.autotests.TestSignalSlotWithCustomTypes.ReceiverObject2;
import io.qt.autotests.TestSignalSlotWithCustomTypes.SenderObject1;
import io.qt.autotests.TestSignalSlotWithCustomTypes.SenderObject2;
import io.qt.autotests.TestSignalSlotWithCustomTypes.UnregisteredType1;
import io.qt.autotests.generated.General;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaObject.Connection;
import io.qt.gui.*;
import io.qt.widgets.*;

public class TestSignalPerformance extends ApplicationInitializer {
	
	static final int count = 25000;
	private static boolean hasSerializableLambdas;
	
	@BeforeClass
    public static void testInitialize() throws Exception {
		Assume.assumeTrue("Performance tests are disabled. Specify -Denable-performance-tests=true to enable them.", Boolean.getBoolean("enable-performance-tests"));
    	ApplicationInitializer.testInitializeWithWidgets();
    	hasSerializableLambdas = General.internalAccess.serializeLambdaExpression((QMetaObject.Slot0)ApplicationInitializer::testInitializeWithWidgets) != null;
    }
	
	static class Widget extends QWidget{
		@QtInvokable
		public void setEnabled2(boolean b){}
	}
	
	@Test
    public void testNativeObjectMethodHandleConnection() {
		Assume.assumeTrue("The JVM does not support lambda serialization", hasSerializableLambdas);
		QAction[] actions = new QAction[count];
		QMetaObject.Connection[] connections = new QMetaObject.Connection[count];
		for (int i = 0; i < actions.length; i++) {
			actions[i] = new QAction();
		}
		QWidget widget = new QWidget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < actions.length; i++) {
			connections[i] = actions[i].toggled.connect(widget::setEnabled);
		}
		long t2 = System.currentTimeMillis();
		for (int i = 0; i < connections.length; i++) {
			Assert.assertTrue(connections[i].isConnected());
		}
		System.out.println("Connecting C++ object method handle took "+format.format((t2-t1)*1000.)+"\u00b5s");
		for (int i = 0; i < connections.length; i++) {
			((QtObjectInterface)connections[i]).dispose();
		}
		for (int i = 0; i < actions.length; i++) {
			actions[i].dispose();
		}
		widget.dispose();
		actions = null;
		connections = null;
		runGC();
	}
	
	@Test
    public void testNativeClassMethodHandleConnection() {
		Assume.assumeTrue("The JVM does not support lambda serialization", hasSerializableLambdas);
		QAction[] actions = new QAction[count];
		QMetaObject.Connection[] connections = new QMetaObject.Connection[count];
		for (int i = 0; i < actions.length; i++) {
			actions[i] = new QAction();
		}
		QWidget widget = new QWidget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < actions.length; i++) {
			connections[i] = actions[i].toggled.connect(widget, QWidget::setEnabled);
		}
		long t2 = System.currentTimeMillis();
		for (int i = 0; i < connections.length; i++) {
			Assert.assertTrue(connections[i].isConnected());
		}
		System.out.println("Connecting C++ class method handle took "+format.format((t2-t1)*1000.)+"\u00b5s");
		for (int i = 0; i < connections.length; i++) {
			((QtObjectInterface)connections[i]).dispose();
		}
		for (int i = 0; i < actions.length; i++) {
			actions[i].dispose();
		}
		widget.dispose();
		actions = null;
		connections = null;
		runGC();
	}
	
	@Test
    public void testNativeMethodSignatureConnection() {
		QAction[] actions = new QAction[count];
		QMetaObject.Connection[] connections = new QMetaObject.Connection[count];
		for (int i = 0; i < actions.length; i++) {
			actions[i] = new QAction();
		}
		QWidget widget = new QWidget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < actions.length; i++) {
			connections[i] = actions[i].toggled.connect(widget, "setEnabled(boolean)");
		}
		long t2 = System.currentTimeMillis();
		for (int i = 0; i < connections.length; i++) {
			Assert.assertTrue(connections[i].isConnected());
		}
		System.out.println("Connecting C++ method signature took "+format.format((t2-t1)*1000.)+"\u00b5s");
		for (int i = 0; i < connections.length; i++) {
			((QtObjectInterface)connections[i]).dispose();
		}
		for (int i = 0; i < actions.length; i++) {
			actions[i].dispose();
		}
		widget.dispose();
		actions = null;
		connections = null;
		runGC();
	}
	
	@Test
    public void testJavaObjectMethodHandleConnection() {
		Assume.assumeTrue("The JVM does not support lambda serialization", hasSerializableLambdas);
		QAction[] actions = new QAction[count];
		QMetaObject.Connection[] connections = new QMetaObject.Connection[count];
		for (int i = 0; i < actions.length; i++) {
			actions[i] = new QAction();
		}
		Widget widget = new Widget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < actions.length; i++) {
			connections[i] = actions[i].toggled.connect(widget::setEnabled2);
		}
		long t2 = System.currentTimeMillis();
		for (int i = 0; i < connections.length; i++) {
			Assert.assertTrue(connections[i].isConnected());
		}
		System.out.println("Connecting Java object method handle took "+format.format((t2-t1)*1000.)+"\u00b5s");
		for (int i = 0; i < connections.length; i++) {
			((QtObjectInterface)connections[i]).dispose();
		}
		for (int i = 0; i < actions.length; i++) {
			actions[i].dispose();
		}
		widget.dispose();
		actions = null;
		connections = null;
		runGC();
	}
	
	@Test
    public void testJavaClassMethodHandleConnection() {
		Assume.assumeTrue("The JVM does not support lambda serialization", hasSerializableLambdas);
		QAction[] actions = new QAction[count];
		QMetaObject.Connection[] connections = new QMetaObject.Connection[count];
		for (int i = 0; i < actions.length; i++) {
			actions[i] = new QAction();
		}
		Widget widget = new Widget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < actions.length; i++) {
			connections[i] = actions[i].toggled.connect(widget, Widget::setEnabled2);
		}
		long t2 = System.currentTimeMillis();
		for (int i = 0; i < connections.length; i++) {
			Assert.assertTrue(connections[i].isConnected());
		}
		System.out.println("Connecting Java class method handle took "+format.format((t2-t1)*1000.)+"\u00b5s");
		for (int i = 0; i < connections.length; i++) {
			((QtObjectInterface)connections[i]).dispose();
		}
		for (int i = 0; i < actions.length; i++) {
			actions[i].dispose();
		}
		widget.dispose();
		actions = null;
		connections = null;
		runGC();
	}
	
	@Test
    public void testJavaMethodSignatureConnection() {
		QAction[] actions = new QAction[count];
		QMetaObject.Connection[] connections = new QMetaObject.Connection[count];
		for (int i = 0; i < actions.length; i++) {
			actions[i] = new QAction();
		}
		Widget widget = new Widget();
		long t1 = System.currentTimeMillis();
		for (int i = 0; i < actions.length; i++) {
			connections[i] = actions[i].toggled.connect(widget, "setEnabled2(boolean)");
		}
		long t2 = System.currentTimeMillis();
		for (int i = 0; i < connections.length; i++) {
			Assert.assertTrue(connections[i].isConnected());
		}
		System.out.println("Connecting Java method signature took "+format.format((t2-t1)*1000.)+"\u00b5s");
		for (int i = 0; i < connections.length; i++) {
			((QtObjectInterface)connections[i]).dispose();
		}
		for (int i = 0; i < actions.length; i++) {
			actions[i].dispose();
		}
		widget.dispose();
		actions = null;
		connections = null;
		runGC();
	}
	
	@Test
    public void testNativeMethodHandleEmit() {
		Assume.assumeTrue("The JVM does not support lambda serialization", hasSerializableLambdas);
		QAction action = new QAction();
		QMetaObject.Connection[] connections = new QMetaObject.Connection[count];
		QWidget widget = new QWidget();
		for (int i = 0; i < connections.length; i++) {
			connections[i] = action.toggled.connect(widget::setEnabled);
			Assert.assertTrue(connections[i].isConnected());
		}
		long t1 = System.currentTimeMillis();
		action.toggled.emit(true);
		long t2 = System.currentTimeMillis();
		System.out.println("Sending to C++ method handle took "+format.format((t2-t1)*1000.)+"\u00b5s");for (int i = 0; i < connections.length; i++) {
			((QtObjectInterface)connections[i]).dispose();
		}
		widget.dispose();
		action.dispose();
		connections = null;
		runGC();
	}
	
	@Test
    public void testNativeMethodSignatureEmit() {
		QAction action = new QAction();
		QMetaObject.Connection[] connections = new QMetaObject.Connection[count];
		QWidget widget = new QWidget();
		for (int i = 0; i < connections.length; i++) {
			connections[i] = action.toggled.connect(widget, "setEnabled(boolean)");
			Assert.assertTrue(connections[i].isConnected());
		}
		long t1 = System.currentTimeMillis();
		action.toggled.emit(true);
		long t2 = System.currentTimeMillis();
		System.out.println("Sending to C++ method signature took "+format.format((t2-t1)*1000.)+"\u00b5s");
		for (int i = 0; i < connections.length; i++) {
			((QtObjectInterface)connections[i]).dispose();
		}
		widget.dispose();
		action.dispose();
		connections = null;
		runGC();
	}
	
	@Test
    public void testJavaMethodHandleEmit() {
		Assume.assumeTrue("The JVM does not support lambda serialization", hasSerializableLambdas);
		QAction action = new QAction();
		QMetaObject.Connection[] connections = new QMetaObject.Connection[count];
		Widget widget = new Widget();
		for (int i = 0; i < connections.length; i++) {
			connections[i] = action.toggled.connect(widget::setEnabled2);
			Assert.assertTrue(connections[i].isConnected());
		}
		long t1 = System.currentTimeMillis();
		action.toggled.emit(true);
		long t2 = System.currentTimeMillis();
		System.out.println("Sending to Java method handle took "+format.format((t2-t1)*1000.)+"\u00b5s");
		for (int i = 0; i < connections.length; i++) {
			((QtObjectInterface)connections[i]).dispose();
		}
		widget.dispose();
		action.dispose();
		connections = null;
		runGC();
	}
	
	@Test
    public void testJavaMethodSignatureEmit() {
		QAction action = new QAction();
		QMetaObject.Connection[] connections = new QMetaObject.Connection[count];
		Widget widget = new Widget();
		for (int i = 0; i < connections.length; i++) {
			connections[i] = action.toggled.connect(widget, "setEnabled2(boolean)");
			Assert.assertTrue(connections[i].isConnected());
		}
		long t1 = System.currentTimeMillis();
		action.toggled.emit(true);
		long t2 = System.currentTimeMillis();
		System.out.println("Sending to Java method signature took "+format.format((t2-t1)*1000.)+"\u00b5s");
		for (int i = 0; i < connections.length; i++) {
			((QtObjectInterface)connections[i]).dispose();
		}
		widget.dispose();
		action.dispose();
		connections = null;
		runGC();
	}
	
	@Test
    public void testConnectionPerformance() {
		SenderObject1 o1 = new SenderObject1();
		ReceiverObject1 r1 = new ReceiverObject1();
		Connection connection1 = o1.change1.connect(r1::receive);
		Assert.assertTrue(connection1 instanceof QtObject);
		SenderObject2 o2 = new SenderObject2();
		ReceiverObject2 r2 = new ReceiverObject2();
		Connection connection2 = o2.change1.connect(r2::receive);
		Assert.assertFalse(connection2 instanceof QtObject);
		UnregisteredType1 t = new UnregisteredType1();
		int count = 1000;
		long time11 = System.currentTimeMillis();
		o1.change0.disconnect();
		for(int i=0; i<count; ++i) {
			o1.change1.emit(t);
		}
		long time21 = System.currentTimeMillis();
		for(int i=0; i<count; ++i) {
			o2.change1.emit(t);
		}
		long time31 = System.currentTimeMillis();
		o1.change0.connect(r1::receive0);
		o2.change0.connect(r2::receive0);
		long time12 = System.currentTimeMillis();
		for(int i=0; i<count; ++i) {
			o1.change0.emit();
		}
		long time22 = System.currentTimeMillis();
		for(int i=0; i<count; ++i) {
			o2.change0.emit();
		}
		long time32 = System.currentTimeMillis();
		System.out.println("    native connection takes "+(time21-time11)+"ms");
		System.out.println("reflective connection takes "+(time31-time21)+"ms");
		System.out.println("    native connection takes "+(time22-time12)+"ms");
		System.out.println("reflective connection takes "+(time32-time22)+"ms");
	}

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestSignalPerformance.class.getName());
    }
}
