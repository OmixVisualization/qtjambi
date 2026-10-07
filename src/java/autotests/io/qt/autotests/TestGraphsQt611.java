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

import java.util.concurrent.atomic.AtomicReference;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QObject;
import io.qt.graphs.QAreaSeries;
import io.qt.qml.QQmlComponent;
import io.qt.qml.QQmlEngine;
import io.qt.quick.QQuickWindow;
import io.qt.quick.QSGRendererInterface;

public class TestGraphsQt611 extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
		QQuickWindow.setGraphicsApi(QSGRendererInterface.GraphicsApi.OpenGL);
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
  @Test
  public void testGradientSignal() {
	  AtomicReference<QObject> received = new AtomicReference<>();
	  QAreaSeries series = new QAreaSeries();
	  series.gradientChanged.connect(received, AtomicReference::set);
	  QQmlEngine engine = new QQmlEngine();
	  try {
		  QQmlComponent component = new QQmlComponent(engine);
		  component.setData("import QtQuick.Shapes 1.11\nLinearGradient{}", "qrc:");
		  QObject gradient = component.create();
		  Assert.assertTrue(gradient!=null);
		  Assert.assertTrue(series.setProperty("gradient", gradient));
		  Assert.assertEquals(gradient, received.get());
		  QObject other = new QObject();
		  Assert.assertFalse(series.setProperty("gradient", other));
		  Assert.assertTrue(series.setProperty("gradient", null));
		  Assert.assertEquals(null, received.get());
	  }finally {
		  engine.dispose();
	  }
  }
}
