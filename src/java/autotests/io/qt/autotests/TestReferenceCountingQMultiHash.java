package io.qt.autotests;

import java.util.concurrent.atomic.AtomicInteger;

import org.junit.Assert;
import org.junit.Test;

import io.qt.autotests.generated.General;
import io.qt.autotests.generated.Tulip;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QMultiHash;
import io.qt.core.QObject;
import io.qt.core.QRunnable;
import io.qt.widgets.QGraphicsEllipseItem;
import io.qt.widgets.QGraphicsItem;
import io.qt.widgets.QGraphicsPathItem;
import io.qt.widgets.QWidgetItem;

public class TestReferenceCountingQMultiHash extends ApplicationInitializer {
	
	public static final int COUNT = 10;
	
	@Test
    public void test_new_QMultiHash_key_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiHash<QObject,String> container = new QMultiHash<>(QObject.class, String.class);
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
    public void test_new_QMultiHash_key_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiHash<QRunnable,String> container = new QMultiHash<>(QRunnable.class, String.class);
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
    public void test_new_QMultiHash_key_QGraphicsItem() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiHash<QGraphicsItem,String> container = new QMultiHash<>(QGraphicsItem.class, String.class);
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
    public void test_new_QMultiHash_value_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiHash<String,QObject> container = new QMultiHash<>(String.class, QObject.class);
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
    public void test_new_QMultiHash_value_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiHash<String,QRunnable> container = new QMultiHash<>(String.class, QRunnable.class);
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
    public void test_new_QMultiHash_value_QGraphicsItem() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiHash<String,QGraphicsItem> container = new QMultiHash<>(String.class, QGraphicsItem.class);
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
    public void test_new_QMultiHash_QObject_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiHash<QObject,QObject> container = new QMultiHash<>(QObject.class, QObject.class);
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
    public void test_new_QMultiHash_QRunnable_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiHash<QRunnable,QRunnable> container = new QMultiHash<>(QRunnable.class, QRunnable.class);
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
    public void test_new_QMultiHash_QGraphicsItem_QGraphicsItem() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiHash<QGraphicsItem,QGraphicsItem> container = new QMultiHash<>(QGraphicsItem.class, QGraphicsItem.class);
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
    public void test_cpp_QMultiHash_value_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiHash<String,QObject> container = Tulip.createStringMultiHashOfObjects();
	    	for(int i=0; i<100; ++i) {
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
        Assert.assertEquals(101, counter.get());
    }
    
    @Test
    public void test_cpp_QMultiHash_value_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiHash<String,QRunnable> container = Tulip.createStringMultiHashOfRunnables();
	    	for(int i=0; i<100; ++i) {
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
        Assert.assertEquals(101, counter.get());
    }
    
    @Test
    public void testQMultiHash_value_assign() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	QMultiHash<String,QWidgetItem> container = new QMultiHash<>(String.class, QWidgetItem.class);
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
    		container.assign(QMultiHash.of(""+item, item));
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
    public void testQMultiHash_assign() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	QMultiHash<QWidgetItem,QWidgetItem> container = new QMultiHash<>(QWidgetItem.class, QWidgetItem.class);
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
    		container.assign(QMultiHash.of(item, item2));
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
        container.swap(new QMultiHash<>(QWidgetItem.class, QWidgetItem.class));
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
    public void testQMultiHash_key_assign() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	QMultiHash<QWidgetItem,String> container = new QMultiHash<>(QWidgetItem.class, String.class);
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
    		container.assign(QMultiHash.of(item, ""));
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
