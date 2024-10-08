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

import io.qt.core.QObject;
import io.qt.core.QStaticMemberSignals;
import io.qt.core.Qt;

public class TestSignalsQt6 extends ApplicationInitializer {
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitialize();
    }
	
	static class Receiver extends QObject{
		AtomicInteger count = new AtomicInteger();
		void increment() {
			count.incrementAndGet();
		}
	}
	
	@Test
    public void testNativeSignalToSlot() {
		Receiver receiver = new Receiver();
		QObject object = new QObject();
		object.objectNameChanged.connect(receiver, "increment()", Qt.ConnectionType.SingleShotConnection);
		Assert.assertEquals(0, receiver.count.get());
		object.setObjectName("Test1");
		Assert.assertEquals(1, receiver.count.get());
		object.setObjectName("Test2");
		Assert.assertEquals(1, receiver.count.get());
    }
	
	@Test
    public void testNativeSignalToLambda() {
		AtomicInteger count = new AtomicInteger();
		QObject object = new QObject();
		object.objectNameChanged.connect(count::incrementAndGet, Qt.ConnectionType.SingleShotConnection);
		Assert.assertEquals(0, count.get());
		object.setObjectName("Test1");
		Assert.assertEquals(1, count.get());
		object.setObjectName("Test2");
		Assert.assertEquals(1, count.get());
    }
	
	static class StaticSender{
		static final QStaticMemberSignals.Signal0 signal = new QStaticMemberSignals.Signal0();
	}
	
	@Test
    public void testJavaSignalToLambda() {
		AtomicInteger count = new AtomicInteger();
		StaticSender.signal.connect(count::incrementAndGet, Qt.ConnectionType.SingleShotConnection);
		Assert.assertEquals(0, count.get());
		StaticSender.signal.emit();
		Assert.assertEquals(1, count.get());
		StaticSender.signal.emit();
		Assert.assertEquals(1, count.get());
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestSignalsQt6.class.getName());
    }
}
