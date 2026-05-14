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

import io.qt.core.*;
import io.qt.gui.*;
import io.qt.qml.*;

public class TestDivideByZerro extends ApplicationInitializer{
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivQSize() {
		QSize value = new QSize(1,1);
		value.div(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivQSizeF() {
		QSizeF value = new QSizeF(1,1);
		value.div(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivQPoint() {
		QPoint value = new QPoint(1,1);
		value.div(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivQPointF() {
		QPointF value = new QPointF(1,1);
		value.div(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivQMargins() {
		QMargins value = new QMargins(1,1,1,1);
		value.div(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivQMarginsF() {
		QMarginsF value = new QMarginsF(1,1,1,1);
		value.div(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivQVector4D() {
		QVector4D value = new QVector4D(1,1,1,1);
		value.div(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivQVector3D() {
		QVector3D value = new QVector3D(1,1,1);
		value.div(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivQVector2D() {
		QVector2D value = new QVector2D(1,1);
		value.div(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDiv2QVector4D() {
		QVector4D value = new QVector4D(1,1,1,1);
		value.div(null);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDiv2QVector3D() {
		QVector3D value = new QVector3D(1,1,1);
		value.div(null);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDiv2QVector2D() {
		QVector2D value = new QVector2D(1,1);
		value.div(null);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivQTransform() {
		QTransform value = new QTransform(1,1,1,1,1,1);
		value.div(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivQQuaternion() {
		QQuaternion value = new QQuaternion(1,1,1,1);
		value.div(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivQJSPrimitiveValue() {
		QJSPrimitiveValue value = new QJSPrimitiveValue(1);
		value.div(new QJSPrimitiveValue(0));
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivQMatrix4x4() {
		QMatrix4x4 value = new QMatrix4x4(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1);
		value.div(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivQMatrix4x3() {
		QMatrix4x3 value = new QMatrix4x3(1,1,1,1,1,1,1,1,1,1,1,1);
		value.div(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivQMatrix4x2() {
		QMatrix4x2 value = new QMatrix4x2(1,1,1,1,1,1,1,1);
		value.div(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivQMatrix3x2() {
		QMatrix3x2 value = new QMatrix3x2(1,1,1,1,1,1);
		value.div(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivQMatrix3x3() {
		QMatrix3x3 value = new QMatrix3x3(1,1,1,1,1,1,1,1,1);
		value.div(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivQMatrix3x4() {
		QMatrix3x4 value = new QMatrix3x4(1,1,1,1,1,1,1,1,1,1,1,1);
		value.div(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivQMatrix2x2() {
		QMatrix2x2 value = new QMatrix2x2(1,1,1,1);
		value.div(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivQMatrix2x3() {
		QMatrix2x3 value = new QMatrix2x3(1,1,1,1,1,1);
		value.div(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivQMatrix2x4() {
		QMatrix2x4 value = new QMatrix2x4(1,1,1,1,1,1,1,1);
		value.div(0);
	}
	
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivideQSize() {
		QSize value = new QSize(1,1);
		value.divide(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivideQSizeF() {
		QSizeF value = new QSizeF(1,1);
		value.divide(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivideQPoint() {
		QPoint value = new QPoint(1,1);
		value.divide(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivideQPointF() {
		QPointF value = new QPointF(1,1);
		value.divide(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivideQMargins() {
		QMargins value = new QMargins(1,1,1,1);
		value.divide(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivideQMarginsF() {
		QMarginsF value = new QMarginsF(1,1,1,1);
		value.divide(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivideQVector4D() {
		QVector4D value = new QVector4D(1,1,1,1);
		value.divide(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivideQVector3D() {
		QVector3D value = new QVector3D(1,1,1);
		value.divide(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivideQVector2D() {
		QVector2D value = new QVector2D(1,1);
		value.divide(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivide2QVector4D() {
		QVector4D value = new QVector4D(1,1,1,1);
		value.divide(null);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivide2QVector3D() {
		QVector3D value = new QVector3D(1,1,1);
		value.divide(null);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivide2QVector2D() {
		QVector2D value = new QVector2D(1,1);
		value.divide(null);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivideQTransform() {
		QTransform value = new QTransform(1,1,1,1,1,1);
		value.divide(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivideQQuaternion() {
		QQuaternion value = new QQuaternion(1,1,1,1);
		value.divide(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivideQMatrix4x4() {
		QMatrix4x4 value = new QMatrix4x4(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1);
		value.divide(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivideQMatrix4x3() {
		QMatrix4x3 value = new QMatrix4x3(1,1,1,1,1,1,1,1,1,1,1,1);
		value.divide(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivideQMatrix4x2() {
		QMatrix4x2 value = new QMatrix4x2(1,1,1,1,1,1,1,1);
		value.divide(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivideQMatrix3x2() {
		QMatrix3x2 value = new QMatrix3x2(1,1,1,1,1,1);
		value.divide(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivideQMatrix3x3() {
		QMatrix3x3 value = new QMatrix3x3(1,1,1,1,1,1,1,1,1);
		value.divide(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivideQMatrix3x4() {
		QMatrix3x4 value = new QMatrix3x4(1,1,1,1,1,1,1,1,1,1,1,1);
		value.divide(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivideQMatrix2x2() {
		QMatrix2x2 value = new QMatrix2x2(1,1,1,1);
		value.divide(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivideQMatrix2x3() {
		QMatrix2x3 value = new QMatrix2x3(1,1,1,1,1,1);
		value.divide(0);
	}
	@org.junit.Test(expected = ArithmeticException.class)
	public void testDivideQMatrix2x4() {
		QMatrix2x4 value = new QMatrix2x4(1,1,1,1,1,1,1,1);
		value.divide(0);
	}
}
