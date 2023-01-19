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

import org.junit.Assert;
import org.junit.Test;

import io.qt.core.QBuffer;
import io.qt.core.QByteArray;
import io.qt.core.QIODevice;
import io.qt.core.QMetaObject;
import io.qt.core.QObject;
import io.qt.scxml.QScxmlEvent;
import io.qt.scxml.QScxmlStateMachine;

public class TestScxml extends ApplicationInitializer {
	
    @Test
    public void test() {
    	QBuffer data = new QBuffer();
    	data.setBuffer(new QByteArray("<scxml xmlns=\"http://www.w3.org/2005/07/scxml\"\n" + 
    			"              version=\"1.0\"\n" + 
    			"              initial=\"hello\">\n" + 
    			"\n" + 
    			"     <final id=\"hello\">\n" + 
    			"      <onentry>\n" + 
    			"       <log expr=\"'hello world'\" />\n" + 
    			"      </onentry>\n" + 
    			"     </final>\n" + 
    			"\n" + 
    			"    </scxml>"));
    	Assert.assertTrue(data.open(QIODevice.OpenModeFlag.ReadOnly));
    	QScxmlStateMachine sm = QScxmlStateMachine.fromData(data);
    	QMetaObject.Connection connection = sm.connectToState("hello", ()->{});
    	Assert.assertTrue("not connected", connection.isConnected());
    	connection = sm.connectToState("hello", b->{});
    	Assert.assertTrue("not connected", connection.isConnected());
    	connection = sm.connectToEvent("hello", ()->{});
    	Assert.assertTrue("not connected", connection.isConnected());
    	connection = sm.connectToEvent("hello", event->{});
    	Assert.assertTrue("not connected", connection.isConnected());
    	class Receiver extends QObject{
    		void receive(){}
    		void receiveBoolean(boolean b){}
    		void receiveEvent(QScxmlEvent e){}
    	}
    	Receiver r = new Receiver();
    	connection = sm.connectToState("hello", r::receive);
    	Assert.assertTrue("not connected", connection.isConnected());
    	connection = sm.connectToState("hello", r::receiveBoolean);
    	Assert.assertTrue("not connected", connection.isConnected());
    	connection = sm.connectToEvent("hello", r::receive);
    	Assert.assertTrue("not connected", connection.isConnected());
    	connection = sm.connectToEvent("hello", r::receiveEvent);
    	Assert.assertTrue("not connected", connection.isConnected());
    	connection = sm.connectToState("hello", r, "receive()");
    	Assert.assertTrue("not connected", connection.isConnected());
    	connection = sm.connectToState("hello", r, "receiveBoolean(boolean)");
    	Assert.assertTrue("not connected", connection.isConnected());
    	connection = sm.connectToEvent("hello", r, "receive()");
    	Assert.assertTrue("not connected", connection.isConnected());
    	connection = sm.connectToEvent("hello", r, "receiveEvent(QScxmlEvent)");
    	Assert.assertTrue("not connected", connection.isConnected());
    	data.close();
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestScxml.class.getName());
    }
}
