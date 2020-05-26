/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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
import static org.junit.Assert.assertTrue;

import java.util.List;

import org.junit.Test;

import io.qt.QtEnumerator;
import io.qt.QtFlagEnumerator;
import io.qt.QtInvokable;
import io.qt.QtPropertyReader;
import io.qt.QtPropertyWriter;
import io.qt.autotests.generated.SignalsAndSlots;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaProperty;
import io.qt.core.QObject;
import io.qt.gui.QStandardItem;

/**
 * Testing the fake meta object.
 * @author eblomfel
 */
public class TestMetaObject extends QApplicationTest {
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

        assertEquals(4, sass.numberOfEnumTypes());

        assertFalse(sass.isFlagsType("YoYoYo"));

        {
            List<String> names = sass.namesOfEnumType("YoYoYo");
            assertEquals(3, names.size());

            assertEquals("Yo", names.get(0));
            assertEquals("YoYo", names.get(1));
            assertEquals("YoYoYo", names.get(2));
        }

        {
            List<Integer> values = sass.valuesOfEnumType("YoYoYo");
            assertEquals(3, values.size());

            assertEquals((Integer)0, values.get(0));
            assertEquals((Integer)1, values.get(1));
            assertEquals((Integer)2, values.get(2));
        }
    }

    @Test public void qtEnumeratorDeclarations() {
        SignalsAndSlotsSubclass sass = new SignalsAndSlotsSubclass();

        assertFalse(sass.isFlagsType("HeyYo"));

        {
            List<String> names = sass.namesOfEnumType("HeyYo");
            assertEquals(2, names.size());

            assertEquals("Hey", names.get(0));
            assertEquals("Yo", names.get(1));
        }

        {
            List<Integer> values = sass.valuesOfEnumType("HeyYo");
            assertEquals(2, values.size());

            assertEquals((Integer)0x4, values.get(0));
            assertEquals((Integer)1123, values.get(1));
        }
    }

    @Test public void flagsDeclarations() {
        SignalsAndSlotsSubclass sass = new SignalsAndSlotsSubclass();

        assertTrue(sass.isFlagsType("FlipModeSquads"));

        {
            List<String> names = sass.namesOfEnumType("FlipModeSquad");
            assertEquals(3, names.size());

            assertEquals("Flip", names.get(0));
            assertEquals("Mode", names.get(1));
            assertEquals("Squad", names.get(2));
        }

        {
            List<Integer> values = sass.valuesOfEnumType("FlipModeSquad");
            assertEquals(3, values.size());

            assertEquals((Integer)1, values.get(0));
            assertEquals((Integer)2, values.get(1));
            assertEquals((Integer)4, values.get(2));
        }
    }
    
    static class SignalTest extends QObject{
    	private final Signal0 quit = new Signal0();
    	public void quit(){
    		quit.emit();
    	}
    }
    
    @Test 
    public void testDisposeLater() {
    	QMetaMethod method = QMetaMethod.fromMethod(QObject::disposeLater);
    	assertTrue(method!=null);
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
    	assertEquals(1, sender._receivers("destroyed()") + sender._receivers("destroyed(QObject)"));
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
    
//    @Test 
    public void testForeignMetaObject() {
    	System.out.println(QObject.staticMetaObject.constructors());
    	System.out.println(QObject.staticMetaObject.methods().size() + QObject.staticMetaObject.constructors().size());
    	System.out.println(QMetaObject.forType(SubQObject.class).constructors());
    	System.out.println(QMetaObject.forType(SubQObject.class).methods());
    	System.out.println(QMetaObject.forType(SubQtObject.class).constructors());
    	System.out.println(QMetaObject.forType(SubQtObject.class).methods());
    	System.out.println(QMetaObject.forType(NonQtType.class).constructors());
    	System.out.println(QMetaObject.forType(NonQtType.class).methods());
    	try {
			System.out.println("new SubQObject = "+QMetaObject.forType(SubQObject.class).newInstance(1, false));
		} catch (NoSuchMethodException e) {
			e.printStackTrace();
		}
    	try {
			System.out.println("new SubQtObject = "+QMetaObject.forType(SubQtObject.class).newInstance(1, false));
		} catch (NoSuchMethodException e) {
			e.printStackTrace();
		}
    	NonQtType object = new NonQtType(8, "test");
    	QMetaObject mo = QMetaObject.forType(object.getClass());
    	for(QMetaProperty prop : mo.properties()) {
    		System.out.print(prop.type().getName()+" "+prop.name()+" = ");
    		System.out.flush();
    		System.out.println(prop.readOnGadget(object));
    	}
    	System.out.println(mo.method("staticMethod").invokeOnGadget(object));
    	System.out.println(mo.method("formatNumber", double.class).invokeOnGadget(object, 0.12345));
    }
    
    @Test public void slotHidesSignal() {
    	assertEquals(QMetaMethod.MethodType.Signal, QMetaObject.forType(SignalTest.class).method("quit").methodType());
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestMetaObject.class.getName());
    }
}
