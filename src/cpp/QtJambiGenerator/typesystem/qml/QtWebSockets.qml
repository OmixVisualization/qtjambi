/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
    packageName: "io.qt.websockets"
    defaultSuperClass: "io.qt.QtObject"
    qtLibrary: "QtWebSockets"
    module: "qtjambi.websockets"
    description: "Provides WebSocket communication compliant with RFC 6455."
    NamespaceType{
        name: "QWebSocketProtocol"
    }
    
    EnumType{
        name: "QWebSocketServer::SslMode"
    }
    
    EnumType{
        name: "QWebSocketProtocol::Version"
        RejectEnumValue{
            name: "VersionLatest"
        }
    }
    
    EnumType{
        name: "QWebSocketProtocol::CloseCode"
    }
    
    ObjectType{
        name: "QMaskGenerator"
    }
    
    ObjectType{
        name: "QWebSocket"
        ModifyFunction{
            signature: "ignoreSslErrors()"
            ppCondition: "!defined(QT_NO_SSL)"
        }
        ModifyFunction{
            signature: "setMaskGenerator(const QMaskGenerator*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcMaskGenerator"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "io.qt.network.QAuthenticator"
                }
                NoNullPointer{
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "%out = qtjambi_cast<jobject>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QAuthenticator* %out  = qtjambi_cast<QAuthenticator*>(%env, %scope, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "preSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.network.QSslPreSharedKeyAuthenticator"
                }
                NoNullPointer{
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "%out = qtjambi_cast<jobject>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QSslPreSharedKeyAuthenticator* %out  = qtjambi_cast<QSslPreSharedKeyAuthenticator*>(%env, %scope, %in);"}
                }
            }
        }
    }
    
    ObjectType{
        name: "QWebSocketServer"
        ModifyFunction{
            signature: "listen(const QHostAddress &, unsigned short)"
            ModifyArgument{
                index: 1
                ReplaceDefaultExpression{
                    expression: "new io.qt.network.QHostAddress(io.qt.network.QHostAddress.SpecialAddress.Any)"
                }
            }
        }
        ModifyFunction{
            signature: "originAuthenticationRequired(QWebSocketCorsAuthenticator *)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.websockets.QWebSocketCorsAuthenticator"
                }
                NoNullPointer{
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "%out = qtjambi_cast<jobject>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QWebSocketCorsAuthenticator* %out  = qtjambi_cast<QWebSocketCorsAuthenticator*>(%env, %scope, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "preSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.network.QSslPreSharedKeyAuthenticator"
                }
                NoNullPointer{
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "%out = qtjambi_cast<jobject>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QSslPreSharedKeyAuthenticator* %out  = qtjambi_cast<QSslPreSharedKeyAuthenticator*>(%env, %scope, %in);"}
                }
            }
        }
    }
    
    ValueType{
        name: "QWebSocketCorsAuthenticator"
        CustomConstructor{
            Text{content: "if(copy){\n"+
                          "    return new(placement) QWebSocketCorsAuthenticator(*copy);\n"+
                          "}else{\n"+
                          "    return new(placement) QWebSocketCorsAuthenticator(\"\");\n"+
                          "}"}
        }
        CustomConstructor{
            type: CustomConstructor.Default
            Text{content: "new(placement) QWebSocketCorsAuthenticator(\"\");"}
        }
        ModifyFunction{
            signature: "operator=(QWebSocketCorsAuthenticator)"
            rename: "set"
            ModifyArgument{
                index: 0
                NoNullPointer{
                }
                ReplaceType{
                    modifiedType: "void"
                }
            }
        }
    }
    
    ValueType{
        name: "QWebSocketHandshakeOptions"
        ModifyFunction{
            signature: "operator=(QWebSocketHandshakeOptions)"
            rename: "set"
            ModifyArgument{
                index: 0
                NoNullPointer{
                }
                ReplaceType{
                    modifiedType: "void"
                }
            }
        }
        since: [6, 4]
    }
    
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QWebSocketCorsAuthenticator."}
}
