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

import static io.qt.core.QtGlobal.qHash;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import org.junit.*;

import io.qt.QtUtilities;
import io.qt.autotests.generated.ContainerTest;
import io.qt.core.*;

public class TestContainerAccessHash extends ApplicationInitializer {
	
    @SuppressWarnings("unlikely-arg-type")
	@Test
    public void testMultiHash_auto() {
    	QMultiHash<Integer, Integer> hash = QMultiHash.of(1, 1, 2, 2, 3, 3, 3, 4, 3, 5, 4, 4, 4, 5, 5, 5);
    	Assert.assertEquals("Hash size", 8, hash.size());
    	{
    		QMultiHash<Integer, Integer> hash2 = hash.clone();
    		hash2.insert(1,100);
    	}
    	{
    		QMultiHash<Integer, Integer> hash2 = hash.clone();
    		int count = hash2.removeAll(3);
        	Assert.assertEquals("removeAll(3)", 3, count);
        	count = hash2.removeAll(8);
        	Assert.assertEquals("removeAll(8)", 0, count);
    	}
    	{
    		QMultiHash<Integer, Integer> hash2 = hash.clone();
    		int removed = hash2.removeAll(3, 3);
    		Assert.assertEquals("removeAll(3,3)", 1, removed);
    		removed = hash2.removeAll(8,8);
    		Assert.assertEquals("removeAll(8,8)", 0, removed);
    	}
    	{
    		QMultiHash<Integer, Integer> hash2 = hash.clone();
    		hash2.replaceOne(3, 10);
    		Assert.assertEquals("replaceOne(3, 10)", hash.size(), hash2.size());
    		Assert.assertEquals("replaceOne(3, 10)", 10, (int)hash2.value(3));
    	}
    	{
    		QMultiHash<Integer, Integer> hash2 = hash.clone();
    		hash2.replaceOne(6, 6);
    		Assert.assertEquals("replaceOne(6, 6)", hash.size()+1, hash2.size());
    	}
    	{
    		QMultiHash<Integer, Integer> hash2 = hash.clone();
        	Integer value3 = hash2.take(3);
        	Assert.assertEquals("take(3)", (Integer)5, value3);
        	value3 = hash2.take(8);
        	Assert.assertEquals("take(8)", null, value3);
    	}
    	QList<Integer> keys = hash.uniqueKeys();
    	Assert.assertEquals("keys", QSet.of(1,2,3,4,5), new QSet<>(keys));
    	QMultiHash.ConstIterator<Integer, Integer> iter = hash.constFind(3);
    	Assert.assertEquals("find(3)", (Integer)5, iter.value());
    	iter = hash.constFind(6);
    	Assert.assertFalse("find(6)", iter.isValid());
    	boolean contains = hash.contains(2);
    	Assert.assertTrue("!contains(2)", contains);
    	contains = hash.contains(2, 2);
    	Assert.assertTrue("!contains(2,2)", contains);
    	contains = hash.contains(2, 5);
    	Assert.assertFalse("contains(2,5)", contains);
    	contains = hash.containsValue(5);
    	Assert.assertTrue("!containsValue(5)", contains);
    	contains = hash.containsValue(6);
    	Assert.assertFalse("containsValue(6)", contains);
    	contains = hash.containsKey(2);
    	Assert.assertTrue("!containsKey(2)", contains);
    	contains = hash.containsKey(6);
    	Assert.assertFalse("!containsKey(6)", contains);
    	int count = hash.count(4);
    	Assert.assertEquals("count(4)", 2, count);
    	count = hash.count(4, 5);
    	Assert.assertEquals("count(4,5)", 1, count);
    	count = hash.count(4, 6);
    	Assert.assertEquals("count(4,6)", 0, count);
    }
    
	@Test
    public void testHash_auto() {
    	QHash<Integer, Integer> hash = QHash.of(1, 1, 2, 2, 3, 3, 4, 4, 5, 5);
    	Assert.assertEquals("Hash size", 5, hash.size());
    	{
    		QHash<Integer, Integer> hash2 = hash.clone();
    		int count = hash2.removeAll(3);
        	Assert.assertEquals("removeAll(3)", 1, count);
    	}
    	{
    		QHash<Integer, Integer> hash2 = hash.clone();
        	Integer value3 = hash2.take(3);
        	Assert.assertEquals("take(3)", (Integer)3, value3);
    	}
    	QList<Integer> keys = hash.keys();
    	Assert.assertEquals("keys", new QList<>(QSet.of(1,2,3,4,5)), keys);
    	QHash.ConstIterator<Integer, Integer> iter = hash.constFind(3);
    	Assert.assertEquals("find(3)", (Integer)3, iter.value());
    	iter = hash.constFind(3);
    	Assert.assertEquals("find(3)", (Integer)3, iter.value());
    	iter = hash.constFind(6);
    	Assert.assertFalse("find(3)", iter.isValid());
    	boolean contains = hash.contains(2);
    	Assert.assertTrue("!contains(2)", contains);
    	contains = hash.containsValue(5);
    	Assert.assertTrue("!containsValue(5)", contains);
    	contains = hash.containsValue(6);
    	Assert.assertFalse("containsValue(6)", contains);
    	contains = hash.containsKey(2);
    	Assert.assertTrue("!containsKey(2)", contains);
    	contains = hash.containsKey(6);
    	Assert.assertFalse("containsKey(6)", contains);
    	int count = hash.count(4);
    	Assert.assertEquals("count(4)", 1, count);
    }
    
    @SuppressWarnings("unlikely-arg-type")
	@Test
    public void testMultiHash_template() {
    	QMultiHash<Short, QByteArray> hash = QMultiHash.of((short)1, new QByteArray("1"), (short)2, new QByteArray("2"), (short)3, new QByteArray("3"), (short)3, new QByteArray("4"), (short)3, new QByteArray("5"), (short)4, new QByteArray("4"), (short)4, new QByteArray("5"), (short)5, new QByteArray("5"));
    	Assert.assertEquals("Hash size", 8, hash.size());
    	{
    		QMultiHash<Short, QByteArray> hash2 = hash.clone();
    		hash2.insert((short)1,new QByteArray("100"));
    	}
    	{
    		QMultiHash<Short, QByteArray> hash2 = hash.clone();
    		int count = hash2.removeAll(3);
        	Assert.assertEquals("removeAll(3)", 3, count);
        	count = hash2.removeAll(8);
        	Assert.assertEquals("removeAll(8)", 0, count);
    	}
    	{
    		QMultiHash<Short, QByteArray> hash2 = hash.clone();
    		int removed = hash2.removeAll((short)3, new QByteArray("3"));
    		Assert.assertEquals("removeAll(3,3)", 1, removed);
    		removed = hash2.removeAll((short)8,new QByteArray("8"));
    		Assert.assertEquals("removeAll(8,8)", 0, removed);
    	}
    	{
    		QMultiHash<Short, QByteArray> hash2 = hash.clone();
    		hash2.replaceOne((short)3, new QByteArray("10"));
    		Assert.assertEquals("replaceOne(3, 10)", hash.size(), hash2.size());
    		Assert.assertEquals("replaceOne(3, 10)", new QByteArray("10"), hash2.value((short)3));
    	}
    	{
    		QMultiHash<Short, QByteArray> hash2 = hash.clone();
    		hash2.replaceOne((short)6, new QByteArray("6"));
    		Assert.assertEquals("replaceOne(6, 6)", hash.size()+1, hash2.size());
    	}
    	{
    		QMultiHash<Short, QByteArray> hash2 = hash.clone();
        	QByteArray value3 = hash2.take((short)3);
        	Assert.assertEquals("take(3)", new QByteArray("5"), value3);
        	value3 = hash2.take((short)8);
        	Assert.assertEquals("take(8)", new QByteArray(), value3);
    	}
    	QList<Short> keys = hash.uniqueKeys();
    	Assert.assertEquals("keys", QSet.of((short)1,(short)2,(short)3,(short)4,(short)5), new QSet<>(keys));
    	QMultiHash.ConstIterator<Short, QByteArray> iter = hash.constFind((short)3);
    	Assert.assertEquals("find(3)", new QByteArray("5"), iter.value());
    	iter = hash.constFind((short)6);
    	Assert.assertFalse("find(6)", iter.isValid());
    	boolean contains = hash.contains((short)2);
    	Assert.assertTrue("!contains(2)", contains);
    	contains = hash.contains((short)2, new QByteArray("2"));
    	Assert.assertTrue("!contains(2,2)", contains);
    	contains = hash.contains((short)2, new QByteArray("5"));
    	Assert.assertFalse("contains(2,5)", contains);
    	contains = hash.containsValue(new QByteArray("5"));
    	Assert.assertTrue("!containsValue(5)", contains);
    	contains = hash.containsValue(new QByteArray("6"));
    	Assert.assertFalse("containsValue(6)", contains);
    	contains = hash.containsKey((short)2);
    	Assert.assertTrue("!containsKey(2)", contains);
    	contains = hash.containsKey((short)6);
    	Assert.assertFalse("!containsKey(6)", contains);
    	int count = hash.count((short)4);
    	Assert.assertEquals("count(4)", 2, count);
    	count = hash.count((short)4, new QByteArray("5"));
    	Assert.assertEquals("count(4,5)", 1, count);
    	count = hash.count((short)4, new QByteArray("6"));
    	Assert.assertEquals("count(4,6)", 0, count);
    }
    
	@Test
    public void testHash_template() {
    	QHash<Integer, QByteArray> hash = QHash.of(1, new QByteArray("1"), 2, new QByteArray("2"), 3, new QByteArray("3"), 4, new QByteArray("4"), 5, new QByteArray("5"));
    	Assert.assertEquals("Hash size", 5, hash.size());
    	{
    		QHash<Integer, QByteArray> hash2 = hash.clone();
    		int count = hash2.removeAll(3);
        	Assert.assertEquals("removeAll(3)", 1, count);
    	}
    	{
    		QHash<Integer, QByteArray> hash2 = hash.clone();
    		QByteArray value3 = hash2.take(3);
        	Assert.assertEquals("take(3)", new QByteArray("3"), value3);
    	}
    	QList<Integer> keys = hash.keys();
    	Assert.assertEquals("keys", new QList<>(QSet.of(1,2,3,4,5)), keys);
    	QHash.ConstIterator<Integer, QByteArray> iter = hash.constFind(3);
    	Assert.assertEquals("find(3)", new QByteArray("3"), iter.value());
    	iter = hash.constFind(6);
    	Assert.assertFalse("find(3)", iter.isValid());
    	boolean contains = hash.contains(2);
    	Assert.assertTrue("!contains(2)", contains);
    	contains = hash.containsValue(new QByteArray("5"));
    	Assert.assertTrue("!containsValue(5)", contains);
    	contains = hash.containsValue(new QByteArray("6"));
    	Assert.assertFalse("containsValue(6)", contains);
    	contains = hash.containsKey(2);
    	Assert.assertTrue("!containsKey(2)", contains);
    	contains = hash.containsKey(6);
    	Assert.assertFalse("containsKey(6)", contains);
    	int count = hash.count(4);
    	Assert.assertEquals("count(4)", 1, count);
    }
    
    @SuppressWarnings("unchecked")
	@Test
    public void testQMultiHashShortDouble() {
    	QMultiHash<Short,Double> container = QMultiHash.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3, (short)1, 4.4);
    	Assert.assertEquals(container.size(), 4);
    	Map<Short,Double> javaContainer = new TreeMap<>((s1,s2)->s1<s2?-1:1);
    	ContainerTest.copyQMultiHashShortDoubleToJavaList(container, javaContainer);
    	Assert.assertEquals(container, javaContainer);
    	Object hash = ContainerTest.getQMultiHashShortDoubleHash(container);
    	Assert.assertEquals(hash, qHash(container));
    	QByteArray array = new QByteArray();
    	QDataStream s = new QDataStream(array, QIODevice.OpenModeFlag.WriteOnly);
    	ContainerTest.writeQMultiHashShortDouble(s, container);
    	s.dispose();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.ReadOnly);
    	QMultiHash<Short,Double> list2 = new QMultiHash<>(short.class, double.class);
    	list2.readFrom(s);
    	s.dispose();
    	Assert.assertEquals(container, list2);
    	
    	array = new QByteArray();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.WriteOnly);
    	container.writeTo(s);
    	s.dispose();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.ReadOnly);
    	list2 = new QMultiHash<>(short.class, double.class);
    	ContainerTest.readQMultiHashShortDouble(s, list2);
    	s.dispose();
    	Assert.assertEquals(container, list2);
    	
    	javaContainer.clear();
    	ContainerTest.copyFromIterable(list2, javaContainer);
    	Assert.assertEquals(list2, javaContainer);
    	QHash<String,Object> variantContainer = ContainerTest.toQVariantHash(list2);
    	if(QtUtilities.qtjambiVersion().majorVersion()>5) {
    		Map<String,Object> javaStringContainer = new TreeMap<>((s1, s2)->s1.compareTo(s2)<0?-1:1);
        	for(Map.Entry<Short,Double> entry : javaContainer.entrySet()) {
        		javaStringContainer.put(""+entry.getKey(), entry.getValue());
        	}
    		Assert.assertEquals(variantContainer, javaStringContainer);
    	}else {
    		List<QPair<String,Object>> javaStringContainer = new ArrayList<>();
	    	for(Map.Entry<Short,Double> entry : javaContainer.entrySet()) {
	    		javaStringContainer.add(new QPair<>(""+entry.getKey(), entry.getValue()));
	    	}
			Assert.assertEquals(variantContainer.size(), javaStringContainer.size());
			for(QPair<String,Object> entry : variantContainer) {
				Assert.assertTrue(javaStringContainer.contains(entry));
			}
    	}
    	Assert.assertEquals(container.size(), ContainerTest.containerSize(container));
    	Assert.assertTrue(ContainerTest.associativeFind(container, (short)1));
    	
    	Assert.assertEquals(0.0, ContainerTest.associativeValue(container, (short)2));
    	QMultiHash<Short,Double> changedContainer = (QMultiHash<Short,Double>)ContainerTest.associativeSetValue(container, (short)1, 11.1);
    	Assert.assertEquals(QMultiHash.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3), changedContainer);
    	QPair<Object,Object> result = ContainerTest.associativeFindAndReplace(container, (short)2, 22.2);
    	Assert.assertEquals(Double.valueOf(2.2), result.second);
    	Assert.assertEquals(QMultiHash.of((short)1, 1.1, (short)2, 22.2, (short)3, 3.3, (short)1, 4.4), result.first);
    	changedContainer = (QMultiHash<Short,Double>)ContainerTest.associativeInsertKey(container, (short)1);
    	Assert.assertEquals(QMultiHash.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3, (short)1, 4.4), changedContainer);
    	changedContainer = (QMultiHash<Short,Double>)ContainerTest.associativeInsertKey(container, (short)128);
    	Assert.assertEquals(QMultiHash.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3, (short)1, 4.4, (short)128, 0.0), changedContainer);
    	changedContainer = (QMultiHash<Short,Double>)ContainerTest.associativeSetValue(container, (short)128, 128.128);
    	Assert.assertEquals(QMultiHash.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3, (short)1, 4.4), changedContainer);
    }
    
    @SuppressWarnings("unchecked")
	@Test
    public void testQHashShortDouble() {
    	QHash<Short,Double> container = QHash.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3);
    	Map<Object,Object> javaContainer = new HashMap<>();
    	ContainerTest.copyQHashShortDoubleToJavaList(container, javaContainer);
    	Assert.assertEquals(container, javaContainer);
    	Object hash = ContainerTest.getQHashShortDoubleHash(container);
    	Assert.assertEquals(hash, qHash(container));
    	QByteArray array = new QByteArray();
    	QDataStream s = new QDataStream(array, QIODevice.OpenModeFlag.WriteOnly);
    	ContainerTest.writeQHashShortDouble(s, container);
    	s.dispose();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.ReadOnly);
    	QHash<Short,Double> list2 = new QHash<>(short.class, double.class);
    	list2.readFrom(s);
    	s.dispose();
    	Assert.assertEquals(container, list2);
    	
    	array = new QByteArray();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.WriteOnly);
    	container.writeTo(s);
    	s.dispose();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.ReadOnly);
    	list2 = new QHash<>(short.class, double.class);
    	ContainerTest.readQHashShortDouble(s, list2);
    	s.dispose();
    	Assert.assertEquals(container, list2);
    	
    	javaContainer.clear();
    	ContainerTest.copyFromIterable(list2, javaContainer);
    	Assert.assertEquals(list2, javaContainer);
    	QHash<String,Object> variantContainer = ContainerTest.toQVariantHash(list2);
    	Map<String,Object> javaStringContainer = new HashMap<>();
    	for(Map.Entry<Object,Object> entry : javaContainer.entrySet()) {
    		javaStringContainer.put(""+entry.getKey(), entry.getValue());
    	}
    	Assert.assertEquals(variantContainer, javaStringContainer);
    	Assert.assertEquals(container.size(), ContainerTest.containerSize(container));
    	Assert.assertTrue(ContainerTest.associativeFind(container, (short)1));
    	Assert.assertEquals(2.2, ContainerTest.associativeValue(container, (short)2));
    	QHash<Short,Double> changedContainer = (QHash<Short,Double>)ContainerTest.associativeSetValue(container, (short)1, 11.1);
    	Assert.assertEquals(QHash.of((short)1, 11.1, (short)2, 2.2, (short)3, 3.3), changedContainer);
    	QPair<Object,Object> result = ContainerTest.associativeFindAndReplace(container, (short)2, 22.2);
    	Assert.assertEquals(Double.valueOf(2.2), result.second);
    	Assert.assertEquals(QHash.of((short)1, 1.1, (short)2, 22.2, (short)3, 3.3), result.first);
    	changedContainer = (QHash<Short,Double>)ContainerTest.associativeInsertKey(container, (short)1);
    	Assert.assertEquals(QHash.of((short)1, 0.0, (short)2, 2.2, (short)3, 3.3), changedContainer);
    	changedContainer = (QHash<Short,Double>)ContainerTest.associativeInsertKey(container, (short)128);
    	Assert.assertEquals(QHash.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3, (short)128, 0.0), changedContainer);
    	changedContainer = (QHash<Short,Double>)ContainerTest.associativeSetValue(container, (short)128, 128.128);
    	Assert.assertEquals(QHash.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3, (short)128, 128.128), changedContainer);
    }

    @Test
    public void testEmptyQHashToVariantContainerNotCrashing() {
		ContainerTest.toQVariantHash(new QHash<>(String.class,String.class));
		ContainerTest.toQVariantMap(new QHash<>(String.class,String.class));
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestContainerAccessHash.class.getName());
    }
}
