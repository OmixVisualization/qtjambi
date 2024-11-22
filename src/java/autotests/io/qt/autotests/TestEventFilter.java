/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.util.HashSet;
import java.util.Set;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtUtilities;
import io.qt.core.QEvent;
import io.qt.core.QEventLoop;
import io.qt.core.QObject;
import io.qt.core.QRegularExpression;
import io.qt.core.QTimer;
import io.qt.core.Qt;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QWindow;
import io.qt.widgets.QWidget;

public class TestEventFilter extends ApplicationInitializer {
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitializeWithWidgets();
    }
    
    @Test
    public void testEventFilter() {
    	// must not lead to crash
    	QGuiApplication.instance().sessionId();
    	QObject eventFilter = new QObject(QGuiApplication.instance()){
    		@Override
    		public boolean eventFilter(QObject obj, QEvent evt) {
//    			System.out.println("testEventFilter(): eventFilter("+obj+", "+evt+")");
    			return false;
    		}
    	};
    	QGuiApplication.instance().installEventFilter(eventFilter);
    	QEventLoop eventLoop = new QEventLoop();
    	try {
	    	QWidget widget = new QWidget();
	    	widget.show();
	    	QTimer.singleShot(1000, eventLoop, QEventLoop::quit);
	    	eventLoop.exec();
	    	widget.dispose();
    	}finally {
    		QGuiApplication.instance().removeEventFilter(eventFilter);
    	}
    }
    
    @Test
    public void testEqualObjectNameSelectiveEventFilter() {
    	QGuiApplication.instance().sessionId();
    	Set<QObject> filteredObjects = new HashSet<>();
    	QObject eventFilter = QtUtilities.asSelectiveEventFilter(new QObject(QGuiApplication.instance()){
    		@Override
    		public boolean eventFilter(QObject obj, QEvent evt) {
    			filteredObjects.add(obj);
//    			System.out.println("testEqualObjectNameSelectiveEventFilter(): eventFilter("+obj+", "+evt+")");
    			return false;
    		}
    	}, QtUtilities.StringComparison.Equal, "FilteredObject");
    	QGuiApplication.instance().installEventFilter(eventFilter);
    	QEventLoop eventLoop = new QEventLoop();
    	try {
	    	QWidget widget1 = new QWidget();
	    	widget1.setObjectName("FilteredWidget1");
	    	widget1.show();
	    	QWidget widget2 = new QWidget();
	    	widget2.setObjectName("FilteredWidget2");
	    	widget2.show();
	    	QWidget widget = new QWidget();
	    	widget.setObjectName("FilteredObject");
	    	widget.show();
	    	QTimer.singleShot(1000, eventLoop, QEventLoop::quit);
	    	eventLoop.exec();
	    	try {
	    		Assert.assertEquals(1, filteredObjects.size());
	    		Assert.assertTrue(filteredObjects.contains(widget));
	    	}finally {
		    	widget.dispose();
		    	widget1.dispose();
		    	widget2.dispose();
	    	}
    	}finally {
    		QGuiApplication.instance().removeEventFilter(eventFilter);
    	}
    }
    
    @Test
    public void testStartsWithObjectNameSelectiveEventFilter() {
    	QGuiApplication.instance().sessionId();
    	Set<QObject> filteredObjects = new HashSet<>();
    	QObject eventFilter = QtUtilities.asSelectiveEventFilter(new QObject(QGuiApplication.instance()){
    		@Override
    		public boolean eventFilter(QObject obj, QEvent evt) {
    			filteredObjects.add(obj);
//    			System.out.println("testStartsWithObjectNameSelectiveEventFilter(): eventFilter("+obj+", "+evt+")");
    			return false;
    		}
    	}, QtUtilities.StringComparison.StartsWith, Qt.CaseSensitivity.CaseInsensitive, "filtered");
    	QGuiApplication.instance().installEventFilter(eventFilter);
    	QEventLoop eventLoop = new QEventLoop();
    	try {
	    	QWidget widget1 = new QWidget();
	    	widget1.setObjectName("FilteredWidget1");
	    	widget1.show();
	    	QWidget widget2 = new QWidget();
	    	widget2.setObjectName("FilteredWidget2");
	    	widget2.show();
	    	QTimer.singleShot(1000, eventLoop, QEventLoop::quit);
	    	eventLoop.exec();
	    	try {
	    		Assert.assertEquals(4, filteredObjects.size());
	    		Assert.assertTrue(filteredObjects.contains(widget1));
	    		Assert.assertTrue(filteredObjects.contains(widget2));
	    	}finally {
		    	widget1.dispose();
		    	widget2.dispose();
	    	}
    	}finally {
    		QGuiApplication.instance().removeEventFilter(eventFilter);
    	}
    }
    
    @Test
    public void testRegexpObjectNameSelectiveEventFilter() {
    	QGuiApplication.instance().sessionId();
    	Set<QObject> filteredObjects = new HashSet<>();
    	QObject eventFilter = QtUtilities.asSelectiveEventFilter(new QObject(QGuiApplication.instance()){
    		@Override
    		public boolean eventFilter(QObject obj, QEvent evt) {
    			filteredObjects.add(obj);
//    			System.out.println("testStartsWithObjectNameSelectiveEventFilter(): eventFilter("+obj+", "+evt+")");
    			return false;
    		}
    	}, new QRegularExpression("(Widget)[1|2]$"));
    	QGuiApplication.instance().installEventFilter(eventFilter);
    	QEventLoop eventLoop = new QEventLoop();
    	try {
	    	QWidget widget1 = new QWidget();
	    	widget1.setObjectName("FilteredWidget1");
	    	widget1.show();
	    	QWidget widget2 = new QWidget();
	    	widget2.setObjectName("FilteredWidget2");
	    	widget2.show();
	    	QTimer.singleShot(1000, eventLoop, QEventLoop::quit);
	    	eventLoop.exec();
	    	try {
	    		Assert.assertEquals(2, filteredObjects.size());
	    		Assert.assertTrue(filteredObjects.contains(widget1));
	    		Assert.assertTrue(filteredObjects.contains(widget2));
	    	}finally {
		    	widget1.dispose();
		    	widget2.dispose();
	    	}
    	}finally {
    		QGuiApplication.instance().removeEventFilter(eventFilter);
    	}
    }
    
    @Test
    public void testMetaObjectSelectiveEventFilter() {
    	QGuiApplication.instance().sessionId();
    	Set<QObject> filteredObjects = new HashSet<>();
    	QObject eventFilter = QtUtilities.asSelectiveEventFilter(new QObject(QGuiApplication.instance()){
    		@Override
    		public boolean eventFilter(QObject obj, QEvent evt) {
    			filteredObjects.add(obj);
//    			System.out.println("testMetaObjectSelectiveEventFilter(): eventFilter("+obj+", "+evt+")");
    			return false;
    		}
    	}, QWindow.staticMetaObject);
    	QGuiApplication.instance().installEventFilter(eventFilter);
    	QEventLoop eventLoop = new QEventLoop();
    	try {
	    	QWidget widget1 = new QWidget();
	    	widget1.show();
	    	QWidget widget2 = new QWidget();
	    	widget2.show();
	    	QTimer.singleShot(1000, eventLoop, QEventLoop::quit);
	    	eventLoop.exec();
	    	try {
	    		Assert.assertEquals(2, filteredObjects.size());
	    		Assert.assertTrue(filteredObjects.contains(widget1.windowHandle()));
	    		Assert.assertTrue(filteredObjects.contains(widget2.windowHandle()));
	    	}finally {
		    	widget1.dispose();
		    	widget2.dispose();
	    	}
    	}finally {
    		QGuiApplication.instance().removeEventFilter(eventFilter);
    	}
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestEventFilter.class.getName());
    }
}
