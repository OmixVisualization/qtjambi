/****************************************************************************
**
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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QMetaObject;
import io.qt.core.QObject;
import io.qt.core.QThread;
import io.qt.core.QTimer;
import io.qt.core.Qt;
import io.qt.widgets.QDialog;

public class TestExceptions2 extends ApplicationInitializer {
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitializeWithWidgets();
    }
    
    @Test
    public void testExceptionThroughSignalThreaded() {
    	Throwable[] throwable = {null, null};
    	QObject sender = new QObject();
    	boolean[] threadStarted = {false};
    	boolean[] threadFinished = {false};
    	QThread senderThread = QThread.create(()->{
    		threadStarted[0] = true;
    		try {
    			sender.setObjectName("TEST");
    		}catch(Throwable t) {
    			throwable[0] = t;
    		}finally {
    			sender.dispose();
    			threadFinished[0] = true;
    		}
    	});
    	sender.moveToThread(senderThread);
    	QDialog receiver = new QDialog(){
    		@SuppressWarnings("unused")
			public void test(String val) {
    			RuntimeException e;
    			throwable[1] = e = new RuntimeException("Slot Exception");
    			QTimer.singleShot(1000, this::accept);
    			throw e;
    		}
    		@SuppressWarnings("unused")
			public void startThread() {
    			senderThread.start();
    		}
    	};
    	try {
	    	QMetaObject.Connection connection = sender.objectNameChanged.connect(receiver, "test(String)");
	    	assertTrue("Connection not initialized.", connection.isConnected());
	    	QMetaObject.invokeMethod(receiver, "startThread()", Qt.ConnectionType.QueuedConnection);
    		assertFalse("Thread finished", threadFinished[0]);
    		int code = 0;
	    	try {
	    		code = receiver.exec();
			} catch (Throwable e) {
				e.printStackTrace();
		    	Assert.fail("An exception has been unexpectedly transferred to QEventLoop::exec().");
			}
	    	assertTrue("Thread not started", threadStarted[0]);
	    	assertTrue("Thread not finished", threadFinished[0]);
	    	assertEquals("An unexpected exception has been thrown in sender's thread.", null, throwable[0]);
	    	assertTrue("An exception was expected to be thrown in slot.", throwable[1]!=null);
	    	assertEquals("Dialog exit with unexpected result.", QDialog.DialogCode.Accepted.value(), code);
    	}finally {
    		receiver.dispose();
    	}
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestExceptions2.class.getName());
    }
}
