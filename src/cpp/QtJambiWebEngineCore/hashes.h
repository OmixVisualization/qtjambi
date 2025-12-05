/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBIQWEBENGINECORE_HASHES_H
#define QTJAMBIQWEBENGINECORE_HASHES_H

#include <QtJambiCore/hashes.h>
#include <QtCore/QtCore>
#include <QtWebEngineCore/QtWebEngineCore>

inline size_t qHash(const QWebEngineHttpRequest &value, size_t seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.url());
    seed = hash(seed, value.headers());
    seed = hash(seed, qint32(value.method()));
    seed = hash(seed, value.postData());
    return seed;
}

inline size_t qHash(const QWebEngineUrlScheme &value, size_t seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.name());
    seed = hash(seed, value.flags());
    seed = hash(seed, qint32(value.syntax()));
    seed = hash(seed, value.defaultPort());
    return seed;
}

inline size_t qHash(const QWebEngineRegisterProtocolHandlerRequest &value, size_t seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.origin());
    seed = hash(seed, value.scheme());
    return seed;
}

inline size_t qHash(const QWebEngineQuotaRequest &value, size_t seed = 0)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
    QtPrivate::QHashCombine hash;
#else
    QtPrivate::QHashCombine hash(seed);
#endif
    seed = hash(seed, value.origin());
    seed = hash(seed, value.requestedSize());
    return seed;
}

inline bool operator==(const QWebEngineFindTextResult& arg1, const QWebEngineFindTextResult& arg2){
    return arg1.numberOfMatches()==arg2.numberOfMatches()
            && arg1.activeMatch()==arg2.activeMatch();
}

inline size_t qHash(const QWebEngineFindTextResult &value, size_t seed = 0)
{
    return qHashMulti(seed, value.numberOfMatches(), value.activeMatch());
}

inline bool operator==(const QWebEngineCookieStore::FilterRequest& arg1, const QWebEngineCookieStore::FilterRequest& arg2){
    return arg1.firstPartyUrl==arg2.firstPartyUrl
            && arg1.origin==arg2.origin
            && arg1.thirdParty==arg2.thirdParty;
}

inline size_t qHash(const QWebEngineCookieStore::FilterRequest &value, size_t seed = 0)
{
    return qHashMulti(seed, value.thirdParty, value.origin, value.firstPartyUrl);
}

inline size_t qHash(const QWebEngineScript &value, size_t seed = 0)
{
    return qHashMulti(seed, value.name(), value.worldId(), value.sourceCode(), value.injectionPoint(), value.runsOnSubFrames());
}

inline bool operator==(const QWebEngineCertificateError& arg1, const QWebEngineCertificateError& arg2){
    return arg1.certificateChain()==arg2.certificateChain()
            && arg1.description()==arg2.description()
            && arg1.isOverridable()==arg2.isOverridable()
            && arg1.type()==arg2.type()
            && arg1.url()==arg2.url();
}

inline size_t qHash(const QWebEngineCertificateError &value, size_t seed = 0)
{
    return qHashMulti(seed, value.certificateChain(), value.url(), value.description(), value.isOverridable(), value.type());
}

inline bool operator==(const QWebEngineHistoryItem& arg1, const QWebEngineHistoryItem& arg2){
    return arg1.originalUrl()==arg2.originalUrl()
            && arg1.url()==arg2.url()
            && arg1.title()==arg2.title()
            && arg1.lastVisited()==arg2.lastVisited()
            && arg1.iconUrl()==arg2.iconUrl()
            && arg1.isValid()==arg2.isValid();
}

inline size_t qHash(const QWebEngineHistoryItem &value, size_t seed = 0)
{
    return qHashMulti(seed, value.originalUrl(), value.url(), value.title(), value.lastVisited(), value.iconUrl(), value.isValid());
}
#if defined(QTJAMBI_GENERATOR_RUNNING)
bool operator==(const QWebEngineFullScreenRequest& arg1, const QWebEngineFullScreenRequest& arg2);
size_t qHash(const QWebEngineFullScreenRequest &value, size_t seed = 0);
#endif //defined(QTJAMBI_GENERATOR_RUNNING)


#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
#if defined(QTJAMBI_GENERATOR_RUNNING)
size_t qHash(const QWebEngineFileSystemAccessRequest& value, size_t seed = 0);
#endif //defined(QTJAMBI_GENERATOR_RUNNING)

#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
#if defined(QTJAMBI_GENERATOR_RUNNING)
size_t qHash(const QWebEnginePermission &value, size_t seed = 0);
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
#if defined(QTJAMBI_GENERATOR_RUNNING)
size_t qHash(const QWebEngineFrame &value, size_t seed = 0);
#endif
#endif //QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)

#endif //QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)

#endif // QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)

#endif // QTJAMBIQWEBENGINECORE_HASHES_H
