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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;

import io.qt.QtUtilities;
import io.qt.core.QPair;

public class TestQPair extends UnitTestInitializer {

	QPair<Integer, Integer> qp1;
	QPair<Integer, Integer> qp2;
	QPair<Integer, Boolean> qp3;
	QPair<Integer, Boolean> qp4;
	QPair<Integer, Integer> qp6;
	QPair<Integer, Integer> qp7;
	Object someObject;

	@org.junit.Before
	public void setUp() {
		someObject = null;
		qp1 = new QPair<Integer, Integer>(3, 5);
		qp2 = new QPair<Integer, Integer>(5, 3);
		qp3 = new QPair<Integer, Boolean>(1, null);
		qp4 = new QPair<Integer, Boolean>(null, true);
		qp6 = new QPair<Integer, Integer>(3, 6);
		qp7 = new QPair<Integer, Integer>(4, 5);
	}

	@org.junit.After
	public void tearDown() {
		qp1 = null;
		qp2 = null;
		qp3 = null;
		qp4 = null;
		qp6 = null;
		qp7 = null;
		someObject = null;
	}

	@org.junit.Test
	public void testEquals() {
		//@if
		//other == null
		assertFalse("qp1 vs someObject with null", qp1.equals(someObject));
		//other.second == null
		assertFalse("qp1 vs qp3 with null S", qp1.equals(qp3));
		//other.first == null
		assertFalse("qp1 vs qp4 with null T", qp1.equals(qp4));
		//first == null
		assertFalse("qp3 null S vs qp1", qp3.equals(qp1));
		//second == null
		assertFalse("qp4 null T vs qp1", qp4.equals(qp1));
		
		//@return
		//first equals
		assertFalse("qp1 vs qp6 with matching T", qp1.equals(qp6));
		//second equals
		assertFalse("qp1 vs qp7 with matching S", qp1.equals(qp7));
	}

	@org.junit.Test
	public void testToString() {
		if(QtUtilities.qtjambiVersion().majorVersion()>5) {
			assertEquals("qp1", "std::pair(3,5)",       qp1.toString());
			assertEquals("qp3", "std::pair(1,null)",    qp3.toString());
			assertEquals("qp4", "std::pair(null,true)", qp4.toString());
		}else {
			assertEquals("qp1", "QPair(3,5)",       qp1.toString());
			assertEquals("qp3", "QPair(1,null)",    qp3.toString());
			assertEquals("qp4", "QPair(null,true)", qp4.toString());
		}
	}

	@org.junit.Test
	public void testClone() {
		qp1 = qp2.clone();
		assertEquals("qp1==qp2", qp1, qp2);
	}

}
