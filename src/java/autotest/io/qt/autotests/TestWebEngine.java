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

import static org.junit.Assume.*;

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QCoreApplication;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QTimer;
import io.qt.core.QUrl;
import io.qt.core.Qt;
import io.qt.gui.QGuiApplication;
import io.qt.webengine.QtWebEngine;
import io.qt.webengine.widgets.QWebEngineView;
import io.qt.widgets.QApplication;

public class TestWebEngine extends QApplicationTest {
    @BeforeClass
    public static void testInitialize() throws Exception {
    	QtWebEngine.initialize();
    	if(QOperatingSystemVersion.currentType()==QOperatingSystemVersion.OSType.MacOS)
    		QCoreApplication.setAttribute(Qt.ApplicationAttribute.AA_ShareOpenGLContexts);
        QApplicationTest.testInitialize();
    	assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
    	boolean found = false;
    	try {
			Class<?> cls = Class.forName("io.qt.webengine.widgets.QWebEngineView");
			if (cls != null)
				found = true;
		} catch (ClassNotFoundException e) {
		}
    	assumeTrue("QWebEngineView not available.", found);
    }

    @Test
    public void test() {
    	QWebEngineView webView = new QWebEngineView();
    	webView.setUrl(new QUrl("http://www.qt.io"));
    	webView.show();
    	webView.loadFinished.connect(b -> QTimer.singleShot(10, ()->{webView.close(); QApplication.quit();}) );
    	QApplication.exec();
    	webView.dispose();
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestWebEngine.class.getName());
    }
}
