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

import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QDanglingPointerException;
import io.qt.QNativePointer;
import io.qt.core.QObject;
import io.qt.core.QSysInfo;

public class TestDanglingPointers1 extends ApplicationInitializer{
	
	static {
//		System.setProperty("io.qt.enable-dangling-pointer-check", "true");
	}
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		Assume.assumeTrue("Runtime property -Dio.qt.enable-dangling-pointer-check=true required.", Boolean.getBoolean("io.qt.enable-dangling-pointer-check"));
		Assume.assumeTrue("Can only run successfully on x86_64", "x86_64".equals(QSysInfo.currentCpuArchitecture()));
		ApplicationInitializer.testInitializeWithWidgets();
    }
	
    @Test
    public void testNativePointer() {
    	try {
			QNativePointer.fromNative(5).object(QObject.class);
		} catch (QDanglingPointerException e) {
			Assert.assertEquals(String.format("Cannot convert dangling pointer to object of type %1$s", QObject.class.getName()), e.getMessage());
		}
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestDanglingPointers1.class.getName());
    }
}
