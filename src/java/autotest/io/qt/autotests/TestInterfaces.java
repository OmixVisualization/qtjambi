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

import java.util.List;
import java.util.Map;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;

import org.junit.After;
import org.junit.Assert;
import org.junit.Assume;
import org.junit.Before;
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
import io.qt.autotests.generated.NameSpace;
import io.qt.autotests.generated.NameSpace.NameSpace2.NameSpace3.InterfaceA;
import io.qt.autotests.generated.NameSpace.NameSpace2.NameSpace3.ObjectC;
import io.qt.autotests.generated.NameSpace.NameSpace2.NameSpace3.ObjectD;
import io.qt.autotests.generated.TestAbstractClass;
import io.qt.autotests.generated.TestInterface;
import io.qt.autotests.generated.TestPrivateInterface;
import io.qt.core.QEasingCurve;
import io.qt.core.QEvent;
import io.qt.core.QEventLoop;
import io.qt.core.QFactoryInterface;
import io.qt.core.QObject;
import io.qt.core.QRect;
import io.qt.core.QRectF;
import io.qt.core.QRunnable;
import io.qt.core.QSize;
import io.qt.core.QThread;
import io.qt.core.QTimer;
import io.qt.core.Qt;
import io.qt.gui.QAbstractUndoItem;
import io.qt.gui.QColor;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QPaintDevice;
import io.qt.gui.QPaintEngine;
import io.qt.gui.QPaintEngineState;
import io.qt.gui.QPainter;
import io.qt.gui.QPainterPath;
import io.qt.gui.QPixmap;
import io.qt.gui.QSurface;
import io.qt.gui.QSurfaceFormat;
import io.qt.internal.QtJambiInternal;
import io.qt.network.auth.QAbstractOAuth.ModifyParametersFunction;
import io.qt.network.auth.QAbstractOAuth.Stage;
import io.qt.qml.QQmlImageProviderBase;
import io.qt.qml.QtQml;
import io.qt.widgets.QApplication;
import io.qt.widgets.QGraphicsItem;
import io.qt.widgets.QGraphicsScene;
import io.qt.widgets.QGraphicsView;
import io.qt.widgets.QLayoutItem;
import io.qt.widgets.QStyleOptionGraphicsItem;
import io.qt.widgets.QWidget;

public class TestInterfaces extends QApplicationTest {
	
	public static class IllegalInterfaceImplementation extends QObject implements QQmlImageProviderBase {
		@Override
		public Flags flags() {
			return new QQmlImageProviderBase.Flags();
		}

		@Override
		public ImageType imageType() {
			return QQmlImageProviderBase.ImageType.Image;
		}
		
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
    public void testInterface() {
		Assume.assumeThat(QGuiApplication.primaryScreen()!=null, QApplicationTest.trueMatcher("A screen is required to create a window."));
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
	
	@Test(expected=QInterfaceCannotBeSubclassedException.class)
    public void run_test_InterfaceCannotBeSubclassed() {
		new IllegalInterfaceImplementation();
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
		};
		Assert.assertFalse(i.isDisposed());
		Assert.assertEquals((Object)null, i.constructorParameter1());
		Assert.assertEquals((Object)null, i.constructorParameter2());
	}
	
	public void run_test_createInterfaceA_constructor_int() {
		NameSpace.NameSpace2.NameSpace3.InterfaceA i = new NameSpace.NameSpace2.NameSpace3.InterfaceA() {
//			@SuppressWarnings("unused")
//			private final QtConstructor.Argument[] param = {new QtConstructor.Argument(28)};
			
			@Override
			public ObjectD fooBar2(ObjectC arg__1) {
				return null;
			}
			
			@Override
			public InterfaceA fooBar(InterfaceA arg__1) {
				return null;
			}
		};
		Assert.assertFalse(i.isDisposed());
		Assert.assertEquals(Integer.valueOf(28), i.constructorParameter1());
		Assert.assertEquals((Object)null, i.constructorParameter2());
	}
	
	public void run_test_createInterfaceA_constructor_Collection() {
		NameSpace.NameSpace2.NameSpace3.InterfaceA i = new NameSpace.NameSpace2.NameSpace3.InterfaceA() {
//			@SuppressWarnings("unused")
//			private final QtConstructor.Argument[] param = {new QtConstructor.Argument(Collections.emptyList())};
			
			@Override
			public ObjectD fooBar2(ObjectC arg__1) {
				return null;
			}
			
			@Override
			public InterfaceA fooBar(InterfaceA arg__1) {
				return null;
			}
		};
		Assert.assertFalse(i.isDisposed());
		Assert.assertTrue(i.constructorParameter1() instanceof java.util.Collection);
		Assert.assertEquals((Object)null, i.constructorParameter2());
	}
	
	public void run_test_createInterfaceA_constructor_Collection_int() {
		NameSpace.NameSpace2.NameSpace3.InterfaceA i = new NameSpace.NameSpace2.NameSpace3.InterfaceA() {
//			@SuppressWarnings("unused")
//			private final QtConstructor.Argument[] param = {new QtConstructor.Argument(Collections.emptyList()), new QtConstructor.Argument(28)};
			
			@Override
			public ObjectD fooBar2(ObjectC arg__1) {
				return null;
			}
			
			@Override
			public InterfaceA fooBar(InterfaceA arg__1) {
				return null;
			}
		};
		Assert.assertFalse(i.isDisposed());
		Assert.assertTrue(i.constructorParameter1() instanceof java.util.Collection);
		Assert.assertEquals(Integer.valueOf(28), i.constructorParameter2());
	}
	
	public void run_test_createInterfaceA_constructor_QGraphicsItem() {
		NameSpace.NameSpace2.NameSpace3.InterfaceA i = new NameSpace.NameSpace2.NameSpace3.InterfaceA() {
//			@SuppressWarnings("unused")
//			private final QtConstructor.Argument[] param = {new QtConstructor.Argument(new QGraphicsItem() {
//				@Override
//				public QRectF boundingRect() {
//					return null;
//				}
//
//				@Override
//				public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {
//				}})};
			
			@Override
			public ObjectD fooBar2(ObjectC arg__1) {
				return null;
			}
			
			@Override
			public InterfaceA fooBar(InterfaceA arg__1) {
				return null;
			}
		};
		Assert.assertFalse(i.isDisposed());
		Assert.assertTrue(i.constructorParameter1() instanceof QGraphicsItem);
		Assert.assertEquals((Object)null, i.constructorParameter2());
	}
	
	public void run_test_createInterfaceA_constructor_QObject() {
		QWidget wdg = new QWidget();
		NameSpace.NameSpace2.NameSpace3.InterfaceA i = new NameSpace.NameSpace2.NameSpace3.InterfaceA() {
//			@SuppressWarnings("unused")
//			private final QtConstructor.Argument[] param = {new QtConstructor.Argument(wdg)};
			
			@Override
			public ObjectD fooBar2(ObjectC arg__1) {
				return null;
			}
			
			@Override
			public InterfaceA fooBar(InterfaceA arg__1) {
				return null;
			}
		};
		Assert.assertFalse(i.isDisposed());
		Assert.assertEquals(wdg, i.constructorParameter1());
		Assert.assertEquals((Object)null, i.constructorParameter2());
	}
	
	private static class FunctionalObject extends QObject implements FunctionalTest.TestFunction1{
		@Override
		public int call(int i, boolean b) {
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
			QtJambiInternal.registerCleaner(ft, ai::incrementAndGet);
			ft.thread();
	    	FunctionalTest functionalTest = new FunctionalTest();
	    	assertEquals(4*9, functionalTest.convert(ft, 4, true));
	    	System.gc();
	    	assertEquals(-1, functionalTest.convert(ft, 4, false));
	    	FunctionalTest.TestFunction1 last1 = functionalTest.last1();
	    	assertEquals(ft, last1);
	    	System.gc();
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
	    	System.gc();
	    	System.runFinalization();
	    	Thread.sleep(20);
	    	ft = null;
		}
    	System.gc();
    	System.runFinalization();
    	System.gc();
    	System.runFinalization();
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
	
	interface CustomInterface extends QtObjectInterface { public void call(); }
	
	@Test
	public void test_MultiInterfaceImpl() {
		class MultiImpl extends QtObject implements QLayoutItem, QPaintDevice, ModifyParametersFunction, QEasingCurve.EasingFunction, QtQml.ObjectCallback{

			@Override
			public void call(Stage arg__1, Map<String, Object> arg__2) {
			}
			
			@Override
			public double call(double v) {
				return v;
			}
			
			@Override
			public io.qt.core.QObject call(io.qt.qml.QQmlEngine arg__1, io.qt.qml.QJSEngine arg__2){
				return null;
			}

			@Override
			public Qt.Orientations expandingDirections() {
				return new Qt.Orientations(0);
			}

			@Override
			public QRect geometry() {
				return new QRect();
			}

			@Override
			public boolean isEmpty() {
				return false;
			}

			@Override
			public QSize maximumSize() {
				return new QSize();
			}

			@Override
			public QSize minimumSize() {
				return new QSize();
			}

			@Override
			public void setGeometry(QRect arg__1) {
			}

			@Override
			public QSize sizeHint() {
				return new QSize();
			}

			@Override
			public QPaintEngine paintEngine() {
				return null;
			}
		}
		MultiImpl object = new MultiImpl();
		Assert.assertFalse(object.isDisposed());
		object.alignment();
		object.paintingActive();
		object.dispose();
		Assert.assertTrue("object not destroyed.", QtJambiInternal.tryIsObjectDisposed(object));
	}
	
	@Test
	public void test_InterfaceLiveCircle2() {
		QRunnable object = ()->{};
		Assert.assertFalse(object.isDisposed());
		object.autoDelete();
		object.dispose();
		Assert.assertTrue("object not destroyed.", QtJambiInternal.tryIsObjectDisposed(object));
	}

	@Test
	public void test_InterfaceLiveCircle3() {
		QtObjectInterface object = new QtObjectInterface() {};
		Assert.assertTrue(object.isDisposed());
		object.dispose();
		Assert.assertTrue("object not destroyed.", QtJambiInternal.tryIsObjectDisposed(object));
	}

	@Test
	public void test_InterfaceLiveCircle5() {
		CustomInterface object = ()->{};
		Assert.assertTrue(object.isDisposed());
		object.call();
		object.dispose();
		Assert.assertTrue("object not destroyed.", QtJambiInternal.tryIsObjectDisposed(object));
	}

	@Test
	public void test_InterfaceLiveCircle6() {
		QtObject object = new QtObject() {};
		Assert.assertTrue(object.isDisposed());
		object.dispose();
		Assert.assertTrue("object not destroyed.", QtJambiInternal.tryIsObjectDisposed(object));
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
		Assert.assertEquals("TestAbstractClass::method1", TestAbstractClass.callMethod1(test));
		Assert.assertEquals("VirtualOverrideTest::method2", TestAbstractClass.callMethod2(test));
		Assert.assertEquals("VirtualOverrideTest::method3", TestAbstractClass.callMethod3(test));
		Assert.assertEquals("VirtualOverrideTest::method4", TestAbstractClass.callMethod4(test));
		Assert.assertEquals("TestAbstractClass::method5", TestAbstractClass.callMethod5(test));
		Assert.assertEquals("TestInterface::method1", TestInterface.callMethod1(test));
		Assert.assertEquals("TestInterface::method2", TestInterface.callMethod2(test));
		Assert.assertEquals("VirtualOverrideTest::method3", TestInterface.callMethod3(test));
		Assert.assertEquals("VirtualOverrideTest::method4", TestInterface.callMethod4(test));
		Assert.assertEquals("TestAbstractClass::method5", TestInterface.callMethod5(test));
	}
}
