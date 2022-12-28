#ifndef QTJAMBIQML_HASHES_H
#define QTJAMBIQML_HASHES_H

#include <QtCore/QtGlobal>
#include <QtQml/QQmlScriptString>
#include <QtQml/QQmlError>
#include <QtQml/QQmlListReference>
#include <QtQml/QJSEngine>
#include <QtJambi/QtJambiAPI>

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

inline hash_type qHash(const QQmlScriptString &value)
{
    hash_type hashCode = qHash(value.isEmpty());
    hashCode = hashCode * 31 + qHash(value.isNullLiteral());
    hashCode = hashCode * 31 + qHash(value.isUndefinedLiteral());
    hashCode = hashCode * 31 + qHash(value.stringLiteral());
    bool ok = false;
    hashCode = hashCode * 31 + qHash(value.numberLiteral(&ok));
    hashCode = hashCode * 31 + qHash(ok);
    ok = false;
    hashCode = hashCode * 31 + qHash(value.booleanLiteral(&ok));
    hashCode = hashCode * 31 + qHash(ok);
    return hashCode;
}

inline hash_type qHash(const QQmlError &value)
{
    hash_type hashCode = qHash(value.url());
    hashCode = hashCode * 31 + qHash(value.description());
    hashCode = hashCode * 31 + qHash(value.line());
    hashCode = hashCode * 31 + qHash(value.column());
    hashCode = hashCode * 31 + qHash(quintptr(value.object()));
    hashCode = hashCode * 31 + qHash(value.messageType());
    return hashCode;
}

hash_type qHash(const QQmlListReference &value);

namespace QtQml {
    typedef QObject *(*QQmlAttachedPropertiesFunc)(QObject *);
#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
    typedef std::function<QObject*(QQmlEngine *, QJSEngine *)> ObjectCallback;
#else
    typedef QObject *(*ObjectCallback)(QQmlEngine *, QJSEngine *);
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    typedef std::function<QJSValue(QQmlEngine *, QJSEngine *)> ValueCallback;
#else
    typedef QJSValue (*ValueCallback)(QQmlEngine *, QJSEngine *);
#endif
}

#endif // QTJAMBIQML_HASHES_H
