package io.qt.autotests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.util.concurrent.atomic.AtomicInteger;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtUtilities;
import io.qt.autotests.generated.General;
import io.qt.core.*;
import io.qt.gui.*;
import io.qt.widgets.*;

public class TestReferenceCountingQModelView extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
        ApplicationInitializer.testInitializeWithWidgets();
		Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
    }
	
	public static final int COUNT = 10;
	
	class ProxyModel extends QAbstractProxyModel {

        @Override
        public QModelIndex mapFromSource(QModelIndex sourceIndex) {

            return null;
        }

        @Override
        public QModelIndex mapToSource(QModelIndex proxyIndex) {

            return null;
        }

        @Override
        public int columnCount(QModelIndex parent) {

            return 0;
        }

        @Override
        public QModelIndex index(int row, int column, QModelIndex parent) {

            return null;
        }

        @Override
        public QModelIndex parent(QModelIndex child) {

            return null;
        }

        @Override
        public int rowCount(QModelIndex parent) {

            return 0;
        }

    };
    
    class SortFilterProxyModel extends QSortFilterProxyModel {

        @Override
        public QModelIndex mapFromSource(QModelIndex sourceIndex) {
            return null;
        }

        @Override
        public QModelIndex mapToSource(QModelIndex proxyIndex) {

            return null;
        }

        @Override
        public int columnCount(QModelIndex parent) {

            return 0;
        }

        @Override
        public QModelIndex index(int row, int column, QModelIndex parent) {

            return null;
        }

        @Override
        public QModelIndex parent(QModelIndex child) {

            return null;
        }

        @Override
        public int rowCount(QModelIndex parent) {

            return 0;
        }

    };
	
	class ItemView extends QAbstractItemView {
		
        @Override
        protected int horizontalOffset() {
            return 0;
        }

        @Override
        public QModelIndex indexAt(QPoint point) {
            return null;
        }

        @Override
        protected boolean isIndexHidden(QModelIndex index) {
            return false;
        }

        @Override
        protected QModelIndex moveCursor(CursorAction cursorAction, Qt.KeyboardModifiers modifiers) {
            return null;
        }

        @Override
        public void scrollTo(QModelIndex index, ScrollHint hint) {
        }

        @Override
        protected void setSelection(QRect rect, QItemSelectionModel.SelectionFlags command) {
        }

        @Override
        protected int verticalOffset() {
            return 0;
        }

        @Override
        public QRect visualRect(QModelIndex index) {
            return null;
        }

        @Override
        protected QRegion visualRegionForSelection(QItemSelection selection) {
            return null;
        }

    }
	
	@Test 
	public final void testQAbstractItemViewSetSelectionModelThenSetModel() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QAbstractItemView view = new ItemView();
	        {
	            QItemSelectionModel model = new QItemSelectionModel(view.model());
	            General.internalAccess.registerCleaner(model, counter::incrementAndGet);
	            view.setSelectionModel(model);
	            view.setModel(new QStandardItemModel());

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
	        Assert.assertEquals(1, counter.get());
	        assertTrue(view.selectionModel() != null);
		}
	}
	
	@Test 
	public final void testQAbstractItemViewSetSelectionModel() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QAbstractItemView view = new ItemView();
	        {
	            view.setModel(new QFileSystemModel());

	            QItemSelectionModel model = new QItemSelectionModel(view.model());
	            General.internalAccess.registerCleaner(model, counter::incrementAndGet);

	            view.setSelectionModel(model);

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
	        assertTrue(view.selectionModel() != null);
	        view = null;
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
	public final void testQAbstractItemViewSetItemDelegate() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QAbstractItemView view = new ItemView();
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
	            view.setItemDelegate(delegate);
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
	        assertTrue(view.itemDelegate() != null);
	        view = null;
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
	public final void testQAbstractItemViewSetItemDelegateNull() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QAbstractItemView view = new ItemView();
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
	            view.setItemDelegate(delegate);
	            view.setItemDelegate(null);
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
	        assertTrue(view.itemDelegate() == null);
	        view = null;
		}
	}
	
	@Test 
	public final void testQAbstractItemViewSetModel() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QAbstractItemView view = new ItemView();
	        {
	        	QStandardItemModel model = new QStandardItemModel();
	            General.internalAccess.registerCleaner(model, counter::incrementAndGet);

	            view.setModel(model);

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
	        assertTrue(view.model() != null);
	        view = null;
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
	public final void testQAbstractProxyModelSetSourceModel() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QAbstractProxyModel proxy = new ProxyModel();
	        {
	        	QStandardItemModel model = new QStandardItemModel();
	        	model.setObjectName("source model");
	            General.internalAccess.registerCleaner(model, counter::incrementAndGet);
	            proxy.setSourceModel(model);
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
	        assertTrue(proxy.sourceModel() != null);
	        proxy = null;
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
	public final void testQAbstractProxyModelSetSourceModelNull() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QAbstractProxyModel proxy = new ProxyModel();
	        {
	        	QStandardItemModel model = new QStandardItemModel();
	        	model.setObjectName("source model");
	            General.internalAccess.registerCleaner(model, counter::incrementAndGet);
	            proxy.setSourceModel(model);
	            proxy.setSourceModel(null);
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
	        Assert.assertEquals(1, counter.get());
	        assertTrue(proxy.sourceModel() == null);
	        proxy = null;
		}
	}
	
	@Test 
	public final void testQSortFilterProxyModelSetSourceModel() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QAbstractProxyModel proxy = new SortFilterProxyModel();
	        {
	        	QStandardItemModel model = new QStandardItemModel();
	        	model.setObjectName("source model");
	            General.internalAccess.registerCleaner(model, counter::incrementAndGet);
	            proxy.setSourceModel(model);
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
	        assertTrue(proxy.sourceModel() != null);
	        proxy = null;
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
	public final void testQSortFilterProxyModelSetSourceModelNull() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QAbstractProxyModel proxy = new SortFilterProxyModel();
	        {
	        	QStandardItemModel model = new QStandardItemModel();
	        	model.setObjectName("source model");
	            General.internalAccess.registerCleaner(model, counter::incrementAndGet);
	            proxy.setSourceModel(model);
	            proxy.setSourceModel(null);
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
	        Assert.assertEquals(1, counter.get());
	        assertTrue(proxy.sourceModel() == null);
	        proxy = null;
		}
	}

    private static int deletedEngines = 0;
    private static class MyIconEngine extends QIconEngine {
        private int i;
        public MyIconEngine(int i) {
            this.i = i;
            QtUtilities.getSignalOnDispose(this).connect(()->{
            	deletedEngines++;
            });
        }

        @Override
        public void paint(QPainter painter, QRect rect, io.qt.gui.QIcon.Mode mode, io.qt.gui.QIcon.State state) {

        }

        @Override
        public QIconEngine clone() {
            return new MyIconEngine(i);
        }
    }

    @Test public final void testIconEngine() {
        QListWidget w = new QListWidget();

        deletedEngines = 0;

        for (int i=0; i<100; ++i) {
            w.addItem(new QListWidgetItem(new QIcon(new MyIconEngine(i)), "" + i));
            ApplicationInitializer.runGC();
        }

        // Don't crash
        w.show();
        assertEquals(0, deletedEngines);
    }

    @Test
    public final void setItemDelegateForRowToNull() {
        QAbstractItemView view = new QTableView();

        boolean caughtException = false;
        try {
            view.setItemDelegateForRow(0, null);
        } catch (NullPointerException e) {
            caughtException = true;
        }

        assertFalse(caughtException);
    }

    @Test
    public final void setItemDelegateForColumnToNull() {
        QAbstractItemView view = new QTableView();

        boolean caughtException = false;
        try {
            view.setItemDelegateForColumn(0, null);
        } catch (NullPointerException e) {
            caughtException = true;
        }

        assertFalse(caughtException);
    }
}
