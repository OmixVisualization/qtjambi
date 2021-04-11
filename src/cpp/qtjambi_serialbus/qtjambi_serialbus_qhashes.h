#ifndef QTJAMBI_SERIALBUS_QHASHES_H
#define QTJAMBI_SERIALBUS_QHASHES_H

#include <QtSerialBus/QtSerialBus>
#include <qtjambi/qtjambi_core.h>

typedef int(*QModbusRequestCalculatorFunction)(const QModbusRequest &);
typedef int(*QModbusResponseCalculatorFunction)(const QModbusResponse &);

inline hash_type qHash(const QCanBusDevice::Filter &value)
{
    hash_type hashCode = qHash(int(value.format));
    hashCode = hashCode * 31 + qHash(value.frameId);
    hashCode = hashCode * 31 + qHash(value.frameIdMask);
    hashCode = hashCode * 31 + qHash(int(value.type));
    return hashCode;
}


#endif // QTJAMBI_SERIALBUS_QHASHES_H
