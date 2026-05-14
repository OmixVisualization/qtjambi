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
    packageName: "io.qt.webview"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtWebView"
    module: "qtjambi.webview"
    description: "Displays web content in a QML application by using APIs native to the platform, without the need to include a full web browser stack."
    RequiredLibrary{
        name: "QtWebEngineQuick"
        mode: RequiredLibrary.Supressed
        since: 6.8
    }
    RequiredLibrary{
        name: "QtWebEngineQuick"
        mode: RequiredLibrary.ProvideOnly
    }
    InjectCode{
        target: CodeClass.MetaInfo
        position: Position.Position1
        Text{content: String.raw`
#if defined(Q_OS_ANDROID)
void initialize_meta_info_QtWebView_android(JavaVM*);
#endif`}
    }
    InjectCode{
        target: CodeClass.MetaInfo
        position: Position.Beginning
        Text{content: String.raw`
#if defined(Q_OS_ANDROID)
    initialize_meta_info_QtWebView_android(%javaVM);
#else
    Q_UNUSED(%javaVM)
#endif`}
    }

    NamespaceType{
        name: "QtWebView"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QLibrary"
                location: Include.Global
                ppCondition: "defined(Q_OS_ANDROID)"
            }
            since: 6.8
        }
    }
    ObjectType{
        name: "QWebView"
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
            signature: "title()const"
            rename: "webViewTitle"
        }
        ModifyFunction{
            signature: "settings()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
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
        since: [6,11]
    }
    ValueType{
        name: "QWebViewLoadingInfo"
        EnumType{name: "LoadStatus"}
        since: [6,11]
    }
    ObjectType{
        name: "QWebViewSettings"
        forceFinal: true
        generate: "no-shell"
        EnumType{name: "WebAttribute"}
        since: [6,11]
    }
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QWebViewLoadingInfo."}
}
