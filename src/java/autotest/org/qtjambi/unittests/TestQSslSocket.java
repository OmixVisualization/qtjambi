package org.qtjambi.unittests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.junit.Assume.assumeTrue;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.junit.After;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.experimental.categories.Category;

import org.qtjambi.autotests.QApplicationTest;
import org.qtjambi.qt.core.QDateTime;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.network.QAbstractSocket;
import org.qtjambi.qt.network.QSslCertificate;
import org.qtjambi.qt.network.QSslSocket;
import org.qtjambi.qt.network.QSslSocket.PeerVerifyMode;
import org.qtjambi.unittests.support.CategorySsl;
import org.qtjambi.unittests.support.FilterSsl;

@Category(CategorySsl.class)
public class TestQSslSocket extends QApplicationTest {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
        assumeTrue(FilterSsl.detectStatic());
        QApplicationTest.testInitialize(null);
    }

	private QSslSocket socket;
	private List<QSslCertificate> certs = new ArrayList<QSslCertificate>();
	// on 31-Mar-2012        www.google.com first="Mon Jan 18 23:59:59 2038" second="Sat Oct 25 08:32:46 2036"
	// on 31-Mar-2012 unittest.qt-jambi.org first="Sun Mar 31 13:13:29 2013"
	private QDateTime first  = QDateTime.fromString("2013-03-31T13:13:29+00:00", Qt.DateFormat.ISODate);
	private QDateTime[] date = { first };

	private static final String K_host_google_com			= "www.google.com";
	private static final String K_host_unittest_qt_jambi_org	= "unittest.qt-jambi.org";

	private void setupSocket(String hostname) {
		socket = new QSslSocket(new QObject());
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
	public void testSupportsSsl() {
		assertTrue(QSslSocket.supportsSsl());
	}

	@org.junit.Test
	public void testConnectToHostEncrypted() {
		setupSocket(K_host_google_com);
		assertEquals("socket.state()", QAbstractSocket.SocketState.ConnectedState, socket.state());
		assertTrue(socket.isValid());
		// Could not use unittest.qt-jambi.org as socket.isEncrypted()==false not sure exactly why,
		//  maybe it doesn't like the CA.
		assertTrue(socket.isEncrypted());
	}

	@org.junit.Test
	public void testConnectToHostWithData() {
		setupSocket(K_host_google_com);
		assertEquals("socket.state()", socket.state(), QAbstractSocket.SocketState.ConnectedState);
		// another test candidate
		String s = "HEAD / HTTP/1.1\r\nHost: " + K_host_google_com + "\r\nConnection: close\r\n\r\n";
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

	@org.junit.Test
	public void testCaCertificates() {
		// We use a remote host we can control the server side certificates on
		setupSocket(K_host_unittest_qt_jambi_org);
		assertEquals("socket.state()", socket.state(), QAbstractSocket.SocketState.ConnectedState);
		certs = socket.peerCertificateChain();
		assertNotNull(certs);
		assertTrue(certs.size() > 0);
		assertEquals(certs.size(), 1);

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
