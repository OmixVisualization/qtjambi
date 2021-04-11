package io.qt.autotests;

import java.util.List;

import org.junit.Assert;
import org.junit.Test;

import io.qt.core.QCoreApplication;
import io.qt.core.QFile;
import io.qt.core.QIODevice;
import io.qt.core.QObject;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QPluginLoader;
import io.qt.designer.QDesignerCustomWidgetCollectionInterface;
import io.qt.designer.QDesignerCustomWidgetInterface;
import io.qt.designer.QFormBuilder;
import io.qt.multimedia.widgets.QVideoWidget;
import io.qt.widgets.QWidget;

public class TestDesignerMultimedia extends QApplicationTest {
	
    @Test
    public void testMultimediaWidget() {
    	QWidget widget;
    	{
			QFormBuilder loader = new QFormBuilder();
			for(String path : QCoreApplication.libraryPaths()) {
				loader.addPluginPath(path+"/designer");
			}
	    	QIODevice device = new QFile("classpath:io/qt/autotests/ui/multimediatest.ui");
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
    	if(System.getProperty("io.qt.debug").equalsIgnoreCase("debug")) {
    		if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Windows)) {
    			lib += "d";
    		}else {
    			lib += "_debug";
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
