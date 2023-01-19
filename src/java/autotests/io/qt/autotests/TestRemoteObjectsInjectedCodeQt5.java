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

import org.junit.Test;

import io.qt.core.QBuffer;
import io.qt.core.QByteArray;
import io.qt.core.QIODevice;
import io.qt.remoteobjects.ClientIoDevice;
import io.qt.remoteobjects.IoDeviceBase.ReadResult;
import io.qt.remoteobjects.ServerIoDevice;

public class TestRemoteObjectsInjectedCodeQt5 extends ApplicationInitializer {
	@Test
    public void testIoDeviceBase()
    {
    	QByteArray data = new QByteArray();
    	QBuffer serverBuffer = new QBuffer(data);
    	serverBuffer.open(QIODevice.OpenModeFlag.ReadWrite);
    	QBuffer clientBuffer = new QBuffer(data);
    	clientBuffer.open(QIODevice.OpenModeFlag.ReadWrite);
    	
    	ServerIoDevice server = new ServerIoDevice() {
			
			@Override
			protected void doClose() {
				serverBuffer.close();
			}
			
			@Override
			public QIODevice connection() {
				return serverBuffer;
			}
		};
		server.stream().setDevice(serverBuffer);
		
    	ClientIoDevice client = new ClientIoDevice() {
			
			@Override
			protected void doClose() {
				clientBuffer.close();
			}
			
			@Override
			public QIODevice connection() {
				return clientBuffer;
			}
			
			@Override
			protected void doDisconnectFromServer() {
			}
			
			@Override
			public void connectToServer() {
			}
		};
		client.stream().setDevice(clientBuffer);
		server.write(new QByteArray("TEST"));
		@SuppressWarnings("unused")
		ReadResult result = client.read();
//		System.out.println(result);
		serverBuffer.close();
		clientBuffer.close();
		server.dispose();
		client.dispose();
		serverBuffer.dispose();
		clientBuffer.dispose();
    }
}
