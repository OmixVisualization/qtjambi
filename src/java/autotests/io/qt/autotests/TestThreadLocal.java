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

import org.junit.Test;

import io.qt.autotests.generated.PerformanceTests;
import io.qt.core.QThread;

public class TestThreadLocal extends ApplicationInitializer{
    
    @Test
    public void testThreadLocal() throws InterruptedException {
		System.out.println("acquireThreadLocal in QThread");
		System.out.flush();
		QThread qThread = QThread.create(PerformanceTests::acquireThreadLocal);
		qThread.start();
		qThread.join(2000);
		System.out.flush();
		System.out.println("acquireThreadLocal in Java Thread");
		System.out.flush();
		Thread jThread = new Thread(()->{
			QThread.currentThread();
			PerformanceTests.acquireThreadLocal();
		});
		jThread.start();
		jThread.join(2000);
	}
	
	@Test
    public void testThreadStorage() throws InterruptedException {
		System.out.println("acquireQThreadStorage in QThread");
		System.out.flush();
		QThread qThread = QThread.create(PerformanceTests::acquireQThreadStorage);
		qThread.start();
		qThread.join(2000);
		System.out.flush();
		System.out.println("acquireQThreadStorage in Java Thread");
		System.out.flush();
		Thread jThread = new Thread(()->{
			QThread.currentThread();
			PerformanceTests.acquireQThreadStorage();
		});
		jThread.start();
		jThread.join(2000);
	}

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestThreadLocal.class.getName());
    }
}
