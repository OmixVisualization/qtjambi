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

import java.util.List;

import io.qt.bluetooth.QBluetoothAddress;
import io.qt.bluetooth.QBluetoothLocalDevice;

public class TestBluetooth extends ApplicationInitializer{
	@org.junit.Before
	public void setUp() throws Exception {
		
	}
	
	@org.junit.After
	public void tearDown() throws Exception {
		
	}
	
	@org.junit.Test
	public void test() {
		QBluetoothLocalDevice localDevice = new QBluetoothLocalDevice();
		@SuppressWarnings("unused")
		String localDeviceName = null;

		// Check if Bluetooth is available on this device
		if (localDevice.isValid()) {

		    // Turn Bluetooth on
		    localDevice.powerOn();

		    // Read local device name
		    localDeviceName = localDevice.name();

		    // Make it visible to others
		    localDevice.setHostMode(QBluetoothLocalDevice.HostMode.HostDiscoverable);

		    // Get connected devices
		    List<QBluetoothAddress> remotes = localDevice.connectedDevices();
		    
		    System.out.println(remotes);
		}
	}
}
