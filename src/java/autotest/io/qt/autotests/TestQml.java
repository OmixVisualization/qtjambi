/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Objects;
import java.util.Random;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicReference;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.Assume;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtClassInfo;
import io.qt.QtPropertyNotify;
import io.qt.QtPropertyReader;
import io.qt.QtPropertyWriter;
import io.qt.QtUtilities;
import io.qt.autotests.generated.TestInterface;
import io.qt.core.QByteArray;
import io.qt.core.QDir;
import io.qt.core.QEvent;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaObject.AbstractPrivateSignal1;
import io.qt.core.QObject;
import io.qt.core.QPointF;
import io.qt.core.QRectF;
import io.qt.core.QRunnable;
import io.qt.core.QUrl;
import io.qt.core.Qt;
import io.qt.gui.QColor;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QPainter;
import io.qt.internal.NativeAccess;
import io.qt.qml.QJSEngine;
import io.qt.qml.QJSValue;
import io.qt.qml.QQmlApplicationEngine;
import io.qt.qml.QQmlComponent;
import io.qt.qml.QQmlEngine;
import io.qt.qml.QQmlIncubationController;
import io.qt.qml.QQmlIncubator;
import io.qt.qml.QQmlListProperty;
import io.qt.qml.QQmlParserStatus;
import io.qt.qml.QQmlProperty;
import io.qt.qml.QQmlPropertyValueSource;
import io.qt.qml.QtQml;
import io.qt.qt3d.core.quick.QQmlAspectEngine;
import io.qt.quick.QQuickItem;
import io.qt.quick.QQuickView;
import io.qt.quick.widgets.QQuickWidget;
import io.qt.widgets.QGraphicsItem;
import io.qt.widgets.QStyleOptionGraphicsItem;
import io.qt.widgets.QWidget;

public class TestQml extends QApplicationTest{
	
	@AfterClass
    public static void testDispose() throws Exception {
		RandomNumberGenerator.instance = null;
		QApplicationTest.testDispose();
	}
	
	@SuppressWarnings("serial")
	public static class TestException extends RuntimeException{}
	
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
	
	@QtClassInfo(key="DefaultProperty", value="testChildren")
	public static class TestObject extends QObject
	{
		@NativeAccess
		private TestObject(QDeclarativeConstructor dc) throws IllegalAccessException {
			super(dc);
		}

		@Override
		public boolean event(QEvent event) {
			return super.event(event);
		}

		@QtPropertyNotify(name="userName")
		public final Signal0 userNameChanged = new Signal0();
	    
	    public TestObject() {
	    	super();
	    }

	    public TestObject(QObject parent) {
	    	super(parent);
	    }

	    @QtPropertyReader(name="userName")
	    public String userName() {return m_userName;}
	    @QtPropertyWriter(name="userName")
	    void setUserName(String userName) { 
	    	if(Objects.equals(m_userName, userName)) 
	    		return; 
	    	m_userName = userName; 
	    	userNameChanged.emit();
	    	userNameChanged.emit();
	    }

	    @QtPropertyReader(name="userName2")
	    public String userName2() {return m_userName2;}
	    @QtPropertyWriter(name="userName2")
	    void setUserName2(String userName2) { 
	    	if(Objects.equals(m_userName2, userName2)) 
	    		return; 
	    	m_userName2 = userName2;
    	}

	    private String m_userName = "";
	    private String m_userName2 = "not set";
	    private List<TestChild> _testChildren = new ArrayList<>();
	    private final QQmlListProperty<TestChild> testChildren = new QQmlListProperty<>(this, _testChildren);
	    private List<QQuickItem> _testItems = new ArrayList<>();
	    private final QQmlListProperty<QQuickItem> testItems = new QQmlListProperty<>(this, _testItems);

	    @QtPropertyReader(name="testChildren")	    
		public QQmlListProperty<TestChild> testChildren() {
			return testChildren;
		}
	    
	    @QtPropertyReader(name="testItems")	    
		public QQmlListProperty<QQuickItem> testItems() {
			return testItems;
		}
	};
	
	public static class TestObjectExn extends QObject
	{
		@NativeAccess
		private TestObjectExn(QDeclarativeConstructor dc) throws IllegalAccessException {
			super(dc);
			throw new TestException();
		}
	};
	
	public static class TestObjectExn2 extends QQuickItem implements TestInterface, QQmlParserStatus, QQmlPropertyValueSource
	{
		@NativeAccess
		private TestObjectExn2(QDeclarativeConstructor dc) throws IllegalAccessException {
			super(dc);
		}

		@Override
		public void classBegin() {
		}

		@Override
		public void componentComplete() {
		}

		@Override
		public void setTarget(QQmlProperty arg__1) {
		}
		
		@Override
		public boolean setReferenceCountTest1(QObject object) {
			return false;
		}

		@Override
		public String method4() {
			return "TestObjectExn2::method4";
		}

		@Override
		public String method5() {
			return "TestObjectExn2::method5";
		}
	};

	public static class TestObjectExn3 extends QObject
	{
		@NativeAccess
		private TestObjectExn3(QDeclarativeConstructor dc) throws IllegalAccessException {
			super(dc);
		}
	    @QtPropertyReader(name="exn")
	    public String exn() {
	    	throw new TestException();
	    }
	    @QtPropertyWriter(name="exn")
	    void setExn(String e) {
	    	throw new TestException();
	    }
	}

	public static class TestObjectExtension extends QObject{
		public TestObjectExtension(QObject parent) {
			super(parent);
		}
		
		private QColor testColor;
		
		@QtPropertyWriter(name="testColor")
	    public void setTestColor(QColor testColor) {
			this.testColor = testColor;
		}

		@QtPropertyReader(name="testColor")
	    public QColor testColor() {return testColor;}
	}
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		QtUtilities.initializePackage("io.qt.network");
		QtUtilities.initializePackage("io.qt.widgets");
		QtUtilities.initializePackage("io.qt.quick");
		QtUtilities.initializePackage("io.qt.quick.widgets");
		QtUtilities.loadQtLibrary("QuickShapes");
		QtUtilities.loadQtLibrary("QuickTemplates2");
		QtUtilities.loadQtLibrary("QuickControls2");
		QtUtilities.loadQtLibrary("QuickParticles");
		QApplicationTest.testInitialize();
	}

	@Before
    public void setUp() {
	}
	
	@After
    public void tearDown() {
		
	}
	
	@Test
    public void run_test() {
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterType(TestChild.class, "io.qt.test", 1, 0, "TestChild");
		QtQml.qmlRegisterType(TestObject.class, "io.qt.test", 1, 0, "TestObject");
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"import QtQuick 2.0\n" + 
				"TestObject {\n" + 
				"    id: backend\n" + 
				"    objectName: \"backend\"\n" +
				"    onUserNameChanged: userName2 = userName\n" +
				"    testChildren: [TestChild{objectName: \"child1\"}, TestChild{objectName: \"child2\"}]\n" +
				"}");
		QQmlEngine engine = new QQmlEngine();
		String prefix = io.qt.QtUtilities.qtPrefix();
        if(new QDir(prefix+"/qml").exists()) {
        	engine.addImportPath(prefix+"/qml");
        }
		QQmlComponent component = new QQmlComponent(engine);
		component.setData(data, null);
		Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
		Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
		QObject root = component.create();
		Assert.assertTrue(root instanceof TestObject);
		TestObject backEnd = (TestObject)root;
//		backEnd.event(new QEvent(QEvent.Type.User));
		Assert.assertEquals("backend", backEnd.objectName());
		backEnd.setUserName("test");
		Assert.assertEquals("test", backEnd.userName());
		Assert.assertEquals("test", backEnd.userName2());
		Assert.assertEquals(2, backEnd._testChildren.size());
		Assert.assertEquals("child1", backEnd._testChildren.get(0).objectName());
		Assert.assertEquals("child1", backEnd.testChildren.at(0).objectName());
		Assert.assertEquals("child2", backEnd._testChildren.get(1).objectName());
		Assert.assertEquals("child2", backEnd.testChildren.at(1).objectName());
	}
	
	@Test
    public void testExceptionInConstructor_QmlComponent() {
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterType(TestObjectExn.class, "io.qt.test", 1, 0, "TestObject");
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"import QtQuick 2.0\n" + 
				"TestObject {\n" + 
				"    id: test\n" + 
				"}");
		QQmlEngine engine = new QQmlEngine();
		String prefix = io.qt.QtUtilities.qtPrefix();
        if(new QDir(prefix+"/qml").exists()) {
        	engine.addImportPath(prefix+"/qml");
        }
		QQmlComponent component = new QQmlComponent(engine);
		component.setData(data, null);
		Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
		Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
		try {
			component.create();
			Assert.assertTrue("Error expected to be thrown", false);
		} catch (AssertionError e) {
			throw e;
		} catch (Throwable e) {
			Assert.assertEquals(e.getMessage(), TestException.class, e.getClass());
		}
	}
	
	@Test
    public void testExceptionInConstructor_QQmlIncubationController_incubateWhile() {
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterType(TestObjectExn.class, "io.qt.test", 1, 0, "TestObject");
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"import QtQuick 2.0\n" + 
				"TestObject {\n" + 
				"    id: test\n" + 
				"}");
		QQmlIncubator incubator = new QQmlIncubator(QQmlIncubator.IncubationMode.Asynchronous);
		QQmlEngine engine = new QQmlEngine();
		QQmlIncubationController incubationController = new QQmlIncubationController.Impl();
		engine.setIncubationController(incubationController);
		String prefix = io.qt.QtUtilities.qtPrefix();
        if(new QDir(prefix+"/qml").exists()) {
        	engine.addImportPath(prefix+"/qml");
        }
		QQmlComponent component = new QQmlComponent(engine);
		component.setData(data, null);
		Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
		Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
		try {
			component.create(incubator);
			QQmlIncubationController.WhileFlag flag = new QQmlIncubationController.WhileFlag();
			incubationController.incubateWhile(flag);
			Assert.assertTrue("Error expected to be thrown", false);
		} catch (AssertionError e) {
			throw e;
		} catch (Throwable e) {
			Assert.assertEquals(e.getMessage(), TestException.class, e.getClass());
		}
	}
	
	@Test
    public void testExceptionInConstructor_QQmlIncubationController_incubateFor() {
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterType(TestObjectExn.class, "io.qt.test", 1, 0, "TestObject");
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"import QtQuick 2.0\n" + 
				"TestObject {\n" + 
				"    id: test\n" + 
				"}");
		QQmlIncubator incubator = new QQmlIncubator(QQmlIncubator.IncubationMode.Asynchronous);
		QQmlEngine engine = new QQmlEngine();
		QQmlIncubationController incubationController = new QQmlIncubationController.Impl();
		engine.setIncubationController(incubationController);
		String prefix = io.qt.QtUtilities.qtPrefix();
        if(new QDir(prefix+"/qml").exists()) {
        	engine.addImportPath(prefix+"/qml");
        }
		QQmlComponent component = new QQmlComponent(engine);
		component.setData(data, null);
		Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
		Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
		try {
			component.create(incubator);
			incubationController.incubateFor(10);
			Assert.assertTrue("Error expected to be thrown", false);
		} catch (AssertionError e) {
			throw e;
		} catch (Throwable e) {
			Assert.assertEquals(e.getMessage(), TestException.class, e.getClass());
		}
	}
	
	@Test
    public void testExceptionInConstructor_QQmlIncubator_forceCompletion() {
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterType(TestObjectExn.class, "io.qt.test", 1, 0, "TestObject");
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"import QtQuick 2.0\n" + 
				"TestObject {\n" + 
				"    id: test\n" + 
				"}");
		QQmlIncubator incubator = new QQmlIncubator(QQmlIncubator.IncubationMode.Asynchronous);
		QQmlEngine engine = new QQmlEngine();
		QQmlIncubationController incubationController = new QQmlIncubationController.Impl();
		engine.setIncubationController(incubationController);
		String prefix = io.qt.QtUtilities.qtPrefix();
        if(new QDir(prefix+"/qml").exists()) {
        	engine.addImportPath(prefix+"/qml");
        }
		QQmlComponent component = new QQmlComponent(engine);
		component.setData(data, null);
		Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
		Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
		try {
			component.create(incubator);
			incubator.forceCompletion();
			Assert.assertTrue("Error expected to be thrown", false);
		} catch (AssertionError e) {
			throw e;
		} catch (Throwable e) {
			Assert.assertEquals(e.getMessage(), TestException.class, e.getClass());
		}
	}
	
	@Test
    public void testExceptionInConstructor_QmlApplicationEngine() {
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterType(TestObjectExn.class, "io.qt.test", 1, 0, "TestObject");
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"import QtQuick 2.0\n" + 
				"TestObject {\n" + 
				"    id: test\n" + 
				"}");
		QQmlApplicationEngine engine = new QQmlApplicationEngine();
		String prefix = io.qt.QtUtilities.qtPrefix();
        if(new QDir(prefix+"/qml").exists()) {
        	engine.addImportPath(prefix+"/qml");
        }
		try {
			engine.loadData(data);
			Assert.assertTrue("Error expected to be thrown", false);
		} catch (AssertionError e) {
			throw e;
		} catch (Throwable e) {
			Assert.assertEquals(e.getMessage(), TestException.class, e.getClass());
		}
	}
	
	@Test
    public void testExceptionInBinding_QmlComponent() {
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterType(TestObjectExn3.class, "io.qt.test", 1, 0, "TestObject");
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"import QtQuick 2.0\n" + 
				"TestObject {\n" + 
				"    id: test\n" +
				"    exn: 'x'\n" + 
				"}");
		QQmlEngine engine = new QQmlEngine();
		String prefix = io.qt.QtUtilities.qtPrefix();
        if(new QDir(prefix+"/qml").exists()) {
        	engine.addImportPath(prefix+"/qml");
        }
		QQmlComponent component = new QQmlComponent(engine);
		component.setData(data, null);
		Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
		Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
		try {
			component.create();
			Assert.assertTrue("Error expected to be thrown", false);
		} catch (AssertionError e) {
			throw e;
		} catch (Throwable e) {
			Assert.assertEquals(e.getMessage(), TestException.class, e.getClass());
		}
	}
	
	@Test
    public void testExceptionInInterface_QmlComponent() {
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterType(TestObjectExn2.class, "io.qt.test", 1, 0, "TestObject");
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"import QtQuick 2.0\n" + 
				"TestObject {\n" + 
				"    id: test\n" + 
				"}");
		QQmlEngine engine = new QQmlEngine();
		String prefix = io.qt.QtUtilities.qtPrefix();
        if(new QDir(prefix+"/qml").exists()) {
        	engine.addImportPath(prefix+"/qml");
        }
		QQmlComponent component = new QQmlComponent(engine);
		component.setData(data, null);
		Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
		Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
		try {
			component.create();
			Assert.assertFalse("Error expected to be thrown", true);
		} catch (AssertionError e) {
			throw e;
		} catch (Error e) {
			assertEquals("Cannot initialize interface io.qt.autotests.generated.TestInterface without arguments. Please use the private constructor and QtUtilities.initializeNativeObject(object, arguments...) instead.", e.getMessage());
		}
	}
	
	@Test
    public void testExceptionInInterface_QQmlAspectEngine() {
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterType(TestObjectExn2.class, "io.qt.test", 1, 0, "TestObject");
		QQmlAspectEngine component = new QQmlAspectEngine();
		String prefix = io.qt.QtUtilities.qtPrefix();
        if(new QDir(prefix+"/qml").exists()) {
        	component.qmlEngine().addImportPath(prefix+"/qml");
        }
		try {
			component.setSource(QUrl.fromLocalFile("classpath:io/qt/autotests/qml/TestExn.qml"));
			Assert.assertFalse("Error expected to be thrown", true);
		} catch (AssertionError e) {
			throw e;
		} catch (Error e) {
			assertEquals("Cannot initialize interface io.qt.autotests.generated.TestInterface without arguments. Please use the private constructor and QtUtilities.initializeNativeObject(object, arguments...) instead.", e.getMessage());
		}
	}
	
	@Test
    public void testExceptionInInterface_QuickWidget() {
		Assume.assumeThat(QGuiApplication.primaryScreen()!=null, QApplicationTest.trueMatcher("A screen is required to create a window."));
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterType(TestObjectExn2.class, "io.qt.test", 1, 0, "TestObject");
		QQuickWidget component = new QQuickWidget();
		String prefix = io.qt.QtUtilities.qtPrefix();
        if(new QDir(prefix+"/qml").exists()) {
        	component.engine().addImportPath(prefix+"/qml");
        }
		try {
			component.setSource(QUrl.fromLocalFile("classpath:io/qt/autotests/qml/TestExn.qml"));
			Assert.assertFalse("Error expected to be thrown", true);
		} catch (AssertionError e) {
			throw e;
		} catch (Error e) {
			assertEquals("Cannot initialize interface io.qt.autotests.generated.TestInterface without arguments. Please use the private constructor and QtUtilities.initializeNativeObject(object, arguments...) instead.", e.getMessage());
		}
	}
	
	@Test
    public void testExceptionInInterface_QuickView() {
		Assume.assumeThat(QGuiApplication.primaryScreen()!=null, QApplicationTest.trueMatcher("A screen is required to create a window."));
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterType(TestObjectExn2.class, "io.qt.test", 1, 0, "TestObject");
		QQuickView component = new QQuickView();
		String prefix = io.qt.QtUtilities.qtPrefix();
        if(new QDir(prefix+"/qml").exists()) {
        	component.engine().addImportPath(prefix+"/qml");
        }
		try {
			component.setSource(QUrl.fromLocalFile("classpath:io/qt/autotests/qml/TestExn.qml"));
			Assert.assertFalse("Error expected to be thrown", true);
		} catch (AssertionError e) {
			throw e;
		} catch (Error e) {
			assertEquals("Cannot initialize interface io.qt.autotests.generated.TestInterface without arguments. Please use the private constructor and QtUtilities.initializeNativeObject(object, arguments...) instead.", e.getMessage());
		}
	}
	
	@Test
    public void run_testDefaultProperty() {
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterType(TestChild.class, "io.qt.test", 1, 0, "TestChild");
		QtQml.qmlRegisterExtendedType(TestObject.class, TestObjectExtension.class, "io.qt.test", 1, 0, "TestObject");
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"import QtQuick 2.0\n" + 
				"TestObject {\n" + 
				"    TestChild{objectName: \"child1\"}\n" + 
				"	 TestChild{objectName: \"child2\"}\n" + 
				"    id: backend\n" + 
				"    objectName: \"backend\"\n" +
				"    onUserNameChanged: userName2 = userName\n" +
				"	 testColor: \"blue\"\n" +
				"}");
		QQmlEngine engine = new QQmlEngine();
		String prefix = io.qt.QtUtilities.qtPrefix();
        if(new QDir(prefix+"/qml").exists()) {
        	engine.addImportPath(prefix+"/qml");
        }
		QQmlComponent component = new QQmlComponent(engine);
		component.setData(data, null);
		Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
		Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
		QObject root = component.create();
		Assert.assertTrue(root instanceof TestObject);
		TestObject backEnd = (TestObject)root;
		Assert.assertEquals(2, backEnd._testChildren.size());
		Assert.assertEquals("child1", backEnd._testChildren.get(0).objectName());
		Assert.assertEquals("child1", backEnd.testChildren.at(0).objectName());
		Assert.assertEquals("child2", backEnd._testChildren.get(1).objectName());
		Assert.assertEquals("child2", backEnd.testChildren.at(1).objectName());
		Assert.assertTrue(backEnd.children().at(0) instanceof TestObjectExtension);
		TestObjectExtension ext = (TestObjectExtension)backEnd.children().at(0);
		Assert.assertEquals(new QColor(Qt.GlobalColor.blue), ext.testColor());
		Assert.assertEquals("backend", backEnd.objectName());
		backEnd.setUserName("test");
		Assert.assertEquals("test", backEnd.userName());
		Assert.assertEquals("test", backEnd.userName2());
	}
	
	@Test
    public void run_testUnmatchingListAssignment() {
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterType(TestChild.class, "io.qt.test", 1, 0, "TestChild");
		QtQml.qmlRegisterType(TestObject.class, "io.qt.test", 1, 0, "TestObject");
		QQmlEngine engine = new QQmlEngine();
		String prefix = io.qt.QtUtilities.qtPrefix();
        if(new QDir(prefix+"/qml").exists()) {
        	engine.addImportPath(prefix+"/qml");
        }
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"import QtQuick 2.0\n" + 
				"TestObject {\n" + 
				"    id: backend\n" + 
				"    testChildren: [Item{}]\n" +
				"}");
		QQmlComponent component = new QQmlComponent(engine);
		component.setData(data, null);
		Assert.assertEquals(QQmlComponent.Status.Error, component.status());
		Assert.assertEquals(":5 Cannot assign object to list property \"testChildren\"", component.errorString().trim());
	}
	
	@Test
    public void run_testUnmatchingListAssignment2() {
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterType(TestChild.class, "io.qt.test", 1, 0, "TestChild");
		QtQml.qmlRegisterType(TestObject.class, "io.qt.test", 1, 0, "TestObject");
		QQmlEngine engine = new QQmlEngine();
		String prefix = io.qt.QtUtilities.qtPrefix();
        if(new QDir(prefix+"/qml").exists()) {
        	engine.addImportPath(prefix+"/qml");
        }
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"import QtQuick 2.0\n" + 
				"TestObject {\n" + 
				"    id: backend\n" + 
				"    testItems: [TestChild{}]\n" +
				"}");
		QQmlComponent component = new QQmlComponent(engine);
		component.setData(data, null);
		Assert.assertEquals(QQmlComponent.Status.Error, component.status());
		Assert.assertEquals(":5 Cannot assign object to list property \"testItems\"", component.errorString().trim());
	}
	
	@Test
    public void run_testExposingProperties() {
		QtQml.qmlClearTypeRegistrations();
		class Msg extends QObject
		{
		    @QtPropertyWriter(name="author")
		    public void setAuthor(String a) {
		        if (!Objects.equals(author,a)) {
		            author = a;
		            authorChanged.emit();
		        }
		    }
		    @QtPropertyReader(name="author")
		    String author(){
		        return author;
		    }
		    @QtPropertyNotify(name="author")
		    public final Signal0 authorChanged = new Signal0();
		    private String author = "test";
		}
		
		Msg msg = new Msg();
		
		QQmlEngine engine = new QQmlEngine();
		String prefix = io.qt.QtUtilities.qtPrefix();
        if(new QDir(prefix+"/qml").exists()) {
        	engine.addImportPath(prefix+"/qml");
        }
		engine.rootContext().setContextProperty("msg", msg);
		QByteArray data = new QByteArray("import QtQuick 2.0\n" + 
				"Text {\n" + 
				"    text: msg.author\n" + 
				"}");
		QQmlComponent component = new QQmlComponent(engine);
		component.setData(data, null);
		Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
		Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
		QObject root = component.create();
		Assert.assertTrue(root instanceof QQuickItem);
		QQuickItem textItem = (QQuickItem)root;
		Assert.assertEquals("test", textItem.property("text"));
		msg.setAuthor("author");
		Assert.assertEquals("author", textItem.property("text"));
	}
	
	static class MessageBody extends QObject
	{
		private MessageBody(QDeclarativeConstructor dc) throws IllegalAccessException {
			super(dc);
		}
		@QtPropertyWriter(name="text")
	    public void setText(String a) {
	        if (!Objects.equals(text,a)) {
	        	text = a;
	            textChanged.emit();
	        }
	    }
	    @QtPropertyReader(name="text")
	    String text(){
	        return text;
	    }
	    @QtPropertyNotify(name="text")
	    public final Signal0 textChanged = new Signal0();
	    private String text = "test";
	}
	
	static class MessageAuthor extends QObject
	{
		public MessageAuthor(QDeclarativeConstructor dc) throws IllegalAccessException {
			super(dc);
		}
		private MessageAuthor(QObject parent) {
			super(parent);
		}
		@QtPropertyWriter(name="name")
	    public void setName(String a) {
	        if (!Objects.equals(name,a)) {
	        	name = a;
	        	nameChanged.emit();
	        }
	    }
		@QtPropertyWriter(name="email")
	    public void setEmail(String a) {
	        if (!Objects.equals(email,a)) {
	        	email = a;
	        	emailChanged.emit();
	        }
	    }
	    @QtPropertyReader(name="name")
	    public String name(){
	        return name;
	    }
	    @QtPropertyReader(name="email")
	    public String email(){
	        return email;
	    }
	    @QtPropertyNotify(name="name")
	    public final Signal0 nameChanged = new Signal0();
	    @QtPropertyNotify(name="email")
	    public final Signal0 emailChanged = new Signal0();
	    private String name = "";
	    private String email = "";
	}
	
	static class Message extends QObject
	{
	    private Message(QDeclarativeConstructor dc) throws IllegalAccessException {
			super(dc);
		}
		@QtPropertyWriter(name="body")
	    public void setBody(MessageBody a) {
	        if (!Objects.equals(body,a)) {
	            body = a;
	            bodyChanged.emit();
	        }
	    }
	    @QtPropertyReader(name="body")
	    public MessageBody body(){
	        return body;
	    }
	    @QtPropertyReader(name="author")
		public MessageAuthor author() {
			return author;
		}
	    @QtPropertyNotify(name="body")
	    public final Signal0 bodyChanged = new Signal0();
	    private MessageBody body;
	    private MessageAuthor author = new MessageAuthor(this);
	}
	
	static class MessageBoard extends QObject
	{
		public final Signal1<String> signalOnChanged = new Signal1<>();
		
		{
			signalOnChanged.connect(s->receivedText = s);
		}
		
		@QtPropertyReader(name="messages")
	    public QQmlListProperty<Message> messages() {
	    	return p_messages;
	    }

	    List<Message> m_messages = new ArrayList<>();
		QQmlListProperty<Message> p_messages = new QQmlListProperty<>(this, m_messages);
		
		boolean hasReceived;
		String receivedText;
		
		public void slotOnChanged() {
			hasReceived = true;
		}
	}
	
	static class Singleton extends QObject{

		public Singleton() {
			super();
		}
		
		@QtPropertyReader(name="text")
	    public String text() {
	    	return "Hello, world!";
	    }
		
		@QtPropertyNotify(name="text")
		public final Signal1<String> textChanged = new Signal1<>();
	}
	
	@Test
    public void run_testPropertieswithObjectTypes() {
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterUncreatableType(MessageAuthor.class, "io.qt.test", 1, 0, "MessageAuthor", "");
		QtQml.qmlRegisterType(MessageBody.class, "io.qt.test", 1, 0, "MessageBody");
		QtQml.qmlRegisterType(Message.class, "io.qt.test", 1, 0, "Message");
		AtomicBoolean singletonCreated = new AtomicBoolean();
		QtQml.ObjectCallback objectCallback;
		QtQml.qmlRegisterSingletonType(Singleton.class, "io.qt.test", 1, 0, "TextSource", objectCallback = (QQmlEngine qmlEngine, QJSEngine jsEngine)->{
			singletonCreated.set(true);
			return new Singleton();
		});
		QtQml.ValueCallback valueCallback;
		QtQml.qmlRegisterSingletonType("io.qt.test", 1, 0, "AuthorSource", valueCallback = (QQmlEngine qmlEngine, QJSEngine jsEngine)->{
			QJSValue example = jsEngine.newObject();
		    example.setProperty("email", new QJSValue("alexandra@mail.com"));
		    example.setProperty("name", new QJSValue("Alexandra"));
			return example;
		});
		
		MessageBoard messageBoard = new MessageBoard();
		
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"Message {\n" + 
				"    body: MessageBody {\n" + 
				"        text: TextSource.text\n" + 
				"        onTextChanged: {messageBoard.slotOnChanged(); messageBoard.signalOnChanged(text); }\n" +
				"    }\n" + 
				"    author.name: AuthorSource.name\n" + 
				"    author.email: AuthorSource.email\n" +  
				"}");
		QQmlEngine engine = new QQmlEngine();
		String prefix = io.qt.QtUtilities.qtPrefix();
        if(new QDir(prefix+"/qml").exists()) {
        	engine.addImportPath(prefix+"/qml");
        }
		QQmlComponent component = new QQmlComponent(engine);
		engine.rootContext().setContextProperty("messageBoard", messageBoard);
		component.setData(data, null);
		Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
		Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
		QObject root = component.create();
		Assert.assertTrue(root instanceof Message);
		Message message = (Message)root;
		Assert.assertTrue(message.body() instanceof MessageBody);
		Assert.assertEquals("Hello, world!", message.body().text());
		messageBoard.hasReceived = false;
		messageBoard.receivedText = null;
		message.body().setText("newBody");
		Assert.assertTrue(messageBoard.hasReceived);
		Assert.assertEquals("newBody", messageBoard.receivedText);
		Assert.assertTrue(new QQmlProperty(message, "author").read() instanceof MessageAuthor);
		Assert.assertEquals("Alexandra", message.author().name());
		Assert.assertEquals("alexandra@mail.com", message.author().email());
		Assert.assertTrue(singletonCreated.get());
		objectCallback.dispose();
		valueCallback.dispose();
	}
	
	@Test
    public void run_testSingletonInstance() {
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterUncreatableType(MessageAuthor.class, "io.qt.test", 1, 0, "MessageAuthor", "");
		QtQml.qmlRegisterType(MessageBody.class, "io.qt.test", 1, 0, "MessageBody");
		QtQml.qmlRegisterType(Message.class, "io.qt.test", 1, 0, "Message");
		Singleton s = new Singleton();
		QtQml.qmlRegisterSingletonInstance(s, "io.qt.test", 1, 0, "TextSource");
		QtQml.ValueCallback valueCallback;
		QtQml.qmlRegisterSingletonType("io.qt.test", 1, 0, "AuthorSource", valueCallback = (QQmlEngine qmlEngine, QJSEngine jsEngine)->{
			QJSValue example = jsEngine.newObject();
		    example.setProperty("email", new QJSValue("alexandra@mail.com"));
		    example.setProperty("name", new QJSValue("Alexandra"));
			return example;
		});
		
		MessageBoard messageBoard = new MessageBoard();
		
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"Message {\n" + 
				"    body: MessageBody {\n" + 
				"        text: TextSource.text\n" + 
				"        onTextChanged: {messageBoard.slotOnChanged(); messageBoard.signalOnChanged(text); }\n" +
				"    }\n" + 
				"    author.name: AuthorSource.name\n" + 
				"    author.email: AuthorSource.email\n" +  
				"}");
		QQmlEngine engine = new QQmlEngine();
		String prefix = io.qt.QtUtilities.qtPrefix();
        if(new QDir(prefix+"/qml").exists()) {
        	engine.addImportPath(prefix+"/qml");
        }
		QQmlComponent component = new QQmlComponent(engine);
		engine.rootContext().setContextProperty("messageBoard", messageBoard);
		component.setData(data, null);
		Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
		Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
		QObject root = component.create();
		Assert.assertTrue(root instanceof Message);
		Message message = (Message)root;
		Assert.assertTrue(message.body() instanceof MessageBody);
		Assert.assertEquals("Hello, world!", message.body().text());
		messageBoard.hasReceived = false;
		messageBoard.receivedText = null;
		message.body().setText("newBody");
		Assert.assertTrue(messageBoard.hasReceived);
		Assert.assertEquals("newBody", messageBoard.receivedText);
		Assert.assertTrue(new QQmlProperty(message, "author").read() instanceof MessageAuthor);
		Assert.assertEquals("Alexandra", message.author().name());
		Assert.assertEquals("alexandra@mail.com", message.author().email());
		valueCallback.dispose();
	}
	
	@Test
    public void run_testClearTypeRegistrations() {
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterType(TestChild.class, "io.qt.test", 1, 0, "TestChild");
		QtQml.qmlRegisterType(TestObject.class, "io.qt.test", 1, 0, "TestObject");
		QtQml.qmlClearTypeRegistrations();
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"TestObject {\n" + 
				"}");
		QQmlEngine engine = new QQmlEngine();
		String prefix = io.qt.QtUtilities.qtPrefix();
        if(new QDir(prefix+"/qml").exists()) {
        	engine.addImportPath(prefix+"/qml");
        }
		QQmlComponent component = new QQmlComponent(engine);
		component.setData(data, null);
		Assert.assertEquals(QQmlComponent.Status.Error, component.status());
		Assert.assertEquals(":1 module \"io.qt.test\" is not installed", component.errorString().trim());
	}
	
	@Test
    public void run_testRegisterTypeNotAvailable() {
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterTypeNotAvailable("io.qt.test", 1, 0, "TestObject", "not available");
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"TestObject {\n" + 
				"}");
		QQmlEngine engine = new QQmlEngine();
		String prefix = io.qt.QtUtilities.qtPrefix();
        if(new QDir(prefix+"/qml").exists()) {
        	engine.addImportPath(prefix+"/qml");
        }
		QQmlComponent component = new QQmlComponent(engine);
		component.setData(data, null);
		Assert.assertEquals(QQmlComponent.Status.Error, component.status());
		Assert.assertEquals("No Errors", 1, component.errors().size());
		QtQml.qmlRegisterType(TestChild.class, "io.qt.test", 1, 0, "TestChild");
		QtQml.qmlRegisterType(TestObject.class, "io.qt.test", 1, 0, "TestObject");
		engine = new QQmlEngine();
        if(new QDir(prefix+"/qml").exists()) {
        	engine.addImportPath(prefix+"/qml");
        }
		component = new QQmlComponent(engine);
		component.setData(data, null);
		Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
		Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
		QObject root = component.create();
		Assert.assertTrue(root instanceof TestObject);
	}
	
	static class AttachTest extends QObject{

		AttachTest(QDeclarativeConstructor dc) throws IllegalAccessException {
			super(dc);
		}
		
		@NativeAccess
		private static AttachedType qmlAttachedProperties(QObject object){
			return new AttachedType(object);
		}
	}
	
	public static class AttachedType extends QObject{

		public AttachedType(QObject parent) {
			super(parent);
		}
		
		private int attached;

	    @QtPropertyReader(name="attached")
		public int getAttached() {
			return attached;
		}

	    @QtPropertyWriter(name="attached")
		public void setAttached(int attached) {
			this.attached = attached;
		}
	}
	
	@Test
    public void run_testProvidingAttachedProperties() {
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterType(TestChild.class, "io.qt.test", 1, 0, "TestChild");
		QtQml.qmlRegisterType(TestObject.class, "io.qt.test", 1, 0, "TestObject");
		QtQml.qmlRegisterType(AttachTest.class, "io.qt.test", 1, 0, "AttachTest");
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"TestObject {\n" + 
				"    AttachTest.attached: 5\n" +
				"}");
		QQmlEngine engine = new QQmlEngine();
		String prefix = io.qt.QtUtilities.qtPrefix();
        if(new QDir(prefix+"/qml").exists()) {
        	engine.addImportPath(prefix+"/qml");
        }
		QQmlComponent component = new QQmlComponent(engine);
		component.setData(data, null);
		Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
		Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
		QObject root = component.create();
		Assert.assertTrue(root instanceof TestObject);
		QObject attached = QtQml.qmlAttachedPropertiesObject(AttachTest.class, root);
		Assert.assertTrue(attached instanceof AttachedType);
		Assert.assertEquals(5, ((AttachedType)attached).attached);
	}
	
	@Test
    public void run_testRegisterUncreatableClass() {
		class Uncreatable extends QObject{}
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterUncreatableClass(Uncreatable.class, "io.qt.test", 1, 0, "Uncreatable", "not creatable");
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"Uncreatable {\n" + 
				"}");
		QQmlEngine engine = new QQmlEngine();
		String prefix = io.qt.QtUtilities.qtPrefix();
        if(new QDir(prefix+"/qml").exists()) {
        	engine.addImportPath(prefix+"/qml");
        }
		QQmlComponent component = new QQmlComponent(engine);
		component.setData(data, null);
		Assert.assertEquals(QQmlComponent.Status.Error, component.status());
		Assert.assertEquals("No Errors", 1, component.errors().size());
	}
	
	static class RandomNumberGenerator extends QObject implements QQmlPropertyValueSource
	{
		static RandomNumberGenerator instance;
	    RandomNumberGenerator(QDeclarativeConstructor dc)
				throws IllegalAccessException {
			super(dc);
			instance = this;
		}

		@QtPropertyNotify(name="maxValue")
	    public final Signal0 maxValueChanged = new Signal0();
	    
		public RandomNumberGenerator(QObject parent){
			super(parent);
	    }
		
	    @QtPropertyReader(name="maxValue")
	    public int maxValue() {return m_maxValue;}
	    @QtPropertyWriter(name="maxValue")
	    public void setMaxValue(int maxValue) { if(Objects.equals(this.m_maxValue, maxValue)){ this.m_maxValue = maxValue; maxValueChanged.emit(); } }

	    public void setTarget(QQmlProperty prop) {
	    	m_targetProperty = prop; updateProperty();
    	}

	    void updateProperty() {
	        m_targetProperty.write(lastNumber = random.nextInt(m_maxValue));
	    }

	    private Random random = new Random();
	    private QQmlProperty m_targetProperty = new QQmlProperty();
//	    private io.qt.core.QTimer m_timer = new io.qt.core.QTimer(); {
//			m_timer.timeout.connect(this::updateProperty);
//	        m_timer.start(500);
//		}
	    
	    private int m_maxValue = 100;
	    private int lastNumber;
	};
	
	static class PropertyValueSourceTest extends QObject{
		
		PropertyValueSourceTest(QDeclarativeConstructor dc) throws IllegalAccessException {
			super(dc);
		}
		
		private int number = -1;
		
		@QtPropertyWriter(name="number")
	    public void setNumber(int number) {
			this.number = number;
		}

		@QtPropertyReader(name="number")
	    public int number() {return number;}
	}
	
	@Test
    public void run_testPropertyValueSource() {
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterType(RandomNumberGenerator.class, "io.qt.test", 1, 0, "RandomNumberGenerator");
		QtQml.qmlRegisterType(TestChild.class, "io.qt.test", 1, 0, "TestChild");
		QtQml.qmlRegisterType(PropertyValueSourceTest.class, "io.qt.test", 1, 0, "PropertyValueSourceTest");
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"PropertyValueSourceTest {\n" + 
				"    RandomNumberGenerator on number { maxValue: 300 }\n" + 
				"}");
		QQmlEngine engine = new QQmlEngine();
		String prefix = io.qt.QtUtilities.qtPrefix();
        if(new QDir(prefix+"/qml").exists()) {
        	engine.addImportPath(prefix+"/qml");
        }
		QQmlComponent component = new QQmlComponent(engine);
		component.setData(data, null);
		Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
		Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
		QObject root = component.create();
		Assert.assertTrue(root instanceof PropertyValueSourceTest);
		PropertyValueSourceTest source = (PropertyValueSourceTest)root;
		Assert.assertTrue(RandomNumberGenerator.instance!=null);
		Assert.assertTrue(source.number>=0);
		Assert.assertTrue(source.number<300);
		Assert.assertEquals(RandomNumberGenerator.instance.lastNumber, source.number);
//		Assert.assertEquals(new QColor(Qt.GlobalColor.blue), ext.testColor());
	}
	
	@Test
    public void run_testQmlSignal() throws Exception {
		QtQml.qmlClearTypeRegistrations();
		QQmlEngine engine = new QQmlEngine();
		String prefix = io.qt.QtUtilities.qtPrefix();
        if(new QDir(prefix+"/qml").exists()) {
        	engine.addImportPath(prefix+"/qml");
        }
		QQmlComponent component = new QQmlComponent(engine);
		component.loadUrl(QUrl.fromLocalFile("classpath:io/qt/autotests/qml/RectangleMethodAndSignal.qml"));
		Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
		Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
		QQuickItem item = component.create(QQuickItem.class);
		assertEquals(0, item.property("testProperty"));
		QQmlProperty property = new QQmlProperty(item, "testProperty");
		AtomicBoolean propertySignalFired = new AtomicBoolean();
		AtomicBoolean slotInvoked = new AtomicBoolean();
		class MyObject extends QObject{
			Signal0 propertySignal = new Signal0();
			{
				propertySignal.connect(()->propertySignalFired.set(true));
			}
			@SuppressWarnings("unused")
			void slot() {
				slotInvoked.set(true);
			}
		}
		MyObject receiver = new MyObject();
		Assert.assertTrue("connectNotifySignal to propertySignal()", property.connectNotifySignal(receiver, "propertySignal()"));
		Assert.assertTrue("connectNotifySignal to slot()", property.connectNotifySignal(receiver, "slot()"));
		Assert.assertTrue(QMetaObject.findSignal(item, "testIntSignal", int.class)!=null);
		Assert.assertTrue(QMetaObject.findSignal(item, "testBoolSignal", boolean.class)!=null);
		Assert.assertTrue(QMetaObject.findSignal(item, "testRealSignal", double.class)!=null);
		Assert.assertTrue(QMetaObject.findSignal(item, "testPointSignal", QPointF.class)!=null);
		Assert.assertTrue(QMetaObject.findSignal(item, "testVarSignal", Object.class)!=null);
		Assert.assertTrue(QMetaObject.findSignal(item, "testVar2Signal", Object.class)!=null);
		Assert.assertTrue(QMetaObject.findSignal(item, "testRectangleSignal", QQuickItem.class)!=null);
		Assert.assertTrue(QMetaObject.findSignal(item, "testStringSignal", String.class)!=null);
		Assert.assertTrue(QMetaObject.findSignal(item, "testPropertyChanged")!=null);
		AtomicReference<Integer> testIntSignalResult = new AtomicReference<>();
		AtomicReference<Boolean> testBoolSignalResult = new AtomicReference<>();
		AtomicReference<Double> testDoubleSignalResult = new AtomicReference<>();
		AtomicReference<QPointF> testPointSignalResult = new AtomicReference<>();
		AtomicReference<Object> testVarSignalResult = new AtomicReference<>();
		AtomicReference<String> testStringSignalResult = new AtomicReference<>();
		AtomicBoolean testPropertyChangedFired = new AtomicBoolean();
		QMetaObject.findSignal(item, "testIntSignal", int.class).connect(testIntSignalResult::set);
		QMetaObject.findSignal(item, "testBoolSignal", boolean.class).connect(testBoolSignalResult::set);
		QMetaObject.findSignal(item, "testRealSignal", double.class).connect(testDoubleSignalResult::set);
		QMetaObject.findSignal(item, "testPointSignal", QPointF.class).connect(testPointSignalResult::set);
		QMetaObject.findSignal(item, "testVarSignal", Object.class).connect(testVarSignalResult::set);
		QMetaObject.findSignal(item, "testStringSignal", String.class).connect(testStringSignalResult::set);
		QMetaObject.findSignal(item, "testPropertyChanged").connect(()->testPropertyChangedFired.set(true));
		Assert.assertTrue(item.property("children") instanceof QQmlListProperty);
		property.write(50);
		Assert.assertTrue("propertySignalFired", propertySignalFired.get());
		Assert.assertTrue("slotInvoked", slotInvoked.get());
		Assert.assertEquals("testIntSignal", Integer.valueOf(50), testIntSignalResult.get());
		Assert.assertEquals("testBoolSignal", 50>0, testBoolSignalResult.get());
		Assert.assertEquals("testDoubleSignal", Double.valueOf(50), testDoubleSignalResult.get());
		Assert.assertEquals("testPointSignal", new QPointF(50, 50), testPointSignalResult.get());
		Assert.assertEquals("testStringSignal", "string 50", testStringSignalResult.get());
		Assert.assertTrue("testPropertyChangedFired", testPropertyChangedFired.get());
		Assert.assertEquals("testVarSignal", new QRectF(0, 0, 50, 50), testVarSignalResult.get());

		Assert.assertTrue(QMetaObject.findSignal(item, "testFunctionCall", Object.class)!=null);
		AtomicReference<Object> testFunctionCallFired = new AtomicReference<>();
		QMetaObject.findSignal(item, "testFunctionCall", Object.class).connect(testFunctionCallFired::set);
		QMetaMethod testFunction = item.metaObject().method("testFunction", Object.class);
		Assert.assertTrue("testFunction exists", testFunction!=null);
		testFunction.invoke(item, 5);
		Assert.assertEquals("testFunctionCall", 5, testFunctionCallFired.get());
		testFunction.invoke(item, 5.5);
		Assert.assertEquals("testFunctionCall", 5.5, testFunctionCallFired.get());
		testFunction.invoke(item, new QColor(Qt.GlobalColor.darkBlue));
		Assert.assertEquals("testFunctionCall", new QColor(Qt.GlobalColor.darkBlue), testFunctionCallFired.get());
		Runnable r = ()->{};
		testFunction.invoke(item, r);
		Assert.assertEquals("testFunctionCall", r, testFunctionCallFired.get());
	}
	
	@Test
    public void run_testUnknownEnums() {
		QtQml.qmlClearTypeRegistrations();
		QByteArray data = new QByteArray("import QtQuick 2.11\n" + 
				"TextInput {\n" + 
				"    \n" + 
				"    \n" + 
				"    \n" + 
				"}");
		QQmlEngine engine = new QQmlEngine();
		String prefix = io.qt.QtUtilities.qtPrefix();
        if(new QDir(prefix+"/qml").exists()) {
        	engine.addImportPath(prefix+"/qml");
        }
		QQmlComponent component = new QQmlComponent(engine);
		component.setData(data, null);
		Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
		Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
		QObject item = component.create();
		AbstractPrivateSignal1<Integer> signal = QMetaObject.findSignal(item, "mouseSelectionModeChanged", int.class);
		Assert.assertTrue(signal!=null);
		List<Object> list = new ArrayList<>();
		signal.connect(i->{
			list.add(i);
		});
		item.setProperty("mouseSelectionMode", 1);
		item.setProperty("mouseSelectionMode", 0);
		Assert.assertEquals(Arrays.asList(1, 0), list);
	}
	
	public static class MultiInterfaceObject extends QObject implements QRunnable, QGraphicsItem
	{
		@NativeAccess
		private MultiInterfaceObject(QDeclarativeConstructor dc) throws IllegalAccessException {
			super(dc);
		}

		@Override
		public QRectF boundingRect() {
			return null;
		}

		@Override
		public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {
		}

		@Override
		public void run() {
		}
	};
	
	/*
	@Test
    public void run_testMultiInterfaceDeletion() {
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterType(MultiInterfaceObje)ct.class, "io.qt.test", 1, 0, "MultiInterfaceObject");
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"import QtQuick 2.0\n" + 
				"MultiInterfaceObject {\n" + 
				"    id: test\n" + 
				"}");
		QQmlComponent component = new QQmlComponent(new QQmlEngine());
		component.setData(data, null);
		MultiInterfaceObject root = component.create(MultiInterfaceObject.class);
		QThread thread = new QThread();
		thread.start();
		QGraphicsScene scene = new QGraphicsScene();
		scene.addItem(root);
		root.moveToThread(thread);
		QThreadPool pool = new QThreadPool();
		pool.start(root);
		QApplication.execStatic();
	}*/
}
