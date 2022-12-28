/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
inline hash_type qHash(const QWebEngineScript &value, hash_type seed = 0)
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

inline hash_type qHash(const QWebEngineCookieStore::FilterRequest &value, hash_type seed = 0)
{
    return qHashMulti(seed, value.thirdParty, value.origin, value.firstPartyUrl);
}

inline hash_type qHash(const QWebEngineHistoryItem &value, hash_type seed = 0)
{
    return qHashMulti(seed, value.originalUrl(), value.url(), value.title(), value.lastVisited(), value.iconUrl(), value.isValid());
}

inline hash_type qHash(const QWebEngineCertificateError &value, hash_type seed = 0)
{
    return qHashMulti(seed, value.certificateChain(), value.url(), value.description(), value.isOverridable(), value.type());
}

inline hash_type qHash(const QWebEngineFindTextResult &value, hash_type seed = 0)
{
    return qHashMulti(seed, value.numberOfMatches(), value.activeMatch());
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
#if defined(QTJAMBI_GENERATOR_RUNNING)
hash_type qHash(const QWebEngineFileSystemAccessRequest& value, hash_type seed = 0);
#else
#include <QtWebEngineCore/QWebEngineFileSystemAccessRequest>

namespace QtWebEngineCore{
class FileSystemAccessPermissionRequestManagerQt{
public:
    static hash_type hash(const QWebEngineFileSystemAccessRequest& value, hash_type seed){
        return qHash(quintptr(&*value.d_ptr), seed);
    }
};
}

inline hash_type qHash(const QWebEngineFileSystemAccessRequest& value, hash_type seed = 0)
{
    return QtWebEngineCore::FileSystemAccessPermissionRequestManagerQt::hash(value, seed);
}
#endif
#endif

#endif

#endif // QTJAMBIQWEBENGINECORE_HASHES_H
