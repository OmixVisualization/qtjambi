/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
package io.qt.autotests;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QNativePointer;
import io.qt.activex.QAxObject;
import io.qt.activex.QAxScriptEngine;
import io.qt.activex.QAxSelect;
import io.qt.activex.QAxWidget;
import io.qt.core.QCoreApplication;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaType;
import io.qt.core.QTimer;

public class TestActiveX extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
	private static class AxWidgetSubclass1 extends QAxWidget{
		
	}
	
	private static class AxWidgetSubclass2 extends QAxWidget{
		@SuppressWarnings("unused")
		public final Signal0 testSignal = new Signal0();
	}
	
	@Test
    public void testSubclassing() {
		Assert.assertEquals(QMetaObject.forType(QAxWidget.class), QAxWidget.staticMetaObject);
		Assert.assertEquals(QMetaObject.forType(QAxObject.class), QAxObject.staticMetaObject);
		Assert.assertEquals(QMetaObject.forType(QAxScriptEngine.class), QAxScriptEngine.staticMetaObject);
		Assert.assertEquals(QMetaObject.forType(AxWidgetSubclass1.class), QAxWidget.staticMetaObject);
		new AxWidgetSubclass1();
		try {
			QMetaObject.forType(AxWidgetSubclass2.class);
			Assert.fail("UnsupportedOperationException expected to be thrown");
		}catch(UnsupportedOperationException e) {}
		try {
			new AxWidgetSubclass2();
			Assert.fail("UnsupportedOperationException expected to be thrown");
		}catch(UnsupportedOperationException e) {}
	}
	
	@Test
    public void testSelect() {
		QAxSelect select = new QAxSelect();
		QTimer.singleShot(100, select::reject);
		select.exec();
	}
	
    @Test
    public void test() {
    	QAxWidget widget = new QAxWidget();
    	widget.setControl("{6bf52a52-394a-11d3-b153-00c04f79faa6}");
    	QMetaMethod msignal = widget.metaObject().method("PlaylistChange(IDispatch*,WMPPlaylistChangeEventType)");
    	Assert.assertTrue(msignal!=null);
    	QMetaObject.AbstractSignal signal = msignal.toSignal(widget);
    	Assert.assertTrue(signal instanceof QMetaObject.AbstractPrivateSignal2);
    	QMetaType.GenericObject[] playlistChangeArgs1 = {null};
    	QNativePointer[] playlistChangeArgs2 = {null};
    	((QMetaObject.AbstractPrivateSignal2<?,?>)signal).connect((a,b)->{
    		playlistChangeArgs1[0] = (QMetaType.GenericObject)a;
    		playlistChangeArgs2[0] = (QNativePointer)b;
        	Assert.assertFalse(playlistChangeArgs1[0].isDisposed());
        	Assert.assertFalse(playlistChangeArgs2[0].isNull());
    	});
    	msignal = widget.metaObject().method("PlayStateChange(int)");
    	Assert.assertTrue(msignal!=null);
    	signal = msignal.toSignal(widget);
    	Assert.assertTrue(signal instanceof QMetaObject.AbstractPrivateSignal1);
    	((QMetaObject.AbstractPrivateSignal1<?>)signal).connect((a)->{
    		Assert.assertTrue(a instanceof Integer);
		});
    	msignal = widget.metaObject().method("CurrentItemChange(IDispatch*)");
    	Assert.assertTrue(msignal!=null);
    	signal = msignal.toSignal(widget);
    	Assert.assertTrue(signal instanceof QMetaObject.AbstractPrivateSignal1);
    	QMetaType.GenericObject[] currentItemChangeArgs1 = {null};
    	((QMetaObject.AbstractPrivateSignal1<?>)signal).connect((a)->{
    		currentItemChangeArgs1[0] = (QMetaType.GenericObject)a;
        	Assert.assertFalse(currentItemChangeArgs1[0].isDisposed());
		});
    	QMetaMethod mtd = widget.metaObject().method("SetURL", String.class);
    	mtd.invoke(widget, "https://cdn2.hubspot.net/hubfs/149513/_website/QtV2/herovideo-0.4-smaller_1_shorter.mp4");
    	widget.show();
    	QTimer.singleShot(1500, QCoreApplication::quit);
    	QCoreApplication.exec();
    	if(playlistChangeArgs1[0]!=null)
    		Assert.assertTrue(playlistChangeArgs1[0].isDisposed());
    	if(playlistChangeArgs2[0]!=null)
    		Assert.assertTrue(playlistChangeArgs2[0].isNull());
    	if(currentItemChangeArgs1[0]!=null)
    		Assert.assertTrue(currentItemChangeArgs1[0].isDisposed());
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestActiveX.class.getName());
    }
}
