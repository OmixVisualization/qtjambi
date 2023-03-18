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
import static org.junit.Assert.assertTrue;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import org.junit.After;
import org.junit.Assert;
import org.junit.Assume;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QFlags;
import io.qt.QNoImplementationException;
import io.qt.QtEnumerator;
import io.qt.QtFlagEnumerator;
import io.qt.QtInvokable;
import io.qt.QtObject;
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
import io.qt.core.QMetaObject;
import io.qt.core.QObject;
import io.qt.core.QRect;
import io.qt.core.QRectF;
import io.qt.core.QResource;
import io.qt.core.QSize;
import io.qt.core.QString;
import io.qt.core.QTimer;
import io.qt.core.QUuid;
import io.qt.core.Qt;
import io.qt.gui.*;
import io.qt.widgets.*;

public class TestClassFunctionality extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
    @Before
    public void setUp() {
        QApplication.processEvents();
    }

    @After
    public void tearDown() {
        QApplication.processEvents();

        ApplicationInitializer.runGC();
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

            options[1].setExposedRect(new QRectF(5,7,9,1));
            super.drawItems(painter, items, options, widget);
        }

        public static void tearDown() {
            items = null;
            options = null;
        }
    }

    @Test
    public void testGraphicsSceneDrawItemsInjections() {
    	Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
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
        
        assertTrue("item still alive", General.internalAccess.nativeId(GraphicsSceneSubclassSubclass.items[0])!=0);
        assertTrue("item still alive", General.internalAccess.nativeId(GraphicsSceneSubclassSubclass.items[1])!=0);

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
        assertEquals(new QRectF(2,3,4,5), ((QStyleOptionGraphicsItem) option).exposedRect());

        option = gsss.secondStyleOption();
        assertTrue(option instanceof QStyleOptionGraphicsItem);
        assertEquals(new QRectF(5,7,9,1), ((QStyleOptionGraphicsItem) option).exposedRect());

        GraphicsSceneSubclassSubclass.tearDown();
        view.hide();
    }

    static class TestQObject extends QObject {
        private final Signal0 a = new Signal0();

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
    public void testToString()
    {
        QByteArray ba = new QByteArray("Pretty flowers æøå");
        assertEquals("Pretty flowers æøå", ba.toString());
    }
    
    @Test
    public void test_GC_versus_access_SplitOwnership() throws InterruptedException {
    	Set<Integer> hashes = new HashSet<>();
    	for (int i = 0; i < 50 && hashes.size()<=1; i++) {
    		{
				ApplicationInitializer.runGC();
	    		QObject globalObject = OrdinaryDestroyed.getGlobalQObjectSplitOwnership();
	    		assertTrue(globalObject!=null);
	    		assertTrue(General.internalAccess.isSplitOwnership(globalObject));
	    		hashes.add(System.identityHashCode(globalObject));
	    		globalObject = null;
    		}
			Thread.yield();
    		Thread.sleep(25);
		}
    	assertTrue("Expect to create more than one java objects for global QObject with split ownership.", hashes.size()>1);
    }

    @Test
    public void testDestructionCppDelete_Ordinary(){
    	DisposeCounter counter = new DisposeCounter();
        OrdinarySubclass sc = new OrdinarySubclass(counter);
        OrdinaryDestroyed.deleteFromCpp(sc);
        assertEquals(1, counter.disposedCount());
        assertTrue(sc.isDisposed());
    }

    @Test
    public void testDestructionByVirtualDestructor_Ordinary(){
    	DisposeCounter counter = new DisposeCounter();
        OrdinarySuperclass sc = new OrdinarySubclass(counter);
        OrdinaryDestroyed.deleteFromCppOther(sc);
        assertEquals(1, counter.disposedCount());
        assertTrue(sc.isDisposed());
    }

    @Test
    public void testDestructionByDispose_QObject()
    {
        DisposeCounter counter = new DisposeCounter();
        QObjectSubclass qobject = new QObjectSubclass(counter, null);
        qobject.dispose();
        assertEquals(1, counter.disposedCount());
        assertTrue(qobject.isDisposed());
    }

    @Test
    public void testDestructionByParent_QObject()
    {
        DisposeCounter counter = new DisposeCounter();
        QObject parent = new QObject();
        QObject qobject = new QObjectSubclass(counter, parent);
        parent.dispose();
        assertEquals(1, counter.disposedCount());
        assertTrue(qobject.isDisposed());
    }

    @Test
    public void testDestructionByDisposeLater_QObject()
    {
        DisposeCounter counter = new DisposeCounter();
        QObject qobject = new QObjectSubclass(counter, null);
        qobject.disposeLater();
        QApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());

        assertEquals(1, counter.disposedCount());
        assertTrue(qobject.isDisposed());
    }

    @Test
    public void testDestructionCppDelete_QObject()
    {
        DisposeCounter counter = new DisposeCounter();
        QObjectSubclass qobject = new QObjectSubclass(counter, null);
        QObjectDestroyed.deleteFromCpp(qobject);
        assertEquals(1, counter.disposedCount());
        assertTrue(qobject.isDisposed());
    }

    @Test
    public void testDestructionByVirtualDestructor_QObject(){
        DisposeCounter counter = new DisposeCounter();
        QObject qobject = new QObjectSubclass(counter, null);
        QObjectDestroyed.deleteFromCppOther(qobject);
        assertEquals(1, counter.disposedCount());
        assertTrue(qobject.isDisposed());
    }
    
    @Test
    public void testDestructionByJavaGC(){
        DisposeCounter counter = new DisposeCounter();
        {
            new OrdinarySubclass(counter);
        }
        try {
            for(int i = 0; i < 60; i++) {  
                ApplicationInitializer.runGC();
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


    /**
     * Test that calling a private virtual function gives you an exception
     */
    @SuppressWarnings("deprecation")
    @Test
    public void run_callPrivateVirtualFunction() {
    	Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
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
        final Signal0 signal = new Signal0();

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
    
	@SuppressWarnings("unused")
    private static class MocTest extends QObject {
    	private int index;
		public int getIndex() {
			return index;
		}
		public void setIndex(int index) {
			this.index = index;
		}
		private String text;

		public String getText() {
			return text;
		}
		public void setText(String text) {
			this.text = text;
		}
		
		private QColor color;
		public QColor getColor() {
			return color;
		}
		public void setColor(QColor color) {
			this.color = color;
		}
		
		private double[] array;
		public double[] getArray() {
			return array;
		}
		public void setArray(double[] array) {
			this.array = array;
		}
		public final Signal0 sig0 = null;
    	public final Signal1<Boolean> sig1 = null;
    	public final Signal2<QObject,QColor> sig2 = null;
    	@QtInvokable
    	public MocTest(QObject p){super(p);}
    	public int int0() {
    		return 0;
    	}
    	public void void1D(double d) {
    		
    	}
    	public float F2CI(char c, int[] i) {
    		return 0;
    	}
    	
    	public enum E implements QtEnumerator{
    		A,B,C,D,E
    	}
    	
    	public enum Ef implements QtFlagEnumerator{
    		A,B,C,D,E
    	}
    	
    	public final class Flag extends QFlags<Ef>{
			private static final long serialVersionUID = 1L;

			@Override
			public Flag clone() {
				return (Flag)super.clone();
			}

			@Override
			public Flag combined(Ef flag) {
				return (Flag)super.combined(flag);
			}

			@Override
			public Flag setFlag(Ef flag) {
				return (Flag)super.setFlag(flag);
			}

			@Override
			public Flag setFlag(Ef flag, boolean on) {
				return (Flag)super.setFlag(flag, on);
			}

			@Override
			public Ef[] flags() {
				return super.flags();
			}

			public Flag(Ef... args) {
				super(args);
			}
    	}
    }
    
    @Test
    public void mocTest() {
    	QMetaObject mo = QMetaObject.forType(MocTest.class);
    	mo.enumerators().forEach(enm->{
    		java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, 
					    		"enumName: "+enm.enumName()+"\n"+
					    		"type: "+enm.type()+"\n"+
					    		"scope: "+enm.scope()+"\n"+
					    		"entries: "+Arrays.toString(enm.entries())+"\n"+
					    		"isFlag: "+enm.isFlag()+"\n"+
					    		"flags: "+enm.flags(3)+"\n"+
					    		"name: "+enm.name());
    	});
    	mo.constructors().forEach(m->{
    		java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, 
    				m.returnClassType() + " " + m.methodSignature() + " = " + m.cppMethodSignature() + "\n" + m.toReflectedConstructor());
    	});
    	mo.methods().forEach(m->{
    		if(m.enclosingMetaObject()==mo) {
	    		switch(m.methodType()) {
				case Method:
				case Slot:
	    			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, 
	    					m.returnClassType() + " " + m.methodSignature() + " = " + m.cppMethodSignature() + "\n" + m.toReflectedMethod());
					break;
				default:
	    			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, 
	    					m.returnClassType() + " " + m.methodSignature() + " = "+m.cppMethodSignature());
					break;
	    		}
    		}
    	});
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
    	Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
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
    	Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
        String[] childrenClassList = {".QGridLayout",".QLabel",".QComboBox",".QLabel",".QComboBox",".QLabel",".QComboBox",".QPushButton",".QTableWidget",".QLabel", ".QPushButton"};

        TestDialog dialog = new TestDialog();
        dialog.show();
        int i = 0;
        List<QObject> children = dialog.children();
        for (QObject o : children) {
    		assertTrue("class at index "+i+" unexpected: "+o.getClass().getName(), o.getClass().getName().endsWith(childrenClassList[i]));
            ++i;
        }
        dialog.hide();
    }

    @Test
    public void test_timeout() {
        SenderTester tester = new SenderTester();
        QTimer.singleShot(1000, tester::timeoutSlot);

        ApplicationInitializer.runGC();

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
		QResource.addClassPath(".");
        QFileInfo file1 = new QFileInfo(":io/qt/autotests/TestClassFunctionality.jar");
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
        try {
            byte b[] = new byte[6];
            uuid.setData4(b);
            Assert.fail("IllegalArgumentException expected to be thrown.");
        } catch (IllegalArgumentException e) {
            assertEquals("Argument 'data4': Wrong number of elements in array. Found: 6, expected: 8", e.getMessage());
        }

        // Make sure it wasn't set after all
        b2 = uuid.data4();
        assertEquals(b2.length, 8);
        for (int i = 0; i < 8; ++i)
            assertEquals(b2[i], b_orig[i]);

        try {
            byte b[] = new byte[100];
            uuid.setData4(b);
            Assert.fail("IllegalArgumentException expected to be thrown.");
        } catch (IllegalArgumentException e) {
            assertEquals("Argument 'data4': Wrong number of elements in array. Found: 100, expected: 8", e.getMessage());
        }
    }

    static class MySpinBox extends QDoubleSpinBox {

        public String receivedString;
        public int receivedPos;

        @Override
        public void fixup(QString input) {
            receivedString = input.toString();
            input.assign("As aught of ");
            input.append(receivedString.substring(2, 8));
            input.append(" birth");
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
    	Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
        MySpinBox spinBox = new MySpinBox();
        SpinBoxHandler handler = new SpinBoxHandler();

        handler.tryFixup(spinBox, "Immortal love, forever full");

        assertEquals("Immortal love, forever full", spinBox.receivedString);
        assertEquals("As aught of mortal birth", handler.my_returned_string());
    }

    @Test 
    public void testVirtualCallToValidate() {
    	Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
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
    	Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
        SpinBoxSubclass sbs = new SpinBoxSubclass();

        String returned = sbs.fixup("Thou dost hang canary birds in parlour windows");
        assertEquals("And Thou art dead", returned);
        assertEquals("Thou dost hang canary birds in parlour windows", sbs.my_received_string());
    }

    @Test 
    public void testFinalCallToValidate() {
    	Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
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
    	Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
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
        assertTrue("painterReference deleted", painterReference.isDisposed());
    }

    @Test
    public void resetAfterUseNonTemporary() {
    	Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
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
        assertTrue("painterReference deleted", !painterReference.isDisposed());
        assertEquals(p, painterReference);
    }

    @Test
    public void resetAfterUseNull() {
    	Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
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
            QtObject new_event = event; /*null;
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
