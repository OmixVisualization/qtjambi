package io.qt.autotests;

import java.lang.reflect.Method;
import java.util.List;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.*;
import io.qt.designer.*;
import io.qt.help.*;
import io.qt.quick.*;
import io.qt.widgets.*;
import io.qt.internal.QtJambiInternal;

public class TestDesignerHelp extends ApplicationInitializer {
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		QCoreApplication.setAttribute(Qt.ApplicationAttribute.AA_ShareOpenGLContexts);
		Method mtd = QQuickWindow.class.getMethod(QLibraryInfo.version().majorVersion()>5 ? "setGraphicsApi" : "setSceneGraphBackend", QSGRendererInterface.GraphicsApi.class);
		mtd.invoke(null, QSGRendererInterface.GraphicsApi.OpenGLRhi);
		ApplicationInitializer.testInitializeWithWidgets();
	}
	
    @Test
    public void testHelpWidget() {
    	QWidget widget;
    	{
			QFormBuilder loader = new QFormBuilder();
			for(String path : QCoreApplication.libraryPaths()) {
				loader.addPluginPath(path+"/designer");
			}
	    	QIODevice device = new QFile(":io/qt/autotests/ui/helptest.ui");
	    	device.open(QIODevice.OpenModeFlag.ReadOnly);
	    	widget = loader.load(device);
	    	device.close();
    	}
    	System.gc();
    	Assert.assertTrue(widget instanceof QHelpSearchQueryWidget);
    	Assert.assertEquals(QHelpSearchQueryWidget.staticMetaObject, widget.metaObject());
    }
    
    @Test
    public void testHelpWidgets() {
    	String lib = "designer/helpwidgets";
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
    		Assert.assertEquals(2, customWidgets.size());
    		QDesignerCustomWidgetInterface wdg = customWidgets.get(0);
    		Assert.assertEquals("QHelpSearchQueryWidget", wdg.name());
    		QWidget widget = wdg.createWidget(null);
    		Assert.assertTrue(widget instanceof QHelpSearchQueryWidget);
    		Assert.assertEquals(QHelpSearchQueryWidget.staticMetaObject, widget.metaObject());
    	}
    }
}
