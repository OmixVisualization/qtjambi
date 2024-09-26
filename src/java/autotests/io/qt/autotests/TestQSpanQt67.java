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

import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.nio.LongBuffer;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
import java.util.List;
import java.util.ListIterator;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.NonNull;
import io.qt.QtObject;
import io.qt.QtPrimitiveType;
import io.qt.autotests.generated.SpanTest;
import io.qt.autotests.generated.SpanTest.AlignedStruct;
import io.qt.core.QByteArray;
import io.qt.core.QByteArrayView;
import io.qt.core.QConstSpan;
import io.qt.core.QList;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaObject.Connection;
import io.qt.core.QMetaProperty;
import io.qt.core.QMetaType;
import io.qt.core.QObject;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QSpan;
import io.qt.core.QStack;
import io.qt.core.QString;
import io.qt.core.QStringList;

public class TestQSpanQt67 extends ApplicationInitializer {
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitialize();
    }
	
	@Test
    public void testSpanOfInt() throws IllegalAccessException {
		QList<Integer> list = QList.ofInt(1,2,3,4);
		QConstSpan<Integer> span = QSpan.ofList(list);
		Assert.assertEquals(4, span.size());
		Assert.assertEquals(list, span.toList());
		for(int i=0; i<span.size(); ++i) {
			Assert.assertEquals(Integer.valueOf(i+1), span.get(i));
		}
		Assert.assertTrue(span instanceof QSpan);
		for(int i=0; i<span.size(); ++i) {
			((QSpan<Integer>)span).set(i, i+2);
		}
		for(int i=0; i<span.size(); ++i) {
			Assert.assertEquals(Integer.valueOf(i+2), span.get(i));
		}
		for(int i=0; i<list.size(); ++i) {
			Assert.assertEquals(Integer.valueOf(i+2), list.get(i));
		}
		span.dispose();
		span = QConstSpan.ofList(list);
		Assert.assertFalse(span instanceof QSpan);
		for(int i=0; i<span.size(); ++i) {
			Assert.assertEquals(Integer.valueOf(i+2), span.get(i));
		}
		for(int i=0; i<list.size(); ++i) {
			Assert.assertEquals(Integer.valueOf(i+2), list.get(i));
		}
		int[] array = {1,2,3,4};
		span = QSpan.ofInt(array);
		for(int i=0; i<span.size(); ++i) {
			Assert.assertEquals(Integer.valueOf(i+1), span.get(i));
		}
		Assert.assertTrue(span instanceof QSpan);
		for(int i=0; i<span.size(); ++i) {
			((QSpan<Integer>)span).set(i, i+2);
		}
		for(int i=0; i<span.size(); ++i) {
			Assert.assertEquals(Integer.valueOf(i+2), span.get(i));
		}
		if(span.isBuffered())
			span.dispose();
		for(int i=0; i<array.length; ++i) {
			Assert.assertEquals(i+2, array[i]);
		}
		span = QConstSpan.ofInt(array);
		Assert.assertFalse(span instanceof QSpan);
		for(int i=0; i<span.size(); ++i) {
			Assert.assertEquals(Integer.valueOf(i+2), span.get(i));
		}
		span.dispose();
		for(int i=0; i<array.length; ++i) {
			Assert.assertEquals(i+2, array[i]);
		}
		span = QSpan.ofList(list);
		for(int i=0; i<span.size(); ++i) {
			Assert.assertEquals(Integer.valueOf(i+2), span.get(i));
		}
		span = span.clone();
		for(int i=0; i<span.size(); ++i) {
			Assert.assertEquals(Integer.valueOf(i+2), span.get(i));
		}
		span = QConstSpan.ofInt(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
		Assert.assertEquals(11, span.size());
		for(int i=0; i<span.size(); ++i) {
			Assert.assertEquals(Integer.valueOf(i), span.get(i));
		}
		QConstSpan<Integer> span2 = span.clone();
		span.dispose();
		span = span2;
		for(int i=0; i<span.size(); ++i) {
			Assert.assertEquals(Integer.valueOf(i), span.get(i));
		}
		span2 = span.subspan(1);
		Assert.assertEquals(10, span2.size());
		for(int i=0; i<span2.size(); ++i) {
			Assert.assertEquals(Integer.valueOf(i+1), span2.get(i));
		}
		span2 = span.subspan(2, 5);
		Assert.assertEquals(4, span2.size());
		for(int i=0; i<span2.size(); ++i) {
			Assert.assertEquals(Integer.valueOf(i+2), span2.get(i));
		}
		array = new int[]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
		span = QSpan.ofInt(array);
		Assert.assertEquals(10, span.size());
		for(int i=0; i<span.size(); ++i) {
			Assert.assertEquals(Integer.valueOf(i+1), span.get(i));
		}
		span2 = span.subspan(1, 1);
		Assert.assertEquals(8, span2.size());
		for(int i=0; i<span2.size(); ++i) {
			Assert.assertEquals(Integer.valueOf(i+2), span2.get(i));
		}
		Assert.assertTrue(span2 instanceof QSpan);
		for(int i=0; i<span2.size(); ++i) {
			((QSpan<Integer>)span2).set(i, 0);
		}
		Assert.assertEquals(Integer.valueOf(1), span.get(0));
		Assert.assertEquals(Integer.valueOf(10), span.get(span.size()-1));
		for(int i=1; i<span.size()-1; ++i) {
			Assert.assertEquals(Integer.valueOf(0), span.get(i));
		}
		if(span.isBuffered()) {
			for(int i=0; i<array.length; ++i) {
				Assert.assertEquals(i+1, array[i]);
			}
			span.dispose();
			for(int i=0; i<array.length; ++i) {
				Assert.assertEquals(i+1, array[i]);
			}
			span2.dispose();
		}
		Assert.assertEquals(1, array[0]);
		Assert.assertEquals(10, array[array.length-1]);
		for(int i=1; i<array.length-1; ++i) {
			Assert.assertEquals(0, array[i]);
		}
		
		List<Integer> javaList = new ArrayList<>(Arrays.asList(1,2,3,4,5,6,7,8,9,10));
		ListIterator<Integer> listIterator = javaList.listIterator();
		Assert.assertEquals(Integer.valueOf(1), listIterator.next());
		Assert.assertEquals(Integer.valueOf(2), listIterator.next());
		listIterator.set(0);
		Assert.assertEquals(Integer.valueOf(0), javaList.get(1));
		
		ArrayDeque<Integer> javaStack = new ArrayDeque<>(Arrays.asList(1,2,3,4,5,6,7,8,9,10));
		Iterator<Integer> descendingIterator = javaStack.descendingIterator();
		Assert.assertEquals(Integer.valueOf(10), descendingIterator.next());
		Assert.assertEquals(Integer.valueOf(9), descendingIterator.next());
		
		list = QList.ofInt(1,2,3,4,5,6,7,8,9,10);
		listIterator = list.listIterator();
		Assert.assertEquals(Integer.valueOf(1), listIterator.next());
		Assert.assertEquals(Integer.valueOf(2), listIterator.next());
		listIterator.set(0);
		Assert.assertEquals(Integer.valueOf(0), list.get(1));
		
		QStack<Integer> stack = QStack.ofInt(1,2,3,4,5,6,7,8,9,10);
		descendingIterator = stack.descendingIterator();
		Assert.assertEquals(Integer.valueOf(10), descendingIterator.next());
		Assert.assertEquals(Integer.valueOf(9), descendingIterator.next());
		
		array = new int[] {1,2,3,4,5,6,7,8,9,10};
		span = QSpan.ofInt(array);
		Assert.assertEquals(IntBuffer.wrap(array), QSpan.asIntBuffer(span));
		Assert.assertArrayEquals(array, QSpan.asIntArray(span));
		
		descendingIterator = span.descendingIterator();
		Assert.assertEquals(Integer.valueOf(10), descendingIterator.next());
		Assert.assertEquals(Integer.valueOf(9), descendingIterator.next());
		
		Assert.assertTrue(span instanceof QSpan);
		listIterator = ((QSpan<Integer>)span).mutableIterator();
		Assert.assertEquals(Integer.valueOf(1), listIterator.next());
		Assert.assertEquals(Integer.valueOf(2), listIterator.next());
		listIterator.set(0);
		if(span.isBuffered())
			span.flush();
		Assert.assertEquals(0, array[1]);
		
		try{
    		span = QSpan.ofBuffer(IntBuffer.wrap(array).asReadOnlyBuffer());
    		Assert.assertTrue(span instanceof QSpan);
    		((QSpan<Integer>)span).mutableIterator();
    		Assert.fail("IllegalAccessException expected to be thrown");
    	}catch(IllegalAccessException e) {
    	}
	}
	
	@Test
    public void testSpanOfString() {
		QList<String> list = QList.ofTyped(String.class, "0", "1", "2", "3");
		QConstSpan<String> span = QSpan.ofList(list);
		for(int i=0; i<span.size(); ++i) {
			Assert.assertEquals(""+i, span.get(i));
		}
		Assert.assertTrue(span instanceof QSpan);
		for(int i=0; i<span.size(); ++i) {
			((QSpan<String>)span).set(i, span.get(i)+"!");
		}
		for(int i=0; i<span.size(); ++i) {
			Assert.assertEquals(i+"!", span.get(i));
		}
		for(int i=0; i<list.size(); ++i) {
			Assert.assertEquals(i+"!", list.get(i));
		}
		span = QConstSpan.ofList(list);
		Assert.assertFalse(span instanceof QSpan);
		for(int i=0; i<span.size(); ++i) {
			Assert.assertEquals(i+"!", span.get(i));
		}
		for(int i=0; i<list.size(); ++i) {
			Assert.assertEquals(i+"!", list.get(i));
		}
		span = QConstSpan.of("0", "1", "2", "3");
		Assert.assertFalse(span instanceof QSpan);
		for(int i=0; i<span.size(); ++i) {
			Assert.assertEquals(i+"", span.get(i));
		}
	}
    
    @Test
    public void testBoolean() {
    	SpanTest spanTest = new SpanTest();
    	Assert.assertEquals(0, spanTest.getBooleans().size());
    	spanTest.setBooleans(true, false, true, false);
    	Assert.assertEquals(4, spanTest.getBooleans().size());
    	Assert.assertArrayEquals(new boolean[]{true, false, true, false}, QSpan.asBooleanArray(spanTest.getBooleans()));
    	spanTest.setBooleans(QList.ofBoolean(false, true, false));
    	Assert.assertEquals(3, spanTest.getBooleans().size());
    	Assert.assertArrayEquals(new boolean[]{false, true, false}, QSpan.asBooleanArray(spanTest.getBooleans()));
    }
    
    @Test
    public void testByte() {
    	SpanTest spanTest = new SpanTest();
    	Assert.assertEquals(0, spanTest.getBytes().size());
    	spanTest.setBytes((byte)1, (byte)2, (byte)3, (byte)4);
    	Assert.assertEquals(4, spanTest.getBytes().size());
    	Assert.assertArrayEquals(new byte[]{(byte)1, (byte)2, (byte)3, (byte)4}, QSpan.asByteArray(spanTest.getBytes()));
    	spanTest.setBytes(QList.ofByte((byte)5, (byte)6, (byte)7));
    	Assert.assertEquals(3, spanTest.getBytes().size());
    	Assert.assertArrayEquals(new byte[]{(byte)5, (byte)6, (byte)7}, QSpan.asByteArray(spanTest.getBytes()));
    	QByteArray qba = new QByteArray("ABCDE");
    	QByteArray expected_qba = new QByteArray("ABCDE");
    	spanTest.setBytes(qba);
    	Assert.assertEquals(5, spanTest.getBytes().size());
    	Assert.assertArrayEquals(expected_qba.toArray(), QSpan.asByteArray(spanTest.getBytes()));
    	Assert.assertEquals(expected_qba, qba);
    	QByteArrayView qbav = new QByteArrayView("ABCDE");
    	QByteArrayView expected_qbav = new QByteArrayView("ABCDE");
    	spanTest.setBytes(qbav);
    	Assert.assertEquals(5, spanTest.getBytes().size());
    	Assert.assertArrayEquals(expected_qbav.toArray(), QSpan.asByteArray(spanTest.getBytes()));
    	Assert.assertEquals(expected_qba, qbav);
    }
    
    @Test
    public void testShort() {
    	SpanTest spanTest = new SpanTest();
    	Assert.assertEquals(0, spanTest.getShorts().size());
    	spanTest.setShorts((short)1, (short)2, (short)3, (short)4);
    	Assert.assertEquals(4, spanTest.getShorts().size());
    	Assert.assertArrayEquals(new short[]{(short)1, (short)2, (short)3, (short)4}, QSpan.asShortArray(spanTest.getShorts()));
    	spanTest.setShorts(QList.ofShort((short)5, (short)6, (short)7));
    	Assert.assertEquals(3, spanTest.getShorts().size());
    	Assert.assertArrayEquals(new short[]{(short)5, (short)6, (short)7}, QSpan.asShortArray(spanTest.getShorts()));
    }
    
    @Test
    public void testInt() {
    	SpanTest spanTest = new SpanTest();
    	Assert.assertEquals(0, spanTest.getInts().size());
    	spanTest.setInts(1, 2, 3, 4);
    	Assert.assertEquals(4, spanTest.getInts().size());
    	Assert.assertArrayEquals(new int[]{1, 2, 3, 4}, QSpan.asIntArray(spanTest.getInts()));
    	spanTest.setInts(QList.ofInt(5, 6, 7));
    	Assert.assertEquals(3, spanTest.getInts().size());
    	Assert.assertArrayEquals(new int[]{5, 6, 7}, QSpan.asIntArray(spanTest.getInts()));
    }
    
    @Test
    public void testLong() {
    	SpanTest spanTest = new SpanTest();
    	Assert.assertEquals(0, spanTest.getLongs().size());
    	spanTest.setLongs(1, 2, 3, 4);
    	Assert.assertEquals(4, spanTest.getLongs().size());
    	Assert.assertArrayEquals(new long[]{1, 2, 3, 4}, QSpan.asLongArray(spanTest.getLongs()));
    	spanTest.setLongs(QList.ofLong(5, 6, 7));
    	Assert.assertEquals(3, spanTest.getLongs().size());
    	Assert.assertArrayEquals(new long[]{5, 6, 7}, QSpan.asLongArray(spanTest.getLongs()));
    }
    
    @Test
    public void testChar() {
    	SpanTest spanTest = new SpanTest();
    	Assert.assertEquals(0, spanTest.getChars().size());
    	spanTest.setChars('1', '2', '3', '4');
    	Assert.assertEquals(4, spanTest.getChars().size());
    	Assert.assertArrayEquals(new char[]{'1', '2', '3', '4'}, QSpan.asCharArray(spanTest.getChars()));
    	spanTest.setChars(QList.ofChar('5', '6', '7'));
    	Assert.assertEquals(3, spanTest.getChars().size());
    	Assert.assertArrayEquals(new char[]{'5', '6', '7'}, QSpan.asCharArray(spanTest.getChars()));
    	QString qstring = new QString("ABCDE");
    	QString expected_qstring = new QString("ABCDE");
    	spanTest.setChars(qstring);
    	Assert.assertEquals(5, spanTest.getChars().size());
    	Assert.assertArrayEquals(expected_qstring.toArray(), QSpan.asCharArray(spanTest.getChars()));
    	Assert.assertEquals(expected_qstring, qstring);
    	String string = new String("GHIJKL");
    	String expected_string = new String("GHIJKL");
    	spanTest.setChars(string);
    	Assert.assertEquals(6, spanTest.getChars().size());
    	Assert.assertArrayEquals(expected_string.toCharArray(), QSpan.asCharArray(spanTest.getChars()));
    	Assert.assertEquals(expected_string, string);
    }
    
    @Test
    public void testFloat() {
    	SpanTest spanTest = new SpanTest();
    	Assert.assertEquals(0, spanTest.getFloats().size());
    	spanTest.setFloats(1, 2, 3, 4);
    	Assert.assertEquals(4, spanTest.getFloats().size());
    	Assert.assertArrayEquals(new float[]{1, 2, 3, 4}, QSpan.asFloatArray(spanTest.getFloats()), 0.001f);
    	spanTest.setFloats(QList.ofFloat(5, 6, 7));
    	Assert.assertEquals(3, spanTest.getFloats().size());
    	Assert.assertArrayEquals(new float[]{5, 6, 7}, QSpan.asFloatArray(spanTest.getFloats()), 0.001f);
    }
    
    @Test
    public void testDouble() {
    	SpanTest spanTest = new SpanTest();
    	Assert.assertEquals(0, spanTest.getDoubles().size());
    	spanTest.setDoubles(1, 2, 3, 4);
    	Assert.assertEquals(4, spanTest.getDoubles().size());
    	Assert.assertArrayEquals(new double[]{1, 2, 3, 4}, QSpan.asDoubleArray(spanTest.getDoubles()), 0.001);
    	spanTest.setDoubles(QList.ofDouble(5, 6, 7));
    	Assert.assertEquals(3, spanTest.getDoubles().size());
    	Assert.assertArrayEquals(new double[]{5, 6, 7}, QSpan.asDoubleArray(spanTest.getDoubles()), 0.001);
    }
    
    @Test
    public void testByteArray() {
    	SpanTest spanTest = new SpanTest();
    	Assert.assertEquals(0, spanTest.getByteArrays().size());
    	spanTest.setByteArrays(new QByteArray("A"), new QByteArray("B"), new QByteArray("C"), new QByteArray("D"));
    	Assert.assertEquals(4, spanTest.getByteArrays().size());
    	Assert.assertArrayEquals(new QByteArray[]{new QByteArray("A"), new QByteArray("B"), new QByteArray("C"), new QByteArray("D")}, spanTest.getByteArrays().toArray());
    	spanTest.setByteArrays(QList.ofTyped(QByteArray.class, new QByteArray("X"), new QByteArray("Y"), new QByteArray("Z")));
    	Assert.assertEquals(3, spanTest.getByteArrays().size());
    	Assert.assertArrayEquals(new QByteArray[]{new QByteArray("X"), new QByteArray("Y"), new QByteArray("Z")}, spanTest.getByteArrays().toArray());
    }
    
    @Test
    public void testString() {
    	SpanTest spanTest = new SpanTest();
    	Assert.assertEquals(0, spanTest.getStrings().size());
    	spanTest.setStrings("W", "X", "Y", "Z");
    	Assert.assertEquals(4, spanTest.getStrings().size());
    	Assert.assertArrayEquals(new String[]{"W", "X", "Y", "Z"}, spanTest.getStrings().toArray());
    	spanTest.setStrings(QList.of("A", "B", "C"));
    	Assert.assertEquals(3, spanTest.getStrings().size());
    	Assert.assertArrayEquals(new String[]{"A", "B", "C"}, spanTest.getStrings().toArray());
    }
    
    @Test
    public void testMutableByte() {
    	SpanTest spanTest = new SpanTest();
    	Assert.assertEquals(0, spanTest.getMutableBytes().size());
    	byte[] array = {(byte)1, (byte)2, (byte)3, (byte)4};
    	byte[] expectedArray = {(byte)2, (byte)3, (byte)4, (byte)5};
    	byte[] expectedArray2 = {(byte)6, (byte)7, (byte)8};
		QList<Byte> list = QList.ofByte((byte)5, (byte)6, (byte)7);
    	spanTest.setMutableBytes(array);
    	Assert.assertEquals(4, spanTest.getMutableBytes().size());
    	Assert.assertArrayEquals(expectedArray, QSpan.asByteArray(spanTest.getMutableBytes()));
    	Assert.assertArrayEquals(expectedArray, array);
    	spanTest.setMutableBytes(list);
    	Assert.assertEquals(3, spanTest.getMutableBytes().size());
    	Assert.assertArrayEquals(expectedArray2, QSpan.asByteArray(spanTest.getMutableBytes()));
    	Assert.assertEquals(QList.ofByte(expectedArray2), list);
    	QByteArray qba = new QByteArray("ABCDE");
    	QByteArray expected_qba = new QByteArray("BCDEF");
    	spanTest.setMutableBytes(qba);
    	Assert.assertEquals(5, spanTest.getMutableBytes().size());
    	Assert.assertEquals(expected_qba.data(), QSpan.asByteBuffer(spanTest.getMutableBytes()));
    	Assert.assertEquals(expected_qba, qba);
    	try{
    		spanTest.setMutableBytes(QSpan.ofBuffer(ByteBuffer.allocate(4).asReadOnlyBuffer()));
    		Assert.fail("IllegalArgumentException expected to be thrown");
    	}catch(IllegalArgumentException e) {
    	}
    	spanTest.setMutableBytes(new QSpan<>());
    	Assert.assertEquals(0, spanTest.getMutableBytes().size());
    }
    
    @Test
    public void testMutableShort() {
    	SpanTest spanTest = new SpanTest();
    	Assert.assertEquals(0, spanTest.getMutableShorts().size());
    	short[] array = {(short)1, (short)2, (short)3, (short)4};
    	short[] expectedArray = {(short)2, (short)3, (short)4, (short)5};
    	short[] expectedArray2 = {(short)6, (short)7, (short)8};
		QList<Short> list = QList.ofShort((short)5, (short)6, (short)7);
    	spanTest.setMutableShorts(array);
    	Assert.assertEquals(4, spanTest.getMutableShorts().size());
    	Assert.assertArrayEquals(expectedArray, QSpan.asShortArray(spanTest.getMutableShorts()));
    	Assert.assertArrayEquals(expectedArray, array);
    	spanTest.setMutableShorts(list);
    	Assert.assertEquals(3, spanTest.getMutableShorts().size());
    	Assert.assertArrayEquals(expectedArray2, QSpan.asShortArray(spanTest.getMutableShorts()));
    	Assert.assertEquals(QList.ofShort(expectedArray2), list);
    }
    
    @Test
    public void testMutableInt() {
    	SpanTest spanTest = new SpanTest();
    	Assert.assertEquals(0, spanTest.getMutableInts().size());
    	int[] array = {1, 2, 3, 4};
    	int[] expectedArray = {2, 3, 4, 5};
    	int[] expectedArray2 = {6, 7, 8};
		QList<Integer> list = QList.ofInt(5, 6, 7);
    	spanTest.setMutableInts(array);
    	Assert.assertEquals(4, spanTest.getMutableInts().size());
    	Assert.assertArrayEquals(expectedArray, QSpan.asIntArray(spanTest.getMutableInts()));
    	Assert.assertArrayEquals(expectedArray, array);
    	spanTest.setMutableInts(list);
    	Assert.assertEquals(3, spanTest.getMutableInts().size());
    	Assert.assertArrayEquals(expectedArray2, QSpan.asIntArray(spanTest.getMutableInts()));
    	Assert.assertEquals(QList.ofInt(expectedArray2), list);
    }
    
    @Test
    public void testMutableLong() {
    	SpanTest spanTest = new SpanTest();
    	Assert.assertEquals(0, spanTest.getMutableLongs().size());
    	long[] array = {1, 2, 3, 4};
    	long[] expectedArray = {2, 3, 4, 5};
    	long[] expectedArray2 = {6, 7, 8};
		QList<Long> list = QList.ofLong(5, 6, 7);
    	spanTest.setMutableLongs(array);
    	Assert.assertEquals(4, spanTest.getMutableLongs().size());
    	Assert.assertArrayEquals(expectedArray, QSpan.asLongArray(spanTest.getMutableLongs()));
    	Assert.assertArrayEquals(expectedArray, array);
    	spanTest.setMutableLongs(list);
    	Assert.assertEquals(3, spanTest.getMutableLongs().size());
    	Assert.assertArrayEquals(expectedArray2, QSpan.asLongArray(spanTest.getMutableLongs()));
    	Assert.assertEquals(QList.ofLong(expectedArray2), list);
    }
    
    @Test
    public void testMutableChar() {
    	SpanTest spanTest = new SpanTest();
    	Assert.assertEquals(0, spanTest.getMutableChars().size());
    	char[] array = {'1', '2', '3', '4'};
    	char[] expectedArray = {'2', '3', '4', '5'};
    	char[] expectedArray2 = {'6', '7', '8'};
    	spanTest.setMutableChars(array);
    	Assert.assertEquals(4, spanTest.getMutableChars().size());
    	Assert.assertArrayEquals(expectedArray, QSpan.asCharArray(spanTest.getMutableChars()));
    	Assert.assertArrayEquals(expectedArray, array);
		QList<Character> list = QList.ofChar('5', '6', '7');
    	spanTest.setMutableChars(list);
    	Assert.assertEquals(3, spanTest.getMutableChars().size());
    	Assert.assertArrayEquals(expectedArray2, QSpan.asCharArray(spanTest.getMutableChars()));
    	Assert.assertEquals(QList.ofChar(expectedArray2), list);
    	QString qstring = new QString("ABCDE");
    	QString expected_qstring = new QString("BCDEF");
    	spanTest.setMutableChars(qstring);
    	Assert.assertEquals(5, spanTest.getMutableChars().size());
    	Assert.assertEquals(expected_qstring.data(), QSpan.asCharBuffer(spanTest.getMutableChars()));
    	Assert.assertEquals(expected_qstring, qstring);
    }
    
    @Test
    public void testMutableFloat() {
    	SpanTest spanTest = new SpanTest();
    	Assert.assertEquals(0, spanTest.getMutableFloats().size());
    	float[] array = {1, 2, 3, 4};
    	float[] expectedArray = {2, 3, 4, 5};
    	float[] expectedArray2 = {6, 7, 8};
		QList<Float> list = QList.ofFloat(5, 6, 7);
    	spanTest.setMutableFloats(array);
    	Assert.assertEquals(4, spanTest.getMutableFloats().size());
    	Assert.assertArrayEquals(expectedArray, QSpan.asFloatArray(spanTest.getMutableFloats()), 0.001f);
    	Assert.assertArrayEquals(expectedArray, array, 0.001f);
    	spanTest.setMutableFloats(list);
    	Assert.assertEquals(3, spanTest.getMutableFloats().size());
    	Assert.assertArrayEquals(expectedArray2, QSpan.asFloatArray(spanTest.getMutableFloats()), 0.001f);
    	Assert.assertEquals(QList.ofFloat(expectedArray2), list);
    }
    
    @Test
    public void testMutableDouble() {
    	SpanTest spanTest = new SpanTest();
    	Assert.assertEquals(0, spanTest.getMutableDoubles().size());
    	double[] array = {1, 2, 3, 4};
    	double[] expectedArray = {2, 3, 4, 5};
    	double[] expectedArray2 = {6, 7, 8};
		QList<Double> list = QList.ofDouble(5, 6, 7);
    	spanTest.setMutableDoubles(array);
    	Assert.assertEquals(4, spanTest.getMutableDoubles().size());
    	Assert.assertArrayEquals(expectedArray, QSpan.asDoubleArray(spanTest.getMutableDoubles()), 0.001);
    	Assert.assertArrayEquals(expectedArray, array, 0.001);
    	spanTest.setMutableDoubles(list);
    	Assert.assertEquals(3, spanTest.getMutableDoubles().size());
    	Assert.assertArrayEquals(expectedArray2, QSpan.asDoubleArray(spanTest.getMutableDoubles()), 0.001);
    	Assert.assertEquals(QList.ofDouble(expectedArray2), list);
    	spanTest.setMutableDoubles(new QSpan<>());
    	Assert.assertEquals(0, spanTest.getMutableDoubles().size());
    }
    
    @Test
    public void testMutableString() {
    	SpanTest spanTest = new SpanTest();
    	Assert.assertEquals(0, spanTest.getMutableStrings().size());
    	String[] array = {"1", "2", "3", "4"};
    	String[] expectedArray = {"1!", "2!", "3!", "4!"};
    	String[] expectedArray2 = {"1!", "2!", "3!"};
		QList<String> list = QList.of("1", "2", "3");
    	spanTest.setMutableStrings(array);
    	Assert.assertEquals(4, spanTest.getMutableStrings().size());
    	Assert.assertArrayEquals(expectedArray, spanTest.getMutableStrings().toArray());
    	Assert.assertArrayEquals(expectedArray, array);
    	spanTest.setMutableStrings(list);
    	Assert.assertEquals(3, spanTest.getMutableStrings().size());
    	Assert.assertArrayEquals(expectedArray2, spanTest.getMutableStrings().toArray());
    	Assert.assertEquals(new QStringList(expectedArray2), list);
    }
    
    @Test
    public void testMutableByteArray() {
    	SpanTest spanTest = new SpanTest();
    	Assert.assertEquals(0, spanTest.getMutableByteArrays().size());
    	QByteArray[] array = {new QByteArray("1"), new QByteArray("2"), new QByteArray("3"), new QByteArray("4")};
    	QByteArray[] expectedArray = {new QByteArray("1!"), new QByteArray("2!"), new QByteArray("3!"), new QByteArray("4!")};
    	QByteArray[] expectedArray2 = {new QByteArray("1!"), new QByteArray("2!"), new QByteArray("3!")};
		QList<QByteArray> list = QList.of(new QByteArray("1"), new QByteArray("2"), new QByteArray("3"));
    	spanTest.setMutableByteArrays(array);
    	Assert.assertEquals(4, spanTest.getMutableByteArrays().size());
    	Assert.assertArrayEquals(expectedArray, spanTest.getMutableByteArrays().toArray());
    	Assert.assertArrayEquals(expectedArray, array);
    	spanTest.setMutableByteArrays(list);
    	Assert.assertEquals(3, spanTest.getMutableByteArrays().size());
    	Assert.assertArrayEquals(expectedArray2, spanTest.getMutableByteArrays().toArray());
    	Assert.assertEquals(QList.ofTyped(QByteArray.class, expectedArray2), list);
    	spanTest.setMutableByteArrays(new QSpan<>());
    	Assert.assertEquals(0, spanTest.getMutableByteArrays().size());
    }
    
    private static class SpanObject extends QObject{
    	public final Signal1<QConstSpan<@QtPrimitiveType Integer>> spanChanged = new Signal1<>();
    	
    	final QList<@QtPrimitiveType Integer> list = new QList<>(int.class);

		@SuppressWarnings("unused")
		public QConstSpan<@QtPrimitiveType Integer> getSpan() {
			return QConstSpan.ofList(list);
		}

		public void setSpan(QConstSpan<@QtPrimitiveType Integer> span) {
			list.assign(span.toList());
			spanChanged.emit(QConstSpan.ofList(list));
		}
    }
    
    private static class SpanReceiver extends QObject{
    	public final QList<@QtPrimitiveType Integer> list = new QList<>(int.class);
		public void receiveSpan(QConstSpan<@QtPrimitiveType Integer> span) {
			list.clear();
			list.addAll(QList.ofBuffer(QSpan.asIntBuffer(span)));
		}
    }
    
    @Test
    public void testSpanSignal() {
    	QMetaObject spanMO = QMetaObject.forType(SpanObject.class);
    	QMetaProperty spanProperty = spanMO.property("span");
    	Assert.assertTrue(spanProperty.isValid());
    	Assert.assertTrue(spanProperty.metaType().isValid());
		Assert.assertEquals(QMetaType.fromName("QSpan<const int>"), spanProperty.metaType());
    	SpanObject object = new SpanObject();
    	SpanReceiver receiver = new SpanReceiver();
		Connection connection = object.spanChanged.connect(receiver::receiveSpan);
		Assert.assertTrue(connection.isConnected());
    	Assert.assertTrue(object.list.isEmpty());
    	Assert.assertTrue(receiver.list.isEmpty());
    	object.setSpan(QConstSpan.ofInt(1,2,3,4));
    	Assert.assertEquals(Arrays.asList(1,2,3,4), receiver.list);
    	Assert.assertEquals(QConstSpan.class, spanProperty.metaType().javaType());
    	Assert.assertTrue(spanProperty.metaType().create(null) instanceof QConstSpan);
    	Assert.assertTrue("unable to write to QSpan", spanProperty.write(object, QConstSpan.ofInt(1,2,3,4,5)));
    	Assert.assertEquals(Arrays.asList(1,2,3,4,5), receiver.list);
    }
    
    @Test
    public void testVirtualMutableStringSpan() {
    	SpanTest spanTest = new SpanTest();
    	String[] array = {"1", "2", "3", "4"};
    	String[] expectedArray = {"1!", "2!", "3!", "4!"};
    	String[] expectedArray2 = {"1!", "2!", "3!"};
		QList<String> list = QList.of("1", "2", "3");
    	spanTest.callVirtualMutableSpan(array);
    	Assert.assertArrayEquals(expectedArray, array);
    	spanTest.callVirtualMutableSpan(list);
    	Assert.assertEquals(new QStringList(expectedArray2), list);
    }
    
    @Test
    public void testVirtualMutableStringSpanOverride() {
    	SpanTest spanTest = new SpanTest(){
			@Override
			public void virtualMutableSpan(@NonNull QSpan<@NonNull String> mutableStrings) {
				for (int i = 0; i < mutableStrings.size(); i++) {
					mutableStrings.set(i, mutableStrings.get(i)+"?");
				}
			}
    	};
    	String[] array = {"1", "2", "3", "4"};
    	String[] expectedArray = {"1?", "2?", "3?", "4?"};
    	String[] expectedArray2 = {"1?", "2?", "3?"};
		QList<String> list = QList.of("1", "2", "3");
    	spanTest.callVirtualMutableSpan(array);
    	Assert.assertArrayEquals(expectedArray, array);
    	spanTest.callVirtualMutableSpan(list);
    	Assert.assertEquals(new QStringList(expectedArray2), list);
    }
    
    @Test
    public void testVirtualByteArraySpan() {
    	SpanTest spanTest = new SpanTest();
    	QByteArray[] array = {new QByteArray("1"), new QByteArray("2"), new QByteArray("3"), new QByteArray("4")};
    	QByteArray[] expectedArray = {new QByteArray("1"), new QByteArray("2"), new QByteArray("3"), new QByteArray("4")};
    	QByteArray[] expectedArray2 = {new QByteArray("1"), new QByteArray("2"), new QByteArray("3")};
		QList<QByteArray> list = QList.of(new QByteArray("1"), new QByteArray("2"), new QByteArray("3"));
		Assert.assertEquals(array.length, spanTest.callVirtualSpan(array));
    	Assert.assertArrayEquals(expectedArray, array);
    	Assert.assertEquals(list.size(), spanTest.callVirtualSpan(list));
    	Assert.assertArrayEquals(expectedArray2, list.toArray());
    }
    
    @Test
    public void testVirtualByteArraySpanOverride() {
    	QtObject[] argument = {null};
    	SpanTest spanTest = new SpanTest(){
			@Override
			public long virtualSpan(@NonNull QConstSpan<@NonNull QByteArray> byteArrays) {
				QByteArray concat = new QByteArray();
				for (QByteArray b : byteArrays) {
					concat = concat.append(b);
				}
				argument[0] = byteArrays;
				return concat.toInt();
			}
    	};
    	QByteArray[] array = {new QByteArray("1"), new QByteArray("2"), new QByteArray("3"), new QByteArray("4")};
    	QByteArray[] expectedArray = {new QByteArray("1"), new QByteArray("2"), new QByteArray("3"), new QByteArray("4")};
    	QByteArray[] expectedArray2 = {new QByteArray("1"), new QByteArray("2"), new QByteArray("3")};
		QList<QByteArray> list = QList.of(new QByteArray("1"), new QByteArray("2"), new QByteArray("3"));
    	Assert.assertTrue(argument[0]==null);
		Assert.assertEquals(1234, spanTest.callVirtualSpan(array));
    	Assert.assertTrue(argument[0]!=null);
    	Assert.assertTrue(argument[0].isDisposed());
    	Assert.assertArrayEquals(expectedArray, array);
    	Assert.assertEquals(123, spanTest.callVirtualSpan(list));
    	Assert.assertArrayEquals(expectedArray2, list.toArray());
    }
    
    @Test
    public void testVirtualLongSpan() {
    	SpanTest spanTest = new SpanTest();
    	long[] array = {1, 2, 3, 4};
    	long[] expectedArray = {1, 2, 3, 4};
    	Long[] expectedArray2 = {1l,2l,3l};
		QList<Long> list = QList.ofLong(1,2,3);
		Assert.assertEquals(12, spanTest.callVirtualSpan(array, 2));
    	Assert.assertArrayEquals(expectedArray, array);
    	Assert.assertEquals(16, spanTest.callVirtualSpan(list, 10));
    	Assert.assertArrayEquals(expectedArray2, list.toArray(new Long[list.size()]));
    }
    
    @Test
    public void testVirtualMutableIntSpan() {
    	SpanTest spanTest = new SpanTest();
    	int[] array = {1, 2, 3, 4};
    	int[] expectedArray = {2, 3, 4, 5};
    	Integer[] expectedArray2 = {2, 3, 4};
		QList<Integer> list = QList.ofInt(1, 2, 3);
		Assert.assertEquals(15, spanTest.callVirtualMutableSpan(array, 5));
    	Assert.assertArrayEquals(expectedArray, array);
    	Assert.assertEquals(21, spanTest.callVirtualMutableSpan(list, 15));
    	Assert.assertArrayEquals(expectedArray2, list.toArray());
    }
    
    @Test
    public void testVirtualMutableIntSpanOverride() {
    	QtObject[] argument = {null};
    	SpanTest spanTest = new SpanTest() {
			@Override
			public int virtualMutableSpan(@NonNull QSpan<@QtPrimitiveType @NonNull Integer> mutableInts, int result) {
				for (int i = 0; i < mutableInts.size(); i++) {
					mutableInts.set(i, result += mutableInts.get(i));
				}
				argument[0] = mutableInts;
				return result;
			}
    	};
    	int[] array = {1, 2, 3, 4};
    	int[] expectedArray = {6, 8, 11, 15};
    	Integer[] expectedArray2 = {2, 4, 7};
		QList<Integer> list = QList.ofInt(1, 2, 3);
    	Assert.assertTrue(argument[0]==null);
		Assert.assertEquals(15, spanTest.callVirtualMutableSpan(array, 5));
    	Assert.assertTrue(argument[0]!=null);
    	Assert.assertTrue(argument[0].isDisposed());
    	Assert.assertArrayEquals(expectedArray, array);
    	Assert.assertEquals(7, spanTest.callVirtualMutableSpan(list, 1));
    	Assert.assertArrayEquals(expectedArray2, list.toArray());
    }
    
    @Test
    public void testVirtualMutableReturning() {
    	SpanTest spanTest = new SpanTest();
    	spanTest.setLongs(new long [] {1,2,3,4,5});
		QSpan<Long> span = spanTest.callVirtualMutableSpanReturning();
		Assert.assertEquals(5, span.size());
		for (int i = 0; i < span.size(); i++) {
			Assert.assertEquals(i+1, (long)span.at(i));
		}
		span.set(0, 20l);
		Assert.assertEquals(20l, (long)spanTest.getLongs().at(0));
		spanTest.getMutableLongs().set(0, 30l);
		Assert.assertEquals(30l, (long)span.at(0));
		spanTest.setLongs(new long [0]);
		spanTest.dispose();
		Assert.assertTrue(span.isDisposed());
    }
    
    @Test
    public void testVirtualMutableReturningOverride() {
		QList<Long> list = QList.ofLong(20,30,40);
    	SpanTest spanTest = new SpanTest(){
			@Override
			public @NonNull QSpan<@QtPrimitiveType @NonNull Long> virtualMutableSpanReturning() {
				return QSpan.ofList(list);
			}
    	};
		QSpan<Long> span = spanTest.callVirtualMutableSpanReturning();
		Assert.assertEquals(3, span.size());
		for (int i = 0; i < span.size(); i++) {
			Assert.assertEquals((i+2)*10, (long)span.at(i));
		}
		span.set(0, 2l);
		Assert.assertEquals(2l, (long)list.at(0));
		list.set(0, 3l);// by this, the shared list is detached from he span
		Assert.assertEquals(2l, (long)span.at(0));
		spanTest.dispose();
		Assert.assertTrue(span.isDisposed());
    }
    
    @Test
    public void testVirtualReturning() {
    	SpanTest spanTest = new SpanTest();
    	spanTest.setLongs(new long [] {1,2,3,4,5});
		QConstSpan<Long> span = spanTest.callVirtualSpanReturning();
		Assert.assertEquals(5, span.size());
		for (int i = 0; i < span.size(); i++) {
			Assert.assertEquals(i+1, (long)span.at(i));
		}
		Assert.assertFalse(span instanceof QSpan);
		Assert.assertEquals(1, (long)spanTest.getLongs().at(0));
		spanTest.getMutableLongs().set(0, 30l);
		Assert.assertEquals(30l, (long)spanTest.getLongs().at(0));
		Assert.assertEquals(1, (long)span.at(0));
		spanTest.setLongs(new long [0]);
		Assert.assertEquals(1, (long)span.at(0));
		spanTest.dispose();
		Assert.assertFalse(span.isDisposed());
		Assert.assertEquals(1, (long)span.at(0));
    }
    
    @Test
    public void testVirtualReturningOverride() {
		QList<Long> list = QList.ofLong(20,30,40,50,60);
    	SpanTest spanTest = new SpanTest() {

			@Override
			public @NonNull QConstSpan<@QtPrimitiveType @NonNull Long> virtualSpanReturning() {
				return QConstSpan.ofList(list);
			}
    	};
		QConstSpan<Long> span = spanTest.callVirtualSpanReturning();
		Assert.assertEquals(5, span.size());
		for (int i = 0; i < span.size(); i++) {
			Assert.assertEquals((i+2)*10, (long)span.at(i));
		}
		Assert.assertFalse(span instanceof QSpan);
		Assert.assertEquals(20, (long)list.at(0));
		list.set(0, 99l);
		Assert.assertEquals(99l, (long)list.at(0));
		Assert.assertEquals(20, (long)span.at(0));
		list.dispose();
		Assert.assertEquals(20, (long)span.at(0));
		spanTest.dispose();
		Assert.assertFalse(span.isDisposed());
		Assert.assertEquals(20, (long)span.at(0));
    }
    
    @Test
    public void testAlign() {
    	QList<AlignedStruct> list = QList.ofTyped(AlignedStruct.class, new AlignedStruct(1, 2, (byte)3), new AlignedStruct(4, 5, (byte)6), new AlignedStruct(7, 8, (byte)9));
    	QSpan<AlignedStruct> span = QSpan.ofList(list);
    	QList<AlignedStruct> list2 = span.toList();
    	Assert.assertEquals(list, list2);
    	AlignedStruct[] array = span.toArray();
    	Assert.assertArrayEquals(list.toArray(), array);
    }
    
    @Test
    public void testBuffer() {
    	QList<Long> list = QList.ofLong(20,30,40,50,60);
		LongBuffer longBuffer = QList.asLongBuffer(list);
		Assert.assertFalse(longBuffer.isReadOnly());
		Assert.assertEquals(LongBuffer.wrap(new long[]{20,30,40,50,60}), longBuffer);
    	list.dispose();
    	if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)) {
    		Assert.assertEquals("buffer limit", 0, longBuffer.limit());
    	}else {
	    	// shared data is not deleted:
	    	Assert.assertEquals("buffer limit", 5, longBuffer.limit());
	    	Assert.assertEquals("buffer[0]", 20, longBuffer.get());
	    	Assert.assertEquals("buffer[1]", 30, longBuffer.get());
	    	Assert.assertEquals("buffer[2]", 40, longBuffer.get());
	    	Assert.assertEquals("buffer[3]", 50, longBuffer.get());
    	}
    	list = QList.ofLong(20,30,40,50,60);
    	QConstSpan<Long> span = QConstSpan.ofList(list);
    	list.dispose();
    	// shared data is not deleted:
    	Assert.assertTrue(span.isConst());
    	Assert.assertEquals("span size", 5, span.size());
    	Assert.assertEquals("span[0]", 20, (long)span.get(0));
    	Assert.assertEquals("span[1]", 30, (long)span.get(1));
    	Assert.assertEquals("span[2]", 40, (long)span.get(2));
    	Assert.assertEquals("span[3]", 50, (long)span.get(3));
    	longBuffer = QSpan.asLongBuffer(span);
    	span.dispose();
    	// shared data is not deleted:
    	Assert.assertTrue(longBuffer.isReadOnly());
    	Assert.assertEquals(LongBuffer.wrap(new long[]{20,30,40,50,60}), longBuffer);
    	Assert.assertEquals("buffer limit", 5, longBuffer.limit());
    	Assert.assertEquals("buffer[0]", 20, longBuffer.get());
    	Assert.assertEquals("buffer[1]", 30, longBuffer.get());
    	Assert.assertEquals("buffer[2]", 40, longBuffer.get());
    	Assert.assertEquals("buffer[3]", 50, longBuffer.get());
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQSpanQt67.class.getName());
    }
}
