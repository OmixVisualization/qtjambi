/****************************************************************************
**
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.webkit.QWebDatabase;
import org.qtjambi.qt.webkit.QWebElement;
import org.qtjambi.qt.webkit.QWebElementCollection;
import org.qtjambi.qt.webkit.QWebHistoryItem;
import org.qtjambi.qt.webkit.QWebHitTestResult;
import org.qtjambi.qt.webkit.QWebPage;
import org.qtjambi.qt.webkit.QWebPage_ViewportAttributes;
import org.qtjambi.qt.webkit.QWebPluginFactory_MimeType;
import org.qtjambi.qt.webkit.QWebPluginFactory_Plugin;
import org.qtjambi.qt.webkit.QWebSecurityOrigin;
import org.qtjambi.unittests.support.CategoryWebkit;
import org.qtjambi.unittests.support.FilterWebkit;

// Declarative support is an optional part of API:
// 1) The javac has to compile this package (this is the usual way the
//    test is deselected by having javac just not compile it)
// 2) The ANT testrunner looks over the source code folder for tests the
//    problem is that this class won't load in environment where Declarative
//    package does not exist.  FIXME
@Category(CategoryWebkit.class)
public class TestCloneableWebkit extends QApplicationTest {

    @BeforeClass
    public static void testInitialize() throws Exception {
        assumeTrue(FilterWebkit.detectStatic());
        QApplicationTest.testInitialize(null);
    }
    
	@Test
	public void run_clone_QWebElement() {
		QWebElement org = new QWebElement();
		QWebElement clone = org.clone();
		org.dispose();
		QWebElement clone2 = clone.clone();
		assertEquals(clone, clone2);
	}

	@Test
	public void run_clone_QWebPage_ViewportAttributes() {
		QWebPage page = new QWebPage();
		QWebPage_ViewportAttributes org = page.viewportAttributesForSize(new QSize(100, 100));
		QWebPage_ViewportAttributes clone = org.clone();
		org.dispose();
		QWebPage_ViewportAttributes clone2 = clone.clone();
		assertEquals(clone.size(), clone2.size());
	}

	@Test
	public void run_clone_QWebPluginFactory_MimeType() {
		QWebPluginFactory_MimeType org = new QWebPluginFactory_MimeType();
		QWebPluginFactory_MimeType clone = org.clone();
		org.dispose();
		QWebPluginFactory_MimeType clone2 = clone.clone();
		assertEquals(clone, clone2);
	}

	@Test
	public void run_clone_QWebPluginFactory_Plugin() {
		QWebPluginFactory_Plugin org = new QWebPluginFactory_Plugin();
		org.setDescription("description");
		org.setName("name");
		QWebPluginFactory_Plugin clone = org.clone();
		org.dispose();
		QWebPluginFactory_Plugin clone2 = clone.clone();
		assertEquals(clone.name(), "name");
		assertEquals(clone.description(), "description");
		assertEquals(clone.name(), clone2.name());
		assertEquals(clone.description(), clone2.description());
	}

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestCloneableWebkit.class.getName());
    }
}
