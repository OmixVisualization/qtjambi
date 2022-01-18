/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.autotests.QtRemoteObjectsPong.NonQtType;
import io.qt.core.QCoreApplication;
import io.qt.core.QEventLoop;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaObject.AbstractPublicSignal0;
import io.qt.core.QMetaType;
import io.qt.core.QObject;
import io.qt.core.QRect;
import io.qt.core.QTimer;
import io.qt.core.QUrl;
import io.qt.core.Qt;
import io.qt.gui.QColor;
import io.qt.remoteobjects.QRemoteObjectDynamicReplica;
import io.qt.remoteobjects.QRemoteObjectNode;
import io.qt.remoteobjects.QRemoteObjectPendingCall;
import io.qt.remoteobjects.QRemoteObjectPendingReply;

public class TestRemoteObjects {
	
	private static Throwable exception = null;
	private static Thread pongThread;
	
	@BeforeClass
	public static void testInitialize() throws Throwable {
		pongThread = new Thread(() -> {
			try {
				System.out.println("Deploying QtRemoteObjectsPong...");
				QApplicationTest.testDeployerApp("QtRemoteObjectsPong", "network", "remoteobjects");
			} catch (InterruptedException e) {
			} catch (Throwable e) {
				exception = e;
			}
		});
		pongThread.start();
		pongThread.join(2000);
		File testFile = new File(new File(new File(new File(System.getProperty("user.dir"), "build"), "tests"), "tmp"), "touch.test");
		while(!testFile.exists() && pongThread.isAlive()){
			pongThread.join(2000);
		}
		if (exception != null)
			throw exception;
		QApplicationTest.testInitialize();
	}
	
	@AfterClass
    public static void testDispose() throws Throwable {
		if(pongThread!=null) {
			pongThread.interrupt();
			pongThread.join();
			pongThread = null;
		}
		if (exception != null)
			throw exception;
		QApplicationTest.testDispose();
	}
	
	@Test
    public void test() throws Throwable
    {
		QMetaType.registerMetaType(NonQtType.class);
		QRemoteObjectNode node = new QRemoteObjectNode(new QUrl("local:ropong"));
		QRemoteObjectDynamicReplica pong = node.acquireDynamic("pong");
		QTimer timer = new QTimer();
		timer.setSingleShot(true);
		timer.setInterval(10000);
		timer.timeout.connect(QCoreApplication::quit);
		timer.start();
		Throwable exception[] = {new AssertionError("Replica not initialized")};
		pong.initialized.connect(()->{
			exception[0] = null;
			timer.stop();
			timer.timeout.disconnect();
			QObject object = new QObject();
			object.destroyed.connect(()->{
				try {
					AbstractPublicSignal0 pongSignal = (AbstractPublicSignal0)QMetaObject.findSignal(pong, "pong");
					AbstractPublicSignal0 pingSignal = (AbstractPublicSignal0)QMetaObject.findSignal(pong, "ping");
					Assert.assertTrue(pongSignal!=null);
					Assert.assertTrue(pingSignal!=null);
					boolean[] received = {false,false};
					pongSignal.connect(()->received[0] = true);
					pingSignal.connect(()->received[1] = true);
					QEventLoop loop = new QEventLoop();
					timer.timeout.connect(loop::quit);
					pingSignal.connect(loop::quit);
					timer.start();
					QMetaObject.invokeMethod(pong, "emitPing");
					loop.exec();
					timer.stop();
					timer.timeout.disconnect();
					Assert.assertTrue("pong not received", received[0]);
					Assert.assertTrue("ping not received", received[1]);
					pongSignal.disconnect();
					pingSignal.disconnect();
					
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
		if(exception[0]!=null)
			throw exception[0];
    }
}
