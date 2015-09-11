package org.qtjambi.unittests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import java.util.List;

import org.qtjambi.qt.core.QCoreApplication;
import org.qtjambi.qt.gui.QImage;
import org.qtjambi.qt.gui.QImage.Format;
import org.qtjambi.qt.gui.QImageReader;

public class TestQImage extends QApplicationTest {

	private QImage qimage32;
	private QImage qimage64PNG;
	private QImage sample;
	private QImage generator;
	private QImage blueAngleJPG;
	private QImage anSVGImage;
	private QImage anotherSVGImage;
	private String qimage64Path = "classpath:org/qtjambi/images/qtlogo-64.png";
	private String blueAngleJPGPath = "classpath:org/qtjambi/images/blue_angle_swirl.jpg";
	private String anSVGImagePath = "classpath:org/qtjambi/unittests/Logo-ubuntu_cof-orange-hex.svg";
	private String anotherSVGImagePath = "classpath:org/qtjambi/unittests/svg-cards.svg";
	
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
		qimage64PNG = new QImage();
		sample = new QImage(3, 3, Format.Format_Indexed8);
		generator = new QImage("classpath:org/qtjambi/images/generator.png");
		blueAngleJPG = new QImage();
		anSVGImage = new QImage();
		anotherSVGImage = new QImage();
	}
	
	@org.junit.After
	public void tearDown() throws Exception {
		qimage32 = null;
		qimage64PNG = null;
		sample = null;
		generator = null;
		blueAngleJPG = null;
		anSVGImage = null;
		anotherSVGImage = null;
	}
	
	@org.junit.Test
	public void testBasic() {
		assertFalse(qimage32.isNull());
		assertFalse(qimage32.isGrayscale());
		assertEquals(32, generator.depth());
	}
	
	@org.junit.Test()
	public void testLoadPNG() {
		assertTrue(qimage64PNG.load(qimage64Path));
	}
	
	@org.junit.Test
	public void testLoadJPG() {
		assertTrue(blueAngleJPG.load(blueAngleJPGPath));
	}
	
	@org.junit.Test
	public void testLoadSVGSmall() {
		assertTrue(anSVGImage.load(anSVGImagePath));
	}
	
//	@org.junit.Ignore
	/**
	 * The following test makes the JVM crash.
	 *
	 * dlm - Maybe that is before "extends QApplicationTest" and the crash is
	 *  due to some interaction of using Qt API without first setting up a
	 *  QApplication/QCoreApplication.  So far for me on Linux this test is not
	 *  crashing.
	 */
	@org.junit.Test
	public void testLoadSVGBig() {
		assertTrue(anotherSVGImage.load(anotherSVGImagePath));
	}
	
	@org.junit.Test
	public void testDimensions() {
		assertEquals(32, qimage32.width());
		assertEquals(qimage32.width(), qimage32.height());
	}
	
	@org.junit.Test
	public void testConvertToFormat() {
		assertEquals(Format.Format_RGB32, generator.format());
		generator = generator.convertToFormat(Format.Format_Mono);
		assertEquals(Format.Format_Mono, generator.format());
	}
	
	@org.junit.Test
	public void testSetPixel() {
		List<Integer> colors = new ArrayList<Integer>();
		
		colors.add(5);
		colors.add(10);
		colors.add(15);
		
		sample.setColorTable(colors);
		
		// x, y, color index in the color table 
		sample.setPixel(0, 0, 0);
		sample.setPixel(2, 2, 2);
		
		assertEquals(15, sample.pixel(2, 2));
		assertEquals( 5, sample.pixel(0, 0));
	}
	
}
