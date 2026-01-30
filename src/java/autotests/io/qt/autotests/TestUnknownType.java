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

import static org.junit.Assume.assumeTrue;

import java.util.Set;
import java.util.TreeSet;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QByteArray;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QList;
import io.qt.core.QObject;
import io.qt.core.QTimer;
import io.qt.core.Qt;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QOpenGLContext;
import io.qt.qml.QQmlApplicationEngine;
import io.qt.quick.QQuickWindow;
import io.qt.quick.QSGRendererInterface;
import io.qt.webengine.core.QWebEngineProfile;
import io.qt.webengine.core.QWebEngineSettings;
import io.qt.widgets.QApplication;

public class TestUnknownType extends ApplicationInitializer{
	
	@BeforeClass
	public static void testInitialize() throws Exception {
    	Assert.assertTrue(io.qt.QtUtilities.initializePackage("io.qt.webengine.quick"));
        QCoreApplication.setAttribute(Qt.ApplicationAttribute.AA_ShareOpenGLContexts);
        QQuickWindow.setGraphicsApi(QSGRendererInterface.GraphicsApi.OpenGLRhi);
        io.qt.webengine.quick.QtWebEngineQuick.initialize();
		ApplicationInitializer.testInitializeWithGui();
    	assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
    	assumeTrue("global share context not available.", QOpenGLContext.globalShareContext()!=null);
    	QWebEngineProfile.defaultProfile().settings().setAttribute(QWebEngineSettings.WebAttribute.PluginsEnabled, true);
        QWebEngineProfile.defaultProfile().settings().setAttribute(QWebEngineSettings.WebAttribute.DnsPrefetchEnabled, true);
        QWebEngineProfile.defaultProfile().setPersistentCookiesPolicy(QWebEngineProfile.PersistentCookiesPolicy.AllowPersistentCookies);
    }
	
    @Test
    public void test() {
    	Set<String> metaObjects = new TreeSet<>();
//    	QLogging.qInstallLoggingMessageHandler(QtMsgType.QtWarningMsg);
    	QGuiApplication app = QGuiApplication.instance();
    	QObject filter = new QObject(app){
    		@Override
    		public boolean eventFilter(QObject obj, QEvent evt) {
    			String className = obj.metaObject().className();
    			if("QGuiApplication".equals(className)) {
    				if(obj!=app)
    					System.err.println("new instance of QGuiApplication: "+System.identityHashCode(obj));
    			}
    			if(!metaObjects.contains(className)) {
    				metaObjects.add(className);
		    		System.out.println(className+" -> "+obj.getClass().getName());
    			}
    			return false;
    		}
    	};
    	QGuiApplication.instance().installEventFilter(filter);
        QQmlApplicationEngine engine = new QQmlApplicationEngine();
    	try {
	    	engine.loadData(new QByteArray(
	    			"import QtQuick\n" + 
	    			"import QtQuick.Controls\n" + 
	    			"import QtWebEngine\n" + 
	    			"\n" + 
	    			"ApplicationWindow {\n" + 
	    			"    visible: true\n" + 
	    			"    width: 500\n" + 
	    			"    height: 400\n" + 
	    			"    WebEngineView {\n" + 
	    			"        id: webView\n" + 
	    			"        anchors.fill: parent\n" + 
	    			"        url: \"http://www.cern.ch\"\n" + 
	    			"    }\n" + 
	    			"}"
	    			));
	        QList<QObject> rootObjects = engine.rootObjects();
	        Assert.assertTrue(!rootObjects.isEmpty());
	        Assert.assertTrue(rootObjects.get(0)!=null);
	    	QTimer.singleShot(2000, QApplication::quit);
	    	QApplication.exec();
	    	Assert.assertTrue(metaObjects.contains("QtWebEngineCore::RenderWidgetHostViewQtDelegateItem"));
    	}finally {
//    		QGuiApplication.instance().removeEventFilter(filter);
    		engine.destroyed.connect(o->System.out.println("destroyed: "+o.getClass()));
	    	engine.dispose();
    	}
//    	System.err.hashCode();
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestUnknownType.class.getName());
    }
}
