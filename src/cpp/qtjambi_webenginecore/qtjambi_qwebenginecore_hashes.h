#ifndef QTJAMBI_QWEBENGINECORE_HASHES_H
#define QTJAMBI_QWEBENGINECORE_HASHES_H

#include <qtjambi_core/qtjambi_core_qhashes.h>
#include <QtCore/QtCore>
#include <QtWebEngineCore/QtWebEngineCore>

inline hash_type qHash(const QWebEngineHttpRequest &value)
{
    hash_type hashCode = qHash(value.url());
    hashCode = hashCode * 31 + qHash(value.headers());
    hashCode = hashCode * 31 + hash_type(value.method());
    hashCode = hashCode * 31 + qHash(value.postData());
    return hashCode;
}

inline hash_type qHash(const QWebEngineUrlScheme &value)
{
    hash_type hashCode = qHash(value.name());
    hashCode = hashCode * 31 + hash_type(value.flags());
    hashCode = hashCode * 31 + hash_type(value.syntax());
    hashCode = hashCode * 31 + qHash(value.defaultPort());
    return hashCode;
}

inline hash_type qHash(const QWebEngineRegisterProtocolHandlerRequest &value)
{
    hash_type hashCode = qHash(value.origin());
    hashCode = hashCode * 31 + qHash(value.scheme());
    return hashCode;
}

inline hash_type qHash(const QWebEngineQuotaRequest &value)
{
    hash_type hashCode = qHash(value.origin());
    hashCode = hashCode * 31 + qHash(value.requestedSize());
    return hashCode;
}

#endif // QTJAMBI_QWEBENGINECORE_HASHES_H
