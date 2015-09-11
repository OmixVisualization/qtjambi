package org.qtjambi.unittests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assume.assumeThat;
import static org.qtjambi.unittests.support.MatcherQtVersion.isQtGe;

import java.util.List;

import org.qtjambi.qt.core.QCoreApplication;
import org.qtjambi.qt.gui.QImage;
import org.qtjambi.qt.gui.QImageReader;
import org.qtjambi.unittests.support.QtVersion;

/**
 * This class tests the features that are marked as arriving
 *  since a certain Qt version as per classname.
 * These tests are isolated into a different *.java file to
 *  allow for excluding from compilation, since they will fail
 *  due to missing symbols.
 */
public class TestQImage46x extends QApplicationTest {

	private QImage qimage32;

	@org.junit.BeforeClass
	public static void init() {
		// Diagnostic information to help locate trouble
		String s;
		s = QCoreApplication.applicationDirPath();
		System.out.println("QCoreApplication.applicationDirPath()=" + s);
		s = QCoreApplication.applicationFilePath();
		System.out.println("QCoreApplication.applicationFilePath()=" + s);
		List<String> libraryPathList = QCoreApplication.libraryPaths();
		for(String libraryPathElement : libraryPathList)
			System.out.println("QCoreApplication.libraryPaths(): " + libraryPathElement);
		// We should re-validate this "." as the choice of the current working directory of
		//  the JUnitTest should not influence the results without good reason.
		//QAbstractFileEngine.addSearchPathForResourceEngine(".");
		System.out.println(QImageReader.supportedImageFormats());
	}

	@org.junit.Before
	public void setUp() throws Exception {
		qimage32 = new QImage("classpath:org/qtjambi/images/qtlogo-32.png");
	}

	@org.junit.After
	public void tearDown() throws Exception {
		qimage32 = null;
	}

	@org.junit.Test
	public void testSize() {
		assumeThat(QtVersion.getQtVersion(), isQtGe(QtVersion.VERSION_4_6));  // Version check

		int size = qimage32.height() * qimage32.bytesPerLine();
		assertEquals(size, qimage32.byteCount()); // API since 4.6.x
	}

}
