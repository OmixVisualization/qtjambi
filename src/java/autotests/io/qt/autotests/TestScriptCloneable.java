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

import static org.junit.Assert.*;

import org.junit.Test;

import io.qt.autotests.generated.General;
import io.qt.script.QScriptContextInfo;
import io.qt.script.QScriptProgram;
import io.qt.script.QScriptString;
import io.qt.script.QScriptValue;

public class TestScriptCloneable extends ApplicationInitializer {

	@Test
	public void run_clone_QScriptContextInfo() {
		QScriptContextInfo org = new QScriptContextInfo();
		QScriptContextInfo clone = org.clone();
		assertTrue(General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QScriptContextInfo clone2 = clone.clone();
		assertTrue(General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone, clone2);
	}

	@Test
	public void run_clone_QScriptProgram() {
		QScriptProgram org = new QScriptProgram();
		QScriptProgram clone = org.clone();
		assertTrue(General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QScriptProgram clone2 = clone.clone();
		assertTrue(General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone, clone2);
	}

	@Test
	public void run_clone_QScriptString() {
		QScriptString org = new QScriptString();
		QScriptString clone = org.clone();
		assertTrue(General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QScriptString clone2 = clone.clone();
		assertTrue(General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone, clone2);
	}

	@Test
	public void run_clone_QScriptValue() {
		QScriptValue org = new QScriptValue(4.5);
		QScriptValue clone = org.clone();
		assertTrue(General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QScriptValue clone2 = clone.clone();
		assertTrue(General.internalAccess.isJavaOwnership(clone2));
		assertEquals((Object)clone.toDouble(), 4.5);
		assertEquals((Object)clone.toDouble(), clone2.toDouble());
	}
}
