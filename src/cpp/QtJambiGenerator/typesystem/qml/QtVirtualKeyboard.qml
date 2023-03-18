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
    packageName: "io.qt.keyboard"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtVirtualKeyboard"
    module: "qtjambi.virtualkeyboard"
    description: "A framework for implementing different input methods as well as a QML virtual keyboard. Supports localized keyboard layouts and custom visual themes."
    Rejection{
        className: "QVirtualKeyboardInputContextPrivate"
    }
    
    ObjectType{
        name: "QVirtualKeyboardAbstractInputMethod"
    }
    
    ObjectType{
        name: "QVirtualKeyboardExtensionPlugin"
    }
    
    ObjectType{
        name: "QVirtualKeyboardInputContext"
    }
    
    EnumType{
        name: "QVirtualKeyboardInputEngine::InputMode"
    }
    
    EnumType{
        name: "QVirtualKeyboardInputEngine::PatternRecognitionMode"
        RejectEnumValue{
            name: "PatternRecognitionDisabled"
        }
        RejectEnumValue{
            name: "HandwritingRecoginition"
        }
    }
    
    EnumType{
        name: "QVirtualKeyboardInputEngine::ReselectFlag"
    }
    
    EnumType{
        name: "QVirtualKeyboardInputEngine::TextCase"
    }
    
    ObjectType{
        name: "QVirtualKeyboardInputEngine"
        ModifyFunction{
            signature: "setInputMethod(QVirtualKeyboardAbstractInputMethod *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcInputMethod"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    EnumType{
        name: "QVirtualKeyboardSelectionListModel::DictionaryType"
    }
    
    EnumType{
        name: "QVirtualKeyboardSelectionListModel::Role"
        RejectEnumValue{
            name: "DisplayRole"
        }
        RejectEnumValue{
            name: "WordCompletionLengthRole"
        }
    }
    
    EnumType{
        name: "QVirtualKeyboardSelectionListModel::Type"
    }
    
    ObjectType{
        name: "QVirtualKeyboardSelectionListModel"
        ModifyFunction{
            signature: "setDataSource(QVirtualKeyboardAbstractInputMethod*,QVirtualKeyboardSelectionListModel::Type)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDataSource"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QVirtualKeyboardTrace"
    }
    
    ObjectType{
        name: "QVirtualKeyboardDictionary"
        since: [6, 1]
    }
    
    ObjectType{
        name: "QVirtualKeyboardDictionaryManager"
        ModifyFunction{
            signature: "createDictionary(QString)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        since: [6, 1]
    }
    
    ObjectType{
        name: "QVirtualKeyboardObserver"
        since: [6, 1]
    }
}
