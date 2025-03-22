/****************************************************************************
**
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

import static org.junit.Assert.assertEquals;

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QCoreApplication;
import io.qt.core.QOperatingSystemVersion;
import io.qt.gui.nativeinterface.QWaylandApplication;
import io.qt.gui.nativeinterface.QX11Application;
import io.qt.opengl.QOpenGLWindow;
import io.qt.widgets.QApplication;

public class TestNativeInterfacesQt65 extends ApplicationInitializer {
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitializeWithWidgets();
	}
	
    @Test
    public void test() {
    	QOpenGLWindow window = new QOpenGLWindow();
    	window.show();
    	QCoreApplication.processEvents();
    	QX11Application x11app = QApplication.instance().nativeInterface(QX11Application.class);
    	QWaylandApplication waylandapp = QApplication.instance().nativeInterface(QWaylandApplication.class);
    	assertEquals("QApplication's nativeInterface is not QX11Application or QWaylandApplication", 
    			!QOperatingSystemVersion.current().isAnyOfType( QOperatingSystemVersion.OSType.MacOS,
												    			QOperatingSystemVersion.OSType.Android,
												    			QOperatingSystemVersion.OSType.IOS,
												    			QOperatingSystemVersion.OSType.TvOS,
												    			QOperatingSystemVersion.OSType.WatchOS,
												    			QOperatingSystemVersion.OSType.Windows), x11app!=null && waylandapp!=null);
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestNativeInterfacesQt65.class.getName());
    }
}
