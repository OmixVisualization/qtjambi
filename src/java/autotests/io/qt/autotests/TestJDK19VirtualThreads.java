/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import io.qt.core.QMetaObject;
import io.qt.core.QThread;
import io.qt.autotests.generated.PerformanceTests;

public class TestJDK19VirtualThreads extends ApplicationInitializer {
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitializeWithWidgets();
		QMetaObject.forType(TestJDK19VirtualThreads.class);
    }
    
	@Test
    public void test() throws InterruptedException {
		QThread[] currentThreads = {QThread.currentThread(), null};
		Thread vthread = Thread.ofVirtual().start(
				()->{
					currentThreads[1] = QThread.currentThread();
				}
			);
		Assert.assertTrue(vthread.isVirtual());
		vthread.join();
		Assert.assertTrue(currentThreads[1]!=null);
		Assert.assertEquals(vthread, currentThreads[1].javaThread());
    }
	
	@Test
    public void testThreadLocal() throws InterruptedException {
		System.out.println("acquireThreadLocal in virtual Thread");
		System.out.flush();
		Thread vThread = Thread.ofVirtual().start(PerformanceTests::acquireThreadLocal);
		vThread.join();
	}
	
	@Test
    public void testThreadStorage() throws InterruptedException {
		System.out.println("acquireQThreadStorage in virtual Thread");
		System.out.flush();
		Thread vThread = Thread.ofVirtual().start(PerformanceTests::acquireQThreadStorage);
		vThread.join();
	}
	
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestJDK19VirtualThreads.class.getName());
    }
}
