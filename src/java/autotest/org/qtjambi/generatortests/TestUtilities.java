/**
 * Unit Test implementations for Utilities.java
 * 
 */

package org.qtjambi.generatortests;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;

import java.io.File;
import java.util.List;

import org.qtjambi.qt.QtJambi_LibraryInitializer;
import org.qtjambi.qt.Utilities;
import org.qtjambi.qt.Utilities.Configuration;
import org.qtjambi.qt.Utilities.OperatingSystem;

public class TestUtilities {

	private String singleString;
	private String failString;
	private String strings;
	private String[] emptyStrings;
	private String validLibrary;
	private String falseLibrary;
	private static String thisOs;
	private static String configuration;
	private File tmpDir;
	private static String user;
	private static String arch;

	@org.junit.BeforeClass
	public static void setUpClass() {
		QtJambi_LibraryInitializer.init();
		user = System.getProperty("user.name");
		arch = System.getProperty("os.arch");
		thisOs = System.getProperty("os.name").toLowerCase();
		configuration = System.getProperty("org.qtjambi.qt.debug");
	}

	@org.junit.Before
	public void setUp() {
		singleString = "test";
		failString = "bad";
		strings = "test test1 test2";
		emptyStrings = new String[0];
		validLibrary = "org_qtjambi_qt_core";
		falseLibrary = "org_qtjambi_qt_kore";
	}

	@org.junit.After
	public void tearDown() {
		singleString = null;
		failString = null;
		strings = null;
		emptyStrings = null;
		validLibrary = null;
		falseLibrary = null;
	}

	/*
	 * return value != null; //isn't covered
	 */
	@org.junit.Test
	public void testMatchProperty() {
		System.setProperty(singleString, strings);
		assertTrue("singleString strings", Utilities.matchProperty(singleString, strings));
		assertFalse("failString strings", Utilities.matchProperty(failString, strings));
		assertTrue("singleString emptyStrings", Utilities.matchProperty(singleString, emptyStrings));
		assertFalse("failString emptyStrings", Utilities.matchProperty(failString, emptyStrings));
		assertTrue("singleString null", Utilities.matchProperty(singleString, (String[]) null));
		assertFalse("failString null", Utilities.matchProperty(failString, (String[]) null));
		assertFalse("singleString failString", Utilities.matchProperty(singleString, failString));
	}

	@org.junit.Test
	public void testJambiTempDir() {
		// This test is only valid if we ran from unpacked JAR, this is more
		//  of an integration test and a unit test.  We should re-instate when
		//  we can control the launch environment for testing.
		tmpDir = Utilities.jambiTempDir();
		//assertTrue("tmpDir", tmpDir.getName().startsWith("QtJambi_" + user + "_" + arch + "_" + Utilities.VERSION_STRING));
	}

	@org.junit.Test
	public void testLoadLibrary() {
		assertTrue("validLibrary", Utilities.loadLibrary(validLibrary));
		assertFalse("falseLibrary", Utilities.loadLibrary(falseLibrary));
	}

	@org.junit.Test
	public void testUnpackPlugins() {
		List<String> paths = Utilities.unpackPlugins();
		// Not sure what this was testing, maybe just code coverage.
		//assertNull("unpackPlugins()", Utilities.unpackPlugins());
	}

	@org.junit.Test
	public void testDecideOperatingSystem() {
		if (thisOs.startsWith("linux"))
			assertTrue("OperatingSystem.Linux", Utilities.operatingSystem.equals(OperatingSystem.Linux));
		else if (thisOs.startsWith("mac os x"))
			assertTrue("OperatingSystem.MacOSX", Utilities.operatingSystem.equals(OperatingSystem.MacOSX));
		else if (thisOs.startsWith("windows"))
			assertTrue("OperatingSystem.Windows", Utilities.operatingSystem.equals(OperatingSystem.Windows));
		else
			assertFalse("true", true);
	}

	// This test is not valid anymore, the property org.qtjambi.qt.debug is no
	//  longer the main mechanism for holding the Configuration type.
	/**@org.junit.Test**/
	public void testDecideConfiguration() {
		if (configuration != null)
			assertTrue("Configuration.Debug", Utilities.configuration.equals(Configuration.Debug));
		else
			assertTrue("Configuration.Release", Utilities.configuration.equals(Configuration.Release));
	}

}
