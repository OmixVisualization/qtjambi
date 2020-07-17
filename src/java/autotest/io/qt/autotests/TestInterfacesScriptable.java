package io.qt.autotests;

import java.io.Serializable;

import org.junit.Assert;
import org.junit.Test;

import io.qt.QtObjectInterface;
import io.qt.core.QRunnable;
import io.qt.internal.QtJambiInternal;
import io.qt.script.QScriptable;

public class TestInterfacesScriptable extends QApplicationTest {
	
	@Test
	public void test_InterfaceLiveCircle1() {
		QScriptable object = new QScriptable() {};
		Assert.assertFalse(object.isDisposed());
		object.argumentCount();
		object.dispose();
		Assert.assertTrue("object not destroyed.", QtJambiInternal.tryIsObjectDisposed(object));
	}

	@Test
	public void test_InterfaceLiveCircle4() {
		QtObjectInterface object = (QRunnable & QScriptable & Serializable) ()->{};
		Assert.assertFalse(object.isDisposed());
		((QRunnable)object).run();
		((QScriptable)object).argumentCount();
		object.dispose();
		Assert.assertTrue("object not destroyed.", QtJambiInternal.tryIsObjectDisposed(object));
	}
}
