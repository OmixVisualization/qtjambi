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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import java.util.List;

import org.junit.Test;

import io.qt.Nullable;
import io.qt.autotests.generated.General;
import io.qt.core.QCoreApplication;
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

public class TestQmlVariant extends ApplicationInitializer{
	
	static class Custom extends QObject{
		
		private String text;

		Custom() {
			this(null);
		}

		Custom(@Nullable QObject parent) {
			super(parent);
			text = parent==null ? "qobj-java" : "qobj-c++";
		}

		public String getText() {
			return text;
		}

		public void setText(String text) {
			this.text = text;
		}
	}
	
	static class Context extends QObject{
		QQmlEngine engine;
		private String text;
		List<Object> references = new ArrayList<>();
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
				qobj = new Custom(this);
				return qobj;
			case 2:
				// this object is getting JavaScript ownership and should therefore not be GCed
				// JavaScript ownership is detected during destruction of QVariant.
				qobj = new Custom();
				return qobj;
			case 3:
				qobj = new Custom();
				references.add(qobj);
				QJSEngine.setObjectOwnership(qobj, QJSEngine.ObjectOwnership.JavaOwnership);
				return qobj;
			case 4:
				QPlaceUser pu = new QPlaceUser();
				pu.setUserId("PlaceUserId");
				pu.setName("PlaceUserName");
				return pu;
				default: break;
			}
			return null;
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
		QVariant v = QVariant.fromValue(new MyColor());
		assertEquals(MyColor.class.getName().replace(".", "::").replace("$", "::"), v.metaType().name());
		QLogging.qInstallMessageHandler((t,c,m)->System.out.println(m));
		QQmlEngine engine = new QQmlEngine();
		QQmlComponent component = new QQmlComponent(engine);
		Context context = new Context();
		context.engine = engine;
		engine.rootContext().setContextProperty("context", context);
		String data = "import QtQml\n"
				+ "QtObject{\n"
				+ "id: testObject\n"
				+ "objectName: \"TestObject\"\n"
				+ "property list<variant> variants\n"
				+ "property list<QtObject> objects\n"
				+ "property QtObject object\n"
				+ "function test(){\n"
				+ "var vec3d = context.data(0);\n"
				+ "variants.push(vec3d);\n"
//				+ "console.log(vec3d);\n"
				+ "var qobj = context.data(1);\n"
				+ "variants.push(qobj);\n"
				+ "variants.push(qobj.text);\n"
				+ "objects.push(qobj);\n"
//				+ "console.log(qobj);\n"
//				+ "console.log(qobj.text);\n"
				+ "var qobj2 = context.data(2);\n"
				+ "variants.push(qobj2);\n"
				+ "variants.push(qobj2.text);\n"
				+ "objects.push(qobj2);\n"
				+ "object = qobj2;\n"
//				+ "console.log(qobj2);\n"
//				+ "console.log(qobj2.text);\n"
				+ "var qobj3 = context.data(3);\n"
				+ "variants.push(qobj3);\n"
				+ "variants.push(qobj3.text);\n"
				+ "objects.push(qobj3);\n"
				+ "object = qobj3;\n"
//				+ "console.log(qobj3);\n"
//				+ "console.log(qobj3.text);\n"
				+ "var pu = context.data(4);\n"
				+ "variants.push(pu);\n"
				+ "variants.push(pu.name);\n"
				+ "variants.push(pu.userId);\n"
				+ "}\n"
				+ "}";
		component.setData(data, "");
		QObject object = component.create();
		assertTrue(component.errorString(), object!=null);
		QMetaMethod testMethod = object.metaObject().method("test");
//		object.metaObject().properties().forEach(p->System.out.println(p.typeName()+" "+p.name()));
		testMethod.invoke(object);
		System.gc();
		QCoreApplication.processEvents();
		List<?> variants = (List<?>)object.property("variants");
		assertEquals(10, variants.size());
		int i=0;
		assertEquals(context.data(0), variants.get(i++));
		assertTrue(variants.get(i++) instanceof Custom);
		assertEquals("qobj-c++", variants.get(i++));
		assertTrue(variants.get(i++) instanceof Custom);
		assertEquals("qobj-java", variants.get(i++));
		assertTrue(variants.get(i++) instanceof Custom);
		assertEquals("qobj-java", variants.get(i++));
		assertTrue(variants.get(i++) instanceof QPlaceUser);
		assertEquals("PlaceUserName", variants.get(i++));
		assertEquals("PlaceUserId", variants.get(i++));
		QQmlListProperty<?> objects = (QQmlListProperty<?>)object.property("objects");
		assertEquals(3, objects.count());
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
		c3 = object.property("object");
		assertTrue(c3 instanceof Custom);
		assertEquals(QJSEngine.ObjectOwnership.JavaOwnership, QJSEngine.objectOwnership((QObject)c3));
		assertTrue(General.internalAccess.isJavaOwnership((QObject)c3));
	}
}
