/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import io.qt.core.QByteArray;

public class TestQByteArray extends ApplicationInitializer{
	@Test
	public void testNull() {
		String strg = null;
		QByteArray ba = new QByteArray(strg);
		assertTrue(ba.isNull());
		assertEquals(0, ba.data().capacity());
	}
	
	@Test
	public void testEmptyString() {
		QByteArray ba = new QByteArray("");
		assertTrue(ba.isNull());
		assertEquals(0, ba.data().capacity());
	}
	
	@Test
	public void testString() {
		QByteArray ba = new QByteArray("TEST");
		assertEquals(4, ba.data().capacity());
		assertArrayEquals(new byte[]{(byte)'T',(byte)'E',(byte)'S',(byte)'T'}, ba.toArray());
	}

    @Test
    public void testToString()
    {
        QByteArray ba = new QByteArray("Pretty flowers æøå");
        assertEquals("Pretty flowers æøå", ba.toString());
    }
}
