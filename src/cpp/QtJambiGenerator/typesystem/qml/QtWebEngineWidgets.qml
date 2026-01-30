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
    packageName: "io.qt.webengine.widgets"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtWebEngineWidgets"
    module: "qtjambi.webenginewidgets"
    description: "Provides C++ classes for rendering web content in a QWidget based application."
    Template{
        name: "web.comsumer.function"
        Text{content: "std::function<void(%TYPE)> %out;\n"+
                      "if(%in){\n"+
                      "    %out = [wrapper = JObjectWrapper(%env, %in)](%TYPE value){\n"+
                      "                    if(JniEnvironment env{200}){\n"+
                      "                        QTJAMBI_TRY{\n"+
                      "                            jobject _value = qtjambi_cast<jobject>(env, value);\n"+
                      "                            Java::Runtime::Consumer::accept(env, wrapper.object(env), _value);\n"+
                      "                        }QTJAMBI_CATCH(const JavaException& exn){\n"+
                      "                            exn.report(env);\n"+
                      "                        }QTJAMBI_TRY_END\n"+
                      "                    }\n"+
                      "                };\n"+
                      "}"}
    }
    
    Template{
        name: "web.comsumer.QWebEngineCallback"
        Text{content: "QWebEngineCallback<%TYPE> %out;\n"+
                      "if(%in){\n"+
                      "    %out = [wrapper = JObjectWrapper(%env, %in)](%TYPE value){\n"+
                      "                    if(JniEnvironment env{200}){\n"+
                      "                        QTJAMBI_TRY{\n"+
                      "                            jobject _value = qtjambi_cast<jobject>(env, value);\n"+
                      "                            Java::Runtime::Consumer::accept(env, wrapper.object(env), _value);\n"+
                      "                        }QTJAMBI_CATCH(const JavaException& exn){\n"+
                      "                            exn.report(env);\n"+
                      "                        }QTJAMBI_TRY_END\n"+
                      "                    }\n"+
                      "                };\n"+
                      "}"}
    }
    
    RequiredLibrary{
        name: "QtQuickWidgets"
        mode: RequiredLibrary.Optional
    }
    
    RequiredLibrary{
        name: "QtPrintSupport"
    }
    
    ObjectType{
        name: "QWebEngineView"

        FunctionalType{
            name: "BooleanConsumer"
            generate: false
            using: "std::function<void(bool)>"
            since: [6, 2]
        }

        FunctionalType{
            name: "DataConsumer"
            generate: false
            using: "std::function<void(const QByteArray&)>"
            since: [6, 2]
        }

        FunctionalType{
            name: "DataConsumer2"
            generate: false
            using: "std::function<void (const QByteArray&)>"
            since: [6, 2]
        }

        FunctionalType{
            name: "ResultConsumer"
            generate: false
            using: "std::function<void(const QWebEngineFindTextResult&)>"
            since: [6, 2]
        }
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
