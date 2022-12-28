package io.qt.autotests;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.qml.QQmlDebuggingEnabler;

public class TestQmlDebugging extends ApplicationInitializer{
	
	@BeforeClass
    public static void testInitialize() throws Exception {
		System.setProperty("io.qt.enabled-qml-debugging", "true");
    	ApplicationInitializer.testInitializeWithGui();
    }
	
	@Test
	public void test() {
		Assert.assertTrue(QQmlDebuggingEnabler.startTcpDebugServer(50000, QQmlDebuggingEnabler.StartMode.DoNotWaitForClient));
	}

}
