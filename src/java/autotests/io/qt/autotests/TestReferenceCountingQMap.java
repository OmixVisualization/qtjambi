package io.qt.autotests;

import java.util.concurrent.atomic.AtomicInteger;

import org.junit.Assert;
import org.junit.Test;

import io.qt.QtUtilities;
import io.qt.autotests.generated.General;
import io.qt.autotests.generated.Tulip;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QMap;
import io.qt.core.QObject;
import io.qt.core.QRunnable;
import io.qt.widgets.QWidgetItem;

public class TestReferenceCountingQMap extends ApplicationInitializer {
	
	public static final int COUNT = 10;
	
	@Test
    public void test_cpp_QMap_value_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMap<String,QObject> container = Tulip.createStringMapOfObjects();
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
    public void test_cpp_QMap_value_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMap<String,QRunnable> container = Tulip.createStringMapOfRunnables();
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
    public void testQMap_assign() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	QMap<QWidgetItem,QWidgetItem> container = new QMap<>(QWidgetItem.class, QWidgetItem.class);
    	for (int i = 0; i < COUNT; i++) {
    		QWidgetItem item = new QWidgetItem(null);
    		QtUtilities.getSignalOnDispose(item).connect(counter::incrementAndGet);
    		QWidgetItem item2 = new QWidgetItem(null);
    		QtUtilities.getSignalOnDispose(item2).connect(counter::incrementAndGet);
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
    		QtUtilities.getSignalOnDispose(item).connect(counter::incrementAndGet);
    		QWidgetItem item2 = new QWidgetItem(null);
    		QtUtilities.getSignalOnDispose(item2).connect(counter::incrementAndGet);
    		QMap<QWidgetItem,QWidgetItem> map = QMap.of(item, item2);
    		Assert.assertTrue(General.internalAccess.isJavaOwnership(map));
    		container.assign(map);
    		Assert.assertTrue(General.internalAccess.isJavaOwnership(map));
    		map = null;
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
        container.swap(new QMap<>(QWidgetItem.class, QWidgetItem.class));
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
    public void testQMap_value_assign() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	QMap<String,QWidgetItem> container = new QMap<>(String.class, QWidgetItem.class);
    	for (int i = 0; i < COUNT; i++) {
    		QWidgetItem item = new QWidgetItem(null);
    		QtUtilities.getSignalOnDispose(item).connect(counter::incrementAndGet);
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
    		QtUtilities.getSignalOnDispose(item).connect(counter::incrementAndGet);
    		container.assign(QMap.of(""+item, item));
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
        container = null;
    	for (int i = 0; i < 50 && counter.get()<COUNT+1; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT+1, counter.get());
    }
    
    @Test
    public void testQMap_key_assign() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	QMap<QWidgetItem,String> container = new QMap<>(QWidgetItem.class, String.class);
    	for (int i = 0; i < COUNT; i++) {
    		QWidgetItem item = new QWidgetItem(null);
    		QtUtilities.getSignalOnDispose(item).connect(counter::incrementAndGet);
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
    		QtUtilities.getSignalOnDispose(item).connect(counter::incrementAndGet);
    		container.assign(QMap.of(item, ""));
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
        container = null;
    	for (int i = 0; i < 50 && counter.get()<COUNT+1; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT+1, counter.get());
    }
}
