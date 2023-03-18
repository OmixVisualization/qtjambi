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
    packageName: "io.qt.websockets"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtWebSockets"
    module: "qtjambi.websockets"
    description: "Provides WebSocket communication compliant with RFC 6455."

    NamespaceType{
        name: "QWebSocketProtocol"

        EnumType{
            name: "Version"
            RejectEnumValue{
                name: "VersionLatest"
            }
        }

        EnumType{
            name: "CloseCode"
        }
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
                NoNullPointer{
                }
            }
        }
        ModifyFunction{
            signature: "preSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *)"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
            }
        }
    }
    
    ObjectType{
        name: "QWebSocketServer"

        EnumType{name: "SslMode"}

        ModifyFunction{
            signature: "listen(const QHostAddress &, quint16)"
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
                NoNullPointer{
                }
            }
        }
        ModifyFunction{
            signature: "preSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *)"
            ModifyArgument{
                index: 1
                NoNullPointer{
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
            Delegate{
                name: "set"
                deprecated: true
            }
        }
    }
    
    ValueType{
        name: "QWebSocketHandshakeOptions"
        ModifyFunction{
            signature: "operator=(QWebSocketHandshakeOptions)"
            Delegate{
                name: "set"
                deprecated: true
            }
        }
        since: [6, 4]
    }
    
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QWebSocketCorsAuthenticator."}
}
