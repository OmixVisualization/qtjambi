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

import io.qt.QtUtilities;
import io.qt.core.QCoreApplication;
import io.qt.core.QEventLoop;
import io.qt.core.QTimer;
import io.qt.core.QUrl;
import io.qt.core.Qt;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QOpenGLContext;
import io.qt.webengine.core.*;
import io.qt.webengine.widgets.*;

public class TestWebEngineWidgets extends ApplicationInitializer {
	
    @BeforeClass
    public static void testInitialize() throws Exception {
    	QtUtilities.initializePackage("io.qt.webengine.widgets");
        QCoreApplication.setAttribute(Qt.ApplicationAttribute.AA_ShareOpenGLContexts);
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
    	QWebEngineProfile.defaultProfile().settings().setAttribute(QWebEngineSettings.WebAttribute.PluginsEnabled, true);
        QWebEngineProfile.defaultProfile().settings().setAttribute(QWebEngineSettings.WebAttribute.DnsPrefetchEnabled, true);
        QWebEngineProfile.defaultProfile().setPersistentCookiesPolicy(QWebEngineProfile.PersistentCookiesPolicy.AllowPersistentCookies);
    }

    @Test
    public void testLoad() {
    	QEventLoop loop = new QEventLoop();
    	QWebEngineView webView = new QWebEngineView();
    	webView.load(new QUrl("http://www.qtjambi.io"));
    	webView.loadProgress.connect(progress -> { if(progress>95) loop.quit(); });
    	webView.show();
    	QTimer.singleShot(25000, loop::quit);
    	loop.exec();
    	webView.dispose();
    	loop.dispose();
    }
    
    @Test
    public void testRunJavaScript() {
    	QEventLoop loop = new QEventLoop();
        QWebEngineView view = new QWebEngineView();
        QWebEnginePage page = new QWebEnginePage(view);
        view.setPage(page);
        page.load(new QUrl("http://info.cern.ch/"));
        page.runJavaScript("window", o -> {
	            // always creates a crash if you interact with this object in any way
        		if(o!=null)
        			o.toString();
                loop.quit();
        	});
        view.show();
    	QTimer.singleShot(25000, loop::quit);
    	loop.exec();
    	view.dispose();
    	loop.dispose();
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestWebEngineWidgets.class.getName());
    }
}
