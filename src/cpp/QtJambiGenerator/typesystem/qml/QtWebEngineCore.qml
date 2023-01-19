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
    defaultSuperClass: "io.qt.QtObject"
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
    
    Rejection{
        className: "QtWebEngineCore::WebContentsAdapter"
    }
    
    NamespaceType{
        name: "QtWebEngineCore"
        ExtraIncludes{
            Include{
                fileName: "qwebenginecontextmenurequest.h"
                location: Include.Global
            }
            since: [6, 2]
        }
    }
    
    GlobalFunction{
        signature: "qWebEngineChromiumSecurityPatchVersion()"
        targetType: "QtWebEngineCore"
        ModifyArgument{
            index: 0
            ReplaceType{
                modifiedType: "java.lang.String"
            }
        }
    }
    
    GlobalFunction{
        signature: "qWebEngineChromiumVersion()"
        targetType: "QtWebEngineCore"
        ModifyArgument{
            index: 0
            ReplaceType{
                modifiedType: "java.lang.String"
            }
        }
    }
    
    GlobalFunction{
        signature: "qWebEngineVersion()"
        targetType: "QtWebEngineCore"
        ModifyArgument{
            index: 0
            ReplaceType{
                modifiedType: "java.lang.String"
            }
        }
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
    
    Rejection{
        className: "QWebEngineUrlScheme"
        enumName: "SpecialPort"
    }
    
    EnumType{
        name: "QtWebEngineCore::ReferrerPolicy"
        RejectEnumValue{
            name: "Last"
        }
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEngineUrlRequestInfo::NavigationType"
    }
    
    EnumType{
        name: "QWebEngineUrlRequestInfo::ResourceType"
        RejectEnumValue{
            name: "ResourceTypeLast"
            since: [5, 14]
        }
    }
    
    EnumType{
        name: "QWebEngineUrlScheme::Flag"
        flags: "QWebEngineUrlScheme::Flags"
    }
    
    
    EnumType{
        name: "QWebEngineUrlScheme::Syntax"
    }
    
    ObjectType{
        name: "QWebEngineUrlRequestInfo"
    }
    
    ValueType{
        name: "QWebEngineQuotaRequest"
    }
    
    ValueType{
        name: "QWebEngineFileSystemAccessRequest"
        ModifyFunction{
            signature: "operator=(QWebEngineFileSystemAccessRequest)"
            remove: RemoveFlag.All
        }
        since: [6, 4]
    }
    
    EnumType{
        name: "QWebEngineFileSystemAccessRequest::HandleType"
        since: [6, 4]
    }
    
    EnumType{
        name: "QWebEngineFileSystemAccessRequest::AccessFlag"
        flags: "QWebEngineFileSystemAccessRequest::AccessFlags"
        since: [6, 4]
    }
    
    ObjectType{
        name: "QWebEngineCookieStore"
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
                    modifiedType: "java.util.function.Predicate<FilterRequest>"
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
    
    FunctionalType{
        name: "QWebEngineCookieStore::CookieFilter"
        generate: false
        using: "std::function<bool(const FilterRequest&)>"
    }
    
    Rejection{
        className: "QWebEngineCookieStore::FilterRequest"
        fieldName: "_reservedFlag"
    }
    
    Rejection{
        className: "QWebEngineCookieStore::FilterRequest"
        fieldName: "_reservedType"
    }
    
    ValueType{
        name: "QWebEngineCookieStore::FilterRequest"
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
    
    EnumType{
        name: "QWebEngineUrlRequestJob::Error"
    }
    
    ObjectType{
        name: "QWebEngineUrlRequestJob"
    }
    
    ValueType{
        name: "QWebEngineUrlScheme"
        ModifyFunction{
            signature: "operator=(QWebEngineUrlScheme)"
            rename: "set"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "void"
                }
            }
        }
    }
    
    EnumType{
        name: "QWebEngineHttpRequest::Method"
    }
    
    ValueType{
        name: "QWebEngineHttpRequest"
        ModifyFunction{
            signature: "operator=(QWebEngineHttpRequest)"
            rename: "set"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "void"
                }
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
        name: "QWebEngineClientCertificateStore"
        ppCondition: "QT_CONFIG(ssl)"
    }
    
    ObjectType{
        name: "QWebEngineNotification"
    }
    
    EnumType{
        name: "QWebEnginePage::Feature"
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEnginePage::FileSelectionMode"
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEnginePage::FindFlag"
        flags: "QWebEnginePage::FindFlags"
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEnginePage::JavaScriptConsoleMessageLevel"
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEnginePage::NavigationType"
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEnginePage::PermissionPolicy"
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEnginePage::RenderProcessTerminationStatus"
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEnginePage::WebAction"
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEnginePage::WebWindowType"
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEnginePage::LifecycleState"
        since: [6, 2]
    }
    
    FunctionalType{
        name: "QWebEnginePage::ScriptConsumer"
        generate: false
        using: "std::function<void(const QVariant&)>"
        since: [6, 2]
    }
    
    
    FunctionalType{
        name: "QWebEnginePage::StringConsumer"
        generate: false
        using: "std::function<void(const QString&)>"
        since: [6, 2]
    }
    
    FunctionalType{
        name: "QWebEnginePage::ByteArrayConsumer"
        generate: false
        using: "std::function<void(const QByteArray&)>"
        since: [6, 2]
    }
    
    FunctionalType{
        name: "QWebEnginePage::ResultConsumer"
        generate: false
        using: "std::function<void(const QWebEngineFindTextResult&)>"
        ExtraIncludes{
            Include{
                fileName: "QtWebEngineCore/QWebEngineFindTextResult"
                location: Include.Global
            }
        }
        since: [6, 2]
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
            signature: "findText(QString, QFlags<QWebEnginePage::FindFlag>, std::function<void(const QWebEngineFindTextResult &)>)"
            ModifyArgument{
                index: 3
                ReplaceDefaultExpression{
                    expression: "null"
                }
                ReplaceType{
                    modifiedType: "java.util.function.Consumer<Boolean>"
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
                    modifiedType: "java.util.function.Consumer<String>"
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
                    modifiedType: "java.util.function.Consumer<String>"
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
                    modifiedType: "java.util.function.Consumer<Object>"
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
            signature: "runJavaScript(QString, unsigned int, const std::function<void(const QVariant &)> &)"
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.util.function.Consumer<Object>"
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
            signature: "setWebChannel(QWebChannel*,uint)"
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
            signature: "proxyAuthenticationRequired(const QUrl&,QAuthenticator*, const QString&)"
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
            signature: "javaScriptPrompt(const QUrl &, const QString &, const QString &, QString *)"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.lang.String"
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
                    modifiedType: "java.util.function.Consumer<io.qt.core.QByteArray>"
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
    
    EnumType{
        name: "QWebEngineProfile::HttpCacheType"
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEngineProfile::PersistentCookiesPolicy"
        since: [6, 2]
    }
    
    ObjectType{
        name: "QWebEngineProfile"
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
                    modifiedType: "java.util.function.Consumer<QWebEngineNotification>"
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
    
    FunctionalType{
        name: "QWebEngineProfile::NotificationPresenter"
        generate: false
        using: "std::function<void(std::unique_ptr<QWebEngineNotification>)>"
        ExtraIncludes{
            Include{
                fileName: "QtWebEngineCore/QWebEngineNotification"
                location: Include.Global
            }
        }
        since: [6, 2]
    }
    
    FunctionalType{
        name: "QWebEngineProfile::IconAvailableCallback3"
        using: "std::function<void(const QIcon&,const QUrl&,const QUrl&)>"
        ExtraIncludes{
            Include{
                fileName: "QtWebEngineCore/QWebEngineNotification"
                location: Include.Global
            }
        }
        since: [6, 2]
    }
    
    FunctionalType{
        name: "QWebEngineProfile::IconAvailableCallback2"
        using: "std::function<void(const QIcon&,const QUrl&)>"
        ExtraIncludes{
            Include{
                fileName: "QtWebEngineCore/QWebEngineNotification"
                location: Include.Global
            }
        }
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEngineCertificateError::Error"
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEngineCertificateError::Type"
        since: [6, 2]
    }
    
    ValueType{
        name: "QWebEngineCertificateError"
        CustomConstructor{
            Text{content: "if(copy){\n"+
                          "    return new(placement) QWebEngineCertificateError(*copy);\n"+
                          "}else{\n"+
                          "    return nullptr;\n"+
                          "}"}
        }
        ModifyFunction{
            signature: "operator=(const QWebEngineCertificateError &)"
            remove: RemoveFlag.All
        }
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEngineScript::InjectionPoint"
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEngineScript::ScriptWorldId"
        since: [6, 2]
    }
    
    ValueType{
        name: "QWebEngineScript"
        ModifyFunction{
            signature: "operator=(QWebEngineScript)"
            rename: "set"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "void"
                }
            }
        }
        since: [6, 2]
    }
    
    ObjectType{
        name: "QWebEngineSettings"
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEngineSettings::FontFamily"
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEngineSettings::FontSize"
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEngineSettings::WebAttribute"
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEngineSettings::UnknownUrlSchemePolicy"
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
            rename: "set"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "void"
                }
            }
        }
        since: [6, 2]
    }
    
    ValueType{
        name: "QWebEngineClientCertificateSelection"
        CustomConstructor{
            Text{content: "if(copy){\n"+
                          "    return new(placement) QWebEngineClientCertificateSelection(*copy);\n"+
                          "}else{\n"+
                          "    return nullptr;\n"+
                          "}"}
        }
        ModifyFunction{
            signature: "operator=(QWebEngineClientCertificateSelection)"
            rename: "set"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "void"
                }
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
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEngineHistoryModel::Roles"
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
            rename: "set"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "void"
                }
            }
        }
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEngineContextMenuRequest::MediaType"
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEngineContextMenuRequest::MediaFlag"
        flags: "QWebEngineContextMenuRequest::MediaFlags"
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEngineContextMenuRequest::EditFlag"
        flags: "QWebEngineContextMenuRequest::EditFlags"
        since: [6, 2]
    }
    
    ObjectType{
        name: "QWebEngineContextMenuRequest"
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEngineDownloadRequest::DownloadState"
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEngineDownloadRequest::DownloadType"
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEngineDownloadRequest::DownloadInterruptReason"
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEngineDownloadRequest::SavePageFormat"
        since: [6, 2]
    }
    
    ObjectType{
        name: "QWebEngineDownloadRequest"
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEngineNavigationRequest::NavigationType"
        since: [6, 2]
    }
    
    ObjectType{
        name: "QWebEngineNavigationRequest"
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEngineNewWindowRequest::DestinationType"
        since: [6, 2]
    }
    
    ObjectType{
        name: "QWebEngineNewWindowRequest"
        since: [6, 2]
    }
    
    ValueType{
        name: "QWebEngineFindTextResult"
        ModifyFunction{
            signature: "operator=(QWebEngineFindTextResult)"
            rename: "set"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "void"
                }
            }
        }
        since: [5, 14]
    }
    
    EnumType{
        name: "QWebEngineLoadingInfo::LoadStatus"
        since: [6, 2]
    }
    
    EnumType{
        name: "QWebEngineLoadingInfo::ErrorDomain"
        since: [6, 2]
    }
    
    ObjectType{
        name: "QWebEngineLoadingInfo"
        ModifyFunction{
            signature: "operator=(QWebEngineLoadingInfo)"
            rename: "set"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "void"
                }
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
    SuppressedWarning{text: "WARNING(CppImplGenerator) :: Value type 'QWebEngineCertificateError' is missing a default constructor. The resulting C++ code will not compile. If possible, use <custom-constructor> tag to provide the constructors."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Type parser doesn't recognize the type *std::function<bool (const FilterRequest &)>* (is_busted)"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Type parser doesn't recognize the type *std::function<void (const QVariant &)>* (is_busted)"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Type parser doesn't recognize the type *std::function<void (const QByteArray &)>* (is_busted)"}
    SuppressedWarning{text: "WARNING(CppImplGenerator) :: Value type 'QWebEngineFullScreenRequest' is missing a default constructor. The resulting C++ code will not compile.*"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QWebEngineFileSystemAccessRequest::QWebEngineFileSystemAccessRequest', unmatched parameter type 'QSharedPointer*'"}
    SuppressedWarning{text: "WARNING(CppImplGenerator) :: Value type 'QWebEngineFileSystemAccessRequest' is missing a default constructor. The resulting C++ code will not compile. If possible, use * tag to provide the constructors."}
}
