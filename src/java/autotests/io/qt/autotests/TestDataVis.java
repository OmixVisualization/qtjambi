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

import static org.junit.Assert.*;

import java.util.*;

import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.*;
import io.qt.autotests.generated.General;
import io.qt.core.*;
import io.qt.datavis.*;
import io.qt.gui.*;

public class TestDataVis extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
    @Test
    public void testQ3DBars() {
    	Q3DBars bars = new Q3DBars();
    	Assume.assumeTrue(bars.hasContext());
    	QCategory3DAxis axis1 = new QCategory3DAxis();
    	bars.setRowAxis(axis1);
    	QValue3DAxis axis2 = new QValue3DAxis();
    	bars.setValueAxis(axis2);
    	QObject parent = new QObject();
    	QCategory3DAxis axis3 = new QCategory3DAxis(parent);
    	bars.setColumnAxis(axis3);
    	assertTrue(axis1.parent()!=null);
    	assertTrue(axis2.parent()!=null);
    	assertTrue(axis3.parent()!=null);
    	assertEquals(axis2.parent(), axis3.parent());
    }
    
    @Test
    public void testQAbstract3DGraph(){
    	QObject parent = new QObject();
    	QAbstract3DGraph abstract3DGraph = new Q3DBars();
    	Assume.assumeTrue(abstract3DGraph.hasContext());
    	QCustom3DItem customItem = new QCustom3DItem();
    	abstract3DGraph.addCustomItem(customItem);
    	assertTrue(customItem.parent()!=null);
    	QAbstract3DInputHandler handler = new Q3DInputHandler();
    	abstract3DGraph.addInputHandler(handler);
    	assertTrue(handler.parent()!=null);
    	Q3DTheme theme = new Q3DTheme();
    	abstract3DGraph.addTheme(theme);
    	assertTrue(theme.parent()!=null);
    	Q3DTheme theme2 = new Q3DTheme(parent);
    	abstract3DGraph.setActiveTheme(theme2);
    	assertTrue(theme2.parent()!=null);
    	assertEquals(theme.parent(), theme2.parent());
    	abstract3DGraph.releaseTheme(theme2);
    	assertFalse(theme2.isDisposed());
    	assertEquals(null, theme2.parent());
    	QAbstract3DInputHandler handler2 = new Q3DInputHandler(parent);
    	abstract3DGraph.setActiveInputHandler(handler2);
    	assertTrue(handler2.parent()!=null);
    	assertEquals(handler.parent(), handler2.parent());
    	abstract3DGraph.releaseInputHandler(handler2);
    	assertFalse(handler2.isDisposed());
    	assertEquals(null, handler2.parent());
    	QCustom3DItem customItem2 = new QCustom3DItem();
    	abstract3DGraph.addCustomItem(customItem2);
    	assertTrue(customItem2.parent()!=null);
    	assertEquals(customItem.parent(), customItem2.parent());
    	abstract3DGraph.releaseCustomItem(customItem2);
    	assertFalse(customItem2.isDisposed());
    	assertEquals(null, customItem2.parent());
    	abstract3DGraph.removeCustomItem(customItem);
    	assertTrue(customItem.isDisposed());
    	abstract3DGraph.removeCustomItem(customItem2);
    	assertTrue(customItem2.isDisposed());
    }
    
    @Test
    public void testQ3DScatter() {
    	Q3DScatter scatter = new Q3DScatter();
    	Assume.assumeTrue(scatter.hasContext());
    	QValue3DAxis axis1 = new QValue3DAxis();
    	scatter.setAxisX(axis1);
    	QValue3DAxis axis2 = new QValue3DAxis();
    	scatter.setAxisY(axis2);
    	QObject parent = new QObject();
    	QValue3DAxis axis3 = new QValue3DAxis(parent);
    	scatter.setAxisZ(axis3);
    	assertTrue(axis1.parent()!=null);
    	assertTrue(axis2.parent()!=null);
    	assertTrue(axis3.parent()!=null);
    	assertEquals(axis2.parent(), axis3.parent());
    	QValue3DAxis axis = new QValue3DAxis(parent);
    	QScatter3DSeries series = new QScatter3DSeries(parent);
    	scatter.addSeries(series);
    	scatter.addAxis(axis);
    	assertTrue(axis.parent()!=null);
    	assertTrue(series.parent()!=null);
    	assertEquals(axis2.parent(), axis.parent());
    	assertEquals(axis2.parent(), series.parent());
    	scatter.releaseAxis(axis);
    	assertFalse(axis.isDisposed());
    	assertEquals(null, axis.parent());
    	scatter.removeSeries(series);
    	assertFalse(series.isDisposed());
    	assertEquals(null, series.parent());
    }
    
    @Test
    public void testQ3DSurface() {
    	Q3DSurface surface = new Q3DSurface();
    	Assume.assumeTrue(surface.hasContext());
    	QValue3DAxis axis1 = new QValue3DAxis();
    	surface.setAxisX(axis1);
    	QValue3DAxis axis2 = new QValue3DAxis();
    	surface.setAxisY(axis2);
    	QObject parent = new QObject();
    	QValue3DAxis axis3 = new QValue3DAxis(parent);
    	surface.setAxisZ(axis3);
    	assertTrue(axis1.parent()!=null);
    	assertTrue(axis2.parent()!=null);
    	assertTrue(axis3.parent()!=null);
    	assertEquals(axis2.parent(), axis3.parent());
    	QValue3DAxis axis = new QValue3DAxis(parent);
    	QSurface3DSeries series = new QSurface3DSeries(parent);
    	surface.addSeries(series);
    	surface.addAxis(axis);
    	assertTrue(axis.parent()!=null);
    	assertTrue(series.parent()!=null);
    	assertEquals(axis2.parent(), axis.parent());
    	assertEquals(axis2.parent(), series.parent());
    	surface.releaseAxis(axis);
    	assertFalse(axis.isDisposed());
    	assertEquals(null, axis.parent());
    	surface.removeSeries(series);
    	assertFalse(series.isDisposed());
    	assertEquals(null, series.parent());
    }
    
    @Test
    public void testQ3DScene() {
    	QObject parent = new QObject();
    	Q3DScene scene = new Q3DScene();
    	Q3DCamera camera = new Q3DCamera();
    	Q3DLight light = new Q3DLight(parent);
    	scene.setActiveCamera(camera);
    	scene.setActiveLight(light);
    	assertTrue(camera.parent()!=null);
    	assertTrue(light.parent()!=null);
    	assertEquals(scene, light.parent());
    	assertEquals(camera.parent(), light.parent());
    	Q3DCamera camera2 = new Q3DCamera();
    	scene.setActiveCamera(camera2);
    	assertEquals(camera2.parent(), light.parent());
    	assertEquals(camera.parent(), light.parent());
    }
    
    @Test
    public void testQSurface3DSeries() {
    	QSurface3DSeries series = new QSurface3DSeries();
    	QSurfaceDataProxy proxy = new QSurfaceDataProxy();
    	series.setDataProxy(proxy);
    	assertTrue(proxy.parent()!=null);
    	QSurfaceDataProxy proxy2 = new QSurfaceDataProxy();
    	series.setDataProxy(proxy2);
    	assertTrue(proxy.isDisposed());
    }
    
    @Test
    public void testQCustom3DVolume() {
    	QCustom3DVolume volume = new QCustom3DVolume();
    	QImage image1 = new QImage(24, 24, QImage.Format.Format_ARGB32);
    	QImage image2 = new QImage(24, 24, QImage.Format.Format_ARGB32);
    	QtObject textureData = volume.createTextureData(Arrays.asList(image1, image2));
    	assertTrue("not split ownership", General.internalAccess.isSplitOwnership(textureData));
    	assertFalse(textureData.isDisposed());
    	volume.dispose();
    	assertTrue(textureData.isDisposed());
    }
    
    
    @Test
    public void testQBarDataProxy(){
    	QBarDataProxy proxy = new QBarDataProxy();
    	QBarDataArray array = new QBarDataArray();
    	proxy.resetArray(array);
    	assertTrue("not cpp ownership", General.internalAccess.isCppOwnership(array));
    	assertFalse(array.isDisposed());
    	proxy.dispose();
    	try{
    		array.clear();
    		fail("QNoNativeResourcesException expected to be thrown.");
    	}catch(QNoNativeResourcesException e) {}
    	assertTrue(array.isDisposed());
    }
    
    @Test
    public void testQSurfaceDataProxy(){
    	QSurfaceDataProxy proxy = new QSurfaceDataProxy();
    	QSurfaceDataArray array = new QSurfaceDataArray();
    	proxy.resetArray(array);
    	assertTrue("not cpp ownership", General.internalAccess.isCppOwnership(array));
    	assertFalse(array.isDisposed());
    	proxy.dispose();
    	try{
    		array.clear();
    		fail("QNoNativeResourcesException expected to be thrown.");
    	}catch(QNoNativeResourcesException e) {}
    	assertTrue(array.isDisposed());
    }
    
    @Test
    public void testQScatterDataProxy(){
    	QScatterDataProxy proxy = new QScatterDataProxy();
    	QScatterDataArray array = new QScatterDataArray();
    	proxy.resetArray(array);
    	assertTrue("not cpp ownership", General.internalAccess.isCppOwnership(array));
    	assertFalse(array.isDisposed());
    	proxy.dispose();
    	try{
    		array.clear();
    		fail("QNoNativeResourcesException expected to be thrown.");
    	}catch(QNoNativeResourcesException e) {}
    	assertTrue(array.isDisposed());
    }
}
