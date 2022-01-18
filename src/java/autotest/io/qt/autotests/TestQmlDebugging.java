package io.qt.autotests;

import org.junit.Assert;
import org.junit.Test;

import io.qt.qml.QQmlDebuggingEnabler;

public class TestQmlDebugging extends QApplicationTest{
	
	@Test
	public void test() {
		System.setProperty("io.qt.enabled-qml-debugging", "true");
		Assert.assertTrue(QQmlDebuggingEnabler.startTcpDebugServer(50000, QQmlDebuggingEnabler.StartMode.DoNotWaitForClient));
	}

}
