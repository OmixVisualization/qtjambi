/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import io.qt.QThreadAffinityException;
import io.qt.QtUtilities;
import io.qt.core.QTimer;
import io.qt.core.QLibraryInfo;
import io.qt.core.QObject;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QThread;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QWindow;
import io.qt.qml.QJSManagedValue;
import io.qt.qml.QJSValue;
import io.qt.qml.QQmlEngine;
import io.qt.qml.QtQml;

public class TestQmlThreadAffinityQt6 extends ApplicationInitializer{
	
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
	
	@Test
    public void testQJSManagedValue() throws Throwable{
		QtQml.qmlClearTypeRegistrations();
		QQmlEngine engine = new QQmlEngine();
		QObject obj = new QObject();
		try {
			QJSManagedValue mv = new QJSManagedValue(engine.newQObject(obj), engine);
			Throwable[] exn = {null};
			Runnable r = ()->{
				try {
					try {
						mv.toVariant();
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					try {
						mv.jsMetaInstantiate();
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					try {
						mv.jsMetaType();
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					try {
						mv.call();
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					try {
						mv.callAsConstructor();
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					try {
						mv.callWithInstance(new QJSValue());
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					try {
						new QJSManagedValue(0, engine);
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					try {
						engine.throwError(new QJSValue());
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					try {
						engine.fromManagedValue(mv, QObject.class);
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					try {
						engine.importModule("");
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					try {
						engine.registerModule("", "");
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					try {
						engine.toManagedValue("TEST");
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					try {
						new QJSValue(mv);
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					try {
						engine.clearSingletons();
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					try {
						engine.catchError();
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					try {
						engine.newSymbol("");
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
					
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
	
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQmlThreadAffinityQt6.class.getName());
    }
}
