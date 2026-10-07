/****************************************************************************
**
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

import org.junit.Assert;
import org.junit.Test;

import io.qt.*;
import io.qt.core.*;
import io.qt.qml.*;

public class TestMetacallsQml extends ApplicationInitializer {
	
	static class SuperClass extends QObject{
		static final RuntimeException exn = new RuntimeException();
		public String getText() {
			throw exn;
		}
		public String invoke() {
			throw exn;
		}
	}
	
	static class SubClass extends SuperClass{
//		private int number = 5;
//
//		public int getNumber() {
//			return number;
//		}
//
//		public void setNumber(int number) {
//			this.number = number;
//		}
		private String text2 = "SubClassTest";

		public String getText2() {
			return text2;
		}

		public void setText2(String text2) {
			this.text2 = text2;
		}
		public String invoke2() {
			return getText2();
		}
	}
	
	@Test
    public void testMethodException() {
		Throwable[] thrown = {null};
		Thread.currentThread().setUncaughtExceptionHandler((Thread t, Throwable e)->{
			thrown[0] = e;
		});
		QQmlEngine engine = new QQmlEngine();
		try{
			SubClass subClass = new SubClass();
			class PeriodicIncubationController extends QObject implements QQmlIncubationController {
				private final int timer;
				public PeriodicIncubationController() {
					timer = startTimer(16);
				}
				public void stop() {
					killTimer(timer);
					disposeLater();
				}
				protected void timerEvent(QTimerEvent e){
					incubateFor(5);
				}
			};
			PeriodicIncubationController control = new PeriodicIncubationController();
			engine.setIncubationController(control);
			engine.rootContext().setContextProperty("SubClass", subClass);
			QQmlComponent component = new QQmlComponent(engine);
			component.setData("import QtQml; QtObject{property var prop : SubClass.invoke();}", ":/");
			Assert.assertFalse(component.errorString(), component.isError());
			QEventLoop loop = new QEventLoop();
			QQmlIncubator incubator = new QQmlIncubator(QQmlIncubator.IncubationMode.Asynchronous){
				@Override
				protected void statusChanged(@NonNull Status status) {
					switch(status) {
					case Loading:
						break;
					case Null:
						break;
					case Error:
					case Ready:
						loop.quit();
						break;
					default:
						break;
					}
				}
			};
			component.create(incubator);
			if(incubator.isLoading())
				loop.exec();
			engine.setIncubationController(null);
			control.stop();
			QObject object = incubator.object();
			Assert.assertEquals("", object.property("prop"));
			Assert.assertEquals(SuperClass.exn, thrown[0]);
		}finally {
			engine.dispose();
			Thread.currentThread().setUncaughtExceptionHandler(null);
		}
	}
	
	@Test
    public void testPropertyException() {
		Throwable[] thrown = {null};
		Thread.currentThread().setUncaughtExceptionHandler((Thread t, Throwable e)->{
			thrown[0] = e;
		});
		QQmlEngine engine = new QQmlEngine();
		try{
			SubClass subClass = new SubClass();
			class PeriodicIncubationController extends QObject implements QQmlIncubationController {
				private final int timer;
				public PeriodicIncubationController() {
					timer = startTimer(16);
				}
				public void stop() {
					killTimer(timer);
					disposeLater();
				}
				protected void timerEvent(QTimerEvent e){
					incubateFor(5);
				}
			};
			PeriodicIncubationController control = new PeriodicIncubationController();
			engine.setIncubationController(control);
			engine.rootContext().setContextProperty("SubClass", subClass);
			QQmlComponent component = new QQmlComponent(engine);
			component.setData("import QtQml; QtObject{property var prop : SubClass.text;}", ":/");
			Assert.assertFalse(component.errorString(), component.isError());
			QEventLoop loop = new QEventLoop();
			QQmlIncubator incubator = new QQmlIncubator(QQmlIncubator.IncubationMode.Asynchronous){
				@Override
				protected void statusChanged(@NonNull Status status) {
					switch(status) {
					case Loading:
						break;
					case Null:
						break;
					case Error:
					case Ready:
						loop.quit();
						break;
					default:
						break;
					}
				}
			};
			component.create(incubator);
			if(incubator.isLoading())
				loop.exec();
			engine.setIncubationController(null);
			control.stop();
			QObject object = incubator.object();
			Assert.assertEquals("", object.property("prop"));
			Assert.assertEquals(SuperClass.exn, thrown[0]);
		}finally {
			engine.dispose();
			Thread.currentThread().setUncaughtExceptionHandler(null);
		}
	}

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestMetacallsQml.class.getName());
    }
}
