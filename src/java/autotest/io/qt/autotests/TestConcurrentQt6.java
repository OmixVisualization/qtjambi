package io.qt.autotests;

import static io.qt.autotests.TestConcurrent.COUNT;
import static org.junit.Assert.*;

import java.util.ArrayList;
import java.util.List;

import org.junit.Test;

import io.qt.concurrent.QtConcurrent;
import io.qt.concurrent.QtConcurrent.*;
import io.qt.core.QFuture;
import io.qt.core.QVoidFuture;
import io.qt.core.QPromise;
import io.qt.core.QThread;
import io.qt.core.QThreadPool;

public class TestConcurrentQt6 extends QApplicationTest {
	
	QThreadPool pool = new QThreadPool();
	
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
		QVoidFuture future = QtConcurrent.run(pool, number->{
			QThread.currentThread().join(200);
			result[0] = Integer.parseInt(number);
		}, "12340");
		future.waitForFinished();
        assertEquals(12340, result[0]);
	}
	
	@Test
    public void testRunWithPromiseWithThreadPool() {
		QFuture<Integer> future = QtConcurrent.run(pool, (QPromise<Integer> promise, String number)->{
			QThread.currentThread().join(200);
			promise.addResult(Integer.parseInt(number));
			promise.addResult(Integer.parseInt(number.substring(1)));
			promise.addResult(Integer.parseInt(number.substring(2)));
			promise.addResult(Integer.parseInt(number.substring(3)));
		}, "12341");
        assertEquals(12341, (int) future.resultAt(0));
        assertEquals(2341, (int) future.resultAt(1));
        assertEquals(341, (int) future.resultAt(2));
        assertEquals(41, (int) future.resultAt(3));
        assertEquals(4, future.resultCount());
        try {
			future.resultAt(4);
			assertFalse("IndexOutOfBoundsException expected", true);
		} catch (IndexOutOfBoundsException e) {
		}
	}
	
	@Test
    public void testTaskWithThreadPool() {
		QTypedTaskBuilder1Arg1<Integer, String> task = QtConcurrent.task((String number)->{
			QThread.currentThread().join(200);
			return Integer.parseInt(number);
		}).withArguments("12342").onThreadPool(pool);
		QFuture<Integer> future = task.spawn();
        assertEquals(12342, (int) future.result());
	}
	
	@Test
    public void testTaskWithPromiseWithThreadPool() {
		QTypedPromiseTaskBuilder1Arg1<Integer, String> task = QtConcurrent.task((QPromise<Integer> promise, String number)->{
			QThread.currentThread().join(200);
			promise.addResult(Integer.parseInt(number));
		}).withArguments("12343").onThreadPool(pool);
		QFuture<Integer> future = task.spawn();
        assertEquals(12343, (int) future.result());
	}
	
	@Test
    public void testRunWithoutPromise() {
		int[] result = {0};
		QVoidFuture future = QtConcurrent.run(number->{
			QThread.currentThread().join(200);
			result[0] = Integer.parseInt(number);
		}, "12344");
		future.waitForFinished();
        assertEquals(12344, result[0]);
	}
	
	@Test
    public void testRunWithPromise() {
		QFuture<Integer> future = QtConcurrent.run((QPromise<Integer> promise, String number)->{
			QThread.currentThread().join(200);
			promise.addResult(Integer.parseInt(number));
			promise.addResult(Integer.parseInt(number.substring(1)));
			promise.addResult(Integer.parseInt(number.substring(2)));
			promise.addResult(Integer.parseInt(number.substring(3)));
		}, "12345");
        assertEquals(12345, (int) future.result());
        assertEquals(2345, (int) future.resultAt(1));
        assertEquals(345, (int) future.resultAt(2));
        assertEquals(45, (int) future.resultAt(3));
        assertEquals(4, future.resultCount());
        try {
			future.resultAt(4);
			assertFalse("IndexOutOfBoundsException expected", true);
		} catch (IndexOutOfBoundsException e) {
		}
	}
	
	@Test
    public void testTask() {
		QTypedTaskBuilder1Arg1<Integer, String> task = QtConcurrent.task((String number)->{
			QThread.currentThread().join(200);
			return Integer.parseInt(number);
		}).withArguments("12346");
		QFuture<Integer> future = task.spawn();
        assertEquals(12346, (int) future.result());
	}
	
	@Test
    public void testTaskWithPromise() {
		QTypedPromiseTaskBuilder1Arg1<Integer, String> task = QtConcurrent.task((QPromise<Integer> promise, String number)->{
			QThread.currentThread().join(200);
			promise.addResult(Integer.parseInt(number));
			promise.addResult(Integer.parseInt(number.substring(1)));
			promise.addResult(Integer.parseInt(number.substring(2)));
			promise.addResult(Integer.parseInt(number.substring(3)));
		}).withArguments("12347");
		QFuture<Integer> future = task.spawn();
        assertEquals(12347, (int) future.result());
        assertEquals(2347, (int) future.resultAt(1));
        assertEquals(347, (int) future.resultAt(2));
        assertEquals(47, (int) future.resultAt(3));
        assertEquals(4, future.resultCount());
        try {
			future.resultAt(4);
			assertFalse("IndexOutOfBoundsException expected", true);
		} catch (IndexOutOfBoundsException e) {
		}
	}
}
