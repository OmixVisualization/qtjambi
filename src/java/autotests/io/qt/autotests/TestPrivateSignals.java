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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.lang.reflect.Field;

import org.junit.Assert;
import org.junit.Test;

import io.qt.QSignalAccessException;
import io.qt.QtObject;
import io.qt.QtSignalEmitterInterface;
import io.qt.core.QAbstractItemModel;
import io.qt.core.QInstanceMemberSignals;
import io.qt.core.QMetaObject;
import io.qt.core.QModelIndex;
import io.qt.core.QObject;
import io.qt.core.QStaticMemberSignals;

public class TestPrivateSignals extends ApplicationInitializer {
    private static class DummyModel extends QAbstractItemModel {
        public int columnCount(QModelIndex parent) {
            return 1;
        }
        public int rowCount(QModelIndex parent) {
            return 1;
        }
        public Object data(QModelIndex index, int role) {
            return null;
        }
        public QModelIndex index(int row, int column, QModelIndex parent) {
            return createIndex(row, column);
        }
        public QModelIndex parent(QModelIndex child) {
            return null;
        }
        public void doReset() {
        	beginResetModel();
        	endResetModel();
        }
    }

    private boolean triggered_modelReset;
    private boolean triggered_modelAboutToBeReset;

    public void handle_modelReset() {
        triggered_modelReset = true & triggered_modelAboutToBeReset;
    }
    public void handle_modelAboutToBeReset() {
        triggered_modelAboutToBeReset = true;
    }

    @Test
    public void test_emit_PrivateSignal0() {
        DummyModel model = new DummyModel();

        triggered_modelReset = false;
        triggered_modelAboutToBeReset = false;

        model.doReset();

        assertFalse(triggered_modelReset);
        assertFalse(triggered_modelAboutToBeReset);

        model.modelReset.connect(this::handle_modelReset);
        model.modelAboutToBeReset.connect(this::handle_modelAboutToBeReset);

        model.doReset();

        assertTrue(triggered_modelAboutToBeReset);
        assertTrue(triggered_modelReset);
    }

    @Test
    public void test_privateSignal_is_PrivateSignal() {
        Field signal = null;
        try {
            signal = QAbstractItemModel.class.getField("modelReset");
        } catch (Exception e) { }

        assertTrue(signal != null);
        assertEquals(signal.getType().getName(), io.qt.core.QObject.PrivateSignal0.class.getName());
    }
    
    private static class PrivateSignalOwnerQObject extends QObject {
    	public final PrivateSignal1<String> privateSignal = new PrivateSignal1<>();
		
		public void emitString(String s) {
			emit(privateSignal, s);
		}
	}
    
    private static class PrivateSignalOwnerObject implements QInstanceMemberSignals, QtSignalEmitterInterface{
    	public final PrivateSignal1<String> privateSignal = new PrivateSignal1<>(this);
		
		public void emitString(String s) {
			QInstanceMemberSignals.emit(privateSignal, s);
		}
	}
    
    private static class PrivateStaticSignalOwner{
    	public static final QStaticMemberSignals.PrivateSignal1<String> privateStaticSignal = new QStaticMemberSignals.PrivateSignal1<>();
		public static void emitStaticString(String s) {
			QStaticMemberSignals.emit(privateStaticSignal, s);
		}
    }
    
    private static class PrivateStaticSignalOwnerQObject extends QObject{
    	public static final QStaticMemberSignals.PrivateSignal1<String> privateStaticSignal = new QStaticMemberSignals.PrivateSignal1<>();
		public static void emitStaticString(String s) {
			QStaticMemberSignals.emit(privateStaticSignal, s);
		}
    }
	
	private static class PrivateSignalAccessor extends QObject{
		static void emitPrivateSignal(PrivateSignal1<String> privateSignal, String s) {
			emit(privateSignal, s);
		}
	}
    
    @Test
    public void test_emit_private_signal_QObject() {
    	PrivateSignalOwnerQObject o = new PrivateSignalOwnerQObject();
    	String[] result = {null};
    	QMetaObject.Connection con = o.privateSignal.connect(s->result[0] = s);
    	assertTrue(con.isConnected());
    	assertTrue(con instanceof QtObject);
    	o.emitString("test");
    	assertEquals("test", result[0]);
    }
    
    @Test
    public void test_external_emit_private_signal_QObject() {
    	PrivateSignalOwnerQObject o = new PrivateSignalOwnerQObject();
    	String[] result = {null};
    	QMetaObject.Connection con = o.privateSignal.connect(s->result[0] = s);
    	assertTrue(con.isConnected());
    	assertTrue(con instanceof QtObject);
    	try{
        	PrivateSignalAccessor.emitPrivateSignal(o.privateSignal, "test");
    		Assert.fail("QSignalAccessException expected to be thrown.");
    	}catch(QSignalAccessException e) {
    	}
    	assertEquals(null, result[0]);
    }
    
    @Test
    public void test_emit_private_signal_InstanceMember() {
    	PrivateSignalOwnerObject o = new PrivateSignalOwnerObject();
    	String[] result = {null};
    	QMetaObject.Connection con = o.privateSignal.connect(s->result[0] = s);
    	assertTrue(con.isConnected());
    	assertFalse(con instanceof QtObject);
    	o.emitString("test");
    	assertEquals("test", result[0]);
    }
    
    @Test
    public void test_external_emit_private_signal_InstanceMember() {
    	PrivateSignalOwnerObject o = new PrivateSignalOwnerObject();
    	String[] result = {null};
    	QMetaObject.Connection con = o.privateSignal.connect(s->result[0] = s);
    	assertTrue(con.isConnected());
    	assertFalse(con instanceof QtObject);
    	try{
    		QInstanceMemberSignals.emit(o.privateSignal, "test");
    		Assert.fail("QSignalAccessException expected to be thrown.");
    	}catch(QSignalAccessException e) {
    	}
    	assertEquals(null, result[0]);
    }
    
    @Test
    public void test_emit_private_signal_static() {
    	String[] result = {null};
    	QMetaObject.Connection con = PrivateStaticSignalOwner.privateStaticSignal.connect(s->result[0] = s);
    	assertTrue(con.isConnected());
    	assertFalse(con instanceof QtObject);
    	PrivateStaticSignalOwner.emitStaticString("statictest");
    	assertEquals("statictest", result[0]);
    }
    
    @Test
    public void test_emit_private_signal_static_QObject() {
    	new PrivateStaticSignalOwnerQObject();
    	String[] result = {null};
    	QMetaObject.Connection con = PrivateStaticSignalOwnerQObject.privateStaticSignal.connect(s->result[0] = s);
    	assertTrue(con.isConnected());
    	assertFalse(con instanceof QtObject);
    	PrivateStaticSignalOwnerQObject.emitStaticString("statictest");
    	assertEquals("statictest", result[0]);
    }
    
    @Test(expected=QSignalAccessException.class)
    public void test_external_emit_private_signal_static() {
    	String[] result = {null};
    	QMetaObject.Connection con = PrivateStaticSignalOwner.privateStaticSignal.connect(s->result[0] = s);
    	assertTrue(con.isConnected());
    	assertFalse(con instanceof QtObject);
    	QStaticMemberSignals.emit(PrivateStaticSignalOwner.privateStaticSignal, "statictest");
    	assertEquals("statictest", result[0]);
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestPrivateSignals.class.getName());
    }
}
