/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
import java.util.List;
import java.util.MissingFormatArgumentException;

import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.Assume;
import org.junit.Test;

import io.qt.QNoImplementationException;
import io.qt.autotests.generated.FutureHandler;
import io.qt.concurrent.QtConcurrent;
import io.qt.concurrent.QtConcurrent.QTypedPromiseTaskBuilder1Arg1;
import io.qt.concurrent.QtConcurrent.QTypedTaskBuilder1Arg1;
import io.qt.core.QFuture;
import io.qt.core.QFutureInterface;
import io.qt.core.QFutureSynchronizer;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QPromise;
import io.qt.core.QThread;
import io.qt.core.QThreadPool;

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
    }private static QThreadPool pool = new QThreadPool();
	
	@AfterClass
    public static void testDispose() throws Exception {
		if(pool.activeThreadCount()>0) {
			System.out.println("Waiting for threads to finish...");
			pool.waitForDone();
		}
		pool.dispose();
		pool = null;
		ApplicationInitializer.testDispose();
    }
	
	@Test
    public void testBlockingMappedWithThreadPool() {
        List<String> strings = new ArrayList<String>();
        for (int i=0; i<COUNT; ++i)
            strings.add("" + (i*i));

        List<Integer> results = QtConcurrent.blockingMapped(pool, strings, Integer::parseInt);

        assertEquals(COUNT, strings.size());
        assertEquals(COUNT, results.size());
        for (int i=0; i<results.size(); ++i)
            assertEquals(i*i, (int) results.get(i));
    }
	
	@Test
    public void testRunWithoutPromiseWithThreadPool() {
		int[] result = {0};
		QFuture<Void> future = QtConcurrent.run(pool, number->{
			QThread.msleep(200);
			result[0] = Integer.parseInt(number);
		}, "12340");
		future.waitForFinished();
        assertEquals(12340, result[0]);
	}
	
	@Test
    public void testRunWithPromiseWithThreadPool() {
		QFuture<Integer> future = QtConcurrent.run(pool, (QPromise<Integer> promise, String number)->{
			QThread.msleep(200);
			promise.addResult(Integer.parseInt(number));
			promise.addResult(Integer.parseInt(number.substring(1)));
			promise.addResult(Integer.parseInt(number.substring(2)));
			promise.addResult(Integer.parseInt(number.substring(3)));
		}, "12341");
		future.waitForFinished();
        assertEquals(4, future.resultCount());
        assertEquals(12341, (int) future.resultAt(0));
        assertEquals(2341, (int) future.resultAt(1));
        assertEquals(341, (int) future.resultAt(2));
        assertEquals(41, (int) future.resultAt(3));
        try {
			future.resultAt(4);
			assertFalse("IndexOutOfBoundsException expected", true);
		} catch (IndexOutOfBoundsException e) {
		}
	}
	
	@Test
    public void testTaskWithThreadPool() {
		QTypedTaskBuilder1Arg1<Integer, String> task = QtConcurrent.task((String number)->{
			QThread.msleep(200);
			return Integer.parseInt(number);
		}).withArguments("12342").onThreadPool(pool);
		QFuture<Integer> future = task.spawn();
        assertEquals(12342, (int) future.result());
	}
	
	@Test
    public void testTaskWithPromiseWithThreadPool() {
		QTypedPromiseTaskBuilder1Arg1<Integer, String> task = QtConcurrent.task((QPromise<Integer> promise, String number)->{
			QThread.msleep(200);
			promise.addResult(Integer.parseInt(number));
		}).withArguments("12343").onThreadPool(pool);
		QFuture<Integer> future = task.spawn();
        assertEquals(12343, (int) future.result());
	}
	
	@Test
    public void testRunWithoutPromise() {
		int[] result = {0};
		QFuture<Void> future = QtConcurrent.run(number->{
			QThread.msleep(200);
			result[0] = Integer.parseInt(number);
		}, "12344");
		future.waitForFinished();
        assertEquals(12344, result[0]);
	}
	
	@Test
    public void testRunWithPromise() {
		QFuture<Integer> future = QtConcurrent.run((QPromise<Integer> promise, String number)->{
			QThread.msleep(200);
			promise.addResult(Integer.parseInt(number));
			promise.addResult(Integer.parseInt(number.substring(1)));
			promise.addResult(Integer.parseInt(number.substring(2)));
			promise.addResult(Integer.parseInt(number.substring(3)));
		}, "12345");
		future.waitForFinished();
        assertEquals(4, future.resultCount());
        assertEquals(12345, (int) future.result());
        assertEquals(2345, (int) future.resultAt(1));
        assertEquals(345, (int) future.resultAt(2));
        assertEquals(45, (int) future.resultAt(3));
        try {
			future.resultAt(4);
			assertFalse("IndexOutOfBoundsException expected", true);
		} catch (IndexOutOfBoundsException e) {
		}
	}
	
	@Test
    public void testTask() {
		QTypedTaskBuilder1Arg1<Integer, String> task = QtConcurrent.task((String number)->{
			QThread.msleep(200);
			return Integer.parseInt(number);
		}).withArguments("12346");
		QFuture<Integer> future = task.spawn();
        assertEquals(12346, (int) future.result());
	}
	
	@Test
    public void testTaskWithPromise() {
		QTypedPromiseTaskBuilder1Arg1<Integer, String> task = QtConcurrent.task((QPromise<Integer> promise, String number)->{
			QThread.msleep(200);
			promise.addResult(Integer.parseInt(number));
			promise.addResult(Integer.parseInt(number.substring(1)));
			promise.addResult(Integer.parseInt(number.substring(2)));
			promise.addResult(Integer.parseInt(number.substring(3)));
		}).withArguments("12347");
		QFuture<Integer> future = task.spawn();
		future.waitForFinished();
        assertEquals(4, future.resultCount());
        assertEquals(12347, (int) future.result());
        assertEquals(2347, (int) future.resultAt(1));
        assertEquals(347, (int) future.resultAt(2));
        assertEquals(47, (int) future.resultAt(3));
        try {
			future.resultAt(4);
			assertFalse("IndexOutOfBoundsException expected", true);
		} catch (IndexOutOfBoundsException e) {
		}
	}
	
	@Test
    public void testNativePromiseToNativeCast() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
		QFuture<String> future = QtConcurrent.run(pool, (QPromise<String> promise)->{
			QThread.msleep(500);
			promise.addResult("Z");
			FutureHandler.fillString(promise);
			promise.addResult("E");
		});
		future.waitForFinished();
        assertEquals(6, future.resultCount());
        assertEquals("Z", future.result());
        assertEquals("A", future.resultAt(1));
        assertEquals("B", future.resultAt(2));
        assertEquals("C", future.resultAt(3));
        assertEquals("D", future.resultAt(4));
        assertEquals("E", future.resultAt(5));
	}
	
	@Test
    public void testJavaPromiseToNativeCast() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
		QPromise<String> promise = new QPromise<String>();
		QFuture<String> future = promise.future();
		promise.start();
		QtConcurrent.run(pool, ()->{
			try {
				QThread.msleep(500);
				promise.addResult("Z");
				FutureHandler.fillString(promise);
				promise.addResult("E");
			}finally {
				promise.finish();
			}
		});
		future.waitForFinished();
        assertEquals(6, future.resultCount());
        assertEquals("Z", future.result());
        assertEquals("A", future.resultAt(1));
        assertEquals("B", future.resultAt(2));
        assertEquals("C", future.resultAt(3));
        assertEquals("D", future.resultAt(4));
        assertEquals("E", future.resultAt(5));
	}
	
	@Test
	@SuppressWarnings({ "unchecked", "rawtypes" })
    public void testPromiseToNativeCast_mismatching_type() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
		{
			QPromise promise = QPromise.createVoidPromise();
			QFuture future = promise.future();
			promise.start();
			QtConcurrent.run(pool, ()->{
				try {
					QThread.msleep(500);
					promise.addResult("Z");
				}catch(Throwable t) {
					promise.setException(t);
				}finally {
					promise.finish();
				}
			});
			try{
				future.waitForFinished();
				assertFalse("Exception expected to be thrown.", true);
			}catch(QNoImplementationException e) {
				Assert.assertEquals("addResult(T,int) not available for QPromise<void>.", e.getMessage());
			}
		}
		
		{
			QPromise promise = QPromise.createVoidPromise();
			QFuture future = promise.future();
			promise.start();
			QtConcurrent.run(pool, ()->{
				try {
					QThread.msleep(500);
					FutureHandler.fillString(promise);
				}catch(Throwable t) {
					promise.setException(t);
				}finally {
					promise.finish();
				}
			});
			try{
				future.waitForFinished();
				assertFalse("Exception expected to be thrown.", true);
			}catch(IllegalArgumentException e) {
				Assert.assertEquals("Cannot cast QPromise<void> to QPromise<QString>.", e.getMessage());
			}
		}
		
		{
			QFutureInterface iface = FutureHandler.interfaceInt(); 
			QPromise promise = new QPromise(iface);
			QFuture future = promise.future();
			promise.start();
			QtConcurrent.run(pool, ()->{
				try {
					QThread.msleep(500);
					FutureHandler.fillString(promise);
				}catch(Throwable t) {
					promise.setException(t);
				}finally {
					promise.finish();
				}
			});
			try{
				future.waitForFinished();
				assertFalse("Exception expected to be thrown.", true);
			}catch(IllegalArgumentException e) {
				Assert.assertEquals("Cannot cast QPromise<int> to QPromise<QString>.", e.getMessage());
			}
		}
		
		{
			QFuture<Void> future = QtConcurrent.run(pool, (QPromise<Void> promise)->{
				try {
					QThread.msleep(500);
					((QPromise)promise).addResult("X");
				}catch(Throwable t) {
					promise.setException(t);
				}
			});
			try{
				future.waitForFinished();
				assertFalse("Exception expected to be thrown.", true);
			}catch(QNoImplementationException e) {
				Assert.assertEquals("addResult(T,int) not available for QPromise<void>.", e.getMessage());
			}
		}
		
		{
			QFuture<Void> future = QtConcurrent.run(pool, (QPromise<Void> promise)->{
				try {
					QThread.msleep(500);
					FutureHandler.fillObject((QPromise)promise);
				}catch(Throwable t) {
					promise.setException(t);
				}
			});
			try{
				future.waitForFinished();
				assertFalse("Exception expected to be thrown.", true);
			}catch(IllegalArgumentException e) {
				Assert.assertEquals("Cannot cast QPromise<void> to QPromise<QObject*>.", e.getMessage());
			}
		}
		
		{
			QFuture<Void> future = QtConcurrent.run(pool, (QPromise<Void> promise)->{
				try {
					QThread.msleep(500);
					FutureHandler.fillInt((QPromise)promise);
				}catch(Throwable t) {
					promise.setException(t);
				}
			});
			try{
				future.waitForFinished();
				assertFalse("Exception expected to be thrown.", true);
			}catch(IllegalArgumentException e) {
				Assert.assertEquals("Cannot cast QPromise<void> to QPromise<int>.", e.getMessage());
			}
		}
		
		{
			QFuture<Void> future = QtConcurrent.run(pool, (QPromise<Void> promise)->{
				try {
					QThread.msleep(500);
					FutureHandler.fillVariant((QPromise)promise);
				}catch(Throwable t) {
					promise.setException(t);
				}
			});
			try{
				future.waitForFinished();
				assertFalse("Exception expected to be thrown.", true);
			}catch(IllegalArgumentException e) {
				Assert.assertEquals("Cannot cast QPromise<void> to QPromise<QVariant>.", e.getMessage());
			}
		}
	}
	
	@Test
    public void testJavaQFutureWithPromise() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	List<String> result = FutureHandler.returnInTheFuture(QtConcurrent.run((QPromise<String> promise)->{
    		QThread.msleep(200);
    		promise.addResult("testJavaQFuture1");
    		promise.addResult("testJavaQFuture2");
    	}));
    	Assert.assertEquals(2, result.size());
    	Assert.assertEquals("testJavaQFuture1", result.get(0));
    	Assert.assertEquals("testJavaQFuture2", result.get(1));
    }
    
    @Test
    public void testSequentialJavaQFutureWithPromise() throws Throwable {
    	Assume.assumeTrue("Disabled unless you specify test.enable.criticals=true", Boolean.getBoolean("test.enable.criticals"));
    	Throwable[] throwable = {null};
    	QThread thread = QThread.create(()->FutureHandler.returnSequentialInTheFuture(QtConcurrent.run((QPromise<String> promise)->{
    		try {
				promise.setProgressValueAndText(1, "sending: testSequentialJavaQFuture1");
				QThread.msleep(200);
				promise.suspendIfRequested();
				promise.addResult("testSequentialJavaQFuture1");
				promise.setProgressValueAndText(2, "sending: testSequentialJavaQFuture2");
				QThread.msleep(20);
				promise.suspendIfRequested();
				promise.addResult("testSequentialJavaQFuture2");
				promise.setProgressValueAndText(3, "sending: testSequentialJavaQFuture3");
				QThread.msleep(20);
				promise.suspendIfRequested();
				promise.addResult("testSequentialJavaQFuture3");
				promise.setProgressValueAndText(4, "sending: testSequentialJavaQFuture4");
				QThread.msleep(20);
				promise.suspendIfRequested();
				promise.addResult("testSequentialJavaQFuture4");
				promise.setProgressValueAndText(5, "sending: testSequentialJavaQFuture5");
				QThread.msleep(20);
				promise.suspendIfRequested();
				promise.addResult("testSequentialJavaQFuture5");
			} catch (Throwable e) {
				throwable[0] = e;
			}
    	})));
    	thread.start();
    	thread.join(120000);
    	try {
	    	Assert.assertFalse("FutureHandler.returnSequentialInTheFuture does not return.", thread.isRunning());
	    	if(throwable[0]!=null)
	    		throw throwable[0];
    	}finally {
    		thread.interrupt();
    	}
    }
    
    @Test
    public void testExceptionJavaQFutureWithPromise() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	try {
	    	FutureHandler.throwInTheFuture(QtConcurrent.run((QPromise<String> promise)->{
	    		QThread.msleep(200);
	    		throw new RuntimeException();
	    	}));
	    	Assert.assertFalse("exception expected to be thrown", true);
		} catch (RuntimeException e) {
		}
    }
    
    @Test
    public void testSuspendingJavaQFuture() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	QFutureInterface<String> promise = new QFutureInterface<>();
		promise.reportStarted();
		boolean[] isSuspending = {false};
    	QtConcurrent.run(()->{
    		try {
    			int i=0;
        		while(!promise.isSuspending() && (i++<30))
        			QThread.msleep(200);
        		isSuspending[0] = promise.isSuspending();
    		}catch(Throwable e) {
    			promise.reportException(e);
    		}finally {
    			promise.reportFinished();
    		}
    	});
    	FutureHandler.suspendInTheFuture(promise.future());
    	Assert.assertTrue(isSuspending[0]);
    }
    
	@Test
    public void testResumeJavaQFuture() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	QFutureInterface<String> promise = new QFutureInterface<>();
		promise.reportStarted();
		promise.setSuspended(true);
		boolean[] isSuspended = {true};
    	QtConcurrent.run(()->{
    		try {
    			int i=0;
        		while(promise.isSuspended() && (i++<30))
        			QThread.msleep(200);
        		isSuspended[0] = promise.isSuspended();
    		}catch(Throwable e) {
    			promise.reportException(e);
    		}finally {
    			promise.reportFinished();
    		}
    	});
    	FutureHandler.resumeInTheFuture(promise.future());
    	Assert.assertFalse(isSuspended[0]);
    }
    
	@Test
    public void testSuspendingJavaQFuture_nested() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	QFutureInterface<String> promise = new QFutureInterface<>();
		promise.reportStarted();
		boolean[] isSuspending = {false};
    	QtConcurrent.run(()->{
    		try {
    			int i=0;
        		while(!promise.isSuspending() && (i++<30))
        			QThread.msleep(200);
        		isSuspending[0] = promise.isSuspending();
    		}catch(Throwable e) {
    			promise.reportException(e);
    		}finally {
    			promise.reportFinished();
    		}
    	});
    	FutureHandler.suspendInTheFuture(FutureHandler.forward(promise.future()));
    	Assert.assertTrue(isSuspending[0]);
    }
    
	@Test
    public void testResumeJavaQFuture_nested() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	QFutureInterface<String> promise = new QFutureInterface<>();
		promise.reportStarted();
		promise.setSuspended(true);
		boolean[] isSuspended = {true};
    	QtConcurrent.run(()->{
    		try {
    			int i=0;
        		while(promise.isSuspended() && (i++<30))
        			QThread.msleep(200);
        		isSuspended[0] = promise.isSuspended();
    		}catch(Throwable e) {
    			promise.reportException(e);
    		}finally {
    			promise.reportFinished();
    		}
    	});
    	FutureHandler.resumeInTheFuture(FutureHandler.forward(promise.future()));
    	Assert.assertFalse(isSuspended[0]);
    }
    
    @Test
    public void testStartFilteredReduced() {
        List<Integer> ints = new ArrayList<Integer>();
        for (int i=0; i<COUNT*2; ++i)
            ints.add(i);

        QtConcurrent.ThreadEngineStarter<Integer> starter = QtConcurrent.startFilteredReduced(
        		QThreadPool.globalInstance(),
        		ints,
        		i->i >= COUNT,
        		(r, intermediate)->{
        			if(intermediate==null)
        				intermediate = 0;
        			if(r==null)
        				r = 0;
        			return r + intermediate;
        		}
        );

        QFuture<Integer> future = starter.startAsynchronously();
        future.waitForFinished();
        assertEquals(COUNT*2, ints.size());
        assertEquals(1, future.resultCount());

        int n=0;
        for (int i=COUNT; i<COUNT*2; ++i)
            n += i;

        assertTrue(future.result()!=null);
        assertEquals(n, future.result().intValue());
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestConcurrent.class.getName());
    }
}
