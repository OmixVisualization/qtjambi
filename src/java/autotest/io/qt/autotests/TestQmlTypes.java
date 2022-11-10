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
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.autotests.qmlreg.LineEditForeignExtendedByJavaWrapper;
import io.qt.autotests.qmlreg.LineEditForeignExtendedByNativeObjectWrapper;
import io.qt.autotests.qmlreg.LineEditForeignExtendedByQObjectWrapper;
import io.qt.core.QByteArray;
import io.qt.core.QList;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaProperty;
import io.qt.core.QObject;
import io.qt.qml.QQmlComponent;
import io.qt.qml.QQmlEngine;
import io.qt.qml.QQmlError;
import io.qt.qml.QtQml;
import io.qt.qml.util.QmlElement;
import io.qt.qml.util.QmlTypeRegistrationException;
import io.qt.qml.util.QmlTypes;
import io.qt.widgets.QLineEdit;

public class TestQmlTypes extends ApplicationInitializer{
	@BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitializeWithWidgets();
		Assume.assumeTrue(io.qt.QtUtilities.initializePackage("io.qt.quick"));
	}

	@Test
    public void run_testQmlTypes() {
		QtQml.qmlClearTypeRegistrations();
		QmlTypes.registerPackage("io.qt.autotests.qmltypes");
		QByteArray data = new QByteArray("import io.qt.autotests.qmltypes 5.0; Car {}");
		QQmlEngine qmlengine = new QQmlEngine();
		QQmlComponent component = new QQmlComponent(qmlengine);
		component.setData(data, null);
		QObject car = component.create();
		Assert.assertFalse(component.errorString(), component.isError());
		Assert.assertTrue("car is null", car!=null);
		QMetaObject carType = car.metaObject();
		Assert.assertTrue(carType.className().startsWith("Car_QMLTYPE_"));
		QMetaProperty engineProp = carType.property("engine");
		Assert.assertTrue("engine property not available", engineProp!=null);
		QMetaProperty wheelsProp = carType.property("wheels");
		Assert.assertTrue("wheels property not available", wheelsProp!=null);
		Assert.assertEquals("io::qt::autotests::qmltypes::Engine*", engineProp.typeName());
		Assert.assertTrue(wheelsProp.typeName().startsWith("QQmlListProperty<Wheel_QMLTYPE_") && wheelsProp.typeName().endsWith(">"));
		Assert.assertEquals(io.qt.autotests.qmltypes.Engine.class, engineProp.classType());
		Assert.assertEquals(io.qt.qml.QQmlListProperty.class, wheelsProp.classType());
		Object engine = car.property("engine");
		Object wheels = car.property("wheels");
		Assert.assertTrue("engine is null", engine!=null);
		Assert.assertTrue("wheels is null", wheels!=null);
		Assert.assertTrue("engine has wrong type: "+engine.getClass().getName(), engine instanceof io.qt.autotests.qmltypes.Engine);
		Assert.assertEquals(150, ((io.qt.autotests.qmltypes.Engine)engine).getPerformance());
		Assert.assertTrue("wheels has wrong type: "+wheels.getClass().getName(), wheels instanceof io.qt.qml.QQmlListProperty);
		io.qt.qml.QQmlListProperty<?> wheelsList = (io.qt.qml.QQmlListProperty<?>)wheels;
		Assert.assertEquals(4, wheelsList.count());
		for (int i = 0; i < wheelsList.count(); i++) {
			QObject wheel = (QObject)wheelsList.at(i);
			Assert.assertEquals(52., wheel.property("radius"));
		}
		car.setProperty("wheelRadius", 53);
		for (int i = 0; i < wheelsList.count(); i++) {
			QObject wheel = (QObject)wheelsList.at(i);
			Assert.assertEquals(53., wheel.property("radius"));
		}
	}
	
	@Test
    public void testForeignQObjectExtendedByQObject() {
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterTypesAndRevisions("io.qt.widgets", 6, LineEditForeignExtendedByQObjectWrapper.class);
		QByteArray data = new QByteArray("import io.qt.widgets 6.0\n" + 
				"QLineEdit {\n" + 
				"    leftMargin: 20\n" + 
				"}");
		QQmlEngine engine = new QQmlEngine();
		engine.setOutputWarningsToStandardError(true);
		engine.warnings.connect(warnings->{
			for(QQmlError warning : warnings) {
				System.out.println(warning);
			}
		});
		
		QQmlComponent component = new QQmlComponent(engine);
		component.setData(data, null);
		Assert.assertEquals(component.errorString().trim(), QQmlComponent.Status.Ready, component.status());
		Assert.assertEquals(component.errorString().trim(), 0, component.errors().size());
		QObject root = component.create();
		Assert.assertTrue(root instanceof QLineEdit);
		QLineEdit lineEdit = (QLineEdit)root;
		Assert.assertEquals(20, lineEdit.textMargins().left());
	}
	
	@Test
    public void testForeignQObjectExtendedByNativeObject() {
		QtQml.qmlClearTypeRegistrations();
		try {
			QtQml.qmlRegisterTypesAndRevisions("io.qt.widgets", 6, LineEditForeignExtendedByNativeObjectWrapper.class);
			Assert.fail("QmlTypeRegistrationException expected");
		} catch (QmlTypeRegistrationException e) {
		}
	}
	
	@Test
    public void testForeignQObjectExtendedByJavaWrapper() {
		QtQml.qmlClearTypeRegistrations();
		try {
			QtQml.qmlRegisterTypesAndRevisions("io.qt.widgets", 6, LineEditForeignExtendedByJavaWrapper.class);
			Assert.fail("QmlTypeRegistrationException expected");
		} catch (QmlTypeRegistrationException e) {
		}
	}
	
	@QmlElement
	static class MyObject extends QObject{
		public MyObject(QDeclarativeConstructor constructor) {
			super(constructor);
			lineEdits.add(new QLineEdit());
			lineEdits.add(null);
			lineEdits.add(null);
			lineEdits.add(null);
		}

		private QList<QLineEdit> lineEdits = new QList<>(QLineEdit.class);

		public QList<QLineEdit> lineEdits() {
			return lineEdits;
		}

		public void setLineEdits(QList<QLineEdit> lineEdits) {
			this.lineEdits = lineEdits;
		}
	}
}
