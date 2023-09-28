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

import java.util.Arrays;

import org.junit.*;
import io.qt.*;
import io.qt.core.*;

public class TestApplicationArgs extends UnitTestInitializer {
	
	@BeforeClass
	public static void testInitialize() throws Exception {
        try {
			if(QCoreApplication.instance()==null) {
				java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testInitialize: begin");
				QResource.addClassPath(".");
				QCoreApplication.setApplicationName("QtJambiUnitTest");
				QCoreApplication.initialize(new String[]{"arg1", "arg2", "arg3"});
			    java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testInitialize: done");
		        QThread.currentThread().setObjectName("main");
			}
		} catch (Throwable e) {
			e.printStackTrace();
			throw e;
		}
    }
	
	@AfterClass
    public static void testDispose() throws Exception {
		ApplicationInitializer.testDispose();
	}
	
    @Test
    public void testArguments() {
    	assertEquals(Arrays.asList("QtJambiUnitTest", "arg1", "arg2", "arg3"), QCoreApplication.arguments());
    }
    
    @Test
    public void testEnv() {
    	assertEquals(null, QtUtilities.getenv("QTJAMBI_ENV_TEST"));
    	QtUtilities.putenv("QTJAMBI_ENV_TEST", "1");
    	assertEquals("1", QtUtilities.getenv("QTJAMBI_ENV_TEST"));
    }
    
    @Test
    public void testReinstallEventNotifyCallback() {
    	assertTrue(QtUtilities.reinstallEventNotifyCallback());
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestApplicationArgs.class.getName());
    }
}
