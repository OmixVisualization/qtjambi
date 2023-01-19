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

import org.junit.BeforeClass;

import io.qt.autotests.ApplicationInitializer;
import io.qt.core.QDateTime;
import io.qt.core.QObject;
import io.qt.core.QRegExp;
import io.qt.script.QScriptContext;
import io.qt.script.QScriptEngine;
import io.qt.script.QScriptString;
import io.qt.script.QScriptSyntaxCheckResult;
import io.qt.script.QScriptValue;
import io.qt.widgets.QApplication;
import io.qt.widgets.QPushButton;

public class TestQScriptEngine extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }

	private QScriptEngine testEngine;
	private QScriptEngine testEngineFromObj;
	private QObject engineParent;
	private QObject holder;

	private QPushButton button;
	private QPushButton button1;
	private QScriptValue scriptButton;
	private QScriptValue scriptButton1;
	private QScriptValue val;
	private QScriptValue val1;
	private QScriptString testString;
	private QScriptContext scriptContext;

	@org.junit.Before
	public void setUp() {
		testEngine = new QScriptEngine();
		engineParent = new QObject();
		holder = new QObject();
		button = new QPushButton();
		button1 = new QPushButton();
		scriptButton = testEngine.newQObject(button);
	}

	@org.junit.After
	public void tearDown() {
		testEngine = null;
		button = null;
		button1 = null;
		scriptButton = null;
		scriptButton1 = null;
		val = null;
		val1 = null;
		scriptContext = null;
        QApplication.processEvents();
	}

	@org.junit.Test
	public void testQscripEngineQObjConstructor() {
		testEngineFromObj = new QScriptEngine(engineParent);
		assertEquals("testEngineFromObj.parent()", engineParent,
				testEngineFromObj.parent());
		testEngineFromObj = new QScriptEngine(null);
		assertEquals("testEngineFromObj.parent()", null,
				testEngineFromObj.parent());
	}

	@org.junit.Test
	public void testEvaluate1() {
		assertTrue("1 + 2 == 3", testEngine.evaluate("1 + 2").toString().equals("3"));
	}
	@org.junit.Test
	public void testEvaluate2() {
		assertEquals("toInt(1 + 2) == 3", 3, testEngine.evaluate("1 + 2").toInt());
	}
	@org.junit.Test
	public void testEvaluate3() {
		assertFalse("1 == 2", testEngine.evaluate("1 == 2").toBoolean());
	}
	@org.junit.Test
	public void testEvaluate4() {
		assertEquals("for()", 120, testEngine.evaluate("var res = 1; for(var i = 1; i <= 5; i++)res = res*i;").toInt());
	}
	@org.junit.Test
	public void testEvaluate5() {
		assertEquals("1 + 2", 3, testEngine.evaluate("1 + 2.1").toInteger(), 0);
	}
	@org.junit.Test
	public void testEvaluate6() {
		assertEquals("3.14", 3.14, testEngine.evaluate("3.14").toDouble(), 0);
	}

	@org.junit.Test
	public void testSetProperty() {
		testEngine.globalObject().setProperty("button", scriptButton);
		scriptButton.setProperty("checkable", new QScriptValue(true));
		assertTrue("(button.checkable = true) == true",
				testEngine.evaluate("button.checkable = true").toBoolean());
	}

	@org.junit.Test
	public void testProperty() {
		scriptButton.setProperty("aBooleanProperty", new QScriptValue(true));
		assertTrue("aBooleanProperty", scriptButton
				.property("aBooleanProperty").toBoolean());
	}

	@org.junit.Test
	public void testNewQObject() {
		scriptButton1 = testEngine.newQObject(button);
		assertTrue("scriptButton1.isQObject()", scriptButton1.isQObject());
		val = testEngine.newQObject(null);
		assertTrue("val.isNull()", val.isNull());
	}

	@org.junit.Test
	public void testNewQObjectOwnerWrapperConstructor() {
		val1 = testEngine.newQObject(button,
				QScriptEngine.ValueOwnership.ScriptOwnership);
		assertTrue("val1.isQObject()", val1.isQObject());
		val = testEngine.newQObject(button,
				QScriptEngine.ValueOwnership.ScriptOwnership,
				QScriptEngine.QObjectWrapOption.ExcludeChildObjects);
		assertTrue("val.isQObject()", val.isQObject());
	}

	@org.junit.Test
	public void testNewQObjectScriptValueConstructor() {
		scriptButton = testEngine.newQObject(scriptButton1, button,
				QScriptEngine.ValueOwnership.ScriptOwnership);
		assertTrue("scriptButton.isQObject()", scriptButton.isQObject());
		// had QScriptEngine.QObjectWrapOption.ExcludeSlots option is not
		// present in older Qt releases
		val = testEngine.newQObject(val1, button1,
				QScriptEngine.ValueOwnership.ScriptOwnership,
				QScriptEngine.QObjectWrapOption.ExcludeChildObjects);
		assertTrue("val.isQObject()", val.isQObject());
	}

	@org.junit.Test
	public void testNewQObjectScriptValueObjectConstructor() {
		val = testEngine.newQObject(val1, button1);
		assertTrue("val.isQObject()", val.isQObject());
	}

	@org.junit.Test
	public void testNewQObjectNullQObj() {
		// had QScriptEngine.QObjectWrapOption.ExcludeSlots option is not
		// present in older Qt releases
		val = testEngine.newQObject(val1, null,
				QScriptEngine.ValueOwnership.ScriptOwnership,
				QScriptEngine.QObjectWrapOption.ExcludeChildObjects);
		assertTrue("val.isNull()", val.isNull());
	}

	@org.junit.Test
	public void testNewQObjectNullQScripVal() {
		// had QScriptEngine.QObjectWrapOption.ExcludeSlots option is not
		// present in older Qt releases
		val = testEngine.newQObject(null, button1,
				QScriptEngine.ValueOwnership.ScriptOwnership,
				QScriptEngine.QObjectWrapOption.ExcludeChildObjects);
		assertTrue("val.isQObject()", val.isQObject());
	}

	@org.junit.Test
	public void testNewQObjectBothNull() {
		// had QScriptEngine.QObjectWrapOption.ExcludeSlots option is not
		// present in older Qt releases
		val = testEngine.newQObject(null, null,
				QScriptEngine.ValueOwnership.ScriptOwnership,
				QScriptEngine.QObjectWrapOption.ExcludeChildObjects);
		assertTrue("val.isNull()", val.isNull());
	}

	@org.junit.Test
	public void testNewQArray() {
		val = testEngine.newArray(1);
		assertTrue("val.isArray()", val.isArray());
		val1 = testEngine.newArray();
		assertTrue("val1.isArray()", val1.isArray());
	}

	@org.junit.Test
	public void testNewDate() {
		val = testEngine.newDate(456789.0);
		assertTrue("456789.0 val.isDate()", val.isDate());
		val = testEngine.newDate(-42.42);
		assertTrue("-42.42 val.isDate()", val.isDate());
		val1 = testEngine.newDate(new QDateTime());
		assertTrue("new QDateTime() val1.isDate()", val1.isDate());
		val1 = testEngine.newDate(null);
		assertTrue("null val1.isDate()", val1.isDate());
		val1 = testEngine.newDate(new QDateTime().addSecs(456789));
		assertTrue("addSecs(456789) val1.isDate()", val1.isDate());
		val = testEngine.newDate(0.0);
		assertTrue("0.0 val.isDate()", val.isDate());
	}

	@org.junit.Test
	public void testNewObject() {
		val = testEngine.newObject();
		assertTrue("val.isObject()", val.isObject());
	}

	@org.junit.Test
	public void testNewRegExp() {
		val = testEngine.newRegExp(new QRegExp());
		assertTrue("val.isRegExp()", val.isRegExp());
		val1 = testEngine.newRegExp("ABCD", "g");
		assertTrue("val1.isRegExp()", val1.isRegExp());
		scriptButton = testEngine.newRegExp(null);
		assertTrue("scriptButton.isRegExp()", scriptButton.isRegExp());
	}

	@org.junit.Test
	public void testNewVariant() {
		val = testEngine.newVariant(new Object());
		assertTrue("val.isVariant()", val.isVariant());
		val1 = testEngine.newVariant(new QScriptValue(), new Object());
		assertTrue("val1.isVariant()", val1.isVariant());
	}

	@org.junit.Test
	public void testNullValue() {
		val = testEngine.nullValue();
		assertTrue("val.isNull()", val.isNull());
	}

	@org.junit.Test
	public void testPushPopContext() {
		testEngine.setProperty("global", new QScriptValue(3));
		scriptContext = testEngine.pushContext();
		scriptContext.activationObject().setProperty("local",
				new QScriptValue(5));
		assertEquals("global + local", 11,
				testEngine.evaluate("global = 6; global + local;").toInt());
		testEngine.popContext();
		assertEquals("global", 3,
				((QScriptValue) testEngine.property("global")).toInt());
	}

	/*
	 * The following method is obsolete in Qt 4.7.
	 * 
	 * How it works here: QScriptEngine.canEvaluate(java.lang.String) always
	 * returns with true, however if the program looks incomplete
	 * (Syntactically) it should return false.
	 * 
	 * Possible workarounds: 1. use QScriptEngine.checkSyntax(java.lang.String)
	 * instead (recommended) 2. check the return value of
	 * QScriptEngine.evaluate(java.lang.String) because it returns
	 * "SyntaxError: Parse error" when canEvaluate(java.lang.String program)
	 * should return false
	 */
	@org.junit.Test
	public void testCanEvaluate() {
		assertTrue("testEngine.canEvaluate(\"foo = 3;\")",
				testEngine.canEvaluate("foo = 3;"));
		// CHECKME Unable to make it ever return false in version: 4.7.3
		// assertFalse("testEngine.canEvaluate(\"foo[\"bar\"\")",
		// testEngine.canEvaluate("foo[\"bar\""));
	}

	@org.junit.Test
	public void testCheckSyntax() {
		assertEquals("Valid", QScriptSyntaxCheckResult.State.Valid,
				QScriptEngine.checkSyntax("foo = 3;").state());
		assertEquals("Error", QScriptSyntaxCheckResult.State.Error,
				QScriptEngine.checkSyntax("foo[\"bar\"").state());
		assertEquals("Intermediate",
				QScriptSyntaxCheckResult.State.Intermediate, QScriptEngine
						.checkSyntax("if (\n").state());
	}

	/*
	 * QtScriptEngine#objectById(long) exposes memory pointer dereference to
	 * QtJambi API. It is also marked as <i>internal</i> Qt C++ API.
	 */
	// @org.junit.Test
	// public void testObjectById() {
	// 		val = testEngine.newQObject(button);
	// 		QScriptValue valFound = testEngine.objectById(val.objectId());
	// 		assertEquals("val", val.objectId(), valFound.objectId());
	// }

	@org.junit.Test
	public void testProcessEventsInterval() {
		testEngine.setProcessEventsInterval(5);
		assertEquals("testEngine.processEventsInterval()", 5,
				testEngine.processEventsInterval());
	}

	@org.junit.Test
	public void testsetGlobalObject() {
		val = testEngine.newQObject(holder);
		testEngine.setGlobalObject(val);
		val.setProperty("testProperty1", new QScriptValue(5));
		assertEquals("testProperty1.toInt()", 5, testEngine.globalObject()
				.property("testProperty1").toInt());
		val.setProperty("testProperty1", new QScriptValue("test"));
		assertTrue("testProperty1 == test",
				testEngine.globalObject().property("testProperty1").toString()
						.equals("test"));
	}

	@org.junit.Test
	public void testToObject() {
		val = new QScriptValue(5);
		val1 = new QScriptValue();
		assertFalse("val1.isObject()", val1.isObject());
		val1 = testEngine.toObject(val);
		assertTrue("val1.isObject()", val1.isObject());
		val1 = testEngine.toObject(null);
		assertFalse("val1.isValid()", val1.isValid());
	}

	@org.junit.Test
	public void testToStringHandle() {
		val = testEngine.newQObject(holder);
		val.setProperty("testProperty", new QScriptValue("te$t"));
		testString = testEngine.toStringHandle("testProperty");
		assertTrue("testString == \"te$t\"", val.property(testString)
				.toString().equals("te$t"));
	}

}
