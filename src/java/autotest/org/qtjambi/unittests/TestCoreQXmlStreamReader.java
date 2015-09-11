package org.qtjambi.unittests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import org.qtjambi.qt.core.QFile;
import org.qtjambi.qt.core.QIODevice;
import org.qtjambi.qt.core.QXmlStreamAttributes;
import org.qtjambi.qt.core.QXmlStreamReader;
import org.qtjambi.qt.core.QXmlStreamReader.TokenType;

public class TestCoreQXmlStreamReader extends QApplicationTest {

	private QXmlStreamReader xmlr;
	private QXmlStreamReader xmlrNoDevice;
	private QFile xmlFile;
	private TokenType token;
	private final String person[] = { "John", "Jane" };
	private final String namespace[] = { "c", "d" };
	private final String namespaceuri[] = { "http://qt-jambi.org",
			"http://en.wikipedia.org" };
	private int i = 0;

	//@org.junit.BeforeClass
	//public static void setUpClass() {
	//	QAbstractFileEngine.addSearchPathForResourceEngine(".");
	//}

	@org.junit.Before
	public void setUp() throws Exception {
		xmlFile = new QFile("classpath:org/qtjambi/unittests/xmlSample1.xml");
		xmlFile.open(new QIODevice.OpenMode(QIODevice.OpenModeFlag.ReadOnly));
		xmlr = new QXmlStreamReader(xmlFile);
		xmlrNoDevice = new QXmlStreamReader();
		/*
		 * placing the first .readNext() here because the first token is XML
		 * definition the definition
		 */
		token = xmlr.readNext();
	}

	@org.junit.After
	public void tearDown() throws Exception {
		xmlFile.close();
	}

	@org.junit.Test
	public void testStartDocument() {
		assertEquals(TokenType.StartDocument, token);
		assertTrue(xmlr.isStartDocument());
	}

	@org.junit.Test
	public void testTokenType() {
		assertEquals(TokenType.StartDocument, xmlr.tokenType());
		assertTrue(xmlr.isStartDocument());
		while (!xmlr.atEnd()) {
			token = xmlr.readNext();
			if (token == TokenType.StartElement) {
				assertEquals(TokenType.StartElement, xmlr.tokenType());
				assertTrue(xmlr.isStartElement());
				if (token.name().equals("persons"))
					continue;
				if (xmlr.name().equals("person")) {
					// skip the text() of StartElement token
					xmlr.readNext();
					assertEquals(TokenType.Characters, xmlr.tokenType());
					assertTrue(xmlr.isCharacters());
				}
			} else if (token == TokenType.EndElement) {
				assertEquals(TokenType.EndElement, xmlr.tokenType());
				assertTrue(xmlr.isEndElement());
			}
		}
		assertEquals(TokenType.EndDocument, xmlr.tokenType());
		assertTrue(xmlr.isEndDocument());
	}

	@org.junit.Test
	public void testDocumentVersion() {
		assertEquals("1.0", xmlr.documentVersion());
	}

	@org.junit.Test
	public void testStartElement() {
		TokenType token = xmlr.readNext();
		assertEquals(TokenType.StartElement, token);
	}

	@org.junit.Test
	public void testName() {
		xmlr.readNext();
		assertEquals("persons", xmlr.name());
	}

	@org.junit.Test
	public void testProcessing1() {
		while (!xmlr.atEnd()) {
			token = xmlr.readNext();
			if (token == TokenType.StartElement) {
				if (token.name().equals("persons"))
					continue;
				if (xmlr.name().equals("person")) {
					// skip the text() of StartElement token
					xmlr.readNext();
					assertEquals(person[i++], xmlr.text());
				}
			}
		}
	}

	@org.junit.Test
	public void testAttributes() {
		QXmlStreamAttributes attr;
		while (!xmlr.atEnd()) {
			token = xmlr.readNext();
			if (token == TokenType.StartElement) {
				if (token.name().equals("persons"))
					continue;
				if (xmlr.name().equals("person")) {
					// attributes extracted from <person></person>
					attr = xmlr.attributes();
					assertEquals(1, attr.count());
					assertEquals(person[i++], attr.value("id"));
				}
			}
		}
	}

	/**
	 * The API XmlReader.skipCurrentElement() didn't exist until
	 *  4.6.x so we do it manually.
	 * This presumes we start off already having read the StartElement
	 *  so are inside the element.
	 */
	private void mySkipCurrentElement(QXmlStreamReader reader) {
		// We're inside the element already so a value < 0 means we
		//  read one more EndElement and can return.
		int nestedLevel = 0;
		do {
			TokenType tt = reader.readNext();
			if(tt == TokenType.EndElement)
				nestedLevel--;
			else if(tt == TokenType.StartElement)
				nestedLevel++;
		} while(nestedLevel >= 0);
	}

	/**
	 * The API XmlReader.readNextStartElement() didn't exist until
	 *  4.6.x so we do it manually.
	 * This presumes we start off already having read the StartElement
	 *  so are inside the element.
	 */
	private boolean myReadNextStartElement(QXmlStreamReader reader) {
		while(true) {
			if(reader.atEnd())
				return false;
			TokenType tt = reader.readNext();
			if(tt == TokenType.EndElement)
				return false;
			else if(tt == TokenType.StartElement)
				return true;
		}
	}

	@org.junit.Test
	public void testSkipCurrentElement() {
		while (!xmlr.atEnd()) {
			token = xmlr.readNext();
			if (token == TokenType.StartElement) {
				if (token.name().equals("persons"))
					continue;
				if (xmlr.name().equals("person")) { // [<person
													// id="John">]John</person>
					mySkipCurrentElement(xmlr); // <person
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
	public void testReadElementText() {
		while (!xmlr.atEnd()) {
			token = xmlr.readNext();
			if (token == TokenType.StartElement) {
				if (token.name().equals("persons"))
					continue;
				if (xmlr.name().equals("person")) {
					//
					assertEquals(TokenType.StartElement, xmlr.tokenType());
					// consume the start element completely
					assertEquals(person[i++], xmlr.readElementText());
					assertEquals(TokenType.EndElement, xmlr.tokenType());
				}
			}
		}
	}

	@org.junit.Test
	public void testLineNumber() {
		// StartDocument
		assertEquals(1, xmlr.lineNumber());
		xmlr.readNext();
		// StartElement - <persons></persons>
		assertEquals(2, xmlr.lineNumber());
		// StartElement - <person></person>
		myReadNextStartElement(xmlr);
		assertEquals(3, xmlr.lineNumber());
		assertEquals("John", xmlr.readElementText());
	}

	@org.junit.Test
	public void testIsWhitespace() {
		while (!xmlr.atEnd()) {
			token = xmlr.readNext();
			if (token == TokenType.StartElement) {
				if (token.name().equals("persons"))
					continue;
				if (xmlr.name().equals("person")) { // [<person
													// id="John">]John</person>
					mySkipCurrentElement(xmlr); // <person
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

	@org.junit.Test
	public void testAddData() {
		StringBuilder sb = new StringBuilder();
		sb.append("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
		sb.append("<persons>");
		sb.append("<person>");
		sb.append("John");
		sb.append("</person>");
		sb.append("<person>");
		sb.append("Jane");
		sb.append("</person>");
		sb.append("</persons>");
		xmlrNoDevice.addData(sb.toString());
		assertEquals(TokenType.StartDocument, xmlrNoDevice.readNext());
		assertEquals("1.0", xmlrNoDevice.documentVersion());
		// xmlrNoDevice.skipCurrentElement();
		while (!xmlrNoDevice.atEnd()) {
			token = xmlrNoDevice.readNext();
			if (token == TokenType.StartElement) {
				if (token.name().equals("persons"))
					continue;
				if (xmlrNoDevice.name().equals("person")) {
					//
					assertEquals(TokenType.StartElement, xmlrNoDevice.tokenType());
					// consume the start element completely
					assertEquals(person[i++], xmlrNoDevice.readElementText());
					assertEquals(TokenType.EndElement, xmlrNoDevice.tokenType());
				}
			}
		}
	}

	@org.junit.Test
	public void testRaiseError() {
		while (!xmlr.atEnd() && !xmlr.hasError()) {
			token = xmlr.readNext();
			if (token == TokenType.StartElement) {
				if (token.name().equals("persons"))
					continue;
				if (xmlr.name().equals("person")) {
					assertEquals("John", xmlr.readElementText());
					xmlr.raiseError("An error occurred...");
				}
			}
		}
		assertEquals("An error occurred...", xmlr.errorString());
		assertEquals(TokenType.Invalid, xmlr.tokenType());
	}

	@org.junit.Test
	public void testNamespace() {
		while (!xmlr.atEnd() && !xmlr.hasError()) {
			token = xmlr.readNext();
			if (token == TokenType.StartElement) {
				if (token.name().equals("persons"))
					continue;
				if (xmlr.name().equals("person")) {
					assertEquals(xmlr.qualifiedName(), xmlr.prefix() + ":"
							+ xmlr.name());
					assertEquals(namespaceuri[i++], xmlr.namespaceUri());
				}
			}
		}
	}

}
