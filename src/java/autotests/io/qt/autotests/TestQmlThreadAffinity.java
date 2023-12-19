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

import static org.junit.Assert.fail;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QThreadAffinityException;
import io.qt.QtUtilities;
import io.qt.core.QByteArray;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QEventLoop;
import io.qt.core.QObject;
import io.qt.core.QThread;
import io.qt.core.QUrl;
import io.qt.qml.QQmlComponent;
import io.qt.qml.QQmlContext;
import io.qt.qml.QQmlEngine;
import io.qt.qml.QQmlIncubationController;
import io.qt.qml.QQmlIncubator;
import io.qt.quick.QQuickItem;

public class TestQmlThreadAffinity extends ApplicationInitializer{
	
	static {
		System.setProperty("io.qt.enable-thread-affinity-check", "true");
		System.setProperty("io.qt.enable-event-thread-affinity-check", "true");
	}
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithGui();
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
		thread.join();
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
	}
	
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQmlThreadAffinity.class.getName());
    }
}
