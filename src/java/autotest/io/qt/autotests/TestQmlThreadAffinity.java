/****************************************************************************
**
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import static org.junit.Assert.fail;

import org.junit.Test;

import io.qt.*;
import io.qt.core.*;
import io.qt.qml.*;

public class TestQmlThreadAffinity extends QApplicationTest {
	
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
		component2.setData(new QByteArray("import QtQuick 2.0\n QtObject {}"), null);
		QQmlIncubator incubator = new QQmlIncubator(QQmlIncubator.IncubationMode.Asynchronous);
		
		QEventLoop loop = new QEventLoop();
		QThread thread = QThread.create(()->{
			try{
				new QQmlContext(engine);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
//			try{
				component.setData(new QByteArray("import QtQuick 2.0\n Item {}"), null);
//				fail("QThreadAffinityException expected to be thrown.");
//			}catch(QThreadAffinityException e) {}
//			try{
			    component2.create();
//				fail("QThreadAffinityException expected to be thrown.");
//			}catch(QThreadAffinityException e) {}
//			try{
			    component.create(context);
//				fail("QThreadAffinityException expected to be thrown.");
//			}catch(QThreadAffinityException e) {}
			    
			    component.create(incubator);
			    
			    QQmlIncubationController incubationController = new QQmlIncubationController.Impl();
				engine.setIncubationController(incubationController);
				QQmlIncubationController.WhileFlag flag = new QQmlIncubationController.WhileFlag(true);
				incubationController.incubateWhile(flag);
				incubator.forceCompletion();
				incubator.object();
		});
		thread.setUncaughtExceptionHandler((Thread t, Throwable e)->throwable = e);
		thread.finished.connect(loop::quit);
		thread.start();
		loop.exec();
		if(throwable!=null)
			throw throwable;
	}
	
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQmlThreadAffinity.class.getName());
    }
}
