package io.qt.autotests;

import java.util.concurrent.atomic.AtomicInteger;

import org.junit.Assert;
import org.junit.Test;

import io.qt.autotests.generated.General;
import io.qt.autotests.generated.Variants;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QObject;
import io.qt.core.QPair;

public class TestReferenceCountingQPair extends ApplicationInitializer {
	
	public static final int COUNT = 10;
    
    @Test
    public void test_QPair_of_pointers_as_QVariant() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
        	QObject variantHolder = new QObject();
        	QPair<QObject,QObject> container = new QPair<>(new QObject(),new QObject());
        	General.internalAccess.registerCleaner(container.first, counter::incrementAndGet);
        	General.internalAccess.registerCleaner(container.second, counter::incrementAndGet);
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
	        General.internalAccess.registerCleaner(variantHolder, counter::incrementAndGet);
	        variantHolder = null;
	    }
        for (int i = 0; i < 50 && counter.get()<2; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(3, counter.get());
    }
}
