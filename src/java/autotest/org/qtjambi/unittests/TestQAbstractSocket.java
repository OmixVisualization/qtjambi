package org.qtjambi.unittests;

import static org.junit.Assert.assertTrue;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.network.QAbstractSocket;

public class TestQAbstractSocket extends QApplicationTest {

	QAbstractSocket socket = null;
	
	@Before
	public void setUp() throws Exception {
		socket = new QAbstractSocket(QAbstractSocket.SocketType.TcpSocket, new QObject());
	}

	@After
	public void tearDown() throws Exception {
		if(socket != null)
			socket.close();
		socket = null;
	}

	@Test
	public void testConnectToHost() {
		socket.connectToHost("http://www.google.com", 80);
		assertTrue(socket.isValid());
	}

}
