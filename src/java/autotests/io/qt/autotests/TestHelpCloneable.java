/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import static org.junit.Assert.*;

import java.util.Arrays;
import java.util.Collections;

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.autotests.generated.*;
import io.qt.core.QMetaType;
import io.qt.core.QVersionNumber;
import io.qt.help.*;

public class TestHelpCloneable extends ApplicationInitializer {

    @BeforeClass
    public static void testInitialize() throws Exception {
        ApplicationInitializer.testInitialize();
    }
    
    @Deprecated
	@Test
	public void run_clone_QHelpSearchQuery() {
		QHelpSearchQuery org = new QHelpSearchQuery();
		org.setFieldName(QHelpSearchQuery.FieldName.ATLEAST);
		org.setWordList(Collections.singletonList("test"));
		QHelpSearchQuery clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QHelpSearchQuery clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.fieldName(), clone2.fieldName());
		assertEquals(clone.wordList(), clone2.wordList());
	}
	
	@Test
	public void run_clone_QHelpFilterData() {
		QHelpFilterData org = new QHelpFilterData();
		org.setComponents(Arrays.asList("A", "B", "C"));
		org.setVersions(Arrays.asList(new QVersionNumber(1,0,0), new QVersionNumber(2,0,0)));
		QHelpFilterData clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QHelpFilterData clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.components(), clone2.components());
		assertEquals(clone.versions(), clone2.versions());
		assertEquals(clone, clone2);
	}
	
	@Test
	public void run_clone_QHelpLink() {
		QHelpLink org = new QHelpLink();
		org.setTitle("Title");
		org.setUrl("https://www.qt.io");
		QHelpLink clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QHelpLink clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.title(), clone2.title());
		assertEquals(clone.url(), clone2.url());
	}
	
	@Test
	public void run_clone_QHelpSearchResult() {
		QHelpSearchResult org = new QHelpSearchResult("Title", "https://www.qt.io", "Snippet");
		QHelpSearchResult clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QHelpSearchResult clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.title(), clone2.title());
		assertEquals(clone.url(), clone2.url());
		assertEquals(clone.snippet(), clone2.snippet());
	}

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestHelpCloneable.class.getName());
    }
}
