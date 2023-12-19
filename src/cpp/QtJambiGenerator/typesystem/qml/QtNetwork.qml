/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of QtJambi.
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

import QtJambiGenerator 1.0

TypeSystem{
    packageName: "io.qt.network"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtNetwork"
    module: "qtjambi.network"
    description: "Classes to make network programming easier and more portable."
    Template{
        name: "network.read_char*_long_long"
        Text{content: "public final int read(byte data[]) {\n"+
                      "    if (data.length == 0) return 0;\n"+
                      "    \n"+
                      "    QNativePointer np = new QNativePointer(QNativePointer.Type.Byte, data.length);\n"+
                      "    int len = (int) read(np, data.length);\n"+
                      "    for (int i=0; i<len; ++i) data[i] = np.byteAt(i);\n"+
                      "    \n"+
                      "    return len;\n"+
                      "}"}
    }
    
    RequiredLibrary{
        name: "QtDBus"
        mode: RequiredLibrary.ProvideOnly
        platforms: ["macos", "linux"]
    }
    
    Rejection{
        className: "sockaddr"
    }
    
    NamespaceType{
        name: "QSsl"
        Include{
            fileName: "qssl.h"
            location: Include.Global
        }

        EnumType{
            name: "SslOption"
        }

        EnumType{
            name: "SupportedFeature"
            since: [6, 1]
        }

        EnumType{
            name: "ImplementedClass"
            since: [6, 1]
        }

        EnumType{
            name: "KeyType"
        }

        EnumType{
            name: "EncodingFormat"
        }

        EnumType{
            name: "KeyAlgorithm"
        }

        EnumType{
            name: "SslProtocol"
        }

        EnumType{
            name: "AlternativeNameEntryType"
        }

        EnumType{
            name: "AlertType"
        }

        EnumType{
            name: "AlertLevel"
        }

        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiNetwork.java"
                quoteAfterLine: "class QSsl___"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    EnumType{
        name: "QAlertLevel"
        since: 6
    }
    
    EnumType{
        name: "QAlertType"
        since: 6
    }
    
    NamespaceType{
        name: "QPasswordDigestor"
    }
    
    ObjectType{
        name: "QNetworkInformation"
        since: [6, 1]

        EnumType{
            name: "Reachability"
        }

        EnumType{
            name: "TransportMedium"
        }

        EnumType{
            name: "Feature"
        }
    }
    
    ObjectType{
        name: "QAbstractNetworkCache"
        ModifyFunction{
            signature: "insert(QIODevice*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QNetworkDiskCache"
        ModifyFunction{
            signature: "insert(QIODevice*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ValueType{
        name: "QNetworkCacheMetaData"
    }
    
    ObjectType{
        name: "QNetworkProxyFactory"
        ModifyFunction{
            signature: "setApplicationProxyFactory(QNetworkProxyFactory*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    
    ValueType{
        name: "QNetworkProxyQuery"

        EnumType{
            name: "QueryType"
        }
    }
    
    ObjectType{
        name: "QAbstractSocket"

        EnumType{
            name: "NetworkLayerProtocol"
        }

        EnumType{
            name: "SocketError"
        }

        EnumType{
            name: "SocketState"
        }

        EnumType{
            name: "SocketType"
        }

        EnumType{
            name: "BindFlag"
        }

        EnumType{
            name: "PauseMode"
        }

        EnumType{
            name: "SocketOption"
        }

        ModifyFunction{
            signature: "setSocketOption(QAbstractSocket::SocketOption, const QVariant)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "socketOption(QAbstractSocket::SocketOption)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "proxyAuthenticationRequired(QNetworkProxy,QAuthenticator*)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
                NoNullPointer{
                }
            }
        }
        ModifyFunction{
            signature: "localPort()const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "int"
                }
            }
        }
        ModifyFunction{
            signature: "peerPort()const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "int"
                }
            }
        }
        ModifyFunction{
            signature: "setLocalPort(quint16)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "int"
                }
            }
        }
        ModifyFunction{
            signature: "setPeerPort(quint16)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "int"
                }
            }
        }
        ModifyFunction{
            signature: "connectToHost(QString, quint16, QIODevice::OpenMode,QAbstractSocket::NetworkLayerProtocol)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "int"
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "connectToHost(QString, quint16, QIODeviceBase::OpenMode,QAbstractSocket::NetworkLayerProtocol)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "int"
                }
            }
            since: 6
        }
        ModifyFunction{
            signature: "connectToHost(QHostAddress, quint16, QIODevice::OpenMode)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "int"
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "connectToHost(QHostAddress, quint16, QIODeviceBase::OpenMode)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "int"
                }
            }
            since: 6
        }
        ModifyFunction{
            signature: "abort()"
            access: Modification.NonFinal
        }
        ModifyFunction{
            signature: "flush()"
            access: Modification.NonFinal
        }
        ModifyFunction{
            signature: "setReadBufferSize(qint64)"
            access: Modification.NonFinal
        }
        ModifyFunction{
            signature: "setSocketDescriptor(qintptr,QAbstractSocket::SocketState,QIODevice::OpenMode)"
            access: Modification.NonFinal
            until: 5
        }
        ModifyFunction{
            signature: "setSocketDescriptor(qintptr,QAbstractSocket::SocketState,QIODeviceBase::OpenMode)"
            access: Modification.NonFinal
            since: 6
        }
        ModifyFunction{
            signature: "waitForConnected(int)"
            access: Modification.NonFinal
        }
        ModifyFunction{
            signature: "waitForDisconnected(int)"
            access: Modification.NonFinal
        }
        ModifyFunction{
            signature: "bind(QHostAddress, quint16, QAbstractSocket::BindMode)"
            ModifyArgument{
                index: 1
                noImplicitCalls: true
            }
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "int"
                }
            }
        }
        ModifyFunction{
            signature: "bind(quint16, QAbstractSocket::BindMode)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "int"
                }
            }
        }
    }
    
    
    
    ObjectType{
        name: "QTcpServer"
        ModifyFunction{
            signature: "addPendingConnection(QTcpSocket*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "listen(QHostAddress,quint16)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "int"
                }
            }
            ModifyArgument{
                index: 1
                ReplaceDefaultExpression{
                    expression: "new io.qt.network.QHostAddress(io.qt.network.QHostAddress.SpecialAddress.Any)"
                }
            }
        }
        ModifyFunction{
            signature: "waitForNewConnection(int,bool*)"
            throwing: "QTimeoutException"
            ModifyArgument{
                index: 1
                RemoveDefaultExpression{
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool ok = false;\n"+
                                  "bool* %out = &ok;"}
                }
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.End
                Text{content: "if(!ok)\n"+
                              "    Java::QtNetwork::QTimeoutException::throwNew(%env, \"Connection timed out\" QTJAMBI_STACKTRACEINFO );"}
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiNetwork.java"
                quoteAfterLine: "class Server___"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ObjectType{
        name: "QTcpSocket"
        ModifyFunction{
            signature: "bind(QHostAddress::SpecialAddress, quint16, QAbstractSocket::BindMode)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "int"
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QSslServer"
        ModifyFunction{
            signature: "preSharedKeyAuthenticationRequired(QSslSocket*,QSslPreSharedKeyAuthenticator*)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
                NoNullPointer{
                }
            }
        }
        since: [6, 4]
    }
    
    ObjectType{
        name: "QUdpSocket"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiNetwork.java"
                quoteAfterLine: "class QUdpSocket___"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "readDatagram(char*,qint64,QHostAddress*,quint16*)"
            ModifyArgument{
                index: 0
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = jlong(%in);\n"+
                                  "if(%4){\n"+
                                  "    Java::QtNetwork::QHostAddress$HostInfo::set_address(%env, %4, qtjambi_cast<jobject>(%env, host));\n"+
                                  "    Java::QtNetwork::QHostAddress$HostInfo::set_port(%env, %4, jshort(port));\n"+
                                  "}"}
                }
            }
            ModifyArgument{
                index: 1
                AsBuffer{
                    lengthParameter: 2
                }
                AddImplicitCall{type: "byte @NonNull[]"}
                AddImplicitCall{type: "io.qt.core.@NonNull QByteArray"}
            }
            ModifyArgument{
                index: 3
                RemoveDefaultExpression{
                }
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QHostAddress host;\n"+
                                  "QHostAddress* %out = %2 ? &host : nullptr;"}
                }
            }
            ModifyArgument{
                index: 4
                ReplaceType{
                    modifiedType: "io.qt.network.QUdpSocket$HostInfo"
                }
                ReplaceDefaultExpression{
                    expression: "null"
                }
                rename: "hostInfo"
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "ushort port(0);\n"+
                                  "ushort* %out = %2 ? &port : nullptr;"}
                }
            }
        }
        ModifyFunction{
            signature: "writeDatagram(const char*,qint64,QHostAddress,quint16)"
            ModifyArgument{
                index: 1
                AsBuffer{
                    lengthParameter: 2
                }
            }
        }
        ModifyFunction{
            signature: "bind(QHostAddress::SpecialAddress, quint16, QAbstractSocket::BindMode)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "int"
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QHttpPart"
        ModifyFunction{
            signature: "operator=(const QHttpPart &)"
            Delegate{
                name: "set"
                deprecated: true
            }
        }
        ModifyFunction{
            signature: "setBodyDevice(QIODevice*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    
    ObjectType{
        name: "QHttpMultiPart"

        EnumType{
            name: "ContentType"
        }
    }
    
    
    
    ObjectType{
        name: "QLocalServer"
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        EnumType{
            name: "SocketOption"
        }
        ModifyFunction{
            signature: "waitForNewConnection(int,bool*)"
            throwing: "QTimeoutException"
            ModifyArgument{
                index: 1
                RemoveDefaultExpression{
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool ok = false;\n"+
                                  "bool* %out = &ok;"}
                }
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.End
                Text{content: "if(!ok)\n"+
                              "    Java::QtNetwork::QTimeoutException::throwNew(%env, \"Connection timed out\" QTJAMBI_STACKTRACEINFO );"}
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiNetwork.java"
                quoteAfterLine: "class Server___"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ObjectType{
        name: "QLocalSocket"

        EnumType{
            name: "LocalSocketError"
        }

        EnumType{
            name: "LocalSocketState"
        }

        EnumType{
            name: "SocketOption"
            since: [6, 2]
        }
    }
    
    ObjectType{
        name: "QBearerEnginePlugin"
    }
    
    ObjectType{
        name: "QNetworkAccessManager"
        EnumType{
            name: "NetworkAccessibility"
        }
        EnumType{
            name: "Operation"
        }
        ModifyFunction{
            signature: "encrypted(QNetworkReply *)"
            ppCondition: "!defined(QT_NO_SSL)"
        }
        ModifyFunction{
            signature: "sslErrors(QNetworkReply *, const QList<QSslError> &)"
            ppCondition: "!defined(QT_NO_SSL)"
        }
        ModifyFunction{
            signature: "setCookieJar(QNetworkCookieJar*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setCache(QAbstractNetworkCache*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setProxyFactory(QNetworkProxyFactory*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "get(QNetworkRequest)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "head(QNetworkRequest)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "post(QNetworkRequest,QIODevice*)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "post(QNetworkRequest,QByteArray)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "post(QNetworkRequest,QHttpMultiPart*)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "put(QNetworkRequest,QByteArray)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "put(QNetworkRequest,QIODevice*)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "put(QNetworkRequest,QHttpMultiPart*)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "deleteResource(QNetworkRequest)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "connectToHostEncrypted(const QString &, quint16, const QSslConfiguration &)"
            threadAffinity: true
            ppCondition: "!defined(QT_NO_SSL)"
        }
        ModifyFunction{
            signature: "connectToHostEncrypted(const QString &, quint16, const QSslConfiguration &, const QString &)"
            threadAffinity: true
            ppCondition: "!defined(QT_NO_SSL)"
        }
        ModifyFunction{
            signature: "connectToHost(const QString &, quint16)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "sendCustomRequest(const QNetworkRequest &, const QByteArray &, QIODevice*)"
            ModifyArgument{
                index: 2
                AddImplicitCall{type: "java.lang.@NonNull String"}
            }
            threadAffinity: true
        }
        ModifyFunction{
            signature: "sendCustomRequest(const QNetworkRequest &, const QByteArray &, QByteArray)"
            ModifyArgument{
                index: 2
                AddImplicitCall{type: "java.lang.@NonNull String"}
            }
            threadAffinity: true
        }
        ModifyFunction{
            signature: "sendCustomRequest(const QNetworkRequest &, const QByteArray &, QHttpMultiPart*)"
            ModifyArgument{
                index: 2
                AddImplicitCall{type: "java.lang.@NonNull String"}
            }
            threadAffinity: true
        }
        ModifyFunction{
            signature: "cookieJar() const"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "createRequest(QNetworkAccessManager::Operation, const QNetworkRequest&, QIODevice*)"
            threadAffinity: true
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "proxyAuthenticationRequired(QNetworkProxy,QAuthenticator*)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
                NoNullPointer{
                }
            }
        }
        ModifyFunction{
            signature: "preSharedKeyAuthenticationRequired(QNetworkReply*,QSslPreSharedKeyAuthenticator*)"
            ppCondition: "!defined(QT_NO_SSL)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
                NoNullPointer{
                }
            }
        }
        ModifyFunction{
            signature: "authenticationRequired(QNetworkReply*,QAuthenticator*)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
                NoNullPointer{
                }
            }
        }
        ModifyFunction{
            signature: "proxyFactory()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QNetworkCookieJar"
        ExtraIncludes{
            Include{
                fileName: "hashes.h"
                location: Include.Local
            }
        }
    }
    
    ObjectType{
        name: "QNetworkReply"
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiNetwork.java"
                quoteAfterLine: "class QNetworkReply___"
                quoteBeforeLine: "}// class"
            }
        }

        EnumType{
            name: "NetworkError"
        }
        ModifyFunction{
            signature: "sslConfigurationImplementation(QSslConfiguration&) const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "preSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                NoNullPointer{
                }
            }
        }
    }
    
    ValueType{
        name: "QHostAddress"
        EnumType{
            name: "SpecialAddress"
        }

        EnumType{
            name: "ConversionModeFlag"
            since: [5, 8]
        }

        EnumType{
            name: "NetworkLayerProtocol"
            since: 7
        }

        Rejection{
            fieldName: "IPv4Protocol"
            since: [6, 2]
        }

        Rejection{
            fieldName: "IPv6Protocol"
            since: [6, 2]
        }

        Rejection{
            fieldName: "AnyIPProtocol"
            since: [6, 2]
        }

        Rejection{
            fieldName: "UnknownNetworkLayerProtocol"
            since: [6, 2]
        }
        ModifyFunction{
            signature: "QHostAddress(quint8*)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "QHostAddress(const quint8*)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setAddress(quint8*)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "setAddress(const quint8*)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "toIPv4Address(bool *) const"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QHostInfo"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JObjectWrapper"
                location: Include.Global
            }
        }
        InjectCode{
            target: CodeClass.Java
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiNetwork.java"
                quoteAfterLine: "class QHostInfo___"
                quoteBeforeLine: "}// class"
            }
        }

        EnumType{
            name: "HostInfoError"
        }

        ModifyFunction{
            signature: "lookupHost(QString, QObject*, const char*)"
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                ArgumentMap{
                    index: 3
                    metaName: "slot"
                }
                ArgumentMap{
                    index: 2
                    metaName: "dest"
                }
                Text{content: "if(slot!=null && !slot.startsWith(\"1\") && !slot.startsWith(\"2\")) {\n"+
                              "    io.qt.core.QMetaMethod method = dest.metaObject().method(slot);\n"+
                              "    if(method!=null && method.isValid()) {\n"+
                              "        if(method.methodType()==io.qt.core.QMetaMethod.MethodType.Signal)\n"+
                              "            slot = \"2\" + method.cppMethodSignature();\n"+
                              "        else\n"+
                              "            slot = \"1\" + method.cppMethodSignature();\n"+
                              "    }\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "lookupHost<Functor>(QString,Functor&&)"
            remove: RemoveFlag.All
            since: 6.6
        }
    }

    ValueType{
        name: "QNetworkAddressEntry"
        EnumType{
            name: "DnsEligibilityStatus"
        }
    }
    
    ValueType{
        name: "QNetworkInterface"

        EnumType{
            name: "InterfaceFlag"
        }

        EnumType{
            name: "InterfaceType"
            RejectEnumValue{
                name: "Ieee80211"
            }
        }
    }
    
    ValueType{
        name: "QNetworkProxy"
        noImplicitConstructors: true

        EnumType{
            name: "Capability"
        }

        EnumType{
            name: "ProxyType"
        }

        ModifyFunction{
            signature: "QNetworkProxy(QNetworkProxy::ProxyType,QString,quint16,QString,QString)"
            access: Modification.Private
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "int"
                }
            }
        }
        ModifyFunction{
            signature: "setPort(quint16)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "int"
                }
            }
        }
        ModifyFunction{
            signature: "port()const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "int"
                }
            }
        }
    }
    
    
    ValueType{
        name: "QIPv6Address"
        ModifyFunction{
            signature: "operator[](int)const"
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "index"
                }
                Text{content: "if(index<0 || index>15)\n"+
                              "    throw new ArrayIndexOutOfBoundsException(index);"}
            }
        }
        ModifyFunction{
            signature: "operator[](int)"
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "index"
                }
                Text{content: "if(index<0 || index>15)\n"+
                              "    throw new ArrayIndexOutOfBoundsException(index);"}
            }
        }
        InjectCode{
            target: CodeClass.Java
            Text{content: "public QIPv6Address(byte[] c){\n"+
                          "    this();\n"+
                          "    setC(c);\n"+
                          "}"}
        }
    }
    
    ValueType{
        name: "QAuthenticator"
        ModifyFunction{
            signature: "operator=(QAuthenticator)"
            Delegate{
                name: "set"
                deprecated: true
            }
        }
    }
    
    ValueType{
        name: "QNetworkCookie"

        EnumType{
            name: "RawForm"
        }

        EnumType{
            name: "SameSite"
            since: [6, 1]
        }
        ModifyFunction{
            signature: "parseCookies(QByteArray)"
            ModifyArgument{
                index: 1
                noImplicitCalls: true
            }
            since: 6.7
        }
    }
    
    ValueType{
        name: "QNetworkRequest"

        EnumType{
            name: "CacheLoadControl"
        }

        EnumType{
            name: "KnownHeaders"
        }

        EnumType{
            name: "LoadControl"
        }

        EnumType{
            name: "Priority"
        }

        EnumType{
            name: "RedirectPolicy"
            since: [5, 9]
        }

        EnumType{
            name: "TransferTimeoutConstant"
            since: [5, 15]
        }

        EnumType{
            name: "Attribute"
            extensible: true
            RejectEnumValue{
                name: "HTTP2AllowedAttribute"
                since: [5, 15]
            }
            RejectEnumValue{
                name: "HTTP2WasUsedAttribute"
                since: [5, 15]
            }
        }

        ModifyFunction{
            signature: "setOriginatingObject(QObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ValueType{
        name: "QSslCipher"
        ppCondition: "!defined(QT_NO_SSL)"
    }
    
    ValueType{
        name: "QSslPreSharedKeyAuthenticator"
        ppCondition: "!defined(QT_NO_SSL)"
        ModifyFunction{
            signature: "operator=(QSslPreSharedKeyAuthenticator)"
            Delegate{
                name: "set"
                deprecated: true
            }
        }
    }
    
    ValueType{
        name: "QSslEllipticCurve"
        ppCondition: "!defined(QT_NO_SSL)"
    }
    
    
    ValueType{
        name: "QSslError"
        ppCondition: "!defined(QT_NO_SSL)"
        EnumType{
            name: "SslError"
        }
    }
    
    ValueType{
        name: "QSslKey"
        ppCondition: "!defined(QT_NO_SSL)"
        Rejection{
            functionName: "backendImplementation"
        }
        ModifyFunction{
            signature: "QSslKey(Qt::HANDLE, QSsl::KeyType)"
            ModifyArgument{
                index: 1
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "Qt::HANDLE %out = reinterpret_cast<Qt::HANDLE>(%in);"}
                }
            }
        }
    }
    
    ValueType{
        name: "QSslCertificate"
        ppCondition: "!defined(QT_NO_SSL)"

        Rejection{
            functionName: "backendImplementation"
        }

        EnumType{
            name: "SubjectInfo"
        }

        EnumType{
            name: "PatternSyntax"
        }

        ModifyFunction{
            signature: "importPkcs12(QIODevice *, QSslKey *, QSslCertificate *, QList<QSslCertificate > *, const QByteArray &)"
            ModifyArgument{
                index: 2
                NoNullPointer{
                }
            }
        }
    }
    
    ValueType{
        name: "QSslCertificateExtension"
        ppCondition: "!defined(QT_NO_SSL)"
    }
    
    ValueType{
        name: "QSslConfiguration"

        EnumType{
            name: "NextProtocolNegotiationStatus"
        }
        ppCondition: "!defined(QT_NO_SSL)"
        ModifyFunction{
            signature: "dtlsCookieVerificationEnabled() const"
            ppCondition: "QT_CONFIG(dtls)"
        }
        ModifyFunction{
            signature: "setDtlsCookieVerificationEnabled(bool)"
            ppCondition: "QT_CONFIG(dtls)"
        }
        ModifyFunction{
            signature: "defaultDtlsConfiguration()"
            ppCondition: "QT_CONFIG(dtls)"
        }
        ModifyFunction{
            signature: "setDefaultDtlsConfiguration(const QSslConfiguration &)"
            ppCondition: "QT_CONFIG(dtls)"
        }
        ModifyField{
            name: "ALPNProtocolHTTP2"
            read: true
            write: false
            access: Modification.Private
        }
        ModifyField{
            name: "NextProtocolHttp1_1"
            read: true
            write: false
            access: Modification.Private
        }
        ModifyField{
            name: "NextProtocolSpdy3_0"
            read: true
            write: false
            access: Modification.Private
            until: 5
        }
        InjectCode{
            Text{content: "public static final String ALPNProtocolHTTP2;\n"+
                          "public static final String NextProtocolHttp1_1;\n"+
                          "static{\n"+
                          "    String _ALPNProtocolHTTP2 = null;\n"+
                          "    String _NextProtocolHttp1_1 = null;\n"+
                          "    try{\n"+
                          "        _ALPNProtocolHTTP2 = ALPNProtocolHTTP2();\n"+
                          "        _NextProtocolHttp1_1 = NextProtocolHttp1_1();\n"+
                          "    }catch(Throwable t){\n"+
                          "    }\n"+
                          "    ALPNProtocolHTTP2 = _ALPNProtocolHTTP2;\n"+
                          "    NextProtocolHttp1_1 = _NextProtocolHttp1_1;\n"+
                          "}"}
        }
        InjectCode{
            until: 5
            Text{content: "public static final String NextProtocolSpdy3_0;\n"+
                          "static{\n"+
                          "    String _NextProtocolSpdy3_0 = null;\n"+
                          "    try{\n"+
                          "    _NextProtocolSpdy3_0 = NextProtocolSpdy3_0();\n"+
                          "    }catch(Throwable t){\n"+
                          "    }\n"+
                          "    NextProtocolSpdy3_0 = _NextProtocolSpdy3_0;\n"+
                          "}"}
        }
        ModifyFunction{
            signature: "setBackendConfiguration(const QMap<QByteArray, QVariant> &)"
            ModifyArgument{
                index: 1
                ReplaceDefaultExpression{
                    expression: "java.util.Collections.emptyMap()"
                }
            }
        }
    }
    
    ValueType{
        name: "QSslDiffieHellmanParameters"
        ppCondition: "!defined(QT_NO_SSL)"

        EnumType{
            name: "Error"
        }
    }
    
    ObjectType{
        name: "QSslSocket"

        EnumType{
            name: "SslMode"
        }

        EnumType{
            name: "PeerVerifyMode"
        }
        ppCondition: "!defined(QT_NO_SSL)"
        ModifyFunction{
            signature: "preSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                NoNullPointer{
                }
            }
        }
    }
    
    ValueType{
        name: "QNetworkConfiguration"

        EnumType{
            name: "BearerType"
        }

        EnumType{
            name: "Purpose"
        }

        EnumType{
            name: "StateFlag"
        }

        EnumType{
            name: "Type"
        }
        until: 5
    }
    
    ObjectType{
        name: "QNetworkConfigurationManager"
        EnumType{
            name: "Capability"
        }
        until: 5
    }
    
    ObjectType{
        name: "QNetworkSession"
        EnumType{
            name: "SessionError"
        }
        EnumType{
            name: "State"
        }
        EnumType{
            name: "UsagePolicy"
        }
        ModifyFunction{
            signature: "interface()const"
            rename: "getInterface"
        }
        ModifyFunction{
            signature: "interface()const"
            rename: "getInterface"
        }
        until: 5
    }
    
    ObjectType{
        name: "QDnsLookup"

        EnumType{
            name: "Error"
        }

        EnumType{
            name: "Type"
        }

        ModifyFunction{
            signature: "QDnsLookup(QDnsLookup::Type, QString const&, QHostAddress const&, QObject*)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QDnsDomainNameRecord"
    }
    
    ValueType{
        name: "QDnsHostAddressRecord"
    }
    
    ValueType{
        name: "QDnsMailExchangeRecord"
    }
    
    ValueType{
        name: "QDnsServiceRecord"
    }
    
    ValueType{
        name: "QDnsTextRecord"
    }
    
    ValueType{
        name: "QNetworkDatagram"
        noImplicitConstructors: true
    }
    
    ValueType{
        name: "QHstsPolicy"
        EnumType{
            name: "PolicyFlag"
        }
        since: [5, 9]
    }
    
    ValueType{
        name: "QOcspResponse"
        since: [5, 13]
    }
    
    EnumType{
        name: "QDtlsError"
        since: [5, 12]
    }
    
    EnumType{
        name: "QOcspCertificateStatus"
        since: [5, 12]
    }
    
    EnumType{
        name: "QOcspRevocationReason"
        since: [5, 12]
    }
    
    ObjectType{
        name: "QDtls"
        EnumType{
            name: "HandshakeState"
            since: [5, 12]
        }
        ValueType{
            name: "GeneratorParameters"
            javaName: "QDtlsClientVerifier.GeneratorParameters"
            generate: false
            since: [5, 12]
        }
        ModifyFunction{
            signature: "handleTimeout(QUdpSocket *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "abortHandshake(QUdpSocket *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "doHandshake(QUdpSocket *, const QByteArray &)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "shutdown(QUdpSocket *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "writeDatagramEncrypted(QUdpSocket *, const QByteArray &)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "pskRequired(QSslPreSharedKeyAuthenticator*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
                NoNullPointer{
                }
            }
        }
        since: [5, 12]
    }
    
    ObjectType{
        name: "QDtlsClientVerifier"
        ValueType{
            name: "GeneratorParameters"
        }
        ModifyFunction{
            signature: "verifyClient(QUdpSocket *, const QByteArray &, const QHostAddress &, quint16)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        since: [5, 12]
    }
    
    ValueType{
        name: "QHttp2Configuration"
        since: [5, 14]
    }

    ValueType{
        name: "QHttp1Configuration"
        since: [6, 5]
    }
    
    
    SuppressedWarning{text: "*unmatched parameter type 'const sockaddr*"}
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: namespace 'io.qt.network.QSslSocket' for enum 'SslMode' is not declared"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: type 'QSslError' is specified in typesystem, but not defined. This could potentially lead to compilation errors."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: type 'QSslCipher' is specified in typesystem, but not defined. This could potentially lead to compilation errors."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: type 'QSslKey' is specified in typesystem, but not defined. This could potentially lead to compilation errors."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: namespace 'io.qt.network.QSslError' for enum 'SslError' is not declared"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: type 'QSslCertificate' is specified in typesystem, but not defined. This could potentially lead to compilation errors."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: namespace 'io.qt.network.QSslSocket' for enum 'PeerVerifyMode' is not declared"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: namespace 'io.qt.network.QSslCertificate' for enum 'SubjectInfo' is not declared"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: type 'QSslConfiguration' is specified in typesystem, but not defined. This could potentially lead to compilation errors."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: type 'QSslSocket' is specified in typesystem, but not defined. This could potentially lead to compilation errors."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: namespace 'io.qt.network.QSsl*' for enum '*' is not declared"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QDns*Record."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QDtlsClientVerifier::GeneratorParameters."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QHostInfo."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QIPv6Address."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QNetworkDatagram."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QNetworkInterface."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QSslCertificateExtension."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*', unmatched *type 'QNetworkInformationBackend*'"}
}
