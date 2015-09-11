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

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;

import org.junit.Test;

import org.qtjambi.qt.core.QFuture;
import org.qtjambi.qt.core.QFutureVoid;
import org.qtjambi.qt.concurrent.QtConcurrent;

public class TestConcurrent extends QApplicationTest {

    private static final int COUNT = 100;

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

        QFutureVoid future = QtConcurrent.map(strings,
                new QtConcurrent.MapFunctor<MutableString>() {

                    public void map(MutableString object) {
                        object.value += " foobar";
                    }
                }
        );

        future.waitForFinished();
        for (int i=0; i<COUNT; ++i)
            assertEquals("" + i + " foobar", strings.get(i).value);
    }

    @Test
    public void testBlockingMap() {
        List<MutableString> strings = new ArrayList<MutableString>();

        for (int i=0; i<COUNT; ++i)
            strings.add(new MutableString("" + i));

        QtConcurrent.blockingMap(strings,
                new QtConcurrent.MapFunctor<MutableString>() {

                    public void map(MutableString object) {
                        object.value += " foobar";
                    }
                }
        );

        for (int i=0; i<COUNT; ++i)
            assertEquals("" + i + " foobar", strings.get(i).value);
    }

    @Test
    public void testMapped() {
        List<String> strings = new ArrayList<String>();
        for (int i=0; i<COUNT; ++i)
            strings.add("" + (i*i));

        QFuture<Integer> future = QtConcurrent.mapped(strings,
                new QtConcurrent.MappedFunctor<Integer, String>() {
                    public Integer map(String object) {
                        return Integer.parseInt(object);
                    }
                }
        );

        future.waitForFinished();
        List<Integer> results = future.results();
        assertEquals(COUNT, results.size());

        for (int i=0; i<results.size(); ++i)
            assertEquals(i*i, (int) results.get(i));
    }

    @Test
    public void testBlockingMapped() {
        List<String> strings = new ArrayList<String>();
        for (int i=0; i<COUNT; ++i)
            strings.add("" + (i*i));

        List<Integer> results = QtConcurrent.blockingMapped(strings,
                new QtConcurrent.MappedFunctor<Integer, String>() {
                    public Integer map(String object) {
                        return Integer.parseInt(object);
                    }
                }
        );

        assertEquals(COUNT, results.size());
        for (int i=0; i<results.size(); ++i)
            assertEquals(i*i, (int) results.get(i));
    }

    @Test
    public void testMappedReduced() {
        List<Integer> strings = new ArrayList<Integer>();
        for (int i=0; i<COUNT; ++i)
            strings.add(i);

        QFuture<MutableString> future = QtConcurrent.mappedReduced(strings,
                new QtConcurrent.MappedFunctor<String, Integer>() {
                    public String map(Integer i) {
                        return ((Integer)(i*i)).toString();
                    }
                },
                new QtConcurrent.ReducedFunctor<MutableString, String>() {
                    public MutableString defaultResult() {
                        return new MutableString("5");
                    }

                    public void reduce(MutableString result, String n) {
                        int i = Integer.parseInt(result.value) + Integer.parseInt(n);
                        result.value = new Integer(i).toString();
                    }
                }
        );

        int n=5;
        for (int i=0; i<COUNT;++i)
            n += i*i;

        future.waitForFinished();
        assertEquals(1, future.resultCount());
        assertEquals(n, Integer.parseInt(future.result().value));
    }

    @Test
    public void testBlockingMappedReduced() {
        List<Integer> strings = new ArrayList<Integer>();
        for (int i=0; i<COUNT; ++i)
            strings.add(i);

        MutableString result = QtConcurrent.blockingMappedReduced(strings,
                new QtConcurrent.MappedFunctor<String, Integer>() {
                    public String map(Integer i) {
                        return ((Integer)(i*i)).toString();
                    }
                },
                new QtConcurrent.ReducedFunctor<MutableString, String>() {
                    public MutableString defaultResult() {
                        return new MutableString("5");
                    }

                    public void reduce(MutableString result, String n) {
                        int i = Integer.parseInt(result.value) + Integer.parseInt(n);
                        result.value = new Integer(i).toString();
                    }
                }
        );

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

        QFuture<Integer> future = QtConcurrent.filtered(ints,
                new QtConcurrent.FilteredFunctor<Integer>() {
                    public boolean filter(Integer i) {
                        return (i >= COUNT);
                    }
                }
        );

        future.waitForFinished();
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

        List<Integer> lst = QtConcurrent.blockingFiltered(ints,
                new QtConcurrent.FilteredFunctor<Integer>() {
                    public boolean filter(Integer i) {
                        return (i >= COUNT);
                    }
                }
        );

        assertEquals(COUNT, lst.size());
        for (int i=0; i<lst.size(); ++i)
            assertEquals(i+COUNT, (int) lst.get(i));
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
                new QtConcurrent.FilteredFunctor<Integer>() {
                    public boolean filter(Integer i) {
                        return (i >= COUNT);
                    }
                },
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
        assertEquals(1, future.resultCount());

        int n=3;
        for (int i=COUNT; i<COUNT*2; ++i)
            n += i;

        assertEquals(n, future.result().value);

    }

    @Test
    public void testBlockingFilteredReduced() {
        List<Integer> ints = new ArrayList<Integer>();
        for (int i=0; i<COUNT*2; ++i)
            ints.add(i);

        MutableInteger result = QtConcurrent.blockingFilteredReduced(ints,
                new QtConcurrent.FilteredFunctor<Integer>() {
                    public boolean filter(Integer i) {
                        return (i >= COUNT);
                    }
                },
                new QtConcurrent.ReducedFunctor<MutableInteger, Integer>() {
                    public MutableInteger defaultResult() {
                        return new MutableInteger(3);
                    }

                    public void reduce(MutableInteger result, Integer intermediate) {
                        result.value += intermediate;
                    }
                }

        );

        int n=3;
        for (int i=COUNT; i<COUNT*2; ++i)
            n += i;

        assertEquals(n, result.value);
    }

    public void method(MutableInteger i) {
        i.value += 123;
    }

    public Integer method2(Integer i) {
        return i + 1234;
    }

    @Test
    public void testRunVoid() {
        Method m = null;
        try {
            m = TestConcurrent.class.getMethod("method", MutableInteger.class);
        } catch (Exception e) {
            e.printStackTrace();
        }

        assertTrue(m != null);

        MutableInteger i = new MutableInteger(321);
        QFutureVoid v = QtConcurrent.runVoidMethod(this, m, i);

        v.waitForFinished();
        assertEquals(444, i.value);
    }

    @Test
    public void testRun() {
        Method m = null;
        try {
            m = TestConcurrent.class.getMethod("method2", Integer.class);
        } catch (Exception e) {
            e.printStackTrace();
        }

        assertTrue(m != null);

        QFuture<Integer> future = QtConcurrent.run(this, m, 4321);

        future.waitForFinished();
        assertEquals(5555, (int) future.result());
    }

    public int method3(int a, byte b, short c, float d) {
        return (int) (a + b + c + d);
    }

    @Test
    public void testRunWithPrimitiveTypes() {
        Method m = null;
        try {
            m = TestConcurrent.class.getMethod("method3", Integer.TYPE, Byte.TYPE, Short.TYPE, Float.TYPE);
        } catch (Exception e) {
            e.printStackTrace();
        }

        assertTrue(m != null);

        // FIXME: -Xcheck:jni this causes a JVM crash, due to incorrect types
        // I guess because they are all treated as Integer.TYPE which is a mismatch for the method.
        // [junit] FATAL ERROR in native method: Wrong object class or methodID passed to JNI call
        // [junit]     at org.qtjambi.qt.core.QtConcurrent.runPrivate(Native Method)
        // [junit]     at org.qtjambi.qt.core.QtConcurrent.run(QtConcurrent.java:276)
        // [junit]     at org.qtjambi.autotests.TestConcurrent.testRunWithPrimitiveTypes(TestConcurrent.java:400)
        ///QFuture<Integer> future = QtConcurrent.run(this, m, 1, 2, 3, 4);
        // so to fix we use:
        QFuture<Integer> future = QtConcurrent.run(this, m, 1, (byte)2, (short)3, 4f);
        // However we should implement and write a test case to make InvalidArgumentException occur.

        future.waitForFinished();
        assertEquals(10, (int) future.result());
    }

    public void method4(MutableInteger a, int b, int c) {
        a.value = b + c;
    }

    @Test
    public void testRunVoidWithPrimitiveTypes() {
        Method m = null;
        try {
            m = TestConcurrent.class.getMethod("method4", MutableInteger.class, Integer.TYPE, Integer.TYPE);
        } catch (Exception e) {
            e.printStackTrace();
        }

        assertTrue(m != null);

        MutableInteger i = new MutableInteger(0);
        QFutureVoid future = QtConcurrent.runVoidMethod(this, m, i, 123, 321);

        future.waitForFinished();
        assertEquals(444, i.value);
    }

    @Test
    public void testResultAt() {
        List<Integer> ints = new ArrayList<Integer>();
        for (int i=0; i<COUNT*2; ++i)
            ints.add(i);

        {
            QFuture<Integer> result = QtConcurrent.mapped(ints,
                    new QtConcurrent.MappedFunctor<Integer, Integer>() {
                        public Integer map(Integer i) {
                            return i + 1;
                        }
                    }
            );

            result.waitForFinished();
            assertEquals(COUNT*2, result.resultCount());
            for (int i=0; i<result.resultCount(); ++i)
                assertEquals(i+1, (int) result.resultAt(i));
        }

        {
            QFuture<Integer> future = QtConcurrent.filtered(ints,
                    new QtConcurrent.FilteredFunctor<Integer>() {
                        public boolean filter(Integer i) {
                            return (i >= COUNT);
                        }
                    }
            );

            future.waitForFinished();
            assertEquals(COUNT, future.resultCount());
            for (int i=0; i<future.resultCount(); ++i)
                assertEquals(i+COUNT, (int) future.resultAt(i));
        }

    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestConcurrent.class.getName());
    }
}
