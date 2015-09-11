/**
 * Unit Test implementations for GeneratorUtilities.java
 * 
 * TODO testCreateExtendedEnum()
 * 
 */

package org.qtjambi.generatortests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import org.qtjambi.qt.GeneratorUtilities;
import org.qtjambi.qt.QtJambi_LibraryInitializer;
import org.qtjambi.qt.core.QDir;
import org.qtjambi.qt.core.QFile;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.gui.QImage;

public class TestGeneratorUtilities {

	private GeneratorUtilities gu1;
	private QObject o1;
	private FetchableTestClass ftc;
	private QObject o2;
	private QImage image;
	private Thread t;

	@org.junit.BeforeClass
	public static void setUpClass() {
		QtJambi_LibraryInitializer.init();
	}

	@org.junit.Before
	public void setUp() {
		gu1 = new GeneratorUtilities();
		o1 = new QObject();
		o2 = new QObject();
		ftc = new FetchableTestClass(o2);
		image = new QImage();
		t = new Thread();
		t.start();
	}

	@org.junit.After
	public void tearDown() {
		gu1 = null;
		o1 = null;
		ftc = null;
		o2 = null;
		image = null;
		ftc = null;
		t.interrupt();
		t = null;
	}

	@org.junit.Test(expected = org.qtjambi.qt.QThreadAffinityException.class)
	public void testThreadCheck() {
		o1.moveToThread(t);
		GeneratorUtilities.threadCheck(o1);
	}

	@org.junit.Test
	public void testFetchField() {
		/*
		 * Access the class variable with it's getter method, then with
		 * GeneratorUtilities.fetchfield(QObject, Class, String); Compare the
		 * references.
		 */
		assertTrue("fakeVar1", ftc.getFakeVar1().equals(GeneratorUtilities.fetchField(ftc, FetchableTestClass.class, "fakeVar1")));
	}

	@org.junit.Test
	public void testSetField() {
		GeneratorUtilities.setField(ftc, FetchableTestClass.class, "str1", "some text");
		assertEquals("ftc.getStr1()", "some text", ftc.getStr1());
	}

	@org.junit.Test
	public void testCountExpense() {
		final String path = "./src/java/qtjambi/org/qtjambi/qt/images/qt-logo.png";
		QFile qfile = new QFile(path);
		boolean bf = qfile.exists();
		assertTrue(bf);

		image.load(path);
		assertEquals("height*bytesPerLine", 4096, image.height() * image.bytesPerLine());
	}

	// TODO implement testCreateExtendedEnum(), example from
	// QAbstractFileEngine.java
	/*
	 * @org.junit.Test public void testCreateExtendedEnum() { }
	 */

}

class FetchableTestClass {

	private QObject fakeVar1;
	private String str1;

	public FetchableTestClass(QObject fakeVar1) {
		this.fakeVar1 = fakeVar1;
	}

	public QObject getFakeVar1() {
		return fakeVar1;
	}

	public String getStr1() {
		return str1;
	}

}
