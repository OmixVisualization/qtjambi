package io.qt.autotests;

import java.util.List;
import java.util.TreeMap;
import java.util.concurrent.atomic.AtomicInteger;

import org.junit.Assert;
import org.junit.Test;

import io.qt.QtObject;
import io.qt.QtUtilities;
import io.qt.autotests.generated.ContainerTest;
import io.qt.autotests.generated.General;
import io.qt.autotests.generated.QList_QHash_QObject_QObject;
import io.qt.autotests.generated.QList_QList_QObject;
import io.qt.autotests.generated.Tulip;
import io.qt.core.QCoreApplication;
import io.qt.core.QEasingCurve;
import io.qt.core.QEvent;
import io.qt.core.QHash;
import io.qt.core.QList;
import io.qt.core.QMap;
import io.qt.core.QMetaType;
import io.qt.core.QMultiHash;
import io.qt.core.QMultiMap;
import io.qt.core.QObject;
import io.qt.core.QPair;
import io.qt.core.QRunnable;
import io.qt.core.QSet;
import io.qt.gui.QTextCursor;
import io.qt.gui.QTextDocument;
import io.qt.widgets.QWidgetItem;

public class TestReferenceCountingQListNested extends ApplicationInitializer {
	
	public static final int COUNT = 10;
	
    @Test
    public void test_java_QList_QPair_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QList<QPair<QObject,QObject>> container = new QList<>(QMetaType.fromType(QPair.class, QMetaType.fromType(QObject.class), QMetaType.fromType(QObject.class)));
	    	for(int i=0; i<COUNT; ++i) {
	    		QObject object = new QObject();
	    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
	    		QObject object2 = new QObject();
	    		QtUtilities.getSignalOnDispose(object2).connect(counter::incrementAndGet);
	    		container.add(new QPair<>(object, object2));
	    		object = null;
	    		object2 = null;
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
    public void test_java_QList_QList_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QList<QList<QObject>> container = new QList<>(QMetaType.fromType(QList.class, QMetaType.fromType(QObject.class)));
	    	for(int i=0; i<COUNT; ++i) {
	    		QObject object = new QObject();
	    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
	    		container.add(QList.of(object));
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
    public void test_derived_QList_QList_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	QList<QList<QObject>> clone;
    	{
	    	QList<QList<QObject>> container = new QList_QList_QObject();
	    	for(int i=0; i<COUNT; ++i) {
	    		QObject object = new QObject();
	    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
	    		container.add(QList.of(object));
	    		Assert.assertTrue(General.internalAccess.isJavaOwnership(object));
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
	        clone = container.clone();
	        container.dispose();
	        General.internalAccess.registerCleaner(clone, counter::incrementAndGet);
	        container = null;
	    }
    	for (int i = 0; i < 50 && counter.get()<1; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(1, counter.get());
        clone = null;
        for (int i = 0; i < 50 && counter.get()<COUNT+2; i++) {
            ApplicationInitializer.runGC();
            Thread.yield();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT+2, counter.get());
    }
    
    @Test
    public void test_derived_prefilled_QList_QList_QObject() throws InterruptedException {
    	QObject nativeObject = null;
    	try {
	    	AtomicInteger counter = new AtomicInteger();
	    	QList<QList<QObject>> clone;
	    	{
	    		QList<QObject> first;
	    		{
		    		QObject object = new QObject();
		    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
		    		first = QList.of(object);
		    		Assert.assertTrue(General.internalAccess.isJavaOwnership(object));
		    		object = null;
		    	}
		    	QList<QList<QObject>> container = ContainerTest.asListOfLists(null, first);
		    	Assert.assertEquals(1, container.size());
		    	nativeObject = container.first().first();
		    	General.internalAccess.registerCleaner(nativeObject, counter::incrementAndGet);
		    	Assert.assertTrue(General.internalAccess.isSplitOwnership(nativeObject));
		    	for(int i=0; i<COUNT; ++i) {
		    		QObject object = new QObject();
		    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
		    		container.add(QList.of(object));
		    		Assert.assertTrue(General.internalAccess.isJavaOwnership(object));
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
		        clone = container.clone();
		        container.dispose();
		        General.internalAccess.registerCleaner(clone, counter::incrementAndGet);
		        container = null;
		    }
	    	for (int i = 0; i < 50 && counter.get()<1; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(1, counter.get());
	        clone = null;
	        for (int i = 0; i < 50 && counter.get()<COUNT+3; i++) {
	            ApplicationInitializer.runGC();
	            Thread.yield();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(COUNT+3, counter.get());
    	}finally {
    		if(nativeObject!=null)
    			nativeObject.dispose();
    	}
    }
    
    @Test
    public void test_derived_QList_QHash_QObject_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	QList<QHash<QObject,QObject>> clone;
    	{
	    	QList<QHash<QObject,QObject>> container = new QList_QHash_QObject_QObject();
	    	for(int i=0; i<COUNT; ++i) {
	    		QObject object = new QObject();
	    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
	    		container.add(QHash.of(object,object));
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
	        clone = container.clone();
	        container.dispose();
	        General.internalAccess.registerCleaner(clone, counter::incrementAndGet);
	        container = null;
	    }
    	for (int i = 0; i < 50 && counter.get()<1; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(1, counter.get());
        clone = null;
        for (int i = 0; i < 50 && counter.get()<COUNT+2; i++) {
            ApplicationInitializer.runGC();
            Thread.yield();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT+2, counter.get());
    }
    
    @Test
    public void test_derived_prefilled_QList_QHash_QObject_QObject() throws InterruptedException {
    	QObject nativeObject = null;
    	try {
	    	AtomicInteger counter = new AtomicInteger();
	    	QList<QHash<QObject,QObject>> clone;
	    	{
	    		QHash<QObject,QObject> first;
	    		{
		    		QObject object = new QObject();
		    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
		    		Assert.assertTrue(General.internalAccess.isJavaOwnership(object));
		    		first = QHash.of(object,object);
		    	}
	    		QList<QHash<QObject,QObject>> container = ContainerTest.asListOfHashs(null, first);
		    	Assert.assertEquals(1, container.size());
		    	nativeObject = container.first().get(null);
		    	General.internalAccess.registerCleaner(nativeObject, counter::incrementAndGet);
		    	Assert.assertTrue(General.internalAccess.isSplitOwnership(nativeObject));
		    	for(int i=0; i<COUNT; ++i) {
		    		QObject object = new QObject();
		    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
		    		container.add(QHash.of(object,object));
		    		Assert.assertTrue(General.internalAccess.isJavaOwnership(object));
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
		        clone = container.clone();
		        container.dispose();
		        General.internalAccess.registerCleaner(clone, counter::incrementAndGet);
		        container = null;
		    }
	    	for (int i = 0; i < 50 && counter.get()<1; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(1, counter.get());
	        clone = null;
	        for (int i = 0; i < 50 && counter.get()<COUNT+3; i++) {
	            ApplicationInitializer.runGC();
	            Thread.yield();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(COUNT+3, counter.get());
    	}finally {
    		if(nativeObject!=null)
    			nativeObject.dispose();
    	}
    }
    
    @Test
    public void test_java_QList_QList_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
    		QList<QList<QRunnable>> container = new QList<>(QMetaType.fromType(QList.class, QMetaType.fromType(QRunnable.class)));
	    	for(int i=0; i<COUNT; ++i) {
	    		QRunnable object = new QRunnable() { @Override public void run() {} };
	    		container.add(QList.of(object));
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
	        container.isDisposed();
	        container = null;
    	}
		for (int i = 0; i < 50 && counter.get() < COUNT+1; i++) {
            ApplicationInitializer.runGC();
            Thread.yield();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(50);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT+1, counter.get());
    }
    
    @Test
    public void test_cpp_QList_QList_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QList<List<QObject>> container = Tulip.createListOfListOfObjects();
	    	for(int i=0; i<COUNT; ++i) {
	    		QObject object = new QObject();
	    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
	    		container.add(QList.of(object));
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
    public void test_cpp_QList_QList_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QList<List<QRunnable>> container = Tulip.createListOfListOfRunnables();
	    	for(int i=0; i<COUNT; ++i) {
	    		QRunnable object = new QRunnable() { @Override public void run() {} };
	    		container.add(QList.of(object));
	    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            Thread.yield();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(50);
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
    public void test_java_QList_QList_QEasingCurve_EasingFunction() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	TreeMap<Integer,Double> calls = new TreeMap<>();
    	{
	    	QList<QList<QEasingCurve.EasingFunction>> container = new QList<>(QMetaType.fromType(QList.class, QMetaType.fromType(QEasingCurve.EasingFunction.class)));
	    	Assert.assertTrue(container!=null);
	    	for(int i=0; i<COUNT; ++i) {
	    		int _i = i;
	    		QEasingCurve.EasingFunction object = progress -> {
					calls.put(_i, progress);
					return 0;
	    		};
	    		container.add(QList.of(object));
//	    		General.internalAccess.setJavaOwnership(object);
	    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
	    		object = null;
	    	}
	    	for(QList<QEasingCurve.EasingFunction> list : container) {
		    	Tulip.testEasingFunctions(list);
	    	}
	        Assert.assertEquals(COUNT, calls.size());
	        for (int i = 0; i < calls.size(); i++) {
	        	Assert.assertEquals(0, calls.get(i).intValue());
			}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            Thread.yield();
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
            Thread.yield();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT+1, counter.get());
    }
    
    public static class CustomRunnable extends QtObject implements QRunnable{
    	@Override public void run() {}
    }
    
    @Test
    public void test_java_QList_QList_CustomPointerType() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QList<List<CustomRunnable>> container = new QList<>(QMetaType.fromType(QList.class, QMetaType.fromType(CustomRunnable.class)));
	    	for(int i=0; i<COUNT; ++i) {
	    		CustomRunnable object = new CustomRunnable();
	    		container.add(QList.of(object));
	    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            Thread.yield();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(50);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        General.internalAccess.registerCleaner(container, counter::incrementAndGet);
	        container.isDisposed();
	        container = null;
    	}
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
    public void test_java_QList_QSet_QEasingCurve_EasingFunction() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	TreeMap<Integer,Double> calls = new TreeMap<>();
    	{
	    	QList<QSet<QEasingCurve.EasingFunction>> container = new QList<>(QMetaType.fromType(QSet.class, QMetaType.fromType(QEasingCurve.EasingFunction.class)));
	    	Assert.assertTrue(container!=null);
	    	for(int i=0; i<COUNT; ++i) {
	    		int _i = i;
	    		QEasingCurve.EasingFunction object = progress -> {
					calls.put(_i, progress);
					return 0;
	    		};
	    		container.add(QSet.of(object));
//	    		General.internalAccess.setJavaOwnership(object);
	    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
	    		object = null;
	    	}
	    	for(QSet<QEasingCurve.EasingFunction> list : container) {
		    	Tulip.testEasingFunctions(list);
	    	}
	        Assert.assertEquals(COUNT, calls.size());
	        for (int i = 0; i < calls.size(); i++) {
	        	Assert.assertEquals(0, calls.get(i).intValue());
			}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            Thread.yield();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        General.internalAccess.registerCleaner(container, counter::incrementAndGet);
	        container.isDisposed();
	        container = null;
    	}
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
    
    @Test
    public void testQList_QList_mid() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	QList<QList<QWidgetItem>> list = new QList<>(QMetaType.fromType(QList.class, QMetaType.fromType(QWidgetItem.class)));
    	for (int i = 0; i < COUNT; i++) {
    		QWidgetItem item = new QWidgetItem(null);
    		QtUtilities.getSignalOnDispose(item).connect(counter::incrementAndGet);
    		list.add(QList.of(item));
		}
		QList<QList<QWidgetItem>> mid = list.mid(2, 1);
    	for (int i = 0; i < 20 && counter.get()==0; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(0, counter.get());
    	list = null;
        for (int i = 0; i < 50 && counter.get()<COUNT-1; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT-1, counter.get());
        QList<QWidgetItem> first = mid.takeFirst();
        mid = null;
        for (int i = 0; i < 20 && counter.get()==0; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT-1, counter.get());
        @SuppressWarnings("unused")
		QWidgetItem firstItem = first.takeFirst();
        firstItem = null;
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
    public void test_owner_QList_QList_TextCursor() throws InterruptedException {
    	QTextDocument owner = new QTextDocument();
		QList<QList<QTextCursor>> list = QList.of(QList.of(new QTextCursor(owner)));
		Assert.assertEquals(1, list.size());
		Assert.assertEquals(owner, General.internalAccess.owner(list));
		list = null;
    }
    
    @Test
    public void test_owner_QList_QPair_TextCursor() throws InterruptedException {
    	QTextDocument owner = new QTextDocument();
		QList<QPair<QTextCursor,QTextCursor>> list = QList.of(new QPair<>(new QTextCursor(owner), new QTextCursor(owner)));
		Assert.assertEquals(1, list.size());
		Assert.assertEquals(owner, General.internalAccess.owner(list));
		list = null;
    }
    
    @Test
    public void test_owner_QList_QMap_key_TextCursor() throws InterruptedException {
    	QTextDocument owner = new QTextDocument();
		QList<QMap<QTextCursor,String>> list = QList.of(QMap.of(new QTextCursor(owner), "QTextCursor"));
		Assert.assertEquals(1, list.size());
		Assert.assertEquals(owner, General.internalAccess.owner(list));
		list = null;
    }
    
    @Test
    public void test_owner_QList_QMultiHash_key_TextCursor() throws InterruptedException {
    	QTextDocument owner = new QTextDocument();
		QList<QMultiHash<QTextCursor,String>> list = QList.of(QMultiHash.of(new QTextCursor(owner), "QTextCursor"));
		Assert.assertEquals(1, list.size());
		Assert.assertEquals(owner, General.internalAccess.owner(list));
		list = null;
    }
    
    @Test
    public void test_owner_QList_QHash_key_TextCursor() throws InterruptedException {
    	QTextDocument owner = new QTextDocument();
		QList<QHash<QTextCursor,String>> list = QList.of(QHash.of(new QTextCursor(owner), "QTextCursor"));
		Assert.assertEquals(1, list.size());
		Assert.assertEquals(owner, General.internalAccess.owner(list));
		list = null;
    }
    
    @Test
    public void test_owner_QList_QMultiMap_key_TextCursor() throws InterruptedException {
    	QTextDocument owner = new QTextDocument();
		QList<QMultiMap<QTextCursor,String>> list = QList.of(QMultiMap.of(new QTextCursor(owner), "QTextCursor"));
		Assert.assertEquals(1, list.size());
		Assert.assertEquals(owner, General.internalAccess.owner(list));
		list = null;
    }
}
