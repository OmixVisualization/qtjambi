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
    packageName: "io.qt.winextras"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtWinExtras"
    module: "qtjambi.winextras"
    description: "Provides platform-specific APIs for Windows."
    defaultPPCondition: "defined(Q_OS_WIN)"

    RequiredLibrary{
        name: "QtWidgets"
    }
    
    Rejection{
        className: "QWinMime"
    }
    
    Rejection{
        className: "QWindowsWindowFunctions::IsTabletModeType"
    }
    
    Rejection{
        className: "QWindowsWindowFunctions::SetHasBorderInFullScreen"
    }
    
    Rejection{
        className: "QWindowsWindowFunctions::SetTouchWindowTouchType"
    }
    
    Rejection{
        className: "QWindowsWindowFunctions::SetWindowActivationBehaviorType"
    }
    
    NamespaceType{
        name: "QtWin"

        EnumType{name: "HBitmapFormat"}
        EnumType{name: "WindowFlip3DPolicy"}

        ModifyFunction{
            signature: "setWindowFlip3DPolicy(QWidget*,QtWin::WindowFlip3DPolicy)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setWindowExcludedFromPeek(QWidget*,bool)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setWindowDisallowPeek(QWidget*,bool)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setWindowDisallowPeek(QWindow*,bool)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setWindowExcludedFromPeek(QWindow*,bool)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setWindowFlip3DPolicy(QWindow*,QtWin::WindowFlip3DPolicy)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "colorizationColor(bool *)"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool ok = false;\n"+
                                  "bool *%out = &ok;"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.winextras.QtWin$ColorizationColorInfo"
                }
                NoNullPointer{}
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtWinExtras::QtWin$ColorizationColorInfo::newInstance(%env, qtjambi_cast<jobject>(%env, %in), jboolean(ok));"}
                }
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiWinExtras.java"
                quoteAfterLine: "class QtWin__"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ObjectType{
        name: "QWinJumpList"
        ModifyFunction{
            signature: "addCategory(QWinJumpListCategory*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcCategories"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "addCategory(const QString &, const QList<QWinJumpListItem *>)"
            ModifyArgument{
                index: "return"
                ReferenceCount{
                    variableName: "__rcCategories"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "addCategory(const QString &, const QList<QWinJumpListItem *>)"
            ModifyArgument{
                index: 2
                ReplaceDefaultExpression{
                    expression: "new java.util.ArrayList<QWinJumpListItem>()"
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                Text{content: "if(__qt_return_value!=null){\n"+
                              "    for(QWinJumpListItem item : %2){\n"+
                              "        __qt_return_value.__add_to_reference_count(item);\n"+
                              "    }\n"+
                              "}"}
            }
        }
    }
    
    ObjectType{
        name: "QWinJumpListCategory"
        EnumType{name: "Type"}
        ModifyFunction{
            signature: "addItem(QWinJumpListItem*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcItems"
                    action: ReferenceCount.Add
                }
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiWinExtras.java"
                quoteAfterLine: "class QWinJumpListCategory__"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ObjectType{
        name: "QWinJumpListItem"
        EnumType{name: "Type"}
    }
    
    
    ObjectType{
        name: "QWinTaskbarButton"
        ModifyFunction{
            signature: "setWindow(QWindow*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "eventFilter(QObject*,QEvent*)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "QWinTaskbarProgress"
    }
    
    ObjectType{
        name: "QWinThumbnailToolBar"
        ModifyFunction{
            signature: "addButton(QWinThumbnailToolButton*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcToolButtons"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "removeButton(QWinThumbnailToolButton*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcToolButtons"
                    action: ReferenceCount.Take
                }
            }
        }
        ModifyFunction{
            signature: "setWindow(QWindow*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QWinThumbnailToolButton"
    }
    
    ObjectType{
        name: "QWinEvent"
    }
    
    ObjectType{
        name: "QWinColorizationChangeEvent"
    }
    
    ObjectType{
        name: "QWinCompositionChangeEvent"
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched * type 'HBITMAP'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched * type 'HRESULT'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched * type 'HDC'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched * type 'HRGN'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched * type 'HICON'"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: Either add or remove specified for reference count variable '__rcCategories' in 'QWinJumpList' but not both."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: Either add or remove specified for reference count variable '__rcItems' in 'QWinJumpListCategory' but not both."}
}
