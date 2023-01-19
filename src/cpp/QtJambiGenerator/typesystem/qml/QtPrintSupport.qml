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
    packageName: "io.qt.printsupport"
    defaultSuperClass: "io.qt.QtObject"
    qtLibrary: "QtPrintSupport"
    module: "qtjambi.printsupport"
    description: "Classes to make printing easier and more portable."

    Rejection{
        className: "QPrinter"
        functionName: "printerSelectionOption"
    }
    
    Rejection{
        className: "QPrinter"
        functionName: "setPrinterSelectionOption"
    }
    
    EnumType{
        name: "QAbstractPrintDialog::PrintDialogOption"
        flags: "QAbstractPrintDialog::PrintDialogOptions"
    }
    
    EnumType{
        name: "QAbstractPrintDialog::PrintRange"
    }
    
    EnumType{
        name: "QPrintPreviewWidget::ViewMode"
    }
    
    EnumType{
        name: "QPrintPreviewWidget::ZoomMode"
    }
    
    EnumType{
        name: "QPrintEngine::PrintEnginePropertyKey"
        ppCondition: "!defined(QT_NO_PRINTER)"
        RejectEnumValue{
            name: "PPK_PaperSize"
        }
    }
    
    EnumType{
        name: "QPrinter::ColorMode"
        ppCondition: "!defined(QT_NO_PRINTER)"
    }
    
    EnumType{
        name: "QPrinter::Orientation"
        ppCondition: "!defined(QT_NO_PRINTER)"
    }
    
    EnumType{
        name: "QPrinter::OutputFormat"
        ppCondition: "!defined(QT_NO_PRINTER)"
    }
    
    EnumType{
        name: "QPrinter::PageOrder"
        ppCondition: "!defined(QT_NO_PRINTER)"
    }
    
    EnumType{
        name: "QPrinter::PaperSource"
        ppCondition: "!defined(QT_NO_PRINTER)"
        RejectEnumValue{
            name: "Upper"
        }
        RejectEnumValue{
            name: "LastPaperSource"
        }
    }
    
    EnumType{
        name: "QPrinter::PrintRange"
        ppCondition: "!defined(QT_NO_PRINTER)"
    }
    
    EnumType{
        name: "QPrinter::PrinterMode"
        ppCondition: "!defined(QT_NO_PRINTER)"
    }
    
    EnumType{
        name: "QPrinter::PrinterState"
        ppCondition: "!defined(QT_NO_PRINTER)"
    }
    
    EnumType{
        name: "QPrinter::Unit"
        ppCondition: "!defined(QT_NO_PRINTER)"
    }
    
    EnumType{
        name: "QPrinter::DuplexMode"
        ppCondition: "!defined(QT_NO_PRINTER)"
    }
    
    ValueType{
        name: "QPrinterInfo"
        ppCondition: "!defined(QT_NO_PRINTER)"
        ModifyFunction{
            signature: "operator=(const QPrinterInfo &)"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "QAbstractPrintDialog"
    }
    
    ObjectType{
        name: "QPrintDialog"
        ModifyFunction{
            signature: "setVisible(bool)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "open(QObject*,const char*)"
            remove: RemoveFlag.All
        }
        InjectCode{
            target: CodeClass.ShellDeclaration
            position: Position.End
            Text{content: "#if !defined(Q_NO_USING_KEYWORD)\n"+
                          "    using QDialog::accepted;\n"+
                          "#else\n"+
                          "    inline void accepted(){\n"+
                          "        QDialog::accepted();\n"+
                          "    }\n"+
                          "#endif"}
        }
    }
    
    ObjectType{
        name: "QPlatformPrinterSupportPlugin"
    }
    
    ObjectType{
        name: "QPrintEngine"
        ppCondition: "!defined(QT_NO_PRINTER)"
    }
    
    ObjectType{
        name: "QPrinter"
        ppCondition: "!defined(QT_NO_PRINTER)"
        ModifyFunction{
            signature: "devType() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setEngines(QPrintEngine*,QPaintEngine*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcPrintEngine"
                    action: ReferenceCount.Set
                }
            }
            ModifyArgument{
                index: 2
                ReferenceCount{
                    variableName: "__rcPaintEngine"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "QPrinter(QPrinter::PrinterMode)"
            InjectCode{
                position: Position.Beginning
                Text{content: "if(io.qt.core.QCoreApplication.instance()==null)\n"+
                              "    throw new IllegalStateException(\"Cannot create QPrinter before initializing QCoreApplication.\");"}
            }
        }
        ModifyFunction{
            signature: "QPrinter(QPrinterInfo,QPrinter::PrinterMode)"
            InjectCode{
                position: Position.Beginning
                Text{content: "if(io.qt.core.QCoreApplication.instance()==null)\n"+
                              "    throw new IllegalStateException(\"Cannot create QPrinter before initializing QCoreApplication.\");"}
            }
        }
        ModifyFunction{
            signature: "getPageMargins(qreal*,qreal*,qreal*,qreal*,QPrinter::Unit)const"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "qreal left = 0;\n"+
                                  "qreal* %out = &left;"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "qreal top = 0;\n"+
                                  "qreal* %out = &top;"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "qreal right = 0;\n"+
                                  "qreal* %out = &right;"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "qreal bottom = 0;\n"+
                                  "qreal* %out = &bottom;"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.core.QMarginsF"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = qtjambi_cast<jobject>(%env, QMarginsF(left, top, right, bottom));"}
                }
            }
            until: 5
        }
    }
    
    ObjectType{
        name: "QPrintPreviewDialog"
        ModifyFunction{
            signature: "open(QObject*,const char*)"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "QPrintPreviewWidget"
        ModifyFunction{
            signature: "setVisible(bool)"
            threadAffinity: true
        }
    }
    
    
    ObjectType{
        name: "QPageSetupDialog"
        ModifyFunction{
            signature: "open(QObject*,const char*)"
            remove: RemoveFlag.All
        }
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Shadowing: QDialog::accepted() and QPrintDialog::accepted(); Java code will not compile"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QPrinterInfo."}
}
