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
    packageName: "io.qt.webengine.widgets"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtWebEngineWidgets"
    module: "qtjambi.webenginewidgets"
    description: "Provides C++ classes for rendering web content in a QWidget based application."
    Template{
        name: "web.comsumer.function"
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
    
    Template{
        name: "web.comsumer.QWebEngineCallback"
        Text{content: "QWebEngineCallback<%TYPE> %out;\n"+
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
        name: "QtQuickWidgets"
        mode: RequiredLibrary.Optional
    }
    
    RequiredLibrary{
        name: "QtPrintSupport"
    }
    
    EnumType{
        name: "QWebEngineCertificateError::Error"
        until: 5
    }
    
    EnumType{
        name: "QWebEngineDownloadItem::DownloadState"
        until: 5
    }
    
    EnumType{
        name: "QWebEngineDownloadItem::DownloadType"
        until: 5
    }
    
    EnumType{
        name: "QWebEngineDownloadItem::DownloadInterruptReason"
        until: 5
    }
    
    EnumType{
        name: "QWebEngineDownloadItem::SavePageFormat"
        until: 5
    }
    
    EnumType{
        name: "QWebEngineProfile::HttpCacheType"
        until: 5
    }
    
    EnumType{
        name: "QWebEngineProfile::PersistentCookiesPolicy"
        until: 5
    }
    
    EnumType{
        name: "QWebEngineScript::InjectionPoint"
        until: 5
    }
    
    EnumType{
        name: "QWebEngineScript::ScriptWorldId"
        until: 5
    }
    
    EnumType{
        name: "QWebEngineSettings::FontFamily"
        until: 5
    }
    
    EnumType{
        name: "QWebEngineSettings::FontSize"
        until: 5
    }
    
    EnumType{
        name: "QWebEngineSettings::WebAttribute"
        until: 5
    }
    
    EnumType{
        name: "QWebEngineSettings::UnknownUrlSchemePolicy"
        until: 5
    }
    
    EnumType{
        name: "QWebEngineContextMenuData::MediaType"
        until: 5
    }
    
    EnumType{
        name: "QWebEngineContextMenuData::MediaFlag"
        until: 5
    }
    
    EnumType{
        name: "QWebEngineContextMenuData::EditFlag"
        until: 5
    }
    
    ValueType{
        name: "QWebEngineCertificateError"
        CustomConstructor{
            Text{content: "if(copy){\n"+
                          "    return new(placement) QWebEngineCertificateError(*copy);\n"+
                          "}else{\n"+
                          "    return new(placement) QWebEngineCertificateError(0, QUrl(), false, QString());\n"+
                          "}"}
        }
        CustomConstructor{
            type: CustomConstructor.Default
            Text{content: "new(placement) QWebEngineCertificateError(0, QUrl(), false, QString());"}
        }
        until: 5
    }
    
    ObjectType{
        name: "QWebEngineDownloadItem"
        until: 5
    }
    
    ObjectType{
        name: "QWebEngineHistory"
        until: 5
    }
    
    ObjectType{
        name: "QWebEngineHistoryModel"
        until: 5
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
        until: 5
    }
    
    ValueType{
        name: "QWebEngineScript"
        ModifyFunction{
            signature: "operator=(QWebEngineScript)"
            Delegate{
                name: "set"
                deprecated: true
            }
        }
        until: 5
    }
    
    ValueType{
        name: "QWebEngineContextMenuData"
        ModifyFunction{
            signature: "operator=(QWebEngineContextMenuData)"
            Delegate{
                name: "set"
                deprecated: true
            }
        }
        until: 5
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
            Delegate{
                name: "set"
                deprecated: true
            }
        }
        until: 5
    }
    
    ObjectType{
        name: "QWebEnginePage"
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
            since: [5, 14]
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
            signature: "setView(QWidget*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcView"
                    action: ReferenceCount.Set
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
            signature: "createStandardContextMenu()"
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
                    signature: "fullScreenRequested(QWebEngineFullScreenRequest)"
                    ModifyArgument{
                        index: 1
                        ReplaceType{
                            modifiedType: "io.qt.webengine.widgets.QWebEngineFullScreenRequest"
                        }
                        NoNullPointer{
                        }
                        ConversionRule{
                            codeClass: CodeClass.Shell
                            Text{content: "%out = qtjambi_cast<jobject>(%env, &%in);"}
                        }
                        ConversionRule{
                            codeClass: CodeClass.Native
                            Text{content: "QWebEngineFullScreenRequest& %out  = *qtjambi_cast<QWebEngineFullScreenRequest*>(%env, %in);"}
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
            signature: "setWebChannel(QWebChannel*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcWebChannel"
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
            signature: "printToPdf(const QString &, const QPageLayout &)"
            ModifyArgument{
                index: 2
                ReplaceDefaultExpression{
                    expression: "new io.qt.gui.QPageLayout(new io.qt.gui.QPageSize(io.qt.gui.QPageSize.PageSizeId.A4), io.qt.gui.QPageLayout.Orientation.Portrait, new io.qt.core.QMarginsF())"
                }
            }
        }
        ModifyFunction{
            signature: "printToPdf(QWebEngineCallback, const QPageLayout &)"
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
                        name: "web.comsumer.QWebEngineCallback"
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
        ModifyFunction{
            signature: "findText(const QString &, QWebEnginePage::FindFlags, QWebEngineCallback)"
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
                        name: "web.comsumer.QWebEngineCallback"
                        Replace{
                            from: "%TYPE"
                            to: "bool"
                        }
                    }
                }
            }
        }
        ModifyFunction{
            signature: "toHtml(QWebEngineCallback)const"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
                ReplaceType{
                    modifiedType: "java.util.function.Consumer<String>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    InsertTemplate{
                        name: "web.comsumer.QWebEngineCallback"
                        Replace{
                            from: "%TYPE"
                            to: "const QString &"
                        }
                    }
                }
            }
        }
        ModifyFunction{
            signature: "toPlainText(QWebEngineCallback)const"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
                ReplaceType{
                    modifiedType: "java.util.function.Consumer<String>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    InsertTemplate{
                        name: "web.comsumer.QWebEngineCallback"
                        Replace{
                            from: "%TYPE"
                            to: "const QString &"
                        }
                    }
                }
            }
        }
        ModifyFunction{
            signature: "runJavaScript(const QString&,QWebEngineCallback)"
            ModifyArgument{
                index: 2
                NoNullPointer{
                }
                ReplaceType{
                    modifiedType: "java.util.function.Consumer<Object>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    InsertTemplate{
                        name: "web.comsumer.QWebEngineCallback"
                        Replace{
                            from: "%TYPE"
                            to: "const QVariant &"
                        }
                    }
                }
            }
        }
        ModifyFunction{
            signature: "runJavaScript(const QString&,quint32,QWebEngineCallback)"
            ModifyArgument{
                index: 3
                NoNullPointer{
                }
                ReplaceType{
                    modifiedType: "java.util.function.Consumer<Object>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    InsertTemplate{
                        name: "web.comsumer.QWebEngineCallback"
                        Replace{
                            from: "%TYPE"
                            to: "const QVariant &"
                        }
                    }
                }
            }
        }
        ModifyFunction{
            signature: "print(QPrinter *,QWebEngineCallback)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "java.util.function.Consumer<Boolean>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    InsertTemplate{
                        name: "web.comsumer.QWebEngineCallback"
                        Replace{
                            from: "%TYPE"
                            to: "bool"
                        }
                    }
                }
            }
        }
        until: 5
    }
    
    ObjectType{
        name: "QWebEngineFullScreenRequest"
        until: 5
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
            signature: "setRequestInterceptor(QWebEngineUrlRequestInterceptor*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcUrlRequestInterceptor"
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
            signature: "setNotificationPresenter(std::function<void(std::unique_ptr<QWebEngineNotification>)>)"
            ModifyArgument{
                index: 1
                ReplaceDefaultExpression{
                    expression: "null"
                }
                ReplaceType{
                    modifiedType: "java.util.function.@Nullable Consumer<io.qt.webengine.core.@NonNull QWebEngineNotification>"
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
        until: 5
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
        until: 5
    }
    
    ObjectType{
        name: "QWebEngineScriptCollection"
        until: 5
    }
    
    ObjectType{
        name: "QWebEngineSettings"
        until: 5
    }
    
    FunctionalType{
        name: "QWebEngineFullScreenRequest::BooleanConsumer"
        generate: false
        using: "std::function<void(bool)>"
        since: [6, 2]
    }
    
    FunctionalType{
        name: "QWebEngineView::BooleanConsumer"
        generate: false
        using: "std::function<void(bool)>"
        since: [6, 2]
    }
    
    FunctionalType{
        name: "QWebEngineView::DataConsumer"
        generate: false
        using: "std::function<void(const QByteArray&)>"
        since: [6, 2]
    }
    
    FunctionalType{
        name: "QWebEngineView::DataConsumer2"
        generate: false
        using: "std::function<void (const QByteArray&)>"
        since: [6, 2]
    }
    
    FunctionalType{
        name: "QWebEngineView::ResultConsumer"
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
        name: "QWebEngineView"
        ExtraIncludes{
            Include{
                fileName: "QtWebEngineCore/QWebEngineFindTextResult"
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
            signature: "setPage(QWebEnginePage*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
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
            signature: "createStandardContextMenu()"
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
            since: [6, 2]
        }
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JObjectWrapper"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "findText(QString, QWebEnginePage::FindFlags, std::function<void(const QWebEngineFindTextResult&)>)"
            ModifyArgument{
                index: 3
                ReplaceDefaultExpression{
                    expression: "null"
                }
                ReplaceType{
                    modifiedType: "java.util.function.@Nullable Consumer<io.qt.webengine.core.@NonNull QWebEngineFindTextResult>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    InsertTemplate{
                        name: "web.comsumer.function"
                        Replace{
                            from: "%TYPE"
                            to: "const QWebEngineFindTextResult &"
                        }
                    }
                }
            }
            since: [6, 2]
        }
        ModifyFunction{
            signature: "findText(const QString &, QWebEnginePage::FindFlags, QWebEngineCallback)"
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
                        name: "web.comsumer.QWebEngineCallback"
                        Replace{
                            from: "%TYPE"
                            to: "bool"
                        }
                    }
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "printToPdf(const QString &, const QPageLayout &,QPageRanges)"
            ModifyArgument{
                index: 2
                ReplaceDefaultExpression{
                    expression: "new io.qt.gui.QPageLayout(new io.qt.gui.QPageSize(io.qt.gui.QPageSize.PageSizeId.A4), io.qt.gui.QPageLayout.Orientation.Portrait, new io.qt.core.QMarginsF())"
                }
            }
            since: [6, 2]
        }
        ModifyFunction{
            signature: "printToPdf(std::function<void(const QByteArray&)>, const QPageLayout &,QPageRanges)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.util.function.@Nullable Consumer<io.qt.core.@NonNull QByteArray>"
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
            since: [6, 2]
        }
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QWebEngineScript::QWebEngineScript', unmatched parameter type '*QtWebEngineCore::UserScript*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QWebEngineContextMenuData::operator=', unmatched parameter type '*QWebEngineContextMenuData::QWebEngineContextDataPrivate*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QWebEngineClientCertificateSelection::QWebEngineClientCertificateSelection', unmatched parameter type '*QSharedPointer*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QWebEngineCertificateError::QWebEngineCertificateError', unmatched parameter type '*QSharedPointer<CertificateErrorController>*'"}
    SuppressedWarning{text: "WARNING(CppImplGenerator) :: Value type 'QWebEngineClientCertificateSelection' is missing a default constructor.*"}
}
