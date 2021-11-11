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

import static org.junit.Assume.assumeTrue;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtUtilities;
import io.qt.core.QCoreApplication;
import io.qt.core.QDir;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaObject.AbstractPrivateSignal1;
import io.qt.core.QObject;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QPluginLoader;
import io.qt.core.QUrl;
import io.qt.core.Qt;
import io.qt.gui.QGuiApplication;
import io.qt.qml.QQmlComponent;
import io.qt.qml.QQmlEngine;
import io.qt.quick.QQuickItem;
import io.qt.webview.QtWebView;

public class TestWebView extends QApplicationTest {
    @BeforeClass
    public static void testInitialize() throws Exception {
    	if(QOperatingSystemVersion.currentType()==QOperatingSystemVersion.OSType.MacOS)
    		QCoreApplication.setAttribute(Qt.ApplicationAttribute.AA_ShareOpenGLContexts);
    	boolean found = false;
        QApplicationTest.testInitialize();
        assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
        try {
    		QtUtilities.initializePackage("io.qt.quick");
    		QtUtilities.initializePackage("io.qt.webengine");
    		QtUtilities.initializePackage("io.qt.webengine.core");
    		QtUtilities.initializePackage("io.qt.webchannel");
    		io.qt.QtUtilities.loadQtLibrary("QtPositioning");
    		QtWebView.initialize();
    		QPluginLoader pluginLoader = new QPluginLoader("webview/qtwebview_webengine");
    		if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    			pluginLoader = new QPluginLoader("webview/qtwebview_webengined");
    		}
    		if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    			pluginLoader = new QPluginLoader("webview/qtwebview_webengine_debug");
    		}
    		if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    			pluginLoader = new QPluginLoader("webview/qtwebview_darwin");
    		}
    		if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    			pluginLoader = new QPluginLoader("webview/qtwebview_darwin_debug");
    			pluginLoader.load();
    		}
			found = pluginLoader.isLoaded();
		} catch (Throwable e) {
		}
        assumeTrue(found);
    }

    @Test
    public void test() {
    	QQmlEngine engine = new QQmlEngine();
		QQmlComponent component = new QQmlComponent(engine);
	    component.setData(
	                "import QtQuick 2.5\n"+
                	"import QtWebView 1.1\n"+
	                "WebView {}",
	                new QUrl());
		Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
		Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
	    QQuickItem object = component.create(QQuickItem.class);
	    Assert.assertTrue(object!=null);
	    AbstractPrivateSignal1<QObject> loadingChanged = QMetaObject.findSignal(object, "loadingChanged", QObject.class);
	    loadingChanged.connect(o->{System.out.println(o);});
	    object.setProperty("url", new QUrl("http://www.qt.io"));
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestWebView.class.getName());
    }
}
