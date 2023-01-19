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
import static org.junit.Assert.assertTrue;

import java.util.StringTokenizer;

import org.junit.Test;

import io.qt.core.QFile;
import io.qt.core.QIODevice;
import io.qt.xml.QDomComment;
import io.qt.xml.QDomDocument;
import io.qt.xml.QDomDocument.Result;
import io.qt.xml.QDomElement;
import io.qt.xml.QDomNodeList;
import io.qt.xml.QDomText;

public class TestXml extends ApplicationInitializer {

    private void compare(String correct, String test) {
        StringTokenizer tokTest = new StringTokenizer(test, "\n");
        StringTokenizer tokCorrect = new StringTokenizer(correct, "\n");

        assertEquals(tokTest.countTokens(), tokCorrect.countTokens());

        while (tokTest.hasMoreTokens()) {
            assertEquals(tokTest.nextToken().trim(), tokCorrect.nextToken().trim());
        }
    }

    @Test
    public void makeDocument() {
        QDomDocument doc = new QDomDocument("MyML");
        QDomElement root = doc.createElement("MyML");
        doc.appendChild(root);

        QDomElement tag = doc.createElement("Greeting");
        root.appendChild(tag);

        QDomText t = doc.createTextNode("Hello World");
        tag.appendChild(t);

        QDomComment c = doc.createComment("comment");
        root.appendChild(c);

        compare("<!DOCTYPE MyML>\n" + "<MyML>\n" + "<Greeting>Hello World</Greeting>\n" + "<!--comment-->\n" + "</MyML>\n",

        doc.toString());
    }

    @Test
    public void readDocument() {
        QDomDocument doc = new QDomDocument("mydocument");
        QFile file = new QFile(":generator/typesystem_core.txt");
        if (!file.open(QIODevice.OpenModeFlag.ReadOnly))
            return;
        Result res = doc.setContent(file, false);
        if (!res.success) {
            file.close();
            assertTrue("Failed to open file", false);
            return;
        }

        QDomNodeList list = doc.elementsByTagName("access");

        boolean found = false;

        for (int i = 0; i < list.size(); i++) {
            QDomElement element = list.at(i).toElement();

            found = found || "private".equals(element.attribute("modifier"));
        }
        file.close();
        assertTrue(found);
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestXml.class.getName());
    }
}
