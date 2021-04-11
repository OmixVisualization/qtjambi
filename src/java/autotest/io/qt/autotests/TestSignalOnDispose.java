/****************************************************************************
**
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

import io.qt.QtUtilities;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QIODevice;
import io.qt.core.QMetaObject.Connection;
import io.qt.core.QObject;
import io.qt.core.QTemporaryFile;
import io.qt.gui.QColor;
import io.qt.gui.QPaintDevice;
import io.qt.gui.QPaintEngine;
import io.qt.internal.QtJambiInternal;

public class TestSignalOnDispose extends QApplicationTest {
	private final static int NativeConnectionPolicy;
    static{
    	if(QtJambiInternal.isQtPatched()) {
	    	int c = 0;
	    	try {
				c = Integer.valueOf(System.getProperty("io.qt.native.connection.policy", "0"));
			} catch (NumberFormatException e) {
				e.printStackTrace();
			}
	    	NativeConnectionPolicy = c;
    	}else {
    		NativeConnectionPolicy = 4;
    	}
    }
	
	private final static List<String> tempFiles = new ArrayList<>();
	
	@Before
    public void setUp() {
	}
	
	@After
    public void tearDown() {
		for(String tempFile : tempFiles) {
			new File(tempFile).delete();
		}
		tempFiles.clear();
	}
	
	static class QAtomicInteger extends QObject{
		AtomicInteger integer = new AtomicInteger();

		public String toString() {
			return integer.toString();
		}

		public final int get() {
			return integer.get();
		}

		public final int incrementAndGet() {
			return integer.incrementAndGet();
		}
	}
	
	@Test
    public void testSignalOnDispose() {
		AtomicInteger disposed = new AtomicInteger();
		AtomicInteger jAoutToClose = new AtomicInteger();
		QAtomicInteger qAboutToClose = new QAtomicInteger();
		
		QTemporaryFile ioDevice = new QTemporaryFile("test");
		
		@SuppressWarnings("unused")
		Connection c1 = ioDevice.aboutToClose.connect(jAoutToClose::incrementAndGet);
		@SuppressWarnings("unused")
		Connection c2 = ioDevice.aboutToClose.connect(qAboutToClose::incrementAndGet);
		@SuppressWarnings("unused")
		Connection c3 = ioDevice.destroyed.connect(disposed::incrementAndGet);
		
		ioDevice.open(QIODevice.OpenModeFlag.ReadOnly);
		tempFiles.add(ioDevice.fileName());
		ioDevice.dispose();
		assertEquals("disposed signal received", 1, disposed.get());
		assertEquals("QObject received aboutToClose", 1, qAboutToClose.get());
		assertEquals("java object received aboutToClose", 1, jAoutToClose.get());
	}
	
	@Test
    public void testSignalOnDisposeLater() {
		AtomicInteger disposed = new AtomicInteger();
		AtomicInteger jAoutToClose = new AtomicInteger();
		QAtomicInteger qAboutToClose = new QAtomicInteger();
		
		QTemporaryFile ioDevice = new QTemporaryFile("test");
		
		@SuppressWarnings("unused")
		Connection c1 = ioDevice.aboutToClose.connect(jAoutToClose::incrementAndGet);
		@SuppressWarnings("unused")
		Connection c2 = ioDevice.aboutToClose.connect(qAboutToClose::incrementAndGet);
		@SuppressWarnings("unused")
		Connection c3 = ioDevice.destroyed.connect(disposed::incrementAndGet);
		
		ioDevice.open(QIODevice.OpenModeFlag.ReadOnly);
		tempFiles.add(ioDevice.fileName());
		ioDevice.disposeLater();
		QCoreApplication.sendPostedEvents(ioDevice, QEvent.Type.DeferredDispose.value());
		assertEquals("disposed signal received", 1, disposed.get());
		assertEquals("QObject received aboutToClose", 1, qAboutToClose.get());
		assertEquals("java object received aboutToClose", 1, jAoutToClose.get());
	}
	
	@Test
    public void testSignalOnGC() {
		AtomicInteger disposed = new AtomicInteger();
		AtomicInteger jAoutToClose = new AtomicInteger();
		QAtomicInteger qAboutToClose = new QAtomicInteger();
		
		{
			QTemporaryFile ioDevice = new QTemporaryFile("test");
			
			Connection c1 = ioDevice.aboutToClose.connect(jAoutToClose::incrementAndGet);
			Connection c2 = ioDevice.aboutToClose.connect(qAboutToClose::incrementAndGet);
			Connection c3 = ioDevice.destroyed.connect(disposed::incrementAndGet);
			assertTrue(c1!=null);
			assertTrue(c2!=null);
			assertTrue(c3!=null);
			
			ioDevice.open(QIODevice.OpenModeFlag.ReadOnly);
			tempFiles.add(ioDevice.fileName());
			ioDevice = null;
		}
		long t1 = System.currentTimeMillis();
		while(disposed.get()==0) {
			System.gc();
			try {
				Thread.sleep(50);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			QCoreApplication.processEvents();
			QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
			if(t1+5000<System.currentTimeMillis())
				break;
		}
		t1 = System.currentTimeMillis();
		while(qAboutToClose.get()==0) {
			QCoreApplication.processEvents();
			QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
			if(t1+5000<System.currentTimeMillis())
				break;
		}
		assertEquals("disposed signal received", 1, disposed.get());
		assertEquals("QObject received aboutToClose", NativeConnectionPolicy==2 || NativeConnectionPolicy==4 ? 1 : 0, qAboutToClose.get());
		assertEquals("java object received aboutToClose", NativeConnectionPolicy==4 ? 1 : 0, jAoutToClose.get());
	}
	
	@Test(expected=IllegalArgumentException.class)
    public void testSignalOnDisposeSelfInterface() {
		QPaintDevice scriptable = new QPaintDevice() {
			@SuppressWarnings("unused")
			public void test() {}

			@Override
			public QPaintEngine paintEngine() {
				return null;
			}
		};
		QtUtilities.getSignalOnDispose(scriptable).connect(scriptable, "test()");
	}
	
	@Test(expected=IllegalArgumentException.class)
    public void testSignalOnDisposeSelfQObject() {
		QObject object = new QObject() {
			@SuppressWarnings("unused")
			public void test() {}
		};
		QtUtilities.getSignalOnDispose(object).connect(object, "test()");
	}
	
	@Test(expected=IllegalArgumentException.class)
    public void testSignalOnDisposeSelfValue() {
		QColor object = new QColor() {
			@SuppressWarnings("unused")
			public void test() {}
		};
		QtUtilities.getSignalOnDispose(object).connect(object, "test()");
	}
	
	@Test
    public void testSignalOnDisposeNotSelf() {
		QObject object = new QObject() {
			QObject indiObject = new QObject();
			{
				QtUtilities.getSignalOnDispose(this).connect(indiObject::dispose);
			}
		};
		Assert.assertTrue(object!=null);
	}
}
