/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.BeforeClass;

import io.qt.QtUtilities;
import io.qt.core.QObject;
import io.qt.core.QUrl;
import io.qt.gui.QMatrix4x4;
import io.qt.qml.QQmlComponent;
import io.qt.qml.QQmlComponent.Status;
import io.qt.qml.QQmlEngine;
import io.qt.qml.QQmlListProperty;
import io.qt.quick.QQuickItem;
import io.qt.quick.QQuickTransform;

public class TestQQmlListProperty extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithGui();
    }

	private QQmlEngine engine;
	private QQmlComponent component;
	private QQuickItem object;
	private Status status;
	private String errorString;
	
	@org.junit.Before
	public void setUp() {
		// this is necessary to properly load QtQuick library:
		QtUtilities.initializePackage("io.qt.quick");
		QtUtilities.initializePackage("io.qt.network");
		engine = new QQmlEngine();
	    component = new QQmlComponent(engine);
	    component.setData(
	                "import QtQuick 2.5\n"+
	                "Text { text: \"Hello world!\"\n"+
	                "transform: [Rotation { origin.x: 25; origin.y: 25; angle: 45},\n"+
	                "Scale { origin.x: 25; origin.y: 25; xScale: 3}] }",
	                new QUrl());
	    status = component.status();
	    errorString = component.errorString();
	    object = QQmlComponent.Status.Ready==component.status() ? component.create(QQuickItem.class) : null;
	}

	@org.junit.After
	public void tearDown() {
		engine = null;
		component = null;
//		if(object!=null)
//			object.dispose();
		object = null;
	}

	@org.junit.Test
	public void testTransform1() {
		assertEquals(errorString, Status.Ready, status);
		assertTrue("object instanceof QQuickItem", object instanceof QQuickItem);
		QQmlListProperty<QQuickTransform> transformList = object.transform();
		assertFalse("transformList==null", transformList==null);
		assertTrue("canCount", transformList.canCount());
		assertTrue("canAt", transformList.canAt());
		assertTrue("canAppend", transformList.canAppend());
		assertTrue("canClear", transformList.canClear());
		assertEquals("count", 2, transformList.count());		
	}
	
	@org.junit.Test
	public void testTransform2() {
		assertEquals(errorString, Status.Ready, status);
		assertTrue("object instanceof QQuickItem", object instanceof QQuickItem);
		QQmlListProperty<QQuickTransform> transformList = object.transform();
		QQuickTransform transform = transformList.at(0);
		QMatrix4x4 matrix = new QMatrix4x4();
		transform.applyTo(matrix);
		QObject parent = transform.parent();
		while(parent!=null) {
			System.out.println(parent);
			parent = parent.parent();
		}
		assertEquals("[0,0]", 0.707107,		matrix.value(0, 0), 0.001);
		assertEquals("[0,1]", -0.707107,	matrix.value(0, 1), 0.001);
		assertEquals("[0,2]", 0, 			matrix.value(0, 2), 0.001);
		assertEquals("[0,3]", 25,			matrix.value(0, 3), 0.001);
		assertEquals("[1,0]", 0.707107, 	matrix.value(1, 0), 0.001);
		assertEquals("[1,1]", 0.707107, 	matrix.value(1, 1), 0.001);
		assertEquals("[1,2]", 0,		 	matrix.value(1, 2), 0.001);
		assertEquals("[1,3]", -10.3553, 	matrix.value(1, 3), 0.001);
		assertEquals("[2,0]", 0,		 	matrix.value(2, 0), 0.001);
		assertEquals("[2,1]", 0,		 	matrix.value(2, 1), 0.001);
		assertEquals("[2,2]", 1,		 	matrix.value(2, 2), 0.001);
		assertEquals("[2,3]", 0,		 	matrix.value(2, 3), 0.001);
		assertEquals("[3,0]", 0,		 	matrix.value(3, 0), 0.001);
		assertEquals("[3,1]", 0,		 	matrix.value(3, 1), 0.001);
		assertEquals("[3,2]", 0,		 	matrix.value(3, 2), 0.001);
		assertEquals("[3,3]", 1,		 	matrix.value(3, 3), 0.001);
	}
		
	@org.junit.Test
	public void testTransform3() {
		assertEquals(errorString, Status.Ready, status);
		assertTrue("object instanceof QQuickItem", object instanceof QQuickItem);
		QQmlListProperty<QQuickTransform> transformList = object.transform();
		QQuickTransform transform = transformList.at(1);
		QMatrix4x4 matrix = new QMatrix4x4();
		transform.applyTo(matrix);
		assertEquals("[0,0]", 3,			matrix.value(0, 0), 0.001);
		assertEquals("[0,1]", 0,			matrix.value(0, 1), 0.001);
		assertEquals("[0,2]", 0, 			matrix.value(0, 2), 0.001);
		assertEquals("[0,3]", -50,			matrix.value(0, 3), 0.001);
		assertEquals("[1,0]", 0,		 	matrix.value(1, 0), 0.001);
		assertEquals("[1,1]", 1,		 	matrix.value(1, 1), 0.001);
		assertEquals("[1,2]", 0,		 	matrix.value(1, 2), 0.001);
		assertEquals("[1,3]", 0,		 	matrix.value(1, 3), 0.001);
		assertEquals("[2,0]", 0,		 	matrix.value(2, 0), 0.001);
		assertEquals("[2,1]", 0,		 	matrix.value(2, 1), 0.001);
		assertEquals("[2,2]", 1,		 	matrix.value(2, 2), 0.001);
		assertEquals("[2,3]", 0,		 	matrix.value(2, 3), 0.001);
		assertEquals("[3,0]", 0,		 	matrix.value(3, 0), 0.001);
		assertEquals("[3,1]", 0,		 	matrix.value(3, 1), 0.001);
		assertEquals("[3,2]", 0,		 	matrix.value(3, 2), 0.001);
		assertEquals("[3,3]", 1,		 	matrix.value(3, 3), 0.001);
	}
		
	@org.junit.Test
	public void testTransform4() {
		assertEquals(errorString, Status.Ready, status);
		assertTrue("object instanceof QQuickItem", object instanceof QQuickItem);
		QQmlListProperty<QQuickTransform> transformList = object.transform();
		component.setData(
                "import QtQuick 2.5\n" +
                "Rotation { origin.x: 0; origin.y: 0; angle: 180}",
                new QUrl());
		assertEquals(component.errorString().trim(), Status.Ready, component.status());
		QObject transformObject = component.create();
		assertTrue("object instanceof QQuickTransform", transformObject instanceof QQuickTransform);
		transformList.append((QQuickTransform)transformObject);
		assertEquals("count", 3, transformList.count());
		QQuickTransform transform = transformList.at(2);
		QMatrix4x4 matrix = new QMatrix4x4();
		transform.applyTo(matrix);
		assertEquals("[0,0]", -1,			matrix.value(0, 0), 0.001);
		assertEquals("[0,1]", 0,			matrix.value(0, 1), 0.001);
		assertEquals("[0,2]", 0, 			matrix.value(0, 2), 0.001);
		assertEquals("[0,3]", 0,			matrix.value(0, 3), 0.001);
		assertEquals("[1,0]", 0,		 	matrix.value(1, 0), 0.001);
		assertEquals("[1,1]", -1,		 	matrix.value(1, 1), 0.001);
		assertEquals("[1,2]", 0,		 	matrix.value(1, 2), 0.001);
		assertEquals("[1,3]", 0,		 	matrix.value(1, 3), 0.001);
		assertEquals("[2,0]", 0,		 	matrix.value(2, 0), 0.001);
		assertEquals("[2,1]", 0,		 	matrix.value(2, 1), 0.001);
		assertEquals("[2,2]", 1,		 	matrix.value(2, 2), 0.001);
		assertEquals("[2,3]", 0,		 	matrix.value(2, 3), 0.001);
		assertEquals("[3,0]", 0,		 	matrix.value(3, 0), 0.001);
		assertEquals("[3,1]", 0,		 	matrix.value(3, 1), 0.001);
		assertEquals("[3,2]", 0,		 	matrix.value(3, 2), 0.001);
		assertEquals("[3,3]", 1,		 	matrix.value(3, 3), 0.001);
	}
		
	@org.junit.Test
	public void testTransform5() {
		assertEquals(errorString, Status.Ready, status);
		assertTrue("object instanceof QQuickItem", object instanceof QQuickItem);
		QQmlListProperty<QQuickTransform> transformList = object.transform();
		transformList.clear();
		assertEquals("count", 0, transformList.count());
	}
}
