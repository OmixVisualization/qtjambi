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

import org.junit.*;
import io.qt.core.*;
import io.qt.gui.*;
import io.qt.qml.*;
import io.qt.quick.*;
import io.qt.quick.widgets.*;
import io.qt.widgets.*;
import io.qt.autotests.TestQuickQt5.*;
import static io.qt.autotests.TestQuickQt5.*;

public class TestQuickWidgetsQt5 extends ApplicationInitializer {
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitializeWithWidgets();
		Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
	}

	
	@Test
    public void testQuickWidget()
    {
		loop = new QEventLoop();
		try {
			updatePaintNode_begone = false;
			updatePaintNode_ended = false;
			updatePaintNode_item_disposed = false;
			QtQml.qmlClearTypeRegistrations();
			QtQml.qmlRegisterType(TestItem.class, "io.qt.test", 1, 0, "TestItem");
		    QTimer timer = new QTimer();
		    timer.timeout.connect(loop::quit);
		    QMainWindow mainWindow = new QMainWindow();
		    mainWindow.setWindowTitle("testQuickWidget");
			QQuickWidget component = new QQuickWidget(mainWindow);
			component.setObjectName("testQuickWidget");
			mainWindow.setCentralWidget(component);
		    QSurfaceFormat format = component.format();
		    format.setSamples(16);
		    component.setFormat(format);
			component.setSource(QUrl.fromLocalFile(":io/qt/autotests/qml/TestItem.qml"));
			String error = "";
			for(QQmlError err : component.errors()) {
				error += err.description() + "\n";
			}
			Assert.assertEquals(error, QQuickWidget.Status.Ready, component.status());
			Assert.assertTrue(component.rootObject() instanceof TestItem);
			mainWindow.show();
			timer.start(5000);
		    loop.exec();
		    timer.timeout.disconnect();
		    component.close();
		    timer.stop();
		    timer.dispose();
		    component.dispose();
		    Assert.assertTrue("updatePaintNode not begone", updatePaintNode_begone);
		    Assert.assertTrue("updatePaintNode not ended", updatePaintNode_ended);
		    Assert.assertTrue("updatePaintNode item not disposed", updatePaintNode_item_disposed);
			QtQml.qmlClearTypeRegistrations();
		}finally{
			loop = null;
		}
    }
}
