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

import java.util.ArrayList;
import java.util.List;

import org.junit.Assert;
import org.junit.Test;

import io.qt.autotests.generated.FlagsAndEnumTest;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaObject.AbstractPrivateSignal1;
import io.qt.core.QMetaType;
import io.qt.core.QObject;
import io.qt.core.QVariant;

public class TestMetaProgrammingQt69 extends ApplicationInitializer {
	
	@Test
    public void testUnknownEnums64_MethodsSignalsProperties() {
    	QObject hiddenType = FlagsAndEnumTest.createHiddenObject(null);
    	List<QMetaType.GenericLongEnumerator> hiddenClassEnums = new ArrayList<>();
    	List<QMetaType.GenericLongFlags> hiddenFlags = new ArrayList<>();
    	{
	    	AbstractPrivateSignal1<QMetaType.GenericLongEnumerator> sig = QMetaObject.findSignal(hiddenType, "hiddenEnum64Changed", QMetaType.GenericLongEnumerator.class);
	    	Assert.assertTrue(sig!=null);
	    	sig.connect(hiddenClassEnums::add);
	    }
    	{
	    	AbstractPrivateSignal1<QMetaType.GenericLongFlags> sig = QMetaObject.findSignal(hiddenType, "hiddenFlags64Changed", QMetaType.GenericLongFlags.class);
	    	Assert.assertTrue(sig!=null);
	    	sig.connect(hiddenFlags::add);
    	}
    	QMetaMethod method = hiddenType.metaObject().method("setHiddenEnum64", QMetaType.GenericLongEnumerator.class);
    	Assert.assertTrue(method!=null && method.isValid());
    	method.invoke(hiddenType, QVariant.convert(5, method.parameterType(0)));
    	method.invoke(hiddenType, QVariant.convert(9, method.parameterType(0)));
    	Assert.assertEquals(2, hiddenClassEnums.size());
    	Assert.assertEquals(5, hiddenClassEnums.get(0).value());
    	Assert.assertEquals(9, hiddenClassEnums.get(1).value());
    	
    	method = hiddenType.metaObject().method("setHiddenFlags64", QMetaType.GenericLongFlags.class);
    	Assert.assertTrue(method!=null && method.isValid());
    	method.invoke(hiddenType, 6);
    	method.invoke(hiddenType, 2);
    	Assert.assertEquals(2, hiddenFlags.size());
    	Assert.assertEquals(6, hiddenFlags.get(0).value());
    	Assert.assertEquals(2, hiddenFlags.get(1).value());
    	
    	method = hiddenType.metaObject().method("hiddenEnum64");
    	Assert.assertTrue(method!=null && method.isValid());
    	Assert.assertEquals(9, ((QMetaType.GenericLongEnumerator)method.invoke(hiddenType)).value());

    	method = hiddenType.metaObject().method("hiddenFlags64");
    	Assert.assertTrue(method!=null && method.isValid());
    	Assert.assertEquals(2, ((QMetaType.GenericLongFlags)method.invoke(hiddenType)).value());
    	
    	Assert.assertTrue(hiddenType.metaObject().property("hiddenEnum64")!=null);
    	Assert.assertTrue(hiddenType.metaObject().property("hiddenFlags64")!=null);
    	Assert.assertEquals(9, ((QMetaType.GenericLongEnumerator)hiddenType.property("hiddenEnum64")).value());
    	Assert.assertEquals(2, ((QMetaType.GenericLongFlags)hiddenType.property("hiddenFlags64")).value());

    	hiddenType.setProperty("hiddenEnum64", 89);
    	Assert.assertEquals(3, hiddenClassEnums.size());
    	Assert.assertEquals(5, hiddenClassEnums.get(0).value());
    	Assert.assertEquals(9, hiddenClassEnums.get(1).value());
    	Assert.assertEquals(89, hiddenClassEnums.get(2).value());
    	
    	hiddenType.setProperty("hiddenFlags64", 79);
    	Assert.assertEquals(3, hiddenFlags.size());
    	Assert.assertEquals(6, hiddenFlags.get(0).value());
    	Assert.assertEquals(2, hiddenFlags.get(1).value());
    	Assert.assertEquals(79, hiddenFlags.get(2).value());
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestMetaProgrammingQt69.class.getName());
    }
}
