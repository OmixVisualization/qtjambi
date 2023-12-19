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
    packageName: "io.qt.webengine.core"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtWebEngineCore"
    module: "qtjambi.webenginecore"
    Description{
        text: "Provides public API shared by both modules {@code qtjambi.webengine} and {@code qtjambi.webenginewidgets}."
        until: 5
    }

    Description{
        text: "Provides public API shared by both modules {@code qtjambi.webenginequick} and {@code qtjambi.webenginewidgets}."
        since: 6
    }

    Template{
        name: "webc.comsumer.function"
        Text{content: "std::function<void(%TYPE)> %out;\n"+
                      "if(%in){\n"+
                      "    JObjectWrapper wrapper(%env, %in);\n"+
                      "    %out = [wrapper](%TYPE value){\n"+
                      "                        if(JniEnvironment env{200}){\n"+
                      "                            jobject _value = qtjambi_cast<jobject>(env, value);\n"+
                      "                            Java::Runtime::Consumer::accept(env, wrapper.object(), _value);\n"+
                      "                        }\n"+
                      "                    };\n"+
                      "}"}
    }
    
    
    
    RequiredLibrary{
        name: "QtQuick"
    }
    
    RequiredLibrary{
        name: "QtWebChannel"
    }
    
    RequiredLibrary{
        name: "QtPositioning"
    }
    
    RequiredLibrary{
        name: "QtDBus"
        mode: RequiredLibrary.Optional
    }
    
    NamespaceType{
        name: "QtWebEngineCore"

        Rejection{
            className: "WebContentsAdapter"
        }
        ExtraIncludes{
            Include{
                fileName: "qwebenginecontextmenurequest.h"
                location: Include.Global
            }
            since: [6, 2]
        }

        EnumType{
            name: "ReferrerPolicy"
            RejectEnumValue{
                name: "Last"
            }
            since: [6, 2]
        }
    }
    
    GlobalFunction{
        signature: "qWebEngineChromiumSecurityPatchVersion()"
        targetType: "QtWebEngineCore"
    }
    
    GlobalFunction{
        signature: "qWebEngineChromiumVersion()"
        targetType: "QtWebEngineCore"
    }
    
    GlobalFunction{
        signature: "qWebEngineVersion()"
        targetType: "QtWebEngineCore"
    }

    GlobalFunction{
        signature: "qWebEngineGetDomainAndRegistry(QUrl)"
        targetType: "QtWebEngineCore"
        since: 6.6
    }
    
    Rejection{
        className: "QWebEngineNavigationRequest"
        functionName: "action"
    }
    
    Rejection{
        className: "QWebEngineNavigationRequest"
        functionName: "setAction"
    }
    
    Rejection{
        className: "QWebEngineNavigationRequest"
        enumName: "NavigationRequestAction"
    }
    
    ObjectType{
        name: "QWebEngineUrlRequestInfo"

        EnumType{
            name: "NavigationType"
        }

        EnumType{
            name: "ResourceType"
            RejectEnumValue{
                name: "ResourceTypeLast"
                since: [5, 14]
            }
        }
    }
    
    ValueType{
        name: "QWebEngineQuotaRequest"
    }
    
    ValueType{
        name: "QWebEngineFileSystemAccessRequest"

        EnumType{
            name: "HandleType"
        }

        EnumType{
            name: "AccessFlag"
        }
        since: [6, 4]
    }
    
    ObjectType{
        name: "QWebEngineCookieStore"
        ValueType{
            name: "FilterRequest"
            Rejection{
                className: "FilterRequest"
                fieldName: "_reservedFlag"
            }

            Rejection{
                className: "FilterRequest"
                fieldName: "_reservedType"
            }
        }

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
        ModifyFunction{
            signature: "setCookieFilter(std::function<bool(const QWebEngineCookieStore::FilterRequest&)>)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.util.function.@Nullable Predicate<@NonNull FilterRequest>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "std::function<bool(const QWebEngineCookieStore::FilterRequest &)> %out;\n"+
                                  "if(%in){\n"+
                                  "    JObjectWrapper wrapper(%env, %in);\n"+
                                  "    %out = [wrapper](const QWebEngineCookieStore::FilterRequest & value) -> bool {\n"+
                                  "                        if(JniEnvironment env{200}){\n"+
                                  "                            jobject _value = qtjambi_cast<jobject>(env, value);\n"+
                                  "                            return Java::Runtime::Predicate::test(env, wrapper.object(), _value);\n"+
                                  "                        }\n"+
                                  "                    return false;\n"+
                                  "                };\n"+
                                  "            }"}
                }
            }
        }
    }
    
    ObjectType{
        name: "QWebEngineUrlRequestInterceptor"
        ModifyFunction{
            signature: "interceptRequest(QWebEngineUrlRequestInfo&)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "QWebEngineUrlRequestJob"

        EnumType{
            name: "Error"
        }
    }
    
    ValueType{
        name: "QWebEngineUrlScheme"

        Rejection{enumName: "SpecialPort"}
        EnumType{
            name: "Flag"
        }
        EnumType{
            name: "Syntax"
        }
        ModifyFunction{
            signature: "operator=(QWebEngineUrlScheme)"
            Delegate{
                name: "set"
                deprecated: true
            }
        }
    }
    
    ValueType{
        name: "QWebEngineHttpRequest"
        EnumType{
            name: "Method"
        }
        ModifyFunction{
            signature: "operator=(QWebEngineHttpRequest)"
            Delegate{
                name: "set"
                deprecated: true
            }
        }
    }
    
    ValueType{
        name: "QWebEngineRegisterProtocolHandlerRequest"
    }
    
    ObjectType{
        name: "QWebEngineCallback"
        generate: false
    }
    
    ObjectType{
        name: "QWebEngineUrlSchemeHandler"
    }

    ObjectType{
        name: "QWebEngineWebAuthUXRequest"
        EnumType{
            name: "WebAuthUXState"
        }
        EnumType{
            name: "PINEntryReason"
        }
        EnumType{
            name: "PINEntryError"
        }
        EnumType{
            name: "RequestFailureReason"
        }
        since: 6.7
    }
    ObjectType{
        name: "QWebEngineWebAuthPINRequest"
        since: 6.7
    }

    ObjectType{
        name: "QWebEngineClientCertificateStore"
        ppCondition: "QT_CONFIG(ssl)"
    }
    
    ObjectType{
        name: "QWebEngineNotification"
    }

    ObjectType{
        name: "QWebEngineUrlResponseInfo"
        since: [6, 6]
    }

    ObjectType{
        name: "QWebEngineGlobalSettings"
        since: [6, 6]
    }
    
    ObjectType{
        name: "QWebEnginePage"
        ExtraIncludes{
            Include{
                fileName: "QtWebEngineCore/QWebEngineFileSystemAccessRequest"
                location: Include.Global
                since: [6, 4]
            }
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
            Include{
                fileName: "QtJambi/JObjectWrapper"
                location: Include.Global
            }
        }

        EnumType{
            name: "Feature"
        }
        EnumType{
            name: "FileSelectionMode"
        }
        EnumType{
            name: "FindFlag"
        }
        EnumType{
            name: "JavaScriptConsoleMessageLevel"
        }
        EnumType{
            name: "NavigationType"
        }
        EnumType{
            name: "PermissionPolicy"
        }
        EnumType{
            name: "RenderProcessTerminationStatus"
        }
        EnumType{
            name: "WebAction"
        }
        EnumType{
            name: "WebWindowType"
        }
        EnumType{
            name: "LifecycleState"
        }
        ModifyFunction{
            signature: "scripts()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
        }
        ModifyFunction{
            signature: "createWindow(QWebEnginePage::WebWindowType)"
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
            signature: "certificateError(QWebEngineCertificateError)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JObjectWrapper"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "findText(QString,QWebEnginePage::FindFlags,std::function<void(const QWebEngineFindTextResult &)>)"
            ModifyArgument{
                index: 3
                ReplaceDefaultExpression{
                    expression: "null"
                }
                ReplaceType{
                    modifiedType: "java.util.function.@Nullable Consumer<@NonNull QWebEngineFindTextResult>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    InsertTemplate{
                        name: "webc.comsumer.function"
                        Replace{
                            from: "%TYPE"
                            to: "const QWebEngineFindTextResult &"
                        }
                    }
                }
            }
        }
        ModifyFunction{
            signature: "toHtml(const std::function<void(const QString &)> &) const"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.util.function.@Nullable Consumer<@NonNull String>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    InsertTemplate{
                        name: "webc.comsumer.function"
                        Replace{
                            from: "%TYPE"
                            to: "const QString &"
                        }
                    }
                }
            }
        }
        ModifyFunction{
            signature: "toPlainText(const std::function<void(const QString &)> &) const"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.util.function.@Nullable Consumer<@NonNull String>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    InsertTemplate{
                        name: "webc.comsumer.function"
                        Replace{
                            from: "%TYPE"
                            to: "const QString &"
                        }
                    }
                }
            }
        }
        ModifyFunction{
            signature: "runJavaScript(QString, const std::function<void(const QVariant &)> &)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "java.util.function.@Nullable Consumer<@Nullable Object>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    InsertTemplate{
                        name: "webc.comsumer.function"
                        Replace{
                            from: "%TYPE"
                            to: "const QVariant &"
                        }
                    }
                }
            }
        }
        ModifyFunction{
            signature: "runJavaScript(QString, quint32, const std::function<void(const QVariant &)> &)"
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.util.function.@Nullable Consumer<@Nullable Object>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    InsertTemplate{
                        name: "webc.comsumer.function"
                        Replace{
                            from: "%TYPE"
                            to: "const QVariant &"
                        }
                    }
                }
            }
        }
        ModifyFunction{
            signature: "setDevToolsPage(QWebEnginePage*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDevToolsPage"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setInspectedPage(QWebEnginePage*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcInspectedPage"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setUrlRequestInterceptor(QWebEngineUrlRequestInterceptor*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcUrlRequestInterceptor"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setWebChannel(QWebChannel*,quint32)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcWebChannel"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "authenticationRequired(const QUrl&,QAuthenticator*)"
            ModifyArgument{
                index: 2
                NoNullPointer{
                }
            }
        }
        ModifyFunction{
            signature: "proxyAuthenticationRequired(const QUrl&,QAuthenticator*, const QString&)"
            ModifyArgument{
                index: 2
                NoNullPointer{
                }
            }
        }
        ModifyFunction{
            signature: "javaScriptPrompt(const QUrl &, const QString &, const QString &, QString *)"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.lang.@Nullable String"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jstring %out = nullptr;\n"+
                                  "if(%in){\n"+
                                  "    %out = qtjambi_cast<jstring>(%env, %4);\n"+
                                  "}"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "bool %out = %in;\n"+
                                  "if(%out && %in){\n"+
                                  "    *%4 = qtjambi_cast<QString>(%env, %in);\n"+
                                  "}"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QString %in;\n"+
                                  "QString* %out = &%in;"}
                }
            }
        }
        ModifyFunction{
            signature: "printToPdf(const QString &, const QPageLayout &, const QPageRanges &)"
            ModifyArgument{
                index: 2
                ReplaceDefaultExpression{
                    expression: "new io.qt.gui.QPageLayout(new io.qt.gui.QPageSize(io.qt.gui.QPageSize.PageSizeId.A4), io.qt.gui.QPageLayout.Orientation.Portrait, new io.qt.core.QMarginsF())"
                }
            }
        }
        ModifyFunction{
            signature: "printToPdf(const std::function<void(const QByteArray &)> &, const QPageLayout &, const QPageRanges &)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.util.function.@Nullable Consumer<io.qt.core.@NonNull QByteArray>"
                }
                NoNullPointer{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    InsertTemplate{
                        name: "web.comsumer.function"
                        Replace{
                            from: "%TYPE"
                            to: "const QByteArray &"
                        }
                    }
                }
            }
            ModifyArgument{
                index: 2
                ReplaceDefaultExpression{
                    expression: "new io.qt.gui.QPageLayout(new io.qt.gui.QPageSize(io.qt.gui.QPageSize.PageSizeId.A4), io.qt.gui.QPageLayout.Orientation.Portrait, new io.qt.core.QMarginsF())"
                }
            }
        }
        since: [6, 2]
    }
    
    ObjectType{
        name: "QWebEngineProfile"
        EnumType{
            name: "HttpCacheType"
        }
        EnumType{
            name: "PersistentCookiesPolicy"
        }

        FunctionalType{
            name: "IconAvailableCallback3"
            using: "std::function<void(const QIcon&,const QUrl&,const QUrl&)>"
            ExtraIncludes{
                Include{
                    fileName: "QtWebEngineCore/QWebEngineNotification"
                    location: Include.Global
                }
            }
        }

        FunctionalType{
            name: "IconAvailableCallback2"
            using: "std::function<void(const QIcon&,const QUrl&)>"
            ExtraIncludes{
                Include{
                    fileName: "QtWebEngineCore/QWebEngineNotification"
                    location: Include.Global
                }
            }
        }
        ExtraIncludes{
            Include{
                fileName: "QtWebEngineCore/QWebEngineNotification"
                location: Include.Global
            }
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
            Include{
                fileName: "QtJambi/JObjectWrapper"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "installUrlSchemeHandler(QByteArray,QWebEngineUrlSchemeHandler*)"
            ModifyArgument{
                index: 2
                ReferenceCount{
                    variableName: "__rcUrlSchemeHandler"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "removeUrlSchemeHandler(QWebEngineUrlSchemeHandler*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcUrlSchemeHandler"
                    action: ReferenceCount.Take
                }
            }
        }
        ModifyFunction{
            signature: "setUrlRequestInterceptor(QWebEngineUrlRequestInterceptor*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcUrlRequestInterceptor"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setNotificationPresenter(std::function<void(std::unique_ptr<QWebEngineNotification>)>)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.util.function.@Nullable Consumer<@NonNull QWebEngineNotification>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "std::function<void(std::unique_ptr<QWebEngineNotification>)> %out;\n"+
                                  "if(%in){\n"+
                                  "    JObjectWrapper wrapper(%env, %in);\n"+
                                  "    %out = [wrapper](std::unique_ptr<QWebEngineNotification> value){\n"+
                                  "                        if(JniEnvironment env{200}){\n"+
                                  "                            QtJambiScope __qtjambi_scope;\n"+
                                  "                            jobject _value = qtjambi_cast<jobject>(env, __qtjambi_scope, value);\n"+
                                  "                            Java::Runtime::Consumer::accept(env, wrapper.object(), _value);\n"+
                                  "                        }\n"+
                                  "                    };\n"+
                                  "}"}
                }
            }
        }
        since: [6, 2]
    }
    
    ValueType{
        name: "QWebEngineCertificateError"
        EnumType{
            name: "Error"
        }
        EnumType{
            name: "Type"
        }
        CustomConstructor{
            Text{content: "if(copy){\n"+
                          "    return new(placement) QWebEngineCertificateError(*copy);\n"+
                          "}else{\n"+
                          "    return nullptr;\n"+
                          "}"}
        }
        since: [6, 2]
    }
    
    ValueType{
        name: "QWebEngineScript"
        EnumType{
            name: "InjectionPoint"
        }
        EnumType{
            name: "ScriptWorldId"
        }
        ModifyFunction{
            signature: "operator=(QWebEngineScript)"
            Delegate{
                name: "set"
                deprecated: true
            }
        }
        since: [6, 2]
    }
    
    ObjectType{
        name: "QWebEngineSettings"
        EnumType{
            name: "FontFamily"
        }
        EnumType{
            name: "FontSize"
        }
        EnumType{
            name: "WebAttribute"
        }
        EnumType{
            name: "UnknownUrlSchemePolicy"
        }
        since: [6, 2]
    }
    
    ObjectType{
        name: "QWebEngineScriptCollection"
        since: [6, 2]
    }
    
    ValueType{
        name: "QWebEngineFullScreenRequest"
        ModifyFunction{
            signature: "operator=(QWebEngineFullScreenRequest)"
            Delegate{
                name: "set"
                deprecated: true
            }
        }
        since: [6, 2]
    }
    
    ValueType{
        name: "QWebEngineClientCertificateSelection"
        ModifyFunction{
            signature: "operator=(QWebEngineClientCertificateSelection)"
            Delegate{
                name: "set"
                deprecated: true
            }
        }
        since: [6, 2]
    }
    
    ObjectType{
        name: "QWebEngineHistory"
        since: [6, 2]
    }
    
    ObjectType{
        name: "QWebEngineHistoryModel"
        EnumType{
            name: "Roles"
        }
        since: [6, 2]
    }
    
    ValueType{
        name: "QWebEngineHistoryItem"
        CustomConstructor{
            Text{content: "if(copy){\n"+
                          "    return new(placement) QWebEngineHistoryItem(*copy);\n"+
                          "}else{\n"+
                          "    return nullptr;\n"+
                          "}"}
        }
        ModifyFunction{
            signature: "operator=(QWebEngineHistoryItem)"
            Delegate{
                name: "set"
                deprecated: true
            }
        }
        since: [6, 2]
    }
    
    ObjectType{
        name: "QWebEngineContextMenuRequest"
        EnumType{
            name: "MediaType"
        }
        EnumType{
            name: "MediaFlag"
        }
        EnumType{
            name: "EditFlag"
        }
        since: [6, 2]
    }
    
    ObjectType{
        name: "QWebEngineDownloadRequest"
        EnumType{
            name: "DownloadState"
        }
        EnumType{
            name: "DownloadType"
        }
        EnumType{
            name: "DownloadInterruptReason"
        }
        EnumType{
            name: "SavePageFormat"
        }
        since: [6, 2]
    }
    
    ObjectType{
        name: "QWebEngineNavigationRequest"
        EnumType{
            name: "NavigationType"
        }
        since: [6, 2]
    }
    
    ObjectType{
        name: "QWebEngineNewWindowRequest"

        EnumType{
            name: "DestinationType"
        }
        since: [6, 2]
    }
    
    ValueType{
        name: "QWebEngineFindTextResult"
        ModifyFunction{
            signature: "operator=(QWebEngineFindTextResult)"
            Delegate{
                name: "set"
                deprecated: true
            }
        }
        since: [5, 14]
    }
    
    ObjectType{
        name: "QWebEngineLoadingInfo"

        EnumType{
            name: "LoadStatus"
        }

        EnumType{
            name: "ErrorDomain"
        }
        ModifyFunction{
            signature: "operator=(QWebEngineLoadingInfo)"
            Delegate{
                name: "set"
                deprecated: true
            }
        }
        since: [6, 2]
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QWebEngineClientCertificateStore::QWebEngineClientCertificateStore', unmatched parameter type 'QtWebEngineCore::ClientCertificateStoreData*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QWebEngineNotification::QWebEngineNotification', unmatched parameter type 'const QSharedPointer<QtWebEngineCore::UserNotificationController>&'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QWebEngineRegisterProtocolHandlerRequest::QWebEngineRegisterProtocolHandlerRequest', unmatched parameter type 'QSharedPointer*"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: enum 'QWebEngineUrlScheme::SpecialPort' does not have a type entry or is not an enum"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QWebEngineUrlRequestJob::QWebEngineUrlRequestJob', unmatched parameter type 'QtWebEngineCore::URLRequestCustomJobDelegate*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QWebEngineQuotaRequest::QWebEngineQuotaRequest', unmatched parameter type 'QSharedPointer*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*', unmatched *type '*QtWebEngineCore::WebContentsAdapter*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*', unmatched *type '*QtWebEngineCore::CertificateErrorController*'"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QWebEngineClientCertificateSelection."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QWebEngineFileSystemAccessRequest::QWebEngineFileSystemAccessRequest', unmatched parameter type 'QSharedPointer*'"}
    SuppressedWarning{text: "WARNING(CppImplGenerator) :: Value type 'QWebEngineFullScreenRequest' is missing a default constructor. If possible, use CustomConstructor{} element to specify default construction."}
    SuppressedWarning{text: "WARNING(CppImplGenerator) :: Value type 'QWebEngineFileSystemAccessRequest' is missing a default constructor. If possible, use CustomConstructor{} element to specify default construction."}
}
