/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

import java.io.File;
import java.nio.file.Files;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtPropertyReader;
import io.qt.QtPropertyWriter;
import io.qt.QtUtilities;
import io.qt.autotests.QtRemoteObjectsPong.NonQtType;
import io.qt.core.QByteArray;
import io.qt.core.QCoreApplication;
import io.qt.core.QDataStream;
import io.qt.core.QEventLoop;
import io.qt.core.QIODevice;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaObject.AbstractPublicSignal0;
import io.qt.core.QMetaType;
import io.qt.core.QObject;
import io.qt.core.QRect;
import io.qt.core.QThread;
import io.qt.core.QTimer;
import io.qt.core.QUrl;
import io.qt.core.Qt;
import io.qt.gui.QColor;
import io.qt.gui.QColorConstants;
import io.qt.internal.TestUtility;
import io.qt.remoteobjects.QRemoteObjectDynamicReplica;
import io.qt.remoteobjects.QRemoteObjectNode;
import io.qt.remoteobjects.QRemoteObjectPendingCall;
import io.qt.remoteobjects.QRemoteObjectPendingReply;
import io.qt.remoteobjects.QtRemoteObjects;

public class TestRemoteObjects extends UnitTestInitializer {
	
	private static Throwable exception = null;
	private static QThread pongThread;
	
	@BeforeClass
	public static void testInitialize() throws Throwable {
		ApplicationInitializer.testInitialize();
		pongThread = QThread.create(() -> {
			QEventLoop loop = new QEventLoop();
			try {
				System.out.println("Deploying QtRemoteObjectsPong...");
				ApplicationInitializer.testDeployerApp("QtRemoteObjectsPong", "network", "remoteobjects");
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
    	final File testFile = new File(targetDir, "QtRemoteObjectsPong.touch.test");
		while(!testFile.exists() && pongThread.isAlive()){
			pongThread.join(2000);
			if(System.currentTimeMillis()-t1>25000) {
				Assume.assumeFalse("Deploying QtRemoteObjectsPong failed", true);
				break;
			}
		}
		if (exception != null) {
			Throwable _exception = exception;
			exception = null;
			throw new org.junit.AssumptionViolatedException("Deploying QtRemoteObjectsPong failed", _exception);
		}
	}
	
	@AfterClass
    public static void testDispose() throws Throwable {
		try {
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
	    			Logger.getLogger("io.qt.autotests").log(Level.FINEST, "Cleaning deployer directory {0}", targetDir);
	    			ApplicationInitializer.clearAndDelete(targetDir);
	    		}
			}
			ApplicationInitializer.testDispose();
		}
	}
	
	@Test
    public void testPingPong() throws Throwable
    {
		QMetaType.qRegisterMetaType(NonQtType.class);
		QRemoteObjectNode node = new QRemoteObjectNode(new QUrl("local:ropong"));
		QRemoteObjectDynamicReplica pong = node.acquireDynamic("pong");
		QTimer timer = new QTimer();
		timer.setSingleShot(true);
		timer.setInterval(10000);
		timer.timeout.connect(QCoreApplication::quit);
		timer.start();
		Throwable exception[] = {new AssertionError("Replica not initialized")};
		try {
			System.out.println("initialize QRemoteObjectDynamicReplica...");
			pong.initialized.connect(()->{
				exception[0] = null;
				timer.stop();
				timer.timeout.disconnect();
				QObject object = new QObject();
				object.destroyed.connect(()->{
					try {
						Object result;
						result = QMetaObject.invokeMethod(pong, "getInt");
						if(result instanceof QRemoteObjectPendingCall) {
							QRemoteObjectPendingReply<Integer> pending = new QRemoteObjectPendingReply<>((QRemoteObjectPendingCall)result, int.class);
							if(!pending.isFinished())
								pending.waitForFinished();
							Assert.assertEquals(QRemoteObjectPendingCall.Error.NoError, pending.error());
							Assert.assertEquals(Integer.valueOf(5), pending.returnValue());
						}else {
							Assert.fail("QRemoteObjectPendingCall expected but was "+result);
						}
						result = QMetaObject.invokeMethod(pong, "toRect", 2,3,4,5);
						if(result instanceof QRemoteObjectPendingCall) {
							QRemoteObjectPendingReply<QRect> pending = new QRemoteObjectPendingReply<>((QRemoteObjectPendingCall)result, QRect.class);
							if(!pending.isFinished())
								pending.waitForFinished();
							Assert.assertEquals(QRemoteObjectPendingCall.Error.NoError, pending.error());
							Assert.assertEquals(new QRect(2,3,4,5), pending.returnValue());
						}else {
							Assert.fail("QRemoteObjectPendingCall expected but was "+result);
						}
						result = QMetaObject.invokeMethod(pong, "getIntArray");
						if(result instanceof QRemoteObjectPendingCall) {
							QRemoteObjectPendingReply<int[]> pending = new QRemoteObjectPendingReply<>((QRemoteObjectPendingCall)result, int[].class);
							if(!pending.isFinished())
								pending.waitForFinished();
							Assert.assertEquals(QRemoteObjectPendingCall.Error.NoError, pending.error());
							int[] array = pending.returnValue();
							Assert.assertTrue(array!=null);
							Assert.assertEquals(5, array.length);
							Assert.assertEquals(1, array[0]);
							Assert.assertEquals(2, array[1]);
							Assert.assertEquals(3, array[2]);
							Assert.assertEquals(4, array[3]);
							Assert.assertEquals(5, array[4]);
						}else {
							Assert.fail("QRemoteObjectPendingCall expected but was "+result);
						}
						result = QMetaObject.invokeMethod(pong, "getNonQtType");
						if(result instanceof QRemoteObjectPendingCall) {
							QRemoteObjectPendingReply<NonQtType> pending = new QRemoteObjectPendingReply<>((QRemoteObjectPendingCall)result, NonQtType.class);
							if(!pending.isFinished())
								pending.waitForFinished();
							Assert.assertEquals(QRemoteObjectPendingCall.Error.NoError, pending.error());
							NonQtType value = pending.returnValue();
							Assert.assertTrue(value!=null);
							Assert.assertEquals(5, value.i);
							Assert.assertEquals(2.5, value.d, 0.0000001);
							Assert.assertEquals("TEST", value.s);
							Assert.assertEquals(false, value.b);
						}else {
							Assert.fail("QRemoteObjectPendingCall expected but was "+result);
						}
	
						result = QMetaObject.invokeMethod(pong, "getColor");
						if(result instanceof QRemoteObjectPendingCall) {
							QRemoteObjectPendingReply<QColor> pending = new QRemoteObjectPendingReply<>((QRemoteObjectPendingCall)result, QColor.class);
							if(!pending.isFinished())
								pending.waitForFinished();
							Assert.assertEquals(QRemoteObjectPendingCall.Error.NoError, pending.error());
							Assert.assertEquals(new QColor(Qt.GlobalColor.magenta), pending.returnValue());
						}else {
							Assert.fail("QRemoteObjectPendingCall expected but was "+result);
						}
						
						AbstractPublicSignal0 pongSignal = (AbstractPublicSignal0)QMetaObject.findSignal(pong, "pong");
						AbstractPublicSignal0 pingSignal = (AbstractPublicSignal0)QMetaObject.findSignal(pong, "ping");
						Assert.assertTrue(pongSignal!=null);
						Assert.assertTrue(pingSignal!=null);
						boolean[] received = {false,false};
						pongSignal.connect(()->{
							System.out.println("pongSignal received");
							received[0] = true;
						});
						pingSignal.connect(()->{
							System.out.println("pingSignal received");
							received[1] = true;
						});
						QEventLoop loop = new QEventLoop();
						timer.timeout.connect(loop::quit);
						pingSignal.connect(loop::quit);
						timer.start();
						System.out.println("calling remote emitPing...");
						QMetaObject.invokeMethod(pong, "emitPing");
						loop.exec();
						timer.stop();
						timer.timeout.disconnect();
						Assert.assertTrue("pong not received", received[0]);
						Assert.assertTrue("ping not received", received[1]);
						pongSignal.disconnect();
						pingSignal.disconnect();
					}catch(Throwable t) {
						exception[0] = t;
					}finally {
						QCoreApplication.quit();
						QMetaObject.invokeMethod(pong, "quit");
					}
				});
				object.disposeLater();
			});
			QCoreApplication.exec();
		}finally {
			timer.dispose();
			pong.dispose();
			node.dispose();
		}
		if(exception[0]!=null)
			throw exception[0];
    }
	
	@Test
    public void testProperties() {
		class PropertyObject{
			private String text;
			private int number;
			private QColor color;
			
			@QtPropertyReader
			public String text() {
				return text;
			}
			@QtPropertyWriter
			public void setText(String text) {
				this.text = text;
			}
			@QtPropertyReader
			public int number() {
				return number;
			}
			@QtPropertyWriter
			public void setNumber(int number) {
				this.number = number;
			}
			@QtPropertyReader
			public QColor color() {
				return color;
			}
			@QtPropertyWriter
			public void setColor(QColor color) {
				this.color = color;
			}
		}
		PropertyObject po = new PropertyObject();
		po.setText("test");
		po.setNumber(5);
		po.setColor(QColorConstants.Svg.aquamarine());
		QByteArray data = new QByteArray();
		QtRemoteObjects.copyStoredProperties(po, new QDataStream(data, QIODevice.OpenModeFlag.WriteOnly));
		PropertyObject po2 = new PropertyObject();
		QtRemoteObjects.copyStoredProperties(new QDataStream(data, QIODevice.OpenModeFlag.ReadOnly), po2);
		Assert.assertEquals(po.text(), po2.text());
		Assert.assertEquals(po.number(), po2.number());
		Assert.assertEquals(po.color(), po2.color());
	}
}
