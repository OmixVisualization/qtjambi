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

#ifndef QTJAMBISERIALBUS_HASHES_H
#define QTJAMBISERIALBUS_HASHES_H

#include <QtSerialBus/QtSerialBus>
#include <QtJambi/Global>

inline bool operator==(const QCanBusDeviceInfo &value1, const QCanBusDeviceInfo &value2){
    return value1.name()==value2.name()
            && value1.description()==value2.description()
            && value1.serialNumber()==value2.serialNumber()
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            && value1.plugin()==value2.plugin()
            && value1.alias()==value2.alias()
#endif
            && value1.channel()==value2.channel()
            && value1.hasFlexibleDataRate()==value2.hasFlexibleDataRate()
            && value1.isVirtual()==value2.isVirtual();
}

inline hash_type qHash(const QCanBusDeviceInfo &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.name());
    seed = hash(seed, value.description());
    seed = hash(seed, value.serialNumber());
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    seed = hash(seed, value.plugin());
    seed = hash(seed, value.alias());
#endif
    seed = hash(seed, value.channel());
    seed = hash(seed, value.hasFlexibleDataRate());
    seed = hash(seed, value.isVirtual());
    return seed;
}

inline bool operator==(const QCanBusFrame::TimeStamp &value1, const QCanBusFrame::TimeStamp &value2){
    return value1.seconds()==value2.seconds()
            && value1.microSeconds()==value2.microSeconds();
}

inline hash_type qHash(const QCanBusFrame::TimeStamp &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.seconds());
    seed = hash(seed, value.microSeconds());
    return seed;
}

inline bool operator==(const QCanBusFrame &value1, const QCanBusFrame &value2){
    return value1.isValid()==value2.isValid()
            && value1.frameType()==value2.frameType()
            && value1.hasExtendedFrameFormat()==value2.hasExtendedFrameFormat()
            && value1.frameId()==value2.frameId()
            && value1.payload()==value2.payload()
            && value1.timeStamp()==value2.timeStamp()
            && value1.error()==value2.error()
            && value1.hasFlexibleDataRateFormat()==value2.hasFlexibleDataRateFormat()
            && value1.hasBitrateSwitch()==value2.hasBitrateSwitch()
            && value1.hasErrorStateIndicator()==value2.hasErrorStateIndicator()
            && value1.hasLocalEcho()==value2.hasLocalEcho();
}

inline hash_type qHash(const QCanBusFrame &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.isValid());
    seed = hash(seed, value.frameType());
    seed = hash(seed, value.hasExtendedFrameFormat());
    seed = hash(seed, value.frameId());
    seed = hash(seed, value.payload());
    seed = hash(seed, value.timeStamp());
    seed = hash(seed, value.error());
    seed = hash(seed, value.hasFlexibleDataRateFormat());
    seed = hash(seed, value.hasBitrateSwitch());
    seed = hash(seed, value.hasErrorStateIndicator());
    seed = hash(seed, value.hasLocalEcho());
    return seed;
}

inline hash_type qHash(const QCanBusDevice::Filter &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, int(value.format));
    seed = hash(seed, value.frameId);
    seed = hash(seed, value.frameIdMask);
    seed = hash(seed, int(value.type));
    return seed;
}

inline bool operator==(const QModbusDataUnit &value1, const QModbusDataUnit &value2){
    return value1.registerType()==value2.registerType()
            && value1.startAddress()==value2.startAddress()
            && value1.values()==value2.values()
            && value1.valueCount()==value2.valueCount();
}

inline hash_type qHash(const QModbusDataUnit &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.registerType());
    seed = hash(seed, value.startAddress());
    seed = hash(seed, value.values());
    seed = hash(seed, value.valueCount());
    return seed;
}

inline bool operator==(const QModbusDeviceIdentification &value1, const QModbusDeviceIdentification &value2){
    if(value1.conformityLevel()==value2.conformityLevel()){
        QList<int> objectIds1 = value1.objectIds();
        if(objectIds1==value2.objectIds()){
            for(int id : objectIds1){
                if(value1.value(id)!=value2.value(id))
                    return false;
            }
            return true;
        }
    }
    return false;
}

inline hash_type qHash(const QModbusDeviceIdentification &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.conformityLevel());
    QList<int> objectIds = value.objectIds();
    seed = hash(seed, objectIds);
    for(int id : objectIds){
        seed = hash(seed, value.value(id));
    }
    return seed;
}

inline bool operator==(const QModbusPdu &value1, const QModbusPdu &value2){
    return value1.isException()==value2.isException()
            && value1.functionCode()==value2.functionCode()
            && value1.data()==value2.data();
}

inline hash_type qHash(const QModbusPdu &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.isException());
    seed = hash(seed, value.functionCode());
    seed = hash(seed, value.data());
    return seed;
}

inline bool operator==(const QModbusRequest &value1, const QModbusRequest &value2){
    return static_cast<const QModbusPdu&>(value1)==static_cast<const QModbusPdu&>(value2);
}

inline hash_type qHash(const QModbusRequest &value, hash_type seed = 0)
{
    return qHash(static_cast<const QModbusPdu&>(value), seed);
}

inline bool operator==(const QModbusResponse &value1, const QModbusResponse &value2){
    return static_cast<const QModbusPdu&>(value1)==static_cast<const QModbusPdu&>(value2);
}

inline hash_type qHash(const QModbusResponse &value, hash_type seed = 0)
{
    return qHash(static_cast<const QModbusPdu&>(value), seed);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)

namespace QTypeTraits
{
template <typename Stream>
struct has_ostream_operator<Stream, QCanSignalDescription::MultiplexValueRange>
        : std::false_type {};
}

inline bool operator==(const QCanFrameProcessor::ParseResult &value1, const QCanFrameProcessor::ParseResult &value2){
    return value1.uniqueId==value2.uniqueId && value1.signalValues==value2.signalValues;
}

inline size_t qHash(const QCanFrameProcessor::ParseResult &value, size_t seed = 0)
{
    QString signalValues;
    QDebug(&signalValues) << value.signalValues;
    return qHashMulti(seed, value.uniqueId, signalValues);
}

#endif

#endif // QTJAMBISERIALBUS_HASHES_H
