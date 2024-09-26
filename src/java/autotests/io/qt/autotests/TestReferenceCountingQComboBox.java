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
import io.qt.core.QModelIndex;
import io.qt.core.QObject;
import io.qt.core.QSize;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QPainter;
import io.qt.gui.QStandardItemModel;
import io.qt.gui.QValidator;
import io.qt.widgets.QAbstractItemDelegate;
import io.qt.widgets.QComboBox;
import io.qt.widgets.QCompleter;
import io.qt.widgets.QLineEdit;
import io.qt.widgets.QStyleOptionViewItem;

public class TestReferenceCountingQComboBox extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
        ApplicationInitializer.testInitializeWithWidgets();
		Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
    }
	
	public static final int COUNT = 10;
	
	@Test 
	public final void testQComboBoxEditableSetCompleter() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
	        QComboBox box = new QComboBox();
	        box.setEditable(true);
	        {
	        	QCompleter completer = new QCompleter();
	        	General.internalAccess.registerCleaner(completer, counter::incrementAndGet);
	        	box.setCompleter(completer);
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
	        assertTrue(box.completer() != null);
	        box = null;
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
	public final void testQComboBoxSetCompleterNull() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
	        final QComboBox box = new QComboBox();
	        QLineEdit lineEdit = new QLineEdit();
	        box.setLineEdit(lineEdit);
	        {
	        	QCompleter completer = new QCompleter();
	        	General.internalAccess.registerCleaner(completer, counter::incrementAndGet);
	        	box.setCompleter(completer);
	        	box.setCompleter(null);
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
	        assertTrue(box.completer() == null);
		}
	}
	
	@Test
	public final void testQComboBoxSetCompleterNoLineEdit() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
	        final QComboBox box = new QComboBox();
	        {
	            QCompleter completer = new QCompleter();
	            General.internalAccess.registerCleaner(completer, counter::incrementAndGet);
	            box.setCompleter(completer);
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
	        assertTrue(box.completer() == null);
		}
	}
	
	@Test 
	public final void testQComboBoxSetCompleter() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
	        QComboBox box = new QComboBox();
	        QLineEdit lineEdit = new QLineEdit();
	        box.setLineEdit(lineEdit);
	
	        {
	            QCompleter completer = new QCompleter();
	            General.internalAccess.registerCleaner(completer, counter::incrementAndGet);
	            box.setCompleter(completer);
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
	        assertTrue(box.completer() != null);
	        box = null;
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
	public final void testQComboBoxLineEditSetCompleter() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
	        QComboBox box = new QComboBox();
	        QLineEdit lineEdit = new QLineEdit();
	        box.setLineEdit(lineEdit);
	
	        {
	            QCompleter completer = new QCompleter();
	            General.internalAccess.registerCleaner(completer, counter::incrementAndGet);
	            box.lineEdit().setCompleter(completer);
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
	        box = null;
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
	public final void testQComboBoxLineEditSetCompleterNull() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
	        final QComboBox box = new QComboBox();
	        QLineEdit lineEdit = new QLineEdit();
	        box.setLineEdit(lineEdit);
	        {
	        	QCompleter completer = new QCompleter();
	        	General.internalAccess.registerCleaner(completer, counter::incrementAndGet);
	        	box.lineEdit().setCompleter(completer);
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
	        assertTrue(box.completer() == null);
		}
	}
	
	@Test 
	public final void testQComboBoxSetValidator() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QComboBox box = new QComboBox();
	        box.setEditable(true);
	        
	        {
	            QValidator validator = new QValidator((QObject)null) {
	                @Override
	                public State validate(QValidationData arg__1) {
	                    return null;
	                }
	            };
	            General.internalAccess.registerCleaner(validator, counter::incrementAndGet);
	            box.setValidator(validator);
	            validator = null;
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
	        assertTrue(box.validator() != null);
	        box = null;
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
	public final void testQComboBoxLineEditSetValidatorNull() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QComboBox box = new QComboBox();
	        box.setEditable(true);
	        
	        {
	            QValidator validator = new QValidator((QObject)null) {
	                @Override
	                public State validate(QValidationData arg__1) {
	                    return null;
	                }
	            };
	            General.internalAccess.registerCleaner(validator, counter::incrementAndGet);
	            box.setValidator(validator);
	            box.lineEdit().setValidator(null);
	            validator = null;
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
	        assertTrue(box.validator() == null);
	        box = null;
		}
	}
	
	@Test 
	public final void testQComboBoxSetValidatorNull() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QComboBox box = new QComboBox();
	        box.setEditable(true);
	        
	        {
	            QValidator validator = new QValidator((QObject)null) {
	                @Override
	                public State validate(QValidationData arg__1) {
	                    return null;
	                }
	            };
	            General.internalAccess.registerCleaner(validator, counter::incrementAndGet);
	            box.setValidator(validator);
	            box.setValidator(null);
	            validator = null;
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
	        assertTrue(box.validator() == null);
	        box = null;
		}
	}
	
	@Test 
	public final void testQComboBoxSetModel() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QComboBox box = new QComboBox();
			{
				QStandardItemModel model = new QStandardItemModel();
				General.internalAccess.registerCleaner(model, counter::incrementAndGet);
	            box.setModel(model);
	            model = null;
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
	        assertTrue(box.model() != null);
	        box = null;
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
	public final void testQComboBoxSetItemDelegate() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
	        QComboBox box = new QComboBox();
	        QLineEdit lineEdit = new QLineEdit();
	        box.setLineEdit(lineEdit);
	
	        {
	        	QAbstractItemDelegate delegate = new QAbstractItemDelegate() {

	                @Override
	                public void paint(QPainter painter, QStyleOptionViewItem option, QModelIndex index) {
	                }

	                @Override
	                public QSize sizeHint(QStyleOptionViewItem option, QModelIndex index) {
	                    return null;
	                }

	            };
	            General.internalAccess.registerCleaner(delegate, counter::incrementAndGet);
	            box.setItemDelegate(delegate);
	            delegate = null;
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
	        assertTrue(box.itemDelegate() != null);
	        box = null;
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
	public final void testQComboBoxSetItemDelegateNull() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
	        final QComboBox box = new QComboBox();
	        QLineEdit lineEdit = new QLineEdit();
	        box.setLineEdit(lineEdit);
	        {
	        	QAbstractItemDelegate delegate = new QAbstractItemDelegate() {

	                @Override
	                public void paint(QPainter painter, QStyleOptionViewItem option, QModelIndex index) {
	                }

	                @Override
	                public QSize sizeHint(QStyleOptionViewItem option, QModelIndex index) {
	                    return null;
	                }

	            };
	        	General.internalAccess.registerCleaner(delegate, counter::incrementAndGet);
	        	box.setItemDelegate(delegate);
	        	QAbstractItemDelegate delegate2 = new QAbstractItemDelegate() {

	                @Override
	                public void paint(QPainter painter, QStyleOptionViewItem option, QModelIndex index) {
	                }

	                @Override
	                public QSize sizeHint(QStyleOptionViewItem option, QModelIndex index) {
	                    return null;
	                }
	            };
	        	box.setItemDelegate(delegate2);
	        	delegate = null;
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
	        assertTrue(box.itemDelegate() != null);
		}
	}
}
