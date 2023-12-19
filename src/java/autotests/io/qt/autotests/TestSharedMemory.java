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
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.concurrent.atomic.AtomicInteger;

import org.junit.Assume;
import org.junit.Before;
import org.junit.Test;

import io.qt.QNativePointer;
import io.qt.core.QDataStream;
import io.qt.core.QIODevice;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QSharedMemory;
import io.qt.core.QThread;

public class TestSharedMemory extends ApplicationInitializer {
    @Before
    public void setUp() {
    	Assume.assumeTrue("Cannot run on macOS or on Android.", 
				!QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.MacOS, 
																QOperatingSystemVersion.OSType.IOS, 
																QOperatingSystemVersion.OSType.WatchOS, 
																QOperatingSystemVersion.OSType.Android));
    }

    @Test(expected=IllegalStateException.class)
    public void test_IllegalState() throws Exception
    {
    	QSharedMemory memory = new QSharedMemory();
    	memory.setKey("test_IllegalState");
    	assertFalse("isAttached", memory.isAttached());
    	try(QSharedMemory.DataAccess access = memory.access()){
    	}    	
    }
    
    @Test
    public void test_createReadOnly() throws Exception
    {
    	QSharedMemory memory = new QSharedMemory();
    	memory.setKey("test_createReadOnly");
    	boolean created = memory.create(1024, QSharedMemory.AccessMode.ReadOnly);
    	assertTrue("create failed: "+memory.errorString(), created);
    	assertTrue("isAttached", memory.isAttached());
    	try(QSharedMemory.DataAccess access = memory.access()){
    		ByteBuffer buffer = access.data();
    		assertTrue("buffer.isReadOnly", buffer.isReadOnly());
    	}
    	memory.detach();
    	assertFalse("isAttached", memory.isAttached());
    }
    
    @Test
    public void test_createReadWrite() throws Exception
    {
    	QSharedMemory memory = new QSharedMemory();
    	memory.setKey("test_createReadWrite");
    	boolean created = memory.create(1024, QSharedMemory.AccessMode.ReadWrite);
    	assertTrue("create failed: "+memory.errorString(), created);
    	assertTrue("isAttached", memory.isAttached());
    	try(QSharedMemory.DataAccess access = memory.access()){
    		ByteBuffer np = access.data();
    		assertFalse("buffer.isReadOnly", np.isReadOnly());
    		np.put((byte)65);
    	}
    	AtomicInteger intValue = new AtomicInteger();
    	Throwable[] throwable = {null};
    	QThread thread = QThread.create(()->{
    		try {
				QSharedMemory memory2 = new QSharedMemory();
				memory2.setKey("test_createReadWrite");
				memory2.attach(QSharedMemory.AccessMode.ReadOnly);
				try(QSharedMemory.DataAccess access = memory2.access()){
					ByteBuffer np = access.data();
					intValue.set(np.get());
				}
				memory2.detach();
			} catch (Throwable e) {
				throwable[0] = e;
			}
    	});
    	thread.start();
    	thread.join(2000);
    	assertEquals(null, throwable[0]);
    	assertFalse(thread.isAlive());
    	assertEquals(65, intValue.intValue());
    	memory.detach();
    	assertFalse("isAttached", memory.isAttached());
    }
    
    @Test
    public void test_createReadWriteQIODeviceFromDirectBuffer() throws Exception
    {
    	QSharedMemory memory = new QSharedMemory();
    	memory.setKey("test_createReadWriteQIODeviceFromDirectBuffer");
    	boolean created = memory.create(1024, QSharedMemory.AccessMode.ReadWrite);
    	assertTrue("create failed: "+memory.errorString(), created);
    	assertTrue("isAttached", memory.isAttached());
    	try(QSharedMemory.DataAccess access = memory.access()){
    		ByteBuffer np = access.data();
    		assertFalse("buffer.isReadOnly", np.isReadOnly());
    		QIODevice device = QIODevice.fromBuffer(np);
    		if(device.open(QIODevice.OpenModeFlag.ReadWrite)) {
    			device.write(new byte[]{1,2,3,4,5,6,7,8,9,10});
    			device.write(new byte[]{31,32,33,34,35,36,37,38,39,40});
	    		device.close();
    		}
    	}
    	byte[] value = new byte[20];
    	Throwable[] throwable = {null};
    	QThread thread = QThread.create(()->{
    		try {
				QSharedMemory memory2 = new QSharedMemory();
				memory2.setKey("test_createReadWriteQIODeviceFromDirectBuffer");
				memory2.attach(QSharedMemory.AccessMode.ReadOnly);
				try(QSharedMemory.DataAccess access = memory2.access()){
					ByteBuffer np = access.data();
					QIODevice device = QIODevice.fromBuffer(np);
					if(device.open(QIODevice.OpenModeFlag.ReadOnly)) {
						device.read(value);
			    		device.close();
					}
				}
				memory2.detach();
			} catch (Throwable e) {
				throwable[0] = e;
			}
    	});
    	thread.start();
    	thread.join(2000);
    	assertEquals(null, throwable[0]);
    	assertFalse(thread.isAlive());
    	assertTrue(Arrays.toString(value), Arrays.equals(new byte[]{1,2,3,4,5,6,7,8,9,10,31,32,33,34,35,36,37,38,39,40}, value));
    	memory.detach();
    	assertFalse("isAttached", memory.isAttached());
    }
    
    @Test
    public void test_createReadWriteQIODeviceFromAddress() throws Exception
    {
    	QSharedMemory memory = new QSharedMemory();
    	memory.setKey("test_createReadWriteQIODeviceFromAddress");
    	boolean created = memory.create(1024, QSharedMemory.AccessMode.ReadWrite);
    	assertTrue("create failed: "+memory.errorString(), created);
    	assertTrue("isAttached", memory.isAttached());
    	try(QSharedMemory.DataAccess access = memory.access()){
    		ByteBuffer np = access.data();
    		assertFalse("buffer.isReadOnly", np.isReadOnly());
    		QIODevice device = QNativePointer.fromBuffer(np).openAsDevice(QIODevice.OpenModeFlag.ReadWrite);
    		if(device!=null) {
    			device.write(new byte[]{1,2,3,4,5,6,7,8,9,10});
    			device.write(new byte[]{31,32,33,34,35,36,37,38,39,40});
	    		device.close();
    		}
    	}
    	byte[] value = new byte[20];
    	Throwable[] throwable = {null};
    	QThread thread = QThread.create(()->{
    		try {
				QSharedMemory memory2 = new QSharedMemory();
				memory2.setKey("test_createReadWriteQIODeviceFromAddress");
				memory2.attach(QSharedMemory.AccessMode.ReadOnly);
				try(QSharedMemory.DataAccess access = memory2.access()){
					ByteBuffer np = access.data();
					QIODevice device = QNativePointer.fromBuffer(np).openAsDevice(QIODevice.OpenModeFlag.ReadOnly);
					if(device!=null) {
						device.read(value);
			    		device.close();
					}
				}
				memory2.detach();
			} catch (Throwable e) {
				throwable[0] = e;
			}
    	});
    	thread.start();
    	thread.join(2000);
    	assertEquals(null, throwable[0]);
    	assertFalse(thread.isAlive());
    	assertTrue(Arrays.toString(value), Arrays.equals(new byte[]{1,2,3,4,5,6,7,8,9,10,31,32,33,34,35,36,37,38,39,40}, value));
    	memory.detach();
    	assertFalse("isAttached", memory.isAttached());
    }
    
    @Test
    public void test_createDataStreamQIODeviceFromAddress() throws Exception
    {
    	QSharedMemory memory = new QSharedMemory();
    	memory.setKey("test_createDataStreamQIODeviceFromAddress");
    	boolean created = memory.create(1024, QSharedMemory.AccessMode.ReadWrite);
    	assertTrue("create failed: "+memory.errorString(), created);
    	assertTrue("isAttached", memory.isAttached());
    	try(QSharedMemory.DataAccess access = memory.access()){
    		ByteBuffer np = access.data();
    		assertFalse("buffer.isReadOnly", np.isReadOnly());
    		QIODevice device = QNativePointer.fromBuffer(np).openAsDevice(QIODevice.OpenModeFlag.ReadWrite);
    		if(device!=null) {
    			QDataStream stream = new QDataStream(device);
    			stream.writeString("TEST");
    			stream.writeString("__TEST__");
    			stream.dispose();
	    		device.close();
    		}
    	}
    	String[] values = {null, null};
    	Throwable[] throwable = {null};
    	QThread thread = QThread.create(()->{
    		try {
				QSharedMemory memory2 = new QSharedMemory();
				memory2.setKey("test_createDataStreamQIODeviceFromAddress");
				memory2.attach(QSharedMemory.AccessMode.ReadOnly);
				try(QSharedMemory.DataAccess access = memory2.access()){
					ByteBuffer np = access.data();
					QIODevice device = QNativePointer.fromBuffer(np).openAsDevice(QIODevice.OpenModeFlag.ReadOnly);
					if(device!=null) {
		    			QDataStream stream = new QDataStream(device);
		    			values[0] = stream.readString();
		    			values[1] = stream.readString();
		    			stream.dispose();
			    		device.close();
					}
				}
				memory2.detach();
			} catch (Throwable e) {
				throwable[0] = e;
			}
    	});
    	thread.start();
    	thread.join(2000);
    	assertEquals(null, throwable[0]);
    	assertFalse(thread.isAlive());
    	assertEquals("TEST", values[0]);
    	assertEquals("__TEST__", values[1]);
    	memory.detach();
    	assertFalse("isAttached", memory.isAttached());
    }
    
    @Test
    public void test_createReadWriteBuffer() throws Exception
    {
    	QSharedMemory memory = new QSharedMemory();
    	memory.setKey("test_createReadWriteBuffer");
    	boolean created = memory.create(1024, QSharedMemory.AccessMode.ReadWrite);
    	assertTrue("create failed: "+memory.errorString(), created);
    	assertTrue("isAttached", memory.isAttached());
    	try(QSharedMemory.DataAccess access = memory.access()){
    		ByteBuffer np = access.data();
    		assertFalse("buffer.isReadOnly", np.isReadOnly());
    		np.put("TEST".getBytes());
    	}
    	String[] value = {""};
    	Throwable[] throwable = {null};
    	QThread thread = QThread.create(()->{
    		try {
				QSharedMemory memory2 = new QSharedMemory();
				memory2.setKey("test_createReadWriteBuffer");
				memory2.attach(QSharedMemory.AccessMode.ReadOnly);
				try(QSharedMemory.DataAccess access = memory2.access()){
					ByteBuffer np = access.data();
					byte[] dst = new byte[4];
					np.get(dst);
					value[0] = new String(dst);
				}
				memory2.detach();
			} catch (Throwable e) {
				throwable[0] = e;
			}
    	});
    	thread.start();
    	thread.join(2000);
    	assertEquals(null, throwable[0]);
    	assertFalse(thread.isAlive());
    	assertEquals("TEST", value[0]);
    	memory.detach();
    	assertFalse("isAttached", memory.isAttached());
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestSharedMemory.class.getName());
    }
}
