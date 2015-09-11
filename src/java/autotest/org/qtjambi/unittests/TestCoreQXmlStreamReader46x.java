package org.qtjambi.unittests;

import static org.qtjambi.unittests.support.MatcherQtVersion.isQtGe;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.junit.Assume.assumeThat;

import org.qtjambi.qt.core.QFile;
import org.qtjambi.qt.core.QIODevice;
import org.qtjambi.qt.core.QXmlStreamReader;
import org.qtjambi.qt.core.QXmlStreamReader.TokenType;
import org.qtjambi.unittests.support.QtVersion;

/**
 * This class tests the features that are marked as arriving
 *  since a certain Qt version as per classname.
 * These tests are isolated into a different *.java file to
 *  allow for excluding from compilation, since they will fail
 *  due to missing symbols.
 */
public class TestCoreQXmlStreamReader46x extends QApplicationTest {

	private QXmlStreamReader xmlr;
	private QFile xmlFile;
	private final String person[] = { "John", "Jane" };

	//@org.junit.BeforeClass
	//public static void setUpClass() {
	//	QAbstractFileEngine.addSearchPathForResourceEngine(".");
	//}

	@org.junit.Before
	public void setUp() throws Exception {
		xmlFile = new QFile("classpath:org/qtjambi/unittests/xmlSample1.xml");
		xmlFile.open(new QIODevice.OpenMode(QIODevice.OpenModeFlag.ReadOnly));
		xmlr = new QXmlStreamReader(xmlFile);
		/*
		 * placing the first .readNext() here because the first token is XML
		 * definition the definition
		 */
		xmlr.readNext();
	}

	@org.junit.After
	public void tearDown() throws Exception {
		xmlFile.close();
	}

	@org.junit.Test
	public void testSkipCurrentElement() {
		assumeThat(QtVersion.getQtVersion(), isQtGe(QtVersion.VERSION_4_6));  // Version check

		while (!xmlr.atEnd()) {
			TokenType token = xmlr.readNext();
			if (token == TokenType.StartElement) {
				if (token.name().equals("persons"))
					continue;
				if (xmlr.name().equals("person")) { // [<person
													// id="John">]John</person>
					xmlr.skipCurrentElement(); // <person  // API since 4.6.x
												// id="John">John[</person>]
					xmlr.readNext(); // [ ]<person id="Jane">Jane</person>
					xmlr.readNext(); // [<person id="Jane">]Jane</person>
					xmlr.readNext(); // <person id="Jane">[Jane]</person>
					assertEquals(person[1], xmlr.text());
				}
			}
		}
	}

	@org.junit.Test
	public void testLineNumber() {
		assumeThat(QtVersion.getQtVersion(), isQtGe(QtVersion.VERSION_4_6));  // Version check

		// StartDocument
		assertEquals(1, xmlr.lineNumber());
		xmlr.readNext();
		// StartElement - <persons></persons>
		assertEquals(2, xmlr.lineNumber());
		// StartElement - <person></person>
		xmlr.readNextStartElement(); // API since 4.6.x
		assertEquals(3, xmlr.lineNumber());
		assertEquals("John", xmlr.readElementText());
	}

	@org.junit.Test
	public void testIsWhitespace() {
		assumeThat(QtVersion.getQtVersion(), isQtGe(QtVersion.VERSION_4_6));  // Version check

		while (!xmlr.atEnd()) {
			TokenType token = xmlr.readNext();
			if (token == TokenType.StartElement) {
				if (token.name().equals("persons"))
					continue;
				if (xmlr.name().equals("person")) { // [<person
													// id="John">]John</person>
					xmlr.skipCurrentElement(); // <person // API since 4.6.x
												// id="John">John[</person>]
					xmlr.readNext(); // [ ]<person id="Jane">Jane</person>
					assertTrue(xmlr.isWhitespace());// ^
					xmlr.readNext(); // [<person id="Jane">]Jane</person>
					xmlr.readNext(); // <person id="Jane">[Jane]</person>
					assertEquals(person[1], xmlr.text());
				}
			}
		}
	}

}
