/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtUtilities;
import io.qt.core.*;
import io.qt.gui.*;
import io.qt.webengine.core.*;
import io.qt.webengine.widgets.*;
import io.qt.widgets.*;

public class TestWebEngineWidgets extends ApplicationInitializer {
	
    @BeforeClass
    public static void testInitialize() throws Exception {
    	QtUtilities.initializePackage("io.qt.webengine.widgets");
        ApplicationInitializer.testInitializeWithWidgets();
    	assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
    	boolean found = false;
    	try {
			Class<?> cls = Class.forName("io.qt.webengine.widgets.QWebEngineView");
			if (cls != null)
				found = true;
		} catch (ClassNotFoundException e) {
		}
    	assumeTrue("QWebEngineView not available.", found);
    	assumeTrue("global share context not available.", QOpenGLContext.globalShareContext()!=null);
    }

    @Test
    public void test() {
    	QWebEngineProfile.defaultProfile().settings().setAttribute(QWebEngineSettings.WebAttribute.PluginsEnabled, true);
        QWebEngineProfile.defaultProfile().settings().setAttribute(QWebEngineSettings.WebAttribute.DnsPrefetchEnabled, true);
        QWebEngineProfile.defaultProfile().setPersistentCookiesPolicy(QWebEngineProfile.PersistentCookiesPolicy.AllowPersistentCookies);
    	QWebEngineView webView = new QWebEngineView();
    	webView.load(new QUrl("http://www.qt.io"));
    	webView.loadProgress.connect(progress -> { if(progress>95) webView.close(); });
    	webView.show();
    	QTimer.singleShot(25000, QApplication::quit);
    	QApplication.exec();
    	webView.dispose();
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestWebEngineWidgets.class.getName());
    }
}
