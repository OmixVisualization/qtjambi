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

//import static io.qt.core.QMetaType.*;
import static io.qt.qml.QtQml.qmlClearTypeRegistrations;
import static io.qt.qml.QtQml.qmlRegisterType;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.Test;

import io.qt.QtAsGadget;
import io.qt.QtEnumerator;
import io.qt.QtPropertyReader;
import io.qt.QtPropertyWriter;
import io.qt.core.QByteArray;
import io.qt.core.QCoreApplication;
import io.qt.core.QIntProperty;
import io.qt.core.QLibraryInfo;
import io.qt.core.QList;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaType;
import io.qt.core.QObject;
import io.qt.core.QRectF;
import io.qt.core.QVariant;
import io.qt.core.QVersionNumber;
import io.qt.gui.QVector4D;
import io.qt.qml.QJSEngine;
import io.qt.qml.QJSManagedValue;
import io.qt.qml.QJSValue;
import io.qt.qml.QQmlComponent;
import io.qt.qml.QQmlEngine;
import io.qt.qml.QQmlError;
import io.qt.qml.QtQml;
import io.qt.qml.util.QmlNamedElement;
import io.qt.qml.util.QmlSingleton;
import io.qt.qml.util.QmlTypeRegistrationException;
import io.qt.qml.util.QmlTypes;

public class TestQmlQt6 extends ApplicationInitializer{
	
	@Test
    public void testJSValue() {
		QJSEngine engine = new QJSEngine();
		QJSValue value = engine.toScriptValue(5.4);
		Assert.assertTrue(value.isNumber());
		Assert.assertEquals(0.000001, 5.4, value.toDouble());
		value = engine.toScriptValue(QCoreApplication.instance());
		Assert.assertTrue(value.isObject());
		QObject result = engine.fromScriptValue(value, QObject.class);
		Assert.assertEquals(QCoreApplication.instance(), result);
	}
	
	@QmlNamedElement(name="Singleton")
	@QmlSingleton
	static class Singleton extends QObject{
		public final QProperty<String> text = new QProperty<>("Hello, world!");
		
		private int m_number = 8;

		public void setNumber(int number) {
			if(this.m_number != number) {
				this.m_number = number;
				this.number.notifyProperty();
			}
		}
		public final QComputedIntProperty number = new QComputedIntProperty(()->this.m_number);
	}
	
	@QmlNamedElement(name="NonQObjectSingleton")
	@QmlSingleton
	@QtAsGadget
	static class Singleton3{
	}
	
	@Test
    public void run_testSingletonInstance() {
		QtQml.qmlClearTypeRegistrations();
		Singleton s = new Singleton();
		int id = QtQml.qmlRegisterSingletonInstance("io.qt.test", 1, 0, "Singleton", s);
		Assert.assertTrue(id!=-1);
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"import QtQuick 2.0\n" +
				"Item{\n" + 
				"    property string text: Singleton.text\n" + 
				"    property int number: Singleton.number\n" + 
				"}");
		QQmlEngine engine = new QQmlEngine();
		QQmlComponent component = new QQmlComponent(engine);
		component.setData(data, null);
		Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
		Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
		QObject root = component.create();
		Assert.assertEquals("Hello, world!", root.property("text"));
		s.text.setValue("Hello, world?");
		Assert.assertEquals("Hello, world?", root.property("text"));
		QIntProperty iprop = new QIntProperty();
		iprop.setBinding(()->s.number.value());
		Assert.assertEquals(8, iprop.value());
		Assert.assertEquals(8, root.property("number"));
		s.setNumber(29);
		Assert.assertEquals(29, iprop.value());
		Assert.assertEquals(29, root.property("number"));
	}
	
	@Test
    public void run_testSingletonType() {
		QtQml.qmlClearTypeRegistrations();
		int id = QmlTypes.registerType(Singleton.class, "io.qt.test", 1);
		Assert.assertTrue(id!=-1);
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"import QtQuick 2.0\n" +
				"Item{\n" + 
				"    property string text: Singleton.text\n" + 
				"    property int number: Singleton.number\n" + 
				"}");
		QQmlEngine engine = new QQmlEngine();
		QQmlComponent component = new QQmlComponent(engine);
		component.setData(data, null);
		Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
		Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
		QObject root = component.create();
		Singleton s = engine.singletonInstance(Singleton.class, id);
		Assert.assertEquals("Hello, world!", root.property("text"));
		s.text.setValue("Hello, world?");
		Assert.assertEquals("Hello, world?", root.property("text"));
		QIntProperty iprop = new QIntProperty();
		iprop.setBinding(()->s.number.value());
		Assert.assertEquals(8, iprop.value());
		Assert.assertEquals(8, root.property("number"));
		s.setNumber(29);
		Assert.assertEquals(29, iprop.value());
		Assert.assertEquals(29, root.property("number"));
	}
	
	@Test
    public void run_testSingletonTypeNonQObject() {
		QtQml.qmlClearTypeRegistrations();
		try{
			QmlTypes.registerType(Singleton3.class, "io.qt.test", 1);
			Assert.fail("QmlTypeRegistrationException expected");
		} catch (QmlTypeRegistrationException e) {
		}
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	@Test
    public void run_testValueType() {
		Assume.assumeTrue("Qt version >= 6.4", QLibraryInfo.version().compareTo(new QVersionNumber(6,4))>=0);
		QtQml.qmlClearTypeRegistrations();
		int id = QtQml.qmlRegisterType((Class)CloneableMetaValue.class, "io.qt.test", 1, 0, "mval");
		Assert.assertTrue(id!=-1);
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"import QtQuick 2.0\n" +
				"Item{\n" + 
				"    property mval customValue;\n" + 
				"    property list<mval> customValues;\n" + 
				"}");
		QQmlEngine engine = new QQmlEngine();
		QQmlComponent component = new QQmlComponent(engine);
		component.setData(data, null);
		Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
		Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
		QObject root = component.create();
		Object customValue = root.property("customValue");
		Assert.assertTrue(customValue instanceof CloneableMetaValue);
		Object customValues = root.property("customValues");
		Assert.assertTrue(customValues instanceof QList);
		QList list = (QList)customValues;
		Assert.assertEquals(0, list.size());
		try {
			list.add("");
			Assert.fail("IllegalArgumentException expected");
		} catch (IllegalArgumentException e) {
		}
		list.add(new CloneableMetaValue());
	}
	
	static class CustomGadgetType
	{
		public CustomGadgetType() {}
		private QRectF rect = new QRectF();
		
		@QtPropertyReader(name="rect")
		public QRectF getRect() {
			return rect;
		}
		@QtPropertyWriter(name="rect")
		public void setRect(QRectF rect) {
			this.rect = rect;
		}
	}
	
	static class CustomValueType implements Cloneable
	{
		public CustomValueType(QJSValue arguments) {
			if(arguments.isArray()) {
				int length = arguments.property("length").toInt();
				if(length==3) {
					i = arguments.property(0).toInt();
					d = arguments.property(1).toDouble();
					s = arguments.property(2).toString();
				}
			}else if(arguments.isObject()) {
				i = arguments.property("i").toInt();
				d = arguments.property("d").toDouble();
				s = arguments.property("s").toString();
			}
		}
		
		public CustomValueType() {}
		
		public CustomValueType(int i, double d, String s) {
			this.i = i;
			this.d = d;
			this.s = s;
		}
		
		CustomValueType(CustomValueType other) {
			this.i = other.i;
			this.d = other.d;
			this.s = other.s;
		}
		private int i;
		private double d;
		private String s = "";
		
		@QtPropertyReader(name="self")
		public CustomValueType self() {
			return this;
		}
		
		@QtPropertyReader(name="i")
		public int getI() {
			return i;
		}
		@QtPropertyWriter(name="i")
		public void setI(int i) {
			this.i = i;
		}
		@QtPropertyReader(name="d")
		public double getD() {
			return d;
		}
		@QtPropertyWriter(name="d")
		public void setD(double d) {
			this.d = d;
		}
		@QtPropertyReader(name="s")
		public String getS() {
			return s;
		}
		@QtPropertyWriter(name="s")
		public void setS(String s) {
			this.s = s;
		}
		@Override
		public int hashCode() {
			final int prime = 31;
			int result = 1;
			long temp;
			temp = Double.doubleToLongBits(d);
			result = prime * result + (int) (temp ^ (temp >>> 32));
			result = prime * result + i;
			result = prime * result + ((s == null) ? 0 : s.hashCode());
			return result;
		}
		@Override
		public boolean equals(Object obj) {
			if (this == obj)
				return true;
			if (obj == null)
				return false;
			if (getClass() != obj.getClass())
				return false;
			CustomValueType other = (CustomValueType) obj;
			if (Double.doubleToLongBits(d) != Double.doubleToLongBits(other.d))
				return false;
			if (i != other.i)
				return false;
			if (s == null) {
				if (other.s != null)
					return false;
			} else if (!s.equals(other.s))
				return false;
			return true;
		}
		@Override
		public String toString() {
			return "CustomValueType [i=" + i + ", d=" + d + ", s=" + s + "]";
		}
		
		@Override
		public CustomValueType clone(){
			return new CustomValueType(this);
		}
	};
	
	static class ObjectType extends QObject{
		public ObjectType() {
			super();
		}
		public ObjectType(QDeclarativeConstructor constructor) {
			super(constructor);
		}
		public ObjectType(QObject parent) {
			super(parent);
		}
		
		public enum SomeEnum implements QtEnumerator{A,B,C,D};
	    public final Signal0 vectorChanged = new Signal0();
	    public final Signal0 someEnumChanged = new Signal0();
	    public final Signal0 customValueTypeChanged = new Signal0();
	    
	    private QVector4D vector = new QVector4D();
	    private SomeEnum someEnum = SomeEnum.A;
	    private CustomValueType customValueType = new CustomValueType();
	    private CustomGadgetType customGadgetType = new CustomGadgetType();
	    private java.lang.Throwable throwable;
	    
		public QVector4D getVector() {
			return vector;
		}
		public void setVector(QVector4D vector) {
			this.vector = vector;
			vectorChanged.emit();
		}
		public SomeEnum getSomeEnum() {
			return someEnum;
		}
		public void setSomeEnum(SomeEnum someEnum) {
			this.someEnum = someEnum;
			someEnumChanged.emit();
		}
		public CustomValueType getCustomValueType() {
			return customValueType;
		}
		public void setCustomValueType(CustomValueType customValueType) {
			if(customValueType==null)
				customValueType = new CustomValueType();
			this.customValueType = customValueType;
			customValueTypeChanged.emit();
		}

		public CustomGadgetType getCustomGadgetType() {
			return customGadgetType;
		}

		public void setCustomGadgetType(CustomGadgetType customGadgetType) {
			this.customGadgetType = customGadgetType;
		}
		public java.lang.Throwable getThrowable() {
			return throwable;
		}
		public void setThrowable(java.lang.Throwable throwable) {
			this.throwable = throwable;
		}
	}
	
	@SuppressWarnings("rawtypes")
	@Test
    public void testCustomTypes() {
		qmlClearTypeRegistrations();
	    qmlRegisterType(ObjectType.class, "io.qt.test", 1, 0, "ObjectType");
	    qmlRegisterType((Class)CustomValueType.class, "io.qt.test", 1, 0, "customValueType");
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"import QtQuick 2.0\n" +
				"Item{\n" + 
				"    property ObjectType object: ObjectType{\n" + 
				"        vector: Qt.vector4d(1,2,3,4)\n" + 
				"        someEnum: ObjectType.D\n" + 
				"        customValueType.i: 5\n" + 
				"        customValueType.d: 7.9\n" + 
				"        customValueType.s: \"TEST\"\n" + 
				"    }\n" + 
				"    property int someEnum: object.someEnum\n" + 
				"    function changeObject(){\n" + 
				"        object.someEnum = ObjectType.B;\n" + 
				"        object.customValueType.i = 6;\n" + 
				"        object.customValueType.d = 3.9;\n" + 
				"        object.customValueType.s = \"TEST2\";\n" + 
				"    }\n" + 
				"    function changeObject2(){\n" + 
				"        object.someEnum = ObjectType.C;\n" + 
				"        var v = [3, 9.6, \"TEST8\"];\n" + 
				"        let v2 = {array: v};\n" + 
				"        console.log(v2);\n" + 
				"        object.customValueType = v;\n" + 
				"    }\n" + 
				"}");
		QQmlEngine engine = new QQmlEngine();
		engine.setOutputWarningsToStandardError(true);
		engine.warnings.connect(warnings->{
			for(QQmlError warning : warnings) {
				System.out.println(warning);//.messageType()+" "+warning.line()+" "+warning.description());
			}
		});
		
		QJSValue jsv = engine.newArray(3);
		jsv.setProperty(0, 5);
		jsv.setProperty(1, 5.6);
		jsv.setProperty(2, "CREATE_TEST");
		CustomValueType v = engine.fromManagedValue(new QJSManagedValue(jsv, engine), CustomValueType.class);
		Assert.assertEquals(5, v.i);
		Assert.assertEquals(5.6, v.d, 0.0001);
		Assert.assertEquals("CREATE_TEST", v.s);
		
		jsv = engine.newObject();
		jsv.setProperty("i", 25);
		jsv.setProperty("d", 25.6);
		jsv.setProperty("s", "CREATE_TEST2");
		v = engine.fromManagedValue(new QJSManagedValue(jsv, engine), CustomValueType.class);
		Assert.assertEquals(25, v.i);
		Assert.assertEquals(25.6, v.d, 0.0001);
		Assert.assertEquals("CREATE_TEST2", v.s);
		v = QVariant.fromValue(jsv).value(CustomValueType.class);
		Assert.assertEquals(25, v.i);
		Assert.assertEquals(25.6, v.d, 0.0001);
		Assert.assertEquals("CREATE_TEST2", v.s);
		
		QQmlComponent component = new QQmlComponent(engine);
		component.setData(data, null);
		Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
		Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
		QObject root = component.create();
		ObjectType object = (ObjectType)root.property("object");
		Assert.assertEquals(new QVector4D(1,2,3,4), object.vector);
		Assert.assertEquals(ObjectType.SomeEnum.D, object.someEnum);
		ObjectType.SomeEnum someEnum = QVariant.convert(root.property("someEnum"), ObjectType.SomeEnum.class);
		Assert.assertEquals(ObjectType.SomeEnum.D, someEnum);
		Assert.assertEquals(5, object.customValueType.i);
		Assert.assertEquals(7.9, object.customValueType.d, 0.0001);
		Assert.assertEquals("TEST", object.customValueType.s);
		
	    QMetaObject mo = root.metaObject();
	    QMetaMethod changeObject = mo.method("changeObject");
	    changeObject.invoke(root);
		Assert.assertEquals(ObjectType.SomeEnum.B, object.someEnum);
		someEnum = QVariant.convert(root.property("someEnum"), ObjectType.SomeEnum.class);
		Assert.assertEquals(ObjectType.SomeEnum.B, someEnum);
		Assert.assertEquals(6, object.customValueType.i);
		Assert.assertEquals(3.9, object.customValueType.d, 0.0001);
		Assert.assertEquals("TEST2", object.customValueType.s);
		
		changeObject = mo.method("changeObject2");
	    changeObject.invoke(root);
		Assert.assertEquals(ObjectType.SomeEnum.C, object.someEnum);
		someEnum = QVariant.convert(root.property("someEnum"), ObjectType.SomeEnum.class);
		Assert.assertEquals(ObjectType.SomeEnum.C, someEnum);
		Assert.assertEquals(3, object.customValueType.i);
		Assert.assertEquals(9.6, object.customValueType.d, 0.0001);
		Assert.assertEquals("TEST8", object.customValueType.s);
		
		Assert.assertEquals(QMetaType.fromType(CustomGadgetType.class), QVariant.fromValue(new CustomGadgetType()).metaType());
		Assert.assertEquals(QMetaType.fromType(CustomValueType.class), QVariant.fromValue(new CustomValueType()).metaType());
		Assert.assertEquals(QMetaType.fromName("JObjectWrapper"), QVariant.fromValue(new ObjectType()).metaType());
//		QMetaObject.forType(ObjectType.class).properties().forEach(p->System.out.println(p.typeName()+" "+p.name()));
//		System.out.println(QMetaType.fromType(java.lang.Runnable.class).flags());
//		System.out.println(QMetaObject.forType(Runnable.class).methods());
//		System.out.println(QMetaObject.forType(Runnable.class).properties());
	}
	
	@SuppressWarnings("rawtypes")
	@Test
    public void testCustomTypes2() {
		Assume.assumeTrue("Qt version >= 6.4", QLibraryInfo.version().compareTo(new QVersionNumber(6,4))>=0);
		qmlClearTypeRegistrations();
	    qmlRegisterType(ObjectType.class, "io.qt.test", 1, 0, "ObjectType");
	    qmlRegisterType((Class)CustomValueType.class, "io.qt.test", 1, 0, "customValueType");
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"import QtQuick 2.0\n" +
				"Item{\n" + 
				"    property customValueType someValueType: [8, 2.1, \"TEST3\"]\n" + 
				"}");
		QQmlEngine engine = new QQmlEngine();
		engine.setOutputWarningsToStandardError(true);
		engine.warnings.connect(warnings->{
			for(QQmlError warning : warnings) {
				System.out.println(warning);//.messageType()+" "+warning.line()+" "+warning.description());
			}
		});
		
		QQmlComponent component = new QQmlComponent(engine);
		component.setData(data, null);
		Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
		Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
		QObject root = component.create();
		
		CustomValueType someValueType = QVariant.convert(root.property("someValueType"), CustomValueType.class);
		Assert.assertEquals(8, someValueType.i);
		Assert.assertEquals(2.1, someValueType.d, 0.0001);
		Assert.assertEquals("TEST3", someValueType.s);
    }
}
