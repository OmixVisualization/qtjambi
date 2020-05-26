/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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
package io.qt.generatortests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.io.File;

import io.qt.QtUtilities;
import io.qt.internal.NativeLibraryManager;

public class TestUtilities {

	private static String thisOs;
	private static String configuration;
	@SuppressWarnings("unused")
	private File tmpDir;
	@SuppressWarnings("unused")
	private static String user;
	@SuppressWarnings("unused")
	private static String arch;

	@org.junit.BeforeClass
	public static void setUpClass() {
		io.qt.QtUtilities.initializePackage("io.qt.internal");
		user = System.getProperty("user.name");
		arch = System.getProperty("os.arch");
		thisOs = System.getProperty("os.name").toLowerCase();
		configuration = System.getProperty("io.qt.debug");
	}

	@org.junit.Test
	public void testJambiTempDir() {
		// This test is only valid if we ran from unpacked JAR, this is more
		//  of an integration test and a unit test.  We should re-instate when
		//  we can control the launch environment for testing.
		tmpDir = QtUtilities.jambiTempDir();
		//assertTrue("tmpDir", tmpDir.getName().startsWith("QtJambi_" + user + "_" + arch + "_" + Utilities.VERSION_STRING));
	}
	
	private static boolean loadQtJambiLibrary(String lib) {
        try {
            NativeLibraryManager.loadQtJambiLibrary(TestUtilities.class, lib);
            return true;
        } catch (Exception e) {
            java.util.logging.Logger.getLogger("io.qt").log(java.util.logging.Level.SEVERE, "", e);
            return false;
        }
    }

	@org.junit.Test
	public void testLoadLibrary() {
		assertTrue("validLibrary", loadQtJambiLibrary("Core"));
		assertFalse("falseLibrary", loadQtJambiLibrary("Kore"));
	}

	@org.junit.Test
	public void testDecideOperatingSystem() {
		if (thisOs.startsWith("linux"))
			assertEquals("OperatingSystem.Linux", NativeLibraryManager.operatingSystem(), NativeLibraryManager.OperatingSystem.Linux);
		else if (thisOs.startsWith("mac os x"))
			assertEquals("OperatingSystem.MacOSX", NativeLibraryManager.operatingSystem(), NativeLibraryManager.OperatingSystem.MacOSX);
		else if (thisOs.startsWith("windows"))
			assertEquals("OperatingSystem.Windows", NativeLibraryManager.operatingSystem(), NativeLibraryManager.OperatingSystem.Windows);
		else
			assertFalse("true", true);
	}

	// This test is not valid anymore, the property io.qt.debug is no
	//  longer the main mechanism for holding the Configuration type.
	/**@org.junit.Test**/
	public void testDecideConfiguration() {
		if (configuration != null)
			assertEquals("Configuration.Debug", NativeLibraryManager.configuration(), NativeLibraryManager.Configuration.Debug);
		else
			assertEquals("Configuration.Release", NativeLibraryManager.configuration(), NativeLibraryManager.Configuration.Release);
	}

}
