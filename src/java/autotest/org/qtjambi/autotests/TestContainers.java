/****************************************************************************
 **
 ** Copyright (C) 1992-2009 Nokia. All rights reserved.
 **
 ** This file is part of Qt Jambi.
 **
 ** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ****************************************************************************/

package org.qtjambi.autotests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;
import java.util.Set;
import java.util.SortedMap;
import java.util.Stack;
import java.util.TreeMap;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import org.qtjambi.autotests.generated.QVector_int;
import org.qtjambi.autotests.generated.QList_int;

import org.qtjambi.autotests.generated.Tulip;

import org.qtjambi.qt.QPair;
import org.qtjambi.qt.core.QCoreApplication;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QLinearGradient;

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
        HashMap<String, String> h2 = tulip.do_QHash_of_strings(h);
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
        Stack<Integer> s = new Stack<Integer>();
        for (int i = 0; i < 10; ++i)
            s.push(i);
        Stack<Integer> s2 = tulip.do_QStack_of_int(s);
        assertTrue(s2 != null);
        assertEquals(s, s2);
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
    public void run_testQueue() {
        Queue<Integer> q = new LinkedList<Integer>();
        for (int i = 0; i < 10; ++i)
            q.add(i);
        Queue<Integer> q2 = tulip.do_QQueue_of_int(q);
        assertTrue(q2 != null);
        assertEquals(q, q2);
    }


    @Test
    public void run_testQVector_outofbounds() {
        QVector_int vector = new QVector_int();

        boolean got;

        got = false; try { vector.at(10); } catch (Exception e) { got = true; } assertTrue(got);
        got = false; try { vector.replace(23, 14); } catch (Exception e) { got =true; } assertTrue(got);
        got = false; try { vector.pop_back(); } catch (Exception e) { got =true; } assertTrue(got);
        got = false; try { vector.pop_front(); } catch (Exception e) { got =true; } assertTrue(got);
        got = false; try { vector.remove(23); } catch (Exception e) { got =true; } assertTrue(got);
        got = false; try { vector.remove(23, 14); } catch (Exception e) { got =true; } assertTrue(got);
    }

    @Test
    public void run_testQList_outofbounds() {
        QList_int list = new QList_int();

        boolean got;

        got = false; try { list.at(10); } catch (Exception e) { got = true; } assertTrue(got);
        got = false; try { list.replace(23, 14); } catch (Exception e) { got =true; } assertTrue(got);
        got = false; try { list.pop_back(); } catch (Exception e) { got =true; } assertTrue(got);
        got = false; try { list.pop_front(); } catch (Exception e) { got =true; } assertTrue(got);
        got = false; try { list.move(14, 15); } catch (Exception e) { got =true; } assertTrue(got);
        got = false; try { list.swap(15, 14); } catch (Exception e) { got =true; } assertTrue(got);
        got = false; try { list.removeAt(14); } catch (Exception e) { got =true; } assertTrue(got);
        got = false; try { list.takeAt(14); } catch (Exception e) { got =true; } assertTrue(got);
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestContainers.class.getName());
    }
}
