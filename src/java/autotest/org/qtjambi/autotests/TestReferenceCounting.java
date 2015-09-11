/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package org.qtjambi.autotests;

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
import org.junit.Before;
import org.junit.Test;

import org.qtjambi.qt.core.QAbstractProxyModel;
import org.qtjambi.qt.core.QCoreApplication;
import org.qtjambi.qt.core.QEvent;
import org.qtjambi.qt.core.QItemSelection;
import org.qtjambi.qt.core.QItemSelectionModel;
import org.qtjambi.qt.core.QModelIndex;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.core.QPoint;
import org.qtjambi.qt.core.QRect;
import org.qtjambi.qt.core.QSignalMapper;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.QSortFilterProxyModel;
import org.qtjambi.qt.core.Qt.KeyboardModifiers;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.gui.QIcon.Mode;
import org.qtjambi.qt.gui.QIconEngine;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QRegion;
import org.qtjambi.qt.gui.QStandardItemModel;
import org.qtjambi.qt.gui.QValidator;
import org.qtjambi.qt.widgets.QAbstractItemDelegate;
import org.qtjambi.qt.widgets.QAbstractItemView;
import org.qtjambi.qt.widgets.QAction;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QButtonGroup;
import org.qtjambi.qt.widgets.QComboBox;
import org.qtjambi.qt.widgets.QCompleter;
import org.qtjambi.qt.widgets.QDirModel;
import org.qtjambi.qt.widgets.QGuiSignalMapper;
import org.qtjambi.qt.widgets.QLineEdit;
import org.qtjambi.qt.widgets.QListWidget;
import org.qtjambi.qt.widgets.QListWidgetItem;
import org.qtjambi.qt.widgets.QPushButton;
import org.qtjambi.qt.widgets.QStackedLayout;
import org.qtjambi.qt.widgets.QStyleOptionViewItem;
import org.qtjambi.qt.widgets.QTableView;
import org.qtjambi.qt.widgets.QWidget;

public class TestReferenceCounting extends QApplicationTest {
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
            assertTrue(equals());
        }
    }

    private static final int COUNT = 200;
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

    private void gc() {
        System.gc();
        System.runFinalization();
        if(Utils.releaseNativeResources() > 0) {
            System.gc();
            System.runFinalization();
        }
    }

    @Before
    public void setUp() {
        QApplication.processEvents();
        clearGcReferences();
        startTimeMillis = System.currentTimeMillis();
    }

    @After
    public void tearDown() {
        long endTimeMillis = System.currentTimeMillis();
        System.gc();
        System.runFinalization();
        if(Utils.releaseNativeResources() > 0) {
            System.gc();
            System.runFinalization();
        }

        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDelete.value());
        QApplication.processEvents();

        QtJambiUnittestTools.getObjectCount(1, 0);  // fflush(stdout)
        QtJambiUnittestTools.getObjectCount(2, 0);  // fflush(stderr)
        int objectCount = QtJambiUnittestTools.getObjectCount(3, 0);  // QtJambiLink::QtJambiLink_dump()
        QtJambiUnittestTools.getObjectCount(2, 0);  // fflush(stderr)
        Utils.println(3, "TestReferenceCounting.tearDown(): end objectCount="+objectCount);
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
        synchronized(TestReferenceCounting.class) {
            weakReferenceMap.put(wr, id);
            phantomReferenceMap.put(pr, id);
            aliveAndUnderTest.add(id);
            alive.add(id);
        }
    }

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

                Utils.releaseNativeResources();
                System.gc();
                System.runFinalization();

                Reference<? extends QObject> thisWr;
                while((thisWr = weakReferenceQueue.poll()) != null) {
                    Integer tmpId;
                    synchronized(TestReferenceCounting.class) {
                        tmpId = weakReferenceMap.remove(thisWr);
                    }
                    Utils.println(5, " weakReferenceQueue.remove(): dequeued id=" + tmpId);
                }
                Reference<? extends QObject> thisPr;
                while((thisPr = phantomReferenceQueue.poll()) != null) {
                    Integer tmpId;
                    synchronized(TestReferenceCounting.class) {
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

                if(loop)
                    Thread.sleep(10);
            } catch(Exception e) {	// exceptions are an idiotic concept; yeah in c++
                e.printStackTrace();
            }
        }

        // Report on status
        if(Utils.isDebugLevel(4)) {
            // Print array format [1, 2, 3]
            synchronized(TestReferenceCounting.class) {
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
    @Test public void testQWidgetAddAction() {
        QWidget w = new QWidget();
        reset();

        for (int i=0; i<COUNT; ++i) {
            w.addAction(new QAction("action" + i, null) {

                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }


            });
            gc();
        }

        assertEquals(0, deleted);
        assertEquals(COUNT, w.actions().size());
        for (int i=0; i<COUNT; ++i) {
            assertTrue(w.actions().get(i) != null);
            assertEquals("action" + i, w.actions().get(i).text());
        }
    }

    @Test public void testQWidgetInsertAction() {
        QWidget w = new QWidget();
        reset();

        for (int i=0; i<COUNT; ++i) {
            w.insertAction(null, new QAction("action" + i, null) {

                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }


            });
            gc();
        }

        assertEquals(0, deleted);
        assertEquals(COUNT, w.actions().size());
        for (int i=0; i<COUNT; ++i) {
            assertTrue(w.actions().get(i) != null);
            assertEquals("action" + i, w.actions().get(i).text());
        }
    }

    @Test public void testQWidgetAddActionDuplicate() {
        QWidget w = new QWidget();

        reset();

        {
            QAction act = new QAction("action", null) {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }
            };

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

    @Test public void testQWidgetActionsNull() {
        QWidget w = new QWidget();
        w.addAction(null);
        w.addActions(null);
        w.removeAction(null);
        assertEquals(0, w.actions().size());
    }

    @Test public void testQWidgetAddActions() {
        QWidget w = new QWidget();

        reset();
        for (int i=0; i<COUNT; ++i) {
            {
                List<QAction> actions = new LinkedList<QAction>();
                actions.add(new QAction("action" + i, null) {
                    @Override
                    public void disposed() {
                        synchronized(TestReferenceCounting.class) {
                            deleted++;
                        }
                    }
                });
                w.addActions(actions);
            }
            gc();
        }

        assertEquals(0, deleted);
        assertEquals(COUNT, w.actions().size());
        for (int i=0; i<COUNT; ++i) {
            assertTrue(w.actions().get(i) != null);
            assertEquals("action" + i, w.actions().get(i).text());
        }
    }

    @Test public void testQWidgetInsertActions() {
        QWidget w = new QWidget();

        reset();
        for (int i=0; i<COUNT; ++i) {
            {
                List<QAction> actions = new LinkedList<QAction>();
                actions.add(new QAction("action" + i, null) {
                    @Override
                    public void disposed() {
                        synchronized(TestReferenceCounting.class) {
                            deleted++;
                        }
                    }
                });
                w.insertActions(null, actions);
            }
            gc();
        }

        assertEquals(0, deleted);
        assertEquals(COUNT, w.actions().size());
        for (int i=0; i<COUNT; ++i) {
            assertTrue(w.actions().get(i) != null);
            assertEquals("action" + i, w.actions().get(i).text());
        }
    }

    @Test public void testQWidgetRemoveAction() {
        final QWidget w = new QWidget();

        reset();
        for (int i=0; i<COUNT; ++i) {
            {
                QAction act = new QAction("action" + i, null) {
                    @Override
                    public void disposed() {
                        synchronized(TestReferenceCounting.class) {
                            deleted++;
                        }
                    }
                };
                w.addAction(act);
                w.removeAction(act);
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

    @Test public void testQAbstractProxyModelSetSourceModel() {
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
            QStandardItemModel model = new QStandardItemModel() {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }
            };

            model.setObjectName("source model");

            proxy.setSourceModel(model);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertEquals(0, deleted);
        assertEquals("source model", proxy.sourceModel().objectName());
    }

    @Test public void testQAbstractProxyModelSetSourceModelNull() {
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
            QStandardItemModel model = new QStandardItemModel() {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }
            };


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

    @Test public void testQSortFilterProxyModelSetSourceModel() {
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
            QStandardItemModel model = new QStandardItemModel() {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }
            };

            model.setObjectName("source model");

            proxy.setSourceModel(model);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertEquals(0, deleted);
        assertEquals("source model", proxy.sourceModel().objectName());
    }

    @Test public void testQSortFilterProxyModelSetSourceModelNull() {
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
            QStandardItemModel model = new QStandardItemModel() {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }
            };

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

    // JVM CRASHER
    /**@Test**/ public void testQStackedLayoutAddStackedWidget() {
        QStackedLayout layout = new QStackedLayout();

        reset();
        for (int i=0; i<COUNT; ++i) {
            QWidget widget = new QWidget() {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }
            };
            layout.addStackedWidget(widget);

            gcAndWait(0, null, null, null);
        }

        assertEquals(COUNT, layout.count());
        assertEquals(0, deleted);
    }

    // JVM CRASHER
    /**@Test**/ public void testQStackLayoutRemoveWidget() {
        final QStackedLayout layout = new QStackedLayout();

        reset();
        for (int i=0; i<COUNT; ++i) {
            QWidget widget = new QWidget() {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }
            };
            layout.addStackedWidget(widget);
            layout.removeWidget(widget);

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

    // JVM CRASHER
    /**@Test**/ public void testQStackLayoutAddWidget() {
        QStackedLayout layout = new QStackedLayout();

        reset();
        for (int i=0; i<COUNT; ++i) {
            QWidget widget = new QWidget() {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }
            };
            // layout.addWidget() is wrong here
            layout.addStackedWidget(widget);
            QApplication.processEvents(); // force ChildAdd/ChildRemove processing
            widget = null;   // kill hard-reference

            gcAndWait(0, null, null, null);
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDelete.value());
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertEquals(COUNT, layout.count());
        assertEquals(0, deleted);
    }

    // JVM CRASHER
    /*@Test*/ public void testQStackLayoutAddWidget2() {
        QStackedLayout layout = new QStackedLayout();

        reset();
        for (int i=0; i<COUNT; ++i) {
            QWidget widget = new QWidget() {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }
            };
            // layout.addWidget() is wrong here
            layout.addStackedWidget(widget);

            gcAndWait(0, null, null, null);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertEquals(COUNT, layout.count());
        assertEquals(0, deleted);
    }

    // JVM CRASHER
    /**@Test**/ public void testQStackLayoutAddRemoveWidget() {
        final QStackedLayout layout = new QStackedLayout();

        reset();
        for (int i=0; i<COUNT; ++i) {
            QWidget widget = new QWidget() {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }
            };
            // layout.addWidget() is wrong here
            layout.addStackedWidget(widget);
            layout.removeWidget(widget);

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

    @Test public void testQComboBoxSetCompleter() {
        QComboBox box = new QComboBox();
        QLineEdit lineEdit = new QLineEdit();
        box.setLineEdit(lineEdit);

        reset();
        {
            QCompleter completer = new QCompleter() {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }
            };
            box.setCompleter(completer);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertTrue(box.completer() != null);
        assertEquals(0, deleted);
    }

    @Test public void testQComboBoxEditableSetCompleter() {
        QComboBox box = new QComboBox();
        box.setEditable(true);

        reset();
        {
            QCompleter completer = new QCompleter() {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }
            };
            box.setCompleter(completer);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertTrue(box.completer() != null);
        assertEquals(0, deleted);
    }


    @Test public void testQComboBoxSetCompleterNull() {
        final QComboBox box = new QComboBox();
        QLineEdit lineEdit = new QLineEdit();
        box.setLineEdit(lineEdit);

        reset();
        {
            QCompleter completer = new QCompleter() {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }
            };
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

    @Test public void testQComboBoxSetCompleterNoLineEdit() {
        final QComboBox box = new QComboBox();

        reset();
        {
            QCompleter completer = new QCompleter() {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }
            };
            box.setCompleter(completer);
            completer = null;
        }

            long millis = System.currentTimeMillis();
            while (System.currentTimeMillis() - millis < 1000)
                gc();

            assertEquals(box.completer(), null); // the completer wil newer be set because of missing line edit.
            assertEquals(0, deleted); // and not deleted, since it has been set.
    }

    @Test public void testQLineEditSetCompleter() {
        QLineEdit lineEdit = new QLineEdit();

        reset();
        {
            QCompleter completer = new QCompleter() {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }
            };
            lineEdit.setCompleter(completer);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertTrue(lineEdit.completer() != null);
        assertEquals(0, deleted);
    }

    @Test public void testQLineEditSetCompleterNull() {
        final QLineEdit lineEdit = new QLineEdit();

        reset();
        {
            QCompleter completer = new QCompleter() {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }
            };
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

    @Test public void testQComboBoxLineEditSetCompleterNull() {
        QComboBox box = new QComboBox();
        final QLineEdit lineEdit = new QLineEdit();

        box.setLineEdit(lineEdit);

        reset();
        {
            QCompleter completer = new QCompleter() {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }
            };
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

    @Test public void testQLineEditComboBoxSetCompleter() {
        QComboBox box = new QComboBox();
        QLineEdit lineEdit = new QLineEdit();

        box.setLineEdit(lineEdit);

        reset();
        {
            QCompleter completer = new QCompleter() {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }
            };
            box.lineEdit().setCompleter(completer);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertTrue(lineEdit.completer() != null);
        assertEquals(0, deleted);
    }

    @Test public void testQLineEditComboBoxSetCompleterNull() {
        QComboBox box = new QComboBox();
        final QLineEdit lineEdit = new QLineEdit();

        box.setLineEdit(lineEdit);

        reset();
        {
            QCompleter completer = new QCompleter() {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }
            };
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


    @Test public void testQComboBoxSetItemDelegate() {
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

                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }

            };

            box.setItemDelegate(delegate);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertTrue(box.itemDelegate() != null);
        assertEquals(0, deleted);
    }

    @Test public void testQComboBoxSetItemDelegateNull() {
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

                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }

            };
            QAbstractItemDelegate delegate2 = new QAbstractItemDelegate() {

                @Override
                public void paint(QPainter painter, QStyleOptionViewItem option, QModelIndex index) {
                }

                @Override
                public QSize sizeHint(QStyleOptionViewItem option, QModelIndex index) {
                    return null;
                }

                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }

            };


            box.setItemDelegate(delegate);
            box.setItemDelegate(delegate2);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertTrue(box.itemDelegate() != null);
        assertEquals(1, deleted);
    }

    @Test public void testQComboBoxSetModel() {
        QComboBox box = new QComboBox();

        reset();
        {
            QStandardItemModel model = new QStandardItemModel() {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }
            };
            box.setModel(model);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertTrue(box.model() != null);
        assertEquals(0, deleted);

    }

    @Test public void testQComboBoxSetValidator() {
        QComboBox box = new QComboBox();
        box.setEditable(true);

        reset();
        {
            QValidator validator = new QValidator((QObject)null) {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }

                @Override
                public State validate(QValidationData arg__1) {
                    // TODO Auto-generated method stub
                    return null;
                }
            };

            box.setValidator(validator);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertTrue(box.validator() != null);
        assertEquals(0, deleted);
    }

    @Test public void testQComboBoxSetValidatorNull() {
        final QComboBox box = new QComboBox();
        box.setEditable(true);

        reset();
        {
            QValidator validator = new QValidator((QObject)null) {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }

                @Override
                public State validate(QValidationData arg__1) {
                    // TODO Auto-generated method stub
                    return null;
                }
            };

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

    @Test public void testQComboBoxLineEditSetValidator() {
        final QComboBox box = new QComboBox();
        box.setEditable(true);

        reset();
        {
            QValidator validator = new QValidator((QObject)null) {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }

                @Override
                public State validate(QValidationData arg__1) {
                    // TODO Auto-generated method stub
                    return null;
                }
            };

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

    @Test public void testQButtonGroupAddButton() {
        QButtonGroup group = new QButtonGroup();

        reset();
        for (int i=0; i<COUNT; ++i) {
            group.addButton(new QPushButton("button" + i) {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }
            });
            gc();
        }

        assertEquals(COUNT, group.buttons().size());
        assertEquals(0, deleted);
        assertEquals("button" + COUNT / 2, group.buttons().get(COUNT / 2).text());

    }

    @Test public void testQButtonGroupAddButtonId() {
        QButtonGroup group = new QButtonGroup();

        reset();
        for (int i=0; i<COUNT; ++i) {
            group.addButton(new QPushButton("button" + i) {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }
            }, i);
            gc();
        }

        assertEquals(COUNT, group.buttons().size());
        assertEquals(0, deleted);
        assertEquals(COUNT / 2, group.id(group.buttons().get(COUNT / 2)));
        assertEquals("button" + COUNT / 2, group.buttons().get(COUNT / 2).text());
    }

    @Test public void testQButtonGroupRemoveButton() {
        final QButtonGroup group = new QButtonGroup();

        reset();
        for (int i=0; i<COUNT; ++i) {
            QPushButton button = new QPushButton("button" + i) {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }
            };
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

    @Test public void testQAbstractItemViewSetItemDelegate() {
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

                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }

            };

            view.setItemDelegate(delegate);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertTrue(view.itemDelegate() != null);
        assertEquals(0, deleted);

    }

    @Test public void testQAbstractItemViewSetItemDelegateNull() {
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

                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }

            };

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

    @Test public void testQAbstractItemViewSetModel() {
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
            QStandardItemModel model = new QStandardItemModel() {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }
            };

            view.setModel(model);
        }

        long millis = System.currentTimeMillis();
        while (System.currentTimeMillis() - millis < 1000)
            gc();

        assertTrue(view.model() != null);
        assertEquals(0, deleted);
    }

    @Test public void testQAbstractItemViewSetSelectionModelThenSetModel() {
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
            QItemSelectionModel model = new QItemSelectionModel(view.model()) {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }
            };

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

    @Test public void testQAbstractItemViewSetSelectionModel() {
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

            QItemSelectionModel model = new QItemSelectionModel(view.model()) {
                @Override
                public void disposed() {
                    synchronized(TestReferenceCounting.class) {
                        deleted++;
                    }
                }
            };

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
        }

        @Override
        public void paint(QPainter painter, QRect rect, Mode mode, org.qtjambi.qt.gui.QIcon.State state) {

        }

        @Override
        public QIconEngine clone() {
            return new MyIconEngine(i);
        }

        @Override
        protected void disposed() {
            deletedEngines++;
        }

    }

    private static class MyIcon extends QIcon {

        public MyIcon(int i) {
            super(new MyIconEngine(i));
        }
    }

    @Test public void testIconEngine() {
        QListWidget w = new QListWidget();

        deletedEngines = 0;

        for (int i=0; i<100; ++i) {
            w.addItem(new QListWidgetItem(new MyIcon(i), "" + i));
            gc();
        }

        // Don't crash
        w.show();
        assertEquals(0, deletedEngines);
    }

    @Test
    public void setItemDelegateForRowToNull() {
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
    public void setItemDelegateForColumnToNull() {
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
    public void setWidgetMappingToNull() {
        QGuiSignalMapper mapper = new QGuiSignalMapper();

        boolean caughtException = false;
        try {
            mapper.setMapping(new QObject(), (QWidget) null);
        } catch (NullPointerException e) {
            caughtException = true;
        }

        assertFalse(caughtException);
    }

    @Test
    public void setObjectMappingToNull() {
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
