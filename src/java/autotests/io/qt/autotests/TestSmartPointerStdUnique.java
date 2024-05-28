/****************************************************************************
**
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

public class TestSmartPointerStdUnique extends ApplicationInitializer {

	public static void main(String args[]) {
		org.junit.runner.JUnitCore.main(TestSmartPointerStdUnique.class.getName());
	}
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }

	@Test
	public void testUniqueObjectToJava() {
		SmartPointerTest object = new SmartPointerTest();
		QObject uobj = object.createUniqueObject();
		assertFalse(uobj.isDisposed());
		assertTrue(uobj instanceof QStandardItemModel);
		assertEquals("UniqueModel", uobj.objectName());
		assertTrue(General.internalAccess.isJavaOwnership(uobj));
	}
	
	@Test
	public void testUniqueObjectNativeVirtual() {
		QObject[] createdObj = {null};
		SmartPointerTest object = new SmartPointerTest(){
			@Override
			public @Nullable QObject createUniqueObject() {
				QObject uobj = super.createUniqueObject();
				createdObj[0] = uobj;
				assertFalse(uobj.isDisposed());
				assertTrue(uobj instanceof QStandardItemModel);
				assertEquals("UniqueModel", uobj.objectName());
				assertTrue(General.internalAccess.isJavaOwnership(uobj));
				return uobj;
			}
			
		};
		QObject uobj = object.createUniqueObject();
		assertTrue(createdObj[0] == uobj);
		assertFalse(uobj.isDisposed());
		assertTrue(uobj instanceof QStandardItemModel);
		assertEquals("UniqueModel", uobj.objectName());
		assertTrue(General.internalAccess.isJavaOwnership(uobj));
	}
	
	@Test
	public void testUniqueObjectUseNative() {
		QObject[] createdObj = {null};
		SmartPointerTest object = new SmartPointerTest(){
			@Override
			public @Nullable QObject createUniqueObject() {
				QObject uobj = super.createUniqueObject();
				createdObj[0] = uobj;
				assertFalse(uobj.isDisposed());
				assertTrue(uobj instanceof QStandardItemModel);
				assertEquals("UniqueModel", uobj.objectName());
				assertTrue(General.internalAccess.isJavaOwnership(uobj));
				return uobj;
			}
			
			@Override
			public void analyze(@Nullable QObject obj) {
				assertFalse(obj.isDisposed());
				assertTrue(createdObj[0] == obj);
				assertTrue(obj instanceof QStandardItemModel);
				assertEquals("UniqueModel", obj.objectName());
				assertTrue(General.internalAccess.isCppOwnership(obj));				
			}
			
		};
		object.useUniqueObject();
		assertTrue(createdObj[0]!=null);
		assertTrue(createdObj[0].isDisposed());
	}
	
	@Test
	public void testUniqueObjectUseJava() {
		QObject[] createdObj = {null};
		SmartPointerTest object = new SmartPointerTest(){
			@Override
			public @Nullable QObject createUniqueObject() {
				QObject uobj = new QTimer();
				uobj.setObjectName("UniqueTimer");
				createdObj[0] = uobj;
				assertTrue(General.internalAccess.isJavaOwnership(uobj));
				return uobj;
			}
			
			@Override
			public void analyze(@Nullable QObject obj) {
				assertFalse(obj.isDisposed());
				assertTrue(createdObj[0] == obj);
				assertTrue(obj instanceof QTimer);
				assertEquals("UniqueTimer", obj.objectName());
				assertTrue(General.internalAccess.isCppOwnership(obj));				
			}
			
		};
		object.useUniqueObject();
		assertTrue(createdObj[0]!=null);
		assertTrue(createdObj[0].isDisposed());
	}

	@Test
	public void testUniqueEventToJava() {
		SmartPointerTest object = new SmartPointerTest();
		QEvent uobj = object.createUniqueEvent();
		assertFalse(uobj.isDisposed());
		assertTrue(uobj instanceof QDynamicPropertyChangeEvent);
		assertEquals("UniqueEvent", ((QDynamicPropertyChangeEvent)uobj).propertyName().toString());
		assertTrue(General.internalAccess.isJavaOwnership(uobj));
	}
	
	@Test
	public void testUniqueEventNativeVirtual() {
		QEvent[] createdObj = {null};
		SmartPointerTest object = new SmartPointerTest(){
			@Override
			public @Nullable QEvent createUniqueEvent() {
				QEvent uobj = super.createUniqueEvent();
				createdObj[0] = uobj;
				assertFalse(uobj.isDisposed());
				assertTrue(uobj instanceof QDynamicPropertyChangeEvent);
				assertEquals("UniqueEvent", ((QDynamicPropertyChangeEvent)uobj).propertyName().toString());
				assertTrue(General.internalAccess.isJavaOwnership(uobj));
				return uobj;
			}
			
		};
		QEvent uobj = object.createUniqueEvent();
		assertTrue(createdObj[0] == uobj);
		assertFalse(uobj.isDisposed());
		assertTrue(uobj instanceof QDynamicPropertyChangeEvent);
		assertEquals("UniqueEvent", ((QDynamicPropertyChangeEvent)uobj).propertyName().toString());
		assertTrue(General.internalAccess.isJavaOwnership(uobj));
	}
	
	@Test
	public void testUniqueEventUseNative() {
		QEvent[] createdObj = {null};
		SmartPointerTest object = new SmartPointerTest(){
			@Override
			public @Nullable QEvent createUniqueEvent() {
				QEvent uobj = super.createUniqueEvent();
				createdObj[0] = uobj;
				assertFalse(uobj.isDisposed());
				assertTrue(uobj instanceof QDynamicPropertyChangeEvent);
				assertEquals("UniqueEvent", ((QDynamicPropertyChangeEvent)uobj).propertyName().toString());
				assertTrue(General.internalAccess.isJavaOwnership(uobj));
				return uobj;
			}
			
			@Override
			public void analyze(@Nullable QEvent obj) {
				assertTrue(createdObj[0].isDisposed());
				assertFalse(obj.isDisposed());
				assertTrue(createdObj[0]!=obj);
				assertTrue(obj instanceof QDynamicPropertyChangeEvent);
				assertEquals("UniqueEvent", ((QDynamicPropertyChangeEvent)obj).propertyName().toString());
				assertTrue(General.internalAccess.isSplitOwnership(obj));				
			}
			
		};
		object.useUniqueEvent();
		assertTrue(createdObj[0]!=null);
		assertTrue(createdObj[0].isDisposed());
	}
	
	@Test
	public void testUniqueEventUseJava() {
		QEvent[] createdObj = {null};
		SmartPointerTest object = new SmartPointerTest(){
			@Override
			public @Nullable QEvent createUniqueEvent() {
				QEvent uobj = new QStatusTipEvent("UniqueEvent");
				createdObj[0] = uobj;
				assertTrue(General.internalAccess.isJavaOwnership(uobj));
				return uobj;
			}
			
			@Override
			public void analyze(@Nullable QEvent obj) {
				assertFalse(obj.isDisposed());
				assertTrue(createdObj[0]==obj);
				assertTrue(obj instanceof QStatusTipEvent);
				assertEquals("UniqueEvent", ((QStatusTipEvent)obj).tip().toString());
				assertTrue(General.internalAccess.isCppOwnership(obj));				
			}
			
		};
		object.useUniqueEvent();
		assertTrue(createdObj[0]!=null);
		assertTrue(createdObj[0].isDisposed());
	}
}
