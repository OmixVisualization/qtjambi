/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

import static org.junit.Assert.assertTrue;

import java.util.Collection;
import java.util.Collections;
import java.util.List;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.Test;

import io.qt.autotests.generated.General;
import io.qt.bluetooth.QBluetoothDeviceInfo;
import io.qt.core.QLibraryInfo;
import io.qt.core.QVersionNumber;

public class TestBluetoothInjectedCodeQt5 extends ApplicationInitializer {
    @Test
    public void testQBluetoothDeviceInfo()
    {
    	Assume.assumeTrue("Test only available for Qt versions less than 5.14.", QLibraryInfo.version().compareTo(new QVersionNumber(5, 14, 0))>0);
    	QBluetoothDeviceInfo bluetoothDeviceInfo = new QBluetoothDeviceInfo();
    	Object serviceUuids = bluetoothDeviceInfo.serviceUuids();
    	assertTrue(serviceUuids!=null);
    	if(serviceUuids instanceof Collection) {
    		Assert.assertEquals(Collections.emptyList(), serviceUuids);
    	}else {
	    	Assert.assertEquals(QBluetoothDeviceInfo.DataCompleteness.DataUnavailable, General.internalAccess.readField(serviceUuids, serviceUuids.getClass(), "completeness", QBluetoothDeviceInfo.DataCompleteness.class));
	    	Assert.assertEquals(Collections.emptyList(), General.internalAccess.readField(serviceUuids, serviceUuids.getClass(), "serviceUuids", List.class));
    	}
    }
}
