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
import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import io.qt.QNativePointer;
import io.qt.QNoNativeResourcesException;
import io.qt.autotests.generated.QVector_int;
import io.qt.autotests.generated.Tulip;
import io.qt.core.QLinkedList;
import io.qt.core.QList;
import io.qt.core.QMetaType;
import io.qt.core.QPoint;
import io.qt.core.QRunnable;
import io.qt.core.QSet;
import io.qt.core.QVector;
import io.qt.widgets.QGraphicsItem;

public class TestContainersQt5 extends ApplicationInitializer {
	
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
    public void run_testLinkedLists() {
        LinkedList<Integer> l = new LinkedList<Integer>();
        for (int i = 0; i < 10; ++i)
            l.add(i);

        QLinkedList<Integer> l2 = tulip.do_QLinkedList_of_int(l);
        assertTrue(l2 != null);
        assertEquals(l2, l);
    }
    
    @Test
    public void run_testVectors() {
        List<Integer> l = new ArrayList<Integer>();
        for (int i = 0; i < 10; ++i)
            l.add(i);

        QVector<Integer> l2 = tulip.do_QVector_of_int(l);
        assertTrue(l2 != null);
        assertEquals(l, l2);
    }
    
    @Test
    public void run_testNativeQVector() {
    	QVector<Double> listRef = tulip.vectorRef();
    	listRef.append(Arrays.asList(3., 5.5, 2.8));
    	assertEquals(Arrays.asList(3., 5.5, 2.8), tulip.constVector());
    	assertEquals(Double.valueOf(3.), tulip.constVector().at(0));
    	listRef.clear();
    	assertEquals(0, tulip.constVector().count());
    	listRef.append(Arrays.asList(3., 5.5, 2.8));
    	listRef.append(3.);
    	assertEquals(2, tulip.constVector().count(3.));
    	listRef.fill(6., 10);
    	assertEquals(10, tulip.constVector().count(6.));
    	listRef.prepend(90.);
    	listRef.move(4, 0);
    	listRef.takeLast();
    	listRef.startsWith(5.);
    	listRef.resize(40);
    	listRef.dispose();
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
        got = false; try { vector.insert(vector.size()+1, 0); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
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
    

    
    @Test
    public void test_create_QLinkedList_QPoint() {
    	int typePrev = QMetaType.type("QLinkedList<QPoint>");
    	assertEquals(0, typePrev);
    	QLinkedList<QPoint> pointList = new QLinkedList<>(QPoint.class);
    	int typeAfter = QMetaType.type("QLinkedList<QPoint>");
    	assertTrue(0!=typeAfter);
    	pointList.append(new QPoint(1,2));
    	pointList.append(new QPoint(3,4));
    	pointList.append(new QPoint(5,6));
    	pointList.prepend(new QPoint(0,0));
    	assertEquals(new QPoint(0,0), pointList.first());
    	assertEquals(new QPoint(5,6), pointList.last());
    	assertTrue(pointList.contains(new QPoint(3,4)));
    	assertEquals(4, pointList.size());
    	pointList.append(new QPoint(1,2));
    	assertEquals(2, pointList.count(new QPoint(1,2)));
    	assertTrue(pointList.endsWith(new QPoint(1,2)));
    	assertFalse(pointList.equals(new QLinkedList<>(QPoint.class)));
    	assertEquals(new QPoint(0,0), pointList.takeFirst());
    	int counter = 0;
    	for(@SuppressWarnings("unused") QPoint p : pointList) {
    		++counter;
    	}
    	assertEquals(4, counter);
    	pointList.removeFirst();
    	assertTrue(pointList.startsWith(new QPoint(3,4)));
    	assertFalse(pointList.removeOne(new QPoint(30,40)));
    	pointList.removeLast();
    	assertEquals(new QPoint(5,6), pointList.takeLast());
    	assertEquals(1, pointList.size());
    	pointList.clear();
    	assertEquals(0, pointList.size());
    	pointList.dispose();
    	try {
			pointList.clear();
			assertFalse(true);
		} catch (QNoNativeResourcesException e) {
		}
    }
    
    @Test
    public void test_create_QVector_QPoint() {
    	int typePrev = QMetaType.type("QVector<QPoint>");
    	assertEquals(0, typePrev);
    	QVector<QPoint> pointList = new QVector<>(QPoint.class);
    	pointList.reserve(20);
    	int typeAfter = QMetaType.type("QVector<QPoint>");
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
    	assertFalse(pointList.equals(new QList<>(QPoint.class)));
    	assertEquals(new QPoint(100,200), pointList.value(100, new QPoint(100,200)));
    	QSet<QPoint> set = pointList.toSet();
    	assertEquals(4, set.size());
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
    	QVector<QPoint> midList = pointList.mid(1);
    	assertEquals(3, midList.size());
    	assertTrue(midList.startsWith(new QPoint(30,30)));
    	int counter = 0;
    	for(@SuppressWarnings("unused") QPoint p : midList) {
    		++counter;
    	}
    	assertEquals(3, counter);
    	midList.clear();
    	assertEquals(0, midList.size());
    }
    
    @Test
    public void test_QList_toSet() {
    	int typePrev = QMetaType.type("QList<QPoint>");
    	assertEquals(0, typePrev);
    	QList<QPoint> pointList = new QList<>(QPoint.class);
    	pointList.reserve(20);
    	int typeAfter = QMetaType.type("QList<QPoint>");
    	assertTrue(0!=typeAfter);
    	pointList.append(new QPoint(1,2));
    	pointList.append(new QPoint(3,4));
    	pointList.append(Arrays.asList(new QPoint(5,6)));
    	pointList.prepend(new QPoint(0,0));
    	QSet<QPoint> set = pointList.toSet();
    	assertEquals(4, set.size());
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestContainersQt5.class.getName());
    }
}
