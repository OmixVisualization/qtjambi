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

import static org.junit.Assert.assertTrue;

import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import java.util.TreeSet;

import org.junit.Assume;
import org.junit.BeforeClass;

import io.qt.QtUtilities;
import io.qt.core.QByteArray;
import io.qt.core.QFile;
import io.qt.core.QFileInfo;
import io.qt.core.QIODevice;
import io.qt.core.QLibraryInfo;
import io.qt.gui.QImageReader;

public class TestPluginImageFormats extends ApplicationInitializer {
	
	private static List<String> imageFormats = new ArrayList<String>();

    @BeforeClass
	public static void testInitialize() throws Exception {
		QtUtilities.initializePackage("io.qt.svg");
		List<QByteArray> list = QImageReader.supportedImageFormats();
		TreeSet<String> _imageFormats = new TreeSet<>();
		for(QByteArray ba : list) {
			_imageFormats.add(ba.toString());
		}
		imageFormats.addAll(_imageFormats);
		System.out.println(imageFormats);
		ApplicationInitializer.testInitialize();
	}

	private boolean searchFormat(String s) {
		s = s.toLowerCase();
		for(String compare : imageFormats) {
			if(s.compareToIgnoreCase(compare) == 0)
				return true;
		}
		return false;
	}
	
	//CHANGED: better test each format individual, so if one is missed
	//we get information about the availability of the other formats.
	@org.junit.Test
	public void testSupportsPNGImageFormats() {
		assertTrue("support: png", searchFormat("png"));
	}
	
	@org.junit.Test
	public void testSupportsGIFImageFormats() {
		assertTrue("support: gif", searchFormat("gif"));
	}
	
	@org.junit.Test
	public void testSupportsJPGImageFormats() {
		assertTrue("support: jpg", searchFormat("jpg"));	// aka "jpeg"
	}
	
	@org.junit.Test
	public void testSupportsICOImageFormats() {
		assertTrue("support: ico", searchFormat("ico"));
	}
	
	@org.junit.Test
	public void testSupportsBMPImageFormats() {
		assertTrue("support: bmp", searchFormat("bmp"));
	}
	
//	@org.junit.Ignore
//	@org.junit.Test
//	public void testSupportsMNGImageFormats() {
//		assertTrue("support: mng", searchFormat("mng"));
//	}
	
	@org.junit.Test
	public void testSupportsPBMImageFormats() {
		assertTrue("support: pbm", searchFormat("pbm"));
	}
	
	@org.junit.Test
	public void testSupportsPGMImageFormats() {
		assertTrue("support: pgm", searchFormat("pgm"));
	}
	
	@org.junit.Test
	public void testSupportsPPMImageFormats() {
		assertTrue("support: ppm", searchFormat("ppm"));
	}
	
	@org.junit.Test
	public void testSupportsXBMImageFormats() {
		assertTrue("support: xbm", searchFormat("xbm"));
	}
	
	@org.junit.Test
	public void testSupportsXPMImageFormats() {
		assertTrue("support: xpm", searchFormat("xpm"));
	}
	
	@org.junit.Test
	public void testSupportsSVGImageFormats() {
		Assume.assumeTrue("QtSvg unavailable", QtUtilities.initializePackage("io.qt.svg"));
		assertTrue("support: svg", searchFormat("svg"));
	}
	
	@org.junit.Test
	public void testSupportsTIFImageFormats() {
		Assume.assumeTrue("version < 6.0.0", QLibraryInfo.version().majorVersion()<6);
		assertTrue("support: tif", searchFormat("tif"));	// aka "tiff"
	}
	
	// TODO: Load each kind of those supported images from a known file with colour wheel and
	//  test loading, size, meta info, a handful of pixel ARGB values.

	@org.junit.Test
	public void testFoobar() {
		ClassLoader cl = TestPluginImageFormats.class.getClassLoader();
		String res = "io/qt/autotests/TestClassFunctionality.jar";
		InputStream inStream = null;
		try {
			URL url = cl.getResource(res);
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testFoobar(res=\"" + res + "\"; url=\"" + url + "\")");
			inStream = cl.getResourceAsStream(res);
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "inStream(\"" + res + "\")=" + inStream);
			byte[] bA = new byte[4096];
			int n = inStream.read(bA);
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "inStream.read()=" + n);
		} catch(IOException e) {
			e.printStackTrace();
		} finally {
			if(inStream != null) {
				try {
					inStream.close();
				} catch(IOException eat) {
				}
				inStream = null;
			}
		}
	}

	@org.junit.Test
	public void testFoobar2() {
		try {
			String res = "F:\\JavaDevel\\deps\\apache-ant-1.8.2\\lib\\ant-junit4.jar";
			QFileInfo fileinfo = new QFileInfo(res);
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testFoobar2(A res=\"" + res + "\"; fileinfo=\"" + fileinfo + "\")");
		} catch(Throwable t) {
			t.printStackTrace();
		} finally {
		}

		try {
			String res = "F:/JavaDevel/deps/apache-ant-1.8.2/lib/ant-junit4.jar";
			QFileInfo fileinfo = new QFileInfo(res);
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testFoobar2(B res=\"" + res + "\"; fileinfo=\"" + fileinfo + "\")");
		} catch(Throwable t) {
			t.printStackTrace();
		} finally {
		}

		String res = ":io/qt/autotests/TestClassFunctionality.jar";
		try {
			QFileInfo fileinfo = new QFileInfo(res);
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testFoobar2(res=\"" + res + "\"; fileinfo=\"" + fileinfo + "\")");
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testFoobar2(res=\"" + res + "\"; fileinfo.size()=" + fileinfo.size());
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testFoobar2(res=\"" + res + "\"; fileinfo.exists()=" + fileinfo.exists());
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testFoobar2(res=\"" + res + "\"; fileinfo.fileName()=" + fileinfo.fileName());
		} catch(Throwable t) {
			t.printStackTrace();
		} finally {
		}
	}

	@org.junit.Test
	public void testFoobar3() {
		try {
			String res = "F:\\JavaDevel\\deps\\apache-ant-1.8.2\\lib\\ant-junit4.jar";
			QFile file = new QFile(res);
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testFoobar3(A res=\"" + res + "\"; file=\"" + file + "\"); file.exists()=" + file.exists());
		} catch(Throwable t) {
			t.printStackTrace();
		} finally {
		}

		try {
			String res = "F:/JavaDevel/deps/apache-ant-1.8.2/lib/ant-junit4.jar";
			QFile file = new QFile(res);
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testFoobar3(A res=\"" + res + "\"; file=\"" + file + "\"); file.exists()=" + file.exists());
		} catch(Throwable t) {
			t.printStackTrace();
		} finally {
		}

		String res = ":io/qt/autotests/TestClassFunctionality.jar";
		try {
			// :io/qt/autotests/TestClassFunctionality.jar
			// :io/qt/autotests/svgcards-example.png
			QFile file = new QFile(res);

			boolean bf = file.open(QIODevice.OpenModeFlag.ReadOnly);
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testFoobar3(res=\"" + res + "\"; file=\"" + file + "\"); file.open()="+bf);

			QByteArray ba = file.readAll();
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "testFoobar3(res=\"" + res + "\"; file=\"" + file + "\"); file.readAll()="+ba.size());
		} catch(Throwable t) {
			t.printStackTrace();
		} finally {
		}
	}

	// Need to load an animated image and then a non-animated and test
	//  ideally one of each supported format
//	@org.junit.Test
//	public void testSupportsAnimation() {
//		boolean bf = QImageReader.supportsAnimation();
//		assertTrue("QImageReader.supportsAnimation()", bf);
//	}

	// Same as above...
//	@org.junit.Test
//	public void testSupportsOption() {
//		boolean bf = QImageReader.supportsOption();
//		assertTrue("QImageReader.supportsAnimation()", bf);
//	}

}
