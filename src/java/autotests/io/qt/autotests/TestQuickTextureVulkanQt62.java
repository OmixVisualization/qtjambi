/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.autotests.generated.General;
import io.qt.core.QEventLoop;
import io.qt.core.QSize;
import io.qt.core.QTimer;
import io.qt.core.Qt;
import io.qt.gui.QColor;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QPixmap;
import io.qt.gui.QSurface;
import io.qt.quick.QQuickRenderTarget;
import io.qt.quick.QQuickWindow;
import io.qt.quick.QSGNode;
import io.qt.quick.QSGRendererInterface;
import io.qt.quick.QSGTexture;
import io.qt.quick.nativeinterface.QSGVulkanTexture;

public class TestQuickTextureVulkanQt62 extends ApplicationInitializer {
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		Assume.assumeTrue("Qt build has no Vulkan support.", General.canVulkan() && General.canCreateVulkanInstance());
		ApplicationInitializer.testInitializeWithGui();
		Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
		io.qt.QtUtilities.loadQtLibrary("OpenGL");
	}
	
	@Test
    public void testVulkanTecture() {
		QQuickWindow.setGraphicsApi(QSGRendererInterface.GraphicsApi.Vulkan);
		QQuickWindow window = new QQuickWindow();
		Assume.assumeTrue("Window needs Vulkan support.", window.surfaceType()==QSurface.SurfaceType.VulkanSurface);
		try {
			QEventLoop loop = new QEventLoop();
			boolean[] hasVulkanInstance = {false};
			QSGTexture[] texture = {null};
			TestQuickQt6.TestItem item = new TestQuickQt6.TestItem() {
				@Override
				protected QSGNode updatePaintNode(QSGNode arg__1, UpdatePaintNodeData arg__2) {
					QPixmap pixmap = new QPixmap(100, 100);
					pixmap.fill(new QColor(Qt.GlobalColor.magenta));
					if(hasVulkanInstance[0] = General.hasVulkanInstance(window))
						texture[0] = window.createTextureFromImage(pixmap.toImage());
					return arg__1;
				}
			};
			item.setParentItem(window.contentItem());
			window.contentItem().setEnabled(true);
			window.show();
			window.sceneGraphInitialized.connect(loop::quit);
		    QTimer.singleShot(20000, loop::quit);
		    loop.exec();
		    item.isDisposed();
		    Assume.assumeTrue("Window is missing QVulkanInstance.", hasVulkanInstance[0]);
			Assert.assertTrue(texture[0]!=null);
			QSGVulkanTexture openglTexture = texture[0].nativeInterface(QSGVulkanTexture.class);
			Assert.assertTrue(openglTexture!=null);
			QQuickRenderTarget target = QQuickRenderTarget.fromVulkanImage(openglTexture.nativeImage(), openglTexture.nativeImageLayout(), new QSize(100, 100));
			Assert.assertTrue(target!=null);
		}finally {
		    window.close();
			window.dispose();
		}
	}
}
