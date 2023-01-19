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
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import io.qt.autotests.generated.General;
import io.qt.xml.QDomAttr;
import io.qt.xml.QDomCDATASection;
import io.qt.xml.QDomCharacterData;
import io.qt.xml.QDomComment;
import io.qt.xml.QDomDocument;
import io.qt.xml.QDomDocumentFragment;
import io.qt.xml.QDomDocumentType;
import io.qt.xml.QDomElement;
import io.qt.xml.QDomEntity;
import io.qt.xml.QDomEntityReference;
import io.qt.xml.QDomImplementation;
import io.qt.xml.QDomNamedNodeMap;
import io.qt.xml.QDomNode;
import io.qt.xml.QDomNodeList;
import io.qt.xml.QDomNotation;
import io.qt.xml.QDomProcessingInstruction;
import io.qt.xml.QDomText;

public class TestXmlCloneable extends ApplicationInitializer {
    @Test
    public void run_clone_QDomAttr() {
        QDomAttr org = new QDomAttr();
        QDomAttr clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QDomAttr clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QDomCDATASection() {
        QDomCDATASection org = new QDomCDATASection();
        QDomCDATASection clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QDomCDATASection clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QDomCharacterData() {
        QDomCharacterData org = new QDomCharacterData();
        QDomCharacterData clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QDomCharacterData clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QDomComment() {
        QDomComment org = new QDomComment();
        QDomComment clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QDomComment clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QDomDocument() {
        QDomDocument org = new QDomDocument();
        QDomDocument clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QDomDocument clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QDomDocumentFragment() {
        QDomDocumentFragment org = new QDomDocumentFragment();
        QDomDocumentFragment clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QDomDocumentFragment clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QDomDocumentType() {
        QDomDocumentType org = new QDomDocumentType();
        QDomDocumentType clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QDomDocumentType clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QDomElement() {
        QDomElement org = new QDomElement();
        QDomElement clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QDomElement clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QDomEntity() {
        QDomEntity org = new QDomEntity();
        QDomEntity clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QDomEntity clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QDomEntityReference() {
        QDomEntityReference org = new QDomEntityReference();
        QDomEntityReference clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QDomEntityReference clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QDomImplementation() {
        QDomImplementation org = new QDomImplementation();
        QDomImplementation clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QDomImplementation clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QDomNamedNodeMap() {
        QDomNamedNodeMap org = new QDomNamedNodeMap();
        QDomNamedNodeMap clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QDomNamedNodeMap clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QDomNode() {
        QDomNode org = new QDomNode();
        QDomNode clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QDomNode clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QDomNodeList() {
        QDomNodeList org = new QDomNodeList();
        QDomNodeList clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QDomNodeList clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QDomNotation() {
        QDomNotation org = new QDomNotation();
        QDomNotation clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QDomNotation clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QDomProcessingInstruction() {
        QDomProcessingInstruction org = new QDomProcessingInstruction();
        QDomProcessingInstruction clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QDomProcessingInstruction clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QDomText() {
        QDomText org = new QDomText();
        QDomText clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QDomText clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

}
