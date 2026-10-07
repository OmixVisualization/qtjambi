/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QNoImplementationException;
import io.qt.StrictNonNull;
import io.qt.core.QAbstractNativeEventFilter;
import io.qt.core.QCoreApplication;
import io.qt.core.QNativeEvent;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QTimer;
import io.qt.widgets.QColorDialog;
import io.qt.widgets.QDialog;
import io.qt.widgets.QWidget;

public class TestNativeEventFilter extends ApplicationInitializer {
	
	private static class CountingFilter extends QAbstractNativeEventFilter {
		private final AtomicInteger calls;

		private CountingFilter(AtomicInteger calls) {
			this.calls = calls;
		}

		@Override
		public boolean nativeEventFilter(@StrictNonNull QNativeEvent event) {
//			System.out.println("nativeEventFilter("+event.eventType()+")");
			calls.incrementAndGet();
			return false;
		}
	}

	@BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitializeWithWidgets();
    }
    
    @Test
    public void testNativeEventFilter() {
    	Assume.assumeFalse(QOperatingSystemVersion.currentType().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	AtomicInteger calls = new AtomicInteger();
    	QAbstractNativeEventFilter filter = new CountingFilter(calls);
		QCoreApplication.instance().installNativeEventFilter(filter);
		try {
			QWidget widget = new QWidget();
			widget.setVisible(true);
			QTimer.singleShot(100, QCoreApplication::quit);
			QCoreApplication.exec();
			Assert.assertTrue(calls.get()>0);
		}finally {
			QCoreApplication.instance().removeNativeEventFilter(filter);
		}
    }
    
    @Test
    public void testMacOSNativeEventFilter() {
    	AtomicInteger calls = new AtomicInteger();
    	QAbstractNativeEventFilter filter = QAbstractNativeEventFilter.asSelectiveMacNSEventFilter(new CountingFilter(calls){
			@Override
			public boolean nativeEventFilter(@StrictNonNull QNativeEvent event) {
				try {
					Object message = event.message();
					if(message instanceof QNativeEvent.NSEvent) {
						QNativeEvent.NSEvent nse = (QNativeEvent.NSEvent)message;
						System.out.println(nse.type());
					}
				} catch (Throwable e) {
					e.printStackTrace();
				}
				return super.nativeEventFilter(event);
			}
    	});
    	if(QOperatingSystemVersion.currentType().isAnyOfType(QOperatingSystemVersion.OSType.MacOS)) {
    		Assert.assertTrue(filter!=null);
    		QCoreApplication.instance().installNativeEventFilter(filter);
    		try {
    			QWidget widget = new QWidget();
    			widget.setVisible(true);
    			QTimer.singleShot(100, QCoreApplication::quit);
    			QCoreApplication.exec();
    			Assert.assertTrue(calls.get()>0);
    		}finally {
    			QCoreApplication.instance().removeNativeEventFilter(filter);
    		}
    	}else {
    		Assert.assertTrue(filter==null);
    		try {
				QNativeEvent.NSEvent.doubleClickInterval();
				Assert.fail("QNoImplementationException expected to be thrown");
			} catch (QNoImplementationException e) {
			}
    	}
    }
    
    @Test
    public void testWindowsNativeEventFilter() {
    	AtomicReference<Long> window = new AtomicReference<>();
    	AtomicInteger created = new AtomicInteger();
    	QAbstractNativeEventFilter createdFilter = QAbstractNativeEventFilter.asSelectiveWindowsMSGEventFilter(new CountingFilter(created){
			@Override
			public boolean nativeEventFilter(@StrictNonNull QNativeEvent event) {
				Object message = event.message();
				if(message instanceof QNativeEvent.MSG) {
					QNativeEvent.MSG msg = (QNativeEvent.MSG)message;
					window.set(msg.hwnd());
				}
				return super.nativeEventFilter(event);
			}
    	}, 1);
    	if(QOperatingSystemVersion.currentType().isAnyOfType(QOperatingSystemVersion.OSType.Windows)) {
    		Assert.assertTrue(createdFilter!=null);
    		QCoreApplication.instance().installNativeEventFilter(createdFilter);
    		try {
    			QColorDialog dialog = new QColorDialog();
    			QTimer.singleShot(50, dialog, QDialog::reject);
    			dialog.exec();
    			Assert.assertTrue(created.get()==1);
    			Assert.assertEquals(dialog.winId(), (long)window.get());
    		}finally {
    			QCoreApplication.instance().removeNativeEventFilter(createdFilter);
    		}
    	}else {
    		Assert.assertTrue(createdFilter==null);
    	}
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestNativeEventFilter.class.getName());
    }
}
