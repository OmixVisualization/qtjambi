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

import org.junit.*;

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtInvokable;
import io.qt.QtUtilities;
import io.qt.autotests.generated.General;
import io.qt.core.*;
import io.qt.gui.*;
import io.qt.qml.*;
import io.qt.webengine.core.*;
import io.qt.webengine.quick.*;
import io.qt.widgets.*;

public class TestWebEngineQuickQt6 extends ApplicationInitializer {
	
    @BeforeClass
    public static void testInitialize() throws Exception {
    	QtUtilities.initializePackage("io.qt.webengine.quick");
        QCoreApplication.setAttribute(Qt.ApplicationAttribute.AA_ShareOpenGLContexts);
    	boolean found = false;
    	try {
			Class<?> cls = Class.forName("io.qt.webengine.quick.QtWebEngineQuick");
			if (cls != null)
				found = true;
		} catch (ClassNotFoundException e) {
		}
    	assumeTrue("QWebEngineView not available.", found);
        QtWebEngineQuick.initialize();
        ApplicationInitializer.testInitializeWithGui();
    	QLogging.qInstallLoggingMessageHandler(QtMsgType.QtWarningMsg);
    	assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
    	assumeTrue("global share context not available.", QOpenGLContext.globalShareContext()!=null);
    	QWebEngineProfile.defaultProfile().settings().setAttribute(QWebEngineSettings.WebAttribute.PluginsEnabled, true);
        QWebEngineProfile.defaultProfile().settings().setAttribute(QWebEngineSettings.WebAttribute.DnsPrefetchEnabled, true);
        QWebEngineProfile.defaultProfile().setPersistentCookiesPolicy(QWebEngineProfile.PersistentCookiesPolicy.AllowPersistentCookies);
        QtUtilities.initializePackage("io.qt.quick");
        QtUtilities.loadQtLibrary("QuickControls2");
    }
    
    @Test
    public void test1() {
    	QQmlApplicationEngine engine = new QQmlApplicationEngine();
    	engine.loadData(new QByteArray("import QtWebEngine\nWebEngineView{\n"
    			+ "url: \"http://info.cern.ch/\"\n"
    			+ "function doRunJavaScript(code,consumer){\n"
    			+ "runJavaScript(code, value => consumer.accept(value));\n"
    			+ "}\n"
    			+ "}"));
        QList<QObject> rootObjects = engine.rootObjects();
        Assert.assertTrue(rootObjects.get(0)!=null);
        dump(rootObjects, 0);
        QObject webEngineView = rootObjects.get(0);
//        webEngineView.metaObject().methods().forEach(m->System.out.println(m.cppMethodSignature()));
        QMetaMethod mtd = webEngineView.metaObject().method("doRunJavaScript", Object.class, Object.class);
        Assert.assertTrue(mtd!=null && mtd.isValid());
        QMetaObject.AbstractPrivateSignal4<?,?,?,?> javaScriptConsoleMessage = (QMetaObject.AbstractPrivateSignal4<?,?,?,?>)webEngineView.metaObject().method("javaScriptConsoleMessage(JavaScriptConsoleMessageLevel,QString,int,QString)").toSignal(webEngineView);
        javaScriptConsoleMessage.connect((a,b,c,d)->{
        	System.out.println(a+" "+b+" "+c+" "+d);
        });
        QJSValue received[] = {null};
        QObject consumer = new QObject() {
			@QtInvokable
			public void accept(QJSValue value) {
				received[0] = value;
			}
        };
//        QMetaObject.forType(consumer.getClass()).methods().forEach(m->System.out.println(m.cppMethodSignature()+" "+m.attributes()));
        mtd.invoke(webEngineView, "window", engine.newQObject(consumer));
//        QCoreApplication.processEvents();
        QObject settings = (QObject)webEngineView.property("settings");
        QObject userScripts = (QObject)webEngineView.property("userScripts");
        QWebEngineSettings _settings = QtWebEngineQuick.toWebEngineSettings(settings);
        QWebEngineScriptCollection _userScripts = QtWebEngineQuick.toWebEngineScriptCollection(userScripts);
        Assert.assertTrue(_settings!=null);
        Assert.assertTrue(_userScripts!=null);
        Assert.assertTrue(General.internalAccess.isSplitOwnership(_settings));
        Assert.assertTrue(General.internalAccess.isSplitOwnership(_userScripts));
        boolean value = QVariant.toBoolean(settings.property("autoLoadImages"));
        _settings.setAttribute(QWebEngineSettings.WebAttribute.AutoLoadImages, !value);
        Assert.assertEquals(!value, settings.property("autoLoadImages"));
        settings.setProperty("autoLoadImages", value);
        Assert.assertEquals(value, _settings.testAttribute(QWebEngineSettings.WebAttribute.AutoLoadImages));
        Assert.assertEquals(0, _userScripts.count());
        _userScripts.insert(new QWebEngineScript());
        Assert.assertEquals(1, _userScripts.count());
        userScripts.metaObject().method("insert", QWebEngineScript.class).invoke(userScripts, new QWebEngineScript());
        Assert.assertEquals(2, _userScripts.count());
        _settings.dispose();
        Assert.assertFalse(settings.isDisposed());
        settings.property("autoLoadImages");// should not crash
    	QTimer.singleShot(2000, QApplication::quit);
    	QApplication.exec();
    	Assert.assertTrue(received[0]!=null);
        engine.dispose();
        Assert.assertTrue(webEngineView.isDisposed());
        Assert.assertTrue(userScripts.isDisposed());
        Assert.assertTrue(_userScripts.isDisposed());
        Assert.assertTrue(settings.isDisposed());
    }

    private static void dump(QList<QObject> objects, int nestedLevel) {
    	StringBuilder suffix = new StringBuilder();
    	for (int i = 0; i < nestedLevel; i++) {
    		suffix.append("    ");
		}
    	for (QObject obj : objects) {
//    		System.out.println(suffix + "| " + obj.getClass().getName() + ": " + obj);
    		obj.metaObject().methods().forEach(mtd -> {
    			if(mtd.methodType()==QMetaMethod.MethodType.Signal) {
    				mtd.toSignal(obj); // must not crash for nonconstructible metatypes
    			}
    		});
    		dump(obj.children(), nestedLevel+1);
		}
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestWebEngineQuickQt6.class.getName());
    }
}
