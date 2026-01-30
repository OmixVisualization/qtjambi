/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.util.List;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtPrimitiveType;
import io.qt.core.QLibraryInfo;
import io.qt.core.QObject;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QTimer;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QWindow;
import io.qt.test.QSignalSpy;
import io.qt.widgets.QCheckBox;

public class TestQTest extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
	@AfterClass
    public static void testDispose() throws Exception {
    	if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.MacOS) 
    			&& QLibraryInfo.version().majorVersion()==6 
    			&& QLibraryInfo.version().minorVersion()==5) {
	    	QWindow window = new QWindow();
	    	window.show();
	    	QTimer.singleShot(200, QGuiApplication.instance(), QGuiApplication::quit);
	    	QGuiApplication.exec();
	    	window.close();
	    	window.disposeLater();
	    	window = null;
    	}
    	ApplicationInitializer.testDispose();
    }
	
    @Test
    public void testSignalSpy1() {
    	QCheckBox box = new QCheckBox();
    	QSignalSpy signalSpy = new QSignalSpy(box.clicked);
    	assertEquals(0, signalSpy.count());
    	box.click();
    	assertEquals(1, signalSpy.count());
    	List<Object> arguments = signalSpy.takeFirst();
    	assertEquals(true, arguments.get(0));
    	box.dispose();
    	assertFalse(signalSpy.isDisposed());
    	signalSpy.dispose();
    	assertTrue(signalSpy.isDisposed());
    }
    
    @Test
    public void testSignalSpy2() {
    	class Sender extends QObject{
    		final Signal2<@QtPrimitiveType Integer, String> sig = new Signal2<>();
    	}
    	Sender object = new Sender();
    	QSignalSpy signalSpy = new QSignalSpy(object.sig);
    	assertEquals(0, signalSpy.size());
    	for(int i=0; i<5; ++i) {
    		object.sig.emit(i, ""+i);
    	}
    	assertEquals(5, signalSpy.size());
    	for(int i=0; i<5; ++i) {
	    	List<Object> arguments = signalSpy.at(i);
	    	assertEquals(2, arguments.size());
	    	assertEquals(i, arguments.get(0));
	    	assertEquals(""+i, arguments.get(1));
    	}
    	assertFalse(signalSpy.isDisposed());
    	signalSpy.dispose();
    	assertTrue(signalSpy.isDisposed());
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQTest.class.getName());
    }
}
