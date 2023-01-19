/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

import org.junit.Assert;
import org.junit.Test;

import io.qt.QtObjectInterface;
import io.qt.quick.QSGGeometry;

public class TestQuickInjectedCode extends ApplicationInitializer {
    @Test
    public void testQSGGeometry_vertexData_Point2D()
    {
    	QSGGeometry geometry = new QSGGeometry(QSGGeometry.defaultAttributes_Point2D(), 4);
    	QSGGeometry.VertexData<?> vertexData = geometry.vertexData();
    	Assert.assertTrue(vertexData instanceof QSGGeometry.Point2DVertexData);
    	QtObjectInterface p1 = vertexData.get(0);
    	Assert.assertTrue(p1 instanceof QSGGeometry.Point2D);
    }
    
    @Test
    public void testQSGGeometry_vertexData_ColoredPoint2D()
    {
    	QSGGeometry geometry = new QSGGeometry(QSGGeometry.defaultAttributes_ColoredPoint2D(), 4);
    	QSGGeometry.VertexData<?> vertexData = geometry.vertexData();
    	Assert.assertTrue(vertexData instanceof QSGGeometry.ColoredPoint2DVertexData);
    	QtObjectInterface p1 = vertexData.get(0);
    	Assert.assertTrue(p1 instanceof QSGGeometry.ColoredPoint2D);
    }
    
    @Test
    public void testQSGGeometry_vertexData_TexturedPoint2D()
    {
    	QSGGeometry geometry = new QSGGeometry(QSGGeometry.defaultAttributes_TexturedPoint2D(), 4);
    	QSGGeometry.VertexData<?> vertexData = geometry.vertexData();
    	Assert.assertTrue(vertexData instanceof QSGGeometry.TexturedPoint2DVertexData);
    	QtObjectInterface p1 = vertexData.get(0);
    	Assert.assertTrue(p1 instanceof QSGGeometry.TexturedPoint2D);
    }
}
