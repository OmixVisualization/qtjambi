package io.qt.autotests;

import java.io.Serializable;

import org.junit.Assert;
import org.junit.Test;

import io.qt.QtObjectInterface;
import io.qt.core.*;
import io.qt.internal.QtJambiInternal;
import io.qt.script.*;

public class TestScriptInterfaces extends ApplicationInitializer {
	
	static class ScriptableObject extends QObject implements QScriptable{
	}
	
	@Test
	public void test_QObjectSubClass() {
		ScriptableObject object = new ScriptableObject();
		Assert.assertFalse(object.isDisposed());
		object.argumentCount();
		object.dispose();
		Assert.assertTrue("object not destroyed.", QtJambiInternal.tryIsObjectDisposed(object));
	}
	
	@Test
	public void test_AnonymousImpl() {
		QScriptable object = new QScriptable() {};
		Assert.assertFalse(object.isDisposed());
		object.argumentCount();
		object.dispose();
		Assert.assertTrue("object not destroyed.", QtJambiInternal.tryIsObjectDisposed(object));
	}

	@Test
	public void test_MultiInterfaceLambda() {
		QtObjectInterface object = (QRunnable & QScriptable & Serializable) ()->{};
		Assert.assertFalse(object.isDisposed());
		((QRunnable)object).run();
		((QScriptable)object).argumentCount();
		object.dispose();
		Assert.assertTrue("object not destroyed.", QtJambiInternal.tryIsObjectDisposed(object));
	}
}
