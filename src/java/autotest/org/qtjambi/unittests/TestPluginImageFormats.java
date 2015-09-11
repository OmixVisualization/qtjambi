/**
 * 
 */

package org.qtjambi.unittests;

import static org.junit.Assert.assertTrue;

import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;

import org.junit.Before;
import org.junit.After;

import org.qtjambi.qt.gui.QImageReader;
import org.qtjambi.qt.core.QByteArray;
import org.qtjambi.qt.core.QFile;
import org.qtjambi.qt.core.QFileInfo;
import org.qtjambi.qt.core.QIODevice;

import org.qtjambi.autotests.Utils;

public class TestPluginImageFormats extends QApplicationTest {
	
	private static List<String> imageFormats = new ArrayList<String>();

	@org.junit.Before
	public void setUp() {
		List<QByteArray> list = QImageReader.supportedImageFormats();
		for(QByteArray ba : list) {
			StringBuilder sb = new StringBuilder();
			for(int i = 0; i < ba.size(); i++)
				sb.append((char)ba.at(i));
			imageFormats.add(sb.toString());
			Utils.println(1, "format: " + ba.toString() + " " + sb.toString());
		}
	}

	@org.junit.After
	public void tearDown() {
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
	
	@org.junit.Test
	public void testSupportsMNGImageFormats() {
		assertTrue("support: mng", searchFormat("mng"));
	}
	
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
		assertTrue("support: svg", searchFormat("svg"));
	}
	
	@org.junit.Test
	public void testSupportsTIFImageFormats() {
		assertTrue("support: tif", searchFormat("tif"));	// aka "tiff"
	}
	
	// TODO: Load each kind of those supported images from a known file with colour wheel and
	//  test loading, size, meta info, a handful of pixel ARGB values.

	@org.junit.Test
	public void testFoobar() {
		ClassLoader cl = TestPluginImageFormats.class.getClassLoader();
		String res = "org/qtjambi/autotests/TestClassFunctionality.jar";
		InputStream inStream = null;
		try {
			URL url = cl.getResource(res);
			Utils.println(1, "testFoobar(res=\"" + res + "\"; url=\"" + url + "\")");
			inStream = cl.getResourceAsStream(res);
			Utils.println(1, "inStream(\"" + res + "\")=" + inStream);
			byte[] bA = new byte[4096];
			int n = inStream.read(bA);
			Utils.println(1, "inStream.read()=" + n);
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
			Utils.println(1, "testFoobar2(A res=\"" + res + "\"; fileinfo=\"" + fileinfo + "\")");
		} catch(Throwable t) {
			t.printStackTrace();
		} finally {
		}

		try {
			String res = "F:/JavaDevel/deps/apache-ant-1.8.2/lib/ant-junit4.jar";
			QFileInfo fileinfo = new QFileInfo(res);
			Utils.println(1, "testFoobar2(B res=\"" + res + "\"; fileinfo=\"" + fileinfo + "\")");
		} catch(Throwable t) {
			t.printStackTrace();
		} finally {
		}

		String res = "classpath:org/qtjambi/autotests/TestClassFunctionality.jar";
		try {
			QFileInfo fileinfo = new QFileInfo(res);
			Utils.println(1, "testFoobar2(res=\"" + res + "\"; fileinfo=\"" + fileinfo + "\")");
			Utils.println(1, "testFoobar2(res=\"" + res + "\"; fileinfo.size()=" + fileinfo.size());
			Utils.println(1, "testFoobar2(res=\"" + res + "\"; fileinfo.exists()=" + fileinfo.exists());
			Utils.println(1, "testFoobar2(res=\"" + res + "\"; fileinfo.fileName()=" + fileinfo.fileName());
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
			Utils.println(1, "testFoobar3(A res=\"" + res + "\"; file=\"" + file + "\"); file.exists()=" + file.exists());
		} catch(Throwable t) {
			t.printStackTrace();
		} finally {
		}

		try {
			String res = "F:/JavaDevel/deps/apache-ant-1.8.2/lib/ant-junit4.jar";
			QFile file = new QFile(res);
			Utils.println(1, "testFoobar3(A res=\"" + res + "\"; file=\"" + file + "\"); file.exists()=" + file.exists());
		} catch(Throwable t) {
			t.printStackTrace();
		} finally {
		}

		String res = "classpath:org/qtjambi/autotests/TestClassFunctionality.jar";
		try {
			// classpath:org/qtjambi/autotests/TestClassFunctionality.jar
			// classpath:org/qtjambi/examples/images/cheese.png
			QFile file = new QFile(res);

			boolean bf = file.open(QIODevice.OpenModeFlag.ReadOnly);
			Utils.println(1, "testFoobar3(res=\"" + res + "\"; file=\"" + file + "\"); file.open()="+bf);

			QByteArray ba = file.readAll();
			Utils.println(1, "testFoobar3(res=\"" + res + "\"; file=\"" + file + "\"); file.readAll()="+ba.size());
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
