package io.qt.autotests;

import java.lang.reflect.Method;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtUtilities;
import io.qt.core.*;
import io.qt.designer.*;
import io.qt.quick.*;
import io.qt.quick.widgets.*;
import io.qt.widgets.*;

public class TestDesignerQuick extends ApplicationInitializer {
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		Assume.assumeTrue("is debug or is Qt5 or is Windows", !QLibraryInfo.isDebugBuild() 
				|| QLibraryInfo.version().majorVersion()>5 
				|| !QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Windows));
		QCoreApplication.setAttribute(Qt.ApplicationAttribute.AA_ShareOpenGLContexts);
		Method mtd = QQuickWindow.class.getMethod(QLibraryInfo.version().majorVersion()>5 ? "setGraphicsApi" : "setSceneGraphBackend", QSGRendererInterface.GraphicsApi.class);
		mtd.invoke(null, QSGRendererInterface.GraphicsApi.OpenGLRhi);
		ApplicationInitializer.testInitializeWithWidgets();
		QQuickWidget.staticMetaObject.hashCode();
	}
    
    @Test
    public void testQuickWidgetForm() {
    	QWidget widget;
    	{
			QFormBuilder loader = new QFormBuilder();
			for(String path : QCoreApplication.libraryPaths()) {
				loader.addPluginPath(path+"/designer");
			}
	    	QIODevice device = new QFile("classpath:io/qt/autotests/ui/quicktest.ui");
	    	device.open(QIODevice.OpenModeFlag.ReadOnly);
	    	widget = loader.load(device);
	    	device.close();
    	}
    	System.gc();
    	Assert.assertTrue("QFormBuilder did not load widget", widget != null);
    	Assert.assertEquals(QQuickWidget.staticMetaObject, widget.metaObject());
		if(!(widget instanceof QQuickWidget)) {
			QQuickWidget quickWidget = widget.qt_metacast(QQuickWidget.class);
			Assert.assertTrue(widget.isDisposed());
			Assert.assertTrue(quickWidget != null);
			Assert.assertFalse(quickWidget.isDisposed());
		}
    }
    
    @Test
    public void testQuickWidget() {
    	String lib = "designer/qquickwidget";
    	if(QLibraryInfo.isDebugBuild()) {
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
    		QDesignerCustomWidgetInterface wdg = servicePlugin.qt_metacast(QDesignerCustomWidgetInterface.class);
    		Assert.assertTrue(wdg != null);
    		Assert.assertEquals("QQuickWidget", wdg.name());
    		QWidget widget = wdg.createWidget(null);
    		Assert.assertTrue("QDesignerCustomWidgetInterface did not create widget", widget != null);
    		Assert.assertEquals(QQuickWidget.staticMetaObject, widget.metaObject());
    		if(!(widget instanceof QQuickWidget)) {
    			QQuickWidget quickWidget = widget.qt_metacast(QQuickWidget.class);
    			Assert.assertTrue(widget.isDisposed());
    			Assert.assertTrue(quickWidget != null);
    			Assert.assertFalse(quickWidget.isDisposed());
    			widget = wdg.createWidget(null);
    			Assert.assertTrue(widget instanceof QQuickWidget);
    		}
    	}
    }
}
