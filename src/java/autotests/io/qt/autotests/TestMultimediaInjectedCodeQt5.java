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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.junit.Assume.assumeTrue;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtUtilities;
import io.qt.core.QByteArray;
import io.qt.core.QFile;
import io.qt.core.QIODevice;
import io.qt.core.QJsonDocument;
import io.qt.core.QJsonValue;
import io.qt.core.QPluginLoader;
import io.qt.core.QSize;
import io.qt.gui.QImage;
import io.qt.multimedia.QAbstractVideoBuffer;
import io.qt.multimedia.QAudioEncoderSettings;
import io.qt.multimedia.QAudioEncoderSettingsControl;
import io.qt.multimedia.QCamera;
import io.qt.multimedia.QCameraExposure;
import io.qt.multimedia.QCameraExposureControl;
import io.qt.multimedia.QCameraImageCapture;
import io.qt.multimedia.QImageEncoderControl;
import io.qt.multimedia.QMediaRecorder;
import io.qt.multimedia.QMediaService;
import io.qt.multimedia.QMediaService.ListResult;
import io.qt.multimedia.QMediaServiceProviderFactoryInterface;
import io.qt.multimedia.QMediaServiceProviderPlugin;
import io.qt.multimedia.QVideoEncoderSettingsControl;
import io.qt.multimedia.QVideoFrame;

public class TestMultimediaInjectedCodeQt5 extends ApplicationInitializer {
	@BeforeClass
    public static void testInitialize() throws Exception {
        QtUtilities.initializePackage("io.qt.multimedia");
        ApplicationInitializer.testInitialize();
    }
	
    @Test
    public void testQAbstractVideoBuffer_map()
    {
    	QFile file = new QFile(":io/qt/autotests/svgcards-example.png");
    	assertTrue(file.open(QIODevice.OpenModeFlag.ReadOnly));
    	QByteArray imageData = file.readAll();
    	file.close();
    	file.dispose();
    	QImage image = QImage.fromData(imageData, "png");
    	QVideoFrame frame = new QVideoFrame(image);
    	assertTrue(frame.isValid());
    	QAbstractVideoBuffer buffer = frame.buffer();
    	assertTrue(buffer!=null);
    	QAbstractVideoBuffer.MapResult result = buffer.map(QAbstractVideoBuffer.MapMode.ReadOnly);
    	assertEquals(QAbstractVideoBuffer.MapMode.ReadOnly, buffer.mapMode());
    	assertTrue(result!=null);
    	assertTrue(result.data!=null);
    	assertEquals(image.bytesPerLine(), result.bytesPerLine);
    	assertEquals(image.sizeInBytes(), result.data.capacity());
    	QImage newImage = new QImage(result.data, image.width(), image.height(), QImage.Format.Format_ARGB32);
    	assertEquals(image, newImage);
    	newImage.dispose();
    	buffer.unmap();
    	frame.dispose();
    }
    
    @Test
    public void testQAbstractVideoBuffer_mapPlanes()
    {
    	QFile file = new QFile(":io/qt/autotests/svgcards-example.png");
    	assertTrue(file.open(QIODevice.OpenModeFlag.ReadOnly));
    	QByteArray imageData = file.readAll();
    	file.close();
    	file.dispose();
    	QImage image = QImage.fromData(imageData, "png");
    	QVideoFrame frame = new QVideoFrame(image);
    	assertTrue(frame.isValid());
    	QAbstractVideoBuffer buffer = frame.buffer();
    	assertTrue(buffer!=null);
    	QAbstractVideoBuffer.MapResult[] result = buffer.mapPlanes(QAbstractVideoBuffer.MapMode.ReadWrite);
    	assertEquals(QAbstractVideoBuffer.MapMode.ReadWrite, buffer.mapMode());
    	assertTrue(result!=null);
    	assertEquals(1, result.length);
    	assertTrue(result[0].data!=null);
    	assertEquals(image.bytesPerLine(), result[0].bytesPerLine);
    	assertEquals(image.sizeInBytes(), result[0].data.capacity());
    	QImage newImage = new QImage(result[0].data, image.width(), image.height(), QImage.Format.Format_ARGB32);
    	assertEquals(image, newImage);
    	newImage.dispose();
    	buffer.unmap();
    	frame.dispose();
    }
    
    @Test
    public void testQAudioEncoderSettingsControl_supportedSampleRates()
    {
    	QPluginLoader pluginLoader = new QPluginLoader();
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/dsengine");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/dsengined");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/qtmedia_audioengine");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/qtmedia_audioengined");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/qavfmediaplayer");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/qavfmediaplayer_debug");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/gstmediaplayer");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/gstmediaplayer_debug");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/gstmediacapture");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/gstmediacapture_debug");
    	}
    	{
	    	boolean b = pluginLoader.isLoaded() || pluginLoader.load();
			Assert.assertTrue(pluginLoader.errorString(), b);
    	}
    	java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "QPluginLoader: "+pluginLoader.fileName() + " = " + new QJsonDocument(pluginLoader.metaData()).toJson());
    	QMediaServiceProviderPlugin servicePlugin = pluginLoader.instance(QMediaServiceProviderPlugin.class);
    	Assert.assertTrue(servicePlugin!=null);
    	QJsonValue metaData = pluginLoader.metaData().value("MetaData");
    	Assert.assertTrue(metaData.isObject());
    	QJsonValue services = metaData.toObject().value("Services");
    	Assert.assertTrue(services.isArray());
    	if(services.toArray().contains(new QJsonValue(QMediaServiceProviderFactoryInterface.Q_MEDIASERVICE_AUDIODECODER))) {
	    	QMediaService service = servicePlugin.create(QMediaServiceProviderFactoryInterface.Q_MEDIASERVICE_AUDIODECODER);
	    	Assert.assertTrue(service!=null);
	    	QAudioEncoderSettingsControl control = service.requestControl(QAudioEncoderSettingsControl.class);
	    	Assert.assertTrue(control!=null);
	    	ListResult<Integer> result = control.supportedSampleRates(new QAudioEncoderSettings());
	    	Assert.assertTrue(result!=null);
	    	service.releaseControl(control);
	    	servicePlugin.release(service);
    	}
    }
    
    @Test
    public void testQCameraExposureControl_supportedParameterRange()
    {
    	QPluginLoader pluginLoader = new QPluginLoader();
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/dsengine");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/dsengined");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/qtmedia_audioengine");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/qtmedia_audioengined");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/qavfmediaplayer");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/qavfmediaplayer_debug");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/gstmediaplayer");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/gstmediaplayer_debug");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/gstmediacapture");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/gstmediacapture_debug");
    	}
    	{
	    	boolean b = pluginLoader.isLoaded() || pluginLoader.load();
			Assert.assertTrue(pluginLoader.errorString(), b);
    	}
    	java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "QPluginLoader: "+pluginLoader.fileName() + " = " + new QJsonDocument(pluginLoader.metaData()).toJson());
    	QMediaServiceProviderPlugin servicePlugin = pluginLoader.instance(QMediaServiceProviderPlugin.class);
    	Assert.assertTrue(servicePlugin!=null);
    	QJsonValue metaData = pluginLoader.metaData().value("MetaData");
    	Assert.assertTrue(metaData.isObject());
    	QJsonValue services = metaData.toObject().value("Services");
    	Assert.assertTrue(services.isArray());
    	if(services.toArray().contains(new QJsonValue(QMediaServiceProviderFactoryInterface.Q_MEDIASERVICE_AUDIODECODER))) {
	    	QMediaService service = servicePlugin.create(QMediaServiceProviderFactoryInterface.Q_MEDIASERVICE_AUDIODECODER);
	    	Assert.assertTrue(service!=null);
	    	QCameraExposureControl control = service.requestControl(QCameraExposureControl.class);
	    	Assert.assertTrue(control!=null);
	    	ListResult<Object> result = control.supportedParameterRange(QCameraExposureControl.ExposureParameter.ISO);
	    	Assert.assertTrue(result!=null);
	    	service.releaseControl(control);
	    	servicePlugin.release(service);
    	}
    }
    
    @Test
    public void testQCameraExposure_supportedApertures()
    {
    	QCamera cam = new QCamera();
    	QCameraExposure exposure = cam.exposure();
    	Assert.assertTrue(exposure!=null);
    	ListResult<Double> result = exposure.supportedApertures();
    	Assert.assertTrue(result!=null);
    }
    
    @Test
    public void testQCameraExposure_supportedIsoSensitivities()
    {
    	QCamera cam = new QCamera();
    	QCameraExposure exposure = cam.exposure();
    	Assert.assertTrue(exposure!=null);
    	ListResult<Integer> result = exposure.supportedIsoSensitivities();
    	Assert.assertTrue(result!=null);
    }
    
    @Test
    public void testQCameraExposure_supportedShutterSpeeds()
    {
    	QCamera cam = new QCamera();
    	QCameraExposure exposure = cam.exposure();
    	Assert.assertTrue(exposure!=null);
    	ListResult<Double> result = exposure.supportedShutterSpeeds();
    	Assert.assertTrue(result!=null);
    }
    
    @Test
    public void testQImageEncoderControl_supportedResolutions()
    {
    	QPluginLoader pluginLoader = new QPluginLoader();
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/qtmedia_audioengine");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/qtmedia_audioengined");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/qavfcamera");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/qavfcamera_debug");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/gstmediacapture");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/gstmediacapture_debug");
    	}
    	{
	    	boolean b = pluginLoader.isLoaded() || pluginLoader.load();
			Assert.assertTrue(pluginLoader.errorString(), b);
    	}
    	java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "QPluginLoader: "+pluginLoader.fileName() + " = " + new QJsonDocument(pluginLoader.metaData()).toJson());
    	QMediaServiceProviderPlugin servicePlugin = pluginLoader.instance(QMediaServiceProviderPlugin.class);
    	Assert.assertTrue(servicePlugin!=null);
    	QJsonValue metaData = pluginLoader.metaData().value("MetaData");
    	Assert.assertTrue(metaData.isObject());
    	QJsonValue services = metaData.toObject().value("Services");
    	Assert.assertTrue(services.isArray());
    	if(services.toArray().contains(new QJsonValue(QMediaServiceProviderFactoryInterface.Q_MEDIASERVICE_MEDIAPLAYER))) {
	    	QMediaService service = servicePlugin.create(QMediaServiceProviderFactoryInterface.Q_MEDIASERVICE_MEDIAPLAYER);
	    	Assert.assertTrue(service!=null);
	    	QImageEncoderControl control = service.requestControl(QImageEncoderControl.class);
	    	Assert.assertTrue(control!=null);
	    	ListResult<QSize> result = control.supportedResolutions(null);
	    	Assert.assertTrue(result!=null);
	    	service.releaseControl(control);
	    	servicePlugin.release(service);
    	}
    }
    
    @Test
    public void testQMediaRecorder_supportedAudioSampleRates()
    {
    	QMediaRecorder recorder = new QMediaRecorder(null);
    	ListResult<Integer> result = recorder.supportedAudioSampleRates();
    	Assert.assertTrue(result!=null);
    }
    
    @Test
    public void testQMediaRecorder_supportedFrameRates()
    {
    	QMediaRecorder recorder = new QMediaRecorder(null);
    	ListResult<Double> result = recorder.supportedFrameRates();
    	Assert.assertTrue(result!=null);
    }
    
    @Test
    public void testQMediaRecorder_supportedResolutions()
    {
    	QMediaRecorder recorder = new QMediaRecorder(null);
    	ListResult<QSize> result = recorder.supportedResolutions();
    	Assert.assertTrue(result!=null);
    }
    
    @Test
    public void testQVideoEncoderSettingsControl_supportedFrameRates()
    {
    	QPluginLoader pluginLoader = new QPluginLoader();
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/qtmedia_audioengine");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/qtmedia_audioengined");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/qavfcamera");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/qavfcamera_debug");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/gstmediacapture");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/gstmediacapture_debug");
    	}
    	{
	    	boolean b = pluginLoader.isLoaded() || pluginLoader.load();
			Assert.assertTrue(pluginLoader.errorString(), b);
    	}
    	java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "QPluginLoader: "+pluginLoader.fileName() + " = " + new QJsonDocument(pluginLoader.metaData()).toJson());
    	QMediaServiceProviderPlugin servicePlugin = pluginLoader.instance(QMediaServiceProviderPlugin.class);
    	Assert.assertTrue(servicePlugin!=null);
    	QJsonValue metaData = pluginLoader.metaData().value("MetaData");
    	Assert.assertTrue(metaData.isObject());
    	QJsonValue services = metaData.toObject().value("Services");
    	Assert.assertTrue(services.isArray());
    	if(services.toArray().contains(new QJsonValue(QMediaServiceProviderFactoryInterface.Q_MEDIASERVICE_MEDIAPLAYER))) {
	    	QMediaService service = servicePlugin.create(QMediaServiceProviderFactoryInterface.Q_MEDIASERVICE_MEDIAPLAYER);
	    	Assert.assertTrue(service!=null);
	    	QVideoEncoderSettingsControl control = service.requestControl(QVideoEncoderSettingsControl.class);
	    	Assert.assertTrue(control!=null);
	    	ListResult<Double> result = control.supportedFrameRates(null);
	    	Assert.assertTrue(result!=null);
	    	service.releaseControl(control);
	    	servicePlugin.release(service);
    	}
    }
    
    @Test
    public void testQVideoEncoderSettingsControl_supportedResolutions()
    {
    	QPluginLoader pluginLoader = new QPluginLoader();
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/qtmedia_audioengine");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/qtmedia_audioengined");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/qavfcamera");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/qavfcamera_debug");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/gstmediacapture");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/gstmediacapture_debug");
    	}
    	{
	    	boolean b = pluginLoader.isLoaded() || pluginLoader.load();
			Assert.assertTrue(pluginLoader.errorString(), b);
    	}
    	java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "QPluginLoader: "+pluginLoader.fileName() + " = " + new QJsonDocument(pluginLoader.metaData()).toJson());
    	QMediaServiceProviderPlugin servicePlugin = pluginLoader.instance(QMediaServiceProviderPlugin.class);
    	Assert.assertTrue(servicePlugin!=null);
    	QJsonValue metaData = pluginLoader.metaData().value("MetaData");
    	Assert.assertTrue(metaData.isObject());
    	QJsonValue services = metaData.toObject().value("Services");
    	Assert.assertTrue(services.isArray());
    	if(services.toArray().contains(new QJsonValue(QMediaServiceProviderFactoryInterface.Q_MEDIASERVICE_MEDIAPLAYER))) {
	    	QMediaService service = servicePlugin.create(QMediaServiceProviderFactoryInterface.Q_MEDIASERVICE_MEDIAPLAYER);
	    	Assert.assertTrue(service!=null);
	    	QVideoEncoderSettingsControl control = service.requestControl(QVideoEncoderSettingsControl.class);
	    	Assert.assertTrue(control!=null);
	    	ListResult<QSize> result = control.supportedResolutions(null);
	    	Assert.assertTrue(result!=null);
	    	service.releaseControl(control);
	    	servicePlugin.release(service);
    	}
    }
    
    @Test
    public void testQCameraImageCapture_supportedResolutions()
    {
    	QCameraImageCapture capture = new QCameraImageCapture(null);
    	ListResult<QSize> result = capture.supportedResolutions();
    	Assert.assertTrue(result!=null);
    }
}
