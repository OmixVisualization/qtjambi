/****************************************************************************
**
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package org.qtjambi.autotests;

import static org.junit.Assert.*;
import static org.junit.Assume.assumeTrue;

import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import org.qtjambi.qt.core.QBuffer;
import org.qtjambi.qt.core.QIODevice;
import org.qtjambi.qt.xmlpatterns.QAbstractXmlReceiver;
import org.qtjambi.qt.xmlpatterns.QSourceLocation;
import org.qtjambi.qt.xmlpatterns.QXmlFormatter;
import org.qtjambi.qt.xmlpatterns.QXmlItem;
import org.qtjambi.qt.xmlpatterns.QXmlName;
import org.qtjambi.qt.xmlpatterns.QXmlNamePool;
import org.qtjambi.qt.xmlpatterns.QXmlNodeModelIndex;
import org.qtjambi.qt.xmlpatterns.QXmlQuery;
import org.qtjambi.qt.xmlpatterns.QXmlSchema;
import org.qtjambi.qt.xmlpatterns.QXmlSerializer;
import org.qtjambi.unittests.support.CategoryXmlpatterns;
import org.qtjambi.unittests.support.FilterXmlpatterns;

public class TestCloneableXmlpatterns extends QApplicationTest {

    @BeforeClass
    public static void testInitialize() throws Exception {
        assumeTrue(FilterXmlpatterns.detectStatic());
        QApplicationTest.testInitialize(null);
    }
    
	@Test
	public void run_clone_QSourceLocation() {
		QSourceLocation org = new QSourceLocation();
		QSourceLocation clone = org.clone();
		org.dispose();
		QSourceLocation clone2 = clone.clone();
		assertEquals(clone, clone2);
	}

	@Test
	public void run_clone_QXmlName() {
		QXmlName org = new QXmlName();
		QXmlName clone = org.clone();
		org.dispose();
		QXmlName clone2 = clone.clone();
		assertEquals(clone, clone2);
	}

	@Test
	public void run_clone_QXmlNodeModelIndex() {
		QXmlNodeModelIndex org = new QXmlNodeModelIndex();
		QXmlNodeModelIndex clone = org.clone();
		org.dispose();
		QXmlNodeModelIndex clone2 = clone.clone();
		assertEquals(clone, clone2);
	}

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestCloneableXmlpatterns.class.getName());
    }
}
