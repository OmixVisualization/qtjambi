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

import org.junit.Assert;
import org.junit.Test;

import io.qt.*;
import io.qt.autotests.TestMetaObject.*;
import io.qt.autotests.generated.*;
import io.qt.core.*;

public class TestMetacallsQt66 extends ApplicationInitializer {
	
	public static class MultiInheritanceObject extends QObject implements TestInterface
	{
		public static final @NonNull QMetaObject staticMetaObject = QMetaObject.forType(MultiInheritanceObject.class);
		@NativeAccess
		private MultiInheritanceObject(QtConstructInPlace dc) {
			super((QPrivateConstructor)null);
			if(dc.matches(QObject.class, String.class)) {
				dc.begin(QObject.class).addIndex(0).begin(TestInterface.class).addIndex(1).initialize(this);
				stringArgument = dc.argumentAt(1, String.class);
			}
			else if(dc.matches(QObject.class)) {
				dc.begin(QObject.class).addIndex(0).begin(TestInterface.class).add("Default").initialize(this);
				stringArgument = "Default";
			}
			else if(dc.matches(String.class)) {
				dc.begin(TestInterface.class).addIndex(0).initialize(this);
				stringArgument = dc.argumentAt(0, String.class);
			}
			else {
				dc.begin(TestInterface.class).add("Default").initialize(this);
				stringArgument = "Default";
			}
		}
		
		@QtInvokable
		public MultiInheritanceObject(QObject parent, String arg) {
			QtUtilities.initializeNativeObject(this, QtArgument.begin(QObject.class).add(parent).begin(TestInterface.class).add(arg));
		}
		
		@QtInvokable
		public MultiInheritanceObject(QObject parent) {
			QtUtilities.initializeNativeObject(this, QtArgument.begin(QObject.class).add(parent).begin(TestInterface.class).add("Default"));
		}
		
		@QtInvokable
		public MultiInheritanceObject(String arg) {
			QtUtilities.initializeNativeObject(this, QtArgument.begin(TestInterface.class).add(arg));
		}
		
		@QtInvokable
		public MultiInheritanceObject() {
			QtUtilities.initializeNativeObject(this, QtArgument.begin(TestInterface.class).add("Default"));
		}

		@Override
		public boolean setReferenceCountTest1(QObject object) {
			return false;
		}

		@Override
		public String method4() {
			return "TestObjectExn2::method4";
		}

		@Override
		public String method5() {
			return "TestObjectExn2::method5";
		}
		
		String stringArgument;
	};
	
	@Test
    public void test_instantiateInPlace_QObject() throws NoSuchMethodException {
		QMetaMethod constructor = SubQObject.staticMetaObject.constructor(int.class, boolean.class, String.class);
		Assert.assertTrue(constructor.isValid());
		Object result = PropertyAndMethodCallTest.instantiateInPlace(constructor, 1, true, "Test1");
		Assert.assertTrue(result instanceof SubQObject);
		SubQObject subQObject = (SubQObject)result;
		Assert.assertTrue(General.internalAccess.isJavaOwnership(subQObject));
		Assert.assertEquals("Test1", subQObject.s());
		Assert.assertEquals(1, subQObject.i());
		subQObject.dispose();
		constructor = SubQObject.staticMetaObject.constructor(int.class, boolean.class);
		Assert.assertTrue(constructor.isValid());
		result = PropertyAndMethodCallTest.instantiateInPlace(constructor, 2, true);
		Assert.assertTrue(result instanceof SubQObject);
		subQObject = (SubQObject)result;
		Assert.assertTrue(General.internalAccess.isJavaOwnership(subQObject));
		Assert.assertEquals("", subQObject.s());
		Assert.assertEquals(2, subQObject.i());
		subQObject.dispose();
		constructor = SubQObject.staticMetaObject.constructor(QObject.class, int.class, boolean.class, String.class);
		Assert.assertTrue(constructor.isValid());
		QObject parent = new QObject();
		try {
			result = PropertyAndMethodCallTest.instantiateInPlace(constructor, parent, 1, true, "Test1");
			Assert.assertTrue(result instanceof SubQObject);
			subQObject = (SubQObject)result;
			Assert.assertEquals(parent, subQObject.parent());
			Assert.assertFalse(General.internalAccess.isJavaOwnership(subQObject));
			Assert.assertEquals("Test1", subQObject.s());
			Assert.assertEquals(1, subQObject.i());
			constructor = SubQObject.staticMetaObject.constructor(QObject.class, int.class, boolean.class);
			Assert.assertTrue(constructor.isValid());
			result = PropertyAndMethodCallTest.instantiateInPlace(constructor, parent, 2, true);
			Assert.assertTrue(result instanceof SubQObject);
			subQObject = (SubQObject)result;
			Assert.assertEquals(parent, subQObject.parent());
			Assert.assertFalse(General.internalAccess.isJavaOwnership(subQObject));
			Assert.assertEquals("", subQObject.s());
			Assert.assertEquals(2, subQObject.i());
			Assert.assertEquals(parent, subQObject.parent());
			constructor = SubQObject2.staticMetaObject.constructor(boolean.class, int.class, String.class, double.class);
			Assert.assertTrue(constructor.isValid());
			result = PropertyAndMethodCallTest.instantiateInPlace(constructor, true, 1, "Test1", 2.2);
			Assert.assertTrue(result instanceof SubQObject2);
			SubQObject2 subQObject2 = (SubQObject2)result;
			Assert.assertTrue(General.internalAccess.isJavaOwnership(subQObject2));
			Assert.assertEquals("Test1", subQObject2.s());
			Assert.assertEquals(1, subQObject2.i());
			Assert.assertEquals(0.1, 2.2, subQObject2.d());
			subQObject2.dispose();
			constructor = SubQObject2.staticMetaObject.constructor(boolean.class, int.class, double.class, QObject.class);
			Assert.assertTrue(constructor.isValid());
			result = PropertyAndMethodCallTest.instantiateInPlace(constructor, false, 3, 5.5, parent);
			Assert.assertTrue(result instanceof SubQObject2);
			subQObject2 = (SubQObject2)result;
			Assert.assertEquals(parent, subQObject2.parent());
			Assert.assertFalse(General.internalAccess.isJavaOwnership(subQObject2));
			Assert.assertEquals("", subQObject2.s());
			Assert.assertEquals(3, subQObject2.i());
			Assert.assertEquals(0.1, 5.5, subQObject2.d());
			
			constructor = MultiInheritanceObject.staticMetaObject.constructor(String.class);
			Assert.assertTrue(constructor.isValid());
			result = PropertyAndMethodCallTest.instantiateInPlace(constructor, "Test1");
			Assert.assertTrue(result instanceof MultiInheritanceObject);
			MultiInheritanceObject miObject = (MultiInheritanceObject)result;
			Assert.assertEquals(null, miObject.parent());
			Assert.assertEquals("Test1", miObject.stringArgument);
			
			constructor = MultiInheritanceObject.staticMetaObject.constructor(QObject.class, String.class);
			Assert.assertTrue(constructor.isValid());
			result = PropertyAndMethodCallTest.instantiateInPlace(constructor, parent, "Test1");
			Assert.assertTrue(result instanceof MultiInheritanceObject);
			miObject = (MultiInheritanceObject)result;
			Assert.assertEquals(parent, miObject.parent());
			Assert.assertEquals("Test1", miObject.stringArgument);
			
			constructor = MultiInheritanceObject.staticMetaObject.constructor(QObject.class);
			Assert.assertTrue(constructor.isValid());
			result = PropertyAndMethodCallTest.instantiateInPlace(constructor, parent);
			Assert.assertTrue(result instanceof MultiInheritanceObject);
			miObject = (MultiInheritanceObject)result;
			Assert.assertEquals(parent, miObject.parent());
			Assert.assertEquals("Default", miObject.stringArgument);
			
			constructor = MultiInheritanceObject.staticMetaObject.constructor();
			Assert.assertTrue(constructor.isValid());
			result = PropertyAndMethodCallTest.instantiateInPlace(constructor);
			Assert.assertTrue(result instanceof MultiInheritanceObject);
			miObject = (MultiInheritanceObject)result;
			Assert.assertEquals(null, miObject.parent());
			Assert.assertEquals("Default", miObject.stringArgument);
		}finally {
			parent.dispose();
		}
    }
	
	@Test
    public void test_QtObject() {
		QMetaMethod constructor = SubQtObject.staticMetaObject.constructor(int.class, boolean.class);
		Assert.assertTrue(constructor.isValid());
		Object result = PropertyAndMethodCallTest.instantiateInPlace(constructor, 1, false); 
		Assert.assertTrue(result instanceof SubQtObject);
		Assert.assertTrue(General.internalAccess.isJavaOwnership((QtObject)result));
    }
	
	@Test
    public void test_NonQtType() {
		QMetaMethod constructor = NonQtType.staticMetaObject.constructor(int.class, String.class);
		Assert.assertTrue(constructor.isValid());
		Object result = PropertyAndMethodCallTest.instantiateInPlace(constructor, 5, "Test5"); 
		Assert.assertTrue(result instanceof NonQtType);
		NonQtType nonQtType = (NonQtType)result;
		Assert.assertEquals("Test5", nonQtType.getText());
		Assert.assertEquals(5, nonQtType.getIntegerNumber());
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestMetacallsQt66.class.getName());
    }
}
