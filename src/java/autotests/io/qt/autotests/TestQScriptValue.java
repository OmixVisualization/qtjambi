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
import static org.junit.Assert.assertNotSame;
import static org.junit.Assert.assertTrue;

import org.junit.BeforeClass;

import io.qt.autotests.ApplicationInitializer;
import io.qt.script.QScriptEngine;
import io.qt.script.QScriptValue;
import io.qt.script.QScriptValue.PropertyFlag;
import io.qt.script.QScriptValue.PropertyFlags;
import io.qt.script.QScriptValue.ResolveFlag;
import io.qt.script.QScriptValue.ResolveFlags;
import io.qt.script.QScriptValue.SpecialValue;
import io.qt.widgets.QPushButton;

public class TestQScriptValue extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }

	private QScriptValue scriptButton;
	private QScriptValue val;
	private QScriptValue val1;
	@SuppressWarnings("unused")
	private QScriptValue val2;

	private QScriptEngine testEngine;
	private QPushButton button;

	private SpecialValue specVal0;
	private SpecialValue specVal1;

	private ResolveFlag resFlag0;
	private ResolveFlag resFlag1;
	private ResolveFlag resFlag2;
	private ResolveFlag resFlag3;

	private ResolveFlags resFlags0;
	private ResolveFlags resFlags1;

	private PropertyFlag propFlag1;
	private PropertyFlag propFlag2;
	private PropertyFlag propFlag4;
	private PropertyFlag propFlag8;
	private PropertyFlag propFlag16;
	private PropertyFlag propFlag32;
	private PropertyFlag propFlag2048;
	private PropertyFlag propFlag_16777216;

	private PropertyFlags propFlags1;
	private PropertyFlags propFlags32;

	@org.junit.Before
	public void setUp() {
		testEngine = new QScriptEngine();
		button = new QPushButton();
		scriptButton = testEngine.newQObject(button);

		specVal0 = SpecialValue.NullValue;
		specVal1 = SpecialValue.UndefinedValue;

		resFlag0 = ResolveFlag.ResolveLocal;
		resFlag1 = ResolveFlag.ResolvePrototype;
		resFlag2 = ResolveFlag.ResolveScope;
		resFlag3 = ResolveFlag.ResolveFull;

		resFlags0 = ResolveFlag.flags(resFlag0);
		resFlags1 = new ResolveFlags(3);

		propFlag1 = PropertyFlag.ReadOnly;
		propFlag2 = PropertyFlag.Undeletable;
		propFlag4 = PropertyFlag.SkipInEnumeration;
		propFlag8 = PropertyFlag.PropertyGetter;
		propFlag16 = PropertyFlag.PropertySetter;
		propFlag32 = PropertyFlag.QObjectMember;
		propFlag2048 = PropertyFlag.KeepExistingFlags;
		propFlag_16777216 = PropertyFlag.UserRange;

		propFlags1 = PropertyFlag.flags(propFlag1);
		propFlags32 = new PropertyFlags(32);
	}

	@org.junit.After
	public void tearDown() {
		val = null;
		val1 = null;
		val2 = null;

		testEngine = null;
		button = null;
		scriptButton = null;

		specVal0 = null;
		specVal1 = null;

		resFlag0 = null;
		resFlag1 = null;
		resFlag2 = null;
		resFlag3 = null;

		resFlags0 = null;
		resFlags1 = null;

		propFlag1 = null;
		propFlag2 = null;
		propFlag4 = null;
		propFlag8 = null;
		propFlag16 = null;
		propFlag32 = null;
		propFlag2048 = null;
		propFlag_16777216 = null;
	}

	@org.junit.Test
	public void test_ResolveFlag_resolve() {
		assertEquals(ResolveFlag.resolve(0), resFlag0);
		assertEquals(ResolveFlag.resolve(1), resFlag1);
		assertEquals(ResolveFlag.resolve(2), resFlag2);
		assertEquals(ResolveFlag.resolve(3), resFlag3);
	}

	@org.junit.Test(expected = io.qt.QNoSuchEnumValueException.class)
	public void test_ResolveFlag_resolveException() {
		assertEquals(ResolveFlag.resolve(-1), resFlag1);
	}

	@org.junit.Test
	public void test_ResolveFlag_value() {
		assertEquals(resFlag0.value(), 0);
		assertEquals(resFlag1.value(), 1);
		assertEquals(resFlag2.value(), 2);
		assertEquals(resFlag3.value(), 3);
	}

	@org.junit.Test
	public void test_ResolveFlag_flags() {
		assertEquals(resFlags0.value(), resFlag0.value());
		assertEquals(resFlags1.value(), 3);
	}

	@org.junit.Test
	public void test_PropertyFlag_resolve() {
		assertEquals(PropertyFlag.resolve(1), propFlag1);
		assertEquals(PropertyFlag.resolve(2), propFlag2);
		assertEquals(PropertyFlag.resolve(4), propFlag4);
		assertEquals(PropertyFlag.resolve(8), propFlag8);
		assertEquals(PropertyFlag.resolve(16), propFlag16);
		assertEquals(PropertyFlag.resolve(32), propFlag32);
		assertEquals(PropertyFlag.resolve(2048), propFlag2048);
		assertEquals(PropertyFlag.resolve(-16777216), propFlag_16777216);
	}

	@org.junit.Test(expected = io.qt.QNoSuchEnumValueException.class)
	public void test_PorpertyFlag_resolveException() {
		assertEquals(PropertyFlag.resolve(-1), propFlag1);
	}

	@org.junit.Test
	public void test_PropertyFlag_value() {
		assertEquals(propFlag1.value(), 1);
		assertEquals(propFlag2.value(), 2);
		assertEquals(propFlag4.value(), 4);
		assertEquals(propFlag8.value(), 8);
		assertEquals(propFlag16.value(), 16);
		assertEquals(propFlag32.value(), 32);
		assertEquals(propFlag2048.value(), 2048);
		assertEquals(propFlag_16777216.value(), -16777216);
	}

	@org.junit.Test
	public void test_PropertyFlag_flags() {
		assertEquals(propFlags1.value(), propFlag1.value());
		assertEquals(propFlags32.value(), 32);
	}

	@org.junit.Test
	public void testSpecialValue_resolve() {
		assertEquals(SpecialValue.resolve(0), specVal0);
		assertEquals(SpecialValue.resolve(1), specVal1);
	}

	@org.junit.Test(expected = io.qt.QNoSuchEnumValueException.class)
	public void testSpecialValue_resolveException() {
		assertEquals(SpecialValue.resolve(-1), specVal0);
	}

	@org.junit.Test
	public void testSpecialValue_value() {
		assertEquals(specVal0.value(), 0);
		assertEquals(specVal1.value(), 1);
	}

	@org.junit.Test
	public void test_emptyConstructor() {
		val1 = new QScriptValue();
		assertFalse(val1.isValid());
	}

	@org.junit.Test
	public void test_QScriptValue_in_constructor() {
		val = testEngine.toObject(scriptButton);
		val1 = new QScriptValue(val);
		assertTrue(val1.isValid());
	}

	@org.junit.Test
	public void test_SpecialValue_in_constructor() {
		val = new QScriptValue(specVal0);
		assertTrue(val.isValid());
	}

	@org.junit.Test
	public void test_bool_in_constructor() {
		val = new QScriptValue(false);
		assertTrue(val.isValid());
	}

	@org.junit.Test
	public void test_int_in_constructor() {
		val = new QScriptValue(0);
		assertTrue(val.isValid());
	}

	@org.junit.Test
	public void test_string_in_constructor() {
		val = new QScriptValue("QScriptValue");
		assertTrue(val.isValid());
	}

	@org.junit.Test
	public void test_char_in_constructor() {
		val = new QScriptValue('a');
		assertTrue(val.isValid());
	}

	@org.junit.Test
	public void test_double_in_constructor() {
		val = new QScriptValue(3.14);
		assertTrue(val.isValid());
	}

	/*
	 * How it works:
	 * call(QScriptValue thisObject, QScriptValue arguments)
	 * where the arguments should be an array of QScriptValue.
	 * In case of plain QScriptValue the call returns with
	 * TypeError: Arguments must be an array
	 * 
	 * How it would be cool:
	 * There is already a class QScriptValueIterator.
	 * Modify QScriptValueIterator to extend some java iterator
	 * interface, then with .next() simply get the QScriptValue,
	 * no need to modify the function header.
	 */
	/*@org.junit.Test
	public void testCall() {
		testEngine.evaluate("function f(a) { return 10 * a + 1; }");
		val = testEngine.globalObject();
		val1 = val.property("f");
		QScriptValue three = new QScriptValue(3);
		QScriptValueIterator qsv = new QScriptValueIterator(three);
		System.out.println(val1.call(new QScriptValue(), qsv.next()));
	}*/

	@org.junit.Test
	public void testEngine() {
		QScriptEngine owner = scriptButton.engine();
		QScriptEngine someEngine = new QScriptEngine();
		assertEquals(owner, testEngine);
		assertNotSame(someEngine, testEngine);
		
		val = new QScriptValue(3);
		assertTrue(val.isValid());
		
		/*
		 * How it works in Qt 4.8: Returns the QScriptEngine that created this
		 * QScriptValue, or 0 if this QScriptValue is invalid or the value is
		 * not associated with a particular engine.
		 * 
		 * How it works here: it returns with null instead.
		 */
		
		assertEquals(null, val.engine());
	}
	
	/*
	 * http://developer.qt.nokia.com/doc/qt-4.8/qscriptvalue.html#equals: the
	 * comparison is not strict, comparing the number 9 to the string "9"
	 * returns true; comparing the number 1 to the boolean value true returns
	 * true;
	 */
	@org.junit.Test
	public void testEquals() {
		val = new QScriptValue(3);
		val1 = new QScriptValue("3");
		assertTrue(val.equals(val1));
		
		val = new QScriptValue(true);
		val1 = new QScriptValue(1);
		assertTrue(val.equals(val1));
		
		val = new QScriptValue(false);
		val1 = new QScriptValue(0);
		assertTrue(val.equals(val1));
		
		val = new QScriptValue(true);
		val1 = new QScriptValue(6);
		assertFalse(val.equals(val1));
		
		val = new QScriptValue("QScriptValue");
		val1 = new QScriptValue("Q ScriptValue");
		assertFalse(val.equals(val1));
	}
	
	/*
	@org.junit.Test
	public void testInstanceOf() {
		val = new QScriptValue(3);
		val1 = new QScriptValue("QScriptValue");

		val.setPrototype(val2);
		val1.setPrototype(val2);
		
		assertEquals(val.prototype(), val1.prototype());
	}
	*/
	
	@org.junit.Test
	public void testToObject() {
		val = testEngine.toObject(scriptButton);
		assertTrue("val.isObject()", val.isObject());
	}

	@org.junit.Test
	public void testToNumber() {
		val = testEngine.evaluate("5 - 2");
		assertTrue("val.isNumber()", val.isNumber());
	}

}
