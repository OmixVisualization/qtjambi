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
    packageName: "io.qt.script.tools"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtScriptTools"
    module: "qtjambi.scripttools"
    description: "Additional components for applications that use Qt Script."

    RequiredLibrary{
        name: "QtWidgets"
    }
    
    RequiredLibrary{
        name: "QtScript"
    }
    
    EnumType{
        name: "QScriptEngineDebugger::DebuggerAction"
    }
    
    EnumType{
        name: "QScriptEngineDebugger::DebuggerState"
    }
    
    EnumType{
        name: "QScriptEngineDebugger::DebuggerWidget"
    }
    
    ObjectType{
        name: "QScriptEngineDebugger"
        ModifyFunction{
            signature: "createStandardToolBar(QWidget *)"
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
            signature: "createStandardMenu(QWidget *)"
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
            signature: "standardWindow() const"
            ModifyArgument{
                index: 0
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = QtJambiAPI::convertQObjectToJavaObject(%env, %in, \"io/qt/widgets/QMainWindow\");"}
                }
            }
        }
        ModifyFunction{
            signature: "createStandardToolBar(QWidget *)"
            ModifyArgument{
                index: 0
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = QtJambiAPI::convertQObjectToJavaObject(%env, %in, \"io/qt/widgets/QToolBar\");"}
                }
            }
        }
        ModifyFunction{
            signature: "createStandardMenu(QWidget *)"
            ModifyArgument{
                index: 0
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = QtJambiAPI::convertQObjectToJavaObject(%env, %in, \"io/qt/widgets/QMenu\");"}
                }
            }
        }
        ModifyFunction{
            signature: "widget(QScriptEngineDebugger::DebuggerWidget) const"
            ModifyArgument{
                index: 0
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = QtJambiAPI::convertQObjectToJavaObject(%env, %in, \"io/qt/widgets/QWidget\");"}
                }
            }
        }
        ModifyFunction{
            signature: "action(QScriptEngineDebugger::DebuggerAction) const"
            ModifyArgument{
                index: 0
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = QtJambiAPI::convertQObjectToJavaObject(%env, %in, \"io/qt/widgets/QAction\");"}
                }
            }
        }
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QHelpContentItem::QHelpContentItem', unmatched parameter type 'QHelpDBReader*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'changeEvent(QEvent * event)' in 'QHelpSearchQueryWidget'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'focusInEvent(QFocusEvent * focusEvent)' in 'QHelpSearchQueryWidget'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'changeEvent(QEvent * event)' in 'QHelpSearchResultWidget'"}
    SuppressedWarning{text: "WARNING(CppImplGenerator) :: protected function '*' in final class '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: visibility of function '*' modified in class '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: hiding of function '*' in class '*'"}
}
