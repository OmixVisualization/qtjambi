/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
                since: 6.8
            }
            Include{
                fileName: "QtJambi/JObjectWrapper"
                location: Include.Global
                since: 6.8
            }
        }
        ModifyFunction{
            signature: "sslSetup(QSslConfiguration)"
            ppCondition: "QT_CONFIG(ssl)"
            until: 6.7
        }
        ModifyFunction{
            signature: "sslSetup(QSslCertificate,QSslKey,QSsl::SslProtocol)"
            ppCondition: "QT_CONFIG(ssl)"
            until: 6.7
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
            until: 6.7
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
                                  "%out = qtjambi_cast<jobject>(%env, std::move(tmp));"}
                }
            }
            until: 6.7
        }
        ModifyFunction{
            signature: "serverPorts()const"
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
                                  "%out = qtjambi_cast<jobject>(%env, std::move(tmp));"}
                }
            }
            since: 6.8
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
            until: 6.7
        }
        ModifyFunction{
            signature: "missingHandler(QHttpServerRequest,QHttpServerResponder&)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
            since: 6.8
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
        FunctionalType{
            name: "WebSocketUpgradeVerifier"
            using: "std::function<QHttpServerWebSocketUpgradeResponse(const QHttpServerRequest&)>"
            generate: false
            since: 6.8
        }
        ModifyFunction{
            signature: "addWebSocketUpgradeVerifier<Handler,true>(const QtPrivate::ContextTypeForFunctor::ContextType<Handler>*,Handler&&)"
            Instantiation{
                Argument{
                    type: "std::function<QHttpServerWebSocketUpgradeResponse(const QHttpServerRequest&)>"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 2
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.core.QMetaObject$Method1<@NonNull QHttpServerRequest, @NonNull QHttpServerWebSocketUpgradeResponse>"
                        contextParameter: 1
                    }
                    NoNullPointer{}
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
auto %out = [functor = JObjectWrapper(%env, %in)](const QHttpServerRequest& request) -> QHttpServerWebSocketUpgradeResponse {
        QHttpServerWebSocketUpgradeResponse response = QHttpServerWebSocketUpgradeResponse::deny();
        if(JniEnvironment env{200}){
            QTJAMBI_TRY{
                jobject result = Java::QtCore::QMetaObject$Method1::invoke(env, functor.object(env), qtjambi_cast<jobject>(env, request));
                response = qtjambi_cast<QHttpServerWebSocketUpgradeResponse>(env, result);
            }QTJAMBI_CATCH(const JavaException& exn){
                exn.report(env);
            }QTJAMBI_TRY_END
        }
        return response;
    };
                            `}
                    }
                }
            }
            since: 6.8
        }
    }

    ValueType{
        name: "QHttpServerWebSocketUpgradeResponse"
        EnumType{
            name: "ResponseType"
        }
        since: 6.8
    }

    ValueType{
        name: "QHttpServerConfiguration"
        since: 6.9
    }

    ValueType{
        name: "MissingHandler"
        generate: false
        since: 6.8
    }

    ValueType{
        name: "AfterRequestHandler"
        generate: false
        since: 6.8
    }

    ObjectType{
        name: "QHttpServer"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
            Include{
                fileName: "QtJambi/JObjectWrapper"
                location: Include.Global
            }
            since: 6.8
        }
        Rejection{
            className: "AfterRequestHandler"
        }
        ModifyFunction{
            signature: "route<Rule,Args...>(Args&&)"
            remove: RemoveFlag.All
            until: 6.7
        }
        ModifyFunction{
            signature: "afterRequest<ViewHandler>(ViewHandler&&)"
            remove: RemoveFlag.All
            until: 6.7
        }
        ModifyFunction{
            signature: "route<Rule,ViewHandler>(QString,ViewHandler&&)"
            remove: RemoveFlag.All
            since: 6.8
        }
        ModifyFunction{
            signature: "route<Rule,ViewHandler>(QString,QHttpServerRequest::Methods,ViewHandler&&)"
            remove: RemoveFlag.All
            since: 6.8
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
            until: 6.7
        }
        ModifyFunction{
            signature: "setMissingHandler<Handler,true>(const QtPrivate::ContextTypeForFunctor::ContextType<Handler>*,Handler&&)"
            Instantiation{
                Argument{
                    type: "MissingHandler"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 2
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.httpserver.QHttpServer$MissingHandlerHandler"
                        contextParameter: 1
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
auto %out = [slot = JObjectWrapper(%env, %in)](const QHttpServerRequest & request, QHttpServerResponder & responder){
                if(JniEnvironment env{200}){
                            QTJAMBI_TRY{
                                Java::QtCore::QMetaObject$Slot2::invoke(env, slot.object(env),
                                                    qtjambi_cast<jobject>(env, request),
                                                    qtjambi_cast<jobject>(env, responder));
                            }QTJAMBI_CATCH(const JavaException& exn){
                                exn.report(env);
                            }QTJAMBI_TRY_END
                }
            };
                            `}
                    }
                }
            }
            since: 6.8
        }
        ModifyFunction{
            signature: "addAfterRequestHandler<Handler,true>(const QtPrivate::ContextTypeForFunctor::ContextType<Handler>*,Handler&&)"
            Instantiation{
                Argument{
                    type: "AfterRequestHandler"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 2
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.httpserver.QHttpServer$AfterRequestHandler"
                        contextParameter: 1
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: String.raw`
auto %out = [slot = JObjectWrapper(%env, %in)](const QHttpServerRequest & request, QHttpServerResponse & responde){
                if(JniEnvironment env{200}){
                            QTJAMBI_TRY{
                                Java::QtCore::QMetaObject$Slot2::invoke(env, slot.object(env),
                                                    qtjambi_cast<jobject>(env, request),
                                                    qtjambi_cast<jobject>(env, responde));
                            }QTJAMBI_CATCH(const JavaException& exn){
                                exn.report(env);
                            }QTJAMBI_TRY_END
                }
            };
                            `}
                    }
                }
            }
            since: 6.8
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
        ModifyFunction{
            signature: "router()const"
            remove: RemoveFlag.All
            since: 6.8
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
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiHttpServer.java"
                quoteAfterLine: "class QHttpServer_65_7__"
                quoteBeforeLine: "}// class"
                since: [6,5]
                until: 6.7
            }
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiHttpServer.java"
                quoteAfterLine: "class QHttpServer_68__"
                quoteBeforeLine: "}// class"
                since: [6,8]
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
            until: 6.7
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
            until: 6.7
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
        ModifyFunction{
            signature: "localPort()const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "int"
                }
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
            until: 6.7
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
            until: 6.7
        }
        ModifyFunction{
            signature: "clearHeader(QByteArray)"
            ModifyArgument{
                index: 1
                AddImplicitCall{type: "java.lang.@NonNull String"}
            }
            until: 6.7
        }
        ModifyFunction{
            signature: "hasHeader(QByteArray)const"
            ModifyArgument{
                index: 1
                AddImplicitCall{type: "java.lang.@NonNull String"}
            }
            until: 6.7
        }
        ModifyFunction{
            signature: "headers(QByteArray)const"
            ModifyArgument{
                index: 1
                AddImplicitCall{type: "java.lang.@NonNull String"}
            }
            until: 6.7
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
            until: 6.7
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
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
            Include{
                fileName: "QtJambi/JObjectWrapper"
                location: Include.Global
            }
            since: 6.8
        }
        Rejection{functionName: "bindCaptured"}
        Rejection{functionName: "bindCapturedImpl"}
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
            until: 6.7
        }
        ModifyFunction{
            signature: "QHttpServerRouterRule<Handler,true>(QString, const QHttpServerRequest::Methods,const QtPrivate::ContextTypeForFunctor::ContextType<Handler>*,Handler&&)"
            Instantiation{
                Argument{
                    type: "RouterHandler"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 4
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.core.QMetaObject$Slot3<io.qt.core.@NonNull QRegularExpressionMatch,io.qt.httpserver.@NonNull QHttpServerRequest,io.qt.httpserver.@StrictNonNull QHttpServerResponder>"
                        contextParameter: 3
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertSlot(%env, %in);"}
                    }
                }
            }
            since: 6.8
        }
        ModifyFunction{
            signature: "QHttpServerRouterRule<Handler,true>(QString,const QtPrivate::ContextTypeForFunctor::ContextType<Handler>*,Handler&&)"
            Instantiation{
                Argument{
                    type: "RouterHandler"
                    isImplicit: true
                }
                ModifyArgument{
                    index: 3
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.core.QMetaObject$Slot3<io.qt.core.@NonNull QRegularExpressionMatch,io.qt.httpserver.@NonNull QHttpServerRequest,io.qt.httpserver.@StrictNonNull QHttpServerResponder>"
                        contextParameter: 2
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertSlot(%env, %in);"}
                    }
                }
            }
            Instantiation{
                Argument{
                    type: "RouterHandler"
                    isImplicit: true
                }
                AddTypeParameter{
                    name: "Receiver"
                    extending: "io.qt.core.QObject"
                }
                ModifyArgument{
                    index: 3
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.core.QMetaObject$Slot4<@NonNull Receiver, io.qt.core.@NonNull QRegularExpressionMatch,io.qt.httpserver.@NonNull QHttpServerRequest,io.qt.httpserver.@StrictNonNull QHttpServerResponder>"
                        contextParameter: 2
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertSlot(%env, %2, %in);"}
                    }
                }
            }
            Instantiation{
                Argument{
                    type: "RouterHandler"
                    isImplicit: true
                }
                AddTypeParameter{
                    name: "Receiver"
                }
                ModifyArgument{
                    index: 3
                    NoNullPointer{}
                    AsSlot{
                        targetType: "io.qt.core.QMetaObject$Slot4<@NonNull Receiver, io.qt.core.@NonNull QRegularExpressionMatch,io.qt.httpserver.@NonNull QHttpServerRequest,io.qt.httpserver.@StrictNonNull QHttpServerResponder>"
                        contextParameter: 2
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "auto %out = convertSlot(%env, __qt_%2, %2, %in);"}
                    }
                }
            }
            since: 6.8
        }
        ModifyFunction{
            signature: "bind_front<F,Args...>(F&&,Args&&)"
            remove: RemoveFlag.All
            since: 6.8
        }
        InjectCode{
            target: CodeClass.ShellDeclaration
            position: Position.Beginning
            Text{content: String.raw`
public:
    template <typename Handler>
    QHttpServerRouterRule_shell(
            const QString &pathPattern,
            const QObject *context,
            Handler &&func);
    template <typename Handler>
    QHttpServerRouterRule_shell(
            const QString &pathPattern, const QHttpServerRequest::Methods methods,
            const QObject *context,
            Handler &&func);
                `}
            since: 6.8
        }
        InjectCode{
            target: CodeClass.Native
            position: Position.End
            Text{content: String.raw`
inline auto convertSlot(JNIEnv* _env, jobject _slot){
    return [slot = JObjectWrapper(_env, _slot)](const QRegularExpressionMatch & regexp, const QHttpServerRequest & request, QHttpServerResponder & responder){
                    if(JniEnvironment env{200}){
                        QTJAMBI_TRY{
                            Java::QtCore::QMetaObject$Slot3::invoke(env, slot.object(env),
                                        qtjambi_cast<jobject>(env, regexp),
                                        qtjambi_cast<jobject>(env, request),
                                        qtjambi_cast<jobject>(env, responder));
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
    return [receiver = JObjectWrapper(_env, _receiver), slot = JObjectWrapper(_env, _slot)](const QRegularExpressionMatch & regexp, const QHttpServerRequest & request, QHttpServerResponder & responder){
                    if(JniEnvironment env{200}){
                        QTJAMBI_TRY{
                            Java::QtCore::QMetaObject$Slot4::invoke(env, slot.object(env),
                                        receiver.object(env),
                                        qtjambi_cast<jobject>(env, regexp),
                                        qtjambi_cast<jobject>(env, request),
                                        qtjambi_cast<jobject>(env, responder));
                        }QTJAMBI_CATCH(const JavaException& exn){
                            exn.report(env);
                        }QTJAMBI_TRY_END
                    }
                };
}
template <typename Handler>
QHttpServerRouterRule_shell::QHttpServerRouterRule_shell(
        const QString &pathPattern,
        const QObject *context,
        Handler &&func)
    : QHttpServerRouterRule(pathPattern, context, std::move(func))
{
    QTJAMBI_IN_CONSTRUCTOR_CALL("QHttpServerRouterRule_shell::QHttpServerRouterRule_shell(const QString& pathPattern, const QObject* context, Handler&& func)", this)
    QHttpServerRouterRule_shell::__shell()->constructed(typeid(QHttpServerRouterRule));
}

template <typename Handler>
QHttpServerRouterRule_shell::QHttpServerRouterRule_shell(
        const QString &pathPattern, const QHttpServerRequest::Methods methods,
        const QObject *context,
        Handler &&func)
    : QHttpServerRouterRule(pathPattern, methods, context, std::move(func))
{
    QTJAMBI_IN_CONSTRUCTOR_CALL("QHttpServerRouterRule_shell::QHttpServerRouterRule_shell(const QString& pathPattern, const QHttpServerRequest::Methods methods, const QObject* context, Handler&& func)", this)
    QHttpServerRouterRule_shell::__shell()->constructed(typeid(QHttpServerRouterRule));
}

// new QHttpServerRouterRule(QString,const QObject*,Handler&&)
void __qt_construct_QHttpServerRouterRule_cref_QString_const_QtPrivate_ContextTypeForFunctor_ContextType_Handler__ptr_rval_Handler(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments, QtJambiAPI::ConstructorOptions)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct QHttpServerRouterRule(QString,const QObject*,Handler&&)")
    QString __qt_pathPattern0 = qtjambi_cast<QString>(__jni_env, static_cast<jstring>(__java_arguments[0].l));
    QObject* __qt_context1 = qtjambi_cast<QObject*>(__jni_env, __java_arguments[1].l);
    auto __qt_func2 = convertSlot(__jni_env, __java_arguments[2].l);
    QHttpServerRouterRule_shell *__qt_this = new(__qtjambi_ptr) QHttpServerRouterRule_shell(__qt_pathPattern0, __qt_context1, std::move(__qt_func2));
    Q_UNUSED(__qt_this)
    Q_UNUSED(__jni_object)
}

// new QHttpServerRouterRule(QString,const QHttpServerRequest::Methods,const QObject*,Handler&&)
void __qt_construct_QHttpServerRouterRule_cref_QString_const_QHttpServerRequest_Methods_const_QtPrivate_ContextTypeForFunctor_ContextType_Handler__ptr_rval_Handler(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments, QtJambiAPI::ConstructorOptions)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct QHttpServerRouterRule(QString,const QHttpServerRequest::Methods,const QObject*,Handler&&)")
    QString __qt_pathPattern0 = qtjambi_cast<QString>(__jni_env, static_cast<jstring>(__java_arguments[0].l));
    QHttpServerRequest::Methods __qt_methods1 = qtjambi_cast<QHttpServerRequest::Methods>(__jni_env, __java_arguments[1].l);
    QObject* __qt_context2 = qtjambi_cast<QObject*>(__jni_env, __java_arguments[2].l);
    auto __qt_func3 = convertSlot(__jni_env, __java_arguments[3].l);
    QHttpServerRouterRule_shell *__qt_this = new(__qtjambi_ptr) QHttpServerRouterRule_shell(__qt_pathPattern0, __qt_methods1, __qt_context2, std::move(__qt_func3));
    Q_UNUSED(__qt_this)
    Q_UNUSED(__jni_object)
}

// new QHttpServerRouterRule(QString,const QHttpServerRequest::Methods,const QObject*,Handler&&)
void __qt_construct_QHttpServerRouterRule_cref_QString_const_QHttpServerRequest_Methods_const_QtPrivate_ContextTypeForFunctor_ContextType_Handler__ptr_rval_Handler_3(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments, QtJambiAPI::ConstructorOptions)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct QHttpServerRouterRule(QString,const QHttpServerRequest::Methods,const QObject*,Handler&&)")
    QString __qt_pathPattern0 = qtjambi_cast<QString>(__jni_env, static_cast<jstring>(__java_arguments[0].l));
    QObject* __qt_context2 = nullptr;
    auto __qt_func3 = convertSlot(__jni_env, __qt_context2, __java_arguments[1].l, __java_arguments[2].l);
    QHttpServerRouterRule_shell *__qt_this = new(__qtjambi_ptr) QHttpServerRouterRule_shell(__qt_pathPattern0, __qt_context2, std::move(__qt_func3));
    Q_UNUSED(__qt_this)
    Q_UNUSED(__jni_object)
}

// new QHttpServerRouterRule(QString,const QHttpServerRequest::Methods,const QObject*,Handler&&)
void __qt_construct_QHttpServerRouterRule_cref_QString_const_QHttpServerRequest_Methods_const_QtPrivate_ContextTypeForFunctor_ContextType_Handler__ptr_rval_Handler_4(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments, QtJambiAPI::ConstructorOptions)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct QHttpServerRouterRule(QString,const QHttpServerRequest::Methods,const QObject*,Handler&&)")
    QString __qt_pathPattern0 = qtjambi_cast<QString>(__jni_env, static_cast<jstring>(__java_arguments[0].l));
    QHttpServerRequest::Methods __qt_methods1 = qtjambi_cast<QHttpServerRequest::Methods>(__jni_env, __java_arguments[1].l);
    QObject* __qt_context2 = nullptr;
    auto __qt_func3 = convertSlot(__jni_env, __qt_context2, __java_arguments[2].l, __java_arguments[3].l);
    QHttpServerRouterRule_shell *__qt_this = new(__qtjambi_ptr) QHttpServerRouterRule_shell(__qt_pathPattern0, __qt_methods1, __qt_context2, std::move(__qt_func3));
    Q_UNUSED(__qt_this)
    Q_UNUSED(__jni_object)
}

// QHttpServerRouterRule::QHttpServerRouterRule(QString,const QObject*,Handler&&)
extern "C" JNIEXPORT void JNICALL Java_io_qt_httpserver_QHttpServerRouterRule_initialize_1native1
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jstring pathPattern0,
 jobject context1,
 jobject func2)
{
    QTJAMBI_NATIVE_METHOD_CALL("QHttpServerRouterRule::QHttpServerRouterRule(QString,const QObject*,Handler&&)")
    QTJAMBI_TRY {
        jvalue arguments[3];
        arguments[0].l = pathPattern0;
        arguments[1].l = context1;
        arguments[2].l = func2;
        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_construct_QHttpServerRouterRule_cref_QString_const_QtPrivate_ContextTypeForFunctor_ContextType_Handler__ptr_rval_Handler, sizeof(QHttpServerRouterRule_shell), typeid(QHttpServerRouterRule), 0, true, &deleter_QHttpServerRouterRule, arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QHttpServerRouterRule::QHttpServerRouterRule(QString,const QHttpServerRequest::Methods,const QObject*,Handler&&)
extern "C" JNIEXPORT void JNICALL Java_io_qt_httpserver_QHttpServerRouterRule_initialize_1native2
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jstring pathPattern0,
 jobject methods1,
 jobject context2,
 jobject func3)
{
    QTJAMBI_NATIVE_METHOD_CALL("QHttpServerRouterRule::QHttpServerRouterRule(QString,const QHttpServerRequest::Methods,const QObject*,Handler&&)")
    QTJAMBI_TRY {
        jvalue arguments[4];
        arguments[0].l = pathPattern0;
        arguments[1].l = methods1;
        arguments[2].l = context2;
        arguments[3].l = func3;
        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_construct_QHttpServerRouterRule_cref_QString_const_QHttpServerRequest_Methods_const_QtPrivate_ContextTypeForFunctor_ContextType_Handler__ptr_rval_Handler, sizeof(QHttpServerRouterRule_shell), typeid(QHttpServerRouterRule), 0, true, &deleter_QHttpServerRouterRule, arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QHttpServerRouterRule::QHttpServerRouterRule(QString,const QHttpServerRequest::Methods,const QObject*,Handler&&)
extern "C" JNIEXPORT void JNICALL Java_io_qt_httpserver_QHttpServerRouterRule_initialize_1native3
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jstring pathPattern0,
 jobject context2,
 jobject func3)
{
    QTJAMBI_NATIVE_METHOD_CALL("QHttpServerRouterRule::QHttpServerRouterRule(QString,const QHttpServerRequest::Methods,const QObject*,Handler&&)")
    QTJAMBI_TRY {
        jvalue arguments[3];
        arguments[0].l = pathPattern0;
        arguments[1].l = context2;
        arguments[2].l = func3;
        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_construct_QHttpServerRouterRule_cref_QString_const_QHttpServerRequest_Methods_const_QtPrivate_ContextTypeForFunctor_ContextType_Handler__ptr_rval_Handler_3, sizeof(QHttpServerRouterRule_shell), typeid(QHttpServerRouterRule), 0, true, &deleter_QHttpServerRouterRule, arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QHttpServerRouterRule::QHttpServerRouterRule(QString,const QHttpServerRequest::Methods,const QObject*,Handler&&)
extern "C" JNIEXPORT void JNICALL Java_io_qt_httpserver_QHttpServerRouterRule_initialize_1native4
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jstring pathPattern0,
 jobject methods1,
 jobject context2,
 jobject func3)
{
    QTJAMBI_NATIVE_METHOD_CALL("QHttpServerRouterRule::QHttpServerRouterRule(QString,const QHttpServerRequest::Methods,const QObject*,Handler&&)")
    QTJAMBI_TRY {
        jvalue arguments[4];
        arguments[0].l = pathPattern0;
        arguments[1].l = methods1;
        arguments[2].l = context2;
        arguments[3].l = func3;
        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_construct_QHttpServerRouterRule_cref_QString_const_QHttpServerRequest_Methods_const_QtPrivate_ContextTypeForFunctor_ContextType_Handler__ptr_rval_Handler_4, sizeof(QHttpServerRouterRule_shell), typeid(QHttpServerRouterRule), 0, true, &deleter_QHttpServerRouterRule, arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}
                `}
            since: 6.8
        }
        InjectCode{
            target: CodeClass.Java
            position: Position.End
            Text{content: String.raw`
public interface RouterHandler extends io.qt.core.QMetaObject.Slot3<io.qt.core.@NonNull QRegularExpressionMatch,io.qt.httpserver.@NonNull QHttpServerRequest,io.qt.httpserver.@StrictNonNull QHttpServerResponder>{
}

/**
 * <p>See <code>QHttpServerRouterRule::<wbr/>QHttpServerRouterRule&lt;Handler&gt;(QString,,<wbr/>Handler&amp;&amp;)</code></p>
 * @param pathPattern
 * @param func
 */
public QHttpServerRouterRule(java.lang.@NonNull String pathPattern, io.qt.core.QMetaObject.@StrictNonNull Slot3<io.qt.core.@NonNull QRegularExpressionMatch,io.qt.httpserver.@NonNull QHttpServerRequest,io.qt.httpserver.@StrictNonNull QHttpServerResponder> func){
    super((QPrivateConstructor)null);
    java.util.Objects.requireNonNull(func, "Argument 'func': null not expected.");
    initialize_native1(this, pathPattern, QtJambi_LibraryUtilities.internal.lambdaContext(func), func);
}

/**
 * <p>See <code>QHttpServerRouterRule::<wbr/>QHttpServerRouterRule&lt;Handler&gt;(QString,<wbr/>const QObject*,<wbr/>Handler&amp;&amp;)</code></p>
 * @param pathPattern
 * @param context
 * @param func
 */
public QHttpServerRouterRule(java.lang.@NonNull String pathPattern, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@StrictNonNull Slot3<io.qt.core.@NonNull QRegularExpressionMatch,io.qt.httpserver.@NonNull QHttpServerRequest,io.qt.httpserver.@StrictNonNull QHttpServerResponder> func){
    super((QPrivateConstructor)null);
    java.util.Objects.requireNonNull(func, "Argument 'func': null not expected.");
    initialize_native1(this, pathPattern, context, func);
}

/**
 * <p>See <code>QHttpServerRouterRule::<wbr/>QHttpServerRouterRule&lt;Handler&gt;(QString,<wbr/>const QHttpServerRequest::Methods,<wbr/>const QObject*,<wbr/>Handler&amp;&amp;)</code></p>
 * @param pathPattern
 * @param methods
 * @param func
 */
public QHttpServerRouterRule(java.lang.@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.QMetaObject.@StrictNonNull Slot3<io.qt.core.@NonNull QRegularExpressionMatch,io.qt.httpserver.@NonNull QHttpServerRequest,io.qt.httpserver.@StrictNonNull QHttpServerResponder> func){
    super((QPrivateConstructor)null);
    java.util.Objects.requireNonNull(func, "Argument 'func': null not expected.");
    initialize_native2(this, pathPattern, methods, QtJambi_LibraryUtilities.internal.lambdaContext(func), func);
}

/**
 * <p>See <code>QHttpServerRouterRule::<wbr/>QHttpServerRouterRule&lt;Handler&gt;(QString,<wbr/>const QHttpServerRequest::Methods,<wbr/>const QObject*,<wbr/>Handler&amp;&amp;)</code></p>
 * @param pathPattern
 * @param methods
 * @param context
 * @param func
 */
public QHttpServerRouterRule(java.lang.@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, io.qt.core.@Nullable QObject context, io.qt.core.QMetaObject.@StrictNonNull Slot3<io.qt.core.@NonNull QRegularExpressionMatch,io.qt.httpserver.@NonNull QHttpServerRequest,io.qt.httpserver.@StrictNonNull QHttpServerResponder> func){
    super((QPrivateConstructor)null);
    java.util.Objects.requireNonNull(func, "Argument 'func': null not expected.");
    initialize_native2(this, pathPattern, methods, context, func);
}

/**
 * <p>See <code>QHttpServerRouterRule::<wbr/>QHttpServerRouterRule&lt;Handler&gt;(QString,<wbr/>const QObject*,<wbr/>Handler&amp;&amp;)</code></p>
 * @param pathPattern
 * @param func
 */
public <Receiver> QHttpServerRouterRule(java.lang.@NonNull String pathPattern, @StrictNonNull Receiver receiver, io.qt.core.QMetaObject.@StrictNonNull Slot4<@NonNull Receiver,io.qt.core.@NonNull QRegularExpressionMatch,io.qt.httpserver.@NonNull QHttpServerRequest,io.qt.httpserver.@StrictNonNull QHttpServerResponder> func){
    super((QPrivateConstructor)null);
    java.util.Objects.requireNonNull(func, "Argument 'func': null not expected.");
    java.util.Objects.requireNonNull(func, "Argument 'receiver': null not expected.");
    initialize_native3(this, pathPattern, receiver, func);
}

/**
 * <p>See <code>QHttpServerRouterRule::<wbr/>QHttpServerRouterRule&lt;Handler&gt;(QString,<wbr/>const QHttpServerRequest::Methods,<wbr/>const QObject*,<wbr/>Handler&amp;&amp;)</code></p>
 * @param pathPattern
 * @param methods
 * @param func
 */
public <Receiver> QHttpServerRouterRule(java.lang.@NonNull String pathPattern, io.qt.httpserver.QHttpServerRequest.@NonNull Methods methods, @StrictNonNull Receiver receiver, io.qt.core.QMetaObject.@StrictNonNull Slot4<@NonNull Receiver,io.qt.core.@NonNull QRegularExpressionMatch,io.qt.httpserver.@NonNull QHttpServerRequest,io.qt.httpserver.@StrictNonNull QHttpServerResponder> func){
    super((QPrivateConstructor)null);
    java.util.Objects.requireNonNull(func, "Argument 'func': null not expected.");
    java.util.Objects.requireNonNull(func, "Argument 'receiver': null not expected.");
    initialize_native4(this, pathPattern, methods, receiver, func);
}

private native static void initialize_native1(QHttpServerRouterRule instance, java.lang.String pathPattern, io.qt.core.QObject context, io.qt.core.QMetaObject.Slot3<io.qt.core.QRegularExpressionMatch,io.qt.httpserver.QHttpServerRequest,io.qt.httpserver.QHttpServerResponder> func);
private native static void initialize_native2(QHttpServerRouterRule instance, java.lang.String pathPattern, io.qt.httpserver.QHttpServerRequest.Methods methods, io.qt.core.QObject context, io.qt.core.QMetaObject.Slot3<io.qt.core.QRegularExpressionMatch,io.qt.httpserver.QHttpServerRequest,io.qt.httpserver.QHttpServerResponder> func);
private native static <Receiver> void initialize_native3(QHttpServerRouterRule instance, java.lang.String pathPattern, Receiver context, io.qt.core.QMetaObject.Slot4<Receiver,io.qt.core.QRegularExpressionMatch,io.qt.httpserver.QHttpServerRequest,io.qt.httpserver.QHttpServerResponder> func);
private native static <Receiver> void initialize_native4(QHttpServerRouterRule instance, java.lang.String pathPattern, io.qt.httpserver.QHttpServerRequest.Methods methods, Receiver context, io.qt.core.QMetaObject.Slot4<Receiver,io.qt.core.QRegularExpressionMatch,io.qt.httpserver.QHttpServerRequest,io.qt.httpserver.QHttpServerResponder> func);
                `}
            since: 6.8
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
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'handleRequest(*)' in 'QHttpServer'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'missingHandler(*)' in 'QHttpServer'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: template method QHttpServerRouterRule::QHttpServerRouterRule* has uninstantiated parameters <Handler>"}
}
