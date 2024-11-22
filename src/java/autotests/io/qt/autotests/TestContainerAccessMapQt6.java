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

import java.util.Arrays;

import org.junit.Assert;
import org.junit.Test;

import io.qt.core.QAssociativeConstIterator;
import io.qt.core.QList;
import io.qt.core.QMap;
import io.qt.core.QMultiMap;
import io.qt.core.QPair;

public class TestContainerAccessMapQt6 extends UnitTestInitializer {
	
    @SuppressWarnings("unlikely-arg-type")
	@Test
    public void testMultiMap() {
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
    	QAssociativeConstIterator<Integer, Integer> iter = map.find(3);
    	Assert.assertEquals("find(3)", QPair.pair(3,5), iter.keyValuePair().orElse(null));
    	iter = map.lowerBound(3);
    	Assert.assertEquals("lowerBound(3)", QPair.pair(3,5), iter.keyValuePair().orElse(null));
    	iter = map.upperBound(3);
    	Assert.assertEquals("upperBound(3)", QPair.pair(4,5), iter.keyValuePair().orElse(null));
    	iter = map.find(3);
    	Assert.assertEquals("find(3)", QPair.pair(3,5), iter.keyValuePair().orElse(null));
    	iter = map.find(6);
    	Assert.assertEquals("find(6)", null, iter.keyValuePair().orElse(null));
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
    	Assert.assertEquals("count(4)", 3, count);
    	count = map.count(4, 5);
    	Assert.assertEquals("count(4,5)", 1, count);
    	count = map.count(4, 6);
    	Assert.assertEquals("count(4,6)", 0, count);
    }
    
	@Test
    public void testMap() {
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
    	QAssociativeConstIterator<Integer, Integer> iter = map.find(3);
    	Assert.assertEquals("find(3)", QPair.pair(3,3), iter.keyValuePair().orElse(null));
    	iter = map.lowerBound(3);
    	Assert.assertEquals("find(3)", QPair.pair(3,3), iter.keyValuePair().orElse(null));
    	iter = map.upperBound(3);
    	Assert.assertEquals("find(3)", QPair.pair(4,4), iter.keyValuePair().orElse(null));
    	iter = map.find(3);
    	Assert.assertEquals("find(3)", QPair.pair(3,3), iter.keyValuePair().orElse(null));
    	iter = map.find(6);
    	Assert.assertEquals("find(3)", null, iter.keyValuePair().orElse(null));
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
    	Assert.assertEquals("count(4)", 2, count);
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestContainerAccessMapQt6.class.getName());
    }
}
