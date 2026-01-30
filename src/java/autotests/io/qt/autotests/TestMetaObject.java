/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import static org.junit.Assert.*;

import java.util.*;
import java.util.concurrent.atomic.*;
import org.junit.*;
import io.qt.*;
import io.qt.autotests.generated.*;
import io.qt.core.*;
import io.qt.gui.*;

/**
 * Testing the fake meta object.
 * @author eblomfel
 */
public class TestMetaObject extends ApplicationInitializer {
    @SuppressWarnings("unused")
    private static class SignalsAndSlotsSubclass extends SignalsAndSlots {
    	public static final @NonNull QMetaObject staticMetaObject = QMetaObject.forType(SignalsAndSlotsSubclass.class);
        enum YoYoYo {
            Yo,
            YoYo,
            YoYoYo
        }

        enum HeyYo implements QtEnumerator {
            Hey(0x4),
            Yo(1123);

            HeyYo(int value) { this.value = value; }
            public int value() { return value; }

            private final int value;
        }

        enum FlipModeSquad implements QtFlagEnumerator {
            Flip(1),
            Mode(2),
            Squad(4);

            FlipModeSquad(int value) { this.value = value; }
            public int value() { return value; }

            private final int value;

			@Override
			public FlipModeSquads asFlags() {
				return new FlipModeSquads(value());
			}
        }

		static class FlipModeSquads extends io.qt.QFlags<FlipModeSquad> implements Comparable<FlipModeSquads> {
			private static final long serialVersionUID = 886661346884528047L;
			public FlipModeSquads(FlipModeSquad ... args) { super(args); }
            public FlipModeSquads(int value) { super(value); }
			@Override
			public FlipModeSquads clone() {
				return new FlipModeSquads(intValue());
			}
			@Override
			public FlipModeSquads combined(FlipModeSquad flag) {
				return new FlipModeSquads(intValue() | flag.value());
			}
			@Override
			public FlipModeSquad[] flags() {
				return flags(FlipModeSquad.values());
			}

	        @Override
	        public final int compareTo(@StrictNonNull FlipModeSquads other){
	            return Integer.compare(value(), other.value());
	        }

	        public final int value(){
	            return intValue();
	        }

	        public final void setValue(int value){
	            setIntValue(value);
	        }
        }
    }

    @Test public void regularEnumDeclarations() {
        SignalsAndSlotsSubclass sass = new SignalsAndSlotsSubclass();

        assertEquals(4, sass.metaObject().enumeratorCount());

        QMetaEnum enumYoYoYo = sass.metaObject().enumerator("YoYoYo");
        assertFalse(enumYoYoYo.isFlag());
        assertEquals(3, enumYoYoYo.keyCount());
        assertEquals("Yo", enumYoYoYo.key(0));
        assertEquals("YoYo", enumYoYoYo.key(1));
        assertEquals("YoYoYo", enumYoYoYo.key(2));
        assertEquals(0, enumYoYoYo.value(0));
        assertEquals(1, enumYoYoYo.value(1));
        assertEquals(2, enumYoYoYo.value(2));
        assertEquals("Yo", enumYoYoYo.entry(0).name());
        assertEquals("YoYo", enumYoYoYo.entry(1).name());
        assertEquals("YoYoYo", enumYoYoYo.entry(2).name());
        assertEquals(0, ((QtEnumerator)enumYoYoYo.entry(0)).value());
        assertEquals(1, ((QtEnumerator)enumYoYoYo.entry(1)).value());
        assertEquals(2, ((QtEnumerator)enumYoYoYo.entry(2)).value());
    }

    @Test public void qtEnumeratorDeclarations() {
        SignalsAndSlotsSubclass sass = new SignalsAndSlotsSubclass();

        QMetaEnum enumHeyYo = sass.metaObject().enumerator("HeyYo");
        assertFalse(enumHeyYo.isFlag());
        assertEquals(2, enumHeyYo.keyCount());
        assertEquals("Hey", enumHeyYo.key(0));
        assertEquals("Yo", enumHeyYo.key(1));
        assertEquals(0x4, enumHeyYo.value(0));
        assertEquals(1123, enumHeyYo.value(1));
        assertEquals("Hey", enumHeyYo.entry(0).name());
        assertEquals("Yo", enumHeyYo.entry(1).name());
        assertEquals(0x4, ((QtEnumerator)enumHeyYo.entry(0)).value());
        assertEquals(1123, ((QtEnumerator)enumHeyYo.entry(1)).value());
    }

    @Test public void flagsDeclarations() {
        SignalsAndSlotsSubclass sass = new SignalsAndSlotsSubclass();

        QMetaEnum enumFlipModeSquads = sass.metaObject().enumerator("FlipModeSquads");
        assertTrue(enumFlipModeSquads.isFlag());
        assertEquals(3, enumFlipModeSquads.keyCount());
        assertEquals("Flip", enumFlipModeSquads.key(0));
        assertEquals("Mode", enumFlipModeSquads.key(1));
        assertEquals("Squad", enumFlipModeSquads.key(2));
        assertEquals(1, enumFlipModeSquads.value(0));
        assertEquals(2, enumFlipModeSquads.value(1));
        assertEquals(4, enumFlipModeSquads.value(2));
        assertEquals("Flip", enumFlipModeSquads.entry(0).name());
        assertEquals("Mode", enumFlipModeSquads.entry(1).name());
        assertEquals("Squad", enumFlipModeSquads.entry(2).name());
        assertEquals(1, ((QtEnumerator)enumFlipModeSquads.entry(0)).value());
        assertEquals(2, ((QtEnumerator)enumFlipModeSquads.entry(1)).value());
        assertEquals(4, ((QtEnumerator)enumFlipModeSquads.entry(2)).value());
    }
    
    static class SignalTest extends QObject{
    	private final Signal0 quit = new Signal0();
    	public void quit(){
    		quit.emit();
    	}
    }
    
    @Test 
    public void testDisposeLater() throws NoSuchMethodException, SecurityException {
    	if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)) {
	    	QMetaMethod method = QMetaMethod.fromReflectedMethod(QObject.class.getMethod("disposeLater"));
	    	assertTrue(method!=null);    		
    	}else {
	    	QMetaMethod method = QMetaMethod.fromMethod(QObject::disposeLater);
	    	assertTrue(method!=null);
    	}
    	class _QObject extends QObject{
    		int _receivers(io.qt.core.QMetaObject.AbstractSignal signalObject){
    			return receivers(signalObject);
    		}
    		int _receivers(String signal){
    			return receivers(signal);
    		}
    	}
    	_QObject sender = new _QObject();
    	QObject receiver = new QObject();
    	assertTrue(QObject.connect(sender, "destroyed(QObject)", receiver, "disposeLater()")!=null);
    	assertTrue(sender.destroyed.disconnect(receiver));
    	assertTrue(sender.destroyed.connect(receiver, "disposeLater()")!=null);
    	assertEquals(1, sender._receivers(sender.destroyed));
    	assertEquals(1, sender._receivers("destroyed(QObject)"));
    }
    
    public static class SubQObject extends QObject{
    	public static final @NonNull QMetaObject staticMetaObject = QMetaObject.forType(SubQObject.class);
    	@QtInvokable
		public SubQObject(int i, boolean b) {
			super();
    		this.i = i;
    		this.b = b;
    		this.s = "";
		}
    	
    	@QtInvokable
		public SubQObject(int i, boolean b, String s) {
			super();
    		this.i = i;
    		this.b = b;
    		this.s = s;
		}
    	
    	@QtInvokable
		public SubQObject(QObject parent, int i, boolean b) {
			super(parent);
    		this.i = i;
    		this.b = b;
    		this.s = "";
		}
    	
    	@QtInvokable
		public SubQObject(QObject parent, int i, boolean b, String s) {
			super(parent);
    		this.i = i;
    		this.b = b;
    		this.s = s;
		}
    	
    	SubQObject(QtConstructInPlace inPlace){
    		super((QPrivateConstructor)null);
    		if(inPlace.matches(int.class, boolean.class, String.class)) {
        		inPlace.initWithArguments(this);
    			this.i = inPlace.argumentAt(0, int.class);
        		this.b = inPlace.argumentAt(1, boolean.class);
        		this.s = inPlace.argumentAt(2, String.class);
    		}else if(inPlace.matches(int.class, boolean.class)) {
        		inPlace.initWithArguments(this);
    			this.i = inPlace.argumentAt(0, int.class);
        		this.b = inPlace.argumentAt(1, boolean.class);
        		this.s = "";
    		}else if(inPlace.matches(QObject.class, int.class, boolean.class, String.class)) {
        		inPlace.initWithArguments(this, 0);
    			this.i = inPlace.argumentAt(1, int.class);
        		this.b = inPlace.argumentAt(2, boolean.class);
        		this.s = inPlace.argumentAt(3, String.class);
    		}else if(inPlace.matches(QObject.class, int.class, boolean.class)) {
        		inPlace.initWithArguments(this, 0);
    			this.i = inPlace.argumentAt(1, int.class);
        		this.b = inPlace.argumentAt(2, boolean.class);
        		this.s = "";
    		}else {
    			throw new Error("Parameters mismatch");
    		}
    	}
    	
    	private int i;
    	private boolean b;
    	private String s;
    	
    	public final PrivateSignal0 dataChanged = new PrivateSignal0();
    	
    	@QtInvokable
    	public boolean initialize(int i, boolean b, String s) {
    		this.i = i;
    		this.b = b;
    		this.s = s;
			emit(dataChanged);
    		return true;
    	}

		public int i() {
			return i;
		}

		public void setI(int i) {
			this.i = i;
			emit(dataChanged);
		}

		public boolean b() {
			return b;
		}

		public void setB(boolean b) {
			this.b = b;
			emit(dataChanged);
		}

		public String s() {
			return s;
		}

		public void setS(String s) {
			this.s = s;
			emit(dataChanged);
		}
    }
    
    public static class SubQObject2 extends SubQObject{
    	public static final @NonNull QMetaObject staticMetaObject = QMetaObject.forType(SubQObject2.class);

		SubQObject2(QtConstructInPlace inPlace) {
			super(inPlace.asArguments(map->{
				if(map.matches(boolean.class, int.class, String.class, double.class)) {
					map.map(1, 0, 2);
	    		}else if(inPlace.matches(boolean.class, int.class, double.class)) {
	    			map.map(1, 0);
	    		}else if(inPlace.matches(boolean.class, int.class, String.class, double.class, QObject.class)) {
	    			map.map(4, 1, 0, 2);
	    		}else if(inPlace.matches(boolean.class, int.class, double.class, QObject.class)) {
	    			map.map(3, 1, 0);
	    		}
			}));
			if(inPlace.matches(boolean.class, int.class, String.class, double.class)) {
    			this.d = inPlace.argumentAt(3, double.class);
    		}else if(inPlace.matches(boolean.class, int.class, double.class)) {
    			this.d = inPlace.argumentAt(2, double.class);
    		}else if(inPlace.matches(boolean.class, int.class, String.class, double.class, QObject.class)) {
    			this.d = inPlace.argumentAt(3, double.class);
    		}else if(inPlace.matches(boolean.class, int.class, double.class, QObject.class)) {
    			this.d = inPlace.argumentAt(2, double.class);
    		}else {
    			throw new Error("Parameters mismatch");
    		}
		}

		@QtInvokable
		public SubQObject2(boolean b, int i, String s) {
			super(i, b, s);
		}

		@QtInvokable
		public SubQObject2(boolean b, int i) {
			super(i, b);
		}

		@QtInvokable
		public SubQObject2(boolean b, int i, String s, QObject parent) {
			super(parent, i, b, s);
		}

		@QtInvokable
		public SubQObject2(boolean b, int i, QObject parent) {
			super(parent, i, b);
		}

		@QtInvokable
		public SubQObject2(boolean b, int i, String s, double d) {
			super(i, b, s);
			this.d = d;
		}

		@QtInvokable
		public SubQObject2(boolean b, int i, double d) {
			super(i, b);
			this.d = d;
		}

		@QtInvokable
		public SubQObject2(boolean b, int i, String s, double d, QObject parent) {
			super(parent, i, b, s);
			this.d = d;
		}

		@QtInvokable
		public SubQObject2(boolean b, int i, double d, QObject parent) {
			super(parent, i, b);
			this.d = d;
		}
		
		private double d;

		public double d() {
			return d;
		}

		public void setD(double d) {
			this.d = d;
		}
    }
    
    public static class SubQtObject extends QStandardItem{
    	public static final @NonNull QMetaObject staticMetaObject = QMetaObject.forType(SubQtObject.class);
    	@QtInvokable
		public SubQtObject(int i, boolean b) {
			super();
		}
    	SubQtObject(QtConstructInPlace inPlace) {
    		super((QPrivateConstructor)null);
    		inPlace.initialize(this);
		}
    }
    
    public static class NonQtType{
    	public static final @NonNull QMetaObject staticMetaObject = QMetaObject.forType(NonQtType.class);
    	@QtInvokable
    	public NonQtType(int integerNumber, String text) {
			super();
			this.integerNumber = integerNumber;
			this.text = text;
		}
    	
		private int integerNumber;
    	private String text;
    	
    	@QtPropertyReader
		public int getIntegerNumber() {
			return integerNumber;
		}
    	@QtPropertyWriter
		public void setIntegerNumber(int i) {
			this.integerNumber = i;
		}
    	@QtPropertyReader
		public String getText() {
			return text;
		}
    	@QtPropertyWriter
		public void setText(String text) {
			this.text = text;
		}
    	
    	@QtInvokable
    	public static String staticMethod() {
    		return "success";
    	}
    	
    	@QtInvokable
    	public String formatNumber(double number) {
    		return Double.toHexString(number);
    	}
    }
    
    @Test 
    public void testForeignMetaObject() throws NoSuchMethodException {
    	assertEquals(null, QObject.staticMetaObject.constructors().get(0).returnClassType());
    	assertEquals(0, QObject.staticMetaObject.constructors().get(0).returnType());
    	assertEquals("", QObject.staticMetaObject.constructors().get(0).typeName());
//    	System.out.println(QObject.staticMetaObject.constructors());
//    	System.out.println(QObject.staticMetaObject.methods().size() + QObject.staticMetaObject.constructors().size());
    	QMetaObject subQObjectMO = QMetaObject.forType(SubQObject.class);
    	QMetaObject subQtObjectMO = QMetaObject.forType(SubQtObject.class);
//    	System.out.println(subQObjectMO.constructors());
    	assertEquals(null, subQObjectMO.constructors().get(0).returnClassType());
    	assertEquals(0, subQObjectMO.constructors().get(0).returnType());
    	assertEquals("", subQObjectMO.constructors().get(0).typeName());
//    	System.out.println(subQObjectMO.methods());
//    	System.out.println(subQtObjectMO.constructors());
//    	System.out.println(subQtObjectMO.methods());
//    	System.out.println(QMetaObject.forType(NonQtType.class).constructors());
//    	System.out.println(QMetaObject.forType(NonQtType.class).methods());
		Object instance = subQObjectMO.newInstance(1, false);
		assertTrue(instance instanceof SubQObject);
		Assert.assertTrue(General.internalAccess.isJavaOwnership((QObject)instance));
    	try {
    		instance = subQtObjectMO.newInstance(1, false);
    		assertTrue("RuntimeException expected to be thrown", false);
		} catch (Exception e) {
			assertEquals("Type io::qt::autotests::TestMetaObject::SubQtObject does not inherit QObject", e.getMessage());
		}
    	NonQtType object = new NonQtType(8, "test");
    	QMetaObject mo = QMetaObject.forType(object.getClass());
    	for(QMetaProperty prop : mo.properties()) {
    		switch(prop.name()) {
    		case "integerNumber":
    			assertEquals("property integerNumber", 8, prop.readOnGadget(object));
    			break;
    		case "text":
    			assertEquals("property text", "test", prop.readOnGadget(object));
    			break;
    		}
    	}
    	assertEquals("success", mo.method("staticMethod").invokeOnGadget(object));
    	double d = 0.12345;
    	assertEquals(Double.toHexString(d), mo.method("formatNumber", double.class).invokeOnGadget(object, d));
    }
    
    @Test public void slotHidesSignal() {
    	assertEquals(QMetaMethod.MethodType.Signal, QMetaObject.forType(SignalTest.class).method("quit").methodType());
    }
    
    public static class PairSlotObject extends QObject {
    	@QtInvokable
        private QPair<String, String> invokablePairSupplier() {
            return new QPair<>("A", "B");
        }
    	
    	@SuppressWarnings("unused")
		private int uninvokable() {
            return 0;
        }
    }
    
    @Test public void testPairMetaType() {
    	PairSlotObject object = new PairSlotObject();
    	QMetaMethod invokablePairSupplier = object.metaObject().method("invokablePairSupplier");
    	Assert.assertTrue(invokablePairSupplier.isValid());
    	Assert.assertTrue(invokablePairSupplier.returnType()!=0);
    	QMetaMethod uninvokable = object.metaObject().method("uninvokable");
    	Assert.assertFalse(uninvokable.isValid());
    }
    
    @Test public void testMetaObjectFromNamespace() {
    	Assert.assertEquals(Qt.staticMetaObject, QMetaObject.forType(Qt.class));
    	Assert.assertEquals(null, QMetaObject.forType(QtAlgorithms.class));
    }
    
    @Test public void testMetaObjectForUnreadableProperty() {
    	// must not lead to NullPointerException
    	QMetaObject.forType(SignalCarrier.class);
    }
    
    static class SignalCarrier extends QObject{
    	public final Signal1<String> xChanged = new Signal1<>();
    	public void setX(String x){}
    }
    
    static class PropertyCarrier extends QObject{
    	private boolean enabled;
    	private boolean visible;

    	@QtPropertyReader(name="isEnabled")
		public boolean isEnabled() {
			return enabled;
		}

		public void setEnabled(boolean enabled) {
			this.enabled = enabled;
		}

		public boolean visible() {
			return visible;
		}

		public void setVisible(boolean visible) {
			this.visible = visible;
		}
    }
    
    @Test public void testPropertyMethodNamingCollisions() {
    	QMetaObject mo = QMetaObject.forType(PropertyCarrier.class);
    	QMetaProperty isEnabledP = mo.property("isEnabled");
    	Assert.assertTrue(isEnabledP.isValid());
    	Assert.assertTrue(isEnabledP.isReadable());
    	Assert.assertFalse(isEnabledP.isWritable());
    	QMetaMethod isEnabledM = mo.method("isEnabled()");
    	Assert.assertTrue(isEnabledM.isValid());
    	
    	QMetaProperty visibleP = mo.property("visible");
    	Assert.assertTrue(visibleP.isValid());
    	Assert.assertTrue(visibleP.isReadable());
    	Assert.assertTrue(visibleP.isWritable());
    	QMetaMethod visibleM = mo.method("visible()");
    	Assert.assertTrue(visibleM.isValid());
    }
    
    static class PropertyMemberCarrier extends QObject {
    	public String text;

		public String getText() {
			return text;
		}

		public void setText(String text) {
			this.text = text;
		}
    }
    
    @Test public void testPublicMember() {
    	// not throwing QPropertyDeclarationException
    	new PropertyMemberCarrier();
    }
    
    @Test public void testInvokeSignal() {
    	QStringListModel model = new QStringListModel();
    	model.insertRow(5);
    	model.layoutChanged.connect(()->{
    		System.out.println("layoutChanged invoked in "+Thread.currentThread());
    	}, Qt.ConnectionType.DirectConnection);
    	model.layoutChanged.emit();
    	QMetaObject.invokeMethod(model.layoutChanged);
    	QMetaObject.invokeMethod(model.layoutChanged, Qt.ConnectionType.DirectConnection);
    	QMetaObject.invokeMethod(model.layoutChanged, Qt.ConnectionType.QueuedConnection);
    	QThread thread = QThread.create(()->{
    		model.layoutChanged.emit();
    		QMetaObject.invokeMethod(model.layoutChanged);
    		QMetaObject.invokeMethod(model.layoutChanged, Qt.ConnectionType.DirectConnection);
    	});
    	thread.start();
    	thread.join(20000);
    	QCoreApplication.processEvents();
    	QCoreApplication.sendPostedEvents();
    }
    
    @Test public void testInvokeDefaultSignal() {
    	class StringListModel extends QStringListModel{
    	    public final Signal2Default2<List<QPersistentModelIndex>, QAbstractItemModel.LayoutChangeHint> layoutChanged2 = 
    	    		new Signal2Default2<>(()->java.util.Collections.singletonList(new QPersistentModelIndex()), ()->QAbstractItemModel.LayoutChangeHint.VerticalSortHint);
    	}
    	StringListModel model = new StringListModel();
    	model.setStringList(Arrays.asList("A", "B", "C"));
    	AtomicReference<List<QPersistentModelIndex>> receivedIndexes = new AtomicReference<>();
    	AtomicReference<QAbstractItemModel.LayoutChangeHint> receivedHints = new AtomicReference<>();
    	model.layoutChanged.connect((indexes,hints)->{
    		receivedIndexes.set(indexes);
    		receivedHints.set(hints);
    	});
    	model.layoutChanged2.connect((indexes,hints)->{
    		receivedIndexes.set(indexes);
    		receivedHints.set(hints);
    	});
    	
    	receivedIndexes.set(null);
		receivedHints.set(null);
    	QMetaObject.invokeMethod(model.layoutChanged);
    	Assert.assertEquals(QAbstractItemModel.LayoutChangeHint.NoLayoutChangeHint, receivedHints.get());
    	Assert.assertEquals(java.util.Collections.emptyList(), receivedIndexes.get());
    	receivedIndexes.set(null);
		receivedHints.set(null);
		QList<QPersistentModelIndex> list = QList.of(new QPersistentModelIndex(model.index(0, 0)));
    	QMetaObject.invokeMethod(model.layoutChanged, list);
    	Assert.assertEquals(QAbstractItemModel.LayoutChangeHint.NoLayoutChangeHint, receivedHints.get());
    	Assert.assertEquals(list, receivedIndexes.get());
    	receivedIndexes.set(null);
		receivedHints.set(null);
		list = QList.of(new QPersistentModelIndex(model.index(0, 0)), new QPersistentModelIndex(model.index(0, 1)));
    	QMetaObject.invokeMethod(model.layoutChanged, list, QAbstractItemModel.LayoutChangeHint.HorizontalSortHint);
    	Assert.assertEquals(QAbstractItemModel.LayoutChangeHint.HorizontalSortHint, receivedHints.get());
    	Assert.assertEquals(list, receivedIndexes.get());
    	
    	receivedIndexes.set(null);
		receivedHints.set(null);
		QMetaMethod layoutChanged = model.metaObject().method("layoutChanged()");
		Assert.assertTrue(layoutChanged.isValid());
		layoutChanged.invoke(model);
    	Assert.assertEquals(QAbstractItemModel.LayoutChangeHint.NoLayoutChangeHint, receivedHints.get());
    	Assert.assertEquals(java.util.Collections.emptyList(), receivedIndexes.get());
    	receivedIndexes.set(null);
		receivedHints.set(null);
		layoutChanged = model.metaObject().method("layoutChanged(QList<QPersistentModelIndex>)");
		Assert.assertTrue(layoutChanged.isValid());
		list = QList.of(new QPersistentModelIndex(model.index(0, 0)));
		layoutChanged.invoke(model, list);
    	Assert.assertEquals(QAbstractItemModel.LayoutChangeHint.NoLayoutChangeHint, receivedHints.get());
    	Assert.assertEquals(list, receivedIndexes.get());
    	receivedIndexes.set(null);
		receivedHints.set(null);
		layoutChanged = model.metaObject().method("layoutChanged(QList<QPersistentModelIndex>,QAbstractItemModel::LayoutChangeHint)");
		Assert.assertTrue(layoutChanged.isValid());
		list = QList.of(new QPersistentModelIndex(model.index(0, 0)), new QPersistentModelIndex(model.index(0, 1)));
		layoutChanged.invoke(model, list, QAbstractItemModel.LayoutChangeHint.HorizontalSortHint);
    	Assert.assertEquals(QAbstractItemModel.LayoutChangeHint.HorizontalSortHint, receivedHints.get());
    	Assert.assertEquals(list, receivedIndexes.get());
    	
    	receivedIndexes.set(null);
		receivedHints.set(null);
    	QMetaObject.invokeMethod(model.layoutChanged2);
    	Assert.assertEquals(QAbstractItemModel.LayoutChangeHint.VerticalSortHint, receivedHints.get());
    	Assert.assertEquals(java.util.Collections.singletonList(new QPersistentModelIndex()), receivedIndexes.get());
    	receivedIndexes.set(null);
		receivedHints.set(null);
		list = QList.of(new QPersistentModelIndex(model.index(0, 0)));
    	QMetaObject.invokeMethod(model.layoutChanged2, list);
    	Assert.assertEquals(QAbstractItemModel.LayoutChangeHint.VerticalSortHint, receivedHints.get());
    	Assert.assertEquals(list, receivedIndexes.get());
    	receivedIndexes.set(null);
		receivedHints.set(null);
		list = QList.of(new QPersistentModelIndex(model.index(0, 0)), new QPersistentModelIndex(model.index(0, 1)));
    	QMetaObject.invokeMethod(model.layoutChanged2, list, QAbstractItemModel.LayoutChangeHint.HorizontalSortHint);
    	Assert.assertEquals(QAbstractItemModel.LayoutChangeHint.HorizontalSortHint, receivedHints.get());
    	Assert.assertEquals(list, receivedIndexes.get());
    	
    	receivedIndexes.set(null);
		receivedHints.set(null);
		QMetaMethod layoutChanged2 = model.metaObject().method("layoutChanged2()");
		Assert.assertTrue(layoutChanged2.isValid());
		layoutChanged2.invoke(model);
    	Assert.assertEquals(QAbstractItemModel.LayoutChangeHint.VerticalSortHint, receivedHints.get());
    	Assert.assertEquals(java.util.Collections.singletonList(new QPersistentModelIndex()), receivedIndexes.get());
    	receivedIndexes.set(null);
		receivedHints.set(null);
		layoutChanged2 = model.metaObject().method("layoutChanged2(QList<QPersistentModelIndex>)");
		Assert.assertTrue(layoutChanged2.isValid());
		list = QList.of(new QPersistentModelIndex(model.index(0, 0)));
		layoutChanged2.invoke(model, list);
    	Assert.assertEquals(QAbstractItemModel.LayoutChangeHint.VerticalSortHint, receivedHints.get());
    	Assert.assertEquals(list, receivedIndexes.get());
    	receivedIndexes.set(null);
		receivedHints.set(null);
		layoutChanged2 = model.metaObject().method("layoutChanged2(QList<QPersistentModelIndex>,QAbstractItemModel::LayoutChangeHint)");
		Assert.assertTrue(layoutChanged2.isValid());
		list = QList.of(new QPersistentModelIndex(model.index(0, 0)), new QPersistentModelIndex(model.index(0, 1)));
		layoutChanged2.invoke(model, list, QAbstractItemModel.LayoutChangeHint.HorizontalSortHint);
    	Assert.assertEquals(QAbstractItemModel.LayoutChangeHint.HorizontalSortHint, receivedHints.get());
    	Assert.assertEquals(list, receivedIndexes.get());
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestMetaObject.class.getName());
    }
}
