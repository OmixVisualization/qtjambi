package io.qt.autotests;

import static org.junit.Assert.*;
import org.junit.*;
import io.qt.core.*;
import io.qt.network.*;
import java.util.*;


public class TestNetwork extends ApplicationInitializer {
    
    @Test
    public void test_QHostAddress_Equals()
    {
        QHostAddress address1 = new QHostAddress(QHostAddress.SpecialAddress.LocalHost);
        QHostAddress address2 = new QHostAddress(QHostAddress.SpecialAddress.LocalHost);
        QHostAddress address3 = new QHostAddress(QHostAddress.SpecialAddress.Broadcast);
        QByteArray array = new QByteArray("127.0.0.1");

        assertFalse(address1 == address2);
        assertFalse(address2 == address3);
        assertTrue(address1.equals(address2));
        assertTrue(address2.equals(address1));
        assertTrue(address3.equals(address3));
        assertEquals(false, address1.equals(address3));
        assertFalse(address2.equals(array));
    }

    @Test
    public void test_QHostAddress_HashCodeAndEquals()
    {
        Hashtable<QHostAddress, QByteArray> address_hash = new Hashtable<QHostAddress, QByteArray>();

        QHostAddress address1 = new QHostAddress(QHostAddress.SpecialAddress.LocalHost);
        QHostAddress address2 = new QHostAddress(QHostAddress.SpecialAddress.LocalHost);
        QHostAddress address3 = new QHostAddress(QHostAddress.SpecialAddress.Broadcast);

        QByteArray ba_address1 = new QByteArray("127.0.0.1 - 1");
        QByteArray ba_address2 = new QByteArray("127.0.0.1 - 2");
        QByteArray ba_address3 = new QByteArray("255.255.255.255");

        address_hash.put(address1, ba_address1);
        assertFalse(address_hash.containsKey(new QHostAddress(QHostAddress.SpecialAddress.Broadcast)));
        assertTrue(address_hash.containsKey(new QHostAddress(QHostAddress.SpecialAddress.LocalHost)));
        assertTrue(address_hash.get(new QHostAddress(QHostAddress.SpecialAddress.LocalHost)) == ba_address1);

        address_hash.put(address2, ba_address2); // overwrites the first entry of this type
        address_hash.put(address3, ba_address3);
        assertTrue(address_hash.containsKey(address1));
        assertTrue(address_hash.containsKey(new QHostAddress(QHostAddress.SpecialAddress.Broadcast)));

        QHostAddress lookup_key1 = new QHostAddress(QHostAddress.SpecialAddress.LocalHost);
        QHostAddress lookup_key2 = new QHostAddress(QHostAddress.SpecialAddress.Broadcast);

        QByteArray value = address_hash.get(lookup_key1);
        assertTrue(value == ba_address2);

        value = address_hash.get(lookup_key2);
        assertTrue(value == ba_address3);
    }
    
    @Test
	public void testConnectToHost() {
    	QAbstractSocket socket = new QAbstractSocket(QAbstractSocket.SocketType.TcpSocket, null);
    	try {
			socket.connectToHost("http://www.google.com", 80);
			assertTrue(socket.isValid());
    	}finally {
    		socket.close();
    	}
	}
}
