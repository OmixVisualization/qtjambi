/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

import java.lang.ref.WeakReference;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QDir;
import io.qt.core.QEventLoop;
import io.qt.core.QSize;
import io.qt.core.QSizeF;
import io.qt.core.QTimer;
import io.qt.core.QUrl;
import io.qt.core.Qt;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QPainter;
import io.qt.gui.QSurfaceFormat;
import io.qt.internal.NativeAccess;
import io.qt.qml.QQmlError;
import io.qt.qml.QtQml;
import io.qt.quick.QQuickFramebufferObject;
import io.qt.quick.QQuickItem;
import io.qt.quick.QQuickPaintedItem;
import io.qt.quick.QQuickView;
import io.qt.quick.QQuickWindow;
import io.qt.quick.QSGGeometry;
import io.qt.quick.QSGGeometryNode;
import io.qt.quick.QSGMaterial;
import io.qt.quick.QSGMaterialShader;
import io.qt.quick.QSGMaterialType;
import io.qt.quick.QSGNode;
import io.qt.quick.widgets.QQuickWidget;

public class TestQuick extends QApplicationTest {
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		QApplicationTest.testInitialize();
		Assume.assumeThat(QGuiApplication.primaryScreen()!=null, QApplicationTest.trueMatcher("A screen is required to create a window."));
	}
	
	private static boolean updatePaintNode_begone;
	private static boolean updatePaintNode_item_disposed;
	private static boolean painted;
	private static boolean updatePaintNode_ended;
	private static QEventLoop loop;
	
	public static class SGMaterialShader extends QSGMaterialShader{
		SGMaterialShader(){
			// since QtQuick has a memory leak in qsgbatchrenderer.cpp
			// SGMaterialShader is never deleted by Qt.
			instances.add(new WeakReference<>(this));
		}
		
		@Override
		public String[] attributeNames() {
			return new String[0];
		}
	}
	
	public static class SGMaterial extends QSGMaterial{
		QSGMaterialType type = new QSGMaterialType();
		
		@Override
		public QSGMaterialShader createShader() {
			return new SGMaterialShader();
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
				loop.quit();
			return geometryNode;
		}
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
				loop.quit();
			return result;
		}

		@Override
		public void paint(QPainter painter) {
			painted = true;
		}
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
			QSGNode result = super.updatePaintNode(arg__1, arg__2);
			updatePaintNode_ended = true;
			if(loop!=null)
				loop.quit();
			return result;
		}

		@Override
		public Renderer createRenderer() {
			painted = true;
			return new Renderer() {
				@Override
				protected void render() {
				}
			};
		}
	}
	
	@Test
    public void testQuickView()
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
			QQuickView component = new QQuickView();
			component.setObjectName("testQuickView");
			String prefix = io.qt.QtUtilities.qtPrefix();
	        if(new QDir(prefix+"/qml").exists()) {
	        	component.engine().addImportPath(prefix+"/qml");
	        }
		    QSurfaceFormat format = component.format();
		    format.setSamples(16);
		    component.setFormat(format);
			component.setSource(QUrl.fromLocalFile("classpath:io/qt/autotests/qml/TestItem.qml"));
			String error = "";
			for(QQmlError err : component.errors()) {
				error += err.description() + "\n";
			}
			Assert.assertEquals(error, QQuickView.Status.Ready, component.status());
			Assert.assertTrue(component.rootObject() instanceof TestItem);
		    component.show();
		    timer.start(15000);
		    loop.exec();
		    timer.timeout.disconnect();
		    component.close();
		    Assert.assertTrue("updatePaintNode not begone", updatePaintNode_begone);
		    Assert.assertTrue("updatePaintNode not ended", updatePaintNode_ended);
		    Assert.assertTrue("updatePaintNode item not disposed", updatePaintNode_item_disposed);
			QtQml.qmlClearTypeRegistrations();
		}finally {
			loop = null;
		}
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
			QQuickWidget component = new QQuickWidget();
			component.setObjectName("testQuickWidget");
			String prefix = io.qt.QtUtilities.qtPrefix();
	        if(new QDir(prefix+"/qml").exists()) {
	        	component.engine().addImportPath(prefix+"/qml");
	        }
		    QSurfaceFormat format = component.format();
		    format.setSamples(16);
		    component.setFormat(format);
			component.setSource(QUrl.fromLocalFile("classpath:io/qt/autotests/qml/TestItem.qml"));
			String error = "";
			for(QQmlError err : component.errors()) {
				error += err.description() + "\n";
			}
			Assert.assertEquals(error, QQuickWidget.Status.Ready, component.status());
			Assert.assertTrue(component.rootObject() instanceof TestItem);
		    component.show();
			timer.start(15000);
		    loop.exec();
		    timer.timeout.disconnect();
		    component.close();
		    Assert.assertTrue("updatePaintNode not begone", updatePaintNode_begone);
		    Assert.assertTrue("updatePaintNode not ended", updatePaintNode_ended);
		    Assert.assertTrue("updatePaintNode item not disposed", updatePaintNode_item_disposed);
			QtQml.qmlClearTypeRegistrations();
		}finally{
			loop = null;
		}
    }
	
	@Test
    public void testQuickWindow()
    {
		loop = new QEventLoop();
		try {
			updatePaintNode_begone = false;
			updatePaintNode_ended = false;
			updatePaintNode_item_disposed = false;
			QtQml.qmlClearTypeRegistrations();
			QQuickWindow component = new QQuickWindow();
			component.setObjectName("testQuickWindow");
		    QSurfaceFormat format = component.format();
		    format.setSamples(16);
		    component.setFormat(format);
		    TestItem test = new TestItem();
		    test.setParentItem(component.contentItem());
		    component.contentItem().setEnabled(true);
		    component.sceneGraphInitialized.connect(()->component.setRenderTarget(0, new QSize(200, 200)), Qt.ConnectionType.DirectConnection);
		    component.show();
		    QTimer timer = new QTimer();
		    timer.timeout.connect(loop::quit);
		    timer.start(5000);
		    loop.exec();
		    timer.timeout.disconnect();
		    component.close();
		    Assert.assertTrue("updatePaintNode not begone", updatePaintNode_begone);
		    Assert.assertTrue("updatePaintNode not ended", updatePaintNode_ended);
		    Assert.assertTrue("updatePaintNode item not disposed", updatePaintNode_item_disposed);
		}finally{
			loop = null;
		}
    }
	
	@Test
    public void testQuickWindow_Painted()
    {
		loop = new QEventLoop();
		try {
			updatePaintNode_begone = false;
			updatePaintNode_ended = false;
			painted = false;
			QtQml.qmlClearTypeRegistrations();
		    QTimer timer = new QTimer();
		    timer.timeout.connect(loop::quit);
			QQuickWindow component = new QQuickWindow();
			component.setObjectName("testQuickWindow_Painted");
		    QSurfaceFormat format = component.format();
		    format.setSamples(16);
		    component.setFormat(format);
		    TestPaintedItem test = new TestPaintedItem();
		    test.setSize(new QSizeF(200, 200));
		    test.setParentItem(component.contentItem());
		    component.contentItem().setEnabled(true);
		    component.sceneGraphInitialized.connect(()->component.setRenderTarget(0, new QSize(200, 200)), Qt.ConnectionType.DirectConnection);
		    component.show();
		    timer.start(5000);
		    loop.exec();
		    timer.timeout.disconnect();
		    component.close();
		    Assert.assertTrue("updatePaintNode not begone", updatePaintNode_begone);
		    Assert.assertTrue("updatePaintNode not ended", updatePaintNode_ended);
		    Assert.assertTrue("not createRenderer", painted);
		}finally{
			loop = null;
		}
    }
	
	@Test
    public void testQuickWindow_FramebufferObject()
    {
		loop = new QEventLoop();
		try {
			updatePaintNode_begone = false;
			updatePaintNode_ended = false;
			painted = false;
			QtQml.qmlClearTypeRegistrations();
		    QTimer timer = new QTimer();
		    timer.timeout.connect(loop::quit);
			QQuickWindow component = new QQuickWindow();
			component.setObjectName("testQuickWindow_FramebufferObject");
		    QSurfaceFormat format = component.format();
		    format.setSamples(16);
		    component.setFormat(format);
		    TestFramebufferObject test = new TestFramebufferObject();
		    test.setSize(new QSizeF(200, 200));
		    test.setParentItem(component.contentItem());
		    component.contentItem().setEnabled(true);
		    component.sceneGraphInitialized.connect(()->component.setRenderTarget(0, new QSize(200, 200)), Qt.ConnectionType.DirectConnection);
		    component.sceneGraphInitialized.connect(()->timer.start(15000));
		    component.show();
		    loop.exec();
		    timer.timeout.disconnect();
		    component.close();
		    Assert.assertTrue("updatePaintNode not begone", updatePaintNode_begone);
		    Assert.assertTrue("updatePaintNode not ended", updatePaintNode_ended);
		    Assert.assertTrue("not painted", painted);
		}finally{
			loop = null;
		}
    }
}
