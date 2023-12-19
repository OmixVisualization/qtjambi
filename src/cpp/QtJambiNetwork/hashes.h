/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBINETWORK_HASHES_H
#define QTJAMBINETWORK_HASHES_H

#include <QtNetwork/QtNetwork>
#include <QtCore/QDeadlineTimer>
#include <QtJambi/Global>

hash_type qHash(const QHttpPart &value, hash_type seed = 0);

inline hash_type qHash(const QHstsPolicy& p, hash_type seed = 0){
    QtPrivate::QHashCombine hash;
    seed = hash(seed, p.expiry());
    seed = hash(seed, p.includesSubDomains());
    seed = hash(seed, p.host());
    return seed;
}

inline hash_type qHash(const QNetworkAddressEntry& p, hash_type seed = 0){
    QtPrivate::QHashCombine hash;
    seed = hash(seed, int(p.dnsEligibility()));
    seed = hash(seed, p.ip());
    seed = hash(seed, p.netmask());
    seed = hash(seed, p.prefixLength());
    seed = hash(seed, p.isLifetimeKnown());
    seed = hash(seed, p.preferredLifetime().deadline());
    seed = hash(seed, p.validityLifetime().deadline());
    seed = hash(seed, p.isPermanent());
    return seed;
}

inline hash_type qHash(const QNetworkCacheMetaData& p, hash_type seed = 0){
    QtPrivate::QHashCombine hash;
    seed = hash(seed, p.isValid());
    seed = hash(seed, p.url());
    seed = hash(seed, p.lastModified());
    seed = hash(seed, p.expirationDate());
    seed = hash(seed, p.saveToDisk());
    QNetworkCacheMetaData::RawHeaderList rawHeaders = p.rawHeaders();
    seed = hash(seed, rawHeaders.size());
    for(const QNetworkCacheMetaData::RawHeader & i : rawHeaders){
        seed = hash(seed, i.first);
        seed = hash(seed, i.second);
    }
    QNetworkCacheMetaData::AttributesMap attributes = p.attributes();
    seed = hash(seed, attributes.size());
    for(QNetworkRequest::Attribute i : attributes.keys()){
        seed = hash(seed, int(i));
    }
    return seed;
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
inline hash_type qHash(const QNetworkConfiguration& p, hash_type seed = 0){
    QtPrivate::QHashCombine hash;
    seed = hash(seed, int(p.state()));
    seed = hash(seed, int(p.type()));
    seed = hash(seed, int(p.purpose()));
    seed = hash(seed, int(p.bearerType()));
    seed = hash(seed, int(p.bearerTypeFamily()));
    seed = hash(seed, p.bearerTypeName());
    seed = hash(seed, p.identifier());
    seed = hash(seed, p.isRoamingAvailable());
    seed = hash(seed, p.name());
    seed = hash(seed, p.isValid());
    seed = hash(seed, p.connectTimeout());
    seed = hash(seed, p.children().size());
    return seed;
}
#endif

inline hash_type qHash(const QSslPreSharedKeyAuthenticator& value, hash_type seed = 0){
    QtPrivate::QHashCombine hash;
    seed = hash(seed, value.identityHint());
    seed = hash(seed, value.identity());
    seed = hash(seed, value.maximumIdentityLength());
    seed = hash(seed, value.preSharedKey());
    seed = hash(seed, value.maximumPreSharedKeyLength());
    return seed;
}

inline hash_type qHash(const QNetworkCookie& p, hash_type seed = 0){
    QtPrivate::QHashCombine hash;
    seed = hash(seed, p.isSecure());
    seed = hash(seed, p.isHttpOnly());
    seed = hash(seed, p.isSessionCookie());
    seed = hash(seed, p.expirationDate());
    seed = hash(seed, p.domain());
    seed = hash(seed, p.path());
    seed = hash(seed, p.name());
    seed = hash(seed, p.value());
    return seed;
}

inline hash_type qHash(const QNetworkProxy& p, hash_type seed = 0){
    QtPrivate::QHashCombine hash;
    seed = hash(seed, int(p.type()));
    seed = hash(seed, p.capabilities());
    seed = hash(seed, p.isCachingProxy());
    seed = hash(seed, p.isTransparentProxy());
    seed = hash(seed, p.user());
    seed = hash(seed, p.password());
    seed = hash(seed, p.hostName());
    seed = hash(seed, p.port());
    for(const QByteArray & i : p.rawHeaderList()){
        seed = hash(seed, i);
    }
    return seed;
}

inline hash_type qHash(const QNetworkProxyQuery& p, hash_type seed = 0){
    QtPrivate::QHashCombine hash;
    seed = hash(seed, int(p.queryType()));
    seed = hash(seed, p.peerPort());
    seed = hash(seed, p.peerHostName());
    seed = hash(seed, p.localPort());
    seed = hash(seed, p.protocolTag());
    seed = hash(seed, p.url());
    return seed;
}

inline hash_type qHash(const QNetworkRequest& p, hash_type seed = 0){
    QtPrivate::QHashCombine hash;
    seed = hash(seed, p.url());
    seed = hash(seed, quintptr(p.originatingObject()));
    seed = hash(seed, int(p.priority()));
    seed = hash(seed, p.maximumRedirectsAllowed());
    return seed;
}

inline hash_type qHash(const QAuthenticator& p, hash_type seed = 0){
    if(p.isNull())
        return 0;
    QtPrivate::QHashCombine hash;
    seed = hash(seed, p.user());
    seed = hash(seed, p.password());
    seed = hash(seed, p.realm());
    seed = hash(seed, p.options().keys());
    return seed;
}

#ifndef QT_NO_SSL

inline hash_type qHash(const QSslKey& p, hash_type seed = 0){
    if(p.isNull())
        return 0;
    QtPrivate::QHashCombine hash;
    seed = hash(seed, p.type());
    seed = hash(seed, quintptr(p.handle()));
    seed = hash(seed, p.length());
    seed = hash(seed, p.algorithm());
    return seed;
}

inline hash_type qHash(const QSslCipher& p, hash_type seed = 0){
    if(p.isNull())
        return 0;
    QtPrivate::QHashCombine hash;
    seed = hash(seed, p.name());
    seed = hash(seed, quintptr(p.protocol()));
    seed = hash(seed, p.usedBits());
    seed = hash(seed, p.supportedBits());
    seed = hash(seed, p.protocolString());
    seed = hash(seed, p.encryptionMethod());
    seed = hash(seed, p.keyExchangeMethod());
    seed = hash(seed, p.authenticationMethod());
    return seed;
}

inline hash_type qHash(const QSslConfiguration& p, hash_type seed = 0){
    if(p.isNull())
        return 0;
    QtPrivate::QHashCombine hash;
    seed = hash(seed, p.ciphers());
    seed = hash(seed, quintptr(p.protocol()));
    seed = hash(seed, p.privateKey());
    seed = hash(seed, p.sessionCipher());
    seed = hash(seed, p.sessionTicket());
    seed = hash(seed, p.caCertificates());
    seed = hash(seed, p.ellipticCurves());
    seed = hash(seed, p.peerVerifyMode());
    seed = hash(seed, p.peerCertificate());
    seed = hash(seed, p.peerVerifyDepth());
    seed = hash(seed, p.sessionProtocol());
    seed = hash(seed, p.localCertificate());
    seed = hash(seed, p.ephemeralServerKey());
    seed = hash(seed, p.ocspStaplingEnabled());
    seed = hash(seed, p.allowedNextProtocols());
    seed = hash(seed, p.peerCertificateChain());
    seed = hash(seed, p.localCertificateChain());
    seed = hash(seed, p.nextNegotiatedProtocol());
    seed = hash(seed, p.diffieHellmanParameters());
    seed = hash(seed, p.preSharedKeyIdentityHint());
    seed = hash(seed, p.sessionTicketLifeTimeHint());
#if QT_CONFIG(dtls)
    seed = hash(seed, p.dtlsCookieVerificationEnabled());
#endif // dtls
    seed = hash(seed, p.nextProtocolNegotiationStatus());
    seed = hash(seed, p.supportedCiphers());
    return seed;
}

#endif // def QT_NO_SSL

#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)

inline hash_type qHash(const QHttp2Configuration& p, hash_type seed = 0){
    QtPrivate::QHashCombine hash;
    seed = hash(seed, p.serverPushEnabled());
    seed = hash(seed, quintptr(p.huffmanCompressionEnabled()));
    seed = hash(seed, p.sessionReceiveWindowSize());
    seed = hash(seed, p.streamReceiveWindowSize());
    seed = hash(seed, p.maxFrameSize());
    return seed;
}

#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0) && defined(QTJAMBI_GENERATOR_RUNNING)
size_t qHash(const QHttp1Configuration &value, size_t seed = 0);
#endif

#endif // QTJAMBINETWORK_HASHES_H
