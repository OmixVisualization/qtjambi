package io.qt.autotests;

import java.io.Serializable;

import org.junit.Assert;
import org.junit.Test;

import io.qt.QtObjectInterface;
import io.qt.core.*;
import io.qt.internal.QtJambiInternal;
import io.qt.dbus.*;
import io.qt.autotests.generated.MetaObjectQtMetaCast;

public class TestDBusInterfaces extends ApplicationInitializer {
	
	static class ContextualObject extends QObject implements QDBusContext{
	}
	
	@Test
	public void test_QObjectSubClass() {
		ContextualObject object = new ContextualObject();
		Assert.assertFalse(object.isDisposed());
		object.isWidgetType();
		object.calledFromDBus();
		try {
			object.connection();
			Assert.assertFalse(true);
		} catch (IllegalStateException e) {
		}
		Assert.assertTrue(object.inherits("QDBusContext"));
		long ptr = MetaObjectQtMetaCast.do_qt_metacast(object, "QDBusContext");
		Assert.assertTrue(ptr!=0 && ptr!=-1);
		object.dispose();
		Assert.assertTrue("object not destroyed.", QtJambiInternal.tryIsObjectDisposed(object));
	}
	
	@Test
	public void test_AnonymousImpl() {
		QDBusContext object = new QDBusContext() {};
		Assert.assertFalse(object.isDisposed());
		object.calledFromDBus();
		object.dispose();
		Assert.assertTrue("object not destroyed.", QtJambiInternal.tryIsObjectDisposed(object));
	}

	@Test
	public void test_MultiInterfaceLambda() {
		QtObjectInterface object = (QRunnable & QDBusContext & Serializable) ()->{};
		Assert.assertFalse(object.isDisposed());
		((QRunnable)object).run();
		((QDBusContext)object).calledFromDBus();
		object.dispose();
		Assert.assertTrue("object not destroyed.", QtJambiInternal.tryIsObjectDisposed(object));
	}
}
