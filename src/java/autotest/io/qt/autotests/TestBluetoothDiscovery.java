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

import org.junit.BeforeClass;

import io.qt.autotests.generated.UI_DeviceDiscovery;
import io.qt.autotests.generated.UI_ServiceDiscovery;
import io.qt.bluetooth.QBluetoothAddress;
import io.qt.bluetooth.QBluetoothDeviceDiscoveryAgent;
import io.qt.bluetooth.QBluetoothDeviceInfo;
import io.qt.bluetooth.QBluetoothLocalDevice;
import io.qt.bluetooth.QBluetoothServiceDiscoveryAgent;
import io.qt.bluetooth.QBluetoothServiceInfo;
import io.qt.core.QList;
import io.qt.core.QLoggingCategory;
import io.qt.core.QMetaObject;
import io.qt.core.QPoint;
import io.qt.core.QTimer;
import io.qt.core.Qt;
import io.qt.gui.*;
import io.qt.widgets.*;

public class TestBluetoothDiscovery extends ApplicationInitializer{
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		QLoggingCategory.setFilterRules("qt.bluetooth* = true");
		ApplicationInitializer.testInitializeWithWidgets();
	}
	
	@org.junit.Test
	public void discover() {
		DeviceDiscoveryDialog d = new DeviceDiscoveryDialog();
	    d.exec();
	}
}

class DeviceDiscoveryDialog extends QDialog
{
	private static QBrush colorForPairing(QBluetoothLocalDevice.Pairing pairing)
	{
	    return new QBrush(new QColor(pairing == QBluetoothLocalDevice.Pairing.Paired
	           || pairing == QBluetoothLocalDevice.Pairing.AuthorizedPaired
	           ? Qt.GlobalColor.green : Qt.GlobalColor.red));
	}
	
	public DeviceDiscoveryDialog() {
		this(null);
	}
	
	public DeviceDiscoveryDialog(QWidget parent) {
		super(parent);
	    this.localDevice = new QBluetoothLocalDevice();
	    this.ui = new UI_DeviceDiscovery();
	    
	    ui.setupUi(this);

	    // In case of multiple Bluetooth adapters it is possible to set the adapter
	    // to be used. Example code:
	    //
	    // QBluetoothAddress address("XX:XX:XX:XX:XX:XX");
	    // discoveryAgent = new QBluetoothDeviceDiscoveryAgent(address);

	    this.discoveryAgent = new QBluetoothDeviceDiscoveryAgent();

	    ui.scan.clicked.connect(this::startScan);

	    discoveryAgent.deviceDiscovered.connect(this::addDevice);
	    discoveryAgent.finished.connect(this::scanFinished);
	    ui.list.itemActivated.connect(this::itemActivated);
	    localDevice.hostModeStateChanged.connect(this::hostModeStateChanged);

	    hostModeStateChanged(localDevice.hostMode());
	    // add context menu for devices to be able to pair device
	    ui.list.setContextMenuPolicy(Qt.ContextMenuPolicy.CustomContextMenu);
	    ui.list.customContextMenuRequested.connect(this::displayPairingMenu);
	    localDevice.pairingFinished.connect(this::pairingDone);
	    QMetaObject.invokeMethod(ui.scan::click, Qt.ConnectionType.QueuedConnection);
	}
	
	public void addDevice(QBluetoothDeviceInfo info) {
	    String label = info.address().toString() + ' ' + info.name();
	    QList<QListWidgetItem> items = ui.list.findItems(label, Qt.MatchFlag.MatchExactly);
	    if (items.isEmpty()) {
	        QListWidgetItem item = new QListWidgetItem(label);
	        QBluetoothLocalDevice.Pairing pairingStatus = localDevice.pairingStatus(info.address());
	        item.setForeground(colorForPairing(pairingStatus));
	        ui.list.addItem(item);
	    }
	}
	
	public void on_power_clicked(boolean clicked) {
		if (clicked)
	        localDevice.powerOn();
	    else
	        localDevice.setHostMode(QBluetoothLocalDevice.HostMode.HostPoweredOff);
	}
	
	public void on_discoverable_clicked(boolean clicked) {
		if (clicked)
	        localDevice.setHostMode(QBluetoothLocalDevice.HostMode.HostDiscoverable);
	    else
	        localDevice.setHostMode(QBluetoothLocalDevice.HostMode.HostConnectable);
	}
	
	public void displayPairingMenu(QPoint pos){
	    if (ui.list.count() == 0)
	        return;
	    QMenu menu = new QMenu(this);
	    QAction pairAction = menu.addAction("Pair");
	    QAction removePairAction = menu.addAction("Remove Pairing");
	    QAction chosenAction = menu.exec(ui.list.viewport().mapToGlobal(pos));
	    QListWidgetItem currentItem = ui.list.currentItem();

	    String text = currentItem.text();
	    int index = text.indexOf(' ');
	    menu.disposeLater();
	    if (index == -1)
	        return;

	    QBluetoothAddress address = new QBluetoothAddress(text.substring(0, index));
	    if (chosenAction == pairAction) {
	        localDevice.requestPairing(address, QBluetoothLocalDevice.Pairing.Paired);
	    } else if (chosenAction == removePairAction) {
	        localDevice.requestPairing(address, QBluetoothLocalDevice.Pairing.Unpaired);
	    }
	}
	
	public void pairingDone(QBluetoothAddress address, QBluetoothLocalDevice.Pairing pairing){
	    QList<QListWidgetItem> items = ui.list.findItems(address.toString(), Qt.MatchFlag.MatchContains);
	    QBrush color = colorForPairing(pairing);
	    for (QListWidgetItem item : items)
	        item.setForeground(color);
	}
    
	private void startScan(){
        discoveryAgent.start();
        ui.scan.setEnabled(false);
    }
    
    private void scanFinished() {
    	ui.scan.setEnabled(true);
    	QTimer.singleShot(2500, this::reject);
    }
    
    private void itemActivated(QListWidgetItem item) {
    	String text = item.text();
        int index = text.indexOf(' ');
        if (index == -1)
            return;

        QBluetoothAddress address = new QBluetoothAddress(text.substring(0, index));
        String name = text.substring(index + 1);

        ServiceDiscoveryDialog d = new ServiceDiscoveryDialog(name, address);
        d.exec();
    }
    
    private void hostModeStateChanged(QBluetoothLocalDevice.HostMode mode){
        ui.power.setChecked(mode != QBluetoothLocalDevice.HostMode.HostPoweredOff);
        ui.discoverable.setChecked(mode == QBluetoothLocalDevice.HostMode.HostDiscoverable);

        boolean on = mode != QBluetoothLocalDevice.HostMode.HostPoweredOff;
        ui.scan.setEnabled(on);
        ui.discoverable.setEnabled(on);
    }
    
    private final QBluetoothDeviceDiscoveryAgent discoveryAgent;
    private final QBluetoothLocalDevice localDevice;
    private final UI_DeviceDiscovery ui;
};


class ServiceDiscoveryDialog extends QDialog
{
	public ServiceDiscoveryDialog(String name, QBluetoothAddress address) {
		this(name, address, null);
	}
	
	public ServiceDiscoveryDialog(String name, QBluetoothAddress address, QWidget parent) {
		this.ui = new UI_ServiceDiscovery();
	    ui.setupUi(this);

	    //Using default Bluetooth adapter
	    QBluetoothLocalDevice localDevice = new QBluetoothLocalDevice();
	    QBluetoothAddress adapterAddress = localDevice.address();

	    // In case of multiple Bluetooth adapters it is possible to
	    // set which adapter will be used by providing MAC Address.
	    // Example code:
	    //
	    // QBluetoothAddress adapterAddress("XX:XX:XX:XX:XX:XX");
	    // discoveryAgent = new QBluetoothServiceDiscoveryAgent(adapterAddress);

	    this.discoveryAgent = new QBluetoothServiceDiscoveryAgent(adapterAddress);

	    discoveryAgent.setRemoteAddress(address);

	    setWindowTitle(name);

	    discoveryAgent.serviceDiscovered.connect(this::addService);
	    discoveryAgent.finished.connect(ui.status::hide);

	    discoveryAgent.start();
	}

	public void addService(QBluetoothServiceInfo info){
	    if (info.serviceName().isEmpty())
	        return;

	    String line = info.serviceName();
	    if (!info.serviceDescription().isEmpty())
	        line += "\n\t" + info.serviceDescription();
	    if (!info.serviceProvider().isEmpty())
	        line += "\n\t" + info.serviceProvider();

	    ui.list.addItem(line);
	}

	private final QBluetoothServiceDiscoveryAgent discoveryAgent;
	private final UI_ServiceDiscovery ui;
};