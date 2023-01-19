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
import io.qt.*;
import io.qt.qml.*;
import io.qt.quick.*;

public class TestQuickQt6 extends ApplicationInitializer {
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitializeWithGui();
		Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
		io.qt.QtUtilities.loadQtLibrary("OpenGL");
	}
	
	static boolean updatePaintNode_begone;
	static boolean updatePaintNode_item_disposed;
	static boolean painted;
	static boolean updatePaintNode_ended;
	static QEventLoop loop;
	
	public static class SGMaterial extends QSGMaterial{
		QSGMaterialType type = new QSGMaterialType();
		
		@Override
		public QSGMaterialShader createShader(QSGRendererInterface.RenderMode r) {
			return new QSGMaterialShader();
		}

		@Override
		public QSGMaterialType type() {
			return type;
		}
	}
	
	public static class TestItem extends QQuickItem{
		public TestItem(QQuickItem parent) {
			super(parent);
			setFlag(QQuickItem.Flag.ItemHasContents, true);
		}

		@NativeAccess
		private TestItem(QDeclarativeConstructor constructor) {
			super(constructor);
			setFlag(QQuickItem.Flag.ItemHasContents, true);
		}

		public TestItem() {
			super();
			setFlag(QQuickItem.Flag.ItemHasContents, true);
		}
		
		@Override
		protected QSGNode updatePaintNode(QSGNode arg__1, UpdatePaintNodeData arg__2) {
			updatePaintNode_begone = true;
			QSGNode testNode = new QSGNode();
			super.updatePaintNode(testNode, arg__2);
			updatePaintNode_item_disposed = testNode.isDisposed();
			updatePaintNode_ended = true;
			QSGGeometryNode geometryNode = new QSGGeometryNode();
			geometryNode.setMaterial(new SGMaterial());
			geometryNode.setGeometry(new QSGGeometry(QSGGeometry.defaultAttributes_Point2D(), 5));
			if(loop!=null)
				QMetaObject.invokeMethod(loop::quit, Qt.ConnectionType.QueuedConnection);
			return geometryNode;
		}
	}
	
	@Test
    public void testQuickView()
    {
		QQuickWindow.setGraphicsApi(QSGRendererInterface.GraphicsApi.OpenGL);
		loop = new QEventLoop();
		try {
			updatePaintNode_begone = false;
			updatePaintNode_ended = false;
			updatePaintNode_item_disposed = false;
			QtQml.qmlClearTypeRegistrations();
			QtQml.qmlRegisterType(TestItem.class, "io.qt.test", 1, 0, "TestItem");
		    QTimer timer = new QTimer();
		    timer.timeout.connect(loop::quit);
			QQuickView component = new QQuickView();
			component.setObjectName("testQuickView");
		    QSurfaceFormat format = component.format();
		    format.setSamples(8);
		    component.setFormat(format);
			component.setSource(QUrl.fromLocalFile(":io/qt/autotests/qml/TestItem.qml"));
			String error = "";
			for(QQmlError err : component.errors()) {
				error += err.description() + "\n";
			}
			Assert.assertEquals(error, QQuickView.Status.Ready, component.status());
			Assert.assertTrue(component.rootObject() instanceof TestItem);
		    component.show();
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
		}finally {
			loop = null;
		}
    }
	
	@Test
    public void testQuickWindow()
    {
		QQuickWindow.setGraphicsApi(QSGRendererInterface.GraphicsApi.Software);
		loop = new QEventLoop();
		try {
			updatePaintNode_begone = false;
			updatePaintNode_ended = false;
			updatePaintNode_item_disposed = false;
			QtQml.qmlClearTypeRegistrations();
		    QTimer timer = new QTimer();
		    timer.setInterval(5000);
		    timer.setSingleShot(true);
		    timer.timeout.connect(loop::quit);
		    QQuickWindow component = new QQuickWindow();
			component.setObjectName("testQuickWindow");
		    QSurfaceFormat format = component.format();
		    format.setSamples(8);
		    component.setFormat(format);
		    TestItem test = new TestItem();
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
		    Assert.assertTrue("updatePaintNode item not disposed", updatePaintNode_item_disposed);
		}finally{
			loop = null;
		}
    }
}
