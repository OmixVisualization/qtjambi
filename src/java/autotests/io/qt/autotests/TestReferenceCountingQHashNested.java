package io.qt.autotests;

import java.util.TreeMap;
import java.util.concurrent.atomic.AtomicInteger;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.autotests.generated.General;
import io.qt.autotests.generated.Tulip;
import io.qt.core.QCoreApplication;
import io.qt.core.QEasingCurve;
import io.qt.core.QEvent;
import io.qt.core.QHash;
import io.qt.core.QList;
import io.qt.core.QMetaType;
import io.qt.core.QSet;
import io.qt.gui.QTextCursor;
import io.qt.gui.QTextDocument;

public class TestReferenceCountingQHashNested extends ApplicationInitializer {
	
	public static final int COUNT = 10;
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
    
    @Test
    public void test_java_QHash_value_QList_QEasingCurve_EasingFunction() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	TreeMap<Integer,Double> calls = new TreeMap<>();
    	{
	    	QHash<String,QList<QEasingCurve.EasingFunction>> container = new QHash<>(String.class, QMetaType.fromType(QList.class, QMetaType.fromType(QEasingCurve.EasingFunction.class)));
	    	Assert.assertTrue(container!=null);
	    	for(int i=0; i<COUNT; ++i) {
	    		int _i = i;
	    		QEasingCurve.EasingFunction object = progress -> {
					calls.put(_i, progress);
					return 0;
	    		};
	    		QList<QEasingCurve.EasingFunction> list = QList.of(object);
//	    		General.internalAccess.setJavaOwnership(object);
	    		container.put(""+i, list);
	    		General.internalAccess.registerCleaner(object, counter::incrementAndGet);
	    		Assert.assertTrue(General.internalAccess.isJavaOwnership(list));
	    		list.dispose();
	    		object = null;
	    		list = null;
	    	}
			QList<QList<QEasingCurve.EasingFunction>> values = container.values();
	    	for(QList<QEasingCurve.EasingFunction> list : values) {
    			Tulip.testEasingFunctions(list);
    			list.dispose();
    			list = null;
	    	}
	    	values = null;
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
	        container.dispose();
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
    public void test_owner_QHash_value_QSet_QList_TextCursor() throws InterruptedException {
    	QTextDocument owner = new QTextDocument();
		QHash<String,QSet<QList<QTextCursor>>> container = QHash.of("A", new QSet<>(QMetaType.fromType(QList.class, QMetaType.fromType(QTextCursor.class))), 
															      "Z", QSet.of(QList.of(new QTextCursor(owner))));
		Assert.assertEquals(2, container.size());
		Assert.assertEquals(owner, General.internalAccess.owner(container));
		container = null;
    }
}
