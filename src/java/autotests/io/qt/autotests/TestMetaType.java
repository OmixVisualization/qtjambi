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

import static org.junit.Assert.*;

import java.io.*;
import java.net.*;

import org.junit.*;

import io.qt.*;
import io.qt.autotests.generated.*;
import io.qt.core.*;
import io.qt.gui.*;
import io.qt.widgets.*;

public class TestMetaType extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
		testInitializeWithWidgets();
    }
	
	@Test
	public void testFromClassLoader() throws Exception {
		Assume.assumeTrue("Qt version >= 6.0", QLibraryInfo.version().compareTo(new QVersionNumber(6,0))>=0);
		Assume.assumeTrue("no qtjambi.antcontrib set", System.getProperties().stringPropertyNames().contains("qtjambi.antcontrib"));
		try(URLClassLoader cl = new URLClassLoader(new URL[]{new File(System.getProperty("qtjambi.antcontrib", ".")).toURI().toURL()})){
			Class<?> cls2 = cl.loadClass("org.apache.tools.ant.taskdefs.Echo");
			QtUtilities.usePackageContentAsGadgets(cls2.getPackage().getName());
			QtUtilities.usePackageContentAsGadgets("org.apache.tools.ant");
			QMetaObject mo2 = QMetaObject.forType(cls2);
			QMetaProperty location = mo2.property("location");
			assertEquals("JObjectWrapper<org::apache::tools::ant::Location>", location.typeName());
//			mo2.properties().forEach(p->System.out.println(p.typeName()+" "+p.name()+" ("+p.userType()+" "+p.classType()+")"));
		}
	}
	
	@Test
    public void testFlagsConnect() {
		class Receiver extends QObject{
			void onWindowStateChanged(Qt.WindowStates oldState, Qt.WindowStates newState) {
			}
		}
		Receiver receiver = new Receiver();
		QMdiSubWindow subwindow = new QMdiSubWindow();
		subwindow.windowStateChanged.connect(receiver::onWindowStateChanged);
//		QMetaType t = QMetaType.fromType(Qt.WindowStates.class);
//		System.out.println(t.id() + " " + t.name());
//		QMetaMethod mtd = QMetaMethod.fromSignal(subwindow.windowStateChanged);
//		for (int i = 0; i < mtd.parameterCount(); i++) {
//			System.out.println(mtd.parameterType(i)+" "+mtd.parameterTypeName(i)+" "+mtd.parameterMetaType(i));
//		}
	}
	
	@Test
    public void testOwnership() {
		QByteArray ba = (QByteArray)new QMetaType(QMetaType.Type.QByteArray).create();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(ba));
	}
	
	@Test
    public void testWellTyped() {
		assertEquals(new QMetaType(QMetaType.Type.QString), QMetaType.fromType(QString.class));
		assertEquals(new QMetaType(QMetaType.Type.QVariant), QMetaType.fromType(QVariant.class));
		assertEquals(new QMetaType(QMetaType.Type.QString), QMetaType.fromObject(new QString()));
		assertEquals(new QMetaType(QMetaType.Type.QVariant), QMetaType.fromObject(new QVariant()));
	}
	
	@Test
    public void testRegisterStreams() {
    	try {
			QMetaType.registerDataStreamOperators((QDataStream s, MetaValue value)->{
				s.writeInt(value.i);
				s.writeDouble(value.d);
				s.writeString(value.s);
			}, s->{
				MetaValue value = new MetaValue();
				value.i = s.readInt();
				value.d = s.readDouble();
				value.s = s.readString();
				return value;
			});
			try {
				QMetaType.registerDataStreamOperators((QDataStream s, MetaValue value)->{}, s->null);
				fail("RuntimeException expected to be thrown.");
			} catch (RuntimeException e) {
			}
		} catch (IllegalArgumentException e1) {
			if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)) {
				QMetaType.registerDataStreamOperators(new QMetaType.DataStreamInFn<MetaValue>(){
					private static final long serialVersionUID = 1L;
					@Override
					public void accept(QDataStream s, MetaValue value) {
						s.writeInt(value.i);
						s.writeDouble(value.d);
						s.writeString(value.s);
					}
				},
				new QMetaType.DataStreamOutFn<MetaValue>(){
					private static final long serialVersionUID = 1L;
					@Override
					public MetaValue apply(QDataStream s) {
						MetaValue value = new MetaValue();
						value.i = s.readInt();
						value.d = s.readDouble();
						value.s = s.readString();
						return value;
					}
				});
			}else {
				throw e1;
			}
		}
		QMetaType.registerDataStreamOperators(new QMetaType.DataStreamInFn<MetaValue2>(){
													private static final long serialVersionUID = 1L;
													@Override
													public void accept(QDataStream s, MetaValue2 value) {
														s.writeInt(value.i);
														s.writeDouble(value.d);
														s.writeString(value.s);
														s.writeFloat(value.f);
													}
												},
												new QMetaType.DataStreamOutFn<MetaValue2>(){
													private static final long serialVersionUID = 1L;
													@Override
													public MetaValue2 apply(QDataStream s) {
														MetaValue2 value = new MetaValue2();
														value.i = s.readInt();
														value.d = s.readDouble();
														value.s = s.readString();
														value.f = s.readFloat();
														return value;
													}
												});
    	try {
	    	QMetaType.registerDebugStreamOperator((QDebug d, MetaValue value)->{
	    		d.append("MetaValue [i=" + value.i + ", d=" + value.d + ", s=" + value.s + "]");
	    	});
	    	try {
				QMetaType.registerDebugStreamOperator((QDebug s, MetaValue value)->{});
				fail("RuntimeException expected to be thrown.");
			} catch (RuntimeException e) {
			}
		} catch (IllegalArgumentException e1) {
			if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)) {
				QMetaType.registerDebugStreamOperator(new QMetaType.DebugStreamFn<MetaValue>(){
					private static final long serialVersionUID = 1L;
					@Override
					public void accept(QDebug d, MetaValue value) {
						d.append("MetaValue [i=" + value.i + ", d=" + value.d + ", s=" + value.s + "]");
					}
				});
			}else {
				throw e1;
			}
		}
    	QMetaType.registerDebugStreamOperator(new QMetaType.DebugStreamFn<MetaValue2>(){
			private static final long serialVersionUID = 1L;
			@Override
			public void accept(QDebug d, MetaValue2 value) {
				d.append("MetaValue2 [i=" + value.i + ", d=" + value.d + ", s=" + value.s + ", f=" + value.f + "]");
			}
		});
    	QMetaType.qRegisterMetaType(SerializableMetaValue.class);
    	MetaValue mvalue = new MetaValue();
    	mvalue.i = 5;
    	mvalue.d = 9.246;
    	mvalue.s = "TEST";
    	QByteArray buffer = new QByteArray();
    	QDataStream stream = new QDataStream(buffer, QIODevice.OpenModeFlag.WriteOnly);
    	stream.writeObject(mvalue);
    	stream.dispose();
    	stream = new QDataStream(buffer, QIODevice.OpenModeFlag.ReadOnly);
    	MetaValue restored = stream.readObject(MetaValue.class);
    	stream.dispose();
    	assertTrue(restored!=null);
    	assertEquals(mvalue.i, restored.i);
    	assertEquals(mvalue.d, restored.d, 0.01);
    	assertEquals(mvalue.s, restored.s);
    	StringBuilder string = new StringBuilder();
    	try(QDebug debug = new QDebug(string)){
    		debug.nospace().append(restored);
    	}
    	assertEquals("MetaValue [i=" + mvalue.i + ", d=" + mvalue.d + ", s=" + mvalue.s + "]", string.toString());
    	
    	SerializableMetaValue svalue = new SerializableMetaValue();
    	svalue.i = 5;
    	svalue.d = 9.246;
    	svalue.s = "TEST";
    	buffer.clear();
    	stream = new QDataStream(buffer, QIODevice.OpenModeFlag.WriteOnly);
    	stream.writeObject(svalue);
    	stream.dispose();
    	stream = new QDataStream(buffer, QIODevice.OpenModeFlag.ReadOnly);
    	SerializableMetaValue srestored = stream.readObject(SerializableMetaValue.class);
    	stream.dispose();
    	assertTrue(srestored!=null);
    	assertEquals(svalue.i, srestored.i);
    	assertEquals(svalue.d, srestored.d, 0.01);
    	assertEquals(svalue.s, srestored.s);
    	string = new StringBuilder();
    	try(QDebug debug = new QDebug(string)){
    		debug.nospace().append(srestored);
    	}
    	assertEquals("SerializableMetaValue [i=" + svalue.i + ", d=" + svalue.d + ", s=" + svalue.s + "]", string.toString());
    	
    	try {
			QMetaType.registerDataStreamOperators((QDataStream s, QRect value)->{}, s->(QRect)null);
			fail("IllegalArgumentException expected to be thrown.");
		} catch (IllegalArgumentException e) {
		}
    	try {
			QMetaType.registerDataStreamOperators((QDataStream s, QWindow value)->{}, s->(QWindow)null);
			fail("IllegalArgumentException expected to be thrown.");
		} catch (IllegalArgumentException e) {
		}
    	try {
			QMetaType.registerDataStreamOperators((QDataStream s, QCoreApplication value)->{}, s->(QCoreApplication)null);
			fail("IllegalArgumentException expected to be thrown.");
		} catch (IllegalArgumentException e) {
		}
    	try {
			QMetaType.registerDataStreamOperators((QDataStream s, MyObject value)->{}, s->(MyObject)null);
			fail("IllegalArgumentException expected to be thrown.");
		} catch (IllegalArgumentException e) {
		}
    	try {
			QMetaType.registerDataStreamOperators((QDataStream s, int[] value)->{}, s->(int[])null);
			fail("IllegalArgumentException expected to be thrown.");
		} catch (IllegalArgumentException e) {
		}
    	try {
    		@SuppressWarnings({ "unchecked", "rawtypes" })
			QMetaType.DataStreamInFn<Object> infn = (QMetaType.DataStreamInFn)(QMetaType.DataStreamInFn<T1>)(QDataStream s, T1 value)->{};
    		@SuppressWarnings({ "unchecked", "rawtypes" })
			QMetaType.DataStreamOutFn<Object> outfn = (QMetaType.DataStreamOutFn)(QMetaType.DataStreamOutFn<T2>)s->(T2)null;
			QMetaType.registerDataStreamOperators(infn, outfn);
			fail("IllegalArgumentException expected to be thrown.");
		} catch (IllegalArgumentException e) {
		}
    }
	
	@Test
    public void testMetaTypeMetaObject() {
		Assume.assumeTrue("Qt version >= 6.0", QLibraryInfo.version().compareTo(new QVersionNumber(6,0))>=0);
		QMetaObject mo = QMetaObject.forType(MyObject.class);
		QMetaMethod mtd = mo.method("subObject");
		QMetaType returnMetaType = new QMetaType(mtd.returnType());
		assertEquals(SubObject.class, returnMetaType.javaType());
		QMetaObject rmo = returnMetaType.metaObject();
		assertEquals(QMetaObject.forType(SubObject.class), rmo);
	}
	
	@Test
    public void testMetaTypeOfJavaMap() {
		QMetaType hashType = QMetaType.fromType(QHash.class, QMetaType.fromType(ThreadGroup.class), QMetaType.fromType(String.class));
		assertTrue(hashType.isValid());
		new QHash<>(ThreadGroup.class, String.class);
	}
	
	@Test
    public void testScopedPointer() {
		QMetaType smartPointerType = QMetaType.fromType(QScopedPointer.class, QMetaType.fromType(QObject.class));
		assertFalse(""+smartPointerType.name(), smartPointerType.isValid());
	}
	
	@Test
    public void testScopedArrayPointer() {
		QMetaType smartPointerType = QMetaType.fromType(QScopedArrayPointer.class, QMetaType.fromType(QSize.class));
		assertFalse(""+smartPointerType.name(), smartPointerType.isValid());
	}
	
	@Test
    public void testQPointer() {
		QMetaType smartPointerType = QMetaType.fromType(QPointer.class, QMetaType.fromType(QObject.class));
		assertTrue(smartPointerType.isValid());
		assertEquals(null, smartPointerType.create());
		QObject qobj = new QObject();
		QVariant variant = TestQVariant.createVariant(smartPointerType, qobj);
		assertEquals(qobj, variant.value());
		smartPointerType = QMetaType.fromType(QPointer.class, QMetaType.fromType(QSize.class));
		assertFalse(smartPointerType.isValid());
		assertEquals(null, smartPointerType.create());
		smartPointerType = QMetaType.fromType(QPointer.class, QMetaType.fromType(QGraphicsItem.class));
		assertFalse(smartPointerType.isValid());
		assertEquals(null, smartPointerType.create());
	}
	
	@Test
    public void testSharedPointer() {
		QMetaType smartPointerType = QMetaType.fromType(QSharedPointer.class, QMetaType.fromType(QObject.class));
		assertTrue(smartPointerType.isValid());
		assertEquals(null, smartPointerType.create());
		QObject qobj = new QObject();
		QVariant variant = TestQVariant.createVariant(smartPointerType, qobj);
		assertEquals(qobj, variant.value());
		smartPointerType = QMetaType.fromType(QSharedPointer.class, QMetaType.fromType(QSize.class));
		assertTrue(smartPointerType.isValid());
		assertEquals(null, smartPointerType.create());
		QSize size = new QSize(1,2);
		variant.dispose();
		variant = TestQVariant.createVariant(smartPointerType, size);
		assertTrue(size==variant.value());
		assertEquals(size, variant.value(QSize.class));
		assertTrue(variant.convert(QSize.class));
		assertEquals(size, variant.value());
		assertFalse(size==variant.value());
		variant.dispose();
		smartPointerType = QMetaType.fromType(QSharedPointer.class, QMetaType.fromType(QGraphicsItem.class));
		assertTrue(smartPointerType.isValid());
		assertEquals(null, smartPointerType.create());
		QGraphicsItem item = new QGraphicsPathItem();
		variant.dispose();
		variant = TestQVariant.createVariant(smartPointerType, item);
		assertTrue(item==variant.value());
		assertTrue(item==variant.value(QGraphicsItem.class));
		smartPointerType = QMetaType.fromType(QSharedPointer.class, QMetaType.fromType(int.class));
		assertTrue(smartPointerType.isValid());
		assertEquals(null, smartPointerType.create());
		variant.dispose();
		variant = TestQVariant.createVariant(smartPointerType, 5);
		assertEquals(5, variant.value());
		smartPointerType = QMetaType.fromType(QSharedPointer.class, QMetaType.fromType(String.class));
		assertTrue(smartPointerType.isValid());
		assertEquals(null, smartPointerType.create());
		variant.dispose();
		variant = TestQVariant.createVariant(smartPointerType, "Test");
		assertEquals("Test", ""+variant.value());
		QString qstring = new QString("Test2");
		variant.dispose();
		variant = TestQVariant.createVariant(smartPointerType, qstring);
		assertEquals(qstring, variant.value());
		assertTrue(qstring==variant.value());
		assertEquals(qstring, variant.value(QString.class));
		variant.dispose();
		smartPointerType = QMetaType.fromType(QSharedPointer.class, QMetaType.fromType(QVariant.class));
		assertTrue(smartPointerType.isValid());
		assertEquals(null, smartPointerType.create());
		variant.dispose();
		variant = TestQVariant.createVariant(smartPointerType, "Test3");
		assertEquals(new QVariant("Test3"), variant.value());
		variant.dispose();
		Object javaObject = new QObject();
		variant = TestQVariant.createVariant(smartPointerType, javaObject);
		assertEquals(javaObject, ((QVariant)variant.value()).value());
		variant.dispose();
		qstring = new QString("Test4");
		variant = TestQVariant.createVariant(smartPointerType, qstring);
		assertEquals(qstring, ((QVariant)variant.value()).value());
		assertFalse(qstring==((QVariant)variant.value()).value());
		assertTrue(variant.value(QVariant.class)!=null);
		variant.dispose();
	}
	
	@Test
    public void testWeakPointer() {
		QMetaType smartPointerType = QMetaType.fromType(QWeakPointer.class, QMetaType.fromType(QObject.class));
		assertTrue(smartPointerType.isValid());
		assertEquals(null, smartPointerType.create());
		QObject qobj = new QObject();
		QVariant variant = TestQVariant.createVariant(smartPointerType, qobj);
		assertEquals(qobj, variant.value());
		smartPointerType = QMetaType.fromType(QSharedPointer.class, QMetaType.fromType(QSize.class));
		assertTrue(smartPointerType.isValid());
		assertEquals(null, smartPointerType.create());
		QSize size = new QSize(1,2);
		variant.dispose();
		variant = TestQVariant.createVariant(smartPointerType, size);
		assertTrue(size==variant.value());
		assertEquals(size, variant.value(QSize.class));
		assertTrue(variant.convert(QSize.class));
		assertEquals(size, variant.value());
		assertFalse(size==variant.value());
		variant.dispose();
		smartPointerType = QMetaType.fromType(QSharedPointer.class, QMetaType.fromType(QGraphicsItem.class));
		assertTrue(smartPointerType.isValid());
		assertEquals(null, smartPointerType.create());
		QGraphicsItem item = new QGraphicsPathItem();
		variant.dispose();
		variant = TestQVariant.createVariant(smartPointerType, item);
		assertTrue(item==variant.value());
		assertTrue(item==variant.value(QGraphicsItem.class));
		smartPointerType = QMetaType.fromType(QSharedPointer.class, QMetaType.fromType(int.class));
		assertTrue(smartPointerType.isValid());
		assertEquals(null, smartPointerType.create());
		variant.dispose();
		variant = TestQVariant.createVariant(smartPointerType, 5);
		assertEquals(5, variant.value());
		smartPointerType = QMetaType.fromType(QSharedPointer.class, QMetaType.fromType(String.class));
		assertTrue(smartPointerType.isValid());
		assertEquals(null, smartPointerType.create());
		variant.dispose();
		variant = TestQVariant.createVariant(smartPointerType, "Test");
		assertEquals("Test", ""+variant.value());
		QString qstring = new QString("Test2");
		variant.dispose();
		variant = TestQVariant.createVariant(smartPointerType, qstring);
		assertEquals(qstring, variant.value());
		assertTrue(qstring==variant.value());
		assertEquals(qstring, variant.value(QString.class));
		variant.dispose();
		smartPointerType = QMetaType.fromType(QSharedPointer.class, QMetaType.fromType(QVariant.class));
		assertTrue(smartPointerType.isValid());
		assertEquals(null, smartPointerType.create());
		variant.dispose();
		variant = TestQVariant.createVariant(smartPointerType, "Test3");
		assertEquals(new QVariant("Test3"), variant.value());
		variant.dispose();
		Object javaObject = new QObject();
		variant = TestQVariant.createVariant(smartPointerType, javaObject);
		assertEquals(javaObject, ((QVariant)variant.value()).value());
		variant.dispose();
		qstring = new QString("Test4");
		variant = TestQVariant.createVariant(smartPointerType, qstring);
		assertEquals(qstring, ((QVariant)variant.value()).value());
		assertFalse(qstring==((QVariant)variant.value()).value());
		assertTrue(variant.value(QVariant.class)!=null);
		variant.dispose();
	}
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestMetaType.class.getName());
    }
	
	static class SubObject extends QObject{
	}
	
	static class MyObject extends QObject{
		public SubObject subObject(){return null;}
	}
	
	static class MetaValue{
		int i;
		double d;
		String s;
	}
	
	static class MetaValue2{
		int i;
		double d;
		String s;
		float f;
	}
	
	static class SerializableMetaValue implements Serializable{
		private static final long serialVersionUID = -4765490824878229108L;
		int i;
		double d;
		String s;
		
		@Override
		public String toString() {
			return "SerializableMetaValue [i=" + i + ", d=" + d + ", s=" + s + "]";
		}
	}
	
	static class T1{
	}
	
	static class T2{
	}
}