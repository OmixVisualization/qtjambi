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
import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import java.util.List;
import java.util.MissingFormatArgumentException;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.Test;

import io.qt.autotests.generated.FutureHandler;
import io.qt.concurrent.QtConcurrent;
import io.qt.core.QFuture;
import io.qt.core.QFutureInterface;
import io.qt.core.QFutureSynchronizer;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QThread;

public class TestConcurrent extends ApplicationInitializer {

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

        QFuture<Void> future = QtConcurrent.map(strings, object -> { object.value += " foobar"; } );

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

        QFuture<String> future = QtConcurrent.mappedReduced(
        		ints, 
        		i->i*i,
        		(String r, Integer n) -> {
        			Integer i = (r==null || r.isEmpty() ? 0 : Integer.parseInt(r)) + n;
		            return i.toString();
		        }
        );
        assertEquals(COUNT, ints.size());

        int n=0;
        for (int i=0; i<COUNT;++i)
            n += i*i;

        future.waitForFinished();
        assertEquals(1, future.resultCount());
        assertEquals(n, Integer.parseInt(future.result()));
    }

    @Test
    public void testBlockingMappedReduced() {
        List<Integer> ints = new ArrayList<Integer>();
        for (int i=0; i<COUNT; ++i)
            ints.add(i);

        String result = QtConcurrent.blockingMappedReduced(ints,
        		i->i*i,
        		(String r, Integer n) -> {
                    Integer i = (r==null || r.isEmpty() ? 0 : Integer.parseInt(r)) + n;
                    return i.toString();
                }
        );
        assertEquals(COUNT, ints.size());

        int n=0;
        for (int i=0; i<COUNT;++i)
            n += i*i;

        assertEquals(n, Integer.parseInt(result));
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

        QFuture<Void> future = QtConcurrent.filter(ints, i->i >= COUNT);
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

        QFuture<Integer> future = QtConcurrent.filteredReduced(ints,
        		i->i >= COUNT,
        		(r, intermediate)->{
        			if(intermediate==null)
        				intermediate = 0;
        			if(r==null)
        				r = 0;
        			return r + intermediate;
        		}
        );

        future.waitForFinished();
        assertEquals(COUNT*2, ints.size());
        assertEquals(1, future.resultCount());

        int n=0;
        for (int i=COUNT; i<COUNT*2; ++i)
            n += i;

        assertTrue(future.result()!=null);
        assertEquals(n, future.result().intValue());

    }

    @Test
    public void testBlockingFilteredReduced() {
        List<Integer> ints = new ArrayList<Integer>();
        for (int i=0; i<COUNT*2; ++i)
            ints.add(i);

        Integer result = QtConcurrent.blockingFilteredReduced(ints,
        		i->i >= COUNT,
        		(r, intermediate)->{
        			if(intermediate==null)
        				intermediate = 0;
        			if(r==null)
        				r = 0;
        			return r + intermediate;
    			}
        );
        assertEquals(COUNT*2, ints.size());

        int n=0;
        for (int i=COUNT; i<COUNT*2; ++i)
            n += i;

        assertTrue(result!=null);
        assertEquals(n, result.intValue());
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
        QFuture<Void> v = QtConcurrent.run(()->this.method(i));

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
        QFuture<Void> future = QtConcurrent.run(()->this.method4(i, 123, 321));

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
    	QFutureSynchronizer<Integer> synchronizer = new QFutureSynchronizer<>();
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
    
    @Test
    public void testJavaQFuture() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	QFutureInterface<String> promise = new QFutureInterface<>();
		promise.reportStarted();
    	QtConcurrent.run(()->{
    		try{
	    		QThread.msleep(200);
	    		promise.reportResult("testJavaQFuture1");
	    		promise.reportResult("testJavaQFuture_nested");
    		}catch(Throwable e) {
    			promise.reportException(e);
    		}finally {
    			promise.reportFinished();
    		}
    	});
    	List<String> result = FutureHandler.returnInTheFuture(promise.future());
    	Assert.assertEquals(2, result.size());
    	Assert.assertEquals("testJavaQFuture1", result.get(0));
    	Assert.assertEquals("testJavaQFuture_nested", result.get(1));
    }
    
    @Test
    public void testJavaQFuture_nested() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	QFutureInterface<String> promise = new QFutureInterface<>();
		promise.reportStarted();
    	QtConcurrent.run(()->{
    		try{
	    		QThread.msleep(200);
	    		promise.reportResult("testJavaQFuture1");
	    		promise.reportResult("testJavaQFuture_nested");
    		}catch(Throwable e) {
    			promise.reportException(e);
    		}finally {
    			promise.reportFinished();
    		}
    	});
    	List<String> result = FutureHandler.returnInTheFuture(FutureHandler.forward(promise.future()));
    	Assert.assertEquals(2, result.size());
    	Assert.assertEquals("testJavaQFuture1", result.get(0));
    	Assert.assertEquals("testJavaQFuture_nested", result.get(1));
    }
    
    @Test
    public void testExceptionJavaQFuture() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	QFutureInterface<String> promise = new QFutureInterface<>();
		promise.reportStarted();
    	QtConcurrent.run(()->{
    		try {
	    		QThread.msleep(200);
	    		throw new UnsupportedOperationException();
    		}catch(Throwable e) {
    			promise.reportException(e);
    		}finally {
    			promise.reportFinished();
    		}
    	});
    	try {
	    	FutureHandler.throwInTheFuture(promise.future());
	    	Assert.assertFalse("exception expected to be thrown", true);
		} catch (UnsupportedOperationException e) {
		}
    }
    
    @Test
    public void testExceptionJavaQFuture_nested() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	QFutureInterface<String> promise = new QFutureInterface<>();
		promise.reportStarted();
    	QtConcurrent.run(()->{
    		try {
	    		QThread.msleep(200);
	    		throw new MissingFormatArgumentException("");
    		}catch(Throwable e) {
    			promise.reportException(e);
    		}finally {
    			promise.reportFinished();
    		}
    	});
    	try {
	    	FutureHandler.throwInTheFuture(FutureHandler.forward(promise.future()));
	    	Assert.assertFalse("exception expected to be thrown", true);
		} catch (MissingFormatArgumentException e) {
		}
    }
    
    @Test
    public void testCanceledJavaQFuture() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	QFutureInterface<String> promise = new QFutureInterface<>();
		promise.reportStarted();
		boolean[] isCanceled = {false};
    	QtConcurrent.run(()->{
    		try {
    			int i=0;
        		while(!promise.isCanceled() && (i++<30))
        			QThread.msleep(200);
        		isCanceled[0] = promise.isCanceled();
    		}catch(Throwable e) {
    			promise.reportException(e);
    		}finally {
    			promise.reportFinished();
    		}
    	});
    	FutureHandler.cancelInTheFuture(promise.future());
    	Assert.assertTrue("FutureHandler.cancelInTheFuture() did not cancel.", isCanceled[0]);
    }
    
    @Test
    public void testCanceledJavaQFuture_nested() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	QFutureInterface<String> promise = new QFutureInterface<>();
		promise.reportStarted();
		boolean[] isCanceled = {false};
    	QtConcurrent.run(()->{
    		try {
    			int i=0;
        		while(!promise.isCanceled() && (i++<30))
        			QThread.msleep(200);
        		isCanceled[0] = promise.isCanceled();
    		}catch(Throwable e) {
    			promise.reportException(e);
    		}finally {
    			promise.reportFinished();
    		}
    	});
    	FutureHandler.cancelInTheFuture(FutureHandler.forward(promise.future()));
    	Assert.assertTrue("FutureHandler.cancelInTheFuture() after forwarding did not cancel.", isCanceled[0]);
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestConcurrent.class.getName());
    }
}
