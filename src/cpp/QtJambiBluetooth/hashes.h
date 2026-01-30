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

#ifndef QTJAMI_BLUETOOTH_HASHES_H
#define QTJAMI_BLUETOOTH_HASHES_H

#include <QtBluetooth/QtBluetooth>
#include <QtJambi/QtJambiAPI>
#include <QtJambi/TypeTests>

namespace QtJambiPrivate {
    template<> struct supports_qHash<QBluetoothServiceInfo::Alternative> : supports_qHash<QList<QVariant>>{};
    template<> struct supports_qHash<QBluetoothServiceInfo::Sequence> : supports_qHash<QList<QVariant>>{};
}

size_t qHash(const QLowEnergyCharacteristicData &value, size_t seed = 0);

inline size_t qHash(const QBluetoothUuid &value, size_t seed = 0)
{
    return qHash(value.toByteArray(), seed);
}

#if QT_VERSION < QT_VERSION_CHECK(6, 6, 0)
inline size_t qHash(const QBluetoothAddress &value, size_t seed = 0)
{
    return qHash(value.toString(), seed);
}
#endif

inline size_t qHash(const QBluetoothDeviceInfo &value, size_t seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.name());
    seed = hash(seed, value.rssi());
    seed = hash(seed, value.address());
    seed = hash(seed, value.isValid());
    seed = hash(seed, value.isCached());
    seed = hash(seed, value.deviceUuid());
    seed = hash(seed, value.serviceUuids());
    seed = hash(seed, value.serviceClasses());
    seed = hash(seed, value.majorDeviceClass());
    seed = hash(seed, value.minorDeviceClass());
    seed = hash(seed, value.coreConfigurations());
    seed = hash(seed, value.manufacturerData());
    seed = hash(seed, value.coreConfigurations());
    return seed;
}

inline size_t qHash(const QBluetoothHostInfo &value, size_t seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.name());
    seed = hash(seed, value.address());
    return seed;
}

inline size_t qHash(const QLowEnergyDescriptor &value, size_t seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.name());
    seed = hash(seed, value.type());
    seed = hash(seed, value.uuid());
    seed = hash(seed, value.value());
    seed = hash(seed, value.isValid());
    return seed;
}

inline size_t qHash(const QLowEnergyAdvertisingData &value, size_t seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.rawData());
    seed = hash(seed, value.services());
    seed = hash(seed, value.localName());
    seed = hash(seed, value.manufacturerId());
    seed = hash(seed, value.discoverability());
    seed = hash(seed, value.manufacturerData());
    seed = hash(seed, value.includePowerLevel());
    seed = hash(seed, value.invalidManufacturerId());
    return seed;
}

inline size_t qHash(const QLowEnergyServiceData &value, size_t seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.type());
    seed = hash(seed, value.uuid());
    seed = hash(seed, value.isValid());
    seed = hash(seed, value.characteristics());
    seed = hash(seed, value.includedServices());
    return seed;
}

inline size_t qHash(const QLowEnergyAdvertisingParameters::AddressInfo &value, size_t seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.type);
    seed = hash(seed, value.address);
    return seed;
}

inline size_t qHash(const QLowEnergyAdvertisingParameters &value, size_t seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.mode());
    seed = hash(seed, value.filterPolicy());
    seed = hash(seed, value.maximumInterval());
    seed = hash(seed, value.minimumInterval());
    seed = hash(seed, value.whiteList());
    return seed;
}

inline size_t qHash(const QLowEnergyDescriptorData &value, size_t seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.uuid());
    seed = hash(seed, value.value());
    seed = hash(seed, int(value.readConstraints()));
    seed = hash(seed, int(value.writeConstraints()));
    seed = hash(seed, value.isReadable());
    seed = hash(seed, value.isWritable());
    seed = hash(seed, value.isValid());
    return seed;
}

inline size_t qHash(const QLowEnergyCharacteristicData &value, size_t seed)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.uuid());
    seed = hash(seed, value.value());
    seed = hash(seed, int(value.properties()));
    seed = hash(seed, int(value.readConstraints()));
    seed = hash(seed, int(value.writeConstraints()));
    seed = hash(seed, value.minimumValueLength());
    seed = hash(seed, value.maximumValueLength());
    seed = hash(seed, value.isValid());
    seed = hash(seed, value.descriptors());
    return seed;
}

inline size_t qHash(const QLowEnergyCharacteristic &value, size_t seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.uuid());
    seed = hash(seed, value.value());
    seed = hash(seed, int(value.properties()));
    seed = hash(seed, value.isValid());
    seed = hash(seed, value.descriptors());
    seed = hash(seed, value.clientCharacteristicConfiguration());
    return seed;
}

inline size_t qHash(const QLowEnergyConnectionParameters &value, size_t seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.minimumInterval());
    seed = hash(seed, value.maximumInterval());
    seed = hash(seed, value.latency());
    seed = hash(seed, value.supervisionTimeout());
    return seed;
}

inline size_t qHash(const QBluetoothServiceInfo &value, size_t seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.serviceName());
    seed = hash(seed, value.serviceDescription());
    seed = hash(seed, value.serviceProvider());
    seed = hash(seed, value.serverChannel());
    seed = hash(seed, value.protocolServiceMultiplexer());
    seed = hash(seed, int(value.socketProtocol()));
    seed = hash(seed, value.device());
    seed = hash(seed, value.isComplete());
    seed = hash(seed, value.isValid());
    seed = hash(seed, value.serviceAvailability());
    seed = hash(seed, value.serviceUuid());
    seed = hash(seed, value.isRegistered());
    seed = hash(seed, value.serviceClassUuids());
    seed = hash(seed, value.attributes());
    return seed;
}

#endif // QTJAMI_BLUETOOTH_HASHES_H
