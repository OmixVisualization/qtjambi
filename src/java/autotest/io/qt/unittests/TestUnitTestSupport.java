/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
package io.qt.unittests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import io.qt.unittests.support.MatcherQtVersion;
import io.qt.unittests.support.QtVersion;

public class TestUnitTestSupport {

	@org.junit.Test
	public void testQtVersion() {
		QtVersion qtVersion = new QtVersion("4.8.2");
		assertEquals("4.8.2", qtVersion.getVersion());

		final int[] versionA = qtVersion.getVersionA();
		assertEquals("[0]", 4, versionA[0]);
		assertEquals("[1]", 8, versionA[1]);
		assertEquals("[2]", 2, versionA[2]);
	}

	@org.junit.Test(expected=NullPointerException.class)
	public void testQtVersionInvalidNPE() {
		new QtVersion(null);
	}

	@org.junit.Test(expected=RuntimeException.class)
	public void testQtVersionInvalidRE1() {
		new QtVersion("");
	}

	@org.junit.Test(expected=RuntimeException.class)
	public void testQtVersionInvalidRE2() {
		new QtVersion("a");
	}

	@org.junit.Test
	public void testQtVersionCompareTo() {
		QtVersion qtVersion474 = new QtVersion("4.7.4");

		assertTrue("compareTo() < 0", (qtVersion474.compareTo(QtVersion.VERSION_5) < 0));
		assertTrue("compareTo() < 0", (qtVersion474.compareTo(QtVersion.VERSION_4_8) < 0));
		assertTrue("compareTo() > 0", (qtVersion474.compareTo(QtVersion.VERSION_4_7) > 0));
		assertTrue("compareTo() > 0", (qtVersion474.compareTo(QtVersion.VERSION_4_6) > 0));
		assertTrue("compareTo() > 0", (qtVersion474.compareTo(QtVersion.VERSION_4_5) > 0));

		QtVersion qtVersion47 = new QtVersion("4.7");

		assertTrue("compareTo() < 0", (qtVersion47.compareTo(QtVersion.VERSION_5) < 0));
		assertTrue("compareTo() < 0", (qtVersion47.compareTo(QtVersion.VERSION_4_8) < 0));
		assertTrue("compareTo() == 0", (qtVersion47.compareTo(QtVersion.VERSION_4_7) == 0));
		assertTrue("compareTo() > 0", (qtVersion47.compareTo(QtVersion.VERSION_4_6) > 0));
		assertTrue("compareTo() > 0", (qtVersion47.compareTo(QtVersion.VERSION_4_5) > 0));
	}

	@org.junit.Test
	public void testQtVersionEquals() {
		QtVersion qtVersion474 = new QtVersion("4.7.4");

		assertFalse("equals()", qtVersion474.equals(QtVersion.VERSION_5));
		assertFalse("equals()", qtVersion474.equals(QtVersion.VERSION_4_8));
		assertFalse("equals()", qtVersion474.equals(QtVersion.VERSION_4_7));
		assertFalse("equals()", qtVersion474.equals(QtVersion.VERSION_4_6));
		assertFalse("equals()", qtVersion474.equals(QtVersion.VERSION_4_5));

		QtVersion qtVersion47 = new QtVersion("4.7");

		assertFalse("equals() < 0", qtVersion47.equals(QtVersion.VERSION_5));
		assertFalse("equals() < 0", qtVersion47.equals(QtVersion.VERSION_4_8));
		assertTrue("equals() == 0", qtVersion47.equals(QtVersion.VERSION_4_7));
		assertFalse("equals() > 0", qtVersion47.equals(QtVersion.VERSION_4_6));
		assertFalse("equals() > 0", qtVersion47.equals(QtVersion.VERSION_4_5));
	}

	@org.junit.Test
	public void testIsQtGe() {
		QtVersion qtVersion474 = new QtVersion("4.7.4");

		assertFalse(MatcherQtVersion.isQtGt(qtVersion474).matches(qtVersion474));
		assertTrue(MatcherQtVersion.isQtGe(qtVersion474).matches(qtVersion474));
		assertTrue(MatcherQtVersion.isQtEq(qtVersion474).matches(qtVersion474));
		assertTrue(MatcherQtVersion.isQtLe(qtVersion474).matches(qtVersion474));
		assertFalse(MatcherQtVersion.isQtLt(qtVersion474).matches(qtVersion474));

		QtVersion qtVersion47 = new QtVersion("4.7");
		assertTrue(MatcherQtVersion.isQtGt(qtVersion474).matches(qtVersion47)); // 4.7.4 > 4.7 : true
		assertTrue(MatcherQtVersion.isQtGe(qtVersion474).matches(qtVersion47));
		assertFalse(MatcherQtVersion.isQtEq(qtVersion474).matches(qtVersion47));
		assertFalse(MatcherQtVersion.isQtLe(qtVersion474).matches(qtVersion47)); // 4.7.4 <= 4.7 : false
		assertFalse(MatcherQtVersion.isQtLt(qtVersion474).matches(qtVersion47));

		QtVersion qtVersion50 = new QtVersion("5.0");
		assertFalse(MatcherQtVersion.isQtGt(qtVersion474).matches(qtVersion50)); // 4.7.4 > 5.0 : false
		assertFalse(MatcherQtVersion.isQtGe(qtVersion474).matches(qtVersion50)); // 4.7.4 >= 5.0 : false
		assertFalse(MatcherQtVersion.isQtEq(qtVersion474).matches(qtVersion50));
		assertTrue(MatcherQtVersion.isQtLe(qtVersion474).matches(qtVersion50));
		assertTrue(MatcherQtVersion.isQtLt(qtVersion474).matches(qtVersion50));
	}

}
