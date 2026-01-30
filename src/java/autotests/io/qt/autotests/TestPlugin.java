/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import io.qt.*;
import io.qt.autotests.generated.General;
import io.qt.core.*;
import io.qt.core.QOperatingSystemVersion.OSType;
import io.qt.core.internal.*;
import io.qt.gui.*;
import io.qt.widgets.*;

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
		try {
			Assert.assertTrue(plugin!=null);
			Assert.assertEquals("QTuioHandler", plugin.metaObject().className());
		}finally {
	        plugin.dispose();
	        loader.dispose();
		}
	}
    
    @Test
    public void testStylePlugin() {
    	Assume.assumeTrue("windows or macos", QOperatingSystemVersion.current().isAnyOfType(OSType.Windows, OSType.MacOS));
		QFactoryLoader loader = new QFactoryLoader(QStylePlugin.class, "/styles");
		try {
			Assert.assertFalse(loader.keyMap().isEmpty());
	        Assert.assertFalse(loader.keyMap().get(0).isEmpty());
			QStyle plugin = loader.loadPlugin(QStylePlugin::create, loader.keyMap().get(0).get(0), loader.keyMap().get(0).get(0));
			//Assert.assertEquals("QTuioHandler", plugin.metaObject().className());
			try{
				Assert.assertTrue(plugin!=null);
			}finally {
				plugin.dispose();
			}
		}finally {
			loader.dispose();
		}
	}
    
    @Test
    public void testImageFormatsPlugin() {
		QFactoryLoader loader = new QFactoryLoader(QImageIOPlugin.class, "/imageformats");
		QImageIOHandler plugin = loader.loadPlugin((QMetaObject.Method2<QImageIOPlugin,QIODevice,QImageIOHandler>)QImageIOPlugin::create, "jpeg", null);
		try {
//			System.out.println(loader.keyMap());
			Assert.assertTrue(plugin!=null);
			//Assert.assertEquals("QTuioHandler", plugin.metaObject().className());
		}finally {
			plugin.dispose();
			loader.dispose();
		}
	}
	
	//@Test
    public void testIconEnginePlugin() {
		QFactoryLoader loader = new QFactoryLoader(QIconEnginePlugin.class, "/iconengines");
		QIconEngine plugin = loader.loadPlugin((QMetaObject.Method2<QIconEnginePlugin,String,QIconEngine>)QIconEnginePlugin::create, "svg", ":io/qt/autotests/svg-cards.svg");
		try{
//			System.out.println(loader.keyMap());
			Assert.assertTrue(plugin!=null);
		}finally {
			plugin.dispose();
			loader.dispose();
		}
	}
	
    @Test
    public void testCustomPlugin() {
		QFactoryLoader.registerFactory(GraphicsPluginFactory.class);
		QPluginLoader.qRegisterStaticPluginFunction(GraphicsPlugin.class);
		QFactoryLoader loader = new QFactoryLoader(GraphicsPluginFactory.class, "/static");
		try {
			Assert.assertEquals(0, loader.indexOf("InterfacePlugin"));
			QGraphicsItem parent = new QGraphicsWidget();
			QGraphicsObject plugin = loader.loadPlugin(GraphicsPluginFactory::create, "InterfacePlugin", parent);
			try {
				Assert.assertTrue("InterfacePlugin not a QGraphicsWidget: "+(plugin==null ? "null" : plugin.getClass()), plugin instanceof QGraphicsWidget);
				plugin.dispose();
				plugin = loader.loadPlugin(QGraphicsObject.class, "InterfacePlugin", parent);
				Assert.assertTrue(plugin instanceof QGraphicsWidget);
			}finally {
				plugin.dispose();
				parent.dispose();
			}
		}finally {
			System.out.println("TestPlugin.testCustomPlugin()");
			loader.dispose();
		}
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
    
    @Test
    public void testMacOSUtilsPlugins() {
    	Assume.assumeTrue("macos only", QOperatingSystemVersion.current().isAnyOfType(OSType.MacOS));
    	QFactoryLoader fl = new QFactoryLoader(QGenericPlugin.class);
    	try {
	    	QObject macOSUtils = fl.loadPlugin(QGenericPlugin::create, "NSWindowUtils", "");
			Assert.assertTrue(macOSUtils!=null);
			try {
				Assert.assertTrue(General.internalAccess.isJavaOwnership(macOSUtils));
		    	QMetaMethod isOnActiveSpace = macOSUtils.metaObject().method("isOnActiveSpace", QWindow.class);
				Assert.assertTrue(isOnActiveSpace.isValid());
				QWindow window = new QWindow();
				window.show();
				Object[] result = {null};
				QTimer.singleShot(100, ()->{
					try {
						result[0] = isOnActiveSpace.invoke(macOSUtils, window);
					}catch(Throwable t) {
						result[0] = t;
					}finally {
						QApplication.quit();
					}
				});
				QApplication.exec();
				window.dispose();
				Assert.assertEquals("Window is not on current space", Boolean.TRUE, result[0]);
			}finally {
				macOSUtils.dispose();
			}
		}finally {
			fl.dispose();
		}
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestPlugin.class.getName());
    }
}
