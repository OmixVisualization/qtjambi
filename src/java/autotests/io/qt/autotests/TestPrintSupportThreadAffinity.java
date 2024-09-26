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

import java.util.concurrent.atomic.AtomicInteger;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtUtilities;
import io.qt.core.QEventLoop;
import io.qt.core.QThread;
import io.qt.printsupport.QPrinter;
import io.qt.printsupport.QPrinterInfo;

public class TestPrintSupportThreadAffinity extends ApplicationInitializer{
	
	static {
		System.setProperty("io.qt.enable-thread-affinity-check", "true");
		System.setProperty("io.qt.enable-event-thread-affinity-check", "true");
	}
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
	@Test
    public void testQPrinter() throws InterruptedException {
		int COUNT = 10;
		AtomicInteger counter = new AtomicInteger();
		QEventLoop loop = new QEventLoop();
		QPrinterInfo pi = QPrinterInfo.defaultPrinter();
		QThread thread = QThread.create(()->{
			for (int i = 0; i < COUNT; i++) {
				QPrinter value = new QPrinter(pi);
				QtUtilities.getSignalOnDispose(value).connect(counter::incrementAndGet);
			}
		});
		thread.finished.connect(loop::quit);
		thread.start();
		loop.exec();
		for (int i = 0; i < 20 && counter.get()!=COUNT; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(100);
            }
		}
        Assert.assertEquals(COUNT, counter.get());
	}

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestPrintSupportThreadAffinity.class.getName());
    }
}
