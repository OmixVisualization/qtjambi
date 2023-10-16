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

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtEnumerator;
import io.qt.autotests.generated.General;
import io.qt.core.QCoreApplication;
import io.qt.core.QLibraryInfo;
import io.qt.core.QMetaObject;
import io.qt.core.QObject;
import io.qt.core.QTimer;
import io.qt.core.QUrl;
import io.qt.core.Qt;
import io.qt.gui.QGuiApplication;
import io.qt.qml.QQmlApplicationEngine;
import io.qt.qml.QQmlComponent;
import io.qt.quick.QQuickItem;
import io.qt.quick.QQuickWindow;
import io.qt.webview.QtWebView;
import io.qt.widgets.QApplication;

public class TestWebView extends ApplicationInitializer {
    @BeforeClass
    public static void testInitialize() throws Exception {
		QCoreApplication.setAttribute(Qt.ApplicationAttribute.AA_UseSoftwareOpenGL);
    	General.canVulkan();// making sure it initializes
		QtWebView.initialize();
		Thread timeoutThread = new Thread(()->{
			try {
				synchronized(TestWebView.class) {
					TestWebView.class.wait(5000);
				}
				System.err.println("TestWebView does not initialize. Forced termination...");
				General.terminate();
			} catch (InterruptedException e) {
			}
		});
		timeoutThread.setDaemon(true);
		timeoutThread.start();
        ApplicationInitializer.testInitializeWithGui();
        timeoutThread.interrupt();
        assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
    }

    @Test
    public void test() {
    	QQmlApplicationEngine engine = new QQmlApplicationEngine();
		QQmlComponent component = new QQmlComponent(engine);
	    component.setData(
	                  "import QtQuick 2.5;"
	                + "import QtQuick.Controls 2.0;"
	                + "import QtQuick.Layouts 1.1;"
	                + "import QtWebView 1.1;"
	                + "ApplicationWindow {"
	                + "visible: true;"
	                + "WebView {"
	                + "anchors.fill: parent;"
	                + "url: \"http://www.qtjambi.io\";"
	                + "}"
	                + "}",
	                new QUrl());
		Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
		Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
		QQuickWindow window = component.create(QQuickWindow.class);
	    Assert.assertTrue(window!=null);
	    try {
		    QQuickItem rootItem = window.contentItem();
		    QQuickItem applicationWindow = rootItem.findChild(QQuickItem.class, "ApplicationWindow");
		    QQuickItem webview = applicationWindow.childItems().first();
		    Assert.assertTrue(webview!=null);
		    QMetaObject.AbstractPrivateSignal1<QObject> signal = QMetaObject.findSignal(webview, "loadingChanged", QObject.class);
		    Assert.assertTrue(signal!=null);
		    Object[] status = {null, null};
		    signal.connect(request -> {
		    	status[0] = request.property("errorString");
		    	status[1] = request.property("status");
			    QTimer.singleShot(3000, QApplication::exit);
//	    		QApplication.exit();
		    });
		    QTimer.singleShot(30000, QApplication::exit);
		    QApplication.exec();
		    Assert.assertTrue(status[0]==null ? "no website loaded" : status[0].getClass().getName(), status[0] instanceof String);
		    if(QLibraryInfo.version().majorVersion()>=6) {
		    	Assert.assertTrue(status[1]==null ? "null" : status[1].getClass().getName(), status[1] instanceof QtEnumerator);
		    }else {
		    	Assert.assertTrue(status[1]==null ? "null" : status[1].getClass().getName(), status[1] instanceof Integer);
		    }
	    }finally {
	    	window.dispose();
	    }
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestWebView.class.getName());
    }
}
