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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import io.qt.autotests.generated.AbstractSocketSubclass;
import io.qt.core.QCoreApplication;
import io.qt.core.QObject;
import io.qt.network.QAbstractSocket;
import io.qt.network.QAuthenticator;
import io.qt.network.QHostAddress;
import io.qt.network.QHostInfo;
import io.qt.network.QNetworkProxy;
import io.qt.network.QTcpServer;
import io.qt.network.QTcpSocket;
import io.qt.network.QTimeoutException;
import io.qt.network.QUdpSocket;

public class TestNetworkInjectedCode extends ApplicationInitializer {
    @Test
    public void testAbstractSocketProxyAuthenticationRequiredFromJavaToCpp()
    {
        AbstractSocketSubclass ass = new AbstractSocketSubclass(QAbstractSocket.SocketType.TcpSocket, null);

        QAbstractSocket as = new QAbstractSocket(QAbstractSocket.SocketType.TcpSocket, null);
        ass.connectProxyAuthenticationRequired(as);

        QNetworkProxy proxy = new QNetworkProxy();
        proxy.setUser("FOO");
        proxy.setPassword("BAR");

        QAuthenticator authenticator = new QAuthenticator();
        authenticator.setUser("ZIM");
        authenticator.setPassword("ZUM");

        as.proxyAuthenticationRequired.emit(proxy, authenticator);

        assertEquals("FOO", authenticator.user());
        assertEquals("BAR", authenticator.password());
    }


    private void myJavaSlot(QNetworkProxy proxy, QAuthenticator authenticator)
    {
        authenticator.setUser(proxy.password());
        authenticator.setPassword(proxy.user());
    }

    @Test
    public void testAbstractSocketProxyAuthenticationFromCppToJava()
    {
        AbstractSocketSubclass ass = new AbstractSocketSubclass(QAbstractSocket.SocketType.TcpSocket, null);

        QAbstractSocket as = new QAbstractSocket(QAbstractSocket.SocketType.TcpSocket, null);
        as.proxyAuthenticationRequired.connect(this::myJavaSlot);

        QNetworkProxy proxy = new QNetworkProxy();
        proxy.setUser("FOO");
        proxy.setPassword("BAR");

        QAuthenticator authenticator = new QAuthenticator();
        authenticator.setUser("ZIM");
        authenticator.setPassword("ZUM");

        ass.emitProxyAuthenticationRequired(as, proxy, authenticator);

        assertEquals("BAR", authenticator.user());
        assertEquals("FOO", authenticator.password());
    }

    @Test
    public void testTcpSocketProxyAuthenticationFromCppToJava()
    {
        AbstractSocketSubclass ass = new AbstractSocketSubclass(QAbstractSocket.SocketType.TcpSocket, null);

        QTcpSocket as = new QTcpSocket(null);
        as.proxyAuthenticationRequired.connect(this::myJavaSlot);

        QNetworkProxy proxy = new QNetworkProxy();
        proxy.setUser("FOO");
        proxy.setPassword("BAR");

        QAuthenticator authenticator = new QAuthenticator();
        authenticator.setUser("ZIM");
        authenticator.setPassword("ZUM");

        ass.emitProxyAuthenticationRequired(as, proxy, authenticator);

        assertEquals("BAR", authenticator.user());
        assertEquals("FOO", authenticator.password());
    }

    @Test
    public void testUdpSocketProxyAuthenticationFromCppToJava()
    {
        AbstractSocketSubclass ass = new AbstractSocketSubclass(QAbstractSocket.SocketType.TcpSocket, null);

        QUdpSocket as = new QUdpSocket(null);
        as.proxyAuthenticationRequired.connect(this::myJavaSlot);

        QNetworkProxy proxy = new QNetworkProxy();
        proxy.setUser("FOO");
        proxy.setPassword("BAR");

        QAuthenticator authenticator = new QAuthenticator();
        authenticator.setUser("ZIM");
        authenticator.setPassword("ZUM");

        ass.emitProxyAuthenticationRequired(as, proxy, authenticator);

        assertEquals("BAR", authenticator.user());
        assertEquals("FOO", authenticator.password());
    }

    private static class LookupHostQObject extends QObject
    {
        public String fromFirstSlot = "";
        public String fromSecondSlot = "";

        public final Signal1<QHostInfo> mySignal = new Signal1<>();

        public LookupHostQObject() {
            mySignal.connect(this::secondSlot);
        }

		public void firstSlot(QHostInfo info) {
            fromFirstSlot = info.addresses().isEmpty() ? "" : info.addresses().get(0).toString();
        }

        public void secondSlot(QHostInfo info) {
            fromSecondSlot = info.addresses().isEmpty() ? "" : info.addresses().get(0).toString();
        }
    }

    @Test
    public void testLookupHostWithSlot()
    {
        LookupHostQObject helloObject = new LookupHostQObject();

        // This API does not need the extra argument only the method name
        QHostInfo.lookupHost("qt.io", helloObject::firstSlot);
        int i = 0;
        while (helloObject.fromFirstSlot.length() == 0 && i < 100) {
            QCoreApplication.processEvents();
            try {
                Thread.sleep(100);
            } catch(InterruptedException e) {
                e.printStackTrace();
            }
            i++;
        }

        // Was: 62.70.27.67 (pre 02-Aug-2011)
        // Now: 62.116.130.8 (since 22-08-2015)
        // Now: 52.18.144.254 (since ???)
        assertEquals("52.18.144.254", helloObject.fromFirstSlot);
        java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "RESULT: " + helloObject.fromFirstSlot + " for " + "qt.io");
    }

    @Test
    public void testLookupHostWithSlotName()
    {
        LookupHostQObject helloObject = new LookupHostQObject();

        // This API does not need the extra argument only the method name
        QHostInfo.lookupHost("qt.io", helloObject, "firstSlot(QHostInfo)");
        int i = 0;
        while (helloObject.fromFirstSlot.length() == 0 && i < 100) {
            QCoreApplication.processEvents();
            try {
                Thread.sleep(100);
            } catch(InterruptedException e) {
                e.printStackTrace();
            }
            i++;
        }

        // Was: 62.70.27.67 (pre 02-Aug-2011)
        // Now: 62.116.130.8 (since 22-08-2015)
        // Now: 52.18.144.254 (since ???)
        assertEquals("52.18.144.254", helloObject.fromFirstSlot);
        java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "RESULT: " + helloObject.fromFirstSlot + " for " + "qt.io");
    }

    @Test
    public void testLookupHostWithSignal()
    {
        LookupHostQObject helloObject = new LookupHostQObject();

        QHostInfo.lookupHost("qt.io", helloObject.mySignal);
        int i = 0;
        while (helloObject.fromSecondSlot.length() == 0 && i < 100) {
            QCoreApplication.processEvents();
            try {
                Thread.sleep(100);
            } catch(InterruptedException e) {
                e.printStackTrace();
            }
            i++;
        }

        assertEquals("52.18.144.254", helloObject.fromSecondSlot);
        java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "RESULT: " + helloObject.fromSecondSlot + " for " + "qt.io");
    }
    
    @Test
    public void testLookupHostWithSignalName()
    {
        LookupHostQObject helloObject = new LookupHostQObject();

        QHostInfo.lookupHost("qt.io", helloObject, "mySignal(QHostInfo)");
        int i = 0;
        while (helloObject.fromSecondSlot.length() == 0 && i < 100) {
            QCoreApplication.processEvents();
            try {
                Thread.sleep(100);
            } catch(InterruptedException e) {
                e.printStackTrace();
            }
            i++;
        }

        assertEquals("52.18.144.254", helloObject.fromSecondSlot);
        java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "RESULT: " + helloObject.fromSecondSlot + " for " + "qt.io");
    }

    @Test
    public void testQTcpServerWaitForConnection() throws QTimeoutException{
        QTcpServer server = new QTcpServer();
        QHostAddress address = new QHostAddress(QHostAddress.SpecialAddress.Any);
        assertTrue(server.listen(address, 0));
        assertFalse(server.waitForNewConnection(100));
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestNetworkInjectedCode.class.getName());
    }
}
