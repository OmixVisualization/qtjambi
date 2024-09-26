package io.qt.autotests;

import java.util.Deque;
import java.util.TreeMap;
import java.util.concurrent.atomic.AtomicInteger;

import org.junit.Assert;
import org.junit.Test;

import io.qt.QtObject;
import io.qt.QtUtilities;
import io.qt.autotests.generated.General;
import io.qt.autotests.generated.Tulip;
import io.qt.core.QCoreApplication;
import io.qt.core.QEasingCurve;
import io.qt.core.QEvent;
import io.qt.core.QHash;
import io.qt.core.QLinkedList;
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

public class TestReferenceCountingQLinkedListNestedQt5 extends ApplicationInitializer {
	
	public static final int COUNT = 10;
    
    
    @Test
    public void test_java_QLinkedList_QLinkedList_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QLinkedList<QLinkedList<QObject>> container = new QLinkedList<>(QMetaType.fromType(QLinkedList.class, QMetaType.fromType(QObject.class)));
	    	for(int i=0; i<COUNT; ++i) {
	    		QObject object = new QObject();
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		container.add(QLinkedList.of(object));
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
	        QtUtilities.getSignalOnDispose(container).connect(counter::incrementAndGet);
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
    public void test_java_QLinkedList_QLinkedList_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
    		QLinkedList<QLinkedList<QRunnable>> container = new QLinkedList<>(QMetaType.fromType(QLinkedList.class, QMetaType.fromType(QRunnable.class)));
	    	for(int i=0; i<COUNT; ++i) {
	    		QRunnable object = new QRunnable() { @Override public void run() {} };
	    		container.add(QLinkedList.of(object));
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
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
	        QtUtilities.getSignalOnDispose(container).connect(counter::incrementAndGet);
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
    public void test_cpp_QLinkedList_QLinkedList_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QLinkedList<Deque<QObject>> container = Tulip.createLinkedListOfLinkedListOfObjects();
	    	for(int i=0; i<COUNT; ++i) {
	    		QObject object = new QObject();
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		container.add(QLinkedList.of(object));
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
	        QtUtilities.getSignalOnDispose(container).connect(counter::incrementAndGet);
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
    public void test_cpp_QLinkedList_QLinkedList_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QLinkedList<Deque<QRunnable>> container = Tulip.createLinkedListOfLinkedListOfRunnables();
	    	for(int i=0; i<COUNT; ++i) {
	    		QRunnable object = new QRunnable() { @Override public void run() {} };
	    		container.add(QLinkedList.of(object));
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
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
	        QtUtilities.getSignalOnDispose(container).connect(counter::incrementAndGet);
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
    public void test_java_QLinkedList_QLinkedList_QEasingCurve_EasingFunction() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	TreeMap<Integer,Double> calls = new TreeMap<>();
    	{
	    	QLinkedList<QLinkedList<QEasingCurve.EasingFunction>> container = new QLinkedList<>(QMetaType.fromType(QLinkedList.class, QMetaType.fromType(QEasingCurve.EasingFunction.class)));
	    	Assert.assertTrue(container!=null);
	    	for(int i=0; i<COUNT; ++i) {
	    		int _i = i;
	    		QEasingCurve.EasingFunction object = progress -> {
					calls.put(_i, progress);
					return 0;
	    		};
	    		container.add(QLinkedList.of(object));
	    		General.internalAccess.setJavaOwnership(object);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	    	for(QLinkedList<QEasingCurve.EasingFunction> list : container) {
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
	        QtUtilities.getSignalOnDispose(container).connect(counter::incrementAndGet);
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
    public void test_java_QLinkedList_QLinkedList_CustomPointerType() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QLinkedList<Deque<CustomRunnable>> container = new QLinkedList<>(QMetaType.fromType(QLinkedList.class, QMetaType.fromType(CustomRunnable.class)));
	    	for(int i=0; i<COUNT; ++i) {
	    		CustomRunnable object = new CustomRunnable();
	    		container.add(QLinkedList.of(object));
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
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
	        QtUtilities.getSignalOnDispose(container).connect(counter::incrementAndGet);
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
    public void test_java_QLinkedList_QSet_QEasingCurve_EasingFunction() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	TreeMap<Integer,Double> calls = new TreeMap<>();
    	{
	    	QLinkedList<QSet<QEasingCurve.EasingFunction>> container = new QLinkedList<>(QMetaType.fromType(QSet.class, QMetaType.fromType(QEasingCurve.EasingFunction.class)));
	    	Assert.assertTrue(container!=null);
	    	for(int i=0; i<COUNT; ++i) {
	    		int _i = i;
	    		QEasingCurve.EasingFunction object = progress -> {
					calls.put(_i, progress);
					return 0;
	    		};
	    		container.add(QSet.of(object));
	    		General.internalAccess.setJavaOwnership(object);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
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
	        QtUtilities.getSignalOnDispose(container).connect(counter::incrementAndGet);
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
    public void test_owner_QLinkedList_QPair_TextCursor() throws InterruptedException {
    	QTextDocument owner = new QTextDocument();
		QLinkedList<QPair<QTextCursor,QTextCursor>> list = QLinkedList.of(new QPair<>(new QTextCursor(owner), new QTextCursor(owner)));
		Assert.assertEquals(1, list.size());
		Assert.assertEquals(owner, General.internalAccess.owner(list));
		list = null;
    }
    
    @Test
    public void test_owner_QLinkedList_QLinkedList_TextCursor() throws InterruptedException {
    	QTextDocument owner = new QTextDocument();
		QLinkedList<QLinkedList<QTextCursor>> list = QLinkedList.of(QLinkedList.of(new QTextCursor(owner)));
		Assert.assertEquals(1, list.size());
		Assert.assertEquals(owner, General.internalAccess.owner(list));
		list = null;
    }
    
    @Test
    public void test_owner_QLinkedList_QMap_key_TextCursor() throws InterruptedException {
    	QTextDocument owner = new QTextDocument();
		QLinkedList<QMap<QTextCursor,String>> list = QLinkedList.of(QMap.of(new QTextCursor(owner), "QTextCursor"));
		Assert.assertEquals(1, list.size());
		Assert.assertEquals(owner, General.internalAccess.owner(list));
		list = null;
    }
    
    @Test
    public void test_owner_QLinkedList_QMultiHash_key_TextCursor() throws InterruptedException {
    	QTextDocument owner = new QTextDocument();
		QLinkedList<QMultiHash<QTextCursor,String>> list = QLinkedList.of(QMultiHash.of(new QTextCursor(owner), "QTextCursor"));
		Assert.assertEquals(1, list.size());
		Assert.assertEquals(owner, General.internalAccess.owner(list));
		list = null;
    }
    
    @Test
    public void test_owner_QLinkedList_QHash_key_TextCursor() throws InterruptedException {
    	QTextDocument owner = new QTextDocument();
		QLinkedList<QHash<QTextCursor,String>> list = QLinkedList.of(QHash.of(new QTextCursor(owner), "QTextCursor"));
		Assert.assertEquals(1, list.size());
		Assert.assertEquals(owner, General.internalAccess.owner(list));
		list = null;
    }
    
    @Test
    public void test_owner_QLinkedList_QMultiMap_key_TextCursor() throws InterruptedException {
    	QTextDocument owner = new QTextDocument();
		QLinkedList<QMultiMap<QTextCursor,String>> list = QLinkedList.of(QMultiMap.of(new QTextCursor(owner), "QTextCursor"));
		Assert.assertEquals(1, list.size());
		Assert.assertEquals(owner, General.internalAccess.owner(list));
		list = null;
    }
}
