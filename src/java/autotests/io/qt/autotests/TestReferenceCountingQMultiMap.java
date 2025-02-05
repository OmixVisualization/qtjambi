package io.qt.autotests;

import java.util.concurrent.atomic.AtomicInteger;

import org.junit.Assert;
import org.junit.Test;

import io.qt.autotests.generated.General;
import io.qt.autotests.generated.Tulip;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QMultiMap;
import io.qt.core.QObject;
import io.qt.core.QRunnable;
import io.qt.widgets.QGraphicsEllipseItem;
import io.qt.widgets.QGraphicsItem;
import io.qt.widgets.QGraphicsPathItem;
import io.qt.widgets.QWidgetItem;

public class TestReferenceCountingQMultiMap extends ApplicationInitializer {
	
	public static final int COUNT = 10;
	
	@Test
    public void test_new_QMultiMap_key_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiMap<QObject,String> container = new QMultiMap<>(QObject.class, String.class);
	    	for(int i=0; i<COUNT; ++i) {
	    		QObject object = new QObject();
	    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
	    		container.insert(object, "");
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
    public void test_new_QMultiMap_key_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiMap<QRunnable,String> container = new QMultiMap<>(QRunnable.class, String.class);
	    	for(int i=0; i<COUNT; ++i) {
	    		QRunnable object = new QRunnable() { @Override public void run() {} };
	    		container.insert(object, "");
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
    public void test_new_QMultiMap_key_QGraphicsItem() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiMap<QGraphicsItem,String> container = new QMultiMap<>(QGraphicsItem.class, String.class);
	    	for(int i=0; i<COUNT; ++i) {
	    		QGraphicsItem object = new QGraphicsPathItem();
	    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
	    		container.insert(object, "");
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
    public void test_new_QMultiMap_value_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiMap<String,QObject> container = new QMultiMap<>(String.class, QObject.class);
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
    public void test_new_QMultiMap_value_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiMap<String,QRunnable> container = new QMultiMap<>(String.class, QRunnable.class);
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
    public void test_new_QMultiMap_value_QGraphicsItem() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiMap<String,QGraphicsItem> container = new QMultiMap<>(String.class, QGraphicsItem.class);
	    	for(int i=0; i<COUNT; ++i) {
	    		QGraphicsItem object = new QGraphicsPathItem();
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
    public void test_new_QMultiMap_QObject_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiMap<QObject,QObject> container = new QMultiMap<>(QObject.class, QObject.class);
	    	for(int i=0; i<COUNT; ++i) {
	    		QObject key = new QObject();
	    		QObject object = new QObject();
	    		container.insert(key, object);
	    		General.internalAccess.registerCleaner(key, counter::incrementAndGet);
	    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
	    		key = null;
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
        for (int i = 0; i < 20 && counter.get()<201; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(2*COUNT+1, counter.get());
    }
    
    @Test
    public void test_new_QMultiMap_QRunnable_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiMap<QRunnable,QRunnable> container = new QMultiMap<>(QRunnable.class, QRunnable.class);
	    	for(int i=0; i<COUNT; ++i) {
	    		QRunnable key = new QRunnable() { @Override public void run() {} };
	    		QRunnable object = new QRunnable() { @Override public void run() {} };
	    		container.insert(key, object);
	    		General.internalAccess.registerCleaner(key, counter::incrementAndGet);
	    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
	    		key = null;
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
        for (int i = 0; i < 20 && counter.get()<201; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(2*COUNT+1, counter.get());
    }
    
    @Test
    public void test_new_QMultiMap_QGraphicsItem_QGraphicsItem() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiMap<QGraphicsItem,QGraphicsItem> container = new QMultiMap<>(QGraphicsItem.class, QGraphicsItem.class);
	    	for(int i=0; i<COUNT; ++i) {
	    		QGraphicsItem key = new QGraphicsEllipseItem();
	    		QGraphicsItem object = new QGraphicsPathItem();
	    		container.insert(key, object);
	    		General.internalAccess.registerCleaner(key, counter::incrementAndGet);
	    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
	    		key = null;
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
        for (int i = 0; i < 20 && counter.get()<201; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(2*COUNT+1, counter.get());
    }
    
    @Test
    public void test_cpp_QMultiMap_value_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiMap<String,QObject> container = Tulip.createStringMultiMapOfObjects();
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
    public void test_cpp_QMultiMap_value_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiMap<String,QRunnable> container = Tulip.createStringMultiMapOfRunnables();
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
    public void testQMultiMap_value_assign() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	QMultiMap<String,QWidgetItem> container = new QMultiMap<>(String.class, QWidgetItem.class);
    	for (int i = 0; i < COUNT; i++) {
    		QWidgetItem item = new QWidgetItem(null);
    		General.internalAccess.registerCleaner(item, counter::incrementAndGet);
    		container.insert(""+item, item);
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
    		container.assign(QMultiMap.of(""+item, item));
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
    public void testQMultiMap_assign() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	QMultiMap<QWidgetItem,QWidgetItem> container = new QMultiMap<>(QWidgetItem.class, QWidgetItem.class);
    	for (int i = 0; i < COUNT; i++) {
    		QWidgetItem item = new QWidgetItem(null);
    		General.internalAccess.registerCleaner(item, counter::incrementAndGet);
    		QWidgetItem item2 = new QWidgetItem(null);
    		General.internalAccess.registerCleaner(item2, counter::incrementAndGet);
    		container.insert(item, item2);
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
    		container.assign(QMultiMap.of(item, item2));
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
        container.swap(new QMultiMap<>(QWidgetItem.class, QWidgetItem.class));
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
    public void testQMultiMap_key_assign() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	QMultiMap<QWidgetItem,String> container = new QMultiMap<>(QWidgetItem.class, String.class);
    	for (int i = 0; i < COUNT; i++) {
    		QWidgetItem item = new QWidgetItem(null);
    		General.internalAccess.registerCleaner(item, counter::incrementAndGet);
    		container.insert(item, "");
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
    		container.assign(QMultiMap.of(item, ""));
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
    	for (int i = 0; i < 50 && counter.get()<COUNT+1; i++) {
            ApplicationInitializer.runGC();
            Thread.yield();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT+1, counter.get());
    }
}
