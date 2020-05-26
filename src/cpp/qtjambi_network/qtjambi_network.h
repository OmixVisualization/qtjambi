/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

#ifndef QTJAMBI_NETWORK_H
#define QTJAMBI_NETWORK_H
#include <QtNetwork/QtNetwork>
#include <QtCore/QDeadlineTimer>

inline uint qHash(const QHstsPolicy& p){
    uint hashCode = qHash(p.expiry());
    hashCode = hashCode * 31 + qHash(p.includesSubDomains());
    hashCode = hashCode * 31 + qHash(p.host());
    return hashCode;
}

inline uint qHash(const QNetworkAddressEntry& p){
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(int(p.dnsEligibility()));
    hashCode = hashCode * 31 + qHash(p.ip());
    hashCode = hashCode * 31 + qHash(p.netmask());
    hashCode = hashCode * 31 + qHash(p.prefixLength());
    hashCode = hashCode * 31 + qHash(p.isLifetimeKnown());
    hashCode = hashCode * 31 + qHash(p.preferredLifetime().deadline());
    hashCode = hashCode * 31 + qHash(p.validityLifetime().deadline());
    hashCode = hashCode * 31 + qHash(p.isPermanent());
    return hashCode;
}

inline uint qHash(const QNetworkCacheMetaData& p){
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(p.isValid());
    hashCode = hashCode * 31 + qHash(p.url());
    hashCode = hashCode * 31 + qHash(p.lastModified());
    hashCode = hashCode * 31 + qHash(p.expirationDate());
    hashCode = hashCode * 31 + qHash(p.saveToDisk());
    QNetworkCacheMetaData::RawHeaderList rawHeaders = p.rawHeaders();
    hashCode = hashCode * 31 + qHash(rawHeaders.size());
    for(const QNetworkCacheMetaData::RawHeader & i : rawHeaders){
        hashCode = hashCode * 31 + qHash(i.first);
        hashCode = hashCode * 31 + qHash(i.second);
    }
    QNetworkCacheMetaData::AttributesMap attributes = p.attributes();
    hashCode = hashCode * 31 + qHash(attributes.size());
    for(QNetworkRequest::Attribute i : attributes.keys()){
        hashCode = hashCode * 31 + qHash(int(i));
    }
    return hashCode;
}

inline uint qHash(const QNetworkConfiguration& p){
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(int(p.state()));
    hashCode = hashCode * 31 + qHash(int(p.type()));
    hashCode = hashCode * 31 + qHash(int(p.purpose()));
    hashCode = hashCode * 31 + qHash(int(p.bearerType()));
    hashCode = hashCode * 31 + qHash(int(p.bearerTypeFamily()));
    hashCode = hashCode * 31 + qHash(p.bearerTypeName());
    hashCode = hashCode * 31 + qHash(p.identifier());
    hashCode = hashCode * 31 + qHash(p.isRoamingAvailable());
    hashCode = hashCode * 31 + qHash(p.name());
    hashCode = hashCode * 31 + qHash(p.isValid());
    hashCode = hashCode * 31 + qHash(p.connectTimeout());
    hashCode = hashCode * 31 + qHash(p.children().size());
    return hashCode;
}

inline uint qHash(const QNetworkCookie& p){
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(p.isSecure());
    hashCode = hashCode * 31 + qHash(p.isHttpOnly());
    hashCode = hashCode * 31 + qHash(p.isSessionCookie());
    hashCode = hashCode * 31 + qHash(p.expirationDate());
    hashCode = hashCode * 31 + qHash(p.domain());
    hashCode = hashCode * 31 + qHash(p.path());
    hashCode = hashCode * 31 + qHash(p.name());
    hashCode = hashCode * 31 + qHash(p.value());
    return hashCode;
}

inline uint qHash(const QNetworkProxy& p){
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(int(p.type()));
    hashCode = hashCode * 31 + qHash(p.capabilities());
    hashCode = hashCode * 31 + qHash(p.isCachingProxy());
    hashCode = hashCode * 31 + qHash(p.isTransparentProxy());
    hashCode = hashCode * 31 + qHash(p.user());
    hashCode = hashCode * 31 + qHash(p.password());
    hashCode = hashCode * 31 + qHash(p.hostName());
    hashCode = hashCode * 31 + qHash(p.port());
    for(const QByteArray & i : p.rawHeaderList()){
        hashCode = hashCode * 31 + qHash(i);
    }
    return hashCode;
}

inline uint qHash(const QNetworkProxyQuery& p){
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(int(p.queryType()));
    hashCode = hashCode * 31 + qHash(p.peerPort());
    hashCode = hashCode * 31 + qHash(p.peerHostName());
    hashCode = hashCode * 31 + qHash(p.localPort());
    hashCode = hashCode * 31 + qHash(p.protocolTag());
    hashCode = hashCode * 31 + qHash(p.url());
    return hashCode;
}

inline uint qHash(const QNetworkRequest& p){
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(p.url());
    hashCode = hashCode * 31 + qHash(quintptr(p.originatingObject()));
    hashCode = hashCode * 31 + qHash(int(p.priority()));
    hashCode = hashCode * 31 + qHash(p.maximumRedirectsAllowed());
    return hashCode;
}

inline uint qHash(const QAuthenticator& p){
    if(p.isNull())
        return 0;
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(p.user());
    hashCode = hashCode * 31 + qHash(p.password());
    hashCode = hashCode * 31 + qHash(p.realm());
    hashCode = hashCode * 31 + qHash(p.options().keys());
    return hashCode;
}

#ifndef QT_NO_SSL

inline uint qHash(const QSslKey& p){
    if(p.isNull())
        return 0;
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(p.type());
    hashCode = hashCode * 31 + qHash(quintptr(p.handle()));
    hashCode = hashCode * 31 + qHash(p.length());
    hashCode = hashCode * 31 + qHash(p.algorithm());
    return hashCode;
}

inline uint qHash(const QSslCipher& p){
    if(p.isNull())
        return 0;
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(p.name());
    hashCode = hashCode * 31 + qHash(quintptr(p.protocol()));
    hashCode = hashCode * 31 + qHash(p.usedBits());
    hashCode = hashCode * 31 + qHash(p.supportedBits());
    hashCode = hashCode * 31 + qHash(p.protocolString());
    hashCode = hashCode * 31 + qHash(p.encryptionMethod());
    hashCode = hashCode * 31 + qHash(p.keyExchangeMethod());
    hashCode = hashCode * 31 + qHash(p.authenticationMethod());
    return hashCode;
}

inline uint qHash(const QSslConfiguration& p){
    if(p.isNull())
        return 0;
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(p.ciphers());
    hashCode = hashCode * 31 + qHash(quintptr(p.protocol()));
    hashCode = hashCode * 31 + qHash(p.privateKey());
    hashCode = hashCode * 31 + qHash(p.sessionCipher());
    hashCode = hashCode * 31 + qHash(p.sessionTicket());
    hashCode = hashCode * 31 + qHash(p.caCertificates());
    hashCode = hashCode * 31 + qHash(p.ellipticCurves());
    hashCode = hashCode * 31 + qHash(p.peerVerifyMode());
    hashCode = hashCode * 31 + qHash(p.peerCertificate());
    hashCode = hashCode * 31 + qHash(p.peerVerifyDepth());
    hashCode = hashCode * 31 + qHash(p.sessionProtocol());
    hashCode = hashCode * 31 + qHash(p.localCertificate());
    hashCode = hashCode * 31 + qHash(p.ephemeralServerKey());
    hashCode = hashCode * 31 + qHash(p.ocspStaplingEnabled());
    hashCode = hashCode * 31 + qHash(p.allowedNextProtocols());
    hashCode = hashCode * 31 + qHash(p.peerCertificateChain());
    hashCode = hashCode * 31 + qHash(p.localCertificateChain());
    hashCode = hashCode * 31 + qHash(p.nextNegotiatedProtocol());
    hashCode = hashCode * 31 + qHash(p.diffieHellmanParameters());
    hashCode = hashCode * 31 + qHash(p.preSharedKeyIdentityHint());
    hashCode = hashCode * 31 + qHash(p.sessionTicketLifeTimeHint());
#if QT_CONFIG(dtls)
    hashCode = hashCode * 31 + qHash(p.dtlsCookieVerificationEnabled());
#endif // dtls
    hashCode = hashCode * 31 + qHash(p.nextProtocolNegotiationStatus());
    hashCode = hashCode * 31 + qHash(p.supportedCiphers());
    return hashCode;
}

#endif // def QT_NO_SSL

#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)

inline uint qHash(const QHttp2Configuration& p){
    uint hashCode = 1;
    hashCode = hashCode * 31 + qHash(p.serverPushEnabled());
    hashCode = hashCode * 31 + qHash(quintptr(p.huffmanCompressionEnabled()));
    hashCode = hashCode * 31 + qHash(p.sessionReceiveWindowSize());
    hashCode = hashCode * 31 + qHash(p.streamReceiveWindowSize());
    hashCode = hashCode * 31 + qHash(p.maxFrameSize());
    return hashCode;
}

#endif

#endif // QTJAMBI_NETWORK_H
