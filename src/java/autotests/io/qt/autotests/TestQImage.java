/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.util.*;
import org.junit.*;

import io.qt.*;
import io.qt.core.*;
import io.qt.gui.*;
import io.qt.gui.QImage.*;

public class TestQImage extends ApplicationInitializer {
	
	private static Set<String> imageFormats = new TreeSet<>();
	
	@BeforeClass
    public static void testInitialize() throws Exception {
		if(QtUtilities.isAvailableQtLibrary("Svg"))
			QtUtilities.loadQtLibrary("Svg");
		QList<QByteArray> list = QImageReader.supportedImageFormats();
		for(QByteArray ba : list) {
			imageFormats.add(ba.toString().toLowerCase());
		}
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
	@Test
	public void testSupportsPNGImageFormats() {
		assertTrue("Qt unexpectedly does not support png", imageFormats.contains("png"));
	}
	
	@Test
	public void testSupportsGIFImageFormats() {
		assertTrue("Qt unexpectedly does not support gif", imageFormats.contains("gif"));
	}
	
	@Test
	public void testSupportsJPGImageFormats() {
		assertTrue("Qt unexpectedly does not support jpg", imageFormats.contains("jpg"));	// aka "jpeg"
	}
	
	@Test
	public void testSupportsICOImageFormats() {
		assertTrue("Qt unexpectedly does not support ico", imageFormats.contains("ico"));
	}
	
	@Test
	public void testSupportsBMPImageFormats() {
		assertTrue("Qt unexpectedly does not support bmp", imageFormats.contains("bmp"));
	}
	
	@Test
	public void testSupportsPBMImageFormats() {
		assertTrue("Qt unexpectedly does not support pbm", imageFormats.contains("pbm"));
	}
	
	@Test
	public void testSupportsPGMImageFormats() {
		assertTrue("Qt unexpectedly does not support pgm", imageFormats.contains("pgm"));
	}
	
	@Test
	public void testSupportsPPMImageFormats() {
		assertTrue("Qt unexpectedly does not support ppm", imageFormats.contains("ppm"));
	}
	
	@Test
	public void testSupportsXBMImageFormats() {
		assertTrue("Qt unexpectedly does not support xbm", imageFormats.contains("xbm"));
	}
	
	@Test
	public void testSupportsXPMImageFormats() {
		assertTrue("Qt unexpectedly does not support xpm", imageFormats.contains("xpm"));
	}
	
	@Test
	public void testSupportsSVGImageFormats() {
		Assume.assumeTrue("QtSvg unavailable", QtUtilities.isAvailableQtLibrary("Svg"));
		assertTrue("Qt unexpectedly does not support svg", imageFormats.contains("svg"));
	}

	private final String qimage64Path = ":/qt-project.org/qmessagebox/images/qtlogo-64.png";
	private final String blueAngleJPGPath = ":io/qt/autotests/blue_angle_swirl.jpg";
	private final String anSVGImagePath = ":io/qt/autotests/Logo-ubuntu_cof-orange-hex.svg";
	private final String anotherSVGImagePath = ":io/qt/autotests/svg-cards.svg";
	private final String mandelbrotURL = ":io/qt/autotests/mandelbrot.png";
	
	@Test
	public void testBasic() {
		QImage image = new QImage(":io/qt/autotests/svgcards-example.png");
		assertFalse(image.isNull());
		assertFalse(image.isGrayscale());
		assertEquals(418, image.width());
		assertEquals(356, image.height());
		assertEquals(Format.Format_ARGB32, image.format());
		QImage imageFromBytes = new QImage(image.bits(), image.width(), image.height(), image.format());
		QImage imageFromBuffer = new QImage(image.bytes(), image.width(), image.height(), image.format());
		assertEquals(imageFromBytes, image);
		assertEquals(imageFromBuffer, image);
	}
	
	@Test
	public void testLoadPNG() {
		QImage image = new QImage();
		assertTrue(image.load(qimage64Path));
		assertEquals(Format.Format_Indexed8, image.format());
	}
	
	@Test
	public void testLoadJPG() {
		QImage image = new QImage();
		assertTrue(image.load(blueAngleJPGPath));
		assertEquals(Format.Format_RGB32, image.format());
		QImage imageFromBytes = new QImage(image.bits(), image.width(), image.height(), image.format());
		QImage imageFromBuffer = new QImage(image.bytes(), image.width(), image.height(), image.format());
		assertEquals(imageFromBytes, image);
		assertEquals(imageFromBuffer, image);
	}
	
	@Test
	public void testLoadSVGSmall() {
		Assume.assumeTrue("Need to support SVG", QImageReader.supportedImageFormats().contains(new QByteArray("svg")));
		QImage image = new QImage();
		assertTrue(image.load(anSVGImagePath));
		assertEquals(Format.Format_ARGB32_Premultiplied, image.format());
		QImage imageFromBytes = new QImage(image.bits(), image.width(), image.height(), image.format());
		QImage imageFromBuffer = new QImage(image.bytes(), image.width(), image.height(), image.format());
		assertEquals(imageFromBytes, image);
		assertEquals(imageFromBuffer, image);
	}
	
	/**
	 * The following test makes the JVM crash.
	 *
	 * dlm - Maybe that is before "extends ApplicationInitializer" and the crash is
	 *  due to some interaction of using Qt API without first setting up a
	 *  QApplication/QCoreApplication.  So far for me on Linux this test is not
	 *  crashing.
	 */
	@Test
	public void testLoadSVGBig() {
		Assume.assumeTrue("Need to support SVG", QImageReader.supportedImageFormats().contains(new QByteArray("svg")));
		QImage image = new QImage();
		assertTrue(image.load(anotherSVGImagePath));
		assertEquals(Format.Format_ARGB32_Premultiplied, image.format());
		QImage imageFromBytes = new QImage(image.bits(), image.width(), image.height(), image.format());
		QImage imageFromBuffer = new QImage(image.bytes(), image.width(), image.height(), image.format());
		assertEquals(imageFromBytes, image);
		assertEquals(imageFromBuffer, image);
	}
	
	@Test
	public void testConvertToFormat() {
		QImage image = new QImage(mandelbrotURL);
		assertEquals(32, image.depth());
		assertEquals(Format.Format_RGB32, image.format());
		QImage monoMandelbrot = image.convertToFormat(Format.Format_Mono);
		assertEquals(Format.Format_Mono, monoMandelbrot.format());
		assertEquals(Format.Format_RGB32, image.format());
		QImage imageFromBytes = new QImage(image.bits(), image.width(), image.height(), image.format());
		QImage imageFromBuffer = new QImage(image.bytes(), image.width(), image.height(), image.format());
		assertEquals(imageFromBytes, image);
		assertEquals(imageFromBuffer, image);
	}
	
	@Test
	public void testSetPixel() {
		List<Integer> colors = new ArrayList<Integer>();
		
		colors.add(5);
		colors.add(10);
		colors.add(15);

		QImage sample = new QImage(3, 3, Format.Format_Indexed8);
		sample.setColorTable(colors);
		
		// x, y, color index in the color table 
		sample.setPixel(0, 0, 0);
		sample.setPixel(2, 2, 2);
		
		assertEquals(15, sample.pixel(2, 2));
		assertEquals( 5, sample.pixel(0, 0));
	}

    @Test
    public void testXPM() {
        // Check that const char *[] is handled properly by the generated code
java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "run_XPMConstructors() BEGIN");
        String qt_plastique_radio[] = { "13 13 2 1", "X c #000000", ". c #ffffff", "....XXXXX....", "..XX.....XX..", ".X.........X.", ".X.........X.", "X...........X", "X...........X",
                "X...........X", "X...........X", "X...........X", ".X.........X.", ".X.........X.", "..XX.....XX..", "....XXXXX...." };

        QImage img = new QImage(qt_plastique_radio);
        assertEquals(img.width(), 13);
        assertEquals(img.height(), 13);
java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "run_XPMConstructors() QIMAGE TESTING");

        assertEquals(img.pixel(2, 1), 0xff000000);
        assertEquals(img.pixel(0, 0), 0xffffffff);

java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "run_XPMConstructors() QPIXMAP TESTING");
        QPixmap pm = new QPixmap(qt_plastique_radio);
        QImage img2 = pm.toImage();
        assertEquals(img2.pixel(2, 1), 0xff000000);
        assertEquals(img2.pixel(12, 12), 0xffffffff);
java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "run_XPMConstructors() END");
    }
	
}
