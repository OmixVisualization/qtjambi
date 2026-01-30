/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtObject;
import io.qt.QtSignalEmitterInterface;
import io.qt.core.QInstanceMemberSignals;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaObject.Connection;
import io.qt.core.QObject;
import io.qt.core.QRectF;
import io.qt.gui.QIcon;
import io.qt.gui.QPainter;
import io.qt.widgets.QGraphicsItem;
import io.qt.widgets.QSlider;
import io.qt.widgets.QStyleOptionGraphicsItem;
import io.qt.widgets.QWidget;

public class TestSignalSlotWithCustomTypes extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
	public static class UnregisteredType1{}
	
	public static class UnregisteredSubType1 extends UnregisteredType1{}
	
	public static class UnregisteredType2{}
	
	public static class UnregisteredType3{}
	
	public static class SenderObject1 extends QObject{
		public final Signal0 change0 = new Signal0();
		public final Signal1<UnregisteredType1> change1 = new Signal1<>();
		public final Signal1<UnregisteredType3> change2 = new Signal1<>();
	}
	
	public static class ReceiverObject1 extends QObject{
		UnregisteredType2 received2;
		UnregisteredType1 received1;
		
		public void receive0() {
//			System.out.println("ReceiverObject1.receive0()");
		}
		
		public void receive(UnregisteredType2 received2) {
			this.received2 = received2;
		}

		public void receive(UnregisteredType1 received) {
			this.received1 = received;
//			System.out.println("ReceiverObject1.receive()");
		}
		
		public Object testMethod(UnregisteredType1 a1, UnregisteredType2 a2, String a3, int a4, QGraphicsItem a5, QIcon a6, QObject a7) {
//			System.out.println("ReceiverObject1.testMethod("+a1+", "+a2+", \""+a3+"\", "+a4+", "+a5+", "+a6+", "+a7+")");
			return a2;
		}
		
		public void testMethod(QObject a7) {
//			System.out.println("ReceiverObject1.testMethod("+a7+")");
		}
		
		public void testMethod(QGraphicsItem a7) {
//			System.out.println("ReceiverObject1.testMethod("+a7+")");
		}
		
		public void testMethod(int a7) {
//			System.out.println("ReceiverObject1.testMethod("+a7+")");
		}
		
		public void testMethod(String a7) {
//			System.out.println("ReceiverObject1.testMethod("+a7+")");
		}
		
		public void testMethod(QIcon a7) {
//			System.out.println("ReceiverObject1.testMethod("+a7+")");
		}
	}
	
	public static class SenderObject2 implements QtSignalEmitterInterface, QInstanceMemberSignals{
		public final Signal0 change0 = new Signal0(this);
		public final Signal1<UnregisteredType1> change1 = new Signal1<>(this);
		public final Signal1<UnregisteredType3> change2 = new Signal1<>(this);
	}
	
	public static class ReceiverObject2{
		UnregisteredType2 received2;
		UnregisteredType1 received1;
		
		public void receive0() {
//			System.out.println("ReceiverObject2.receive0()");
		}
		
		public void receive(UnregisteredType2 received2) {
			this.received2 = received2;
		}

		public void receive(UnregisteredType1 received) {
			this.received1 = received;
//			System.out.println("ReceiverObject2.receive()");
		}
	}
	
	@Before
    public void setUp() {
	}
	
	@After
    public void tearDown() {
	}
	
	@Test
    public void testArgumentConversion() {
		ReceiverObject1 o1 = new ReceiverObject1();
		QMetaMethod method = o1.metaObject().method("testMethod", UnregisteredType1.class, UnregisteredType2.class, String.class, int.class, QGraphicsItem.class, QIcon.class, QObject.class);
		Assert.assertTrue(method!=null);
		UnregisteredType1 a1 = new UnregisteredType1();
		UnregisteredType2 a2 = new UnregisteredType2();
		String a3 = "test";
		int a4 = 12345678;
		QGraphicsItem a5 = new QGraphicsItem() {
			@Override
			public String toString() {
				return "QGraphicsItem";
			}

			@Override
			public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {
			}
			
			@Override
			public QRectF boundingRect() {
				return null;
			}
		};
		a5.isDisposed();
		QIcon a6 = new QIcon();
		QObject a7 = new QObject();
		a7.setObjectName("a7");
		Object result = method.invoke(o1, a1, a2, a3, a4, a5, a6, a7);
		Assert.assertEquals(a2, result);
		method = o1.metaObject().method("testMethod", int.class);
		method.invoke(o1, a4);
		method = o1.metaObject().method("testMethod", String.class);
		method.invoke(o1, a3);
		method = o1.metaObject().method("testMethod", QIcon.class);
		method.invoke(o1, a6);
		method = o1.metaObject().method("testMethod", QObject.class);
		method.invoke(o1, a7);
		method = o1.metaObject().method("testMethod", QGraphicsItem.class);
		method.invoke(o1, a5);
	}
	
	@Test
    public void test() {
//		System.out.println("SenderObject1:");
//		QMetaObject.forType(SenderObject1.class).methods().forEach(method->{
//			System.out.println(method);
//		});
//		System.out.println("ReceiverObject1:");
//		QMetaObject.forType(ReceiverObject1.class).methods().forEach(System.out::println);
		SenderObject1 o1 = new SenderObject1();
		ReceiverObject1 r1 = new ReceiverObject1();
		Connection connection = o1.change1.connect(r1::receive);
		Assert.assertTrue(connection instanceof QtObject);
		UnregisteredType1 t1 = new UnregisteredType1();
		o1.change1.emit(t1);
		Assert.assertEquals(t1, r1.received1);
		t1 = new UnregisteredSubType1();
		o1.change1.emit(t1);
		Assert.assertEquals(t1, r1.received1);
		t1 = null;
		o1.change1.emit(t1);
		Assert.assertEquals(t1, r1.received1);
	}
    
    @Test
    public void testVariantSlot() {
    	Object[] result = {null};
    	class Foobar extends QObject {
            void onChanged(Object arg) { result[0] = arg; }
        }
        QSlider slider = new QSlider();
        Foobar foo = new Foobar();
        QMetaObject.Connection connection = slider.valueChanged.connect(foo::onChanged);
        Assert.assertTrue(connection.isConnected());
        slider.setValue(5);
        Assert.assertEquals(5, result[0]);
    }
}
