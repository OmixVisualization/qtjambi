/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
    precompiledHeader: "pch_p.h"
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
    }
    
    EnumType{
        name: "QAlertType"
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
            signature: "connectToHost(QString, quint16, QIODeviceBase::OpenMode,QAbstractSocket::NetworkLayerProtocol)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "int"
                }
            }
        }
        ModifyFunction{
            signature: "connectToHost(QHostAddress, quint16, QIODeviceBase::OpenMode)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "int"
                }
            }
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
            signature: "setSocketDescriptor(qintptr,QAbstractSocket::SocketState,QIODeviceBase::OpenMode)"
            access: Modification.NonFinal
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
            signature: "serverPort()const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "int"
                }
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
            since: 6.2
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
        ModifyFunction{
            signature: "serverPort()const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "int"
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
                                  "    Java::QtNetwork::QHostAddress$HostInfo::set_address(%env, %4, qtjambi_cast<jobject>(%env, std::move(host)));\n"+
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
            since: 6.2
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
        ModifyFunction{
            signature: "addPendingConnection(QLocalSocket*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
            since: 6.8
        }
        ModifyFunction{
            signature: "nextPendingConnection()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            since: 6.8
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
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "head(QNetworkRequest)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "post(QNetworkRequest,QIODevice*)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "post(QNetworkRequest,QByteArray)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "post(QNetworkRequest,QHttpMultiPart*)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "put(QNetworkRequest,QByteArray)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "put(QNetworkRequest,QIODevice*)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "put(QNetworkRequest,QHttpMultiPart*)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "deleteResource(QNetworkRequest)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
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
        ModifyFunction{
            signature: "setTransferTimeout(std::chrono::milliseconds)"
            ModifyArgument{
                index: 1
                ReplaceDefaultExpression{
                    expression: "java.time.Duration.ofMillis(io.qt.network.QNetworkRequest.TransferTimeoutConstant.DefaultTransferTimeoutConstant.value())"
                }
            }
            since: 6.7
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

        EnumType{
            name: "NetworkError"
        }
        ModifyFunction{
            signature: "hasRawHeader(const QByteArray &) const"
            ModifyArgument{
                index: 1
                AddImplicitCall{type: "java.lang.@NonNull String"}
            }
            until: 6.6
        }
        ModifyFunction{
            signature: "rawHeader(const QByteArray &) const"
            ModifyArgument{
                index: 1
                AddImplicitCall{type: "java.lang.@NonNull String"}
            }
            until: 6.6
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
            signature: "QHostAddress(const quint8*)"
            remove: RemoveFlag.All
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
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
            Include{
                fileName: "QtJambi/JObjectWrapper"
                location: Include.Global
            }
        }

        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: String.raw`
inline auto convertSlot(JNIEnv* _env, jobject _slot){
    return [slot = JObjectWrapper(_env, _slot)](const QHostInfo& info){
                    if(JniEnvironment env{200}){
                        QTJAMBI_TRY{
                            jobject _info = qtjambi_cast<jobject>(env, info);
                            Java::QtCore::QMetaObject$Slot1::invoke(env, slot.object(env), _info);
                        }QTJAMBI_CATCH(const JavaException& exn){
                            exn.report(env);
                        }QTJAMBI_TRY_END
                    }
                };
}

inline auto convertSlot(JNIEnv* _env, QObject*& qobject, jobject _receiver, jobject _slot){
    if(Java::QtCore::QObject::isInstanceOf(_env, _receiver)){
        qobject = qtjambi_cast<QObject*>(_env, _receiver);
    }
    return [slot = JObjectWrapper(_env, _slot), receiver = JObjectWrapper(_env, _receiver)](const QHostInfo& info){
        if(JniEnvironment env{200}){
            QTJAMBI_TRY{
                jobject _info = qtjambi_cast<jobject>(env, info);
                Java::QtCore::QMetaObject$Slot2::invoke(env, slot.object(env), receiver.object(env), _info);
            }QTJAMBI_CATCH(const JavaException& exn){
                exn.report(env);
            }QTJAMBI_TRY_END
        }
    };
}

inline auto convertSlot(JNIEnv* _env, jobject _receiver, jobject _slot){
    return [slot = JObjectWrapper(_env, _slot), receiver = JObjectWrapper(_env, _receiver)](const QHostInfo& info){
        if(JniEnvironment env{200}){
            QTJAMBI_TRY{
                jobject _info = qtjambi_cast<jobject>(env, info);
                Java::QtCore::QMetaObject$Slot2::invoke(env, slot.object(env), receiver.object(env), _info);
            }QTJAMBI_CATCH(const JavaException& exn){
                exn.report(env);
            }QTJAMBI_TRY_END
        }
    };
}
`}
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
                              "    }else slot = \"1\" + slot;\n"+
                              "}"}
            }
            until: 6.6
        }

        ModifyFunction{
            signature: "lookupHost(QString, const QObject*, const char*)"
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
                              "    }else slot = \"1\" + slot;\n"+
                              "}"}
            }
            since: 6.7
        }

        FunctionalType{
            name: "Slot"
            using: "std::function<void(QHostInfo)>"
            generate: false
        }

        FunctionalType{
            name: "ObjectSlot"
            using: "std::function<void(QVariant,QHostInfo)>"
            generate: false
        }

        FunctionalType{
            name: "QObjectSlot"
            using: "std::function<void(QObject*,QHostInfo)>"
            generate: false
        }

        ModifyFunction{
            signature: "lookupHost<Func>(const QString&,const QtPrivate::FunctionPointer::Object<Func>*,Func)"
            ModifyArgument{
                index: 0
                replaceType: "int"
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = %in;"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QHostInfo)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 3
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.core.QMetaObject$Slot1<@NonNull QHostInfo>"
                        contextParameter: 2
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertSlot(%env, %in);"}
                    }
                }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{
                        index: 1
                        metaName: "name"
                    }
                    ArgumentMap{
                        index: 2
                        metaName: "context"
                    }
                    ArgumentMap{
                        index: 3
                        metaName: "slot"
                    }
                    Text{content: "io.qt.core.QMetaMethod metaMethod = io.qt.core.QMetaMethod.fromMethod(java.util.Objects.requireNonNull(slot, \"Argument 'slot': null not expected.\"));\n"+
                                  "if(metaMethod!=null && metaMethod.isValid()) {\n"+
                                  "    if(metaMethod.parameterCount()!=1 && metaMethod.parameterType(0)!=io.qt.core.QMetaType.fromType(QHostInfo.class).id()) {\n"+
                                  "        throw new IllegalArgumentException(\"Method does not take a single QHostInfo argument: \"+metaMethod.cppMethodSignature());\n"+
                                  "    }\n"+
                                  "    io.qt.core.QObject object = QtJambi_LibraryUtilities.internal.lambdaContext(slot);\n"+
                                  "    if(context!=null && context==object) {\n"+
                                  "        switch(metaMethod.methodType()) {\n"+
                                  "        case Signal:\n"+
                                  "            return lookupHost(name, context, \"2\"+metaMethod.cppMethodSignature());\n"+
                                  "        case Method:\n"+
                                  "        case Slot:\n"+
                                  "            return lookupHost(name, context, \"1\"+metaMethod.cppMethodSignature());\n"+
                                  "        default:\n"+
                                  "            break;\n"+
                                  "        }\n"+
                                  "    }\n"+
                                  "}\n"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QVariant,QHostInfo)>"
                    isImplicit: true
                }
                AddTypeParameter{
                    name: "Receiver"
                }
                ModifyArgument{
                    index: 2
                    NoNullPointer{}
                    replaceType: "Receiver"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QObject* %out{nullptr};"}
                    }
                }
                ModifyArgument{
                    index: 3
                    NoNullPointer{}
                    replaceType: "io.qt.core.QMetaObject$Slot2<Receiver,@NonNull QHostInfo>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertSlot(%env, __qt_%2, %2, %in);"}
                    }
                }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{
                        index: 1
                        metaName: "name"
                    }
                    ArgumentMap{
                        index: 2
                        metaName: "context"
                    }
                    ArgumentMap{
                        index: 3
                        metaName: "slot"
                    }
                    Text{content: "io.qt.core.QMetaMethod metaMethod = io.qt.core.QMetaMethod.fromMethod(java.util.Objects.requireNonNull(slot, \"Argument 'slot': null not expected.\"));\n"+
                                  "if(metaMethod!=null && metaMethod.isValid()) {\n"+
                                  "    if(metaMethod.parameterCount()!=1 && metaMethod.parameterType(0)!=io.qt.core.QMetaType.fromType(QHostInfo.class).id()) {\n"+
                                  "        throw new IllegalArgumentException(\"Method does not take a single QHostInfo argument: \"+metaMethod.cppMethodSignature());\n"+
                                  "    }\n"+
                                  "    if(context instanceof io.qt.core.QObject) {\n"+
                                  "        switch(metaMethod.methodType()) {\n"+
                                  "        case Signal:\n"+
                                  "            return lookupHost(name, (io.qt.core.QObject)context, \"2\"+metaMethod.cppMethodSignature());\n"+
                                  "        case Method:\n"+
                                  "        case Slot:\n"+
                                  "            return lookupHost(name, (io.qt.core.QObject)context, \"1\"+metaMethod.cppMethodSignature());\n"+
                                  "        default:\n"+
                                  "            break;\n"+
                                  "        }\n"+
                                  "    }\n"+
                                  "}\n"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QObject*,QHostInfo)>"
                    isImplicit: true
                }
                AddTypeParameter{
                    name: "Receiver"
                    extending: "io.qt.core.QObject"
                }
                ModifyArgument{
                    index: 2
                    NoNullPointer{}
                    replaceType: "Receiver"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QObject* %out = qtjambi_cast<QObject*>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 3
                    NoNullPointer{}
                    replaceType: "io.qt.core.QMetaObject$Slot2<Receiver,@NonNull QHostInfo>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertSlot(%env, %2, %in);"}
                    }
                }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{
                        index: 1
                        metaName: "name"
                    }
                    ArgumentMap{
                        index: 2
                        metaName: "context"
                    }
                    ArgumentMap{
                        index: 3
                        metaName: "slot"
                    }
                    Text{content: "io.qt.core.QMetaMethod metaMethod = io.qt.core.QMetaMethod.fromMethod(java.util.Objects.requireNonNull(slot, \"Argument 'slot': null not expected.\"));\n"+
                                  "if(metaMethod!=null && metaMethod.isValid()) {\n"+
                                  "    if(metaMethod.parameterCount()!=1 && metaMethod.parameterType(0)!=io.qt.core.QMetaType.fromType(QHostInfo.class).id()) {\n"+
                                  "        throw new IllegalArgumentException(\"Method does not take a single QHostInfo argument: \"+metaMethod.cppMethodSignature());\n"+
                                  "    }\n"+
                                  "    switch(metaMethod.methodType()) {\n"+
                                  "    case Signal:\n"+
                                  "        return lookupHost(name, context, \"2\"+metaMethod.cppMethodSignature());\n"+
                                  "    case Method:\n"+
                                  "    case Slot:\n"+
                                  "        return lookupHost(name, context, \"1\"+metaMethod.cppMethodSignature());\n"+
                                  "    default:\n"+
                                  "        break;\n"+
                                  "    }\n"+
                                  "}\n"}
                }
            }
            until: 6.5
        }

        ModifyFunction{
            signature: "lookupHost<Functor>(const QString&,const QtPrivate::ContextTypeForFunctor::ContextType<Functor>*,Functor&&)"
            Instantiation{
                Argument{
                    type: "std::function<void(QHostInfo)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 3
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.core.QMetaObject$Slot1<@NonNull QHostInfo>"
                        contextParameter: 2
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertSlot(%env, %in);"}
                    }
                }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{
                        index: 2
                        metaName: "context"
                    }
                    ArgumentMap{
                        index: 3
                        metaName: "slot"
                    }
                    Text{content: "io.qt.core.QMetaMethod metaMethod = io.qt.core.QMetaMethod.fromMethod(java.util.Objects.requireNonNull(slot, \"Argument 'slot': null not expected.\"));\n"+
                                  "if(metaMethod!=null && metaMethod.isValid()) {\n"+
                                  "    if(metaMethod.parameterCount()!=1 && metaMethod.parameterType(0)!=io.qt.core.QMetaType.fromType(QHostInfo.class).id()) {\n"+
                                  "        throw new IllegalArgumentException(\"Method does not take a single QHostInfo argument: \"+metaMethod.cppMethodSignature());\n"+
                                  "    }\n"+
                                  "    io.qt.core.QObject object = QtJambi_LibraryUtilities.internal.lambdaContext(slot);\n"+
                                  "    if(context!=null && context==object) {\n"+
                                  "        switch(metaMethod.methodType()) {\n"+
                                  "        case Signal:\n"+
                                  "            return lookupHost(name, context, \"2\"+metaMethod.cppMethodSignature());\n"+
                                  "        case Method:\n"+
                                  "        case Slot:\n"+
                                  "            return lookupHost(name, context, \"1\"+metaMethod.cppMethodSignature());\n"+
                                  "        default:\n"+
                                  "            break;\n"+
                                  "        }\n"+
                                  "    }\n"+
                                  "}\n"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QVariant,QHostInfo)>"
                    isImplicit: true
                }
                AddTypeParameter{
                    name: "Receiver"
                }
                ModifyArgument{
                    index: 2
                    NoNullPointer{}
                    replaceType: "Receiver"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QObject* %out{nullptr};"}
                    }
                }
                ModifyArgument{
                    index: 3
                    NoNullPointer{}
                    replaceType: "io.qt.core.QMetaObject$Slot2<@NonNull Receiver,@NonNull QHostInfo>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertSlot(%env, __qt_%2, %2, %in);"}
                    }
                }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{
                        index: 2
                        metaName: "context"
                    }
                    ArgumentMap{
                        index: 3
                        metaName: "slot"
                    }
                    Text{content: "io.qt.core.QMetaMethod metaMethod = io.qt.core.QMetaMethod.fromMethod(slot);\n"+
                                  "if(metaMethod!=null && metaMethod.isValid()) {\n"+
                                  "    if(metaMethod.parameterCount()!=1 && metaMethod.parameterType(0)!=io.qt.core.QMetaType.fromType(QHostInfo.class).id()) {\n"+
                                  "        throw new IllegalArgumentException(\"Method does not take a single QHostInfo argument: \"+metaMethod.cppMethodSignature());\n"+
                                  "    }\n"+
                                  "    if(context instanceof io.qt.core.QObject) {\n"+
                                  "        switch(metaMethod.methodType()) {\n"+
                                  "        case Signal:\n"+
                                  "            return lookupHost(name, (io.qt.core.QObject)context, \"2\"+metaMethod.cppMethodSignature());\n"+
                                  "        case Method:\n"+
                                  "        case Slot:\n"+
                                  "            return lookupHost(name, (io.qt.core.QObject)context, \"1\"+metaMethod.cppMethodSignature());\n"+
                                  "        default:\n"+
                                  "            break;\n"+
                                  "        }\n"+
                                  "    }\n"+
                                  "}\n"}
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QObject*,QHostInfo)>"
                    isImplicit: true
                }
                AddTypeParameter{
                    name: "Receiver"
                    extending: "io.qt.core.QObject"
                }
                ModifyArgument{
                    index: 2
                    NoNullPointer{}
                    replaceType: "Receiver"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "QObject* %out = qtjambi_cast<QObject*>(%env, %in);"}
                    }
                }
                ModifyArgument{
                    index: 3
                    NoNullPointer{}
                    replaceType: "io.qt.core.QMetaObject$Slot2<@NonNull Receiver,@NonNull QHostInfo>"
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertSlot(%env, %2, %in);"}
                    }
                }
                InjectCode{
                    target: CodeClass.Java
                    position: Position.Beginning
                    ArgumentMap{
                        index: 2
                        metaName: "context"
                    }
                    ArgumentMap{
                        index: 3
                        metaName: "slot"
                    }
                    Text{content: "io.qt.core.QMetaMethod metaMethod = io.qt.core.QMetaMethod.fromMethod(slot);\n"+
                                  "if(metaMethod!=null && metaMethod.isValid()) {\n"+
                                  "    if(metaMethod.parameterCount()!=1 && metaMethod.parameterType(0)!=io.qt.core.QMetaType.fromType(QHostInfo.class).id()) {\n"+
                                  "        throw new IllegalArgumentException(\"Method does not take a single QHostInfo argument: \"+metaMethod.cppMethodSignature());\n"+
                                  "    }\n"+
                                  "    switch(metaMethod.methodType()) {\n"+
                                  "    case Signal:\n"+
                                  "        return lookupHost(name, context, \"2\"+metaMethod.cppMethodSignature());\n"+
                                  "    case Method:\n"+
                                  "    case Slot:\n"+
                                  "        return lookupHost(name, context, \"1\"+metaMethod.cppMethodSignature());\n"+
                                  "    default:\n"+
                                  "        break;\n"+
                                  "    }\n"+
                                  "}\n"}
                }
            }
            since: 6.6
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
            signature: "parseCookies(QByteArrayView)"
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
        }

        EnumType{
            name: "TransferTimeoutConstant"
        }

        EnumType{
            name: "Attribute"
            extensible: true
            RejectEnumValue{
                name: "HTTP2AllowedAttribute"
            }
            RejectEnumValue{
                name: "HTTP2WasUsedAttribute"
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

        InjectCode{
            Text{
                content: "public final static int DefaultTransferTimeout = TransferTimeoutConstant.DefaultTransferTimeoutConstant.value();"
            }
            since: 6.7
        }
        ModifyFunction{
            signature: "setTransferTimeout(std::chrono::milliseconds)"
            ModifyArgument{
                index: 1
                ReplaceDefaultExpression{
                    expression: "java.time.Duration.ofMillis(io.qt.network.QNetworkRequest.TransferTimeoutConstant.DefaultTransferTimeoutConstant.value())"
                }
            }
            since: 6.7
        }
        ModifyFunction{
            signature: "setRawHeader(const QByteArray &, const QByteArray &)"
            ModifyArgument{
                index: 1
                AddImplicitCall{type: "java.lang.@NonNull String"}
            }
            ModifyArgument{
                index: 2
                AddImplicitCall{type: "java.lang.@NonNull String"}
            }
        }
        ModifyFunction{
            signature: "rawHeader(QAnyStringView)const"
            ModifyArgument{
                index: 1
                replaceType: "io.qt.core.@NonNull QByteArrayView"
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QByteArrayView %out = qtjambi_cast<QByteArrayView>(%env, %in);"}
                }
            }
            ModifyArgument{
                index: 1
                AddImplicitCall{type: "io.qt.core.@NonNull QByteArray"}
                AddImplicitCall{type: "java.nio.@NonNull ByteBuffer"}
                AddImplicitCall{type: "java.lang.@NonNull CharSequence"}
                AddImplicitCall{type: "java.lang.@NonNull String"}
                AddImplicitCall{type: "byte @NonNull[]"}
            }
            since: 6.7
        }
        ModifyFunction{
            signature: "hasRawHeader(QAnyStringView)const"
            ModifyArgument{
                index: 1
                replaceType: "io.qt.core.@NonNull QByteArrayView"
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QByteArrayView %out = qtjambi_cast<QByteArrayView>(%env, %in);"}
                }
            }
            ModifyArgument{
                index: 1
                AddImplicitCall{type: "io.qt.core.@NonNull QByteArray"}
                AddImplicitCall{type: "java.nio.@NonNull ByteBuffer"}
                AddImplicitCall{type: "java.lang.@NonNull CharSequence"}
                AddImplicitCall{type: "java.lang.@NonNull String"}
                AddImplicitCall{type: "byte @NonNull[]"}
            }
            since: 6.7
        }
    }
    
    ValueType{
        name: "QSslCipher"
        ppCondition: "!defined(QT_NO_SSL)"
    }
    
    ValueType{
        name: "QSslPreSharedKeyAuthenticator"
        ppCondition: "!defined(QT_NO_SSL)"
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
        ModifyFunction{
            signature: "nameserverPort()const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "int"
                }
            }
        }
    }

    ValueType{
        name: "QDnsTlsAssociationRecord"
        EnumType{
            name: "CertificateUsage"
            RejectEnumValue{name: "PKIX_TA"}
            RejectEnumValue{name: "PKIX_EE"}
            RejectEnumValue{name: "DANE_TA"}
            RejectEnumValue{name: "DANE_EE"}
            RejectEnumValue{name: "PrivCert"}
        }
        EnumType{
            name: "Selector"
            RejectEnumValue{name: "Cert"}
            RejectEnumValue{name: "SPKI"}
            RejectEnumValue{name: "PrivSel"}
        }
        EnumType{
            name: "MatchingType"
            RejectEnumValue{name: "PrivMatch"}
        }
        since: 6.8
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
    }
    
    ValueType{
        name: "QOcspResponse"
    }
    
    EnumType{
        name: "QDtlsError"
    }
    
    EnumType{
        name: "QOcspCertificateStatus"
    }
    
    EnumType{
        name: "QOcspRevocationReason"
    }
    
    ObjectType{
        name: "QDtls"
        EnumType{
            name: "HandshakeState"
        }
        ValueType{
            name: "GeneratorParameters"
            javaName: "QDtlsClientVerifier.GeneratorParameters"
            generate: false
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
    }
    
    ValueType{
        name: "QHttp2Configuration"
    }

    ValueType{
        name: "QHttp1Configuration"
        since: [6, 5]
    }
    
    ObjectType{
        name: "QRestReply"
        since: 6.7
    }

    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: template baseclass 'QtPrivate::ContextTypeForFunctor::ContextType<Functor>' of '' is not known"}
    Template{
        name: "rest.comsumer.function"
        Text{content: "auto %out = [wrapper = JObjectWrapper(%env, %in)](QRestReply* reply){\n"+
                      "                    if(JniEnvironment env{200}){\n"+
                      "                        QTJAMBI_TRY{\n"+
                      "                            jobject _reply = qtjambi_cast<jobject>(env, reply);\n"+
                      "                            Java::Runtime::Consumer::accept(env, wrapper.object(env), _reply);\n"+
                      "                        }QTJAMBI_CATCH(const JavaException& exn){\n"+
                      "                            exn.report(env);\n"+
                      "                        }QTJAMBI_TRY_END\n"+
                      "                    }\n"+
                      "                };"}
    }

    ObjectType{
        name: "QRestAccessManager"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
            Include{
                fileName: "QtJambi/JObjectWrapper"
                location: Include.Global
            }
        }

        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: "auto convertConsumer(JNIEnv* _env, jobject _consumer){\n"+
                          "    return [consumer = JObjectWrapper(_env, _consumer)](QRestReply& reply){\n"+
                          "                    if(JniEnvironment env{200}){\n"+
                          "                        QTJAMBI_TRY{\n"+
                          "                            jobject _reply = qtjambi_cast<jobject>(env, &reply);\n"+
                          "                            InvalidateAfterUse inv(env, _reply);\n"+
                          "                            Java::Runtime::Consumer::accept(env, consumer.object(env), _reply);\n"+
                          "                        }QTJAMBI_CATCH(const JavaException& exn){\n"+
                          "                            exn.report(env);\n"+
                          "                        }QTJAMBI_TRY_END\n"+
                          "                    }\n"+
                          "                };\n"+
                          "}"}
        }

        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiNetwork.java"
                quoteAfterLine: "class QRestAccessManager___"
                quoteBeforeLine: "}// class"
            }
        }

        FunctionalType{
            name: "Callback"
            using: "std::function<void(QRestReply&)>"
            generate: false
        }

        ModifyFunction{
            signature: "sendCustomRequest(const QNetworkRequest&, const QByteArray &, const QByteArray &)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }

        ModifyFunction{
            signature: "sendCustomRequest(const QNetworkRequest&, const QByteArray &, QIODevice *)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }

        ModifyFunction{
            signature: "sendCustomRequest(const QNetworkRequest&, const QByteArray &, QHttpMultiPart *)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }

        ModifyFunction{
            signature: "deleteResource(const QNetworkRequest&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }

        ModifyFunction{
            signature: "head(const QNetworkRequest&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }

        ModifyFunction{
            signature: "get(const QNetworkRequest&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }

        ModifyFunction{
            signature: "get(const QNetworkRequest&,QIODevice*)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }

        ModifyFunction{
            signature: "get(const QNetworkRequest&,const QByteArray&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }

        ModifyFunction{
            signature: "get(const QNetworkRequest&,const QJsonDocument&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }

        ModifyFunction{
            signature: "post(const QNetworkRequest&,const QMap<QString,QVariant> &)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }

        ModifyFunction{
            signature: "post(const QNetworkRequest&,QHttpMultiPart*)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }

        ModifyFunction{
            signature: "post(const QNetworkRequest&,QIODevice*)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }

        ModifyFunction{
            signature: "post(const QNetworkRequest&,const QByteArray&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }

        ModifyFunction{
            signature: "post(const QNetworkRequest&,const QJsonDocument&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }

        ModifyFunction{
            signature: "put(const QNetworkRequest&,const QMap<QString,QVariant> &)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }

        ModifyFunction{
            signature: "put(const QNetworkRequest&,QHttpMultiPart*)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }

        ModifyFunction{
            signature: "put(const QNetworkRequest&,QIODevice*)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }

        ModifyFunction{
            signature: "put(const QNetworkRequest&,const QByteArray&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }

        ModifyFunction{
            signature: "put(const QNetworkRequest&,const QJsonDocument&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }

        ModifyFunction{
            signature: "patch(const QNetworkRequest&,const QMap<QString,QVariant>&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }

        ModifyFunction{
            signature: "patch(const QNetworkRequest&,QIODevice*)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }

        ModifyFunction{
            signature: "patch(const QNetworkRequest&,const QByteArray&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }

        ModifyFunction{
            signature: "patch(const QNetworkRequest&,const QJsonDocument&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }

        ModifyFunction{
            signature: "deleteResource<Functor,true>(const QNetworkRequest&,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 3
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 2
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }

        ModifyFunction{
            signature: "head<Functor,true>(const QNetworkRequest&,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 3
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 2
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }

        ModifyFunction{
            signature: "get<Functor,true>(const QNetworkRequest&,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 3
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 2
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }

        ModifyFunction{
            signature: "get<Functor,true>(const QNetworkRequest&,QIODevice*,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 4
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 3
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }

        ModifyFunction{
            signature: "get<Functor,true>(const QNetworkRequest&,const QByteArray&,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 4
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 3
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }

        ModifyFunction{
            signature: "get<Functor,true>(const QNetworkRequest&,const QJsonDocument&,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 4
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 3
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }

        ModifyFunction{
            signature: "patch<Functor,true>(const QNetworkRequest&,QIODevice*,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 4
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 3
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }

        ModifyFunction{
            signature: "patch<Functor,true>(const QNetworkRequest&,const QByteArray&,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 4
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 3
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }

        ModifyFunction{
            signature: "patch<Functor,true>(const QNetworkRequest&,const QJsonArray&,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 4
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 3
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }

        ModifyFunction{
            signature: "patch<Functor,true>(const QNetworkRequest&,const QJsonDocument&,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 4
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 3
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }

        ModifyFunction{
            signature: "patch<Functor,true>(const QNetworkRequest&,const QVariantMap&,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 4
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 3
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }

        ModifyFunction{
            signature: "patch<Functor,true>(QNetworkRequest,QMap<QString,QVariant>,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 4
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 3
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }

        ModifyFunction{
            signature: "post<Functor,true>(const QNetworkRequest&,QHttpMultiPart*,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 4
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 3
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }

        ModifyFunction{
            signature: "post<Functor,true>(const QNetworkRequest&,QIODevice*,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 4
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 3
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }

        ModifyFunction{
            signature: "post<Functor,true>(const QNetworkRequest&,const QByteArray&,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 4
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 3
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }

        ModifyFunction{
            signature: "post<Functor,true>(const QNetworkRequest&,const QJsonArray&,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 4
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 3
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }

        ModifyFunction{
            signature: "post<Functor,true>(const QNetworkRequest&,const QJsonDocument&,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 4
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 3
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }

        ModifyFunction{
            signature: "post<Functor,true>(const QNetworkRequest&,const QVariantMap&,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 4
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 3
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }

        ModifyFunction{
            signature: "post<Functor,true>(QNetworkRequest,QMap<QString,QVariant>,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 4
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 3
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }

        ModifyFunction{
            signature: "put<Functor,true>(const QNetworkRequest&,QHttpMultiPart*,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 4
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 3
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }

        ModifyFunction{
            signature: "put<Functor,true>(const QNetworkRequest&,QIODevice*,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 4
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 3
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }

        ModifyFunction{
            signature: "put<Functor,true>(const QNetworkRequest&,const QByteArray&,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 4
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 3
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }

        ModifyFunction{
            signature: "put<Functor,true>(const QNetworkRequest&,const QJsonArray&,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 4
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 3
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }

        ModifyFunction{
            signature: "put<Functor,true>(const QNetworkRequest&,const QJsonDocument&,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 4
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 3
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }

        ModifyFunction{
            signature: "put<Functor,true>(const QNetworkRequest&,const QVariantMap&,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 4
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 3
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }

        ModifyFunction{
            signature: "put<Functor,true>(QNetworkRequest,QMap<QString,QVariant>,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 4
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 3
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }

        ModifyFunction{
            signature: "sendCustomRequest<Functor,true>(const QNetworkRequest&,const QByteArray&,QHttpMultiPart*,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 5
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 4
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }

        ModifyFunction{
            signature: "sendCustomRequest<Functor,true>(const QNetworkRequest&,const QByteArray&,QIODevice*,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 5
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 4
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }

        ModifyFunction{
            signature: "sendCustomRequest<Functor,true>(const QNetworkRequest&,const QByteArray&,const QByteArray&,const QRestAccessManager::ContextTypeForFunctor<Functor>*,Functor&&)"
            threadAffinity: true
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
            Instantiation{
                Argument{
                    type: "std::function<void(QRestReply&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 5
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.QRestAccessManager$Callback"
                        contextParameter: 4
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertConsumer(%env, %in);"}
                    }
                }
            }
        }
        since: 6.7
    }

    ObjectType{
        name: "QNetworkRequestFactory"
        since: 6.7
    }

    ValueType{
        name: "QHttpHeaders"
        EnumType{
            name: "WellKnownHeader"
        }
        since: 6.7
    }

    ObjectType{
        name: "QFormDataBuilder"
        since: 6.8
    }

    ObjectType{
        name: "QFormDataPartBuilder"
        ModifyFunction{
            signature: "setBody(QByteArrayView,QAnyStringView,QAnyStringView)"
            ModifyArgument{
                index: 1
                InhibitImplicitCall{type: "io.qt.core.@NonNull QByteArray"}
            }
        }
        ModifyFunction{
            signature: "setBody<>(QByteArray,QAnyStringView,QAnyStringView)"
            noImplicitArguments: true
        }
        ModifyFunction{
            signature: "setBodyDevice(QIODevice*,QAnyStringView,QAnyStringView)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        since: 6.8
    }

    GlobalFunction{
        signature: "qt_qhostinfo_lookup(const QString&,QObject*,const char*,bool*,int*)"
        targetType: "QHostInfo"
        remove: RemoveFlag.All // is internal
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
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*', unmatched *type 'QFormDataPartBuilder::PrivateConstructor'"}
}
