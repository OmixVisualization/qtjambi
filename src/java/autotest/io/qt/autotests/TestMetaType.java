/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import java.io.Serializable;

import org.junit.Test;

import io.qt.core.QByteArray;
import io.qt.core.QCoreApplication;
import io.qt.core.QDataStream;
import io.qt.core.QDebug;
import io.qt.core.QIODevice;
import io.qt.core.QMetaType;
import io.qt.core.QObject;
import io.qt.core.QRect;
import io.qt.core.QVariant;
import io.qt.gui.QColor;
import io.qt.gui.QWindow;

public class TestMetaType extends ApplicationInitializer {
    @Test
    public void testRegisterStreams() {
    	QMetaType.registerDataStreamOperators((QDataStream s, MetaValue value)->{
    		s.writeInt(value.i);
    		s.writeDouble(value.d);
    		s.writeString(value.s);
    	}, s->{
    		MetaValue value = new MetaValue();
    		value.i = s.readInt();
    		value.d = s.readDouble();
    		value.s = s.readString();
    		return value;
    	});
    	try {
			QMetaType.registerDataStreamOperators((QDataStream s, MetaValue value)->{}, s->null);
			fail("RuntimeException expected to be thrown.");
		} catch (RuntimeException e) {
		}
    	QMetaType.registerDebugStreamOperator((QDebug d, MetaValue value)->{
    		d.append("MetaValue [i=" + value.i + ", d=" + value.d + ", s=" + value.s + "]");
    	});
    	try {
			QMetaType.registerDebugStreamOperator((QDebug s, MetaValue value)->{});
			fail("RuntimeException expected to be thrown.");
		} catch (RuntimeException e) {
		}
    	QMetaType.registerMetaType(SerializableMetaValue.class);
    	MetaValue mvalue = new MetaValue();
    	mvalue.i = 5;
    	mvalue.d = 9.246;
    	mvalue.s = "TEST";
    	QByteArray buffer = new QByteArray();
    	QVariant.saveObject(new QDataStream(buffer, QIODevice.OpenModeFlag.WriteOnly), mvalue);
    	Object restored = QVariant.loadObject(new QDataStream(buffer, QIODevice.OpenModeFlag.ReadOnly));
    	assertTrue(restored instanceof MetaValue);
    	assertEquals(mvalue.i, ((MetaValue)restored).i);
    	assertEquals(mvalue.d, ((MetaValue)restored).d, 0.01);
    	assertEquals(mvalue.s, ((MetaValue)restored).s);
    	StringBuilder string = new StringBuilder();
    	try(QDebug debug = new QDebug(string)){
    		debug.nospace().append(restored);
    	}
    	assertEquals("MetaValue [i=" + mvalue.i + ", d=" + mvalue.d + ", s=" + mvalue.s + "]", string.toString());
    	
    	SerializableMetaValue svalue = new SerializableMetaValue();
    	svalue.i = 5;
    	svalue.d = 9.246;
    	svalue.s = "TEST";
    	buffer.clear();
    	QVariant.saveObject(new QDataStream(buffer, QIODevice.OpenModeFlag.WriteOnly), svalue);
    	restored = QVariant.loadObject(new QDataStream(buffer, QIODevice.OpenModeFlag.ReadOnly));
    	assertTrue(restored instanceof SerializableMetaValue);
    	assertEquals(svalue.i, ((SerializableMetaValue)restored).i);
    	assertEquals(svalue.d, ((SerializableMetaValue)restored).d, 0.01);
    	assertEquals(svalue.s, ((SerializableMetaValue)restored).s);
    	string = new StringBuilder();
    	try(QDebug debug = new QDebug(string)){
    		debug.nospace().append(restored);
    	}
    	assertEquals("SerializableMetaValue [i=" + svalue.i + ", d=" + svalue.d + ", s=" + svalue.s + "]", string.toString());
    	
    	try {
			QMetaType.registerDataStreamOperators((QDataStream s, QRect value)->{}, s->(QRect)null);
			fail("IllegalArgumentException expected to be thrown.");
		} catch (IllegalArgumentException e) {
		}
    	try {
			QMetaType.registerDataStreamOperators((QDataStream s, QWindow value)->{}, s->(QWindow)null);
			fail("IllegalArgumentException expected to be thrown.");
		} catch (IllegalArgumentException e) {
		}
    	try {
			QMetaType.registerDataStreamOperators((QDataStream s, QCoreApplication value)->{}, s->(QCoreApplication)null);
			fail("IllegalArgumentException expected to be thrown.");
		} catch (IllegalArgumentException e) {
		}
    	try {
			QMetaType.registerDataStreamOperators((QDataStream s, MyColor value)->{}, s->(MyColor)null);
			fail("IllegalArgumentException expected to be thrown.");
		} catch (IllegalArgumentException e) {
		}
    	try {
			QMetaType.registerDataStreamOperators((QDataStream s, MyObject value)->{}, s->(MyObject)null);
			fail("IllegalArgumentException expected to be thrown.");
		} catch (IllegalArgumentException e) {
		}
    	try {
			QMetaType.registerDataStreamOperators((QDataStream s, int[] value)->{}, s->(int[])null);
			fail("IllegalArgumentException expected to be thrown.");
		} catch (IllegalArgumentException e) {
		}
    	try {
    		@SuppressWarnings({ "unchecked", "rawtypes" })
			QMetaType.DataStreamInFn<Object> infn = (QMetaType.DataStreamInFn)(QMetaType.DataStreamInFn<T1>)(QDataStream s, T1 value)->{};
    		@SuppressWarnings({ "unchecked", "rawtypes" })
			QMetaType.DataStreamOutFn<Object> outfn = (QMetaType.DataStreamOutFn)(QMetaType.DataStreamOutFn<T2>)s->(T2)null;
			QMetaType.registerDataStreamOperators(infn, outfn);
			fail("IllegalArgumentException expected to be thrown.");
		} catch (IllegalArgumentException e) {
		}
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestMetaType.class.getName());
    }
}

class MyColor extends QColor{
}

class MyObject extends QObject{
}

class MetaValue{
	int i;
	double d;
	String s;
}

class SerializableMetaValue implements Serializable{
	private static final long serialVersionUID = -4765490824878229108L;
	int i;
	double d;
	String s;
	
	@Override
	public String toString() {
		return "SerializableMetaValue [i=" + i + ", d=" + d + ", s=" + s + "]";
	}
}

class T1{
}

class T2{
}