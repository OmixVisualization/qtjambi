#ifndef QTJAMBI_SERIALBUS_QHASHES_H
#define QTJAMBI_SERIALBUS_QHASHES_H

#include <QtSerialBus/QtSerialBus>

typedef int(*QModbusRequestCalculatorFunction)(const QModbusRequest &);
typedef int(*QModbusResponseCalculatorFunction)(const QModbusResponse &);

#endif // QTJAMBI_SERIALBUS_QHASHES_H
