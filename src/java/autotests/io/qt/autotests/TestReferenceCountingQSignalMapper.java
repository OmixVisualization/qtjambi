package io.qt.autotests;

import static org.junit.Assert.assertFalse;

import java.util.concurrent.atomic.AtomicInteger;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.autotests.generated.General;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QObject;
import io.qt.core.QSignalMapper;
import io.qt.gui.QGuiApplication;
import io.qt.widgets.QWidget;

public class TestReferenceCountingQSignalMapper extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
        ApplicationInitializer.testInitializeWithWidgets();
		Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
    }
	
	public static final int COUNT = 10;
	
	@Test
    public final void testMappingInt() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QSignalMapper mapper = new QSignalMapper();
			for (int i=0; i<COUNT; ++i) {
				QObject object = new QObject();
				General.internalAccess.registerCleaner(object, counter::incrementAndGet);
				mapper.setMapping(object, i);
				object = null;
			}
			for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
			Assert.assertEquals(0, counter.get());
			mapper = null;
		}
		for (int i = 0; i < 50 && counter.get()<COUNT; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT, counter.get());
	}
	
	@Test
    public final void testMappingString() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QSignalMapper mapper = new QSignalMapper();
			for (int i=0; i<COUNT; ++i) {
				QObject object = new QObject();
				General.internalAccess.registerCleaner(object, counter::incrementAndGet);
				mapper.setMapping(object, ""+i);
				object = null;
			}
			for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
			Assert.assertEquals(0, counter.get());
			mapper = null;
		}
		for (int i = 0; i < 50 && counter.get()<COUNT; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT, counter.get());
	}
	
	@Test
    public final void testMappingObject() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QSignalMapper mapper = new QSignalMapper();
			for (int i=0; i<COUNT; ++i) {
				QObject object = new QObject();
				QObject object2 = new QObject();
				General.internalAccess.registerCleaner(object, counter::incrementAndGet);
				General.internalAccess.registerCleaner(object2, counter::incrementAndGet);
				mapper.setMapping(object, object2);
				object = null;
				object2 = null;
			}
			for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
			Assert.assertEquals(0, counter.get());
			mapper = null;
		}
		for (int i = 0; i < 50 && counter.get()<COUNT*2; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT*2, counter.get());
	}

    @Test
    public final void setWidgetMappingToNull() {
		QSignalMapper mapper = new QSignalMapper();

        boolean caughtException = false;
        try {
            mapper.setMapping(new QObject(), (QWidget) null);
        } catch (NullPointerException e) {
            caughtException = true;
        }

        assertFalse(caughtException);
    }

    @Test
    public final void setObjectMappingToNull() {
        QSignalMapper mapper = new QSignalMapper();

        boolean caughtException = false;
        try {
            mapper.setMapping(new QObject(), (QObject) null);
        } catch (NullPointerException e) {
            caughtException = true;
        }

        assertFalse(caughtException);
    }
}
