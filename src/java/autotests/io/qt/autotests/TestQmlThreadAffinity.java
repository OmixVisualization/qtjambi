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

import static org.junit.Assert.fail;

import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QThreadAffinityException;
import io.qt.QtUtilities;
import io.qt.core.QByteArray;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QEventLoop;
import io.qt.core.QMetaObject;
import io.qt.core.QObject;
import io.qt.core.QThread;
import io.qt.core.QTimer;
import io.qt.core.QUrl;
import io.qt.core.Qt;
import io.qt.qml.QJSEngine;
import io.qt.qml.QJSValue;
import io.qt.qml.QQmlComponent;
import io.qt.qml.QQmlContext;
import io.qt.qml.QQmlEngine;
import io.qt.qml.QQmlIncubationController;
import io.qt.qml.QQmlIncubator;
import io.qt.qml.QtQml;
import io.qt.quick.QQuickItem;
import io.qt.quick.QQuickWindow;

public class TestQmlThreadAffinity extends ApplicationInitializer{
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithGui();
    	QtUtilities.setThreadAffinityCheckEnabled(true);
    	QtUtilities.setEventThreadAffinityCheckEnabled(true);
    }
	
	@AfterClass
    public static void testDispose() throws Exception {
    	QtUtilities.setThreadAffinityCheckEnabled(false);
    	QtUtilities.setEventThreadAffinityCheckEnabled(false);
    	ApplicationInitializer.testDispose();
	}
	
	private Throwable throwable;
	
	@Test
    public void testQQml() throws Throwable{
		throwable = null;
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
		QQmlEngine engine = new QQmlEngine();
		QQmlComponent component = new QQmlComponent(engine);
		QQmlComponent component2 = new QQmlComponent(engine);
		QQmlContext context = new QQmlContext(engine);
		component2.setData(new QByteArray("import QtQuick 2.0\n QtObject {}"), (QUrl)null);
		QQmlIncubator incubator = new QQmlIncubator(QQmlIncubator.IncubationMode.Asynchronous);
		
		QEventLoop loop = new QEventLoop();
		QObject[] objects = new QObject[3];
		QThread thread = QThread.create(()->{
			try{
				new QQmlContext(engine);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			component.setData(new QByteArray("import QtQuick 2.0\n Item {}"), (QUrl)null);
//			try{
			objects[0] = component2.create();
//				fail("QThreadAffinityException expected to be thrown.");
//			}catch(QThreadAffinityException e) {}
//			try{
			objects[1] = component.create(context);
//				fail("QThreadAffinityException expected to be thrown.");
//			}catch(QThreadAffinityException e) {}
			    
			    component.create(incubator);
			    
			    QQmlIncubationController incubationController = new QQmlIncubationController.Impl();
				engine.setIncubationController(incubationController);
				QQmlIncubationController.WhileFlag flag = new QQmlIncubationController.WhileFlag(true);
				incubationController.incubateWhile(flag);
				incubator.forceCompletion();
				objects[2] = incubator.object();
		});
		thread.setUncaughtExceptionHandler((Thread t, Throwable e)->throwable = e);
		thread.finished.connect(loop::quit);
		thread.start();
		loop.exec();
		if(throwable!=null)
			throw throwable;
		thread.join(5000);
		Assert.assertTrue(objects[0] instanceof QObject);
		Assert.assertTrue(objects[1] instanceof QQuickItem);
		Assert.assertTrue(objects[2] instanceof QQuickItem);
		try{
			QCoreApplication.sendPostedEvents(objects[0]);
			fail("QThreadAffinityException expected to be thrown.");
		}catch(QThreadAffinityException e) {}
		try{
			QCoreApplication.sendPostedEvents(objects[1]);
			fail("QThreadAffinityException expected to be thrown.");
		}catch(QThreadAffinityException e) {}
		try{
			QCoreApplication.sendPostedEvents(objects[2]);
			fail("QThreadAffinityException expected to be thrown.");
		}catch(QThreadAffinityException e) {}
		try{
			QCoreApplication.sendEvent(objects[1], new QEvent(QEvent.Type.AcceptDropsChange));
			fail("QThreadAffinityException expected to be thrown.");
		}catch(QThreadAffinityException e) {}
		thread.dispose();
		engine.dispose();
	}
	
	@Test
    public void testQJSValue() throws Throwable{
		Throwable[] exn = {null};
		QtQml.qmlClearTypeRegistrations();
		QQmlEngine engine = new QQmlEngine();
		try {
			QQmlComponent component = new QQmlComponent(engine);
			component.setData(new QByteArray("import QtQuick 2.0\n Item {property var array: [1,2,3,4,5]}"), (QUrl)null);
			QObject object = component.create();
			Object array = object.property("array");
			Assert.assertTrue(array instanceof QJSValue);
			QJSValue myObject = (QJSValue)array;
			Assert.assertTrue(myObject.isArray());
			Runnable r = ()->{
				try {
					try {
						object.property("array");
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					try {
						engine.evaluate("");
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					try {
						engine.fromScriptValue(new QJSValue(), null);
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					try {
						engine.installExtensions(QJSEngine.Extension.ConsoleExtension.asFlags());
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					try {
						engine.newArray();
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					try {
						engine.newErrorObject(QJSValue.ErrorType.EvalError);
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					try {
						engine.newObject();
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					try {
						engine.newQMetaObject(QObject.staticMetaObject);
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					try {
						engine.newQObject(object);
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					try {
						engine.toScriptValue(object);
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					try {
						engine.singletonInstance(0);
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					try {
						engine.throwError("");
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					try {
						engine.clearComponentCache();
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					engine.collectGarbage();
				}catch(Throwable e){
					exn[0] = e;
				}
			};
			QThread thread1 = QThread.create(r);
			thread1.start();
			thread1.join(5000);
			if(exn[0]!=null)
				throw exn[0];
		}finally {
			engine.dispose();
		}
	}
	
	static class PropertyCarrier extends QObject{
		private int count;

		public int getCount() {
			return count;
		}

		public void setCount(int count) {
			if(this.count != count) {
				this.count = count;
				countChanged.emit();
			}
		}
		
		public final Signal0 countChanged = new Signal0();
	}
	
	@Test
    public void testPropertyNotifier() throws Throwable{
		QtQml.qmlClearTypeRegistrations();
		QQmlEngine engine = new QQmlEngine();
		PropertyCarrier propertyCarrier = new PropertyCarrier();
		QTimer timer = new QTimer();
		timer.timeout.connect(()->{
			propertyCarrier.setCount(propertyCarrier.getCount()+1);
		}, Qt.ConnectionType.DirectConnection);
		timer.setInterval(50);
		QThread thread = new QThread();
		timer.moveToThread(thread);
		thread.started.connect(timer, QTimer::start, Qt.ConnectionType.DirectConnection);
		thread.finished.connect(timer, QTimer::stop, Qt.ConnectionType.DirectConnection);
		engine.rootContext().setContextProperty("propertyCarrier", propertyCarrier);
		QQmlComponent component = new QQmlComponent(engine);
		component.setData(new QByteArray("import QtQml 2.15\nimport QtQuick 2.15\nimport QtQuick.Window 2.15\n Window { Text { id: txt; text: \"\" + propertyCarrier.count } }"), (QUrl)null);
		Assert.assertFalse(component.errorString(), component.isError());
		QQuickWindow object = component.create(QQuickWindow.class);
		object.show();
		QEventLoop loop = new QEventLoop();
		QMetaObject.invokeMethod(thread, (QMetaObject.Slot1<QThread>)QThread::start, Qt.ConnectionType.QueuedConnection);
		QTimer.singleShot(1000, loop::quit);
		loop.exec();
		timer = null;
		thread.quit();
	}
	
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQmlThreadAffinity.class.getName());
    }
}
