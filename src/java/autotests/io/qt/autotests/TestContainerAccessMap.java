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

import java.util.*;
import org.junit.*;

import io.qt.QtUtilities;
import io.qt.autotests.generated.ContainerTest;
import io.qt.core.*;

public class TestContainerAccessMap extends ApplicationInitializer {
	
    @SuppressWarnings("unlikely-arg-type")
	@Test
    public void testMultiMap_auto() {
    	QMultiMap<Integer, Integer> map = QMultiMap.of(1, 1, 2, 2, 3, 3, 3, 4, 3, 5, 4, 4, 4, 5, 5, 5);
    	Assert.assertEquals("map size", 8, map.size());
    	{
    		QMultiMap<Integer, Integer> map2 = map.clone();
    		int count = map2.removeAll(3);
        	Assert.assertEquals("removeAll(3)", 3, count);
    	}
    	{
    		QMultiMap<Integer, Integer> map2 = map.clone();
    		int removed = map2.removeAll(3, 3);
    		Assert.assertEquals("removeAll(3,3)", 1, removed);
    	}
    	{
    		QMultiMap<Integer, Integer> map2 = map.clone();
    		map2.replaceOne(3, 10);
    		Assert.assertEquals("replaceOne(3, 10)", map.size(), map2.size());
    		Assert.assertEquals("replaceOne(3, 10)", 10, (int)map2.value(3));
    	}
    	{
    		QMultiMap<Integer, Integer> map2 = map.clone();
    		map2.replaceOne(6, 6);
    		Assert.assertEquals("replaceOne(3, 10)", map.size()+1, map2.size());
    	}
    	{
    		QMultiMap<Integer, Integer> map2 = map.clone();
        	Integer value3 = map2.take(3);
        	Assert.assertEquals("take(3)", (Integer)5, value3);
    	}
    	QList<Integer> keys = map.keys();
    	Assert.assertEquals("keys", Arrays.asList(1,2,3,3,3,4,4,5), keys);
    	QList<Integer> uniqueKeys = map.uniqueKeys();
    	Assert.assertEquals("uniqueKeys", Arrays.asList(1,2,3,4,5), uniqueKeys);
    	QMultiMap.ConstIterator<Integer, Integer> iter = map.constFind(3);
    	Assert.assertEquals("constFind(3)", (Integer)5, iter.value());
    	iter = map.constLowerBound(3);
    	Assert.assertEquals("constLowerBound(3)", (Integer)5, iter.value());
    	iter = map.constUpperBound(3);
    	Assert.assertEquals("constUpperBound(3)", (Integer)5, iter.value());
    	iter = map.constFind(6);
    	Assert.assertFalse("constFind(6)", iter.isValid());
    	boolean contains = map.contains(2);
    	Assert.assertTrue("!contains(2)", contains);
    	contains = map.contains(2, 2);
    	Assert.assertTrue("!contains(2,2)", contains);
    	contains = map.contains(2, 5);
    	Assert.assertFalse("contains(2,5)", contains);
    	contains = map.containsValue(5);
    	Assert.assertTrue("!containsValue(5)", contains);
    	contains = map.containsValue(6);
    	Assert.assertFalse("containsValue(6)", contains);
    	contains = map.containsKey(2);
    	Assert.assertTrue("!containsKey(2)", contains);
    	contains = map.containsKey(6);
    	Assert.assertFalse("!containsKey(6)", contains);
    	int count = map.count(4);
    	Assert.assertEquals("count(4)", 2, count);
    	count = map.count(4, 5);
    	Assert.assertEquals("count(4,5)", 1, count);
    	count = map.count(4, 6);
    	Assert.assertEquals("count(4,6)", 0, count);
    }
    
	@Test
    public void testMap_auto() {
    	QMap<Integer, Long> map = QMap.of(1, 1l, 2, 2l, 3, 3l, 4, 4l, 5, 5l);
    	Assert.assertEquals("map size", 5, map.size());
    	{
    		QMap<Integer, Long> map2 = map.clone();
    		int count = map2.removeAll(3);
        	Assert.assertEquals("removeAll(3)", 1, count);
    	}
    	{
    		QMap<Integer, Long> map2 = map.clone();
    		Long value3 = map2.take(3);
        	Assert.assertEquals("take(3)", (Long)3l, value3);
    	}
    	QList<Integer> keys = map.keys();
    	Assert.assertEquals("keys", Arrays.asList(1,2,3,4,5), keys);
    	QMap.ConstIterator<Integer, Long> iter = map.constFind(3);
    	Assert.assertEquals("constFind(3)", (Long)3l, iter.value());
    	iter = map.constLowerBound(3);
    	Assert.assertEquals("constLowerBound(3)", (Long)3l, iter.value());
    	iter = map.constUpperBound(3);
    	Assert.assertEquals("constUpperBound(3)", (Long)4l, iter.value());
    	iter = map.constFind(6);
    	Assert.assertFalse("constFind(6)", iter.isValid());
    	boolean contains = map.contains(2);
    	Assert.assertTrue("!contains(2)", contains);
    	contains = map.containsValue(5l);
    	Assert.assertTrue("!containsValue(5)", contains);
    	contains = map.containsValue(6l);
    	Assert.assertFalse("containsValue(6)", contains);
    	contains = map.containsKey(2);
    	Assert.assertTrue("!containsKey(2)", contains);
    	contains = map.containsKey(6);
    	Assert.assertFalse("containsKey(6)", contains);
    	int count = map.count(4);
    	Assert.assertEquals("count(4)", 1, count);
    }
	
	@SuppressWarnings("unlikely-arg-type")
	@Test
    public void testMultiMap_template() {
    	QMultiMap<Integer, String> map = QMultiMap.of(1, "1", 2, "2", 3, "3", 3, "4", 3, "5", 4, "4", 4, "5", 5, "5");
    	Assert.assertEquals("map size", 8, map.size());
    	{
    		QMultiMap<Integer, String> map2 = map.clone();
    		int count = map2.removeAll(3);
        	Assert.assertEquals("removeAll(3)", 3, count);
    	}
    	{
    		QMultiMap<Integer, String> map2 = map.clone();
    		int removed = map2.removeAll(3, "3");
    		Assert.assertEquals("removeAll(3,3)", 1, removed);
    	}
    	{
    		QMultiMap<Integer, String> map2 = map.clone();
    		map2.replaceOne(1, "10");
    		Assert.assertEquals("replaceOne(1, 10)", map.size(), map2.size());
    		Assert.assertEquals("replaceOne(1, 10)", "10", map2.value(1));
    	}
    	{
    		QMultiMap<Integer, String> map2 = map.clone();
    		map2.replaceOne(6, "6");
    		Assert.assertEquals("replaceOne(6, 6)", map.size()+1, map2.size());
    	}
    	{
    		QMultiMap<Integer, String> map2 = map.clone();
    		String value3 = map2.take(3);
        	Assert.assertEquals("take(3)", "5", value3);
    	}
    	QList<Integer> keys = map.keys();
    	Assert.assertEquals("keys", Arrays.asList(1,2,3,3,3,4,4,5), keys);
    	QList<Integer> uniqueKeys = map.uniqueKeys();
    	Assert.assertEquals("uniqueKeys", Arrays.asList(1,2,3,4,5), uniqueKeys);
    	QMultiMap.ConstIterator<Integer, String> iter = map.constFind(3);
    	Assert.assertEquals("constFind(3)", "5", iter.value());
    	iter = map.constLowerBound(3);
    	Assert.assertEquals("constLowerBound(3)", "5", iter.value());
    	iter = map.constUpperBound(3);
    	Assert.assertEquals("constUpperBound(3)", "5", iter.value());
    	iter = map.constFind(6);
    	Assert.assertFalse("constFind(6)", iter.isValid());
    	boolean contains = map.contains(2);
    	Assert.assertTrue("!contains(2)", contains);
    	contains = map.contains(2, "2");
    	Assert.assertTrue("!contains(2,2)", contains);
    	contains = map.contains(2, "5");
    	Assert.assertFalse("contains(2,5)", contains);
    	contains = map.containsValue("5");
    	Assert.assertTrue("!containsValue(5)", contains);
    	contains = map.containsValue("6");
    	Assert.assertFalse("containsValue(6)", contains);
    	contains = map.containsKey(2);
    	Assert.assertTrue("!containsKey(2)", contains);
    	contains = map.containsKey(6);
    	Assert.assertFalse("!containsKey(6)", contains);
    	int count = map.count(4);
    	Assert.assertEquals("count(4)", 2, count);
    	count = map.count(4, "5");
    	Assert.assertEquals("count(4,5)", 1, count);
    	count = map.count(4, "6");
    	Assert.assertEquals("count(4,6)", 0, count);
    }
    
	@Test
    public void testMap_template() {
    	QMap<Integer, Integer> map = QMap.of(1, 1, 2, 2, 3, 3, 4, 4, 5, 5);
    	Assert.assertEquals("map size", 5, map.size());
    	{
    		QMap<Integer, Integer> map2 = map.clone();
    		int count = map2.removeAll(3);
        	Assert.assertEquals("removeAll(3)", 1, count);
    	}
    	{
    		QMap<Integer, Integer> map2 = map.clone();
        	Integer value3 = map2.take(3);
        	Assert.assertEquals("take(3)", (Integer)3, value3);
    	}
    	QList<Integer> keys = map.keys();
    	Assert.assertEquals("keys", Arrays.asList(1,2,3,4,5), keys);
    	QMap.ConstIterator<Integer, Integer> iter = map.constFind(3);
    	Assert.assertEquals("constFind(3)", (Integer)3, iter.value());
    	iter = map.constLowerBound(3);
    	Assert.assertEquals("constLowerBound(3)", (Integer)3, iter.value());
    	iter = map.constUpperBound(3);
    	Assert.assertEquals("constUpperBound(3)", (Integer)4, iter.value());
    	iter = map.constFind(6);
    	Assert.assertFalse("constFind(6)", iter.isValid());
    	boolean contains = map.contains(2);
    	Assert.assertTrue("!contains(2)", contains);
    	contains = map.containsValue(5);
    	Assert.assertTrue("!containsValue(5)", contains);
    	contains = map.containsValue(6);
    	Assert.assertFalse("containsValue(6)", contains);
    	contains = map.containsKey(2);
    	Assert.assertTrue("!containsKey(2)", contains);
    	contains = map.containsKey(6);
    	Assert.assertFalse("containsKey(6)", contains);
    	int count = map.count(4);
    	Assert.assertEquals("count(4)", 1, count);
    }
    
    @SuppressWarnings("unchecked")
	@Test
    public void testQMapShortDouble() {
    	QMap<Short,Double> container = QMap.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3);
    	Map<Short,Double> javaContainer = new HashMap<>();
    	ContainerTest.copyQMapShortDoubleToJavaList(container, javaContainer);
    	Assert.assertEquals(container, javaContainer);
    	QByteArray array = new QByteArray();
    	QDataStream s = new QDataStream(array, QIODevice.OpenModeFlag.WriteOnly);
    	ContainerTest.writeQMapShortDouble(s, container);
    	s.dispose();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.ReadOnly);
    	QMap<Short,Double> list2 = new QMap<>(short.class, double.class);
    	list2.readFrom(s);
    	s.dispose();
    	Assert.assertEquals(container, list2);
    	
    	array = new QByteArray();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.WriteOnly);
    	container.writeTo(s);
    	s.dispose();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.ReadOnly);
    	list2 = new QMap<>(short.class, double.class);
    	ContainerTest.readQMapShortDouble(s, list2);
    	s.dispose();
    	Assert.assertEquals(container, list2);
    	
    	javaContainer.clear();
    	ContainerTest.copyFromIterable(list2, javaContainer);
    	Assert.assertEquals(list2, javaContainer);
    	QMap<String,Object> variantContainer = ContainerTest.toQVariantMap(list2);
    	Map<String,Object> javaStringContainer = new HashMap<>();
    	for(Map.Entry<Short,Double> entry : javaContainer.entrySet()) {
    		javaStringContainer.put(""+entry.getKey(), entry.getValue());
    	}
    	Assert.assertEquals(variantContainer, javaStringContainer);
    	Assert.assertEquals(container.size(), ContainerTest.containerSize(container));
    	
    	Assert.assertEquals(2.2, ContainerTest.associativeValue(container, (short)2));
    	QMap<Short,Double> changedContainer = (QMap<Short,Double>)ContainerTest.associativeSetValue(container, (short)1, 11.1);
    	Assert.assertEquals(QMap.of((short)1, 11.1, (short)2, 2.2, (short)3, 3.3), changedContainer);
    	QPair<Object,Object> result = ContainerTest.associativeFindAndReplace(container, (short)2, 22.2);
    	Assert.assertEquals(Double.valueOf(2.2), result.second);
    	Assert.assertEquals(QMap.of((short)1, 1.1, (short)2, 22.2, (short)3, 3.3), result.first);
    	changedContainer = (QMap<Short,Double>)ContainerTest.associativeInsertKey(container, (short)1);
    	Assert.assertEquals(QMap.of((short)1, 0.0, (short)2, 2.2, (short)3, 3.3), changedContainer);
    	changedContainer = (QMap<Short,Double>)ContainerTest.associativeInsertKey(container, (short)128);
    	Assert.assertEquals(QMap.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3, (short)128, 0.0), changedContainer);
    	changedContainer = (QMap<Short,Double>)ContainerTest.associativeSetValue(container, (short)128, 128.128);
    	Assert.assertEquals(QMap.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3, (short)128, 128.128), changedContainer);
    }
    
    @SuppressWarnings("unchecked")
	@Test
    public void testQMultiMapShortDouble() {
    	QMultiMap<Short,Double> container = QMultiMap.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3, (short)1, 4.4);
    	Map<Short,Double> javaContainer = new TreeMap<>((s1,s2)->s1<s2?-1:1);
    	ContainerTest.copyQMultiMapShortDoubleToJavaList(container, javaContainer);
    	if(QtUtilities.qtjambiVersion().majorVersion()>5) {
    		Assert.assertEquals(container, javaContainer);
    	}else {
    		Assert.assertEquals(container.size(), javaContainer.size());
    		for(Map.Entry<Short,Double> entry : javaContainer.entrySet()) {
    			Assert.assertTrue(container.contains(entry.getKey(), entry.getValue()));
    		}
    	}
//    	long hash = ContainerTest.getQMultiMapShortDoubleHash(container);
//    	Assert.assertEquals(hash, qHash(container));
    	QByteArray array = new QByteArray();
    	QDataStream s = new QDataStream(array, QIODevice.OpenModeFlag.WriteOnly);
    	ContainerTest.writeQMultiMapShortDouble(s, container);
    	s.dispose();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.ReadOnly);
    	QMultiMap<Short,Double> list2 = new QMultiMap<>(short.class, double.class);
    	list2.readFrom(s);
    	s.dispose();
    	Assert.assertEquals(container, list2);
    	
    	array = new QByteArray();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.WriteOnly);
    	container.writeTo(s);
    	s.dispose();
    	s = new QDataStream(array, QIODevice.OpenModeFlag.ReadOnly);
    	list2 = new QMultiMap<>(short.class, double.class);
    	ContainerTest.readQMultiMapShortDouble(s, list2);
    	s.dispose();
    	if(QtUtilities.qtjambiVersion().majorVersion()>5) {
    		Assert.assertEquals(container, list2);
    	}else{
    		Assert.assertEquals(container.size(), list2.size());
    		for(QPair<Short,Double> entry : list2) {
    			Assert.assertTrue(container.contains(entry.first, entry.second));
    		}
    	}
    	
    	javaContainer.clear();
    	ContainerTest.copyFromIterable(list2, javaContainer);
    	if(QtUtilities.qtjambiVersion().majorVersion()>5) {
    		Assert.assertEquals(list2, javaContainer);
    	}else {
    		Assert.assertEquals(list2.size(), javaContainer.size());
    		for(Map.Entry<Short,Double> entry : javaContainer.entrySet()) {
    			Assert.assertTrue(list2.contains(entry.getKey(), entry.getValue()));
    		}
    	}
    	QMap<String,Object> variantContainer = ContainerTest.toQVariantMap(list2);
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
    	
    	Assert.assertEquals(0.0, ContainerTest.associativeValue(container, (short)2));
		QMultiMap<Short,Double> changedContainer = (QMultiMap<Short,Double>)ContainerTest.associativeSetValue(container, (short)1, 11.1);
    	Assert.assertEquals(QMultiMap.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3, (short)1, 4.4), changedContainer);
    	QPair<Object,Object> result = ContainerTest.associativeFindAndReplace(container, (short)2, 22.2);
    	Assert.assertEquals(Double.valueOf(2.2), result.second);
    	Assert.assertEquals(QMultiMap.of((short)1, 1.1, (short)2, 22.2, (short)3, 3.3, (short)1, 4.4), result.first);
    	changedContainer = (QMultiMap<Short,Double>)ContainerTest.associativeInsertKey(container, (short)1);
    	Assert.assertEquals(QMultiMap.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3, (short)1, 4.4, (short)1, 0.0), changedContainer);
    	changedContainer = (QMultiMap<Short,Double>)ContainerTest.associativeInsertKey(container, (short)128);
    	Assert.assertEquals(QMultiMap.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3, (short)1, 4.4, (short)128, 0.0), changedContainer);
    	changedContainer = (QMultiMap<Short,Double>)ContainerTest.associativeSetValue(container, (short)128, 128.128);
    	Assert.assertEquals(QMultiMap.of((short)1, 1.1, (short)2, 2.2, (short)3, 3.3, (short)1, 4.4), changedContainer);
    }
    
    @Test
    public void testEmptyQMapToVariantContainerNotCrashing() {
		ContainerTest.toQVariantHash(new QMap<>(String.class,String.class));
		ContainerTest.toQVariantMap(new QMap<>(String.class,String.class));
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestContainerAccessMap.class.getName());
    }
}
