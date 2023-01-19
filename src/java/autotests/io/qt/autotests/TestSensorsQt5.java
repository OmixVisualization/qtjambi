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

import java.util.Collections;

import org.junit.Assert;
import org.junit.Test;

import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaObject.AbstractPublicSignal0;
import io.qt.core.QMetaObject.AbstractSignal;
import io.qt.sensors.QSensorGesture;
import io.qt.sensors.QSensorGestureManager;

public class TestSensorsQt5 extends ApplicationInitializer {
	
	private static class SensorGestureSubclass1 extends QSensorGesture{
		public SensorGestureSubclass1() {
			super(Collections.emptyList());
		}
	}
	
	private static class SensorGestureSubclass2 extends QSensorGesture{
		@SuppressWarnings("unused")
		public final Signal0 testSignal = new Signal0();
		public SensorGestureSubclass2() {
			super(Collections.emptyList());
		}
	}
	
	@Test
    public void testSubclassing() {
		Assert.assertEquals(QMetaObject.forType(QSensorGesture.class), QSensorGesture.staticMetaObject);
		Assert.assertEquals(QMetaObject.forType(SensorGestureSubclass1.class), QSensorGesture.staticMetaObject);
		new SensorGestureSubclass1();
		try {
			QMetaObject.forType(SensorGestureSubclass2.class);
			Assert.fail("UnsupportedOperationException expected to be thrown");
		}catch(UnsupportedOperationException e) {}
		try {
			new SensorGestureSubclass2();
			Assert.fail("UnsupportedOperationException expected to be thrown");
		}catch(UnsupportedOperationException e) {}
	}
	
    @Test
    public void test() {
    	// Create a QSensorGestureManager
        QSensorGestureManager gestureManager = new QSensorGestureManager();

        // Create a QSensorGeture object for each of those gesture recognizers
        QSensorGesture gesture = new QSensorGesture(gestureManager.gestureIds());
        // Connect the known signals up.
        String[] detected = {null};
        gesture.detected.connect(d->detected[0] = d);
        gesture.detected.emit("test");
        Assert.assertEquals("test", detected[0]);
        for(QMetaMethod method : gesture.metaObject().methods()) {
        	if("detected".equals(method.name().toString())) {
        		AbstractSignal sig = method.toSignal(gesture);
        		Assert.assertTrue(sig==gesture.detected);
        	}
        }
        for(String signal : gesture.gestureSignals()) {
        	QMetaMethod method = gesture.metaObject().method(signal);
        	Assert.assertTrue(signal, method.isValid());
        	AbstractSignal sig = method.toSignal(gesture);
        	Assert.assertTrue(sig!=null);
        	if(sig instanceof AbstractPublicSignal0) {
        		boolean[] b = {false};
        		((AbstractPublicSignal0)sig).connect(()->b[0] = true);
        		((AbstractPublicSignal0)sig).emit();
        		Assert.assertTrue(b[0]);
        	}
        }
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestSensorsQt5.class.getName());
    }
}
