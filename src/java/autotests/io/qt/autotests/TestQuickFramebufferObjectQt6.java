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
import io.qt.*;
import io.qt.core.*;
import io.qt.gui.*;
import io.qt.qml.*;
import io.qt.quick.*;
import static io.qt.autotests.TestQuickQt6.*;

public class TestQuickFramebufferObjectQt6 extends ApplicationInitializer {
	
	private static boolean contextIsOpenGL;
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitializeWithGui();
		Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
		io.qt.QtUtilities.loadQtLibrary("OpenGL");
	}
	
	public static class TestFramebufferObject extends QQuickFramebufferObject{
		public TestFramebufferObject(QQuickItem parent) {
			super(parent);
			setFlag(QQuickItem.Flag.ItemHasContents, true);
		}

		@NativeAccess
		private TestFramebufferObject(QDeclarativeConstructor constructor) {
			super(constructor);
			setFlag(QQuickItem.Flag.ItemHasContents, true);
		}

		public TestFramebufferObject() {
			super();
			setFlag(QQuickItem.Flag.ItemHasContents, true);
		}

		@Override
		protected QSGNode updatePaintNode(QSGNode arg__1, UpdatePaintNodeData arg__2) {
			updatePaintNode_begone = true;
			QSGRendererInterface rendererInterface = this.window().rendererInterface();
			contextIsOpenGL = rendererInterface!=null && rendererInterface.graphicsApi()==QSGRendererInterface.GraphicsApi.OpenGL;
			QSGNode result = super.updatePaintNode(null, arg__2);
			updatePaintNode_ended = true;
			return result;
		}

		@Override
		public Renderer createRenderer() {
			painted = true;
			if(loop!=null)
				QMetaObject.invokeMethod(loop::quit, Qt.ConnectionType.QueuedConnection);
			return new Renderer() {
				@Override
				protected void render() {
				}
			};
		}
	}
	
	@Test
    public void testQuickWindow_FramebufferObject()
    {
		QQuickWindow.setGraphicsApi(QSGRendererInterface.GraphicsApi.OpenGL);
		loop = new QEventLoop();
		try {
			updatePaintNode_begone = false;
			updatePaintNode_ended = false;
			painted = false;
			QtQml.qmlClearTypeRegistrations();
		    QTimer timer = new QTimer();
		    timer.setInterval(10000);
		    timer.setSingleShot(true);
		    timer.timeout.connect(loop::quit);
			QQuickWindow component = new QQuickWindow();
			String[] message = {null};
			QQuickWindow.SceneGraphError[] error = {null};
			component.sceneGraphError.connect((err,msg)->{error[0]=err; message[0]=msg;});
			Assume.assumeTrue(component.rendererInterface().graphicsApi()==QSGRendererInterface.GraphicsApi.OpenGL);
			component.setObjectName("testQuickWindow_FramebufferObject");
		    QSurfaceFormat format = component.format();
		    format.setSamples(16);
		    component.setFormat(format);
		    TestFramebufferObject test = new TestFramebufferObject();
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
		    Assume.assumeTrue(""+message[0], error[0]==null);
		    Assert.assertTrue("updatePaintNode not begone", updatePaintNode_begone);
		    Assert.assertTrue("updatePaintNode not ended", updatePaintNode_ended);
		    if(contextIsOpenGL)
		    	Assert.assertTrue("no renderer created", painted);
		}finally{
			loop = null;
		}
    }
}
