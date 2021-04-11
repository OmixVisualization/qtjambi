package io.qt.autotests;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QCoreApplication;
import io.qt.core.QFile;
import io.qt.core.QIODevice;
import io.qt.core.QLibraryInfo;
import io.qt.core.QObject;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QPluginLoader;
import io.qt.designer.QDesignerCustomWidgetInterface;
import io.qt.designer.QFormBuilder;
import io.qt.quick.widgets.QQuickWidget;
import io.qt.widgets.QWidget;

public class TestDesignerQuick extends QApplicationTest {
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		QApplicationTest.testInitialize();
		Assume.assumeTrue(!QLibraryInfo.isDebugBuild() 
				|| QLibraryInfo.version().majorVersion()>5 
				|| !QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Windows));
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
    	Assert.assertTrue(widget != null);
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
    		QDesignerCustomWidgetInterface wdg = servicePlugin.qt_metacast(QDesignerCustomWidgetInterface.class);
    		Assert.assertTrue(wdg != null);
    		Assert.assertEquals("QQuickWidget", wdg.name());
    		QWidget widget = wdg.createWidget(null);
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
