/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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
**
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

import org.junit.Test;

import io.qt.core.QBuffer;
import io.qt.core.QByteArray;
import io.qt.core.QIODevice;
import io.qt.core.QLibraryInfo;
import io.qt.xml.QDomDocument;
import io.qt.xml.QDomElement;

public class TestXmlInjectedCodeQt5 extends ApplicationInitializer {
	

    
    @Test
    public void testQDomDocument_setContent() {
    	String content = "<root></root>";
    	QDomDocument doc = new QDomDocument();
    	QDomDocument.Result result = doc.setContent(content);
    	assertEquals(result.errorMessage, true, result.success);
    	assertEquals(0, result.errorColumn);
    	assertEquals(0, result.errorLine);
    	assertEquals("", result.errorMessage);
    	result = doc.setContent(new QByteArray(content));
    	assertEquals(result.errorMessage, true, result.success);
    	assertEquals(0, result.errorColumn);
    	assertEquals(0, result.errorLine);
    	assertEquals("", result.errorMessage);
    	QBuffer buffer = new QBuffer(new QByteArray(content));
    	buffer.open(QIODevice.OpenModeFlag.ReadOnly);
    	result = doc.setContent(buffer);
    	buffer.close();
    	assertEquals(result.errorMessage, true, result.success);
    	assertEquals(0, result.errorColumn);
    	assertEquals(0, result.errorLine);
    	assertEquals("", result.errorMessage);
    	result = doc.setContent(content, false);
    	assertEquals(result.errorMessage, true, result.success);
    	assertEquals(0, result.errorColumn);
    	assertEquals(0, result.errorLine);
    	assertEquals("", result.errorMessage);
    	result = doc.setContent(new QByteArray(content), false);
    	assertEquals(result.errorMessage, true, result.success);
    	assertEquals(0, result.errorColumn);
    	assertEquals(0, result.errorLine);
    	assertEquals("", result.errorMessage);
    	buffer = new QBuffer(new QByteArray(content));
    	buffer.open(QIODevice.OpenModeFlag.ReadOnly);
    	result = doc.setContent(buffer);
    	buffer.close();
    	assertEquals(result.errorMessage, true, result.success);
    	assertEquals(0, result.errorColumn);
    	assertEquals(0, result.errorLine);
    	assertEquals("", result.errorMessage);
    	result = doc.setContent("<unclosed>");
    	assertEquals(false, result.success);
    	if(QLibraryInfo.version().majorVersion()==5) {
	    	assertEquals(11, result.errorColumn);
	    	assertEquals(1, result.errorLine);
	    	assertEquals("unexpected end of file", result.errorMessage);
    	}else {
	    	assertEquals(10, result.errorColumn);
	    	assertEquals(1, result.errorLine);
	    	assertEquals("Premature end of document.", result.errorMessage);    		
    	}
    	result = doc.setContent("<unclosed></X>");
    	assertEquals(false, result.success);
    	assertEquals(14, result.errorColumn);
    	assertEquals(1, result.errorLine);
    	if(QLibraryInfo.version().majorVersion()==5) {
    		assertEquals("tag mismatch", result.errorMessage);
    	}else {
    		assertEquals("Opening and ending tag mismatch.", result.errorMessage);
    	}
    	result = doc.setContent("no XML");
    	assertEquals(false, result.success);
    	assertEquals(1, result.errorColumn);
    	assertEquals(1, result.errorLine);
    	if(QLibraryInfo.version().majorVersion()==5) {
    		assertEquals("error occurred while parsing element", result.errorMessage);
    	}else {
    		assertEquals("Start tag expected.", result.errorMessage);
    	}
    }

    @Deprecated
    static class XmlReaderSubclassSubclass extends io.qt.autotests.generated.XmlReaderSubclass {

        @Override
        public Boolean feature(String name) {
    		if(name.equals("javaTrue")) {
    			super.feature(name);
    			return true;
    		}
        	return super.feature(name);
        }

        @Override
        public io.qt.xml.QXmlContentHandler contentHandler() {
            return null;
        }

        @Override
        public io.qt.xml.QXmlDeclHandler declHandler() {
            return null;
        }

        @Override
        public io.qt.xml.QXmlDTDHandler DTDHandler() {
            return null;
        }

        @Override
        public io.qt.xml.QXmlEntityResolver entityResolver() {
            return null;
        }

        @Override
        public io.qt.xml.QXmlErrorHandler errorHandler() {
            return null;
        }

        @Override
        public boolean hasFeature(String name) {
            return name.equals("javaTrue");
        }

        @Override
        public io.qt.xml.QXmlLexicalHandler lexicalHandler() {
            return null;
        }

        @Override
        public boolean parse(io.qt.xml.QXmlInputSource input) {
            return false;
        }

        @Override
        public void setContentHandler(io.qt.xml.QXmlContentHandler handler) {

        }

        @Override
        public void setDeclHandler(io.qt.xml.QXmlDeclHandler handler) {

        }

        @Override
        public void setDTDHandler(io.qt.xml.QXmlDTDHandler handler) {

        }

        @Override
        public void setEntityResolver(io.qt.xml.QXmlEntityResolver handler) {

        }

        @Override
        public void setErrorHandler(io.qt.xml.QXmlErrorHandler handler) {

        }

        @Override
        public void setFeature(String name, boolean value) {

        }

        @Override
        public void setLexicalHandler(io.qt.xml.QXmlLexicalHandler handler) {

        }

    }
    
    @Test
    @Deprecated
    public void testQDomDocument_setXMLContent() {
    	String content = "<root></root>";
    	QDomDocument doc = new QDomDocument();
    	io.qt.xml.QXmlInputSource source = new io.qt.xml.QXmlInputSource(new QBuffer(new QByteArray(content)));
    	QDomDocument.Result result = doc.setContent(source, false);
    	assertEquals(true, result.success);
    	assertEquals(0, result.errorColumn);
    	assertEquals(0, result.errorLine);
    	assertEquals("", result.errorMessage);
    	io.qt.xml.QXmlReader reader = new io.qt.xml.QXmlSimpleReader();
    	source = new io.qt.xml.QXmlInputSource(new QBuffer(new QByteArray(content)));
    	result = doc.setContent(source, reader);
    	assertEquals(true, result.success);
    	assertEquals(0, result.errorColumn);
    	assertEquals(0, result.errorLine);
    	assertEquals("", result.errorMessage);
    }
    
    @Test
    @Deprecated
    public void testQXmlEntityResolver_resolveEntity() {
    	io.qt.xml.QXmlEntityResolver handler = new io.qt.xml.QXmlDefaultHandler();
    	io.qt.xml.QXmlEntityResolver.ResolvedEntity entity = handler.resolveEntity("http://io.qt", "http://io.qt");
    	assertFalse(entity.error);
    	assertEquals(null, entity.inputSource);
    }
    
    @Test
    @Deprecated
    public void testQXmlNamespaceSupport_processName() {
    	io.qt.xml.QXmlNamespaceSupport handler = new io.qt.xml.QXmlNamespaceSupport();
    	io.qt.xml.QXmlNamespaceSupport.ProcessedName result = handler.processName("NS:NAME", true);
    	assertEquals("NAME", result.localName);
    	assertEquals("", result.nsuri);
    }
    
    @Test
    @Deprecated
    public void testQXmlNamespaceSupport_splitName() {
    	io.qt.xml.QXmlNamespaceSupport handler = new io.qt.xml.QXmlNamespaceSupport();
    	io.qt.xml.QXmlNamespaceSupport.SplitName result = handler.splitName("NS:NAME");
    	assertEquals("NAME", result.localname);
    	assertEquals("NS", result.prefix);
    }

    @Test
    @Deprecated
    public void testQXmlReaderFeature() {
        XmlReaderSubclassSubclass xrss = new XmlReaderSubclassSubclass();

        assertFalse(xrss.callFeature("javaFalse"));
        assertEquals("javaFalse", xrss.myName());
        assertFalse(xrss.myOk());

        assertTrue(xrss.callFeature("javaTrue"));
        assertEquals("javaTrue", xrss.myName());
        assertTrue(xrss.myOk());

        xrss.setMyOk(false);
        assertFalse(xrss.callFeature("true"));
        assertEquals("true", xrss.myName());
        assertFalse(xrss.myOk());
    }
    


    @Deprecated
    static class XmlEntityResolverSubclassSubclass extends io.qt.autotests.generated.XmlEntityResolverSubclass{

        @Override
        public ResolvedEntity resolveEntity(String publicId, String systemId) {
            if (publicId.equals("In java")) {
            	io.qt.xml.QXmlInputSource src = new io.qt.xml.QXmlInputSource();
                src.setData("Made in Java");
                return new ResolvedEntity(systemId.equals("error"), src);
            } else {
                return super.resolveEntity(publicId, systemId);
            }
        }

        @Override
        public String errorString() {
            return null;
        }

    }


    @Test
    @Deprecated
    public void QXmlEntityResolverResolveEntityMadeInJava() {
        XmlEntityResolverSubclassSubclass xerss = new XmlEntityResolverSubclassSubclass();
        io.qt.xml.QXmlInputSource src = xerss.callResolveEntity("In java", "");
        assertEquals("Made in Java", src.data());
    }

    @Test
    @Deprecated
    public void QXmlEntityResolverResolveEntityMadeInJavaWithError() {
        XmlEntityResolverSubclassSubclass xerss = new XmlEntityResolverSubclassSubclass();
        io.qt.xml.QXmlInputSource src = xerss.callResolveEntity("In java", "error");
        assertEquals("Made in Java with error", src.data());
    }

    @Test
    @Deprecated
    public void QXmlEntityResolverResolveEntityMadeInCpp() {
        XmlEntityResolverSubclassSubclass xerss = new XmlEntityResolverSubclassSubclass();
        io.qt.xml.QXmlInputSource src = xerss.callResolveEntity("c++", "");
        assertEquals("Made in C++", src.data());
    }

    @Test
    @Deprecated
    public void QXmlEntityResolverResolveEntityMadeInCppWithError() {
        XmlEntityResolverSubclassSubclass xerss = new XmlEntityResolverSubclassSubclass();
        io.qt.xml.QXmlInputSource src = xerss.callResolveEntity("c++", "error");
        assertEquals("Made in C++ with error", src.data());
    }
}
