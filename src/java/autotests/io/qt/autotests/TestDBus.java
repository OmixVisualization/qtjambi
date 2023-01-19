/****************************************************************************
**
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

import java.util.List;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QList;
import io.qt.core.QMetaObject;
import io.qt.dbus.QDBusConnection;
import io.qt.dbus.QDBusInterface;
import io.qt.dbus.QDBusMessage;
import io.qt.dbus.QDBusPendingCall;
import io.qt.dbus.QDBusPendingReply;
import io.qt.dbus.QDBusReply;

public class TestDBus extends ApplicationInitializer {
	
	@BeforeClass
	public static void testInitialize() throws Exception {
    	Assume.assumeTrue("QDBus session bus is not connected.", QDBusConnection.sessionBus().isConnected());
		ApplicationInitializer.testInitialize();
	}
	
	private static class DBusInterfaceSubclass1 extends QDBusInterface{
		public DBusInterfaceSubclass1() {
			super("org.freedesktop.DBus", "/org/freedesktop/DBus",
						"org.freedesktop.DBus", QDBusConnection.sessionBus());
		}
	}
	
	private static class DBusInterfaceSubclass2 extends QDBusInterface{
		@SuppressWarnings("unused")
		public final Signal0 testSignal = new Signal0();
		public DBusInterfaceSubclass2() {
			super("org.freedesktop.DBus", "/org/freedesktop/DBus",
						"org.freedesktop.DBus", QDBusConnection.sessionBus());
		}
	}
	
	@Test
    public void testSubclassing() {
		Assert.assertEquals(QMetaObject.forType(QDBusInterface.class), QDBusInterface.staticMetaObject);
		Assert.assertEquals(QMetaObject.forType(DBusInterfaceSubclass1.class), QDBusInterface.staticMetaObject);
		new DBusInterfaceSubclass1();
		try {
			QMetaObject.forType(DBusInterfaceSubclass2.class);
			Assert.fail("UnsupportedOperationException expected to be thrown");
		}catch(UnsupportedOperationException e) {}
		try {
			new DBusInterfaceSubclass2();
			Assert.fail("UnsupportedOperationException expected to be thrown");
		}catch(UnsupportedOperationException e) {}
	}
	
    @Test
    public void test_call_QDBusPendingReply() {
	    QDBusInterface dbus_iface = new QDBusInterface("org.freedesktop.DBus", "/org/freedesktop/DBus",
	                              						"org.freedesktop.DBus", QDBusConnection.sessionBus());
	    QDBusMessage msg = dbus_iface.call("ListNames");
	    Assert.assertTrue(msg!=null);
	    Assert.assertTrue(msg.arguments().size()>0);
	    Assert.assertTrue(msg.errorMessage(), msg.errorMessage().isEmpty());
	    QDBusPendingReply<List<String>> preply = QDBusPendingReply.newStringListInstance(msg);
	    Assert.assertFalse(preply.error().message(), preply.isError());
	    Assert.assertTrue(preply.value() instanceof QList);
    }
    
    @Test
    public void test_asyncCall_QDBusPendingReply() {
	    QDBusInterface dbus_iface = new QDBusInterface("org.freedesktop.DBus", "/org/freedesktop/DBus",
	                              						"org.freedesktop.DBus", QDBusConnection.sessionBus());
	    QDBusPendingCall msg = dbus_iface.asyncCall("ListNames");
	    Assert.assertTrue(msg!=null);
		QDBusPendingReply<List<String>> preply = QDBusPendingReply.newStringListInstance(msg);
	    Assert.assertFalse(preply.error().message(), preply.isError());
	    Assert.assertTrue(preply.value() instanceof QList);
    }
    
    
    @Test
    public void test_QDBusReply() {
		QDBusReply<List<String>> reply = QDBusConnection.sessionBus().connectionInterface().registeredServiceNames();
		Assert.assertTrue(reply!=null);
	    Assert.assertFalse(reply.error().message(), reply.error().isValid());
	    Assert.assertTrue(reply.value() instanceof QList);
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestDBus.class.getName());
    }
}
