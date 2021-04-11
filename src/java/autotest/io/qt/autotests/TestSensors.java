/****************************************************************************
**
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import org.junit.Test;

import io.qt.core.QMetaObject.Slot1;
import io.qt.sensors.QSensorGesture;
import io.qt.sensors.QSensorGestureManager;

public class TestSensors extends QApplicationTest {
    @Test
    public void test() {
    	// Create a QSensorGestureManager
        QSensorGestureManager gestureManager = new QSensorGestureManager();

        // Get a list of known recognizers
        List<String> recognizersList = gestureManager.gestureIds();

        // Create a QSensorGeture object for each of those gesture recognizers
        QSensorGesture gesture = new QSensorGesture(recognizersList);
        // Connect the known signals up.
        gesture.detected.connect((Slot1<String>)System.out::println);
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestSensors.class.getName());
    }
}
