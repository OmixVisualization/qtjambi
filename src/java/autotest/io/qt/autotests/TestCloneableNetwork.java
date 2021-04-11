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

package io.qt.autotests;

import static org.junit.Assert.*;
import org.junit.*;
import io.qt.network.*;
import io.qt.core.QDateTime;
import io.qt.internal.*;

public class TestCloneableNetwork extends QApplicationTest {
    @Test
    public void run_clone_QAuthenticator() {
        QAuthenticator org = new QAuthenticator();
        QAuthenticator clone = org.clone();
		assertEquals(QtJambiInternal.Ownership.Java, QtJambiInternal.ownership(clone));
        org.dispose();
        QAuthenticator clone2 = clone.clone();
		assertEquals(QtJambiInternal.Ownership.Java, QtJambiInternal.ownership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QHostAddress() {
        QHostAddress org = new QHostAddress();
        QHostAddress clone = org.clone();
		assertEquals(QtJambiInternal.Ownership.Java, QtJambiInternal.ownership(clone));
        org.dispose();
        QHostAddress clone2 = clone.clone();
		assertEquals(QtJambiInternal.Ownership.Java, QtJambiInternal.ownership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QHostInfo() {
        QHostInfo org = new QHostInfo();
        QHostInfo clone = org.clone();
		assertEquals(QtJambiInternal.Ownership.Java, QtJambiInternal.ownership(clone));
        org.dispose();
        QHostInfo clone2 = clone.clone();
		assertEquals(QtJambiInternal.Ownership.Java, QtJambiInternal.ownership(clone2));
        assertEquals(clone.hostName(), clone2.hostName());
    }

    @Test
    public void run_clone_QNetworkAddressEntry() {
        QNetworkAddressEntry org = new QNetworkAddressEntry();
        QNetworkAddressEntry clone = org.clone();
		assertEquals(QtJambiInternal.Ownership.Java, QtJambiInternal.ownership(clone));
        org.dispose();
        QNetworkAddressEntry clone2 = clone.clone();
		assertEquals(QtJambiInternal.Ownership.Java, QtJambiInternal.ownership(clone2));
        assertEquals(clone.broadcast(), clone2.broadcast());
    }

    @Test
    public void run_clone_QNetworkCookie() {
        QNetworkCookie org = new QNetworkCookie();
        QNetworkCookie clone = org.clone();
		assertEquals(QtJambiInternal.Ownership.Java, QtJambiInternal.ownership(clone));
        org.dispose();
        QNetworkCookie clone2 = clone.clone();
		assertEquals(QtJambiInternal.Ownership.Java, QtJambiInternal.ownership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QNetworkInterface() {
        QNetworkInterface org = new QNetworkInterface();
        QNetworkInterface clone = org.clone();
		assertEquals(QtJambiInternal.Ownership.Java, QtJambiInternal.ownership(clone));
        org.dispose();
        QNetworkInterface clone2 = clone.clone();
		assertEquals(QtJambiInternal.Ownership.Java, QtJambiInternal.ownership(clone2));
        assertEquals(clone.toString(), clone2.toString());
    }

    @Test
    public void run_clone_QNetworkProxy() {
        QNetworkProxy org = new QNetworkProxy();
        QNetworkProxy clone = org.clone();
		assertEquals(QtJambiInternal.Ownership.Java, QtJambiInternal.ownership(clone));
        org.dispose();
        QNetworkProxy clone2 = clone.clone();
		assertEquals(QtJambiInternal.Ownership.Java, QtJambiInternal.ownership(clone2));
        assertEquals(clone.hostName(), clone2.hostName());
    }

    @Test
    public void run_clone_QNetworkRequest() {
        QNetworkRequest org = new QNetworkRequest();
        QNetworkRequest clone = org.clone();
		assertEquals(QtJambiInternal.Ownership.Java, QtJambiInternal.ownership(clone));
        org.dispose();
        QNetworkRequest clone2 = clone.clone();
		assertEquals(QtJambiInternal.Ownership.Java, QtJambiInternal.ownership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
	public void run_clone_QIPv6Address() {
		for(QHostAddress address : QNetworkInterface.allAddresses()){
			QIPv6Address org = address.toIPv6Address();
			QIPv6Address clone = org.clone();
			assertEquals(QtJambiInternal.Ownership.Java, QtJambiInternal.ownership(clone));
			org.dispose();
			QIPv6Address clone2 = clone.clone();
			assertEquals(QtJambiInternal.Ownership.Java, QtJambiInternal.ownership(clone2));
			assertArrayEquals(clone.c(), clone2.c());
		}
	}

	@Test
	public void run_clone_QNetworkCacheMetaData() {
		QNetworkCacheMetaData org = new QNetworkCacheMetaData();
		org.setLastModified(QDateTime.currentDateTimeUtc());
		QNetworkCacheMetaData clone = org.clone();
		assertEquals(QtJambiInternal.Ownership.Java, QtJambiInternal.ownership(clone));
		org.dispose();
		QNetworkCacheMetaData clone2 = clone.clone();
		assertEquals(QtJambiInternal.Ownership.Java, QtJambiInternal.ownership(clone2));
		assertEquals(clone.lastModified(), clone2.lastModified());
	}

	@Test
	public void run_clone_QNetworkProxyQuery() {
		QNetworkProxyQuery org = new QNetworkProxyQuery();
		org.setPeerHostName("test.org");
		QNetworkProxyQuery clone = org.clone();
		assertEquals(QtJambiInternal.Ownership.Java, QtJambiInternal.ownership(clone));
		org.dispose();
		QNetworkProxyQuery clone2 = clone.clone();
		assertEquals(QtJambiInternal.Ownership.Java, QtJambiInternal.ownership(clone2));
		assertEquals(clone.peerHostName(), "test.org");
		assertEquals(clone.peerHostName(), clone2.peerHostName());
	}
}
