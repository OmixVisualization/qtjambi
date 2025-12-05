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
    packageName: "io.qt.webengine.core"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtWebEngineCore"
    module: "qtjambi.webenginecore"
    precompiledHeader: "pch_p.h"
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
        Text{content: String.raw`
std::function<void(%TYPE)> %out;
if(%in){
    %out = [wrapper = JObjectWrapper(%env, %in)](%TYPE value){
                    if(JniEnvironment env{200}){
                        QTJAMBI_TRY{
                            jobject _value = qtjambi_cast<jobject>(env, value);
                            Java::Runtime::Consumer::accept(env, wrapper.object(env), _value);
                        }QTJAMBI_CATCH(const JavaException& exn){
                            exn.report(env);
                        }QTJAMBI_TRY_END
                    }
                };
}`}
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
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: String.raw`
namespace QtWebEngineCore{
class FileSystemAccessPermissionRequestManagerQt{
public:
    static size_t hash(const QWebEngineFileSystemAccessRequest& value, size_t seed){
        return qHash(quintptr(&*value.d_ptr), seed);
    }
};
}

size_t qHash(const QWebEngineFileSystemAccessRequest& value, size_t seed = 0)
{
    return QtWebEngineCore::FileSystemAccessPermissionRequestManagerQt::hash(value, seed);
}`}
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
                    Text{content: String.raw`
std::function<bool(const QWebEngineCookieStore::FilterRequest &)> %out;
if(%in){
    %out = [wrapper = JObjectWrapper(%env, %in)](const QWebEngineCookieStore::FilterRequest & value) -> bool {
                    bool result{false};
                    if(JniEnvironment env{200}){
                        QTJAMBI_TRY{
                            jobject _value = qtjambi_cast<jobject>(env, value);
                            result = Java::Runtime::Predicate::test(env, wrapper.object(env), _value);
                        }QTJAMBI_CATCH(const JavaException& exn){
                            exn.report(env);
                        }QTJAMBI_TRY_END
                    }
                    return result;
                };
}`}
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
        name: "QWebEngineWebAuthUxRequest"
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
                invalidateAfterUse: true
                NoNullPointer{
                }
            }
        }
        ModifyFunction{
            signature: "proxyAuthenticationRequired(const QUrl&,QAuthenticator*, const QString&)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
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
                    Text{content: String.raw`
                        jstring %out = nullptr;
                        if(%in){
                            %out = qtjambi_cast<jstring>(%env, %4);
                        }`}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: String.raw`
                        bool %out = %in;
                        if(%out && %in){
                            *%4 = qtjambi_cast<QString>(%env, %in);
                        }`}
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
                    Text{content: String.raw`QString %in;
                                  QString* %out = &%in;`}
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

        ExtraIncludes{
            Include{
                fileName: "QtWebEngineCore/QWebEngineNotification"
                location: Include.Global
            }
            Include{
                fileName: "QtGui/QIcon"
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
                    Text{content: String.raw`
std::function<void(std::unique_ptr<QWebEngineNotification>)> %out;
if(%in){
    %out = [wrapper = JObjectWrapper(%env, %in)](std::unique_ptr<QWebEngineNotification> value){
                    if(JniEnvironment env{200}){
                        QTJAMBI_TRY{
                            QtJambiScope __qtjambi_scope;
                            jobject _value = qtjambi_cast<jobject>(env, __qtjambi_scope, value);
                            Java::Runtime::Consumer::accept(env, wrapper.object(env), _value);
                        }QTJAMBI_CATCH(const JavaException& exn){
                            exn.report(env);
                        }QTJAMBI_TRY_END
                    }
                };
}`}
                }
            }
        }

        FunctionalType{
            name: "IconAvailableCallback2"
            generate: false
            using: "std::function<void(const QIcon&,const QUrl&)>"
        }
        ModifyFunction{
            signature: "requestIconForIconURL(const QUrl &, int, std::function<void(const QIcon &, const QUrl &)>) const"
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.util.function.@Nullable BiConsumer<io.qt.gui.@NonNull QIcon, io.qt.core.@NonNull QUrl>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: String.raw`
std::function<void(const QIcon &, const QUrl &)> %out;
if(%in){
    %out = [wrapper = JObjectWrapper(%env, %in)](const QIcon & icon, const QUrl & url){
                    if(JniEnvironment env{200}){
                        QTJAMBI_TRY{
                            jobject _icon = qtjambi_cast<jobject>(env, icon);
                            jobject _url = qtjambi_cast<jobject>(env, url);
                            Java::Runtime::BiConsumer::accept(env, wrapper.object(env), _icon, _url);
                        }QTJAMBI_CATCH(const JavaException& exn){
                            exn.report(env);
                        }QTJAMBI_TRY_END
                    }
                };
}`}
                }
            }
        }

        FunctionalType{
            name: "IconAvailableCallback"
            generate: false
            using: "std::function<void(const QIcon&,const QUrl&,const QUrl&)>"
        }
        ModifyFunction{
            signature: "requestIconForPageURL(const QUrl &, int, std::function<void(const QIcon &, const QUrl &, const QUrl &)>) const"
            ModifyArgument{
                index: 3
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: String.raw`
std::function<void(const QIcon &, const QUrl &, const QUrl &)> %out;
if(%in){
    %out = [wrapper = JObjectWrapper(%env, %in)](const QIcon & icon, const QUrl & url, const QUrl & url2){
                    if(JniEnvironment env{200}){
                        QTJAMBI_TRY{
                            jobject _icon = qtjambi_cast<jobject>(env, icon);
                            jobject _url = qtjambi_cast<jobject>(env, url);
                            jobject _url2 = qtjambi_cast<jobject>(env, url2);
                            Java::QtWebEngineCore::QWebEngineProfile$IconAvailableCallback::accept(env, wrapper.object(env), _icon, _url, _url2);
                        }QTJAMBI_CATCH(const JavaException& exn){
                            exn.report(env);
                        }QTJAMBI_TRY_END
                    }
                };
}`}
                }
            }
        }
        InjectCode{
            target: CodeClass.Java
            Text{content: String.raw`
/**
 * <p>Java wrapper for Qt callable <code>std::function&lt;void(const QIcon&amp;,const QUrl&amp;,const QUrl&amp;)&gt;</code></p>
 */
@FunctionalInterface
public interface IconAvailableCallback {
    public void accept(io.qt.gui.@NonNull QIcon icon, io.qt.core.@NonNull QUrl url1, io.qt.core.@NonNull QUrl url2);
}
                `}
        }
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: String.raw`
namespace Java{
namespace QtWebEngineCore{
QTJAMBI_REPOSITORY_DECLARE_CLASS(QWebEngineProfile$IconAvailableCallback,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(accept))
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/webengine/core,QWebEngineProfile$IconAvailableCallback,
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(accept,(Lio/qt/gui/QIcon;Lio/qt/core/QUrl;Lio/qt/core/QUrl;)V)
                                )
}
}
                `}
        }
        since: [6, 2]
    }

    ObjectType{
        name: "QWebEngineProfileBuilder"
        ModifyFunction{
            signature: "createOffTheRecordProfile(QObject*)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "createProfile(const QString &, QObject *)const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        since: 6.9
    }
    
    ValueType{
        name: "QWebEngineCertificateError"
        EnumType{
            name: "Error"
        }
        EnumType{
            name: "Type"
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
            name: "ImageAnimationPolicy"
        }
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
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: String.raw`
class QWebEnginePagePrivate{
public:
    static const void* get(const QWebEngineFullScreenRequest &value) {
        return value.d_ptr.get();
    }
};
size_t qHash(const QWebEngineFullScreenRequest &value, size_t seed = 0){
    return qHash(QWebEnginePagePrivate::get(value), seed);
}
bool operator==(const QWebEngineFullScreenRequest& arg1, const QWebEngineFullScreenRequest& arg2){
    return QWebEnginePagePrivate::get(arg1)==QWebEnginePagePrivate::get(arg2);
}`}
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

    ObjectType{
        name: "QWebEngineDesktopMediaRequest"
        since: 6.7
    }
    ObjectType{
        name: "QWebEngineMediaSourceModel"
        since: 6.7
    }
    ObjectType{
        name: "QWebEngineWebAuthPinRequest"
        since: 6.7
    }
    ObjectType{
        name: "QWebEngineWebAuthUxRequest"
        EnumType{
            name: "WebAuthUxState"
        }
        EnumType{
            name: "PinEntryReason"
        }
        EnumType{
            name: "PinEntryError"
        }
        EnumType{
            name: "RequestFailureReason"
        }
        since: 6.7
    }
    ObjectType{
        name: "QWebEngineClientHints"
        since: 6.8
    }
    ObjectType{
        name: "QWebEngineFrame"
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
            signature: "printToPdf(QJSValue)"
            ModifyArgument{
                index: 1
                threadAffinity: true
                noImplicitCalls: true
            }
        }
        ModifyFunction{
            signature: "runJavaScript(QString,QJSValue)"
            ModifyArgument{
                index: 2
                threadAffinity: true
                noImplicitCalls: true
            }
        }
        ModifyFunction{
            signature: "runJavaScript(QString,quint32,QJSValue)"
            ModifyArgument{
                index: 3
                threadAffinity: true
                noImplicitCalls: true
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
            signature: "printToPdf(const std::function<void(const QByteArray &)> &)"
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
        }
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: String.raw`
class QWebEnginePagePrivate{
public:
    static inline size_t hash(const QWebEngineFrame &value, size_t seed) {
        return qHashMulti(seed, value.m_adapter.toStrongRef().get(), value.m_id);
    }
};
size_t qHash(const QWebEngineFrame &value, size_t seed = 0){
    return QWebEnginePagePrivate::hash(value, seed);
}`}
        }
        since: 6.8
    }

    GlobalFunction{
        signature: "qWebEngineProcessName()"
        targetType: "QtWebEngineCore"
        //since: 6.7
    }

    ObjectType{
        name: "QWebEnginePermission"
        EnumType{
            name: "Feature"
        }
        EnumType{
            name: "State"
        }
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: String.raw`
class QWebEnginePagePrivate{
public:
    static const void* get(const QWebEnginePermission &value) {
        return value.d_ptr.get();
    }
};
size_t qHash(const QWebEnginePermission &value, size_t seed = 0){
    return qHash(QWebEnginePagePrivate::get(value), seed);
}`}
        }
        since: 6.8
    }

    ObjectType{
        name: "QWebEngineExtensionInfo"
        since: [6,10]
    }

    ObjectType{
        name: "QWebEngineExtensionManager"
        since: [6,10]
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
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Final class 'QWebEngineDownloadRequest' set to non-final, as it is extended by other classes"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping signal 'QWebEnginePage::fileSystemAccessRequested(QWebEngineFileSystemAccessRequest) -> void', unmatched parameter type 'QWebEngineFileSystemAccessRequest'"; since: 6; until: 6.2}
}
