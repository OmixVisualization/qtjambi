package io.qt.autotests;

import static org.junit.Assert.assertEquals;

import org.junit.Assert;
import org.junit.Test;

import io.qt.autotests.TestQml.TestObjectExn2;
import io.qt.core.QUrl;
import io.qt.qml.QtQml;
import io.qt.qt3d.core.quick.QQmlAspectEngine;

public class TestQml3DCore extends QApplicationTest{
	@Test
    public void testExceptionInInterface_QQmlAspectEngine() {
		QtQml.qmlClearTypeRegistrations();
		QtQml.qmlRegisterType(TestObjectExn2.class, "io.qt.test", 1, 0, "TestObject");
		QQmlAspectEngine component = new QQmlAspectEngine();
		try {
			component.setSource(QUrl.fromLocalFile("classpath:io/qt/autotests/qml/TestExn.qml"));
			Assert.assertFalse("Error expected to be thrown", true);
		} catch (AssertionError e) {
			throw e;
		} catch (Error e) {
			assertEquals("Cannot initialize interface io.qt.autotests.generated.TestInterface without arguments. Please use the private constructor and QtUtilities.initializeNativeObject(object, arguments...) instead.", e.getMessage());
		}
	}
}
