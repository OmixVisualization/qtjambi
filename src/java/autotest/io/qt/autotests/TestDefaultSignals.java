/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import io.qt.core.QObject;

public class TestDefaultSignals extends QApplicationTest {
    private static class Sender extends QObject {
    	public final Signal2Default1<String, Integer> signal21 = new Signal2Default1<>(()->7654);
    	public final Signal2Default2<String, Integer> signal22 = new Signal2Default2<>(()->"No arg", ()->12345);
    }
    
    private static class Receiver{
    	boolean received;
    	String received1;
    	Integer received2;
    	
    	void receive2(String s, int i) {
    		received1 = s;
    		received2 = i;
    		received = true;
    	}
    	
    	void receive1(String s) {
    		received1 = s;
    		received = true;
    	}
    	
    	void receive0() {
    		received = true;
    	}
    	
    	void reset() {
    		received1 = null;
    		received2 = null;
    		received = false;
    	}
    }

    @Test
    public void test_emit_DefaultSignal() {
    	Sender sender = new Sender();
    	Receiver receiver = new Receiver();
    	assertTrue(sender.signal22.connect(receiver::receive2)!=null);
    	assertTrue(sender.signal21.connect(receiver::receive1)!=null);
    	
    	assertNull(receiver.received1);
    	assertNull(receiver.received2);
    	assertFalse(receiver.received);
    	sender.signal22.emit("TEST", 5);
    	assertEquals("TEST", receiver.received1);
    	assertEquals(Integer.valueOf(5), receiver.received2);
    	assertTrue(receiver.received);
    	receiver.reset();
    	
    	assertNull(receiver.received1);
    	assertNull(receiver.received2);
    	assertFalse(receiver.received);
    	sender.signal22.emit("TEST");
    	assertEquals("TEST", receiver.received1);
    	assertEquals(Integer.valueOf(12345), receiver.received2);
    	assertTrue(receiver.received);
    	receiver.reset();
    	
    	assertNull(receiver.received1);
    	assertNull(receiver.received2);
    	assertFalse(receiver.received);
    	sender.signal22.emit();
    	assertEquals("No arg", receiver.received1);
    	assertEquals(Integer.valueOf(12345), receiver.received2);
    	assertTrue(receiver.received);
    	receiver.reset();
    	
    	assertNull(receiver.received1);
    	assertNull(receiver.received2);
    	assertFalse(receiver.received);
    	sender.signal21.emit("TEST", 5);
    	assertEquals("TEST", receiver.received1);
    	assertNull(receiver.received2);
    	assertTrue(receiver.received);
    	receiver.reset();
    	
    	assertNull(receiver.received1);
    	assertNull(receiver.received2);
    	assertFalse(receiver.received);
    	sender.signal21.emit("TEST");
    	assertEquals("TEST", receiver.received1);
    	assertNull(receiver.received2);
    	assertTrue(receiver.received);
    	receiver.reset();
    	
    	assertTrue(sender.signal22.disconnect(receiver::receive2));
    	assertTrue(sender.signal21.disconnect(receiver::receive1));
    	
    	assertNull(receiver.received1);
    	assertNull(receiver.received2);
    	assertFalse(receiver.received);
    	sender.signal21.emit("TEST");
    	sender.signal22.emit("TEST");
    	assertNull(receiver.received1);
    	assertNull(receiver.received2);
    	assertFalse(receiver.received);

    	assertTrue(sender.signal22.connect(receiver::receive1)!=null);
    	assertTrue(sender.signal21.connect(receiver::receive0)!=null);
    	
    	assertNull(receiver.received1);
    	assertNull(receiver.received2);
    	assertFalse(receiver.received);
    	sender.signal22.emit("TEST", 5);
    	assertEquals("TEST", receiver.received1);
    	assertNull(receiver.received2);
    	assertTrue(receiver.received);
    	receiver.reset();
    	
    	assertNull(receiver.received1);
    	assertNull(receiver.received2);
    	assertFalse(receiver.received);
    	sender.signal22.emit("TEST");
    	assertEquals("TEST", receiver.received1);
    	assertNull(receiver.received2);
    	assertTrue(receiver.received);
    	receiver.reset();
    	
    	assertNull(receiver.received1);
    	assertNull(receiver.received2);
    	assertFalse(receiver.received);
    	sender.signal22.emit();
    	assertEquals("No arg", receiver.received1);
    	assertNull(receiver.received2);
    	assertTrue(receiver.received);
    	receiver.reset();
    	
    	assertNull(receiver.received1);
    	assertNull(receiver.received2);
    	assertFalse(receiver.received);
    	sender.signal21.emit("TEST", 5);
    	assertNull(receiver.received1);
    	assertNull(receiver.received2);
    	assertTrue(receiver.received);
    	receiver.reset();
    	
    	assertNull(receiver.received1);
    	assertNull(receiver.received2);
    	assertFalse(receiver.received);
    	sender.signal21.emit("TEST");
    	assertNull(receiver.received1);
    	assertNull(receiver.received2);
    	assertTrue(receiver.received);
    	receiver.reset();    	
    	
    	assertTrue(sender.signal22.disconnect(receiver::receive1));
    	assertTrue(sender.signal21.disconnect(receiver::receive0));
    	
    	assertNull(receiver.received1);
    	assertNull(receiver.received2);
    	assertFalse(receiver.received);
    	sender.signal21.emit("TEST");
    	sender.signal22.emit("TEST");
    	assertNull(receiver.received1);
    	assertNull(receiver.received2);
    	assertFalse(receiver.received);

    	assertTrue(sender.signal22.connect(receiver::receive0)!=null);
    	
    	assertNull(receiver.received1);
    	assertNull(receiver.received2);
    	assertFalse(receiver.received);
    	sender.signal22.emit("TEST", 5);
    	assertNull(receiver.received1);
    	assertNull(receiver.received2);
    	assertTrue(receiver.received);
    	receiver.reset();
    	
    	assertNull(receiver.received1);
    	assertNull(receiver.received2);
    	assertFalse(receiver.received);
    	sender.signal22.emit("TEST");
    	assertNull(receiver.received1);
    	assertNull(receiver.received2);
    	assertTrue(receiver.received);
    	receiver.reset();
    	
    	assertNull(receiver.received1);
    	assertNull(receiver.received2);
    	assertFalse(receiver.received);
    	sender.signal22.emit();
    	assertNull(receiver.received1);
    	assertNull(receiver.received2);
    	assertTrue(receiver.received);
    	receiver.reset();
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestDefaultSignals.class.getName());
    }
}
