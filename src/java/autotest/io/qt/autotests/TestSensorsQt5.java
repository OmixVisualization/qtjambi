/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import org.junit.Assert;
import org.junit.Test;

import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject.*;
import io.qt.sensors.QSensorGesture;
import io.qt.sensors.QSensorGestureManager;

public class TestSensorsQt5 extends QApplicationTest {
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
