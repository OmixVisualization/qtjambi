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

import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

import org.junit.Assert;
import org.junit.Test;

import io.qt.QtUtilities;
import io.qt.autotests.generated.General;
import io.qt.core.QLibraryInfo;
import io.qt.core.QObject;
import io.qt.core.QPluginLoader;
import io.qt.location.QGeoCodingManagerEngine;
import io.qt.location.QGeoRoutingManagerEngine;
import io.qt.location.QGeoServiceProvider;
import io.qt.location.QGeoServiceProviderFactory;
import io.qt.location.QGeoServiceProviderFactory.Result;
import io.qt.location.QPlaceManagerEngine;

public class TestLocationInjectedCode extends ApplicationInitializer {
	
	static {
		QtUtilities.initializePackage(io.qt.location.QGeoServiceProviderFactory.class);
	}
	
	@Test
    public void test()
    {
		String errorString = "";
		QPluginLoader pluginLoader = null;
		if(QLibraryInfo.version().majorVersion()<=5) {
			pluginLoader = new QPluginLoader("geoservices/qtgeoservices_mapbox");
			if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
				pluginLoader = new QPluginLoader("geoservices/qtgeoservices_mapboxd");
			}
			if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
				pluginLoader = new QPluginLoader("geoservices/qtgeoservices_mapbox_debug");
			}
			if(pluginLoader.isLoaded() || pluginLoader.load()){
				Map<String,Object> parameters = new HashMap<>();
				parameters.put("mapbox.access_token", "12345");
	
				QObject servicePlugin = pluginLoader.instance();
				Assert.assertTrue(servicePlugin!=null);
				Assert.assertTrue(servicePlugin.inherits(QGeoServiceProviderFactory.class));
				QGeoServiceProviderFactory factory = servicePlugin.qt_metacast(QGeoServiceProviderFactory.class);
				
				Result<QGeoRoutingManagerEngine> createRoutingManagerEngine = factory.createRoutingManagerEngine(parameters);
				Assert.assertTrue(createRoutingManagerEngine!=null);
				Assert.assertTrue(createRoutingManagerEngine.engine()!=null);
				Assert.assertEquals(null, createRoutingManagerEngine.engine().parent());
				Assert.assertTrue(General.internalAccess.isJavaOwnership(createRoutingManagerEngine.engine()));
				Assert.assertEquals(QGeoServiceProvider.Error.NoError, createRoutingManagerEngine.error());
				Assert.assertEquals(null, createRoutingManagerEngine.errorString());
				Assert.assertEquals("", createRoutingManagerEngine.engine().managerName());
				Assert.assertEquals("QGeoRoutingManagerEngineMapbox", createRoutingManagerEngine.engine().metaObject().className());
				
				Result<QPlaceManagerEngine> createPlaceManagerEngine = factory.createPlaceManagerEngine(Collections.emptyMap());
				Assert.assertTrue(createPlaceManagerEngine!=null);
				Assert.assertEquals(null, createPlaceManagerEngine.engine());
				Assert.assertEquals(QGeoServiceProvider.Error.MissingRequiredParameterError, createPlaceManagerEngine.error());
				Assert.assertEquals("Mapbox plugin requires a 'mapbox.access_token' parameter.\n" + 
						"Please visit https://www.mapbox.com", createPlaceManagerEngine.errorString());
				
				Result<QGeoCodingManagerEngine> createGeocodingManagerEngine = factory.createGeocodingManagerEngine(Collections.emptyMap());
				Assert.assertTrue(createGeocodingManagerEngine!=null);
				Assert.assertEquals(null, createGeocodingManagerEngine.engine());
				Assert.assertEquals(QGeoServiceProvider.Error.MissingRequiredParameterError, createGeocodingManagerEngine.error());
				Assert.assertEquals("Mapbox plugin requires a 'mapbox.access_token' parameter.\n" + 
						"Please visit https://www.mapbox.com", createGeocodingManagerEngine.errorString());
				
				servicePlugin.dispose();
				Assert.assertTrue(factory.isDisposed());
			}else {
				errorString += pluginLoader.errorString()+" ("+pluginLoader.fileName()+") ";
			}
			
			pluginLoader = new QPluginLoader("geoservices/qtgeoservices_nokia");
			if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
	    		pluginLoader = new QPluginLoader("geoservices/qtgeoservices_nokiad");
	    	}
			if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
	    		pluginLoader = new QPluginLoader("geoservices/qtgeoservices_nokia_debug");
	    	}
			if(pluginLoader.isLoaded() || pluginLoader.load()){
				Map<String,Object> parameters = new HashMap<>();
				parameters.put("here.app_id", "qtjambi");
				parameters.put("here.token", "12345");

				QObject servicePlugin = pluginLoader.instance();
				Assert.assertTrue(servicePlugin!=null);
				Assert.assertTrue(servicePlugin.inherits(QGeoServiceProviderFactory.class));
				QGeoServiceProviderFactory factory = servicePlugin.qt_metacast(QGeoServiceProviderFactory.class);

				Result<QGeoRoutingManagerEngine> createRoutingManagerEngine = factory.createRoutingManagerEngine(parameters);
				Assert.assertTrue(createRoutingManagerEngine!=null);
				Assert.assertTrue(createRoutingManagerEngine.engine()!=null);
				Assert.assertEquals(null, createRoutingManagerEngine.engine().parent());
				Assert.assertTrue(General.internalAccess.isJavaOwnership(createRoutingManagerEngine.engine()));
				Assert.assertEquals(QGeoServiceProvider.Error.NoError, createRoutingManagerEngine.error());
				Assert.assertEquals(null, createRoutingManagerEngine.errorString());
				Assert.assertEquals("", createRoutingManagerEngine.engine().managerName());
				Assert.assertEquals("QGeoRoutingManagerEngineNokia", createRoutingManagerEngine.engine().metaObject().className());
				
				Result<QPlaceManagerEngine> createPlaceManagerEngine = factory.createPlaceManagerEngine(Collections.emptyMap());
				Assert.assertTrue(createPlaceManagerEngine!=null);
				Assert.assertEquals(null, createPlaceManagerEngine.engine());
				Assert.assertEquals(QGeoServiceProvider.Error.MissingRequiredParameterError, createPlaceManagerEngine.error());
				Assert.assertEquals("Qt Location requires app_id and token parameters.\n" + 
						"Please register at https://developer.here.com/ to get your personal application credentials.", createPlaceManagerEngine.errorString());
				
				Result<QGeoCodingManagerEngine> createGeocodingManagerEngine = factory.createGeocodingManagerEngine(Collections.emptyMap());
				Assert.assertTrue(createGeocodingManagerEngine!=null);
				Assert.assertEquals(null, createGeocodingManagerEngine.engine());
				Assert.assertEquals(QGeoServiceProvider.Error.MissingRequiredParameterError, createGeocodingManagerEngine.error());
				Assert.assertEquals("Qt Location requires app_id and token parameters.\n" + 
						"Please register at https://developer.here.com/ to get your personal application credentials.", createGeocodingManagerEngine.errorString());
			}else {
				errorString += pluginLoader.errorString()+" ("+pluginLoader.fileName()+") ";
			}
		}
		
		pluginLoader = new QPluginLoader("geoservices/qtgeoservices_itemsoverlay");
		if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
			pluginLoader = new QPluginLoader("geoservices/qtgeoservices_itemsoverlayd");
    	}
		if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
			pluginLoader = new QPluginLoader("geoservices/qtgeoservices_itemsoverlay_debug");
    	}
		if(pluginLoader.isLoaded() || pluginLoader.load()){
			QObject servicePlugin = pluginLoader.instance();
			Assert.assertTrue(servicePlugin!=null);
			Assert.assertTrue(servicePlugin.inherits(QGeoServiceProviderFactory.class));
			QGeoServiceProviderFactory factory = servicePlugin.qt_metacast(QGeoServiceProviderFactory.class);

			Result<QGeoRoutingManagerEngine> createRoutingManagerEngine = factory.createRoutingManagerEngine(Collections.emptyMap());
			Assert.assertTrue(createRoutingManagerEngine!=null);
			Assert.assertEquals(null, createRoutingManagerEngine.engine());
			Assert.assertEquals(QGeoServiceProvider.Error.NoError, createRoutingManagerEngine.error());
			Assert.assertEquals(null, createRoutingManagerEngine.errorString());
			
			Result<QPlaceManagerEngine> createPlaceManagerEngine = factory.createPlaceManagerEngine(Collections.emptyMap());
			Assert.assertTrue(createPlaceManagerEngine!=null);
			Assert.assertEquals(null, createPlaceManagerEngine.engine());
			Assert.assertEquals(QGeoServiceProvider.Error.NoError, createPlaceManagerEngine.error());
			Assert.assertEquals(null, createPlaceManagerEngine.errorString());
			
			Result<QGeoCodingManagerEngine> createGeocodingManagerEngine = factory.createGeocodingManagerEngine(Collections.emptyMap());
			Assert.assertTrue(createGeocodingManagerEngine!=null);
			Assert.assertEquals(null, createGeocodingManagerEngine.engine());
			Assert.assertEquals(QGeoServiceProvider.Error.NoError, createGeocodingManagerEngine.error());
			Assert.assertEquals(null, createGeocodingManagerEngine.errorString());

			QGeoServiceProviderFactory factory2 = servicePlugin.qt_metacast(QGeoServiceProviderFactory.class);
			
			Assert.assertTrue(factory2==factory);

			factory.dispose();
			servicePlugin.objectName(); // must not be deleted
			Assert.assertFalse(servicePlugin.isDisposed());

			QGeoServiceProviderFactory factory3 = servicePlugin.qt_metacast(QGeoServiceProviderFactory.class);
			Assert.assertTrue(factory3!=factory);
			Assert.assertTrue(factory3!=factory2);
		}else {
			errorString += pluginLoader.errorString()+" ("+pluginLoader.fileName()+") ";
		}

		
		pluginLoader = new QPluginLoader("geoservices/qtgeoservices_osm");
		if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("geoservices/qtgeoservices_osmd");
    	}
		if(!pluginLoader.isLoaded() && !pluginLoader.load()) {
    		pluginLoader = new QPluginLoader("geoservices/qtgeoservices_osm_debug");
    	}
		if(pluginLoader.isLoaded() || pluginLoader.load()){
			QObject servicePlugin = pluginLoader.instance();
			Assert.assertTrue(servicePlugin!=null);
			Assert.assertTrue(servicePlugin.inherits(QGeoServiceProviderFactory.class));
			QGeoServiceProviderFactory factory = servicePlugin.qt_metacast(QGeoServiceProviderFactory.class);

			Result<QGeoRoutingManagerEngine> createRoutingManagerEngine = factory.createRoutingManagerEngine(Collections.emptyMap());
			Assert.assertTrue(createRoutingManagerEngine!=null);
			Assert.assertTrue(createRoutingManagerEngine.engine()!=null);
			Assert.assertEquals(null, createRoutingManagerEngine.engine().parent());
			Assert.assertTrue(General.internalAccess.isJavaOwnership(createRoutingManagerEngine.engine()));
			Assert.assertEquals(QGeoServiceProvider.Error.NoError, createRoutingManagerEngine.error());
			Assert.assertEquals(null, createRoutingManagerEngine.errorString());
			Assert.assertEquals("", createRoutingManagerEngine.engine().managerName());
			Assert.assertEquals("QGeoRoutingManagerEngineOsm", createRoutingManagerEngine.engine().metaObject().className());
			
			Result<QPlaceManagerEngine> createPlaceManagerEngine = factory.createPlaceManagerEngine(Collections.emptyMap());
			Assert.assertTrue(createPlaceManagerEngine!=null);
			Assert.assertTrue(createPlaceManagerEngine.engine()!=null);
			Assert.assertEquals(null, createPlaceManagerEngine.engine().parent());
			Assert.assertTrue(General.internalAccess.isJavaOwnership(createPlaceManagerEngine.engine()));
			Assert.assertEquals(QGeoServiceProvider.Error.NoError, createPlaceManagerEngine.error());
			Assert.assertEquals(null, createPlaceManagerEngine.errorString());
			Assert.assertEquals("", createPlaceManagerEngine.engine().managerName());
			Assert.assertEquals("QPlaceManagerEngineOsm", createPlaceManagerEngine.engine().metaObject().className());
			
			Result<QGeoCodingManagerEngine> createGeocodingManagerEngine = factory.createGeocodingManagerEngine(Collections.emptyMap());
			Assert.assertTrue(createGeocodingManagerEngine!=null);
			Assert.assertTrue(createGeocodingManagerEngine.engine()!=null);
			Assert.assertEquals(null, createGeocodingManagerEngine.engine().parent());
			Assert.assertTrue(General.internalAccess.isJavaOwnership(createGeocodingManagerEngine.engine()));
			Assert.assertEquals(QGeoServiceProvider.Error.NoError, createGeocodingManagerEngine.error());
			Assert.assertEquals(null, createGeocodingManagerEngine.errorString());
			Assert.assertEquals("", createGeocodingManagerEngine.engine().managerName());
			Assert.assertEquals("QGeoCodingManagerEngineOsm", createGeocodingManagerEngine.engine().metaObject().className());
		}else {
			errorString += pluginLoader.errorString()+" ("+pluginLoader.fileName()+") ";
		}

		{
			Assert.assertTrue(errorString, errorString.isEmpty());
    	}
    }
}
