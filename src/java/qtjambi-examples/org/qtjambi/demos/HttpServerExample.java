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

package org.qtjambi.demos;
import org.qtjambi.examples.QtJambiExample;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.core.QTextStream;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.network.QHostAddress;
import org.qtjambi.qt.network.QHostAddress.SpecialAddress;
import org.qtjambi.qt.network.QTcpServer;
import org.qtjambi.qt.network.QTcpSocket;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QGridLayout;
import org.qtjambi.qt.widgets.QMessageBox;
import org.qtjambi.qt.widgets.QPushButton;
import org.qtjambi.qt.widgets.QTextEdit;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "Simple HTTP Server")
public class HttpServerExample extends QWidget {

    public static void main(String[] args) {
        QApplication.initialize(args);

        HttpServerExample example = new HttpServerExample(null);
        example.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }

    HttpServer server;
    QTextEdit editor;

    public HttpServerExample(QWidget parent) {
        server = new HttpServer(this);
        if (!server.start()) {
            QMessageBox.critical(this, tr("HTTP Server"),
                    tr("Unable to start the server: ") + server.errorString());
            close();
        }

        QPushButton publishButton = new QPushButton(this);
        publishButton.setText("Publish");
        editor = new QTextEdit(this);

        editor.setPlainText("<h1>Server is up and running!</h1>"
                + "You should be able to view it in a normal web browser."
                + " Try this address: http://localhost:" + server.serverPort());

        QGridLayout layout = new QGridLayout(this);
        setLayout(layout);
        layout.addWidget(publishButton);
        layout.addWidget(editor);

        publishButton.clicked.connect(this, "publish()");

        setWindowTitle(tr("Simple HTTP Server"));
        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));
    }

    protected void publish() {
        server.publish(editor.toPlainText());
    }

    class HttpServer extends QTcpServer {
        private String text;

        public HttpServer(QObject parent) {
            super(parent);
        }

        public void publish(String text) {
            this.text = text;
        }

        public boolean start() {
            if (!listen(new QHostAddress(SpecialAddress.Any), (char) 8080)) {
                close();
                return false;
            }

            this.newConnection.connect(this, "newConnection()");

            return true;
        }

        protected void newConnection() {
            QTcpSocket socket = nextPendingConnection();
            if (socket != null) {
                socket.readyRead.connect(this, "readClient()");
                socket.disconnected.connect(socket, "disposeLater()");
            }
        }

        protected void readClient() {
            QTcpSocket socket = (QTcpSocket) signalSender();

            if (socket.canReadLine()) {
                if (socket.readLine().startsWith("GET ")) {
                    QTextStream os = new QTextStream(socket);
                    os.setCodec("utf-8");

                    os.writeString("HTTP/1.0 200 Ok\r\n");
                    os.writeString("Content-Type: text/html; charset=\"utf-8\"\r\n");
                    os.writeString("\r\n");
                    if (text != null && !text.equals("")) {
                        os.writeString(text);
                    } else {
                        os.writeString("<h1>This page is empty</h1>");
                    }
                    socket.close();
                }
            }
        }
    }
}
