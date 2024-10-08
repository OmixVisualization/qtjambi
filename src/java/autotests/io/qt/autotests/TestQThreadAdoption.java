/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.concurrent.atomic.AtomicInteger;

import org.junit.Assert;
import org.junit.BeforeClass;

import io.qt.QNoNativeResourcesException;
import io.qt.QtUtilities;
import io.qt.autotests.generated.General;
import io.qt.autotests.generated.ThreadFactory;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QEventLoop;
import io.qt.core.QMetaObject;
import io.qt.core.QObject;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QThread;
import io.qt.core.QTimer;
import io.qt.core.Qt;

public class TestQThreadAdoption extends ApplicationInitializer{
	
	private static Qt.ConnectionType singleShotConnection = Qt.ConnectionType.DirectConnection;
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitialize();
		try {
			singleShotConnection = Qt.ConnectionType.valueOf("SingleShotConnection");
		}catch(Throwable t) {}
    }
	
	private void initializeThread(QThread[] qthreads, Thread[] jthreads, int[] ownerships, AtomicInteger running, AtomicInteger gcQtThreadObjectsCounter, AtomicInteger gcJavaThreadObjectsCounter, boolean[] deletedQtThreadObjects, AtomicInteger deletedQtThreadObjectsCounter, int i, boolean isAdopted) {
		QThread currentQThread = QThread.currentThread();
		Thread currentThread = Thread.currentThread();
		qthreads[i] = currentQThread;
		jthreads[i] = currentThread;
		if(General.internalAccess.isJavaOwnership(qthreads[i]))
			ownerships[i] = 1;
		else if(General.internalAccess.isCppOwnership(qthreads[i]))
			ownerships[i] = 2;
		else if(General.internalAccess.isSplitOwnership(qthreads[i]))
			ownerships[i] = 3;
		if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Windows) || !isAdopted) {
			QMetaObject.Connection[] connection = {null};
			connection[0] = qthreads[i].destroyed.connect(()->{
					deletedQtThreadObjects[i]=true; 
					deletedQtThreadObjectsCounter.incrementAndGet();
					QObject.disconnect(connection[0]);
				}, Qt.ConnectionType.DirectConnection, singleShotConnection);
		}else {
			QtUtilities.getSignalOnDispose(qthreads[i]).connect(()->{
					deletedQtThreadObjects[i]=true; 
					deletedQtThreadObjectsCounter.incrementAndGet();
				}, Qt.ConnectionType.DirectConnection, singleShotConnection);
		}
		@SuppressWarnings("unused")
		Object cleanable = General.internalAccess.registerCleaner(qthreads[i], gcQtThreadObjectsCounter::incrementAndGet);
		cleanable = General.internalAccess.registerCleaner(jthreads[i], gcJavaThreadObjectsCounter::incrementAndGet);
		cleanable = null;
		String threadStrg = qthreads[0].toString();
		QObject deleteOnFinish = new QObject();
		{
			QMetaObject.Connection[] connection = {null};
			connection[0] = deleteOnFinish.destroyed.connect(()->{
				QThread _currentThread = QThread.currentThread();
				System.out.println("Object destroyed in thread " + _currentThread);
				_currentThread = null;
				QObject.disconnect(connection[0]);
				}, Qt.ConnectionType.DirectConnection, singleShotConnection);
		}
		{
			QMetaObject.Connection[] connection = {null};
			connection[0] = qthreads[i].finished.connect(()->{
				QThread _currentThread = QThread.currentThread();
				System.out.println(threadStrg+" finished in thread " + _currentThread);
				_currentThread = null;
				deleteOnFinish.dispose();
				QObject.disconnect(connection[0]);
			}, Qt.ConnectionType.DirectConnection, singleShotConnection);
		}
		running.incrementAndGet();
		currentQThread = null;
		currentThread = null;
	}
	
	@org.junit.Test
	public void testAdoptedThreads() throws InterruptedException {
		AtomicInteger running = new AtomicInteger(0);
		AtomicInteger gcQtThreadObjectsCounter = new AtomicInteger(0);
		AtomicInteger gcJavaThreadObjectsCounter = new AtomicInteger(0);
		AtomicInteger deletedQtThreadObjectsCounter = new AtomicInteger(0);
		boolean[] deletedQtThreadObjects = new boolean[8];
		class Stopper extends QObject{
			public final Signal0 stop = new Signal0();
		}
		QThread[] qthreads = new QThread[8];
		Thread[] jthreads = new Thread[8];
		int[] ownerships = new int[8];
		Stopper stopper = new Stopper();
		int c = 0;
		// QThread 0 is natively started looping Qt thread adopted by Java
		QThread thread0;
		{
			QThread thread = thread0 = ThreadFactory.startQLoopThread();
			int i=c++;
			Assert.assertTrue("QThread "+i+" has not split ownwership", General.internalAccess.isSplitOwnership(thread));
			thread.setObjectName("thread"+i);
			QObject obj = new QObject();
			obj.destroyed.connect(()->initializeThread(qthreads, jthreads, ownerships, running, gcQtThreadObjectsCounter, gcJavaThreadObjectsCounter, deletedQtThreadObjects, deletedQtThreadObjectsCounter, i, false), Qt.ConnectionType.DirectConnection, singleShotConnection);
			obj.moveToThread(thread);
			obj.disposeLater();
			stopper.stop.connect(thread::quit);
			thread = null;
		}
		// QThread 1 is natively started Qt thread adopted by Java
		QThread thread1;
		{
			int i=c++;
			QThread thread = thread1 = ThreadFactory.startQWorkerThread(()->{
				initializeThread(qthreads, jthreads, ownerships, running, gcQtThreadObjectsCounter, gcJavaThreadObjectsCounter, deletedQtThreadObjects, deletedQtThreadObjectsCounter, i, false);
				QEventLoop loop = new QEventLoop();
				stopper.stop.connect(loop::quit);
				loop.exec();
				loop.dispose();
			});
			Assert.assertTrue("QThread "+i+" has not split ownwership", General.internalAccess.isSplitOwnership(thread));
			thread.setObjectName("thread"+i);
			thread = null;
		}
		// QThread 2 is natively started C++ thread adopted by Qt and by Java
		{
			int i=c++;
			ThreadFactory.startNativeWorkerThread(()->{
				initializeThread(qthreads, jthreads, ownerships, running, gcQtThreadObjectsCounter, gcJavaThreadObjectsCounter, deletedQtThreadObjects, deletedQtThreadObjectsCounter, i, true);
				QEventLoop loop = new QEventLoop();
				stopper.stop.connect(loop::quit);
				loop.exec();
				loop.dispose();
			});
		}
		// QThread 3 is Java thread adopted by Qt
		Thread thread3;
		{
			int i=c++;
			Thread thread = thread3 = new Thread(()->{
				initializeThread(qthreads, jthreads, ownerships, running, gcQtThreadObjectsCounter, gcJavaThreadObjectsCounter, deletedQtThreadObjects, deletedQtThreadObjectsCounter, i, false);
				QEventLoop loop = new QEventLoop();
				stopper.stop.connect(loop::quit);
				loop.exec();
				loop.dispose();
			});
			thread.setName("thread"+i);
			thread.start();
			thread = null;
		}
		// QThread 4 is Java-started Qt thread adopted by Java
		QThread thread4;
		{
			int i=c++;
			QThread thread = thread4 = QThread.create(()->{
				initializeThread(qthreads, jthreads, ownerships, running, gcQtThreadObjectsCounter, gcJavaThreadObjectsCounter, deletedQtThreadObjects, deletedQtThreadObjectsCounter, i, false);
				QEventLoop loop = new QEventLoop();
				stopper.stop.connect(loop::quit);
				loop.exec();
				loop.dispose();
			});
			Assert.assertTrue("QThread "+i+" has not java ownwership", General.internalAccess.isJavaOwnership(thread));
			thread.setName("thread"+i);
			thread.start();
			Assert.assertTrue("QThread "+i+" has not java ownwership", General.internalAccess.isJavaOwnership(thread));
			thread = null;
		}
		// QThread 5 is Java-started looping Qt thread adopted by Java
		QThread thread5;
		{
			int i=c++;
			QThread thread = thread5 = new QThread();
			thread.setObjectName("thread"+i);
			{
				QObject obj = new QObject();
				obj.destroyed.connect(()->initializeThread(qthreads, jthreads, ownerships, running, gcQtThreadObjectsCounter, gcJavaThreadObjectsCounter, deletedQtThreadObjects, deletedQtThreadObjectsCounter, i, false));
				obj.moveToThread(thread);
				obj.disposeLater();
			}
			Assert.assertTrue("QThread "+i+" has not java ownwership", General.internalAccess.isJavaOwnership(thread));
			thread.start();
			stopper.stop.connect(thread::quit);
			Assert.assertTrue("QThread "+i+" has not java ownwership", General.internalAccess.isJavaOwnership(thread));
			thread = null;
		}
		// QThread 6 is natively created looping Qt thread adopted by Java
		QThread thread6;
		{
			int i=c++;
			QThread thread = thread6 = ThreadFactory.createQLoopThread();
			thread.setObjectName("thread"+i);
			{
				QObject obj = new QObject();
				obj.destroyed.connect(()->initializeThread(qthreads, jthreads, ownerships, running, gcQtThreadObjectsCounter, gcJavaThreadObjectsCounter, deletedQtThreadObjects, deletedQtThreadObjectsCounter, i, false));
				obj.moveToThread(thread);
				obj.disposeLater();
			}
			Assert.assertTrue("QThread "+i+" has not java ownwership", General.internalAccess.isJavaOwnership(thread));
			thread.start();
			stopper.stop.connect(thread::quit);
			Assert.assertTrue("QThread "+i+" has not java ownwership", General.internalAccess.isJavaOwnership(thread));
			thread = null;
		}
		// QThread 7 is natively created Qt thread adopted by Java
		QThread thread7;
		{
			int i=c++;
			QThread thread = thread7 = ThreadFactory.createQWorkerThread(()->{
				initializeThread(qthreads, jthreads, ownerships, running, gcQtThreadObjectsCounter, gcJavaThreadObjectsCounter, deletedQtThreadObjects, deletedQtThreadObjectsCounter, i, false);
				QEventLoop loop = new QEventLoop();
				stopper.stop.connect(loop::quit);
				loop.exec();
				loop.dispose();
			});
			Assert.assertTrue("QThread "+i+" has not java ownwership", General.internalAccess.isJavaOwnership(thread));
			thread.setObjectName("thread"+i);
			thread.start();
			Assert.assertTrue("QThread "+i+" has not java ownwership", General.internalAccess.isJavaOwnership(thread));
			thread = null;
		}
		
		int i=0;
		while(running.get()<qthreads.length && i<100) {
			Thread.yield();
			Thread.sleep(150);
			runGC();
			++i;
		}
		Assert.assertEquals(qthreads.length, running.get());
		Set<Thread> runningThreads = Thread.getAllStackTraces().keySet();
		for (int j = 0; j < jthreads.length; j++) {
			Assert.assertTrue(jthreads[j]!=null);
			Assert.assertTrue(runningThreads.contains(jthreads[j]));
			Assert.assertEquals(Thread.State.RUNNABLE, jthreads[j].getState());
			Assert.assertEquals("Java thread "+j+" is not associated to Qt thread "+j, qthreads[j], QThread.thread(jthreads[j]));
			Assert.assertEquals("Qt thread "+j+" is not associated to Java thread "+j, jthreads[j], qthreads[j].javaThread());
		}
		Assert.assertEquals(thread0, qthreads[0]);
		Assert.assertEquals(thread1, qthreads[1]);
		Assert.assertEquals(thread3, jthreads[3]);
		Assert.assertEquals(thread4, qthreads[4]);
		Assert.assertEquals(thread5, qthreads[5]);
		Assert.assertEquals(thread6, qthreads[6]);
		Assert.assertEquals(thread7, qthreads[7]);
		thread0 = null;
		thread1 = null;
		thread3 = null;
		thread4 = null;
		thread5 = null;
		thread6 = null;
		thread7 = null;
		for (int j = 0; j < qthreads.length; j++) {
			Assert.assertTrue("QThread "+j+" is null", qthreads[j]!=null);
			if(j==2 || j==3)
				Assert.assertEquals("QThread "+j+" has wrong thread-affinity", qthreads[j], qthreads[j].thread());
			else
				Assert.assertEquals("QThread "+j+" has wrong thread-affinity", QThread.currentThread(), qthreads[j].thread());
			Assert.assertFalse("QThread "+j+" is disposed", qthreads[j].isDisposed());
			Assert.assertTrue("QThread "+j+" not alive", qthreads[j].isAlive());
			Assert.assertTrue("QThread "+j+" not running", qthreads[j].isRunning());
			Assert.assertFalse("QThread "+j+" is finished", qthreads[j].isFinished());
			Assert.assertEquals(jthreads[j], qthreads[j].javaThread());
			Assert.assertFalse("QThread "+j+" has Cpp ownwership", General.internalAccess.isCppOwnership(qthreads[j]));
			if(j<4) {
				Assert.assertTrue("running QThread "+j+" has not split ownwership", ownerships[j]==3);
			}else {
				Assert.assertTrue("running QThread "+j+" has not Java ownwership", ownerships[j]==1);
			}
		}
		stopper.stop.emit();
		stopper.stop.disconnect();
		
		i = 0;
		while(i<20) {
			Thread.yield();
			Thread.sleep(150);
			runGC();
			++i;
		}
		for (int j = 0; j < qthreads.length; j++) {
			try {
				if(qthreads[j].isRunning()) {
					i = 0;
					while(i<20 && qthreads[j].isRunning()) {
						Thread.yield();
						Thread.sleep(150);
						runGC();
						++i;
					}
				}
			}catch(Throwable t){}
		}
		runningThreads = Thread.getAllStackTraces().keySet();
		for (int j = 0; j < jthreads.length; j++) {
			if(!QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android))
				Assert.assertEquals("post-mortem Java thread "+j+" state", Thread.State.TERMINATED, jthreads[j].getState());
			else
				Assert.assertTrue("post-mortem Java thread "+j+" state==RUNNABLE", Thread.State.RUNNABLE!=jthreads[j].getState());
			Assert.assertFalse("post-mortem Java thread "+j+" is still running", runningThreads.contains(jthreads[j]));
			if(!qthreads[j].isDisposed()) {
				Assert.assertEquals("post-mortem Java thread "+j+" is not associated to Qt thread "+j, qthreads[j], QThread.thread(jthreads[j]));
				Assert.assertEquals("post-mortem Qt thread "+j+" is not associated to Java thread "+j, jthreads[j], qthreads[j].javaThread());
			}else {
				Assert.assertEquals("post-mortem Java thread "+j+" is still associated to Qt thread", null, QThread.thread(jthreads[j]));
			}
			jthreads[j] = null;
		}
		runningThreads = null;
		for (int j = 0; j < qthreads.length; j++) {
			Assert.assertTrue("QThread "+j+" is null", qthreads[j]!=null);
			try {
				// threads2 and thread3 might already be deleted or
				// are deleted in parallel to this test
				Assert.assertFalse("post-mortem QThread "+j+" still alive", qthreads[j].isAlive());
				Assert.assertFalse("post-mortem QThread "+j+" still running", qthreads[j].isRunning());
				Assert.assertTrue("post-mortem QThread "+j+" not finished", qthreads[j].isFinished());
				switch(ownerships[j]) {
				case 1:
					Assert.assertTrue("post-mortem QThread "+j+" not Java ownership", General.internalAccess.isJavaOwnership(qthreads[j]));
					break;
				case 2:
					Assert.assertTrue("post-mortem QThread "+j+" not Cpp ownership", General.internalAccess.isCppOwnership(qthreads[j]));
					break;
				case 3:
					Assert.assertTrue("post-mortem QThread "+j+" not split ownership", General.internalAccess.isSplitOwnership(qthreads[j]));
					break;
				}
			}catch(QNoNativeResourcesException e){
				if(j!=2 && j!=3) {
					throw e;
				}
			}
			qthreads[j] = null;
		}
		QEventLoop loop = new QEventLoop();
		{
			QTimer timer = new QTimer();
			long t1 = System.currentTimeMillis();
			timer.timeout.connect(()->{
				Thread.yield();
				Thread.sleep(250);
				runGC();
				QCoreApplication.processEvents();
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
				if(deletedQtThreadObjectsCounter.get()==qthreads.length || System.currentTimeMillis()-t1>15000) {
					loop.quit();
				}
			});
			timer.setInterval(300);
			timer.start();
			loop.exec();
			timer.dispose();
		}
		{
			QTimer timer = new QTimer();
			long t1 = System.currentTimeMillis();
			timer.timeout.connect(()->{
				Thread.yield();
				Thread.sleep(250);
				runGC();
				QCoreApplication.processEvents();
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
				if(gcQtThreadObjectsCounter.get()==qthreads.length || System.currentTimeMillis()-t1>15000) {
					loop.quit();
				}
			});
			timer.setInterval(300);
			timer.start();
			loop.exec();
			timer.dispose();
		}
		{
			QTimer timer = new QTimer();
			long t1 = System.currentTimeMillis();
			timer.timeout.connect(()->{
				Thread.yield();
				Thread.sleep(250);
				runGC();
				QCoreApplication.processEvents();
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
				if(gcJavaThreadObjectsCounter.get()==jthreads.length || System.currentTimeMillis()-t1>15000) {
					loop.quit();
				}
			});
			timer.setInterval(300);
			timer.start();
			loop.exec();
			timer.dispose();
		}
		List<String> threadNames = new ArrayList<>();
		for (int j = 0; j < deletedQtThreadObjects.length; j++) {
			if(!deletedQtThreadObjects[j])
				threadNames.add(""+j);
		}
		Assert.assertEquals("Number of GC-removed Java thread objects", jthreads.length, gcJavaThreadObjectsCounter.get());
		Assert.assertTrue("QThreads have not been deleted: "+String.join(", ", threadNames), threadNames.isEmpty());
		Assert.assertEquals("Number of GC-removed QThread objects", qthreads.length, gcQtThreadObjectsCounter.get());
	}
}
