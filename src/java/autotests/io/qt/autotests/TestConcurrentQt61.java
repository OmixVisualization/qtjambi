package io.qt.autotests;

import static org.junit.Assert.assertEquals;

import java.util.Arrays;

import org.junit.Test;

import io.qt.concurrent.QtConcurrent;
import io.qt.core.QFuture;
import io.qt.core.QStringList;
import io.qt.core.QtFuture;

@SuppressWarnings("removal")
public class TestConcurrentQt61 extends ApplicationInitializer {
	@Test
	@Deprecated
    public void testFutureUnwrap() {
    	QStringList list = new QStringList("A", "B", "C");
		QFuture<QFuture<QFuture<String>>> results = QtConcurrent.mapped(list, s->QtFuture.makeReadyFuture(Arrays.asList(QtFuture.makeReadyFuture(s), QtFuture.makeReadyFuture(s))));
    	QFuture<String> unwrapped = results.unwrap(String.class);
    	assertEquals(6, unwrapped.results().size());
    }
}
