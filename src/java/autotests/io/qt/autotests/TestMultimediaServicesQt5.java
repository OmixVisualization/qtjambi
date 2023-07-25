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

import static org.junit.Assume.assumeTrue;

import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtUtilities;
import io.qt.core.QByteArray;
import io.qt.core.QFileInfo;
import io.qt.core.QIODevice;
import io.qt.core.QJsonArray;
import io.qt.core.QJsonDocument;
import io.qt.core.QJsonValue;
import io.qt.core.QObject;
import io.qt.core.QPluginLoader;
import io.qt.core.QUrl;
import io.qt.multimedia.QAudioDecoderControl;
import io.qt.multimedia.QCameraControl;
import io.qt.multimedia.QMediaContent;
import io.qt.multimedia.QMediaControl;
import io.qt.multimedia.QMediaPlayer;
import io.qt.multimedia.QMediaPlayer.MediaStatus;
import io.qt.multimedia.QMediaPlayer.State;
import io.qt.multimedia.QMediaPlayerControl;
import io.qt.multimedia.QMediaRecorderControl;
import io.qt.multimedia.QMediaService;
import io.qt.multimedia.QMediaServiceFeaturesInterface;
import io.qt.multimedia.QMediaServiceProviderFactoryInterface;
import io.qt.multimedia.QMediaServiceProviderHint;
import io.qt.multimedia.QMediaServiceProviderHint.Features;
import io.qt.multimedia.QMediaServiceProviderPlugin;
import io.qt.multimedia.QMediaServiceSupportedDevicesInterface;
import io.qt.multimedia.QMediaServiceSupportedFormatsInterface;
import io.qt.multimedia.QMediaTimeRange;
import io.qt.multimedia.QMultimedia.SupportEstimate;
import io.qt.multimedia.QRadioTunerControl;

public class TestMultimediaServicesQt5 extends ApplicationInitializer {
	
    @BeforeClass
    public static void testInitialize() throws Exception {
        QtUtilities.initializePackage("io.qt.multimedia");
        ApplicationInitializer.testInitializeWithWidgets();
    }

    @Test
    public void test() {
    	QPluginLoader pluginLoader = new QPluginLoader("mediaservice/wmfengine");
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/wmfengined");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/dsengine");
    	}
    	if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("mediaservice/dsengined");
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
		{
			QObject servicePlugin = pluginLoader.instance();
			Assert.assertTrue(new QFileInfo(pluginLoader.fileName()).fileName() + ": " + servicePlugin.metaObject().className() +" ("+servicePlugin.getClass().getName()+") expected to inherit QMediaServiceProviderPlugin", servicePlugin.metaObject().inherits(QMediaServiceProviderPlugin.staticMetaObject));
		}
    	QMediaServiceProviderPlugin servicePlugin = pluginLoader.instance(QMediaServiceProviderPlugin.class);
    	Assert.assertTrue(servicePlugin!=null);
    	QJsonValue metaData = pluginLoader.metaData().value("MetaData");
    	Assert.assertTrue(metaData.isObject());
    	QJsonValue services = metaData.toObject().value("Services");
    	Assert.assertTrue(services.isArray());
		if(services.toArray().contains(new QJsonValue(QMediaServiceProviderFactoryInterface.Q_MEDIASERVICE_MEDIAPLAYER))) {
	    	QMediaService service = servicePlugin.create(QMediaServiceProviderFactoryInterface.Q_MEDIASERVICE_MEDIAPLAYER);
	    	Assert.assertTrue(service!=null);
	    	QMediaPlayerControl control = service.requestControl(QMediaPlayerControl.class);
	    	Assert.assertTrue(control!=null);
	    	service.releaseControl(control);
	    	servicePlugin.release(service);
		}else if(services.toArray().contains(new QJsonValue(QMediaServiceProviderFactoryInterface.Q_MEDIASERVICE_CAMERA))) {
	    	QMediaService service = servicePlugin.create(QMediaServiceProviderFactoryInterface.Q_MEDIASERVICE_CAMERA);
	    	Assert.assertTrue(service!=null);
	    	QCameraControl control = service.requestControl(QCameraControl.class);
	    	Assert.assertTrue(control!=null);
	    	service.releaseControl(control);
	    	servicePlugin.release(service);
		}else if(services.toArray().contains(new QJsonValue(QMediaServiceProviderFactoryInterface.Q_MEDIASERVICE_AUDIODECODER))) {
	    	QMediaService service = servicePlugin.create(QMediaServiceProviderFactoryInterface.Q_MEDIASERVICE_AUDIODECODER);
	    	Assert.assertTrue(service!=null);
	    	QAudioDecoderControl control = service.requestControl(QAudioDecoderControl.class);
	    	Assert.assertTrue(control!=null);
	    	service.releaseControl(control);
	    	servicePlugin.release(service);
		}else if(services.toArray().contains(new QJsonValue(QMediaServiceProviderFactoryInterface.Q_MEDIASERVICE_AUDIOSOURCE))) {
	    	QMediaService service = servicePlugin.create(QMediaServiceProviderFactoryInterface.Q_MEDIASERVICE_AUDIOSOURCE);
	    	Assert.assertTrue(service!=null);
	    	QMediaRecorderControl control = service.requestControl(QMediaRecorderControl.class);
	    	Assert.assertTrue(control!=null);
	    	service.releaseControl(control);
	    	servicePlugin.release(service);
		}else if(services.toArray().contains(new QJsonValue(QMediaServiceProviderFactoryInterface.Q_MEDIASERVICE_RADIO))) {
	    	QMediaService service = servicePlugin.create(QMediaServiceProviderFactoryInterface.Q_MEDIASERVICE_RADIO);
	    	Assert.assertTrue(service!=null);
	    	QRadioTunerControl control = service.requestControl(QRadioTunerControl.class);
	    	Assert.assertTrue(control!=null);
	    	service.releaseControl(control);
	    	servicePlugin.release(service);
		}
    }
    
    static class MyMediaServiceProviderPlugin extends QMediaServiceProviderPlugin implements QMediaServiceSupportedFormatsInterface, QMediaServiceSupportedDevicesInterface, QMediaServiceFeaturesInterface{
		QMediaContent _media;
		QIODevice _stream;
		boolean _playCommand;
    	
    	class MyMediaService extends QMediaService{
			protected MyMediaService(QObject parent) {
				super(parent);
			}

			@Override
			public void releaseControl(QMediaControl control) {
			}

			@Override
			public <T extends QMediaControl> T requestControl(Class<T> name) {
				if(name==QMediaPlayerControl.class) {
					return name.cast(new MyMediaPlayerControl(this));
				}
				return null;
			}
    	}
    	
    	class MyMediaPlayerControl extends QMediaPlayerControl{
    		
			public MyMediaPlayerControl(QObject parent) {
				super(parent);
			}

			@Override
			public QMediaTimeRange availablePlaybackRanges() {
				return new QMediaTimeRange();
			}

			@Override
			public int bufferStatus() {
				return 0;
			}

			@Override
			public long duration() {
				return 0;
			}

			@Override
			public boolean isAudioAvailable() {
				return false;
			}

			@Override
			public boolean isMuted() {
				return false;
			}

			@Override
			public boolean isSeekable() {
				return false;
			}

			@Override
			public boolean isVideoAvailable() {
				return false;
			}

			@Override
			public QMediaContent media() {
				return _media;
			}

			@Override
			public MediaStatus mediaStatus() {
				return MediaStatus.LoadedMedia;
			}

			@Override
			public QIODevice mediaStream() {
				return _stream;
			}

			@Override
			public void pause() {
			}

			@Override
			public void play() {
				_playCommand = true;
			}

			@Override
			public double playbackRate() {
				return 0;
			}

			@Override
			public long position() {
				return 0;
			}

			@Override
			public void setMedia(QMediaContent media, QIODevice stream) {
				_media = media;
				_stream = stream;
			}

			@Override
			public void setMuted(boolean mute) {
			}

			@Override
			public void setPlaybackRate(double rate) {
			}

			@Override
			public void setPosition(long position) {
			}

			@Override
			public void setVolume(int volume) {
			}

			@Override
			public State state() {
				return State.PausedState;
			}

			@Override
			public void stop() {
			}

			@Override
			public int volume() {
				return 0;
			}
    		
    	}
    	
		@Override
		public QMediaService create(String key) {
			if(Q_MEDIASERVICE_MEDIAPLAYER.equals(key)) {
				return new MyMediaService(this);
			}
			return null;
		}

		@Override
		public void release(QMediaService service) {
		}

		@SuppressWarnings("unlikely-arg-type")
		@Override
		public Features supportedFeatures(QByteArray service) {
			if(service.equals(Q_MEDIASERVICE_MEDIAPLAYER)) {
				return QMediaServiceProviderHint.Feature.flags(QMediaServiceProviderHint.Feature.values());
			}
			return null;
		}

		@Override
		public String deviceDescription(QByteArray service, QByteArray device) {
			return null;
		}

		@Override
		public List<QByteArray> devices(QByteArray service) {
			return Collections.emptyList();
		}

		@Override
		public SupportEstimate hasSupport(String mimeType, Collection<String> codecs) {
			return SupportEstimate.MaybeSupported;
		}

		@Override
		public List<String> supportedMimeTypes() {
			return Collections.emptyList();
		}
    }
    
    @Test
    public void testMediaPlayerPlugin() {
    	MyMediaServiceProviderPlugin plugin = new MyMediaServiceProviderPlugin();
    	QJsonValue key = new QJsonValue("Test");
    	QJsonValue value = new QJsonValue(QMediaServiceProviderFactoryInterface.Q_MEDIASERVICE_MEDIAPLAYER);
    	Map<String, Object> map = new HashMap<>();
    	map.put("Keys", new QJsonArray(key));
    	map.put("Services", new QJsonArray(value));
    	QPluginLoader.qRegisterStaticPluginFunction(plugin, map);
    	QMediaPlayer player = new QMediaPlayer(null, QMediaPlayer.Flag.VideoSurface, QMediaPlayer.Flag.StreamPlayback);
    	QMediaContent content = new QMediaContent(QUrl.fromLocalFile("Test.mp4"));
    	player.setMedia(content);
    	try {
	    	Assert.assertEquals(content, plugin._media);
	    	Assert.assertEquals(null, plugin._stream);
	    	Assert.assertFalse(plugin._playCommand);
	    	player.play();
	    	Assert.assertTrue(plugin._playCommand);
	    	Assert.assertEquals(player.state(), QMediaPlayer.State.PausedState);
    	}finally {
	    	content.dispose();
	    	player.dispose();
	    	plugin.dispose();
    	}
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestMultimediaServicesQt5.class.getName());
    }
}
