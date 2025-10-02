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

import static org.junit.Assert.assertTrue;

import java.io.File;

import org.junit.AfterClass;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtUtilities;
import io.qt.core.QDir;
import io.qt.core.QFile;
import io.qt.core.QFileInfo;
import io.qt.core.QIODevice;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QResource;
import io.qt.core.QStringList;

public class TestFileEngineClass extends ApplicationInitializer {
	static{
		System.setProperty("io.qt.allow-classfiles-as-resource", "true");
	}
	private static String search_path1 = null;
	private static java.io.File tmpFile1 = null;
	private static File tmpFile2 = null;
	private static String search_path2 = null;
	
	@BeforeClass
    public static void testInitialize() throws Exception {
		Assume.assumeFalse("Cannot test on Android", QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
	@AfterClass
    public static void testDispose() throws Exception {
		if(search_path1!=null) {
			runGC();
			QResource.removeClassPath(search_path1);
			if(tmpFile1!=null && !tmpFile1.delete()) {
				tmpFile1.deleteOnExit();
			}
		}
		if(search_path2!=null) {
			runGC();
			QResource.removeClassPath(search_path2);
			if(tmpFile2!=null && !tmpFile2.delete()) {
				tmpFile2.deleteOnExit();
			}
		}
		ApplicationInitializer.testDispose();
	}
	@Test
    public void testClassInFileEngine() throws Exception {
		QtUtilities.initializePackage(io.qt.autotests.generated.General.class);
		QFileInfo info = new QFileInfo(":io/qt/autotests/TestClassFunctionality.jar");
        assertTrue(info.exists());
        info.dispose();
        
        info = new QFileInfo(":io/qt/autotests/generated/");
		assertTrue(info.isDir());
		QDir dir = new QDir(":io/qt/autotests/generated/");
		QStringList content = dir.entryList(QDir.Filter.NoFilter.asFlags(), QDir.SortFlag.NoSort.asFlags());
		dir.dispose();
		assertTrue("directory :io/qt/autotests/generated/ contains class", content.contains("General.class"));
		assertTrue("directory :io/qt/autotests/generated/ contains image", content.contains("qjlogo.png"));
        info.dispose();
		
        info = new QFileInfo(":io/qt/autotests/generated/General.class");
		assertTrue(info.exists());
        info.dispose();
	}
	
	@Test
    public void testGeneratorResources() {
		QFile file = new QFile(":io/qt/qtjambi/deployer/plugin.cpp");
		try{
			assertTrue(file.exists());
			assertTrue(file.open(QIODevice.OpenModeFlag.ReadOnly));
		}finally {
			file.close();
		}
	}
	
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestFileEngineClass.class.getName());
    }
}
