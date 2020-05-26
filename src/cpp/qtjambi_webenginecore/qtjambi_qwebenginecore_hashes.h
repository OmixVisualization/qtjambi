#ifndef QTJAMBI_QWEBENGINECORE_HASHES_H
#define QTJAMBI_QWEBENGINECORE_HASHES_H

#include <qtjambi_core/qtjambi_core_qhashes.h>
#include <QtCore/QtCore>
#include <QtWebEngineCore/QtWebEngineCore>

inline uint qHash(const QWebEngineHttpRequest &value)
{
    uint hashCode = qHash(value.url());
    hashCode = hashCode * 31 + qHash(value.headers());
    hashCode = hashCode * 31 + uint(value.method());
    hashCode = hashCode * 31 + qHash(value.postData());
    return hashCode;
}

inline uint qHash(const QWebEngineUrlScheme &value)
{
    uint hashCode = qHash(value.name());
    hashCode = hashCode * 31 + uint(value.flags());
    hashCode = hashCode * 31 + uint(value.syntax());
    hashCode = hashCode * 31 + qHash(value.defaultPort());
    return hashCode;
}

inline uint qHash(const QWebEngineRegisterProtocolHandlerRequest &value)
{
    uint hashCode = qHash(value.origin());
    hashCode = hashCode * 31 + qHash(value.scheme());
    return hashCode;
}

inline uint qHash(const QWebEngineQuotaRequest &value)
{
    uint hashCode = qHash(value.origin());
    hashCode = hashCode * 31 + qHash(value.requestedSize());
    return hashCode;
}

#endif // QTJAMBI_QWEBENGINECORE_HASHES_H
