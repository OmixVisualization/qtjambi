/****************************************************************************
**
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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.lang.ref.WeakReference;
import java.util.List;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;

import org.junit.Test;

import io.qt.QtObject;
import io.qt.autotests.generated.General;
import io.qt.autotests.generated.SharedPointerTest;
import io.qt.core.QEvent;
import io.qt.core.QIODevice;
import io.qt.core.QMetaObject.Connection;
import io.qt.core.QObject;
import io.qt.core.QTemporaryFile;
import io.qt.internal.QtJambiInternal;
import io.qt.widgets.QApplication;
import io.qt.widgets.QGraphicsItem;
import io.qt.widgets.QGraphicsTextItem;
import io.qt.widgets.QGridLayout;
import io.qt.widgets.QLayout;
import io.qt.widgets.QLayoutItem;
import io.qt.widgets.QSpacerItem;
import io.qt.widgets.QWidget;
import io.qt.widgets.QWidgetItem;

public class TestSharedPointer extends QApplicationTest {

	public static void main(String args[]) {
		org.junit.runner.JUnitCore.main(TestSharedPointer.class.getName());
	}

	private SharedPointerTest object = new SharedPointerTest();

	@Test
	public void testSharedObject10() {
		QObject sharedObject = object.createSharedObject1();
		assertNotNull(sharedObject);
		assertTrue(QtJambiInternal.isSharedPointer(sharedObject));
		assertEquals("SharedObject1", sharedObject.objectName());
		assertEquals("", object.deletedSharedObjectName());
		sharedObject.dispose();
		assertEquals("SharedObject1", object.deletedSharedObjectName());
	}
	
	@Test
	public void testSharedObject11() {
		QObject sharedObject = object.createSharedObject1();
		assertNotNull(sharedObject);
		assertTrue(QtJambiInternal.isSharedPointer(sharedObject));
		assertEquals("SharedObject1", sharedObject.objectName());
		assertEquals("", object.deletedSharedObjectName());
		assertTrue(General.internalAccess.isJavaOwnership(sharedObject));
		WeakReference<QObject> weak = new WeakReference<QObject>(sharedObject);
		sharedObject = null;
		int count = 0;
		while(weak.get()!=null){
			System.gc();
			synchronized(object){
				try {
					object.wait(50);
				} catch (InterruptedException e) {
				}
			}
			count++;
			if(count>5)
				break;
		}
		while(object.deletedSharedObjectName().isEmpty()){
			QApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
			count++;
			if(count>15)
				break;
		}
		assertEquals(null, weak.get());
		QApplication.processEvents();
		QApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		assertEquals("SharedObject1", object.deletedSharedObjectName());
	}
	
	@Test
	public void testSharedObject20() {
		QGraphicsItem sharedObject = object.createSharedObject2();
		assertNotNull(sharedObject);
		assertTrue(QtJambiInternal.isSharedPointer(sharedObject));
		assertEquals(QGraphicsTextItem.class, sharedObject.getClass());
		assertEquals("", object.deletedSharedObjectName());
		((QGraphicsTextItem)sharedObject).dispose();
		assertEquals("SharedObject2", object.deletedSharedObjectName());
	}

	@Test
	public void testSharedObject21() {
		QGraphicsItem sharedObject = object.createSharedObject2();
		assertNotNull(sharedObject);
		assertTrue(QtJambiInternal.isSharedPointer(sharedObject));
		assertEquals(QGraphicsTextItem.class, sharedObject.getClass());
		assertEquals("", object.deletedSharedObjectName());
		General.internalAccess.setJavaOwnership(sharedObject);
		WeakReference<QGraphicsItem> weak = new WeakReference<QGraphicsItem>(sharedObject);
		sharedObject = null;
		int count = 0;
		while(weak.get()!=null){
			System.gc();
			synchronized(object){
				try {
					object.wait(50);
				} catch (InterruptedException e) {
				}
			}
			count++;
			if(count>5)
				break;
		}
		while(object.deletedSharedObjectName().isEmpty()){
			QApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
			count++;
			if(count>15)
				break;
		}
		assertEquals("SharedObject2", object.deletedSharedObjectName());
	}
	
	@Test
	public void testSharedObject30() {
		QEvent sharedObject = object.createSharedObject3();
		assertNotNull(sharedObject);
		assertTrue(QtJambiInternal.isSharedPointer(sharedObject));
		assertEquals(QEvent.Type.User, sharedObject.type());
		assertEquals("", object.deletedSharedObjectName());
		sharedObject.dispose();
		assertEquals("SharedEvent", object.deletedSharedObjectName());
	}

	@Test
	public void testSharedObject31() {
		QEvent sharedObject = object.createSharedObject3();
		assertNotNull(sharedObject);
		assertTrue(QtJambiInternal.isSharedPointer(sharedObject));
		assertEquals("", object.deletedSharedObjectName());
		General.internalAccess.setJavaOwnership(sharedObject);
		sharedObject = null;
		int count = 0;
		while(object.deletedSharedObjectName().isEmpty()){
			System.gc();
			synchronized(object){
				try {
					object.wait(50);
				} catch (InterruptedException e) {
				}
			}
			count++;
			if(count>5)
				break;
		}
		assertEquals("SharedEvent", object.deletedSharedObjectName());
	}
	
	@Test
	public void testSharedObject40() {
		QLayoutItem sharedObject = object.createSharedObject4();
		assertNotNull(sharedObject);
		assertTrue(QtJambiInternal.isSharedPointer(sharedObject));
		assertEquals(QGridLayout.class, sharedObject.getClass());
		assertEquals("", object.deletedSharedObjectName());
		((QLayout)sharedObject).dispose();
		assertEquals("SharedObject4", object.deletedSharedObjectName());
	}

	@Test
	public void testSharedObject41() {
		QLayoutItem sharedObject = object.createSharedObject4();
		assertNotNull(sharedObject);
		assertTrue(QtJambiInternal.isSharedPointer(sharedObject));
		assertEquals(QGridLayout.class, sharedObject.getClass());
		assertEquals("", object.deletedSharedObjectName());
		General.internalAccess.setJavaOwnership(sharedObject);
		WeakReference<QLayoutItem> weak = new WeakReference<QLayoutItem>(sharedObject);
		sharedObject = null;
		int count = 0;
		while(weak.get()!=null){
			System.gc();
			synchronized(object){
				try {
					object.wait(50);
				} catch (InterruptedException e) {
				}
			}
			count++;
			if(count>5)
				break;
		}
		while(object.deletedSharedObjectName().isEmpty()){
			QApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
			count++;
			if(count>15)
				break;
		}
		assertEquals("SharedObject4", object.deletedSharedObjectName());
	}
	
	@Test
	public void testSharedObject50() {
		QLayoutItem sharedObject = object.createSharedObject5();
		assertNotNull(sharedObject);
		assertTrue(QtJambiInternal.isSharedPointer(sharedObject));
		assertEquals(QSpacerItem.class, sharedObject.getClass());
		assertEquals("", object.deletedSharedObjectName());
		((QSpacerItem)sharedObject).dispose();
		assertEquals("SpacerItem", object.deletedSharedObjectName());
	}

	@Test
	public void testSharedObject51() {
		QLayoutItem sharedObject = object.createSharedObject5();
		assertNotNull(sharedObject);
		assertTrue(QtJambiInternal.isSharedPointer(sharedObject));
		assertEquals(QSpacerItem.class, sharedObject.getClass());
		assertEquals("", object.deletedSharedObjectName());
		General.internalAccess.setJavaOwnership(sharedObject);
		sharedObject = null;
		int count = 0;
		while(object.deletedSharedObjectName().isEmpty()){
			System.gc();
			synchronized(object){
				try {
					object.wait(50);
				} catch (InterruptedException e) {
				}
			}
			count++;
			if(count>5)
				break;
		}
		assertEquals("SpacerItem", object.deletedSharedObjectName());
	}
	
	@Test
	public void testSharedObject60() {
		QLayoutItem sharedObject = object.createSharedObject6();
		assertNotNull(sharedObject);
		assertTrue(QtJambiInternal.isSharedPointer(sharedObject));
		assertEquals(QWidgetItem.class, sharedObject.getClass());
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
	public void testSharedObject61() {
		QLayoutItem sharedObject = object.createSharedObject6();
		assertNotNull(sharedObject);
		assertTrue(QtJambiInternal.isSharedPointer(sharedObject));
		assertEquals(QWidgetItem.class, sharedObject.getClass());
		assertEquals("", object.deletedSharedObjectName());
		QWidget widget = ((QWidgetItem)sharedObject).widget();
		General.internalAccess.setJavaOwnership(sharedObject);
		sharedObject = null;
		int count = 0;
		while(object.deletedSharedObjectName().isEmpty()){
			System.gc();
			synchronized(object){
				try {
					object.wait(50);
				} catch (InterruptedException e) {
				}
			}
			count++;
			if(count>5)
				break;
		}
		assertEquals("WidgetItem", object.deletedSharedObjectName());
		object.resetSharedObjectName();
		General.internalAccess.setJavaOwnership(widget);
		AtomicBoolean disposed = new AtomicBoolean(false);
		widget.destroyed.connect(()->disposed.set(true));
		widget = null;
		for (int i = 0; i < 50 && !disposed.get(); ++i) {
			System.gc();
			System.runFinalization();
			QApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
			QApplication.processEvents();
			try {
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
	public void testSharedObject70() {
		AtomicInteger disposed = new AtomicInteger();
		AtomicInteger jAoutToClose = new AtomicInteger();
		QAtomicInteger qAboutToClose = new QAtomicInteger();
		QTemporaryFile ioDevice = object.createSharedObject7();
		assertNotNull(ioDevice);
		assertTrue(QtJambiInternal.isSharedPointer(ioDevice));
		
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
	public void testSharedObject80() {
		List<String> sharedObject = object.createSharedObject8();
		assertTrue(sharedObject instanceof QtObject);
		assertTrue(QtJambiInternal.isSharedPointer((QtObject)sharedObject));
		assertEquals(1, sharedObject.size());
	}
	
	@Test
	public void testObjectToSharedPointer() {
		QObject newObject = new QObject();
		QObject sharedObject = SharedPointerTest.asSharedPointer(newObject);
		assertTrue(QtJambiInternal.isSharedPointer(sharedObject));
		assertEquals(newObject, sharedObject);
		assertTrue(QtJambiInternal.isSharedPointer(newObject));
	}
}
