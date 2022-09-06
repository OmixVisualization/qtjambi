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

import org.junit.Assert;
import org.junit.Test;

import io.qt.core.QByteArray;
import io.qt.core.QCoreApplication;
import io.qt.core.QIntProperty;
import io.qt.core.QObject;
import io.qt.qml.QJSEngine;
import io.qt.qml.QJSValue;
import io.qt.qml.QQmlComponent;
import io.qt.qml.QQmlEngine;
import io.qt.qml.QtQml;

public class TestQmlQt6 extends ApplicationInitializer{
	
	@Test
    public void testJSValue() {
		QJSEngine engine = new QJSEngine();
		QJSValue value = engine.toScriptValue(5.4);
		Assert.assertTrue(value.isNumber());
		Assert.assertEquals(0.000001, 5.4, value.toDouble());
		value = engine.toScriptValue(QCoreApplication.instance());
		Assert.assertTrue(value.isObject());
		QObject result = engine.fromScriptValue(value, QObject.class);
		Assert.assertEquals(QCoreApplication.instance(), result);
	}
	
	static class Singleton extends QObject{
		public final QProperty<String> text = new QProperty<>("Hello, world!");
		
		private int m_number = 8;

		public void setNumber(int number) {
			if(this.m_number != number) {
				this.m_number = number;
				this.number.notifyProperty();
			}
		}
		public final QComputedIntProperty number = new QComputedIntProperty(()->this.m_number);
	}
	
	@Test
    public void run_testSingletonInstance() {
		QtQml.qmlClearTypeRegistrations();
		Singleton s = new Singleton();
		QtQml.qmlRegisterSingletonInstance(s, "io.qt.test", 1, 0, "Singleton");
		QByteArray data = new QByteArray("import io.qt.test 1.0\n" + 
				"import QtQuick 2.0\n" +
				"Item{\n" + 
				"    property string text: Singleton.text\n" + 
				"    property int number: Singleton.number\n" + 
				"}");
		QQmlEngine engine = new QQmlEngine();
		QQmlComponent component = new QQmlComponent(engine);
		component.setData(data, null);
		Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
		Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
		QObject root = component.create();
		Assert.assertEquals("Hello, world!", root.property("text"));
		s.text.setValue("Hello, world?");
		Assert.assertEquals("Hello, world?", root.property("text"));
		QIntProperty iprop = new QIntProperty();
		iprop.setBinding(()->s.number.value());
		Assert.assertEquals(8, iprop.value());
		Assert.assertEquals(8, root.property("number"));
		s.setNumber(29);
		Assert.assertEquals(29, iprop.value());
		Assert.assertEquals(29, root.property("number"));
	}
}
