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

package org.qtjambi.examples;

import java.util.Iterator;
import java.util.List;
import java.util.Vector;

import org.qtjambi.qt.core.QByteArray;
import org.qtjambi.qt.core.QIODevice;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.core.QProcess;
import org.qtjambi.qt.core.QRegExp;
import org.qtjambi.qt.core.QTextCodec;
import org.qtjambi.qt.core.QTime;
import org.qtjambi.qt.core.QTimer;
import org.qtjambi.qt.core.QTimerEvent;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.core.Qt.MatchFlag;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QTextCursor;
import org.qtjambi.qt.gui.QTextTable;
import org.qtjambi.qt.gui.QTextTableFormat;
import org.qtjambi.qt.network.QAbstractSocket;
import org.qtjambi.qt.network.QHostAddress;
import org.qtjambi.qt.network.QHostInfo;
import org.qtjambi.qt.network.QNetworkAddressEntry;
import org.qtjambi.qt.network.QNetworkInterface;
import org.qtjambi.qt.network.QTcpServer;
import org.qtjambi.qt.network.QTcpSocket;
import org.qtjambi.qt.network.QUdpSocket;
import org.qtjambi.qt.qtjambi.util.RetroTranslatorHelper;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QDialog;
import org.qtjambi.qt.widgets.QListWidgetItem;
import org.qtjambi.qt.widgets.QMessageBox;
import org.qtjambi.qt.widgets.QScrollBar;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "Chat")
public class Chat extends QDialog {

    public static void main(String[] args) {

        QApplication.initialize(args);
        Chat chat = new Chat();
        chat.show();
        QApplication.execStatic();
        QApplication.shutdown();

    }

    private Ui_ChatDialog ui = new Ui_ChatDialog();
    private Client client = new Client();
    private String myNickName;
    private QTextTableFormat tableFormat = new QTextTableFormat();

    public Chat() {
        this(null);
    }

    public Chat(QWidget parent) {
        super(parent);

        ui.setupUi(this);

        ui.lineEdit.setFocusPolicy(Qt.FocusPolicy.StrongFocus);
        ui.textEdit.setFocusPolicy(Qt.FocusPolicy.NoFocus);
        ui.textEdit.setReadOnly(true);
        ui.listWidget.setFocusPolicy(Qt.FocusPolicy.NoFocus);

        ui.lineEdit.returnPressed.connect(this, "returnPressed()");

        client.newMessage__from__message.connect(this, "appendMessage(String, String)");
        client.newParticipant__nick.connect(this, "newParticipant(String)");
        client.participantLeft__nick.connect(this, "participantLeft(String)");

        myNickName = client.nickName();
        newParticipant(myNickName);
        tableFormat.setBorder(0);

        setWindowTitle("Chat " + myNickName);

        QTimer.singleShot(10 * 1000, this, "showInformation()");
    }

    public String tr(String str, Object... arguments) {
        return String.format(tr(str), arguments);
    }

    private void appendMessage(final String from, final String message) {
        if (from.equals("") || message.equals(""))
            return;

        QTextCursor cursor = new QTextCursor(ui.textEdit.textCursor());
        cursor.movePosition(QTextCursor.MoveOperation.End);
        QTextTable table = cursor.insertTable(1, 2, tableFormat);
        table.cellAt(0, 0).firstCursorPosition().insertText("<" + from + "> ");
        table.cellAt(0, 1).firstCursorPosition().insertText(message);
        QScrollBar bar = ui.textEdit.verticalScrollBar();
        bar.setValue(bar.maximum());
    }

    void returnPressed() {
        String text = ui.lineEdit.text();
        if (text.equals(""))
            return;

        if (text.startsWith("/")) {
            QColor color = ui.textEdit.textColor();
            ui.textEdit.setTextColor(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.red));
            ui.textEdit.append(tr("! Unknown command: ") + text.substring(text.indexOf(' ')));
            ui.textEdit.setTextColor(color);
        } else {
            client.sendMessage(text);
            appendMessage(myNickName, text);
        }

        ui.lineEdit.clear();
    }

    private void newParticipant(final String nick) {
        if (nick.equals(""))
            return;

        QColor color = ui.textEdit.textColor();
        ui.textEdit.setTextColor(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.gray));
        ui.textEdit.append(String.format(tr("* %1$s has joined"), nick));
        ui.textEdit.setTextColor(color);
        ui.listWidget.addItem(new QListWidgetItem(nick));
    }

    void participantLeft(final String nick) {
        if (nick.equals(""))
            return;

        List<QListWidgetItem> items = ui.listWidget.findItems(nick, MatchFlag.MatchExactly);

        for (Iterator<QListWidgetItem> iterator = items.iterator(); iterator.hasNext();) {
            QListWidgetItem item = (QListWidgetItem) iterator.next();
            ui.listWidget.removeItemWidget(item);
            ui.listWidget.takeItem(ui.listWidget.row(item));
        }

        QColor color = ui.textEdit.textColor();
        ui.textEdit.setTextColor(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.gray));
        ui.textEdit.append(String.format(tr("* %1$s has left"), nick));
        ui.textEdit.setTextColor(color);
    }

    void showInformation() {
        if (ui.listWidget.count() == 1) {
            QMessageBox.information(this, tr("Chat"), tr("Launch several instances of this " + "program on your local network and " + "start chatting!"));
        }
    }

    class Client extends QObject {

        private PeerManager peerManager;
        private Server server = new Server(this);
        private Vector<Connection> peers = new Vector<Connection>();
        private Vector<Connection> inConnection = new Vector<Connection>();

        // from, message
        Signal2<String, String> newMessage__from__message = new Signal2<String, String>();
        // nick
        Signal1<String> newParticipant__nick = new Signal1<String>();
        // nick
        Signal1<String> participantLeft__nick = new Signal1<String>();

        Client() {
            peerManager = new PeerManager(server);
            peerManager.startBroadcasting();

            peerManager.newConnection__hostAddress_port.connect(this, "newConnection(QHostAddress, int)");
            server.newConnection__descriptor.connect(this, "newConnection(int)");
        }

        public synchronized void sendMessage(final String message) {
            if (message.equals(""))
                return;

            for (Connection connection : peers) {
                connection.sendMessage(message);
            }
        }

        public String nickName() {
            return peerManager.userName() + "@" + QHostInfo.localHostName() + ":" + (int) server.serverPort();
        }

        protected void newConnection(QHostAddress address, int port) {
            if (inConnection.isEmpty()) {
                for (Connection conn : peers) {
                    if (conn.equals(address, port))
                        return;
                }
                Connection connection = new Connection(this, port, server.serverPort());
                connection.connectToHost(address, port, new QIODevice.OpenMode(QIODevice.OpenModeFlag.ReadWrite));
                newConnection(connection);
            }
        }

        protected void newConnection(int descriptor) {
            Connection connection = new Connection(this, -1, server.serverPort());
            connection.setSocketDescriptor(descriptor);
            newConnection(connection);
        }

        private void newConnection(Connection connection) {
            inConnection.add(connection);
            connection.setGreetingMessage(peerManager.userName().toString());

            connection.error.connect(this, "connectionError(QAbstractSocket$SocketError)");
            connection.disconnected.connect(this, "disconnected()");
            connection.readyForUse.connect(this, "readyForUse()");
        }

        protected synchronized void readyForUse() {
            Connection connection = (Connection) signalSender();

            connection.newMessage.connect(this.newMessage__from__message);

            peers.add(connection);

            String nick = connection.name();

            if (!nick.equals(""))
                newParticipant__nick.emit(nick);

            inConnection.removeElement(connection);
        }

        protected void disconnected() {
            removeConnection((Connection) signalSender());
        }

        protected void connectionError(QAbstractSocket.SocketError socketError) {
            removeConnection((Connection) signalSender());
        }

        protected void removeConnection(Connection con) {
            if (peers.removeElement(con)) {
                String nick = con.name();
                if (!nick.equals(""))
                    participantLeft__nick.emit(nick);
            }
        }
    }

    static class Connection extends QTcpSocket {

        static final int MaxBufferSize = 1024000;
        static final int TransferTimeout = 30 * 1000;
        static final int PongTimeout = 60 * 1000;
        static final int PingInterval = 5 * 1000;
        static final char SeparatorToken = ' ';

        private enum ConnectionState {
            WaitingForGreeting, ReadingGreeting, ReadyForUse
        };

        private enum DataType {
            PlainText, Ping, Pong, Greeting, Undefined
        };

        Signal0 readyForUse = new Signal0();
        Signal2<String, String> newMessage = new Signal2<String, String>();

        private QTextCodec codec = QTextCodec.codecForName(new QByteArray("UTF-8"));

        private String greetingMessage;
        private String username;
        private QTimer pingTimer = new QTimer();
        private QTime pongTime = new QTime();
        private QByteArray buffer = new QByteArray();
        private ConnectionState state;
        private DataType currentDataType;
        private int numBytesForCurrentDataType;
        private int transferTimerId;
        private boolean isGreetingMessageSent;
        private int otherServerPort;
        private int thisServerPort;

        Connection(QObject parent, int otherServerPort, int thisServerPort) {
            super(parent);
            this.otherServerPort = otherServerPort;
            this.thisServerPort = thisServerPort;
            greetingMessage = tr("undefined");
            username = tr("unknown");
            state = ConnectionState.WaitingForGreeting;
            currentDataType = DataType.Undefined;
            numBytesForCurrentDataType = -1;
            transferTimerId = 0;
            isGreetingMessageSent = false;
            pingTimer.setInterval(PingInterval);

            readyRead.connect(this, "processReadyRead()");
            disconnected.connect(pingTimer, "stop()");
            pingTimer.timeout.connect(this, "sendPing()");
            connected.connect(this, "sendGreetingMessage()");
        }

        @Override
        public boolean equals(Object other) {
            if (other instanceof Connection) {
                Connection con = (Connection) other;
                return (con.peerAddress().equals(peerAddress()) && con.otherServerPort == otherServerPort);
            }
            return false;
        }

        public boolean equals(QHostAddress address, int port) {
            return this.peerAddress().equals(address) && this.otherServerPort == port;
        }

        String name() {
            return username;
        }

        void setGreetingMessage(final String message) {
            greetingMessage = message;
        }

        boolean sendMessage(final String message) {
            if (message.equals(""))
                return false;

            QByteArray msg = codec.fromUnicode(message);
            QByteArray data = new QByteArray("MESSAGE " + msg.length() + " ");
            data.append(msg);
            return write(data) == data.size();
        }

        public void timerEvent(QTimerEvent timerEvent) {
            if (timerEvent.timerId() == transferTimerId) {
                abort();
                killTimer(transferTimerId);
                transferTimerId = 0;
            }
        }

        void processReadyRead() {
            if (state == ConnectionState.WaitingForGreeting) {
                if (!readProtocolHeader())
                    return;
                if (currentDataType != DataType.Greeting) {
                    abort();
                    return;
                }
                state = ConnectionState.ReadingGreeting;
            }

            if (state == ConnectionState.ReadingGreeting) {
                if (!hasEnoughData())
                    return;

                buffer = read(numBytesForCurrentDataType);
                if (buffer.size() != numBytesForCurrentDataType) {
                    abort();
                    return;
                }

                username = buffer.toString();

                // real regex can not use RetroTranslatorHelper
                String[] list = username.split("[:@]");
                if(list.length!=3) {
                    abort();
                    return;
                }
                otherServerPort = Integer.parseInt(list[2]);

                currentDataType = DataType.Undefined;
                numBytesForCurrentDataType = 0;
                buffer.clear();

                if (!isValid()) {
                    abort();
                    return;
                }

                if (!isGreetingMessageSent)
                    sendGreetingMessage();

                pingTimer.start();
                pongTime.start();
                state = ConnectionState.ReadyForUse;

                readyForUse.emit();
            }

            do {
                if (currentDataType == DataType.Undefined) {
                    if (!readProtocolHeader())
                        return;
                }
                if (!hasEnoughData())
                    return;
                processData();
            } while (bytesAvailable() > 0);
        }

        void sendPing() {
            if (pongTime.elapsed() > PongTimeout) {
                abort();
                return;
            }

            write(new QByteArray("PING 1 p"));
        }

        private void sendGreetingMessage() {
            QByteArray greeting = new QByteArray(greetingMessage + "@" + this.localAddress() + ":" + thisServerPort);
            QByteArray data = new QByteArray("GREETING " + greeting.length() + " " + greeting);

            if (write(data) == data.size())
                isGreetingMessageSent = true;
        }

        private int readDataIntoBuffer(int maxSize) {
            if (maxSize > MaxBufferSize)
                return 0;

            int numBytesBeforeRead = buffer.size();
            if (numBytesBeforeRead == MaxBufferSize) {
                abort();
                return 0;
            }

            while (bytesAvailable() > 0 && buffer.size() < maxSize) {
                buffer.append(read(1));
                if (buffer.endsWith("" + SeparatorToken))
                    break;
            }
            return buffer.size() - numBytesBeforeRead;
        }

        private int dataLengthForCurrentDataType() {
            if (bytesAvailable() <= 0 || readDataIntoBuffer(MaxBufferSize) <= 0 || !buffer.endsWith("" + SeparatorToken))
                return 0;

            buffer.chop(1);
            int number = buffer.toInt();
            buffer.clear();
            return number;
        }

        private boolean readProtocolHeader() {
            if (transferTimerId != 0) {
                killTimer(transferTimerId);
                transferTimerId = 0;
            }

            if (readDataIntoBuffer(MaxBufferSize) <= 0) {
                transferTimerId = startTimer(TransferTimeout);
                return false;
            }

            if (buffer.equals("PING ")) {
                currentDataType = DataType.Ping;
            } else if (buffer.equals("PONG ")) {
                currentDataType = DataType.Pong;
            } else if (buffer.equals("MESSAGE ")) {
                currentDataType = DataType.PlainText;
            } else if (buffer.equals("GREETING ")) {
                currentDataType = DataType.Greeting;
            } else {
                currentDataType = DataType.Undefined;
                abort();
                return false;
            }

            buffer.clear();
            numBytesForCurrentDataType = dataLengthForCurrentDataType();
            return true;
        }

        private boolean hasEnoughData() {
            if (transferTimerId != 0) {
                killTimer(transferTimerId);
                transferTimerId = 0;
            }

            if (numBytesForCurrentDataType <= 0)
                numBytesForCurrentDataType = dataLengthForCurrentDataType();

            if (bytesAvailable() < numBytesForCurrentDataType || numBytesForCurrentDataType <= 0) {
                transferTimerId = startTimer(TransferTimeout);
                return false;
            }

            return true;
        }

        private void processData() {
            buffer = read(numBytesForCurrentDataType);
            if (buffer.size() != numBytesForCurrentDataType) {
                abort();
                return;
            }

            switch (currentDataType) {
            case PlainText:
                newMessage.emit(username, codec.toUnicode(buffer));
                break;
            case Ping:
                write(new QByteArray("PONG 1 p"));
                break;
            case Pong:
                pongTime.restart();
                break;
            default:
                break;
            }

            currentDataType = DataType.Undefined;
            numBytesForCurrentDataType = 0;
            buffer.clear();
        }
    }

    class PeerManager extends QObject {
        static final int BroadcastInterval = 2000;
        static final int broadcastPort = 45000;

        private Server server;
        private Vector<QHostAddress> broadcastAddresses = new Vector<QHostAddress>();
        private Vector<QHostAddress> ipAddresses = new Vector<QHostAddress>();
        private QUdpSocket broadcastSocket = new QUdpSocket();
        private QTimer broadcastTimer = new QTimer();
        private String username = "";

        Signal2<QHostAddress, Integer> newConnection__hostAddress_port = new Signal2<QHostAddress, Integer>();

        public PeerManager(Server server) {
            this.server = server;

            List<String> envVariables = new Vector<String>();

            envVariables.add("USERNAME.*");
            envVariables.add("USER.*");
            envVariables.add("USERDOMAIN.*");
            envVariables.add("HOSTNAME.*");
            envVariables.add("DOMAINNAME.*");

            List<String> environment = QProcess.systemEnvironment();

            for (String string : envVariables) {
                int index = 0;
                for (String entry : environment) {
                    if (new QRegExp(string).exactMatch(entry))
                        break;
                    index++;
                }
                if (index < environment.size()) {
                    String[] stringList = RetroTranslatorHelper.split(environment.get(index), "=");
                    if (stringList.length == 2) {
                        username = stringList[1];
                        break;
                    }
                }
            }

            if (username.equals(""))
                username = "unknown";

            updateAddresses();

            broadcastSocket.bind(new QHostAddress(QHostAddress.SpecialAddress.Any), broadcastPort, new QUdpSocket.BindMode(QUdpSocket.BindFlag.ShareAddress,
                    QUdpSocket.BindFlag.ReuseAddressHint));

            broadcastSocket.readyRead.connect(this, "readBroadcastDatagram()");

            broadcastTimer.setInterval(BroadcastInterval);

            broadcastTimer.timeout.connect(this, "sendBroadcastDatagram()");

        }

        String userName() {
            return username;
        }

        void startBroadcasting() {
            broadcastTimer.start();
        }

        private boolean isLocalHostAddress(final QHostAddress address) {
            for (QHostAddress localAddress : ipAddresses) {
                if (address.equals(localAddress)) {
                    return true;
                }
            }
            return false;
        }

        void sendBroadcastDatagram() {
            QByteArray datagram = new QByteArray(username + "@" + server.serverPort());
            boolean validBroadcastAddresses = true;
            for (QHostAddress address : broadcastAddresses) {
                if (broadcastSocket.writeDatagram(datagram, address, broadcastPort) == -1)
                    validBroadcastAddresses = false;
            }

            if (!validBroadcastAddresses)
                updateAddresses();
        }

        void readBroadcastDatagram() {
            while (broadcastSocket.hasPendingDatagrams()) {
                byte datagram[] = new byte[(int) broadcastSocket.pendingDatagramSize()];
                QUdpSocket.HostInfo info = new QUdpSocket.HostInfo();
                if (broadcastSocket.readDatagram(datagram, info) == -1)
                    continue;

                QByteArray baDatagram = new QByteArray(datagram);
                List<QByteArray> list = baDatagram.split((byte) '@');
                if (list.size() != 2)
                    continue;

                int senderServerPort = Integer.parseInt(list.get(1).toString());

                if (isLocalHostAddress(info.address) && (senderServerPort == server.serverPort())) {
                    continue;
                }
                newConnection__hostAddress_port.emit(info.address, senderServerPort);
            }
        }

        private void updateAddresses() {
            updateAddresses(false);
            if (broadcastAddresses.isEmpty())
                updateAddresses(true);
        }

        private void updateAddresses(boolean enableLoopBack) {
            broadcastAddresses.clear();
            ipAddresses.clear();
            for (QNetworkInterface networkInterface : QNetworkInterface.allInterfaces()) {
                if (enableLoopBack || !networkInterface.flags().isSet(QNetworkInterface.InterfaceFlag.IsLoopBack)) {
                    for (QNetworkAddressEntry entry : networkInterface.addressEntries()) {

                        QHostAddress broadcastAddress = entry.broadcast();
                        if (!broadcastAddress.equals(QHostAddress.SpecialAddress.Null)) {
                            broadcastAddresses.add(broadcastAddress);
                        }
                        ipAddresses.add(entry.ip());
                    }
                }
            }
        }
    }

    class Server extends QTcpServer {
        Signal1<Integer> newConnection__descriptor = new Signal1<Integer>();

        public Server(QObject parent) {
            super(parent);
            listen(new QHostAddress(QHostAddress.SpecialAddress.Any), 0);
        }

        public void incomingConnection(int socketDescriptor) {
            newConnection__descriptor.emit(socketDescriptor);
        }
    }
}
