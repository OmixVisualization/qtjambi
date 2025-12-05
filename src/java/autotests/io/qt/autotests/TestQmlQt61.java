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

import static io.qt.qml.QtQml.qmlClearTypeRegistrations;
import static io.qt.qml.QtQml.qmlRegisterType;

import org.junit.Assert;
import org.junit.Test;

import io.qt.autotests.TestQml4.AutoGadgetValueType;
import io.qt.autotests.TestQml4.CustomValueType;
import io.qt.autotests.generated.General;
import io.qt.core.QObject;
import io.qt.core.QVariant;
import io.qt.qml.QJSEngine;
import io.qt.qml.QJSManagedValue;
import io.qt.qml.QJSValue;
import io.qt.qml.QQmlEngine;
import io.qt.qml.QQmlError;

public class TestQmlQt61 extends ApplicationInitializer{
	@SuppressWarnings("rawtypes")
	@Test
    public void testQJSManagedValue() {
		qmlClearTypeRegistrations();
		qmlRegisterType((Class)AutoGadgetValueType.class, "io.qt.test", 1, 0, "autoGadgetValueType");
		qmlRegisterType((Class)CustomValueType.class, "io.qt.test", 1, 0, "customValueType");
		QQmlEngine engine = new QQmlEngine();
		engine.setOutputWarningsToStandardError(true);
		engine.warnings.connect(warnings->{
			for(QQmlError warning : warnings) {
				System.out.println(warning);//.messageType()+" "+warning.line()+" "+warning.description());
			}
		});
		
		{
			QJSValue jsv = engine.newArray(3);
			jsv.setProperty(0, 5);
			jsv.setProperty(1, 5.6);
			jsv.setProperty(2, "CREATE_TEST");
			AutoGadgetValueType v = engine.fromManagedValue(new QJSManagedValue(jsv, engine), AutoGadgetValueType.class);
			Assert.assertEquals(5, v.i);
			Assert.assertEquals(5.6, v.d, 0.0001);
			Assert.assertEquals("CREATE_TEST", v.s);
			
			jsv = engine.newObject();
			jsv.setProperty("i", 25);
			jsv.setProperty("d", 25.6);
			jsv.setProperty("s", "CREATE_TEST2");
			v = engine.fromManagedValue(new QJSManagedValue(jsv, engine), AutoGadgetValueType.class);
			Assert.assertEquals(25, v.i);
			Assert.assertEquals(25.6, v.d, 0.0001);
			Assert.assertEquals("CREATE_TEST2", v.s);
			v = QVariant.fromValue(jsv).value(AutoGadgetValueType.class);
			Assert.assertEquals(25, v.i);
			Assert.assertEquals(25.6, v.d, 0.0001);
			Assert.assertEquals("CREATE_TEST2", v.s);
		}
		{
			QJSValue jsv = engine.newArray(3);
			jsv.setProperty(0, 5);
			jsv.setProperty(1, 5.6);
			jsv.setProperty(2, "CREATE_TEST");
			CustomValueType v = engine.fromManagedValue(new QJSManagedValue(jsv, engine), CustomValueType.class);
			Assert.assertEquals(5, v.i);
			Assert.assertEquals(5.6, v.d, 0.0001);
			Assert.assertEquals("CREATE_TEST", v.s);
			
			jsv = engine.newObject();
			jsv.setProperty("i", 25);
			jsv.setProperty("d", 25.6);
			jsv.setProperty("s", "CREATE_TEST2");
			v = engine.fromManagedValue(new QJSManagedValue(jsv, engine), CustomValueType.class);
			Assert.assertEquals(25, v.i);
			Assert.assertEquals(25.6, v.d, 0.0001);
			Assert.assertEquals("CREATE_TEST2", v.s);
			v = QVariant.fromValue(jsv).value(CustomValueType.class);
			Assert.assertEquals(25, v.i);
			Assert.assertEquals(25.6, v.d, 0.0001);
			Assert.assertEquals("CREATE_TEST2", v.s);
		}
	}
	
	@Test
    public void testObjectOwnership() {
		QQmlEngine engine = new QQmlEngine();
		QObject scriptValueObject = new QObject();
		QJSManagedValue jsValue = engine.toManagedValue(scriptValueObject);
        Assert.assertEquals(QJSEngine.ObjectOwnership.JavaOwnership, QJSEngine.objectOwnership(scriptValueObject));
		Assert.assertTrue(General.internalAccess.isJavaOwnership(scriptValueObject));
        Assert.assertTrue(jsValue.isQObject());
        Assert.assertEquals(scriptValueObject, jsValue.toQObject());
        engine.dispose();
        Assert.assertFalse(scriptValueObject.isDisposed());
	}
}
