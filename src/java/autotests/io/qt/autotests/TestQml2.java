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

import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.NativeAccess;
import io.qt.QtClassInfo;
import io.qt.QtPropertyReader;
import io.qt.QtUtilities;
import io.qt.core.QByteArray;
import io.qt.core.QLibraryInfo;
import io.qt.core.QList;
import io.qt.core.QMetaObject;
import io.qt.core.QObject;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QSize;
import io.qt.core.QTimer;
import io.qt.core.QUrl;
import io.qt.core.QVariant;
import io.qt.gui.QColor;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QWindow;
import io.qt.qml.QQmlComponent;
import io.qt.qml.QQmlEngine;
import io.qt.qml.QQmlListProperty;
import io.qt.qml.QtQml;
import io.qt.quick.QQuickItem;

public class TestQml2 extends ApplicationInitializer{
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		QtUtilities.initializePackage("io.qt.network");
		QtUtilities.initializePackage("io.qt.widgets");
		QtUtilities.initializePackage("io.qt.quick");
		QtUtilities.initializePackage("io.qt.quick.widgets");
		if(io.qt.QtUtilities.isAvailableQtLibrary("QuickShapes"))
			QtUtilities.loadQtLibrary("QuickShapes");
		if(io.qt.QtUtilities.isAvailableQtLibrary("QuickTemplates2"))
			QtUtilities.loadQtLibrary("QuickTemplates2");
		if(io.qt.QtUtilities.isAvailableQtLibrary("QuickControls2"))
			QtUtilities.loadQtLibrary("QuickControls2");
		if(io.qt.QtUtilities.isAvailableQtLibrary("QuickParticles"))
			QtUtilities.loadQtLibrary("QuickParticles");
		ApplicationInitializer.testInitializeWithGui();
	}
	
	@AfterClass
    public static void testDispose() throws Exception {
		if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.MacOS) 
    			&& QLibraryInfo.version().majorVersion()==6 
    			&& QLibraryInfo.version().minorVersion()==5) {
	    	QWindow window = new QWindow();
	    	window.show();
	    	QTimer.singleShot(200, QGuiApplication.instance(), QGuiApplication::quit);
	    	QGuiApplication.exec();
	    	window.close();
	    	window.disposeLater();
	    	window = null;
    	}
    	ApplicationInitializer.testDispose();
    }
	
	@QtClassInfo(key="DefaultProperty", value="testObjects")
	public static class TestObjects extends QObject
	{
		@NativeAccess
		private TestObjects(QDeclarativeConstructor dc) throws IllegalAccessException {
			super(dc);
		}
		
	    private final QList<QObject> _testObjects = new QList<>(QObject.class);
		
	    @QtPropertyReader(name="testObjects")	    
		public QQmlListProperty<QObject> testObjects() {
	    	QQmlListProperty<QObject> list = new QQmlListProperty<>(this, _testObjects);
	    	System.out.println("TestQml2.TestObjects.testObjects() returning list can append: "+list.canAppend());
	    	return list;
		}
	}
	
	public static class TestChild extends QObject
	{
		@NativeAccess
		private TestChild(QDeclarativeConstructor dc) throws IllegalAccessException {
			super(dc);
		}

		@NativeAccess
		private TestChild(QObject parent) {
			super(parent);
		}
	}
	
	@Test
    public void run_testDifferentObjectsList() {
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterType(TestChild.class, "io.qt.test", 1, 0, "TestChild");
		QtQml.qmlRegisterType(TestObjects.class, "io.qt.test", 1, 0, "TestObjects");
		QQmlEngine engine = new QQmlEngine();
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"import QtQuick 2.0\n" + 
				"TestObjects {\n" + 
				"    TestChild{objectName: \"child1\"}\n" + 
				"    Item{objectName: \"item\"}\n" + 
				"    Rectangle{objectName: \"rectangle\"}\n" +
				"}");
		QQmlComponent component = new QQmlComponent(engine);
		component.setData(data, (QUrl)null);
		Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
		Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
		QObject root = component.create();
		Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
		Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
		Assert.assertTrue(root!=null);
		Assert.assertTrue(root instanceof TestObjects);
		TestObjects backEnd = (TestObjects)root;
		Assert.assertEquals(3, backEnd._testObjects.size());
		Assert.assertEquals("child1", backEnd._testObjects.get(0).objectName());
		Assert.assertEquals("child1", backEnd.testObjects().at(0).objectName());
		Assert.assertEquals("item", backEnd._testObjects.get(1).objectName());
		Assert.assertEquals("item", backEnd.testObjects().at(1).objectName());
		Assert.assertEquals("rectangle", backEnd._testObjects.get(2).objectName());
		Assert.assertEquals("rectangle", backEnd.testObjects().at(2).objectName());
		Assert.assertTrue(backEnd._testObjects.get(0) instanceof TestChild);
		Assert.assertTrue(backEnd._testObjects.get(1) instanceof QQuickItem);
		Assert.assertTrue(backEnd._testObjects.get(2) instanceof QQuickItem);
	}
	
	@Test
    public void run_testFindQmlTypesInScope() {
		QtQml.qmlClearTypeRegistrations();
		QQuickItem.Flag.values();// ensures QtQuick to be loaded
		QQmlEngine engine = new QQmlEngine();
	    QQmlComponent component = new QQmlComponent(engine);
	    component.loadUrl(new QUrl("qrc:/io/qt/autotests/qml/Root.qml"));
	    QObject root = component.create();
	    Assert.assertTrue(component.errorString(), root!=null);
	    Assert.assertEquals("root", root.objectName());
	    Assert.assertEquals(1, root.children().size());
	    Assert.assertEquals("child", root.children().get(0).objectName());
	}
	
	@Test
    public void testNullness() {
		@SuppressWarnings("unused")
		class NullnessObject extends QObject{
			Object variant = QVariant.INVALID;
			int i;
			QSize size = new QSize();
			QColor color = new QColor();
			QObject object = null;
			public Object getVariant() {
				return variant;
			}
			public void setVariant(Object variant) {
				System.out.println("NullnessObject.setVariant("+(variant==null ? "null" : variant + "[" + variant.getClass().getName()+"]")+")");
				this.variant = variant;
			}
			public int getI() {
				return i;
			}
			public void setI(int i) {
				System.out.println("NullnessObject.setI("+i+")");
				this.i = i;
			}
			public QSize getSize() {
				return size;
			}
			public void setSize(QSize size) {
				System.out.println("NullnessObject.setSize("+size+")");
				this.size = size;
			}
			public QColor getColor() {
				return color;
			}
			public void setColor(QColor color) {
				System.out.println("NullnessObject.setColor("+color+")");
				this.color = color;
			}
			public QObject getObject() {
				return object;
			}
			public void setObject(QObject object) {
				System.out.println("NullnessObject.setObject("+object+")");
				this.object = object;
			}
		}
		QtQml.qmlClearTypeRegistrations();
		NullnessObject nullnessObject = new NullnessObject();
		QtQml.qmlRegisterSingletonInstance("io.qt.test", 1, 0, "Nullness", nullnessObject);
		QQmlEngine engine = new QQmlEngine();
		QByteArray data = new QByteArray("import io.qt.test 1.0\n"
				+ "import QtQuick 2.0\n"
				+ "QtObject {\n"
				+ "    function testNull(){\n"
				+ "        let v = null\n"
				+ "        Nullness.setVariant(v)\n"
				+ "        Nullness.setObject(v)\n"
//				+ "        Nullness.setSize(v)\n"
				+ "        Nullness.setColor(v)\n"
				+ "        Nullness.setI(v)\n"
				+ "        Nullness.variant = v\n"
				+ "        Nullness.object = v\n"
//				+ "        Nullness.i = v\n"
				+ "        Nullness.size = v\n"
				+ "        Nullness.color = v\n"
				+ "    }\n"
				+ "    function testUndefined(){\n"
				+ "        let v = undefined\n"
				+ "        Nullness.setVariant(v)\n"
				+ "        Nullness.setObject(v)\n"
//				+ "        Nullness.setSize(v)\n"
//				+ "        Nullness.setColor(v)\n"
				+ "        Nullness.setI(v)\n"
				+ "        Nullness.variant = v\n"
//				+ "        Nullness.object = v\n"
//				+ "        Nullness.i = v\n"
//				+ "        Nullness.size = v\n"
//				+ "        Nullness.color = v\n"
				+ "    }\n"
				+ "}");
		QQmlComponent component = new QQmlComponent(engine);
		component.setData(data, (QUrl)null);
		QObject root = component.create();
	    Assert.assertTrue(component.errorString(), root!=null);
	    System.out.println("testNull()");
	    QMetaObject.invokeMethod(root, "testNull");
	    System.out.println("testUndefined()");
	    QMetaObject.invokeMethod(root, "testUndefined");
//	    System.out.println(component.errorString());
	}
}
