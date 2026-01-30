/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import static io.qt.test.QTest.QCOMPARE;
import static io.qt.test.QTest.QTEST_MAIN;
import static io.qt.test.QTest.QTRY_VERIFY_WITH_TIMEOUT;
import static org.junit.Assert.assertTrue;
import static org.junit.Assume.assumeTrue;

import org.junit.Test;

import io.qt.core.QLibraryInfo;
import io.qt.core.QObject;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QSysInfo;
import io.qt.core.QTimer;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QIcon;
import io.qt.gui.QWindow;
import io.qt.test.QSignalSpy;
import io.qt.widgets.QApplication;

public class TestTestlib extends UnitTestInitializer {
	
    private static void showWidget() {
    	if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.MacOS) 
    			&& QLibraryInfo.version().majorVersion()==6 
    			&& QLibraryInfo.version().minorVersion()==5) {
			QWindow window = new QWindow();
	    	window.show();
	    	QTimer.singleShot(500, QApplication.instance(), QApplication::quit);
	    	QApplication.exec();
	    	window.dispose();
    	}
	}
	
    @Test
    public void testMain() {
    	assumeTrue("Can only run successfully on x86_64", "x86_64".equals(QSysInfo.currentCpuArchitecture()));
    	TestObject.initMainCalled = false;
    	TestObject.test1Called = false;
    	TestObject.test2Called = false;
    	TestObject.test3Called = false;
    	TestObject.test4Called = false;
    	TestObject.main();
    	assertTrue("initMain not called", TestObject.initMainCalled);
    	assertTrue("initTestCase not called", TestObject.initTestCaseCalled);
    	assertTrue("test1 not called", TestObject.test1Called);
    	assertTrue("test2 not called", TestObject.test2Called);
    	assertTrue("test3 not called", TestObject.test3Called);
    	assertTrue("test4 not called", TestObject.test4Called);
    }
    
    @Test
    public void testTest() {
    	tst_QTimer.main();
    }
    
	@SuppressWarnings("unused")
    public static class TestObject extends QObject{
    	static boolean initMainCalled;
    	static boolean initTestCaseCalled;
    	static boolean test1Called;
    	static boolean test2Called;
    	static boolean test3Called;
    	static boolean test4Called;
    	public static void initMain() {
    		initMainCalled = true;
    	}
    	public void initTestCase() {
    		QGuiApplication.setWindowIcon(new QIcon(":io/qt/autotests/icon.png"));
    		initTestCaseCalled = true;
    	}
    	public void cleanupTestCase() {
    		TestTestlib.showWidget();
    	}
    	public static void main(String... args) {
    		QTEST_MAIN(args);
    	}
    	
		private void test1() {
    		test1Called = true;
    	}
    	private void test2() {
    		test2Called = true;
    		
    	}
    	private void test3() {
    		test3Called = true;
			
		}
    	private void test4() {
    		test4Called = true;
			
		}
    }
	
	@SuppressWarnings("unused")
    public static class tst_QTimer extends QObject{
		public static void initMain() {
//			QLogging.qInstallLoggingMessageHandler();
		}
		private static final int TIMEOUT_TIMEOUT = 200;
		private void timeout()
		{
		    QTimer timer = new QTimer();
		    QSignalSpy timeoutSpy = new QSignalSpy(timer.timeout);
		    timer.start(100);

		    QCOMPARE(timeoutSpy.count(), 0);

		    QTRY_VERIFY_WITH_TIMEOUT(()->timeoutSpy.count() > 0, TIMEOUT_TIMEOUT);
		    int oldCount = timeoutSpy.count();

		    QTRY_VERIFY_WITH_TIMEOUT(()->timeoutSpy.count() > oldCount, TIMEOUT_TIMEOUT);
		}
		public void initTestCase() {
    		QGuiApplication.setWindowIcon(new QIcon(":io/qt/autotests/icon.png"));
		}
		public void cleanupTestCase() {
			TestTestlib.showWidget();
    	}
		public static void main(String... args) {
    		QTEST_MAIN(args);
    	}
	}
}
