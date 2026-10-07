package io.qt.autotests;

import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QCoreApplication;
import io.qt.core.QLibraryInfo;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QTimer;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QWindow;
import io.qt.qml.QQmlDebuggingEnabler;

public class TestQmlDebugging extends ApplicationInitializer{
	
	@BeforeClass
    public static void testInitialize() throws Exception {
		Assume.assumeFalse("Does not work on Android batched tests", QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android) || QCoreApplication.instance()==null);
		Assume.assumeFalse("Cannot run with Qt >= 6.11", QLibraryInfo.version().compareTo(new int[]{6,11,0})>=0);
		System.setProperty("io.qt.enabled-qml-debugging", "true");
    	ApplicationInitializer.testInitializeWithGui();
    }
	
	@AfterClass
    public static void testDispose() throws Exception {
		if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.MacOS) 
    			&& QLibraryInfo.version().majorVersion()==6 
    			&& QLibraryInfo.version().minorVersion()==5) {
	    	QWindow window = new QWindow();
	    	window.show();
	    	QTimer.singleShot(200, QGuiApplication.instance(), QGuiApplication::quit);
	    	QGuiApplication.exec();
	    	window.close();
	    	window.disposeLater();
	    	window = null;
    	}
    	ApplicationInitializer.testDispose();
    }
	
	@Test
	public void test() {
		Assert.assertTrue(QQmlDebuggingEnabler.startTcpDebugServer(50000, QQmlDebuggingEnabler.StartMode.DoNotWaitForClient));
	}

}
