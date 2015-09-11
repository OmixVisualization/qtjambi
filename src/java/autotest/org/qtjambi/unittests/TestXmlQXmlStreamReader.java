package org.qtjambi.unittests;

import static org.junit.Assert.*;

import org.qtjambi.qt.core.QFile;
import org.qtjambi.qt.core.QIODevice;
import org.qtjambi.qt.core.*;

// See also TestCoreQXmlStreamReader
@SuppressWarnings("deprecation")
public class TestXmlQXmlStreamReader extends QApplicationTest {

	// MacOSX version of Qt does not have this API it has only org.qtjambi.qt.core.QXmlStreamReader
	QXmlStreamReader xmlr;
	QFile xmlFile;
	QIODevice iod;

	@org.junit.Before
	public void setUp() throws Exception {
		xmlFile = new QFile("classpath:org/qtjambi/unittests/xmlSample1.xml");
		xmlr = new QXmlStreamReader();
	}

	@org.junit.After
	public void tearDown() throws Exception {
	}

	@org.junit.Test
	public void testQXmlStreamReader() {
		assertTrue(xmlFile.exists());
		QXmlStreamReader.TokenType token1 = xmlr.readNext();
		System.out.println(token1.name());
		QXmlStreamReader.TokenType token2 = xmlr.readNext();
		System.out.println(token2.name());
	}

}
