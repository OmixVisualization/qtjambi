/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import java.io.File;
import java.nio.file.Files;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.junit.AfterClass;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtUtilities;
import io.qt.core.QByteArray;
import io.qt.core.QDataStream;
import io.qt.core.QEventLoop;
import io.qt.core.QRect;
import io.qt.core.QRectF;
import io.qt.core.QThread;
import io.qt.core.Qt;
import io.qt.dbus.QDBusConnection;
import io.qt.dbus.QDBusInterface;
import io.qt.dbus.QDBusMessage;
import io.qt.dbus.QDBusPendingCall;
import io.qt.dbus.QDBusPendingReply;
import io.qt.dbus.QDBusReply;
import io.qt.gui.QColor;
import io.qt.internal.TestUtility;

public class TestDBusConnect {
	
	private static QDBusConnection sb;
	private static Throwable exception = null;
	private static QThread pongThread;
	private static QDBusInterface iface;
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		System.out.println("TestDBusConnect::testInitialize()...");
		sb = QDBusConnection.sessionBus();
		Assume.assumeTrue("QDBus session bus is not connected: "+sb.lastError().message(), sb.isConnected());
		ApplicationInitializer.testInitialize();
		QtDBusPong.registerTypes();
		pongThread = QThread.create(() -> {
			QEventLoop loop = new QEventLoop();
			try {
				System.out.println("TestDBusConnect: Deploying QtDBusPong...");
				ApplicationInitializer.testDeployerApp("QtDBusPong", "dbus");
				System.out.println("TestDBusConnect: Deploying QtDBusPong finished");
			} catch (InterruptedException e) {
			} catch (Throwable e) {
				exception = e;
			}finally {
				loop.dispose();
			}
		});
		pongThread.start();
		pongThread.join(2000);
		long t1 = System.currentTimeMillis();
    	final String jambidir = System.getProperty("user.dir");
    	final File testsDir = new File(jambidir).getParentFile();
    	final File targetDir = new File(testsDir, "tmp_"+TestUtility.processName());
    	final File testFile = new File(targetDir, "QtDBusPong.touch.test");
		while(!testFile.exists() && pongThread.isAlive()){
			pongThread.join(2000);
			if(System.currentTimeMillis()-t1>25000) {
				Assume.assumeFalse("Deploying QtDBusPong failed", true);
				break;
			}
		}
		if (exception != null) {
			Throwable _exception = exception;
			exception = null;
			throw new org.junit.AssumptionViolatedException("Deploying QtDBusPong failed", _exception);
		}
		System.out.println("TestDBusConnect: Get bus interface...");
		iface = new QDBusInterface(QtDBusPong.PONG_SERVICE, "/", QtDBusPong.PONG_INTERFACE_NAME, sb);
		if(!iface.isValid())
			fail(sb.lastError().message());
		System.out.println("TestDBusConnect::testInitialize() finished");
	}
	
	@AfterClass
    public static void testDispose() throws Throwable {
		try {
			if(iface!=null) {
				iface.call("quit");
				iface.dispose();
				iface = null;
			}
			if(pongThread!=null) {
				pongThread.interrupt();
				pongThread.join();
				pongThread.dispose();
				pongThread = null;
			}
			if (exception != null)
				throw exception;
		} finally {
			{
		    	final String version = QtUtilities.qtjambiVersion().toString();
		    	final String jambidir = System.getProperty("user.dir");
		    	final File testsDir = new File(jambidir).getParentFile();
		    	final File targetDir = new File(testsDir, "tmp_"+TestUtility.processName());
				File tmpDir = new File(System.getProperty("java.io.tmpdir"));
				if(new File(targetDir, "pid").isFile()) {
					String processName = new String(Files.readAllBytes(new File(targetDir, "pid").toPath())).trim();;
					if(processName!=null && !processName.isEmpty()) {
						File jambiDeploymentDir = new File(tmpDir, "QtJambi" + version + "_" + System.getProperty("user.name") + "_" + processName);
						System.out.println("Deleting "+jambiDeploymentDir.getAbsolutePath());
						if(jambiDeploymentDir.isDirectory())
							ApplicationInitializer.clearAndDelete(jambiDeploymentDir);
					}
				}
				
				if(targetDir.isDirectory() && !Boolean.getBoolean("qtjambi.deployer.skip.deletion")) {
	    			Logger.getLogger("io.qt.autotests").log(Level.FINEST, ()->"Cleaning deployer directory "+targetDir);
	    			ApplicationInitializer.clearAndDelete(targetDir);
	    		}
			}
			ApplicationInitializer.testDispose();
		}
	}
	
	@Test
	public void test_to_rect() {
		QDBusMessage msg = iface.call("toRect", 4,5,6,7);
		QDBusReply<QRect> reply = new QDBusReply<>(msg, QRect.class);
		if(!reply.isValid())
			fail(reply.error().message());
		QRect rect = reply.value();
		assertTrue(rect!=null);
		assertEquals(4, rect.left());
		assertEquals(5, rect.top());
		assertEquals(6, rect.width());
		assertEquals(7, rect.height());
	}
	
	@Test
	public void test_from_rectf() {
		QDBusMessage msg = iface.call("fromRectF", new QRectF(4,5,6,7));
		QDBusReply<QByteArray> reply = new QDBusReply<>(msg, QByteArray.class);
		if(!reply.isValid())
			fail(reply.error().message());
		QByteArray array = reply.value();
		assertTrue(array!=null);
		QDataStream stream = new QDataStream(array);
		assertEquals(4, stream.readDouble(), 0.001);
		assertEquals(5, stream.readDouble(), 0.001);
		assertEquals(6, stream.readDouble(), 0.001);
		assertEquals(7, stream.readDouble(), 0.001);
	}

	@Test
	public void test_pingPong() {
		for (int i = 0; i < 10; ++i) {
			QDBusMessage msg = iface.call("pingPong", "Ping#" + i);
			QDBusReply<String> reply = new QDBusReply<>(msg, String.class);
			if(!reply.isValid())
				fail(reply.error().message());
			assertTrue("Pong#X expected: "+reply.value(), reply.value().startsWith("Pong#"));
		}
	}
	@Test
	public void test_to_rect_async() {
		QDBusPendingCall pending = iface.asyncCall("toRect", 4,5,6,7);
		QDBusPendingReply<QRect> reply = new QDBusPendingReply<>(pending, QRect.class);
		reply.waitForFinished();
		if(reply.isError())
			fail(reply.error().message());
		QRect rect = reply.argumentAt0();
		assertTrue(rect!=null);
		assertEquals(4, rect.left());
		assertEquals(5, rect.top());
		assertEquals(6, rect.width());
		assertEquals(7, rect.height());
	}
	@Test
	public void test_from_rectf_async() {
		QDBusPendingCall pending = iface.asyncCall("fromRectF", new QRectF(4,5,6,7));
		QDBusPendingReply<QByteArray> reply = new QDBusPendingReply<>(pending, QByteArray.class);
		reply.waitForFinished();
		if(reply.isError())
			fail(reply.error().message());
		QByteArray array = reply.argumentAt0();
		assertTrue(array!=null);
		QDataStream stream = new QDataStream(array);
		assertEquals(4, stream.readDouble(), 0.001);
		assertEquals(5, stream.readDouble(), 0.001);
		assertEquals(6, stream.readDouble(), 0.001);
		assertEquals(7, stream.readDouble(), 0.001);
	}
	
	@Test
	public void test_getIntArray_async() {
		System.out.println("async calling getIntArray");
		QDBusPendingCall pending = iface.asyncCall("getIntArray");
		QDBusPendingReply<int[]> reply = new QDBusPendingReply<>(pending, int[].class);
		reply.waitForFinished();
		if(reply.isError())
			fail(reply.error().message());
		int[] array = reply.argumentAt0();
		System.out.println("result of getIntArray...");
		assertTrue(array!=null);
		assertEquals(5, array.length);
		assertEquals(1, array[0]);
		assertEquals(2, array[1]);
		assertEquals(3, array[2]);
		assertEquals(4, array[3]);
		assertEquals(5, array[4]);
	}
	
	@Test
	public void test_getIntArray() {
		System.out.println("calling getIntArray");
		QDBusMessage msg = iface.call("getIntArray");
		System.out.println("calling getIntArray finished");
		QDBusReply<int[]> reply = new QDBusReply<>(msg, int[].class);
		if(!reply.isValid())
			fail(reply.error().message());
		int[] array = reply.value();
		assertTrue(array!=null);
		assertEquals(5, array.length);
		assertEquals(1, array[0]);
		assertEquals(2, array[1]);
		assertEquals(3, array[2]);
		assertEquals(4, array[3]);
		assertEquals(5, array[4]);
	}
	
	@Test
	public void test_getNonQtType() {
		QDBusMessage msg = iface.call("getNonQtType");
		QDBusReply<QtDBusPong.NonQtType> reply = new QDBusReply<>(msg, QtDBusPong.NonQtType.class);
		if(!reply.isValid())
			fail(reply.error().message());
		QtDBusPong.NonQtType value = reply.value();
		assertTrue(value!=null);
		assertEquals(5, value.i);
		assertEquals(2.5, value.d, 0.00001);
		assertEquals("TEST", value.s);
	}
	
	@Test
	public void test_getNonQtType_async() {
		QDBusPendingCall pending = iface.asyncCall("getNonQtType");
		QDBusPendingReply<QtDBusPong.NonQtType> reply = new QDBusPendingReply<>(pending, QtDBusPong.NonQtType.class);
		reply.waitForFinished();
		if(reply.isError())
			fail(reply.error().message());
		QtDBusPong.NonQtType value = reply.value();
		assertTrue(value!=null);
		assertEquals(5, value.i);
		assertEquals(2.5, value.d, 0.00001);
		assertEquals("TEST", value.s);
	}
	
	@Test
	public void test_getColor() {
		QDBusMessage msg = iface.call("getColor");
		QDBusReply<QColor> reply = new QDBusReply<>(msg, QColor.class);
		if(!reply.isValid())
			fail(reply.error().message());
		QColor value = reply.value();
		assertTrue(value!=null);
		assertEquals(new QColor(Qt.GlobalColor.magenta), value);
	}
	
	@Test
	public void test_getColor_async() {
		QDBusPendingCall pending = iface.asyncCall("getColor");
		QDBusPendingReply<QColor> reply = new QDBusPendingReply<>(pending, QColor.class);
		reply.waitForFinished();
		if(reply.isError())
			fail(reply.error().message());
		QColor value = reply.value();
		assertTrue(value!=null);
		assertEquals(new QColor(Qt.GlobalColor.magenta), value);
	}
	
	@Test
	public void test_multiArgs() {
		QtDBusPong.NonQtType value = new QtDBusPong.NonQtType();
		value.s = "test_multiArgs";
		QDBusMessage msg = iface.call("callMultiArgs", value/*, 5, 3.9, new QColor(4,3,2)*/);
		QDBusReply<String> reply = new QDBusReply<>(msg, String.class);
		if(!reply.isValid())
			fail(reply.error().message());
		System.out.println(reply.value());
	}

	public static void main(String args[]) {
		org.junit.runner.JUnitCore.main(TestDBusConnect.class.getName());
	}
}
