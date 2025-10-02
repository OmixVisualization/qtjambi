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

import java.util.concurrent.atomic.AtomicInteger;

import org.junit.Assert;
import org.junit.Test;

import io.qt.QtObject;
import io.qt.autotests.TestMetaObject.*;
import io.qt.autotests.generated.*;
import io.qt.core.*;

public class TestMetacalls extends ApplicationInitializer {
	
	@Test
    public void test_QObject() {
		QMetaObject.forType(SubQObject.class);
		QMetaObject.forType(SubQObject2.class);
		Object[] args = {null, 1, true, "Test1"};
		Assert.assertTrue(SubQObject.staticMetaObject.static_metacall(QMetaObject.Call.CreateInstance, 1, args)<1);
		Assert.assertTrue(args[0] instanceof SubQObject);
		Assert.assertTrue(General.internalAccess.isJavaOwnership((QObject)args[0]));
		SubQObject subQObject = (SubQObject)args[0];
		AtomicInteger counter = new AtomicInteger();
		subQObject.dataChanged.connect(counter::incrementAndGet);
		Assert.assertEquals("Test1", subQObject.s());
		Assert.assertEquals(0, counter.get());
		args = new Object[]{null, 2, true, "Test2"};
		QMetaMethod method = SubQObject.staticMetaObject.method("initialize", int.class, boolean.class, String.class);
		Assert.assertTrue(QMetaObject.metacall(subQObject, QMetaObject.Call.InvokeMetaMethod, method.methodIndex(), args)<1);
		Assert.assertEquals(true, args[0]);
		Assert.assertEquals(1, counter.get());
		args = new Object[]{null};
		QMetaProperty property = SubQObject.staticMetaObject.property("s");
		Assert.assertTrue(QMetaObject.metacall(subQObject, QMetaObject.Call.ReadProperty, property.propertyIndex(), args)<1);
		Assert.assertEquals("Test2", args[0]);
		Assert.assertEquals(1, counter.get());
		args[0] = "Test3";
		Assert.assertTrue(QMetaObject.metacall(subQObject, QMetaObject.Call.WriteProperty, property.propertyIndex(), args)<1);
		Assert.assertEquals("Test3", subQObject.s());
		Assert.assertEquals(2, counter.get());
		QMetaObject.activate(subQObject, 3, null);
		Assert.assertEquals(3, counter.get());
	}
	
	@Test
    public void test_QtObject() {
		Object[] args = {null, 1, false};
		Assert.assertTrue(SubQtObject.staticMetaObject.static_metacall(QMetaObject.Call.CreateInstance, 0, args)<1);
		Assert.assertTrue(args[0] instanceof SubQtObject);
		Assert.assertTrue(General.internalAccess.isJavaOwnership((QtObject)args[0]));
    }
	
	@Test
    public void test_NonQtType() {
		Object[] args = {null, 5, "Test5"};
		Assert.assertTrue(NonQtType.staticMetaObject.static_metacall(QMetaObject.Call.CreateInstance, 0, args)<1);
		Assert.assertTrue(args[0] instanceof NonQtType);
		NonQtType nonQtType = (NonQtType)args[0];
		Assert.assertEquals("Test5", nonQtType.getText());
		Assert.assertEquals(5, nonQtType.getIntegerNumber());
		QMetaProperty property = NonQtType.staticMetaObject.property("text");
		Assert.assertEquals("Test5", property.readOnGadget(nonQtType));
		Assert.assertTrue(property.writeOnGadget(nonQtType, "Test3"));
		Assert.assertEquals("Test3", nonQtType.getText());
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestMetacalls.class.getName());
    }
}
