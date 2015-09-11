package org.qtjambi.unittests;

import static junit.framework.Assert.assertNotNull;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import junit.framework.Assert;

import org.junit.After;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import org.qtjambi.qt.core.QAbstractFileEngine;
import org.qtjambi.qt.core.QBuffer;
import org.qtjambi.qt.core.QIODevice;
import org.qtjambi.qt.core.QIODevice.OpenMode;
import org.qtjambi.qt.qtjambi.util.RetroTranslatorHelper;
import org.qtjambi.qt.xmlpatterns.QXmlFormatter;
import org.qtjambi.qt.xmlpatterns.QXmlItem;
import org.qtjambi.qt.xmlpatterns.QXmlName;
import org.qtjambi.qt.xmlpatterns.QXmlQuery;
import org.qtjambi.qt.xmlpatterns.QXmlResultItems;
import org.qtjambi.qt.xmlpatterns.QXmlSerializer;

public class TestQXmlFormatter extends QApplicationTest {

    QXmlQuery query;
    QXmlSerializer serializer;
    QXmlResultItems xmlResultItems;
    QXmlFormatter formatter;
    String samplePath1 = "doc('classpath:org/qtjambi/unittests/xquerySample1.xml')";
    String samplePath2 = "doc('classpath:org/qtjambi/unittests/xmlSample2.xml')";

    //@BeforeClass
    //public static void init() {
    //    QAbstractFileEngine.addSearchPathForResourceEngine(".");
    //}

    @Before
    public void setUp() throws Exception {
        query = new QXmlQuery();
        xmlResultItems = new QXmlResultItems();
    }

    @After
    public void tearDown() throws Exception {
        query = null;
    }

    @Test
    public void testConstruct() {
        assertFalse(query == null);
        assertFalse(query.isValid());
    }

    @Test
    public void testValid() {
        query.setQuery("doc('')");
        assertTrue(query.isValid());
    }

    @Test
    public void testEvaluateToString() {
        query.setQuery(samplePath2 + "/persons/person/firstname[1]");
        assertTrue(query.isValid());
        String result = query.evaluateTo();
        assertNotNull(result);

        String[] expected = { "<firstname>John</firstname>",
                "<firstname>Jane</firstname>", "<firstname>Baby</firstname>" };
        String[] results = RetroTranslatorHelper.split(result, "\n");

        int i = 0;
        for (String str : results) {
            assertEquals(expected[i++], str);
        }
    }

    @Test
    public void testEvaluateToQXmlSerializer() {
        query.setQuery(samplePath1 + "/a/p");
        QBuffer buffer = new QBuffer();
        assertTrue(buffer.open(QIODevice.OpenModeFlag.ReadWrite));
        TestSerializerClass clazz = new TestSerializerClass(query, buffer);
        assertTrue(query.isValid());
        query.evaluateTo(clazz.outputDevice());
    }
    
    @Test
    public void testEvaluateToQXmlResultItems() {
        query.setQuery(samplePath1 + "/a/p");
        assertTrue(query.isValid());
        query.evaluateTo(xmlResultItems);
        
        QXmlItem item = xmlResultItems.next();
        while(!item.isNull()) {
            if (item.isNode()) {
                query.setFocus(item);
                query.setQuery(samplePath1 + "/a/p/string()");
                String s = query.evaluateTo();
                assertNotNull(s);
                // We are seeing EOL characters appended to the results here
                if(s.length() > 0 && Character.codePointAt(s, s.length() - 1) == '\n')
                    s = s.substring(0, s.length() - 1);
                assertEquals("Some Text in p", s);
            }
            item = xmlResultItems.next();
        }
    }
    
}

class TestSerializerClass extends QXmlSerializer {

    public TestSerializerClass(QXmlQuery query, QIODevice outputDevice) {
        super(query, outputDevice);
        // TODO Auto-generated constructor stub
    }

    @Override
    public void attribute(QXmlName name, String value) {
        System.out.println("TestSerializerClass#attribute(QXmlName,String): " + ((value != null) ? value.toString() : value));
    }

    @Override
    public void atomicValue(Object value) {
        System.out.println("TestSerializerClass#atomicValue(Object): " + ((value != null) ? value.toString() : value));
    }

    @Override
    public void startDocument() {
        System.out.println("TestSerializerClass#startDocument(): doc start");
    }

    @Override
    public void characters(String str) {
        System.out.println("TestSerializerClass#characters(String): " + str);
    }
}
