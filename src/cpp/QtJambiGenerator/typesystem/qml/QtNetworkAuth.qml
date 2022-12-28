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
    packageName: "io.qt.network.auth"
    defaultSuperClass: "io.qt.QtObject"
    qtLibrary: "QtNetworkAuth"
    module: "qtjambi.networkauth"
    description: "Provides support for OAuth-based authorization to online services."
    EnumType{
        name: "QAbstractOAuth::Status"
    }
    
    EnumType{
        name: "QAbstractOAuth::Stage"
    }
    
    EnumType{
        name: "QAbstractOAuth::Error"
    }
    
    EnumType{
        name: "QAbstractOAuth::ContentType"
    }
    
    EnumType{
        name: "QOAuth1::SignatureMethod"
    }
    
    EnumType{
        name: "QOAuth1Signature::HttpRequestMethod"
    }
    
    ValueType{
        name: "QOAuth1Signature"
        ModifyFunction{
            signature: "operator=(const QOAuth1Signature &)"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "QAbstractOAuth"
        ModifyFunction{
            signature: "setNetworkAccessManager(QNetworkAccessManager*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcNetworkAccessManager"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setReplyHandler(QAbstractOAuthReplyHandler*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcReplyHandler"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "prepareRequest(QNetworkRequest *, const QByteArray &, const QByteArray &)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.network.QNetworkRequest"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QNetworkRequest *%out = qtjambi_cast<QNetworkRequest *>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jobject %out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
        }
    }
    
    ObjectType{
        name: "QAbstractOAuth2"
    }
    
    ObjectType{
        name: "QAbstractOAuthReplyHandler"
    }
    
    ObjectType{
        name: "QOAuth1"
        ModifyFunction{
            signature: "setup(QNetworkRequest *, const QMap<QString, QVariant> &, QNetworkAccessManager::Operation)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.network.QNetworkRequest"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QNetworkRequest *%out = qtjambi_cast<QNetworkRequest *>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jobject %out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "setup(QNetworkRequest *, const QMap<QString, QVariant> &, const QByteArray &)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.network.QNetworkRequest"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QNetworkRequest *%out = qtjambi_cast<QNetworkRequest *>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jobject %out = qtjambi_cast<jobject>(%env, %in);"}
                }
            }
        }
    }
    
    ObjectType{
        name: "QOAuth2AuthorizationCodeFlow"
    }
    
    ObjectType{
        name: "QOAuthHttpServerReplyHandler"
        ModifyFunction{
            signature: "listen(const QHostAddress &, unsigned short)"
            ModifyArgument{
                index: 1
                ReplaceDefaultExpression{
                    expression: "new io.qt.network.QHostAddress(io.qt.network.QHostAddress.SpecialAddress.Any)"
                }
            }
        }
    }
    
    ObjectType{
        name: "QOAuthOobReplyHandler"
    }
    
    FunctionalType{
        name: "QAbstractOAuth::ModifyParametersFunction"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QVariantMap"
                location: Include.Global
            }
        }
    }
    
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QOAuth1Signature."}
}
