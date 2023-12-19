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
import static org.junit.Assume.*;

import java.lang.reflect.InvocationTargetException;

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.*;

public class TestQPermissionQt65 extends ApplicationInitializer {
	@BeforeClass
    public static void testInitialize() throws Exception {
		assumeTrue("Can only run on macOS, IOS or Android.", QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.MacOS, QOperatingSystemVersion.OSType.Android, QOperatingSystemVersion.OSType.IOS));
        ApplicationInitializer.testInitializeWithWidgets();
    }
	
	@Test
    public void test() throws InstantiationException, IllegalAccessException, IllegalArgumentException, InvocationTargetException, NoSuchMethodException, SecurityException {
    	Class<? extends QPermission> permissionClass = null;
    	try {
    		@SuppressWarnings({ "unchecked", "rawtypes" })
			Class<? extends QPermission> pc = (Class)Class.forName("io.qt.core.QLocationPermission");
			permissionClass = pc;
		} catch (ClassNotFoundException e) {
		}
		assumeTrue("Permission class required", permissionClass!=null);
		Object[] results = {null,null};
		QEventLoop loop = new QEventLoop();
		QCoreApplication.instance().requestPermission(permissionClass, (permission,status)->{
			System.out.println(permission+" = "+status);
			results[0] = permission;
			results[1] = status;
			QMetaObject.invokeMethod(loop, "quit()", Qt.ConnectionType.AutoConnection);
		});
		QTimer.singleShot(java.time.Duration.ofSeconds(20), loop, "quit()");
		loop.exec();
		assertTrue("permission has not been requested", results[1]!=null);
		assertTrue("permission result = "+results[1], results[1] instanceof Qt.PermissionStatus);
		assertTrue("permission type does not match. Expected: "+permissionClass.getName()+", got: "+(results[0]==null ? "null" : results[0].getClass().getName()), permissionClass.isInstance(results[0]));
    }
}
