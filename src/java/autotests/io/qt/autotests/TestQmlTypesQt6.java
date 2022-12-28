/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.autotests.TestQmlTypes.MyObject;
import io.qt.autotests.qmlreg.FormatRangeForeignExtendedByJavaWrapper;
import io.qt.autotests.qmlreg.FormatRangeForeignExtendedByNativeObjectWrapper;
import io.qt.autotests.qmlreg.FormatRangeForeignExtendedByQObjectWrapper;
import io.qt.autotests.qmlreg.LineEditContainerForeign;
import io.qt.autotests.qmlreg.LineEditForeignExtendedByQObjectWrapper;
import io.qt.autotests.qmlreg.PointF;
import io.qt.autotests.qmlreg.PointFForeignExtendedByJavaMirror;
import io.qt.autotests.qmlreg.PointFForeignExtendedByJavaWrapper;
import io.qt.autotests.qmlreg.PointFForeignExtendedByNativeObjectWrapper;
import io.qt.autotests.qmlreg.PointFForeignExtendedByQObjectWrapper;
import io.qt.core.QByteArray;
import io.qt.core.QLibraryInfo;
import io.qt.core.QObject;
import io.qt.core.QVersionNumber;
import io.qt.gui.QTextLayout;
import io.qt.qml.QQmlComponent;
import io.qt.qml.QQmlEngine;
import io.qt.qml.QQmlError;
import io.qt.qml.QtQml;
import io.qt.qml.util.QmlTypeRegistrationException;
import io.qt.quick.QQuickItem;
import io.qt.widgets.QLineEdit;

public class TestQmlTypesQt6 extends ApplicationInitializer{
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		Assume.assumeTrue("Qt version >= 6.4", QLibraryInfo.version().compareTo(new QVersionNumber(6,4))>=0);
		ApplicationInitializer.testInitializeWithWidgets();
		Assume.assumeTrue(io.qt.QtUtilities.initializePackage("io.qt.quick"));
	}
	
	@Test
    public void testForeignJavaValueExtendedByJavaMirror() {
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterTypesAndRevisions("values", 1, PointFForeignExtendedByJavaMirror.class);
		QByteArray data = new QByteArray("import values 1.0\n" + 
				"import QtQuick\n" + 
				"Item {\n" + 
				"    property pointf p\n" +
				"    function test1(){\n" + 
				"        p.x = 50;\n" + 
				"        p.y = 30;\n" + 
				"    }\n" + 
				"    \n" + 
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
		Assert.assertTrue(root instanceof QQuickItem);
		Object pointf = root.property("p");
		Assert.assertTrue(pointf instanceof PointF);
		Assert.assertEquals(0, ((PointF)pointf).x(), 0.000001);
		Assert.assertEquals(0, ((PointF)pointf).y(), 0.000001);
		root.metaObject().method("test1").invoke(root);
		pointf = root.property("p");
		Assert.assertEquals(50, ((PointF)pointf).x(), 0.000001);
		Assert.assertEquals(30, ((PointF)pointf).y(), 0.000001);
	}
	
	@Test
    public void testForeignJavaValueExtendedByJavaWrapper() {
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterTypesAndRevisions("values", 1, PointFForeignExtendedByJavaWrapper.class);
		QByteArray data = new QByteArray("import values 1.0\n" + 
				"import QtQuick\n" + 
				"Item {\n" + 
				"    property pointf p\n" +
				"    function test1(){\n" + 
				"        p.x = 50;\n" + 
				"        p.y = 30;\n" + 
				"    }\n" + 
				"    \n" + 
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
		Assert.assertTrue(root instanceof QQuickItem);
		Object pointf = root.property("p");
		Assert.assertTrue(pointf instanceof PointF);
		Assert.assertEquals(0, ((PointF)pointf).x(), 0.000001);
		Assert.assertEquals(0, ((PointF)pointf).y(), 0.000001);
		root.metaObject().method("test1").invoke(root);
		pointf = root.property("p");
		Assert.assertEquals(50, ((PointF)pointf).x(), 0.000001);
		Assert.assertEquals(30, ((PointF)pointf).y(), 0.000001);
	}
	
	@Test
    public void testForeignJavaValueExtendedByNativeObjectClass() {
		QtQml.qmlClearTypeRegistrations();
		try {
			QtQml.qmlRegisterTypesAndRevisions("values", 1, PointFForeignExtendedByNativeObjectWrapper.class);
			Assert.fail("QmlTypeRegistrationException expected");
		} catch (QmlTypeRegistrationException e) {
		}
	}
	
	@Test
    public void testForeignJavaValueExtendedByQObjectClass() {
		QtQml.qmlClearTypeRegistrations();
		try {
			QtQml.qmlRegisterTypesAndRevisions("values", 1, PointFForeignExtendedByQObjectWrapper.class);
			Assert.fail("QmlTypeRegistrationException expected");
		} catch (QmlTypeRegistrationException e) {
		}
	}
	
	@Test
    public void testForeignNativeValueExtendedByNativeWrapper() {
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterTypesAndRevisions("values", 1, FormatRangeForeignExtendedByNativeObjectWrapper.class);
		QByteArray data = new QByteArray("import values 1.0\n" + 
				"import QtQuick\n" + 
				"Item {\n" + 
				"    property formatrange f\n" +
				"    function test1(){\n" + 
				"        f.length = 50;\n" + 
				"        f.start = 30;\n" + 
				"    }\n" + 
				"    \n" + 
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
		Assert.assertTrue(root instanceof QQuickItem);
		Object v = root.property("f");
		Assert.assertTrue(v instanceof QTextLayout.FormatRange);
		Assert.assertEquals(0, ((QTextLayout.FormatRange)v).length());
		Assert.assertEquals(0, ((QTextLayout.FormatRange)v).start());
		root.metaObject().method("test1").invoke(root);
		v = root.property("f");
		Assert.assertEquals(50, ((QTextLayout.FormatRange)v).length());
		Assert.assertEquals(30, ((QTextLayout.FormatRange)v).start());
	}
	
	@Test
    public void testForeignNativeValueExtendedByQObjectWrapper() {
		QtQml.qmlClearTypeRegistrations();
		try {
			QtQml.qmlRegisterTypesAndRevisions("values", 1, FormatRangeForeignExtendedByQObjectWrapper.class);
			Assert.fail("QmlTypeRegistrationException expected");
		} catch (QmlTypeRegistrationException e) {
		}
	}
	
	@Test
    public void testForeignNativeValueExtendedByJavaWrapper() {
		QtQml.qmlClearTypeRegistrations();
		try {
			QtQml.qmlRegisterTypesAndRevisions("values", 1, FormatRangeForeignExtendedByJavaWrapper.class);
			Assert.fail("QmlTypeRegistrationException expected");
		} catch (QmlTypeRegistrationException e) {
		}
	}
	
	@Test
    public void testContainer() {
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterTypesAndRevisions("values", 1, LineEditContainerForeign.class);
		QtQml.qmlRegisterTypesAndRevisions("values", 1, MyObject.class);
		QtQml.qmlRegisterTypesAndRevisions("io.qt.widgets", 6, LineEditForeignExtendedByQObjectWrapper.class);
		QByteArray data = new QByteArray("import values 1.0\n" + 
				"import QtQuick\n" + 
				"import io.qt.widgets 6.0\n" + 
				"QtObject {\n" + 
				"    property MyObject object: MyObject{}\n" +
				"    function lineEditSize(){\n" +
				"        return object.lineEdits.length;\n" +
				"    }\n" +
				"    function getLineEdit(i){\n" +
				"        return object.lineEdits[i];\n" +
				"    }\n" +
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
		Assert.assertTrue(root!=null);
		int size = (int)root.metaObject().method("lineEditSize").invoke(root);
		Assert.assertEquals(4, size);
		Object result = root.metaObject().method("getLineEdit", Object.class).invoke(root, 0);
		Assert.assertTrue(result instanceof QLineEdit);
	}
}
