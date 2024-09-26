package io.qt.autotests;

import java.util.TreeMap;
import java.util.concurrent.atomic.AtomicInteger;

import org.junit.Assert;
import org.junit.Test;

import io.qt.QtUtilities;
import io.qt.autotests.generated.General;
import io.qt.autotests.generated.Tulip;
import io.qt.autotests.generated.Variants;
import io.qt.core.QCoreApplication;
import io.qt.core.QEasingCurve;
import io.qt.core.QEvent;
import io.qt.core.QItemSelection;
import io.qt.core.QObject;
import io.qt.core.QRunnable;
import io.qt.core.QLinkedList;
import io.qt.gui.QStandardItemModel;
import io.qt.gui.QTextCursor;
import io.qt.gui.QTextDocument;
import io.qt.widgets.QGraphicsItem;
import io.qt.widgets.QGraphicsPathItem;
import io.qt.widgets.QWidgetItem;

public class TestReferenceCountingQLinkedListQt5 extends ApplicationInitializer {
	
	public static final int COUNT = 10;
	
	@Test
    public void test_new_QLinkedList_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QLinkedList<QObject> container = new QLinkedList<>(QObject.class);
	    	for(int i=0; i<COUNT; ++i) {
	    		QObject object = new QObject();
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		container.add(object);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingQLinkedListQt5.class) {
	            	Thread.sleep(25);
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
            synchronized(TestReferenceCountingQLinkedListQt5.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT+1, counter.get());
    }
    
    @Test
    public void test_new_QLinkedList_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QLinkedList<QRunnable> container = new QLinkedList<>(QRunnable.class);
	    	for(int i=0; i<COUNT; ++i) {
	    		QRunnable object = new QRunnable() { @Override public void run() {} };
	    		container.add(object);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingQLinkedListQt5.class) {
	            	Thread.sleep(25);
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
            synchronized(TestReferenceCountingQLinkedListQt5.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT+1, counter.get());
    }
    
    @Test
    public void test_new_QLinkedList_QGraphicsItem() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QLinkedList<QGraphicsItem> container = new QLinkedList<>(QGraphicsItem.class);
	    	for(int i=0; i<COUNT; ++i) {
	    		QGraphicsItem object = new QGraphicsPathItem();
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		container.add(object);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingQLinkedListQt5.class) {
	            	Thread.sleep(25);
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
            synchronized(TestReferenceCountingQLinkedListQt5.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT+1, counter.get());
    }
    
    @Test
    public void test_cpp_QLinkedList_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QLinkedList<QObject> container = Tulip.createLinkedListOfObjects();
	    	for(int i=0; i<100; ++i) {
	    		QObject object = new QObject();
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		container.add(object);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingQLinkedListQt5.class) {
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
            synchronized(TestReferenceCountingQLinkedListQt5.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(101, counter.get());
    }
    
    @Test
    public void test_cpp_QLinkedList_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QLinkedList<QRunnable> container = Tulip.createLinkedListOfRunnables();
	    	for(int i=0; i<100; ++i) {
	    		QRunnable object = new QRunnable() { @Override public void run() {} };
	    		container.add(object);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingQLinkedListQt5.class) {
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
            synchronized(TestReferenceCountingQLinkedListQt5.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(101, counter.get());
    }
    
    @Test
    public void test_cpp_QLinkedList_QEasingCurve_EasingFunction() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	TreeMap<Integer,Double> calls = new TreeMap<>();
    	{
	    	QLinkedList<QEasingCurve.EasingFunction> container = Tulip.createLinkedListOfEasingFunctions();
	    	Assert.assertTrue(container!=null);
	    	for(int i=0; i<100; ++i) {
	    		int _i = i;
	    		QEasingCurve.EasingFunction object = progress -> {
					calls.put(_i, progress);
					return 0;
	    		};
	    		container.add(object);
	    		General.internalAccess.setJavaOwnership(object);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	    	Tulip.testEasingFunctions(container);
	        Assert.assertEquals(100, calls.size());
	        for (int i = 0; i < calls.size(); i++) {
	        	Assert.assertEquals(i+1, calls.get(i).intValue());
			}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingQLinkedListQt5.class) {
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
            synchronized(TestReferenceCountingQLinkedListQt5.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(101, counter.get());
    }
    
    @Test
    public void test_java_QLinkedList_QEasingCurve_EasingFunction() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	TreeMap<Integer,Double> calls = new TreeMap<>();
    	{
	    	QLinkedList<QEasingCurve.EasingFunction> container = new QLinkedList<>(QEasingCurve.EasingFunction.class);
	    	Assert.assertTrue(container!=null);
	    	for(int i=0; i<100; ++i) {
	    		int _i = i;
	    		QEasingCurve.EasingFunction object = progress -> {
					calls.put(_i, progress);
					return 0;
	    		};
	    		container.add(object);
	    		General.internalAccess.setJavaOwnership(object);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	    	Tulip.testEasingFunctions(container);
	        Assert.assertEquals(100, calls.size());
	        for (int i = 0; i < calls.size(); i++) {
	        	Assert.assertEquals(i, calls.get(i).intValue());
			}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingQLinkedListQt5.class) {
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
            synchronized(TestReferenceCountingQLinkedListQt5.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(101, counter.get());
    }
    
    @Test
    public void test_cpp_QLinkedList_Tulip_TestStdFunction() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	TreeMap<Integer,Integer> calls = new TreeMap<>();
    	{
	    	QLinkedList<Tulip.TestStdFunction> container = Tulip.createLinkedListOfStdFunctions();
	    	Assert.assertTrue(container!=null);
	    	for(int i=0; i<100; ++i) {
	    		int _i = i;
	    		Tulip.TestStdFunction object = (int arg__1, boolean arg__2, double arg__3) -> {
					calls.put(_i, arg__1);
	    		};
	    		container.add(object);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	    	Tulip.testStdFunctions(container);
	        Assert.assertEquals(100, calls.size());
	        for (int i = 0; i < calls.size(); i++) {
	        	Assert.assertEquals(i+1, calls.get(i).intValue());
			}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingQLinkedListQt5.class) {
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
            synchronized(TestReferenceCountingQLinkedListQt5.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(101, counter.get());
    }
    
    @Test
    public void test_java_QLinkedList_Tulip_TestStdFunction() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	TreeMap<Integer,Integer> calls = new TreeMap<>();
    	{
	    	QLinkedList<Tulip.TestStdFunction> container = new QLinkedList<>(Tulip.TestStdFunction.class);
	    	Assert.assertTrue(container!=null);
	    	for(int i=0; i<100; ++i) {
	    		int _i = i;
	    		Tulip.TestStdFunction object = (int arg__1, boolean arg__2, double arg__3) -> {
					calls.put(_i, arg__1);
	    		};
	    		container.add(object);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	    	Tulip.testStdFunctions(container);
	        Assert.assertEquals(100, calls.size());
	        for (int i = 0; i < calls.size(); i++) {
	        	Assert.assertEquals(i, calls.get(i).intValue());
			}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingQLinkedListQt5.class) {
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
            synchronized(TestReferenceCountingQLinkedListQt5.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(101, counter.get());
    }
    
    @Test
    public void test_QLinkedList_assign() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	QLinkedList<QWidgetItem> container = new QLinkedList<>(QWidgetItem.class);
    	for (int i = 0; i < COUNT; i++) {
    		QWidgetItem item = new QWidgetItem(null);
    		QtUtilities.getSignalOnDispose(item).connect(counter::incrementAndGet);
    		container.add(item);
		}
    	for (int i = 0; i < 20 && counter.get()==0; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingQLinkedListQt5.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(0, counter.get());
    	{
    		QWidgetItem item = new QWidgetItem(null);
    		QtUtilities.getSignalOnDispose(item).connect(counter::incrementAndGet);
    		container.assign(QLinkedList.of(item));
    	}
    	for (int i = 0; i < 50 && counter.get()<COUNT; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingQLinkedListQt5.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT, counter.get());
        container.swap(new QLinkedList<>(QWidgetItem.class));
    	for (int i = 0; i < 50 && counter.get()<COUNT+1; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingQLinkedListQt5.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT+1, counter.get());
    }
    
    @Test
    public void test_owner_QItemSelection() throws InterruptedException {
    	QStandardItemModel model = new QStandardItemModel();
    	model.insertColumns(0, 10);
    	model.insertRows(0, 10);
		QItemSelection selection = new QItemSelection();
		selection.select(model.index(2, 2), model.index(4, 4));
		selection.select(model.index(1, 2), model.index(1, 4));
		Assert.assertEquals(2, selection.size());
		Assert.assertEquals(model, General.internalAccess.owner(selection));
		selection = null;
    }
    
    @Test
    public void test_owner_QLinkedList_TextCursor() throws InterruptedException {
    	QTextDocument owner = new QTextDocument();
		QLinkedList<QTextCursor> list = QLinkedList.of(new QTextCursor(owner));
		Assert.assertEquals(1, list.size());
		Assert.assertEquals(owner, General.internalAccess.owner(list));
		list = null;
    }
    
    @Test
    public void test_QLinkedList_QObject_as_QVariant() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
        	QObject variantHolder = new QObject();
	    	QLinkedList<QObject> container = new QLinkedList<>(QObject.class);
	    	for(int i=0; i<COUNT; ++i) {
	    		QObject object = new QObject();
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		container.add(object);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingQLinkedListQt5.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        variantHolder.setProperty("dynamicVariant", container);
	        Assert.assertTrue(Variants.compare(variantHolder, "dynamicVariant", container));
	        QtUtilities.getSignalOnDispose(container).connect(counter::incrementAndGet);
	        container = null;
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingQLinkedListQt5.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        QtUtilities.getSignalOnDispose(variantHolder).connect(counter::incrementAndGet);
	        variantHolder = null;
	    }
        for (int i = 0; i < 50 && counter.get()<COUNT+2; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingQLinkedListQt5.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT+2, counter.get());
    }
}
