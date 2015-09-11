/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package org.qtjambi.autotests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.util.StringTokenizer;

import org.junit.Test;

import org.qtjambi.qt.core.QFile;
import org.qtjambi.qt.core.QIODevice.OpenModeFlag;
import org.qtjambi.qt.xml.QDomComment;
import org.qtjambi.qt.xml.QDomDocument;
import org.qtjambi.qt.xml.QDomDocument.Result;
import org.qtjambi.qt.xml.QDomElement;
import org.qtjambi.qt.xml.QDomNodeList;
import org.qtjambi.qt.xml.QDomText;

public class TestXml extends QApplicationTest {

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
        QFile file = new QFile("classpath:generator/typesystem_core.txt");
        if (!file.open(OpenModeFlag.ReadOnly))
            return;
        Result res = doc.setContent(file);
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

        assertTrue(found);

        file.close();

    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestXml.class.getName());
    }
}
