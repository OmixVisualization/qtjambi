/****************************************************************************
**
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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotEquals;
import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import java.util.List;

import org.junit.Assert;
import org.junit.Test;

import io.qt.Nullable;
import io.qt.autotests.generated.General;
import io.qt.core.QCoreApplication;
import io.qt.core.QList;
import io.qt.core.QLogging;
import io.qt.core.QMetaMethod;
import io.qt.core.QObject;
import io.qt.core.QVariant;
import io.qt.gui.QColor;
import io.qt.gui.QVector3D;
import io.qt.location.QPlaceUser;
import io.qt.qml.QJSEngine;
import io.qt.qml.QQmlComponent;
import io.qt.qml.QQmlEngine;
import io.qt.qml.QQmlListProperty;
import io.qt.qml.QtQml;
import io.qt.quick.QQuickItem;

public class TestQmlVariant extends ApplicationInitializer{
	
	static class Custom extends QObject{
		
		private String text;

		Custom(String ownership) {
			this(ownership, null);
		}

		Custom(String ownership, @Nullable QObject parent) {
			super(parent);
			text = parent==null ? ownership : "C++-ownership";
			String strg = toString().substring(TestQmlVariant.class.getName().replace(".", "::").length()+2);
			String _text = text;
			General.internalAccess.registerCleaner(this, ()->{
				System.out.println("disposing " + strg+" "+_text);
			});
		}

		public String getText() {
			return text;
		}

		public void setText(String text) {
			this.text = text;
		}
	}
	
	static class Context extends QObject{
		private String text;
		List<Object> references = new ArrayList<>();
		
		Context(){
			String strg = toString().substring(TestQmlVariant.class.getName().replace(".", "::").length()+2);
			General.internalAccess.registerCleaner(this, ()->{
				System.out.println("disposing " + strg);
			});
		}
		
		public String getText() {
			return text;
		}

		public void setText(String text) {
			this.text = text;
		}
		
		public final QObject asQObject(Object variant) {
			return QVariant.convert(variant, QObject.class);
		}
		
		public final Object data(int type) {
			QObject qobj;
			switch(type) {
			case 0:
				return new QVector3D(1, 2, 3);
			case 1:
				// this object is by default in c++ ownership and should not be GCed
				qobj = new Custom(null, this);
				return qobj;
			case 2:
				// this object is getting JavaScript ownership and should therefore not be GCed
				// JavaScript ownership is detected during destruction of QVariant.
				qobj = new Custom("JavaScript-ownership");
				return qobj;
			case 3:
				qobj = new Custom("Java-ownership");
				references.add(qobj);
				QJSEngine.setObjectOwnership(qobj, QJSEngine.ObjectOwnership.JavaOwnership);
				return qobj;
			case 4:
				// this object is by default in c++ ownership and should not be GCed
				qobj = new Custom(null, this);
				return QtQml.qjsEngine(this).newQObject(qobj);
			case 5:
				// this object is getting JavaScript ownership and should therefore not be GCed
				// JavaScript ownership is detected during destruction of QVariant.
				qobj = new Custom("JavaScript-ownership");
				return QtQml.qjsEngine(this).newQObject(qobj);
			case 6:
				qobj = new Custom("Java-ownership");
				references.add(qobj);
				QJSEngine.setObjectOwnership(qobj, QJSEngine.ObjectOwnership.JavaOwnership);
				return QtQml.qjsEngine(this).newQObject(qobj);
			case 7:
				QPlaceUser pu = new QPlaceUser();
				pu.setUserId("PlaceUserId");
				pu.setName("PlaceUserName");
				return pu;
			default: 
				return null;
			}
		}
	}
	
	public static class MyColor extends QColor{
		public MyColor() {
			super();
		}
		
		public MyColor(MyColor c) {
			super(c);
		}

		@Override
		public MyColor clone() {
			return new MyColor(this);
		}
	}
	
	@Test
    public void test() {
		QQuickItem item = new QQuickItem();
		QVariant v = QVariant.fromValue(new MyColor());
		assertEquals(MyColor.class.getName().replace(".", "::").replace("$", "::"), v.metaType().name());
		QLogging.qInstallMessageHandler((t,c,m)->System.out.println(m));
		QQmlEngine engine = new QQmlEngine();
		Context context = new Context();
		try{
			QtQml.qmlRegisterSingletonInstance("Test", 1, 0, "Context", context);
			String data = "import QtQml\n"
				+ "import QtQuick\n"
				+ "import Test 1.0\n"
				+ "QtObject{\n"
				+ "id: testObject\n"
				+ "objectName: \"TestObject\"\n"
				+ "property list<variant> variants\n"
				+ "property list<QtObject> objects\n"
				+ "property list<Item> items\n"
				+ "property QtObject object\n"
				+ "function test(){\n"
				+ "var vec3d = Context.data(0);\n"
				+ "variants.push(vec3d);\n"
//				+ "console.log(vec3d);\n"
				+ "var qobj = Context.data(1);\n"
				+ "variants.push(qobj);\n"
				+ "variants.push(qobj.text);\n"
				+ "objects.push(qobj);\n"
//				+ "console.log(qobj);\n"
//				+ "console.log(qobj.text);\n"
				+ "var qobj2 = Context.data(2);\n"
				+ "variants.push(qobj2);\n"
				+ "variants.push(qobj2.text);\n"
				+ "objects.push(qobj2);\n"
				+ "object = qobj2;\n"
//				+ "console.log(qobj2);\n"
//				+ "console.log(qobj2.text);\n"
				+ "var qobj3 = Context.data(3);\n"
				+ "variants.push(qobj3);\n"
				+ "variants.push(qobj3.text);\n"
				+ "objects.push(qobj3);\n"
				+ "object = qobj3;\n"
//				+ "console.log(qobj3);\n"
//				+ "console.log(qobj3.text);\n"
				+ "var qobj4 = Context.data(4);\n"
				+ "variants.push(qobj4);\n"
				+ "variants.push(qobj4.text);\n"
				+ "objects.push(qobj4);\n"
//				+ "console.log(qobj4);\n"
//				+ "console.log(qobj4.text);\n"
				+ "var qobj5 = Context.data(5);\n"
				+ "variants.push(qobj5);\n"
				+ "variants.push(qobj5.text);\n"
				+ "objects.push(qobj5);\n"
				+ "object = qobj5;\n"
//				+ "console.log(qobj5);\n"
//				+ "console.log(qobj5.text);\n"
				+ "var qobj6 = Context.data(6);\n"
				+ "variants.push(qobj6);\n"
				+ "variants.push(qobj6.text);\n"
				+ "objects.push(qobj6);\n"
				+ "object = qobj6;\n"
//				+ "console.log(qobj6);\n"
//				+ "console.log(qobj6.text);\n"
				+ "var pu = Context.data(7);\n"
				+ "variants.push(pu);\n"
				+ "variants.push(pu.name);\n"
				+ "variants.push(pu.userId);\n"
				+ "}\n"
				+ "}";
			QQmlComponent component = new QQmlComponent(engine);
			component.setData(data, "");
			QObject object = component.create();
			assertTrue(component.errorString(), object!=null);
			QMetaMethod testMethod = object.metaObject().method("test");
			object.metaObject().properties().forEach(p->System.out.println(p.typeName()+" "+p.name()));
			testMethod.invoke(object);
			System.gc();
			QCoreApplication.processEvents();
			QList<?> variants = (QList<?>)object.property("variants");
			assertEquals(16, variants.size());
			int i=0;
			Object variantI = variants.get(i++);
			assertEquals(context.data(0), variantI);
			variantI = variants.get(i++);
			assertNotEquals("C++-owned object expected not to be disposed", null, variantI);
			assertTrue("Expected instance of Custom but was "+(variantI==null ? "null (deleted, C++ ownership)" : variantI.getClass().getTypeName()), variantI instanceof Custom);
			variantI = variants.get(i++);
			assertEquals("C++-ownership", variantI);
			variantI = variants.get(i++);
			assertNotEquals("JavaScript-owned object expected not to be disposed", null, variantI);
			assertTrue("Expected instance of Custom but was "+(variantI==null ? "null (deleted, JavaScript ownership)" : variantI.getClass().getTypeName()), variantI instanceof Custom);
			variantI = variants.get(i++);
			assertEquals("JavaScript-ownership", variantI);
			variantI = variants.get(i++);
			assertNotEquals("Java-owned object expected not to be disposed", null, variantI);
			assertTrue("Expected instance of Custom but was "+(variantI==null ? "null (deleted, Java ownership)" : variantI.getClass().getTypeName()), variantI instanceof Custom);
			variantI = variants.get(i++);
			assertEquals("Java-ownership", variantI);
			variantI = variants.get(i++);
			assertNotEquals("C++-owned object expected not to be disposed", null, variantI);
			assertTrue("Expected instance of Custom but was "+(variantI==null ? "null (deleted, C++ ownership)" : variantI.getClass().getTypeName()), variantI instanceof Custom);
			variantI = variants.get(i++);
			assertEquals("C++-ownership", variantI);
			variantI = variants.get(i++);
			assertNotEquals("JavaScript-owned object expected not to be disposed", null, variantI);
			assertTrue("Expected instance of Custom but was "+(variantI==null ? "null (deleted, JavaScript ownership)" : variantI.getClass().getTypeName()), variantI instanceof Custom);
			variantI = variants.get(i++);
			assertEquals("JavaScript-ownership", variantI);
			variantI = variants.get(i++);
			assertNotEquals("Java-owned object expected not to be disposed", null, variantI);
			assertTrue("Expected instance of Custom but was "+(variantI==null ? "null (deleted, Java ownership)" : variantI.getClass().getTypeName()), variantI instanceof Custom);
			variantI = variants.get(i++);
			assertEquals("Java-ownership", variantI);
			variantI = variants.get(i++);
			assertTrue("Expected instance of QPlaceUser but was "+(variantI==null ? "null" : variantI.getClass().getTypeName()), variantI instanceof QPlaceUser);
			variantI = variants.get(i++);
			assertEquals("PlaceUserName", variantI);
			variantI = variants.get(i++);
			assertEquals("PlaceUserId", variantI);
			@SuppressWarnings("unchecked")
			QQmlListProperty<QObject> objects = (QQmlListProperty<QObject>)object.property("objects");
			assertEquals(6, objects.count());
			i = 0;
			Object c1 = objects.at(i++);
			assertTrue(c1 instanceof Custom);
			assertEquals(QJSEngine.ObjectOwnership.JavaScriptOwnership, QJSEngine.objectOwnership((QObject)c1));
			assertTrue(General.internalAccess.isCppOwnership((QObject)c1));
			Object c2 = objects.at(i++);
			assertTrue(c2 instanceof Custom);
			assertEquals(QJSEngine.ObjectOwnership.JavaScriptOwnership, QJSEngine.objectOwnership((QObject)c2));
			assertTrue(General.internalAccess.isCppOwnership((QObject)c2));
			Object c3 = objects.at(i++);
			assertTrue(c3 instanceof Custom);
			assertEquals(QJSEngine.ObjectOwnership.JavaOwnership, QJSEngine.objectOwnership((QObject)c3));
			assertTrue(General.internalAccess.isJavaOwnership((QObject)c3));
			Object c4 = objects.at(i++);
			assertTrue(c4 instanceof Custom);
			assertEquals(QJSEngine.ObjectOwnership.JavaScriptOwnership, QJSEngine.objectOwnership((QObject)c4));
			assertTrue(General.internalAccess.isCppOwnership((QObject)c4));
			Object c5 = objects.at(i++);
			assertTrue(c5 instanceof Custom);
			assertEquals(QJSEngine.ObjectOwnership.JavaScriptOwnership, QJSEngine.objectOwnership((QObject)c5));
			assertTrue(General.internalAccess.isCppOwnership((QObject)c5));
			Object c6 = objects.at(i++);
			assertTrue(c6 instanceof Custom);
			assertEquals(QJSEngine.ObjectOwnership.JavaOwnership, QJSEngine.objectOwnership((QObject)c6));
			assertTrue(General.internalAccess.isJavaOwnership((QObject)c6));
			assertTrue(objects.canAppend());
			objects.append(new QObject(object));
			assertEquals(7, objects.count());
			c6 = object.property("object");
			assertTrue(c6 instanceof Custom);
			assertEquals(QJSEngine.ObjectOwnership.JavaOwnership, QJSEngine.objectOwnership((QObject)c6));
			assertTrue(General.internalAccess.isJavaOwnership((QObject)c6));
			@SuppressWarnings("unchecked")
			QQmlListProperty<QObject> items = (QQmlListProperty<QObject>)object.property("items");
			assertEquals(0, items.count());
			assertTrue(items.canAppend());
			try {
				items.append(new QObject(object));
				Assert.fail("IllegalArgumentException expected to be thrown");
			} catch (IllegalArgumentException e) {
			}
			items.append(item);
			assertEquals(1, items.count());
			try {
				items.replace(0, new QObject(object));
				Assert.fail("IllegalArgumentException expected to be thrown");
			} catch (IllegalArgumentException e) {
			}
			items.removeLast();
			assertEquals(0, items.count());
		}finally {
			engine.dispose();
			engine = null;
			context.references.clear();
			context = null;
			QtQml.qmlClearTypeRegistrations();
			System.gc();
		}
	}
}
