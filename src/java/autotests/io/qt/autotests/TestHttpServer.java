/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.util.concurrent.atomic.AtomicInteger;

import org.junit.Assert;
import org.junit.Test;

import io.qt.core.QCoreApplication;
import io.qt.core.QEventLoop;
import io.qt.core.QIODevice;
import io.qt.core.QSemaphore;
import io.qt.core.QThread;
import io.qt.httpserver.QHttpServer;
import io.qt.httpserver.QHttpServerRequest;
import io.qt.httpserver.QHttpServerResponder;
import io.qt.httpserver.QHttpServerResponse;
import io.qt.network.QHttpHeaders;
import io.qt.network.QNetworkAccessManager;
import io.qt.network.QNetworkReply;
import io.qt.network.QNetworkRequest;
import io.qt.network.QTcpServer;

public class TestHttpServer extends ApplicationInitializer {
	
    @Test
    public void test() {
    	AtomicInteger port = new AtomicInteger();
    	QSemaphore semaphore = new QSemaphore();
    	QThread thread = new QThread(){
    		@Override
    		protected void run() {
    			QHttpServer httpServer = new QHttpServer();
            	httpServer.route("/", () -> "Hello world");
            	
            	httpServer.route("/query", (QHttpServerRequest request) ->
            		request.value("Host") + "/query/"
        	    );
        	
        	    httpServer.route("/query/<arg>", (Integer id, QHttpServerRequest request) ->
        	        String.format("%s/query/%d", request.value("Host"), id)
        	    );
        	
        	    httpServer.route("/query/<arg>/log", (Integer id, QHttpServerRequest request) ->
        	        String.format("%s/query/%d/log", request.value("Host"), id)
        	    );
        	
        	    httpServer.route("/query/<arg>/log/<arg>", (Integer id, Float threshold, QHttpServerRequest request) ->
        	        String.format("%s/query/%d/log/%f", request.value("Host"), id, threshold)
        	    );
        	    
        	    httpServer.addAfterRequestHandler(httpServer, (QHttpServerRequest req, QHttpServerResponse resp) -> {
                    QHttpHeaders headers = resp.headers();
                    headers.append(QHttpHeaders.WellKnownHeader.Server, "Qt HTTP Server");
                    resp.setHeaders(headers);
                });
        	    
        	    httpServer.setMissingHandler((request, responder)->{
        	    	responder.sendResponse(new QHttpServerResponse(request.query().query(), QHttpServerResponder.StatusCode.BadRequest));
        	    });
        	    QTcpServer tcpServer = new QTcpServer();
                if (!tcpServer.listen() || !httpServer.bind(tcpServer)) {
                    System.err.println(QCoreApplication.translate("QHttpServerExample",
                            "Server failed to listen on a port."));
                    semaphore.release();
                    return;
                }
//    	    	System.out.println("http://127.0.0.1:"+tcpServer.serverPort()+"/query");
                port.set(tcpServer.serverPort());
                semaphore.release();
    			this.exec();
    			tcpServer.dispose();
    			httpServer.dispose();
    		}
    	};
    	try {
	    	thread.start();
	    	semaphore.acquire();
	    	Assert.assertTrue(port.get()!=0);
	    	QNetworkAccessManager mgr = new QNetworkAccessManager();
			QNetworkReply reply = mgr.get(new QNetworkRequest("http://127.0.0.1:"+port.get()+"/"));
			QEventLoop eventLoop = new QEventLoop();
			reply.finished.connect(eventLoop, QEventLoop::quit);
			eventLoop.exec();
			Assert.assertTrue(reply.open(QIODevice.OpenModeFlag.ReadOnly));
			try {
				Assert.assertEquals("Hello world", ""+reply.readAll());
			}finally {
				reply.close();
			}
			reply = mgr.get(new QNetworkRequest("http://127.0.0.1:"+port.get()+"/query"));
			reply.finished.connect(eventLoop, QEventLoop::quit);
			eventLoop.exec();
			Assert.assertTrue(reply.open(QIODevice.OpenModeFlag.ReadOnly));
			try {
				Assert.assertEquals("Qt HTTP Server", reply.header(QNetworkRequest.KnownHeaders.ServerHeader));
				Assert.assertEquals("127.0.0.1:"+port.get()+"/query/", ""+reply.readAll());
			}finally {
				reply.close();
			}
			reply = mgr.get(new QNetworkRequest("http://127.0.0.1:"+port.get()+"/query/5"));
			reply.finished.connect(eventLoop, QEventLoop::quit);
			eventLoop.exec();
			Assert.assertTrue(reply.open(QIODevice.OpenModeFlag.ReadOnly));
			try {
				Assert.assertEquals(String.format("127.0.0.1:%d/query/%d", port.get(), 5), ""+reply.readAll());
			}finally {
				reply.close();
			}
			reply = mgr.get(new QNetworkRequest("http://127.0.0.1:"+port.get()+"/query/5/log/4.3"));
			reply.finished.connect(eventLoop, QEventLoop::quit);
			eventLoop.exec();
			Assert.assertTrue(reply.open(QIODevice.OpenModeFlag.ReadOnly));
			try {
				Assert.assertEquals(String.format("127.0.0.1:%d/query/%d/log/%f", port.get(), 5, 4.3), ""+reply.readAll());
			}finally {
				reply.close();
			}
    	}finally {
			thread.quit();
			thread.join();
		}
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestHttpServer.class.getName());
    }
}
