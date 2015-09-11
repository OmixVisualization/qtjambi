/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package org.qtjambi.autotests;

import static org.junit.Assert.assertEquals;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;

import org.qtjambi.autotests.generated.MessageHandler;
import org.qtjambi.qt.QNoNativeResourcesException; 
import org.qtjambi.qt.core.QCoreApplication;
import org.qtjambi.qt.core.QEvent;
import org.qtjambi.qt.core.QEventLoop;
import org.qtjambi.qt.core.QMessageHandler;
import org.qtjambi.qt.widgets.QApplication;

public class TestQMessageHandler extends QMessageHandler {
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
        QApplication.initialize(new String[] {});
        Utils.setupNativeResourceThread();
        Utils.println(2, "TestQMessageHandler.testInitialize(): done");
    }

    @AfterClass
    public static void testDispose() throws Exception {
        Utils.println(2, "TestQMessageHandler.testDispose(): begin");
        if(Utils.releaseNativeResources() > 0) {
            System.gc();
            System.runFinalization();
        }
        QApplication.processEvents();
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDelete.value());
        QApplication.quit();
        System.err.flush();
        System.out.flush();
        QApplication app = QApplication.instance();
        if(app != null)
            app.dispose();
        try {
            Utils.println(3, "TestQMessageHandler.testDispose(): done  app="+app);
        } catch(QNoNativeResourcesException e) {
            Utils.println(3, "TestQMessageHandler.testDispose(): done  org.qtjambi.qt.QNoNativeResourcesException: app="+e.getMessage());
        }
        app = null;		// kill hard-reference
        Utils.println(3, "TestQMessageHandler.testDispose(): shutdown PRE");
        QApplication.shutdown();
        Utils.println(3, "TestQMessageHandler.testDispose(): shutdown POST");

        QtJambiUnittestTools.getObjectCount(1, 0);  // fflush(stdout)
        QtJambiUnittestTools.getObjectCount(2, 0);  // fflush(stderr)
        int objectCount = QtJambiUnittestTools.getObjectCount(3, 0);  // QtJambiLink::QtJambiLink_dump()
        QtJambiUnittestTools.getObjectCount(2, 0);  // fflush(stderr)
        Utils.println(3, "TestQMessageHandler.testDispose(): end objectCount="+objectCount);

        if(objectCount == 0)
            return;  // optimization due to class loading causing some references to be set

        QtJambiUnittestTools.clearStaticReferences();
        System.gc();
        System.runFinalization();
        System.gc();
        System.runFinalization();

        QtJambiUnittestTools.getObjectCount(1, 0);  // fflush(stdout)
        QtJambiUnittestTools.getObjectCount(2, 0);  // fflush(stderr)
        objectCount = QtJambiUnittestTools.getObjectCount(3, 0);  // QtJambiLink::QtJambiLink_dump()
        QtJambiUnittestTools.getObjectCount(2, 0);  // fflush(stderr)
        Utils.println(3, "TestQMessageHandler.testDispose(): end objectCount="+objectCount);
    }

    @Test
    public void testOneHandler() {
        try {
            QMessageHandler.installMessageHandler(this);

            MessageHandler.sendDebug("debug sent");
            assertEquals(lastDebug, "debug sent");

            MessageHandler.sendWarning("warning sent");
            assertEquals(lastWarning, "warning sent");

            MessageHandler.sendCritical("critical sent");
            assertEquals(lastCritical, "critical sent");

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
                assertEquals(lastDebug, "debug sent: " + j);

                MessageHandler.sendWarning("warning sent: " + j);
                assertEquals(lastWarning, "warning sent: " + j);

                MessageHandler.sendCritical("critical sent: " + j);
                assertEquals(lastCritical, "critical sent: " + j);

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
    private String lastFatal;

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQMessageHandler.class.getName());
    }
}
