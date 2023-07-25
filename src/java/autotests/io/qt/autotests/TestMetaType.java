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
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import java.io.File;
import java.io.Serializable;
import java.net.URL;
import java.net.URLClassLoader;

import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtUtilities;
import io.qt.autotests.generated.General;
import io.qt.core.QByteArray;
import io.qt.core.QCoreApplication;
import io.qt.core.QDataStream;
import io.qt.core.QDebug;
import io.qt.core.QIODevice;
import io.qt.core.QLibraryInfo;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaProperty;
import io.qt.core.QMetaType;
import io.qt.core.QObject;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QRect;
import io.qt.core.QString;
import io.qt.core.QVariant;
import io.qt.core.QVersionNumber;
import io.qt.core.Qt;
import io.qt.gui.QWindow;
import io.qt.widgets.QMdiSubWindow;

public class TestMetaType extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
		testInitializeWithWidgets();
    }
	
	@Test
	public void testFromClassLoader() throws Exception {
		Assume.assumeTrue("Qt version >= 6.0", QLibraryInfo.version().compareTo(new QVersionNumber(6,0))>=0);
		Assume.assumeTrue("no qtjambi.antcontrib set", System.getProperties().containsKey("qtjambi.antcontrib"));
		try(URLClassLoader cl = new URLClassLoader(new URL[]{new File(System.getProperty("qtjambi.antcontrib", ".")).toURI().toURL()})){
			Class<?> cls2 = cl.loadClass("org.apache.tools.ant.taskdefs.Echo");
			QtUtilities.usePackageContentAsGadgets(cls2.getPackage().getName());
			QtUtilities.usePackageContentAsGadgets("org.apache.tools.ant");
			QMetaObject mo2 = QMetaObject.forType(cls2);
			QMetaProperty location = mo2.property("location");
			assertEquals("org::apache::tools::ant::Location*", location.typeName());
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