package io.qt.autotests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.lang.reflect.Method;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.*;
import io.qt.gui.*;
import io.qt.quick.*;
import io.qt.widgets.*;
import io.qt.widgets.tools.*;
import io.qt.internal.QtJambiInternal;

public class TestUiTools extends ApplicationInitializer {
	
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
	    	QFile device = new QFile(":io/qt/autotests/ui/dialogtest.ui");
	    	device.open(QIODevice.OpenModeFlag.ReadOnly);
	    	widget = loader.load(device);
	    	errorString = loader.errorString();
	    	device.close();
    	}
    	ApplicationInitializer.runGC();
    	assertTrue((widget==null ? "widget is null: "+errorString : "widget is "+widget.getClass().getName()), widget instanceof QDialog);
    	assertEquals(null, widget.parent());
    	QDialog dialog = (QDialog)widget;
    	QList<QObject> chidren = dialog.children();
    	assertEquals(4, chidren.size());
    	assertTrue(chidren.get(1) instanceof QGridLayout);
    	assertTrue(chidren.get(2) instanceof QToolBox);
    	assertTrue(chidren.get(3) instanceof QDialogButtonBox);
    	assertEquals("QFormInternal::TranslationWatcher", chidren.get(0).metaObject().className());
    	assertEquals("gridLayout", chidren.get(1).objectName());
    	assertEquals("toolBox", chidren.get(2).objectName());
    	assertEquals("buttonBox", chidren.get(3).objectName());
    	QTimer.singleShot(1000, dialog::accept);
    	dialog.exec();
    }
    
    @Test
    public void testCustomWidgetPlugin()
    {
    	String errorString = null;
    	QWidget widget;
    	{
	    	QUiLoader loader = new QUiLoader();
			String version = QtJambiInternal.majorVersion()+"."+QtJambiInternal.minorVersion()+"."+QtJambiInternal.qtjambiPatchVersion();
	    	if(QtJambiInternal.isDebugBuild()) {
	    		loader.addPluginPath(QDir.fromNativeSeparators(System.getProperty("user.dir", ""))+"/"+version+"/build/tests/debug/plugins/designer");
			}else {
				loader.addPluginPath(QDir.fromNativeSeparators(System.getProperty("user.dir", ""))+"/"+version+"/build/tests/release/plugins/designer");
			}
	    	for(String path : QCoreApplication.libraryPaths()) {
				loader.addPluginPath(path);
			}
	    	QFile device = new QFile(":io/qt/autotests/ui/customwidgettest.ui");
	    	Assert.assertTrue(device.open(QIODevice.OpenModeFlag.ReadOnly));
	    	widget = loader.load(device);
	    	errorString = loader.errorString();
	    	device.close();
    	}
    	ApplicationInitializer.runGC();
    	Assert.assertTrue((widget==null ? "widget is null: "+errorString : "widget is "+widget.getClass().getName()), widget instanceof QWidget);
    	Assert.assertEquals("io::qt::designer::customwidgets::CustomWidget", widget.metaObject().className());
    	Assert.assertEquals(new QRect(3, 6, 12, 24), widget.property("customRect"));
    	Assert.assertEquals("This is custom text", widget.property("customText"));
    	Assert.assertEquals(QKeySequence.fromString("CTRL+S"), widget.property("customKeySequence"));
    	Assert.assertEquals(Qt.ArrowType.RightArrow, widget.property("customArrowType"));
    }
}
