/****************************************************************************
**
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

import java.util.List;

import org.junit.Assert;
import org.junit.Test;

import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QObject;
import io.qt.core.QRunnable;
import io.qt.core.QThread;
import io.qt.core.QThreadPool;
import io.qt.script.QScriptable;
import io.qt.widgets.QApplication;

public class TestExceptions extends QApplicationTest {
	
    @Test
    public void test() {
    	{
			QObject parent = new QObject();
			class MultiImpl2 extends QObject implements QRunnable, QScriptable{
			    public MultiImpl2() {
					super();
				}

			    @SuppressWarnings("unused")
				MultiImpl2(QDeclarativeConstructor dc) throws IllegalAccessException {
					super(dc);
				}

			    @SuppressWarnings("unused")
				public MultiImpl2(QObject parent) {
					super(parent);
				}

			    @SuppressWarnings("unused")
				MultiImpl2(QPrivateConstructor p) {
					super(p);
				}

				public void run() {
			    	System.out.println("TestInterfaces.test_MultiInterfaceImpl() "+Thread.currentThread()+" / object thread "+this.thread());
			    	throw new RuntimeException();
			    }
			}
			MultiImpl2 object = new MultiImpl2();
			object.setParent(parent);
			object.destroyed.connect(()->{
				System.out.println("disposing QObject in thread "+Thread.currentThread());
			});
			Assert.assertFalse(object.isDisposed());
			object.objectName();
			object.argumentCount();
			object.autoDelete();
			QThreadPool pool = new QThreadPool();
			pool.start(object);
			pool.waitForDone();
			Assert.assertFalse(object.isDisposed());
			List<QObject> children = parent.children();
			Assert.assertEquals(object, children.get(0));
			try {
				object.argumentCount();
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
    	
    	{
			QThread t = new QThread();
			class ExnEventObject extends QObject{
				@Override
				public boolean event(QEvent event) {
					if(event.type()==QEvent.Type.WinEventAct) {
						disposeLater();
						throw new RuntimeException();
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
			try {
				t.join();
			} catch (Throwable e) {
				e.printStackTrace();
			}
		}
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestExceptions.class.getName());
    }
}
