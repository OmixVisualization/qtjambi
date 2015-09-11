/**
 * Unit Test implementations for QQmlListProperty.java
 */

package org.qtjambi.unittests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Assert;
import org.qtjambi.autotests.QApplicationTest;
import org.qtjambi.qt.Utilities;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.qml.*;
import org.qtjambi.qt.quick.*;
import org.qtjambi.qt.widgets.QApplication;

public class TestQQmlListProperty extends QApplicationTest {

	private QQmlEngine engine;
	private QQmlComponent component;
	private QQuickItem object;
	
	@org.junit.Before
	public void setUp() {
		engine = new QQmlEngine();
	    component = new QQmlComponent(engine);
	    component.setData(
	                "import QtQuick 2.5\n"+
	                "Text { text: \"Hello world!\"\n"+
	                "transform: [Rotation { origin.x: 25; origin.y: 25; angle: 45},\n"+
	                "Scale { origin.x: 25; origin.y: 25; xScale: 3}] }",
	                new QUrl());
	    object = component.create(QQuickItem.class);
	}

	@org.junit.After
	public void tearDown() {
		engine = null;
		component = null;
		object = null;
	}

	@org.junit.Test
	public void testTransform1() {
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
		QQmlListProperty<QQuickTransform> transformList = object.transform();
		QQuickTransform transform = transformList.at(0);
		QMatrix4x4 matrix = new QMatrix4x4();
		transform.applyTo(matrix);
		assertEquals("[0,0]", 0.707107,		matrix.getValue(0, 0), 0.001);
		assertEquals("[0,1]", -0.707107,	matrix.getValue(0, 1), 0.001);
		assertEquals("[0,2]", 0, 			matrix.getValue(0, 2), 0.001);
		assertEquals("[0,3]", 25,			matrix.getValue(0, 3), 0.001);
		assertEquals("[1,0]", 0.707107, 	matrix.getValue(1, 0), 0.001);
		assertEquals("[1,1]", 0.707107, 	matrix.getValue(1, 1), 0.001);
		assertEquals("[1,2]", 0,		 	matrix.getValue(1, 2), 0.001);
		assertEquals("[1,3]", -10.3553, 	matrix.getValue(1, 3), 0.001);
		assertEquals("[2,0]", 0,		 	matrix.getValue(2, 0), 0.001);
		assertEquals("[2,1]", 0,		 	matrix.getValue(2, 1), 0.001);
		assertEquals("[2,2]", 1,		 	matrix.getValue(2, 2), 0.001);
		assertEquals("[2,3]", 0,		 	matrix.getValue(2, 3), 0.001);
		assertEquals("[3,0]", 0,		 	matrix.getValue(3, 0), 0.001);
		assertEquals("[3,1]", 0,		 	matrix.getValue(3, 1), 0.001);
		assertEquals("[3,2]", 0,		 	matrix.getValue(3, 2), 0.001);
		assertEquals("[3,3]", 1,		 	matrix.getValue(3, 3), 0.001);
	}
		
	@org.junit.Test
	public void testTransform3() {
		QQmlListProperty<QQuickTransform> transformList = object.transform();
		QQuickTransform transform = transformList.at(1);
		QMatrix4x4 matrix = new QMatrix4x4();
		transform.applyTo(matrix);
		assertEquals("[0,0]", 3,			matrix.getValue(0, 0), 0.001);
		assertEquals("[0,1]", 0,			matrix.getValue(0, 1), 0.001);
		assertEquals("[0,2]", 0, 			matrix.getValue(0, 2), 0.001);
		assertEquals("[0,3]", -50,			matrix.getValue(0, 3), 0.001);
		assertEquals("[1,0]", 0,		 	matrix.getValue(1, 0), 0.001);
		assertEquals("[1,1]", 1,		 	matrix.getValue(1, 1), 0.001);
		assertEquals("[1,2]", 0,		 	matrix.getValue(1, 2), 0.001);
		assertEquals("[1,3]", 0,		 	matrix.getValue(1, 3), 0.001);
		assertEquals("[2,0]", 0,		 	matrix.getValue(2, 0), 0.001);
		assertEquals("[2,1]", 0,		 	matrix.getValue(2, 1), 0.001);
		assertEquals("[2,2]", 1,		 	matrix.getValue(2, 2), 0.001);
		assertEquals("[2,3]", 0,		 	matrix.getValue(2, 3), 0.001);
		assertEquals("[3,0]", 0,		 	matrix.getValue(3, 0), 0.001);
		assertEquals("[3,1]", 0,		 	matrix.getValue(3, 1), 0.001);
		assertEquals("[3,2]", 0,		 	matrix.getValue(3, 2), 0.001);
		assertEquals("[3,3]", 1,		 	matrix.getValue(3, 3), 0.001);
	}
		
	@org.junit.Test
	public void testTransform4() {
		QQmlListProperty<QQuickTransform> transformList = object.transform();
		component.setData(
                "import QtQuick 2.5\n" +
                "Rotation { origin.x: 0; origin.y: 0; angle: 180}",
                new QUrl());
		QObject transformObject = component.create();
		assertTrue("object instanceof QQuickTransform", transformObject instanceof QQuickTransform);
		transformList.append((QQuickTransform)transformObject);
		assertEquals("count", 3, transformList.count());
		QQuickTransform transform = transformList.at(2);
		QMatrix4x4 matrix = new QMatrix4x4();
		transform.applyTo(matrix);
		assertEquals("[0,0]", -1,			matrix.getValue(0, 0), 0.001);
		assertEquals("[0,1]", 0,			matrix.getValue(0, 1), 0.001);
		assertEquals("[0,2]", 0, 			matrix.getValue(0, 2), 0.001);
		assertEquals("[0,3]", 0,			matrix.getValue(0, 3), 0.001);
		assertEquals("[1,0]", 0,		 	matrix.getValue(1, 0), 0.001);
		assertEquals("[1,1]", -1,		 	matrix.getValue(1, 1), 0.001);
		assertEquals("[1,2]", 0,		 	matrix.getValue(1, 2), 0.001);
		assertEquals("[1,3]", 0,		 	matrix.getValue(1, 3), 0.001);
		assertEquals("[2,0]", 0,		 	matrix.getValue(2, 0), 0.001);
		assertEquals("[2,1]", 0,		 	matrix.getValue(2, 1), 0.001);
		assertEquals("[2,2]", 1,		 	matrix.getValue(2, 2), 0.001);
		assertEquals("[2,3]", 0,		 	matrix.getValue(2, 3), 0.001);
		assertEquals("[3,0]", 0,		 	matrix.getValue(3, 0), 0.001);
		assertEquals("[3,1]", 0,		 	matrix.getValue(3, 1), 0.001);
		assertEquals("[3,2]", 0,		 	matrix.getValue(3, 2), 0.001);
		assertEquals("[3,3]", 1,		 	matrix.getValue(3, 3), 0.001);
	}
		
	@org.junit.Test
	public void testTransform5() {
		QQmlListProperty<QQuickTransform> transformList = object.transform();
		transformList.clear();
		assertEquals("count", 0, transformList.count());
	}
}
