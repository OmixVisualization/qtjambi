/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QThreadAffinityException;
import io.qt.core.QThread;
import io.qt.qml.QQmlEngine;
import io.qt.qml.QtQml;

public class TestQmlThreadAffinityQt65 extends ApplicationInitializer{
	
	static {
		System.setProperty("io.qt.enable-thread-affinity-check", "true");
		System.setProperty("io.qt.enable-event-thread-affinity-check", "true");
	}
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithGui();
    }
	
	@Test
    public void testQQmlEngine() throws Throwable{
		QtQml.qmlClearTypeRegistrations();
		QQmlEngine engine = new QQmlEngine();
		try {
			Throwable[] exn = {null};
			Runnable r = ()->{
				try {
					try {
						engine.singletonInstance("", "");
						Assert.fail("QThreadAffinityException expected to be thrown");
					} catch (QThreadAffinityException e) {
					}
				}catch(Throwable e){
					exn[0] = e;
				}
			};
			QThread thread1 = QThread.create(r);
			thread1.start();
			thread1.join(5000);
			if(exn[0]!=null)
				throw exn[0];
		}finally {
			engine.dispose();
		}
	}
	
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQmlThreadAffinityQt65.class.getName());
    }
}
