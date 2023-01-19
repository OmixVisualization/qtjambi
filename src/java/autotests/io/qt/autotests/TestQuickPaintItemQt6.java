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

import static io.qt.autotests.TestQuickQt6.loop;
import static io.qt.autotests.TestQuickQt6.painted;
import static io.qt.autotests.TestQuickQt6.updatePaintNode_begone;
import static io.qt.autotests.TestQuickQt6.updatePaintNode_ended;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.NativeAccess;
import io.qt.core.QEventLoop;
import io.qt.core.QMetaObject;
import io.qt.core.QSize;
import io.qt.core.QSizeF;
import io.qt.core.QTimer;
import io.qt.core.Qt;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QPainter;
import io.qt.gui.QSurfaceFormat;
import io.qt.qml.QtQml;
import io.qt.quick.QQuickItem;
import io.qt.quick.QQuickPaintedItem;
import io.qt.quick.QQuickRenderTarget;
import io.qt.quick.QQuickWindow;
import io.qt.quick.QSGNode;
import io.qt.quick.QSGRendererInterface;

public class TestQuickPaintItemQt6 extends ApplicationInitializer {
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitializeWithGui();
		Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
		io.qt.QtUtilities.loadQtLibrary("OpenGL");
	}
	
	public static class TestPaintedItem extends QQuickPaintedItem{
		public TestPaintedItem(QQuickItem parent) {
			super(parent);
			setFlag(QQuickItem.Flag.ItemHasContents, true);
		}

		@NativeAccess
		private TestPaintedItem(QDeclarativeConstructor constructor) {
			super(constructor);
			setFlag(QQuickItem.Flag.ItemHasContents, true);
		}

		public TestPaintedItem() {
			super();
			setFlag(QQuickItem.Flag.ItemHasContents, true);
		}

		@Override
		protected QSGNode updatePaintNode(QSGNode arg__1, UpdatePaintNodeData arg__2) {
			updatePaintNode_begone = true;
			QSGNode result = super.updatePaintNode(arg__1, arg__2);
			updatePaintNode_ended = true;
			if(loop!=null)
				QMetaObject.invokeMethod(loop::quit, Qt.ConnectionType.QueuedConnection);
			return result;
		}

		@Override
		public void paint(QPainter painter) {
			painted = true;
		}
	}
	
	@Test
    public void testQuickWindow_Painted()
    {
		QQuickWindow.setGraphicsApi(QSGRendererInterface.GraphicsApi.Software);
		loop = new QEventLoop();
		try {
			updatePaintNode_begone = false;
			updatePaintNode_ended = false;
			painted = false;
			QtQml.qmlClearTypeRegistrations();
		    QTimer timer = new QTimer();
		    timer.setInterval(5000);
		    timer.setSingleShot(true);
		    timer.timeout.connect(loop::quit);
			QQuickWindow component = new QQuickWindow();
			component.setObjectName("testQuickWindow_Painted");
		    QSurfaceFormat format = component.format();
		    format.setSamples(8);
		    component.setFormat(format);
		    TestPaintedItem test = new TestPaintedItem();
		    test.setSize(new QSizeF(200, 200));
		    test.setParentItem(component.contentItem());
		    component.contentItem().setEnabled(true);
		    component.sceneGraphInitialized.connect(()->component.setRenderTarget(QQuickRenderTarget.fromOpenGLTexture(0, new QSize(200, 200))), Qt.ConnectionType.DirectConnection);
		    component.sceneGraphInitialized.connect(timer::start);
		    component.show();
		    QTimer.singleShot(20000, loop::quit);
		    loop.exec();
		    timer.timeout.disconnect();
		    component.close();
		    timer.stop();
		    timer.dispose();
		    component.dispose();
		    Assert.assertTrue("updatePaintNode not begone", updatePaintNode_begone);
		    Assert.assertTrue("updatePaintNode not ended", updatePaintNode_ended);
		    Assert.assertTrue("not createRenderer", painted);
		}finally{
			loop = null;
		}
    }
}
