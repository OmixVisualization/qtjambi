/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

import java.lang.ref.PhantomReference;
import java.lang.ref.Reference;
import java.lang.ref.ReferenceQueue;
import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Vector;

import org.junit.After;
import org.junit.Assume;
import org.junit.Before;
import org.junit.BeforeClass;

import io.qt.QtUtilities;
import io.qt.core.QAbstractProxyModel;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QItemSelection;
import io.qt.core.QItemSelectionModel;
import io.qt.core.QModelIndex;
import io.qt.core.QObject;
import io.qt.core.QPoint;
import io.qt.core.QRect;
import io.qt.core.QSignalMapper;
import io.qt.core.QSize;
import io.qt.core.QSortFilterProxyModel;
import io.qt.core.Qt.KeyboardModifiers;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QIcon;
import io.qt.gui.QIcon.Mode;
import io.qt.gui.QIconEngine;
import io.qt.gui.QPainter;
import io.qt.gui.QRegion;
import io.qt.gui.QStandardItemModel;
import io.qt.gui.QValidator;
import io.qt.widgets.QAbstractButton;
import io.qt.widgets.QAbstractItemDelegate;
import io.qt.widgets.QAbstractItemView;
import io.qt.widgets.QAction;
import io.qt.widgets.QApplication;
import io.qt.widgets.QButtonGroup;
import io.qt.widgets.QComboBox;
import io.qt.widgets.QCompleter;
import io.qt.widgets.QDirModel;
import io.qt.widgets.QLineEdit;
import io.qt.widgets.QListWidget;
import io.qt.widgets.QListWidgetItem;
import io.qt.widgets.QPushButton;
import io.qt.widgets.QStackedLayout;
import io.qt.widgets.QStyleOptionViewItem;
import io.qt.widgets.QTableView;
import io.qt.widgets.QWidget;

public abstract class AbstractTestReferenceCounting extends QApplicationTest {
    private static ReferenceQueue<QObject> weakReferenceQueue = new ReferenceQueue<QObject>();
    private static ReferenceQueue<QObject> phantomReferenceQueue = new ReferenceQueue<QObject>();
    private static Map<WeakReference<QObject>,Integer> weakReferenceMap = new HashMap<WeakReference<QObject>,Integer>();
    private static Map<PhantomReference<QObject>,Integer> phantomReferenceMap = new HashMap<PhantomReference<QObject>,Integer>();
    private static List<Integer> alive = new ArrayList<Integer>();
    private static List<Integer> aliveAndUnderTest = new ArrayList<Integer>();
    private static int counter = 0;

    public class AssertEquals {
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

                gc();
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
                Utils.println(3, "AssertEquals.test(timeout=" + timeout + ")  couldStopAt="+couldStopAt+"; diff="+(stop-(couldStopAt!=null?couldStopAt.longValue():0)));
            assertTrue(""+this.getClass().getName(), equals());
        }
    }

    private static final int COUNT = 200;
    @SuppressWarnings("unused")
    private long startTimeMillis;

    private void reset() {
        synchronized(AbstractTestReferenceCounting.class) {
            deleted = 0;
        }
    }

    private int getDeletedCount() {
        synchronized(AbstractTestReferenceCounting.class) {
            return deleted;
        }
    }

    private void gc() {
        System.gc();
        System.runFinalization();
    }
    
    @BeforeClass
    public static void testInitialize() throws Exception {
        QApplicationTest.testInitialize();
		Assume.assumeThat(QGuiApplication.primaryScreen()!=null, QApplicationTest.trueMatcher("A screen is required to create a window."));
    }

    @Before
    public void setUp() {
        QApplication.processEvents();
        clearGcReferences();
        startTimeMillis = System.currentTimeMillis();
    }

    @After
    public void tearDown() {
        System.gc();
        System.runFinalization();
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
        QApplication.processEvents();
        System.gc();
        System.runFinalization();
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
        QApplication.processEvents();
    }

    public static Integer getIdNext() {
        int idNext;
        synchronized(AbstractTestReferenceCounting.class) {
            counter++;
            idNext = counter;
        }
        return Integer.valueOf(idNext);
    }
    public void clearGcReferences() {
        synchronized(AbstractTestReferenceCounting.class) {
            aliveAndUnderTest.clear();
        }
    }
    public int getSize() {
        synchronized(AbstractTestReferenceCounting.class) {
            return aliveAndUnderTest.size();
        }
    }

    // This does not have "id" field, so make one up for it
    @SuppressWarnings("unused")
	private static void accountingForNewObject(QObject o, Integer id) {
        assertNotNull(o);

        if(id == null)
            id = getIdNext();

        if(Utils.isDebugLevel(4)) {
            String className = o.getClass().getName();
            String shortClassName = className;
            int i = shortClassName.lastIndexOf('.');
            if(i > 0)
                shortClassName = shortClassName.substring(i + 1);
            Utils.println(4, shortClassName + ".ctor " + className + "@" + System.identityHashCode(o) + "; thread=" + Thread.currentThread().getId() + "; id=" + id);
        }
        WeakReference<QObject> wr = new WeakReference<QObject>(o, weakReferenceQueue);
        PhantomReference<QObject> pr = new PhantomReference<QObject>(o, phantomReferenceQueue);
        synchronized(AbstractTestReferenceCounting.class) {
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
            ", " + Utils.toStringOrNull(excessLimitMillis) +
            ", " + Utils.toStringOrNull(disposedCount) +
            ", " + Utils.toStringOrNull(destroyedCount) + ")";
        while(loop) {
            try {

                System.gc();
                System.runFinalization();

                Reference<? extends QObject> thisWr;
                while((thisWr = weakReferenceQueue.poll()) != null) {
                    Integer tmpId;
                    synchronized(AbstractTestReferenceCounting.class) {
                        tmpId = weakReferenceMap.remove(thisWr);
                    }
                    Utils.println(5, " weakReferenceQueue.remove(): dequeued id=" + tmpId);
                }
                Reference<? extends QObject> thisPr;
                while((thisPr = phantomReferenceQueue.poll()) != null) {
                    Integer tmpId;
                    synchronized(AbstractTestReferenceCounting.class) {
                        tmpId = phantomReferenceMap.remove(thisPr);
                    }
                    Utils.println(5, " phantomReferenceQueue.remove(): dequeued id=" + tmpId);
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

                    Utils.println(5, debugPrefix + ": elapsed=" + elapsed + "; end loop" +
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
        if(Utils.isDebugLevel(4)) {
            // Print array format [1, 2, 3]
            synchronized(AbstractTestReferenceCounting.class) {
                String aliveString = Utils.listToString(alive);
                String aliveAndUnderTestString = Utils.listToString(aliveAndUnderTest);
                String weakReferenceMapString = Utils.mapValueToString(weakReferenceMap);
                String phantomReferenceMapString = Utils.mapValueToString(phantomReferenceMap);
                Utils.println(4, "elapsed=" + elapsed + "; alive=" + aliveString + "; aliveAndUnderTest=" + aliveAndUnderTestString +
                        "; weakReferenceMapSize=" + weakReferenceMapString + "; phantomReferenceMapSize=" + phantomReferenceMapString);
            }
        }
        return obtainGoal;
    }


    private int deleted = 0;
    
    private void increaseDeleted() {
	    synchronized(AbstractTestReferenceCounting.class) {
	        deleted++;
	    }
    }
    
    void testQWidgetAddAction() {
        QWidget w = new QWidget();
        reset();

        for (int i=0; i<COUNT; ++i) {
        	QAction act = new QAction("action" + i, null);
            w.addAction(act);
            act.destroyed.connect(this::increaseDeleted);
            gc();
        }

        assertEquals(0, deleted);
        List<QAction> actions = w.actions();
        assertEquals(COUNT, actions.size());
        for (int i=0; i<COUNT; ++i) {
            assertTrue(actions.get(i) != null);
            assertEquals("action" + i, actions.get(i).text());
        }
    }

    void testQWidgetInsertAction() {
        QWidget w = new QWidget();
        reset();

        for (int i=0; i<COUNT; ++i) {
        	QAction act = new QAction("action" + i, null);
            w.insertAction(null, act);
            act.destroyed.connect(this::increaseDeleted);
            act = null;
            gc();
        }

        assertEquals(0, deleted);
        List<QAction> actions = w.actions();
        assertEquals(COUNT, actions.size());
        for (int i=0; i<COUNT; ++i) {
            assertTrue(actions.get(i) != null);
            assertEquals("action" + i, actions.get(i).text());
        }
    }

    void testQWidgetAddActionDuplicate() {
        QWidget w = new QWidget();

        reset();

        {
            QAction act = new QAction("action", null);
            act.destroyed.connect(this::increaseDeleted);

            w.addAction(act);
            w.addAction(act);

            assertEquals(1, w.actions().size());

            w.removeAction(act);
            act = null;
            assertEquals(0, w.actions().size());
        }
        gcAndWait(10000, null, 1, null);
        new AssertEquals() {
            @Override
            protected boolean equals() {
                return 1 == deleted;
            }
        }.test();
    }

    void testQWidgetActionsNull() {
        QWidget w = new QWidget();
        w.addAction(null);
        w.addActions(null);
        w.removeAction(null);
        assertEquals(0, w.actions().size());
    }

    void testQWidgetAddActions() {
        QWidget w = new QWidget();

        reset();
        for (int i=0; i<COUNT; ++i) {
            {
                List<QAction> actions = new LinkedList<QAction>();
                QAction act = new QAction("action" + i, null);
                actions.add(act);
            	act.destroyed.connect(this::increaseDeleted);
                w.addActions(actions);
                act = null;
                actions = null;
            }
            gc();
        }

        assertEquals(0, deleted);
        List<QAction> actions = w.actions();
        assertEquals(COUNT, actions.size());
        for (int i=0; i<COUNT; ++i) {
            assertTrue(actions.get(i) != null);
            assertEquals("action" + i, actions.get(i).text());
        }
    }

    void testQWidgetInsertActions() {
        QWidget w = new QWidget();

        reset();
        for (int i=0; i<COUNT; ++i) {
            {
                List<QAction> actions = new LinkedList<QAction>();
                QAction act = new QAction("action" + i, null);
                actions.add(act);
                act.destroyed.connect(this::increaseDeleted);
                w.insertActions(null, actions);
                act = null;
                actions = null;
            }
            gc();
        }

        assertEquals(0, deleted);
        List<QAction> actions = w.actions();
        assertEquals(COUNT, actions.size());
        for (int i=0; i<COUNT; ++i) {
            assertTrue(actions.get(i) != null);
            assertEquals("action" + i, actions.get(i).text());
        }
    }

    void testQWidgetRemoveAction() {
        final QWidget w = new QWidget();

        reset();
        for (int i=0; i<COUNT; ++i) {
            {
                QAction act = new QAction("action" + i, null);
                act.destroyed.connect(this::increaseDeleted);
                w.addAction(act);
                w.removeAction(act);
                act = null;
            }
            gc();
        }
        gcAndWait(10000, null, COUNT, null);
        new AssertEquals() {
            @Override
            protected boolean equals() {
                return COUNT == deleted;
            }
        }.test();
        new AssertEquals() {
            @Override
            protected boolean equals() {
                return 0 == w.actions().size();
            }
        }.test();
    }

    void testQAbstractProxyModelSetSourceModel() {
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
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertEquals(0, deleted);
        assertEquals("source model", proxy.sourceModel().objectName());
    }

    void testQAbstractProxyModelSetSourceModelNull() {
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
            protected boolean equals() {
                return 1 == deleted;
            }
        }.test();

        new AssertEquals() {
            @Override
            protected boolean equals() {
                return null == proxy.sourceModel();
            }
        }.test();
    }

    void testQSortFilterProxyModelSetSourceModel() {
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
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertEquals(0, deleted);
        assertEquals("source model", proxy.sourceModel().objectName());
    }

    void testQSortFilterProxyModelSetSourceModelNull() {
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
            protected boolean equals() {
                return 1 == deleted;
            }
        }.test();

        new AssertEquals() {
            @Override
            protected boolean equals() {
                return null == proxy.sourceModel();
            }
        }.test();
    }

    void testQStackedLayoutAddStackedWidget() {
        QStackedLayout layout = new QStackedLayout();

        reset();
        for (int i=0; i<COUNT; ++i) {
            QWidget widget = new QWidget();
            widget.destroyed.connect(this::increaseDeleted);
            layout.addWidget(widget);
            widget = null;
            gcAndWait(0, null, null, null);
        }

        assertEquals(COUNT, layout.count());
        assertEquals(0, deleted);
    }

    void testQStackedLayoutRemoveWidget() {
        final QStackedLayout layout = new QStackedLayout();

        reset();
        for (int i=0; i<COUNT; ++i) {
            QWidget widget = new QWidget();
            widget.destroyed.connect(this::increaseDeleted);
            layout.addWidget(widget);
            layout.removeWidget(widget);
            widget = null;
            gcAndWait(0, null, null, null);
        }

        new AssertEquals() {
            @Override
            protected boolean equals() {
                return 0 == layout.count();
            }
        }.test();
        gcAndWait(10000, null, COUNT, null);
        new AssertEquals() {
            @Override
            protected boolean equals() {
                return COUNT == deleted;
            }
        }.test();
    }

    void testQStackedLayoutAddWidget() {
        QStackedLayout layout = new QStackedLayout();

        reset();
        for (int i=0; i<COUNT; ++i) {
            QWidget widget = new QWidget();
            widget.destroyed.connect(this::increaseDeleted);
            // layout.addWidget() is wrong here
            layout.addWidget(widget);
            QApplication.processEvents(); // force ChildAdd/ChildRemove processing
            widget = null;   // kill hard-reference

            gcAndWait(0, null, null, null);
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertEquals(COUNT, layout.count());
        assertEquals(0, deleted);
    }

    void testQStackedLayoutAddWidget2() {
        QStackedLayout layout = new QStackedLayout();

        reset();
        for (int i=0; i<COUNT; ++i) {
            QWidget widget = new QWidget();
            widget.destroyed.connect(this::increaseDeleted);
            // layout.addWidget() is wrong here
            layout.addWidget(widget);
            widget = null;
            gcAndWait(0, null, null, null);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertEquals(COUNT, layout.count());
        assertEquals(0, deleted);
    }

    void testQStackedLayoutAddRemoveWidget() {
        final QStackedLayout layout = new QStackedLayout();

        reset();
        for (int i=0; i<COUNT; ++i) {
            QWidget widget = new QWidget();
            widget.destroyed.connect(this::increaseDeleted);
            // layout.addWidget() is wrong here
            layout.addWidget(widget);
            layout.removeWidget(widget);
            widget = null;
            gcAndWait(0, null, null, null);
        }

        new AssertEquals() {
            @Override
            protected boolean equals() {
                return 0 == layout.count();
            }
        }.test();
        gcAndWait(10000, null, COUNT, null);
        new AssertEquals() {
            @Override
            protected boolean equals() {
                return COUNT == deleted;
            }
        }.test();
    }

    void testQComboBoxSetCompleter() {
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
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertTrue(box.completer() != null);
        assertEquals(0, deleted);
    }

    void testQComboBoxEditableSetCompleter() {
        QComboBox box = new QComboBox();
        box.setEditable(true);

        reset();
        {
            QCompleter completer = new QCompleter();
            completer.destroyed.connect(this::increaseDeleted);
            box.setCompleter(completer);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertTrue(box.completer() != null);
        assertEquals(0, deleted);
    }


    void testQComboBoxSetCompleterNull() {
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
            protected boolean equals() {
                return box.completer() == null;
            }
        }.test();
        gcAndWait(10000, null, 1, null);
        new AssertEquals() {
            @Override
            protected boolean equals() {
                return 1 == deleted;
            }
        }.test();
    }

    void testQComboBoxSetCompleterNoLineEdit() {
        final QComboBox box = new QComboBox();

        reset();
        {
            QCompleter completer = new QCompleter();
            completer.destroyed.connect(this::increaseDeleted);
            box.setCompleter(completer);
            completer = null;
        }

            long millis = System.currentTimeMillis();
            while (System.currentTimeMillis() - millis < 1000)
                gc();

            assertEquals(box.completer(), null); // the completer wil newer be set because of missing line edit.
            assertEquals(0, deleted); // and not deleted, since it has been set.
    }

    void testQLineEditSetCompleter() {
        QLineEdit lineEdit = new QLineEdit();

        reset();
        {
            QCompleter completer = new QCompleter();
            completer.destroyed.connect(this::increaseDeleted);
            lineEdit.setCompleter(completer);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertTrue(lineEdit.completer() != null);
        assertEquals(0, deleted);
    }

    void testQLineEditSetCompleterNull() {
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
            protected boolean equals() {
                return lineEdit.completer() == null;
            }
        }.test();
        gcAndWait(10000, null, 1, null);
        new AssertEquals() {
            @Override
            protected boolean equals() {
                return 1 == deleted;
            }
        }.test();
    }

    void testQComboBoxLineEditSetCompleterNull() {
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
            protected boolean equals() {
                return 1 == deleted;
            }
        }.test();

        new AssertEquals() {
            @Override
            protected boolean equals() {
                return lineEdit.completer() == null;
            }
        }.test();
    }

    void testQLineEditComboBoxSetCompleter() {
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
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertTrue(lineEdit.completer() != null);
        assertEquals(0, deleted);
    }

    void testQLineEditComboBoxSetCompleterNull() {
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
            protected boolean equals() {
                return lineEdit.completer() == null;
            }
        }.test();
        gcAndWait(10000, null, 1, null);
        new AssertEquals() {
            @Override
            protected boolean equals() {
                return 1 == deleted;
            }
        }.test();
    }


    void testQComboBoxSetItemDelegate() {
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
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertTrue(box.itemDelegate() != null);
        assertEquals(0, deleted);
    }

    void testQComboBoxSetItemDelegateNull() {
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
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertTrue(box.itemDelegate() != null);
        assertEquals(1, deleted);
    }

    void testQComboBoxSetModel() {
        QComboBox box = new QComboBox();

        reset();
        {
            QStandardItemModel model = new QStandardItemModel();
            model.destroyed.connect(this::increaseDeleted);
            box.setModel(model);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertTrue(box.model() != null);
        assertEquals(0, deleted);

    }

    void testQComboBoxSetValidator() {
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
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertTrue(box.validator() != null);
        assertEquals(0, deleted);
    }

    void testQComboBoxSetValidatorNull() {
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
            protected boolean equals() {
                return box.validator() == null;
            }
        }.test();
        gcAndWait(10000, null, 1, null);
        new AssertEquals() {
            @Override
            protected boolean equals() {
                return 1 == deleted;
            }
        }.test();
    }

    void testQComboBoxLineEditSetValidator() {
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
            protected boolean equals() {
                return box.validator() == null;
            }
        }.test();
        gcAndWait(10000, (long)250, 0, null);
        new AssertEquals() {
            @Override
            protected boolean equals() {
              //  return 1 == deleted;  // this wan't work because box will probably remember the validator.
                return 0 == deleted;
            }
        }.test();
    }

    void testQButtonGroupAddButton() {
        QButtonGroup group = new QButtonGroup();

        reset();
        for (int i=0; i<COUNT; ++i) {
        	QPushButton button;
            group.addButton(button = new QPushButton("button" + i));
            button.destroyed.connect(this::increaseDeleted);
            button = null;
            gc();
        }

        List<QAbstractButton> buttons = group.buttons();
        assertEquals(COUNT, buttons.size());
        assertEquals(0, deleted);
        assertEquals("button" + COUNT / 2, buttons.get(COUNT / 2).text());
    }

    void testQButtonGroupAddButtonId() {
        QButtonGroup group = new QButtonGroup();

        reset();
        for (int i=0; i<COUNT; ++i) {
        	QPushButton button;
            group.addButton(button = new QPushButton("button" + i), i);
            button.destroyed.connect(this::increaseDeleted);
            button = null;
            gc();
        }

        List<QAbstractButton> buttons = group.buttons();
        assertEquals(COUNT, buttons.size());
        assertEquals(0, deleted);
        assertEquals(COUNT / 2, group.id(buttons.get(COUNT / 2)));
        assertEquals("button" + COUNT / 2, buttons.get(COUNT / 2).text());
    }

    void testQButtonGroupRemoveButton() {
        final QButtonGroup group = new QButtonGroup();

        reset();
        for (int i=0; i<COUNT; ++i) {
            QPushButton button = new QPushButton("button" + i);
        	button.destroyed.connect(this::increaseDeleted);
            group.addButton(button);
            group.removeButton(button);
            gc();
        }

        new AssertEquals() {
            @Override
            protected boolean equals() {
                return group.buttons().size() == 0;
            }
        }.test();
        gcAndWait(10000, null, COUNT, null);
        new AssertEquals() {
            @Override
            protected boolean equals() {
                return COUNT == deleted;
            }
        }.test();
    }

    void testQAbstractItemViewSetItemDelegate() {
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
            protected QModelIndex moveCursor(CursorAction cursorAction, KeyboardModifiers modifiers) {
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
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertTrue(view.itemDelegate() != null);
        assertEquals(0, deleted);

    }

    void testQAbstractItemViewSetItemDelegateNull() {
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
            protected QModelIndex moveCursor(CursorAction cursorAction, KeyboardModifiers modifiers) {
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
            protected boolean equals() {
                return view.itemDelegate() == null;
            }
        }.test();
        gcAndWait(10000, null, 1, null);
        new AssertEquals() {
            @Override
            protected boolean equals() {
                return 1 == deleted;
            }
        }.test();
    }

    void testQAbstractItemViewSetModel() {
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
            protected QModelIndex moveCursor(CursorAction cursorAction, KeyboardModifiers modifiers) {
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
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertTrue(view.model() != null);
        assertEquals(0, deleted);
    }

    void testQAbstractItemViewSetSelectionModelThenSetModel() {
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
            protected QModelIndex moveCursor(CursorAction cursorAction, KeyboardModifiers modifiers) {
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
        }.test();
        gcAndWait(10000, null, 1, null);
        new AssertEquals() {
            @Override
            protected boolean equals() {
                return 1 == deleted;
            }
        }.test();

    }

    void testQAbstractItemViewSetSelectionModel() {
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
            protected QModelIndex moveCursor(CursorAction cursorAction, KeyboardModifiers modifiers) {
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
            view.setModel(new QDirModel());

            QItemSelectionModel model = new QItemSelectionModel(view.model());
            model.destroyed.connect(this::increaseDeleted);

            view.setSelectionModel(model);

            model = null;
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertTrue(view.selectionModel() != null);
        assertEquals(0, deleted);


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
        public void paint(QPainter painter, QRect rect, Mode mode, io.qt.gui.QIcon.State state) {

        }

        @Override
        public QIconEngine clone() {
            return new MyIconEngine(i);
        }
    }

    void testIconEngine() {
        QListWidget w = new QListWidget();

        deletedEngines = 0;

        for (int i=0; i<100; ++i) {
            w.addItem(new QListWidgetItem(new QIcon(new MyIconEngine(i)), "" + i));
            gc();
        }

        // Don't crash
        w.show();
        assertEquals(0, deletedEngines);
    }

    void setItemDelegateForRowToNull() {
        QAbstractItemView view = new QTableView();

        boolean caughtException = false;
        try {
            view.setItemDelegateForRow(0, null);
        } catch (NullPointerException e) {
            caughtException = true;
        }

        assertFalse(caughtException);
    }

    void setItemDelegateForColumnToNull() {
        QAbstractItemView view = new QTableView();

        boolean caughtException = false;
        try {
            view.setItemDelegateForColumn(0, null);
        } catch (NullPointerException e) {
            caughtException = true;
        }

        assertFalse(caughtException);
    }

    void setWidgetMappingToNull() {
		QSignalMapper mapper = new QSignalMapper();

        boolean caughtException = false;
        try {
            mapper.setMapping(new QObject(), (QWidget) null);
        } catch (NullPointerException e) {
            caughtException = true;
        }

        assertFalse(caughtException);
    }

    void setObjectMappingToNull() {
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
        org.junit.runner.JUnitCore.main(AbstractTestReferenceCounting.class.getName());
    }
}
