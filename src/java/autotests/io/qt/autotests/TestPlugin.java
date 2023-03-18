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

import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtUtilities;
import io.qt.core.QCoreApplication;
import io.qt.core.QFileInfo;
import io.qt.core.QIODevice;
import io.qt.core.QLibrary;
import io.qt.core.QMetaObject;
import io.qt.core.QObject;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QOperatingSystemVersion.OSType;
import io.qt.core.QPluginLoader;
import io.qt.core.internal.QFactoryLoader;
import io.qt.core.internal.QtPluginMetaData;
import io.qt.gui.QGenericPlugin;
import io.qt.gui.QIconEngine;
import io.qt.gui.QIconEnginePlugin;
import io.qt.gui.QImageIOHandler;
import io.qt.gui.QImageIOPlugin;
import io.qt.widgets.QGraphicsItem;
import io.qt.widgets.QGraphicsObject;
import io.qt.widgets.QGraphicsWidget;
import io.qt.widgets.QStyle;
import io.qt.widgets.QStylePlugin;

public class TestPlugin extends ApplicationInitializer {
	
	public interface GraphicsPluginFactory extends io.qt.QtObjectInterface{
		public QGraphicsObject create(QGraphicsItem parent);
	}
	
	@QtPluginMetaData(json="{\"Keys\": [ \"InterfacePlugin\" ] }")
	static class GraphicsPlugin extends QObject implements GraphicsPluginFactory{
		@Override
		public QGraphicsObject create(QGraphicsItem parent) {
			return new QGraphicsWidget(parent);
		}
	}
	
	@BeforeClass
    public static void testInitialize() throws Exception {
        ApplicationInitializer.testInitializeWithWidgets();
    }
	
	@Test
    public void testGenericPlugin() {
		Assume.assumeFalse("not android", QOperatingSystemVersion.current().isAnyOfType(OSType.Android));
		Assume.assumeTrue("QtNetwork unavailable", QtUtilities.initializePackage("io.qt.network"));
		QFactoryLoader loader = new QFactoryLoader(QGenericPlugin.class, "/generic");
		QObject plugin = loader.loadPlugin(QGenericPlugin::create, "TuioTouch", "");
		Assert.assertTrue(plugin!=null);
		Assert.assertEquals("QTuioHandler", plugin.metaObject().className());
        plugin.dispose();
        loader.dispose();
	}
    
    @Test
    public void testStylePlugin() {
    	Assume.assumeTrue("windows or macos", QOperatingSystemVersion.current().isAnyOfType(OSType.Windows, OSType.MacOS));
		QFactoryLoader loader = new QFactoryLoader(QStylePlugin.class, "/styles");
		Assert.assertFalse(loader.keyMap().isEmpty());
        Assert.assertFalse(loader.keyMap().get(0).isEmpty());
		QStyle plugin = loader.loadPlugin(QStylePlugin::create, loader.keyMap().get(0).get(0), loader.keyMap().get(0).get(0));
		Assert.assertTrue(plugin!=null);
		//Assert.assertEquals("QTuioHandler", plugin.metaObject().className());
	}
    
    @Test
    public void testImageFormatsPlugin() {
		QFactoryLoader loader = new QFactoryLoader(QImageIOPlugin.class, "/imageformats");
		System.out.println(loader.keyMap());
		QImageIOHandler plugin = loader.loadPlugin((QMetaObject.Method2<QImageIOPlugin,QIODevice,QImageIOHandler>)QImageIOPlugin::create, "jpeg", null);
		Assert.assertTrue(plugin!=null);
		//Assert.assertEquals("QTuioHandler", plugin.metaObject().className());
	}
	
	//@Test
    public void testIconEnginePlugin() {
		QFactoryLoader loader = new QFactoryLoader(QIconEnginePlugin.class, "/iconengines");
		System.out.println(loader.keyMap());
		QIconEngine plugin = loader.loadPlugin((QMetaObject.Method2<QIconEnginePlugin,String,QIconEngine>)QIconEnginePlugin::create, "svg", ":io/qt/autotests/svg-cards.svg");
		Assert.assertTrue(plugin!=null);
	}
	
    @Test
    public void testCustomPlugin() {
		QFactoryLoader.registerFactory(GraphicsPluginFactory.class);
		QPluginLoader.qRegisterStaticPluginFunction(GraphicsPlugin.class);
		QFactoryLoader loader = new QFactoryLoader(GraphicsPluginFactory.class, "/static");
		Assert.assertEquals(0, loader.indexOf("InterfacePlugin"));
		QGraphicsItem parent = new QGraphicsWidget();
		QGraphicsObject plugin = loader.loadPlugin(GraphicsPluginFactory::create, "InterfacePlugin", parent);
		Assert.assertTrue("InterfacePlugin not a QGraphicsWidget: "+(plugin==null ? "null" : plugin.getClass()), plugin instanceof QGraphicsWidget);
		plugin = loader.loadPlugin(QGraphicsObject.class, "InterfacePlugin", parent);
		Assert.assertTrue(plugin instanceof QGraphicsWidget);
    }
    
    @Test
    public void testLoadPluginLibrary() {
		Assume.assumeTrue("macos only", QOperatingSystemVersion.current().isAnyOfType(OSType.MacOS));
		for(String path : QCoreApplication.libraryPaths()) {
			QFileInfo file = new QFileInfo(path+"/designer/libcustomwidgets.dylib");
			if(file.exists()) {
				QLibrary library = new QLibrary(file.absoluteFilePath());
				try {
					boolean loaded = library.load();
					Assert.assertTrue(library.errorString(), loaded);
				}finally {
					library.dispose();
				}
			}
		}
	}
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestPlugin.class.getName());
    }
}
