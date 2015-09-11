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
import static org.junit.Assert.assertTrue;

import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.List;

import org.junit.After;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import org.qtjambi.autotests.generated.OrdinaryDestroyed;
import org.qtjambi.autotests.generated.QObjectDestroyed;
import org.qtjambi.autotests.generated.GraphicsSceneSubclass;
import org.qtjambi.autotests.generated.SpinBoxHandler;
import org.qtjambi.autotests.generated.OrdinarySuperclass;
import org.qtjambi.autotests.generated.TestDialog;
import org.qtjambi.autotests.generated.SpinBoxSubclass;
import org.qtjambi.autotests.generated.General;
import org.qtjambi.qt.QNoImplementationException;
import org.qtjambi.qt.QSignalEmitter;
import org.qtjambi.qt.core.QAbstractFileEngine;
import org.qtjambi.qt.core.QByteArray;
import org.qtjambi.qt.core.QCoreApplication;
import org.qtjambi.qt.core.QDate;
import org.qtjambi.qt.core.QEvent;
import org.qtjambi.qt.core.QEventLoop;
import org.qtjambi.qt.core.QFileInfo;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.core.QRect;
import org.qtjambi.qt.core.QRectF;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.QTime;
import org.qtjambi.qt.core.QTimer;
import org.qtjambi.qt.core.QUuid;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.widgets.*;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.gui.QKeySequence;
import org.qtjambi.qt.gui.QPaintEvent;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPen;
import org.qtjambi.qt.gui.QPixmap;
import org.qtjambi.qt.gui.QResizeEvent;
import org.qtjambi.qt.gui.QValidator;
import org.qtjambi.qt.internal.QSignalEmitterInternal;
import org.qtjambi.qt.internal.QtJambiInternal;
import org.qtjambi.qt.internal.QtJambiObject;
import org.qtjambi.qt.network.QHostAddress;
import org.qtjambi.qt.osinfo.OSInfo;

class OrdinarySubclass extends OrdinaryDestroyed {
    private TestClassFunctionality tc = null;

    public OrdinarySubclass(TestClassFunctionality tc) {
        this.tc = tc;
    }

    @Override
    protected void disposed() {
        synchronized(tc) {
            tc.disposed++;
        }
    }
}

class QObjectSubclass extends QObjectDestroyed {
    private TestClassFunctionality tc = null;

    public QObjectSubclass(QObject parent, TestClassFunctionality tc) {
        super(parent);

        this.tc = tc;
    }

    @Override
    protected void disposed() {
        synchronized(tc) {
            tc.disposed++;
        }
    }
}

class GeneralObject {
    public String data;
}

class CustomEvent extends QEvent {
    @Override
	protected void disposed() {
    	System.out.println("#"+Integer.toHexString(System.identityHashCode(this))+": CustomEvent.disposed() #"+Long.toHexString(nativeId()));
		super.disposed();
	}

	@Override
	protected void finalize() {
		System.out.println("#"+Integer.toHexString(System.identityHashCode(this))+": CustomEvent.finalize()");
		super.finalize();
	}

	public String s;

    public CustomEvent(String param) {
        super(QEvent.Type.resolve(QEvent.Type.User.value() + 16));
        s = param;
    }
}

class EventReceiver extends QWidget {
    public String myString = null;
    public String customEventString = null;
    public QSize resizeEventSize = null;
    public QSize resizeEventOldSize = null;
    public QEvent.Type customEventType;
    public QEvent.Type resizeEventType;
    public QEvent.Type paintEventType;
    public boolean paintEventCastWorked = false;
    public boolean paintRectMatched = false;

    public EventReceiver(QWidget parent, String str) {
        super(parent);
        myString = str;
    }

    @Override
    public boolean event(QEvent event) {
        if (event instanceof QResizeEvent) {
            QResizeEvent rs = (QResizeEvent) event;
            resizeEventType = event.type();
            resizeEventSize = rs.size();
            resizeEventOldSize = rs.oldSize();
        } else if (event instanceof CustomEvent) {
            CustomEvent ce = (CustomEvent) event;
            customEventType = event.type();
            customEventString = ce.s;
        } else if (event.type() == QEvent.Type.Paint) {
            QtJambiObject new_event = event; /*null;
            try {
                new_event = QtJambiObject.reassignNativeResources(event, QPaintEvent.class);
            } catch (Exception e) {
                throw new RuntimeException(e);
            }*/

            if (new_event != null && new_event instanceof QPaintEvent) {
                paintEventCastWorked = true;
                QPaintEvent paintEvent = (QPaintEvent) new_event;
                paintEventType = paintEvent.type();
                QRect paintRect = paintEvent.rect();
                paintRectMatched = paintRect.width() == width();
                paintRectMatched = paintRectMatched && paintRect.height() == height();
            }
        }

        return false;
    }
}

public class TestClassFunctionality extends QApplicationTest {
    @BeforeClass
    public static void testInitialize() throws Exception {
        String args[] = new String[3];
        args[0] = "A";
        args[1] = "B";
        args[2] = "C";
        QApplicationTest.testInitialize(args);
    }

    @Before
    public void setUp() {
        QApplication.processEvents();
    }

    @After
    public void tearDown() {
        QApplication.processEvents();

        System.gc();
        System.runFinalization();
        if(Utils.releaseNativeResources() > 0) {
            System.gc();
            System.runFinalization();
        }
    }

    static class GraphicsSceneSubclassSubclass extends GraphicsSceneSubclass {
        public static QGraphicsItemInterface items[];
        public static QStyleOptionGraphicsItem options[];

        @Override
        protected void drawItems(QPainter painter, QGraphicsItemInterface[] items, QStyleOptionGraphicsItem[] options, QWidget widget) {
            GraphicsSceneSubclassSubclass.items = items;
            GraphicsSceneSubclassSubclass.options = options;

            options[1].setLevelOfDetail(3.0);
            super.drawItems(painter, items, options, widget);
        }

        public static void tearDown() {
            items = null;
            options = null;
        }
    }

    @Test
    public void testGraphicsSceneDrawItemsInjections() {
        GraphicsSceneSubclassSubclass gsss = new GraphicsSceneSubclassSubclass();
        QGraphicsEllipseItem item1 = new QGraphicsEllipseItem(new QRectF(1.0, 2.0, 3.0, 4.0));
        item1.setZValue(2.0);
        item1.setPen(new QPen(Qt.PenStyle.NoPen));
        QGraphicsEllipseItem item2 = new QGraphicsEllipseItem(new QRectF(2.0, 3.0, 4.0, 5.0));
        item2.setZValue(1.0);
        item2.setPen(new QPen(Qt.PenStyle.NoPen));
        gsss.addItem(item1);
        gsss.addItem(item2);

        QGraphicsView view = new QGraphicsView();
        view.setScene(gsss);
        view.setOptimizationFlag(QGraphicsView.OptimizationFlag.IndirectPainting);
        view.show();

	long t = System.currentTimeMillis();
	while (t + 5000 > System.currentTimeMillis()) {
	    // For some reason when we compile with ECJ this loop times out and then
	    //  fails the: assertTrue(GraphicsSceneSubclassSubclass.items != null);
	    // I tried again without using synchronized() here but am still seeing
	    //   occasional random failures in this test.  So putting lock back in place.
	    // Upped timeout from 2500 to 5000
	    synchronized(GraphicsSceneSubclassSubclass.class) {
                if(GraphicsSceneSubclassSubclass.items != null)
	            break;
            }
	    QApplication.processEvents();
        }

        assertTrue(GraphicsSceneSubclassSubclass.items != null);
        assertTrue(GraphicsSceneSubclassSubclass.options != null);
        assertEquals(2, GraphicsSceneSubclassSubclass.items.length);
        assertEquals(2, GraphicsSceneSubclassSubclass.options.length);
        assertTrue(GraphicsSceneSubclassSubclass.items[0] == item2);
        assertTrue(GraphicsSceneSubclassSubclass.items[1] == item1);

        QRectF brect = GraphicsSceneSubclassSubclass.items[0].boundingRect();
        assertEquals(2.0, brect.left(), 0.0001);
        assertEquals(3.0, brect.top(), 0.0001);
        assertEquals(4.0, brect.width(), 0.0001);
        assertEquals(5.0, brect.height(), 0.0001);

        brect = GraphicsSceneSubclassSubclass.items[1].boundingRect();
        assertEquals(1.0, brect.left(), 0.0001);
        assertEquals(2.0, brect.top(), 0.0001);
        assertEquals(3.0, brect.width(), 0.0001);
        assertEquals(4.0, brect.height(), 0.0001);

        assertEquals(2, gsss.numItems());

        brect = gsss.firstBoundingRect();
        assertEquals(2.0, brect.left(), 0.0001);
        assertEquals(3.0, brect.top(), 0.0001);
        assertEquals(4.0, brect.width(), 0.0001);
        assertEquals(5.0, brect.height(), 0.0001);

        brect = gsss.secondBoundingRect();
        assertEquals(1.0, brect.left(), 0.0001);
        assertEquals(2.0, brect.top(), 0.0001);
        assertEquals(3.0, brect.width(), 0.0001);
        assertEquals(4.0, brect.height(), 0.0001);

        assertTrue(gsss.firstItem() == item2);
        assertTrue(gsss.secondItem() == item1);

        assertEquals(QStyleOption.OptionType.SO_GraphicsItem.value(), gsss.firstStyleOptionType());
        assertEquals(QStyleOption.OptionType.SO_GraphicsItem.value(), gsss.secondStyleOptionType());
        assertEquals(QStyleOptionGraphicsItem.StyleOptionVersion.Version.value(), gsss.firstStyleOptionVersion());
        assertEquals(QStyleOptionGraphicsItem.StyleOptionVersion.Version.value(), gsss.secondStyleOptionVersion());

        QStyleOption option = gsss.firstStyleOption();
        assertTrue(option instanceof QStyleOptionGraphicsItem);
        assertEquals(((QStyleOptionGraphicsItem) option).levelOfDetail(), 1.0, 0.0001);

        option = gsss.secondStyleOption();
        assertTrue(option instanceof QStyleOptionGraphicsItem);
        assertEquals(((QStyleOptionGraphicsItem) option).levelOfDetail(), 3.0, 0.0001);

        GraphicsSceneSubclassSubclass.tearDown();
    }

    static class TestQObject extends QObject {
        private Signal0 a = new Signal0();

        public boolean slot_called = false;

        public boolean signalIsNull() {
            return a == null;
        }

        public boolean signalIsEqualTo(AbstractSignal signal) {
            return a == signal;
        }

        private void slot() {
            slot_called = true;
        }
    }

    @Test
    public void testEquals()
    {
        QHostAddress address1 = new QHostAddress(QHostAddress.SpecialAddress.LocalHost);
        QHostAddress address2 = new QHostAddress(QHostAddress.SpecialAddress.LocalHost);
        QHostAddress address3 = new QHostAddress(QHostAddress.SpecialAddress.Broadcast);
        QByteArray array = new QByteArray("127.0.0.1");

        assertFalse(address1 == address2);
        assertFalse(address2 == address3);
        assertTrue(address1.equals(address2));
        assertTrue(address2.equals(address1));
        assertTrue(address3.equals(address3));
        assertEquals(false, address1.equals(address3));
        assertFalse(address2.equals(array));
    }

    @Test
    public void testHashCodeAndEquals()
    {
        Hashtable<QHostAddress, QByteArray> address_hash = new Hashtable<QHostAddress, QByteArray>();

        QHostAddress address1 = new QHostAddress(QHostAddress.SpecialAddress.LocalHost);
        QHostAddress address2 = new QHostAddress(QHostAddress.SpecialAddress.LocalHost);
        QHostAddress address3 = new QHostAddress(QHostAddress.SpecialAddress.Broadcast);

        QByteArray ba_address1 = new QByteArray("127.0.0.1 - 1");
        QByteArray ba_address2 = new QByteArray("127.0.0.1 - 2");
        QByteArray ba_address3 = new QByteArray("255.255.255.255");

        address_hash.put(address1, ba_address1);
        assertFalse(address_hash.containsKey(new QHostAddress(QHostAddress.SpecialAddress.Broadcast)));
        assertTrue(address_hash.containsKey(new QHostAddress(QHostAddress.SpecialAddress.LocalHost)));
        assertTrue(address_hash.get(new QHostAddress(QHostAddress.SpecialAddress.LocalHost)) == ba_address1);

        address_hash.put(address2, ba_address2); // overwrites the first entry of this type
        address_hash.put(address3, ba_address3);
        assertTrue(address_hash.containsKey(address1));
        assertTrue(address_hash.containsKey(new QHostAddress(QHostAddress.SpecialAddress.Broadcast)));

        QHostAddress lookup_key1 = new QHostAddress(QHostAddress.SpecialAddress.LocalHost);
        QHostAddress lookup_key2 = new QHostAddress(QHostAddress.SpecialAddress.Broadcast);

        QByteArray value = address_hash.get(lookup_key1);
        assertTrue(value == ba_address2);

        value = address_hash.get(lookup_key2);
        assertTrue(value == ba_address3);
    }

    @Test
    public void testToString()
    {
        QByteArray ba = new QByteArray("Pretty flowers æøå");
        assertEquals("Pretty flowers æøå", ba.toString());
    }

    @Test
    public void run_testCallQtJambiInternalNativeFunctions() {

        Field field = null;
        try {
            field = TestQObject.class.getDeclaredField("a");
        } catch (NoSuchFieldException e) {
            e.printStackTrace();
            assertEquals(e, null);
        }
        assertTrue(field != null);

        Method method = null;
        try {
            method = QtJambiInternal.class.getDeclaredMethod("fetchSignal", QSignalEmitterInternal.class, Field.class);
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
            assertEquals(e, null);
        }

        TestQObject test_qobject = new TestQObject();
        try {
            method.setAccessible(true);
            QSignalEmitter.AbstractSignal signal = (QObject.AbstractSignal) method.invoke(null, test_qobject, field);
            assertTrue(test_qobject.signalIsEqualTo(signal));
        } catch (Exception e) {
            e.printStackTrace();
            assertEquals(e, null);
        }

        long method_long = 0;
        try {
            method = QtJambiInternal.class.getDeclaredMethod("resolveSlot", Method.class);
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
            assertEquals(e, null);
        }

        try {
            Method slotMethod = TestQObject.class.getDeclaredMethod("slot", (Class[]) null);
            method.setAccessible(true);
            method_long = (Long) method.invoke(null, slotMethod);
            assertTrue(method_long != 0);
        } catch (Exception e) {
            e.printStackTrace();
            assertEquals(e, null);
        }

        try {
            method = QtJambiInternal.class.getDeclaredMethod("invokeSlot", Object.class, Long.TYPE, Byte.TYPE, Object[].class, int[].class);
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
            assertEquals(e, null);
        }

        assertEquals(test_qobject.slot_called, false);
        try {
            method.setAccessible(true);
            method.invoke(null, test_qobject, method_long, (byte) 'V', new Object[] {}, new int[] {});
        } catch (Exception e) {
            e.printStackTrace();
            assertEquals(e, null);
        }

        assertEquals(test_qobject.slot_called, true);
    }

    public int disposed = 0;

    @Test
    public void run_testDestruction() {
        // Delete from Java
        {
            disposed = 0;
            {
                new OrdinarySubclass(this);
            }
            System.gc();
            System.runFinalization();
            try {
                for(int i = 0; i < 60; i++) {  
                    synchronized(this) {
                        if(disposed != 0)
                            break;
                    }
                    Thread.sleep(10);
                }
            } catch(Exception e) {
                e.printStackTrace();
            }

            assertEquals(disposed, 1);
        }

        // Delete from C++
        {
            disposed = 0;
            OrdinarySubclass sc = new OrdinarySubclass(this);
            OrdinaryDestroyed.deleteFromCpp(sc);
            assertEquals(disposed, 1);
            assertEquals(sc.nativeId(), 0L);
        }

        // Delete through virtual destructor
        {
            disposed = 0;
            OrdinarySuperclass sc = new OrdinarySubclass(this);
            OrdinaryDestroyed.deleteFromCppOther(sc);
            assertEquals(disposed, 1);
            assertEquals(sc.nativeId(), 0L);
        }

        // Delete QObject from Java
        {
            disposed = 0;
            QObjectSubclass qobject = new QObjectSubclass(null, this);
            qobject.dispose();
            assertEquals(disposed, 1);
            assertEquals(qobject.nativeId(), 0L);
        }

        // Delete QObject from parent
        {
            disposed = 0;
            QObject parent = new QObject();
            QObject qobject = new QObjectSubclass(parent, this);
            parent.dispose();
            assertEquals(disposed, 1);
            assertEquals(qobject.nativeId(), 0L);
        }

        // Delete QObject later
        {
            disposed = 0;
            QObject qobject = new QObjectSubclass(null, this);
            qobject.disposeLater();
            QApplication.sendPostedEvents(null, QEvent.Type.DeferredDelete.value());

            assertEquals(disposed, 1);
            assertEquals(qobject.nativeId(), 0L);
        }

        // Delete QObject from C++
        {
            disposed = 0;
            QObjectSubclass qobject = new QObjectSubclass(null, this);
            QObjectDestroyed.deleteFromCpp(qobject);
            assertEquals(disposed, 1);
            assertEquals(qobject.nativeId(), 0L);
        }

        // Delete QObject from C++ through virtual destructor
        {
            disposed = 0;
            QObject qobject = new QObjectSubclass(null, this);
            QObjectDestroyed.deleteFromCppOther(qobject);
            assertEquals(disposed, 1);
            assertEquals(qobject.nativeId(), 0L);
        }
    }

    /**
     * Test that calling a private virtual function gives you an exception
     */
    @SuppressWarnings("deprecation")
    @Test
    public void run_callPrivateVirtualFunction() {
        QTableWidget w = new QTableWidget();

        boolean gotException = false;
        try {
            w.setModel(null);
        } catch (QNoImplementationException e) {
            // This test checks that this exception is thrown (for a PASS)
            gotException = true;
        }

        assertTrue(gotException);
    }

    /*-------------------------------------------------------------------------
     * Test that QObject.sender() returns something valid during
     * a signal emittion...
     */
    private static class SenderTester extends QObject {
        Signal0 signal = new Signal0();

        public boolean is_null, is_valid;

        public QTime timeouted;

        public long msec = 0L;

        public SenderTester() {
            timeouted = new QTime();
            timeouted.start();
        }

        public void checkSender() {
            is_null = org.qtjambi.qt.internal.QtJambiInternal.sender(this) == null;
            is_valid = org.qtjambi.qt.internal.QtJambiInternal.sender(this) == this;
        }

        public void emitSignal() {
            signal.emit();
        }

        void timeoutSlot() {
            msec = timeouted.elapsed();
        }
    }

    @Test
    public void run_senderNotNull() {
        SenderTester tester = new SenderTester();
        tester.signal.connect(tester, "checkSender()");

        tester.emitSignal();

        assertTrue(!tester.is_null);
        assertTrue(tester.is_valid);
    }

    @Test
    public void run_cppAndJavaObjects() {
        CustomEvent event1 = new CustomEvent("this is my stuff");
        QResizeEvent event2 = new QResizeEvent(new QSize(101, 102), new QSize(103, 104));

        QWidget parentWidget = new QWidget(null);
        EventReceiver someQObject = new EventReceiver(parentWidget, "some stuff");

        List<QObject> children = parentWidget.children();
        assertEquals(children.size(), 1);

        QObject child = children.get(0);
        assertTrue(child != null);
        assertTrue(child instanceof EventReceiver);
        assertTrue(someQObject == child);

        EventReceiver receiver = (EventReceiver) child;
        assertTrue(someQObject == receiver);
        assertEquals(receiver.myString, "some stuff");
        assertTrue(receiver.parent() == parentWidget);

        QApplication.postEvent(receiver, event1);
        QApplication.postEvent(someQObject, event2);

        // This should still be false (as we've not processed posted events yet or issues show())
        assertFalse(receiver.paintEventCastWorked);
        parentWidget.show();
        // CHECKME: This ends up still being false on Linux/Windows.
        // MacOSX: This is already true here (not the difference between platforms), show() caused it to happen
        if(OSInfo.isMacOS() == false)
            assertFalse(receiver.paintEventCastWorked);
        // ensure this gets called at least once (important on MacOSX, otherwise loop will not cause it)
        QApplication.processEvents();

	long t = System.currentTimeMillis();
	while (t + 10000 > System.currentTimeMillis()) {
	    // For some reason when we compile with ECJ this loop times out and then
	    //  fails the: assertTrue(receiver.paintEventCastWorked);
	    // I tried again without using synchronized() here but am still seeing
	    //   occasional random failures in this test.  So putting lock back in place.
	    // Upped timeout from 2500 to 5000 then to 10000 for ARM Raspberry Pi
            synchronized(receiver) {
                // TODO: Rework this check, put a flag on everything that needs to happen, each custom event and the paint/show events.
                if(receiver.paintEventCastWorked)
                    break;
            }
	    QApplication.processEvents();
        }
        assertEquals(receiver.customEventString, "this is my stuff");
        assertEquals(receiver.customEventType, QEvent.Type.resolve(QEvent.Type.User.value() + 16));
        assertEquals(receiver.resizeEventType, QEvent.Type.Resize);
        assertEquals(receiver.resizeEventSize.width(), 101);
        assertEquals(receiver.resizeEventSize.height(), 102);
        assertEquals(receiver.resizeEventOldSize.width(), 103);
        assertEquals(receiver.resizeEventOldSize.height(), 104);
        assertTrue(receiver.paintEventCastWorked);
        assertTrue(receiver.paintRectMatched);
        assertEquals(receiver.paintEventType, QEvent.Type.Paint);

        QFileDialog d = new QFileDialog();

        QWidget some_widget = new QWidget();
        {
            List<QAction> actions = new ArrayList<QAction>();
            actions.add(new QAction("bite", d));
            actions.add(new QAction("me", d));

            some_widget.addActions(actions);
        }

        {
            List<QAction> read_back = some_widget.actions();

            assertEquals(read_back.size(), 2);
            assertEquals(read_back.get(0).text(), "bite");
            assertEquals(read_back.get(1).text(), "me");
        }
        some_widget.dispose();
    }

    @Test
    public void testDialog() {
        String[] childrenClassList = {"QGridLayout","QLabel","QComboBox","QLabel","QComboBox","QLabel","QComboBox","QPushButton","QTableWidget","QLabel", "QPushButton"};

        TestDialog dialog = new TestDialog();
        dialog.show();
        int i = 0;
        for (QObject o : dialog.children()) {
            assertTrue(o.getClass().getName().endsWith("." + childrenClassList[i]));
            i++;
        }
        dialog.hide();
    }

    @Test
    public void run_injectedCode() {
        QObject obj = new QObject();
        QAction act = new QAction(obj);
        act.setShortcut("Ctrl+A");
        QKeySequence seq = act.shortcut();

        assertEquals(seq.count(), 1);
        assertEquals(seq.at(0), Qt.KeyboardModifier.ControlModifier.value() | Qt.Key.Key_A.value());

        SenderTester tester = new SenderTester();
        QTimer.singleShot(1000, tester, "timeoutSlot()");

        System.gc();
        System.runFinalization();

        try {
            while (tester.timeouted.elapsed() < 1500) {
                QApplication.processEvents();
                // We can sleep a bit here
                Thread.sleep(50);
            }
        } catch (Exception e) {
            e.printStackTrace();
            assertTrue(false);
        }

        assertTrue("tester.msec >= 1000 took " + tester.msec, tester.msec >= 1000);
        assertTrue("tester.msec <= 1500 took " + tester.msec, tester.msec <= 1500);
    }

    @Test
    public void run_copyConstructor() {
        QAbstractFileEngine.addSearchPathForResourceEngine(".");
        QFileInfo file1 = new QFileInfo("classpath:org/qtjambi/autotests/TestClassFunctionality.jar");
        assertTrue(file1.exists());

        QFileInfo file2 = new QFileInfo(file1);

        assertEquals(file2.size(), file1.size());
        assertEquals(file2.exists(), file1.exists());
        assertEquals(file2.fileName(), file1.fileName());
    }

    @Test
    public void run_settersAndGetters() {
        QStyleOptionButton so = new QStyleOptionButton();

        assertTrue(so.icon().isNull());

        QPixmap pm = new QPixmap(100, 100);
        pm.fill(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.red));
        QIcon icon = new QIcon(pm);
        so.setIcon(icon);
        so.setText("A travelling salesman walks into a bar");

        assertEquals(so.text(), "A travelling salesman walks into a bar");
        assertTrue(!so.icon().isNull());

        so.setIcon(new QIcon());
        assertTrue(so.icon().isNull());

        so.setText("A priest and a rabbi sitting in a bar");
        assertEquals(so.text(), "A priest and a rabbi sitting in a bar");

        QUuid uuid = new QUuid();

        byte b_orig[] = new byte[8];
        b_orig[0] = 2;
        b_orig[1] = 3;
        b_orig[3] = 5;
        b_orig[4] = 7;
        b_orig[5] = 11;
        b_orig[6] = 13;
        b_orig[7] = 17;
        uuid.setData4(b_orig);

        byte b2[] = uuid.data4();
        assertEquals(b2.length, 8);
        for (int i = 0; i < 8; ++i)
            assertEquals(b2[i], b_orig[i]);

        // Wrong number of entries
        Exception fe = null;
        try {
            byte b[] = new byte[6];
            uuid.setData4(b);
        } catch (Exception e) {
            // java.lang.IllegalArgumentException: Wrong number of elements in array. Found: 6, expected: 8
            fe = e;
        }

        assertTrue(fe != null);
        assertTrue(fe instanceof IllegalArgumentException);

        // Make sure it wasn't set after all
        b2 = uuid.data4();
        assertEquals(b2.length, 8);
        for (int i = 0; i < 8; ++i)
            assertEquals(b2[i], b_orig[i]);

        fe = null;
        try {
            byte b[] = new byte[100];
            uuid.setData4(b);
        } catch (Exception e) {
            // java.lang.IllegalArgumentException: Wrong number of elements in array. Found: 100, expected: 8
            fe = e;
        }

        assertTrue(fe != null);
        assertTrue(fe instanceof IllegalArgumentException);
    }

    static class MySpinBox extends QDoubleSpinBox {

        public String receivedString;
        public int receivedPos;

        @Override
        public String fixup(String input) {
            receivedString = input;
            return "As aught of " + input.substring(2, 8) + " birth";
        }

        @Override
        public QValidator.State validate(QValidator.QValidationData data)
        {
            receivedString = data.string;
            receivedPos = data.position;

            data.string = "The " + data.string.substring(9, 13) + " where Death has set his seal";
            data.position += 13;

            return QValidator.State.Acceptable;
        }


    }

    @Test public void testVirtualCallToFixup() {
        MySpinBox spinBox = new MySpinBox();
        SpinBoxHandler handler = new SpinBoxHandler();

        handler.tryFixup(spinBox, "Immortal love, forever full");

        assertEquals("Immortal love, forever full", spinBox.receivedString);
        assertEquals("As aught of mortal birth", handler.my_returned_string());
    }

    @Test public void testVirtualCallToValidate() {
        MySpinBox spinBox = new MySpinBox();
        SpinBoxHandler handler = new SpinBoxHandler();

        handler.tryValidate(spinBox, "Immortal love, forever full", 15);
        assertEquals("Immortal love, forever full", spinBox.receivedString);
        assertEquals(15, spinBox.receivedPos);
        assertEquals("The love where Death has set his seal", handler.my_returned_string());
        assertEquals(28, handler.my_returned_pos());
        assertEquals(QValidator.State.Acceptable, handler.my_returned_state());
    }

    @Test public void testFinalCallToFixup() {
        SpinBoxSubclass sbs = new SpinBoxSubclass();

        String returned = sbs.fixup("Thou dost hang canary birds in parlour windows");
        assertEquals("And Thou art dead", returned);
        assertEquals("Thou dost hang canary birds in parlour windows", sbs.my_received_string());
    }

    @Test public void testFinalCallToValidate() {
        SpinBoxSubclass sbs = new SpinBoxSubclass();

        QValidator.QValidationData data = new QValidator.QValidationData("dream and you have a sloppy body from being brought to bed of crocuses", 14);
        QValidator.State returned = sbs.validate(data);

        assertEquals(QValidator.State.Intermediate, returned);
        assertEquals("dream and you have a sloppy body from being brought to bed of crocuses", sbs.my_received_string());
        assertEquals(14, sbs.my_received_pos());
        assertEquals("The silence of that dreamless sleep", data.string);
        assertEquals(27, data.position);
    }

    /*
     * Tests for QInvokable and QCoreApplication.invokeLater(Runnable);
     */

    /**
     * The run() function sets the executing thread.
     */
    private static class Invokable implements Runnable {
        public void run() {
            thread = Thread.currentThread();
        }

        public boolean wasRun() {
            return thread != null;
        }

        public Thread thread;
    }

    public QPainter painterReference;

    @Test
    public void resetAfterUseTemporary() {
        painterReference = null;

        QCalendarWidget w = new QCalendarWidget() {
            @Override
            public void paintCell(QPainter painter, QRect rect, QDate date) {
                painterReference = painter;
            }
        };

        // painter == null passes a temporary C++ painter
        General.callPaintCell(w, null);

        assertTrue(painterReference != null);
        assertEquals(0, painterReference.nativeId());
    }

    @Test
    public void resetAfterUseNonTemporary() {
        painterReference = null;

        QCalendarWidget w = new QCalendarWidget() {
            @Override
            public void paintCell(QPainter painter, QRect rect, QDate date) {
                painterReference = painter;
            }
        };

        QPainter p = new QPainter();
        General.callPaintCell(w, p);

        assertTrue(painterReference != null);
        assertTrue(0 != painterReference.nativeId());
        assertEquals(p, painterReference);
    }

    @Test
    public void resetAfterUseNull() {
        painterReference = new QPainter();

        QCalendarWidget w = new QCalendarWidget() {
            @Override
            public void paintCell(QPainter painter, QRect rect, QDate date) {
                painterReference = painter;
            }
        };

        General.callPaintCellNull(w);

        assertEquals(null, painterReference);
    }

    /**
     * Create an invokable object and post it. Then verify that its not executed
     * before after we call processEvents()
     */
    @Test
    public void run_invokeLater_mainThread() {
        Invokable invokable = new Invokable();
        QCoreApplication.invokeLater(invokable);

        assertTrue(!invokable.wasRun());
        QCoreApplication.processEvents();
        assertTrue(invokable.wasRun());
    }

    private static boolean invokeLater_in_otherThread;

    private static Invokable invokable_in_otherThread;

    /**
     * Same as the test above, except that the invokable is now created in a
     * different thread and we wait for this thread to finish before testing if
     * the invokable was run. We also in this case check that the invokable is
     * executed by the correct thread.
     */
    //FIXME @Test
    /*public void run_invokeLater_otherThread() {
        Thread thread = new Thread() {
            public void run() {
                invokable_in_otherThread = new Invokable();
                QApplication.invokeLater(invokable_in_otherThread);
                try {
                    Thread.sleep(500);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        };
        thread.start();
        try {
            thread.join();
        } catch (Exception e) {
            e.printStackTrace();
        }

        assertTrue(!invokable_in_otherThread.wasRun());
        QCoreApplication.processEvents();
        assertTrue(invokable_in_otherThread.wasRun());
        assertEquals(invokable_in_otherThread.thread, QCoreApplication.instance().thread());

        invokable_in_otherThread = null;  // tearDown() static
    }*/

    @Test
    public void invokeAndWaitSameThread() {
        Invokable i = new Invokable();
        QCoreApplication.invokeAndWait(i);
        assertTrue(i.wasRun());
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestClassFunctionality.class.getName());
    }
}
