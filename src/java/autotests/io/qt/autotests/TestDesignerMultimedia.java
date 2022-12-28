package io.qt.autotests;

import java.lang.reflect.Method;
import java.util.List;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QCoreApplication;
import io.qt.core.QFile;
import io.qt.core.QIODevice;
import io.qt.core.QLibraryInfo;
import io.qt.core.QObject;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QPluginLoader;
import io.qt.core.Qt;
import io.qt.designer.QDesignerCustomWidgetCollectionInterface;
import io.qt.designer.QDesignerCustomWidgetInterface;
import io.qt.designer.QFormBuilder;
import io.qt.internal.TestUtility;
import io.qt.multimedia.widgets.QVideoWidget;
import io.qt.quick.QQuickWindow;
import io.qt.quick.QSGRendererInterface;
import io.qt.widgets.QWidget;

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
    	String errorString = null;
    	QWidget widget;
    	{
			QFormBuilder loader = new QFormBuilder();
			for(String path : QCoreApplication.libraryPaths()) {
				loader.addPluginPath(path);
				loader.addPluginPath(path+"/designer");
			}
	    	QIODevice device = new QFile(":io/qt/autotests/ui/multimediatest.ui");
	    	device.open(QIODevice.OpenModeFlag.ReadOnly);
	    	widget = loader.load(device);
	    	errorString = loader.errorString();
	    	device.close();
    	}
    	ApplicationInitializer.runGC();
    	Assert.assertTrue(errorString, widget instanceof QVideoWidget);
    	Assert.assertEquals(QVideoWidget.staticMetaObject, widget.metaObject());
    }
    
    @Test
    public void testMultimediaWidgets() {
    	String lib = "designer/multimediawidgets";
    	if(TestUtility.isDebugBuild()) {
    		if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Windows)) {
    			lib += "d";
    		}
    	}
    	if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android))
    		lib = "plugins/"+lib;
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
