package io.qt.autotests;

import static org.junit.Assert.assertTrue;

import java.util.concurrent.atomic.AtomicInteger;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.autotests.generated.General;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.gui.QGuiApplication;
import io.qt.widgets.QCompleter;
import io.qt.widgets.QLineEdit;

public class TestReferenceCountingQLineEdit extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
        ApplicationInitializer.testInitializeWithWidgets();
		Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
    }
	
	public static final int COUNT = 10;
	
	@Test 
	public final void testQLineEditSetCompleter() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
	        QLineEdit lineEdit = new QLineEdit();
	
	        {
	            QCompleter completer = new QCompleter();
	            General.internalAccess.registerCleaner(completer, counter::incrementAndGet);
	            lineEdit.setCompleter(completer);
	            completer = null;
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
	        assertTrue(lineEdit.completer() != null);
	        lineEdit = null;
		}
		for (int i = 0; i < 50 && counter.get()==0; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(1, counter.get());
    }
	
	@Test
	public final void testQLineEditSetCompleterNull() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
	        QLineEdit lineEdit = new QLineEdit();
	        {
	        	QCompleter completer = new QCompleter();
	        	General.internalAccess.registerCleaner(completer, counter::incrementAndGet);
	        	lineEdit.setCompleter(completer);
	        	lineEdit.setCompleter(null);
	            completer = null;
	        }
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(1, counter.get());
	        assertTrue(lineEdit.completer() == null);
		}
	}
}
