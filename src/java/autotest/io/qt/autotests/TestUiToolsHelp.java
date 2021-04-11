package io.qt.autotests;

import org.junit.Assert;
import org.junit.Test;

import io.qt.core.QFile;
import io.qt.core.QIODevice;
import io.qt.help.QHelpSearchQueryWidget;
import io.qt.widgets.QWidget;
import io.qt.widgets.tools.QUiLoader;

public class TestUiToolsHelp extends QApplicationTest {
    
    @Test
    public void test()
    {
    	QWidget widget;
    	{
	    	QUiLoader loader = new QUiLoader();
	    	QFile device = new QFile("classpath:io/qt/autotests/ui/helptest.ui");
	    	device.open(QIODevice.OpenModeFlag.ReadOnly);
	    	widget = loader.load(device);
	    	device.close();
    	}
    	System.gc();
    	Assert.assertTrue(widget instanceof QHelpSearchQueryWidget);
    	Assert.assertEquals(QHelpSearchQueryWidget.staticMetaObject, widget.metaObject());
    }
}
