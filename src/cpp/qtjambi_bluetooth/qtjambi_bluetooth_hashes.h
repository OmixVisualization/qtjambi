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

#ifndef QTJAMI_BLUETOOTH_HASHES_H
#define QTJAMI_BLUETOOTH_HASHES_H

#include <QtBluetooth/QtBluetooth>
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_typetests.h>

namespace QtJambiPrivate {
    template<> struct supports_qHash<QBluetoothServiceInfo::Alternative> : supports_qHash<QList<QVariant>>{};
    template<> struct supports_qHash<QBluetoothServiceInfo::Sequence> : supports_qHash<QList<QVariant>>{};
}

hash_type qHash(const QLowEnergyCharacteristicData &value);

inline hash_type qHash(const QBluetoothUuid &value)
{
    return qHash(value.toByteArray());
}

inline hash_type qHash(const QBluetoothAddress &value)
{
    return qHash(value.toString());
}

inline hash_type qHash(const QBluetoothDeviceInfo &value)
{
    hash_type hashCode = qHash(value.name());
    hashCode = hashCode * 31 + qHash(value.rssi());
    hashCode = hashCode * 31 + qHash(value.address());
    hashCode = hashCode * 31 + qHash(value.isValid());
    hashCode = hashCode * 31 + qHash(value.isCached());
    hashCode = hashCode * 31 + qHash(value.deviceUuid());
    hashCode = hashCode * 31 + qHash(value.serviceUuids());
    hashCode = hashCode * 31 + qHash(value.serviceClasses());
    hashCode = hashCode * 31 + qHash(value.majorDeviceClass());
    hashCode = hashCode * 31 + qHash(value.minorDeviceClass());
    hashCode = hashCode * 31 + qHash(value.coreConfigurations());
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    hashCode = hashCode * 31 + qHash(value.serviceUuidsCompleteness());
#else
    hashCode = hashCode * 31 + qHash(value.manufacturerData());
    hashCode = hashCode * 31 + qHash(value.coreConfigurations());
#endif
    return hashCode;
}

inline hash_type qHash(const QBluetoothHostInfo &value)
{
    hash_type hashCode = qHash(value.name());
    hashCode = hashCode * 31 + qHash(value.address());
    return hashCode;
}

inline hash_type qHash(const QLowEnergyDescriptor &value)
{
    hash_type hashCode = qHash(value.name());
    hashCode = hashCode * 31 + qHash(value.type());
    hashCode = hashCode * 31 + qHash(value.uuid());
    hashCode = hashCode * 31 + qHash(value.value());
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    hashCode = hashCode * 31 + qHash(value.handle());
#endif
    hashCode = hashCode * 31 + qHash(value.isValid());
    return hashCode;
}

inline hash_type qHash(const QLowEnergyAdvertisingData &value)
{
    hash_type hashCode = qHash(value.rawData());
    hashCode = hashCode * 31 + qHash(value.services());
    hashCode = hashCode * 31 + qHash(value.localName());
    hashCode = hashCode * 31 + qHash(value.manufacturerId());
    hashCode = hashCode * 31 + qHash(value.discoverability());
    hashCode = hashCode * 31 + qHash(value.manufacturerData());
    hashCode = hashCode * 31 + qHash(value.includePowerLevel());
    hashCode = hashCode * 31 + qHash(value.invalidManufacturerId());
    return hashCode;
}

inline hash_type qHash(const QLowEnergyServiceData &value)
{
    hash_type hashCode = qHash(value.type());
    hashCode = hashCode * 31 + qHash(value.uuid());
    hashCode = hashCode * 31 + qHash(value.isValid());
    hashCode = hashCode * 31 + qHash(value.characteristics());
    hashCode = hashCode * 31 + qHash(value.includedServices());
    return hashCode;
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
inline hash_type qHash(const QBluetoothTransferRequest &value)
{
    hash_type hashCode = qHash(value.address());
    return hashCode;
}
#endif

inline hash_type qHash(const QLowEnergyAdvertisingParameters::AddressInfo &value)
{
    hash_type hashCode = qHash(value.type);
    hashCode = hashCode * 31 + qHash(value.address);
    return hashCode;
}

inline hash_type qHash(const QLowEnergyAdvertisingParameters &value)
{
    hash_type hashCode = qHash(value.mode());
    hashCode = hashCode * 31 + qHash(value.filterPolicy());
    hashCode = hashCode * 31 + qHash(value.maximumInterval());
    hashCode = hashCode * 31 + qHash(value.minimumInterval());
    hashCode = hashCode * 31 + qHash(value.whiteList());
    return hashCode;
}

inline hash_type qHash(const QLowEnergyDescriptorData &value)
{
    hash_type hashCode = qHash(value.uuid());
    hashCode = hashCode * 31 + qHash(value.value());
    hashCode = hashCode * 31 + qHash(int(value.readConstraints()));
    hashCode = hashCode * 31 + qHash(int(value.writeConstraints()));
    hashCode = hashCode * 31 + qHash(value.isReadable());
    hashCode = hashCode * 31 + qHash(value.isWritable());
    hashCode = hashCode * 31 + qHash(value.isValid());
    return hashCode;
}

inline hash_type qHash(const QLowEnergyCharacteristicData &value)
{
    hash_type hashCode = qHash(value.uuid());
    hashCode = hashCode * 31 + qHash(value.value());
    hashCode = hashCode * 31 + qHash(int(value.properties()));
    hashCode = hashCode * 31 + qHash(int(value.readConstraints()));
    hashCode = hashCode * 31 + qHash(int(value.writeConstraints()));
    hashCode = hashCode * 31 + qHash(value.minimumValueLength());
    hashCode = hashCode * 31 + qHash(value.maximumValueLength());
    hashCode = hashCode * 31 + qHash(value.isValid());
    hashCode = hashCode * 31 + qHash(value.descriptors());
    return hashCode;
}

inline hash_type qHash(const QLowEnergyCharacteristic &value)
{
    hash_type hashCode = qHash(value.uuid());
    hashCode = hashCode * 31 + qHash(value.value());
    hashCode = hashCode * 31 + qHash(int(value.properties()));
    hashCode = hashCode * 31 + qHash(value.isValid());
    hashCode = hashCode * 31 + qHash(value.descriptors());
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    hashCode = hashCode * 31 + qHash(value.handle());
#else
    hashCode = hashCode * 31 + qHash(value.clientCharacteristicConfiguration());
#endif
    return hashCode;
}

inline hash_type qHash(const QLowEnergyConnectionParameters &value)
{
    hash_type hashCode = qHash(value.minimumInterval());
    hashCode = hashCode * 31 + qHash(value.maximumInterval());
    hashCode = hashCode * 31 + qHash(value.latency());
    hashCode = hashCode * 31 + qHash(value.supervisionTimeout());
    return hashCode;
}

inline hash_type qHash(const QBluetoothServiceInfo &value)
{
    hash_type hashCode = qHash(value.serviceName());
    hashCode = hashCode * 31 + qHash(value.serviceDescription());
    hashCode = hashCode * 31 + qHash(value.serviceProvider());
    hashCode = hashCode * 31 + qHash(value.serverChannel());
    hashCode = hashCode * 31 + qHash(value.protocolServiceMultiplexer());
    hashCode = hashCode * 31 + qHash(int(value.socketProtocol()));
    hashCode = hashCode * 31 + qHash(value.device());
    hashCode = hashCode * 31 + qHash(value.isComplete());
    hashCode = hashCode * 31 + qHash(value.isValid());
    hashCode = hashCode * 31 + qHash(value.serviceAvailability());
    hashCode = hashCode * 31 + qHash(value.serviceUuid());
    hashCode = hashCode * 31 + qHash(value.isRegistered());
    hashCode = hashCode * 31 + qHash(value.serviceClassUuids());
    hashCode = hashCode * 31 + qHash(value.attributes());
    return hashCode;
}

#endif // QTJAMI_BLUETOOTH_HASHES_H
