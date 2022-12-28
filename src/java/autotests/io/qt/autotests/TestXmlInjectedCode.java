/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import io.qt.core.QBuffer;
import io.qt.core.QByteArray;
import io.qt.core.QIODevice;
import io.qt.core.QLibraryInfo;
import io.qt.xml.QDomDocument;
import io.qt.xml.QDomElement;

public class TestXmlInjectedCode extends ApplicationInitializer {
	
    @Test
    public void testQDomElement_setAttributeNS() {
        QDomDocument document = new QDomDocument();
        QDomElement element = document.createElement("tag");
        assertTrue(element!=null);
        element.setAttributeNS("something", "foo", "bar");
        assertEquals("bar", element.attributeNS("something", "foo"));
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestXmlInjectedCode.class.getName());
    }
    
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
}
