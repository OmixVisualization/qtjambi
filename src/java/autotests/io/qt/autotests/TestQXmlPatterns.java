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

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import io.qt.autotests.ApplicationInitializer;
import io.qt.core.QBuffer;
import io.qt.core.QIODevice;
import io.qt.core.QUrl;
import io.qt.core.QtMsgType;
import io.qt.xmlpatterns.QAbstractMessageHandler;
import io.qt.xmlpatterns.QSourceLocation;
import io.qt.xmlpatterns.QXmlFormatter;
import io.qt.xmlpatterns.QXmlItem;
import io.qt.xmlpatterns.QXmlName;
import io.qt.xmlpatterns.QXmlQuery;
import io.qt.xmlpatterns.QXmlResultItems;
import io.qt.xmlpatterns.QXmlSerializer;

public class TestQXmlPatterns extends ApplicationInitializer {

    QXmlQuery query;
    QXmlSerializer serializer;
    QXmlResultItems xmlResultItems;
    QXmlFormatter formatter;
    String samplePath1 = "doc('qrc:/io/qt/autotests/xquerySample1.xml')";
    String samplePath2 = "doc('qrc:/io/qt/autotests/xmlSample2.xml')";

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
        String result = query.evaluateToString();
        assertTrue(result!=null);

        String[] expected = { "<firstname>John</firstname>",
                "<firstname>Jane</firstname>", "<firstname>Baby</firstname>" };
        String[] results = result.split("\\\n");

        int i = 0;
        for (String str : results) {
            assertEquals(expected[i++], str);
        }
    }
    
    @Test
    public void testSubclassMessageHandler() {
    	new QAbstractMessageHandler() {
			@Override
			protected void handleMessage(QtMsgType type, String description, QUrl identifier, QSourceLocation sourceLocation) {
			}
		};
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
                String s = query.evaluateToString();
                assertTrue(s!=null);
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
