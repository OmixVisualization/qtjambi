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

//import static io.qt.core.QMetaType.*;
import static io.qt.qml.QtQml.qmlClearTypeRegistrations;
import static io.qt.qml.QtQml.qmlRegisterType;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Objects;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.Test;

import io.qt.Nullable;
import io.qt.QtAsGadget;
import io.qt.QtEnumerator;
import io.qt.QtPropertyConstant;
import io.qt.QtPropertyReader;
import io.qt.QtPropertyStored;
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
import io.qt.core.QUrl;
import io.qt.core.QVariant;
import io.qt.core.QVersionNumber;
import io.qt.gui.QVector4D;
import io.qt.qml.QJSEngine;
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
		try {
			QJSValue value = engine.toScriptValue(5.4);
			Assert.assertTrue(value.isNumber());
			Assert.assertEquals(0.000001, 5.4, value.toNumber());
			value = engine.toScriptValue(QCoreApplication.instance());
			Assert.assertTrue(value.isObject());
			QObject result = engine.fromScriptValue(value, QObject.class);
			Assert.assertEquals(QCoreApplication.instance(), result);
		}finally {
			engine.dispose();
		}
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
		
		public int getIntValue() {
			return 0;
		}
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
		try {
			QQmlComponent component = new QQmlComponent(engine);
			component.setData(data, (QUrl)null);
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
		}finally {
			engine.dispose();
		}
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
		try {
			QQmlComponent component = new QQmlComponent(engine);
			component.setData(data, (QUrl)null);
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
		}finally {
			engine.dispose();
		}
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
		try {
			QQmlComponent component = new QQmlComponent(engine);
			component.setData(data, (QUrl)null);
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
		}finally {
			engine.dispose();
		}
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
	
	@QtAsGadget
	static class AutoGadgetType
	{
		public AutoGadgetType() {}
		private QRectF rect = new QRectF();
		
		public QRectF getRect() {
			return rect;
		}
		public void setRect(QRectF rect) {
			this.rect = rect;
		}
	}
	
	static class CustomValueType implements Cloneable
	{
		public static final List<WeakReference<CustomValueType>> instances = Collections.synchronizedList(new ArrayList<>());
		
		public CustomValueType(QJSValue arguments) {
			instances.add(new WeakReference<>(this));
			if(arguments.isArray()) {
				int length = arguments.property("length").toInt();
				if(length==3) {
					i = arguments.property(0).toInt();
					d = arguments.property(1).toNumber();
					s = arguments.property(2).toString();
				}
			}else if(arguments.isObject()) {
				i = arguments.property("i").toInt();
				d = arguments.property("d").toNumber();
				s = arguments.property("s").toString();
			}
		}
		
		public CustomValueType() {
			instances.add(new WeakReference<>(this));
		}
		
		public CustomValueType(int i, double d, String s) {
			instances.add(new WeakReference<>(this));
			this.i = i;
			this.d = d;
			this.s = s;
		}
		
		CustomValueType(CustomValueType other) {
			instances.add(new WeakReference<>(this));
			this.i = other.i;
			this.d = other.d;
			this.s = other.s;
		}
		int i;
		double d;
		String s = "";
		
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
	
	@QtAsGadget
	static class AutoGadgetValueType implements Cloneable
	{
		public AutoGadgetValueType(QJSValue arguments) {
			if(arguments.isArray()) {
				int length = arguments.property("length").toInt();
				if(length==3) {
					i = arguments.property(0).toInt();
					d = arguments.property(1).toNumber();
					s = arguments.property(2).toString();
				}
			}else if(arguments.isObject()) {
				i = arguments.property("i").toInt();
				d = arguments.property("d").toNumber();
				s = arguments.property("s").toString();
			}
		}
		
		public AutoGadgetValueType() {}
		
		public AutoGadgetValueType(int i, double d, String s) {
			this.i = i;
			this.d = d;
			this.s = s;
		}
		
		AutoGadgetValueType(AutoGadgetValueType other) {
			this.i = other.i;
			this.d = other.d;
			this.s = other.s;
		}
		int i;
		public double d;
		String s = "";
		
		@QtPropertyReader
		public AutoGadgetValueType self() {
			return this;
		}
		
		public int getI() {
			return i;
		}
		public void setI(int i) {
			this.i = i;
		}
		public String getS() {
			return s;
		}
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
			AutoGadgetValueType other = (AutoGadgetValueType) obj;
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
			return "AutoGadgetValueType [i=" + i + ", d=" + d + ", s=" + s + "]";
		}
		
		@Override
		public AutoGadgetValueType clone(){
			return new AutoGadgetValueType(this);
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
	    
	    QVector4D vector = new QVector4D();
	    SomeEnum someEnum = SomeEnum.A;
	    CustomValueType customValueType = new CustomValueType();
	    CustomGadgetType customGadgetType = new CustomGadgetType();
	    AutoGadgetValueType autoGadgetValueType = new AutoGadgetValueType();
	    AutoGadgetType autoGadgetType = new AutoGadgetType();
	    boolean isNull;
	    java.lang.Throwable throwable;
	    
	    public CustomGadgetType supplyNullCustomGadgetType() {
	    	return null;
	    }
	    
	    public AutoGadgetType supplyNullAutoGadgetType() {
	    	return null;
	    }
	    
	    public boolean checkNull(Object object) {
	    	return Objects.isNull(object);
	    }
	    
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
		public AutoGadgetValueType getAutoGadgetValueType() {
			return autoGadgetValueType;
		}
		public void setAutoGadgetValueType(AutoGadgetValueType autoGadgetValueType) {
			this.autoGadgetValueType = autoGadgetValueType;
		}
		public AutoGadgetType getAutoGadgetType() {
			return autoGadgetType;
		}
		public void setAutoGadgetType(AutoGadgetType autoGadgetType) {
			this.autoGadgetType = autoGadgetType;
		}
		public boolean getIsNull() {
			return isNull;
		}
		public void setIsNull(boolean isNull) {
			this.isNull = isNull;
		}
	}
	
	@SuppressWarnings("rawtypes")
	@Test
    public void testCustomTypes() {
		qmlClearTypeRegistrations();
//		QLogging.qInstallMessageHandler((t,c,m)->{System.out.println(m);});
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
				"        var v = object.supplyNullCustomGadgetType();\n" + 
				"        object.isNull = object.checkNull(v);\n" +
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
		try {
			engine.setOutputWarningsToStandardError(true);
			engine.warnings.connect(warnings->{
				for(QQmlError warning : warnings) {
					System.out.println(warning);//.messageType()+" "+warning.line()+" "+warning.description());
				}
			});
			
			QQmlComponent component = new QQmlComponent(engine);
			component.setData(data, (QUrl)null);
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
		    object.setIsNull(false);
		    QMetaMethod changeObject = mo.method("changeObject");
		    changeObject.invoke(root);
			Assert.assertEquals(ObjectType.SomeEnum.B, object.someEnum);
			someEnum = QVariant.convert(root.property("someEnum"), ObjectType.SomeEnum.class);
			Assert.assertEquals(ObjectType.SomeEnum.B, someEnum);
			Assert.assertEquals(6, object.customValueType.i);
			Assert.assertEquals(3.9, object.customValueType.d, 0.0001);
			Assert.assertEquals("TEST2", object.customValueType.s);
			Assert.assertTrue(object.getIsNull());
			
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
			Assert.assertTrue(QVariant.fromValue(new ObjectType()).metaType().name().startsWith("JObjectWrapper<"));
	//		QMetaObject.forType(ObjectType.class).properties().forEach(p->System.out.println(p.typeName()+" "+p.name()));
	//		System.out.println(QMetaType.fromType(java.lang.Runnable.class).flags());
	//		System.out.println(QMetaObject.forType(Runnable.class).methods());
	//		System.out.println(QMetaObject.forType(Runnable.class).properties());
		}finally {
			engine.dispose();
		}
	}
	
	@SuppressWarnings("rawtypes")
	@Test
    public void testAutoGadgetTypes() {
		qmlClearTypeRegistrations();
		//this type needs to be available until engine is deleted otherwise crash:
	    qmlRegisterType(ObjectType.class, "io.qt.test", 1, 0, "ObjectType");
	    qmlRegisterType((Class)AutoGadgetValueType.class, "io.qt.test", 1, 0, "autoGadgetValueType");
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"import QtQuick 2.0\n" +
				"Item{\n" + 
				"    property ObjectType object: ObjectType{\n" + 
				"        vector: Qt.vector4d(1,2,3,4)\n" + 
				"        someEnum: ObjectType.D\n" + 
				"        autoGadgetValueType.i: 5\n" + 
				"        autoGadgetValueType.d: 7.9\n" + 
				"        autoGadgetValueType.s: \"TEST\"\n" + 
				"    }\n" + 
				"    property int someEnum: object.someEnum\n" + 
				"    function changeObject(){\n" + 
				"        object.someEnum = ObjectType.B;\n" + 
				"        object.autoGadgetValueType.i = 6;\n" + 
				"        object.autoGadgetValueType.d = 3.9;\n" + 
				"        object.autoGadgetValueType.s = \"TEST2\";\n" + 
				"        var v = object.supplyNullAutoGadgetType();\n" + 
				"        object.isNull = object.checkNull(v);\n" +
				"    }\n" + 
				"    function changeObject2(){\n" + 
				"        object.someEnum = ObjectType.C;\n" + 
				"        var v = [3, 9.6, \"TEST8\"];\n" + 
				"        let v2 = {array: v};\n" + 
				"        console.log(v2);\n" + 
				"        object.autoGadgetValueType = v;\n" + 
				"    }\n" + 
				"}");
		QQmlEngine engine = new QQmlEngine();
		try {
			engine.setOutputWarningsToStandardError(true);
			engine.warnings.connect(warnings->{
				for(QQmlError warning : warnings) {
					System.out.println(warning);//.messageType()+" "+warning.line()+" "+warning.description());
				}
			});
			
			QQmlComponent component = new QQmlComponent(engine);
			component.setData(data, (QUrl)null);
			Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
			Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
			QObject root = component.create();
			ObjectType object = (ObjectType)root.property("object");
			Assert.assertEquals(new QVector4D(1,2,3,4), object.vector);
			Assert.assertEquals(ObjectType.SomeEnum.D, object.someEnum);
			ObjectType.SomeEnum someEnum = QVariant.convert(root.property("someEnum"), ObjectType.SomeEnum.class);
			Assert.assertEquals(ObjectType.SomeEnum.D, someEnum);
			Assert.assertEquals(5, object.autoGadgetValueType.i);
			Assert.assertEquals(7.9, object.autoGadgetValueType.d, 0.0001);
			Assert.assertEquals("TEST", object.autoGadgetValueType.s);
			
			object.setIsNull(false);
		    QMetaObject mo = root.metaObject();
		    QMetaMethod changeObject = mo.method("changeObject");
		    changeObject.invoke(root);
			Assert.assertEquals(ObjectType.SomeEnum.B, object.someEnum);
			someEnum = QVariant.convert(root.property("someEnum"), ObjectType.SomeEnum.class);
			Assert.assertEquals(ObjectType.SomeEnum.B, someEnum);
			Assert.assertEquals(6, object.autoGadgetValueType.i);
			Assert.assertEquals(3.9, object.autoGadgetValueType.d, 0.0001);
			Assert.assertEquals("TEST2", object.autoGadgetValueType.s);
			Assert.assertTrue(object.getIsNull());
			
			changeObject = mo.method("changeObject2");
		    changeObject.invoke(root);
			Assert.assertEquals(ObjectType.SomeEnum.C, object.someEnum);
			someEnum = QVariant.convert(root.property("someEnum"), ObjectType.SomeEnum.class);
			Assert.assertEquals(ObjectType.SomeEnum.C, someEnum);
			Assert.assertEquals(3, object.autoGadgetValueType.i);
			Assert.assertEquals(9.6, object.autoGadgetValueType.d, 0.0001);
			Assert.assertEquals("TEST8", object.autoGadgetValueType.s);
			
			Assert.assertEquals(QMetaType.fromType(AutoGadgetType.class), QVariant.fromValue(new AutoGadgetType()).metaType());
			Assert.assertEquals(QMetaType.fromType(AutoGadgetValueType.class), QVariant.fromValue(new AutoGadgetValueType()).metaType());
			Assert.assertTrue(QVariant.fromValue(new ObjectType()).metaType().name().startsWith("JObjectWrapper<"));
	//		QMetaObject.forType(ObjectType.class).properties().forEach(p->System.out.println(p.typeName()+" "+p.name()));
	//		System.out.println(QMetaType.fromType(java.lang.Runnable.class).flags());
	//		System.out.println(QMetaObject.forType(Runnable.class).methods());
	//		System.out.println(QMetaObject.forType(Runnable.class).properties());
		}finally {
			engine.dispose();
		}
	}
	
	@SuppressWarnings("rawtypes")
	@Test
    public void testCustomValueType() {
		Assume.assumeTrue("Qt version >= 6.4", QLibraryInfo.version().compareTo(new QVersionNumber(6,4))>=0);
		qmlClearTypeRegistrations();
	    qmlRegisterType(ObjectType.class, "io.qt.test", 1, 0, "ObjectType");
	    qmlRegisterType((Class)CustomValueType.class, "io.qt.test", 1, 0, "customValueType");
	    CustomValueType.instances.clear();
	    
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"import QtQuick 2.0\n" +
				"Item{\n" + 
				"    property customValueType someValueType: [8, 2.1, \"TEST3\"]\n" + 
				"}");
		QQmlEngine engine = new QQmlEngine();
		try {
			engine.setOutputWarningsToStandardError(true);
			engine.warnings.connect(warnings->{
				for(QQmlError warning : warnings) {
					System.out.println(warning);//.messageType()+" "+warning.line()+" "+warning.description());
				}
			});
			
			QQmlComponent component = new QQmlComponent(engine);
			component.setData(data, (QUrl)null);
	//		Assert.assertEquals(0, CustomValueType.instances.size());
			Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
			Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
			QObject root = component.create();
	//		Assert.assertEquals(4, CustomValueType.instances.size());
			
			Object someValueTypeVar = root.property("someValueType");
	//		Assert.assertEquals(7, CustomValueType.instances.size());
			CustomValueType someValueType = QVariant.convert(someValueTypeVar, CustomValueType.class);
			Assert.assertEquals(8, someValueType.i);
			Assert.assertEquals(2.1, someValueType.d, 0.0001);
			Assert.assertEquals("TEST3", someValueType.s);
			someValueType = null;
			someValueTypeVar = null;
			for(int i=0; i<10; ++i) {
				engine.collectGarbage();
				ApplicationInitializer.runGC();
				Thread.yield();
				QCoreApplication.processEvents();
			}
			int counter=1, alife = 0;
			for(WeakReference<CustomValueType> weak : CustomValueType.instances) {
				CustomValueType value = weak.get();
				if(value!=null) {
					System.out.println(counter+": "+value);
					++alife;
				}
				++counter;
			}
			Assert.assertEquals(null, root.parent());
			Assert.assertTrue(alife<=2);
			engine.dispose();
			Assert.assertEquals(null, component.engine());
			Assert.assertFalse(component.isDisposed());
			component.dispose();
			Assert.assertEquals(null, root.parent());
			Assert.assertFalse(root.isDisposed());
			root.dispose();
			
			for(int i=0; i<10; ++i) {
				ApplicationInitializer.runGC();
				Thread.yield();
				QCoreApplication.processEvents();
			}
			counter=1;
			alife = 0;
			for(WeakReference<CustomValueType> weak : CustomValueType.instances) {
				CustomValueType value = weak.get();
				if(value!=null) {
					System.out.println(counter+": "+value);
					++alife;
				}
				++counter;
			}
			Assert.assertEquals("number of CustomValueType alife after GC", 0, alife);
		}finally {
			engine.dispose();
		}
	}
	
	@SuppressWarnings("rawtypes")
	@Test
    public void testAutoGadgetValueType() {
//		QMetaObject.forType(Singleton.class).properties().forEach(p->System.out.println(p.typeName()+" "+p.name()));
//		QMetaObject.forType(AutoGadgetValueType.class).properties().forEach(p->System.out.println(p.typeName()+" "+p.name()));
		Assume.assumeTrue("Qt version >= 6.4", QLibraryInfo.version().compareTo(new QVersionNumber(6,4))>=0);
		qmlClearTypeRegistrations();
	    qmlRegisterType(ObjectType.class, "io.qt.test", 1, 0, "ObjectType");
	    qmlRegisterType((Class)AutoGadgetValueType.class, "io.qt.test", 1, 0, "autoGadgetValueType");
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"import QtQuick 2.0\n" +
				"Item{\n" + 
				"    property autoGadgetValueType someValueType: {\"i\": 8, \"d\": 2.1, \"s\": \"TEST3\"}\n" + 
				"}");
		QQmlEngine engine = new QQmlEngine();
		try {
			engine.setOutputWarningsToStandardError(true);
			engine.warnings.connect(warnings->{
				for(QQmlError warning : warnings) {
					System.out.println(warning);//.messageType()+" "+warning.line()+" "+warning.description());
				}
			});
			
			QQmlComponent component = new QQmlComponent(engine);
			component.setData(data, (QUrl)null);
			Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
			Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
			QObject root = component.create();
			
			AutoGadgetValueType someValueType = QVariant.convert(root.property("someValueType"), AutoGadgetValueType.class);
			Assert.assertEquals(8, someValueType.i);
			Assert.assertEquals(2.1, someValueType.d, 0.0001);
			Assert.assertEquals("TEST3", someValueType.s);
		}finally {
			engine.dispose();
		}
    }
	
	public static interface CustomJavaInterface{
		
	}
	
	public static class CustomObject extends QObject{
		CustomObject(@Nullable QObject parent) {
			super(parent);
		}
	}
	
	@Test
    public void testVariantConversion() {
//		int tid = 
		QMetaType.qRegisterMetaType(CustomObject.class);
//		System.out.println(QMetaType.typeName(tid));
		QByteArray data = new QByteArray("import QtQuick 2.0\n" +
				"QtObject{\n" + 
				"    property QtObject model: context.model\n" + 
				"}");
		QQmlEngine engine = new QQmlEngine();
		try {
			engine.rootContext().setContextProperty("context", new QObject(engine) {
				@QtPropertyReader
				@QtPropertyStored("false")
				@QtPropertyConstant(false)
				public final QVariant model() {
					return QVariant.fromValue(new CustomObject(this));
				}
			});
			
			engine.setOutputWarningsToStandardError(true);
			engine.warnings.connect(warnings->{
				for(QQmlError warning : warnings) {
					System.out.println(warning);//.messageType()+" "+warning.line()+" "+warning.description());
				}
			});
			
			QQmlComponent component = new QQmlComponent(engine);
			component.setData(data, (QUrl)null);
			Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
			Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
			QObject root = component.create();
			Assert.assertTrue(root!=null);
			Assert.assertTrue(root.property("model") instanceof CustomObject);
		}finally {
			engine.dispose();
		}
	}
}
