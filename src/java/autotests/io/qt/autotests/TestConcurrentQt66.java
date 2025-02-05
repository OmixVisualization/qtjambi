package io.qt.autotests;

import static org.junit.Assert.assertEquals;

import java.util.Arrays;

import org.junit.Test;

import io.qt.concurrent.QtConcurrent;
import io.qt.core.QFuture;
import io.qt.core.QStringList;
import io.qt.core.QtFuture;

public class TestConcurrentQt66 extends ApplicationInitializer {
    
    @Test
    public void testFutureUnwrap() {
    	QStringList list = new QStringList("A", "B", "C");
		QFuture<QFuture<QFuture<String>>> results = QtConcurrent.mapped(list, s->QtFuture.makeReadyRangeFuture(Arrays.asList(QtFuture.makeReadyValueFuture(s), QtFuture.makeReadyValueFuture(s))));
    	QFuture<String> unwrapped = results.unwrap(String.class);
    	assertEquals(6, unwrapped.results().size());
    }
}
