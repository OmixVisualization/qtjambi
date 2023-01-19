/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
package io.qt.autotests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.lang.ref.*;
import java.util.*;

import org.junit.*;

import io.qt.*;
import io.qt.core.*;
import io.qt.gui.*;
import io.qt.widgets.*;

public class TestReferenceCounting extends ApplicationInitializer {
    private static ReferenceQueue<QObject> weakReferenceQueue = new ReferenceQueue<QObject>();
    private static ReferenceQueue<QObject> phantomReferenceQueue = new ReferenceQueue<QObject>();
    private static Map<WeakReference<QObject>,Integer> weakReferenceMap = new HashMap<WeakReference<QObject>,Integer>();
    private static Map<PhantomReference<QObject>,Integer> phantomReferenceMap = new HashMap<PhantomReference<QObject>,Integer>();
    private static List<Integer> alive = new ArrayList<Integer>();
    private static List<Integer> aliveAndUnderTest = new ArrayList<Integer>();
    private static int counter = 0;

    public class AssertEquals {
    	protected String description() {
    		return ""+this.getClass().getName();
    	}
        protected boolean equals() {
            return false;
        }

        public void test() {
            test(10000);
        }
        public void test(int timeout) {
            Long couldStopAt = null;
            long stop = System.currentTimeMillis() + timeout;
            Vector<Long> garbage = new Vector<Long>();
            while(stop > System.currentTimeMillis() && !equals()) {

                garbage.add(System.currentTimeMillis());

                ApplicationInitializer.runGC();
                if(couldStopAt == null && equals()) {
                    couldStopAt = Long.valueOf(System.currentTimeMillis());
                //    break;
                }
                try {
                    Thread.sleep(1);
                } catch (InterruptedException e) {
                }
            }
            if(stop > System.currentTimeMillis())
                java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "AssertEquals.test(timeout=" + timeout + ")  couldStopAt="+couldStopAt+"; diff="+(stop-(couldStopAt!=null?couldStopAt.longValue():0)));
            assertTrue(description(), equals());
        }
    }

    static final int COUNT = 200;
    @SuppressWarnings("unused")
    private long startTimeMillis;

    private void reset() {
        synchronized(TestReferenceCounting.class) {
            deleted = 0;
        }
    }

    private int getDeletedCount() {
        synchronized(TestReferenceCounting.class) {
            return deleted;
        }
    }

    @BeforeClass
    public static void testInitialize() throws Exception {
        ApplicationInitializer.testInitializeWithWidgets();
		Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
    }

    @Before
    public void setUp() {
        QApplication.processEvents();
        clearGcReferences();
        startTimeMillis = System.currentTimeMillis();
    }

    @After
    public void tearDown() {
        ApplicationInitializer.runGC();
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
        QApplication.processEvents();
        ApplicationInitializer.runGC();
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
        QApplication.processEvents();
    }

    public static Integer getIdNext() {
        int idNext;
        synchronized(TestReferenceCounting.class) {
            counter++;
            idNext = counter;
        }
        return Integer.valueOf(idNext);
    }
    public void clearGcReferences() {
        synchronized(TestReferenceCounting.class) {
            aliveAndUnderTest.clear();
        }
    }
    public int getSize() {
        synchronized(TestReferenceCounting.class) {
            return aliveAndUnderTest.size();
        }
    }

    // This does not have "id" field, so make one up for it
    @SuppressWarnings("unused")
	private static void accountingForNewObject(QObject o, Integer id) {
        assertNotNull(o);

        if(id == null)
            id = getIdNext();

        {
        	Integer _id = id;
            java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, 
            		() -> { 
            			String className = o.getClass().getName();
                        String shortClassName = className;
                        int i = shortClassName.lastIndexOf('.');
                        if(i > 0)
                            shortClassName = shortClassName.substring(i + 1);
            			return shortClassName + ".ctor " + className + "@" + System.identityHashCode(o) + "; thread=" + Thread.currentThread().getId() + "; id=" + _id;
            		});
        }
        WeakReference<QObject> wr = new WeakReference<QObject>(o, weakReferenceQueue);
        PhantomReference<QObject> pr = new PhantomReference<QObject>(o, phantomReferenceQueue);
        synchronized(TestReferenceCounting.class) {
            weakReferenceMap.put(wr, id);
            phantomReferenceMap.put(pr, id);
            aliveAndUnderTest.add(id);
            alive.add(id);
        }
    }

    @SuppressWarnings("unused")
    private boolean gcAndWait(long timeLimitMillis) {
        return gcAndWait(timeLimitMillis, null, null, null);
    }

    private boolean gcAndWait(long timeLimitMillis, Long excessLimitMillis, Integer disposedCount, Integer destroyedCount) {
        boolean obtainGoal = false;	// did we meet conditions within timelimit?

        long startTime = System.currentTimeMillis();
        long excessTime = 0;
        long elapsed = 0;

        int currentDisposedCount;
        int currentDestroyedCount;
        boolean loop = true;
        String debugPrefix = " gcAndWait(" + timeLimitMillis +
            ", " + excessLimitMillis +
            ", " + disposedCount +
            ", " + destroyedCount + ")";
        while(loop) {
            try {

                ApplicationInitializer.runGC();

                Reference<? extends QObject> thisWr;
                while((thisWr = weakReferenceQueue.poll()) != null) {
                    Integer tmpId;
                    synchronized(TestReferenceCounting.class) {
                        tmpId = weakReferenceMap.remove(thisWr);
                    }
                    java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINER, " weakReferenceQueue.remove(): dequeued id=" + tmpId);
                }
                Reference<? extends QObject> thisPr;
                while((thisPr = phantomReferenceQueue.poll()) != null) {
                    Integer tmpId;
                    synchronized(TestReferenceCounting.class) {
                        tmpId = phantomReferenceMap.remove(thisPr);
                    }
                    java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINER, " phantomReferenceQueue.remove(): dequeued id=" + tmpId);
                }

                if(!obtainGoal) {	// Try to obtainGoal
                    currentDisposedCount = getDeletedCount();
                    currentDestroyedCount = 0; // MyOrdinaryDestroyed.destroyedCount();

                    boolean loopDisposedCount = false;
                    if(disposedCount != null) {
                        if(disposedCount.intValue() > currentDisposedCount)
                            loopDisposedCount = true;
                    }
                    boolean loopDestroyedCount = false;
                    if(destroyedCount != null) {
                        if(destroyedCount.intValue() > currentDestroyedCount)
                            loopDestroyedCount = true;
                    }

                    if(!loopDisposedCount && !loopDestroyedCount) {
                        obtainGoal = true;	// we have met the callers goals
                        excessTime = System.currentTimeMillis();	// reset this for excessLimitMillis
                    }

                    java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINER, debugPrefix + ": elapsed=" + elapsed + "; end loop" +
                        "; currentDisposedCount=" + currentDisposedCount +
                        "; currentDestroyedCount=" + currentDestroyedCount +
                        "; obtainGoal=" + obtainGoal
                        );
                }

                if(obtainGoal) {
                    elapsed = System.currentTimeMillis() - excessTime;
                    if(excessLimitMillis != null)
                        loop = elapsed < excessLimitMillis.intValue();
                    else
                        loop = false;
                } else {
                    elapsed = System.currentTimeMillis() - startTime;
                    loop = elapsed < timeLimitMillis;
                }

                if(loop) {
                    Thread.sleep(10);
                    QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
                }
            } catch(Exception e) {	// exceptions are an idiotic concept; yeah in c++
                e.printStackTrace();
            }
        }

        // Report on status
        long _elapsed = elapsed;
        java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, ()->"elapsed=" + _elapsed + "; alive=" + alive + "; aliveAndUnderTest=" + aliveAndUnderTest +
                "; weakReferenceMapSize=" + weakReferenceMap.values() + "; phantomReferenceMapSize=" + phantomReferenceMap.values());
        return obtainGoal;
    }


    private int deleted = 0;
    
    private void increaseDeleted() {
	    synchronized(TestReferenceCounting.class) {
	        deleted++;
	    }
    }
    
    @Test public final void testQAbstractProxyModelSetSourceModel() {
        reset();

        QAbstractProxyModel proxy = new QAbstractProxyModel() {

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

        {
            QStandardItemModel model = new QStandardItemModel();

            model.setObjectName("source model");
            model.destroyed.connect(this::increaseDeleted);

            proxy.setSourceModel(model);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000) {
        	ApplicationInitializer.runGC();
        	QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
        }

        assertEquals(0, getDeletedCount());
        assertEquals("source model", proxy.sourceModel().objectName());
    }

    @Test public final void testQAbstractProxyModelSetSourceModelNull() {
        reset();

        final QAbstractProxyModel proxy = new QAbstractProxyModel() {

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

        {
            QStandardItemModel model = new QStandardItemModel();


            model.setObjectName("source model");
            model.destroyed.connect(this::increaseDeleted);

            proxy.setSourceModel(model);
            proxy.setSourceModel(null);

            model = null;
        }

        gcAndWait(10000, null, 1, null);
        new AssertEquals() {
            @Override
            protected String description() {
            	return "deleted != 1";
            }
            @Override
            protected boolean equals() {
                return 1 == getDeletedCount();
            }
        }.test();

        new AssertEquals() {
            @Override
            protected String description() {
            	return "proxy.sourceModel() != null";
            }
            @Override
            protected boolean equals() {
                return null == proxy.sourceModel();
            }
        }.test();
    }

    @Test public final void testQSortFilterProxyModelSetSourceModel() {
        reset();

        QSortFilterProxyModel proxy = new QSortFilterProxyModel() {

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

        {
            QStandardItemModel model = new QStandardItemModel();
            model.destroyed.connect(this::increaseDeleted);

            model.setObjectName("source model");

            proxy.setSourceModel(model);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000) {
        	ApplicationInitializer.runGC();
        	QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
        }

        assertEquals(0, getDeletedCount());
        assertEquals("source model", proxy.sourceModel().objectName());
    }

    @Test public final void testQSortFilterProxyModelSetSourceModelNull() {
        reset();

        final QSortFilterProxyModel proxy = new QSortFilterProxyModel() {

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

        {
            QStandardItemModel model = new QStandardItemModel();
            model.destroyed.connect(this::increaseDeleted);
            model.setObjectName("source model");

            proxy.setSourceModel(model);
            proxy.setSourceModel(null);

            model = null;
        }

        gcAndWait(10000, null, 1, null);
        new AssertEquals() {
            @Override
            protected String description() {
            	return "deleted != 1";
            }
            @Override
            protected boolean equals() {
                return 1 == getDeletedCount();
            }
        }.test();

        new AssertEquals() {
            @Override
            protected String description() {
            	return "proxy.sourceModel() != null";
            }
            @Override
            protected boolean equals() {
                return null == proxy.sourceModel();
            }
        }.test();
    }

    @Test public final void testQComboBoxSetCompleter() {
        QComboBox box = new QComboBox();
        QLineEdit lineEdit = new QLineEdit();
        box.setLineEdit(lineEdit);

        reset();
        {
            QCompleter completer = new QCompleter();
            completer.destroyed.connect(this::increaseDeleted);
            box.setCompleter(completer);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000) {
        	ApplicationInitializer.runGC();
        	QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
        }

        assertTrue(box.completer() != null);
        assertEquals(0, getDeletedCount());
    }

    @Test public final void testQComboBoxEditableSetCompleter() {
        QComboBox box = new QComboBox();
        box.setEditable(true);

        reset();
        {
            QCompleter completer = new QCompleter();
            completer.destroyed.connect(this::increaseDeleted);
            box.setCompleter(completer);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000) {
        	ApplicationInitializer.runGC();
        	QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
        }

        assertTrue(box.completer() != null);
        assertEquals(0, getDeletedCount());
    }


    @Test public final void testQComboBoxSetCompleterNull() {
        final QComboBox box = new QComboBox();
        QLineEdit lineEdit = new QLineEdit();
        box.setLineEdit(lineEdit);

        reset();
        {
            QCompleter completer = new QCompleter();
            completer.destroyed.connect(this::increaseDeleted);
            box.setCompleter(completer);
            box.setCompleter(null);
            completer = null;
        }

        new AssertEquals() {
            @Override
            protected String description() {
            	return "box.completer() != null";
            }
            @Override
            protected boolean equals() {
                return box.completer() == null;
            }
        }.test();
        gcAndWait(10000, null, 1, null);
        new AssertEquals() {
            @Override
            protected String description() {
            	return "deleted != 1";
            }
            @Override
            protected boolean equals() {
                return 1 == getDeletedCount();
            }
        }.test();
    }

    @Test public final void testQComboBoxSetCompleterNoLineEdit() {
        final QComboBox box = new QComboBox();

        reset();
        {
            QCompleter completer = new QCompleter();
            completer.destroyed.connect(this::increaseDeleted);
            box.setCompleter(completer);
            completer = null;
        }

            long millis = System.currentTimeMillis();
            while (System.currentTimeMillis() - millis < 1000) {
            	ApplicationInitializer.runGC();
            	QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            }

            assertEquals(box.completer(), null); // the completer wil newer be set because of missing line edit.
            assertEquals(1, getDeletedCount()); 
    }

    @Test public final void testQLineEditSetCompleter() {
        QLineEdit lineEdit = new QLineEdit();

        reset();
        {
            QCompleter completer = new QCompleter();
            completer.destroyed.connect(this::increaseDeleted);
            lineEdit.setCompleter(completer);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000) {
        	ApplicationInitializer.runGC();
        	QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
        }

        assertTrue(lineEdit.completer() != null);
        synchronized(TestReferenceCounting.class) {
        	assertEquals(0, getDeletedCount());
        }
    }

    @Test public final void testQLineEditSetCompleterNull() {
        final QLineEdit lineEdit = new QLineEdit();

        reset();
        {
            QCompleter completer = new QCompleter();
            completer.destroyed.connect(this::increaseDeleted);
            lineEdit.setCompleter(completer);
            lineEdit.setCompleter(null);

            completer = null;
        }

        new AssertEquals() {
            @Override
            protected String description() {
            	return "lineEdit.completer() != null";
            }
            @Override
            protected boolean equals() {
                return lineEdit.completer() == null;
            }
        }.test();
        gcAndWait(10000, null, 1, null);
        new AssertEquals() {
            @Override
            protected String description() {
            	return "deleted != 1";
            }
            @Override
            protected boolean equals() {
            	synchronized(TestReferenceCounting.class) {
            		return 1 == getDeletedCount();
            	}
            }
        }.test();
    }

    @Test public final void testQComboBoxLineEditSetCompleterNull() {
        QComboBox box = new QComboBox();
        final QLineEdit lineEdit = new QLineEdit();

        box.setLineEdit(lineEdit);

        reset();
        {
            QCompleter completer = new QCompleter();
            completer.destroyed.connect(this::increaseDeleted);
            lineEdit.setCompleter(completer);
            box.lineEdit().setCompleter(null);

            completer = null;
        }

        gcAndWait(10000, null, 1, null);
        new AssertEquals() {
            @Override
            protected String description() {
            	return "deleted != 1";
            }
            @Override
            protected boolean equals() {
        		return 1 == getDeletedCount();
            }
        }.test();

        new AssertEquals() {
            @Override
            protected String description() {
            	return "lineEdit.completer() != null";
            }
            @Override
            protected boolean equals() {
                return lineEdit.completer() == null;
            }
        }.test();
    }

    @Test public final void testQLineEditComboBoxSetCompleter() {
        QComboBox box = new QComboBox();
        QLineEdit lineEdit = new QLineEdit();

        box.setLineEdit(lineEdit);

        reset();
        {
            QCompleter completer = new QCompleter();
            completer.destroyed.connect(this::increaseDeleted);
            box.lineEdit().setCompleter(completer);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000) {
        	ApplicationInitializer.runGC();
        	QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
        }

        assertTrue(lineEdit.completer() != null);
    	assertEquals(0, getDeletedCount());
    }

    @Test public final void testQLineEditComboBoxSetCompleterNull() {
        QComboBox box = new QComboBox();
        final QLineEdit lineEdit = new QLineEdit();

        box.setLineEdit(lineEdit);

        reset();
        {
            QCompleter completer = new QCompleter();
            completer.destroyed.connect(this::increaseDeleted);
            box.lineEdit().setCompleter(completer);
            lineEdit.setCompleter(null);
            completer = null;
        }

        new AssertEquals() {
            @Override
            protected String description() {
            	return "lineEdit.completer() != null";
            }
            @Override
            protected boolean equals() {
                return lineEdit.completer() == null;
            }
        }.test();
        gcAndWait(10000, null, 1, null);
        new AssertEquals() {
            @Override
            protected String description() {
            	return "deleted != 1";
            }
            @Override
            protected boolean equals() {
        		return 1 == getDeletedCount();
            }
        }.test();
    }


    @Test public final void testQComboBoxSetItemDelegate() {
        QComboBox box = new QComboBox();

        reset();
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
            delegate.destroyed.connect(this::increaseDeleted);

            box.setItemDelegate(delegate);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000) {
        	ApplicationInitializer.runGC();
        	Thread.yield();
            try {
				Thread.sleep(20);
			} catch (InterruptedException e) {
			}
        	QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
        }

        assertTrue(box.itemDelegate() != null);
    	assertEquals(0, getDeletedCount());
    }

    @Test public final void testQComboBoxSetItemDelegateNull() {
        QComboBox box = new QComboBox();

        reset();
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
            delegate.destroyed.connect(this::increaseDeleted);
            QAbstractItemDelegate delegate2 = new QAbstractItemDelegate() {

                @Override
                public void paint(QPainter painter, QStyleOptionViewItem option, QModelIndex index) {
                }

                @Override
                public QSize sizeHint(QStyleOptionViewItem option, QModelIndex index) {
                    return null;
                }
            };
            delegate2.destroyed.connect(this::increaseDeleted);


            box.setItemDelegate(delegate);
            box.setItemDelegate(delegate2);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 2000) {
            ApplicationInitializer.runGC();
            Thread.yield();
            try {
				Thread.sleep(20);
			} catch (InterruptedException e) {
			}
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            if(getDeletedCount()==1)
            	break;
        }

        assertTrue(box.itemDelegate() != null);
    	assertEquals(1, getDeletedCount());
    }

    @Test public final void testQComboBoxSetModel() {
        QComboBox box = new QComboBox();

        reset();
        {
            QStandardItemModel model = new QStandardItemModel();
            model.destroyed.connect(this::increaseDeleted);
            box.setModel(model);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000)
            ApplicationInitializer.runGC();

        assertTrue(box.model() != null);
        assertEquals(0, getDeletedCount());

    }

    @Test public final void testQComboBoxSetValidator() {
        QComboBox box = new QComboBox();
        box.setEditable(true);

        reset();
        {
            QValidator validator = new QValidator((QObject)null) {
                @Override
                public State validate(QValidationData arg__1) {
                    // TODO Auto-generated method stub
                    return null;
                }
            };
            validator.destroyed.connect(this::increaseDeleted);

            box.setValidator(validator);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000) {
        	ApplicationInitializer.runGC();
        	QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
        }

        assertTrue(box.validator() != null);
        assertEquals(0, getDeletedCount());
    }

    @Test public final void testQComboBoxSetValidatorNull() {
        final QComboBox box = new QComboBox();
        box.setEditable(true);

        reset();
        {
            QValidator validator = new QValidator((QObject)null) {
                @Override
                public State validate(QValidationData arg__1) {
                    // TODO Auto-generated method stub
                    return null;
                }
            };
            validator.destroyed.connect(this::increaseDeleted);

            box.setValidator(validator);
            box.setValidator(null);

            validator = null;
        }

        new AssertEquals() {
            @Override
            protected String description() {
            	return "box.validator() != null";
            }
            @Override
            protected boolean equals() {
                return box.validator() == null;
            }
        }.test();
        gcAndWait(10000, null, 1, null);
        new AssertEquals() {
            @Override
            protected String description() {
            	return "deleted != 1";
            }
            @Override
            protected boolean equals() {
                return 1 == getDeletedCount();
            }
        }.test();
    }

    @Test public final void testQComboBoxLineEditSetValidator() {
        final QComboBox box = new QComboBox();
        box.setEditable(true);

        reset();
        {
            QValidator validator = new QValidator((QObject)null) {
                @Override
                public State validate(QValidationData arg__1) {
                    // TODO Auto-generated method stub
                    return null;
                }
            };
            validator.destroyed.connect(this::increaseDeleted);

            box.setValidator(validator);
            box.lineEdit().setValidator(null);

            validator = null;
        }

        new AssertEquals() {
            @Override
            protected String description() {
            	return "box.validator() != null";
            }
            @Override
            protected boolean equals() {
                return box.validator() == null;
            }
        }.test();
        gcAndWait(10000, (long)250, 0, null);
        new AssertEquals() {
            @Override
            protected String description() {
            	return "deleted != 0";
            }
            @Override
            protected boolean equals() {
              //  return 1 == deleted;  // this wan't work because box will probably remember the validator.
                return 0 == getDeletedCount();
            }
        }.test();
    }

    @Test public final void testQButtonGroupAddButton() {
        QButtonGroup group = new QButtonGroup();

        reset();
        for (int i=0; i<COUNT; ++i) {
        	QPushButton button;
            group.addButton(button = new QPushButton("button" + i));
            button.destroyed.connect(this::increaseDeleted);
            button = null;
            ApplicationInitializer.runGC();
        }

        List<QAbstractButton> buttons = group.buttons();
        assertEquals(COUNT, buttons.size());
        assertEquals(0, getDeletedCount());
        assertEquals("button" + COUNT / 2, buttons.get(COUNT / 2).text());
    }

    @Test public final void testQButtonGroupAddButtonId() {
        QButtonGroup group = new QButtonGroup();

        reset();
        for (int i=0; i<COUNT; ++i) {
        	QPushButton button;
            group.addButton(button = new QPushButton("button" + i), i);
            button.destroyed.connect(this::increaseDeleted);
            button = null;
            ApplicationInitializer.runGC();
        }

        List<QAbstractButton> buttons = group.buttons();
        assertEquals(COUNT, buttons.size());
        assertEquals(0, getDeletedCount());
        assertEquals(COUNT / 2, group.id(buttons.get(COUNT / 2)));
        assertEquals("button" + COUNT / 2, buttons.get(COUNT / 2).text());
    }

    @Test public final void testQButtonGroupRemoveButton() {
        final QButtonGroup group = new QButtonGroup();

        reset();
        for (int i=0; i<COUNT; ++i) {
            QPushButton button = new QPushButton("button" + i);
        	button.destroyed.connect(this::increaseDeleted);
            group.addButton(button);
            group.removeButton(button);
            ApplicationInitializer.runGC();
        }

        new AssertEquals() {
            @Override
            protected String description() {
            	return "group.buttons().size() != 0";
            }
            @Override
            protected boolean equals() {
                return group.buttons().size() == 0;
            }
        }.test();
        gcAndWait(10000, null, COUNT, null);
        new AssertEquals() {
            @Override
            protected String description() {
            	return "deleted != "+COUNT;
            }
            @Override
            protected boolean equals() {
                return COUNT == getDeletedCount();
            }
        }.test();
    }

    @Test public final void testQAbstractItemViewSetItemDelegate() {
        QAbstractItemView view = new QAbstractItemView() {

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

        };

        reset();
        {
            QAbstractItemDelegate delegate = new QAbstractItemDelegate() {

                @Override
                public void paint(QPainter painter, QStyleOptionViewItem option, QModelIndex index) {
                    // TODO Auto-generated method stub

                }

                @Override
                public QSize sizeHint(QStyleOptionViewItem option, QModelIndex index) {
                    // TODO Auto-generated method stub
                    return null;
                }

            };
            delegate.destroyed.connect(this::increaseDeleted);

            view.setItemDelegate(delegate);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000) {
        	ApplicationInitializer.runGC();
        	QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
        }

        assertTrue(view.itemDelegate() != null);
        assertEquals(0, getDeletedCount());

    }

    @Test public final void testQAbstractItemViewSetItemDelegateNull() {
        final QAbstractItemView view = new QAbstractItemView() {

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

        };

        reset();
        {
            QAbstractItemDelegate delegate = new QAbstractItemDelegate() {

                @Override
                public void paint(QPainter painter, QStyleOptionViewItem option, QModelIndex index) {
                    // TODO Auto-generated method stub

                }

                @Override
                public QSize sizeHint(QStyleOptionViewItem option, QModelIndex index) {
                    // TODO Auto-generated method stub
                    return null;
                }

            };
            delegate.destroyed.connect(this::increaseDeleted);

            view.setItemDelegate(delegate);
            view.setItemDelegate(null);

            delegate = null;
        }

        new AssertEquals() {
            @Override
            protected String description() {
            	return "view.itemDelegate() != null";
            }
            @Override
            protected boolean equals() {
                return view.itemDelegate() == null;
            }
        }.test();
        gcAndWait(10000, null, 1, null);
        new AssertEquals() {
            @Override
            protected String description() {
            	return "deleted != 1";
            }
            @Override
            protected boolean equals() {
                return 1 == getDeletedCount();
            }
        }.test();
    }

    @Test public final void testQAbstractItemViewSetModel() {
        QAbstractItemView view = new QAbstractItemView() {

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

        };

        reset();
        {
            QStandardItemModel model = new QStandardItemModel();
        	model.destroyed.connect(this::increaseDeleted);

            view.setModel(model);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000) {
        	ApplicationInitializer.runGC();
        	QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
        }

        assertTrue(view.model() != null);
        assertEquals(0, getDeletedCount());
    }

    @Test public final void testQAbstractItemViewSetSelectionModelThenSetModel() {
        final QAbstractItemView view = new QAbstractItemView() {

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

        };

        reset();
        {
            QItemSelectionModel model = new QItemSelectionModel(view.model());
            model.destroyed.connect(this::increaseDeleted);

            view.setSelectionModel(model);
            view.setModel(new QStandardItemModel());

            model = null;
        }

        new AssertEquals() {
            @Override
            protected boolean equals() {
                return view.selectionModel() != null;
            }
            @Override
            protected String description() {
            	return "view.selectionModel() == null";
            }
        }.test();
        gcAndWait(10000, null, 1, null);
        new AssertEquals() {
            @Override
            protected boolean equals() {
                return 1 == getDeletedCount();
            }
            @Override
            protected String description() {
            	return "deleted != 1";
            }
        }.test();

    }

    @Test public final void testQAbstractItemViewSetSelectionModel() {
        QAbstractItemView view = new QAbstractItemView() {

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

        };

        reset();
        {
            view.setModel(new QFileSystemModel());

            QItemSelectionModel model = new QItemSelectionModel(view.model());
            model.destroyed.connect(this::increaseDeleted);

            view.setSelectionModel(model);

            model = null;
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000)
            ApplicationInitializer.runGC();

        assertTrue(view.selectionModel() != null);
        assertEquals(0, getDeletedCount());


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

    @Test
    public final void setWidgetMappingToNull() {
		QSignalMapper mapper = new QSignalMapper();

        boolean caughtException = false;
        try {
            mapper.setMapping(new QObject(), (QWidget) null);
        } catch (NullPointerException e) {
            caughtException = true;
        }

        assertFalse(caughtException);
    }

    @Test
    public final void setObjectMappingToNull() {
        QSignalMapper mapper = new QSignalMapper();

        boolean caughtException = false;
        try {
            mapper.setMapping(new QObject(), (QObject) null);
        } catch (NullPointerException e) {
            caughtException = true;
        }

        assertFalse(caughtException);
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestReferenceCounting.class.getName());
    }
}
