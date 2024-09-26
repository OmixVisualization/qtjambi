package io.qt.autotests;

import java.util.concurrent.atomic.AtomicInteger;

import org.junit.Assert;
import org.junit.Test;

import io.qt.autotests.generated.General;
import io.qt.core.QConstSpan;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QList;
import io.qt.core.QMetaType;
import io.qt.core.QObject;
import io.qt.core.QSpan;
import io.qt.gui.QTextCursor;
import io.qt.gui.QTextDocument;

public class TestReferenceCountingQSpanQt67 extends ApplicationInitializer {
	
	public static final int COUNT = 10;
    
	@Test
    public void test_java_QList_QSpan_int() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QList<QSpan<Integer>> container = new QList<>(QMetaType.fromType(QSpan.class, QMetaType.fromType(int.class)));
	    	for(int i=0; i<COUNT; ++i) {
	    		QSpan<Integer> object = QSpan.ofInt(new int[] {1,2,3});
	    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
	    		container.add(object);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        General.internalAccess.registerCleaner(container, counter::incrementAndGet);
	        container = null;
	    }
        for (int i = 0; i < 50 && counter.get()<COUNT+1; i++) {
            ApplicationInitializer.runGC();
            Thread.yield();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT+1, counter.get());
    }
	
	@Test
    public void test_java_QList_QSpan_const_int() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QList<QConstSpan<Integer>> container = new QList<>(QMetaType.fromType(QConstSpan.class, QMetaType.fromType(int.class)));
	    	for(int i=0; i<COUNT; ++i) {
	    		QConstSpan<Integer> object = QConstSpan.ofInt(1,2,3);
	    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
	    		container.add(object);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        General.internalAccess.registerCleaner(container, counter::incrementAndGet);
	        container = null;
	    }
        for (int i = 0; i < 50 && counter.get()<COUNT+1; i++) {
            ApplicationInitializer.runGC();
            Thread.yield();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT+1, counter.get());
    }
	
	@Test
    public void test_java_QList_QSpan_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QList<QSpan<QObject>> container = new QList<>(QMetaType.fromType(QSpan.class, QMetaType.fromType(QObject.class)));
	    	for(int i=0; i<COUNT; ++i) {
	    		QObject ptr = new QObject();
	    		General.internalAccess.registerCleaner(ptr, counter::incrementAndGet);
	    		QSpan<QObject> object = QSpan.of(new QObject[]{ptr});
	    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
	    		container.add(object);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        General.internalAccess.registerCleaner(container, counter::incrementAndGet);
	        container = null;
	    }
        for (int i = 0; i < 50 && counter.get()<COUNT*2+1; i++) {
            ApplicationInitializer.runGC();
            Thread.yield();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT*2+1, counter.get());
    }
	
	@Test
    public void test_owner_QSpan_TextCursor() throws InterruptedException {
    	QTextDocument owner = new QTextDocument();
		QSpan<QTextCursor> span = QSpan.of(new QTextCursor[]{new QTextCursor(owner)});
		Assert.assertEquals(1, span.size());
		Assert.assertEquals(owner, General.internalAccess.owner(span));
		span = null;
    }
}
