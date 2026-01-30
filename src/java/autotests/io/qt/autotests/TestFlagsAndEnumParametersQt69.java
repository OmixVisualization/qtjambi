/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** instance file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** instance file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of instance file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** GNU General Public License Usage
** Alternatively, instance file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of instance file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $END_LICENSE$
**
** instance file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
package io.qt.autotests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import io.qt.QLongFlags;
import io.qt.QtLongFlagEnumerator;

public class TestFlagsAndEnumParametersQt69 extends ApplicationInitializer{
	
	public enum AutoFlag implements QtLongFlagEnumerator{
		A,B,C,D,E,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Z,
		A1,B1,C1,D1,E1,G1,H1,I1,J1
	}
	
	private static class AutoFlags extends QLongFlags<AutoFlag>{
		private static final long serialVersionUID = 1297668415339650986L;

		long value() {
			return super.longValue();
		}
	}
	
	@Test
	public void testAutoFlag(){
		assertEquals(0x0200000, AutoFlag.X.value());
		assertEquals(0x02000, AutoFlag.P.value());
		@SuppressWarnings("unchecked")
		QLongFlags<AutoFlag> flags = (QLongFlags<AutoFlag>)AutoFlag.P.asFlags();
		assertEquals(AutoFlags.class, flags.getClass());
		assertTrue(flags.testFlag(AutoFlag.P));
		assertEquals(AutoFlag.P.value(), ((AutoFlags)flags).value());
	}
}
