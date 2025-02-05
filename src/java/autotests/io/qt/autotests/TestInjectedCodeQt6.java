/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package io.qt.autotests;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;

import java.nio.BufferOverflowException;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;

import org.junit.Assert;
import org.junit.Test;

import io.qt.autotests.generated.General;
import io.qt.core.QLineF;
import io.qt.core.QList;
import io.qt.core.QPointF;

public class TestInjectedCodeQt6 extends ApplicationInitializer {
    
	@Test
    public void testByteArrayView() {
		io.qt.core.QByteArrayView bv = new io.qt.core.QByteArrayView("ByteArray\0View".getBytes());
		for(@SuppressWarnings("unused") Byte b : bv) {
		}
		assertEquals(new io.qt.core.QByteArray("ByteArray\0View".getBytes()), bv.toByteArray());
		assertEquals("ByteArray\0View", bv.toString());
		assertEquals("ByteArray\0View", new String(bv.toArray()));
		bv.dispose();
		bv = new io.qt.core.QByteArrayView(new io.qt.core.QByteArray("ByteArray\0View".getBytes()));
		assertEquals(new io.qt.core.QByteArray("ByteArray\0View".getBytes()), bv.toByteArray());
		assertEquals("ByteArray\0View", bv.toString());
		assertEquals("ByteArray\0View", new String(bv.toArray()));
		bv.dispose();
		bv = new io.qt.core.QByteArrayView("ByteArray\0View".getBytes());
		assertEquals(new io.qt.core.QByteArray("ByteArray\0View".getBytes()), bv.toByteArray());
		assertEquals("ByteArray\0View", bv.toString());
		assertEquals("ByteArray\0View", new String(bv.toArray()));
		bv.dispose();
		bv = new io.qt.core.QByteArrayView("ByteArrayView");
		assertEquals(new io.qt.core.QByteArray("ByteArrayView"), bv.toByteArray());
		assertEquals("ByteArrayView", bv.toString());
		assertEquals("ByteArrayView", new String(bv.toArray()));
		bv.dispose();
		bv = new io.qt.core.QByteArrayView(ByteBuffer.wrap("ByteArray\0View".getBytes()));
		assertEquals("ByteArray\0View", bv.toByteArray().toString());
		assertEquals("ByteArray\0View", bv.toString());
		assertEquals("ByteArray\0View", new String(bv.toArray()));
		bv.dispose();
		bv = new io.qt.core.QByteArrayView("ByteArrayView".getBytes(), 2, 4);
		assertEquals(new io.qt.core.QByteArray("teAr"), bv.toByteArray());
		assertEquals("teAr", bv.toString());
		assertEquals("teAr", new String(bv.toArray()));
		bv.dispose();
		bv = new io.qt.core.QByteArrayView(ByteBuffer.wrap("ByteArrayView".getBytes(), 2, 4));
		assertEquals(new io.qt.core.QByteArray("teAr"), bv.toByteArray());
		assertEquals("teAr", bv.toString());
		assertEquals("teAr", new String(bv.toArray()));
		bv.dispose();
	}
	
    @Test
    public void testQLineFintersection() {
        QLineF line1 = new QLineF(10, 0, 10, 20);
        QLineF line2 = new QLineF(0, 10, 20, 10);

        QPointF intersectionPoint = new QPointF();

        assertEquals(QLineF.IntersectionType.BoundedIntersection, line1.intersects(line2, intersectionPoint));
        assertEquals(10.0, intersectionPoint.x(), 0.0);
        assertEquals(10.0, intersectionPoint.y(), 0.0);

        line2 = new QLineF(0, 30, 20, 30);
        assertEquals(QLineF.IntersectionType.UnboundedIntersection, line1.intersects(line2, intersectionPoint));
        assertEquals(10.0, intersectionPoint.x(), 0.0);
        assertEquals(30.0, intersectionPoint.y(), 0.0);

        line2 = new QLineF(11, 0, 11, 20);
        assertEquals(QLineF.IntersectionType.NoIntersection, line1.intersects(line2, null));
    }
    
    @Test
    public void testQListBuffers() {
    	io.qt.core.QList<Character> list = new io.qt.core.QList<>(char.class);
    	list.fill('X', 8);
    	assertArrayEquals(new Character[] {'X','X','X','X','X','X','X','X'}, list.toArray(new Character[list.size()]));
    	assertEquals(8, list.size());
    	CharBuffer rbuffer = QList.asCharBuffer(list);
//    	try{
//    		rbuffer.put('A');
//    		Assert.fail("ReadOnlyBufferException expected to be thrown");
//    	} catch (ReadOnlyBufferException e) {
//		}
    	assertEquals('X', rbuffer.get());
    	CharBuffer wbuffer = General.internalAccess.mutableDataC(list);
    	assertEquals(list.capacity(), wbuffer.capacity());
    	wbuffer.put('A');
    	assertArrayEquals(new Character[] {'A','X','X','X','X','X','X','X'}, list.toArray(new Character[list.size()]));
    	// on most JRE versions wbuffer is operating on shared data of list.
    	// thus, changing or deleting of list does not affect Buffer.
    	Object att = null;
    	try {
			att = General.internalAccess.readField(wbuffer, General.internalAccess.getClass(wbuffer), "att", Object.class);
		} catch (Throwable e1) {
		}
    	if(att instanceof QList) {
    		Assert.assertTrue(list.isSharedWith((QList<?>)att));
    		list.append('A');
    		assertArrayEquals(new Character[] {'A','X','X','X','X','X','X','X','A'}, list.toArray(new Character[list.size()]));
    		assertArrayEquals(new Character[] {'A','X','X','X','X','X','X','X'}, list.toArray(new Character[list.size()]));
	    	list.dispose();
			wbuffer.put('M');
			assertArrayEquals(new Character[] {'A','M','X','X','X','X','X','X'}, list.toArray(new Character[list.size()]));
    	}else{
	    	list.dispose();
	    	try {
				wbuffer.put('M');
				Assert.fail("BufferOverflowException expected to be thrown");
			} catch (BufferOverflowException e) {
			}
    	}
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestInjectedCodeQt6.class.getName());
    }
}
