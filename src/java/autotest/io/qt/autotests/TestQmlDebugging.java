package io.qt.autotests;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.qml.QQmlDebuggingEnabler;

public class TestQmlDebugging extends ApplicationInitializer{
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithGui();
    }
	
	@Test
	public void test() {
		System.setProperty("io.qt.enabled-qml-debugging", "true");
		Assert.assertTrue(QQmlDebuggingEnabler.startTcpDebugServer(50000, QQmlDebuggingEnabler.StartMode.DoNotWaitForClient));
	}

}
