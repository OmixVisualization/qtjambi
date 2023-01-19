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

import static io.qt.core.QLogging.qCCritical;
import static io.qt.core.QLogging.qCDebug;
import static io.qt.core.QLogging.qCWarning;
import static io.qt.core.QLogging.qCritical;
import static io.qt.core.QLogging.qDebug;
import static io.qt.core.QLogging.qInstallExceptionMessageHandler;
import static io.qt.core.QLogging.qInstallMessageHandler;
import static io.qt.core.QLogging.qWarning;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import java.util.EnumMap;
import java.util.Map;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.autotests.generated.General;
import io.qt.autotests.generated.MessageHandler;
import io.qt.core.QCoreApplication;
import io.qt.core.QLibraryInfo;
import io.qt.core.QLoggingCategory;
import io.qt.core.QResource;
import io.qt.core.QThread;
import io.qt.core.QtMessageHandler;
import io.qt.core.QtMsgType;
import io.qt.widgets.QApplication;

public class TestQMessageHandler extends UnitTestInitializer {
	
    // We don't use ApplicationInitializer since that will by default initialize QMessageHandler
    @BeforeClass
    public static void testInitialize() throws Exception {
        java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "TestQMessageHandler.testInitialize(): begin");
        if(QCoreApplication.instance()==null) {
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "ApplicationInitializer.testInitialize(): begin");
		    QResource.addClassPath(".");
		    QCoreApplication.setApplicationName("QtJambiUnitTest");
		    QApplication.initialize(new String[]{});
		    java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "ApplicationInitializer.testInitialize(): done");
	        QThread.currentThread().setObjectName("main");
		}
        java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "TestQMessageHandler.testInitialize(): done");
    }

    @AfterClass
    public static void testDispose() throws Exception {
    	ApplicationInitializer.testDispose();
    }
    
    @Test
    public void testInstallHandler() {
    	QtMessageHandler handler = (messageType, context, message)->{
    		lastMessage.put(messageType, message);
    		lastFile.put(messageType, context.file());
    		lastFunction.put(messageType, context.function());
    	};
        try {
        	QtMessageHandler oldHandler = qInstallMessageHandler(handler);
        	assertTrue(oldHandler!=handler);
        	assertTrue("not java ownership", General.internalAccess.isJavaOwnership(oldHandler));
        	assertTrue("not cpp ownership", General.internalAccess.isCppOwnership(handler));
        	oldHandler = qInstallMessageHandler(null);
        	assertEquals(oldHandler, handler);
        	assertTrue("not java ownership", General.internalAccess.isJavaOwnership(handler));
        	oldHandler = qInstallMessageHandler(handler, QtMsgType.QtDebugMsg);
        	assertTrue("not java ownership", General.internalAccess.isJavaOwnership(handler));
            MessageHandler.sendDebug("debug sent");
            assertEquals("debug sent", lastMessage.get(QtMsgType.QtDebugMsg));

            MessageHandler.sendWarning("warning sent");
            assertEquals(null, lastMessage.get(QtMsgType.QtWarningMsg));

            MessageHandler.sendCritical("critical sent");
            assertEquals(null, lastMessage.get(QtMsgType.QtCriticalMsg));
            
            oldHandler = qInstallMessageHandler(handler);

            MessageHandler.sendDebug("debug sent");
            assertEquals("debug sent", lastMessage.get(QtMsgType.QtDebugMsg));

            MessageHandler.sendWarning("warning sent");
            assertEquals("warning sent", lastMessage.get(QtMsgType.QtWarningMsg));

            MessageHandler.sendCritical("critical sent");
            assertEquals("critical sent", lastMessage.get(QtMsgType.QtCriticalMsg));
            
            lastMessage.clear();
            lastFile.clear();
            lastFunction.clear();
            
            qDebug("debug sent");
            assertEquals("debug sent", lastMessage.get(QtMsgType.QtDebugMsg));
            if(QLibraryInfo.isDebugBuild()) {
	            assertEquals(TestQMessageHandler.class.getName(), lastFile.get(QtMsgType.QtDebugMsg));
	            assertEquals("testInstallHandler", lastFunction.get(QtMsgType.QtDebugMsg));
            }

            qWarning("warning sent");
            assertEquals("warning sent", lastMessage.get(QtMsgType.QtWarningMsg));
            if(QLibraryInfo.isDebugBuild()) {
	            assertEquals(TestQMessageHandler.class.getName(), lastFile.get(QtMsgType.QtWarningMsg));
	            assertEquals("testInstallHandler", lastFunction.get(QtMsgType.QtWarningMsg));
            }

            qCritical("critical sent");
            assertEquals("critical sent", lastMessage.get(QtMsgType.QtCriticalMsg));
            if(QLibraryInfo.isDebugBuild()) {
	            assertEquals(TestQMessageHandler.class.getName(), lastFile.get(QtMsgType.QtCriticalMsg));
	            assertEquals("testInstallHandler", lastFunction.get(QtMsgType.QtCriticalMsg));
            }
            
            lastMessage.clear();
            lastFile.clear();
            lastFunction.clear();
            
            qDebug().append("debug").append("sent").close();
            assertEquals("debug sent", lastMessage.get(QtMsgType.QtDebugMsg));
            if(QLibraryInfo.isDebugBuild()) {
	            assertEquals(TestQMessageHandler.class.getName(), lastFile.get(QtMsgType.QtDebugMsg));
	            assertEquals("testInstallHandler", lastFunction.get(QtMsgType.QtDebugMsg));
            }

            qWarning().append("warning").append("sent").close();
            assertEquals("warning sent", lastMessage.get(QtMsgType.QtWarningMsg));
            if(QLibraryInfo.isDebugBuild()) {
	            assertEquals(TestQMessageHandler.class.getName(), lastFile.get(QtMsgType.QtWarningMsg));
	            assertEquals("testInstallHandler", lastFunction.get(QtMsgType.QtWarningMsg));
            }

            qCritical().append("critical").append("sent").close();
            assertEquals("critical sent", lastMessage.get(QtMsgType.QtCriticalMsg));
            if(QLibraryInfo.isDebugBuild()) {
	            assertEquals(TestQMessageHandler.class.getName(), lastFile.get(QtMsgType.QtCriticalMsg));
	            assertEquals("testInstallHandler", lastFunction.get(QtMsgType.QtCriticalMsg));
            }
            
            lastMessage.clear();
            lastFile.clear();
            lastFunction.clear();
            
            QLoggingCategory category = new QLoggingCategory("test");
            QLoggingCategory.installFilter(_category->{
            	if("test".equals(_category.categoryName()))
        		_category.setEnabled(QtMsgType.QtWarningMsg, false);
            });
            assertFalse(category.isWarningEnabled());
            
            qCDebug(category, "debug sent");
            assertEquals("debug sent", lastMessage.get(QtMsgType.QtDebugMsg));
            if(QLibraryInfo.isDebugBuild()) {
	            assertEquals(TestQMessageHandler.class.getName(), lastFile.get(QtMsgType.QtDebugMsg));
	            assertEquals("testInstallHandler", lastFunction.get(QtMsgType.QtDebugMsg));
            }

            qCWarning(category, "warning sent");
            assertEquals(null, lastMessage.get(QtMsgType.QtWarningMsg));
            if(QLibraryInfo.isDebugBuild()) {
	            assertEquals(null, lastFile.get(QtMsgType.QtWarningMsg));
	            assertEquals(null, lastFunction.get(QtMsgType.QtWarningMsg));
            }

            qCCritical(category, "critical sent");
            assertEquals("critical sent", lastMessage.get(QtMsgType.QtCriticalMsg));
            if(QLibraryInfo.isDebugBuild()) {
	            assertEquals(TestQMessageHandler.class.getName(), lastFile.get(QtMsgType.QtCriticalMsg));
	            assertEquals("testInstallHandler", lastFunction.get(QtMsgType.QtCriticalMsg));
            }
            
            lastMessage.clear();
            lastFile.clear();
            lastFunction.clear();
            
            qCDebug(category).append("debug").append("sent").close();
            assertEquals("debug sent", lastMessage.get(QtMsgType.QtDebugMsg));
            if(QLibraryInfo.isDebugBuild()) {
	            assertEquals(TestQMessageHandler.class.getName(), lastFile.get(QtMsgType.QtDebugMsg));
	            assertEquals("testInstallHandler", lastFunction.get(QtMsgType.QtDebugMsg));
            }

            qCWarning(category).append("warning").append("sent").close();
            assertEquals(null, lastMessage.get(QtMsgType.QtWarningMsg));
            if(QLibraryInfo.isDebugBuild()) {
	            assertEquals(null, lastFile.get(QtMsgType.QtWarningMsg));
	            assertEquals(null, lastFunction.get(QtMsgType.QtWarningMsg));
            }

            qCCritical(category).append("critical").append("sent").close();
            assertEquals("critical sent", lastMessage.get(QtMsgType.QtCriticalMsg));
            if(QLibraryInfo.isDebugBuild()) {
	            assertEquals(TestQMessageHandler.class.getName(), lastFile.get(QtMsgType.QtCriticalMsg));
	            assertEquals("testInstallHandler", lastFunction.get(QtMsgType.QtCriticalMsg));
            }
            
            qCDebug(category, "This must not lead to crash: %s");
            assertEquals("This must not lead to crash: %s", lastMessage.get(QtMsgType.QtDebugMsg));
            qCCritical(category, "critical sent (%1$s,%2$s,%3$s,%4$s)", 5, 6, 7, 8.2);
            assertEquals("critical sent (5,6,7,8.2)", lastMessage.get(QtMsgType.QtCriticalMsg));
            
            // should not crash
            qWarning()
            	.ws()
            	.uppercasedigits()
            	.uppercasebase()
            	.showbase()
            	.scientific()
	            .right()
	            .reset()
	            .qSetRealNumberPrecision(2)
	            .qSetPadChar('A')
	            .qSetFieldWidth(4)
	            .bin()
	            .bom()
	            .center()
	            .dec()
	            .endl()
	            .fixed()
	            .flush()
	            .forcepoint()
	            .forcesign()
	            .left()
	            .center()
	            .hex()
	            .oct()
	            .lowercasebase()
	            .lowercasedigits()
	            .noforcepoint()
	            .noforcesign()
	            .noshowbase()
	            .append("warning")
	            .close();
            
            lastMessage.clear();
            lastFile.clear();
            lastFunction.clear();
            
            qInstallExceptionMessageHandler();
            try {
				qWarning("EXN");
				fail("Exception expected to be thrown.");
			} catch (RuntimeException e) {
			}
            
            try {
				qWarning().append("EXN").close();
				fail("Exception expected to be thrown.");
			} catch (RuntimeException e) {
			}
            
            try {
				MessageHandler.sendWarning("EXN");
				fail("Exception expected to be thrown.");
			} catch (RuntimeException e) {
			}
        } finally {
        	qInstallMessageHandler(null);
        }
    }

    private Map<QtMsgType,String> lastMessage = new EnumMap<>(QtMsgType.class);
    private Map<QtMsgType,String> lastFile = new EnumMap<>(QtMsgType.class);
    private Map<QtMsgType,String> lastFunction = new EnumMap<>(QtMsgType.class);

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQMessageHandler.class.getName());
    }
}
