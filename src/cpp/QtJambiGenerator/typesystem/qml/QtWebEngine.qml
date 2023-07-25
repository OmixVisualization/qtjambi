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
    packageName: "io.qt.webengine"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtWebEngine"
    module: "qtjambi.webengine"
    description: "Classes and functions for embedding web content in applications using the Chromium browser project."
    until: 5
    
    ObjectType{
        name: "QQuickWebEngineProfile"
        Rejection{
            functionName: "settings"
        }
        EnumType{
            name: "HttpCacheType"
        }
        EnumType{
            name: "PersistentCookiesPolicy"
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
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "downloadRequested(QQuickWebEngineDownloadItem *)"
            privateSignal: true
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "? extends io.qt.core.@Nullable QObject"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "Q_UNUSED(in)\n"+
                                  "Java::Runtime::IllegalAccessException::throwNew(%env, \"Calling signal downloadRequested from Java not allowed.\" QTJAMBI_STACKTRACEINFO );\n"+
                                  "QQuickWebEngineDownloadItem* %out{nullptr};"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "%out = qtjambi_cast<jobject>(%env, reinterpret_cast<QObject*>(%in));"}
                }
            }
        }
        ModifyFunction{
            signature: "downloadFinished(QQuickWebEngineDownloadItem *)"
            privateSignal: true
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "? extends io.qt.core.@Nullable QObject"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "Q_UNUSED(in)\n"+
                                  "Java::Runtime::IllegalAccessException::throwNew(%env, \"Calling signal downloadFinished from Java not allowed.\" QTJAMBI_STACKTRACEINFO );\n"+
                                  "QQuickWebEngineDownloadItem* %out{nullptr};"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "%out = qtjambi_cast<jobject>(%env, reinterpret_cast<QObject*>(%in));"}
                }
            }
        }
    }
    
    ObjectType{
        name: "QQuickWebEngineScript"
        EnumType{
            name: "InjectionPoint"
        }
        EnumType{
            name: "ScriptWorldId"
        }
    }
    
    ObjectType{
        name: "QQuickWebEngineDownloadItem"
        generate: false
    }
    
    NamespaceType{
        name: "QtWebEngine"
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QQuickWebEngineProfile::*', unmatched parameter type 'QQuickWebEngineDownloadItem*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QQuickWebEngineProfile::*', unmatched parameter type 'QQuickWebEngineDownloadItem*'"}
}
