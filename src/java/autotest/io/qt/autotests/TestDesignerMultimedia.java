package io.qt.autotests;

import java.lang.reflect.Method;
import java.util.List;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.*;
import io.qt.designer.*;
import io.qt.multimedia.widgets.*;
import io.qt.quick.*;
import io.qt.widgets.*;
import io.qt.internal.QtJambiInternal;

public class TestDesignerMultimedia extends ApplicationInitializer {
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		QCoreApplication.setAttribute(Qt.ApplicationAttribute.AA_ShareOpenGLContexts);
		Method mtd = QQuickWindow.class.getMethod(QLibraryInfo.version().majorVersion()>5 ? "setGraphicsApi" : "setSceneGraphBackend", QSGRendererInterface.GraphicsApi.class);
		mtd.invoke(null, QSGRendererInterface.GraphicsApi.OpenGLRhi);
		ApplicationInitializer.testInitializeWithWidgets();
	}
	
    @Test
    public void testMultimediaWidget() {
    	QWidget widget;
    	{
			QFormBuilder loader = new QFormBuilder();
			for(String path : QCoreApplication.libraryPaths()) {
				loader.addPluginPath(path+"/designer");
			}
	    	QIODevice device = new QFile(":io/qt/autotests/ui/multimediatest.ui");
	    	device.open(QIODevice.OpenModeFlag.ReadOnly);
	    	widget = loader.load(device);
	    	device.close();
    	}
    	System.gc();
    	Assert.assertTrue(widget instanceof QVideoWidget);
    	Assert.assertEquals(QVideoWidget.staticMetaObject, widget.metaObject());
    }
    
    @Test
    public void testMultimediaWidgets() {
    	String lib = "designer/multimediawidgets";
    	if(QtJambiInternal.isDebugBuild()) {
    		if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Windows)) {
    			lib += "d";
    		}
    	}
    	QPluginLoader pluginLoader = new QPluginLoader(lib);
    	if(!pluginLoader.load())
    		Assert.assertTrue(pluginLoader.errorString()+" "+lib, false);
    	else{
    		QObject servicePlugin = pluginLoader.instance();
    		Assert.assertTrue(servicePlugin != null);
    		QDesignerCustomWidgetCollectionInterface collection = servicePlugin.qt_metacast(QDesignerCustomWidgetCollectionInterface.class);
    		Assert.assertTrue(collection != null);
    		List<QDesignerCustomWidgetInterface> customWidgets = collection.customWidgets();
    		Assert.assertEquals(1, customWidgets.size());
    		QDesignerCustomWidgetInterface wdg = customWidgets.get(0);
    		Assert.assertEquals("QVideoWidget", wdg.name());
    		QWidget widget = wdg.createWidget(null);
        	Assert.assertTrue(widget instanceof QVideoWidget);
    		Assert.assertEquals(QVideoWidget.staticMetaObject, widget.metaObject());
    	}
    }
}
