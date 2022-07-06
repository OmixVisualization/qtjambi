package io.qt.autotests;

import java.lang.reflect.Method;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QCoreApplication;
import io.qt.core.QFile;
import io.qt.core.QIODevice;
import io.qt.core.QLibraryInfo;
import io.qt.core.Qt;
import io.qt.help.QHelpSearchQueryWidget;
import io.qt.quick.QQuickWindow;
import io.qt.quick.QSGRendererInterface;
import io.qt.widgets.QWidget;
import io.qt.widgets.tools.QUiLoader;

public class TestUiToolsHelp extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
		QCoreApplication.setAttribute(Qt.ApplicationAttribute.AA_ShareOpenGLContexts);
		Method mtd = QQuickWindow.class.getMethod(QLibraryInfo.version().majorVersion()>5 ? "setGraphicsApi" : "setSceneGraphBackend", QSGRendererInterface.GraphicsApi.class);
		mtd.invoke(null, QSGRendererInterface.GraphicsApi.OpenGLRhi);
    	ApplicationInitializer.testInitializeWithWidgets();
    }
    
    @Test
    public void test()
    {
    	String errorString = null;
    	QWidget widget;
    	{
	    	QUiLoader loader = new QUiLoader();
	    	for(String path : QCoreApplication.libraryPaths()) {
				loader.addPluginPath(path);
			}
	    	QFile device = new QFile(":io/qt/autotests/ui/helptest.ui");
	    	device.open(QIODevice.OpenModeFlag.ReadOnly);
	    	widget = loader.load(device);
	    	errorString = loader.errorString();
	    	device.close();
    	}
    	ApplicationInitializer.runGC();
    	Assert.assertTrue((widget==null ? "widget is null: "+errorString : "widget is "+widget.getClass().getName()), widget instanceof QHelpSearchQueryWidget);
    	Assert.assertEquals(QHelpSearchQueryWidget.staticMetaObject, widget.metaObject());
    }
}
