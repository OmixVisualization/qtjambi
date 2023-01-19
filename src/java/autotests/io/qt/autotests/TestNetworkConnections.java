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
import static org.junit.Assert.assertTrue;

import java.util.List;

import org.junit.Test;

import io.qt.QtPointerType;
import io.qt.autotests.generated.AbstractSocketSubclass;
import io.qt.core.QObject;
import io.qt.network.QAuthenticator;
import io.qt.network.QNetworkAccessManager;
import io.qt.network.QNetworkReply;


public class TestNetworkConnections extends ApplicationInitializer
{
	static class Auth extends QObject{
		QAuthenticator authenticator;
		void onAuthenticationRequired(QNetworkReply reply, @QtPointerType QAuthenticator authenticator)
		{
			authenticator.setUser("TEST");
			authenticator.setPassword("PW");
			this.authenticator = authenticator;
		}
		
		void onAuthenticationRequired2(QNetworkReply reply, QAuthenticator authenticator)
		{
			authenticator.setUser("TEST");
			authenticator.setPassword("PW");
			this.authenticator = authenticator;
		}
	}
    
	@Test
    public void testAuthenticatorPointerJavaCall() {
		Auth auth = new Auth();
//		auth.metaObject().methods().forEach(m->System.out.println(m.cppMethodSignature()));
    	QNetworkAccessManager accessManager = new QNetworkAccessManager();
    	assertTrue(accessManager.authenticationRequired.connect(auth::onAuthenticationRequired)!=null);
    	QAuthenticator authenticator = new QAuthenticator();
    	assertEquals("", authenticator.user());
    	assertEquals("", authenticator.password());
    	accessManager.authenticationRequired.emit(null, authenticator);
    	assertEquals("TEST", authenticator.user());
    	assertEquals("PW", authenticator.password());
    }
	
	@Test
    public void testAuthenticatorJavaCall() {
		Auth auth = new Auth();
//		auth.metaObject().methods().forEach(m->System.out.println(m.cppMethodSignature()));
    	QNetworkAccessManager accessManager = new QNetworkAccessManager();
    	assertTrue(accessManager.authenticationRequired.connect(auth::onAuthenticationRequired2)!=null);
    	QAuthenticator authenticator = new QAuthenticator();
    	assertEquals("", authenticator.user());
    	assertEquals("", authenticator.password());
    	accessManager.authenticationRequired.emit(null, authenticator);
    	assertEquals("TEST", authenticator.user());
    	assertEquals("PW", authenticator.password());
    }
	
	@Test
    public void testAuthenticatorPointerCppCall() {
		Auth auth = new Auth();
    	QNetworkAccessManager accessManager = new QNetworkAccessManager();
    	assertTrue(accessManager.authenticationRequired.connect(auth::onAuthenticationRequired)!=null);
    	List<String> result = AbstractSocketSubclass.emitAuthenticationRequired(accessManager, null);
    	assertEquals(4, result.size());
    	assertEquals("", result.get(0));
    	assertEquals("", result.get(1));
    	assertEquals("TEST", result.get(2));
    	assertEquals("PW", result.get(3));
    	assertTrue(auth.authenticator!=null);
    	assertTrue(auth.authenticator.isDisposed());
    }
	
	@Test
    public void testAuthenticatorCppCall() {
		Auth auth = new Auth();
    	QNetworkAccessManager accessManager = new QNetworkAccessManager();
    	assertTrue(accessManager.authenticationRequired.connect(auth::onAuthenticationRequired2)!=null);
    	List<String> result = AbstractSocketSubclass.emitAuthenticationRequired(accessManager, null);
    	assertEquals(4, result.size());
    	assertEquals("", result.get(0));
    	assertEquals("", result.get(1));
    	assertEquals("TEST", result.get(2));
    	assertEquals("PW", result.get(3));
    	assertTrue(auth.authenticator!=null);
    	assertTrue(auth.authenticator.isDisposed());
    }

    public static void main(String args[]) {
    	org.junit.runner.JUnitCore.main(TestNetworkConnections.class.getName());
    }
}
