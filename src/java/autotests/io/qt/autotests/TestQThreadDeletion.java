/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import static org.junit.Assert.assertTrue;

import java.util.concurrent.atomic.AtomicBoolean;

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtUtilities;
import io.qt.autotests.generated.General;
import io.qt.autotests.generated.SignalReceiver;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QObject;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QThread;
import io.qt.core.Qt;

public class TestQThreadDeletion extends ApplicationInitializer {
	@BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitialize();
    }
	
	@Test
    public void testJavaThreadDeletion() throws InterruptedException {
		boolean[] splitOwnership = {false};
    	AtomicBoolean qthreadDisposed = new AtomicBoolean();
		AtomicBoolean qobjectDisposed = new AtomicBoolean();
		AtomicBoolean qthreadCleaned = new AtomicBoolean();
		SignalReceiver qthreadFinished = new SignalReceiver();
		SignalReceiver qthreadDestroyed = new SignalReceiver();
		AtomicBoolean qobjectDestroyed = new AtomicBoolean();
		AtomicBoolean threadCleaned = new AtomicBoolean();
		AtomicBoolean qobjectCleaned = new AtomicBoolean();
    	{
    		class MyThread extends Thread{
				public MyThread(Runnable target, String name) {
					super(target, name);
				}
    		}
	    	Thread thread = new MyThread(()->{
	    		try {
	    			QThread qthread = QThread.currentThread();
	    			qthread.finished.connect(qthreadFinished, "receiveSignal()", Qt.ConnectionType.DirectConnection);
	    			splitOwnership[0] = General.internalAccess.isSplitOwnership(qthread);
	    			QtUtilities.getSignalOnDispose(qthread).connect(()->qthreadDisposed.set(true), Qt.ConnectionType.DirectConnection);
					qthread.destroyed.connect(qthreadDestroyed, "receiveSignal()", Qt.ConnectionType.DirectConnection);
	    			General.internalAccess.registerCleaner(qthread, ()->qthreadCleaned.set(true));
					QObject object = new QObject();
					QtUtilities.getSignalOnDispose(object).connect(()->qobjectDisposed.set(true), Qt.ConnectionType.DirectConnection);
					object.destroyed.connect(()->qobjectDestroyed.set(true), Qt.ConnectionType.DirectConnection);
					General.internalAccess.registerCleaner(object, ()->qobjectCleaned.set(true));
					object = null;
					qthread = null;
				} catch (Throwable e) {
					e.printStackTrace();
				}
	    	}, "\0\0");
	    	General.internalAccess.registerCleaner(thread, ()->threadCleaned.set(true));
	    	thread.setDaemon(true);
	    	thread.start();
	    	thread = null;
    	}
		Thread.yield();
		ApplicationInitializer.runGC();
		Thread.sleep(100);
		for (int i = 0; i < 500; i++) {
			if(qthreadFinished.received())
    			break;
    		Thread.yield();
    		Thread.sleep(100);
    		runGC();
    		QCoreApplication.processEvents();
    		QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		}
		assertTrue("QThread is not finished", qthreadFinished.received());
		assertTrue("QThread has not split ownership", splitOwnership[0]);
    	for (int i = 0; i < 50; i++) {
    		if(qobjectCleaned.get()) {
        		QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
        		Thread.yield();
        		Thread.sleep(100);
    			break;
    		}
    		Thread.yield();
    		Thread.sleep(100);
    		runGC();
    		QCoreApplication.processEvents();
    		QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		}
    	assertTrue("QObject is not cleaned", qobjectCleaned.get());
    	assertTrue("QObject is not disposed", qobjectDisposed.get());
    	assertTrue("QObject is not destroyed", qobjectDestroyed.get());
    	for (int i = 0; i < 50; i++) {
    		if(threadCleaned.get()) {
        		QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
        		Thread.yield();
        		Thread.sleep(100);
    			break;
    		}
    		Thread.yield();
    		Thread.sleep(100);
    		runGC();
    		QCoreApplication.processEvents();
    		QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		}
    	assertTrue("Thread is not cleaned", threadCleaned.get());
    	assertTrue("QThread is not disposed", qthreadDisposed.get());
    	for (int i = 0; i < 50; i++) {
    		if(qthreadDestroyed.received()) {
        		QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
        		Thread.yield();
        		Thread.sleep(100);
    			break;
    		}
    		Thread.yield();
    		Thread.sleep(100);
    		runGC();
    		QCoreApplication.processEvents();
    		QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		}
    	for (int i = 0; i < 50; i++) {
    		if(qthreadCleaned.get()) {
        		QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
        		Thread.yield();
        		Thread.sleep(100);
    			break;
    		}
    		Thread.yield();
    		Thread.sleep(100);
    		runGC();
    		QCoreApplication.processEvents();
    		QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		}
    	assertTrue("QThread is not cleaned", qthreadCleaned.get());
    	if(!QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Windows)
    			|| !(System.getProperty("java.version", "").startsWith("1.8") 
    					|| System.getProperty("java.version", "").startsWith("8"))) {
        	assertTrue("QThread is not destroyed", qthreadDestroyed.received());
    	}
    }
    
    @Test
    public void testQThreadDeletion() throws InterruptedException {
		AtomicBoolean threadDisposed = new AtomicBoolean();
		AtomicBoolean qobjectDisposed = new AtomicBoolean();
		AtomicBoolean threadDestroyed = new AtomicBoolean();
		AtomicBoolean qobjectDestroyed = new AtomicBoolean();
    	{
	    	QThread thread = QThread.create(()->{
	    		QObject object = new QObject();
	    		QtUtilities.getSignalOnDispose(object).connect(()->qobjectDisposed.set(true), Qt.ConnectionType.DirectConnection);
	    		object.destroyed.connect(()->qobjectDestroyed.set(true), Qt.ConnectionType.DirectConnection);
	    	});
	    	QtUtilities.getSignalOnDispose(thread).connect(()->threadDisposed.set(true), Qt.ConnectionType.DirectConnection);
    		thread.destroyed.connect(()->threadDestroyed.set(true), Qt.ConnectionType.DirectConnection);
	    	thread.setDaemon(true);
	    	thread.start();
    	}
		Thread.yield();
		ApplicationInitializer.runGC();
		Thread.sleep(100);
    	for (int i = 0; i < 200; i++) {
    		if(threadDisposed.get())
    			i = 200;
    		Thread.yield();
    		ApplicationInitializer.runGC();
    		Thread.sleep(10);
        	QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		}
    	Thread.sleep(100);
		ApplicationInitializer.runGC();
		Thread.yield();
    	QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
    	assertTrue("QObject is not disposed", qobjectDisposed.get());
    	assertTrue("QObject is not destroyed", qobjectDestroyed.get());
    	assertTrue("QThread is not disposed", threadDisposed.get());
    	for (int i = 0; i < 200; i++) {
    		if(threadDestroyed.get())
    			i = 200;
    		Thread.yield();
    		ApplicationInitializer.runGC();
    		Thread.sleep(10);
        	QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		}
    	assertTrue("QThread is not destroyed", threadDestroyed.get());
    }
    
    @Test
    public void testEventThreadDeletion() throws InterruptedException {
    	AtomicBoolean threadFinished = new AtomicBoolean();
		AtomicBoolean threadDisposed = new AtomicBoolean();
		AtomicBoolean qobjectDisposed = new AtomicBoolean();
		AtomicBoolean threadDestroyed = new AtomicBoolean();
		AtomicBoolean qobjectDestroyed = new AtomicBoolean();
    	{
	    	QThread thread = new QThread() {};
    		QObject object = new QObject();
    		General.internalAccess.registerCleaner(object, ()->qobjectDisposed.set(true));
    		object.destroyed.connect(()->qobjectDestroyed.set(true), Qt.ConnectionType.DirectConnection);
    		object.destroyed.connect(thread::quit, Qt.ConnectionType.DirectConnection);
    		object.moveToThread(thread);
    		object.disposeLater();
    		
    		General.internalAccess.registerCleaner(thread, ()->threadDisposed.set(true));
    		thread.destroyed.connect(()->threadDestroyed.set(true), Qt.ConnectionType.DirectConnection);
	    	thread.setDaemon(true);
	    	thread.finished.connect(()->threadFinished.set(true), Qt.ConnectionType.DirectConnection);
	    	thread.start();
	    	thread = null;
	    	object = null;
    	}
		Thread.yield();
		Thread.sleep(100);
		ApplicationInitializer.runGC();
    	for (int i = 0; i < 200; i++) {
    		if(threadFinished.get())
    			i = 200;
    		Thread.yield();
    		ApplicationInitializer.runGC();
    		Thread.sleep(10);
        	QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		}
    	for (int i = 0; i < 200; i++) {
    		if(threadDisposed.get())
    			i = 200;
    		Thread.yield();
    		ApplicationInitializer.runGC();
    		Thread.sleep(10);
        	QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		}
    	assertTrue("QObject is not disposed", qobjectDisposed.get());
    	assertTrue("QObject is not destroyed", qobjectDestroyed.get());
    	assertTrue("QThread is not disposed", threadDisposed.get());
    	for (int i = 0; i < 200; i++) {
    		if(threadDestroyed.get())
    			i = 200;
    		Thread.yield();
    		ApplicationInitializer.runGC();
    		Thread.sleep(10);
        	QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		}
    	assertTrue("QThread is not destroyed", threadDestroyed.get());
    }
}
