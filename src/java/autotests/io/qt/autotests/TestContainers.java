/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

import static io.qt.autotests.generated.ContainerFactory.*;
import static io.qt.autotests.generated.ContainerConverter.*;
import static org.junit.Assert.*;

import java.util.*;

import org.junit.*;

import io.qt.*;
import io.qt.autotests.generated.*;
import io.qt.core.*;
import io.qt.gui.*;
import io.qt.widgets.*;

public class TestContainers extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
    
    @Test
    public void testListSort(){
    	List<String> list = new QStringList("F", "G", "A", "B", "D", "C");
    	list.listIterator();
//    	list = new ArrayList<>(Arrays.asList("F", "G", "A", "B", "D", "C"));
    	list.sort(String::compareTo);
    	assertEquals(Arrays.asList("A", "B", "C", "D", "F", "G"), list);
    	ListIterator<String> iter = list.listIterator(2);
    	assertEquals(2, iter.nextIndex());
    	assertEquals("C", iter.next());
    	iter.remove();
    	assertEquals(Arrays.asList("A", "B", "D", "F", "G"), list);
    	assertEquals("D", iter.next());
    	iter.add("K");
    	assertEquals(Arrays.asList("A", "B", "D", "K", "F", "G"), list);
    	assertEquals("K", iter.previous());
    	assertEquals("K", iter.next());
    	assertEquals(3, iter.previousIndex());
    	assertEquals(3, iter.previousIndex());
    	assertEquals(4, iter.nextIndex());
    	assertEquals(4, iter.nextIndex());
    }
    
    @Test
    public void testStringList(){
    	Object container = new QString("A::B").split("::");
    	assertTrue(container instanceof QStringList);
    	QStringList list = (QStringList)container;
		List<QString> qStringList = new ArrayList<QString>();
    	qStringList.add(new QString("C"));
    	qStringList.add(new QString("D"));
    	qStringList.add(new QString("E"));
    	list.addAllStrings(qStringList);
    	list.add(new QString("F"));
    	assertEquals(Arrays.asList("A", "B", "C", "D", "E", "F"), list);
    	assertEquals(QList.of("A", "B", "C", "D", "E", "F"), list);
    	assertEquals(QList.of(new QString("A"), new QString("B"), new QString("C"), new QString("D"), new QString("E"), new QString("F")), list);
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
        QList<QPair<Double, QColor>> read_stops = gradient.stops();

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
    	List<String> libraryPaths = QCoreApplication.libraryPaths();
    	try {
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
    	}finally {
    		QCoreApplication.setLibraryPaths(libraryPaths);
    	}
    }

    @Test
    public void run_testLists() {
        List<Integer> l = new ArrayList<Integer>();
        for (int i = 0; i < 10; ++i)
            l.add(i);

        List<Integer> l2 = do_QList_of_int(l);
        assertTrue(l2 != null);
        assertEquals(l, l2);
    }

    @Test
    public void run_testHash() {
        HashMap<String, String> h = new HashMap<String, String>();
        for (int i = 0; i < 10; ++i)
            h.put("key_" + String.valueOf(i), "value_" + String.valueOf(i));
        Map<String, String> h2 = do_QHash_of_strings(h);
        assertTrue(h2 != null);
        assertEquals(h, h2);
    }

    @Test
    public void run_testMap() {
        SortedMap<String, String> s = new TreeMap<String, String>();
        for (int i = 0; i < 10; ++i)
            s.put("key_" + String.valueOf(i), "value_" + String.valueOf(i));
        SortedMap<String, String> s2 = do_QMap_of_strings(s);
        assertTrue(s2 != null);
        assertEquals(s, s2);
    }

    @Test
    public void run_testStringList() {
        List<String> s = new ArrayList<String>();
        for (int i = 0; i < 10; ++i)
            s.add("value_" + String.valueOf(i));

        List<String> s2 = do_QStringList(s);
        assertTrue(s2 != null);
        assertEquals(s, s2);
    }

    @Test
    public void run_testQPair() {
        QPair<Integer, Integer> p1 = new QPair<Integer, Integer>(1, 2);
        QPair<Integer, Integer> p2 = do_QPair_of_ints(p1);

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
        Collection<Integer> s2 = do_QStack_of_int(s);
        assertTrue(s2 != null);
        assertEquals(new ArrayList<Integer>(s), new ArrayList<Integer>(s2));
    }

    @Test
    public void run_testQStack() {
        QStack_int s = new QStack_int();
        for (int i = 0; i < 10; ++i)
            s.push(i);
        Collection<Integer> s2 = do_QStack_of_int(s);
        assertTrue(s2 != null);
        assertEquals(new ArrayList<Integer>(s), new ArrayList<Integer>(s2));
    }

    @Test
    public void run_testSet() {
        Set<Integer> s = new HashSet<Integer>();
        for (int i = 0; i < 10; ++i)
            s.add(i);
        Set<Integer> s2 = do_QSet_of_int(s);
        assertTrue(s2 != null);
        assertEquals(s, s2);
    }

    @Test
    public void run_testQSet() {
        QSet_int s = new QSet_int();
        for (int i = 0; i < 10; ++i)
            s.insert(i);
        Set<Integer> s2 = do_QSet_of_int(s);
        assertTrue(s2 != null);
        assertEquals(s2, s);
    }

    @Test
    public void run_testQueue() {
        Queue<Integer> q = new LinkedList<Integer>();
        for (int i = 0; i < 10; ++i)
            q.add(i);
        Queue<Integer> q2 = do_QQueue_of_int(q);
        assertTrue("queue object exists", q2 != null);
        assertEquals("queues are not equals", q, q2);
    }

    @Test
    public void run_testQQueue() {
        QQueue_int q = new QQueue_int();
        for (int i = 0; i < 10; ++i)
            q.append(i);
        Queue<Integer> q2 = do_QQueue_of_int(q);
        assertTrue("queue object exists", q2 != null);
        assertEquals("queues are not equals", new ArrayList<Integer>(q), new ArrayList<Integer>(q2));
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
    public void run_testQList_outofbounds() {
        QList_int list = new QList_int();
        try { 
        	list.remove(23, 14); 
        	fail("IndexOutOfBoundsException expected to be thrown");
    	} catch (IndexOutOfBoundsException e) {
    	}

        boolean got;

        got = false; try { list.get(10); } catch (Exception e) { got = true; } assertTrue("exception expected", got);
        got = false; try { list.set(23, 14); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { list.removeLast(); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { list.removeFirst(); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { list.takeLast(); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { list.takeFirst(); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { list.move(14, 15); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { list.swapItemsAt(15, 14); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { list.remove((Object)14); } catch (Exception e) { got =true; } assertFalse("exception unexpected", got);
        got = false; try { list.takeAt(14); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { list.removeAt(23); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { list.takeAt(23); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { list.mid(23, 14); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { list.move(23, 14); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
        got = false; try { list.insert(list.size()+1, 0); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
    }
    
    @Test
    public void run_testQMap_int() {
    	QMap_int map = new QMap_int();
    	map.insert(0, 1);
    	map.insert(1, 5);
    	map.insert(2, 9);
    	map.insert(2, 10);
    	assertEquals("map size", 3, map.size());
    	assertEquals("number of 1 values", Integer.valueOf(5), map.value(1));
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
    	assertEquals("number of 1 values", Integer.valueOf(5), map.value(1));
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
    public void test_create_QList_QPoint() {
    	@SuppressWarnings("deprecation")
		int typePrev = QMetaType.type("QList<QPoint>");
    	assertEquals(0, typePrev);
    	QList<QPoint> pointList = new QList<>(QPoint.class);
    	pointList.reserve(20);
    	@SuppressWarnings("deprecation")
    	int typeAfter = QMetaType.type("QList<QPoint>");
    	assertTrue(0!=typeAfter);
    	pointList.append(new QPoint(1,2));
    	pointList.append(new QPoint(3,4));
    	pointList.append(Arrays.asList(new QPoint(5,6)));
    	pointList.prepend(new QPoint(0,0));
    	assertEquals(new QPoint(5,6), pointList.at(3));
    	assertTrue(pointList.contains(new QPoint(3,4)));
    	assertEquals(4, pointList.size());
    	pointList.append(new QPoint(1,2));
    	assertEquals(2, pointList.count(new QPoint(1,2)));
    	assertEquals(1, pointList.indexOf(new QPoint(1,2)));
    	assertEquals(4, pointList.lastIndexOf(new QPoint(1,2)));
    	assertTrue(pointList.endsWith(new QPoint(1,2)));
    	assertFalse(pointList.startsWith(new QPoint(1,2)));
    	pointList.swapItemsAt(0, 1);
    	assertTrue(pointList.startsWith(new QPoint(1,2)));
    	assertFalse(pointList.equals(new QList<>(QPoint.class)));
    	assertEquals(new QPoint(100,200), pointList.value(100, new QPoint(100,200)));
    	pointList.removeAt(1);
    	assertEquals(new QPoint(5,6), pointList.takeAt(2));
    	pointList.append(new QPoint(3,4));
    	assertFalse(pointList.removeOne(new QPoint(5,6)));
    	assertEquals(2, pointList.removeAll(new QPoint(3,4)));
    	pointList.replace(0, new QPoint(10,20));
    	assertEquals(1, pointList.count(new QPoint(1,2)));
    	assertEquals(1, pointList.count(new QPoint(10,20)));
    	pointList.prepend(new QPoint(0,0));
    	pointList.move(0, 2);
    	assertTrue(pointList.endsWith(new QPoint(0,0)));
    	pointList.insert(1, new QPoint(30,30));
    	QList<QPoint> midList = pointList.mid(1);
    	assertEquals(3, midList.size());
    	assertTrue(midList.startsWith(new QPoint(30,30)));
    	int counter = 0;
    	for(@SuppressWarnings("unused") QPoint p : midList) {
    		++counter;
    	}
    	assertEquals(3, counter);
    	midList.clear();
    	assertEquals(0, midList.size());
    	pointList = new QList<>(QPoint.class);
    	pointList.add(0, new QPoint(5,6));
    	assertEquals(new QPoint(5,6), pointList.at(0));
    	assertEquals(1, pointList.size());
    }
    
    @Test
    public void test_create_QList_QPair() {
    	QMetaType pairType = QMetaType.fromType(QPair.class, QMetaType.fromType(int.class), QMetaType.fromType(int.class));
    	QList<QPair<Integer,Integer>> pointList = new QList<>(pairType);
    	pointList.reserve(20);
    	pointList.append(new QPair<>(1,2));
    	pointList.append(new QPair<>(3,4));
    	pointList.append(Arrays.asList(new QPair<>(5,6)));
    	pointList.prepend(new QPair<>(0,0));
    	assertEquals(new QPair<>(5,6), pointList.at(3));
    	assertTrue(pointList.contains(new QPair<>(3,4)));
    	assertEquals(4, pointList.size());
    	pointList.append(new QPair<>(1,2));
    	assertEquals(2, pointList.count(new QPair<>(1,2)));
    	assertEquals(1, pointList.indexOf(new QPair<>(1,2)));
    	assertEquals(4, pointList.lastIndexOf(new QPair<>(1,2)));
    	assertTrue(pointList.endsWith(new QPair<>(1,2)));
    	assertFalse(pointList.startsWith(new QPair<>(1,2)));
    	pointList.swapItemsAt(0, 1);
    	assertTrue(pointList.startsWith(new QPair<>(1,2)));
    	assertFalse(pointList.equals(new QList<>(pairType)));
    	assertEquals(new QPair<>(100,200), pointList.value(100, new QPair<>(100,200)));
    	pointList.removeAt(1);
    	assertEquals(new QPair<>(5,6), pointList.takeAt(2));
    	pointList.append(new QPair<>(3,4));
    	assertFalse(pointList.removeOne(new QPair<>(5,6)));
    	assertEquals(2, pointList.removeAll(new QPair<>(3,4)));
    	pointList.replace(0, new QPair<>(10,20));
    	assertEquals(1, pointList.count(new QPair<>(1,2)));
    	assertEquals(1, pointList.count(new QPair<>(10,20)));
    	pointList.prepend(new QPair<>(0,0));
    	pointList.move(0, 2);
    	assertTrue(pointList.endsWith(new QPair<>(0,0)));
    	pointList.insert(1, new QPair<>(30,30));
    	QList<QPair<Integer,Integer>> midList = pointList.mid(1);
    	assertEquals(3, midList.size());
    	assertTrue(midList.startsWith(new QPair<>(30,30)));
    	int counter = 0;
    	for(@SuppressWarnings("unused") QPair<Integer,Integer> p : midList) {
    		++counter;
    	}
    	assertEquals(3, counter);
    	midList.clear();
    	assertEquals(0, midList.size());
    	pointList = new QList<>(pairType);
    	pointList.add(0, new QPair<>(5,6));
    	assertEquals(new QPair<>(5,6), pointList.at(0));
    	assertEquals(1, pointList.size());
    }
    
    @Test
    public void test_create_QStack_QPoint() {
    	QStack<QPoint> pointList = new QStack<>(QPoint.class);
    	pointList.reserve(20);
    	pointList.append(new QPoint(1,2));
    	pointList.append(new QPoint(3,4));
    	pointList.append(Arrays.asList(new QPoint(5,6)));
    	pointList.prepend(new QPoint(0,0));
    	assertEquals(new QPoint(5,6), pointList.at(3));
    	assertTrue(pointList.contains(new QPoint(3,4)));
    	assertEquals(4, pointList.size());
    	pointList.append(new QPoint(1,2));
    	assertEquals(2, pointList.count(new QPoint(1,2)));
    	assertEquals(1, pointList.indexOf(new QPoint(1,2)));
    	assertEquals(4, pointList.lastIndexOf(new QPoint(1,2)));
    	assertTrue(pointList.endsWith(new QPoint(1,2)));
    	assertFalse(pointList.startsWith(new QPoint(1,2)));
    	pointList.swapItemsAt(0, 1);
    	assertTrue(pointList.startsWith(new QPoint(1,2)));
    	assertFalse(pointList.equals(new QStack<>(QPoint.class)));
    	assertEquals(new QPoint(100,200), pointList.value(100, new QPoint(100,200)));
    	pointList.removeAt(1);
    	assertEquals(new QPoint(5,6), pointList.takeAt(2));
    	pointList.append(new QPoint(3,4));
    	assertFalse(pointList.removeOne(new QPoint(5,6)));
    	assertEquals(2, pointList.removeAll(new QPoint(3,4)));
    	pointList.replace(0, new QPoint(10,20));
    	assertEquals(1, pointList.count(new QPoint(1,2)));
    	assertEquals(1, pointList.count(new QPoint(10,20)));
    	pointList.prepend(new QPoint(0,0));
    	pointList.move(0, 2);
    	assertTrue(pointList.endsWith(new QPoint(0,0)));
    	pointList.insert(1, new QPoint(30,30));
    }
    
    @Test
    public void test_create_QSet_QPoint() {
    	QMetaType typePre = QMetaType.fromName("QSet<QPointF>");
    	assertFalse(typePre.isValid());
    	QSet<QPointF> pointSet = new QSet<>(QPointF.class);
    	assertEquals(0, pointSet.capacity());
    	pointSet.reserve(20);
    	QMetaType typeAfter = QMetaType.fromName("QSet<QPointF>");
    	assertTrue(typeAfter.isValid());
    	pointSet.insert(new QPointF(1,2));
    	pointSet.insert(new QPointF(1,2));
    	pointSet.insert(new QPointF(3,4));
    	assertEquals(2, pointSet.size());
    	assertFalse(pointSet.equals(new QSet<>(QPointF.class)));
    	assertEquals(pointSet, Arrays.asList(new QPointF(1,2), new QPointF(3,4)));
    	assertEquals(pointSet, Arrays.asList(new QPointF(3,4), new QPointF(1,2)));
    	assertEquals(pointSet, Arrays.asList(new QPointF(3,4), new QPointF(1,2), new QPointF(1,2)));
    	pointSet.remove(new QPointF(3,4));
    	assertEquals(1, pointSet.size());
    	pointSet.insert(new QPointF(5,6));
    	assertTrue(pointSet.intersects(Arrays.asList(new QPointF(0,0), new QPointF(1,2), new QPointF(3,4))));
    	pointSet.intersect(Arrays.asList(new QPointF(0,0), new QPointF(1,2), new QPointF(3,4)));
    	assertEquals(1, pointSet.size());
    	pointSet.unite(Arrays.asList(new QPointF(0,0), new QPointF(1,2), new QPointF(3,4)));
    	assertEquals(3, pointSet.size());
    	int counter = 0;
    	for(@SuppressWarnings("unused") QPointF p : pointSet) {
    		++counter;
    	}
    	assertEquals(3, counter);
    	pointSet.subtract(Arrays.asList(new QPointF(1,2), new QPointF(7,8), new QPointF(9,0)));
    	assertEquals(2, pointSet.size());
    	assertEquals(pointSet, Arrays.asList(new QPointF(3,4), new QPointF(0,0)));
    	pointSet.remove(new QPointF(0,0));
    	QList<QPointF> values = pointSet.values();
    	assertEquals(values, Arrays.asList(new QPointF(3,4)));
    	pointSet.clear();
    	assertEquals(0, pointSet.size());
    	pointSet.dispose();
    }
    
    @Test
    public void test_create_SetList() {
    	int setId = QMetaType.qRegisterMetaType(QSet.class, QMetaType.fromType(String.class));
    	QList<QSet<String>> setList = new QList<>(new QMetaType(setId));
    	QSet<String> set0 = new QSet<>(String.class);
    	set0.insert("A");
    	set0.insert("B");
    	set0.insert("C");
    	setList.append(set0);
    	QSet<String> set1 = new QSet<>(String.class);
    	set1.insert("D");
    	set1.insert("E");
    	set1.insert("F");
    	setList.append(set1);
    	assertEquals(set0, ContainerTest.valueAt(setList, 0));
    	assertEquals(set1, ContainerTest.valueAt(setList, 1));
    }
    
    @SuppressWarnings({ "unchecked", "rawtypes" })
	@Test
    public void testContainerTypeMismatch() {
		QList container = new QList<>(QRunnable.class);
    	try {
			container.append(new QWidget());
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
    	try {
			container.append(new Object());
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
    	try {
			container.append("ABC");
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
    	container.append(QRunnable.of(()->{}));
    	container.append((QRunnable)()->{});
    	
    	container = createListOfRunnables();
    	try {
			container.append(new QWidget());
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
    	try {
			container.append(new Object());
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
    	try {
			container.append("ABC");
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
    	container.append(QRunnable.of(()->{}));
    	container.append((QRunnable)()->{});
    	
    	container = createListOfEasingFunctions();
    	try {
			container.append(new QWidget());
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
    	try {
			container.append(new Object());
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
    	try {
			container.append("ABC");
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
    	try {
	    	container.append(QRunnable.of(()->{}));
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
    	container.append((QEasingCurve.EasingFunction)d->d);
    	
    	container = createListOfListOfRunnables();
    	try {
			container.append(new QWidget());
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
    	try {
			container.append(new Object());
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
    	try {
			container.append("ABC");
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
    	try {
	    	container.append((QRunnable)()->{});
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
    	container.add(new ArrayList<>());
    	container.add(Arrays.asList(QRunnable.of(()->{})));
    	container.addAll(new ArrayList<>());
    	try {
	    	container.addAll(Arrays.asList(1));
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
    	
    	container = new QList<>(QObject.class);
		container.append(new QWidget());
    	try {
			container.append(new Object());
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
    	try {
			container.append("ABC");
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
    	try {
    		container.append(QRunnable.of(()->{}));
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
    	try {
    		QObject object = new QObject();
    		object.dispose();
    		container.append(object);
			assertTrue("QNoNativeResourcesException expected to be thrown", false);
		} catch (QNoNativeResourcesException e) {
		}
    	
    	container = new QList<>(QWidget.class);
		container.append(new QWidget());
		container.append(new QCheckBox());
    	try {
    		QCheckBox object = new QCheckBox();
    		object.dispose();
    		container.append(object);
			assertTrue("QNoNativeResourcesException expected to be thrown", false);
		} catch (QNoNativeResourcesException e) {
		}
		try {
			container.append(new QObject());
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
		try {
			container.append(new Object());
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
    	try {
			container.append("ABC");
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
    	try {
        	container.append(QRunnable.of(()->{}));
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
    	
    	container = new QList<>(int.class);
    	try {
			container.append(new QWidget());
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
    	try {
			container.append(new Object());
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
    	try {
			container.append("ABC");
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
    	try {
			container.append("2");
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
    	try {
    		container.append(QRunnable.of(()->{}));
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
    	container.append(5);
    	
    	container = new QStringList();
    	try {
			container.append(new QWidget());
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
    	try {
			container.append(new Object());
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
    	try {
        	container.append(QRunnable.of(()->{}));
			fail("IllegalArgumentException expected to be thrown");
		} catch (IllegalArgumentException e) {
		}
		container.append("ABC");
    }
    
    @Test
    public void testVariantListOverwrite() {
		ContainerTest.toQVariantList(new ArrayList<>());
		QMetaType easingFunction = QMetaType.fromType(QEasingCurve.EasingFunction.class);
		QMetaType listType = QMetaType.fromType(QList.class, easingFunction);
		assertFalse("QVariantList".equals(""+listType.name()));
	}
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestContainers.class.getName());
    }
}
