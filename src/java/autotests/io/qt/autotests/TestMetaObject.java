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
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Assert;
import org.junit.Test;

import io.qt.QtEnumerator;
import io.qt.QtFlagEnumerator;
import io.qt.QtInvokable;
import io.qt.QtPropertyReader;
import io.qt.QtPropertyWriter;
import io.qt.autotests.generated.SignalsAndSlots;
import io.qt.core.QMetaEnum;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaProperty;
import io.qt.core.QObject;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QPair;
import io.qt.core.Qt;
import io.qt.core.QtAlgorithms;
import io.qt.gui.QStandardItem;

/**
 * Testing the fake meta object.
 * @author eblomfel
 */
public class TestMetaObject extends ApplicationInitializer {
    @SuppressWarnings("unused")
    private static class SignalsAndSlotsSubclass extends SignalsAndSlots {
        enum YoYoYo {
            Yo,
            YoYo,
            YoYoYo
        }

        enum HeyYo implements QtEnumerator {
            Hey(0x4),
            Yo(1123);

            HeyYo(int value) { this.value = value; }
            public int value() { return value; }

            private final int value;
        }

        enum FlipModeSquad implements QtFlagEnumerator {
            Flip(1),
            Mode(2),
            Squad(4);

            FlipModeSquad(int value) { this.value = value; }
            public int value() { return value; }

            private final int value;

			@Override
			public FlipModeSquads asFlags() {
				return new FlipModeSquads(value());
			}
        }

		static class FlipModeSquads extends io.qt.QFlags<FlipModeSquad> {
			private static final long serialVersionUID = 886661346884528047L;
			public FlipModeSquads(FlipModeSquad ... args) { super(args); }
            public FlipModeSquads(int value) { setValue(value); }
			@Override
			public FlipModeSquads clone() {
				return new FlipModeSquads(value());
			}
			@Override
			public FlipModeSquads combined(FlipModeSquad flag) {
				return new FlipModeSquads(value() | flag.value());
			}
			@Override
			public FlipModeSquad[] flags() {
				return flags(FlipModeSquad.values());
			}
        }
    }

    @Test public void regularEnumDeclarations() {
        SignalsAndSlotsSubclass sass = new SignalsAndSlotsSubclass();

        assertEquals(4, sass.metaObject().enumeratorCount());

        QMetaEnum enumYoYoYo = sass.metaObject().enumerator("YoYoYo");
        assertFalse(enumYoYoYo.isFlag());
        assertEquals(3, enumYoYoYo.keyCount());
        assertEquals("Yo", enumYoYoYo.key(0));
        assertEquals("YoYo", enumYoYoYo.key(1));
        assertEquals("YoYoYo", enumYoYoYo.key(2));
        assertEquals(0, enumYoYoYo.value(0));
        assertEquals(1, enumYoYoYo.value(1));
        assertEquals(2, enumYoYoYo.value(2));
        assertEquals("Yo", enumYoYoYo.entry(0).name());
        assertEquals("YoYo", enumYoYoYo.entry(1).name());
        assertEquals("YoYoYo", enumYoYoYo.entry(2).name());
        assertEquals(0, ((QtEnumerator)enumYoYoYo.entry(0)).value());
        assertEquals(1, ((QtEnumerator)enumYoYoYo.entry(1)).value());
        assertEquals(2, ((QtEnumerator)enumYoYoYo.entry(2)).value());
    }

    @Test public void qtEnumeratorDeclarations() {
        SignalsAndSlotsSubclass sass = new SignalsAndSlotsSubclass();

        QMetaEnum enumHeyYo = sass.metaObject().enumerator("HeyYo");
        assertFalse(enumHeyYo.isFlag());
        assertEquals(2, enumHeyYo.keyCount());
        assertEquals("Hey", enumHeyYo.key(0));
        assertEquals("Yo", enumHeyYo.key(1));
        assertEquals(0x4, enumHeyYo.value(0));
        assertEquals(1123, enumHeyYo.value(1));
        assertEquals("Hey", enumHeyYo.entry(0).name());
        assertEquals("Yo", enumHeyYo.entry(1).name());
        assertEquals(0x4, ((QtEnumerator)enumHeyYo.entry(0)).value());
        assertEquals(1123, ((QtEnumerator)enumHeyYo.entry(1)).value());
    }

    @Test public void flagsDeclarations() {
        SignalsAndSlotsSubclass sass = new SignalsAndSlotsSubclass();

        QMetaEnum enumFlipModeSquads = sass.metaObject().enumerator("FlipModeSquads");
        assertTrue(enumFlipModeSquads.isFlag());
        assertEquals(3, enumFlipModeSquads.keyCount());
        assertEquals("Flip", enumFlipModeSquads.key(0));
        assertEquals("Mode", enumFlipModeSquads.key(1));
        assertEquals("Squad", enumFlipModeSquads.key(2));
        assertEquals(1, enumFlipModeSquads.value(0));
        assertEquals(2, enumFlipModeSquads.value(1));
        assertEquals(4, enumFlipModeSquads.value(2));
        assertEquals("Flip", enumFlipModeSquads.entry(0).name());
        assertEquals("Mode", enumFlipModeSquads.entry(1).name());
        assertEquals("Squad", enumFlipModeSquads.entry(2).name());
        assertEquals(1, ((QtEnumerator)enumFlipModeSquads.entry(0)).value());
        assertEquals(2, ((QtEnumerator)enumFlipModeSquads.entry(1)).value());
        assertEquals(4, ((QtEnumerator)enumFlipModeSquads.entry(2)).value());
    }
    
    static class SignalTest extends QObject{
    	private final Signal0 quit = new Signal0();
    	public void quit(){
    		quit.emit();
    	}
    }
    
    @Test 
    public void testDisposeLater() throws NoSuchMethodException, SecurityException {
    	if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)) {
	    	QMetaMethod method = QMetaMethod.fromReflectedMethod(QObject.class.getMethod("disposeLater"));
	    	assertTrue(method!=null);    		
    	}else {
	    	QMetaMethod method = QMetaMethod.fromMethod(QObject::disposeLater);
	    	assertTrue(method!=null);
    	}
    	class _QObject extends QObject{
    		int _receivers(io.qt.core.QMetaObject.AbstractSignal signalObject){
    			return receivers(signalObject);
    		}
    		int _receivers(String signal){
    			return receivers(signal);
    		}
    	}
    	_QObject sender = new _QObject();
    	QObject receiver = new QObject();
    	assertTrue(QObject.connect(sender, "destroyed(QObject)", receiver, "disposeLater()")!=null);
    	assertTrue(sender.destroyed.disconnect(receiver));
    	assertTrue(sender.destroyed.connect(receiver, "disposeLater()")!=null);
    	assertEquals(1, sender._receivers(sender.destroyed));
    	assertEquals(1, sender._receivers("destroyed(QObject)"));
    }
    
    public static class SubQObject extends QObject{
    	@QtInvokable
		public SubQObject(int i, boolean b) {
			super();
		}
    	
    	@QtInvokable
		public SubQObject(int i, boolean b, String s) {
			super();
		}
    }
    
    public static class SubQtObject extends QStandardItem{
    	@QtInvokable
		public SubQtObject(int i, boolean b) {
			super();
		}
    }
    
    public static class NonQtType{
    	@QtInvokable
    	public NonQtType(int integerNumber, String text) {
			super();
			this.integerNumber = integerNumber;
			this.text = text;
		}
    	
		private int integerNumber;
    	private String text;
    	
    	@QtPropertyReader
		public int getIntegerNumber() {
			return integerNumber;
		}
    	@QtPropertyWriter
		public void setIntegerNumber(int i) {
			this.integerNumber = i;
		}
    	@QtPropertyReader
		public String getText() {
			return text;
		}
    	@QtPropertyWriter
		public void setText(String text) {
			this.text = text;
		}
    	
    	@QtInvokable
    	public static String staticMethod() {
    		return "success";
    	}
    	
    	@QtInvokable
    	public String formatNumber(double number) {
    		return Double.toHexString(number);
    	}
    }
    
    @Test 
    public void testForeignMetaObject() throws NoSuchMethodException {
    	assertEquals(null, QObject.staticMetaObject.constructors().get(0).returnClassType());
    	assertEquals(0, QObject.staticMetaObject.constructors().get(0).returnType());
    	assertEquals("", QObject.staticMetaObject.constructors().get(0).typeName());
//    	System.out.println(QObject.staticMetaObject.constructors());
//    	System.out.println(QObject.staticMetaObject.methods().size() + QObject.staticMetaObject.constructors().size());
    	QMetaObject subQObjectMO = QMetaObject.forType(SubQObject.class);
    	QMetaObject subQtObjectMO = QMetaObject.forType(SubQtObject.class);
//    	System.out.println(subQObjectMO.constructors());
    	assertEquals(null, subQObjectMO.constructors().get(0).returnClassType());
    	assertEquals(0, subQObjectMO.constructors().get(0).returnType());
    	assertEquals("", subQObjectMO.constructors().get(0).typeName());
//    	System.out.println(subQObjectMO.methods());
//    	System.out.println(subQtObjectMO.constructors());
//    	System.out.println(subQtObjectMO.methods());
//    	System.out.println(QMetaObject.forType(NonQtType.class).constructors());
//    	System.out.println(QMetaObject.forType(NonQtType.class).methods());
		Object instance = subQObjectMO.newInstance(1, false);
		assertTrue(instance instanceof SubQObject);
    	try {
    		instance = subQtObjectMO.newInstance(1, false);
    		assertTrue("RuntimeException expected to be thrown", false);
		} catch (Exception e) {
			assertEquals("Type io::qt::autotests::TestMetaObject::SubQtObject does not inherit QObject", e.getMessage());
		}
    	NonQtType object = new NonQtType(8, "test");
    	QMetaObject mo = QMetaObject.forType(object.getClass());
    	for(QMetaProperty prop : mo.properties()) {
    		switch(prop.name()) {
    		case "integerNumber":
    			assertEquals("property integerNumber", 8, prop.readOnGadget(object));
    			break;
    		case "text":
    			assertEquals("property text", "test", prop.readOnGadget(object));
    			break;
    		}
    	}
    	assertEquals("success", mo.method("staticMethod").invokeOnGadget(object));
    	double d = 0.12345;
    	assertEquals(Double.toHexString(d), mo.method("formatNumber", double.class).invokeOnGadget(object, d));
    }
    
    @Test public void slotHidesSignal() {
    	assertEquals(QMetaMethod.MethodType.Signal, QMetaObject.forType(SignalTest.class).method("quit").methodType());
    }
    
    public static class PairSlotObject extends QObject {
    	@QtInvokable
        private QPair<String, String> invokablePairSupplier() {
            return new QPair<>("A", "B");
        }
    	
    	@SuppressWarnings("unused")
		private int uninvokable() {
            return 0;
        }
    }
    
    @Test public void testPairMetaType() {
    	PairSlotObject object = new PairSlotObject();
    	QMetaMethod invokablePairSupplier = object.metaObject().method("invokablePairSupplier");
    	Assert.assertTrue(invokablePairSupplier.isValid());
    	Assert.assertTrue(invokablePairSupplier.returnType()!=0);
    	QMetaMethod uninvokable = object.metaObject().method("uninvokable");
    	Assert.assertFalse(uninvokable.isValid());
    }
    
    @Test public void testMetaObjectFromNamespace() {
    	Assert.assertEquals(Qt.staticMetaObject, QMetaObject.forType(Qt.class));
    	Assert.assertEquals(null, QMetaObject.forType(QtAlgorithms.class));
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestMetaObject.class.getName());
    }
}
