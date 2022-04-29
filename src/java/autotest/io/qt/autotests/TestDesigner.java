package io.qt.autotests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QFile;
import io.qt.core.QIODevice;
import io.qt.core.QList;
import io.qt.core.QObject;
import io.qt.core.QTimer;
import io.qt.designer.QFormBuilder;
import io.qt.widgets.QDialog;
import io.qt.widgets.QDialogButtonBox;
import io.qt.widgets.QGridLayout;
import io.qt.widgets.QToolBox;
import io.qt.widgets.QWidget;

public class TestDesigner extends ApplicationInitializer {
    
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
    @Test
    public void testFormBuilder()
    {
    	QWidget widget;
    	{
    		QFormBuilder loader = new QFormBuilder();
	    	QFile device = new QFile(":io/qt/autotests/ui/dialogtest.ui");
	    	device.open(QIODevice.OpenModeFlag.ReadOnly);
	    	widget = loader.load(device);
	    	device.close();
    	}
    	System.gc();
    	Assert.assertTrue(widget != null);
    	assertEquals(null, widget.parent());
    	assertTrue(widget instanceof QDialog);
    	QDialog dialog = (QDialog)widget;
    	QList<QObject> chidren = dialog.children();
    	assertEquals(3, chidren.size());
    	assertTrue(chidren.get(0) instanceof QGridLayout);
    	assertTrue(chidren.get(1) instanceof QToolBox);
    	assertTrue(chidren.get(2) instanceof QDialogButtonBox);
    	assertEquals("gridLayout", chidren.get(0).objectName());
    	assertEquals("toolBox", chidren.get(1).objectName());
    	assertEquals("buttonBox", chidren.get(2).objectName());
    	QTimer.singleShot(1000, dialog::accept);
    	dialog.exec();
    }
}
