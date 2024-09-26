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
import io.qt.core.QList;
import io.qt.core.QObject;
import io.qt.core.QRunnable;
import io.qt.gui.QStandardItemModel;
import io.qt.gui.QTextCursor;
import io.qt.gui.QTextDocument;
import io.qt.widgets.QGraphicsItem;
import io.qt.widgets.QGraphicsPathItem;
import io.qt.widgets.QWidgetItem;

public class TestReferenceCountingQList extends ApplicationInitializer {
	
	public static final int COUNT = 10;
	
	@Test
    public void test_new_QList_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QList<QObject> container = new QList<>(QObject.class);
	    	for(int i=0; i<COUNT; ++i) {
	    		QObject object = new QObject();
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
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT+1, counter.get());
    }
    
    @Test
    public void test_new_QList_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QList<QRunnable> container = new QList<>(QRunnable.class);
	    	for(int i=0; i<COUNT; ++i) {
	    		QRunnable object = new QRunnable() { @Override public void run() {} };
	    		container.add(object);
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
        for (int i = 0; i < 50 && counter.get()<COUNT+1; i++) {
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
    public void test_new_QList_QGraphicsItem() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QList<QGraphicsItem> container = new QList<>(QGraphicsItem.class);
	    	for(int i=0; i<COUNT; ++i) {
	    		QGraphicsItem object = new QGraphicsPathItem();
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
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT+1, counter.get());
    }
    
    @Test
    public void test_cpp_QList_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QList<QObject> container = Tulip.createListOfObjects();
	    	for(int i=0; i<100; ++i) {
	    		QObject object = new QObject();
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
    public void test_cpp_QList_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QList<QRunnable> container = Tulip.createListOfRunnables();
	    	for(int i=0; i<100; ++i) {
	    		QRunnable object = new QRunnable() { @Override public void run() {} };
	    		container.add(object);
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
    public void test_cpp_QList_QEasingCurve_EasingFunction() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	TreeMap<Integer,Double> calls = new TreeMap<>();
    	{
	    	QList<QEasingCurve.EasingFunction> container = Tulip.createListOfEasingFunctions();
	    	Assert.assertTrue(container!=null);
	    	for(int i=0; i<100; ++i) {
	    		int _i = i;
	    		QEasingCurve.EasingFunction object = progress -> {
					calls.put(_i, progress);
					return 0;
	    		};
	    		container.add(object);
	    		General.internalAccess.setJavaOwnership(object);
	    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
	    		object = null;
	    	}
	    	Tulip.testEasingFunctions(container);
	        Assert.assertEquals(100, calls.size());
	        for (int i = 0; i < calls.size(); i++) {
	        	Assert.assertEquals(i+1, calls.get(i).intValue());
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
    public void test_java_QList_QEasingCurve_EasingFunction() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	TreeMap<Integer,Double> calls = new TreeMap<>();
    	{
	    	QList<QEasingCurve.EasingFunction> container = new QList<>(QEasingCurve.EasingFunction.class);
	    	Assert.assertTrue(container!=null);
	    	for(int i=0; i<100; ++i) {
	    		int _i = i;
	    		QEasingCurve.EasingFunction object = progress -> {
					calls.put(_i, progress);
					return 0;
	    		};
	    		container.add(object);
	    		General.internalAccess.setJavaOwnership(object);
	    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
	    		object = null;
	    	}
	    	Tulip.testEasingFunctions(container);
	        Assert.assertEquals(100, calls.size());
	        for (int i = 0; i < calls.size(); i++) {
	        	Assert.assertEquals(i, calls.get(i).intValue());
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
    public void test_cpp_QList_Tulip_TestStdFunction() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	TreeMap<Integer,Integer> calls = new TreeMap<>();
    	{
	    	QList<Tulip.TestStdFunction> container = Tulip.createListOfStdFunctions();
	    	Assert.assertTrue(container!=null);
	    	for(int i=0; i<100; ++i) {
	    		int _i = i;
	    		Tulip.TestStdFunction object = (int arg__1, boolean arg__2, double arg__3) -> {
					calls.put(_i, arg__1);
	    		};
	    		container.add(object);
	    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
	    		object = null;
	    	}
	    	Tulip.testStdFunctions(container);
	        Assert.assertEquals(100, calls.size());
	        for (int i = 0; i < calls.size(); i++) {
	        	Assert.assertEquals(i+1, calls.get(i).intValue());
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
    public void test_java_QList_Tulip_TestStdFunction() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	TreeMap<Integer,Integer> calls = new TreeMap<>();
    	{
	    	QList<Tulip.TestStdFunction> container = new QList<>(Tulip.TestStdFunction.class);
	    	Assert.assertTrue(container!=null);
	    	for(int i=0; i<100; ++i) {
	    		int _i = i;
	    		Tulip.TestStdFunction object = (int arg__1, boolean arg__2, double arg__3) -> {
					calls.put(_i, arg__1);
	    		};
	    		container.add(object);
	    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
	    		object = null;
	    	}
	    	Tulip.testStdFunctions(container);
	        Assert.assertEquals(100, calls.size());
	        for (int i = 0; i < calls.size(); i++) {
	        	Assert.assertEquals(i, calls.get(i).intValue());
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
    public void test_owner_QList_TextCursor() throws InterruptedException {
    	QTextDocument owner = new QTextDocument();
		QList<QTextCursor> list = QList.of(new QTextCursor(owner));
		Assert.assertEquals(1, list.size());
		Assert.assertEquals(owner, General.internalAccess.owner(list));
		list = null;
    }
    
    @Test
    public void test_QList_assign() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	QList<QWidgetItem> container = new QList<>(QWidgetItem.class);
    	for (int i = 0; i < COUNT; i++) {
    		QWidgetItem item = new QWidgetItem(null);
    		QtUtilities.getSignalOnDispose(item).connect(counter::incrementAndGet);
    		container.add(item);
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
    		container.assign(QList.of(item));
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
        container.swap(new QList<>(QWidgetItem.class));
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
    public void testQList_mid() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	QList<QWidgetItem> list = new QList<>(QWidgetItem.class);
    	for (int i = 0; i < COUNT; i++) {
    		QWidgetItem item = new QWidgetItem(null);
    		QtUtilities.getSignalOnDispose(item).connect(counter::incrementAndGet);
    		list.add(item);
		}
		QList<QWidgetItem> mid = list.mid(2, 1);
		Assert.assertEquals(1, mid.size());
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
        for (int i = 0; i < 50 && counter.get()<2; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT-1, counter.get());
        @SuppressWarnings("unused")
		QWidgetItem first = mid.takeFirst();
        first = null;
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
    public void test_QList_QObject_as_QVariant() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
        	QObject variantHolder = new QObject();
	    	QList<QObject> container = new QList<>(QObject.class);
	    	for(int i=0; i<COUNT; ++i) {
	    		QObject object = new QObject();
	    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
	    		container.add(object);
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
	        variantHolder.setProperty("dynamicVariant", container);
	        Assert.assertTrue(Variants.compare(variantHolder, "dynamicVariant", container));
	        General.internalAccess.registerCleaner(container, counter::incrementAndGet);
	        container = null;
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
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
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT+2, counter.get());
    }
}
