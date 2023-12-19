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
    packageName: "io.qt.httpserver"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtHttpServer"
    module: "qtjambi.httpserver"
    description: "Qt HTTP Server supports building an HTTP server into an application."
    
    Rejection{
        className: "QHttpServerAfterRequestViewTraits"
    }
    
    Rejection{
        className: "QHttpServerRouterViewTraits"
    }
    
    ObjectType{
        name: "QAbstractHttpServer"
        ModifyFunction{
            signature: "sslSetup(QSslConfiguration)"
            ppCondition: "QT_CONFIG(ssl)"
        }
        ModifyFunction{
            signature: "sslSetup(QSslCertificate,QSslKey,QSsl::SslProtocol)"
            ppCondition: "QT_CONFIG(ssl)"
        }
        ModifyFunction{
            signature: "newWebSocketConnection()"
            ppCondition: "defined(QT_WEBSOCKETS_LIB)"
        }
        ModifyFunction{
            signature: "hasPendingWebSocketConnections() const"
            ppCondition: "defined(QT_WEBSOCKETS_LIB)"
        }
        ModifyFunction{
            signature: "nextPendingWebSocketConnection()"
            ppCondition: "defined(QT_WEBSOCKETS_LIB)"
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
                index: 0
                ReplaceType{
                    modifiedType: "int"
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                Text{content: "if(port<0 || port>=0x0ffff){\n"+
                              "throw new IllegalArgumentException(\"Port out of range: \"+port);\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "serverPorts()"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.core.@NonNull QList<java.lang.@QtPrimitiveType@NonNull Integer>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QList<int> tmp;\n"+
                                  "for(ushort s : %in){\n"+
                                  "tmp << int(s);\n"+
                                  "}\n"+
                                  "%out = qtjambi_cast<jobject>(%env, tmp);"}
                }
            }
        }
        ModifyFunction{
            signature: "missingHandler(QHttpServerRequest,QTcpSocket*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
            until: [6,4]
        }
        ModifyFunction{
            signature: "handleRequest(QHttpServerRequest,QTcpSocket*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
            until: [6,4]
        }
        ModifyFunction{
            signature: "missingHandler(QHttpServerRequest,QHttpServerResponder&&)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
            since: [6,5]
        }
        ModifyFunction{
            signature: "handleRequest(QHttpServerRequest,QHttpServerResponder&)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
            since: [6,5]
        }
    }
    
    ObjectType{
        name: "QHttpServer"
        Rejection{
            className: "AfterRequestHandler"
        }
        ModifyFunction{
            signature: "route<Rule,Args...>(Args&&)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "afterRequest<ViewHandler>(ViewHandler&&)"
            remove: RemoveFlag.All
        }
        FunctionalType{
            name: "MissingHandler"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "router()"
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                Text{content: "if(__rcRouter!=null && !__rcRouter.isDisposed())\n"+
                              "    return __rcRouter;"}
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                Text{content: "__rcRouter = __qt_return_value;\n"+
                              "QtJambi_LibraryUtilities.internal.registerDependentObject(__rcRouter, this);"}
            }
        }
        InjectCode{
            target: CodeClass.Java
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiHttpServer.java"
                quoteAfterLine: "class QHttpServer___"
                quoteBeforeLine: "}// class"
            }
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiHttpServer.java"
                quoteAfterLine: "class QHttpServer_64__"
                quoteBeforeLine: "}// class"
                until: [6,4]
            }
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiHttpServer.java"
                quoteAfterLine: "class QHttpServer_65__"
                quoteBeforeLine: "}// class"
                since: [6,5]
            }
        }
    }
    
    ObjectType{
        name: "QHttpServerResponder"

        EnumType{
            name: "StatusCode"
        }
        ModifyFunction{
            signature: "writeBody(const char*, qint64)"
            ModifyArgument{
                index: 1
                AsBuffer{
                    lengthParameter: 2
                }
            }
        }
        ModifyFunction{
            signature: "write(QIODevice*,QByteArray,QHttpServerResponder::StatusCode)"
            ModifyArgument{
                index: 2
                AddImplicitCall{type: "java.lang.@NonNull String"}
            }
        }
        ModifyFunction{
            signature: "write(QByteArray,QByteArray,QHttpServerResponder::StatusCode)"
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
            signature: "writeHeader(QByteArray,QByteArray)"
            ModifyArgument{
                index: 1
                AddImplicitCall{type: "java.lang.@NonNull String"}
            }
            ModifyArgument{
                index: 2
                AddImplicitCall{type: "java.lang.@NonNull String"}
            }
        }
        InjectCode{
            target: CodeClass.Java
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiHttpServer.java"
                quoteAfterLine: "class QHttpServerResponder___"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ObjectType{
        name: "QHttpServerRequest"

        EnumType{
            name: "Method"
            RejectEnumValue{
                name: "GET"
            }
            RejectEnumValue{
                name: "PUT"
            }
            RejectEnumValue{
                name: "DELETE"
            }
            RejectEnumValue{
                name: "POST"
            }
            RejectEnumValue{
                name: "HEAD"
            }
            RejectEnumValue{
                name: "OPTIONS"
            }
            RejectEnumValue{
                name: "PATCH"
            }
            RejectEnumValue{
                name: "CONNECT"
            }
        }
        ModifyFunction{
            signature: "value(QByteArray)const"
            ModifyArgument{
                index: 1
                AddImplicitCall{type: "java.lang.@NonNull String"}
            }
        }
    }
    
    
    ObjectType{
        name: "QHttpServerResponse"
        ModifyFunction{
            signature: "QHttpServerResponse(const char*,QHttpServerResponder::StatusCode)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "addHeaders<Container>(Container)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QHttpServerResponse(QJsonArray, QHttpServerResponder::StatusCode)"
            ModifyArgument{
                index: 2
                ReplaceDefaultExpression{
                    expression: "QHttpServerResponder.StatusCode.Ok"
                }
            }
        }
        ModifyFunction{
            signature: "QHttpServerResponse(QJsonObject, QHttpServerResponder::StatusCode)"
            ModifyArgument{
                index: 2
                ReplaceDefaultExpression{
                    expression: "QHttpServerResponder.StatusCode.Ok"
                }
            }
        }
        ModifyFunction{
            signature: "QHttpServerResponse(QString, QHttpServerResponder::StatusCode)"
            ModifyArgument{
                index: 2
                ReplaceDefaultExpression{
                    expression: "QHttpServerResponder.StatusCode.Ok"
                }
            }
        }
        ModifyFunction{
            signature: "QHttpServerResponse(QByteArray, QHttpServerResponder::StatusCode)"
            ModifyArgument{
                index: 2
                ReplaceDefaultExpression{
                    expression: "QHttpServerResponder.StatusCode.Ok"
                }
            }
        }
        ModifyFunction{
            signature: "QHttpServerResponse(QByteArray, QByteArray, QHttpServerResponder::StatusCode)"
            ModifyArgument{
                index: 1
                AddImplicitCall{type: "java.lang.@NonNull String"}
            }
            ModifyArgument{
                index: 2
                AddImplicitCall{type: "java.lang.@NonNull String"}
            }
            ModifyArgument{
                index: 3
                ReplaceDefaultExpression{
                    expression: "QHttpServerResponder.StatusCode.Ok"
                }
            }
        }
        ModifyFunction{
            signature: "addHeader(QByteArray,QByteArray)"
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
            signature: "clearHeader(QByteArray)"
            ModifyArgument{
                index: 1
                AddImplicitCall{type: "java.lang.@NonNull String"}
            }
        }
        ModifyFunction{
            signature: "hasHeader(QByteArray)const"
            ModifyArgument{
                index: 1
                AddImplicitCall{type: "java.lang.@NonNull String"}
            }
        }
        ModifyFunction{
            signature: "headers(QByteArray)const"
            ModifyArgument{
                index: 1
                AddImplicitCall{type: "java.lang.@NonNull String"}
            }
        }
        ModifyFunction{
            signature: "hasHeader(QByteArray,QByteArray)const"
            ModifyArgument{
                index: 1
                AddImplicitCall{type: "java.lang.@NonNull String"}
            }
            ModifyArgument{
                index: 2
                AddImplicitCall{type: "java.lang.@NonNull String"}
            }
        }
    }
    
    ObjectType{
        name: "QHttpServerRouter"

        Rejection{functionName: "bind_front"}
        ModifyFunction{
            signature: "addConverter<Type>(QAnyStringView)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "addRule<ViewHandler,ViewTraits>(std::unique_ptr<QHttpServerRouterRule>)"
            remove: RemoveFlag.All
        }
        InjectCode{
            target: CodeClass.Java
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiHttpServer.java"
                quoteAfterLine: "class QHttpServerRouter___"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ObjectType{
        name: "QHttpServerRouterRule"
        FunctionalType{
            name: "RouterHandler"
            ExtraIncludes{
                Include{
                    fileName: "QtCore/QRegularExpressionMatch"
                    location: Include.Global
                }
                Include{
                    fileName: "QtNetwork/QTcpSocket"
                    location: Include.Global
                    until: [6,4]
                }
                Include{
                    fileName: "QtHttpServer/QHttpServerResponder"
                    location: Include.Global
                    since: [6,5]
                }
            }
        }
        ExtraIncludes{
            Include{
                fileName: "QtCore/QRegularExpressionMatch"
                location: Include.Global
            }
            Include{
                fileName: "QtNetwork/QTcpSocket"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "matches(QHttpServerRequest,QRegularExpressionMatch *) const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*', unmatched *type 'ViewTraits::BindableType'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*', unmatched *type 'QHttpServer::AfterRequestHandler'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: unsupported default value 'qMakePair(1u, 1u)' of argument in function 'writeStatusLine', class 'QHttpServerResponder'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Cannot find enum constant for value 'StatusCode::Ok' in '*' or any of its super classes"}
}
