/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import java.util.List;

import org.junit.Assume;
import org.junit.BeforeClass;

import io.qt.QtUtilities;
import io.qt.core.QByteArray;
import io.qt.core.QCoreApplication;
import io.qt.gui.QImage;
import io.qt.gui.QImage.Format;
import io.qt.gui.QImageReader;

public class TestQImage extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }

	private QImage qimage32;
	private QImage qimage64PNG;
	private QImage sample;
	private QImage mandelbrot;
	private QImage blueAngleJPG;
	private QImage anSVGImage;
	private QImage anotherSVGImage;
	private String qimage64Path = ":/qt-project.org/qmessagebox/images/qtlogo-64.png";
	private String blueAngleJPGPath = ":io/qt/autotests/blue_angle_swirl.jpg";
	private String anSVGImagePath = ":io/qt/autotests/Logo-ubuntu_cof-orange-hex.svg";
	private String anotherSVGImagePath = ":io/qt/autotests/svg-cards.svg";
	
	@org.junit.BeforeClass
	public static void init() {
		// Diagnostic information to help locate trouble
		QtUtilities.initializePackage("io.qt.svg");
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
		qimage32 = new QImage(":io/qt/autotests/svgcards-example.png");
		qimage64PNG = new QImage();
		sample = new QImage(3, 3, Format.Format_Indexed8);
		mandelbrot = new QImage(":io/qt/autotests/mandelbrot.png");
		blueAngleJPG = new QImage();
		anSVGImage = new QImage();
		anotherSVGImage = new QImage();
	}
	
	@org.junit.After
	public void tearDown() throws Exception {
		qimage32 = null;
		qimage64PNG = null;
		sample = null;
		mandelbrot = null;
		blueAngleJPG = null;
		anSVGImage = null;
		anotherSVGImage = null;
	}
	
	@org.junit.Test
	public void testBasic() {
		assertFalse(qimage32.isNull());
		assertFalse(qimage32.isGrayscale());
		assertEquals(32, mandelbrot.depth());
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
		Assume.assumeTrue("Need to support SVG", QImageReader.supportedImageFormats().contains(new QByteArray("svg")));
		assertTrue(anSVGImage.load(anSVGImagePath));
	}
	
//	@org.junit.Ignore
	/**
	 * The following test makes the JVM crash.
	 *
	 * dlm - Maybe that is before "extends ApplicationInitializer" and the crash is
	 *  due to some interaction of using Qt API without first setting up a
	 *  QApplication/QCoreApplication.  So far for me on Linux this test is not
	 *  crashing.
	 */
	@org.junit.Test
	public void testLoadSVGBig() {
		Assume.assumeTrue("Need to support SVG", QImageReader.supportedImageFormats().contains(new QByteArray("svg")));
		assertTrue(anotherSVGImage.load(anotherSVGImagePath));
	}
	
	@org.junit.Test
	public void testDimensions() {
		assertEquals(418, qimage32.width());
		assertEquals(356, qimage32.height());
	}
	
	@org.junit.Test
	public void testConvertToFormat() {
		assertEquals(Format.Format_RGB32, mandelbrot.format());
		mandelbrot = mandelbrot.convertToFormat(Format.Format_Mono);
		assertEquals(Format.Format_Mono, mandelbrot.format());
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
