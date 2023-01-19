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

import static org.junit.Assume.assumeTrue;

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtInvokable;
import io.qt.QtUtilities;
import io.qt.core.QByteArray;
import io.qt.core.QCoreApplication;
import io.qt.core.QList;
import io.qt.core.QMetaMethod;
import io.qt.core.QObject;
import io.qt.core.Qt;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QOpenGLContext;
import io.qt.qml.QQmlApplicationEngine;
import io.qt.webengine.QtWebEngine;
import io.qt.widgets.QApplication;

public class TestWebEngineQuickQt5 extends ApplicationInitializer {
	
    @BeforeClass
    public static void testInitialize() throws Exception {
    	QtUtilities.initializePackage("io.qt.webengine");
        QCoreApplication.setAttribute(Qt.ApplicationAttribute.AA_ShareOpenGLContexts);
        ApplicationInitializer.testInitializeWithGui();
    	assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
    	boolean found = false;
    	try {
			Class<?> cls = Class.forName("io.qt.webengine.QtWebEngine");
			if (cls != null)
				found = true;
		} catch (ClassNotFoundException e) {
		}
    	assumeTrue("QtWebEngine not available.", found);
    	assumeTrue("global share context not available.", QOpenGLContext.globalShareContext()!=null);
        QtWebEngine.initialize();
        QtUtilities.initializePackage("io.qt.quick");
        QtUtilities.loadQtLibrary("QuickControls2");
    }
    
    public static class UIController extends QObject {
        @QtInvokable public void passWebView(Object obj) {
            System.out.println("got " + obj.getClass().getName() + ": " + obj);
            QApplication.quit();
        }
    }
    
    @Test
    public void test1() {
    	QQmlApplicationEngine engine = new QQmlApplicationEngine();
    	engine.loadData(new QByteArray("import QtWebEngine 1.0\nWebEngineView{}"));
    	engine.setOutputWarningsToStandardError(true);
    	engine.warnings.connect(e -> System.out.println(e));
        QList<QObject> rootObjects = engine.rootObjects();
        dump(rootObjects, 0);
    }

    private static void dump(QList<QObject> objects, int nestedLevel) {
    	final StringBuilder suffix = new StringBuilder();
    	for (int i = 0; i < nestedLevel; i++) {
    		suffix.append("    ");
		}
    	for (QObject obj : objects) {
    		System.out.println(suffix + "| " + obj.getClass().getName() + ": " + obj);
    		obj.metaObject().methods().forEach(mtd -> {
//    			System.out.println(suffix + "     "+mtd.cppMethodSignature());
    			if(mtd.methodType()==QMetaMethod.MethodType.Signal) {
    				System.out.println(suffix + "     "+mtd.toSignal(obj));
    			}
    		});
    		dump(obj.children(), nestedLevel+1);
		}
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestWebEngineQuickQt5.class.getName());
    }
}
