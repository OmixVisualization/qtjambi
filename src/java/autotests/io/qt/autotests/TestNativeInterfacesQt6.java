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

import static org.junit.Assert.*;
import static org.junit.Assert.fail;

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QCoreApplication;
import io.qt.core.QMetaObject;
import io.qt.core.QObject;
import io.qt.core.QOperatingSystemVersion;
import io.qt.gui.QOpenGLContext;
import io.qt.gui.nativeinterface.QCocoaGLContext;
import io.qt.gui.nativeinterface.QCocoaWindow;
import io.qt.gui.nativeinterface.QEGLContext;
import io.qt.gui.nativeinterface.QGLXContext;
import io.qt.gui.nativeinterface.QWGLContext;
import io.qt.gui.nativeinterface.QWindowsApplication;
import io.qt.gui.nativeinterface.QWindowsWindow;
import io.qt.gui.nativeinterface.QX11Application;
import io.qt.gui.nativeinterface.QXcbScreen;
import io.qt.gui.nativeinterface.QXcbWindow;
import io.qt.opengl.QOpenGLWindow;
import io.qt.widgets.QApplication;

public class TestNativeInterfacesQt6 extends ApplicationInitializer {
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitializeWithWidgets();
	}
	
    @Test
    public void test() {
    	QOpenGLWindow window = new QOpenGLWindow();
    	window.show();
    	QCoreApplication.processEvents();
    	QWindowsWindow winwin = window.nativeInterface(QWindowsWindow.class);
    	QWindowsApplication winapp = QApplication.instance().nativeInterface(QWindowsApplication.class);
    	assertEquals(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Windows), winapp!=null);
    	assertEquals(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Windows), winwin!=null);
    	QCocoaWindow cocoawin = window.nativeInterface(QCocoaWindow.class);
    	assertEquals(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.MacOS), cocoawin!=null);
    	QX11Application x11app = QApplication.instance().nativeInterface(QX11Application.class);
    	assertEquals(!QOperatingSystemVersion.current().isAnyOfType(
													    			QOperatingSystemVersion.OSType.MacOS,
													    			QOperatingSystemVersion.OSType.Android,
													    			QOperatingSystemVersion.OSType.IOS,
													    			QOperatingSystemVersion.OSType.TvOS,
													    			QOperatingSystemVersion.OSType.WatchOS,
													    			QOperatingSystemVersion.OSType.Windows), x11app!=null);
    	QXcbWindow xcbwin = window.nativeInterface(QXcbWindow.class);
    	assertEquals(!QOperatingSystemVersion.current().isAnyOfType(
    			QOperatingSystemVersion.OSType.MacOS,
    			QOperatingSystemVersion.OSType.Android,
    			QOperatingSystemVersion.OSType.IOS,
    			QOperatingSystemVersion.OSType.TvOS,
    			QOperatingSystemVersion.OSType.WatchOS,
    			QOperatingSystemVersion.OSType.Windows), xcbwin!=null);
    	assertEquals(null, QApplication.instance().nativeInterface(QWindowsWindow.class));
    	try {
			QApplication.instance().nativeInterface(QObject.class);
			fail("IllegalArgumentException expected to be thrown.");
		} catch (IllegalArgumentException e) {
			assertEquals("Class io.qt.core.QObject is not a native interface type.", e.getMessage());
		}
    	QXcbScreen xcbScreen = QApplication.primaryScreen().nativeInterface(QXcbScreen.class);
    	assertEquals(!QOperatingSystemVersion.current().isAnyOfType(
    			QOperatingSystemVersion.OSType.MacOS,
    			QOperatingSystemVersion.OSType.Android,
    			QOperatingSystemVersion.OSType.IOS,
    			QOperatingSystemVersion.OSType.TvOS,
    			QOperatingSystemVersion.OSType.WatchOS,
    			QOperatingSystemVersion.OSType.Windows), xcbScreen!=null);
    	QOpenGLContext context = window.context();
    	if(cocoawin!=null) {
    		assertFalse(cocoawin instanceof QObject);
    		QObject cocoawinobject = QMetaObject.cast(QObject.class, cocoawin);
    		assertTrue(cocoawinobject!=null);
    	}
    	if(context!=null) {
	    	QGLXContext glxContext = context.nativeInterface(QGLXContext.class);
	    	QWGLContext wglContext = context.nativeInterface(QWGLContext.class);
	    	QEGLContext eglContext = context.nativeInterface(QEGLContext.class);
	    	QCocoaGLContext cocoaContext = context.nativeInterface(QCocoaGLContext.class);
	    	assertEquals(context.isOpenGLES(), eglContext!=null);
	    	assertEquals(!context.isOpenGLES() 
	    			&& !QOperatingSystemVersion.current().isAnyOfType(
	    			QOperatingSystemVersion.OSType.MacOS,
	    			QOperatingSystemVersion.OSType.Android,
	    			QOperatingSystemVersion.OSType.IOS,
	    			QOperatingSystemVersion.OSType.TvOS,
	    			QOperatingSystemVersion.OSType.WatchOS,
	    			QOperatingSystemVersion.OSType.Windows), glxContext!=null);
	    	assertEquals(!context.isOpenGLES() && QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Windows), wglContext!=null);
	    	assertEquals(!context.isOpenGLES() && QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.MacOS), cocoaContext!=null);
    	}
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestNativeInterfacesQt6.class.getName());
    }
}
