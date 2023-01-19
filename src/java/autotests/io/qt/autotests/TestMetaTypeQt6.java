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
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import java.io.Serializable;

import org.junit.Assert;
import org.junit.Test;

import io.qt.QtPropertyMember;
import io.qt.core.QByteArray;
import io.qt.core.QDataStream;
import io.qt.core.QDebug;
import io.qt.core.QHash;
import io.qt.core.QIODevice;
import io.qt.core.QList;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaProperty;
import io.qt.core.QMetaType;
import io.qt.core.QObject;
import io.qt.core.QOperatingSystemVersion;
import io.qt.qml.QJSValue;

public class TestMetaTypeQt6 extends ApplicationInitializer {
    @Test
    public void testRegisterValueType() {
    	QMetaType metaType = QMetaType.fromType(CloneableMetaValue.class);
    	CloneableMetaValue v1 = (CloneableMetaValue)metaType.create();
    	v1.d = 4.0;
    	v1.i = 9;
    	v1.s = "TEST";
    	CloneableMetaValue v2 = (CloneableMetaValue)metaType.create(v1);
    	Assert.assertEquals(v1.i, v2.i);
    	Assert.assertEquals(0.000001, v1.d, v2.d);
    	Assert.assertEquals(v1.s, v2.s);
    	
    	try {
	    	QMetaType.registerDataStreamOperators((QDataStream s, CloneableMetaValue value)->{
				s.writeInt(value.i);
				s.writeDouble(value.d);
				s.writeString(value.s);
			}, s->{
				CloneableMetaValue value = new CloneableMetaValue();
				value.i = s.readInt();
				value.d = s.readDouble();
				value.s = s.readString();
				return value;
			});
		} catch (IllegalArgumentException e1) {
			if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)) {
				QMetaType.registerDataStreamOperators(new QMetaType.DataStreamInFn<CloneableMetaValue>(){
					private static final long serialVersionUID = 1L;
					@Override
					public void accept(QDataStream s, CloneableMetaValue value) {
						s.writeInt(value.i);
						s.writeDouble(value.d);
						s.writeString(value.s);
					}
				},
				new QMetaType.DataStreamOutFn<CloneableMetaValue>(){
					private static final long serialVersionUID = 1L;
					@Override
					public CloneableMetaValue apply(QDataStream s) {
						CloneableMetaValue value = new CloneableMetaValue();
						value.i = s.readInt();
						value.d = s.readDouble();
						value.s = s.readString();
						return value;
					}
				});
			}else {
				throw e1;
			}
		}
    	
    	try {
	    	QMetaType.registerDebugStreamOperator((QDebug d, CloneableMetaValue value)->{
	    		d.append("CloneableMetaValue [i=" + value.i + ", d=" + value.d + ", s=" + value.s + "]");
	    	});
	    	try {
				QMetaType.registerDebugStreamOperator((QDebug s, CloneableMetaValue value)->{});
				fail("RuntimeException expected to be thrown.");
			} catch (RuntimeException e) {
			}
		} catch (IllegalArgumentException e1) {
			if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)) {
				QMetaType.registerDebugStreamOperator(new QMetaType.DebugStreamFn<CloneableMetaValue>(){
					private static final long serialVersionUID = 1L;
					@Override
					public void accept(QDebug d, CloneableMetaValue value) {
						d.append("CloneableMetaValue [i=" + value.i + ", d=" + value.d + ", s=" + value.s + "]");
					}
				});
			}else {
				throw e1;
			}
		}
    	QByteArray buffer = new QByteArray();
    	QDataStream stream = new QDataStream(buffer, QIODevice.OpenModeFlag.WriteOnly);
    	stream.writeObject(v2);
    	stream.dispose();
    	stream = new QDataStream(buffer, QIODevice.OpenModeFlag.ReadOnly);
    	CloneableMetaValue restored = stream.readObject(CloneableMetaValue.class);
    	stream.dispose();
    	assertTrue(restored instanceof CloneableMetaValue);
    	assertEquals(v2.i, restored.i);
    	assertEquals(v2.d, restored.d, 0.01);
    	assertEquals(v2.s, restored.s);
    	
    	QList<CloneableMetaValue> list = new QList<>(CloneableMetaValue.class);
    	list.resize(10);
    	for (CloneableMetaValue v : list) {
			assertTrue(v!=null);
		}
    	
    	QMetaObject mo = QMetaObject.forType(CloneableMetaValue.class);
    	Assert.assertEquals(3, mo.properties().size());
    	
    	class CloneableMetaValueHolder extends QObject{
    		public final Signal1<CloneableMetaValue> metaValueChanged = new Signal1<>();
    		public final QProperty<CloneableMetaValue> metaValue = new QProperty<>();
    		public final Signal0 metaValue2Changed = new Signal0();
    		public final QProperty<CloneableMetaValue> metaValue2 = new QProperty<>();
    	}
    	CloneableMetaValueHolder holder = new CloneableMetaValueHolder();
    	QMetaProperty prop = holder.metaObject().property("metaValue");
    	assertTrue(prop!=null && prop.isValid());
    	Assert.assertEquals(metaType, prop.metaType());
    	QMetaMethod signal = holder.metaObject().method("metaValueChanged", CloneableMetaValue.class);
    	assertTrue(signal!=null && signal.isValid());
    	Assert.assertEquals(metaType, signal.parameterMetaType(0));
    	assertTrue(holder.metaValue.value()!=null);
    	CloneableMetaValue[] received = {null};
    	holder.metaValueChanged.connect(c->{received[0] = c;});
    	holder.metaValue.setValue(v2);
    	assertTrue(v2!=received[0]);
    	Assert.assertEquals(v2, received[0]);
    	holder.metaValue2Changed.connect(()->{received[0] = null;});
    	holder.metaValue2.setValue(v2);
    	Assert.assertEquals(null, received[0]);
    }
    
    @Test
    public void testRegisterIOValueType() {
    	QMetaType metaType = QMetaType.fromType(IOCloneableMetaValue.class);
    	IOCloneableMetaValue v1 = (IOCloneableMetaValue)metaType.create();
    	v1.d = 4.0;
    	v1.i = 9;
    	v1.s = "TEST";
    	IOCloneableMetaValue v2 = (IOCloneableMetaValue)metaType.create(v1);
    	Assert.assertEquals(v1.i, v2.i);
    	Assert.assertEquals(0.000001, v1.d, v2.d);
    	Assert.assertEquals(v1.s, v2.s);
    	QByteArray buffer = new QByteArray();
    	QDataStream stream = new QDataStream(buffer, QIODevice.OpenModeFlag.WriteOnly);
    	stream.writeObject(v2);
    	stream.dispose();
    	stream = new QDataStream(buffer, QIODevice.OpenModeFlag.ReadOnly);
    	IOCloneableMetaValue restored = stream.readObject(IOCloneableMetaValue.class);
    	stream.dispose();
    	assertTrue(restored!=null);
    	assertEquals(v2.i, restored.i);
    	assertEquals(v2.d, restored.d, 0.01);
    	assertEquals(v2.s, restored.s);
    	
    	QList<IOCloneableMetaValue> list = new QList<>(IOCloneableMetaValue.class);
    	list.resize(10);
    	for (IOCloneableMetaValue v : list) {
			assertTrue(v!=null);
		}
    	list.at(0).s = "TEST";
    	assertEquals(list.at(0).s, null);
    	list.insert(0, v1);
		assertTrue(v1!=list.at(0));
    	Assert.assertEquals(v1.i, list.at(0).i);
    	Assert.assertEquals(0.000001, v1.d, list.at(0).d);
    	Assert.assertEquals(v1.s, list.at(0).s);
    }
    
    @Test
    public void testRegisterSerializableValueType() {
    	QMetaType metaType = QMetaType.fromType(SerializableCloneableMetaValue.class);
    	SerializableCloneableMetaValue v1 = (SerializableCloneableMetaValue)metaType.create();
    	v1.d = 4.0;
    	v1.i = 9;
    	v1.s = "TEST";
    	SerializableCloneableMetaValue v2 = (SerializableCloneableMetaValue)metaType.create(v1);
    	Assert.assertEquals(v1.i, v2.i);
    	Assert.assertEquals(0.000001, v1.d, v2.d);
    	Assert.assertEquals(v1.s, v2.s);
    	QByteArray buffer = new QByteArray();
    	QDataStream stream = new QDataStream(buffer, QIODevice.OpenModeFlag.WriteOnly);
    	stream.writeObject(v2);
    	stream.dispose();
    	stream = new QDataStream(buffer, QIODevice.OpenModeFlag.ReadOnly);
    	SerializableCloneableMetaValue restored = stream.readObject(SerializableCloneableMetaValue.class);
    	stream.dispose();
    	assertTrue(restored!=null);
    	assertEquals(v2.i, restored.i);
    	assertEquals(v2.d, restored.d, 0.01);
    	assertEquals(v2.s, restored.s);
    	
    	QList<SerializableCloneableMetaValue> list = new QList<>(SerializableCloneableMetaValue.class);
    	list.resize(10);
    	for (SerializableCloneableMetaValue v : list) {
			assertTrue(v!=null);
		}
    	list.at(0).s = "TEST";
    	assertEquals(list.at(0).s, null);
    	list.insert(0, v1);
		assertTrue(v1!=list.at(0));
    	Assert.assertEquals(v1.i, list.at(0).i);
    	Assert.assertEquals(0.000001, v1.d, list.at(0).d);
    	Assert.assertEquals(v1.s, list.at(0).s);
    }
    
    @Test
    public void testRegisterContainerMetaType() {
    	QMetaType type = QMetaType.fromObject(new QHash<>(String.class, Iface.class));
    	System.out.println(type);
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestMetaTypeQt6.class.getName());
    }
}

interface Iface{
}

class CloneableMetaValue implements Cloneable{
	
	@SuppressWarnings("unused")
	private static CloneableMetaValue create(QJSValue arguments) {
		CloneableMetaValue value = new CloneableMetaValue();
		if(arguments.isArray()) {
			int length = arguments.property("length").toInt();
			if(length==3) {
				value.i = arguments.property(0).toInt();
				value.d = arguments.property(1).toDouble();
				value.s = arguments.property(2).toString();
			}
		}
		return value;
	}
	
	@QtPropertyMember
	int i;
	@QtPropertyMember
	double d;
	@QtPropertyMember
	String s;
	
	@Override
	public CloneableMetaValue clone() {
		CloneableMetaValue v = new CloneableMetaValue();
		v.d = d;
		v.i = i;
		v.s = s;
		return v;
	}
	
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		long temp;
		temp = Double.doubleToLongBits(d);
		result = prime * result + (int) (temp ^ (temp >>> 32));
		result = prime * result + i;
		result = prime * result + ((s == null) ? 0 : s.hashCode());
		return result;
	}
	
	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		CloneableMetaValue other = (CloneableMetaValue) obj;
		if (Double.doubleToLongBits(d) != Double.doubleToLongBits(other.d))
			return false;
		if (i != other.i)
			return false;
		if (s == null) {
			if (other.s != null)
				return false;
		} else if (!s.equals(other.s))
			return false;
		return true;
	}
}

class IOCloneableMetaValue implements Cloneable{
	@QtPropertyMember
	int i;
	@QtPropertyMember
	double d;
	@QtPropertyMember
	String s;
	@Override
	public IOCloneableMetaValue clone() {
		IOCloneableMetaValue v = new IOCloneableMetaValue();
		v.d = d;
		v.i = i;
		v.s = s;
		return v;
	}
	
	@SuppressWarnings("unused")
	private void writeTo(QDataStream s) {
		s.writeInt(this.i);
		s.writeDouble(this.d);
		s.writeString(this.s);		
	}
	
	@SuppressWarnings("unused")
	private void readFrom(QDataStream s) {
		this.i = s.readInt();
		this.d = s.readDouble();
		this.s = s.readString();
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		long temp;
		temp = Double.doubleToLongBits(d);
		result = prime * result + (int) (temp ^ (temp >>> 32));
		result = prime * result + i;
		result = prime * result + ((s == null) ? 0 : s.hashCode());
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		IOCloneableMetaValue other = (IOCloneableMetaValue) obj;
		if (Double.doubleToLongBits(d) != Double.doubleToLongBits(other.d))
			return false;
		if (i != other.i)
			return false;
		if (s == null) {
			if (other.s != null)
				return false;
		} else if (!s.equals(other.s))
			return false;
		return true;
	}
}

class SerializableCloneableMetaValue implements Cloneable, Serializable{
	private static final long serialVersionUID = 1L;
	@QtPropertyMember
	int i;
	@QtPropertyMember
	double d;
	@QtPropertyMember
	String s;

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		long temp;
		temp = Double.doubleToLongBits(d);
		result = prime * result + (int) (temp ^ (temp >>> 32));
		result = prime * result + i;
		result = prime * result + ((s == null) ? 0 : s.hashCode());
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		SerializableCloneableMetaValue other = (SerializableCloneableMetaValue) obj;
		if (Double.doubleToLongBits(d) != Double.doubleToLongBits(other.d))
			return false;
		if (i != other.i)
			return false;
		if (s == null) {
			if (other.s != null)
				return false;
		} else if (!s.equals(other.s))
			return false;
		return true;
	}
}
