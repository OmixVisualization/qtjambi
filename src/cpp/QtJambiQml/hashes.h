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

inline hash_type qHash(const QJSPrimitiveValue &value, hash_type seed = 0)
{
    switch(value.type()){
    case QJSPrimitiveValue::Undefined:
        return std::numeric_limits<hash_type>::max();
    case QJSPrimitiveValue::Null:
        return 0;
    case QJSPrimitiveValue::Boolean:
        return qHash(value.toBoolean(), seed);
    case QJSPrimitiveValue::Integer:
        return qHash(value.toInteger(), seed);
    case QJSPrimitiveValue::Double:
        return qHash(value.toDouble(), seed);
    case QJSPrimitiveValue::String:
        return qHash(value.toString(), seed);
    }
    return 0;
}
#endif // QT_VERSION >= QT_VERSION_CHECK(6, 1, 0)

inline hash_type qHash(const QQmlScriptString &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.isEmpty());
    seed = hash(seed, value.isNullLiteral());
    seed = hash(seed, value.isUndefinedLiteral());
    seed = hash(seed, value.stringLiteral());
    bool ok = false;
    seed = hash(seed, value.numberLiteral(&ok));
    seed = hash(seed, ok);
    ok = false;
    seed = hash(seed, value.booleanLiteral(&ok));
    seed = hash(seed, ok);
    return seed;
}

inline hash_type qHash(const QQmlError &value, hash_type seed = 0)
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.url());
    seed = hash(seed, value.description());
    seed = hash(seed, value.line());
    seed = hash(seed, value.column());
    seed = hash(seed, quintptr(value.object()));
    seed = hash(seed, value.messageType());
    return seed;
}

hash_type qHash(const QQmlListReference &value, hash_type seed = 0);

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

#if defined(QTJAMBI_GENERATOR_RUNNING) && QT_VERSION >= QT_VERSION_CHECK(6,7,0)
template <typename T>
int qmlRegisterSingletonType(
    const char *uri, int versionMajor, int versionMinor,  const char *typeName,
    QObject*(*callback)(QQmlEngine*,QJSEngine*));
int qmlRegisterSingletonType(
    const char *uri, int versionMajor, int versionMinor, const char *typeName,
    QJSValue(*callback)(QQmlEngine *, QJSEngine *))
#endif

#endif // QTJAMBIQML_HASHES_H
