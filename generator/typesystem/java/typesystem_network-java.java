/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import io.qt.*;
import io.qt.network.*;

class QSsl___{
    @io.qt.QtUninvokable
    public native static boolean isAvailable();
}// class

class QHostInfo___ extends QHostInfo {
    /**
     * Looks up the IP address(es) associated with host name <code>name</code>,
     * and returns an ID for the lookup. When the result of the lookup is ready,
     * the slot is called with a <code>QHostInfo</code> argument.
     * The <code>QHostInfo</code> object can then be inspected to get the results
     * of the lookup.
     *
     * @param name The host name.
     * @param slot The clot taking the result.
     */
    @io.qt.QtUninvokable
    public static int lookupHost(String name, io.qt.core.QMetaObject.Slot1<QHostInfo> slot) {
        io.qt.core.QObject context = QtJambi_LibraryUtilities.internal.lambdaContext(slot);
        return lookupHost(name, context, slot);
    }
    
    @io.qt.QtUninvokable
    public static native int lookupHost(String name, io.qt.core.QObject context, io.qt.core.QMetaObject.Slot1<QHostInfo> slot);
}// class

class QAbstractSocket___ extends QAbstractSocket {
}// class

class Server___ {
    @io.qt.QtUninvokable
    public final boolean waitForNewConnection() throws QTimeoutException {
        return waitForNewConnection(0);
    }
}// class

class QUdpSocket___ extends QUdpSocket {

        public static final class HostInfo {
            public HostInfo() {
            }

            public QHostAddress address(){return address;}
            public int port(){return port;}
            
            private QHostAddress address;
            private short port;
        }

        public final long readDatagram(byte data[], HostInfo info) {
            return readDatagram(java.nio.ByteBuffer.wrap(data), info);
        }

        public final long readDatagram(byte data[]) {
            return readDatagram(data, null);
        }
        
        public final long readDatagram(java.nio.ByteBuffer data) {
            return readDatagram(data, null);
        }
        
        public final long writeDatagram(byte data[], QHostAddress address, short port) {
            return writeDatagram(java.nio.ByteBuffer.wrap(data), address, port);
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

class QNetworkReply___ extends QNetworkReply {
    @io.qt.QtUninvokable
    public final io.qt.core.QByteArray rawHeader(String headerName){
        return rawHeader(new io.qt.core.QByteArray(headerName));
    }
    @io.qt.QtUninvokable
    public final boolean hasRawHeader(String headerName){
        return hasRawHeader(new io.qt.core.QByteArray(headerName));
    }
}// class

class QNetworkAccessManager___ extends QNetworkAccessManager {
}// class

