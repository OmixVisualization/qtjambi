package io.qt.autotests;

import static io.qt.autotests.generated.ContainerFactory.*;
import java.util.TreeMap;
import java.util.concurrent.atomic.AtomicInteger;

import org.junit.*;

import io.qt.autotests.generated.*;
import io.qt.core.*;
import io.qt.gui.*;

public class TestReferenceCountingQMapNested extends ApplicationInitializer {
	
	public static final int COUNT = 10;
    
    @Test
    public void test_java_QMap_value_QList_QEasingCurve_EasingFunction() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	TreeMap<Integer,Double> calls = new TreeMap<>();
    	{
	    	QMap<String,QList<QEasingCurve.EasingFunction>> container = new QMap<>(String.class, QMetaType.fromType(QList.class, QMetaType.fromType(QEasingCurve.EasingFunction.class)));
	    	Assert.assertTrue(container!=null);
	    	for(int i=0; i<COUNT; ++i) {
	    		int _i = i;
	    		QEasingCurve.EasingFunction object = progress -> {
					calls.put(_i, progress);
					return 0;
	    		};
	    		container.put(""+i, QList.of(object));
//	    		General.internalAccess.setJavaOwnership(object);
	    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
	    		object = null;
	    	}
	    	for(QList<QEasingCurve.EasingFunction> list : container.values()) {
		    	testEasingFunctions(list);
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
    public void test_owner_QMap_value_QSet_QList_TextCursor() throws InterruptedException {
    	QTextDocument owner = new QTextDocument();
		QMap<String,QSet<QList<QTextCursor>>> container = QMap.of("A", new QSet<>(QMetaType.fromType(QList.class, QMetaType.fromType(QTextCursor.class))), 
															      "Z", QSet.of(QList.of(new QTextCursor(owner))));
		Assert.assertEquals(2, container.size());
		Assert.assertEquals(owner, General.internalAccess.owner(container));
		container = null;
    }
}
