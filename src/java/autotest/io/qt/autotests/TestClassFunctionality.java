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
import static org.junit.Assert.assertTrue;

import java.lang.ref.WeakReference;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Hashtable;
import java.util.List;
import java.util.Set;

import org.junit.After;
import org.junit.Assume;
import org.junit.Before;
import org.junit.Test;

import io.qt.QNoImplementationException;
import io.qt.autotests.generated.General;
import io.qt.autotests.generated.GraphicsSceneSubclass;
import io.qt.autotests.generated.OrdinaryDestroyed;
import io.qt.autotests.generated.OrdinarySuperclass;
import io.qt.autotests.generated.QObjectDestroyed;
import io.qt.autotests.generated.SpinBoxHandler;
import io.qt.autotests.generated.SpinBoxSubclass;
import io.qt.autotests.generated.TestDialog;
import io.qt.core.QByteArray;
import io.qt.core.QDate;
import io.qt.core.QElapsedTimer;
import io.qt.core.QEvent;
import io.qt.core.QFileInfo;
import io.qt.core.QList;
import io.qt.core.QMetaObject;
import io.qt.core.QObject;
import io.qt.core.QRect;
import io.qt.core.QRectF;
import io.qt.core.QSize;
import io.qt.core.QTimer;
import io.qt.core.QUuid;
import io.qt.core.Qt;
import io.qt.gui.QColor;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QIcon;
import io.qt.gui.QKeySequence;
import io.qt.gui.QPaintEvent;
import io.qt.gui.QPainter;
import io.qt.gui.QPen;
import io.qt.gui.QPixmap;
import io.qt.gui.QResizeEvent;
import io.qt.gui.QValidator;
import io.qt.internal.QtJambiInternal;
import io.qt.internal.QtJambiObject;
import io.qt.network.QHostAddress;
import io.qt.widgets.QAction;
import io.qt.widgets.QApplication;
import io.qt.widgets.QCalendarWidget;
import io.qt.widgets.QDoubleSpinBox;
import io.qt.widgets.QFileDialog;
import io.qt.widgets.QGraphicsEllipseItem;
import io.qt.widgets.QGraphicsItem;
import io.qt.widgets.QGraphicsView;
import io.qt.widgets.QStyleOption;
import io.qt.widgets.QStyleOptionButton;
import io.qt.widgets.QStyleOptionGraphicsItem;
import io.qt.widgets.QTableWidget;
import io.qt.widgets.QWidget;

class OrdinarySubclass extends OrdinaryDestroyed {
    public OrdinarySubclass(DisposeCounter destroyCounter) {
    	super(destroyCounter);
    	io.qt.QtUtilities.getSignalOnDispose(this).connect(destroyCounter::onDisposed, Qt.ConnectionType.DirectConnection);
    }
}

class QObjectSubclass extends QObjectDestroyed {
    public QObjectSubclass(DisposeCounter counter, QObject parent) {
        super(counter, parent);
        destroyed.connect(counter::onDisposed, Qt.ConnectionType.DirectConnection);
    }
}

class GeneralObject {
    public String data;
}

class CustomEvent extends QEvent {
	public String s;
    public CustomEvent(String param) {
        super(customType);
        s = param;
    }
    
    private static QEvent.Type customType = QEvent.Type.resolve(QEvent.registerEventType());
    
	public static QEvent.Type customType() {
		return customType;
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
    @Before
    public void setUp() {
        QApplication.processEvents();
    }

    @After
    public void tearDown() {
        QApplication.processEvents();

        System.gc();
        System.runFinalization();
    }

    static class GraphicsSceneSubclassSubclass extends GraphicsSceneSubclass {
        public static QGraphicsItem items[];
        public static QStyleOptionGraphicsItem options[];
        
        {
        	instances.add(new WeakReference<>(this));
        }

        @Override
        protected void drawItems(QPainter painter, QGraphicsItem[] items, QStyleOptionGraphicsItem[] options, QWidget widget) {
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
    	Assume.assumeThat(QGuiApplication.primaryScreen()!=null, QApplicationTest.trueMatcher("A screen is required to create a window."));
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
        instances.add(new WeakReference<>(view));

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
        
        assertTrue("item still alive", QtJambiInternal.nativeId(GraphicsSceneSubclassSubclass.items[0])!=0);
        assertTrue("item still alive", QtJambiInternal.nativeId(GraphicsSceneSubclassSubclass.items[1])!=0);

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

        assertEquals(15, gsss.firstStyleOptionType());
        assertEquals(15, gsss.secondStyleOptionType());
        assertEquals(1, gsss.firstStyleOptionVersion());
        assertEquals(1, gsss.secondStyleOptionVersion());

        QStyleOption option = gsss.firstStyleOption();
        assertTrue(option instanceof QStyleOptionGraphicsItem);
        assertEquals(((QStyleOptionGraphicsItem) option).levelOfDetail(), 1.0, 0.0001);

        option = gsss.secondStyleOption();
        assertTrue(option instanceof QStyleOptionGraphicsItem);
        assertEquals(((QStyleOptionGraphicsItem) option).levelOfDetail(), 3.0, 0.0001);

        GraphicsSceneSubclassSubclass.tearDown();
        view.hide();
    }

    static class TestQObject extends QObject {
        private Signal0 a = new Signal0();

        public boolean slot_called = false;

        public boolean signalIsNull() {
            return a == null;
        }

        public boolean signalIsEqualTo(QMetaObject.AbstractSignal signal) {
            return a == signal;
        }

        @SuppressWarnings("unused")
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
            method = QtJambiInternal.class.getDeclaredMethod("fetchSignal", Object.class, Field.class, boolean.class);
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
            assertEquals(e, null);
        }

        TestQObject test_qobject = new TestQObject();
        try {
            method.setAccessible(true);
            QMetaObject.AbstractSignal signal = (QMetaObject.AbstractSignal) method.invoke(null, test_qobject, field, false);
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
    
    @Test
    public void test_GC_versus_access_SplitOwnership() throws InterruptedException {
    	Set<Integer> hashes = new HashSet<>();
    	Thread thread = new Thread(()->{
			try {
	    		while(true) {
						System.gc();
						Thread.sleep(6);
	    		}
			} catch (InterruptedException e) {}
    	});
    	thread.setPriority(Thread.NORM_PRIORITY-1);
    	thread.start();
    	for (int i = 0; i < 500; i++) {
    		{
	    		QObject globalObject = OrdinaryDestroyed.getGlobalQObjectSplitOwnership();
	    		assertTrue(globalObject!=null);
	    		assertEquals(QtJambiInternal.Ownership.Split, QtJambiInternal.ownership(globalObject));
	    		hashes.add(System.identityHashCode(globalObject));
	    		globalObject = null;
    		}
    		Thread.sleep(5);
		}
    	assertTrue("Expect to create more than one java objects for global QObject with split ownership.", hashes.size()>1);
    	thread.interrupt();
    }

    @Test
    public void run_testDestruction() {
        // Delete from Java
        {
            DisposeCounter counter = new DisposeCounter();
            {
                new OrdinarySubclass(counter);
            }
            System.gc();
            System.runFinalization();
            try {
                for(int i = 0; i < 60; i++) {  
                    synchronized(this) {
                        if(counter.disposedCount() != 0)
                            break;
                    }
                    Thread.sleep(10);
                }
            } catch(Exception e) {
                e.printStackTrace();
            }

            assertEquals(1, counter.disposedCount());
        }

        // Delete from C++
        {
        	DisposeCounter counter = new DisposeCounter();
            OrdinarySubclass sc = new OrdinarySubclass(counter);
            OrdinaryDestroyed.deleteFromCpp(sc);
            assertEquals(1, counter.disposedCount());
            assertEquals(QtJambiInternal.nativeId(sc), 0L);
        }

        // Delete through virtual destructor
        {
        	DisposeCounter counter = new DisposeCounter();
            OrdinarySuperclass sc = new OrdinarySubclass(counter);
            OrdinaryDestroyed.deleteFromCppOther(sc);
            assertEquals(1, counter.disposedCount());
            assertEquals(QtJambiInternal.nativeId(sc), 0L);
        }

        // Delete QObject from Java
        {
            DisposeCounter counter = new DisposeCounter();
            QObjectSubclass qobject = new QObjectSubclass(counter, null);
            qobject.dispose();
            assertEquals(1, counter.disposedCount());
            assertEquals(QtJambiInternal.nativeId(qobject), 0L);
        }

        // Delete QObject from parent
        {
            DisposeCounter counter = new DisposeCounter();
            QObject parent = new QObject();
            QObject qobject = new QObjectSubclass(counter, parent);
            parent.dispose();
            assertEquals(1, counter.disposedCount());
            assertEquals(QtJambiInternal.nativeId(qobject), 0L);
        }

        // Delete QObject later
        {
            DisposeCounter counter = new DisposeCounter();
            QObject qobject = new QObjectSubclass(counter, null);
            qobject.disposeLater();
            QApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());

            assertEquals(1, counter.disposedCount());
            assertEquals(QtJambiInternal.nativeId(qobject), 0L);
        }

        // Delete QObject from C++
        {
            DisposeCounter counter = new DisposeCounter();
            QObjectSubclass qobject = new QObjectSubclass(counter, null);
            QObjectDestroyed.deleteFromCpp(qobject);
            assertEquals(1, counter.disposedCount());
            assertEquals(QtJambiInternal.nativeId(qobject), 0L);
        }

        // Delete QObject from C++ through virtual destructor
        {
            DisposeCounter counter = new DisposeCounter();
            QObject qobject = new QObjectSubclass(counter, null);
            QObjectDestroyed.deleteFromCppOther(qobject);
            assertEquals(1, counter.disposedCount());
            assertEquals(QtJambiInternal.nativeId(qobject), 0L);
        }
    }

    /**
     * Test that calling a private virtual function gives you an exception
     */
    @SuppressWarnings("deprecation")
    @Test
    public void run_callPrivateVirtualFunction() {
    	Assume.assumeThat(QGuiApplication.primaryScreen()!=null, QApplicationTest.trueMatcher("A screen is required to create a window."));
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

        public QElapsedTimer timeouted;

        public long msec = 0L;

        public SenderTester() {
            timeouted = new QElapsedTimer();
            timeouted.start();
        }

        public void checkSender() {
            is_null = this.sender() == null;
            is_valid = this.sender() == this;
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
        tester.signal.connect(tester::checkSender);

        tester.emitSignal();

        assertTrue(!tester.is_null);
        assertTrue(tester.is_valid);
    }

    @Test
    public void run_cppAndJavaObjects() {
    	Assume.assumeThat(QGuiApplication.primaryScreen()!=null, QApplicationTest.trueMatcher("A screen is required to create a window."));
        CustomEvent event1 = new CustomEvent("this is my stuff");
        QResizeEvent event2 = new QResizeEvent(new QSize(101, 102), new QSize(103, 104));

        QWidget parentWidget = new QWidget(null);
        EventReceiver someQObject = new EventReceiver(parentWidget, "some stuff");
        someQObject.setObjectName("run_cppAndJavaObjects");

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
        try{
	        // CHECKME: This ends up still being false on Linux/Windows.
	        // MacOSX: This is already true here (not the difference between platforms), show() caused it to happen
/*	        if(OSInfo.isMacOS() == false)
	            assertFalse(receiver.paintEventCastWorked);*/
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
	        assertEquals(receiver.customEventType, CustomEvent.customType());
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
	
	        try{
	            List<QAction> read_back = some_widget.actions();
	
	            assertEquals(read_back.size(), 2);
	            assertEquals(read_back.get(0).text(), "bite");
	            assertEquals(read_back.get(1).text(), "me");
	        }finally{
	        	some_widget.dispose();
	        	d.dispose();
	        }
        }finally {
        	parentWidget.hide();
        	parentWidget.dispose();
        }
    }

    @Test
    public void testDialog() {
    	Assume.assumeThat(QGuiApplication.primaryScreen()!=null, QApplicationTest.trueMatcher("A screen is required to create a window."));
        String[] childrenClassList = {".QGridLayout",".QLabel",".QComboBox",".QLabel",".QComboBox",".QLabel",".QComboBox",".QPushButton",".QTableWidget",".QLabel", ".QPushButton"};

        TestDialog dialog = new TestDialog();
        dialog.show();
        int i = 0;
        QList<QObject> children = dialog.children();
        for (QObject o : children) {
    		assertTrue("class at index "+i+" unexpected: "+o.getClass().getName(), o.getClass().getName().endsWith(childrenClassList[i]));
            ++i;
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
        QTimer.singleShot(1000, tester::timeoutSlot);

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
        io.qt.QtResources.addSearchPath(".");
        QFileInfo file1 = new QFileInfo("classpath:io/qt/autotests/TestClassFunctionality.jar");
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
        pm.fill(new QColor(io.qt.core.Qt.GlobalColor.red));
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

    @Test 
    public void testVirtualCallToFixup() {
    	Assume.assumeThat(QGuiApplication.primaryScreen()!=null, QApplicationTest.trueMatcher("A screen is required to create a window."));
        MySpinBox spinBox = new MySpinBox();
        SpinBoxHandler handler = new SpinBoxHandler();

        handler.tryFixup(spinBox, "Immortal love, forever full");

        assertEquals("Immortal love, forever full", spinBox.receivedString);
        assertEquals("As aught of mortal birth", handler.my_returned_string());
    }

    @Test 
    public void testVirtualCallToValidate() {
    	Assume.assumeThat(QGuiApplication.primaryScreen()!=null, QApplicationTest.trueMatcher("A screen is required to create a window."));
        MySpinBox spinBox = new MySpinBox();
        SpinBoxHandler handler = new SpinBoxHandler();

        handler.tryValidate(spinBox, "Immortal love, forever full", 15);
        assertEquals("Immortal love, forever full", spinBox.receivedString);
        assertEquals(15, spinBox.receivedPos);
        assertEquals("The love where Death has set his seal", handler.my_returned_string());
        assertEquals(28, handler.my_returned_pos());
        assertEquals(QValidator.State.Acceptable, handler.my_returned_state());
    }

    @Test 
    public void testFinalCallToFixup() {
    	Assume.assumeThat(QGuiApplication.primaryScreen()!=null, QApplicationTest.trueMatcher("A screen is required to create a window."));
        SpinBoxSubclass sbs = new SpinBoxSubclass();

        String returned = sbs.fixup("Thou dost hang canary birds in parlour windows");
        assertEquals("And Thou art dead", returned);
        assertEquals("Thou dost hang canary birds in parlour windows", sbs.my_received_string());
    }

    @Test 
    public void testFinalCallToValidate() {
    	Assume.assumeThat(QGuiApplication.primaryScreen()!=null, QApplicationTest.trueMatcher("A screen is required to create a window."));
        SpinBoxSubclass sbs = new SpinBoxSubclass();

        QValidator.QValidationData data = new QValidator.QValidationData("dream and you have a sloppy body from being brought to bed of crocuses", 14);
        QValidator.State returned = sbs.validate(data);

        assertEquals(QValidator.State.Intermediate, returned);
        assertEquals("dream and you have a sloppy body from being brought to bed of crocuses", sbs.my_received_string());
        assertEquals(14, sbs.my_received_pos());
        assertEquals("The silence of that dreamless sleep", data.string);
        assertEquals(27, data.position);
    }

    public QPainter painterReference;

    @Test
    public void resetAfterUseTemporary() {
    	Assume.assumeThat(QGuiApplication.primaryScreen()!=null, QApplicationTest.trueMatcher("A screen is required to create a window."));
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
        assertTrue("painterReference deleted", 0==QtJambiInternal.nativeId(painterReference));
    }

    @Test
    public void resetAfterUseNonTemporary() {
    	Assume.assumeThat(QGuiApplication.primaryScreen()!=null, QApplicationTest.trueMatcher("A screen is required to create a window."));
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
        assertTrue("painterReference deleted", 0 != QtJambiInternal.nativeId(painterReference));
        assertEquals(p, painterReference);
    }

    @Test
    public void resetAfterUseNull() {
    	Assume.assumeThat(QGuiApplication.primaryScreen()!=null, QApplicationTest.trueMatcher("A screen is required to create a window."));
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

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestClassFunctionality.class.getName());
    }
}
