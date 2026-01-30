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
    packageName: "io.qt.network.auth"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtNetworkAuth"
    module: "qtjambi.networkauth"
    description: "Provides support for OAuth-based authorization to online services."
    
    RequiredLibrary{
        name: "QtGui"
        mode: RequiredLibrary.ProvideOnly
        since: 6.8
    }

    ValueType{
        name: "QOAuth1Signature"
        EnumType{
            name: "HttpRequestMethod"
        }
    }
    
    ObjectType{
        name: "QAbstractOAuth"
        FunctionalType{
            name: "ModifyParametersFunction"
            ExtraIncludes{
                Include{
                    fileName: "QtCore/QVariantMap"
                    location: Include.Global
                }
            }
        }
        EnumType{
            name: "Status"
        }

        EnumType{
            name: "Stage"
        }

        EnumType{
            name: "Error"
        }

        EnumType{
            name: "ContentType"
        }
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
            signature: "prepareRequest(QNetworkRequest*,QByteArray,QByteArray)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "get(QUrl,const QMap<QString,QVariant>&)"
            threadAffinity: true
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
        }
        ModifyFunction{
            signature: "head(QUrl,const QMap<QString,QVariant>&)"
            threadAffinity: true
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
        }
        ModifyFunction{
            signature: "post(QUrl,const QMap<QString,QVariant>&)"
            threadAffinity: true
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
        }
        ModifyFunction{
            signature: "put(QUrl,const QMap<QString,QVariant>&)"
            threadAffinity: true
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
        }
        ModifyFunction{
            signature: "deleteResource(QUrl,const QMap<QString,QVariant>&)"
            threadAffinity: true
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
        }
    }

    ObjectType{
        name: "QOAuth2DeviceAuthorizationFlow"
        ModifyFunction{
            signature: "QOAuth2DeviceAuthorizationFlow(QNetworkAccessManager *, QObject *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcNetworkAccessManager"
                    action: ReferenceCount.Set
                }
            }
        }
        since: 6.9
    }

    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: template baseclass 'QtPrivate::ContextTypeForFunctor::ContextType<Functor>' of '' is not known"}
    ObjectType{
        name: "QAbstractOAuth2"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
            Include{
                fileName: "QtJambi/JObjectWrapper"
                location: Include.Global
            }
            since: 6.9
        }
        EnumType{
            name: "NonceMode"
            since: 6.9
        }
        TypeAliasType{
            name: "ContextTypeForFunctor"
        }

        FunctionalType{
            name: "Callback"
            using: "std::function<void(QNetworkRequest&, QAbstractOAuth::Stage)>"
            generate: false
            since: 6.9
        }
        ModifyFunction{
            signature: "setNetworkRequestModifier<Functor,true>(const QAbstractOAuth2::ContextTypeForFunctor<Functor>*, Functor&&)"
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
                    type: "std::function<void(QNetworkRequest&, QAbstractOAuth::Stage)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 2
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.network.auth.QAbstractOAuth2$Callback"
                        contextParameter: 1
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertBiConsumer(%env, %in);"}
                    }
                }
            }
            since: 6.9
        }
        ModifyFunction{
            signature: "refreshTokensImplementation()"
            access: Modification.NonFinal
            since: 6.9
        }
        InjectCode{
            Text{
                content: String.raw`
                /**
                 * Callback for setNetworkRequestModifier.
                 */
                public interface Callback extends java.util.function.@StrictNonNull BiConsumer<io.qt.network.@StrictNonNull QNetworkRequest, io.qt.network.auth.QAbstractOAuth.@NonNull Stage>, java.io.Serializable{
                }
                `
            }
            since: 6.9
        }
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: "auto convertBiConsumer(JNIEnv* _env, jobject _consumer){\n"+
                          "    return [consumer = JObjectWrapper(_env, _consumer)](QNetworkRequest& request,QAbstractOAuth::Stage stage){\n"+
                          "                    if(JniEnvironment env{200}){\n"+
                          "                        QTJAMBI_TRY{\n"+
                          "                            jobject _request = qtjambi_cast<jobject>(env, &request);\n"+
                          "                            InvalidateAfterUse inv(env, _request);\n"+
                          "                            Java::Runtime::BiConsumer::accept(env, consumer.object(env), _request, qtjambi_cast<jobject>(env, stage));\n"+
                          "                        }QTJAMBI_CATCH(const JavaException& exn){\n"+
                          "                            exn.report(env);\n"+
                          "                        }QTJAMBI_TRY_END\n"+
                          "                    }\n"+
                          "                };\n"+
                          "}"}
            since: 6.9
        }
    }
    
    ObjectType{
        name: "QAbstractOAuthReplyHandler"
    }
    
    ObjectType{
        name: "QOAuth1"
        EnumType{
            name: "SignatureMethod"
        }
    }
    
    ObjectType{
        name: "QOAuth2AuthorizationCodeFlow"
    }
    
    ObjectType{
        name: "QOAuthHttpServerReplyHandler"
        ModifyFunction{
            signature: "listen(QHostAddress, quint16)"
            ModifyArgument{
                index: 1
                ReplaceDefaultExpression{
                    expression: "new io.qt.network.QHostAddress(io.qt.network.QHostAddress.SpecialAddress.Any)"
                }
            }
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "int"
                }
            }
        }
        ModifyFunction{
            signature: "listen(QSslConfiguration, QHostAddress, quint16)"
            ModifyArgument{
                index: 2
                ReplaceDefaultExpression{
                    expression: "new io.qt.network.QHostAddress(io.qt.network.QHostAddress.SpecialAddress.Any)"
                }
            }
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "int"
                }
            }
            since: [6,10]
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

        InjectCode{
            Text{content: String.raw`
/**
 * @deprecated please use {@link #listen(io.qt.network.QHostAddress, int)} instead.
 */
@SuppressWarnings({"exports"})
@QtUninvokable
@Deprecated(forRemoval = true)
public final boolean listen(io.qt.network.QHostAddress.@NonNull SpecialAddress address, short port) {
    return listen(address, (int)port);
}

/**
 * @deprecated please use {@link #listen(io.qt.network.QHostAddress, int)} instead.
 */
@SuppressWarnings({"exports"})
@QtUninvokable
@Deprecated(forRemoval = true)
public final boolean listen(io.qt.network.@NonNull QHostAddress address, short port) {
    return listen(address, (int)port);
}
                `}
        }
    }
    
    ObjectType{
        name: "QOAuthOobReplyHandler"
    }

    ObjectType{
        name: "QOAuthUriSchemeReplyHandler"
        since: 6.8
    }
    
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QOAuth1Signature."}
}
