/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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
import static org.junit.Assert.assertTrue;

import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Deque;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Queue;
import java.util.Set;
import java.util.SortedMap;
import java.util.TreeMap;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import io.qt.autotests.generated.QHash_int;
import io.qt.autotests.generated.QList_int;
import io.qt.autotests.generated.QMap_int;
import io.qt.autotests.generated.QQueue_int;
import io.qt.autotests.generated.QSet_int;
import io.qt.autotests.generated.QStack_int;
import io.qt.autotests.generated.QVector_int;
import io.qt.autotests.generated.Tulip;
import io.qt.core.QByteArray;
import io.qt.core.QCoreApplication;
import io.qt.core.QHash;
import io.qt.core.QList;
import io.qt.core.QMap;
import io.qt.core.QMultiHash;
import io.qt.core.QMultiMap;
import io.qt.core.QObject;
import io.qt.core.QPair;
import io.qt.core.QPoint;
import io.qt.core.QQueue;
import io.qt.core.QVector;
import io.qt.gui.QColor;
import io.qt.gui.QLinearGradient;

public class TestContainers extends QApplicationTest {
	
    private Tulip tulip;

    @Before
    public void setUp() throws Exception {
        tulip = new Tulip();
    }

    @After
    public void tearDown() throws Exception {
        tulip = null;
    }

    @Test
    public void run_writeReadVectorOfPairs() {
        // Generate my data...
        // We change the generics from Double to Number to allow it to compile on ARM and non-ARM
        // It was a QPair<Double, QColor> but on ARM it needs to be QPair<Float, QColor>
        List<QPair<Double, QColor>> write_stops = new ArrayList<QPair<Double, QColor>>();
        for (int i = 0; i < 11; ++i) {
            QPair<Double, QColor> p = new QPair<Double, QColor>(i / 10.0, new QColor(i * 255 / 10, 0, 0));
            write_stops.add(p);
        }

        // Set data..
        QLinearGradient gradient = new QLinearGradient(0, 0, 100, 100);
        gradient.setStops(write_stops);

        // Read back...
        List<QPair<Double, QColor>> read_stops = gradient.stops();

        assertEquals(write_stops.size(), read_stops.size());
        for (int i = 0; i < write_stops.size(); ++i) {
            QPair<Double, QColor> wstop = (QPair<Double, QColor>) write_stops.get(i);
            QPair<Double, QColor> rstop = (QPair<Double, QColor>) read_stops.get(i);
            assertEquals(wstop.first, rstop.first); // compare as Number (since could be Float/Double)
            QColor wsecond = wstop.second;
            QColor rsecond = rstop.second;
            assertEquals(wsecond.red(), rsecond.red());
            assertEquals(wsecond.green(), rsecond.green());
            assertEquals(wsecond.blue(), rsecond.blue());
        }
    }

    @Test
    public void run_objectChildren() {
        QObject root = new QObject();
        root.setObjectName("parentObject");
        QObject child1 = new QObject(root);
        QObject child2 = new QObject(root);
        QObject child3 = new QObject(root);
        child1.setObjectName("child1");
        child2.setObjectName("child2");
        child3.setObjectName("child3");

        List<QObject> children = root.children();
        assertEquals(children.size(), 3);
        assertEquals(children.get(0).objectName(), "child1");
        assertEquals(children.get(1).objectName(), "child2");
        assertEquals(children.get(2).objectName(), "child3");

        assertEquals(children.get(0), child1);
        assertEquals(children.get(1), child2);
        assertEquals(children.get(2), child3);
    }

    @Test
    public void run_QStringList() {
        List<String> items = new ArrayList<String>();
        for (int i = 0; i < 10; ++i)
            items.add("" + i);

        // Set the paths.
        QCoreApplication.setLibraryPaths(items);

        // Get the paths back..
        List<String> read_items = QCoreApplication.libraryPaths();
        assertEquals(items.size(), read_items.size());
        for (int i = 0; i < 10; ++i) {
            assertEquals(items.get(i), read_items.get(i));
        }
    }

    @Test
    public void run_testLinkedLists() {
        LinkedList<Integer> l = new LinkedList<Integer>();
        for (int i = 0; i < 10; ++i)
            l.add(i);

        LinkedList<Integer> l2 = tulip.do_QLinkedList_of_int(l);
        assertTrue(l2 != null);
        assertEquals(l, l2);
    }

    @Test
    public void run_testLists() {
        List<Integer> l = new ArrayList<Integer>();
        for (int i = 0; i < 10; ++i)
            l.add(i);

        List<Integer> l2 = tulip.do_QList_of_int(l);
        assertTrue(l2 != null);
        assertEquals(l, l2);
    }

    @Test
    public void run_testVectors() {
        List<Integer> l = new ArrayList<Integer>();
        for (int i = 0; i < 10; ++i)
            l.add(i);

        List<Integer> l2 = tulip.do_QVector_of_int(l);
        assertTrue(l2 != null);
        assertEquals(l, l2);
    }

    @Test
    public void run_testHash() {
        HashMap<String, String> h = new HashMap<String, String>();
        for (int i = 0; i < 10; ++i)
            h.put("key_" + String.valueOf(i), "value_" + String.valueOf(i));
        Map<String, String> h2 = tulip.do_QHash_of_strings(h);
        assertTrue(h2 != null);
        assertEquals(h, h2);
    }

    @Test
    public void run_testMap() {
        SortedMap<String, String> s = new TreeMap<String, String>();
        for (int i = 0; i < 10; ++i)
            s.put("key_" + String.valueOf(i), "value_" + String.valueOf(i));
        SortedMap<String, String> s2 = tulip.do_QMap_of_strings(s);
        assertTrue(s2 != null);
        assertEquals(s, s2);
    }

    @Test
    public void run_testStringList() {
        List<String> s = new ArrayList<String>();
        for (int i = 0; i < 10; ++i)
            s.add("value_" + String.valueOf(i));

        List<String> s2 = tulip.do_QStringList(s);
        assertTrue(s2 != null);
        assertEquals(s, s2);
    }

    @Test
    public void run_testQPair() {
        QPair<Integer, Integer> p1 = new QPair<Integer, Integer>(1, 2);
        QPair<Integer, Integer> p2 = tulip.do_QPair_of_ints(p1);

        assertTrue(p1 != null);
        assertTrue(p1.first != null);
        assertTrue(p1.second != null);
        assertTrue(p2 != null);
        assertTrue(p2.first != null);
        assertTrue(p2.second != null);

        assertEquals(p1, p2);
    }

    @Test
    public void run_testStack() {
        ArrayDeque<Integer> s = new ArrayDeque<Integer>();
        for (int i = 0; i < 10; ++i)
            s.push(i);
        Deque<Integer> s2 = tulip.do_QStack_of_int(s);
        assertTrue(s2 != null);
        assertEquals(List.copyOf(s), List.copyOf(s2));
    }

    @Test
    public void run_testQStack() {
        QStack_int s = new QStack_int();
        for (int i = 0; i < 10; ++i)
            s.push(i);
        Deque<Integer> s2 = tulip.do_QStack_of_int(s);
        assertTrue(s2 != null);
        assertTrue("stack not equals", s.equals(s2));
    }

    @Test
    public void run_testSet() {
        Set<Integer> s = new HashSet<Integer>();
        for (int i = 0; i < 10; ++i)
            s.add(i);
        Set<Integer> s2 = tulip.do_QSet_of_int(s);
        assertTrue(s2 != null);
        assertEquals(s, s2);
    }

    @Test
    public void run_testQSet() {
        QSet_int s = new QSet_int();
        for (int i = 0; i < 10; ++i)
            s.insert(i);
        Set<Integer> s2 = tulip.do_QSet_of_int(s);
        assertTrue(s2 != null);
        assertEquals(s, s2);
    }

    @Test
    public void run_testQueue() {
        Queue<Integer> q = new LinkedList<Integer>();
        for (int i = 0; i < 10; ++i)
            q.add(i);
        Queue<Integer> q2 = tulip.do_QQueue_of_int(q);
        assertTrue("queue object exists", q2 != null);
        assertEquals("queues are not equals", q, q2);
    }

    @Test
    public void run_testQQueue() {
        QQueue_int q = new QQueue_int();
        for (int i = 0; i < 10; ++i)
            q.append(i);
        Queue<Integer> q2 = tulip.do_QQueue_of_int(q);
        assertTrue("queue object exists", q2 != null);
        assertEquals("queues are not equals", q, q2);
    }


    @Test
    public void run_testQVector_outofbounds() {
        QVector_int vector = new QVector_int();

        boolean got;

        got = false; try { vector.get(10); } catch (Exception e) { got = true; } assertTrue("exception expected", got);
        got = false; try { vector.set(23, 14); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { vector.removeLast(); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { vector.removeFirst(); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { vector.takeLast(); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { vector.takeFirst(); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { vector.remove((Object)23); } catch (Exception e) { got =true; } assertFalse("exception unexpected", got);
        got = false; try { vector.removeAt(23); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { vector.takeAt(23); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { vector.remove(23, 14); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { vector.mid(23, 14); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { vector.move(23, 14); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
    }
    
    @Test
    public void run_testQList_iterator() {
    	QList_int list = new QList_int();
    	list.add(1);
    	list.add(2);
    	list.append(3);
    	int k=0;
    	int s=0;
    	for(Integer i : list) {
    		k++;
    		s += i;
    	}
    	assertEquals("list count", 3, k);
    	assertEquals("sum of list entries", 6, s);
    }
    
    @Test
    public void run_testQVector_iterator() {
    	QVector_int list = new QVector_int();
    	list.add(1);
    	list.add(2);
    	list.append(3);
    	int k=0;
    	int s=0;
    	for(Integer i : list) {
    		k++;
    		s += i;
    	}
    	assertEquals("list count", 3, k);
    	assertEquals("sum of vector entries", 6, s);
    }

    @SuppressWarnings("deprecation")
	@Test
    public void run_testQList_outofbounds() {
        QList_int list = new QList_int();

        boolean got;

        got = false; try { list.get(10); } catch (Exception e) { got = true; } assertTrue("exception expected", got);
        got = false; try { list.set(23, 14); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { list.removeLast(); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { list.removeFirst(); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { list.takeLast(); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { list.takeFirst(); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { list.move(14, 15); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { list.swap(15, 14); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { list.remove((Object)14); } catch (Exception e) { got =true; } assertFalse("exception unexpected", got);
        got = false; try { list.takeAt(14); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { list.removeAt(23); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { list.takeAt(23); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { list.mid(23, 14); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { list.move(23, 14); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
    }
    
    @Test
    public void run_testQMap_int() {
    	QMap_int map = new QMap_int();
    	map.insert(0, 1);
    	map.insert(1, 5);
    	map.insert(2, 9);
    	map.insert(2, 10);
    	assertEquals("map size", 3, map.size());
    	assertEquals("number of 0 values", 1, map.values(0).size());
    	assertEquals("number of 2 values", 1, map.values(2).size());
    	assertEquals("number of 1 values", 5, map.value(1));
    	int c=0;
    	int k=0;
    	int v=0;
    	for(QPair<Integer,Integer> p : map) {
    		c++;
    		k += p.first;
    		v += p.second;
    	}
    	assertEquals("map count", 3, c);
    	assertEquals("sum of map keys", 3, k);
    	assertEquals("sum of map values", 16, v);
    	c=0;
    	k=0;
    	v=0;
    	for(Map.Entry<Integer,Integer> p : map.entrySet()) {
    		c++;
    		k += p.getKey();
    		v += p.getValue();
    	}
    	assertEquals("entry set count", 3, c);
    	assertEquals("sum of entry set keys", 3, k);
    	assertEquals("sum of entry set values", 16, v);
    	assertEquals("entry set size", 3, map.entrySet().size());
    	assertEquals("tail map size", 2, map.tailMap(1).size());
    	assertEquals("head map size", 1, map.headMap(1).size());
    	assertEquals("sub map size", 0, map.subMap(1, 1).size());
    	assertEquals("sub map size", 1, map.subMap(1, 2).size());
    }
    
    @Test
    public void run_testQHash_int() {
    	QHash_int map = new QHash_int();
    	map.insert(0, 1);
    	map.insert(1, 5);
    	map.insert(2, 9);
    	map.insert(2, 10);
    	assertEquals("hash size", 3, map.size());
    	assertEquals("number of 0 values", 1, map.values(0).size());
    	assertEquals("number of 2 values", 1, map.values(2).size());
    	assertEquals("number of 1 values", 5, map.value(1));
    	int c=0;
    	int k=0;
    	int v=0;
    	for(QPair<Integer,Integer> p : map) {
    		c++;
    		k += p.first;
    		v += p.second;
    	}
    	assertEquals("hash count", 3, c);
    	assertEquals("sum of map keys", 3, k);
    	assertEquals("sum of map values", 16, v);
    	c=0;
    	k=0;
    	v=0;
    	for(Map.Entry<Integer,Integer> p : map.entrySet()) {
    		c++;
    		k += p.getKey();
    		v += p.getValue();
    	}
    	assertEquals("entry set count", 3, c);
    	assertEquals("sum of entry set keys", 3, k);
    	assertEquals("sum of entry set values", 16, v);
    	assertEquals("entry set size", 3, map.entrySet().size());
    }
    
    @Test
    public void run_testNativeQListMemberField() {
    	List<String> listRef = tulip.listRef();
    	List<String> constList = tulip.m_list();
    	for (String string : listRef) {
			string.length();
		}
    	assertEquals(0, listRef.size());
    	assertEquals(0, constList.size());
    	listRef.add("test");
    	assertEquals(1, listRef.size());
    	assertEquals(1, constList.size());
    	try {
    		constList.add("X");
    		assertFalse(true);
    	}catch(UnsupportedOperationException e) {
    		assertTrue(true);
    	}
    	try {
    		constList.remove("test");
    		assertFalse(true);
    	}catch(UnsupportedOperationException e) {
    		assertTrue(true);
    	}
    	try {
    		constList.remove(0);
    		assertFalse(true);
    	}catch(UnsupportedOperationException e) {
    		assertTrue(true);
    	}
    	assertEquals("test", constList.get(0));
    	listRef.add("test2");
    	assertEquals(2, listRef.size());
    	assertEquals(2, constList.size());
    	String concat = "";
    	for (String string : listRef) {
    		concat += string;
		}
    	assertEquals("testtest2", concat);
    	assertTrue(constList.contains("test2"));
    	
    	QList<String> qlistRef = (QList<String>)listRef;
    	QList<String> constQList = (QList<String>)constList;
    	listRef.add("test");
    	assertEquals(2, qlistRef.count("test"));
    	assertEquals(2, constQList.count("test"));
    	assertEquals(1, qlistRef.indexOf("test2"));
    	assertEquals(1, qlistRef.lastIndexOf("test2"));
    	qlistRef.prepend("doggy");
    	qlistRef.prepend("catty");
    	assertEquals(3, constQList.indexOf("test2"));
    	assertEquals(3, constQList.lastIndexOf("test2"));
    	assertEquals(List.of("doggy", "test"), qlistRef.mid(1, 2));
    	assertEquals(List.of("doggy", "test"), constQList.mid(1, 2));
    	qlistRef.move(0, 1);
    	assertEquals(List.of("catty", "test"), qlistRef.mid(1, 2));
    	assertEquals(List.of("catty", "test"), constQList.mid(1, 2));
    	qlistRef.insert(2, "horsy");
    	assertEquals(List.of("catty", "horsy"), qlistRef.mid(1, 2));
    	assertEquals(List.of("catty", "horsy"), constQList.mid(1, 2));
    	qlistRef.removeAt(1);
    	assertEquals(List.of("horsy", "test"), qlistRef.mid(1, 2));
    	assertEquals(List.of("horsy", "test"), constQList.mid(1, 2));
    	qlistRef.removeOne("horsy");
    	assertEquals(List.of("test", "test2"), qlistRef.mid(1, 2));
    	assertEquals(List.of("test", "test2"), constQList.mid(1, 2));
    	qlistRef.removeAll("test");
    	assertEquals(2, listRef.size());
    	assertEquals(2, constList.size());
    	qlistRef.swapItemsAt(0, 1);
    	assertEquals(List.of("test2", "doggy"), qlistRef);
    	assertEquals(List.of("test2", "doggy"), constQList);
    	assertEquals("test2", qlistRef.takeAt(0));
    	assertEquals("", qlistRef.value(20));
    	assertEquals("nothing", qlistRef.value(20, "nothing"));
    }
    
    @Test
    public void run_testNativeQList() {
    	List<String> listRef = tulip.listRef();
    	List<String> constList = tulip.constList();
    	for (String string : listRef) {
			string.length();
		}
    	assertEquals(0, listRef.size());
    	assertEquals(0, constList.size());
    	listRef.add("test");
    	assertEquals(1, listRef.size());
    	assertEquals(1, constList.size());
    	try {
    		constList.add("X");
    		assertFalse(true);
    	}catch(UnsupportedOperationException e) {
    		assertTrue(true);
    	}
    	try {
    		constList.remove("test");
    		assertFalse(true);
    	}catch(UnsupportedOperationException e) {
    		assertTrue(true);
    	}
    	try {
    		constList.remove(0);
    		assertFalse(true);
    	}catch(UnsupportedOperationException e) {
    		assertTrue(true);
    	}
    	assertEquals("test", constList.get(0));
    	listRef.add("test2");
    	assertEquals(2, listRef.size());
    	assertEquals(2, constList.size());
    	String concat = "";
    	for (String string : listRef) {
    		concat += string;
		}
    	assertEquals("testtest2", concat);
    	assertTrue(constList.contains("test2"));
    	
    	QList<String> qlistRef = (QList<String>)listRef;
    	QList<String> constQList = (QList<String>)constList;
    	listRef.add("test");
    	assertEquals(2, qlistRef.count("test"));
    	assertEquals(2, constQList.count("test"));
    	assertEquals(1, qlistRef.indexOf("test2"));
    	assertEquals(1, qlistRef.lastIndexOf("test2"));
    	qlistRef.prepend("doggy");
    	qlistRef.prepend("catty");
    	assertEquals(3, constQList.indexOf("test2"));
    	assertEquals(3, constQList.lastIndexOf("test2"));
    	assertEquals(List.of("doggy", "test"), qlistRef.mid(1, 2));
    	assertEquals(List.of("doggy", "test"), constQList.mid(1, 2));
    	qlistRef.move(0, 1);
    	assertEquals(List.of("catty", "test"), qlistRef.mid(1, 2));
    	assertEquals(List.of("catty", "test"), constQList.mid(1, 2));
    	qlistRef.insert(2, "horsy");
    	assertEquals(List.of("catty", "horsy"), qlistRef.mid(1, 2));
    	assertEquals(List.of("catty", "horsy"), constQList.mid(1, 2));
    	qlistRef.removeAt(1);
    	assertEquals(List.of("horsy", "test"), qlistRef.mid(1, 2));
    	assertEquals(List.of("horsy", "test"), constQList.mid(1, 2));
    	qlistRef.removeOne("horsy");
    	assertEquals(List.of("test", "test2"), qlistRef.mid(1, 2));
    	assertEquals(List.of("test", "test2"), constQList.mid(1, 2));
    	qlistRef.removeAll("test");
    	assertEquals(2, listRef.size());
    	assertEquals(2, constList.size());
    	qlistRef.swapItemsAt(0, 1);
    	assertEquals(List.of("test2", "doggy"), qlistRef);
    	assertEquals(List.of("test2", "doggy"), constQList);
    	assertEquals("test2", qlistRef.takeAt(0));
    	assertEquals("", qlistRef.value(20));
    	assertEquals("nothing", qlistRef.value(20, "nothing"));
    }
    
    @Test
    public void run_testNativeQStringList() {
    	List<String> listRef = tulip.stringListRef();
    	List<String> constList = tulip.constStringList();
    	for (String string : listRef) {
			string.length();
		}
    	assertEquals(0, listRef.size());
    	assertEquals(0, constList.size());
    	listRef.add("test");
    	assertEquals(1, listRef.size());
    	assertEquals(1, constList.size());
    	try {
    		constList.add("X");
    		assertFalse(true);
    	}catch(UnsupportedOperationException e) {
    		assertTrue(true);
    	}
    	try {
    		constList.remove("test");
    		assertFalse(true);
    	}catch(UnsupportedOperationException e) {
    		assertTrue(true);
    	}
    	try {
    		constList.remove(0);
    		assertFalse(true);
    	}catch(UnsupportedOperationException e) {
    		assertTrue(true);
    	}
    	assertEquals("test", constList.get(0));
    	listRef.add("test2");
    	assertEquals(2, listRef.size());
    	assertEquals(2, constList.size());
    	String concat = "";
    	for (String string : listRef) {
    		concat += string;
		}
    	assertEquals("testtest2", concat);
    	assertTrue(constList.contains("test2"));
    	
    	QList<String> qlistRef = (QList<String>)listRef;
    	QList<String> constQList = (QList<String>)constList;
    	listRef.add("test");
    	assertEquals(2, qlistRef.count("test"));
    	assertEquals(2, constQList.count("test"));
    	assertEquals(1, qlistRef.indexOf("test2"));
    	assertEquals(1, qlistRef.lastIndexOf("test2"));
    	qlistRef.prepend("doggy");
    	qlistRef.prepend("catty");
    	assertEquals(3, constQList.indexOf("test2"));
    	assertEquals(3, constQList.lastIndexOf("test2"));
    	assertEquals(List.of("doggy", "test"), qlistRef.mid(1, 2));
    	assertEquals(List.of("doggy", "test"), constQList.mid(1, 2));
    	qlistRef.move(0, 1);
    	assertEquals(List.of("catty", "test"), qlistRef.mid(1, 2));
    	assertEquals(List.of("catty", "test"), constQList.mid(1, 2));
    	qlistRef.insert(2, "horsy");
    	assertEquals(List.of("catty", "horsy"), qlistRef.mid(1, 2));
    	assertEquals(List.of("catty", "horsy"), constQList.mid(1, 2));
    	qlistRef.removeAt(1);
    	assertEquals(List.of("horsy", "test"), qlistRef.mid(1, 2));
    	assertEquals(List.of("horsy", "test"), constQList.mid(1, 2));
    	qlistRef.removeOne("horsy");
    	assertEquals(List.of("test", "test2"), qlistRef.mid(1, 2));
    	assertEquals(List.of("test", "test2"), constQList.mid(1, 2));
    	qlistRef.removeAll("test");
    	assertEquals(2, listRef.size());
    	assertEquals(2, constList.size());
    	qlistRef.swapItemsAt(0, 1);
    	assertEquals(List.of("test2", "doggy"), qlistRef);
    	assertEquals(List.of("test2", "doggy"), constQList);
    	assertEquals("test2", qlistRef.takeAt(0));
    	assertEquals("", qlistRef.value(20));
    	assertEquals("nothing", qlistRef.value(20, "nothing"));
    }
    
    @Test
    public void run_testNativeQueue() {
    	Queue<String> listRef = tulip.queueRef();
    	Queue<String> constList = tulip.constQueue();
    	for (String string : listRef) {
			string.length();
		}
    	assertEquals(0, listRef.size());
    	assertEquals(0, constList.size());
    	listRef.add("test");
    	assertEquals(1, listRef.size());
    	assertEquals(1, constList.size());
    	try {
    		constList.add("X");
    		assertFalse(true);
    	}catch(UnsupportedOperationException e) {
    		assertTrue(true);
    	}
    	try {
    		constList.remove("test");
    		assertFalse(true);
    	}catch(UnsupportedOperationException e) {
    		assertTrue(true);
    	}
    	assertEquals("test", constList.peek());
    	listRef.add("test2");
    	assertEquals(2, listRef.size());
    	assertEquals(2, constList.size());
    	String concat = "";
    	for (String string : listRef) {
    		concat += string;
		}
    	assertEquals("testtest2", concat);
    	assertTrue(constList.contains("test2"));
    	
    	QQueue<String> qlistRef = (QQueue<String>)listRef;
    	QQueue<String> constQList = (QQueue<String>)constList;
    	listRef.add("test");
    	assertEquals(2, qlistRef.count("test"));
    	assertEquals(2, constQList.count("test"));
    	assertEquals(1, qlistRef.indexOf("test2"));
    	assertEquals(1, qlistRef.lastIndexOf("test2"));
    	qlistRef.prepend("doggy");
    	qlistRef.prepend("catty");
    	assertEquals(3, constQList.indexOf("test2"));
    	assertEquals(3, constQList.lastIndexOf("test2"));
    	assertEquals(List.of("doggy", "test"), qlistRef.mid(1, 2));
    	assertEquals(List.of("doggy", "test"), constQList.mid(1, 2));
    	qlistRef.move(0, 1);
    	assertEquals(List.of("catty", "test"), qlistRef.mid(1, 2));
    	assertEquals(List.of("catty", "test"), constQList.mid(1, 2));
    	qlistRef.insert(2, "horsy");
    	assertEquals(List.of("catty", "horsy"), qlistRef.mid(1, 2));
    	assertEquals(List.of("catty", "horsy"), constQList.mid(1, 2));
    	qlistRef.removeAt(1);
    	assertEquals(List.of("horsy", "test"), qlistRef.mid(1, 2));
    	assertEquals(List.of("horsy", "test"), constQList.mid(1, 2));
    	qlistRef.removeOne("horsy");
    	assertEquals(List.of("test", "test2"), qlistRef.mid(1, 2));
    	assertEquals(List.of("test", "test2"), constQList.mid(1, 2));
    	qlistRef.removeAll("test");
    	assertEquals(2, listRef.size());
    	assertEquals(2, constList.size());
    	qlistRef.swap(0, 1);
    	assertEquals(List.of("test2", "doggy"), qlistRef);
    	assertEquals(List.of("test2", "doggy"), constQList);
    	assertEquals("test2", qlistRef.takeAt(0));
    	assertEquals("", qlistRef.value(20));
    	assertEquals("nothing", qlistRef.value(20, "nothing"));
    }
    
    @Test
    public void run_testNativeQByteArrayList() {
    	List<QByteArray> listRef = tulip.byteArrayListRef();
    	List<QByteArray> constList = tulip.constByteArrayList();
    	for (QByteArray string : listRef) {
			string.length();
		}
    	assertEquals(0, listRef.size());
    	assertEquals(0, constList.size());
    	listRef.add(new QByteArray("test"));
    	assertEquals(1, listRef.size());
    	assertEquals(1, constList.size());
    	try {
    		constList.add(new QByteArray("X"));
    		assertFalse(true);
    	}catch(UnsupportedOperationException e) {
    		assertTrue(true);
    	}
    	try {
    		constList.remove(new QByteArray("test"));
    		assertFalse(true);
    	}catch(UnsupportedOperationException e) {
    		assertTrue(true);
    	}
    	try {
    		constList.remove(0);
    		assertFalse(true);
    	}catch(UnsupportedOperationException e) {
    		assertTrue(true);
    	}
    	assertEquals(new QByteArray("test"), constList.get(0));
    	listRef.add(new QByteArray("test2"));
    	assertEquals(2, listRef.size());
    	assertEquals(2, constList.size());
    	String concat = "";
    	for (QByteArray string : listRef) {
    		concat += string;
		}
    	assertEquals("testtest2", concat);
    	assertTrue(constList.contains(new QByteArray("test2")));
    	
    	QList<QByteArray> qlistRef = (QList<QByteArray>)listRef;
    	QList<QByteArray> constQList = (QList<QByteArray>)constList;
    	listRef.add(new QByteArray("test"));
    	assertEquals(2, qlistRef.count(new QByteArray("test")));
    	assertEquals(2, constQList.count(new QByteArray("test")));
    	assertEquals(1, qlistRef.indexOf(new QByteArray("test2")));
    	assertEquals(1, qlistRef.lastIndexOf(new QByteArray("test2")));
    	qlistRef.prepend(new QByteArray("doggy"));
    	qlistRef.prepend(new QByteArray("catty"));
    	assertEquals(3, constQList.indexOf(new QByteArray("test2")));
    	assertEquals(3, constQList.lastIndexOf(new QByteArray("test2")));
    	assertEquals(List.of(new QByteArray("doggy"), new QByteArray("test")), qlistRef.mid(1, 2));
    	assertEquals(List.of(new QByteArray("doggy"), new QByteArray("test")), constQList.mid(1, 2));
    	qlistRef.move(0, 1);
    	assertEquals(List.of(new QByteArray("catty"), new QByteArray("test")), qlistRef.mid(1, 2));
    	assertEquals(List.of(new QByteArray("catty"), new QByteArray("test")), constQList.mid(1, 2));
    	qlistRef.insert(2, new QByteArray("horsy"));
    	assertEquals(List.of(new QByteArray("catty"), new QByteArray("horsy")), qlistRef.mid(1, 2));
    	assertEquals(List.of(new QByteArray("catty"), new QByteArray("horsy")), constQList.mid(1, 2));
    	qlistRef.removeAt(1);
    	assertEquals(List.of(new QByteArray("horsy"), new QByteArray("test")), qlistRef.mid(1, 2));
    	assertEquals(List.of(new QByteArray("horsy"), new QByteArray("test")), constQList.mid(1, 2));
    	qlistRef.removeOne(new QByteArray("horsy"));
    	assertEquals(List.of(new QByteArray("test"), new QByteArray("test2")), qlistRef.mid(1, 2));
    	assertEquals(List.of(new QByteArray("test"), new QByteArray("test2")), constQList.mid(1, 2));
    	qlistRef.removeAll(new QByteArray("test"));
    	assertEquals(2, listRef.size());
    	assertEquals(2, constList.size());
    	qlistRef.swapItemsAt(0, 1);
    	assertEquals(List.of(new QByteArray("test2"), new QByteArray("doggy")), qlistRef);
    	assertEquals(List.of(new QByteArray("test2"), new QByteArray("doggy")), constQList);
    	assertEquals(new QByteArray("test2"), qlistRef.takeAt(0));
    	assertEquals(new QByteArray(), qlistRef.value(20));
    	assertEquals(new QByteArray("nothing"), qlistRef.value(20, new QByteArray("nothing")));
    }

    @Test
    public void run_testNativeQListList() {
    	List<List<Float>> listRef = tulip.listListRef();
    	List<List<Float>> constList = tulip.constListList();
    	listRef.add(List.of(3f, 5.5f, 2.8f));
    	assertEquals(List.of(3f, 5.5f, 2.8f), constList.get(0));
    }
    
    @Test
    public void run_testNativeQVector() {
    	QVector<Double> listRef = tulip.vectorRef();
    	QVector<Double> constList = tulip.constVector();
    	listRef.append(List.of(3., 5.5, 2.8));
    	assertEquals(List.of(3., 5.5, 2.8), constList.toList());
    	assertEquals(Double.valueOf(3.), constList.at(0));
    	listRef.clear();
    	assertEquals(0, constList.count());
    	listRef.append(List.of(3., 5.5, 2.8));
    	listRef.append(3.);
    	assertEquals(2, constList.count(3.));
    	assertEquals(5, listRef.capacity());
    	listRef.capacity();
    	listRef.fill(6., 10);
    	assertEquals(10, constList.count(6.));
    	listRef.prepend(90.);
    	listRef.move(4, 0);
    	listRef.takeLast();
    	listRef.startsWith(5.);
    	listRef.resize(40);
    	listRef.dispose();
    }
    
    @Test
    public void run_testNativeQMap() {
    	QMap<String, QPoint> mapRef = tulip.mapRef();
    	QMap<String, QPoint> constMap = tulip.constMap();
    	mapRef.insert("1", new QPoint(0,0));
    	mapRef.insert("2", new QPoint(5,6));
    	mapRef.insert("A", new QPoint(6,7));
    	mapRef.insert("1", new QPoint(1,1));
    	assertEquals(3, constMap.count());
    	assertEquals(new QPoint(1,1), constMap.first());
    	assertEquals(new QPoint(6,7), constMap.last());
    	assertEquals("1", constMap.firstKey());
    	assertEquals("A", constMap.lastKey());
    	assertEquals(new QPoint(6,7), mapRef.take("A"));
    	mapRef.removeAll("1");
    	assertEquals("2", constMap.firstKey());
    	assertEquals("2", constMap.lastKey());
    	assertFalse(constMap.find("X").keyValuePair().isPresent());
    	assertEquals(new QPoint(5,6), constMap.find("2").keyValuePair().orElse(new QPair<>(null, null)).second);
    	assertEquals(Arrays.asList("2"), mapRef.keys());
    }
    
    @Test
    public void run_testNativeQMultiMap() {
    	QMultiMap<String, QPoint> mapRef = tulip.multiMapRef();
    	QMultiMap<String, QPoint> constMap = tulip.constMultiMap();
    	mapRef.insert("1", new QPoint(0,0));
    	mapRef.insert("2", new QPoint(5,6));
    	mapRef.insert("A", new QPoint(6,7));
    	mapRef.insert("1", new QPoint(1,1));
    	assertEquals(4, constMap.count());
    	assertEquals(new QPoint(1,1), constMap.first());
    	assertEquals(new QPoint(6,7), constMap.last());
    	assertEquals("1", constMap.firstKey());
    	assertEquals("A", constMap.lastKey());
    	assertEquals(new QPoint(6,7), mapRef.take("A"));
    	assertEquals(2, mapRef.removeAll("1"));
    	assertEquals("2", constMap.firstKey());
    	assertEquals("2", constMap.lastKey());
    	assertFalse(constMap.find("X").keyValuePair().isPresent());
    	assertEquals(new QPoint(5,6), constMap.find("2").keyValuePair().orElse(new QPair<>(null, null)).second);
    	assertEquals(Arrays.asList("2"), mapRef.keys());
    }
    
    @Test
    public void run_testNativeQHash() {
    	QHash<Integer, QPoint> mapRef = tulip.hashRef();
    	QHash<Integer, QPoint> constMap = tulip.constHash();
    	mapRef.insert(1, new QPoint(0,0));
    	mapRef.insert(2, new QPoint(5,6));
    	mapRef.insert(3, new QPoint(6,7));
    	mapRef.insert(1, new QPoint(1,1));
    	assertEquals(3, constMap.count());
    	assertEquals(new QPoint(6,7), mapRef.take(3));
    	mapRef.removeAll(1);
    	assertFalse(constMap.find(20).keyValuePair().isPresent());
    	assertEquals(new QPoint(5,6), constMap.find(2).keyValuePair().orElse(new QPair<>(null, null)).second);
    	assertEquals(Arrays.asList(2), mapRef.keys());
    }
    
    @Test
    public void run_testNativeQMultiHash() {
    	QMultiHash<Integer, QPoint> mapRef = tulip.multiHashRef();
    	QMultiHash<Integer, QPoint> constMap = tulip.constMultiHash();
    	mapRef.insert(1, new QPoint(0,0));
    	mapRef.insert(2, new QPoint(5,6));
    	mapRef.insert(3, new QPoint(6,7));
    	mapRef.insert(1, new QPoint(1,1));
    	assertEquals(4, constMap.count());
    	assertEquals(new QPoint(6,7), mapRef.take(3));
    	mapRef.removeAll(1);
    	assertFalse(constMap.find(20).keyValuePair().isPresent());
    	assertEquals(new QPoint(5,6), constMap.find(2).keyValuePair().orElse(new QPair<>(null, null)).second);
    	assertEquals(Arrays.asList(2), mapRef.keys());
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestContainers.class.getName());
    }
}
