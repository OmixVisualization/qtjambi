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
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;
import static org.junit.Assume.assumeTrue;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.junit.After;
import org.junit.BeforeClass;

import io.qt.core.QDateTime;
import io.qt.core.Qt;
import io.qt.network.QAbstractSocket;
import io.qt.network.QSslCertificate;
import io.qt.network.QSslSocket;
import io.qt.network.QSslSocket.PeerVerifyMode;

public class TestQSslSocket extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
        ApplicationInitializer.testInitialize();
        try {
        	assumeTrue("Qt does not support SSL", QSslSocket.supportsSsl());
        }catch(Exception e) {
        	ApplicationInitializer.testDispose();
        	throw e;
        }
    }

	private QSslSocket socket;
	private List<QSslCertificate> certs = new ArrayList<QSslCertificate>();
	// on 31-Mar-2012        www.google.com first="Mon Jan 18 23:59:59 2038" second="Sat Oct 25 08:32:46 2036"
	// on 31-Mar-2012 unittest.qt-jambi.org first="Sun Mar 31 13:13:29 2013"
	private QDateTime first  = QDateTime.fromString("2013-03-31T13:13:29+00:00", Qt.DateFormat.ISODate);
	private QDateTime[] date = { first };

	private void setupSocket(String hostname) {
		socket = new QSslSocket();
		socket.connectToHostEncrypted(hostname, (short) 443);
		// We don't care what CA certificate chain the remote end claims we're
		// testing generic SSL functionality is available.
		socket.setPeerVerifyMode(PeerVerifyMode.VerifyNone);
		// block the calling thread until an encrypted connection has been
		// established.
		socket.waitForEncrypted(20000);
	}

	@After
	public void tearDown() throws Exception {
		if(socket != null)
			socket.close();
		socket = null;
	}

	@org.junit.Test
	public void testConnectToHostEncrypted() {
		setupSocket("www.google.com");
		assertEquals("socket.state()", QAbstractSocket.SocketState.ConnectedState, socket.state());
		assertTrue(socket.isValid());
		// Could not use unittest.qt-jambi.org as socket.isEncrypted()==false not sure exactly why,
		//  maybe it doesn't like the CA.
		assertTrue(socket.isEncrypted());
	}

	@org.junit.Test
	public void testConnectToHostWithData() {
		setupSocket("www.google.com");
		assertEquals("socket.state()", socket.state(), QAbstractSocket.SocketState.ConnectedState);
		// another test candidate
		String s = "HEAD / HTTP/1.1\r\nHost: www.google.com\r\nConnection: close\r\n\r\n";
		byte[] bA;
		bA = s.getBytes();
		int n = socket.write(bA);
		assertEquals("write()", bA.length, n);
		socket.flush();

		int totalBytesRead = 0;
		bA = new byte[4096];
		do {
			if(socket.waitForReadyRead(5000) == false)
				break;
			n = socket.read(bA);	// try to fill buffer within 5 seconds
			if(n <= 0)
				break;
			totalBytesRead += n;
		} while(n > 0);
		// we can take the reception of any data as demonstrating SSL working
		assertTrue("read length", (totalBytesRead > 0));
		if(socket.state() != QAbstractSocket.SocketState.UnconnectedState)
			socket.waitForDisconnected(5000);
	}

//	@org.junit.Test
	public void testCaCertificates() {
		// We use a remote host we can control the server side certificates on
		setupSocket("qt.io");
		assertEquals("socket.state()", QAbstractSocket.SocketState.ConnectedState, socket.state());
		certs = socket.peerCertificateChain();
		assertNotNull(certs);
		assertTrue(certs.size() > 0);
		assertEquals(1, certs.size());

		// The problem with www.google.com:443 is they are a large multi-national web operation
		//  using many machine and many systems on the same URL.  So it depends too much on the
		//  country your request originates from, the current certificate supplier policy, the
		//  time of year and the wind direction.  Making this check too brittle for general use.
		// Ideally this should be hosted by unittest.qt-jambi.org so we can control exactly the
		//  server side.  Making the user setup puppet and other things just presents additional
		//  barriers to unittesting.

		certs = certs.subList(0, date.length);
		Iterator<QSslCertificate> i = certs.iterator();
		int j = 0;
		
		while (i.hasNext()) {
			QSslCertificate cert = i.next();
			QDateTime expDate = date[j];
			// convert local time to UTC since certification exp. date is given
			// in UTC too
			expDate.setTimeSpec(Qt.TimeSpec.UTC);

			String desc = "cert[" + Integer.valueOf(j).toString() + "]";
			assertFalse(desc + ".isBlacklisted()", cert.isBlacklisted());
			assertEquals(desc + ".cert.expiryDate()" , expDate, cert.expiryDate());

			j++;
		}
		assertEquals(date.length, j);  // check we looped and checked everything
	}
}
