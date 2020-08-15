/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.autotests.generated.MessageHandler;
import io.qt.core.QCoreApplication;
import io.qt.core.QMessageHandler;
import io.qt.core.QThread;
import io.qt.widgets.QApplication;

public class TestQMessageHandler extends QMessageHandler {
	static {
		io.qt.QtUtilities.initializePackage("io.qt.autotests.generated");
	}
	
    @Override
    public void debug(String message) {
        lastDebug = message;
    }

    @Override
    public void warning(String message) {
        lastWarning = message;
    }

    @Override
    public void critical(String message) {
        lastCritical = message;
    }

    @Override
    public void fatal(String message) {
        lastFatal = message;
    }

    // We don't use QApplicationTest since that will by default initialize QMessageHandler
    @BeforeClass
    public static void testInitialize() throws Exception {
        Utils.println(2, "TestQMessageHandler.testInitialize(): begin");
        if(QCoreApplication.instance()==null) {
			Utils.println(2, "QApplicationTest.testInitialize(): begin");
		    io.qt.QtResources.addSearchPath(".");
		    QCoreApplication.setApplicationName("QtJambiUnitTest");
		    QApplication.initialize(new String[]{});
		    Utils.println(2, "QApplicationTest.testInitialize(): done");
	        QThread.currentThread().setObjectName("main");
		}
        Utils.println(2, "TestQMessageHandler.testInitialize(): done");
    }

    @AfterClass
    public static void testDispose() throws Exception {
    	QApplicationTest.testDispose();
    }

    @Test
    public void testOneHandler() {
        try {
            QMessageHandler.installMessageHandler(this);

            MessageHandler.sendDebug("debug sent");
            assertEquals("debug sent", lastDebug);

            MessageHandler.sendWarning("warning sent");
            assertEquals("warning sent", lastWarning);

            MessageHandler.sendCritical("critical sent");
            assertEquals("critical sent", lastCritical);

            // Want to send fatal, but that will shut down app...
            //MessageHandler.sendFatal("fatal sent");
            //assertEquals(lastFatal, "fatal sent");
        } finally {
            QMessageHandler.removeMessageHandler(this);
        }
    }

    @Test
    public void testMultipleHandlers() {
        TestQMessageHandler handlerOne = null;
        TestQMessageHandler handlerTwo = null;
        try {
            QMessageHandler.installMessageHandler(this);
            handlerOne = new TestQMessageHandler();
            QMessageHandler.installMessageHandler(handlerOne);
            handlerTwo = new TestQMessageHandler();
            QMessageHandler.installMessageHandler(handlerTwo);

            for(int i = 0; i < 5; i++) {
                int j = i;
                if(j == 4)
                    j = 3;	// No handlers so nothing got updated

                MessageHandler.sendDebug("debug sent: " + j);
                assertEquals("debug sent: " + j, lastDebug);

                MessageHandler.sendWarning("warning sent: " + j);
                assertEquals("warning sent: " + j, lastWarning);

                MessageHandler.sendCritical("critical sent: " + j);
                assertEquals("critical sent: " + j, lastCritical);

                // Want to send fatal, but that will shut down app...
                //MessageHandler.sendFatal("fatal sent: " + j);
                //assertEquals(lastFatal, "fatal sent: " + j);

                if(i == 0) {
                    QMessageHandler.removeMessageHandler(handlerOne);
                } else if(i == 1) {
                    QMessageHandler.removeMessageHandler(handlerTwo);
                } else if(i == 2) {
                    QMessageHandler.installMessageHandler(handlerOne);
                    QMessageHandler.installMessageHandler(handlerTwo);
                } else if(i == 3) {
                    QMessageHandler.removeMessageHandler(this);
                } else if(i == 4) {
                    QMessageHandler.removeMessageHandler(handlerOne);
                    QMessageHandler.removeMessageHandler(handlerTwo);
                }
            }
        } finally {
            // Ensure they are removed
           QMessageHandler.removeMessageHandler(this);
           if(handlerOne != null)
               QMessageHandler.removeMessageHandler(handlerOne);
           if(handlerTwo != null)
               QMessageHandler.removeMessageHandler(handlerTwo);
        }
    }

    private String lastDebug;
    private String lastWarning;
    private String lastCritical;
    @SuppressWarnings("unused")
	private String lastFatal;

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQMessageHandler.class.getName());
    }
}
