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
import static org.junit.Assume.assumeTrue;

import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import org.qtjambi.qt.script.QScriptContextInfo;
import org.qtjambi.qt.script.QScriptProgram;
import org.qtjambi.qt.script.QScriptString;
import org.qtjambi.qt.script.QScriptSyntaxCheckResult;
import org.qtjambi.qt.script.QScriptValue;
import org.qtjambi.unittests.support.CategoryScript;
import org.qtjambi.unittests.support.FilterScript;

public class TestCloneableScript extends QApplicationTest {

    @BeforeClass
    public static void testInitialize() throws Exception {
        assumeTrue(FilterScript.detectStatic());
        QApplicationTest.testInitialize(null);
    }
    
	@Test
	public void run_clone_QScriptContextInfo() {
		QScriptContextInfo org = new QScriptContextInfo();
		QScriptContextInfo clone = org.clone();
		org.dispose();
		QScriptContextInfo clone2 = clone.clone();
		assertEquals(clone, clone2);
	}

	@Test
	public void run_clone_QScriptProgram() {
		QScriptProgram org = new QScriptProgram();
		QScriptProgram clone = org.clone();
		org.dispose();
		QScriptProgram clone2 = clone.clone();
		assertEquals(clone, clone2);
	}

	@Test
	public void run_clone_QScriptString() {
		QScriptString org = new QScriptString();
		QScriptString clone = org.clone();
		org.dispose();
		QScriptString clone2 = clone.clone();
		assertEquals(clone, clone2);
	}

	@Test
	public void run_clone_QScriptValue() {
		QScriptValue org = new QScriptValue(4.5);
		QScriptValue clone = org.clone();
		org.dispose();
		QScriptValue clone2 = clone.clone();
		assertEquals((Object)clone.toDouble(), 4.5);
		assertEquals((Object)clone.toDouble(), clone2.toDouble());
	}

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestCloneableScript.class.getName());
    }
}
