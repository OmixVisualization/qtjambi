/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import org.junit.Assert;
import org.junit.Test;

import io.qt.core.*;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QIcon;
import io.qt.webengine.core.*;
import io.qt.webengine.widgets.*;
import io.qt.widgets.*;

public class TestInitializationWebEngineWidgets extends UnitTestInitializer {
    @Test
    public void initialize() {
    	if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.MacOS)
    			&& "x86_64".equals(System.getProperty("os.arch").toLowerCase())
    			&& QLibraryInfo.version().compareTo(new QVersionNumber(6,5,4))>=0) {
    		Assert.assertTrue("env SYSTEM_VERSION_COMPAT=0 required to run WebEngine on macOS x86_64", "0".equals(System.getenv("SYSTEM_VERSION_COMPAT")));
    	}
		Assert.assertTrue(io.qt.QtUtilities.initializePackage("io.qt.webengine.widgets"));
        QCoreApplication.setAttribute(Qt.ApplicationAttribute.AA_ShareOpenGLContexts);
    	QApplication.initialize(new String[0]);
    	QGuiApplication.setWindowIcon(new QIcon(":io/qt/autotests/icon.png"));
    	QWebEngineProfile.defaultProfile().settings().setAttribute(QWebEngineSettings.WebAttribute.PluginsEnabled, true);
        QWebEngineProfile.defaultProfile().settings().setAttribute(QWebEngineSettings.WebAttribute.DnsPrefetchEnabled, true);
        QWebEngineProfile.defaultProfile().setPersistentCookiesPolicy(QWebEngineProfile.PersistentCookiesPolicy.AllowPersistentCookies);
        {
	    	QWebEngineView window = new QWebEngineView();
	    	window.setUrl("http://info.cern.ch");
	    	window.show();
	    	QTimer.singleShot(500, QApplication.instance(), QApplication::quit);
	    	QApplication.exec();
	    }
    	QApplication.shutdown();
    }
}
