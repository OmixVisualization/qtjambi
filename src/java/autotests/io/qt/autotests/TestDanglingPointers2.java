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
import io.qt.core.QLibraryInfo;
import io.qt.core.QList;
import io.qt.core.QObject;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QSysInfo;
import io.qt.widgets.QGraphicsEllipseItem;
import io.qt.widgets.QGraphicsItem;
import io.qt.widgets.QGraphicsScene;

public class TestDanglingPointers2 extends ApplicationInitializer{
	
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
    public void testGraphicsItem() {
		Assume.assumeTrue("Can only run successfully on Windows MSVC.", QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Windows) && QLibraryInfo.build().contains("MSVC"));
		QGraphicsScene scene = new QGraphicsScene();
		scene.addText("2");
		QGraphicsItem item1 = scene.addEllipse(0,0,0,0);
		Assert.assertFalse(item1 instanceof QObject);
		QList<QGraphicsItem> items = scene.items();
		Assert.assertEquals(2, items.size());
		scene.dispose();
    	try {
    		items.at(1);
		} catch (QDanglingPointerException e) {
			Assert.assertEquals(String.format("Cannot convert dangling pointer to object of type %1$s", QGraphicsItem.class.getName()), e.getMessage());
		}
    	Assert.assertFalse(item1.isDisposed());
    	try {
    		item1.boundingRect();
		} catch (QDanglingPointerException e) {
			Assert.assertEquals(String.format("Dangling pointer to object of type %1$s", QGraphicsEllipseItem.class.getName()), e.getMessage());
		}
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestDanglingPointers2.class.getName());
    }
}
