/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

package io.qt.autotests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assume.assumeTrue;

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.internal.QtJambiInternal;
import io.qt.xmlpatterns.QSourceLocation;
import io.qt.xmlpatterns.QXmlName;
import io.qt.xmlpatterns.QXmlNodeModelIndex;

public class TestXmlpatternsCloneable extends ApplicationInitializer {

	@Test
	public void run_clone_QSourceLocation() {
		QSourceLocation org = new QSourceLocation();
		QSourceLocation clone = org.clone();
		assertEquals(QtJambiInternal.Ownership.Java, QtJambiInternal.ownership(clone));
		org.dispose();
		QSourceLocation clone2 = clone.clone();
		assertEquals(QtJambiInternal.Ownership.Java, QtJambiInternal.ownership(clone2));
		assertEquals(clone, clone2);
	}

	@Test
	public void run_clone_QXmlName() {
		QXmlName org = new QXmlName();
		QXmlName clone = org.clone();
		assertEquals(QtJambiInternal.Ownership.Java, QtJambiInternal.ownership(clone));
		org.dispose();
		QXmlName clone2 = clone.clone();
		assertEquals(QtJambiInternal.Ownership.Java, QtJambiInternal.ownership(clone2));
		assertEquals(clone, clone2);
	}

	@Test
	public void run_clone_QXmlNodeModelIndex() {
		QXmlNodeModelIndex org = new QXmlNodeModelIndex();
		QXmlNodeModelIndex clone = org.clone();
		assertEquals(QtJambiInternal.Ownership.Java, QtJambiInternal.ownership(clone));
		org.dispose();
		QXmlNodeModelIndex clone2 = clone.clone();
		assertEquals(QtJambiInternal.Ownership.Java, QtJambiInternal.ownership(clone2));
		assertEquals(clone, clone2);
	}
}
