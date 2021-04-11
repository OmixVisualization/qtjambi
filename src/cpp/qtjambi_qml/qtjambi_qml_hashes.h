#ifndef QTJAMBI_QML_HASHES_H
#define QTJAMBI_QML_HASHES_H

#include <QtCore/QtGlobal>
#include <qtjambi/qtjambi_core.h>

#if QT_VERSION >= QT_VERSION_CHECK(6, 1, 0)
#include <QtQml/QJSPrimitiveValue>

inline hash_type qHash(const QJSPrimitiveValue &value)
{
    switch(value.type()){
    case QJSPrimitiveValue::Undefined:
        return std::numeric_limits<hash_type>::max();
    case QJSPrimitiveValue::Null:
        return 0;
    case QJSPrimitiveValue::Boolean:
        return qHash(value.toBoolean());
    case QJSPrimitiveValue::Integer:
        return qHash(value.toInteger());
    case QJSPrimitiveValue::Double:
        return qHash(value.toDouble());
    case QJSPrimitiveValue::String:
        return qHash(value.toString());
    }
    return 0;
}
#endif // QT_VERSION >= QT_VERSION_CHECK(6, 1, 0)

#endif // QTJAMBI_QML_HASHES_H
