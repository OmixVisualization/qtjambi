/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import static org.junit.Assume.assumeTrue;

import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicReference;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtInvokable;
import io.qt.QtUtilities;
import io.qt.autotests.generated.General;
import io.qt.core.*;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QOpenGLContext;
import io.qt.network.*;
import io.qt.qml.QJSValue;
import io.qt.qml.QQmlApplicationEngine;
import io.qt.quick.QQuickItem;
import io.qt.quick.QQuickWindow;
import io.qt.webchannelquick.QQmlWebChannel;
import io.qt.webengine.core.QWebEngineLoadingInfo;
import io.qt.webengine.core.QWebEngineProfile;
import io.qt.webengine.core.QWebEngineSettings;
import io.qt.webengine.quick.QtWebEngineQuick;
import io.qt.websockets.*;
import io.qt.widgets.QApplication;

public class TestWebSockets extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
        if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.MacOS)
    			&& "x86_64".equals(System.getProperty("os.arch").toLowerCase())
    			&& QLibraryInfo.version().compareTo(new QVersionNumber(6,5,4))>=0) {
    		Assert.assertFalse("env SYSTEM_VERSION_COMPAT=0 required to run WebEngine on macOS x86_64", "10.16".equals(General.stringSysctlByName("kern.osproductversion")));
    	}
        Assume.assumeFalse("Cannot run on Android", QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
        Assume.assumeFalse("Cannot run on Windows MINGW", QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Windows) && !QLibraryInfo.build().contains("MSVC"));
    	QtUtilities.initializePackage("io.qt.webengine.widgets");
        QCoreApplication.setAttribute(Qt.ApplicationAttribute.AA_ShareOpenGLContexts);
        ApplicationInitializer.testInitializeWithWidgets();
    	assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
    	boolean found = false;
    	try {
			Class<?> cls = Class.forName("io.qt.webengine.widgets.QWebEngineView");
			if (cls != null)
				found = true;
		} catch (ClassNotFoundException e) {
		}
    	assumeTrue("QWebEngineView not available.", found);
    	QtWebEngineQuick.initialize();
    	assumeTrue("global share context not available.", QOpenGLContext.globalShareContext()!=null);
    	QWebEngineProfile.defaultProfile().settings().setAttribute(QWebEngineSettings.WebAttribute.PluginsEnabled, true);
        QWebEngineProfile.defaultProfile().settings().setAttribute(QWebEngineSettings.WebAttribute.DnsPrefetchEnabled, true);
        QWebEngineProfile.defaultProfile().setPersistentCookiesPolicy(QWebEngineProfile.PersistentCookiesPolicy.AllowPersistentCookies);
    }
    
    private static class Server extends QObject {
    	Server(AtomicReference<String> received, AtomicBoolean connected, int[] port){
    		this(received, connected, port, null);
    	}
    	
    	Server(AtomicReference<String> received, AtomicBoolean connected, int[] port, QObject parent){
        	super(parent);
        	this.connected = connected;
        	this.received = received;
        	if (!m_pWebSocketServer.listen(QHostAddress.SpecialAddress.LocalHost, port[0])) {
            	QRandomGenerator random = new QRandomGenerator();
            	random.bounded(1025, 49151);
            	do {
            		port[0] = random.invoke();
            	}while(!m_pWebSocketServer.listen(QHostAddress.SpecialAddress.LocalHost, port[0]));
            }
    		System.out.println("Server listening on port "+port[0]);
            connect(m_pWebSocketServer.newConnection, this, Server::onNewConnection);
        }
        
        void destroy(){
        	m_pWebSocketServer.close();
        	dispose();
        }
        
        private static QString getIdentifier(QWebSocket peer)
        {
            return new QString("%1:%2").arg(peer.peerAddress().toString(),
                                               QString.number(peer.peerPort()));
        }

        private void onNewConnection(){
        	connected.set(true);
            var pSocket = m_pWebSocketServer.nextPendingConnection();
            System.out.println(getIdentifier(pSocket)+" connected!");
            pSocket.setParent(this);

            connect(pSocket.textMessageReceived, this, Server::processMessage);
            connect(pSocket.disconnected, this, Server::socketDisconnected);
            m_clients.append(pSocket);
        }
        
        private void processMessage(String message){
        	received.set(message);
        	System.out.println(message);
            QWebSocket pSender = (QWebSocket)sender();
            for (QWebSocket pClient : m_clients) {
                if (pClient != pSender) //don't echo message back to sender
                    pClient.sendTextMessage(message);
            }
        }
        private void socketDisconnected(){
        	QWebSocket pClient = (QWebSocket)sender();
        	System.out.println(getIdentifier(pClient)+" disconnected!");
            if (pClient!=null) {
                m_clients.removeAll(pClient);
                pClient.disposeLater();
            }
        }

        private final QWebSocketServer m_pWebSocketServer = new QWebSocketServer("Chat Server", QWebSocketServer.SslMode.NonSecureMode, this);
        private final QList<QWebSocket> m_clients = new QList<>(QWebSocket.class);
        private final AtomicBoolean connected;
        private final AtomicReference<String> received;
    }
    
    @Test
    public void test() throws Throwable {
    	QQmlWebChannel.staticMetaObject.hashCode();
        QQuickWindow.staticMetaObject.hashCode();
        QQuickItem.staticMetaObject.hashCode();
        
    	QSemaphore lock = new QSemaphore();
    	int[] port = {1024};
    	AtomicBoolean connected = new AtomicBoolean();
    	AtomicReference<String> received = new AtomicReference<>();
    	Throwable[] throwable = {null};
    	QThread thread = new QThread() {
			@Override
			protected void run() {
				Server server;
				try {
					server = new Server(received, connected, port);
				}catch(Throwable t) {
					throwable[0] = t;
					lock.release();
					return;
				}
				lock.release();
				exec();
				server.destroy();
			}
    	};
    	thread.start();
    	lock.acquire();
    	if(throwable[0]!=null)
    		throw throwable[0];
    	QQmlApplicationEngine engine = new QQmlApplicationEngine();
    	engine.loadData(new QByteArray("import QtQuick;"
    			+ "import QtQuick.Window;"
    			+ "import QtWebEngine;"
    			+ "Window{"
    			+ "Row{anchors.fill: parent;"
    			+ "WebEngineView{id: devtools;width: parent.width / 2;height: parent.height}"
    			+ "WebEngineView{objectName: 'QQuickWebEngineView';devToolsView: devtools;width: parent.width / 2;height: parent.height;"
    			+ "function doRunJavaScript(code,consumer){"
    			+ "runJavaScript(code, value => consumer.accept(value));"
    			+ "}"
    			+ "}"
    			+ "}"
    			+ "}"));
    	QList<QObject> rootObjects = engine.rootObjects();
        Assert.assertTrue(rootObjects.get(0)!=null);
        QQuickWindow window = (QQuickWindow)rootObjects.get(0);
        QQuickItem webEngineView = window.findChild(QQuickItem.class, "QQuickWebEngineView");
        Object wreceived[] = {null};
        QObject consumer = new QObject() {
			@QtInvokable
			public void accept(QJSValue value) {
				wreceived[0] = value.toVariant();
			}
        };
        QMetaMethod mtd = webEngineView.metaObject().method("doRunJavaScript", Object.class, Object.class);
        QMetaObject.findSignal(webEngineView, "loadingChanged", QWebEngineLoadingInfo.class).connect(info->{
        	switch(info.getStatus()) {
			case LoadFailedStatus:
				QApplication.quit();
				break;
			case LoadSucceededStatus:
			{
        		mtd.invoke(webEngineView, "triggerConnect("+port[0]+")", consumer);
        		QApplication.processEvents();
        		QTimer.singleShot(1000, ()->{
        			mtd.invoke(webEngineView, "triggerMessage('A', 'B')", consumer);
        			QApplication.processEvents();
        			QTimer.singleShot(1500, QCoreApplication::quit);
        		});
        	}
				break;
			default:
				break;
        	
        	}
        });
        window.show();
        QTimer.singleShot(500, ()->webEngineView.setProperty("url", new QUrl("qrc:io/qt/autotests/websocketstest.html")));
        QCoreApplication.exec();
    	thread.quit();
    	thread.join();
    	Assert.assertTrue(connected.get());
    	Assert.assertEquals("A: B", received.get());
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestWebSockets.class.getName());
    }
}
