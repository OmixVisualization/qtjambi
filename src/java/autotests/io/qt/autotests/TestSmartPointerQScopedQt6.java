/****************************************************************************
**
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

import static org.junit.Assert.*;

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.Nullable;
import io.qt.autotests.generated.General;
import io.qt.autotests.generated.SmartPointerTest;
import io.qt.core.QDynamicPropertyChangeEvent;
import io.qt.core.QEvent;
import io.qt.core.QObject;
import io.qt.core.QTimer;
import io.qt.gui.QStandardItemModel;
import io.qt.gui.QStatusTipEvent;

public class TestSmartPointerQScopedQt6 extends ApplicationInitializer {

	public static void main(String args[]) {
		org.junit.runner.JUnitCore.main(TestSmartPointerQScopedQt6.class.getName());
	}
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }

	@Test
	public void testScopedObjectToJava() {
		SmartPointerTest object = new SmartPointerTest();
		QObject uobj = object.createScopedObject();
		assertFalse(uobj.isDisposed());
		assertTrue(uobj instanceof QStandardItemModel);
		assertEquals("ScopedModel", uobj.objectName());
		assertTrue(General.internalAccess.isJavaOwnership(uobj));
	}
	
	@Test
	public void testScopedObjectNativeVirtual() {
		QObject[] createdObj = {null};
		SmartPointerTest object = new SmartPointerTest(){
			@Override
			public @Nullable QObject createScopedObject() {
				QObject uobj = super.createScopedObject();
				createdObj[0] = uobj;
				assertFalse(uobj.isDisposed());
				assertTrue(uobj instanceof QStandardItemModel);
				assertEquals("ScopedModel", uobj.objectName());
				assertTrue(General.internalAccess.isJavaOwnership(uobj));
				return uobj;
			}
			
		};
		QObject uobj = object.createScopedObject();
		assertTrue(createdObj[0] == uobj);
		assertFalse(uobj.isDisposed());
		assertTrue(uobj instanceof QStandardItemModel);
		assertEquals("ScopedModel", uobj.objectName());
		assertTrue(General.internalAccess.isJavaOwnership(uobj));
	}
	
	@Test
	public void testScopedObjectUseNative() {
		QObject[] createdObj = {null};
		SmartPointerTest object = new SmartPointerTest(){
			@Override
			public @Nullable QObject createScopedObject() {
				QObject uobj = super.createScopedObject();
				createdObj[0] = uobj;
				assertFalse(uobj.isDisposed());
				assertTrue(uobj instanceof QStandardItemModel);
				assertEquals("ScopedModel", uobj.objectName());
				assertTrue(General.internalAccess.isJavaOwnership(uobj));
				return uobj;
			}
			
			@Override
			public void analyze(@Nullable QObject obj) {
				assertFalse(obj.isDisposed());
				assertTrue(createdObj[0] == obj);
				assertTrue(obj instanceof QStandardItemModel);
				assertEquals("ScopedModel", obj.objectName());
				assertTrue(General.internalAccess.isCppOwnership(obj));				
			}
			
		};
		object.useScopedObject();
		assertTrue(createdObj[0]!=null);
		assertTrue(createdObj[0].isDisposed());
	}
	
	@Test
	public void testScopedObjectUseJava() {
		QObject[] createdObj = {null};
		SmartPointerTest object = new SmartPointerTest(){
			@Override
			public @Nullable QObject createScopedObject() {
				QObject uobj = new QTimer();
				uobj.setObjectName("ScopedTimer");
				createdObj[0] = uobj;
				assertTrue(General.internalAccess.isJavaOwnership(uobj));
				return uobj;
			}
			
			@Override
			public void analyze(@Nullable QObject obj) {
				assertFalse(obj.isDisposed());
				assertTrue(createdObj[0] == obj);
				assertTrue(obj instanceof QTimer);
				assertEquals("ScopedTimer", obj.objectName());
				assertTrue(General.internalAccess.isCppOwnership(obj));				
			}
			
		};
		object.useScopedObject();
		assertTrue(createdObj[0]!=null);
		assertTrue(createdObj[0].isDisposed());
	}

	@Test
	public void testScopedEventToJava() {
		SmartPointerTest object = new SmartPointerTest();
		QEvent uobj = object.createScopedEvent();
		assertFalse(uobj.isDisposed());
		assertTrue(uobj instanceof QDynamicPropertyChangeEvent);
		assertEquals("ScopedEvent", ((QDynamicPropertyChangeEvent)uobj).propertyName().toString());
		assertTrue(General.internalAccess.isJavaOwnership(uobj));
	}
	
	@Test
	public void testScopedEventNativeVirtual() {
		QEvent[] createdObj = {null};
		SmartPointerTest object = new SmartPointerTest(){
			@Override
			public @Nullable QEvent createScopedEvent() {
				QEvent uobj = super.createScopedEvent();
				createdObj[0] = uobj;
				assertFalse(uobj.isDisposed());
				assertTrue(uobj instanceof QDynamicPropertyChangeEvent);
				assertEquals("ScopedEvent", ((QDynamicPropertyChangeEvent)uobj).propertyName().toString());
				assertTrue(General.internalAccess.isJavaOwnership(uobj));
				return uobj;
			}
			
		};
		QEvent uobj = object.createScopedEvent();
		assertTrue(createdObj[0] == uobj);
		assertFalse(uobj.isDisposed());
		assertTrue(uobj instanceof QDynamicPropertyChangeEvent);
		assertEquals("ScopedEvent", ((QDynamicPropertyChangeEvent)uobj).propertyName().toString());
		assertTrue(General.internalAccess.isJavaOwnership(uobj));
	}
	
	@Test
	public void testScopedEventUseNative() {
		QEvent[] createdObj = {null};
		SmartPointerTest object = new SmartPointerTest(){
			@Override
			public @Nullable QEvent createScopedEvent() {
				QEvent uobj = super.createScopedEvent();
				createdObj[0] = uobj;
				assertFalse(uobj.isDisposed());
				assertTrue(uobj instanceof QDynamicPropertyChangeEvent);
				assertEquals("ScopedEvent", ((QDynamicPropertyChangeEvent)uobj).propertyName().toString());
				assertTrue(General.internalAccess.isJavaOwnership(uobj));
				return uobj;
			}
			
			@Override
			public void analyze(@Nullable QEvent obj) {
				assertTrue(createdObj[0].isDisposed());
				assertFalse(obj.isDisposed());
				assertTrue(createdObj[0]!=obj);
				assertTrue(obj instanceof QDynamicPropertyChangeEvent);
				assertEquals("ScopedEvent", ((QDynamicPropertyChangeEvent)obj).propertyName().toString());
				assertTrue(General.internalAccess.isSplitOwnership(obj));				
			}
			
		};
		object.useScopedEvent();
		assertTrue(createdObj[0]!=null);
		assertTrue(createdObj[0].isDisposed());
	}
	
	@Test
	public void testScopedEventUseJava() {
		QEvent[] createdObj = {null};
		SmartPointerTest object = new SmartPointerTest(){
			@Override
			public @Nullable QEvent createScopedEvent() {
				QEvent uobj = new QStatusTipEvent("ScopedEvent");
				createdObj[0] = uobj;
				assertTrue(General.internalAccess.isJavaOwnership(uobj));
				return uobj;
			}
			
			@Override
			public void analyze(@Nullable QEvent obj) {
				assertFalse(obj.isDisposed());
				assertTrue(createdObj[0]==obj);
				assertTrue(obj instanceof QStatusTipEvent);
				assertEquals("ScopedEvent", ((QStatusTipEvent)obj).tip().toString());
				assertTrue(General.internalAccess.isCppOwnership(obj));				
			}
			
		};
		object.useScopedEvent();
		assertTrue(createdObj[0]!=null);
		assertTrue(createdObj[0].isDisposed());
	}
}
