package io.qt.autotests;

import java.util.concurrent.atomic.AtomicInteger;

import org.junit.Assert;
import org.junit.Test;

import io.qt.autotests.generated.General;
import io.qt.autotests.generated.Tulip;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QHash;
import io.qt.core.QObject;
import io.qt.core.QRunnable;
import io.qt.widgets.QWidgetItem;

public class TestReferenceCountingQHash extends ApplicationInitializer {
	
	public static final int COUNT = 10;
	
	@Test
    public void test_cpp_QHash_value_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QHash<String,QObject> container = Tulip.createStringHashOfObjects();
	    	for(int i=0; i<COUNT; ++i) {
	    		QObject object = new QObject();
	    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
	    		container.insert(""+i, object);
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
	        container.isDisposed();
	        container = null;
	    }
        for (int i = 0; i < 50 && counter.get()<101; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT+1, counter.get());
    }
    
    @Test
    public void test_cpp_QHash_value_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QHash<String,QRunnable> container = Tulip.createStringHashOfRunnables();
	    	for(int i=0; i<COUNT; ++i) {
	    		QRunnable object = new QRunnable() { @Override public void run() {} };
	    		container.insert(""+i, object);
	    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
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
        for (int i = 0; i < 50 && counter.get()<101; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT+1, counter.get());
    }
    
    @Test
    public void testQHash_value_assign() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	QHash<String,QWidgetItem> container = new QHash<>(String.class, QWidgetItem.class);
    	for (int i = 0; i < COUNT; i++) {
    		QWidgetItem item = new QWidgetItem(null);
    		General.internalAccess.registerCleaner(item, counter::incrementAndGet);
    		container.put(""+item, item);
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
    	{
    		QWidgetItem item = new QWidgetItem(null);
    		General.internalAccess.registerCleaner(item, counter::incrementAndGet);
    		container.assign(QHash.of(""+item, item));
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
        container.isDisposed();
        container = null;
    	for (int i = 0; i < 60 && counter.get()<COUNT+1; i++) {
            ApplicationInitializer.runGC();
            Thread.yield();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT+1, counter.get());
    }
    
    @Test
    public void testQHash_assign() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	QHash<QWidgetItem,QWidgetItem> container = new QHash<>(QWidgetItem.class, QWidgetItem.class);
    	for (int i = 0; i < COUNT; i++) {
    		QWidgetItem item = new QWidgetItem(null);
    		General.internalAccess.registerCleaner(item, counter::incrementAndGet);
    		QWidgetItem item2 = new QWidgetItem(null);
    		General.internalAccess.registerCleaner(item2, counter::incrementAndGet);
    		container.put(item, item2);
    		item = null;
    		item2 = null;
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
    	{
    		QWidgetItem item = new QWidgetItem(null);
    		General.internalAccess.registerCleaner(item, counter::incrementAndGet);
    		QWidgetItem item2 = new QWidgetItem(null);
    		General.internalAccess.registerCleaner(item2, counter::incrementAndGet);
    		container.assign(QHash.of(item, item2));
    		item = null;
    		item2 = null;
    	}
    	for (int i = 0; i < 50 && counter.get()<6; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(2*COUNT, counter.get());
        container.swap(new QHash<>(QWidgetItem.class, QWidgetItem.class));
    	for (int i = 0; i < 50 && counter.get()<2*COUNT+2; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(2*COUNT+2, counter.get());
    }
    
    @Test
    public void testQHash_key_assign() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	QHash<QWidgetItem,String> container = new QHash<>(QWidgetItem.class, String.class);
    	for (int i = 0; i < COUNT; i++) {
    		QWidgetItem item = new QWidgetItem(null);
    		General.internalAccess.registerCleaner(item, counter::incrementAndGet);
    		container.put(item, "");
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
    	{
    		QWidgetItem item = new QWidgetItem(null);
    		General.internalAccess.registerCleaner(item, counter::incrementAndGet);
    		container.assign(QHash.of(item, ""));
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
        container.isDisposed();
        container = null;
    	for (int i = 0; i < 60 && counter.get()<COUNT+1; i++) {
            ApplicationInitializer.runGC();
            Thread.yield();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT+1, counter.get());
    }
}
