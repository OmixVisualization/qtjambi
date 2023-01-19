/****************************************************************************
**
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

import java.util.List;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.Test;

import io.qt.QMissingVirtualOverridingException;
import io.qt.QtPrivateOverride;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QEventLoop;
import io.qt.core.QTimer;
import io.qt.gui.QColor;
import io.qt.gui.QFont;
import io.qt.gui.QFontDatabase;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QVector3D;
import io.qt.qt3d.core.QEntity;
import io.qt.qt3d.core.QTransform;
import io.qt.qt3d.extras.QAbstractCameraController;
import io.qt.qt3d.extras.QExtrudedTextMesh;
import io.qt.qt3d.extras.QOrbitCameraController;
import io.qt.qt3d.extras.QPhongMaterial;
import io.qt.qt3d.extras.QPlaneMesh;
import io.qt.qt3d.extras.Qt3DWindow;
import io.qt.qt3d.render.QCamera;

public class TestQt3DCore extends ApplicationInitializer {
	/**
	 * Crashes in Qt 5.15 because of a dangled pointer in GLShader::setGraphicsContext
	 */
//    @Test
    public void test_3D_text() {
    	Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
        Qt3DWindow view = new Qt3DWindow();
        view.defaultFrameGraph().setClearColor(new QColor(210, 210, 220));

        QEntity root = new QEntity();

        { // plane
        	QEntity plane = new QEntity(root);
        	QPlaneMesh planeMesh = new QPlaneMesh();
        	QTransform planeTransform = new QTransform();
        	QPhongMaterial planeMaterial = new QPhongMaterial(root);
            planeMesh.setWidth(10); planeMesh.setHeight(10);
            planeTransform.setTranslation(new QVector3D(0, 0, 0));
            planeMaterial.setDiffuse(new QColor(150, 150, 150));

            plane.addComponent(planeMaterial);
            plane.addComponent(planeMesh);
            plane.addComponent(planeTransform);
        }

        QPhongMaterial textMaterial = new QPhongMaterial(root);
        { // text
            int i = 0;
            List<String> fonts = new QFontDatabase().families();

            for (String family : fonts)
            {
            	QEntity text = new QEntity(root);
            	QExtrudedTextMesh textMesh = new QExtrudedTextMesh();

            	QTransform textTransform = new QTransform();
                QFont font = new QFont(family, 32, -1, false);
                textTransform.setTranslation(new QVector3D(-2.45f, i * .5f, 0));
                textTransform.setScale(.2f);
                textMesh.setDepth(.45f);
                textMesh.setFont(font);
                textMesh.setText(family);
                textMaterial.setDiffuse(new QColor(111, 150, 255));

                text.addComponent(textMaterial);
                text.addComponent(textMesh);
                text.addComponent(textTransform);

                i++;
                if(i>20)
                	break;
            }
        }

        { // camera
            float aspect = (view.screen().size().width()*1.f) / view.screen().size().height();
            QCamera camera = view.camera();
            camera.lens().setPerspectiveProjection(65.f, aspect, 0.1f, 100.f);
            camera.setPosition(new QVector3D(0, 5, 3));
            camera.setViewCenter(new QVector3D(0, 5, 0));

            QOrbitCameraController cameraController = new QOrbitCameraController(root);
            cameraController.setCamera(camera);
        }

        view.setRootEntity(root);
        view.show();
        QEventLoop loop = new QEventLoop();
        view.visibleChanged.connect(loop::quit);
        QTimer.singleShot(1000, loop::quit);
        loop.exec();
        view.close();
        loop.dispose();
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
        view.dispose();
        // make sure, aspects are deleted
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
    }
    
    static class IncompleteCameraController extends QAbstractCameraController{
    }
    
    static class HalfCameraController extends QAbstractCameraController{
    	@SuppressWarnings("unused")
		private void moveCamera(InputState state, float dt){}
    }
    
    static class CameraController extends QAbstractCameraController{
    	@QtPrivateOverride
    	private void moveCamera(InputState state, float dt){}
    }
    
    @Test
    public void testCameraController_incomplete() {
    	try {
    		new IncompleteCameraController();
			Assert.assertFalse("Expected to throw QMissingVirtualOverridingException", true); 
		} catch (QMissingVirtualOverridingException e) {
			Assert.assertEquals("Class io.qt.autotests.TestQt3D.IncompleteCameraController does not implement private pure virtual function 'void moveCamera(io.qt.qt3d.extras.QAbstractCameraController.InputState, float)' from Qt declared in Qt3DExtras::QAbstractCameraController.", e.getMessage());
		}
    }
    
    @Test
    public void testCameraController_no_private_overload() {
    	try {
			new HalfCameraController();
			Assert.assertFalse("Expected to throw QMissingVirtualOverridingException", true); 
		} catch (QMissingVirtualOverridingException e) {
			Assert.assertEquals("Method 'void moveCamera(io.qt.qt3d.extras.QAbstractCameraController.InputState, float)' in class io.qt.autotests.TestQt3D.HalfCameraController misses @QtPrivateOverride annotation.", e.getMessage());
		}
    }
    
    @Test
    public void testCameraController() {
		new CameraController();
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQt3DCore.class.getName());
    }
}
