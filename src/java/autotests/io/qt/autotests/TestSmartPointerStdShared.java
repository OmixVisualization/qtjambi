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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.lang.ref.WeakReference;
import java.util.List;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtObject;
import io.qt.autotests.generated.General;
import io.qt.autotests.generated.SmartPointerTest;
import io.qt.core.QEvent;
import io.qt.core.QIODevice;
import io.qt.core.QMetaObject.Connection;
import io.qt.core.QObject;
import io.qt.core.QTemporaryFile;
import io.qt.internal.TestUtility;
import io.qt.widgets.QApplication;
import io.qt.widgets.QGraphicsItem;
import io.qt.widgets.QGraphicsTextItem;
import io.qt.widgets.QGridLayout;
import io.qt.widgets.QLayout;
import io.qt.widgets.QLayoutItem;
import io.qt.widgets.QSpacerItem;
import io.qt.widgets.QWidget;
import io.qt.widgets.QWidgetItem;

public class TestSmartPointerStdShared extends ApplicationInitializer {

	public static void main(String args[]) {
		org.junit.runner.JUnitCore.main(TestSmartPointerStdShared.class.getName());
	}
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
	@Test
	public void testStdSharedObject10() {
		SmartPointerTest object = new SmartPointerTest();
		QObject sharedObject = object.createStdSharedObject1();
		assertNotNull(sharedObject);
		assertTrue(TestUtility.isSharedPointer(sharedObject));
		assertEquals("SharedObject1", sharedObject.objectName());
		assertEquals("", object.deletedSharedObjectName());
		sharedObject.dispose();
		assertEquals("SharedObject1", object.deletedSharedObjectName());
	}
	
	@Test
	public void testStdSharedObject11() {
		SmartPointerTest object = new SmartPointerTest();
		QObject sharedObject = object.createStdSharedObject1();
		assertNotNull(sharedObject);
		assertTrue(TestUtility.isSharedPointer(sharedObject));
		assertEquals("SharedObject1", sharedObject.objectName());
		assertEquals("", object.deletedSharedObjectName());
		WeakReference<QObject> weak = new WeakReference<QObject>(sharedObject);
		sharedObject = null;
		int count = 0;
		while(weak.get()!=null && count++<50){
			ApplicationInitializer.runGC();
			Thread.yield();
			synchronized(object){
				try {
					object.wait(50);
				} catch (InterruptedException e) {
				}
			}
		}
		while(object.deletedSharedObjectName().isEmpty() && count++<100){
			QApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		}
		assertEquals(null, weak.get());
		QApplication.processEvents();
		QApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		assertEquals("SharedObject1", object.deletedSharedObjectName());
	}
	
	@Test
	public void testStdSharedObject20() {
		SmartPointerTest object = new SmartPointerTest();
		QGraphicsItem sharedObject = object.createStdSharedObject2();
		assertNotNull(sharedObject);
		assertTrue(TestUtility.isSharedPointer(sharedObject));
		assertEquals(QGraphicsTextItem.class, General.internalAccess.getClass(sharedObject));
		assertEquals("", object.deletedSharedObjectName());
		((QGraphicsTextItem)sharedObject).dispose();
		assertEquals("SharedObject2", object.deletedSharedObjectName());
	}

	@Test
	public void testStdSharedObject21() {
		SmartPointerTest object = new SmartPointerTest();
		QGraphicsItem sharedObject = object.createStdSharedObject2();
		assertNotNull(sharedObject);
		assertTrue(TestUtility.isSharedPointer(sharedObject));
		assertEquals(QGraphicsTextItem.class, General.internalAccess.getClass(sharedObject));
		assertEquals("", object.deletedSharedObjectName());
		General.internalAccess.setJavaOwnership(sharedObject);
		WeakReference<QGraphicsItem> weak = new WeakReference<QGraphicsItem>(sharedObject);
		sharedObject = null;
		int count = 0;
		while(weak.get()!=null && count++<50){
			ApplicationInitializer.runGC();
			Thread.yield();
			synchronized(object){
				try {
					object.wait(50);
				} catch (InterruptedException e) {
				}
			}
		}
		while(object.deletedSharedObjectName().isEmpty() && count++<100){
			QApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		}
		assertEquals("SharedObject2", object.deletedSharedObjectName());
	}
	
	@Test
	public void testStdSharedObject30() {
		SmartPointerTest object = new SmartPointerTest();
		QEvent sharedObject = object.createStdSharedObject3();
		assertNotNull(sharedObject);
		assertTrue(TestUtility.isSharedPointer(sharedObject));
		assertEquals(QEvent.Type.User, sharedObject.type());
		assertEquals("", object.deletedSharedObjectName());
		sharedObject.dispose();
		assertEquals("SharedEvent", object.deletedSharedObjectName());
	}

	@Test
	public void testStdSharedObject31() {
		SmartPointerTest object = new SmartPointerTest();
		QEvent sharedObject = object.createStdSharedObject3();
		assertNotNull(sharedObject);
		assertTrue(TestUtility.isSharedPointer(sharedObject));
		assertEquals("", object.deletedSharedObjectName());
		General.internalAccess.setJavaOwnership(sharedObject);
		sharedObject = null;
		int count = 0;
		while(object.deletedSharedObjectName().isEmpty() && count++<50){
			ApplicationInitializer.runGC();
			Thread.yield();
			synchronized(object){
				try {
					object.wait(50);
				} catch (InterruptedException e) {
				}
			}
		}
		assertEquals("SharedEvent", object.deletedSharedObjectName());
	}
	
	@Test
	public void testStdSharedObject40() {
		SmartPointerTest object = new SmartPointerTest();
		QLayoutItem sharedObject = object.createStdSharedObject4();
		assertNotNull(sharedObject);
		assertTrue(TestUtility.isSharedPointer(sharedObject));
		assertEquals(QGridLayout.class, General.internalAccess.getClass(sharedObject));
		assertEquals("", object.deletedSharedObjectName());
		((QLayout)sharedObject).dispose();
		assertEquals("SharedObject4", object.deletedSharedObjectName());
	}

	@Test
	public void testStdSharedObject41() {
		SmartPointerTest object = new SmartPointerTest();
		QLayoutItem sharedObject = object.createStdSharedObject4();
		assertNotNull(sharedObject);
		assertTrue(TestUtility.isSharedPointer(sharedObject));
		assertEquals(QGridLayout.class, General.internalAccess.getClass(sharedObject));
		assertEquals("", object.deletedSharedObjectName());
		General.internalAccess.setJavaOwnership(sharedObject);
		WeakReference<QLayoutItem> weak = new WeakReference<QLayoutItem>(sharedObject);
		sharedObject = null;
		int count = 0;
		while(weak.get()!=null && count++<50){
			ApplicationInitializer.runGC();
			Thread.yield();
			synchronized(object){
				try {
					object.wait(50);
				} catch (InterruptedException e) {
				}
			}
		}
		while(object.deletedSharedObjectName().isEmpty() && count++<100){
			QApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		}
		assertEquals("SharedObject4", object.deletedSharedObjectName());
	}
	
	@Test
	public void testStdSharedObject50() {
		SmartPointerTest object = new SmartPointerTest();
		QLayoutItem sharedObject = object.createStdSharedObject5();
		assertNotNull(sharedObject);
		assertTrue(TestUtility.isSharedPointer(sharedObject));
		assertEquals(QSpacerItem.class, General.internalAccess.getClass(sharedObject));
		assertEquals("", object.deletedSharedObjectName());
		((QSpacerItem)sharedObject).dispose();
		assertEquals("SpacerItem", object.deletedSharedObjectName());
	}

	@Test
	public void testStdSharedObject51() {
		SmartPointerTest object = new SmartPointerTest();
		QLayoutItem sharedObject = object.createStdSharedObject5();
		assertNotNull(sharedObject);
		assertTrue(TestUtility.isSharedPointer(sharedObject));
		assertEquals(QSpacerItem.class, General.internalAccess.getClass(sharedObject));
		assertEquals("", object.deletedSharedObjectName());
		General.internalAccess.setJavaOwnership(sharedObject);
		sharedObject = null;
		int count = 0;
		while(object.deletedSharedObjectName().isEmpty() && count++<50){
			ApplicationInitializer.runGC();
			Thread.yield();
			synchronized(object){
				try {
					object.wait(50);
				} catch (InterruptedException e) {
				}
			}
		}
		assertEquals("SpacerItem", object.deletedSharedObjectName());
	}
	
	@Test
	public void testStdSharedObject60() {
		SmartPointerTest object = new SmartPointerTest();
		QLayoutItem sharedObject = object.createStdSharedObject6();
		assertNotNull(sharedObject);
		assertTrue(TestUtility.isSharedPointer(sharedObject));
		assertEquals(QWidgetItem.class, General.internalAccess.getClass(sharedObject));
		assertEquals("", object.deletedSharedObjectName());
		QWidget widget = ((QWidgetItem)sharedObject).widget();
		boolean[] disposed = {false};
		widget.destroyed.connect(()->disposed[0]=true);
		((QWidgetItem)sharedObject).dispose();
		assertTrue(sharedObject.isDisposed());
		assertNotNull(widget);
		assertEquals("WidgetItem", object.deletedSharedObjectName());
		object.resetSharedObjectName();
		widget.dispose();
		assertTrue(widget.isDisposed());
		assertTrue(disposed[0]);
		assertEquals("Widget", object.deletedSharedObjectName());
	}

	@Test
	public void testStdSharedObject61() {
		SmartPointerTest object = new SmartPointerTest();
		QLayoutItem sharedObject = object.createStdSharedObject6();
		assertNotNull(sharedObject);
		assertTrue(TestUtility.isSharedPointer(sharedObject));
		assertEquals(QWidgetItem.class, General.internalAccess.getClass(sharedObject));
		assertEquals("", object.deletedSharedObjectName());
		QWidget widget = ((QWidgetItem)sharedObject).widget();
		General.internalAccess.setJavaOwnership(sharedObject);
		sharedObject = null;
		int count = 0;
		while(object.deletedSharedObjectName().isEmpty() && count++<50){
			ApplicationInitializer.runGC();
			Thread.yield();
			synchronized(object){
				try {
					object.wait(50);
				} catch (InterruptedException e) {
				}
			}
		}
		assertEquals("WidgetItem", object.deletedSharedObjectName());
		object.resetSharedObjectName();
		General.internalAccess.setJavaOwnership(widget);
		AtomicBoolean disposed = new AtomicBoolean(false);
		widget.destroyed.connect(()->disposed.set(true));
		widget = null;
		for (int i = 0; i < 50 && !disposed.get(); ++i) {
			ApplicationInitializer.runGC();
			QApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
			QApplication.processEvents();
			try {
				Thread.yield();
				Thread.sleep(5);
			} catch (InterruptedException e) {}
		}
		assertTrue("Widget in QSharedPointer is not disposed", disposed.get());
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
	public void testStdSharedObject70() {
		SmartPointerTest object = new SmartPointerTest();
		AtomicInteger disposed = new AtomicInteger();
		AtomicInteger jAoutToClose = new AtomicInteger();
		QAtomicInteger qAboutToClose = new QAtomicInteger();
		QTemporaryFile ioDevice = object.createStdSharedObject7();
		assertNotNull(ioDevice);
		assertTrue(TestUtility.isSharedPointer(ioDevice));
		
		@SuppressWarnings("unused")
		Connection c1 = ioDevice.aboutToClose.connect(jAoutToClose::incrementAndGet);
		@SuppressWarnings("unused")
		Connection c2 = ioDevice.aboutToClose.connect(qAboutToClose::incrementAndGet);
		@SuppressWarnings("unused")
		Connection c3 = ioDevice.destroyed.connect(disposed::incrementAndGet);
		
		ioDevice.open(QIODevice.OpenModeFlag.ReadOnly);
		ioDevice.dispose();
		assertEquals("disposed signal received", 1, disposed.get());
		assertEquals("QObject received aboutToClose", 1, qAboutToClose.get());
		assertEquals("java object received aboutToClose", 1, jAoutToClose.get());
	}
	
	@Test
	public void testStdSharedObject80() {
		SmartPointerTest object = new SmartPointerTest();
		List<String> sharedObject = object.createStdSharedObject8();
		assertTrue(sharedObject instanceof QtObject);
		assertTrue(TestUtility.isSharedPointer((QtObject)sharedObject));
		assertEquals(1, sharedObject.size());
	}
	
	@Test
	public void testObjectToSharedPointer() {
		QObject newObject = new QObject();
		QObject sharedObject = SmartPointerTest.asStdSharedPointer(newObject);
		assertTrue(TestUtility.isSharedPointer(sharedObject));
		assertEquals(newObject, sharedObject);
		assertTrue(TestUtility.isSharedPointer(newObject));
	}
}
