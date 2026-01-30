/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

import org.junit.Assert;
import org.junit.Test;

import io.qt.Nullable;
import io.qt.core.*;

public class TestLockers extends ApplicationInitializer {
	
    @Test
    public void testSemaphore() {
    	QSemaphore semaphore = new QSemaphore(1);
    	Assert.assertEquals(1, semaphore.available());
    	Assert.assertTrue(semaphore.tryAcquire(1));
    	Assert.assertEquals(0, semaphore.available());
    	Assert.assertFalse(semaphore.tryAcquire(1));
    	try(QSemaphoreReleaser releaser = new QSemaphoreReleaser(semaphore, 5)){
    		Assert.assertEquals(0, semaphore.available());
    		Assert.assertFalse(semaphore.tryAcquire(1));
    		Assert.assertEquals(semaphore, releaser.semaphore());
    	}
    	Assert.assertEquals(5, semaphore.available());
    	Assert.assertTrue(semaphore.tryAcquire(5));
    	Assert.assertEquals(0, semaphore.available());
    	Assert.assertFalse(semaphore.tryAcquire(1));
    }
    
    @Test
    public void testReadWriteLock() {
    	QReadWriteLock rwlock = new QReadWriteLock();
    	if(rwlock.tryLockForRead()) {
    		rwlock.unlock();
    	}else {
    		Assert.fail("tryLockForRead expected to be true");
    	}
    	try(QReadLocker locker = new QReadLocker(rwlock)){
    		Assert.assertFalse(rwlock.tryLockForWrite());
    		locker.unlock();
    		if(rwlock.tryLockForWrite()) {
        		rwlock.unlock();
        	}else {
        		Assert.fail("tryLockForWrite expected to be true");
        	}
    		locker.relock();
    		Assert.assertFalse(rwlock.tryLockForWrite());
    		Assert.assertEquals(rwlock, locker.readWriteLock());
    	}
    	try(QWriteLocker locker = new QWriteLocker(rwlock)){
    		Assert.assertFalse(rwlock.tryLockForRead());
    		locker.unlock();
    		if(rwlock.tryLockForRead()) {
        		rwlock.unlock();
        	}else {
        		Assert.fail("tryLockForRead expected to be true");
        	}
    		locker.relock();
    		Assert.assertFalse(rwlock.tryLockForRead());
    		Assert.assertEquals(rwlock, locker.readWriteLock());
    	}
    	if(rwlock.tryLockForWrite()) {
    		rwlock.unlock();
    	}else {
    		Assert.fail("tryLockForWrite expected to be true");
    	}
    }
    
	@Test
    public void testMutex() {
    	QMutex mutex = new QMutex();
    	if(mutex.tryLock()) {
    		mutex.unlock();
    	}else {
    		Assert.fail("tryLock expected to be true");
    	}
    	try(QMutexLocker locker = new QMutexLocker(mutex)){
    		Assert.assertFalse(mutex.tryLock());
    		locker.unlock();
    		if(mutex.tryLock()) {
    			mutex.unlock();
        	}else {
        		Assert.fail("tryLock expected to be true");
        	}
    		locker.relock();
    		Assert.assertFalse(mutex.tryLock());
    		QMutex lockersMutex = locker.mutex();
    		try {
				@SuppressWarnings("unused")
				QRecursiveMutex lockersRMutex = locker.mutex();
				Assert.fail("ClassCastException expected to be thrown");
			} catch (ClassCastException e) {
			}
    		Assert.assertTrue(mutex==lockersMutex);
    	}
    }
    
    @Test
    public void testRecursiveMutex() {
    	QRecursiveMutex mutex = new QRecursiveMutex();
    	if(mutex.tryLock()) {
    		mutex.unlock();
    	}else {
    		Assert.fail("tryLock expected to be true");
    	}
    	QThread thread = new QThread();
    	QObject context = new QObject();
    	context.moveToThread(thread);
    	thread.start();
    	try(QMutexLocker locker = new QMutexLocker(mutex)){
    		Assert.assertFalse(QMetaObject.invokeMethod(context, ()->mutex.tryLock(), Qt.ConnectionType.BlockingQueuedConnection));
    		locker.unlock();
    		if(mutex.tryLock()) {
    			mutex.unlock();
        	}else {
        		Assert.fail("tryLock expected to be true");
        	}
    		locker.relock();
    		Assert.assertFalse(QMetaObject.invokeMethod(context, ()->mutex.tryLock(), Qt.ConnectionType.BlockingQueuedConnection));
    		try {
				@SuppressWarnings("unused")
	    		QMutex lockersMutex = locker.mutex();
				Assert.fail("ClassCastException expected to be thrown");
			} catch (ClassCastException e) {
			}
    		QRecursiveMutex lockersRMutex = locker.mutex();
    		Assert.assertTrue(mutex==lockersRMutex);
    	}finally {
    		thread.quit();
    	}
    }
    
    @Test
    public void testSignalBlocker() {
    	QObject object = new QObject();
    	String[] strg = new String[1];
    	object.objectNameChanged.connect(n->strg[0]=n, Qt.ConnectionType.DirectConnection);
    	object.setObjectName("Test1");
    	Assert.assertEquals("Test1", strg[0]);
    	try(QSignalBlocker blocker = new QSignalBlocker(object)){
        	object.setObjectName("Test2");
        	Assert.assertEquals("Test1", strg[0]);
        	blocker.unblock();
        	object.setObjectName("Test3");
        	Assert.assertEquals("Test3", strg[0]);
        	blocker.reblock();
        	object.setObjectName("Test4");
        	Assert.assertEquals("Test3", strg[0]);
    	}
    	object.setObjectName("Test5");
    	Assert.assertEquals("Test5", strg[0]);
    }
    
    @Test
    public void testEventLoop() {
    	QEventLoop loop = new QEventLoop();
    	QObject obj = new QObject() {
			@Override
			public boolean event(@Nullable QEvent event) {
				try(QEventLoopLocker locker = new QEventLoopLocker(loop)){
		    		//should quit event loop
		    	}
				return super.event(event);
			}
    	};
    	QCoreApplication.postEvent(obj, new QEvent(QEvent.Type.User));
    	boolean[] fail = {false};
    	QTimer.singleShot(2000, loop, ()->{
    		loop.quit();
    		fail[0] = true;
    	});
    	loop.quit();
    	obj.dispose();
    	loop.dispose();
    	Assert.assertFalse("QEventLoopLocker did not quit event loop", fail[0]);
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestLockers.class.getName());
    }
}
