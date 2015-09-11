/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package generator;

import org.qtjambi.qt.*;
import org.qtjambi.qt.network.*;

class QHostInfo___ extends QHostInfo {
        /**
         * Looks up the IP address(es) associated with host name <code>name</code>,
         * and returns an ID for the lookup. When the result of the lookup is ready,
         * the specified method in <code>receiver</code> is called with a
         * <code>QHostInfo</code> argument. The <code>QHostInfo</code> object can
         * then be inspected to get the results of the lookup.
         *
         * @param name The host name.
         * @param receiver The object on which the method will be called.
         * @param methodName The name of a method which takes a single <code>QHostInfo</code> argument.
         */
        public static int lookupHost(String name, org.qtjambi.qt.core.QObject receiver, String methodName) {
            methodName = org.qtjambi.qt.internal.QtJambiInternal.SlotPrefix + methodName + "(QHostInfo)";
            return lookupHost(name, receiver, org.qtjambi.qt.QNativePointer.createCharPointer(methodName));
        }

        /**
         * Looks up the IP address(es) associated with host name <code>name</code>,
         * and returns an ID for the lookup. When the result of the lookup is ready,
         * the specified signal is emitted with a <code>QHostInfo</code> argument.
         * The <code>QHostInfo</code> object can then be inspected to get the results
         * of the lookup.
         *
         * @param name The host name.
         * @param signal The signal to emit. Must take a single <code>QHostInfo</code>
         *               argument, and must be contained in a <code>QObject</code>
         *               subclass.
         */
        public static int lookupHost(String name, org.qtjambi.qt.QSignalEmitter.Signal1<QHostInfo> signal) {
            if (!(signal.containingObject() instanceof org.qtjambi.qt.core.QObject)) {
                throw new IllegalArgumentException("Only signals contained in QObject subclasses are supported. "
                                                   + "Signal's class is '" + signal.containingObject().getClass() + "'.");
            }

            String methodName = org.qtjambi.qt.internal.QtJambiInternal.SignalPrefix + signal.name() + "(QHostInfo)";
            return lookupHost(name, (org.qtjambi.qt.core.QObject)signal.containingObject(),
                              org.qtjambi.qt.QNativePointer.createCharPointer(methodName));
        }
}// class

class QAbstractSocket___ extends QAbstractSocket {

        public final void connectToHost(String host, int port, org.qtjambi.qt.core.QIODevice.OpenMode mode) {
            connectToHost(host, (short) port, mode);
        }

        public final void connectToHost(String host, int port, org.qtjambi.qt.core.QIODevice.OpenMode mode, QAbstractSocket.NetworkLayerProtocol protocol) {
            connectToHost(host, (short) port, mode, protocol);
        }

        public final void connectToHost(String host, int port, org.qtjambi.qt.core.QIODevice.OpenModeFlag ... mode) {
            connectToHost(host, (short) port, mode);
        }

        public final void connectToHost(String host, int port) {
            connectToHost(host, (short) port);
        }

        public final void connectToHost(QHostAddress host, int port, org.qtjambi.qt.core.QIODevice.OpenMode mode) {
            connectToHost(host, (short) port, mode);
        }

        public final void connectToHost(QHostAddress host, int port, org.qtjambi.qt.core.QIODevice.OpenModeFlag ... mode) {
            connectToHost(host, (short) port, mode);
        }

        public final void connectToHost(QHostAddress host, int port) {
            connectToHost(host, (short) port);
        }

        public final int localPort() {
            return localPort_private();
        }

        public final int peerPort() {
            return peerPort_private();
        }

        protected final void setLocalPort(int port) {
            setLocalPort((short) port);
        }

        protected final void setPeerPort(int port) {
            setPeerPort((short) port);
        }

        public final boolean bind(QHostAddress address, int port) {
            return bind_private(address, (short) port);
        }

        public final boolean bind() {
            return bind_private();
        }

        public final boolean bind(int port) {
            return bind_private((short) port);
        }

        public final boolean bind(QHostAddress address, int port, BindFlag ... mode) {
            return bind_private(address, (short) port, mode);
        }

        public final boolean bind(QHostAddress address, int port, BindMode mode) {
            return bind_private(address, (short) port, mode);
        }

        public final boolean bind(int port, BindFlag ... mode) {
            return bind_private((short) port, mode);
        }

        public final boolean bind(int port, BindMode mode) {
            return bind_private((short) port, mode);
        }

}// class

class QSslSocket___ extends QSslSocket {
}// class

class QTcpServer___ extends QTcpServer {

        public final boolean listen(QHostAddress address, int port) {
            return listen(address, (short) port);
        }

        public final boolean listen(QHostAddress address) {
            return listen(address, '\0');
        }

        public final boolean listen() {
            return listen(new org.qtjambi.qt.network.QHostAddress(org.qtjambi.qt.network.QHostAddress.SpecialAddress.Any));
        }

        public final int serverPort() {
            return serverPort_private();
        }

        public enum Result {
            Success, Failure, TimedOut
        }

        public final Result waitForNewConnection(int msec) {
            QNativePointer np = new QNativePointer(QNativePointer.Type.Boolean);
            boolean success = waitForNewConnection(msec, np);

            return (np.booleanValue() ? Result.TimedOut : (success ? Result.Success : Result.Failure));
        }

        public final Result waitForNewConnection() {
            return waitForNewConnection(0);
        }

}// class

class QUdpSocket___ extends QUdpSocket {

        public static class HostInfo {
                public HostInfo() {
                    address = new QHostAddress();
                    port = 0;
                }

                public QHostAddress address;
                public int port;
        }

        public final int readDatagram(byte data[], HostInfo info) {
            QNativePointer np = new QNativePointer(QNativePointer.Type.Byte, data.length);
            QNativePointer address = info != null && info.address != null ? info.address.nativePointer() : null;
            QNativePointer port = new QNativePointer(QNativePointer.Type.Short);

            int len = (int) readDatagram(np, data.length, address, port);
            if (info != null)
                info.port = port.shortValue();
            for (int i = 0; i < len; ++i)
                data[i] = np.byteAt(i);

            return len;
        }

        public final int readDatagram(byte data[]) {
            return readDatagram(data, null);
        }

        public final int writeDatagram(byte data[], QHostAddress address, int port) {
            QNativePointer np = org.qtjambi.qt.internal.QtJambiInternal.byteArrayToNativePointer(data);
            return (int) writeDatagram(np, data.length, address, (short) port);
        }

        public final int writeDatagram(org.qtjambi.qt.core.QByteArray data, QHostAddress address, int port) {
            return (int) writeDatagram(data, address, (short) port);
        }

}// class

class QNetworkProxy___ extends QNetworkProxy {

        public QNetworkProxy(QNetworkProxy.ProxyType type) {
            this(type, null, '\0');
        }

        public QNetworkProxy(QNetworkProxy.ProxyType type, String host) {
            this(type, host, '\0');
        }

        public QNetworkProxy(QNetworkProxy.ProxyType type, String host, int port) {
            this(type, host, (short) port);
        }

        public QNetworkProxy(QNetworkProxy.ProxyType type, String host, int port, String username) {
            this(type, host, (short) port, username);
        }

        public QNetworkProxy(QNetworkProxy.ProxyType type, String host, int port, String username, String password) {
            this(type, host, (short) port, username, password);
        }

        public final void setPort(int port) {
            setPort((short) port);
        }

        public final int port() {
            return port_private();
        }

}// class

class QNetworkAccessManager___ extends QNetworkAccessManager {
}// class

class QLocalServer___ extends QLocalServer {

        public enum Result {
            Success, Failure, TimedOut
        }

        public final Result waitForNewConnection(int msec) {
            QNativePointer np = new QNativePointer(QNativePointer.Type.Boolean);
            boolean success = waitForNewConnection(msec, np);

            return (np.booleanValue() ? Result.TimedOut : (success ? Result.Success : Result.Failure));
        }

        public final Result waitForNewConnection() {
            return waitForNewConnection(0);
        }

}// class
