/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
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
    /**
     * Determines the availability of ssl on this platform.
     */
    @QtUninvokable
    public native static boolean isAvailable();
}// class

class QHostInfo___ extends QHostInfo {
    
    /**
     * <p>See <a href="@docRoot/qhostinfo.html#lookupHost"><code>QHostInfo::lookupHost(QString, Functor)</code></a></p>
     * @see #lookupHost(String, io.qt.core.QObject, io.qt.core.QMetaObject.Slot1)
     */
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static int lookupHost(@NonNull String name, io.qt.core.QMetaObject.@StrictNonNull Connectable1<@NonNull QHostInfo> signal) {
        io.qt.core.QObject context = null;
        if(signal.containingObject() instanceof io.qt.core.QObject){
            context = (io.qt.core.QObject)signal.containingObject();
        }
        io.qt.core.QMetaMethod metaMethod = io.qt.core.QMetaMethod.fromSignal(signal);
        if(metaMethod!=null && metaMethod.isValid()) {
            if(metaMethod.parameterCount()!=1 && metaMethod.parameterType(0)!=io.qt.core.QMetaType.fromType(QHostInfo.class).id()) {
                throw new IllegalArgumentException("Method does not take a single QHostInfo argument: "+metaMethod.cppMethodSignature());
            }
            if(context!=null) {
                switch(metaMethod.methodType()) {
                case Signal:
                    return lookupHost(name, context, "2"+metaMethod.cppMethodSignature());
                case Method:
                case Slot:
                    return lookupHost(name, context, "1"+metaMethod.cppMethodSignature());
                default:
                    break;
                }
            }
        }
        if(signal instanceof io.qt.core.QMetaObject.Emitable1)
            return lookupHost(name, context, ((io.qt.core.QMetaObject.Emitable1<QHostInfo>)signal)::emit);
        else
            throw new IllegalArgumentException("Signal is not emitable.");
    }
}// class

class QAbstractSocket___ extends QAbstractSocket {
}// class

class Server___ {
    @QtUninvokable
    public final boolean waitForNewConnection() throws QTimeoutException {
        return waitForNewConnection(0);
    }
}// class

class QUdpSocket___ extends QUdpSocket {
        /**
         * Result class for <a href="@docRoot/qudpsocket.html#readDatagram"><code>QUdpSocket::readDatagram(char *, qint64, QHostAddress *, quint16 *)</code></a>.
         */
        public static final class HostInfo {
            public HostInfo() {
            }
            public @NonNull QHostAddress address(){return address;}
            public int port(){return port;}
            
            private QHostAddress address;
            private short port;
        }
}// class

class QRestAccessManager___ extends QRestAccessManager {
    /**
     * Callback for rest requests.
     */
    public interface Callback extends java.util.function.@StrictNonNull Consumer<io.qt.network.@Nullable QRestReply>, java.io.Serializable{
    }
}// class
