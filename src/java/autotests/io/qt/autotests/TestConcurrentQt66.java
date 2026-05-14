package io.qt.autotests;

import static org.junit.Assert.assertEquals;

import java.util.Arrays;

import org.junit.Test;

import io.qt.concurrent.QtConcurrent;
import io.qt.core.*;

public class TestConcurrentQt66 extends ApplicationInitializer {
    
    @Test
    public void testFutureUnwrap() throws InterruptedException {
//    	QSemaphore semaphore = new QSemaphore();
    	QStringList list = new QStringList("A", "B", "C");
		QFuture<QFuture<QFuture<String>>> results = QtConcurrent.mapped(list, s->{
//			semaphore.acquire();
			return QtFuture.makeReadyRangeFuture(Arrays.asList(QtFuture.makeReadyValueFuture(s), QtFuture.makeReadyValueFuture(s)));
		});
		Thread.sleep(200);
    	QFuture<String> unwrapped = results.unwrap(String.class);
//    	semaphore.release(list.size());
    	assertEquals(6, unwrapped.results().size());
    }
}
