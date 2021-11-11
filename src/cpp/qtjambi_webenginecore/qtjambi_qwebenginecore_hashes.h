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

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
inline hash_type qHash(const QWebEngineScript &value)
{
    hash_type hashCode = qHash(value.name());
    hashCode = hashCode * 31 + qHash(value.worldId());
    hashCode = hashCode * 31 + qHash(value.sourceCode());
    hashCode = hashCode * 31 + qHash(value.injectionPoint());
    hashCode = hashCode * 31 + qHash(value.runsOnSubFrames());
    return hashCode;
}

inline bool operator==(const QWebEngineCertificateError& arg1, const QWebEngineCertificateError& arg2){
    return arg1.certificateChain()==arg2.certificateChain()
            && arg1.description()==arg2.description()
            && arg1.isOverridable()==arg2.isOverridable()
            && arg1.type()==arg2.type()
            && arg1.url()==arg2.url();
}

inline bool operator==(const QWebEngineFindTextResult& arg1, const QWebEngineFindTextResult& arg2){
    return arg1.numberOfMatches()==arg2.numberOfMatches()
            && arg1.activeMatch()==arg2.activeMatch();
}

inline bool operator==(const QWebEngineHistoryItem& arg1, const QWebEngineHistoryItem& arg2){
    return arg1.originalUrl()==arg2.originalUrl()
            && arg1.url()==arg2.url()
            && arg1.title()==arg2.title()
            && arg1.lastVisited()==arg2.lastVisited()
            && arg1.iconUrl()==arg2.iconUrl()
            && arg1.isValid()==arg2.isValid();
}

inline bool operator==(const QWebEngineCookieStore::FilterRequest& arg1, const QWebEngineCookieStore::FilterRequest& arg2){
    return arg1.firstPartyUrl==arg2.firstPartyUrl
            && arg1.origin==arg2.origin
            && arg1.thirdParty==arg2.thirdParty;
}

inline hash_type qHash(const QWebEngineCookieStore::FilterRequest &value)
{
    return qHashMulti(0, value.thirdParty, value.origin, value.firstPartyUrl);
}

inline hash_type qHash(const QWebEngineHistoryItem &value)
{
    return qHashMulti(0, value.originalUrl(), value.url(), value.title(), value.lastVisited(), value.iconUrl(), value.isValid());
}

inline hash_type qHash(const QWebEngineCertificateError &value)
{
    return qHashMulti(0, value.certificateChain(), value.url(), value.description(), value.isOverridable(), value.type());
}

inline hash_type qHash(const QWebEngineFindTextResult &value)
{
    return qHashMulti(0, value.numberOfMatches(), value.activeMatch());
}
#endif

#endif // QTJAMBI_QWEBENGINECORE_HASHES_H
