/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import java.util.List;

import org.junit.Test;

import io.qt.concurrent.QtConcurrent;
import io.qt.core.QFuture;
import io.qt.core.QFutureSynchronizer;
import io.qt.core.QVoidFuture;

public class TestConcurrent extends QApplicationTest {

    static final int COUNT = 100;

    static class MutableString {
        public String value;

        public MutableString(String value) { this.value = value; }

        @Override
        public boolean equals(Object other) {
            if (other instanceof MutableString)
                return ((MutableString)other).value.equals(value);
            else if (other instanceof String)
                return value.equals((String) other);
            else
                return super.equals(value);
        }
    }

    @Test
    public void testMap() {
        List<MutableString> strings = new ArrayList<MutableString>();

        for (int i=0; i<COUNT; ++i)
            strings.add(new MutableString("" + i));

        QVoidFuture future = QtConcurrent.map(strings, object -> { object.value += " foobar"; } );

        future.waitForFinished();
        assertEquals(COUNT, strings.size());
        for (int i=0; i<COUNT; ++i)
            assertEquals("" + i + " foobar", strings.get(i).value);
    }

    @Test
    public void testBlockingMap() {
        List<MutableString> strings = new ArrayList<MutableString>();

        for (int i=0; i<COUNT; ++i)
            strings.add(new MutableString("" + i));

        QtConcurrent.blockingMap(strings, object -> { object.value += " foobar"; } );

        assertEquals(COUNT, strings.size());
        for (int i=0; i<COUNT; ++i)
            assertEquals("" + i + " foobar", strings.get(i).value);
    }

    @Test
    public void testMapped() {
        List<String> strings = new ArrayList<String>();
        for (int i=0; i<COUNT; ++i)
            strings.add("" + (i*i));

        QFuture<Integer> future = QtConcurrent.mapped(strings, Integer::parseInt);

        future.waitForFinished();
        List<Integer> results = future.results();
        assertEquals(COUNT, strings.size());
        assertEquals(COUNT, results.size());

        for (int i=0; i<results.size(); ++i)
            assertEquals(i*i, (int) results.get(i));
    }

    @Test
    public void testBlockingMapped() {
        List<String> strings = new ArrayList<String>();
        for (int i=0; i<COUNT; ++i)
            strings.add("" + (i*i));

        List<Integer> results = QtConcurrent.blockingMapped(strings, Integer::parseInt);

        assertEquals(COUNT, strings.size());
        assertEquals(COUNT, results.size());
        for (int i=0; i<results.size(); ++i)
            assertEquals(i*i, (int) results.get(i));
    }

    @Test
    public void testMappedReduced() {
        List<Integer> ints = new ArrayList<Integer>();
        for (int i=0; i<COUNT; ++i)
            ints.add(i);

        QFuture<MutableString> future = QtConcurrent.mappedReduced(ints, i->((Integer)(i*i)).toString(),
                new QtConcurrent.ReducedFunctor<MutableString, String>() {
                    public MutableString defaultResult() {
                        return new MutableString("5");
                    }

                    public void reduce(MutableString result, String n) {
                        int i = Integer.parseInt(result.value) + Integer.parseInt(n);
                        result.value = Integer.valueOf(i).toString();
                    }
                }
        );
        assertEquals(COUNT, ints.size());

        int n=5;
        for (int i=0; i<COUNT;++i)
            n += i*i;

        future.waitForFinished();
        assertEquals(1, future.resultCount());
        assertEquals(n, Integer.parseInt(future.result().value));
    }

    @Test
    public void testBlockingMappedReduced() {
        List<Integer> ints = new ArrayList<Integer>();
        for (int i=0; i<COUNT; ++i)
            ints.add(i);

        MutableString result = QtConcurrent.blockingMappedReduced(ints,
        		i->((Integer)(i*i)).toString(),
                new QtConcurrent.ReducedFunctor<MutableString, String>() {
                    public MutableString defaultResult() {
                        return new MutableString("5");
                    }

                    public void reduce(MutableString result, String n) {
                        int i = Integer.parseInt(result.value) + Integer.parseInt(n);
                        result.value = Integer.valueOf(i).toString();
                    }
                }
        );
        assertEquals(COUNT, ints.size());

        int n=5;
        for (int i=0; i<COUNT;++i)
            n += i*i;

        assertEquals(n, Integer.parseInt(result.value));
    }

    @Test
    public void testFiltered() {
        List<Integer> ints = new ArrayList<Integer>();
        for (int i=0; i<COUNT*2; ++i)
            ints.add(i);

        QFuture<Integer> future = QtConcurrent.filtered(ints, i->i >= COUNT);

        future.waitForFinished();
        assertEquals(COUNT*2, ints.size());
        assertEquals(COUNT, future.resultCount());

        List<Integer> lst = future.results();
        for (int i=0; i<future.resultCount(); ++i)
            assertEquals(i+COUNT, (int) lst.get(i));
    }

    @Test
    public void testBlockingFiltered() {
        List<Integer> ints = new ArrayList<Integer>();
        for (int i=0; i<COUNT*2; ++i)
            ints.add(i);

        List<Integer> lst = QtConcurrent.blockingFiltered(ints, i->i >= COUNT);

        assertEquals(COUNT*2, ints.size());
        assertEquals(COUNT, lst.size());
        for (int i=0; i<lst.size(); ++i)
            assertEquals(i+COUNT, (int) lst.get(i));
    }
    
    @Test
    public void testBlockingFilter() {
        List<Integer> ints = new ArrayList<Integer>();
        for (int i=0; i<COUNT*2; ++i)
            ints.add(i);

        QtConcurrent.blockingFilter(ints, i->i >= COUNT);

        assertEquals(COUNT, ints.size());
        for (int i=0; i<ints.size(); ++i)
            assertEquals(i+COUNT, (int) ints.get(i));
    }
    
    @Test
    public void testFilter() {
        List<Integer> ints = new ArrayList<Integer>();
        for (int i=0; i<COUNT*2; ++i)
            ints.add(i);

        QVoidFuture future = QtConcurrent.filter(ints, i->i >= COUNT);
        future.waitForFinished();

        assertEquals(COUNT, ints.size());
        for (int i=0; i<ints.size(); ++i)
            assertEquals(i+COUNT, (int) ints.get(i));
    }


    static class MutableInteger {
        public int value;

        public MutableInteger(int value) { this.value = value; }
    }

    @Test
    public void testFilteredReduced() {
        List<Integer> ints = new ArrayList<Integer>();
        for (int i=0; i<COUNT*2; ++i)
            ints.add(i);

        QFuture<MutableInteger> future = QtConcurrent.filteredReduced(ints,
        		i->i >= COUNT,
                new QtConcurrent.ReducedFunctor<MutableInteger, Integer>() {
                    public MutableInteger defaultResult() {
                        return new MutableInteger(3);
                    }

                    public void reduce(MutableInteger result, Integer intermediate) {
                        result.value += intermediate;
                    }
                }
        );

        future.waitForFinished();
        assertEquals(COUNT*2, ints.size());
        assertEquals(1, future.resultCount());

        int n=3;
        for (int i=COUNT; i<COUNT*2; ++i)
            n += i;

        assertTrue(future.result()!=null);
        assertEquals(n, future.result().value);

    }

    @Test
    public void testBlockingFilteredReduced() {
        List<Integer> ints = new ArrayList<Integer>();
        for (int i=0; i<COUNT*2; ++i)
            ints.add(i);

        MutableInteger result = QtConcurrent.blockingFilteredReduced(ints,
        		i->i >= COUNT,
                new QtConcurrent.ReducedFunctor<MutableInteger, Integer>() {
                    public MutableInteger defaultResult() {
                        return new MutableInteger(3);
                    }

                    public void reduce(MutableInteger result, Integer intermediate) {
                        result.value += intermediate;
                    }
                }

        );
        assertEquals(COUNT*2, ints.size());

        int n=3;
        for (int i=COUNT; i<COUNT*2; ++i)
            n += i;

        assertTrue(result!=null);
        assertEquals(n, result.value);
    }

    private void method(MutableInteger i) {
        i.value += 123;
    }

    private Integer method2(Integer i) {
        return i + 1234;
    }

    @Test
    public void testRunVoid() {
        MutableInteger i = new MutableInteger(321);
        QVoidFuture v = QtConcurrent.run(()->this.method(i));

        v.waitForFinished();
        assertEquals(444, i.value);
    }

    @Test
    public void testRun() {
        QFuture<Integer> future = QtConcurrent.run(()->this.method2(4321));

        future.waitForFinished();
        assertEquals(5555, (int) future.result());
    }

    private int method3(int a, byte b, short c, float d) {
        return (int) (a + b + c + d);
    }

    @Test
    public void testRunWithPrimitiveTypes() {
        // FIXME: -Xcheck:jni this causes a JVM crash, due to incorrect types
        // I guess because they are all treated as Integer.TYPE which is a mismatch for the method.
        // [junit] FATAL ERROR in native method: Wrong object class or methodID passed to JNI call
        // [junit]     at io.qt.core.QtConcurrent.runPrivate(Native Method)
        // [junit]     at io.qt.core.QtConcurrent.run(QtConcurrent.java:276)
        // [junit]     at io.qt.autotests.TestConcurrent.testRunWithPrimitiveTypes(TestConcurrent.java:400)
        ///QFuture<Integer> future = QtConcurrent.run(this, m, 1, 2, 3, 4);
        // so to fix we use:
        QFuture<Integer> future = QtConcurrent.run(()->this.method3(1, (byte)2, (short)3, 4f));
        // However we should implement and write a test case to make InvalidArgumentException occur.

        future.waitForFinished();
        assertEquals(10, (int) future.result());
    }

    private void method4(MutableInteger a, int b, int c) {
        a.value = b + c;
    }

    @Test
    public void testRunVoidWithPrimitiveTypes() {
        MutableInteger i = new MutableInteger(0);
        QVoidFuture future = QtConcurrent.run(()->this.method4(i, 123, 321));

        future.waitForFinished();
        assertEquals(444, i.value);
    }

    @Test
    public void testResultAt_mapped() {
        List<Integer> ints = new ArrayList<Integer>();
        for (int i=0; i<COUNT*2; ++i)
            ints.add(i);

        {
            QFuture<Integer> result = QtConcurrent.mapped(ints, i->i+1);

            result.waitForFinished();
            assertEquals(COUNT*2, ints.size());
            assertEquals(COUNT*2, result.resultCount());
            for (int i=0; i<result.resultCount(); ++i)
                assertEquals(i+1, (int) result.resultAt(i));
        }
    }

    @Test
    public void testResultAt_filtered() {
        List<Integer> ints = new ArrayList<Integer>();
        for (int i=0; i<COUNT*2; ++i)
            ints.add(i);
        {
            QFuture<Integer> future = QtConcurrent.filtered(ints, i->i >= COUNT );

            future.waitForFinished();
            assertEquals(COUNT*2, ints.size());
            assertEquals(COUNT, future.resultCount());
            for (int i=0; i<future.resultCount(); ++i)
                assertEquals(i+COUNT, (int) future.resultAt(i));
        }

    }
    
    @Test
    public void testFutureSynchronizer() {
    	List<Integer> ints = new ArrayList<Integer>();
        for (int i=0; i<COUNT*2; ++i)
            ints.add(i);
    	QFutureSynchronizer<QFuture<Integer>> synchronizer = new QFutureSynchronizer<>();
        synchronizer.addFuture(QtConcurrent.mapped(ints, i->i+1));
        synchronizer.addFuture(QtConcurrent.filtered(ints, i->i >= COUNT));
    	List<QFuture<Integer>> futures = synchronizer.futures();
    	assertEquals(2, futures.size());
    	synchronizer.waitForFinished();
    	{
    		QFuture<Integer> future = futures.get(0);
    		assertEquals(COUNT*2, future.resultCount());
            for (int i=0; i<future.resultCount(); ++i)
                assertEquals(i+1, (int) future.resultAt(i));
    	}
    	{
    		QFuture<Integer> future = futures.get(1);
    		assertEquals(COUNT, future.resultCount());
            for (int i=0; i<future.resultCount(); ++i)
                assertEquals(i+COUNT, (int) future.resultAt(i));
    	}
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestConcurrent.class.getName());
    }
}
