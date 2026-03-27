package io.qt.autotests;

import java.util.logging.Handler;
import java.util.logging.Logger;

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QLogging;
import io.qt.core.QSize;

public class TestLogs extends UnitTestInitializer{
	@BeforeClass
	public static void testInitialize() throws Exception {
		System.setProperty("io.qt.enable-method-logs", "true");
		System.setProperty("io.qt.enable-cleanup-logs", "true");
		Logger rootLogger = Logger.getLogger("");
		for (Handler h : rootLogger.getHandlers()) {
		    rootLogger.removeHandler(h);
		}
		rootLogger.addHandler(new QLogging.Handler());
		QLogging.qInstallMessageHandler((type,context,message)->{
			System.out.println(message);
		});
//		ApplicationInitializer.testInitialize();
	}
	
	@Test
    public void test() {
		QSize size = new QSize();
		size.dispose();
	}
}
