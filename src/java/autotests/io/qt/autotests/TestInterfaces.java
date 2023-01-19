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

import java.util.List;
import java.util.TreeMap;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;

import org.junit.After;
import org.junit.Assert;
import org.junit.Assume;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QInterfaceCannotBeSubclassedException;
import io.qt.QMissingVirtualOverridingException;
import io.qt.QNoNativeResourcesException;
import io.qt.QtArgument;
import io.qt.QtFinalOverride;
import io.qt.QtObject;
import io.qt.QtObjectInterface;
import io.qt.QtUninvokable;
import io.qt.QtUtilities;
import io.qt.autotests.generated.FunctionalTest;
import io.qt.autotests.generated.General;
import io.qt.autotests.generated.NameSpace;
import io.qt.autotests.generated.NameSpace.NameSpace2.NameSpace3.InterfaceA;
import io.qt.autotests.generated.NameSpace.NameSpace2.NameSpace3.ObjectC;
import io.qt.autotests.generated.NameSpace.NameSpace2.NameSpace3.ObjectD;
import io.qt.autotests.generated.TestAbstractClass;
import io.qt.autotests.generated.TestInterface;
import io.qt.autotests.generated.TestPrivateInterface;
import io.qt.autotests.generated.Tulip;
import io.qt.core.QEasingCurve;
import io.qt.core.QEvent;
import io.qt.core.QEventLoop;
import io.qt.core.QFactoryInterface;
import io.qt.core.QList;
import io.qt.core.QMetaObject;
import io.qt.core.QObject;
import io.qt.core.QPointF;
import io.qt.core.QRectF;
import io.qt.core.QRunnable;
import io.qt.core.QSize;
import io.qt.core.QSizeF;
import io.qt.core.QThread;
import io.qt.core.QTimer;
import io.qt.gui.QAbstractUndoItem;
import io.qt.gui.QColor;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QPaintDevice;
import io.qt.gui.QPaintEngine;
import io.qt.gui.QPaintEngineState;
import io.qt.gui.QPainter;
import io.qt.gui.QPainterPath;
import io.qt.gui.QPixmap;
import io.qt.gui.QScrollPrepareEvent;
import io.qt.gui.QSurface;
import io.qt.gui.QSurfaceFormat;
import io.qt.internal.TestUtility;
import io.qt.widgets.QApplication;
import io.qt.widgets.QGraphicsItem;
import io.qt.widgets.QGraphicsScene;
import io.qt.widgets.QGraphicsView;
import io.qt.widgets.QStyleOptionGraphicsItem;
import io.qt.widgets.QWidget;

public class TestInterfaces extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
	static class GraphicsItem implements QPaintDevice, QGraphicsItem{
		@SuppressWarnings("unused")
		private final QGraphicsItem.MemberAccess __qt_methodAccess_QGraphicsItem = QGraphicsItem.MemberAccess.of(this);
		private final QPaintDevice.MemberAccess __qt_methodAccess_QPaintDevice = QPaintDevice.MemberAccess.of(this);
		
		public GraphicsItem() {
			super();
		}
		
		@QtUninvokable
	    protected int metric(io.qt.gui.QPaintDevice.PaintDeviceMetric metric){
	        return __qt_methodAccess_QPaintDevice.metric(metric);
	    }
		
		@Override
		public QPaintEngine paintEngine() {
			return new QPaintEngine() {
				
				@Override
				public void updateState(QPaintEngineState state) {
					
				}
				
				@Override
				public Type type() {
					return Type.User;
				}
				
				@Override
				public boolean end() {
					return false;
				}
				
				@Override
				public void drawPixmap(QRectF r, QPixmap pm, QRectF sr) {
				}
				
				@Override
				public boolean begin(QPaintDevice pdev) {
					return false;
				}
			};
		}

		public final AtomicBoolean painted = new AtomicBoolean();

		@Override
		public QRectF boundingRect() {
			return new QRectF(-20, -20, 40, 40);
		}

		@Override
		public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {
			painted.set(true);
		}

		@Override
		public QPainterPath shape() {
			QPainterPath path = new QPainterPath();
			path.addRect(boundingRect());
			return path;
		}
	}
	
	@Before
    public void setUp() {
	}
	
	@After
    public void tearDown() {
	}
	
	@Test
    public void testSubClassTheSubClass() {
		GraphicsItem item = new GraphicsItem(){};
		item.metric(io.qt.gui.QPaintDevice.PaintDeviceMetric.PdmDepth);
	}
	
	@Test
    public void testInterface() {
		Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
		AtomicInteger disposed = new AtomicInteger();
		GraphicsItem item = new GraphicsItem();
		QtUtilities.getSignalOnDispose(item).connect(disposed::incrementAndGet);
		QGraphicsScene scene = new QGraphicsScene();
		scene.addItem(item);
		QGraphicsView view = new QGraphicsView();
		view.setScene(scene);
		scene.setParent(view);
		view.show();
		scene.update();
		QEventLoop loop = new QEventLoop();
		QTimer.singleShot(1000, ()->{
			QThread.create(()->{
				item.dispose();
			}).start();
		});
		QTimer.singleShot(3000, loop::quit);
		loop.exec();
		view.hide();
		Assert.assertTrue(item.painted.get());
		Assert.assertEquals(1, disposed.get());
	}
	
	@Test(expected=QMissingVirtualOverridingException.class)
	public void run_test_MissingVirtualOverriding() {
		NameSpace.NameSpace2.NameSpace3.InterfaceA iface = new NameSpace.NameSpace2.NameSpace3.InterfaceA() {
			@Override
			public InterfaceA fooBar(InterfaceA arg__1) {
				return null;
			}

			@Override
			public ObjectD fooBar2(ObjectC arg__1) {
				return null;
			}
		};
		iface.publicFinal();
	}
	
	@Test
	public void run_test_custom_inline_interface_implementation() {
		QGraphicsItem item = new QGraphicsItem(){
			@Override
			public QRectF boundingRect() {
				return shape().boundingRect();
			}

			@Override
			public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {
			}
			@Override
			public QPainterPath shape() {
				QPainterPath path = new QPainterPath();
				return path;
			}
			
			{
				// should be allowed
				QGraphicsItem.MemberAccess.of(this);
			}
		};
		Assert.assertFalse(item.isDisposed());
	}
	
	@Test(expected=RuntimeException.class)
	public void run_test_illegal_MemberAccess_from_outside() {
		QGraphicsItem item = new QGraphicsItem(){
			@Override
			public QRectF boundingRect() {
				return shape().boundingRect();
			}

			@Override
			public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {
			}
			@Override
			public QPainterPath shape() {
				QPainterPath path = new QPainterPath();
				return path;
			}
		};
		// expected to throw exception
		QGraphicsItem.MemberAccess.of(item);
	}
	
	@Test(expected=RuntimeException.class)
	public void run_test_illegal_MemberAccess_from_Impl_class() {
		QGraphicsItem item = new QGraphicsItem.Impl(){
			@Override
			public QRectF boundingRect() {
				return shape().boundingRect();
			}

			@Override
			public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {
			}
			@Override
			public QPainterPath shape() {
				QPainterPath path = new QPainterPath();
				return path;
			}
			
			{
				// expected to throw exception
				QGraphicsItem.MemberAccess.of(this);
			}
		};
		Assert.assertTrue(item!=null);
	}
	
	private static class Incombinable extends QColor implements QAbstractUndoItem{
		@Override
		public void redo() {
		}

		@Override
		public void undo() {
		}
	}
	
	private static class InconstructibleSurfaceObject extends QEvent implements QSurface{
		InconstructibleSurfaceObject() {
			super((QPrivateConstructor)null);
			QtUtilities.initializeNativeObject(this, 
					QtArgument.begin(  QEvent.class).add(CustomTypeInconstructible.customType));
		}

		@Override
		public QSurfaceFormat format() {
			return null;
		}

		@Override
		public QSize size() {
			return null;
		}

		@Override
		public SurfaceType surfaceType() {
			return null;
		}
	}
	
	private static class InconstructibleSurfaceInterface implements QSurface{
		InconstructibleSurfaceInterface() {
			QtUtilities.initializeNativeObject(this, 
					QtArgument.begin(QSurface.class));
		}

		@Override
		public QSurfaceFormat format() {
			return null;
		}

		@Override
		public QSize size() {
			return null;
		}

		@Override
		public SurfaceType surfaceType() {
			return null;
		}
	}
	
	@Test(expected=QInterfaceCannotBeSubclassedException.class)
	public void run_testInconstructibleSurfaceInterface() {
		new InconstructibleSurfaceInterface();
	}
	
	@Test(expected=QInterfaceCannotBeSubclassedException.class)
	public void run_testInconstructibleSurfaceObject() {
		new InconstructibleSurfaceObject();
	}
	
	@Test(expected=QInterfaceCannotBeSubclassedException.class)
	public void run_testPrivateInterface() {
		TestPrivateInterface test = new TestPrivateInterface(){
			@Override
			public void interfaceVirtual() {
			}
		};
		test.isDisposed();
	}
	
	private static class CustomTypeInconstructible extends QEvent implements TestInterface{
		static final QEvent.Type customType = QEvent.Type.resolve(QEvent.registerEventType(), "CustomTestEventType");
		CustomTypeInconstructible() {
			super(customType);
		}

		CustomTypeInconstructible(QPrivateConstructor p) {
			super(p);
		}
		
		@Override
		public boolean setReferenceCountTest1(QObject object) {
			return false;
		}

		@Override
		public String method4() {
			return "CustomTypeInconstructible::method4";
		}

		@Override
		public String method5() {
			return "CustomTypeInconstructible::method5";
		}
	}
	
	private static class CustomTypeWithArgs extends CustomTypeInconstructible{
		CustomTypeWithArgs(int s){
			super((QPrivateConstructor)null);
			switch(s) {
			case 0:
				QtUtilities.initializeNativeObject(this, 
						QtArgument.begin(  QEvent.class).add(CustomTypeInconstructible.customType)
								  .begin(TestInterface.class).add("Hello World!"));
				break;
			case 1:
				QtUtilities.initializeNativeObject(this, QtArgument.begin(QEvent.class));
				break;
			case 2:
				QtUtilities.initializeNativeObject(this, QtArgument.begin(QEvent.class).add(CustomTypeInconstructible.customType).begin(TestInterface.class).add("test").add(5));
				break;
			case 3:
				QtUtilities.initializeNativeObject(this, QtArgument.begin(QEvent.class).add(CustomTypeInconstructible.customType));
				break;
			}
		}
	}
	
	private static class CustomObjectMultiInterface extends QObject implements QAbstractUndoItem, TestInterface, QFactoryInterface{

		public CustomObjectMultiInterface() {
			super();
		}

		public CustomObjectMultiInterface(boolean withArgs) {
			super((QPrivateConstructor)null);
			QtUtilities.initializeNativeObject(this, QtArgument.begin(TestInterface.class).add("test"));
		}

		@Override
		public List<String> keys() {
			return null;
		}

		@Override
		public boolean setReferenceCountTest1(QObject object) {
			return false;
		}

		@Override
		public String method4() {
			return "CustomObjectMultiInterface::method4";
		}

		@Override
		public String method5() {
			return "CustomObjectMultiInterface::method5";
		}

		@Override
		public void redo() {
		}

		@Override
		public void undo() {
		}
	}
	
	private static class CustomObjectMultiInterfaceWithArgs extends CustomObjectMultiInterface{
		public CustomObjectMultiInterfaceWithArgs() {
			super(true);
		}
	}
	
	@Test
	public void run_testInterfaceAccess_Incombinable() {
		try {
			new Incombinable();
		}catch(AssertionError e) {
			throw e;
		} catch (Error e) {
			Assert.assertEquals("It is not permitted to create a derived type of io.qt.gui.QColor implementing any Qt interface.", e.getLocalizedMessage());
		}
	}
	
	@Test
	public void run_testInterfaceAccess_CustomTypeWithArgs() {
		CustomTypeWithArgs object = new CustomTypeWithArgs(0);
		Assert.assertFalse(object.isDisposed());
	}
	
	@Test
	public void run_testInterfaceAccess_CustomTypeWithArgs_wrongNumberOfArgs() {
		try {
			CustomTypeWithArgs object = new CustomTypeWithArgs(2);
			Assert.assertFalse(object.isDisposed());
		}catch(AssertionError e) {
			throw e;
		}catch(IllegalArgumentException e) {
			Assert.assertEquals("Cannot find constructor io.qt.autotests.generated.TestInterface(java.lang.String, int).", e.getLocalizedMessage());
		}
	}
	
	@Test
	public void run_testInterfaceAccess_CustomTypeWithArgs_noArgs() {
		try {
			CustomTypeWithArgs object = new CustomTypeWithArgs(1);
			Assert.assertFalse(object.isDisposed());
		}catch(AssertionError e) {
			throw e;
		}catch(IllegalArgumentException e) {
			Assert.assertEquals("Cannot find constructor io.qt.core.QEvent().", e.getLocalizedMessage());
		}
	}
	
	@Test
	public void run_testInterfaceAccess_CustomTypeWithArgs_nullArgs() {
		try {
			CustomTypeWithArgs object = new CustomTypeWithArgs(3);
			Assert.assertEquals(null, object);
		}catch(AssertionError e) {
			throw e;
		}catch(IllegalArgumentException e) {
			Assert.assertEquals("Cannot find constructor io.qt.autotests.generated.TestInterface().", e.getLocalizedMessage());
		}
	}
	
	@Test(expected=QInterfaceCannotBeSubclassedException.class)
	public void run_testInterfaceAccess_inline_QSurface() {
		QSurface object = new QSurface() {
			@Override
			public QSurfaceFormat format() {
				return null;
			}

			@Override
			public QSize size() {
				return null;
			}

			@Override
			public SurfaceType surfaceType() {
				return null;
			}
		};
		object.supportsOpenGL();
	}
	
	@Test
	public void run_testInterfaceAccess_CustomObjectMultiInterface() {
		CustomObjectMultiInterface o = new CustomObjectMultiInterface(true);
		assertFalse(o.isDisposed());
	}
	
	@Test
	public void run_testInterfaceAccess_CustomObjectMultiInterface_noArgs() {
		try {
			CustomObjectMultiInterface object = new CustomObjectMultiInterface();
			Assert.assertEquals(null, object);
		}catch(AssertionError e) {
			throw e;
		}catch(Error e) {
			Assert.assertEquals("Cannot initialize interface io.qt.autotests.generated.TestInterface without arguments. Please use the private constructor and QtUtilities.initializeNativeObject(object, arguments...) instead.", e.getLocalizedMessage());
		}
	}
	
	@Test
	public void run_testInterfaceAccess_CustomTypeInconstructible() {
		try {
			CustomTypeInconstructible object = new CustomTypeInconstructible();
			Assert.assertEquals(null, object);
		}catch(AssertionError e) {
			throw e;
		}catch(Error e) {
			Assert.assertEquals("Cannot initialize interface io.qt.autotests.generated.TestInterface without arguments. Please use the private constructor and QtUtilities.initializeNativeObject(object, arguments...) instead.", e.getLocalizedMessage());
		}
	}
	
	@Test
	public void run_testInterfaceAccess_CustomObjectMultiInterface_withArgs() {
		CustomObjectMultiInterfaceWithArgs object = new CustomObjectMultiInterfaceWithArgs();
		Assert.assertFalse(object.isDisposed());
	}
	
	@Test(expected=RuntimeException.class)
	public void run_testIllegalInterfaceImplementation2() {
		QGraphicsItem item = new QGraphicsItem(){
			@Override
			public QRectF boundingRect() {
				return shape().boundingRect();
			}

			@Override
			public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {
			}
			@Override
			public QPainterPath shape() {
				QPainterPath path = new QPainterPath();
				return path;
			}
			@QtUninvokable
		    protected void addToIndex(){
		        QGraphicsItem.MemberAccess.of(this).addToIndex();
		    }
		};
		Assert.assertFalse(item.isDisposed());
	}
	
	@Test
	public void run_test_createInterfaceA() {
		NameSpace.NameSpace2.NameSpace3.InterfaceA i = new NameSpace.NameSpace2.NameSpace3.InterfaceA() {
			@Override
			public ObjectD fooBar2(ObjectC arg__1) {
				return null;
			}
			
			@Override
			public InterfaceA fooBar(InterfaceA arg__1) {
				return null;
			}
			
            @io.qt.QtUninvokable
            protected io.qt.autotests.generated.NameSpace.NameSpace2.NameSpace3.InterfaceA protectedFooBar(io.qt.autotests.generated.NameSpace.NameSpace2.NameSpace3.InterfaceA arg__1){
            	return arg__1;
            }
		};
		Assert.assertFalse(i.isDisposed());
		Assert.assertEquals((Object)null, i.constructorParameter1());
		Assert.assertEquals((Object)null, i.constructorParameter2());
	}
	
	@Test
	public void run_test_createInterfaceA_constructor_int() {
		NameSpace.NameSpace2.NameSpace3.InterfaceA i = new NameSpace.NameSpace2.NameSpace3.InterfaceA() {
			{
				QtUtilities.initializeNativeObject(this, 
						QtArgument.begin(NameSpace.NameSpace2.NameSpace3.InterfaceA.class).add(28));
			}
			
			@Override
			public ObjectD fooBar2(ObjectC arg__1) {
				return null;
			}
			
			@Override
			public InterfaceA fooBar(InterfaceA arg__1) {
				return null;
			}
			
            @io.qt.QtUninvokable
            protected io.qt.autotests.generated.NameSpace.NameSpace2.NameSpace3.InterfaceA protectedFooBar(io.qt.autotests.generated.NameSpace.NameSpace2.NameSpace3.InterfaceA arg__1){
            	return arg__1;
            }
		};
		Assert.assertFalse(i.isDisposed());
		Assert.assertEquals(Integer.valueOf(28), i.constructorParameter1());
		Assert.assertEquals((Object)null, i.constructorParameter2());
	}
	
	@Test
	public void run_test_createInterfaceA_constructor_Collection() {
		NameSpace.NameSpace2.NameSpace3.InterfaceA i = new NameSpace.NameSpace2.NameSpace3.InterfaceA() {
			{
				QtUtilities.initializeNativeObject(this, 
						QtArgument.begin(NameSpace.NameSpace2.NameSpace3.InterfaceA.class).add(java.util.Collections.emptyList()));
			}
			
			@Override
			public ObjectD fooBar2(ObjectC arg__1) {
				return null;
			}
			
			@Override
			public InterfaceA fooBar(InterfaceA arg__1) {
				return null;
			}
			
            @io.qt.QtUninvokable
            protected io.qt.autotests.generated.NameSpace.NameSpace2.NameSpace3.InterfaceA protectedFooBar(io.qt.autotests.generated.NameSpace.NameSpace2.NameSpace3.InterfaceA arg__1){
            	return arg__1;
            }
		};
		Assert.assertFalse(i.isDisposed());
		Assert.assertTrue(i.constructorParameter1() instanceof java.util.Collection);
		Assert.assertEquals(0, i.constructorParameter2());
	}
	
	@Test
	public void run_test_createInterfaceA_constructor_Collection_int() {
		NameSpace.NameSpace2.NameSpace3.InterfaceA i = new NameSpace.NameSpace2.NameSpace3.InterfaceA() {
			{
				QtUtilities.initializeNativeObject(this, 
						QtArgument.begin(NameSpace.NameSpace2.NameSpace3.InterfaceA.class).add(java.util.Collections.emptyList()).add(28));
			}
			
			@Override
			public ObjectD fooBar2(ObjectC arg__1) {
				return null;
			}
			
			@Override
			public InterfaceA fooBar(InterfaceA arg__1) {
				return null;
			}
			
            @io.qt.QtUninvokable
            protected io.qt.autotests.generated.NameSpace.NameSpace2.NameSpace3.InterfaceA protectedFooBar(io.qt.autotests.generated.NameSpace.NameSpace2.NameSpace3.InterfaceA arg__1){
            	return arg__1;
            }
		};
		Assert.assertFalse(i.isDisposed());
		Assert.assertTrue(i.constructorParameter1() instanceof java.util.Collection);
		Assert.assertEquals(Integer.valueOf(28), i.constructorParameter2());
	}
	
	@Test
	public void run_test_createInterfaceA_constructor_QGraphicsItem() {
		NameSpace.NameSpace2.NameSpace3.InterfaceA i = new NameSpace.NameSpace2.NameSpace3.InterfaceA() {
			{
				QtUtilities.initializeNativeObject(this, 
						QtArgument.begin(NameSpace.NameSpace2.NameSpace3.InterfaceA.class).add(
								new QGraphicsItem() {
									@Override
									public QRectF boundingRect() {
										return null;
									}
					
									@Override
									public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {
									}
								}
							));
			}
			
			@Override
			public ObjectD fooBar2(ObjectC arg__1) {
				return null;
			}
			
			@Override
			public InterfaceA fooBar(InterfaceA arg__1) {
				return null;
			}
			
            @io.qt.QtUninvokable
            protected io.qt.autotests.generated.NameSpace.NameSpace2.NameSpace3.InterfaceA protectedFooBar(io.qt.autotests.generated.NameSpace.NameSpace2.NameSpace3.InterfaceA arg__1){
            	return arg__1;
            }
		};
		Assert.assertFalse(i.isDisposed());
		Assert.assertTrue(i.constructorParameter1() instanceof QGraphicsItem);
		Assert.assertEquals((Object)null, i.constructorParameter2());
	}
	
	@Test
	public void run_test_createInterfaceA_constructor_QObject() {
		QWidget wdg = new QWidget();
		NameSpace.NameSpace2.NameSpace3.InterfaceA i = new NameSpace.NameSpace2.NameSpace3.InterfaceA() {
			{
				QtUtilities.initializeNativeObject(this, 
						QtArgument.begin(NameSpace.NameSpace2.NameSpace3.InterfaceA.class).add(wdg));
			}
			
			@Override
			public ObjectD fooBar2(ObjectC arg__1) {
				return null;
			}
			
			@Override
			public InterfaceA fooBar(InterfaceA arg__1) {
				return null;
			}
			
            @io.qt.QtUninvokable
            protected io.qt.autotests.generated.NameSpace.NameSpace2.NameSpace3.InterfaceA protectedFooBar(io.qt.autotests.generated.NameSpace.NameSpace2.NameSpace3.InterfaceA arg__1){
            	return arg__1;
            }
		};
		Assert.assertFalse(i.isDisposed());
		Assert.assertEquals(wdg, i.constructorParameter1());
		Assert.assertEquals((Object)null, i.constructorParameter2());
	}
	
	private static class FunctionalObject extends QObject implements FunctionalTest.TestFunction1{
		@Override
		public int applyAsInt(int i, boolean b) {
			return b ? i*9 : -1;
		}
		
		FunctionalObject(){
		}
	}
	
	@Test
    public void test_QObject_inherit_functional() throws InterruptedException {
		AtomicInteger ai = new AtomicInteger();
		{
			FunctionalObject ft = new FunctionalObject();
			General.internalAccess.registerCleaner(ft, ai::incrementAndGet);
			ft.thread();
	    	FunctionalTest functionalTest = new FunctionalTest();
	    	assertEquals(4*9, functionalTest.convert(ft, 4, true));
	    	ApplicationInitializer.runGC();
	    	assertEquals(-1, functionalTest.convert(ft, 4, false));
	    	FunctionalTest.TestFunction1 last1 = functionalTest.last1();
	    	assertEquals(ft, last1);
	    	ApplicationInitializer.runGC();
			ft.thread();
	    	ft.dispose();
	    	assertEquals(0, functionalTest.convertWithLast1(5, true));
	    	try {
	    		functionalTest.convert(ft, 6, true);
	    		assertTrue("QNoNativeResourcesException expected", false);
	    	}catch (QNoNativeResourcesException e) {
	    		assertTrue("QNoNativeResourcesException expected", true);
			}
	    	assertEquals(0, ai.get());
	    	last1 = null;
	    	ft = null;
	    	ApplicationInitializer.runGC();
	    	Thread.sleep(20);
	    	ft = null;
		}
    	ApplicationInitializer.runGC();
    	Thread.sleep(20);
    	ApplicationInitializer.runGC();
    	Thread.sleep(20);
    	ApplicationInitializer.runGC();
    	Thread.sleep(20);
    	assertEquals(1, ai.get());
    	QApplication.processEvents();
    	QApplication.processEvents();
	}
	
	@Test
	public void test_customEasingCurve() {
		QEasingCurve ec = new QEasingCurve();
		QEasingCurve.EasingFunction lambda;
		ec.setCustomType(lambda = a -> {
			return a < 0.5 ? 0.2 : 0.8;
		});
		assertEquals(0.2, ec.valueForProgress(0.1), 0.01);
		assertEquals(0.8, ec.valueForProgress(0.7), 0.01);
		QEasingCurve.EasingFunction f1 = ec.customType();
		assertEquals(lambda, f1);
		QEasingCurve ec2 = ec.clone();
		QEasingCurve.EasingFunction f2 = ec2.customType();
		assertEquals(lambda, f2);
		assertEquals(0.2, ec2.valueForProgress(0.1), 0.01);
		assertEquals(0.8, ec2.valueForProgress(0.7), 0.01);
		f2.dispose();
		try {
			ec.valueForProgress(0.1);
			Assert.assertTrue(false);
		} catch (Exception e) {
			Assert.assertTrue(e instanceof NullPointerException);
		}
	}
	
	@Test
	public void test_manyEasingFunctions() {
		QList<QEasingCurve.EasingFunction> container = Tulip.createListOfEasingFunctions();
    	Assert.assertTrue(container!=null);
    	TreeMap<Integer,Double> calls = new TreeMap<>();
    	for(int i=0; i<100; ++i) {
    		int _i = i;
    		QEasingCurve.EasingFunction object = progress -> {
				calls.put(_i, progress);
				return 0;
    		};
    		container.add(object);
    	}
    	Tulip.testEasingFunctions(container);
        Assert.assertEquals(100, calls.size());
        for (int i = 0; i < calls.size(); i++) {
        	Assert.assertEquals(i+1, calls.get(i).intValue());
		}
	}
	
	static class Implementor{
		public double applyAsDouble(double d) {
			return d * 1.2345;
		}
	}
	
	static class EasingFunctionImpl extends Implementor implements QEasingCurve.EasingFunction {
	}
	
	static interface ImplInterface{
		public default double applyAsDouble(double d) {
			return d * 1.2345;
		}
	}
	
	@Test
	public void test_customEasingCurve_multiinheritance() {
		QEasingCurve ec = new QEasingCurve();
		ec.setCustomType(new EasingFunctionImpl());
		assertEquals(0.8*1.2345, ec.valueForProgress(0.8), 0.01);
	}
	
	interface CustomInterface extends QtObjectInterface { public void call(); }
	
	@Test
	public void test_InterfaceLiveCircle2() {
		QRunnable object = ()->{};
		Assert.assertFalse(object.isDisposed());
		object.autoDelete();
		object.dispose();
		assertTrue("object not destroyed.", TestUtility.tryIsObjectDisposed(object));
	}

	@Test
	public void test_InterfaceLiveCircle3() {
		QtObjectInterface object = new QtObjectInterface() {};
		assertTrue(object.isDisposed());
		object.dispose();
		assertTrue("object not destroyed.", TestUtility.tryIsObjectDisposed(object));
	}

	@Test
	public void test_InterfaceLiveCircle5() {
		CustomInterface object = ()->{};
		assertTrue(object.isDisposed());
		object.call();
		object.dispose();
		assertTrue("object not destroyed.", TestUtility.tryIsObjectDisposed(object));
	}

	@Test
	public void test_InterfaceLiveCircle6() {
		QtObject object = new QtObject() {};
		assertTrue(object.isDisposed());
		object.dispose();
		assertTrue("object not destroyed.", TestUtility.tryIsObjectDisposed(object));
	}
	
	static class VirtualOverrideTest extends TestAbstractClass implements TestInterface{
		
		public VirtualOverrideTest() {
			super(null);
			QtUtilities.initializeNativeObject(this, QtArgument.begin(TestAbstractClass.class).begin(TestInterface.class).add("QString arg"));
		}

		@Override
		public String method4() {
			return "VirtualOverrideTest::method4";
		}

		@Override
		@QtFinalOverride
		public String method2() {
			return "VirtualOverrideTest::method2";
		}
		
		@Override
		public String method3() {
			return "VirtualOverrideTest::method3";
		}

		@Override
		public boolean setReferenceCountTest1(QObject object) {
			return false;
		}
	}
	
	@Test
	public void test_VirtualOverride() {
		VirtualOverrideTest test = new VirtualOverrideTest();
		assertEquals("TestAbstractClass::method1", TestAbstractClass.callMethod1(test));
		assertEquals("VirtualOverrideTest::method2", TestAbstractClass.callMethod2(test));
		assertEquals("VirtualOverrideTest::method3", TestAbstractClass.callMethod3(test));
		assertEquals("VirtualOverrideTest::method4", TestAbstractClass.callMethod4(test));
		assertEquals("TestAbstractClass::method5", TestAbstractClass.callMethod5(test));
		assertEquals("TestInterface::method1", TestInterface.callMethod1(test));
		assertEquals("TestInterface::method2", TestInterface.callMethod2(test));
		assertEquals("VirtualOverrideTest::method3", TestInterface.callMethod3(test));
		assertEquals("VirtualOverrideTest::method4", TestInterface.callMethod4(test));
		assertEquals("TestAbstractClass::method5", TestInterface.callMethod5(test));
	}
	
	@Test
	public void test_castInterfaceFromQObject() {
		QObject object = TestAbstractClass.createInterfaceImplementingQObject();
		assertTrue(object instanceof QTimer);
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(object));
		TestInterface testInterface = QMetaObject.cast(TestInterface.class, object);
		assertTrue(testInterface instanceof TestInterface);
		assertEquals("InterfaceImplementingObject", testInterface.method4());
		testInterface.setReferenceCountTest1(null);
		assertTrue("not split ownership", General.internalAccess.isSplitOwnership(testInterface));
		assertTrue(object==QMetaObject.cast(QObject.class, testInterface));
		assertTrue(testInterface==QMetaObject.cast(TestInterface.class, object));
		testInterface.dispose();
		assertFalse(object.isDisposed());
		assertEquals("InterfaceImplementingObject", object.objectName());
		testInterface = QMetaObject.cast(TestInterface.class, object);
		assertTrue(testInterface instanceof TestInterface);
		assertFalse(testInterface.isDisposed());
		object.dispose();
		assertTrue(testInterface.isDisposed());
	}
	
	@Test
	public void test_castInterfaceToQObject() {
		TestInterface testInterface = TestAbstractClass.createQObjectInheritingInterface();
		assertTrue(testInterface instanceof TestInterface);
		assertEquals("InterfaceImplementingObject", testInterface.method4());
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(testInterface));
		QObject object = QMetaObject.cast(QObject.class, testInterface);
		assertTrue(object instanceof QTimer);
		assertTrue("not split ownership", General.internalAccess.isSplitOwnership(object));
		assertEquals("InterfaceImplementingObject", object.objectName());
		QScrollPrepareEvent event = new QScrollPrepareEvent(new QPointF(1,2));
		assertTrue(object.event(event));
		assertEquals(new QSizeF(20,30), event.viewportSize());
		assertEquals(new QPointF(5,5), event.contentPos());
		assertEquals(new QRectF(5,7,9,2), event.contentPosRange());
		assertTrue(testInterface==QMetaObject.cast(TestInterface.class, object));
		object.dispose();
		assertFalse(testInterface.isDisposed());
		object = QMetaObject.cast(QObject.class, testInterface);
		assertTrue(object instanceof QTimer);
		assertFalse(object.isDisposed());
		testInterface.dispose();
		assertTrue(object.isDisposed());
	}
}
