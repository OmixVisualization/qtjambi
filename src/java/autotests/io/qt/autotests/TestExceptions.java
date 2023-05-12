/****************************************************************************
**
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

import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.autotests.generated.General;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QEventLoop;
import io.qt.core.QList;
import io.qt.core.QMetaObject;
import io.qt.core.QObject;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QRunnable;
import io.qt.core.QSysInfo;
import io.qt.core.QThread;
import io.qt.core.QThreadPool;
import io.qt.core.Qt;
import io.qt.gui.QPaintDevice;
import io.qt.gui.QPaintEngine;
import io.qt.widgets.QApplication;
import io.qt.widgets.QWizard;
import io.qt.widgets.QWizardPage;

public class TestExceptions extends ApplicationInitializer {
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitializeWithWidgets();
    }
	
	@Test
    public void test_runnable_run() {
		try {
			General.run(()->{
				throw new NegativeArraySizeException();
			});
			Assert.assertFalse(true);
		} catch (NegativeArraySizeException e) {
		}
		try {
			General.run(QThread.create(()->{
				throw new NegativeArraySizeException();
			}));
			Assert.assertFalse(true);
		} catch (NegativeArraySizeException e) {
		}
	}
	
    @Test
    public void test_multi_inherited_QRunnable_QObject_Deletion() {
    	{
			QObject parent = new QObject();
			class MultiImpl extends QObject implements QRunnable, QPaintDevice{
			    public MultiImpl() {
					super();
				}

			    @SuppressWarnings("unused")
				MultiImpl(QDeclarativeConstructor dc) throws IllegalAccessException {
					super(dc);
				}

			    @SuppressWarnings("unused")
				public MultiImpl(QObject parent) {
					super(parent);
				}

			    @SuppressWarnings("unused")
				MultiImpl(QPrivateConstructor p) {
					super(p);
				}

				public void run() {
//			    	System.out.println("TestInterfaces.test_MultiInterfaceImpl() "+Thread.currentThread()+" / object thread "+this.thread());
			    	throw new RuntimeException("Run Exception");
			    }

				@Override
				public QPaintEngine paintEngine() {
					return null;
				}
			}
			MultiImpl object = new MultiImpl();
			object.setParent(parent);
//			object.destroyed.connect(()->{
//				System.out.println("disposing QObject in thread "+Thread.currentThread());
//			});
			Assert.assertFalse(object.isDisposed());
			object.objectName();
			object.paintingActive();
			object.autoDelete();
			QThreadPool pool = new QThreadPool();
			pool.start(object);
			pool.waitForDone();
			Assert.assertFalse(object.isDisposed());
			QList<QObject> children = parent.children();
			Assert.assertEquals(object, children.get(0));
			try {
				object.paintingActive();
				Assert.assertTrue(false);
			} catch (Exception e) {
				Assert.assertTrue(e instanceof io.qt.QNoNativeResourcesException);
			}
			QApplication.sendPostedEvents(null, io.qt.core.QEvent.Type.DeferredDispose.value());
			QApplication.processEvents();
			QApplication.processEvents();
			QApplication.processEvents();
			QApplication.processEvents();
			Assert.assertTrue(object.isDisposed());
			Assert.assertEquals(0, parent.children().size());
		}
    }
    
    @Test
    public void test_multi_inherited_Runnable_QObject_Deletion() {
    	{
			QObject parent = new QObject();
			class MultiImpl extends QObject implements Runnable, QPaintDevice{
			    public MultiImpl() {
					super();
				}

			    @SuppressWarnings("unused")
				MultiImpl(QDeclarativeConstructor dc) throws IllegalAccessException {
					super(dc);
				}

			    @SuppressWarnings("unused")
				public MultiImpl(QObject parent) {
					super(parent);
				}

			    @SuppressWarnings("unused")
				MultiImpl(QPrivateConstructor p) {
					super(p);
				}

				public void run() {
//			    	System.out.println("TestInterfaces.test_MultiInterfaceImpl() "+Thread.currentThread()+" / object thread "+this.thread());
			    	throw new RuntimeException("Run Exception");
			    }

				@Override
				public QPaintEngine paintEngine() {
					return null;
				}
			}
			MultiImpl object = new MultiImpl();
			object.setParent(parent);
//			object.destroyed.connect(()->{
//				System.out.println("disposing QObject in thread "+Thread.currentThread());
//			});
			Assert.assertFalse(object.isDisposed());
			object.objectName();
			object.paintingActive();
			QThreadPool pool = new QThreadPool();
			pool.start(object);
			pool.waitForDone();
			Assert.assertFalse(object.isDisposed());
			QList<QObject> children = parent.children();
			Assert.assertEquals(object, children.get(0));
			object.paintingActive();
			QApplication.sendPostedEvents(null, io.qt.core.QEvent.Type.DeferredDispose.value());
			QApplication.processEvents();
			QApplication.processEvents();
			QApplication.processEvents();
			QApplication.processEvents();
			object.paintingActive();
			Assert.assertFalse(object.isDisposed());
			Assert.assertEquals(1, parent.children().size());
		}
    }
    	
    @Test
    public void testExceptionDuringEvent_threaded() {
    	{
			QThread t = new QThread();
			class ExnEventObject extends QObject{
				@Override
				public boolean event(QEvent event) {
					if(event.type()==QEvent.Type.WinEventAct) {
						disposeLater();
						throw new RuntimeException("Event Exception");
					}
					if(event.type()==QEvent.Type.DeferredDispose) {
						t.quit();
					}
					return super.event(event);
				}
			}
			ExnEventObject o = new ExnEventObject();
			o.moveToThread(t);
			QCoreApplication.postEvent(o, new QEvent(QEvent.Type.WinEventAct));
			t.start();
			t.join();
		}
    }
    
    static class SimpleExnEventObject extends QObject{
		@Override
		public boolean event(QEvent event) {
			if(event.type()==QEvent.Type.WinEventAct) {
				throw new RuntimeException("Event Exception");
			}
			return super.event(event);
		}
	}
    
    @Test
    public void testExceptionDuringEvent_send() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	SimpleExnEventObject o = new SimpleExnEventObject();
		try{
			QCoreApplication.sendEvent(o, new QEvent(QEvent.Type.WinEventAct));
			Assert.fail("Exception expected to be thrown.");
		}catch(RuntimeException e) {
			Assert.assertEquals("Event Exception", e.getMessage());
		}
    }
    
    @Test
    public void testExceptionDuringEvent_posted() {
    	Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	SimpleExnEventObject o = new SimpleExnEventObject();
		QCoreApplication.postEvent(o, new QEvent(QEvent.Type.WinEventAct));
		try{
			QCoreApplication.sendPostedEvents(o);
			Assert.fail("Exception expected to be thrown.");
		}catch(RuntimeException e) {
			Assert.assertEquals("Event Exception", e.getMessage());
		}
    }
    
    @Test
    public void testExceptionDuringEvent_notify() {
    	SimpleExnEventObject o = new SimpleExnEventObject();
		try {
			QCoreApplication.instance().notify(o, new QEvent(QEvent.Type.WinEventAct));
			Assert.fail("Exception expected to be thrown.");
		}catch(RuntimeException e) {
			Assert.assertEquals("Event Exception", e.getMessage());
		}
    }
    
    @Test
    public void testExceptionDuringEvent_eventLoop() {
    	SimpleExnEventObject o = new SimpleExnEventObject();
    	QCoreApplication.postEvent(o, new QEvent(QEvent.Type.WinEventAct));
    	QEventLoop loop = new QEventLoop();
    	QMetaObject.invokeMethod(loop::quit, Qt.ConnectionType.QueuedConnection);
    	int result = loop.exec();
		Assert.assertEquals(-1, result);
    }
    
    @Test
    public void testExceptionDuringEvent_threaded_handled() {
    	QThread thread = new QThread();
    	Throwable[] thrown = {null};
    	thread.setUncaughtExceptionHandler((Thread t, Throwable e)->{
    		thrown[0] = e;
    		QThread.thread(t).exit(-1);
    	});
    	thread.moveToThread(thread);
    	SimpleExnEventObject o = new SimpleExnEventObject();
    	o.moveToThread(thread);
    	QCoreApplication.postEvent(o, new QEvent(QEvent.Type.WinEventAct));
    	thread.start();
    	Assert.assertTrue(thread.join(15000));
		Assert.assertTrue(thrown[0]!=null);
		Assert.assertEquals("Event Exception", thrown[0].getMessage());
    }
    
    @Test
    public void testExceptionDuringEvent_eventLoop_handled() {
    	Throwable[] thrown = {null};
    	QThread.currentThread().setUncaughtExceptionHandler((Thread t, Throwable e)->{
    		thrown[0] = e;
    		QThread.thread(t).exit(-1);
    	});
    	try {
	    	SimpleExnEventObject o = new SimpleExnEventObject();
	    	QCoreApplication.postEvent(o, new QEvent(QEvent.Type.WinEventAct));
	    	QEventLoop loop = new QEventLoop();
	    	int result = loop.exec();
			Assert.assertEquals(-1, result);
			Assert.assertTrue(thrown[0]!=null);
			Assert.assertEquals("Event Exception", thrown[0].getMessage());
    	}finally {
    		QThread.currentThread().setUncaughtExceptionHandler(null);
    	}
    }
    
    @Test
    public void testExceptionDuringExec() {
    	Assume.assumeTrue("Can only run successfully on x86_64", "x86_64".equals(QSysInfo.currentCpuArchitecture()));
    	QWizard dialog = new QWizard();
    	dialog.addPage(new QWizardPage(){

			@Override
			public void initializePage() {
				throw new RuntimeException("Exec Exception");
			}
    	});
    	try {
    		dialog.exec();
			Assert.fail("Exception expected to be thrown.");
		}catch(RuntimeException e) {
			Assert.assertEquals("Exec Exception", e.getMessage());
    	}
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestExceptions.class.getName());
    }
}
